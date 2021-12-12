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

#ifndef __MHAL_CMD_SERVICE_HH__
#define __MHAL_CMD_SERVICE_HH__

#include "mhal_common.h"

//old
#define MHAL_CMDQ_POLLNEQ_TIMEOUT        (0x1<<8)
#define MHAL_CMDQ_POLLEQ_TIMEOUT         (0x1<<9)
#define MHAL_CMDQ_WAIT_TIMEOUT           (0x1<<10)
#define MHAL_CMDQ_WRITE_TIMEOUT          (0x1<<12)
//new 
#define MHAL_CMDQ_ALL_TIMEOUT            (0x1<<8)//I6e
 
 // OR above timeout option
#define MHAL_CMDQ_ERROR_STATUS           (MHAL_CMDQ_POLLNEQ_TIMEOUT|MHAL_CMDQ_POLLEQ_TIMEOUT |MHAL_CMDQ_WAIT_TIMEOUT|MHAL_CMDQ_WRITE_TIMEOUT)

typedef enum
{
    E_CMDQ_EARLYRUN_BUSY = 0XBC5D,
    E_CMDQ_EARLYRUN_DONE = 0x9090,
} MHAL_CMDQ_EarlyRunStatus_e;

typedef enum
{
    E_CMDQ_BYTE_SEL_ALL   = 0,
    E_CMDQ_BYTE_SEL_HBYTE = 1,
    E_CMDQ_BYTE_SEL_LBYTE = 2
} MHAL_CMDQ_ByteSel_e;


#define MHAL_CMDQ_TO_DBG          (1)//I6e func for reg0x3D,0x40,0x43 not guarantee for debug info
#if(MHAL_CMDQ_TO_DBG)
#define NUMBER_MHAL_CMDQ_HW       (8)
#define MHAL_MAILBOX_BANK         (0x100400)
#define MHAL_CMDQ0_BANK           (0x112000)
#define MHAL_CMDQ4_BANK           (0x112600)

#define MHAL_CMDQ0_PEQ_MBOFST_VALUE     (0x10)
#define MHAL_CMDQ1_PEQ_MBOFST_VALUE     (0x11)
#define MHAL_CMDQ2_PEQ_MBOFST_VALUE     (0x12)
#define MHAL_CMDQ3_PEQ_MBOFST_VALUE     (0x13)
#define MHAL_CMDQ4_PEQ_MBOFST_VALUE     (0x30)
#define MHAL_CMDQ5_PEQ_MBOFST_VALUE     (0x31)
#define MHAL_CMDQ6_PEQ_MBOFST_VALUE     (0x32)
#define MHAL_CMDQ7_PEQ_MBOFST_VALUE     (0x33)
#define MHAL_CMDQ0_PENQ_MBOFST_VALUE    (0x14)
#define MHAL_CMDQ1_PENQ_MBOFST_VALUE    (0x15)
#define MHAL_CMDQ2_PENQ_MBOFST_VALUE    (0x16)
#define MHAL_CMDQ3_PENQ_MBOFST_VALUE    (0x17)
#define MHAL_CMDQ4_PENQ_MBOFST_VALUE    (0x34)
#define MHAL_CMDQ5_PENQ_MBOFST_VALUE    (0x35)
#define MHAL_CMDQ6_PENQ_MBOFST_VALUE    (0x36)
#define MHAL_CMDQ7_PENQ_MBOFST_VALUE    (0x37)

#define MHAL_CMDQ0_PEQ_MBOFST_MASK      (0x18)
#define MHAL_CMDQ1_PEQ_MBOFST_MASK      (0x19)
#define MHAL_CMDQ2_PEQ_MBOFST_MASK      (0x1A)
#define MHAL_CMDQ3_PEQ_MBOFST_MASK      (0x1B)
#define MHAL_CMDQ4_PEQ_MBOFST_MASK      (0x38)
#define MHAL_CMDQ5_PEQ_MBOFST_MASK      (0x39)
#define MHAL_CMDQ6_PEQ_MBOFST_MASK      (0x3A)
#define MHAL_CMDQ7_PEQ_MBOFST_MASK      (0x3B)
#define MHAL_CMDQ0_PENQ_MBOFST_MASK     (0x1C)
#define MHAL_CMDQ1_PENQ_MBOFST_MASK     (0x1D)
#define MHAL_CMDQ2_PENQ_MBOFST_MASK     (0x1E)
#define MHAL_CMDQ3_PENQ_MBOFST_MASK     (0x1F)
#define MHAL_CMDQ4_PENQ_MBOFST_MASK     (0x3C)
#define MHAL_CMDQ5_PENQ_MBOFST_MASK     (0x3D)
#define MHAL_CMDQ6_PENQ_MBOFST_MASK     (0x3E)
#define MHAL_CMDQ7_PENQ_MBOFST_MASK     (0x3F)

