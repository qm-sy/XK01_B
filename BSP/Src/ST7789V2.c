#include "ST7789V2.h"
#include "font.h"
#include "spi.h"
#include "gpio.h"
#include "Hzlib.h" 

#include "stm32f1xx_hal.h"
#include "SPI_Flash_w25q64.h"
#include "string.h"

#define LCD_TOTAL_BUF_SIZE	(240*320*2)
#define LCD_Buf_Size 1536

static uint8_t lcd_buf[LCD_Buf_Size];
volatile uint8_t transmit_complete_flag1 = 0;

uint16_t	POINT_COLOR = BLACK;	//画笔颜色	默认为黑色
uint16_t	BACK_COLOR 	= WHITE;	//背景颜色	默认为白色


void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if( hspi->Instance == SPI1 )
	{
		transmit_complete_flag1 = 1;
	}
}

/******************************************************************************
function:wait transmit
******************************************************************************/
void wait_spi1_dma_transmit(void)
{
	while(transmit_complete_flag1 == 0)
    {

    }
	transmit_complete_flag1 = 0;
}

/**
 * @brief	LCD底层SPI发送数据函数
 *
 * @param   data	数据的起始地址
 * @param   size	发送数据大小
 *
 * @return  void
 */

static void LCD_SPI_Send(uint8_t *data, uint16_t size)
{
	SPI1_WriteByte(data, size);
}

/**
 * @brief	LCD底层SPI发送数据函数--DMA
 *
 * @param   data	数据的起始地址
 * @param   size	发送数据大小
 *
 * @return  void
 */

static void LCD_SPI_Send_DMA(const uint8_t *data, uint16_t size)
{
	SPI1_WriteByte_DMA(data, size);
}

//static void LCD_SPI2_Send(uint8_t *data, uint16_t size)
//{
//	SPI2_WriteByte(data, size);
//}


/**
 * @brief	写命令到LCD
 *
 * @param   cmd		需要发送的命令
 *
 * @return  void
 */
static void LCD_Write_Cmd(uint8_t cmd)
{
    LCD_DC(0);

    LCD_SPI_Send(&cmd, 1);
}

/**
 * @brief	写数据到LCD
 *
 * @param   cmd		需要发送的数据
 *
 * @return  void
 */
static void LCD_Write_Data(uint8_t data)
{
    LCD_DC(1);

    LCD_SPI_Send(&data, 1);
}

/**
 * @brief	写半个字的数据到LCD
 *
 * @param   cmd		需要发送的数据
 *
 * @return  void
 */
void LCD_Write_Data_16bit(const uint16_t data)
{
    uint8_t data_buf[2] = {0};

    data_buf[0] = data >> 8;
    data_buf[1] = data;

    LCD_DC(1);
    LCD_SPI_Send(data_buf, 2);
}


/**
 * @brief 设置数据写入LCD缓存区域
 * 
 * @param   x1	起点坐标x
 * @param   y1	起点坐标y
 * @param   x2	终点坐标x
 * @param   y2	终点坐标y
 *
 * @return  void
 */
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    LCD_Write_Cmd(0x2a);        //设置列
    LCD_Write_Data_16bit(x1);
    LCD_Write_Data_16bit(x2);

    LCD_Write_Cmd(0x2b);        //设置行
    LCD_Write_Data_16bit(y1);
    LCD_Write_Data_16bit(y2);

    LCD_Write_Cmd(0x2C);        //使能行列设置
}

/**
 * 打开LCD显示
 *
 * @param   void
 *
 * @return  void
 */
void LCD_DisplayOn(void)
{

}
/**
 * 关闭LCD显示
 *
 * @param   void
 *
 * @return  void
 */
void LCD_DisplayOff(void)
{

}

/**
 * 以一种颜色清空LCD屏
 *
 * @param   color	清屏颜色
 *
 * @return  void
 */
