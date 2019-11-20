#include "mcc_generated_files/mcc.h"
//#include "mcc_generated_files/system.h"
#include <stdio.h>
#include <math.h>

int16_t hpid_p;
int16_t hpid_i;
int16_t hpid_d;
int32_t hpid_integrated;
int32_t hpid_windup_limit;
int32_t hpid_error_prev;
uint16_t hpid_target;
bool hpid_tune_up;
uint16_t hpid_tune_crosses;
uint16_t hpid_tune_threshold;
uint16_t hpid_tune_min;
uint16_t hpid_tune_max;
uint16_t hpid_tune_p;

uint16_t heater_adc;
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
    
//    CCP1RA = 0x7FFF + error;
//    CCP1RA = 0xFFFF - output;
    
    sim_output = output;
    heater_sim();
}

void sim_init( void )
{
    sim_output = 0;
    sim_mass_temp = 0;
}

void sim_autotune_start( void )
{
    hpid_p = 200;
    hpid_i = 0;
    hpid_d = 0;
    
    hpid_tune_up = hpid_target > heater_temp;
    hpid_tune_crosses = 0;
    hpid_tune_threshold = 50;
    hpid_tune_min = heater_temp;
    hpid_tune_max = heater_temp;
    hpid_tune_p = 10;
}

void sim_autotune( void )
{
    int32_t error;
    bool update = 0;
    
    if ( heater_temp > hpid_tune_max )
        hpid_tune_max = heater_temp;
    else if ( heater_temp < hpid_tune_min )
        hpid_tune_min = heater_temp;
    
    if ( (    hpid_tune_up   && ( heater_temp > hpid_target ) && ( (int32_t)( hpid_tune_max - heater_temp ) > hpid_tune_threshold ) ) ||
         ( ( !hpid_tune_up ) && ( heater_temp < hpid_target ) && ( (int32_t)( heater_temp - hpid_tune_min ) > hpid_tune_threshold ) ) )
        update = 1;
    
//    CCP1RA = heater_temp;
    
    if ( update )
    {
        hpid_tune_up ^= 1;
        
        if ( hpid_tune_crosses < 0xFFFF )
            hpid_tune_crosses++;
        
        if ( hpid_tune_crosses >= 2 )
        {
            int16_t p_change;
            
            error = ( ( (int32_t)hpid_tune_max + (int32_t)hpid_tune_min - ( (int32_t)hpid_target << 1 ) ) << 15 ) / ( (int32_t)hpid_tune_max - (int32_t)hpid_tune_min );
            CCP1RA = 0x7FFF - error;
            p_change = ( error * hpid_tune_p ) >> 13;
            if ( p_change == 0 )
            {
                if ( error > 0 )
                    p_change = 1;
                else if ( error < 0 )
                    p_change = -1;
            }
            hpid_p -= p_change;
            if ( hpid_p < 1 )
                hpid_p = 1;
        }
        
        if ( hpid_tune_up )
            hpid_tune_max = heater_temp;
        else
            hpid_tune_min = heater_temp;
    }
}

float get_heater_temp( void )
{
    /*
    float R1 = 22000;
    float logR2, R2, T;
    float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
    
    heater_adc = ADCBUF0;
    
    R2 = R1 * ( ( 4095.0 / (float)heater_adc ) - 1.0);
    logR2 = log(R2);
    T = ( 1.0 / ( c1 + c2*logR2 + c3*logR2*logR2*logR2 ) );
    T = T - 273.15;
    */
    
#define THERMISTORNOMINAL   100000
#define TEMPERATURENOMINAL  25
#define BCOEFFICIENT        3950
#define SERIESRESISTOR      2200
    
    float steinhart;
    float R;
    static float avg_temp = 0;
    
    heater_adc = ADCBUF0;
    avg_temp = 0.99 * avg_temp + 0.01 * heater_adc;
    
    if ( avg_temp > 0 )
    {
        R = ( 4095.0 / avg_temp ) - 1;
        R = SERIESRESISTOR / R;
        steinhart = R / THERMISTORNOMINAL;                  // (R/Ro)
        steinhart = log(steinhart);                         // ln(R/Ro)
        steinhart /= BCOEFFICIENT;                          // 1/B * ln(R/Ro)
        steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15);   // + (1/To)
        steinhart = 1.0 / steinhart;                        // Invert
        steinhart -= 273.15;
    }
    else 
        steinhart = 0;
    
    return steinhart;
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
    
//    CCP1RA = 0;
    CCP1RA = 0xFFFF - 60000;
    
//    sim_autotune_start();
    
    while (1)
    {
        float temp_c = get_heater_temp();
        
        printf( "Temp %0.3f\n", temp_c );
        
//        CCP1RA = 0xFFFF - heater_temp;
//        sim_autotune();
    }
}
