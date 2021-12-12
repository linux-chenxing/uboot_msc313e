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

#include "drv_disp_os.h"
#include "disp_debug.h"
#include "cam_sysfs.h"
#include "cam_clkgen.h"
#include "mhal_common.h"
#include "mhal_cmdq.h"
#include "mhal_disp_datatype.h"
#include "mhal_disp.h"


#include "imi_heap.h"
#include "mdrv_padmux.h"
#include "hal_disp_chip.h"
//-------------------------------------------------------------------------------------------------
//  Defines & Macro
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Structure
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Variable
//-------------------------------------------------------------------------------------------------
struct device_node *gpDispDeviceNode = NULL;

//-------------------------------------------------------------------------------------------------
//  Internal Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Public Functions
//-------------------------------------------------------------------------------------------------
void * DrvDispOsMemAlloc(u32 u32Size)
{
    return CamOsMemAlloc(u32Size);
}

void DrvDispOsMemRelease(void *pPtr)
{
    CamOsMemRelease(pPtr);
}

void DrvDispOsMsSleep(u32 u32Msec)
{
    CamOsMsSleep(u32Msec);
}

void DrvDispOsUsSleep(u32 u32Usec)
{
    CamOsUsSleep(u32Usec);
}

void DrvDispOsMsDelay(u32 u32Msec)
{
    CamOsMsDelay(u32Msec);
}

void DrvDispOsUsDelay(u32 u32Usec)
{
    CamOsUsDelay(u32Usec);
}

s32 DrvDispOsImiHeapAlloc(u32 u32Size, void **ppAddr)
{
    s32 s32Ret;
    s32Ret = ImiHeapMalloc(0, u32Size, ppAddr);
    return s32Ret;
}

void DrvDispOsImiHeapFree(void *pAddr)
{
    ImiHeapFree(0, pAddr);
}

bool DrvDispOsPadMuxActive(void)
{
    bool bRet = mdrv_padmux_active() ? 1 : 0;
    return bRet;
}

bool DrvDispOsCreateTask(DrvDispOsTaskConfig_t *pstTaskCfg, TaskEntryCb pTaskEntry, void *pDataPtr, u8 *pTaskName, bool bAuotStart)
{
    CamOsThreadAttrb_t stThreadAttrCfg = {0};
    bool bRet = TRUE;

    stThreadAttrCfg.nPriority = 50;
    stThreadAttrCfg.nStackSize = 0;
    stThreadAttrCfg.szName = pTaskName;

    if(CamOsThreadCreate(&pstTaskCfg->stThreadCfg, &stThreadAttrCfg, pTaskEntry, pDataPtr) == CAM_OS_OK)
    {

        if(bAuotStart)
        {
            if(CamOsThreadWakeUp(pstTaskCfg->stThreadCfg) == CAM_OS_OK)
            {
                bRet = TRUE;
                pstTaskCfg->s32Id = 1;

            }
            else
            {
                bRet = FALSE;
                pstTaskCfg->s32Id = -1;
                DISP_ERR("%s %d, CamOsThreadWakeUp Fail\n", __FUNCTION__, __LINE__);
            }
        }
    }
    else
    {
        pstTaskCfg->s32Id = -1;
        bRet = FALSE;
        DISP_ERR("%s %d, CamOsThreadCreate Fail\n", __FUNCTION__, __LINE__);
    }
    return bRet;
}

