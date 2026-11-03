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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define OSC_IN_Pin GPIO_PIN_0
#define OSC_IN_GPIO_Port GPIOH
#define OSC_OUT_Pin GPIO_PIN_1
#define OSC_OUT_GPIO_Port GPIOH
#define PWMA1_Pin GPIO_PIN_0
#define PWMA1_GPIO_Port GPIOB
#define PWMA2_Pin GPIO_PIN_1
#define PWMA2_GPIO_Port GPIOB
#define PWM3_Pin GPIO_PIN_9
#define PWM3_GPIO_Port GPIOE
#define PWM1_Pin GPIO_PIN_13
#define PWM1_GPIO_Port GPIOE
#define PWM2_Pin GPIO_PIN_14
#define PWM2_GPIO_Port GPIOE
#define XJ1_Pin GPIO_PIN_8
#define XJ1_GPIO_Port GPIOD
#define XJ2_Pin GPIO_PIN_9
#define XJ2_GPIO_Port GPIOD
#define XJ3_Pin GPIO_PIN_10
#define XJ3_GPIO_Port GPIOD
#define XJ4_Pin GPIO_PIN_11
#define XJ4_GPIO_Port GPIOD
#define DIN2_Pin GPIO_PIN_6
#define DIN2_GPIO_Port GPIOC
#define DIN1_Pin GPIO_PIN_7
#define DIN1_GPIO_Port GPIOC
#define CIN2_Pin GPIO_PIN_8
#define CIN2_GPIO_Port GPIOC
#define CIN1_Pin GPIO_PIN_9
#define CIN1_GPIO_Port GPIOC
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define PWMB1_Pin GPIO_PIN_4
#define PWMB1_GPIO_Port GPIOB
#define PWMB2_Pin GPIO_PIN_5
#define PWMB2_GPIO_Port GPIOB
#define BIN2_Pin GPIO_PIN_6
#define BIN2_GPIO_Port GPIOB
#define BIN1_Pin GPIO_PIN_7
#define BIN1_GPIO_Port GPIOB
#define AIN2_Pin GPIO_PIN_8
#define AIN2_GPIO_Port GPIOB
#define AIN1_Pin GPIO_PIN_9
#define AIN1_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
