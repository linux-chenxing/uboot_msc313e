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

#include <common.h>
#include <malloc.h>
#include <linux/err.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/nand_ecc.h>
#include <asm/io.h>
#include <asm/errno.h>
#include <mdrvSNI.h>
#include <drvSPINAND.h>
#include <nand.h>

static const u8 au8_magicData[] = {0x4D, 0x53, 0x54, 0x41, 0x52, 0x53, 0x45, 0x4D, 0x49, 0x55, 0x53, 0x46, 0x44, 0x43, 0x49, 0x53};
static const u8 au8_extMagicData[] = {'S', 'S', 'T', 'A', 'R', 'S', 'E', 'M', 'I', 'S', 'N', 'I', 'V', '0', '0', '0'};

static u32 g_u32_sni_address = 0;
static SPINAND_INFO_t *g_pst_spinand_info = NULL;
static SPINAND_EXT_INFO_t *g_pst_spinand_ext_info = NULL;

#define FLASH_LEAGCY_SNI_SIZE           (sizeof(au8_magicData) + sizeof(SPINAND_INFO_t))

#define FLASH_SNI_HEADER_SIZE           sizeof(au8_magicData)
#define FLASH_EXT_SNI_HEADER_SIZE       sizeof(au8_extMagicData)
#define FLASH_MAX_SNI_SIZE              512
#define FLASH_PAGE_SIZE_MASK            (g_pst_spinand_info->u16_PageByteCnt - 1)
#define FLASH_BLOCK_MASK                (g_pst_spinand_info->u16_BlkPageCnt - 1)
#define FLASH_PAGE_SIZE                 g_pst_spinand_info->u16_PageByteCnt
#define FLASH_BLOCK_SIZE                (FLASH_PAGE_SIZE * g_pst_spinand_info->u16_BlkPageCnt)
#define FLASH_BLOCK_SIZE_MASK           (FLASH_BLOCK_SIZE - 1)
#define FLASH_BLOCK_COUNT               g_pst_spinand_info->u16_BlkCnt
#define FLASH_PAGES_PER_BLOCK           g_pst_spinand_info->u16_BlkPageCnt
#define FLASH_SNI_BLOCKS_SIZE           10
#define FLASH_SNI_TABLE_SIZE            0x200

#define FLASH_2K_PAGE_SIZE              (2 << 10)
#define FLASH_2K_BLOCK_SIZE             (FLASH_2K_PAGE_SIZE * 64)
#define FLASH_4K_PAGE_SIZE              (4 << 10)
#define FLASH_4K_BLOCK_SIZE             (FLASH_4K_PAGE_SIZE * 64)

#define FLASH_GOOD_BLOCK_MAKER           0xFF

#if 1
#define spi_nand_msg(fmt, ...) printk(KERN_NOTICE "%s: " fmt "\n", __func__, ##__VA_ARGS__)
#define spi_nand_debug(fmt, ...)
#else
#define spi_nand_msg(fmt, ...)
#define spi_nand_debug(fmt, ...) printk(KERN_NOTICE "%s: " fmt "\n", __func__, ##__VA_ARGS__)
#endif

u8 _flash_count_bits(u32 u32_x)
{
    u8 u8_i = 0;

    while (u32_x)
    {
        u8_i++;
        u32_x >>= 1;
    }

    return (u8_i - 1);
}

static u8 _flash_is_sni_header(const u8 *u8_header, u8 u8_size, u8 *pu8_data)
{
#if 0
    u8 u8_i;
    printf("sni:");
    for (u8_i = 0; u8_i < u8_size; u8_i++)
    {
        printf(" %x",pu8_data[u8_i]);
    }
    printf("\r\n");

#endif
    return !(memcmp(u8_header, pu8_data, u8_size));
}

