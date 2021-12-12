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

#define _DRV_DISP_IF_C_

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "drv_disp_os.h"
#include "hal_disp_common.h"
#include "disp_debug.h"
#include "mhal_common.h"
#include "mhal_cmdq.h"
#include "mhal_disp_datatype.h"
#include "hal_disp_chip.h"
#include "hal_disp_st.h"
#include "hal_disp_if.h"
#include "hal_disp_util.h"
#include "hal_disp.h"
#include "drv_disp_ctx.h"
#include "drv_disp_irq.h"
//-------------------------------------------------------------------------------------------------
//  Defines & Macro
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  structure
//-------------------------------------------------------------------------------------------------
typedef struct
{
    bool bValid;
    HalDispDeviceTiming_e   enDevTimingType;
    HalDispDeviceTimingConfig_t stDevTiming;
}DrvDispIfTimingData_t;

typedef struct
{
    u32 u32DataSize;
    DrvDispIfTimingData_t *pstData;
}DrvDispIfTimingConfig_t;
//-------------------------------------------------------------------------------------------------
//  Variable
//-------------------------------------------------------------------------------------------------
DrvDispIfTimingConfig_t gstDispDevTimingCfg = {0, NULL};

u32 _gu32DispDbgLevel = 0;

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
HalDispDeviceTiming_e __DrvDispIfTransDevTimingTypeToHal(MHAL_DISP_DeviceTiming_e eMhalTiming)
{
    HalDispDeviceTiming_e eHalTiming;

    eHalTiming = eMhalTiming == E_MHAL_DISP_OUTPUT_PAL           ?  E_HAL_DISP_OUTPUT_PAL          :
                 eMhalTiming == E_MHAL_DISP_OUTPUT_NTSC          ?  E_HAL_DISP_OUTPUT_NTSC         :
                 eMhalTiming == E_MHAL_DISP_OUTPUT_960H_PAL      ?  E_HAL_DISP_OUTPUT_960H_PAL     :
                 eMhalTiming == E_MHAL_DISP_OUTPUT_960H_NTSC     ?  E_HAL_DISP_OUTPUT_960H_NTSC    :
                 eMhalTiming == E_MHAL_DISP_OUTPUT_1080P24       ?  E_HAL_DISP_OUTPUT_1080P24      :
                 eMhalTiming == E_MHAL_DISP_OUTPUT_1080P25       ?  E_HAL_DISP_OUTPUT_1080P25      :
                 eMhalTiming == E_MHAL_DISP_OUTPUT_1080P30       ?  E_HAL_DISP_OUTPUT_1080P30      :
                 eMhalTiming == E_MHAL_DISP_OUTPUT_720P50        ?  E_HAL_DISP_OUTPUT_720P50       :
                 eMhalTiming == E_MHAL_DISP_OUTPUT_720P60        ?  E_HAL_DISP_OUTPUT_720P60       :
                 eMhalTiming == E_MHAL_DISP_OUTPUT_1080I50       ?  E_HAL_DISP_OUTPUT_1080I50      :
                 eMhalTiming == E_MHAL_DISP_OUTPUT_1080I60       ?  E_HAL_DISP_OUTPUT_1080I60      :
                 eMhalTiming == E_MHAL_DISP_OUTPUT_1080P50       ?  E_HAL_DISP_OUTPUT_1080P50      :
                 eMhalTiming == E_MHAL_DISP_OUTPUT_1080P60       ?  E_HAL_DISP_OUTPUT_1080P60      :
                 eMhalTiming == E_MHAL_DISP_OUTPUT_576P50        ?  E_HAL_DISP_OUTPUT_576P50       :
                 eMhalTiming == E_MHAL_DISP_OUTPUT_480P60        ?  E_HAL_DISP_OUTPUT_480P60       :
                 eMhalTiming == E_MHAL_DISP_OUTPUT_640x480_60    ?  E_HAL_DISP_OUTPUT_640x480_60   :
                 eMhalTiming == E_MHAL_DISP_OUTPUT_800x600_60    ?  E_HAL_DISP_OUTPUT_800x600_60   :
                 eMhalTiming == E_MHAL_DISP_OUTPUT_1024x768_60   ?  E_HAL_DISP_OUTPUT_1024x768_60  :
                 eMhalTiming == E_MHAL_DISP_OUTPUT_1280x1024_60  ?  E_HAL_DISP_OUTPUT_1280x1024_60 :
                 eMhalTiming == E_MHAL_DISP_OUTPUT_1366x768_60   ?  E_HAL_DISP_OUTPUT_1366x768_60  :
                 eMhalTiming == E_MHAL_DISP_OUTPUT_1440x900_60   ?  E_HAL_DISP_OUTPUT_1440x900_60  :
                 eMhalTiming == E_MHAL_DISP_OUTPUT_1280x800_60   ?  E_HAL_DISP_OUTPUT_1280x800_60  :
                 eMhalTiming == E_MHAL_DISP_OUTPUT_1680x1050_60  ?  E_HAL_DISP_OUTPUT_1680x1050_60 :
                 eMhalTiming == E_MHAL_DISP_OUTPUT_1920x2160_30  ?  E_HAL_DISP_OUTPUT_1920x2160_30 :
                 eMhalTiming == E_MHAL_DISP_OUTPUT_1600x1200_60  ?  E_HAL_DISP_OUTPUT_1600x1200_60 :
                 eMhalTiming == E_MHAL_DISP_OUTPUT_1920x1200_60  ?  E_HAL_DISP_OUTPUT_1920x1200_60 :
                 eMhalTiming == E_MHAL_DISP_OUTPUT_2560x1440_30  ?  E_HAL_DISP_OUTPUT_2560x1440_30 :
                 eMhalTiming == E_MHAL_DISP_OUTPUT_2560x1600_60  ?  E_HAL_DISP_OUTPUT_2560x1600_60 :
                 eMhalTiming == E_MHAL_DISP_OUTPUT_3840x2160_30  ?  E_HAL_DISP_OUTPUT_3840x2160_30 :
                 eMhalTiming == E_MHAL_DISP_OUTPUT_3840x2160_60  ?  E_HAL_DISP_OUTPUT_3840x2160_60 :
                 eMhalTiming == E_MHAL_DISP_OUTPUT_USER          ?  E_HAL_DISP_OUTPUT_USER         :
                                                                    E_HAL_DISP_OUTPUT_MAX;
    return eHalTiming;
}

HalDispCscmatrix_e __DrvDispIfTransCscMatrixToHal(MHAL_DISP_CscMattrix_e eMhalCscMatrixType)
{
    HalDispCscmatrix_e eHalCscMatrixType;

    eHalCscMatrixType =  eMhalCscMatrixType == E_MHAL_DISP_CSC_MATRIX_BYPASS          ? E_HAL_DISP_CSC_MATRIX_BYPASS          :
                         eMhalCscMatrixType == E_MHAL_DISP_CSC_MATRIX_BT601_TO_BT709  ? E_HAL_DISP_CSC_MATRIX_BT601_TO_BT709  :
                         eMhalCscMatrixType == E_MHAL_DISP_CSC_MATRIX_BT709_TO_BT601  ? E_HAL_DISP_CSC_MATRIX_BT709_TO_BT601  :
                         eMhalCscMatrixType == E_MHAL_DISP_CSC_MATRIX_BT601_TO_RGB_PC ? E_HAL_DISP_CSC_MATRIX_BT601_TO_RGB_PC :
                         eMhalCscMatrixType == E_MHAL_DISP_CSC_MATRIX_BT709_TO_RGB_PC ? E_HAL_DISP_CSC_MATRIX_BT709_TO_RGB_PC :
                         eMhalCscMatrixType == E_MHAL_DISP_CSC_MATRIX_RGB_TO_BT601_PC ? E_HAL_DISP_CSC_MATRIX_RGB_TO_BT601_PC :
                         eMhalCscMatrixType == E_MHAL_DISP_CSC_MATRIX_RGB_TO_BT709_PC ? E_HAL_DISP_CSC_MATRIX_RGB_TO_BT709_PC :
                                                                                        E_HAL_DISP_CSC_MATRIX_MAX;
    return eHalCscMatrixType;
}

HalDispPixelCompressMode_e __DrvDispIfTransCompressToHal(MHAL_DISP_PixelCompressMode_e eMhalCompress)
{
    HalDispPixelCompressMode_e eHalCompress;

    eHalCompress = eMhalCompress == E_MHAL_DISP_COMPRESS_MODE_NONE  ?  E_HAL_DISP_COMPRESS_MODE_NONE  :
                   eMhalCompress == E_MHAL_DISP_COMPRESS_MODE_SEG   ?  E_HAL_DISP_COMPRESS_MODE_SEG   :
                   eMhalCompress == E_MHAL_DISP_COMPRESS_MODE_LINE  ?  E_HAL_DISP_COMPRESS_MODE_LINE  :
                   eMhalCompress == E_MHAL_DISP_COMPRESS_MODE_FRAME ?  E_HAL_DISP_COMPRESS_MODE_FRAME :
                                                                       E_HAL_DISP_COMPRESS_MODE_MAX;
    return eHalCompress;
}

HalDispTileMode_e __DrvDispIfTransTileModeToHal(MHAL_DISP_TileMode_e eMhalTile)
{
    HalDispTileMode_e eHalTile;

    eHalTile = eMhalTile == E_MHAL_DISP_TILE_MODE_16x16 ? E_HAL_DISP_TILE_MODE_16x16 :
               eMhalTile == E_MHAL_DISP_TILE_MODE_16x32 ? E_HAL_DISP_TILE_MODE_16x32 :
               eMhalTile == E_MHAL_DISP_TILE_MODE_32x16 ? E_HAL_DISP_TILE_MODE_32x16 :
               eMhalTile == E_MHAL_DISP_TILE_MODE_32x32 ? E_HAL_DISP_TILE_MODE_32x32 :
                                                            E_HAL_DISP_TILE_MODE_NONE;
    return eHalTile;
}

HalDispPixelFormat_e __DrvDispIfTransPixelFormatToHal(MHalPixelFormat_e eMhalPixelFormat)
{
    HalDispPixelFormat_e eHalPixelFormat;

    eHalPixelFormat = eMhalPixelFormat ==  E_MHAL_PIXEL_FRAME_YUV422_YUYV        ? E_HAL_DISP_PIXEL_FRAME_YUV422_YUYV        :
                      eMhalPixelFormat ==  E_MHAL_PIXEL_FRAME_ARGB8888           ? E_HAL_DISP_PIXEL_FRAME_ARGB8888           :
                      eMhalPixelFormat ==  E_MHAL_PIXEL_FRAME_ABGR8888           ? E_HAL_DISP_PIXEL_FRAME_ABGR8888           :
                      eMhalPixelFormat ==  E_MHAL_PIXEL_FRAME_BGRA8888           ? E_HAL_DISP_PIXEL_FRAME_BGRA8888           :
                      eMhalPixelFormat ==  E_MHAL_PIXEL_FRAME_RGB565             ? E_HAL_DISP_PIXEL_FRAME_RGB565             :
                      eMhalPixelFormat ==  E_MHAL_PIXEL_FRAME_ARGB1555           ? E_HAL_DISP_PIXEL_FRAME_ARGB1555           :
                      eMhalPixelFormat ==  E_MHAL_PIXEL_FRAME_ARGB4444           ? E_HAL_DISP_PIXEL_FRAME_ARGB4444           :
                      eMhalPixelFormat ==  E_MHAL_PIXEL_FRAME_YUV_SEMIPLANAR_422 ? E_HAL_DISP_PIXEL_FRAME_YUV_SEMIPLANAR_422 :
                      eMhalPixelFormat ==  E_MHAL_PIXEL_FRAME_YUV_SEMIPLANAR_420 ? E_HAL_DISP_PIXEL_FRAME_YUV_SEMIPLANAR_420 :
                      eMhalPixelFormat ==  E_MHAL_PIXEL_FRAME_YUV_MST_420        ? E_HAL_DISP_PIXEL_FRAME_YUV_MST_420        :
                                                                                  E_HAL_DISP_PIXEL_FRAME_FORMAT_MAX;
    return eHalPixelFormat;
}

HalDispDmaPixelFormat_e __DrvDispIfTransDmaPixelFmtToHal(MHalPixelFormat_e eMhalPixelFormat)
{
    HalDispDmaPixelFormat_e eHalPixelFormat;

    eHalPixelFormat = eMhalPixelFormat ==  E_MHAL_PIXEL_FRAME_YUV422_YUYV        ? E_HAL_DISP_DMA_PIX_FMT_YUV422   :
                      eMhalPixelFormat ==  E_MHAL_PIXEL_FRAME_YUV_SEMIPLANAR_422 ? E_HAL_DISP_DMA_PIX_FMT_YCSep422 :
                      eMhalPixelFormat ==  E_MHAL_PIXEL_FRAME_YUV_SEMIPLANAR_420 ? E_HAL_DISP_DMA_PIX_FMT_YUV420   :
                      eMhalPixelFormat ==  E_MHAL_PIXEL_FRAME_YUV_MST_420        ? E_HAL_DISP_DMA_PIX_FMT_YUV420   :
                      eMhalPixelFormat ==  E_MHAL_PIXEL_FRAME_ABGR8888           ? E_HAL_DISP_DMA_PIX_FMT_RGBA     :
                      eMhalPixelFormat ==  E_MHAL_PIXEL_FRAME_BGRA8888           ? E_HAL_DISP_DMA_PIX_FMT_ARGB     :
                      eMhalPixelFormat ==  E_MHAL_PIXEL_FRAME_ARGB8888           ? E_HAL_DISP_DMA_PIX_FMT_BGRA     :
                                                                                   E_HAL_DISP_DMA_PIX_FMT_NUM;
    return eHalPixelFormat;

}

