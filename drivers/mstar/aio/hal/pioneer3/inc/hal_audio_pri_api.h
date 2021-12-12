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


#ifndef __HAL_AUD_PRI_API_H__
#define __HAL_AUD_PRI_API_H__



BOOL HalAudPriSetPadMuxI2sTdmTx(S32 nPadMuxMode);
BOOL HalAudPriSetPadMuxI2sTdmRx(S32 nPadMuxMode);
BOOL HalAudPriSetPadMuxDmic(S32 nPadMuxMode);
BOOL HalAudPriSetPadMuxMck(S32 nPadMuxMode);
BOOL HalAudPriSetPadMuxI2sMode(S32 nPadMuxMode);



#endif //__HAL_AUD_PRI_API_H__
