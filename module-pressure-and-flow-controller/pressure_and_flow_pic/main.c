#include <xc.h>
#define FCY 75000000UL
//#define FCY 8000000UL
#include <libpic30.h>
#include <string.h>
#include "mcc_generated_files/mcc.h"
#include "common.h"
#include "spi.h"
#include "ads1115.h"

/* Flow / Pressure Constants */
#define NUM_PRESSURE_CLTRLS                 4
#define PRESSURE_SHL                        3
//#define PRESSURE_ADC_REF_MV                 3300
//#define PRESSURE_ADC_MBAR                   5000
#define PRESSURE_ADC_REF_MV                 6144
#define PRESSURE_ADC_BITRES                 15
#define PRESSURE_CTLR_REF_MV                5000
#define PRESSURE_CTLR_MBAR                  5000
#define PRESSURE_ADC_SCALE                  ( (uint32_t)PRESSURE_ADC_REF_MV * PRESSURE_CTLR_MBAR / PRESSURE_CTLR_REF_MV )

/* Flow / Pressure Macros */
#define ADC_CHAN_MAX                        ( NUM_PRESSURE_CLTRLS - 1 )
#define ADC_PERIOD_MS                       100
#define PRESSURE_ADC_TO_MBARSHL(adc)        ( ( (uint32_t)adc * PRESSURE_ADC_SCALE ) >> ( PRESSURE_ADC_BITRES - PRESSURE_SHL ) )
//#define PRESSURE_ADC_TO_MBARSHL(adc)        ( ( (uint32_t)adc << PRESSURE_SHL ) * PRESSURE_ADC_MBAR / PRESSURE_ADC_REF_MV )
//#define PRESSURE_ADC_TO_MBAR(adc)        ( (uint32_t)adc * PRESSURE_ADC_MBAR / PRESSURE_ADC_REF_MV )

/* Comms Constants */
#define PACKET_TYPE_GET_ID                  1
#define PACKET_TYPE_SET_PRESSURE_TARGET     2
#define PACKET_TYPE_GET_PRESSURE_TARGET     3
#define PACKET_TYPE_GET_PRESSURE_ACTUAL     4

/* DAC Constants */
typedef enum
{
    DAC_CHAN_A      = 0b000,
    DAC_CHAN_B      = 0b001,
    DAC_CHAN_C      = 0b010,
    DAC_CHAN_D      = 0b011,
    DAC_CHAN_ALL    = 0b111,
} E_DAC_CHAN;

typedef enum
{
    ADC_STATE_START,
    ADC_STATE_SAMPLE,
    ADC_STATE_WAIT,
} E_ADC_STATE;

/* System Data */
uint8_t device_id[] = "MICROFLOW";
volatile uint16_t timer_ms;

/* Flow / Pressure Data */
volatile uint16_t pressure_mbar_shl_actual[NUM_PRESSURE_CLTRLS];
uint16_t pressure_mbar_shl_target[NUM_PRESSURE_CLTRLS];
E_ADC_STATE adc_state;
uint8_t adc_go = 0;
uint8_t adc_chan;
uint16_t adc_time;
ads1115_datarate adc_datarate = DATARATE_128SPS;
ads1115_fsr_gain adc_gain = FSR_6_144;
uint8_t adc_i2c_addr = ADS1115_ADDR_GND;
ads1115_task_t adc_task;

/* Packet Data */
spi_packet_buf_t spi_packet;
uint8_t packet_type;
uint8_t packet_data[SPI_PACKET_BUF_SIZE];
uint8_t packet_data_size;

