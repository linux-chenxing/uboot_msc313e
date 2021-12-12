/*
* spinand.c- Sigmastar
*
* Copyright (C) 2018 Sigmastar Technology Corp.
*
* Author: edie.chen <edie.chen@sigmastar.com.tw>
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
#include <spinand_bbt.h>
#include <nand.h>
#include <linux/mtd/mtd.h>

/********************************************************************************
*Function: nand_is_valid_bbt(u32 u32_address)
*Description: Check it is a valid bbt table or not
*Input:
        u32_address: The address where stored the bbt info
*Output:
*Retrun:
        TRUE : It is a valid bbt table
        FALSE: It is not a valid bbt table
********************************************************************************/
static u8 nand_is_valid_bbt(u32 u32_address)
{
    BBT_INFO_t *pst_bbt_info = (BBT_INFO_t*) u32_address;
    if (BBT_HEADER == pst_bbt_info->u32_Header &&
        pst_bbt_info->u32_Crc32 == crc32(0, (void*)pst_bbt_info->stBlkInfo, sizeof(BLOCK_INFO_t) * BLOCK_INFO_NUM))
    {
        return 1;
    }

    return 0;
}

/********************************************************************************
*Function: nand_bbt_fill_blk_info(u32 u32Offset, u32 u32_address, u8 u8BlkType)
*Description: Fill the block type to bbt
*Input:
        u32Offset  : The address(byte) of block
        u32_address: The address where stored the bbt info
        u8BlkType  : The bad block type
*Output:
*Retrun:
********************************************************************************/
void nand_bbt_fill_blk_info(u32 u32Offset, u32 u32_address, u8 u8BlkType)
{
    u8 u8Remd = 0;
    u16 u16BlkIndex = 0;
    BBT_INFO_t *pst_bbt_info = (BBT_INFO_t*)u32_address;
    nand_info_t *nand = &nand_info[0];

    struct erase_info einfo;
    struct nand_chip *nand_chip = nand->priv;
    if((u8BlkType == FACTORY_BAD_BLOCK) || (u8BlkType == RUN_TIME_BAD_BLOCK))
    {
        memset(&einfo, 0, sizeof(einfo));
        einfo.mtd = nand;
        einfo.addr = u32Offset;
        einfo.len = 1ULL << nand_chip->phys_erase_shift;
        if(!nand_erase_nand(nand, &einfo, 0))
        {
            mtd_oob_ops_t ops = {
            .datbuf = NULL,
            .oobbuf = &u8BlkType,
            .ooblen = 1,
            .ooboffs = nand_chip->badblockpos,
            .mode = MTD_OPS_PLACE_OOB
            };
            if(mtd_write_oob(nand,u32Offset,&ops))
                printf("%s,u32Offset=0x%x,write oob failed\r\n",__func__,u32Offset);
        }
        else
            printf("%s,u32Offset=0x%x,erase block failed\r\n",__func__,u32Offset);

    }
    u16BlkIndex = u32Offset / nand->erasesize;    //u32Offset/u32BlkSz
    u8Remd = u16BlkIndex & 0x01;                  //u16BlkIndex%2
    u16BlkIndex = u16BlkIndex >> 1;               //u16BlkIndex/2
    if(u8Remd)
        pst_bbt_info->stBlkInfo[u16BlkIndex].b1 = u8BlkType;
    else
        pst_bbt_info->stBlkInfo[u16BlkIndex].b0 = u8BlkType;

    pst_bbt_info->u32_Crc32 = crc32(0, (void*)pst_bbt_info->stBlkInfo, sizeof(BLOCK_INFO_t) * BLOCK_INFO_NUM);
}

