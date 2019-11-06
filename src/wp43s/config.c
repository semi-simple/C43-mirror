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
 * \file config.c
 ***********************************************/

#include "wp43s.h"



/********************************************//**
 * \brief Sets the calc to meet Chinese standards
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnConfigChina(uint16_t unusedParamButMandatory) {
  radixMark = RM_PERIOD;
  groupingGap = 4;
  refreshStack();
  timeFormat = TF_H24;
  dateFormat = DF_YMD;
  oldTime[0] = 0;
  firstGregorianDay = 1949; // JDN of the first day in the Gregorian calendar
}



/********************************************//**
 * \brief Sets the calc to meet European standards
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnConfigEurope(uint16_t unusedParamButMandatory) {
  radixMark = RM_COMMA;
  groupingGap = 3;
  refreshStack();
  timeFormat = TF_H24;
  dateFormat = DF_DMY;
  oldTime[0] = 0;
  firstGregorianDay = 1582; // JDN of the first day in the Gregorian calendar
}



/********************************************//**
 * \brief Sets the calc to meet Indian standards
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnConfigIndia(uint16_t unusedParamButMandatory) {
  radixMark = RM_PERIOD;
  groupingGap = 3;
  refreshStack();
  timeFormat = TF_H24;
  dateFormat = DF_DMY;
  oldTime[0] = 0;
  firstGregorianDay = 1752; // JDN of the first day in the Gregorian calendar
}



/********************************************//**
 * \brief Sets the calc to meet Japanese standards
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnConfigJapan(uint16_t unusedParamButMandatory) {
  radixMark = RM_PERIOD;
  groupingGap = 3;
  refreshStack();
  timeFormat = TF_H24;
  dateFormat = DF_YMD;
  oldTime[0] = 0;
  firstGregorianDay = 1873; // JDN of the first day in the Gregorian calendar
}



/********************************************//**
 * \brief Sets the calc to meet English standards
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnConfigUk(uint16_t unusedParamButMandatory) {
  radixMark = RM_PERIOD;
  groupingGap = 3;
  refreshStack();
  timeFormat = TF_H12;
  dateFormat = DF_DMY;
  oldTime[0] = 0;
  firstGregorianDay = 1752; // JDN of the first day in the Gregorian calendar
}



/********************************************//**
 * \brief Sets the calc to meet American standards
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnConfigUsa(uint16_t unusedParamButMandatory) {
  radixMark = RM_PERIOD;
  groupingGap = 3;
  refreshStack();
  timeFormat = TF_H12;
  dateFormat = DF_MDY;
  oldTime[0] = 0;
  firstGregorianDay = 1752; // JDN of the first day in the Gregorian calendar
}



/********************************************//**
 * \brief Sets the integer mode
 *
 * \param[in] mode uint16_t Integer mode
 * \return void
 ***********************************************/
void fnIntegerMode(uint16_t mode) {
  shortIntegerMode = mode;
  
  fnRefreshRadioState(RB_SIM, mode);                                            //dr

  showIntegerMode();
  refreshStack();
}

/********************************************//**
 * \brief Sets the the leading zeros mode in integer mode
 *
 * \param[in] dlz uint16_t
 * \return void
 ***********************************************/
void fnLeadingZeros(uint16_t dlz) {
  displayLeadingZeros = dlz;

  fnRefreshRadioState(RB_BLZ, dlz);                                             //dr

  refreshStack();
}



