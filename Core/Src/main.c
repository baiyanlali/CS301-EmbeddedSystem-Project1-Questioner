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
#include "dma.h"
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
// UART_HandleTypeDef huart1;
extern uint8_t rxBuffer[20];
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

uint8_t uart1_rx_buffer[2048]; // for uart1 receive buffer
uint8_t uart2_rx_buffer[2048]; // for uart2 receive buffer

extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern int esp8266_mode;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void Question();
void Judge();
void Answer(uint8_t *);

void send_message(uint8_t *msg);
void send_msg_uart1(uint8_t *, int);
void transmit1(uint8_t *msg);
void send_message_without_delay(uint8_t *msg);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
extern UART_HandleTypeDef haurt1;
extern uint8_t rxBuffer[20];
char time_l[20]; // lyu
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
  MX_DMA_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  // __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
  // HAL_UART_Receive_DMA(&huart1, (uint8_t *)uart1_rx_buffer, 2048);
  __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
  HAL_UART_Receive_DMA(&huart1, (uint8_t *)uart1_rx_buffer, 2048);

  __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
  HAL_UART_Receive_DMA(&huart2, (uint8_t *)uart2_rx_buffer, 2048);
  // HAL_UART_Receive_IT(&huart1, (uint8_t *)rxBuffer, 1);
  __HAL_TIM_CLEAR_FLAG(&htim2, TIM_SR_UIF); // lyu 清除tim 的flag�???? 否则刚开始start就会进入回调函数
  __HAL_TIM_CLEAR_FLAG(&htim3, TIM_SR_UIF); // lyu 清除tim 的flag�???? 否则刚开始start就会进入回调函数
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  // esp8266_mode = 1;
  // send_msg_uart1((uint8_t *)"SET AS SERVER\r\n", 0);
  // send_cmd("AT+CWMODE=3\r\n", 1000);
  esp8266_mode = 1;
  init_server();
  LCD_ShowString(30, 40, 200, 24, 16, "Connecting...");
  // send_msg_uart1((uint8_t *)"Init Server End\r\n", 0);
  // Question();
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
} state;

struct question
{
  int index;
  char *content;
  char *answerList;
  int answerIndex;
  int pointAward;
  int time;
};

struct question questions[4] = {
    {1, "Do you like embedded system course?\n", "0: Yes 1:No", 0, 20, 50},
    {2, "Do you like C?\n", "0: Yes 1:No", 1, 100, 50},
    {3, "Do you like Disco Elysium?\n", "0: Yes 1:No", 1, 150, 100},
    {4, "Do you like Divine Original Sin 2?\n", "0: Yes 1:No", 1, 150, 20}};

int answerIndex = 0;

struct question *q;

int point = 0;

int time_left = 0;

char *answer;
unsigned char msg[100];

void Question()
{
  state = QuestionState;

  q = &questions[answerIndex];
  POINT_COLOR = RED;
  LCD_Clear(WHITE);
  // LCD_Color_Fill(0,0,240, 320,WHITE);
  LCD_ShowString(30, 40, 200, 24, 16, "Question   Time:");
  char strs[20];
  sprintf(strs, "point: %d", q->pointAward);
  LCD_ShowString(30, 70, 200, 16, 12, strs);
  LCD_ShowString(30, 110, 200, 16, 12, q->content);
  LCD_ShowString(30, 150, 200, 16, 12, q->answerList);
  sprintf(strs, "answer: %d", q->answerIndex);
  LCD_ShowString(30, 170, 200, 16, 12, strs);
  LCD_ShowString(30, 190, 200, 16, 12, "Click any key to send the question.");
  // LCD_ShowString(30, 70, 200, 16, 16, q->content);
  POINT_COLOR = BLACK;
  HAL_UART_Transmit(&huart1, "\nEnter Question Mode\n", strlen("\nEnter Question Mode\n"), 0xffff);
  time_left = q->time;
  answerIndex++;
  answerIndex %= 4;
}

void Answer(uint8_t *ans)
{
  // switch (state)
  // {
  // case AnswerState:
  char a[100];
  // sprintf(a, "Answer is: |%s|", ans);
  // transmit1(a);
  ans = strchr(ans, ':') + 1;
  sprintf(a, "\nAnswer is: |%s|\n", ans);
  transmit1(a);
  if (atoi(ans) == q->answerIndex)
  {
    sprintf(msg, "Check right: %d %d\n", atoi(ans), q->answerIndex);
    // send_message(msg);
    HAL_UART_Transmit(&huart1, msg, strlen(msg), HAL_MAX_DELAY);
    HAL_TIM_Base_Stop_IT(&htim2);
    HAL_TIM_Base_Stop_IT(&htim3); //lyu
    // LCD_ShowString(30, 70, 200, 16, 12, "Check right!");
    point = point + q->pointAward;
    // HAL_Delay(500);
    Judge();
  }
  else
  {
    sprintf(msg, "Check wrong: %d %d\n", atoi(ans), q->answerIndex);
    // send_message(msg);
    HAL_UART_Transmit(&huart1, msg, strlen(msg), HAL_MAX_DELAY);
    LCD_Clear(RED);
    // LCD_Color_Fill(0,0,240, 320,RED);
    LCD_ShowString(30, 70, 200, 16, 12, "Check wrong!");
  }
  //   break;

  // default:
  //   break;
  // }
}