#define MHAL_CMDQ0_PEQ_MBOFST_HADR      (0x20)
#define MHAL_CMDQ0_PEQ_MBOFST_LADR      (0x21)
#define MHAL_CMDQ1_PEQ_MBOFST_HADR      (0x22)
#define MHAL_CMDQ1_PEQ_MBOFST_LADR      (0x23)
#define MHAL_CMDQ2_PEQ_MBOFST_HADR      (0x24)
#define MHAL_CMDQ2_PEQ_MBOFST_LADR      (0x25)
#define MHAL_CMDQ3_PEQ_MBOFST_HADR      (0x26)
#define MHAL_CMDQ3_PEQ_MBOFST_LADR      (0x27)

#define MHAL_CMDQ4_PEQ_MBOFST_HADR      (0x40)
#define MHAL_CMDQ4_PEQ_MBOFST_LADR      (0x41)
#define MHAL_CMDQ5_PEQ_MBOFST_HADR      (0x42)
#define MHAL_CMDQ5_PEQ_MBOFST_LADR      (0x43)
#define MHAL_CMDQ6_PEQ_MBOFST_HADR      (0x44)
#define MHAL_CMDQ6_PEQ_MBOFST_LADR      (0x45)
#define MHAL_CMDQ7_PEQ_MBOFST_HADR      (0x46)
#define MHAL_CMDQ7_PEQ_MBOFST_LADR      (0x47)
#define MHAL_CMDQ0_PNEQ_MBOFST_HADR      (0x28)
#define MHAL_CMDQ0_PNEQ_MBOFST_LADR      (0x29)
#define MHAL_CMDQ1_PNEQ_MBOFST_HADR      (0x2A)
#define MHAL_CMDQ1_PNEQ_MBOFST_LADR      (0x2B)
#define MHAL_CMDQ2_PNEQ_MBOFST_HADR      (0x2C)
#define MHAL_CMDQ2_PNEQ_MBOFST_LADR      (0x2D)
#define MHAL_CMDQ3_PNEQ_MBOFST_HADR      (0x2E)
#define MHAL_CMDQ3_PNEQ_MBOFST_LADR      (0x2F)

#define MHAL_CMDQ4_PNEQ_MBOFST_HADR      (0x48)
#define MHAL_CMDQ4_PNEQ_MBOFST_LADR      (0x49)
#define MHAL_CMDQ5_PNEQ_MBOFST_HADR      (0x4A)
#define MHAL_CMDQ5_PNEQ_MBOFST_LADR      (0x4B)
#define MHAL_CMDQ6_PNEQ_MBOFST_HADR      (0x4C)
#define MHAL_CMDQ6_PNEQ_MBOFST_LADR      (0x4D)
#define MHAL_CMDQ7_PNEQ_MBOFST_HADR      (0x4E)
#define MHAL_CMDQ7_PNEQ_MBOFST_LADR      (0x4F)
#define MHAL_CMDQ0_BUF_OFST             (0x0E)
#define MHAL_CMDQ1_BUF_OFST             (0x0F)
#define MHAL_CMDQ2_BUF_OFST             (0x2D)
#define MHAL_CMDQ3_BUF_OFST             (0x2E)
#define MHAL_CMDQ0_BUF_OFST_0_7         (0x79)
#define MHAL_CMDQ1_BUF_OFST_0_7         (0x7A)
#define MHAL_CMDQ2_BUF_OFST_0_7         (0x7B)
#define MHAL_CMDQ3_BUF_OFST_0_7         (0x7C)
#define MHAL_CMDQ4_BUF_OFST_0_7         (0x79)
#define MHAL_CMDQ5_BUF_OFST_0_7         (0x7A)
#define MHAL_CMDQ6_BUF_OFST_0_7         (0x7B)
#define MHAL_CMDQ7_BUF_OFST_0_7         (0x7C)

