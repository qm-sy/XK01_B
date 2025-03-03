#include "modbus_rtu.h"

MODBUS_INFO modbus;

/**
 * @brief	发送1帧数据
 * 
 * @param   buf：待发送数组首地址           
 * @param   len：数组长度           
 * 
  @return  void
 */
void modbus_send_data( uint8_t *buf , uint8_t len )
{
    HAL_UART_Transmit(&huart2,(uint8_t*)buf,len,1000);
    
    while (__HAL_UART_GET_FLAG(&huart2,UART_FLAG_TC) != SET);
}

uint8_t modbus_wait_receive( void )
{
    uint16_t wait_time_cnt = 300;

    while((rs485.reflag!=1)&&(wait_time_cnt!=0))
    {
        wait_time_cnt--;
        delay_ms(1);
    }

    if( wait_time_cnt == 0 )
    {
        printf("receive error \r\n");
        return 0;
    }

    return 1;
}
/**
 * @brief	modbus接收函数，接收并判断Function后转到相应Function函数进行处理
 * 
 * @param   buf：待发送数组首地址           
 * @param   len：数组长度           
 * 
  @return  void
 */
void Modbus_Event( void )
{
    uint16_t crc,rccrc;

    /*1.接收完毕                                           */
    if( rs485.reflag == 1 )
    { 
        rs485.reflag = 0;
        /*2.CRC校验                                         */
        crc = MODBUS_CRC16(rs485.rcvbuf, rs485.recount-2);
        rccrc = (rs485.rcvbuf[rs485.recount-2]<<8) | (rs485.rcvbuf[rs485.recount-1]);

        if ( crc == rccrc)
        {
            if( rs485.rcvbuf[0] == SLAVE1_ADDR )
            {
                switch ( rs485.rcvbuf[1] )
                {         
                    case 0x03:		Modbus_Fun3();		        break;
                    case 0x04:		Modbus_Fun4_slave1();       break;            
                    case 0x06:		Modbus_Fun6();		        break;        
                    case 0x10:	    Modbus_Fun16();		        break;  

                    default:						    break;
                }
            }
            if( rs485.rcvbuf[0] == SLAVE2_ADDR )
            {
                Modbus_Fun4_slave2();
            }
            if( rs485.rcvbuf[0] == SLAVE3_ADDR )
            {
                Modbus_Fun4_slave3();
            }
            rs485.recount = 0;
        }
    }
}

void Modbus_Fun3()
{
    uint8_t start_addr_03 = 3;              //Slave reply  DATA1_H address

    for( uint16_t i = 0; i < 5; i++)
    {
        switch (i)
        {
        case 0:
            gui_info.fan_level = rs485.rcvbuf[start_addr_03 + 1];                  
            break;

        case 1:
            key.channel_select = rs485.rcvbuf[start_addr_03 + 1];  
            gui_info.power_percentage =   rs485.rcvbuf[start_addr_03];  
            break; 

        default:
            break;
        }
        start_addr_03 += 2;
    }
    /* RX允许继续接收，开启超时接收计时               */
    


}

void Modbus_Fun4_slave1()
{
    modbus.NTC1_current_value = rs485.rcvbuf[4];

    /* RX允许继续接收，开启超时接收计时               */

   
}

void Modbus_Fun4_slave2()
{
    modbus.NTC2_current_value = rs485.rcvbuf[4];

    /* RX允许继续接收，开启超时接收计时               */

    
}

void Modbus_Fun4_slave3()
{
    modbus.NTC3_current_value = rs485.rcvbuf[4];

    /* RX允许继续接收，开启超时接收计时               */

    
}
void Modbus_Fun6()
{

}

void Modbus_Fun16()
{
	
}

/**
 * @brief	crc校验函数
 * 
 * @param   buf：  Address(1 byte) +Funtion(1 byte) ）+Data(n byte)   
 * @param   length:数据长度           
 * 
  @return  crc16:crc校验的值 2byte
 */
//buf内的值为 
uint16_t MODBUS_CRC16(uint8_t *buf, uint8_t length)
{
	uint8_t	i;
	uint16_t	crc16;

    /* 1, 预置16位CRC寄存器为0xffff（即全为1）                          */
	crc16 = 0xffff;	

	do
	{
        /* 2, 把8位数据与16位CRC寄存器的低位相异或，把结果放于CRC寄存器     */        
		crc16 ^= (uint16_t)*buf;		//
		for(i=0; i<8; i++)		
		{
            /* 3, 如果最低位为1，把CRC寄存器的内容右移一位(朝低位)，用0填补最高位 再异或0xA001    */
			if(crc16 & 1)
            {
                crc16 = (crc16 >> 1) ^ 0xA001;
            }
            /* 4, 如果最低位为0，把CRC寄存器的内容右移一位(朝低位)，用0填补最高位                */
            else
            {
                crc16 >>= 1;
            }		
		}
		buf++;
	}while(--length != 0);

	return	(crc16);
}


