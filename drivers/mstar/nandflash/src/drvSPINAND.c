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

#include <drvSPINAND.h>
#include <drvFSP_QSPI.h>
#include <halFSP_QSPI.h>
#include <mdrvSNI.h>
#include <hal_bdma.h>

extern void invalidate_dcache_range(unsigned long start, unsigned long stop);
extern void flush_dcache_range(unsigned long start, unsigned long stop);

#define SPI_NAND_CMD_RFC_DUMMY                  0x08

DRV_SPINAND_INFO_t st_spinand_drv_info;

static u8 (*_DRV_SPINAND_read_from_cache)(u16, u8*, u32);
static u8 (*_DRV_SPINAND_program_load_data)(u8*, u32);

static u8 _DRV_SPINAND_is_quad_cmd(u8 u8_cmd)
{
    switch (u8_cmd)
    {
        case SPI_NAND_CMD_QPL:
        case 0x6B:
        case SPI_NAND_CMD_QRPL:
        case 0xEB:
            return 1;
        default:
            return 0;
    }
}

u8 DRV_SPINAND_receive_data(u8* pu8_buf, u32 u32_size)
{
    u8 u8_rd_size;

    DRV_FSP_init(FSP_USE_SINGLE_CMD);
    DRV_QSPI_use_sw_cs(1);

    while (0 != u32_size)
    {
        DRV_FSP_set_which_cmd_size(1, 0);
        u8_rd_size = DRV_FSP_set_rbf_size_after_which_cmd(1, u32_size);

        DRV_FSP_trigger();

        if (!DRV_FSP_is_done())
        {
            break;
        }

        u32_size -= DRV_FSP_read_rbf(pu8_buf, u8_rd_size);
        pu8_buf += u8_rd_size;

        DRV_FSP_clear_trigger();
    }

    DRV_QSPI_use_sw_cs(0);

    if (0 != u32_size)
    {
        return ERR_SPINAND_TIMEOUT;
    }
	
    return ERR_SPINAND_SUCCESS;
}

u8 DRV_SPINAND_burst_transfer(u8* pu8_buf, u32 u32_size)
{
    u8 u8_wr_size;
	
    DRV_QSPI_use_sw_cs(1);

    while (0 != u32_size)
    {
        DRV_FSP_init(FSP_USE_SINGLE_CMD);
        u8_wr_size = DRV_FSP_write_wbf(pu8_buf, u32_size);
        u32_size -= u8_wr_size;
        pu8_buf += u8_wr_size;
        DRV_FSP_set_which_cmd_size(1, u8_wr_size);

        DRV_FSP_trigger();

        if (!DRV_FSP_is_done())
        {
        	break;
        }

        DRV_FSP_clear_trigger();
    }
	
    DRV_QSPI_use_sw_cs(0);

    if (0 != u32_size)
    {
        return ERR_SPINAND_TIMEOUT;
    }
	
    return ERR_SPINAND_SUCCESS;
}

u8 DRV_SPINAND_single_transfer(u8* pu8_buf, u32 u32_size)
{
    u8 u8_wr_size;

    DRV_FSP_init(FSP_USE_SINGLE_CMD);
    u8_wr_size = DRV_FSP_write_wbf(pu8_buf, u32_size);

    if (u32_size != u8_wr_size)
    {
        return ERR_SPINAND_INVALID;
    }

    DRV_FSP_set_which_cmd_size(1, u8_wr_size);

    DRV_FSP_trigger();

    if (!DRV_FSP_is_done())
    {
        return ERR_SPINAND_TIMEOUT;
    }

    DRV_FSP_clear_trigger();

    return ERR_SPINAND_SUCCESS;
}

u8 DRV_SPINAND_reset(void)
{
    u8 u8_cmd;
    u8_cmd = SPI_NAND_CMD_RESET;

    return DRV_SPINAND_single_transfer(&u8_cmd, 1);
}

u8 DRV_SPINAND_reset_status(void)
{
    if (ERR_SPINAND_SUCCESS != DRV_SPINAND_reset())
    {
        return ERR_SPINAND_TIMEOUT;
    }

    return DRV_SPINAND_check_status();
}

u8 DRV_SPINAND_write_enable(void)
{
    u8 u8_cmd;
    u8_cmd = SPI_NAND_CMD_WREN;

    return DRV_SPINAND_single_transfer(&u8_cmd, 1);
}

u8 DRV_SPINAND_write_disable(void)
{
    u8 u8_cmd;
    u8_cmd = SPI_NAND_CMD_WRDIS;

    return DRV_SPINAND_single_transfer(&u8_cmd, 1);
}

