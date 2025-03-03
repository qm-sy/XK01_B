#ifndef __LCD_H_
#define __LCD_H_

#include "stm32f1xx_hal.h"
#include "sys.h"
#include "delay.h"

extern uint16_t	POINT_COLOR;		//Default brush color
extern uint16_t	BACK_COLOR;		//Default background color
extern volatile uint8_t transmit_complete_flag1;
//Width and height definitions of LCD
#define LCD_Width 	320
#define LCD_Height 	240

//Brush color
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0 //黄色
#define BROWN 			 0x43BF //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
#define DARKBLUE      	 0XFE30	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

/*
	LCD_RST:	PC4
	LCD_DC:		PA6	
*/

#define	LCD_RST(n)		(n?HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_RESET))
#define	LCD_DC(n)		(n?HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET))

void wait_dma_transmit(void);
void LCD_Init(void);																	
void LCD_DisplayOn(void);																
void LCD_DisplayOff(void);																
void LCD_Write_Data_16bit(const uint16_t data);												
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);				
void LCD_Clear(uint16_t color);															
void LCD_Fill(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t color);				
void LCD_Draw_Point(uint16_t x, uint16_t y);														
void LCD_Draw_ColorPoint(uint16_t x, uint16_t y,uint16_t color);										
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color);								
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color);								
void LCD_Draw_Circle(uint16_t x0, uint16_t y0, uint8_t r,uint16_t color);											
void LCD_ShowChar(uint16_t x, uint16_t y, char chr, uint8_t size,uint16_t back_color,uint16_t char_color);		
void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint16_t back_color,uint16_t char_color);		
void LCD_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode,uint16_t back_color,uint16_t char_color);		
void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,char *p,uint16_t back_color,uint16_t char_color);		
void LCD_Show_Image_Internal_Flash(uint16_t x, uint16_t y, uint16_t width, uint16_t height,const uint8_t *pic, uint16_t pic_size);
void LCD_Show_Image_External_Flash(uint16_t x, uint16_t y, uint16_t width, uint16_t height,const uint8_t start_addr, uint16_t pic_size);				
void PutChinese_16(uint16_t Xpos,uint16_t Ypos,uint8_t *str,uint16_t back_color,uint16_t char_color);
void PutChinese_12(uint16_t Xpos,uint16_t Ypos,uint8_t *str,uint16_t back_color,uint16_t char_color);
void PutChinese_strings(uint16_t Xpos,uint16_t Ypos,uint8_t *str,uint16_t back_color,uint16_t char_color);

#endif


