#ifndef __DELAY_H_
#define __DELAY_H_

#include "stm32f10x.h"

void delay_init(void);
void delay_ms(uint32_t ms);
void delay_us(uint32_t us);


#endif /* __DELAY_H_ */
