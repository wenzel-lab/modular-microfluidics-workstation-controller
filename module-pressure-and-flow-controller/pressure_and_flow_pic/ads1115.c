#define FCY 8000000UL
#include <libpic30.h>
#include "mcc_generated_files/mcc.h"
#include "ads1115.h"

/**************************************************************************/
/*!
    @brief  write register of ADC
*/
/**************************************************************************/
uint8_t ads1115_write_register(uint8_t addr, uint8_t reg, uint16_t data)
{
    volatile static I2C3_MESSAGE_STATUS status;
    volatile static uint8_t writeBuffer[3];
    
    writeBuffer[0] = reg;
    writeBuffer[1] = data >> 8;
    writeBuffer[2] = data & 0xFF;
    
    I2C3_MasterWrite( writeBuffer, 3, addr, &status );
    while ( status != I2C3_MESSAGE_COMPLETE );
    
/*
	i2c_start_wait((addr << 1) + I2C_WRITE);
	i2c_write((uint8_t)reg);
	i2c_write((uint8_t)(data >> 8));
	i2c_write((uint8_t)(data & 0xFF));
	i2c_stop();
 */
	
	return 0;
}

/**************************************************************************/
/*!
    @brief  read register from ADC
*/
/**************************************************************************/
uint16_t ads1115_read_register(uint8_t addr, uint8_t reg)
{
    volatile static uint8_t buf[2];
    volatile static I2C3_MESSAGE_STATUS status;
    static I2C3_TRANSACTION_REQUEST_BLOCK trBlocks[2];

    I2C3_MasterWriteTRBBuild( &trBlocks[0], &reg, 1, addr );
    I2C3_MasterReadTRBBuild( &trBlocks[1], buf, 2, addr );
    I2C3_MasterTRBInsert( 2, &trBlocks, &status );
    while ( status != I2C3_MESSAGE_COMPLETE );

    return ( ( (uint16_t)(buf[0]) ) << 8 ) | buf[1];
    
/*
	i2c_start_wait((addr << 1) + I2C_WRITE);
	i2c_write(reg);
	i2c_stop();
	
	i2c_rep_start((addr << 1) + I2C_READ);
	uint8_t msb = i2c_readAck();
	uint8_t lsb = i2c_readNak();
	i2c_stop();
	
	uint16_t data = (msb << 8 | lsb);
	return data;
 */
}

/**************************************************************************/
/*!
    @brief  read raw data from one channel of ADC
*/
/**************************************************************************/
uint16_t ads1115_readADC_SingleEnded(uint8_t addr, uint8_t channel, ads1115_datarate dr, ads1115_fsr_gain gain)
{
	// Check channel number
	if(channel > 3)
	{
		return 0;
	}
	
	uint16_t adc_config = ADS1115_COMP_QUE_DIS	|
				 ADS1115_COMP_LAT_NonLatching |
				 ADS1115_COMP_POL_3_ACTIVELOW |	
				 ADS1115_COMP_MODE_TRADITIONAL | 
				 dr |
				 //DR_128SPS |
				 ADS1115_MODE_SINGLE | 
				 gain;
				 //FSR_6_144;
	
	if(channel == 0)
	{
		adc_config |= ADS1115_MUX_AIN0_GND;
	} 
	else if(channel == 1)
	{
		adc_config |= ADS1115_MUX_AIN1_GND;
	} 
	else if(channel == 2)
	{
		adc_config |= ADS1115_MUX_AIN2_GND;
	} 
	else if(channel == 3)
	{
		adc_config |= ADS1115_MUX_AIN3_GND;
	}
	
	adc_config |= ADS1115_OS_SINGLE;	
	
	ads1115_write_register(addr, ADS1115_REG_CONFIG, adc_config);
	__delay_ms( 8 );
	
	return ads1115_read_register(addr, ADS1115_REG_CONVERSION) >> 0;
}

