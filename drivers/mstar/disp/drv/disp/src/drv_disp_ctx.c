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

#define _DRV_DISP_CTX_C_

#include "drv_disp_os.h"
#include "hal_disp_common.h"
#include "hal_disp_chip.h"
#include "hal_disp_st.h"
#include "hal_disp_if.h"
#include "hal_disp_util.h"
#include "hal_disp.h"
#include "disp_debug.h"
#include "drv_disp_ctx.h"
//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Defines & Macro
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  structure
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Variable
//-------------------------------------------------------------------------------------------------
DrvDispCtxContain_t gstDispCtxContainTbl[HAL_DISP_CTX_MAX_INST];
bool gbDispCtxInit = 0;

DrvDispCtxConfig_t *pgstCurDispDevCtx[HAL_DISP_DEVICE_MAX];
DrvDispCtxConfig_t *pgstCurVidLayerCtx[HAL_DISP_VIDLAYER_MAX];
DrvDispCtxConfig_t *pgstCurInputPortCtx[HAL_DISP_INPUTPORT_MAX];
DrvDispCtxConfig_t *pgstCurDmaCtx[HAL_DISP_DMA_MAX];
//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
bool __DrvDispCtxBindInputPortWidthVidLayer(DrvDispCtxConfig_t *pstVidLayerCtx, u16 u16InputPortCtxId, u16 u16InputPortId)
{
    bool bRet = 1;
    DrvDispCtxVideoLayerContain_t *pstVidLayerContain = NULL;
    DrvDispCtxInputPortContain_t *pstInputPortContain = NULL;

    if(pstVidLayerCtx->enCtxType == E_DISP_CTX_TYPE_VIDLAYER &&
       u16InputPortCtxId != HAL_DISP_INPUTPORT_MAX &&
       u16InputPortId < HAL_DISP_INPUTPORT_NUM)
    {
        pstVidLayerContain =
            pstVidLayerCtx->pstCtxContain->pstVidLayerContain[pstVidLayerCtx->u32Idx];

        pstInputPortContain =
            pstVidLayerCtx->pstCtxContain->pstInputPortContain[u16InputPortCtxId];

        pstVidLayerContain->pstInputPortContain[u16InputPortId] = pstInputPortContain;
        pstInputPortContain->pstVidLayerContain = pstVidLayerContain;

        DISP_DBG(DISP_DBG_LEVEL_CTX, "%s %d, VideCtxId=%ld, VideId=%ld,(%p), InputPortCtxId:%d, InputPortId:%d,(%p)\n",
            __FUNCTION__, __LINE__,
            pstVidLayerCtx->u32Idx, pstVidLayerContain->u32VidLayerId, pstVidLayerContain,
            u16InputPortCtxId, u16InputPortId, pstInputPortContain);
    }
    else
    {
       bRet = 0;
       DISP_ERR("%s %d, Ctx Type is Err %s, \n",
        __FUNCTION__, __LINE__,  PARSING_CTX_TYPE(pstVidLayerCtx->enCtxType));
    }
    return bRet;
}


bool __DrvDispCtxUnBindInputPortWidthVidLayer(DrvDispCtxConfig_t *pstInputPortCtx)
{
    bool bRet = 1;
    DrvDispCtxVideoLayerContain_t *pstVidLayerContain = NULL;
    DrvDispCtxInputPortContain_t *pstInputPortContain = NULL;

    if(pstInputPortCtx->enCtxType == E_DISP_CTX_TYPE_INPUTPORT)
    {
        pstInputPortContain = pstInputPortCtx->pstCtxContain->pstInputPortContain[pstInputPortCtx->u32Idx];
        pstVidLayerContain = pstInputPortContain->pstVidLayerContain;

        pstVidLayerContain->pstInputPortContain[pstInputPortContain->u32PortId] = NULL;
        pstInputPortCtx->pstCtxContain->pstInputPortContain[pstInputPortCtx->u32Idx]->pstVidLayerContain = NULL;

        DISP_DBG(DISP_DBG_LEVEL_CTX, "%s %d, VideLayerId=%ld, InputPortCtxId:%ld, InputPortId:%ld\n",
            __FUNCTION__, __LINE__,
            pstVidLayerContain->u32VidLayerId,
            pstInputPortCtx->u32Idx,
            pstInputPortContain->u32PortId);
    }
    else
    {
        bRet = 0;
        DISP_ERR("%s %d, Ctx Type is Err %s, \n",
         __FUNCTION__, __LINE__,  PARSING_CTX_TYPE(pstInputPortCtx->enCtxType));

    }
    return bRet;
}

