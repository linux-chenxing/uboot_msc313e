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

#ifndef DRV_AUDIO_IO_H_
#define DRV_AUDIO_IO_H_

#define TRACE_UT_LEVEL_TAG  0
#define ERROR_UT_LEVEL_TAG  1
#define LOG_UT_LEVEL_TAG    1

#define TRACE_UT_LEVEL  "[AUD UT TRACE] "
#define ERROR_UT_LEVEL  "[AUD UT ERROR] "
#define LOG_UT_LEVEL    "[AUD UT LOG] "

#define LOG_MSG (1)
#if LOG_MSG
#define AUD_UT_PRINTF(level ,fmt, arg...)   if (level##_TAG) CamOsPrintf(level fmt, ##arg);
#else
#define AUD_UT_PRINTF(level ,fmt, arg...)
#endif

#endif /* DRV_AUDIO_IO_H_ */
