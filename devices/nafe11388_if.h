#ifndef __NAFE11388_IF_H__
#define __NAFE11388_IF_H__

#include "at32f435_437.h"
#include "api_qmc_common.h"

// 通道掩码
#define MASK_CHANNEL_0    0x0001
#define MASK_CHANNEL_1    0x0002
#define MASK_CHANNEL_2    0x0004
#define ADC_CHANNEL_3     0x0008
#define MASK_CHANNEL_4    0x0010
#define MASK_CHANNEL_5    0x0020
#define MASK_CHANNEL_6    0x0040
#define MASK_CHANNEL_7    0x0080
#define MASK_CHANNEL_8    0x0100
#define MASK_CHANNEL_9    0x0200
#define MASK_CHANNEL_10   0x0400
#define MASK_CHANNEL_11   0x0800
#define MASK_CHANNEL_12   0x1000
#define MASK_CHANNEL_13   0x2000
#define MASK_CHANNEL_14   0x4000
#define MASK_CHANNEL_15   0x8000
#define MASK_CHANNEL_HALF 0x00ff
#define MASK_CHANNEL_ALL  0xFFff


/**
 * @brief 采样模式 
 */
typedef enum _SampleMode
{
    SampleMode_scsrBlock,
    SampleMode_sccrBlock,
    SampleMode_mcmrBlock,
    SampleMode_mccrBlock,
	SampleMode_mcsrBlock
} SampleMode_t;

/**
 * @brief 初始化nafe11388芯片
 * @param none
 * @retval none
 */
void nafe11388_init(void);

/**
 * @brief 启动转换
 * @param sample_mode [in]采样模式
 * @param start_channel [in]多通道模式下的起始通道，单通道模式下为需要读取的目的通道
 * @param channel_mask [in]读取的通道掩码
 * @param pdata [out]数据缓冲区地址
 * @retval 状态码
 */
qmc_status_t nafe11388_startSample(uint8_t sample_mode, uint8_t start_channel, uint16_t channel_mask, double *pdata);

#endif /* __NAFE11388_IF_H__ */