bool _DrvDispCtxInitDeviceContain(DrvDispCtxContain_t *pDispContain)
{
    u16 i;
    bool bRet = 1;

    for(i=0; i<HAL_DISP_DEVICE_MAX; i++)
    {
        pDispContain->bDevUsed[i] = 0;
        pDispContain->pstDevContain[i] = DrvDispOsMemAlloc(sizeof(DrvDispCtxDeviceContain_t));
        if(pDispContain->pstDevContain[i] == NULL)
        {
            bRet = 0;
            DISP_ERR("%s %d, Alloc Device Contain Fail\n", __FUNCTION__, __LINE__);
            break;
        }
        memset(pDispContain->pstDevContain[i], 0, sizeof(DrvDispCtxDeviceContain_t));
    }
    return bRet;
}

void _DrvDispCtxDeInitDeviceContain(DrvDispCtxContain_t *pDispContain)
{
    u16 i;

    for(i=0; i<HAL_DISP_DEVICE_MAX; i++)
    {
        if(pDispContain->pstDevContain[i])
        {
            DrvDispOsMemRelease(pDispContain->pstDevContain[i]);
            pDispContain->pstDevContain[i] = NULL;
            pDispContain->bDevUsed[i] = 0;
        }
    }
}

bool _DrvDispCtxInitVidLayerContain(DrvDispCtxContain_t *pDispContain)
{
    u16 i;
    bool bRet = 1;

    for(i=0; i<HAL_DISP_VIDLAYER_MAX; i++)
    {
        pDispContain->bVidLayerUsed[i] = 0;
        pDispContain->pstVidLayerContain[i] = DrvDispOsMemAlloc(sizeof(DrvDispCtxVideoLayerContain_t));
        if(pDispContain->pstVidLayerContain[i] == NULL)
        {
            bRet = 0;
            DISP_ERR("%s %d, Alloc VidLayer Contain Fail\n", __FUNCTION__, __LINE__);
            break;
        }
        memset(pDispContain->pstVidLayerContain[i], 0, sizeof(DrvDispCtxVideoLayerContain_t));
    }
    return bRet;
}
void _DrvDispCtxDeInitVidLayerContain(DrvDispCtxContain_t *pDispContain)
{
    u16 i;

    for(i=0; i<HAL_DISP_VIDLAYER_MAX; i++)
    {
        if(pDispContain->pstVidLayerContain[i])
        {
            DrvDispOsMemRelease(pDispContain->pstVidLayerContain[i]);
            pDispContain->pstVidLayerContain[i] = NULL;
            pDispContain->bVidLayerUsed[i] = 0;
        }
    }
}

bool _DrvDispCtxInitInputPortContain(DrvDispCtxContain_t *pstDispContain)
{
    u16 i;
    bool bRet = 1;

    for(i=0; i<HAL_DISP_INPUTPORT_MAX; i++)
    {
        pstDispContain->bInputPortUsed[i] = 0;
        pstDispContain->pstInputPortContain[i] = DrvDispOsMemAlloc(sizeof(DrvDispCtxInputPortContain_t));
        if(pstDispContain->pstInputPortContain[i] == NULL)
        {
            bRet = 0;
            DISP_ERR("%s %d, Alloc InputPort Contain Fail\n", __FUNCTION__, __LINE__);
            break;
        }
        memset(pstDispContain->pstInputPortContain[i], 0, sizeof(DrvDispCtxInputPortContain_t));
    }
    return bRet;

}

void _DrvDispCtxDeInitInputPortContain(DrvDispCtxContain_t *pstDispCtxContain)
{
    u16 i;

    for(i=0; i<HAL_DISP_INPUTPORT_MAX; i++)
    {
        if(pstDispCtxContain->pstInputPortContain[i])
        {
            DrvDispOsMemRelease(pstDispCtxContain->pstInputPortContain[i]);
            pstDispCtxContain->pstInputPortContain[i] = NULL;
            pstDispCtxContain->bInputPortUsed[i] = 0;
        }
    }
}

