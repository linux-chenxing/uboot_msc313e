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

#ifndef __DRVCMDQ_IO_HH__
#define __DRVCMDQ_IO_HH__

#define IOCTL_CMDQ_TEST1_CONFIG                                (1)
#define IOCTL_VIP_MAGIC                                        ('3')
/**
* Used to set CMDQ cmd, use ST_IOCTL_VIP_CMDQ_CONFIG.
*/
#define IOCTL_CMDQ_TEST_1                            _IO(IOCTL_VIP_MAGIC,  IOCTL_CMDQ_TEST1_CONFIG)


#endif
