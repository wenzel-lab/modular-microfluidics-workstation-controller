#include "mcc_generated_files/system.h"

int main(void)
{
    SYSTEM_Initialize();
    
    /* Start ADC level trigger */
    ADCON3Lbits.SWLCTRG = 1;
    
    CCP1RA = 200;
    
    while (1)
    {
        
    }
}
