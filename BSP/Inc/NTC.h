#ifndef __NTC_H_
#define __NTC_H_

#include "adc.h"

uint8_t *get_temp( void );
uint8_t LookupTable( uint16_t *temp_tab, uint8_t tab_num, uint16_t adc_val );

#endif
