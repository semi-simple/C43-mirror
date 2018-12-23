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
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnConfigChina");
  #endif

  radixMark = RM_PERIOD;
  groupingGap = 4;
  refreshStack();
  timeFormat = TF_H24;
  dateFormat = DF_YMD;
  oldTime[0] = 0;
  firstGregorianDay = 1949; // JDN of the first day in the Gregorian calendar

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnConfigChina");
  #endif
}



/********************************************//**
 * \brief Sets the calc to meet European standards
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnConfigEurope(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnConfigEurope");
  #endif

  radixMark = RM_COMMA;
  groupingGap = 3;
  refreshStack();
  timeFormat = TF_H24;
  dateFormat = DF_DMY;
  oldTime[0] = 0;
  firstGregorianDay = 1582; // JDN of the first day in the Gregorian calendar

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnConfigEurope");
  #endif
}



/********************************************//**
 * \brief Sets the calc to meet Indian standards
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnConfigIndia(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnConfigIndia");
  #endif

  radixMark = RM_PERIOD;
  groupingGap = 0;
  refreshStack();
  timeFormat = TF_H24;
  dateFormat = DF_DMY;
  oldTime[0] = 0;
  firstGregorianDay = 1752; // JDN of the first day in the Gregorian calendar

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnConfigIndia");
  #endif
}



/********************************************//**
 * \brief Sets the calc to meet Japanese standards
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnConfigJapan(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnConfigJapan");
  #endif

  radixMark = RM_PERIOD;
  groupingGap = 3;
  refreshStack();
  timeFormat = TF_H24;
  dateFormat = DF_YMD;
  oldTime[0] = 0;
  firstGregorianDay = 1873; // JDN of the first day in the Gregorian calendar

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnConfigJapan");
  #endif
}



/********************************************//**
 * \brief Sets the calc to meet English standards
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnConfigUk(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnConfigUk");
  #endif

  radixMark = RM_PERIOD;
  groupingGap = 3;
  refreshStack();
  timeFormat = TF_H12;
  dateFormat = DF_DMY;
  oldTime[0] = 0;
  firstGregorianDay = 1752; // JDN of the first day in the Gregorian calendar

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnConfigUk");
  #endif
}



/********************************************//**
 * \brief Sets the calc to meet American standards
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnConfigUsa(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnConfigUsa");
  #endif

  radixMark = RM_PERIOD;
  groupingGap = 3;
  refreshStack();
  timeFormat = TF_H12;
  dateFormat = DF_MDY;
  oldTime[0] = 0;
  firstGregorianDay = 1752; // JDN of the first day in the Gregorian calendar

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnConfigUsa");
  #endif
}



/********************************************//**
 * \brief Displays credits to the brave men who
 * made this project work.
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnWho(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnWho");
  #endif

  temporaryInformation = TI_WHO;
  refreshRegisterLine(REGISTER_X);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnWho");
  #endif
 }



/********************************************//**
 * \brief Displays the version of this software
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnVersion(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnVersion");
  #endif

  temporaryInformation = TI_VERSION;
  refreshRegisterLine(REGISTER_X);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnVersion");
  #endif
}



/********************************************//**
 * \brief Sets X to the amount of free RAM
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnFreeMemory(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnFreeMemory");
  #endif

  liftStack(dtReal16, REAL16_SIZE);
  uInt32ToReal16(lastFreeByte - firstFreeByte + 1, POINTER_TO_REGISTER_DATA(REGISTER_X));
  refreshStack();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnFreeMemory");
  #endif
}



/********************************************//**
 * \brief Sets X to the value of the rounding mode
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnGetRoundingMode(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnGetRoundingMode");
  #endif

  liftStack(dtReal16, REAL16_SIZE);
  int32ToReal16(roundingMode, POINTER_TO_REGISTER_DATA(REGISTER_X));
  refreshStack();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnGetRoundingMode");
  #endif
}



/********************************************//**
 * \brief Sets X to the value of the integer mode
 *
 * \return void
 ***********************************************/
void fnGetIntegerSignMode(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnGetIntegerSignMode");
  #endif

  liftStack(dtReal16, REAL16_SIZE);
  int32ToReal16((smallIntegerMode==SIM_2COMPL ? 2 : (smallIntegerMode==SIM_1COMPL ? 1 : (smallIntegerMode==SIM_UNSIGN ? 0 : -1))), POINTER_TO_REGISTER_DATA(REGISTER_X));
  refreshStack();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnGetIntegerSignMode");
  #endif
}



