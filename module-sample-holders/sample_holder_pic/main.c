#include "mcc_generated_files/mcc.h"
//#include "mcc_generated_files/system.h"
#include <stdio.h>
#include <math.h>
#include "common.h"
#include "spi.h"

/* System Macros */
#define MIN(a,b)                        ((a)<=(b)?(a):(b))
#define MAX(a,b)                        ((a)>=(b)?(a):(b))
#define PTR_TO_16BIT(ptr)               ( ( (*((uint8_t *)ptr+1)) << 8 ) | *(uint8_t *)ptr )
#define COPY_16BIT_TO_PTR(ptr,val)      {*(ptr+1)=*((uint8_t *)&val+1); *ptr=*(uint8_t *)&val;}
#define COPY_16BIT_TO_PTR_REV(ptr,val)  {*ptr=*((uint8_t *)&val+1); *(ptr+1)=*(uint8_t *)&val;}
#define COPY_32BIT_TO_PTR_REV(ptr,val)  {*ptr=*((uint8_t *)&val+3); *(ptr+1)=*(uint8_t *)&val+2;\
                                         *(ptr+2)=*((uint8_t *)&val+1); *(ptr+3)=*(uint8_t *)&val;}
//#define EEPROM_NSS                      PORTBbits.RB3

/* System Constants */
#define PI                          3.14159265359

/* Application Macros */
#define SET_HEATER_OUTPUT(output)   { heater_output = output; CCP1RA = -(uint16_t)(output); }
#define HPID_INTERRUPT_ON()         { IEC7bits.ADFLTR0IE = 1; }
#define HPID_INTERRUPT_OFF()        { IEC7bits.ADFLTR0IE = 0; }

/* Heater Read Constants */
#define HEATER_POWER_MAX                    0xFFFF
#define HEATER_PERIOD_MS                    100
#define HEATER_PERIOD_S_COUNTS              ( 1000 / HEATER_PERIOD_MS )
#define HEATER_ADC_SHIFT                    8   // 16-bit ADC SHL 8 = 24-bit -> fits float
#define HEATER_ADC_FILT_SHIFT               4
#define HEATER_ADC_FILT_MUL                 ( ( 1 << HEATER_ADC_FILT_SHIFT ) - 1 )
#define HEATER_DIFF_FILT_SHIFT              7
#define HEATER_DIFF_FILT_MUL                ( ( 1 << HEATER_DIFF_FILT_SHIFT ) - 1 )
#define HEATER_ADC_BITS                     14
#define HEATER_ADC_MAX                      ( ( 1 << HEATER_ADC_BITS ) - 1 )
#define HEATER_ADC_REG                      ADCBUF0
#define HEATER_ADC_FLT_REG                  ADFL0DAT
#define HEATER_ADC_RDY_REG                  (ADSTATLbits.AN0RDY)
#define HEATER_ADC_FLT_RDY_REG              (ADFL0CONbits.RDY)
#define HEATER_THERM_REF_R                  100000
#define HEATER_THERM_REF_TEMP               25
#define HEATER_THERM_B_COEFF                3950
#define HEATER_THERM_RES_R                  3300
#define HEATER_TEMP_SCALE                   100
#define HEATER_TEMP_MIN_SCALED              (0*HEATER_TEMP_SCALE)
#define HEATER_TEMP_MAX_SCALED              (80*HEATER_TEMP_SCALE)

/* Heater Autotune Constants */
#define HTUNE_POWER_MAX                     ( HEATER_POWER_MAX >> 2 )
#define HTUNE_TRANS_TEMP_HYST               ( HEATER_TEMP_SCALE / 2 )
//#define HTUNE_TRANS_TEMP_HYST               ( HEATER_TEMP_SCALE / 10 )
#define HTUNE_TRANS_TIME_MIN_S              5
#define HTUNE_BIAS_ALLOWANCE                20
#define HTUNE_PERIOD_TIMEOUT_S              600
#define HTUNE_KP_SHL                        6
#define HTUNE_KI_SHL                        14
#define HTUNE_KD_SHR                        2
#define HTUNE_CYCLES_MIN                    5
#define HTUNE_CYCLES_MAX                    50
#define HTUNE_CONV_ASYMMETRY_SHL            15
#define HTUNE_CONV_COUNT                    5
#define HTUNE_CONV_ASYMMETRY_THRESHOLD_PC   10
#define HTUNE_CONV_PASS_COUNT_THRESHOLD     3
#define HTUNE_CONV_CYCLE_MAX_MS_PER_DEGC    (10*60*(int32_t)1000)

/* Comms Constants */
#define PACKET_TYPE_GET_ID                  1
#define PACKET_TYPE_TEMP_SET_TARGET         2
#define PACKET_TYPE_TEMP_GET_TARGET         3
#define PACKET_TYPE_TEMP_GET_ACTUAL         4
#define PACKET_TYPE_PID_SET_COEFFS          5
#define PACKET_TYPE_PID_GET_COEFFS          6
#define PACKET_TYPE_PID_SET_RUNNING         7
#define PACKET_TYPE_PID_GET_STATUS          8
#define PACKET_TYPE_AUTOTUNE_SET_RUNNING    9
#define PACKET_TYPE_AUTOTUNE_GET_RUNNING    10
#define PACKET_TYPE_AUTOTUNE_GET_STATUS     11

/* Defined range limits
 * - HTUNE_PERIOD_TIMEOUT_S max is UINT16_MAX / ( 1000 / HEATER_PERIOD_MS )
 *   = ( 65535 / ( 1000 / 100 ) ) = 6553s
 * - Limit temperatures to +-INT16_MAX / HEATER_TEMP_SCALE
 *   = +-32767 / 100 = 327 degrees C
 * - ( diff * hpid_d ) << ( HEATER_ADC_SHIFT + HEATER_DIFF_FILT_SHIFT ) minmax INT32_MAX
 *
 * Sanity checks:
 * - htune periods must be within HTUNE_PERIOD_TIMEOUT_S
 * - <htune_cycles> must be range checked
 */

/* System Data */
uint8_t device_id[] = "SAMPLE_HOLDER";
volatile uint16_t timer1_counter;

/* Heater Variables */
uint16_t heater_output;
volatile uint32_t heater_adc_avg;
int16_t heater_temp_c_scaled;

/* Heater PID Types */
typedef enum
{
    HPID_STATE_UNCONFIGURED,
    HPID_STATE_READY,
    HPID_STATE_RUNNING,
    HPID_STATE_SUSPENDED,
    HPID_STATE_ERROR,
} E_HPID_STATE;

