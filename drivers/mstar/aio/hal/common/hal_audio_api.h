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

#ifndef __HAL_AUD_API_H__
#define __HAL_AUD_API_H__

typedef BOOL (*AoAmpCb)(BOOL, S8 ch);

void HalAudMainInit(void);
void HalAudMainDeInit(void);

void HalAudSetAoAmpCb(AoAmpCb cb);
BOOL HalAudSetMux(AudMux_e eMux, U8 nChoice);
BOOL HalAudSetPadMux(AudPadMux_e PadMux, BOOL bEnable);
BOOL HalAudSrcSetRate(AudRate_e eRate);
BOOL HalAudAtopOpen(AudAtopPath_e path);
BOOL HalAudAtopClose(AudAtopPath_e path);
BOOL HalAudAtopMicAmpGain(AudAdcSel_e eAdcSel, U16 nSel, S8 ch);
BOOL HalAudAtopSetAdcGain(AudAdcSel_e eAdcSel, U16 nSel, S8 ch);
BOOL HalAudAtopSetAdcMux(AudAdcSel_e eAdc);
BOOL HalAudAtopSwap(BOOL bEnable);
BOOL HalAudI2sSaveRxTdmWsPgm(BOOL bEnable);
BOOL HalAudI2sSaveRxTdmWsWidth(U8 nWsWidth);
BOOL HalAudI2sSaveRxTdmWsInv(BOOL bEnable);
BOOL HalAudI2sSaveRxTdmChSwap(U8 nSwap_0_2, U8 nSwap_0_4);
BOOL HalAudI2sSaveTxTdmWsPgm(BOOL bEnable);
BOOL HalAudI2sSaveTxTdmWsWidth(U8 nWsWidth);
BOOL HalAudI2sSaveTxTdmWsInv(BOOL bEnable);
BOOL HalAudI2sSaveTxTdmChSwap(U8 nSwap_0_2, U8 nSwap_0_4);
BOOL HalAudI2sSaveTxTdmActiveSlot(U8 nActiveSlot);
BOOL HalAudI2sSetTdmDetails(AudI2s_e eI2s);
BOOL HalAudI2sSetKeepClk(BOOL bEnable);
BOOL HalAudI2sSetPcmMode(BOOL bEnable);
BOOL HalAudI2sSetRate(AudI2s_e eI2s,AudRate_e eRate);
BOOL HalAudI2sSetTdmMode(AudI2s_e eI2s, AudI2sMode_e eMode);
BOOL HalAudI2sSetMsMode(AudI2s_e eI2s, AudI2sMsMode_e eMsMode);
BOOL HalAudI2sSetFmt(AudI2s_e eI2s, AudI2sFmt_e eFmt);
BOOL HalAudI2sSetWidth(AudI2s_e eI2s, AudBitWidth_e eWidth);
BOOL HalAudI2sSetChannel(AudI2s_e eI2s, U16 nChannel);
BOOL HalAudI2sSetWireMode(AudI2s_e eI2s, AudWireMode_e eWireMode);
BOOL HalAudI2sSetSlotConfig(AudI2s_e eI2s, U16 nSlotMsk, AudTdmChnMap_e eMap);
BOOL HalAudI2sSetWckWidth(AudI2s_e eI2s, U16 nWidth);
BOOL HalAudI2sSetMck(AudI2s_e eI2s, AudI2sMck_e eMck);
BOOL HalAudI2sEnableMck(AudI2s_e eI2s, BOOL bEnable);
BOOL HalAudI2sEnable(AudI2s_e eI2s, BOOL bEnable);
BOOL HalAudDmaSetRate(AudDmaChn_e eDmaChannel, AudRate_e eRate);
BOOL HalAudDmaGetRate(AudDmaChn_e eDmaChannel, AudRate_e *eRate, U32 *nRate);
BOOL HalAudDmaReset(AudDmaChn_e eDmaChannel);
BOOL HalAudDmaEnable(AudDmaChn_e eDmaChannel, BOOL bEnable);
BOOL HalAudDmaPause(AudDmaChn_e eDmaChannel);
BOOL HalAudDmaResume(AudDmaChn_e eDmaChannel);
BOOL HalAudDmaIntEnable(AudDmaChn_e eDmaChannel, BOOL bDatatrigger, BOOL bDataboundary);
BOOL HalAudDmaGetInt(AudDmaChn_e eDmaChannel, BOOL *bDatatrigger, BOOL *bDataboundary);
BOOL HalAudDmaClearInt(AudDmaChn_e eDmaChannel);
U32 HalAudDmaGetLevelCnt(AudDmaChn_e eDmaChannel);
U32 HalAudDmaGetRawLevelCnt(AudDmaChn_e eDmaChannel);
U32 HalAudDmaTrigLevelCnt(AudDmaChn_e eDmaChannel, U32 nDataSize);
BOOL HalAudDmaSetPhyAddr(AudDmaChn_e eDmaChannel, U32 nBufAddrOffset, U32 nBufSize);
BOOL HalAudDmaWrSetThreshold(AudDmaChn_e eDmaChannel, U32 nOverrunTh);
BOOL HalAudDmaRdSetThreshold(AudDmaChn_e eDmaChannel, U32 nUnderrunTh);
BOOL HalAudDmaGetFlags(AudDmaChn_e eDmaChannel, BOOL *pbDatatrigger, BOOL *pbDataboundary, BOOL *pbLocalData);
BOOL HalAudDmaWrConfigMch(AudMchSet_e eMchSet, U16 nChNum, AudMchClkRef_e eMchClkRef, const int *mch_sel);
BOOL HalAudDmaMonoEnable(AudDmaChn_e eDmaChannel, BOOL bEnable);
void HalAudDpgaCtrl(AudDpga_e eDpga, BOOL bEnable, BOOL bMute, BOOL bFade);
BOOL HalAudDpgaSetGain(AudDpga_e eDpga, S8 gain, S8 ch);
BOOL HalAudDpgaSetGainOffset(AudDpga_e eDpga, S8 gainRegValue, S8 ch);
BOOL HalAudDpgaSetGainFading(AudDpga_e eDpga, U8 nFading, S8 ch);
BOOL HalAudDigMicEnable(BOOL bEn);
BOOL HalAudDigMicSetChannel(U16 nCh);
BOOL HalAudDigMicSetRate(AudRate_e eRate);
BOOL HalAudDigMicSetGain(S8 gain, S8 ch);
BOOL HalAudCfgI2sIfPrivateCb( void *pPriData );
BOOL HalAudDmaSineGen(AudDmaChn_e eDmaChannel, BOOL bEnable, U8 freq, U8 gain);
BOOL HalAud2Hdmi(BOOL bEnable);
BOOL HalAudKeepAdcPowerOn(U8 u8AdcPowerOn);
BOOL HalAudKeepDacPowerOn(U8 u8DacPowerOn);

#endif //__HAL_AUD_API_H__
