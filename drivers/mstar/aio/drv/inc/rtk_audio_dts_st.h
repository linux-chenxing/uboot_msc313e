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

#ifndef __RTK_AUDIO_DTS_ST_H__
#define __RTK_AUDIO_DTS_ST_H__
// ------------------------
// DTS define
// ------------------------

// Macro
#define AUD_ARRAY_SIZE(arr)     (sizeof(arr) / sizeof((arr)[0]))

typedef struct
{
    const char *str;
    const int pin;
    const int value;

} AudRtkDtsGpio_t;

typedef struct
{
    const char *str;
    const int value;

} AudRtkDtsPadMux_t;

typedef struct
{
    const char *str;
    const int value;

} AudRtkDtsKeepI2sClk_t;

typedef struct
{
    const char *str;
    const int value;

} AudRtkDtsKeepAdcPowerOn_t;

typedef struct
{
    const char *str;
    const int value;

} AudRtkDtsKeepDacPowerOn_t;

typedef struct
{
    const char *str;
    const int value;

} AudRtkDtsI2sPcmMode_t;

typedef struct
{
    const char *AudRtkDtsCompatibleStr;
    const int AudRtkDtsIrqId;
    const AudRtkDtsGpio_t *AudRtkDtsGpioList;
    const int AudRtkDtsNumOfGpio;
    const AudRtkDtsPadMux_t *AudRtkDtsPadmuxList;
    const int AudRtkDtsNumOfPadmux;
    const AudRtkDtsKeepI2sClk_t *AudRtkDtsKeepI2sClkList;
    const AudRtkDtsKeepAdcPowerOn_t *AudRtkDtsKeepAdcPowerOn;
    const AudRtkDtsKeepDacPowerOn_t *AudRtkDtsKeepDacPowerOn;
    const int AudRtkDtsNumOfKeepI2sClk;
    const AudRtkDtsI2sPcmMode_t *AudRtkDtsI2sPcmMode;

} AudRtkDts_t;

// ------------------------
// DTS API
// ------------------------
BOOL _AudRtkDtsFindCompatibleNode( const char *compatible_str );
int _AudRtkDtsGetIrqId( void );
BOOL _AudRtkDtsGetGpio( const char *amp_gpio_str, U32 *aGpio );
BOOL _AudRtkDtsGetPadMux( const char *padmux_str, S32 *padmux );
BOOL _AudRtkDtsGetKeepI2sClk(const char *keep_i2s_clk_str, S32 *keepI2sClk);
BOOL _AudRtkDtsGetKeepDacPowerOn(const char *keep_dac_power_on, S32 *keepDacPowerOn);
BOOL _AudRtkDtsGetKeepAdcPowerOn(const char *keep_adc_power_on, S32 *keepAdcPowerOn);
BOOL _AudRtkDtsGetI2sPcmMode(const char *i2s_pcm_mode_str, S32 *I2sPcmMode);

#endif