#define MHAL_CMDQ_WAIT          	    (0x0020)
#define MHAL_CMDQ_PEQ            	    (0x0030)
#define MHAL_CMDQ_PNEQ           	    (0x00B0)
#define MHAL_CMDQ_PEQ_EVENT             (0xFE)
#define MHAL_CMDQ_PNEQ_EVENT            (0xFF)

typedef struct
{
    MS_U8                 eTrigEvnt;
    MS_U16                 nBitMask;
} MHAL_CMDQTriggerEvent_t;
#endif

typedef enum
{	
    E_MHAL_CMDQ_INTATR_SINGLE       =   0,
    E_MHAL_CMDQ_INTATR_SEPERATE     =   1,
    E_MHAL_CMDQ_INTATR_MAX		
} MHAL_CMDQ_INT_ATR_e;
//Don't use physical bit operation here, add enum number for new event ID, should sync with HAL_CMDQ_EventId 
typedef enum
{
    E_MHAL_CMDQEVE_DUMMY15          =   1,
    E_MHAL_CMDQEVE_DUMMY14          =   2,
    E_MHAL_CMDQEVE_DUMMY13          =   3,
    E_MHAL_CMDQEVE_DUMMY12          =   4,
    E_MHAL_CMDQEVE_DUMMY11          =   5,
    E_MHAL_CMDQEVE_DUMMY10          =   6,
    E_MHAL_CMDQEVE_DUMMY09          =   7,
    E_MHAL_CMDQEVE_DUMMY08          =   8,
    E_MHAL_CMDQEVE_DUMMY07          =   9,
    E_MHAL_CMDQEVE_DUMMY06          =  10,
    E_MHAL_CMDQEVE_DUMMY05          =  11,
    E_MHAL_CMDQEVE_DUMMY04          =  12,
    E_MHAL_CMDQEVE_DUMMY03          =  13,
    E_MHAL_CMDQEVE_DUMMY02          =  14,
    E_MHAL_CMDQEVE_DUMMY01          =  15,
    E_MHAL_CMDQEVE_DUMMY00          =  16,
    E_MHAL_CMDQEVE_DIP_TRIG         =  17,
    E_MHAL_CMDQEVE_MFE_TRIG         =  18,
    E_MHAL_CMDQEVE_MHE_TRIG         =  19,
    E_MHAL_CMDQEVE_DMAGEN_TRIGGER   =  20,
    E_MHAL_CMDQEVE_JPE_TRIG         =  21,
    E_MHAL_CMDQEVE_DMA_DONE         =  22,
    E_MHAL_CMDQEVE_SC_TRIG1         =  23,
    E_MHAL_CMDQEVE_SC_TRIG0         =  24,
    E_MHAL_CMDQEVE_ISP_TRIG         =  25,
    E_MHAL_CMDQEVE_LDC_TRIG         =  26, //I6e
    E_MHAL_CMDQEVE_DIP_CMDQ_INT1    =  27, //I6e
    E_MHAL_CMDQEVE_DIP_INT1         =  28, //I6e
    E_MHAL_CMDQEVE_IVE_CMDQ         =  29, //I6e
    E_MHAL_CMDQEVE_ISPVSYNC         =  30, // M6
    E_MHAL_CMDQEVE_DISP0_INT        =  31, // M6    
    E_MHAL_CMDQEVE_DISP1_INT        =  32, // M6    
    E_MHAL_CMDQEVE_BDMA_TRIG        =  33, // M6    
    E_MHAL_CMDQEVE_CODEC_INT        =  34, // M6    
    E_MHAL_CMDQEVE_WDMA0_DONE       =  35, // M6
    E_MHAL_CMDQEVE_WDMA1_DONE       =  36, // M6
    E_MHAL_CMDQEVE_WDMA2_DONE       =  37, // M6    
    E_MHAL_CMDQEVE_WDMA3_DONE       =  38, // M6
    E_MHAL_CMDQEVE_ISP_VSYNC_TRIG   =  99, // M6
    E_MHAL_CMDQEVE_IP_MAX
} MHAL_CMDQ_EventId_e;

typedef struct MHAL_CMDQ_BufDescript_s
{
    MS_U32 u32CmdqBufSize;
    MS_U16 u32CmdqBufSizeAlign;
    MS_U32 u32MloadBufSize;
    MS_U16 u16MloadBufSizeAlign;
} MHAL_CMDQ_BufDescript_t;

