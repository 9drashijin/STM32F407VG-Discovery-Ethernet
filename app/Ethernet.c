#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_eth.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_rcc.h"
#include "Ethernet.h"
#include "LED.h"
#include <string.h>

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

/*Global Variable*/
ETH_HandleTypeDef heth;

void HAL_ETH_MspInit(ETH_HandleTypeDef *heth)
{
	GPIO_InitTypeDef GpioInfo;
	if (heth->Instance == ETH) {

		//__ETH_CLK_ENABLE();
		//__SYSCFG_CLK_ENABLE();

		__GPIOA_CLK_ENABLE();
		__GPIOB_CLK_ENABLE();
		__GPIOC_CLK_ENABLE();

		// Configure PA1, PA2 and PA7 (ETH_RMII_REF_CLK, ETH_MDIO, ETH_RMII_CRS_DV
		GpioInfo.Alternate	= GPIO_AF11_ETH;
		GpioInfo.Mode		= GPIO_MODE_AF_PP;
		GpioInfo.Pin 		= GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_7;
		GpioInfo.Pull		= GPIO_NOPULL;
		GpioInfo.Speed		= GPIO_SPEED_HIGH;
		HAL_GPIO_Init(GPIOA, &GpioInfo);

		// Configure PB11 (ETH_RMII_TX_EN), PB12 (ETH_RMII_TXD0), PB13 (ETH_RMII_TXD1)
		GpioInfo.Alternate	= GPIO_AF11_ETH;
		GpioInfo.Mode		= GPIO_MODE_AF_PP;
		GpioInfo.Pin 		= GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13;
		GpioInfo.Pull		= GPIO_NOPULL;
		GpioInfo.Speed		= GPIO_SPEED_HIGH;
		HAL_GPIO_Init(GPIOB, &GpioInfo);

		// Configure PC1, PC4 and PC5 (ETH_MDC, ETH_RMII_RXD0, ETH_RMII_RXD1)
		GpioInfo.Alternate	= GPIO_AF11_ETH;
		GpioInfo.Mode		= GPIO_MODE_AF_PP;
		GpioInfo.Pin 		= GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5;
		GpioInfo.Pull		= GPIO_NOPULL;
		GpioInfo.Speed		= GPIO_SPEED_HIGH;
		HAL_GPIO_Init(GPIOC, &GpioInfo);

		/*
		//PE2 Reset pin: Optional
		GpioInfo.Mode		= GPIO_MODE_OUTPUT_PP;
		GpioInfo.Pin 		= GPIO_PIN_2;
		GpioInfo.Pull		= GPIO_PULLUP;
		GpioInfo.Speed		= GPIO_SPEED_MEDIUM;
		HAL_GPIO_Init(GPIOE, &GpioInfo);

		int i;
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_RESET);
		for (i = 0; i < 20000; i++);
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_SET);
		for (i = 0; i < 20000; i++);
		*/

		HAL_NVIC_SetPriority(ETH_IRQn, 12, 0);
		HAL_NVIC_EnableIRQ(ETH_IRQn);

		__HAL_RCC_ETH_CLK_ENABLE();
		__HAL_RCC_SYSCFG_CLK_ENABLE();

		__HAL_RCC_ETHMAC_CLK_ENABLE();
		__HAL_RCC_ETHMACTX_CLK_ENABLE();
		__HAL_RCC_ETHMACRX_CLK_ENABLE();
	}
}

void HAL_ETH_MspDeInit(ETH_HandleTypeDef* heth)
{
    if (heth->Instance == ETH) {
        __ETH_CLK_DISABLE();

        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_1  | GPIO_PIN_4  | GPIO_PIN_5);
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_1  | GPIO_PIN_2  | GPIO_PIN_7);
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13);
    }
}

void ETH_IRQHandler(void)
{
    HAL_ETH_IRQHandler(&heth);
}

uint32_t Ethernet_Init()
{
	uint32_t frameLength = 0;
	ETH_DMADescTypeDef  DMARxDscrTab[ETH_RXBUFNB], DMATxDscrTab[ETH_TXBUFNB]; 	//Rx & Tx DMA Descriptors
	uint8_t Rx_Buff[ETH_RXBUFNB][ETH_RX_BUF_SIZE]; 								// Receive buffers
	uint8_t Tx_Buff[ETH_TXBUFNB][ETH_TX_BUF_SIZE]; 								// Transmit buffers
	uint8_t MacAddress[6] = {MAC_ADDR0, MAC_ADDR1, MAC_ADDR2, MAC_ADDR3, MAC_ADDR4, MAC_ADDR5};

	heth.Instance = ETH;
	heth.Init.AutoNegotiation = ETH_AUTONEGOTIATION_ENABLE;     // ETH_AUTONEGOTIATION_DISABLE
	heth.Init.Speed = ETH_SPEED_100M;							// ETH_SPEED_10M
	heth.Init.DuplexMode = ETH_MODE_FULLDUPLEX; 				// ETH_MODE_HALFDUPLEX
	heth.Init.PhyAddress = DP83848_PHY_ADDRESS; 				// This parameter must be a number between Min_Data = 0 and Max_Data = 32
	heth.Init.MACAddr = MacAddress; 							// MAC Address of used Hardware: must be pointer on an array of 6 bytes
	heth.Init.RxMode = ETH_RXPOLLING_MODE; 						// ETH_RXINTERRUPT_MODE
	heth.Init.ChecksumMode = ETH_CHECKSUM_BY_SOFTWARE; 			// ETH_CHECKSUM_BY_HARDWARE
	heth.Init.MediaInterface = ETH_MEDIA_INTERFACE_RMII;		// ETH_MEDIA_INTERFACE_RMII
	/*
	turnOnLED1();
	turnOnLED2();
	turnOnLED3();
	turnOnLED4();
	*/
	//turnOnLED1();
	if(HAL_ETH_Init(&heth) == !HAL_OK)
	{
		HAL_ETH_DeInit(&heth);
	}
	//turnOnLED2();
	HAL_ETH_DMATxDescListInit(&heth, DMATxDscrTab, &Tx_Buff[0][0], ETH_TXBUFNB);
	HAL_ETH_DMARxDescListInit(&heth, DMARxDscrTab, &Rx_Buff[0][0], ETH_RXBUFNB);
	//turnOnLED3();

	HAL_ETH_Start(&heth);

	//turnOnLED1();

	uint8_t *buffer = (uint8_t *)(heth.TxDesc->Buffer1Addr);

	uint32_t testArray[4] = {1,0,1,0};
	uint8_t *a = (uint8_t*)(&testArray);
	memcpy(buffer, a, 4*4);

	//memcpy(buffer,"A", 1);

	frameLength = 4*4;
	//HAL_ETH_TransmitFrame(&heth,frameLength);

	if(HAL_ETH_TransmitFrame(&heth, frameLength) == HAL_OK) turnOnLED3();

	//if(HAL_ETH_GetReceivedFrame(&heth) == HAL_OK) turnOnLED4();

	return HAL_OK;
}
