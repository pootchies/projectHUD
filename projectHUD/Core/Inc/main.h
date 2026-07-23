/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#define LIGHT_Pin GPIO_PIN_0
#define LIGHT_GPIO_Port GPIOC
#define TACH_Pin GPIO_PIN_0
#define TACH_GPIO_Port GPIOA
#define SPEED_Pin GPIO_PIN_1
#define SPEED_GPIO_Port GPIOA
#define BLANK_Pin GPIO_PIN_1
#define BLANK_GPIO_Port GPIOB
#define SCLK_Pin GPIO_PIN_13
#define SCLK_GPIO_Port GPIOB
#define LATCH_Pin GPIO_PIN_14
#define LATCH_GPIO_Port GPIOB
#define SIN_Pin GPIO_PIN_15
#define SIN_GPIO_Port GPIOB
#define GREEN_LED_Pin GPIO_PIN_7
#define GREEN_LED_GPIO_Port GPIOC
#define YELLOW_LED_Pin GPIO_PIN_8
#define YELLOW_LED_GPIO_Port GPIOC
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