typedef struct MHAL_CMDQ_MultiCmdBuf_s
{
    MS_U32 u32RegAddr;
    MS_U32 u32RegValue;
} MHAL_CMDQ_MultiCmdBuf_t;

typedef struct MHAL_CMDQ_MultiCmdBufMask_s
{
    MS_U32 u32RegAddr;
    MS_U16 u16RegValue;
    MS_U16 u16Mask;
} MHAL_CMDQ_MultiCmdBufMask_t;

typedef struct MHAL_CMDQ_Mmap_Info_s
{
    MS_U8     u8CmdqMmapGid;                         // Mmap ID
    MS_U8     u8CmdqMmapLayer;                       // Memory Layer
    MS_U8     u8CmdqMmapMiuNo;                       // 0: MIU0 / 1: MIU1 / 2: MIU2
    MS_U8     u8CmdqMmapCMAHid;                      // Memory CMAHID
    MS_U32    u32CmdqMmapPhyAddr;                       // phy Memory Address
    MS_U32    u32CmdqMmapVirAddr;                       // vir Memory Address
    MS_U32    u32CmdqMmapSize;                       // Memory Size
    MS_U32    u32CmdqMmapAlign;                      // Memory Align
    MS_U32    u32CmdqMmapMemoryType;

    MS_U8     u8MloadMmapGid;                         // Mmap ID
    MS_U8     u8MloadMmapLayer;                       // Memory Layer
    MS_U8     u8MloadMmapMiuNo;                       // 0: MIU0 / 1: MIU1 / 2: MIU2
    MS_U8     u8MloadMmapCMAHid;                      // Memory CMAHID
    MS_U32    u32MloadMmapPhyAddr;                       //phy Memory Address
    MS_U32    u32MloadMmapVirAddr;                       //vir Memory Address
    MS_U32    u32MloadMmapSize;                       // Memory Size
    MS_U32    u32MloadMmapAlign;                      // Memory Align
    MS_U32    u32MloadMmapMemoryType;
} MHAL_CMDQ_Mmap_Info_t;

typedef struct MHAL_CMDQ_CmdqInterface_s     MHAL_CMDQ_CmdqInterface_t;

