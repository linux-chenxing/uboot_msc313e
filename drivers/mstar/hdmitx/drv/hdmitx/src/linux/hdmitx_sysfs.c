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
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/fs.h>
#include <asm/segment.h>
#include <asm/uaccess.h>
#include <linux/buffer_head.h>
#include <linux/path.h>
#include <linux/namei.h>
#include <linux/device.h>

#include "mhal_common.h"
#include "drv_hdmitx_os.h"
#include "cam_sysfs.h"
#include "hdmitx_debug.h"
#include "hal_hdmitx_chip.h"
#include "mhal_hdmitx_datatype.h"
#include "mhal_hdmitx.h"
#include "drv_hdmitx_if.h"
#include "hal_hdmitx_st.h"
#include "drv_hdmitx_ctx.h"
#include "hal_hdmitx_if.h"
//-------------------------------------------------------------------------------------------------
//  Defines & Macro
//-------------------------------------------------------------------------------------------------
#define HDMITXSYSFS_SPRINTF_STRCAT(str, _fmt, _args...) \
    do {                                   \
        char tmpstr[1024];                 \
        sprintf(tmpstr, _fmt, ## _args);   \
        strcat(str, tmpstr);               \
    }while(0)

#define HDMITXSYSFS_SPRINTF(str, _fmt, _args...)  sprintf(str, _fmt, ## _args)


#define HDMITXSYSFS_PRINTF(_fmt, _args...)                 \
        do{                                         \
            printf(PRINT_GREEN _fmt PRINT_NONE, ## _args);       \
        }while(0)

//-------------------------------------------------------------------------------------------------
//  structure
//-------------------------------------------------------------------------------------------------
typedef struct
{
    int argc;
    char *argv[100];
}HdmitxSysFsStrConfig_t;


//-------------------------------------------------------------------------------------------------
//  Variable
//-------------------------------------------------------------------------------------------------
bool bPtGenEn = 0;
u8   u8PtGenWidth = 0;
u32 u32HdmitxDbgmgLevel = 0;


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
int _HdmitxSysFsSplit(char **arr, char *str,  char* del)
{
    char *cur = str;
    char *token = NULL;
    int cnt = 0;

    token = strsep(&cur, del);
    while (token)
    {
        arr[cnt] = token;
        token = strsep(&cur, del);
        cnt++;
    }
    return cnt;
}

void _HdmitxDbgmgStore(HdmitxSysFsStrConfig_t *pstStringCfg)
{
    int ret;
    DrvHdmitxCtxConfig_t stHdmitxCtxCfg;
    if(pstStringCfg->argc < 1)
    {
        return;
    }

    ret = kstrtol(pstStringCfg->argv[0], 16, (long *)&u32HdmitxDbgmgLevel);
    MhalHdmitxSetDebugLevel(&stHdmitxCtxCfg, u32HdmitxDbgmgLevel);
    HDMITXSYSFS_PRINTF("%s %d, DebugLevel=%x\n", __FUNCTION__, __LINE__, u32HdmitxDbgmgLevel);
}

int _HdmitxDbgmgShow(char *DstBuf)
{
    int RetSprintf = -1;
    char *SrcBuf;

    SrcBuf = (char *)DrvHdmitxOsMemAlloc(1024*5);

    if(SrcBuf)
    {
        HDMITXSYSFS_SPRINTF_STRCAT(SrcBuf, "dbglevle: %08x \n", (unsigned int)u32HdmitxDbgmgLevel);
        RetSprintf = HDMITXSYSFS_SPRINTF(DstBuf, "%s", SrcBuf);
        DrvHdmitxOsMemRelease(SrcBuf);
    }
    return RetSprintf;
}


void _HdmitxPtGenStore(HdmitxSysFsStrConfig_t *pstStringCfg)
{
    #define HDMITXREG(bank, addr)              (*((volatile u16 *)(((u32)0xFD000000+(bank<<1U)) + ((addr)<<2U))))

    int ret;
    u16 u16Val;
    long PtGenEn, PtGenWidth;

    if(pstStringCfg->argc == 2)
    {
        ret = kstrtol(pstStringCfg->argv[0], 10, &PtGenEn);
        ret = kstrtol(pstStringCfg->argv[1], 10, &PtGenWidth);

        bPtGenEn = PtGenEn ? 1 : 0;
        u8PtGenWidth = (u8)PtGenWidth;

        u16Val = PtGenEn ? (PtGenWidth << 8 | 0x03) : 0x0000;
        HDMITXREG(0x112500, 0x05) = u16Val;
    }
    else
    {
        HDMITXSYSFS_PRINTF("[En] [Pt Wdith]\n");
    }
}

int _HdmitxPtGenShow(char *DstBuf)
{
    int RetSprintf = -1;
    char *SrcBuf;

    SrcBuf = (char *)DrvHdmitxOsMemAlloc(1024*5);

    if(SrcBuf)
    {
        HDMITXSYSFS_SPRINTF_STRCAT(SrcBuf, "ptgen=%d %d \n", bPtGenEn, u8PtGenWidth);
        RetSprintf = HDMITXSYSFS_SPRINTF(DstBuf, "%s", SrcBuf);
        DrvHdmitxOsMemRelease(SrcBuf);
    }
    return RetSprintf;
}

void _HdmitxClkStore(HdmitxSysFsStrConfig_t *pstStringCfg)
{
    int ret, idx;
    long Enable = 0;
    long ClkRate = 0;
    char *ClkName = NULL;
    bool abClkEn[HAL_HDMITX_CLK_NUM] = HAL_HDMITX_CLK_ON_SETTING;
    u32  au32ClkRate[HAL_HDMITX_CLK_NUM] = HAL_HDMITX_CLK_RATE_SETTING;
    u8   au8ClkName[HAL_HDMITX_CLK_NUM][20] = HAL_HDMITX_CLK_NAME;
    u8   au8ClkMuxAttr[HAL_HDMITX_CLK_NUM] = HAL_HDMITX_CLK_MUX_ATTR;
    DrvHdmitxCtxConfig_t stHdmitxCtxCfg;
    u32 i;

    if(pstStringCfg->argc == HAL_HDMITX_CLK_NUM + 2)
    {
        ClkName = pstStringCfg->argv[0];
        ret = kstrtol(pstStringCfg->argv[1], 10, &Enable);
        for(i=0; i<HAL_HDMITX_CLK_NUM; i++)
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
        HDMITXSYSFS_SPRINTF_STRCAT(SrcBuf, "----------------- CLK TREE  -----------------\n");
        HDMITXSYSFS_SPRINTF_STRCAT(SrcBuf, "clktree [En] [%s 0 ~ %d]\n", au8ClkMuxAttr[i] ? "CLKIdx": "ClkRate", HAL_HDMITX_CLK_NUM-1);
        HDMITXSYSFS_SPRINTF_STRCAT(SrcBuf, "----------------- DRV Update -----------------\n");
        HDMITXSYSFS_SPRINTF_STRCAT(SrcBuf, "[CLK Type] [En] [%s]\n",  au8ClkMuxAttr[i] ? "CLKIdx": "ClkRate");
        HDMITXSYSFS_SPRINTF_STRCAT(SrcBuf, "----------------- CLK Type ----------------- \n");
        for(i=0; i< HAL_HDMITX_CLK_NUM; i++)
        {
            HDMITXSYSFS_SPRINTF_STRCAT(SrcBuf, "%s, \n", au8ClkName[i]);
        }
        HDMITXSYSFS_PRINTF("%s\n", SrcBuf);
        CamOsMemRelease(SrcBuf);
        return;
    }

    if(strcmp(ClkName, "clktree") == 0)
    {
        if(Enable)
        {
            if(DrvHdmitxOsSetClkOn(au32ClkRate, HAL_HDMITX_CLK_NUM) == 0)
            {
                HDMITX_ERR("%s %d, Set Clk On Fail\n", __FUNCTION__, __LINE__);
            }
        }
        else
        {
            if(DrvHdmitxOsSetClkOff(au32ClkRate, HAL_HDMITX_CLK_NUM) == 0)
            {
                HDMITX_ERR("%s %d, Set Clk Off Fail\n", __FUNCTION__, __LINE__);
            }
        }
    }
    else
    {
        stHdmitxCtxCfg.pstHalCtx = (DrvHdmitxCtxHalContain_t *)CamOsMemAlloc(sizeof(DrvHdmitxCtxHalContain_t));

        if(stHdmitxCtxCfg.pstHalCtx)
        {
            if( DrvHdmitxIfGetClk((void *)&stHdmitxCtxCfg, abClkEn, au32ClkRate, HAL_HDMITX_CLK_NUM) != E_MHAL_HDMITX_RET_SUCCESS)
            {
                HDMITX_ERR("%s %d, Get Clk Fail\n", __FUNCTION__, __LINE__);
            }
            else
            {
                for(idx=0; idx<HAL_HDMITX_CLK_NUM; idx++)
                {
                    if(strcmp(ClkName, au8ClkName[idx]) == 0)
                    {
                        abClkEn[idx] = Enable ? 1 : 0,
                        au32ClkRate[idx] = ClkRate;
                        if(DrvHdmitxIfSetClk((void *)&stHdmitxCtxCfg, abClkEn, au32ClkRate, HAL_HDMITX_CLK_NUM) != E_MHAL_HDMITX_RET_SUCCESS)
                        {
                            HDMITX_ERR("%s %d, Set Clk Fail\n", __FUNCTION__, __LINE__);
                        }
                        break;
                    }
                }

                if(idx == HAL_HDMITX_CLK_NUM)
                {
                    HDMITX_ERR("%s %d, ClkName (%s) is not correct\n", __FUNCTION__, __LINE__, ClkName);
                }
            }
            CamOsMemRelease(stHdmitxCtxCfg.pstHalCtx);
        }
        else
        {
            HDMITX_ERR("%s %d, MemeAlloc Fail\n", __FUNCTION__, __LINE__);
        }
    }
}

int _HdmitxClkShow(char *DstBuf)
{
    bool abEn[HAL_HDMITX_CLK_NUM];
    u32 au32ClkRate[HAL_HDMITX_CLK_NUM];
    int RetSprintf = -1;
    char *SrcBuf;
    DrvHdmitxCtxConfig_t stHdmitxCtxCfg;
    char aCLKName[][20] = {"CLK_HDMI", "CLK_DISP_432", "CLK_DISP_216"};
    u32 i;

    SrcBuf = (char *)DrvHdmitxOsMemAlloc(1024*5);

    if(SrcBuf)
    {
        stHdmitxCtxCfg.pstHalCtx = (DrvHdmitxCtxHalContain_t *)CamOsMemAlloc(sizeof(DrvHdmitxCtxHalContain_t));

        if(stHdmitxCtxCfg.pstHalCtx)
        {
            if(DrvHdmitxIfGetClk((void *)&stHdmitxCtxCfg, abEn, au32ClkRate, HAL_HDMITX_CLK_NUM) != E_MHAL_HDMITX_RET_SUCCESS)
            {
                HDMITX_ERR("%s %d, Get CLK Fail\n", __FUNCTION__, __LINE__);
            }
            CamOsMemRelease(stHdmitxCtxCfg.pstHalCtx);
            for(i=0; i<HAL_HDMITX_CLK_NUM; i++)
            {
                HDMITXSYSFS_SPRINTF_STRCAT(SrcBuf, "%-15s: En:%d, ClkRate:%d \n", aCLKName[i], abEn[i], au32ClkRate[i]);
            }
            RetSprintf = HDMITXSYSFS_SPRINTF(DstBuf, "%s", SrcBuf);
        }
        else
        {
            HDMITX_ERR("%s %d, MemAlloc Fail\n", __FUNCTION__, __LINE__);
        }
        DrvHdmitxOsMemRelease(SrcBuf);

    }
    return RetSprintf;
}


void _HdmitxForceCfgStore(HdmitxSysFsStrConfig_t *pstStringCfg)
{
    int ret;
    long OutpuModeEn;
    HalHdmitxQueryConfig_t stQueryCfg;
    HalHdmitxForceAttrConfig_t stForceAttrCfg;
    DrvHdmitxCtxConfig_t stHdmitxCtxCfg;

    if(pstStringCfg->argc == 2)
    {
        ret = kstrtol(pstStringCfg->argv[0], 10, &OutpuModeEn);

        stQueryCfg.stInCfg.enQueryType = E_HAL_HDMITX_QUERY_FORCE_CFG_SET;
        stQueryCfg.stInCfg.u32CfgSize = sizeof(HalHdmitxForceAttrConfig_t);
        stQueryCfg.stInCfg.pInCfg = &stForceAttrCfg;

        stForceAttrCfg.bOutputMode = OutpuModeEn ? 1 :0;

        stForceAttrCfg.enOutputMode =  strcmp(pstStringCfg->argv[1], "hdmi") == 0 ?
                                       E_MHAL_HDMITX_OUTPUT_MODE_HDMI :
                                       E_MHAL_HDMITX_OUTPUT_MODE_DVI;

        HalHdmitxIfInit();

        if(HalHdmitxIfQuery((void *)&stHdmitxCtxCfg, &stQueryCfg))
        {
            if(stQueryCfg.stOutCfg.pSetFunc)
            {
                stQueryCfg.stOutCfg.pSetFunc((void *)&stHdmitxCtxCfg, stQueryCfg.stInCfg.pInCfg);
            }
        }
    }
    else
    {
        HDMITXSYSFS_PRINTF("[OutputModeEn] [OutputMode]\n");
    }
}

int _HdmitxForceCfgShow(char *DstBuf)
{
    int RetSprintf = -1;
    char *SrcBuf;
    HalHdmitxQueryConfig_t stQueryCfg;
    HalHdmitxForceAttrConfig_t stForceAttrCfg;
    DrvHdmitxCtxConfig_t stHdmitxCtxCfg;

    SrcBuf = (char *)DrvHdmitxOsMemAlloc(1024*5);

    if(SrcBuf)
    {
        stQueryCfg.stInCfg.enQueryType = E_HAL_HDMITX_QUERY_FORCE_CFG_GET;
        stQueryCfg.stInCfg.u32CfgSize = sizeof(HalHdmitxForceAttrConfig_t);
        stQueryCfg.stInCfg.pInCfg = &stForceAttrCfg;

        memset(&stForceAttrCfg, 0 ,sizeof(HalHdmitxForceAttrConfig_t));

        HalHdmitxIfInit();

        if(HalHdmitxIfQuery((void *)&stHdmitxCtxCfg, &stQueryCfg))
        {
            if(stQueryCfg.stOutCfg.pSetFunc)
            {
                stQueryCfg.stOutCfg.pSetFunc((void *)&stHdmitxCtxCfg, stQueryCfg.stInCfg.pInCfg);
            }
        }

        HDMITXSYSFS_SPRINTF_STRCAT(SrcBuf, "ForceOutputMode(%d, %s) \n", stForceAttrCfg.bOutputMode, PARSING_HAL_OUTPUT_MODE(stForceAttrCfg.enOutputMode));
        RetSprintf = HDMITXSYSFS_SPRINTF(DstBuf, "%s", SrcBuf);
        DrvHdmitxOsMemRelease(SrcBuf);
    }
    return RetSprintf;
}



void _HdmitxSysFsParsingCommand(char *str, HdmitxSysFsStrConfig_t *pstStrCfg)
{
    char del[] = " ";
    int len;

    pstStrCfg->argc = _HdmitxSysFsSplit(pstStrCfg->argv, (char *)str, del);
    len = strlen(pstStrCfg->argv[pstStrCfg->argc-1]);
    pstStrCfg->argv[pstStrCfg->argc-1][len-1] = '\0';
}



ssize_t check_hdmitxdbgmg_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t n)
{
    if(NULL!=buf)
    {
        HdmitxSysFsStrConfig_t stStringCfg;

        _HdmitxSysFsParsingCommand((char *)buf, &stStringCfg);
        _HdmitxDbgmgStore(&stStringCfg);
        return n;
    }
    return 0;
}

ssize_t check_hdmitxdbgmg_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    return _HdmitxDbgmgShow(buf);
}