/********************************************//**
 * \brief Displays credits to the brave men who
 * made this project work.
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnWho(uint16_t unusedParamButMandatory) {
  temporaryInformation = TI_WHO;
  refreshRegisterLine(REGISTER_X);
 }



/********************************************//**
 * \brief Displays the version of this software
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnVersion(uint16_t unusedParamButMandatory) {
  temporaryInformation = TI_VERSION;
  refreshRegisterLine(REGISTER_X);
}



/********************************************//**
 * \brief Sets X to the amount of free RAM
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnFreeMemory(uint16_t unusedParamButMandatory) {
  longInteger_t mem;

  liftStack();

  longIntegerInit(mem);
  uIntToLongInteger(getFreeRamMemory(), mem);
  convertLongIntegerToLongIntegerRegister(mem, REGISTER_X);
  longIntegerFree(mem);

  refreshStack();
}



/********************************************//**
 * \brief Sets X to the value of the rounding mode
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnGetRoundingMode(uint16_t unusedParamButMandatory) {
  longInteger_t rounding;

  liftStack();

  longIntegerInit(rounding);
  uIntToLongInteger(roundingMode, rounding);
  convertLongIntegerToLongIntegerRegister(rounding, REGISTER_X);
  longIntegerFree(rounding);

  refreshStack();
}



/********************************************//**
 * \brief Sets X to the value of the integer mode
 *
 * \return void
 ***********************************************/
void fnGetIntegerSignMode(uint16_t unusedParamButMandatory) {
  longInteger_t ism;

  liftStack();

  longIntegerInit(ism);
  uIntToLongInteger((shortIntegerMode==SIM_2COMPL ? 2 : (shortIntegerMode==SIM_1COMPL ? 1 : (shortIntegerMode==SIM_UNSIGN ? 0 : -1))), ism);
  convertLongIntegerToLongIntegerRegister(ism, REGISTER_X);
  longIntegerFree(ism);

  refreshStack();
}



/********************************************//**
 * \brief Gets the word size
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnGetWordSize(uint16_t unusedParamButMandatory) {
  longInteger_t wordSize;

  liftStack();

  longIntegerInit(wordSize);
  uIntToLongInteger(shortIntegerWordSize, wordSize);
  convertLongIntegerToLongIntegerRegister(wordSize, REGISTER_X);
  longIntegerFree(wordSize);

  refreshStack();
}



/********************************************//**
 * \brief Sets the word size
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnSetWordSize(uint16_t WS) {
  bool_t reduceWordSize;
  if(WS == 0) {
    WS = 64;
  }

  reduceWordSize = (WS < shortIntegerWordSize);

  shortIntegerWordSize = WS;
  showIntegerMode();

  if(shortIntegerWordSize == 64) {
    shortIntegerMask    = -1;
    shortIntegerSignBit = (uint64_t)1 << 63;
  }
  else {
    shortIntegerMask    = ((uint64_t)1 << shortIntegerWordSize) - 1;
    shortIntegerSignBit = (uint64_t)1 << (shortIntegerWordSize - 1);
  }
  //printf("shortIntegerMask  =   %08x-%08x\n", (unsigned int)(shortIntegerMask>>32), (unsigned int)(shortIntegerMask&0xffffffff));
  //printf("shortIntegerSignBit = %08x-%08x\n", (unsigned int)(shortIntegerSignBit>>32), (unsigned int)(shortIntegerSignBit&0xffffffff));

  if(reduceWordSize) {
    // reduce the word size of integers on the stack
    for(calcRegister_t regist=REGISTER_X; regist<=getStackTop(); regist++) {
      if(getRegisterDataType(regist) == dtShortInteger) {
        *(REGISTER_SHORT_INTEGER_DATA(regist)) &= shortIntegerMask;
      }
    }

    // reduce the word size of integers in the L register
    if(getRegisterDataType(REGISTER_L) == dtShortInteger) {
      *(REGISTER_SHORT_INTEGER_DATA(REGISTER_L)) &= shortIntegerMask;
    }
  }

  refreshStack();
}



/********************************************//**
 * \brief Sets X to the amount of free flash memory
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnFreeFlashMemory(uint16_t unusedParamButMandatory) {
  longInteger_t flashMem;

  liftStack();

  longIntegerInit(flashMem);
  uIntToLongInteger(getFreeFlash(), flashMem);
  convertLongIntegerToLongIntegerRegister(flashMem, REGISTER_X);
  longIntegerFree(flashMem);

  refreshStack();
}



/********************************************//**
 * \brief Sets X to the battery voltage
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnBatteryVoltage(uint16_t unusedParamButMandatory) {
  realIc_t value;

  liftStack();

  #ifdef PC_BUILD
    int32ToRealIc(3100, &value);
  #endif

  #ifdef DMCP_BUILD
    int32ToRealIc(read_power_voltage(), &value);
  #endif

  realIcDivide(&value, const_1000, &value);
  realIcToReal16(&value, REGISTER_REAL16_DATA(REGISTER_X));
  refreshStack();
}



/********************************************//**
 * \brief Retrives the amount of free flash memory
 *
 * \param void
 * \return uint32_t Number of bytes free
 ***********************************************/