HalDispDeviceTimingConfig_t * __DrvDispIfGetTimingFromTbl(HalDispDeviceTiming_e enHalTiminId)
{
    u32 i;
    HalDispDeviceTimingConfig_t *pstTimingCfg = NULL;
    DrvDispIfTimingData_t *pstTimingData = NULL;

    for(i=0; i< gstDispDevTimingCfg.u32DataSize; i++)
    {
        pstTimingData = &gstDispDevTimingCfg.pstData[i];
        if(pstTimingData->bValid && pstTimingData->enDevTimingType == enHalTiminId)
        {
            pstTimingCfg =  &pstTimingData->stDevTiming;
            break;
        }
    }

    return pstTimingCfg;
}

bool _DrvDispIfTransDeviceOutpuTimingInfoToHal(MHAL_DISP_DeviceTimingInfo_t *pstMhalCfg, HalDispDeviceTimingInfo_t *pstHalCfg)
{
    bool bRet = 1;
    HalDispDeviceTimingConfig_t *pstDeviceTimingCfg = NULL;
    u32 u32Tmp;

    pstHalCfg->eTimeType = __DrvDispIfTransDevTimingTypeToHal(pstMhalCfg->eTimeType);

    if(pstHalCfg->eTimeType == E_HAL_DISP_OUTPUT_USER)
    {
        if(pstMhalCfg->pstSyncInfo)
        {
            pstHalCfg->stDeviceTimingCfg.u16HsyncWidth     = pstMhalCfg->pstSyncInfo->u16Hpw;
            pstHalCfg->stDeviceTimingCfg.u16HsyncBackPorch = pstMhalCfg->pstSyncInfo->u16Hbb;
            pstHalCfg->stDeviceTimingCfg.u16Hactive        = pstMhalCfg->pstSyncInfo->u16Hact;
            pstHalCfg->stDeviceTimingCfg.u16Hstart         = pstMhalCfg->pstSyncInfo->u16Hpw + pstMhalCfg->pstSyncInfo->u16Hbb;
            pstHalCfg->stDeviceTimingCfg.u16Htotal         = pstMhalCfg->pstSyncInfo->u16Hpw + pstMhalCfg->pstSyncInfo->u16Hbb +
                                                             pstMhalCfg->pstSyncInfo->u16Hact + pstMhalCfg->pstSyncInfo->u16Hfb;
            pstHalCfg->stDeviceTimingCfg.u16VsyncWidth     = pstMhalCfg->pstSyncInfo->u16Vpw;
            pstHalCfg->stDeviceTimingCfg.u16VsyncBackPorch = pstMhalCfg->pstSyncInfo->u16Vbb;
            pstHalCfg->stDeviceTimingCfg.u16Vactive        = pstMhalCfg->pstSyncInfo->u16Vact;
            pstHalCfg->stDeviceTimingCfg.u16Vstart         = pstMhalCfg->pstSyncInfo->u16Vpw + pstMhalCfg->pstSyncInfo->u16Vbb;
            pstHalCfg->stDeviceTimingCfg.u16Vtotal         = pstMhalCfg->pstSyncInfo->u16Vpw + pstMhalCfg->pstSyncInfo->u16Vbb +
                                                             pstMhalCfg->pstSyncInfo->u16Vact + pstMhalCfg->pstSyncInfo->u16Vfb;
            pstHalCfg->stDeviceTimingCfg.u16Fps            = pstMhalCfg->pstSyncInfo->u32FrameRate;

            u32Tmp = 1000000UL / (u32)pstHalCfg->stDeviceTimingCfg.u16Fps;
            pstHalCfg->stDeviceTimingCfg.u32VSyncPeriod       = u32Tmp *  (u32)pstMhalCfg->pstSyncInfo->u16Vpw / (u32)pstHalCfg->stDeviceTimingCfg.u16Vtotal;
            pstHalCfg->stDeviceTimingCfg.u32VBackPorchPeriod  = u32Tmp *  (u32)pstMhalCfg->pstSyncInfo->u16Vbb / (u32)pstHalCfg->stDeviceTimingCfg.u16Vtotal;
            pstHalCfg->stDeviceTimingCfg.u32VActivePeriod     = u32Tmp *  (u32)pstMhalCfg->pstSyncInfo->u16Vact / (u32)pstHalCfg->stDeviceTimingCfg.u16Vtotal;
            pstHalCfg->stDeviceTimingCfg.u32VFrontPorchPeriod = u32Tmp *  (u32)pstMhalCfg->pstSyncInfo->u16Vfb / (u32)pstHalCfg->stDeviceTimingCfg.u16Vtotal;
        }
        else
        {
            bRet = 0;
            DISP_ERR("%s %d, SyncInfo is Null\n ", __FUNCTION__, __LINE__);
        }
    }
    else
    {
        pstDeviceTimingCfg = __DrvDispIfGetTimingFromTbl(pstHalCfg->eTimeType);
        if(pstDeviceTimingCfg)
        {
            memcpy(&pstHalCfg->stDeviceTimingCfg, pstDeviceTimingCfg, sizeof(HalDispDeviceTimingConfig_t));
        }
        else
        {
            bRet = 0;
            DISP_ERR("%s %d, Can't find Timing(%s) in Table\n",
                __FUNCTION__, __LINE__, PARSING_HAL_TMING_ID(pstHalCfg->eTimeType));
        }
    }
    return bRet;
}

void _DrvDispIfTransDeviceCvbsParamToHal(MHAL_DISP_CvbsParam_t *pstMhalCfg, HalDispCvbsParam_t *pstHalCfg)
{
    pstHalCfg->bEnable= pstMhalCfg->bEnable;
    DISP_DBG(DISP_DBG_LEVEL_DRV, "%s %d, En:%d\n",
        __FUNCTION__, __LINE__,  pstHalCfg->bEnable);

}

void _DrvDispIfTransDeviceVgaParamToHal(MHAL_DISP_VgaParam_t *pstMhalCfg, HalDispVgaParam_t *pstHalCfg)
{
    pstHalCfg->stCsc.eCscMatrix    = __DrvDispIfTransCscMatrixToHal(pstMhalCfg->stCsc.eCscMatrix);
    pstHalCfg->stCsc.u32Luma       = pstMhalCfg->stCsc.u32Luma;
    pstHalCfg->stCsc.u32Hue        = pstMhalCfg->stCsc.u32Hue;
    pstHalCfg->stCsc.u32Contrast   = pstMhalCfg->stCsc.u32Contrast;
    pstHalCfg->stCsc.u32Saturation = pstMhalCfg->stCsc.u32Saturation;

    pstHalCfg->u32Gain = pstMhalCfg->u32Gain;
    pstHalCfg->u32Sharpness = pstMhalCfg->u32Sharpness;

    DISP_DBG(DISP_DBG_LEVEL_DRV, "%s %d, Csc:%s, Luma:%ld, Hue:%ld, Con:%ld, Sat%ld, Gain:%ld Sharp:%ld\n",
        __FUNCTION__, __LINE__,
        PARSING_HAL_CSC_MATRIX(pstHalCfg->stCsc.eCscMatrix),
        pstHalCfg->stCsc.u32Luma, pstHalCfg->stCsc.u32Hue, pstHalCfg->stCsc.u32Contrast,
        pstHalCfg->stCsc.u32Saturation, pstHalCfg->u32Gain, pstHalCfg->u32Sharpness);
}

void _DrvDispIfTransDeviceHdmiParamToHal(MHAL_DISP_HdmiParam_t *pstMhalCfg, HalDispHdmiParam_t *pstHalCfg)
{
    pstHalCfg->stCsc.eCscMatrix    = __DrvDispIfTransCscMatrixToHal(pstMhalCfg->stCsc.eCscMatrix);
    pstHalCfg->stCsc.u32Luma       = pstMhalCfg->stCsc.u32Luma;
    pstHalCfg->stCsc.u32Hue        = pstMhalCfg->stCsc.u32Hue;
    pstHalCfg->stCsc.u32Contrast   = pstMhalCfg->stCsc.u32Contrast;
    pstHalCfg->stCsc.u32Saturation = pstMhalCfg->stCsc.u32Saturation;
    pstHalCfg->u32Sharpness        = pstMhalCfg->u32Sharpness;

    DISP_DBG(DISP_DBG_LEVEL_DRV, "%s %d, Csc:%s, Luma:%ld, Hue:%ld, Con:%ld, Sat:%ld, Sharp:%ld\n",
        __FUNCTION__, __LINE__,
        PARSING_HAL_CSC_MATRIX(pstHalCfg->stCsc.eCscMatrix),
        pstHalCfg->stCsc.u32Luma, pstHalCfg->stCsc.u32Hue, pstHalCfg->stCsc.u32Contrast,
        pstHalCfg->stCsc.u32Saturation,
        pstHalCfg->u32Sharpness);
}

void _DrvDispIfTransDeviceLcdParamToHal(MHAL_DISP_LcdParam_t *pstMhalCfg, HalDispLcdParam_t *pstHalCfg)
{
    pstHalCfg->stCsc.eCscMatrix    = __DrvDispIfTransCscMatrixToHal(pstMhalCfg->stCsc.eCscMatrix);
    pstHalCfg->stCsc.u32Luma       = pstMhalCfg->stCsc.u32Luma;
    pstHalCfg->stCsc.u32Hue        = pstMhalCfg->stCsc.u32Hue;
    pstHalCfg->stCsc.u32Contrast   = pstMhalCfg->stCsc.u32Contrast;
    pstHalCfg->stCsc.u32Saturation = pstMhalCfg->stCsc.u32Saturation;

    pstHalCfg->u32Sharpness = pstMhalCfg->u32Sharpness;

    DISP_DBG(DISP_DBG_LEVEL_DRV, "%s %d, Csc:%s, Luma:%ld, Hue:%ld, Con:%ld, Sat%ld, Sharp:%ld\n",
        __FUNCTION__, __LINE__,
        PARSING_HAL_CSC_MATRIX(pstHalCfg->stCsc.eCscMatrix),
        pstHalCfg->stCsc.u32Luma, pstHalCfg->stCsc.u32Hue, pstHalCfg->stCsc.u32Contrast,
        pstHalCfg->stCsc.u32Saturation, pstHalCfg->u32Sharpness);
}

void _DrvDispIfTransdeviceGammaParamToHal(MHAL_DISP_GammaParam_t *pstMhalCfg, HalDispGammaParam_t *pstHalCfg)
{
    pstHalCfg->bEn = pstMhalCfg->bEn;
    pstHalCfg->u16EntryNum = pstMhalCfg->u16EntryNum;
    pstHalCfg->pu8ColorR = pstMhalCfg->pu8ColorR;
    pstHalCfg->pu8ColorG = pstMhalCfg->pu8ColorG;
    pstHalCfg->pu8ColorB = pstMhalCfg->pu8ColorB;

    DISP_DBG(DISP_DBG_LEVEL_DRV, "%s %d, En:%d, EntryNum:%d\n",
        __FUNCTION__, __LINE__,
        pstHalCfg->bEn, pstHalCfg->u16EntryNum);
}



void _DrvDispIfTransDeviceColorTempToHal(MHAL_DISP_ColorTempeture_t *pstMhalCfg, HalDispColorTemp_t *pstHalCfg)
{
    pstHalCfg->u16RedColor   = pstMhalCfg->u16RedColor;
    pstHalCfg->u16GreenColor = pstMhalCfg->u16GreenColor;
    pstHalCfg->u16BlueColor  = pstMhalCfg->u16BlueColor;
    pstHalCfg->u16RedOffset   = pstMhalCfg->u16RedOffset;
    pstHalCfg->u16GreenOffset = pstMhalCfg->u16GreenOffset;
    pstHalCfg->u16BlueOffset  = pstMhalCfg->u16BlueOffset;

    DISP_DBG(DISP_DBG_LEVEL_DRV, "%s %d, Color(%d %d %d), Offset(%d %d %d)\n",
        __FUNCTION__, __LINE__,
        pstHalCfg->u16RedColor, pstHalCfg->u16GreenColor, pstHalCfg->u16BlueColor,
        pstHalCfg->u16RedOffset, pstHalCfg->u16GreenOffset, pstHalCfg->u16BlueOffset);
}

void _DrvDispIfTransVidLayerAttrToHal(MHAL_DISP_VideoLayerAttr_t *pstMhalCfg, HalDispVideoLayerAttr_t *pstHalCfg)
{
    pstHalCfg->stVidLayerDispWin.u16X      = pstMhalCfg->stVidLayerDispWin.u16X;
    pstHalCfg->stVidLayerDispWin.u16Y      = pstMhalCfg->stVidLayerDispWin.u16Y;
    pstHalCfg->stVidLayerDispWin.u16Width  = pstMhalCfg->stVidLayerDispWin.u16Width;
    pstHalCfg->stVidLayerDispWin.u16Height = pstMhalCfg->stVidLayerDispWin.u16Height;

    pstHalCfg->stVidLayerSize.u32Height = pstMhalCfg->stVidLayerSize.u32Height;
    pstHalCfg->stVidLayerSize.u32Width = pstMhalCfg->stVidLayerSize.u32Width;

    pstHalCfg->ePixFormat = __DrvDispIfTransPixelFormatToHal(pstMhalCfg->ePixFormat);

    DISP_DBG(DISP_DBG_LEVEL_DRV, "%s %d, Pixel:%s, Disp(%d %d %d %d) Size(%ld %ld)\n",
        __FUNCTION__, __LINE__,
        PARSING_HAL_PIXEL_FMT(pstHalCfg->ePixFormat),
        pstHalCfg->stVidLayerDispWin.u16X, pstHalCfg->stVidLayerDispWin.u16Y,
        pstHalCfg->stVidLayerDispWin.u16Width, pstHalCfg->stVidLayerDispWin.u16Height,
        pstHalCfg->stVidLayerSize.u32Width, pstHalCfg->stVidLayerSize.u32Height);
}