void LCD_Clear(uint16_t color)
{
    uint16_t i, j;
    uint8_t data[2] = {0};

    data[0] = color >> 8;
    data[1] = color;

    LCD_Address_Set(0, 0, LCD_Width - 1, LCD_Height - 1);

    for(j = 0; j < LCD_Buf_Size / 2; j++)
    { 
        lcd_buf[j * 2] =  data[0];
        lcd_buf[j * 2 + 1] =  data[1];
    }

    LCD_DC(1);

    for(i = 0; i < LCD_TOTAL_BUF_SIZE/LCD_Buf_Size; i++)
    {
        LCD_SPI_Send(lcd_buf, LCD_Buf_Size);
    }
    
    // uint16_t i, j;
 
    // LCD_Address_Set(0, 0, LCD_Width - 1, LCD_Height - 1);

	// for( i = 0; i < LCD_Height; i++ )
	// {
    //     for( j = 0; j < LCD_Width; j++ )
    //     {	
	// 		LCD_Write_Data_16bit(color);
	// 	}
	// }
	
}

/**
 * 用一个颜色填充整个区域
 *
 * @param   x_start,y_start     起点坐标
 * @param   x_end,y_end			终点坐标
 * @param   color       		填充颜色
 *
 * @return  void
 */
void LCD_Fill(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t color)
{
    uint16_t i = 0;
    uint32_t size = 0, size_remain = 0;

    size = (x_end - x_start + 1) * (y_end - y_start + 1) * 2;

    if(size > LCD_Buf_Size)
    {
        size_remain = size - LCD_Buf_Size;
        size = LCD_Buf_Size;
    }

    LCD_Address_Set(x_start, y_start, x_end, y_end);

    while(1)
    {
        for(i = 0; i < size / 2; i++)
        {
            lcd_buf[2 * i] = color >> 8;
            lcd_buf[2 * i + 1] = color;
        }

        LCD_DC(1);
        LCD_SPI_Send(lcd_buf, size);

        if(size_remain == 0)
            break;

        if(size_remain > LCD_Buf_Size)
        {
            size_remain = size_remain - LCD_Buf_Size;
        }

        else
        {
            size = size_remain;
            size_remain = 0;
        }
    }
}

/**
 * 画点函数
 *
 * @param   x,y		画点坐标
 *
 * @return  void
 */
void LCD_Draw_Point(uint16_t x, uint16_t y)
{
    LCD_Address_Set(x, y, x, y);
    LCD_Write_Data_16bit(POINT_COLOR);
}

/**
 * 画点带颜色函数
 *
 * @param   x,y		画点坐标
 *
 * @return  void
 */
void LCD_Draw_ColorPoint(uint16_t x, uint16_t y,uint16_t color)
{
    LCD_Address_Set(x, y, x, y);
    LCD_Write_Data_16bit(color);
}



/**
 * @brief	画线函数(直线、斜线)
 *
 * @param   x1,y1	起点坐标
 * @param   x2,y2	终点坐标
 * @param   color	颜色
 *
 * @return  void
 */
  void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color)
{
    uint16_t t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, row, col;
    uint32_t i = 0;

    if(y1 == y2)
    {
        /*快速画水平线*/
        LCD_Address_Set(x1, y1, x2, y2);

        for(i = 0; i < x2 - x1; i++)
        {
            lcd_buf[2 * i] = color >> 8;
            lcd_buf[2 * i + 1] = color;
        }

        LCD_DC(1);
        LCD_SPI_Send(lcd_buf, (x2 - x1) * 2);
        return;
    }

    delta_x = x2 - x1;
    delta_y = y2 - y1;
    row = x1;
    col = y1;

    if(delta_x > 0)
    {
        incx = 1;
    }
    else if(delta_x == 0)
    {
        incx = 0;
    }else
    {
        incx = -1;
        delta_x = -delta_x;
    }

    if(delta_y > 0)
    {
        incy = 1;
    }
    else if(delta_y == 0)
    {
        incy = 0;
    }else
    {
        incy = -1;
        delta_y = -delta_y;
    }

    if(delta_x > delta_y)
    {
        distance = delta_x;
    }else
    {
        distance = delta_y;
    }

    for(t = 0; t <= distance + 1; t++)
    {
        LCD_Draw_ColorPoint(row, col,color);
        xerr += delta_x ;
        yerr += delta_y ;

        if(xerr > distance)
        {
            xerr -= distance;
            row += incx;
        }

        if(yerr > distance)
        {
            yerr -= distance;
            col += incy;
        }
    }
}

/**
 * @brief	画一个矩形
 *
 * @param   x1,y1	起点坐标
 * @param   x2,y2	终点坐标
 * @param   color	颜色
 *
 * @return  void
 */
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color)
{
    LCD_DrawLine(x1, y1, x2, y1,color);
    LCD_DrawLine(x1, y1, x1, y2,color);
    LCD_DrawLine(x1, y2, x2, y2,color);
    LCD_DrawLine(x2, y1, x2, y2,color);
}

