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
  setSystemFlag(FLAG_DECIMP);
  groupingGap = 4;
  setSystemFlag(FLAG_TDM24); // time format = 24H
  clearSystemFlag(FLAG_DMY); // date format
  clearSystemFlag(FLAG_MDY); // date format
  setSystemFlag(FLAG_YMD);   // date format
  firstGregorianDay = 1949;  // JDN of the first day in the Gregorian calendar
}



/********************************************//**
 * \brief Sets the calc to meet European standards
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnConfigEurope(uint16_t unusedParamButMandatory) {
  clearSystemFlag(FLAG_DECIMP);
  groupingGap = 3;
  setSystemFlag(FLAG_TDM24); // time format = 24H
  clearSystemFlag(FLAG_MDY); // date format
  clearSystemFlag(FLAG_YMD); // date format
  setSystemFlag(FLAG_DMY);   // date format
  firstGregorianDay = 1582;  // JDN of the first day in the Gregorian calendar
}



/********************************************//**
 * \brief Sets the calc to meet Indian standards
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnConfigIndia(uint16_t unusedParamButMandatory) {
  setSystemFlag(FLAG_DECIMP);
  groupingGap = 3;
  setSystemFlag(FLAG_TDM24); // time format = 24H
  clearSystemFlag(FLAG_MDY); // date format
  clearSystemFlag(FLAG_YMD); // date format
  setSystemFlag(FLAG_DMY);   // date format
  firstGregorianDay = 1752;  // JDN of the first day in the Gregorian calendar
}



/********************************************//**
 * \brief Sets the calc to meet Japanese standards
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnConfigJapan(uint16_t unusedParamButMandatory) {
  setSystemFlag(FLAG_DECIMP);
  groupingGap = 3;
  setSystemFlag(FLAG_TDM24); // time format = 24H
  clearSystemFlag(FLAG_MDY); // date format
  clearSystemFlag(FLAG_DMY); // date format
  setSystemFlag(FLAG_YMD);   // date format
  firstGregorianDay = 1873;  // JDN of the first day in the Gregorian calendar
}



/********************************************//**
 * \brief Sets the calc to meet English standards
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnConfigUk(uint16_t unusedParamButMandatory) {
  setSystemFlag(FLAG_DECIMP);
  groupingGap = 3;
  clearSystemFlag(FLAG_TDM24); // time format = 12H
  clearSystemFlag(FLAG_MDY);   // date format
  clearSystemFlag(FLAG_YMD);   // date format
  setSystemFlag(FLAG_DMY);     // date format
  firstGregorianDay = 1752;    // JDN of the first day in the Gregorian calendar
}



/********************************************//**
 * \brief Sets the calc to meet American standards
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnConfigUsa(uint16_t unusedParamButMandatory) {
  setSystemFlag(FLAG_DECIMP);
  groupingGap = 3;
  clearSystemFlag(FLAG_TDM24); // time format = 12H
  clearSystemFlag(FLAG_YMD);   // date format
  clearSystemFlag(FLAG_DMY);   // date format
  setSystemFlag(FLAG_MDY);     // date format
  firstGregorianDay = 1752;    // JDN of the first day in the Gregorian calendar
}



/********************************************//**
 * \brief Sets the integer mode
 *
 * \param[in] mode uint16_t Integer mode
 * \return void
 ***********************************************/
void fnIntegerMode(uint16_t mode) {
  shortIntegerMode = mode;
  
  fnRefreshRadioState(RB_IM, mode);                            //dr

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
 }



/********************************************//**
 * \brief Displays the version of this software
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnVersion(uint16_t unusedParamButMandatory) {
  temporaryInformation = TI_VERSION;
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

  if(shortIntegerWordSize == 64) {
    shortIntegerMask    = -1;
  }
  else {
    shortIntegerMask    = ((uint64_t)1 << shortIntegerWordSize) - 1;
  }

  shortIntegerSignBit = (uint64_t)1 << (shortIntegerWordSize - 1);
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

  fnRefreshRadioState(RB_WS, WS);                              //dr

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
}



/********************************************//**
 * \brief Sets X to the battery voltage
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnBatteryVoltage(uint16_t unusedParamButMandatory) {
  real_t value;

  liftStack();

  #ifdef PC_BUILD
    int32ToReal(3100, &value);
  #endif

  #ifdef DMCP_BUILD
    int32ToReal(get_vbat(), &value);
  #endif

  realDivide(&value, const_1000, &value, &ctxtReal39);
  realToReal34(&value, REGISTER_REAL34_DATA(REGISTER_X));
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
}



/********************************************//**
 * \brief Sets the rounding mode
 *
 * \param[in] RM uint16_t Rounding mode
 * \return void
 ***********************************************/
void fnRoundingMode(uint16_t RM) {
  roundingMode = RM;

  if(RM == 0) ctxtReal34.round = DEC_ROUND_HALF_EVEN;
  else if(RM == 1) ctxtReal34.round = DEC_ROUND_HALF_UP;
  else if(RM == 2) ctxtReal34.round = DEC_ROUND_HALF_DOWN;
  else if(RM == 3) ctxtReal34.round = DEC_ROUND_UP;
  else if(RM == 4) ctxtReal34.round = DEC_ROUND_DOWN;
  else if(RM == 5) ctxtReal34.round = DEC_ROUND_CEILING;
  else if(RM == 6) ctxtReal34.round = DEC_ROUND_FLOOR;
  else {
    sprintf(errorMessage, "In function fnRoundingMode: %d is an unexpected value for RM! Must be from 0 to 6", RM);
    displayBugScreen(errorMessage);
  }
}



/********************************************//**
 * \brief Sets the angular mode
 *
 * \param[in] am uint16_t Angular mode
 * \return void
 ***********************************************/
void fnAngularMode(uint16_t am) {
  currentAngularMode = am;

  fnRefreshRadioState(RB_AM, am);                              //dr

}



void fnFractionType(uint16_t unusedParamButMandatory) {
  if(getSystemFlag(FLAG_FRACT)) {
    flipSystemFlag(FLAG_PROPFR);
  }
  else {
    setSystemFlag(FLAG_FRACT);
  }
}



void setConfirmationMode(void (*func)(uint16_t)) {
  previousCalcMode = calcMode;
  cursorEnabled = false;
  calcMode = CM_CONFIRMATION;
  clearSystemFlag(FLAG_ALPHA);
  confirmedFunction = func;
  temporaryInformation = TI_ARE_YOU_SURE;
}



void fnRange(uint16_t unusedParamButMandatory) {
  longInteger_t longInt;

  if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
    convertLongIntegerRegisterToLongInteger(REGISTER_X, longInt);
  }
  else if(getRegisterDataType(REGISTER_X) == dtReal34) {
    convertReal34ToLongInteger(REGISTER_REAL34_DATA(REGISTER_X), longInt, DEC_ROUND_DOWN);
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot use %s for setting RANGE", getRegisterDataTypeName(REGISTER_X, true, true));
      moreInfoOnError("In function fnRange:", errorMessage, NULL, NULL);
    #endif
    return;
  }

  longIntegerSetPositiveSign(longInt);

  if(longIntegerCompareInt(longInt, 6145) > 0) {
    exponentLimit = 6145;
  }
  else if(longIntegerCompareInt(longInt, 99) < 0) {
    exponentLimit = 99;
  }
  else {
    exponentLimit = (int16_t)(longInt->_mp_d[0]); // OK for 32 and 64 bit limbs
  }

  longIntegerFree(longInt);
}



void fnGetRange(uint16_t unusedParamButMandatory) {
  longInteger_t range;

  liftStack();

  longIntegerInit(range);
  uIntToLongInteger(exponentLimit, range);
  convertLongIntegerToLongIntegerRegister(range, REGISTER_X);
  longIntegerFree(range);
}



void fnClAll(uint16_t confirmation) {
  if(confirmation == NOT_CONFIRMED) {
    setConfirmationMode(fnClAll);
  }
  else {
    calcRegister_t regist;

    fnClPAll(CONFIRMED);  // Clears all the programs
    fnClSigma(CONFIRMED); // Clears and releases the memory of all statistical sums
    if(savedStatisticalSumsPointer != NULL) {
      freeWp43s(savedStatisticalSumsPointer, NUMBER_OF_STATISTICAL_SUMS * TO_BYTES(REAL_SIZE));
    }

    // Clear local registers
    allocateLocalRegisters(0);

    // Clear registers including stack, I, J, K and L
    for(regist=0; regist<FIRST_LOCAL_REGISTER; regist++) {
      clearRegister(regist);
    }

    // Clear saved stack registers
    for(regist=FIRST_SAVED_STACK_REGISTER; regist<=TEMP_REGISTER; regist++) {
      clearRegister(regist);
    }
    thereIsSomethingToUndo = false;

    // TODO: clear (or delete) named variables

    // Clear global flags
    for(int32_t sixteenFlags=0; sixteenFlags<7; sixteenFlags++) { // 7 times uint16_t = 112 flags
      globalFlags[sixteenFlags] = 0;
    }

    temporaryInformation = TI_NO_INFO;
  }
}



