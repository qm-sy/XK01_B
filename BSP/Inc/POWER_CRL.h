#ifndef __POWER_CRL_H_
#define __POWER_CRL_H_

#include "main.h"

#define	POWER_CH1(n)    (n?HAL_GPIO_WritePin(AC_OUT1_GPIO_Port,AC_OUT1_Pin,GPIO_PIN_RESET):HAL_GPIO_WritePin(AC_OUT1_GPIO_Port,AC_OUT1_Pin,GPIO_PIN_SET))
#define	POWER_CH2(n)    (n?HAL_GPIO_WritePin(AC_OUT2_GPIO_Port,AC_OUT2_Pin,GPIO_PIN_RESET):HAL_GPIO_WritePin(AC_OUT2_GPIO_Port,AC_OUT2_Pin,GPIO_PIN_SET))
#define	POWER_CH3(n)    (n?HAL_GPIO_WritePin(AC_OUT3_GPIO_Port,AC_OUT3_Pin,GPIO_PIN_RESET):HAL_GPIO_WritePin(AC_OUT3_GPIO_Port,AC_OUT3_Pin,GPIO_PIN_SET))
#define	POWER_CH4(n)    (n?HAL_GPIO_WritePin(AC_OUT4_GPIO_Port,AC_OUT4_Pin,GPIO_PIN_RESET):HAL_GPIO_WritePin(AC_OUT4_GPIO_Port,AC_OUT4_Pin,GPIO_PIN_SET))


extern volatile uint8_t zero_flag;
extern volatile uint8_t power_ch1_statu;
extern volatile uint8_t power_ch2_statu;
extern volatile uint8_t power_ch3_statu;
extern volatile uint8_t power_ch4_statu;
extern volatile uint16_t power_phase_delay;
extern volatile uint8_t buzzer_flag;
void power_crl(uint16_t power_num);
void buzzer_crl( void );

#endif
