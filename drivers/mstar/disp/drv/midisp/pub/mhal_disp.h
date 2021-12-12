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


/**
 * \defgroup HAL_DISP_group  HAL_DISP driver
 * @{
 */
#ifndef __MHAL_DISP_H__
#define __MHAL_DISP_H__

#include "mhal_disp_datatype.h"
//=============================================================================
// API
//=============================================================================

#define DISP_ENABLE_CUS_ALLOCATOR 0

#ifndef __MHAL_DISP_C__
#define INTERFACE extern
#else
#define INTERFACE
#endif

// Driver Physical memory: MI

//----------------------------------- Device -----------------------------------------------

INTERFACE MS_BOOL MHAL_DISP_GetDevIrq(void *pDevCtx, MS_U32* pu32DevIrq);
INTERFACE MS_BOOL MHAL_DISP_EnableDevIrq(void *pDevCtx, MS_U32 u32DevIrq, MS_BOOL bEnable);
INTERFACE MS_BOOL MHAL_DISP_ClearDevInterrupt(void *pDevCtx, void* pData);
INTERFACE MS_BOOL MHAL_DISP_GetDevIrqFlag(void *pDevCtx, MHAL_DISP_IRQFlag_t *pstIrqFlag);
INTERFACE MS_BOOL MHAL_DISP_GetDevIrqById(MS_U32 u32DevId, MS_U32 *pu32DevIrq);

INTERFACE MS_BOOL MHAL_DISP_InitPanelConfig(MHAL_DISP_PanelConfig_t* pstPanelConfig, MS_U8 u8Size);

INTERFACE MS_BOOL MHAL_DISP_InitMmapConfig(MHAL_DISP_MmapType_e eMemType, MHAL_DISP_MmapInfo_t* pstMmapInfo);

// Set Reg Access
INTERFACE MS_BOOL MHAL_DISP_SetRegAccessConfig(void *pDevCtx, MHAL_DISP_RegAccessConfig_t *pstRegAccessCfg);

// Set Reg Flip
INTERFACE MS_BOOL MHAL_DISP_SetRegFlipConfig(void *pDevCtx, MHAL_DISP_RegFlipConfig_t *pstRegFlipCfg);

// Set Reg WaitDone
INTERFACE MS_BOOL MHAL_DISP_SetRegWaitDoneConfig(void *pDevCtx, MHAL_DISP_RegWaitDoneConfig_t *pstRegWaitDoneCfg);

// Set Cmdq Interface
INTERFACE MS_BOOL MHAL_DISP_SetCmdqInterface(u32 u32DevId, MHAL_DISP_CmdqInfConfig_t *pstCmdqInfCfg);

// Device: Create/Distroy (scaler0/1)
INTERFACE MS_BOOL MHAL_DISP_DeviceCreateInstance(const MHAL_DISP_AllocPhyMem_t *pstAlloc, const MS_U32 u32DeviceId, void **pDevCtx);
INTERFACE MS_BOOL MHAL_DISP_DeviceDestroyInstance(void *pDevCtx);

// Device: Enbale/Disable
INTERFACE MS_BOOL MHAL_DISP_DeviceEnable(void *pDevCtx, const MS_BOOL bEnable);

// Device: Scaler0 attach/deattach Scaler1
INTERFACE MS_BOOL MHAL_DISP_DeviceAttach(const void *pSrcDevCtx, const void *pDstDevCtx);
INTERFACE MS_BOOL MHAL_DISP_DeviceDetach(const void *pSrcDevCtx, const void *pDstDevCtx);

// Device: Set/Get BackGround Color in RGB format
INTERFACE MS_BOOL MHAL_DISP_DeviceSetBackGroundColor(void *pDevCtx, const MS_U32 u32BgColor);

INTERFACE MS_BOOL MHAL_DISP_DeviceAddOutInterface(void *pDevCtx, const MS_U32 u32Interface);

// Device: Set output Timing
INTERFACE MS_BOOL MHAL_DISP_DeviceSetOutputTiming(void *pDevCtx, const MS_U32 u32Interface, const MHAL_DISP_DeviceTimingInfo_t *pstTimingInfo);

// Device: Set VGA Parameters
INTERFACE MS_BOOL MHAL_DISP_DeviceSetVgaParam(void *pDevCtx, const MHAL_DISP_VgaParam_t *pstVgaInfo);

// Device: Set HDMI Parameters
INTERFACE MS_BOOL MHAL_DISP_DeviceSetHdmiParam(void *pDevCtx, const MHAL_DISP_HdmiParam_t *pstHdmiInfo);

