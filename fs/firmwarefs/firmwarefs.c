/* SigmaStar trade secret */
/* Copyright (c) 2018-2019 Sigmastar Technology Corp.
 All rights reserved.

 Unless otherwise stipulated in writing, any and all information contained
herein regardless in any format shall remain the sole proprietary of
Sigmastar Technology Corp. and be kept in strict confidence
(Sigmastar Confidential Information) by the recipient.
Any unauthorized act including without limitation unauthorized disclosure,
copying, use, reproduction, sale, distribution, modification, disassembling,
reverse engineering and compiling of the contents of Sigmastar Confidential
Information is unlawful and strictly prohibited. Sigmastar hereby reserves the
rights to any and all damages, losses, costs and expenses resulting therefrom.
*/


/*
 * firmwarefs.c
 */
#include <stdlib.h>
#include <fwfs.h>
#include <firmwarefs.h>

#if !defined(CONFIG_MS_SPINAND)
#include <asm/errno.h>
#include <spi.h>
#include <spi_flash.h>
#endif

#if defined(CONFIG_FS_FIRMWAREFS)

typedef struct
{
    fwfs_t fwfs;
    struct fwfs_config cfg;
} FirmwarefsMntHandle;

typedef struct
{
    fwfs_t *fwfs_file;
    fwfs_file_t fwfs_fd;
} FirmwarefsFileHandle;


extern int printf(const char *fmt, ...);

#if defined(CONFIG_MS_SPINAND)
extern  void MDrv_SPINAND_GetPartitionInfo(char *partition, void *pPartInfo);
extern  U32 MDrv_SPINAND_ReadData(U32 u32_PageIdx, U8 *u8Data, U8 *pu8_SpareBuf, U32 u32_DataSize, void *fwfsPartitionInfo);
extern U32 MDrv_SPINAND_Write(U32 u32_PageIdx, U8 *u8Data, U8 *pu8_SpareBuf);
extern U32 MDrv_SPINAND_BLOCK_ERASE(U32 u32_BlkIdx);
#define FIRMWAREFS_GET_PART_INFO                  MDrv_SPINAND_GetPartitionInfo

#define FIRMWAREFS_CACHE_SIZE                     2048
#define FIRMWAREFS_FILE_CACHE_SIZE                8192
#define FIRMWAREFS_READ_SIZE                      2048
#define FIRMWAREFS_PROG_SIZE                      2048
#define FIRMWAREFS_BLK_SIZE                       131072
#define FIRMWAREFS_SUBBLK_SIZE                    32768
#define FIRMWAREFS_CACHE_POOL_SIZE                8

#else
extern int nor_flash_get_partitioninfo(char *partition, void *pPartitionInfo);
extern int _spi_flash_read(struct spi_flash *flash, u32 offset,
                           size_t len, void *buf);
extern int _spi_flash_write(struct spi_flash *flash, u32 offset,
                            size_t len, const void *buf);
extern int _spi_flash_erase(struct spi_flash *flash, u32 offset,
                            size_t len);
#define FIRMWAREFS_GET_PART_INFO                  nor_flash_get_partitioninfo
static struct spi_flash *spinor_flash = NULL;

#define FIRMWAREFS_CACHE_SIZE                     2048
#define FIRMWAREFS_FILE_CACHE_SIZE                2048
#define FIRMWAREFS_READ_SIZE                      2048
#define FIRMWAREFS_PROG_SIZE                      2048
#define FIRMWAREFS_BLK_SIZE                       65536
#define FIRMWAREFS_SUBBLK_SIZE                    32768
#define FIRMWAREFS_CACHE_POOL_SIZE                4

#endif

static FirmwarefsMntHandle _gFwfsMntHandle;


//block device operations
static int block_device_read(const struct fwfs_config *c, fwfs_block_t block, fwfs_off_t off, void *buffer, fwfs_size_t size)
{
    U32 u32Addr;
    u32Addr = c->block_size * (block + c->block_offset) + off;

#if defined(CONFIG_MS_SPINAND)
    S32 ret;
    U32 u32PageIdx;
    U32 u32PageCnt, i;
    u32PageIdx = u32Addr / c->page_size;
    u32PageCnt =  size / c->page_size;

    for (i = 0; i < u32PageCnt; i++)
    {
        ret = MDrv_SPINAND_ReadData(u32PageIdx+i, (u8*)buffer+c->page_size*i, NULL, c->page_size, (void*)&c->fwfs_partition);
        if(ret)
        {
            break;
        }
    }
    return ret;
#else
    return _spi_flash_read(spinor_flash, u32Addr, size, buffer);
#endif

}

