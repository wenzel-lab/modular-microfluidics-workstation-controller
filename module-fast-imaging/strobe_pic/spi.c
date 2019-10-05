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
     * [STX U8=2][size U8][packet type U8][data...]
     */
    
    err rc = ERR_OK;
    uint8_t packet_size;
    uint8_t type;
    uint8_t stx;
    
    *packet_type = 0;
    
    while ( spi_read_bytes_available() && ( packet->buf_bytes < SPI_PACKET_BUF_SIZE ) )
    {
        if ( packet->buf_bytes > 0 )
            packet->buf[packet->buf_bytes++] = spi_read_byte();
        else
        {
            /* If start of packet, read until we find STX */
            
            stx = spi_read_byte();
            if ( stx == STX )
            {
                packet->buf[0] = STX;
                packet->buf_bytes = 1;
            }
        }
    }
    
    if ( packet->buf_bytes >= 3 )
    {
        /* We have at minimum: STX, size and packet type */
        
//        stx = packet->buf[0];
        packet_size = packet->buf[1];
        
        if ( ( packet_size > SPI_PACKET_BUF_SIZE ) || ( packet_size > data_buf_size ) )
            rc = ERR_PACKET_OVERFLOW;
        else if ( packet->buf_bytes >= packet_size )
        {
            type = packet->buf[2];
            
            if ( type == 0 )
                rc = ERR_PACKET_INVALID;
            else
            {
                *packet_type = type;
                *data_size = packet_size - 3;
                memcpy( data, &packet->buf[3], *data_size );
                
                if ( packet->buf_bytes == packet_size )
                    packet->buf_bytes = 0;
                else
                {
                    /* Copy over remaining data */
                    
                    uint8_t buf_bytes = packet->buf_bytes;
                    
                    packet->buf_bytes -= packet_size;
                    memcpy( packet->buf, &packet->buf[buf_bytes], packet->buf_bytes );
                }
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
