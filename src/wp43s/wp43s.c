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

//#define JMSHOWCODES


#include "wp43s.h"

#if defined(PC_BUILD) || defined(TESTSUITE_BUILD)
  bool_t              debugMemAllocation;
#endif // PC_BUILD || TESTSUITE_BUILD
#ifdef PC_BUILD
  bool_t              calcLandscape;
  bool_t              calcAutoLandscapePortrait;
  GtkWidget           *screen;
  GtkWidget           *frmCalc;
  int16_t             screenStride;
  int16_t             debugWindow;
  uint32_t            *screenData;
  bool_t              screenChange;
  char                debugString[10000];
  #if (DEBUG_REGISTER_L == 1)
    GtkWidget         *lblRegisterL1;
    GtkWidget         *lblRegisterL2;
  #endif // (DEBUG_REGISTER_L == 1)
  #if (SHOW_MEMORY_STATUS == 1)
    GtkWidget         *lblMemoryStatus;
  #endif // (SHOW_MEMORY_STATUS == 1)
#endif // PC_BUILD

const font_t          *fontForShortInteger;
const font_t          *cursorFont;
TO_QSPI const char     digits[17] = "0123456789ABCDEF";
real51_t               const *gammaLanczosCoefficients;
real39_t               const *angle180;
real39_t               const *angle90;
real39_t               const *angle45;
void                   (*confirmedFunction)(uint16_t);

// Variables stored in RAM
bool_t                 funcOK;
bool_t                 keyActionProcessed;
bool_t                 fnKeyInCatalog;
bool_t                 hourGlassIconEnabled;
bool_t                 watchIconEnabled;
bool_t                 printerIconEnabled;
bool_t                 shiftF;
bool_t                 shiftG;
bool_t                 showContent;
bool_t                 rbr1stDigit;
bool_t                 updateDisplayValueX;
bool_t                 thereIsSomethingToUndo;
bool_t                 lastProgramListEnd;
bool_t                 programListEnd;
bool_t                 serialIOIconEnabled;
bool_t                 neverUsed;

realContext_t          ctxtReal34;   //   34 digits
realContext_t          ctxtReal39;   //   39 digits: used for 34 digits intermediate calculations
realContext_t          ctxtReal51;   //   51 digits: used for 34 digits intermediate calculations
realContext_t          ctxtReal75;   //   75 digits: used in SLVQ
realContext_t          ctxtReal1071; // 1071 digits: used in radian angle reduction
//realContext_t          ctxtReal2139; // 2139 digits: used for really big modulo

registerHeader_t       globalRegister[NUMBER_OF_GLOBAL_REGISTERS];
registerHeader_t       savedStackRegister[NUMBER_OF_SAVED_STACK_REGISTERS + NUMBER_OF_TEMP_REGISTERS];
registerHeader_t      *currentLocalRegisters;

dataBlock_t            allSubroutineLevels;
dataBlock_t           *statisticalSumsPointer;
dataBlock_t           *savedStatisticalSumsPointer;
dataBlock_t           *ram = NULL;
dataBlock_t           *currentLocalFlags;
dataBlock_t           *currentSubroutineLevelData;

namedVariableHeader_t *allNamedVariables;
softmenuStack_t        softmenuStack[SOFTMENU_STACK_SIZE];
calcKey_t              kbd_usr[37];
calcRegister_t         errorMessageRegisterLine;
glyph_t                glyphNotFound = {.charCode = 0x0000, .colsBeforeGlyph = 0, .colsGlyph = 13, .colsAfterGlyph = 0, .rowsGlyph = 19, .data = NULL};
freeMemoryRegion_t     freeMemoryRegions[MAX_FREE_REGION];
pcg32_random_t         pcg32_global = PCG32_INITIALIZER;
labelList_t           *labelList = NULL;
programList_t         *programList = NULL;
angularMode_t          currentAngularMode;

char                  *tmpString = NULL;
char                  *tmpStringLabelOrVariableName = NULL;
char                  *errorMessage;
char                  *aimBuffer; // aimBuffer is also used for NIM
char                  *nimBufferDisplay;
char                  *tamBuffer;
char                   asmBuffer[5];
char                   oldTime[8];
char                   dateTimeString[12];
char                   displayValueX[DISPLAY_VALUE_LEN];