/********************************************//**
 * \brief Gets the word size
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnGetWordSize(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnGetWordSize");
  #endif

  liftStack(dtReal16, REAL16_SIZE);
  int32ToReal16(smallIntegerWordSize, POINTER_TO_REGISTER_DATA(REGISTER_X));
  refreshStack();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnGetWordSize");
  #endif
}



/********************************************//**
 * \brief Sets the word size
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnSetWordSize(uint16_t WS) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnSetWordSize");
  #endif

  bool_t reduceWordSize;
  if(WS == 0) {
    WS = 64;
  }

  reduceWordSize = (WS < smallIntegerWordSize);

  smallIntegerWordSize = WS;
  showIntegerMode();

  if(smallIntegerWordSize == 64) {
    smallIntegerMask    = -1;
    smallIntegerSignBit = (uint64_t)1 << 63;
  }
  else {
    smallIntegerMask    = ((uint64_t)1 << smallIntegerWordSize) - 1;
    smallIntegerSignBit = (uint64_t)1 << (smallIntegerWordSize - 1);
  }
  //printf("smallIntegerMask  =   %08x-%08x\n", (unsigned int)(smallIntegerMask>>32), (unsigned int)(smallIntegerMask&0xffffffff));
  //printf("smallIntegerSignBit = %08x-%08x\n", (unsigned int)(smallIntegerSignBit>>32), (unsigned int)(smallIntegerSignBit&0xffffffff));

  if(reduceWordSize) {
    // reduce the word size of integers on the stack
    for(calcRegister_t regist=REGISTER_X; regist<=getStackTop(); regist++) {
      if(getRegisterDataType(regist) == dtSmallInteger) {
        *(uint64_t *)(POINTER_TO_REGISTER_DATA(regist)) &= smallIntegerMask;
      }
    }

    // reduce the word size of integers in the L register
    if(getRegisterDataType(REGISTER_L) == dtSmallInteger) {
      *(uint64_t *)(POINTER_TO_REGISTER_DATA(REGISTER_L)) &= smallIntegerMask;
    }
  }

  refreshStack();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnSetWordSize");
  #endif
}



/********************************************//**
 * \brief Sets X to the amount of free flash memory
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnFreeFlashMemory(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnFreeFlashMemory");
  #endif

  liftStack(dtReal16, REAL16_SIZE);
  int32ToReal16(getFreeFlash(), POINTER_TO_REGISTER_DATA(REGISTER_X));
  refreshStack();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnFreeFlashMemory");
  #endif
}



/********************************************//**
 * \brief Sets X to the battery voltage
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnBatteryVoltage(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnBatteryVoltage");
  #endif

  liftStack(dtReal16, REAL16_SIZE);

  #ifdef PC_BUILD
    uInt32ToReal16(3100, POINTER_TO_REGISTER_DATA(REGISTER_X));
  #endif

  #ifdef DMCP_BUILD
    uInt32ToReal16(read_power_voltage(), POINTER_TO_REGISTER_DATA(REGISTER_X));
  #endif

  real16Divide(POINTER_TO_REGISTER_DATA(REGISTER_X), const16_1000, POINTER_TO_REGISTER_DATA(REGISTER_X));
  refreshStack();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnBatteryVoltage");
  #endif
}



/********************************************//**
 * \brief Retrives the amount of free flash memory
 *
 * \param void
 * \return uint32_t Number of bytes free
 ***********************************************/
uint32_t getFreeFlash(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("getFreeFlash");
  #endif

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("getFreeFlash");
  #endif

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
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnGetSignificantDigits");
  #endif

  liftStack(dtReal16, REAL16_SIZE);
  int32ToReal16(significantDigits, POINTER_TO_REGISTER_DATA(REGISTER_X));
  refreshStack();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnGetSignificantDigits");
  #endif
}



/********************************************//**
 * \brief Sets the rounding mode
 *
 * \param[in] RM uint16_t Rounding mode
 * \return void
 ***********************************************/
void fnRoundingMode(uint16_t RM) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnRoundingMode");
  #endif

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

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnRoundingMode");
  #endif
}



/********************************************//**
 * \brief Sets the angular mode
 *
 * \param[in] angularMode uint16_t Angular mode
 * \return void
 ***********************************************/
void fnAngularMode(uint16_t am) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnAngularMode");
  #endif

  angularMode = am;
  showAngularMode();
  refreshStack();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnAngularMode");
  #endif
}



void setConfirmationMode(void (*func)(uint16_t)) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("setConfirmationMode");
  #endif

  previousCalcMode = calcMode;
  calcMode = CM_CONFIRMATION;
  confirmedFunction = func;
  temporaryInformation = TI_ARE_YOU_SURE;
  refreshStack();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("setConfirmationMode");
  #endif
}



void fnClAll(uint16_t confirmation) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnClAll");
  #endif

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


    // initialize the temporary registers
    for(regist=FIRST_TEMPORARY_REGISTER; regist<FIRST_TEMPORARY_REGISTER+NUMBER_OF_TEMPORARY_REGISTERS; regist++) {
      tempRegistersInUse[regist - FIRST_TEMPORARY_REGISTER] = false;
    }

    // Clear flags
    for(int32_t sixteenFlags=0; sixteenFlags<7; sixteenFlags++) { // 7 times uint16_t = 112 flags
      flags[sixteenFlags] = 0;
    }

    temporaryInformation = TI_NO_INFO;
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnClAll");
  #endif
}