// void get_slave_init_statu_multifunpower( void )
// {
//     get_slave1_init_statu_multifunpower();
//     get_slave2_init_statu_multifunpower();
//     get_slave3_init_statu_multifunpower();
// }

void get_slave_init_statu_multifunpower( void )
{
    uint8_t send_buf[8] = {0x17,0x03,0x00,0x00,0x00,0x02,0xFD,0xC6};

    RS485_TX;
    delay_ms(5);

    memcpy(modbus.modbus_send_buf,send_buf,8);

    modbus_send_data(modbus.modbus_send_buf,8); 

    RS485_RX;
    delay_ms(5);
}

// void get_slave2_init_statu_multifunpower( void )
// {
//     uint8_t send_buf[8] = {0x18,0x03,0x00,0x00,0x00,0x02,0xFD,0xC6};

//     RS485_TX;
//     delay_ms(5);

//     memcpy(modbus.modbus_send_buf,send_buf,8);

//     modbus_send_data(modbus.modbus_send_buf,8); 

//     RS485_RX;
//     delay_ms(5);
// }

// void get_slave3_init_statu_multifunpower( void )
// {
//     uint8_t send_buf[8] = {0x19,0x03,0x00,0x00,0x00,0x02,0xFD,0xC6};

//     RS485_TX;
//     delay_ms(5);

//     memcpy(modbus.modbus_send_buf,send_buf,8);

//     modbus_send_data(modbus.modbus_send_buf,8); 

//     RS485_RX;
//     delay_ms(5);

// }

void get_slave_current_statu_multifunpower( void )
{
    if(modbus.slave1_flag == 1)
    {
        get_slave1_current_statu_multifunpower();
    }
    if(modbus.slave2_flag == 1)
    {
        get_slave2_current_statu_multifunpower();
    }
    if(modbus.slave3_flag == 1)
    {
        get_slave3_current_statu_multifunpower();
    }
}

void get_slave1_current_statu_multifunpower( void )
{
    uint8_t send_buf[8] = {0x17,0x04,0x00,0x00,0x00,0x03,0xFD,0XB2};

    RS485_TX;
    delay_ms(5);
    
    memcpy(modbus.modbus_send_buf,send_buf,8);

    modbus_send_data(modbus.modbus_send_buf,8); 
    RS485_RX;
    delay_ms(5);
}
void get_slave2_current_statu_multifunpower( void )
{
    uint8_t send_buf[8] = {0x18,0x04,0x00,0x00,0x00,0x03,0x02,0XB2};

    RS485_TX;
    delay_ms(5);
    
    memcpy(modbus.modbus_send_buf,send_buf,8);

    modbus_send_data(modbus.modbus_send_buf,8); 
    RS485_RX;
    delay_ms(5);
}
void get_slave3_current_statu_multifunpower( void )
{
    uint8_t send_buf[8] = {0x19,0x04,0x00,0x00,0x00,0x03,0xD3,0XB3};

    RS485_TX;
    delay_ms(5);
    
    memcpy(modbus.modbus_send_buf,send_buf,8);

    modbus_send_data(modbus.modbus_send_buf,8); 
    RS485_RX;
    delay_ms(5);
}

void write_slave_reg( void )
{
    delay_ms(5);
    modbus.slave_send_flag = 0;
    switch (key.mode_select)
    {
        case 0:
            write_slave1_reg();
            mode2_off();
            mode3_off();
           
            break;
        
        case 1:
            write_slave2_reg();
            mode1_off();
            mode3_off();
           
            break;

        case 2:
            write_slave3_reg();
            mode2_off();
            mode1_off();
            
            break;

        case 3:
            write_slave1_reg();
            write_slave2_reg();
            mode3_off();
            
            break;

        case 4:
            write_slave1_reg();
            write_slave2_reg();
            write_slave3_reg();
           
            break;

        default:
            
            break;
    }
    modbus.modbus_04_scan_allow = 1;
    modbus.slave_send_flag = 1;
}

void write_slave1_reg( void )
{
    uint8_t send_buf[19];
    uint16_t crc;
    if((key.sync_allow_flag == 0)||(gui_info.sync_24v_statu == 1))
    {
        modbus.modbus_04_scan_allow = 0;

        RS485_TX;
        delay_ms(5);
    
        send_buf[0] = 0X17;
        send_buf[1] = 0x10;
        send_buf[2] = 0x00;
        send_buf[3] = 0x00;
        send_buf[4] = 0x00;
        send_buf[5] = 0x02;
        send_buf[6] = 0x04;
    
        send_buf[7] = 0x00;
        send_buf[8] = gui_info.fan_level;
    
        send_buf[9]  = gui_info.power_percentage;
        send_buf[10] = key.channel_select;
    
        crc = MODBUS_CRC16(send_buf,11);
    
        send_buf[11] = crc>>8;
        send_buf[12] = crc;
    
        memcpy(modbus.modbus_send_buf,send_buf,13);
    
        modbus_send_data(modbus.modbus_send_buf,13);
        
        delay_ms(100);
    }
    
}

