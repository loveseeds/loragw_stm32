#include "spi.h"

void spi_init(void)
{
    SPI_InitTypeDef SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    /* 使能SPI时钟 */
    CFG_SPI_APBxClock_FUN(CFG_SPI_CLK, ENABLE);

    /* 使能SPI引脚相关的时钟 */
    CFG_SPI_CS_APBxClock_FUN(CFG_SPI_CS_CLK | CFG_SPI_SCK_CLK | CFG_SPI_MISO_PIN | CFG_SPI_MOSI_PIN, ENABLE);

    /* 配置SPI的 CS引脚，普通IO即可 */
    GPIO_InitStructure.GPIO_Pin = CFG_SPI_CS_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(CFG_SPI_CS_PORT, &GPIO_InitStructure);

    /* 配置SPI的 SCK引脚*/
    GPIO_InitStructure.GPIO_Pin = CFG_SPI_SCK_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(CFG_SPI_SCK_PORT, &GPIO_InitStructure);

    /* 配置SPI的 MISO引脚*/
    GPIO_InitStructure.GPIO_Pin = CFG_SPI_MISO_PIN;
    GPIO_Init(CFG_SPI_MISO_PORT, &GPIO_InitStructure);

    /* 配置SPI的 MOSI引脚*/
    GPIO_InitStructure.GPIO_Pin = CFG_SPI_MOSI_PIN;
    GPIO_Init(CFG_SPI_MOSI_PORT, &GPIO_InitStructure);

    /* 停止信号 FLASH: CS引脚高电平*/
    CFG_SPI_CS_HIGH();

    /* SPI 模式配置 */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; // 模式0
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(CFG_SPIx, &SPI_InitStructure);

    /* 使能 SPI  */
    SPI_Cmd(CFG_SPIx, ENABLE);
}

uint8_t spi_trans_byte(uint8_t byte)
{
    uint8_t retry = 0;
    while (SPI_I2S_GetFlagStatus(CFG_SPIx, SPI_I2S_FLAG_TXE) == RESET)
    {
        if ((retry++) > 200)
            return 0x00;
    }

    SPI_I2S_SendData(CFG_SPIx, byte);

    retry = 0;
    while (SPI_I2S_GetFlagStatus(CFG_SPIx, SPI_I2S_FLAG_RXNE) == RESET)
    {
        if ((retry++) > 200)
            return 0x00;
    }

    return SPI_I2S_ReceiveData(CFG_SPIx);
}

uint16_t spi_write_buffer(uint8_t *buffer, uint16_t size)
{
    uint16_t r = size;
    while (size--)
    {
        spi_trans_byte(*buffer);
        buffer++;
    }
    return r;
}

uint16_t spi_read_buffer(uint8_t *buffer, uint16_t size)
{
    uint16_t r = size;
    while (size--)
    {
        *buffer = spi_trans_byte(CFG_SPI_TRANS_DUMMY_BYTE);
        buffer++;
    }
    return r;
}

uint16_t spi_trans_buffer(uint8_t *buff_wr, uint16_t size, uint8_t *buff_rd)
{
    uint16_t r = size;
    while (size--)
    {
        *buff_rd = spi_trans_byte(*buff_wr);
        buff_wr++;
        buff_rd++;
    }
    return r;
}