static void _flash_unlock_whole_block(SPINAND_EXT_INFO_t *pst_ext_sni)
{
    u32 u32_status;

    SPINAND_PROTECT_t *pst_protect_status;
    FLASH_CMD_SET_t *pst_cmd;

    if (_flash_is_sni_header(au8_extMagicData, FLASH_EXT_SNI_HEADER_SIZE, pst_ext_sni->au8_magic))
    {
        if (0 == (SPINAND_ALL_LOCK & pst_ext_sni->st_profile.u16_flags))
        {
            return;
        }

        pst_protect_status = &pst_ext_sni->st_profile.st_extConfig.st_protectStatus;
        pst_cmd = &pst_protect_status->st_blockStatus.st_blocks;
        u32_status = 0;

        if (ERR_SPINAND_SUCCESS != DRV_SPINAND_get_features(pst_cmd->u32_address, (u8*)&u32_status, pst_cmd->u16_dataBytes))
        {
            printf("%s get features failed\r\n",__func__);
        }

        u32_status &= ~pst_cmd->u16_value;

        if (ERR_SPINAND_SUCCESS != DRV_SPINAND_set_features(pst_cmd->u32_address, (u8*)&u32_status, pst_cmd->u16_dataBytes))
        {
            printf("%s set features failed\r\n",__func__);
        }

        printf("[FLASH] Unlock all block.\r\n");
    }
}

static u8 _flash_is_id_match(u8* au8_id, u8 u8_id_len)
{
    u8 u8_i;
    u8 au8_device_id[8];
    if (ERR_SPINAND_SUCCESS != DRV_SPINAND_read_id(au8_device_id, u8_id_len))
    {
        spi_nand_debug("[FLASH] read device id fail!\r\n");
        return 0;
    }
    for(u8_i = 0; u8_id_len > u8_i; u8_i++)
    {
        if (au8_device_id[u8_i] != au8_id[u8_i])
        {
            return 0;
        }
    }
    return 1;
}

static int _flash_set_quad_mode(u8 u8_enabled)
{
    SPINAND_ACCESS_CONFIG_t *pst_access;
    u32 u32_qe_status;

    pst_access = &g_pst_spinand_ext_info->st_profile.st_access;
    u32_qe_status = 0;

    if(ERR_SPINAND_SUCCESS == DRV_SPINAND_check_status())
    {
        if (ERR_SPINAND_SUCCESS != DRV_SPINAND_get_features(pst_access->st_qeStatus.u32_address, (u8*)&u32_qe_status, pst_access->st_qeStatus.u16_dataBytes))
        {
            spi_nand_msg("[FLASH_ERR]get features failed\r\n");
            return 0;
        }

        if (u8_enabled)
        {
            u32_qe_status |= pst_access->st_qeStatus.u16_value;
        }
        else
        {
            u32_qe_status &= ~pst_access->st_qeStatus.u16_value;
        }

        if (ERR_SPINAND_SUCCESS != DRV_SPINAND_set_features(pst_access->st_qeStatus.u32_address, (u8*)&u32_qe_status, pst_access->st_qeStatus.u16_dataBytes))
        {
            spi_nand_msg("[FLASH_ERR] set features failed\r\n");
            return 0;
        }
    }
    else
    {
        return 0;
    }

    if(ERR_SPINAND_SUCCESS == DRV_SPINAND_check_status())
        return 1;

    return 0;
}

static void _flash_setup_by_sni(SPINAND_EXT_INFO_t *pst_ext_sni)
{
    SPINAND_EXT_PROFILE_t *pst_profile;
    SPINAND_ACCESS_CONFIG_t *pst_access;
    if(pst_ext_sni)
    {
        if (_flash_is_sni_header(au8_extMagicData, FLASH_EXT_SNI_HEADER_SIZE, pst_ext_sni->au8_magic))
        {
            pst_profile = &pst_ext_sni->st_profile;
            pst_access = &pst_profile->st_access;
            DRV_SPINAND_setup_access(pst_access->st_read.u8_command,
                                     pst_access->st_read.u8_dummy,
                                     pst_access->st_program.u8_command,
                                     pst_access->st_random.u8_command);
            if (pst_profile->u16_flags & SPINAND_NEED_QE)
            {
                if(!_flash_set_quad_mode(1))
                    spi_nand_msg("[FLASH_ERR] Enable quad mode failed\r\n");
            }
            DRV_SPINAND_setup_timeout(pst_profile->u32_maxWaitTime);
            _flash_unlock_whole_block(pst_ext_sni);
        }
    }
}

static u32 _flash_offset_to_page_address(u32 u32_bytes_offset, u32 u32_page_size)
{
    u32 u32_page_mask;
    u32_page_mask = u32_page_size - 1;
    return ((u32_bytes_offset & ~u32_page_mask) >> _flash_count_bits(u32_page_size));
}

