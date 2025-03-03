#ifndef __GUI_H_
#define __GUI_H_

#include "sys.h"
#include "ST7789V2.h"
#include "modbus_rtu.h"

#define DIS_ON 			1
#define DIS_OFF 		0

#define BEAT_ON         1
#define BEAT_OFF        0

/*      需要跳动显示的内容   */
#define POWER                1
#define WIND                 2
#define MODE                 3
#define KONG                 4


typedef struct  
{     
    int  fan_level;
    int  power_percentage;       
    int  mode_num;
    int  dht11_temp;
    int  dht11_humidity;
    uint8_t  sync_switch;
    uint8_t  sync_24v_statu;
    uint8_t  mode;
    uint8_t  channel;
}GUI_INFO;

typedef struct 
{
    uint8_t  update_flag;
    uint8_t  beat_allow_flag;
    uint8_t  beat_start_flag;
    uint8_t  beat_select;
    uint8_t  beat_switch; 
    uint8_t  beat_clear;
}GUI_BEAT;

extern GUI_INFO gui_info;
extern GUI_BEAT gui_beat;

void icon_beat(uint8_t pic_code , uint8_t on_off );
void dis_ac220_switch( uint8_t on_off );
void dis_ac220_set( uint8_t on_off );
void dis_led( uint8_t on_off );
void dis_fan( uint8_t on_off );
void dis_bake_power( uint8_t on_off );
void dis_bake_windspeed( uint8_t on_off );
void gui_init( void );
void check_icon( uint8_t icon_select );
void gui_icon_init( void );
void refresh_icon( void );
void sync_switch( void );
void slave_statu_update( void );
void channel_switch( uint8_t mode_num);
void channel_switch1( uint8_t num );
void channel_switch2( uint8_t num );
void channel_switch3( uint8_t num );
void mode_switch( uint8_t num );
void wind_dis( void );
void power_dis( void );
void mode_dis( void );

#endif