bool _DrvDispCtxInitDmaContain(DrvDispCtxContain_t *pDispContain)
{
    u16 i;
    bool bRet = 1;

    for(i=0; i<HAL_DISP_DMA_MAX; i++)
    {
        pDispContain->bDmaUsed[i] = 0;
        pDispContain->pstDmaContain[i] = DrvDispOsMemAlloc(sizeof(DrvDispCtxDmaContain_t));
        if(pDispContain->pstDmaContain[i] == NULL)
        {
            bRet = 0;
            DISP_ERR("%s %d, Alloc Dma Contain Fail\n", __FUNCTION__, __LINE__);
            break;
        }
        memset(pDispContain->pstDmaContain[i], 0, sizeof(DrvDispCtxDmaContain_t));
    }
    return bRet;

}

void _DrvDispCtxDeInitDmaContain(DrvDispCtxContain_t *pDispCtxContain)
{
    u16 i;

    for(i=0; i<HAL_DISP_DMA_MAX; i++)
    {
        if(pDispCtxContain->pstDmaContain[i])
        {
            DrvDispOsMemRelease(pDispCtxContain->pstDmaContain[i]);
            pDispCtxContain->pstDmaContain[i] = NULL;
            pDispCtxContain->bInputPortUsed[i] = 0;
        }
    }
}

bool _DrvDispCtxInitHalHwContain(DrvDispCtxContain_t *pDispContain)
{
    bool bRet = 1;
    if(sizeof(HalDispHwContain_t) != 0)
    {
        pDispContain->pstHalHwContain = DrvDispOsMemAlloc(sizeof(HalDispHwContain_t));
        if(pDispContain->pstHalHwContain == NULL)
        {
            bRet = 0;
            DISP_ERR("%s %d, Alloc Hw Contain Fail\n", __FUNCTION__, __LINE__);
        }
        memset(pDispContain->pstHalHwContain, 0, sizeof(HalDispHwContain_t));
    }
    return bRet;
}

void _DrvDispCtxDeInitHalHwContain(DrvDispCtxContain_t *pDispCtxContain)
{
    if(pDispCtxContain->pstHalHwContain)
    {
        DrvDispOsMemRelease(pDispCtxContain->pstHalHwContain);
        pDispCtxContain->pstHalHwContain = NULL;
    }
}

bool _DrvDispCtxAllocDevContain(DrvDispCtxAllocConfig_t *pstAllocCfg, DrvDispCtxConfig_t **pCtx)
{
    u16 i, j, u16EmptyIdx, u16DevCtxId;
    bool bRet = 1;
    DrvDispCtxConfig_t *pstDispCtx = NULL;

    u16EmptyIdx = HAL_DISP_CTX_MAX_INST;
    u16DevCtxId = HAL_DISP_DEVICE_MAX;

    for(i=0; i<HAL_DISP_CTX_MAX_INST; i++)
    {
        for(j=0; j< HAL_DISP_DEVICE_MAX; j++)
        {
            if(gstDispCtxContainTbl[i].bDevUsed[j] == 0)
            {
                u16EmptyIdx = i;
                u16DevCtxId = j;
                break;
            }
        }
    }

    if(u16EmptyIdx != HAL_DISP_CTX_MAX_INST && u16DevCtxId != HAL_DISP_DEVICE_MAX)
    {
        pstDispCtx = DrvDispOsMemAlloc(sizeof(DrvDispCtxConfig_t));

        if(pstDispCtx)
        {
            pstDispCtx->enCtxType = E_DISP_CTX_TYPE_DEVICE;
            pstDispCtx->u32Idx = u16DevCtxId;
            pstDispCtx->pstCtxContain = &gstDispCtxContainTbl[u16EmptyIdx];

            memset(pstDispCtx->pstCtxContain->pstDevContain[u16DevCtxId], 0, sizeof(DrvDispCtxDeviceContain_t));

            pstDispCtx->pstCtxContain->bDevUsed[u16DevCtxId] = 1;
            pstDispCtx->pstCtxContain->pstDevContain[u16DevCtxId]->u32DevId = pstAllocCfg->u32Id;
            pstDispCtx->pstCtxContain->stMemAllcCfg.alloc = pstAllocCfg->stMemAllcCfg.alloc;
            pstDispCtx->pstCtxContain->stMemAllcCfg.free = pstAllocCfg->stMemAllcCfg.free;
            *pCtx = pstDispCtx;
        }
        else
        {
            bRet = 0;
            *pCtx = NULL;
            DISP_ERR("%s %d, Alloc Ctx Fail\n", __FUNCTION__, __LINE__);
        }
    }
    else
    {
        bRet = 0;
        *pCtx = NULL;
        DISP_ERR("%s %d, Alloc Dev Contain Fail\n", __FUNCTION__, __LINE__);
    }

    return bRet;
}