void _DrvDispIfTransDeviceTimeZoneToMhal(MHAL_DISP_TimeZone_t *pstMhalCfg, HalDispTimeZone_t *pstHalCfg)
{
    pstMhalCfg->enType = pstHalCfg->enType == E_HAL_DISP_TIMEZONE_SYNC       ? E_MHAL_DISP_TIMEZONE_SYNC :
                         pstHalCfg->enType == E_HAL_DISP_TIMEZONE_BACKPORCH  ? E_MHAL_DISP_TIMEZONE_BACKPORCH :
                         pstHalCfg->enType == E_HAL_DISP_TIMEZONE_ACTIVE     ? E_MHAL_DISP_TIMEZONE_ACTIVE :
                         pstHalCfg->enType == E_HAL_DISP_TIMEZONE_FRONTPORCH ? E_MHAL_DISP_TIMEZONE_FRONTPORCH :
                                                                               E_MHAL_DISP_TIMEZONE_UNKONWN;

    //DISP_DBG(DISP_DBG_LEVEL_DRV, "%s %d, TimeZone=%s\n",
    //    __FUNCTION__, __LINE__,
    //    PARSING_HAL_TIMEZONE(pstHalCfg->enType));

}

void _DrvDispIfTransDeviceTimeZoneCfgToHal(MHAL_DISP_TimeZoneConfig_t *pstMhalCfg, HalDispTimeZoneConfig_t *pstHalCfg)
{
    u8 i;

    for(i=0; i<E_MHAL_DISP_TIMEZONE_NUM; i++)
    {
        pstHalCfg->u16InvalidArea[i] = pstMhalCfg->u16InvalidArea[i];
    }
}

void _DrvDispIfTransDeviceDisplayInfoToMhal(MHAL_DISP_DisplayInfo_t *pstMhalCfg, HalDispDisplayInfo_t *pstHalCfg)
{
    pstMhalCfg->u16Htotal = pstHalCfg->u16Htotal;
    pstMhalCfg->u16Vtotal = pstHalCfg->u16Vtotal;
    pstMhalCfg->u16HdeStart = pstHalCfg->u16HdeStart;
    pstMhalCfg->u16VdeStart = pstHalCfg->u16VdeStart;
    pstMhalCfg->u16Width = pstHalCfg->u16Width;
    pstMhalCfg->u16Height = pstHalCfg->u16Height;
    pstMhalCfg->bInterlaceMode = pstHalCfg->bInterlace;
    pstMhalCfg->bYuvOutput = pstHalCfg->bYuvOutput;

    DISP_DBG(DISP_DBG_LEVEL_DRV, "%s %d: Htt:%d, Vtt:%d, Hst:%d Vst:%d, Interlace:%d, Yuv:%d\n",
        __FUNCTION__, __LINE__,
        pstMhalCfg->u16Htotal, pstMhalCfg->u16Vtotal,
        pstMhalCfg->u16HdeStart, pstMhalCfg->u16VdeStart,
        pstMhalCfg->bInterlaceMode, pstMhalCfg->bYuvOutput);
}

void _DrvDispIfTransVidLayerCompressToHal(MHAL_DISP_CompressAttr_t *pstMhalCfg, HalDispVideoLayerCompressAttr_t *pstHalCfg)
{

   pstHalCfg->bEnbale = pstMhalCfg->bEnbale;

    DISP_DBG(DISP_DBG_LEVEL_DRV, "%s %d, bEn:%d\n", __FUNCTION__, __LINE__, pstHalCfg->bEnbale);
}

void _DrvDispIfTransInputPortFrameDataToHal(MHAL_DISP_VideoFrameData_t *pstMhalCfg, HalDispVideoFrameData_t *pstHalCfg)
{
    pstHalCfg->ePixelFormat = __DrvDispIfTransPixelFormatToHal(pstMhalCfg->ePixelFormat);
    pstHalCfg->eCompressMode = __DrvDispIfTransCompressToHal(pstMhalCfg->eCompressMode);
    pstHalCfg->eTileMode = __DrvDispIfTransTileModeToHal(pstMhalCfg->eTileMode);
    pstHalCfg->au32Stride[0] = pstMhalCfg->au32Stride[0];
    pstHalCfg->au32Stride[1] = pstMhalCfg->au32Stride[1];
    pstHalCfg->au32Stride[2] = pstMhalCfg->au32Stride[2];

    pstHalCfg->au64PhyAddr[0] = pstMhalCfg->aPhyAddr[0];
    pstHalCfg->au64PhyAddr[1] = pstMhalCfg->aPhyAddr[1];
    pstHalCfg->au64PhyAddr[2] = pstMhalCfg->aPhyAddr[2];

    DISP_DBG(DISP_DBG_LEVEL_DRV, "%s %d, Pixel:%s, Compress:%s, Title:%s, (%llx, %llx, %llx), (%ld %ld %ld)\n",
        __FUNCTION__, __LINE__,
        PARSING_HAL_PIXEL_FMT(pstHalCfg->ePixelFormat ),
        PARSING_HAL_COMPRESS_MD(pstHalCfg->eCompressMode),
        PARSING_HAL_TILE_MD(pstHalCfg->eTileMode),
        pstHalCfg->au64PhyAddr[0], pstHalCfg->au64PhyAddr[1], pstHalCfg->au64PhyAddr[2],
        pstHalCfg->au32Stride[0], pstHalCfg->au32Stride[1], pstHalCfg->au32Stride[2]);
}

void _DrvDispIfTransInputPortRingBufAttrToHal(MHAL_DISP_RingBufferAttr_t *pstMhalCfg, HalDispRingBuffAttr_t *pstHalCfg)
{
    pstHalCfg->bEn = pstMhalCfg->bEn;
    pstHalCfg->u16BuffHeight = pstMhalCfg->u16BuffHeight;
    pstHalCfg->u16BuffStartLine = pstMhalCfg->u16BuffStartLine;

    pstHalCfg->eAccessType =
        pstMhalCfg->eAccessType == E_MHAL_DISP_DMA_ACCESS_TYPE_EMI ? E_HAL_DISP_DMA_ACCESS_TYPE_EMI :
        pstMhalCfg->eAccessType == E_MHAL_DISP_DMA_ACCESS_TYPE_IMI ? E_HAL_DISP_DMA_ACCESS_TYPE_IMI :
                                                                     E_HAL_DISP_DMA_ACCESS_TYPE_NUM;
    DISP_DBG(DISP_DBG_LEVEL_DRV, "%s %d, En:%d, BuffHeight:%d, StartLine:%d, Access:%s\n",
        __FUNCTION__, __LINE__,
        pstHalCfg->bEn, pstHalCfg->u16BuffHeight, pstHalCfg->u16BuffStartLine,
        PARSING_HAL_DMA_ACCESS_TYPE(pstHalCfg->eAccessType));
}

void _DrvDispIfTransInputPortRotateToHal(MHAL_DISP_RotateConfig_t *pstMhalCfg, HalDispInputPortRotate_t *pstHalCfg)
{
    pstHalCfg->enRotate = pstMhalCfg->enRotate == E_MHAL_DISP_ROTATE_NONE ? E_HAL_DISP_ROTATE_NONE :
                          pstMhalCfg->enRotate == E_MHAL_DISP_ROTATE_90   ? E_HAL_DISP_ROTATE_90 :
                          pstMhalCfg->enRotate == E_MHAL_DISP_ROTATE_180  ? E_HAL_DISP_ROTATE_180 :
                          pstMhalCfg->enRotate == E_MHAL_DISP_ROTATE_270  ? E_HAL_DISP_ROTATE_270 :
                                                                            E_HAL_DISP_ROTATE_NUM;


    DISP_DBG(DISP_DBG_LEVEL_DRV, "%s %d, Rotate:%s\n",
        __FUNCTION__, __LINE__,
        PARSING_HAL_ROTATE(pstHalCfg->enRotate));
}

void _DrvDispIfTransInputPortAttrToHal(MHAL_DISP_InputPortAttr_t *pMhalCfg, HalDispInputPortAttr_t *pstHalCfg)
{
    pstHalCfg->stDispWin.u16X      = pMhalCfg->stDispWin.u16X;
    pstHalCfg->stDispWin.u16Y      = pMhalCfg->stDispWin.u16Y;
    pstHalCfg->stDispWin.u16Width  = pMhalCfg->stDispWin.u16Width;
    pstHalCfg->stDispWin.u16Height = pMhalCfg->stDispWin.u16Height;
    pstHalCfg->u16SrcHeight        = pMhalCfg->u16SrcHeight;
    pstHalCfg->u16SrcWidth         = pMhalCfg->u16SrcWidth;

    DISP_DBG(DISP_DBG_LEVEL_DRV, "%s %d, Rect(%d %d %d %d) Src(%d %d)\n",
        __FUNCTION__, __LINE__,
        pstHalCfg->stDispWin.u16X, pstHalCfg->stDispWin.u16Y,
        pstHalCfg->stDispWin.u16Width, pstHalCfg->stDispWin.u16Height,
        pstHalCfg->u16SrcWidth, pstHalCfg->u16SrcHeight);
}
void _DrvDispIfTransInputPortCropAttrToHal(MHAL_DISP_VidWinRect_t *pMhalCfg, HalDispVidWinRect_t *pstHalCfg)
{
    pstHalCfg->u16X      = pMhalCfg->u16X;
    pstHalCfg->u16Y      = pMhalCfg->u16Y;
    pstHalCfg->u16Width  = pMhalCfg->u16Width;
    pstHalCfg->u16Height = pMhalCfg->u16Height;


    DISP_DBG(DISP_DBG_LEVEL_DRV, "%s %d, crop Rect(%d %d %d %d)\n",
        __FUNCTION__, __LINE__,
        pstHalCfg->u16X, pstHalCfg->u16Y,
        pstHalCfg->u16Width, pstHalCfg->u16Height);
}

void _DrvDispIfTransRegAccessToHal(MHAL_DISP_RegAccessConfig_t *pMhalCfg, HalDispRegAccessConfig_t *pstHalCfg)
{
    pstHalCfg->enType = pMhalCfg->enType == E_MHAL_DISP_REG_ACCESS_CPU  ? E_HAL_DISP_REG_ACCESS_CPU :
                        pMhalCfg->enType == E_MHAL_DISP_REG_ACCESS_CMDQ ? E_HAL_DISP_REG_ACCESS_CMDQ :
                                                                          E_HAL_DISP_REG_ACCESS_NUM;
    pstHalCfg->pCmdqInf = (void *)pMhalCfg->pCmdqInf;

    DISP_DBG(DISP_DBG_LEVEL_DRV, "%s %d, Access:%s, CmdqIntf:%p \n",
        __FUNCTION__, __LINE__,
        PARSING_HAL_REG_ACCESS_TYPE(pstHalCfg->enType), pstHalCfg->pCmdqInf);
}

void _DrvDispIfTransRegFlipToHal(MHAL_DISP_RegFlipConfig_t *pMhalCfg, HalDispRegFlipConfig_t *pstHalCfg)
{
    pstHalCfg->bEnable = pMhalCfg->bEnable;
    pstHalCfg->pCmdqInf = (void *)pMhalCfg->pCmdqInf;

    DISP_DBG(DISP_DBG_LEVEL_DRV, "%s %d, Enable:%d, CmdqIntf:%p \n",
        __FUNCTION__, __LINE__,
        pstHalCfg->bEnable, pstHalCfg->pCmdqInf);
}

void _DrvDispIfTransRegWaitDoneToHal(MHAL_DISP_RegWaitDoneConfig_t *pMhalCfg, HalDispRegWaitDoneConfig_t *pstHalCfg)
{
    pstHalCfg->u32WaitType = pMhalCfg->u32WaitType;
    pstHalCfg->pCmdqInf = (void *)pMhalCfg->pCmdqInf;

    DISP_DBG(DISP_DBG_LEVEL_DRV, "%s %d, WaitType:%lx, CmdqIntf:%p \n",
        __FUNCTION__, __LINE__,
        pstHalCfg->u32WaitType, pstHalCfg->pCmdqInf);
}

void _DrvDispIfTransPqCfgToHal(MHAL_DISP_PqConfig_t *pstMhalCfg, HalDispPqConfig_t *pstHalCfg)
{
    pstHalCfg->u32PqFlags = pstMhalCfg->u32PqFlags;
    pstHalCfg->u32DataSize = pstMhalCfg->u32DataSize;
    pstHalCfg->pData = pstMhalCfg->pData;
    DISP_DBG(DISP_DBG_LEVEL_DRV, "%s %d, PqFlag=%lx, Size=%ld, pData=%p \n",
        __FUNCTION__, __LINE__,
        pstHalCfg->u32PqFlags, pstHalCfg->u32DataSize, pstHalCfg->pData);
}

void _DrvDispIfTransDmaBindToHal(MHAL_DISP_DmaBindConfig_t *pstMhalCfg, HalDispDmaBindConfig_t *pstHalCfg)
{
    pstHalCfg->pSrcDevCtx = pstMhalCfg->pSrcDevCtx;
    pstHalCfg->pDestDevCtx = pstMhalCfg->pDestDevCtx;
}

