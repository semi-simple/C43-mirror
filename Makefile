# $@ target
# $< 1st dependency
# $^ dependency list
# $? recent dependency list
# $* target without extension


GENERATECONSTANTS_APP = generateConstants
TTF2RASTERFONTS_APP = ttf2RasterFonts
TESTTTF2RASTERFONTS_APP = testTtf2RasterFonts
WP43S_APP = wp43s


ifeq '$(findstring ;,$(PATH))' ';'
    detected_OS := Windows
else
    detected_OS := $(shell uname 2>/dev/null || echo Unknown)
    detected_OS := $(patsubst CYGWIN%,Cygwin,$(detected_OS))
    detected_OS := $(patsubst MSYS%,MSYS,$(detected_OS))
    detected_OS := $(patsubst MINGW%,MSYS,$(detected_OS))
endif

ifeq ($(detected_OS),Windows)
    CFLAGS += -D WIN32
    GENERATECONSTANTS_APP = $(GENERATECONSTANTS_APP).exe
    TTF2RASTERFONTS_APP = $(TTF2RASTERFONTS_APP).exe
    TESTTTF2RASTERFONTS_APP = $(TESTTTF2RASTERFONTS_APP).exe
    WP43S_APP = $(WP43S_APP).exe
else ifeq ($(detected_OS),Darwin)        # Mac OS X
    CFLAGS += -D OSX
    LDFLAGS += -L/opt/local/libs
else ifeq ($(detected_OS),Linux)
    CFLAGS += -D LINUX
endif


 
CC = gcc
INC = -IdecNumberICU -Isrc/wp43s

ifeq ($(DEST), gitlab)
  CFLAGS += -march=x86-64
else
  CFLAGS += -march=native
endif

ifdef DEBUG_WP43S
  CFLAGS += -g
else
  CFLAGS += -O2 -s
endif

CFLAGS += -Wextra -Wall -std=c11 -m64 -fshort-enums -fomit-frame-pointer -DPC_BUILD -DTFM_NO_ASM -DTFM_X86_64

LDFLAGS += -m64

SRC_DECIMAL              = \
	$(addprefix decNumberICU/, \
		decContext.c decDouble.c decimal128.c decimal64.c decNumber.c \
		decQuad.c)
OBJ_DECIMAL              = $(SRC_DECIMAL:.c=.o)

SRC_WP43S                = \
	$(addprefix src/wp43s/, \
		assign.c bufferize.c charString.c config.c constantPointers.c \
		constants.c conversionAngles.c conversionUnits.c \
		curveFitting.c dateTime.c debug.c display.c error.c flags.c \
		fonts.c fractions.c gui.c integers.c items.c keyboard.c \
		rasterFontsData.c registerValueConversions.c registers.c \
		saveRestoreCalcState.c screen.c softmenus.c sort.c stack.c \
		stats.c statusBar.c testFunctions.c timer.c tomsfastmath.c \
		wp43s.c) \
	$(addprefix src/wp43s/mathematics/, \
		10pow.c 2pow.c addition.c arccos.c arcsin.c arctan.c \
		changeSign.c comparisonReals.c conjugate.c cos.c cube.c \
		division.c exp.c factorial.c fractionalPart.c gamma.c gcd.c \
		imaginaryPart.c integerPart.c invert.c lcm.c ln.c log10.c \
		log2.c magnitude.c minusOnePow.c multiplication.c power.c \
		realPart.c sign.c sin.c sqrt.c square.c subtraction.c \
		swapRealImaginary.c tan.c toPolar.c toRect.c unitVector.c \
		wp34s.c) \
	$(addprefix src/wp43s/logicalOps/, \
		not.c) \
	$(addprefix src/wp43s/browsers/, \
		flagBrowser.c fontBrowser.c registerBrowser.c)
OBJ_WP43S                = $(SRC_WP43S:.c=.o) $(OBJ_DECIMAL)

SRC_GENERATECONSTANTS    = \
	$(addprefix src/generateConstants/, \
		generateConstants.c)
OBJ_GENERATECONSTANTS    = $(SRC_GENERATECONSTANTS:.c=.o) $(OBJ_DECIMAL)

SRC_TTF2RASTERFONTS      = \
	$(addprefix src/ttf2RasterFonts/, \
		ttf2RasterFonts.c)
OBJ_TTF2RASTERFONTS      = $(SRC_TTF2RASTERFONTS:.c=.o)