/**
 * @brief	画一个圆
 *
 * @param   x0,y0	圆心坐标
 * @param   r       圆半径
 *
 * @return  void
 */
void LCD_Draw_Circle(uint16_t x0, uint16_t y0, uint8_t r,uint16_t color)
{
    int a, b;
    int di;
    a = 0;
    b = r;
    di = 3 - (r << 1);

    while(a <= b)
    {
        LCD_Draw_ColorPoint(x0 - b, y0 - a,color);
        LCD_Draw_ColorPoint(x0 + b, y0 - a,color);
        LCD_Draw_ColorPoint(x0 - a, y0 + b,color);
        LCD_Draw_ColorPoint(x0 - b, y0 - a,color);
        LCD_Draw_ColorPoint(x0 - a, y0 - b,color);
        LCD_Draw_ColorPoint(x0 + b, y0 + a,color);
        LCD_Draw_ColorPoint(x0 + a, y0 - b,color);
        LCD_Draw_ColorPoint(x0 + a, y0 + b,color);
        LCD_Draw_ColorPoint(x0 - b, y0 + a,color);
        a++;

        //Bresenham
        if(di < 0)di += 4 * a + 6;

        else
        { 
            di += 10 + 4 * (a - b);
            b--;
        }

        LCD_Draw_ColorPoint(x0 + a, y0 + b,color);
    }
}

/**
 * @brief	显示一个ASCII码字符
 *
 * @param   x,y		显示起始坐标
 * @param   chr		需要显示的字符
 * @param   size	字体大小(支持16/24/32号字体)
 *
 * @return  void
 */
void LCD_ShowChar(uint16_t x, uint16_t y, char chr, uint8_t size,uint16_t back_color,uint16_t char_color)
{
    uint8_t temp, t1, t;
    uint8_t csize;		//得到字体一个字符对应点阵集所占的字节数
    uint16_t colortemp;
    uint8_t sta;

    chr = chr - ' '; //得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）

    if((x > (LCD_Width - size / 2)) || (y > (LCD_Height - size)))	return;     //超出边界

    LCD_Address_Set(x, y, x + size / 2 - 1, y + size - 1);//(x,y,x+8-1,y+16-1)   16号字体：8-16像素  16号字体：16-32像素 

    if((size == 16) || (size == 32) )	//16和32号字体

    {
        csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);

        for(t = 0; t < csize; t++)
        {
            if(size == 16)
            {
                temp = asc2_1608[chr][t];	//调用1608字体
            }

            else if(size == 32)
            {
                temp = asc2_3216[chr][t];	//调用3216字体
            }

            else return;			//没有的字库

            for(t1 = 0; t1 < 8; t1++)
            {
                if(temp & 0x80)
                {
                    colortemp = back_color;
                }else
                {
                    colortemp = char_color;
                }
                
                LCD_Write_Data_16bit(colortemp);
                temp <<= 1;
            }
        }
    }

	else if  (size == 12)	//12号字体
	{
        csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);

        for(t = 0; t < csize; t++)
        {
            temp = asc2_1206[chr][t];

            for(t1 = 0; t1 < 6; t1++)
            {
                if(temp & 0x80)
                {
                    colortemp = back_color;
                }else
                {
                    colortemp = char_color;
                }

                LCD_Write_Data_16bit(colortemp);
                temp <<= 1;
            }
        }
    }
	
    else if(size == 24)		//24号字体
    {
        csize = (size * 16) / 8;

        for(t = 0; t < csize; t++)
        {
            temp = asc2_2412[chr][t];

            if(t % 2 == 0)sta = 8;

            else sta = 4;

            for(t1 = 0; t1 < sta; t1++)
            {
                if(temp & 0x80)
                {
                    colortemp = back_color;
                }else
                {
                    colortemp = char_color;
                }

                LCD_Write_Data_16bit(colortemp);
                temp <<= 1;
            }
        }
    }
}

/**
 * @brief	m^n函数
 *
 * @param   m,n		输入参数
 *
 * @return  m^n次方
 */
static uint32_t LCD_Pow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;

    while(n--)
    { 
        result = result * m;
    }

    return result;
}

