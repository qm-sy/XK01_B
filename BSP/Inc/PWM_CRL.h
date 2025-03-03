#ifndef __PWM_CRL_H_
#define __PWM_CRL_H_

#include "tim.h"
#include "sys.h"
#include "modbus_rtu.h"
#include "communication.h"

typedef struct  
{
    uint8_t  modbus_03[8];
    uint8_t  modbus_06[8];
    uint8_t  fan_info;
    uint8_t  fan_pwm7_level;         //本设备从机地址
    uint8_t  fan_pwm8_level;         //本设备从机地址
    uint8_t  fan_pwm7_statu;       //modbus数据持续时间
    uint8_t  fan_pwm8_statu;
    uint8_t  slave_send_buf[8];
}SLAVE_PWM;

extern SLAVE_PWM slave_pwm;

void pwm_crl( uint8_t pwm1_dutycycle, uint8_t pwm2_dutycycle, uint8_t pwm3_dutycycle,uint32_t frequency);
void slave_fan_statu_query( void );
void slave_fan_statu_modify( void );

#endif