/* Heater PID Data */
E_HPID_STATE hpid_state;
int32_t hpid_p;
int32_t hpid_i;
int32_t hpid_d;
int32_t hpid_integrated;
int32_t hpid_windup_limit;
int32_t hpid_diff;
int32_t hpid_error_prev;
int16_t hpid_target;
volatile uint16_t hpid_counter;

/* Autotune Types */
typedef enum
{
    HTUNE_STATE_DEFAULT,
    HTUNE_STATE_RUNNING,
    HTUNE_STATE_ABORTED,
    HTUNE_STATE_FINISHED,
    HTUNE_STATE_FAILED
} E_HTUNE_STATE;

typedef enum
{
    HTUNE_FAIL_NONE,
    HTUNE_FAIL_RATE,
    HTUNE_FAIL_CYCLES
} E_HTUNE_FAIL;

/* Autotune Data */
E_HTUNE_STATE htune_state;
E_HTUNE_FAIL htune_fail;
bool htune_active;
bool htune_heating;
uint16_t htune_bias;
uint16_t htune_delta;
int16_t htune_target;
uint16_t htune_cycles;
uint16_t htune_timer;
uint16_t htune_timer_heating;
uint16_t htune_timer_cooling;
uint16_t htune_period_heating;
uint16_t htune_period_cooling;
uint16_t htune_temp_max;
uint16_t htune_temp_min;
int32_t htune_p;
int32_t htune_i;
int32_t htune_d;
float htune_ku;
float htune_tu;
float htune_kp;
float htune_ki;
float htune_kd;
int32_t htune_log[HTUNE_CYCLES_MAX][2];
float htune_log_ku[HTUNE_CYCLES_MAX];
float htune_log_tu[HTUNE_CYCLES_MAX];
uint8_t htune_log_index;
uint16_t htune_cycle_start_time;
int16_t htune_cycle_start_temp_scaled;

/* Packet Data */
spi_packet_buf_t spi_packet;
uint8_t packet_type;
uint8_t packet_data[SPI_PACKET_BUF_SIZE];
uint8_t packet_data_size;

/* Static Function Prototypes */
void heater_pid_start( void );
void autotune( bool write_output );

inline int32_t constrain_i32( int32_t value, int32_t min, int32_t max )
{
    if ( value < min )
        return min;
    else if ( value > max )
        return max;
    else return value;
}

void heater_pid( void )
{
    /* To prevent overflow, we constrain values and terms such that when
     * multiplied together, fit into I32.
     */
    
    int32_t output;
    int32_t diff;
    int32_t error;
    
    hpid_counter++;
    
    error = (int32_t)hpid_target - (int32_t)heater_temp_c_scaled;
    error = constrain_i32( error, INT16_MIN, INT16_MAX );
    
    hpid_integrated += constrain_i32( error * hpid_i, -(int32_t)UINT16_MAX << HTUNE_KI_SHL, UINT16_MAX << HTUNE_KI_SHL );
    hpid_integrated = constrain_i32( hpid_integrated, 0, hpid_windup_limit << HTUNE_KI_SHL );
    diff = ( constrain_i32( error - hpid_error_prev, INT16_MIN, INT16_MAX ) * hpid_d ) << HTUNE_KD_SHR;
    hpid_diff = ( ( hpid_diff * HEATER_DIFF_FILT_MUL ) + diff ) >> HEATER_DIFF_FILT_SHIFT;
    hpid_error_prev = error;
    
//    output = constrain_i32( ( error * hpid_p ) >> HTUNE_KP_SHL, -(int32_t)UINT16_MAX, UINT16_MAX ) +
//             hpid_integrated >> HTUNE_KI_SHL +
//             constrain_i32( hpid_diff, -(int32_t)UINT16_MAX, UINT16_MAX );
    output = constrain_i32( ( error * hpid_p ) >> HTUNE_KP_SHL, -(int32_t)UINT16_MAX, UINT16_MAX ) +
             ( hpid_integrated >> HTUNE_KI_SHL ) +
             constrain_i32( hpid_diff, -(int32_t)UINT16_MAX, UINT16_MAX );
    output = constrain_i32( output, 0, HEATER_POWER_MAX );
    
    SET_HEATER_OUTPUT( output );
}

int32_t select_kth( int32_t *arr, uint8_t n, uint8_t k )
{
    uint8_t i, j;
    int16_t min;
    uint8_t min_pos;

    for ( i=0; i<=k; i++ )
    {
        min = arr[i];
        min_pos = i;

        for ( j=(i+1); j<n; j++ )
        {
            if ( arr[j] < min )
            {
                min = arr[j];
                min_pos = j;
            }
        }

        arr[min_pos] = arr[i];
    }

    return min;
}

void autotune_calculate_pid( float htune_ku, float htune_tu,
                             float *htune_kp, float *htune_ki, float *htune_kd,
                             int32_t *hpid_p, int32_t *hpid_i, int32_t *hpid_d )
{
    *htune_kp = 0.6f * htune_ku;
    *htune_ki = 2.0f * *htune_kp / htune_tu;
    *htune_kd = *htune_kp * htune_tu * 0.125f;
//    htune_kp = 0.2f * htune_ku;
//    htune_ki = 2.0f * htune_kp / htune_tu;
//    htune_kd = htune_kp * htune_tu * 0.3333f;

    *hpid_p = constrain_i32( *htune_kp * ( 1 << HTUNE_KP_SHL ), 0, UINT16_MAX );
    *hpid_i = constrain_i32( *htune_ki * ( 1 << HTUNE_KI_SHL ) * HEATER_PERIOD_MS / 1000, 0, UINT16_MAX );
    *hpid_d = constrain_i32( *htune_kd * 1000 / ( (uint32_t)HEATER_PERIOD_MS << HTUNE_KD_SHR ), 0, UINT16_MAX );
}

