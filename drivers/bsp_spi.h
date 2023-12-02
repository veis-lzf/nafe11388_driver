#ifndef __BSP_SPI_H__
#define __BSP_SPI_H__

#include "at32f435_437.h"

#ifndef NULL
#define NULL	0
#endif

/**
 * @brief SPIģʽ����
 */
typedef enum _SPI_Mode
{
    SPI_Mode_0 = 0x00, // CPOL=0,CPHA=0
    SPI_Mode_1 = 0x01, // CPOL=0,CPHA=1
    SPI_Mode_2 = 0x02, // CPOL=1,CPHA=0
    SPI_Mode_3 = 0x03  // CPOL=1,CPHA=1
} SPI_Mode_t;

/**
 * @brief SPI����   APB1=120M
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
 * @brief SPI���
 */
typedef enum _SPI_Seq
{
    SPI_Seq1 = 0x00,
    SPI_Seq2 = 0x01,
    SPI_Seq3 = 0x02,
    SPI_Seq4 = 0x03
} SPI_Seq_t;

/**
 * @brief SPI��������
 */
typedef enum _SPI_Type
{
  SPI_Type_FULL_DUPLEX      = 0x00,
  SPI_Type_SIMPLEX_RX       = 0x01,
  SPI_Type_HALF_DUPLEX_RX   = 0x02,
  SPI_Type_HALF_DUPLEX_TX   = 0x03
} SPI_Type_t;

/**
 * @brief SPI����ģʽ
 */
typedef enum _SPI_Role
{
    SPI_Mode_Slave  = 0x00,
    SPI_Mode_Master = 0x01
} SPI_Role_t;

/**
 * @brief SPI����λ��
 */
typedef enum _SPI_Frame_Bitnum
{
  SPI_Frame_8BIT  = 0x00, /*!< 8-bit data frame format */
  SPI_Frame_16BIT = 0x01  /*!< 16-bit data frame format */
} SPI_Frame_Bitnum_t;

/**
 * @brief �����ֽ���
 */
typedef enum _SPI_ByteOrder
{
  SPI_FRAME_LSB  = 0x00,
  SPI_FRAME_MSB  = 0x01
} SPI_ByteOrder_t;

/**
 * @brief Ƭѡ״̬
 */
typedef enum _SPI_CS_Status
{
  ENABLE_CS  = 0x00,
  DISABLE_CS = 0x01
} SPI_CS_Status_t;

/**
 * @brief �ص�����ָ��
 */
typedef void (*transmit_cb) (uint8_t spix);
typedef void (*received_cb) (uint8_t spix);

/*public function****************************************************************/

/**
 * @brief SPI���ó�ʼ��
 * @param spix SPI��ţ���@SPI_Seq_t
 * @param mode SPI����ģʽ����@SPI_Mode_t
 * @param speed SPI���ʣ���@SPI_Speed_t
 * @param is_master ����ģʽ����ģʽ��1����ģʽ��0����@SPI_Role_t
 * @param type SPI�������ͣ��磺ȫ˫������˫������@SPI_Type_t
 * @param bitnum SPI����λ����@SPI_Frame_Bitnum_t
 * @param is_lsb �ֽ���0��ʾ�ȷ�LSB��1��ʾ�ȷ�MSB��@SPI_ByteOrder_t
 * @retval none
 */
void bsp_spi_config(uint8_t spix, uint8_t mode, uint8_t speed, uint8_t is_master, uint8_t type, uint8_t bitnum, uint8_t is_lsb);
/**
 * @brief ע��ص�����
 * @param t_cb ���ͻص�����ָ��
 * @param r_cb ���ܻص�����ָ��
 * @retval none
 */
void bsp_register_callback(transmit_cb t_cb, received_cb r_cb);

/**
 * @brief �������֪ͨ����
 * @param spix SPI���
 * @retval none
 */
void bsp_spi_transmit_done(uint8_t spix);


/**
 * @brief ����Ƭѡ����״̬
 * @param status 0����ʾ���ͣ�1��ʾ����
 * @retval none
 */
void bsp_spi_set_cs(uint8_t status);


/*
���������д�����Ͷ�������ͬ����ɵġ�
���ֻ����д����������ֻ����Խ��յ����ֽڣ�
��֮��������Ҫ��ȡ�ӻ���һ���ֽڣ��ͱ��뷢��һ�����ֽ��������ӻ��Ĵ��䡣
*/
/**
 * @brief SPI��ȡ��д��һ���ֵ����ݣ���ȡʱ����0xffff
 * @param spix SPI���
 * @param TxData �����͵�����
 * @retval none
 */
uint16_t SPI_ReadWriteHalfWord(uint8_t spix, uint16_t TxData);

/**
 * @brief SPI��ȡ��д��һ���ֽڵ����ݣ���ȡʱ����0xff
 * @param spix SPI���
 * @param TxData �����͵�����
 * @retval none
 */
uint8_t SPI_ReadWriteByte(uint8_t spix, uint8_t TxData);

/*end of public function****************************************************************/


#endif /* __BSP_SPI_H__ */

