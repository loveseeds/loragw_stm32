#ifndef _LORA_GW_H_
#define _LORA_GW_H_

#define LORAGW_RESET_PORT    	GPIOB
#define LORAGW_RESET_CLK 	    RCC_APB2Periph_GPIOB
#define LORAGW_RESET_PIN		GPIO_Pin_5

void loragw_init(void);
void loragw_get_version(void);
void loragw_memory_test(void);

#endif // !_LORA_GW_H_

