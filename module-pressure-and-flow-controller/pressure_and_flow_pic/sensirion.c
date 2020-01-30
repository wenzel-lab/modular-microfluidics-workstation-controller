#include "mcc_generated_files/mcc.h"
#include <stdio.h>
#include "common.h"
#include "sensirion.h"

#define I2C_ADDR            8
#define I2C_TIMEOUT_MS      2

/* Static Prototypes */
static err i2c_wait_for_reply( volatile I2C2_MESSAGE_STATUS *status );

/* Extern Functions */

extern err sensirion_read_id( uint32_t *product_num, uint64_t *serial )
{
    err rc = ERR_OK;
    volatile I2C2_MESSAGE_STATUS status;
    I2C2_TRANSACTION_REQUEST_BLOCK trBlocks[3];
    uint16_t cmd1 = 0x7C36;
    uint16_t cmd2 = 0x02E1;
    uint8_t buf[18];

    I2C2_MasterWriteTRBBuild( &trBlocks[0], (void *)&cmd1, 2, I2C_ADDR );
    I2C2_MasterWriteTRBBuild( &trBlocks[1], (void *)&cmd2, 2, I2C_ADDR );
    I2C2_MasterReadTRBBuild( &trBlocks[2], (void *)buf, 18, I2C_ADDR );
    I2C2_MasterTRBInsert( 3, (I2C2_TRANSACTION_REQUEST_BLOCK *)&trBlocks, (I2C2_MESSAGE_STATUS *)&status );
    rc = i2c_wait_for_reply( &status );
    
    if ( rc != ERR_OK )
    {
        *product_num = 0;
        *serial = 0;
    }
    else
    {
        ((uint8_t *)product_num)[0] = buf[4];
        ((uint8_t *)product_num)[1] = buf[3];
        ((uint8_t *)product_num)[2] = buf[1];
        ((uint8_t *)product_num)[3] = buf[0];
        ((uint8_t *)serial)[0] = buf[16];
        ((uint8_t *)serial)[1] = buf[15];
        ((uint8_t *)serial)[2] = buf[13];
        ((uint8_t *)serial)[3] = buf[12];
        ((uint8_t *)serial)[4] = buf[10];
        ((uint8_t *)serial)[5] = buf[9];
        ((uint8_t *)serial)[6] = buf[7];
        ((uint8_t *)serial)[7] = buf[6];
    }
    
    return rc;
}

extern err sensirion_measurement_start( void )
{
    err rc = ERR_OK;
    volatile I2C2_MESSAGE_STATUS status;
    uint16_t start_cmd;
    
    start_cmd = 0x0836;    // Start WATER measurement
    
    I2C2_MasterWrite( (void *)&start_cmd, 2, I2C_ADDR, (I2C2_MESSAGE_STATUS *)&status );
    rc = i2c_wait_for_reply( &status );
    
	return rc;
}

extern err sensirion_measurement_stop( void )
{
    err rc = ERR_OK;
    volatile I2C2_MESSAGE_STATUS status;
    uint16_t stop_cmd;
    
    stop_cmd = 0xF93F;
    
    I2C2_MasterWrite( (void *)&stop_cmd, 2, I2C_ADDR, (I2C2_MESSAGE_STATUS *)&status );
    rc = i2c_wait_for_reply( &status );
    
	return rc;
}

extern err sensirion_measurement_read( int16_t *flow, int16_t *temp, sensirion_flags_t *flags )
{
    err rc = ERR_OK;
    volatile I2C2_MESSAGE_STATUS status;
    I2C2_TRANSACTION_REQUEST_BLOCK trBlocks[1];
    uint8_t buf[9];

    I2C2_MasterReadTRBBuild( &trBlocks[0], (void *)buf, 9, I2C_ADDR );
    I2C2_MasterTRBInsert( 1, (I2C2_TRANSACTION_REQUEST_BLOCK *)&trBlocks, (I2C2_MESSAGE_STATUS *)&status );
    rc = i2c_wait_for_reply( &status );
    
    if ( rc != ERR_OK )
    {
        *flow = 0;
        *temp = 0;
        *(uint16_t*)flags = 0;
    }
    else
    {
        ((uint8_t *)flow)[0] = buf[1];
        ((uint8_t *)flow)[1] = buf[0];
        ((uint8_t *)temp)[0] = buf[4];
        ((uint8_t *)temp)[1] = buf[3];
        ((uint8_t *)flags)[0] = buf[7];
        ((uint8_t *)flags)[1] = buf[6];
    }
    
    /*
    printf( "Flow %i\n", *flow );
    printf( "Temp %0.2f\n", (float)*temp / SENSIRION_TEMP_SCALE_DEGC );
    printf( "Flags %u\n", *flags );
    printf( "Air In Line %hu\n", flags->air_in_line );
    printf( "High Flow %hu\n", flags->high_flow );
    printf( "Exp Smoothing Active %hu\n", flags->exp_smoothing_active );
    */
    
    return rc;
}

/* Static Functions */

static err i2c_wait_for_reply( volatile I2C2_MESSAGE_STATUS *status )
{
    err rc = ERR_OK;
    uint16_t time;
    
    time = timer_ms;
    while ( ( *status != I2C2_MESSAGE_COMPLETE ) && ( ( timer_ms - time ) <= I2C_TIMEOUT_MS ) );
    if ( *status != I2C2_MESSAGE_COMPLETE )
    {
        rc = ERR_SENSIRION_COMMS_FAIL;
        I2C2_Abort();
    }
    
    return rc;
}