void autotune_start( int16_t target_temp )
{
    HPID_INTERRUPT_OFF();
    
    if ( hpid_state == HPID_STATE_RUNNING )
        hpid_state = HPID_STATE_SUSPENDED;
    
    htune_active = false;
    htune_heating = heater_temp_c_scaled <= target_temp;
    htune_bias = HTUNE_POWER_MAX >> 1;
    htune_delta = HTUNE_POWER_MAX >> 1;
    htune_cycles = 0;
    htune_timer = 0;
    htune_timer_heating = 0;
    htune_timer_cooling = 0;
    htune_period_heating = 0;
    htune_period_cooling = 0;
    htune_cycle_start_time = htune_timer;
    htune_cycle_start_temp_scaled = heater_temp_c_scaled;
    htune_temp_max = heater_temp_c_scaled;
    htune_temp_min = htune_temp_max;
    htune_log_index = 0;
    htune_target = target_temp;
    
    /* This also initialises cycle start temp and time. */
    autotune( true );
    
    htune_state = HTUNE_STATE_RUNNING;
    htune_fail = HTUNE_FAIL_NONE;
    htune_active = true;
    
    HPID_INTERRUPT_ON();
    
    printf( "Autotune started at %i\n", target_temp );
}

void autotune_stop( E_HTUNE_STATE state )
{
    HPID_INTERRUPT_OFF();
    
    htune_active = false;
    htune_state = state;
    
    if ( hpid_state == HPID_STATE_SUSPENDED )
        heater_pid_start();
    
    HPID_INTERRUPT_ON();
}

void autotune( bool write_output )
{
    bool update = false;
//    int16_t temp_c_scaled = ( heater_temp_c_scaled / 10 ) * 10;
    int16_t temp_c_scaled = heater_temp_c_scaled;
    
    if ( temp_c_scaled > htune_temp_max )
        htune_temp_max = temp_c_scaled;
    else if ( temp_c_scaled < htune_temp_min )
        htune_temp_min = temp_c_scaled;
    
    if ( htune_heating && ( temp_c_scaled > ( htune_target + HTUNE_TRANS_TEMP_HYST ) ) )
    {
        if ( ( htune_timer - htune_timer_cooling ) >
             ( HTUNE_TRANS_TIME_MIN_S * HEATER_PERIOD_S_COUNTS ) )
        {
            htune_timer_heating = htune_timer;
            htune_period_heating = htune_timer_heating - htune_timer_cooling;
            htune_heating = false;
            update = true;
        }
    }
    else if ( ( !htune_heating ) && ( temp_c_scaled < ( htune_target - HTUNE_TRANS_TEMP_HYST ) ) )
    {
        if ( ( htune_timer - htune_timer_heating ) >
             ( HTUNE_TRANS_TIME_MIN_S * HEATER_PERIOD_S_COUNTS ) )
        {
            htune_timer_cooling = htune_timer;
            htune_period_cooling = htune_timer_cooling - htune_timer_heating;
            htune_heating = true;
            update = true;
        }
    }
    
    if ( update )
    {
        htune_cycles++;
        write_output = true;
        
        if ( ( htune_cycles > 2 ) && htune_heating )
        {
            /* The first "cycle" will be only partial.  Then an additional
             * two cycles are needed.  Then we can only adjust after cooling.
             */
            
            int32_t bias_i32;
            int32_t asymmetry;

            asymmetry = ( labs( (int32_t)htune_period_heating - (int32_t)htune_period_cooling ) << HTUNE_CONV_ASYMMETRY_SHL ) /
                        ( (int32_t)htune_period_heating + (int32_t)htune_period_cooling );
            htune_log[htune_log_index][0] = htune_bias;
            htune_log[htune_log_index][1] = asymmetry;
            
            bias_i32 = (int32_t)htune_bias + ( (int32_t)htune_delta * ( (int32_t)htune_period_heating - (int32_t)htune_period_cooling ) ) / ( (int32_t)htune_period_heating + (int32_t)htune_period_cooling );
            htune_bias = constrain_i32( bias_i32, HTUNE_BIAS_ALLOWANCE, HTUNE_POWER_MAX - HTUNE_BIAS_ALLOWANCE );
            htune_delta = ( htune_bias > ( HTUNE_POWER_MAX >> 1 ) ) ? ( HTUNE_POWER_MAX - 1 - htune_bias ) : htune_bias;
            
            htune_ku = ( 4.0f * htune_delta ) / ( PI * ( htune_temp_max - htune_temp_min ) * 0.5f );
            htune_tu = ( (float)( htune_period_heating + htune_period_cooling ) * HEATER_PERIOD_MS / 1000.0f );
            
            autotune_calculate_pid( htune_ku, htune_tu, &htune_kp, &htune_ki, &htune_kd, &htune_p, &htune_i, &htune_d );
            
            if ( htune_cycles >= HTUNE_CYCLES_MIN )
            {
                htune_log_ku[htune_log_index] = htune_ku;
                htune_log_tu[htune_log_index] = htune_tu;
                htune_log_index++;
            }
        }
            
        if ( htune_heating )
            htune_temp_max = temp_c_scaled;
        else
            htune_temp_min = temp_c_scaled;
        
        htune_cycle_start_time = htune_timer;
        htune_cycle_start_temp_scaled = temp_c_scaled;
    }
    
    if ( write_output )
        SET_HEATER_OUTPUT( htune_heating ? ( htune_bias + htune_delta ) : ( htune_bias - htune_delta ) );
}

