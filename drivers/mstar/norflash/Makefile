CONFIG_SYS_CONFIG_NAME := $(subst ",,$(CONFIG_SYS_CONFIG_NAME))

ccflags-y += -I$(srctree)/$(src)/$(CONFIG_SYS_CONFIG_NAME)
ccflags-y += -I$(srctree)/$(src)/inc
ccflags-y += -I$(srctree)/$(src)/../fsp_qspi/drv/inc
ccflags-y += -I$(srctree)/$(src)/../fsp_qspi/hal/inc
ccflags-y += -I$(srctree)/$(src)/../bdma/$(CONFIG_SYS_CONFIG_NAME)

obj-y += src/drvSPINOR.o
obj-y += src/mtd_isp.o

