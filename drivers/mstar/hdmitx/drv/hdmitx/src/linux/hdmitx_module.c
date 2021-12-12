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

#define _HDMITX_MODULE_C_

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


//-------------------------------------------------------------------------------------------------
// Structure
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Variable
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// internal function
//-------------------------------------------------------------------------------------------------
void _DrvHdmiTxUpdateHpdIrq(struct platform_device *pDev)
{
    u32 u32HpdInt = 0;
    if(CamofPropertyReadU32(pDev->dev.of_node, "hpd_irq", &u32HpdInt))
    {
        //printf(PRINT_RED "%s %d, No hpd_irq property \n" PRINT_NONE, __FUNCTION__, __LINE__);
        u32HpdInt = 0;
    }
    DrvHdmitxOsSetHpdIrq(u32HpdInt ? 1 : 0);
}

void _DrvHdmitxUpdateI2cId(struct platform_device *pDev)
{
    u32 u32Val;

    if(CamofPropertyReadU32(pDev->dev.of_node, "i2c_id", &u32Val))
    {
        u32Val = 0;
    }

    DrvHdmitxOsSetI2cId(u32Val);
}


void _DrvHdmitxUpdateHpdGpio(struct platform_device *pDev)
{
    u32 u32Val;

    if(CamofPropertyReadU32(pDev->dev.of_node, "hpd_gpio", &u32Val))
    {
        printf(PRINT_RED "%s %d, No hpd_gpio property \n" PRINT_NONE, __FUNCTION__, __LINE__);
    }
    else
    {

        MS_U8 u8Pin = u32Val & 0xFF;
        DrvHdmitxOsSetHpdGpinPin(u8Pin);
        DrvHdmitxOsGpioRequestInput(u8Pin);
    }
}

void _DrvHdmixUpdateSWDdcGpio(struct platform_device *pDev)
{
    u32 u32SwI2c = 0;
    u32 u32I2cSda = 0;
    u32 u32I2cScl = 0;
    if(CamofPropertyReadU32(pDev->dev.of_node, "i2c_sw", &u32SwI2c))
    {
        printf(PRINT_RED "%s %d, No sw i2c property \n" PRINT_NONE, __FUNCTION__, __LINE__);
        u32SwI2c = 0;
    }

    DrvHdmitxOsSetSwI2cEn(u32SwI2c ? TRUE : FALSE);

    if(u32SwI2c)
    {
        if(CamofPropertyReadU32(pDev->dev.of_node, "i2c_sda_gpio", &u32I2cSda))
        {
            printf(PRINT_RED "%s %d, No i2c_sda_gpio property \n" PRINT_NONE, __FUNCTION__, __LINE__);
            u32I2cSda = 0;
        }

        if(CamofPropertyReadU32(pDev->dev.of_node, "i2c_scl_gpio", &u32I2cScl))
        {
            printf(PRINT_RED "%s %d, No i2c_sda_gpio property \n" PRINT_NONE, __FUNCTION__, __LINE__);
            u32I2cSda = 0;
        }

        DrvHdmitxOsSetSwI2cPin(u32I2cSda, u32I2cScl);
    }
}

void _DrvHdmitxModuleInit(void)
{
    int s32Ret;

    if(_tHdmitxDevice.refCnt == 0)
    {
        _tHdmitxDevice.refCnt++;

        s32Ret = alloc_chrdev_region(&_tHdmitxDevice.tDevNumber, 0, 1, DRV_HDMITX_DEVICE_NAME);

        if (s32Ret < 0)
        {
            printf(PRINT_RED "%s %d major number alloc fail\r\n", __FUNCTION__, __LINE__);
        }

        if(!_tHdmitxClass)
        {
            _tHdmitxClass = msys_get_sysfs_class();
            if(!_tHdmitxClass)
            {
                _tHdmitxClass = CamClassCreate(THIS_MODULE, HdmitxClassName);
            }
        }
        else
        {
            cdev_init(&_tHdmitxDevice.cdev, &_tHdmitxDevice.fops);
            if (0 != (s32Ret= cdev_add(&_tHdmitxDevice.cdev, _tHdmitxDevice.tDevNumber, DRV_HDMITX_DEVICE_COUNT)))
            {
                printf(PRINT_RED "%s %d Unable add a character device\n" PRINT_NONE, __FUNCTION__, __LINE__);
            }
        }

        if(_tHdmitxDevice.devicenode == NULL && _tHdmitxClass)
        {
            _tHdmitxDevice.devicenode = CamDeviceCreate(_tHdmitxClass, NULL, _tHdmitxDevice.tDevNumber, NULL, DRV_HDMITX_DEVICE_NAME);

            if (NULL == _tHdmitxDevice.devicenode)
            {
                printf(PRINT_RED "%s %d device_create() fail\n" PRINT_NONE, __FUNCTION__, __LINE__);
            }

            DrvHdmitxSysfsInit(_tHdmitxDevice.devicenode);
        }

        if(stDrvHdmitxPlatformDevice.dev.of_node == NULL)
        {
            stDrvHdmitxPlatformDevice.dev.of_node = of_find_compatible_node(NULL, NULL, "sstar,hdmitx");
        }
        if(stDrvHdmitxPlatformDevice.dev.of_node == NULL)
        {
            printf(PRINT_RED "%s %d Get Device mode Fail!!\n" PRINT_NONE, __FUNCTION__, __LINE__);
        }

        _DrvHdmitxUpdateI2cId(&stDrvHdmitxPlatformDevice);


        _DrvHdmitxUpdateHpdGpio(&stDrvHdmitxPlatformDevice);
        _DrvHdmiTxUpdateHpdIrq(&stDrvHdmitxPlatformDevice);
        _DrvHdmixUpdateSWDdcGpio(&stDrvHdmitxPlatformDevice);
        DrvHdmitxOsSetDeviceNode(&stDrvHdmitxPlatformDevice);

    }
}

void _DrvHdmitxModuleDeInit(void)
{
    if (_tHdmitxDevice.refCnt)
    {
        _tHdmitxDevice.refCnt--;
    }

    if (_tHdmitxDevice.refCnt == 0)
    {
        if (_tHdmitxDevice.cdev.count)
        {
            // Remove a cdev from the system
            cdev_del(&_tHdmitxDevice.cdev);
         }

         //CamDeviceDestroy(m_pstSclClass, _tHdmitxDevice.tDevNumber);
         unregister_chrdev_region(_tHdmitxDevice.tDevNumber, 1);

         stDrvHdmitxPlatformDevice.dev.of_node = NULL;
         _tHdmitxClass = NULL;
     }
}

