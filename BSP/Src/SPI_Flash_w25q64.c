#include "SPI_Flash_w25q64.h"
//#include "../inc/pic.h"

extern SPI_HandleTypeDef hspi2;
static SPI_HandleTypeDef *g_HSPI_Flash = &hspi2;
volatile uint8_t receive_complete_flag2 = 0;


void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if( hspi->Instance == SPI2)
	{
		receive_complete_flag2 = 1;
	}
}


/**
 * @brief 使用SPI发送/接收数据
 *
 * @param pTxData: 要发送的数据
 * @param pRxData: 接收缓冲区
 * @param Size:    数据长度
 * @param Timeout: 超时时间(单位ms)
 * 
 * @return : 0 - 成功, -1 - 失败
 * 
**/
static int W25Q64_TxRx(uint8_t *pTxData, uint8_t *pRxData, uint16_t Size, uint32_t Timeout)
{
    if (HAL_OK == HAL_SPI_TransmitReceive(g_HSPI_Flash, pTxData, pRxData, Size, Timeout))
    {
        return 0;
    }else
    {
        return -1;
    }
}

/**
 * @brief 使用SPI发送数据
 *
 * @param pTxData: 要发送的数据
 * @param Size:    数据长度
 * @param Timeout: 超时时间(单位ms)
 * 
 * @return : 0 - 成功, -1 - 失败
 * 
**/
static int W25Q64_Tx(uint8_t *pTxData, uint16_t Size, uint32_t Timeout)
{
    if (HAL_OK == HAL_SPI_Transmit(g_HSPI_Flash, pTxData, Size, Timeout))
    {
        return 0;
    }else
    {
        return -1;
    }
}

/**
 * @brief 使用SPI读取数据
 *
 * @param pRxData: 接收缓冲区
 * @param Size:    数据长度
 * @param Timeout: 超时时间(单位ms)
 * 
 * @return : 0 - 成功, -1 - 失败
 * 
**/
static int  W25Q64_Rx(uint8_t *pRxData, uint16_t Size, uint32_t Timeout)
{
    if (HAL_OK == HAL_SPI_Receive(g_HSPI_Flash, pRxData, Size, Timeout))
    {
        return 0;
    }else
    {
        return -1;
    }
}

/**
 * @brief 使用SPI_DMA读取数据
 *
 * @param pRxData: 接收缓冲区
 * @param Size:    数据长度
 * @param Timeout: 超时时间(单位ms)
 * 
 * @return : 0 - 成功, -1 - 失败
 * 
**/
static int  W25Q64_Rx_DMA(uint8_t *pRxData, uint16_t Size)
{
    if (HAL_OK == HAL_SPI_Receive_DMA(&hspi2, pRxData, Size))
    {
        return 0;
    }else
    {
        return -1;
    }
}

/**
 * @brief 等待DMA传输完毕
 *
 * @param
 * 
 * @return 
 * 
**/
static void wait_spi2_dma_receive(void)
{
	while(receive_complete_flag2 == 0);
	receive_complete_flag2 = 0;
}

/**
 * @brief 读取芯片ID
 *
 * @param 
 * 
 * @return : 0XEF13 - 表示芯片型号为W25Q80
 *           0XEF14 - 表示芯片型号为W25Q16    
 *           0XEF15 - 表示芯片型号为W25Q32  
 *           0XEF16 - 表示芯片型号为W25Q64 
 * 
**/  
uint16_t W25X_ReadID(void)
{
    uint16_t Temp = 0;
    uint8_t tmpbuf[4] = {0x90,0x00,0x00,0x00};
    uint8_t rx_buf[4] = {0};

    W25Q64_Select;   

    W25Q64_Tx(tmpbuf,4,W25Q64_TIMEOUT);  
    W25Q64_Rx(rx_buf,2,W25Q64_TIMEOUT);

    W25Q64_Deselect;

    Temp|= rx_buf[0]<<8;  
    Temp|= rx_buf[1];  

    return Temp;
}


/**
 * @brief 等待W25Q64就绪
 *
 * @param 
 * 
 * @return : 0 - 成功, -1 - 失败
 * 
**/
static int W25Q64_WaitReady(void)
{
    unsigned char tx_buf[2];
    unsigned char rx_buf[2];
    int timeout = W25Q64_TIMEOUT;

    tx_buf[0] = 0x05; /* 读状态 */
    tx_buf[1] = 0xff;
	
	while (timeout--)
	{
		rx_buf[0] = rx_buf[1] = 0;
        W25Q64_Select;
		W25Q64_TxRx(tx_buf, rx_buf, 2, W25Q64_TIMEOUT);
        W25Q64_Deselect;
		if ((rx_buf[1] & 1) == 0)
        {
            return 0;
        }
        HAL_Delay(1);
	}

    if (!timeout)
    {
    	return -1;
    }
	return 0;
}

