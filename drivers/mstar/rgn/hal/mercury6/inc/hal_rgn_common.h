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


#ifndef __HAL_RGN_COMMON_H__
#define __HAL_RGN_COMMON_H__
#include "mhal_common.h"
#include "mhal_cmdq.h"
#include "drv_rgn_os_header.h"
#include "rgn_debug.h"


//=============================================================================
// enum
//=============================================================================
typedef enum
{
    E_HAL_RGN_CMDQ_VPE_ID_0 = 0,
    E_HAL_RGN_CMDQ_DIVP_ID_1,
    E_HAL_RGN_CMDQ_LDC_ID_2,
    E_HAL_RGN_CMDQ_DISP_ID_3,
    E_HAL_RGN_CMDQ_ID_NUM,
}HalRgnCmdqIdType_e;
typedef enum
{
    E_HAL_RGN_OSD_FLAG_PORT0     = 0x1,
    E_HAL_RGN_OSD_FLAG_PORT1     = 0x2,
    E_HAL_RGN_OSD_FLAG_PORT2     = 0x4,
    E_HAL_RGN_OSD_FLAG_PORT3     = 0x8,
    E_HAL_RGN_OSD_FLAG_DIP       = 0x10,
    E_HAL_RGN_OSD_FLAG_LDC       = 0x20,
    E_HAL_RGN_OSD_FLAG_PORT4     = 0x40,
    E_HAL_RGN_OSD_FLAG_PORT5     = 0x80,
    E_HAL_RGN_OSD_FLAG_DISP0_CUR = 0x100,
    E_HAL_RGN_OSD_FLAG_DISP0_UI  = 0x200,
    E_HAL_RGN_OSD_FLAG_DISP1_CUR = 0x400,
    E_HAL_RGN_OSD_FLAG_DISP1_UI  = 0x800,
}HalRgnOsdFlag_e;

//=============================================================================
// struct
//=============================================================================
//=============================================================================
// Defines
//=============================================================================
#define HAL_RGN_GOP_NUM 10
#define HAL_RGN_GOP_GWIN_NUM 8
//#define HAL_RGN_OSD_REAL_NUM 12
#define HAL_RGN_DISP_GWIN_CNT 1
//#define RIU_32_EN 1

#ifndef bool
#define  bool   unsigned char
#endif

#include "hal_rgn_if.h"
#endif /* __HAL_GOP_H__ */