bool DrvDispOsDestroyTask(DrvDispOsTaskConfig_t *pstTaskCfg)
{
    if(CamOsThreadStop(pstTaskCfg->stThreadCfg) == CAM_OS_OK)
    {
        pstTaskCfg->s32Id = -1;
        return TRUE;
    }
    else
    {
        DISP_ERR("%s %d, CamOsThreadStop Fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
}

bool DrvDispOsSetDeviceNode(void *pPlatFormDev)
{
    gpDispDeviceNode = ((struct platform_device *)pPlatFormDev)->dev.of_node;

    if(gpDispDeviceNode)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
#ifdef CONFIG_CAM_CLK
    #include "drv_camclk_Api.h"
    void **pvDispclk = NULL;
    u32 DispParentCnt = 1;

u8 Disp_ClkRegister(struct device_node *device_node)
{
    u32 u32clknum;
    u32 DispClk;
    u8 str[16];

    if(of_find_property(device_node,"camclk",&DispParentCnt))
    {
        DispParentCnt /= sizeof(int);
        //printk( "[%s] Number : %d\n", __func__, num_parents);
        if(DispParentCnt < 0)
        {
            printk( "[%s] Fail to get parent count! Error Number : %d\n", __func__, DispParentCnt);
            return 0;
        }
        pvDispclk = CamOsMemAlloc((sizeof(void *) * DispParentCnt));
        if(!pvDispclk){
            return 0;
        }
        for(u32clknum = 0; u32clknum < DispParentCnt; u32clknum++)
        {
            DispClk = 0;
            msys_of_property_read_u32_index(device_node,"camclk", u32clknum,&(DispClk));
            if (!DispClk)
            {
                printk( "[%s] Fail to get clk!\n", __func__);
            }
            else
            {
                CamOsSnprintf(str, 16, "Disp_%d ",u32clknum);
                CamClkRegister(str,DispClk,&(pvDispclk[u32clknum]));
            }
        }
    }
    else
    {
        printk( "[%s] W/O Camclk \n", __func__);
    }
    return 1;
}
u8 Disp_ClkUnregister(void)
{

    u32 u32clknum;

    for(u32clknum=0;u32clknum<DispParentCnt;u32clknum++)
    {
        if(pvDispclk[u32clknum])
        {
            printk(KERN_DEBUG "[%s] %p\n", __func__,pvDispclk[u32clknum]);
            CamClkUnregister(pvDispclk[u32clknum]);
            pvDispclk[u32clknum] = NULL;
        }
    }
    CamOsMemRelease(pvDispclk);

    return 1;
}
#endif
bool DrvDispOsSetClkOn(void *pClkRate, u32 u32ClkRateSize)
{

    bool bRet = TRUE;
    unsigned long clk_rate;
    unsigned long *clk_rate_buf = (unsigned long *)pClkRate;
    bool abClkMuxAttr[HAL_DISP_CLK_NUM] = HAL_DISP_CLK_MUX_ATTR;
#ifdef CONFIG_CAM_CLK
    u32 u32clknum = 0;
    CAMCLK_Set_Attribute stSetCfg;
    CAMCLK_Get_Attribute stGetCfg;

    if(gpDispDeviceNode && pClkRate && u32ClkRateSize == HAL_DISP_CLK_NUM)
    {
        Disp_ClkRegister(gpDispDeviceNode);
        for(u32clknum = 0; u32clknum < DispParentCnt; u32clknum++)
        {
            if (pvDispclk[u32clknum])
            {
                clk_rate = clk_rate_buf[u32clknum];
                if(abClkMuxAttr[u32clknum] == 1)
                {
                    CamClkAttrGet(pvDispclk[u32clknum],&stGetCfg);
                    CAMCLK_SETPARENT(stSetCfg,stGetCfg.u32Parent[clk_rate]);
                    CamClkAttrSet(pvDispclk[u32clknum],&stSetCfg);
                }
                else
                {
                    CAMCLK_SETRATE_ROUNDUP(stSetCfg,clk_rate);
                    CamClkAttrSet(pvDispclk[u32clknum],&stSetCfg);
                }
                CamClkSetOnOff(pvDispclk[u32clknum],1);
            }
        }
    }
#else
    u32 num_parents, idx;
    struct clk **disp_clks;
    struct clk_hw *hw_parent;

    if(gpDispDeviceNode && pClkRate && u32ClkRateSize == HAL_DISP_CLK_NUM)
    {
        num_parents = CamOfClkGetParentCount(gpDispDeviceNode);

        DISP_DBG(DISP_DBG_LEVEL_CLK, "%s %d num_parents:%d \n", __FUNCTION__, __LINE__, num_parents);
        for(idx=0;idx<u32ClkRateSize; idx++)
        {
            DISP_DBG(DISP_DBG_LEVEL_CLK, "%s %d, CLK_%d = %ld\n", __FUNCTION__, __LINE__, idx, clk_rate_buf[idx]);
        }

        if(num_parents ==  HAL_DISP_CLK_NUM)
        {
            disp_clks = CamOsMemAlloc((sizeof(struct clk *) * num_parents));

            if(disp_clks == NULL)
            {
                DISP_ERR( "%s %d Alloc disp_clks is NULL\n" , __FUNCTION__, __LINE__);
                return 0;
            }

            for(idx=0; idx<num_parents; idx++)
            {
                disp_clks[idx] = of_clk_get(gpDispDeviceNode, idx);
                if (IS_ERR(disp_clks[idx]))
                {
                    DISP_ERR( "%s %d, Fail to get [Disp] %s\n" ,
                        __FUNCTION__, __LINE__, CamOfClkGetParentName(gpDispDeviceNode, idx) );
                    CamOsMemRelease(disp_clks);
                    return 0;
                }

                if(abClkMuxAttr[idx] == 1)
                {
                    clk_rate = clk_rate_buf[idx];
                    hw_parent = CamClkGetParentByIndex(__CamClkGetHw(disp_clks[idx]), clk_rate);
                    CamClkSetParent(disp_clks[idx], hw_parent->clk);
                }
                else
                {
                    clk_rate = CamClkRoundRate(disp_clks[idx], clk_rate_buf[idx]);
                    CamClkSetRate(disp_clks[idx], clk_rate + 1000000);
                }

                DISP_DBG(DISP_DBG_LEVEL_CLK, "%s %d, [Disp] %d-%d %15s %ld\n" ,
                            __FUNCTION__, __LINE__,
                            num_parents, idx+1,
                            CamOfClkGetParentName(gpDispDeviceNode, idx),
                            clk_rate);

                CamClkPrepareEnable(disp_clks[idx]);
                clk_put(disp_clks[idx]);
            }

            CamOsMemRelease(disp_clks);
        }
        else
        {
            bRet = FALSE;
            DISP_ERR("%s %d, num_parents %d != %d\n", __FUNCTION__, __LINE__, num_parents, HAL_DISP_CLK_NUM);
        }
    }
    else
    {
        bRet = FALSE;
        DISP_ERR("%s %d, Param Null, DeviceNode:%x, ClkRate:%x, ClkSize:%ld\n",
            __FUNCTION__, __LINE__, gpDispDeviceNode, pClkRate, u32ClkRateSize);
    }
#endif
    return bRet;
}

bool DrvDispOsSetClkOff(void *pClkRate, u32 u32ClkRateSize)
{
    bool bRet = TRUE;
#ifdef CONFIG_CAM_CLK
    u32 u32clknum = 0;

    for(u32clknum = 0; u32clknum < DispParentCnt; u32clknum++)
    {
        if (pvDispclk[u32clknum])
        {
            CamClkSetOnOff(pvDispclk[u32clknum],0);
        }
    }
    Disp_ClkUnregister();
#else
    u32 num_parents, idx;
    struct clk **disp_clks;

    if(gpDispDeviceNode)
    {
        num_parents = CamOfClkGetParentCount(gpDispDeviceNode);

        DISP_DBG(DISP_DBG_LEVEL_CLK, "%s %d num_parents:%d\n" , __FUNCTION__, __LINE__, num_parents);
        if(num_parents == HAL_DISP_CLK_NUM)
        {
            disp_clks = CamOsMemAlloc((sizeof(struct clk *) * num_parents));

            if(disp_clks == NULL)
            {
                DISP_ERR( "%s %d Alloc disp_clks is NULL\n" , __FUNCTION__, __LINE__);
                return 0;
            }


            for(idx=0; idx<num_parents; idx++)
            {
                disp_clks[idx] = of_clk_get(gpDispDeviceNode, idx);
                if (IS_ERR(disp_clks[idx]))
                {
                    DISP_ERR( "%s %d, Fail to get [Disp] %s\n" ,
                        __FUNCTION__, __LINE__, CamOfClkGetParentName(gpDispDeviceNode, idx) );
                    CamOsMemRelease(disp_clks);
                    return 0;
                }
                else
                {
                    DISP_DBG(DISP_DBG_LEVEL_CLK,"%s %d, [Disp] %d-%d, %15s Off\n",
                        __FUNCTION__, __LINE__, num_parents, idx+1,
                        CamOfClkGetParentName(gpDispDeviceNode, idx));
                    CamClkDisableUnprepare(disp_clks[idx]);
                }
                clk_put(disp_clks[idx]);
            }
            CamOsMemRelease(disp_clks);
        }
        else
        {
            bRet = FALSE;
        }
    }
    else
    {
        bRet = FALSE;
    }
#endif
    return bRet;
}


u32 DrvDispOsGetSystemTime (void)
{
    CamOsTimespec_t ts;
    CamOsGetTimeOfDay(&ts);
    return ts.nSec* 1000+ ts.nNanoSec/1000000;
}

u64 DrvDispOsGetSystemTimeStamp (void)
{
    u64 u64TimeStamp;
    CamOsTimespec_t stRes;
    CamOsGetMonotonicTime(&stRes);
    u64TimeStamp =(u64)stRes.nSec* 1000000ULL+ (u64)(stRes.nNanoSec/1000LL);
    return u64TimeStamp;
}

s32 DrvDispOsSplit(u8 **arr, u8 *str,  u8* del)
{
    char *cur = str;
    char *token = NULL;
    int cnt = 0;

    token = strsep(&cur, del);
    while (token)
    {
        if(cnt >= DISP_OS_STRING_MAX)
        {
            DISP_ERR("%s %d, Out of Size\n", __FUNCTION__, __LINE__);
            break;
        }
        arr[cnt] = token;
        token = strsep(&cur, del);
        cnt++;
    }
    return cnt;
}

u32 DrvDispOsGetFileSize(u8 *path)
{
    struct path p;
    struct kstat ks;
    int filesize;

    kern_path(path, 0, &p);
    vfs_getattr(&p, &ks);
    path_put(&p);
    filesize =  ks.size;
    return filesize;
}

bool DrvDispOsOpenFile(u8 *path,s32 flag,s32 mode, DrvDispOsFileConfig_t *pstFileCfg)
{
    bool bRet = 1;
    mm_segment_t oldfs;
    int err = 0;

    pstFileCfg->pFile = NULL;
    oldfs = get_fs();
    set_fs(get_ds());
    pstFileCfg->pFile = filp_open(path, flag, mode);
    set_fs(oldfs);
    if(IS_ERR(pstFileCfg->pFile))
    {
        DISP_ERR("%s %d, Open File Fail\n", __FUNCTION__, __LINE__);
        err = PTR_ERR(pstFileCfg->pFile);
        pstFileCfg->pFile = NULL;
        bRet = 0;
    }
    return bRet;
}

s32 DrvDispOsWriteFile(DrvDispOsFileConfig_t *pFileCfg, u8 *buf,s32 writelen)
{
    mm_segment_t oldfs;
    int ret;
    unsigned long long offset = 0;

    oldfs = get_fs();
    set_fs(get_ds());

    ret = vfs_write(pFileCfg->pFile, buf, writelen, &offset);

    set_fs(oldfs);
    return ret;
}

s32 DrvDispOsReadFile(DrvDispOsFileConfig_t *pFileCfg,u8 *buf,s32 readlen)
{
    mm_segment_t oldfs;
    int ret;
    unsigned long long offset = 0;

    oldfs = get_fs();
    set_fs(get_ds());

    ret = vfs_read(pFileCfg->pFile, buf, readlen, &offset);

    set_fs(oldfs);
    return ret;
}

s32 DrvDispOsCloseFile(DrvDispOsFileConfig_t *pFileCfg)
{
    filp_close(pFileCfg->pFile,NULL);
    return 0;
}

void DrvDispOsSplitByToken(DrvDispOsStrConfig_t *pstStrCfg, u8 *pBuf, u8 *token)
{
    pstStrCfg->argc = DrvDispOsSplit(pstStrCfg->argv, (char *)pBuf, token);
}

void DrvDispOsParsingTxt(u8 *pFileName, DrvDispOsTextConfig_t *pstTxtCfg)
{
    DrvDispOsFileConfig_t stReadFileCfg;
    char *pBuf;
    int FileSize;
    char filedel[] = "\n";
    char linedel[] = " ";
    int i, j, len;
    char LinePattern[1024];
    DrvDispOsStrConfig_t stLineStrCfg;
    DrvDispOsStrConfig_t stFileStrCfg;
    DrvDispOsTextItemConfig_t *pstTxtItemCfg;
    u32 u32LineCnt;

    DrvDispOsOpenFile(pFileName,O_RDONLY, 0, &stReadFileCfg);

    if(stReadFileCfg.pFile)
    {
        FileSize = DrvDispOsGetFileSize(pFileName);

        pBuf =DrvDispOsMemAlloc(FileSize+1);

        if(pBuf)
        {
            DrvDispOsReadFile(&stReadFileCfg, pBuf, FileSize);
            DrvDispOsCloseFile(&stReadFileCfg);
            pBuf[FileSize] = '\0';

            DrvDispOsSplitByToken(&stFileStrCfg, pBuf, filedel);
            u32LineCnt = stFileStrCfg.argc;

            pstTxtCfg->pstItem = DrvDispOsMemAlloc(sizeof(DrvDispOsTextItemConfig_t) * u32LineCnt);
            pstTxtCfg->u32Size = 0;

            if(pstTxtCfg->pstItem)
            {
                for(i=0; i<u32LineCnt; i++)
                {
                    if(i >= DISP_OS_STRING_MAX)
                    {
                        DISP_ERR("%s %d, Out Of Max String Size\n", __FUNCTION__, __LINE__);
                        break;
                    }

                    if(stFileStrCfg.argv[i] == NULL)
                    {
                        DISP_ERR("NULL Point, %d\n", i);
                        continue;
                    }


                    memset(LinePattern, 0, 1024);
                    len = strlen(stFileStrCfg.argv[i]);
                    memcpy(LinePattern, stFileStrCfg.argv[i], len);
                    DrvDispOsSplitByToken(&stLineStrCfg, LinePattern, linedel);

                    len = strlen(stLineStrCfg.argv[0]);
                    if((len == 0) || (stLineStrCfg.argv[0][0] == '#'))
                    {
                        continue;
                    }
                    pstTxtItemCfg = &pstTxtCfg->pstItem[pstTxtCfg->u32Size++];
                    len = strlen(stLineStrCfg.argv[0]);
                    memcpy(pstTxtItemCfg->pu8Name, stLineStrCfg.argv[0], len);
                    pstTxtItemCfg->pu8Name[len] = '\0';

                    for(j=1; j<stLineStrCfg.argc; j++)
                    {
                        if(j >= DISP_OS_STRING_MAX)
                        {
                            DISP_ERR("%s %d, Out Of Max String Size\n", __FUNCTION__, __LINE__);
                            break;
                        }

                        if(stLineStrCfg.argv[j][0] == '#')
                        {
                            break;
                        }

                        len = strlen(stLineStrCfg.argv[j]);

                        if(len != 0 && (strcmp(stLineStrCfg.argv[j], "=")!=0))
                        {
                            memcpy(pstTxtItemCfg->pu8Value, stLineStrCfg.argv[j], len);
                            pstTxtItemCfg->pu8Value[len] = '\0';
                        }
                    }
                    //printk("line:%d, %s = %s\n", i, pstTxtItemCfg->pu8Name, pstTxtItemCfg->pu8Value);
                }
            }

           CamOsMemRelease(pBuf);
        }
    }
}

void DrvDispOsStrToL(u8 *pStr, u16 u16Base, u16 *pu16Val)
{
    int ret;
    long u16Val = 0;
    ret = kstrtol(pStr, u16Base, &u16Val);

    *pu16Val = u16Val;
}

bool DrvDispOsStrCmp(u8 *pu8Str, u8 *pu8Pat)
{
    if(strcmp(pu8Str, pu8Pat) == 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}


bool DrvDispOsCreateMutex(DrvDispOsMutexConfig_t *pstMutexCfg)
{
    bool bRet = TRUE;

    if(CamOsMutexInit(&pstMutexCfg->stMutxCfg) == CAM_OS_OK)
    {
        pstMutexCfg->s32Id = 1;
        bRet = TRUE;
    }
    else
    {
        pstMutexCfg->s32Id  = -1;

        DISP_ERR("%s %d, MutexCreate Fail\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    return bRet;
}

bool DrvDispOsDestroyMutex(DrvDispOsMutexConfig_t *pstMutexCfg)
{
    bool bRet;

    if(CamOsMutexDestroy(&pstMutexCfg->stMutxCfg) == CAM_OS_OK)
    {
        pstMutexCfg->s32Id = -1;
        bRet = TRUE;
    }
    else
    {
        DISP_ERR("%s %d, Mutex Destroy Fail\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    return bRet;
}


bool DrvDispOsObtainMutex(DrvDispOsMutexConfig_t *pstMutexCfg)
{
    if(CamOsMutexLock(&pstMutexCfg->stMutxCfg) == CAM_OS_OK)
    {
        return TRUE;
    }
    else
    {

        DISP_ERR("%s %d, Mutex Lock Fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
}


bool DrvDispOsReleaseMutex(DrvDispOsMutexConfig_t *pstMutexCfg)
{
    if(CamOsMutexUnlock(&pstMutexCfg->stMutxCfg) == CAM_OS_OK)
    {
        return TRUE;
    }
    else
    {

        DISP_ERR("%s %d, Mutex UnLock Fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
}


EXPORT_SYMBOL(MHAL_DISP_DeviceSetBackGroundColor);
EXPORT_SYMBOL(MHAL_DISP_VideoLayerUnBind);
EXPORT_SYMBOL(MHAL_DISP_VideoLayerCreateInstance);
EXPORT_SYMBOL(MHAL_DISP_VideoLayerDestoryInstance);
EXPORT_SYMBOL(MHAL_DISP_DeviceCreateInstance);
EXPORT_SYMBOL(MHAL_DISP_InputPortSetAttr);
EXPORT_SYMBOL(MHAL_DISP_DeviceSetHdmiParam);
EXPORT_SYMBOL(MHAL_DISP_VideoLayerBind);
EXPORT_SYMBOL(MHAL_DISP_DeviceEnable);
EXPORT_SYMBOL(MHAL_DISP_InputPortShow);
EXPORT_SYMBOL(MHAL_DISP_InputPortAttrEnd);
EXPORT_SYMBOL(MHAL_DISP_ClearDevInterrupt);
EXPORT_SYMBOL(MHAL_DISP_VideoLayerEnable);
EXPORT_SYMBOL(MHAL_DISP_DeviceSetOutputTiming);
EXPORT_SYMBOL(MHAL_DISP_DeviceDestroyInstance);
EXPORT_SYMBOL(MHAL_DISP_InputPortCreateInstance);
EXPORT_SYMBOL(MHAL_DISP_InputPortEnable);
EXPORT_SYMBOL(MHAL_DISP_InitPanelConfig);
EXPORT_SYMBOL(MHAL_DISP_GetDevIrq);
EXPORT_SYMBOL(MHAL_DISP_GetDevIrqFlag);
EXPORT_SYMBOL(MHAL_DISP_InitMmapConfig);
EXPORT_SYMBOL(MHAL_DISP_DeviceAttach);
EXPORT_SYMBOL(MHAL_DISP_InputPortAttrBegin);
EXPORT_SYMBOL(MHAL_DISP_InputPortFlip);
EXPORT_SYMBOL(MHAL_DISP_DeviceDetach);
EXPORT_SYMBOL(MHAL_DISP_DeviceSetCvbsParam);
EXPORT_SYMBOL(MHAL_DISP_InputPortDestroyInstance);
EXPORT_SYMBOL(MHAL_DISP_VideoLayerCheckBufferFired);
EXPORT_SYMBOL(MHAL_DISP_InputPortHide);
EXPORT_SYMBOL(MHAL_DISP_DeviceSetVgaParam);
EXPORT_SYMBOL(MHAL_DISP_VideoLayerSetAttr);
EXPORT_SYMBOL(MHAL_DISP_DeviceAddOutInterface);
EXPORT_SYMBOL(MHAL_DISP_EnableDevIrq);
EXPORT_SYMBOL(MHAL_DISP_DbgLevel);
EXPORT_SYMBOL(MHAL_DISP_InputPortRotate);
EXPORT_SYMBOL(MHAL_DISP_InputPortSetCropAttr);
EXPORT_SYMBOL(MHAL_DISP_DeviceSetColorTempeture);
EXPORT_SYMBOL(MHAL_DISP_DeviceSetLcdParam);
EXPORT_SYMBOL(MHAL_DISP_DeviceSetGammaParam);
EXPORT_SYMBOL(MHAL_DISP_DeviceGetTimeZone);
EXPORT_SYMBOL(MHAL_DISP_DeviceSetTimeZoneConfig);
EXPORT_SYMBOL(MHAL_DISP_DeviceGetDisplayInfo);
EXPORT_SYMBOL(MHAL_DISP_DeviceGetInstance);
EXPORT_SYMBOL(MHAL_DISP_DeviceSetPqConfig);
EXPORT_SYMBOL(MHAL_DISP_DeviceSetConfig);
EXPORT_SYMBOL(MHAL_DISP_DeviceGetConfig);
EXPORT_SYMBOL(MHAL_DISP_SetRegAccessConfig);
EXPORT_SYMBOL(MHAL_DISP_SetRegFlipConfig);
EXPORT_SYMBOL(MHAL_DISP_SetRegWaitDoneConfig);
EXPORT_SYMBOL(MHAL_DISP_SetCmdqInterface);
EXPORT_SYMBOL(MHAL_DISP_DmaCreateInstance);
EXPORT_SYMBOL(MHAL_DISP_DmaDestoryInstance);
EXPORT_SYMBOL(MHAL_DISP_DmaBind);
EXPORT_SYMBOL(MHAL_DISP_DmaUnBind);
EXPORT_SYMBOL(MHAL_DISP_DmaSetAttr);
EXPORT_SYMBOL(MHAL_DISP_DmaSetBufferAttr);
EXPORT_SYMBOL(MHAL_DISP_EnableDmaIrq);
EXPORT_SYMBOL(MHAL_DISP_ClearDmaInterrupt);
EXPORT_SYMBOL(MHAL_DISP_GetDmaIrqFlag);
EXPORT_SYMBOL(MHAL_DISP_InputPortSetRingBuffAttr);
EXPORT_SYMBOL(MHAL_DISP_GetDmaIrq);
EXPORT_SYMBOL(MHAL_DISP_GetDevIrqById);
EXPORT_SYMBOL(MHAL_DISP_DeviceGetHwCount);
EXPORT_SYMBOL(MHAL_DISP_VideoLayerGetHwCount);
EXPORT_SYMBOL(MHAL_DISP_InputPortGetHwCount);
EXPORT_SYMBOL(MHAL_DISP_InputPortSetImiAddr);

EXPORT_SYMBOL(DrvDispOsMemRelease);
EXPORT_SYMBOL(DrvDispOsMemAlloc);
EXPORT_SYMBOL(DrvDispOsCreateTask);
EXPORT_SYMBOL(DrvDispOsDestroyTask);
EXPORT_SYMBOL(DrvDispOsMsSleep);
EXPORT_SYMBOL(DrvDispOsGetSystemTime);
EXPORT_SYMBOL(DrvDispOsGetSystemTimeStamp);
