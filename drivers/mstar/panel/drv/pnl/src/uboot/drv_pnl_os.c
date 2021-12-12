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

#define _DRV_PNL_OS_C_
#include <common.h>
#include <command.h>
#include <config.h>
#include <malloc.h>
#include <stdlib.h>

void * DrvPnlOsMemAlloc(u32 u32Size)
{
    return malloc(u32Size);
}

void DrvPnlOsMemRelease(void *pPtr)
{
    free(pPtr);
}

void DrvPnlOsMsSleep(u32 u32Msec)
{
    mdelay(u32Msec);
}

void DrvPnlOsUsSleep(u32 u32Usec)
{
    udelay(u32Usec);
}


bool DrvPnlOsPadMuxActive(void)
{
    return 0;
}

int DrvPnlOsPadMuxGetMode(void)
{
    return 0;
}

int DrvPnlOsGetMode(u16 u16LinkType, u16 u16OutputFormatBitMode)
{
    return 0;
}

bool DrvPnlOsSetDeviceNode(void *pPlatFormDev)
{
    return 0;
}

bool DrvPnlOsSetClkOn(void *pClkRate, u32 u32ClkRateSize)
{
    return 0;
}

bool DrvPnlOsSetClkOff(void *pClkRate, u32 u32ClkRateSize)
{
    return 0;
}
