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

#ifndef _HAL_DISP_MOP_IF_H_
#define _HAL_DISP_MOP_IF_H_

//-------------------------------------------------------------------------------------------------
//  Defines & Macro
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  structure & Enum
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Prototype
//-------------------------------------------------------------------------------------------------

#ifdef _HAL_DISP_MOP_IF_C_
#define INTERFACE
#else
#define INTERFACE extern
#endif

INTERFACE HalDispQueryRet_e HalDispMopIfGetInfoInputPortInit(void *pCtx, void *pCfg);
INTERFACE void HalDispMopIfSetInputPortInit(void *pCtx, void *pCfg);
INTERFACE HalDispQueryRet_e HalDispMopIfGetInfoInputPortEnable(void *pCtx, void *pCfg);
INTERFACE void HalDispMopIfSetInputPortEnable(void *pCtx, void *pCfg);
INTERFACE HalDispQueryRet_e HalDispMopIfGetInfoInputPortAttr(void *pCtx, void *pCfg);
INTERFACE void HalDispMopIfSetInputPortAttr(void *pCtx, void *pCfg);
INTERFACE HalDispQueryRet_e HalDispMopIfGetInfoInputPortFlip(void *pCtx, void *pCfg);
INTERFACE void HalDispMopIfSetInputPortFlip(void *pCtx, void *pCfg);
INTERFACE HalDispQueryRet_e HalDispMopIfGetInfoInputPortRotate(void *pCtx, void *pCfg);
INTERFACE void HalDispMopIfSetInputPortRotate(void *pCtx, void *pCfg);
INTERFACE HalDispQueryRet_e HalDispMopIfGetInfoInputPortCrop(void *pCtx, void *pCfg);
INTERFACE void HalDispMopIfSetInputPortCrop(void *pCtx, void *pCfg);
INTERFACE HalDispQueryRet_e HalDispMopIfGetInfoStretchWinSize(void *pCtx, void *pCfg);
INTERFACE void HalDispMopIfSetInfoStretchWinSize(void *pCtx, void *pCfg);
INTERFACE void HalDispMopIfSetInfoHextSize(void *pCtx, void *pCfg);
INTERFACE void HalDispMopIfInputPortDeinit(void);
INTERFACE void HalDispMopIfSetInfoStretchWinMop0Auto(void *pCtx, bool bEnAuto);
INTERFACE HalDispQueryRet_e HalDispMopIfGetInfoInputPortRingBuffAttr(void *pCtx, void *pCfg);
INTERFACE void HalDispMopIfSetInputPortRingBuffAttr(void *pCtx, void *pCfg);
INTERFACE HalDispQueryRet_e HalDispMopIfGetInfoImiStartAddr(void *pCtx, void *pCfg);
INTERFACE void HalDispMopIfSetImiStartAddr(void *pCtx, void *pCfg);
INTERFACE void HalDispMopIfSetDdbfWr(void *pCtx);
INTERFACE void HalDispMopIfSetForceWr(void *pCtx, bool bEn);

#undef INTERFACE
#endif
