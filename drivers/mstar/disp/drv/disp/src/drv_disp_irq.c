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

#define _DRV_DISP_IRQ_C_

#include "drv_disp_os.h"
#include "hal_disp_common.h"
#include "hal_disp_chip.h"
#include "hal_disp_st.h"
#include "hal_disp_if.h"
#include "hal_disp_util.h"
#include "hal_disp_irq.h"
#include "hal_disp.h"
#include "disp_debug.h"
#include "mhal_common.h"
#include "mhal_cmdq.h"
#include "mhal_disp_datatype.h"
#include "drv_disp_ctx.h"
//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Defines & Macro
//-------------------------------------------------------------------------------------------------
#define ENABLE_INTENAL_IRQ_DBG  0

//-------------------------------------------------------------------------------------------------
//  structure
//-------------------------------------------------------------------------------------------------
typedef struct
{
    u32 u32IrqNum;
    bool bInit;
}DrvDispIrqConfig_t;
//-------------------------------------------------------------------------------------------------
//  Variable
//-------------------------------------------------------------------------------------------------
DrvDispIrqConfig_t gstDispIrqCfg[HAL_DISP_IRQ_ID_MAX] = HAL_DISP_IRQ_CFG;
bool gbDispIrqInit = 0;
extern u32 gu32InternalIsrFlag ;
//-------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
void DrvDispIrqSetIsrNum(u32 u32DevId, u32 u32IsrNum)
{
    if(u32DevId >= HAL_DISP_IRQ_ID_MAX)
    {
        DISP_ERR("%s %d, DevId too big: %ld\n", __FUNCTION__, __LINE__, u32DevId);
        return;
    }
    gstDispIrqCfg[u32DevId].bInit = 1;
    gstDispIrqCfg[u32DevId].u32IrqNum = u32IsrNum;
}

bool DrvDispIrqGetIsrNumByDevId(u32 u32DevId, u32 *pu32IsrNum)
{
    bool bRet = 1;
    HalDispIrqIoctlConfig_t stIrqIoctlCfg;
    DrvDispCtxConfig_t stDispCtxCfg;
    DrvDispCtxContain_t stDispCtxContain;
    DrvDispCtxDeviceContain_t stDevContain;
    u8 u8Idx;

    // Create a Fake Ctx
    stDispCtxCfg.enCtxType = E_DISP_CTX_TYPE_DEVICE;
    stDispCtxCfg.u32Idx = u32DevId;
    stDispCtxCfg.pstCtxContain = &stDispCtxContain;
    stDispCtxContain.pstDevContain[u32DevId] = &stDevContain;
    stDevContain.u32DevId = u32DevId;

    stIrqIoctlCfg.enIoctlType = E_HAL_DISP_IRQ_IOCTL_GET_ID;
    stIrqIoctlCfg.enIrqType = E_HAL_DISP_IRQ_TYPE_NONE;
    stIrqIoctlCfg.pDispCtx = &stDispCtxCfg;
    stIrqIoctlCfg.pParam = (void *)&u8Idx;
    HalDispIrqIoCtl(&stIrqIoctlCfg);

    if(gstDispIrqCfg[u8Idx].bInit)
    {
        *pu32IsrNum = gstDispIrqCfg[u8Idx].u32IrqNum;
        DISP_DBG(DISP_DBG_LEVEL_IRQ, "%s %d, DevId:%ld, Idx:%d, IrqNum=%ld\n",
            __FUNCTION__, __LINE__,
            u32DevId,
            u8Idx,
            *pu32IsrNum);
    }
    else
    {
        bRet = 0;
        DISP_ERR("%s %d, Irq Idx %d, not init\n", __FUNCTION__, __LINE__, u8Idx);
    }
    return bRet;
}