static int block_device_prog(const struct fwfs_config *c, fwfs_block_t block,
                             fwfs_off_t off, const void *buffer, fwfs_size_t size)
{
#if defined(CONFIG_MS_SPINAND)
    S32 ret;
    u32 u32Addr;
    u32 u32PageIdx;
    u32 u32PageCnt, i;

    if ((size % c->page_size) != 0)
    {
        //EINVAL
        return 22;
    }

    u32Addr = c->block_size * (block + c->block_offset) + off;
    u32PageIdx = u32Addr / c->page_size;
    u32PageCnt = size / c->page_size;

    for (i = 0; i < u32PageCnt; i++)
    {
        ret = MDrv_SPINAND_Write(u32PageIdx + i, (u8*)buffer + c->page_size * i, NULL);
        if (ret)
        {
            return ret;
        }
    }

    return ret;
#else
    u32 u32Addr;

    u32Addr = c->block_size * (block + c->block_offset) + off;
    return _spi_flash_write(spinor_flash, u32Addr, size, buffer);
#endif
}

static int block_device_erase(const struct fwfs_config *c, fwfs_block_t block)
{
#if defined(CONFIG_MS_SPINAND)
    u32 u32PA;
    S32 ret;

    u32PA = (block + c->block_offset) << 6;
    ret = MDrv_SPINAND_BLOCK_ERASE(u32PA);
    return ret;
#else
    u32 u32Addr;

    u32Addr = c->block_size * (block + c->block_offset);
    return _spi_flash_erase(spinor_flash, u32Addr, c->block_size);
#endif
}


static int block_device_sync(const struct fwfs_config *c)
{
    //CamOsPrintf("### block_device_sync\n");
    return 0;
}

S32  firmwarefs_mount(char *partition, char *mnt_path)
{
    int err = 0;

    memset(&_gFwfsMntHandle.cfg.fwfs_partition, 0x0, sizeof(_gFwfsMntHandle.cfg.fwfs_partition));
#if !defined(CONFIG_MS_SPINAND)
    if (spinor_flash == NULL)
    {
        spinor_flash = spi_flash_probe(0, 0, 1000000, SPI_MODE_3);
        if (!spinor_flash)
        {
            return -ENODEV;
        }
    }
#endif
    _gFwfsMntHandle.cfg.fwfs_partition.u8_UseExternBlockSize = 0;
    _gFwfsMntHandle.cfg.fwfs_partition.u32_BlkSize = FIRMWAREFS_BLK_SIZE;
    FIRMWAREFS_GET_PART_INFO(partition, (void*)&_gFwfsMntHandle.cfg.fwfs_partition);

    //block device operations
    _gFwfsMntHandle.cfg.read = block_device_read;
    _gFwfsMntHandle.cfg.prog = block_device_prog;
    _gFwfsMntHandle.cfg.erase = block_device_erase;
    _gFwfsMntHandle.cfg.sync = block_device_sync;

    //block device configuration
     //spinad need set flash page size, value is 2048, spinor not limit
    _gFwfsMntHandle.cfg.page_size = _gFwfsMntHandle.cfg.fwfs_partition.u32_PageSize;
    // block size is a multiple of cache size
    _gFwfsMntHandle.cfg.block_size = _gFwfsMntHandle.cfg.fwfs_partition.u32_BlkSize;
    _gFwfsMntHandle.cfg.block_count = _gFwfsMntHandle.cfg.fwfs_partition.u16_BlkCnt;
    _gFwfsMntHandle.cfg.block_offset = _gFwfsMntHandle.cfg.fwfs_partition.u16_StartBlk;
    _gFwfsMntHandle.cfg.subblock_size = FIRMWAREFS_SUBBLK_SIZE;

    _gFwfsMntHandle.cfg.read_size = FIRMWAREFS_READ_SIZE;
    _gFwfsMntHandle.cfg.cache_pool_size = FIRMWAREFS_CACHE_POOL_SIZE;
    //cache size is a multiple of read sizes
    _gFwfsMntHandle.cfg.cache_size = FIRMWAREFS_CACHE_SIZE;
    _gFwfsMntHandle.cfg.prog_size = FIRMWAREFS_PROG_SIZE;
    //Must be a multiple of the read sizes
    _gFwfsMntHandle.cfg.file_cache_size = FIRMWAREFS_FILE_CACHE_SIZE;
    _gFwfsMntHandle.cfg.block_cycles = 500;
    _gFwfsMntHandle.cfg.lookahead_size = 8;

    // mount the filesystem
    err = fwfs_mount(&_gFwfsMntHandle.fwfs, &_gFwfsMntHandle.cfg);

    return err;
}

