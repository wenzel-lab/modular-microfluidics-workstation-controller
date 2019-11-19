#include "mcc_generated_files/mcc.h"
//#include "mcc_generated_files/system.h"

int16_t hpid_p;
int16_t hpid_i;
int16_t hpid_d;
int32_t hpid_integrated;
int32_t hpid_windup_limit;
int32_t hpid_error_prev;
uint16_t hpid_target;

uint16_t heater_temp;

uint16_t sim_output;
uint16_t sim_mass_temp;

void heater_sim( void )
{
    int32_t transfer;
    
    /* Internal diffusion */
    sim_mass_temp += sim_output;
    transfer = sim_mass_temp >> 10;
    sim_mass_temp -= transfer;
    heater_temp += transfer;
    
    /* External diffusion / cooling */
    heater_temp -= heater_temp >> 10;
    
//    CCP1RA = 0xFFFF - heater_temp;
}

void heater_pid( void )
{
    int32_t output;
    int32_t diff;
    int32_t error;
    
    error = (int32_t)hpid_target - (int32_t)heater_temp;
    
    hpid_integrated += error * hpid_i;
    if ( hpid_integrated > hpid_windup_limit )
        hpid_integrated = hpid_windup_limit;
    else if ( hpid_integrated < -hpid_windup_limit )
        hpid_integrated = -hpid_windup_limit;
    diff = error - hpid_error_prev;
    hpid_error_prev = error;
    output = (int32_t)error * hpid_p + hpid_integrated + diff * hpid_d;
    output >>= 15;
    if ( output < 0 )
        output = 0;
    
    CCP1RA = 0x7FFF + error;
//    CCP1RA = 0xFFFF - output;
    
    sim_output = output;
    heater_sim();
}

void sim_init( void )
{
    sim_output = 0;
    sim_mass_temp = 0;
}

void init( void )
{
    hpid_p = 50;
    hpid_i = 10;
    hpid_d = 10000;
    hpid_integrated = 0;
    hpid_windup_limit = (int32_t)10 << 15;
    hpid_error_prev = 0;
    hpid_target = 10000;
    
    heater_temp = 0;
}

int main(void)
{
    SYSTEM_Initialize();
    
    init();
    sim_init();
    
    TMR1_SetInterruptHandler( heater_pid );
    
    /* Start ADC level trigger */
    ADCON3Lbits.SWLCTRG = 1;
    
//    CCP1RA = 200;
    
    while (1)
    {
        
    }
}