void _DrvDispIfTransDmaAttrToHal(MHAL_DISP_DmaAttrConfig_t *pstMhalCfg, HalDispDmaAttrConfig_t *pstHalCfg)
{
    // In Config
    pstHalCfg->stInputCfg.eType =
        pstMhalCfg->stInputCfg.eType == E_MHAL_DISP_DMA_INPUT_DEVICE_FRONT ? E_HAL_DISP_DMA_INPUT_DEVICE_FRONT :
        pstMhalCfg->stInputCfg.eType == E_MHAL_DISP_DMA_INPUT_DEVICE_BACK  ? E_HAL_DISP_DMA_INPUT_DEVICE_BACK  :
                                                                             E_HAL_DISP_DMA_INPUT_NUM;

    pstHalCfg->stInputCfg.ePixelFormat  = __DrvDispIfTransDmaPixelFmtToHal(pstMhalCfg->stInputCfg.ePixelFormat);
    pstHalCfg->stInputCfg.eCompressMode = __DrvDispIfTransCompressToHal(pstMhalCfg->stInputCfg.eCompressMode);

    pstHalCfg->stInputCfg.u16Width = pstMhalCfg->stInputCfg.u16Width;
    pstHalCfg->stInputCfg.u16Height = pstMhalCfg->stInputCfg.u16Height;

    // Out Config
    pstHalCfg->stOutputCfg.eAccessType =
        pstMhalCfg->stOutputCfg.eAccessType == E_MHAL_DISP_DMA_ACCESS_TYPE_EMI ? E_HAL_DISP_DMA_ACCESS_TYPE_EMI :
        pstMhalCfg->stOutputCfg.eAccessType == E_MHAL_DISP_DMA_ACCESS_TYPE_IMI ? E_HAL_DISP_DMA_ACCESS_TYPE_IMI :
                                                                                 E_HAL_DISP_DMA_ACCESS_TYPE_NUM;
    pstHalCfg->stOutputCfg.eMode =
        pstMhalCfg->stOutputCfg.eMode == E_MHAL_DISP_DMA_OUTPUT_MODE_FRAME ? E_HAL_DISP_DMA_OUTPUT_MODE_FRAME :
         pstMhalCfg->stOutputCfg.eMode == E_MHAL_DISP_DMA_OUTPUT_MODE_RING ? E_HAL_DISP_DMA_OUTPUT_MODE_RING  :
                                                                             E_HAL_DISP_DMA_OUTPUT_MODE_NUM;

    pstHalCfg->stOutputCfg.ePixelFormat  = __DrvDispIfTransDmaPixelFmtToHal(pstMhalCfg->stOutputCfg.ePixelFormat);
    pstHalCfg->stOutputCfg.eCompressMode = __DrvDispIfTransCompressToHal(pstMhalCfg->stOutputCfg.eCompressMode);

    pstHalCfg->stOutputCfg.u16Width = pstMhalCfg->stOutputCfg.u16Width;
    pstHalCfg->stOutputCfg.u16Height = pstMhalCfg->stOutputCfg.u16Height;


    DISP_DBG(DISP_DBG_LEVEL_DRV, "%s %d, IN: Type:%s, PixFmt%s, Compress%s, Size(%d %d)\n",
        __FUNCTION__, __LINE__,
        PARSING_HAL_DMA_INPUT_TYPE(pstHalCfg->stInputCfg.eType),
        PARSING_HAL_DMA_PIXEL_FMT(pstHalCfg->stInputCfg.ePixelFormat),
        PARSING_HAL_COMPRESS_MD(pstHalCfg->stInputCfg.eCompressMode),
        pstHalCfg->stInputCfg.u16Width, pstHalCfg->stInputCfg.u16Height);

    DISP_DBG(DISP_DBG_LEVEL_DRV, "%s %d, OUT:Access:%s, Mode:%s, PixFmt:%s, Compress:%s, Width:%d, Heigh:%d\n",
        __FUNCTION__, __LINE__,
        PARSING_HAL_DMA_ACCESS_TYPE(pstHalCfg->stOutputCfg.eAccessType),
        PARSING_HAL_DMA_OUTPUT_MODE(pstHalCfg->stOutputCfg.eMode),
        PARSING_HAL_DMA_PIXEL_FMT(pstHalCfg->stOutputCfg.ePixelFormat),
        PARSING_HAL_COMPRESS_MD(pstHalCfg->stOutputCfg.eCompressMode),
        pstHalCfg->stOutputCfg.u16Width, pstHalCfg->stOutputCfg.u16Height);
}

void _DrvDispIfTransDmaBufferAttrToHal(MHAL_DISP_DmaBufferAttrConfig_t *pstMhalCfg, HalDispDmaBufferAttrConfig_t *pstHalCfg)
{
    u8 j;

        pstHalCfg->bEn = pstMhalCfg->bEn;

        for(j=0; j<HAL_DISP_DMA_PIX_FMT_PLANE_MAX; j++)
        {
            pstHalCfg->u64PhyAddr[j] = pstMhalCfg->u64PhyAddr[j];
            pstHalCfg->u32Stride[j] = pstMhalCfg->u32Stride[j];
        }

        pstHalCfg->u16RingBuffHeight = pstMhalCfg->u16RingBuffHeight;
        pstHalCfg->u16RingStartLine = pstMhalCfg->u16RingStartLine;
        pstHalCfg->u16FrameIdx = pstMhalCfg->u16FrameIdx;

        DISP_DBG(DISP_DBG_LEVEL_DRV, "%s %d, PhyAddr(%llx %llx %llx), Stride(%ld %ld %ld), RingBufHeight:%d, RingStartLine:%d FrameIdx:%d\n",
            __FUNCTION__, __LINE__,
            pstHalCfg->u64PhyAddr[0], pstHalCfg->u64PhyAddr[1], pstHalCfg->u64PhyAddr[2],
            pstHalCfg->u32Stride[0], pstHalCfg->u32Stride[1], pstHalCfg->u32Stride[2],
            pstHalCfg->u16RingBuffHeight,
            pstHalCfg->u16RingStartLine,
            pstHalCfg->u16FrameIdx);

}

void _DrvDispIfTransDeviceConfigToHal(MHAL_DISP_DeviceConfig_t *pMhalCfg, HalDispInternalConfig_t *pstHalCfg)
{
    pstHalCfg->eType = pMhalCfg->eType;
    pstHalCfg->bBootlogoEn = pMhalCfg->bBootlogoEn;
    pstHalCfg->u8ColorId = pMhalCfg->u8ColorId;
    pstHalCfg->u8GopBlendId = pMhalCfg->u8GopBlendId;
    DISP_DBG(DISP_DBG_LEVEL_DRV, "%s %d, Type:%x, Bootlogo:%d, ColorId:%d GopBlendId:%d\n",
        __FUNCTION__, __LINE__,
        pstHalCfg->eType, pstHalCfg->bBootlogoEn,
        pstHalCfg->u8ColorId, pstHalCfg->u8GopBlendId);
}

void _DrvDispIfTransDeviceConfigToMHal(MHAL_DISP_DeviceConfig_t *pMhalCfg, HalDispInternalConfig_t *pstHalCfg)
{
    pMhalCfg->eType = pstHalCfg->eType;
    pMhalCfg->bBootlogoEn = pstHalCfg->bBootlogoEn;
    pMhalCfg->u8ColorId = pstHalCfg->u8ColorId;
    pMhalCfg->u8GopBlendId = pstHalCfg->u8GopBlendId;
    DISP_DBG(DISP_DBG_LEVEL_DRV, "%s %d, Type:%x, Bootlogo:%d, ColorId:%d GopBlendId:%d\n",
        __FUNCTION__, __LINE__,
        pstHalCfg->eType, pstHalCfg->bBootlogoEn,
        pstHalCfg->u8ColorId, pstHalCfg->u8GopBlendId);
}


bool _DrvDispIfExecuteQuery(void *pCtx, HalDispQueryConfig_t *pstQueryCfg)
{
    bool bRet = 1;

    if(HalDispIfQuery(pCtx, pstQueryCfg))
    {
        if(pstQueryCfg->stOutCfg.enQueryRet == E_HAL_DISP_QUERY_RET_OK ||
           pstQueryCfg->stOutCfg.enQueryRet == E_HAL_DISP_QUERY_RET_NONEED)
        {
            if(pstQueryCfg->stOutCfg.pSetFunc)
            {
                pstQueryCfg->stOutCfg.pSetFunc(pCtx, pstQueryCfg->stInCfg.pInCfg);
            }
        }
        else if( pstQueryCfg->stOutCfg.enQueryRet == E_HAL_DISP_QUERY_RET_IMPLICIT_ERR)
        {
            bRet = 0;
        }
        else
        {
            bRet = 0;
            DISP_ERR("%s %d, Query:%s, Ret:%s\n",
                __FUNCTION__,__LINE__,
                PARSING_HAL_QUERY_TYPE(pstQueryCfg->stInCfg.enQueryType),
                PARSING_HAL_QUERY_RET(pstQueryCfg->stOutCfg.enQueryRet));
        }
    }
    else
    {
        bRet = 0;
        DISP_ERR("%s %d, Query Fail\n", __FUNCTION__, __LINE__);
    }
    return bRet;
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
bool DrvDispIfGetClk(void *pDevCtx, bool *pbEn, u32 *pu32ClkRate, u32 u32ClkNum)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;
    HalDispClkConfig_t stHalClkCfg;

    HalDispIfInit();

    memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));
    memset(&stHalClkCfg, 0, sizeof(HalDispClkConfig_t));
    stHalClkCfg.u32Num = u32ClkNum;

    stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_CLK_GET;
    stQueryCfg.stInCfg.u32CfgSize = sizeof(stHalClkCfg);
    stQueryCfg.stInCfg.pInCfg = &stHalClkCfg;

    if(_DrvDispIfExecuteQuery(pDevCtx, &stQueryCfg))
    {
        if(stHalClkCfg.u32Num == u32ClkNum)
        {
            memcpy(pu32ClkRate, stHalClkCfg.u32Rate, sizeof(stHalClkCfg.u32Rate));
            memcpy(pbEn, stHalClkCfg.bEn, sizeof(stHalClkCfg.bEn));
            bRet = 1;
        }
    }
    else
    {
        bRet = FALSE;
        DISP_ERR("%s %d, Get Clk Fail\n", __FUNCTION__, __LINE__);
    }
    return bRet;
}

bool DrvDispIfSetClk(void *pDevCtx, bool *pbEn, u32 *pu32ClkRate, u32 u32ClkNum)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;
    HalDispClkConfig_t stHalClkCfg;

    if( sizeof(stHalClkCfg.u32Rate) != sizeof(u32)*u32ClkNum ||
        sizeof(stHalClkCfg.bEn) != sizeof(bool)*u32ClkNum)
    {
        bRet = 0;
        DISP_ERR("%s %d, Clk Num is not correct: Rate:%d != %ld, En:%d != %ld",
            __FUNCTION__, __LINE__,
            sizeof(stHalClkCfg.u32Rate), sizeof(u32)*u32ClkNum,
            sizeof(stHalClkCfg.bEn), sizeof(bool)*u32ClkNum);
    }
    else
    {
        HalDispIfInit();

        memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));
        stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_CLK_SET;
        stQueryCfg.stInCfg.u32CfgSize = sizeof(HalDispClkConfig_t);
        stQueryCfg.stInCfg.pInCfg = &stHalClkCfg;


        stHalClkCfg.u32Num = u32ClkNum;
        memcpy(stHalClkCfg.u32Rate, pu32ClkRate, sizeof(u32)*u32ClkNum);
        memcpy(stHalClkCfg.bEn, pbEn, sizeof(bool)*u32ClkNum);

        bRet = _DrvDispIfExecuteQuery(pDevCtx, &stQueryCfg);
    }
    return bRet;
}


