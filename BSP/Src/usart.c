/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    usart.c
 * @brief   This file provides code for the configuration
 *          of the USART instances.
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
#include "usart.h"
/******************************************************************************************/
/* 加入以下代码, 支持printf函数, 而不需要选择use MicroLIB */

#if 1
#if (__ARMCC_VERSION >= 6010050)           /* 使用AC6编译器时 */
__asm(".global __use_no_semihosting\n\t"); /* 声明不使用半主机模式 */
__asm(".global __ARM_use_no_argv \n\t");   /* AC6下需要声明main函数为无参数格式，否则部分例程可能出现半主机模式 */

#else
/* 使用AC5编译器时, 要在这里定义__FILE 和 不使用半主机模式 */
// #pragma import(__use_no_semihosting)

struct __FILE
{
  int handle;
  /* Whatever you require here. If the only file you are using is */
  /* standard output using printf() for debugging, no file handling */
  /* is required. */
};

#endif

/* 不使用半主机模式，至少需要重定义_ttywrch\_sys_exit\_sys_command_string函数,以同时兼容AC6和AC5模式 */
int _ttywrch(int ch)
{
  ch = ch;
  return ch;
}

/* 定义_sys_exit()以避免使用半主机模式 */
void _sys_exit(int x)
{
  x = x;
}

char *_sys_command_string(char *cmd, int len)
{
  return NULL;
}

/* FILE 在 stdio.h里面定义. */
FILE __stdout;
#ifdef Keil_MDK
/* 重定义fputc函数, printf函数最终会通过调用fputc输出字符串到串口 */
int fputc(int ch, FILE *f)
{
  while ((USART1->SR & 0X40) == 0)
    ; /* 等待上一个字符发送完成 */

  USART1->DR = (uint8_t)ch; /* 将要发送的字符 ch 写入到DR寄存器 */
  return ch;
}
#endif
#ifdef __GNUC__
int _write(int fd, char *ptr, int len)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)ptr, len, 0xFFFF);
  return len;
}
#endif
#endif
/*********************************USER Global variables BEGIN***************************************/
/********UART1 Global variables********/

UART_HandleTypeDef huart1;
DMA_HandleTypeDef dma_usart1_tx;
DMA_HandleTypeDef dma_usart1_rx;
struct UART_Device stm32_uart1;
/*********************************USER Global variables END*****************************************/

/* HAL USART function */
void MX_USART_UART_Init(void)
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
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */
  /* USER CODE END USART1_Init 2 */
}
void HAL_UART_MspInit(UART_HandleTypeDef *uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if (uartHandle->Instance == USART1)
  {
    /* USER CODE BEGIN USART1_MspInit 0 */

    /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
#if uart1_tx_dma
    /* USART1_TX Init */
    dma_usart1_tx.Instance = DMA2_Stream7;
    dma_usart1_tx.Init.Channel = DMA_CHANNEL_4;
    dma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    dma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    dma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
    dma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    dma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    dma_usart1_tx.Init.Mode = DMA_NORMAL;
    dma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
    dma_usart1_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&dma_usart1_tx) != HAL_OK)
    {
      Error_Handler();
    }
    __HAL_LINKDMA(uartHandle, hdmatx, dma_usart1_tx);
#endif
#if uart1_rx_dma
    /* USART1_RX Init */
    dma_usart1_rx.Instance = DMA2_Stream2;
    dma_usart1_rx.Init.Channel = DMA_CHANNEL_4;
    dma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    dma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    dma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
    dma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    dma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    dma_usart1_rx.Init.Mode = DMA_NORMAL;
    dma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
    dma_usart1_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&dma_usart1_rx) != HAL_OK)
    {
      Error_Handler();
    }
    __HAL_LINKDMA(uartHandle, hdmarx, dma_usart1_rx);
