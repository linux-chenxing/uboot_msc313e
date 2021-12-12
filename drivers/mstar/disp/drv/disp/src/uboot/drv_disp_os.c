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

#define _DRV_DISP_OS_C_
#include <common.h>
#include <command.h>
#include <config.h>
#include <malloc.h>
#include <stdlib.h>

#include "drv_disp_os.h"
//-------------------------------------------------------------------------------------------------
//  Defines & Macro
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Structure
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Variable
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Internal Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Public Functions
//-------------------------------------------------------------------------------------------------
void * DrvDispOsMemAlloc(u32 u32Size)
{
    void *pBuf = NULL;
    pBuf = malloc(u32Size);
    if(pBuf)
    {
        memset(pBuf, 0, u32Size);
    }
    return pBuf;
}

void DrvDispOsMemRelease(void *pPtr)
{
    free(pPtr);
}

void DrvDispOsMsSleep(u32 u32Msec)
{
    mdelay(u32Msec);
}

void DrvDispOsUsSleep(u32 u32Usec)
{
    udelay(u32Usec);
}

void DrvDispOsMsDelay(u32 u32Msec)
{
    mdelay(u32Msec);
}

void DrvDispOsUsDelay(u32 u32Usec)
{
    udelay(u32Usec);
}

s32 DrvDispOsImiHeapAlloc(u32 u32Size, void **ppAddr)
{
    return 0;
}

void DrvDispOsImiHeapFree(void *pAddr)
{

}

bool DrvDispOsPadMuxActive(void)
{
    return 0;
}

bool DrvDispOsCreateTask(DrvDispOsTaskConfig_t *pstTaskCfg, TaskEntryCb pTaskEntry, void *pDataPtr, u8 *pTaskName, bool bAuotStart)
{
    return 0;
}

bool DrvDispOsDestroyTask(DrvDispOsTaskConfig_t *pstTaskCfg)
{
    return 0;
}

bool DrvDispOsSetDeviceNode(void *pPlatFormDev)
{
    return 1;
}

bool DrvDispOsSetClkOn(void *pClkRate, u32 u32ClkRateSize)
{
    return 0;
}

bool DrvDispOsSetClkOff(void *pClkRate, u32 u32ClkRateSize)
{
    return 0;
}

u32 DrvDispOsGetSystemTime (void)
{
    return 0;
}

u64 DrvDispOsGetSystemTimeStamp (void)
{
    return 0;
}

s32 DrvDispOsSplit(u8 **arr, u8 *str,  u8* del)
{
    return 0;
}

u32 DrvDispOsGetFileSize(u8 *path)
{
    return 0;
}

bool DrvDispOsOpenFile(u8 *path,s32 flag,s32 mode, DrvDispOsFileConfig_t *pstFileCfg)
{
    return 0;
}

s32 DrvDispOsWriteFile(DrvDispOsFileConfig_t *pFileCfg, u8 *buf,s32 writelen)
{
    return 0;
}

s32 DrvDispOsReadFile(DrvDispOsFileConfig_t *pFileCfg,u8 *buf,s32 readlen)
{
    return 0;
}


s32 DrvDispOsCloseFile(DrvDispOsFileConfig_t *pFileCfg)
{
    return 0;
}

void DrvDispOsSplitByToken(DrvDispOsStrConfig_t *pstStrCfg, u8 *pBuf, u8 *token)
{
}

void DrvDispOsParsingTxt(u8 *pFileName, DrvDispOsTextConfig_t *pstTxtCfg)
{
}

void DrvDispOsStrToL(u8 *pStr, u16 u16Base, u16 *pu16Val)
{
}

bool DrvDispOsStrCmp(u8 *pu8Str, u8 *pu8Pat)
{
    return 0;
}

bool DrvDispOsCreateMutex(DrvDispOsMutexConfig_t *pstMutexCfg)
{
    return 1;
}

bool DrvDispOsDestroyMutex(DrvDispOsMutexConfig_t *pstMutexCfg)
{
    return 1;
}

bool DrvDispOsObtainMutex(DrvDispOsMutexConfig_t *pstMutexCfg)
{
    return 1;
}

bool DrvDispOsReleaseMutex(DrvDispOsMutexConfig_t *pstMutexCfg)
{
    return 1;
}

