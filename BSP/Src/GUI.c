#include "GUI.h"
#include "pic.h"

GUI_INFO gui_info;
GUI_BEAT gui_beat;

void power_dis( void )
{
    LCD_ShowNum(63,195,gui_info.power_percentage,3,24,POINT_COLOR,BACK_COLOR);
}

void wind_dis( void )
{
    LCD_ShowNum(179,195,gui_info.fan_level,1,24,POINT_COLOR,BACK_COLOR);
}

void mode_dis( void )
{
    LCD_ShowNum(288,195,gui_info.mode_num,1,24,POINT_COLOR,BACK_COLOR);
}

static void power_beat( void )
{
    if( gui_beat.beat_clear == 1 )
    {
        LCD_Fill(63,195,99,219,WHITE);
    }else
    {
        power_dis();
    }  
}

static void wind_beat( void )
{
    if( gui_beat.beat_clear == 1 )
    {
        LCD_Fill(179,195,191,219,WHITE);
    }else
    {
        wind_dis();
    }  
}

static void mode_beat( void )
{
    if( gui_beat.beat_clear == 1 )
    {
        LCD_Fill(288,195,300,219,WHITE);
    }else
    {
        mode_dis();
    }  
}

void gui_init( void )
{
    gui_info.fan_level              = 3;
    gui_info.power_percentage       = 50;  
    gui_info.mode_num               = 2; 
    gui_info.dht11_temp             = 25;
    gui_info.dht11_humidity         = 65;
    gui_info.sync_switch            = 1;

    gui_beat.beat_allow_flag        = 0;
    gui_beat.beat_start_flag        = 0;
    gui_beat.beat_select            = 6;
    gui_beat.beat_switch            = BEAT_OFF;
    gui_beat.beat_clear             = 0;
}

void icon_beat(uint8_t pic_code , uint8_t on_off )
{
    if( on_off == 1 )
    {
        gui_beat.beat_allow_flag  = 1;
    }else
    {
        gui_beat.beat_allow_flag  = 0;
    }

    if( gui_beat.beat_start_flag == 1)
    {
        gui_beat.beat_clear = 1 -  gui_beat.beat_clear;
        switch (pic_code)
        {
            case POWER:
                power_beat();
                break;

            case WIND:
                power_dis();
                wind_beat();
                break;

            case MODE:
                wind_dis();
                mode_beat();
                break;
                 
            case KONG:
                mode_dis();
                refresh_icon();  
                write_slave_reg();
                break;

            default:
                break;
        }
    }
    /* 				绛夊緟300ms鍐濇?″埛鏂帮紝闂?鐑濇晥鏋? 				*/
    gui_beat.beat_start_flag = 0; 
}


void mode_switch( uint8_t mode_num )
{
    key.channel_select = 0;
    switch (mode_num)
    {
        case 0:
        LCD_Show_Image_Internal_Flash(16,55,33,22,gImage_dry1_on,1452);
        LCD_Show_Image_Internal_Flash(16,97,32,22,gImage_dry2_off,1408);
        LCD_Show_Image_Internal_Flash(16,138,32,22,gImage_dry3_off,1408);
        channel_switch2(5);
        channel_switch3(5);
        channel_switch1(0);
        break;

        case 1:
        LCD_Show_Image_Internal_Flash(16,55,33,22,gImage_dry1_off,1452);
        LCD_Show_Image_Internal_Flash(16,97,32,22,gImage_dry2_on,1408);
        LCD_Show_Image_Internal_Flash(16,138,32,22,gImage_dry3_off,1408);
        channel_switch1(5);
        channel_switch3(5);
        channel_switch2(0);

        break;

        case 2:
        LCD_Show_Image_Internal_Flash(16,55,33,22,gImage_dry1_off,1452);
        LCD_Show_Image_Internal_Flash(16,97,32,22,gImage_dry2_off,1408);
        LCD_Show_Image_Internal_Flash(16,138,32,22,gImage_dry3_on,1408);
        channel_switch1(5);
        channel_switch2(5);
        channel_switch3(0);
        break;

        case 3:
        LCD_Show_Image_Internal_Flash(16,55,33,22,gImage_dry1_on,1452);
        LCD_Show_Image_Internal_Flash(16,97,32,22,gImage_dry2_on,1408);
        LCD_Show_Image_Internal_Flash(16,138,32,22,gImage_dry3_off,1408);
        channel_switch3(5);
        channel_switch1(0);
        channel_switch2(0);
    
        break;

        case 4:
        LCD_Show_Image_Internal_Flash(16,55,33,22,gImage_dry1_on,1452);
        LCD_Show_Image_Internal_Flash(16,97,32,22,gImage_dry2_on,1408);
        LCD_Show_Image_Internal_Flash(16,138,32,22,gImage_dry3_on,1408);
        channel_switch1(0);
        channel_switch2(0);
        channel_switch3(0);
    
        break;
        
        default:
            break;
    }
}