u8 DRV_SPINAND_get_features(u8 u8_address, u8 *pu8_data, u8 u8_size)
{
    u16 u16_cmd;

    u16_cmd = SPI_NAND_CMD_GF | (u8_address << 8);

    if (ERR_SPINAND_SUCCESS != DRV_SPINAND_single_transfer((u8*)&u16_cmd, 2))
    {
        return ERR_SPINAND_DEVICE_FAILURE;
    }

    return DRV_SPINAND_receive_data(pu8_data, u8_size);
}

u8 DRV_SPINAND_check_status(void)
{
    u32 u32_timeout;
    u32 u32_status;

    u32_timeout = st_spinand_drv_info.u32_maxWaitTime;

    do
    {
        u32_status = 0;

        if (ERR_SPINAND_SUCCESS != DRV_SPINAND_get_features(SPI_NAND_REG_STAT, ((u8*)&u32_status), 1))
        {
            return ERR_SPINAND_DEVICE_FAILURE;
        }

        udelay(10);

        if (10 < u32_timeout)
        {
            u32_timeout -= 10;
        }
        else
        {
            u32_timeout = 0;
        }

        if (0 == u32_timeout)
        {
            return ERR_SPINAND_TIMEOUT;
        }

    } while (u32_status & SPI_NAND_STAT_OIP);

    if (u32_status & SPI_NAND_STAT_P_FAIL)
    {
        return ERR_SPINAND_P_FAIL;
    }

    if (u32_status & SPI_NAND_STAT_E_FAIL)
    {
        return ERR_SPINAND_E_FAIL;
    }

    u32_status &= ECC_STATUS_MASK;

    if (0 < u32_status)
    {
        if (ECC_NOT_CORRECTED == u32_status)
        {
            return ERR_SPINAND_ECC_NOT_CORRECTED;
        }

        if (ECC_STATUS_RESERVED == u32_status)
        {
            return ERR_SPINAND_ECC_RESERVED;
        }
        return ERR_SPINAND_ECC_CORRECTED;
    }
    return ERR_SPINAND_SUCCESS;
}

u8 DRV_SPINAND_set_features(u8 u8_address, u8 *pu8_data, u8 u8_size)
{
    u16 u16_cmd;

    u16_cmd = SPI_NAND_CMD_SF | (u8_address << 8);

    if (ERR_SPINAND_SUCCESS != DRV_SPINAND_single_transfer((u8*)&u16_cmd, 2))
    {
        return ERR_SPINAND_DEVICE_FAILURE;
    }

    return DRV_SPINAND_burst_transfer(pu8_data, u8_size);
}

u8 DRV_SPINAND_cmd_read_id(void)
{
    u16 u16_cmd;

    u16_cmd = SPI_NAND_CMD_RDID | (0x00 << 8);

    return DRV_SPINAND_single_transfer((u8*)&u16_cmd, 2);
}

u8 DRV_SPINAND_read_id(u8 *pu8_ids, u8 u8_bytes)
{
    if (ERR_SPINAND_SUCCESS != DRV_SPINAND_cmd_read_id())
    {
        return ERR_SPINAND_DEVICE_FAILURE;
    }

    return DRV_SPINAND_receive_data(pu8_ids, u8_bytes);
}

u8 DRV_SPINAND_page_read(u32 u32_row_address)
{
    u32 u32_cmd;

    u32_cmd = SPI_NAND_CMD_PGRD | (((u32_row_address >> 16) & 0xff) << 8) | (((u32_row_address >> 8) & 0xff) << 16) | ((u32_row_address & 0xff) << 24);

    return DRV_SPINAND_single_transfer((u8*)&u32_cmd, 4);
}

u8 DRV_SPINAND_page_read_with_status(u32 u32_row_address)
{
    if (ERR_SPINAND_SUCCESS != DRV_SPINAND_page_read(u32_row_address))
    {
        return ERR_SPINAND_DEVICE_FAILURE;
    }

    return DRV_SPINAND_check_status();
}

