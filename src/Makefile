SRC:=.
DEST:=../build
ENTRYPOINT:= 0x100000

# ARCH!=../conf/uname.sh

ARCH:=i386

TARGET:=$(ARCH)-elf

ARCHDIR:=arch/$(ARCH)

CC:=gcc

CFLAGS:= -static
# x86_64 需要注释此行
CFLAGS:= -m32

CFLAGS+= -fno-builtin
CFLAGS+= -fno-common
CFLAGS+= -nostdinc
CFLAGS+= -fno-pic
CFLAGS+= -fno-pie
CFLAGS+= -nostdlib
CFLAGS+= -fno-stack-protector
CFLAGS+= -Og
CFLAGS+= -g
CFLAGS:=$(strip ${CFLAGS})

# 交叉编译器
# CC:=$(HOME)/opt/cross/bin/$(TARGET)-gcc
# CFLAGS:= -g
# CFLAGS+= -ffreestanding -Wall -Wextra -nostdlib

DEBUG:= -g

include boot/make.conf
include libk/make.conf
include kernel/make.conf
include $(ARCHDIR)/make.conf

# ======== 保留，不用来编译，必须使用构建的工具链进行编译 =======
# 
# CRT_S:=$(foreach dir, $(ARCHDIR), $(wildcard $(dir)/*.S))
# CRT_OBJS:=$(patsubst %.c,$(DEST)/%.o,$(CRT_S))
# CRTBEGIN_OBJ:=$(shell $(CC) $(CFLAGS) -print-file-name=crtbegin.o)
# CRTEND_OBJ:=$(shell $(CC) $(CFLAGS) -print-file-name=crtend.o)

# OBJS=\
# $(DEST)/$(ARCHDIR)/crti.o \
# $(CRTBEGIN_OBJ) \
# $(CRTEND_OBJ) \
# $(DEST)/$(ARCHDIR)/crtn.o \

# $(DEST)/$(ARCHDIR)/crti.o: $(SRC)/$(ARCHDIR)/crti.S
# 	$(shell mkdir -p $(dir $@))
# 	gcc -MD -c $(SRC)/$(ARCHDIR)/crti.S -o $(DEST)/$(ARCHDIR)/crti.o $(CFLAGS)

# $(DEST)/$(ARCHDIR)/crtn.o: $(SRC)/$(ARCHDIR)/crtn.S
# 	$(shell mkdir -p $(dir $@))
# 	gcc -MD -c $(SRC)/$(ARCHDIR)/crtn.S -o $(DEST)/$(ARCHDIR)/crtn.o $(CFLAGS)


# ==================================================================






OBJS:=\
$(START) \
$(ARCH_OBJS_NASM) \
$(ARCH_OBJS_C) \
$(KERNEL_OBJS) \
$(LIBKA) \

KERNEL:=$(DEST)/kernel.bin

$(KERNEL): $(OBJS) 
	@echo linking...
	$(shell mkdir -p $(dir $@))
	$(CC) -T $(ARCHDIR)/linker.ld -o $@ $(CFLAGS) $(OBJS)
# ld -m elf_x86_64 -static $^ -o $@ -Ttext $(ENTRYPOINT)
	@echo complete!


include ../conf/image.conf
include ../conf/emulator.conf

.PHONY: clean
clean: 
	rm -rf ../build

.PHONY: target
# target: $(KERNEL)
target: 
	@echo "入口程序可重定位目标文件"
	@echo $(START)
	@echo ""
	@echo "目标架构中的所有汇编源文件"
	@echo $(ARCH_SRCS_NASM)
	@echo ""
	@echo "目标架构中的所有C源文件"
	@echo $(ARCH_SRCS_C)
	@echo ""
	@echo "目标架构中的所有由汇编编译的可重定位目标文件"
	@echo $(ARCH_OBJS_NASM)
	@echo ""
	@echo "目标架构中的所有由C代码编译出的可重定位目标文件"
	@echo $(ARCH_OBJS_C)
	@echo ""
	@echo "libk库的所有可重定位目标文件"
	@echo $(LIBK_OBJS)
	@echo ""
	@echo "被链接的所有文件"
	@echo $(OBJS)

.PHONY: kernel
kernel: $(KERNEL)