uint32_t getFreeFlash(void) {
  return 1234567u;
}



/********************************************//**
 * \brief Sets X to the number of signficant digits
 * rounding after each operation
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnGetSignificantDigits(uint16_t unusedParamButMandatory) {
  longInteger_t sigDigits;

  liftStack();

  longIntegerInit(sigDigits);
  uIntToLongInteger(significantDigits, sigDigits);
  convertLongIntegerToLongIntegerRegister(sigDigits, REGISTER_X);
  longIntegerFree(sigDigits);

  refreshStack();
}



/********************************************//**
 * \brief Sets the rounding mode
 *
 * \param[in] RM uint16_t Rounding mode
 * \return void
 ***********************************************/
void fnRoundingMode(uint16_t RM) {
  roundingMode = RM;

  if(RM == 0) ctxtReal16.round = DEC_ROUND_HALF_EVEN;
  else if(RM == 1) ctxtReal16.round = DEC_ROUND_HALF_UP;
  else if(RM == 2) ctxtReal16.round = DEC_ROUND_HALF_DOWN;
  else if(RM == 3) ctxtReal16.round = DEC_ROUND_UP;
  else if(RM == 4) ctxtReal16.round = DEC_ROUND_DOWN;
  else if(RM == 5) ctxtReal16.round = DEC_ROUND_CEILING;
  else if(RM == 6) ctxtReal16.round = DEC_ROUND_FLOOR;
  else {
    sprintf(errorMessage, "In function fnRoundingMode: %d is an unexpected value for RM! Must be from 0 to 6", RM);
    displayBugScreen(errorMessage);
  }

  ctxtReal34.round = ctxtReal16.round;
}



/********************************************//**
 * \brief Sets the angular mode
 *
 * \param[in] am uint16_t Angular mode
 * \return void
 ***********************************************/
void fnAngularMode(uint16_t am) {
  currentAngularMode = am;

  fnRefreshRadioState(RB_AM, am);                                               //dr

  showAngularMode();
  refreshStack();
}



void setConfirmationMode(void (*func)(uint16_t)) {
  previousCalcMode = calcMode;
  calcMode = CM_CONFIRMATION;
  confirmedFunction = func;
  temporaryInformation = TI_ARE_YOU_SURE;
  refreshStack();
}



/********************************************//**
 * \brief Defines the complex unit i or j
 *
 * \param[in] complexUnit uint16_t
 * \return void
 ***********************************************/
void fnComplexUnit(uint16_t cu) {
  complexUnit = cu;

  fnRefreshRadioState(RB_CU, cu);                                               //dr

  refreshStack();
}

/********************************************//**
 * \brief Defines if an operation result can be complex or not
 *
 * \param[in] complexRes uint16_t
 * \return void
 ***********************************************/
void fnComplexResult(uint16_t complexResult) {
  complexResult ? fnSetFlag(FLAG_CPXRES) : fnClearFlag(FLAG_CPXRES);

  fnRefreshRadioState(RB_BCR, complexResult);                                   //dr
}

/********************************************//**
 * \brief Defines the complex display mode: rectangular or polar
 *
 * \param[in] complexMode uint16_t
 * \return void
 ***********************************************/
void fnComplexMode(uint16_t cm) {
  complexMode = cm;

  fnRefreshRadioState(RB_CM, cm);                                               //dr

  showComplexMode();
  refreshStack();
}



