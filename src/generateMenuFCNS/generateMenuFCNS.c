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
 * \file generateMenuFCNS.c
 ***********************************************/

#include "wp43s.h"

#if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
  bool_t             debugMemAllocation;
#endif
#ifdef PC_BUILD
  bool_t             calcLandscape;
  bool_t             calcAutoLandscapePortrait;
  GtkWidget          *screen;
  GtkWidget          *frmCalc;
  int16_t            screenStride;
  int16_t            debugWindow;
  uint32_t           *screenData;
  bool_t             screenChange;
  #if (DEBUG_REGISTER_L == 1)
    GtkWidget        *lblRegisterL1;
    GtkWidget        *lblRegisterL2;
  #endif
  #if (SHOW_MEMORY_STATUS == 1)
    GtkWidget        *lblMemoryStatus;
  #endif
#endif

char                 *ram;
bool_t               allowScreenUpdate;
bool_t               funcOK;

// Variables stored in RAM
decContext           ctxtReal16;  // 16 digits
decContext           ctxtReal34;  // 34 digits
decContext           ctxtRealIc;  // 39 digits: used for 34 digits intermediate calculations
decContext           ctxtReal51;  // 51 digits: used in trigonometric function from WP34S
decContext           ctxtReal451; // 451 digits: used in radian angle reduction
decContext           ctxtReal850; // 850 digits: used for really big modulo
uint16_t             flags[7];
char                 tmpStr3000[TMP_STR_LENGTH];
char                 errorMessage[ERROR_MESSAGE_LENGTH];
char                 aimBuffer[AIM_BUFFER_LENGTH];
char                 nimBuffer[NIM_BUFFER_LENGTH];
char                 nimBufferDisplay[NIM_BUFFER_LENGTH];
char                 tamBuffer[TAM_BUFFER_LENGTH];
char                 oldTime[8];
char                 dateTimeString[12];
softmenuStack_t      softmenuStack[7];
registerDescriptor_t reg[112];
registerDescriptor_t savedStackRegister[9+1];
int16_t              tamFunction;
int16_t              tamNumber;
int16_t              tamNumberMin;
int16_t              tamNumberMax;
int16_t              tamDigit;
int16_t              tamOperation;
int16_t              tamLetteredRegister;
int16_t              tamCurrentOperation;
int16_t              currentRegisterBrowserScreen;
int16_t              lineTWidth;
int16_t              rbrRegister;
int16_t              displayHasNDigits;
uint16_t             numberOfLocalRegisters;
uint16_t             numberOfLocalFlags;
uint16_t             numberOfNamedVariables;
char                 *allLocalRegisterPointer;
char                 *allNamedVariablePointer;
char                 *statisticalSumsPointer;
uint16_t             programCounter;
uint16_t             xCursor;
uint16_t             yCursor;
uint16_t             tamMode;
uint32_t             firstGregorianDay;
uint32_t             denMax;
uint32_t             lastIntegerBase;
uint8_t              softmenuStackPointer;
uint8_t              transitionSystemState;
uint8_t              cursorBlinkCounter;
uint8_t              numScreensStandardFont;
uint8_t              currentFntScr;
uint8_t              currentFlgScr;
uint8_t              displayFormat;
uint8_t              displayFormatDigits;
uint8_t              shortIntegerWordSize;
uint8_t              denominatorMode;
uint8_t              significantDigits;
uint8_t              shortIntegerMode;
uint8_t              previousCalcMode;
uint8_t              groupingGap;
uint8_t              dateFormat;
uint8_t              curveFitting;
uint8_t              roundingMode;
uint8_t              calcMode;
uint8_t              nextChar;
uint8_t              complexUnit;
uint8_t              displayStack;
uint8_t              productSign;
uint8_t              fractionType;
uint8_t              radixMark;
uint8_t              displayModeOverride;
uint8_t              stackSize;
uint8_t              complexMode;
uint8_t              alphaCase;
uint8_t              numLinesNumericFont;
uint8_t              numLinesStandardFont;
uint8_t              cursorEnabled;
uint8_t              cursorFont;
uint8_t              nimNumberPart;
uint8_t              hexDigits;
uint8_t              lastErrorCode;
uint8_t              serialIOIconEnabled;
uint8_t              timeFormat;
uint8_t              temporaryInformation;
uint8_t              rbrMode;
uint8_t              numScreensNumericFont;
uint8_t              currentAngularMode;
bool_t               hourGlassIconEnabled;
bool_t               watchIconEnabled;
bool_t               userModeEnabled;
bool_t               printerIconEnabled;
bool_t               batteryIconEnabled;
bool_t               shiftF;
bool_t               shiftG;
bool_t               showContent;
bool_t               stackLiftEnabled;
bool_t               displayLeadingZeros;
bool_t               displayRealAsFraction;
bool_t               savedStackLiftEnabled;
bool_t               rbr1stDigit;
bool_t               nimInputIsReal34;
calcKey_t            kbd_usr[37];
calcRegister_t       errorMessageRegisterLine;
calcRegister_t       errorRegisterLine;
uint16_t             row[100];
uint64_t             shortIntegerMask;
uint64_t             shortIntegerSignBit;
glyph_t              glyphNotFound = {.charCode = 0x0000, .colsBeforeGlyph = 0, .colsGlyph = 13, .colsAfterGlyph = 0, .rowsGlyph = 19};
char                 transitionSystemOperation[4];
int16_t              exponentSignLocation;
int16_t              denominatorLocation;
int16_t              imaginaryExponentSignLocation;
int16_t              imaginaryMantissaSignLocation;
size_t               gmpMemInBytes;
size_t               wp43sMemInBytes;
freeBlock_t          freeBlocks[MAX_FREE_BLOCKS];
int32_t              numberOfFreeBlocks;
void                 (*confirmedFunction)(uint16_t);
realIc_t             const *gammaConstants;
realIc_t             const *angle180;
realIc_t             const *angle90;
realIc_t             const *angle45;
#ifdef DMCP_BUILD
  bool_t               endOfProgram;
  uint32_t             nextScreenRefresh; // timer substitute for refreshScreen(), which does cursor blinking and other stuff
  #define TIMER_IDX_SCREEN_REFRESH 0      // use timer 0 to wake up for screen refresh
