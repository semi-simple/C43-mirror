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
  uint32_t numberOfBytesForTheTestPrograms = 1304; // 326*4

  resizeProgramMemory(TO_BLOCKS(numberOfBytesForTheTestPrograms));
  currentProgramMemoryPointer = programMemoryPointer;
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
    *(stepAddress++) = REGISTER_X;

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

  // 511
  *(stepAddress++) = 255; // .END.
  *(stepAddress++) = 255; // .END.

  #if defined(DMCP_BUILD)
    freeProgramBytes = (4 - ((uint32_t)stepAddress)) % 4;
  #else
    freeProgramBytes = (4 - ((uint64_t)stepAddress)) % 4;
  #endif

  #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
    printf("Prime + Bairstow + Speed + Fact + 2 : %u bytes\n", (uint32_t)(stepAddress - programMemoryPointer));
    if((uint32_t)TO_BLOCKS((stepAddress - programMemoryPointer) + 2) > (uint32_t)TO_BLOCKS(numberOfBytesForTheTestPrograms)) {
      printf("Increase allocated memory for programs!\n");
      exit(0);
    }
    listPrograms();
  #endif
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

    shortIntegerWordSize = 64;
    shortIntegerMode = SIM_2COMPL;

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
    addTestPrograms();

    // The following lines are test data
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
