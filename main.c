/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
typedef struct {
	uint8_t x;
	uint8_t y;
}  Coordinates;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
// 0: area clear, 1: mine
uint8_t mine_field[10][10] = {{0}};
Coordinates minesweeper_position;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
// void MX_USB_HOST_Process(void);

/* USER CODE BEGIN PFP */
void GameSetup(void);
void Move_Up(void);
void Move_Down(void);
void Move_Left(void);
void Move_Right(void);
void Uncover_Field(void);
void my_FillRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
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
  JOYState_TypeDef JoyState = JOY_NONE;
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
  /* USER CODE BEGIN 2 */
  /* Initialize the LEDs */
  // BSP_LED_Init(LED_GREEN);
  // BSP_LED_Init(LED_ORANGE);
  BSP_LED_Init(LED_RED);
  // BSP_LED_Init(LED_BLUE);
  
  /* Configure the Key push-button in GPIO Mode */
  // BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);

  /*## Initialize the LCD #################################################*/
  if (BSP_LCD_Init() != LCD_OK)
  {
	  Error_Handler();
  }

  if (BSP_JOY_Init(JOY_MODE_GPIO) != IO_OK)
  {
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 95, (uint8_t *)"ERROR", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 80, (uint8_t *)"Joystick cannot be initialized", CENTER_MODE);
    Error_Handler();
  }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  GameSetup();
	  while (1)
	  {

		    JoyState = BSP_JOY_GetState();

		    switch(JoyState)
		    {
		    case JOY_UP:
		      Move_Up();
		      HAL_Delay(200);
		      break;
		    case JOY_DOWN:
			  Move_Down();
		      HAL_Delay(200);
		      break;
		    case JOY_LEFT:
			  Move_Left();
		      HAL_Delay(200);
		      break;
		    case JOY_RIGHT:
			  Move_Right();
		      HAL_Delay(200);
		      break;
		    case JOY_SEL:
		      Uncover_Field();
		      HAL_Delay(200);
			  break;
		    default:
		      break;
		    }
	  }
    /* USER CODE END WHILE */
    // MX_USB_HOST_Process();

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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV5;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.Prediv1Source = RCC_PREDIV1_SOURCE_PLL2;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  RCC_OscInitStruct.PLL2.PLL2State = RCC_PLL2_ON;
  RCC_OscInitStruct.PLL2.PLL2MUL = RCC_PLL2_MUL8;
  RCC_OscInitStruct.PLL2.HSEPrediv2Value = RCC_HSE_PREDIV2_DIV5;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_ADC
                              |RCC_PERIPHCLK_USB;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV3;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  // HAL_RCC_MCOConfig(RCC_MCO, RCC_MCO1SOURCE_HSE, RCC_MCODIV_1);
  // __HAL_RCC_PLLI2S_ENABLE();
}

/* USER CODE BEGIN 4 */
void GameSetup(void)
{
	BSP_LCD_Clear(LCD_COLOR_LIGHTGRAY);

	// Set minesweeper's position

	uint16_t backup_color = 0;
	minesweeper_position.x = 0;
	minesweeper_position.y = 0;

	// To ważne: < zamiast <= w pierwszej pętli
	for (uint16_t i = 0; i<240; i+=24)
	{
		BSP_LCD_DrawHLine(0, i, 240);
	}

	for (uint16_t i = 0; i<=240; i+=24)
	{
		BSP_LCD_DrawVLine(i, 0, 240);
	}

	// Mark minesweeper's position on the field

	backup_color = BSP_LCD_GetTextColor();
	BSP_LCD_SetTextColor(LCD_COLOR_DARKGRAY);
	my_FillRect(minesweeper_position.x*24+1, minesweeper_position.y*24+1, 23, 23);
	BSP_LCD_SetTextColor(backup_color);

}

void Move_Up(void)
{
	// Create the square
	uint16_t backup_color = 0;
	backup_color = BSP_LCD_GetTextColor();
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
	my_FillRect(minesweeper_position.x*24+1, minesweeper_position.y*24+1, 23, 23);
	BSP_LCD_SetTextColor(backup_color);

	// Change minesweeper's position
	if (minesweeper_position.y != 0)
		minesweeper_position.y--;
	else
		minesweeper_position.y = 9;
	// Mark minesweeper's new position
	backup_color = BSP_LCD_GetTextColor();
	BSP_LCD_SetTextColor(LCD_COLOR_DARKGRAY);
	my_FillRect(minesweeper_position.x*24+1, minesweeper_position.y*24+1, 23, 23);
	BSP_LCD_SetTextColor(backup_color);
}

void Move_Down(void)
{
	// Create the square
	uint16_t backup_color = 0;
	backup_color = BSP_LCD_GetTextColor();
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
	my_FillRect(minesweeper_position.x*24+1, minesweeper_position.y*24+1, 23, 23);
	BSP_LCD_SetTextColor(backup_color);

	// Change minesweeper's position
	if (minesweeper_position.y != 9)
		minesweeper_position.y++;
	else
		minesweeper_position.y = 0;
	// Mark minesweeper's new position
	backup_color = BSP_LCD_GetTextColor();
	BSP_LCD_SetTextColor(LCD_COLOR_DARKGRAY);
	my_FillRect(minesweeper_position.x*24+1, minesweeper_position.y*24+1, 23, 23);
	BSP_LCD_SetTextColor(backup_color);
}

void Move_Left(void)
{
	// Create the square
	uint16_t backup_color = 0;
	backup_color = BSP_LCD_GetTextColor();
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
	my_FillRect(minesweeper_position.x*24+1, minesweeper_position.y*24+1, 23, 23);
	BSP_LCD_SetTextColor(backup_color);

	// Change minesweeper's position
	if (minesweeper_position.x != 0)
		minesweeper_position.x--;
	else
		minesweeper_position.x = 9;
	// Mark minesweeper's new position
	backup_color = BSP_LCD_GetTextColor();
	BSP_LCD_SetTextColor(LCD_COLOR_DARKGRAY);
	my_FillRect(minesweeper_position.x*24+1, minesweeper_position.y*24+1, 23, 23);
	BSP_LCD_SetTextColor(backup_color);
}

void Move_Right(void)
{
	// Create the square
	uint16_t backup_color = 0;
	backup_color = BSP_LCD_GetTextColor();
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
	my_FillRect(minesweeper_position.x*24+1, minesweeper_position.y*24+1, 23, 23);
	BSP_LCD_SetTextColor(backup_color);

	// Change minesweeper's position
	if (minesweeper_position.x != 9)
		minesweeper_position.x++;
	else
		minesweeper_position.x = 0;
	// Mark minesweeper's new position
	backup_color = BSP_LCD_GetTextColor();
	BSP_LCD_SetTextColor(LCD_COLOR_DARKGRAY);
	my_FillRect(minesweeper_position.x*24+1, minesweeper_position.y*24+1, 23, 23);
	BSP_LCD_SetTextColor(backup_color);
}

void Uncover_Field(void)
{

}

void my_FillRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  while(Height--)
  {
    BSP_LCD_DrawHLine(Xpos, Ypos++, Width);
  }
}
/**
  * @brief Toggle Leds
  * @param  None
  * @retval None
  */
void Toggle_Leds(void)
{
  static uint8_t ticks = 0;
  
  if(ticks++ > 100)
  {
    // BSP_LED_Toggle(LED_BLUE);
    ticks = 0;
  }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  BSP_LED_On(LED_RED);
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