/********************************************************************************
*Function: u8 nand_load_bbt(u32 u32_address)
*Description: Read bbt from flash to dram
*Input:
        u32_address: The addresss to store the bbt
*Output:
*Retrun:
        TRUE : Read bbt successful
        FALSE: Read bbt failed
********************************************************************************/
u8 nand_load_bbt(u32 u32_address)
{
    u8 u8_block;
    u8 u8_bbt_search;
    u32 u32_offset;
    u32 u32_bbt_search_offset;
    size_t rwsize;
    nand_info_t *nand = &nand_info[0];

    for (u8_block = 1; u8_block < MAX_CIS_BLK_NUM; u8_block += 2)
    {
        u32_offset = u8_block * nand->erasesize;

        u32_bbt_search_offset = nand->writesize;
        u8_bbt_search = 2;
        /*u8_bbt_search is used to search page1 and page0*/
        while (0 != u8_bbt_search)
        {
            /*if the bbt stored in odd block page1,read it to dram for power off recover.
              Normally,the bbt stored in odd block page0,read the bbt from flash to dram*/
            if (!mtd_read(nand,u32_offset + u32_bbt_search_offset, sizeof(BBT_INFO_t), &rwsize, (u_char*)u32_address))
            {
                if(sizeof(BBT_INFO_t) == rwsize)
                {
                    if (nand_is_valid_bbt(u32_address))
                    {
                        printf("Found BBT in block %d, page %d\r\n", u8_block, u8_bbt_search - 1);
                        return 1;
                    }
                }
            }

            u32_bbt_search_offset -= nand->writesize;
            u8_bbt_search--;
        }
    }

    return 0;
}

/********************************************************************************
*Function: nand_save_bbt(u32 u32_address, u8 u8_update)
*Description: Save bbt table to flash
*Input:
        u32_address: The addresss where store the new bbt
        u8_update  : TRUE-Uptate bbt to page1   FALSE-Program bbt to page0
*Output:
*Retrun:
        TRUE : Save bbt successful
        FALSE: Save bbt failed
********************************************************************************/
u8 nand_save_bbt(u32 u32_address, u8 u8_update)
{
    u8 u8_block;
    u32 u32_offset;
    size_t rwsize;
    nand_info_t *nand = &nand_info[0];
    struct erase_info erase;
    memset(&erase, 0, sizeof(erase));

    for (u8_block = 1; u8_block < MAX_CIS_BLK_NUM; u8_block += 2)
    {
        u32_offset = u8_block * nand->erasesize;

        if (!nand_block_isbad(nand,u32_offset))
        {
            /*When do bbm flow,Update bbt in odd block page1*/
            if (u8_update)
            {
                if (!mtd_write(nand,u32_offset + nand->writesize, sizeof(BBT_INFO_t), &rwsize, (const u_char*)u32_address))
                {
                    if(sizeof(BBT_INFO_t) == rwsize)
                    {
                        printf("Upate BBT in block %d, page 1\r\n", u8_block);
                        return 1;
                    }
                }
            }
            else/*When creat bbt,program bbt in odd block page0*/
            {
                erase.mtd = nand;
                erase.len  = nand->erasesize;
                erase.addr = u32_offset;
                if(!mtd_erase(nand, &erase) )
                {
                    if (!mtd_write(nand, u32_offset, sizeof(BBT_INFO_t), &rwsize,(const u_char*)u32_address))
                    {
                        printf("Program BBT in block %d, page 0\r\n", u8_block);
                        return 1;
                    }
                }
            }
        }

        nand_bbt_fill_blk_info(u32_offset, u32_address, RUN_TIME_BAD_BLOCK);
    }
    return 0;
}

/********************************************************************************
*Function: nand_bbt_get_blk_info(u32 u32Offset)
*Description: Get the block info
*Input:
         u32Offset: The address(byte) of block
*Output:
*Retrun: 0x00:factory bad block       0x01:Ecc correctable  0x02:running-time bad block
         0x03:Ecc correctable-not fix 0x0f:good block       0xff:No bbt
********************************************************************************/
u8 nand_bbt_get_blk_info(u32 u32Offset)
{
    u8 u8Remd = 0;
    u8 u8Ret = 0x0f;
    u16 u16BlkIndex = 0;
    BBT_INFO_t st_bbt_info;
    nand_info_t *nand = &nand_info[0];

    nand_load_bbt((u32)&st_bbt_info);
    if (!nand_is_valid_bbt((u32)&st_bbt_info))
    {
        printf("No BBT!\r\n");
        return -1;
    }
    u16BlkIndex = u32Offset / nand->erasesize;    //u32Offset/u32BlkSz
    u8Remd = u16BlkIndex & 0x01;                  //u16BlkIndex%2
    u16BlkIndex = u16BlkIndex >> 1;               //u16BlkIndex/2
    if(u8Remd)
        u8Ret &= st_bbt_info.stBlkInfo[u16BlkIndex].b1;
    else
        u8Ret &= st_bbt_info.stBlkInfo[u16BlkIndex].b0;

    return u8Ret;
}

