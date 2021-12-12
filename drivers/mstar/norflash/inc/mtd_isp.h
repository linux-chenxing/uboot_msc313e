/*
 *#############################################################################
 * SigmaStar trade secret
 * Copyright (c) 2006-2011 SigmaStar Technology Corp.
 * All rights reserved.
 *
 * Unless otherwise stipulated in writing, any and all information contained
 * herein regardless in any format shall remain the sole proprietary of
 * SigmaStar Technology Corp. and be kept in strict confidence
 * (SigmaStar Confidential Information) by the recipient.
 * Any unauthorized act including without limitation unauthorized disclosure,
 * copying, use, reproduction, sale, distribution, modification, disassembling,
 * reverse engineering and compiling of the contents of SigmaStar Confidential
 * Information is unlawful and strictly prohibited. SigmaStar hereby reserves the
 * rights to any and all damages, losses, costs and expenses resulting therefrom.
 *
 *#############################################################################
 */

#ifndef _MTD_ISP_H_
#define _MTD_ISP_H_
#include <common.h>

typedef struct {
    u32 u32_eraseSize;
    u32 u32_writeSize;
    u32 u32_capacity;
} FLASH_INFO_t;

#define FLASH_SEARCH_OFFSET             (((*((volatile U16*)(SPI_BASE_ADDR + 0x08))) + 0xFFF) & ~0xFFF)
#define FLASH_SEARCH_END                (0x10000)

#define FLASH_DEFAULT_SNI_OFFSET        0x20000

#define FLASH_GET_OFFSET(offset)            (SPI_BASE_ADDR + (offset))

#define BIT(x)                  (1 << (x))
#define BITS(_bits_)            (BIT(((1)? _bits_) + 1) - BIT(((0)? _bits_)))
#define BIT_FILTER(x)           while(1) {\
                                if ((x) & 0x1) break; \
                                (x) = (x) >> 1; \
                                \}

#endif