void channel_switch( uint8_t channel_num)
{
    switch (key.mode_select)
    {
        case 0:
            channel_switch1(key.channel_select);
            break;

        case 1:
            channel_switch2(key.channel_select);
            break;

        case 2:
            channel_switch3(key.channel_select);
            break;

        case 3:
            channel_switch1(key.channel_select);
            channel_switch2(key.channel_select);
            break;

        case 4:
            channel_switch1(key.channel_select);
            channel_switch2(key.channel_select);
            channel_switch3(key.channel_select);
            break;

        default:
            break;
    }
    

}
void channel_switch1( uint8_t channel_num )
{
    switch (channel_num)
    {
        case 0 :
        LCD_Show_Image_Internal_Flash(85,55,46,22,gImage_channel_on,2024);
        LCD_Show_Image_Internal_Flash(145,55,46,22,gImage_channel_off,2024);
        LCD_Show_Image_Internal_Flash(220,55,46,22,gImage_channel_off,2024);
        break;
        
        case 1 :
        LCD_Show_Image_Internal_Flash(85,55,46,22,gImage_channel_off,2024);
        LCD_Show_Image_Internal_Flash(145,55,46,22,gImage_channel_on,2024);
        LCD_Show_Image_Internal_Flash(220,55,46,22,gImage_channel_off,2024);
        break;

        case 2 :
        LCD_Show_Image_Internal_Flash(85,55,46,22,gImage_channel_off,2024);
        LCD_Show_Image_Internal_Flash(145,55,46,22,gImage_channel_off,2024);
        LCD_Show_Image_Internal_Flash(220,55,46,22,gImage_channel_on,2024);
        break;

        case 3 :
        LCD_Show_Image_Internal_Flash(85,55,46,22,gImage_channel_on,2024);
        LCD_Show_Image_Internal_Flash(145,55,46,22,gImage_channel_on,2024);
        LCD_Show_Image_Internal_Flash(220,55,46,22,gImage_channel_off,2024);
        break;

        case 4 :
        LCD_Show_Image_Internal_Flash(85,55,46,22,gImage_channel_on,2024);
        LCD_Show_Image_Internal_Flash(145,55,46,22,gImage_channel_on,2024);
        LCD_Show_Image_Internal_Flash(220,55,46,22,gImage_channel_on,2024);
        break;

        case 5 :
        LCD_Show_Image_Internal_Flash(85,55,46,22,gImage_channel_off,2024);
        LCD_Show_Image_Internal_Flash(145,55,46,22,gImage_channel_off,2024);
        LCD_Show_Image_Internal_Flash(220,55,46,22,gImage_channel_off,2024);
        break;

        default:
            break;
    }
}

void channel_switch2( uint8_t channel_num )
{
    switch (channel_num)
    {
        case 0 :
        LCD_Show_Image_Internal_Flash(85,97,46,22,gImage_channel_on,2024);
        LCD_Show_Image_Internal_Flash(145,97,46,22,gImage_channel_off,2024);
        LCD_Show_Image_Internal_Flash(220,97,46,22,gImage_channel_off,2024);
        break;
        
        case 1 :
        LCD_Show_Image_Internal_Flash(85,97,46,22,gImage_channel_off,2024);
        LCD_Show_Image_Internal_Flash(145,97,46,22,gImage_channel_on,2024);
        LCD_Show_Image_Internal_Flash(220,97,46,22,gImage_channel_off,2024);
        break;

        case 2 :
        LCD_Show_Image_Internal_Flash(85,97,46,22,gImage_channel_off,2024);
        LCD_Show_Image_Internal_Flash(145,97,46,22,gImage_channel_off,2024);
        LCD_Show_Image_Internal_Flash(220,97,46,22,gImage_channel_on,2024);
        break;

        case 3 :
        LCD_Show_Image_Internal_Flash(85,97,46,22,gImage_channel_on,2024);
        LCD_Show_Image_Internal_Flash(145,97,46,22,gImage_channel_on,2024);
        LCD_Show_Image_Internal_Flash(220,97,46,22,gImage_channel_off,2024);
        break;

        case 4 :
        LCD_Show_Image_Internal_Flash(85,97,46,22,gImage_channel_on,2024);
        LCD_Show_Image_Internal_Flash(145,97,46,22,gImage_channel_on,2024);
        LCD_Show_Image_Internal_Flash(220,97,46,22,gImage_channel_on,2024);
        break;

        case 5 :
        LCD_Show_Image_Internal_Flash(85,97,46,22,gImage_channel_off,2024);
        LCD_Show_Image_Internal_Flash(145,97,46,22,gImage_channel_off,2024);
        LCD_Show_Image_Internal_Flash(220,97,46,22,gImage_channel_off,2024);
        break;
    }
}

