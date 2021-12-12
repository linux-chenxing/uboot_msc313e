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

#define _DISP_SYSFS_C_
//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include <linux/platform_device.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/fs.h>
#include <asm/segment.h>
#include <asm/uaccess.h>
#include <linux/buffer_head.h>
#include <linux/path.h>
#include <linux/namei.h>
#include <linux/delay.h>
#include <linux/device.h>

#include "ms_msys.h"
#include "ms_platform.h"

#include "cam_sysfs.h"
#include "drv_disp_os.h"
#include "hal_disp_common.h"
#include "disp_debug.h"
#include "hal_disp_util.h"
#include "hal_disp_reg.h"

#include "mhal_common.h"
#include "mhal_cmdq.h"
#include "mhal_disp_datatype.h"
#include "mhal_disp.h"
#include "drv_disp_if.h"
#include "hal_disp_chip.h"
#include "hal_disp_st.h"
#include "hal_disp_if.h"
#include "hal_disp_util.h"
#include "hal_disp.h"
#include "drv_disp_ctx.h"
#include "hal_disp.h"

//-------------------------------------------------------------------------------------------------
//  Defines & Macro
//-------------------------------------------------------------------------------------------------
#define DISPSYSFS_SPRINTF(str, _fmt, _args...)  sprintf(str, _fmt, ## _args)