#endif
    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);

    /* USER CODE BEGIN USART1_MspInit 1 */

    /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *uartHandle)
{

  if (uartHandle->Instance == USART1)
  {
    /* USER CODE BEGIN USART1_MspDeInit 0 */

    /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6 | GPIO_PIN_7);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
    /* USER CODE BEGIN USART1_MspDeInit 1 */

    /* USER CODE END USART1_MspDeInit 1 */
  }
}
struct UART_Device *cur_uart1_dev;
/* USER CODE BEGIN 1 */
// UART 发送中断回调函数
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  struct UART_Data *data;
  if (huart == &huart1)
  {
    data = cur_uart1_dev->priv_data;
    /* 释放信号量 */
    xSemaphoreGiveFromISR(data->xTxSem, NULL);
  }
}
// UART 接收中断回调函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  struct UART_Data *pData;
  int len = huart->RxXferSize - huart->RxXferCount;
  if (huart->Instance == USART1)
  {
    pData = cur_uart1_dev->priv_data;
  }
  if (pData->dma_rx_handle)
  {
    for (int i = 0; i < len; i++)
    {
      xQueueSendFromISR(pData->xRxQueue, pData->rxdatas, NULL);
      HAL_UARTEx_ReceiveToIdle_DMA(&huart1, pData->rxdatas, huart->RxXferSize);
    }
  }
  else
  {
    xQueueSendFromISR(pData->xRxQueue, pData->rxdatas, NULL);
    HAL_UART_Receive_IT(&huart1, pData->rxdatas, 1);
  }
}
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
  struct UART_Data *pData;
  int len = Size;
  if (huart == &huart1)
  {
    pData = cur_uart1_dev->priv_data;
  }
  for (int i = 0; i < len; i++)
  {
    xQueueSendFromISR(pData->xRxQueue, pData->rxdatas, NULL);
    HAL_UARTEx_ReceiveToIdle_DMA(&huart1, pData->rxdatas, huart->RxXferSize);
  }
}
/*********************************HAL funtion CODE END*************************************************/

/*********************************USER CODE BEGIN 1****************************************************/
void stm32_uartx_init(struct UART_Device *pDev)
{
  cur_uart1_dev = pDev;
  struct UART_Data *pData = cur_uart1_dev->priv_data;
  pData->xRxQueue = xQueueCreate(uart_max_num, 1);
  pData->xTxSem = xSemaphoreCreateBinary();
  /*开启第一次接收数据*/
  HAL_UART_Receive_IT(pData->uart_handle, &pData->rxdatas[0], 1);
}

BaseType_t stm32_uartx_send(struct UART_Device *pDev, uint8_t *buf, uint16_t len, TickType_t timeout)
{
  struct UART_Data *pData = pDev->priv_data;
  if (pData->dma_tx_handle)
  {
    HAL_UART_Transmit_DMA(pData->uart_handle, buf, len);
  }
  else
  {
    HAL_UART_Transmit_IT(pData->uart_handle, buf, len);
  }
  if (pdTRUE == xSemaphoreTake(pData->xTxSem, timeout))
    return 1;
  else
    return 0;
}
/* 队列接收数据，把数据放到缓冲区里*/
BaseType_t stm32_uartx_recv(struct UART_Device *pDev, uint8_t *rxbuff, TickType_t timeout)
{
  struct UART_Data *pData = pDev->priv_data;
  /* 读取队列得到数据, 问题:谁写队列?中断:写队列 */
  if (pdTRUE == xQueueReceive(pData->xRxQueue, &rxbuff[pData->rx_cnt], timeout))
  {
    pData->rx_cnt += 1;
    return pdTRUE;
  }
  else
    return pdFALSE;
}

/*********************************USER CODE END 1******************************************************/

/***** USER UART INIT ******/
static struct UART_Data uart1_data =
    {
        .uart_handle = &huart1,
#if uart1_tx_dma
        .dma_tx_handle = &dma_usart1_tx,
#endif
#if uart1_rx_dma
        .dma_rx_handle = &dma_usart1_rx,
#endif
        //&UART1_SemHandle,
        //&UART1_QueueHandle,
        //&uart1_rx_data,
        // uart1_rx_buf,
        //&uart1_rx_cnt,
};
struct UART_Device stm32_uart1 =
    {
        stm32_uartx_init,
        stm32_uartx_send,
        stm32_uartx_recv,
        &uart1_data};