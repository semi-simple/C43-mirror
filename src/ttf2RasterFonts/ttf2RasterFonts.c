/* This file is part of 43S.
 *
 * 43S is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * 43S is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 43S.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ft2build.h>
#include FT_FREETYPE_H

FT_Library library;
FT_Error   error;
FILE       *cFile, *hFile;
int        byte, numBits;

const char* getErrorMessage(FT_Error err) {
  #undef __FTERRORS_H__
  #define FT_ERRORDEF( e, v, s )  case e: return s;
  #define FT_ERROR_START_LIST     switch (err) {
  #define FT_ERROR_END_LIST       }
  #include FT_ERRORS_H
  return "(Unknown error)";
}

int sortCharCodes(const void *a, const void *b) {
  return  *(int *)a - *(int *)b;
}

void addBit(int bit) {
  byte <<= 1;
  byte |= bit;
  numBits++;
  if(numBits == 8) {
    fprintf(cFile, "\\x%02x", byte);
    numBits = 0;
    byte = 0;
  }
}

void exportCStructure(char const *ttfName) {
  FT_Face      face;
  FT_UInt      glyphIndex;
  FT_ULong     charCode;
  FT_ULong     charCodes[1000];
  FT_Vector    pen;
  char         glyphName[100];
  char         ttfName2[100], path[200], *fontName;
  int          x, y, cc, bytesPerRow, bit; // ,dataLength
  int          fontHeightPixels, unitsPerEm, onePixelSize, numberOfGlyphs;
  int          colsBeforeGlyph, colsGlyph, colsAfterGlyph;
  int          rowsAboveGlyph, rowsGlyph, rowsBelowGlyph;

  /////////////////////
  // Open the face 0 //
  /////////////////////
  sprintf(path, "fonts/%s", ttfName);
  if((error = FT_New_Face(library, path, 0, &face)) != FT_Err_Ok) {
    fprintf(stderr, "Error during face creation from file %s\n", path);
    fprintf(stderr, "Error %d : %s\n", error, getErrorMessage(error));
    exit(1);
  }

  ///////////////////
  // Set font size //
  ///////////////////
  unitsPerEm       = face->units_per_EM;
  onePixelSize     = unitsPerEm==1024 ? 32 : 50;      // ???
  fontHeightPixels = unitsPerEm / onePixelSize; // What is the right formula?
  if((error = FT_Set_Pixel_Sizes(face, 0, fontHeightPixels)) != FT_Err_Ok) {
    fprintf(stderr, "Error during FT_Set_Pixel_Sizes from file %s\n", ttfName);
    fprintf(stderr, "Error %d : %s\n", error, getErrorMessage(error));
    exit(1);
  }

  ///////////////////////
  // Get the font name //
  ///////////////////////
  strcpy(ttfName2, ttfName);
  x = strlen(ttfName2) - 4;
  ttfName2[x] = 0; // truncates .ttf
  while(--x>=0 && ttfName2[x] != '/');
  if(x < 0) x = 0;
  if(ttfName2[x] == '/') {
    fontName = ttfName2 + x + 7;
  }
  else {
    fontName = ttfName2 + x + 6;
  }

  fontName[0] |= 0x20; // 1st letter lower case
  printf("fontName=<%s>\n", fontName);


  //////////////////////////////////////////////////////////////
  // Get the number of glyphs and sort them per unicode point //
  //////////////////////////////////////////////////////////////
  numberOfGlyphs = 0;
  charCode = FT_Get_First_Char(face, &glyphIndex);

  while(glyphIndex) {
    if(charCode) {
      charCodes[numberOfGlyphs++] = charCode;
    }

    charCode = FT_Get_Next_Char(face, charCode, &glyphIndex);
  }

  qsort(charCodes, numberOfGlyphs, sizeof(charCodes[0]), sortCharCodes);

  //////////////////////
  // Face infomations //
  //////////////////////
  printf("There are %3d glyphs in face 0 of %s\n", numberOfGlyphs,  ttfName);
  printf("ascender=%d descender=%d\n", face->ascender/onePixelSize, face->descender/onePixelSize);

  ///////////////////////////////////////
  // Go thru all glyphs to render them //
  ///////////////////////////////////////
  fprintf(cFile, "const font_t %s = {\n", fontName);
  fprintf(cFile, "  .id             = %d,\n", fontName[0] == 'n' ? 0 : 1);
  //fprintf(cFile, "  .name           = \"%s\",\n", fontName);
  //fprintf(cFile, "  .ascender       = %d,\n", face->ascender/onePixelSize);
  //fprintf(cFile, "  .descender      = %d,\n", face->descender/onePixelSize);
  fprintf(cFile, "  .numberOfGlyphs = %d,\n", numberOfGlyphs);
  fprintf(cFile, "  .glyphs = {\n\n");

  pen.x = 0;
  pen.y = (fontHeightPixels - face->ascender/onePixelSize)*64;

  FT_Set_Transform(face, NULL, &pen);
  for(cc=0; cc<numberOfGlyphs; cc++) {
    glyphIndex = FT_Get_Char_Index(face, charCodes[cc]);
    if(glyphIndex == 0) {
      fprintf(stderr, "character code 0x%04x not undefined in %s\n", (unsigned int)charCodes[cc], ttfName);
    }
    else {
      if(cc != 0) {
        fprintf(cFile, ",\n\n");
      }

      FT_Get_Glyph_Name(face, glyphIndex, glyphName, 100);
      printf("glyphIndex=%4d   charCode=0x%04x   %s\n", glyphIndex, (unsigned int)(charCodes[cc]>=0x0080 ? charCodes[cc]|0x8000 : charCodes[cc]), glyphName);

      if((error = FT_Load_Glyph(face, glyphIndex, FT_LOAD_RENDER)) != FT_Err_Ok) {
        fprintf(stderr, "warning: failed FT_Load_Glyph 0x%04x\n", (unsigned int)charCodes[cc]);
        fprintf(stderr, "Error %d : %s\n", error, getErrorMessage(error));
      }

      //////////////////////////
      // Columns in the glyph //
      //////////////////////////
      colsBeforeGlyph = face->glyph->metrics.horiBearingX/64;
      colsGlyph       = face->glyph->bitmap.width;
      colsAfterGlyph  = face->glyph->metrics.horiAdvance/64 - colsBeforeGlyph - colsGlyph;
      if(colsBeforeGlyph < 0) {
        colsGlyph += colsBeforeGlyph;
        colsBeforeGlyph = 0;
      }
      printf("Columns: %2d %2d %2d\n", colsBeforeGlyph, colsGlyph, colsAfterGlyph);

      ///////////////////////
      // Rows in the glyph //
      ///////////////////////
      rowsAboveGlyph = face->ascender/onePixelSize - face->glyph->metrics.horiBearingY/64;
      rowsGlyph      = face->glyph->bitmap.rows;
      rowsBelowGlyph = face->glyph->metrics.horiBearingY/64 - face->descender/onePixelSize - rowsGlyph;
      if(rowsAboveGlyph < 0) {
        rowsGlyph += rowsAboveGlyph;
        rowsAboveGlyph = 0;
      }
      printf("Rows   : %2d %2d %2d\n", rowsAboveGlyph, rowsGlyph, rowsBelowGlyph);

      //////////////////////
      // Render the glyph //
      //////////////////////
      fprintf(cFile, "    // %s \n", glyphName);

      bytesPerRow = colsGlyph;

      if(bytesPerRow % 8 == 0) {
        bytesPerRow /= 8;
      }
      else {
        bytesPerRow = bytesPerRow/8 + 1;
      }

      fprintf(cFile, "    {.charCode=0x%04x, .colsBeforeGlyph=%2d, .colsGlyph=%2d, .colsAfterGlyph=%2d, .rowsAboveGlyph=%2d, .rowsGlyph=%2d, .rowsBelowGlyph=%2d,\n",
                      (unsigned int)(charCodes[cc]>=0x0080 ? charCodes[cc]|0x8000 : charCodes[cc]), colsBeforeGlyph, colsGlyph, colsAfterGlyph, rowsAboveGlyph, rowsGlyph, rowsBelowGlyph);
      fprintf(cFile, "     .data=\"");

      for(y=0; y<rowsGlyph; y++) {
        byte = 0;

        for(x=0; x<colsGlyph; x++) {
          bit = face->glyph->bitmap.buffer[y * face->glyph->bitmap.width + x] >= 128 ? 1 : 0;
          putchar(bit == 1 ? '#' : '.');
          addBit(bit);
        }

        putchar('\n');
        while(numBits != 0) {
          addBit(0);
        }
      }

      fprintf(cFile, "\"}");
    }
  }

  fprintf(cFile, "}\n};\n");

  /////////////////////////////
  // Free the face ressources //
  /////////////////////////////
  FT_Done_Face(face);
}

int main(void) {
  /////////////////////////////////////
  // Initialize the freetype library //
  /////////////////////////////////////
  if((error = FT_Init_FreeType(&library)) != FT_Err_Ok) {
    fprintf(stderr, "Error during freetype2 library initialisation.\n");
    fprintf(stderr, "Error %d : %s\n", error, getErrorMessage(error));
    exit(1);
  }

  #if (__linux__ == 1)
   cFile = fopen("src/wp43s/rasterFontsData.c", "wb");
  #elif defined(__MINGW64__)
   cFile = fopen("src\\wp43s\\rasterFontsData.c", "wb");
  #elif defined(__APPLE__) 
   cFile = fopen("src/wp43s/rasterFontsData.c", "wb");
  #else
   #error Only Linux, MacOS and Windows MINGW64 are supported for now
  #endif
  fprintf(cFile, "/* This file is part of 43S.\n");
  fprintf(cFile, " *\n");
  fprintf(cFile, " * 43S is free software: you can redistribute it and/or modify\n");
  fprintf(cFile, " * it under the terms of the GNU General Public License as published by\n");
  fprintf(cFile, " * the Free Software Foundation, either version 3 of the License, or\n");
  fprintf(cFile, " * (at your option) any later version.\n");
  fprintf(cFile, " *\n");
  fprintf(cFile, " * 43S is distributed in the hope that it will be useful,\n");
  fprintf(cFile, " * but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
  fprintf(cFile, " * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n");
  fprintf(cFile, " * GNU General Public License for more details.\n");
  fprintf(cFile, " *\n");
  fprintf(cFile, " * You should have received a copy of the GNU General Public License\n");
  fprintf(cFile, " * along with 43S.  If not, see <http://www.gnu.org/licenses/>.\n");
  fprintf(cFile, " */\n\n");

  fprintf(cFile, "/********************************************//**\n");
  fprintf(cFile, " * \\file rasterFontsData.c C structures of the raster fonts\n");
  fprintf(cFile, " ***********************************************/\n\n");

  fprintf(cFile, "/**********************************************************************************************\n");
  fprintf(cFile, "* Do not edit this file manually! It's automagically generated by the program ttf2RasterFonts *\n");
  fprintf(cFile, "***********************************************************************************************/\n\n");

  fprintf(cFile, "#include \"wp43s.h\"\n\n");

  ///////////////////////////
  // Generate the C arrays //
  ///////////////////////////
  exportCStructure("WP43S_NumericFont.ttf");
  exportCStructure("WP43S_StandardFont.ttf");

  fclose(cFile);

  ////////////////////////////////
  // Close the freetype library //
  ////////////////////////////////
  FT_Done_FreeType(library);

  return 0;
}
