#include <stdio.h>
#include "mcc_generated_files/mcc.h"
#include "common.h"
#include "storage.h"
#include "eeprom.h"

#define GET_STORE_OFFSET( member )  offsetof( store_t, member )

typedef struct __attribute__((packed))
{
    uint8_t eeprom_ver;
} store_t;

extern err store_save_eeprom_ver( uint8_t eeprom_ver )
{
    err rc = ERR_OK;
    uint8_t verify_rc;
    
    eeprom_read_write_bytes( GET_STORE_OFFSET(eeprom_ver), sizeof(eeprom_ver), &eeprom_ver );
    verify_rc = eeprom_verify_bytes( GET_STORE_OFFSET(eeprom_ver), sizeof(eeprom_ver), &eeprom_ver );
    
    rc = ( verify_rc == 0 ) ? ERR_OK : ERR_EEPROM_VERIFY_FAIL;
    
    return rc;
}

extern err store_load_eeprom_ver( uint8_t *eeprom_ver_p )
{
    err rc = ERR_OK;
    uint8_t eeprom_ver;
    
    eeprom_read_bytes( GET_STORE_OFFSET(eeprom_ver), sizeof(eeprom_ver), (uint8_t *)&eeprom_ver );
    
    *eeprom_ver_p = eeprom_ver;
    
    return rc;
}
