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

#define __MHAL_DISP_C__

#include "drv_disp_os.h"
#include "mhal_common.h"
#include "mhal_cmdq.h"
#include "mhal_disp_datatype.h"
#include "mhal_disp.h"

#include "disp_debug.h"
#include "drv_disp_if.h"
#include "drv_disp_irq.h"

MS_BOOL MHAL_DISP_InitPanelConfig(MHAL_DISP_PanelConfig_t* pstPanelConfig, MS_U8 u8Size)
{
    MS_BOOL bRet = TRUE;
    if(pstPanelConfig == NULL || u8Size == 0)
    {
        DISP_ERR("%s %d, pstPanelConfig or u8Size is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        if(DrvDispIfInitPanelConfig(pstPanelConfig, u8Size) == FALSE)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, Init PanelConfig Fail\n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}

MS_BOOL MHAL_DISP_InitMmapConfig(MHAL_DISP_MmapType_e eMmType, MHAL_DISP_MmapInfo_t* pstMmapInfo)
{
    MS_BOOL bRet = TRUE;
    return bRet;
}


MS_BOOL MHAL_DISP_DeviceCreateInstance(const MHAL_DISP_AllocPhyMem_t *pstAlloc, const MS_U32 u32DeviceId, void **pDevCtx)
{
    MS_BOOL bRet = TRUE;
    if(pstAlloc == NULL)
    {
        DISP_ERR("%s %d, pstAlloc is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        if(DrvDispIfDeviceCreateInstance((MHAL_DISP_AllocPhyMem_t *)pstAlloc, (u32)u32DeviceId, pDevCtx) == FALSE)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, CreateInstance Fail\n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}


MS_BOOL MHAL_DISP_DeviceDestroyInstance(void *pDevCtx)
{
    MS_BOOL bRet = TRUE;
    if(pDevCtx == NULL)
    {
        DISP_ERR("%s %d, pDevCtx is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        if(DrvDispIfDeviceDestroyInstance(pDevCtx) == FALSE)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, DestroyInstance Fail\n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}

MS_BOOL MHAL_DISP_DeviceEnable(void *pDevCtx, const MS_BOOL bEnable)
{
    MS_BOOL bRet = TRUE;

    if(pDevCtx == NULL)
    {
        DISP_ERR("%s %d, pDevCtx is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        if(DrvDispIfDeviceEnable(pDevCtx, bEnable) == FALSE)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, DeviceEnable Fail\n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}

MS_BOOL MHAL_DISP_DeviceSetBackGroundColor(void *pDevCtx, const MS_U32 u32BgColor)
{
    MS_BOOL bRet = TRUE;

    if(pDevCtx == NULL)
    {
        DISP_ERR("%s %d, pDevCtx is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        if(DrvDispIfDeviceSetBackGroundColor(pDevCtx, (u32)u32BgColor) == FALSE)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, SetBackGroundColor Fail\n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}


MS_BOOL MHAL_DISP_DeviceAddOutInterface(void *pDevCtx, const MS_U32 u32Interface)
{
    MS_BOOL bRet = TRUE;
    if(pDevCtx == NULL)
    {
        DISP_ERR("%s %d, pDevCtx is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        if(DrvDispIfDeviceAddOutInterface(pDevCtx, (u32)u32Interface) == FALSE)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, AddOutInterface Fail\n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}



MS_BOOL MHAL_DISP_DeviceSetOutputTiming(void *pDevCtx, const MS_U32 u32Interface, const MHAL_DISP_DeviceTimingInfo_t *pstTimingInfo)
{
    MS_BOOL bRet = TRUE;
    if(pDevCtx == NULL || pstTimingInfo == NULL)
    {
        DISP_ERR("%s %d, pDevCtx is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        if(DrvDispIfDeviceSetOutputTiming(pDevCtx, (u32)u32Interface, (MHAL_DISP_DeviceTimingInfo_t *)pstTimingInfo) == FALSE)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, SetOutputTiming Fail\n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}


MS_BOOL MHAL_DISP_DeviceSetColorTempeture(void *pDevCtx, const MHAL_DISP_ColorTempeture_t *pstColorTempInfo)
{
    MS_BOOL bRet = TRUE;
    if(pDevCtx == NULL || pstColorTempInfo == NULL)
    {
        DISP_ERR("%s %d, pDevCtx  or pstColorTempInfo is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        if(DrvDispIfDeviceSetColortemp (pDevCtx, (MHAL_DISP_ColorTempeture_t *)pstColorTempInfo) == FALSE)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, Set ColorTemp Fail\n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}

MS_BOOL MHAL_DISP_DeviceSetCvbsParam(void *pDevCtx, const MHAL_DISP_CvbsParam_t *pstCvbsInfo)
{
    MS_BOOL bRet = TRUE;
    if(pDevCtx == NULL || pstCvbsInfo == NULL)
    {
        DISP_ERR("%s %d, pDevCtx  or pstCvbsInfo is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        if(DrvDispIfDeviceSetCvbsParam(pDevCtx, (MHAL_DISP_CvbsParam_t *)pstCvbsInfo) == FALSE)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, Set CvbsParam Fail\n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}

MS_BOOL MHAL_DISP_DeviceSetHdmiParam(void *pDevCtx, const MHAL_DISP_HdmiParam_t *pstHdmiInfo)
{
    MS_BOOL bRet = TRUE;
    if(pDevCtx == NULL || pstHdmiInfo == NULL)
    {
        DISP_ERR("%s %d, pDevCtx  or pstHdmiInfo is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        if(DrvDispIfDeviceSetHdmiParam(pDevCtx, (MHAL_DISP_HdmiParam_t *)pstHdmiInfo) == FALSE)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, Set HdmiParam Fail\n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}

MS_BOOL MHAL_DISP_DeviceSetLcdParam(void *pDevCtx, const MHAL_DISP_LcdParam_t *pstLcdInfo)
{
    MS_BOOL bRet = TRUE;
    if(pDevCtx == NULL || pstLcdInfo == NULL)
    {
        DISP_ERR("%s %d, pDevCtx  or pstLcdInfo is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        if(DrvDispIfDeviceSetLcdParam(pDevCtx, (MHAL_DISP_LcdParam_t *)pstLcdInfo) == FALSE)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, Set LcdParam Fail\n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}

MS_BOOL MHAL_DISP_DeviceSetGammaParam(void *pDevCtx, const MHAL_DISP_GammaParam_t *pstGammaInfo)
{
    MS_BOOL bRet = TRUE;
    if(pDevCtx == NULL || pstGammaInfo == NULL)
    {
        DISP_ERR("%s %d, pDevCtx  or pstGammaInfo is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        if(DrvDispIfDeviceSetGammaParam(pDevCtx, (MHAL_DISP_GammaParam_t *)pstGammaInfo) == FALSE)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, Set GammaParam Fail\n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}

MS_BOOL MHAL_DISP_DeviceSetVgaParam(void *pDevCtx, const MHAL_DISP_VgaParam_t *pstVgaInfo)
{
    MS_BOOL bRet = TRUE;
    if(pDevCtx == NULL || pstVgaInfo == NULL)
    {
        DISP_ERR("%s %d, pDevCtx  or pstVgaInfo is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        if(DrvDispIfDeviceSetVgaParam(pDevCtx, (MHAL_DISP_VgaParam_t *)pstVgaInfo) == FALSE)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, Set VgaParam Fail\n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}


 MS_BOOL MHAL_DISP_DeviceAttach(const void *pSrcDevCtx, const void *pDstDevCtx)
{
     MS_BOOL bRet = TRUE;
     if(pSrcDevCtx == NULL || pDstDevCtx == NULL)
     {
         DISP_ERR("%s %d, pSrcDevCtx  or pDstDevCtx is Null\n", __FUNCTION__, __LINE__);
         bRet = FALSE;
     }
     else
     {
         if(DrvDispIfDeviceAttach((void *)pSrcDevCtx, (void *)pDstDevCtx) == FALSE)
         {
             bRet = FALSE;
             DISP_ERR("%s %d, Device Attach Fail \n", __FUNCTION__, __LINE__);
         }
     }
     return bRet;
}

MS_BOOL MHAL_DISP_DeviceDetach(const void *pSrcDevCtx, const void *pDstDevCtx)
{
    MS_BOOL bRet = TRUE;
    if(pSrcDevCtx == NULL || pDstDevCtx == NULL)
    {
        DISP_ERR("%s %d, pSrcDevCtx  or pDstDevCtx is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        if(DrvDispIfDeviceDetach((void *)pSrcDevCtx, (void *)pDstDevCtx) == FALSE)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, Get Device Detach Fail \n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}

MS_BOOL MHAL_DISP_DeviceSetTimeZoneConfig(void *pDevCtx, MHAL_DISP_TimeZoneConfig_t *pstTimeZoneCfg)
{
    MS_BOOL bRet = TRUE;
    if(pDevCtx == NULL)
    {
        DISP_ERR("%s %d, pDevCtx is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        if(DrvDispIfDeviceSetTimeZoneConfig((void *)pDevCtx, (void *)pstTimeZoneCfg) == FALSE)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, Set Device TimeZone Fail \n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}

MS_BOOL MHAL_DISP_DeviceGetTimeZone(void *pDevCtx, MHAL_DISP_TimeZone_t *pstTimeZone)
{
    MS_BOOL bRet = TRUE;
    if(pDevCtx == NULL)
    {
        DISP_ERR("%s %d, pDevCtx is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        if(DrvDispIfDeviceGetTimeZone((void *)pDevCtx, (void *)pstTimeZone) == FALSE)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, Device TimeZone Fail \n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}

MS_BOOL MHAL_DISP_DeviceGetDisplayInfo(void *pDevCtx, MHAL_DISP_DisplayInfo_t *pstDisplayInfo)
{
    MS_BOOL bRet = TRUE;
    if(pDevCtx == NULL)
    {
        DISP_ERR("%s %d, pDevCtx is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        if(DrvDispIfDeviceGetDisplayInfo((void *)pDevCtx, (void *)pstDisplayInfo) == FALSE)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, Device DisplayInfo Fail \n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}

MS_BOOL MHAL_DISP_DeviceGetInstance(MS_U32 u32DeviceId, void **pDevCtx)
{
    MS_BOOL bRet = TRUE;
    if(DrvDispIfDeviceGetInstance(u32DeviceId, pDevCtx) == FALSE)
    {
        bRet = FALSE;
        DISP_ERR("%s %d, Device DisplayInfo Fail \n", __FUNCTION__, __LINE__);
    }
    return bRet;
}

MS_BOOL MHAL_DISP_DeviceSetPqConfig(void *pDevCtx,  MHAL_DISP_PqConfig_t *pstPqCfg)
{
    MS_BOOL bRet = TRUE;

    if(DrvDispIfDeviceSetPqConfig(pDevCtx, pstPqCfg) == FALSE)
    {
        bRet = FALSE;
        DISP_ERR("%s %d, Device DisplayInfo Fail \n", __FUNCTION__, __LINE__);
    }
    return bRet;
}

MS_BOOL MHAL_DISP_DeviceGetHwCount(MS_U32 *pu32Count)
{
    MS_BOOL bRet = TRUE;

    if(DrvDispIfDeviceGetHwCount((u32 *)pu32Count) == FALSE)
    {
        DISP_ERR("%s %d, Get Hw Count Fail\n", __FUNCTION__, __LINE__);
        bRet = FALSE;

    }
    return bRet;
}

MS_BOOL MHAL_DISP_DeviceSetConfig(MS_U32 u32DevId, MHAL_DISP_DeviceConfig_t *pstDevCfg)
{
    MS_BOOL bRet = TRUE;

    if(DrvDispIfSetDeviceConfig(u32DevId, pstDevCfg) == FALSE)
    {
        DISP_ERR("%s %d, Set Config\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    return bRet;
}

MS_BOOL MHAL_DISP_DeviceGetConfig(MS_U32 u32DevId, MHAL_DISP_DeviceConfig_t *pstDevCfg)
{
    MS_BOOL bRet = TRUE;

    if(DrvDispIfGetDeviceConfig(u32DevId, pstDevCfg) == FALSE)
    {
        DISP_ERR("%s %d, Get Config\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    return bRet;
}


MS_BOOL MHAL_DISP_VideoLayerCreateInstance(const MHAL_DISP_AllocPhyMem_t *pstAlloc, const MS_U32 u32LayerId, void **pVidLayerCtx)
{
    MS_BOOL bRet = TRUE;
    if(pstAlloc == NULL)
    {
        DISP_ERR("%s %d, pstAlloc is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        if(DrvDispIfVideoLayerCreateInstance((MHAL_DISP_AllocPhyMem_t *)pstAlloc, (u32)u32LayerId, pVidLayerCtx) == FALSE)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, CreateInstance Fail\n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}

MS_BOOL MHAL_DISP_VideoLayerDestoryInstance(void *pVidLayerCtx)
{
    MS_BOOL bRet = TRUE;

    if(pVidLayerCtx == NULL)
    {
        DISP_ERR("%s %d, pVidLayerCtx is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        if(DrvDispIfVideoLayerDestoryInstance(pVidLayerCtx) == FALSE)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, DestoryInstance Fail\n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}


MS_BOOL MHAL_DISP_VideoLayerEnable(void *pVidLayerCtx, const MS_BOOL bEnable)
{
    MS_BOOL bRet = TRUE;

    if(pVidLayerCtx == NULL)
    {
        DISP_ERR("%s %d, pVidLayerCtx is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        if(DrvDispIfVideoLayerEnable(pVidLayerCtx, bEnable) == FALSE)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, VideoLayerEnable Fail\n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}

MS_BOOL MHAL_DISP_VideoLayerBind(void *pVidLayerCtx, void *pDevCtx)
{
    MS_BOOL bRet = TRUE;

    if(pVidLayerCtx == NULL || pDevCtx == NULL)
    {
        DISP_ERR("%s %d, pVidLayerCtx or PDevCtx is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        if(DrvDispIfVideoLayerBind(pVidLayerCtx, pDevCtx) == FALSE)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, VideoLyaer Bind Fail\n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}

MS_BOOL MHAL_DISP_VideoLayerUnBind(void *pVidLayerCtx, void *pDevCtx)
{
    MS_BOOL bRet = TRUE;

    if(pVidLayerCtx == NULL || pDevCtx == NULL)
    {
        DISP_ERR("%s %d, pVidLayerCtx or PDevCtx is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        if(DrvDispIfVideoLayerUnBind(pVidLayerCtx, pDevCtx) == FALSE)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, VideoLyaer UnBind Fail\n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}

MS_BOOL MHAL_DISP_VideoLayerSetAttr(void *pVidLayerCtx, const MHAL_DISP_VideoLayerAttr_t *pstAttr)
{
    MS_BOOL bRet = TRUE;

    if(pVidLayerCtx == NULL || pstAttr == NULL)
    {
        DISP_ERR("%s %d, pVidLayerCtx or pstAttr is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        if(DrvDispIfVideoLayerSetAttr(pVidLayerCtx, (MHAL_DISP_VideoLayerAttr_t *)pstAttr) == FALSE)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, VideoLyaer SetAttr Fail\n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}

MS_BOOL MHAL_DISP_VideoLayerBufferFire(void *pVidLayerCtx)
{
    MS_BOOL bRet = TRUE;

    if(pVidLayerCtx == NULL)
    {
        DISP_ERR("%s %d, pVidLayerCtx is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        if(DrvDispIfVideoLayerBufferFire(pVidLayerCtx) == FALSE)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, VideoLyaer BufferFire Fail\n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}

MS_BOOL MHAL_DISP_VideoLayerCheckBufferFired(void *pVidLayerCtx)
{
    MS_BOOL bRet = TRUE;

    if(pVidLayerCtx == NULL)
    {
        DISP_ERR("%s %d, pVidLayerCtx is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        if(DrvDispIfVideoLayerCheckBufferFired(pVidLayerCtx) == FALSE)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, VideoLyaer Check BufferFire Fail\n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}

// Video layer: Set/Get compress
MS_BOOL MHAL_DISP_VideoLayerSetCompress(void *pVidLayerCtx, const MHAL_DISP_CompressAttr_t* pstCompressAttr)
{
    MS_BOOL bRet = TRUE;

    if(pVidLayerCtx == NULL || pstCompressAttr == NULL)
    {
        DISP_ERR("%s %d, pVidLayerCtx or pstCompressAttr is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        if(DrvDispIfVideoLayerSetCompress(pVidLayerCtx, (MHAL_DISP_CompressAttr_t*) pstCompressAttr) == FALSE)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, VideoLyaer SetComporess Fail\n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}

// Video layer: Set/Get display priority
MS_BOOL MHAL_DISP_VideoLayerSetPriority(void *pVidLayerCtx, const MS_U32 u32Priority)
{
    MS_BOOL bRet = TRUE;

    if(pVidLayerCtx == NULL)
    {
        DISP_ERR("%s %d, pVidLayerCtx is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        if(DrvDispIfVideoLayerSetPriority(pVidLayerCtx, (u32) u32Priority) == FALSE)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, VideoLyaer SetPriority Fail\n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}

MS_BOOL MHAL_DISP_VideoLayerGetHwCount(MS_U32 *pu32Count)
{
    MS_BOOL bRet = TRUE;

    if(DrvDispIfVideoLayerGetHwCount((u32 *)pu32Count) == FALSE)
    {
        DISP_ERR("%s %d, Get Hw Count Fail\n", __FUNCTION__, __LINE__);
        bRet = FALSE;

    }
    return bRet;
}

MS_BOOL MHAL_DISP_InputPortCreateInstance(const MHAL_DISP_AllocPhyMem_t *pstAlloc, void *pVidLayerCtx, const MS_U32 u32PortId, void **pCtx)
{
    MS_BOOL bRet = TRUE;
    if(pstAlloc == NULL || pVidLayerCtx == NULL)
    {
        DISP_ERR("%s %d, pstAlloc or pVidLayerCtx is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {

        if(DrvDispIfInputPortCreateInstance((MHAL_DISP_AllocPhyMem_t *)pstAlloc, pVidLayerCtx, (u32)u32PortId, pCtx) == FALSE)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, CreateInstance Fail\n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}

MS_BOOL MHAL_DISP_InputPortDestroyInstance(const void *pInputPortCtx)
{
    MS_BOOL bRet = TRUE;

    if(pInputPortCtx == NULL)
    {
        DISP_ERR("%s %d, pInputPortCtx is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        if(DrvDispIfInputPortDestroyInstance((void *)pInputPortCtx) == FALSE)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, DestoryInstance Fail\n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}

MS_BOOL MHAL_DISP_InputPortFlip(void *pInputPortCtx, MHAL_DISP_VideoFrameData_t *pstVideoFrameBuffer)
{
    MS_BOOL bRet = TRUE;

    if(pInputPortCtx == NULL || pstVideoFrameBuffer == NULL)
    {
        DISP_ERR("%s %d, pInputPortCtx or pstVideoFrameBuffer is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        if(DrvDispIfInputPortFlip(pInputPortCtx, pstVideoFrameBuffer) == FALSE)
        {
            bRet = FALSE;
            DISP_DBG(DISP_DBG_LEVEL_DRV, "%s %d, InputPort Flip Fail\n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}


MS_BOOL MHAL_DISP_InputPortEnable(void *pInputPortCtx, const MS_BOOL bEnable)
{
    MS_BOOL bRet = TRUE;

    if(pInputPortCtx == NULL)
    {
        DISP_ERR("%s %d, pInputPortCtx is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        if(DrvDispIfInputPortEnable(pInputPortCtx, (bool)bEnable) == FALSE)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, InputPort Enable Fail\n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}


MS_BOOL MHAL_DISP_InputPortSetAttr(void *pInputPortCtx, const MHAL_DISP_InputPortAttr_t *pstAttr)
{
    MS_BOOL bRet = TRUE;

    if(pInputPortCtx == NULL || pstAttr == NULL)
    {
        DISP_ERR("%s %d, pInputPortCtx or pstAttr is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        if(DrvDispIfInputPortSetAttr(pInputPortCtx, (MHAL_DISP_InputPortAttr_t *)pstAttr) == FALSE)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, InputPort SetAttr Fail\n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}

MS_BOOL MHAL_DISP_InputPortShow(void *pInputPortCtx)
{
    MS_BOOL bRet = TRUE;

    if(pInputPortCtx == NULL)
    {
        DISP_ERR("%s %d, pInputPortCtx is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        if(DrvDispIfInputPortShow(pInputPortCtx) == FALSE)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, InputPort Show Fail\n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}

MS_BOOL MHAL_DISP_InputPortHide(void *pInputPortCtx)
{
    MS_BOOL bRet = TRUE;

    if(pInputPortCtx == NULL)
    {
        DISP_ERR("%s %d, pInputPortCtx is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        if(DrvDispIfInputPortHide(pInputPortCtx) == FALSE)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, InputPort Hide Fail\n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}



MS_BOOL MHAL_DISP_InputPortAttrBegin(void *pVidLayerCtx)
{
    MS_BOOL bRet = TRUE;

    if(pVidLayerCtx == NULL)
    {
        DISP_ERR("%s %d, pVidLayerCtx is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        if(DrvDispIfInputPortAttrBegin(pVidLayerCtx) == FALSE)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, InputPort AttrBegin Fail\n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}

MS_BOOL MHAL_DISP_InputPortAttrEnd(void *pVidLayerCtx)
{
    MS_BOOL bRet = TRUE;

    if(pVidLayerCtx == NULL)
    {
        DISP_ERR("%s %d, pVidLayerCtx is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        if(DrvDispIfInputPortAttrEnd(pVidLayerCtx) == FALSE)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, InputPort AttrEnd Fail\n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}

MS_BOOL MHAL_DISP_InputPortRotate(void *pInputPortCtx, MHAL_DISP_RotateConfig_t *pstRotateCfg)
{
    MS_BOOL bRet = TRUE;

    if(pInputPortCtx == NULL)
    {
        DISP_ERR("%s %d, pInputPortCtx is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        if(DrvDispIfInputPortRotate(pInputPortCtx, pstRotateCfg) == FALSE)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, InputPort Rotate Fail\n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}

MS_BOOL MHAL_DISP_InputPortSetCropAttr(void *pInputPortCtx, MHAL_DISP_VidWinRect_t *pstWinRect)

{
    MS_BOOL bRet = TRUE;

    if(pInputPortCtx == NULL)
    {
        DISP_ERR("%s %d, pInputPortCtx is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        if(DrvDispIfInputPortSetCropAttr(pInputPortCtx, pstWinRect) == FALSE)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, InputPort Rotate Fail\n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}

MS_BOOL MHAL_DISP_InputPortSetRingBuffAttr(void *pInputPortCtx, MHAL_DISP_RingBufferAttr_t *pstRingBufAttr)
{
    MS_BOOL bRet = TRUE;

    if(pInputPortCtx == NULL)
    {
        DISP_ERR("%s %d, pInputPortCtx is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        if(DrvDispIfInputPortSetRingBuffAttr(pInputPortCtx, pstRingBufAttr) == FALSE)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, InputPort RingBuffAttr Fail\n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}

MS_BOOL MHAL_DISP_InputPortGetHwCount(MS_U32 u32VidLayerId, MS_U32 *pu32Count)
{
    MS_BOOL bRet = TRUE;

    if(DrvDispIfInputPortGetHwCount((u32)u32VidLayerId, (u32 *)pu32Count) == FALSE)
    {
        DISP_ERR("%s %d, Get Hw Count Fail\n", __FUNCTION__, __LINE__);
        bRet = FALSE;

    }
    return bRet;
}

MS_BOOL MHAL_DISP_InputPortSetImiAddr(void *pInputPortCtx, const MS_U32 u32ImiAddr)
{
    MS_BOOL bRet = TRUE;

    if(pInputPortCtx == NULL)
    {
        DISP_ERR("%s %d, pInputPortCtx is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        if(DrvDispIfInputPortSetImiAddr(pInputPortCtx, (u32)u32ImiAddr) == FALSE)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, InputPort Enable Fail\n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}


MS_BOOL MHAL_DISP_DmaCreateInstance(MHAL_DISP_AllocPhyMem_t *pstAlloc, MS_U32 u32DmaId, void **pDmaCtx)
{
    MS_BOOL bRet = TRUE;

    if(DrvDispIfDmaCreateInstance((MHAL_DISP_AllocPhyMem_t *)pstAlloc, u32DmaId, pDmaCtx) == FALSE)
    {
        bRet = FALSE;
        DISP_ERR("%s %d, CreateInstance Fail\n", __FUNCTION__, __LINE__);
    }
    return bRet;
}

MS_BOOL MHAL_DISP_DmaDestoryInstance(void *pDmaCtx)
{
    MS_BOOL bRet = TRUE;
    if(DrvDispIfDmaDestoryInstance(pDmaCtx) == FALSE)
    {
        bRet = FALSE;
        DISP_ERR("%s %d, DestoryInstance Fail\n", __FUNCTION__, __LINE__);
    }
    return bRet;
}

MS_BOOL MHAL_DISP_DmaBind(void *pDmaCtx, MHAL_DISP_DmaBindConfig_t *pstDmaBindCfg)
{
    MS_BOOL bRet = TRUE;
    if(DrvDispIfDmaBind(pDmaCtx, pstDmaBindCfg) == FALSE)
    {
        bRet = FALSE;
        DISP_ERR("%s %d, Device Bind Dma Fail \n", __FUNCTION__, __LINE__);
    }
    return bRet;
}

MS_BOOL MHAL_DISP_DmaUnBind(void *pDmaCtx, MHAL_DISP_DmaBindConfig_t *pstDmaBindCfg)
{
    MS_BOOL bRet = TRUE;
    if(DrvDispIfDmaUnBind(pDmaCtx, pstDmaBindCfg) == FALSE)
    {
        bRet = FALSE;
        DISP_ERR("%s %d, Device UnBind Dma Fail \n", __FUNCTION__, __LINE__);
    }
    return bRet;
}

MS_BOOL MHAL_DISP_DmaSetAttr(void *pDmaCtx, MHAL_DISP_DmaAttrConfig_t *pstDmaAttrCfg)
{
    MS_BOOL bRet = TRUE;

    if(DrvDispIfDmaSetAttr(pDmaCtx, pstDmaAttrCfg) == FALSE)
    {
        bRet = FALSE;
        DISP_ERR("%s %d, Device SetDmaAttr Fail \n", __FUNCTION__, __LINE__);
    }
    return bRet;
}

MS_BOOL MHAL_DISP_DmaSetBufferAttr(void *pDmaCtx, MHAL_DISP_DmaBufferAttrConfig_t *pstDmaBufferAttrCfg)
{
    MS_BOOL bRet = TRUE;
    if(DrvDispIfDmaSetBufferAttr(pDmaCtx, pstDmaBufferAttrCfg) == FALSE)
    {
        bRet = FALSE;
        DISP_ERR("%s %d, Device SetDmaBufferAttr Fail \n", __FUNCTION__, __LINE__);
    }
    return bRet;
}


MS_BOOL MHAL_DISP_EnableDmaIrq(void *pDmaCtx, MS_U32 u32DevIrq, MS_BOOL bEnable)
{
    MS_BOOL bRet = TRUE;

    if(pDmaCtx == NULL)
    {
        DISP_ERR("%s %d, Ctx is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        if(DrvDispIrqEnable(pDmaCtx, u32DevIrq, bEnable) == FALSE)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, Set EnableIrq Fail \n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}

MS_BOOL MHAL_DISP_ClearDmaInterrupt(void *pDmaCtx, void* pData)
{
    MS_BOOL bRet = TRUE;

    if(pDmaCtx == NULL)
    {
        DISP_ERR("%s %d, Ctx is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        if(DrvDispIrqClear(pDmaCtx, pData) == FALSE)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, Clear Irq Fail \n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}


MS_BOOL MHAL_DISP_GetDmaIrqFlag(void *pDmaCtx, MHAL_DISP_IRQFlag_t *pstIrqFlag)
{
    MS_BOOL bRet = TRUE;

    if(pDmaCtx == NULL)
    {
        DISP_ERR("%s %d, Ctx is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        if(DrvDispIrqGetFlag(pDmaCtx, pstIrqFlag) == FALSE)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, Clear Irq Fail \n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}


MS_BOOL MHAL_DISP_GetDmaIrq(void *pDmaCtx, MS_U32* pu32DevIrq)
{
    MS_BOOL bRet = TRUE;

    if(pDmaCtx == NULL)
    {
        DISP_ERR("%s %d, Ctx is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        if(DrvDispIrqGetIsrNum(pDmaCtx, (u32 *)pu32DevIrq) == FALSE)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, Clear Irq Fail \n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}

MS_BOOL MHAL_DISP_DbgLevel(void *p)
{
    MS_BOOL bRet = TRUE;

    if(p == NULL)
    {
        DISP_ERR("%s %d, p is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        if(DrvDispIfSetDbgLevel(p) == FALSE)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, Set DebugLevel Fail \n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}


// Irq
MS_BOOL MHAL_DISP_EnableDevIrq(void *pDevCtx, MS_U32 u32DevIrq, MS_BOOL bEnable)
{
    MS_BOOL bRet = TRUE;

    if(pDevCtx == NULL)
    {
        DISP_ERR("%s %d, Ctx is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        if(DrvDispIrqEnable(pDevCtx, u32DevIrq, bEnable) == FALSE)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, Set EnableIrq Fail \n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}

MS_BOOL MHAL_DISP_ClearDevInterrupt(void *pDevCtx, void* pData)
{
    MS_BOOL bRet = TRUE;

    if(pDevCtx == NULL)
    {
        DISP_ERR("%s %d, Ctx is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        if(DrvDispIrqClear(pDevCtx, pData) == FALSE)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, Clear Irq Fail \n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}


MS_BOOL MHAL_DISP_GetDevIrqFlag(void *pDevCtx, MHAL_DISP_IRQFlag_t *pstIrqFlag)
{
    MS_BOOL bRet = TRUE;

    if(pDevCtx == NULL)
    {
        DISP_ERR("%s %d, Ctx is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        if(DrvDispIrqGetFlag(pDevCtx, pstIrqFlag) == FALSE)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, Clear Irq Fail \n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}


MS_BOOL MHAL_DISP_GetDevIrq(void *pDevCtx, MS_U32* pu32DevIrq)
{
    MS_BOOL bRet = TRUE;

    if(pDevCtx == NULL)
    {
        DISP_ERR("%s %d, Ctx is Null\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        if(DrvDispIrqGetIsrNum(pDevCtx, (u32 *)pu32DevIrq) == FALSE)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, Clear Irq Fail \n", __FUNCTION__, __LINE__);
        }
    }
    return bRet;
}

MS_BOOL MHAL_DISP_SetRegAccessConfig(void * pDevCtx, MHAL_DISP_RegAccessConfig_t *pstRegAccessCfg)
{
    MS_BOOL bRet = TRUE;
    if(DrvDispIfSetRegAccessConfig(pDevCtx, pstRegAccessCfg) == FALSE)
    {
        bRet = FALSE;
        DISP_ERR("%s %d, Device SetRegAccess Fail \n", __FUNCTION__, __LINE__);
    }
    return bRet;
}

MS_BOOL MHAL_DISP_SetRegFlipConfig(void * pDevCtx, MHAL_DISP_RegFlipConfig_t *pstRegAccessCfg)
{
    MS_BOOL bRet = TRUE;
    if(DrvDispIfSetRegFlipConfig(pDevCtx, pstRegAccessCfg) == FALSE)
    {
        bRet = FALSE;
        DISP_ERR("%s %d, Device SetRegFlip Fail \n", __FUNCTION__, __LINE__);
    }
    return bRet;
}

MS_BOOL MHAL_DISP_SetRegWaitDoneConfig(void * pDevCtx, MHAL_DISP_RegWaitDoneConfig_t *pstRegWaitDoneCfg)
{
    MS_BOOL bRet = TRUE;
    if(DrvDispIfSetRegWaitDoneConfig(pDevCtx, pstRegWaitDoneCfg) == FALSE)
    {
        bRet = FALSE;
        DISP_ERR("%s %d, Device SetRegWaitDone Fail \n", __FUNCTION__, __LINE__);
    }
    return bRet;
}

MS_BOOL MHAL_DISP_SetCmdqInterface(u32 u32DevId, MHAL_DISP_CmdqInfConfig_t *pstCmdqInfCfg)
{
    MS_BOOL bRet = TRUE;
    if(DrvDispIfSetCmdqInterfaceConfig(u32DevId, pstCmdqInfCfg) == FALSE)
    {
        bRet = FALSE;
        DISP_ERR("%s %d, SetCmdqInf Fail \n", __FUNCTION__, __LINE__);
    }
    return bRet;
}

MS_BOOL MHAL_DISP_GetDevIrqById(MS_U32 u32DevId, MS_U32 *pu32DevIrq)
{
    MS_BOOL bRet = TRUE;


    if(DrvDispIrqGetIsrNumByDevId(u32DevId, (u32 *)pu32DevIrq) == FALSE)
    {
        bRet = FALSE;
        DISP_ERR("%s %d, Clear Irq Fail \n", __FUNCTION__, __LINE__);
    }

    return bRet;
}

