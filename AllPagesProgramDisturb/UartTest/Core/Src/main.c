/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
static UART_HandleTypeDef huart1;

NAND_HandleTypeDef hnand1;

/* USER CODE BEGIN PV */
static NAND_HandleTypeDef hNAND; /* NAND handle */
static FMC_NAND_PCC_TimingTypeDef NAND_ComTiming; /* Common memory space timing */
static FMC_NAND_PCC_TimingTypeDef NAND_AttTiming; /* Attribute memory space timing */
static NAND_IDTypeDef NAND_ID = {0};
//uint8_t aTxBuffer[BUFFER_SIZE] = {0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41};
uint8_t aTxBuffer[BUFFER_SIZE] = {0};
uint8_t aRxBuffer[BUFFER_SIZE]= {0};
uint8_t stableBytes1[BUFFER_SIZE] = {0};
uint8_t stableBytes3[BUFFER_SIZE] = {0};
#define PAGE_0 0
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_FMC_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
/*int _write(int file, char *data, int len)
{
   HAL_StatusTypeDef status = HAL_UART_Transmit(&huart1, (uint8_t*)data, len, 1000);

   // return # of bytes written - as best we can tell
   return (status == HAL_OK ? len : 0);
}*/
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int NandControllerInitAndTest(void)
{
//NAND_AddressTypedef Address;

 /*##-1- Configure the NAND device ###############################*/
//	hNAND.Instance = FMC_NAND_DEVICE;
//
	/* NAND device configuration */
	NAND_ComTiming.SetupTime = 0xf1;
	NAND_ComTiming.WaitSetupTime = 0xf2;
	NAND_ComTiming.HoldSetupTime = 0xf3;
	NAND_ComTiming.HiZSetupTime = 0xf1;
	NAND_AttTiming.SetupTime = 0xf1;
	NAND_AttTiming.WaitSetupTime = 0xf2;
	NAND_AttTiming.HoldSetupTime = 0xf3;
	NAND_AttTiming.HiZSetupTime = 0xf1;

	hNAND.Init.NandBank = FMC_NAND_BANK2;
	hNAND.Init.Waitfeature = FMC_NAND_PCC_WAIT_FEATURE_ENABLE;

	hNAND.Init.MemoryDataWidth = FMC_NAND_PCC_MEM_BUS_WIDTH_8;
	hNAND.Init.EccComputation = FMC_NAND_ECC_DISABLE;
	hNAND.Init.ECCPageSize = FMC_NAND_ECC_PAGE_SIZE_512BYTE;
	hNAND.Init.TCLRSetupTime = 0x0;
	hNAND.Init.TARSetupTime = 0x0;
	hNAND.Config.BlockNbr = 0x400;
	hNAND.Config.BlockSize = 0x40;
	hNAND.Config.PageSize = 0x800;
	hNAND.Config.PlaneNbr = 0x01;
	hNAND.Config.PlaneSize = 0x400;
	hNAND.Config.SpareAreaSize = 0x40;

	uint8_t nextLine[0X02] = "\n\n";
	uint8_t iterationStr[32] = "ITERATION PAGE IS THE FOLLOWING";
	uint8_t flippedBitStr[41] = "FLIPPED BIT PAGE NUMBER IS THE FOLLOWING";
	uint8_t deviceInfo[28] = "PRINTING DEVICE INFORMATION";

	/* NAND controller initialization */
	if(HAL_NAND_Init(&hNAND, &NAND_ComTiming, &NAND_AttTiming) != HAL_OK)
	{
		return NAND_STATUS_INIT_ERROR;
	}

	/*##-2- Read & check the NAND device IDs ########################*/
	/* Read the NAND memory ID */
	if(HAL_NAND_Read_ID(&hNAND, &NAND_ID) != HAL_OK)
	{
		return NAND_STATUS_READ_ID_ERROR;
	}

	HAL_UART_Transmit(&huart1, deviceInfo, sizeof(deviceInfo), 100);
	HAL_Delay(100);
	HAL_UART_Transmit(&huart1, nextLine, sizeof(nextLine), 100);
	HAL_Delay(100);
	HAL_UART_Transmit(&huart1, nextLine, sizeof(nextLine), 100);
	HAL_Delay(100);
	HAL_UART_Transmit(&huart1, &(NAND_ID.Maker_Id), sizeof(&(NAND_ID.Maker_Id)), 100);
	HAL_Delay(100);
	HAL_UART_Transmit(&huart1, &(NAND_ID.Device_Id), sizeof(&(NAND_ID.Device_Id)), 100);
	HAL_Delay(100);
	HAL_UART_Transmit(&huart1, &(NAND_ID.Third_Id), sizeof(&(NAND_ID.Third_Id)), 100);
	HAL_Delay(100);
	HAL_UART_Transmit(&huart1, nextLine, sizeof(nextLine), 100);
	HAL_Delay(100);
	HAL_UART_Transmit(&huart1, nextLine, sizeof(nextLine), 100);
	HAL_Delay(100);
	HAL_UART_Transmit(&huart1, nextLine, sizeof(nextLine), 100);
	HAL_Delay(100);



	NAND_AddressTypedef Address_BLOCK_0;

	Address_BLOCK_0.Page = PAGE_0;
	Address_BLOCK_0.Plane = 0;
	Address_BLOCK_0.Block = 0;


	/* Send NAND erase block operation */
	if (HAL_NAND_Erase_Block(&hNAND, &Address_BLOCK_0) != HAL_OK)
	{
		return NAND_STATUS_ERASE_ERROR;
	}


	for(int i=0; i<BUFFER_SIZE; i++)
	{
	    aTxBuffer[i] = 0XAA;
	    stableBytes1[i] = 0X00;
	}


	uint8_t page = 0X00;
	uint8_t ipage = 0X00;
	uint8_t flag = 0X00;


	/* Write test-data to all the pages in the block the NAND memory */
	for(page=0X00; page <= 0X40; page++){
		Address_BLOCK_0.Page = page;
		if (HAL_NAND_Write_Page_8b(&hNAND, &Address_BLOCK_0, aTxBuffer, 1) != HAL_OK){
				return NAND_STATUS_WRITE_ERROR;
		}
	}


	// Iterate over all the 64 pages,
	// On each iteration, write to a single page continuously 10000 times
	// and then reading other pages to check for program disturb.
	// Print only if there is a bit-flip detected.
	// stableBytes - represents the program disturb.
	for(page=0X00; page <= 0X10; page++){
		HAL_UART_Transmit(&huart1, iterationStr, sizeof(iterationStr), 100);
		HAL_Delay(100);
		HAL_UART_Transmit(&huart1, nextLine, sizeof(nextLine), 100);
		HAL_Delay(100);
		HAL_UART_Transmit(&huart1, nextLine, sizeof(nextLine), 100);
		HAL_Delay(100);
		HAL_UART_Transmit(&huart1, &page, sizeof(&page), 100);
		HAL_Delay(100);
		HAL_UART_Transmit(&huart1, nextLine, sizeof(nextLine), 100);
		HAL_Delay(100);
		HAL_UART_Transmit(&huart1, nextLine, sizeof(nextLine), 100);
		HAL_Delay(100);
		Address_BLOCK_0.Page = page;
		/* Reset the stableBytes1 */
		for(int is=0; is<BUFFER_SIZE; is++){
			stableBytes1[is] = 0X00;
		}

		/* Send NAND erase block operation */
		if (HAL_NAND_Erase_Block(&hNAND, &Address_BLOCK_0) != HAL_OK)
		{
			return NAND_STATUS_ERASE_ERROR;
		}

		/*##-4- NAND memory read/write access ###########################*/
		/* Write data to the NAND memory */
		for(ipage=0X00; ipage <= 0X40; ipage++){
			Address_BLOCK_0.Page = ipage;
			if (HAL_NAND_Write_Page_8b(&hNAND, &Address_BLOCK_0, aTxBuffer, 1) != HAL_OK){
					return NAND_STATUS_WRITE_ERROR;
			}
		}


		Address_BLOCK_0.Page = page;
		for(int i = 0; i < 5000; i++){
			if (HAL_NAND_Write_Page_8b(&hNAND, &Address_BLOCK_0, aTxBuffer, 1) != HAL_OK){
				return NAND_STATUS_WRITE_ERROR;
			}
		}

		/* Read back data from the NAND memory */
		for(ipage = 0; ipage < 0X40; ipage++){
			Address_BLOCK_0.Page = ipage;
			if (HAL_NAND_Read_Page_8b(&hNAND, &Address_BLOCK_0, aRxBuffer, 1) != HAL_OK){
				return NAND_STATUS_READ_ERROR;
			}
			for(int i=0; i<BUFFER_SIZE; i++){
				if(aRxBuffer[i]==0XAA)
				{
					stableBytes1[i] = 0XAA;
				}
				else{
					stableBytes1[i] = aRxBuffer[i];
					flag = 0X01;
				}
			}
			// Print only for fliped bits
			if(flag == 0X01){
				HAL_UART_Transmit(&huart1, flippedBitStr, sizeof(flippedBitStr), 100);
				HAL_Delay(100);
				HAL_UART_Transmit(&huart1, nextLine, sizeof(nextLine), 100);
				HAL_Delay(100);
				HAL_UART_Transmit(&huart1, nextLine, sizeof(nextLine), 100);
				HAL_Delay(100);
				HAL_UART_Transmit(&huart1, &ipage, sizeof(&ipage), 100);
				HAL_Delay(100);
				HAL_UART_Transmit(&huart1, nextLine, sizeof(nextLine), 100);
				HAL_Delay(100);
				HAL_UART_Transmit(&huart1, nextLine, sizeof(nextLine), 100);
				HAL_Delay(100);
				HAL_UART_Transmit(&huart1, nextLine, sizeof(nextLine), 100);
				HAL_Delay(100);
				HAL_UART_Transmit(&huart1, nextLine, sizeof(nextLine), 100);
				HAL_Delay(100);
				HAL_UART_Transmit(&huart1, stableBytes1, sizeof(stableBytes1), 100);
				HAL_Delay(100);
				HAL_UART_Transmit(&huart1, nextLine, sizeof(nextLine), 100);
				HAL_Delay(100);
				HAL_UART_Transmit(&huart1, nextLine, sizeof(nextLine), 100);
				HAL_Delay(100);
				HAL_UART_Transmit(&huart1, nextLine, sizeof(nextLine), 100);
				HAL_Delay(100);
				HAL_UART_Transmit(&huart1, nextLine, sizeof(nextLine), 100);
				HAL_Delay(100);
				flag = 0X00;
			}
		}
	}
	return NAND_STATUS_OK;
}
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
  MX_FMC_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  uint8_t errorMsg[21] = "Something Went Wrong";
  uint8_t successMsg[27] = "SUCCESS RERUNNING THE TEST";
  uint8_t nextLine[0X02] = "\n\n";

  while (1)
  {
    /* USER CODE END WHILE */
	if(NandControllerInitAndTest() != NAND_STATUS_OK){
		HAL_UART_Transmit(&huart1, errorMsg, sizeof(errorMsg), 100);
		HAL_Delay(500);
	}
	else{
		HAL_UART_Transmit(&huart1, successMsg, sizeof(successMsg), 100);
		HAL_Delay(500);
		HAL_UART_Transmit(&huart1, nextLine, sizeof(nextLine), 100);
		HAL_Delay(500);
		HAL_UART_Transmit(&huart1, nextLine, sizeof(nextLine), 100);
		HAL_Delay(500);
		HAL_UART_Transmit(&huart1, nextLine, sizeof(nextLine), 100);
		HAL_Delay(500);
		HAL_UART_Transmit(&huart1, nextLine, sizeof(nextLine), 100);
		HAL_Delay(500);
	}
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

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
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

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{
	//HAL_UART_StateTypeDef
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

  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/* FMC initialization function */
static void MX_FMC_Init(void)
{

  /* USER CODE BEGIN FMC_Init 0 */

  /* USER CODE END FMC_Init 0 */

  FMC_NAND_PCC_TimingTypeDef ComSpaceTiming = {0};
  FMC_NAND_PCC_TimingTypeDef AttSpaceTiming = {0};

  /* USER CODE BEGIN FMC_Init 1 */

  /* USER CODE END FMC_Init 1 */

  /** Perform the NAND1 memory initialization sequence
  */
  hnand1.Instance = FMC_NAND_DEVICE;
  /* hnand1.Init */
  hnand1.Init.NandBank = FMC_NAND_BANK2;
  hnand1.Init.Waitfeature = FMC_NAND_PCC_WAIT_FEATURE_ENABLE;
  hnand1.Init.MemoryDataWidth = FMC_NAND_PCC_MEM_BUS_WIDTH_8;
  hnand1.Init.EccComputation = FMC_NAND_ECC_DISABLE;
  hnand1.Init.ECCPageSize = FMC_NAND_ECC_PAGE_SIZE_256BYTE;
  hnand1.Init.TCLRSetupTime = 0;
  hnand1.Init.TARSetupTime = 0;
  /* hnand1.Config */
  hnand1.Config.PageSize = 0;
  hnand1.Config.SpareAreaSize = 0;
  hnand1.Config.BlockSize = 0;
  hnand1.Config.BlockNbr = 0;
  hnand1.Config.PlaneNbr = 0;
  hnand1.Config.PlaneSize = 0;
  hnand1.Config.ExtraCommandEnable = DISABLE;
  /* ComSpaceTiming */
  ComSpaceTiming.SetupTime = 252;
  ComSpaceTiming.WaitSetupTime = 252;
  ComSpaceTiming.HoldSetupTime = 252;
  ComSpaceTiming.HiZSetupTime = 252;
  /* AttSpaceTiming */
  AttSpaceTiming.SetupTime = 252;
  AttSpaceTiming.WaitSetupTime = 252;
  AttSpaceTiming.HoldSetupTime = 252;
  AttSpaceTiming.HiZSetupTime = 252;

  if (HAL_NAND_Init(&hnand1, &ComSpaceTiming, &AttSpaceTiming) != HAL_OK)
  {
    Error_Handler( );
  }

  /* USER CODE BEGIN FMC_Init 2 */

  /* USER CODE END FMC_Init 2 */
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
