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
#define PACKET_TYPE_AUTOTUNE_SET_RUNNING    7
#define PACKET_TYPE_AUTOTUNE_GET_RUNNING    8
#define PACKET_TYPE_AUTOTUNE_GET_STATUS     9
#define PACKET_TYPE_AUTOTUNE_GET_RESULT     10

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

/* Heater PID Data */
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

/* Autotune Data */
E_HTUNE_STATE htune_state;
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
    htune_temp_max = heater_temp_c_scaled;
    htune_temp_min = htune_temp_max;
    htune_log_index = 0;
    htune_target = target_temp;
    
    /* This also initialises cycle start temp and time. */
    autotune( true );
    
    htune_state = HTUNE_STATE_RUNNING;
    htune_active = true;
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
        heater_pid_start();
        
        htune_active = false;
        htune_state = HTUNE_STATE_FINISHED;
        printf( "Autotune Finished: Success\n" );
    }
    else if ( temp_array_len >= HTUNE_CYCLES_MAX )
    {
        htune_active = false;
        htune_state = HTUNE_STATE_FAILED;
        printf( "Autotune Fail: Max Cycles\n" );
    }
    
    if ( htune_active == false )
        heater_pid_start();
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
        
        htune_active = false;
        htune_state = HTUNE_STATE_FAILED;
        printf( "Autotune Fail: Rate\n" );
        heater_pid_start();
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
    
    heater_adc_avg = ( ( heater_adc_avg * HEATER_ADC_FILT_MUL ) + ( (uint32_t)( HEATER_ADC_FLT_REG & HEATER_ADC_MAX ) << HEATER_ADC_SHIFT ) ) >> HEATER_ADC_FILT_SHIFT;
    heater_temp_c_scaled = get_heater_temp();
    
    if ( !htune_active )
        heater_pid();
    else
    {
        autotune( false );
        htune_timer++;
    }
    
//    ADCON3Lbits.SWCTRG = 1;
    ADCON3Lbits.SWLCTRG = 0;
    ADCON3Lbits.SWLCTRG = 1;
//    ADCON3Lbits.SWLCTRG ^= 1;
}

void __attribute__ ( ( interrupt, no_auto_psv ) ) _ADFLTR0Interrupt ( void )
{
//    ADCON3Lbits.SWLCTRG = 0;
    IFS7bits.ADFLTR0IF = 0;
    
#if 0
    heater_adc_avg = ( ( heater_adc_avg * HEATER_ADC_FILT_MUL ) + ( (uint32_t)HEATER_ADC_FLT_REG << HEATER_ADC_SHIFT ) ) >> HEATER_ADC_FILT_SHIFT;
//    heater_adc_avg = HEATER_ADC_FLT_REG << HEATER_ADC_SHIFT;
    heater_temp_c_scaled = get_heater_temp() * 10;
    
    if ( !htune_active )
        heater_pid();
    else
        autotune( false );
#endif
}

void __attribute__ ( ( __interrupt__ , auto_psv ) ) _ADCAN0Interrupt ( void )
{
    uint16_t valADCAN0;
    valADCAN0 = ADCBUF0;
    
//    heater_adc_avg = ( ( heater_adc_avg * HEATER_ADC_FILT_MUL ) + ( valADCAN0 << HEATER_ADC_SHIFT ) ) >> HEATER_ADC_FILT_SHIFT;
    
    IFS5bits.ADCAN0IF = 0;
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
    
    if ( packet_data_size != sizeof(int16_t) )    
        rc = ERR_PACKET_INVALID;
    else
    {
        temp_target = (int16_t)PTR_TO_16BIT( packet_data );
        
        if ( ( temp_target < HEATER_TEMP_MIN_SCALED ) ||
             ( temp_target > HEATER_TEMP_MAX_SCALED ) )
            rc = ERR_HEAT_TARGET_INVALID;
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

void heater_pid_start( void )
{
    HPID_INTERRUPT_OFF();
    
    hpid_integrated = 0;
    hpid_diff = 0;
    hpid_error_prev = 0;
    
    htune_active = false;
    htune_state = HTUNE_STATE_DEFAULT;
    
    HPID_INTERRUPT_ON();
}

void init( void )
{
    timer1_counter = 0;
    
    HPID_INTERRUPT_OFF();
    
    SET_HEATER_OUTPUT( 0 );
    
/*
Temp 34.45 /  15607, Output   2768, p   1147, i    296, d  1111, i_int      9, hdiff      6, pt    985, it      9, dt   1774*/
    
//    hpid_p = 9142 / 3;
//    hpid_i = 28 / 3;
//    hpid_d = 735940 / 3 / 0.125f * 0.3333f;
//    hpid_p = 10095;
//    hpid_i = 34;
//    hpid_d = 11569;
    hpid_p = 41475;
    hpid_i = 401;
    hpid_d = 16727;
    hpid_counter = 0;
    hpid_windup_limit = HEATER_POWER_MAX;
    hpid_target = 3500;
    
    /* Start ADC level trigger */
//    ADFL0CONbits.FLEN = 0;
//    ADFL0CONbits.FLEN = 1;
//    ADCON3Lbits.SWLCTRG = 1;
//    while ( HEATER_ADC_RDY_REG == 0 );
    IFS7bits.ADFLTR0IF = 0;
    ADFL0CONbits.RDY = 0;
//    ADCON3Lbits.SWCTRG = 1;
//    ADCON3Lbits.SWLCTRG = 1;
    heater_adc_avg = 0;
    heater_temp_c_scaled = 0;
    
//    while ( HEATER_ADC_FLT_RDY_REG == 0 );
//    heater_adc_avg = HEATER_ADC_FLT_REG << HEATER_ADC_SHIFT;
}

int main(void)
{
    err rc = 0;
    uint16_t time;
    uint8_t htune_log_index_last;
    
    SYSTEM_Initialize();
    
    printf( "Starting...\n\n" );
    
    init();
    
    /* Init SPI */
    spi_init();
    spi_packet_clear( &spi_packet );
    
    TMR1_SetInterruptHandler( timer1_isr );
    
    ADCON3Lbits.SWLCTRG = 1;
    while ( heater_adc_avg == 0 );
    heater_adc_avg = (uint32_t)HEATER_ADC_FLT_REG << HEATER_ADC_SHIFT;
    heater_temp_c_scaled = get_heater_temp();
//    hpid_integrated = ( (int32_t)hpid_target - (int32_t)heater_temp_c_scaled ) * hpid_p;
//    hpid_integrated >>= 1;
//    if ( hpid_integrated > hpid_windup_limit )
//        hpid_integrated = hpid_windup_limit;    
    
    heater_pid_start();
    autotune_start( 3500 );
    
    htune_log_index_last = htune_log_index;
    
    while (1)
    {
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
        
        if ( ( spi_packet_read( &spi_packet, &packet_type, (uint8_t *)&packet_data, &packet_data_size, SPI_PACKET_BUF_SIZE ) == ERR_OK ) &&
             ( packet_type != 0 ) )
        {
            rc = ERR_OK;
            
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
                default:
                    rc = ERR_PACKET_INVALID;
            }
            
            if ( rc != ERR_OK )
                spi_packet_write( packet_type, &rc, 1 );
        }
    }
}