void fnClAll(uint16_t confirmation) {
  if(confirmation == NOT_CONFIRMED) {
    setConfirmationMode(fnClAll);
  }
  else {
//printf("Running CLALL\n");
    calcRegister_t regist;

    fnClPAll(CONFIRMED);  // Clears all the programs
    fnClSigma(CONFIRMED); // Clears and releases the memory of all statistical sums

    // Clear local registers
    allocateLocalRegisters(0);
    //for(regist=FIRST_LOCAL_REGISTER; regist<FIRST_LOCAL_REGISTER + numberOfLocalRegisters; regist++) {
    //  clearRegister(regist);
    //}

    // Clear registers including stack, I, J, K and L
    for(regist=0; regist<FIRST_LOCAL_REGISTER; regist++) {
      clearRegister(regist);
    }

    // Clear flags
    for(int32_t sixteenFlags=0; sixteenFlags<7; sixteenFlags++) { // 7 times uint16_t = 112 flags
      flags[sixteenFlags] = 0;
    }

    temporaryInformation = TI_NO_INFO;
  }
}



void fnClPAll(uint16_t confirmation) {
  if(confirmation == NOT_CONFIRMED) {
    setConfirmationMode(fnClPAll);
  }
  else {
    //printf("Running CLPALL\n");
    // TODO: actually clear all programs
    programCounter = 0;
    showPgmBegin();
    temporaryInformation = TI_NO_INFO;
  }
}



void fnClSigma(uint16_t unusedParamButMandatory) {
  if(statisticalSumsPointer != NULL) {
    freeWp43s(statisticalSumsPointer, 14*REAL34_SIZE);
    statisticalSumsPointer = NULL;
  }
}



