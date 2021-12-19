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
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd.h"
#include <stdio.h>
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
UART_HandleTypeDef huart1;
extern uint8_t rxBuffer[20];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void Question();
void Judge();
void Answer(char*);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
extern UART_HandleTypeDef haurt1;
extern uint8_t rxBuffer[20];
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
  LCD_Init();
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&huart1, (uint8_t *)rxBuffer, 1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  Question();
  while (1)
  {
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

enum QuestionerState
{
  QuestionState,
  AnswerState,
  JudgeState
};

struct question
{
  int index;
  char *content;
  char *answerList;
  int answerIndex;
  int pointAward;
};

struct question questions[2] = {
    {1, "Do you like embedded system course?\n", "0: Yes 1:No", 0, 20},
    {2, "Do you like C?\n", "0: Yes 1:No", 1, 100}};

int answerIndex = 0;

enum QuestionerState state;

struct question *q;

int point = 0;

char *answer;
unsigned char msg[100];


void Question()
{
  state = QuestionState;

  q = &questions[answerIndex];
  POINT_COLOR = RED;
  LCD_ShowString(30, 40, 200, 24, 16, "Question   Time:5s");
  LCD_ShowString(30, 70, 200, 16, 12, q->content);
  // LCD_ShowString(30, 70, 200, 16, 16, q->content);
  POINT_COLOR = BLACK;

  answerIndex++;
  answerIndex %= 2;
}

void Answer(char* ans)
{
  // switch (state)
  // {
  // case AnswerState:
    if (atoi(ans)== q->answerIndex)
    {
      HAL_TIM_Base_Stop_IT(&htim2);
      LCD_ShowString(30, 70, 200, 16, 12, "Check right!");
      point = point + q->pointAward;
      // HAL_Delay(500);
      Judge();
    }else{
      sprintf(msg,"Check wrong: %d %d\n",atoi(ans),q->answerIndex);
      HAL_UART_Transmit(&huart1,msg,strlen(msg),HAL_MAX_DELAY);
      LCD_ShowString(30, 70, 200, 16, 12, "Check wrong!");
    }
  //   break;

  // default:
  //   break;
  // }
}

void Judge()
{
  HAL_UART_Transmit(&huart1,"Enter Judge\n",strlen("Enter Judge\n"),HAL_MAX_DELAY);
  state=JudgeState;
  char strs[64];
  sprintf(strs, "Your point: %d", point);
  LCD_ShowString(30, 70, 200, 16, 12, strs);
  LCD_ShowString(30, 200, 200, 16, 12, "Click 0 to play and 1 to reset.");
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  Judge();
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

  switch (GPIO_Pin)

  {
  case KEY0_Pin:
    if (HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin) == GPIO_PIN_RESET)
    {
      HAL_UART_Transmit(&huart1,(uint8_t*)"Key 0 pressed\n",20,HAL_MAX_DELAY);
      switch (state)
      {
      case QuestionState:
        HAL_UART_Transmit(&huart1, q->content, strlen(q->content), 0xffff);
        HAL_TIM_Base_Start_IT(&htim2);
        state = AnswerState;

        break;
      case JudgeState:
        state = QuestionState;
        Question();
        break;

      default:
        break;
      }
    }
    break;
  case KEY1_Pin:
  HAL_UART_Transmit(&huart1,(uint8_t*)"Key 1 pressed\n",20,HAL_MAX_DELAY);
    if (HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == GPIO_PIN_RESET)
    {
      switch (state)
      {
      case QuestionState:
        HAL_UART_Transmit(&huart1, q->content, strlen(q->content), 0xffff);
        HAL_TIM_Base_Start_IT(&htim2);
        state = AnswerState;
        break;
      case JudgeState:
        state = QuestionState;
        answerIndex = 0;
        Question();
        break;

      default:
        break;
      }
    }
  }
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1){
		static unsigned char uRx_Data[20] = {0};
		static unsigned char uLength = 0;
		if(rxBuffer[0] == '\n'){
			uLength = 0;
      HAL_UART_Transmit(&huart1,(uint8_t*)uRx_Data,12,HAL_MAX_DELAY);
      Answer(uRx_Data);
			for(int i = 0; i < 20; i++){
				uRx_Data[i] = '\0';
			}
			for(int i = 0; i < 20; i++){
				rxBuffer[i] = '\0';
			}
		}
		else if (rxBuffer[0] == '\r') {
			//HAL_UART_Transmit(&huart1,(uint8_t*)"BEST WISHES\n",12,HAL_MAX_DELAY);
		}
		else{
			uRx_Data[uLength] = rxBuffer[0];
			uLength++;
		}
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
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
  }

#ifdef USE_FULL_ASSERT
  /**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
  void assert_failed(uint8_t * file, uint32_t line)
  {
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
  }
#endif /* USE_FULL_ASSERT */

  /************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
