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
#include "MC60.h"
#include "BMA253.h"
#include "GNSS_data.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
uint8_t rx_buffer[1000]; 
uint8_t cmd[5];
const char respond[]= "commandReceived\n";
uint8_t dataReceived = 0;


uint8_t receiveCommand(){
	if(!strcmp(cmd, "GPSON")) return 1;
	if(!strcmp(cmd, "GPSVW")) return 2;
	if(!strcmp(cmd, "_TEST")) return 3;
	return 0;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if(huart == &huart1){
//		dataReceived = 1;
		HAL_UART_Transmit_IT(&huart1, (uint8_t*)respond, sizeof(respond));
		HAL_GPIO_TogglePin(GPIOA, pa11_Pin);
	}
	if(huart == &huart3){
		HAL_GPIO_WritePin(GPIOA, pa11_Pin, 1);
	}	
}
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_I2C1_Init(void);
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
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
//	HAL_GPIO_WritePin(GPIOB, pb12_Pin, 0); 
//	HAL_Delay(100);
//	HAL_GPIO_WritePin(GPIOB, pb14_Pin, 1);	
//	HAL_Delay(2000);
	MC60_PowerOn();
	HAL_GPIO_WritePin(GPIOB, pb13_Pin, 1);
	HAL_Delay(4000);
//	MC60_ATCommand_Write(&huart1, (uint8_t*)"AT+QGNSSC", "1");
// 10 minutes 
	uint8_t tx_buffer_0[] = "AT+QIFGCNT=2\r\n";
	uint8_t tx_buffer_1[] = "AT+QICSGP=1,\"CMNET\"\r\n";
	uint8_t tx_buffer_2[] = "AT+CREG?;+CGREG?\r\n";
	uint8_t tx_buffer_3[] = "AT+QGNSSTS?\r\n";
	uint8_t tx_buffer_4[] = "AT+QGREFLOC=21.003189,105.507098\r\n";
	uint8_t tx_buffer_5[] = "AT+QGNSSEPO=1\r\n";
	uint8_t tx_buffer_6[] = "AT+QGNSSC=1\r\n";	
	uint8_t tx_buffer_7[] = "AT+QGNSSRD?\r\n";


// Firmware
//	uint8_t tx_buffer_0[] = "ATE0\r\n";
//	uint8_t tx_buffer_1[] = "AT+CPIN?\r\n";
//	uint8_t tx_buffer_2[] = "AT+QIFGCNT=2\r\n";
//	uint8_t tx_buffer_3[] = "AT+QICSGP=1,\"v-internet\"\r\n";
//	uint8_t tx_buffer_4[] = "AT+CREG?;+CGREG?\r\n";
//	uint8_t tx_buffer_5[] = "AT+QNTP=\"time.google.com\"\r\n";
//	uint8_t tx_buffer_6[] = "AT+CCLK?\r\n";
//	uint8_t tx_buffer_7[] = "AT+QGREFLOC=21.014935,105.832585\r\n";
//	uint8_t tx_buffer_8[] = "AT+QGNSSEPO=1\r\n";
//	uint8_t tx_buffer_9[] = "AT+QGNSSC=1\r\n";	
//	uint8_t tx_buffer_10[] = "AT+QGNSSRD?\r\n";
	
	
//	uint8_t tx_buffer_0[] = "AT+QMTOPEN=?\r\n";
//	uint8_t tx_buffer_1[] = "AT+QMTOPEN=0,\"mqtt://test.mosquitto.org:1883/\",1883\r\n";
//	//uint8_t tx_buffer_1[] = "AT+QMTOPEN=0,\"mqtt-dashboard.com\",8884\r\n";
//	uint8_t tx_buffer_2[] = "AT+QMTCONN=?\r\n";
//	uint8_t tx_buffer_3[] = "AT+QMTCONN=0,\"clientId-k3lyc9Czut\"\r\n";
	
	//uint8_t tx_buffer_4[] = "AT+QMTSUB=0,1,\"testtopic/1\",1\r\n";
	//uint8_t tx_buffer_5[] = "AT+QMTPUB=0,1,1,0,\"testtopic/1\",\"Hello World\"\r\n";	

// SSL - Firmware
//	uint8_t tx_buffer_0[] = "ATE0\r\n";
//	uint8_t tx_buffer_1[] = "AT+QSECWRITE=\"RAM:ca_cert.pem\",1448,100 \r\n";
//	uint8_t tx_buffer_2[] = "AT+QIFGCNT=0\r\n";
//	uint8_t tx_buffer_3[] = "AT+QSSLCFG=\"ignorertctime\",1\r\n";
//	uint8_t tx_buffer_4[] = "AT+QSSLCFG=\"sslversion\",0,4\r\n";
//	uint8_t tx_buffer_5[] = "AT+QSSLCFG=\"ciphersuite\",0,\"0XFFFF\"\r\n";
//	uint8_t tx_buffer_6[] = "AT+QSSLCFG=\"seclevel\",0,1\r\n";
//	uint8_t tx_buffer_7[] = "AT+QSECREAD=\"RAM:ca_cert.pem\"\r\n";
//	uint8_t tx_buffer_8[] = "AT+QSSLCFG=\"cacert\",0,\"RAM:ca_cert.pem\"\r\n";
//	uint8_t tx_buffer_9[] = "AT+QSSLOPEN=1,0,\"test.mosquitto.org\",1883,0,90\r\n";

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	
//		MC60_SendCommand(&huart3,(char*) tx_buffer_6);
	HAL_UART_Transmit_IT(&huart3,(uint8_t*)tx_buffer_6, sizeof(tx_buffer_6));
	HAL_Delay(200);
	HAL_UART_Receive_IT(&huart3,(uint8_t *)rx_buffer,sizeof(rx_buffer));	
