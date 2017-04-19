# Copyright 2017 Julian Ingram
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# 	http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

MIPS_MTI_ELF_ROOT ?= /opt/imgtec/Toolchains/mips-mti-elf/2016.05-03
# Overrides the system value that is set to the img-elf toolchain by default
MIPS_ELF_ROOT := $(MIPS_MTI_ELF_ROOT)

ROMABLE := 1

DEFINES += SKIP_COPY_TO_RAM

ABI := 32
APP_START := 0x9d001000
ISR_VECTOR_COUNT := 1
MEMORY_SIZE := 32K
LDSCRIPT := pic32mz_uhi.ld

include ${MIPS_ELF_ROOT}/share/mips/rules/mipshal.mk

SRCS := main.c pic32_cpy_data.c pic32mz_config.c p32mz2048efg100.S excpt_isr.S
SRCS += reset_mod.S
RM := rm -rf
MKDIR := mkdir -p
BINDIR ?= bin
BUILDDIR ?= build
DEPDIR ?= $(BUILDDIR)/deps
TARGET ?= $(BINDIR)/led_flash.elf
# If you have a pic32mz2048ecg rather than an efg on your board change
# `-march=m5101` to `-march=m14kec` in both the CFLAGS and LDFLAGS below
CFLAGS += -march=m5101 -EL -g3 -O2 -Werror -Wall -Wextra -Wpedantic -Wconversion
CFLAGS += -Wcast-align -Wint-to-pointer-cast -Wstrict-prototypes -Wcast-qual
CFLAGS += -Wundef -Wstrict-overflow=5 -Wwrite-strings -Wshadow -Wfloat-equal
CFLAGS += -pedantic-errors -std=c11 $(IDIRS:%=-I%/) $(DEFINES:%=-D%)
LDFLAGS += -march=m5101 -EL -Wl,--defsym,__use_excpt_boot=0 -g3 -O2
LDFLAGS += -Wl,-Map,$(BINDIR)/output.map
IDIRS :=
DEPFLAGS = -MMD -MP -MF $(@:$(BUILDDIR)/%.o=$(DEPDIR)/%.d)
RM_ON_CLEAN := $(TARGET) $(TARGET:%.elf=%_physical.elf) $(DEPDIR) $(BUILDDIR)
RM_ON_CLEAN += $(TARGET:%.elf=%_pickit.hex) $(BINDIR)/output.map $(BINDIR)

SRCS := $(filter %.c %.S %.s, $(SRCS))

OBJS := $(SRCS:%.c=$(BUILDDIR)/%.o)
OBJS := $(OBJS:%.S=$(BUILDDIR)/%.o)
OBJS := $(OBJS:%.s=$(BUILDDIR)/%.o)
DEPS := $(OBJS:$(BUILDDIR)/%.o=$(DEPDIR)/%.d)

OBJCOPY_TO_PHY := --change-section-lma .lowerbootflashalias-0xa0000000
OBJCOPY_TO_PHY += --change-section-lma .bootflash1-0xa0000000
OBJCOPY_TO_PHY += --change-section-lma .bootflash2-0xa0000000
OBJCOPY_TO_PHY += --change-section-lma .exception_vector-0x80000000
OBJCOPY_TO_PHY += --change-section-lma .text-0x80000000
OBJCOPY_TO_PHY += --change-section-lma .init-0x80000000
OBJCOPY_TO_PHY += --change-section-lma .fini-0x80000000
OBJCOPY_TO_PHY += --change-section-lma .eh_frame-0x80000000
OBJCOPY_TO_PHY += --change-section-lma .jcr-0x80000000
OBJCOPY_TO_PHY += --change-section-lma .ctors-0x80000000
OBJCOPY_TO_PHY += --change-section-lma .dtors-0x80000000
OBJCOPY_TO_PHY += --change-section-lma .rodata-0x80000000
OBJCOPY_TO_PHY += --change-section-lma .data-0x80000000
OBJCOPY_TO_PHY += --change-section-lma .bss-0x80000000
OBJCOPY_TO_PHY += --change-section-lma .startdata-0x80000000

.PHONY: all
all: $(TARGET)
	$(OBJCOPY) $(OBJCOPY_TO_PHY) $(TARGET) $(TARGET:%.elf=%_physical.elf)
	objcopy -O ihex $(TARGET:%.elf=%_physical.elf) $(TARGET:%.elf=%_pickit.hex)

# link
$(TARGET): $(OBJS)
	$(if $(BINDIR),$(MKDIR) $(BINDIR),)
	$(CC) -o $@ $^ $(LDFLAGS)

# compile and/or generate dep files
$(BUILDDIR)/%.o: %.c
	$(MKDIR) $(BUILDDIR)/$(dir $<)
	$(MKDIR) $(DEPDIR)/$(dir $<)
	$(CC) $(DEPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/%.o: %.S
	$(MKDIR) $(BUILDDIR)/$(dir $<)
	$(MKDIR) $(DEPDIR)/$(dir $<)
	$(CC) $(DEPFLAGS) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	$(RM) $(RM_ON_CLEAN)

-include $(DEPS)
