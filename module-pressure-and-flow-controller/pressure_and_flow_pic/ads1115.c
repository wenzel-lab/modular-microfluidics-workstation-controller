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
    
    I2C3_MasterWrite( writeBuffer, 3, addr, &status );
    
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
    I2C3_MasterReadTRBBuild( &trBlocks[1], buf, 2, addr );
    I2C3_MasterTRBInsert( 2, &trBlocks, &status );
    
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
