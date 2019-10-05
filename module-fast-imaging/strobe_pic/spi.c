#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <xc.h>
#include "common.h"
#include "spi.h"
#include "mcc_generated_files/spi1.h"

#define READ_BUF_SIZE           32
#define READ_BUF_SIZE_MASK      ( READ_BUF_SIZE - 1 )
#define WRITE_BUF_SIZE          32
#define WRITE_BUF_SIZE_MASK     ( WRITE_BUF_SIZE - 1 )

#define STX                     2

#ifdef SPI_READ_SUPPORTED
volatile uint8_t read_buf[READ_BUF_SIZE];
volatile uint8_t read_buf_head;
volatile uint8_t read_buf_tail;
volatile uint8_t read_buf_remaining;
#endif

#ifdef SPI_WRITE_SUPPORTED
volatile uint8_t write_buf[WRITE_BUF_SIZE];
volatile uint8_t write_buf_head;
volatile uint8_t write_buf_tail;
volatile uint8_t write_buf_remaining;
#endif

// Static Prototypes -------------------------------------------------------

static uint8_t spi_handler( uint8_t byte );

// Extern Functions --------------------------------------------------------

extern void spi_init( void )
{
#ifdef SPI_READ_SUPPORTED
    read_buf_head = 0;
    read_buf_tail = 0;
    read_buf_remaining = READ_BUF_SIZE;
#endif
    
    SPI1_setExchangeHandler( spi_handler );
}

#ifdef SPI_READ_SUPPORTED
extern uint8_t spi_read_bytes_available( void )
{
    return ( READ_BUF_SIZE - read_buf_remaining );
}
#endif

#ifdef SPI_READ_SUPPORTED
extern uint8_t spi_read_byte( void )
{
    uint8_t byte = 0;
    
    if ( READ_BUF_SIZE != read_buf_remaining )
    {
        byte = read_buf[read_buf_tail];
        read_buf_tail = ( read_buf_tail + 1 ) & READ_BUF_SIZE_MASK;
        read_buf_remaining++;
    }
    
    return byte;
}
#endif

extern void spi_packet_clear( spi_packet_buf_t *packet )
{
    packet->buf_bytes = 0;
}

#ifdef SPI_READ_SUPPORTED
extern err spi_packet_read( spi_packet_buf_t *packet, uint8_t *packet_type, uint8_t *data, uint8_t *data_size, uint8_t data_buf_size )
{
    /* Returns packet type if good packet, 0 otherwise */
    
    /* Packet format:
     * [STX U8=2][size U8][packet type U8][data...][checksum U8]
     */
    
    err rc = ERR_OK;
    uint8_t invalidate = 0;
    uint8_t packet_size;
    uint8_t type;
    uint8_t checksum;
    uint8_t *data_ptr;
    uint8_t *buf_ptr;
    uint8_t i;
    
    *packet_type = 0;
    
    /* Read until we find STX. */
    while ( ( packet->buf_bytes == 0 ) && spi_read_bytes_available() )
    {
        if ( spi_read_byte() == STX )
        {
            packet->buf[0] = STX;
            packet->buf_bytes = 1;
        }
    }
    
    if ( packet->buf_bytes > 0 )
    {
        /* We have an STX already. Keep reading. */
        while ( spi_read_bytes_available() && ( packet->buf_bytes < SPI_PACKET_BUF_SIZE ) )
            packet->buf[packet->buf_bytes++] = spi_read_byte();

        if ( packet->buf_bytes >= 3 )
        {
            /* We have at minimum: STX, size and packet type */

            packet_size = packet->buf[1];

            if ( ( packet_size > SPI_PACKET_BUF_SIZE ) || ( packet_size > data_buf_size ) )
            {
                rc = ERR_PACKET_OVERFLOW;
                invalidate = 1;
            }
            else if ( packet_size < 4 )
            {
                /* Packet size is too small to be valid. */
                rc = ERR_PACKET_INVALID;
                invalidate = 1;
            }
            else if ( packet->buf_bytes >= packet_size )
            {
                type = packet->buf[2];
                *data_size = packet_size - 4;
                checksum = STX + packet_size + type + packet->buf[packet_size-1];
                data_ptr = data;
                buf_ptr = &packet->buf[3];

                /* Calculate checksum and copy over data. */
                for ( i=0; i<*data_size; i++ )
                {
                    checksum += *buf_ptr;
                    *data_ptr++ = *buf_ptr++;
                }

                if ( checksum != 0 )
                {
                    /* Bad checksum. Search until we find next STX. */
                    rc = ERR_PACKET_INVALID;
                    invalidate = 1;
                }
                else
                {
                    /* Checksum is good. */

                    if ( packet->buf_bytes == packet_size )
                        packet->buf_bytes = 0;
                    else
                    {
                        /* Shift up remaining data */

                        uint8_t buf_bytes = packet->buf_bytes;
                        packet->buf_bytes -= packet_size;
                        memcpy( packet->buf, &packet->buf[buf_bytes], packet->buf_bytes );
                    }

                    if ( type == 0 )
                        rc = ERR_PACKET_INVALID;
                    else
                        *packet_type = type;
                }
            }

            if ( invalidate )
            {
                /* Data in buffer is invalid -> look for next STX. */

                buf_ptr = &packet->buf[1];
                for ( i=1; i<packet->buf_bytes; i++ )
                    if ( packet->buf[i] == STX )
                        break;

                packet->buf_bytes -= i;
                memcpy( packet->buf, &packet->buf[i], packet->buf_bytes );
            }
        }
    }
    
    return rc;
}
#endif

// Static Functions --------------------------------------------------------

static uint8_t spi_handler( uint8_t byte )
{
#ifdef SPI_READ_SUPPORTED
    if ( read_buf_remaining )
    {
        read_buf[read_buf_head] = byte;
        read_buf_head = ( read_buf_head + 1 ) & READ_BUF_SIZE_MASK;
        read_buf_remaining--;
    }
#endif
    
	PIR3bits.SSP1IF = 0;
    
    return 0;
}
