SRC:=.
DEST:=../build
ENTRYPOINT:= 0x10000

# TARGET:= main

CC:=gcc

CFLAGS:= -m32
CFLAGS+= -fno-builtin
CFLAGS+= -fno-common
CFLAGS+= -nostdinc
CFLAGS+= -fno-pic
CFLAGS+= -fno-pie
CFLAGS+= -nostdlib
CFLAGS+= -fno-stack-protector
CFLAGS+= -g
CFLAGS:=$(strip ${CFLAGS})

DEBUG:= -g

include boot/make.conf
include libk/make.conf
include kernel/make.conf
include arch/i386/make.conf

# ======== 保留，不用来编译，必须使用构建的i686工具链进行编译 =======
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
	ld -m elf_i386 -static $^ -o $@ -Ttext $(ENTRYPOINT)
	@echo complete!


include ../conf/image.conf
include ../conf/emulator.conf

.PHONY: clean
clean: 
	rm -rf ../build

.PHONY: target
# target: $(KERNEL)
target: 
	@echo $(START)
	@echo $(ARCH_SRCS_NASM)
	@echo $(ARCH_SRCS_C)
	@echo $(ARCH_OBJS_NASM)
	@echo $(ARCH_OBJS_C)
	@echo $(LIBK_OBJS)
	@echo $(OBJS)