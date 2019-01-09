# $@ target
# $< 1st dependency
# $^ dependency list
# $? recent dependency list
# $* target without extension
 
CC = gcc
INC = -IdecNumberICU -Isrc/wp43s

ifeq ($(DEST), gitlab)
  CFLAGS = -march=x86-64
else
  CFLAGS = -march=native
endif

ifdef DEBUG_WP43S
  CFLAGS += -g
else
  CFLAGS += -O2 -s
endif

CFLAGS += -Wextra -Wall -std=c11 -m64 -fshort-enums -fomit-frame-pointer -DPC_BUILD -DTFM_NO_ASM -DTFM_X86_64

LDFLAGS = -m64

SRC_DECIMAL              = decNumberICU/decContext.c decNumberICU/decDouble.c decNumberICU/decimal128.c decNumberICU/decimal64.c decNumberICU/decNumber.c decNumberICU/decQuad.c
OBJ_DECIMAL              = $(SRC_DECIMAL:.c=.o)

SRC_WP43S                = $(wildcard src/wp43s/*.c) $(wildcard src/wp43s/mathematics/*.c)
OBJ_WP43S                = $(SRC_WP43S:.c=.o) $(OBJ_DECIMAL)

SRC_GENERATECONSTANTS    = $(wildcard src/generateConstants/*.c)
OBJ_GENERATECONSTANTS    = $(SRC_GENERATECONSTANTS:.c=.o) $(OBJ_DECIMAL)



.PHONY: clean_wp43s clean_generateConstants clean_ttf2RasterFonts clean_testTtf2RasterFonts all clean_all mrproper decNumberICU generateConstants ttf2RasterFonts testTtf2RasterFonts wp43s

all: mrproper wp43s

clean_all: clean_decNumberICU clean_wp43s clean_generateConstants clean_ttf2RasterFonts clean_testTtf2RasterFonts

mrproper: clean_all
	rm -f generateConstants.exe
	rm -f ttf2RasterFonts.exe
	rm -f testTtf2RasterFonts.exe
	rm -f wp43s.exe


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
	$(CC) $(CFLAGS) -m64 $(OBJ_GENERATECONSTANTS) -o generateConstants.exe
	rm -f src/wp43s/constantPointers.h
	rm -f src/wp43s/constantPointers.c
	test -f generateConstants.exe && ./generateConstants.exe

src/generateConstants/%.o: src/generateConstants/%.c
	@echo -e "\n====> generateConstants $@ <===="
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<



clean_ttf2RasterFonts:
	rm -f src/ttf2RasterFonts/ttf2RasterFonts.o

ttf2RasterFonts: src/ttf2RasterFonts/ttf2RasterFonts.o fonts/WP43S_NumericFont.ttf fonts/WP43S_StandardFont.ttf
	@echo -e "\n====> ttf2RasterFonts $@ <===="
	$(CC) $(CFLAGS) -m64 src/ttf2RasterFonts/ttf2RasterFonts.o -o ttf2RasterFonts.exe `pkg-config --libs freetype2` 
	rm -f src/wp43s/rasterFontsData.c
	test -f ttf2RasterFonts.exe && ./ttf2RasterFonts.exe > /dev/null

src/ttf2RasterFonts/%.o: src/ttf2RasterFonts/%.c
	@echo -e "\n====> ttf2RasterFonts $@ <===="
	$(CC) $(CFLAGS) $(INC) `pkg-config --cflags freetype2` `pkg-config --cflags gtk+-3.0` -c -o $@ $<



clean_testTtf2RasterFonts:
	rm -f src/ttf2RasterFonts/testTtf2RasterFonts.o
	rm -f src/wp43s/rasterFontsData.o

testTtf2RasterFonts: src/ttf2RasterFonts/testTtf2RasterFonts.o src/wp43s/rasterFontsData.o fonts/WP43S_NumericFont.ttf fonts/WP43S_StandardFont.ttf
	@echo -e "\n====> testTtf2RasterFonts $@ <===="
	$(CC) $(CFLAGS) -m64 `pkg-config --libs freetype2` src/ttf2RasterFonts/testTtf2RasterFonts.o src/wp43s/rasterFontsData.o -o testTtf2RasterFonts.exe



clean_wp43s: 
	rm -f src/wp43s/*.o

wp43s: generateConstants ttf2RasterFonts $(OBJ_WP43S)
	@echo -e "\n====> wp43s $@ <===="
	$(CC) $(CFLAGS) -m64 $(OBJ_WP43S) -o wp43s.exe `pkg-config --libs gtk+-3.0`

src/wp43s/%.o: src/wp43s/%.c
	@echo -e "\n====> wp43s $@ <===="
	$(CC) $(CFLAGS) $(INC) `pkg-config --cflags gtk+-3.0` -c -o $@ $<
