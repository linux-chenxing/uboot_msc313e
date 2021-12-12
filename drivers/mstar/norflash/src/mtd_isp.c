/*
* mtd_isp.c- Sigmastar
*
* Copyright (C) 2018 Sigmastar Technology Corp.
*
* Author: karl.xiao <karl.xiao@sigmastar.com.tw>
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

#include <common.h>
#include <flash.h>
#include <malloc.h>
#include <asm/errno.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/concat.h>
#include <mdrvSNI.h>
#include <spi_flash.h>
#include <drvSPINOR.h>
#include <mtd_isp.h>
#include <mdrvSNI.h>
#include <linux/err.h>
#include "asm/arch/mach/platform.h"

static u32 g_u32_sni_address = 0;
const u8 au8_basicMagicData[] = {0x4D, 0x53, 0x54, 0x41, 0x52, 0x53, 0x45, 0x4D, 0x49, 0x55, 0x53, 0x46, 0x44, 0x43, 0x49, 0x53};
static SPINOR_INFO_t *g_pst_spinor_info;
unsigned int g_size=0;

#define FLASH_FLASH_SIZE                    g_pst_spinor_info->u32_Capacity
#define FLASH_SECTOR_SIZE                   g_pst_spinor_info->u16_SectorByteCnt
#define FLASH_BLOCK_SIZE                    g_pst_spinor_info->u32_BlkBytesCnt
#define FLASH_SECTOR_SIZE_MASK              (g_pst_spinor_info->u16_SectorByteCnt - 1)
#define FLASH_BLOCK_SIZE_MASK               (g_pst_spinor_info->u32_BlkBytesCnt - 1)
#define FLASH_PAGE_SIZE                     g_pst_spinor_info->u16_PageByteCnt
#define FLASH_PAGE_SIZE_MASK                (g_pst_spinor_info->u16_PageByteCnt - 1)
#define FLASH_SNI_HEADER_SIZE               sizeof(au8_basicMagicData)
#define FLASH_SNI_TABLE_SIZE                (sizeof(SPINOR_INFO_t) + FLASH_SNI_HEADER_SIZE)

#ifdef CONFIG_MS_MTD_ISP_FLASH

static u8 _spi_flash_is_sni_header(u8 *pu8_data)
{
#if 0
    u8 u8_i;

    for (u8_i = 0; u8_i < FLASH_SNI_HEADER_SIZE; u8_i++)
    {
        if (au8_basicMagicData[u8_i] != pu8_data[u8_i])
        {
            return 0;
        }
    }

    return 1;
#endif
    return !(memcmp(au8_basicMagicData, pu8_data, sizeof(au8_basicMagicData)));
}

static void _spi_flash_show_device_id(SPINOR_INFO_t *pst_spinor_info)
{
    u8 u8_i;

    printk("[FLASH] Device id in SNI is 0x");

    for (u8_i = 0; pst_spinor_info->u8_IDByteCnt > u8_i; u8_i++)
    {
        printk("%x",pst_spinor_info->au8_ID[u8_i]);

        if (u8_i != (pst_spinor_info->u8_IDByteCnt - 1))
        {
            printk(" 0x");
        }
    }
    printk("\r\n");
}

static u8 _spi_flash_is_sni_match(SPINOR_INFO_t *pst_spinor_info)
{
    u8 au8_device_id[8];

    if (ERR_SPINOR_SUCCESS != DRV_SPINOR_read_id(((U8*)&au8_device_id), pst_spinor_info->u8_IDByteCnt))
    {
        debug("[FLASH_ERR] read device id fail!\r\n");
        return 0;
    }

    return !memcmp(au8_device_id, pst_spinor_info->au8_ID ,pst_spinor_info->u8_IDByteCnt);
}

static void _spi_flash_switch_config(FLASH_CMD_SET_t *pt_cmt, u8 u8_enabled)
{
    u8 u8_fn;
    u8 u8_status;
    u8 u8_status_bytes;
    u8 (*read_status[3])(u8*, u8) = {DRV_SPINOR_read_status1, DRV_SPINOR_read_status2, DRV_SPINOR_read_status3};
    u32 u32_status;

    u32_status = 0;
    u8_status_bytes = 0;
    for (u8_fn = 0; sizeof(read_status)/sizeof(void*) > u8_fn && pt_cmt->u16_dataBytes > u8_status_bytes; u8_fn++)
    {
        if (0x01 & (pt_cmt->u32_address >> u8_fn))
        {
            if (ERR_SPINOR_SUCCESS != read_status[u8_fn](&u8_status, 1))
            {
                printk("[FLASH_ERR] Read status registers fail!\r\n");
            }
            u32_status |= (u8_status << (u8_status_bytes * 8));
            u8_status_bytes++;
            printk("[FLASH] status%d = 0x%x\r\n",u8_fn + 1,u8_status);
        }
    }

    if (u8_enabled)
    {
        u32_status |= pt_cmt->u16_value;
    }
    else
    {
        u32_status &= ~pt_cmt->u16_value;
    }

    if (ERR_SPINOR_SUCCESS != DRV_SPINOR_write_status(pt_cmt->u8_command, (u8*)&u32_status, pt_cmt->u16_dataBytes))
    {
        printk("[FLASH_ERR] DRV_SPINOR_write_status fail!\r\n");
    }
}

static void _spi_flash_unlock_whole_flash(u32 u32_sni_address)
{
    SPINOR_INFO_t *pst_spinor_info;
    SPINOR_PROTECT_STATUS_t *pst_w_protectStatus;
    FLASH_CMD_SET_t *pt_w_cmt;

    pst_spinor_info = (SPINOR_INFO_t*)u32_sni_address;
    pst_w_protectStatus = &pst_spinor_info->st_wProtectStatus;
    pt_w_cmt = &pst_w_protectStatus->st_blockStatus.st_blocks;

    if (0 < pt_w_cmt->u16_dataBytes)
    {
        pst_w_protectStatus = &pst_spinor_info->st_wProtectStatus;
        pt_w_cmt = &pst_w_protectStatus->st_blockStatus.st_blocks;
        _spi_flash_switch_config(pt_w_cmt, 0);
    }
}

static void _spi_flash_show_protect_status(u32 u32_sni_address)
{
    u8 u8_status_name_index;

    u32 u32_status_size;
    u32 u32_status;

    SPINOR_INFO_t *pst_spinor_info;
    SPINOR_PROTECT_STATUS_t *pst_protectStatus;
    FLASH_CMD_SET_t *pt_cmt;

    const u8 *pau8_status_name[] = {(const u8*)"complement", (const u8*)"top/buttom", (const u8*)"blocks", (const u8*)"SRP0", (const u8*)"SRP1"};

    pst_spinor_info = (SPINOR_INFO_t*)u32_sni_address;
    pst_protectStatus = &pst_spinor_info->st_rProtectStatus;

    u32_status_size = sizeof(SPINOR_PROTECT_STATUS_t);
    pt_cmt = (FLASH_CMD_SET_t*) &pst_protectStatus->st_blockStatus.st_complement;
    u8_status_name_index = 0;

    while (0 != u32_status_size)
    {
        if (0 < pt_cmt->u16_dataBytes)
        {
            u32_status = 0;

            if (ERR_SPINOR_SUCCESS != DRV_SPINOR_read_status(pt_cmt->u8_command, (u8*)&u32_status, pt_cmt->u16_dataBytes))
            {
                printk("[FLASH_ERR] DRV_SPINOR_read_status fail!\r\n");
            }
            printk("[FLASH] %s = 0x%02x\r\n",pau8_status_name[u8_status_name_index],u32_status & pt_cmt->u16_value);
        }

        pt_cmt++;
        u32_status_size -= sizeof(FLASH_CMD_SET_t);
        u8_status_name_index++;
    }
}

static void _spi_flash_setup_by_sni(SPINOR_INFO_t *pst_spinor_info)
{
    SPINOR_QUAD_CFG_t *pst_quad_enabled;

    pst_quad_enabled = &pst_spinor_info->st_QE;

    if (pst_quad_enabled->u8_needQE)
    {
        _spi_flash_switch_config(&pst_quad_enabled->st_wQuadEnabled, 1);
    }

    _spi_flash_show_protect_status((u32)pst_spinor_info);

    DRV_SPINOR_setup_access(pst_spinor_info->st_readData.u8_command,
                            pst_spinor_info->st_readData.u8_dummy,
                            pst_spinor_info->st_program.u8_command);
    DRV_SPINOR_setup_time_wait(pst_spinor_info->u32_MaxWaitTime);

    if (!(pst_spinor_info->au8_reserved[0] & 0x01))
    {
        DRV_SPINOR_use_bdma(1);
        printk("[FLASH] Use BDMA.\r\n");
    }

    return;
}

static int _spi_flash_load_sni(u32 u32_address)
{
    u32 u32_sni_address;

    u32_sni_address = 0;

    for (u32_sni_address = FLASH_SEARCH_OFFSET; FLASH_SEARCH_END > u32_sni_address; u32_sni_address += 0x1000)
    {
        if (_spi_flash_is_sni_header((U8*)FLASH_GET_OFFSET(u32_sni_address)))
        {
            break;
        }
    }

    if (u32_sni_address == FLASH_SEARCH_END)
    {
        u32_sni_address = FLASH_DEFAULT_SNI_OFFSET;

        if (!_spi_flash_is_sni_header((U8*)FLASH_GET_OFFSET(u32_sni_address)))
        {
            printk("[FLASH_ERR] Can't find SNI from device!!\r\n");
            return -1;
        }
    }

    if (ERR_SPINOR_SUCCESS != DRV_SPINOR_read(u32_sni_address, (u8*)u32_address, FLASH_SNI_TABLE_SIZE))
    {
        printk("[FLASH_ERR] Load SNI fail!\r\n");
        return -1;
    }

    printk("[FLASH] Load SNI from 0x%x to 0x%x\r\n",u32_sni_address,u32_address);
    return 0;
}

static int _spi_flash_init(u32 u32_address)
{
    if (ERR_SPINOR_SUCCESS != DRV_SPINOR_init())
    {
        printk("[FLASH_ERR] init fail!\r\n");
        return (-EIO);
    }

    g_pst_spinor_info = (SPINOR_INFO_t*)(u32_address + FLASH_SNI_HEADER_SIZE);

    if (!_spi_flash_is_sni_header((u8*)u32_address))
    {
        _spi_flash_load_sni(u32_address);
    }

    if (_spi_flash_is_sni_header((u8*)u32_address))
    {
        _spi_flash_show_device_id(g_pst_spinor_info);
        _spi_flash_setup_by_sni(g_pst_spinor_info);
        _spi_flash_unlock_whole_flash((u32)g_pst_spinor_info);
    }

    printk("[FLASH] End flash init.\r\n");
    return 0;
}

unsigned long flash_init (void)
{
    unsigned int bus = CONFIG_SF_DEFAULT_BUS;
    unsigned int cs = CONFIG_SF_DEFAULT_CS;
    unsigned int speed = CONFIG_SF_DEFAULT_SPEED;
    unsigned int mode = CONFIG_SF_DEFAULT_MODE;

    spi_flash_probe(bus, cs, speed, mode);
    if (g_size>0)
        return (g_size);

    return 0;
}

#endif // CONFIG_MS_MTD_ISP_FLASH

#ifdef CONFIG_CMD_SF
#include <spi_flash.h>
int _spi_flash_read(struct spi_flash *flash, u32 offset,
		size_t len, void *buf)
{
    printk(KERN_WARNING "%s from 0x%x, len 0x%x, to 0x%x \r\n", __func__, (u32)offset, len, (unsigned int)buf);
    u8 u8_status = 0;
    u32 starttime = get_timer(0);

    /* sanity checks */
    if (!len)
        return -EINVAL;

    if (offset + len > flash->size)
        return -EINVAL;

    if (ERR_SPINOR_SUCCESS != (u8_status = DRV_SPINOR_read(offset, (U8*)buf, len)))
    {
        printk(KERN_WARNING "err 0x%x\r\n", u8_status);
        return (-EIO);
    }

    printk(KERN_WARNING "(cost %ld ms)\n", get_timer(0) - starttime);
    return 0;
}

