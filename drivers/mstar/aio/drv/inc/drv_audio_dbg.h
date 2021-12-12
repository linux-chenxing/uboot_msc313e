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

#ifndef __DRV_AUDIO_DEBUG_H__
#define __DRV_AUDIO_DEBUG_H__
//------------------------------------------------------------------------------
//  Macros
//------------------------------------------------------------------------------

#if 0 // All Enable
#define TRACE_LEVEL_TAG     1
#define ERROR_LEVEL_TAG     1
#define DEBUG_LEVEL_TAG     1
#define IRQ_LEVEL_TAG       1
#define MHAL_LEVEL_TAG      1
#define LC_LEVEL_TAG        1 // Level Count
#else
#define TRACE_LEVEL_TAG     0
#define ERROR_LEVEL_TAG     1
#define DEBUG_LEVEL_TAG     0
#define IRQ_LEVEL_TAG       0
#define MHAL_LEVEL_TAG      0
#define LC_LEVEL_TAG        0 // Level Count
#endif

#define TRACE_LEVEL         "[AUDIO TRACE]"
#define ERROR_LEVEL         "[AUDIO ERROR]"
#define DEBUG_LEVEL         "[AUDIO DEBUG]"
#define IRQ_LEVEL           "[AUDIO IRQ]"
#define MHAL_LEVEL          "[AUDIO MHAL]"
#define LC_LEVEL            "[AUDIO LC]"

#define LOG_MSG 					1
#if LOG_MSG
#define AUD_PRINTF(level ,fmt, arg...)		if (level##_TAG) CamOsPrintf(level fmt, ##arg);
#else
#define AUD_PRINTF(level ,fmt, arg...)
#endif

#endif  /* __DRV_AUDIO_DEBUG_H__ */
