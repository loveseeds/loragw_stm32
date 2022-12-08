#ifndef __VM_SPI_H_
#define __VM_SPI_H_

#include "stm32f10x.h"

//VM_SPI_SCK
#define VM_SPI_SCK_PORT             GPIOA
#define VM_SPI_SCK_PIN              GPIO_Pin_5
#define VM_SPI_SCK_CLK              RCC_APB2Periph_GPIOA
//VM_SPI_MOSI       
#define VM_SPI_MOSI_PORT            GPIOA
#define VM_SPI_MOSI_PIN             GPIO_Pin_7
#define VM_SPI_MOSI_CLK             RCC_APB2Periph_GPIOA
//VM_SPI_MISO		        
#define VM_SPI_MISO_PORT            GPIOA
#define VM_SPI_MISO_PIN             GPIO_Pin_6
#define VM_SPI_MISO_CLK             RCC_APB2Periph_GPIOA
//VM_SPI_CS     
#define VM_SPI_CS_PORT              GPIOA
#define VM_SPI_CS_PIN               GPIO_Pin_4
#define VM_SPI_CS_CLK               RCC_APB2Periph_GPIOA

#define VM_SPI_SCLK(x)      	    ((x) ? ( GPIO_SetBits(VM_SPI_SCK_PORT, VM_SPI_SCK_PIN)) : (GPIO_ResetBits(VM_SPI_SCK_PORT, VM_SPI_SCK_PIN)))
#define VM_SPI_MOSI(x)      	    ((x) ? ( GPIO_SetBits(VM_SPI_MOSI_PORT, VM_SPI_MOSI_PIN)) : (GPIO_ResetBits(VM_SPI_MOSI_PORT, VM_SPI_MOSI_PIN)))
#define VM_SPI_MISO()			    (GPIO_ReadInputDataBit(VM_SPI_MISO_PORT, VM_SPI_MISO_PIN))

#define VM_SPI_CS_LOW()       	    (GPIO_ResetBits(VM_SPI_CS_PORT, VM_SPI_CS_PIN))
#define VM_SPI_CS_HIGH()      	    (GPIO_SetBits(VM_SPI_CS_PORT, VM_SPI_CS_PIN))

#define VM_SPI_TRANS_DUMMY_BYTE     (0x00)

void vm_spi_init(void);
uint16_t vm_spi_write_buffer(uint8_t* buffer, uint16_t size);
uint16_t vm_spi_read_buffer(uint8_t *buffer, uint16_t size);
uint16_t vm_spi_trans_buffer(uint8_t *buff_wr, uint16_t size, uint8_t *buff_rd);

#endif 




