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

#include "mhal_common.h"
#include "apiHDMITx.h"
#include "drvHDMITx.h"
#include "HDMITX_private.h"


/*********************************************************************/
/*                                                                                                                     */
/*                                                      Defines                                                    */
/*                                                                                                                     */
/*********************************************************************/
#define EN_HDMITX_V2_DBG                (0)

#if (EN_HDMITX_V2_DBG == 1)
#define DBG_HDMITX_V2(_f)                  _f
#else
#define DBG_HDMITX_V2(_f)
#endif