void channel_switch3( uint8_t channel_num )
{
    switch (channel_num)
    {
        case 0 :
        LCD_Show_Image_Internal_Flash(85,138,46,22,gImage_channel_on,2024);
        LCD_Show_Image_Internal_Flash(145,138,46,22,gImage_channel_off,2024);
        LCD_Show_Image_Internal_Flash(220,138,46,22,gImage_channel_off,2024);
        break;
        
        case 1 :
        LCD_Show_Image_Internal_Flash(85,138,46,22,gImage_channel_off,2024);
        LCD_Show_Image_Internal_Flash(145,138,46,22,gImage_channel_on,2024);
        LCD_Show_Image_Internal_Flash(220,138,46,22,gImage_channel_off,2024);
        break;

        case 2 :
        LCD_Show_Image_Internal_Flash(85,138,46,22,gImage_channel_off,2024);
        LCD_Show_Image_Internal_Flash(145,138,46,22,gImage_channel_off,2024);
        LCD_Show_Image_Internal_Flash(220,138,46,22,gImage_channel_on,2024);
        break;

        case 3 :
        LCD_Show_Image_Internal_Flash(85,138,46,22,gImage_channel_on,2024);
        LCD_Show_Image_Internal_Flash(145,138,46,22,gImage_channel_on,2024);
        LCD_Show_Image_Internal_Flash(220,138,46,22,gImage_channel_off,2024);
        break;

        case 4 :
        LCD_Show_Image_Internal_Flash(85,138,46,22,gImage_channel_on,2024);
        LCD_Show_Image_Internal_Flash(145,138,46,22,gImage_channel_on,2024);
        LCD_Show_Image_Internal_Flash(220,138,46,22,gImage_channel_on,2024);
        break;

        case 5 :
        LCD_Show_Image_Internal_Flash(85,138,46,22,gImage_channel_off,2024);
        LCD_Show_Image_Internal_Flash(145,138,46,22,gImage_channel_off,2024);
        LCD_Show_Image_Internal_Flash(220,138,46,22,gImage_channel_off,2024);
        break;
    }
}

void sync_switch( void )
{
    if( key.sync_allow_flag == 1)
    {
        LCD_Show_Image_Internal_Flash(55,3,30,30,gImage_sync_on,1800);
        if(gui_info.sync_24v_statu == 0)
        {
            mode1_off();
            mode2_off();
            mode3_off();
        }
    }else
    {
        LCD_Fill(55,3,85,33,WHITE);
        write_slave_reg();
    }
}

void refresh_icon( void )
{
    power_dis();
    wind_dis();
    mode_dis();
}