void addTestPrograms(void) {
  uint8_t *stepAddress;
  uint32_t numberOfBytesForTheTestPrograms = 8192; // Multiple of 4

  resizeProgramMemory(TO_BLOCKS(numberOfBytesForTheTestPrograms));
  currentProgramMemoryPointer = programMemoryPointer;
  firstDisplayedStepPointer   = programMemoryPointer;
  firstDisplayedStep          = 0;
  stepAddress = programMemoryPointer;

  { // Prime number checker
    // 0
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 5; // String length
    *(stepAddress++) = 'P';
    *(stepAddress++) = 'r';
    *(stepAddress++) = 'i';
    *(stepAddress++) = 'm';
    *(stepAddress++) = 'e';

    // 1
    *(stepAddress++) = ITM_MAGNITUDE;

    // 2
    *(stepAddress++) = ITM_IP;

    // 3
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 0;

    // 4
    *(stepAddress++) = ITM_SQUAREROOTX;

    // 5
    *(stepAddress++) = ITM_IP;

    // 6
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 2;

    // 7
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    // 8
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 1;

    // 9
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 10
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    // 11
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 12
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    // 13
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 14
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    // 15
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 16
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '4'; // THE string

    // 17
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 18
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 2;

    // 19
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    // 20
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 21
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '4'; // THE string

    // 22
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 23
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    // 24
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 25
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '4'; // THE string

    // 26
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 27
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '6'; // THE string

    // 28
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 29
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    // 30
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 31
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '6'; // THE string

    // 32
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 33
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '4'; // THE string

    // 34
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 35
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    // 36
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 37
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '4'; // THE string

    // 38
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 39
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '6'; // THE string

    // 40
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 41
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '6'; // THE string

    // 42
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 43
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    // 44
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 45
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '6'; // THE string

    // 46
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 47
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '4'; // THE string

    // 48
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 49
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    // 50
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 51
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '6'; // THE string

    // 52
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 53
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '4'; // THE string

    // 54
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 55
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '6'; // THE string

    // 56
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 57
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '8'; // THE string

    // 58
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 59
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '4'; // THE string

    // 60
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 61
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    // 62
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 63
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '4'; // THE string

    // 64
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 65
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    // 66
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 67
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '4'; // THE string

    // 68
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 69
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '8'; // THE string

    // 70
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 71
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '6'; // THE string

    // 72
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 73
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '4'; // THE string

    // 74
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 75
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '6'; // THE string

    // 76
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 77
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    // 78
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 79
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '4'; // THE string

    // 80
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 81
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '6'; // THE string

    // 82
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 83
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    // 84
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 85
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '6'; // THE string

    // 86
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 87
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '6'; // THE string

    // 88
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 89
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '4'; // THE string

    // 90
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 91
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    // 92
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 93
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '4'; // THE string

    // 94
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 95
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '6'; // THE string

    // 96
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 97
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    // 98
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 99
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '6'; // THE string

    // 100
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 101
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '4'; // THE string

    // 102
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 103
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    // 104
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 105
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '4'; // THE string

    // 106
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 107
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    // 108
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 109
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 2; // String length
    *(stepAddress++) = '1'; // THE string
    *(stepAddress++) = '0'; // THE string

    // 110
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 111
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    // 112
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 113
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 2; // String length
    *(stepAddress++) = '1'; // THE string
    *(stepAddress++) = '0'; // THE string

    // 114
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 115
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 2;

    // 116
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 1;

    // 117
    *(stepAddress++) = ITM_XLT;
    *(stepAddress++) = REGISTER_Y;

    // 118
    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 2;

    // 119
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 0;

    // 120
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 0;

    // 121
    *(stepAddress++) = (ITM_STOP >> 8) | 0x80;
    *(stepAddress++) =  ITM_STOP       & 0xff;

    // 122
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 9;

    // 123
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 1;

    // 124
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 0;

    // 125
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 1;

    // 126
    *(stepAddress++) = ITM_MOD;

    // 127
    *(stepAddress++) = ITM_XNE;
    *(stepAddress++) = VALUE_0;

    // 128
    *(stepAddress++) = ITM_RTN;

    // 129
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 0;

    // 130
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 1;

    // 131
    *(stepAddress++) = (ITM_END >> 8) | 0x80;
    *(stepAddress++) =  ITM_END       & 0xff;

    #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
      printf("Prime : %u bytes\n", (uint32_t)(stepAddress - programMemoryPointer));
    #endif
  }

  { // Bairstow polynomial root finder
    // 132
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 7; // String length
    *(stepAddress++) = 'B';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'i';
    *(stepAddress++) = 'r';
    *(stepAddress++) = 's';
    *(stepAddress++) = 't';
    *(stepAddress++) = 'o';

    // 133
    *(stepAddress++) = (ITM_ALL >> 8) | 0x80;
    *(stepAddress++) =  ITM_ALL       & 0xff;
    *(stepAddress++) = 0;

    // 134
    *(stepAddress++) = (ITM_CLREGS >> 8) | 0x80;
    *(stepAddress++) =  ITM_CLREGS       & 0xff;

    // 135
    *(stepAddress++) = (ITM_CLSTK >> 8) | 0x80;
    *(stepAddress++) =  ITM_CLSTK       & 0xff;

    // 136
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_LABEL_VARIABLE;
    *(stepAddress++) = 19; // String length
    *(stepAddress++) = 'P'; // THE string
    *(stepAddress++) = 'o';
    *(stepAddress++) = 'l';
    *(stepAddress++) = 'y';
    *(stepAddress++) = 'n';
    *(stepAddress++) = 'o';
    *(stepAddress++) = 'm';
    *(stepAddress++) = 'i';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'l';
    *(stepAddress++) = ' ';
    *(stepAddress++) = 'd';
    *(stepAddress++) = 'e';
    *(stepAddress++) = 'g';
    *(stepAddress++) = 'r';
    *(stepAddress++) = 'e';
    *(stepAddress++) = 'e';
    *(stepAddress++) = ' ';
    *(stepAddress++) = '?';

    // 137
    *(stepAddress++) = (ITM_STOP >> 8) | 0x80;
    *(stepAddress++) =  ITM_STOP       & 0xff;

    // 138
    *(stepAddress++) = ITM_MAGNITUDE;

    // 139
    *(stepAddress++) = ITM_IP;

    // 140
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 90;

    // 141
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 91;

    // 142
    *(stepAddress++) = (ITM_CLSTK >> 8) | 0x80;
    *(stepAddress++) =  ITM_CLSTK       & 0xff;

    // 143
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 91;

    // 144
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 3; // String length
    *(stepAddress++) = '1'; // THE string
    *(stepAddress++) = 'e';
    *(stepAddress++) = '3';

    // 145
    *(stepAddress++) = ITM_DIV;

    // 146
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 92;

    // 147
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 100 + 'A' - 'A'; // A

    // 148
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_LABEL_VARIABLE;
    *(stepAddress++) = 2; // String length
    *(stepAddress++) = 'x'; // THE string
    *(stepAddress++) = '^';

    // 149
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 91;

    // 150
    *(stepAddress++) = ITM_ADD;

    // 151
    *(stepAddress++) = (ITM_STOP >> 8) | 0x80;
    *(stepAddress++) =  ITM_STOP       & 0xff;

    // 152
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 93;

    // 153
    *(stepAddress++) = (ITM_CLSTK >> 8) | 0x80;
    *(stepAddress++) =  ITM_CLSTK       & 0xff;

    // 154
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    // 155
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 93;

    // 156
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 93;

    // 157
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 92;

    // 158
    *(stepAddress++) = ITM_ISG;
    *(stepAddress++) = 92;

    // 159
    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 100 + 'A' - 'A'; // A

    // 160
    *(stepAddress++) = (ITM_CLSTK >> 8) | 0x80;
    *(stepAddress++) =  ITM_CLSTK       & 0xff;

    // 161
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    // 162
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 0;

    // 163
    *(stepAddress++) = ITM_XNE;
    *(stepAddress++) = REGISTER_Y;

    // 164
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 100 + 'B' - 'A'; // B

    // 165
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_LABEL_VARIABLE;
    *(stepAddress++) = 10; // String length
    *(stepAddress++) = 'T'; // THE string
    *(stepAddress++) = 'o';
    *(stepAddress++) = 'l';
    *(stepAddress++) = 'e';
    *(stepAddress++) = 'r';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'n';
    *(stepAddress++) = 'c';
    *(stepAddress++) = 'e';
    *(stepAddress++) = '?';

    // 166
    *(stepAddress++) = (ITM_STOP >> 8) | 0x80;
    *(stepAddress++) =  ITM_STOP       & 0xff;

    // 167
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 99;

    // 168
    *(stepAddress++) = (ITM_CLSTK >> 8) | 0x80;
    *(stepAddress++) =  ITM_CLSTK       & 0xff;

    // 169
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 10;

    // 170
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '3'; // THE string

    // 171
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 94;

    // 172
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 94;

    // 173
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 90;

    // 174
    *(stepAddress++) = ITM_XLT;
    *(stepAddress++) = REGISTER_Y;

    // 175
    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 2;

    // 176
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '0'; // THE string

    // 177
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 95;

    // 178
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 96;

    // 179
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 100 + 'D' - 'A'; // D

    // 180
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 100 + 'J' - 'A'; // J

    // 181
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 11;

    // 182
    *(stepAddress++) = (ITM_STOP >> 8) | 0x80;
    *(stepAddress++) =  ITM_STOP       & 0xff;

    // 183
    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 10;

    // 184
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 2;

    // 185
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    // 186
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 94;

    // 187
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 94;

    // 188
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 90;

    // 189
    *(stepAddress++) = ITM_XNE;
    *(stepAddress++) = REGISTER_Y;

    // 190
    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 1;

    // 191
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 1;

    // 192
    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 90;

    // 193
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 1;

    // 194
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 13;

    // 195
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 90;

    // 196
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_LABEL_VARIABLE;
    *(stepAddress++) = 9; // String length
    *(stepAddress++) = 'L'; // THE string
    *(stepAddress++) = 'a';
    *(stepAddress++) = 's';
    *(stepAddress++) = 't';
    *(stepAddress++) = ' ';
    *(stepAddress++) = 'r';
    *(stepAddress++) = 'o';
    *(stepAddress++) = 'o';
    *(stepAddress++) = 't';

    // 197
    *(stepAddress++) = ITM_RTN;

    // 198
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 100 + 'B' - 'A'; // B

    // 199
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    // 200
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 93;

    // 201
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 90;

    // 202
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 3; // String length
    *(stepAddress++) = '1'; // THE string
    *(stepAddress++) = 'e';
    *(stepAddress++) = '3';

    // 203
    *(stepAddress++) = ITM_DIV;

    // 204
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 92;

    // 205
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 100 + 'C' - 'A'; // C

    // 206
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 0;

    // 207
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 93;

    // 208
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    // 209
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 93;

    // 210
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 92;

    // 211
    *(stepAddress++) = ITM_ISG;
    *(stepAddress++) = 92;

    // 212
    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 100 + 'C' - 'A'; // C

    // 213
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    // 214
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 0;

    // 215
    *(stepAddress++) = ITM_RTN;

    // 216
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 100 + 'D' - 'A'; // D

    // 217
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    // 218
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 97;

    // 219
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '0'; // THE string

    // 220
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 98;

    // 221
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 100 + 'I' - 'A'; // I

    // 222
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    // 223
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 91;

    // 224
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 90;

    // 225
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 3; // String length
    *(stepAddress++) = '1'; // THE string
    *(stepAddress++) = 'e';
    *(stepAddress++) = '3';

    // 226
    *(stepAddress++) = ITM_DIV;

    // 227
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    // 228
    *(stepAddress++) = ITM_ADD;

    // 229
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 89;

    // 230
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 2; // String length
    *(stepAddress++) = '3'; // THE string
    *(stepAddress++) = '0'; // THE string

    // 231
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 92;

    // 232
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 93;

    // 233
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 94;

    // 234
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    // 235
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 93;

    // 236
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 94;

    // 237
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 94;

    // 238
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '0'; // THE string

    // 239
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 92;

    // 240
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    // 241
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 93;

    // 242
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 100 + 'E' - 'A'; // E

    // 243
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 91;

    // 244
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 94;

    // 245
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 93;

    // 246
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 95;

    // 247
    *(stepAddress++) = ITM_MULT;

    // 248
    *(stepAddress++) = ITM_CHS;

    // 249
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 94;

    // 250
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 92;

    // 251
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 96;

    // 252
    *(stepAddress++) = ITM_MULT;

    // 253
    *(stepAddress++) = ITM_CHS;

    // 254
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 94;

    // 255
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    // 256
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 91;

    // 257
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 92;

    // 258
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 93;

    // 259
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 94;

    // 260
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 89;

    // 261
    *(stepAddress++) = ITM_ISG;
    *(stepAddress++) = 89;

    // 262
    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 100 + 'E' - 'A'; // E

    // 263
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 92;

    // 264
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 84;

    // 265
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 93;

    // 266
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 85;

    // 267
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 2; // String length
    *(stepAddress++) = '3'; // THE string
    *(stepAddress++) = '0'; // THE string

    // 268
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 91;

    // 269
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 90;

    // 270
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 3; // String length
    *(stepAddress++) = '1'; // THE string
    *(stepAddress++) = 'e';
    *(stepAddress++) = '3';

    // 271
    *(stepAddress++) = ITM_DIV;

    // 272
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    // 273
    *(stepAddress++) = ITM_ADD;

    // 274
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 89;

    // 275
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 2; // String length
    *(stepAddress++) = '6'; // THE string
    *(stepAddress++) = '0'; // THE string

    // 276
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 92;

    // 277
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 93;

    // 278
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 94;

    // 279
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    // 280
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 93;

    // 281
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 94;

    // 282
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 94;

    // 283
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '0'; // THE string

    // 284
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 92;

    // 285
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '0'; // THE string

    // 286
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 93;

    // 287
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 100 + 'F' - 'A'; // F

    // 288
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 91;

    // 289
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 94;

    // 290
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 93;

    // 291
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 95;

    // 292
    *(stepAddress++) = ITM_MULT;

    // 293
    *(stepAddress++) = ITM_CHS;

    // 294
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 94;

    // 295
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 92;

    // 296
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 96;

    // 297
    *(stepAddress++) = ITM_MULT;

    // 298
    *(stepAddress++) = ITM_CHS;

    // 299
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 94;

    // 300
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    // 301
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 91;

    // 302
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 92;

    // 303
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 93;

    // 304
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 94;

    // 305
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 89;

    // 306
    *(stepAddress++) = ITM_ISG;
    *(stepAddress++) = 89;

    // 307
    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 100 + 'F' - 'A'; // F

    // 308
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 92;

    // 309
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 92;

    // 310
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 93;

    // 311
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 93;

    // 312
    *(stepAddress++) = (ITM_CLSTK >> 8) | 0x80;
    *(stepAddress++) =  ITM_CLSTK       & 0xff;

    // 313
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 95;

    // 314
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 93;

    // 315
    *(stepAddress++) = ITM_MULT;

    // 316
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 96;

    // 317
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 92;

    // 318
    *(stepAddress++) = ITM_MULT;

    // 319
    *(stepAddress++) = ITM_ADD;

    // 320
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 92;

    // 321
    *(stepAddress++) = ITM_MULT;

    // 322
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 93;

    // 323
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 93;

    // 324
    *(stepAddress++) = ITM_MULT;

    // 325
    *(stepAddress++) = ITM_ADD;

    // 326
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 88;

    // 327
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 84;

    // 328
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 93;

    // 329
    *(stepAddress++) = ITM_MULT;

    // 330
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 85;

    // 331
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 92;

    // 332
    *(stepAddress++) = ITM_MULT;

    // 333
    *(stepAddress++) = ITM_SUB;

    // 334
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 88;

    // 335
    *(stepAddress++) = ITM_DIV;

    // 336
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 87;

    // 337
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 95;

    // 338
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 93;

    // 339
    *(stepAddress++) = ITM_MULT;

    // 340
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 96;

    // 341
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 92;

    // 342
    *(stepAddress++) = ITM_MULT;

    // 343
    *(stepAddress++) = ITM_ADD;

    // 344
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 84;

    // 345
    *(stepAddress++) = ITM_MULT;

    // 346
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 85;

    // 347
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 93;

    // 348
    *(stepAddress++) = ITM_MULT;

    // 349
    *(stepAddress++) = ITM_ADD;

    // 350
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 88;

    // 351
    *(stepAddress++) = ITM_DIV;

    // 352
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 86;

    // 353
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 87;

    // 354
    *(stepAddress++) = ITM_MAGNITUDE;

    // 355
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = REGISTER_X;

    // 356
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 86;

    // 357
    *(stepAddress++) = ITM_MAGNITUDE;

    // 358
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = REGISTER_Y;

    // 359
    *(stepAddress++) = ITM_XGT;
    *(stepAddress++) = REGISTER_Y;

    // 360
    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 100 + 'G' - 'A'; // G

    // 361
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 87;

    // 362
    *(stepAddress++) = ITM_MAGNITUDE;

    // 363
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 97;

    // 364
    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 100 + 'H' - 'A'; // H

    // 365
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 100 + 'G' - 'A'; // G

    // 366
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 86;

    // 367
    *(stepAddress++) = ITM_MAGNITUDE;

    // 368
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 97;

    // 369
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 100 + 'H' - 'A'; // H

    // 370
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 87;

    // 371
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 95;

    // 372
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 86;

    // 373
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 96;

    // 374
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    // 375
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 98;

    // 376
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 3; // String length
    *(stepAddress++) = '1'; // THE string
    *(stepAddress++) = 'e';
    *(stepAddress++) = '4';

    // 377
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = REGISTER_X;

    // 378
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 98;

    // 379
    *(stepAddress++) = ITM_XGT;
    *(stepAddress++) = REGISTER_Y;

    // 380
    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 4;

    // 381
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 99;

    // 382
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 97;

    // 383
    *(stepAddress++) = ITM_XGT;
    *(stepAddress++) = REGISTER_Y;

    // 384
    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 100 + 'I' - 'A';

    // 385
    *(stepAddress++) = ITM_RTN;

    // 386
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 100 + 'J' - 'A'; // J

    // 387
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '0'; // THE string

    // 388
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 91;

    // 389
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 2; // String length
    *(stepAddress++) = '3'; // THE string
    *(stepAddress++) = '1'; // THE string

    // 390
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 92;

    // 391
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 90;

    // 392
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    // 393
    *(stepAddress++) = ITM_SUB;

    // 394
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 93;

    // 395
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 3; // String length
    *(stepAddress++) = '1'; // THE string
    *(stepAddress++) = 'e';
    *(stepAddress++) = '3';

    // 396
    *(stepAddress++) = ITM_DIV;

    // 397
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 94;

    // 398
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 88;

    // 399
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 92;

    // 400
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 91;

    // 401
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    // 402
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 91;

    // 403
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 92;

    // 404
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 94;

    // 405
    *(stepAddress++) = ITM_ISG;
    *(stepAddress++) = 94;

    // 406
    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 88;

    // 407
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 90;

    // 408
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    // 409
    *(stepAddress++) = ITM_SUB;

    // 410
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 90;

    // 411
    *(stepAddress++) = ITM_RTN;

    // 412
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 11;

    // 413
    *(stepAddress++) = (ITM_CLSTK >> 8) | 0x80;
    *(stepAddress++) =  ITM_CLSTK       & 0xff;

    // 414
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 95;

    // 415
    *(stepAddress++) = ITM_CHS;

    // 416
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    // 417
    *(stepAddress++) = ITM_DIV;

    // 418
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 91;

    // 419
    *(stepAddress++) = ITM_SQUARE;

    // 420
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 96;

    // 421
    *(stepAddress++) = ITM_SUB;

    // 422
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 92;

    // 423
    *(stepAddress++) = (ITM_CLSTK >> 8) | 0x80;
    *(stepAddress++) =  ITM_CLSTK       & 0xff;

    // 424
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 91;

    // 425
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 92;

    // 426
    *(stepAddress++) = ITM_SQUAREROOTX;

    // 427
    *(stepAddress++) = ITM_ADD;

    // 428
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 91;

    // 429
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 92;

    // 430
    *(stepAddress++) = ITM_SQUAREROOTX;

    // 431
    *(stepAddress++) = ITM_SUB;

    // 432
    *(stepAddress++) = ITM_RTN;

    // 433
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 12;

    // 434
    *(stepAddress++) = (ITM_CLSTK >> 8) | 0x80;
    *(stepAddress++) =  ITM_CLSTK       & 0xff;

    // 435
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_LABEL_VARIABLE;
    *(stepAddress++) = 12; // String length
    *(stepAddress++) = '.'; // THE string
    *(stepAddress++) = '.';
    *(stepAddress++) = '.';
    *(stepAddress++) = ' ';
    *(stepAddress++) = 'c';
    *(stepAddress++) = 'o';
    *(stepAddress++) = 'n';
    *(stepAddress++) = 't';
    *(stepAddress++) = 'i';
    *(stepAddress++) = 'n';
    *(stepAddress++) = 'u';
    *(stepAddress++) = 'e';

    // 436
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 1;

    // 437
    *(stepAddress++) = ITM_CHS;

    // 438
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    // 439
    *(stepAddress++) = ITM_DIV;

    // 440
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 91;

    // 441
    *(stepAddress++) = ITM_SQUARE;

    // 442
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 2;

    // 443
    *(stepAddress++) = ITM_SUB;

    // 444
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 92;

    // 445
    *(stepAddress++) = (ITM_CLSTK >> 8) | 0x80;
    *(stepAddress++) =  ITM_CLSTK       & 0xff;

    // 446
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 91;

    // 447
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 92;

    // 448
    *(stepAddress++) = ITM_SQUAREROOTX;

    // 449
    *(stepAddress++) = ITM_ADD;

    // 450
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 91;

    // 451
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 92;

    // 452
    *(stepAddress++) = ITM_SQUAREROOTX;

    // 453
    *(stepAddress++) = ITM_SUB;

    // 454
    *(stepAddress++) = ITM_RTN;

    // 455
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 13;

    // 456
    *(stepAddress++) = (ITM_CLSTK >> 8) | 0x80;
    *(stepAddress++) =  ITM_CLSTK       & 0xff;

    // 457
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_LABEL_VARIABLE;
    *(stepAddress++) = 12; // String length
    *(stepAddress++) = '.'; // THE string
    *(stepAddress++) = '.';
    *(stepAddress++) = '.';
    *(stepAddress++) = ' ';
    *(stepAddress++) = 'c';
    *(stepAddress++) = 'o';
    *(stepAddress++) = 'n';
    *(stepAddress++) = 't';
    *(stepAddress++) = 'i';
    *(stepAddress++) = 'n';
    *(stepAddress++) = 'u';
    *(stepAddress++) = 'e';

    // 458
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 1;

    // 459
    *(stepAddress++) = ITM_CHS;

    // 460
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 91;

    // 461
    *(stepAddress++) = (ITM_CLSTK >> 8) | 0x80;
    *(stepAddress++) =  ITM_CLSTK       & 0xff;

    // 462
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 91;

    // 463
    *(stepAddress++) = ITM_RTN;

    // 464
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 4;

    // 465
    *(stepAddress++) = (ITM_CLSTK >> 8) | 0x80;
    *(stepAddress++) =  ITM_CLSTK       & 0xff;

    // 466
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_LABEL_VARIABLE;
    *(stepAddress++) = 11; // String length
    *(stepAddress++) = 'E'; // THE string
    *(stepAddress++) = 'r';
    *(stepAddress++) = 'r';
    *(stepAddress++) = 'o';
    *(stepAddress++) = 'r';
    *(stepAddress++) = ' ';
    *(stepAddress++) = 'm';
    *(stepAddress++) = '>';
    *(stepAddress++) = '8';
    *(stepAddress++) = '0';
    *(stepAddress++) = '!';

    // 467
    *(stepAddress++) = ITM_RTN;

    // 468
    *(stepAddress++) = (ITM_END >> 8) | 0x80;
    *(stepAddress++) =  ITM_END       & 0xff;

    #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
      printf("Prime + Bairstow : %u bytes\n", (uint32_t)(stepAddress - programMemoryPointer));
    #endif
  }

  { // Speed test. See: https://forum.swissmicros.com/viewtopic.php?p=17308
    // 469
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 5; // String length
    *(stepAddress++) = 'S';
    *(stepAddress++) = 'p';
    *(stepAddress++) = 'e';
    *(stepAddress++) = 'e';
    *(stepAddress++) = 'd';

    // 470
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 10;

    // 471
    *(stepAddress++) = (ITM_TICKS >> 8) | 0x80;
    *(stepAddress++) =  ITM_TICKS       & 0xff;

    // 472
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 0;

    // 473
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '0'; // THE string

    // 474
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 11;

    // 475
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    // 476
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 12;

    // 477
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 1;

    // 478
    *(stepAddress++) = ITM_arctan;

    // 479
    *(stepAddress++) = ITM_sin;

    // 480
    *(stepAddress++) = ITM_EXP;

    // 481
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '3'; // THE string

    // 482
    *(stepAddress++) = ITM_1ONX;

    // 483
    *(stepAddress++) = ITM_YX;

    // 484
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 11;

    // 485
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 12;

    // 486
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    // 487
    *(stepAddress++) = ITM_ADD;

    // 488
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 12;

    // 489
    *(stepAddress++) = ITM_DSE;
    *(stepAddress++) = 10;

    // 490
    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 1;

    // 491
    *(stepAddress++) = (ITM_TICKS >> 8) | 0x80;
    *(stepAddress++) =  ITM_TICKS       & 0xff;

    // 492
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 0;

    // 493
    *(stepAddress++) = ITM_SUB;

    // 494
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 2; // String length
    *(stepAddress++) = '1'; // THE string
    *(stepAddress++) = '0';

    // 495
    *(stepAddress++) = ITM_DIV;

    // 496
    *(stepAddress++) = (ITM_END >> 8) | 0x80;
    *(stepAddress++) =  ITM_END       & 0xff;

    #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
      printf("Prime + Bairstow + Speed : %u bytes\n", (uint32_t)(stepAddress - programMemoryPointer));
    #endif
  }

  { // Factorial: the recursive way
    // 497
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 4; // String length
    *(stepAddress++) = 'F';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'c';
    *(stepAddress++) = 't';

    // 498
    *(stepAddress++) = ITM_IP;

    // 499
    *(stepAddress++) = ITM_XGT;
    *(stepAddress++) = VALUE_1;

    // 500
    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 0;

    // 501
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    // 502
    *(stepAddress++) = ITM_RTN;

    // 503
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 0;

    // 504
    *(stepAddress++) = (ITM_LocR >> 8) | 0x80;
    *(stepAddress++) =  ITM_LocR       & 0xff;
    *(stepAddress++) = 1;

    // 505
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 112; // .00

    // 506
    *(stepAddress++) = ITM_DEC;
    *(stepAddress++) = REGISTER_X;

    // 507
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 4; // String length
    *(stepAddress++) = 'F';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'c';
    *(stepAddress++) = 't';

    // 508
    *(stepAddress++) = ITM_RCLMULT;
    *(stepAddress++) = 112; // .00

    // 509
    *(stepAddress++) = ITM_RTN;

    // 510
    *(stepAddress++) = (ITM_END >> 8) | 0x80;
    *(stepAddress++) =  ITM_END       & 0xff;

    #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
      printf("Prime + Bairstow + Speed + Fact : %u bytes\n", (uint32_t)(stepAddress - programMemoryPointer));
    #endif
  }

  { // All OP's
    //currentProgramMemoryPointer = stepAddress;
    // 511
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 7; // String length
    *(stepAddress++) = 'A';
    *(stepAddress++) = 'l';
    *(stepAddress++) = 'l';
    *(stepAddress++) = 'O';
    *(stepAddress++) = 'p';
    *(stepAddress++) = '\'';
    *(stepAddress++) = 's';

    // 512
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 0;

    // 513
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 99;

    // 514
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 'A' - 'A' + 100; // A

    // 515
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 'J' - 'A' + 100; // J

    // 516
    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 0;

    // 517
    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 99;

    // 518
    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 'A' - 'A' + 100; // A

    // 519
    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 'J' - 'A' + 100; // J

    // 520
    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 5;
    *(stepAddress++) = 'L';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'b';
    *(stepAddress++) = 'e';
    *(stepAddress++) = 'l';

    // 521
    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    // 522
    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    // 523
    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    // 524
    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    // 525
    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 526
    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 527
    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 528
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 0;

    // 529
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 99;

    // 530
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 'A' - 'A' + 100; // A

    // 531
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 'J' - 'A' + 100; // J

    // 532
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 5;
    *(stepAddress++) = 'L';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'b';
    *(stepAddress++) = 'e';
    *(stepAddress++) = 'l';

    // 533
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    // 534
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    // 535
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    // 536
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    // 537
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 538
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 539
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 540
    *(stepAddress++) = ITM_RTN;

    // 541
    *(stepAddress++) = ITM_ISE;
    *(stepAddress++) = 0;

    // 542
    *(stepAddress++) = ITM_ISE;
    *(stepAddress++) = 99;

    // 543
    *(stepAddress++) = ITM_ISE;
    *(stepAddress++) = REGISTER_X;

    // 544
    *(stepAddress++) = ITM_ISE;
    *(stepAddress++) = REGISTER_K;

    // 545
    *(stepAddress++) = ITM_ISE;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 546
    *(stepAddress++) = ITM_ISE;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 547
    *(stepAddress++) = ITM_ISE;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 548
    *(stepAddress++) = ITM_ISE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    // 549
    *(stepAddress++) = ITM_ISE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    // 550
    *(stepAddress++) = ITM_ISE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    // 551
    *(stepAddress++) = ITM_ISE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    // 552
    *(stepAddress++) = ITM_ISE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 553
    *(stepAddress++) = ITM_ISE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 554
    *(stepAddress++) = ITM_ISE;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 555
    *(stepAddress++) = ITM_ISG;
    *(stepAddress++) = 0;

    // 556
    *(stepAddress++) = ITM_ISG;
    *(stepAddress++) = 99;

    // 557
    *(stepAddress++) = ITM_ISG;
    *(stepAddress++) = REGISTER_X;

    // 558
    *(stepAddress++) = ITM_ISG;
    *(stepAddress++) = REGISTER_K;

    // 559
    *(stepAddress++) = ITM_ISG;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 560
    *(stepAddress++) = ITM_ISG;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 561
    *(stepAddress++) = ITM_ISG;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 562
    *(stepAddress++) = ITM_ISG;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    // 563
    *(stepAddress++) = ITM_ISG;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    // 564
    *(stepAddress++) = ITM_ISG;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    // 565
    *(stepAddress++) = ITM_ISG;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    // 566
    *(stepAddress++) = ITM_ISG;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 567
    *(stepAddress++) = ITM_ISG;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 568
    *(stepAddress++) = ITM_ISG;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 569
    *(stepAddress++) = ITM_ISZ;
    *(stepAddress++) = 0;

    // 570
    *(stepAddress++) = ITM_ISZ;
    *(stepAddress++) = 99;

    // 571
    *(stepAddress++) = ITM_ISZ;
    *(stepAddress++) = REGISTER_X;

    // 572
    *(stepAddress++) = ITM_ISZ;
    *(stepAddress++) = REGISTER_K;

    // 573
    *(stepAddress++) = ITM_ISZ;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 574
    *(stepAddress++) = ITM_ISZ;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 575
    *(stepAddress++) = ITM_ISZ;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 576
    *(stepAddress++) = ITM_ISZ;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    // 577
    *(stepAddress++) = ITM_ISZ;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    // 578
    *(stepAddress++) = ITM_ISZ;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    // 579
    *(stepAddress++) = ITM_ISZ;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    // 580
    *(stepAddress++) = ITM_ISZ;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 581
    *(stepAddress++) = ITM_ISZ;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 582
    *(stepAddress++) = ITM_ISZ;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 583
    *(stepAddress++) = ITM_DSE;
    *(stepAddress++) = 0;

    // 584
    *(stepAddress++) = ITM_DSE;
    *(stepAddress++) = 99;

    // 585
    *(stepAddress++) = ITM_DSE;
    *(stepAddress++) = REGISTER_X;

    // 586
    *(stepAddress++) = ITM_DSE;
    *(stepAddress++) = REGISTER_K;

    // 587
    *(stepAddress++) = ITM_DSE;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 588
    *(stepAddress++) = ITM_DSE;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 589
    *(stepAddress++) = ITM_DSE;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 590
    *(stepAddress++) = ITM_DSE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    // 591
    *(stepAddress++) = ITM_DSE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    // 592
    *(stepAddress++) = ITM_DSE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    // 593
    *(stepAddress++) = ITM_DSE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    // 594
    *(stepAddress++) = ITM_DSE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 595
    *(stepAddress++) = ITM_DSE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 596
    *(stepAddress++) = ITM_DSE;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 597
    *(stepAddress++) = ITM_DSL;
    *(stepAddress++) = 0;

    // 598
    *(stepAddress++) = ITM_DSL;
    *(stepAddress++) = 99;

    // 599
    *(stepAddress++) = ITM_DSL;
    *(stepAddress++) = REGISTER_X;

    // 600
    *(stepAddress++) = ITM_DSL;
    *(stepAddress++) = REGISTER_K;

    // 601
    *(stepAddress++) = ITM_DSL;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 602
    *(stepAddress++) = ITM_DSL;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 603
    *(stepAddress++) = ITM_DSL;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 604
    *(stepAddress++) = ITM_DSL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    // 605
    *(stepAddress++) = ITM_DSL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    // 606
    *(stepAddress++) = ITM_DSL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    // 607
    *(stepAddress++) = ITM_DSL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    // 608
    *(stepAddress++) = ITM_DSL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 609
    *(stepAddress++) = ITM_DSL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 610
    *(stepAddress++) = ITM_DSL;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 611
    *(stepAddress++) = ITM_DSZ;
    *(stepAddress++) = 0;

    // 612
    *(stepAddress++) = ITM_DSZ;
    *(stepAddress++) = 99;

    // 613
    *(stepAddress++) = ITM_DSZ;
    *(stepAddress++) = REGISTER_X;

    // 614
    *(stepAddress++) = ITM_DSZ;
    *(stepAddress++) = REGISTER_K;

    // 615
    *(stepAddress++) = ITM_DSZ;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 616
    *(stepAddress++) = ITM_DSZ;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 617
    *(stepAddress++) = ITM_DSZ;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 618
    *(stepAddress++) = ITM_DSZ;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    // 619
    *(stepAddress++) = ITM_DSZ;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    // 620
    *(stepAddress++) = ITM_DSZ;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    // 621
    *(stepAddress++) = ITM_DSZ;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    // 622
    *(stepAddress++) = ITM_DSZ;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 623
    *(stepAddress++) = ITM_DSZ;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 624
    *(stepAddress++) = ITM_DSZ;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 625
    *(stepAddress++) = ITM_XEQU;
    *(stepAddress++) = 0;

    // 626
    *(stepAddress++) = ITM_XEQU;
    *(stepAddress++) = 99;

    // 627
    *(stepAddress++) = ITM_XEQU;
    *(stepAddress++) = REGISTER_X;

    // 628
    *(stepAddress++) = ITM_XEQU;
    *(stepAddress++) = REGISTER_K;

    // 629
    *(stepAddress++) = ITM_XEQU;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 630
    *(stepAddress++) = ITM_XEQU;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 631
    *(stepAddress++) = ITM_XEQU;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 632
    *(stepAddress++) = ITM_XEQU;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    // 633
    *(stepAddress++) = ITM_XEQU;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    // 634
    *(stepAddress++) = ITM_XEQU;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    // 635
    *(stepAddress++) = ITM_XEQU;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    // 636
    *(stepAddress++) = ITM_XEQU;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 637
    *(stepAddress++) = ITM_XEQU;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 638
    *(stepAddress++) = ITM_XEQU;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 639
    *(stepAddress++) = ITM_XNE;
    *(stepAddress++) = 0;

    // 640
    *(stepAddress++) = ITM_XNE;
    *(stepAddress++) = 99;

    // 641
    *(stepAddress++) = ITM_XNE;
    *(stepAddress++) = REGISTER_X;

    // 642
    *(stepAddress++) = ITM_XNE;
    *(stepAddress++) = REGISTER_K;

    // 643
    *(stepAddress++) = ITM_XNE;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 644
    *(stepAddress++) = ITM_XNE;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 645
    *(stepAddress++) = ITM_XNE;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 646
    *(stepAddress++) = ITM_XNE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    // 647
    *(stepAddress++) = ITM_XNE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    // 648
    *(stepAddress++) = ITM_XNE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    // 649
    *(stepAddress++) = ITM_XNE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    // 650
    *(stepAddress++) = ITM_XNE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 651
    *(stepAddress++) = ITM_XNE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 652
    *(stepAddress++) = ITM_XNE;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 653
    *(stepAddress++) = ITM_XEQUP0;

    // 654
    *(stepAddress++) = ITM_XEQUM0;

    // 655
    *(stepAddress++) = ITM_XAEQU;
    *(stepAddress++) = 0;

    // 656
    *(stepAddress++) = ITM_XAEQU;
    *(stepAddress++) = 99;

    // 657
    *(stepAddress++) = ITM_XAEQU;
    *(stepAddress++) = REGISTER_X;

    // 658
    *(stepAddress++) = ITM_XAEQU;
    *(stepAddress++) = REGISTER_K;

    // 659
    *(stepAddress++) = ITM_XAEQU;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 660
    *(stepAddress++) = ITM_XAEQU;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 661
    *(stepAddress++) = ITM_XAEQU;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 662
    *(stepAddress++) = ITM_XAEQU;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    // 663
    *(stepAddress++) = ITM_XAEQU;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    // 664
    *(stepAddress++) = ITM_XAEQU;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    // 665
    *(stepAddress++) = ITM_XAEQU;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    // 666
    *(stepAddress++) = ITM_XAEQU;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 667
    *(stepAddress++) = ITM_XAEQU;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 668
    *(stepAddress++) = ITM_XAEQU;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 669
    *(stepAddress++) = ITM_XLT;
    *(stepAddress++) = 0;

    // 670
    *(stepAddress++) = ITM_XLT;
    *(stepAddress++) = 99;

    // 671
    *(stepAddress++) = ITM_XLT;
    *(stepAddress++) = REGISTER_X;

    // 672
    *(stepAddress++) = ITM_XLT;
    *(stepAddress++) = REGISTER_K;

    // 673
    *(stepAddress++) = ITM_XLT;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 674
    *(stepAddress++) = ITM_XLT;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 675
    *(stepAddress++) = ITM_XLT;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 676
    *(stepAddress++) = ITM_XLT;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    // 677
    *(stepAddress++) = ITM_XLT;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    // 678
    *(stepAddress++) = ITM_XLT;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    // 679
    *(stepAddress++) = ITM_XLT;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    // 680
    *(stepAddress++) = ITM_XLT;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 681
    *(stepAddress++) = ITM_XLT;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 682
    *(stepAddress++) = ITM_XLT;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 683
    *(stepAddress++) = ITM_XLE;
    *(stepAddress++) = 0;

    // 684
    *(stepAddress++) = ITM_XLE;
    *(stepAddress++) = 99;

    // 685
    *(stepAddress++) = ITM_XLE;
    *(stepAddress++) = REGISTER_X;

    // 686
    *(stepAddress++) = ITM_XLE;
    *(stepAddress++) = REGISTER_K;

    // 687
    *(stepAddress++) = ITM_XLE;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 688
    *(stepAddress++) = ITM_XLE;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 689
    *(stepAddress++) = ITM_XLE;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 690
    *(stepAddress++) = ITM_XLE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    // 691
    *(stepAddress++) = ITM_XLE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    // 692
    *(stepAddress++) = ITM_XLE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    // 693
    *(stepAddress++) = ITM_XLE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    // 694
    *(stepAddress++) = ITM_XLE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 695
    *(stepAddress++) = ITM_XLE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 696
    *(stepAddress++) = ITM_XLE;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 697
    *(stepAddress++) = ITM_XGE;
    *(stepAddress++) = 0;

    // 698
    *(stepAddress++) = ITM_XGE;
    *(stepAddress++) = 99;

    // 699
    *(stepAddress++) = ITM_XGE;
    *(stepAddress++) = REGISTER_X;

    // 700
    *(stepAddress++) = ITM_XGE;
    *(stepAddress++) = REGISTER_K;

    // 701
    *(stepAddress++) = ITM_XGE;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 702
    *(stepAddress++) = ITM_XGE;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 703
    *(stepAddress++) = ITM_XGE;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 704
    *(stepAddress++) = ITM_XGE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    // 705
    *(stepAddress++) = ITM_XGE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    // 706
    *(stepAddress++) = ITM_XGE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    // 707
    *(stepAddress++) = ITM_XGE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    // 708
    *(stepAddress++) = ITM_XGE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 709
    *(stepAddress++) = ITM_XGE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 710
    *(stepAddress++) = ITM_XGE;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 711
    *(stepAddress++) = ITM_XGT;
    *(stepAddress++) = 0;

    // 712
    *(stepAddress++) = ITM_XGT;
    *(stepAddress++) = 99;

    // 713
    *(stepAddress++) = ITM_XGT;
    *(stepAddress++) = REGISTER_X;

    // 714
    *(stepAddress++) = ITM_XGT;
    *(stepAddress++) = REGISTER_K;

    // 715
    *(stepAddress++) = ITM_XGT;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 716
    *(stepAddress++) = ITM_XGT;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 717
    *(stepAddress++) = ITM_XGT;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 718
    *(stepAddress++) = ITM_XGT;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    // 719
    *(stepAddress++) = ITM_XGT;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    // 720
    *(stepAddress++) = ITM_XGT;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    // 721
    *(stepAddress++) = ITM_XGT;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    // 722
    *(stepAddress++) = ITM_XGT;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 723
    *(stepAddress++) = ITM_XGT;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 724
    *(stepAddress++) = ITM_XGT;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 725
    *(stepAddress++) = ITM_FC;
    *(stepAddress++) = 0;

    // 726
    *(stepAddress++) = ITM_FC;
    *(stepAddress++) = 99;

    // 727
    *(stepAddress++) = ITM_FC;
    *(stepAddress++) = REGISTER_X;

    // 728
    *(stepAddress++) = ITM_FC;
    *(stepAddress++) = REGISTER_K;

    // 729
    *(stepAddress++) = ITM_FC;
    *(stepAddress++) = FIRST_LOCAL_FLAG;

    // 730
    *(stepAddress++) = ITM_FC;
    *(stepAddress++) = LAST_LOCAL_FLAG;

    // 731
    *(stepAddress++) = ITM_FC;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    // 732
    *(stepAddress++) = ITM_FC;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    // 733
    *(stepAddress++) = ITM_FC;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    // 734
    *(stepAddress++) = ITM_FC;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    // 735
    *(stepAddress++) = ITM_FC;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_FLAG;

    // 736
    *(stepAddress++) = ITM_FC;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_FLAG;

    // 737
    *(stepAddress++) = ITM_FC;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 738
    *(stepAddress++) = ITM_FS;
    *(stepAddress++) = 0;

    // 739
    *(stepAddress++) = ITM_FS;
    *(stepAddress++) = 99;

    // 740
    *(stepAddress++) = ITM_FS;
    *(stepAddress++) = REGISTER_X;

    // 741
    *(stepAddress++) = ITM_FS;
    *(stepAddress++) = REGISTER_K;

    // 742
    *(stepAddress++) = ITM_FS;
    *(stepAddress++) = FIRST_LOCAL_FLAG;

    // 743
    *(stepAddress++) = ITM_FS;
    *(stepAddress++) = LAST_LOCAL_FLAG;

    // 744
    *(stepAddress++) = ITM_FS;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    // 745
    *(stepAddress++) = ITM_FS;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    // 746
    *(stepAddress++) = ITM_FS;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    // 747
    *(stepAddress++) = ITM_FS;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    // 748
    *(stepAddress++) = ITM_FS;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_FLAG;

    // 749
    *(stepAddress++) = ITM_FS;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_FLAG;

    // 750
    *(stepAddress++) = ITM_FS;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 751
    *(stepAddress++) = ITM_EVEN;

    // 752
    *(stepAddress++) = ITM_ODD;

    // 753
    *(stepAddress++) = ITM_FPQ;

    // 754
    *(stepAddress++) = ITM_INT;

    // 755
    *(stepAddress++) = ITM_CPX;

    // 756
    *(stepAddress++) = ITM_MATR;

    // 757
    *(stepAddress++) = ITM_NAN;

    // 758
    *(stepAddress++) = ITM_REAL;

    // 759
    *(stepAddress++) = ITM_SPEC;

    // 760
    *(stepAddress++) = ITM_STRI;

    // 761
    *(stepAddress++) = ITM_PMINFINITY;

    // 762
    *(stepAddress++) = ITM_PRIME;

    // 763
    *(stepAddress++) = ITM_TOP;

    // 764
    *(stepAddress++) = ITM_ENTER;

    // 765
    *(stepAddress++) = ITM_XexY;

    // 766
    *(stepAddress++) = ITM_DROP;

    // 767
    *(stepAddress++) = ITM_DROPY;

    // 768
    *(stepAddress++) = ITM_Rup;

    // 769
    *(stepAddress++) = ITM_Rdown;

    // 770
    *(stepAddress++) = ITM_CLX;

    // 771
    *(stepAddress++) = ITM_FILL;

    // 772
    *(stepAddress++) = ITM_INPUT;

    // 773
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 0;

    // 774
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 99;

    // 775
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = REGISTER_X;

    // 776
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = REGISTER_K;

    // 777
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 778
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 779
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 780
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    // 781
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    // 782
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    // 783
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    // 784
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 785
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 786
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 787
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 0;

    // 788
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 99;

    // 789
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = REGISTER_X;

    // 790
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = REGISTER_K;

    // 791
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 792
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 793
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 794
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    // 795
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    // 796
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    // 797
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    // 798
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 799
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 800
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 801
    *(stepAddress++) = ITM_STOSUB;
    *(stepAddress++) = 0;

    // 802
    *(stepAddress++) = ITM_STOSUB;
    *(stepAddress++) = 99;

    // 803
    *(stepAddress++) = ITM_STOSUB;
    *(stepAddress++) = REGISTER_X;

    // 804
    *(stepAddress++) = ITM_STOSUB;
    *(stepAddress++) = REGISTER_K;

    // 805
    *(stepAddress++) = ITM_STOSUB;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 806
    *(stepAddress++) = ITM_STOSUB;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 807
    *(stepAddress++) = ITM_STOSUB;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 808
    *(stepAddress++) = ITM_STOSUB;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    // 809
    *(stepAddress++) = ITM_STOSUB;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    // 810
    *(stepAddress++) = ITM_STOSUB;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    // 811
    *(stepAddress++) = ITM_STOSUB;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    // 812
    *(stepAddress++) = ITM_STOSUB;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 813
    *(stepAddress++) = ITM_STOSUB;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 814
    *(stepAddress++) = ITM_STOSUB;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 815
    *(stepAddress++) = ITM_STOMULT;
    *(stepAddress++) = 0;

    // 816
    *(stepAddress++) = ITM_STOMULT;
    *(stepAddress++) = 99;

    // 817
    *(stepAddress++) = ITM_STOMULT;
    *(stepAddress++) = REGISTER_X;

    // 818
    *(stepAddress++) = ITM_STOMULT;
    *(stepAddress++) = REGISTER_K;

    // 819
    *(stepAddress++) = ITM_STOMULT;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 820
    *(stepAddress++) = ITM_STOMULT;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 821
    *(stepAddress++) = ITM_STOMULT;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 822
    *(stepAddress++) = ITM_STOMULT;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    // 823
    *(stepAddress++) = ITM_STOMULT;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    // 824
    *(stepAddress++) = ITM_STOMULT;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    // 825
    *(stepAddress++) = ITM_STOMULT;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    // 826
    *(stepAddress++) = ITM_STOMULT;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 827
    *(stepAddress++) = ITM_STOMULT;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 828
    *(stepAddress++) = ITM_STOMULT;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 829
    *(stepAddress++) = ITM_STODIV;
    *(stepAddress++) = 0;

    // 830
    *(stepAddress++) = ITM_STODIV;
    *(stepAddress++) = 99;

    // 831
    *(stepAddress++) = ITM_STODIV;
    *(stepAddress++) = REGISTER_X;

    // 832
    *(stepAddress++) = ITM_STODIV;
    *(stepAddress++) = REGISTER_K;

    // 833
    *(stepAddress++) = ITM_STODIV;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 834
    *(stepAddress++) = ITM_STODIV;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 835
    *(stepAddress++) = ITM_STODIV;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 836
    *(stepAddress++) = ITM_STODIV;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    // 837
    *(stepAddress++) = ITM_STODIV;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    // 838
    *(stepAddress++) = ITM_STODIV;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    // 839
    *(stepAddress++) = ITM_STODIV;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    // 840
    *(stepAddress++) = ITM_STODIV;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 841
    *(stepAddress++) = ITM_STODIV;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 842
    *(stepAddress++) = ITM_STODIV;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 843
    *(stepAddress++) = ITM_STOMAX;
    *(stepAddress++) = 0;

    // 844
    *(stepAddress++) = ITM_STOMAX;
    *(stepAddress++) = 99;

    // 845
    *(stepAddress++) = ITM_STOMAX;
    *(stepAddress++) = REGISTER_X;

    // 846
    *(stepAddress++) = ITM_STOMAX;
    *(stepAddress++) = REGISTER_K;

    // 847
    *(stepAddress++) = ITM_STOMAX;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 848
    *(stepAddress++) = ITM_STOMAX;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 849
    *(stepAddress++) = ITM_STOMAX;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 850
    *(stepAddress++) = ITM_STOMAX;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    // 851
    *(stepAddress++) = ITM_STOMAX;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    // 852
    *(stepAddress++) = ITM_STOMAX;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    // 853
    *(stepAddress++) = ITM_STOMAX;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    // 854
    *(stepAddress++) = ITM_STOMAX;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 855
    *(stepAddress++) = ITM_STOMAX;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 856
    *(stepAddress++) = ITM_STOMAX;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 857
    *(stepAddress++) = ITM_STOMIN;
    *(stepAddress++) = 0;

    // 858
    *(stepAddress++) = ITM_STOMIN;
    *(stepAddress++) = 99;

    // 859
    *(stepAddress++) = ITM_STOMIN;
    *(stepAddress++) = REGISTER_X;

    // 860
    *(stepAddress++) = ITM_STOMIN;
    *(stepAddress++) = REGISTER_K;

    // 861
    *(stepAddress++) = ITM_STOMIN;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 862
    *(stepAddress++) = ITM_STOMIN;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 863
    *(stepAddress++) = ITM_STOMIN;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 864
    *(stepAddress++) = ITM_STOMIN;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    // 865
    *(stepAddress++) = ITM_STOMIN;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    // 866
    *(stepAddress++) = ITM_STOMIN;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    // 867
    *(stepAddress++) = ITM_STOMIN;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    // 868
    *(stepAddress++) = ITM_STOMIN;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 869
    *(stepAddress++) = ITM_STOMIN;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 870
    *(stepAddress++) = ITM_STOMIN;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 871
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 0;

    // 872
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 99;

    // 873
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = REGISTER_X;

    // 874
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = REGISTER_K;

    // 875
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 876
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 877
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 878
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    // 879
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    // 880
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    // 881
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    // 882
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 883
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 884
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 885
    *(stepAddress++) = ITM_RCLADD;
    *(stepAddress++) = 0;

    // 886
    *(stepAddress++) = ITM_RCLADD;
    *(stepAddress++) = 99;

    // 887
    *(stepAddress++) = ITM_RCLADD;
    *(stepAddress++) = REGISTER_X;

    // 888
    *(stepAddress++) = ITM_RCLADD;
    *(stepAddress++) = REGISTER_K;

    // 889
    *(stepAddress++) = ITM_RCLADD;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 890
    *(stepAddress++) = ITM_RCLADD;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 891
    *(stepAddress++) = ITM_RCLADD;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 892
    *(stepAddress++) = ITM_RCLADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    // 893
    *(stepAddress++) = ITM_RCLADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    // 894
    *(stepAddress++) = ITM_RCLADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    // 895
    *(stepAddress++) = ITM_RCLADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    // 896
    *(stepAddress++) = ITM_RCLADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 897
    *(stepAddress++) = ITM_RCLADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 898
    *(stepAddress++) = ITM_RCLADD;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 899
    *(stepAddress++) = ITM_RCLSUB;
    *(stepAddress++) = 0;

    // 900
    *(stepAddress++) = ITM_RCLSUB;
    *(stepAddress++) = 99;

    // 901
    *(stepAddress++) = ITM_RCLSUB;
    *(stepAddress++) = REGISTER_X;

    // 902
    *(stepAddress++) = ITM_RCLSUB;
    *(stepAddress++) = REGISTER_K;

    // 903
    *(stepAddress++) = ITM_RCLSUB;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 904
    *(stepAddress++) = ITM_RCLSUB;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 905
    *(stepAddress++) = ITM_RCLSUB;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 906
    *(stepAddress++) = ITM_RCLSUB;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    // 907
    *(stepAddress++) = ITM_RCLSUB;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    // 908
    *(stepAddress++) = ITM_RCLSUB;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    // 909
    *(stepAddress++) = ITM_RCLSUB;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    // 910
    *(stepAddress++) = ITM_RCLSUB;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 911
    *(stepAddress++) = ITM_RCLSUB;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 912
    *(stepAddress++) = ITM_RCLSUB;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 913
    *(stepAddress++) = ITM_RCLMULT;
    *(stepAddress++) = 0;

    // 914
    *(stepAddress++) = ITM_RCLMULT;
    *(stepAddress++) = 99;

    // 915
    *(stepAddress++) = ITM_RCLMULT;
    *(stepAddress++) = REGISTER_X;

    // 916
    *(stepAddress++) = ITM_RCLMULT;
    *(stepAddress++) = REGISTER_K;

    // 917
    *(stepAddress++) = ITM_RCLMULT;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 918
    *(stepAddress++) = ITM_RCLMULT;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 919
    *(stepAddress++) = ITM_RCLMULT;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 920
    *(stepAddress++) = ITM_RCLMULT;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    // 921
    *(stepAddress++) = ITM_RCLMULT;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    // 922
    *(stepAddress++) = ITM_RCLMULT;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    // 923
    *(stepAddress++) = ITM_RCLMULT;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    // 924
    *(stepAddress++) = ITM_RCLMULT;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 925
    *(stepAddress++) = ITM_RCLMULT;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 926
    *(stepAddress++) = ITM_RCLMULT;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 927
    *(stepAddress++) = ITM_RCLDIV;
    *(stepAddress++) = 0;

    // 928
    *(stepAddress++) = ITM_RCLDIV;
    *(stepAddress++) = 99;

    // 929
    *(stepAddress++) = ITM_RCLDIV;
    *(stepAddress++) = REGISTER_X;

    // 930
    *(stepAddress++) = ITM_RCLDIV;
    *(stepAddress++) = REGISTER_K;

    // 931
    *(stepAddress++) = ITM_RCLDIV;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 932
    *(stepAddress++) = ITM_RCLDIV;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 933
    *(stepAddress++) = ITM_RCLDIV;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 934
    *(stepAddress++) = ITM_RCLDIV;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    // 935
    *(stepAddress++) = ITM_RCLDIV;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    // 936
    *(stepAddress++) = ITM_RCLDIV;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    // 937
    *(stepAddress++) = ITM_RCLDIV;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    // 938
    *(stepAddress++) = ITM_RCLDIV;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 939
    *(stepAddress++) = ITM_RCLDIV;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 940
    *(stepAddress++) = ITM_RCLDIV;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 941
    *(stepAddress++) = ITM_RCLMAX;
    *(stepAddress++) = 0;

    // 942
    *(stepAddress++) = ITM_RCLMAX;
    *(stepAddress++) = 99;

    // 943
    *(stepAddress++) = ITM_RCLMAX;
    *(stepAddress++) = REGISTER_X;

    // 944
    *(stepAddress++) = ITM_RCLMAX;
    *(stepAddress++) = REGISTER_K;

    // 945
    *(stepAddress++) = ITM_RCLMAX;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 946
    *(stepAddress++) = ITM_RCLMAX;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 947
    *(stepAddress++) = ITM_RCLMAX;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 948
    *(stepAddress++) = ITM_RCLMAX;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    // 949
    *(stepAddress++) = ITM_RCLMAX;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    // 950
    *(stepAddress++) = ITM_RCLMAX;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    // 951
    *(stepAddress++) = ITM_RCLMAX;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    // 952
    *(stepAddress++) = ITM_RCLMAX;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 953
    *(stepAddress++) = ITM_RCLMAX;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 954
    *(stepAddress++) = ITM_RCLMAX;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 955
    *(stepAddress++) = ITM_RCLMIN;
    *(stepAddress++) = 0;

    // 956
    *(stepAddress++) = ITM_RCLMIN;
    *(stepAddress++) = 99;

    // 957
    *(stepAddress++) = ITM_RCLMIN;
    *(stepAddress++) = REGISTER_X;

    // 958
    *(stepAddress++) = ITM_RCLMIN;
    *(stepAddress++) = REGISTER_K;

    // 959
    *(stepAddress++) = ITM_RCLMIN;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 960
    *(stepAddress++) = ITM_RCLMIN;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 961
    *(stepAddress++) = ITM_RCLMIN;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 962
    *(stepAddress++) = ITM_RCLMIN;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    // 963
    *(stepAddress++) = ITM_RCLMIN;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    // 964
    *(stepAddress++) = ITM_RCLMIN;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    // 965
    *(stepAddress++) = ITM_RCLMIN;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    // 966
    *(stepAddress++) = ITM_RCLMIN;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 967
    *(stepAddress++) = ITM_RCLMIN;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 968
    *(stepAddress++) = ITM_RCLMIN;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 969
    *(stepAddress++) = ITM_SQUARE;

    // 970
    *(stepAddress++) = ITM_CUBE;

    // 971
    *(stepAddress++) = ITM_YX;

    // 972
    *(stepAddress++) = ITM_SQUAREROOTX;

    // 973
    *(stepAddress++) = ITM_CUBEROOT;

    // 974
    *(stepAddress++) = ITM_XTHROOT;

    // 975
    *(stepAddress++) = ITM_2X;

    // 976
    *(stepAddress++) = ITM_EXP;

    // 977
    *(stepAddress++) = ITM_EX1;

    // 978
    *(stepAddress++) = ITM_10x;

    // 979
    *(stepAddress++) = ITM_LOG2;

    // 980
    *(stepAddress++) = ITM_LN;

    // 981
    *(stepAddress++) = ITM_LN1X;

    // 982
    *(stepAddress++) = ITM_LOG10;

    // 983
    *(stepAddress++) = ITM_LOGXY;

    // 984
    *(stepAddress++) = ITM_1ONX;

    // 985
    *(stepAddress++) = ITM_cos;

    // 986
    *(stepAddress++) = ITM_cosh;

    // 987
    *(stepAddress++) = ITM_sin;

    // 988
    *(stepAddress++) = ITM_sinc;

    // 989
    *(stepAddress++) = ITM_sinh;

    // 990
    *(stepAddress++) = ITM_tan;

    // 991
    *(stepAddress++) = ITM_tanh;

    // 992
    *(stepAddress++) = ITM_arccos;

    // 993
    *(stepAddress++) = ITM_arcosh;

    // 994
    *(stepAddress++) = ITM_arcsin;

    // 995
    *(stepAddress++) = ITM_arsinh;

    // 996
    *(stepAddress++) = ITM_arctan;

    // 997
    *(stepAddress++) = ITM_artanh;

    // 998
    *(stepAddress++) = ITM_CEIL;

    // 999
    *(stepAddress++) = ITM_FLOOR;

    // 1000
    *(stepAddress++) = ITM_GCD;

    // 1001
    *(stepAddress++) = ITM_LCM;

    // 1002
    *(stepAddress++) = ITM_DEC;
    *(stepAddress++) = 0;

    // 1003
    *(stepAddress++) = ITM_DEC;
    *(stepAddress++) = 99;

    // 1004
    *(stepAddress++) = ITM_DEC;
    *(stepAddress++) = REGISTER_X;

    // 1005
    *(stepAddress++) = ITM_DEC;
    *(stepAddress++) = REGISTER_K;

    // 1006
    *(stepAddress++) = ITM_DEC;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 1007
    *(stepAddress++) = ITM_DEC;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 1008
    *(stepAddress++) = ITM_DEC;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 1009
    *(stepAddress++) = ITM_DEC;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    // 1010
    *(stepAddress++) = ITM_DEC;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    // 1011
    *(stepAddress++) = ITM_DEC;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    // 1012
    *(stepAddress++) = ITM_DEC;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    // 1013
    *(stepAddress++) = ITM_DEC;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 1014
    *(stepAddress++) = ITM_DEC;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 1015
    *(stepAddress++) = ITM_DEC;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 1016
    *(stepAddress++) = ITM_INC;
    *(stepAddress++) = 0;

    // 1017
    *(stepAddress++) = ITM_INC;
    *(stepAddress++) = 99;

    // 1018
    *(stepAddress++) = ITM_INC;
    *(stepAddress++) = REGISTER_X;

    // 1019
    *(stepAddress++) = ITM_INC;
    *(stepAddress++) = REGISTER_K;

    // 1020
    *(stepAddress++) = ITM_INC;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 1021
    *(stepAddress++) = ITM_INC;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 1022
    *(stepAddress++) = ITM_INC;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 1023
    *(stepAddress++) = ITM_INC;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    // 1024
    *(stepAddress++) = ITM_INC;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    // 1025
    *(stepAddress++) = ITM_INC;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    // 1026
    *(stepAddress++) = ITM_INC;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    // 1027
    *(stepAddress++) = ITM_INC;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 1028
    *(stepAddress++) = ITM_INC;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 1029
    *(stepAddress++) = ITM_INC;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 1030
    *(stepAddress++) = ITM_IP;

    // 1031
    *(stepAddress++) = ITM_FP;

    // 1032
    *(stepAddress++) = ITM_ADD;

    // 1033
    *(stepAddress++) = ITM_SUB;

    // 1034
    *(stepAddress++) = ITM_CHS;

    // 1035
    *(stepAddress++) = ITM_MULT;

    // 1036
    *(stepAddress++) = ITM_DIV;

    // 1037
    *(stepAddress++) = ITM_IDIV;

    // 1038
    *(stepAddress++) = ITM_IDIVR;

    // 1039
    *(stepAddress++) = ITM_MOD;

    // 1040
    *(stepAddress++) = ITM_MAX;

    // 1041
    *(stepAddress++) = ITM_MIN;

    // 1042
    *(stepAddress++) = ITM_MAGNITUDE;

    // 1043
    *(stepAddress++) = ITM_NEIGHB;

    // 1044
    *(stepAddress++) = ITM_NEXTP;

    // 1045
    *(stepAddress++) = ITM_XFACT;

    // 1046
    *(stepAddress++) = ITM_pi;

    // 1047
    *(stepAddress++) = ITM_CF;
    *(stepAddress++) = 0;

    // 1048
    *(stepAddress++) = ITM_CF;
    *(stepAddress++) = 99;

    // 1049
    *(stepAddress++) = ITM_CF;
    *(stepAddress++) = REGISTER_X;

    // 1050
    *(stepAddress++) = ITM_CF;
    *(stepAddress++) = REGISTER_K;

    // 1051
    *(stepAddress++) = ITM_CF;
    *(stepAddress++) = FIRST_LOCAL_FLAG;

    // 1052
    *(stepAddress++) = ITM_CF;
    *(stepAddress++) = LAST_LOCAL_FLAG;

    // 1053
    *(stepAddress++) = ITM_CF;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    // 1054
    *(stepAddress++) = ITM_CF;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    // 1055
    *(stepAddress++) = ITM_CF;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    // 1056
    *(stepAddress++) = ITM_CF;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    // 1057
    *(stepAddress++) = ITM_CF;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_FLAG;

    // 1058
    *(stepAddress++) = ITM_CF;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_FLAG;

    // 1059
    *(stepAddress++) = ITM_CF;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 1060
    *(stepAddress++) = ITM_SF;
    *(stepAddress++) = 0;

    // 1061
    *(stepAddress++) = ITM_SF;
    *(stepAddress++) = 99;

    // 1062
    *(stepAddress++) = ITM_SF;
    *(stepAddress++) = REGISTER_X;

    // 1063
    *(stepAddress++) = ITM_SF;
    *(stepAddress++) = REGISTER_K;

    // 1064
    *(stepAddress++) = ITM_SF;
    *(stepAddress++) = FIRST_LOCAL_FLAG;

    // 1065
    *(stepAddress++) = ITM_SF;
    *(stepAddress++) = LAST_LOCAL_FLAG;

    // 1066
    *(stepAddress++) = ITM_SF;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    // 1067
    *(stepAddress++) = ITM_SF;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    // 1068
    *(stepAddress++) = ITM_SF;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    // 1069
    *(stepAddress++) = ITM_SF;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    // 1070
    *(stepAddress++) = ITM_SF;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_FLAG;

    // 1071
    *(stepAddress++) = ITM_SF;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_FLAG;

    // 1072
    *(stepAddress++) = ITM_SF;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 1073
    *(stepAddress++) = ITM_FF;
    *(stepAddress++) = 0;

    // 1074
    *(stepAddress++) = ITM_FF;
    *(stepAddress++) = 99;

    // 1075
    *(stepAddress++) = ITM_FF;
    *(stepAddress++) = REGISTER_X;

    // 1076
    *(stepAddress++) = ITM_FF;
    *(stepAddress++) = REGISTER_K;

    // 1077
    *(stepAddress++) = ITM_FF;
    *(stepAddress++) = FIRST_LOCAL_FLAG;

    // 1078
    *(stepAddress++) = ITM_FF;
    *(stepAddress++) = LAST_LOCAL_FLAG;

    // 1079
    *(stepAddress++) = ITM_FF;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    // 1080
    *(stepAddress++) = ITM_FF;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    // 1081
    *(stepAddress++) = ITM_FF;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    // 1082
    *(stepAddress++) = ITM_FF;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    // 1083
    *(stepAddress++) = ITM_FF;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_FLAG;

    // 1084
    *(stepAddress++) = ITM_FF;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_FLAG;

    // 1085
    *(stepAddress++) = ITM_FF;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 1086
    *(stepAddress++) = ITM_sincpi;

    // 1087
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = BINARY_SHORT_INTEGER;
    *(stepAddress++) = 16; // Base
    *(stepAddress++) = 1;
    *(stepAddress++) = 2;
    *(stepAddress++) = 3;
    *(stepAddress++) = 4;
    *(stepAddress++) = 5;
    *(stepAddress++) = 6;
    *(stepAddress++) = 7;
    *(stepAddress++) = 8;

    // 1088
    *(stepAddress++) = (ITM_NOP >> 8) | 0x80; // TODO: implement ITM_LITT BINARY_LONG_INTEGER
    *(stepAddress++) =  ITM_NOP       & 0xff;

    // 1089
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = BINARY_REAL34;
    *(stepAddress++) =  1;
    *(stepAddress++) =  2;
    *(stepAddress++) =  3;
    *(stepAddress++) =  4;
    *(stepAddress++) =  5;
    *(stepAddress++) =  6;
    *(stepAddress++) =  7;
    *(stepAddress++) =  8;
    *(stepAddress++) =  9;
    *(stepAddress++) = 10;
    *(stepAddress++) = 11;
    *(stepAddress++) = 12;
    *(stepAddress++) = 13;
    *(stepAddress++) = 14;
    *(stepAddress++) = 15;
    *(stepAddress++) = 16;

    // 1090
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = BINARY_ANGLE34;
    *(stepAddress++) = AM_MULTPI;
    *(stepAddress++) = 16;
    *(stepAddress++) = 15;
    *(stepAddress++) = 14;
    *(stepAddress++) = 13;
    *(stepAddress++) = 12;
    *(stepAddress++) = 11;
    *(stepAddress++) = 10;
    *(stepAddress++) =  9;
    *(stepAddress++) =  8;
    *(stepAddress++) =  7;
    *(stepAddress++) =  6;
    *(stepAddress++) =  5;
    *(stepAddress++) =  4;
    *(stepAddress++) =  3;
    *(stepAddress++) =  2;
    *(stepAddress++) =  1;

    // 1091
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = BINARY_COMPLEX34;
    *(stepAddress++) =  1;
    *(stepAddress++) =  2;
    *(stepAddress++) =  3;
    *(stepAddress++) =  4;
    *(stepAddress++) =  5;
    *(stepAddress++) =  6;
    *(stepAddress++) =  7;
    *(stepAddress++) =  8;
    *(stepAddress++) =  9;
    *(stepAddress++) = 10;
    *(stepAddress++) = 11;
    *(stepAddress++) = 12;
    *(stepAddress++) = 13;
    *(stepAddress++) = 14;
    *(stepAddress++) = 15;
    *(stepAddress++) = 16;
    *(stepAddress++) = 17;
    *(stepAddress++) = 18;
    *(stepAddress++) = 19;
    *(stepAddress++) = 20;
    *(stepAddress++) = 21;
    *(stepAddress++) = 22;
    *(stepAddress++) = 23;
    *(stepAddress++) = 24;
    *(stepAddress++) = 25;
    *(stepAddress++) = 26;
    *(stepAddress++) = 27;
    *(stepAddress++) = 28;
    *(stepAddress++) = 29;
    *(stepAddress++) = 30;
    *(stepAddress++) = 31;
    *(stepAddress++) = 32;

    // 1092
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_SHORT_INTEGER;
    *(stepAddress++) =  16; // Base
    *(stepAddress++) =  7;  // String length
    *(stepAddress++) =  '-';
    *(stepAddress++) =  'A';
    *(stepAddress++) =  'B';
    *(stepAddress++) =  'C';
    *(stepAddress++) =  'D';
    *(stepAddress++) =  'E';
    *(stepAddress++) =  'F';

    // 1093
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_LONG_INTEGER;
    *(stepAddress++) =  7;  // String length
    *(stepAddress++) =  '-';
    *(stepAddress++) =  '1';
    *(stepAddress++) =  '2';
    *(stepAddress++) =  '3';
    *(stepAddress++) =  '4';
    *(stepAddress++) =  '5';
    *(stepAddress++) =  '6';

    // 1094
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_ANGLE34;
    *(stepAddress++) = AM_DEGREE;
    *(stepAddress++) = 2; // String length
    *(stepAddress++) = '4';
    *(stepAddress++) = '5';

    // 1095
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_COMPLEX34;
    *(stepAddress++) = 4; // String length
    *(stepAddress++) = '1';
    *(stepAddress++) = '-';
    *(stepAddress++) = 'i';
    *(stepAddress++) = '2';

    // 1096
    *(stepAddress++) = (CST_01 >> 8) | 0x80;
    *(stepAddress++) =  CST_01       & 0xff;

    // 1097
    *(stepAddress++) = (CST_02 >> 8) | 0x80;
    *(stepAddress++) =  CST_02       & 0xff;

    // 1098
    *(stepAddress++) = (CST_03 >> 8) | 0x80;
    *(stepAddress++) =  CST_03       & 0xff;

    // 1099
    *(stepAddress++) = (CST_04 >> 8) | 0x80;
    *(stepAddress++) =  CST_04       & 0xff;

    // 1100
    *(stepAddress++) = (CST_05 >> 8) | 0x80;
    *(stepAddress++) =  CST_05       & 0xff;

    // 1101
    *(stepAddress++) = (CST_06 >> 8) | 0x80;
    *(stepAddress++) =  CST_06       & 0xff;

    // 1102
    *(stepAddress++) = (CST_07 >> 8) | 0x80;
    *(stepAddress++) =  CST_07       & 0xff;

    // 1103
    *(stepAddress++) = (CST_08 >> 8) | 0x80;
    *(stepAddress++) =  CST_08       & 0xff;

    // 1104
    *(stepAddress++) = (CST_09 >> 8) | 0x80;
    *(stepAddress++) =  CST_09       & 0xff;

    // 1105
    *(stepAddress++) = (CST_10 >> 8) | 0x80;
    *(stepAddress++) =  CST_10       & 0xff;

    // 1106
    *(stepAddress++) = (CST_11 >> 8) | 0x80;
    *(stepAddress++) =  CST_11       & 0xff;

    // 1107
    *(stepAddress++) = (CST_12 >> 8) | 0x80;
    *(stepAddress++) =  CST_12       & 0xff;

    // 1108
    *(stepAddress++) = (CST_13 >> 8) | 0x80;
    *(stepAddress++) =  CST_13       & 0xff;

    // 1109
    *(stepAddress++) = (CST_14 >> 8) | 0x80;
    *(stepAddress++) =  CST_14       & 0xff;

    // 1110
    *(stepAddress++) = (CST_15 >> 8) | 0x80;
    *(stepAddress++) =  CST_15       & 0xff;

    // 1111
    *(stepAddress++) = (CST_16 >> 8) | 0x80;
    *(stepAddress++) =  CST_16       & 0xff;

    // 1112
    *(stepAddress++) = (CST_17 >> 8) | 0x80;
    *(stepAddress++) =  CST_17       & 0xff;

    // 1113
    *(stepAddress++) = (CST_18 >> 8) | 0x80;
    *(stepAddress++) =  CST_18       & 0xff;

    // 1114
    *(stepAddress++) = (CST_19 >> 8) | 0x80;
    *(stepAddress++) =  CST_19       & 0xff;

    // 1115
    *(stepAddress++) = (CST_20 >> 8) | 0x80;
    *(stepAddress++) =  CST_20       & 0xff;

    // 1116
    *(stepAddress++) = (CST_21 >> 8) | 0x80;
    *(stepAddress++) =  CST_21       & 0xff;

    // 1117
    *(stepAddress++) = (CST_22 >> 8) | 0x80;
    *(stepAddress++) =  CST_22       & 0xff;

    // 1118
    *(stepAddress++) = (CST_23 >> 8) | 0x80;
    *(stepAddress++) =  CST_23       & 0xff;

    // 1119
    *(stepAddress++) = (CST_24 >> 8) | 0x80;
    *(stepAddress++) =  CST_24       & 0xff;

    // 1120
    *(stepAddress++) = (CST_25 >> 8) | 0x80;
    *(stepAddress++) =  CST_25       & 0xff;

    // 1121
    *(stepAddress++) = (CST_26 >> 8) | 0x80;
    *(stepAddress++) =  CST_26       & 0xff;

    // 1122
    *(stepAddress++) = (CST_27 >> 8) | 0x80;
    *(stepAddress++) =  CST_27       & 0xff;

    // 1123
    *(stepAddress++) = (CST_28 >> 8) | 0x80;
    *(stepAddress++) =  CST_28       & 0xff;

    // 1124
    *(stepAddress++) = (CST_29 >> 8) | 0x80;
    *(stepAddress++) =  CST_29       & 0xff;

    // 1125
    *(stepAddress++) = (CST_30 >> 8) | 0x80;
    *(stepAddress++) =  CST_30       & 0xff;

    // 1126
    *(stepAddress++) = (CST_31 >> 8) | 0x80;
    *(stepAddress++) =  CST_31       & 0xff;

    // 1127
    *(stepAddress++) = (CST_32 >> 8) | 0x80;
    *(stepAddress++) =  CST_32       & 0xff;

    // 1128
    *(stepAddress++) = (CST_33 >> 8) | 0x80;
    *(stepAddress++) =  CST_33       & 0xff;

    // 1129
    *(stepAddress++) = (CST_34 >> 8) | 0x80;
    *(stepAddress++) =  CST_34       & 0xff;

    // 1130
    *(stepAddress++) = (CST_35 >> 8) | 0x80;
    *(stepAddress++) =  CST_35       & 0xff;

    // 1131
    *(stepAddress++) = (CST_36 >> 8) | 0x80;
    *(stepAddress++) =  CST_36       & 0xff;

    // 1132
    *(stepAddress++) = (CST_37 >> 8) | 0x80;
    *(stepAddress++) =  CST_37       & 0xff;

    // 1133
    *(stepAddress++) = (CST_38 >> 8) | 0x80;
    *(stepAddress++) =  CST_38       & 0xff;

    // 1134
    *(stepAddress++) = (CST_39 >> 8) | 0x80;
    *(stepAddress++) =  CST_39       & 0xff;

    // 1135
    *(stepAddress++) = (CST_40 >> 8) | 0x80;
    *(stepAddress++) =  CST_40       & 0xff;

    // 1136
    *(stepAddress++) = (CST_41 >> 8) | 0x80;
    *(stepAddress++) =  CST_41       & 0xff;

    // 1137
    *(stepAddress++) = (CST_42 >> 8) | 0x80;
    *(stepAddress++) =  CST_42       & 0xff;

    // 1138
    *(stepAddress++) = (CST_43 >> 8) | 0x80;
    *(stepAddress++) =  CST_43       & 0xff;

    // 1139
    *(stepAddress++) = (CST_44 >> 8) | 0x80;
    *(stepAddress++) =  CST_44       & 0xff;

    // 1140
    *(stepAddress++) = (CST_45 >> 8) | 0x80;
    *(stepAddress++) =  CST_45       & 0xff;

    // 1141
    *(stepAddress++) = (CST_46 >> 8) | 0x80;
    *(stepAddress++) =  CST_46       & 0xff;

    // 1142
    *(stepAddress++) = (CST_47 >> 8) | 0x80;
    *(stepAddress++) =  CST_47       & 0xff;

    // 1143
    *(stepAddress++) = (CST_48 >> 8) | 0x80;
    *(stepAddress++) =  CST_48       & 0xff;

    // 1144
    *(stepAddress++) = (CST_49 >> 8) | 0x80;
    *(stepAddress++) =  CST_49       & 0xff;

    // 1145
    *(stepAddress++) = (CST_50 >> 8) | 0x80;
    *(stepAddress++) =  CST_50       & 0xff;

    // 1146
    *(stepAddress++) = (CST_51 >> 8) | 0x80;
    *(stepAddress++) =  CST_51       & 0xff;

    // 1147
    *(stepAddress++) = (CST_52 >> 8) | 0x80;
    *(stepAddress++) =  CST_52       & 0xff;

    // 1148
    *(stepAddress++) = (CST_53 >> 8) | 0x80;
    *(stepAddress++) =  CST_53       & 0xff;

    // 1149
    *(stepAddress++) = (CST_54 >> 8) | 0x80;
    *(stepAddress++) =  CST_54       & 0xff;

    // 1150
    *(stepAddress++) = (CST_55 >> 8) | 0x80;
    *(stepAddress++) =  CST_55       & 0xff;

    // 1151
    *(stepAddress++) = (CST_56 >> 8) | 0x80;
    *(stepAddress++) =  CST_56       & 0xff;

    // 1152
    *(stepAddress++) = (CST_57 >> 8) | 0x80;
    *(stepAddress++) =  CST_57       & 0xff;

    // 1153
    *(stepAddress++) = (CST_58 >> 8) | 0x80;
    *(stepAddress++) =  CST_58       & 0xff;

    // 1154
    *(stepAddress++) = (CST_59 >> 8) | 0x80;
    *(stepAddress++) =  CST_59       & 0xff;

    // 1155
    *(stepAddress++) = (CST_60 >> 8) | 0x80;
    *(stepAddress++) =  CST_60       & 0xff;

    // 1156
    *(stepAddress++) = (CST_61 >> 8) | 0x80;
    *(stepAddress++) =  CST_61       & 0xff;

    // 1157
    *(stepAddress++) = (CST_62 >> 8) | 0x80;
    *(stepAddress++) =  CST_62       & 0xff;

    // 1158
    *(stepAddress++) = (CST_63 >> 8) | 0x80;
    *(stepAddress++) =  CST_63       & 0xff;

    // 1159
    *(stepAddress++) = (CST_64 >> 8) | 0x80;
    *(stepAddress++) =  CST_64       & 0xff;

    // 1160
    *(stepAddress++) = (CST_65 >> 8) | 0x80;
    *(stepAddress++) =  CST_65       & 0xff;

    // 1161
    *(stepAddress++) = (CST_66 >> 8) | 0x80;
    *(stepAddress++) =  CST_66       & 0xff;

    // 1162
    *(stepAddress++) = (CST_67 >> 8) | 0x80;
    *(stepAddress++) =  CST_67       & 0xff;

    // 1163
    *(stepAddress++) = (CST_68 >> 8) | 0x80;
    *(stepAddress++) =  CST_68       & 0xff;

    // 1164
    *(stepAddress++) = (CST_69 >> 8) | 0x80;
    *(stepAddress++) =  CST_69       & 0xff;

    // 1165
    *(stepAddress++) = (CST_70 >> 8) | 0x80;
    *(stepAddress++) =  CST_70       & 0xff;

    // 1166
    *(stepAddress++) = (CST_71 >> 8) | 0x80;
    *(stepAddress++) =  CST_71       & 0xff;

    // 1167
    *(stepAddress++) = (CST_72 >> 8) | 0x80;
    *(stepAddress++) =  CST_72       & 0xff;

    // 1168
    *(stepAddress++) = (CST_73 >> 8) | 0x80;
    *(stepAddress++) =  CST_73       & 0xff;

    // 1169
    *(stepAddress++) = (CST_74 >> 8) | 0x80;
    *(stepAddress++) =  CST_74       & 0xff;

    // 1170
    *(stepAddress++) = (CST_75 >> 8) | 0x80;
    *(stepAddress++) =  CST_75       & 0xff;

    // 1171
    *(stepAddress++) = (CST_76 >> 8) | 0x80;
    *(stepAddress++) =  CST_76       & 0xff;

    // 1172
    *(stepAddress++) = (CST_77 >> 8) | 0x80;
    *(stepAddress++) =  CST_77       & 0xff;

    // 1173
    *(stepAddress++) = (CST_78 >> 8) | 0x80;
    *(stepAddress++) =  CST_78       & 0xff;

    // 1174
    *(stepAddress++) = (CST_79 >> 8) | 0x80;
    *(stepAddress++) =  CST_79       & 0xff;

    // 1175
    *(stepAddress++) = (ITM_CNST >> 8) | 0x80;
    *(stepAddress++) =  ITM_CNST       & 0xff;
    *(stepAddress++) =  1;

    // 1176
    *(stepAddress++) = (ITM_CNST >> 8) | 0x80;
    *(stepAddress++) =  ITM_CNST       & 0xff;
    *(stepAddress++) =  79;

    // 1177
    *(stepAddress++) = (ITM_CNST >> 8) | 0x80;
    *(stepAddress++) =  ITM_CNST       & 0xff;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    // 1178
    *(stepAddress++) = (ITM_CNST >> 8) | 0x80;
    *(stepAddress++) =  ITM_CNST       & 0xff;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    // 1179
    *(stepAddress++) = (ITM_CNST >> 8) | 0x80;
    *(stepAddress++) =  ITM_CNST       & 0xff;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    // 1180
    *(stepAddress++) = (ITM_CNST >> 8) | 0x80;
    *(stepAddress++) =  ITM_CNST       & 0xff;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    // 1181
    *(stepAddress++) = (ITM_CNST >> 8) | 0x80;
    *(stepAddress++) =  ITM_CNST       & 0xff;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 1182
    *(stepAddress++) = (ITM_CNST >> 8) | 0x80;
    *(stepAddress++) =  ITM_CNST       & 0xff;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 1183
    *(stepAddress++) = (ITM_CNST >> 8) | 0x80;
    *(stepAddress++) =  ITM_CNST       & 0xff;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 1184
    *(stepAddress++) = (ITM_CtoF >> 8) | 0x80;
    *(stepAddress++) =  ITM_CtoF       & 0xff;

    // 1185
    *(stepAddress++) = (ITM_FtoC >> 8) | 0x80;
    *(stepAddress++) =  ITM_FtoC       & 0xff;

    // 1186
    *(stepAddress++) = (ITM_DBtoPR >> 8) | 0x80;
    *(stepAddress++) =  ITM_DBtoPR       & 0xff;

    // 1187
    *(stepAddress++) = (ITM_DBtoPRb >> 8) | 0x80;
    *(stepAddress++) =  ITM_DBtoPRb       & 0xff;

    // 1188
    *(stepAddress++) = (ITM_DBtoPRc >> 8) | 0x80;
    *(stepAddress++) =  ITM_DBtoPRc       & 0xff;

    // 1189
    *(stepAddress++) = (ITM_DBtoFR >> 8) | 0x80;
    *(stepAddress++) =  ITM_DBtoFR       & 0xff;

    // 1190
    *(stepAddress++) = (ITM_DBtoFRb >> 8) | 0x80;
    *(stepAddress++) =  ITM_DBtoFRb       & 0xff;

    // 1191
    *(stepAddress++) = (ITM_DBtoFRc >> 8) | 0x80;
    *(stepAddress++) =  ITM_DBtoFRc       & 0xff;

    // 1192
    *(stepAddress++) = (ITM_PRtoDB >> 8) | 0x80;
    *(stepAddress++) =  ITM_PRtoDB       & 0xff;

    // 1193
    *(stepAddress++) = (ITM_PRtoDBb >> 8) | 0x80;
    *(stepAddress++) =  ITM_PRtoDBb       & 0xff;

    // 1194
    *(stepAddress++) = (ITM_PRtoDBc >> 8) | 0x80;
    *(stepAddress++) =  ITM_PRtoDBc       & 0xff;

    // 1195
    *(stepAddress++) = (ITM_FRtoDB >> 8) | 0x80;
    *(stepAddress++) =  ITM_FRtoDB       & 0xff;

    // 1196
    *(stepAddress++) = (ITM_FRtoDBb >> 8) | 0x80;
    *(stepAddress++) =  ITM_FRtoDBb       & 0xff;

    // 1197
    *(stepAddress++) = (ITM_FRtoDBc >> 8) | 0x80;
    *(stepAddress++) =  ITM_FRtoDBc       & 0xff;

    // 1198
    *(stepAddress++) = (ITM_ACtoM2 >> 8) | 0x80;
    *(stepAddress++) =  ITM_ACtoM2       & 0xff;

    // 1199
    *(stepAddress++) = (ITM_ACtoM2b >> 8) | 0x80;
    *(stepAddress++) =  ITM_ACtoM2b       & 0xff;

    // 1200
    *(stepAddress++) = (ITM_M2toAC >> 8) | 0x80;
    *(stepAddress++) =  ITM_M2toAC       & 0xff;

    // 1201
    *(stepAddress++) = (ITM_M2toACb >> 8) | 0x80;
    *(stepAddress++) =  ITM_M2toACb       & 0xff;

    // 1202
    *(stepAddress++) = (ITM_ACUStoM2 >> 8) | 0x80;
    *(stepAddress++) =  ITM_ACUStoM2       & 0xff;

    // 1203
    *(stepAddress++) = (ITM_ACUStoM2b >> 8) | 0x80;
    *(stepAddress++) =  ITM_ACUStoM2b       & 0xff;

    // 1204
    *(stepAddress++) = (ITM_M2toACUS >> 8) | 0x80;
    *(stepAddress++) =  ITM_M2toACUS       & 0xff;

    // 1205
    *(stepAddress++) = (ITM_M2toACUSb >> 8) | 0x80;
    *(stepAddress++) =  ITM_M2toACUSb       & 0xff;

    // 1206
    *(stepAddress++) = (ITM_PAtoATM >> 8) | 0x80;
    *(stepAddress++) =  ITM_PAtoATM       & 0xff;

    // 1207
    *(stepAddress++) = (ITM_ATMtoPA >> 8) | 0x80;
    *(stepAddress++) =  ITM_ATMtoPA       & 0xff;

    // 1208
    *(stepAddress++) = (ITM_AUtoM >> 8) | 0x80;
    *(stepAddress++) =  ITM_AUtoM       & 0xff;

    // 1209
    *(stepAddress++) = (ITM_MtoAU >> 8) | 0x80;
    *(stepAddress++) =  ITM_MtoAU       & 0xff;

    // 1210
    *(stepAddress++) = (ITM_BARtoPA >> 8) | 0x80;
    *(stepAddress++) =  ITM_BARtoPA       & 0xff;

    // 1211
    *(stepAddress++) = (ITM_PAtoBAR >> 8) | 0x80;
    *(stepAddress++) =  ITM_PAtoBAR       & 0xff;

    // 1212
    *(stepAddress++) = (ITM_BTUtoJ >> 8) | 0x80;
    *(stepAddress++) =  ITM_BTUtoJ       & 0xff;

    // 1213
    *(stepAddress++) = (ITM_JtoBTU >> 8) | 0x80;
    *(stepAddress++) =  ITM_JtoBTU       & 0xff;

    // 1214
    *(stepAddress++) = (ITM_CALtoJ >> 8) | 0x80;
    *(stepAddress++) =  ITM_CALtoJ       & 0xff;

    // 1215
    *(stepAddress++) = (ITM_JtoCAL >> 8) | 0x80;
    *(stepAddress++) =  ITM_JtoCAL       & 0xff;

    // 1216
    *(stepAddress++) = (ITM_LBFFTtoNM >> 8) | 0x80;
    *(stepAddress++) =  ITM_LBFFTtoNM       & 0xff;

    // 1217
    *(stepAddress++) = (ITM_LBFFTtoNMb >> 8) | 0x80;
    *(stepAddress++) =  ITM_LBFFTtoNMb       & 0xff;

    // 1218
    *(stepAddress++) = (ITM_NMtoLBFFT >> 8) | 0x80;
    *(stepAddress++) =  ITM_NMtoLBFFT       & 0xff;

    // 1219
    *(stepAddress++) = (ITM_NMtoLBFFTb >> 8) | 0x80;
    *(stepAddress++) =  ITM_NMtoLBFFTb       & 0xff;

    // 1220
    *(stepAddress++) = (ITM_CWTtoKG >> 8) | 0x80;
    *(stepAddress++) =  ITM_CWTtoKG       & 0xff;

    // 1221
    *(stepAddress++) = (ITM_KGtoCWT >> 8) | 0x80;
    *(stepAddress++) =  ITM_KGtoCWT       & 0xff;

    // 1222
    *(stepAddress++) = (ITM_FTtoM >> 8) | 0x80;
    *(stepAddress++) =  ITM_FTtoM       & 0xff;

    // 1223
    *(stepAddress++) = (ITM_MtoFT >> 8) | 0x80;
    *(stepAddress++) =  ITM_MtoFT       & 0xff;

    // 1224
    *(stepAddress++) = (ITM_FTUStoM >> 8) | 0x80;
    *(stepAddress++) =  ITM_FTUStoM       & 0xff;

    // 1225
    *(stepAddress++) = (ITM_FTUStoMb >> 8) | 0x80;
    *(stepAddress++) =  ITM_FTUStoMb       & 0xff;

    // 1226
    *(stepAddress++) = (ITM_FTUStoMc >> 8) | 0x80;
    *(stepAddress++) =  ITM_FTUStoMc       & 0xff;

    // 1227
    *(stepAddress++) = (ITM_MtoFTUS >> 8) | 0x80;
    *(stepAddress++) =  ITM_MtoFTUS       & 0xff;

    // 1228
    *(stepAddress++) = (ITM_MtoFTUSb >> 8) | 0x80;
    *(stepAddress++) =  ITM_MtoFTUSb       & 0xff;

    // 1229
    *(stepAddress++) = (ITM_MtoFTUSc >> 8) | 0x80;
    *(stepAddress++) =  ITM_MtoFTUSc       & 0xff;

    // 1230
    *(stepAddress++) = (ITM_FZUKtoM3 >> 8) | 0x80;
    *(stepAddress++) =  ITM_FZUKtoM3       & 0xff;

    // 1231
    *(stepAddress++) = (ITM_FZUKtoM3b >> 8) | 0x80;
    *(stepAddress++) =  ITM_FZUKtoM3b       & 0xff;

    // 1232
    *(stepAddress++) = (ITM_M3toFZUK >> 8) | 0x80;
    *(stepAddress++) =  ITM_M3toFZUK       & 0xff;

    // 1233
    *(stepAddress++) = (ITM_M3toFZUKb >> 8) | 0x80;
    *(stepAddress++) =  ITM_M3toFZUKb       & 0xff;

    // 1234
    *(stepAddress++) = (ITM_FZUStoM3 >> 8) | 0x80;
    *(stepAddress++) =  ITM_FZUStoM3       & 0xff;

    // 1235
    *(stepAddress++) = (ITM_FZUStoM3b >> 8) | 0x80;
    *(stepAddress++) =  ITM_FZUStoM3b       & 0xff;

    // 1236
    *(stepAddress++) = (ITM_M3toFZUS >> 8) | 0x80;
    *(stepAddress++) =  ITM_M3toFZUS       & 0xff;

    // 1237
    *(stepAddress++) = (ITM_M3toFZUSb >> 8) | 0x80;
    *(stepAddress++) =  ITM_M3toFZUSb       & 0xff;

    // 1238
    *(stepAddress++) = (ITM_GLUKtoM3 >> 8) | 0x80;
    *(stepAddress++) =  ITM_GLUKtoM3       & 0xff;

    // 1239
    *(stepAddress++) = (ITM_M3toGLUK >> 8) | 0x80;
    *(stepAddress++) =  ITM_M3toGLUK       & 0xff;

    // 1240
    *(stepAddress++) = (ITM_GLUStoM3 >> 8) | 0x80;
    *(stepAddress++) =  ITM_GLUStoM3       & 0xff;

    // 1241
    *(stepAddress++) = (ITM_M3toGLUS >> 8) | 0x80;
    *(stepAddress++) =  ITM_M3toGLUS       & 0xff;

    // 1242
    *(stepAddress++) = (ITM_HPEtoW >> 8) | 0x80;
    *(stepAddress++) =  ITM_HPEtoW       & 0xff;

    // 1243
    *(stepAddress++) = (ITM_WtoHPE >> 8) | 0x80;
    *(stepAddress++) =  ITM_WtoHPE       & 0xff;

    // 1244
    *(stepAddress++) = (ITM_HPMtoW >> 8) | 0x80;
    *(stepAddress++) =  ITM_HPMtoW       & 0xff;

    // 1245
    *(stepAddress++) = (ITM_WtoHPM >> 8) | 0x80;
    *(stepAddress++) =  ITM_WtoHPM       & 0xff;

    // 1246
    *(stepAddress++) = (ITM_HPUKtoW >> 8) | 0x80;
    *(stepAddress++) =  ITM_HPUKtoW       & 0xff;

    // 1247
    *(stepAddress++) = (ITM_WtoHPUK >> 8) | 0x80;
    *(stepAddress++) =  ITM_WtoHPUK       & 0xff;

    // 1248
    *(stepAddress++) = (ITM_IHGtoPA >> 8) | 0x80;
    *(stepAddress++) =  ITM_IHGtoPA       & 0xff;

    // 1249
    *(stepAddress++) = (ITM_IHGtoPA >> 8) | 0x80;
    *(stepAddress++) =  ITM_IHGtoPA       & 0xff;

    // 1250
    *(stepAddress++) = (ITM_IHGtoPAb >> 8) | 0x80;
    *(stepAddress++) =  ITM_IHGtoPAb       & 0xff;

    // 1251
    *(stepAddress++) = (ITM_PAtoIHG >> 8) | 0x80;
    *(stepAddress++) =  ITM_PAtoIHG       & 0xff;

    // 1252
    *(stepAddress++) = (ITM_PAtoIHGb >> 8) | 0x80;
    *(stepAddress++) =  ITM_PAtoIHGb       & 0xff;

    // 1253
    *(stepAddress++) = (ITM_INtoM >> 8) | 0x80;
    *(stepAddress++) =  ITM_INtoM       & 0xff;

    // 1254
    *(stepAddress++) = (ITM_MtoINCH >> 8) | 0x80;
    *(stepAddress++) =  ITM_MtoINCH       & 0xff;

    // 1255
    *(stepAddress++) = (ITM_WHtoJ >> 8) | 0x80;
    *(stepAddress++) =  ITM_WHtoJ       & 0xff;

    // 1256
    *(stepAddress++) = (ITM_JtoWH >> 8) | 0x80;
    *(stepAddress++) =  ITM_JtoWH       & 0xff;

    // 1257
    *(stepAddress++) = (ITM_KGtoLBS >> 8) | 0x80;
    *(stepAddress++) =  ITM_KGtoLBS       & 0xff;

    // 1258
    *(stepAddress++) = (ITM_LBStoKG >> 8) | 0x80;
    *(stepAddress++) =  ITM_LBStoKG       & 0xff;

    // 1259
    *(stepAddress++) = (ITM_KGtoOZ >> 8) | 0x80;
    *(stepAddress++) =  ITM_KGtoOZ       & 0xff;

    // 1260
    *(stepAddress++) = (ITM_OZtoKG >> 8) | 0x80;
    *(stepAddress++) =  ITM_OZtoKG       & 0xff;

    // 1261
    *(stepAddress++) = (ITM_KGtoSCW >> 8) | 0x80;
    *(stepAddress++) =  ITM_KGtoSCW       & 0xff;

    // 1262
    *(stepAddress++) = (ITM_KGtoSCWb >> 8) | 0x80;
    *(stepAddress++) =  ITM_KGtoSCWb       & 0xff;

    // 1263
    *(stepAddress++) = (ITM_SCWtoKG >> 8) | 0x80;
    *(stepAddress++) =  ITM_SCWtoKG       & 0xff;

    // 1264
    *(stepAddress++) = (ITM_SCWtoKGb >> 8) | 0x80;
    *(stepAddress++) =  ITM_SCWtoKGb       & 0xff;

    // 1265
    *(stepAddress++) = (ITM_KGtoSTO >> 8) | 0x80;
    *(stepAddress++) =  ITM_KGtoSTO       & 0xff;

    // 1266
    *(stepAddress++) = (ITM_KGtoSTOb >> 8) | 0x80;
    *(stepAddress++) =  ITM_KGtoSTOb       & 0xff;

    // 1267
    *(stepAddress++) = (ITM_STOtoKG >> 8) | 0x80;
    *(stepAddress++) =  ITM_STOtoKG       & 0xff;

    // 1268
    *(stepAddress++) = (ITM_STOtoKGb >> 8) | 0x80;
    *(stepAddress++) =  ITM_STOtoKGb       & 0xff;

    // 1269
    *(stepAddress++) = (ITM_KGtoST >> 8) | 0x80;
    *(stepAddress++) =  ITM_KGtoST       & 0xff;

    // 1270
    *(stepAddress++) = (ITM_KGtoSTb >> 8) | 0x80;
    *(stepAddress++) =  ITM_KGtoSTb       & 0xff;

    // 1271
    *(stepAddress++) = (ITM_KGtoSTc >> 8) | 0x80;
    *(stepAddress++) =  ITM_KGtoSTc       & 0xff;

    // 1272
    *(stepAddress++) = (ITM_STtoKG >> 8) | 0x80;
    *(stepAddress++) =  ITM_STtoKG       & 0xff;

    // 1273
    *(stepAddress++) = (ITM_STtoKGb >> 8) | 0x80;
    *(stepAddress++) =  ITM_STtoKGb       & 0xff;

    // 1274
    *(stepAddress++) = (ITM_STtoKGc >> 8) | 0x80;
    *(stepAddress++) =  ITM_STtoKGc       & 0xff;

    // 1275
    *(stepAddress++) = (ITM_KGtoTON >> 8) | 0x80;
    *(stepAddress++) =  ITM_KGtoTON       & 0xff;

    // 1276
    *(stepAddress++) = (ITM_KGtoTONb >> 8) | 0x80;
    *(stepAddress++) =  ITM_KGtoTONb       & 0xff;

    // 1277
    *(stepAddress++) = (ITM_KGtoTONc >> 8) | 0x80;
    *(stepAddress++) =  ITM_KGtoTONc       & 0xff;

    // 1278
    *(stepAddress++) = (ITM_TONtoKG >> 8) | 0x80;
    *(stepAddress++) =  ITM_TONtoKG       & 0xff;

    // 1279
    *(stepAddress++) = (ITM_TONtoKGb >> 8) | 0x80;
    *(stepAddress++) =  ITM_TONtoKGb       & 0xff;

    // 1280
    *(stepAddress++) = (ITM_TONtoKGc >> 8) | 0x80;
    *(stepAddress++) =  ITM_TONtoKGc       & 0xff;

    // 1281
    *(stepAddress++) = (ITM_KGtoTRZ >> 8) | 0x80;
    *(stepAddress++) =  ITM_KGtoTRZ       & 0xff;

    // 1282
    *(stepAddress++) = (ITM_KGtoTRZb >> 8) | 0x80;
    *(stepAddress++) =  ITM_KGtoTRZb       & 0xff;

    // 1283
    *(stepAddress++) = (ITM_TRZtoKG >> 8) | 0x80;
    *(stepAddress++) =  ITM_TRZtoKG       & 0xff;

    // 1284
    *(stepAddress++) = (ITM_TRZtoKGb >> 8) | 0x80;
    *(stepAddress++) =  ITM_TRZtoKGb       & 0xff;

    // 1285
    *(stepAddress++) = (ITM_LBFtoN >> 8) | 0x80;
    *(stepAddress++) =  ITM_LBFtoN       & 0xff;

    // 1286
    *(stepAddress++) = (ITM_NtoLBF >> 8) | 0x80;
    *(stepAddress++) =  ITM_NtoLBF       & 0xff;

    // 1287
    *(stepAddress++) = (ITM_LYtoM >> 8) | 0x80;
    *(stepAddress++) =  ITM_LYtoM       & 0xff;

    // 1288
    *(stepAddress++) = (ITM_MtoLY >> 8) | 0x80;
    *(stepAddress++) =  ITM_MtoLY       & 0xff;

    // 1289
    *(stepAddress++) = (ITM_MMHGtoPA >> 8) | 0x80;
    *(stepAddress++) =  ITM_MMHGtoPA       & 0xff;

    // 1290
    *(stepAddress++) = (ITM_MMHGtoPAb >> 8) | 0x80;
    *(stepAddress++) =  ITM_MMHGtoPAb       & 0xff;

    // 1291
    *(stepAddress++) = (ITM_PAtoMMHG >> 8) | 0x80;
    *(stepAddress++) =  ITM_PAtoMMHG       & 0xff;

    // 1292
    *(stepAddress++) = (ITM_PAtoMMHGb >> 8) | 0x80;
    *(stepAddress++) =  ITM_PAtoMMHGb       & 0xff;

    // 1293
    *(stepAddress++) = (ITM_MItoM >> 8) | 0x80;
    *(stepAddress++) =  ITM_MItoM       & 0xff;

    // 1294
    *(stepAddress++) = (ITM_MtoMI >> 8) | 0x80;
    *(stepAddress++) =  ITM_MtoMI       & 0xff;

    // 1295
    *(stepAddress++) = (ITM_MtoNMI >> 8) | 0x80;
    *(stepAddress++) =  ITM_MtoNMI       & 0xff;

    // 1296
    *(stepAddress++) = (ITM_NMItoM >> 8) | 0x80;
    *(stepAddress++) =  ITM_NMItoM       & 0xff;

    // 1297
    *(stepAddress++) = (ITM_MtoPC >> 8) | 0x80;
    *(stepAddress++) =  ITM_MtoPC       & 0xff;

    // 1298
    *(stepAddress++) = (ITM_PCtoM >> 8) | 0x80;
    *(stepAddress++) =  ITM_PCtoM       & 0xff;

    // 1299
    *(stepAddress++) = (ITM_MtoPOINTb >> 8) | 0x80;
    *(stepAddress++) =  ITM_MtoPOINTb       & 0xff;

    // 1300
    *(stepAddress++) = (ITM_MtoPOINT >> 8) | 0x80;
    *(stepAddress++) =  ITM_MtoPOINT       & 0xff;

    // 1301
    *(stepAddress++) = (ITM_POINTtoM >> 8) | 0x80;
    *(stepAddress++) =  ITM_POINTtoM       & 0xff;

    // 1302
    *(stepAddress++) = (ITM_POINTtoMb >> 8) | 0x80;
    *(stepAddress++) =  ITM_POINTtoMb       & 0xff;

    // 1303
    *(stepAddress++) = (ITM_POINTtoMc >> 8) | 0x80;
    *(stepAddress++) =  ITM_POINTtoMc       & 0xff;

    // 1304
    *(stepAddress++) = (ITM_MtoYD >> 8) | 0x80;
    *(stepAddress++) =  ITM_MtoYD       & 0xff;

    // 1305
    *(stepAddress++) = (ITM_YDtoM >> 8) | 0x80;
    *(stepAddress++) =  ITM_YDtoM       & 0xff;

    // 1306
    *(stepAddress++) = (ITM_PSItoPA >> 8) | 0x80;
    *(stepAddress++) =  ITM_PSItoPA       & 0xff;

    // 1307
    *(stepAddress++) = (ITM_PAtoPSI >> 8) | 0x80;
    *(stepAddress++) =  ITM_PAtoPSI       & 0xff;

    // 1308
    *(stepAddress++) = (ITM_PAtoTOR >> 8) | 0x80;
    *(stepAddress++) =  ITM_PAtoTOR       & 0xff;

    // 1309
    *(stepAddress++) = (ITM_PAtoTORb >> 8) | 0x80;
    *(stepAddress++) =  ITM_PAtoTORb       & 0xff;

    // 1310
    *(stepAddress++) = (ITM_TORtoPA >> 8) | 0x80;
    *(stepAddress++) =  ITM_TORtoPA       & 0xff;

    // 1311
    *(stepAddress++) = (ITM_TORtoPAb >> 8) | 0x80;
    *(stepAddress++) =  ITM_TORtoPAb       & 0xff;

    // 1312
    *(stepAddress++) = (ITM_StoYEAR >> 8) | 0x80;
    *(stepAddress++) =  ITM_StoYEAR       & 0xff;

    // 1313
    *(stepAddress++) = (ITM_YEARtoS >> 8) | 0x80;
    *(stepAddress++) =  ITM_YEARtoS       & 0xff;

    // 1314
    *(stepAddress++) = (ITM_CARATtoKG >> 8) | 0x80;
    *(stepAddress++) =  ITM_CARATtoKG       & 0xff;

    // 1315
    *(stepAddress++) = (ITM_CARATtoKGb >> 8) | 0x80;
    *(stepAddress++) =  ITM_CARATtoKGb       & 0xff;

    // 1316
    *(stepAddress++) = (ITM_CARATtoKGc >> 8) | 0x80;
    *(stepAddress++) =  ITM_CARATtoKGc       & 0xff;

    // 1317
    *(stepAddress++) = (ITM_KGtoCARAT >> 8) | 0x80;
    *(stepAddress++) =  ITM_KGtoCARAT       & 0xff;

    // 1318
    *(stepAddress++) = (ITM_KGtoCARATb >> 8) | 0x80;
    *(stepAddress++) =  ITM_KGtoCARATb       & 0xff;

    // 1319
    *(stepAddress++) = (ITM_KGtoCARATc >> 8) | 0x80;
    *(stepAddress++) =  ITM_KGtoCARATc       & 0xff;

    // 1320
    *(stepAddress++) = (ITM_QTtoM3 >> 8) | 0x80;
    *(stepAddress++) =  ITM_QTtoM3       & 0xff;

    // 1321
    *(stepAddress++) = (ITM_M3toQT >> 8) | 0x80;
    *(stepAddress++) =  ITM_M3toQT       & 0xff;

    // 1322
    *(stepAddress++) = (ITM_FATHOMtoM >> 8) | 0x80;
    *(stepAddress++) =  ITM_FATHOMtoM       & 0xff;

    // 1323
    *(stepAddress++) = (ITM_FATHOMtoMb >> 8) | 0x80;
    *(stepAddress++) =  ITM_FATHOMtoMb       & 0xff;

    // 1324
    *(stepAddress++) = (ITM_FATHOMtoMc >> 8) | 0x80;
    *(stepAddress++) =  ITM_FATHOMtoMc       & 0xff;

    // 1325
    *(stepAddress++) = (ITM_MtoFATHOM >> 8) | 0x80;
    *(stepAddress++) =  ITM_MtoFATHOM       & 0xff;

    // 1326
    *(stepAddress++) = (ITM_MtoFATHOMb >> 8) | 0x80;
    *(stepAddress++) =  ITM_MtoFATHOMb       & 0xff;

    // 1327
    *(stepAddress++) = (ITM_MtoFATHOMc >> 8) | 0x80;
    *(stepAddress++) =  ITM_MtoFATHOMc       & 0xff;

    // 1328
    *(stepAddress++) = (ITM_BARRELtoM3 >> 8) | 0x80;
    *(stepAddress++) =  ITM_BARRELtoM3       & 0xff;

    // 1329
    *(stepAddress++) = (ITM_BARRELtoM3b >> 8) | 0x80;
    *(stepAddress++) =  ITM_BARRELtoM3b       & 0xff;

    // 1330
    *(stepAddress++) = (ITM_M3toBARREL >> 8) | 0x80;
    *(stepAddress++) =  ITM_M3toBARREL       & 0xff;

    // 1331
    *(stepAddress++) = (ITM_M3toBARRELb >> 8) | 0x80;
    *(stepAddress++) =  ITM_M3toBARRELb       & 0xff;

    // 1332
    *(stepAddress++) = (ITM_ATMtoPAb >> 8) | 0x80;
    *(stepAddress++) =  ITM_ATMtoPAb       & 0xff;

    // 1333
    *(stepAddress++) = (ITM_PAtoATMb >> 8) | 0x80;
    *(stepAddress++) =  ITM_PAtoATMb       & 0xff;

    // 1334
    *(stepAddress++) = (ITM_HECTAREtoM2 >> 8) | 0x80;
    *(stepAddress++) =  ITM_HECTAREtoM2       & 0xff;

    // 1335
    *(stepAddress++) = (ITM_M2toHECTARE >> 8) | 0x80;
    *(stepAddress++) =  ITM_M2toHECTARE       & 0xff;



    // 1336
    *(stepAddress++) = (ITM_END >> 8) | 0x80;
    *(stepAddress++) =  ITM_END       & 0xff;

    #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
      printf("Prime + Bairstow + Speed + Fact + AllOp's : %u bytes\n", (uint32_t)(stepAddress - programMemoryPointer));
    #endif
  }

  // 1337
  *(stepAddress++) = 255; // .END.
  *(stepAddress++) = 255; // .END.

  freeProgramBytes = (uint8_t *)(ram + RAM_SIZE) - stepAddress;

  #ifndef DMCP_BUILD
    printf("Prime + Bairstow + Speed + Fact + AllOp's + 2 : %u bytes\n", (uint32_t)(stepAddress - programMemoryPointer));
    if((uint32_t)TO_BLOCKS((stepAddress - programMemoryPointer) + 2) > (uint32_t)TO_BLOCKS(numberOfBytesForTheTestPrograms)) {
      printf("Increase allocated memory for programs!\n");
      exit(0);
    }

    printf("freeProgramBytes = %u\n", freeProgramBytes);

    listPrograms();
    scanLabels();
    //exit(0);
  #endif // !DMCP_BUILD
}



