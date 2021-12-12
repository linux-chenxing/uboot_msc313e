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

#include "padmux.h"
#include "mdrv_padmux.h"
#include "mdrv_puse.h"
#include "hal_audio_common.h"
#include "hal_audio_pri_api.h"



BOOL HalAudOsApiChangeAmpGpio(U32 *nGpioPad, S8 s8Ch)
{
    if(s8Ch == 0)
        *nGpioPad = mdrv_padmux_getpad(MDRV_PUSE_AIO_AMP_PWR); // It may be PAD_UNKNOWN.

    return TRUE;
}

BOOL HalAudOsApiPreSetAllPadmux(void)
{
    S32 nMode, nPadMuxMode;
    nMode = mdrv_padmux_getmode(MDRV_PUSE_AIO_MCK); // It may be PINMUX_FOR_UNKNOWN_MODE.
    if( nMode == PINMUX_FOR_I2S_MCK_MODE_1 ||
        nMode == PINMUX_FOR_I2S_MCK_MODE_2 ||
        nMode == PINMUX_FOR_I2S_MCK_MODE_3 ||
        nMode == PINMUX_FOR_I2S_MCK_MODE_4
        ) {
        if (nMode == PINMUX_FOR_I2S_MCK_MODE_1)
        {
            nPadMuxMode = 1;
        }
        else if (nMode == PINMUX_FOR_I2S_MCK_MODE_2)
        {
            nPadMuxMode = 2;
        }
        else if (nMode == PINMUX_FOR_I2S_MCK_MODE_3)
        {
            nPadMuxMode = 3;
        }
        else if (nMode == PINMUX_FOR_I2S_MCK_MODE_4)
        {
            nPadMuxMode = 4;
        }
        else
        {
            nPadMuxMode = 0;
        }
        HalAudPriSetPadMuxI2sTX(nPadMuxMode);
    }

    // I2S
    nMode = mdrv_padmux_getmode(MDRV_PUSE_I2S_BCK); // It may be PINMUX_FOR_UNKNOWN_MODE.
    //for 4-wire mode or 6-wire mode used
    if( nMode == PINMUX_FOR_I2S_TX_MODE_1 ||
        nMode == PINMUX_FOR_I2S_TX_MODE_2 ||
        nMode == PINMUX_FOR_I2S_TX_MODE_3 ||
        nMode == PINMUX_FOR_I2S_TX_MODE_4 ||
        nMode == PINMUX_FOR_I2S_TX_MODE_5 ||
        nMode == PINMUX_FOR_I2S_TX_MODE_6 ||
        nMode == PINMUX_FOR_I2S_TX_MODE_7
        ) {
        if (nMode == PINMUX_FOR_I2S_TX_MODE_1)
        {
            nPadMuxMode = 1;
        }
        else if (nMode == PINMUX_FOR_I2S_TX_MODE_2)
        {
            nPadMuxMode = 2;
        }
        else if (nMode == PINMUX_FOR_I2S_TX_MODE_3)
        {
            nPadMuxMode = 3;
        }
        else if (nMode == PINMUX_FOR_I2S_TX_MODE_4)
        {
            nPadMuxMode = 4;
        }
        else if (nMode == PINMUX_FOR_I2S_TX_MODE_5)
        {
            nPadMuxMode = 5;
        }
        else if (nMode == PINMUX_FOR_I2S_TX_MODE_6)
        {
            nPadMuxMode = 6;
        }
        else if (nMode == PINMUX_FOR_I2S_TX_MODE_7)
        {
            nPadMuxMode = 7;
        }
        else
        {
            nPadMuxMode = 0;
        }
        HalAudPriSetPadMuxI2sTX(nPadMuxMode);
    }

    //for 4-wire mode used
    if( nMode == PINMUX_FOR_I2S_RXTX_MODE_1 ||
        nMode == PINMUX_FOR_I2S_RXTX_MODE_2 ||
        nMode == PINMUX_FOR_I2S_RXTX_MODE_3 ||
        nMode == PINMUX_FOR_I2S_RXTX_MODE_4 ||
        nMode == PINMUX_FOR_I2S_RXTX_MODE_5 ||
        nMode == PINMUX_FOR_I2S_RXTX_MODE_6
        ) {
        if (nMode == PINMUX_FOR_I2S_RXTX_MODE_1)
        {
            nPadMuxMode = 1;
        }
        else if (nMode == PINMUX_FOR_I2S_RXTX_MODE_2)
        {
            nPadMuxMode = 2;
        }
        else if (nMode == PINMUX_FOR_I2S_RXTX_MODE_3)
        {
            nPadMuxMode = 3;
        }
        else if (nMode == PINMUX_FOR_I2S_RXTX_MODE_4)
        {
            nPadMuxMode = 4;
        }
        else if (nMode == PINMUX_FOR_I2S_RXTX_MODE_5)
        {
            nPadMuxMode = 5;
        }
        else if (nMode == PINMUX_FOR_I2S_RXTX_MODE_6)
        {
            nPadMuxMode = 6;
        }
        else
        {
            nPadMuxMode = 0;
        }
        HalAudPriSetPadMuxI2sTXRX(nPadMuxMode);
    }

    if( nMode == PINMUX_FOR_I2S_RX_MODE_1 ||
        nMode == PINMUX_FOR_I2S_RX_MODE_2 ||
        nMode == PINMUX_FOR_I2S_RX_MODE_3 ||
        nMode == PINMUX_FOR_I2S_RX_MODE_4
        ) {
        if (nMode == PINMUX_FOR_I2S_RX_MODE_1)
        {
            nPadMuxMode = 1;
        }
        else if (nMode == PINMUX_FOR_I2S_RX_MODE_2)
        {
            nPadMuxMode = 2;
        }
        else if (nMode == PINMUX_FOR_I2S_RX_MODE_3)
        {
            nPadMuxMode = 3;
        }
        else if (nMode == PINMUX_FOR_I2S_RX_MODE_4)
        {
            nPadMuxMode = 4;
        }
        else
        {
            nPadMuxMode = 0;
        }
        HalAudPriSetPadMuxI2sRX(nPadMuxMode);
    }

    // DMIC
    nMode = mdrv_padmux_getmode(MDRV_PUSE_DMIC_CLK); // It may be PINMUX_FOR_UNKNOWN_MODE.

    if (nMode == PINMUX_FOR_DMIC_MODE_1)
    {
        nPadMuxMode = 1;
    }
    else if (nMode == PINMUX_FOR_DMIC_MODE_2)
    {
        nPadMuxMode = 2;
    }
    else if (nMode == PINMUX_FOR_DMIC_MODE_3)
    {
        nPadMuxMode = 3;
    }
    else if (nMode == PINMUX_FOR_DMIC_MODE_4)
    {
        nPadMuxMode = 4;
    }
    else if (nMode == PINMUX_FOR_DMIC_MODE_5)
    {
        nPadMuxMode = 5;
    }
    else if (nMode == PINMUX_FOR_DMIC_MODE_6)
    {
        nPadMuxMode = 6;
    }
    else if (nMode == PINMUX_FOR_DMIC_MODE_7)
    {
        nPadMuxMode = 7;
    }
    else if (nMode == PINMUX_FOR_DMIC_MODE_8)
    {
        nPadMuxMode = 8;
    }
    else
    {
        nPadMuxMode = 0;
    }

    HalAudPriSetPadMuxDmic(nPadMuxMode);

    return TRUE;
}