int _spi_flash_write(struct spi_flash *flash, u32 offset,
		size_t len, const void *buf)
{
    printk(KERN_WARNING "%s to 0x%x, len 0x%x from 0x%x \r\n", __func__,(u32)offset, len, (unsigned int)buf);
    u32 starttime = get_timer(0);
    u16 u16_write_size = 0;
    u8 *pu8_data = (u8 *)buf;
    u32 total;
    u32 percent = 0;
    u32 pre_percent = 0;

    /* sanity checks */
    if (!len)
        return (-EINVAL);
    if (offset + len > flash->size)
        return (-EINVAL);

    total = len;

    while (0 != len)
    {
        u16_write_size = FLASH_PAGE_SIZE - (FLASH_PAGE_SIZE_MASK & offset);
        if (u16_write_size > len)
        {
            u16_write_size = len;
        }

        if (ERR_SPINOR_SUCCESS != DRV_SPINOR_program_page(offset, pu8_data, u16_write_size))
        {
            printk("[FLASH_ERR] Program page fail\r\n");
            return (-EIO);
        }

        offset += u16_write_size;
        pu8_data += u16_write_size;
        len -= u16_write_size;

        percent = (total - len) * 100 / total;
        if(((percent - pre_percent) >= 5) || (percent == 100))
        {
            printk(KERN_WARNING "\rWriting at 0x%x -- %3d%% complete.", offset, percent);
            pre_percent = percent;
        }
    }

    printk(KERN_WARNING "(cost %ld ms)\n", get_timer(0) - starttime);
    return 0;
}