struct MHAL_CMDQ_CmdqInterface_s
{
    //menuload ring buffer dynamic allocation service
//------------------------------------------------------------------------------
/// @brief Get Menuload buffer current write point
/// @param[in] MHAL_CMDQ_CmdqInterface_t *pCmdinf : CMDQ interface
/// @param[out]MS_PHYADDR* phyWritePtr : assign write point here
/// @return MHAL_SUCCESS : is ok
/// @return MHAL_FAILURE : is failed
//------------------------------------------------------------------------------
    MS_S32(*MHAL_CMDQ_GetNextMloadRingBufWritePtr)(MHAL_CMDQ_CmdqInterface_t *pCmdinf, MS_PHYADDR* phyWritePtr);
//------------------------------------------------------------------------------
/// @brief Update Menuload buffer read point
/// @param[in] MHAL_CMDQ_CmdqInterface_t *pCmdinf : CMDQ interface
/// @param[in] MS_PHYADDR phyReadPtr : read point to updtae
/// @param[out]
/// @return MHAL_SUCCESS : is ok
/// @return MHAL_FAILURE : is failed
//------------------------------------------------------------------------------
    MS_S32(*MHAL_CMDQ_UpdateMloadRingBufReadPtr)(MHAL_CMDQ_CmdqInterface_t* pCmdinf, MS_PHYADDR phyReadPtr);
//------------------------------------------------------------------------------
/// @brief copy buffer to cmdq 's Menuload buffer
/// @param[in] MHAL_CMDQ_CmdqInterface_t *pCmdinf : CMDQ interface
/// @param[in] void * MloadBuf
/// @param[in] MS_U32 u32Size
/// @param[in] MS_U16 u16Alignment
/// @param[out]MS_PHYADDR *phyRetAddr : menuload buffer head
/// @return MHAL_SUCCESS : is ok
/// @return MHAL_FAILURE : is failed
//------------------------------------------------------------------------------
    MS_S32(*MHAL_CMDQ_MloadCopyBuf)(MHAL_CMDQ_CmdqInterface_t *pCmdinf, void * MloadBuf, MS_U32 u32Size, MS_U16 u16Alignment, MS_PHYADDR *phyRetAddr);
//------------------------------------------------------------------------------
/// @brief check cmdq buffer is available
/// @param[in] MHAL_CMDQ_CmdqInterface_t *pCmdinf : CMDQ interface
/// @param[in] MS_U32 u32CmdqNum : check cmd number
/// @param[out]
/// @return 0 : is unavailable
/// @return current cmdq available number : is success
//------------------------------------------------------------------------------
    MS_S32(*MHAL_CMDQ_CheckBufAvailable)(MHAL_CMDQ_CmdqInterface_t *pCmdinf, MS_U32 u32CmdqNum);
//------------------------------------------------------------------------------
/// @brief write CMDQ dummy register
/// @param[in] MHAL_CMDQ_CmdqInterface_t *pCmdinf : CMDQ interface
/// @param[in] MS_U16 u16Value
/// @param[out]
/// @return MHAL_SUCCESS : is ok
/// @return MHAL_FAILURE : is failed
//------------------------------------------------------------------------------
    MS_S32(*MHAL_CMDQ_WriteDummyRegCmdq)(MHAL_CMDQ_CmdqInterface_t *pCmdinf, MS_U16 u16Value);
//------------------------------------------------------------------------------
/// @brief Read CMDQ dummy register
/// @param[in] MHAL_CMDQ_CmdqInterface_t *pCmdinf : CMDQ interface
/// @param[out]MS_U16* u16RegVal : assign cmdq dummy register value.
/// @return MHAL_SUCCESS : is ok
/// @return MHAL_FAILURE : is failed
//------------------------------------------------------------------------------
    MS_S32(*MHAL_CMDQ_ReadDummyRegCmdq)(MHAL_CMDQ_CmdqInterface_t *pCmdinf, MS_U16* u16RegVal);
//------------------------------------------------------------------------------
/// @brief add write command with Mask
/// @param[in] MHAL_CMDQ_CmdqInterface_t *pCmdinf : CMDQ interface.
/// @param[in] MS_U32 u32RegAddr
/// @param[in] MS_U16 u16Value
/// @param[in] MS_U16 u16WriteMask : set bit as 1 , this bit is available.
/// @return MHAL_SUCCESS : is ok
/// @return MHAL_FAILURE : is failed
//------------------------------------------------------------------------------
    MS_S32(*MHAL_CMDQ_WriteRegCmdqMask)(MHAL_CMDQ_CmdqInterface_t *pCmdinf, MS_U32 u32RegAddr, MS_U16 u16Value, MS_U16 u16WriteMask);
//------------------------------------------------------------------------------
/// @brief add multiple write command with Mask
/// @param[in] MHAL_CMDQ_CmdqInterface_t *pCmdinf : CMDQ interface.
/// @param[in] MHAL_CMDQ_MultiCmdBufMask_t *u32MultiCmdBufMask
/// @param[in] MS_U16 u16Size
/// @return Actual size already write to CMDQ buffer
//------------------------------------------------------------------------------
    MS_U32(*MHAL_CMDQ_WriteRegCmdqMaskMulti)(MHAL_CMDQ_CmdqInterface_t *pCmdinf, MHAL_CMDQ_MultiCmdBufMask_t *u32MultiCmdBufMask, MS_U16 u16Size);
//------------------------------------------------------------------------------
/// @brief add write command without Mask
/// @param[in] MHAL_CMDQ_CmdqInterface_t *pCmdinf : CMDQ interface.
/// @param[in] MS_U32 u32RegAddr
/// @param[in] MS_U16 u16Value
/// @return MHAL_SUCCESS : is ok
/// @return MHAL_FAILURE : is failed
//------------------------------------------------------------------------------
    MS_S32(*MHAL_CMDQ_WriteRegCmdq)(MHAL_CMDQ_CmdqInterface_t *pCmdinf, MS_U32 u32RegAddr, MS_U16 u16Value);
//------------------------------------------------------------------------------
/// @brief add multiple write command without Mask
/// @param[in] MHAL_CMDQ_CmdqInterface_t *pCmdinf : CMDQ interface.
/// @param[in] MHAL_CMDQ_MultiCmdBuf_t *u32MultiCmdBuf
/// @param[in] MS_U16 u16Size
/// @return Actual size already write to CMDQ buffer
//------------------------------------------------------------------------------
    MS_U32(*MHAL_CMDQ_WriteRegCmdqMulti)(MHAL_CMDQ_CmdqInterface_t *pCmdinf, MHAL_CMDQ_MultiCmdBuf_t *u32MultiCmdBuf, MS_U16 u16Size);
//------------------------------------------------------------------------------
/// @brief add poll command with Mask
/// @param[in] MHAL_CMDQ_CmdqInterface_t *pCmdinf : CMDQ interface.
/// @param[in] MS_U32 u32RegAddr
/// @param[in] MS_U16 u16Value
/// @param[in] MS_U16 u16WriteMask
/// @param[in] MS_BOOL bPollEq : true is poll eq command , false is poll neq command
/// @return MHAL_SUCCESS : is ok
/// @return MHAL_FAILURE : is failed
//------------------------------------------------------------------------------
    MS_S32(*MHAL_CMDQ_CmdqPollRegBits)(MHAL_CMDQ_CmdqInterface_t *pCmdinf, MS_U32 u32RegAddr, MS_U16 u16Value,  MS_U16 u16WriteMask, MS_BOOL bPollEq);
//------------------------------------------------------------------------------
/// @brief add wait command
/// @param[in] MHAL_CMDQ_CmdqInterface_t *pCmdinf : CMDQ interface.
/// @param[in] MHAL_CMDQ_EventId_e eEvent
/// @return MHAL_SUCCESS : is ok
/// @return MHAL_FAILURE : is failed
//------------------------------------------------------------------------------
    MS_S32(*MHAL_CMDQ_CmdqAddWaitEventCmd)(MHAL_CMDQ_CmdqInterface_t *pCmdinf, MHAL_CMDQ_EventId_e eEvent);
//------------------------------------------------------------------------------
/// @brief Abort cmdq buffer , will go back previrous write point
/// @param[in] MHAL_CMDQ_CmdqInterface_t *pCmdinf : CMDQ interface.
/// @return MHAL_SUCCESS : is ok
/// @return MHAL_FAILURE : is failed
//------------------------------------------------------------------------------
    MS_S32(*MHAL_CMDQ_CmdqAbortBuffer)(MHAL_CMDQ_CmdqInterface_t *pCmdinf);
//------------------------------------------------------------------------------
/// @brief reset cmdq engine
/// @param[in] MHAL_CMDQ_CmdqInterface_t *pCmdinf : CMDQ interface.
/// @return MHAL_SUCCESS : is ok
/// @return MHAL_FAILURE : is failed
//------------------------------------------------------------------------------
    MS_S32(*MHAL_CMDQ_CmdqResetEngine)(MHAL_CMDQ_CmdqInterface_t *pCmdinf);
//------------------------------------------------------------------------------
/// @brief Read cmdq current status
/// @param[in] MHAL_CMDQ_CmdqInterface_t *pCmdinf : CMDQ interface.
/// @return MHAL_SUCCESS : is ok
/// @return MHAL_FAILURE : is failed
//------------------------------------------------------------------------------
    MS_S32(*MHAL_CMDQ_ReadStatusCmdq)(MHAL_CMDQ_CmdqInterface_t *pCmdinf, MS_U32* u32StatVal);
//------------------------------------------------------------------------------
/// @brief kick off cmdq
/// @param[in] MHAL_CMDQ_CmdqInterface_t *pCmdinf : CMDQ interface.
/// @return < 0 : is failed
/// @return kick off cmd number : is success
//------------------------------------------------------------------------------
    MS_S32(*MHAL_CMDQ_KickOffCmdq)(MHAL_CMDQ_CmdqInterface_t *pCmdinf);
//------------------------------------------------------------------------------
/// @brief clear all trigger bus event
/// @param[in] MHAL_CMDQ_CmdqInterface_t *pCmdinf : CMDQ interface.
/// @return MHAL_SUCCESS : is ok
/// @return MHAL_FAILURE : is failed
//------------------------------------------------------------------------------
    MS_S32(*MHAL_CMDQ_ClearTriggerEvent)(MHAL_CMDQ_CmdqInterface_t *pCmdinf);

//------------------------------------------------------------------------------
/// @brief check cmdq is empty
/// @param[in] MHAL_CMDQ_CmdqInterface_t *pCmdinf : CMDQ interface.
/// @param[out],MS_BOOL* bIdleVal : idle value
/// @return MHAL_SUCCESS : is ok
/// @return MHAL_FAILURE : is failed
//------------------------------------------------------------------------------
    MS_S32(*MHAL_CMDQ_IsCmdqEmptyIdle)(MHAL_CMDQ_CmdqInterface_t *pCmdinf, MS_BOOL* bIdleVal);
    VOID *  pCtx;
//------------------------------------------------------------------------------
/// @brief kick off cmdq
/// @param[in] MHAL_CMDQ_CmdqInterface_t *pCmdinf : CMDQ interface.
/// @return < 0 : is failed
/// @return kick off cmd number : is success
//------------------------------------------------------------------------------
    MS_S32(*MHAL_CMDQ_KickOffCmdqByTag)(MHAL_CMDQ_CmdqInterface_t *pCmdinf,MS_U16 u16Tag);
//------------------------------------------------------------------------------
/// @brief command delay
/// @param[in] MHAL_CMDQ_CmdqInterface_t *pCmdinf : CMDQ interface
///            MS_U32 uTimeNs : ns
/// @return < 0 : is failed
/// @
//------------------------------------------------------------------------------
    MS_S32(*MHAL_CMDQ_CmdDelay)(MHAL_CMDQ_CmdqInterface_t *pCmdinf,MS_U32 uTimeNs);
//------------------------------------------------------------------------------
/// @brief Get current cmdq number
/// @param[in] MHAL_CMDQ_CmdqInterface_t *pCmdinf : CMDQ interface
/// @return : cmdq number
/// @
//------------------------------------------------------------------------------
    MS_U32 (*MHAL_CMDQ_GetCurrentCmdqNumber)(MHAL_CMDQ_CmdqInterface_t *pCmdinf);
//------------------------------------------------------------------------------
/// @brief add poll command with Mask
/// @param[in] MHAL_CMDQ_CmdqInterface_t *pCmdinf : CMDQ interface.
/// @param[in] MS_U32 u32RegAddr
/// @param[in] MS_U16 u16Value
/// @param[in] MS_U16 u16WriteMask
/// @param[in] MS_BOOL bPollEq : true is poll eq command , false is poll neq command
/// @param[in] MS_U32 uTimeNs : poll time (ns)
/// @return MHAL_SUCCESS : is ok
/// @return MHAL_FAILURE : is failed
//------------------------------------------------------------------------------
	MS_S32(*MHAL_CMDQ_CmdqPollRegBits_ByTime)(MHAL_CMDQ_CmdqInterface_t *pCmdinf, MS_U32 u32RegAddr, MS_U16 u16Value,  MS_U16 u16WriteMask, MS_BOOL bPollEq,MS_U32 uTimeNs);

//------------------------------------------------------------------------------
/// @brief get a dummy tag register for MI internal use
/// @param[in] MHAL_CMDQ_CmdqInterface_t *pCmdinf : CMDQ interface.
/// @return MS_U32 u32RegAddr : u32RegAddr of dummy register
//------------------------------------------------------------------------------
    MS_S32 (*MHAL_CMDQ_CmdqGetAppEventDummyRegAddr)(MHAL_CMDQ_CmdqInterface_t *pCmdinf);

//------------------------------------------------------------------------------
/// @brief set weighting register of RoundRobin for MI  
/// @param[in] MHAL_CMDQ_CmdqInterface_t *pCmdinf : CMDQ interface.
/// @param[in] MS_U32 u32Weight
//------------------------------------------------------------------------------
	MS_S32 (*MHAL_CMDQ_SetWeight)(MHAL_CMDQ_CmdqInterface_t *pCmdinf, MS_U32 u32Weight);
};

