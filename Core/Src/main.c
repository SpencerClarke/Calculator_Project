/* USER CODE BEGIN Header */
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
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
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
	char input_string[256];
	int input_index;
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
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

  HD44780U_Init();
  button_matrix_init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  input_index = 0;
  input_string[0] = '\0';
  for(;;)
  {
	  char c = button_matrix_get_key();

	  if(input_index == 0)
	  {
		  HD44780U_Clear(); /* Clear the screen after displaying text. No effect on initial input */
	  }

	  if(c == 'd') /* Delete key */
	  {
		  if(input_index == 0)
		  {
			  continue;
		  }
		  HD44780U_Delete_At_Cursor();
		  input_index--;
		  input_string[input_index] = '\0';
	  }
	  else if(c == 'n') /* Enter key */
	  {
		  double retval;
		  int status;
		  char *input_ptr = &(input_string[0]);
		  status = parse_top(&input_ptr, &retval);
		  if(!status) /* Syntax error */
		  {
			  HD44780U_Clear();
			  if(error == SYNTAX)
			  {
				  HD44780U_Write_Line(0, "Syntax error");
			  }
			  else if(error == DIV_ZERO)
			  {
				  HD44780U_Write_Line(0, "Divide by zero");
			  }
			  else if(error == BAD_LN)
			  {
				  HD44780U_Write_Line(0, "Bad natural log (<=0)");
			  }
		  }
		  else
		  {
			  HD44780U_Write_Line(2, "                    ");
			  HD44780U_Write_Line(3, "                    ");
			  HD44780U_Write_Line(2, "Result:");
			  HD44780U_Move_Cursor(3, 0);
			  HD44780U_Write_Double(retval);
		  }

		  input_index = 0;
		  HD44780U_Move_Cursor(0, 0);
	  }
	  else if(c == 'p') /* pi */
	  {
		  HD44780U_Write_At_Cursor(247);
		  input_string[input_index] = 'p';
		  input_index++;
		  input_string[input_index] = '\0';

	  }
	  else if(c == 'e') /* e */
	  {
		  HD44780U_Write_At_Cursor('e');
		  input_string[input_index] = 'e';
		  input_index++;
		  input_string[input_index] = '\0';

	  }
	  else if(c == 'l') /* natural log, multi-character */
	  {
		  if(79 - input_index < 3)
		  {
			  continue;
		  }
		  HD44780U_Write_At_Cursor('l');
		  input_string[input_index] = 'l';
		  input_index++;
		  HD44780U_Write_At_Cursor('n');
		  input_string[input_index] = 'n';
		  input_index++;
		  HD44780U_Write_At_Cursor('(');
		  input_string[input_index] = '(';
		  input_index++;
		  input_string[input_index] = '\0';
	  }
	  else if(c == 'a') /* Ans, multi-character */
	  {
		  if(79 - input_index < 3)
		  {
			  continue;
		  }
		  HD44780U_Write_At_Cursor('A');
		  input_string[input_index] = 'A';
		  input_index++;
		  HD44780U_Write_At_Cursor('n');
		  input_string[input_index] = 'n';
		  input_index++;
		  HD44780U_Write_At_Cursor('s');
		  input_string[input_index] = 's';
		  input_index++;
		  input_string[input_index] = '\0';
	  }
	  else if(c == 's') /* sin, multi-character */
	  {
		  if(79 - input_index < 4)
		  {
			  continue;
		  }
		  HD44780U_Write_At_Cursor('s');
		  input_string[input_index] = 's';
		  input_index++;
		  HD44780U_Write_At_Cursor('i');
		  input_string[input_index] = 'i';
		  input_index++;
		  HD44780U_Write_At_Cursor('n');
		  input_string[input_index] = 'n';
		  input_index++;
		  HD44780U_Write_At_Cursor('(');
		  input_string[input_index] = '(';
		  input_index++;
		  input_string[input_index] = '\0';
	  }
	  else if(c == 'c') /* cos, multi-character */
	  {
		  if(79 - input_index < 4)
		  {
			  continue;
		  }
		  HD44780U_Write_At_Cursor('c');
		  input_string[input_index] = 'c';
		  input_index++;
		  HD44780U_Write_At_Cursor('o');
		  input_string[input_index] = 'o';
		  input_index++;
		  HD44780U_Write_At_Cursor('s');
		  input_string[input_index] = 's';
		  input_index++;
		  HD44780U_Write_At_Cursor('(');
		  input_string[input_index] = '(';
		  input_index++;
		  input_string[input_index] = '\0';
	  }
	  else if(c == '\0') /* Dead key, skip */
	  {
		  continue;
	  }
	  else /* Something else */
	  {
		  if(79 - input_index < 1)
		  {
			  continue;
		  }
		  HD44780U_Write_At_Cursor(c);
		  input_string[input_index] = c;
		  input_index++;
		  input_string[input_index] = '\0';
	  }
  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */
  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */
  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4294967295;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */
  /* USER CODE END TIM2_Init 2 */

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6
                          |GPIO_PIN_7|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC0 PC1 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PC2 PC3 PC7 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA1 PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA2 PA4 PA5 PA6
                           PA7 PA9 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6
                          |GPIO_PIN_7|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB3 PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_3|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB1 PB6 PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
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
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