static DEVICE_ATTR(dbgmg,0644, check_hdmitxdbgmg_show, check_hdmitxdbgmg_store);


ssize_t check_hdmitxptgen_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t n)
{
    if(NULL!=buf)
    {
        HdmitxSysFsStrConfig_t stStringCfg;

        _HdmitxSysFsParsingCommand((char *)buf, &stStringCfg);
        _HdmitxPtGenStore(&stStringCfg);
        return n;
    }
    return 0;
}

ssize_t check_hdmitxptgen_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    return _HdmitxPtGenShow(buf);
}

static DEVICE_ATTR(ptgen,0644, check_hdmitxptgen_show, check_hdmitxptgen_store);



ssize_t check_hdmitxclk_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t n)
{
    if(NULL!=buf)
    {
        HdmitxSysFsStrConfig_t stStringCfg;

        _HdmitxSysFsParsingCommand((char *)buf, &stStringCfg);
        _HdmitxClkStore(&stStringCfg);
        return n;
    }
    return 0;
}

ssize_t check_hdmitxclk_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    return _HdmitxClkShow(buf);
}

static DEVICE_ATTR(clk, 0644, check_hdmitxclk_show, check_hdmitxclk_store);

ssize_t check_hdmitxforcecfg_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t n)
{
    if(NULL!=buf)
    {
        HdmitxSysFsStrConfig_t stStringCfg;

        _HdmitxSysFsParsingCommand((char *)buf, &stStringCfg);
        _HdmitxForceCfgStore(&stStringCfg);
        return n;
    }
    return 0;
}

ssize_t check_hdmitxforcecfg_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    return _HdmitxForceCfgShow(buf);
}

static DEVICE_ATTR(forcecfg,0644, check_hdmitxforcecfg_show, check_hdmitxforcecfg_store);


void DrvHdmitxSysfsInit(struct device *device)
{
    int ret;

    ret = CamDeviceCreateFile(device, &dev_attr_dbgmg);
    ret = CamDeviceCreateFile(device, &dev_attr_ptgen);
    ret = CamDeviceCreateFile(device, &dev_attr_clk);
    ret = CamDeviceCreateFile(device, &dev_attr_forcecfg);

}
