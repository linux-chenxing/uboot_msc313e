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

#ifndef _DRV_DISP_CTX_H_
#define _DRV_DISP_CTX_H_


//-------------------------------------------------------------------------------------------------
//  Defines & Macro
//-------------------------------------------------------------------------------------------------
#define DRV_CTX_INVAILD_IDX     0xFFFF
//-------------------------------------------------------------------------------------------------
//  Enum
//-------------------------------------------------------------------------------------------------
typedef enum
{
    E_DISP_CTX_TYPE_DEVICE,
    E_DISP_CTX_TYPE_VIDLAYER,
    E_DISP_CTX_TYPE_INPUTPORT,
    E_DISP_CTX_TYPE_DMA,
    E_DISP_CTX_TYPE_MAX,
}DrvDispCtxType_e;

//-------------------------------------------------------------------------------------------------
//  structure
//-------------------------------------------------------------------------------------------------
typedef s32 (*pDispCtxMemAlloc)(u8 *pu8Name, u32 size, u64 *pu64PhyAddr);
typedef s32 (*pDispCtxMemFree)(u64 u64PhyAddr);

typedef struct
{
    pDispCtxMemAlloc alloc;
    // Success return 0
    pDispCtxMemFree free;
}DrvDispCtxMemAllocConfig_t;

typedef struct
{
    bool bUsed;
    u32 u32PortId;
    bool bEnable;
    bool bDisplay;
    bool bBeginEnd;
    HalDispVideoFrameData_t stFrameData;
    HalDispInputPortAttr_t stAttr;
    HalDispVidWinRect_t stCrop;
    HalDispInputPortRotate_t stRot;
    HalDispRingBuffAttr_t stRingBuffAttr;
    void *pstVidLayerContain;
    u16 u16FlipFrontPorchCnt;
}DrvDispCtxInputPortContain_t;

typedef struct
{
    u32 u32VidLayerId;
    void *pstDevCtx;
    bool bEnable;
    HalDispVideoLayerAttr_t stAttr;
    HalDispVideoLayerCompressAttr_t stCompress;
    u32 u32Priority;
    DrvDispCtxInputPortContain_t *pstInputPortContain[HAL_DISP_INPUTPORT_NUM];
}DrvDispCtxVideoLayerContain_t;

typedef struct
{
    u32 u32DmaId;
    HalDispDmaInputConfig_t stInputCfg;
    HalDispDmaOutputConfig_t stOutputCfg;
    HalDispDmaBufferAttrConfig_t stBufferAttrCfg;
    void *pSrcDevContain;
    void *pDestDevContain;
}DrvDispCtxDmaContain_t;

typedef struct
{
    u32  u32DevId;
    bool bEnable;
    u32  u32BgColor;
    u32  u32BindVideoLayerNum;
    u32  u32Interface;
    HalDispDeviceTiming_e eTimeType;
    HalDispDeviceTimingConfig_t stDevTimingCfg;
    HalDispCvbsParam_t stCvbsParam;
    HalDispHdmiParam_t stHdmiParam;
    HalDispVgaParam_t  stVgaParam;
    HalDispLcdParam_t  stLcdParam;
    HalDispGammaParam_t stGammaParam;
    HalDispColorTemp_t stColorTemp;
    HalDispTimeZoneConfig_t stTimeZoeCfg;
    DrvDispCtxVideoLayerContain_t *pstVidLayeCtx[HAL_DISP_VIDLAYER_MAX];
    DrvDispCtxDmaContain_t *pstDmaContain[HAL_DISP_DMA_MAX];
    void *pstDevAttachSrc;
}DrvDispCtxDeviceContain_t;


typedef struct
{
    bool bDevUsed[HAL_DISP_DEVICE_MAX];
    bool bVidLayerUsed[HAL_DISP_VIDLAYER_MAX];
    bool bInputPortUsed[HAL_DISP_INPUTPORT_MAX];
    bool bDmaUsed[HAL_DISP_DMA_MAX];
    DrvDispCtxDeviceContain_t *pstDevContain[HAL_DISP_DEVICE_MAX];
    DrvDispCtxVideoLayerContain_t *pstVidLayerContain[HAL_DISP_VIDLAYER_MAX];
    DrvDispCtxInputPortContain_t *pstInputPortContain[HAL_DISP_INPUTPORT_MAX];
    DrvDispCtxDmaContain_t *pstDmaContain[HAL_DISP_DMA_MAX];
    DrvDispCtxMemAllocConfig_t stMemAllcCfg;
    HalDispHwContain_t *pstHalHwContain;
}DrvDispCtxContain_t;

typedef struct
{
    DrvDispCtxType_e enCtxType;
    u32 u32Idx;
    DrvDispCtxContain_t *pstCtxContain;
}DrvDispCtxConfig_t;

typedef struct
{
    DrvDispCtxType_e enType;
    u32 u32Id;
    DrvDispCtxConfig_t *pstBindCtx;
    DrvDispCtxMemAllocConfig_t stMemAllcCfg;
}DrvDispCtxAllocConfig_t;
//-------------------------------------------------------------------------------------------------
//  Prototype
//-------------------------------------------------------------------------------------------------
#ifndef _DRV_DISP_CTX_C_
#define INTERFACE extern
#else
#define INTERFACE
#endif

INTERFACE bool DrvDispCtxInit(void);
INTERFACE bool DrvDispCtxDeInit(void);
INTERFACE bool DrvDispCtxAllocate(DrvDispCtxAllocConfig_t *pAllocCfg, DrvDispCtxConfig_t **pCtx);
INTERFACE bool DrvDispCtxFree(DrvDispCtxConfig_t *pCtx);
INTERFACE bool DrvDispCtxIsAllFree(void);
INTERFACE bool DrvDispCtxSetCurCtx(DrvDispCtxConfig_t *pCtx, u32 u32Idx);
INTERFACE bool DrvDispCtxGetCurCtx(DrvDispCtxType_e enCtxType, u32 u32Idx, DrvDispCtxConfig_t **pCtx);

#undef INTERFACE

#endif
