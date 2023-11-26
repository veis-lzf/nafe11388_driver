/**
 * @file bsp_spi.c
 * @author veis (1256639585@qq.com)
 * @brief 对AT32F435的SPI进行驱动层封装，提供相关同步接口进行读写操作，暂时不支持中断和DMA接口，只实现SPI2的GPIO封装
 * @version 0.0.1
 * @date 2023-11-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "bsp_spi.h"
#include "at32f435_437_gpio.h"
#include "at32f435_437_spi.h"
#include "at32f435_437_crm.h"
#include <assert.h>

///////////////////////////////////////////pins define/////////////////////////////////////////////////
// MOSI
#define SPI2_MOSI_PINMUX    GPIO_PINS_15
#define SPI2_MOSI_PORT      GPIOB
#define SPI2_MOSI_SOURCE    GPIO_PINS_SOURCE15
#define SPI2_MOSI_GPIO_CLK  CRM_GPIOB_PERIPH_CLOCK
// MISO
#define SPI2_MISO_PINMUX    GPIO_PINS_14
#define SPI2_MISO_PORT      GPIOB
#define SPI2_MISO_SOURCE    GPIO_PINS_SOURCE14
#define SPI2_MISO_GPIO_CLK  CRM_GPIOB_PERIPH_CLOCK
// SCK
#define SPI2_SCK_PINMUX    GPIO_PINS_13
#define SPI2_SCK_PORT      GPIOB
#define SPI2_SCK_SOURCE    GPIO_PINS_SOURCE13
#define SPI2_SCK_GPIO_CLK  CRM_GPIOB_PERIPH_CLOCK
// CS
#define SPI2_CS_PINMUX     GPIO_PINS_12
#define SPI2_CS_PORT       GPIOB
#define SPI2_CS_SOURCE     GPIO_PINS_SOURCE12
#define SPI2_CS_GPIO_CLK   CRM_GPIOB_PERIPH_CLOCK
// 外设时钟
#define SPI2_PERIPH_CLOCK  CRM_SPI2_PERIPH_CLOCK
// 复用组
#define SPI2_IOMUX_SEQ     GPIO_MUX_5
///////////////////////////////////////////end of pins define/////////////////////////////////////////////////



// 回调函数指针变量
static transmit_cb transmit_callback_func = NULL;
static received_cb received_callback_func = NULL;

/**
 * @brief Construct a new bsp spi gpio config object
 * @param  none
 * @retval none
 */
static void bsp_spi_gpio_config(void)
{
    gpio_init_type gpio_initstructure;

    crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
    gpio_default_para_init(&gpio_initstructure);
    /* spi2 sck pin */
    gpio_initstructure.gpio_out_type       = GPIO_OUTPUT_PUSH_PULL;
    gpio_initstructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
    gpio_initstructure.gpio_pull           = GPIO_PULL_DOWN;
    gpio_initstructure.gpio_mode           = GPIO_MODE_MUX;
    gpio_initstructure.gpio_pins           = SPI2_SCK_PINMUX;
    gpio_init(SPI2_SCK_PORT, &gpio_initstructure);
    gpio_pin_mux_config(SPI2_SCK_PORT, SPI2_SCK_SOURCE, SPI2_IOMUX_SEQ);

    /* spi2 miso pin */
    gpio_initstructure.gpio_pull           = GPIO_PULL_UP;
    gpio_initstructure.gpio_pins           = SPI2_MISO_PINMUX;
    gpio_init(SPI2_MISO_PORT, &gpio_initstructure);
    gpio_pin_mux_config(SPI2_MISO_PORT, SPI2_MISO_SOURCE, SPI2_IOMUX_SEQ);

    /* spi2 mosi pin */
    gpio_initstructure.gpio_pull           = GPIO_PULL_UP;
    gpio_initstructure.gpio_pins           = SPI2_MOSI_PINMUX;
    gpio_init(SPI2_MOSI_PORT, &gpio_initstructure);
    gpio_pin_mux_config(SPI2_MOSI_PORT, SPI2_MOSI_SOURCE, SPI2_IOMUX_SEQ);

    /* spi2 cs pin */
    gpio_initstructure.gpio_pull           = GPIO_PULL_UP;
    gpio_initstructure.gpio_mode           = GPIO_MODE_OUTPUT;
    gpio_initstructure.gpio_pins           = SPI2_CS_PINMUX;
    gpio_init(SPI2_CS_PORT, &gpio_initstructure);
}