static u8 _DRV_SPINAND_read_from_cache_by_riu(u16 u16_col_address, u8 *pu8_data, u32 u32_size)
{
    u8 au8_cmd[2];
    u8 u8_dummy;
    u8 u8_wr_size = 0;

    au8_cmd[0] = (u16_col_address >> 8) & 0xff;
    au8_cmd[1] = u16_col_address & 0xff;

    if (ERR_SPINAND_SUCCESS != DRV_SPINAND_single_transfer(&st_spinand_drv_info.u8_rfc, 1))
    {
        return ERR_SPINAND_DEVICE_FAILURE;
    }

    u8_dummy = (st_spinand_drv_info.u8_dummy * 1) / 8;

    if (0xEB == st_spinand_drv_info.u8_rfc)
    {
        DRV_FSP_use_quad_mode(1);
        u8_dummy = (st_spinand_drv_info.u8_dummy * 4) / 8;
    }

    if (ERR_SPINAND_SUCCESS != DRV_SPINAND_burst_transfer(au8_cmd, 2))
    {
        return ERR_SPINAND_DEVICE_FAILURE;
    }

    while (0 != u8_dummy)
    {
        au8_cmd[u8_wr_size++] = 0x0;
        u8_dummy--;

        if (0 == u8_dummy || u8_wr_size == sizeof(au8_cmd))
        {
            if (ERR_SPINAND_SUCCESS != DRV_SPINAND_burst_transfer(au8_cmd, u8_wr_size))
            {
                return ERR_SPINAND_DEVICE_FAILURE;
            }

            u8_wr_size = 0;
        }
    }

    DRV_FSP_use_quad_mode(_DRV_SPINAND_is_quad_cmd(st_spinand_drv_info.u8_rfc));

    if (ERR_SPINAND_SUCCESS != DRV_SPINAND_receive_data(pu8_data, u32_size))
    {
        return ERR_SPINAND_DEVICE_FAILURE;
    }

    DRV_FSP_use_quad_mode(0);
    return ERR_SPINAND_SUCCESS;
}

static u8 _DRV_SPINAND_read_from_cache_by_bdma(u16 u16_col_address, u8 *pu8_data, u32 u32_size)
{
    HalBdmaParam_t tBdmaParam;

    invalidate_dcache_range((u32)pu8_data, (u32)pu8_data + u32_size);
#ifdef SPI_TO_MIU_PATCH
    u32 u32_remainder = 0;
    if (0 != (u16_col_address % 8))
    {
        u32_remainder = 8 -(u16_col_address % 8);

        if (u32_size < u32_remainder)
        {
            u32_remainder = u32_size;
        }

        if (ERR_SPINAND_SUCCESS != _DRV_SPINAND_read_from_cache_by_riu(u16_col_address, pu8_data, u32_remainder))
        {
            return ERR_SPINAND_DEVICE_FAILURE;
        }

        u16_col_address += u32_remainder;
        pu8_data += u32_remainder;
        u32_size -= u32_remainder;
    }
#endif
    if (0 < u32_size)
    {
        DRV_QSPI_use_2bytes_address_mode(DRV_QSPI_cmd_to_mode(st_spinand_drv_info.u8_rfc), st_spinand_drv_info.u8_dummy);

        memset(&tBdmaParam, 0, sizeof(HalBdmaParam_t));
        tBdmaParam.bIntMode          = 0; //0:use polling mode
        tBdmaParam.ePathSel          = HAL_BDMA_SPI_TO_MIU0;
        tBdmaParam.eSrcDataWidth     = HAL_BDMA_DATA_BYTE_8;
        tBdmaParam.eDstDataWidth     = HAL_BDMA_DATA_BYTE_16;
        tBdmaParam.eDstAddrMode      = HAL_BDMA_ADDR_INC; //address increase
        tBdmaParam.u32TxCount        = u32_size;
        tBdmaParam.u32Pattern        = 0;
        tBdmaParam.pSrcAddr          = (void*)(u32)u16_col_address;
        tBdmaParam.pDstAddr          = (void*)(u32)pu8_data;
        tBdmaParam.pfTxCbFunc        = NULL;

        if (HAL_BDMA_PROC_DONE != HalBdma_DoTransfer(HAL_BDMA_CH0, &tBdmaParam))
        {
            return ERR_SPINAND_BDMA_FAILURE;
        }

        DRV_QSPI_use_2bytes_address_mode(DRV_QSPI_cmd_to_mode(SPI_NAND_CMD_RFC), SPI_NAND_CMD_RFC_DUMMY);
    }

    return ERR_SPINAND_SUCCESS;
}


u8 DRV_SPINAND_cmd_program_execute(u32 u32_row_address)
{
    u32 u32_cmd;

    u32_cmd = 0;
    u32_cmd = SPI_NAND_CMD_PE | (((u32_row_address >> 16) & 0xff) << 8) | ((u32_row_address >> 8) & 0xff) << 16 | (u32_row_address & 0xff) << 24;

    return DRV_SPINAND_single_transfer((u8*)&u32_cmd, 4);
}

u8 DRV_SPINAND_cmd_normal_read_from_cache(u16 u16_col_address)
{
    u8 au8_cmd[4];

    au8_cmd[0] = SPI_NAND_CMD_RFC;
    au8_cmd[1] = u16_col_address >> 8;
    au8_cmd[2] = u16_col_address;
    au8_cmd[3] = 0x00; //dummy cycle

    return DRV_SPINAND_single_transfer(au8_cmd, 4);
}