#endif // DMCP_BUILD

#ifdef __APPLE__
  // needed for chdir:
  #include<unistd.h>
#endif // __APPLE__

#define MAX_NUMBER_OF_ITEMS 1000
FILE *menuFCNS;
int32_t numberOfItems, item, functionList[MAX_NUMBER_OF_ITEMS];



static int sortItems(void const *a, void const *b)
 {
  int32_t const *pa = a;
  int32_t const *pb = b;

  return compareString(indexOfItems[*pa].itemName, indexOfItems[*pb].itemName);
 }



#ifdef __APPLE__
int main(int argc, char* argv[]) {
    // we take the directory where the application is as the root for this application.
    // in argv[0] is the application itself. We strip the name of the app by searching for the last '/':
    if (argc>=1) {
      char *curdir = malloc(1000);
      // find last /:
      char *s = strrchr(argv[0], '/');
      if (s != 0) {
        // take the directory before the appname:
        strncpy(curdir, argv[0], s-argv[0]);
        chdir(curdir);
        free(curdir);
      }
    }
#else
  int main(void) {
#endif

  if(stringGlyphLength(sortingOrder) != SORT_STRING_LEN_GLYPHS) {
    printf("In file sort.h change SORT_STRING_LEN_GLYPHS definition:\n");
    printf("#define SORT_STRING_LEN_GLYPHS %" FMT32S "\n", stringGlyphLength(sortingOrder));
    exit(-1);
  }

  menuFCNS = fopen("src/wp43s/softmenuFCNS.h", "wb");
  if(menuFCNS == NULL) {
    fprintf(stderr, "Cannot create file src/wp43s/softmenuFCNS.c\n");
    exit(1);
  }

  fprintf(menuFCNS, "/* This file is part of 43S.\n");
  fprintf(menuFCNS, " *\n");
  fprintf(menuFCNS, " * 43S is free software: you can redistribute it and/or modify\n");
  fprintf(menuFCNS, " * it under the terms of the GNU General Public License as published by\n");
  fprintf(menuFCNS, " * the Free Software Foundation, either version 3 of the License, or\n");
  fprintf(menuFCNS, " * (at your option) any later version.\n");
  fprintf(menuFCNS, " *\n");
  fprintf(menuFCNS, " * 43S is distributed in the hope that it will be useful,\n");
  fprintf(menuFCNS, " * but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
  fprintf(menuFCNS, " * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n");
  fprintf(menuFCNS, " * GNU General Public License for more details.\n");
  fprintf(menuFCNS, " *\n");
  fprintf(menuFCNS, " * You should have received a copy of the GNU General Public License\n");
  fprintf(menuFCNS, " * along with 43S.  If not, see <http://www.gnu.org/licenses/>.\n");
  fprintf(menuFCNS, " */\n\n");

  fprintf(menuFCNS, "/********************************************//**\n");
  fprintf(menuFCNS, "* \\file softmenuFCNS.c\n");
  fprintf(menuFCNS, "***********************************************/\n\n");

  fprintf(menuFCNS, "/***********************************************************************************************\n");
  fprintf(menuFCNS, "* Do not edit this file manually! It's automagically generated by the program generateMenuFCNS *\n");
  fprintf(menuFCNS, "************************************************************************************************/\n\n");

  fprintf(menuFCNS, "/*<--------- 6 functions --------->*/\n");
  fprintf(menuFCNS, "/*<---- 6 f shifted functions ---->*/\n");
  fprintf(menuFCNS, "/*<---- 6 g shifted functions ---->*/\n");

  fprintf(menuFCNS, "const int16_t menu_FCNS[] = {\n");

  numberOfItems = 0;
  for(item=1; item<LAST_ITEM; item++) {
    if(indexOfItems[item].catalog == CAT_FNCT) {
      functionList[numberOfItems++] = item;
      if(numberOfItems == MAX_NUMBER_OF_ITEMS) {
        printf("Array functionList is too small: increase the value of MAX_NUMBER_OF_ITEMS\n");
        exit(-1);
      }
    }
  }

  qsort(functionList, numberOfItems, sizeof(*functionList), sortItems);

  for(item=0; item<numberOfItems; item++) {
    fprintf(menuFCNS, "%5d,", functionList[item]);
    if((item + 1) % 6 == 0) {
      fprintf(menuFCNS, "\n");
    }
  }

  //for(item=1; item<=5; item++) {
  //  fprintf(menuFCNS, "%5d,", 0);
  //  numberOfItems++;
  //}

  while(numberOfItems++ % 6 != 0) {
    fprintf(menuFCNS, "%5d,", 0);
  }

  fprintf(menuFCNS, "\n};\n");

  fclose(menuFCNS);

  return 0;
}
