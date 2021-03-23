# $@ target
# $< 1st dependency
# $^ dependency list with path
# $+ like $^ without duplicates
# $? out of date dependency list
# $* target without extension

GENERATECONSTANTS_APP = generateConstants$(EXE)
GENERATECATALOGS_APP = generateCatalogs$(EXE)
GENERATETESTPGMS_APP = generateTestPgms$(EXE)
TTF2RASTERFONTS_APP = ttf2RasterFonts$(EXE)
TESTTTF2RASTERFONTS_APP = testTtf2RasterFonts$(EXE)
TESTSUITE_APP = testSuite$(EXE)
WP43S_APP = wp43c$(EXE)
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
#    CC = gcc
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

INC = -IdecNumberICU -Isrc/wp43s -Isrc/testSuite

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

ifeq ($(RASPBERRY),YES32)
        CFLAGS += -Wextra -Wall -std=c11 -DPC_BUILD -DOS32BIT -DRASPBERRY -MMD
endif

ifeq ($(RASPBERRY),YES64)
        CFLAGS += -Wextra -Wall -std=c11 -DPC_BUILD -DOS64BIT -DRASPBERRY -MMD -fshort-enums
endif

ifeq ($(RASPBERRY),NO32)
        CFLAGS += -Wextra -Wall -std=c11 -DPC_BUILD -DOS32BIT -MMD
endif

ifeq ($(RASPBERRY),NO64)
        CFLAGS += -Wextra -Wall -std=c11 -DPC_BUILD -DOS64BIT -MMD -fshort-enums -m64
        LDFLAGS += -m64
endif

CFLAGS += `pkg-config --cflags freetype2` `pkg-config --cflags gtk+-3.0`

LDFLAGS += -lgmp -lm `pkg-config --libs freetype2` `pkg-config --libs gtk+-3.0`

SRC_DECIMAL              = $(addprefix decNumberICU/, decContext.c decDouble.c decimal128.c decimal64.c decNumber.c decQuad.c)
OBJ_DECIMAL              = $(SRC_DECIMAL:.c=.o)
DEPS_DECIMAL             = $(SRC_DECIMAL:.c=.d)

SRC_WP43S                = \
	$(addprefix src/wp43s/, \
		assign.c bufferize.c charString.c config.c constantPointers.c \
		constants.c conversionAngles.c conversionUnits.c \
		curveFitting.c dateTime.c debug.c display.c error.c flags.c \
		fonts.c fractions.c gui.c integers.c items.c keyboard.c \
        plotstat.c \
		rasterFontsData.c recall.c registerValueConversions.c registers.c \
		saveRestoreCalcState.c screen.c softmenus.c sort.c stack.c \
		stats.c statusBar.c store.c stringFuncs.c timer.c \
		wp43s.c memory.c) \
	$(addprefix src/wp43s/mathematics/, \
		10pow.c 2pow.c addition.c agm.c arccos.c arccosh.c arcsin.c arcsinh.c arctan.c arctanh.c arg.c bessel.c bn.c \
		ceil.c changeSign.c checkValue.c compare.c comparisonReals.c conjugate.c cos.c cosh.c cross.c cube.c cubeRoot.c \
		cxToRe.c cpyx.c dblDivision.c dblMultiplication.c deltaPercent.c idiv.c idivr.c decomp.c dot.c \
		division.c erf.c erfc.c exp.c expMOne.c expt.c factorial.c fib.c floor.c fractionalPart.c gamma.c gammaP.c gammaQ.c gammaXyLower.c gammaXyUpper.c gcd.c gd.c \
		imaginaryPart.c int.c incDec.c integerPart.c invert.c iteration.c ixyz.c lcm.c ln.c lnPOne.c log10.c logxy.c lnbeta.c beta.c \
		log2.c magnitude.c mant.c max.c mean.c min.c minusOnePow.c modulo.c multiplication.c neighb.c opmod.c ortho_polynom.c parallel.c pcg_basic.c \
		percent.c percentMRR.c percentPlusMG.c percentSigma.c percentT.c power.c prime.c \
		random.c rdp.c realPart.c remainder.c reToCx.c round.c roundi.c rsd.c shiftDigits.c sign.c sin.c sinc.c sincpi.c sinh.c slvq.c square.c squareRoot.c \
		subtraction.c swapRealImaginary.c tan.c tanh.c toPolar.c toRect.c ulp.c unitVector.c xthRoot.c\
		variance.c w_inverse.c w_negative.c w_positive.c wp34s.c zeta.c) \
	$(addprefix src/wp43s/programming/, \
		decode.c lblGtoXeq.c manage.c nextStep.c) \
	$(addprefix src/wp43s/logicalOps/, \
		and.c countBits.c mask.c nand.c nor.c not.c or.c rotateBits.c setClearFlipBits.c xnor.c xor.c) \
	$(addprefix src/wp43s/distributions/, \
		chi2.c f.c normal.c t.c) \
	$(addprefix src/wp43s/browsers/, \
		flagBrowser.c fontBrowser.c registerBrowser.c) \
	$(addprefix src/wp43s/ui/, \
		tam.c) \
	$(addprefix src/wp43s/, \
		addons.c graphs.c jm.c jm_graph.c inlineTest.c keyboardTweak.c radioButtonCatalog.c  graphText.c textfiles.c xeqm.c)
