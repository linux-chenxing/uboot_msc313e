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


#ifndef __HAL_COVER_H__
#define __HAL_COVER_H__
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
    E_HAL_ISPSCL0_COVER = 0,
    E_HAL_ISPSCL1_COVER,
    E_HAL_ISPSCL2_COVER,
    E_HAL_ISPSCL3_COVER,
    E_HAL_ISPSCL4_COVER,
    E_HAL_ISPSCL5_COVER,
    E_HAL_DIP_COVER_ID,
    E_HAL_COVER_ID_NUM,
} HalCoverIdType_e;

typedef enum
{
    E_HAL_COVER_BWIN_ID_0 = 0,
    E_HAL_COVER_BWIN_ID_1 = 1,
    E_HAL_COVER_BWIN_ID_2 = 2,
    E_HAL_COVER_BWIN_ID_3 = 3,
    E_HAL_COVER_BWIN_ID_NUM = 4,
} HalCoverWinIdType_e;

typedef enum
{
    E_HAL_COVER_CMDQ_VPE_ID_0 = 0,
    E_HAL_COVER_CMDQ_DIVP_ID_1 = 1,
    E_HAL_COVER_CMDQ_ID_NUM = 1,
}HalCoverCmdqIdType_e;

#include "hal_cover_if.h"
#endif /* __HAL_COVER_H__ */
