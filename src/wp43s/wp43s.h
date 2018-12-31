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
 * \file wp43s.h
 ***********************************************/



#define VERSION   "Pre-alpha" STD_SPACE_3_PER_EM "version" STD_SPACE_3_PER_EM "2018.12.30"
#define COPYRIGHT "The WP43S team"
#define WHO       "WP" STD_SPACE_3_PER_EM "43S" STD_SPACE_3_PER_EM "v0.1" STD_SPACE_3_PER_EM "2018.12" STD_SPACE_3_PER_EM "by" STD_SPACE_3_PER_EM "Pauli," STD_SPACE_3_PER_EM "Walter" STD_SPACE_3_PER_EM "&" STD_SPACE_3_PER_EM "Martin"

#ifndef wp43s_H_INCLUDED
#define wp43s_H_INCLUDED

#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-result"
#ifdef PC_BUILD
  #pragma GCC diagnostic ignored "-Wstringop-truncation"
#endif

#define EXTRA_INFO_ON_CALC_ERROR 1
#define DEBUG_PANEL              1
#define DEBUG_REGISTER_L         1
#define STACK_LIFT_DEBUG         1
#define LOG_FUNCTIONS            0
#define IBM_DECIMAL              1
#define TOMS_FAST_MATH           1

#if !defined(PC_BUILD) && !defined(DMCP_BUILD)
  #error One of PC_BUILD and DMCP_BUILD must be defined
#endif

#if defined(PC_BUILD) && defined(DMCP_BUILD)
  #error Only one of PC_BUILD and DMCP_BUILD must be defined
#endif

#ifdef PC_BUILD
  #ifdef __MINGW64__ // No DEBUG_PANEL mode for Windows
    #undef  DEBUG_PANEL
    #define DEBUG_PANEL 0
  #endif
#endif

#ifdef DMCP_BUILD
  #undef  DEBUG_PANEL
  #define DEBUG_PANEL 0
  #undef  EXTRA_INFO_ON_CALC_ERROR
  #define EXTRA_INFO_ON_CALC_ERROR 0
  #undef  LOG_FUNCTIONS
  #define LOG_FUNCTIONS 0
#endif



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <math.h>

#ifdef PC_BUILD
  #include <glib.h>
  #include <gtk/gtk.h>
  #include <gdk/gdk.h>
#endif

#ifdef DMCP_BUILD
  #include <main.h>
  #include <dmcp.h>
#endif

/********************************************//**
 * \typedef bool_t
 * \brief Boolean type
 ***********************************************/
typedef enum {
  false = 0,     ///< Value for false
  true  = !false ///< Value for true
} bool_t; // 1 bit


typedef int16_t calcRegister_t;

#if (IBM_DECIMAL == 1)
 #define DECNUMDIGITS 51
 #include "decimal128.h"
 #include "decimal64.h"
 #include "decDouble.h"
 #include "decQuad.h"
 #include "decNumberWrappers.h"
#endif

#define MAX_BIG_INTEGER_SIZE_IN_BITS 4096

#if (TOMS_FAST_MATH == 1)
 #define FP_MAX_SIZE (MAX_BIG_INTEGER_SIZE_IN_BITS+(8*DIGIT_BIT))

 #include "tfm.h"
 #include "tomsFastMathWrappers.h"
#endif

#if defined(__MINGW64__)
 #include <locale.h>
#endif


#include "addition.h"
#include "assign.h"
#include "bufferize.h"
#include "complex.h"
#include "config.h"
#include "conversions.h"
#include "curveFitting.h"
#include "dateTime.h"
#include "division.h"
#include "constants.h"
#include "debug.h"
#include "error.h"
#include "flagBrowser.h"
#include "flags.h"
#include "fontBrowser.h"
#include "fractions.h"
#include "fonts.h"
#include "charString.h" // must be after font.h
#include "display.h"    // must be after font.h
#include "gui.h"
#include "integers.h"
#include "items.h"
#include "keyboard.h"
#include "mathematics.h"
#include "multiplication.h"
#include "power.h"
#include "registerBrowser.h"
#include "registers.h"
#include "registerValueConversions.h"
#include "screen.h"
#include "subtraction.h"
#include "timer.h"
#include "softmenus.h"
#include "sort.h"
#include "stack.h"
#include "stats.h"
#include "statusBar.h"
#include "testFunctions.h"