bool DrvDispIrqGetIsrNum(void *pDevCtx, u32 *pu32IsrNum)
{
    bool bRet = 1;
    DrvDispCtxConfig_t *pstDispCtx = (DrvDispCtxConfig_t *)pDevCtx;
    HalDispIrqIoctlConfig_t stIrqIoctlCfg;
    u8 u8Idx;

    if(pstDispCtx->enCtxType == E_DISP_CTX_TYPE_DEVICE)
    {
        stIrqIoctlCfg.enIoctlType = E_HAL_DISP_IRQ_IOCTL_GET_ID;
        stIrqIoctlCfg.enIrqType = E_HAL_DISP_IRQ_TYPE_NONE;
        stIrqIoctlCfg.pDispCtx = pDevCtx;
        stIrqIoctlCfg.pParam = (void *)&u8Idx;
        HalDispIrqIoCtl(&stIrqIoctlCfg);

        if(gstDispIrqCfg[u8Idx].bInit)
        {
            *pu32IsrNum = gstDispIrqCfg[u8Idx].u32IrqNum;
            DISP_DBG(DISP_DBG_LEVEL_IRQ, "%s %d, CtxId=%ld, DevId:%ld, Idx:%d, IrqNum=%ld\n",
                __FUNCTION__, __LINE__,
                pstDispCtx->u32Idx,
                pstDispCtx->pstCtxContain->pstDevContain[pstDispCtx->u32Idx]->u32DevId,
                u8Idx,
                *pu32IsrNum);
        }
        else
        {
            bRet = 0;
            DISP_ERR("%s %d, Irq Idx %d, not init\n", __FUNCTION__, __LINE__, u8Idx);
        }
    }
    else if(pstDispCtx->enCtxType == E_DISP_CTX_TYPE_DMA)
    {
        stIrqIoctlCfg.enIoctlType = E_HAL_DISP_IRQ_IOCTL_DMA_GET_ID;
        stIrqIoctlCfg.enIrqType = E_HAL_DISP_IRQ_TYPE_NONE;
        stIrqIoctlCfg.pDispCtx = pDevCtx;
        stIrqIoctlCfg.pParam = (void *)&u8Idx;

        if(HalDispIrqIoCtl(&stIrqIoctlCfg) == 0)
        {
            DISP_ERR("%s %d, Irq Ioctl(%d) Fail\n", __FUNCTION__, __LINE__, stIrqIoctlCfg.enIoctlType);
            *pu32IsrNum = 0;
        }
        else
        {
            if(gstDispIrqCfg[u8Idx].bInit)
            {
                *pu32IsrNum = gstDispIrqCfg[u8Idx].u32IrqNum;
                DISP_DBG(DISP_DBG_LEVEL_IRQ_DMA, "%s %d, CtxId=%ld, DmaId:%ld, Idx:%d, IrqNum=%ld\n",
                    __FUNCTION__, __LINE__,
                    pstDispCtx->u32Idx,
                    pstDispCtx->pstCtxContain->pstDmaContain[pstDispCtx->u32Idx]->u32DmaId,
                    u8Idx,
                    *pu32IsrNum);
            }
            else
            {
                bRet = 0;
                DISP_ERR("%s %d, Irq Idx %d, not init\n", __FUNCTION__, __LINE__, u8Idx);
            }
        }
    }
    else
    {
        bRet = 0;
        DISP_ERR("%s %d, CtxType=%s, Wrong\n",
            __FUNCTION__, __LINE__, PARSING_CTX_TYPE(pstDispCtx->enCtxType));
    }
    return bRet;
}