//	HAL_UART_Receive_IT(&huart3, rx_buffer, sizeof(rx_buffer));
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		
			//UART_Receive(&huart1);
		HAL_Delay(1000);
//		HAL_UART_Transmit_IT(&huart3,(uint8_t*)tx_buffer_0, sizeof(tx_buffer_0));
//		HAL_Delay(200);
//		HAL_UART_Receive_IT(&huart3,(uint8_t *)rx_buffer,sizeof(rx_buffer));
//		HAL_UART_Transmit_IT(&huart3,(uint8_t*)tx_buffer_1, sizeof(tx_buffer_1));
//		HAL_Delay(200);
//		HAL_UART_Receive_IT(&huart3,(uint8_t *)rx_buffer,sizeof(rx_buffer));
//		HAL_UART_Transmit_IT(&huart3,(uint8_t*)tx_buffer_2, sizeof(tx_buffer_2));
//		HAL_Delay(200);
//		HAL_UART_Receive_IT(&huart3,(uint8_t *)rx_buffer,sizeof(rx_buffer));	
//		HAL_UART_Transmit_IT(&huart3,(uint8_t*)tx_buffer_3, sizeof(tx_buffer_3));
//		HAL_Delay(200);
//		HAL_UART_Receive_IT(&huart3,(uint8_t *)rx_buffer,sizeof(rx_buffer)); 
//		HAL_UART_Transmit_IT(&huart3,(uint8_t*)tx_buffer_4, sizeof(tx_buffer_4));
//		HAL_Delay(200);
//		HAL_UART_Receive_IT(&huart3,(uint8_t *)rx_buffer,sizeof(rx_buffer));
//		HAL_UART_Transmit_IT(&huart3,(uint8_t*)tx_buffer_5, sizeof(tx_buffer_5));
//		HAL_Delay(200);
//		HAL_UART_Receive_IT(&huart3,(uint8_t *)rx_buffer,sizeof(rx_buffer));
//		HAL_UART_Transmit_IT(&huart3,(uint8_t*)tx_buffer_6, sizeof(tx_buffer_6));
//		HAL_Delay(200);
//		HAL_UART_Receive_IT(&huart3,(uint8_t *)rx_buffer,sizeof(rx_buffer)); 
//		 
//		HAL_UART_Transmit_IT(&huart3,(uint8_t*)tx_buffer_7, sizeof(tx_buffer_7));
//		HAL_Delay(200);
//		HAL_UART_Receive_IT(&huart3,(uint8_t *)rx_buffer,sizeof(rx_buffer));
	
	HAL_UART_Receive_IT(&huart1, cmd, sizeof(cmd));
//	if(dataReceived){
	uint8_t i;
	i = receiveCommand();
	switch(i){
		case 1:{
			MC60_GNSS_On(&huart3);
			HAL_Delay(200);
			HAL_UART_Transmit_IT(&huart1, "GNSSActivated\n" , sizeof("GNSSActivated\n"));
			break;
		}
		case 2:{
			MC60_Receive_NMEA(&huart3);
			MC60_View_NMEA(&huart1);
			//MC60_Get_Sentence_By_Header(&huart1);
			break;
		}
		case 3:{ 
			//HAL_UART_Transmit_IT(&huart1, "tested\n", sizeof("tested\n"));
//			MC60_ATCommand_Send(&huart3, "AT+QGNSSC?\r\n");
//			HAL_Delay(200);
//			HAL_UART_Receive_IT(&huart3, rxbuffer, sizeof(rxbuffer));
			MC60_Debug_test(&huart3, "AT+QGNSSC?\r\n");
			//HAL_Delay(200);

			MC60_test_view(&huart1);
			//HAL_UART_Transmit_IT(&huart1, (uint8_t*)rx_buffer, sizeof(rx_buffer));

			break;
	  }
		default:
			break;
	}
//			dataReceived = 0;
//}
	
	
		uint8_t read = HAL_GPIO_ReadPin(GPIOB, pb15_Pin);
		if (read == 0){
			HAL_GPIO_WritePin(GPIOA, pa11_Pin, 0);
			HAL_Delay(200);
			HAL_GPIO_WritePin(GPIOA, pa11_Pin, 1);
			HAL_Delay(200);
		}
		else if (read == 1){
		  HAL_GPIO_WritePin(GPIOA, pa12_Pin, 0);
			HAL_Delay(200);
			HAL_GPIO_WritePin(GPIOA, pa12_Pin, 1);
			HAL_Delay(200);
		}
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
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, pb12_Pin|pb13_Pin|pb14_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, pa11_Pin|pa12_Pin, GPIO_PIN_RESET);

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
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
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
