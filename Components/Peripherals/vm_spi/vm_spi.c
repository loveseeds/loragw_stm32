#include "vm_spi.h"
#include "delay.h"

void vm_spi_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	// Clock
	RCC_APB2PeriphClockCmd(VM_SPI_SCK_CLK | VM_SPI_MOSI_CLK | VM_SPI_MISO_CLK | VM_SPI_CS_CLK, ENABLE);
	// IO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	// VM_SPI_SCK
	GPIO_InitStructure.GPIO_Pin = VM_SPI_SCK_PIN;
	GPIO_Init(VM_SPI_SCK_PORT, &GPIO_InitStructure);
	// VM_SPI_MOSI
	GPIO_InitStructure.GPIO_Pin = VM_SPI_MOSI_PIN;
	GPIO_Init(VM_SPI_MOSI_PORT, &GPIO_InitStructure);
	// VM_SPI_CS
	GPIO_InitStructure.GPIO_Pin = VM_SPI_CS_PIN;
	GPIO_Init(VM_SPI_CS_PORT, &GPIO_InitStructure);
	// VM_SPI_MISO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = VM_SPI_MISO_PIN;
	GPIO_Init(VM_SPI_MISO_PORT, &GPIO_InitStructure);
	// VM_SPI_CS_HIGH
	VM_SPI_CS_HIGH();
}

// Mode0 MSB
uint8_t vm_spi_trans_byte(uint8_t byte)
{
	uint8_t rx_data = 0;

	VM_SPI_SCLK(0);
	for (u8 i = 0; i < 8; i++)
	{
		if (byte & (1 << 7))
			VM_SPI_MOSI(1);
		else
			VM_SPI_MOSI(0);

		byte <<= 1;
		delay_us(1);
		VM_SPI_SCLK(1);

		delay_us(1);
		rx_data <<= 1;
		rx_data |= VM_SPI_MISO();
		VM_SPI_SCLK(0);
	}
	VM_SPI_SCLK(0);

	return rx_data;
}

uint16_t vm_spi_write_buffer(uint8_t *buffer, uint16_t size)
{
	uint16_t r = size;
	while (size--)
	{
		vm_spi_trans_byte(*buffer);
		buffer++;
	}
	return r;
}

uint16_t vm_spi_read_buffer(uint8_t *buffer, uint16_t size)
{
	uint16_t r = size;
	while (size--)
	{
		*buffer = vm_spi_trans_byte(VM_SPI_TRANS_DUMMY_BYTE);
		buffer++;
	}
	return r;
}

uint16_t vm_spi_trans_buffer(uint8_t *buff_wr, uint16_t size, uint8_t *buff_rd)
{
	uint16_t r = size;
	while (size--)
	{
		*buff_rd = vm_spi_trans_byte(*buff_wr);
		buff_wr++;
		buff_rd++;
	}
	return r;
}