/**
 * @brief SPI配置初始化
 * @param spix SPI序号，见@SPI_Seq_t
 * @param mode SPI工作模式，见@SPI_Mode_t
 * @param speed SPI速率，见@SPI_Speed_t
 * @param is_master 主从模式，主模式：1，从模式：0，见@SPI_Role_t
 * @param type SPI工作类型，如：全双工，半双工，见@SPI_Type_t
 * @param bitnum SPI数据位数，@SPI_Frame_Bitnum_t
 * @param is_lsb 字节序，0表示先发LSB，1表示先发MSB，@SPI_ByteOrder_t
 * @retval none
 */
void bsp_spi_config(uint8_t spix, uint8_t mode, uint8_t speed, uint8_t is_master, uint8_t type, uint8_t bitnum, uint8_t is_lsb)
{
    // 0.参数有效性校验
#if NO_USE_MICROLIB
    assert(spix > SPI_Seq3);
	assert(mode > SPI_Mode_3);
	assert(is_master > SPI_Mode_Master);
	assert(type > SPI_Type_HALF_DUPLEX_TX);
	assert(bitnum > SPI_Frame_16BIT);
	assert(is_lsb > SPI_FRAME_MSB);
#endif
    // 1.初始化GPIO
    bsp_spi_gpio_config();

    // 2.配置SPI参数
    spi_init_type spi_init_struct;
    spi_default_para_init(&spi_init_struct);

    // 设置SPI工作模式
    switch(mode)
    {
        case SPI_Mode_0:
        {
            spi_init_struct.clock_polarity = SPI_CLOCK_POLARITY_LOW;
            spi_init_struct.clock_phase = SPI_CLOCK_PHASE_1EDGE;
        }
        break;
        case SPI_Mode_1:
        {
            spi_init_struct.clock_polarity = SPI_CLOCK_POLARITY_LOW;
            spi_init_struct.clock_phase = SPI_CLOCK_PHASE_2EDGE;
        }
        break;
        case SPI_Mode_2:
        {
            spi_init_struct.clock_polarity = SPI_CLOCK_POLARITY_HIGH;
            spi_init_struct.clock_phase = SPI_CLOCK_PHASE_1EDGE;
        }
        break;
        case SPI_Mode_3:
        {
            spi_init_struct.clock_polarity = SPI_CLOCK_POLARITY_HIGH;
            spi_init_struct.clock_phase = SPI_CLOCK_PHASE_2EDGE;
        }
        break;
    }

    // 设置通信方向性 
    switch(type)
    {
        case SPI_Type_FULL_DUPLEX:
            spi_init_struct.transmission_mode = SPI_TRANSMIT_FULL_DUPLEX;
            break;
        case SPI_Type_SIMPLEX_RX:
            spi_init_struct.transmission_mode = SPI_TRANSMIT_SIMPLEX_RX;
            break;
        case SPI_Type_HALF_DUPLEX_RX:
            spi_init_struct.transmission_mode = SPI_TRANSMIT_HALF_DUPLEX_RX;
            break;
        case SPI_Type_HALF_DUPLEX_TX:
            spi_init_struct.transmission_mode = SPI_TRANSMIT_HALF_DUPLEX_TX;
            break;
    }
    
    // 设置主从模式
    if(is_master == SPI_Mode_Master)
    {
        spi_init_struct.master_slave_mode = SPI_MODE_MASTER;
    }
    else if(is_master == SPI_Mode_Slave)
    {
        spi_init_struct.master_slave_mode = SPI_MODE_SLAVE;
    }
    
    // 设置速率
    switch (speed)
    {
    case SPI_Speed_140Kbps:
        spi_init_struct.mclk_freq_division = SPI_MCLK_DIV_1024;
        break;
    case SPI_Speed_280Kbps:
        spi_init_struct.mclk_freq_division = SPI_MCLK_DIV_512;
        break;
    case SPI_Speed_560Kbps:
        spi_init_struct.mclk_freq_division = SPI_MCLK_DIV_256;
        break;
    case SPI_Speed_1M1bps:
        spi_init_struct.mclk_freq_division = SPI_MCLK_DIV_128;
        break;
    case SPI_Speed_2M2bps:
        spi_init_struct.mclk_freq_division = SPI_MCLK_DIV_64;
        break;
    case SPI_Speed_4M5bps:
        spi_init_struct.mclk_freq_division = SPI_MCLK_DIV_32;
        break;
    case SPI_Speed_9Mbps:
        spi_init_struct.mclk_freq_division = SPI_MCLK_DIV_16;
        break;    
    case SPI_Speed_18Mbps:
        spi_init_struct.mclk_freq_division = SPI_MCLK_DIV_8;
        break;
    case SPI_Speed_36Mbps:
        spi_init_struct.mclk_freq_division = SPI_MCLK_DIV_4;
        break;
    case SPI_Speed_48Mbps:
        spi_init_struct.mclk_freq_division = SPI_MCLK_DIV_3;
        break;
    case SPI_Speed_72Mbps:
        spi_init_struct.mclk_freq_division = SPI_MCLK_DIV_2;
        break;
    }
    
    // 字节序
    if(is_lsb == SPI_FRAME_LSB)
    {
        spi_init_struct.first_bit_transmission = SPI_FIRST_BIT_LSB;
    }
    else if(is_lsb == SPI_FRAME_MSB)
    {
        spi_init_struct.first_bit_transmission = SPI_FIRST_BIT_MSB;
    }

    // 选择数据位
    if(bitnum == SPI_Frame_8BIT)
    {
        spi_init_struct.frame_bit_num = SPI_FRAME_8BIT;
    }
    else if(bitnum == SPI_Frame_16BIT)
    {
        spi_init_struct.frame_bit_num = SPI_FRAME_16BIT;
    }

    // 默认为软件管理片选
    spi_init_struct.cs_mode_selection = SPI_CS_SOFTWARE_MODE;

    // 设置SPI序号,使能SPI
    switch (spix)
    {
    case SPI_Seq1:
        crm_periph_clock_enable(CRM_SPI1_PERIPH_CLOCK, TRUE);
        spi_init(SPI1, &spi_init_struct);
        spi_enable(SPI1, TRUE);
        break;
    case SPI_Seq2:
        crm_periph_clock_enable(CRM_SPI2_PERIPH_CLOCK, TRUE);
        spi_init(SPI2, &spi_init_struct);
        spi_enable(SPI2, TRUE);
        break;
    case SPI_Seq3:
        crm_periph_clock_enable(CRM_SPI3_PERIPH_CLOCK, TRUE);
        spi_init(SPI3, &spi_init_struct);
        spi_enable(SPI3, TRUE);
        break;
    case SPI_Seq4:
        crm_periph_clock_enable(CRM_SPI4_PERIPH_CLOCK, TRUE);
        spi_init(SPI4, &spi_init_struct);
        spi_enable(SPI4, TRUE);
        break;
    }
}