static void _flash_wait_dev_ready(void)
{
    if (ERR_SPINAND_TIMEOUT == DRV_SPINAND_check_status())
    {
        printf("[FLASH_ERR] check status fail!\r\n");
    }
}

static u8 _flash_check_page_status(void)
{
    SPINAND_EXT_PROFILE_t *pst_profile;

    u8 u8_status;

    if (ERR_SPINAND_TIMEOUT != (u8_status = DRV_SPINAND_check_status()))
    {
        if (_flash_is_sni_header(au8_extMagicData, FLASH_EXT_SNI_HEADER_SIZE, g_pst_spinand_ext_info->au8_magic))
        {
            pst_profile = &g_pst_spinand_ext_info->st_profile;

            if (ERR_SPINAND_ECC_RESERVED == u8_status)
            {
                if (pst_profile->u16_flags & SPINAND_ECC_RESERVED_NONE_CORRECTED)
                {
                    return ERR_SPINAND_ECC_NOT_CORRECTED;
                }
                else if (pst_profile->u16_flags & SPINAND_ECC_RESERVED_CORRECTED)
                {
                    return ERR_SPINAND_ECC_CORRECTED;
                }
            }
        }
    }

    return u8_status;
}

static u16 _flash_offset_wrap(u32 u32_page_address)
{
    if (1 < g_pst_spinand_info->u8PlaneCnt)
    {
        if (0 < (u32_page_address & FLASH_PAGES_PER_BLOCK))
        {
            return (1 << 12);
        }
    }

    return 0;
}

static void _flash_die_sel(u32 u32_page_address)
{
    u32 u32_which_die;
    u32 u32_limit_page_address;

    if (g_pst_spinand_ext_info->st_profile.u16_flags & SPINAND_MULTI_DIES)
    {
        u32_which_die = 0;
        u32_limit_page_address = g_pst_spinand_ext_info->st_profile.st_extConfig.st_dieConfig.u32_dieSize / FLASH_PAGE_SIZE;

        u32_which_die = g_pst_spinand_ext_info->st_profile.st_extConfig.st_dieConfig.st_dieCode.u8_command;

        if (u32_limit_page_address <= u32_page_address)
        {
            u32_which_die |= (1 << 8);
        }

        if (ERR_SPINAND_SUCCESS != DRV_SPINAND_single_transfer((u8*)&u32_which_die, g_pst_spinand_ext_info->st_profile.st_extConfig.st_dieConfig.st_dieCode.u16_dataBytes + 1))
        {
            spi_nand_msg("[FLASH_ERR] Change die fail!");
        }
    }
}

static void _flash_otp_enable(u8 u8_enable)
{
    u8 u8_status = 0;
    SPINAND_OTP_t *pst_otp;

    pst_otp = &g_pst_spinand_ext_info->st_profile.st_extConfig.st_otp;

    do
    {
        if (ERR_SPINAND_SUCCESS !=DRV_SPINAND_get_features(pst_otp->st_otpEnabled.u32_address, &u8_status, pst_otp->st_otpEnabled.u16_dataBytes))
        {
            spi_nand_msg("get status register:0x%x failed\r\n",pst_otp->st_otpEnabled.u32_address);
            break;
        }
        if(u8_enable)
        {
            u8_status |= pst_otp->st_otpEnabled.u16_value;
        }
        else
        {
            u8_status &= ~(pst_otp->st_otpEnabled.u16_value);
        }

        if (ERR_SPINAND_SUCCESS != DRV_SPINAND_set_features(pst_otp->st_otpEnabled.u32_address, &u8_status, pst_otp->st_otpEnabled.u16_dataBytes))
        {
            spi_nand_msg("set status register:0x%x failed\r\n",pst_otp->st_otpEnabled.u32_address);
        }
    } while (0);
}

