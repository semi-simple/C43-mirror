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

  pgm = allocWp43s(1024);
  // 1
  *(pgm +   0) = ITM_LBL;
  *(pgm +   1) = 255;
  *(pgm +   2) = (5 << 4) | 5; // 6 glyphs and 7 bytes
  *(pgm +   3) = 'P';
  *(pgm +   4) = 'r';
  *(pgm +   5) = 'i';
  *(pgm +   6) = 'm';
  *(pgm +   7) = 'e';

  // 2
  *(pgm +   8) = ITM_MAGNITUDE;

  // 3
  *(pgm +   9) = ITM_IP;

  // 4
  *(pgm +  10) = ITM_STO;
  *(pgm +  11) = 0;

  // 5
  *(pgm +  12) = ITM_SQUAREROOTX;

  // 6
  *(pgm +  13) = ITM_IP;

  // 7
  *(pgm +  14) = ITM_STO;
  *(pgm +  15) = 2;

  // 8
  *(pgm +  16) = ITM_LITT;
  *(pgm +  17) = 5; // String to real34
  *(pgm +  18) = 1; // String length
  *(pgm +  19) = '1'; // THE string

  // 9
  *(pgm +  20) = ITM_STO;
  *(pgm +  21) = 1;

  // 10
  *(pgm +  22) = ITM_XEQ;
  *(pgm +  23) = 9;

  // 11
  *(pgm +  24) = ITM_LITT;
  *(pgm +  25) = 5; // String to real34
  *(pgm +  26) = 1; // String length
  *(pgm +  27) = '1'; // THE string

  // 12
  *(pgm +  28) = ITM_XEQ;
  *(pgm +  29) = 9;

  // 13
  *(pgm +  30) = ITM_LITT;
  *(pgm +  31) = 5; // String to real34
  *(pgm +  32) = 1; // String length
  *(pgm +  33) = '2'; // THE string

  // 14
  *(pgm +  34) = ITM_XEQ;
  *(pgm +  35) = 9;

  // 15
  *(pgm +  36) = ITM_LITT;
  *(pgm +  37) = 5; // String to real34
  *(pgm +  38) = 1; // String length
  *(pgm +  39) = '2'; // THE string

  // 16
  *(pgm +  40) = ITM_XEQ;
  *(pgm +  41) = 9;

  // 17
  *(pgm +  42) = ITM_LITT;
  *(pgm +  43) = 5; // String to real34
  *(pgm +  44) = 1; // String length
  *(pgm +  45) = '4'; // THE string

  // 18
  *(pgm +  46) = ITM_XEQ;
  *(pgm +  47) = 9;

  // 19
  *(pgm +  48) = ITM_LBL;
  *(pgm +  49) = 2;

  // 20
  *(pgm +  50) = ITM_LITT;
  *(pgm +  51) = 5; // String to real34
  *(pgm +  52) = 1; // String length
  *(pgm +  53) = '2'; // THE string

  // 21
  *(pgm +  54) = ITM_XEQ;
  *(pgm +  55) = 9;

  // 22
  *(pgm +  56) = ITM_LITT;
  *(pgm +  57) = 5; // String to real34
  *(pgm +  58) = 1; // String length
  *(pgm +  59) = '4'; // THE string

  // 23
  *(pgm +  60) = ITM_XEQ;
  *(pgm +  61) = 9;

  // 24
  *(pgm +  62) = ITM_LITT;
  *(pgm +  63) = 5; // String to real34
  *(pgm +  64) = 1; // String length
  *(pgm +  65) = '2'; // THE string

  // 25
  *(pgm +  66) = ITM_XEQ;
  *(pgm +  67) = 9;

  // 26
  *(pgm +  68) = ITM_LITT;
  *(pgm +  69) = 5; // String to real34
  *(pgm +  70) = 1; // String length
  *(pgm +  71) = '4'; // THE string

  // 27
  *(pgm +  72) = ITM_XEQ;
  *(pgm +  73) = 9;

  // 28
  *(pgm +  74) = ITM_LITT;
  *(pgm +  75) = 5; // String to real34
  *(pgm +  76) = 1; // String length
  *(pgm +  77) = '6'; // THE string

  // 29
  *(pgm +  78) = ITM_XEQ;
  *(pgm +  79) = 9;

  // 30
  *(pgm +  80) = ITM_LITT;
  *(pgm +  81) = 5; // String to real34
  *(pgm +  82) = 1; // String length
  *(pgm +  83) = '2'; // THE string

  // 31
  *(pgm +  84) = ITM_XEQ;
  *(pgm +  85) = 9;

  // 32
  *(pgm +  86) = ITM_LITT;
  *(pgm +  87) = 5; // String to real34
  *(pgm +  88) = 1; // String length
  *(pgm +  89) = '6'; // THE string

  // 33
  *(pgm +  90) = ITM_XEQ;
  *(pgm +  91) = 9;

  // 34
  *(pgm +  92) = ITM_LITT;
  *(pgm +  93) = 5; // String to real34
  *(pgm +  94) = 1; // String length
  *(pgm +  95) = '4'; // THE string

  // 35
  *(pgm +  96) = ITM_XEQ;
  *(pgm +  97) = 9;

  // 36
  *(pgm +  98) = ITM_LITT;
  *(pgm +  99) = 5; // String to real34
  *(pgm + 100) = 1; // String length
  *(pgm + 101) = '2'; // THE string

  // 37
  *(pgm + 102) = ITM_XEQ;
  *(pgm + 103) = 9;

  // 38
  *(pgm + 104) = ITM_LITT;
  *(pgm + 105) = 5; // String to real34
  *(pgm + 106) = 1; // String length
  *(pgm + 107) = '4'; // THE string

  // 39
  *(pgm + 108) = ITM_XEQ;
  *(pgm + 109) = 9;

  // 40
  *(pgm + 110) = ITM_LITT;
  *(pgm + 111) = 5; // String to real34
  *(pgm + 112) = 1; // String length
  *(pgm + 113) = '6'; // THE string

  // 41
  *(pgm + 114) = ITM_XEQ;
  *(pgm + 115) = 9;

  // 42
  *(pgm + 116) = ITM_LITT;
  *(pgm + 117) = 5; // String to real34
  *(pgm + 118) = 1; // String length
  *(pgm + 119) = '6'; // THE string

  // 43
  *(pgm + 120) = ITM_XEQ;
  *(pgm + 121) = 9;

  // 44
  *(pgm + 122) = ITM_LITT;
  *(pgm + 123) = 5; // String to real34
  *(pgm + 124) = 1; // String length
  *(pgm + 125) = '2'; // THE string

  // 45
  *(pgm + 126) = ITM_XEQ;
  *(pgm + 127) = 9;

  // 46
  *(pgm + 128) = ITM_LITT;
  *(pgm + 129) = 5; // String to real34
  *(pgm + 130) = 1; // String length
  *(pgm + 131) = '6'; // THE string

  // 47
  *(pgm + 132) = ITM_XEQ;
  *(pgm + 133) = 9;

  // 48
  *(pgm + 134) = ITM_LITT;
  *(pgm + 135) = 5; // String to real34
  *(pgm + 136) = 1; // String length
  *(pgm + 137) = '4'; // THE string

  // 49
  *(pgm + 138) = ITM_XEQ;
  *(pgm + 139) = 9;

  // 50
  *(pgm + 140) = ITM_LITT;
  *(pgm + 141) = 5; // String to real34
  *(pgm + 142) = 1; // String length
  *(pgm + 143) = '2'; // THE string

  // 51
  *(pgm + 144) = ITM_XEQ;
  *(pgm + 145) = 9;

  // 52
  *(pgm + 146) = ITM_LITT;
  *(pgm + 147) = 5; // String to real34
  *(pgm + 148) = 1; // String length
  *(pgm + 149) = '6'; // THE string

  // 53
  *(pgm + 150) = ITM_XEQ;
  *(pgm + 151) = 9;

  // 54
  *(pgm + 152) = ITM_LITT;
  *(pgm + 153) = 5; // String to real34
  *(pgm + 154) = 1; // String length
  *(pgm + 155) = '4'; // THE string

  // 55
  *(pgm + 156) = ITM_XEQ;
  *(pgm + 157) = 9;

  // 56
  *(pgm + 158) = ITM_LITT;
  *(pgm + 159) = 5; // String to real34
  *(pgm + 160) = 1; // String length
  *(pgm + 161) = '6'; // THE string

  // 57
  *(pgm + 162) = ITM_XEQ;
  *(pgm + 163) = 9;

  // 58
  *(pgm + 164) = ITM_LITT;
  *(pgm + 165) = 5; // String to real34
  *(pgm + 166) = 1; // String length
  *(pgm + 167) = '8'; // THE string

  // 59
  *(pgm + 168) = ITM_XEQ;
  *(pgm + 169) = 9;

  // 60
  *(pgm + 170) = ITM_LITT;
  *(pgm + 171) = 5; // String to real34
  *(pgm + 172) = 1; // String length
  *(pgm + 173) = '4'; // THE string

  // 61
  *(pgm + 174) = ITM_XEQ;
  *(pgm + 175) = 9;

  // 62
  *(pgm + 176) = ITM_LITT;
  *(pgm + 177) = 5; // String to real34
  *(pgm + 178) = 1; // String length
  *(pgm + 179) = '2'; // THE string

  // 63
  *(pgm + 180) = ITM_XEQ;
  *(pgm + 181) = 9;

  // 64
  *(pgm + 182) = ITM_LITT;
  *(pgm + 183) = 5; // String to real34
  *(pgm + 184) = 1; // String length
  *(pgm + 185) = '4'; // THE string

  // 65
  *(pgm + 186) = ITM_XEQ;
  *(pgm + 187) = 9;

  // 66
  *(pgm + 188) = ITM_LITT;
  *(pgm + 189) = 5; // String to real34
  *(pgm + 190) = 1; // String length
  *(pgm + 191) = '2'; // THE string

  // 67
  *(pgm + 192) = ITM_XEQ;
  *(pgm + 193) = 9;

  // 68
  *(pgm + 194) = ITM_LITT;
  *(pgm + 195) = 5; // String to real34
  *(pgm + 196) = 1; // String length
  *(pgm + 197) = '4'; // THE string

  // 69
  *(pgm + 198) = ITM_XEQ;
  *(pgm + 199) = 9;

  // 70
  *(pgm + 200) = ITM_LITT;
  *(pgm + 201) = 5; // String to real34
  *(pgm + 202) = 1; // String length
  *(pgm + 203) = '8'; // THE string

  // 71
  *(pgm + 204) = ITM_XEQ;
  *(pgm + 205) = 9;

  // 72
  *(pgm + 206) = ITM_LITT;
  *(pgm + 207) = 5; // String to real34
  *(pgm + 208) = 1; // String length
  *(pgm + 209) = '6'; // THE string

  // 73
  *(pgm + 210) = ITM_XEQ;
  *(pgm + 211) = 9;

  // 74
  *(pgm + 212) = ITM_LITT;
  *(pgm + 213) = 5; // String to real34
  *(pgm + 214) = 1; // String length
  *(pgm + 215) = '4'; // THE string

  // 75
  *(pgm + 216) = ITM_XEQ;
  *(pgm + 217) = 9;

  // 76
  *(pgm + 218) = ITM_LITT;
  *(pgm + 219) = 5; // String to real34
  *(pgm + 220) = 1; // String length
  *(pgm + 221) = '6'; // THE string

  // 77
  *(pgm + 222) = ITM_XEQ;
  *(pgm + 223) = 9;

  // 78
  *(pgm + 224) = ITM_LITT;
  *(pgm + 225) = 5; // String to real34
  *(pgm + 226) = 1; // String length
  *(pgm + 227) = '2'; // THE string

  // 79
  *(pgm + 228) = ITM_XEQ;
  *(pgm + 229) = 9;

  // 80
  *(pgm + 230) = ITM_LITT;
  *(pgm + 231) = 5; // String to real34
  *(pgm + 232) = 1; // String length
  *(pgm + 233) = '4'; // THE string

  // 81
  *(pgm + 234) = ITM_XEQ;
  *(pgm + 235) = 9;

  // 82
  *(pgm + 236) = ITM_LITT;
  *(pgm + 237) = 5; // String to real34
  *(pgm + 238) = 1; // String length
  *(pgm + 239) = '6'; // THE string

  // 83
  *(pgm + 240) = ITM_XEQ;
  *(pgm + 241) = 9;

  // 84
  *(pgm + 242) = ITM_LITT;
  *(pgm + 243) = 5; // String to real34
  *(pgm + 244) = 1; // String length
  *(pgm + 245) = '2'; // THE string

  // 85
  *(pgm + 246) = ITM_XEQ;
  *(pgm + 247) = 9;

  // 86
  *(pgm + 248) = ITM_LITT;
  *(pgm + 249) = 5; // String to real34
  *(pgm + 250) = 1; // String length
  *(pgm + 251) = '6'; // THE string

  // 87
  *(pgm + 252) = ITM_XEQ;
  *(pgm + 253) = 9;

  // 88
  *(pgm + 254) = ITM_LITT;
  *(pgm + 255) = 5; // String to real34
  *(pgm + 256) = 1; // String length
  *(pgm + 257) = '6'; // THE string

  // 89
  *(pgm + 258) = ITM_XEQ;
  *(pgm + 259) = 9;

  // 90
  *(pgm + 260) = ITM_LITT;
  *(pgm + 261) = 5; // String to real34
  *(pgm + 262) = 1; // String length
  *(pgm + 263) = '4'; // THE string

  // 91
  *(pgm + 264) = ITM_XEQ;
  *(pgm + 265) = 9;

  // 92
  *(pgm + 266) = ITM_LITT;
  *(pgm + 267) = 5; // String to real34
  *(pgm + 268) = 1; // String length
  *(pgm + 269) = '2'; // THE string

  // 93
  *(pgm + 270) = ITM_XEQ;
  *(pgm + 271) = 9;

  // 94
  *(pgm + 272) = ITM_LITT;
  *(pgm + 273) = 5; // String to real34
  *(pgm + 274) = 1; // String length
  *(pgm + 275) = '4'; // THE string

  // 94
  *(pgm + 276) = ITM_XEQ;
  *(pgm + 277) = 9;

  // 96
  *(pgm + 278) = ITM_LITT;
  *(pgm + 279) = 5; // String to real34
  *(pgm + 280) = 1; // String length
  *(pgm + 281) = '6'; // THE string

  // 97
  *(pgm + 282) = ITM_XEQ;
  *(pgm + 283) = 9;

  // 98
  *(pgm + 284) = ITM_LITT;
  *(pgm + 285) = 5; // String to real34
  *(pgm + 286) = 1; // String length
  *(pgm + 287) = '2'; // THE string

  // 99
  *(pgm + 288) = ITM_XEQ;
  *(pgm + 289) = 9;

  // 100
  *(pgm + 290) = ITM_LITT;
  *(pgm + 291) = 5; // String to real34
  *(pgm + 292) = 1; // String length
  *(pgm + 293) = '6'; // THE string

  // 101
  *(pgm + 294) = ITM_XEQ;
  *(pgm + 295) = 9;

  // 102
  *(pgm + 296) = ITM_LITT;
  *(pgm + 297) = 5; // String to real34
  *(pgm + 298) = 1; // String length
  *(pgm + 299) = '4'; // THE string

  // 103
  *(pgm + 300) = ITM_XEQ;
  *(pgm + 301) = 9;

  // 104
  *(pgm + 302) = ITM_LITT;
  *(pgm + 303) = 5; // String to real34
  *(pgm + 304) = 1; // String length
  *(pgm + 305) = '2'; // THE string

  // 105
  *(pgm + 306) = ITM_XEQ;
  *(pgm + 307) = 9;

  // 106
  *(pgm + 308) = ITM_LITT;
  *(pgm + 309) = 5; // String to real34
  *(pgm + 310) = 1; // String length
  *(pgm + 311) = '4'; // THE string

  // 107
  *(pgm + 312) = ITM_XEQ;
  *(pgm + 313) = 9;

  // 108
  *(pgm + 314) = ITM_LITT;
  *(pgm + 315) = 5; // String to real34
  *(pgm + 316) = 1; // String length
  *(pgm + 317) = '2'; // THE string

  // 109
  *(pgm + 318) = ITM_XEQ;
  *(pgm + 319) = 9;

  // 110
  *(pgm + 320) = ITM_LITT;
  *(pgm + 321) = 5; // String to real34
  *(pgm + 322) = 2; // String length
  *(pgm + 323) = '1'; // THE string
  *(pgm + 324) = '0'; // THE string

  // 111
  *(pgm + 325) = ITM_XEQ;
  *(pgm + 326) = 9;

  // 112
  *(pgm + 327) = ITM_LITT;
  *(pgm + 328) = 5; // String to real34
  *(pgm + 329) = 1; // String length
  *(pgm + 330) = '2'; // THE string

  // 113
  *(pgm + 331) = ITM_XEQ;
  *(pgm + 332) = 9;

  // 114
  *(pgm + 333) = ITM_LITT;
  *(pgm + 334) = 5; // String to real34
  *(pgm + 335) = 2; // String length
  *(pgm + 336) = '1'; // THE string
  *(pgm + 337) = '0'; // THE string

  // 115
  *(pgm + 338) = ITM_XEQ;
  *(pgm + 339) = 9;

  // 116
  *(pgm + 340) = ITM_RCL;
  *(pgm + 341) = 2;

  // 117
  *(pgm + 342) = ITM_RCL;
  *(pgm + 343) = 1;

  // 118
  *(pgm + 344) = ITM_XLT;
  *(pgm + 345) = 101; // Y

  // 119
  *(pgm + 346) = ITM_GTO;
  *(pgm + 347) = 2;

  // 120
  *(pgm + 348) = ITM_RCL;
  *(pgm + 349) = 0;

  // 121
  *(pgm + 350) = ITM_RCL;
  *(pgm + 351) = 0;

  // 122
  *(pgm + 352) = (ITM_STOP >> 8) | 0x80;
  *(pgm + 353) =  ITM_STOP       & 0xff;

  // 123
  *(pgm + 354) = ITM_LBL;
  *(pgm + 355) = 9;

  // 124
  *(pgm + 356) = ITM_STOPLUS;
  *(pgm + 357) = 1;

  // 125
  *(pgm + 358) = ITM_RCL;
  *(pgm + 359) = 0;

  // 126
  *(pgm + 360) = ITM_RCL;
  *(pgm + 361) = 1;

  // 127
  *(pgm + 362) = ITM_MOD;

  // 128
  *(pgm + 363) = ITM_XNE;
  *(pgm + 364) = 250; // 0.

  // 129
  *(pgm + 365) = ITM_RTN;

  // 130
  *(pgm + 366) = ITM_RCL;
  *(pgm + 367) = 0;

  // 131
  *(pgm + 368) = ITM_RCL;
  *(pgm + 369) = 1;

  // 132
  *(pgm + 370) = (ITM_END >> 8) | 0x80;
  *(pgm + 371) =  ITM_END       & 0xff;




  // 1
  *(pgm + 372) = ITM_LBL;
  *(pgm + 373) = 255;
  *(pgm + 374) = (7 << 4) | 7; // 7 glyphs and 7 bytes
  *(pgm + 375) = 'B';
  *(pgm + 376) = 'a';
  *(pgm + 377) = 'i';
  *(pgm + 378) = 'r';
  *(pgm + 379) = 's';
  *(pgm + 380) = 't';
  *(pgm + 381) = 'o';

  // 2
  *(pgm + 382) = (ITM_ALL >> 8) | 0x80;
  *(pgm + 383) =  ITM_ALL       & 0xff;
  *(pgm + 384) = 0;

  // 3
  *(pgm + 385) = (ITM_CLREGS >> 8) | 0x80;
  *(pgm + 386) =  ITM_CLREGS       & 0xff;

  // 4
  *(pgm + 387) = (ITM_CLSTK >> 8) | 0x80;
  *(pgm + 388) =  ITM_CLSTK       & 0xff;

  // 5
  *(pgm + 389) = ITM_LITT;
  *(pgm + 390) = 8; // String
  *(pgm + 391) = 19; // String length
  *(pgm + 392) = 'P'; // THE string
  *(pgm + 393) = 'o';
  *(pgm + 394) = 'l';
  *(pgm + 395) = 'y';
  *(pgm + 396) = 'n';
  *(pgm + 397) = 'o';
  *(pgm + 398) = 'm';
  *(pgm + 399) = 'i';
  *(pgm + 400) = 'a';
  *(pgm + 401) = 'l';
  *(pgm + 402) = ' ';
  *(pgm + 403) = 'd';
  *(pgm + 404) = 'e';
  *(pgm + 405) = 'g';
  *(pgm + 406) = 'r';
  *(pgm + 407) = 'e';
  *(pgm + 408) = 'e';
  *(pgm + 409) = ' ';
  *(pgm + 410) = '?';

  // 6
  *(pgm + 411) = (ITM_STOP >> 8) | 0x80;
  *(pgm + 412) =  ITM_STOP       & 0xff;

  // 7
  *(pgm + 413) = ITM_MAGNITUDE;

  // 8
  *(pgm + 414) = ITM_IP;

  // 9
  *(pgm + 415) = ITM_STO;
  *(pgm + 416) = 90;

  // 10
  *(pgm + 417) = ITM_STO;
  *(pgm + 418) = 91;

  // 11
  *(pgm + 419) = (ITM_CLSTK >> 8) | 0x80;
  *(pgm + 420) =  ITM_CLSTK       & 0xff;

  // 12
  *(pgm + 421) = ITM_RCL;
  *(pgm + 422) = 91;

  // 13
  *(pgm + 423) = ITM_LITT;
  *(pgm + 424) = 5; // String to real34
  *(pgm + 425) = 3; // String length
  *(pgm + 426) = '1'; // THE string
  *(pgm + 427) = 'e';
  *(pgm + 428) = '3';

  // 14
  *(pgm + 429) = ITM_DIV;

  // 15
  *(pgm + 430) = ITM_STO;
  *(pgm + 431) = 92;

  // 16
  *(pgm + 432) = ITM_LBL;
  *(pgm + 433) = 100; // A

  // 17
  *(pgm + 434) = ITM_LITT;
  *(pgm + 435) = 8; // String
  *(pgm + 436) = 2; // String length
  *(pgm + 437) = 'x'; // THE string
  *(pgm + 438) = '^';

  // 18
  *(pgm + 439) = ITM_RCL;
  *(pgm + 440) = 91;

  // 19
  *(pgm + 441) = ITM_ADD;






  *(pgm + 442) = 127 | 0x80; // .END.
  *(pgm + 443) = 255;        // .END.


  uint16_t step = 0;
  stepAddress = pgm;
  while(stepAddress) {
    printf("%4u  ", ++step);
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