// Device: Set CVBS Parameters
INTERFACE MS_BOOL MHAL_DISP_DeviceSetCvbsParam(void *pDevCtx, const MHAL_DISP_CvbsParam_t *pstCvbsInfo);

// Device: Set ColorTempeture
INTERFACE MS_BOOL MHAL_DISP_DeviceSetColorTempeture(void *pDevCtx, const MHAL_DISP_ColorTempeture_t *pstColorTempInfo);

// Device: Set LCD Parameters;
INTERFACE MS_BOOL MHAL_DISP_DeviceSetLcdParam(void *pDevCtx, const MHAL_DISP_LcdParam_t *pstLcdInfo);

// Device: Set Gamma Parameters
INTERFACE MS_BOOL MHAL_DISP_DeviceSetGammaParam(void *pDevCtx, const MHAL_DISP_GammaParam_t *pstGammaInfo);

// Device: Get TimeZone
INTERFACE MS_BOOL MHAL_DISP_DeviceGetTimeZone(void *pDevCtx, MHAL_DISP_TimeZone_t *pstTimeZone);

// Device: Set TimeZone
INTERFACE MS_BOOL MHAL_DISP_DeviceSetTimeZoneConfig(void *pDevCtx, MHAL_DISP_TimeZoneConfig_t *pstTimeZoneCfg);

// Deivce: Get DisplayInfo
INTERFACE MS_BOOL MHAL_DISP_DeviceGetDisplayInfo(void *pDevCtx, MHAL_DISP_DisplayInfo_t *pstDisplayInfo);

// Device: Get Instance
INTERFACE MS_BOOL MHAL_DISP_DeviceGetInstance(MS_U32 u32DeviceId, void **pDevCtx);

// Device: Set PQ
INTERFACE MS_BOOL MHAL_DISP_DeviceSetPqConfig(void *pDevCtx, MHAL_DISP_PqConfig_t *pstPqCfg);

// Device Set Config
INTERFACE MS_BOOL MHAL_DISP_DeviceSetConfig(MS_U32 u32DevId, MHAL_DISP_DeviceConfig_t *pstDevCfg);

// Device Get Config
INTERFACE MS_BOOL MHAL_DISP_DeviceGetConfig(MS_U32 u32DevId, MHAL_DISP_DeviceConfig_t *pstDevCfg);

// DMA
INTERFACE MS_BOOL MHAL_DISP_DmaCreateInstance(MHAL_DISP_AllocPhyMem_t *pstAlloc, MS_U32 u32DmaId, void **pDmaCtx);
INTERFACE MS_BOOL MHAL_DISP_DmaDestoryInstance(void *pDmaCtx);

INTERFACE MS_BOOL MHAL_DISP_DmaBind(void *pDmaCtx, MHAL_DISP_DmaBindConfig_t *pstDmaBindCfg);
INTERFACE MS_BOOL MHAL_DISP_DmaUnBind(void *pDmaCtx, MHAL_DISP_DmaBindConfig_t *pstDmaBindCfg);

INTERFACE MS_BOOL MHAL_DISP_DmaSetAttr(void *pDmaCtx, MHAL_DISP_DmaAttrConfig_t *pstDmaAttrCfg);
INTERFACE MS_BOOL MHAL_DISP_DmaSetBufferAttr(void *pDmaCtx, MHAL_DISP_DmaBufferAttrConfig_t *pstDmaBufferAttrCfg);

INTERFACE MS_BOOL MHAL_DISP_EnableDmaIrq(void *pDmaCtx, MS_U32 u32DevIrq, MS_BOOL bEnable);
INTERFACE MS_BOOL MHAL_DISP_ClearDmaInterrupt(void *pDmaCtx, void* pData);
INTERFACE MS_BOOL MHAL_DISP_GetDmaIrqFlag(void *pDmaCtx, MHAL_DISP_IRQFlag_t *pstIrqFlag);
INTERFACE MS_BOOL MHAL_DISP_GetDmaIrq(void *pDmaCtx, MS_U32* pu32DevIrq);

// Device: Get Hw count
INTERFACE MS_BOOL MHAL_DISP_DeviceGetHwCount(MS_U32 *pu32Count);
//----------------------------------- Video Layer --------------------------------------------
// Video Layer: Create/Destroy
INTERFACE MS_BOOL MHAL_DISP_VideoLayerCreateInstance(const MHAL_DISP_AllocPhyMem_t *pstAlloc, const MS_U32 u32LayerId, void **pVidLayerCtx);
INTERFACE MS_BOOL MHAL_DISP_VideoLayerDestoryInstance(void *pVidLayerCtx);

