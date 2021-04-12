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

/********************************************//**
 * \file generateCatalogs.c
 ***********************************************/

#include "wp43s.h"
#include <unistd.h>

bool_t  funcOK;
glyph_t glyphNotFound = {.charCode = 0x0000, .colsBeforeGlyph = 0, .colsGlyph = 13, .colsAfterGlyph = 0, .rowsGlyph = 19, .data = NULL};

#define MAX_NUMBER_OF_ITEMS 1000
#define GENERATION_FOR_DMCP    1
#define GENERATION_FOR_PC      2
#define GENERATION_FOR_BOTH    3
FILE *catalogFile;
int32_t numberOfItems, item, itemList[MAX_NUMBER_OF_ITEMS];



static int sortItems(void const *a, void const *b)
 {
  int32_t const *pa = a;
  int32_t const *pb = b;

  return compareString(indexOfItems[*pa].itemCatalogName, indexOfItems[*pb].itemCatalogName, CMP_EXTENSIVE);
 }



void sortOneCatalog(const char *menuName, int catalogType, int16_t generationType) {
  printf("Generating catalog %s\n", menuName);
  fprintf(catalogFile, "TO_QSPI const int16_t menu_%s[] = {\n", menuName);

  numberOfItems = 0;
  for(item=1; item<LAST_ITEM; item++) {
    if((indexOfItems[item].status & CAT_STATUS) == catalogType && strcmp(indexOfItems[item].itemCatalogName, "CATALOG") && strcmp(indexOfItems[item].itemCatalogName, "MENUS")) { // CATALOG and MENUS are not in another catalog
      if(   generationType == GENERATION_FOR_DMCP
         || generationType == GENERATION_FOR_BOTH
         || (generationType == GENERATION_FOR_PC && strcmp(indexOfItems[item].itemCatalogName, "SYSTEM")
                                                 && strcmp(indexOfItems[item].itemCatalogName, "SETTIM")
                                                 && strcmp(indexOfItems[item].itemCatalogName, "SETDAT"))) { // (not SYSTEM) and (not SETTIM) and (not SETDAT) when generating program for PC
        itemList[numberOfItems++] = item;
        if(numberOfItems == MAX_NUMBER_OF_ITEMS) {
          printf("Array itemList is too small: increase the value of MAX_NUMBER_OF_ITEMS\n");
          exit(-1);
        }
      }
    }
  }

  qsort(itemList, numberOfItems, sizeof(*itemList), sortItems);

  if(generationType == GENERATION_FOR_PC || generationType == GENERATION_FOR_DMCP) {
    fprintf(catalogFile, "  ");
  }
  for(item=0; item<numberOfItems; item++) {
    fprintf(catalogFile, "%5d,", itemList[item] * (catalogType == CAT_MENU ? -1 : 1)); // Menus are negative
    if((item + 1) % 6 == 0) {
      fprintf(catalogFile, "\n");
      if(generationType == GENERATION_FOR_PC || generationType == GENERATION_FOR_DMCP) {
        fprintf(catalogFile, "  ");
      }
    }
  }

  //while(numberOfItems++ % 6 != 0) {
  //  fprintf(catalogFile, "%5d,", 0);
  //}

  if(generationType == GENERATION_FOR_PC || generationType == GENERATION_FOR_DMCP) {
    fprintf(catalogFile, "\n  };\n");
  }
  else {
    fprintf(catalogFile, "\n};\n");
  }
}



int main(int argc, char* argv[]) {
  #ifdef CODEBLOCKS_OVER_SCORE // Since December 27th 2020 when running in code::blocks, we are no longer in the correct directory! Why?
    (*strstr(argv[0], "/bin/")) = 0;
    chdir(argv[0]);
  #endif // CODEBLOCKS_OVER_SCORE

  #ifdef __APPLE__
    // we take the directory where the application is as the root for this application.
    // in argv[0] is the application itself. We strip the name of the app by searching for the last '/':
    if(argc>=1) {
      char *curdir = malloc(1000);
      // find last /:
      char *s = strrchr(argv[0], '/');
      if(s != 0) {
        // take the directory before the appname:
        strncpy(curdir, argv[0], s-argv[0]);
        chdir(curdir);
        free(curdir);
      }
    }
  #endif // __APPLE__

  catalogFile = fopen("src/wp43s/softmenuCatalogs.h", "wb");
  if(catalogFile == NULL) {
    fprintf(stderr, "Cannot create file src/wp43s/softmenuCatalogs.h\n");
    exit(1);
  }

  fprintf(catalogFile, "/* This file is part of 43S.\n");
  fprintf(catalogFile, " *\n");
  fprintf(catalogFile, " * 43S is free software: you can redistribute it and/or modify\n");
  fprintf(catalogFile, " * it under the terms of the GNU General Public License as published by\n");
  fprintf(catalogFile, " * the Free Software Foundation, either version 3 of the License, or\n");
  fprintf(catalogFile, " * (at your option) any later version.\n");
  fprintf(catalogFile, " *\n");
  fprintf(catalogFile, " * 43S is distributed in the hope that it will be useful,\n");
  fprintf(catalogFile, " * but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
  fprintf(catalogFile, " * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n");
  fprintf(catalogFile, " * GNU General Public License for more details.\n");
  fprintf(catalogFile, " *\n");
  fprintf(catalogFile, " * You should have received a copy of the GNU General Public License\n");
  fprintf(catalogFile, " * along with 43S.  If not, see <http://www.gnu.org/licenses/>.\n");
  fprintf(catalogFile, " */\n\n");

  fprintf(catalogFile, "/********************************************//**\n");
  fprintf(catalogFile, "* \\file softmenuCatalogs.c\n");
  fprintf(catalogFile, "***********************************************/\n\n");

  fprintf(catalogFile, "/***********************************************************************************************\n");
  fprintf(catalogFile, "* Do not edit this file manually! It's automagically generated by the program generateCatalogs *\n");
  fprintf(catalogFile, "************************************************************************************************/\n\n");

  fprintf(catalogFile, "/*<--------- 6 functions --------->*/\n");
  fprintf(catalogFile, "/*<---- 6 f shifted functions ---->*/\n");
  fprintf(catalogFile, "/*<---- 6 g shifted functions ---->*/\n");

  fprintf(catalogFile, "#if DMCP_BUILD\n");
  sortOneCatalog("FCNS",       CAT_FNCT, GENERATION_FOR_DMCP);
  fprintf(catalogFile, "#else // !DMCP_BUILD\n");
  sortOneCatalog("FCNS",       CAT_FNCT, GENERATION_FOR_PC);
  fprintf(catalogFile, "#endif // DMCP_BUILD\n");

  sortOneCatalog("CONST",      CAT_CNST, GENERATION_FOR_BOTH);
  sortOneCatalog("MENUS",      CAT_MENU, GENERATION_FOR_BOTH);
  sortOneCatalog("SYSFL",      CAT_SYFL, GENERATION_FOR_BOTH);
  sortOneCatalog("alpha_INTL", CAT_AINT, GENERATION_FOR_BOTH);
  sortOneCatalog("alpha_intl", CAT_aint, GENERATION_FOR_BOTH);

  fclose(catalogFile);

  return 0;
}
