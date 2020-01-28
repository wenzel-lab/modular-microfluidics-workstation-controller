#ifndef SENSIRION_H
#define	SENSIRION_H

#ifdef	__cplusplus
extern "C" {
#endif

extern err sensirion_write_register( uint8_t addr, uint8_t reg, uint16_t data );
extern err sensirion_read_id( uint8_t addr, uint32_t *product_num, uint64_t *serial );

#ifdef	__cplusplus
}
#endif

#endif	/* SENSIRION_H */

