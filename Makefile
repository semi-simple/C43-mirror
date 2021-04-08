# $@ target
# $< 1st dependency
# $^ dependency list with path
# $+ like $^ without duplicates
# $? out of date dependency list
# $* target without extension

BUILD_DIR = build
GENERATECONSTANTS_APP = $(BUILD_DIR)/generateConstants/generateConstants$(EXE)
GENERATECATALOGS_APP = $(BUILD_DIR)/generateCatalogs/generateCatalogs$(EXE)
GENERATETESTPGMS_APP = $(BUILD_DIR)/generateTestPgms/generateTestPgms$(EXE)
TTF2RASTERFONTS_APP = $(BUILD_DIR)/ttf2RasterFonts/ttf2RasterFonts$(EXE)
TESTTTF2RASTERFONTS_APP = testTtf2RasterFonts$(EXE)
TESTSUITE_APP = testSuite$(EXE)
WP43S_APP = wp43s$(EXE)
EXE =

CC = gcc

ifeq ($(OS),Windows_NT)
  EXE = .exe
  CFLAGS += -DWIN32
  detected_OS := Windows
else
  UNAME_S := $(shell uname -s)
  ifeq ($(UNAME_S),Linux)
    detected_OS := Linux
    CFLAGS += -DLINUX
  endif
  ifeq ($(UNAME_S),Darwin)
    detected_OS := Darwin
    CC = clang
    CFLAGS += -DOSX
    ifneq ($(wildcard /opt/homebrew/.),)
      # Homebrew on Arm Macs installs into /opt/homebrew
      CFLAGS += -I/opt/homebrew/include
      LDFLAGS += -L/opt/homebrew/lib
    else
      CFLAGS += -I/usr/local/include/
      LDFLAGS += -L/usr/local/lib
    endif
  endif
endif

RASPBERRY = $(shell ./onARaspberry)

INC = -Idep/decNumberICU -Isrc/wp43s -Isrc/testSuite

ifeq ($(DEST), gitlab)
	CFLAGS += -march=x86-64
else ifeq ($(detected_OS),Darwin)
	# Don't add -march=native on macOS as it isn't supported in clang on Arm
else
	CFLAGS += -march=native
endif

ifdef DEBUG_WP43S
	CFLAGS += -g
else
	CFLAGS += -O2
endif

CFLAGS += -Wextra -Wall -std=c11 -DPC_BUILD -MMD

ifeq ($(RASPBERRY),YES32)
        CFLAGS += -DOS32BIT -DRASPBERRY
endif

ifeq ($(RASPBERRY),YES64)
        CFLAGS += -DOS64BIT -DRASPBERRY -fshort-enums
endif

ifeq ($(RASPBERRY),NO32)
        CFLAGS += -DOS32BIT
endif

ifeq ($(RASPBERRY),NO64)
        CFLAGS += -DOS64BIT -fshort-enums -m64
        LDFLAGS += -m64
endif

CFLAGS += `pkg-config --cflags freetype2` `pkg-config --cflags gtk+-3.0`

LDFLAGS += -lgmp -lm `pkg-config --libs freetype2` `pkg-config --libs gtk+-3.0`

SRC_DECIMAL              = $(addprefix dep/decNumberICU/, decContext.c decDouble.c decimal128.c decimal64.c decNumber.c decQuad.c)