#define min(a,b)                ((a)<(b)?(a):(b))
#define max(a,b)                ((a)>(b)?(a):(b))
#define modulo(a, b)            (((a)%(b))<0 ? ((a)%(b))+(b) : ((a)%(b))) // the % operator is remainder rather than modulo!

// On/Off 1 bit
#define OFF                     0
#define ON                      1

// Denominator mode 2 bits
#define DM_FIX                  0
#define DM_FAC                  1
#define DM_ANY                  2
#define DM_DENMAX            9999

// Small integer mode 2 bits
#define SIM_UNSIGN              0
#define SIM_1COMPL              1
#define SIM_2COMPL              2
#define SIM_SIGNMT              3

#define SMALL_INTEGER_SIZE      8 // 8 bytes = 64 bits

// Display format 2 bits
#define DF_ALL                  0
#define DF_FIX                  1
#define DF_SCI                  2
#define DF_ENG                  3

// Display override 1 bit
#define DO_SCI                  0
#define DO_ENG                  1

// Angular mode 3 bits
#define AM_DEGREE               0
#define AM_GRAD                 1
#define AM_RADIAN               2
#define AM_MULTPI               3
#define AM_DMS                  4

// Time format 1 bit
#define TF_H24                  0
#define TF_H12                  1

// Date format 2 bits
#define DF_DMY                  0
#define DF_YMD                  1
#define DF_MDY                  2

// Curve fitting 3 bits
#define CF_LINEAR_FITTING       0
#define CF_EXPONENTIAL_FITTING  1
#define CF_LOGARITHMIC_FITTING  2
#define CF_POWER_FITTING        3
#define CF_BEST_FITTING         4

// Rounding mode 3 bits
#define RM_HALF_EVEN            0
#define RM_HALF_UP              1
#define RM_HALF_DOWN            2
#define RM_UP                   3
#define RM_DOWN                 4
#define RM_CEIL                 5
#define RM_FLOOR                6

// Calc mode 4 bits
#define CM_NORMAL               0 // Normal operation
#define CM_AIM                  1 // Alpha input mode
#define CM_TAM                  2 // Temporary input mode
#define CM_NIM                  3 // Numeric input mode
#define CM_ASSIGN               4 // Assign mode
#define CM_REGISTER_BROWSER     5 // Register browser
#define CM_FLAG_BROWSER         6 // Flag browser
#define CM_FONT_BROWSER         7 // Font browser
#define CM_ERROR_MESSAGE        8 // Error message in one of the register lines
#define CM_BUG_ON_SCREEN        9 // Bug message on screen
#define CM_CONFIRMATION        10 // Waiting for confirmation or canceling

// Next character 2 bits
#define NC_NORMAL               0
#define NC_SUBSCRIPT            1
#define NC_SUPERSCRIPT          2

// Complex unit 1 bit
#define CU_I                    0
#define CU_J                    1
#define COMPLEX_UNIT            (complexUnit == CU_I ? NUM_i : NUM_j)

// Product sign 1 bit
#define PS_DOT                  0
#define PS_CROSS                1
#define PRODUCT_SIGN            (productSign == PS_CROSS ? NUM_CROSS : NUM_DOT)

// Fraction type 1 bit
#define FT_PROPER               0 // a b/c
#define FT_IMPROPER             1 // d/c

// Radix Mark 1 bit
#define RM_PERIOD               0
#define RM_COMMA                1
#define RADIX16_MARK_CHAR       (radixMark == RM_PERIOD ? '.' : ',')
#define RADIX16_MARK_STRING     (radixMark == RM_PERIOD ? "." : ",")
#define RADIX34_MARK_STRING     (radixMark == RM_PERIOD ? NUM_PERIOD34 : NUM_COMMA34)

