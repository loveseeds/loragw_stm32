#include "loragw.h"
#include "delay.h"
#include "log.h"
#include "loragw_spi.h"

void* spi_target_ptr = NULL;

void loragw_reset_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(LORAGW_RESET_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = LORAGW_RESET_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LORAGW_RESET_PORT, &GPIO_InitStructure);
}

#define LORAGW_RESET_LOW()  	GPIO_ResetBits(LORAGW_RESET_PORT, LORAGW_RESET_PIN)
#define LORAGW_RESET_HIGH() 	GPIO_SetBits(LORAGW_RESET_PORT, LORAGW_RESET_PIN)

void loragw_reset(void)
{
	LORAGW_RESET_HIGH();
    delay_ms(100);
    LORAGW_RESET_LOW();
    delay_ms(100);
}

void loragw_init(void)
{
    loragw_reset_init();
    loragw_reset();
    lgw_spi_open(NULL, &spi_target_ptr);
}

#define BUFF_SIZE   		200
#define SX1302_AGC_MCU_MEM  0x0000
#define SX1302_REG_COMMON   0x5600
#define SX1302_REG_AGC_MCU  0x5780

void loragw_get_version(void)
{
    uint8_t data = 0;
    lgw_spi_r(spi_target_ptr, LGW_SPI_MUX_TARGET_SX1302, SX1302_REG_COMMON + 6, &data);
    log_i("SX1302 version: 0x%02X\n", data);
}

void loragw_memory_test(void)
{
    uint8_t data = 0;
    uint8_t test_buff[BUFF_SIZE] = {0};
    uint8_t read_buff[BUFF_SIZE] = {0};
    uint16_t size = BUFF_SIZE;
    int i;

	lgw_spi_r(spi_target_ptr, LGW_SPI_MUX_TARGET_SX1302, SX1302_REG_AGC_MCU + 0, &data);
    lgw_spi_w(spi_target_ptr, LGW_SPI_MUX_TARGET_SX1302, SX1302_REG_AGC_MCU + 0, 0x06); /* mcu_clear, host_prog */
	
	for(i=0; i<BUFF_SIZE; i++)
	{
		test_buff[i] = i + 0x0A;
	}

    lgw_spi_wb(spi_target_ptr, LGW_SPI_MUX_TARGET_SX1302, SX1302_AGC_MCU_MEM, test_buff, size);
    lgw_spi_rb(spi_target_ptr, LGW_SPI_MUX_TARGET_SX1302, SX1302_AGC_MCU_MEM, read_buff, size);

    for (i=0; ((i<size) && (test_buff[i] == read_buff[i])); ++i) ;
	
	if (i != size)
	{
		printf("error during the buffer comparison\n");
		printf("Written values:\n");
		for (i=0; i<size; ++i) {
			printf(" %02X ", test_buff[i]);
			if (i%16 == 15) printf("\n");
		}
		printf("\n");
		printf("Read values:\n");
		for (i=0; i<size; ++i) {
			printf(" %02X ", read_buff[i]);
			if (i%16 == 15) printf("\n");
		}
		printf("\n");
	}
	else {
		printf("did a %i-byte R/W on a data buffer with no error\n", size);
	}
}