bool _DrvDispCtxFreeDevContain(DrvDispCtxConfig_t *pCtx)
{
    DrvDispCtxDeviceContain_t *pstDevContain = NULL;
    bool bRet = 1;

    if(pCtx->enCtxType == E_DISP_CTX_TYPE_DEVICE)
    {
        pstDevContain = pCtx->pstCtxContain->pstDevContain[pCtx->u32Idx];
        memset(pstDevContain, 0, sizeof(DrvDispCtxDeviceContain_t));
        pCtx->pstCtxContain->bDevUsed[pCtx->u32Idx] = 0;
        DrvDispOsMemRelease(pCtx);
    }
    else
    {
        bRet = 0;
        DISP_ERR("%s %d, Ctx Type Err, %s\n",
            __FUNCTION__, __LINE__, PARSING_CTX_TYPE(pCtx->enCtxType));
    }
    return bRet;
}

bool _DrvDispCtxAllocVidLayerContain(DrvDispCtxAllocConfig_t *pstAllocCfg, DrvDispCtxConfig_t **pCtx)
{
    u16 i, j, u16EmptyIdx, u16VidLayerCtxId;
    DrvDispCtxConfig_t *pstDispCtx = NULL;
    bool bRet = 1;

    u16EmptyIdx = HAL_DISP_CTX_MAX_INST;
    u16VidLayerCtxId = HAL_DISP_VIDLAYER_MAX;

    for(i=0; i<HAL_DISP_CTX_MAX_INST; i++)
    {
        for(j=0; j< HAL_DISP_VIDLAYER_MAX; j++)
        {
            if(gstDispCtxContainTbl[i].bVidLayerUsed[j] == 0)
            {
                u16EmptyIdx = i;
                u16VidLayerCtxId = j;
                break;
            }
        }
    }

    if(u16EmptyIdx != HAL_DISP_CTX_MAX_INST && u16VidLayerCtxId != HAL_DISP_VIDLAYER_MAX)
    {
        pstDispCtx = DrvDispOsMemAlloc(sizeof(DrvDispCtxConfig_t));

        if(pstDispCtx)
        {
            pstDispCtx->enCtxType = E_DISP_CTX_TYPE_VIDLAYER;
            pstDispCtx->u32Idx = u16VidLayerCtxId;
            pstDispCtx->pstCtxContain = &gstDispCtxContainTbl[u16EmptyIdx];

            memset(pstDispCtx->pstCtxContain->pstVidLayerContain[u16VidLayerCtxId], 0, sizeof(DrvDispCtxVideoLayerContain_t));
            pstDispCtx->pstCtxContain->bVidLayerUsed[u16VidLayerCtxId] = 1;
            pstDispCtx->pstCtxContain->pstVidLayerContain[u16VidLayerCtxId]->u32VidLayerId = pstAllocCfg->u32Id;
            *pCtx = pstDispCtx;
        }
        else
        {
            *pCtx = NULL;
            bRet = 0;
            DISP_ERR("%s %d, Alloc Ctx Fail\n", __FUNCTION__, __LINE__);
        }
    }
    else
    {
        *pCtx = NULL;
        bRet = 0;
        DISP_ERR("%s %d, Alloc VidLayer Contain Fail\n", __FUNCTION__, __LINE__);
    }
    return bRet;
}

bool _DrvDispCtxFreeVidLayerContain(DrvDispCtxConfig_t *pCtx)
{
    DrvDispCtxVideoLayerContain_t *pstVidLayerContain = NULL;
    bool bRet = 1;

    if(pCtx->enCtxType == E_DISP_CTX_TYPE_VIDLAYER)
    {
        pstVidLayerContain = pCtx->pstCtxContain->pstVidLayerContain[pCtx->u32Idx];

        memset(pstVidLayerContain, 0, sizeof(DrvDispCtxVideoLayerContain_t));
        pCtx->pstCtxContain->bVidLayerUsed[pCtx->u32Idx] = 0;
        DrvDispOsMemRelease(pCtx);
    }
    else
    {
        bRet = 0;
        DISP_ERR("%s %d, Ctx Type Err, %s\n",
            __FUNCTION__, __LINE__, PARSING_CTX_TYPE(pCtx->enCtxType));
    }
    return bRet;
}

