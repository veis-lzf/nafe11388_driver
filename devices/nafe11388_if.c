#include "nafe11388_if.h"
#include "bsp_nafe11388.h"
#include "bsp_spi.h"
#include <assert.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define CHANNEL_AMT				8u			/* Amount of channels used in AFE */
#define CONT_SAMPLE_AMT			1u			/* Amount of readings per channel */

#define V_REF					3.3		    /* Referential voltage */


/* System config */
static NAFE_sysConfig_t gs_SysConfig = {
    .adcResolutionCode = kNafeAdcResolution_24bits,			/* 24 bit ADC resolution */
    .triggerMode = kNafeTrigger_spiCmd,						/* AFE readings triggered by SPI commands */
    .readyPinSeqMode = kNafeReadyPinSeqMode_onConversion,	/* Data readings right after a conversion */
    .enabledChnMask = 0x00ff,                               /* Enable logical configurable channel for ADC conversion in Sequencer mode: 0 = disable, 1 = enable.CH15 is bit15, CH0 is bit0 */
};

/* Channel config array */
static NAFE_chnConfig_t gs_ChnConfig[CHANNEL_AMT] = {
    {
        .chnIndex = 0u,									/* Channel index */
        .inputSel = kNafeInputSel_hvsig,				/* Input type - HV signals: H = HV_AIP - HV_AIN */
        .hvAip = kNafeHvInputPos_ai1p,					/* TEMP_AFE */
        .hvAin = kNafeHvInputNeg_gnd,					/* GND */
        .gain = kNafeChnGain_0p8x,						/* Channel gain 0.8x */
        .dataRateCode = 4u,								/* Data rate code */
        .adcSinc = kNafeAdcSinc_sinc4,					/* Sinc type */
        .chDelayCode = 0u,								/* Programmable delay */
        .adcSettling = kNafeAdcSettling_singleCycle,	/* ADC settling type */
    },
    {
        .chnIndex = 1u,									/* Channel index */
        .inputSel = kNafeInputSel_hvsig,				/* Input type - HV signals: H = HV_AIP - HV_AIN */
        .hvAip = kNafeHvInputPos_gnd,					/* GND */
        .hvAin = kNafeHvInputNeg_ai1n,					/* TEMP_AFE1 */
        .gain = kNafeChnGain_0p8x,						/* Channel gain 0.8x */
        .dataRateCode = 4u,								/* Data rate code */
        .adcSinc = kNafeAdcSinc_sinc4,					/* Sinc type */
        .chDelayCode = 0u,								/* Programmable delay */
        .adcSettling = kNafeAdcSettling_singleCycle,	/* ADC settling type */
    },
    {
        .chnIndex = 2u,									/* Channel index */
        .inputSel = kNafeInputSel_hvsig,				/* Input type - HV signals: H = HV_AIP - HV_AIN */
        .hvAip = kNafeHvInputPos_ai2p,					/* TEMP_AFE */
        .hvAin = kNafeHvInputNeg_gnd,					/* GND */
        .gain = kNafeChnGain_0p8x,						/* Channel gain 0.8x */
        .dataRateCode = 4u,								/* Data rate code */
        .adcSinc = kNafeAdcSinc_sinc4,					/* Sinc type */
        .chDelayCode = 0u,								/* Programmable delay */
        .adcSettling = kNafeAdcSettling_singleCycle,	/* ADC settling type */
    },
    {
        .chnIndex = 3u,									/* Channel index */
        .inputSel = kNafeInputSel_hvsig,				/* Input type - HV signals: H = HV_AIP - HV_AIN */
        .hvAip = kNafeHvInputPos_gnd,					/* GND */
        .hvAin = kNafeHvInputNeg_ai2n,					/* TEMP_AFE1 */
        .gain = kNafeChnGain_0p8x,						/* Channel gain 0.8x */
        .dataRateCode = 4u,								/* Data rate code */
        .adcSinc = kNafeAdcSinc_sinc4,					/* Sinc type */
        .chDelayCode = 0u,								/* Programmable delay */
        .adcSettling = kNafeAdcSettling_singleCycle,	/* ADC settling type */
    },
    {
        .chnIndex = 4u,									/* Channel index */
        .inputSel = kNafeInputSel_hvsig,				/* Input type - HV signals: H = HV_AIP - HV_AIN */
        .hvAip = kNafeHvInputPos_ai3p,					/* TEMP_AFE */
        .hvAin = kNafeHvInputNeg_gnd,					/* GND */
        .gain = kNafeChnGain_0p8x,						/* Channel gain 0.8x */
        .dataRateCode = 4u,								/* Data rate code */
        .adcSinc = kNafeAdcSinc_sinc4,					/* Sinc type */
        .chDelayCode = 0u,								/* Programmable delay */
        .adcSettling = kNafeAdcSettling_singleCycle,	/* ADC settling type */
    },
    {
        .chnIndex = 5u,									/* Channel index */
        .inputSel = kNafeInputSel_hvsig,				/* Input type - HV signals: H = HV_AIP - HV_AIN */
        .hvAip = kNafeHvInputPos_gnd,					/* GND */
        .hvAin = kNafeHvInputNeg_ai3n,					/* TEMP_AFE1 */
        .gain = kNafeChnGain_0p8x,						/* Channel gain 0.8x */
        .dataRateCode = 4u,								/* Data rate code */
        .adcSinc = kNafeAdcSinc_sinc4,					/* Sinc type */
        .chDelayCode = 0u,								/* Programmable delay */
        .adcSettling = kNafeAdcSettling_singleCycle,	/* ADC settling type */
    },
    {
        .chnIndex = 6u,									/* Channel index */
        .inputSel = kNafeInputSel_hvsig,				/* Input type - HV signals: H = HV_AIP - HV_AIN */
        .hvAip = kNafeHvInputPos_ai4p,					/* TEMP_AFE */
        .hvAin = kNafeHvInputNeg_gnd,					/* GND */
        .gain = kNafeChnGain_0p8x,						/* Channel gain 0.8x */
        .dataRateCode = 4u,								/* Data rate code */
        .adcSinc = kNafeAdcSinc_sinc4,					/* Sinc type */
        .chDelayCode = 0u,								/* Programmable delay */
        .adcSettling = kNafeAdcSettling_singleCycle,	/* ADC settling type */
    },
    {
        .chnIndex = 7u,									/* Channel index */
        .inputSel = kNafeInputSel_hvsig,				/* Input type - HV signals: H = HV_AIP - HV_AIN */
        .hvAip = kNafeHvInputPos_gnd,					/* GND */
        .hvAin = kNafeHvInputNeg_ai4n,					/* TEMP_AFE1 */
        .gain = kNafeChnGain_0p8x,						/* Channel gain 0.8x */
        .dataRateCode = 4u,								/* Data rate code */
        .adcSinc = kNafeAdcSinc_sinc4,					/* Sinc type */
        .chDelayCode = 0u,								/* Programmable delay */
        .adcSettling = kNafeAdcSettling_singleCycle,	/* ADC settling type */
    },
};

