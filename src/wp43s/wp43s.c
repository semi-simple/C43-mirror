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
realContext_t        ctxtReal16;  // 16 digits
realContext_t        ctxtReal34;  // 34 digits
realContext_t        ctxtReal39;  // 39 digits: used for 34 digits intermediate calculations
realContext_t        ctxtReal51;  // 51 digits: used in trigonometric function from WP34S
realContext_t        ctxtReal75;  // 75 digits: used in SLVQ
realContext_t        ctxtReal459; // 459 digits: used in radian angle reduction
realContext_t        ctxtReal855; // 855 digits: used for really big modulo
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
int16_t              alphaSelectionMenu;
int16_t              lastFcnsMenuPos;
int16_t              lastMenuMenuPos;
int16_t              lastCnstMenuPos;
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
uint32_t             alphaSelectionTimer;
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
uint8_t              SigFigMode;                              //JM SIGFIG
bool_t               eRPN;                                    //JM eRPN Create a flag to enable or disable eRPN. See bufferize.c
bool_t               HOME3;                                   //JM HOME Create a flag to enable or disable triple shift HOME3.
bool_t               ShiftTimoutMode;                         //JM SHIFT Create a flag to enable or disable SHIFT TIMER CANCEL.
bool_t               Home3TimerMode;                          //JM HOME Create a flag to enable or disable HOME TIMER CANCEL.
bool_t               UNITDisplay;                             //JM UNITDisplay
bool_t               SH_BASE_HOME;                            //JM BASEHOME
bool_t               SH_BASE_MYMENU;                          //JM BASEHOME
bool_t               SH_BASE_AHOME;                           //JM BASEHOME
bool_t               SH_BASE_MYA;                             //JM BASEHOME
int16_t              Norm_Key_00_VAR;                         //JM USER NORMAL
uint8_t              Input_Default;                           //JM Input Default
bool_t               hourGlassIconEnabled;
bool_t               watchIconEnabled;
bool_t               userModeEnabled;
bool_t               printerIconEnabled;
bool_t               batteryIconEnabled;
bool_t               shiftF;
bool_t               shiftG;
bool_t               shiftStateChanged;
bool_t               showContent;
bool_t               stackLiftEnabled;
bool_t               displayLeadingZeros;
bool_t               displayRealAsFraction;
bool_t               savedStackLiftEnabled;
bool_t               rbr1stDigit;
bool_t               nimInputIsReal34;
bool_t               updateDisplayValueX;
calcKey_t            kbd_usr[37];
radiocb_t            indexOfRadioCbItems[MAX_RADIO_CB_ITEMS];                   //vv dr build RadioButton, CheckBox
uint16_t             cntOfRadioCbItems;                                         //^^
calcRegister_t       errorMessageRegisterLine;
calcRegister_t       errorRegisterLine;
uint16_t             row[100];
uint64_t             shortIntegerMask;
uint64_t             shortIntegerSignBit;
glyph_t              glyphNotFound = {.charCode = 0x0000, .colsBeforeGlyph = 0, .colsGlyph = 13, .colsAfterGlyph = 0, .rowsGlyph = 19};
char                 transitionSystemOperation[4];
char                 displayValueX[DISPLAY_VALUE_LEN];
int16_t              exponentSignLocation;
int16_t              denominatorLocation;
int16_t              imaginaryExponentSignLocation;
int16_t              imaginaryMantissaSignLocation;
size_t               gmpMemInBytes;
size_t               wp43sMemInBytes;
freeBlock_t          freeBlocks[MAX_FREE_BLOCKS];
int32_t              numberOfFreeBlocks;
int32_t              lgCatalogSelection;
void                 (*confirmedFunction)(uint16_t);
real39_t             const *gammaConstants;
real39_t             const *angle180;
real39_t             const *angle90;
real39_t             const *angle45;
pcg32_random_t       pcg32_global = PCG32_INITIALIZER;
#ifdef DMCP_BUILD
  bool_t               endOfProgram;
  uint32_t             nextScreenRefresh; // timer substitute for refreshScreen(), which does cursor blinking and other stuff
  #define TIMER_IDX_SCREEN_REFRESH 0      // use timer 0 to wake up for screen refresh
#endif // DMCP_BUILD


/********************************************//**
 * \brief Sets all the default values of the calc and refreshes the stack
 *
 * \param void
 * \return void
 ***********************************************/
