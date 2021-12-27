/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f1xx_it.c
  * @brief   Interrupt Service Routines.
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
#include "stm32f1xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
// #include <string.h>
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
int esp8266_mode = 1; // esp8266_mode = 1 -> server mode, esp8266_mode = 0 -> client mode
int connect_flag = 0; //connect_flag = 0 -> not connected, 1-> TCP not connected but wifi connected, 2-> connected
int sending_flag = 0; //set the sending flag, 1-> is sending, 0 -> is not sending
int link_number = -1; //表示连接序号, 0号连接可client或server连接,其他id只能用于连接远程server,-1表示没有连接
int timer_count = 0;
int connection_counter = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
/* USER CODE BEGIN EV */
// uint8_t rxBuffer[20];
extern uint8_t uart1_rx_buffer[2048]; // for uart1 receive buffer
extern uint8_t uart2_rx_buffer[2048]; // for uart2 receive buffer
/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M3 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 channel5 global interrupt.
  */
void DMA1_Channel5_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel5_IRQn 0 */

  /* USER CODE END DMA1_Channel5_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart1_rx);
  /* USER CODE BEGIN DMA1_Channel5_IRQn 1 */

  /* USER CODE END DMA1_Channel5_IRQn 1 */
}

/**
  * @brief This function handles DMA1 channel6 global interrupt.
  */
void DMA1_Channel6_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel6_IRQn 0 */

  /* USER CODE END DMA1_Channel6_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart2_rx);
  /* USER CODE BEGIN DMA1_Channel6_IRQn 1 */

  /* USER CODE END DMA1_Channel6_IRQn 1 */
}

/**
  * @brief This function handles EXTI line[9:5] interrupts.
  */
void EXTI9_5_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI9_5_IRQn 0 */

  /* USER CODE END EXTI9_5_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
  /* USER CODE BEGIN EXTI9_5_IRQn 1 */

  /* USER CODE END EXTI9_5_IRQn 1 */
}

/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */

  /* USER CODE END TIM2_IRQn 0 */
  HAL_TIM_IRQHandler(&htim2);
  /* USER CODE BEGIN TIM2_IRQn 1 */

  /* USER CODE END TIM2_IRQn 1 */
}

/**
  * @brief This function handles TIM3 global interrupt.
  */
void TIM3_IRQHandler(void)
{
  /* USER CODE BEGIN TIM3_IRQn 0 */

  /* USER CODE END TIM3_IRQn 0 */
  HAL_TIM_IRQHandler(&htim3);
  /* USER CODE BEGIN TIM3_IRQn 1 */

  /* USER CODE END TIM3_IRQn 1 */
}

/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */

  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */
  HAL_UART_RxCpltCallback(&huart1);
	HAL_UART_Receive_DMA(&huart1, (uint8_t*) uart1_rx_buffer, 2048);
  // HAL_UART_Receive_IT(&huart1,(uint8_t*)rxBuffer,1);
  /* USER CODE END USART1_IRQn 1 */
}

/**
  * @brief This function handles USART2 global interrupt.
  */
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */

  /* USER CODE END USART2_IRQn 0 */
  HAL_UART_IRQHandler(&huart2);
  /* USER CODE BEGIN USART2_IRQn 1 */
  HAL_UART_RxCpltCallback(&huart2);
	HAL_UART_Receive_DMA(&huart2, (uint8_t*) uart2_rx_buffer, 2048);
  /* USER CODE END USART2_IRQn 1 */
}

/**
  * @brief This function handles EXTI line[15:10] interrupts.
  */
void EXTI15_10_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI15_10_IRQn 0 */

  /* USER CODE END EXTI15_10_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
  /* USER CODE BEGIN EXTI15_10_IRQn 1 */

  /* USER CODE END EXTI15_10_IRQn 1 */
}

/* USER CODE BEGIN 1 */

void transmit1(uint8_t* msg){
  HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), 0xffff);
}

void send_msg_uart1(uint8_t *msg, int delay_time)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), 0xffff);
  HAL_Delay(delay_time);
}

void send_cmd(uint8_t *cmd, int delay_time)
{
  char command[100];
  sprintf(command,"Start send command:%s\n",cmd);
  // send_msg_uart1(command,20);
  HAL_UART_Transmit(&huart2, (uint8_t *)cmd, strlen(cmd), 0xffff);
  // send_msg_uart1("Start send command END\n",20);
  // HAL_UART_Transmit(&huart1, (uint8_t *)cmd, strlen(cmd), 0xffff);
  HAL_Delay(delay_time);
}

