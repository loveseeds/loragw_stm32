#include "loragw_spi.h"

#include <stdint.h>     /* C99 types */
#include <stdio.h>      /* printf fprintf */
#include <stdlib.h>     /* malloc free */

#include "loragw_spi_port.h"

/* -------------------------------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#if DEBUG_SPI == 1
    // #define DEBUG_MSG(str)                fprintf(stderr, str)
    // #define DEBUG_PRINTF(fmt, args...)    fprintf(stderr,"%s:%d: "fmt, __FUNCTION__, __LINE__, args)
    // #define CHECK_NULL(a)                if(a==NULL){fprintf(stderr,"%s:%d: ERROR: NULL POINTER AS ARGUMENT\n", __FUNCTION__, __LINE__);return LGW_SPI_ERROR;}
#else
    #define DEBUG_MSG(str)
    #define DEBUG_PRINTF(fmt, args...)
    #define CHECK_NULL(a)                if(a==NULL){return LGW_SPI_ERROR;}
#endif

/* -------------------------------------------------------------------------- */
/* --- PRIVATE CONSTANTS ---------------------------------------------------- */

#define READ_ACCESS     0x00
#define WRITE_ACCESS    0x80

/* -------------------------------------------------------------------------- */
/* --- PUBLIC FUNCTIONS DEFINITION ------------------------------------------ */

/* SPI initialization and configuration */
int lgw_spi_open(const char * spidev_path, void **spi_target_ptr)
{
    *spi_target_ptr = (void *)"spi_target_ptr";
    loragw_spi_port_init();
    DEBUG_MSG("Note: SPI port opened and configured ok\n");
    return LGW_SPI_SUCCESS;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* SPI release */
int lgw_spi_close(void *spi_target)
{
    DEBUG_MSG("Note: SPI port closed\n");
    return LGW_SPI_SUCCESS;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* Simple write */
int lgw_spi_w(void *spi_target, uint8_t spi_mux_target, uint16_t address, uint8_t data)
{
    uint8_t out_buf[4];
    uint8_t command_size;

    /* prepare frame to be sent */
    out_buf[0] = spi_mux_target;
    out_buf[1] = WRITE_ACCESS | ((address >> 8) & 0x7F);
    out_buf[2] =                ((address >> 0) & 0xFF);
    out_buf[3] = data;
    command_size = 4;

    /* transaction */
    loragw_spi_port_cs(CS_LOW);
    loragw_spi_port_write_buffer(out_buf, command_size);
    loragw_spi_port_cs(CS_HIGH);

    return LGW_SPI_SUCCESS;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* Simple read */
int lgw_spi_r(void *spi_target, uint8_t spi_mux_target, uint16_t address, uint8_t *data) 
{
    uint8_t out_buf[5];
    uint8_t command_size;
    uint8_t in_buf[ARRAY_SIZE(out_buf)];
    
    CHECK_NULL(data);

    /* prepare frame to be sent */
    out_buf[0] = spi_mux_target;
    out_buf[1] = READ_ACCESS | ((address >> 8) & 0x7F);
    out_buf[2] =               ((address >> 0) & 0xFF);
    out_buf[3] = 0x00;
    out_buf[4] = 0x00;
    command_size = 5;

    /* transaction */
    loragw_spi_port_cs(CS_LOW);
    loragw_spi_port_trans_buffer(out_buf, command_size, in_buf);
    loragw_spi_port_cs(CS_HIGH);

    *data = in_buf[command_size - 1];
    return LGW_SPI_SUCCESS;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* Burst (multiple-byte) write */
int lgw_spi_wb(void *spi_target, uint8_t spi_mux_target, uint16_t address, const uint8_t *data, uint16_t size) {
    uint8_t command[3];
    uint8_t command_size;
    int size_to_do, chunk_size, offset;
    int byte_transfered = 0;
    int i;

    /* check input parameters */
    CHECK_NULL(data);
    if (size == 0) {
        return LGW_SPI_ERROR;
    }

    /* prepare command byte */
    command[0] = spi_mux_target;
    command[1] = WRITE_ACCESS | ((address >> 8) & 0x7F);
    command[2] =                ((address >> 0) & 0xFF);
    command_size = 3;
    size_to_do = size;

    /* transaction */
    loragw_spi_port_cs(CS_LOW);
    loragw_spi_port_write_buffer(command, command_size);
    
    for (i=0; size_to_do > 0; ++i) {
        chunk_size = (size_to_do < LGW_BURST_CHUNK) ? size_to_do : LGW_BURST_CHUNK;
        offset = i * LGW_BURST_CHUNK;
        byte_transfered += loragw_spi_port_write_buffer((uint8_t*)(data + offset), chunk_size);
        size_to_do -= chunk_size; /* subtract the quantity of data already transferred */
    }

    loragw_spi_port_cs(CS_HIGH);

    return LGW_SPI_SUCCESS;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* Burst (multiple-byte) read */
int lgw_spi_rb(void *spi_target, uint8_t spi_mux_target, uint16_t address, uint8_t *data, uint16_t size) {
    uint8_t command[4];
    uint8_t command_size;
    int size_to_do, chunk_size, offset;
    int byte_transfered = 0;
    int i;

    /* check input parameters */
    CHECK_NULL(data);
    if (size == 0) {
        return LGW_SPI_ERROR;
    }

    /* prepare command byte */
    command[0] = spi_mux_target;
    command[1] = READ_ACCESS | ((address >> 8) & 0x7F);
    command[2] =               ((address >> 0) & 0xFF);
    command[3] = 0x00;
    command_size = 4;
    size_to_do = size;

    /* transaction */
    loragw_spi_port_cs(CS_LOW);
    loragw_spi_port_write_buffer(command, command_size);

    for (i=0; size_to_do > 0; ++i) {
        chunk_size = (size_to_do < LGW_BURST_CHUNK) ? size_to_do : LGW_BURST_CHUNK;
        offset = i * LGW_BURST_CHUNK;
        byte_transfered += loragw_spi_port_read_buffer((uint8_t*)(data + offset), chunk_size);
        size_to_do -= chunk_size;  /* subtract the quantity of data already transferred */
    }
    
    loragw_spi_port_cs(CS_HIGH);
    
    return LGW_SPI_SUCCESS;
}

/* --- EOF ------------------------------------------------------------------ */