// Stack size 1 bit
#define SS_4                    0
#define SS_8                    1

// Complex mode 1 bit
#define CM_RECTANGULAR          0
#define CM_POLAR                1

// Alpha case 1 bit
#define AC_UPPER                0
#define AC_LOWER                1

// Cursor font 1 bit
#define CF_NUMERIC              0
#define CF_STANDARD             1

// TAM mode 2 bits
#define TM_NORMAL               0
#define TM_CMP                  1
#define TM_STO_RCL              2
#define TM_NORMAL_10000     10000
#define TM_CMP_10000        10001
#define TM_STO_RCL_10000    10002

// NIM number part
#define NP_EMPTY                0
#define NP_INT_10               1
#define NP_INT_16               2
#define NP_INT_BASE             3
#define NP_REAL_FLOAT_PART      4
#define NP_REAL_EXPONENT        5
#define NP_FRACTION_DENOMINATOR 6
#define NP_COMPLEX_INT_PART     7
#define NP_COMPLEX_FLOAT_PART   8
#define NP_COMPLEX_EXPONENT     9

// Temporary information
#define TI_NO_INFO              0
#define TI_RESET                1
#define TI_RADIUS_THETA         2
#define TI_ANGLE                3
#define TI_STATISTIC_SUMS       4
#define TI_ARE_YOU_SURE         5
#define TI_VERSION              6
#define TI_WHO                  7

// Register browser mode
#define RBR_GLOBAL              0
#define RBR_LOCAL               1
#define RBR_NAMED               2

// Debug window
#define DBG_BIT_FIELDS                 0
#define DBG_FLAGS                      1
#define DBG_REGISTERS                  2
#define DBG_LOCAL_REGISTERS            3
#define DBG_STATISTICAL_SUMS           4
#define DBG_NAMED_REGISTERS            5
#define DBG_TMP_SAVED_STACK_REGISTERS  6

#if (__linux__ == 1)
 #define FMT64U "lu"
 #define FMT64S "ld"
 #define FMT32U "u"
 #define FMT32S "d"
 #define FMT16U "u"
 #define FMT16S "d"
 #define FMT8U  "u"
 #define FMT8S  "d"
 #define FMTPTR "lu"
#elif defined(__arm__)
 #define FMT64U "llu"
 #define FMT64S "lld"
 #define FMT32U "lu"
 #define FMT32S "ld"
 #define FMT16U "u"
 #define FMT16S "d"
 #define FMT8U  "u"
 #define FMT8S  "d"
 #define FMTPTR "d"
#elif defined(__MINGW64__)
 #define FMT64U "I64u"
 #define FMT64S "I64d"
 #define FMT32U "u"
 #define FMT32S "d"
 #define FMT16U "u"
 #define FMT16S "d"
 #define FMT8U  "u"
 #define FMT8S  "d"
 #define FMTPTR "lu"
#else
 #error Only Linux, ARM and Windows MINGW64 are supported for now
#endif

//#define tamMode                    ((*bits5 >> OFFSET_TAMMODE        ) & ((1 << LENGTH_TAMMODE        ) - 1)) // TAM mode
//#define setTamMode(x)              *bits5 = (*bits5 & ~(((1 << LENGTH_TAMMODE        ) - 1) << OFFSET_TAMMODE        )) | ((x) << OFFSET_TAMMODE        )


// Variables for the simulator
#ifdef PC_BUILD
  extern bool_t               calcLandscape, calcAutoLandscapePortrait;
  extern GtkWidget            *screen, *frmCalc;
  extern int16_t              screenStride, debugWindow;
  extern uint32_t             *screenData;
  extern bool_t               screenChange;
  #if (DEBUG_REGISTER_L == 1)
    extern                    GtkWidget *lblRegisterL;
  #endif
#endif

#define RAM_SIZE              65536 //98304 // 96kb
extern char                   *ram;
extern bool_t                 allowScreenUpdate, funcOK;

