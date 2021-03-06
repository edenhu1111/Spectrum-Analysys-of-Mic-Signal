/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32h7xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32h7xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "adc.h"
#include <rtthread.h>
#include "arm_math.h"
#include "rthw.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern ADC_HandleTypeDef hadc1;
/* USER CODE BEGIN EV */
static uint8_t dat_num = 0;
extern rt_sem_t dynamic_sem;
extern float32_t signal_for_fft[64];
//static uint16_t adc_val;
extern float32_t fft_in[64];
extern float32_t w_blackman[64];

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
//void NMI_Handler(void)
//{
//  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */
//////////////////////////////////////////////////////////////////////
//  /* USER CODE END NonMaskableInt_IRQn 0 */
//  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
//////////////////////////////////////////////////////////////////////  while (1)
//////////////////////////////////////////////////////////////////////  {
//////////////////////////////////////////////////////////////////////  }
//  /* USER CODE END NonMaskableInt_IRQn 1 */
//}
//
///**
//  * @brief This function handles Hard fault interrupt.
//  */
//void HardFault_Handler(void)
//{
//  /* USER CODE BEGIN HardFault_IRQn 0 */
//////////////////////////////////////////////////////////////////////
//  /* USER CODE END HardFault_IRQn 0 */
//  while (1)
//  {
//    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
//    /* USER CODE END W1_HardFault_IRQn 0 */
//  }
//}
//
///**
//  * @brief This function handles Memory management fault.
//  */
//void MemManage_Handler(void)
//{
//  /* USER CODE BEGIN MemoryManagement_IRQn 0 */
//////////////////////////////////////////////////////////////////////
//  /* USER CODE END MemoryManagement_IRQn 0 */
//  while (1)
//  {
//    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
//    /* USER CODE END W1_MemoryManagement_IRQn 0 */
//  }
//}
//
///**
//  * @brief This function handles Pre-fetch fault, memory access fault.
//  */
//void BusFault_Handler(void)
//{
//  /* USER CODE BEGIN BusFault_IRQn 0 */
//////////////////////////////////////////////////////////////////////
//  /* USER CODE END BusFault_IRQn 0 */
//  while (1)
//  {
//    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
//    /* USER CODE END W1_BusFault_IRQn 0 */
//  }
//}
//
///**
//  * @brief This function handles Undefined instruction or illegal state.
//  */
//void UsageFault_Handler(void)
//{
//  /* USER CODE BEGIN UsageFault_IRQn 0 */
//////////////////////////////////////////////////////////////////////
//  /* USER CODE END UsageFault_IRQn 0 */
//  while (1)
//  {
//    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
//    /* USER CODE END W1_UsageFault_IRQn 0 */
//  }
//}
//
///**
//  * @brief This function handles System service call via SWI instruction.
//  */
//void SVC_Handler(void)
//{
//  /* USER CODE BEGIN SVCall_IRQn 0 */
//////////////////////////////////////////////////////////////////////
//  /* USER CODE END SVCall_IRQn 0 */
//  /* USER CODE BEGIN SVCall_IRQn 1 */
//////////////////////////////////////////////////////////////////////
//  /* USER CODE END SVCall_IRQn 1 */
//}
//
///**
//  * @brief This function handles Debug monitor.
//  */
//void DebugMon_Handler(void)
//{
//  /* USER CODE BEGIN DebugMonitor_IRQn 0 */
//////////////////////////////////////////////////////////////////////
//  /* USER CODE END DebugMonitor_IRQn 0 */
//  /* USER CODE BEGIN DebugMonitor_IRQn 1 */
//////////////////////////////////////////////////////////////////////
//  /* USER CODE END DebugMonitor_IRQn 1 */
//}
//
///**
//  * @brief This function handles Pendable request for system service.
//  */
//void PendSV_Handler(void)
//{
//  /* USER CODE BEGIN PendSV_IRQn 0 */
//////////////////////////////////////////////////////////////////////
//  /* USER CODE END PendSV_IRQn 0 */
//  /* USER CODE BEGIN PendSV_IRQn 1 */
//////////////////////////////////////////////////////////////////////
//  /* USER CODE END PendSV_IRQn 1 */
//}
//
///**
//  * @brief This function handles System tick timer.
//  */
//void SysTick_Handler(void)
//{
//  /* USER CODE BEGIN SysTick_IRQn 0 */
//////////////////////////////////////////////////////////////////////
//  /* USER CODE END SysTick_IRQn 0 */
//  HAL_IncTick();
//  /* USER CODE BEGIN SysTick_IRQn 1 */
//////////////////////////////////////////////////////////////////////
//  /* USER CODE END SysTick_IRQn 1 */
//}

/******************************************************************************/
/* STM32H7xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32h7xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles ADC1 and ADC2 global interrupts.
  */
void ADC_IRQHandler(void)
{
  /* USER CODE BEGIN ADC_IRQn 0 */
//  rt_interrupt_enter();
  /* USER CODE END ADC_IRQn 0 */
  HAL_ADC_IRQHandler(&hadc1);
  /* USER CODE BEGIN ADC_IRQn 1 */
//  rt_interrupt_leave();
  /* USER CODE END ADC_IRQn 1 */
}

/* USER CODE BEGIN 1 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) //ADC????????????????????????????????????????????
{
//  The bit number of ADC is 10.
//  adc_val = HAL_ADC_GetValue(&hadc1);
//  signal_for_fft[dat_num] = adc_val * 0.00322265625;
//    rt_interrupt_enter();
  signal_for_fft[dat_num] = HAL_ADC_GetValue(&hadc1)* 0.00322265625;
  dat_num++;
  if(dat_num >= 64){
      dat_num = 0;
      rt_sem_take(dynamic_sem,RT_WAITING_FOREVER);      //?????????take
      for(int i = 0;i<64;i++){
          fft_in[i] = signal_for_fft[i] * w_blackman[i];// ??????????????????????????? ???Blackman??????
      }
      rt_sem_release(dynamic_sem);                      //?????????release
  }
//  rt_interrupt_leave();

}
/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
