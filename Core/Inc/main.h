/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "sys.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define NTC1_Pin GPIO_PIN_0
#define NTC1_GPIO_Port GPIOC
#define NTC2_Pin GPIO_PIN_1
#define NTC2_GPIO_Port GPIOC
#define NTC3_Pin GPIO_PIN_2
#define NTC3_GPIO_Port GPIOC
#define NTC4_Pin GPIO_PIN_3
#define NTC4_GPIO_Port GPIOC
#define DR_485_Pin GPIO_PIN_1
#define DR_485_GPIO_Port GPIOA
#define TX_485_Pin GPIO_PIN_2
#define TX_485_GPIO_Port GPIOA
#define RX_485_Pin GPIO_PIN_3
#define RX_485_GPIO_Port GPIOA
#define TEMP_Pin GPIO_PIN_4
#define TEMP_GPIO_Port GPIOA
#define TFT_SCK_Pin GPIO_PIN_5
#define TFT_SCK_GPIO_Port GPIOA
#define TFT_DC_Pin GPIO_PIN_6
#define TFT_DC_GPIO_Port GPIOA
#define TFT_SDA_Pin GPIO_PIN_7
#define TFT_SDA_GPIO_Port GPIOA
#define TFT_RESET_Pin GPIO_PIN_4
#define TFT_RESET_GPIO_Port GPIOC
#define IN_24V_Pin GPIO_PIN_5
#define IN_24V_GPIO_Port GPIOC
#define IR_AO_Pin GPIO_PIN_1
#define IR_AO_GPIO_Port GPIOB
#define IR_DO_Pin GPIO_PIN_2
#define IR_DO_GPIO_Port GPIOB
#define LEVEL_L_Pin GPIO_PIN_10
#define LEVEL_L_GPIO_Port GPIOB
#define LEVEL_H_Pin GPIO_PIN_11
#define LEVEL_H_GPIO_Port GPIOB
#define W25_CS_Pin GPIO_PIN_12
#define W25_CS_GPIO_Port GPIOB
#define W25_SCK_Pin GPIO_PIN_13
#define W25_SCK_GPIO_Port GPIOB
#define W25_MISO_Pin GPIO_PIN_14
#define W25_MISO_GPIO_Port GPIOB
#define W25_MOSI_Pin GPIO_PIN_15
#define W25_MOSI_GPIO_Port GPIOB
#define D3_B3_Pin GPIO_PIN_6
#define D3_B3_GPIO_Port GPIOC
#define D2_B2_Pin GPIO_PIN_7
#define D2_B2_GPIO_Port GPIOC
#define D1_B1_Pin GPIO_PIN_8
#define D1_B1_GPIO_Port GPIOC
#define D0_B0_Pin GPIO_PIN_9
#define D0_B0_GPIO_Port GPIOC
#define OUT_24V_Pin GPIO_PIN_8
#define OUT_24V_GPIO_Port GPIOA
#define BUZZER_Pin GPIO_PIN_11
#define BUZZER_GPIO_Port GPIOA
#define LED_Pin GPIO_PIN_12
#define LED_GPIO_Port GPIOA
#define LED4_Pin GPIO_PIN_15
#define LED4_GPIO_Port GPIOA
#define LED3_Pin GPIO_PIN_10
#define LED3_GPIO_Port GPIOC
#define LED2_Pin GPIO_PIN_11
#define LED2_GPIO_Port GPIOC
#define LED1_Pin GPIO_PIN_12
#define LED1_GPIO_Port GPIOC
#define AC_OUT1_Pin GPIO_PIN_2
#define AC_OUT1_GPIO_Port GPIOD
#define AC_OUT2_Pin GPIO_PIN_3
#define AC_OUT2_GPIO_Port GPIOB
#define AC_OUT3_Pin GPIO_PIN_4
#define AC_OUT3_GPIO_Port GPIOB
#define AC_OUT4_Pin GPIO_PIN_5
#define AC_OUT4_GPIO_Port GPIOB
#define ZERO_Pin GPIO_PIN_6
#define ZERO_GPIO_Port GPIOB
#define ZERO_EXTI_IRQn EXTI9_5_IRQn
#define PWM1_Pin GPIO_PIN_7
#define PWM1_GPIO_Port GPIOB
#define PWM2_Pin GPIO_PIN_8
#define PWM2_GPIO_Port GPIOB
#define PWM3_Pin GPIO_PIN_9
#define PWM3_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

#define RS485_TX HAL_GPIO_WritePin(DR_485_GPIO_Port,DR_485_Pin,GPIO_PIN_SET)
#define RS485_RX HAL_GPIO_WritePin(DR_485_GPIO_Port,DR_485_Pin,GPIO_PIN_RESET)

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