OBJ_WP43S                = $(SRC_WP43S:.c=.o) $(OBJ_DECIMAL)
DEPS_WP43S               = $(SRC_WP43S:.c=.d)

SRC_TESTSUITE            = $(addprefix src/testSuite/, testSuite.c)
OBJ_TESTSUITE            = $(SRC_TESTSUITE:.c=.o) $(OBJ_WP43S:.o=.ts.o)
DEPS_TESTSUITE           = $(OBJ_TESTSUITE:.o=.d)

SRC_GENERATECONSTANTS    = $(addprefix src/generateConstants/, generateConstants.c)
OBJ_GENERATECONSTANTS    = $(SRC_GENERATECONSTANTS:.c=.o) $(OBJ_DECIMAL)
DEPS_GENERATECONSTANTS   = $(SRC_GENERATECONSTANTS:.c=.d)

SRC_GENERATECATALOGS     = $(addprefix src/generateCatalogs/, generateCatalogs.c)
SRC_GENERATECATALOGS2    = $(addprefix src/wp43s/, charString.c fonts.c items.c rasterFontsData.c sort.c)
OBJ_GENERATECATALOGS     = $(SRC_GENERATECATALOGS:.c=.o) $(SRC_GENERATECATALOGS2:.c=.gc.o)
DEPS_GENERATECATALOGS    = $(SRC_GENERATECATALOGS:.c=.d) $(SRC_GENERATECATALOGS2:.c=.d)

SRC_GENERATETESTPGMS    = $(addprefix src/generateTestPgms/, generateTestPgms.c)
OBJ_GENERATETESTPGMS    = $(SRC_GENERATETESTPGMS:.c=.o)
DEPS_GENERATETESTPGMS   = $(SRC_GENERATETESTPGMS:.c=.d)

SRC_TTF2RASTERFONTS      = $(addprefix src/ttf2RasterFonts/, ttf2RasterFonts.c)
OBJ_TTF2RASTERFONTS      = $(SRC_TTF2RASTERFONTS:.c=.o)
DEPS_TTF2RASTERFONTS     = $(SRC_TTF2RASTERFONTS:.c=.d)

SRC_TESTTTF2RASTERFONTS  = $(addprefix src/ttf2RasterFonts/, testTtf2RasterFonts.c)
OBJ_TESTTTF2RASTERFONTS  = $(SRC_TESTTTF2RASTERFONTS:.c=.o) src/wp43s/rasterFontsData.o
DEPS_TESTTTF2RASTERFONTS = $(SRC_TESTTTF2RASTERFONTS:.c=.d)

GEN_SRC_CONSTANTPOINTERS = $(addprefix src/wp43s/, constantPointers.c constantPointers.h)
GEN_SRC_RASTERFONTSDATA  = $(addprefix src/wp43s/, rasterFontsData.c)
GEN_SRC_SOFTMENUCATALOGS = $(addprefix src/wp43s/, softmenuCatalogs.h)
GEN_BIN_TESTPGMS         = $(addprefix DM42 binary/, testPgms.bin)

GENERATED_SOURCES = $(GEN_SRC_CONSTANTPOINTERS) $(GEN_SRC_RASTERFONTSDATA) $(GEN_SRC_SOFTMENUCATALOGS) $(GEN_BIN_TESTPGMS)

STAMP_FILES = .stamp-constantPointers .stamp-rasterFontsData .stamp-softmenuCatalog .stamp-testPgms