void autotune_check_cycle( void )
{
    uint8_t i;
    int32_t temp_array[HTUNE_CYCLES_MAX];
    uint8_t htune_log_len;
    uint8_t temp_array_len;
    uint8_t start_index;
    int32_t median_bias;
    int32_t nearest_index;
    uint8_t nearest_count;
    int32_t temp;
    uint8_t pass_count;
    int32_t best_asym;
    uint8_t best_index;
    
    htune_log_len = htune_log_index;
//    start_index = htune_log_len >> 1;
    start_index = 0;
    temp_array_len = htune_log_len - start_index;
    
    /* Ignore first half of data. */
    for ( i=start_index; i<htune_log_len; i++ )
        temp_array[i-start_index] = htune_log[i][0];
    
    median_bias = select_kth( temp_array, temp_array_len, temp_array_len >> 1 );
    
    for ( i=0; i<temp_array_len; i++ )
        temp_array[i] = i + start_index;
    
    for ( nearest_count=0; nearest_count<MIN(HTUNE_CONV_COUNT, temp_array_len); nearest_count++ )
    {
        nearest_index = nearest_count;
        
        for ( i=(nearest_count+1); i<temp_array_len; i++ )
        {
            if ( labs( htune_log[ temp_array[i] ][0] - median_bias ) <= labs( htune_log[ temp_array[nearest_index] ][0] - median_bias ) )
                nearest_index = i;
        }
        
        temp = temp_array[nearest_count];
        temp_array[nearest_count] = temp_array[nearest_index];
        temp_array[nearest_index] = temp;
    }
    
    pass_count = 0;
    best_asym = INT32_MAX;
    
    printf( "Nearest %hu to bias %li:\n", nearest_count, median_bias );
    
    for ( i=0; i<nearest_count; i++ )
    {
        int32_t asym_pc = ( htune_log[ temp_array[i] ][1] * 100 ) >> HTUNE_CONV_ASYMMETRY_SHL;
        
        if ( asym_pc <= HTUNE_CONV_ASYMMETRY_THRESHOLD_PC )
        {
            pass_count++;
            
            if ( asym_pc <= best_asym )
            {
                best_asym = asym_pc;
                best_index = temp_array[i];
            }
        }
        
        printf( " index %2li bias %6li asym %6li =%3li%%\n",
                temp_array[i],
                htune_log[ temp_array[i] ][0],
                htune_log[ temp_array[i] ][1],
                asym_pc
              );
    }
    
    printf( "Pass: %hu of %hu\n", pass_count, (uint8_t)HTUNE_CONV_PASS_COUNT_THRESHOLD );
    
    if ( pass_count >= HTUNE_CONV_PASS_COUNT_THRESHOLD )
    {
        htune_ku = htune_log_ku[best_index];
        htune_tu = htune_log_tu[best_index];
        autotune_calculate_pid( htune_ku, htune_tu, &htune_kp, &htune_ki, &htune_kd, &hpid_p, &hpid_i, &hpid_d );
        
        autotune_stop( HTUNE_STATE_FINISHED );
        printf( "Autotune Finished: Success\n" );
    }
    else if ( temp_array_len >= HTUNE_CYCLES_MAX )
    {
        autotune_stop( HTUNE_STATE_FAILED );
        htune_fail = HTUNE_FAIL_CYCLES;
        printf( "Autotune Fail: Max Cycles\n" );
    }
}

void autotune_check_timeout( void )
{
    int16_t temp_c_scaled = heater_temp_c_scaled;
    int32_t temp_rate;
    
    if ( temp_c_scaled == htune_cycle_start_temp_scaled )
        temp_rate = 0;
    else
        temp_rate = ( ( (int32_t)( htune_timer - htune_cycle_start_time ) * HEATER_PERIOD_MS * HEATER_TEMP_SCALE ) /
                      MAX( HEATER_TEMP_SCALE, (int32_t)( temp_c_scaled - htune_cycle_start_temp_scaled ) ) );
    
    if ( labs(temp_rate) > HTUNE_CONV_CYCLE_MAX_MS_PER_DEGC )
    {
        /* Heating/cooling rate slower than expected.  Not enough power? */
        
        autotune_stop( HTUNE_STATE_FAILED );
        htune_fail = HTUNE_FAIL_RATE;
        printf( "Autotune Fail: Rate\n" );
    }
    
//    printf( "Temp Rate: %li / %li\n", temp_rate, HTUNE_CONV_CYCLE_MAX_MS_PER_DEGC );
}

int16_t get_heater_temp( void )
{
    float steinhart;
    float R;
    
    if ( heater_adc_avg > 0 )
    {
        R = ( (float)( (int32_t)HEATER_ADC_MAX << HEATER_ADC_SHIFT ) / heater_adc_avg ) - 1;
        R = HEATER_THERM_RES_R / R;
        steinhart = R / HEATER_THERM_REF_R;                     // (R/Ro)
        steinhart = log(steinhart);                             // ln(R/Ro)
        steinhart /= HEATER_THERM_B_COEFF;                      // 1/B * ln(R/Ro)
        steinhart += 1.0 / ( HEATER_THERM_REF_TEMP + 273.15 );  // + (1/To)
        steinhart = 1.0 / steinhart;                            // Invert
        steinhart -= 273.15;
    }
    else 
        steinhart = 0;
    
    return (int16_t)( steinhart * HEATER_TEMP_SCALE );
}

void timer1_isr( void )
{
    timer1_counter++;
    
/*
    heater_adc_avg = ( ( heater_adc_avg * HEATER_ADC_FILT_MUL ) + ( (uint32_t)( HEATER_ADC_FLT_REG & HEATER_ADC_MAX ) << HEATER_ADC_SHIFT ) ) >> HEATER_ADC_FILT_SHIFT;
    heater_temp_c_scaled = get_heater_temp();
    
    if ( htune_active )
    {
        autotune( false );
        htune_timer++;
    }
    else if ( hpid_state == HPID_STATE_RUNNING )
        heater_pid();
    else
        SET_HEATER_OUTPUT( 0 );
*/

//    ADCON3Lbits.SWCTRG = 1;
//    ADCON3Lbits.SWLCTRG = 0;
//    ADCON3Lbits.SWLCTRG = 1;
//    ADCON3Lbits.SWLCTRG ^= 1;
}

void __attribute__ ( ( interrupt, no_auto_psv ) ) _ADFLTR0Interrupt ( void )
{
    IFS5bits.ADCIF = 0;
    ADCON3Lbits.SWLCTRG = 0;
    IFS7bits.ADFLTR0IF = 0;
    
#if 0
    heater_adc_avg = ( ( heater_adc_avg * HEATER_ADC_FILT_MUL ) + ( (uint32_t)HEATER_ADC_FLT_REG << HEATER_ADC_SHIFT ) ) >> HEATER_ADC_FILT_SHIFT;
//    heater_adc_avg = HEATER_ADC_FLT_REG << HEATER_ADC_SHIFT;
    heater_temp_c_scaled = get_heater_temp() * 10;
    
    if ( !htune_active )
        heater_pid();
    else
        autotune( false );
#elif 1
    heater_adc_avg = ( ( heater_adc_avg * HEATER_ADC_FILT_MUL ) + ( (uint32_t)( HEATER_ADC_FLT_REG & HEATER_ADC_MAX ) << HEATER_ADC_SHIFT ) ) >> HEATER_ADC_FILT_SHIFT;
    heater_temp_c_scaled = get_heater_temp();
    
    if ( htune_active )
    {
        autotune( false );
        htune_timer++;
    }
    else if ( hpid_state == HPID_STATE_RUNNING )
        heater_pid();
    else
        SET_HEATER_OUTPUT( 0 );
#endif
}

void __attribute__ ( ( __interrupt__ , auto_psv ) ) _ADCAN0Interrupt ( void )
{
    uint16_t valADCAN0;
    valADCAN0 = ADCBUF0;
    
//    heater_adc_avg = ( ( heater_adc_avg * HEATER_ADC_FILT_MUL ) + ( valADCAN0 << HEATER_ADC_SHIFT ) ) >> HEATER_ADC_FILT_SHIFT;
    
    IFS5bits.ADCAN0IF = 0;
}

