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

#ifndef _AUDIO_TEST_TYPE_H_
#define _AUDIO_TEST_TYPE_H_

typedef struct AUD_I2sCfg_s
{
    int nTdmMode;
    int nMsMode;
    int nBitWidth;
    int nFmt;
    int u16Channels;
    int e4WireMode;
    int eMck;
}_AUD_I2sCfg_s;

typedef struct AUD_PcmCfg_s
{
    int nRate;
    int nBitWidth;
    int n16Channels;
    int nInterleaved;
    int n32PeriodSize; //bytes
    int n32StartThres;
    int nTimeMs;
    int nI2sConfig;
    _AUD_I2sCfg_s sI2s;
    int nSpecialDumpChannels;
    char aoUseFile;
    char file_name[100];
}_AUDPcmCfg_t;

// Bench
typedef struct AUD_BenchRead_s
{
    void *aBufAddr;
    unsigned int nLen;
}_AUD_BenchRead_s;

typedef struct AUD_SineGenCfg_s
{
    unsigned char bEnable;
    unsigned char u8Freq;
    unsigned char u8Gain;
}_AUD_SineGenCfg_s;

typedef struct AUD_GainCfg_s
{
    int s16Gain;
    char s8Ch;
    
}_AUD_GainCfg_s;

#endif /* _AUDIO_TEST_TYPE_H_ */
