#ifndef STORAGE_H
#define	STORAGE_H

#ifdef	__cplusplus
extern "C" {
#endif

#define EEPROM_VER  1

extern err store_save_eeprom_ver( uint8_t eeprom_ver );

extern err store_load_eeprom_ver( uint8_t *eeprom_ver_p );

#ifdef	__cplusplus
}
#endif

#endif	/* STORAGE_H */

