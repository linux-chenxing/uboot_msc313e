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

#ifndef __HAL_AUDIO_SYS_H__
#define __HAL_AUDIO_SYS_H__

////////////////////////////////////////////////////////////////////////////
//
// Config by platform
//
////////////////////////////////////////////////////////////////////////////

// Padmux Defualt
#define AUD_DEF_DIG_PMUX        3
#define AUD_DEF_I2S_TX_PMUX     6
#define AUD_DEF_I2S_RX_PMUX     6
#define AUD_DEF_I2S_MCK_PMUX    7
#define AUD_DEF_I2S_RXTX_PMUX   0


// Dev ID
typedef enum
{
    AUD_AI_DEV_AMIC,
    AUD_AI_DEV_DMIC,
    AUD_AI_DEV_I2S_RX,
    AUD_AI_DEV_LINE_IN,
    AUD_AI_DEV_AMIC_D2,
    AUD_AI_DEV_NUM,

} AudAiDev_e;

typedef enum
{
    AUD_AO_DEV_LINE_OUT,
    AUD_AO_DEV_I2S_TX,
    AUD_AO_DEV_LINE_OUT_D1_L,
    AUD_AO_DEV_LINE_OUT_D2_L,
    AUD_AO_DEV_LINE_OUT_EP,
    AUD_AO_DEV_NUM,

} AudAoDev_e;

// ------------------------
// Common Fucntion
// ------------------------

// No exhaust space for write AO dma buffer, No exhaust data for read AI dma buffer.
#define NO_EXHAUST_DMA_BUF          ( 0 )
#define DMA_BUF_REMAINDER           ( 128 ) // 128 Bytes ( 8 MIU Line )

#define MIU_WORD_BYTE_SIZE          ( 16 )    //bytes
#define DMA_LOCALBUF_SIZE           ( 64 )    //bytes
#define DMA_LOCALBUF_LINE           ( DMA_LOCALBUF_SIZE/MIU_WORD_BYTE_SIZE )

#define BACH_DPGA_GAIN_MAX_DB       ( 34 )
#define BACH_DPGA_GAIN_MIN_DB       ( -64 )   //actually -63.5 dB
#define BACH_DPGA_GAIN_MIN_IDX      ( 0x7F )

#endif  // __HAL_AUDIO_SYS_H__