bool DrvDispIrqEnable(void *pDevCtx, u32 u32DevIrq, bool bEnable)
{
    bool bRet = 1;
    HalDispIrqIoctlConfig_t stIrqIoctlCfg;
    DrvDispCtxConfig_t *pstDispCtx = (DrvDispCtxConfig_t *)pDevCtx;
    DrvDispCtxDeviceContain_t *pstDevContain = NULL;
    u8 u8Idx;

    if(pstDispCtx->enCtxType == E_DISP_CTX_TYPE_DEVICE)
    {
        pstDevContain = pstDispCtx->pstCtxContain->pstDevContain[pstDispCtx->u32Idx];
        stIrqIoctlCfg.enIoctlType = E_HAL_DISP_IRQ_IOCTL_GET_ID;
        stIrqIoctlCfg.enIrqType = E_HAL_DISP_IRQ_TYPE_NONE;
        stIrqIoctlCfg.pDispCtx = pDevCtx;
        stIrqIoctlCfg.pParam = (void *)&u8Idx;
        HalDispIrqIoCtl(&stIrqIoctlCfg);

        if(gstDispIrqCfg[u8Idx].u32IrqNum == u32DevIrq)
        {
            stIrqIoctlCfg.enIoctlType = E_HAL_DISP_IRQ_IOCTL_ENABLE;
            stIrqIoctlCfg.enIrqType = E_HAL_DISP_IRQ_TYPE_VSYNC;
            stIrqIoctlCfg.pDispCtx = pDevCtx;
            stIrqIoctlCfg.pParam = (void *)&bEnable;
            HalDispIrqIoCtl(&stIrqIoctlCfg);
        }
        else
        {
            bRet = 0;
            DISP_ERR("%s %d, IrqNum not match %ld != %ld\n",
                __FUNCTION__, __LINE__, u32DevIrq, gstDispIrqCfg[pstDevContain->u32DevId].u32IrqNum);
        }
    }
    else if(pstDispCtx->enCtxType == E_DISP_CTX_TYPE_DMA)
    {
        stIrqIoctlCfg.enIoctlType = E_HAL_DISP_IRQ_IOCTL_DMA_GET_ID;
        stIrqIoctlCfg.enIrqType = E_HAL_DISP_IRQ_TYPE_NONE;
        stIrqIoctlCfg.pDispCtx = pDevCtx;
        stIrqIoctlCfg.pParam = (void *)&u8Idx;
        HalDispIrqIoCtl(&stIrqIoctlCfg);

        if(gstDispIrqCfg[u8Idx].u32IrqNum == u32DevIrq)
        {
            stIrqIoctlCfg.enIoctlType = E_HAL_DISP_IRQ_IOCTL_DMA_ENABLE;
            stIrqIoctlCfg.enIrqType = E_HAL_DISP_IRQ_TYPE_DMA_ACTIVE_OFF;
            stIrqIoctlCfg.pDispCtx = pDevCtx;
            stIrqIoctlCfg.pParam = (void *)&bEnable;
            HalDispIrqIoCtl(&stIrqIoctlCfg);
        }
        else
        {
            bRet = 0;
            DISP_ERR("%s %d, IrqNum not match %ld != %ld\n",
                __FUNCTION__, __LINE__, u32DevIrq, gstDispIrqCfg[pstDevContain->u32DevId].u32IrqNum);
        }
    }
    else
    {
        bRet = 0;
        DISP_ERR("%s %d, CtxType=%s, Wrong\n",
            __FUNCTION__, __LINE__, PARSING_CTX_TYPE(pstDispCtx->enCtxType));
    }
    return bRet;
}

bool DrvDispIrqGetFlag(void *pDevCtx, MHAL_DISP_IRQFlag_t *pstIrqFlag)
{
    bool bRet = 1;
    u32 u32IrqFlag;
    HalDispIrqIoctlConfig_t stIrqIoctlCfg;
    DrvDispCtxConfig_t *pstDispCtx = (DrvDispCtxConfig_t *)pDevCtx;

    if(pstDispCtx->enCtxType == E_DISP_CTX_TYPE_DEVICE)
    {
        stIrqIoctlCfg.enIoctlType = E_HAL_DISP_IRQ_IOCTL_GET_FLAG;
        stIrqIoctlCfg.enIrqType = E_HAL_DISP_IRQ_TYPE_VSYNC;
        stIrqIoctlCfg.pDispCtx = pDevCtx;
        stIrqIoctlCfg.pParam = (void *)&u32IrqFlag;
        HalDispIrqIoCtl(&stIrqIoctlCfg);
        pstIrqFlag->u32IrqFlag = u32IrqFlag;
        pstIrqFlag->u32IrqMask = E_HAL_DISP_IRQ_TYPE_VSYNC;
    }
    else if(pstDispCtx->enCtxType == E_DISP_CTX_TYPE_DMA)
    {
        stIrqIoctlCfg.enIoctlType = E_HAL_DISP_IRQ_IOCTL_DMA_GET_FLAG;
        stIrqIoctlCfg.enIrqType = E_HAL_DISP_IRQ_TYPE_DMA_ACTIVE_OFF;
        stIrqIoctlCfg.pDispCtx = pDevCtx;
        stIrqIoctlCfg.pParam = (void *)&u32IrqFlag;
        HalDispIrqIoCtl(&stIrqIoctlCfg);
        pstIrqFlag->u32IrqFlag = u32IrqFlag;
        pstIrqFlag->u32IrqMask = E_HAL_DISP_IRQ_TYPE_DMA_ACTIVE_OFF;
    }
    else
    {
        bRet = 0;
        DISP_ERR("%s %d, CtxType=%s, Wrong\n",
            __FUNCTION__, __LINE__, PARSING_CTX_TYPE(pstDispCtx->enCtxType));
    }
    return bRet;

}

