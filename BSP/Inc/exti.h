#ifndef _EXTI_H
#define _EXTI_H
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "key.h"
#include "led.h"
#endif // !_EXTI_H

#define KEY0_INT_IRQn       EXTI2_IRQn
#define KEY0_INT_IRQHandler EXTI2_IRQHandler
#define KEY1_INT_IRQn       EXTI3_IRQn
#define KEY1_INT_IRQHandler EXTI3_IRQHandler
#define KEY2_INT_IRQn       EXTI4_IRQn
#define KEY2_INT_IRQHandler EXTI4_IRQHandler

extern QueueHandle_t key0Queue;
extern QueueHandle_t key1Queue;
extern QueueHandle_t key2Queue;

void MX_EXTI_Init(void);
void EXTI_Queue_Init(void);
