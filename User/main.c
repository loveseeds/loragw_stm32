#include "stm32f10x.h"
#include "main.h"

int main(void)
{
	log_init();
	delay_init();
	loragw_init();
	loragw_get_version();
	
	while(1)
	{
		loragw_memory_test();
		delay_ms(5000);
	}
}



