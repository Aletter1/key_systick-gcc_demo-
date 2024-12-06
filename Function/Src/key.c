#include "key.h"
void KEY_GPIO_Init(void)
{
  /* GPIO配置参数存储变量 */
  GPIO_InitTypeDef gpio_init_struct;
  // 初始化时钟
  KEY0_GPIO_CLK_ENABLE();
  KEY1_GPIO_CLK_ENABLE();
  KEY2_GPIO_CLK_ENABLE();

  gpio_init_struct.Pin   = KEY0_GPIO_PIN;
  gpio_init_struct.Mode  = KEY0_GPIO_MODE;
  gpio_init_struct.Pull  = GPIO_PULLUP;
  gpio_init_struct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(KEY0_GPIO_PORT, &gpio_init_struct); /* KEY0引脚模式设置,上拉输入 */

  gpio_init_struct.Pin   = KEY1_GPIO_PIN;           /* KEY1引脚 */
  gpio_init_struct.Mode  = KEY1_GPIO_MODE;          /* 输入 */
  gpio_init_struct.Pull  = GPIO_PULLUP;             /* 上拉 */
  gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;    /* 高速 */
  HAL_GPIO_Init(KEY1_GPIO_PORT, &gpio_init_struct); /* KEY1引脚模式设置,上拉输入 */

  gpio_init_struct.Pin   = KEY2_GPIO_PIN;           /* KEY2引脚 */
  gpio_init_struct.Mode  = KEY2_GPIO_MODE;          /* 输入 */
  gpio_init_struct.Pull  = GPIO_PULLUP;             /* 上拉 */
  gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;    /* 高速 */
  HAL_GPIO_Init(KEY2_GPIO_PORT, &gpio_init_struct); /* KEY2引脚模式设置,上拉输入 */
}
uint8_t key_scan(uint8_t mode)
{
  static uint8_t key_up = 1; /* 按键按松开标志 */
  uint8_t keyval        = 0;
  if (mode) key_up = 1; /* 支持连按 */

  if (key_up && (KEY0 == 0 || KEY1 == 0 || KEY2 == 0)) /* 按键松开标志为1, 且有任意一个按键按下了 */
  {
    HAL_Delay(10); /* 去抖动 */
    key_up = 0;
    if (KEY0 == 0) keyval = KEY0_PRES;
    if (KEY1 == 0) keyval = KEY1_PRES;
    if (KEY2 == 0) keyval = KEY2_PRES;
  } else if (KEY0 == 1 && KEY1 == 1 && KEY2 == 1) /* 没有任何按键按下, 标记按键松开 */
  {
    key_up = 1;
  }
  return keyval; /* 返回键值 */
}