bool DrvDispIfInitPanelConfig(MHAL_DISP_PanelConfig_t* pstPanelConfig, u8 u8Size)
{
    u8 i;
    bool bRet = TRUE;
    u32 u32Mod, u32HttVtt,u32DClkhz;
    u32 u32Tmp;
    u16 u16FrontPorch;
    HalDispDeviceTimingConfig_t *pstDevTiming = NULL;
    DrvDispIfTimingData_t *pstTimingData = NULL;

    if(u8Size > E_HAL_DISP_OUTPUT_MAX)
    {
        bRet = FALSE;
        DISP_ERR("%s %d, The size (%d) is bigger than %d\n", __FUNCTION__, __LINE__, u8Size, E_HAL_DISP_OUTPUT_MAX);
    }
    else
    {
        DISP_DBG(DISP_DBG_LEVEL_DRV, "%s %d, u8Size:%d\n", __FUNCTION__, __LINE__, u8Size);

        if(gstDispDevTimingCfg.pstData)
        {
            DrvDispOsMemRelease(gstDispDevTimingCfg.pstData);
            gstDispDevTimingCfg.pstData = NULL;
            gstDispDevTimingCfg.u32DataSize = 0;
        }

        gstDispDevTimingCfg.pstData = DrvDispOsMemAlloc(sizeof(DrvDispIfTimingData_t) * u8Size);
        gstDispDevTimingCfg.u32DataSize = u8Size;

        if(gstDispDevTimingCfg.pstData == NULL)
        {
            bRet = FALSE;
            DISP_ERR("%s %d, Allocate Memory Fail\n", __FUNCTION__, __LINE__);
        }
        else
        {
            for(i=0; i<u8Size; i++)
            {
                pstTimingData = &gstDispDevTimingCfg.pstData[i];
                pstTimingData->bValid = pstPanelConfig[i].bValid;
                pstTimingData->enDevTimingType = __DrvDispIfTransDevTimingTypeToHal(pstPanelConfig[i].eTiming);

                pstDevTiming = &pstTimingData->stDevTiming;
                pstDevTiming->u16HsyncWidth     = pstPanelConfig[i].stPanelAttr.m_ucPanelHSyncWidth;
                pstDevTiming->u16HsyncBackPorch = pstPanelConfig[i].stPanelAttr.m_ucPanelHSyncBackPorch;
                pstDevTiming->u16Hstart         = pstPanelConfig[i].stPanelAttr.m_wPanelHStart;
                pstDevTiming->u16Hactive        = pstPanelConfig[i].stPanelAttr.m_wPanelWidth;
                pstDevTiming->u16Htotal         = pstPanelConfig[i].stPanelAttr.m_wPanelHTotal;

                pstDevTiming->u16VsyncWidth     = pstPanelConfig[i].stPanelAttr.m_ucPanelVSyncWidth;
                pstDevTiming->u16VsyncBackPorch = pstPanelConfig[i].stPanelAttr.m_ucPanelVBackPorch;
                pstDevTiming->u16Vstart         = pstPanelConfig[i].stPanelAttr.m_wPanelVStart;
                pstDevTiming->u16Vactive        = pstPanelConfig[i].stPanelAttr.m_wPanelHeight;
                pstDevTiming->u16Vtotal         = pstPanelConfig[i].stPanelAttr.m_wPanelVTotal;

                u32DClkhz = ((u32)pstPanelConfig[i].stPanelAttr.m_dwPanelDCLK * 1000000);
                u32HttVtt = ((u32)pstPanelConfig[i].stPanelAttr.m_wPanelHTotal * (u32)pstPanelConfig[i].stPanelAttr.m_wPanelVTotal);
                u32Mod = u32DClkhz % u32HttVtt;

                if(u32Mod > (u32HttVtt/2))
                {
                    pstDevTiming->u16Fps = (u32DClkhz + u32HttVtt -1) / u32HttVtt;
                }
                else
                {
                    pstDevTiming->u16Fps = u32DClkhz / u32HttVtt;
                }

                pstDevTiming->u16SscSpan = pstPanelConfig[i].stPanelAttr.m_wSpreadSpectrumSpan;
                pstDevTiming->u16SscStep = pstPanelConfig[i].stPanelAttr.m_wSpreadSpectrumStep;

                u32Tmp = 1000000UL / (u32)pstDevTiming->u16Fps;
                u16FrontPorch = (pstDevTiming->u16Vtotal - pstDevTiming->u16VsyncWidth - pstDevTiming->u16VsyncBackPorch - pstDevTiming->u16Vactive);
                pstDevTiming->u32VSyncPeriod       = u32Tmp * (u32)pstDevTiming->u16VsyncWidth / (u32)pstDevTiming->u16Vtotal;
                pstDevTiming->u32VBackPorchPeriod  = u32Tmp * (u32)pstDevTiming->u16VsyncBackPorch / (u32)pstDevTiming->u16Vtotal;
                pstDevTiming->u32VActivePeriod     = u32Tmp * (u32)pstDevTiming->u16Vactive / (u32)pstDevTiming->u16Vtotal;
                pstDevTiming->u32VFrontPorchPeriod = u32Tmp * (u32)u16FrontPorch / (u32)pstDevTiming->u16Vtotal;
            }
        }
    }
    return bRet;
}


bool DrvDispIfSetRegAccessConfig(void *pDevCtx, MHAL_DISP_RegAccessConfig_t *pstRegAccessCfg)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;
    HalDispRegAccessConfig_t stHalRegAccessCfg;

    if(DrvDispCtxSetCurCtx((DrvDispCtxConfig_t *)pDevCtx, ((DrvDispCtxConfig_t *)pDevCtx)->u32Idx) == FALSE)
    {
        bRet = FALSE;
    }
    else
    {
        memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));

        stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_REG_ACCESS;
        stQueryCfg.stInCfg.u32CfgSize = sizeof(HalDispRegAccessConfig_t);
        stQueryCfg.stInCfg.pInCfg = (void *)&stHalRegAccessCfg;

        _DrvDispIfTransRegAccessToHal(pstRegAccessCfg, &stHalRegAccessCfg);

        bRet = _DrvDispIfExecuteQuery(pDevCtx, &stQueryCfg);
    }
    return bRet;
}

bool DrvDispIfSetRegFlipConfig(void *pDevCtx, MHAL_DISP_RegFlipConfig_t *pstRegFlipCfg)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;
    HalDispRegFlipConfig_t stHalRegFlipCfg;

    if(DrvDispCtxSetCurCtx((DrvDispCtxConfig_t *)pDevCtx, ((DrvDispCtxConfig_t *)pDevCtx)->u32Idx) == FALSE)
    {
        bRet = FALSE;
    }
    else
    {
        memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));

        stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_REG_FLIP;
        stQueryCfg.stInCfg.u32CfgSize = sizeof(stHalRegFlipCfg);
        stQueryCfg.stInCfg.pInCfg = (void *)&stHalRegFlipCfg;

        _DrvDispIfTransRegFlipToHal(pstRegFlipCfg, &stHalRegFlipCfg);

        bRet = _DrvDispIfExecuteQuery(pDevCtx, &stQueryCfg);
    }
    return bRet;
}

bool DrvDispIfSetRegWaitDoneConfig(void *pDevCtx, MHAL_DISP_RegWaitDoneConfig_t *pstRegWaitDoneCfg)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;
    HalDispRegWaitDoneConfig_t stHalRegWaitDoneCfg;

    if(DrvDispCtxSetCurCtx((DrvDispCtxConfig_t *)pDevCtx, ((DrvDispCtxConfig_t *)pDevCtx)->u32Idx) == FALSE)
    {
        bRet = FALSE;
    }
    else
    {
        memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));

        stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_REG_WAITDONE;
        stQueryCfg.stInCfg.u32CfgSize = sizeof(HalDispRegWaitDoneConfig_t);
        stQueryCfg.stInCfg.pInCfg = (void *)&stHalRegWaitDoneCfg;

        _DrvDispIfTransRegWaitDoneToHal(pstRegWaitDoneCfg, &stHalRegWaitDoneCfg);

        bRet = _DrvDispIfExecuteQuery(pDevCtx, &stQueryCfg);
    }
    return bRet;
}

bool DrvDispIfSetCmdqInterfaceConfig(u32 u32DevId, MHAL_DISP_CmdqInfConfig_t *pstCmdqInfCfg)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;
    HalDispCmdqInfConfig_t stHalCmdqIfCfg;
    DrvDispCtxConfig_t stDispCtxCfg;

    HalDispIfInit();

    memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));

    stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_CMDQINF;
    stQueryCfg.stInCfg.u32CfgSize = sizeof(HalDispCmdqInfConfig_t);
    stQueryCfg.stInCfg.pInCfg = (void *)&stHalCmdqIfCfg;

    stHalCmdqIfCfg.pCmdqInf = (void *)pstCmdqInfCfg->pCmdqInf;
    stHalCmdqIfCfg.u32DevId = u32DevId;

    bRet = _DrvDispIfExecuteQuery(&stDispCtxCfg, &stQueryCfg);
    return bRet;
}

bool DrvDispIfDeviceCreateInstance(MHAL_DISP_AllocPhyMem_t *pstAlloc, u32 u32DeviceId, void **pDevCtx)
{
    bool bRet = TRUE;
    DrvDispCtxAllocConfig_t stCtxAllocCfg;
    HalDispQueryConfig_t stQueryCfg;
    DrvDispCtxConfig_t *pstDispCtx = NULL;

    DrvDispCtxInit();

    stCtxAllocCfg.enType = E_DISP_CTX_TYPE_DEVICE;
    stCtxAllocCfg.u32Id = u32DeviceId;
    stCtxAllocCfg.pstBindCtx = NULL;
    stCtxAllocCfg.stMemAllcCfg.alloc = (pDispCtxMemAlloc)pstAlloc->alloc;
    stCtxAllocCfg.stMemAllcCfg.free =  (pDispCtxMemFree)pstAlloc->free;

    if(DrvDispCtxAllocate(&stCtxAllocCfg, &pstDispCtx) == FALSE)
    {
        bRet = 0;
        *pDevCtx = NULL;
        DISP_ERR("%s %d, CreateInstance Fail\n", __FUNCTION__, __LINE__);
    }
    else
    {
        u32 au32ClkRate[HAL_DISP_CLK_NUM] = HAL_DISP_CLK_RATE_SETTING;
        bool abClkEn[HAL_DISP_CLK_NUM] = HAL_DISP_CLK_ON_SETTING;

        *pDevCtx = (void *)pstDispCtx;

        if(DrvDispOsSetClkOn(au32ClkRate, HAL_DISP_CLK_NUM) == 0)
        {
            if(DrvDispIfSetClk((void *)pstDispCtx, abClkEn, au32ClkRate, HAL_DISP_CLK_NUM) == 0)
            {
                DISP_ERR("%s %d:: SetClk Fail\n", __FUNCTION__, __LINE__);
                bRet = 0;
            }
        }

        if(bRet == 1)
        {
            memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));
            stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_DEVICE_INIT;
            stQueryCfg.stInCfg.pInCfg = NULL;
            stQueryCfg.stInCfg.u32CfgSize = 0;

            if(_DrvDispIfExecuteQuery(pstDispCtx, &stQueryCfg))
            {
                DrvDispCtxSetCurCtx(pstDispCtx, u32DeviceId);
                DrvDispIrqCreateInternalIsr((void *)pstDispCtx);
            }
            else
            {
                DISP_ERR("%s %d:: Device Init Fail\n", __FUNCTION__, __LINE__);
                bRet = 0;
            }
        }
    }

    return bRet;
}

bool DrvDispIfDeviceDestroyInstance(void *pDevCtx)
{
    bool bRet = TRUE;
    HalDispQueryConfig_t stQueryCfg;
    DrvDispCtxConfig_t *pstDispCtx = (DrvDispCtxConfig_t *) pDevCtx;

    if(DrvDispCtxSetCurCtx(pstDispCtx, pstDispCtx->u32Idx) == FALSE)
    {
        bRet = FALSE;
    }
    else
    {
        memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));
        stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_DEVICE_DEINIT;
        stQueryCfg.stInCfg.pInCfg = NULL;
        stQueryCfg.stInCfg.u32CfgSize = 0;

        if(_DrvDispIfExecuteQuery(pstDispCtx, &stQueryCfg))
        {
            u32 au32ClkRate[HAL_DISP_CLK_NUM] = HAL_DISP_CLK_OFF_RATE_SETTING;
            bool abClkEn[HAL_DISP_CLK_NUM] = HAL_DISP_CLK_OFF_SETTING;

            if(DrvDispOsSetClkOff(au32ClkRate, HAL_DISP_CLK_NUM) == 0)
            {
                if(DrvDispIfSetClk(pDevCtx, abClkEn, au32ClkRate, HAL_DISP_CLK_NUM) ==0)
                {
                    bRet = FALSE;
                    DISP_ERR("%s %d, Set Clk Off Fail\n", __FUNCTION__, __LINE__);
                }
            }

            bRet = DrvDispIrqDestroyInternalIsr((void *)pstDispCtx);

            if(bRet && DrvDispCtxFree(pstDispCtx))
            {
                if(DrvDispCtxIsAllFree())
                {
                    DrvDispCtxDeInit();
                }
            }
            else
            {
                bRet = 0;
                DISP_ERR("%s %d, DestroyInstance Fail\n", __FUNCTION__, __LINE__);
            }
        }
        else
        {
            bRet = 0;
            DISP_ERR("%s %d, DestroyInstance Fail\n", __FUNCTION__, __LINE__);
        }
    }

    return bRet;
}

bool DrvDispIfDeviceEnable(void *pDevCtx, bool bEnable)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;

    if(DrvDispCtxSetCurCtx((DrvDispCtxConfig_t *)pDevCtx, ((DrvDispCtxConfig_t *)pDevCtx)->u32Idx) == FALSE)
    {
        bRet = FALSE;
    }
    else
    {
        memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));

        stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_DEVICE_ENABLE;
        stQueryCfg.stInCfg.u32CfgSize = sizeof(bool);
        stQueryCfg.stInCfg.pInCfg = (void *)&bEnable;

        bRet = _DrvDispIfExecuteQuery(pDevCtx, &stQueryCfg);
    }
    return bRet;
}

bool DrvDispIfDeviceSetBackGroundColor(void *pDevCtx, u32 u32BgColor)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;

    if(DrvDispCtxSetCurCtx((DrvDispCtxConfig_t *)pDevCtx, ((DrvDispCtxConfig_t *)pDevCtx)->u32Idx) == FALSE)
    {
        bRet = FALSE;
    }
    else
    {
        memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));

        stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_DEVICE_BACKGROUND_COLOR;
        stQueryCfg.stInCfg.u32CfgSize = sizeof(u32);
        stQueryCfg.stInCfg.pInCfg = (void *)&u32BgColor;

        bRet = _DrvDispIfExecuteQuery(pDevCtx, &stQueryCfg);
    }
    return bRet;
}