void setupDefaults(void) {
  void *memPtr;

  ram = malloc(RAM_SIZE);
  memset(ram, 0, RAM_SIZE);
  numberOfFreeBlocks = 1;
  freeBlocks[0].address = 0;
  freeBlocks[0].sizeInBlocks = BYTES_TO_BLOCKS(RAM_SIZE);

  glyphNotFound.data   = malloc(38);
  #ifndef __APPLE__
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wstringop-truncation"
  #endif
  strncpy(glyphNotFound.data, "\xff\xf8\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\xff\xf8", 38);
  #ifndef __APPLE__
    #pragma GCC diagnostic pop
  #endif

         //JM vv bug: Overwritten by fnReset
  // Initialization of user key assignments
  memcpy(kbd_usr, kbd_std, sizeof(kbd_std));
  kbd_usr[ 0].keyLblAim   = CHR_A_GRAVE;
  kbd_usr[ 0].fShiftedAim = CHR_A_GRAVE;
  kbd_usr[ 4].keyLblAim   = CHR_E_ACUTE;
  kbd_usr[ 4].fShiftedAim = CHR_E_ACUTE;
  kbd_usr[18].fShifted    = -MNU_VARS;
  kbd_usr[18].gShifted    = CST_54;
  kbd_usr[19].fShifted    = ITM_SW;
  kbd_usr[19].gShifted    = ITM_SXY;
  kbd_usr[20].gShifted    = ITM_LYtoM;
         //JM ^^ bug: Overwritten by fnReset

  // initialize the RadaioButton/Checkbox items
  fnRebuildRadioState();                                                        //dr build RadioButton, CheckBox

  // initialize the 112 global registers
  for(calcRegister_t regist=0; regist<FIRST_LOCAL_REGISTER; regist++) {
    setRegisterDataType(regist, dtReal16, AM_NONE);
    memPtr = allocWp43s(REAL16_SIZE);
    setRegisterDataPointer(regist, memPtr);
    real16Zero(memPtr);
  }

  // initialize the 9+1 saved stack registers
  for(calcRegister_t regist=SAVED_REGISTER_X; regist<=LAST_SAVED_REGISTER; regist++) {
    setRegisterDataType(regist, dtReal16, AM_NONE);
    memPtr = allocWp43s(REAL16_SIZE);
    setRegisterDataPointer(regist, memPtr);
    real16Zero(memPtr);
  }

  #ifdef PC_BUILD
    debugWindow = DBG_REGISTERS;
  #endif
  allowScreenUpdate   = false;

  temporaryInformation = TI_NO_INFO;

  decContextDefault(&ctxtReal16, DEC_INIT_DECDOUBLE);
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

  decContextDefault(&ctxtReal459,  DEC_INIT_DECQUAD);
  ctxtReal459.digits  = 459;
  ctxtReal459.traps   = 0;

  decContextDefault(&ctxtReal855,  DEC_INIT_DECQUAD);
  ctxtReal855.digits  = 855;
  ctxtReal855.traps   = 0;

  statisticalSumsPointer = NULL;

//JM below, indented if fnReset is over-writing the content of setupdefaults. fnReset is in config.c
  fnSetWordSize(64); // word size from 1 to 64               //JM bug: Overwritten by fnReset
  fnIntegerMode(SIM_2COMPL);                                 //JM bug: Overwritten by fnReset

  groupingGap = 3;                                           //JM bug: Overwritten by fnReset. equivalent function, not directly set.

  displayFormat = DF_ALL;                                    //JM bug: Overwritten by fnReset. equivalent function, not directly set.
  displayFormatDigits = 0;                                   //JM bug: Overwritten by fnReset. equivalent function, not directly set.
  fnTimeFormat(TF_H24);                                      //JM bug: Overwritten by fnReset
  fnComplexUnit(CU_I);                                       //JM bug: Overwritten by fnReset
  fnAngularMode(AM_DEGREE);                                  //JM bug: Overwritten by fnReset
  fnDenMode(DM_ANY);                                         //JM bug: Overwritten by fnReset
  denMax = DM_DENMAX;
  fnCurveFitting(CF_LINEAR_FITTING);                         //JM bug: Overwritten by fnReset
  fnLeadingZeros(false);                                     //JM bug: Overwritten by fnReset
  fnProductSign(PS_CROSS);                                   //JM bug: Overwritten by fnReset
  fnFractionType(FT_PROPER);                                 //JM bug: Overwritten by fnReset
  displayRealAsFraction = false;                             //JM bug: Overwritten by fnReset
  fnRadixMark(RM_PERIOD);                                    //JM bug: Overwritten by fnReset
  fnComplexResult(true);                        //JM change: //JM bug: Overwritten by fnReset. CPXRES set default
  fnComplexMode(CM_RECTANGULAR);                             //JM bug: Overwritten by fnReset
  fnDisplayOvr(DO_SCI);                                      //JM bug: Overwritten by fnReset
  fnStackSize(SS_8);                            //JM change: //JM bug: Overwritten by fnReset. SSTACK Stack size 8 default. Tired of changing it every time I reset. Was SS_4 before.
  fnDateFormat(DF_YMD);                                      //JM bug: Overwritten by fnReset
  showFracMode();
  significantDigits = 0;
  fnRoundingMode(RM_HALF_EVEN); // DEC_ROUND_HALF_EVEN.      //JM bug: Overwritten by fnReset
  fnDisplayStack(4);                                         //JM bug: Overwritten by fnReset

  showDateTime();

  shiftF = false;
  shiftG = false;
  shiftStateChanged = false;

  FN_key_pressed = 0;  //JM LONGPRESS FN
  FN_timeouts = 0;     //JM LONGPRESS FN
  FN_counter = JM_FN_TIMER;      //JM LONGPRESS FN
  SigFigMode = 0;                                                //JM SIGFIG Default 0.
  eRPN = false;                                                  //JM eRPN Default. Create a flag to enable or disable eRPN. See bufferize.c
  HOME3 = true;                                                  //JM HOME Default. Create a flag to enable or disable triple shift HOME3.
  ShiftTimoutMode = true;                                        //JM SHIFT Default. Create a flag to enable or disable SHIFT TIMER CANCEL.
  Home3TimerMode = true;                                         //JM SHIFT Default. Create a flag to enable or disable SHIFT TIMER MODE FOR HOME.
  UNITDisplay = false;                                           //JM HOME Default. Create a flag to enable or disable UNIT display
  SH_BASE_HOME   = true;      
  SH_BASE_MYMENU = false;    
  SH_BASE_AHOME  = false;    
  SH_BASE_MYA    = true;       
  Norm_Key_00_VAR  = ITM_SIGMAPLUS;
  Input_Default =  ID_43S;                                       //JM Input Default

  
  softmenuStackPointer_MEM = 0;                                  //JM HOME temporary flag to remember and restore state
  #ifdef DMCP_BUILD                                              //JM TIMER variable tmp mem, to check expired time
  now_MEM = 0;                                                   //JM HOME temporary flag to remember and
  #endif
  #ifdef PC_BUILD
  now_MEM = 0;                                                   //JM HOME temporary flag to remember and
  #endif
  JM_auto_drop_activated = false;                                //JM AUTO-DROP TIMER
  JM_auto_drop_enabled = false;                                  //JM AUTO-DROP TIMER
  JM_SHIFT_RESET = JM_SHIFT_TIMER_LOOP;                          //JM TIMER
  JM_SHIFT_HOME_TIMER1 = JM_SHIFT_TIMER_LOOP;                    //JM TIMER
  JM_ASN_MODE = 0;                                               //JM ASSIGN

  
  #ifndef TESTSUITE_BUILD
    showShiftState();
  #endif // TESTSUITE_BUILD

  currentFntScr = 0;
  currentFlgScr = 0;
  currentRegisterBrowserScreen = 9999;

  softmenuStackPointer = 0;

  aimBuffer[0] = 0;


  fnClearFlag(FLAG_OVERFLOW);
  fnClearFlag(FLAG_CARRY);
  showOverflowCarry();

  fnSetFlag(FLAG_CPXRES);                //JM change: Also overwritten by fnReset. CPXRES set default
  showRealComplexResult();

  showAlphaMode();

  programCounter = 0;
  showPgmBegin();

  cursorBlinkCounter = 0;

  oldTime[0] = 0;

  numberOfLocalRegisters = 0;
  numberOfLocalFlags = 0;
  allLocalRegisterPointer = NULL;

  numberOfNamedVariables = 0;
  allNamedVariablePointer = NULL;

  STACK_LIFT_ENABLE;

  lastErrorCode = 0;

  refreshStack();

  allowScreenUpdate = true;

  hideUserMode();

  gammaConstants = const_gammaC01;
  angle180 = const_180;
  angle90  = const_90;
  angle45  = const_45;

  alphaSelectionMenu = ASM_NONE;

  #ifndef TESTSUITE_BUILD
    resetAlphaSelectionBuffer();
  #endif

  lastFcnsMenuPos = 0;
  lastMenuMenuPos = 0;
  lastCnstMenuPos = 0;

  #ifdef TESTSUITE_BUILD
    calcMode = CM_NORMAL;
  #else
    calcModeNormal();
  #endif // TESTSUITE_BUILD

  #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
    debugMemAllocation = false;
  #endif
}



