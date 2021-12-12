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

#include <common.h>
#include <environment.h>

#include <linux/stddef.h>
#include "asm/arch/mach/platform.h"

typedef struct parts_info
{
    u8 au8_partName[16];   //The partition's name
    u8 u8_Trunk;           //A distinguishing number for partitions with the same name,e.g. IPL0,IPL1,IPL2
    u8 u8_BackupTrunk;     //The backup_partition's index
    u8 u8_Active;          //Mark the partition is active or not
    u8 u8_Group;           //Reserved function, e.g. put IPL0,IPL_CUST1,UBOOT0 into the same group,then there status are all active
    u32 u32_Offset;        //The partition's start address(byte)
    u32 u32_Size;          //The partition's size
    u32 u32_Reserved;
} PARTS_INFO_t;

typedef struct parts_tbl
{
    u8 au8_magic[16];        //"SSTARSEMICIS0001"
    u32 u32_Checksum;
    u32 u32_Size;
    //PARTS_INFO_t *pst_partsInfo;
} PARTS_TBL_t;

static void _flash_search_parts(u32 u32_parts_address, u32 u32_size, const char *parts, size_t *offset, size_t *limit)
{
    u32 u32_parts_end;

    PARTS_TBL_t *pst_tbl;
    PARTS_INFO_t *pst_part;

    pst_tbl = (PARTS_TBL_t*)u32_parts_address;
    pst_part = (PARTS_INFO_t*)(u32_parts_address + sizeof(PARTS_TBL_t));

    *offset = 0;
    *limit = 0;
    u32_parts_end = (u32)pst_part + pst_tbl->u32_Size;

    if (u32_size < pst_tbl->u32_Size)
    {
        return;
    }

    for (; u32_parts_end > (u32)pst_part; pst_part++)
    {
        if (!strcmp((const char*)parts, (const char*)pst_part->au8_partName))
        {
            *offset = pst_part->u32_Offset;
            *limit = pst_part->u32_Size;
            printf("%s: offset = 0x%x size = 0x%x\r\n", pst_part->au8_partName, *offset, pst_part->u32_Size);
            break;
        }
    }
}

#ifdef CONFIG_MS_ISP_FLASH

#include <spi_flash.h>

#define PARTS_SEARCH_OFFSET             (((*((volatile short*)(SPI_BASE_ADDR + 0x08))) + (0xFFF * 2)) & ~0xFFF)
#define PARTS_SEARCH_END                (0x10000)
#define DEFAULT_PARTS_OFFSET            (0x21000)

static const char au8_magicData[] = {'S', 'S', 'T', 'A', 'R', 'S', 'E', 'M', 'I', 'C', 'I', 'S', '0', '0', '0', '1'};

void flash_get_parts(void *flash, const char *parts, size_t *offset, size_t *limit)
{
    int u32_pni_address;

    struct spi_flash *nor = (struct spi_flash*)flash;

    char *p;

    *offset = 0;
    *limit = 0;

    p = malloc(nor->erase_size);

    do
    {
        for (u32_pni_address = PARTS_SEARCH_OFFSET; PARTS_SEARCH_END > u32_pni_address; u32_pni_address += nor->erase_size)
        {
            if (spi_flash_read(nor, u32_pni_address, nor->erase_size, p))
            {
                printf("spi_flash_read() failed!\r\n");
            }

            if (!memcmp(au8_magicData, (char*)p, sizeof(au8_magicData)))
            {
                break;
            }
            //asm("b .");
        }

        if (PARTS_SEARCH_END == u32_pni_address)
        {
            u32_pni_address = DEFAULT_PARTS_OFFSET;

            if (spi_flash_read(nor, u32_pni_address, nor->erase_size, p))
            {
                printf("spi_flash_read() failed\r\n");
            }

            if (memcmp(au8_magicData, (char*)p, sizeof(au8_magicData)))
            {
                break;
            }
        }

        _flash_search_parts((u32)p, nor->erase_size, parts, offset, limit);
    } while (0);

    free(p);
}

#else
#include <nand.h>

void flash_get_parts(void* flash, const char *parts, size_t *offset, size_t *limit)
{
    nand_info_t *nand = (nand_info_t*) flash;
    char *p;

    size_t off;
    size_t rdsize;
    size_t blk;

    if (NULL == (p = malloc(nand->writesize)))
    {
        return;
    }

    for (blk = 0; blk < 10; blk += 2)
    {
        off = (nand->erasesize * blk) + nand->writesize;
        rdsize = nand->writesize;
        printf("[+]%s: off = 0x%x rdsize = 0x%x\r\n", __func__, off, rdsize);

        if (!nand_read_skip_bad(nand, off, &rdsize, NULL, nand->writesize, (u_char *)p))
        {
            break;
        }

        printf("[-]%s: off = 0x%x rdsize = 0x%x\r\n", __func__, off, rdsize);
    }

    _flash_search_parts((u32)p, nand->writesize, parts, offset, limit);

    free(p);
}

#endif

