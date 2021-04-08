######################################
# target
######################################
TARGET = WP43S

######################################
# building variables
######################################
# debug build?
ifdef DEBUG
DEBUG = 1
endif

#######################################
# pathes
#######################################
# Build path
BUILD_DIR = ../build/dmcp

# Path to aux build scripts (including trailing /)
# Leave empty for scripts in PATH
BIN_DIR = bin/

######################################
# System sources
######################################
C_INCLUDES += -I../dep/DMCP_SDK/dmcp -I../src/wp43s-dmcp
C_SOURCES += ../dep/DMCP_SDK/dmcp/sys/pgm_syscalls.c
ASM_SOURCES = ../dep/DMCP_SDK/dmcp/startup_pgm.s

#######################################
# Custom section
#######################################

# Includes
C_INCLUDES += -I../src/wp43s -I../dep/decNumberICU -I../dep/gmp-6.2.0

# C sources
C_SOURCES += ../dep/decNumberICU/decContext.c
C_SOURCES += ../dep/decNumberICU/decDouble.c
C_SOURCES += ../dep/decNumberICU/decimal128.c
C_SOURCES += ../dep/decNumberICU/decimal64.c
C_SOURCES += ../dep/decNumberICU/decNumber.c
C_SOURCES += ../dep/decNumberICU/decQuad.c
C_SOURCES += $(wildcard ../src/wp43s/*.c) \
             $(wildcard ../src/wp43s/mathematics/*.c) \
             $(wildcard ../src/wp43s/browsers/*.c) \
             $(wildcard ../src/wp43s/logicalOps/*.c) \
             $(wildcard ../src/wp43s/programming/*.c) \
             $(wildcard ../src/wp43s/distributions/*.c) \
             $(wildcard ../src/wp43s/ui/*.c)

# WP43S related defines
C_DEFS += -DDMCP_BUILD -DOS32BIT -DUSE_HAL_DRIVER -DSTM32L476xx -DARM

# Libraries
GMPLIB = ../dep/gmp-6.2.0/.libs/libgmp.a
LIBS += $(GMPLIB)


#######################################
# binaries
#######################################
CC = arm-none-eabi-gcc
AS = arm-none-eabi-gcc -x assembler-with-cpp
OBJCOPY = arm-none-eabi-objcopy
AR = arm-none-eabi-ar
SIZE = arm-none-eabi-size
HEX = $(OBJCOPY) -O ihex
BIN = $(OBJCOPY) -O binary -S

#######################################
# CFLAGS
#######################################
# macros for gcc
C_DEFS += -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))"
CPUFLAGS += -mthumb -march=armv7e-m -mfloat-abi=hard -mfpu=fpv4-sp-d16 --specs=nosys.specs

# compile gcc flags
ASFLAGS = $(CPUFLAGS) $(OPT) -Wall -fdata-sections -ffunction-sections
CFLAGS  = $(CPUFLAGS) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections
CFLAGS += -Wno-misleading-indentation

ifeq ($(DEBUG), 1)
CFLAGS += -O0 -DDEBUG
else
CFLAGS += -O2 -s -fomit-frame-pointer
endif

# Generate dependency information
CFLAGS += -MD -MP -MF $(BUILD_DIR)/.dep/$(@F).d

#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = ../src/wp43s-dmcp/stm32_program.ld
LIBDIR =
LDFLAGS = $(CPUFLAGS) -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections -Wl,--wrap=_malloc_r

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf

../dep/DMCP_SDK/dmcp/sys/pgm_syscalls.c:
	git submodule update --init --recursive ../dep/DMCP_SDK

#######################################
# build the application
#######################################

# forcecrc32 needed because QSPI update process in DMCP requires a specific CRC
$(BUILD_DIR)/forcecrc32: $(BUILD_DIR)
	gcc ../dep/forcecrc32.c -o $(BUILD_DIR)/forcecrc32

# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti

../dep/gmp-6.2.0:
	wget -qO- https://gmplib.org/download/gmp/gmp-6.2.0.tar.lz | tar --lzip -xv -C ../dep

$(GMPLIB): ../dep/gmp-6.2.0
	cd ../dep/gmp-6.2.0 && ./configure \
		--build=x86_64-pc-linux-gnu \
		--host=arm-none-eabi \
		--disable-assembly \
		--disable-shared \
		--disable-fft \
		--disable-werror \
		--disable-cxx \
		LDFLAGS='-Wl,--fix-cortex-a8 -Wl,--no-undefined -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now' \
		CFLAGS="-nostartfiles --specs=nosys.specs -mcpu=cortex-m4 -Os -s -pedantic -fomit-frame-pointer -Wa,--noexecstack -ffunction-sections -funwind-tables -no-canonical-prefixes -fno-strict-aliasing -fstack-protector -finline-limit=64 -mthumb -march=armv7e-m -mfloat-abi=hard -mfpu=fpv4-sp-d16"
	cd ../dep/gmp-6.2.0 && make -j8 V=1

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(BUILD_DIR)/forcecrc32 $(GMPLIB) $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(OBJCOPY) --remove-section .qspi -O ihex    $@  $(BUILD_DIR)/$(TARGET)_flash.hex
	$(OBJCOPY) --remove-section .qspi -O binary  $@  $(BUILD_DIR)/$(TARGET)_flash.bin
	$(OBJCOPY) --only-section   .qspi -O ihex    $@  $(BUILD_DIR)/$(TARGET)_qspi.hex
	$(OBJCOPY) --only-section   .qspi -O binary  $@  $(BUILD_DIR)/$(TARGET)_qspi.bin
	$(BIN_DIR)modify_crc $(TARGET)
	$(BIN_DIR)check_qspi_crc $(TARGET) ../src/wp43s-dmcp/qspi_crc.h || ( $(MAKE) clean && false )
	$(BIN_DIR)add_pgm_chsum $(BUILD_DIR)/$(TARGET)_flash.bin $(BUILD_DIR)/$(TARGET).pgm
	$(SIZE) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@

$(BUILD_DIR):
	mkdir -p $@

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)/.dep $(BUILD_DIR)/*.o $(BUILD_DIR)/*.lst

#######################################
# dependencies
#######################################
-include $(shell mkdir -p $(BUILD_DIR)/.dep 2>/dev/null) $(wildcard $(BUILD_DIR)/.dep/*)

.PHONY: clean all

