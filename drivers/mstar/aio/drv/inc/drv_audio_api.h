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

#ifndef __DRV_AUDIO_API_H__
#define __DRV_AUDIO_API_H__



typedef BOOL (*DmaStopChannelCbFn)(AudDmaChn_e);



//extern void DrvAudApiDtsInit(void);
//extern void DrvAudApiIrqInit(void);
//extern void DrvAudApiIrqSetDmaStopCb(DmaStopChannelCbFn pfnCb );
//extern BOOL DrvAudApiIrqRequest(const char *name, void *dev_id);
//extern BOOL DrvAudApiIrqFree(void *dev_id);
extern BOOL DrvAudApiAoAmpEnable(BOOL bEnable, S8 s8Ch);
extern BOOL DrvAudApiAoAmpCtrlInit(void);
//extern u32 DrvAudApiIoAddress(u32 addr);



#endif  /* __DRV_AUDIO_API_H__ */
