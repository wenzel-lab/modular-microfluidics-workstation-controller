#include "mcc_generated_files/mcc.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define CLOCK_FREQ      32000000
#define PS_PER_TICK     ( 1000000000 / ( CLOCK_FREQ / 1000 ) )  // 31250 ps/tick
#define TIME_SCALING    10  // 31250 can be divided by 10 whole
#define MAX_TIME_NS     ( INT32_MAX / TIME_SCALING )

#if 0
uint32_t find_scalers_freq( uint32_t target_freq, uint8_t *prescale, uint8_t *postscale, uint8_t *period )
{
    /** This function has to be fixed. */
    
    uint32_t rem;
    uint32_t period_loop;
    uint8_t postscale_loop;
    int8_t prescale_loop;
    uint32_t freq_loop;
    uint8_t postscale_best;
    uint8_t prescale_best;
    uint32_t period_best;
    uint32_t freq_best;
    uint32_t denom;

    freq_best = 0;
    postscale_best = 0;
    prescale_best = 0;
    period_best = 0;

    if ( target_freq == 0 )
        return 0;

    for ( postscale_loop=16; postscale_loop>=1; postscale_loop-- )
    {
        rem = ( CLOCK_FREQ / target_freq ) / postscale_loop;

        for ( prescale_loop=7; prescale_loop>=0; prescale_loop-- )
        {
            if ( prescale_loop == 0 )
                period_loop = rem;
            else
                period_loop = ( ( rem >> ( prescale_loop - 1 ) ) + 1 ) >> 1;  // Round

            denom = postscale_loop * period_loop;

            if ( denom > 0 )
                freq_loop = ( ( CLOCK_FREQ >> prescale_loop ) / denom );
            else
                freq_loop = 0;

            if ( abs( freq_loop - target_freq ) < abs( freq_best - target_freq ) )
            {
                freq_best = freq_loop;
                postscale_best = postscale_loop;
                prescale_best = prescale_loop;
                period_best = period_loop;
            }

            if ( freq_loop == target_freq )
                break;
        }
    }
    
    if ( period_best > 0x100 )
        return 0;   // We failed
    else
    {
        *prescale = prescale_best;
        *postscale = postscale_best - 1;
        *period = (uint8_t)( period_best - 1 );
        
        return freq_best;
    }
}
#endif

uint32_t find_scalers_time( uint32_t target_time_ns, uint8_t *prescale, uint8_t *postscale, uint8_t *period )
{
    uint32_t ticks;
    uint32_t rem;
    uint32_t period_loop;
    int8_t postscale_loop;
    int8_t prescale_loop;
    uint32_t time_ns_loop;
    uint32_t time_ns_best;
    uint8_t postscale_best;
    uint8_t prescale_best;
    uint32_t period_best;

    if ( target_time_ns > MAX_TIME_NS )
        return 0;

//    ticks = target_time_ns * ( 1000 / TIME_SCALING ) / ( PS_PER_TICK / TIME_SCALING );
    ticks = ( target_time_ns * ( 1000 / TIME_SCALING ) + ( ( PS_PER_TICK >> 1 ) / TIME_SCALING ) ) / ( PS_PER_TICK / TIME_SCALING );

    time_ns_best = 0;
    postscale_best = 0;
    prescale_best = 0;
    period_best = 0;

    for ( postscale_loop=16; postscale_loop>=1; postscale_loop-- )
    {
        rem = ticks / postscale_loop;

        for ( prescale_loop=7; prescale_loop>=0; prescale_loop-- )
        {
            if ( prescale_loop == 0 )
                period_loop = rem;
            else
                period_loop = ( ( rem >> ( prescale_loop - 1 ) ) + 1 ) >> 1;  // Round

            if ( ( period_loop > 0 ) && ( period_loop <= 0xFF ) && ( ( period_loop > 1 ) || ( prescale_loop > 0 ) ) )
            {
                time_ns_loop = ( ( ( ( (uint32_t)PS_PER_TICK / TIME_SCALING ) << prescale_loop ) * period_loop ) * postscale_loop ) / ( 1000 / TIME_SCALING );

                if ( abs( time_ns_loop - target_time_ns ) < abs( time_ns_best - target_time_ns ) )
                {
                    time_ns_best = time_ns_loop;
                    postscale_best = postscale_loop;
                    prescale_best = prescale_loop;
                    period_best = period_loop;
                }

                if ( time_ns_loop == target_time_ns )
                    break;
            }
        }
    }
    
    *prescale = prescale_best;
    *postscale = postscale_best - 1;
    *period = (uint8_t)( period_best - 1 );

    return time_ns_best;
}

#if 0
void set_strobe_timing( uint32_t target_freq )
{
    uint8_t prescale;
    uint8_t postscale;
    uint8_t period;
    uint32_t freq;
    
    freq = find_scalers_freq( target_freq, &prescale, &postscale, &period );
    
    /* Turn off and reset timers */
    T2CON = 0;
    
    if ( freq > 0 )
    {
        /* If freq==0 -> couldn't calculate register values */
        
        PR2 = period;
        T2CON = 0b10000000 | ( prescale << 4 ) | postscale;
    }
}
#endif

void set_strobe_timing( uint32_t wait_target_ns, uint32_t duration_target_ns )
{
    uint8_t wait_prescale;
    uint8_t wait_postscale;
    uint8_t wait_period;
    uint8_t duration_prescale;
    uint8_t duration_postscale;
    uint8_t duration_period;
    uint32_t wait_ns;
    uint32_t duration_ns;
    
    wait_ns = find_scalers_time( wait_target_ns, &wait_prescale, &wait_postscale, &wait_period );
    duration_ns = find_scalers_time( duration_target_ns, &duration_prescale, &duration_postscale, &duration_period );
    
    /* Turn off and reset timers */
    T2CON = 0;
    T4CON = 0;
    
//    duration_prescale = 0;
//    duration_postscale = 0;
//    duration_period = 0;
    
// 7 0 10 8
// 7 0 20 15
// 7 0 39 25
// 7 0 255 188
    
    if ( ( wait_ns > 0 ) && ( duration_ns > 0 ) )
    {
        /* If time_ns==0 -> couldn't calculate register values */
        
        PR2 = wait_period;
        PR4 = duration_period;
        T2CON = 0b10000000 | ( wait_prescale << 4 ) | wait_postscale;
        T4CON = 0b10000000 | ( duration_prescale << 4 ) | duration_postscale;
    }
}

void main(void)
{
    SYSTEM_Initialize();

    //INTERRUPT_GlobalInterruptEnable();
    //INTERRUPT_PeripheralInterruptEnable();
    //INTERRUPT_GlobalInterruptDisable();
    //INTERRUPT_PeripheralInterruptDisable();
    
    set_strobe_timing( 2000000, 2000 );

    while ( 1 )
    {
        
    }
}