uint8_t                numScreensStandardFont;
uint8_t                currentFntScr;
uint8_t                currentFlgScr;
uint8_t                displayFormat;
uint8_t                displayFormatDigits;
uint8_t                timeDisplayFormatDigits;
uint8_t                shortIntegerWordSize;
uint8_t                significantDigits;
uint8_t                shortIntegerMode;
uint8_t                previousCalcMode;
uint8_t                groupingGap;
uint8_t                roundingMode;
uint8_t                calcMode;
uint8_t                nextChar;
uint8_t                displayStack;
uint8_t                alphaCase;
uint8_t                numLinesNumericFont;
uint8_t                numLinesStandardFont;
uint8_t                cursorEnabled;
uint8_t                nimNumberPart;
uint8_t                hexDigits;
uint8_t                lastErrorCode;
uint8_t                temporaryInformation;
uint8_t                rbrMode;
uint8_t                numScreensNumericFont;
uint8_t               *beginOfProgramMemory;
uint8_t               *beginOfCurrentProgram;
uint8_t               *endOfCurrentProgram;
uint8_t               *firstFreeProgramByte;
uint8_t               *firstDisplayedStep;
uint8_t               *currentStep;

tamState_t             tam;
int16_t                currentRegisterBrowserScreen;
int16_t                lineTWidth;
int16_t                rbrRegister;
int16_t                catalog;
int16_t                lastCatalogPosition[NUMBER_OF_CATALOGS];
int16_t                showFunctionNameItem;

uint8_t               displayStackSHOIDISP;          //JM SHOIDISP
bool_t                numLock;                       //JM
bool_t                doRefreshSoftMenu;                       //dr
bool_t                jm_GGREEK; 
bool_t                jm_FG_LINE;                              //JM Screen / keyboard operation setup
bool_t                jm_NO_BASE_SCREEN;                              //JM Screen / keyboard operation setup
bool_t                jm_G_DOUBLETAP;                          //JM Screen / keyboard operation setup
bool_t                jm_HOME_SUM;                             //JMHOME
bool_t                jm_HOME_MIR;                             //JMHOME
bool_t                jm_HOME_FIX;                             //JMHOME
bool_t                jm_LARGELI;
uint8_t               SigFigMode;                              //JM SIGFIG
bool_t                eRPN;                                    //JM eRPN Create a flag to enable or disable eRPN. See bufferize.c
bool_t                HOME3;                                   //JM HOME Create a flag to enable or disable triple shift HOME3.
bool_t                ShiftTimoutMode;                         //JM SHIFT Create a flag to enable or disable SHIFT TIMER CANCEL.
bool_t                Home3TimerMode;                          //JM HOME Create a flag to enable or disable HOME TIMER CANCEL.
bool_t                UNITDisplay;                             //JM UNITDisplay
bool_t                SH_BASE_HOME;                            //JM BASEHOME
bool_t                SH_BASE_AHOME;                           //JM BASEHOME
int16_t               Norm_Key_00_VAR;                         //JM USER NORMAL
uint8_t               Input_Default;                           //JM Input Default
float                 graph_xmin;                              //JM Graph
float                 graph_xmax;                              //JM Graph
float                 graph_ymin;                              //JM Graph
float                 graph_ymax;                              //JM Graph
// float                 graph_dx;                                //JM Graph
// float                 graph_dy;                                //JM Graph
// bool_t                extentx;                                 //JM Graph
// bool_t                extenty;                                 //JM Graph
// bool_t                jm_VECT;                                 //JM GRAPH
// bool_t                jm_NVECT;                                //JM GRAPH
// bool_t                jm_SCALE;                                //JM GRAPH
// bool_t                Aspect_Square;                           //JM GRAPH
// bool_t                PLOT_LINE;                               //JM GRAPH
// bool_t                PLOT_CROSS;                              //JM GRAPH
// bool_t                PLOT_BOX;                                //JM GRAPH
// bool_t                PLOT_INTG;                               //JM GRAPH
// bool_t                PLOT_DIFF;                               //JM GRAPH
// bool_t                PLOT_RMS;                                //JM GRAPH
// bool_t                PLOT_SHADE;                              //JM GRAPH
// bool_t                PLOT_AXIS;                               //JM GRAPH
// int8_t                PLOT_ZMX;                                //JM GRAPH
// int8_t                PLOT_ZMY;                                //JM GRAPH
uint8_t               lastSetAngularMode;
bool_t                AlphaSelectionBufferTimerRunning;        //JM
#ifdef INLINE_TEST                                             //vv dr
bool_t                testEnabled;                             //
uint16_t              testBitset;                              //
#endif                                                         //^^
int16_t                longpressDelayedkey2;         //JM
int16_t                longpressDelayedkey3;         //JM
int16_t                T_cursorPos;                  //JMCURSOR
int16_t                SHOWregis;                    //JMSHOW
int16_t                ListXYposition;               //JMSHOW
int16_t                mm_MNU_HOME;                  //JM
int16_t                mm_MNU_ALPHA;                 //JM
int16_t                MY_ALPHA_MENU = MY_ALPHA_MENU_CNST;  //JM
int16_t                JM_auto_doublepress_enabled;  //JM TIMER CLRDROP //drop
int16_t                JM_auto_longpress_enabled;    //JM TIMER CLRDROP //clstk
uint8_t                JM_SHIFT_HOME_TIMER1;         //Local to keyboard.c, but defined here
int16_t                JM_ASN_MODE;                  //JM ASSIGN
bool_t                 ULFL, ULGL;                   //JM Underline
int16_t                FN_key_pressed, FN_key_pressed_last; //JM LONGPRESS FN
bool_t                 FN_timeouts_in_progress;      //JM LONGPRESS FN
bool_t                 Shft_timeouts;                //JM SHIFT NEW FN
bool_t                 FN_timed_out_to_NOP;          //JM LONGPRESS FN
bool_t                 FN_timed_out_to_RELEASE_EXEC; //JM LONGPRESS FN
bool_t                 FN_handle_timed_out_to_EXEC;
char                   indexOfItemsXEQM[18*8];       //JMXEQ
int16_t                fnXEQMENUpos;                 //JMXEQ
uint8_t                last_CM = 255;                //Do extern !!
uint8_t                FN_state; // = ST_0_INIT;      