void slave_statu_update( void )
{
    if( gui_beat.beat_switch == 0 )
    {
        if(( modbus.modbus_04_scan_flag == 1 ) && ( modbus.modbus_04_scan_allow == 1 ))
        {
            get_slave_current_statu_multifunpower();
            LCD_ShowNum(270,57,modbus.NTC1_current_value,3,16,POINT_COLOR,BACK_COLOR);
            LCD_ShowNum(270,99,modbus.NTC2_current_value,3,16,POINT_COLOR,BACK_COLOR);
            LCD_ShowNum(270,141,modbus.NTC3_current_value,3,16,POINT_COLOR,BACK_COLOR);
            if( modbus.NTC1_current_value == 0 )
            {
                LCD_Show_Image_Internal_Flash(97,3,29,29,gImage_alarm_M,1682);
            }else if( modbus.NTC2_current_value == 0 )
            {
                LCD_Show_Image_Internal_Flash(97,3,29,29,gImage_alarm_M,1682);
            }else if( modbus.NTC2_current_value == 0 )
            {
                LCD_Show_Image_Internal_Flash(97,3,29,29,gImage_alarm_M,1682);
            }else
            {
                LCD_Show_Image_Internal_Flash(97,3,29,29,gImage_alarm_L,1682);
            }
            modbus.modbus_04_scan_flag = 0;
        }
    }else
    {
        modbus.modbus_04_scan_allow = 0;
    }

}
void gui_icon_init( void )
{
    /*              connect icon                */
    LCD_Show_Image_Internal_Flash(13,3,29,30,gImage_connect_on,1740);
    // LCD_Show_Image_Internal_Flash(15,3,29,30,gImage_connect_off,1740);

    /*              sync icon                */
    LCD_Show_Image_Internal_Flash(55,3,30,30,gImage_sync_off,1800);
    // LCD_Show_Image_Internal_Flash(45,3,30,30,gImage_sync_off),1800);

    /*              temp alarm                */
    //LCD_Show_Image_Internal_Flash(97,3,29,29,gImage_alarm_H,1682);
    // LCD_Show_Image_Internal_Flash(75,3,29,29,gImage_alarm_M,1682);
    // LCD_Show_Image_Internal_Flash(75,3,29,29,gImage_alarm_L,1682);

    /*              temp && humidity                 */
    LCD_ShowNum(233,13,25,2,16,DARKBLUE,BACK_COLOR);
    //LCD_Show_Image_Internal_Flash(250,11,21,21,gImage_sheshidu_big,882);
    PutChinese_12(17,255,"度",BACK_COLOR,POINT_COLOR);
    LCD_ShowNum(274,13,65,2,16,DARKBLUE,BACK_COLOR);
    LCD_ShowString(295,16,12,12,12,"RH",POINT_COLOR,BACK_COLOR);
    

    
    /*              channel  icon                */
    // LCD_Show_Image_Internal_Flash(85,46,46,22,gImage_channel_on,2024);
    // LCD_Show_Image_Internal_Flash(70,76,46,22,gImage_channel_on,2024);
    // LCD_Show_Image_Internal_Flash(70,106,46,22,gImage_channel_on,2024);

    // LCD_Show_Image_Internal_Flash(145,46,46,22,gImage_channel_off,2024);
    // LCD_Show_Image_Internal_Flash(145,76,46,22,gImage_channel_off,2024);
    // LCD_Show_Image_Internal_Flash(145,106,46,22,gImage_channel_off,2024);

    // LCD_Show_Image_Internal_Flash(220,46,46,22,gImage_channel_on,2024);
    // LCD_Show_Image_Internal_Flash(220,76,46,22,gImage_channel_on,2024);
    // LCD_Show_Image_Internal_Flash(220,106,46,22,gImage_channel_on,2024);
    channel_switch(key.mode_select);
    LCD_ShowNum(270,57,modbus.NTC1_current_value,3,16,POINT_COLOR,BACK_COLOR);
    PutChinese_12(60,300,"度",BACK_COLOR,POINT_COLOR);
    LCD_ShowNum(270,99,modbus.NTC2_current_value,3,16,POINT_COLOR,BACK_COLOR);
    PutChinese_12(103,300,"度",BACK_COLOR,POINT_COLOR);
    LCD_ShowNum(270,141,modbus.NTC3_current_value,3,16,POINT_COLOR,BACK_COLOR);
    PutChinese_12(144,300,"度",BACK_COLOR,POINT_COLOR);

   /*              mode  icon                */
//    LCD_Show_Image_Internal_Flash(3,55,33,22,gImage_dry1_on,1452);
//    LCD_Show_Image_Internal_Flash(3,125,32,22,gImage_dry2_off,1408);
//    LCD_Show_Image_Internal_Flash(3,195,32,22,gImage_dry3_on,1408);
    mode_switch(key.mode_select);

    // /*              temp  num                */
    // LCD_Show_Image_Internal_Flash(100,167,21,21,gImage_sheshidu_big,882);
    // LCD_ShowNum(55,165,gui_info.ntc_temp,3,24,POINT_COLOR,BACK_COLOR);
 
    /*              power  icon&& num %            */
    LCD_Show_Image_Internal_Flash(25,193,32,32,gImage_power,2048);
    LCD_ShowNum(63,195,gui_info.power_percentage,3,24,POINT_COLOR,BACK_COLOR);
    LCD_ShowChar(103,195,'%',24,POINT_COLOR,BACK_COLOR);
    //LCD_ShowNum(170,165,gui_info.power_percentage,3,24,POINT_COLOR,BACK_COLOR);
    //LCD_Show_Image_Internal_Flash(100,172,13,12,gImage_baifenbi,312);
    
    /*              fan  icon && num %           */
    LCD_Show_Image_Internal_Flash(138,193,32,32,gImage_wind,2048);
    LCD_ShowNum(179,195,gui_info.fan_level,1,24,POINT_COLOR,BACK_COLOR);
    //LCD_ShowNum(200,165,gui_info.fan_level,1,24,POINT_COLOR,BACK_COLOR);

    /*              mode num                */
    LCD_Show_Image_Internal_Flash(236,193,42,31,gImage_mode,2604);
    LCD_ShowNum(288,195,gui_info.mode_num,1,24,POINT_COLOR,BACK_COLOR);
    //LCD_ShowNum(260,210,gui_info.mode,1,24,POINT_COLOR,BACK_COLOR);

    /*              draw line                */
    LCD_DrawLine(1,36,320,36,GRAY);
    LCD_DrawLine(64,45,64,160,BROWN);
    LCD_DrawLine(20,179,300,179,GRAY);
   // LCD_DrawLine(50,40,50,230,BROWN);

}

