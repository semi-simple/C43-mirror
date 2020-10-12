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
 * \file wp43s.c
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
  char               debugString[10000];
  #if (DEBUG_REGISTER_L == 1)
    GtkWidget        *lblRegisterL1;
    GtkWidget        *lblRegisterL2;
  #endif
  #if (SHOW_MEMORY_STATUS == 1)
    GtkWidget        *lblMemoryStatus;
  #endif
#endif

dataBlock_t          *ram;
bool_t                funcOK;
bool_t                keyActionProcessed;
const font_t         *fontForShortInteger;
const font_t         *cursorFont;

// Variables stored in RAM
realContext_t         ctxtReal34;   //   34 digits
realContext_t         ctxtReal39;   //   39 digits: used for 34 digits intermediate calculations
realContext_t         ctxtReal51;   //   51 digits: used for 34 digits intermediate calculations
realContext_t         ctxtReal75;   //   75 digits: used in SLVQ
realContext_t         ctxtReal1071; // 1071 digits: used in radian angle reduction
//realContext_t         ctxtReal2139; // 2139 digits: used for really big modulo
uint16_t              globalFlags[7];
char                  tmpStr3000[TMP_STR_LENGTH];
char                  errorMessage[ERROR_MESSAGE_LENGTH];
char                  aimBuffer[AIM_BUFFER_LENGTH]; // aimBuffer is also used for NIM
char                  nimBufferDisplay[NIM_BUFFER_LENGTH];
char                  tamBuffer[TAM_BUFFER_LENGTH];
char                  asmBuffer[5];
char                  oldTime[8];
char                  dateTimeString[12];
softmenuStack_t       softmenuStack[7];
registerDescriptor_t  reg[112];
registerDescriptor_t  savedStackRegister[9+1];
int16_t               tamFunction;
int16_t               tamNumber;
int16_t               tamNumberMin;
int16_t               tamNumberMax;
int16_t               tamDigit;
int16_t               tamOperation;
int16_t               tamLetteredRegister;
int16_t               tamCurrentOperation;
int16_t               currentRegisterBrowserScreen;
int16_t               lineTWidth;
int16_t               rbrRegister;
int16_t               alphaSelectionMenu;
int16_t               lastFcnsMenuPos;
int16_t               lastMenuMenuPos;
int16_t               lastCnstMenuPos;
int16_t               lastSyFlMenuPos;
int16_t               lastAIntMenuPos;
int16_t               showFunctionNameItem;
uint16_t              numberOfLocalFlags;
uint16_t              glyphRow[NUMBER_OF_GLYPH_ROWS];
dataBlock_t          *allLocalRegisterPointer;
dataBlock_t          *allNamedVariablePointer;
dataBlock_t          *statisticalSumsPointer;
dataBlock_t          *savedStatisticalSumsPointer;
uint16_t              programCounter;
uint16_t              xCursor;
uint16_t              yCursor;
uint16_t              tamMode;
uint32_t              firstGregorianDay;
uint32_t              denMax;
uint32_t              lastIntegerBase;
uint32_t              alphaSelectionTimer;
uint8_t               softmenuStackPointer;
uint8_t               softmenuStackPointerBeforeAIM;
uint8_t               transitionSystemState;
uint8_t               numScreensStandardFont;
uint8_t               currentFntScr;
uint8_t               currentFlgScr;
uint8_t               displayFormat;
uint8_t               displayFormatDigits;
uint8_t               shortIntegerWordSize;
uint8_t               significantDigits;
uint8_t               shortIntegerMode;
uint8_t               previousCalcMode;
uint8_t               groupingGap;
uint8_t               roundingMode;
uint8_t               calcMode;
uint8_t               nextChar;
uint8_t               displayStack;
uint8_t               alphaCase;
uint8_t               numLinesNumericFont;
uint8_t               numLinesStandardFont;
uint8_t               cursorEnabled;
uint8_t               nimNumberPart;
uint8_t               hexDigits;
uint8_t               lastErrorCode;
uint8_t               serialIOIconEnabled;
uint8_t               temporaryInformation;
uint8_t               rbrMode;
uint8_t               numScreensNumericFont;
uint8_t               currentAngularMode;
bool_t                hourGlassIconEnabled;
bool_t                watchIconEnabled;
bool_t                printerIconEnabled;
bool_t                shiftF;
bool_t                shiftG;
bool_t                showContent;
bool_t                rbr1stDigit;
bool_t                updateDisplayValueX;
bool_t                thereIsSomethingToUndo;
calcKey_t             kbd_usr[37];
calcRegister_t        errorMessageRegisterLine;
uint64_t              shortIntegerMask;
uint64_t              shortIntegerSignBit;
uint64_t              systemFlags;
uint64_t              savedSystemFlags;
glyph_t               glyphNotFound = {.charCode = 0x0000, .colsBeforeGlyph = 0, .colsGlyph = 13, .colsAfterGlyph = 0, .rowsGlyph = 19};
char                  displayValueX[DISPLAY_VALUE_LEN];
int16_t               exponentSignLocation;
int16_t               denominatorLocation;
int16_t               imaginaryExponentSignLocation;
int16_t               imaginaryMantissaSignLocation;
int16_t               exponentLimit;
int16_t               showFunctionNameCounter;
size_t                gmpMemInBytes;
size_t                wp43sMemInBytes;
freeMemoryRegion_t    freeMemoryRegions[MAX_FREE_REGION];
int32_t               numberOfFreeMemoryRegions;
int32_t               lgCatalogSelection;
void                  (*confirmedFunction)(uint16_t);
real51_t              const *gammaLanczosCoefficients;
real39_t              const *angle180;
real39_t              const *angle90;
real39_t              const *angle45;
pcg32_random_t        pcg32_global = PCG32_INITIALIZER;
const char            digits[17] = "0123456789ABCDEF";
#ifdef DMCP_BUILD
  bool_t              backToDMCP;
  uint32_t            nextScreenRefresh; // timer substitute for refreshLcd(), which does cursor blinking and other stuff
#endif // DMCP_BUILD
uint8_t *pgm, *stepAddress, param;

/********************************************//**
 * \brief Sets all the default values of the calc and refreshes the stack
 *
 * \param void
 * \return void
 ***********************************************/
