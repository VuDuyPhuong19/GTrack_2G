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
#include "stm32f0xx_hal.h"

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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define pb12_Pin GPIO_PIN_12
#define pb12_GPIO_Port GPIOB
#define pb13_Pin GPIO_PIN_13
#define pb13_GPIO_Port GPIOB
#define pb14_Pin GPIO_PIN_14
#define pb14_GPIO_Port GPIOB
#define pb15_Pin GPIO_PIN_15
#define pb15_GPIO_Port GPIOB
#define pa11_Pin GPIO_PIN_11
#define pa11_GPIO_Port GPIOA
#define pa12_Pin GPIO_PIN_12
#define pa12_GPIO_Port GPIOA
#define pb6_scl_Pin GPIO_PIN_6
#define pb6_scl_GPIO_Port GPIOB
#define pb7_sda_Pin GPIO_PIN_7
#define pb7_sda_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