static void _flash_otp_lock(u8 u8_enable)
{
    u8 u8_status;

    SPINAND_OTP_t *pst_otp;

    pst_otp = &g_pst_spinand_ext_info->st_profile.st_extConfig.st_otp;

    do
    {
        if (ERR_SPINAND_SUCCESS !=DRV_SPINAND_get_features(pst_otp->st_otpLock.u32_address, &u8_status, pst_otp->st_otpLock.u16_dataBytes))
        {
            spi_nand_msg("get status register:0x%x failed",pst_otp->st_otpLock.u32_address);
            break;
        }

        if (u8_enable)
        {
            u8_status |= pst_otp->st_otpLock.u16_value;
        }
        else
        {
            spi_nand_msg("just for reading the register status=0x%x,can't unlock the otp\r\n",u8_status);
            u8_status &= (~pst_otp->st_otpLock.u16_value);
        }

        if (ERR_SPINAND_SUCCESS != DRV_SPINAND_set_features(pst_otp->st_otpLock.u32_address, &u8_status, pst_otp->st_otpLock.u16_dataBytes))
        {
            spi_nand_msg("set status register:0x%x failed",pst_otp->st_otpLock.u32_address);
            break;
        }

        if (ERR_SPINAND_SUCCESS != DRV_SPINAND_cmd_program_execute(0))
        {
            spi_nand_msg("DRV_SPINAND_cmd_program_execute fail");
        }
    } while (0);

}

static void _flash_do_ecc(u8 u8_enable)
{
    u8 u8Status;

    FLASH_CMD_SET_t *pst_eccEnable;
    pst_eccEnable = &g_pst_spinand_ext_info->st_profile.st_extConfig.st_eccEnabled;

    do
    {
        if (ERR_SPINAND_SUCCESS != DRV_SPINAND_get_features(pst_eccEnable->u32_address, &u8Status, pst_eccEnable->u16_dataBytes))
        {
            spi_nand_msg("get status register:0x%x failed.", pst_eccEnable->u32_address);
            break;
        }

        if(u8_enable)
        {
            u8Status |= pst_eccEnable->u16_value;
        }
        else
        {
            u8Status &= ~pst_eccEnable->u16_value;
        }

        if (ERR_SPINAND_SUCCESS != DRV_SPINAND_set_features(pst_eccEnable->u32_address, &u8Status, pst_eccEnable->u16_dataBytes))
        {
            spi_nand_msg("set status register:0x%x failed.", pst_eccEnable->u32_address);
            break;
        }

        u8Status = 0;

        if (ERR_SPINAND_SUCCESS != DRV_SPINAND_get_features(pst_eccEnable->u32_address, &u8Status, pst_eccEnable->u16_dataBytes))
        {
            spi_nand_msg("get status register:0x%x failed.", pst_eccEnable->u32_address);
            break;
        }

        spi_nand_msg("after ECC engine: 0x%x.", u8Status);
    } while (0);
}

static int _flash_read_from_cache(u32 u32_offset, u32 u32_address, u32 u32_size)
{
    if (ERR_SPINAND_SUCCESS != DRV_SPINAND_read_from_cache(u32_offset, (u8*)u32_address, u32_size))
    {
        spi_nand_msg("[FLASH_ERR] Read data from page fail!");
        return 0;
    }

    return u32_size;
}

static u32 _flash_read_oob(u32 u32_page_address, u16 u16_oob_offset, u32 u32_address, u32 u32_size)
{
    if (ERR_SPINAND_SUCCESS != DRV_SPINAND_page_read(u32_page_address))
    {
        spi_nand_msg("[FLASH_ERR] Page read fail!\r\n");
    }

    _flash_wait_dev_ready();

    return _flash_read_from_cache(u16_oob_offset, u32_address, u32_size);;
}

static u8 _flash_is_bad(u32 u32_page_address, u16 u16_oob_offset)
{
    u8 u8_bad;

    u8_bad = 0x00;

    if (1 == _flash_read_oob(u32_page_address, u16_oob_offset, (u32)&u8_bad, 1))
    {
        if (0xFF == u8_bad)
        {
            return 0;
        }

        printf("[FLASH] bad block @ page%d, oob_offset = 0x%x, oob = 0x%x.\r\n", u32_page_address, u16_oob_offset, u8_bad);
    }

    return 1;
}

