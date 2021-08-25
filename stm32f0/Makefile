.PHONY: menuconfig post_config autoconf

CURDIR          :=$(shell pwd)
Q               := @
obj             := $(CURDIR)/scripts/kconfig
SUBDIR          := $(obj)
CONFIG_KCONFIG  := Kconfig

all:post_config autoconf test.bin

menuconfig : $(obj)/mconf $(obj)/conf
	$(Q)$< $(CONFIG_KCONFIG)
	@scripts/autoconf.sh

savedefconfig: $(obj)/conf
	$< $(silent) --$@=defconfig $(CONFIG_KCONFIG)

$(obj)/mconf:
	echo $(MAKE)
	$(Q)$(MAKE) -C $(SUBDIR) prepare
	$(Q)$(MAKE) -C $(SUBDIR)

$(obj)/conf:
	$(Q)$(MAKE) -C $(SUBDIR) prepare
	$(Q)$(MAKE) -C $(SUBDIR)

autoconf:
	@scripts/autoconf.sh

post_config:
	@scripts/config_parse.sh


CROSS_COMPILE=arm-none-eabi-
AR      = $(CROSS_COMPILE)ar
AS      = $(CROSS_COMPILE)as
CC      = $(CROSS_COMPILE)gcc
LD      = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
NM      = $(CROSS_COMPILE)nm
STRIP   = $(CROSS_COMPILE)strip

MY_INCLUDE_DIR += system/inc
MY_INCLUDE_DIR += core/inc
MY_INCLUDE_DIR += stlib/inc
MY_INCLUDE_DIR += include
#MY_INCLUDE_DIR += include/uboot
#MY_INCLUDE_DIR += include/lib
#MY_INCLUDE_DIR += include/linux

cobj-y := main.o

-include .config
-include config.mk

INCLUDE_CMPL_DIR =$(addprefix -I , $(MY_INCLUDE_DIR))


CCFLAGS += $(INCLUDE_CMPL_DIR)
CCFLAGS += -I./
CCFLAGS += -mcpu=cortex-m4 -mthumb -Wall  -std=gnu99
CCFLAGS += -Wno-unused-variable #don't waring unused variable
CCFLAGS += -Wno-unused-function #don't waring unused function
CCFLAGS += -nostartfiles
CCFLAGS += -ffunction-sections -fdata-sections
CCFLAGS += -g
CCFLAGS += -fno-builtin
CCFLAGS += -fsigned-char
CCFLAGS += -static-libgcc

LDFLAGS += -mcpu=cortex-m4 -mthumb
LDFLAGS += -specs=nano.specs
LDFLAGS += -Wl,--gc-sections
LDFLAGS += -u_printf_float

DEFS += -DUSE_STDPERIPH_DRIVER

cobj-y += newlib/syscalls.o

%_defconfig: $(obj)/conf
	$(Q)$< $(silent) --defconfig=configs/$@ $(CONFIG_KCONFIG)

clean:
	@rm -f  $(sobj-y) $(cobj-y)
	@rm -rf output
distclean:
	@rm -f  $(sobj-y) $(cobj-y)
	@rm -rf output .config include/autoconf.h
test.bin:test.elf
	@$(OBJCOPY) -O binary -S $< $@
	@mkdir -p output
	@mv $@ output/$@
	@mv $< output/$<
test.elf: $(cobj-y) $(sobj-y)
	@$(CC) $(LDFLAGS) $^ -Tsystem/STM32F030R8_FLASH.ld -o $@
	@arm-none-eabi-size $@

burn:test.bin
	@./download.sh output/test.bin

$(sobj-y):%.o:%.S
	@echo -e "\033[035mcompiling\033[0m \033[033m[$(CC)]\033[0m: \033[032m$<\033[0m"
	@$(CC) $(CCFLAGS) -c $< -o $@
$(cobj-y):%.o:%.c
	@echo -e "\033[035mcompiling\033[0m \033[033m[$(CC)]\033[0m: \033[032m$<\033[0m"
	@$(CC) $(CCFLAGS) $(DEFS)  -c $< -o $@
