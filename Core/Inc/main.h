/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32l0xx_hal.h"

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
#define LED_AMP_DIRECT_Pin GPIO_PIN_2
#define LED_AMP_DIRECT_GPIO_Port GPIOE
#define LEDS_PWM_Pin GPIO_PIN_3
#define LEDS_PWM_GPIO_Port GPIOE
#define ENCODER_A_Pin GPIO_PIN_4
#define ENCODER_A_GPIO_Port GPIOE
#define ENCODER_A_EXTI_IRQn EXTI4_15_IRQn
#define ENCODER_B_Pin GPIO_PIN_5
#define ENCODER_B_GPIO_Port GPIOE
#define ENCODER_B_EXTI_IRQn EXTI4_15_IRQn
#define INPUT_DAC_Pin GPIO_PIN_6
#define INPUT_DAC_GPIO_Port GPIOE
#define INPUT_2_Pin GPIO_PIN_13
#define INPUT_2_GPIO_Port GPIOC
#define DAC_ENABLE_Pin GPIO_PIN_14
#define DAC_ENABLE_GPIO_Port GPIOC
#define INPUT_1_Pin GPIO_PIN_15
#define INPUT_1_GPIO_Port GPIOC
#define DC_PROTECT_Pin GPIO_PIN_9
#define DC_PROTECT_GPIO_Port GPIOH
#define DC_PROTECT_EXTI_IRQn EXTI4_15_IRQn
#define INPUT_0_Pin GPIO_PIN_10
#define INPUT_0_GPIO_Port GPIOH
#define SPEAKERS_ENABLE_Pin GPIO_PIN_0
#define SPEAKERS_ENABLE_GPIO_Port GPIOH
#define V_PROTECT_Pin GPIO_PIN_1
#define V_PROTECT_GPIO_Port GPIOH
#define V_PROTECT_EXTI_IRQn EXTI0_1_IRQn
#define BUTTONS_2_Pin GPIO_PIN_0
#define BUTTONS_2_GPIO_Port GPIOC
#define LED_VOLUME_Pin GPIO_PIN_1
#define LED_VOLUME_GPIO_Port GPIOC
#define LED_RGB_0_Pin GPIO_PIN_2
#define LED_RGB_0_GPIO_Port GPIOC
#define LED_RGB_1_Pin GPIO_PIN_3
#define LED_RGB_1_GPIO_Port GPIOC
#define ADC_VOLUME_0_Pin GPIO_PIN_0
#define ADC_VOLUME_0_GPIO_Port GPIOA
#define ADC_VOLUME_1_Pin GPIO_PIN_1
#define ADC_VOLUME_1_GPIO_Port GPIOA
#define LED_RGB_2_Pin GPIO_PIN_2
#define LED_RGB_2_GPIO_Port GPIOA
#define LEDS_PWM_RGB_Pin GPIO_PIN_3
#define LEDS_PWM_RGB_GPIO_Port GPIOA
#define ADC_BALANCE_Pin GPIO_PIN_4
#define ADC_BALANCE_GPIO_Port GPIOA
#define ADC_TREBLE_Pin GPIO_PIN_5
#define ADC_TREBLE_GPIO_Port GPIOA
#define ADC_BASS_Pin GPIO_PIN_6
#define ADC_BASS_GPIO_Port GPIOA
#define LEDS_PWM_AUX_Pin GPIO_PIN_7
#define LEDS_PWM_AUX_GPIO_Port GPIOA
#define LED_BALANCE_Pin GPIO_PIN_4
#define LED_BALANCE_GPIO_Port GPIOC
#define LED_TREBLE_Pin GPIO_PIN_5
#define LED_TREBLE_GPIO_Port GPIOC
#define LED_BASS_Pin GPIO_PIN_0
#define LED_BASS_GPIO_Port GPIOB
#define BUTTONS_1_Pin GPIO_PIN_1
#define BUTTONS_1_GPIO_Port GPIOB
#define LED_LOUDNESS_Pin GPIO_PIN_2
#define LED_LOUDNESS_GPIO_Port GPIOB
#define LED_SPEAKERS_B_Pin GPIO_PIN_8
#define LED_SPEAKERS_B_GPIO_Port GPIOD
#define LED_SPEAKERS_A_Pin GPIO_PIN_9
#define LED_SPEAKERS_A_GPIO_Port GPIOD
#define LED_DIRECT_Pin GPIO_PIN_10
#define LED_DIRECT_GPIO_Port GPIOD
#define IR_Pin GPIO_PIN_11
#define IR_GPIO_Port GPIOD
#define IR_EXTI_IRQn EXTI4_15_IRQn
#define POWER_ENABLE_Pin GPIO_PIN_12
#define POWER_ENABLE_GPIO_Port GPIOD
#define GPIO_0_Pin GPIO_PIN_13
#define GPIO_0_GPIO_Port GPIOD
#define POWER_BUTTON_Pin GPIO_PIN_14
#define POWER_BUTTON_GPIO_Port GPIOD
#define POWER_BUTTON_EXTI_IRQn EXTI4_15_IRQn
#define LED_POWER_Pin GPIO_PIN_15
#define LED_POWER_GPIO_Port GPIOD
#define LED_STANDBY_Pin GPIO_PIN_6
#define LED_STANDBY_GPIO_Port GPIOC
#define LED_APD_Pin GPIO_PIN_7
#define LED_APD_GPIO_Port GPIOC
#define SEC_POWER_Pin GPIO_PIN_8
#define SEC_POWER_GPIO_Port GPIOC
#define SEC_POWER_EXTI_IRQn EXTI4_15_IRQn
#define BUZZER_Pin GPIO_PIN_9
#define BUZZER_GPIO_Port GPIOC
#define BT_ENABLE_Pin GPIO_PIN_8
#define BT_ENABLE_GPIO_Port GPIOA
#define BT_TX_Pin GPIO_PIN_9
#define BT_TX_GPIO_Port GPIOA
#define BT_RX_Pin GPIO_PIN_10
#define BT_RX_GPIO_Port GPIOA
#define BT_CTS_Pin GPIO_PIN_11
#define BT_CTS_GPIO_Port GPIOA
#define BT_RTS_Pin GPIO_PIN_12
#define BT_RTS_GPIO_Port GPIOA
#define LED_HEARTBEAT_Pin GPIO_PIN_15
#define LED_HEARTBEAT_GPIO_Port GPIOA
#define BASS_ENABLE_Pin GPIO_PIN_10
#define BASS_ENABLE_GPIO_Port GPIOC
#define MUTE_DISABLE_Pin GPIO_PIN_11
#define MUTE_DISABLE_GPIO_Port GPIOC
#define DIRECT_ENABLE_Pin GPIO_PIN_12
#define DIRECT_ENABLE_GPIO_Port GPIOC
#define DIR_CS_Pin GPIO_PIN_0
#define DIR_CS_GPIO_Port GPIOD
#define DIR_SCK_Pin GPIO_PIN_1
#define DIR_SCK_GPIO_Port GPIOD
#define VOL_RESET_Pin GPIO_PIN_2
#define VOL_RESET_GPIO_Port GPIOD
#define DIR_MISO_Pin GPIO_PIN_3
#define DIR_MISO_GPIO_Port GPIOD
#define DIR_MISOD4_Pin GPIO_PIN_4
#define DIR_MISOD4_GPIO_Port GPIOD
#define WM_CS_Pin GPIO_PIN_5
#define WM_CS_GPIO_Port GPIOD
#define DIR_RESET_Pin GPIO_PIN_6
#define DIR_RESET_GPIO_Port GPIOD
#define DIR_ERR_Pin GPIO_PIN_7
#define DIR_ERR_GPIO_Port GPIOD
#define DIR_ERR_EXTI_IRQn EXTI4_15_IRQn
#define DIR_NPCM_Pin GPIO_PIN_3
#define DIR_NPCM_GPIO_Port GPIOB
#define LED_SACD_Pin GPIO_PIN_4
#define LED_SACD_GPIO_Port GPIOB
#define LED_NETWORK_Pin GPIO_PIN_5
#define LED_NETWORK_GPIO_Port GPIOB
#define LED_PHONO_Pin GPIO_PIN_8
#define LED_PHONO_GPIO_Port GPIOB
#define LED_TUNER_Pin GPIO_PIN_9
#define LED_TUNER_GPIO_Port GPIOB
#define LED_AUX_Pin GPIO_PIN_0
#define LED_AUX_GPIO_Port GPIOE
#define LED_RECORDER_Pin GPIO_PIN_1
#define LED_RECORDER_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
