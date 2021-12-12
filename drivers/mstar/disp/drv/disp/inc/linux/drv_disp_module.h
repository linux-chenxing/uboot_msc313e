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

#ifndef _DRV_DISP_MODULE_H_
#define _DRV_DISP_MODULE_H_


//-------------------------------------------------------------------------------------------------
// Define & Macro
//-------------------------------------------------------------------------------------------------
#define DRV_DISP_DEVICE_COUNT    1
#define DRV_DISP_DEVICE_NAME     "mdisp"


//-------------------------------------------------------------------------------------------------
// Structure
//-------------------------------------------------------------------------------------------------
typedef struct
{
    dev_t tDevNumber;
    int refCnt;
    struct cdev cdev;
    struct file_operations fops;
    struct device *devicenode;
}DrvDispModuleDevice_t;


//-------------------------------------------------------------------------------------------------
// Varialbe
//-------------------------------------------------------------------------------------------------
#ifdef _DISP_MODULE_C_
DrvDispModuleDevice_t _tDispDevice =
{
    .tDevNumber = 0,
    .refCnt = 0,
    .devicenode = NULL,
    .cdev =
    {
        .kobj = {.name= DRV_DISP_DEVICE_NAME, },
        .owner = THIS_MODULE,
    },
    /*
    .fops =
    {
        .open = DrvDispModuleOpen,
        .release = DrvDispModuleRelease,
        .unlocked_ioctl = DrvDispModuleIoctl,
        .poll = DrvDispModulePoll,
    },*/
};

struct class * _tDispClass = NULL;
char * DispClassName = "m_disp_class";


u64 u64Disp_DmaMask = 0xffffffffUL;

struct platform_device stDrvDispPlatformDevice =
{
    .name = DRV_DISP_DEVICE_NAME,
    .id = 0,
    .dev =
    {
        .of_node = NULL,
        .dma_mask = &u64Disp_DmaMask,
        .coherent_dma_mask = 0xffffffffUL
    }
};

#endif


//-------------------------------------------------------------------------------------------------
// Prototype
//-------------------------------------------------------------------------------------------------

#ifdef _DRV_DISP_MODULE_C_
#define INTERFACE
#else
#define INTERFACE extern
#endif



#undef INTERFACE

#endif
