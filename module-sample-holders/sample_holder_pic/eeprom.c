#include <stdio.h>
#include "mcc_generated_files/mcc.h"
#include "eeprom.h"

#define PAGE_SIZE 16

#define EE_CMD_READ     0b011
#define EE_CMD_WRITE    0b010
#define EE_CMD_WRDI     0b100   // Write Disable
#define EE_CMD_WREN     0b110   // Write Enable
#define EE_CMD_RDSR     0b101   // Read Status Register
#define EE_CMD_WRSR     0b001   // Write Status Register

static void eeprom_print_write( uint16_t addr, uint8_t num, uint8_t *data );

extern ee_status_t eeprom_read_status( void )
{
    uint8_t buf[2];
    
    buf[0] = EE_CMD_RDSR;
    buf[1] = 0xFF;
    EE_SS2OUT_SetLow();
    SPI2_Exchange8bitBuffer( buf, 2, buf );
    EE_SS2OUT_SetHigh();
    
//    printf( "Status size %hu\n", sizeof(ee_status_t) );
//    printf( "WIP %hu\n", ((ee_status_t)buf[1]).WIP );
//    printf( "WEL %hu\n", ((ee_status_t)buf[1]).WEL );
//    printf( "BP %hu\n", ((ee_status_t)buf[1]).protect );
    
    return (ee_status_t)buf[1];
}

extern uint8_t eeprom_read_byte( uint16_t addr )
{
    uint8_t buf[3];
    
    buf[0] = EE_CMD_READ | ( ( addr & 0x100 ) >> 5 );
    buf[1] = addr & 0xFF;
    buf[2] = 0xFF;
    EE_SS2OUT_SetLow();
    SPI2_Exchange8bitBuffer( buf, 3, buf );
    EE_SS2OUT_SetHigh();
    
    return buf[2];
}

extern void eeprom_read_bytes( uint16_t addr, uint8_t num, uint8_t *data )
{
    uint8_t buf[2];
    
    buf[0] = EE_CMD_READ | ( ( addr & 0x100 ) >> 5 );
    buf[1] = addr & 0xFF;
    EE_SS2OUT_SetLow();
    SPI2_Exchange8bitBuffer( buf, 2, NULL );
    SPI2_Exchange8bitBuffer( NULL, num, data );
    EE_SS2OUT_SetHigh();
}

extern uint8_t eeprom_verify_bytes( uint16_t addr, uint8_t num, uint8_t *data )
{
    uint8_t buf[2];
    uint8_t rc = 0;
    
    buf[0] = EE_CMD_READ | ( ( addr & 0x100 ) >> 5 );
    buf[1] = addr & 0xFF;
    
    EE_SS2OUT_SetLow();
    
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
    
    EE_SS2OUT_SetHigh();
    
    return rc;
}

extern void eeprom_write_byte( uint16_t addr, uint8_t byte )
{
    uint8_t buf[3];
    
    /* Write Enable */
    buf[0] = EE_CMD_WREN;
    EE_SS2OUT_SetLow();
    SPI2_Exchange8bitBuffer( buf, 1, NULL );
    EE_SS2OUT_SetHigh();
    
    /* Write */
    buf[0] = EE_CMD_WRITE | ( ( addr & 0x100 ) >> 5 );
    buf[1] = addr & 0xFF;
    buf[2] = byte;
    EE_SS2OUT_SetLow();
    SPI2_Exchange8bitBuffer( buf, 3, NULL );
    EE_SS2OUT_SetHigh();
    
//    printf( "Writing" );
    while ( eeprom_read_status().WIP );
//        printf( "." );
//    printf( "\n" );
}

extern void eeprom_write_bytes( uint16_t addr, uint8_t num, uint8_t *data )
{
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
        EE_SS2OUT_SetLow();
        SPI2_Exchange8bitBuffer( buf, 1, NULL );
        EE_SS2OUT_SetHigh();

        /* Write */
        buf[0] = EE_CMD_WRITE | ( ( addr & 0x100 ) >> 5 );
        buf[1] = addr & 0xFF;
        EE_SS2OUT_SetLow();
        SPI2_Exchange8bitBuffer( buf, 2, NULL );
        SPI2_Exchange8bitBuffer( data, count, NULL );
        EE_SS2OUT_SetHigh();

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

extern void eeprom_read_write_bytes( uint16_t addr, uint8_t num, uint8_t *data )
{
    uint8_t count;
    uint8_t page_buf[PAGE_SIZE];
    uint16_t check_addr;
    uint8_t check_count;
    uint8_t *check_data;
    uint8_t *buf_data;
    uint8_t write_bytes;
    uint8_t write_addr;
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

static void eeprom_print_write( uint16_t addr, uint8_t num, uint8_t *data )
{
    printf( "Write block from %hu to %hu, %hu bytes =", addr, addr+num-1, num );
    
    while ( num-- )
        printf( " %hu", *data++ );
    
    printf( "\n" );
}