u8 DRV_SPINAND_cmd_read_status_register(void)
{
    u16 u16_cmd;

    u16_cmd = SPI_NAND_CMD_GF | (SPI_NAND_REG_STAT << 8);

    return DRV_SPINAND_single_transfer((u8*)&u16_cmd, 2);
}

u8 DRV_SPINAND_read_from_cache(u16 u16_col_address, u8 *pu8_data, u32 u32_size)
{
    if (0 != (((u32)pu8_data) % 64) || 0 != (u32_size % 64))
    {
        return _DRV_SPINAND_read_from_cache_by_riu(u16_col_address, pu8_data, u32_size);
    }
    return _DRV_SPINAND_read_from_cache(u16_col_address, pu8_data, u32_size);
}

u8 DRV_SPINAND_block_erase(u32 u32_row_address)
{
    u8 au8_cmd[4];

    if (ERR_SPINAND_SUCCESS != DRV_SPINAND_write_enable())
    {
        return ERR_SPINAND_DEVICE_FAILURE;
    }

    au8_cmd[0] = SPI_NAND_CMD_BE;
    au8_cmd[1] = (u32_row_address >> 16) & 0xff;
    au8_cmd[2] = (u32_row_address >> 8) & 0xff;
    au8_cmd[3] = u32_row_address & 0xff;

    return DRV_SPINAND_single_transfer(au8_cmd, 4);
}

static u8 _DRV_SPINAND_program_load_data_by_bdma(u8* pu8_data, u32 u32_size)
{
	u8 u8_status = ERR_SPINAND_SUCCESS;
	
    HalBdmaParam_t    tBdmaParam;

	if (0 != ((u32)pu8_data & 0x3f) || 0 != ((u32)u32_size & 0x3f))
	{
		if (ERR_SPINAND_SUCCESS != DRV_SPINAND_burst_transfer(pu8_data, u32_size))
		{
			return ERR_SPINAND_DEVICE_FAILURE;
		}

		return ERR_SPINAND_SUCCESS;
	}
	
	flush_dcache_range((unsigned long)pu8_data, (unsigned long)pu8_data + u32_size);
	
    DRV_QSPI_use_sw_cs(1);
	DRV_FSP_enable_outside_wbf(0, 0, u32_size);
    DRV_FSP_init(FSP_USE_SINGLE_CMD);
    DRV_FSP_set_which_cmd_size(1, 0);
	
	do
	{
		memset(&tBdmaParam, 0, sizeof(HalBdmaParam_t));
	    tBdmaParam.bIntMode          = 0; //0:use polling mode
	    tBdmaParam.ePathSel          = HAL_BDMA_MIU0_TO_SPI;
	    tBdmaParam.eSrcDataWidth     = HAL_BDMA_DATA_BYTE_16;
	    tBdmaParam.eDstDataWidth     = HAL_BDMA_DATA_BYTE_8;
	    tBdmaParam.eDstAddrMode      = HAL_BDMA_ADDR_INC; //address increase
	    tBdmaParam.u32TxCount        = u32_size;
	    tBdmaParam.u32Pattern        = 0;
	    tBdmaParam.pSrcAddr          = (void*)pu8_data;
	    tBdmaParam.pDstAddr          = 0;
	    tBdmaParam.pfTxCbFunc        = NULL;

	    DRV_FSP_trigger();

	    if (HAL_BDMA_PROC_DONE != HalBdma_DoTransfer(HAL_BDMA_CH0, &tBdmaParam))
	    {
	        u8_status = ERR_SPINAND_BDMA_FAILURE;
			break;
	    }

	    if (!DRV_FSP_is_done())
	    {
	        u8_status = ERR_SPINAND_TIMEOUT;
	    }

	    DRV_FSP_clear_trigger();
	} while (0);
    
    DRV_FSP_disable_outside_wbf();
    DRV_QSPI_use_sw_cs(0);
    return u8_status;
}

u8 DRV_SPINAND_program_load_data(const u8* pu8_data, u32 u32_size)
{

    DRV_FSP_use_quad_mode(_DRV_SPINAND_is_quad_cmd(st_spinand_drv_info.u8_programLoad));

    if (ERR_SPINAND_SUCCESS != _DRV_SPINAND_program_load_data((u8*)pu8_data, u32_size))
    {
        return ERR_SPINAND_DEVICE_FAILURE;
    }

    DRV_FSP_use_quad_mode(0);

    return DRV_SPINAND_check_status();
}

