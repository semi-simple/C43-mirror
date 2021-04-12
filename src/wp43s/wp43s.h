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

#ifndef wp43s_H_INCLUDED
  #define wp43s_H_INCLUDED

  #include "defines.h"

  #pragma GCC diagnostic ignored "-Wunused-parameter"


  #include <stdlib.h>
  #include <stdio.h>
  #include <string.h>
  #include <time.h>
  #include <stdint.h>
  #if (__linux__ == 1)
    #include <math.h>
  #endif // (__linux__ == 1)

  #ifdef __APPLE__
    // needed by chdir
    #include <unistd.h>
  #endif // __APPLE__

  #ifdef PC_BUILD
    #include <glib.h>
    #include <gtk/gtk.h>
    #include <gdk/gdk.h>
  #endif // PC_BUILD

  #ifdef __MINGW64__
    #include <locale.h>
  #endif // __MINGW64__

  #ifdef DMCP_BUILD
    #include <main.h>
    #include <dmcp.h>
    #define TMP_STR_LENGTH       AUX_BUF_SIZE
  #else // !DMCP_BUILD
      #define TMP_STR_LENGTH     2560 //JMMAX ORG:2560
  #endif // DMCP_BUILD
  #define WRITE_BUFFER_LEN       4096
  #define ERROR_MESSAGE_LENGTH    512 //JMMAX(325) 512          //JMMAX Temporarily reduced - ORG:512.
  #define DISPLAY_VALUE_LEN        80

  #include "typeDefinitions.h"

  #if (IBM_DECIMAL == 1)
    #include "decimal128.h"
    #include "decimal64.h"
    #include "decDouble.h"
    #include "decQuad.h"
    #include "decNumberWrappers.h"
    extern int decGetInt(const decNumber *x); // Because decNumberToInt32 seems buggy! Needs more investigation
  #endif // (IBM_DECIMAL == 1)

  #if (LIBGMP == 1)
    #include <gmp.h>
    #include "gmpWrappers.h"
  #endif // (LIBGMP == 1)

  #ifndef RASPBERRY
    #undef SCREEN_800X480
    #define SCREEN_800x480 0
  #endif // RASPBERRY

  #if !defined(PC_BUILD) && !defined(DMCP_BUILD)
    #error One of PC_BUILD and DMCP_BUILD must be defined
  #endif // !defined(PC_BUILD) && !defined(DMCP_BUILD)

  #if defined(PC_BUILD) && defined(DMCP_BUILD)
    #error Only one of PC_BUILD and DMCP_BUILD must be defined
  #endif // defined(PC_BUILD) && defined(DMCP_BUILD)

  #if !defined(OS32BIT) && !defined(OS64BIT)
    #error One of OS32BIT and OS64BIT must be defined
  #endif // !defined(OS32BIT) && !defined(OS64BIT)

  #if defined(OS32BIT) && defined(OS64BIT)
    #error Only one of OS32BIT and OS64BIT must be defined
  #endif // defined(OS32BIT) && defined(OS64BIT)

  #ifdef PC_BUILD
    #ifdef __MINGW64__ // No DEBUG_PANEL mode for Windows
      #undef  DEBUG_PANEL
      #define DEBUG_PANEL 0
    #endif // __MINGW64__
    #ifdef RASPBERRY // No DEBUG_PANEL mode for Raspberry Pi
      #undef  DEBUG_PANEL
      #define DEBUG_PANEL 0
    #endif // RASPBERRY
  #endif // PC_BUILD

  #if defined(DMCP_BUILD) || (SCREEN_800X480 == 1)
    #undef  DEBUG_PANEL
    #define DEBUG_PANEL 0
    #undef  DEBUG_REGISTER_L
    #define DEBUG_REGISTER_L 0
    #undef  SHOW_MEMORY_STATUS
    #define SHOW_MEMORY_STATUS 0
    #undef  EXTRA_INFO_ON_CALC_ERROR
    #define EXTRA_INFO_ON_CALC_ERROR 0
  #ifndef SWAP_TO_L1_ON_DM42
    #undef  JM_LAYOUT_1A
    #define JM_LAYOUT_2_DM42_STRICT    //DM42 compatible layout
  #else
    #define JM_LAYOUT_1A
    #undef  JM_LAYOUT_2_DM42_STRICT    //DM42 compatible layout
  #endif
 #endif // defined(DMCP_BUILD) || (SCREEN_800X480 == 1)

