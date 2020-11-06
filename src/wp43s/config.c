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
  uint8_t *stepAddress;
  uint32_t numberOfBytesForTheTestPrograms = 2048 * 4; // Multiple of 4

  resizeProgramMemory(TO_BLOCKS(numberOfBytesForTheTestPrograms));
  currentProgramMemoryPointer = programMemoryPointer;
  firstDisplayedStepPointer   = programMemoryPointer;
  stepAddress                 = programMemoryPointer;
  currentStep                 = 0;
  firstDisplayedStep          = 0;
/*
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

    *(stepAddress++) = ITM_MAGNITUDE;

    *(stepAddress++) = ITM_IP;

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_SQUAREROOTX;

    *(stepAddress++) = ITM_IP;

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 2;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 1;

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    // 10
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '4'; // THE string

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 2;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    // 20
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '4'; // THE string

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '4'; // THE string

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '6'; // THE string

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    // 30
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '6'; // THE string

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '4'; // THE string

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '4'; // THE string

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '6'; // THE string

    // 40
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '6'; // THE string

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '6'; // THE string

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '4'; // THE string

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    // 50
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '6'; // THE string

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '4'; // THE string

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '6'; // THE string

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '8'; // THE string

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '4'; // THE string

    // 60
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '4'; // THE string

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '4'; // THE string

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '8'; // THE string

    // 70
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '6'; // THE string

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '4'; // THE string

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '6'; // THE string

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '4'; // THE string

    // 80
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '6'; // THE string

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '6'; // THE string

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '6'; // THE string

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '4'; // THE string

    // 90
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '4'; // THE string

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '6'; // THE string

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '6'; // THE string

    // 100
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '4'; // THE string

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '4'; // THE string

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 2; // String length
    *(stepAddress++) = '1'; // THE string
    *(stepAddress++) = '0'; // THE string

    // 110
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 2; // String length
    *(stepAddress++) = '1'; // THE string
    *(stepAddress++) = '0'; // THE string

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 2;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 1;

    *(stepAddress++) = ITM_XLT;
    *(stepAddress++) = REGISTER_Y;

    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 2;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 0;

    // 120
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 0;

    *(stepAddress++) = (ITM_STOP >> 8) | 0x80;
    *(stepAddress++) =  ITM_STOP       & 0xff;

    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 9;

    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 1;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 1;

    *(stepAddress++) = ITM_MOD;

    *(stepAddress++) = ITM_XNE;
    *(stepAddress++) = VALUE_0;

    *(stepAddress++) = ITM_RTN;

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

  { // Len1
    // 132
    *(stepAddress++) = (ITM_END >> 8) | 0x80;
    *(stepAddress++) =  ITM_END       & 0xff;

    #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
      printf("Prime + Len1 : %u bytes\n", (uint32_t)(stepAddress - programMemoryPointer));
    #endif
  }

  { // Len2
    // 133
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 4; // String length
    *(stepAddress++) = 'L';
    *(stepAddress++) = 'e';
    *(stepAddress++) = 'n';
    *(stepAddress++) = '2';

    // 134
    *(stepAddress++) = (ITM_END >> 8) | 0x80;
    *(stepAddress++) =  ITM_END       & 0xff;

    #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
      printf("Prime + Len1 + Len2 : %u bytes\n", (uint32_t)(stepAddress - programMemoryPointer));
    #endif
  }

  { // Len3
    // 135
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 4; // String length
    *(stepAddress++) = 'L';
    *(stepAddress++) = 'e';
    *(stepAddress++) = 'n';
    *(stepAddress++) = '3';

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 2;

    // 137
    *(stepAddress++) = (ITM_END >> 8) | 0x80;
    *(stepAddress++) =  ITM_END       & 0xff;

    #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
      printf("Prime + Len1 + Len2 + Len3 : %u bytes\n", (uint32_t)(stepAddress - programMemoryPointer));
    #endif
  }

  { // Len4
    // 138
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 4; // String length
    *(stepAddress++) = 'L';
    *(stepAddress++) = 'e';
    *(stepAddress++) = 'n';
    *(stepAddress++) = '4';

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 2;

    // 140
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 3;

    // 131
    *(stepAddress++) = (ITM_END >> 8) | 0x80;
    *(stepAddress++) =  ITM_END       & 0xff;

    #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
      printf("Prime + Len1 + Len2 + Len3 + Len4 : %u bytes\n", (uint32_t)(stepAddress - programMemoryPointer));
    #endif
  }

  { // Len5
    // 142
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 4; // String length
    *(stepAddress++) = 'L';
    *(stepAddress++) = 'e';
    *(stepAddress++) = 'n';
    *(stepAddress++) = '5';

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 2;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 3;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 4;

    // 146
    *(stepAddress++) = (ITM_END >> 8) | 0x80;
    *(stepAddress++) =  ITM_END       & 0xff;

    #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
      printf("Prime + Len1 + Len2 + Len3 + Len4 + Len5 : %u bytes\n", (uint32_t)(stepAddress - programMemoryPointer));
    #endif
  }

  { // Len6
    // 147
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 4; // String length
    *(stepAddress++) = 'L';
    *(stepAddress++) = 'e';
    *(stepAddress++) = 'n';
    *(stepAddress++) = '6';

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 2;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 3;

    // 150
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 4;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 5;

    // 152
    *(stepAddress++) = (ITM_END >> 8) | 0x80;
    *(stepAddress++) =  ITM_END       & 0xff;

    #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
      printf("Prime + Len1 + Len2 + Len3 + Len4 + Len5 + Len6 : %u bytes\n", (uint32_t)(stepAddress - programMemoryPointer));
    #endif
  }

  { // Len7
    // 153
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 4; // String length
    *(stepAddress++) = 'L';
    *(stepAddress++) = 'e';
    *(stepAddress++) = 'n';
    *(stepAddress++) = '7';

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 2;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 3;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 4;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 5;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 6;

    // 159
    *(stepAddress++) = (ITM_END >> 8) | 0x80;
    *(stepAddress++) =  ITM_END       & 0xff;

    #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
      printf("Prime + Len1 + Len2 + Len3 + Len4 + Len5 + Len6 + Len7 : %u bytes\n", (uint32_t)(stepAddress - programMemoryPointer));
    #endif
  }

  { // Len8
    // 160
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 4; // String length
    *(stepAddress++) = 'L';
    *(stepAddress++) = 'e';
    *(stepAddress++) = 'n';
    *(stepAddress++) = '8';

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 2;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 3;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 4;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 5;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 6;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 7;

    // 167
    *(stepAddress++) = (ITM_END >> 8) | 0x80;
    *(stepAddress++) =  ITM_END       & 0xff;

    #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
      printf("Prime + Len1 + Len2 + Len3 + Len4 + Len5 + Len6 + Len7 + Len8 : %u bytes\n", (uint32_t)(stepAddress - programMemoryPointer));
    #endif
  }

  { // Bairstow polynomial root finder
    // 168
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

    *(stepAddress++) = (ITM_ALL >> 8) | 0x80;
    *(stepAddress++) =  ITM_ALL       & 0xff;
    *(stepAddress++) = 0;

    // 170
    *(stepAddress++) = (ITM_CLREGS >> 8) | 0x80;
    *(stepAddress++) =  ITM_CLREGS       & 0xff;

    *(stepAddress++) = (ITM_CLSTK >> 8) | 0x80;
    *(stepAddress++) =  ITM_CLSTK       & 0xff;

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

    *(stepAddress++) = (ITM_STOP >> 8) | 0x80;
    *(stepAddress++) =  ITM_STOP       & 0xff;

    *(stepAddress++) = ITM_MAGNITUDE;

    *(stepAddress++) = ITM_IP;

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 90;

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 91;

    *(stepAddress++) = (ITM_CLSTK >> 8) | 0x80;
    *(stepAddress++) =  ITM_CLSTK       & 0xff;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 91;

    // 180
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 3; // String length
    *(stepAddress++) = '1'; // THE string
    *(stepAddress++) = 'e';
    *(stepAddress++) = '3';

    *(stepAddress++) = ITM_DIV;

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 92;

    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 100 + 'A' - 'A'; // A

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_LABEL_VARIABLE;
    *(stepAddress++) = 2; // String length
    *(stepAddress++) = 'x'; // THE string
    *(stepAddress++) = '^';

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 91;

    *(stepAddress++) = ITM_ADD;

    *(stepAddress++) = (ITM_STOP >> 8) | 0x80;
    *(stepAddress++) =  ITM_STOP       & 0xff;

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 93;

    *(stepAddress++) = (ITM_CLSTK >> 8) | 0x80;
    *(stepAddress++) =  ITM_CLSTK       & 0xff;

    // 190
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 93;

    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 93;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 92;

    *(stepAddress++) = ITM_ISG;
    *(stepAddress++) = 92;

    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 100 + 'A' - 'A'; // A

    *(stepAddress++) = (ITM_CLSTK >> 8) | 0x80;
    *(stepAddress++) =  ITM_CLSTK       & 0xff;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_XNE;
    *(stepAddress++) = REGISTER_Y;

    // 200
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 100 + 'B' - 'A'; // B

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

    *(stepAddress++) = (ITM_STOP >> 8) | 0x80;
    *(stepAddress++) =  ITM_STOP       & 0xff;

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 99;

    *(stepAddress++) = (ITM_CLSTK >> 8) | 0x80;
    *(stepAddress++) =  ITM_CLSTK       & 0xff;

    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 10;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '3'; // THE string

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 94;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 94;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 90;

    // 210
    *(stepAddress++) = ITM_XLT;
    *(stepAddress++) = REGISTER_Y;

    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 2;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '0'; // THE string

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 95;

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 96;

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 100 + 'D' - 'A'; // D

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 100 + 'J' - 'A'; // J

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 11;

    *(stepAddress++) = (ITM_STOP >> 8) | 0x80;
    *(stepAddress++) =  ITM_STOP       & 0xff;

    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 10;

    // 220
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 2;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 94;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 94;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 90;

    *(stepAddress++) = ITM_XNE;
    *(stepAddress++) = REGISTER_Y;

    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 1;

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 1;

    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 90;

    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 1;

    // 230
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 13;

    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 90;

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

    *(stepAddress++) = ITM_RTN;

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 100 + 'B' - 'A'; // B

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 93;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 90;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 3; // String length
    *(stepAddress++) = '1'; // THE string
    *(stepAddress++) = 'e';
    *(stepAddress++) = '3';

    *(stepAddress++) = ITM_DIV;

    // 240
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 92;

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 100 + 'C' - 'A'; // C

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 93;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 93;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 92;

    *(stepAddress++) = ITM_ISG;
    *(stepAddress++) = 92;

    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 100 + 'C' - 'A'; // C

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    // 250
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_RTN;

    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 100 + 'D' - 'A'; // D

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 97;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '0'; // THE string

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 98;

    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 100 + 'I' - 'A'; // I

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 91;

    // 260
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 90;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 3; // String length
    *(stepAddress++) = '1'; // THE string
    *(stepAddress++) = 'e';
    *(stepAddress++) = '3';

    *(stepAddress++) = ITM_DIV;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    *(stepAddress++) = ITM_ADD;

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 89;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 2; // String length
    *(stepAddress++) = '3'; // THE string
    *(stepAddress++) = '0'; // THE string

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 92;

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 93;

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 94;

    // 270
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 93;

    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 94;

    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 94;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '0'; // THE string

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 92;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 93;

    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 100 + 'E' - 'A'; // E

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 91;

    // 280
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 94;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 93;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 95;

    *(stepAddress++) = ITM_MULT;

    *(stepAddress++) = ITM_CHS;

    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 94;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 92;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 96;

    *(stepAddress++) = ITM_MULT;

    *(stepAddress++) = ITM_CHS;

    // 290
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 94;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 91;

    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 92;

    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 93;

    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 94;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 89;

    *(stepAddress++) = ITM_ISG;
    *(stepAddress++) = 89;

    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 100 + 'E' - 'A'; // E

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 92;

    // 300
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 84;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 93;

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 85;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 2; // String length
    *(stepAddress++) = '3'; // THE string
    *(stepAddress++) = '0'; // THE string

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 91;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 90;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 3; // String length
    *(stepAddress++) = '1'; // THE string
    *(stepAddress++) = 'e';
    *(stepAddress++) = '3';

    *(stepAddress++) = ITM_DIV;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    *(stepAddress++) = ITM_ADD;

    // 310
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 89;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 2; // String length
    *(stepAddress++) = '6'; // THE string
    *(stepAddress++) = '0'; // THE string

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 92;

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 93;

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 94;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 93;

    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 94;

    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 94;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '0'; // THE string

    // 320
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 92;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '0'; // THE string

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 93;

    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 100 + 'F' - 'A'; // F

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 91;

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 94;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 93;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 95;

    *(stepAddress++) = ITM_MULT;

    *(stepAddress++) = ITM_CHS;

    // 330
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 94;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 92;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 96;

    *(stepAddress++) = ITM_MULT;

    *(stepAddress++) = ITM_CHS;

    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 94;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 91;

    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 92;

    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 93;

    // 340
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 94;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 89;

    *(stepAddress++) = ITM_ISG;
    *(stepAddress++) = 89;

    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 100 + 'F' - 'A'; // F

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 92;

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 92;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 93;

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 93;

    *(stepAddress++) = (ITM_CLSTK >> 8) | 0x80;
    *(stepAddress++) =  ITM_CLSTK       & 0xff;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 95;

    // 350
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 93;

    *(stepAddress++) = ITM_MULT;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 96;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 92;

    *(stepAddress++) = ITM_MULT;

    *(stepAddress++) = ITM_ADD;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 92;

    *(stepAddress++) = ITM_MULT;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 93;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 93;

    // 360
    *(stepAddress++) = ITM_MULT;

    *(stepAddress++) = ITM_ADD;

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 88;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 84;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 93;

    *(stepAddress++) = ITM_MULT;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 85;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 92;

    *(stepAddress++) = ITM_MULT;

    *(stepAddress++) = ITM_SUB;

    // 370
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 88;

    *(stepAddress++) = ITM_DIV;

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 87;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 95;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 93;

    *(stepAddress++) = ITM_MULT;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 96;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 92;

    *(stepAddress++) = ITM_MULT;

    *(stepAddress++) = ITM_ADD;

    // 380
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 84;

    *(stepAddress++) = ITM_MULT;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 85;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 93;

    *(stepAddress++) = ITM_MULT;

    *(stepAddress++) = ITM_ADD;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 88;

    *(stepAddress++) = ITM_DIV;

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 86;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 87;

    // 390
    *(stepAddress++) = ITM_MAGNITUDE;

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 86;

    *(stepAddress++) = ITM_MAGNITUDE;

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = REGISTER_Y;

    *(stepAddress++) = ITM_XGT;
    *(stepAddress++) = REGISTER_Y;

    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 100 + 'G' - 'A'; // G

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 87;

    *(stepAddress++) = ITM_MAGNITUDE;

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 97;

    // 400
    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 100 + 'H' - 'A'; // H

    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 100 + 'G' - 'A'; // G

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 86;

    *(stepAddress++) = ITM_MAGNITUDE;

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 97;

    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 100 + 'H' - 'A'; // H

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 87;

    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 95;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 86;

    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 96;

    // 410
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 98;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 3; // String length
    *(stepAddress++) = '1'; // THE string
    *(stepAddress++) = 'e';
    *(stepAddress++) = '4';

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 98;

    *(stepAddress++) = ITM_XGT;
    *(stepAddress++) = REGISTER_Y;

    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 4;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 97;

    *(stepAddress++) = ITM_XGT;
    *(stepAddress++) = REGISTER_Y;

    // 420
    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 100 + 'I' - 'A';

    *(stepAddress++) = ITM_RTN;

    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 100 + 'J' - 'A'; // J

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '0'; // THE string

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 91;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 2; // String length
    *(stepAddress++) = '3'; // THE string
    *(stepAddress++) = '1'; // THE string

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 92;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 90;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    *(stepAddress++) = ITM_SUB;

    // 430
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 93;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 3; // String length
    *(stepAddress++) = '1'; // THE string
    *(stepAddress++) = 'e';
    *(stepAddress++) = '3';

    *(stepAddress++) = ITM_DIV;

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 94;

    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 88;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 92;

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 91;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 91;

    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 92;

    // 440
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 94;

    *(stepAddress++) = ITM_ISG;
    *(stepAddress++) = 94;

    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 88;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 90;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    *(stepAddress++) = ITM_SUB;

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 90;

    *(stepAddress++) = ITM_RTN;

    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 11;

    *(stepAddress++) = (ITM_CLSTK >> 8) | 0x80;
    *(stepAddress++) =  ITM_CLSTK       & 0xff;

    // 450
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 95;

    *(stepAddress++) = ITM_CHS;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    *(stepAddress++) = ITM_DIV;

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 91;

    *(stepAddress++) = ITM_SQUARE;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 96;

    *(stepAddress++) = ITM_SUB;

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 92;

    *(stepAddress++) = (ITM_CLSTK >> 8) | 0x80;
    *(stepAddress++) =  ITM_CLSTK       & 0xff;

    // 460
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 91;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 92;

    *(stepAddress++) = ITM_SQUAREROOTX;

    *(stepAddress++) = ITM_ADD;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 91;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 92;

    *(stepAddress++) = ITM_SQUAREROOTX;

    *(stepAddress++) = ITM_SUB;

    *(stepAddress++) = ITM_RTN;

    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 12;

    // 470
    *(stepAddress++) = (ITM_CLSTK >> 8) | 0x80;
    *(stepAddress++) =  ITM_CLSTK       & 0xff;

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

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 1;

    *(stepAddress++) = ITM_CHS;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++)= STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '2'; // THE string

    *(stepAddress++) = ITM_DIV;

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 91;

    *(stepAddress++) = ITM_SQUARE;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 2;

    *(stepAddress++) = ITM_SUB;

    // 480
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 92;

    *(stepAddress++) = (ITM_CLSTK >> 8) | 0x80;
    *(stepAddress++) =  ITM_CLSTK       & 0xff;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 91;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 92;

    *(stepAddress++) = ITM_SQUAREROOTX;

    *(stepAddress++) = ITM_ADD;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 91;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 92;

    *(stepAddress++) = ITM_SQUAREROOTX;

    *(stepAddress++) = ITM_SUB;

    // 490
    *(stepAddress++) = ITM_RTN;

    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 13;

    *(stepAddress++) = (ITM_CLSTK >> 8) | 0x80;
    *(stepAddress++) =  ITM_CLSTK       & 0xff;

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

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 1;

    *(stepAddress++) = ITM_CHS;

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 91;

    *(stepAddress++) = (ITM_CLSTK >> 8) | 0x80;
    *(stepAddress++) =  ITM_CLSTK       & 0xff;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 91;

    *(stepAddress++) = ITM_RTN;

    // 500
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 4;

    *(stepAddress++) = (ITM_CLSTK >> 8) | 0x80;
    *(stepAddress++) =  ITM_CLSTK       & 0xff;

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

    *(stepAddress++) = ITM_RTN;

    // 504
    *(stepAddress++) = (ITM_END >> 8) | 0x80;
    *(stepAddress++) =  ITM_END       & 0xff;

    #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
      printf("Prime + Len1 + Len2 + Len3 + Len4 + Len5 + Len6 + Len7 + Len8 + Bairstow : %u bytes\n", (uint32_t)(stepAddress - programMemoryPointer));
    #endif
  }

  { // Speed test. See: https://forum.swissmicros.com/viewtopic.php?p=17308
    // 505
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 5; // String length
    *(stepAddress++) = 'S';
    *(stepAddress++) = 'p';
    *(stepAddress++) = 'e';
    *(stepAddress++) = 'e';
    *(stepAddress++) = 'd';

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 10;

    *(stepAddress++) = (ITM_TICKS >> 8) | 0x80;
    *(stepAddress++) =  ITM_TICKS       & 0xff;

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '0'; // THE string

    // 510
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 11;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 12;

    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 1;

    *(stepAddress++) = ITM_arctan;

    *(stepAddress++) = ITM_sin;

    *(stepAddress++) = ITM_EXP;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '3'; // THE string

    *(stepAddress++) = ITM_1ONX;

    *(stepAddress++) = ITM_YX;

    // 520
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 11;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 12;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    *(stepAddress++) = ITM_ADD;

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 12;

    *(stepAddress++) = ITM_DSE;
    *(stepAddress++) = 10;

    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 1;

    *(stepAddress++) = (ITM_TICKS >> 8) | 0x80;
    *(stepAddress++) =  ITM_TICKS       & 0xff;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_SUB;

    // 530
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 2; // String length
    *(stepAddress++) = '1'; // THE string
    *(stepAddress++) = '0';

    *(stepAddress++) = ITM_DIV;

    // 532
    *(stepAddress++) = (ITM_END >> 8) | 0x80;
    *(stepAddress++) =  ITM_END       & 0xff;

    #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
      printf("Prime + Len1 + Len2 + Len3 + Len4 + Len5 + Len6 + Len7 + Len8 + Bairstow + Speed : %u bytes\n", (uint32_t)(stepAddress - programMemoryPointer));
    #endif
  }
*/
  { // Factorial: the recursive way
    // 533
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 4; // String length
    *(stepAddress++) = 'F';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'c';
    *(stepAddress++) = 't';

    *(stepAddress++) = ITM_IP;

    *(stepAddress++) = ITM_XGT;
    *(stepAddress++) = VALUE_1;

    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_REAL34;
    *(stepAddress++) = 1; // String length
    *(stepAddress++) = '1'; // THE string

    *(stepAddress++) = ITM_RTN;

    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 0;

    // 540
    *(stepAddress++) = (ITM_LocR >> 8) | 0x80;
    *(stepAddress++) =  ITM_LocR       & 0xff;
    *(stepAddress++) = 1;

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 112; // .00

    *(stepAddress++) = ITM_DEC;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 4; // String length
    *(stepAddress++) = 'F';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'c';
    *(stepAddress++) = 't';

    *(stepAddress++) = ITM_RCLMULT;
    *(stepAddress++) = 112; // .00

    *(stepAddress++) = ITM_RTN;

    // 546
    *(stepAddress++) = (ITM_END >> 8) | 0x80;
    *(stepAddress++) =  ITM_END       & 0xff;

    #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
      printf("Prime + Len1 + Len2 + Len3 + Len4 + Len5 + Len6 + Len7 + Len8 + Bairstow + Speed + Fact : %u bytes\n", (uint32_t)(stepAddress - programMemoryPointer));
    #endif
  }