/**
 * @brief 注册回调函数
 * @param t_cb 发送回调函数指针
 * @param r_cb 接受回调函数指针
 * @retval none
 */
void bsp_register_callback(transmit_cb t_cb, received_cb r_cb)
{
    transmit_callback_func = t_cb;
    received_callback_func = r_cb;
}

/**
 * @brief SPI发送一个字节数据，同步接口
 * @param spix SPI序号
 * @param data 
 * @retval none
 */
void bsp_spi_transmit_byte(uint8_t spix, uint8_t data)
{
#if NO_USE_MICROLIB
	assert(spix > SPI_Seq3);
#endif
	
    bsp_spi_transmit_halfword(spix, data);
}

/**
 * @brief SPI发送2个字节数据，同步接口
 * @param spix SPI序号
 * @param data 待发送的数据
 * @retval none
 */
void bsp_spi_transmit_halfword(uint8_t spix, uint16_t data)
{
#if NO_USE_MICROLIB
	assert(spix > SPI_Seq3);
#endif
	
    if(spix == SPI_Seq1)
    {
        while(spi_i2s_flag_get(SPI1, SPI_I2S_TDBE_FLAG) == RESET);
        spi_i2s_data_transmit(SPI1, data);
    }
    else if(spix == SPI_Seq2)
    {
        while(spi_i2s_flag_get(SPI2, SPI_I2S_TDBE_FLAG) == RESET);
        spi_i2s_data_transmit(SPI2, data);
    }
    else if(spix == SPI_Seq3)
    {
        while(spi_i2s_flag_get(SPI3, SPI_I2S_TDBE_FLAG) == RESET);
        spi_i2s_data_transmit(SPI3, data);
    }
    else if(spix == SPI_Seq4)
    {
        while(spi_i2s_flag_get(SPI4, SPI_I2S_TDBE_FLAG) == RESET);
        spi_i2s_data_transmit(SPI4, data);
    }

    bsp_spi_transmit_done(spix);
}