#if defined(TESTSUITE_BUILD) && !defined(GENERATE_CATALOGS)
  #undef  JM_LAYOUT_1A
  #undef  JM_LAYOUT_2_DM42_STRICT    //DM42 compatible layout

    #undef  PC_BUILD
    #undef  DMCP_BUILD
    #undef  DEBUG_PANEL
    #define DEBUG_PANEL 0
    #undef  DEBUG_REGISTER_L
    #define DEBUG_REGISTER_L 0
    #undef  SHOW_MEMORY_STATUS
    #define SHOW_MEMORY_STATUS 0
    #undef  EXTRA_INFO_ON_CALC_ERROR
    #define EXTRA_INFO_ON_CALC_ERROR 0
    #define addItemToBuffer fnNop
    #define fnOff           fnNop
    #define fnAim           fnNop
    #define registerBrowser fnNop
    #define flagBrowser     fnNop
    #define fontBrowser     fnNop
    #define flagBrowser_old fnNop      //JM
    #define refreshRegisterLine(a)  {}
    #define displayBugScreen(a)     { printf("\n-----------------------------------------------------------------------\n"); printf("%s\n", a); printf("\n-----------------------------------------------------------------------\n");}
    #define showHideHourGlass()     {}
    #define refreshScreen()         {}
    #define refreshLcd(a)           {}
    #define initFontBrowser()       {}

  #define JM_LAYOUT_1A               //JM Preferred layout
  #endif // defined(TESTSUITE_BUILD) && !defined(GENERATE_CATALOGS)

#define INLINE_TEST                     //vv dr
#undef INLINE_TEST                    //^^

  /* Turn off -Wunused-result for a specific function call */
  #define ignore_result(M) if(1==((int)M)){;}

  #include "assign.h"
  #include "ui/tam.h"
  #include "bufferize.h"
  #include "browsers/browsers.h"
  #include "config.h"
  #include "conversionAngles.h"
  #include "conversionUnits.h"
  #include "curveFitting.h"
  #include "dateTime.h"
  #include "constants.h"
  #include "debug.h"
  #include "error.h"
  #include "flags.h"
  #include "fractions.h"
  #include "fonts.h"
  #include "charString.h" // must be after font.h
  #include "display.h"    // must be after font.h
  #include "distributions/distributions.h"
  #include "gui.h"
  #include "integers.h"
  #include "items.h"
  #include "keyboard.h"
  #include "logicalOps/logicalOps.h"
  #include "mathematics/mathematics.h"
  #include "memory.h"
  #include "programming/programming.h"
  #include "plotstat.h"
  #include "recall.h"
  #include "registers.h"
  #include "registerValueConversions.h"
  #include "saveRestoreCalcState.h"
  #include "screen.h"
  #include "softmenus.h"
  #include "sort.h"
  #include "stack.h"
  #include "stats.h"
  #include "statusBar.h"
  #include "store.h"
  #include "stringFuncs.h"
  #include "timer.h"
  #include "jm_graph.h"                   //JM include 
  #include "graphs.h"                     //JM include 
  #include "jm.h"                         //JM include
  #include "addons.h"                     //JM include
  #include "graphText.h"                  //JM include
  #include "textfiles.h"                  //JM include
  #include "xeqm.h"                       //JM include
  #include "keyboardTweak.h"              //dr
  #include "radioButtonCatalog.h"         //dr
  #ifdef INLINE_TEST                      //vv dr
    #include "inlineTest.h"
  #endif                                  //^^


