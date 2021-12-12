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

#ifndef _HAL_DISP_PICTURE_IF_H_
#define _HAL_DISP_PICTURE_IF_H_

//-------------------------------------------------------------------------------------------------
//  Defines & Macro
//-------------------------------------------------------------------------------------------------
#define HAL_DISP_PICTURE_IF_SHARPNESS_NUM   2

//-------------------------------------------------------------------------------------------------
//  structure & Enum
//-------------------------------------------------------------------------------------------------
typedef enum
{
    E_HAL_DISP_PICTURE_CONTRAST = 0,
    E_HAL_DISP_PICTURE_R_BRIGHTNESS,
    E_HAL_DISP_PICTURE_G_BRIGHTNESS,
    E_HAL_DISP_PICTURE_B_BRIGHTNESS,
    E_HAL_DISP_PICTURE_SATURATION,
    E_HAL_DISP_PICTURE_SHARPNESS,
    E_HAL_DISP_PICTURE_SHARPNESS1,
    E_HAL_DISP_PICTURE_HUE,
    E_HAL_DISP_PICTURE_NUM,
}HalDispPictureIfType_e;


typedef struct
{
    u16 u16OSD_0;
    u16 u16OSD_25;
    u16 u16OSD_50;
    u16 u16OSD_75;
    u16 u16OSD_100;
}HalDispPictureIfNonLinearCurveType_t;

typedef struct
{
    HalDispPictureIfNonLinearCurveType_t stDispDevice0[E_HAL_DISP_PICTURE_NUM];
    HalDispPictureIfNonLinearCurveType_t stDispDevice1[E_HAL_DISP_PICTURE_NUM];
}HalDispPictureIfNonLinearCurveConfig_t;

typedef struct
{
    u16 u16Hue;
    u16 u16Saturation;
    u16 u16Contrast;
    u16 u16BrightnessR;
    u16 u16BrightnessG;
    u16 u16BrightnessB;
    u16 u16Sharpness[HAL_DISP_PICTURE_IF_SHARPNESS_NUM];
}HalDispPictureConfig_t;

typedef struct
{
    bool bUpdate;
    u16 u16GainR;
    u16 u16GainG;
    u16 u16GainB;
    u16 u16BrightnessR;
    u16 u16BrightnessG;
    u16 u16BrightnessB;
    u16 u16Hue;
    u16 u16Saturation;
    u16 u16Contrast;
    u16 u16Sharpness;
    u16 u16Sharpness1;
    s16 as16Coef[9];
}HalDispPicturePqConfig_t;
//-------------------------------------------------------------------------------------------------
//  Prototype
//-------------------------------------------------------------------------------------------------

#ifdef _HAL_DISP_PICTURE_IF_C_
#define INTERFACE
#else
#define INTERFACE extern
#endif

INTERFACE void HalDispPictureIfSetConfig(HalDispCscmatrix_e eCscMatrix, HalDispPictureConfig_t *pstPictureCfg, void *pCtx);
INTERFACE bool HalDispPictureIfTransNonLinear(void *pCtx, HalDispCsc_t *pstCsc, u32 *pu32Sharpness, HalDispPictureConfig_t *pstPictureCfg);
INTERFACE void HalDispPictureIfSetPqConfig(void *pCtx);


#undef INTERFACE
#endif