bool DrvDispIfDeviceAddOutInterface(void *pDevCtx, u32 u32Interface)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;

    if(DrvDispCtxSetCurCtx((DrvDispCtxConfig_t *)pDevCtx, ((DrvDispCtxConfig_t *)pDevCtx)->u32Idx) == FALSE)
    {
        bRet = FALSE;
    }
    else
    {
        memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));

        stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_DEVICE_INTERFACE;
        stQueryCfg.stInCfg.u32CfgSize = sizeof(u32);
        stQueryCfg.stInCfg.pInCfg = (void *)&u32Interface;

        bRet = _DrvDispIfExecuteQuery(pDevCtx, &stQueryCfg);
    }
    return bRet;
}

bool DrvDispIfDeviceSetOutputTiming(void *pDevCtx, u32 u32Interface, MHAL_DISP_DeviceTimingInfo_t *pstTimingInfo)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;
    HalDispDeviceTimingInfo_t stHalTimingIfo;

    if(DrvDispCtxSetCurCtx((DrvDispCtxConfig_t *)pDevCtx, ((DrvDispCtxConfig_t *)pDevCtx)->u32Idx) == FALSE)
    {
        bRet = FALSE;
    }
    else
    {
        memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));

        stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_DEVICE_OUTPUTTIMING;
        stQueryCfg.stInCfg.u32CfgSize = sizeof(HalDispDeviceTimingInfo_t);
        stQueryCfg.stInCfg.pInCfg = (void *)&stHalTimingIfo;

        if(_DrvDispIfTransDeviceOutpuTimingInfoToHal(pstTimingInfo, &stHalTimingIfo))
        {
            bRet = _DrvDispIfExecuteQuery(pDevCtx, &stQueryCfg);
        }
        else
        {
            bRet = 0;
        }
    }

    return bRet;
}

bool DrvDispIfDeviceSetColortemp(void *pDevCtx, MHAL_DISP_ColorTempeture_t *pstcolorTemp)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;
    HalDispColorTemp_t stHalColorTemp;

    if(DrvDispCtxSetCurCtx((DrvDispCtxConfig_t *)pDevCtx, ((DrvDispCtxConfig_t *)pDevCtx)->u32Idx) == FALSE)
    {
        bRet = FALSE;
    }
    else
    {
        memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));

        stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_DEVICE_COLORTEMP;
        stQueryCfg.stInCfg.u32CfgSize = sizeof(HalDispColorTemp_t);
        stQueryCfg.stInCfg.pInCfg = (void *)&stHalColorTemp;

        _DrvDispIfTransDeviceColorTempToHal(pstcolorTemp, &stHalColorTemp);

        bRet = _DrvDispIfExecuteQuery(pDevCtx, &stQueryCfg);
    }
    return bRet;
}

bool DrvDispIfDeviceSetCvbsParam(void *pDevCtx, MHAL_DISP_CvbsParam_t *pstCvbsInfo)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;
    HalDispCvbsParam_t stHalCvbsParam;

    if(DrvDispCtxSetCurCtx((DrvDispCtxConfig_t *)pDevCtx, ((DrvDispCtxConfig_t *)pDevCtx)->u32Idx) == FALSE)
    {
        bRet = FALSE;
    }
    else
    {
        memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));

        stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_DEVICE_CVBS_PARAM;
        stQueryCfg.stInCfg.u32CfgSize = sizeof(HalDispCvbsParam_t);
        stQueryCfg.stInCfg.pInCfg = (void *)&stHalCvbsParam;

        _DrvDispIfTransDeviceCvbsParamToHal(pstCvbsInfo, &stHalCvbsParam);

        bRet = _DrvDispIfExecuteQuery(pDevCtx, &stQueryCfg);
    }
    return bRet;
}

bool DrvDispIfDeviceSetHdmiParam(void *pDevCtx, MHAL_DISP_HdmiParam_t *pstHdmiInfo)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;
    HalDispHdmiParam_t stHalHdmiParam;

    if(DrvDispCtxSetCurCtx((DrvDispCtxConfig_t *)pDevCtx, ((DrvDispCtxConfig_t *)pDevCtx)->u32Idx) == FALSE)
    {
        bRet = FALSE;
    }
    else
    {
        memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));

        stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_DEVICE_HDMI_PARAM;
        stQueryCfg.stInCfg.u32CfgSize = sizeof(HalDispHdmiParam_t);
        stQueryCfg.stInCfg.pInCfg = (void *)&stHalHdmiParam;

        _DrvDispIfTransDeviceHdmiParamToHal(pstHdmiInfo, &stHalHdmiParam);

        bRet = _DrvDispIfExecuteQuery(pDevCtx, &stQueryCfg);
    }
    return bRet;
}

bool DrvDispIfDeviceSetLcdParam(void *pDevCtx, MHAL_DISP_LcdParam_t *pstLcdInfo)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;
    HalDispLcdParam_t stHalLcdParam;

    if(DrvDispCtxSetCurCtx((DrvDispCtxConfig_t *)pDevCtx, ((DrvDispCtxConfig_t *)pDevCtx)->u32Idx) == FALSE)
    {
        bRet = FALSE;
    }
    else
    {
        memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));

        stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_DEVICE_LCD_PARAM;
        stQueryCfg.stInCfg.u32CfgSize = sizeof(HalDispLcdParam_t);
        stQueryCfg.stInCfg.pInCfg = (void *)&stHalLcdParam;

        _DrvDispIfTransDeviceLcdParamToHal(pstLcdInfo, &stHalLcdParam);

        bRet = _DrvDispIfExecuteQuery(pDevCtx, &stQueryCfg);
    }
    return bRet;
}

bool DrvDispIfDeviceSetGammaParam(void *pDevCtx, MHAL_DISP_GammaParam_t *pstGammaInfo)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;
    HalDispGammaParam_t stHalGammaaram;

    if(DrvDispCtxSetCurCtx((DrvDispCtxConfig_t *)pDevCtx, ((DrvDispCtxConfig_t *)pDevCtx)->u32Idx) == FALSE)
    {
        bRet = FALSE;
    }
    else
    {
        memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));

        stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_DEVICE_GAMMA_PARAM;
        stQueryCfg.stInCfg.u32CfgSize = sizeof(HalDispGammaParam_t);
        stQueryCfg.stInCfg.pInCfg = (void *)&stHalGammaaram;

        _DrvDispIfTransdeviceGammaParamToHal(pstGammaInfo, &stHalGammaaram);

        bRet = _DrvDispIfExecuteQuery(pDevCtx, &stQueryCfg);
    }
    return bRet;
}

bool DrvDispIfDeviceSetVgaParam(void *pDevCtx, MHAL_DISP_VgaParam_t *pstVgaInfo)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;
    HalDispVgaParam_t stHalVgaParam;

    if(DrvDispCtxSetCurCtx((DrvDispCtxConfig_t *)pDevCtx, ((DrvDispCtxConfig_t *)pDevCtx)->u32Idx) == FALSE)
    {
        bRet = FALSE;
    }
    else
    {
        memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));

        stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_DEVICE_VGA_PARAM;
        stQueryCfg.stInCfg.u32CfgSize = sizeof(HalDispVgaParam_t);
        stQueryCfg.stInCfg.pInCfg = (void *)&stHalVgaParam;

        _DrvDispIfTransDeviceVgaParamToHal(pstVgaInfo, &stHalVgaParam);

        bRet = _DrvDispIfExecuteQuery(pDevCtx, &stQueryCfg);
    }
    return bRet;
}


bool DrvDispIfDeviceAttach(void *pSrcDevCtx, void *pDstDevCtx)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;


    if(DrvDispCtxSetCurCtx((DrvDispCtxConfig_t *)pSrcDevCtx, ((DrvDispCtxConfig_t *)pSrcDevCtx)->u32Idx) == FALSE ||
       DrvDispCtxSetCurCtx((DrvDispCtxConfig_t *)pDstDevCtx, ((DrvDispCtxConfig_t *)pDstDevCtx)->u32Idx) == FALSE)
    {
        bRet = FALSE;
    }
    else
    {
        memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));

        stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_DEVICE_ATTACH;
        stQueryCfg.stInCfg.u32CfgSize = sizeof(pDstDevCtx);
        stQueryCfg.stInCfg.pInCfg = pDstDevCtx;

        bRet = _DrvDispIfExecuteQuery(pSrcDevCtx, &stQueryCfg);
    }
    return bRet;
}

bool DrvDispIfDeviceDetach(void *pSrcDevCtx, void *pDstDevCtx)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;

    if(DrvDispCtxSetCurCtx((DrvDispCtxConfig_t *)pSrcDevCtx, ((DrvDispCtxConfig_t *)pSrcDevCtx)->u32Idx) == FALSE ||
       DrvDispCtxSetCurCtx((DrvDispCtxConfig_t *)pDstDevCtx, ((DrvDispCtxConfig_t *)pDstDevCtx)->u32Idx) == FALSE)
    {
        bRet = FALSE;
    }
    else
    {
        memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));

        stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_DEVICE_DETACH;
        stQueryCfg.stInCfg.u32CfgSize = sizeof(pDstDevCtx);
        stQueryCfg.stInCfg.pInCfg = pDstDevCtx;

        bRet = _DrvDispIfExecuteQuery(pSrcDevCtx, &stQueryCfg);
    }
    return bRet;
}

bool DrvDispIfDeviceGetTimeZone(void *pDevCtx, MHAL_DISP_TimeZone_t *pstTimeZone)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;
    HalDispTimeZone_t stHalTimeZone;

    if(DrvDispCtxSetCurCtx((DrvDispCtxConfig_t *)pDevCtx, ((DrvDispCtxConfig_t *)pDevCtx)->u32Idx) == FALSE)
    {
        bRet = FALSE;
    }
    else
    {
        memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));

        stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_DEVICE_TIME_ZONE;
        stQueryCfg.stInCfg.u32CfgSize = sizeof(HalDispTimeZone_t);
        stQueryCfg.stInCfg.pInCfg = &stHalTimeZone;

        bRet = _DrvDispIfExecuteQuery(pDevCtx, &stQueryCfg);

        if(bRet)
        {
            _DrvDispIfTransDeviceTimeZoneToMhal(pstTimeZone, &stHalTimeZone);
        }
        else
        {
            pstTimeZone->enType = E_MHAL_DISP_TIMEZONE_UNKONWN;
        }
    }

    return bRet;
}

bool DrvDispIfDeviceSetTimeZoneConfig(void *pDevCtx, MHAL_DISP_TimeZoneConfig_t *pstTimeZoneCfg)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;
    HalDispTimeZoneConfig_t stTimeZoneCfg;

    if(DrvDispCtxSetCurCtx((DrvDispCtxConfig_t *)pDevCtx, ((DrvDispCtxConfig_t *)pDevCtx)->u32Idx) == FALSE)
    {
        bRet = FALSE;
    }
    else
    {
        memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));

        stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_DEVICE_TIME_ZONE_CFG;
        stQueryCfg.stInCfg.u32CfgSize = sizeof(HalDispTimeZoneConfig_t);
        stQueryCfg.stInCfg.pInCfg = (void *)&stTimeZoneCfg;

        _DrvDispIfTransDeviceTimeZoneCfgToHal(pstTimeZoneCfg, &stTimeZoneCfg);

        bRet = _DrvDispIfExecuteQuery(pDevCtx, &stQueryCfg);
    }
    return bRet;
}

bool DrvDispIfDeviceGetDisplayInfo(void *pDevCtx, MHAL_DISP_DisplayInfo_t *pstDisplayInfo)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;
    HalDispDisplayInfo_t stHalDisplayInfo;

    memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));

    stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_DEVICE_DISPLAY_INFO;
    stQueryCfg.stInCfg.u32CfgSize = sizeof(HalDispDisplayInfo_t);
    stQueryCfg.stInCfg.pInCfg = &stHalDisplayInfo;

    bRet = _DrvDispIfExecuteQuery(pDevCtx, &stQueryCfg);

    if(bRet)
    {
        _DrvDispIfTransDeviceDisplayInfoToMhal(pstDisplayInfo, &stHalDisplayInfo);
    }
    else
    {
        memset(pstDisplayInfo, 0, sizeof(MHAL_DISP_DisplayInfo_t));
    }

    return bRet;
}

bool DrvDispIfDeviceGetInstance(u32 u32DeviceId, void **pDevCtx)
{
    bool bRet = TRUE;
    DrvDispCtxConfig_t *pstDispCtx = NULL;

    if(DrvDispCtxGetCurCtx(E_DISP_CTX_TYPE_DEVICE, u32DeviceId, &pstDispCtx))
    {
        *pDevCtx = (void *)pstDispCtx;
    }
    else
    {
        bRet = FALSE;
        *pDevCtx = NULL;
    }
    return bRet;
}

bool DrvDispIfDeviceSetPqConfig(void *pDevCtx, MHAL_DISP_PqConfig_t *pstPqCfg)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;
    HalDispPqConfig_t stPqCfg;

    if(DrvDispCtxSetCurCtx((DrvDispCtxConfig_t *)pDevCtx, ((DrvDispCtxConfig_t *)pDevCtx)->u32Idx) == FALSE)
    {
        bRet = FALSE;
    }
    else
    {
        memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));

        stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_PQ_SET;
        stQueryCfg.stInCfg.u32CfgSize = sizeof(HalDispPqConfig_t);
        stQueryCfg.stInCfg.pInCfg = (void *)&stPqCfg;

        _DrvDispIfTransPqCfgToHal(pstPqCfg, &stPqCfg);

        bRet = _DrvDispIfExecuteQuery(pDevCtx, &stQueryCfg);
    }
    return bRet;
}

