#ifndef __KEY_CRL_H_
#define __KEY_CRL_H_

#include "main.h"
#include "delay.h"
#include "GUI.h"
#include "POWER_CRL.h"

#define KEY1        7
#define KEY2        13  
#define KEY3        11
#define KEY4        14

typedef struct 
{       
    uint8_t key_init_flag;
    uint8_t sync_allow_flag;
    uint8_t key_value_flag;

    uint8_t gui_key2_allow_flag;
    uint8_t gui_key3_allow_flag;
    uint8_t gui_key4_allow_flag;  

    uint8_t key1_press_cnt;
    uint8_t key4_press_cnt;

    uint8_t mode_select;
    uint8_t channel_select;

}KEY;

extern KEY key;

#define B0_VAL HAL_GPIO_ReadPin(D0_B0_GPIO_Port,D0_B0_Pin)
#define B1_VAL HAL_GPIO_ReadPin(D1_B1_GPIO_Port,D1_B1_Pin)
#define B2_VAL HAL_GPIO_ReadPin(D2_B2_GPIO_Port,D2_B2_Pin)
#define B3_VAL HAL_GPIO_ReadPin(D3_B3_GPIO_Port,D3_B3_Pin)

void key_scan( void );

void key_init( void );

void key1_press( void );

void key2_press( void );

void key3_press( void );

void key4_press( void );

void jump_to_init( void );

void mode_channel_switch( void );
#endif
