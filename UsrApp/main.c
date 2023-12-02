/**
  **************************************************************************
  * @file     main.c
  * @brief    main program
  **************************************************************************
  */

#include "at32f435_437_clock.h"
#include "nafe11388_if.h"
#include "bsp_spi.h"

#define USE_SPI_TEST	0

#if USE_SPI_TEST

#define BUFFER_SIZE                      32

uint8_t spi2_buffer_tx[BUFFER_SIZE] = {0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,

																				0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x60,

																				0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,

																				0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70};

uint8_t spi2_buffer_rx[BUFFER_SIZE];
uint32_t tx_index = 0, rx_index = 0;

#endif


/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
    qmc_status_t status = kStatus_QMC_Err;
    double data_buf[8] = {0};

    system_clock_config();

    // 串口初始化，重定向printf
    // TODO: Add your code here!
#if USE_SPI_TEST
		bsp_spi_config(
        SPI_Seq2, 
        SPI_Mode_1, 
        SPI_Speed_2M2bps, 
        SPI_Mode_Master, 
        SPI_Type_FULL_DUPLEX, 
        SPI_Frame_8BIT, 
        SPI_FRAME_MSB
        );
		
		bsp_spi_set_cs(ENABLE_CS);
		
		for(int i = 0; i < BUFFER_SIZE; i++)
		{
				bsp_spi_transmit_byte(SPI_Seq2, spi2_buffer_tx[i]);
				//(void)bsp_spi_received_byte(SPI_Seq2); // 空读
				
		}
		bsp_spi_set_cs(DISABLE_CS);
		
#endif
    // 初始化，配置8个通道参数
    nafe11388_init();
		
    // SCSR模式读取
    status = nafe11388_startSample(SampleMode_scsrBlock, 0, MASK_CHANNEL_0, &(data_buf[0]));
//		status = nafe11388_startSample(SampleMode_scsrBlock, 1, MASK_CHANNEL_1, &(data_buf[1]));
//		status = nafe11388_startSample(SampleMode_scsrBlock, 2, MASK_CHANNEL_2, &(data_buf[2]));
//		status = nafe11388_startSample(SampleMode_scsrBlock, 3, MASK_CHANNEL_3, &(data_buf[3]));
//		status = nafe11388_startSample(SampleMode_scsrBlock, 4, MASK_CHANNEL_4, &(data_buf[4]));
//		status = nafe11388_startSample(SampleMode_scsrBlock, 5, MASK_CHANNEL_5, &(data_buf[5]));
//		status = nafe11388_startSample(SampleMode_scsrBlock, 6, MASK_CHANNEL_6, &(data_buf[6]));
//		status = nafe11388_startSample(SampleMode_scsrBlock, 7, MASK_CHANNEL_7, &(data_buf[7]));
    if(status != kStatus_QMC_Ok)
    {
        // TODO: Add your code here!
        // for retry
    }
    else
    {
    #ifdef DEBUG_ENABLE
        printf("adc value:%lf\r\n", data_buf[0]);
    #endif
    }

    // MSMR读取
    status = nafe11388_startSample(SampleMode_mcmrBlock, 0, MASK_CHANNEL_HALF, (data_buf));
    if(status != kStatus_QMC_Ok)
    {
        // TODO: Add your code here!
        // for retry
    }
    else
    {
    #ifdef DEBUG_ENABLE
        for (uint16_t i = 0; i < 8; i++)
        {
           printf("adc channel%d value:%lf\r\n", i, data_buf[0]);
        }
    #endif
    }

    while(1)
    {
		
    }
}

/**
  * @}
  */

/**
  * @}
  */
