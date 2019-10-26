#include <xc.h>
#define FCY 8000000UL
#include <libpic30.h>
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/spi2.h"
#include "mcc_generated_files/pin_manager.h"

void dac_cmd( uint8_t cmd, uint8_t addr, uint16_t value )
{
    struct
    {
        uint8_t addr : 3;
        uint8_t cmd : 3;
        uint8_t unused : 2;
        uint8_t value_high;
        uint8_t value_low;
    } data;
    
    data.cmd = cmd;
    data.addr = addr;
    data.value_high = value >> 8;
    data.value_low = value & 0xFF;
    
    DAC_NSYNC_SetLow();
    SPI2_Exchange8bitBuffer( (uint8_t *)&data, 3, NULL );
    DAC_NSYNC_SetHigh();
}

int main(void)
{
    SYSTEM_Initialize();
    
    __delay_ms( 1000 );
    
//    dac_cmd( 0b111, 0, 1 );
    dac_cmd( 0b011, 0, 1048 << 4 );
    
    while (1)
    {
    }
    
    return 1; 
}
