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


#ifndef __HAL_OSD_H__
#define __HAL_OSD_H__
#include "hal_rgn_common.h"

//=============================================================================
// Defines
//=============================================================================
#ifndef bool
#define  bool   unsigned char
#endif
//=============================================================================
// enum
//=============================================================================
typedef enum
{
    E_HAL_OSD_PORT0,
    E_HAL_OSD_PORT1,
    E_HAL_OSD_PORT2,
    E_HAL_OSD_DIP,
    E_HAL_OSD_LDC,
    E_HAL_OSD_PORT3,
    E_HAL_OSD_PORT4,
    E_HAL_OSD_PORT5,
    E_HAL_OSD_DISP0_CUR,
    E_HAL_OSD_DISP0_UI,
    E_HAL_OSD_DISP1_CUR,
    E_HAL_OSD_DISP1_UI,
    E_HAL_OSD_ID_MAX
}HalOsdIdType_e;

typedef enum
{
    E_HAL_GOP_AE0 = 0,
    E_HAL_GOP_AE1,
    E_HAL_GOP_AE2,
    E_HAL_GOP_AE3,
    E_HAL_GOP_AE4,
    E_HAL_GOP_AE5,
    E_HAL_GOP_AE6,
    E_HAL_GOP_AE7
} HalOsdColorInvWindowIdType_e;
#include "hal_osd_if.h"
#endif /* __HAL_GOP_H__ */