// Variables stored in FLASH
extern const item_t           indexOfItems[];
extern const char             *errorMessages[NUMBER_OF_ERROR_CODES];
extern const calcKey_t        kbd_std[37];
extern const font_t           standardFont, numericFont;
extern void                   (* const addition[12][12])(void);
extern void                   (* const subtraction[12][12])(void);
extern void                   (* const multiplication[12][12])(void);
extern void                   (* const division[12][12])(void);
extern const softmenu_t       softmenu[];
extern const int16_t          softkeyRow[];

// Variables stored in RAM
extern decContext             ctxtReal16, ctxtReal34, ctxtReal51;
extern uint16_t               flags[7];
#define TMP_STR_LENGTH 3000
#define ERROR_MESSAGE_LENGTH 512
extern char                   tmpStr3000[TMP_STR_LENGTH], errorMessage[ERROR_MESSAGE_LENGTH], aimBuffer[AIM_BUFFER_LENGTH], nimBuffer[NIM_BUFFER_LENGTH], nimBufferDisplay[NIM_BUFFER_LENGTH], tamBuffer[TAM_BUFFER_LENGTH], oldTime[8], dateTimeString[12];
extern softmenuStack_t        softmenuStack[7];
extern uint32_t               reg[112], savedStackRegister[8], tempRegister[NUMBER_OF_TEMPORARY_REGISTERS];
extern int16_t                tamFunction, tamNumber, tamNumberMin, tamNumberMax, tamDigit, tamOperation, tamLetteredRegister, tamCurrentOperation;
extern int16_t                currentRegisterBrowserScreen;
extern calcRegister_t         result, op1, op2;
extern uint16_t               numberOfLocalRegisters, numberOfNamedRegisters;
extern uint32_t               allLocalRegisterPointer, allNamedRegisterPointer, statisticalSumsPointer, firstFreeByte, lastFreeByte;
extern uint16_t               programCounter, xCursor, yCursor;
extern uint32_t               firstGregorianDay, denMax;
extern uint8_t                softmenuStackPointer, transitionSystemStatus, cursorBlinkCounter, numScreensStandardFont;
extern uint8_t                currentFntScr, currentFlgScr, displayFormat, displayFormatDigits;
extern uint8_t                smallIntegerWordSize, denominatorMode, significantDigits, smallIntegerMode;
extern uint8_t                angularMode, groupingGap, dateFormat, curveFitting;
extern uint8_t                roundingMode, calcMode, nextChar, complexUnit;
extern uint8_t                displayStack, productSign, fractionType, radixMark;
extern uint8_t                displayModeOverride, stackSize, complexMode, alphaCase;
extern uint8_t                numLinesNumericFont, numLinesStandardFont, cursorEnabled, cursorFont;
extern uint8_t                nimNumberPart, hexDigits, lastErrorCode, serialIOIconEnabled;
extern uint8_t                timeFormat, tamMode, temporaryInformation, rbrMode;
extern uint8_t                numScreensNumericFont, previousCalcMode;
extern bool_t                 hourGlassIconEnabled, watchIconEnabled, userModeEnabled; //, displayDateTime;
extern bool_t                 printerIconEnabled, batteryIconEnabled, shiftF, shiftG;
extern bool_t                 showContent, stackLiftEnabled, displayLeadingZeros, displayRealAsFraction;
extern bool_t                 savedStackLiftEnabled, tempRegistersInUse[NUMBER_OF_TEMPORARY_REGISTERS];
extern calcKey_t              kbd_usr[37];
extern calcRegister_t         errorMessageRegisterLine, errorRegisterLine;
extern uint16_t               row[100];
extern uint64_t               smallIntegerMask, smallIntegerSignBit;
extern glyph_t                glyphNotFound;
extern char                   transitionSystemOperation[4];
extern int16_t                exponentSignLocation, imaginaryExponentSignLocation, imaginaryMantissaSignLocation;
extern void                   (*confirmedFunction)(uint16_t);
#ifdef DMCP_BUILD
  extern bool_t               endOfProgram;
#endif // DMCP_BUILD

#include "constantPointers.h"
#endif // wp43s_H_INCLUDED