err temp_valid( int16_t temp_c_scaled )
{
    err rc = ERR_OK;
    
    if ( ( temp_c_scaled < HEATER_TEMP_MIN_SCALED ) ||
         ( temp_c_scaled > HEATER_TEMP_MAX_SCALED ) )
        rc = ERR_HEAT_TARGET_INVALID;
    
    return rc;
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

err parse_packet_temp_set_target( uint8_t packet_type, uint8_t *packet_data, uint8_t packet_data_size )
{
    err rc = ERR_OK;
    int16_t temp_target;
    
    if ( packet_data_size != sizeof(temp_target) )    
        rc = ERR_PACKET_INVALID;
    else
    {
        temp_target = (int16_t)PTR_TO_16BIT( packet_data );
        
        rc = temp_valid( temp_target );
        
        if ( rc == ERR_OK )
        {
            hpid_target = temp_target;
            spi_packet_write( packet_type, &rc, 1 );
        }
    }
    
    return rc;
}

err parse_packet_temp_get_target( uint8_t packet_type, uint8_t *packet_data, uint8_t packet_data_size )
{
    err rc = ERR_OK;
    uint8_t return_buf[ sizeof(err) + sizeof(hpid_target) ];
    
    return_buf[0] = ERR_OK;
    COPY_16BIT_TO_PTR_REV( &return_buf[1], hpid_target );
    
    spi_packet_write( packet_type, return_buf, sizeof(return_buf) );
    
    return rc;
}

err parse_packet_temp_get_actual( uint8_t packet_type, uint8_t *packet_data, uint8_t packet_data_size )
{
    err rc = ERR_OK;
    uint8_t return_buf[ sizeof(err) + sizeof(heater_temp_c_scaled) ];
    
    return_buf[0] = ERR_OK;
    COPY_16BIT_TO_PTR_REV( &return_buf[1], heater_temp_c_scaled );
    
    spi_packet_write( packet_type, return_buf, sizeof(return_buf) );
    
    return rc;
}

err parse_packet_pid_set_coeffs( uint8_t packet_type, uint8_t *packet_data, uint8_t packet_data_size )
{
    err rc = ERR_OK;
    uint16_t pid_p;
    uint16_t pid_i;
    uint16_t pid_d;
    
    if ( packet_data_size != ( 3 * sizeof(pid_p) ) )
        rc = ERR_PACKET_INVALID;
    else
    {
        pid_p = PTR_TO_16BIT( &packet_data[0] );
        pid_i = PTR_TO_16BIT( &packet_data[2] );
        pid_d = PTR_TO_16BIT( &packet_data[4] );
        
        HPID_INTERRUPT_OFF();
        hpid_p = pid_p;
        hpid_i = pid_i;
        hpid_d = pid_d;
        HPID_INTERRUPT_ON();
        
        if ( hpid_state == HPID_STATE_UNCONFIGURED )
            hpid_state = HPID_STATE_READY;
    }

    if ( rc == ERR_OK )
        spi_packet_write( packet_type, &rc, 1 );
    
    return rc;
}

err parse_packet_pid_get_coeffs( uint8_t packet_type, uint8_t *packet_data, uint8_t packet_data_size )
{
    err rc = ERR_OK;
    uint16_t coeff;
    uint8_t return_buf[ sizeof(err) + 3*sizeof(coeff) ];
    
    return_buf[0] = ERR_OK;
    
    HPID_INTERRUPT_OFF();
    coeff = (uint16_t)hpid_p;
    COPY_16BIT_TO_PTR_REV( &return_buf[1], coeff );
    coeff = (uint16_t)hpid_i;
    COPY_16BIT_TO_PTR_REV( &return_buf[3], coeff );
    coeff = (uint16_t)hpid_d;
    COPY_16BIT_TO_PTR_REV( &return_buf[5], coeff );
    HPID_INTERRUPT_ON();
    
    spi_packet_write( packet_type, return_buf, sizeof(return_buf) );
    
    return rc;
}

err parse_packet_pid_set_running( uint8_t packet_type, uint8_t *packet_data, uint8_t packet_data_size )
{
    err rc = ERR_OK;
    uint8_t run;
    int16_t bias_temp;
    
    if ( packet_data_size < 1 )
        rc = ERR_PACKET_INVALID;
    {
        run = packet_data[0];

        if ( run )
        {
            if ( packet_data_size == ( 1 + sizeof(bias_temp) ) )
                bias_temp = (int16_t)PTR_TO_16BIT( &packet_data[1] );
            else
                bias_temp = hpid_target;
            
            switch ( hpid_state )
            {
                case HPID_STATE_UNCONFIGURED:
                case HPID_STATE_ERROR:
                    rc = ERR_HEAT_PID_NOT_READY;
                    break;
                case HPID_STATE_READY:
                case HPID_STATE_SUSPENDED:
                {
                    rc = temp_valid( bias_temp );
                    if ( rc == ERR_OK )
                    {
                        hpid_target = bias_temp;
                        heater_pid_start();
                    }
                    break;
                }
                case HPID_STATE_RUNNING:
                    rc = temp_valid( bias_temp );
                    if ( rc == ERR_OK )
                        hpid_target = bias_temp;
                    break;
                default:
                    rc = ERR_ERROR;
            }
        } else if ( hpid_state == HPID_STATE_RUNNING )
            hpid_state = HPID_STATE_READY;
    }
    
    if ( rc == ERR_OK )
        spi_packet_write( packet_type, &rc, 1 );
    
    return rc;
}

err parse_packet_pid_get_status( uint8_t packet_type, uint8_t *packet_data, uint8_t packet_data_size )
{
    err rc = ERR_OK;
    uint8_t return_buf[ sizeof(err) + sizeof(uint8_t) ];
    
    return_buf[0] = ERR_OK;
    return_buf[1] = hpid_state;
    
    spi_packet_write( packet_type, return_buf, sizeof(return_buf) );
    
    return rc;
}

err parse_packet_autotune_set_running( uint8_t packet_type, uint8_t *packet_data, uint8_t packet_data_size )
{
    err rc = ERR_OK;
    uint8_t run;
    int16_t temp_target;
    
    if ( packet_data_size < 1 )
        rc = ERR_PACKET_INVALID;
    {
        run = packet_data[0];

        if ( run )
        {
            if ( packet_data_size == ( 1 + sizeof(temp_target) ) )
                temp_target = (int16_t)PTR_TO_16BIT( &packet_data[1] );
            else
                temp_target = htune_target;

            autotune_start( temp_target );
        } else if ( htune_active )
        {
            autotune_stop( HTUNE_STATE_ABORTED );
            printf( "Autotune User Abort\n" );
        }
    }
    
    if ( rc == ERR_OK )
        spi_packet_write( packet_type, &rc, 1 );
    
    return rc;
}

err parse_packet_autotune_get_running( uint8_t packet_type, uint8_t *packet_data, uint8_t packet_data_size )
{
    err rc = ERR_OK;
    uint8_t return_buf[ sizeof(err) + sizeof(uint8_t) ];
    
    return_buf[0] = ERR_OK;
    return_buf[1] = htune_active ? 1 : 0;
    
    spi_packet_write( packet_type, return_buf, sizeof(return_buf) );
    
    return rc;
}

err parse_packet_autotune_get_status( uint8_t packet_type, uint8_t *packet_data, uint8_t packet_data_size )
{
    err rc = ERR_OK;
    uint8_t return_buf[ sizeof(err) + 2*sizeof(uint8_t) ];
    
    return_buf[0] = ERR_OK;
    return_buf[1] = htune_state;
    return_buf[2] = htune_fail;
    
    spi_packet_write( packet_type, return_buf, sizeof(return_buf) );
    
    return rc;
}

void heater_pid_start( void )
{
    if ( ( hpid_state == HPID_STATE_READY ) || ( hpid_state == HPID_STATE_SUSPENDED ) )
    {
        HPID_INTERRUPT_OFF();

        hpid_integrated = 0;
        hpid_diff = 0;
        hpid_error_prev = 0;

        if ( htune_active )
            htune_state = HTUNE_STATE_ABORTED;
        htune_active = false;
        
        hpid_state = HPID_STATE_RUNNING;
        
        HPID_INTERRUPT_ON();
    }
}

void init( void )
{
    timer1_counter = 0;
    
    HPID_INTERRUPT_OFF();
    SET_HEATER_OUTPUT( 0 );
    hpid_state = HPID_STATE_UNCONFIGURED;
    htune_state = HTUNE_STATE_DEFAULT;
    htune_fail = HTUNE_FAIL_NONE;
    htune_active = false;
    
/*
Temp 34.45 /  15607, Output   2768, p   1147, i    296, d  1111, i_int      9, hdiff      6, pt    985, it      9, dt   1774*/
    
//    hpid_p = 9142 / 3;
//    hpid_i = 28 / 3;
//    hpid_d = 735940 / 3 / 0.125f * 0.3333f;
//    hpid_p = 10095;
//    hpid_i = 34;
//    hpid_d = 11569;
    hpid_p = 0;
    hpid_i = 0;
    hpid_d = 0;
    hpid_counter = 0;
    hpid_windup_limit = HEATER_POWER_MAX;
    hpid_target = 3500;
    
    /* Start ADC level trigger */
//    HPID_INTERRUPT_ON();
    IFS5bits.ADCAN0IF = 0;
    heater_adc_avg = 0;
    heater_temp_c_scaled = 0;
    ADFL0CONbits.RDY = 0;
//    ADFL0CONbits.FLEN = 0;
    ADFL0CONbits.FLEN = 1;
//    ADFL0CONbits.IE = 1;
//    ADCON3Lbits.SWLCTRG = 1;
//    while ( HEATER_ADC_RDY_REG == 0 );
//    IFS7bits.ADFLTR0IF = 0;
//    ADCON3Lbits.SWCTRG = 1;
//    ADCON3Lbits.SWLCTRG = 0;
    ADCON3Lbits.SWLCTRG = 1;
    printf( "Waiting for filter...\n" );
    while(ADFL0CONbits.RDY == 0)
    {
//        IFS5bits.ADCIF = 0;
        printf( "ADC Buf 0=%u, Filter=%u\n", ADCBUF0, ADFL0DAT );
    }
    printf( "Filter Ready\n" );
    
//    while ( HEATER_ADC_FLT_RDY_REG == 0 );
//    heater_adc_avg = HEATER_ADC_FLT_REG << HEATER_ADC_SHIFT;
    
//    hpid_state = HPID_STATE_READY;
}

#define EE_CMD_READ     0b011
#define EE_CMD_WRITE    0b010
#define EE_CMD_WRDI     0b100   // Write Disable
#define EE_CMD_WREN     0b110   // Write Enable
#define EE_CMD_RDSR     0b101   // Read Status Register
#define EE_CMD_WRSR     0b001   // Write Status Register

typedef enum __attribute__((packed))
{
    EE_PROTECT_NONE          = 0,
    EE_PROTECT_UPPER_QUARTER = 1,
    EE_PROTECT_UPPER_HALF    = 2,
    EE_PROTECT_ALL           = 3,
} E_EE_PROTECT;

typedef union
{
    struct
    {
        uint8_t WIP : 1;
        uint8_t WEL : 1;
        E_EE_PROTECT protect : 2;
    };
    uint8_t value;
} ee_status_t;

ee_status_t eeprom_read_status( void )
{
    uint8_t buf[2];
    
    buf[0] = EE_CMD_RDSR;
    buf[1] = 0xFF;
    SS2OUT_SetLow();
    SPI2_Exchange8bitBuffer( buf, 2, buf );
    SS2OUT_SetHigh();
    
//    printf( "Status size %hu\n", sizeof(ee_status_t) );
//    printf( "WIP %hu\n", ((ee_status_t)buf[1]).WIP );
//    printf( "WEL %hu\n", ((ee_status_t)buf[1]).WEL );
//    printf( "BP %hu\n", ((ee_status_t)buf[1]).protect );
    
    return (ee_status_t)buf[1];
}

uint8_t eeprom_read_byte( uint16_t addr )
{
    uint8_t buf[3];
    
    buf[0] = EE_CMD_READ | ( ( addr & 0x100 ) >> 5 );
    buf[1] = addr & 0xFF;
    buf[2] = 0xFF;
    SS2OUT_SetLow();
    SPI2_Exchange8bitBuffer( buf, 3, buf );
    SS2OUT_SetHigh();
    
    return buf[2];
}

void eeprom_read_bytes( uint16_t addr, uint8_t num, uint8_t *data )
{
    uint8_t buf[2];
    
    buf[0] = EE_CMD_READ | ( ( addr & 0x100 ) >> 5 );
    buf[1] = addr & 0xFF;
    SS2OUT_SetLow();
    SPI2_Exchange8bitBuffer( buf, 2, NULL );
    SPI2_Exchange8bitBuffer( NULL, num, data );
    SS2OUT_SetHigh();
}

uint8_t eeprom_verify_bytes( uint16_t addr, uint8_t num, uint8_t *data )
{
    uint8_t buf[2];
    uint8_t rc = 0;
    
    buf[0] = EE_CMD_READ | ( ( addr & 0x100 ) >> 5 );
    buf[1] = addr & 0xFF;
    
    SS2OUT_SetLow();
    
    SPI2_Exchange8bitBuffer( buf, 2, NULL );
    
    while ( num )
    {
        SPI2_Exchange8bitBuffer( NULL, 1, buf );
        
        if ( *buf == *data )
        {
            data++;
            num--;
        }
        else
        {
            rc = 1;
            break;
        }
    }
    
    SS2OUT_SetHigh();
    
    return rc;
}

void eeprom_write_byte( uint16_t addr, uint8_t byte )
{
    uint8_t buf[3];
    
    /* Write Enable */
    buf[0] = EE_CMD_WREN;
    SS2OUT_SetLow();
    SPI2_Exchange8bitBuffer( buf, 1, NULL );
    SS2OUT_SetHigh();
    
    /* Write */
    buf[0] = EE_CMD_WRITE | ( ( addr & 0x100 ) >> 5 );
    buf[1] = addr & 0xFF;
    buf[2] = byte;
    SS2OUT_SetLow();
    SPI2_Exchange8bitBuffer( buf, 3, NULL );
    SS2OUT_SetHigh();
    
//    printf( "Writing" );
    while ( eeprom_read_status().WIP );
//        printf( "." );
//    printf( "\n" );
}

void eeprom_write_bytes( uint16_t addr, uint8_t num, uint8_t *data )
{
    #define PAGE_SIZE 16
    uint8_t count;
    uint8_t buf[2];
    
//    eeprom_print_write( addr, num, data );
    
    while ( num )
    {
        count = PAGE_SIZE - ( addr & ( PAGE_SIZE - 1 ) );
        
        if ( count > num )
            count = num;
        
//        printf( "Write %hu to %hu, %hu bytes\n", addr, addr+count-1, count );
        
        /* Write Enable */
        buf[0] = EE_CMD_WREN;
        SS2OUT_SetLow();
        SPI2_Exchange8bitBuffer( buf, 1, NULL );
        SS2OUT_SetHigh();

        /* Write */
        buf[0] = EE_CMD_WRITE | ( ( addr & 0x100 ) >> 5 );
        buf[1] = addr & 0xFF;
        SS2OUT_SetLow();
        SPI2_Exchange8bitBuffer( buf, 2, NULL );
        SPI2_Exchange8bitBuffer( data, count, NULL );
        SS2OUT_SetHigh();

        while ( eeprom_read_status().WIP );
        
/*
        printf( "Writing" );
        while ( eeprom_read_status().WIP );
            printf( "." );
        printf( "\n" );
 */
        
        num -= count;
        addr += count;
        data += count;
    }
}

void eeprom_print_write( uint16_t addr, uint8_t num, uint8_t *data )
{
    printf( "Write block from %hu to %hu, %hu bytes =", addr, addr+num-1, num );
    
    while ( num-- )
        printf( " %hu", *data++ );
    
    printf( "\n" );
}

void eeprom_read_write_bytes( uint16_t addr, uint8_t num, uint8_t *data )
{
    #define PAGE_SIZE 16
    uint8_t count;
    uint8_t buf[2];
    uint8_t page_buf[PAGE_SIZE];
    uint8_t check_addr;
    uint8_t check_count;
    uint8_t *check_data;
    uint8_t *buf_data;
    uint8_t write_bytes;
    uint8_t *write_addr;
    uint8_t *write_buf_addr;
    
    write_bytes = 0;
    
    while ( num )
    {
        count = PAGE_SIZE - ( addr & ( PAGE_SIZE - 1 ) );
        
        if ( count > num )
            count = num;
        
        eeprom_read_bytes( addr, count, page_buf );
        
        check_addr = addr;
        check_count = count;
        check_data = data;
        buf_data = page_buf;
//        write_bytes = 0;
        
        while ( check_count )
        {
            if ( *check_data != *buf_data )
            {
                if ( write_bytes == 0 )
                {
                    /* Mark start of block to write */
                    
                    write_addr = check_addr;
                    write_buf_addr = check_data;
                }
                
                write_bytes++;
            }
            else if ( write_bytes != 0 )
            {
                /* We have a block to write */
                
                eeprom_write_bytes( write_addr, write_bytes, write_buf_addr );
                write_bytes = 0;
            }
            
            check_addr++;
            check_count--;
            check_data++;
            buf_data++;
        }
        
        if ( write_bytes )
        {
            eeprom_write_bytes( write_addr, write_bytes, write_buf_addr );
            write_bytes = 0;
        }
        
        num -= count;
        addr += count;
        data += count;
    }
    
    if ( write_bytes )
        eeprom_write_bytes( write_addr, write_bytes, write_buf_addr );
}

void eeprom_test( void )
{
    #define TEST_BYTES_NUM 55
    #define TEST_BYTES_START 0
    ee_status_t eeprom_status;
    uint8_t eeprom_byte;
    uint8_t i;
    uint8_t data[TEST_BYTES_NUM];
    
    printf( "EEPROM Test\n" );
    
    eeprom_status = eeprom_read_status();
    printf( "EEPROM Status %hu\n", eeprom_status.value );
    
#if 0
    #define TEST_WRITE_ADDR 4
    eeprom_byte = 66;
    printf( "EEPROM Write %hu\n", eeprom_byte );
    eeprom_write_byte( TEST_WRITE_ADDR, eeprom_byte );
    
    eeprom_byte = eeprom_verify_bytes( TEST_WRITE_ADDR, 1, &eeprom_byte );
    printf( "Verify %s\n", eeprom_byte ? "FAIL" : "OK" );
#endif
    
    eeprom_status = eeprom_read_status();
    printf( "EEPROM Status %hu\n", eeprom_status.value );
    eeprom_byte = eeprom_read_byte( 0 );
    printf( "EEPROM Read %hu\n", eeprom_byte );
    
    for ( i=0; i<50; i++ )
        data[i] = i + 1;
//    eeprom_write_bytes( 1, 50, data );
//    for ( i=13; i<20; i++ )
//        data[i] = i-13;
    eeprom_read_write_bytes( 1, 50, data );
    eeprom_byte = eeprom_verify_bytes( 1, 50, data );
    printf( "Verify %s\n", eeprom_byte ? "FAIL" : "OK" );
    
    printf( "Reading %hu bytes starting at %hu:\n", TEST_BYTES_NUM, TEST_BYTES_START );
    eeprom_read_bytes( TEST_BYTES_START, TEST_BYTES_NUM, data );
    for ( i=0; i<TEST_BYTES_NUM; i++ )
        printf( " %hu", data[i] );
    printf( "\n" );
    
    eeprom_byte = eeprom_verify_bytes( TEST_BYTES_START, TEST_BYTES_NUM, data );
    printf( "Verify %s\n", eeprom_byte ? "FAIL" : "OK" );
    
    printf( "EEPROM Test Finish\n" );
    while (1);
}

int main(void)
{
    err rc = 0;
    err comms_rc;
    uint16_t time;
    uint8_t htune_log_index_last;
    
    SYSTEM_Initialize();
    
    printf( "Starting...\n\n" );
    
    eeprom_test();
    
    init();
    
    /* Init SPI */
    spi_init();
    spi_packet_init( &spi_packet, &timer1_counter, 3 );
    
    TMR1_SetInterruptHandler( timer1_isr );
    
    ADCON3Lbits.SWLCTRG = 1;
    while ( heater_adc_avg == 0 );
    heater_adc_avg = (uint32_t)HEATER_ADC_FLT_REG << HEATER_ADC_SHIFT;
    heater_temp_c_scaled = get_heater_temp();
//    hpid_integrated = ( (int32_t)hpid_target - (int32_t)heater_temp_c_scaled ) * hpid_p;
//    hpid_integrated >>= 1;
//    if ( hpid_integrated > hpid_windup_limit )
//        hpid_integrated = hpid_windup_limit;    
    
//    heater_pid_start();
//    autotune_start( 3500 );
    
    htune_log_index_last = htune_log_index;
    
    while (1)
    {
//        SPI1STATLbits.SPIROV = 0;
        
        if ( ( timer1_counter - time ) >= 10 )
        {
            time = timer1_counter;
            
            if ( htune_active )
            {
                printf( "Temp %0.2f / %6u, Output %6u, heating %1d, bias %5u, delta %5u, cycles %2u, min %3d, max %3d, heattime %6d, cooltime %6d, ku %0.1f, tu %0.3f, p %6li, i %6li, d %5li\n", (float)heater_temp_c_scaled / HEATER_TEMP_SCALE, HEATER_ADC_FLT_REG, heater_output, htune_heating, htune_bias, htune_delta, htune_cycles, htune_temp_min, htune_temp_max, htune_period_heating, htune_period_cooling, htune_ku, htune_tu, htune_p, htune_i, htune_d );
                autotune_check_timeout();
            }
            else
                printf( "Temp %0.2f / %6u, Output %6u, p %6li, i %6li, d %5li, i_int %6li, hdiff %6li, pt %6li, it %6li, dt %6li\n",
                        (float)heater_temp_c_scaled / HEATER_TEMP_SCALE, HEATER_ADC_FLT_REG,
                        heater_output,
                        hpid_p, hpid_i, hpid_d,
                        hpid_integrated >> HTUNE_KI_SHL, hpid_diff >> HEATER_ADC_SHIFT,
                        constrain_i32( ( hpid_error_prev * hpid_p ) >> HTUNE_KP_SHL, -(int32_t)UINT16_MAX, UINT16_MAX ),
                        hpid_integrated >> HTUNE_KI_SHL,
                        constrain_i32( hpid_diff, -(int32_t)UINT16_MAX, UINT16_MAX ) );
            
            if ( htune_log_index > htune_log_index_last )
            {
                htune_log_index_last = htune_log_index;
                
                autotune_check_cycle();
            }
        }
        
        comms_rc = spi_packet_read( &spi_packet, &packet_type, (uint8_t *)&packet_data, &packet_data_size, SPI_PACKET_BUF_SIZE );
        
        if ( ( comms_rc == ERR_OK ) && ( packet_type != 0 ) )
        {
            rc = ERR_OK;
            
            printf( "Packet received: Cmd %hu\n", packet_type );
            spi_clear_write();
            
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
                case PACKET_TYPE_TEMP_SET_TARGET:
                {
                    rc = parse_packet_temp_set_target( packet_type, packet_data, packet_data_size );
                    break;
                }
                case PACKET_TYPE_TEMP_GET_TARGET:
                {
                    rc = parse_packet_temp_get_target( packet_type, packet_data, packet_data_size );
                    break;
                }
                case PACKET_TYPE_TEMP_GET_ACTUAL:
                {
                    rc = parse_packet_temp_get_actual( packet_type, packet_data, packet_data_size );
                    break;
                }
                case PACKET_TYPE_PID_SET_COEFFS:
                {
                    rc = parse_packet_pid_set_coeffs( packet_type, packet_data, packet_data_size );
                    break;
                }
                case PACKET_TYPE_PID_GET_COEFFS:
                {
                    rc = parse_packet_pid_get_coeffs( packet_type, packet_data, packet_data_size );
                    break;
                }
                case PACKET_TYPE_PID_SET_RUNNING:
                {
                    rc = parse_packet_pid_set_running( packet_type, packet_data, packet_data_size );
                    break;
                }
                case PACKET_TYPE_PID_GET_STATUS:
                {
                    rc = parse_packet_pid_get_status( packet_type, packet_data, packet_data_size );
                    break;
                }
                case PACKET_TYPE_AUTOTUNE_SET_RUNNING:
                {
                    rc = parse_packet_autotune_set_running( packet_type, packet_data, packet_data_size );
                    break;
                }
                case PACKET_TYPE_AUTOTUNE_GET_RUNNING:
                {
                    rc = parse_packet_autotune_get_running( packet_type, packet_data, packet_data_size );
                    break;
                }
                case PACKET_TYPE_AUTOTUNE_GET_STATUS:
                {
                    rc = parse_packet_autotune_get_status( packet_type, packet_data, packet_data_size );
                    break;
                }
                default:
                    rc = ERR_PACKET_INVALID;
            }
            
            if ( rc != ERR_OK )
                spi_packet_write( packet_type, &rc, 1 );
        }
        else if ( ( spi_write_bytes_written() > 0 ) && spi_packet_timeout( &spi_packet ) )
        {
            spi_clear_write();
            printf( "Cleared\n" );
        }
    }
}