void setupDefaults(void) {
  void *memPtr;

  ram = (dataBlock_t *)malloc(TO_BYTES(RAM_SIZE));
  memset(ram, 0, TO_BYTES(RAM_SIZE));
  numberOfFreeMemoryRegions = 1;
  freeMemoryRegions[0].address = 0;
  freeMemoryRegions[0].sizeInBlocks = RAM_SIZE;

  glyphNotFound.data   = malloc(38);
  xcopy(glyphNotFound.data, "\xff\xf8\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\xff\xf8", 38);

  // Initialization of user key assignments
  xcopy(kbd_usr, kbd_std, sizeof(kbd_std));
  //kbd_usr[ 0].keyLblAim   = CHR_A_GRAVE;
  //kbd_usr[ 0].fShiftedAim = CHR_A_GRAVE;
  //kbd_usr[ 4].keyLblAim   = CHR_E_ACUTE;
  //kbd_usr[ 4].fShiftedAim = CHR_E_ACUTE;
  //kbd_usr[18].fShifted    = -MNU_VARS;
  //kbd_usr[18].gShifted    = CST_54;
  //kbd_usr[19].fShifted    = ITM_SW;
  //kbd_usr[19].gShifted    = ITM_SXY;
  //kbd_usr[20].gShifted    = ITM_LYtoM;

  // initialize the global registers
  for(calcRegister_t regist=0; regist<FIRST_LOCAL_REGISTER; regist++) {
    setRegisterDataType(regist, dtReal34, AM_NONE);
    memPtr = allocWp43s(TO_BYTES(REAL34_SIZE));
    setRegisterDataPointer(regist, memPtr);
    real34Zero(memPtr);
  }

  // initialize the 9+1 saved stack registers
  for(calcRegister_t regist=SAVED_REGISTER_X; regist<=LAST_SAVED_REGISTER; regist++) {
    setRegisterDataType(regist, dtReal34, AM_NONE);
    memPtr = allocWp43s(TO_BYTES(REAL34_SIZE));
    setRegisterDataPointer(regist, memPtr);
    real34Zero(memPtr);
  }

  // allocating space for the named variable list
  allNamedVariablePointer = allocWp43s(TO_BYTES(1)); //  1 block for the number of named variables
  allNamedVariablePointer->numberOfNamedVariables = 0;

  // allocate space for the local register list
  allLocalRegisterPointer = allocWp43s(TO_BYTES(1)); //  1 block for the number of local registers and the local flags
  numberOfLocalFlags = 0;
  allLocalRegisterPointer->numberOfLocalRegisters = 0;
  allLocalRegisterPointer->localFlags = 0;

  #ifdef PC_BUILD
    debugWindow = DBG_REGISTERS;
  #endif

  temporaryInformation = TI_NO_INFO;

  decContextDefault(&ctxtReal34, DEC_INIT_DECQUAD);

  decContextDefault(&ctxtReal39, DEC_INIT_DECQUAD);
  ctxtReal39.digits = 39;
  ctxtReal39.traps  = 0;

  decContextDefault(&ctxtReal51, DEC_INIT_DECQUAD);
  ctxtReal51.digits = 51;
  ctxtReal51.traps  = 0;

  decContextDefault(&ctxtReal75, DEC_INIT_DECQUAD);
  ctxtReal75.digits = 75;
  ctxtReal75.traps  = 0;

  decContextDefault(&ctxtReal1071,  DEC_INIT_DECQUAD);
  ctxtReal1071.digits = 1071;
  ctxtReal1071.traps  = 0;

  //decContextDefault(&ctxtReal2139,  DEC_INIT_DECQUAD);
  //ctxtReal2139.digits = 2139;
  //ctxtReal2139.traps  = 0;

  statisticalSumsPointer = NULL;
  savedStatisticalSumsPointer = NULL;

  fnSetWordSize(64); // word size from 1 to 64
  fnIntegerMode(SIM_2COMPL);

  groupingGap = 3;

  systemFlags = 0;
  displayFormat = DF_ALL;
  displayFormatDigits = 0;
  setSystemFlag(FLAG_TDM24); // time format = 24H
  clearSystemFlag(FLAG_CPXj);
  fnAngularMode(AM_DEGREE);
  setSystemFlag(FLAG_DENANY);
  denMax = MAX_DENMAX;
  clearSystemFlag(FLAG_LEAD0);
  setSystemFlag(FLAG_MULTx);
  clearSystemFlag(FLAG_FRACT);
  clearSystemFlag(FLAG_PROPFR);
  setSystemFlag(FLAG_DECIMP);
  clearSystemFlag(FLAG_CPXRES);
  clearSystemFlag(FLAG_POLAR);
  clearSystemFlag(FLAG_ALLENG);
  setSystemFlag(FLAG_AUTOFF);
  clearSystemFlag(FLAG_SSIZE8);
  clearSystemFlag(FLAG_MDY); // date format
  clearSystemFlag(FLAG_DMY); // date format
  setSystemFlag(FLAG_YMD);   // date format
  clearSystemFlag(FLAG_OVERFLOW);
  clearSystemFlag(FLAG_CARRY);
  clearSystemFlag(FLAG_USER);
  clearSystemFlag(FLAG_LOWBAT);
  clearSystemFlag(FLAG_USB);

  hourGlassIconEnabled = false;
  programCounter = 0;
  watchIconEnabled = false;
  serialIOIconEnabled = false;
  printerIconEnabled = false;
  thereIsSomethingToUndo = false;

  significantDigits = 0;
  fnRoundingMode(RM_HALF_EVEN); // DEC_ROUND_HALF_EVEN
  fnDisplayStack(4);

  shiftF = false;
  shiftG = false;

  initFontBrowser();
  currentFlgScr = 0;
  currentRegisterBrowserScreen = 9999;

  softmenuStackPointer = 0;

  aimBuffer[0] = 0;

  setSystemFlag(FLAG_ASLIFT);

  lastErrorCode = 0;

  gammaLanczosCoefficients = (real51_t *)const_gammaC01;

  angle180 = (real39_t *)const_180;
  angle90  = (real39_t *)const_90;
  angle45  = (real39_t *)const_45;

  alphaSelectionMenu = ASM_NONE;

  #ifndef TESTSUITE_BUILD
    resetAlphaSelectionBuffer();
  #endif

  lastFcnsMenuPos = 0;
  lastMenuMenuPos = 0;
  lastCnstMenuPos = 0;
  lastSyFlMenuPos = 0;
  lastAIntMenuPos = 0;

  exponentLimit = 6145;

  #ifdef TESTSUITE_BUILD
    calcMode = CM_NORMAL;
    clearSystemFlag(FLAG_ALPHA);
  #else
    calcModeNormal();
  #endif // TESTSUITE_BUILD

  #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
    debugMemAllocation = true;
  #endif

  // Defining a little program

  uint16_t step = 0;
  pgm = allocWp43s(1200);
  while(1) {
    // 1
    *(pgm + step++) = ITM_LBL;
    *(pgm + step++) = STRING_LABEL_VARIABLE;
    *(pgm + step++) = 5; // String length
    *(pgm + step++) = 'P';
    *(pgm + step++) = 'r';
    *(pgm + step++) = 'i';
    *(pgm + step++) = 'm';
    *(pgm + step++) = 'e';

    // 2
    *(pgm + step++) = ITM_MAGNITUDE;

    // 3
    *(pgm + step++) = ITM_IP;

    // 4
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = 0;

    // 5
    *(pgm + step++) = ITM_SQUAREROOTX;

    // 6
    *(pgm + step++) = ITM_IP;

    // 7
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = 2;

    // 8
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '1'; // THE string

    // 9
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = 1;

    // 10
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 11
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '1'; // THE string

    // 12
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 13
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '2'; // THE string

    // 14
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 15
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '2'; // THE string

    // 16
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 17
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '4'; // THE string

    // 18
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 19
    *(pgm + step++) = ITM_LBL;
    *(pgm + step++) = 2;

    // 20
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '2'; // THE string

    // 21
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 22
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '4'; // THE string

    // 23
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 24
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '2'; // THE string

    // 25
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 26
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '4'; // THE string

    // 27
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 28
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '6'; // THE string

    // 29
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 30
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '2'; // THE string

    // 31
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 32
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '6'; // THE string

    // 33
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 34
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '4'; // THE string

    // 35
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 36
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '2'; // THE string

    // 37
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 38
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '4'; // THE string

    // 39
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 40
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '6'; // THE string

    // 41
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 42
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '6'; // THE string

    // 43
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 44
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '2'; // THE string

    // 45
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 46
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '6'; // THE string

    // 47
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 48
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '4'; // THE string

    // 49
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 50
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '2'; // THE string

    // 51
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 52
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '6'; // THE string

    // 53
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 54
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '4'; // THE string

    // 55
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 56
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '6'; // THE string

    // 57
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 58
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '8'; // THE string

    // 59
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 60
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '4'; // THE string

    // 61
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 62
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '2'; // THE string

    // 63
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 64
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '4'; // THE string

    // 65
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 66
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '2'; // THE string

    // 67
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 68
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '4'; // THE string

    // 69
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 70
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '8'; // THE string

    // 71
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 72
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '6'; // THE string

    // 73
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 74
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '4'; // THE string

    // 75
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 76
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '6'; // THE string

    // 77
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 78
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '2'; // THE string

    // 79
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 80
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '4'; // THE string

    // 81
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 82
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '6'; // THE string

    // 83
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 84
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '2'; // THE string

    // 85
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 86
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '6'; // THE string

    // 87
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 88
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '6'; // THE string

    // 89
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 90
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '4'; // THE string

    // 91
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 92
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '2'; // THE string

    // 93
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 94
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '4'; // THE string

    // 94
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 96
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '6'; // THE string

    // 97
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 98
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '2'; // THE string

    // 99
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 100
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '6'; // THE string

    // 101
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 102
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '4'; // THE string

    // 103
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 104
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '2'; // THE string

    // 105
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 106
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '4'; // THE string

    // 107
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 108
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '2'; // THE string

    // 109
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 110
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 2; // String length
    *(pgm + step++) = '1'; // THE string
    *(pgm + step++) = '0'; // THE string

    // 111
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 112
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '2'; // THE string

    // 113
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 114
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 2; // String length
    *(pgm + step++) = '1'; // THE string
    *(pgm + step++) = '0'; // THE string

    // 115
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 9;

    // 116
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 2;

    // 117
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 1;

    // 118
    *(pgm + step++) = ITM_XLT;
    *(pgm + step++) = REGISTER_Y;

    // 119
    *(pgm + step++) = ITM_GTO;
    *(pgm + step++) = 2;

    // 120
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 0;

    // 121
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 0;

    // 122
    *(pgm + step++) = (ITM_STOP >> 8) | 0x80;
    *(pgm + step++) =  ITM_STOP       & 0xff;

    // 123
    *(pgm + step++) = ITM_LBL;
    *(pgm + step++) = 9;

    // 124
    *(pgm + step++) = ITM_STOPLUS;
    *(pgm + step++) = 1;

    // 125
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 0;

    // 126
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 1;

    // 127
    *(pgm + step++) = ITM_MOD;

    // 128
    *(pgm + step++) = ITM_XNE;
    *(pgm + step++) = VALUE_0;

    // 129
    *(pgm + step++) = ITM_RTN;

    // 130
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 0;

    // 131
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 1;

    // 132
    *(pgm + step++) = (ITM_END >> 8) | 0x80;
    *(pgm + step++) =  ITM_END       & 0xff;

    printf("Prime : %u bytes\n", step);
    break;
  }


  while(1) {
    // 1
    *(pgm + step++) = ITM_LBL;
    *(pgm + step++) = STRING_LABEL_VARIABLE;
    *(pgm + step++) = 7; // String length
    *(pgm + step++) = 'B';
    *(pgm + step++) = 'a';
    *(pgm + step++) = 'i';
    *(pgm + step++) = 'r';
    *(pgm + step++) = 's';
    *(pgm + step++) = 't';
    *(pgm + step++) = 'o';

    // 2
    *(pgm + step++) = (ITM_ALL >> 8) | 0x80;
    *(pgm + step++) =  ITM_ALL       & 0xff;
    *(pgm + step++) = 0;

    // 3
    *(pgm + step++) = (ITM_CLREGS >> 8) | 0x80;
    *(pgm + step++) =  ITM_CLREGS       & 0xff;

    // 4
    *(pgm + step++) = (ITM_CLSTK >> 8) | 0x80;
    *(pgm + step++) =  ITM_CLSTK       & 0xff;

    // 5
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++)= STRING_LABEL_VARIABLE;
    *(pgm + step++) = 19; // String length
    *(pgm + step++) = 'P'; // THE string
    *(pgm + step++) = 'o';
    *(pgm + step++) = 'l';
    *(pgm + step++) = 'y';
    *(pgm + step++) = 'n';
    *(pgm + step++) = 'o';
    *(pgm + step++) = 'm';
    *(pgm + step++) = 'i';
    *(pgm + step++) = 'a';
    *(pgm + step++) = 'l';
    *(pgm + step++) = ' ';
    *(pgm + step++) = 'd';
    *(pgm + step++) = 'e';
    *(pgm + step++) = 'g';
    *(pgm + step++) = 'r';
    *(pgm + step++) = 'e';
    *(pgm + step++) = 'e';
    *(pgm + step++) = ' ';
    *(pgm + step++) = '?';

    // 6
    *(pgm + step++) = (ITM_STOP >> 8) | 0x80;
    *(pgm + step++) =  ITM_STOP       & 0xff;

    // 7
    *(pgm + step++) = ITM_MAGNITUDE;

    // 8
    *(pgm + step++) = ITM_IP;

    // 9
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = 90;

    // 10
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = 91;

    // 11
    *(pgm + step++) = (ITM_CLSTK >> 8) | 0x80;
    *(pgm + step++) =  ITM_CLSTK       & 0xff;

    // 12
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 91;

    // 13
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 3; // String length
    *(pgm + step++) = '1'; // THE string
    *(pgm + step++) = 'e';
    *(pgm + step++) = '3';

    // 14
    *(pgm + step++) = ITM_DIV;

    // 15
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = 92;

    // 16
    *(pgm + step++) = ITM_LBL;
    *(pgm + step++) = 100 + 'A' - 'A'; // A

    // 17
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++)= STRING_LABEL_VARIABLE;
    *(pgm + step++) = 2; // String length
    *(pgm + step++) = 'x'; // THE string
    *(pgm + step++) = '^';

    // 18
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 91;

    // 19
    *(pgm + step++) = ITM_ADD;

    // 20
    *(pgm + step++) = (ITM_STOP >> 8) | 0x80;
    *(pgm + step++) =  ITM_STOP       & 0xff;

    // 21
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = INDIRECT_REGISTER;
    *(pgm + step++) = 93;

    // 22
    *(pgm + step++) = (ITM_CLSTK >> 8) | 0x80;
    *(pgm + step++) =  ITM_CLSTK       & 0xff;

    // 23
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '1'; // THE string

    // 24
    *(pgm + step++) = ITM_STOPLUS;
    *(pgm + step++) = 93;

    // 25
    *(pgm + step++) = ITM_STOPLUS;
    *(pgm + step++) = 93;

    // 26
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 92;

    // 27
    *(pgm + step++) = ITM_ISG;
    *(pgm + step++) = 92;

    // 28
    *(pgm + step++) = ITM_GTO;
    *(pgm + step++) = 100 + 'A' - 'A'; // A

    // 29
    *(pgm + step++) = (ITM_CLSTK >> 8) | 0x80;
    *(pgm + step++) =  ITM_CLSTK       & 0xff;

    // 30
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '1'; // THE string

    // 31
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 0;

    // 32
    *(pgm + step++) = ITM_XNE;
    *(pgm + step++) = REGISTER_Y;

    // 33
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 100 + 'B' - 'A'; // B

    // 34
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++)= STRING_LABEL_VARIABLE;
    *(pgm + step++) = 10; // String length
    *(pgm + step++) = 'T'; // THE string
    *(pgm + step++) = 'o';
    *(pgm + step++) = 'l';
    *(pgm + step++) = 'e';
    *(pgm + step++) = 'r';
    *(pgm + step++) = 'a';
    *(pgm + step++) = 'n';
    *(pgm + step++) = 'c';
    *(pgm + step++) = 'e';
    *(pgm + step++) = '?';

    // 35
    *(pgm + step++) = (ITM_STOP >> 8) | 0x80;
    *(pgm + step++) =  ITM_STOP       & 0xff;

    // 36
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = 99;

    // 37
    *(pgm + step++) = (ITM_CLSTK >> 8) | 0x80;
    *(pgm + step++) =  ITM_CLSTK       & 0xff;

    // 38
    *(pgm + step++) = ITM_LBL;
    *(pgm + step++) = 10;

    // 39
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '3'; // THE string

    // 40
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = 94;

    // 41
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 94;

    // 42
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 90;

    // 43
    *(pgm + step++) = ITM_XLT;
    *(pgm + step++) = REGISTER_X;

    // 44
    *(pgm + step++) = ITM_GTO;
    *(pgm + step++) = 2;

    // 45
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '0'; // THE string

    // 46
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = 95;

    // 47
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = 96;

    // 48
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 100 + 'D' - 'A'; // D

    // 49
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 100 + 'J' - 'A'; // J

    // 50
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 11;

    // 51
    *(pgm + step++) = (ITM_STOP >> 8) | 0x80;
    *(pgm + step++) =  ITM_STOP       & 0xff;

    // 52
    *(pgm + step++) = ITM_GTO;
    *(pgm + step++) = 10;

    // 53
    *(pgm + step++) = ITM_LBL;
    *(pgm + step++) = 2;

    // 54
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '2'; // THE string

    // 55
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = 94;

    // 56
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 94;

    // 57
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 90;

    // 58
    *(pgm + step++) = ITM_XNE;
    *(pgm + step++) = REGISTER_Y;

    // 59
    *(pgm + step++) = ITM_GTO;
    *(pgm + step++) = 1;

    // 60
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 1;

    // 61
    *(pgm + step++) = ITM_GTO;
    *(pgm + step++) = 90;

    // 62
    *(pgm + step++) = ITM_LBL;
    *(pgm + step++) = 1;

    // 63
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 13;

    // 64
    *(pgm + step++) = ITM_LBL;
    *(pgm + step++) = 90;

    // 65
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++)= STRING_LABEL_VARIABLE;
    *(pgm + step++) = 9; // String length
    *(pgm + step++) = 'L'; // THE string
    *(pgm + step++) = 'a';
    *(pgm + step++) = 's';
    *(pgm + step++) = 't';
    *(pgm + step++) = ' ';
    *(pgm + step++) = 'r';
    *(pgm + step++) = 'o';
    *(pgm + step++) = 'o';
    *(pgm + step++) = 't';

    // 66
    *(pgm + step++) = ITM_RTN;

    // 67
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 100 + 'B' - 'A'; // B

    // 68
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '1'; // THE string

    // 69
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = 93;

    // 70
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 90;

    // 71
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 3; // String length
    *(pgm + step++) = '1'; // THE string
    *(pgm + step++) = 'e';
    *(pgm + step++) = '3';

    // 72
    *(pgm + step++) = ITM_DIV;

    // 73
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = 92;

    // 74
    *(pgm + step++) = ITM_XEQ;
    *(pgm + step++) = 100 + 'C' - 'A'; // C

    // 75
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 0;

    // 76
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = INDIRECT_REGISTER;
    *(pgm + step++) = 93;

    // 77
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '1'; // THE string

    // 78
    *(pgm + step++) = ITM_STOPLUS;
    *(pgm + step++) = 93;

    // 79
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 92;

    // 80
    *(pgm + step++) = ITM_ISG;
    *(pgm + step++) = 92;

    // 81
    *(pgm + step++) = ITM_GTO;
    *(pgm + step++) = 100 + 'C' - 'A'; // C

    // 82
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '1'; // THE string

    // 83
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = 0;

    // 84
    *(pgm + step++) = ITM_RTN;

    // 85
    *(pgm + step++) = ITM_LBL;
    *(pgm + step++) = 100 + 'D' - 'A'; // D

    // 86
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++)= STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '1'; // THE string

    // 87
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = 97;

    // 88
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++)= STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '0'; // THE string

    // 89
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = 98;

    // 90
    *(pgm + step++) = ITM_LBL;
    *(pgm + step++) = 100 + 'I' - 'A'; // I

    // 91
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++)= STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '1'; // THE string

    // 92
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = 91;

    // 93
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 90;

    // 94
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 3; // String length
    *(pgm + step++) = '1'; // THE string
    *(pgm + step++) = 'e';
    *(pgm + step++) = '3';

    // 95
    *(pgm + step++) = ITM_DIV;

    // 96
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++)= STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '1'; // THE string

    // 97
    *(pgm + step++) = ITM_ADD;

    // 98
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = 89;

    // 98
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++)= STRING_REAL34;
    *(pgm + step++) = 2; // String length
    *(pgm + step++) = '3'; // THE string
    *(pgm + step++) = '0'; // THE string

    // 100
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = 92;

    // 101
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = 93;

    // 102
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = 94;

    // 103
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++)= STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '1'; // THE string

    // 104
    *(pgm + step++) = ITM_STOPLUS;
    *(pgm + step++) = 93;

    // 105
    *(pgm + step++) = ITM_STOPLUS;
    *(pgm + step++) = 94;

    // 106
    *(pgm + step++) = ITM_STOPLUS;
    *(pgm + step++) = 94;

    // 107
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++)= STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '0'; // THE string

    // 108
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = INDIRECT_REGISTER;
    *(pgm + step++) = 92;

    // 109
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++)= STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '1'; // THE string

    // 110
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = INDIRECT_REGISTER;
    *(pgm + step++) = 93;

    // 111
    *(pgm + step++) = ITM_LBL;
    *(pgm + step++) = 100 + 'L' - 'A'; // L

    // 112
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = INDIRECT_REGISTER;
    *(pgm + step++) = 91;

    // 113
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = INDIRECT_REGISTER;
    *(pgm + step++) = 94;

    // 114
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = INDIRECT_REGISTER;
    *(pgm + step++) = 93;

    // 115
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 95;

    // 116
    *(pgm + step++) = ITM_MULT;

    // 117
    *(pgm + step++) = ITM_CHS;

    // 118
    *(pgm + step++) = ITM_STOPLUS;
    *(pgm + step++) = INDIRECT_REGISTER;
    *(pgm + step++) = 94;

    // 119
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = INDIRECT_REGISTER;
    *(pgm + step++) = 92;

    // 120
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 96;

    // 121
    *(pgm + step++) = ITM_MULT;

    // 122
    *(pgm + step++) = ITM_CHS;

    // 123
    *(pgm + step++) = ITM_STOPLUS;
    *(pgm + step++) = INDIRECT_REGISTER;
    *(pgm + step++) = 94;

    // 124
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++)= STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '1'; // THE string

    // 125
    *(pgm + step++) = ITM_STOPLUS;
    *(pgm + step++) = INDIRECT_REGISTER;
    *(pgm + step++) = 91;

    // 126
    *(pgm + step++) = ITM_STOPLUS;
    *(pgm + step++) = INDIRECT_REGISTER;
    *(pgm + step++) = 92;

    // 127
    *(pgm + step++) = ITM_STOPLUS;
    *(pgm + step++) = INDIRECT_REGISTER;
    *(pgm + step++) = 93;

    // 128
    *(pgm + step++) = ITM_STOPLUS;
    *(pgm + step++) = INDIRECT_REGISTER;
    *(pgm + step++) = 94;

    // 129
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 89;

    // 130
    *(pgm + step++) = ITM_ISG;
    *(pgm + step++) = 89;

    // 131
    *(pgm + step++) = ITM_GTO;
    *(pgm + step++) = 100 + 'E' - 'A'; // E

    // 132
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = INDIRECT_REGISTER;
    *(pgm + step++) = 92;

    // 133
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = 84;

    // 134
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = INDIRECT_REGISTER;
    *(pgm + step++) = 93;

    // 135
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = 85;

    // 136
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++)= STRING_REAL34;
    *(pgm + step++) = 2; // String length
    *(pgm + step++) = '3'; // THE string
    *(pgm + step++) = '0'; // THE string

    // 137
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = 91;

    // 138
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 90;

    // 139
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 3; // String length
    *(pgm + step++) = '1'; // THE string
    *(pgm + step++) = 'e';
    *(pgm + step++) = '3';

    // 140
    *(pgm + step++) = ITM_DIV;

    // 141
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++)= STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '1'; // THE string

    // 142
    *(pgm + step++) = ITM_ADD;

    // 143
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = 89;

    // 144
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++)= STRING_REAL34;
    *(pgm + step++) = 2; // String length
    *(pgm + step++) = '6'; // THE string
    *(pgm + step++) = '0'; // THE string

    // 145
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = 92;

    // 146
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = 93;

    // 147
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = 94;

    // 148
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++)= STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '1'; // THE string

    // 149
    *(pgm + step++) = ITM_STOPLUS;
    *(pgm + step++) = 93;

    // 150
    *(pgm + step++) = ITM_STOPLUS;
    *(pgm + step++) = 94;

    // 151
    *(pgm + step++) = ITM_STOPLUS;
    *(pgm + step++) = 94;

    // 152
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++)= STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '0'; // THE string

    // 153
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = INDIRECT_REGISTER;
    *(pgm + step++) = 92;

    // 154
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++)= STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '0'; // THE string

    // 155
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = INDIRECT_REGISTER;
    *(pgm + step++) = 93;

    // 156
    *(pgm + step++) = ITM_LBL;
    *(pgm + step++) = 100 + 'F' - 'A'; // F

    // 157
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = INDIRECT_REGISTER;
    *(pgm + step++) = 91;

    // 158
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = INDIRECT_REGISTER;
    *(pgm + step++) = 94;

    // 159
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = INDIRECT_REGISTER;
    *(pgm + step++) = 93;

    // 160
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 95;

    // 161
    *(pgm + step++) = ITM_MULT;

    // 162
    *(pgm + step++) = ITM_CHS;

    // 163
    *(pgm + step++) = ITM_STOPLUS;
    *(pgm + step++) = INDIRECT_REGISTER;
    *(pgm + step++) = 94;

    // 164
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = INDIRECT_REGISTER;
    *(pgm + step++) = 92;

    // 165
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 96;

    // 166
    *(pgm + step++) = ITM_MULT;

    // 167
    *(pgm + step++) = ITM_CHS;

    // 168
    *(pgm + step++) = ITM_STOPLUS;
    *(pgm + step++) = INDIRECT_REGISTER;
    *(pgm + step++) = 94;

    // 169
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++)= STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '1'; // THE string

    // 170
    *(pgm + step++) = ITM_STOPLUS;
    *(pgm + step++) = INDIRECT_REGISTER;
    *(pgm + step++) = 91;

    // 171
    *(pgm + step++) = ITM_STOPLUS;
    *(pgm + step++) = INDIRECT_REGISTER;
    *(pgm + step++) = 92;

    // 172
    *(pgm + step++) = ITM_STOPLUS;
    *(pgm + step++) = INDIRECT_REGISTER;
    *(pgm + step++) = 93;

    // 173
    *(pgm + step++) = ITM_STOPLUS;
    *(pgm + step++) = INDIRECT_REGISTER;
    *(pgm + step++) = 94;

    // 174
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 89;

    // 175
    *(pgm + step++) = ITM_ISG;
    *(pgm + step++) = 89;

    // 176
    *(pgm + step++) = ITM_GTO;
    *(pgm + step++) = 100 + 'F' - 'A'; // F

    // 177
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = INDIRECT_REGISTER;
    *(pgm + step++) = 92;

    // 178
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = 92;

    // 179
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = INDIRECT_REGISTER;
    *(pgm + step++) = 93;

    // 180
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = 93;

    // 181
    *(pgm + step++) = (ITM_CLSTK >> 8) | 0x80;
    *(pgm + step++) =  ITM_CLSTK       & 0xff;

    // 182
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 95;

    // 183
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 93;

    // 184
    *(pgm + step++) = ITM_MULT;

    // 185
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 96;

    // 186
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 92;

    // 187
    *(pgm + step++) = ITM_MULT;

    // 188
    *(pgm + step++) = ITM_ADD;

    // 189
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 92;

    // 190
    *(pgm + step++) = ITM_MULT;

    // 191
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 93;

    // 192
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 93;

    // 193
    *(pgm + step++) = ITM_MULT;

    // 194
    *(pgm + step++) = ITM_ADD;

    // 195
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = 88;

    // 196
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 84;

    // 197
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 93;

    // 198
    *(pgm + step++) = ITM_MULT;

    // 199
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 85;

    // 200
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 92;

    // 201
    *(pgm + step++) = ITM_MULT;

    // 202
    *(pgm + step++) = ITM_SUB;

    // 203
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 88;

    // 204
    *(pgm + step++) = ITM_DIV;

    // 205
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = 87;

    // 206
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 95;

    // 207
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 93;

    // 208
    *(pgm + step++) = ITM_MULT;

    // 209
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 96;

    // 210
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 92;

    // 211
    *(pgm + step++) = ITM_MULT;

    // 212
    *(pgm + step++) = ITM_ADD;

    // 213
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 84;

    // 214
    *(pgm + step++) = ITM_MULT;

    // 215
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 85;

    // 216
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 93;

    // 217
    *(pgm + step++) = ITM_MULT;

    // 218
    *(pgm + step++) = ITM_ADD;

    // 219
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 88;

    // 220
    *(pgm + step++) = ITM_DIV;

    // 221
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = 86;

    // 222
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 87;

    // 223
    *(pgm + step++) = ITM_MAGNITUDE;

    // 224
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = REGISTER_X;

    // 225
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 86;

    // 226
    *(pgm + step++) = ITM_MAGNITUDE;

    // 227
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = REGISTER_Y;

    // 228
    *(pgm + step++) = ITM_XGT;
    *(pgm + step++) = REGISTER_Y;

    // 229
    *(pgm + step++) = ITM_GTO;
    *(pgm + step++) = 100 + 'G' - 'A'; // G

    // 230
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 87;

    // 231
    *(pgm + step++) = ITM_MAGNITUDE;

    // 232
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = 97;

    // 233
    *(pgm + step++) = ITM_GTO;
    *(pgm + step++) = 100 + 'H' - 'A'; // H

    // 234
    *(pgm + step++) = ITM_LBL;
    *(pgm + step++) = 100 + 'G' - 'A'; // G

    // 235
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 86;

    // 236
    *(pgm + step++) = ITM_MAGNITUDE;

    // 237
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = 97;

    // 238
    *(pgm + step++) = ITM_LBL;
    *(pgm + step++) = 100 + 'H' - 'A'; // H

    // 239
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 87;

    // 240
    *(pgm + step++) = ITM_STOPLUS;
    *(pgm + step++) = 95;

    // 241
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 86;

    // 242
    *(pgm + step++) = ITM_STOPLUS;
    *(pgm + step++) = 96;

    // 243
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++)= STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '1'; // THE string

    // 244
    *(pgm + step++) = ITM_STOPLUS;
    *(pgm + step++) = 98;

    // 245
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 3; // String length
    *(pgm + step++) = '1'; // THE string
    *(pgm + step++) = 'e';
    *(pgm + step++) = '4';

    // 246
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = REGISTER_X;

    // 247
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 98;

    // 248
    *(pgm + step++) = ITM_XGT;
    *(pgm + step++) = REGISTER_Y;

    // 249
    *(pgm + step++) = ITM_GTO;
    *(pgm + step++) = 4;

    // 250
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 99;

    // 251
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 97;

    // 252
    *(pgm + step++) = ITM_XGT;
    *(pgm + step++) = REGISTER_Y;

    // 253
    *(pgm + step++) = ITM_GTO;
    *(pgm + step++) = 100 + 'I' - 'A';

    // 254
    *(pgm + step++) = ITM_RTN;

    // 255
    *(pgm + step++) = ITM_LBL;
    *(pgm + step++) = 100 + 'J' - 'A'; // J

    // 256
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++)= STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '0'; // THE string

    // 257
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = 91;

    // 258
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++)= STRING_REAL34;
    *(pgm + step++) = 2; // String length
    *(pgm + step++) = '3'; // THE string
    *(pgm + step++) = '1'; // THE string

    // 259
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = 92;

    // 260
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 90;

    // 261
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++)= STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '1'; // THE string

    // 262
    *(pgm + step++) = ITM_SUB;

    // 263
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = 93;

    // 264
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++) = STRING_REAL34;
    *(pgm + step++) = 3; // String length
    *(pgm + step++) = '1'; // THE string
    *(pgm + step++) = 'e';
    *(pgm + step++) = '3';

    // 265
    *(pgm + step++) = ITM_DIV;

    // 266
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = 94;

    // 267
    *(pgm + step++) = ITM_LBL;
    *(pgm + step++) = 88;

    // 268
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = INDIRECT_REGISTER;
    *(pgm + step++) = 92;

    // 269
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = INDIRECT_REGISTER;
    *(pgm + step++) = 91;

    // 270
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++)= STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '1'; // THE string

    // 271
    *(pgm + step++) = ITM_STOPLUS;
    *(pgm + step++) = 91;

    // 272
    *(pgm + step++) = ITM_STOPLUS;
    *(pgm + step++) = 92;

    // 273
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 94;

    // 274
    *(pgm + step++) = ITM_ISG;
    *(pgm + step++) = 94;

    // 275
    *(pgm + step++) = ITM_GTO;
    *(pgm + step++) = 88;

    // 276
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 90;

    // 277
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++)= STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '2'; // THE string

    // 278
    *(pgm + step++) = ITM_SUB;

    // 279
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = 90;

    // 280
    *(pgm + step++) = ITM_RTN;

    // 281
    *(pgm + step++) = ITM_LBL;
    *(pgm + step++) = 11;

    // 282
    *(pgm + step++) = (ITM_CLSTK >> 8) | 0x80;
    *(pgm + step++) =  ITM_CLSTK       & 0xff;

    // 283
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 95;

    // 284
    *(pgm + step++) = ITM_CHS;

    // 285
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++)= STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '2'; // THE string

    // 286
    *(pgm + step++) = ITM_DIV;

    // 287
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = 91;

    // 288
    *(pgm + step++) = ITM_SQUARE;

    // 289
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 96;

    // 290
    *(pgm + step++) = ITM_SUB;

    // 291
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = 92;

    // 292
    *(pgm + step++) = (ITM_CLSTK >> 8) | 0x80;
    *(pgm + step++) =  ITM_CLSTK       & 0xff;

    // 293
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 91;

    // 294
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 92;

    // 295
    *(pgm + step++) = ITM_SQUAREROOTX;

    // 296
    *(pgm + step++) = ITM_ADD;

    // 297
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 91;

    // 298
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 92;

    // 299
    *(pgm + step++) = ITM_SQUAREROOTX;

    // 300
    *(pgm + step++) = ITM_SUB;

    // 301
    *(pgm + step++) = ITM_RTN;

    // 302
    *(pgm + step++) = ITM_LBL;
    *(pgm + step++) = 12;

    // 303
    *(pgm + step++) = (ITM_CLSTK >> 8) | 0x80;
    *(pgm + step++) =  ITM_CLSTK       & 0xff;

    // 304
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++)= STRING_LABEL_VARIABLE;
    *(pgm + step++) = 12; // String length
    *(pgm + step++) = '.'; // THE string
    *(pgm + step++) = '.';
    *(pgm + step++) = '.';
    *(pgm + step++) = ' ';
    *(pgm + step++) = 'c';
    *(pgm + step++) = 'o';
    *(pgm + step++) = 'n';
    *(pgm + step++) = 't';
    *(pgm + step++) = 'i';
    *(pgm + step++) = 'n';
    *(pgm + step++) = 'u';
    *(pgm + step++) = 'e';

    // 305
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 1;

    // 306
    *(pgm + step++) = ITM_CHS;

    // 307
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++)= STRING_REAL34;
    *(pgm + step++) = 1; // String length
    *(pgm + step++) = '2'; // THE string

    // 308
    *(pgm + step++) = ITM_DIV;

    // 309
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = 91;

    // 310
    *(pgm + step++) = ITM_SQUARE;

    // 311
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 2;

    // 312
    *(pgm + step++) = ITM_SUB;

    // 313
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = 92;

    // 314
    *(pgm + step++) = (ITM_CLSTK >> 8) | 0x80;
    *(pgm + step++) =  ITM_CLSTK       & 0xff;

    // 315
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 91;

    // 316
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 92;

    // 317
    *(pgm + step++) = ITM_SQUAREROOTX;

    // 318
    *(pgm + step++) = ITM_ADD;

    // 319
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 91;

    // 320
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 92;

    // 321
    *(pgm + step++) = ITM_SQUAREROOTX;

    // 322
    *(pgm + step++) = ITM_SUB;

    // 323
    *(pgm + step++) = ITM_RTN;

    // 324
    *(pgm + step++) = ITM_LBL;
    *(pgm + step++) = 13;

    // 325
    *(pgm + step++) = (ITM_CLSTK >> 8) | 0x80;
    *(pgm + step++) =  ITM_CLSTK       & 0xff;

    // 326
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++)= STRING_LABEL_VARIABLE;
    *(pgm + step++) = 12; // String length
    *(pgm + step++) = '.'; // THE string
    *(pgm + step++) = '.';
    *(pgm + step++) = '.';
    *(pgm + step++) = ' ';
    *(pgm + step++) = 'c';
    *(pgm + step++) = 'o';
    *(pgm + step++) = 'n';
    *(pgm + step++) = 't';
    *(pgm + step++) = 'i';
    *(pgm + step++) = 'n';
    *(pgm + step++) = 'u';
    *(pgm + step++) = 'e';

    // 327
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 1;

    // 328
    *(pgm + step++) = ITM_CHS;

    // 329
    *(pgm + step++) = ITM_STO;
    *(pgm + step++) = 91;

    // 330
    *(pgm + step++) = (ITM_CLSTK >> 8) | 0x80;
    *(pgm + step++) =  ITM_CLSTK       & 0xff;

    // 331
    *(pgm + step++) = ITM_RCL;
    *(pgm + step++) = 91;

    // 332
    *(pgm + step++) = ITM_RTN;

    // 333
    *(pgm + step++) = ITM_LBL;
    *(pgm + step++) = 4;

    // 334
    *(pgm + step++) = (ITM_CLSTK >> 8) | 0x80;
    *(pgm + step++) =  ITM_CLSTK       & 0xff;

    // 335
    *(pgm + step++) = ITM_LITT;
    *(pgm + step++)= STRING_LABEL_VARIABLE;
    *(pgm + step++) = 11; // String length
    *(pgm + step++) = 'E'; // THE string
    *(pgm + step++) = 'r';
    *(pgm + step++) = 'r';
    *(pgm + step++) = 'o';
    *(pgm + step++) = 'r';
    *(pgm + step++) = ' ';
    *(pgm + step++) = 'm';
    *(pgm + step++) = '>';
    *(pgm + step++) = '8';
    *(pgm + step++) = '0';
    *(pgm + step++) = '!';

    // 336
    *(pgm + step++) = ITM_RTN;

    // 337
    *(pgm + step++) = 127 | 0x80; // .END.
    *(pgm + step++) = 255;        // .END.

    printf("Prime + Bairstow : %u bytes\n", step);
    break;
  }

  step = 0;
  stepAddress = pgm;
  while(stepAddress) {
    printf("%4u  ", step++);
    if(*stepAddress == ((ITM_END >> 8) | 0x80) && *(stepAddress + 1) == (ITM_END & 0xff)) {
      step = 1;
    }
    stepAddress = decodeOneStep();
  }

  exit(0);
}



