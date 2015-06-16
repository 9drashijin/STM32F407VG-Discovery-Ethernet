#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"

extern ETH_HandleTypeDef heth; // ethernet Handle

void Ethernet_Init();
void HAL_ETH_Ready();
void HAL_ETH_Send();
void HAL_ETH_Receive();