static int _flash_check_sni_available(u32 u32_address)
{
    u8 u8_i;
    u32 u32_page_size;
    u32 u32_block_size;
    u32 u32_page_address;

    u32_page_size = FLASH_2K_PAGE_SIZE;
    u32_block_size = FLASH_2K_BLOCK_SIZE;

reLoadSNI:
    for (u8_i = 0; FLASH_SNI_BLOCKS_SIZE > u8_i; u8_i += 2)
    {
        u32_page_address = _flash_offset_to_page_address(u32_block_size * u8_i, u32_page_size);

        if (!_flash_is_bad(u32_page_address, u32_page_size))
        {
            if (ERR_SPINAND_ECC_NOT_CORRECTED != _flash_check_page_status())
            {
                _flash_read_from_cache(0, u32_address, FLASH_SNI_TABLE_SIZE);

                if (_flash_is_sni_header(au8_magicData, FLASH_SNI_HEADER_SIZE, (u8*)u32_address))
                {
                    printf("[FLASH] Found SNI in block %d.\r\n", u8_i);
                    return 1;
                }
            }
        }

    }

    if (FLASH_4K_PAGE_SIZE != u32_page_size)
    {
        u32_page_size = FLASH_4K_PAGE_SIZE;
        u32_block_size = FLASH_4K_BLOCK_SIZE;
        goto reLoadSNI;
    }

    printf("[FLASH] Not found device id in SNI\r\n");

    return 0;
}

static u32 _flash_find_sni(u32 u32_address)
{
    SPINAND_INFO_t *pst_info;
    while (_flash_is_sni_header(au8_magicData, FLASH_SNI_HEADER_SIZE, (u8*)u32_address))
    {
        pst_info = (SPINAND_INFO_t*)(u32_address + FLASH_SNI_HEADER_SIZE);
        if (_flash_is_id_match(pst_info->au8_ID, pst_info->u8_IDByteCnt))
        {
            return u32_address;
        }
        u32_address+= 512;
    }
    return 0;
}

uint8_t	spi_nand_read_byte(struct mtd_info *mtd)
{
    u8 u8_byte;
    DRV_SPINAND_receive_data(&u8_byte, 1);
    return u8_byte;
}

u16 spi_nand_read_word(struct mtd_info *mtd)
{
    u16 u16_word;
    DRV_SPINAND_receive_data((u8*)&u16_word, 2);
    return u16_word;
}

void spi_nand_write_buf(struct mtd_info *mtd, const uint8_t *buf, int len)
{
    DRV_SPINAND_burst_transfer((u8*)buf, len);
}

void spi_nand_read_buf(struct mtd_info *mtd, uint8_t *buf, int len)
{
    DRV_SPINAND_receive_data(buf, len);
}

void spi_nand_select_chip(struct mtd_info *mtd, int chip)
{
    //printf("spi_nand_select_chip  Not support\r\n");
}

void spi_nand_cmd_ctrl(struct mtd_info *mtd, int dat, unsigned int ctrl)
{
    spi_nand_debug("spi_nand_cmd_ctrl Not support\r\n");
}

int spi_nand_dev_ready(struct mtd_info *mtd)
{
    spi_nand_debug("spi_nand_dev_ready Not support\r\n");

    return 1;
}