SRC_TESTTTF2RASTERFONTS  = \
	$(addprefix src/ttf2RasterFonts/, \
		testTtf2RasterFonts.c)
OBJ_TESTTTF2RASTERFONTS  = $(SRC_TESTTTF2RASTERFONTS:.c=.o) src/wp43s/rasterFontsData.o

.PHONY: clean_wp43s clean_generateConstants clean_ttf2RasterFonts clean_testTtf2RasterFonts all clean_all mrproper decNumberICU generateConstants ttf2RasterFonts testTtf2RasterFonts wp43s

all: mrproper wp43s


clean_all: clean_decNumberICU clean_wp43s clean_generateConstants clean_ttf2RasterFonts clean_testTtf2RasterFonts

mrproper: clean_all
	rm -f $(GENERATECONSTANTS_APP)
	rm -f $(TTF2RASTERFONTS_APP)
	rm -f $(TESTTTF2RASTERFONTS_APP)
	rm -f $(WP43S_APP)


clean_decNumberICU:
	rm -f decNumberICU/*.o

decNumberICU: $(OBJ_DECIMAL)
	@echo -e "\n====> decNumberICU $@ <===="

decNumberICU/%.o: decNumberICU/%.c
	@echo -e "\n====> decNumberICU $@ <===="
	$(CC) $(CFLAGS) -c -o $@ $<



clean_generateConstants: 
	rm -f src/generateConstants/*.o

generateConstants: decNumberICU $(OBJ_GENERATECONSTANTS)
	@echo -e "\n====> generateConstants $@ <===="
	$(CC) $(CFLAGS) -m64 $(OBJ_GENERATECONSTANTS) -o $(GENERATECONSTANTS_APP)
	rm -f src/wp43s/constantPointers.h
	rm -f src/wp43s/constantPointers.c
	test -f $(GENERATECONSTANTS_APP) && ./$(GENERATECONSTANTS_APP)

src/generateConstants/%.o: src/generateConstants/%.c
	@echo -e "\n====> generateConstants $@ <===="
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<



clean_ttf2RasterFonts:
	rm -f src/ttf2RasterFonts/ttf2RasterFonts.o

ttf2RasterFonts: $(OBJ_TTF2RASTERFONTS) fonts/WP43S_NumericFont.ttf fonts/WP43S_StandardFont.ttf
	@echo -e "\n====> ttf2RasterFonts $@ <===="
	$(CC) $(CFLAGS) -m64 $(OBJ_TTF2RASTERFONTS) -o $(TTF2RASTERFONTS_APP) `pkg-config --libs freetype2`
	rm -f src/wp43s/rasterFontsData.c
	test -f $(TTF2RASTERFONTS_APP) && ./$(TTF2RASTERFONTS_APP) > /dev/null

src/ttf2RasterFonts/%.o: src/ttf2RasterFonts/%.c
	@echo -e "\n====> ttf2RasterFonts $@ <===="
	$(CC) $(CFLAGS) $(INC) `pkg-config --cflags freetype2` `pkg-config --cflags gtk+-3.0` -c -o $@ $<



clean_testTtf2RasterFonts:
	rm -f src/ttf2RasterFonts/testTtf2RasterFonts.o
	rm -f src/wp43s/rasterFontsData.o

testTtf2RasterFonts: $(OBJ_TESTTTF2RASTERFONTS) fonts/WP43S_NumericFont.ttf fonts/WP43S_StandardFont.ttf
	@echo -e "\n====> testTtf2RasterFonts $@ <===="
	$(CC) $(CFLAGS) -m64 `pkg-config --libs freetype2` $(OBJ_TESTTTF2RASTERFONTS) -o $(TESTTTF2RASTERFONTS_APP)



clean_wp43s: 
	rm -f src/wp43s/*.o
	rm -f src/wp43s/mathematics/*.o
	rm -f src/wp43s/logicalops/*.o
	rm -f src/wp43s/browsers/*.o

wp43s: generateConstants ttf2RasterFonts $(OBJ_WP43S)
	@echo -e "\n====> wp43s $@ <===="
	$(CC) $(CFLAGS) -m64 $(OBJ_WP43S) -o $(WP43S_APP) `pkg-config --libs gtk+-3.0`

src/wp43s/%.o: src/wp43s/%.c
	@echo -e "\n====> wp43s $@ <===="
	$(CC) $(CFLAGS) $(INC) `pkg-config --cflags gtk+-3.0` -c -o $@ $<