static NAFE_devHdl_t nafe_dev = 
{
    .devAddr = 0u,								/* SPI device index */
    .sysConfig = &gs_SysConfig,					/* NAFE system config */
    .chConfig = gs_ChnConfig,					/* NAFE channel config */
    .currentSampleMode = kNafeSampleMode_none,	/* NAFE sample mode */
#if USE_SPI_HANDLE
    .halHdl = NULL,			                    /* FLEXIO SPI handle */
#endif
    .currentChnIndex = 0						/* Current channel index */
};

static double gs_Result = 0; /* Change to array of length [CH_AMT * CONT_SAMPLE_AMT] for SCCR/MCCR/MCMR */

/* Transfer handle */
static NAFE_xferHdl_t gs_XferHdl = {
	.sampleMode = kNafeSampleMode_none,			/* NAFE sample mode */
	.pResult = &gs_Result,						/* Sample result */
	.contSampleAmt = CONT_SAMPLE_AMT,			/* Amount of readings per channel */
	.chnAmt = CHANNEL_AMT,						/* Amount of channels */
	.requestedChn = 0							/* Requested channel index */
};



/**
 * @brief 初始化SYNC，INTB，DRDY三个引脚
 * @retval none
 */
static void nafe1138_gpio_config(void)
{
    // ADC_DRDY是PH3,ADC_SYNC是PD9,ADC_INTB是PD10
	gpio_init_type gpio_initstructure;

    crm_periph_clock_enable(CRM_GPIOH_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_GPIOD_PERIPH_CLOCK, TRUE);
    gpio_default_para_init(&gpio_initstructure);

    // ADC_INTB是PD10
    /*
    INTB (active low) is an output pin that notifies when a global alarm interrupt has occurred. The driver can be
    programmed as open-drain with internal pullup or CMOS output.
    */
    gpio_initstructure.gpio_pull           = GPIO_PULL_UP;
    gpio_initstructure.gpio_mode           = GPIO_MODE_INPUT;
    gpio_initstructure.gpio_pins           = GPIO_PINS_10;
    gpio_init(GPIOD, &gpio_initstructure);

    // ADC_SYNC是PD9
    /*
    SYNC is an external pin used to synchronize the data conversion to external events. The user can start any
    conversion mode either by using SPI command or asserting low to high transition on the SYNC pin given that
    the SYS_CONFIG0.ADC_SYNC bit = 1.
    */
    gpio_initstructure.gpio_out_type       = GPIO_OUTPUT_PUSH_PULL;
    gpio_initstructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
    gpio_initstructure.gpio_pull           = GPIO_PULL_UP;
    gpio_initstructure.gpio_mode           = GPIO_MODE_OUTPUT;
    gpio_initstructure.gpio_pins           = GPIO_PINS_9;
    gpio_init(GPIOD, &gpio_initstructure);

    // ADC_DRDY是PH3，
    /*
    DRDY (rising edge) is an output pin that indicates the conversion status. DRDY is driven to high when the new conversion result is 
    stored in the SPI buffer and ready for reading.
    DRDY pulse stays high for 2 x T_sys_clk if SYS_CONFIG0.DRDY_PWDT bit = 0. Otherwise, the pulse stays
    high for 8 x T_sys_clk if DRDY_PWDT bit = 1.
    */
    gpio_initstructure.gpio_out_type       = GPIO_OUTPUT_OPEN_DRAIN;
    gpio_initstructure.gpio_pull           = GPIO_PULL_NONE;
    gpio_initstructure.gpio_mode           = GPIO_MODE_INPUT;
    gpio_initstructure.gpio_pins           = GPIO_PINS_3;
    gpio_init(GPIOD, &gpio_initstructure);
}