/**
 * @brief 写使能
 *
 * @param 
 * 
 * @return : 0 - 成功, -1 - 失败
 * 
**/
static int W25Q64_WriteEnable(void)
{
    unsigned char tmpbuf[1];
    int err;
    
    tmpbuf[0] = 0x06; /* 写使能 */
    W25Q64_Select;
    err = W25Q64_Tx(tmpbuf, 1, W25Q64_TIMEOUT);
    W25Q64_Deselect;

    return err;
}

/**
 * @brief W25Q64读函数
 *
 * @param offset:读哪个地址
 * @param buf   :用于保存数据
 * @param len   :读多少字节
 * 
 * @return : 非负数 - 读取了多少字节的数据, (-1) - 失败
 * 
**/
uint32_t W25Q64_Read(uint32_t offset, uint8_t *buf, uint32_t len)
{
    unsigned char tmpbuf[4];
    int err;
    
    /* 自己实现SPI Flash的读操作 */
    tmpbuf[0] = 0x03;
    tmpbuf[1] = (offset >> 16) & 0xff;
    tmpbuf[2] = (offset >> 8) & 0xff;
    tmpbuf[3] = (offset >> 0) & 0xff;
    
    W25Q64_Select;

    /* 发送读命令 */
    err = W25Q64_Tx(tmpbuf, 4, W25Q64_TIMEOUT);
    if (err)
    {
        W25Q64_Deselect;
        return 1;
    }

    /* 读数据 */
    err = W25Q64_Rx_DMA(buf, len);
    wait_spi2_dma_receive();
    //err = W25Q64_Rx(buf, len, W25Q64_TIMEOUT);
    if (err)
    {
        W25Q64_Deselect;
        return 1;
    }
    W25Q64_Deselect;    
    return len;
}

/**
 * @brief W25Q64写函数(需要先擦除)
 *
 * @param offset:写哪个地址
 * @param buf   :数据buffer
 * @param len   :写多少字节
 * 
 * @return : 非负数 - 读取了多少字节的数据, (-1) - 失败
 * 
**/
int W25Q64_Write(uint32_t offset, uint8_t *buf, uint32_t len)
{
    uint8_t tmpbuf[4];
    uint32_t phy_pos = offset;
    int err;
    uint32_t cur_len;
    uint32_t remain_len;

    /* 写数据 */
    phy_pos = offset;
    remain_len = len;

    /* 可能不是从Page开头写数据(1个Page是256字节) */
    cur_len = offset & (256-1);
    cur_len = 256 - cur_len;
    if (cur_len > len)
        cur_len = len;
    
    for (; phy_pos < offset + len; )
    {
        /* 写使能 */
        err = W25Q64_WriteEnable();
        if (err)
        {
            return -1;
        }
        
        tmpbuf[0] = 0x02; /* page program */
        tmpbuf[1] = (phy_pos >> 16) & 0xff;
        tmpbuf[2] = (phy_pos >> 8) & 0xff;
        tmpbuf[3] = (phy_pos >> 0) & 0xff;

        /* 发送page program命令 */
        W25Q64_Select;
        err = W25Q64_Tx(tmpbuf, 4, W25Q64_TIMEOUT);
        if (err)
        {
            W25Q64_Deselect;
            return -1;
        }
        /* 发送数据 */
        err = W25Q64_Tx(buf, cur_len, W25Q64_TIMEOUT);
        if (err)
        {
            W25Q64_Deselect;
            //printf("send data error \r\n");
            return -1;
        }
        W25Q64_Deselect;
        
        /* 读状态 */
        err = W25Q64_WaitReady();
        if (err)
        {
            //printf("read status error2 \r\n");
            return -1;
        }

        phy_pos += cur_len;
        buf     += cur_len;
        remain_len -= cur_len;

        cur_len = (remain_len < 256) ? remain_len : 256;
    }
    return len;
}

