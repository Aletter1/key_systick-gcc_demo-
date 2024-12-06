/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    usart.h
 * @brief   This file contains all the function prototypes for
 *          the usart.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stdio.h"
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#define uart_max_num 256
struct UART_Data {
  UART_HandleTypeDef *uart_handle;
  DMA_HandleTypeDef *dma_tx_handle;
  DMA_HandleTypeDef *dma_rx_handle;
  SemaphoreHandle_t xTxSem;
  QueueHandle_t xRxQueue;
  uint8_t rxdatas[uart_max_num / 8];
  uint16_t rx_cnt;
};

struct UART_Device {
  void (*Init)(struct UART_Device *pDev);
  BaseType_t (*send)(struct UART_Device *pDev, uint8_t *buf, uint16_t len, TickType_t timeout);
  BaseType_t (*recv)(struct UART_Device *pDev, uint8_t *rxbuff, TickType_t timeout);
  struct UART_Data *priv_data;
};

/**********************UART1***********************/
#define uart1_buf_len uart_max_num
#define uart1_tx_dma  1
#define uart1_rx_dma  1
extern UART_HandleTypeDef huart1;
extern struct UART_Device stm32_uart1;
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_USART_UART_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */
