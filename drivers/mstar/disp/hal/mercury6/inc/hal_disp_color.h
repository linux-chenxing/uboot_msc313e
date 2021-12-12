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

#ifndef _HAL_DISP_COLOR_H_
#define _HAL_DISP_COLOR_H_

//-------------------------------------------------------------------------------------------------
//  Defines & Macro
//-------------------------------------------------------------------------------------------------
#define HAL_DISP_COLOR_CSC_NUM  4


#define HAL_DISP_COLOR_CSC_ID   1

//-------------------------------------------------------------------------------------------------
//  structure & Enum
//-------------------------------------------------------------------------------------------------
typedef enum
{
    E_DISP_COLOR_YUV_2_RGB_MATRIX_BYPASS,
    E_DISP_COLOR_YUV_2_RGB_MATRIX_HDTV,
    E_DISP_COLOR_YUV_2_RGB_MATRIX_SDTV,
    E_DISP_COLOR_YUV_2_RGB_MATRIX_USER,
    E_DISP_COLOR_YUV_2_RGB_MATRIX_MAX,
}HalDispColorYuvToRgbMatrixType_e;

typedef enum
{
    E_DISP_COLOR_CSC_ID_0, // video
    E_DISP_COLOR_CSC_ID_1, //
    E_DISP_COLOR_CSC_ID_2,
    E_DISP_COLOR_CSC_ID_3,
    E_DISP_COLOR_CSC_ID_NUM,
}HalDispColorCscIdType_e;
//-------------------------------------------------------------------------------------------------
//  Prototype
//-------------------------------------------------------------------------------------------------

#ifdef _HAL_DISP_COLOR_C_
#define INTERFACE
#else
#define INTERFACE extern
#endif

INTERFACE bool HalDispColorInitVar(void *pCtx);
INTERFACE bool HalDispColorSetColorCorrectMatrix(u8 u8Id, s16 *psColorCorrectMatrix, void *pCtx);
INTERFACE bool HalDispColorSeletYuvToRgbMatrix(u8 u8Id, HalDispColorYuvToRgbMatrixType_e enType, s16 *psYuv2RgbMatrix, void *pCtx);
INTERFACE bool HalDispColorAdjustBrightness( u8 u8Id, u8 u8BrightnessR, u8 u8BrightnessG, u8 u8BrightnessB, void *pCtx);
INTERFACE bool HalDispColorAdjustHCS( u8 u8Id, u8 u8Hue, u8  u8Saturation, u8  u8Contrast, void *pCtx );
INTERFACE bool HalDispColorAdjustVideoContrast( u8 u8Id, u8 u8Contrast, void *pCtx );
INTERFACE bool HalDispColorAdjustVideoSaturation( u8 u8Id, u8 u8Saturation, void *pCtx );
INTERFACE bool HalDispColorAdjustVideoCbCr( u8 u8Id, u8 u8Cb, u8 u8Cr, void *pCtx );
INTERFACE bool HalDispColorAdjustVideoHue( u8 u8Id, u8 u8Hue, void *pCtx );
INTERFACE bool HalDispColorAdjustVideoRGB( u8 u8Id, u8 u8RCon, u8 u8GCon, u8 u8BCon, void *pCtx);
INTERFACE bool HalDispColorAdjustPcContrastRGB( u8 u8Id, u8 u8Contrast, u8 u8Red, u8 u8Green, u8 u8Blue, void *pCtx );
INTERFACE bool HalDispColorAdjustPcContrast( u8 u8Id, u8 u8Contrast, void *pCtx );
INTERFACE bool HalDispColorAdjustPcRGB( u8 u8Id, u8 u8Red, u8 u8Green, u8 u8Blue, void *pCtx );
INTERFACE bool HalDispColorSetYUV2RGBCtrl( u8 u8Id, bool bEnable, void *pCtx );
INTERFACE bool HalDispColorSetPCsRGBCtrl( u8 u8Id, bool bEnable, void *pCtx );
INTERFACE bool HalDispColorSetHue256StepCtrl( u8 u8Id, bool bEnable, void *pCtx );


#undef INTERFACE
#endif
