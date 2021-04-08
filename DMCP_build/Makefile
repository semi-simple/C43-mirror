######################################
# target
######################################
WP43S_DMCP = WP43S

#######################################
# pathes
#######################################
# Build path
BUILD_DIR = ../build

# Path to aux build scripts (including trailing /)
# Leave empty for scripts in PATH
BIN_DIR = bin

######################################
# System sources
######################################
C_INCLUDES += -I../dep/DMCP_SDK/dmcp -I../src/wp43s-dmcp
ASM_SOURCES = ../dep/DMCP_SDK/dmcp/startup_pgm.s

#######################################
# Custom section
#######################################

# Includes
C_INCLUDES += -I../src/wp43s -I../dep/decNumberICU -I../dep/gmp-6.2.0

# C sources
SRC_DECIMAL              = $(addprefix ../dep/decNumberICU/, decContext.c decDouble.c decimal128.c decimal64.c decNumber.c decQuad.c)

SRC_WP43S                = $(SRC_DECIMAL) \
                           $(wildcard ../src/wp43s/*.c) \
                           $(wildcard ../src/wp43s/mathematics/*.c) \
                           $(wildcard ../src/wp43s/browsers/*.c) \
                           $(wildcard ../src/wp43s/logicalOps/*.c) \
                           $(wildcard ../src/wp43s/programming/*.c) \
                           $(wildcard ../src/wp43s/distributions/*.c) \
                           $(wildcard ../src/wp43s/ui/*.c)

SRC_DMCP                 = $(SRC_WP43S) \
                           ../dep/DMCP_SDK/dmcp/sys/pgm_syscalls.c
OBJ_DMCP                 = $(addprefix $(BUILD_DIR)/dmcp/,$(notdir $(SRC_DMCP:.c=.o)))
DEPS_DMCP                = $(addprefix $(BUILD_DIR)/dmcp/,$(notdir $(SRC_DMCP:.c=.d)))

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
BIN = $(OBJCOPY) -O binary -S

#######################################
# CFLAGS
#######################################
CFLAGS += -Wextra -Wall -std=c11 -MMD
# macros for gcc
C_DEFS += -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))"
CPUFLAGS += -mthumb -march=armv7e-m -mfloat-abi=hard -mfpu=fpv4-sp-d16 --specs=nosys.specs

# compile gcc flags
ASFLAGS = $(CPUFLAGS) $(OPT) -Wall -fdata-sections -ffunction-sections
CFLAGS  = $(CPUFLAGS) $(C_DEFS) $(C_INCLUDES) $(OPT) -fdata-sections -ffunction-sections
#CFLAGS += -Wno-misleading-indentation

ifdef DEBUG_WP43S
CFLAGS += -O0 -DDEBUG
else
CFLAGS += -O2 -s -fomit-frame-pointer
endif

#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = ../src/wp43s-dmcp/stm32_program.ld
LIBDIR =
LDFLAGS = $(CPUFLAGS) -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/dmcp/$(WP43S_DMCP).map,--cref -Wl,--gc-sections -Wl,--wrap=_malloc_r

dmcp: $(BUILD_DIR)/dmcp/$(WP43S_DMCP)_qspi.bin $(BUILD_DIR)/dmcp/$(WP43S_DMCP).pgm

# default action: build all
all: dmcp

../dep/DMCP_SDK/dmcp/sys/pgm_syscalls.c:
	git submodule update --init --recursive ../dep/DMCP_SDK

#######################################
# build the application
#######################################

# forcecrc32 needed because QSPI update process in DMCP requires a specific CRC
$(BUILD_DIR)/dmcp/forcecrc32: ../dep/forcecrc32.c | $(BUILD_DIR)/dmcp
	gcc ../dep/forcecrc32.c -o $(BUILD_DIR)/dmcp/forcecrc32

# list of objects
vpath %.c $(sort $(dir $(SRC_DMCP)))
# list of ASM program objects
OBJ_DMCP += $(addprefix $(BUILD_DIR)/dmcp/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

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

$(BUILD_DIR)/dmcp/%.o: %.c $(GMPLIB) | $(BUILD_DIR)/dmcp
	@echo -e "\n====> $<: $@ <===="
	$(CC) $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/dmcp/$(notdir $(<:.c=.lst)) -c -o $@ $<

$(BUILD_DIR)/dmcp/%.o: %.s | $(BUILD_DIR)/dmcp
	@echo -e "\n====> $<: $@ <===="
	$(AS) -c $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/dmcp/$(WP43S_DMCP).elf: $(BUILD_DIR)/dmcp/forcecrc32 $(GMPLIB) $(OBJ_DMCP)
	@echo -e "\n====> $(WP43S_DMCP): binary/exe $@ <===="
	$(CC) $(OBJ_DMCP) $(LDFLAGS) -o $@
	$(SIZE) $@

$(BUILD_DIR)/dmcp/$(WP43S_DMCP)_flash.bin: $(BUILD_DIR)/dmcp/$(WP43S_DMCP).elf
	@echo -e "\n====> $(WP43S_DMCP): binary/exe $@ <===="
	$(OBJCOPY) --remove-section .qspi -O binary  $<  $(BUILD_DIR)/dmcp/$(WP43S_DMCP)_flash.bin

$(BUILD_DIR)/dmcp/$(WP43S_DMCP)_qspi.bin: $(BUILD_DIR)/dmcp/$(WP43S_DMCP).elf
	@echo -e "\n====> $(WP43S_DMCP): binary/exe $@ <===="
	$(OBJCOPY) --only-section .qspi -O binary  $<  $(BUILD_DIR)/dmcp/$(WP43S_DMCP)_qspi.bin
	$(BIN_DIR)/modify_crc $(WP43S_DMCP)
	$(BIN_DIR)/check_qspi_crc $(WP43S_DMCP) ../src/wp43s-dmcp/qspi_crc.h || ( $(MAKE) clean_dmcp && false )

$(BUILD_DIR)/dmcp/$(WP43S_DMCP).pgm: $(BUILD_DIR)/dmcp/$(WP43S_DMCP)_flash.bin
	@echo -e "\n====> $(WP43S_DMCP): binary/exe $@ <===="
	$(BIN_DIR)/add_pgm_chsum $< $@

$(BUILD_DIR)/dmcp:
	mkdir -p $@

#######################################
# clean up
#######################################
clean_dmcp:
	-rm -fR $(BUILD_DIR)/dmcp/.dep $(BUILD_DIR)/dmcp/*.o $(BUILD_DIR)/dmcp/*.lst

#######################################
# dependencies
#######################################
-include $(DEPS_DMCP)

.PHONY: clean_dmcp dmcp all

