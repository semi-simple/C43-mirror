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
 * \file registerValueConversions.c
 ***********************************************/

#include "wp43s.h"



/********************************************//**
 * \brief Convert a register to a real34 or complex
 *        if it is not already. Throws an
 *        error if X is not a single or double
 *        precision real or complex.
 *
 * \param r calcRegister_t Register number
 * \return void
 ***********************************************/
void convertRegister16To34(calcRegister_t regist) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("convertRegister16To34");
  #endif

  // Register is already a real34 or complex34
  if(getRegisterDataType(regist) == dtReal34|| getRegisterDataType(regist) == dtComplex34) {

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("convertRegister16To34");
    #endif

    return;
  }

  if(getRegisterDataType(regist) == dtReal16) {
    calcRegister_t temp = allocateTemporaryRegister();
    reallocateRegister(temp, dtReal34, REAL34_SIZE, getRegisterDataInfo(regist));
    real16ToReal34(POINTER_TO_REGISTER_DATA(regist), POINTER_TO_REGISTER_DATA(temp));
    copySourceRegisterToDestRegister(temp, regist);
    freeTemporaryRegister(temp);
  }
  else if(getRegisterDataType(regist) == dtComplex16) {
    calcRegister_t temp = allocateTemporaryRegister();
    reallocateRegister(temp, dtComplex34, COMPLEX34_SIZE, getRegisterDataInfo(regist));
    real16ToReal34(POINTER_TO_REGISTER_DATA(regist), POINTER_TO_REGISTER_DATA(temp));
    real16ToReal34(POINTER_TO_REGISTER_DATA(regist) + REAL16_SIZE, POINTER_TO_REGISTER_DATA(temp) + REAL34_SIZE);
    copySourceRegisterToDestRegister(temp, regist);
    freeTemporaryRegister(temp);
  }
  else {
    displayBugScreen("In function convertRegister16To34: the register to convert must be real16 or complex16!");
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("convertRegister16To34");
  #endif
}



/********************************************//**
 * \brief Convert a register to a real16 or complex
 *        if it is not already. Throws an
 *        error if X is not a single or double
 *        precision real or complex.
 *
 * \param r calcRegister_t Register number
 * \return void
 ***********************************************/
void convertRegister34To16(calcRegister_t regist) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("convertRegister34To16");
  #endif

  // Register is already a real16 or complex16
  if(getRegisterDataType(regist) == dtReal16|| getRegisterDataType(regist) == dtComplex16) {

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("convertRegister34To16");
    #endif

    return;
  }

  if(getRegisterDataType(regist) == dtReal34) {
    calcRegister_t temp = allocateTemporaryRegister();
    reallocateRegister(temp, dtReal16, REAL16_SIZE, getRegisterDataInfo(regist));
    real34ToReal16(POINTER_TO_REGISTER_DATA(regist), POINTER_TO_REGISTER_DATA(temp));
    copySourceRegisterToDestRegister(temp, regist);
    freeTemporaryRegister(temp);
  }
  else if(getRegisterDataType(regist) == dtComplex34) {
    calcRegister_t temp = allocateTemporaryRegister();
    reallocateRegister(temp, dtComplex16, COMPLEX16_SIZE, getRegisterDataInfo(regist));
    real34ToReal16(POINTER_TO_REGISTER_DATA(regist), POINTER_TO_REGISTER_DATA(temp));
    real34ToReal16(POINTER_TO_REGISTER_DATA(regist) + REAL34_SIZE, POINTER_TO_REGISTER_DATA(temp) + REAL16_SIZE);
    copySourceRegisterToDestRegister(temp, regist);
    freeTemporaryRegister(temp);
  }
  else {
    displayBugScreen("In function convertRegister34To16: the register to convert must be real34 or complex34!");
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("convertRegister34To16");
  #endif
}



void convertBigIntegerToBigIntegerRegister(const bigInteger_t *bigInteger, calcRegister_t regist) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("convertBigIntegerToBigIntegerRegister");
  #endif

  reallocateRegister(regist, dtBigInteger, bigInteger->used * (DIGIT_BIT / CHAR_BIT), bigInteger->sign);
  memcpy(POINTER_TO_REGISTER_BIG_INTEGER(regist), bigInteger, bigInteger->used * (DIGIT_BIT / CHAR_BIT));

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("convertBigIntegerToBigIntegerRegister");
  #endif
}



void convertBigIntegerRegisterToBigInteger(calcRegister_t regist, bigInteger_t *bigInteger) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("convertBigIntegerRegisterToBigInteger");
  #endif

  bigIntegerSetZero(bigInteger);
  bigInteger->sign = getRegisterDataInfo(regist);
  bigInteger->used = *(uint16_t *)POINTER_TO_REGISTER_DATA(regist);
  memcpy(bigInteger, POINTER_TO_REGISTER_BIG_INTEGER(regist), bigInteger->used);
  bigInteger->used = (bigInteger->used * CHAR_BIT) / DIGIT_BIT;

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("convertBigIntegerRegisterToBigInteger");
  #endif
}