/*public function*************************************************************************************/

/**
 * @brief 初始化nafe11388芯片
 * @param none
 * @retval none
 */
void nafe11388_init(void)
{
    // 初始化相关GPIO(当前驱动未使用到硬件GPIO，使用软件命令进行连续转换)
//	nafe1138_gpio_config();

    // 初始化SPI
    bsp_spi_config(
        SPI_Seq2, 
        SPI_Mode_1, 
        SPI_Speed_4M5bps, 
        SPI_Mode_Master, 
        SPI_Type_FULL_DUPLEX, 
        SPI_Frame_8BIT,
        SPI_FRAME_MSB
        );

    // 配置ADC芯片
    NAFE_init(&nafe_dev, &gs_XferHdl);
}

/**
 * @brief 启动转换
 * @param sample_mode [in]采样模式
 * @param start_channel [in]多通道模式下的起始通道，单通道模式下为需要读取的目的通道
 * @param channel_mask [in]读取的通道掩码
 * @param pdata [out]数据缓冲区地址
 * @retval 状态码
 */
qmc_status_t nafe11388_startSample(uint8_t sample_mode, uint8_t start_channel, uint16_t channel_mask, double *pdata)
{
    qmc_status_t status = kStatus_QMC_Err;
    // 参数校验
#if NO_USE_MICROLIB
    assert(channel_mask > MASK_CHANNEL_ALL);
	assert(sample_mode > SampleMode_mcsrBlock);
    assert(pdata == NULL);
#endif
    // 设置采样模式
    switch(sample_mode)
    {
        case SampleMode_mccrBlock:
           gs_XferHdl.sampleMode = kNafeSampleMode_mccrBlock;
           break;
        case SampleMode_mcmrBlock:
            gs_XferHdl.sampleMode = kNafeSampleMode_mcmrBlock;
            break;
        case SampleMode_sccrBlock:
            gs_XferHdl.sampleMode = kNafeSampleMode_sccrBlock;
            break;
        case SampleMode_scsrBlock:
            gs_XferHdl.sampleMode = kNafeSampleMode_scsrBlock;
            break;
        case SampleMode_mcsrBlock:
            gs_XferHdl.sampleMode = kNafeSampleMode_mcsrBlock;
            break;
    }

    nafe_dev.sysConfig->enabledChnMask = channel_mask;
    gs_XferHdl.requestedChn = start_channel;
    gs_XferHdl.pResult = pdata;

    status = (qmc_status_t)NAFE_startSample(&nafe_dev, &gs_XferHdl);

    return status;
}

/*end of public function*************************************************************************************/