/**
 * @brief	显示数字,高位为0不显示
 *
 * @param   x,y		起点坐标
 * @param   num		需要显示的数字,数字范围(0~4294967295)
 * @param   len		需要显示的位数
 * @param   size	字体大小
 *
 * @return  void
 */
void LCD_ShowNum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size,uint16_t back_color,uint16_t char_color)
{
    uint8_t t, temp;
    uint8_t enshow = 0;

    for(t = 0; t < len; t++)
    {
        temp = (num / LCD_Pow(10, len - t - 1)) % 10;

        if( enshow == 0 && t < (len - 1) )
        {
            if( temp == 0 )
            {
                LCD_ShowChar(x + (size / 2)*t, y, ' ', size,back_color,char_color);
                continue;
            }else
            {
                enshow = 1;
            } 
        }

        LCD_ShowChar(x + (size / 2)*t, y, temp + '0', size,back_color,char_color);
    }
}



/**
 * @brief	显示数字,高位为0,可以控制显示为0还是不显示
 *
 * @param   x,y		起点坐标
 * @param   num		需要显示的数字,数字范围(0~999999999)
 * @param   len		需要显示的位数
 * @param   size	字体大小
 * @param   mode	1:高位显示0		0:高位不显示
 *
 * @return  void
 */
void LCD_ShowxNum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size, uint8_t mode,uint16_t back_color,uint16_t char_color)
{
    uint8_t t, temp;
    uint8_t enshow = 0;


    for(t = 0; t < len; t++)
    {
        temp = (num / LCD_Pow(10, len - t - 1)) % 10;

        if(enshow == 0 && t < (len - 1))
        {
            if(temp == 0)
            {
                if(mode)
                {
                    LCD_ShowChar(x + (size / 2)*t, y, '0', size,char_color,char_color);
                }else
                {
                    LCD_ShowChar(x + (size / 2)*t, y, ' ', size,char_color,char_color);
                }
                continue;
            }else
            {
                enshow = 1;
            }
        }

        LCD_ShowChar(x + (size / 2)*t, y, temp + '0', size,char_color,char_color);
    }
}


/**
 * @brief	显示字符串
 *
 * @param   x,y		    起点坐标
 * @param   width	    字符显示区域宽度,1char/1byte=8
 * @param   height	    字符显示区域高度，根据char’s size
 * @param   size	    字体大小，根据char’s size
 * @param   p		    字符串起始地址
 * @param   back_color  字体背景颜色
 * @param   char_color  字体颜色
 * 
 * @return  void
 */
void LCD_ShowString(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t size, char *p,uint16_t back_color,uint16_t char_color)
{
    uint8_t x0 = x;
    width += x;
    height += y;

    while((*p <= '~') && (*p >= ' ')) //判断是不是非法字符!
    {
        if(x >= width)
        {
            x = x0;
            y += size;
        }

        if(y >= height)
        {
            break; //退出
        }

        LCD_ShowChar(x, y, *p, size,back_color,char_color);
        x += size / 2;
        p++;
    }
}


/**
 * @brief	显示图片,从单片机内部flash读取
 *
 * @remark	Image2Lcd取模方式：	C语言数据/水平扫描/16位真彩色(RGB565)/高位在前		其他的不要选
 *
 * @param   x,y		    起点坐标
 * @param   width	    图片宽度
 * @param   height	    图片高度
 * @param   pic		    图片缓存数据起始地址,数组名
 * @param   pic_size    图片数组大小
 *
 * @return  void
 */
void LCD_Show_Image_Internal_Flash(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *pic, uint16_t pic_size)
{
    
    //uint8_t buf[240];v 
    //HAL_StatusTypeDef status ;
    if(x + width > LCD_Width || y + height > LCD_Height)
    {
        return;
    }

    LCD_Address_Set(x, y, x + width - 1, y + height - 1);

    LCD_DC(1);

    // for (uint32_t i = 0; i < 967680; i+=240)
    // {
    //     W25Q64_Read(i, buf, 240);
    //     LCD_SPI_Send_DMA(buf, 240);
    // }
    LCD_SPI_Send_DMA(pic, pic_size);  
    wait_spi1_dma_transmit();
}

