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


#include <linux/pfn.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>          /* seems do not need this */
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <asm/uaccess.h>
#include <linux/fs.h>
#include <linux/path.h>
#include <linux/namei.h>
#include <linux/delay.h>
#include <linux/device.h>

#include <asm/io.h>
#include <asm/string.h>
#include <linux/clk.h>
#include <linux/clk-provider.h>

#include <linux/cdev.h>
#include <linux/interrupt.h>
#include <linux/poll.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/blkdev.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/version.h>
#include <linux/gpio.h>
#include <linux/irq.h>
#include <linux/sched.h>
#include <linux/wait.h>

#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/of_address.h>


#include "cam_os_wrapper.h"

//-------------------------------------------------------------------------------------------------
//  Defines & Macro
//-------------------------------------------------------------------------------------------------

#define DISP_IO_OFFSET 0xDE000000

#define DISP_IO_ADDRESS(x)           ( (u32)(x) + DISP_IO_OFFSET )
//#define __io_address(n)       ((void __iomem __force *)DISP_IO_ADDRESS(n))

/* read register by byte */
#define disp_readb(a) (*(volatile unsigned char *)DISP_IO_ADDRESS(a))

/* read register by word */
#define disp_readw(a) (*(volatile unsigned short *)DISP_IO_ADDRESS(a))

/* read register by long */
#define disp_readl(a) (*(volatile unsigned int *)DISP_IO_ADDRESS(a))

/* write register by byte */
#define disp_writeb(v,a) (*(volatile unsigned char *)DISP_IO_ADDRESS(a) = (v))

/* write register by word */
#define disp_writew(v,a) (*(volatile unsigned short *)DISP_IO_ADDRESS(a) = (v))

/* write register by long */
#define disp_writel(v,a) (*(volatile unsigned int *)DISP_IO_ADDRESS(a) = (v))



#define READ_BYTE(x)         disp_readb(x)
#define READ_WORD(x)         disp_readw(x)
#define READ_LONG(x)         disp_readl(x)
#define WRITE_BYTE(x, y)     disp_writeb((u8)(y), x)
#define WRITE_WORD(x, y)     disp_writew((u16)(y), x)
#define WRITE_LONG(x, y)     disp_writel((u32)(y), x)

#define DISP_TIMEZONE_ISR_SUPPORT             HAL_DISP_TIMEZONE_ISR_SUPPORT_LINUX
#define DISP_VGA_HPD_ISR_SUPPORT              HAL_DISP_VGA_HPD_ISR_SUPPORT_LINUX
#define DISP_REG_ACCESS_MODE                  HAL_DISP_REG_ACCESS_MD_LINUX

#define DISP_OS_STRING_MAX                     256

#define DISP_IRQNUM_Default                    0

#define DISP_OS_VIR_RIUBASE                     0xFD000000


#define DrvDispOsSprintfStrcat(str, _fmt, _args...) \
    do {                                   \
        char tmpstr[1024];                 \
        sprintf(tmpstr, _fmt, ## _args);   \
        strcat(str, tmpstr);               \
    }while(0)


//-------------------------------------------------------------------------------------------------
//  Structure & Emu
//-------------------------------------------------------------------------------------------------

// Task
typedef void* (*TaskEntryCb)(void *argv);


typedef struct
{
    s32 s32Id;
    CamOsThread stThreadCfg;
} DrvDispOsTaskConfig_t;

typedef struct
{
    struct file *pFile;
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
    CamOsMutex_t stMutxCfg;
}DrvDispOsMutexConfig_t;


#endif
