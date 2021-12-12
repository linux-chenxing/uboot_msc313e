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

#ifndef _DRV_DISP_OS_HEADER_H_
#define _DRV_DISP_OS_HEADER_H_


#include <common.h>
#include <command.h>
#include <config.h>
#include <malloc.h>
#include <stdlib.h>


//-------------------------------------------------------------------------------------------------
//  Defines & Macro
//-------------------------------------------------------------------------------------------------
#define CamOsPrintf  printf


#define WRITE_SCL_REG(addr, type, data)  ((*(volatile type *)(addr)) = (data))
#define READ_SCL_REG(addr, type)         ((*(volatile type *)(addr)))


#define READ_BYTE(x)         READ_SCL_REG(x, u8)
#define READ_WORD(x)         READ_SCL_REG(x, u16)
#define READ_LONG(x)         READ_SCL_REG(x, u32)
#define WRITE_BYTE(x, y)     WRITE_SCL_REG(x, u8, y)
#define WRITE_WORD(x, y)     WRITE_SCL_REG(x, u16, y)
#define WRITE_LONG(x, y)     WRITE_SCL_REG(x, u32, y)

#define DISP_TIMEZONE_ISR_SUPPORT             HAL_DISP_TIMEZONE_ISR_SUPPORT_UBOOT
#define DISP_VGA_HPD_ISR_SUPPORT              HAL_DISP_VGA_HPD_ISR_SUPPORT_UBOOT
#define DISP_REG_ACCESS_MODE                  HAL_DISP_REG_ACCESS_MD_UBOOT

#define DISP_OS_STRING_MAX                     256

#define DISP_IRQNUM_Default                    0

#define DISP_OS_VIR_RIUBASE                     0x1F000000

#define DrvDispOsSprintfStrcat(str, _fmt, _args...) \
{  \
    str=str;\
    printf( _fmt, ## _args); \
}

//-------------------------------------------------------------------------------------------------
//  Structure & Emu
//-------------------------------------------------------------------------------------------------

// Task
typedef void* (*TaskEntryCb)(void *argv);

typedef struct
{
    s32 s32Id;
} DrvDispOsTaskConfig_t;


typedef struct
{
    void *pFile;
}DrvDispOsFileConfig_t;

typedef struct
{
    s32 argc;
    u8 *argv[DISP_OS_STRING_MAX];
}DrvDispOsStrConfig_t;


typedef struct
{
    u8 pu8Name[64];
    u8 pu8Value[64];
}DrvDispOsTextItemConfig_t;

typedef struct
{
    DrvDispOsTextItemConfig_t *pstItem;
    u32 u32Size;
}DrvDispOsTextConfig_t;

typedef struct
{
    s32 s32Id;
}DrvDispOsMutexConfig_t;

#endif