/**************************************************************************/
/*!
    @brief  read raw data / conversion result of ADC:
			differential between P: AIN0 and N: AIN1
*/
/**************************************************************************/
int16_t ads1115_readADC_Diff_A0_1(uint8_t addr, ads1115_datarate dr, ads1115_fsr_gain gain)
{
	uint16_t adc_config = ADS1115_COMP_QUE_DIS	|
						  ADS1115_COMP_LAT_NonLatching |
						  ADS1115_COMP_POL_3_ACTIVELOW |	
						  ADS1115_COMP_MODE_TRADITIONAL | 
						  dr |
						  //ADS1115_DR_128SPS |
						  ADS1115_MODE_SINGLE | 
						  gain;
						  //FSR_6_144;
	
	adc_config |= ADS1115_MUX_AIN0_AIN1;
	
	adc_config |= ADS1115_OS_SINGLE;	
	
	ads1115_write_register(addr, ADS1115_REG_CONFIG, adc_config);
	__delay_ms( 8 );
	
	return (int16_t)ads1115_read_register(addr, ADS1115_REG_CONVERSION);
}

/**************************************************************************/
/*!
    @brief  read raw data / conversion result of ADC:
			differential between P: AIN0 and N: AIN3
*/
/**************************************************************************/
int16_t ads1115_readADC_Diff_A0_3(uint8_t addr, ads1115_datarate dr, ads1115_fsr_gain gain)
{
	uint16_t adc_config = ADS1115_COMP_QUE_DIS	|
						  ADS1115_COMP_LAT_NonLatching |
						  ADS1115_COMP_POL_3_ACTIVELOW |	
						  ADS1115_COMP_MODE_TRADITIONAL | 
						  dr |
						  //ADS1115_DR_128SPS |
						  ADS1115_MODE_SINGLE | 
						  gain;
						  //FSR_6_144;
	
	adc_config |= ADS1115_MUX_AIN0_AIN3;
	
	adc_config |= ADS1115_OS_SINGLE;	
	
	ads1115_write_register(addr, ADS1115_REG_CONFIG, adc_config);
	__delay_ms( 8 );
	
	return (int16_t)ads1115_read_register(addr, ADS1115_REG_CONVERSION);
}

/**************************************************************************/
/*!
    @brief  read raw data / conversion result of ADC:
			differential between P: AIN1 and N: AIN3
*/
/**************************************************************************/
int16_t ads1115_readADC_Diff_A1_3(uint8_t addr, ads1115_datarate dr, ads1115_fsr_gain gain)
{
	uint16_t adc_config = ADS1115_COMP_QUE_DIS	|
						  ADS1115_COMP_LAT_NonLatching |
						  ADS1115_COMP_POL_3_ACTIVELOW |	
						  ADS1115_COMP_MODE_TRADITIONAL | 
						  dr |
						  //ADS1115_DR_128SPS |
						  ADS1115_MODE_SINGLE | 
						  gain;
						  //FSR_6_144;
	
	adc_config |= ADS1115_MUX_AIN1_AIN3;
	
	adc_config |= ADS1115_OS_SINGLE;	
	
	ads1115_write_register(addr, ADS1115_REG_CONFIG, adc_config);
	__delay_ms( 8 );
	
	return (int16_t)ads1115_read_register(addr, ADS1115_REG_CONVERSION);
}

/**************************************************************************/
/*!
    @brief  read raw data / conversion result of ADC:
			differential between P: AIN2 and N: AIN3
*/
/**************************************************************************/
int16_t ads1115_readADC_Diff_A2_3(uint8_t addr, ads1115_datarate dr, ads1115_fsr_gain gain)
{
	uint16_t adc_config = ADS1115_COMP_QUE_DIS	|
						  ADS1115_COMP_LAT_NonLatching |
						  ADS1115_COMP_POL_3_ACTIVELOW |	
						  ADS1115_COMP_MODE_TRADITIONAL | 
						  dr |
						  //ADS1115_DR_128SPS |
						  ADS1115_MODE_SINGLE | 
						  gain;
						  //FSR_6_144;
	
	adc_config |= ADS1115_MUX_AIN2_AIN3;
	
	adc_config |= ADS1115_OS_SINGLE;	
	
	ads1115_write_register(addr, ADS1115_REG_CONFIG, adc_config);
	__delay_ms( 8 );
	
	return (int16_t)ads1115_read_register(addr, ADS1115_REG_CONVERSION);
}