void fnClPAll(uint16_t confirmation) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnClPAll");
  #endif

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

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnClPAll");
  #endif
}



void fnClSigma(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnClSigma");
  #endif

  if(statisticalSumsPointer != 0) {
    freeMemory(statisticalSumsPointer, 14*REAL16_SIZE);
    statisticalSumsPointer = 0;
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnClSigma");
  #endif
}



void fnReset(uint16_t confirmation) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnReset");
  #endif

  if(confirmation == NOT_CONFIRMED) {
    setConfirmationMode(fnReset);
  }
  else {
    //printf("Running RESET\n");
    clearScreen(true, true, true);

    fnClAll(CONFIRMED); // Clears pgm and registers

    fnTimeFormat(TF_H24);
    fnIntegerMode(SIM_2COMPL);
    fnDisplayFormatAll(0);
    fnDisplayFormatGap(3);
    fnComplexUnit(CU_I);
    fnAngularMode(AM_DEGREE);
    fnDenMode(DM_ANY);
    fnDenMax(0);
    fnDisplayStack(4);
    firstGregorianDay = 1752;
    fnCurveFitting(CF_LINEAR_FITTING);
    fnLeadingZeros(false);
    fnProductSign(PS_CROSS);
    fnFractionType(FT_PROPER);
    fnRadixMark(RM_PERIOD);
    fnRoundingMode(RM_HALF_EVEN);
    fnDisplayOvr(DO_SCI);
    fnStackSize(SS_4);
    //tDisp = -1;
    fnSetWordSize(64);
    fnDateFormat(DF_YMD);
    fnComplexMode(CM_RECTANGULAR);

    displayRealAsFraction = false;
    #if (STACK_LIFT_DEBUG == 1)
      stackLiftDisable();
    #else
      stackLiftEnabled = false;
    #endif
    showOverflowCarry();

    // Initialization of user key assignments
    memcpy(kbd_usr, kbd_std, sizeof(kbd_std));

    while(softmenuStackPointer > 0) {
      popSoftmenu();
    }

    oldTime[0] = 0;

    temporaryInformation = TI_RESET;

    // The following lines are test data
    kbd_usr[ 0].keyLblAim   = CHR_A_GRAVE;
    kbd_usr[ 0].fShiftedAim = CHR_A_GRAVE;
    kbd_usr[ 4].keyLblAim   = CHR_E_ACUTE;
    kbd_usr[ 4].fShiftedAim = CHR_E_ACUTE;
    kbd_usr[18].fShifted    = -MNU_VARS;
    kbd_usr[18].gShifted    = CST_54;
    kbd_usr[19].fShifted    = ITM_SW;
    kbd_usr[19].gShifted    = ITM_SXY;
    kbd_usr[20].gShifted    = ITM_LYtoM;

    setFlag(  3);
    setFlag( 11);
    setFlag( 33);
    setFlag( 34);
    setFlag( 52);
    setFlag( 62);
    setFlag( 77);
    setFlag( 85);
    setFlag(FLAG_CARRY);
    setFlag(FLAG_DANGER);
    showOverflowCarry();

    allocateLocalRegisters(2);
    setFlag(FIRST_LOCAL_REGISTER+0);
    setFlag(NUMBER_OF_LOCAL_FLAGS+2);
    reallocateRegister(FIRST_LOCAL_REGISTER+0, dtReal16, REAL16_SIZE, 0);
    stringToReal16("5.555", POINTER_TO_REGISTER_DATA(FIRST_LOCAL_REGISTER));

    strcpy(tmpStr3000, "Pure ASCII string requiring 38 bytes!");
    reallocateRegister(FIRST_LOCAL_REGISTER+1, dtString, strlen(tmpStr3000), 0);
    strcpy(POINTER_TO_REGISTER_STRING(FIRST_LOCAL_REGISTER + 1), tmpStr3000);


    allocateNamedRegister("Z" STD_a_DIARESIS "hler");
    allocateNamedRegister(STD_omega STD_SUB_1);
    allocateNamedRegister(STD_omega STD_SUB_2);

    #if (DEBUG_PANEL == 1)
      debugWindow = DBG_REGISTERS;
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(chkHexaString), false);
      refreshDebugPanel();
    #endif

    refreshStack();
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnReset");
  #endif
}



void backToSystem(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("backToSystem");
  #endif

  #ifdef PC_BUILD
    fnOff(NOPARAM);
  #endif

  #ifdef DMCP_BUILD
    endOfProgram = true;
  #endif

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("backToSystem");
  #endif
}