/**
 * @brief	显示图片 从外部flash W25Q64中读取
 *
 * @remark	Image2Lcd取模方式：	C语言数据/水平扫描/16位真彩色(RGB565)/高位在前		其他的不要选
 *
 * @param   x,y		    起点坐标
 * @param   width	    图片宽度
 * @param   height	    图片高度
 * @param   start_addr	起始地址
 * @param   pic_size    图片数组大小
 * 
 * @return  void
 */
void LCD_Show_Image_External_Flash(uint16_t x, uint16_t y, uint16_t width, uint16_t height,const uint8_t start_addr, uint16_t pic_size)
{
    
    uint8_t buf[240];

    if(x + width > LCD_Width || y + height > LCD_Height)
    {
        return;
    }

    LCD_Address_Set(x, y, x + width - 1, y + height - 1);

    LCD_DC(1);

    for (uint32_t i = start_addr; i < start_addr + pic_size; i+=240)
    {
        W25Q64_Read(i, buf, 240);
        LCD_SPI_Send_DMA(buf, 240);
    }
 
    wait_spi1_dma_transmit();
}


/**
 * @brief	LCD初始化
 *
 * @param   void
 *
 * @return  void
 */
void LCD_Init(void)
{
    //LCD_Gpio_Init();	//硬件接口初始化

    LCD_RST(0);
    HAL_Delay(12);
    LCD_RST(1);
	
    HAL_Delay(12);
    /* Sleep Out */
    LCD_Write_Cmd(0x11);
    /* wait for power stability */
    HAL_Delay(12);

    /* Memory Data Access Control */
    LCD_Write_Cmd(0x36);
    LCD_Write_Data(0xa0);

    /* RGB 5-6-5-bit  */
    LCD_Write_Cmd(0x3A);
    LCD_Write_Data(0x65);

    /* Porch Setting */
    LCD_Write_Cmd(0xB2);
    LCD_Write_Data(0x0C);
    LCD_Write_Data(0x0C);
    LCD_Write_Data(0x00);
    LCD_Write_Data(0x33);
    LCD_Write_Data(0x33);


    /*  Gate Control */
    LCD_Write_Cmd(0xB7);
    LCD_Write_Data(0x72);

    /* VCOM Setting */
    LCD_Write_Cmd(0xBB);
    LCD_Write_Data(0x3D);   //Vcom=1.625V

    /* LCM Control */
    LCD_Write_Cmd(0xC0);
    LCD_Write_Data(0x2C);

    /* VDV and VRH Command Enable */
    LCD_Write_Cmd(0xC2);
    LCD_Write_Data(0x01);

    /* VRH Set */
    LCD_Write_Cmd(0xC3);
    LCD_Write_Data(0x19);

    /* VDV Set */
    LCD_Write_Cmd(0xC4);
    LCD_Write_Data(0x20);

    /* Frame Rate Control in Normal Mode */
    LCD_Write_Cmd(0xC6);
    LCD_Write_Data(0x0F);	//60MHZ

    /* Power Control 1 */
    LCD_Write_Cmd(0xD0);
    LCD_Write_Data(0xA4);
    LCD_Write_Data(0xA1);

    /* Positive Voltage Gamma Control */
    LCD_Write_Cmd(0xE0);
    LCD_Write_Data(0xD0);
    LCD_Write_Data(0x04);
    LCD_Write_Data(0x0D);
    LCD_Write_Data(0x11);
    LCD_Write_Data(0x13);
    LCD_Write_Data(0x2B);
    LCD_Write_Data(0x3F);
    LCD_Write_Data(0x54);
    LCD_Write_Data(0x4C);
    LCD_Write_Data(0x18);
    LCD_Write_Data(0x0D);
    LCD_Write_Data(0x0B);
    LCD_Write_Data(0x1F);
    LCD_Write_Data(0x23);

    /* Negative Voltage Gamma Control */
    LCD_Write_Cmd(0xE1);
    LCD_Write_Data(0xD0);
    LCD_Write_Data(0x04);
    LCD_Write_Data(0x0C);
    LCD_Write_Data(0x11);
    LCD_Write_Data(0x13);
    LCD_Write_Data(0x2C);
    LCD_Write_Data(0x3F);
    LCD_Write_Data(0x44);
    LCD_Write_Data(0x51);
    LCD_Write_Data(0x2F);
    LCD_Write_Data(0x1F);
    LCD_Write_Data(0x1F);
    LCD_Write_Data(0x20);
    LCD_Write_Data(0x23);

    /* Display Inversion On */
    LCD_Write_Cmd(0x21);

    LCD_Write_Cmd(0x29);

    LCD_Address_Set(0, 0, LCD_Width - 1, LCD_Height - 1);

    LCD_Clear(BACK_COLOR);

    /* Display on */

}