void mode1_off( void )
{
    uint8_t send_buf[19];
    uint16_t crc;

    modbus.modbus_04_scan_allow = 0;

    RS485_TX;
    delay_ms(5);

    send_buf[0] = 0X17;
    send_buf[1] = 0x10;
    send_buf[2] = 0x00;
    send_buf[3] = 0x00;
    send_buf[4] = 0x00;
    send_buf[5] = 0x02;
    send_buf[6] = 0x04;

    send_buf[7] = 0x00;
    send_buf[8] = 0x00;

    send_buf[9]  = gui_info.power_percentage;
    send_buf[10] = 0x05;

    crc = MODBUS_CRC16(send_buf,11);

    send_buf[11] = crc>>8;
    send_buf[12] = crc;

    memcpy(modbus.modbus_send_buf,send_buf,13);

    modbus_send_data(modbus.modbus_send_buf,13);
    
    delay_ms(100);
}

void write_slave2_reg( void )
{
    uint8_t send_buf[19];
    uint16_t crc;
    if((key.sync_allow_flag == 0)||(gui_info.sync_24v_statu == 1))
    {
        modbus.modbus_04_scan_allow = 0;

        RS485_TX;
        delay_ms(5);

        send_buf[0] = 0X18;
        send_buf[1] = 0x10;
        send_buf[2] = 0x00;
        send_buf[3] = 0x00;
        send_buf[4] = 0x00;
        send_buf[5] = 0x02;
        send_buf[6] = 0x04;

        send_buf[7] = 0x00;
        send_buf[8] = gui_info.fan_level;

        send_buf[9]  = gui_info.power_percentage;
        send_buf[10] = key.channel_select;

        crc = MODBUS_CRC16(send_buf,11);

        send_buf[11] = crc>>8;
        send_buf[12] = crc;

        memcpy(modbus.modbus_send_buf,send_buf,13);

        modbus_send_data(modbus.modbus_send_buf,13);

        delay_ms(100);
    }
    
}

void mode2_off( void )
{
    uint8_t send_buf[19];
    uint16_t crc;

    modbus.modbus_04_scan_allow = 0;

    RS485_TX;
    delay_ms(5);

    send_buf[0] = 0X18;
    send_buf[1] = 0x10;
    send_buf[2] = 0x00;
    send_buf[3] = 0x00;
    send_buf[4] = 0x00;
    send_buf[5] = 0x02;
    send_buf[6] = 0x04;

    send_buf[7] = 0x00;
    send_buf[8] = 0x00;

    send_buf[9]  = gui_info.power_percentage;
    send_buf[10] = 0x05;

    crc = MODBUS_CRC16(send_buf,11);

    send_buf[11] = crc>>8;
    send_buf[12] = crc;

    memcpy(modbus.modbus_send_buf,send_buf,13);

    modbus_send_data(modbus.modbus_send_buf,13);
    

    delay_ms(100);
}

void write_slave3_reg( void )
{
    uint8_t send_buf[19];
    uint16_t crc;
    if((key.sync_allow_flag == 0)||(gui_info.sync_24v_statu == 1))
    {
        modbus.modbus_04_scan_allow = 0;

        RS485_TX;
        delay_ms(5);

        send_buf[0] = 0X19;
        send_buf[1] = 0x10;
        send_buf[2] = 0x00;
        send_buf[3] = 0x00;
        send_buf[4] = 0x00;
        send_buf[5] = 0x02;
        send_buf[6] = 0x04;

        send_buf[7] = 0x00;
        send_buf[8] = gui_info.fan_level;

        send_buf[9]  = gui_info.power_percentage;
        send_buf[10] = key.channel_select;

        crc = MODBUS_CRC16(send_buf,11);

        send_buf[11] = crc>>8;
        send_buf[12] = crc;

        memcpy(modbus.modbus_send_buf,send_buf,13);

        modbus_send_data(modbus.modbus_send_buf,13);
        
        delay_ms(100);
    }
    
}

void mode3_off( void )
{
    uint8_t send_buf[19];
    uint16_t crc;

    modbus.modbus_04_scan_allow = 0;

    RS485_TX;
    delay_ms(5);

    send_buf[0] = 0X19;
     send_buf[1] = 0x10;
    send_buf[2] = 0x00;
    send_buf[3] = 0x00;
    send_buf[4] = 0x00;
    send_buf[5] = 0x02;
    send_buf[6] = 0x04;

    send_buf[7] = 0x00;
    send_buf[8] = 0x00;

    send_buf[9]  = gui_info.power_percentage;
    send_buf[10] = 0x05;

    crc = MODBUS_CRC16(send_buf,11);

    send_buf[11] = crc>>8;
    send_buf[12] = crc;

    memcpy(modbus.modbus_send_buf,send_buf,13);

    modbus_send_data(modbus.modbus_send_buf,13);
    
    delay_ms(100);
}