int16_t                exponentSignLocation;
int16_t                denominatorLocation;
int16_t                imaginaryExponentSignLocation;
int16_t                imaginaryMantissaSignLocation;
int16_t                exponentLimit;
int16_t                showFunctionNameCounter;
int16_t                dynamicMenuItem;
int16_t               *menu_RAM;
int16_t                numberOfTamMenusToPop;

uint16_t               globalFlags[7];
uint16_t               freeProgramBytes;
uint16_t               glyphRow[NUMBER_OF_GLYPH_ROWS];
uint16_t               firstDisplayedLocalStepNumber;
uint16_t               numberOfLabels;
uint16_t               numberOfPrograms;
uint16_t               numberOfNamedVariables;
uint16_t               currentLocalStepNumber;
uint16_t               currentProgramNumber;

int32_t                numberOfFreeMemoryRegions;
int32_t                lgCatalogSelection;

uint32_t               firstGregorianDay;
uint32_t               denMax;
uint32_t               lastIntegerBase;
uint32_t               alphaSelectionTimer;
uint32_t               xCursor;
uint32_t               yCursor;
uint32_t               tamOverPemYPos;

uint64_t               shortIntegerMask;
uint64_t               shortIntegerSignBit;
uint64_t               systemFlags;
uint64_t               savedSystemFlags;

size_t                 gmpMemInBytes;
size_t                 wp43sMemInBlocks;

#ifdef DMCP_BUILD
  #ifdef JMSHOWCODES                                           //JM Test
    int8_t            telltale_pos;                            //JM Test
    int8_t            telltale_lastkey;                        //JM Test
  #endif                                                       //JM Test 
  uint32_t            nextTimerRefresh;                        //dr timer substitute for refreshTimer()
#ifdef BUFFER_CLICK_DETECTION
  uint32_t            timeStampKey;                                             //dr - internal keyBuffer POC
#endif
  bool_t              backToDMCP;
//  int                 keyAutoRepeat;   // removed autorepeat stuff
//  int16_t             previousItem;    // removed autorepeat stuff
  uint32_t            nextScreenRefresh; // timer substitute for refreshLcd(), which does cursor blinking and other stuff