/**
 * @brief   ：显示单个汉字字符串 16*16
 *
 * @param   xpos: 0~(319-16) 
 * @param   Ypos: 0~(239-16)
 * @param   str: 中文字符串首址
 * @param   Color: 字符颜色  
 *
 * @return  void
 */
void PutChinese_16(uint16_t Xpos,uint16_t Ypos,uint8_t *str,uint16_t back_color,uint16_t char_color)
{
    uint8_t i,j;
	uint16_t k;
	uint16_t HZnum;
    uint8_t buffer[32];
    uint16_t tmp_char = 0;

    HZnum=sizeof(tfont16)/sizeof(typFNT_GB16);	//自动统计汉字数目

    for ( k = 0; k < HZnum; k++ ) 
	{
        if ( ( tfont16[k].Index[0] == *(str)) && (tfont16[k].Index[1] == *(str+1) ) )
        {

            memcpy(buffer, tfont16[k].Msk, sizeof(tfont16[k].Msk));

            for(i = 0; i < 16; i++ )
            {
                tmp_char=buffer[i*2];
                tmp_char=(tmp_char<<8);
                tmp_char|=buffer[2*i+1];

                for ( j = 0; j < 16; j++ )
                {
                    if ( (tmp_char >> 15-j) & 0x01 == 0x01)
                    {
                        LCD_Draw_ColorPoint(Ypos+j,Xpos+i,char_color);
                    }
                    else
                    {
                        //LCD_Draw_ColorPoint(Ypos+j,Xpos+i,back_color);//指定字体背景颜色
                    }
                }
            }
        }
    }
}

/**
 * @brief   ：显示单个汉字字符串 12*12
 *
 * @param   xpos: 0~(319-16) 
 * @param   Ypos: 0~(239-16)
 * @param   str: 中文字符串首址
 * @param   Color: 字符颜色  
 *
 * @return  void
 */
void PutChinese_12(uint16_t Xpos,uint16_t Ypos,uint8_t *str,uint16_t back_color,uint16_t char_color)
{
    uint8_t i,j;
	uint16_t k;
	uint16_t HZnum;
    uint8_t buffer[24];
    uint16_t tmp_char = 0;

    HZnum=sizeof(tfont12)/sizeof(typFNT_GB12);	//自动统计汉字数目

    for ( k = 0; k < HZnum; k++ ) 
	{
        if ( ( tfont12[k].Index[0] == *(str)) && (tfont12[k].Index[1] == *(str+1) ) )
        {

            memcpy(buffer, tfont12[k].Msk, sizeof(tfont12[k].Msk));

            for(i = 0; i < 12; i++ )
            {
                tmp_char=buffer[i*2];
                tmp_char=(tmp_char<<6);
                tmp_char|=buffer[2*i+1];

                for ( j = 0; j < 12; j++ )
                {
                    if ( (tmp_char >> 11-j) & 0x01 == 0x01)
                    {
                        LCD_Draw_ColorPoint(Ypos+j,Xpos+i,char_color);
                    }
                    else
                    {
                        //LCD_Draw_ColorPoint(Ypos+j,Xpos+i,back_color);//指定字体背景颜色
                    }
                }
            }
        }
    }
}


/**
 * @brief   ：显示汉字字符串
 *
 * @param   xpos: 0~(319-16) 
 * @param   Ypos: 0~(239-16)
 * @param   str: 中文字符串首址
 * @param   Color: 字符颜色  
 *
 * @return  void
 */   
void PutChinese_strings(uint16_t Xpos,uint16_t Ypos,uint8_t *str,uint16_t back_color,uint16_t char_color)
{
    
    uint16_t Tmp_x, Tmp_y;
    uint8_t *tmp_str=str;
    Tmp_x = Ypos;
    Tmp_y = Xpos;
    
    
    while(*tmp_str != '\0')
    {
        PutChinese_16(Tmp_x,Tmp_y,tmp_str,back_color,char_color);
        
        tmp_str += 2 ;
        Tmp_y += 16 ;	
    }       
}