void fnReset(uint16_t confirmation) {
  if(confirmation == NOT_CONFIRMED) {
    setConfirmationMode(fnReset);
  }
  else {
    clearScreen(true, true, true);

    fnClAll(CONFIRMED); // Clears pgm and registers

    fnTimeFormat(TF_H24);                                //JM bug. over-writing the content of setupdefaults
    fnIntegerMode(SIM_2COMPL);                           //JM bug. over-writing the content of setupdefaults
    fnDisplayFormatAll(0);                               //JM bug. over-writing the content of setupdefaults
    fnDisplayFormatGap(3);                               //JM bug. over-writing the content of setupdefaults
    fnComplexUnit(CU_I);                                 //JM bug. over-writing the content of setupdefaults
    fnAngularMode(AM_DEGREE);                            //JM bug. over-writing the content of setupdefaults
    fnDenMode(DM_ANY);                                   //JM bug. over-writing the content of setupdefaults
    fnDenMax(0);
    fnDisplayStack(4);                                   //JM bug. over-writing the content of setupdefaults
    firstGregorianDay = 1752;
    fnCurveFitting(CF_LINEAR_FITTING);                   //JM bug. over-writing the content of setupdefaults
    fnLeadingZeros(false);                               //JM bug. over-writing the content of setupdefaults
    fnProductSign(PS_CROSS);                             //JM bug. over-writing the content of setupdefaults
    fnFractionType(FT_PROPER); // a b/c                  //JM bug. over-writing the content of setupdefaults
    fnRadixMark(RM_PERIOD);                              //JM bug. over-writing the content of setupdefaults
    fnRoundingMode(RM_HALF_EVEN);                        //JM bug. over-writing the content of setupdefaults
    fnDisplayOvr(DO_SCI);                                //JM bug. over-writing the content of setupdefaults
    fnStackSize(SS_8);               //JM Changed 4 to 8 //JM bug. over-writing the content of setupdefaults
    //tDisp = -1;
    fnSetWordSize(64);                                   //JM bug. over-writing the content of setupdefaults
    fnDateFormat(DF_YMD);                                //JM bug. over-writing the content of setupdefaults
    fnComplexMode(CM_RECTANGULAR);                       //JM bug. over-writing the content of setupdefaults
    fnComplexResult(true);           //JM CPXRES set     //JM bug. over-writing the content of setupdefaults
    showRealComplexResult();
    allocateLocalRegisters(0);

    displayRealAsFraction = false;                       //JM bug. over-writing the content of setupdefaults
    STACK_LIFT_DISABLE;
    showOverflowCarry();
    hideUserMode();

    // Initialization of user key assignments
    memcpy(kbd_usr, kbd_std, sizeof(kbd_std));

    // initialize the RadaioButton/Checkbox items
    fnRebuildRadioState();                                                      //dr build RadioButton, CheckBox

    #ifndef TESTSUITE_BUILD
      while(softmenuStackPointer > 0) {
        popSoftmenu();
      }
    #endif // TESTSUITE_BUILD

    oldTime[0] = 0;

    temporaryInformation = TI_RESET;

    //JM Default USER
    fnJM(USER_DEFAULTS);                         //JM USER
//    kbd_usr[0].primary     = KEY_CC;           //JM CPX TEMP DEFAULT         //JM bug. over-writing the content of setupdefaults
//    kbd_usr[0].gShifted    = KEY_TYPCON_UP;    //JM TEMP DEFAULT            //JM bug. over-writing the content of setupdefaults
//    kbd_usr[0].fShifted    = KEY_TYPCON_DN;    //JM TEMP DEFAULT            //JM bug. over-writing the content of setupdefaults

    // The following lines are test data
    //kbd_usr[ 0].keyLblAim   = CHR_A_GRAVE;
    //kbd_usr[ 0].fShiftedAim = CHR_A_GRAVE;
    //kbd_usr[ 4].keyLblAim   = CHR_E_ACUTE;
    //kbd_usr[ 4].fShiftedAim = CHR_E_ACUTE;
    //kbd_usr[18].fShifted    = -MNU_VARS;
    //kbd_usr[18].gShifted    = CST_54;
    //kbd_usr[19].fShifted    = ITM_SW;
    //kbd_usr[19].gShifted    = ITM_SXY;
    //kbd_usr[20].gShifted    = ITM_LYtoM;

    //fnSetFlag(  3);
    //fnSetFlag( 11);
    //fnSetFlag( 33);
    //fnSetFlag( 34);
    //fnSetFlag( 52);
    //fnSetFlag( 62);
    //fnSetFlag( 77);
    //fnSetFlag( 85);
    //fnSetFlag(FLAG_CARRY);
    //fnSetFlag(FLAG_DANGER);
    //showOverflowCarry();

    //allocateLocalRegisters(2);
    //fnSetFlag(FIRST_LOCAL_REGISTER+0);
    //fnSetFlag(NUMBER_OF_LOCAL_FLAGS+2);
    //reallocateRegister(FIRST_LOCAL_REGISTER+0, dtReal16, REAL16_SIZE, RT_REAL);
    //stringToReal16("5.555", REGISTER_REAL16_DATA(FIRST_LOCAL_REGISTER));

    //strcpy(tmpStr3000, "Pure ASCII string requiring 38 bytes!");
    //reallocateRegister(FIRST_LOCAL_REGISTER+1, dtString, strlen(tmpStr3000), AM_NONE);
    //strcpy(REGISTER_STRING_DATA(FIRST_LOCAL_REGISTER + 1), tmpStr3000);


    //allocateNamedVariable("Z" STD_a_DIARESIS "hler");
    //allocateNamedVariable(STD_omega STD_SUB_1);
    //allocateNamedVariable(STD_omega STD_SUB_2);

    #if (DEBUG_PANEL == 1)
      debugWindow = DBG_REGISTERS;
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(chkHexaString), false);
      refreshDebugPanel();
    #endif

    refreshStack();
  }
}



void backToSystem(uint16_t unusedParamButMandatory) {
  #ifdef PC_BUILD
    fnOff(NOPARAM);
  #endif

  #ifdef DMCP_BUILD
    endOfProgram = true;
  #endif
}



