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

#ifndef _DRV_DISP_IRQ_H_
#define _DRV_DISP_IRQ_H_

//-------------------------------------------------------------------------------------------------
//  Defines & Macro
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Enum
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  structure
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Prototype
//-------------------------------------------------------------------------------------------------


#ifdef _DRV_DISP_IRQ_C_
#define INTERFACE
#else
#define INTERFACE extern
#endif
INTERFACE void DrvDispIrqSetIsrNum(u32 u32DevId, u32 u32IsrNum);
INTERFACE bool DrvDispIrqGetIsrNum(void *pDevCtx, u32 *pu32IsrNum);
INTERFACE bool DrvDispIrqGetIsrNumByDevId(u32 u32DevId, u32 *pu32IsrNum);
INTERFACE bool DrvDispIrqEnable(void *pDevCtx, u32 u32DevIrq, bool bEnable);
INTERFACE bool DrvDispIrqGetFlag(void *pDevCtx, MHAL_DISP_IRQFlag_t *pstIrqFlag);
INTERFACE bool DrvDispIrqClear(void *pDevCtx, void* pData);

INTERFACE bool DrvDispIrqCreateInternalIsr(void *pDispCtx);
INTERFACE bool DrvDispIrqDestroyInternalIsr(void *pDispCtx);
INTERFACE u16  DrvDispIrqGetIrqCount(void);

#undef INTERFACE
#endif
