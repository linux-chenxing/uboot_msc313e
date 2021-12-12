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
#define __DRV_RGN_COMMON_C__


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "drv_rgn_common.h"
#include "rgn_sysfs.h"

//-------------------------------------------------------------------------------------------------
//  Defines & Macro
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  structure
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Variable
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

bool _DrvRgnTransCmdqId(DrvRgnCmdqIdType_e eCmdqId,HalRgnCmdqIdType_e *pHalCmdqId)
{
    bool bRet = TRUE;
    bool str[8];

    if(eCmdqId == E_DRV_RGN_VPE_CMDQ_ID_0)
    {
        strcpy((char *)str,RGN_DEVICE_SC);
    }
    else if(eCmdqId == E_DRV_RGN_DIVP_CMDQ_ID_1)
    {
        strcpy((char *)str,RGN_DEVICE_DIP);
    }
    else if(eCmdqId == E_DRV_RGN_LDC_CMDQ_ID_2)
    {
        strcpy((char *)str,RGN_DEVICE_LDC);
    }
    else if(eCmdqId == E_DRV_RGN_DISP_CMDQ_ID_3)
    {
        strcpy((char *)str,RGN_DEVICE_DISP);
    }
    else
    {
        return FALSE;
    }
    bRet = HalRgnTransCmdqId((const char*)str,pHalCmdqId);
    return bRet;
}
//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
bool DrvRgnUpdateReg(void)
{
    HalRgnUpdateReg();
    return 1;
}
bool DrvRgnInit(void)
{
    HalRgnInit();
    HalRgnInitCmdq();
    return 1;
}
bool DrvRgnDeinit(void)
{
    HalRgnDeinit();
    HalRgnInitCmdq();
    return 1;
}
bool DrvRgnSetOsdBindGop(unsigned int eGopNum)
{
    HalRgnOsdFlag_e eOsdNum = eGopNum;
    if(HalRgnCheckBindRation(&eOsdNum))
    {
        geGopOsdFlag = eGopNum;
        return 1;
    }
    else
    {
        return 0;
    }
}
bool DrvRgnGetOsdBindGop(unsigned int *eGopNum)
{
    *eGopNum = geGopOsdFlag;
    return 1;
}
void DrvRgnUseCmdq(bool bEn)
{
    HalRgnUseCmdq(bEn);
}
bool DrvRgnSetCmdq(MHAL_CMDQ_CmdqInterface_t *pstCmdq,DrvRgnCmdqIdType_e eCmdqId)
{
    bool bRet = FALSE;
    HalRgnCmdqIdType_e eHalCmdqId;
    if(_DrvRgnTransCmdqId(eCmdqId, &eHalCmdqId) == TRUE)
    {
        HalRgnSetCmdq(pstCmdq,eHalCmdqId);
        bRet = TRUE;
    }
    return bRet;
}
