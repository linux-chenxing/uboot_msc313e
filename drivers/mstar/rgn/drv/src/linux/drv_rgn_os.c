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

#include "drv_rgn_os.h"
#include "hal_gop.h"

#include <linux/clk.h>
#include <linux/clk-provider.h>
#include <linux/module.h>

#include "cam_clkgen.h"
#include "cam_sysfs.h"

unsigned long gGopDmaThd = 0;
module_param(gGopDmaThd, ulong, S_IRUGO | S_IWUSR);
bool _DrvGopTransId(DrvGopIdType_e eGopId, HalGopIdType_e *pHalId);
u8 DrvGopClkDisable(DrvGopIdType_e eGopId,bool bEn);
u8 DrvGopClkEnable(DrvGopIdType_e eGopId);
bool DrvRgnOsSetGopClkEnable(DrvGopIdType_e eGopId)
{
#ifndef CONFIG_CAM_CLK
    u32 u32clknum;
    HalGopIdType_e enType;
    struct device_node *dev_node = NULL;
    u32 num_parents;
    struct clk **ip_clks;

    if(!_DrvGopTransId(eGopId,&enType))
    {
        return 0;
    }

    HalGopGetClkNum(enType,&u32clknum);

    dev_node = of_find_compatible_node(NULL, NULL, "sigmastar,gop");
    if(dev_node)
    {
        num_parents = CamOfClkGetParentCount(dev_node);
        if(num_parents > u32clknum)
        {
            ip_clks = CamOsMemAlloc((sizeof(struct clk *) * num_parents));
            if(ip_clks == NULL)
            {
                return 0;
            }

            //enable all clk
            ip_clks[u32clknum] = of_clk_get(dev_node, u32clknum);
            if (IS_ERR(ip_clks[u32clknum]))
            {
                DRVRGNERR( "Fail to get [RGN] clk!\n" );
                CamOsMemRelease(ip_clks);
                return 0;
            }
            DRVRGNDBG( "[RGN] clk on num_parents:%d! %s\n"
                ,num_parents
                ,CamOfClkGetParentName(dev_node,u32clknum)
                );
            CamClkPrepareEnable(ip_clks[u32clknum]);
            clk_put(ip_clks[u32clknum]);
            CamOsMemRelease(ip_clks);
        }
    }
    return 1;
#else
    return DrvGopClkEnable(eGopId);

#endif
}

bool DrvRgnOsSetGopClkDisable(DrvGopIdType_e eGopId,bool bEn)
{
#ifndef CONFIG_CAM_CLK
    u32 u32clknum;
    HalGopIdType_e enType;
    struct device_node *dev_node = NULL;
    u32 num_parents;
    struct clk **ip_clks;

    if(!bEn)
    {
        return 0;
    }
    if(!_DrvGopTransId(eGopId,&enType))
    {
        return 0;
    }
    
    HalGopGetClkNum(enType,&u32clknum);

    dev_node = of_find_compatible_node(NULL, NULL, "sigmastar,gop");
    if(dev_node)
    {
        num_parents = CamOfClkGetParentCount(dev_node);
        //SCL_DBGERR( "[SCL] num_parents:%d! %s\n" ,num_parents ,CamOfClkGetParentName(dev_node,u32clknumu32clknum));
        if(num_parents > u32clknum)
        {
            ip_clks = CamOsMemAlloc((sizeof(struct clk *) * num_parents));
            if(ip_clks == NULL)
            {
                DRVRGNERR( "[RGN]%s kzalloc failed!\n" ,__FUNCTION__);
                return 0;
            }

            //enable all clk
            ip_clks[u32clknum] = of_clk_get(dev_node, u32clknum);
            if (IS_ERR(ip_clks[u32clknum]))
            {
                DRVRGNERR( "Fail to get [RGN] clk!\n" );
                CamOsMemRelease(ip_clks);
                return 0;
            }
            CamClkDisableUnprepare(ip_clks[u32clknum]);
            clk_put(ip_clks[u32clknum]);
            CamOsMemRelease(ip_clks);
        }
    }
    return bEn;
#else
    return DrvGopClkDisable(eGopId,bEn);
#endif

}
