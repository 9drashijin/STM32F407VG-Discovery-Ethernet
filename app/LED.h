#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"

#define turnOnLED1() 	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
#define turnOffLED1() 	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
#define turnOnLED2() 	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
#define turnOffLED2()	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
#define turnOnLED3() 	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
#define turnOffLED3()	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
#define turnOnLED4() 	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
#define turnOffLED4()	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);

#define switchControl()	HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)

#define initTaskBlock(x) 		((x)->state = LED_INITIAL)
#define yieldLED(x) 			(x)->state = __LINE__; } break; case __LINE__:
#define startTaskLED(x)			switch((x)->state) { case LED_INITIAL:
#define endTaskLED(x)			(x)->state = LED_INITIAL;} break;}

#define RunLight1(x) turnOnLED1();turnOffLED2();turnOffLED3();turnOnLED4(); // on 1
#define RunLight2(x) turnOffLED1();turnOnLED2();turnOffLED3();turnOnLED4(); // on 2
#define RunLight3(x) turnOffLED1();turnOffLED2();turnOnLED3();turnOnLED4(); // on 3
#define RunLight4(x) turnOffLED1();turnOffLED2();turnOffLED3();turnOffLED4(); // on 4

#define seriousCase(x) if(delay(x, previousTime)){previousTime = currentTime;

typedef enum{
	LED_INITIAL,
	LED_ON_STATE,
	LED_OFF_STATE
	//LED_FINAL
}State;

typedef struct{
	uint32_t state;
}TaskBlock;

void initControl();
void initLED1();
void initLED2();
void initLED3();
void initLED4();
uint32_t getCurrentTime();
int delay(uint32_t delayCycle, uint32_t previousTime);
void delays(uint32_t delayCycle);
void blink_LED1();
void blink_LED2();
void blink_LED3();
void blink_LED1_yield(TaskBlock *tb);
void blink_LED2_yield(TaskBlock *tb);
void blink_LED3_yield(TaskBlock *tb);
void blink_4_LEDs(TaskBlock *tb);
void yieldTest(TaskBlock *tb);