bool DrvDispIfDeviceGetHwCount(u32 *pu32Count)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;
    HalDispHwInfoConfig_t stHalHwInfoCfg;
    DrvDispCtxConfig_t stDispCtxCfg;

    HalDispIfInit();

    memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));
    stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_HW_INFO;
    stQueryCfg.stInCfg.u32CfgSize = sizeof(HalDispHwInfoConfig_t);
    stQueryCfg.stInCfg.pInCfg = &stHalHwInfoCfg;


    memset(&stHalHwInfoCfg, 0, sizeof(HalDispHwInfoConfig_t));
    stHalHwInfoCfg.eType = E_HAL_DISP_HW_INFO_DEVICE;

    if( _DrvDispIfExecuteQuery(&stDispCtxCfg, &stQueryCfg) )
    {
        *pu32Count = stHalHwInfoCfg.u32Count;
        bRet = 1;
    }
    else
    {
        *pu32Count = 0;
        bRet = 0;
    }
    return bRet;
}

bool DrvDispIfSetDeviceConfig(u32 u32DevId, MHAL_DISP_DeviceConfig_t *pstDevCfg)
{
    HalDispQueryConfig_t stQueryCfg;
    HalDispInternalConfig_t stInterCfg;
    DrvDispCtxConfig_t stDispCtxCfg;
    bool bRet;

    HalDispIfInit();

    memset(&stInterCfg, 0,sizeof(HalDispInternalConfig_t));
    memset(&stQueryCfg, 0,sizeof(HalDispQueryConfig_t));

    stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_INTERCFG_SET;
    stQueryCfg.stInCfg.u32CfgSize = sizeof(HalDispInternalConfig_t);
    stQueryCfg.stInCfg.pInCfg = (void *)&stInterCfg;

    stDispCtxCfg.u32Idx = u32DevId;
    stDispCtxCfg.enCtxType = E_DISP_CTX_TYPE_DEVICE;
    stDispCtxCfg.pstCtxContain = NULL;

    _DrvDispIfTransDeviceConfigToHal(pstDevCfg, &stInterCfg);

    bRet = _DrvDispIfExecuteQuery(&stDispCtxCfg, &stQueryCfg);
    return bRet;
}

bool DrvDispIfGetDeviceConfig(u32 u32DevId, MHAL_DISP_DeviceConfig_t *pstDevCfg)
{
    HalDispQueryConfig_t stQueryCfg;
    HalDispInternalConfig_t stInterCfg;
    DrvDispCtxConfig_t stDispCtxCfg;
    bool bRet = 1;

    HalDispIfInit();

    memset(&stInterCfg, 0,sizeof(HalDispInternalConfig_t));
    memset(&stQueryCfg, 0,sizeof(HalDispQueryConfig_t));

    stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_INTERCFG_GET;
    stQueryCfg.stInCfg.u32CfgSize = sizeof(HalDispInternalConfig_t);
    stQueryCfg.stInCfg.pInCfg = (void *)&stInterCfg;

    stDispCtxCfg.u32Idx = u32DevId;
    stDispCtxCfg.enCtxType = E_DISP_CTX_TYPE_DEVICE;
    stDispCtxCfg.pstCtxContain = NULL;

    if(_DrvDispIfExecuteQuery(&stDispCtxCfg, &stQueryCfg))
    {
        _DrvDispIfTransDeviceConfigToMHal(pstDevCfg, &stInterCfg);
    }
    else
    {
        bRet = 0;
        DISP_ERR("%s %d Get Config Fail\n", __FUNCTION__, __LINE__);
    }
    return bRet;
}


// VideoLayer
bool DrvDispIfVideoLayerCreateInstance(MHAL_DISP_AllocPhyMem_t *pstAlloc, u32 u32LayerId, void **pVidLayerCtx)
{
    bool bRet = TRUE;
    DrvDispCtxAllocConfig_t stCtxAllocCfg;
    HalDispQueryConfig_t stQueryCfg;
    DrvDispCtxConfig_t *pstDispCtx = NULL;

    DrvDispCtxInit();

    stCtxAllocCfg.enType = E_DISP_CTX_TYPE_VIDLAYER;
    stCtxAllocCfg.u32Id = u32LayerId;
    stCtxAllocCfg.pstBindCtx = NULL;
    stCtxAllocCfg.stMemAllcCfg.alloc = NULL;
    stCtxAllocCfg.stMemAllcCfg.free = NULL;

    if(DrvDispCtxAllocate(&stCtxAllocCfg, &pstDispCtx) == FALSE)
    {
        bRet = 0;
        *pVidLayerCtx = NULL;
        DISP_ERR("%s %d, CreateInstance Fail\n", __FUNCTION__, __LINE__);
    }
    else
    {
        *pVidLayerCtx = (void *)pstDispCtx;

        memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));
        stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_VIDEOLAYER_INIT;
        stQueryCfg.stInCfg.pInCfg = NULL;
        stQueryCfg.stInCfg.u32CfgSize = 0;
        bRet = _DrvDispIfExecuteQuery(pstDispCtx, &stQueryCfg);
    }

    return bRet;
}

bool DrvDispIfVideoLayerDestoryInstance(void *pVidLayerCtx)
{
    bool bRet = TRUE;
    DrvDispCtxConfig_t *pstDispCtx = (DrvDispCtxConfig_t *) pVidLayerCtx;

    if(DrvDispCtxFree(pstDispCtx))
    {
        if(DrvDispCtxIsAllFree())
        {
            DrvDispCtxDeInit();
        }
    }
    else
    {
        bRet = 0;
        DISP_ERR("%s %d, DestroyInstance Fail\n", __FUNCTION__, __LINE__);
    }

    return bRet;
}


bool DrvDispIfVideoLayerEnable(void *pVidLayerCtx,  bool bEnable)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;

    memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));

    stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_VIDEOLAYER_ENABLE;
    stQueryCfg.stInCfg.u32CfgSize = sizeof(bool);
    stQueryCfg.stInCfg.pInCfg = &bEnable;

    bRet = _DrvDispIfExecuteQuery(pVidLayerCtx, &stQueryCfg);

    return bRet;
}

bool DrvDispIfVideoLayerBind(void *pVidLayerCtx, void *pDevCtx)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;

    memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));

    stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_VIDEOLAYER_BIND;
    stQueryCfg.stInCfg.u32CfgSize = sizeof(pDevCtx);
    stQueryCfg.stInCfg.pInCfg = pDevCtx;

    bRet = _DrvDispIfExecuteQuery(pVidLayerCtx, &stQueryCfg);

    return bRet;
}

bool DrvDispIfVideoLayerUnBind(void *pVidLayerCtx, void *pDevCtx)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;

    memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));

    stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_VIDEOLAYER_UNBIND;
    stQueryCfg.stInCfg.u32CfgSize = sizeof(pDevCtx);
    stQueryCfg.stInCfg.pInCfg = pDevCtx;

    bRet = _DrvDispIfExecuteQuery(pVidLayerCtx, &stQueryCfg);

    return bRet;
}

bool DrvDispIfVideoLayerSetAttr(void *pVidLayerCtx,  MHAL_DISP_VideoLayerAttr_t *pstAttr)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;
    HalDispVideoLayerAttr_t stHalAttrCfg;

    memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));

    stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_VIDEOLAYER_ATTR;
    stQueryCfg.stInCfg.u32CfgSize = sizeof(HalDispVideoLayerAttr_t);
    stQueryCfg.stInCfg.pInCfg = &stHalAttrCfg;

    _DrvDispIfTransVidLayerAttrToHal(pstAttr, &stHalAttrCfg);

    bRet = _DrvDispIfExecuteQuery(pVidLayerCtx, &stQueryCfg);

    return bRet;
}

bool DrvDispIfVideoLayerBufferFire(void *pVidLayerCtx)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;

    memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));

    stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_VIDEOLAYER_BUFFER_FIRE;
    stQueryCfg.stInCfg.u32CfgSize = 0;
    stQueryCfg.stInCfg.pInCfg = NULL;

    bRet = _DrvDispIfExecuteQuery(pVidLayerCtx, &stQueryCfg);

    return bRet;
}

bool DrvDispIfVideoLayerCheckBufferFired(void *pVidLayerCtx)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;

    memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));

    stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_VIDEOLAYER_CHECK_FIRE;
    stQueryCfg.stInCfg.u32CfgSize = 0;
    stQueryCfg.stInCfg.pInCfg = NULL;

    bRet = _DrvDispIfExecuteQuery(pVidLayerCtx, &stQueryCfg);

    return bRet;
}

bool DrvDispIfVideoLayerSetCompress(void *pVidLayerCtx, MHAL_DISP_CompressAttr_t* pstCompressAttr)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;
    HalDispVideoLayerCompressAttr_t stHalCompressCfg;

    memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));

    stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_VIDEOLAYER_COMPRESS;
    stQueryCfg.stInCfg.u32CfgSize = sizeof(HalDispVideoLayerCompressAttr_t);
    stQueryCfg.stInCfg.pInCfg = &stHalCompressCfg;

    _DrvDispIfTransVidLayerCompressToHal(pstCompressAttr, &stHalCompressCfg);
    bRet = _DrvDispIfExecuteQuery(pVidLayerCtx, &stQueryCfg);

    return bRet;
}

bool DrvDispIfVideoLayerSetPriority(void *pVidLayerCtx, u32 u32Priority)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;

    memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));

    stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_VIDEOLAYER_PRIORITY;
    stQueryCfg.stInCfg.u32CfgSize = sizeof(u32);
    stQueryCfg.stInCfg.pInCfg = &u32Priority;

    bRet = _DrvDispIfExecuteQuery(pVidLayerCtx, &stQueryCfg);

    return bRet;
}

bool DrvDispIfVideoLayerGetHwCount(u32 *pu32Count)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;
    HalDispHwInfoConfig_t stHalHwInfoCfg;
    DrvDispCtxConfig_t stDispCtxCfg;

    HalDispIfInit();

    memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));
    stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_HW_INFO;
    stQueryCfg.stInCfg.u32CfgSize = sizeof(HalDispHwInfoConfig_t);
    stQueryCfg.stInCfg.pInCfg = &stHalHwInfoCfg;

    memset(&stHalHwInfoCfg, 0, sizeof(HalDispHwInfoConfig_t));
    stHalHwInfoCfg.eType = E_HAL_DISP_HW_INFO_VIDEOLAYER;

    if( _DrvDispIfExecuteQuery(&stDispCtxCfg, &stQueryCfg) )
    {
        *pu32Count = stHalHwInfoCfg.u32Count;
        bRet = 1;
    }
    else
    {
        *pu32Count = 0;
        bRet = 0;
    }
    return bRet;
}

// InputPort
bool DrvDispIfInputPortCreateInstance(MHAL_DISP_AllocPhyMem_t *pstAlloc, void *pVidLayerCtx, u32 u32PortId, void **pCtx)
{
    bool bRet = TRUE;
    DrvDispCtxAllocConfig_t stCtxAllocCfg;
    HalDispQueryConfig_t stQueryCfg;
    DrvDispCtxConfig_t *pstDispCtx = NULL;

    DrvDispCtxInit();

    stCtxAllocCfg.enType = E_DISP_CTX_TYPE_INPUTPORT;
    stCtxAllocCfg.u32Id = u32PortId;
    stCtxAllocCfg.pstBindCtx = (DrvDispCtxConfig_t *)pVidLayerCtx;
    stCtxAllocCfg.stMemAllcCfg.alloc = NULL;
    stCtxAllocCfg.stMemAllcCfg.free = NULL;

    if(DrvDispCtxAllocate(&stCtxAllocCfg, &pstDispCtx) == FALSE)
    {
        bRet = 0;
        *pCtx = NULL;
        DISP_ERR("%s %d, CreateInstance Fail\n", __FUNCTION__, __LINE__);
    }
    else
    {
        *pCtx = (void *)pstDispCtx;

         memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));
         stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_INPUTPORT_INIT;
         stQueryCfg.stInCfg.pInCfg = NULL;
         stQueryCfg.stInCfg.u32CfgSize = 0;
         bRet = _DrvDispIfExecuteQuery(pstDispCtx, &stQueryCfg);
    }

    return bRet;
}


bool DrvDispIfInputPortDestroyInstance(void *pInputPortCtx)
{
    bool bRet = TRUE;
    DrvDispCtxConfig_t *pstDispCtx = (DrvDispCtxConfig_t *) pInputPortCtx;

    if(DrvDispCtxFree(pstDispCtx))
    {
        if(DrvDispCtxIsAllFree())
        {
            DrvDispCtxDeInit();
        }
    }
    else
    {
        bRet = 0;
        DISP_ERR("%s %d, DestroyInstance Fail\n", __FUNCTION__, __LINE__);
    }

    return bRet;
}

bool DrvDispIfInputPortRotate(void *pInputPortCtx, MHAL_DISP_RotateConfig_t *pstRotateCfg)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;
    HalDispInputPortRotate_t stRotate;

    memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));

    stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_INPUTPORT_ROTATE;
    stQueryCfg.stInCfg.u32CfgSize = sizeof(HalDispInputPortRotate_t);
    stQueryCfg.stInCfg.pInCfg = &stRotate;

    _DrvDispIfTransInputPortRotateToHal(pstRotateCfg, &stRotate);
    bRet = _DrvDispIfExecuteQuery(pInputPortCtx, &stQueryCfg);

    return bRet;
}