/*
  { // OM page 203
    // 547
    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 6; // String length
    *(stepAddress++) = 'O';
    *(stepAddress++) = 'M';
    *(stepAddress++) = 'p';
    *(stepAddress++) = '2';
    *(stepAddress++) = '0';
    *(stepAddress++) = '3';

    *(stepAddress++) = ITM_SQUARE;

    *(stepAddress++) = ITM_pi;

    // 550
    *(stepAddress++) = ITM_MULT;

    *(stepAddress++) = ITM_RTN;
    // 552
    *(stepAddress++) = (ITM_END >> 8) | 0x80;
    *(stepAddress++) =  ITM_END       & 0xff;

    #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
      printf("Prime + Len1 + Len2 + Len3 + Len4 + Len5 + Len6 + Len7 + Len8 + Bairstow + Speed + Fact : %u bytes\n", (uint32_t)(stepAddress - programMemoryPointer));
    #endif
  }

  { // All OP's
    //currentProgramMemoryPointer = stepAddress;
    // 553
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

    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 'A' - 'A' + 100; // A

    *(stepAddress++) = ITM_LBL;
    *(stepAddress++) = 'J' - 'A' + 100; // J

    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 99;

    // 560
    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 'A' - 'A' + 100; // A

    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = 'J' - 'A' + 100; // J

    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 5;
    *(stepAddress++) = 'L';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'b';
    *(stepAddress++) = 'e';
    *(stepAddress++) = 'l';

    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_GTO;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 570
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 'A' - 'A' + 100; // A

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = 'J' - 'A' + 100; // J

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 5;
    *(stepAddress++) = 'L';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'b';
    *(stepAddress++) = 'e';
    *(stepAddress++) = 'l';

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 580
    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_XEQ;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_RTN;

    *(stepAddress++) = ITM_ISE;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_ISE;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_ISE;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_ISE;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_ISE;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_ISE;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_ISE;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 590
    *(stepAddress++) = ITM_ISE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_ISE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_ISE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_ISE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_ISE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_ISE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 590
    *(stepAddress++) = ITM_ISE;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_ISG;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_ISG;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_ISG;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_ISG;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_ISG;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_ISG;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_ISG;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_ISG;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_ISG;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    // 600
    *(stepAddress++) = ITM_ISG;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_ISG;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_ISG;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_ISG;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_ISG;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_ISZ;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_ISZ;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_ISZ;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_ISZ;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_ISZ;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 610
    *(stepAddress++) = ITM_ISZ;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_ISZ;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_ISZ;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_ISZ;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_ISZ;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_ISZ;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_ISZ;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_ISZ;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_ISZ;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_DSE;
    *(stepAddress++) = 0;

    // 620
    *(stepAddress++) = ITM_DSE;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_DSE;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_DSE;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_DSE;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_DSE;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_DSE;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_DSE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_DSE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_DSE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_DSE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    // 630
    *(stepAddress++) = ITM_DSE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_DSE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_DSE;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_DSL;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_DSL;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_DSL;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_DSL;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_DSL;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_DSL;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_DSL;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 640
    *(stepAddress++) = ITM_DSL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_DSL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_DSL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_DSL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_DSL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_DSL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_DSL;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_DSZ;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_DSZ;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_DSZ;
    *(stepAddress++) = REGISTER_X;

    // 650
    *(stepAddress++) = ITM_DSZ;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_DSZ;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_DSZ;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_DSZ;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_DSZ;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_DSZ;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_DSZ;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_DSZ;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_DSZ;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_DSZ;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 660
    *(stepAddress++) = ITM_DSZ;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_XEQU;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_XEQU;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_XEQU;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_XEQU;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_XEQU;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_XEQU;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_XEQU;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_XEQU;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_XEQU;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    // 670
    *(stepAddress++) = ITM_XEQU;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_XEQU;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_XEQU;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_XEQU;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_XEQU;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_XNE;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_XNE;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_XNE;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_XNE;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_XNE;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 680
    *(stepAddress++) = ITM_XNE;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_XNE;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_XNE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_XNE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_XNE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_XNE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_XNE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_XNE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_XNE;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_XEQUP0;

    // 690
    *(stepAddress++) = ITM_XEQUM0;

    *(stepAddress++) = ITM_XAEQU;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_XAEQU;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_XAEQU;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_XAEQU;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_XAEQU;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_XAEQU;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_XAEQU;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_XAEQU;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_XAEQU;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    // 700
    *(stepAddress++) = ITM_XAEQU;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_XAEQU;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_XAEQU;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_XAEQU;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_XAEQU;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_XLT;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_XLT;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_XLT;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_XLT;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_XLT;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 710
    *(stepAddress++) = ITM_XLT;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_XLT;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_XLT;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_XLT;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_XLT;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_XLT;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_XLT;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_XLT;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_XLT;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_XLE;
    *(stepAddress++) = 0;

    // 720
    *(stepAddress++) = ITM_XLE;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_XLE;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_XLE;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_XLE;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_XLE;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_XLE;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_XLE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_XLE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_XLE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_XLE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    // 730
    *(stepAddress++) = ITM_XLE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_XLE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_XLE;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_XGE;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_XGE;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_XGE;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_XGE;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_XGE;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_XGE;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_XGE;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 740
    *(stepAddress++) = ITM_XGE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_XGE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_XGE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_XGE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_XGE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_XGE;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_XGE;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_XGT;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_XGT;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_XGT;
    *(stepAddress++) = REGISTER_X;

    // 750
    *(stepAddress++) = ITM_XGT;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_XGT;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_XGT;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_XGT;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_XGT;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_XGT;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_XGT;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_XGT;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_XGT;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_XGT;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 760
    *(stepAddress++) = ITM_XGT;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_FC;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_FC;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_FC;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_FC;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_FC;
    *(stepAddress++) = FIRST_LOCAL_FLAG;

    *(stepAddress++) = ITM_FC;
    *(stepAddress++) = LAST_LOCAL_FLAG;

    *(stepAddress++) = ITM_FC;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_FC;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_FC;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    // 770
    *(stepAddress++) = ITM_FC;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_FC;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_FLAG;

    *(stepAddress++) = ITM_FC;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_FLAG;

    *(stepAddress++) = ITM_FC;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_FS;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_FS;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_FS;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_FS;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_FS;
    *(stepAddress++) = FIRST_LOCAL_FLAG;

    *(stepAddress++) = ITM_FS;
    *(stepAddress++) = LAST_LOCAL_FLAG;

    // 780
    *(stepAddress++) = ITM_FS;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_FS;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_FS;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_FS;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_FS;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_FLAG;

    *(stepAddress++) = ITM_FS;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_FLAG;

    *(stepAddress++) = ITM_FS;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_EVEN;

    *(stepAddress++) = ITM_ODD;

    *(stepAddress++) = ITM_FPQ;

    // 790
    *(stepAddress++) = ITM_INT;

    *(stepAddress++) = ITM_CPX;

    *(stepAddress++) = ITM_MATR;

    *(stepAddress++) = ITM_NAN;

    *(stepAddress++) = ITM_REAL;

    *(stepAddress++) = ITM_SPEC;

    *(stepAddress++) = ITM_STRI;

    *(stepAddress++) = ITM_PMINFINITY;

    *(stepAddress++) = ITM_PRIME;

    *(stepAddress++) = ITM_TOP;

    // 800
    *(stepAddress++) = ITM_ENTER;

    *(stepAddress++) = ITM_XexY;

    *(stepAddress++) = ITM_DROP;

    *(stepAddress++) = ITM_DROPY;

    *(stepAddress++) = ITM_Rup;

    *(stepAddress++) = ITM_Rdown;

    *(stepAddress++) = ITM_CLX;

    *(stepAddress++) = ITM_FILL;

    *(stepAddress++) = ITM_INPUT;

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 0;

    // 810
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    // 820
    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_STO;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 830
    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_STOADD;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_STOSUB;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_STOSUB;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_STOSUB;
    *(stepAddress++) = REGISTER_X;

    // 840
    *(stepAddress++) = ITM_STOSUB;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_STOSUB;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_STOSUB;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_STOSUB;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_STOSUB;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_STOSUB;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_STOSUB;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_STOSUB;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_STOSUB;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_STOSUB;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 850
    *(stepAddress++) = ITM_STOSUB;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_STOMULT;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_STOMULT;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_STOMULT;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_STOMULT;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_STOMULT;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_STOMULT;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_STOMULT;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_STOMULT;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_STOMULT;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    // 860
    *(stepAddress++) = ITM_STOMULT;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_STOMULT;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_STOMULT;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_STOMULT;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_STOMULT;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_STODIV;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_STODIV;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_STODIV;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_STODIV;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_STODIV;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 870
    *(stepAddress++) = ITM_STODIV;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_STODIV;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_STODIV;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_STODIV;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_STODIV;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_STODIV;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_STODIV;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_STODIV;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_STODIV;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_STOMAX;
    *(stepAddress++) = 0;

    // 880
    *(stepAddress++) = ITM_STOMAX;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_STOMAX;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_STOMAX;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_STOMAX;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_STOMAX;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_STOMAX;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_STOMAX;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_STOMAX;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_STOMAX;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_STOMAX;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    // 890
    *(stepAddress++) = ITM_STOMAX;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_STOMAX;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_STOMAX;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_STOMIN;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_STOMIN;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_STOMIN;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_STOMIN;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_STOMIN;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_STOMIN;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_STOMIN;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 900
    *(stepAddress++) = ITM_STOMIN;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_STOMIN;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_STOMIN;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_STOMIN;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_STOMIN;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_STOMIN;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_STOMIN;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = REGISTER_X;

    // 910
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 920
    *(stepAddress++) = ITM_RCL;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_RCLADD;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_RCLADD;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_RCLADD;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_RCLADD;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_RCLADD;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_RCLADD;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_RCLADD;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_RCLADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_RCLADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    // 930
    *(stepAddress++) = ITM_RCLADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_RCLADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_RCLADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_RCLADD;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_RCLADD;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_RCLSUB;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_RCLSUB;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_RCLSUB;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_RCLSUB;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_RCLSUB;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 940
    *(stepAddress++) = ITM_RCLSUB;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_RCLSUB;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_RCLSUB;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_RCLSUB;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_RCLSUB;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_RCLSUB;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_RCLSUB;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_RCLSUB;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_RCLSUB;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_RCLMULT;
    *(stepAddress++) = 0;

    // 950
    *(stepAddress++) = ITM_RCLMULT;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_RCLMULT;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_RCLMULT;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_RCLMULT;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_RCLMULT;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_RCLMULT;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_RCLMULT;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_RCLMULT;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_RCLMULT;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_RCLMULT;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    // 960
    *(stepAddress++) = ITM_RCLMULT;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_RCLMULT;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_RCLMULT;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_RCLDIV;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_RCLDIV;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_RCLDIV;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_RCLDIV;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_RCLDIV;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_RCLDIV;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_RCLDIV;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 970
    *(stepAddress++) = ITM_RCLDIV;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_RCLDIV;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_RCLDIV;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_RCLDIV;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_RCLDIV;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_RCLDIV;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_RCLDIV;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_RCLMAX;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_RCLMAX;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_RCLMAX;
    *(stepAddress++) = REGISTER_X;

    // 980
    *(stepAddress++) = ITM_RCLMAX;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_RCLMAX;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_RCLMAX;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_RCLMAX;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_RCLMAX;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_RCLMAX;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_RCLMAX;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_RCLMAX;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_RCLMAX;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_RCLMAX;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    // 990
    *(stepAddress++) = ITM_RCLMAX;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_RCLMIN;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_RCLMIN;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_RCLMIN;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_RCLMIN;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_RCLMIN;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_RCLMIN;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_RCLMIN;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_RCLMIN;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_RCLMIN;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    // 1000
    *(stepAddress++) = ITM_RCLMIN;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_RCLMIN;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_RCLMIN;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_RCLMIN;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_RCLMIN;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_SQUARE;

    *(stepAddress++) = ITM_CUBE;

    *(stepAddress++) = ITM_YX;

    *(stepAddress++) = ITM_SQUAREROOTX;

    *(stepAddress++) = ITM_CUBEROOT;

    // 1010
    *(stepAddress++) = ITM_XTHROOT;

    *(stepAddress++) = ITM_2X;

    *(stepAddress++) = ITM_EXP;

    *(stepAddress++) = ITM_EX1;

    *(stepAddress++) = ITM_10x;

    *(stepAddress++) = ITM_LOG2;

    *(stepAddress++) = ITM_LN;

    *(stepAddress++) = ITM_LN1X;

    *(stepAddress++) = ITM_LOG10;

    *(stepAddress++) = ITM_LOGXY;

    // 1020
    *(stepAddress++) = ITM_1ONX;

    *(stepAddress++) = ITM_cos;

    *(stepAddress++) = ITM_cosh;

    *(stepAddress++) = ITM_sin;

    *(stepAddress++) = ITM_sinc;

    *(stepAddress++) = ITM_sinh;

    *(stepAddress++) = ITM_tan;

    *(stepAddress++) = ITM_tanh;

    *(stepAddress++) = ITM_arccos;

    *(stepAddress++) = ITM_arcosh;

    // 1030
    *(stepAddress++) = ITM_arcsin;

    *(stepAddress++) = ITM_arsinh;

    *(stepAddress++) = ITM_arctan;

    *(stepAddress++) = ITM_artanh;

    *(stepAddress++) = ITM_CEIL;

    *(stepAddress++) = ITM_FLOOR;

    *(stepAddress++) = ITM_GCD;

    *(stepAddress++) = ITM_LCM;

    *(stepAddress++) = ITM_DEC;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_DEC;
    *(stepAddress++) = 99;

    // 1040
    *(stepAddress++) = ITM_DEC;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_DEC;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_DEC;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_DEC;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_DEC;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_DEC;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_DEC;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_DEC;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_DEC;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_DEC;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    // 1050
    *(stepAddress++) = ITM_DEC;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_DEC;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_INC;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_INC;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_INC;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_INC;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_INC;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_INC;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_INC;
    *(stepAddress++) = STRING_LABEL_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_INC;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    // 1060
    *(stepAddress++) = ITM_INC;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_INC;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_INC;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_INC;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_INC;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = ITM_INC;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_IP;

    *(stepAddress++) = ITM_FP;

    *(stepAddress++) = ITM_ADD;

    *(stepAddress++) = ITM_SUB;

    // 1070
    *(stepAddress++) = ITM_CHS;

    *(stepAddress++) = ITM_MULT;

    *(stepAddress++) = ITM_DIV;

    *(stepAddress++) = ITM_IDIV;

    *(stepAddress++) = ITM_IDIVR;

    *(stepAddress++) = ITM_MOD;

    *(stepAddress++) = ITM_MAX;

    *(stepAddress++) = ITM_MIN;

    *(stepAddress++) = ITM_MAGNITUDE;

    *(stepAddress++) = ITM_NEIGHB;

    // 1080
    *(stepAddress++) = ITM_NEXTP;

    *(stepAddress++) = ITM_XFACT;

    *(stepAddress++) = ITM_pi;

    *(stepAddress++) = ITM_CF;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_CF;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_CF;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_CF;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_CF;
    *(stepAddress++) = FIRST_LOCAL_FLAG;

    *(stepAddress++) = ITM_CF;
    *(stepAddress++) = LAST_LOCAL_FLAG;

    *(stepAddress++) = ITM_CF;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    // 1090
    *(stepAddress++) = ITM_CF;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_CF;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_CF;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_CF;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_FLAG;

    *(stepAddress++) = ITM_CF;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_FLAG;

    *(stepAddress++) = ITM_CF;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_SF;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_SF;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_SF;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_SF;
    *(stepAddress++) = REGISTER_K;

    // 1100
    *(stepAddress++) = ITM_SF;
    *(stepAddress++) = FIRST_LOCAL_FLAG;

    *(stepAddress++) = ITM_SF;
    *(stepAddress++) = LAST_LOCAL_FLAG;

    *(stepAddress++) = ITM_SF;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_SF;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_SF;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_SF;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_SF;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_FLAG;

    *(stepAddress++) = ITM_SF;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_FLAG;

    *(stepAddress++) = ITM_SF;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_FF;
    *(stepAddress++) = 0;

    // 1110
    *(stepAddress++) = ITM_FF;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_FF;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_FF;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_FF;
    *(stepAddress++) = FIRST_LOCAL_FLAG;

    *(stepAddress++) = ITM_FF;
    *(stepAddress++) = LAST_LOCAL_FLAG;

    *(stepAddress++) = ITM_FF;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    *(stepAddress++) = ITM_FF;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    *(stepAddress++) = ITM_FF;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = ITM_FF;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = ITM_FF;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_FLAG;

    // 1120
    *(stepAddress++) = ITM_FF;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_FLAG;

    *(stepAddress++) = ITM_FF;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    *(stepAddress++) = ITM_sincpi;

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

    *(stepAddress++) = (ITM_NOP >> 8) | 0x80; // TODO: implement ITM_LITT BINARY_LONG_INTEGER
    *(stepAddress++) =  ITM_NOP       & 0xff;

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

    // 1130
    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_ANGLE34;
    *(stepAddress++) = AM_DEGREE;
    *(stepAddress++) = 2; // String length
    *(stepAddress++) = '4';
    *(stepAddress++) = '5';

    *(stepAddress++) = ITM_LITT;
    *(stepAddress++) = STRING_COMPLEX34;
    *(stepAddress++) = 4; // String length
    *(stepAddress++) = '1';
    *(stepAddress++) = '-';
    *(stepAddress++) = 'i';
    *(stepAddress++) = '2';

    *(stepAddress++) = (CST_01 >> 8) | 0x80;
    *(stepAddress++) =  CST_01       & 0xff;

    *(stepAddress++) = (CST_02 >> 8) | 0x80;
    *(stepAddress++) =  CST_02       & 0xff;

    *(stepAddress++) = (CST_03 >> 8) | 0x80;
    *(stepAddress++) =  CST_03       & 0xff;

    *(stepAddress++) = (CST_04 >> 8) | 0x80;
    *(stepAddress++) =  CST_04       & 0xff;

    *(stepAddress++) = (CST_05 >> 8) | 0x80;
    *(stepAddress++) =  CST_05       & 0xff;

    *(stepAddress++) = (CST_06 >> 8) | 0x80;
    *(stepAddress++) =  CST_06       & 0xff;

    *(stepAddress++) = (CST_07 >> 8) | 0x80;
    *(stepAddress++) =  CST_07       & 0xff;

    *(stepAddress++) = (CST_08 >> 8) | 0x80;
    *(stepAddress++) =  CST_08       & 0xff;

    // 1140
    *(stepAddress++) = (CST_09 >> 8) | 0x80;
    *(stepAddress++) =  CST_09       & 0xff;

    *(stepAddress++) = (CST_10 >> 8) | 0x80;
    *(stepAddress++) =  CST_10       & 0xff;

    *(stepAddress++) = (CST_11 >> 8) | 0x80;
    *(stepAddress++) =  CST_11       & 0xff;

    *(stepAddress++) = (CST_12 >> 8) | 0x80;
    *(stepAddress++) =  CST_12       & 0xff;

    *(stepAddress++) = (CST_13 >> 8) | 0x80;
    *(stepAddress++) =  CST_13       & 0xff;

    *(stepAddress++) = (CST_14 >> 8) | 0x80;
    *(stepAddress++) =  CST_14       & 0xff;

    *(stepAddress++) = (CST_15 >> 8) | 0x80;
    *(stepAddress++) =  CST_15       & 0xff;

    *(stepAddress++) = (CST_16 >> 8) | 0x80;
    *(stepAddress++) =  CST_16       & 0xff;

    *(stepAddress++) = (CST_17 >> 8) | 0x80;
    *(stepAddress++) =  CST_17       & 0xff;

    *(stepAddress++) = (CST_18 >> 8) | 0x80;
    *(stepAddress++) =  CST_18       & 0xff;

    // 1150
    *(stepAddress++) = (CST_19 >> 8) | 0x80;
    *(stepAddress++) =  CST_19       & 0xff;

    *(stepAddress++) = (CST_20 >> 8) | 0x80;
    *(stepAddress++) =  CST_20       & 0xff;

    *(stepAddress++) = (CST_21 >> 8) | 0x80;
    *(stepAddress++) =  CST_21       & 0xff;

    *(stepAddress++) = (CST_22 >> 8) | 0x80;
    *(stepAddress++) =  CST_22       & 0xff;

    *(stepAddress++) = (CST_23 >> 8) | 0x80;
    *(stepAddress++) =  CST_23       & 0xff;

    *(stepAddress++) = (CST_24 >> 8) | 0x80;
    *(stepAddress++) =  CST_24       & 0xff;

    *(stepAddress++) = (CST_25 >> 8) | 0x80;
    *(stepAddress++) =  CST_25       & 0xff;

    *(stepAddress++) = (CST_26 >> 8) | 0x80;
    *(stepAddress++) =  CST_26       & 0xff;

    *(stepAddress++) = (CST_27 >> 8) | 0x80;
    *(stepAddress++) =  CST_27       & 0xff;

    *(stepAddress++) = (CST_28 >> 8) | 0x80;
    *(stepAddress++) =  CST_28       & 0xff;

    // 1160
    *(stepAddress++) = (CST_29 >> 8) | 0x80;
    *(stepAddress++) =  CST_29       & 0xff;

    *(stepAddress++) = (CST_30 >> 8) | 0x80;
    *(stepAddress++) =  CST_30       & 0xff;

    *(stepAddress++) = (CST_31 >> 8) | 0x80;
    *(stepAddress++) =  CST_31       & 0xff;

    *(stepAddress++) = (CST_32 >> 8) | 0x80;
    *(stepAddress++) =  CST_32       & 0xff;

    *(stepAddress++) = (CST_33 >> 8) | 0x80;
    *(stepAddress++) =  CST_33       & 0xff;

    *(stepAddress++) = (CST_34 >> 8) | 0x80;
    *(stepAddress++) =  CST_34       & 0xff;

    *(stepAddress++) = (CST_35 >> 8) | 0x80;
    *(stepAddress++) =  CST_35       & 0xff;

    *(stepAddress++) = (CST_36 >> 8) | 0x80;
    *(stepAddress++) =  CST_36       & 0xff;

    *(stepAddress++) = (CST_37 >> 8) | 0x80;
    *(stepAddress++) =  CST_37       & 0xff;

    *(stepAddress++) = (CST_38 >> 8) | 0x80;
    *(stepAddress++) =  CST_38       & 0xff;

    // 1170
    *(stepAddress++) = (CST_39 >> 8) | 0x80;
    *(stepAddress++) =  CST_39       & 0xff;

    *(stepAddress++) = (CST_40 >> 8) | 0x80;
    *(stepAddress++) =  CST_40       & 0xff;

    *(stepAddress++) = (CST_41 >> 8) | 0x80;
    *(stepAddress++) =  CST_41       & 0xff;

    *(stepAddress++) = (CST_42 >> 8) | 0x80;
    *(stepAddress++) =  CST_42       & 0xff;

    *(stepAddress++) = (CST_43 >> 8) | 0x80;
    *(stepAddress++) =  CST_43       & 0xff;

    *(stepAddress++) = (CST_44 >> 8) | 0x80;
    *(stepAddress++) =  CST_44       & 0xff;

    *(stepAddress++) = (CST_45 >> 8) | 0x80;
    *(stepAddress++) =  CST_45       & 0xff;

    *(stepAddress++) = (CST_46 >> 8) | 0x80;
    *(stepAddress++) =  CST_46       & 0xff;

    *(stepAddress++) = (CST_47 >> 8) | 0x80;
    *(stepAddress++) =  CST_47       & 0xff;

    *(stepAddress++) = (CST_48 >> 8) | 0x80;
    *(stepAddress++) =  CST_48       & 0xff;

    // 1180
    *(stepAddress++) = (CST_49 >> 8) | 0x80;
    *(stepAddress++) =  CST_49       & 0xff;

    *(stepAddress++) = (CST_50 >> 8) | 0x80;
    *(stepAddress++) =  CST_50       & 0xff;

    *(stepAddress++) = (CST_51 >> 8) | 0x80;
    *(stepAddress++) =  CST_51       & 0xff;

    *(stepAddress++) = (CST_52 >> 8) | 0x80;
    *(stepAddress++) =  CST_52       & 0xff;

    *(stepAddress++) = (CST_53 >> 8) | 0x80;
    *(stepAddress++) =  CST_53       & 0xff;

    *(stepAddress++) = (CST_54 >> 8) | 0x80;
    *(stepAddress++) =  CST_54       & 0xff;

    *(stepAddress++) = (CST_55 >> 8) | 0x80;
    *(stepAddress++) =  CST_55       & 0xff;

    *(stepAddress++) = (CST_56 >> 8) | 0x80;
    *(stepAddress++) =  CST_56       & 0xff;

    *(stepAddress++) = (CST_57 >> 8) | 0x80;
    *(stepAddress++) =  CST_57       & 0xff;

    *(stepAddress++) = (CST_58 >> 8) | 0x80;
    *(stepAddress++) =  CST_58       & 0xff;

    // 1190
    *(stepAddress++) = (CST_59 >> 8) | 0x80;
    *(stepAddress++) =  CST_59       & 0xff;

    *(stepAddress++) = (CST_60 >> 8) | 0x80;
    *(stepAddress++) =  CST_60       & 0xff;

    *(stepAddress++) = (CST_61 >> 8) | 0x80;
    *(stepAddress++) =  CST_61       & 0xff;

    *(stepAddress++) = (CST_62 >> 8) | 0x80;
    *(stepAddress++) =  CST_62       & 0xff;

    *(stepAddress++) = (CST_63 >> 8) | 0x80;
    *(stepAddress++) =  CST_63       & 0xff;

    *(stepAddress++) = (CST_64 >> 8) | 0x80;
    *(stepAddress++) =  CST_64       & 0xff;

    *(stepAddress++) = (CST_65 >> 8) | 0x80;
    *(stepAddress++) =  CST_65       & 0xff;

    *(stepAddress++) = (CST_66 >> 8) | 0x80;
    *(stepAddress++) =  CST_66       & 0xff;

    *(stepAddress++) = (CST_67 >> 8) | 0x80;
    *(stepAddress++) =  CST_67       & 0xff;

    *(stepAddress++) = (CST_68 >> 8) | 0x80;
    *(stepAddress++) =  CST_68       & 0xff;

    // 1200
    *(stepAddress++) = (CST_69 >> 8) | 0x80;
    *(stepAddress++) =  CST_69       & 0xff;

    *(stepAddress++) = (CST_70 >> 8) | 0x80;
    *(stepAddress++) =  CST_70       & 0xff;

    *(stepAddress++) = (CST_71 >> 8) | 0x80;
    *(stepAddress++) =  CST_71       & 0xff;

    *(stepAddress++) = (CST_72 >> 8) | 0x80;
    *(stepAddress++) =  CST_72       & 0xff;

    *(stepAddress++) = (CST_73 >> 8) | 0x80;
    *(stepAddress++) =  CST_73       & 0xff;

    *(stepAddress++) = (CST_74 >> 8) | 0x80;
    *(stepAddress++) =  CST_74       & 0xff;

    *(stepAddress++) = (CST_75 >> 8) | 0x80;
    *(stepAddress++) =  CST_75       & 0xff;

    *(stepAddress++) = (CST_76 >> 8) | 0x80;
    *(stepAddress++) =  CST_76       & 0xff;

    *(stepAddress++) = (CST_77 >> 8) | 0x80;
    *(stepAddress++) =  CST_77       & 0xff;

    *(stepAddress++) = (CST_78 >> 8) | 0x80;
    *(stepAddress++) =  CST_78       & 0xff;

    // 1210
    *(stepAddress++) = (CST_79 >> 8) | 0x80;
    *(stepAddress++) =  CST_79       & 0xff;

    *(stepAddress++) = (ITM_CNST >> 8) | 0x80;
    *(stepAddress++) =  ITM_CNST       & 0xff;
    *(stepAddress++) =  1;

    *(stepAddress++) = (ITM_CNST >> 8) | 0x80;
    *(stepAddress++) =  ITM_CNST       & 0xff;
    *(stepAddress++) =  79;

    *(stepAddress++) = (ITM_CNST >> 8) | 0x80;
    *(stepAddress++) =  ITM_CNST       & 0xff;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 0;

    *(stepAddress++) = (ITM_CNST >> 8) | 0x80;
    *(stepAddress++) =  ITM_CNST       & 0xff;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = 99;

    *(stepAddress++) = (ITM_CNST >> 8) | 0x80;
    *(stepAddress++) =  ITM_CNST       & 0xff;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_X;

    *(stepAddress++) = (ITM_CNST >> 8) | 0x80;
    *(stepAddress++) =  ITM_CNST       & 0xff;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = REGISTER_K;

    *(stepAddress++) = (ITM_CNST >> 8) | 0x80;
    *(stepAddress++) =  ITM_CNST       & 0xff;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = FIRST_LOCAL_REGISTER;

    *(stepAddress++) = (ITM_CNST >> 8) | 0x80;
    *(stepAddress++) =  ITM_CNST       & 0xff;
    *(stepAddress++) = INDIRECT_REGISTER;
    *(stepAddress++) = LAST_LOCAL_REGISTER;

    *(stepAddress++) = (ITM_CNST >> 8) | 0x80;
    *(stepAddress++) =  ITM_CNST       & 0xff;
    *(stepAddress++) = INDIRECT_VARIABLE;
    *(stepAddress++) = 3;
    *(stepAddress++) = 'V';
    *(stepAddress++) = 'a';
    *(stepAddress++) = 'r';

    // 1220
    *(stepAddress++) = (ITM_CtoF >> 8) | 0x80;
    *(stepAddress++) =  ITM_CtoF       & 0xff;

    *(stepAddress++) = (ITM_FtoC >> 8) | 0x80;
    *(stepAddress++) =  ITM_FtoC       & 0xff;

    *(stepAddress++) = (ITM_DBtoPR >> 8) | 0x80;
    *(stepAddress++) =  ITM_DBtoPR       & 0xff;

    *(stepAddress++) = (ITM_DBtoPRb >> 8) | 0x80;
    *(stepAddress++) =  ITM_DBtoPRb       & 0xff;

    *(stepAddress++) = (ITM_DBtoPRc >> 8) | 0x80;
    *(stepAddress++) =  ITM_DBtoPRc       & 0xff;

    *(stepAddress++) = (ITM_DBtoFR >> 8) | 0x80;
    *(stepAddress++) =  ITM_DBtoFR       & 0xff;

    *(stepAddress++) = (ITM_DBtoFRb >> 8) | 0x80;
    *(stepAddress++) =  ITM_DBtoFRb       & 0xff;

    *(stepAddress++) = (ITM_DBtoFRc >> 8) | 0x80;
    *(stepAddress++) =  ITM_DBtoFRc       & 0xff;

    *(stepAddress++) = (ITM_PRtoDB >> 8) | 0x80;
    *(stepAddress++) =  ITM_PRtoDB       & 0xff;

    *(stepAddress++) = (ITM_PRtoDBb >> 8) | 0x80;
    *(stepAddress++) =  ITM_PRtoDBb       & 0xff;

    // 1230
    *(stepAddress++) = (ITM_PRtoDBc >> 8) | 0x80;
    *(stepAddress++) =  ITM_PRtoDBc       & 0xff;

    *(stepAddress++) = (ITM_FRtoDB >> 8) | 0x80;
    *(stepAddress++) =  ITM_FRtoDB       & 0xff;

    *(stepAddress++) = (ITM_FRtoDBb >> 8) | 0x80;
    *(stepAddress++) =  ITM_FRtoDBb       & 0xff;

    *(stepAddress++) = (ITM_FRtoDBc >> 8) | 0x80;
    *(stepAddress++) =  ITM_FRtoDBc       & 0xff;

    *(stepAddress++) = (ITM_ACtoM2 >> 8) | 0x80;
    *(stepAddress++) =  ITM_ACtoM2       & 0xff;

    *(stepAddress++) = (ITM_ACtoM2b >> 8) | 0x80;
    *(stepAddress++) =  ITM_ACtoM2b       & 0xff;

    *(stepAddress++) = (ITM_M2toAC >> 8) | 0x80;
    *(stepAddress++) =  ITM_M2toAC       & 0xff;

    *(stepAddress++) = (ITM_M2toACb >> 8) | 0x80;
    *(stepAddress++) =  ITM_M2toACb       & 0xff;

    *(stepAddress++) = (ITM_ACUStoM2 >> 8) | 0x80;
    *(stepAddress++) =  ITM_ACUStoM2       & 0xff;

    *(stepAddress++) = (ITM_ACUStoM2b >> 8) | 0x80;
    *(stepAddress++) =  ITM_ACUStoM2b       & 0xff;

    // 1240
    *(stepAddress++) = (ITM_M2toACUS >> 8) | 0x80;
    *(stepAddress++) =  ITM_M2toACUS       & 0xff;

    *(stepAddress++) = (ITM_M2toACUSb >> 8) | 0x80;
    *(stepAddress++) =  ITM_M2toACUSb       & 0xff;

    *(stepAddress++) = (ITM_PAtoATM >> 8) | 0x80;
    *(stepAddress++) =  ITM_PAtoATM       & 0xff;

    *(stepAddress++) = (ITM_ATMtoPA >> 8) | 0x80;
    *(stepAddress++) =  ITM_ATMtoPA       & 0xff;

    *(stepAddress++) = (ITM_AUtoM >> 8) | 0x80;
    *(stepAddress++) =  ITM_AUtoM       & 0xff;

    *(stepAddress++) = (ITM_MtoAU >> 8) | 0x80;
    *(stepAddress++) =  ITM_MtoAU       & 0xff;

    *(stepAddress++) = (ITM_BARtoPA >> 8) | 0x80;
    *(stepAddress++) =  ITM_BARtoPA       & 0xff;

    *(stepAddress++) = (ITM_PAtoBAR >> 8) | 0x80;
    *(stepAddress++) =  ITM_PAtoBAR       & 0xff;

    *(stepAddress++) = (ITM_BTUtoJ >> 8) | 0x80;
    *(stepAddress++) =  ITM_BTUtoJ       & 0xff;

    *(stepAddress++) = (ITM_JtoBTU >> 8) | 0x80;
    *(stepAddress++) =  ITM_JtoBTU       & 0xff;

    // 1250
    *(stepAddress++) = (ITM_CALtoJ >> 8) | 0x80;
    *(stepAddress++) =  ITM_CALtoJ       & 0xff;

    *(stepAddress++) = (ITM_JtoCAL >> 8) | 0x80;
    *(stepAddress++) =  ITM_JtoCAL       & 0xff;

    *(stepAddress++) = (ITM_LBFFTtoNM >> 8) | 0x80;
    *(stepAddress++) =  ITM_LBFFTtoNM       & 0xff;

    *(stepAddress++) = (ITM_LBFFTtoNMb >> 8) | 0x80;
    *(stepAddress++) =  ITM_LBFFTtoNMb       & 0xff;

    *(stepAddress++) = (ITM_NMtoLBFFT >> 8) | 0x80;
    *(stepAddress++) =  ITM_NMtoLBFFT       & 0xff;

    *(stepAddress++) = (ITM_NMtoLBFFTb >> 8) | 0x80;
    *(stepAddress++) =  ITM_NMtoLBFFTb       & 0xff;

    *(stepAddress++) = (ITM_CWTtoKG >> 8) | 0x80;
    *(stepAddress++) =  ITM_CWTtoKG       & 0xff;

    *(stepAddress++) = (ITM_KGtoCWT >> 8) | 0x80;
    *(stepAddress++) =  ITM_KGtoCWT       & 0xff;

    *(stepAddress++) = (ITM_FTtoM >> 8) | 0x80;
    *(stepAddress++) =  ITM_FTtoM       & 0xff;

    *(stepAddress++) = (ITM_MtoFT >> 8) | 0x80;
    *(stepAddress++) =  ITM_MtoFT       & 0xff;

    // 1260
    *(stepAddress++) = (ITM_FTUStoM >> 8) | 0x80;
    *(stepAddress++) =  ITM_FTUStoM       & 0xff;

    *(stepAddress++) = (ITM_FTUStoMb >> 8) | 0x80;
    *(stepAddress++) =  ITM_FTUStoMb       & 0xff;

    *(stepAddress++) = (ITM_FTUStoMc >> 8) | 0x80;
    *(stepAddress++) =  ITM_FTUStoMc       & 0xff;

    *(stepAddress++) = (ITM_MtoFTUS >> 8) | 0x80;
    *(stepAddress++) =  ITM_MtoFTUS       & 0xff;

    *(stepAddress++) = (ITM_MtoFTUSb >> 8) | 0x80;
    *(stepAddress++) =  ITM_MtoFTUSb       & 0xff;

    *(stepAddress++) = (ITM_MtoFTUSc >> 8) | 0x80;
    *(stepAddress++) =  ITM_MtoFTUSc       & 0xff;

    *(stepAddress++) = (ITM_FZUKtoM3 >> 8) | 0x80;
    *(stepAddress++) =  ITM_FZUKtoM3       & 0xff;

    *(stepAddress++) = (ITM_FZUKtoM3b >> 8) | 0x80;
    *(stepAddress++) =  ITM_FZUKtoM3b       & 0xff;

    *(stepAddress++) = (ITM_M3toFZUK >> 8) | 0x80;
    *(stepAddress++) =  ITM_M3toFZUK       & 0xff;

    *(stepAddress++) = (ITM_M3toFZUKb >> 8) | 0x80;
    *(stepAddress++) =  ITM_M3toFZUKb       & 0xff;

    // 1270
    *(stepAddress++) = (ITM_FZUStoM3 >> 8) | 0x80;
    *(stepAddress++) =  ITM_FZUStoM3       & 0xff;

    *(stepAddress++) = (ITM_FZUStoM3b >> 8) | 0x80;
    *(stepAddress++) =  ITM_FZUStoM3b       & 0xff;

    *(stepAddress++) = (ITM_M3toFZUS >> 8) | 0x80;
    *(stepAddress++) =  ITM_M3toFZUS       & 0xff;

    *(stepAddress++) = (ITM_M3toFZUSb >> 8) | 0x80;
    *(stepAddress++) =  ITM_M3toFZUSb       & 0xff;

    *(stepAddress++) = (ITM_GLUKtoM3 >> 8) | 0x80;
    *(stepAddress++) =  ITM_GLUKtoM3       & 0xff;

    *(stepAddress++) = (ITM_M3toGLUK >> 8) | 0x80;
    *(stepAddress++) =  ITM_M3toGLUK       & 0xff;

    *(stepAddress++) = (ITM_GLUStoM3 >> 8) | 0x80;
    *(stepAddress++) =  ITM_GLUStoM3       & 0xff;

    *(stepAddress++) = (ITM_M3toGLUS >> 8) | 0x80;
    *(stepAddress++) =  ITM_M3toGLUS       & 0xff;

    *(stepAddress++) = (ITM_HPEtoW >> 8) | 0x80;
    *(stepAddress++) =  ITM_HPEtoW       & 0xff;

    *(stepAddress++) = (ITM_WtoHPE >> 8) | 0x80;
    *(stepAddress++) =  ITM_WtoHPE       & 0xff;

    // 1280
    *(stepAddress++) = (ITM_HPMtoW >> 8) | 0x80;
    *(stepAddress++) =  ITM_HPMtoW       & 0xff;

    *(stepAddress++) = (ITM_WtoHPM >> 8) | 0x80;
    *(stepAddress++) =  ITM_WtoHPM       & 0xff;

    *(stepAddress++) = (ITM_HPUKtoW >> 8) | 0x80;
    *(stepAddress++) =  ITM_HPUKtoW       & 0xff;

    *(stepAddress++) = (ITM_WtoHPUK >> 8) | 0x80;
    *(stepAddress++) =  ITM_WtoHPUK       & 0xff;

    *(stepAddress++) = (ITM_IHGtoPA >> 8) | 0x80;
    *(stepAddress++) =  ITM_IHGtoPA       & 0xff;

    *(stepAddress++) = (ITM_IHGtoPA >> 8) | 0x80;
    *(stepAddress++) =  ITM_IHGtoPA       & 0xff;

    *(stepAddress++) = (ITM_IHGtoPAb >> 8) | 0x80;
    *(stepAddress++) =  ITM_IHGtoPAb       & 0xff;

    *(stepAddress++) = (ITM_PAtoIHG >> 8) | 0x80;
    *(stepAddress++) =  ITM_PAtoIHG       & 0xff;

    *(stepAddress++) = (ITM_PAtoIHGb >> 8) | 0x80;
    *(stepAddress++) =  ITM_PAtoIHGb       & 0xff;

    *(stepAddress++) = (ITM_INtoM >> 8) | 0x80;
    *(stepAddress++) =  ITM_INtoM       & 0xff;

    // 1290
    *(stepAddress++) = (ITM_MtoINCH >> 8) | 0x80;
    *(stepAddress++) =  ITM_MtoINCH       & 0xff;

    *(stepAddress++) = (ITM_WHtoJ >> 8) | 0x80;
    *(stepAddress++) =  ITM_WHtoJ       & 0xff;

    *(stepAddress++) = (ITM_JtoWH >> 8) | 0x80;
    *(stepAddress++) =  ITM_JtoWH       & 0xff;

    *(stepAddress++) = (ITM_KGtoLBS >> 8) | 0x80;
    *(stepAddress++) =  ITM_KGtoLBS       & 0xff;

    *(stepAddress++) = (ITM_LBStoKG >> 8) | 0x80;
    *(stepAddress++) =  ITM_LBStoKG       & 0xff;

    *(stepAddress++) = (ITM_KGtoOZ >> 8) | 0x80;
    *(stepAddress++) =  ITM_KGtoOZ       & 0xff;

    *(stepAddress++) = (ITM_OZtoKG >> 8) | 0x80;
    *(stepAddress++) =  ITM_OZtoKG       & 0xff;

    *(stepAddress++) = (ITM_KGtoSCW >> 8) | 0x80;
    *(stepAddress++) =  ITM_KGtoSCW       & 0xff;

    *(stepAddress++) = (ITM_KGtoSCWb >> 8) | 0x80;
    *(stepAddress++) =  ITM_KGtoSCWb       & 0xff;

    *(stepAddress++) = (ITM_SCWtoKG >> 8) | 0x80;
    *(stepAddress++) =  ITM_SCWtoKG       & 0xff;

    // 1300
    *(stepAddress++) = (ITM_SCWtoKGb >> 8) | 0x80;
    *(stepAddress++) =  ITM_SCWtoKGb       & 0xff;

    *(stepAddress++) = (ITM_KGtoSTO >> 8) | 0x80;
    *(stepAddress++) =  ITM_KGtoSTO       & 0xff;

    *(stepAddress++) = (ITM_KGtoSTOb >> 8) | 0x80;
    *(stepAddress++) =  ITM_KGtoSTOb       & 0xff;

    *(stepAddress++) = (ITM_STOtoKG >> 8) | 0x80;
    *(stepAddress++) =  ITM_STOtoKG       & 0xff;

    *(stepAddress++) = (ITM_STOtoKGb >> 8) | 0x80;
    *(stepAddress++) =  ITM_STOtoKGb       & 0xff;

    *(stepAddress++) = (ITM_KGtoST >> 8) | 0x80;
    *(stepAddress++) =  ITM_KGtoST       & 0xff;

    *(stepAddress++) = (ITM_KGtoSTb >> 8) | 0x80;
    *(stepAddress++) =  ITM_KGtoSTb       & 0xff;

    *(stepAddress++) = (ITM_KGtoSTc >> 8) | 0x80;
    *(stepAddress++) =  ITM_KGtoSTc       & 0xff;

    *(stepAddress++) = (ITM_STtoKG >> 8) | 0x80;
    *(stepAddress++) =  ITM_STtoKG       & 0xff;

    *(stepAddress++) = (ITM_STtoKGb >> 8) | 0x80;
    *(stepAddress++) =  ITM_STtoKGb       & 0xff;

    // 1310
    *(stepAddress++) = (ITM_STtoKGc >> 8) | 0x80;
    *(stepAddress++) =  ITM_STtoKGc       & 0xff;

    *(stepAddress++) = (ITM_KGtoTON >> 8) | 0x80;
    *(stepAddress++) =  ITM_KGtoTON       & 0xff;

    *(stepAddress++) = (ITM_KGtoTONb >> 8) | 0x80;
    *(stepAddress++) =  ITM_KGtoTONb       & 0xff;

    *(stepAddress++) = (ITM_KGtoTONc >> 8) | 0x80;
    *(stepAddress++) =  ITM_KGtoTONc       & 0xff;

    *(stepAddress++) = (ITM_TONtoKG >> 8) | 0x80;
    *(stepAddress++) =  ITM_TONtoKG       & 0xff;

    *(stepAddress++) = (ITM_TONtoKGb >> 8) | 0x80;
    *(stepAddress++) =  ITM_TONtoKGb       & 0xff;

    *(stepAddress++) = (ITM_TONtoKGc >> 8) | 0x80;
    *(stepAddress++) =  ITM_TONtoKGc       & 0xff;

    *(stepAddress++) = (ITM_KGtoTRZ >> 8) | 0x80;
    *(stepAddress++) =  ITM_KGtoTRZ       & 0xff;

    *(stepAddress++) = (ITM_KGtoTRZb >> 8) | 0x80;
    *(stepAddress++) =  ITM_KGtoTRZb       & 0xff;

    *(stepAddress++) = (ITM_TRZtoKG >> 8) | 0x80;
    *(stepAddress++) =  ITM_TRZtoKG       & 0xff;

    // 1320
    *(stepAddress++) = (ITM_TRZtoKGb >> 8) | 0x80;
    *(stepAddress++) =  ITM_TRZtoKGb       & 0xff;

    *(stepAddress++) = (ITM_LBFtoN >> 8) | 0x80;
    *(stepAddress++) =  ITM_LBFtoN       & 0xff;

    *(stepAddress++) = (ITM_NtoLBF >> 8) | 0x80;
    *(stepAddress++) =  ITM_NtoLBF       & 0xff;

    *(stepAddress++) = (ITM_LYtoM >> 8) | 0x80;
    *(stepAddress++) =  ITM_LYtoM       & 0xff;

    *(stepAddress++) = (ITM_MtoLY >> 8) | 0x80;
    *(stepAddress++) =  ITM_MtoLY       & 0xff;

    *(stepAddress++) = (ITM_MMHGtoPA >> 8) | 0x80;
    *(stepAddress++) =  ITM_MMHGtoPA       & 0xff;

    *(stepAddress++) = (ITM_MMHGtoPAb >> 8) | 0x80;
    *(stepAddress++) =  ITM_MMHGtoPAb       & 0xff;

    *(stepAddress++) = (ITM_PAtoMMHG >> 8) | 0x80;
    *(stepAddress++) =  ITM_PAtoMMHG       & 0xff;

    *(stepAddress++) = (ITM_PAtoMMHGb >> 8) | 0x80;
    *(stepAddress++) =  ITM_PAtoMMHGb       & 0xff;

    *(stepAddress++) = (ITM_MItoM >> 8) | 0x80;
    *(stepAddress++) =  ITM_MItoM       & 0xff;

    // 1330
    *(stepAddress++) = (ITM_MtoMI >> 8) | 0x80;
    *(stepAddress++) =  ITM_MtoMI       & 0xff;

    *(stepAddress++) = (ITM_MtoNMI >> 8) | 0x80;
    *(stepAddress++) =  ITM_MtoNMI       & 0xff;

    *(stepAddress++) = (ITM_NMItoM >> 8) | 0x80;
    *(stepAddress++) =  ITM_NMItoM       & 0xff;

    *(stepAddress++) = (ITM_MtoPC >> 8) | 0x80;
    *(stepAddress++) =  ITM_MtoPC       & 0xff;

    *(stepAddress++) = (ITM_PCtoM >> 8) | 0x80;
    *(stepAddress++) =  ITM_PCtoM       & 0xff;

    *(stepAddress++) = (ITM_MtoPOINTb >> 8) | 0x80;
    *(stepAddress++) =  ITM_MtoPOINTb       & 0xff;

    *(stepAddress++) = (ITM_MtoPOINT >> 8) | 0x80;
    *(stepAddress++) =  ITM_MtoPOINT       & 0xff;

    *(stepAddress++) = (ITM_POINTtoM >> 8) | 0x80;
    *(stepAddress++) =  ITM_POINTtoM       & 0xff;

    *(stepAddress++) = (ITM_POINTtoMb >> 8) | 0x80;
    *(stepAddress++) =  ITM_POINTtoMb       & 0xff;

    *(stepAddress++) = (ITM_POINTtoMc >> 8) | 0x80;
    *(stepAddress++) =  ITM_POINTtoMc       & 0xff;

    // 1340
    *(stepAddress++) = (ITM_MtoYD >> 8) | 0x80;
    *(stepAddress++) =  ITM_MtoYD       & 0xff;

    *(stepAddress++) = (ITM_YDtoM >> 8) | 0x80;
    *(stepAddress++) =  ITM_YDtoM       & 0xff;

    *(stepAddress++) = (ITM_PSItoPA >> 8) | 0x80;
    *(stepAddress++) =  ITM_PSItoPA       & 0xff;

    *(stepAddress++) = (ITM_PAtoPSI >> 8) | 0x80;
    *(stepAddress++) =  ITM_PAtoPSI       & 0xff;

    *(stepAddress++) = (ITM_PAtoTOR >> 8) | 0x80;
    *(stepAddress++) =  ITM_PAtoTOR       & 0xff;

    *(stepAddress++) = (ITM_PAtoTORb >> 8) | 0x80;
    *(stepAddress++) =  ITM_PAtoTORb       & 0xff;

    *(stepAddress++) = (ITM_TORtoPA >> 8) | 0x80;
    *(stepAddress++) =  ITM_TORtoPA       & 0xff;

    *(stepAddress++) = (ITM_TORtoPAb >> 8) | 0x80;
    *(stepAddress++) =  ITM_TORtoPAb       & 0xff;

    *(stepAddress++) = (ITM_StoYEAR >> 8) | 0x80;
    *(stepAddress++) =  ITM_StoYEAR       & 0xff;

    *(stepAddress++) = (ITM_YEARtoS >> 8) | 0x80;
    *(stepAddress++) =  ITM_YEARtoS       & 0xff;

    // 1350
    *(stepAddress++) = (ITM_CARATtoKG >> 8) | 0x80;
    *(stepAddress++) =  ITM_CARATtoKG       & 0xff;

    *(stepAddress++) = (ITM_CARATtoKGb >> 8) | 0x80;
    *(stepAddress++) =  ITM_CARATtoKGb       & 0xff;

    *(stepAddress++) = (ITM_CARATtoKGc >> 8) | 0x80;
    *(stepAddress++) =  ITM_CARATtoKGc       & 0xff;

    *(stepAddress++) = (ITM_KGtoCARAT >> 8) | 0x80;
    *(stepAddress++) =  ITM_KGtoCARAT       & 0xff;

    *(stepAddress++) = (ITM_KGtoCARATb >> 8) | 0x80;
    *(stepAddress++) =  ITM_KGtoCARATb       & 0xff;

    *(stepAddress++) = (ITM_KGtoCARATc >> 8) | 0x80;
    *(stepAddress++) =  ITM_KGtoCARATc       & 0xff;

    *(stepAddress++) = (ITM_QTtoM3 >> 8) | 0x80;
    *(stepAddress++) =  ITM_QTtoM3       & 0xff;

    *(stepAddress++) = (ITM_M3toQT >> 8) | 0x80;
    *(stepAddress++) =  ITM_M3toQT       & 0xff;

    *(stepAddress++) = (ITM_FATHOMtoM >> 8) | 0x80;
    *(stepAddress++) =  ITM_FATHOMtoM       & 0xff;

    *(stepAddress++) = (ITM_FATHOMtoMb >> 8) | 0x80;
    *(stepAddress++) =  ITM_FATHOMtoMb       & 0xff;

    // 1360
    *(stepAddress++) = (ITM_FATHOMtoMc >> 8) | 0x80;
    *(stepAddress++) =  ITM_FATHOMtoMc       & 0xff;

    *(stepAddress++) = (ITM_MtoFATHOM >> 8) | 0x80;
    *(stepAddress++) =  ITM_MtoFATHOM       & 0xff;

    *(stepAddress++) = (ITM_MtoFATHOMb >> 8) | 0x80;
    *(stepAddress++) =  ITM_MtoFATHOMb       & 0xff;

    *(stepAddress++) = (ITM_MtoFATHOMc >> 8) | 0x80;
    *(stepAddress++) =  ITM_MtoFATHOMc       & 0xff;

    *(stepAddress++) = (ITM_BARRELtoM3 >> 8) | 0x80;
    *(stepAddress++) =  ITM_BARRELtoM3       & 0xff;

    *(stepAddress++) = (ITM_BARRELtoM3b >> 8) | 0x80;
    *(stepAddress++) =  ITM_BARRELtoM3b       & 0xff;

    *(stepAddress++) = (ITM_M3toBARREL >> 8) | 0x80;
    *(stepAddress++) =  ITM_M3toBARREL       & 0xff;

    *(stepAddress++) = (ITM_M3toBARRELb >> 8) | 0x80;
    *(stepAddress++) =  ITM_M3toBARRELb       & 0xff;

    *(stepAddress++) = (ITM_ATMtoPAb >> 8) | 0x80;
    *(stepAddress++) =  ITM_ATMtoPAb       & 0xff;

    *(stepAddress++) = (ITM_PAtoATMb >> 8) | 0x80;
    *(stepAddress++) =  ITM_PAtoATMb       & 0xff;

    // 1370
    *(stepAddress++) = (ITM_HECTAREtoM2 >> 8) | 0x80;
    *(stepAddress++) =  ITM_HECTAREtoM2       & 0xff;

    *(stepAddress++) = (ITM_M2toHECTARE >> 8) | 0x80;
    *(stepAddress++) =  ITM_M2toHECTARE       & 0xff;



    // 1372
    *(stepAddress++) = (ITM_END >> 8) | 0x80;
    *(stepAddress++) =  ITM_END       & 0xff;

    #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
      printf("Prime + Len1 + Len2 + Len3 + Len4 + Len5 + Len6 + Len7 + Len8 + Bairstow + Speed + Fact + AllOp's : %u bytes\n", (uint32_t)(stepAddress - programMemoryPointer));
    #endif
  }
*/
  firstFreeProgramBytePointer = stepAddress;

  // 1373
  *(stepAddress++) = 255; // .END.
  *(stepAddress++) = 255; // .END.

  freeProgramBytes = (uint8_t *)(ram + RAM_SIZE) - stepAddress;
  scanLabels();

  #ifndef DMCP_BUILD
    printf("Prime + Bairstow + Speed + Fact + AllOp's + 2 : %u bytes\n", (uint32_t)(stepAddress - programMemoryPointer));
    if((uint32_t)(stepAddress - programMemoryPointer) + 2 > numberOfBytesForTheTestPrograms) {
      printf("Increase allocated memory for programs!\n");
      exit(0);
    }

    printf("freeProgramBytes = %u\n", freeProgramBytes);

    listPrograms();
    listLabels();
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

    if(tmpString == NULL) {
      #ifdef DMCP_BUILD
         tmpString        = aux_buf_ptr();   // 2560 byte buffer provided by DMCP
         errorMessage     = write_buf_ptr(); // 4096 byte buffer provided by DMCP
         aimBuffer        = errorMessage + ERROR_MESSAGE_LENGTH;
         nimBufferDisplay = aimBuffer + AIM_BUFFER_LENGTH;
         tamBuffer        = nimBufferDisplay + NIM_BUFFER_LENGTH;
       #else
         tmpString        = (char *)malloc(TMP_STR_LENGTH);
         errorMessage     = (char *)malloc(ERROR_MESSAGE_LENGTH);
         aimBuffer        = (char *)malloc(AIM_BUFFER_LENGTH);
         nimBufferDisplay = (char *)malloc(NIM_BUFFER_LENGTH);
         tamBuffer        = (char *)malloc(TAM_BUFFER_LENGTH);
      #endif // DMCP_BUILD
    }
    memset(tmpString,        0, TMP_STR_LENGTH);
    memset(errorMessage,     0, ERROR_MESSAGE_LENGTH);
    memset(aimBuffer,        0, AIM_BUFFER_LENGTH);
    memset(nimBufferDisplay, 0, NIM_BUFFER_LENGTH);
    memset(tamBuffer,        0, TAM_BUFFER_LENGTH);

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
printf("freeProgramBytes = %u\n", freeProgramBytes);

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

    shortIntegerMode = SIM_2COMPL;
    fnSetWordSize(64);

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

    //strcpy(tmpString, "Pure ASCII string requiring 38 bytes!");
    //reallocateRegister(FIRST_LOCAL_REGISTER+1, dtString, TO_BLOCKS(strlen(tmpString) + 1), AM_NONE);
    //strcpy(REGISTER_STRING_DATA(FIRST_LOCAL_REGISTER + 1), tmpString);


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