SRC_WP43S                = $(SRC_DECIMAL) \
                           $(wildcard src/wp43s/*.c) \
                           $(wildcard src/wp43s/mathematics/*.c) \
                           $(wildcard src/wp43s/browsers/*.c) \
                           $(wildcard src/wp43s/logicalOps/*.c) \
                           $(wildcard src/wp43s/programming/*.c) \
                           $(wildcard src/wp43s/distributions/*.c) \
                           $(wildcard src/wp43s/ui/*.c)

SRC_SIMULATOR            = $(SRC_WP43S) \
                           $(wildcard src/wp43s-gtk/*.c)
OBJ_SIMULATOR            = $(addprefix $(BUILD_DIR)/simulator/,$(notdir $(SRC_SIMULATOR:.c=.o)))
DEPS_SIMULATOR           = $(addprefix $(BUILD_DIR)/simulator/,$(notdir $(SRC_SIMULATOR:.c=.d)))

SRC_TESTSUITE            = $(SRC_WP43S) \
                           $(addprefix src/testSuite/, testSuite.c)
OBJ_TESTSUITE            = $(addprefix $(BUILD_DIR)/testSuite/,$(notdir $(SRC_TESTSUITE:.c=.o)))
DEPS_TESTSUITE           = $(addprefix $(BUILD_DIR)/testSuite/,$(notdir $(SRC_TESTSUITE:.c=.d)))

SRC_GENERATECONSTANTS    = $(SRC_DECIMAL) \
                           $(addprefix src/generateConstants/, generateConstants.c)
OBJ_GENERATECONSTANTS    = $(addprefix $(BUILD_DIR)/generateConstants/,$(notdir $(SRC_GENERATECONSTANTS:.c=.o)))
DEPS_GENERATECONSTANTS   = $(addprefix $(BUILD_DIR)/generateConstants/,$(notdir $(SRC_GENERATECONSTANTS:.c=.d)))

SRC_GENERATECATALOGS     = $(addprefix src/generateCatalogs/, generateCatalogs.c) \
                           $(addprefix src/wp43s/, charString.c fonts.c items.c rasterFontsData.c sort.c)
OBJ_GENERATECATALOGS     = $(addprefix $(BUILD_DIR)/generateCatalogs/,$(notdir $(SRC_GENERATECATALOGS:.c=.o)))
DEPS_GENERATECATALOGS    = $(addprefix $(BUILD_DIR)/generateCatalogs/,$(notdir $(SRC_GENERATECATALOGS:.c=.d)))

SRC_GENERATETESTPGMS    = $(addprefix src/generateTestPgms/, generateTestPgms.c)
OBJ_GENERATETESTPGMS    = $(addprefix $(BUILD_DIR)/generateTestPgms/,$(notdir $(SRC_GENERATETESTPGMS:.c=.o)))
DEPS_GENERATETESTPGMS   = $(addprefix $(BUILD_DIR)/generateTestPgms/,$(notdir $(SRC_GENERATETESTPGMS:.c=.d)))

SRC_TTF2RASTERFONTS      = $(addprefix src/ttf2RasterFonts/, ttf2RasterFonts.c)
OBJ_TTF2RASTERFONTS      = $(addprefix $(BUILD_DIR)/ttf2RasterFonts/,$(notdir $(SRC_TTF2RASTERFONTS:.c=.o)))
DEPS_TTF2RASTERFONTS     = $(addprefix $(BUILD_DIR)/ttf2RasterFonts/,$(notdir $(SRC_TTF2RASTERFONTS:.c=.d)))

SRC_TESTTTF2RASTERFONTS  = $(addprefix src/ttf2RasterFonts/, testTtf2RasterFonts.c) \
                           $(addprefix src/wp43s/, rasterFontsData.c)
OBJ_TESTTTF2RASTERFONTS  = $(addprefix $(BUILD_DIR)/ttf2RasterFonts/,$(notdir $(SRC_TESTTTF2RASTERFONTS:.c=.o)))
DEPS_TESTTTF2RASTERFONTS = $(addprefix $(BUILD_DIR)/ttf2RasterFonts/,$(notdir $(SRC_TESTTTF2RASTERFONTS:.c=.d)))

GEN_SRC_CONSTANTPOINTERS = $(addprefix src/wp43s/, constantPointers.c constantPointers.h)
GEN_SRC_RASTERFONTSDATA  = $(addprefix src/wp43s/, rasterFontsData.c)
GEN_SRC_SOFTMENUCATALOGS = $(addprefix src/wp43s/, softmenuCatalogs.h)
GEN_BIN_TESTPGMS         = $(addprefix binaries/dmcp/, testPgms.bin)

GENERATED_SOURCES = $(GEN_SRC_CONSTANTPOINTERS) $(GEN_SRC_RASTERFONTSDATA) $(GEN_SRC_SOFTMENUCATALOGS) $(GEN_BIN_TESTPGMS)

STAMP_FILES = $(BUILD_DIR)/.stamp-constantPointers $(BUILD_DIR)/.stamp-rasterFontsData $(BUILD_DIR)/.stamp-softmenuCatalog $(BUILD_DIR)/.stamp-testPgms

vpath %.c dep/decNumberICU src/testSuite src/generateConstants src/generateCatalogs src/generateTestPgms src/ttf2RasterFonts \
          src/wp43s src/wp43s/mathematics src/wp43s/browsers src/wp43s/logicalOps src/wp43s/programming src/wp43s/distributions \
          src/wp43s/ui src/wp43s-gtk

all: 	wp43s
ifeq '$(detected_OS)' 'Darwin'
	rsync -u wp43s binaries/macOS
endif

rebuild:
	$(MAKE) mrproper
	$(MAKE) all

ifeq ($(CI_COMMIT_TAG),)
  WIN_DIST_DIR = wp43s-windows
  MAC_DIST_DIR = wp43s-macos
  DM_DIST_DIR = wp43s-dm42
else
  WIN_DIST_DIR = wp43s-windows-$(CI_COMMIT_TAG)
  MAC_DIST_DIR = wp43s-macos-$(CI_COMMIT_TAG)
  DM_DIST_DIR = wp43s-dm42-$(CI_COMMIT_TAG)
endif

dist_windows:	wp43s.exe
	mkdir -p $(WIN_DIST_DIR)/artwork $(WIN_DIST_DIR)/binaries/dmcp
	cp wp43s.exe $(WIN_DIST_DIR)/
	cp artwork/*.png $(WIN_DIST_DIR)/artwork/
	cp binaries/dmcp/testPgms.bin $(WIN_DIST_DIR)/binaries/dmcp/
	cp wp43s_pre.css $(WIN_DIST_DIR)/
#	zip -r wp43s-windows.zip $(WIN_DIST_DIR)

dist_macos:	wp43s
	mkdir -p $(MAC_DIST_DIR)/artwork $(MAC_DIST_DIR)/binaries/dmcp
	cp wp43s $(MAC_DIST_DIR)/
	cp artwork/*.png $(MAC_DIST_DIR)/artwork/
	cp binaries/dmcp/testPgms.bin $(MAC_DIST_DIR)/binaries/dmcp/
	cp wp43s_pre.css $(MAC_DIST_DIR)/
	zip -r wp43s-macos.zip $(MAC_DIST_DIR)

dist_dm42:
	cd DMCP_build && make
	mkdir -p $(DM_DIST_DIR)
	cp build/dmcp/WP43S.pgm build/dmcp/WP43S_qspi.bin $(DM_DIST_DIR)
	cp -r offimg $(DM_DIST_DIR)
	cp binaries/dmcp/keymap.bin binaries/dmcp/original_DM42_keymap.bin binaries/dmcp/testPgms.bin $(DM_DIST_DIR)
	zip -r wp43s-dm42.zip $(DM_DIST_DIR)

.PHONY: clean_wp43s clean_generateConstants clean_generateCatalogs clean_generateTestPgms clean_ttf2RasterFonts clean_testTtf2RasterFonts clean_testSuite all clean_all mrproper sources rebuild dist_macos

ifneq ($(EXE),)
generateConstants: $(GENERATECONSTANTS_APP)
generateCatalogs: $(GENERATECATALOGS_APP)
generateTestPgms: $(GENERATETESTPGMS_APP)
ttf2RasterFonts: $(TTF2RASTERFONTS_APP)
testTtf2RasterFonts: $(TESTTTF2RASTERFONTS_APP)
testSuite: $(TESTSUITE_APP)
wp43s: $(WP43S_APP)

.PHONY: generateConstants generateCatalogs generateTestPgms ttf2RasterFonts testTtf2RasterFonts testSuite wp43s
endif

sources: $(STAMP_FILES)

clean_all: clean_wp43s clean_generateConstants clean_generateCatalogs clean_generateTestPgms clean_ttf2RasterFonts clean_testTtf2RasterFonts clean_testSuite

mrproper: clean_all
	#rm -f $(GENERATED_SOURCES)
	rm -f $(STAMP_FILES)
	rm -f $(GENERATECONSTANTS_APP)
	rm -f $(GENERATECATALOGS_APP)
	rm -f $(GENERATETESTPGMS_APP)
	rm -f $(TTF2RASTERFONTS_APP)
	rm -f $(TESTTTF2RASTERFONTS_APP)
	rm -f $(WP43S_APP)
	rm -f $(TESTSUITE_APP)
	rm -f gmon.out


$(BUILD_DIR)/simulator:
	mkdir -p $(BUILD_DIR)/simulator

$(BUILD_DIR)/testSuite:
	mkdir -p $(BUILD_DIR)/testSuite

$(BUILD_DIR)/generateConstants:
	mkdir -p $(BUILD_DIR)/generateConstants

$(BUILD_DIR)/generateCatalogs:
	mkdir -p $(BUILD_DIR)/generateCatalogs

$(BUILD_DIR)/generateTestPgms:
	mkdir -p $(BUILD_DIR)/generateTestPgms

$(BUILD_DIR)/ttf2RasterFonts:
	mkdir -p $(BUILD_DIR)/ttf2RasterFonts



clean_generateConstants:
	rm -rf $(BUILD_DIR)/generateConstants

-include $(DEPS_GENERATECONSTANTS)

$(GENERATECONSTANTS_APP): $(OBJ_GENERATECONSTANTS)
	@echo -e "\n====> $(GENERATECONSTANTS_APP): binary/exe $@ <===="
	$(CC) $(CFLAGS) $(OBJ_GENERATECONSTANTS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/generateConstants/%.o: %.c | $(BUILD_DIR)/generateConstants
	@echo -e "\n====> $<: $@ <===="
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

$(BUILD_DIR)/.stamp-constantPointers: $(GENERATECONSTANTS_APP)
	@echo -e "\n====> running $(GENERATECONSTANTS_APP) <===="
	./$(GENERATECONSTANTS_APP)
	touch $@

$(GEN_SRC_CONSTANTPOINTERS): $(BUILD_DIR)/.stamp-constantPointers



clean_generateCatalogs:
	rm -rf $(BUILD_DIR)/generateCatalogs

-include $(DEPS_GENERATECATALOGS)

$(GENERATECATALOGS_APP): CFLAGS += -DGENERATE_CATALOGS
$(GENERATECATALOGS_APP): $(OBJ_GENERATECATALOGS)
	@echo -e "\n====> $(GENERATECATALOGS_APP): binary/exe $@ <===="
	$(CC) $(CFLAGS) $(OBJ_GENERATECATALOGS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/generateCatalogs/%.o: %.c $(BUILD_DIR)/.stamp-constantPointers | $(BUILD_DIR)/generateCatalogs
	@echo -e "\n====> $<: $@ <===="
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

$(BUILD_DIR)/.stamp-softmenuCatalog: $(GENERATECATALOGS_APP)
	@echo -e "\n====> running $(GENERATECATALOGS_APP) <===="
	./$(GENERATECATALOGS_APP)
	touch $@

$(GEN_SRC_SOFTMENUCATALOGS): $(BUILD_DIR)/.stamp-softmenuCatalog



clean_generateTestPgms:
	rm -rf $(BUILD_DIR)/generateTestPgms

-include $(DEPS_GENERATETESTPGMS)

$(GENERATETESTPGMS_APP): $(OBJ_GENERATETESTPGMS)
	@echo -e "\n====> $(GENERATETESTPGMS_APP): binary/exe $@ <===="
	$(CC) $(CFLAGS) $(OBJ_GENERATETESTPGMS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/generateTestPgms/%.o: %.c | $(BUILD_DIR)/generateTestPgms
	@echo -e "\n====> $<: $@ <===="
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

$(BUILD_DIR)/.stamp-testPgms: $(GENERATETESTPGMS_APP)
	@echo -e "\n====> running $(GENERATETESTPGMS_APP) <===="
	./$(GENERATETESTPGMS_APP)
	touch $@

$(GEN_BIN_TESTPGMS): $(BUILD_DIR)/.stamp-testPgms



clean_ttf2RasterFonts:
	rm -rf $(BUILD_DIR)/ttf2RasterFonts

-include $(DEPS_TTF2RASTERFONTS)

$(TTF2RASTERFONTS_APP): $(OBJ_TTF2RASTERFONTS)
	@echo -e "\n====> $(TTF2RASTERFONTS_APP): binary/exe $@ <===="
	$(CC) $(CFLAGS) $(OBJ_TTF2RASTERFONTS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/ttf2RasterFonts/%.o: %.c | $(BUILD_DIR)/ttf2RasterFonts
	@echo -e "\n====> $<: $@ <===="
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

$(BUILD_DIR)/.stamp-rasterFontsData: $(TTF2RASTERFONTS_APP) fonts/WP43S_NumericFont.ttf fonts/WP43S_StandardFont.ttf
	@echo -e "\n====> running $(TTF2RASTERFONTS_APP) <===="
	./$(TTF2RASTERFONTS_APP) > /dev/null
	touch $@

$(BUILD_DIR)/ttf2RasterFonts/rasterFontsData.o: $(BUILD_DIR)/.stamp-rasterFontsData

$(GEN_SRC_RASTERFONTSDATA): $(BUILD_DIR)/.stamp-rasterFontsData


clean_testTtf2RasterFonts:
	rm -f $(BUILD_DIR)/ttf2RasterFonts/testTtf2RasterFonts.o

-include $(DEPS_TESTTTF2RASTERFONTS)

$(TESTTTF2RASTERFONTS_APP): $(OBJ_TESTTTF2RASTERFONTS)
	@echo -e "\n====> $(TESTTTF2RASTERFONTS_APP): binary/exe <===="
	$(CC) $(CFLAGS) $(OBJ_TESTTTF2RASTERFONTS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/ttf2RasterFonts/testTtf2RasterFonts.o: $(BUILD_DIR)/.stamp-constantPointers


clean_testSuite:
	rm -rf $(BUILD_DIR)/testSuite

-include $(DEPS_TESTSUITE)

$(TESTSUITE_APP): CFLAGS += -DTESTSUITE_BUILD
$(TESTSUITE_APP): $(OBJ_TESTSUITE)
	@echo -e "\n====> $(TESTSUITE_APP): binary/exe $@ <===="
	$(CC) $(CFLAGS) $(OBJ_TESTSUITE) -o $(TESTSUITE_APP) $(LDFLAGS)

$(BUILD_DIR)/testSuite/%.o: %.c $(BUILD_DIR)/.stamp-constantPointers | $(BUILD_DIR)/testSuite
	@echo -e "\n====> $<: $@ <===="
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<



clean_wp43s:
	rm -rf $(BUILD_DIR)/simulator

-include $(DEPS_SIMULATOR)

$(WP43S_APP): $(OBJ_SIMULATOR)
	@echo -e "\n====> $(WP43S_APP): binary/exe $@ <===="
	$(CC) $(CFLAGS) $(OBJ_SIMULATOR) -o $(WP43S_APP) $(LDFLAGS)

$(BUILD_DIR)/simulator/%.o: %.c $(BUILD_DIR)/.stamp-constantPointers $(BUILD_DIR)/.stamp-softmenuCatalog $(BUILD_DIR)/.stamp-testPgms | $(BUILD_DIR)/simulator
	@echo -e "\n====> $<: $@ <===="
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<