bool DrvDispIrqClear(void *pDevCtx, void* pData)
{
    bool bRet = 1;
    HalDispIrqIoctlConfig_t stIrqIoctlCfg;
    MHAL_DISP_IRQFlag_t *pstIrqFlag = (MHAL_DISP_IRQFlag_t *)pData;
    DrvDispCtxConfig_t *pstDispCtx = (DrvDispCtxConfig_t *)pDevCtx;

    if(pstDispCtx->enCtxType == E_DISP_CTX_TYPE_DEVICE)
    {
        stIrqIoctlCfg.enIoctlType = E_HAL_DISP_IRQ_IOCTL_CLEAR;
        stIrqIoctlCfg.enIrqType = pstIrqFlag->u32IrqFlag & pstIrqFlag->u32IrqMask;
        stIrqIoctlCfg.pDispCtx = pDevCtx;
        stIrqIoctlCfg.pParam = NULL;
        HalDispIrqIoCtl(&stIrqIoctlCfg);
    }
    else if(pstDispCtx->enCtxType == E_DISP_CTX_TYPE_DMA)
    {
        stIrqIoctlCfg.enIoctlType = E_HAL_DISP_IRQ_IOCTL_DMA_CLEAR;
        stIrqIoctlCfg.enIrqType = pstIrqFlag->u32IrqFlag & pstIrqFlag->u32IrqMask;
        stIrqIoctlCfg.pDispCtx = pDevCtx;
        stIrqIoctlCfg.pParam = NULL;
        HalDispIrqIoCtl(&stIrqIoctlCfg);
    }
    else
    {
        bRet = 0;
        DISP_ERR("%s %d, CtxType=%s, Wrong\n",
            __FUNCTION__, __LINE__, PARSING_CTX_TYPE(pstDispCtx->enCtxType));
    }
    return bRet;
}

//------------------------------------------------------------------------------
// Internal Isr
//------------------------------------------------------------------------------