void Judge()
{

  HAL_UART_Transmit(&huart1, "Enter Judge\n", strlen("Enter Judge\n"), HAL_MAX_DELAY);
  state = AnswerState;
  char strs[64];
  sprintf(strs, "F:You get point:|%d|", point);
  LCD_ShowString(30, 200, 200, 16, 12, "Click 1 to send feedback.");
  LCD_Clear(WHITE);
  // LCD_Color_Fill(0,0,240, 320,WHITE);
  LCD_ShowString(30, 70, 200, 16, 12, strs);

  //  uint32_t delay=1008600;
  //  while(delay--){
  //
  //  }
  //  transmit1("\nStart Delay\n");
  //   HAL_Delay(500);
  //   transmit1("\nEND Delay\n");
  // HAL_TIM_Base_Stop_IT(&htim2); // lyu
  // HAL_TIM_Base_Stop_IT(&htim3); // lyu
}

void SendFeedBack()
{
  state = JudgeState;
  char strs[64];
  transmit1("\nStart to Send Feedback\n");
  sprintf(strs, "F:You get point:|%d|", point);
  LCD_ShowString(30, 200, 200, 16, 12, "Click 1 to play and 0 to reset.");
  HAL_TIM_Base_Stop_IT(&htim2); // lyu
  HAL_TIM_Base_Stop_IT(&htim3); // lyu
  send_message_without_delay(strs);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  char messages_send[1024];
  switch (GPIO_Pin)

  {
  case KEY0_Pin:
    HAL_UART_Transmit(&huart1, (uint8_t *)"Key 0 pressed\n", 20, HAL_MAX_DELAY);

    if (HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin) == GPIO_PIN_RESET)
    {

      switch (state)
      {
        // case QuestionState:

        //   sprintf(messages_send, "Q:%s|%s|%d|%d|", q->content, q->answerList, q->pointAward, q->time);
        //   send_message(messages_send);
        //   transmit1(messages_send);

        //   LCD_Clear(GREEN);
        //   // LCD_Color_Fill(0,0,240, 320,GREEN);
        //   LCD_ShowString(30, 40, 200, 24, 16, "Question   Time:");
        //   LCD_ShowString(30, 70, 200, 16, 12, msg);
        //   // time_left = 5; // lyu
        //   HAL_TIM_Base_Start_IT(&htim2);
        //   HAL_TIM_Base_Start_IT(&htim3); // lyu
        //   state = AnswerState;

        //   break;
        // case AnswerState:
        //   SendFeedBack();
        //   break;

      case AnswerState:
		  SendFeedBack();
		  break;

      case JudgeState:
        answerIndex = 0;
        state = QuestionState;
        Question();
        break;

      default:
        break;
      }
    }
    break;
  case KEY1_Pin:
    if (HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == GPIO_PIN_RESET)
    {
      HAL_UART_Transmit(&huart1, (uint8_t *)"\nKey 1 pressed\n", 16, HAL_MAX_DELAY);

      switch (state)
      {
      case QuestionState:
        sprintf(messages_send, "Q:%s|%s|%d|%d|", q->content, q->answerList, q->pointAward, q->time);
        send_message(messages_send);
        transmit1(messages_send);

        LCD_Clear(GREEN);
        // LCD_Color_Fill(0,0,240, 320,GREEN);
        LCD_ShowString(30, 40, 200, 24, 16, "Question   Time:");
        LCD_ShowString(30, 70, 200, 16, 12, msg);
        // time_left = 5; // lyu
        HAL_TIM_Base_Start_IT(&htim2);
        HAL_TIM_Base_Start_IT(&htim3); // lyu
        state = AnswerState;

        break;
      case JudgeState:
        state = QuestionState;
        Question();
        break;

      case AnswerState:
        SendFeedBack();
        break;

      default:
        break;
      }
    }
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  // lyu test
  if (htim->Instance == htim3.Instance)
  {
    time_left--;
    sprintf(time_l, "%d\n", time_left);
    char strs[20];
    sprintf(strs, "Question   Time:%d s  ", time_left);
    LCD_ShowString(30, 40, 200, 24, 16, strs);
    HAL_UART_Transmit(&huart1, (uint8_t *)time_l, strlen(time_l), HAL_MAX_DELAY);
    if (time_left == 0)
    {
      HAL_UART_Transmit(&huart1, "timer\n", strlen("timer\n"), HAL_MAX_DELAY);
      Judge();
    }
  }
  // if (htim->Instance == htim2.Instance)
  // {
  //   HAL_UART_Transmit(&huart1, "timer\n", strlen("timer\n"), HAL_MAX_DELAY);
  //   // send_message("timer\n");
  //   Judge();
  // }
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
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
