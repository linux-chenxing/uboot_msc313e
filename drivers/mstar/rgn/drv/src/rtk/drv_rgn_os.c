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

#include "drv_rgn_os.h"
#include "hal_gop.h"


unsigned long gGopDmaThd = 0;

bool _DrvGopTransId(DrvGopIdType_e eGopId, HalGopIdType_e *pHalId);
u8 DrvGopClkDisable(DrvGopIdType_e eGopId,bool bEn);
u8 DrvGopClkEnable(DrvGopIdType_e eGopId);
bool DrvRgnOsSetGopClkEnable(DrvGopIdType_e eGopId)
{
#ifndef CONFIG_CAM_CLK

    HalGopIdType_e enType;

    if(!_DrvGopTransId(eGopId,&enType))
    {
        return 0;
    }
    return HalGopSetClkEnable(enType);
#else
    return DrvGopClkEnable(eGopId);
#endif
}

bool DrvRgnOsSetGopClkDisable(DrvGopIdType_e eGopId,bool bEn)
{
#ifndef CONFIG_CAM_CLK
    HalGopIdType_e enType;

    DrvGopClkDisable(eGopId,bEn);
    if(!_DrvGopTransId(eGopId,&enType))
    {
        return 0;
    }
    return HalGopSetClkDisable(enType,bEn);
#else
    return DrvGopClkDisable(eGopId,bEn);
#endif
}