all: 	wp43c
ifeq '$(detected_OS)' 'Darwin'
	rsync -u wp43c MacOs\ binaries
	rsync -u c43_pre.css MacOs\ binaries
	rsync -u dm42l_L1.png MacOs\ binaries
endif

rebuild:
	$(MAKE) mrproper
	$(MAKE) all

.PHONY: clean_wp43s clean_generateConstants clean_generateCatalogs clean_generateTestPgms clean_ttf2RasterFonts clean_testTtf2RasterFonts clean_testSuite all clean_all mrproper decNumberICU sources rebuild

ifneq ($(EXE),)
generateConstants: $(GENERATECONSTANTS_APP)
generateCatalogs: $(GENERATECATALOGS_APP)
generateTestPgms: $(GENERATETESTPGMS_APP)
ttf2RasterFonts: $(TTF2RASTERFONTS_APP)
testTtf2RasterFonts: $(TESTTTF2RASTERFONTS_APP)
testSuite: $(TESTSUITE_APP)
wp43c: $(WP43S_APP)

.PHONY: generateConstants generateCatalogs generateTestPgms ttf2RasterFonts testTtf2RasterFonts testSuite wp43s
endif

sources: $(STAMP_FILES)

clean_all: clean_decNumberICU clean_wp43s clean_generateConstants clean_generateCatalogs clean_generateTestPgms clean_ttf2RasterFonts clean_testTtf2RasterFonts clean_testSuite

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


clean_decNumberICU:
	rm -f $(OBJ_DECIMAL)
	rm -f $(DEPS_DECIMAL)

-include $(DEPS_DECIMAL)

decNumberICU/%.o: decNumberICU/%.c
	@echo -e "\n====> decNumberICU/%.o: $@ <===="
	$(CC) $(CFLAGS) -c -o $@ $<



clean_generateConstants:
	rm -f $(OBJ_GENERATECONSTANTS)
	rm -f $(DEPS_GENERATECONSTANTS)

-include $(DEPS_GENERATECONSTANTS)

$(GENERATECONSTANTS_APP): $(OBJ_GENERATECONSTANTS)
	@echo -e "\n====> $(GENERATECONSTANTS_APP): binary/exe $@ <===="
	$(CC) $(CFLAGS) $(OBJ_GENERATECONSTANTS) -o $@ $(LDFLAGS)

src/generateConstants/%.o: src/generateConstants/%.c
	@echo -e "\n====> src/generateConstants/%.o: $@ <===="
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

.stamp-constantPointers: $(GENERATECONSTANTS_APP)
	@echo -e "\n====> running $(GENERATECONSTANTS_APP) <===="
	./$(GENERATECONSTANTS_APP)
	touch $@

$(GEN_SRC_CONSTANTPOINTERS): .stamp-constantPointers



clean_generateCatalogs:
	rm -f $(OBJ_GENERATECATALOGS)
	rm -f $(DEPS_GENERATECATALOGS)

-include $(DEPS_GENERATECATALOGS)

$(GENERATECATALOGS_APP): CFLAGS += -DGENERATE_CATALOGS
$(GENERATECATALOGS_APP): $(OBJ_GENERATECATALOGS)
	@echo -e "\n====> $(GENERATECATALOGS_APP): binary/exe $@ <===="
	$(CC) $(CFLAGS) $(OBJ_GENERATECATALOGS) -o $@ $(LDFLAGS)

src/generateCatalogs/%.o: src/generateCatalogs/%.c
	@echo -e "\n====> src/generateCatalogs/%.o: $@ <===="
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

src/wp43s/%.gc.o: src/wp43s/%.c .stamp-constantPointers
	@echo -e "\n====> src/wp43s/%.gc.o: $@ <===="
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

.stamp-softmenuCatalog: $(GENERATECATALOGS_APP)
	@echo -e "\n====> running $(GENERATECATALOGS_APP) <===="
	./$(GENERATECATALOGS_APP)
	touch $@

$(GEN_SRC_SOFTMENUCATALOGS): .stamp-softmenuCatalog



clean_generateTestPgms:
	rm -f $(OBJ_GENERATETESTPGMS)
	rm -f $(DEPS_GENERATETESTPGMS)

-include $(DEPS_GENERATETESTPGMS)

$(GENERATETESTPGMS_APP): $(OBJ_GENERATETESTPGMS)
	@echo -e "\n====> $(GENERATETESTPGMS_APP): binary/exe $@ <===="
	$(CC) $(CFLAGS) $(OBJ_GENERATETESTPGMS) -o $@ $(LDFLAGS)

