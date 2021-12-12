/*
* mhal_pinmux.h- Sigmastar
*
* Copyright (C) 2018 Sigmastar Technology Corp.
*
* Author: nick.lin <nick.lin@sigmastar.com.tw>
*
* This software is licensed under the terms of the GNU General Public
* License version 2, as published by the Free Software Foundation, and
* may be copied, distributed, and modified under those terms.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
*/
#ifndef __MHAL_PINMUX_H__
#define __MHAL_PINMUX_H__

#include <MsTypes.h>

extern S32 HalPadSetVal(U32 u32PadID, U32 u32Mode);
extern S32 HalPadSetMode(U32 u32Mode);

#endif // __MHAL_PINMUX_H__