#ifdef PC_BUILD
int main(int argc, char* argv[]) {
  #if defined __APPLE__
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
  #endif

  wp43sMemInBytes = 0;
  gmpMemInBytes = 0;
  mp_set_memory_functions(allocGmp, reallocGmp, freeGmp);

  calcLandscape             = false;
  calcAutoLandscapePortrait = true;

  for(int arg=1; arg<argc; arg++) {
    if(strcmp(argv[arg], "--landscape") == 0) {
      calcLandscape             = true;
      calcAutoLandscapePortrait = false;
    }

    if(strcmp(argv[arg], "--portrait") == 0) {
      calcLandscape             = false;
      calcAutoLandscapePortrait = false;
    }

    if(strcmp(argv[arg], "--auto") == 0) {
      calcLandscape             = false;
      calcAutoLandscapePortrait = true;
    }
  }

  if(strcmp(indexOfItems[LAST_ITEM].itemSoftmenuName, "Last item") != 0) {
    printf("The last item of indexOfItems[] is not \"Last item\"\n");
    exit(1);
  }

  gtk_init(&argc, &argv);
  setupUI();

  setupDefaults();
  restoreCalc();
  //fnReset(CONFIRMED);

  refreshScreen();

  gdk_threads_add_timeout(SCREEN_REFRESH_PERIOD, refreshLcd, NULL); // refreshLcd is called every SCREEN_REFRESH_PERIOD ms

  gtk_main();

  return 0;
}
#endif