#define DISPSYSFS_DBG(_fmt, _args...)                 \
    do{                                         \
        CamOsPrintf(PRINT_GREEN _fmt PRINT_NONE, ## _args);       \
    }while(0)

//-------------------------------------------------------------------------------------------------
//  structure
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Variable
//-------------------------------------------------------------------------------------------------
MHAL_DISP_AllocPhyMem_t gstMemAlloc = {NULL, NULL};

u32 u32DispDbgmgLevel = 0;
u8  u8PtGenMode = 0;

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
void DispDbgmgStore(DrvDispOsStrConfig_t *pstStringCfg)
{
    int ret;
    long u32Level;

    if(pstStringCfg->argc < 1)
    {
        return;
    }

    if(pstStringCfg->argc == 1)
    {
        ret = kstrtol(pstStringCfg->argv[0], 16, &u32Level);
    }
    else
    {
        DISPSYSFS_DBG("dbgg [level] \n");
        DISPSYSFS_DBG("DRV:       0x00000001 \n");
        DISPSYSFS_DBG("HAL:       0x00000002 \n");
        DISPSYSFS_DBG("IO:        0x00000004 \n");
        DISPSYSFS_DBG("CTX:       0x00000008 \n");
        DISPSYSFS_DBG("COLOR:     0x00000010 \n");
        DISPSYSFS_DBG("IRQ:       0x00000020 \n");
        DISPSYSFS_DBG("IRQ:       0x00000040 \n");
        DISPSYSFS_DBG("Inter IRQ: 0x00000080 \n");
        DISPSYSFS_DBG("CLK:       0x00000100 \n");
        return;
    }

    u32DispDbgmgLevel = u32Level;
    MHAL_DISP_DbgLevel((void *)&u32Level);
    printk("DbgLevel:%08lx\n", u32Level);

}

int DispDbgmgShow(char *DstBuf)
{
    int RetSprintf = -1;
    char *SrcBuf;
    u8 i;

    SrcBuf = (char *)DrvDispOsMemAlloc(1024*3);

    if(SrcBuf)
    {
        void *pDevCtx;


        DrvDispOsSprintfStrcat(SrcBuf, "dbglevle: %08x \n", (unsigned int)u32DispDbgmgLevel);


        for(i=0; i<HAL_DISP_DEVICE_MAX; i++)
        {
            MHAL_DISP_DeviceGetInstance(i, &pDevCtx);
            if(pDevCtx)
            {
                HalDispQueryConfig_t stQueryCfg;
                HalDispDbgmgConfig_t stDbgmgCfg;

                memset(&stQueryCfg, 0 ,sizeof(HalDispQueryConfig_t));
                memset(&stDbgmgCfg, 0 ,sizeof(HalDispDbgmgConfig_t));

                stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_DBGMG_GET;
                stQueryCfg.stInCfg.u32CfgSize = sizeof(HalDispDbgmgConfig_t);
                stQueryCfg.stInCfg.pInCfg = (void *)&stDbgmgCfg;

                stDbgmgCfg.pData = SrcBuf;

                if(HalDispIfQuery(pDevCtx, &stQueryCfg))
                {
                    if(stQueryCfg.stOutCfg.pSetFunc)
                    {
                        stQueryCfg.stOutCfg.pSetFunc(pDevCtx, stQueryCfg.stInCfg.pInCfg);
                    }
                }
            }

            RetSprintf = DISPSYSFS_SPRINTF(DstBuf, "%s", SrcBuf);
        }

        DrvDispOsMemRelease(SrcBuf);
    }
    return RetSprintf;
}


void DispClkStore(DrvDispOsStrConfig_t *pstStringCfg)
{
    int ret, idx;
    long Enable = 0;
    long ClkRate = 0;
    DrvDispCtxConfig_t stDispCtx;
    char *ClkName = NULL;
    bool abClkEn[HAL_DISP_CLK_NUM] = HAL_DISP_CLK_ON_SETTING;
    u32  au32ClkRate[HAL_DISP_CLK_NUM] = HAL_DISP_CLK_RATE_SETTING;
    char au8ClkName[HAL_DISP_CLK_NUM][20] = HAL_DISP_CLK_NAME;
    u8   au8ClkMuxAttr[HAL_DISP_CLK_NUM] = HAL_DISP_CLK_MUX_ATTR;
    u32 i;

    if(pstStringCfg->argc < 1)
    {
        return;
    }

    if(pstStringCfg->argc == HAL_DISP_CLK_NUM + 2)
    {
        ClkName = pstStringCfg->argv[0];
        ret = kstrtol(pstStringCfg->argv[1], 10, &Enable);
        for(i=0; i<HAL_DISP_CLK_NUM; i++)
        {
            ret = kstrtol(pstStringCfg->argv[i+2], 10, &ClkRate);
            au32ClkRate[i] = ClkRate;
        }
    }
    else if(pstStringCfg->argc == 3)
    {
        ClkName = pstStringCfg->argv[0];
        ret = kstrtol(pstStringCfg->argv[1], 10, &Enable);
        ret = kstrtol(pstStringCfg->argv[2], 10, &ClkRate);
    }
    else
    {
        char *SrcBuf = NULL;
        SrcBuf = (char *)CamOsMemAlloc(1024*3);
        DrvDispOsSprintfStrcat(SrcBuf, "----------------- CLK TREE  -----------------\n");
        DrvDispOsSprintfStrcat(SrcBuf, "clktree [En] [%s 0 ~ %d]\n", au8ClkMuxAttr[i] ? "CLKIdx": "ClkRate", HAL_DISP_CLK_NUM-1);
        DrvDispOsSprintfStrcat(SrcBuf, "----------------- DRV Update -----------------\n");
        DrvDispOsSprintfStrcat(SrcBuf, "[CLK Type] [En] [%s]\n",  au8ClkMuxAttr[i] ? "CLKIdx": "ClkRate");
        DrvDispOsSprintfStrcat(SrcBuf, "----------------- CLK Type ----------------- \n");
        for(i=0; i< HAL_DISP_CLK_NUM; i++)
        {
            DrvDispOsSprintfStrcat(SrcBuf,"%s, \n", au8ClkName[i]);
        }
        DISPSYSFS_DBG("%s\n", SrcBuf);
        CamOsMemRelease(SrcBuf);
        return;
    }

    if(strcmp(ClkName, "clktree") == 0)
    {
        if(Enable)
        {
            if(DrvDispOsSetClkOn(au32ClkRate, HAL_DISP_CLK_NUM) == 0)
            {
                DISP_ERR("%s %d, Set Clk On Fail\n", __FUNCTION__, __LINE__);
            }
        }
        else
        {
            if(DrvDispOsSetClkOff(au32ClkRate, HAL_DISP_CLK_NUM) == 0)
            {
                DISP_ERR("%s %d, Set Clk Off Fail\n", __FUNCTION__, __LINE__);
            }
        }
    }
    else
    {
        if(DrvDispIfGetClk((void *)&stDispCtx, abClkEn, au32ClkRate, HAL_DISP_CLK_NUM) == 0)
        {
            DISP_ERR("%s %d, Get Clk Fail\n", __FUNCTION__, __LINE__);
            return;
        }

        for(idx =0; idx < HAL_DISP_CLK_NUM; idx++)
        {
            if(strcmp(ClkName, au8ClkName[idx]) == 0)
            {
                abClkEn[idx] = Enable,
                au32ClkRate[idx] = ClkRate;
                if(DrvDispIfSetClk((void *)&stDispCtx, abClkEn, au32ClkRate, HAL_DISP_CLK_NUM) == 0)
                {
                    DISP_ERR("%s %d, Set Clk Fail\n", __FUNCTION__, __LINE__);
                }
                break;
            }
        }

        if(idx == HAL_DISP_CLK_NUM)
        {
            DISP_ERR("%s %d, ClkName (%s) is not correct\n", __FUNCTION__, __LINE__, ClkName);
        }
    }
}

int DispClkShow(char *DstBuf)
{
    bool abEn[HAL_DISP_CLK_NUM];
    u32 au32ClkRate[HAL_DISP_CLK_NUM];
    int RetSprintf = -1;
    char *SrcBuf;
    DrvDispCtxConfig_t stDispCtx;
    char  aClkName[HAL_DISP_CLK_NUM][20] = HAL_DISP_CLK_NAME;
    u32 i;

    SrcBuf = (char *)DrvDispOsMemAlloc(1024*3);

    if(SrcBuf)
    {
        if(DrvDispIfGetClk((void *)&stDispCtx, abEn, au32ClkRate, HAL_DISP_CLK_NUM) == 0)
        {
            DISP_ERR("%s %d, Get Clk Fail\n", __FUNCTION__, __LINE__);
        }

        for(i=0; i<HAL_DISP_CLK_NUM; i++)
        {
            DrvDispOsSprintfStrcat(SrcBuf, "CLK_%-15s: En:%d, ClkRate:%ld \n", aClkName[i], abEn[i], au32ClkRate[i]);
        }

        RetSprintf = DISPSYSFS_SPRINTF(DstBuf, "%s", SrcBuf);
        DrvDispOsMemRelease(SrcBuf);
    }
    return RetSprintf;
}

void DispPqStore(DrvDispOsStrConfig_t *pstStringCfg)
{
    char *pFileName;
    DrvDispOsTextConfig_t stTxtCfg;
    HalDispQueryConfig_t stQueryCfg;
    HalDispPqConfig_t  stPqCfg;
    DrvDispCtxConfig_t stDispCtx;
    long PqType;
    int ret;

    if(pstStringCfg->argc < 1)
    {
        return;
    }

    if(pstStringCfg->argc == 2)
    {
        pFileName = pstStringCfg->argv[0];

        ret = kstrtol(pstStringCfg->argv[1], 16, &PqType);
    }
    else
    {
        PqType = 0x112233;
    }

    if(PqType == 0x112233)
    {
        char *SrcBuf;

        SrcBuf = (char *)DrvDispOsMemAlloc(1024*10);
        if(SrcBuf)
        {
            memset(&stPqCfg, 0,sizeof(HalDispPqConfig_t));
            memset(&stQueryCfg, 0,sizeof(HalDispQueryConfig_t));

            stPqCfg.pData= (void *)SrcBuf;
            stPqCfg.u32PqFlags = 0x112233;
            stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_PQ_GET;
            stQueryCfg.stInCfg.u32CfgSize = sizeof(HalDispPqConfig_t);
            stQueryCfg.stInCfg.pInCfg = (void *)&stPqCfg;

            HalDispIfInit();
            if(HalDispIfQuery((void *)&stDispCtx, &stQueryCfg))
            {
                if(stQueryCfg.stOutCfg.pSetFunc)
                {
                    stQueryCfg.stOutCfg.pSetFunc((void *)&stDispCtx, stQueryCfg.stInCfg.pInCfg);
                }
            }

            DISPSYSFS_DBG("%s", SrcBuf);
            DrvDispOsMemRelease(SrcBuf);
        }
    }
    else
    {
        DrvDispOsParsingTxt(pFileName, &stTxtCfg);

        memset(&stPqCfg, 0,sizeof(HalDispPqConfig_t));
        memset(&stQueryCfg, 0,sizeof(HalDispQueryConfig_t));


        stPqCfg.pData= (void *)&stTxtCfg;
        stPqCfg.u32PqFlags = PqType;

        stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_PQ_SET;
        stQueryCfg.stInCfg.u32CfgSize = sizeof(HalDispPqConfig_t);
        stQueryCfg.stInCfg.pInCfg = (void *)&stPqCfg;

        HalDispIfInit();

        if(HalDispIfQuery((void *)&stDispCtx, &stQueryCfg))
        {
            if(stQueryCfg.stOutCfg.pSetFunc)
            {
                stQueryCfg.stOutCfg.pSetFunc((void *)&stDispCtx, stQueryCfg.stInCfg.pInCfg);
            }
        }
        CamOsMemRelease(stTxtCfg.pstItem);
    }
}

int DispPqShow(char *DstBuf)
{
    HalDispQueryConfig_t stQueryCfg;
    HalDispPqConfig_t  stPqCfg;
    DrvDispCtxConfig_t stDispCtx;
    int RetSprintf = -1;
    char *SrcBuf;

    SrcBuf = (char *)DrvDispOsMemAlloc(1024*10);

    if(SrcBuf)
    {
        memset(&stPqCfg, 0,sizeof(HalDispPqConfig_t));
        memset(&stQueryCfg, 0,sizeof(HalDispQueryConfig_t));

        stPqCfg.pData= (void *)SrcBuf;

        stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_PQ_GET;
        stQueryCfg.stInCfg.u32CfgSize = sizeof(HalDispPqConfig_t);
        stQueryCfg.stInCfg.pInCfg = (void *)&stPqCfg;

        HalDispIfInit();
        if(HalDispIfQuery((void *)&stDispCtx, &stQueryCfg))
        {
            if(stQueryCfg.stOutCfg.pSetFunc)
            {
                stQueryCfg.stOutCfg.pSetFunc((void *)&stDispCtx, stQueryCfg.stInCfg.pInCfg);
            }
        }

        RetSprintf = DISPSYSFS_SPRINTF(DstBuf, "%s", SrcBuf);
        DrvDispOsMemRelease(SrcBuf);
    }
    return RetSprintf;
}

void DispTurnDrvStore(DrvDispOsStrConfig_t *pstStringCfg)
{
    int ret;
    long val;
    HalDispQueryConfig_t stQueryCfg;
    HalDispDrvTurningConfig_t stDacTrimCfg;
    DrvDispCtxConfig_t stDispCtx;

    memset(&stDacTrimCfg, 0,sizeof(HalDispDrvTurningConfig_t));
    memset(&stQueryCfg, 0,sizeof(HalDispQueryConfig_t));

    if(pstStringCfg->argc == 2 && strcmp(pstStringCfg->argv[0], "vga_drv") == 0)
    {
        ret = kstrtol(pstStringCfg->argv[1], 10, &val);
        stDacTrimCfg.enType = E_HAL_DISP_DRV_TURNING_RGB;
        stDacTrimCfg.u16Trim[0] = val;
        stDacTrimCfg.u16Trim[1] = val;
        stDacTrimCfg.u16Trim[2] = val;

    }
    else
    {
        DISPSYSFS_DBG("vga_drv [value] \n");
        return;
    }

    stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_DRVTURNING_SET;
    stQueryCfg.stInCfg.u32CfgSize = sizeof(HalDispDrvTurningConfig_t);
    stQueryCfg.stInCfg.pInCfg = (void *)&stDacTrimCfg;

    HalDispIfInit();

    if(HalDispIfQuery((void *)&stDispCtx, &stQueryCfg))
    {
        if(stQueryCfg.stOutCfg.pSetFunc)
        {
            stQueryCfg.stOutCfg.pSetFunc((void *)&stDispCtx, stQueryCfg.stInCfg.pInCfg);
        }
    }
}

int DispTurnDrvShow(char *DstBuf)
{
    HalDispQueryConfig_t stQueryCfg;
    HalDispDrvTurningConfig_t stDacTrimCfg;
    DrvDispCtxConfig_t stDispCtx;
    int RetSprintf = -1;
    char *SrcBuf;

    SrcBuf = (char *)DrvDispOsMemAlloc(1024*10);

    if(SrcBuf)
    {

        memset(&stDacTrimCfg, 0,sizeof(HalDispDrvTurningConfig_t));
        memset(&stQueryCfg, 0,sizeof(HalDispQueryConfig_t));

        stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_DRVTURNING_GET;
        stQueryCfg.stInCfg.u32CfgSize = sizeof(HalDispDrvTurningConfig_t);
        stQueryCfg.stInCfg.pInCfg = (void *)&stDacTrimCfg;

        HalDispIfInit();
        if(HalDispIfQuery((void *)&stDispCtx, &stQueryCfg))
        {
            if(stQueryCfg.stOutCfg.pSetFunc)
            {
                stQueryCfg.stOutCfg.pSetFunc((void *)&stDispCtx, stQueryCfg.stInCfg.pInCfg);
            }
        }

        DrvDispOsSprintfStrcat(SrcBuf, "Vga Drv: 0x%02x 0x%02x 0x%02x \n",
            stDacTrimCfg.u16Trim[0], stDacTrimCfg.u16Trim[1], stDacTrimCfg.u16Trim[2]);

        RetSprintf = DISPSYSFS_SPRINTF(DstBuf, "%s", SrcBuf);
        DrvDispOsMemRelease(SrcBuf);
    }
    return RetSprintf;
}


void _DispSysFsParsingCommand(char *str, DrvDispOsStrConfig_t *pstStrCfg)
{
    char del[] = " ";
    int len;

    pstStrCfg->argc = DrvDispOsSplit(pstStrCfg->argv, (char *)str, del);
    len = strlen(pstStrCfg->argv[pstStrCfg->argc-1]);
    pstStrCfg->argv[pstStrCfg->argc-1][len-1] = '\0';
}
//------------------------------------------------------------------------------
ssize_t check_dispdbgmg_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t n)
{
    if(NULL!=buf)
    {
        DrvDispOsStrConfig_t stStringCfg;

        _DispSysFsParsingCommand((char *)buf, &stStringCfg);
        DispDbgmgStore(&stStringCfg);
        return n;
    }
    return 0;
}

