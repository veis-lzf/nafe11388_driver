#ifndef _API_QMC_COMMON_H_
#define _API_QMC_COMMON_H_

#include <stdint.h>

#define assert_platfrom(a)

/*******************************************************************************
 * Definitions => Event bits
 ******************************************************************************/

/* Input & Button Status */
#define QMC_IOEVENT_BTN1_PRESSED  (1 <<  0)
#define QMC_IOEVENT_BTN1_RELEASED (1 <<  1)
#define QMC_IOEVENT_BTN2_PRESSED  (1 <<  2)
#define QMC_IOEVENT_BTN2_RELEASED (1 <<  3)
#define QMC_IOEVENT_BTN3_PRESSED  (1 <<  4)
#define QMC_IOEVENT_BTN3_RELEASED (1 <<  5)
#define QMC_IOEVENT_BTN4_PRESSED  (1 <<  6)
#define QMC_IOEVENT_BTN4_RELEASED (1 <<  7)
#define QMC_IOEVENT_INPUT0_HIGH   (1 <<  8)
#define QMC_IOEVENT_INPUT0_LOW    (1 <<  9)
#define QMC_IOEVENT_INPUT1_HIGH   (1 << 10)
#define QMC_IOEVENT_INPUT1_LOW    (1 << 11)
#define QMC_IOEVENT_INPUT2_HIGH   (1 << 12)
#define QMC_IOEVENT_INPUT2_LOW    (1 << 13)
#define QMC_IOEVENT_INPUT3_HIGH   (1 << 14)
#define QMC_IOEVENT_INPUT3_LOW    (1 << 15)
#define QMC_IOEVENT_INPUT4_HIGH   (1 << 16)
#define QMC_IOEVENT_INPUT4_LOW    (1 << 17)
#define QMC_IOEVENT_INPUT5_HIGH   (1 << 18)
#define QMC_IOEVENT_INPUT5_LOW    (1 << 19)
#define QMC_IOEVENT_INPUT6_HIGH   (1 << 20)
#define QMC_IOEVENT_INPUT6_LOW    (1 << 21)
#define QMC_IOEVENT_INPUT7_HIGH   (1 << 22)
#define QMC_IOEVENT_INPUT7_LOW    (1 << 23)

/* System Status */
#define QMC_SYSEVENT_LIFECYCLE_Commissioning     (1 <<  0)
#define QMC_SYSEVENT_LIFECYCLE_Operational       (1 <<  1)
#define QMC_SYSEVENT_LIFECYCLE_Error             (1 <<  2)
#define QMC_SYSEVENT_LIFECYCLE_Maintenance       (1 <<  3)
#define QMC_SYSEVENT_LIFECYCLE_Decommissioning   (1 <<  4)
#define QMC_SYSEVENT_FAULT_Motor1                (1 <<  5)
#define QMC_SYSEVENT_FAULT_Motor2                (1 <<  6)
#define QMC_SYSEVENT_FAULT_Motor3                (1 <<  7)
#define QMC_SYSEVENT_FAULT_Motor4                (1 <<  8)
#define QMC_SYSEVENT_FAULT_System                (1 <<  9)
#define QMC_SYSEVENT_FWUPDATE_RestartRequired    (1 << 10)
#define QMC_SYSEVENT_FWUPDATE_VerifyMode         (1 << 11)
#define QMC_SYSEVENT_CONFIGURATION_ConfigChanged (1 << 12)
#define QMC_SYSEVENT_ANOMALYDETECTION_Audio      (1 << 13)
#define QMC_SYSEVENT_ANOMALYDETECTION_Current    (1 << 14)
#define QMC_SYSEVENT_SHUTDOWN_PowerLoss          (1 << 15)
#define QMC_SYSEVENT_SHUTDOWN_WatchdogReset      (1 << 16)
#define QMC_SYSEVENT_MEMORY_SdCardAvailable      (1 << 17)
#define QMC_SYSEVENT_NETWORK_TsnSyncLost         (1 << 18)
#define QMC_SYSEVENT_NETWORK_NoLink              (1 << 19)
#define QMC_SYSEVENT_LOG_FlashError              (1 << 20)
#define QMC_SYSEVENT_LOG_LowMemory               (1 << 21)
#define QMC_SYSEVENT_LOG_MessageLost             (1 << 22)