#if DISP_TIMEZONE_ISR_SUPPORT
static irqreturn_t _DrvDispIrqTimeZoneCb(int eIntNum, void* pstDevParam)
{
    u32 u32Flag[HAL_DISP_DEVICE_MAX];
    HalDispIrqIoctlConfig_t stIrqIoctlCfg[HAL_DISP_DEVICE_MAX];
    DrvDispCtxConfig_t *pstDispCtxCfg = (DrvDispCtxConfig_t *)pstDevParam;
    DrvDispCtxDeviceContain_t *pstDevContain = pstDispCtxCfg->pstCtxContain->pstDevContain[pstDispCtxCfg->u32Idx];
    u32 u32DevId = pstDevContain->u32DevId;

#if ENABLE_INTENAL_IRQ_DBG
    #define FLAG_VSYNC_P             0
    #define FLAG_VDE_P               1
    #define FLAG_VDE_N               2
    #define FLAG_VSYNC_P_VDE_P       3
    #define FLAG_VSYNC_P_VDE_N       4
    #define FLAG_VDE_P_VDE_N         5
    #define FLAG_VSYNC_P_VDE_P_VDE_N 6
    #define FLGA_UNKNOWN             7
    #define FLAG_NUM                 8

    static u8 u8Num[HAL_DISP_DEVICE_MAX][FLAG_NUM] =
    {
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
    };

    static u8 u8Cnt[HAL_DISP_DEVICE_MAX] = {0, 0};
#endif

    stIrqIoctlCfg[u32DevId].enIoctlType = E_HAL_DISP_IRQ_IOCTL_TIMEZONE_GET_FLAG;
    stIrqIoctlCfg[u32DevId].enIrqType = E_HAL_DISP_IRQ_TYPE_TIMEZONE;
    stIrqIoctlCfg[u32DevId].pDispCtx = pstDevParam;
    stIrqIoctlCfg[u32DevId].pParam = (void *)&u32Flag[u32DevId];
    HalDispIrqIoCtl(&stIrqIoctlCfg[u32DevId]);

    stIrqIoctlCfg[u32DevId].enIoctlType = E_HAL_DISP_IRQ_IOCTL_TIMEZONE_CLEAR;
    stIrqIoctlCfg[u32DevId].enIrqType = u32Flag[u32DevId];
    stIrqIoctlCfg[u32DevId].pDispCtx = pstDevParam;
    stIrqIoctlCfg[u32DevId].pParam = NULL;
    HalDispIrqIoCtl(&stIrqIoctlCfg[u32DevId]);

#if ENABLE_INTENAL_IRQ_DBG

    if(u32Flag[u32DevId] == E_HAL_DISP_IRQ_TYPE_TIMEZONE_VSYNC_POSITIVE)
    {
        u8Num[u32DevId][FLAG_VSYNC_P]++;
    }
    else if(u32Flag[u32DevId] == E_HAL_DISP_IRQ_TYPE_TIMEZONE_VDE_POSITIVE)
    {
        u8Num[u32DevId][FLAG_VDE_P]++;
    }
    else if(u32Flag[u32DevId] == E_HAL_DISP_IRQ_TYPE_TIMEZONE_VDE_NEGATIVE)
    {
        u8Num[u32DevId][FLAG_VDE_N]++;
    }
    else if(u32Flag[u32DevId] == (E_HAL_DISP_IRQ_TYPE_TIMEZONE_VSYNC_POSITIVE | E_HAL_DISP_IRQ_TYPE_TIMEZONE_VDE_POSITIVE))
    {
        u8Num[u32DevId][FLAG_VSYNC_P_VDE_P]++;
    }
    else if(u32Flag[u32DevId] == (E_HAL_DISP_IRQ_TYPE_TIMEZONE_VSYNC_POSITIVE | E_HAL_DISP_IRQ_TYPE_TIMEZONE_VDE_NEGATIVE))
    {
        u8Num[u32DevId][FLAG_VSYNC_P_VDE_N]++;
    }
    else if(u32Flag[u32DevId] == (E_HAL_DISP_IRQ_TYPE_TIMEZONE_VDE_POSITIVE | E_HAL_DISP_IRQ_TYPE_TIMEZONE_VDE_NEGATIVE))
    {
        u8Num[u32DevId][FLAG_VDE_P_VDE_N]++;
    }
    else if(u32Flag[u32DevId] == (E_HAL_DISP_IRQ_TYPE_TIMEZONE_VSYNC_POSITIVE | E_HAL_DISP_IRQ_TYPE_TIMEZONE_VDE_POSITIVE | E_HAL_DISP_IRQ_TYPE_TIMEZONE_VDE_NEGATIVE))
    {
        u8Num[u32DevId][FLAG_VSYNC_P_VDE_P_VDE_N]++;
    }
    else
    {
        u8Num[u32DevId][FLGA_UNKNOWN]++;
    }
    u8Cnt[u32DevId]++;

    if(u8Cnt[u32DevId]== 150)
    {
        DISP_MSG("%s %d:: DevId:%d, VsynP:%d, VdeP:%d VdeN:%d, VsynPVdeP:%d, VsyncPVdeN:%d, VdePVdeN:%d VsyncPVdePVdeN:%d Unknown:%d\n",
            __FUNCTION__, __LINE__,
            u32DevId,
            u8Num[u32DevId][FLAG_VSYNC_P],
            u8Num[u32DevId][FLAG_VDE_P],
            u8Num[u32DevId][FLAG_VDE_N],
            u8Num[u32DevId][FLAG_VSYNC_P_VDE_P],
            u8Num[u32DevId][FLAG_VSYNC_P_VDE_N],
            u8Num[u32DevId][FLAG_VDE_P_VDE_N],
            u8Num[u32DevId][FLAG_VSYNC_P_VDE_P_VDE_N],
            u8Num[u32DevId][FLGA_UNKNOWN]);

        u8Cnt[u32DevId] = 0;
        u8Num[u32DevId][FLAG_VSYNC_P] = 0;
        u8Num[u32DevId][FLAG_VDE_P] = 0;
        u8Num[u32DevId][FLAG_VDE_N] = 0;
        u8Num[u32DevId][FLAG_VSYNC_P_VDE_P] = 0;
        u8Num[u32DevId][FLAG_VSYNC_P_VDE_N] = 0;
        u8Num[u32DevId][FLAG_VDE_P_VDE_N] = 0;
        u8Num[u32DevId][FLAG_VSYNC_P_VDE_P_VDE_N] = 0;
        u8Num[u32DevId][FLGA_UNKNOWN] = 0;
    }
#endif

    return IRQ_HANDLED;
}
#endif