void fnReset(uint16_t confirmation) {
  if(confirmation == NOT_CONFIRMED) {
    setConfirmationMode(fnReset);
  }
  else {
    void *memPtr;

    if(ram == NULL) {
      ram = (dataBlock_t *)malloc(TO_BYTES(RAM_SIZE));
    }
    memset(ram, 0, TO_BYTES(RAM_SIZE));
    numberOfFreeMemoryRegions = 1;
    freeMemoryRegions[0].address = 0;
    freeMemoryRegions[0].sizeInBlocks = RAM_SIZE - 1; // - 1: one block for an empty program

    // Empty program initialization
    programMemoryPointer = (uint8_t *)(ram + freeMemoryRegions[0].sizeInBlocks);
    currentProgramMemoryPointer = programMemoryPointer;
    programCounter              = programMemoryPointer;
    firstFreeProgramBytePointer = programMemoryPointer;
    firstDisplayedStepPointer   = programMemoryPointer;
    firstDisplayedStep          = 0;
    *programMemoryPointer       = 255; // .END.
    *(programMemoryPointer + 1) = 255; // .END.
    freeProgramBytes = 2;

    // "Not found glyph" initialization
    if(glyphNotFound.data == NULL) {
      glyphNotFound.data = malloc(38);
    }
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

    // Clear global flags
    memset(globalFlags, 0, sizeof(globalFlags));

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

//    shortIntegerMode = SIM_2COMPL;
      fnSetWordSize(64);
      fnIntegerMode(SIM_2COMPL);                       //JM

    groupingGap = 3;

    systemFlags = 0;
    displayFormat = DF_ALL;
    displayFormatDigits = 0;
    currentAngularMode = AM_DEGREE;
    denMax = MAX_DENMAX;
    setSystemFlag(FLAG_DENANY);
    setSystemFlag(FLAG_MULTx);
    setSystemFlag(FLAG_DECIMP);
    setSystemFlag(FLAG_AUTOFF);
    setSystemFlag(FLAG_TDM24); // time format = 24H
    setSystemFlag(FLAG_YMD);   // date format = yyyy-mm-dd
    setSystemFlag(FLAG_ASLIFT);

    hourGlassIconEnabled = false;
    watchIconEnabled = false;
    serialIOIconEnabled = false;
    printerIconEnabled = false;
    thereIsSomethingToUndo = false;
    shiftF = false;
    shiftG = false;

    significantDigits = 0;
    roundingMode = RM_HALF_EVEN;
    ctxtReal34.round = DEC_ROUND_HALF_EVEN;
    displayStack = 4;

    initFontBrowser();
    currentFlgScr = 0;
    currentRegisterBrowserScreen = 9999;
    softmenuStackPointer = 0;
    aimBuffer[0] = 0;
    lastErrorCode = 0;

    gammaLanczosCoefficients = (real51_t *)const_gammaC01;
    angle180 = (real39_t *)const_180;
    angle90  = (real39_t *)const_90;
    angle45  = (real39_t *)const_45;

    #ifndef TESTSUITE_BUILD
      resetAlphaSelectionBuffer();
    #endif

    #ifdef TESTSUITE_BUILD
      calcMode = CM_NORMAL;
    #else
      calcModeNormal();
    #endif // TESTSUITE_BUILD

    #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
      debugMemAllocation = true;
    #endif

    // RNG initialisation
    pcg32_srandom(0x1963073019931121ULL, 0x1995062319981019ULL);

    alphaSelectionMenu = ASM_NONE;
    lastFcnsMenuPos = 0;
    lastMenuMenuPos = 0;
    lastCnstMenuPos = 0;
    lastSyFlMenuPos = 0;
    lastAIntMenuPos = 0;
    firstGregorianDay = 1752;
    exponentLimit = 6145;
    lastIntegerBase = 0;
    temporaryInformation = TI_RESET;

      
//********** JM CHECKQQ Copy from the original wp43s.c configurations

        //setSystemFlag(FLAG_TDM24);
        //setSystemFlag(FLAG_MULTx);
        //setSystemFlag(FLAG_DECIMP);
        //setSystemFlag(FLAG_AUTOFF);
        //setSystemFlag(FLAG_YMD);   // date format
        clearSystemFlag(FLAG_DENANY);                              //JM Default
        fnDenMax(0);                                               //JM Default
        clearSystemFlag(FLAG_ASLIFT);  //JM??
        fnDisplayFormatAll(3);                                     //JM Default
        setSystemFlag(FLAG_SSIZE8);                                //JM Default
        setSystemFlag(FLAG_CPXRES);                                //JM Default

      //fnIntegerMode(SIM_2COMPL);
      //fnDisplayFormatGap(3);
 //    clearSystemFlag(FLAG_CPXj);
 //    fnAngularMode(AM_DEGREE);
 //    fnDisplayStack(4);
 //    clearSystemFlag(FLAG_LEAD0);
 //    fnRoundingMode(RM_HALF_EVEN);
 //    clearSystemFlag(FLAG_ALLENG);
 //  
 //    fnSetWordSize(64);
 //    clearSystemFlag(FLAG_MDY); // date format
 //    clearSystemFlag(FLAG_DMY); // date format
 //    clearSystemFlag(FLAG_POLAR);
 //    allocateLocalRegisters(0);
 //    
 //    clearSystemFlag(FLAG_FRACT);
 //    clearSystemFlag(FLAG_PROPFR);
 //    clearSystemFlag(FLAG_OVERFLOW);
 //    clearSystemFlag(FLAG_CARRY);
 //    clearSystemFlag(FLAG_USER);
 //    clearSystemFlag(FLAG_LOWBAT);
 //    clearSystemFlag(FLAG_USB);
      
      
            
#ifndef TESTSUITE_BUILD
      while(softmenuStackPointer > 0) {
          popSoftmenu();
      }
      calcModeNormal();
      if(SH_BASE_HOME && softmenuStackPointer == 0) pushSoftmenu(mm_MNU_HOME); //JM Reset to BASE MENU HOME;
#endif // TESTSUITE_BUILD
      
      //    showRealComplexResult();                                   //JM, from wp43s.c
      reset_jm_defaults(true); 
      
//********** JM CHECKQQ

      
      
    //JM Default USER
    fnUserJM(USER_RESET);                                      //JM USER
//    kbd_usr[0].primary     = KEY_CC;                         //JM CPX TEMP DEFAULT        //JM note. over-writing the content of setupdefaults
//    kbd_usr[0].gShifted    = KEY_TYPCON_UP;                  //JM TEMP DEFAULT            //JM note. over-writing the content of setupdefaults
//    kbd_usr[0].fShifted    = KEY_TYPCON_DN;                  //JM TEMP DEFAULT            //JM note. over-writing the content of setupdefaults

    // The following lines are test data
    addTestPrograms();
    //fnSetFlag(  3);
    //fnSetFlag( 11);
    //fnSetFlag( 33);
    //fnSetFlag( 34);
    //fnSetFlag( 52);
    //fnSetFlag( 62);
    //fnSetFlag( 77);
    //fnSetFlag( 85);
    //setSystemFlag(FLAG_CARRY);
    //setSystemFlag(FLAG_SPCRES);

    //allocateLocalRegisters(2);
    //fnSetFlag(FIRST_LOCAL_REGISTER+0);
    //fnSetFlag(NUMBER_OF_GLOBAL_FLAGS+2);
    //reallocateRegister(FIRST_LOCAL_REGISTER+0, dtReal16, REAL16_SIZE, RT_REAL);
    //stringToReal16("5.555", REGISTER_REAL34_DATA(FIRST_LOCAL_REGISTER));

    //strcpy(tmpStr3000, "Pure ASCII string requiring 38 bytes!");
    //reallocateRegister(FIRST_LOCAL_REGISTER+1, dtString, TO_BLOCKS(strlen(tmpStr3000) + 1), AM_NONE);
    //strcpy(REGISTER_STRING_DATA(FIRST_LOCAL_REGISTER + 1), tmpStr3000);


    //allocateNamedVariable("Z" STD_a_DIARESIS "hler");
    //allocateNamedVariable(STD_omega STD_SUB_1);
    //allocateNamedVariable(STD_omega STD_SUB_2);

    #if (DEBUG_PANEL == 1)
      debugWindow = DBG_REGISTERS;
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(chkHexaString), false);
      refreshDebugPanel();
    #endif

    //JM                                                       //JM TEMPORARY TEST DATA IN REGISTERS
    fnStrtoX("C43TEST, 2020-10-22, C43-PEM IMPORT TEST");
    fnStore(102);
    fnDrop(0);

    //JM                                                       //JM TEMPORARY TEST DATA IN REGISTERS
    fnStrtoX("Reg 11,12 & 13 have: The 3 cubes = 3.");
    fnStore(10);
    fnDrop(0);
    fnStrInputLongint("569936821221962380720");
    fnStore(11);
    fnDrop(0);
    fnStrInputLongint("-569936821113563493509");
    fnStore(12);
    fnDrop(0);
    fnStrInputLongint("-472715493453327032");
    fnStore(13);
    fnDrop(0);

    fnStrtoX("Reg 15, 16 & 17 have: The 3 cubes = 42.");
    fnStore(14);
    fnDrop(0);
    fnStrInputLongint("-80538738812075974");
    fnStore(15);
    fnDrop(0);
    fnStrInputLongint("80435758145817515");
    fnStore(16);
    fnDrop(0);
    fnStrInputLongint("12602123297335631");
    fnStore(17);
    fnDrop(0);

    fnStrtoX("37 digits of pi, Reg19 / Reg20.");
    fnStore(18);
    fnDrop(0);
    fnStrInputLongint("2646693125139304345");
    fnStore(19);
    fnDrop(0);
    fnStrInputLongint("842468587426513207");
    fnStore(20);
    fnDrop(0);

    fnStrtoX("Primes: Carol, Kynea, repunit, Woodal");
    fnStore(21);
    fnDrop(0);
    fnStrInputLongint("18014398241046527");
    fnStore(22);
    fnDrop(0);
    fnStrInputLongint("18446744082299486207");
    fnStore(23);
    fnDrop(0);
    fnStrInputLongint("7369130657357778596659");
    fnStore(24);
    fnDrop(0);
    fnStrInputLongint("195845982777569926302400511");
    fnStore(25);
    fnDrop(0);
    fnStrInputLongint("4776913109852041418248056622882488319");
    fnStore(26);
    fnDrop(0);
    fnStrInputLongint("225251798594466661409915431774713195745814267044878909733007331390393510002687");
    fnStore(27);
    fnDrop(0);

    doRefreshSoftMenu = true;     //jm dr
  }
}



void backToSystem(uint16_t unusedParamButMandatory) {
  #ifdef PC_BUILD
    fnOff(NOPARAM);
  #endif

  #ifdef DMCP_BUILD
    backToDMCP = true;
  #endif
}