//vv dr build RadioButton, CheckBox
int8_t fnCbIsSet(int16_t item) {
  int8_t result = -1;
  uint16_t itemNr = max(item, -item);

  for(uint8_t i=0; i < cntOfRadioCbItems; i++) {
    if(indexOfRadioCbItems[i].itemNr == itemNr) {
      result = indexOfRadioCbItems[i].state;
    }
  }

  return result;
}



void fnRefreshRadioState(char rb, uint16_t mode) {
  for(uint8_t i=0; i < cntOfRadioCbItems; i++) {
    if(indexOfRadioCbItems[i].radioButton == rb) {
      uint8_t cb = (indexOfRadioCbItems[i].param == mode) ? 1 : 0;
      if(indexOfRadioCbItems[i].state != cb) {
        indexOfRadioCbItems[i].state = cb;
#ifndef TESTSUITE_BUILD
        showSoftmenuCurrentPart();
#endif
      }
      //indexOfRadioCbItems[i].state = indexOfRadioCbItems[i].param == mode;
    }
  }
}



void fnRefreshComboxState(char rb, uint16_t param, bool_t state) {
  for(uint8_t i=0; i < cntOfRadioCbItems; i++) {
    if((indexOfRadioCbItems[i].radioButton == rb) && (indexOfRadioCbItems[i].param == param)) {
      uint8_t cb = state ? 3 : 2;
      if(indexOfRadioCbItems[i].state != cb) {
        indexOfRadioCbItems[i].state = cb;
#ifndef TESTSUITE_BUILD
        showSoftmenuCurrentPart();
#endif
      }
    }
  }
}



