#include "mcc_generated_files/mcc.h"
#include "common.h"
#include "ads1115.h"

#define I2C_TIMEOUT_MS      2

uint8_t conversion_reg = ADS1115_REG_CONVERSION;

uint8_t ads1115_write_register( uint8_t addr, uint8_t reg, uint16_t data )
{
    volatile I2C2_MESSAGE_STATUS status;
    uint8_t writeBuffer[3];
    uint16_t time;
    
    writeBuffer[0] = reg;
    writeBuffer[1] = data >> 8;
    writeBuffer[2] = data & 0xFF;
    
    I2C2_MasterWrite( writeBuffer, 3, addr, (I2C2_MESSAGE_STATUS *)&status );
    
    time = timer_ms;
    while ( ( status != I2C2_MESSAGE_COMPLETE ) && ( ( timer_ms - time ) <= I2C_TIMEOUT_MS ) );
    if ( status != I2C2_MESSAGE_COMPLETE )
        I2C2_Abort();
    
	return 0;
}

uint16_t ads1115_read_register(uint8_t addr, uint8_t reg)
{
    uint8_t buf[2];
    volatile I2C2_MESSAGE_STATUS status;
    I2C2_TRANSACTION_REQUEST_BLOCK trBlocks[2];
    uint16_t time;

    I2C2_MasterWriteTRBBuild( &trBlocks[0], &reg, 1, addr );
    I2C2_MasterReadTRBBuild( &trBlocks[1], (void *)buf, 2, addr );
    I2C2_MasterTRBInsert( 2, (I2C2_TRANSACTION_REQUEST_BLOCK *)&trBlocks, (I2C2_MESSAGE_STATUS *)&status );
    
    time = timer_ms;
    while ( ( status != I2C2_MESSAGE_COMPLETE ) && ( ( timer_ms - time ) <= I2C_TIMEOUT_MS ) );
    if ( status != I2C2_MESSAGE_COMPLETE )
        I2C2_Abort();
    
    return ( ( (uint16_t)(buf[0]) ) << 8 ) | buf[1];
}

void ads1115_set_ready_pin( uint8_t addr )
{
    ads1115_write_register( addr, ADS1115_REG_HI_THRESH, 0b1000000000000000 );
    ads1115_write_register( addr, ADS1115_REG_LO_THRESH, 0b0000000000000000 );
}

void ads1115_read_adc_start( uint8_t addr, int8_t read_channel, int8_t start_channel, ads1115_datarate dr, ads1115_fsr_gain gain, ads1115_task_t *task )
{
    uint16_t adc_config;
    uint8_t trb_count;
    
    /** Clear I2C buffer */
    
    trb_count = 0;
    task->channel = read_channel;
    
    if ( read_channel >= 0 )
    {
        /* Read conversion */
        
        I2C2_MasterWriteTRBBuild( &task->trBlocks[trb_count++], &conversion_reg, 1, addr );
        I2C2_MasterReadTRBBuild( &task->trBlocks[trb_count++], task->read_data, 2, addr );
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
            task->write_data[0] = ADS1115_REG_CONFIG;
            task->write_data[1] = adc_config >> 8;
            task->write_data[2] = adc_config & 0xFF;

            I2C2_MasterWriteTRBBuild( &task->trBlocks[trb_count++], task->write_data, 3, addr );
        }
    }
    
    I2C2_MasterTRBInsert( trb_count, (I2C2_TRANSACTION_REQUEST_BLOCK *)&task->trBlocks, (I2C2_MESSAGE_STATUS *)&task->status );
    
    /* Record start time */
    task->start_time = timer_ms;
}

int8_t ads1115_read_adc_return( uint16_t *value, int8_t *channel, ads1115_task_t *task )
{
    /* Returns: 0 if still waiting
     *          1 if result available / success
     *          -1 if timeout
     * Channel: <ADC channel number> or -1 if no read was requested
     */
    
    int8_t rc;
    
    if ( task->status == I2C2_MESSAGE_COMPLETE )
    {
        /* Return conversion */
        *value = ( ( (uint16_t)(task->read_data[0]) ) << 8 ) | task->read_data[1];
        *channel = task->channel;
        rc = 1;
    }
    else if ( ( timer_ms - task->start_time ) > I2C_TIMEOUT_MS )
    {
        /* Timeout */
        I2C2_Abort();
        rc = -1;
    }
    else
    {
        /* Still waiting */
        rc = 0;
    }
    
    return rc;
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
