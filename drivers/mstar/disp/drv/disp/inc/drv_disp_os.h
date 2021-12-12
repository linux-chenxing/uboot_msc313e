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

#ifndef _DRV_DISP_OS_H_
#define _DRV_DISP_OS_H_

#include "drv_disp_os_header.h"

#ifdef _DRV_DISP_OS_C_
#define INTERFACE
#else
#define INTERFACE extern
#endif

INTERFACE void *DrvDispOsMemAlloc(u32 u32Size);
INTERFACE void DrvDispOsMemRelease(void *pPtr);
INTERFACE void DrvDispOsMsSleep(u32 u32Msec);
INTERFACE void DrvDispOsUsSleep(u32 u32Usec);
INTERFACE void DrvDispOsMsDelay(u32 u32Msec);
INTERFACE void DrvDispOsUsDelay(u32 u32Usec);
INTERFACE s32  DrvDispOsImiHeapAlloc(u32 u32Size, void **ppAddr);
INTERFACE void DrvDispOsImiHeapFree(void *pAddr);
INTERFACE bool DrvDispOsPadMuxActive(void);
INTERFACE bool DrvDispOsSetDeviceNode(void *pPlatFormDev);
INTERFACE bool DrvDispOsSetClkOn(void *pClkRate, u32 u32ClkRateSize);
INTERFACE bool DrvDispOsSetClkOff(void *pClkRate, u32 u32ClkRateSize);
INTERFACE bool DrvDispOsCreateTask(DrvDispOsTaskConfig_t *pstTaskCfg, TaskEntryCb pTaskEntry, void *pDataPtr, u8 *pTaskName, bool bAuotStart);
INTERFACE bool DrvDispOsDestroyTask(DrvDispOsTaskConfig_t *pstTaskCfg);
INTERFACE u32  DrvDispOsGetSystemTime (void);
INTERFACE u64  DrvDispOsGetSystemTimeStamp (void);
INTERFACE bool DrvDispOsCreateMutex(DrvDispOsMutexConfig_t *pstMutexCfg);
INTERFACE bool DrvDispOsDestroyMutex(DrvDispOsMutexConfig_t *pstMutexCfg);
INTERFACE bool DrvDispOsObtainMutex(DrvDispOsMutexConfig_t *pstMutexCfg);
INTERFACE bool DrvDispOsReleaseMutex(DrvDispOsMutexConfig_t *pstMutexCfg);

INTERFACE s32  DrvDispOsSplit(u8 **arr, u8 *str,  u8* del);
INTERFACE u32  DrvDispOsGetFileSize(u8 *path);
INTERFACE s32  DrvDispOsWriteFile(DrvDispOsFileConfig_t *pFileCfg, u8 *buf,s32 writelen);
INTERFACE bool DrvDispOsOpenFile(u8 *path,s32 flag,s32 mode, DrvDispOsFileConfig_t *pstFileCfg);
INTERFACE s32  DrvDispOsCloseFile(DrvDispOsFileConfig_t *pFileCfg);
INTERFACE s32  DrvDispOsReadFile(DrvDispOsFileConfig_t *pFileCfg,u8 *buf,s32 readlen);
INTERFACE void DrvDispOsSplitByToken(DrvDispOsStrConfig_t *pstStrCfg, u8 *pBuf, u8 *token);
INTERFACE void DrvDispOsParsingTxt(u8 *pFileName, DrvDispOsTextConfig_t *pstTxtCfg);
INTERFACE void DrvDispOsStrToL(u8 *pStr, u16 u16Base, u16 *pu16Val);
INTERFACE bool DrvDispOsStrCmp(u8 *pu8Str, u8 *pu8Pat);

#undef INTERFACE

#endif