bool _DrvDispCtxAllocInputPortContain(DrvDispCtxAllocConfig_t *pstAllocCfg, DrvDispCtxConfig_t **pCtx)
{
    u16 i, j, u16EmptyIdx, u16InputPortCtxId;
    DrvDispCtxConfig_t *pstDispCtx = NULL;
    bool bRet = 1;

    u16EmptyIdx = HAL_DISP_CTX_MAX_INST;
    u16InputPortCtxId = HAL_DISP_INPUTPORT_MAX;

    for(i=0; i<HAL_DISP_CTX_MAX_INST; i++)
    {
        for(j=0; j< HAL_DISP_INPUTPORT_MAX; j++)
        {
            if(gstDispCtxContainTbl[i].bInputPortUsed[j] == 0)
            {
                u16EmptyIdx = i;
                u16InputPortCtxId = j;
                break;
            }
        }
    }

    if(u16EmptyIdx != HAL_DISP_CTX_MAX_INST && u16InputPortCtxId != HAL_DISP_INPUTPORT_MAX)
    {
        pstDispCtx = DrvDispOsMemAlloc(sizeof(DrvDispCtxConfig_t));

        if(pstDispCtx)
        {
            pstDispCtx->enCtxType = E_DISP_CTX_TYPE_INPUTPORT;
            pstDispCtx->u32Idx = u16InputPortCtxId;
            pstDispCtx->pstCtxContain = &gstDispCtxContainTbl[u16EmptyIdx];

            memset(pstDispCtx->pstCtxContain->pstInputPortContain[u16InputPortCtxId], 0, sizeof(DrvDispCtxInputPortContain_t));

            if(__DrvDispCtxBindInputPortWidthVidLayer(pstAllocCfg->pstBindCtx, u16InputPortCtxId, pstAllocCfg->u32Id))
            {
                pstDispCtx->pstCtxContain->bInputPortUsed[u16InputPortCtxId] = 1;
                pstDispCtx->pstCtxContain->pstInputPortContain[u16InputPortCtxId]->u32PortId = pstAllocCfg->u32Id;
                *pCtx = pstDispCtx;
            }
            else
            {
                DrvDispOsMemRelease(pstDispCtx);
                *pCtx = NULL;
                bRet = 0;
                DISP_ERR("%s %d, Bind Fail\n", __FUNCTION__, __LINE__);
            }
        }
        else
        {
            *pCtx = NULL;
            bRet = 0;
            DISP_ERR("%s %d, Alloc Ctx Fail\n", __FUNCTION__, __LINE__);
        }
    }
    else
    {
        *pCtx = NULL;
        bRet = 0;
        DISP_ERR("%s %d, Alloc InputPort Contain Fail\n", __FUNCTION__, __LINE__);
    }
    return bRet;
}


bool _DrvDispCtxFreeInputPortContain(DrvDispCtxConfig_t *pCtx)
{
    DrvDispCtxInputPortContain_t *pstInputPortContain = NULL;
    bool bRet = 1;

    if(pCtx->enCtxType == E_DISP_CTX_TYPE_INPUTPORT)
    {
        if(__DrvDispCtxUnBindInputPortWidthVidLayer(pCtx))
        {
            pstInputPortContain = pCtx->pstCtxContain->pstInputPortContain[pCtx->u32Idx];

            memset(pstInputPortContain, 0, sizeof(DrvDispCtxInputPortContain_t));
            pCtx->pstCtxContain->bInputPortUsed[pCtx->u32Idx] = 0;
            DrvDispOsMemRelease(pCtx);
        }
        else
        {
            bRet = 0;
            DISP_ERR("%s %d, UnBind Fail\n", __FUNCTION__, __LINE__ );
        }
    }
    else
    {
        bRet = 0;
        DISP_ERR("%s %d, Ctx Type Err, %s\n",
            __FUNCTION__, __LINE__, PARSING_CTX_TYPE(pCtx->enCtxType));
    }
    return bRet;
}


