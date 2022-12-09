#ifndef _LORA_GW_SPI_H_
#define _LORA_GW_SPI_H_

/* -------------------------------------------------------------------------- */
/* --- DEPENDANCIES --------------------------------------------------------- */

#include <stdint.h>        /* C99 types*/

/* -------------------------------------------------------------------------- */
/* --- PUBLIC CONSTANTS ----------------------------------------------------- */

#define LGW_SPI_SUCCESS     0
#define LGW_SPI_ERROR       -1
#define LGW_BURST_CHUNK     1024

#define SPI_SPEED       2000000

#define LGW_SPI_MUX_TARGET_SX1302   0x00
#define LGW_SPI_MUX_TARGET_RADIOA   0x01
#define LGW_SPI_MUX_TARGET_RADIOB   0x02

int lgw_spi_open(const char * spidev_path, void **spi_target_ptr);
int lgw_spi_close(void *spi_target);
int lgw_spi_w(void *spi_target, uint8_t spi_mux_target, uint16_t address, uint8_t data);
int lgw_spi_r(void *spi_target, uint8_t spi_mux_target, uint16_t address, uint8_t *data);
int lgw_spi_wb(void *spi_target, uint8_t spi_mux_target, uint16_t address, const uint8_t *data, uint16_t size);
int lgw_spi_rb(void *spi_target, uint8_t spi_mux_target, uint16_t address, uint8_t *data, uint16_t size);

#endif // !_LORA_GW_SPI_H_