u8 DRV_SPINAND_random_program_load(u16 u16_col_address, u8 *pu8_data, u32 u32_size)
{
    u32 u32_cmd;

    u32_cmd = 0;
    u32_cmd |= st_spinand_drv_info.u8_randomLoad;
    u32_cmd |= (u16_col_address >> 8 & 0xff) << 8;
    u32_cmd |= (u16_col_address & 0xff) << 16;

    if (ERR_SPINAND_SUCCESS != DRV_SPINAND_single_transfer((u8*)&u32_cmd, 3))
    {
        return ERR_SPINAND_DEVICE_FAILURE;
    }

    DRV_FSP_use_quad_mode(_DRV_SPINAND_is_quad_cmd(st_spinand_drv_info.u8_randomLoad));

    if (ERR_SPINAND_SUCCESS != _DRV_SPINAND_program_load_data(pu8_data, u32_size))
    {
        return ERR_SPINAND_DEVICE_FAILURE;
    }

    DRV_FSP_use_quad_mode(0);

    return ERR_SPINAND_SUCCESS;
}

u8 DRV_SPINAND_cmd_program_load(u16 u16_col_address)
{
    u32 u32_cmd;

    if (ERR_SPINAND_SUCCESS != DRV_SPINAND_write_enable())
    {
        return ERR_SPINAND_DEVICE_FAILURE;
    }

    u32_cmd = 0;
    u32_cmd |= st_spinand_drv_info.u8_programLoad;
    u32_cmd |= ((u16_col_address >> 8) & 0xff) << 8;
    u32_cmd |= (u16_col_address& 0xff) << 16;

    return DRV_SPINAND_single_transfer((u8*)&u32_cmd, 3);
}

void DRV_SPINAND_setup_timeout(u32 u32_timeout)
{
    st_spinand_drv_info.u32_maxWaitTime = u32_timeout;
}

void DRV_SPINAND_use_bdma(u8 u8_enabled)
{
    if (u8_enabled)
    {
        _DRV_SPINAND_read_from_cache = _DRV_SPINAND_read_from_cache_by_bdma;
        _DRV_SPINAND_program_load_data = _DRV_SPINAND_program_load_data_by_bdma;
    }
    else
    {
        _DRV_SPINAND_read_from_cache = _DRV_SPINAND_read_from_cache_by_riu;
        _DRV_SPINAND_program_load_data = DRV_SPINAND_burst_transfer;
    }
}

void DRV_SPINAND_setup_access(u8 u8_read, u8 u8_dummy_cycle, u8 u8_program, u8 u8_random)
{
    if (st_spinand_drv_info.u8_rfc != u8_read || st_spinand_drv_info.u8_dummy != u8_dummy_cycle)
    {
        st_spinand_drv_info.u8_rfc = u8_read;
        st_spinand_drv_info.u8_dummy = u8_dummy_cycle;
        printf("[SPINAND] RFC ues command 0x%x with 0x%02x dummy clock.\r\n", st_spinand_drv_info.u8_rfc, st_spinand_drv_info.u8_dummy);
    }

    if (st_spinand_drv_info.u8_programLoad != u8_program || st_spinand_drv_info.u8_randomLoad != u8_random)
    {
        st_spinand_drv_info.u8_programLoad = u8_program;
        st_spinand_drv_info.u8_randomLoad = u8_random;
        printf("[SPINAND] Program load with command 0x%02x.\r\n", st_spinand_drv_info.u8_programLoad);
        printf("[SPINAND] Random load with command 0x%02x.\r\n", st_spinand_drv_info.u8_randomLoad);
    }

    return;
}

u8 DRV_SPINAND_init(void)
{
    DRV_FSP_QSPI_init();

    st_spinand_drv_info.u8_rfc = SPI_NAND_CMD_RFC;
    st_spinand_drv_info.u8_dummy = SPI_NAND_CMD_RFC_DUMMY;
    st_spinand_drv_info.u8_programLoad = SPI_NAND_CMD_PL;
    st_spinand_drv_info.u8_randomLoad = SPI_NAND_CMD_RPL;
    st_spinand_drv_info.u32_maxWaitTime = -1;
    DRV_QSPI_use_2bytes_address_mode(DRV_QSPI_cmd_to_mode(st_spinand_drv_info.u8_rfc), st_spinand_drv_info.u8_dummy);
    _DRV_SPINAND_read_from_cache = _DRV_SPINAND_read_from_cache_by_riu;
    _DRV_SPINAND_program_load_data = DRV_SPINAND_burst_transfer;
    return DRV_SPINAND_reset_status();
}