void convertBigIntegerRegisterToReal16Register(calcRegister_t source, calcRegister_t destination) {
  bigInteger_t tmp;

  #if (LOG_FUNCTIONS == 1)
    enteringFunction("convertBigIntegerRegisterToReal16Register");
  #endif

  convertBigIntegerRegisterToBigInteger(source, &tmp);
  bigIntegerToString(&tmp, tmpStr3000, 10);
  reallocateRegister(destination, dtReal16, REAL16_SIZE, 0);
  stringToReal16(tmpStr3000, POINTER_TO_REGISTER_DATA(destination));

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("convertBigIntegerRegisterToReal16Register");
  #endif
}



void convertBigIntegerRegisterToSmallIntegerRegister(calcRegister_t source, calcRegister_t destination) {
  bigInteger_t tmp;

  #if (LOG_FUNCTIONS == 1)
    enteringFunction("convertBigIntegerRegisterToReal16Register");
  #endif

  convertBigIntegerRegisterToBigInteger(source, &tmp);
  reallocateRegister(destination, dtSmallInteger, SMALL_INTEGER_SIZE, 0);
  *(uint64_t *)(POINTER_TO_REGISTER_DATA(destination)) = tmp.dp[0] & smallIntegerMask;
  if(bigIntegerIsNegative(&tmp)) {
    *(uint64_t *)(POINTER_TO_REGISTER_DATA(destination)) = WP34S_intChs(*(uint64_t *)(POINTER_TO_REGISTER_DATA(destination)));
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("convertBigIntegerRegisterToReal16Register");
  #endif
}



void convertBigIntegerRegisterToReal34Register(calcRegister_t source, calcRegister_t destination) {
  bigInteger_t tmp;

  #if (LOG_FUNCTIONS == 1)
    enteringFunction("convertBigIntegerRegisterToReal34Register");
  #endif

  convertBigIntegerRegisterToBigInteger(source, &tmp);
  bigIntegerToString(&tmp, tmpStr3000, 10);
  reallocateRegister(destination, dtReal34, REAL34_SIZE, 0);
  stringToReal34(tmpStr3000, POINTER_TO_REGISTER_DATA(destination));

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("convertBigIntegerRegisterToReal34Register");
  #endif
}



void convertSmallIntegerRegisterToReal16Register(calcRegister_t source, calcRegister_t destination) {
  bigInteger_t tmp;
  uint64_t value = *(uint64_t *)(POINTER_TO_REGISTER_DATA(source));

  #if (LOG_FUNCTIONS == 1)
    enteringFunction("convertSmallIntegerRegisterToReal16Register");
  #endif

  if(smallIntegerMode == SIM_UNSIGN) {
    uIntToBigInteger(value, &tmp);
  }
  else {
    if(value & smallIntegerSignBit) { // Negative value
      if(smallIntegerMode == SIM_2COMPL) {
        value = ((~value) + 1) & smallIntegerMask;
      }
      else if(smallIntegerMode == SIM_1COMPL) {
        value = (~value) & smallIntegerMask;
      }
      else if(smallIntegerMode == SIM_SIGNMT) {
        value -= smallIntegerSignBit;
      }
      else {
        sprintf(errorMessage, "In function convertSmallIntegerRegisterToReal16Register: %" FMT8U " is an unexpected value for smallIntegerMode!", smallIntegerMode);
        displayBugScreen(errorMessage);
      }

      uIntToBigInteger(value, &tmp);
      bigIntegerSetNegativeSign(&tmp);
    }
    else { // Positive value
      uIntToBigInteger(value, &tmp);
    }
  }

  bigIntegerToString(&tmp, tmpStr3000, 10);
  reallocateRegister(destination, dtReal16, REAL16_SIZE, 0);
  stringToReal16(tmpStr3000, POINTER_TO_REGISTER_DATA(destination));

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("convertSmallIntegerRegisterToReal16Register");
  #endif
}



void convertSmallIntegerRegisterToReal34Register(calcRegister_t source, calcRegister_t destination) {
  bigInteger_t tmp;
  uint64_t value = *(uint64_t *)(POINTER_TO_REGISTER_DATA(source));

  #if (LOG_FUNCTIONS == 1)
    enteringFunction("convertSmallIntegerRegisterToReal34Register");
  #endif

  if(smallIntegerMode == SIM_UNSIGN) {
    uIntToBigInteger(value, &tmp);
  }
  else {
    if(value & smallIntegerSignBit) { // Negative value
      if(smallIntegerMode == SIM_2COMPL) {
        value = ((~value) + 1) & smallIntegerMask;
      }
      else if(smallIntegerMode == SIM_1COMPL) {
        value = (~value) & smallIntegerMask;
      }
      else if(smallIntegerMode == SIM_SIGNMT) {
        value -= smallIntegerSignBit;
      }
      else {
        sprintf(errorMessage, "In function convertSmallIntegerRegisterToReal34Register: %" FMT8U " is an unexpected value for smallIntegerMode!", smallIntegerMode);
        displayBugScreen(errorMessage);
      }

      uIntToBigInteger(value, &tmp);
      bigIntegerSetNegativeSign(&tmp);
    }
    else { // Positive value
      uIntToBigInteger(value, &tmp);
    }
  }

  bigIntegerToString(&tmp, tmpStr3000, 10);
  reallocateRegister(destination, dtReal34, REAL34_SIZE, 0);
  stringToReal34(tmpStr3000, POINTER_TO_REGISTER_DATA(destination));

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("convertSmallIntegerRegisterToReal34Register");
  #endif
}



void convertSmallIntegerRegisterToUInt64(calcRegister_t regist, int16_t *sign, uint64_t *value) {
  *value = *(uint64_t *)(POINTER_TO_REGISTER_DATA(regist));

  #if (LOG_FUNCTIONS == 1)
    enteringFunction("convertSmallIntegerRegisterToUInt64");
  #endif

  if(smallIntegerMode == SIM_UNSIGN) {
    *sign = 0;
  }
  else {
    if(*value & smallIntegerSignBit) { // Negative value
      *sign = 1;

      if(smallIntegerMode == SIM_2COMPL) {
        *value = ((~*value) + 1) & smallIntegerMask;
      }
      else if(smallIntegerMode == SIM_1COMPL) {
        *value = (~*value) & smallIntegerMask;
      }
      else if(smallIntegerMode == SIM_SIGNMT) {
       *value -= smallIntegerSignBit;
      }
      else {
        sprintf(errorMessage, "In function convertSmallIntegerRegisterToUInt64: %" FMT8U " is an unexpected value for smallIntegerMode!", smallIntegerMode);
        displayBugScreen(errorMessage);
        *sign = 0;
        *value = 0;
      }
    }
    else { // Positive value
      *sign = 0;
    }
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("convertSmallIntegerRegisterToUInt64");
  #endif
}



void convertSmallIntegerRegisterBigIntegerRegister(calcRegister_t source, calcRegister_t destination) {
  bigInteger_t tmp;
  uint64_t value = *(uint64_t *)(POINTER_TO_REGISTER_DATA(source));

  #if (LOG_FUNCTIONS == 1)
    enteringFunction("convertSmallIntegerRegisterBigIntegerRegister");
  #endif

  if(smallIntegerMode == SIM_UNSIGN) {
    uIntToBigInteger(value, &tmp);
  }
  else {
    if(value & smallIntegerSignBit) { // Negative value
      if(smallIntegerMode == SIM_2COMPL) {
        value = ((~value) + 1) & smallIntegerMask;
      }
      else if(smallIntegerMode == SIM_1COMPL) {
        value = (~value) & smallIntegerMask;
      }
      else if(smallIntegerMode == SIM_SIGNMT) {
        value -= smallIntegerSignBit;
      }
      else {
        sprintf(errorMessage, "In function convertSmallIntegerRegisterToRea16lRegister: %" FMT8U " is an unexpected value for smallIntegerMode!", smallIntegerMode);
        displayBugScreen(errorMessage);
      }

      uIntToBigInteger(value, &tmp);
      bigIntegerSetNegativeSign(&tmp);
    }
    else {// Positive value
      uIntToBigInteger(value, &tmp);
    }
  }

  convertBigIntegerToBigIntegerRegister(&tmp, destination);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("convertSmallIntegerRegisterBigIntegerRegister");
  #endif
}



void convertUInt64ToSmallIntegerRegister(int16_t sign, uint64_t value, uint32_t base, calcRegister_t regist) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("convertUInt64ToSmallIntegerRegister");
  #endif

  if(sign) { // Negative value
    if(smallIntegerMode == SIM_2COMPL) {
      value = (~value) + 1;
    }
    else if(smallIntegerMode == SIM_1COMPL) {
      value = ~value;
    }
    else if(smallIntegerMode == SIM_SIGNMT) {
      value += smallIntegerSignBit;
    }
    else {
      sprintf(errorMessage, "In function convertUInt64ToSmallIntegerRegister: %" FMT8U " is an unexpected value for smallIntegerMode!", smallIntegerMode);
      displayBugScreen(errorMessage);
      value = 0;
    }
  }

  reallocateRegister(result, dtSmallInteger, SMALL_INTEGER_SIZE, base);
  *(uint64_t *)(POINTER_TO_REGISTER_DATA(regist)) = value & smallIntegerMask;

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("convertUInt64ToSmallIntegerRegister");
  #endif
}