//------------------------------------------------------------------------------
/// @brief Get cmdq free ID
/// @param[in]
/// @param[out]
/// @return -1  : no free ID
/// @return >=0 : cmdq free ID
//------------------------------------------------------------------------------
MS_S32 MHAL_CMDQ_GetFreeId(void);

//------------------------------------------------------------------------------
/// @brief Get cmdq specific ID
/// @param[in]
/// @param[out]
/// @return -1  : this ID is not free
/// @return >=0 : cmdq specific ID
//------------------------------------------------------------------------------
MS_S32 MHAL_CMDQ_GetSpecificId(MS_S32 nCmdqId);

//------------------------------------------------------------------------------
/// @brief Release cmdq ID
/// @param[in]
/// @param[out]
/// @return
//------------------------------------------------------------------------------
void MHAL_CMDQ_ReleaseId(MS_S32 nCmdqId);

//------------------------------------------------------------------------------
/// @brief Get cmdq hw number
/// @param[in]
/// @param[out]
/// @return : cmdq hw number
//------------------------------------------------------------------------------
MS_U32 MHAL_CMDQ_GetCmdqHWNumber(void);

//------------------------------------------------------------------------------
/// @brief Init CMDQ mmap info
/// @param[in] MHAL_CMDQ_Mmap_Info_t *pCmdqMmapInfo
/// @param[out]
/// @return MHAL_SUCCESS : is ok
/// @return MHAL_FAILURE : is failed
//------------------------------------------------------------------------------
MS_S32 MHAL_CMDQ_InitCmdqMmapInfo(MHAL_CMDQ_Mmap_Info_t *pCmdqMmapInfo);