bool _DrvDispCreateTimeZoneIsr(DrvDispCtxConfig_t *pDispCtx)
{
#if DISP_TIMEZONE_ISR_SUPPORT
    s32 s32IrqRet;
    u32 u32IrqNum;
    bool bSupported;
    HalDispIrqIoctlConfig_t stIrqIoctlCfg;
    DrvDispCtxDeviceContain_t *pstDevContain = NULL;

    stIrqIoctlCfg.enIoctlType = E_HAL_DISP_IRQ_IOCTL_TIMEZONE_SUPPORTED;
    stIrqIoctlCfg.enIrqType = E_HAL_DISP_IRQ_TYPE_NONE;
    stIrqIoctlCfg.pDispCtx = (void *)pDispCtx;
    stIrqIoctlCfg.pParam = (void *)&bSupported;
    HalDispIrqIoCtl(&stIrqIoctlCfg);

    if(bSupported)
    {
        u8 u8DeviceIdx;
        bool bEnable;

        stIrqIoctlCfg.enIoctlType = E_HAL_DISP_IRQ_IOCTL_TIMEZONE_GET_ID;
        stIrqIoctlCfg.enIrqType = E_HAL_DISP_IRQ_TYPE_NONE;
        stIrqIoctlCfg.pDispCtx = (void *)pDispCtx;
        stIrqIoctlCfg.pParam = (void *)&u8DeviceIdx;
        HalDispIrqIoCtl(&stIrqIoctlCfg);

        u32IrqNum = gstDispIrqCfg[u8DeviceIdx].u32IrqNum;

        pstDevContain = pDispCtx->pstCtxContain->pstDevContain[pDispCtx->u32Idx];

        s32IrqRet = request_irq(u32IrqNum,
                                _DrvDispIrqTimeZoneCb,
                                (IRQF_TRIGGER_NONE | IRQF_SHARED),
                                pstDevContain->u32DevId ?
                                "mdisp_interisr1" : "mdisp_interisr0",
                                (void *)pDispCtx);
        if(s32IrqRet)
        {
            DISP_ERR("Attach Irq Fail\n");
            return 0;
        }
        else
        {
            DISP_DBG(DISP_DBG_LEVEL_IRQ_TIMEZONE, "%s %d, IrqNum=%d\n",
                __FUNCTION__, __LINE__, u32IrqNum);
        }

        disable_irq(u32IrqNum);
        enable_irq(u32IrqNum);

        bEnable = 1;
        stIrqIoctlCfg.enIoctlType = E_HAL_DISP_IRQ_IOCTL_TIMEZONE_ENABLE;
        stIrqIoctlCfg.enIrqType = E_HAL_DISP_IRQ_TYPE_TIMEZONE;
        stIrqIoctlCfg.pDispCtx = (void *)pDispCtx;
        stIrqIoctlCfg.pParam = (void *)&bEnable;
        HalDispIrqIoCtl(&stIrqIoctlCfg);
    }
    else
    {
        DISP_DBG(DISP_DBG_LEVEL_IRQ_TIMEZONE, "%s %d, Not Support\n", __FUNCTION__, __LINE__);
    }
#endif

    return 1;
}


#if DISP_VGA_HPD_ISR_SUPPORT
static irqreturn_t _DrvDispIrqVgaHpdIsrCb(int eIntNum, void* pstDevParam)
{
    u32 u32Flag;
    HalDispIrqIoctlConfig_t stIrqIoctlCfg;

    stIrqIoctlCfg.enIoctlType = E_HAL_DISP_IRQ_IOCTL_VGA_HPD_GET_FLAG;
    stIrqIoctlCfg.enIrqType = E_HAL_DISP_IRQ_TYPE_VGA_HPD_ON_OFF;
    stIrqIoctlCfg.pDispCtx = pstDevParam;
    stIrqIoctlCfg.pParam = (void *)&u32Flag;
    HalDispIrqIoCtl(&stIrqIoctlCfg);

    stIrqIoctlCfg.enIoctlType = E_HAL_DISP_IRQ_IOCTL_VGA_HPD_CLEAR;
    stIrqIoctlCfg.enIrqType = u32Flag;
    stIrqIoctlCfg.pDispCtx = pstDevParam;
    stIrqIoctlCfg.pParam = NULL;
    HalDispIrqIoCtl(&stIrqIoctlCfg);

    HalDispIrqSetDacEn((u32Flag & E_HAL_DISP_IRQ_TYPE_VGA_HPD_OFF) ? 0 : 1);

    DISP_DBG(DISP_DBG_LEVEL_IRQ_VGA_HPD, "%s %d VgaHpd:%s\n",
        __FUNCTION__, __LINE__, (u32Flag & E_HAL_DISP_IRQ_TYPE_VGA_HPD_ON) ? "ON":"OFF");

    return IRQ_HANDLED;
}
#endif