int _spi_flash_erase(struct spi_flash *flash, u32 offset,
		size_t len)
{
    printk(KERN_WARNING"%s: addr 0x%x, len 0x%x \r\n", __func__, (u32)offset, len);

    u32 u32_bytes_left = 0;
    u32 starttime = get_timer(0);
    u32 percent = 0;
    u32 pre_percent = 0;
	u32 u32_erase_size;

    /* range and alignment check */
    if (!len)
        return (-EINVAL);
    if (offset + len > flash->size || len % FLASH_SECTOR_SIZE || offset % FLASH_SECTOR_SIZE)
        return (-EINVAL);

    u32_bytes_left = len;

    while (0 != u32_bytes_left)
    {
        if (0 != (~FLASH_BLOCK_SIZE_MASK & u32_bytes_left))
        {
            if (ERR_SPINOR_SUCCESS != DRV_SPINOR_block_erase(offset))
            {
                printk(KERN_WARNING "block erase failed!\n");
                return (-EIO);
            }
            u32_erase_size = FLASH_BLOCK_SIZE;
        }
        else
        {
            if (ERR_SPINOR_SUCCESS != DRV_SPINOR_sector_erase(offset))
            {
                printk(KERN_WARNING "sector erase failed!\n");
                return (-EIO);
            }
            u32_erase_size = FLASH_SECTOR_SIZE;
        }

        offset += u32_erase_size;
        u32_bytes_left -= u32_erase_size;

        percent = (len - u32_bytes_left) * 100 / len;
        if(((percent - pre_percent) >= 5) || (percent == 100))
        {
            printk(KERN_WARNING "\rErasing at 0x%x -- %3d%% complete.", offset, percent);
            pre_percent = percent;
        }

    }

    printk(KERN_WARNING "(cost %ld ms)\n", get_timer(0) - starttime);
    return 0;
}