#define nbrOfElements(x)        (sizeof(x) / sizeof((x)[0]))        //dr vv

  // Variables for the simulator
  #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
    extern bool_t               debugMemAllocation;
  #endif // defined(PC_BUILD) || defined (TESTSUITE_BUILD)
  #ifdef PC_BUILD
    extern bool_t               calcLandscape;
    extern bool_t               calcAutoLandscapePortrait;
    extern GtkWidget           *screen;
    extern GtkWidget           *frmCalc;
    extern int16_t              screenStride;
    extern int16_t              debugWindow;
    extern uint32_t            *screenData;
    extern bool_t               screenChange;
    extern char                 debugString[10000];
    #if (DEBUG_REGISTER_L == 1)
      extern GtkWidget        *lblRegisterL1;
      extern GtkWidget        *lblRegisterL2;
    #endif // (DEBUG_REGISTER_L == 1)
    #if (SHOW_MEMORY_STATUS == 1)
      extern GtkWidget        *lblMemoryStatus;
    #endif // (SHOW_MEMORY_STATUS == 1)
  #endif //PC_BUILD

  // Variables stored in FLASH
  extern const item_t                    indexOfItems[];
  extern const reservedVariableHeader_t  allReservedVariables[];
  extern const char                     *errorMessages[NUMBER_OF_ERROR_CODES];
  extern const calcKey_t                 kbd_std[37];
  extern const font_t                    standardFont, numericFont;
  extern const font_t                   *fontForShortInteger;
  extern const font_t                   *cursorFont;
  extern const char                      digits[17];
  extern void                            (* const addition[NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS][NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS])(void);
  extern void                            (* const subtraction[NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS][NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS])(void);
  extern void                            (* const multiplication[NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS][NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS])(void);
  extern void                            (* const division[NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS][NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS])(void);
  extern void                            (*confirmedFunction)(uint16_t);
  extern const softmenu_t                softmenu[];
  extern real51_t                 const *gammaLanczosCoefficients;
  extern real39_t                 const *angle180;
  extern real39_t                 const *angle90;
  extern real39_t                 const *angle45;

  // Variables stored in RAM
  extern bool_t                 funcOK;
  extern bool_t                 keyActionProcessed;
  extern bool_t                 fnKeyInCatalog;
  extern bool_t                 hourGlassIconEnabled;
  extern bool_t                 watchIconEnabled;
  extern bool_t                 printerIconEnabled;
  extern bool_t                 shiftF;
  extern bool_t                 shiftG;
  extern bool_t                 showContent;
  extern bool_t                 rbr1stDigit;
  extern bool_t                 updateDisplayValueX;
  extern bool_t                 thereIsSomethingToUndo;
  extern bool_t                 lastProgramListEnd;
  extern bool_t                 programListEnd;
  extern bool_t                 serialIOIconEnabled;
  extern bool_t                 neverUsed;

  extern bool_t                 AlphaSelectionBufferTimerRunning;                  //JM  #ifndef USEFLOAT
  extern realContext_t          ctxtRealShort;//   Limited digits: used for high speed internal calcs
  extern realContext_t          ctxtReal34;   //   34 digits
  extern realContext_t          ctxtReal39;   //   39 digits: used for 34 digits intermediate calculations
  extern realContext_t          ctxtReal51;   //   51 digits: used for 34 digits intermediate calculations
  extern realContext_t          ctxtReal75;   //   75 digits: used in SLVQ
  extern realContext_t          ctxtReal1071; // 1071 digits: used in radian angle reduction
  //extern realContext_t          ctxtReal2139; // 2139 digits: used for really big modulo

  extern registerHeader_t       globalRegister[NUMBER_OF_GLOBAL_REGISTERS];
  extern registerHeader_t       savedStackRegister[NUMBER_OF_SAVED_STACK_REGISTERS + NUMBER_OF_TEMP_REGISTERS];
  extern registerHeader_t      *currentLocalRegisters;
  extern dynamicSoftmenu_t      dynamicSoftmenu[NUMBER_OF_DYNAMIC_SOFTMENUS];

  extern dataBlock_t            allSubroutineLevels;
  extern dataBlock_t           *statisticalSumsPointer;
  extern dataBlock_t           *savedStatisticalSumsPointer;
  extern dataBlock_t           *ram;
  extern dataBlock_t           *currentLocalFlags;
  extern dataBlock_t           *currentSubroutineLevelData;

  extern namedVariableHeader_t *allNamedVariables;
  extern softmenuStack_t        softmenuStack[SOFTMENU_STACK_SIZE];
  extern calcKey_t              kbd_usr[37];
  extern calcRegister_t         errorMessageRegisterLine;
  extern glyph_t                glyphNotFound;
  extern freeMemoryRegion_t     freeMemoryRegions[MAX_FREE_REGION];
  extern pcg32_random_t         pcg32_global;
  extern labelList_t           *labelList;
  extern programList_t         *programList;
  extern angularMode_t          currentAngularMode;

  extern char                  *tmpString;
  extern char                  *tmpStringLabelOrVariableName;
  extern char                  *errorMessage;
  extern char                  *aimBuffer; // aimBuffer is also used for NIM
  extern char                  *nimBufferDisplay;

  /**
   * Buffer for output of TAM current state. After calling ::tamProcessInput this
   * buffer is updated to the latest TAM state and should be redrawn to the relevant
   * part of the screen.
   */
  extern char                  *tamBuffer;
  extern char                   asmBuffer[5];
  extern char                   oldTime[8];
  extern char                   dateTimeString[12];
  extern char                   displayValueX[DISPLAY_VALUE_LEN];

  extern uint8_t                numScreensStandardFont;
  extern uint8_t                currentFntScr;
  extern uint8_t                currentFlgScr;
  extern uint8_t                displayFormat;
  extern uint8_t                displayFormatDigits;
  extern uint8_t                timeDisplayFormatDigits;
  extern uint8_t                shortIntegerWordSize;
  extern uint8_t                significantDigits;
  extern uint8_t                shortIntegerMode;
  extern uint8_t                previousCalcMode;
  extern uint8_t                groupingGap;
  extern uint8_t                roundingMode;
  extern uint8_t                calcMode;
  extern uint8_t                nextChar;
  extern uint8_t                displayStack;
  extern uint8_t                displayStackSHOIDISP;         //JM SHOIDISP
  extern bool_t                 numLock;                      //JM
  extern uint8_t                alphaCase;
  extern uint8_t                numLinesNumericFont;
  extern uint8_t                numLinesStandardFont;
  extern uint8_t                cursorEnabled;
  extern uint8_t                nimNumberPart;
  extern uint8_t                hexDigits;
  extern uint8_t                lastErrorCode;
  extern uint8_t                temporaryInformation;
  extern uint8_t                rbrMode;
  extern uint8_t                numScreensNumericFont;
  extern uint8_t               *beginOfProgramMemory;
  extern uint8_t               *beginOfCurrentProgram;
  extern uint8_t               *endOfCurrentProgram;
  extern uint8_t               *firstFreeProgramByte;
  extern uint8_t               *firstDisplayedStep;
  extern uint8_t               *currentStep;

  /**
   * Instance of the internal state for TAM.
   */
  extern tamState_t             tam;
  extern int16_t                currentRegisterBrowserScreen;
  extern int16_t                lineTWidth;
  extern int16_t                rbrRegister;
  extern int16_t                catalog;
  extern int16_t                lastCatalogPosition[NUMBER_OF_CATALOGS];
  extern int16_t                showFunctionNameItem;
  extern int16_t                exponentSignLocation;
  extern int16_t                denominatorLocation;
  extern int16_t                imaginaryExponentSignLocation;
  extern int16_t                imaginaryMantissaSignLocation;
  extern int16_t                exponentLimit;
  extern int16_t                showFunctionNameCounter;
  extern int16_t                dynamicMenuItem;
  extern int16_t               *menu_RAM;
  extern int16_t                numberOfTamMenusToPop;

  extern uint16_t               globalFlags[7];
  extern int16_t                longpressDelayedkey2;         //JM
  extern int16_t                longpressDelayedkey3;         //JM
  extern int16_t                T_cursorPos;                  //JMCURSOR
  extern int16_t                SHOWregis;                    //JMSHOW
  extern int16_t                ListXYposition;               //JM
  extern int16_t                mm_MNU_HOME;                  //JM
  extern int16_t                mm_MNU_ALPHA;                 //JM
  extern int16_t                MY_ALPHA_MENU;                //JM Replaced define
  extern uint8_t                lastSetAngularMode;           //JM
  extern int16_t                JM_auto_doublepress_enabled;  //JM TIMER CLRDROP //drop
  extern int16_t                JM_auto_longpress_enabled;    //JM TIMER CLRDROP //clstk
  extern uint8_t                JM_SHIFT_HOME_TIMER1;         //Local to keyboard.c, but defined here
  extern int16_t                JM_ASN_MODE;                  //JM ASSIGN
  extern bool_t                 ULFL, ULGL;                   //JM Underline
  extern int16_t                FN_key_pressed, FN_key_pressed_last; //JM LONGPRESS FN
  extern bool_t                 FN_timeouts_in_progress;      //JM LONGPRESS FN
  extern bool_t                 Shft_timeouts;                //JM SHIFT NEW FN
  extern bool_t                 FN_timed_out_to_NOP;          //JM LONGPRESS FN
  extern bool_t                 FN_timed_out_to_RELEASE_EXEC; //JM LONGPRESS FN
  extern bool_t                 FN_handle_timed_out_to_EXEC;
  extern char                   indexOfItemsXEQM[18*8];       //JMXEQ
  extern int16_t                fnXEQMENUpos;                 //JMXEQ
  extern uint8_t                last_CM;                      //Do extern !!
  extern uint8_t                FN_state; // = ST_0_INIT;      

  extern uint16_t               glyphRow[NUMBER_OF_GLYPH_ROWS];
  extern uint16_t               freeProgramBytes;
  extern uint16_t               firstDisplayedLocalStepNumber;
  extern uint16_t               numberOfLabels;
  extern uint16_t               numberOfPrograms;
  extern uint16_t               numberOfNamedVariables;
  extern uint16_t               currentLocalStepNumber;
  extern uint16_t               currentProgramNumber;
  extern uint16_t               lrSelection;
  extern uint16_t               lrChosen;
  extern uint16_t               lastPlotMode;
  extern uint16_t               plotSelection;
  
  extern int32_t                numberOfFreeMemoryRegions;
  extern int32_t                lgCatalogSelection;

  extern uint32_t               firstGregorianDay;
  extern uint32_t               denMax;
  extern uint32_t               lastIntegerBase;
  extern uint32_t               alphaSelectionTimer;
  extern uint32_t               xCursor;
  extern uint32_t               yCursor;
  extern uint32_t               tamOverPemYPos;

  extern uint64_t               shortIntegerMask;
  extern uint64_t               shortIntegerSignBit;
  extern uint64_t               systemFlags;
  extern uint64_t               savedSystemFlags;

  extern size_t                 gmpMemInBytes;
  extern size_t                 wp43sMemInBlocks;

  #ifdef DMCP_BUILD
    extern bool_t              backToDMCP;
    extern uint32_t            nextTimerRefresh;    //dr
  #ifdef BUFFER_CLICK_DETECTION
    extern uint32_t            timeStampKey;                                      //dr - internal keyBuffer POC
  #endif
  //extern int                 keyAutoRepeat; // Key repetition // Removed the autorepeat stuff
  //extern int16_t             previousItem;                    // Removed the autorepeat stuff
  #endif // DMCP_BUILD

  #include "constantPointers.h"
#endif // wp43s_H_INCLUDED