void send_cmd_without_delay(uint8_t *cmd)
{
  char command[100];
  sprintf(command,"Start send command:\n%s\n",cmd);
  transmit1(command);
  HAL_UART_Transmit(&huart2, (uint8_t *)cmd, strlen(cmd), 0xffff);
  transmit1("Start send command END\n");
  // HAL_UART_Transmit(&huart1, (uint8_t *)cmd, strlen(cmd), 0xffff);
}


void send_message_without_delay(uint8_t* msg){
  uint8_t activate[100];
  sprintf(activate, "AT+CIPSEND=%d,%d\r\n", link_number, strlen(msg));
  // send_cmd((uint8_t *)activate, 0);
  send_cmd_without_delay((uint8_t *)activate);
  // transmit1("\n>>>>>SENDING<<<<<\n");
  // transmit1(msg);
  // transmit1("\n>>>>>SENDING END<<<<<\n");
  // send_cmd(msg, 0);
  send_cmd_without_delay(msg);
}

void send_message(uint8_t *msg)
{
  HAL_Delay(400);
  uint8_t activate[100];
  sprintf(activate, "AT+CIPSEND=%d,%d\r\n", link_number, strlen(msg));
  send_cmd((uint8_t *)activate, 2000);
  send_msg_uart1(msg, 500);
  // printOut(msg, strlen(msg), 1, connect_flag);
  send_cmd(msg, 1000);
}

void connect_to_wifi() {
	init_var();
	send_msg_uart1((uint8_t*) "start connect to wifi\r\n", 2000);
	send_cmd("AT+RST\r\n", 5000);
	send_cmd("AT+CWMODE=1\r\n", 2000);
	send_cmd("AT+RST\r\n", 5000);
	send_cmd("AT+CWJAP=\"SUSTC-WIFI-FAKE\",\"987654321\"\r\n", 10000);
	send_cmd("AT+CIPMUX=1\r\n", 5000);
	send_cmd("AT+CIPSERVER=1\r\n", 5000);
}

void init_var()
{
  connect_flag = 0;
  sending_flag = 0;
  link_number = -1;
  connection_counter = 0;
  timer_count = 0;
  // StateChange(connect_flag);
  HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
}

void init_server()
{
  init_var();
  send_msg_uart1((uint8_t *)"start initialize server\r\n", 0);
  send_cmd("AT+CWMODE=3\r\n", 2000);
  send_cmd("AT+RST\r\n", 5000);
  send_cmd("AT+CWSAP=\"SUSTC-WIFI-FAKE\",\"987654321\",1,0,4,0\r\n", 2000);
  
  // send_cmd("AT+CWSAP=\"SUSTC-WIFI-FAKE\",\"987654321\",5,3,4,0\r\n", 2000);
  send_cmd("AT+CIPMUX=1\r\n", 2000);
  send_cmd("AT+CIPSERVER=1,8089\r\n", 2000);
  send_msg_uart1((uint8_t *)"end initialize server\r\n", 0);
}