ssize_t check_dispdbgmg_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    return DispDbgmgShow(buf);
}

static DEVICE_ATTR(dbgmg,0644, check_dispdbgmg_show, check_dispdbgmg_store);

ssize_t check_disppq_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t n)
{
    if(NULL!=buf)
    {
        DrvDispOsStrConfig_t stStringCfg;

        _DispSysFsParsingCommand((char *)buf, &stStringCfg);

        DispPqStore(&stStringCfg);

        return n;
    }
    return 0;
}

ssize_t check_disppq_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    return DispPqShow(buf);
}

static DEVICE_ATTR(pq,0644, check_disppq_show, check_disppq_store);


ssize_t check_dispclk_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t n)
{
    if(NULL!=buf)
    {
        DrvDispOsStrConfig_t stStringCfg;

        _DispSysFsParsingCommand((char *)buf, &stStringCfg);
        DispClkStore(&stStringCfg);
        return n;
    }
    return 0;
}

ssize_t check_dispclk_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    return DispClkShow(buf);
}

static DEVICE_ATTR(clk,0644, check_dispclk_show, check_dispclk_store);


ssize_t check_dispturndrv_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t n)
{
    if(NULL!=buf)
    {
        DrvDispOsStrConfig_t stStringCfg;

        _DispSysFsParsingCommand((char *)buf, &stStringCfg);
        DispTurnDrvStore(&stStringCfg);
        return n;
    }
    return 0;
}

ssize_t check_dispturndrv_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    return DispTurnDrvShow(buf);
}

static DEVICE_ATTR(turndrv,0644, check_dispturndrv_show, check_dispturndrv_store);



void DrvDispSysfsInit(struct device *device)
{
    CamDeviceCreateFile(device, &dev_attr_dbgmg);
    CamDeviceCreateFile(device, &dev_attr_pq);
    CamDeviceCreateFile(device, &dev_attr_clk);
    CamDeviceCreateFile(device, &dev_attr_turndrv);
}