void spi_nand_cmdfunc(struct mtd_info *mtd, unsigned command, int column, int page_addr)
{
    struct nand_chip *chip;
    chip = (struct nand_chip*)mtd->priv;

    switch (command)
    {
        case NAND_CMD_STATUS:
            spi_nand_debug("NAND_CMD_STATUS");
            DRV_SPINAND_cmd_read_status_register();
            break;

        case NAND_CMD_PAGEPROG:
            spi_nand_debug("NAND_CMD_PAGEPROG");
            _flash_die_sel(chip->pagebuf);
            DRV_SPINAND_cmd_program_execute(chip->pagebuf);
            chip->pagebuf = -1;
            break;

        case NAND_CMD_READOOB:
            spi_nand_debug("NAND_CMD_READOOB page_addr: 0x%x", page_addr);
            _flash_die_sel(page_addr);
            if (ERR_SPINAND_TIMEOUT != DRV_SPINAND_page_read_with_status(page_addr))
            {
                DRV_SPINAND_cmd_normal_read_from_cache(_flash_offset_wrap(page_addr) | (mtd->writesize + column));
            }
            break;

        case NAND_CMD_READID:
            spi_nand_debug("NAND_CMD_READID\r\n");
            DRV_SPINAND_cmd_read_id();
            break;

        case NAND_CMD_ERASE2:
            spi_nand_debug("NAND_CMD_ERASE2\r\n");
            break;

        case NAND_CMD_ERASE1:
            spi_nand_debug("NAND_CMD_ERASE1, page_addr: 0x%x", page_addr);
            _flash_die_sel(page_addr);
            DRV_SPINAND_block_erase(page_addr);
            break;
        case NAND_CMD_READ0:
            spi_nand_debug("NAND_CMD_READ0 page = %d", page_addr);
            _flash_die_sel(page_addr);
            DRV_SPINAND_page_read(page_addr);
            break;
        case NAND_CMD_SEQIN:
            spi_nand_debug("NAND_CMD_SEQIN");
            chip->pagebuf = page_addr;
            DRV_SPINAND_cmd_program_load(column | _flash_offset_wrap(page_addr));
            break;
        case NAND_CMD_RESET:
            spi_nand_debug("NAND_CMD_RESET");
            DRV_SPINAND_reset_status();
            break;
        case NAND_CMD_OTP_ENABLE:
            spi_nand_debug("NAND_CMD_OTP_ENABLE");
            _flash_otp_enable(column);
            break;
        case NAND_CMD_OTP_LOCK:
            spi_nand_debug("NAND_CMD_OTP_LOCK");
            _flash_otp_lock(column);
            break;
        case NAND_CMD_DO_ECC:
            spi_nand_debug("NAND_CMD_DO_ECC");
            _flash_do_ecc(column);
            break;
        case NAND_CMD_DETECT:
            spi_nand_debug("NAND_CMD_DETECT");//for empty chip upgrade
            if (0 != (page_addr = _flash_find_sni(column)))
            {
                if (_flash_is_sni_header(au8_magicData, FLASH_SNI_HEADER_SIZE, (u8*)page_addr))
                {
                    memcpy((void*)g_u32_sni_address, (void*)page_addr, 512);
                    nand_curr_device = -1;
                    nand_init();
                }
            }
            break;
        default:
            spi_nand_msg("unsupported command %02Xh\n", command);
            break;
    }
    return;
}

int spi_nand_waitfunc(struct mtd_info *mtd, struct nand_chip *this)
{
    spi_nand_debug("spi_nand_waitfunc\r\n");
    u8 u8_status;
    SPINAND_EXT_INFO_t *pst_spinand_ext_info = (SPINAND_EXT_INFO_t *)this->priv;

    if (FL_ERASING == this->state)
    {
        DRV_SPINAND_setup_timeout(400000);
    }

    u8_status = DRV_SPINAND_check_status();

    DRV_SPINAND_setup_timeout(pst_spinand_ext_info->st_profile.u32_maxWaitTime);

    return (ERR_SPINAND_SUCCESS == u8_status)? NAND_STATUS_READY : NAND_STATUS_FAIL;
}

void spi_nand_ecc_hwctl(struct mtd_info *mtd, int mode)
{
    spi_nand_debug(" spi_nand_ecc_hwctl Not support");
}

int spi_nand_ecc_calculate(struct mtd_info *mtd, const uint8_t *dat, uint8_t *ecc_code)
{
    spi_nand_debug("spi_nand_ecc_calculate Not support");
    return 0;
}

int spi_nand_ecc_correct(struct mtd_info *mtd, uint8_t *dat, uint8_t *read_ecc, uint8_t *calc_ecc)
{
    spi_nand_debug(" spi_nand_ecc_correct Not support");
    return 0;
}

int spi_nand_ecc_read_page_raw(struct mtd_info *mtd, struct nand_chip *chip,
    uint8_t *buf, int oob_required, int page)
{
    spi_nand_debug("page = 0x%0x", page);
    _flash_wait_dev_ready();
    _flash_read_from_cache((0 | _flash_offset_wrap(page)), (u32)buf, mtd->writesize);

    if (oob_required)
    {
        _flash_read_from_cache((mtd->writesize | _flash_offset_wrap(page)), (u32)chip->oob_poi, mtd->oobsize);
    }

    return 0;
}

int spi_nand_ecc_write_page_raw(struct mtd_info *mtd, struct nand_chip *chip, const uint8_t *buf, int oob_required)
{
    DRV_SPINAND_program_load_data(buf, mtd->writesize);

    if (oob_required)
        DRV_SPINAND_random_program_load(mtd->writesize | _flash_offset_wrap(chip->pagebuf), chip->oob_poi, mtd->oobsize);

    return 0;
}