void fnRebuildRadioState() {
  uint8_t i=0;
  for(uint16_t k=0; k<LAST_ITEM; k++) {
    if(indexOfItems[k].func == fnAngularMode) {
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = indexOfItems[k].param;
      rb.state = (currentAngularMode == rb.param) ? 1 : 0;
      rb.radioButton = RB_AM;
      indexOfRadioCbItems[i] = rb;
      if(i<MAX_RADIO_CB_ITEMS) {
        i++;
      }
    }
    else if(indexOfItems[k].func == fnComplexMode) {
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = indexOfItems[k].param;
      rb.state = (complexMode == rb.param) ? 1 : 0;
      rb.radioButton = RB_CM;
      indexOfRadioCbItems[i] = rb;
      if(i<MAX_RADIO_CB_ITEMS) {
        i++;
      }
    }
    else if(indexOfItems[k].func == fnComplexUnit) {
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = indexOfItems[k].param;
      rb.state = (complexUnit == rb.param) ? 1 : 0;
      rb.radioButton = RB_CU;
      indexOfRadioCbItems[i] = rb;
      if(i<MAX_RADIO_CB_ITEMS) {
        i++;
      }
    }
    else if(indexOfItems[k].func == fnCurveFitting) {
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = indexOfItems[k].param;
      rb.state = (curveFitting == rb.param) ? 1 : 0;
      rb.radioButton = RB_CF;
      indexOfRadioCbItems[i] = rb;
      if(i<MAX_RADIO_CB_ITEMS) {
        i++;
      }
    }
    else if(indexOfItems[k].func == fnDateFormat) {
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = indexOfItems[k].param;
      rb.state = (dateFormat == rb.param) ? 1 : 0;
      rb.radioButton = RB_DF;
      indexOfRadioCbItems[i] = rb;
      if(i<MAX_RADIO_CB_ITEMS) {
        i++;
      }
    }
    else if(indexOfItems[k].func == fnDenMode) {
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = indexOfItems[k].param;
      rb.state = (denominatorMode == rb.param) ? 1 : 0;
      rb.radioButton = RB_DM;
      indexOfRadioCbItems[i] = rb;
      if(i<MAX_RADIO_CB_ITEMS) {
        i++;
      }
    }
    else if(indexOfItems[k].func == fnDisplayFormatAll) {
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = DF_ALL;
      rb.state = (displayFormat == rb.param) ? 1 : 0;
      rb.radioButton = RB_DI;
      indexOfRadioCbItems[i] = rb;
      if(i<MAX_RADIO_CB_ITEMS) {
        i++;
      }
    }
    else if(indexOfItems[k].func == fnDisplayFormatEng) {
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = DF_ENG;
      rb.state = (displayFormat == rb.param) ? 1 : 0;
      rb.radioButton = RB_DI;
      indexOfRadioCbItems[i] = rb;
      if(i<MAX_RADIO_CB_ITEMS) {
        i++;
      }
    }
    else if(indexOfItems[k].func == fnDisplayFormatFix) {
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = DF_FIX;
      rb.state = (displayFormat == rb.param) && !(UNITDisplay || (SigFigMode != 0)) ? 1 : 0;  //JM
      rb.radioButton = RB_DI;
      indexOfRadioCbItems[i] = rb;
      if(i<MAX_RADIO_CB_ITEMS) {
        i++;
      }
    }
    else if(indexOfItems[k].func == fnDisplayFormatSigFig) {          //JM vv
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = DF_SF;
      rb.state = (SigFigMode != 0) ? 1 : 0;
      rb.radioButton = RB_DI;
      indexOfRadioCbItems[i] = rb;
      if(i<MAX_RADIO_CB_ITEMS) {
        i++;
      }
    }

    else if(indexOfItems[k].func == fnDisplayFormatUnit) {
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = DF_UN;
      rb.state = UNITDisplay ? 1 : 0;
      rb.radioButton = RB_DI;
      indexOfRadioCbItems[i] = rb;
      if(i<MAX_RADIO_CB_ITEMS) {
        i++;
      }
    }                                                                 //JM ^^
    else if(indexOfItems[k].func == fnDisplayFormatSci) {
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = DF_SCI;
      rb.state = (displayFormat == rb.param) ? 1 : 0;
      rb.radioButton = RB_DI;
      indexOfRadioCbItems[i] = rb;
      if(i<MAX_RADIO_CB_ITEMS) {
        i++;
      }
    }
    else if(indexOfItems[k].func == fnDisplayOvr) {
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = indexOfItems[k].param;
      rb.state = (displayModeOverride == rb.param) ? 1 : 0;
      rb.radioButton = RB_DO;
      indexOfRadioCbItems[i] = rb;
      if(i<MAX_RADIO_CB_ITEMS) {
        i++;
      }
    }
    else if(indexOfItems[k].func == fnFractionType) {
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = indexOfItems[k].param;
      rb.state = (fractionType == rb.param) ? 1 : 0;
      rb.radioButton = RB_FT;
      indexOfRadioCbItems[i] = rb;
      if(i<MAX_RADIO_CB_ITEMS) {
        i++;
      }
    }
    else if(indexOfItems[k].func == fnIntegerMode) {
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = indexOfItems[k].param;
      rb.state = (shortIntegerMode == rb.param) ? 1 : 0;
      rb.radioButton = RB_SIM;
      indexOfRadioCbItems[i] = rb;
      if(i<MAX_RADIO_CB_ITEMS) {
        i++;
      }
    }
    else if(indexOfItems[k].func == fnProductSign) {
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = indexOfItems[k].param;
      rb.state = (productSign == rb.param) ? 1 : 0;
      rb.radioButton = RB_PS;
      indexOfRadioCbItems[i] = rb;
      if(i<MAX_RADIO_CB_ITEMS) {
        i++;
      }
    }
    else if(indexOfItems[k].func == fnRadixMark) {
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = indexOfItems[k].param;
      rb.state = (radixMark == rb.param) ? 1 : 0;
      rb.radioButton = RB_RM;
      indexOfRadioCbItems[i] = rb;
      if(i<MAX_RADIO_CB_ITEMS) {
        i++;
      }
    }
    else if(indexOfItems[k].func == fnStackSize) {
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = indexOfItems[k].param;
      rb.state = (stackSize == rb.param) ? 1 : 0;
      rb.radioButton = RB_SS;
      indexOfRadioCbItems[i] = rb;
      if(i<MAX_RADIO_CB_ITEMS) {
        i++;
      }
    }
    else if(indexOfItems[k].func == fnTimeFormat) {
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = indexOfItems[k].param;
      rb.state = (timeFormat == rb.param) ? 1 : 0;
      rb.radioButton = RB_TF;
      indexOfRadioCbItems[i] = rb;
      if(i<MAX_RADIO_CB_ITEMS) {
        i++;
      }
    }
    else if(indexOfItems[k].func == fnInDefault) {
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = indexOfItems[k].param;
      rb.state = (Input_Default == rb.param) ? 1 : 0;
      rb.radioButton = RB_ID;
      indexOfRadioCbItems[i] = rb;
      if(i<MAX_RADIO_CB_ITEMS) {
        i++;
      }
    }
    else if(indexOfItems[k].func == fnSigmaAssign) {
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = indexOfItems[k].param;
      rb.state = (Norm_Key_00_VAR == rb.param) ? 1 : 0;
      rb.radioButton = RB_SA;
      indexOfRadioCbItems[i] = rb;
      if(i<MAX_RADIO_CB_ITEMS) {
        i++;
      }
    }
    else if(indexOfItems[k].func == fnRefreshLcd) {         //dr
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = indexOfItems[k].param;
      rb.state = (LcdTimeout == rb.param) ? 1 : 0;
      rb.radioButton = RB_RL;
      indexOfRadioCbItems[i] = rb;
      if(i<MAX_RADIO_CB_ITEMS) {
        i++;
      }
    }
    else if(indexOfItems[k].func == fnComplexResult) {
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = indexOfItems[k].param;
      rb.state = (getFlag(FLAG_CPXRES) == rb.param) ? 1 : 0;
      rb.radioButton = RB_BCR;
      indexOfRadioCbItems[i] = rb;
      if(i<MAX_RADIO_CB_ITEMS) {
        i++;
      }
    }
    else if(indexOfItems[k].func == fnLeadingZeros) {
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = indexOfItems[k].param;
      rb.state = (displayLeadingZeros == rb.param) ? 1 : 0;
      rb.radioButton = RB_BLZ;
      indexOfRadioCbItems[i] = rb;
      if(i<MAX_RADIO_CB_ITEMS) {
        i++;
      }
    }
    else if(indexOfItems[k].func == fnSetSetJM) {
      radiocb_t rb;
      rb.itemNr = k;
      rb.param = indexOfItems[k].param;
      switch (rb.param)
      {
      case JC_ERPN:
        rb.state = eRPN? 3 : 2;
        break;
      
      case JC_HOME_TRIPLE:
        rb.state = HOME3? 3 : 2;
        break;
      
      case JC_SHFT_4s:
        rb.state = ShiftTimoutMode? 3 : 2;
        break;
      
      case JC_BASE_HOME:
        rb.state = SH_BASE_HOME? 3 : 2;
        break;
      
      case JC_BASE_MYMENU:
        rb.state = SH_BASE_MYMENU? 3 : 2;
        break;
      
      case JC_BASE_AHOME:
        rb.state = SH_BASE_AHOME? 3 : 2;
        break;
      
      case JC_BASE_MYA:
        rb.state = SH_BASE_MYA? 3 : 2;
        break;
      
      case JC_SH_3T:
        rb.state = Home3TimerMode? 3 : 2;
        break;
      
      case DR_ITM_LCD:        //dr
        rb.state = RefreshLcd? 3 : 2;
        break;
      
      default:
        break;
      }
      rb.radioButton = CB_JC;
      indexOfRadioCbItems[i] = rb;
      if(i<MAX_RADIO_CB_ITEMS) {
        i++;
      }
    }
    
  }
  cntOfRadioCbItems = i;

  printf("Nbr of RadioButton/Checkbox  %6u\n", cntOfRadioCbItems);
}
//^^