bool DrvDispIfInputPortSetCropAttr(void *pInputPortCtx, MHAL_DISP_VidWinRect_t *pstCropAttr)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;
    HalDispVidWinRect_t stHalCropAttr;

    memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));

    stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_INPUTPORT_CROP;
    stQueryCfg.stInCfg.u32CfgSize = sizeof(HalDispVidWinRect_t);
    stQueryCfg.stInCfg.pInCfg = &stHalCropAttr;

    _DrvDispIfTransInputPortCropAttrToHal(pstCropAttr, &stHalCropAttr);
    bRet = _DrvDispIfExecuteQuery(pInputPortCtx, &stQueryCfg);

    return bRet;
}
bool DrvDispIfInputPortFlip(void *pInputPortCtx, MHAL_DISP_VideoFrameData_t *pstVideoFrameData)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;
    HalDispVideoFrameData_t stHalFrameData;

    memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));

    stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_INPUTPORT_FLIP;
    stQueryCfg.stInCfg.u32CfgSize = sizeof(HalDispVideoFrameData_t);
    stQueryCfg.stInCfg.pInCfg = &stHalFrameData;

    _DrvDispIfTransInputPortFrameDataToHal(pstVideoFrameData, &stHalFrameData);
    bRet = _DrvDispIfExecuteQuery(pInputPortCtx, &stQueryCfg);

    return bRet;
}

bool DrvDispIfInputPortSetRingBuffAttr(void *pInputPortCtx, MHAL_DISP_RingBufferAttr_t *pstRingBufAttr)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;
    HalDispRingBuffAttr_t stRingBuffAttr;

    memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));

    stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_INPUTPORT_RING_BUFF_ATTR;
    stQueryCfg.stInCfg.u32CfgSize = sizeof(HalDispRingBuffAttr_t);
    stQueryCfg.stInCfg.pInCfg = &stRingBuffAttr;

    _DrvDispIfTransInputPortRingBufAttrToHal(pstRingBufAttr, &stRingBuffAttr);
    bRet = _DrvDispIfExecuteQuery(pInputPortCtx, &stQueryCfg);

    return bRet;

}

bool DrvDispIfInputPortEnable(void *pInputPortCtx, bool bEnable)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;

    memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));

    stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_INPUTPORT_ENABLE;
    stQueryCfg.stInCfg.u32CfgSize = sizeof(bool);
    stQueryCfg.stInCfg.pInCfg = &bEnable;

    bRet = _DrvDispIfExecuteQuery(pInputPortCtx, &stQueryCfg);

    return bRet;
}


bool DrvDispIfInputPortSetAttr(void *pInputPortCtx, MHAL_DISP_InputPortAttr_t *pstAttr)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;
    HalDispInputPortAttr_t stHalAttr;

    memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));

    stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_INPUTPORT_ATTR;
    stQueryCfg.stInCfg.u32CfgSize = sizeof(HalDispInputPortAttr_t);
    stQueryCfg.stInCfg.pInCfg = &stHalAttr;

    _DrvDispIfTransInputPortAttrToHal(pstAttr, &stHalAttr);
    bRet = _DrvDispIfExecuteQuery(pInputPortCtx, &stQueryCfg);

    return bRet;
}

bool DrvDispIfInputPortShow(void *pInputPortCtx)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;

    memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));

    stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_INPUTPORT_SHOW;
    stQueryCfg.stInCfg.u32CfgSize = 0;
    stQueryCfg.stInCfg.pInCfg = NULL;

    bRet = _DrvDispIfExecuteQuery(pInputPortCtx, &stQueryCfg);

    return bRet;
}

bool DrvDispIfInputPortHide(void *pInputPortCtx)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;

    memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));

    stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_INPUTPORT_HIDE;
    stQueryCfg.stInCfg.u32CfgSize = 0;
    stQueryCfg.stInCfg.pInCfg = NULL;

    bRet = _DrvDispIfExecuteQuery(pInputPortCtx, &stQueryCfg);

    return bRet;
}

bool DrvDispIfInputPortAttrBegin(void *pVidLayerCtx)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;

    memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));

    stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_INPUTPORT_BEGIN;
    stQueryCfg.stInCfg.u32CfgSize = 0;
    stQueryCfg.stInCfg.pInCfg = NULL;

    bRet = _DrvDispIfExecuteQuery(pVidLayerCtx, &stQueryCfg);

    return bRet;
}

bool DrvDispIfInputPortAttrEnd(void *pVidLayerCtx)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;

    memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));

    stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_INPUTPORT_END;
    stQueryCfg.stInCfg.u32CfgSize = 0;
    stQueryCfg.stInCfg.pInCfg = NULL;

    bRet = _DrvDispIfExecuteQuery(pVidLayerCtx, &stQueryCfg);

    return bRet;
}

bool DrvDispIfInputPortGetHwCount(u32 u32VidLayerId, u32 *pu32Count)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;
    HalDispHwInfoConfig_t stHalHwInfoCfg;
    DrvDispCtxConfig_t stDispCtxCfg;

    HalDispIfInit();

    memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));
    stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_HW_INFO;
    stQueryCfg.stInCfg.u32CfgSize = sizeof(HalDispHwInfoConfig_t);
    stQueryCfg.stInCfg.pInCfg = &stHalHwInfoCfg;

    memset(&stHalHwInfoCfg, 0, sizeof(HalDispHwInfoConfig_t));
    stHalHwInfoCfg.eType = E_HAL_DISP_HW_INFO_INPUTPORT;
    stHalHwInfoCfg.u32Id = u32VidLayerId;

    if( _DrvDispIfExecuteQuery(&stDispCtxCfg, &stQueryCfg) )
    {
        *pu32Count = stHalHwInfoCfg.u32Count;
        bRet = 1;
    }
    else
    {
        *pu32Count = 0;
        bRet = 0;
    }
    return bRet;
}

bool DrvDispIfInputPortSetImiAddr(void *pInputPortCtx, u32 u32ImiAddr)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;

    memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));

    stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_INPUTPORT_IMIADDR;
    stQueryCfg.stInCfg.u32CfgSize = sizeof(u32);
    stQueryCfg.stInCfg.pInCfg = &u32ImiAddr;

    bRet = _DrvDispIfExecuteQuery(pInputPortCtx, &stQueryCfg);

    return bRet;
}

bool DrvDispIfSetDbgLevel(void *p)
{
    _gu32DispDbgLevel = *((u32 *)p);
    return TRUE;
}

bool DrvDispIfDmaCreateInstance(MHAL_DISP_AllocPhyMem_t *pstAlloc, u32 u32DmaId, void **pDmaCtx)
{
    bool bRet = TRUE;
    DrvDispCtxAllocConfig_t stCtxAllocCfg;
    HalDispQueryConfig_t stQueryCfg;
    DrvDispCtxConfig_t *pstDispCtx = NULL;

    DrvDispCtxInit();

    stCtxAllocCfg.enType = E_DISP_CTX_TYPE_DMA;
    stCtxAllocCfg.u32Id = u32DmaId;
    stCtxAllocCfg.pstBindCtx = NULL;
    stCtxAllocCfg.stMemAllcCfg.alloc = NULL;
    stCtxAllocCfg.stMemAllcCfg.free = NULL;

    if(DrvDispCtxAllocate(&stCtxAllocCfg, &pstDispCtx) == FALSE)
    {
        bRet = 0;
        *pDmaCtx = NULL;
        DISP_ERR("%s %d, CreateInstance Fail\n", __FUNCTION__, __LINE__);
    }
    else
    {
        *pDmaCtx = (void *)pstDispCtx;

        memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));
        stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_DMA_INIT;
        stQueryCfg.stInCfg.pInCfg = NULL;
        stQueryCfg.stInCfg.u32CfgSize = 0;
        bRet = _DrvDispIfExecuteQuery((void *)pstDispCtx, &stQueryCfg);
    }

    return bRet;
}

bool DrvDispIfDmaDestoryInstance(void *pDmaCtx)
{
    bool bRet = TRUE;
    DrvDispCtxConfig_t *pstDispCtx = (DrvDispCtxConfig_t *) pDmaCtx;
    HalDispQueryConfig_t stQueryCfg;

    memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));
    stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_DMA_DEINIT;
    stQueryCfg.stInCfg.pInCfg = NULL;
    stQueryCfg.stInCfg.u32CfgSize = 0;


    if(_DrvDispIfExecuteQuery(pstDispCtx, &stQueryCfg))
    {
        if(DrvDispCtxFree(pstDispCtx))
        {
            if(DrvDispCtxIsAllFree())
            {
                DrvDispCtxDeInit();
            }
        }
        else
        {
            bRet = 0;
            DISP_ERR("%s %d, DestroyInstance Fail\n", __FUNCTION__, __LINE__);
        }
    }
    else
    {
        bRet = 0;
        DISP_ERR("%s %d, Dma DeInit Fail\n", __FUNCTION__, __LINE__);
    }
    return bRet;
}

bool DrvDispIfDmaBind(void *pDmaCtx, MHAL_DISP_DmaBindConfig_t *pstDmaBindCfg)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;
    HalDispDmaBindConfig_t stDmaBindCfg;

    memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));

    stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_DMA_BIND;
    stQueryCfg.stInCfg.u32CfgSize = sizeof(HalDispDmaBindConfig_t);
    stQueryCfg.stInCfg.pInCfg = &stDmaBindCfg;

    _DrvDispIfTransDmaBindToHal(pstDmaBindCfg, &stDmaBindCfg);
    bRet = _DrvDispIfExecuteQuery(pDmaCtx, &stQueryCfg);

    return bRet;
}

bool DrvDispIfDmaUnBind(void *pDmaCtx, MHAL_DISP_DmaBindConfig_t *pstDmaBindCfg)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;
    HalDispDmaBindConfig_t stDmaBindCfg;

    memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));

    stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_DMA_UNBIND;
    stQueryCfg.stInCfg.u32CfgSize = sizeof(HalDispDmaBindConfig_t);
    stQueryCfg.stInCfg.pInCfg = &stDmaBindCfg;

    _DrvDispIfTransDmaBindToHal(pstDmaBindCfg, &stDmaBindCfg);
    bRet = _DrvDispIfExecuteQuery(pDmaCtx, &stQueryCfg);

    return bRet;
}

bool DrvDispIfDmaSetAttr(void *pDmaCtx, MHAL_DISP_DmaAttrConfig_t *pstDmaAttrCfg)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;
    HalDispDmaAttrConfig_t stHalDmaAttrCfg;

    if(DrvDispCtxSetCurCtx((DrvDispCtxConfig_t *)pDmaCtx, ((DrvDispCtxConfig_t *)pDmaCtx)->u32Idx) == FALSE)
    {
        bRet = FALSE;
    }
    else
    {
        memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));

        stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_DMA_ATTR;
        stQueryCfg.stInCfg.u32CfgSize = sizeof(MHAL_DISP_DmaAttrConfig_t);
        stQueryCfg.stInCfg.pInCfg = (void *)&stHalDmaAttrCfg;

        _DrvDispIfTransDmaAttrToHal(pstDmaAttrCfg, &stHalDmaAttrCfg);

        bRet = _DrvDispIfExecuteQuery(pDmaCtx, &stQueryCfg);
    }
    return bRet;
}

bool DrvDispIfDmaSetBufferAttr(void *pDmaCtx, MHAL_DISP_DmaBufferAttrConfig_t *pstDmaBufferAttrCfg)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;
    HalDispDmaBufferAttrConfig_t stHalDmaBufferAttrCfg;

    if(DrvDispCtxSetCurCtx((DrvDispCtxConfig_t *)pDmaCtx, ((DrvDispCtxConfig_t *)pDmaCtx)->u32Idx) == FALSE)
    {
        bRet = FALSE;
    }
    else
    {
        memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));

        stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_DMA_BUFFERATTR;
        stQueryCfg.stInCfg.u32CfgSize = sizeof(MHAL_DISP_DmaBufferAttrConfig_t);
        stQueryCfg.stInCfg.pInCfg = (void *)&stHalDmaBufferAttrCfg;

        _DrvDispIfTransDmaBufferAttrToHal(pstDmaBufferAttrCfg, &stHalDmaBufferAttrCfg);

        bRet = _DrvDispIfExecuteQuery(pDmaCtx, &stQueryCfg);
    }
    return bRet;
}


bool DrvDispIfClkOn(void)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;
    HalDispClkInitConfig_t stHalClkInitCfg;
    DrvDispCtxConfig_t stDispCtxCfg;

    HalDispIfInit();

    memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));
    stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_CLK_INIT;
    stQueryCfg.stInCfg.u32CfgSize = sizeof(HalDispClkInitConfig_t);
    stQueryCfg.stInCfg.pInCfg = &stHalClkInitCfg;

    stHalClkInitCfg.bEn = 1;

    bRet = _DrvDispIfExecuteQuery(&stDispCtxCfg, &stQueryCfg);
    return bRet;
}

bool DrvDispIfClkOff(void)
{
    bool bRet = 1;
    HalDispQueryConfig_t stQueryCfg;
    HalDispClkInitConfig_t stHalClkInitCfg;
    DrvDispCtxConfig_t stDispCtxCfg;

    HalDispIfInit();

    memset(&stQueryCfg, 0, sizeof(HalDispQueryConfig_t));
    stQueryCfg.stInCfg.enQueryType = E_HAL_DISP_QUERY_CLK_INIT;
    stQueryCfg.stInCfg.u32CfgSize = sizeof(HalDispClkInitConfig_t);
    stQueryCfg.stInCfg.pInCfg = &stHalClkInitCfg;

    stHalClkInitCfg.bEn = 0;

    bRet = _DrvDispIfExecuteQuery(&stDispCtxCfg, &stQueryCfg);
    return bRet;
}

//-------------------------------------------------------------------------------------------------
