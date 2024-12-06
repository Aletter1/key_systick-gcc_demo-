/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER BSP CODE BEGIN Includes */
#include "gpio.h"
#include "usart.h"
#include "exti.h"
/* USER Function CODE BEGIN Includes */
#include "led.h"
#include "key.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define START_TASK_PRIO 1
#define START_STK_SIZE  128
TaskHandle_t StartTask_Handler;
void start_task(void *pvParameters);

#define TASK1_PRIO     5
#define TASK1_STK_SIZE 512
TaskHandle_t Task1Task_Handler;
void task1(void *pvParameters);

#define TASK2_PRIO     5
#define TASK2_STK_SIZE 128
TaskHandle_t Task2Task_Handler;
void task2(void *pvParameters);

#define TASK3_PRIO     5
#define TASK3_STK_SIZE 128
TaskHandle_t Task3Task_Handler;
void task3(void *pvParameters);
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
    .name       = "defaultTask",
    .stack_size = 128 * 4,
    .priority   = (osPriority_t)osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void)
{
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  xTaskCreate((TaskFunction_t)start_task,
              (const char *)"start_task",
              (uint16_t)START_STK_SIZE,
              (void *)NULL,
              (UBaseType_t)START_TASK_PRIO,
              (TaskHandle_t *)&StartTask_Handler);
  vTaskStartScheduler();

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */
}

/* USER CODE BEGIN Header_StartDefaultTask */
void start_task(void *pvParameters)
{
  taskENTER_CRITICAL();
  xTaskCreate((TaskFunction_t)task1,
              (const char *)"task1",
              (uint16_t)TASK1_STK_SIZE,
              (void *)NULL,
              (UBaseType_t)TASK1_PRIO,
              (TaskHandle_t *)&Task1Task_Handler);
  xTaskCreate((TaskFunction_t)task2,
              (const char *)"task2",
              (uint16_t)TASK2_STK_SIZE,
              (void *)NULL,
              (UBaseType_t)TASK2_PRIO,
              (TaskHandle_t *)&Task2Task_Handler);
  xTaskCreate((TaskFunction_t)task3,
              (const char *)"task3",
              (uint16_t)TASK3_STK_SIZE,
              (void *)NULL,
              (UBaseType_t)TASK3_PRIO,
              (TaskHandle_t *)&Task3Task_Handler);
  EXTI_Queue_Init();
  vTaskDelete(StartTask_Handler);
  taskEXIT_CRITICAL();
}
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void task1(void *pvParameters)
{
  while (1) {

    vTaskDelay(500);
  }
}

void task2(void *pvParameters)
{
  printf("task2 init ok\r\n");
  struct UART_Device *UART_pDev = &stm32_uart1;
  struct UART_Data *uart_data   = UART_pDev->priv_data;
  uint8_t uart_rxbuf[256];
  UART_pDev->Init(UART_pDev);
  UART_pDev->send(UART_pDev, "uart1 init ok\r\n", 16, 20);
  while (1) {
    if (pdTRUE == UART_pDev->recv(UART_pDev, uart_rxbuf, 10)) {
      printf("cnt: %d , data: %c\r\n", uart_data->rx_cnt, uart_rxbuf[uart_data->rx_cnt - 1]);
    } else if (uart_data->rx_cnt > 0) {
      // UART_pDev->send(UART_pDev, "recvive finish, recv buf:", 26, 1);
      // UART_pDev->send(UART_pDev, uart_rxbuf, uart_data->rx_cnt, 20);
      uart_data->rx_cnt = 0;
      memset(uart_rxbuf, 0, uart1_buf_len);
    }
  }
}

void task3(void *pvParameters)
{
  uint8_t key0_vaule;
  uint8_t key1_vaule;
  uint8_t key2_vaule;
  while (1) {
#if key_exti
    if (xQueueReceive(key0Queue, &key0_vaule, 100)) {
      printf("key0\r\n");
    }
    if (xQueueReceive(key1Queue, &key1_vaule, 100)) {
      printf("key1\r\n");
    }
    if (xQueueReceive(key2Queue, &key2_vaule, 100)) {
      printf("key2\r\n");
    }
#else
    key0_vaule = key_scan(1);
    if (key0_vaule) {
      printf("key0_vaule:%d\r\n", key0_vaule);
      key0_vaule = 0;
    } else
#endif
    vTaskDelay(150);
  }
}
/* USER CODE END Application */
