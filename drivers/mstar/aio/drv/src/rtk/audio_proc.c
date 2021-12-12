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

#include "audio_proc.h"

//
#include "hal_audio_common.h"
#include "hal_audio_config.h"
#include "hal_audio_sys.h"
#include "hal_audio_types.h"
#include "hal_audio_api.h"

AudProcInfoAi_t gAudProInfoAiList[AUD_AI_DEV_NUM] = {0};
AudProcInfoAo_t gAudProInfoAoList[AUD_AO_DEV_NUM] = {0};

void AudioProcInit(void)
{
}

void AudioProcDeInit(void)
{
}
