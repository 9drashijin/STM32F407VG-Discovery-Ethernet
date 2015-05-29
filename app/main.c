#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_eth.h"
#include "LED.h"
#include "Ethernet.h"

ETH_HandleTypeDef heth;

int main(void)
{
	initControl();
	initLED1();
	initLED2();
	initLED3();
	initLED4();

	while(1)
		{
		 //blink_LED1_yield();
		 //blink_LED2_yield();
		 //blink_LED3_yield();

			turnOnLED2();
			if(switchControl() == GPIO_PIN_SET)
			{
				turnOnLED4();
				if(Ethernet_Init() == 1)
				{
					HAL_ETH_Start(&heth);
					turnOnLED1();
				}
				turnOffLED4();
			}
		}
}
