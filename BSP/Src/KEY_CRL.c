#include "KEY_CRL.h"

KEY key;

void key_init( void )
{
    key.key_init_flag = 1;

    key.gui_key2_allow_flag = 1;
    key.gui_key3_allow_flag = 1;
    key.gui_key4_allow_flag = 0;  

    key.sync_allow_flag = 0;

    key.key1_press_cnt = 0;
    key.key4_press_cnt = 0;

    key.mode_select    = 0;
    key.channel_select = 0;


}

void jump_to_init( void )
{
    /*       按键2、3恢复调节风扇功率功能     */
    key.gui_key2_allow_flag = 1;
    key.gui_key3_allow_flag = 1;

    /*       按键2、3恢复调节风扇功率功能     */
    key.key1_press_cnt = 0;
    key.key4_press_cnt = 0;
    
    /*       无图标跳动     */
    gui_beat.beat_switch = 0;

    /*       可继续查询从机状态     */
    modbus.modbus_04_scan_allow = 1;

    /*       复位图标跳动起始点     */
    gui_beat.beat_select = KONG;
}


static void buzzer_open( void )
{
    HAL_GPIO_WritePin(BUZZER_GPIO_Port,BUZZER_Pin,GPIO_PIN_SET);
}

static void buzzer_close( void )
{
    delay_ms(150);
    HAL_GPIO_WritePin(BUZZER_GPIO_Port,BUZZER_Pin,GPIO_PIN_RESET);
}


void key_scan( void )
{
    uint8_t key_value;
    uint8_t key4_delay_cnt = 100;

    if(key.key_value_flag == 1)
	{
		key_value = (B0_VAL) | (B1_VAL<<1) | (B2_VAL<<2) | (B3_VAL<<3);
	}
    switch (key_value)
    {
        case KEY1:      key1_press();       break;
        case KEY3:      key3_press();       break;
        case KEY2:      key2_press();       break;

        case KEY4:    
            while((key_value == KEY4) && (key4_delay_cnt!=0)) 
            {
                key_value = (B0_VAL) | (B1_VAL<<1) | (B2_VAL<<2) | (B3_VAL<<3);
                key4_delay_cnt--;
                delay_ms(10); 
            }
            if( key4_delay_cnt == 0 )
            {
                buzzer_open();
                if( gui_beat.beat_switch == 0 )
                {
                    key.sync_allow_flag = 1 - key.sync_allow_flag;
                    sync_switch();
                }
                buzzer_close();
            }else
            {
                key4_press(); 
            }
                  
            break;

        default:
            break;
    }
}



void key1_press( void )
{
    buzzer_open();
    refresh_icon();
    switch (gui_beat.beat_select)
    {
        case MODE:
            key.key1_press_cnt = 1;
            gui_beat.beat_select = key.key1_press_cnt;
            gui_beat.beat_switch = BEAT_ON;

            key.gui_key2_allow_flag = 1;
            key.gui_key3_allow_flag = 1;

            modbus.modbus_04_scan_allow = 0;

            key.gui_key4_allow_flag = 1;
            break;

        default:
            key.key1_press_cnt++;

            gui_beat.beat_select = key.key1_press_cnt;
            gui_beat.beat_switch = BEAT_ON;

            key.gui_key2_allow_flag = 1;
            key.gui_key3_allow_flag = 1;

            modbus.modbus_04_scan_allow = 0;

            key.gui_key4_allow_flag = 1;
            break;
    }
    
    buzzer_close();
    key.key_value_flag = 0;

}

// void alarm_temp_set( void )
// {
//     buzzer_open();

//     key.key1_press_cnt++;
//     gui_beat.beat_select = key.key1_press_cnt;
//     gui_beat.beat_switch = BEAT_ON;
//     key.gui_key2_allow_flag = 1;
//     key.gui_key3_allow_flag = 1;
//     modbus.modbus_04_scan_allow = 0;

// }
void key2_press()
{
    buzzer_open();

    if( gui_beat.beat_switch == 0 )
    {
        key.mode_select++;
        if( key.mode_select > 4 )
        {
            key.mode_select = 0;
        }
        mode_switch(key.mode_select);
        write_slave_reg();
    }

    if( key.gui_key2_allow_flag == 1 )
    {
        switch(gui_beat.beat_select)
        {
            case POWER:
                gui_info.power_percentage += 5;
                if( gui_info.power_percentage >= 100 )
                {
                    gui_info.power_percentage = 100;
                }
                break;

            case WIND:
                gui_info.fan_level += 1;
                if( gui_info.fan_level >= 6 )
                {
                    gui_info.fan_level = 6;
                }

            case MODE:
                gui_info.mode_num += 1;
                if( gui_info.mode_num >= 3 )
                {
                    gui_info.mode_num = 3;
                }
                break;



            default:
                break;
        }
    }

    buzzer_close();
    key.key_value_flag = 0;
}

void key3_press()
{
    
    buzzer_open();

    if( gui_beat.beat_switch == 0 )
    {
        key.channel_select++;
        if( key.channel_select > 4 )
        {
            key.channel_select = 0;
        }
        channel_switch(key.mode_select);
        write_slave_reg();
    }

    if( key.gui_key3_allow_flag == 1 )
    {
        switch(gui_beat.beat_select)
        {
            case POWER:
                gui_info.power_percentage -= 5;
                if( gui_info.power_percentage <= 10 )
                {
                    gui_info.power_percentage = 10;
                }
                break;

            case WIND:
                gui_info.fan_level -= 1;
                if( gui_info.fan_level <= 0 )
                {
                    gui_info.fan_level = 0;
                }

            case MODE:
                gui_info.mode_num -= 1;
                if( gui_info.mode_num <= 1 )
                {
                    gui_info.mode_num = 1;
                }
                break;

            default:
                break;
        }
    }

    buzzer_close();
    key.key_value_flag = 0;
}

void key4_press()
{
    buzzer_open();

    key.key4_press_cnt++;
    if( key.key4_press_cnt == 2 )
        {
            jump_to_init();
            write_slave_reg();
        }
    if( key.gui_key4_allow_flag == 1 )
    {
        switch (gui_beat.beat_select)
        {
            case POWER:
                write_slave_reg();
                jump_to_init();
            break;

            case WIND:
                write_slave_reg();
                jump_to_init();
            break;

            case MODE:
                mode_channel_switch();
                write_slave_reg();
                jump_to_init();
                break;
            
            default:
                break;
        } 
    }
    buzzer_close();
    key.key_value_flag = 0;
}

void mode_channel_switch( void )
{
    if( gui_info.mode_num == 1)
    {
        gui_info.power_percentage = 30;
        gui_info.fan_level = 3;
        wind_dis();
        power_dis(); 
    }
    else if( gui_info.mode_num == 2 )
    {
        gui_info.power_percentage = 50;
        gui_info.fan_level = 4;
        wind_dis();
        power_dis(); 
    }else
    {
        gui_info.power_percentage = 80;
        gui_info.fan_level = 6;
        wind_dis();
        power_dis(); 
    }
}

