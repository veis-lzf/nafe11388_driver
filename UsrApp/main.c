/**
  **************************************************************************
  * @file     main.c
  * @brief    main program
  **************************************************************************
  */

#include "at32f435_437_clock.h"
#include "nafe11388_if.h"



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

    // ���ڳ�ʼ�����ض���printf
    // TODO: Add your code here!

    // ��ʼ��������8��ͨ������
    nafe11388_init();

    // SCSRģʽ��ȡ
    status = nafe11388_startSample(SampleMode_scsrBlock, 0, MASK_CHANNEL_0, &(data_buf[0]));
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

    // MSMR��ȡ
    status = nafe11388_startSample(SampleMode_mcmrBlock, 0, MASK_CHANNEL_HALF, &(data_buf[0]));
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