// void send_message(uint8_t *msg) {
// 	HAL_Delay(400);
// 	uint8_t activate[100];
// 	sprintf(activate, "AT+CIPSEND=%d,%d\r\n", link_number, strlen(msg));
// 	send_cmd((uint8_t*) activate, 2000);
// 	send_msg_uart1(msg, 500);
// 	printOut(msg, strlen(msg), 1, connect_flag);
// 	send_cmd(msg, 1000);
// }

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  // send_msg_uart1("MSG RECIEVED\n", 1);
  if (huart->Instance == USART1)
  {

    if (RESET != __HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE)) {
			__HAL_UART_CLEAR_IDLEFLAG(&huart1);
			HAL_UART_DMAStop(&huart1);
			uint8_t data_length = 2048 - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);
			send_msg_uart1((uint8_t*) "enter usart receive function\n", 0);
			if (link_number != -1 && uart1_rx_buffer[0] == '>'
					&& connect_flag == 2) {
				sending_flag = 1;
				HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET); //1
				HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET); //1
				send_message(uart1_rx_buffer + 1);
				sending_flag = 0;
				HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET); //0
				HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET); //1
			} else if (strcmp(uart1_rx_buffer, "server\r\n") == 0) {
				esp8266_mode = 1;
				send_msg_uart1((uint8_t*) "SET AS SERVER\r\n", 0);
				send_cmd("AT+CWMODE=3\r\n", 1000);
			} else if (strcmp(uart1_rx_buffer, "client\r\n") == 0) {
				esp8266_mode = 0;
				send_msg_uart1((uint8_t*) "SET AS CLIENT\r\n", 0);
				send_cmd("AT+CWJAP?\r\n", 1000);
				send_cmd("AT+CIFSR\r\n", 1000);
			} else if (strcmp(uart1_rx_buffer, "start\r\n") == 0
					&& esp8266_mode == 1) {
				// reset();
				init_server();
			} else if (strcmp(uart1_rx_buffer, "connect to wifi\r\n") == 0
					&& esp8266_mode == 0) {
				// reset();
				connect_to_wifi();
			} else if (strcmp(uart1_rx_buffer, "connect to server\r\n") == 0
					&& esp8266_mode == 0) {
				send_cmd((uint8_t*) "AT+CIPSTART=0,\"TCP\",\"192.168.4.1\",8089\r\n", 0);
			} else if (strcmp(uart1_rx_buffer, "close connection\r\n") == 0) {
				send_cmd((uint8_t*) "AT+CIPCLOSE=0\r\n", 2000);
				init_var();
			} else if (strncmp(uart1_rx_buffer, "quit wifi\r\n") == 0
					&& esp8266_mode == 0) {
				send_cmd("AT+CWQAP\r\n", 1000);
				init_var();
			} else if (strncmp(uart1_rx_buffer, "ip status\r\n") == 0) {
				send_cmd("AT+CIPSTATUS\r\n", 1000);
				send_cmd("AT+CIFSR\r\n", 1000);
			} else if (strncmp(uart1_rx_buffer, "wifi status\r\n") == 0) {
//				send_cmd("AT+CWSAP?\r\n", 1000);
				send_cmd("AT+CWLIF\r\n", 1000);
				send_cmd("AT+CIPSTATUS\r\n", 1000);
			} else {
				HAL_UART_Transmit(&huart2, (uint8_t*) uart1_rx_buffer,
						data_length, 0xffff);
            HAL_UART_Transmit(&huart1, (uint8_t*) uart1_rx_buffer,
						data_length, 0xffff);
			}
			memset(uart1_rx_buffer, 0, data_length);
			data_length = 0;
			HAL_UART_Receive_DMA(&huart1, (uint8_t*) uart1_rx_buffer, 2048);
		}
    // // send_msg_uart1("USART1 RECIEVED\n", 1);
    // // uint8_t data_length = 2048 - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);
    // static unsigned char uRx_Data[20] = {0};
    // static unsigned char uLength = 0;
    // if (rxBuffer[0] == '\n')
    // {
    //   uLength = 0;
    //   HAL_UART_Transmit(&huart1, (uint8_t *)uRx_Data, 12, HAL_MAX_DELAY);
    //   Answer(uRx_Data);
    //   for (int i = 0; i < 20; i++)
    //   {
    //     uRx_Data[i] = '\0';
    //   }
    //   for (int i = 0; i < 20; i++)
    //   {
    //     rxBuffer[i] = '\0';
    //   }
    // }
    // else if (rxBuffer[0] == '\r')
    // {
    //   //HAL_UART_Transmit(&huart1,(uint8_t*)"BEST WISHES\n",12,HAL_MAX_DELAY);
    // }
    // else
    // {
    //   uRx_Data[uLength] = rxBuffer[0];
    //   uLength++;
    // }
  }
  else if (huart->Instance == USART2)
  {
    if (RESET != __HAL_UART_GET_FLAG(&huart2, UART_FLAG_IDLE)) {
			__HAL_UART_CLEAR_IDLEFLAG(&huart2);
			HAL_UART_DMAStop(&huart2);
			uint8_t data_length = 2048 - __HAL_DMA_GET_COUNTER(&hdma_usart2_rx);
			if (strncmp(uart2_rx_buffer, "WIFI CONNECTED\r\n", 16) == 0
					&& connect_flag == 0) {
				connect_flag = 1;
				HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET); //0
				HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET); //1
				link_number = 0;
			} else if (strncmp(uart2_rx_buffer, "WIFI GOT IP\r\n", 13) == 0) { // client 端会收到这个消息, 这里变成连接状态
				connect_flag = 1;
				HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET); //0
				HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET); //1
				link_number = 0;
			} else if (connect_flag == 1
					&& strncmp(uart2_rx_buffer, (uint8_t*) "+", 1) == 0) {
				if (strstr(uart2_rx_buffer, "+timeout") != NULL) {
					if (connection_counter > 2) {
						send_msg_uart1((uint8_t*) "connection closed\r\n", 100);
						send_cmd((uint8_t*) "AT+CWQAP\r\n", 2000);
						init_var();
						connection_counter = 0;
						// StateChange(0);
					} else {
						send_msg_uart1((uint8_t*) "connection disturbance\r\n",
								100);
						++connection_counter;
					}
				} else {
					connection_counter = 0;
//                        send_msg_uart1((uint8_t *) "connection alive\r\n", 100);
				}
			} else if (connect_flag == 2) {
				if (strcmp(uart2_rx_buffer, "WIFI DISCONNECT\r\n", 16) == 0) { //client 端会收到这个消息
					init_var();
					HAL_UART_Transmit(&huart1, (uint8_t*) uart2_rx_buffer,
							data_length, 0xffff);
				} else if (strncmp(uart2_rx_buffer, "\r\n+IPD,", 7) == 0) {
					HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET); //1
					HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET); //1
					// HAL_UART_Transmit(&huart1, (uint8_t*) uart2_rx_buffer,
					// 		data_length, 0xffff);
          // send_msg_uart1("Start Recieve\n",10);
          // send_msg_uart1(uart2_rx_buffer,10);
          // send_msg_uart1("End Recieve\n",10);

          HAL_UART_Transmit(&huart1, (uint8_t *)"Start Recieve\n", strlen("Start Recieve\n"), 0xffff);
          HAL_UART_Transmit(&huart1, (uint8_t *)uart2_rx_buffer, strlen(uart2_rx_buffer), 0xffff);
          HAL_UART_Transmit(&huart1, (uint8_t *)"END Recieve\n", strlen("END Recieve\n"), 0xffff);
              //TODO: 在这里接收
					// char * idx = strchr((char*) uart2_rx_buffer, ':') + 1;
          // send_msg_uart1(idx,0);
          Answer(uart2_rx_buffer);
					// printOut(uart2_rx_buffer + 1 + idx, data_length - idx, 2,
					// 		connect_flag);
				} else if (strncmp(uart2_rx_buffer, (uint8_t*) "SEND FAIL", 9)
						== 0) {
					init_var();
					HAL_UART_Transmit(&huart1, (uint8_t*) uart2_rx_buffer,
							data_length, 0xffff);
				} else if (strncmp(uart2_rx_buffer, (uint8_t*) "0,CLOSE OK", 10)
						== 0) {
					connect_flag = 1;
					sending_flag = 0;
					link_number = -1;
					connection_counter = 0;
					timer_count = 0;
					HAL_UART_Transmit(&huart1, (uint8_t*) uart2_rx_buffer,
							data_length, 0xffff);
				}
			} else if (strncmp(uart2_rx_buffer, "0,CONNECT\r\n", 11) == 0) {
				//server 端要设置
				link_number = 0;
				connect_flag = 2;
				// StateChange(connect_flag);
				HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET); //0
				HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET); //1
				HAL_UART_Transmit(&huart1, (uint8_t*) "Connection on id 0\r\n",
						20, 0xffff);
        Question();
			} else if (strncmp(uart2_rx_buffer, "0,CONNECT FAIL\r\n", 16) == 0 ||
			strstr(uart2_rx_buffer, "0,CLOSED\r\n") != NULL) {
				init_var();
				HAL_UART_Transmit(&huart1,
						(uint8_t*) "Close connection on id 0\r\n", 26, 0xffff);
			} else if (strncmp(uart2_rx_buffer, "0,CLOSED\r\n", 10) == 0) {
				connect_flag = 1;
				sending_flag = 0;
				link_number = -1;
				connection_counter = 0;
				timer_count = 0;
				HAL_UART_Transmit(&huart1,
						(uint8_t*) "connection closed on id 0\r\n", 26, 0xffff);
			} else {
				uint8_t msg[15];
				sprintf(msg, "data length=%d\r\n", data_length);
				HAL_UART_Transmit(&huart1, (uint8_t *) msg, strlen(msg), 0xffff);
				// send to uart1 again, to show on the usart1 display
				HAL_UART_Transmit(&huart1, (uint8_t*) uart2_rx_buffer,
						data_length, 0xffff);
			}
			memset(uart2_rx_buffer, 0, data_length);
			data_length = 0;
			HAL_UART_Receive_DMA(&huart2, (uint8_t*) uart2_rx_buffer, 2048);
		}
	
  }else{
    send_msg_uart1("USART UNKNOWN RECIEVED\n", 1);
  }
}


/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
