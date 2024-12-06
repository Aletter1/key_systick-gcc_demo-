#include "exti.h"

// 声明消息队列
QueueHandle_t key0Queue;
QueueHandle_t key1Queue;
QueueHandle_t key2Queue;
// 功能:初始化FreeRTOS环境中EXTI需要的消息队列
// 返回值与参数:无
// 备注:无
void EXTI_Queue_Init(void)
{
#if key_exti
  key0Queue = xQueueCreate(10, sizeof(uint8_t));
  key1Queue = xQueueCreate(10, sizeof(uint8_t));
  key2Queue = xQueueCreate(10, sizeof(uint8_t));
#endif
}
// 功能:初始化EXTI
// 返回值与参数:无
// 备注:
/*设置抢断优先级，HAL库设置中断分组为NVIC_PRIORITYGROUP_4,
即4位抢占优先级，0位子优先级4位抢占优先级，0位子优先级*/
void MX_EXTI_Init(void)
{
#if key_exti
  // 设置中断优先级
  HAL_NVIC_SetPriority(KEY0_INT_IRQn, 6, 0); /* 抢占6，子优先级2 */
  HAL_NVIC_EnableIRQ(KEY0_INT_IRQn);         /* 使能中断线1 */

  HAL_NVIC_SetPriority(KEY1_INT_IRQn, 6, 1); /* 抢占6，子优先级2 */
  HAL_NVIC_EnableIRQ(KEY1_INT_IRQn);         /* 使能中断线15 */

  HAL_NVIC_SetPriority(KEY2_INT_IRQn, 6, 2); /* 抢占6，子优先级2 */
  HAL_NVIC_EnableIRQ(KEY2_INT_IRQn);         /* 使能中断线15 */
#endif                                       // 0
}

void EXTI2_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(KEY0_GPIO_PIN); /* 调用中断处理公用函数 清除KEY0所在中断线 的中断标志位 */
  __HAL_GPIO_EXTI_CLEAR_IT(KEY0_GPIO_PIN);
}
void EXTI3_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(KEY1_GPIO_PIN); /* 调用中断处理公用函数 清除KEY0所在中断线 的中断标志位 */
  __HAL_GPIO_EXTI_CLEAR_IT(KEY1_GPIO_PIN);
}
void EXTI4_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(KEY2_GPIO_PIN); /* 调用中断处理公用函数 清除KEY0所在中断线 的中断标志位 */
  __HAL_GPIO_EXTI_CLEAR_IT(KEY2_GPIO_PIN);
}
/**
 * @brief       中断服务程序中需要做的事情
                在HAL库中所有的外部中断服务函数都会调用此函数
 * @param       GPIO_Pin:中断引脚号
 * @retval      无
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  uint8_t key0_level;
  uint8_t key1_level;
  uint8_t key2_level;
  static uint32_t last_time = 0;
  uint32_t current_time     = HAL_GetTick();
  if ((current_time - last_time) > 20) {
    last_time = current_time;
    switch (GPIO_Pin) {
      case KEY0_GPIO_PIN:
        key0_level = HAL_GPIO_ReadPin(KEY0_GPIO_PORT, KEY0_GPIO_PIN);
        if (key0_level == GPIO_PIN_RESET) {
          xQueueSendFromISR(key0Queue, (void *)1, NULL);
        }
        break;
      case KEY1_GPIO_PIN:
        key1_level = HAL_GPIO_ReadPin(KEY1_GPIO_PORT, KEY1_GPIO_PIN);
        if (key1_level == GPIO_PIN_RESET) {
          xQueueSendFromISR(key1Queue, (void *)1, NULL);
        }
        break;
      case KEY2_GPIO_PIN:
        key2_level = HAL_GPIO_ReadPin(KEY2_GPIO_PORT, KEY2_GPIO_PIN);
        if (key2_level == GPIO_PIN_RESET) {
          xQueueSendFromISR(key2Queue, (void *)1, NULL);
        }
        break;
      default:
        break;
    }
  }
}
