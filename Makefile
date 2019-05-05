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

SRC_DECIMAL              = decNumberICU/decContext.c decNumberICU/decDouble.c decNumberICU/decimal128.c decNumberICU/decimal64.c decNumberICU/decNumber.c decNumberICU/decQuad.c
OBJ_DECIMAL              = $(SRC_DECIMAL:.c=.o)

SRC_WP43S                = \
	$(wildcard src/wp43s/*.c) \
	$(wildcard src/wp43s/mathematics/*.c) \
	$(wildcard src/wp43s/logicalOps/*.c) \
	$(wildcard src/wp43s/browsers/*.c)	
OBJ_WP43S                = $(SRC_WP43S:.c=.o) $(OBJ_DECIMAL)

SRC_GENERATECONSTANTS    = $(wildcard src/generateConstants/*.c)
OBJ_GENERATECONSTANTS    = $(SRC_GENERATECONSTANTS:.c=.o) $(OBJ_DECIMAL)

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

ttf2RasterFonts: src/ttf2RasterFonts/ttf2RasterFonts.o fonts/WP43S_NumericFont.ttf fonts/WP43S_StandardFont.ttf
	@echo -e "\n====> ttf2RasterFonts $@ <===="
	$(CC) $(CFLAGS) -m64 src/ttf2RasterFonts/ttf2RasterFonts.o -o $(TTF2RASTERFONTS_APP) `pkg-config --libs freetype2` 
	rm -f src/wp43s/rasterFontsData.c
	test -f $(TTF2RASTERFONTS_APP) && ./$(TTF2RASTERFONTS_APP) > /dev/null

src/ttf2RasterFonts/%.o: src/ttf2RasterFonts/%.c
	@echo -e "\n====> ttf2RasterFonts $@ <===="
	$(CC) $(CFLAGS) $(INC) `pkg-config --cflags freetype2` `pkg-config --cflags gtk+-3.0` -c -o $@ $<



clean_testTtf2RasterFonts:
	rm -f src/ttf2RasterFonts/testTtf2RasterFonts.o
	rm -f src/wp43s/rasterFontsData.o

testTtf2RasterFonts: src/ttf2RasterFonts/testTtf2RasterFonts.o src/wp43s/rasterFontsData.o fonts/WP43S_NumericFont.ttf fonts/WP43S_StandardFont.ttf
	@echo -e "\n====> testTtf2RasterFonts $@ <===="
	$(CC) $(CFLAGS) -m64 `pkg-config --libs freetype2` src/ttf2RasterFonts/testTtf2RasterFonts.o src/wp43s/rasterFontsData.o -o $(TESTTTF2RASTERFONTS_APP)



clean_wp43s: 
	rm -f src/wp43s/*.o
	rm -f src/wp43s/mathematics/*.o
	rm -f src/wp43s/logicalops/*.o
	rm -f src/wp43s/browsers/*.o

wp43s: generateConstants ttf2RasterFonts $(OBJ_WP43S)
	@echo -e "\n====> wp43s $@ <===="
	$(CC) $(CFLAGS) -m64 $(OBJ_WP43S) -o $(WP43S_APP) `PKG-CONFIG --libs GTK+-3.0`

src/wp43s/%.o: src/wp43s/%.c
	@echo -e "\n====> wp43s $@ <===="
	$(CC) $(CFLAGS) $(INC) `pkg-config --cflags gtk+-3.0` -c -o $@ $<

