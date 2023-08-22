/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define BUTTON_Pin GPIO_PIN_0
#define BUTTON_GPIO_Port GPIOA
#define PPM_Pin GPIO_PIN_1
#define PPM_GPIO_Port GPIOA
#define iBUS_TX_Pin GPIO_PIN_2
#define iBUS_TX_GPIO_Port GPIOA
#define iBUS_RX_Pin GPIO_PIN_3
#define iBUS_RX_GPIO_Port GPIOA
#define FLASH_CS_Pin GPIO_PIN_4
#define FLASH_CS_GPIO_Port GPIOA
#define DRIVE_CH1_Pin GPIO_PIN_8
#define DRIVE_CH1_GPIO_Port GPIOA
#define DRIVE_CH2_Pin GPIO_PIN_9
#define DRIVE_CH2_GPIO_Port GPIOA
#define GNSS_TX_Pin GPIO_PIN_15
#define GNSS_TX_GPIO_Port GPIOA
#define GNSS_RX_Pin GPIO_PIN_3
#define GNSS_RX_GPIO_Port GPIOB
#define GNSS_EN_Pin GPIO_PIN_4
#define GNSS_EN_GPIO_Port GPIOB
#define SERVO1_Pin GPIO_PIN_5
#define SERVO1_GPIO_Port GPIOB
#define SERVO2_Pin GPIO_PIN_6
#define SERVO2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
