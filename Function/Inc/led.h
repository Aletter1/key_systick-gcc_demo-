#ifndef _LED_H
#include "main.h"
#endif // !_LED_H
/* USER CODE BEGIN Prototypes */

#define LED0_GPIO_PORT GPIOE
#define LED0_GPIO_PIN GPIO_PIN_0

#define LED1_GPIO_PORT GPIOE
#define LED1_GPIO_PIN GPIO_PIN_1

#define LED0(x)                                                                                                                            \
  do                                                                                                                                       \
  {                                                                                                                                        \
    x ? HAL_GPIO_WritePin(LED0_GPIO_PORT, LED0_GPIO_PIN, GPIO_PIN_SET) : HAL_GPIO_WritePin(LED0_GPIO_PORT, LED0_GPIO_PIN, GPIO_PIN_RESET); \
  } while (0) /* LED0 = RED */

#define LED1(x)                                                                                                                            \
  do                                                                                                                                       \
  {                                                                                                                                        \
    x ? HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_GPIO_PIN, GPIO_PIN_SET) : HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_GPIO_PIN, GPIO_PIN_RESET); \
  } while (0) /* LED1 = GREEN */

#define LED0_TOGGLE()                                  \
  do                                                   \
  {                                                    \
    HAL_GPIO_TogglePin(LED0_GPIO_PORT, LED0_GPIO_PIN); \
  } while (0) /* LED0 = !LED0 */
#define LED1_TOGGLE()                                  \
  do                                                   \
  {                                                    \
    HAL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_GPIO_PIN); \
  } while (0) /* LED1 = !LED1 */
/* USER CODE END Prototypes */
void LED_GPIO_Init(void);