//------------------------------------------------------------------------------
/// @brief Deinit CMDQ mmap info for cmdq pool being released
/// @param[in]
/// @param[out]
/// @return MHAL_SUCCESS : is ok
/// @return MHAL_FAILURE : is failed
//------------------------------------------------------------------------------
MS_S32 MHAL_CMDQ_DeinitCmdqMmapInfo(void);

//------------------------------------------------------------------------------
/// @brief Get Cmdq service
/// @param[in] MS_S32 nCmdqId : CMDQ ID
/// @param[in] MHAL_CMDQ_BufDescript_t *pCmdqBufDesp : CMDQ buffer description
/// @param[in] MS_BOOL bForceRIU : CMDQ RIU mode
/// @param[out]
/// @return NULL : is failed
/// @return MHAL_CMDQ_CmdqInterface_t point is success
/// @return MI_ERR_INVALID_PARAMETER: Null parameter
//------------------------------------------------------------------------------
MHAL_CMDQ_CmdqInterface_t *MHAL_CMDQ_GetSysCmdqService(MS_S32 nCmdqId, MHAL_CMDQ_BufDescript_t *pCmdqBufDesp, MS_BOOL bForceRIU);
//------------------------------------------------------------------------------
/// @brief release cmdq service
/// @param[in] MS_S32 nCmdqId : CMDQ ID
/// @param[out]
/// @return
//------------------------------------------------------------------------------
void MHAL_CMDQ_ReleaseSysCmdqService(MS_S32 nCmdqId);