int spi_nand_ecc_read_page(struct mtd_info *mtd, struct nand_chip *chip,
    uint8_t *buf, int oob_required, int page)
{
    u8 u8_status;
    spi_nand_debug("page = 0x%0x, buf = 0x%0x", page, buf);

    if (ERR_SPINAND_ECC_NOT_CORRECTED != (u8_status = _flash_check_page_status()))
    {
        do
        {
            if (mtd->writesize != _flash_read_from_cache((0 | _flash_offset_wrap(page)), (u32)buf, mtd->writesize))
            {
                break;
            }

            if (ERR_SPINAND_ECC_CORRECTED == u8_status)
            {
                mtd->ecc_stats.corrected++;
            }

            if (oob_required)
            {
                if (mtd->oobsize != _flash_read_from_cache((mtd->writesize | _flash_offset_wrap(page)), (u32)chip->oob_poi, mtd->oobsize))
                {
                    break;
                }
            }

            return 0;
        } while (0);
    }

    mtd->ecc_stats.failed++;
    return -EIO;
}

int spi_nand_ecc_read_subpage(struct mtd_info *mtd, struct nand_chip *chip,
    uint32_t offs, uint32_t len, uint8_t *buf, int page)
{
    u8 u8_status;
    spi_nand_debug("page = 0x%0x, offs = 0x%0x, len = 0x%x", page, offs, len);
    if (ERR_SPINAND_ECC_NOT_CORRECTED != (u8_status = _flash_check_page_status()))
    {
        do
        {
            if (len != _flash_read_from_cache((offs | _flash_offset_wrap(page)), (u32)buf + offs, len))
            {
                break;
            }

            if (ERR_SPINAND_ECC_CORRECTED == u8_status)
            {
                mtd->ecc_stats.corrected++;
            }

            return 0;
        } while (0);
    }

    mtd->ecc_stats.failed++;
    return -EIO;
}

int spi_nand_ecc_write_subpage(struct mtd_info *mtd, struct nand_chip *chip,
                               uint32_t offset, uint32_t data_len,
                               const uint8_t *data_buf, int oob_required)
{
    do
    {
        if (ERR_SPINAND_SUCCESS != DRV_SPINAND_random_program_load(offset | _flash_offset_wrap(chip->pagebuf), (u8*)data_buf, data_len))
        {
            break;
        }

        if (oob_required)
        {
            if (ERR_SPINAND_SUCCESS != DRV_SPINAND_random_program_load((mtd->writesize | _flash_offset_wrap(chip->pagebuf)), chip->oob_poi, mtd->oobsize))
            {
                break;
            }
        }

        return 0;
    } while (0);

    return -EIO;
}

int spi_nand_ecc_write_page(struct mtd_info *mtd, struct nand_chip *chip, const uint8_t *buf, int oob_required)
{
    spi_nand_debug("spi_nand_ecc_write_page\r\n");
    do
    {
        if (ERR_SPINAND_SUCCESS != DRV_SPINAND_program_load_data(buf, mtd->writesize))
        {
            break;
        }

        if (oob_required)
        {
            if (ERR_SPINAND_SUCCESS != DRV_SPINAND_random_program_load((mtd->writesize | _flash_offset_wrap(chip->pagebuf)), chip->oob_poi, mtd->oobsize))
            {
                break;
            }
        }

        return 0;
    } while (0);

    return -EIO;
}