// Video Layer: Enbale/Disable
INTERFACE MS_BOOL MHAL_DISP_VideoLayerEnable(void *pVidLayerCtx, const MS_BOOL bEnable);

// Video Layer: Binder/Unbinder device
INTERFACE MS_BOOL MHAL_DISP_VideoLayerBind(void *pVidLayerCtx, void *pDevCtx);
INTERFACE MS_BOOL MHAL_DISP_VideoLayerUnBind(void *pVidLayerCtx, void *pDevCtx);

// Video Layer: Set/Get Attribute
INTERFACE MS_BOOL MHAL_DISP_VideoLayerSetAttr(void *pVidLayerCtx, const MHAL_DISP_VideoLayerAttr_t *pstAttr);

// Video layer: Set/Get compress
INTERFACE MS_BOOL MHAL_DISP_VideoLayerSetCompress(void *pVidLayerCtx, const MHAL_DISP_CompressAttr_t* pstCompressAttr);

// Video layer: Set/Get display priority
INTERFACE MS_BOOL MHAL_DISP_VideoLayerSetPriority(void *pVidLayerCtx, const MS_U32 u32Priority);

// SW Trigger MGWIN switch double buffer
INTERFACE MS_BOOL MHAL_DISP_VideoLayerBufferFire(void *pVidLayerCtx);

// SW check MGWIN double buffer already run
INTERFACE MS_BOOL MHAL_DISP_VideoLayerCheckBufferFired(void *pVidLayerCtx);

// Video Lyaer: Get Hw Count
INTERFACE MS_BOOL MHAL_DISP_VideoLayerGetHwCount(MS_U32 *pu32Count);
//----------------------------------- Input Port ---------------------------------------------
// Input port: Create/Destroy
INTERFACE MS_BOOL MHAL_DISP_InputPortCreateInstance(const MHAL_DISP_AllocPhyMem_t *pstAlloc, void *pVidLayerCtx, const MS_U32 u32PortId, void **pCtx);

INTERFACE MS_BOOL MHAL_DISP_InputPortDestroyInstance(const void *pCtx);

// Input port: Enbale/Disbale
INTERFACE MS_BOOL MHAL_DISP_InputPortEnable(void *pInputPortCtx, const MS_BOOL bEnable);

// Input port: Enbale/Disbale
INTERFACE MS_BOOL MHAL_DISP_InputPortSetAttr(void *pInputPortCtx, const MHAL_DISP_InputPortAttr_t *pstAttr);

// Input port: Show
INTERFACE MS_BOOL MHAL_DISP_InputPortShow(void *pInputPortCtx);

// Input port: Hide
INTERFACE MS_BOOL MHAL_DISP_InputPortHide(void *pInputPortCtx);

// Input port: Attr batch process, All Channel setting enter batch process.
INTERFACE MS_BOOL MHAL_DISP_InputPortAttrBegin(void *pVidLayerCtx);
INTERFACE MS_BOOL MHAL_DISP_InputPortAttrEnd(void *pVidLayerCtx);

// Input port: Flip
INTERFACE MS_BOOL MHAL_DISP_InputPortFlip(void *pInputPortCtx, MHAL_DISP_VideoFrameData_t *pstVideoFrameBuffer);

// Input port: Rotate
INTERFACE MS_BOOL MHAL_DISP_InputPortRotate(void *pInputPortCtx, MHAL_DISP_RotateConfig_t *pstRotateCfg);

// Input port: Set Crop
INTERFACE MS_BOOL MHAL_DISP_InputPortSetCropAttr(void *pInputPortCtx, MHAL_DISP_VidWinRect_t *pstWinRect);

// Input port: Set  Ring Buffer Attribute
INTERFACE MS_BOOL MHAL_DISP_InputPortSetRingBuffAttr(void *pInputPortCtx, MHAL_DISP_RingBufferAttr_t *pstRingBufAttr);

// Input port: Set imi address
INTERFACE MS_BOOL MHAL_DISP_InputPortSetImiAddr(void *pInputPortCtx, const MS_U32 u32ImiAddr);

// Input port: Get Hw Count
INTERFACE MS_BOOL MHAL_DISP_InputPortGetHwCount(MS_U32 u32VidLayerId, MS_U32 *pu32Count);

INTERFACE MS_BOOL MHAL_DISP_DbgLevel(void *p);



#undef INTERFACE
#endif //
/** @} */ // end of HAL_DISP_group