/**
 * @brief W25Q64擦除函数
 *
 * @param offset:擦除哪个地址(4096对齐)
 * @param len   :擦除多少字节(4096对齐)
 * 
 * @return : 非负数 - 读取了多少字节的数据, (-1) - 失败
 * 
**/
int W25Q64_Erase(uint32_t offset, uint32_t len)
{
    unsigned char tmpbuf[4];
    uint32_t phy_pos = offset;
    int err;

    if ((offset & (4096-1)) && (len & (4096-1)))
    {
        return -1;
    }

    for (int sector = 0; sector < len/4096; sector++)
    {
        /* 写使能 */
        err = W25Q64_WriteEnable();
        if (err)
        {
            return -1;
        }

        tmpbuf[0] = 0x20; /* 擦除 */
        tmpbuf[1] = (phy_pos >> 16) & 0xff;
        tmpbuf[2] = (phy_pos >> 8) & 0xff;
        tmpbuf[3] = (phy_pos >> 0) & 0xff;
        
        W25Q64_Select;

        err = W25Q64_Tx(tmpbuf, 4, W25Q64_TIMEOUT);

        if (err)
        {;
            W25Q64_Deselect;
            return -1;
        }
        W25Q64_Deselect;
        
        phy_pos += 4096;

        /* 读状态 */
        err = W25Q64_WaitReady();
        if (err)
        {
            return -1;
        }
    }
    return len;
}

/**
 * @brief W25Q64测试程序
 *
 * @param[in]
 * 
 * @return 
 * 
**/
void W25Q64_Test(void)
{
//    uint16_t chip_id;

//    chip_id = W25X_ReadID();
    //printf("The chip id is 0x%04x \r\n",chip_id);
    //W25Q64_Erase(0,967680);
    
    // W25Q64_Write(0,gImage_xiaonvhai1_1,30240);
    // W25Q64_Write(30240,gImage_xiaonvhai1_2,30240);
    // W25Q64_Write(60480,gImage_xiaonvhai1_3,30240);
    // W25Q64_Write(90720,gImage_xiaonvhai1_4,30240);	 

    // W25Q64_Write(120960,gImage_xiaonvhai2_1,30240);
    // W25Q64_Write(151200,gImage_xiaonvhai2_2,30240);
    // W25Q64_Write(181440,gImage_xiaonvhai2_3,30240);
    // W25Q64_Write(211680,gImage_xiaonvhai2_4,30240);

    // W25Q64_Write(241920,gImage_xiaonvhai3_1,30240);
    // W25Q64_Write(272160,gImage_xiaonvhai3_2,30240);
    // W25Q64_Write(302400,gImage_xiaonvhai3_3,30240);
    // W25Q64_Write(332640,gImage_xiaonvhai3_4,30240);	 

    // W25Q64_Write(362880,gImage_xiaonvhai4_1,30240);
    // W25Q64_Write(393120,gImage_xiaonvhai4_2,30240);
    // W25Q64_Write(423360,gImage_xiaonvhai4_3,30240);
    // W25Q64_Write(453600,gImage_xiaonvhai4_4,30240);

    // W25Q64_Write(483840,gImage_xiaonvhai5_1,30240);
    // W25Q64_Write(514080,gImage_xiaonvhai5_2,30240);
    // W25Q64_Write(544320,gImage_xiaonvhai5_3,30240);
    // W25Q64_Write(574560,gImage_xiaonvhai5_4,30240);	 

    // W25Q64_Write(604800,gImage_xiaonvhai6_1,30240);
    // W25Q64_Write(635040,gImage_xiaonvhai6_2,30240);
    // W25Q64_Write(665280,gImage_xiaonvhai6_3,30240);
    // W25Q64_Write(695520,gImage_xiaonvhai6_4,30240);

    // W25Q64_Write(725760,gImage_xiaonvhai7_1,30240);
    // W25Q64_Write(756000,gImage_xiaonvhai7_2,30240);
    // W25Q64_Write(786240,gImage_xiaonvhai7_3,30240);
    // W25Q64_Write(816480,gImage_xiaonvhai7_4,30240);	 

    // W25Q64_Write(846720,gImage_xiaonvhai8_1,30240);
    // W25Q64_Write(876960,gImage_xiaonvhai8_2,30240);
    // W25Q64_Write(907200,gImage_xiaonvhai8_3,30240);
    // W25Q64_Write(937440,gImage_xiaonvhai8_4,30240);

    
    // HAL_UART_Transmit_DMA(&huart2,buf2,5);
    // W25X_ReadID();
}
