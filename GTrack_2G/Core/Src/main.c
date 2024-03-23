/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "MC60.h"
#include "BMA253.h"
#include "GNSS_data.h"
#include "Fifo.h"
#include "../../MDK-ARM/W25Q16.h"
#include "Timer.h"
#include "UART.h"
#include "LowPowerMode.h"
#include "ModeLightIndicator.h"
#include "MQTT.h"
#include "StringProcessing.h"
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
I2C_HandleTypeDef hi2c1;

IWDG_HandleTypeDef hiwdg;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
#define numBlOCK 32 // number of total blocks for 16mb flash, 32x16X16 pagex 256 bytes
//#define RX_BUFFER_SIZE 2048	

//--------------------------------------------------------------------------------------------------------------------
//uint32_t last_uart3_time_Rx = 0; // thoi diem lan gan nhat uart3 truyen toi mcu
#define COUNT_WAKEUP_FROM_STOPMODE 40 // dem so lan wakeup tu IWDG cho STOPMODE
#define NUMBEROFRESTBYCREGFAIL 10 // dem so lan reset mcu tu viec creg loi 

//--------------------------------------------------------------------------------------------------------------------
//uint8_t rx_buffer[RX_BUFFER_SIZE]; 
//uint16_t rx_index  = 0; 
//uint8_t rx_buffer_temp[RX_BUFFER_SIZE];
//uint16_t rx_index_temp = 0;
uint8_t receive_flag = 0;
uint8_t publish_success = 0; 
uint16_t ID = 0;
uint8_t wakeup_From_Ext; // bien theo doi wakup gay ra tu nguyen nhan gi
uint8_t wakeup_count_From_IWDG; // bien theo doi wakup gay ra tu nguyen nhan gi
uint8_t go_to_Standby=1; // bien theo doi wakup gay ra tu nguyen nhan gi


//------------bien cho power low--------------------------------------------------------------------------------------------------------
uint32_t lastMotionTime = 0;
//-----------------END khai bao cao bien------------------------------------------------------------------------------------------------


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_I2C1_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM3_Init(void);
static void MX_IWDG_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//ham ngat ngoai
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == GPIO_PIN_3) {
      // Set the interrupt flag
			lastMotionTime = HAL_GetTick(); // Update the last motion time	
			wakeup_From_Ext=1;
			go_to_Standby=0;
			// For stop mode
			Exit_StopMode();
			Exit_StandbyMode();			
			HAL_UART_Transmit(&huart1, "\nMOVE\n", strlen("\nMOVE\n"), HAL_MAX_DELAY); 	
			//HAL_GPIO_TogglePin(GPIOA, pa12_Pin); 			
    }
}

// W25Q16

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
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
	Exit_StandbyMode(); //////////// exit standby_mode 		// exit from standby mode
	Exit_StopMode();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  //MX_TIM3_Init();
  MX_IWDG_Init();
  /* USER CODE BEGIN 2 */
	
//-------------BEGIN config and check cam bien gia toc----------------------
	configureNoMotionInterrupt(&hi2c1);
	//Exit_StandbyMode();
	HAL_StatusTypeDef ret= HAL_I2C_IsDeviceReady(&hi2c1,0x18<<1,1,100);

	for( int i=0; i<5;i++)
{
	if(ret==HAL_OK)
		{
			HAL_GPIO_WritePin(GPIOA, pa11_Pin, 0); 
			HAL_Delay(200); 
			HAL_GPIO_WritePin(GPIOA, pa11_Pin, 1); 
			HAL_Delay(200); 
		}
	else
		{
			HAL_GPIO_WritePin(GPIOA, pa12_Pin, 0); 
			HAL_Delay(200); 
			HAL_GPIO_WritePin(GPIOA, pa12_Pin, 1); 
			HAL_Delay(200); 
		}
}
//-------------END config and check cam bien gia toc---------------------------------------------------------------------------------


// kiem tra nguyen nhan wake up co phai do EXT3 hay khong---------------------------------------------------------------------------
//	W25Q16_Write_Byte_to_Sector_Start (SECTORFORwakeup_From_Ext, 1);// sectir 512 luu gia tri flag wakeup_From_Ext
//	W25Q16_Write_Byte_to_Sector_Start (SECTORFORwakeupcountFromIWDG, 0);// sectir 511 luu bien dem so lan vao stop mode ma wakeup tu IWDG wakeup_count_From_IWDG
//	W25Q16_Write_Byte_to_Sector_Start (SECTORFORgotoStandby, 0);// sectir 510 luu gia tri flag go_to_Standby
///-------------------setting gia tri ban dau khi flah chua ghi gi-----------------------//

//W25Q16_Write_Byte_to_Sector_Start(SECTORFORloop_Write_Index, 0);
//W25Q16_Write_Byte_to_Sector_Start(SECTORFORloop_Read_Index, 0);

//--------------- cau hinh ban dau --------------------------------------------------------
  //HAL_Delay(100);
	// Trước khi bắt đầu các hoạt động đọc/ghi mới
	//W25Q16_ReleaseFromDeepPowerDown();
	 //W25Q16_FastRead(SECTORFORlastKnownLocation,0,34,lastKnownLocation);// lay ra toa do gan nhat

			  // W25Q16_Write_Byte_to_Sector_Start(SECTORFORloop_Write_Index, 0);
			  // W25Q16_Write_Byte_to_Sector_Start(SECTORFORloop_Read_Index, 0);