void firmwarefs_unmount(void)
{
    fwfs_unmount(&_gFwfsMntHandle.fwfs);
#if !defined(CONFIG_MS_SPINAND)
    if (spinor_flash != NULL)
    {
        spi_flash_free(spinor_flash);
        spinor_flash = NULL;
    }
#endif
}

void* firmwarefs_open(char *filename, U32 flags, U32 mode)
{
    int err;
    int nflags = 0x0;
    void *ret = NULL;
    FirmwarefsFileHandle *fd = NULL;

    fd = (FirmwarefsFileHandle *)calloc(sizeof(FirmwarefsFileHandle), 1);
    if (!fd)
    {
        printf("%s: alloc fd fail\n", __FUNCTION__);
        goto firmwarefs_open_end;
    }

    if (flags & O_RDONLY)
        nflags |= FWFS_O_RDONLY;

    if (flags & O_WRONLY)
        nflags |= FWFS_O_WRONLY;

    if (flags & O_RDWR)
        nflags |= FWFS_O_RDWR;

    if (flags & O_CREAT)
        nflags |= FWFS_O_CREAT;

    if (flags & O_APPEND)
        nflags |= FWFS_O_APPEND;

    if (flags & O_TRUNC)
        nflags |= FWFS_O_TRUNC;

    if (nflags == 0x0)
        nflags = FWFS_O_RDONLY;

    err = fwfs_file_open(&_gFwfsMntHandle.fwfs, &fd->fwfs_fd, filename, nflags);
    if (err)
    {
        printf("%s: open %s fail(%d)\n", __FUNCTION__, filename, err);
        free((void*)fd);
        goto firmwarefs_open_end;
    }

    fd->fwfs_file = &_gFwfsMntHandle.fwfs;
    ret = fd;

firmwarefs_open_end:
    return ret;
}

S32 firmwarefs_close(void* fd)
{
    int ret = 0;
    FirmwarefsFileHandle *file = (FirmwarefsFileHandle *)fd;
    if (fd)
    {
        ret = fwfs_file_close(file->fwfs_file, &file->fwfs_fd);
        if(ret)
        {
            printf("%s: close fail(%d)\n", __FUNCTION__, ret);
        }
        free(fd);
        fd = NULL;
    }
    return ret;
}

S32 firmwarefs_read(void* fd, void *buf, U32 count)
{
    FirmwarefsFileHandle *file = (FirmwarefsFileHandle *)fd;

    return fwfs_file_read(file->fwfs_file, &file->fwfs_fd, buf, count);
}

S32 firmwarefs_write(void* fd, void *buf, U32 count)
{
    FirmwarefsFileHandle *file = (FirmwarefsFileHandle *)fd;

    return fwfs_file_write(file->fwfs_file, &file->fwfs_fd, buf, count);
}

S32 firmwarefs_lseek(void* fd, S32 offset, S32 whence)
{
    FirmwarefsFileHandle *file = (FirmwarefsFileHandle *)fd;
    u32 ret;

    switch (whence)
    {
        case SEEK_SET:
            ret = fwfs_file_seek(file->fwfs_file, &file->fwfs_fd,
                                 offset, FWFS_SEEK_SET);
            break;
        case SEEK_CUR:
            ret = fwfs_file_seek(file->fwfs_file, &file->fwfs_fd,
                                 offset, FWFS_SEEK_CUR);
        case SEEK_END:
            ret = fwfs_file_size(file->fwfs_file, &file->fwfs_fd);
            break;
        default:
            ret = 0;
            break;
    }

    return ret;
}
#endif