/*******************************************************************************
 * Definitions => Enumerations
 ******************************************************************************/

/*! @brief Status group numbers. */
enum _status_groups
{
    kStatusGroup_Generic = 0,                 /*!< Group number for generic status codes. */
    kStatusGroup_FLEXIO_SPI = 5,              /*!< Group number for FLEXIO SPI status codes. */
	kStatusGroup_QMC = 158,                   /*!< Group number for QuadMotorControl specific status codes. */

};

/*! @brief Construct a status code value from a group and code number. */
#define MAKE_STATUS(group, code) ((((group)*100) + (code)))

/*! \public
 * @brief Generic status return codes.
 */
enum
{
    kStatus_Success = MAKE_STATUS(kStatusGroup_Generic, 0),  /*!< Generic status for Success. */
    kStatus_Fail = MAKE_STATUS(kStatusGroup_Generic, 1),      /*!< Generic status for Fail. */
    kStatus_ReadOnly = MAKE_STATUS(kStatusGroup_Generic, 2),    /*!< Generic status for read only failure. */
    kStatus_OutOfRange = MAKE_STATUS(kStatusGroup_Generic, 3),   /*!< Generic status for out of range access. */
    kStatus_InvalidArgument = MAKE_STATUS(kStatusGroup_Generic, 4),   /*!< Generic status for invalid argument check. */
    kStatus_Timeout = MAKE_STATUS(kStatusGroup_Generic, 5),   /*!< Generic status for timeout. */
    kStatus_NoTransferInProgress = MAKE_STATUS(kStatusGroup_Generic, 6),   /*!< Generic status for no transfer in progress. */
};


/*!
 * @brief Common API return values.
 */
typedef enum _qmc_status
{
    kStatus_QMC_Ok            = kStatus_Success,                         /*!< Operation was successful; no errors occurred. */
    kStatus_QMC_Err           = kStatus_Fail,                            /*!< General error; not further specified. */
    kStatus_QMC_ErrRange      = kStatus_OutOfRange,                      /*!< Error in case input argument is out of range e.g. array out of bounds, value not defined in enumeration. */
    kStatus_QMC_ErrArgInvalid = kStatus_InvalidArgument,                 /*!< Argument invalid e.g. wrong type, NULL pointer, etc. */
    kStatus_QMC_Timeout       = kStatus_Timeout,                         /*!< Operation was not carried out because a timeout occurred. */
    kStatus_QMC_ErrBusy        = MAKE_STATUS(kStatusGroup_QMC, 7),       /*!< Error because device or resource is busy. */
    kStatus_QMC_ErrMem         = MAKE_STATUS(kStatusGroup_QMC, 8),       /*!< Out of memory error. */
    kStatus_QMC_ErrSync        = MAKE_STATUS(kStatusGroup_QMC, 9),       /*!< Synchronization error when accessing shared resources. */
    kStatus_QMC_ErrNoMsg       = MAKE_STATUS(kStatusGroup_QMC, 10),      /*!< Error if a message shall be retrieved (e.g. from a message queue) but is not available. */
    kStatus_QMC_ErrInterrupted = MAKE_STATUS(kStatusGroup_QMC, 11),      /*!< Operation could not be executed because it was interrupted. */
    kStatus_QMC_ErrNoBufs      = MAKE_STATUS(kStatusGroup_QMC, 12),      /*!< Given buffer space is not sufficient. */
    kStatus_QMC_ErrInternal    = MAKE_STATUS(kStatusGroup_QMC, 13),      /*!< An internal error occurred. */
    kStatus_QMC_ErrSignatureInvalid = MAKE_STATUS(kStatusGroup_QMC, 14)  /*!< A signature check failed. */
} qmc_status_t;


/*! @brief Type used for all status and error return values. */
typedef int32_t status_t;

/*******************************************************************************
 * Definitions => Other
 ******************************************************************************/

/* TODO: put fractional types here */

#endif /* _API_QMC_COMMON_H_ */