bool _DrvDispIrqCreateVgaHpdIsr(DrvDispCtxConfig_t *pDispCtx)
{
#if DISP_VGA_HPD_ISR_SUPPORT
    s32 s32IrqRet;
    u32 u32IrqNum;
    bool bSupported;
    HalDispIrqIoctlConfig_t stIrqIoctlCfg;

    stIrqIoctlCfg.enIoctlType = E_HAL_DISP_IRQ_IOCTL_VGA_HPD_SUPPORTED;
    stIrqIoctlCfg.enIrqType = E_HAL_DISP_IRQ_TYPE_NONE;
    stIrqIoctlCfg.pDispCtx = (void *)pDispCtx;
    stIrqIoctlCfg.pParam = (void *)&bSupported;
    HalDispIrqIoCtl(&stIrqIoctlCfg);

    if(bSupported)
    {
        u8 u8DeviceIdx;
        bool bEnable;

        stIrqIoctlCfg.enIoctlType = E_HAL_DISP_IRQ_IOCTL_VGA_HPD_GET_ID;
        stIrqIoctlCfg.enIrqType = E_HAL_DISP_IRQ_TYPE_NONE;
        stIrqIoctlCfg.pDispCtx = (void *)pDispCtx;
        stIrqIoctlCfg.pParam = (void *)&u8DeviceIdx;
        HalDispIrqIoCtl(&stIrqIoctlCfg);

        u32IrqNum = gstDispIrqCfg[u8DeviceIdx].u32IrqNum;

        s32IrqRet = request_irq(u32IrqNum, _DrvDispIrqVgaHpdIsrCb, IRQF_TRIGGER_NONE, "mdisp_vgahpdisr", (void *)pDispCtx);
        if(s32IrqRet)
        {
            DISP_ERR("Attach Irq Fail\n");
            return 0;
        }
        else
        {
            DISP_DBG(DISP_DBG_LEVEL_IRQ_VGA_HPD, "%s %d, IrqNum=%d\n",
                __FUNCTION__, __LINE__, u32IrqNum);
        }

        disable_irq(u32IrqNum);
        enable_irq(u32IrqNum);

        bEnable = 1;
        stIrqIoctlCfg.enIoctlType = E_HAL_DISP_IRQ_IOCTL_VGA_HPD_ENABLE;
        stIrqIoctlCfg.enIrqType = E_HAL_DISP_IRQ_TYPE_VGA_HPD_ON_OFF;
        stIrqIoctlCfg.pDispCtx = (void *)pDispCtx;
        stIrqIoctlCfg.pParam = (void *)&bEnable;
        HalDispIrqIoCtl(&stIrqIoctlCfg);
    }
    else
    {
        DISP_DBG(DISP_DBG_LEVEL_IRQ_VGA_HPD, "%s %d, Not Support\n", __FUNCTION__, __LINE__);
    }
#endif
    return 1;
}

