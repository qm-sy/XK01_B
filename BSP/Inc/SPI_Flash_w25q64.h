#ifndef __SPI_FLASH_W25Q64_
#define __SPI_FLASH_W25Q64_

#include "main.h"

/* CS:        PB12
 * SPI1_MISO: PB14
 * SPI1_MOSI: PB15
 * SPI1_SCK:  PB13
 */

#define W25Q64_CS_GPIO_GROUP GPIOB
#define W25Q64_CS_GPIO_PIN   GPIO_PIN_12
#define W25Q64_TIMEOUT       500

#define W25Q64_Select   HAL_GPIO_WritePin(W25Q64_CS_GPIO_GROUP, W25Q64_CS_GPIO_PIN, GPIO_PIN_RESET)
#define W25Q64_Deselect HAL_GPIO_WritePin(W25Q64_CS_GPIO_GROUP, W25Q64_CS_GPIO_PIN, GPIO_PIN_SET)

extern volatile uint8_t receive_complete_flag2;

uint16_t W25X_ReadID(void);
uint32_t W25Q64_Read(uint32_t offset, uint8_t *buf, uint32_t len);
int W25Q64_Write(uint32_t offset, uint8_t *buf, uint32_t len);
int W25Q64_Erase(uint32_t offset, uint32_t len);
void W25Q64_Test(void);

#endif

