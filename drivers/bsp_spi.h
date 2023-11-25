#ifndef __BSP_SPI_H__
#define __BSP_SPI_H__

#include "at32f435_437.h"

#ifndef NULL
#define NULL	0
#endif

/**
 * @brief SPI模式定义
 */
typedef enum _SPI_Mode
{
    SPI_Mode_0 = 0x00, // CPOL=0,CPHA=0
    SPI_Mode_1 = 0x01, // CPOL=0,CPHA=1
    SPI_Mode_2 = 0x02, // CPOL=1,CPHA=0
    SPI_Mode_3 = 0x03  // CPOL=1,CPHA=1
} SPI_Mode_t;

/**
 * @brief SPI速率
 */
typedef enum _SPI_Speed
{
    SPI_Speed_140Kbps = 0x00,
    SPI_Speed_280Kbps = 0x01,
    SPI_Speed_560Kbps = 0x02,
    SPI_Speed_1M1bps  = 0x03,
    SPI_Speed_2M2bps  = 0x04,
    SPI_Speed_4M5bps  = 0x05,
    SPI_Speed_9Mbps   = 0x06,
    SPI_Speed_18Mbps  = 0x07,
    SPI_Speed_36Mbps  = 0x08,
    SPI_Speed_48Mbps  = 0x09,
    SPI_Speed_72Mbps  = 0x0a
} SPI_Speed_t;

/**
 * @brief SPI序号
 */
typedef enum _SPI_Seq
{
    SPI_Seq1 = 0x00,
    SPI_Seq2 = 0x01,
    SPI_Seq3 = 0x02,
    SPI_Seq4 = 0x03
} SPI_Seq_t;

/**
 * @brief SPI工作类型
 */
typedef enum _SPI_Type
{
  SPI_Type_FULL_DUPLEX      = 0x00,
  SPI_Type_SIMPLEX_RX       = 0x01,
  SPI_Type_HALF_DUPLEX_RX   = 0x02,
  SPI_Type_HALF_DUPLEX_TX   = 0x03
} SPI_Type_t;

/**
 * @brief SPI主从模式
 */
typedef enum _SPI_Role
{
    SPI_Mode_Slave  = 0x00,
    SPI_Mode_Master = 0x01
} SPI_Role_t;

/**
 * @brief SPI数据位数
 */
typedef enum _SPI_Frame_Bitnum
{
  SPI_Frame_8BIT  = 0x00, /*!< 8-bit data frame format */
  SPI_Frame_16BIT = 0x01  /*!< 16-bit data frame format */
} SPI_Frame_Bitnum_t;

/**
 * @brief 传输字节序
 */
typedef enum _SPI_ByteOrder
{
  SPI_FRAME_LSB  = 0x00,
  SPI_FRAME_MSB  = 0x01
} SPI_ByteOrder_t;

/**
 * @brief 片选状态
 */
typedef enum _SPI_CS_Status
{
  ENABLE_CS  = 0x00,
  DISABLE_CS = 0x01
} SPI_CS_Status_t;

/**
 * @brief 回调函数指针
 */
typedef void (*transmit_cb) (uint8_t spix);
typedef void (*received_cb) (uint8_t spix);

/*public function****************************************************************/

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
void bsp_spi_config(uint8_t spix, uint8_t mode, uint8_t speed, uint8_t is_master, uint8_t type, uint8_t bitnum, uint8_t is_lsb);
/**
 * @brief 注册回调函数
 * @param t_cb 发送回调函数指针
 * @param r_cb 接受回调函数指针
 * @retval none
 */
void bsp_register_callback(transmit_cb t_cb, received_cb r_cb);

/**
 * @brief SPI发送一个字节数据，同步接口
 * @param spix SPI序号
 * @param data 
 * @retval none
 */
void bsp_spi_transmit_byte(uint8_t spix, uint8_t data);

/**
 * @brief SPI发送2个字节数据，同步接口
 * @param spix SPI序号
 * @param data 待发送的数据
 * @retval none
 */
void bsp_spi_transmit_halfword(uint8_t spix, uint16_t data);

/**
 * @brief 发送完成通知函数
 * @param spix SPI序号
 * @retval none
 */
void bsp_spi_transmit_done(uint8_t spix);

/**
 * @brief SPI读取1个字节数据，同步接口
 * @param spix SPI序号
 * @retval uint8_t 返回读取到的数据
 */
uint8_t bsp_spi_received_byte(uint8_t spix);

/**
 * @brief SPI读取2个字节数据，同步接口
 * @param spix SPI序号
 * @retval uint16_t 返回读取到的数据
 */
uint16_t bsp_spi_received_halfword(uint8_t spix);

/**
 * @brief 接受完成通知函数
 * @param spix
 * @retval none
 */
void bsp_spi_received_done(uint8_t spix);

/**
 * @brief 设置片选引脚状态
 * @param status 0：表示拉低，1表示拉高
 * @retval none
 */
void bsp_spi_set_cs(uint8_t status);

/*end of public function****************************************************************/


#endif /* __BSP_SPI_H__ */

