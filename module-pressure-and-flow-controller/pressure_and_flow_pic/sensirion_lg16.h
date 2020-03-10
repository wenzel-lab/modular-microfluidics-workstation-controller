#ifndef SENSIRION_H
#define	SENSIRION_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#define SENSIRION_FLOW_SCALE_ML_MIN     500
#define SENSIRION_TEMP_SCALE_DEGC       200

typedef struct __attribute__((packed))
{
    uint8_t air_in_line : 1;
    uint8_t high_flow : 1;
    uint8_t : 3;
    uint8_t exp_smoothing_active : 1;
    uint16_t : 10;
} sensirion_flags_t;

extern err sensirion_read_id( uint32_t *product_num, uint64_t *serial );

extern err sensirion_measurement_start( void );
extern err sensirion_measurement_stop( void );
extern err sensirion_measurement_read( int16_t *flow, int16_t *temp, sensirion_flags_t *flags );

#ifdef	__cplusplus
}
#endif

#endif	/* SENSIRION_H */