//------------------------------------------------------------------------------
/// @brief get cmdq status in early boot time
/// @param[in] MS_S32 nCmdqId : CMDQ ID
/// @param[out]
/// @return E_CMDQ_EARLYRUN_BUSY : this cmdqId is busy
/// @return E_CMDQ_EARLYRUN_DONE : this cmdqId is done
//------------------------------------------------------------------------------
MHAL_CMDQ_EarlyRunStatus_e MHAL_CMDQ_GetEarlyRunStatus(MS_S32 nCmdqId);

//------------------------------------------------------------------------------
/// @brief run cmdq with direct mode in early boot time
/// @param[in] MS_S32 nCmdqId : CMDQ ID
/// @param[in] MHAL_CMDQ_Mmap_Info_t *pCmdqMmapInfo
/// @param[out]
/// @return -1 : direct mode early run fail
/// @return  0 : direct mode early run success
//------------------------------------------------------------------------------
MS_S32 MHAL_CMDQ_DirectModeEarlyRun(MS_S32 nCmdqId, MHAL_CMDQ_Mmap_Info_t *pCmdqMmapInfo);

//------------------------------------------------------------------------------
/// @brief stop cmdq with direct mode early run
/// @param[in] MS_S32 nCmdqId : CMDQ ID
/// @param[out]
/// @return -1 : direct mode early run fail
/// @return  0 : direct mode early run success
//------------------------------------------------------------------------------
MS_S32 MHAL_CMDQ_DirectModeEarlyRunStop(MS_S32 nCmdqId);

//------------------------------------------------------------------------------
/// @brief add byte mode switch command to buffer
/// @param[in] MS_S32 nCmdqId : CMDQ ID
/// @param[in] MHAL_CMDQ_ByteSel_e eByteSel
/// @param[in] void *pCmdqBuf
/// @param[out]
/// @return buffer used size
//------------------------------------------------------------------------------
MS_S32 MHAL_CMDQ_DirectModeByteModeCmd(MS_S32 nCmdqId, MHAL_CMDQ_ByteSel_e eByteSel, void *pCmdqBuf);

#endif