bool _DrvDispCtxAllocDmaContain(DrvDispCtxAllocConfig_t *pstAllocCfg, DrvDispCtxConfig_t **pCtx)
{
    u16 i, j, u16EmptyIdx, u16DmaCtxId;
    DrvDispCtxConfig_t *pstDispCtx = NULL;
    bool bRet = 1;

    u16EmptyIdx = HAL_DISP_CTX_MAX_INST;
    u16DmaCtxId = HAL_DISP_DMA_MAX;

    for(i=0; i<HAL_DISP_CTX_MAX_INST; i++)
    {
        for(j=0; j< HAL_DISP_DMA_MAX; j++)
        {
            if(gstDispCtxContainTbl[i].bDmaUsed[j] == 0)
            {
                u16EmptyIdx = i;
                u16DmaCtxId = j;
                break;
            }
        }
    }

    if(u16EmptyIdx != HAL_DISP_CTX_MAX_INST && u16DmaCtxId != HAL_DISP_DMA_MAX)
    {
        pstDispCtx = DrvDispOsMemAlloc(sizeof(DrvDispCtxConfig_t));

        if(pstDispCtx)
        {
            pstDispCtx->enCtxType = E_DISP_CTX_TYPE_DMA;
            pstDispCtx->u32Idx = u16DmaCtxId;
            pstDispCtx->pstCtxContain = &gstDispCtxContainTbl[u16EmptyIdx];

            memset(pstDispCtx->pstCtxContain->pstDmaContain[u16DmaCtxId], 0, sizeof(DrvDispCtxDmaContain_t));
            pstDispCtx->pstCtxContain->bDmaUsed[u16DmaCtxId] = 1;
            pstDispCtx->pstCtxContain->pstDmaContain[u16DmaCtxId]->u32DmaId = pstAllocCfg->u32Id;
            *pCtx = pstDispCtx;
        }
        else
        {
            *pCtx = NULL;
            bRet = 0;
            DISP_ERR("%s %d, Alloc Dma Ctx Fail\n", __FUNCTION__, __LINE__);
        }
    }
    else
    {
        *pCtx = NULL;
        bRet = 0;
        DISP_ERR("%s %d, Alloc Dma Contain Fail\n", __FUNCTION__, __LINE__);
    }
    return bRet;
}

