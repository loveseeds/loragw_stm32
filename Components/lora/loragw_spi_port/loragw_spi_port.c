#include "loragw_spi_port.h"

#if !LGW_SPI_VM_ENABLE
#include "spi.h"
#else
#include "vm_spi.h"
#endif

void loragw_spi_port_init(void)
{
#if !LGW_SPI_VM_ENABLE
	spi_init();
#else
	vm_spi_init();
#endif
}

void loragw_spi_port_cs(char level)
{
	if (level)
	{
#if !LGW_SPI_VM_ENABLE
		CFG_SPI_CS_HIGH();
#else
		VM_SPI_CS_HIGH();
#endif
	}
	else
	{
#if !LGW_SPI_VM_ENABLE
		CFG_SPI_CS_LOW();
#else
		VM_SPI_CS_LOW();
#endif
	}
}

uint16_t loragw_spi_port_write_buffer(uint8_t *buffer, uint16_t size)
{
#if !LGW_SPI_VM_ENABLE
	return spi_write_buffer(buffer, size);
#else
	return vm_spi_write_buffer(buffer, size);
#endif
}

uint16_t loragw_spi_port_read_buffer(uint8_t *buffer, uint16_t size)
{
#if !LGW_SPI_VM_ENABLE
	return spi_read_buffer(buffer, size);
#else
	return vm_spi_read_buffer(buffer, size);
#endif
}

uint16_t loragw_spi_port_trans_buffer(uint8_t *buff_wr, uint16_t size, uint8_t *buff_rd)
{
#if !LGW_SPI_VM_ENABLE
	return spi_trans_buffer(buff_wr, size, buff_rd);
#else
	return vm_spi_trans_buffer(buff_wr, size, buff_rd);
#endif
}