int board_nand_init(struct nand_chip *nand)
{
    u8 u8_dev_id;
    u32 u32_flash_size;

    struct nand_flash_dev *type;
    do
    {
        if (ERR_SPINAND_SUCCESS != DRV_SPINAND_init())
        {
            printf("[FLASH_ERR] init fail!\r\n");
            break;
        }
        if (0 == g_u32_sni_address)
        {
            if (0 == (g_u32_sni_address = (u32)calloc(1, FLASH_SNI_TABLE_SIZE)))
            {
                break;
            }
        }

        g_pst_spinand_info = (SPINAND_INFO_t*)(g_u32_sni_address + sizeof(au8_magicData));
        g_pst_spinand_ext_info = (SPINAND_EXT_INFO_t*)(g_u32_sni_address + FLASH_LEAGCY_SNI_SIZE);
        if (!_flash_is_sni_header(au8_magicData, FLASH_SNI_HEADER_SIZE, (u8*)g_u32_sni_address))
        {
            if (!_flash_check_sni_available((u32)g_u32_sni_address))
            {
                g_pst_spinand_info->au8_ID[0] = 0xEE;
                g_pst_spinand_info->au8_ID[1] = 0xEE;
                g_pst_spinand_info->au8_ID[2] = 0x01;
                g_pst_spinand_info->au8_ID[3] = 0x00;
                g_pst_spinand_info->au8_ID[4] = 0x06;
                g_pst_spinand_info->u8_IDByteCnt = 5;
                u32_flash_size = 0x3600;
                g_pst_spinand_info->u16_PageByteCnt = 0x800;
                g_pst_spinand_info->u16_SpareByteCnt = 0x40;
                g_pst_spinand_info->u16_BlkPageCnt = 0x40;
                g_pst_spinand_info->u16_SectorByteCnt = 0x40;
            }
        }

        u32_flash_size = g_pst_spinand_info->u16_BlkPageCnt * g_pst_spinand_info->u16_PageByteCnt * g_pst_spinand_info->u16_BlkCnt;

        u8_dev_id = 0xEE;

        for (type = nand_flash_ids; type->name != NULL; type++)
        {
		    if (u8_dev_id == type->dev_id)
            {
                printf("[FLASH] dev_id = 0x%x\r\n", type->dev_id);
                type->mfr_id = g_pst_spinand_info->au8_ID[0];
                type->dev_id = g_pst_spinand_info->au8_ID[1];
                type->id_len = g_pst_spinand_info->u8_IDByteCnt;
                strncpy((char*)type->id, (const char*)g_pst_spinand_info->au8_ID, g_pst_spinand_info->u8_IDByteCnt);
                type->chipsize = u32_flash_size >> 20;
                type->pagesize = g_pst_spinand_info->u16_PageByteCnt;
                type->oobsize = g_pst_spinand_info->u16_SpareByteCnt;
                type->erasesize = g_pst_spinand_info->u16_BlkPageCnt * g_pst_spinand_info->u16_PageByteCnt;
                type->ecc.strength_ds = g_pst_spinand_info->u16_SectorByteCnt;
                type->ecc.step_ds = g_pst_spinand_info->u16_PageByteCnt / g_pst_spinand_info->u16_SectorByteCnt;
                printf("[FLASH] mfr_id = 0x%x, dev_id= 0x%x id_len = 0x%x\r\n", type->id[0], type->id[1], type->id_len);
                break;
            }
        }
        _flash_setup_by_sni(g_pst_spinand_ext_info);

        if (0 == g_pst_spinand_info->U8RIURead)
        {
            printf("[FLASH] Use BDMA.\r\n");
            DRV_SPINAND_use_bdma(1);
        }

        nand->options = NAND_BROKEN_XD | NAND_SUBPAGE_READ | NAND_SKIP_BBTSCAN | NAND_NO_SUBPAGE_WRITE;
        nand->read_byte = spi_nand_read_byte;
        nand->read_word = spi_nand_read_word;
        nand->write_buf = spi_nand_write_buf;
        nand->read_buf = spi_nand_read_buf;

        nand->select_chip = spi_nand_select_chip;
        nand->cmd_ctrl = spi_nand_cmd_ctrl;
        nand->dev_ready = spi_nand_dev_ready;
        nand->cmdfunc = spi_nand_cmdfunc;
        nand->waitfunc = spi_nand_waitfunc;

        nand->bits_per_cell = 1;
        nand->chip_delay = 0;
        nand->bbt_options = NAND_BBT_USE_FLASH;
        nand->ecc.mode = NAND_ECC_HW;
        nand->ecc.size = g_pst_spinand_info->u16_SectorByteCnt;
        nand->ecc.hwctl = spi_nand_ecc_hwctl;
        nand->ecc.calculate = spi_nand_ecc_calculate;
        nand->ecc.correct = spi_nand_ecc_correct;
        nand->ecc.read_page_raw = spi_nand_ecc_read_page_raw;
        nand->ecc.write_page_raw = spi_nand_ecc_write_page_raw;
        nand->ecc.read_page = spi_nand_ecc_read_page;
        nand->ecc.read_subpage = spi_nand_ecc_read_subpage;
        nand->ecc.write_page = spi_nand_ecc_write_page;
        nand->ecc.write_subpage = spi_nand_ecc_write_subpage;
		nand->priv = g_pst_spinand_ext_info;
        return 0;
    } while (0);

    if (0 != g_u32_sni_address)
    {
        free((void*)g_u32_sni_address);
    }
    return -1;

}