//	LoadWriteSectorIndexFromFlash();
//	LoadReadSectorIndexFromFlash();
//	Load_SECTORFORloop_Write_Index();
//  Load_SECTORFORloop_Read_Index();




	 W25Q16_FastRead(SECTORFORwakeup_From_Ext,0,1,&wakeup_From_Ext); // doc gia tri flag wakeup_From_Ext
	 HAL_Delay(10);
//------------- neu wake up khong phai tu ngat ( thi se la thi watchdog) thi ta se tiep tuc StopMode hoac Standby------
if (go_to_Standby ==0) {  	W25Q16_Write_Byte_to_Sector_Start (SECTORFORgotoStandby, 0);  }// sectir 510 luu gia tri flag go_to_Standby  }
//----------------- neu reset boi creg nhieu lan thi stop mode----------------
			W25Q16_FastRead(COUNTRESETFROMCREG,0,1,&count_creg_resetMCU);
			if(count_creg_resetMCU>NUMBEROFRESTBYCREGFAIL)	{
			W25Q16_Write_Byte_to_Sector_Start (SECTORFORwakeup_From_Ext, 0);
			wakeup_From_Ext=0;
			W25Q16_Write_Byte_to_Sector_Start(COUNTRESETFROMCREG,0);
			Entry_StopMode();
			Exit_StopMode();
			}
//----------------- neu reset boi creg nhieu lan thi stop mode----------------
if ( wakeup_From_Ext==0){ 
	  W25Q16_FastRead(SECTORFORgotoStandby,0,1,&go_to_Standby);

		
		
	
	// vao standby tiep
	if ( go_to_Standby==1) Entry_StandbyMode();
	
	
	W25Q16_FastRead(SECTORFORwakeupcountFromIWDG,0,1,&wakeup_count_From_IWDG); // đoc gia tri cua bien  wakeup_count_From_IWDG
	if( wakeup_count_From_IWDG >COUNT_WAKEUP_FROM_STOPMODE) { 
		  W25Q16_Write_Byte_to_Sector_Start (SECTORFORgotoStandby, 1); // go to Standby lien tuc 
			W25Q16_Write_Byte_to_Sector_Start (SECTORFORwakeupcountFromIWDG, 0);// reset lai bien dem so lan wakeup tu IWDG
			Entry_StandbyMode(); 
	}
	wakeup_count_From_IWDG++;
	W25Q16_Write_Byte_to_Sector_Start (SECTORFORwakeupcountFromIWDG,wakeup_count_From_IWDG );

	
	
    Entry_StopMode();  
		Exit_StopMode();
}
if ( wakeup_From_Ext==1) {		W25Q16_Write_Byte_to_Sector_Start (SECTORFORwakeup_From_Ext, 1); // neu wakeup tu EXT thi  wakeup_From_Ex=1//} 
	
	
  }

//------------- neu wake up khong phai tu ngat ( thi se la thi watchdog) thi ta se tiep tuc StopMode------
				
		// spi flash
	W25Q16_Reset();
	ID=W25Q16_ReadID();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	HAL_UART_Receive_IT(&huart3, &rx_buffer[0], 1); // Start receiving data from MC60
	
	MC60_PowerOn();
	HAL_Delay(1000);
	AGPS_On();

  while (1)
  {
		HAL_IWDG_Refresh(&hiwdg);
		MQTT_Process();
//		sendCommandToMC60("AT+CBC\r\n");

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x2000090E;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief IWDG Initialization Function
  * @param None
  * @retval None
  */
static void MX_IWDG_Init(void)
{

  /* USER CODE BEGIN IWDG_Init 0 */

  /* USER CODE END IWDG_Init 0 */

  /* USER CODE BEGIN IWDG_Init 1 */

  /* USER CODE END IWDG_Init 1 */
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_256;
  hiwdg.Init.Window = 0xFFF;
  hiwdg.Init.Reload = 0xFFF;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN IWDG_Init 2 */

  /* USER CODE END IWDG_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */
HAL_NVIC_SetPriority(SPI1_IRQn, 2, 1); // Lower priority than EXTI
		  HAL_NVIC_EnableIRQ(SPI1_IRQn);
  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 99;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 7999;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */
	HAL_NVIC_SetPriority(USART1_IRQn, 3, 0); // Lower priority than EXTI
	  HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */
	HAL_NVIC_SetPriority(USART3_6_IRQn, 0, 0); // Lower priority than EXTI
	  HAL_NVIC_EnableIRQ(USART3_6_IRQn);
  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, pb12_Pin|pb13_Pin|pb14_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, pa11_Pin|pa12_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : CS1_Pin */
  GPIO_InitStruct.Pin = CS1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(CS1_GPIO_Port, &GPIO_InitStruct);
  /*Configure GPIO pins : PA1 PA2 PA3 PA8
                           PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_8
                          |GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
// Configure PA0 as input

GPIO_InitStruct.Pin = GPIO_PIN_0;
GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
GPIO_InitStruct.Pull = GPIO_PULLDOWN; 
HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  /*Configure GPIO pins : pb12_Pin pb13_Pin pb14_Pin */
  GPIO_InitStruct.Pin = pb12_Pin|pb13_Pin|pb14_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : pb15_Pin */
  GPIO_InitStruct.Pin = pb15_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(pb15_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : pa11_Pin pa12_Pin */
  GPIO_InitStruct.Pin = pa11_Pin|pa12_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI2_3_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI2_3_IRQn);

/* USER CODE END MX_GPIO_Init_2 */
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
