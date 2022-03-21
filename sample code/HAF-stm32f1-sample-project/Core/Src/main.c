/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "i2c.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "HAF.h"
#include "Port.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

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
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  HafSensorConfig senssor = {0};

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  // put your setup code here, to run once:
  senssor.i2cAddress = 0x48;
  senssor.flowOffset = 0;
  Port_I2cInit();
  printf("The driver version is %d.%02d\r\n",MAJOR_VERSION,MINOR_VERSION);
  printf("I2c init OK!\r\n");
  //suggest warm up time
  HAL_Delay(50);

  /*********************get serial number**********************/
  while (HAF_GetSn(senssor.i2cAddress, senssor.sn) != 0)
  {
    printf("Get sn error!\r\n");
    HAL_Delay(1000);
  }
  printf("SN: %02X%02X%02X%02X\r\n",senssor.sn[0],senssor.sn[1],senssor.sn[2],senssor.sn[3]);

  /************************get factor*************************/
  while (HAF_GetFactor(senssor.i2cAddress, &senssor.flowFactor) != 0)
  {
    printf("Get factor error!\r\n");
  }
  printf("Factor: %d\r\n",senssor.flowFactor);

  //get unit
  if (HAF_GetUnit(senssor.i2cAddress, &senssor.flowUnit) != 0)
  {
    printf("Get unit error!\r\n");
  }
  else
  {
    if (senssor.flowUnit == HAF_UNIT_SCCM)
	  printf("Unit: sccm\r\n");
    else if (senssor.flowUnit == HAF_UNIT_SLM)
	  printf("Unit: slm\r\n");
  }

  /**************************set gas type**********************/
  if (HAF_SetGasType(senssor.i2cAddress, HAF_GAS_TYPE_AIR) != 0)
  {
    printf("Set gas type error!\r\n");
  }
  else
  {
    printf("Set gas type successfully!\r\n");
  }

  /*********************set gas proportion**********************/
  if (HAF_SetGasProportion(senssor.i2cAddress, 1000) != 0)
  {
    printf("Set gas proportion error!\r\n");
  }
  else
  {
    printf("Set gas proportion successfully!\r\n");
  }

  /*********************get gas type***************************/
  if (HAF_GetGasType(senssor.i2cAddress, &senssor.gasType) != 0)
  {
    printf("Get gas type error!\r\n");
  }
  else
  {
    if (senssor.gasType == HAF_GAS_TYPE_AIR)
	  printf("Gas type: Air\r\n");
    else if (senssor.gasType == HAF_GAS_TYPE_O2)
	  printf("Gas type: O2\r\n");
    else if (senssor.gasType == HAF_GAS_TYPE_N2O)
	  printf("Gas type: N2O\r\n");
    else if (senssor.gasType == HAF_GAS_TYPE_O2_AIR)
	  printf("Gas type: O2&Air\r\n");
    else if (senssor.gasType == HAF_GAS_TYPE_N2O_AIR)
	  printf("Gas type: N2O&Air\r\n");
    else if (senssor.gasType == HAF_GAS_TYPE_O2_N2O)
	  printf("Gas type: O2&N2O\r\n");
  }

  /*********************get gas proportion**********************/
  if (HAF_GetGasProportion(senssor.i2cAddress, &senssor.gasProportion) != 0)
  {
    printf("Get gas proportion error!\r\n");
  }
  else
  {
    printf("Gas proportion: %.1f%\r\n",(float)senssor.gasProportion/10);
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    // put your main code here, to run repeatedly:
    /*********************get flow and temp**********************/
    if (HAF_GetFlowTemp(senssor.i2cAddress, &senssor.flow, &senssor.temp) != 0)
    {
      printf("Get flow and temp error!");
    }
    else
    {
  	  if (senssor.flowUnit == HAF_UNIT_SCCM)
        printf("Flow: %.3fsccm",(float)senssor.flow / senssor.flowFactor);
	  else if (senssor.flowUnit == HAF_UNIT_SLM)
        printf("Flow: %.3fslm",(float)senssor.flow / senssor.flowFactor);
      printf("   Temp: %.1f¡æ\r\n",(float)senssor.temp / 100);
    }

    HAL_Delay(1000);
	  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
