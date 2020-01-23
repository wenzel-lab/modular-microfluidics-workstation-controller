#include "mcc_generated_files/mcc.h"
#include "common.h"
#include "sensirion.h"

#define I2C_TIMEOUT_MS      2

extern uint8_t sensirion_write_cmd( uint8_t addr, uint16_t cmd )
{
    volatile I2C2_MESSAGE_STATUS status;
    uint8_t writeBuffer[2];
    uint16_t time;
    
    writeBuffer[0] = cmd >> 8;
    writeBuffer[1] = cmd & 0xFF;
    
    I2C2_MasterWrite( writeBuffer, 2, addr, (I2C2_MESSAGE_STATUS *)&status );
    
    time = timer_ms;
    while ( ( status != I2C2_MESSAGE_COMPLETE ) && ( ( timer_ms - time ) <= I2C_TIMEOUT_MS ) );
    if ( status != I2C2_MESSAGE_COMPLETE )
        I2C2_Abort();
    
	return 0;
}

err sensirion_read_id( uint8_t addr, uint32_t *product_num, uint64_t *serial )
{
    err rc = ERR_OK;
    volatile I2C2_MESSAGE_STATUS status;
    I2C2_TRANSACTION_REQUEST_BLOCK trBlocks[3];
    uint16_t cmd1 = 0x7C36;
    uint16_t cmd2 = 0x02E1;
    uint8_t buf[18];
    uint16_t time;

    I2C2_MasterWriteTRBBuild( &trBlocks[0], (void *)&cmd1, 2, addr );
    I2C2_MasterWriteTRBBuild( &trBlocks[1], (void *)&cmd2, 2, addr );
    I2C2_MasterReadTRBBuild( &trBlocks[2], (void *)buf, 18, addr );
    I2C2_MasterTRBInsert( 3, (I2C2_TRANSACTION_REQUEST_BLOCK *)&trBlocks, (I2C2_MESSAGE_STATUS *)&status );
    
    time = timer_ms;
    while ( ( status != I2C2_MESSAGE_COMPLETE ) && ( ( timer_ms - time ) <= I2C_TIMEOUT_MS ) );
    if ( status != I2C2_MESSAGE_COMPLETE )
    {
        rc = ERR_SENSIRION_COMMS_FAIL;
        *product_num = 0;
        *serial = 0;
        I2C2_Abort();
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


