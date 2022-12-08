#ifndef _CFG_SPI_H_
#define _CFG_SPI_H_

#include "stm32f10x.h"
#include <stdio.h>
#include <stdint.h>

/*SPI接口定义-开头****************************/
#define CFG_SPIx                        SPI1
#define CFG_SPI_APBxClock_FUN           RCC_APB2PeriphClockCmd
#define CFG_SPI_CLK                     RCC_APB2Periph_SPI1

// CS(NSS)引脚 片选选普通GPIO即可
#define CFG_SPI_CS_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define CFG_SPI_CS_CLK                  RCC_APB2Periph_GPIOA
#define CFG_SPI_CS_PORT                 GPIOA
#define CFG_SPI_CS_PIN                  GPIO_Pin_4

// SCK引脚
#define CFG_SPI_SCK_APBxClock_FUN       RCC_APB2PeriphClockCmd
#define CFG_SPI_SCK_CLK                 RCC_APB2Periph_GPIOA
#define CFG_SPI_SCK_PORT                GPIOA
#define CFG_SPI_SCK_PIN                 GPIO_Pin_5
// MISO引脚
#define CFG_SPI_MISO_APBxClock_FUN      RCC_APB2PeriphClockCmd
#define CFG_SPI_MISO_CLK                RCC_APB2Periph_GPIOA
#define CFG_SPI_MISO_PORT               GPIOA
#define CFG_SPI_MISO_PIN                GPIO_Pin_6
// MOSI引脚
#define CFG_SPI_MOSI_APBxClock_FUN      RCC_APB2PeriphClockCmd
#define CFG_SPI_MOSI_CLK                RCC_APB2Periph_GPIOA
#define CFG_SPI_MOSI_PORT               GPIOA
#define CFG_SPI_MOSI_PIN                GPIO_Pin_7

#define CFG_SPI_CS_LOW() 				GPIO_ResetBits(CFG_SPI_CS_PORT, CFG_SPI_CS_PIN)
#define CFG_SPI_CS_HIGH() 				GPIO_SetBits(CFG_SPI_CS_PORT, CFG_SPI_CS_PIN)

#define CFG_SPI_TRANS_DUMMY_BYTE        (0x00)

void spi_init(void);
uint16_t spi_write_buffer(uint8_t* buffer, uint16_t size);
uint16_t spi_read_buffer(uint8_t *buffer, uint16_t size);
uint16_t spi_trans_buffer(uint8_t *buff_wr, uint16_t size, uint8_t *buff_rd);

#endif // !_CFG_SPI_H_
