#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_eth.h"
#include "LED.h"
#include "Ethernet.h"

int main(void)
{
	TaskBlock tb1, tb2, tb3, tb4;
	initTaskBlock(&tb1);
	initTaskBlock(&tb2);
	initTaskBlock(&tb3);
	initTaskBlock(&tb4);

	initControl();
	initLED1();
	initLED2();
	initLED3();
	initLED4();

	uint32_t frameLength = 0;

	while(1)
	{
	//blink_LED1_yield(&tb1);
	//blink_LED2_yield(&tb2);
	//blink_LED3_yield(&tb3);
	//blink_4_LEDs(&tb4);
		//Ethernet_Init();
		turnOnLED1();
		if(switchControl() == GPIO_PIN_SET)
		{
			frameLength = 100;
			turnOnLED2();
			if(Ethernet_Init() == HAL_OK)
			{
				turnOnLED3();
				HAL_ETH_TransmitFrame(&heth,frameLength);
				turnOnLED4();
			}
		}
	}
}
