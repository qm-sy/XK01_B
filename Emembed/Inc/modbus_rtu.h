#ifndef __MODBUS_RTU_H_
#define __MODBUS_RTU_H_

#include "usart.h"
#include "main.h"
#include "PWM_CRL.h"
#include "communication.h"
#include "delay.h"
#include "GUI.h"

typedef struct  
{
    uint8_t modbus_send_buf[48];
    uint8_t modbus_rcv_buf[48];

    uint8_t NTC1_current_value;     //30001
    uint8_t NTC2_current_value; 
    uint8_t NTC3_current_value;     //30002
    uint8_t NTC4_current_value;    
    uint8_t IR1_adc_value;          //30003
    uint8_t IR2_adc_value;          
    uint8_t I_out1_value;           //30004
    uint8_t I_out2_value;           //30004
    uint8_t I_out3_value;           //30005

    uint8_t PWM_info;               //40001
    uint8_t LED_info;               //40002
    uint8_t AC220_info;             //40003
    uint8_t NTC1_alarm_value;       //40004
    uint8_t NTC2_alarm_value;       //40004
    uint8_t NTC3_alarm_value;       //40005

    uint8_t modbus_04_scan_flag;
    uint8_t modbus_04_scan_allow;

    uint8_t slave1_flag;
    uint8_t slave2_flag;
    uint8_t slave3_flag;

    uint8_t slave_send_flag;
}MODBUS_INFO;

extern MODBUS_INFO modbus;

void Modbus_Fun3( void );

void Modbus_Fun4_slave1( void );
void Modbus_Fun4_slave2( void );
void Modbus_Fun4_slave3( void );

void Modbus_Fun6( void );

void Modbus_Fun16( void );

void Modbus_Event( void );

void modbus_send_data( uint8_t *buf , uint8_t len );

uint8_t modbus_wait_receive( void );

uint16_t MODBUS_CRC16(uint8_t *buf, uint8_t length);

void slave_statu_query_modify( uint8_t fun, uint16_t reg_addr,uint16_t reg_num,uint16_t reg_val );
void test_hanshu(void);
void Modbus_fun03_Master( uint16_t reg_addr,uint16_t reg_num );
void Modbus_fun04_Master( uint16_t reg_addr,uint16_t reg_num );
void Modbus_fun06_Master( uint16_t reg_addr,uint16_t reg_num, uint16_t reg_val );
void send_param( void );
void get_slave_init_statu_multifunpower( void );
// void get_slave1_init_statu_multifunpower( void )
// void get_slave2_init_statu_multifunpower( void )
// void get_slave3_init_statu_multifunpower( void )
void get_slave_current_statu_multifunpower( void );
void get_slave1_current_statu_multifunpower( void );
void get_slave2_current_statu_multifunpower( void );
void get_slave3_current_statu_multifunpower( void );
void write_slave_reg( void );
void write_slave1_reg( void );
void write_slave2_reg( void );
void write_slave3_reg( void );
void mode1_off( void );
void mode2_off( void );
void mode3_off( void );
#endif