#endif // DMCP_BUILD


#ifdef PC_BUILD
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

  #ifdef __MINGW64__
    system("chcp 65001"); // Configure for UTF-8 output on the Windows console
  #endif // __MINGW64__

    wp43sMemInBlocks = 0;
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

    restoreCalc();

    // Without the following 8 lines of code
      // the f- and g-shifted labels are
      // miss aligned! I dont know why!
      calcModeAimGui();
      while(gtk_events_pending()) {
        gtk_main_iteration();
      }
      calcModeNormalGui();
      while(gtk_events_pending()) {
        gtk_main_iteration();
      }

   #ifdef INIT_RAMDUMP
   ramDump();
   #endif

  refreshScreen();

  gdk_threads_add_timeout(SCREEN_REFRESH_PERIOD, refreshLcd, NULL); // refreshLcd is called every SCREEN_REFRESH_PERIOD ms
  fnTimerReset();                                                    //dr timeouts for kb handling
  fnTimerConfig(TO_FG_LONG, refreshFn, TO_FG_LONG/*, 580*/);
  fnTimerConfig(TO_CL_LONG, refreshFn, TO_CL_LONG/*, 500*/);
  fnTimerConfig(TO_FG_TIMR, refreshFn, TO_FG_TIMR/*, 4000*/);
  fnTimerConfig(TO_FN_LONG, refreshFn, TO_FN_LONG/*, 450*/);
  fnTimerConfig(TO_FN_EXEC, execFnTimeout, 0/*, 150*/);
  fnTimerConfig(TO_3S_CTFF, shiftCutoff, TO_3S_CTFF/*, 600*/);
  fnTimerConfig(TO_CL_DROP, fnTimerDummyTest, TO_CL_DROP/*, 500*/);
//fnTimerConfig(TO_KB_ACTV, fnTimerDummyTest, TO_KB_ACTV/*, 6000*/);  //dr no keyboard scan boost for emulator
  gdk_threads_add_timeout(5, refreshTimer, NULL);                     //dr refreshTimer is called every 5 ms    //^^

    gtk_main();

    return 0;
  }
#endif // PC_BUILD

