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
  uint32_t numberOfBytesForTheTestPrograms = 1264;

  resizeProgramMemory(TO_BLOCKS(numberOfBytesForTheTestPrograms));
  currentProgramMemoryPointer = programMemoryPointer;
  stepAddress = programMemoryPointer;

  { // Prime number checker
    // 1
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 5; // String length
    *(stepAddress++) = 'P';
    *(stepAddress++) = 'r';
    *(stepAddress++) = 'i';
    *(stepAddress++) = 'm';
    *(stepAddress++) = 'e';

    // 2
    *(stepAddress++) = ITM_MAGNITUDE;

    // 3
    *(stepAddress++) = ITM_IP;

    // 4
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 0;

    // 5
    *(stepAddress++) = ITM_SQUAREROOTX;

    // 6
    *(stepAddress++) = ITM_IP;

    // 7
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 2;

    // 8
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    // 9
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 1;

    // 10
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 11
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    // 12
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 13
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    // 14
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 15
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    // 16
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 17
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '4'; // THE string

    // 18
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 19
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 2;

    // 20
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    // 21
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 22
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '4'; // THE string

    // 23
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 24
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    // 25
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 26
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '4'; // THE string

    // 27
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 28
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '6'; // THE string

    // 29
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 30
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    // 31
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 32
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '6'; // THE string

    // 33
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 34
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '4'; // THE string

    // 35
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 36
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    // 37
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 38
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '4'; // THE string

    // 39
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 40
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '6'; // THE string

    // 41
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 42
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '6'; // THE string

    // 43
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 44
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    // 45
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 46
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '6'; // THE string

    // 47
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 48
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '4'; // THE string

    // 49
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 50
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    // 51
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 52
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '6'; // THE string

    // 53
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 54
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '4'; // THE string

    // 55
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 56
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '6'; // THE string

    // 57
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 58
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '8'; // THE string

    // 59
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 60
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '4'; // THE string

    // 61
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 62
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    // 63
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 64
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '4'; // THE string

    // 65
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 66
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    // 67
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 68
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '4'; // THE string

    // 69
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 70
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '8'; // THE string

    // 71
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 72
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '6'; // THE string

    // 73
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 74
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '4'; // THE string

    // 75
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 76
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '6'; // THE string

    // 77
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 78
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    // 79
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 80
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '4'; // THE string

    // 81
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 82
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '6'; // THE string

    // 83
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 84
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    // 85
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 86
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '6'; // THE string

    // 87
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 88
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '6'; // THE string

    // 89
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 90
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '4'; // THE string

    // 91
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 92
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    // 93
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 94
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '4'; // THE string

    // 94
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 96
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '6'; // THE string

    // 97
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 98
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    // 99
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 100
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '6'; // THE string

    // 101
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 102
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '4'; // THE string

    // 103
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 104
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    // 105
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 106
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '4'; // THE string

    // 107
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 108
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    // 109
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 110
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 2; // String length
    *(stepAddress++) = '1'; // THE string
    *(stepAddress++) = '0'; // THE string

    // 111
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 112
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    // 113
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 114
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 2; // String length
    *(stepAddress++) = '1'; // THE string
    *(stepAddress++) = '0'; // THE string

    // 115
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 116
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 2;

    // 117
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 1;

    // 118
    *(stepAddress++) = ITM_XLT;
    *(stepAddress++) = REGISTER_Y;

    // 119
    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 2;

    // 120
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 0;

    // 121
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 0;

    // 122
    *(stepAddress++) = (ITM_STOP >> 8) | 0x80;
    *(stepAddress++) =  ITM_STOP       & 0xff;

    // 123
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 9;

    // 124
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 1;

    // 125
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 0;

    // 126
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 1;

    // 127
    *(stepAddress++) = ITM_MOD;

    // 128
    *(stepAddress++) = ITM_XNE;
    *(stepAddress++) = VALUE_0;

    // 129
    *(stepAddress++) = ITM_RTN;

    // 130
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 0;

    // 131
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 1;

    // 132
    *(stepAddress++) = (ITM_END >> 8) | 0x80;
    *(stepAddress++) =  ITM_END       & 0xff;

    #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
      printf("Prime : %u bytes\n", (uint32_t)(stepAddress - programMemoryPointer));
    #endif
  }

  { // Bairstow polynomial root finder
    // 1
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

    // 2
    *(stepAddress++) = (ITM_ALL >> 8) | 0x80;
    *(stepAddress++) =  ITM_ALL       & 0xff;
    *(stepAddress++) = 0;

    // 3
    *(stepAddress++) = (ITM_CLREGS >> 8) | 0x80;
    *(stepAddress++) =  ITM_CLREGS       & 0xff;

    // 4
    *(stepAddress++) = (ITM_CLSTK >> 8) | 0x80;
    *(stepAddress++) =  ITM_CLSTK       & 0xff;

    // 5
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

    // 6
    *(stepAddress++) = (ITM_STOP >> 8) | 0x80;
    *(stepAddress++) =  ITM_STOP       & 0xff;

    // 7
    *(stepAddress++) = ITM_MAGNITUDE;

    // 8
    *(stepAddress++) = ITM_IP;

    // 9
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 90;

    // 10
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 91;

    // 11
    *(stepAddress++) = (ITM_CLSTK >> 8) | 0x80;
    *(stepAddress++) =  ITM_CLSTK       & 0xff;

    // 12
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 91;

    // 13
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 3; // String length
    *(stepAddress++) = '1'; // THE string
    *(stepAddress++) = 'e';
    *(stepAddress++) = '3';

    // 14
    *(stepAddress++) = ITM_DIV;

    // 15
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 92;

    // 16
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 100 + 'A' - 'A'; // A

    // 17
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_LABEL_VARIABLE;
    *(stepAddress++) = 2; // String length
    *(stepAddress++) = 'x'; // THE string
    *(stepAddress++) = '^';

    // 18
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 91;

    // 19
    *(stepAddress++) = ITM_ADD;

    // 20
    *(stepAddress++) = (ITM_STOP >> 8) | 0x80;
    *(stepAddress++) =  ITM_STOP       & 0xff;

    // 21
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 93;

    // 22
    *(stepAddress++) = (ITM_CLSTK >> 8) | 0x80;
    *(stepAddress++) =  ITM_CLSTK       & 0xff;

    // 23
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    // 24
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 93;

    // 25
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 93;

    // 26
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 92;

    // 27
    *(stepAddress++) = ITM_ISG;
    *(stepAddress++) = 92;

    // 28
    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 100 + 'A' - 'A'; // A

    // 29
    *(stepAddress++) = (ITM_CLSTK >> 8) | 0x80;
    *(stepAddress++) =  ITM_CLSTK       & 0xff;

    // 30
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    // 31
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 0;

    // 32
    *(stepAddress++) = ITM_XNE;
    *(stepAddress++) = REGISTER_Y;

    // 33
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 100 + 'B' - 'A'; // B

    // 34
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

    // 35
    *(stepAddress++) = (ITM_STOP >> 8) | 0x80;
    *(stepAddress++) =  ITM_STOP       & 0xff;

    // 36
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 99;

    // 37
    *(stepAddress++) = (ITM_CLSTK >> 8) | 0x80;
    *(stepAddress++) =  ITM_CLSTK       & 0xff;

    // 38
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 10;

    // 39
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '3'; // THE string

    // 40
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 94;

    // 41
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 94;

    // 42
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 90;

    // 43
    *(stepAddress++) = ITM_XLT;
    *(stepAddress++) = REGISTER_X;

    // 44
    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 2;

    // 45
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '0'; // THE string

    // 46
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 95;

    // 47
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 96;

    // 48
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 100 + 'D' - 'A'; // D

    // 49
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 100 + 'J' - 'A'; // J

    // 50
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 11;

    // 51
    *(stepAddress++) = (ITM_STOP >> 8) | 0x80;
    *(stepAddress++) =  ITM_STOP       & 0xff;

    // 52
    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 10;

    // 53
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 2;

    // 54
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    // 55
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 94;

    // 56
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 94;

    // 57
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 90;

    // 58
    *(stepAddress++) = ITM_XNE;
    *(stepAddress++) = REGISTER_Y;

    // 59
    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 1;

    // 60
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 1;

    // 61
    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 90;

    // 62
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 1;

    // 63
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 13;

    // 64
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 90;

    // 65
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

    // 66
    *(stepAddress++) = ITM_RTN;

    // 67
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 100 + 'B' - 'A'; // B

    // 68
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    // 69
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 93;

    // 70
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 90;

    // 71
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 3; // String length
    *(stepAddress++) = '1'; // THE string
    *(stepAddress++) = 'e';
    *(stepAddress++) = '3';

    // 72
    *(stepAddress++) = ITM_DIV;

    // 73
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 92;

    // 74
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 100 + 'C' - 'A'; // C

    // 75
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 0;

    // 76
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 93;

    // 77
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    // 78
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 93;

    // 79
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 92;

    // 80
    *(stepAddress++) = ITM_ISG;
    *(stepAddress++) = 92;

    // 81
    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 100 + 'C' - 'A'; // C

    // 82
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    // 83
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 0;

    // 84
    *(stepAddress++) = ITM_RTN;

    // 85
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 100 + 'D' - 'A'; // D

    // 86
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    // 87
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 97;

    // 88
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '0'; // THE string

    // 89
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 98;

    // 90
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 100 + 'I' - 'A'; // I

    // 91
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    // 92
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 91;

    // 93
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 90;

    // 94
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 3; // String length
    *(stepAddress++) = '1'; // THE string
    *(stepAddress++) = 'e';
    *(stepAddress++) = '3';

    // 95
    *(stepAddress++) = ITM_DIV;

    // 96
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    // 97
    *(stepAddress++) = ITM_ADD;

    // 98
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 89;

    // 98
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 2; // String length
    *(stepAddress++) = '3'; // THE string
    *(stepAddress++) = '0'; // THE string

    // 100
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 92;

    // 101
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 93;

    // 102
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 94;

    // 103
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    // 104
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 93;

    // 105
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 94;

    // 106
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 94;

    // 107
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '0'; // THE string

    // 108
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 92;

    // 109
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    // 110
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 93;

    // 111
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 100 + 'E' - 'A'; // E

    // 112
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 91;

    // 113
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 94;

    // 114
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 93;

    // 115
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 95;

    // 116
    *(stepAddress++) = ITM_MULT;

    // 117
    *(stepAddress++) = ITM_CHS;

    // 118
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 94;

    // 119
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 92;

    // 120
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 96;

    // 121
    *(stepAddress++) = ITM_MULT;

    // 122
    *(stepAddress++) = ITM_CHS;

    // 123
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 94;

    // 124
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    // 125
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 91;

    // 126
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 92;

    // 127
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 93;

    // 128
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 94;

    // 129
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 89;

    // 130
    *(stepAddress++) = ITM_ISG;
    *(stepAddress++) = 89;

    // 131
    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 100 + 'E' - 'A'; // E

    // 132
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 92;

    // 133
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 84;

    // 134
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 93;

    // 135
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 85;

    // 136
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 2; // String length
    *(stepAddress++) = '3'; // THE string
    *(stepAddress++) = '0'; // THE string

    // 137
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 91;

    // 138
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 90;

    // 139
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 3; // String length
    *(stepAddress++) = '1'; // THE string
    *(stepAddress++) = 'e';
    *(stepAddress++) = '3';

    // 140
    *(stepAddress++) = ITM_DIV;

    // 141
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    // 142
    *(stepAddress++) = ITM_ADD;

    // 143
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 89;

    // 144
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 2; // String length
    *(stepAddress++) = '6'; // THE string
    *(stepAddress++) = '0'; // THE string

    // 145
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 92;

    // 146
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 93;

    // 147
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 94;

    // 148
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    // 149
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 93;

    // 150
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 94;

    // 151
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 94;

    // 152
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '0'; // THE string

    // 153
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 92;

    // 154
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '0'; // THE string

    // 155
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 93;

    // 156
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 100 + 'F' - 'A'; // F

    // 157
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 91;

    // 158
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 94;

    // 159
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 93;

    // 160
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 95;

    // 161
    *(stepAddress++) = ITM_MULT;

    // 162
    *(stepAddress++) = ITM_CHS;

    // 163
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 94;

    // 164
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 92;

    // 165
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 96;

    // 166
    *(stepAddress++) = ITM_MULT;

    // 167
    *(stepAddress++) = ITM_CHS;

    // 168
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 94;

    // 169
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    // 170
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 91;

    // 171
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 92;

    // 172
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 93;

    // 173
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 94;

    // 174
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 89;

    // 175
    *(stepAddress++) = ITM_ISG;
    *(stepAddress++) = 89;

    // 176
    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 100 + 'F' - 'A'; // F

    // 177
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 92;

    // 178
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 92;

    // 179
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 93;

    // 180
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 93;

    // 181
    *(stepAddress++) = (ITM_CLSTK >> 8) | 0x80;
    *(stepAddress++) =  ITM_CLSTK       & 0xff;

    // 182
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 95;

    // 183
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 93;

    // 184
    *(stepAddress++) = ITM_MULT;

    // 185
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 96;

    // 186
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 92;

    // 187
    *(stepAddress++) = ITM_MULT;

    // 188
    *(stepAddress++) = ITM_ADD;

    // 189
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 92;

    // 190
    *(stepAddress++) = ITM_MULT;

    // 191
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 93;

    // 192
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 93;

    // 193
    *(stepAddress++) = ITM_MULT;

    // 194
    *(stepAddress++) = ITM_ADD;

    // 195
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 88;

    // 196
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 84;

    // 197
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 93;

    // 198
    *(stepAddress++) = ITM_MULT;

    // 199
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 85;

    // 200
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 92;

    // 201
    *(stepAddress++) = ITM_MULT;

    // 202
    *(stepAddress++) = ITM_SUB;

    // 203
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 88;

    // 204
    *(stepAddress++) = ITM_DIV;

    // 205
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 87;

    // 206
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 95;

    // 207
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 93;

    // 208
    *(stepAddress++) = ITM_MULT;

    // 209
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 96;

    // 210
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 92;

    // 211
    *(stepAddress++) = ITM_MULT;

    // 212
    *(stepAddress++) = ITM_ADD;

    // 213
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 84;

    // 214
    *(stepAddress++) = ITM_MULT;

    // 215
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 85;

    // 216
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 93;

    // 217
    *(stepAddress++) = ITM_MULT;

    // 218
    *(stepAddress++) = ITM_ADD;

    // 219
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 88;

    // 220
    *(stepAddress++) = ITM_DIV;

    // 221
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 86;

    // 222
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 87;

    // 223
    *(stepAddress++) = ITM_MAGNITUDE;

    // 224
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = REGISTER_X;

    // 225
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 86;

    // 226
    *(stepAddress++) = ITM_MAGNITUDE;

    // 227
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = REGISTER_Y;

    // 228
    *(stepAddress++) = ITM_XGT;
    *(stepAddress++) = REGISTER_Y;

    // 229
    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 100 + 'G' - 'A'; // G

    // 230
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 87;

    // 231
    *(stepAddress++) = ITM_MAGNITUDE;

    // 232
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 97;

    // 233
    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 100 + 'H' - 'A'; // H

    // 234
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 100 + 'G' - 'A'; // G

    // 235
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 86;

    // 236
    *(stepAddress++) = ITM_MAGNITUDE;

    // 237
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 97;

    // 238
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 100 + 'H' - 'A'; // H

    // 239
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 87;

    // 240
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 95;

    // 241
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 86;

    // 242
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 96;

    // 243
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    // 244
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 98;

    // 245
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 3; // String length
    *(stepAddress++) = '1'; // THE string
    *(stepAddress++) = 'e';
    *(stepAddress++) = '4';

    // 246
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = REGISTER_X;

    // 247
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 98;

    // 248
    *(stepAddress++) = ITM_XGT;
    *(stepAddress++) = REGISTER_Y;

    // 249
    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 4;

    // 250
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 99;

    // 251
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 97;

    // 252
    *(stepAddress++) = ITM_XGT;
    *(stepAddress++) = REGISTER_Y;

    // 253
    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 100 + 'I' - 'A';

    // 254
    *(stepAddress++) = ITM_RTN;

    // 255
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 100 + 'J' - 'A'; // J

    // 256
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '0'; // THE string

    // 257
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 91;

    // 258
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 2; // String length
    *(stepAddress++) = '3'; // THE string
    *(stepAddress++) = '1'; // THE string

    // 259
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 92;

    // 260
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 90;

    // 261
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    // 262
    *(stepAddress++) = ITM_SUB;

    // 263
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 93;

    // 264
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 3; // String length
    *(stepAddress++) = '1'; // THE string
    *(stepAddress++) = 'e';
    *(stepAddress++) = '3';

    // 265
    *(stepAddress++) = ITM_DIV;

    // 266
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 94;

    // 267
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 88;

    // 268
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 92;

    // 269
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 91;

    // 270
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    // 271
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 91;

    // 272
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 92;

    // 273
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 94;

    // 274
    *(stepAddress++) = ITM_ISG;
    *(stepAddress++) = 94;

    // 275
    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 88;

    // 276
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 90;

    // 277
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    // 278
    *(stepAddress++) = ITM_SUB;

    // 279
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 90;

    // 280
    *(stepAddress++) = ITM_RTN;

    // 281
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 11;

    // 282
    *(stepAddress++) = (ITM_CLSTK >> 8) | 0x80;
    *(stepAddress++) =  ITM_CLSTK       & 0xff;

    // 283
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 95;

    // 284
    *(stepAddress++) = ITM_CHS;

    // 285
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    // 286
    *(stepAddress++) = ITM_DIV;

    // 287
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 91;

    // 288
    *(stepAddress++) = ITM_SQUARE;

    // 289
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 96;

    // 290
    *(stepAddress++) = ITM_SUB;

    // 291
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 92;

    // 292
    *(stepAddress++) = (ITM_CLSTK >> 8) | 0x80;
    *(stepAddress++) =  ITM_CLSTK       & 0xff;

    // 293
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 91;

    // 294
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 92;

    // 295
    *(stepAddress++) = ITM_SQUAREROOTX;

    // 296
    *(stepAddress++) = ITM_ADD;

    // 297
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 91;

    // 298
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 92;

    // 299
    *(stepAddress++) = ITM_SQUAREROOTX;

    // 300
    *(stepAddress++) = ITM_SUB;

    // 301
    *(stepAddress++) = ITM_RTN;

    // 302
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 12;

    // 303
    *(stepAddress++) = (ITM_CLSTK >> 8) | 0x80;
    *(stepAddress++) =  ITM_CLSTK       & 0xff;

    // 304
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

    // 305
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 1;

    // 306
    *(stepAddress++) = ITM_CHS;

    // 307
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    // 308
    *(stepAddress++) = ITM_DIV;

    // 309
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 91;

    // 310
    *(stepAddress++) = ITM_SQUARE;

    // 311
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 2;

    // 312
    *(stepAddress++) = ITM_SUB;

    // 313
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 92;

    // 314
    *(stepAddress++) = (ITM_CLSTK >> 8) | 0x80;
    *(stepAddress++) =  ITM_CLSTK       & 0xff;

    // 315
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 91;

    // 316
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 92;

    // 317
    *(stepAddress++) = ITM_SQUAREROOTX;

    // 318
    *(stepAddress++) = ITM_ADD;

    // 319
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 91;

    // 320
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 92;

    // 321
    *(stepAddress++) = ITM_SQUAREROOTX;

    // 322
    *(stepAddress++) = ITM_SUB;

    // 323
    *(stepAddress++) = ITM_RTN;

    // 324
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 13;

    // 325
    *(stepAddress++) = (ITM_CLSTK >> 8) | 0x80;
    *(stepAddress++) =  ITM_CLSTK       & 0xff;

    // 326
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

    // 327
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 1;

    // 328
    *(stepAddress++) = ITM_CHS;

    // 329
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 91;

    // 330
    *(stepAddress++) = (ITM_CLSTK >> 8) | 0x80;
    *(stepAddress++) =  ITM_CLSTK       & 0xff;

    // 331
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 91;

    // 332
    *(stepAddress++) = ITM_RTN;

    // 333
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 4;

    // 334
    *(stepAddress++) = (ITM_CLSTK >> 8) | 0x80;
    *(stepAddress++) =  ITM_CLSTK       & 0xff;

    // 335
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

    // 336
    *(stepAddress++) = ITM_RTN;

    // 337
    *(stepAddress++) = (ITM_END >> 8) | 0x80;
    *(stepAddress++) =  ITM_END       & 0xff;

    #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
      printf("Prime + Bairstow : %u bytes\n", (uint32_t)(stepAddress - programMemoryPointer));
    #endif
  }

  { // Speed test. See: https://forum.swissmicros.com/viewtopic.php?p=17308
    // 1
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 5; // String length
    *(stepAddress++) = 'S';
    *(stepAddress++) = 'p';
    *(stepAddress++) = 'e';
    *(stepAddress++) = 'e';
    *(stepAddress++) = 'd';

    // 2
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 10;

    // 3
    *(stepAddress++) = (ITM_TICKS >> 8) | 0x80;
    *(stepAddress++) =  ITM_TICKS       & 0xff;

    // 4
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 0;

    // 5
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '0'; // THE string

    // 6
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 11;

    // 7
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    // 8
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 12;

    // 9
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 1;

    // 10
    *(stepAddress++) = ITM_arctan;

    // 11
    *(stepAddress++) = ITM_sin;

    // 12
    *(stepAddress++) = ITM_EXP;

    // 13
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '3'; // THE string

    // 14
    *(stepAddress++) = ITM_1ONX;

    // 15
    *(stepAddress++) = ITM_YX;

    // 16
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 11;

    // 17
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 12;

    // 18
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    // 19
    *(stepAddress++) = ITM_ADD;

    // 20
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 12;

    // 21
    *(stepAddress++) = ITM_DSE;
    *(stepAddress++) = 10;

    // 22
    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 1;

    // 23
    *(stepAddress++) = (ITM_TICKS >> 8) | 0x80;
    *(stepAddress++) =  ITM_TICKS       & 0xff;

    // 24
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 0;

    // 25
    *(stepAddress++) = ITM_SUB;

    // 26
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 2; // String length
    *(stepAddress++) = '1'; // THE string
    *(stepAddress++) = '0';

    // 27
    *(stepAddress++) = ITM_DIV;

    // 28
    *(stepAddress++) = (ITM_END >> 8) | 0x80;
    *(stepAddress++) =  ITM_END       & 0xff;

    #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
      printf("Prime + Bairstow + Speed : %u bytes\n", (uint32_t)(stepAddress - programMemoryPointer));
    #endif
  }

  *(stepAddress++) = 255; // .END.
  *(stepAddress++) = 255; // .END.

  #if defined(DMCP_BUILD)
    freeProgramBytes = (4 - ((uint32_t)stepAddress)) % 4;
  #else
    freeProgramBytes = (4 - ((uint64_t)stepAddress)) % 4;
  #endif

  #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
    printf("Prime + Bairstow + Speed + 2 : %u bytes\n", (uint32_t)(stepAddress - programMemoryPointer));
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