/**
 * @brief 发送完成通知函数
 * @param spix SPI序号
 * @retval none
 */
void bsp_spi_transmit_done(uint8_t spix)
{
#if NO_USE_MICROLIB
	assert(spix > SPI_Seq3);
#endif
	
    if(transmit_callback_func != NULL)
        transmit_callback_func(spix);
}

/**
 * @brief SPI读取1个字节数据，同步接口
 * @param spix SPI序号
 * @retval uint8_t 返回读取到的数据
 */
uint8_t bsp_spi_received_byte(uint8_t spix)
{
#if NO_USE_MICROLIB
	assert(spix > SPI_Seq3);
#endif
	
    uint8_t retval = 0;
    retval = bsp_spi_received_halfword(spix);
    return retval;
}

/**
 * @brief SPI读取2个字节数据，同步接口
 * @param spix SPI序号
 * @retval uint16_t 返回读取到的数据
 */
uint16_t bsp_spi_received_halfword(uint8_t spix)
{
#if NO_USE_MICROLIB
	assert(spix > SPI_Seq3);
#endif
	
    uint16_t retval = 0;
    if(spix == SPI_Seq1)
    {
        while(spi_i2s_flag_get(SPI1, SPI_I2S_RDBF_FLAG) == RESET);
        retval = spi_i2s_data_receive(SPI1);
    }
    else if(spix == SPI_Seq2)
    {
        while(spi_i2s_flag_get(SPI2, SPI_I2S_RDBF_FLAG) == RESET);
        retval = spi_i2s_data_receive(SPI2);
    }
    else if(spix == SPI_Seq3)
    {
        while(spi_i2s_flag_get(SPI3, SPI_I2S_RDBF_FLAG) == RESET);
        retval = spi_i2s_data_receive(SPI3);
    }
    else if(spix == SPI_Seq4)
    {
        while(spi_i2s_flag_get(SPI4, SPI_I2S_RDBF_FLAG) == RESET);
        retval = spi_i2s_data_receive(SPI4);
    }

    bsp_spi_received_done(spix);
    
    return retval;
}

/**
 * @brief 接受完成通知函数
 * @param spix
 * @retval none
 */
void bsp_spi_received_done(uint8_t spix)
{
#if NO_USE_MICROLIB
	assert(spix > SPI_Seq3);
#endif
	
    if(received_callback_func != NULL)
        received_callback_func(spix);
}

/**
 * @brief 设置片选引脚状态
 * @param status 0：表示拉低，1表示拉高
 * @retval none
 */
void bsp_spi_set_cs(uint8_t status)
{
    if(status == ENABLE_CS)
    {
        gpio_bits_reset(SPI2_CS_PORT, SPI2_CS_PINMUX);
    }
    else
    {
        gpio_bits_set(SPI2_CS_PORT, SPI2_CS_PINMUX);
    }
}