#ifdef DMCP_BUILD
  void program_main(void) {
    int key = 0;
    char charKey[3];
#ifdef BUFFER_KEY_COUNT
  int keyCount = 0;                                                             //dr - internal keyBuffer POC
#endif
#ifdef BUFFER_CLICK_DETECTION
  timeStampKey = (uint32_t)sys_current_ms();                                    //dr - internal keyBuffer POC
#endif
  int count_refreshes = 0;                                                      //dr clock down refresh after 1 minute of no keystroke
  //bool_t wp43sKbdLayout, inFastRefresh, seenKeyPress = 0;;            // removed autorepeat stuff //dr - no keymap is used


    uint16_t currentVolumeSetting, savedVoluleSetting; // used for beep signaling screen shot
    uint32_t previousRefresh;

    wp43sMemInBlocks = 0;
    gmpMemInBytes = 0;
    mp_set_memory_functions(allocGmp, reallocGmp, freeGmp);

  lcd_clear_buf();

#ifdef NOKEYMAP
  lcd_putsAt(t24, 4, "Press the bottom left key."); lcd_refresh();           //vv dr - no keymap is used
  while(key != 33 && key != 37) {
    key = key_pop();
    while(key == -1) {
      sys_sleep();
      key = key_pop();
    }
  }

    wp43sKbdLayout = (key == 37); // bottom left key
    key = 0;

  lcd_clear_buf();                                             //^^
#endif //NOKEYMAP


  fnReset(CONFIRMED);
  refreshScreen();

  #ifdef JMSHOWCODES                                           //JM test
  telltale_lastkey = 0;                                        //JM test
  telltale_pos = 0;                                            //JM test
  #endif                                                       //JM test

    #if 0
      longInteger_t li;
      uint32_t addr, min, max, *ptr;

      min = 1;
      max = 100000000;
      while(min+1 < max) {
        ptr = malloc((max + min) >> 1);
        if(ptr) {
          free(ptr);
          min = (max + min) >> 1;
        }
        else {
          max = (max + min) >> 1;
        }
      }

      ptr = malloc(min);
      xcopy(&addr, &ptr, 4);
      free(ptr);
      longIntegerInit(li);
      uIntToLongInteger(addr, li);
      convertLongIntegerToShortIntegerRegister(li, 16, 50);

      uIntToLongInteger(min, li);
      convertLongIntegerToShortIntegerRegister(li, 10, 51);

      ptr = (uint32_t *)qspi_user_addr();
      xcopy(&addr, &ptr, 4);
      uIntToLongInteger(addr, li);
      convertLongIntegerToShortIntegerRegister(li, 16, 52);

      addr = (uint32_t)qspi_user_size(); // QSPI user size in bytes
      uIntToLongInteger(addr, li);
      convertLongIntegerToShortIntegerRegister(li, 10, 53);

      ptr = (uint32_t *)&ram;
      xcopy(&addr, &ptr, 4);
      uIntToLongInteger(addr, li);
      convertLongIntegerToShortIntegerRegister(li, 16, 54);

      ptr = (uint32_t *)&indexOfItems;
      xcopy(&addr, &ptr, 4);
      uIntToLongInteger(addr, li);
      convertLongIntegerToShortIntegerRegister(li, 16, 55);

      ptr = (uint32_t *)ppgm_fp;
      xcopy(&addr, &ptr, 4);
      uIntToLongInteger(addr, li);
      convertLongIntegerToShortIntegerRegister(li, 16, 56);

      ptr = (uint32_t *)get_reset_state_file();
      xcopy(&addr, &ptr, 4);
      uIntToLongInteger(addr, li);
      convertLongIntegerToShortIntegerRegister(li, 16, 57);

      addr = 0x38; // RESET_STATE_FILE_SIZE;
      uIntToLongInteger(addr, li);
      convertLongIntegerToShortIntegerRegister(li, 10, 58);

      ptr = (uint32_t *)aux_buf_ptr();
      xcopy(&addr, &ptr, 4);
      uIntToLongInteger(addr, li);
      convertLongIntegerToShortIntegerRegister(li, 16, 59);

      addr = AUX_BUF_SIZE;
      uIntToLongInteger(addr, li);
      convertLongIntegerToShortIntegerRegister(li, 10, 60);

      ptr = (uint32_t *)write_buf_ptr();
      xcopy(&addr, &ptr, 4);
      uIntToLongInteger(addr, li);
      convertLongIntegerToShortIntegerRegister(li, 16, 61);

      addr = (uint32_t)write_buf_size();
      uIntToLongInteger(addr, li);
      convertLongIntegerToShortIntegerRegister(li, 10, 62);

      addr = (uint32_t)get_hw_id();
      uIntToLongInteger(addr, li);
      convertLongIntegerToShortIntegerRegister(li, 10, 63);

      ptr = (uint32_t *)resizeProgramMemory;
      xcopy(&addr, &ptr, 4);
      uIntToLongInteger(addr, li);
      convertLongIntegerToShortIntegerRegister(li, 16, 64);

      longIntegerFree(li);
    #endif // 1

    backToDMCP = false;

    lcd_forced_refresh();                                        //JM 
    previousRefresh = sys_current_ms();
    //inFastRefresh = 0;                                       // removed autorepeat stuff
    nextScreenRefresh = previousRefresh + SCREEN_REFRESH_PERIOD;
    //runner_key_tout_init(0); // Enables fast auto repeat     // Remove all autorepeat stuff

    fnTimerReset();                                              //vv dr timeouts for kb handling
    fnTimerConfig(TO_FG_LONG, refreshFn, TO_FG_LONG/*, 580*/);
    fnTimerConfig(TO_CL_LONG, refreshFn, TO_CL_LONG/*, 500*/);
    fnTimerConfig(TO_FG_TIMR, refreshFn, TO_FG_TIMR/*, 4000*/);
    fnTimerConfig(TO_FN_LONG, refreshFn, TO_FN_LONG/*, 450*/);
    fnTimerConfig(TO_FN_EXEC, execFnTimeout, 0/*, 150*/);
    fnTimerConfig(TO_3S_CTFF, shiftCutoff, TO_3S_CTFF/*, 600*/);
    fnTimerConfig(TO_CL_DROP, fnTimerDummyTest, TO_CL_DROP/*, 500*/);
    fnTimerConfig(TO_KB_ACTV, fnTimerDummyTest, TO_KB_ACTV/*, 6000*/);
    nextTimerRefresh = 0;                                        //vv

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
      else if ((!ST(STAT_PGM_END) && key_empty() && emptyKeyBuffer())) {        // Just wait if no keys available.      //dr - internal keyBuffer POC
        uint32_t sleepTime = max(1, nextScreenRefresh - sys_current_ms());        //vv dr timer without DMCP timer
        if(nextTimerRefresh != 0) {
          uint32_t timeoutTime = max(1, nextTimerRefresh - sys_current_ms());
          sleepTime = min(sleepTime, timeoutTime);
        }
        if(fnTimerGetStatus(TO_KB_ACTV) == TMR_RUNNING) {
          sleepTime = min(sleepTime, 40);
        }
        if(fnTimerGetStatus(TO_FN_EXEC) == TMR_RUNNING) {
          sleepTime = min(sleepTime, 15);
        }                                                                         //^^
        CLR_ST(STAT_RUNNING);
        sys_timer_start(TIMER_IDX_SCREEN_REFRESH, max(1, sleepTime));             // wake up for screen refresh           //dr
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

#ifdef NOKEYMAP
      // Fetch the key
      //  < 0 -> No key event
      //  > 0 -> Key pressed
      // == 0 -> Key released
      //key = key_pop();
      key = runner_get_key_delay(&keyAutoRepeat, 100, 100, 100, 100);
      //key = runner_get_key(&keyAutoRepeat);

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
      //The 3 lines below to see in the top left screen corner the pressed keycode
      //char sysLastKeyCh[5];
      //sprintf(sysLastKeyCh, " %02d", key);
      //showString(sysLastKeyCh, &standardFont, 0, 0, vmReverse, true, true);
      //The line below to emit a beep
      //while(get_beep_volume() < 11) beep_volume_up(); start_buzzer_freq(220000); sys_delay(200); stop_buzzer();

      // If we have seen a key press, increase the refresh to pick up auto key repeats
      // seenKeyPress = (key > 0);    //Removed AUTOREPEAT STUFF
#endif //NOKEYMAP


    uint8_t outKey;
    keyBuffer_pop();

#ifdef BUFFER_CLICK_DETECTION
    uint32_t timeSpan_1;
    uint32_t timeSpan_B;
#ifdef BUFFER_KEY_COUNT
    uint8_t outKeyCount;
    if(outKeyBuffer(&outKey, &outKeyCount, &timeStampKey, &timeSpan_1, &timeSpan_B) == BUFFER_SUCCESS) {
      key = outKey;
      keyCount = outKeyCount;
//    if(outKeyCount > 0) {
//      do someting
//    }
    }
#else
    if(outKeyBuffer(&outKey, &timeStampKey, &timeSpan_1, &timeSpan_B) == BUFFER_SUCCESS) {
      key = outKey;
    }
#endif
#else
#ifdef BUFFER_KEY_COUNT
    uint8_t outKeyCount;
    if(outKeyBuffer(&outKey, &outKeyCount) == BUFFER_SUCCESS) {
      key = outKey;
      keyCount = outKeyCount;
    }
#else
    if(outKeyBuffer(&outKey) == BUFFER_SUCCESS) {
      key = outKey;
    }
#endif
#endif
    else {
      key = -1;
    }                                                       //^^


#ifdef AUTOREPEAT
      if(keyAutoRepeat) {
        if(key == 27 || key == 32) { // UP or DOWN keys
          //beep(2200, 50);
          key = 0; // to trigger btnReleased
        }
        else {
          key = -1;
        }
      }
#endif //AUTOREPEAT

      if(key == 44) { //DISP for special SCREEN DUMP key code. To be 16 but shift decoding already done to 44 in DMCP
      resetShiftState();                                       //JM to avoid f or g top left of the screen

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
   
   #ifdef JMSHOWCODES 
    fnDisplayStack(1);
    //Show key codes
    if(sys_last_key()!=telltale_lastkey) {
       telltale_lastkey = sys_last_key();
       telltale_pos++;
       telltale_pos = telltale_pos & 0x03;
       char aaa[100];
       sprintf   (aaa,"k=%d d=%ld      ",key, timeSpan);
       showString(aaa, &standardFont, 300, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(REGISTER_T - REGISTER_X), vmNormal, true, true);
       sprintf   (aaa,"Rel=%d, nop=%d, St=%d, Key=%d, FN_kp=%d   ",FN_timed_out_to_RELEASE_EXEC, FN_timed_out_to_NOP, FN_state, sys_last_key(), FN_key_pressed);
       showString(aaa, &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(REGISTER_Z - REGISTER_X), vmNormal, true, true);
       sprintf   (aaa,"%4d(%4ld)<<",sys_last_key(),timeSpan);
       showString(aaa, &standardFont, telltale_pos*90+ 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(REGISTER_Y - REGISTER_X), vmNormal, true, true);
     }
    #endif

      if(38 <= key && key <=43) { // Function key
        sprintf(charKey, "%c", key+11);
        btnFnPressed(charKey);
      //lcd_refresh_dma();
      }
      else if(1 <= key && key <= 37) { // Not a function key
        sprintf(charKey, "%02d", key - 1);
        btnPressed(charKey);
      //lcd_refresh_dma();
      }

#ifdef AUTOREPEAT
      else if(key == 0) { // Autorepeat
        if(charKey[1] == 0) { // Last key pressed was one of the 6 function keys
          btnFnReleased(charKey);
        }
        else { // Last key pressed was not one of the 6 function keys
          //beep(440, 50);
          btnReleased(charKey);
        }
        keyAutoRepeat = 0;
        lcd_refresh();
      }

      // Compute refresh period
      if(showFunctionNameCounter > 0 || seenKeyPress) {
        inFastRefresh = 1;
        nextScreenRefresh = previousRefresh + FAST_SCREEN_REFRESH_PERIOD;
      } else {
        inFastRefresh = 0;
      }
#endif //AUTOREPEAT


      else if(key == 0 && FN_key_pressed != 0) {                 //JM, key=0 is release, therefore there must have been a press before that. If the press was a FN key, FN_key_pressed > 0 when it comes back here for release.
        btnFnReleased(NULL);                                     //    in short, it can only execute FN release after there was a FN press.
      //lcd_refresh_dma();
      }
      else if(key == 0) {
        btnReleased(NULL);
      //lcd_refresh_dma();
      }

      if(key >= 0) {                                          //dr
        lcd_refresh_dma();
        fnTimerStart(TO_KB_ACTV, TO_KB_ACTV, JM_TO_KB_ACTV);  //dr
        count_refreshes = 10;                                 //dr
        }

    uint32_t now = sys_current_ms();


//      if(nextScreenRefresh <= now) {
//        previousRefresh = now;
//        nextScreenRefresh = previousRefresh + (inFastRefresh ? FAST_SCREEN_REFRESH_PERIOD : SCREEN_REFRESH_PERIOD);
//        refreshLcd();
//        lcd_refresh();
//      }


    if(nextTimerRefresh != 0 && nextTimerRefresh <= now) {  //vv dr Timer
      refreshTimer();
    }                                                       //^^
    now = sys_current_ms();                                 //vv dr
    if(nextScreenRefresh <= now) {
      nextScreenRefresh += SCREEN_REFRESH_PERIOD;
      count_refreshes++;
      if(count_refreshes > 384)
        nextScreenRefresh += SCREEN_REFRESH_PERIOD;
      if(nextScreenRefresh < now) {
        nextScreenRefresh = now + SCREEN_REFRESH_PERIOD;    // we were out longer than expected; just skip ahead.
      }
      refreshLcd();
      if(key >= 0) lcd_refresh();    //JMTOCHECK if key>0 is needed. what about -1?
      else {lcd_refresh_wait();}
    }                                                       //^^

  }
}
#endif // DMCP_BUILD

#ifdef TESTSUITE_BUILD
  #include "testSuite.h"

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

    wp43sMemInBlocks = 0;
    gmpMemInBytes = 0;
    mp_set_memory_functions(allocGmp, reallocGmp, freeGmp);

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
    debugMemory("End of testsuite");

    return 0;
  }
#endif // TESTSUITE_BUILD
