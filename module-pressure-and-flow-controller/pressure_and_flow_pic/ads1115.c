#include "mcc_generated_files/mcc.h"
#include "common.h"
#include "ads1115.h"

#define I2C_TIMEOUT_MS      2

uint8_t ads1115_write_register(uint8_t addr, uint8_t reg, uint16_t data)
{
    volatile I2C3_MESSAGE_STATUS status;
    uint8_t writeBuffer[3];
    uint16_t time;
    
    writeBuffer[0] = reg;
    writeBuffer[1] = data >> 8;
    writeBuffer[2] = data & 0xFF;
    
    I2C3_MasterWrite( writeBuffer, 3, addr, (I2C3_MESSAGE_STATUS *)&status );
    
    time = timer_ms;
    while ( ( status != I2C3_MESSAGE_COMPLETE ) && ( ( timer_ms - time ) <= I2C_TIMEOUT_MS ) );
    if ( status != I2C3_MESSAGE_COMPLETE )
        I2C3_Abort();
    
	return 0;
}

uint16_t ads1115_read_register(uint8_t addr, uint8_t reg)
{
    uint8_t buf[2];
    volatile I2C3_MESSAGE_STATUS status;
    I2C3_TRANSACTION_REQUEST_BLOCK trBlocks[2];
    uint16_t time;

    I2C3_MasterWriteTRBBuild( &trBlocks[0], &reg, 1, addr );
    I2C3_MasterReadTRBBuild( &trBlocks[1], (void *)buf, 2, addr );
    I2C3_MasterTRBInsert( 2, (I2C3_TRANSACTION_REQUEST_BLOCK *)&trBlocks, (I2C3_MESSAGE_STATUS *)&status );
    
    time = timer_ms;
    while ( ( status != I2C3_MESSAGE_COMPLETE ) && ( ( timer_ms - time ) <= I2C_TIMEOUT_MS ) );
    if ( status != I2C3_MESSAGE_COMPLETE )
        I2C3_Abort();
    
    return ( ( (uint16_t)(buf[0]) ) << 8 ) | buf[1];
}

void ads1115_set_ready_pin( uint8_t addr )
{
    ads1115_write_register( addr, ADS1115_REG_HI_THRESH, 0b10000000 );
    ads1115_write_register( addr, ADS1115_REG_LO_THRESH, 0b00000000 );
}

uint16_t ads1115_read_adc_next( uint8_t addr, uint8_t read, int8_t start_channel, ads1115_datarate dr, ads1115_fsr_gain gain )
{
    uint8_t buf[2];
    volatile I2C3_MESSAGE_STATUS status;
    I2C3_TRANSACTION_REQUEST_BLOCK trBlocks[3];
    uint8_t read_reg = ADS1115_REG_CONVERSION;
    uint8_t writeBuffer[3];
    uint16_t adc_config;
    uint8_t trb_count;
    uint16_t time;
    
    /** Clear I2C buffer */
    
    trb_count = 0;
    
    if ( read )
    {
        /* Read conversion */
        
        I2C3_MasterWriteTRBBuild( &trBlocks[trb_count++], &read_reg, 1, addr );
        I2C3_MasterReadTRBBuild( &trBlocks[trb_count++], buf, 2, addr );
    }
    
    if ( start_channel >= 0 )
    {
        /* Start new conversion */
        
        adc_config =    ADS1115_COMP_QUE_CON1 |
                        ADS1115_COMP_LAT_Latching |
                        ADS1115_COMP_POL_3_ACTIVELOW |
                        ADS1115_COMP_MODE_TRADITIONAL |
                        dr |
                        ADS1115_MODE_SINGLE |
                        gain |
                        ADS1115_OS_SINGLE;

        switch ( start_channel )
        {
            case 0:
                adc_config |= ADS1115_MUX_AIN0_GND;
                break;
            case 1:
                adc_config |= ADS1115_MUX_AIN1_GND;
                break;
            case 2:
                adc_config |= ADS1115_MUX_AIN2_GND;
                break;
            case 3:
                adc_config |= ADS1115_MUX_AIN3_GND;
                break;
            default:
                ;
        }

        if ( start_channel <= 3 )
        {
            writeBuffer[0] = ADS1115_REG_CONFIG;
            writeBuffer[1] = adc_config >> 8;
            writeBuffer[2] = adc_config & 0xFF;

            I2C3_MasterWriteTRBBuild( &trBlocks[trb_count++], writeBuffer, 3, addr );
        }
    }
    
    I2C3_MasterTRBInsert( trb_count, (I2C3_TRANSACTION_REQUEST_BLOCK *)&trBlocks, (I2C3_MESSAGE_STATUS *)&status );
    
    /* Wait for I2C to finish */
    time = timer_ms;
    while ( ( status != I2C3_MESSAGE_COMPLETE ) && ( ( timer_ms - time ) <= I2C_TIMEOUT_MS ) );
    if ( status != I2C3_MESSAGE_COMPLETE )
        I2C3_Abort();
    
    /* Return conversion */
    return ( ( (uint16_t)(buf[0]) ) << 8 ) | buf[1];
}

void ads1115_start_single( uint8_t addr, uint8_t channel, ads1115_datarate dr, ads1115_fsr_gain gain )
{
	uint16_t adc_config =   ADS1115_COMP_QUE_CON1 |
                            ADS1115_COMP_LAT_Latching |
                            ADS1115_COMP_POL_3_ACTIVELOW |
                            ADS1115_COMP_MODE_TRADITIONAL |
                            dr |
                            ADS1115_MODE_SINGLE |
                            gain |
                            ADS1115_OS_SINGLE;
	
    switch ( channel )
    {
        case 0:
            adc_config |= ADS1115_MUX_AIN0_GND;
            break;
        case 1:
            adc_config |= ADS1115_MUX_AIN1_GND;
            break;
        case 2:
            adc_config |= ADS1115_MUX_AIN2_GND;
            break;
        case 3:
            adc_config |= ADS1115_MUX_AIN3_GND;
            break;
        default:
            return;
	}
    
	ads1115_write_register( addr, ADS1115_REG_CONFIG, adc_config );
}

uint16_t ads1115_get_result( uint8_t addr )
{
    return ads1115_read_register( addr, ADS1115_REG_CONVERSION );
}