#ifdef DMCP_BUILD
void program_main(void) {
  int key = 0;
  char charKey[3];
  bool_t wp43sKbdLayout;
  uint16_t currentVolumeSetting, savedVoluleSetting; // used for beep signaling screen shot

  wp43sMemInBytes = 0;
  gmpMemInBytes = 0;
  mp_set_memory_functions(allocGmp, reallocGmp, freeGmp);

  lcd_clear_buf();
  lcd_putsAt(t24, 4, "Press the bottom left key.");
  lcd_refresh();
  while(key != 33 && key != 37) {
    key = key_pop();
    while(key == -1) {
      sys_sleep();
      key = key_pop();
    }
  }

  wp43sKbdLayout = (key == 37); // bottom left key
  key = 0;

  lcd_clear_buf();
  setupDefaults();

  fnReset(CONFIRMED);
  refreshScreen();

/*longInteger_t li;
longIntegerInit(li);
uint32_t addr;

addr = (uint32_t)ram;
uIntToLongInteger(addr, li);
convertLongIntegerToShortIntegerRegister(li, 16, REGISTER_T);

addr = (uint32_t)row;
uIntToLongInteger(addr, li);
convertLongIntegerToShortIntegerRegister(li, 16, REGISTER_Z);

addr = (uint32_t)(&angle180);
uIntToLongInteger(addr, li);
convertLongIntegerToShortIntegerRegister(li, 16, REGISTER_Y);

addr = (uint32_t)indexOfItems;
uIntToLongInteger(addr, li);
convertLongIntegerToShortIntegerRegister(li, 16, REGISTER_X);

refreshScreen();
longIntegerFree(li);*/

  backToDMCP = false;

  lcd_refresh();
  nextScreenRefresh = sys_current_ms() + SCREEN_REFRESH_PERIOD;

  // Status flags:
  //   ST(STAT_PGM_END)   - Indicates that program should go to off state (set by auto off timer)
  //   ST(STAT_SUSPENDED) - Program signals it is ready for off and doesn't need to be woken-up again
  //   ST(STAT_OFF)       - Program in off state (OS goes to sleep and only [EXIT] key can wake it up again)
  //   ST(STAT_RUNNING)   - OS doesn't sleep in this mode
  while(!backToDMCP) {
    if(ST(STAT_PGM_END) && ST(STAT_SUSPENDED)) { // Already in off mode and suspended
      CLR_ST(STAT_RUNNING);
      sys_sleep();
    }
    else if((!ST(STAT_PGM_END) && key_empty())) {         // Just wait if no keys available.
      CLR_ST(STAT_RUNNING);
      sys_timer_start(TIMER_IDX_SCREEN_REFRESH, max(1, nextScreenRefresh - sys_current_ms()));  // wake up for screen refresh
      sys_sleep();
      sys_timer_disable(TIMER_IDX_SCREEN_REFRESH);
    }

    // Wakeup in off state or going to sleep
    if(ST(STAT_PGM_END) || ST(STAT_SUSPENDED)) {
      if(!ST(STAT_SUSPENDED)) {
        // Going to off mode
        lcd_set_buf_cleared(0); // Mark no buffer change region
        draw_power_off_image(1);

        LCD_power_off(0);
        SET_ST(STAT_SUSPENDED);
        SET_ST(STAT_OFF);
      }
      // Already in OFF -> just continue to sleep above
      continue;
    }

    // Well, we are woken-up
    SET_ST(STAT_RUNNING);

    // Clear suspended state, because now we are definitely reached the active state
    CLR_ST(STAT_SUSPENDED);

    // Get up from OFF state
    if(ST(STAT_OFF)) {
      LCD_power_on();
      rtc_wakeup_delay(); // Ensure that RTC readings after power off will be OK

      CLR_ST(STAT_OFF);

      if(!lcd_get_buf_cleared()) {
        lcd_forced_refresh(); // Just redraw from LCD buffer
      }
    }

    // Key is ready -> clear auto off timer
    if(!key_empty()) {
      reset_auto_off();
    }

    if(wp43sKbdLayout) {
      /////////////////////////////////////////////////
      // For key reassignment see:
      // https://technical.swissmicros.com/dm42/devel/dmcp_devel_manual/#_system_key_table
      //
      // Output of keymap2layout keymap.txt
      //
      //    +-----+-----+-----+-----+-----+-----+
      // 1: | F1  | F2  | F3  | F4  | F5  | F6  |
      //    |38:38|39:39|40:40|41:41|42:42|43:43|
      //    +-----+-----+-----+-----+-----+-----+
      // 2: | 1/x |Sum+ | SIN | LN  | LOG |SQRT |
      //    | 1: 2| 2: 1| 3:10| 4: 5| 5: 4| 6: 3|
      //    +-----+-----+-----+-----+-----+-----+
      // 3: | STO | RCL | RDN | COS | TAN |SHIFT|
      //    | 7: 7| 8: 8| 9: 9|10:11|11:12|12:28|
      //    +-----+-----+-----+-----+-----+-----+
      // 4: |   ENTER   |x<>y | CHS |  E  | <-- |
      //    |   13:13   |14:14|15:15|16:16|17:17|
      //    +-----------+-----+-----+-----+-----+
      // 5: |  DIV |   7  |   8  |   9  |  XEQ  |
      //    | 18:22| 19:19| 20:20| 21:21| 22: 6 |
      //    +------+------+------+------+-------+
      // 6: |  MUL |   4  |   5  |   6  |  UP   |
      //    | 23:27| 24:24| 25:25| 26:26| 27:18 |
      //    +------+------+------+------+-------+
      // 7: |  SUB |   1  |   2  |   3  | DOWN  |
      //    | 28:32| 29:29| 30:30| 31:31| 32:23 |
      //    +------+------+------+------+-------+
      // 8: |  ADD |   0  |  DOT |  RUN | EXIT  |
      //    | 33:37| 34:34| 35:35| 36:36| 37:33 |
      //    +------+------+------+------+-------+
      //
      // Fetch the key
      //  < 0 -> No key event
      //  > 0 -> Key pressed
      // == 0 -> Key released
      key = key_pop();

      //The switch instruction below is implemented as follows e.g. for the up arrow key on the WP43S layout:
      //  the output of keymap2layout for this key is UP 27:18 so we need the line:
      //    case 18: key = 27; break;
      switch(key) {               // Original
        case  1: key =  2; break; // SUM+
        case  2: key =  1; break; // 1/x
        case  3: key =  6; break; // SQRT
        case  4: key =  5; break; // LOG
        case  5: key =  4; break; // LN
        case  6: key = 22; break; // XEQ
      //case  7: key =  7; break; // STO
      //case  8: key =  8; break; // RCL
      //case  9: key =  9; break; // RDN
        case 10: key =  3; break; // SIN
        case 11: key = 10; break; // COS
        case 12: key = 11; break; // TAN
      //case 13: key = 13; break; // ENTER
      //case 14: key = 14; break; // x<>y
      //case 15: key = 15; break; // +/-
      //case 16: key = 16; break; // E
      //case 17: key = 17; break; // <--
        case 18: key = 27; break; // UP
      //case 19: key = 19; break; // 7
      //case 20: key = 20; break; // 8
      //case 21: key = 21; break; // 9
        case 22: key = 18; break; // /
        case 23: key = 32; break; // DOWN
      //case 24: key = 24; break; // 4
      //case 25: key = 25; break; // 5
      //case 26: key = 26; break; // 6
        case 27: key = 23; break; // x
        case 28: key = 12; break; // SHIFT
      //case 29: key = 29; break; // 1
      //case 30: key = 30; break; // 2
      //case 31: key = 31; break; // 3
        case 32: key = 28; break; // -
        case 33: key = 37; break; // EXIT
      //case 34: key = 34; break; // 0
      //case 35: key = 35; break; // .
      //case 36: key = 36; break; // R/S
        case 37: key = 33; break; // +
        default: {}
      }
    }
    else {
      // Fetch the key
      //  < 0 -> No key event
      //  > 0 -> Key pressed
      // == 0 -> Key released
      key = key_pop();
    }

    //The 3 lines below to see in the top left screen corner the pressed keycode
    //char sysLastKeyCh[5];
    //sprintf(sysLastKeyCh, "%2d", sys_last_key());
    //showString(sysLastKeyCh, &standardFont, 0, 0, vmReverse, true, true);

    if(sys_last_key() == 44 ) { //DISP for special SCREEN DUMP key code. To be 16 but shift decoding already done to 44 in DMCP
      shiftF = false;
      shiftG = false; //To avoid f or g top left of the screen, clear again to make sure

      currentVolumeSetting = get_beep_volume();
      savedVoluleSetting = currentVolumeSetting;
      while(currentVolumeSetting < 11) {
        beep_volume_up();
        currentVolumeSetting = get_beep_volume();
      }

      start_buzzer_freq(100000); //Click before screen dump
      sys_delay(5);
      stop_buzzer();

      create_screenshot(0);      //Screen dump

      start_buzzer_freq(400000); //Click after screen dump
      sys_delay(5);
      stop_buzzer();

      while(currentVolumeSetting != savedVoluleSetting) { //Restore volume
        beep_volume_down();
        currentVolumeSetting = get_beep_volume();
      }
    }

    if(38 <= key && key <=43) {
      sprintf(charKey, "%c", key+11);
      btnFnPressed(charKey);
      lcd_refresh();
    }
    else if(1 <= key && key <= 37) {
      sprintf(charKey, "%02d", key - 1);
      btnPressed(charKey);
      lcd_refresh();
    }
    else if(key == 0) {
      if(charKey[1] == 0) { // Last key pressed was one of the 6 function keys
        btnFnReleased(NULL);
      }
      else { // Last key pressed was not one of the 6 function keys
        btnReleased(NULL);
      }
      lcd_refresh();
    }

    uint32_t now = sys_current_ms();
    if(nextScreenRefresh <= now) {
      nextScreenRefresh = now + SCREEN_REFRESH_PERIOD;
      refreshLcd();
      lcd_refresh();
    }
  }
}
#endif

#ifdef TESTSUITE_BUILD
#include "testSuite.h"

int main(int argc, char* argv[]) {
  #if defined __APPLE__
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
  #endif

  wp43sMemInBytes = 0;
  gmpMemInBytes = 0;
  mp_set_memory_functions(allocGmp, reallocGmp, freeGmp);

  setupDefaults();

  fnReset(CONFIRMED);

/*
longInteger_t li;
longIntegerInit(li);
uIntToLongInteger(1, li);
convertLongIntegerToLongIntegerRegister(li, REGISTER_Z);
uIntToLongInteger(2, li);
convertLongIntegerToLongIntegerRegister(li, REGISTER_Y);
uIntToLongInteger(2203, li);
convertLongIntegerToLongIntegerRegister(li, REGISTER_X);
fnPower(NOPARAM);
fnSwapXY(NOPARAM);
fnSubtract(NOPARAM);
printf("a\n");
fnIsPrime(NOPARAM);
printf("b\n");
longIntegerFree(li);
return 0;
*/


  processTests();
  printf("The memory owned by GMP should be 0 bytes. Else report a bug please!\n");
  debugMemory();

  return 0;
}
#endif
