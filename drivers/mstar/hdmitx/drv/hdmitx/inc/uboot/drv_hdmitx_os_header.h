/* SigmaStar trade secret */
/* Copyright (c) [2019~2020] SigmaStar Technology.
 All rights reserved.

 Unless otherwise stipulated in writing, any and all information contained
herein regardless in any format shall remain the sole proprietary of
SigmaStar and be kept in strict confidence
(SigmaStar Confidential Information) by the recipient.
Any unauthorized act including without limitation unauthorized disclosure,
copying, use, reproduction, sale, distribution, modification, disassembling,
reverse engineering and compiling of the contents of SigmaStar Confidential
Information is unlawful and strictly prohibited. SigmaStar hereby reserves the
rights to any and all damages, losses, costs and expenses resulting therefrom.
*/

#ifndef _DRV_HDMITX_OS_HEADER_H_
#define _DRV_HDMITX_OS_HEADER_H_


#include <common.h>
#include <command.h>
#include <config.h>
#include <malloc.h>
#include <stdlib.h>


//-------------------------------------------------------------------------------------------------
//  Defines & Macro
//-------------------------------------------------------------------------------------------------
#define CAM_OS_EVENTGROUP            0

#define HDMITX_SPRINTF_STRCAT(str, _fmt, _args...) \
    do {                                   \
        char tmpstr[1024];                 \
        sprintf(tmpstr, _fmt, ## _args);   \
        strcat((char *)str, tmpstr);               \
    }while(0)

#define TICK_PER_ONE_MS             (1) //Note: confirm Kernel fisrt
#define HDMITX_OS_WAIT_FOREVER      (0xffffff00/TICK_PER_ONE_MS)

#define HDMITX_OS_MMIO_NONEPM_BASE  0x1F000000UL
#define HDMITX_OS_MMIO_NONEPM_SIZE  0x00200000UL

#define HDMITX_OS_MMIO_PM_BASE      0x1F000000UL
#define HDMITX_OS_MMIO_PM_SIZE      0x00400000UL
//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------



// Timer
typedef struct
{
    MS_S32  s32Id;
    bool bCycle;
    bool bStop;
    MS_U32 u32TimerOut;
    void (*pfnFunc)(MS_U32 stTimer, MS_U32 u32Data);
}DrvHdmitxOsTimerConfig_t;


// Mutex

typedef struct
{
    MS_S32 s32Id;
}DrvHdmitxOsMutexConfig_t;


// Semaphore

typedef struct
{
    MS_S32 s32Id;
}DrvHdmitxOsSemConfig_t;


// Event
typedef enum
{
    E_DRV_HDMITX_OS_EVENT_MD_AND,                      ///< Specify all of the requested events are require.
    E_DRV_HDMITX_OS_EVENT_MD_OR,                       ///< Specify any of the requested events are require.
    E_DRV_HDMITX_OS_EVENT_MD_AND_CLEAR,                ///< Specify all of the requested events are require. If the request are successful, clear the event.
    E_DRV_HDMITX_OS_EVENT_MD_OR_CLEAR,                 ///< Specify any of the requested events are require. If the request are successful, clear the event.
} DrvHdmitxOsEventWaitMoodeType_e;

#if CAM_OS_EVENTGROUP
typedef struct
{
    bool          bUsed;
    u32           u32EventGroup;
} DrvHdmitxOsEventGroupInfoConfig_t;
#else

struct work_data
{
    MS_U32                     u32EventFlag;
};


typedef struct
{
    MS_BOOL                     bUsed;
    MS_U32                      u32Waiting;
    MS_U32                      u32EventGroup;
    // pthread_cond_t              stSemaphore; // ?????????????
} DrvHdmitxOsEventGroupInfoConfig_t;
#endif

// Interrupt

typedef void ( *InterruptCallBack) (MS_U32 u32IntNum);
typedef void (*InterruptCb) (int irq, void *dev_instance);

// Task

typedef void* (*TaskEntryCb)(void *argv);


typedef struct
{
    MS_S32 s32Id;
} DrvHdmitxOsTaskConfig_t;


// MMIO
typedef enum
{
    E_HDMITX_OS_MMIO_PM,
    E_HDMITX_OS_MMIO_NONEPM,
} DrvHdmitxOsMmioType_e;

#endif