void dac_cmd( uint8_t cmd, E_DAC_CHAN chan, uint16_t value )
{
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
    
    SPI2_Exchange32bit( *(uint32_t *)&data );
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

void capture_pressures( void )
{
//    uint8_t i;
    
    ads1115_start_single( adc_i2c_addr, 0, DATARATE_128SPS, FSR_6_144 );
    __delay_ms( 8 );
    pressure_mbar_shl_actual[0] = PRESSURE_ADC_TO_MBARSHL( ads1115_get_result( adc_i2c_addr ) );
    
//    for ( i=0; i<NUM_PRESSURE_CLTRLS; i++ )
//        pressure_mbar_shl_actual[i] = PRESSURE_ADC_TO_MBARSHL( ads1115_readADC_SingleEnded( adc_i2c_addr, i, DATARATE_128SPS, FSR_6_144 ) );
//    pressure_mbar_shl_actual[0] = PRESSURE_ADC_TO_MBARSHL( ads1115_readADC_SingleEnded( adc_i2c_addr, 0, DATARATE_128SPS, FSR_6_144 ) );
    
/*    
//    while ( !ADC1_IsSharedChannelAN2ConversionComplete() );
    pressure_mbar_shl_actual[0] = PRESSURE_ADC_TO_MBARSHL( ADC1_SharedChannelAN2ConversionResultGet() );
//    while ( !ADC1_IsSharedChannelAN3ConversionComplete() );
    pressure_mbar_shl_actual[1] = PRESSURE_ADC_TO_MBARSHL( ADC1_SharedChannelAN3ConversionResultGet() );
//    while ( !ADC1_IsSharedChannelAN4ConversionComplete() );
    pressure_mbar_shl_actual[2] = PRESSURE_ADC_TO_MBARSHL( ADC1_SharedChannelAN4ConversionResultGet() );
//    while ( !ADC1_IsSharedChannelAN9ConversionComplete() );
    pressure_mbar_shl_actual[3] = PRESSURE_ADC_TO_MBARSHL( ADC1_SharedChannelAN9ConversionResultGet() );
//    ADC1_SoftwareTriggerEnable();
 */
}

void retrigger_ADCs( void )
{
    if ( ADC1_IsSharedChannelAN9ConversionComplete() )
        ADC1_SoftwareTriggerEnable();
}

void set_pressures( void )
{
    dac_write_ldac( DAC_CHAN_A, pressure_mbar_shl_target[0], 0 );
    dac_write_ldac( DAC_CHAN_B, pressure_mbar_shl_target[1], 0 );
    dac_write_ldac( DAC_CHAN_C, pressure_mbar_shl_target[2], 0 );
    dac_write_ldac( DAC_CHAN_D, pressure_mbar_shl_target[3], 1 );
}

err parse_packet_get_id( uint8_t packet_type, uint8_t *packet_data, uint8_t packet_data_size )
{
    err rc = ERR_OK;
    uint8_t i;
    uint8_t return_buf[ sizeof(err) + sizeof(device_id)];
    return_buf[0] = ERR_OK;
    for ( i=0; i<sizeof(device_id); i++ )
        return_buf[i+1] = device_id[i];
//    memcpy( &return_buf[1], device_id, sizeof(device_id) );
    
    spi_packet_write( packet_type, return_buf, sizeof(return_buf) );
    
    return rc;
}

err parse_packet_set_pressure_target( uint8_t packet_type, uint8_t *packet_data, uint8_t packet_data_size )
{
    /* Data: n*[ [Controller Mask U8][Pressure mbar U16>>3] ] */

    err rc = ERR_OK;
    uint8_t *press_data_ptr = packet_data;
    uint8_t press_chan_mask;
    uint8_t press_chan;
    uint16_t pressures_mbar[NUM_PRESSURE_CLTRLS];
    uint16_t pressure_mbar;
    int16_t data_size = packet_data_size;
    
    while ( data_size >= ( 1 + sizeof(pressure_mbar) ) )
    {
        /* U8 mask + U16 flow */
        press_chan_mask = *press_data_ptr++;
        pressure_mbar = ( press_data_ptr[1] << 8 ) | press_data_ptr[0];
        press_data_ptr += sizeof(pressure_mbar);
        data_size -= ( 1 + sizeof(pressure_mbar) );

        for ( press_chan=0; press_chan<NUM_PRESSURE_CLTRLS; press_chan++ )
        {
            if ( press_chan_mask & 0x01 )
                pressures_mbar[press_chan] = pressure_mbar;
            press_chan_mask >>= 1;
        }
    }
    
    if ( data_size != 0 )
        rc = ERR_PACKET_INVALID;
    else
    {
        memcpy( pressure_mbar_shl_target, pressures_mbar, sizeof(pressures_mbar) );
        set_pressures();
        spi_packet_write( packet_type, &rc, 1 );
    }
    
    return rc;
}

err parse_packet_get_pressure_actual( uint8_t packet_type, uint8_t *packet_data, uint8_t packet_data_size )
{
    /* Return: [err U8]4x[Pressure mbar U16>>3] */
    
    err rc = ERR_OK;
    uint8_t i;
    uint8_t return_buf[ sizeof(err) + (NUM_PRESSURE_CLTRLS*sizeof(uint16_t)) ];
    uint8_t *return_buf_ptr;
    
    return_buf_ptr = return_buf;
    *return_buf_ptr++ = ERR_OK;
    
//    capture_pressures();
//    ads1115_start_single( adc_i2c_addr, 0, DATARATE_128SPS, FSR_6_144 );
    
    for ( i=0; i<NUM_PRESSURE_CLTRLS; i++ )
    {
        *(uint16_t *)return_buf_ptr = pressure_mbar_shl_actual[i];
        return_buf_ptr += sizeof(uint16_t);
    }
    
    spi_packet_write( packet_type, return_buf, sizeof(return_buf) );
    
    return rc;
}

void init( void )
{
    memset( pressure_mbar_shl_target, 0, sizeof(pressure_mbar_shl_target) );
    memset( pressure_mbar_shl_actual, 0, sizeof(pressure_mbar_shl_actual) );
    
    adc_state = ADC_STATE_START;
    adc_chan = 0;
    adc_time = 0;
    timer_ms = 0;
}

void adc_rdy_isr( void )
{
    /*
    switch ( adc_state )
    {
        case ADC_STATE_INIT:
            ads1115_start_single( adc_i2c_addr, 0, DATARATE_128SPS, FSR_6_144 );
            adc_state = ADC_STATE_SAMPLE;
            break;
        case ADC_STATE_SAMPLE:
            pressure_mbar_shl_actual[0] = PRESSURE_ADC_TO_MBARSHL( ads1115_get_result( adc_i2c_addr ) );
            ads1115_start_single( adc_i2c_addr, 0, DATARATE_128SPS, FSR_6_144 );
            break;
        default:;
    }
    */
    adc_go = 1;
}

void __attribute__ ((weak)) timer_isr(void)
{
//    PORTAbits.RA1 = ( ( !PORTAbits.RA1 ) && OSCCONbits.LOCK ) ? 1 : 0;
//    PORTAbits.RA1 = !PORTAbits.RA1;
//    PORTAbits.RA1 = OSCCONbits.LOCK ? 1 : 0;
//    PORTAbits.RA1 = OSCCONbits.OSWEN;
    timer_ms++;
}

int main(void)
{
    err rc = 0;
    bool adc_i2c_wait;
    
    SYSTEM_Initialize();
    init();
    
    __delay_ms( 100 );
    
    /* Init Timers */
    TMR1_SetInterruptHandler( &timer_isr );
    
    /* Init ADC */
    ads1115_set_ready_pin( adc_i2c_addr );
//    ADC_RDY_SetInterruptHandler( adc_rdy_isr );
//    ads1115_get_result( adc_i2c_addr );
//    ads1115_start_single( adc_i2c_addr, 0, DATARATE_128SPS, FSR_6_144 );
//    ADC1_SoftwareLevelTriggerEnable();
//    ADC1_SoftwareTriggerEnable();
    
    /* Init DAC */
    dac_reset();
//    dac_ref_internal( 1 );
    set_pressures();
    
    /* Init SPI */
    spi_init();
    spi_packet_clear( &spi_packet );
    
    __delay_ms( 100 );

    adc_time = timer_ms;
    adc_i2c_wait = 0;
    
    while (1)
    {
        if ( I2C3_Aborted() )
        {
            adc_state = ADC_STATE_WAIT;
            adc_i2c_wait = 0;
        }
        
        if ( adc_i2c_wait )
        {
            int8_t adc_rc;
            uint16_t adc_value;
            int8_t channel;
            
            /* See what's happening on ADC I2C */
            adc_rc = ads1115_read_adc_return( &adc_value, &channel, &adc_task );

            switch ( adc_rc )
            {
                case 0:
                {
                    /* Still waiting */
                    break;
                }
                case 1:
                {
                    /* I2C success */
                    adc_i2c_wait = 0;
                    
                    if ( channel >= 0 )
                    {
                        /* Value returned */
                        pressure_mbar_shl_actual[channel] = PRESSURE_ADC_TO_MBARSHL( adc_value );
                    }
                    
                    break;
                }
                case -1:
                {
                    /* Timeout */
                    adc_state = ADC_STATE_WAIT;
                    adc_i2c_wait = 0;
                    break;
                }
                default:
                    ;
            }
        }
        else switch ( adc_state )
        {
            case ADC_STATE_START:
            {
                adc_chan = 0;
                ads1115_read_adc_start( adc_i2c_addr, -1, adc_chan, adc_datarate, adc_gain, &adc_task );
                adc_i2c_wait = 1;
                adc_state = ADC_STATE_SAMPLE;
                break;
            }
            case ADC_STATE_SAMPLE:
            {
                if ( !ADC_RDY_GetValue() )
                {
                    if ( adc_chan >= ADC_CHAN_MAX )
                    {
                        /* Read last channel */
                        ads1115_read_adc_start( adc_i2c_addr, adc_chan, -1, adc_datarate, adc_gain, &adc_task );
                        adc_state = ADC_STATE_WAIT;
                    }
                    else
                    {
                        /* Read and start next */
                        uint8_t read_chan = adc_chan;
                        adc_chan++;
                        ads1115_read_adc_start( adc_i2c_addr, read_chan, adc_chan, adc_datarate, adc_gain, &adc_task );
                    }
                    
                    adc_i2c_wait = 1;
                }
                
                break;
            }
            case ADC_STATE_WAIT:
            {
                while ( ( timer_ms - adc_time ) > ADC_PERIOD_MS )
                {
                    adc_state = ADC_STATE_START;
                    adc_time += ADC_PERIOD_MS;
                }
                break;
            }
            default:
                adc_state = ADC_STATE_START;
        }

        if ( ( spi_packet_read( &spi_packet, &packet_type, (uint8_t *)&packet_data, &packet_data_size, SPI_PACKET_BUF_SIZE ) == ERR_OK ) &&
             ( packet_type != 0 ) )
        {
            rc = ERR_OK;
            
            switch ( packet_type )
            {
                case 0:
                {
                    /* No or invalid packet */
                    rc = ERR_PACKET_INVALID;
                    break;
                }
                case PACKET_TYPE_GET_ID:
                {
                    rc = parse_packet_get_id( packet_type, packet_data, packet_data_size );
                    break;
                }
                case PACKET_TYPE_SET_PRESSURE_TARGET:
                {
                    rc = parse_packet_set_pressure_target( packet_type, packet_data, packet_data_size );
                    break;
                }
                case PACKET_TYPE_GET_PRESSURE_TARGET:
                {
                    break;
                }
                case PACKET_TYPE_GET_PRESSURE_ACTUAL:
                {
                    rc = parse_packet_get_pressure_actual( packet_type, packet_data, packet_data_size );
                    break;
                }
                default:
                    rc = ERR_PACKET_INVALID;
            }
            
            if ( rc != ERR_OK )
                spi_packet_write( packet_type, &rc, 1 );
        }
        
//        retrigger_ADCs();
    }
    
    return 1; 
}