struct spi_flash *spi_flash_probe(unsigned int bus, unsigned int cs,
		unsigned int max_hz, unsigned int spi_mode)
{
    int i = 0;

    struct spi_flash *flash = NULL;
    struct mtd_info *mtd = NULL;

    if (!IS_ERR(mtd = get_mtd_device_nm("nor0")))
    {
        del_mtd_device(mtd);
    }

    if (0 == g_u32_sni_address)
    {
        g_u32_sni_address = (u32)malloc(FLASH_SNI_TABLE_SIZE);

        if (!g_u32_sni_address) {
            debug("SF: Failed to allocate memory\n");
            goto err;
        }

        // jedec_probe() will read id, so initialize hardware first
        if(0 != _spi_flash_init(g_u32_sni_address))
        {
            goto err;
        }
        printk("flash init successful\r\n");
    }

    if (!_spi_flash_is_sni_header((u8*)g_u32_sni_address)) //for empty chip upgrade
    {
        g_pst_spinor_info->u32_Capacity = 0x1000000;
        g_pst_spinor_info->u16_PageByteCnt = 0x100;
        g_pst_spinor_info->u16_SectorByteCnt = 0x1000;
    }
    flash = malloc(sizeof(struct spi_flash));
    if (!flash) {
        debug("SF: Failed to allocate memory\n");
        goto err;
    }

    flash->spi = NULL;
    flash->name = "nor0";
    flash->write = _spi_flash_write;
    flash->erase = _spi_flash_erase;
    flash->read = _spi_flash_read;
    flash->size = g_pst_spinor_info->u32_Capacity;
    flash->page_size = g_pst_spinor_info->u16_PageByteCnt;
    flash->sector_size = g_pst_spinor_info->u16_SectorByteCnt;
    flash->erase_size = g_pst_spinor_info->u16_SectorByteCnt;

    if(g_u32_sni_address)
    {
        flash->mtd.priv = flash;
        flash->mtd.name ="nor0";
        flash->mtd.type = MTD_NORFLASH;
        flash->mtd.writesize = 1;
        flash->mtd.flags = MTD_CAP_NORFLASH;
        flash->mtd.size = flash->size;
        g_size = flash->mtd.size;
        flash->mtd.erasesize = flash->erase_size ;
        flash->mtd.numeraseregions = 0;
        printk(
               "mtd .name = %s, .size = 0x%.8x (%uMiB) "
               ".erasesize = 0x%.8x .numeraseregions = %d\n",
               flash->mtd.name,
               (unsigned int)flash->mtd.size, (unsigned int)(flash->mtd.size >>20),
               (unsigned int)flash->mtd.erasesize,
               flash->mtd.numeraseregions);

        if (flash->mtd.numeraseregions)
            for (i = 0; i < flash->mtd.numeraseregions; i++)
                printk(
                       "mtd.eraseregions[%d] = { .offset = 0x%.8x, "
                       ".erasesize = 0x%.8x (%uKiB), "
                       ".numblocks = %d }\n",
                       i, (u32)flash->mtd.eraseregions[i].offset,
                       (unsigned int)flash->mtd.eraseregions[i].erasesize,
                       (flash->mtd.eraseregions[i].erasesize >>10 ),
                       (unsigned int)flash->mtd.eraseregions[i].numblocks);

        if (!add_mtd_device(&flash->mtd))
        {
            printk("SF: add mtd device successfully\n");
        }
    }
    if (!flash) {
        printk("SF: Unsupported manufacturer\n");
        goto err;
    }

    printk("SF: Detected %s with total size ", flash->name);
    print_size(flash->size, "\n");

    return flash;

err:
    return NULL;
}

struct spi_flash* spi_flash_probe_driver(u32 u32_address, unsigned int bus, unsigned int cs,
		unsigned int max_hz, unsigned int spi_mode)
{
    SPINOR_INFO_t *pst_info;

    while (_spi_flash_is_sni_header((u8*)u32_address))
    {
        pst_info = (SPINOR_INFO_t*)(u32_address + FLASH_SNI_HEADER_SIZE);

        if (_spi_flash_is_sni_match(pst_info))
        {
            if (0 != g_u32_sni_address)
            {
                memcpy((void*)g_u32_sni_address, (void*)u32_address, FLASH_SNI_TABLE_SIZE);
            }
            printf("spi_flash_probe_driver\r\n");
            _spi_flash_init(g_u32_sni_address);
            break;
        }

        u32_address += FLASH_SNI_TABLE_SIZE;
    }

    return spi_flash_probe(bus, cs, max_hz, spi_mode);
}

void spi_flash_free(struct spi_flash *flash)
{
    free(flash);
}

#endif