bool _DrvDispCtxFreeDmaContain(DrvDispCtxConfig_t *pCtx)
{
    DrvDispCtxDmaContain_t *pstDmaContain = NULL;
    bool bRet = 1;

    if(pCtx->enCtxType == E_DISP_CTX_TYPE_DMA)
    {
        pstDmaContain = pCtx->pstCtxContain->pstDmaContain[pCtx->u32Idx];
        memset(pstDmaContain, 0, sizeof(DrvDispCtxDmaContain_t));
        pCtx->pstCtxContain->bDmaUsed[pCtx->u32Idx] = 0;
        DrvDispOsMemRelease(pCtx);
    }
    else
    {
        bRet = 0;
        DISP_ERR("%s %d, Ctx Type Err, %s\n",
            __FUNCTION__, __LINE__, PARSING_CTX_TYPE(pCtx->enCtxType));
    }
    return bRet;
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
bool DrvDispCtxInit(void)
{
    u16 i;
    bool bRet = 1;

    if(gbDispCtxInit == 1)
    {
        return 1;
    }

    memset(&gstDispCtxContainTbl, 0, sizeof(DrvDispCtxContain_t) * HAL_DISP_CTX_MAX_INST);
    HalDispIfInit();

    for(i=0; i<HAL_DISP_CTX_MAX_INST; i++)
    {
        // Device
        if(_DrvDispCtxInitDeviceContain(&gstDispCtxContainTbl[i]) == FALSE)
        {
            _DrvDispCtxDeInitDeviceContain(&gstDispCtxContainTbl[i]);
            bRet = 0;
            break;
        }

        // VideoLayer
        if(_DrvDispCtxInitVidLayerContain(&gstDispCtxContainTbl[i]) == FALSE)
        {
            _DrvDispCtxDeInitVidLayerContain(&gstDispCtxContainTbl[i]);
            _DrvDispCtxDeInitDeviceContain(&gstDispCtxContainTbl[i]);
            bRet = 0;
            break;
        }

        //InputPort
        if(_DrvDispCtxInitInputPortContain(&gstDispCtxContainTbl[i]) == FALSE)
        {
            _DrvDispCtxDeInitInputPortContain(&gstDispCtxContainTbl[i]);
            _DrvDispCtxDeInitVidLayerContain(&gstDispCtxContainTbl[i]);
            _DrvDispCtxDeInitDeviceContain(&gstDispCtxContainTbl[i]);
            bRet = 0;
            break;
        }

        // Dma
        if(_DrvDispCtxInitDmaContain(&gstDispCtxContainTbl[i]) == FALSE)
        {
            _DrvDispCtxDeInitDmaContain(&gstDispCtxContainTbl[i]);
            _DrvDispCtxDeInitInputPortContain(&gstDispCtxContainTbl[i]);
            _DrvDispCtxDeInitVidLayerContain(&gstDispCtxContainTbl[i]);
            _DrvDispCtxDeInitDeviceContain(&gstDispCtxContainTbl[i]);
            bRet = 0;
            break;
        }

        //HalHw
        if(_DrvDispCtxInitHalHwContain(&gstDispCtxContainTbl[i]) == FALSE)
        {
            _DrvDispCtxDeInitHalHwContain(&gstDispCtxContainTbl[i]);
            _DrvDispCtxDeInitDmaContain(&gstDispCtxContainTbl[i]);
            _DrvDispCtxDeInitInputPortContain(&gstDispCtxContainTbl[i]);
            _DrvDispCtxDeInitVidLayerContain(&gstDispCtxContainTbl[i]);
            _DrvDispCtxDeInitDeviceContain(&gstDispCtxContainTbl[i]);
            bRet = 0;
            break;
        }
        // MemAlloc
        gstDispCtxContainTbl[i].stMemAllcCfg.alloc = NULL;
        gstDispCtxContainTbl[i].stMemAllcCfg.free = NULL;
    }

    gbDispCtxInit = 1;
    return bRet;
}


bool DrvDispCtxDeInit(void)
{
    u16 i;
    bool bRet = 1;

    if(gbDispCtxInit == 0)
    {
        bRet = 0;
        DISP_ERR("%s %d, Ctx not Init\n", __FUNCTION__, __LINE__);
    }
    else
    {
        for(i=0; i<HAL_DISP_CTX_MAX_INST; i++)
        {
            _DrvDispCtxDeInitHalHwContain(&gstDispCtxContainTbl[i]);
            _DrvDispCtxDeInitDmaContain(&gstDispCtxContainTbl[i]);
            _DrvDispCtxDeInitInputPortContain(&gstDispCtxContainTbl[i]);
            _DrvDispCtxDeInitVidLayerContain(&gstDispCtxContainTbl[i]);
            _DrvDispCtxDeInitDeviceContain(&gstDispCtxContainTbl[i]);
            gstDispCtxContainTbl[i].stMemAllcCfg.alloc = NULL;
            gstDispCtxContainTbl[i].stMemAllcCfg.free = NULL;
        }

        for(i=0; i< HAL_DISP_DEVICE_MAX; i++)
        {
            pgstCurDispDevCtx[i] = NULL;
        }

        for(i=0; i< HAL_DISP_VIDLAYER_MAX; i++)
        {
            pgstCurVidLayerCtx[i] = NULL;
        }

        for(i=0; i< HAL_DISP_INPUTPORT_MAX; i++)
        {
            pgstCurInputPortCtx[i] = NULL;
        }

        for(i=0; i<HAL_DISP_DMA_MAX; i++)
        {
            pgstCurDmaCtx[i] = NULL;
        }

        HalDispIfDeInit();
        gbDispCtxInit = 0;
    }

    return bRet;

}

bool DrvDispCtxAllocate(DrvDispCtxAllocConfig_t *pstAllocCfg, DrvDispCtxConfig_t **pCtx)
{
    bool bRet = 1;

    if(pstAllocCfg->enType == E_DISP_CTX_TYPE_DEVICE)
    {
        bRet = _DrvDispCtxAllocDevContain(pstAllocCfg, pCtx);
    }
    else if(pstAllocCfg->enType == E_DISP_CTX_TYPE_VIDLAYER)
    {
        bRet = _DrvDispCtxAllocVidLayerContain(pstAllocCfg, pCtx);
    }
    else if(pstAllocCfg->enType == E_DISP_CTX_TYPE_INPUTPORT)
    {
        bRet = _DrvDispCtxAllocInputPortContain(pstAllocCfg, pCtx);
    }
    else if(pstAllocCfg->enType == E_DISP_CTX_TYPE_DMA)
    {
        bRet = _DrvDispCtxAllocDmaContain(pstAllocCfg, pCtx);
    }
    else
    {
        bRet = 0;
        DISP_ERR("%s %d, Alloc Type is Err %s\n",
            __FUNCTION__, __LINE__,
            PARSING_CTX_TYPE(pstAllocCfg->enType));
    }

    return bRet;
}

bool DrvDispCtxFree(DrvDispCtxConfig_t *pCtx)
{
    bool bRet = 1;

    if(pCtx->enCtxType == E_DISP_CTX_TYPE_DEVICE)
    {
        bRet = _DrvDispCtxFreeDevContain(pCtx);
    }
    else if(pCtx->enCtxType == E_DISP_CTX_TYPE_VIDLAYER)
    {
        bRet = _DrvDispCtxFreeVidLayerContain(pCtx);
    }
    else if(pCtx->enCtxType == E_DISP_CTX_TYPE_INPUTPORT)
    {
        bRet = _DrvDispCtxFreeInputPortContain(pCtx);
    }
    else if(pCtx->enCtxType == E_DISP_CTX_TYPE_DMA)
    {
        bRet = _DrvDispCtxFreeDmaContain(pCtx);
    }
    else
    {
        bRet = 0;
        DISP_ERR("%s %d, Alloc Type is Err %s\n",
            __FUNCTION__, __LINE__,
            PARSING_CTX_TYPE(pCtx->enCtxType));
    }
    return bRet;
}

bool DrvDispCtxIsAllFree(void)
{
    u16 i, j;
    bool bContainUse = 0;

    for(i=0; i< HAL_DISP_CTX_MAX_INST; i++)
    {

        for(j=0; j< HAL_DISP_DEVICE_MAX; j++)
        {
            bContainUse |= gstDispCtxContainTbl[i].bDevUsed[j];
        }

        for(j=0; j< HAL_DISP_VIDLAYER_MAX; j++)
        {
            bContainUse |= gstDispCtxContainTbl[i].bVidLayerUsed[j];
        }

        for(j=0; j< HAL_DISP_INPUTPORT_MAX; j++)
        {
            bContainUse |= gstDispCtxContainTbl[i].bInputPortUsed[j];
        }

        for(j=0; j< HAL_DISP_DMA_MAX; j++)
        {
            bContainUse |= gstDispCtxContainTbl[i].bDmaUsed[j];
        }
    }

    return bContainUse ? 0 : 1;
}

bool DrvDispCtxSetCurCtx(DrvDispCtxConfig_t *pCtx, u32 u32Idx)
{
    bool bRet = TRUE;;
    if(pCtx->enCtxType == E_DISP_CTX_TYPE_DEVICE && u32Idx < HAL_DISP_DEVICE_MAX)
    {
        pgstCurDispDevCtx[u32Idx] = pCtx;
    }
    else if(pCtx->enCtxType == E_DISP_CTX_TYPE_VIDLAYER && u32Idx < HAL_DISP_VIDLAYER_MAX)
    {
        pgstCurVidLayerCtx[u32Idx] = pCtx;
    }
    else if(pCtx->enCtxType == E_DISP_CTX_TYPE_INPUTPORT&& u32Idx < HAL_DISP_INPUTPORT_MAX)
    {
        pgstCurInputPortCtx[u32Idx] = pCtx;
    }
    else if(pCtx->enCtxType == E_DISP_CTX_TYPE_DMA && u32Idx < HAL_DISP_DMA_MAX)
    {
        pgstCurDmaCtx[u32Idx] = pCtx;
    }
    else
    {
        bRet = 0;
        DISP_ERR("%s %d, SetCurCtx Type is Err %s\n",
            __FUNCTION__, __LINE__,
            PARSING_CTX_TYPE(pCtx->enCtxType));
    }
    return bRet;
}

bool DrvDispCtxGetCurCtx(DrvDispCtxType_e enCtxType, u32 u32Idx, DrvDispCtxConfig_t **pCtx)
{
    bool bRet = TRUE;

    if(enCtxType == E_DISP_CTX_TYPE_DEVICE && u32Idx < HAL_DISP_DEVICE_MAX)
    {
        *pCtx = pgstCurDispDevCtx[u32Idx];

    }
    else if(enCtxType == E_DISP_CTX_TYPE_VIDLAYER && u32Idx < HAL_DISP_VIDLAYER_MAX)
    {
        *pCtx = pgstCurVidLayerCtx[u32Idx];
    }
    else if(enCtxType == E_DISP_CTX_TYPE_INPUTPORT && u32Idx < HAL_DISP_INPUTPORT_MAX)
    {
        *pCtx = pgstCurInputPortCtx[u32Idx];
    }
    else if(enCtxType == E_DISP_CTX_TYPE_DMA && u32Idx < HAL_DISP_DMA_MAX)
    {
        *pCtx = pgstCurDmaCtx[u32Idx];
    }
    else
    {
        bRet = FALSE;
        *pCtx = NULL;
        DISP_ERR("%s %d, GetCurCtx Fail, Type=%s, Idx=%ld\n",
            __FUNCTION__, __LINE__,
            PARSING_CTX_TYPE(enCtxType), u32Idx);
    }
    return bRet;
}

