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

#define _DRV_HDMITX_MODULE_C_

#include <linux/cdev.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>

#include "ms_msys.h"
#include "cam_os_wrapper.h"
#include "cam_sysfs.h"
#include "hdmitx_sysfs.h"
#include "mhal_common.h"
#include "drv_hdmitx_os.h"
#include "drv_hdmitx_module.h"

//-------------------------------------------------------------------------------------------------
// Define & Macro
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Prototype
//-------------------------------------------------------------------------------------------------
static int DrvHdmitxModuleProbe(struct platform_device *pdev);
static int DrvHdmitxModuleRemove(struct platform_device *pdev);
static int DrvHdmitxModuleSuspend(struct platform_device *dev, pm_message_t state);
static int DrvHdmitxModuleResume(struct platform_device *dev);
extern void _DrvHdmitxModuleInit(void);
extern void _DrvHdmitxModuleDeInit(void);
//-------------------------------------------------------------------------------------------------
// Structure
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
// Variable
//-------------------------------------------------------------------------------------------------
extern DrvHdmitxModuleDevice_t _tHdmitxDevice;
extern struct class * _tHdmitxClass;
extern char * HdmitxClassName;
extern struct platform_device stDrvHdmitxPlatformDevice;


struct of_device_id _HdmitxMatchTable[] =
{
    { .compatible = "sstar,hdmitx" },
    {}
};


struct platform_driver stDrvHdmitxPlatformDriver =
{
    .probe      = DrvHdmitxModuleProbe,
    .remove     = DrvHdmitxModuleRemove,
    .suspend    = DrvHdmitxModuleSuspend,
    .resume     = DrvHdmitxModuleResume,
    .driver =
    {
        .name   = DRV_HDMITX_DEVICE_NAME,
        .owner  = THIS_MODULE,
        .of_match_table = of_match_ptr(_HdmitxMatchTable),
    },
};

//-------------------------------------------------------------------------------------------------
// internal function
//-------------------------------------------------------------------------------------------------


//==============================================================================
static int DrvHdmitxModuleSuspend(struct platform_device *dev, pm_message_t state)
{
    int ret = 0;
    return ret;
}

static int DrvHdmitxModuleResume(struct platform_device *dev)
{
    int ret = 0;
    return ret;
}

void _DrvHdmitxGetIrqNum(struct platform_device *pDev, u8 u8Idx, u8 u8DevId)
{
    unsigned int HdmitxIrqId = 0; //INT_IRQ_AU_SYSTEM;
    HdmitxIrqId  = CamOfIrqToResource(pDev->dev.of_node, u8Idx, NULL);

    if (!HdmitxIrqId)
    {
        printf(PRINT_RED "%s %d Can't Get SCL_IRQ\n" PRINT_NONE, __FUNCTION__, __LINE__);
    }
    else
    {
        //DrvHdmitxIrqSetIsrNum(u8DevId, HdmitxIrqId);
    }
}


//-------------------------------------------------------------------------------------------------
// Module functions
//-------------------------------------------------------------------------------------------------
static int DrvHdmitxModuleProbe(struct platform_device *pdev)
{
    _tHdmitxClass = msys_get_sysfs_class();

    if (IS_ERR(_tHdmitxClass))
    {
        printf(PRINT_RED "%s %d class_create() fail. Please exec [mknod] before operate the device\n" PRINT_NONE,  __FUNCTION__, __LINE__);
    }

    stDrvHdmitxPlatformDevice.dev.of_node = pdev->dev.of_node;

    //create device
    _DrvHdmitxModuleInit();

    return 0;
}

static int DrvHdmitxModuleRemove(struct platform_device *pdev)
{
    _DrvHdmitxModuleDeInit();

    CamDeviceUnregister(_tHdmitxDevice.devicenode);
    return 0;
}

int DrvHdmitxModuleInit(void)
{
    int ret = 0;

    ret = CamPlatformDriverRegister(&stDrvHdmitxPlatformDriver);
    if (!ret)
    {
        //printf(PRINT_RED "%s %d CamPlatformDriverRegister Successful\n" PRINT_NONE, __FUNCTION__, __LINE__);
    }
    else
    {
        printf(PRINT_RED "%s %d CamPlatformDriverRegister failed\n" PRINT_NONE, __FUNCTION__, __LINE__);
        CamPlatformDriverUnregister(&stDrvHdmitxPlatformDriver);
    }

    return ret;
}
void DrvHdmitxModuleExit(void)
{
    /*de-initial the who GFLIPDriver */
    CamPlatformDriverUnregister(&stDrvHdmitxPlatformDriver);
}

module_init(DrvHdmitxModuleInit);
module_exit(DrvHdmitxModuleExit);

MODULE_AUTHOR("CAMDRIVER");
MODULE_DESCRIPTION("camdriver sclhvsp ioctrl driver");
#ifdef SUPPORT_GPL_SYMBOL
MODULE_LICENSE("GPL");
#else
MODULE_LICENSE("PROPRIETARY");
#endif