bool _DrvDispIrqDestroyTimeZoneIsr(DrvDispCtxConfig_t *pDispCtx)
{
#if DISP_TIMEZONE_ISR_SUPPORT
    u32 u32IrqNum;
    bool bSupported;
    HalDispIrqIoctlConfig_t stIrqIoctlCfg;

    stIrqIoctlCfg.enIoctlType = E_HAL_DISP_IRQ_IOCTL_TIMEZONE_SUPPORTED;
    stIrqIoctlCfg.enIrqType = E_HAL_DISP_IRQ_TYPE_NONE;
    stIrqIoctlCfg.pDispCtx = (void *)pDispCtx;
    stIrqIoctlCfg.pParam = (void *)&bSupported;
    HalDispIrqIoCtl(&stIrqIoctlCfg);

    if(bSupported)
    {
        u8 u8DeviceIdx;
        bool bEnable;

        stIrqIoctlCfg.enIoctlType = E_HAL_DISP_IRQ_IOCTL_TIMEZONE_GET_ID;
        stIrqIoctlCfg.enIrqType = E_HAL_DISP_IRQ_TYPE_NONE;
        stIrqIoctlCfg.pDispCtx = (void *)pDispCtx;
        stIrqIoctlCfg.pParam = (void *)&u8DeviceIdx;
        HalDispIrqIoCtl(&stIrqIoctlCfg);

        u32IrqNum = gstDispIrqCfg[u8DeviceIdx].u32IrqNum;
        free_irq(u32IrqNum, (void *)pDispCtx);
        disable_irq(u32IrqNum);

        bEnable = 0;
        stIrqIoctlCfg.enIoctlType = E_HAL_DISP_IRQ_IOCTL_TIMEZONE_ENABLE;
        stIrqIoctlCfg.enIrqType = E_HAL_DISP_IRQ_TYPE_TIMEZONE;
        stIrqIoctlCfg.pDispCtx = (void *)pDispCtx;
        stIrqIoctlCfg.pParam = (void *)&bEnable;
        HalDispIrqIoCtl(&stIrqIoctlCfg);
    }
    else
    {
        DISP_DBG(DISP_DBG_LEVEL_IRQ_TIMEZONE, "%s %d, Not Support\n", __FUNCTION__, __LINE__);
    }
#endif
    return 1;
}

bool _DrvDispIrqDestroyVgaHpdIsr(DrvDispCtxConfig_t *pDispCtx)
{
#if DISP_VGA_HPD_ISR_SUPPORT
    u32 u32IrqNum;
    bool bSupported;
    HalDispIrqIoctlConfig_t stIrqIoctlCfg;

    stIrqIoctlCfg.enIoctlType = E_HAL_DISP_IRQ_IOCTL_VGA_HPD_SUPPORTED;
    stIrqIoctlCfg.enIrqType = E_HAL_DISP_IRQ_TYPE_NONE;
    stIrqIoctlCfg.pDispCtx = (void *)pDispCtx;
    stIrqIoctlCfg.pParam = (void *)&bSupported;
    HalDispIrqIoCtl(&stIrqIoctlCfg);

    if(bSupported)
    {
        u8 u8DeviceIdx;
        bool bEnable;

        stIrqIoctlCfg.enIoctlType = E_HAL_DISP_IRQ_IOCTL_VGA_HPD_GET_ID;
        stIrqIoctlCfg.enIrqType = E_HAL_DISP_IRQ_TYPE_NONE;
        stIrqIoctlCfg.pDispCtx = (void *)pDispCtx;
        stIrqIoctlCfg.pParam = (void *)&u8DeviceIdx;
        HalDispIrqIoCtl(&stIrqIoctlCfg);

        u32IrqNum = gstDispIrqCfg[u8DeviceIdx].u32IrqNum;
        free_irq(u32IrqNum, (void *)pDispCtx);
        disable_irq(u32IrqNum);

        bEnable = 0;
        stIrqIoctlCfg.enIoctlType = E_HAL_DISP_IRQ_IOCTL_VGA_HPD_ENABLE;
        stIrqIoctlCfg.enIrqType = E_HAL_DISP_IRQ_TYPE_VGA_HPD_ON_OFF;
        stIrqIoctlCfg.pDispCtx = (void *)pDispCtx;
        stIrqIoctlCfg.pParam = (void *)&bEnable;
        HalDispIrqIoCtl(&stIrqIoctlCfg);
    }
    else
    {
        DISP_DBG(DISP_DBG_LEVEL_IRQ_VGA_HPD, "%s %d, Not Support\n", __FUNCTION__, __LINE__);
    }
#endif
    return 1;
}



bool DrvDispIrqCreateInternalIsr(void *pDispCtx)
{
    bool bRet = 1;

    bRet &= _DrvDispCreateTimeZoneIsr((DrvDispCtxConfig_t *)pDispCtx);
    bRet &= _DrvDispIrqCreateVgaHpdIsr((DrvDispCtxConfig_t *)pDispCtx);
    return bRet;
}


bool DrvDispIrqDestroyInternalIsr(void *pDispCtx)
{
    bool bRet = 1;

    bRet &= _DrvDispIrqDestroyTimeZoneIsr((DrvDispCtxConfig_t *)pDispCtx);
    bRet &= _DrvDispIrqDestroyVgaHpdIsr((DrvDispCtxConfig_t *)pDispCtx);
    return bRet;
}

u16 DrvDispIrqGetIrqCount(void)
{
    return HAL_DISP_IRQ_ID_MAX;
}
