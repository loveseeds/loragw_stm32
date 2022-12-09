#ifndef _LORA_GW_SPI_PORT_H_
#define _LORA_GW_SPI_PORT_H_

#include "stdint.h"

enum {
    CS_LOW = 0,
    CS_HIGH
};

// spi or vm_spi
#define LGW_SPI_VM_ENABLE (0)

void loragw_spi_port_init(void);
void loragw_spi_port_cs(char level);
uint16_t loragw_spi_port_write_buffer(uint8_t *buffer, uint16_t size);
uint16_t loragw_spi_port_read_buffer(uint8_t *buffer, uint16_t size);
uint16_t loragw_spi_port_trans_buffer(uint8_t *buff_wr, uint16_t size, uint8_t *buff_rd);

#endif