src/generateTestPgms/%.o: src/generateTestPgms/%.c
	@echo -e "\n====> src/generateTestPgms/%.o: $@ <===="
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

.stamp-testPgms: $(GENERATETESTPGMS_APP)
	@echo -e "\n====> running $(GENERATETESTPGMS_APP) <===="
	./$(GENERATETESTPGMS_APP)
	touch $@

$(GEN_BIN_TESTPGMS): .stamp-testPgms



clean_ttf2RasterFonts:
	rm -f $(OBJ_TTF2RASTERFONTS)
	rm -f $(DEPS_TTF2RASTERFONTS)

-include $(DEPS_TTF2RASTERFONTS)

$(TTF2RASTERFONTS_APP): $(OBJ_TTF2RASTERFONTS)
	@echo -e "\n====> $(TTF2RASTERFONTS_APP): binary/exe $@ <===="
	$(CC) $(CFLAGS) $(OBJ_TTF2RASTERFONTS) -o $@ $(LDFLAGS)

src/ttf2RasterFonts/%.o: src/ttf2RasterFonts/%.c
	@echo -e "\n====> src/ttf2RasterFonts/%.o: $@ <===="
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

.stamp-rasterFontsData: $(TTF2RASTERFONTS_APP) fonts/C43___NumericFont.ttf fonts/C43___StandardFont.ttf
#.stamp-rasterFontsData: $(TTF2RASTERFONTS_APP) fonts/WP43S_NumericFont.ttf fonts/WP43S_StandardFont.ttf
	@echo -e "\n====> running $(TTF2RASTERFONTS_APP) <===="
	./$(TTF2RASTERFONTS_APP) > /dev/null
	touch $@

src/wp43s/rasterFontsData.o: .stamp-rasterFontsData

$(GEN_SRC_RASTERFONTSDATA): .stamp-rasterFontsData


clean_testTtf2RasterFonts:
	rm -f $(OBJ_TESTTTF2RASTERFONTS)
	rm -f $(DEPS_TESTTTF2RASTERFONTS)

-include $(DEPS_TESTTTF2RASTERFONTS)

$(TESTTTF2RASTERFONTS_APP): $(OBJ_TESTTTF2RASTERFONTS)
	@echo -e "\n====> $(TESTTTF2RASTERFONTS_APP): binary/exe <===="
	$(CC) $(CFLAGS) $(OBJ_TESTTTF2RASTERFONTS) -o $@ $(LDFLAGS)

src/ttf2RasterFonts/testTtf2RasterFonts.o: .stamp-constantPointers


clean_testSuite:
	rm -f $(OBJ_TESTSUITE)
	rm -f $(DEPS_TESTSUITE) $(DEPS_WP43S)

-include $(DEPS_TESTSUITE)

$(TESTSUITE_APP): CFLAGS += -DTESTSUITE_BUILD
$(TESTSUITE_APP): $(OBJ_TESTSUITE)
	@echo -e "\n====> $(TESTSUITE_APP): binary/exe $@ <===="
	$(CC) $(CFLAGS) $(OBJ_TESTSUITE) -o $(TESTSUITE_APP) $(LDFLAGS)

src/testSuite/%.o: src/testSuite/%.c .stamp-constantPointers
	@echo -e "\n====> src/testSuite/%.o: $@ <===="
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

src/wp43s/%.ts.o: src/wp43s/%.c .stamp-constantPointers
	@echo -e "\n====> src/wp43s/%.ts.o: $@ <===="
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

decNumberICU/%.ts.o: decNumberICU/%.c
	@echo -e "\n====> decNumberICU/%.ts.o: $@ <===="
	$(CC) $(CFLAGS) -c -o $@ $<



clean_wp43s:
	rm -f $(OBJ_WP43S)
	rm -f $(DEPS_WP43S)

-include $(DEPS_WP43S)

$(WP43S_APP): $(OBJ_WP43S)
	@echo -e "\n====> $(WP43S_APP): binary/exe $@ <===="
	$(CC) $(CFLAGS) $(OBJ_WP43S) -o $(WP43S_APP) $(LDFLAGS)

src/wp43s/%.o: src/wp43s/%.c .stamp-constantPointers .stamp-softmenuCatalog .stamp-testPgms
	@echo -e "\n====> src/wp43s/%.o: $@ <===="
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<
