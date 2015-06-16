#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_eth.h"
#include "LED.h"
#include "Ethernet.h"

void SystemClock_Config(void);
void GPIO_Init(void);

/* Ethernet Pins Configuration
 * ===========================
 * ETH_MDC---------------->PC1
 * ETH_MDIO--------------->PA2
 * ETH_RMII_REF_CLK------->PA1
 * ETH_RMII_CRS_DV-------->PA7
 * ETH_RMII_RXD0---------->PC4
 * ETH_RMII_RXD1---------->PC5
 * ETH_RMII_TX_EN--------->PB11
 * ETH_RMII_TXD0---------->PB12
 * ETH_RMII_TXD1---------->PB13
 * */

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

	SystemClock_Config();
	GPIO_Init();

	while(1)
	{
		//blink_LED1_yield(&tb1);
		//blink_LED2_yield(&tb2);
		//blink_LED3_yield(&tb3);
		turnOnLED1();
		if(switchControl() == GPIO_PIN_SET)
		{
			turnOnLED2();
			Ethernet_Init();
			HAL_ETH_Ready();
			HAL_ETH_Send();
			//HAL_ETH_Receive();
			turnOnLED4();
		}
	}
}
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  __PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 200;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

}
void GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __GPIOC_CLK_ENABLE();
  __GPIOA_CLK_ENABLE();
  __GPIOB_CLK_ENABLE();

}
