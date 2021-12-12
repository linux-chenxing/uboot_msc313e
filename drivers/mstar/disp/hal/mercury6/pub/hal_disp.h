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

#ifndef _HAL_DISP_H_
#define _HAL_DISP_H_

//-------------------------------------------------------------------------------------------------
//  Defines & Macro
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  structure & Enum
//-------------------------------------------------------------------------------------------------
typedef struct
{
    bool bTimingUpdate[HAL_DISP_DEVICE_MAX];
    bool bClkGpCtrl[HAL_DISP_DEVICE_MAX];
    void *pvCmdqCtx[HAL_DISP_UTILITY_CMDQ_NUM];
    void *pvClkGpCtrl[HAL_DISP_DEVICE_MAX];
}HalDispHwContain_t;
//-------------------------------------------------------------------------------------------------
//  Prototype
//-------------------------------------------------------------------------------------------------

#ifdef __HAL_DISP_C__
#define INTERFACE
#else
#define INTERFACE extern
#endif

INTERFACE bool HalDispGetCmdqByCtx(void *pCtx, void **pCmdqCtx);

#undef INTERFACE

#endif
