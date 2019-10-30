#include <xc.h>
#define FCY 8000000UL
#include <libpic30.h>
#include "mcc_generated_files/mcc.h"
#include "common.h"
#include "spi.h"

typedef enum
{
    DAC_CHAN_A      = 0b000,
    DAC_CHAN_B      = 0b001,
    DAC_CHAN_C      = 0b010,
    DAC_CHAN_D      = 0b011,
    DAC_CHAN_ALL    = 0b111,
} E_DAC_CHAN;

/* Packet Data */
spi_packet_buf_t spi_packet;
uint8_t packet_type;
uint8_t packet_data[SPI_PACKET_BUF_SIZE];
uint8_t packet_data_size;
uint8_t return_buf[9];

void dac_cmd( uint8_t cmd, E_DAC_CHAN chan, uint16_t value )
{
    /*
    struct
    {
        uint8_t chan : 3;
        uint8_t cmd : 3;
        uint8_t unused : 2;
        uint8_t value_high;
        uint8_t value_low;
    } data;
    */
    struct
    {
        uint8_t unused_byte;
        uint8_t value_low;
        uint8_t value_high;
        uint8_t chan : 3;
        uint8_t cmd : 3;
        uint8_t unused : 2;
    } data;
    
    data.cmd = cmd;
    data.chan = chan;
    data.value_high = value >> 8;
    data.value_low = value & 0xFF;
    
//    DAC_NSYNC_SetLow();
//    SPI2_Exchange8bitBuffer( (uint8_t *)&data, 3, NULL );
//    while ( SPI2STATLbits.SPITBE == 0 );
    SPI2_Exchange32bit( *(uint32_t *)&data );
//    DAC_NSYNC_SetHigh();
}

void dac_reset( void )
{
    dac_cmd( 0b101, 0, 0 );                 // Software reset
}

void dac_ref_internal( uint8_t internal )
{
    dac_cmd( 0b111, 0, internal ? 1 : 0 );  // Internal reference -> 5V range
}

void dac_write_and_update( E_DAC_CHAN dac_chan, uint16_t value )
{
    dac_cmd( 0b011, dac_chan, value );
}

void dac_write_ldac( E_DAC_CHAN dac_chan, uint16_t value, uint8_t update_all )
{
    dac_cmd( update_all ? 0b010 : 0b000, dac_chan, value );
}

int main(void)
{
    err rc = 0;
    uint16_t dac_val = 0;
    uint16_t adc_val = 0;
    uint8_t pina0 = 0;
    
    SYSTEM_Initialize();
    ADC1_SoftwareLevelTriggerEnable();
    spi_init();
    spi_packet_clear( &spi_packet );
    
    __delay_ms( 100 );

    dac_reset();
//    dac_ref_internal( 1 );
    
    while (1)
    {
        uint8_t dummy;
        
        /*
        adc_val = ADC1_SharedChannelAN2ConversionResultGet();
        
        dac_write_ldac( DAC_CHAN_A, dac_val << 4, 0 );
        dac_write_ldac( DAC_CHAN_B, adc_val << 4, 0 );
        dac_write_ldac( DAC_CHAN_C, dac_val << 4, 1 );
        
        dac_val++;
        dac_val &= 0xFFF;
        /**/
        
//        ADC1_IsSharedChannelAN2ConversionComplete()
//        ADC1_SharedChannelAN2ConversionResultGet();
//        ADCON3Lbits.CNVCHSEL = 0;   // Individual Channel Select
//        ADCON3Lbits.CNVRTCH = 1;    // Individual Channel Trigger
        
//        if ( spi_read_bytes_available() )
//            PORTAbits.RA0 ^= 1;

#if 0
//        SPI1STATL = 0;
//        dummy = SPI1BUFL;
//        SPI1BUFL = 10;
        
//        dummy = SPI1_Exchange8bit( 10 );
        
//        spi_read_byte();
//        spi_write_byte( 11 );
        
//        if ( spiabc == 1 )
//        if ( dummy != 0 )
//            PORTAbits.RA0 = 1;
        
//        PORTAbits.RA0 = spiabc;
//        PORTAbits.RA0 = spiabc ? 1 : 0;
        PORTAbits.RA0 = ( spi_read_bytes_available() > 12 ) ? 1 : 0;
#else
        if ( spi_packet_read( &spi_packet, &packet_type, (uint8_t *)&packet_data, &packet_data_size, SPI_PACKET_BUF_SIZE ) == ERR_OK )
        {
            switch ( packet_type )
            {
                case 0:
                {
                    /* No or invalid packet */
                    break;
                }
                default:
                    pina0 ^= 1;
                    LATAbits.LATA0 = pina0;
                    rc = 6;
                    spi_packet_write( 0x7, &rc, 1 );
            }
        }
#endif
    }
    
    return 1; 
}