#ifdef PC_BUILD
int main(int argc, char* argv[]) {
  #if defined __APPLE__
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
  #endif

  wp43sMemInBytes = 0;
  gmpMemInBytes = 0;
  mp_set_memory_functions(allocGmp, reallocGmp, freeGmp);

  calcLandscape             = false;
  calcAutoLandscapePortrait = true;

  for(int arg=1; arg<=argc-1; arg++) {
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


  restoreCalc();

  gdk_threads_add_timeout(LCD_REFRESH_TIMEOUT, refreshScreen, NULL); // refreshScreen is called every 100 ms

  gtk_main();

  return 0;
}
#endif

#ifdef DMCP_BUILD
void program_main(void) {
  int key = 0;
  char charKey[3];
//bool_t wp43sKbdLayout;                                                        //dr - no keymap is used

  wp43sMemInBytes = 0;
  gmpMemInBytes = 0;
  mp_set_memory_functions(allocGmp, reallocGmp, freeGmp);

  // Initialization
  //program_init();

  lcd_clear_buf();
/*lcd_putsAt(t24, 4, "Press EXIT from DM42 (not from WP43S)");                  //vv dr - no keymap is used
  lcd_refresh();
  while (key != 33 && key != 37) {
    key = key_pop();
    while(key == -1) {
      sys_sleep();
      key = key_pop();
    }
  }

  wp43sKbdLayout = (key == 37); // bottom left key
  key = 0;

  lcd_clear_buf();*/                                                            //^^
  setupDefaults();

  endOfProgram = false;

  lcd_refresh();
  nextScreenRefresh = sys_current_ms()+LCD_REFRESH_TIMEOUT;

  // Status flags:
  //   ST(STAT_PGM_END)   - Indicates that program should go to off state (set by auto off timer)
  //   ST(STAT_SUSPENDED) - Program signals it is ready for off and doesn't need to be woken-up again
  //   ST(STAT_OFF)       - Program in off state (OS goes to sleep and only [EXIT] key can wake it up again)
  //   ST(STAT_RUNNING)   - OS doesn't sleep in this mode
  while(!endOfProgram) {
    if(ST(STAT_PGM_END) && ST(STAT_SUSPENDED)) { // Already in off mode and suspended
      CLR_ST(STAT_RUNNING);
      sys_sleep();
    } else if ((!ST(STAT_PGM_END) && key_empty())) {         // Just wait if no keys available.
      CLR_ST(STAT_RUNNING);
      sys_timer_start(TIMER_IDX_SCREEN_REFRESH, max(1, nextScreenRefresh-sys_current_ms()));  // wake up for screen refresh
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

    // Fetch the key
    //  < 0 -> No key event
    //  > 0 -> Key pressed
    // == 0 -> Key released
    key = key_pop();

//    if(38 <= key && key <=43) {
//      sprintf(charKey, "%c", key+11);
//      btnFnClicked(NULL, charKey);
//      lcd_refresh();
//    }

    if(38 <= key && key <= 43) {
      sprintf(charKey, "%c", key +11);
      btnFnPressed(NULL, charKey);
      lcd_refresh();
    } else if(key == 0 && FN_key_pressed != 0) {
      btnFnReleased(NULL,NULL);
      lcd_refresh();
    }


    else if(1 <= key && key <= 37) {
      sprintf(charKey, "%02d", key - 1);
      btnPressed(NULL, charKey);
      lcd_refresh();
    } else if(key == 0) {
      btnReleased(NULL,NULL);
      lcd_refresh();
    }

    uint32_t now = sys_current_ms();
    if(nextScreenRefresh <= now) {
      nextScreenRefresh += LCD_REFRESH_TIMEOUT;
      if(nextScreenRefresh < now) {
        nextScreenRefresh = now + LCD_REFRESH_TIMEOUT;                // we were out longer than expected; just skip ahead.
      }
      refreshScreen();
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
  #endif

  wp43sMemInBytes = 0;
  gmpMemInBytes = 0;
  mp_set_memory_functions(allocGmp, reallocGmp, freeGmp);

  setupDefaults();

  fnReset(CONFIRMED);
/*
reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);
stringToReal16("-2.1", REGISTER_REAL16_DATA(REGISTER_X));
stringToReal16("0", REGISTER_IMAG16_DATA(REGISTER_X));
printf("X = "); printRegisterToConsole(REGISTER_X); printf("\n");
fnSetFlag(FLAG_DANGER);
fnSetFlag(FLAG_CPXRES);
fnLnGamma(NOPARAM);
printf("lngamma(X) = "); printRegisterToConsole(REGISTER_X); printf("\n");
return 0;
*/
  processTests();
  printf("The memory owned by GMP should be 0 bytes. Else report a bug please!\n");
  debugMemory();

  return 0;
}
#endif
