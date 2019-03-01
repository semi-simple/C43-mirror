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
  // Register is already a real34 or complex34
  #if (ANGLE16 == 1)
    if(getRegisterDataType(regist) == dtReal34 || getRegisterDataType(regist) == dtComplex34) {
      return;
    }
  #endif
  #if (ANGLE34 == 1)
    if(getRegisterDataType(regist) == dtReal34 || getRegisterDataType(regist) == dtComplex34 || getRegisterDataType(regist) == dtAngle) {
      return;
    }
  #endif

  if(getRegisterDataType(regist) == dtReal16 || getRegisterDataType(regist) == dtAngle) {
    calcRegister_t temp = allocateTemporaryRegister();
    reallocateRegister(temp, dtReal34, REAL34_SIZE, getRegisterDataInfo(regist));
    real16ToReal34(REGISTER_REAL16_DATA(regist), REGISTER_REAL34_DATA(temp));
    copySourceRegisterToDestRegister(temp, regist);
    freeTemporaryRegister(temp);
  }
  else if(getRegisterDataType(regist) == dtComplex16) {
    calcRegister_t temp = allocateTemporaryRegister();
    reallocateRegister(temp, dtComplex34, COMPLEX34_SIZE, getRegisterDataInfo(regist));
    real16ToReal34(REGISTER_REAL16_DATA(regist), REGISTER_REAL34_DATA(temp));
    real16ToReal34(REGISTER_IMAG16_DATA(regist), REGISTER_IMAG34_DATA(temp));
    copySourceRegisterToDestRegister(temp, regist);
    freeTemporaryRegister(temp);
  }
  else {
    displayBugScreen("In function convertRegister16To34: the register to convert must be real16 or complex16!");
  }
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
  // Register is already a real16 or complex16
  #if (ANGLE16 == 1)
    if(getRegisterDataType(regist) == dtReal16 || getRegisterDataType(regist) == dtComplex16 || getRegisterDataType(regist) == dtAngle) {
      return;
    }
  #endif
  #if (ANGLE34 == 1)
    if(getRegisterDataType(regist) == dtReal16 || getRegisterDataType(regist) == dtComplex16) {
      return;
    }
  #endif

  if(getRegisterDataType(regist) == dtReal34 || getRegisterDataType(regist) == dtAngle) {
    calcRegister_t temp = allocateTemporaryRegister();
    reallocateRegister(temp, dtReal16, REAL16_SIZE, getRegisterDataInfo(regist));
    real34ToReal16(REGISTER_REAL34_DATA(regist), REGISTER_REAL16_DATA(temp));
    copySourceRegisterToDestRegister(temp, regist);
    freeTemporaryRegister(temp);
  }
  else if(getRegisterDataType(regist) == dtComplex34) {
    calcRegister_t temp = allocateTemporaryRegister();
    reallocateRegister(temp, dtComplex16, COMPLEX16_SIZE, getRegisterDataInfo(regist));
    real34ToReal16(REGISTER_REAL34_DATA(regist), REGISTER_REAL16_DATA(temp));
    real34ToReal16(REGISTER_IMAG34_DATA(regist), REGISTER_IMAG16_DATA(temp));
    copySourceRegisterToDestRegister(temp, regist);
    freeTemporaryRegister(temp);
  }
  else {
    displayBugScreen("In function convertRegister34To16: the register to convert must be real34 or complex34!");
  }
}



void convertLongIntegerToLongIntegerRegister(const longInteger_t *longInteger, calcRegister_t regist) {
  reallocateRegister(regist, dtLongInteger, longInteger->used * SIZEOF_FP_DIGIT, longInteger->sign);
  memcpy(REGISTER_LONG_INTEGER_DATA(regist), longInteger, longInteger->used * SIZEOF_FP_DIGIT);
}



void convertLongIntegerRegisterToLongInteger(calcRegister_t regist, longInteger_t *longInteger) {
  longIntegerSetZero(longInteger);
  longInteger->sign = getRegisterDataInfo(regist);
  longInteger->used = *REGISTER_DATA_MAX_LEN(regist);
  memcpy(longInteger, REGISTER_LONG_INTEGER_DATA(regist), longInteger->used);
  longInteger->used = (longInteger->used * CHAR_BIT) / DIGIT_BIT;
}



void convertLongIntegerRegisterToReal16Register(calcRegister_t source, calcRegister_t destination) {
  longInteger_t tmp;

  convertLongIntegerRegisterToLongInteger(source, &tmp);
  longIntegerToString(&tmp, tmpStr3000, 10);
  reallocateRegister(destination, dtReal16, REAL16_SIZE, 0);
  stringToReal16(tmpStr3000, REGISTER_REAL16_DATA(destination));
}



void convertLongIntegerRegisterToShortIntegerRegister(calcRegister_t source, calcRegister_t destination) {
  longInteger_t tmp;

  convertLongIntegerRegisterToLongInteger(source, &tmp);
  reallocateRegister(destination, dtShortInteger, SHORT_INTEGER_SIZE, 0);
  *(REGISTER_SHORT_INTEGER_DATA(destination)) = tmp.dp[0] & shortIntegerMask;
  if(longIntegerIsNegative(&tmp)) {
    *(REGISTER_SHORT_INTEGER_DATA(destination)) = WP34S_intChs(*(REGISTER_SHORT_INTEGER_DATA(destination)));
  }
}



void convertLongIntegerRegisterToReal34Register(calcRegister_t source, calcRegister_t destination) {
  longInteger_t tmp;

  convertLongIntegerRegisterToLongInteger(source, &tmp);
  longIntegerToString(&tmp, tmpStr3000, 10);
  reallocateRegister(destination, dtReal34, REAL34_SIZE, 0);
  stringToReal34(tmpStr3000, REGISTER_REAL34_DATA(destination));
}



void convertShortIntegerRegisterToReal16Register(calcRegister_t source, calcRegister_t destination) {
  longInteger_t tmp;
  uint64_t value = *(REGISTER_SHORT_INTEGER_DATA(source));

  if(shortIntegerMode == SIM_UNSIGN) {
    uIntToLongInteger(value, &tmp);
  }
  else {
    if(value & shortIntegerSignBit) { // Negative value
      if(shortIntegerMode == SIM_2COMPL) {
        value = ((~value) + 1) & shortIntegerMask;
      }
      else if(shortIntegerMode == SIM_1COMPL) {
        value = (~value) & shortIntegerMask;
      }
      else if(shortIntegerMode == SIM_SIGNMT) {
        value -= shortIntegerSignBit;
      }
      else {
        sprintf(errorMessage, "In function convertShortIntegerRegisterToReal16Register: %" FMT8U " is an unexpected value for shortIntegerMode!", shortIntegerMode);
        displayBugScreen(errorMessage);
      }

      uIntToLongInteger(value, &tmp);
      longIntegerSetNegativeSign(&tmp);
    }
    else { // Positive value
      uIntToLongInteger(value, &tmp);
    }
  }

  longIntegerToString(&tmp, tmpStr3000, 10);
  reallocateRegister(destination, dtReal16, REAL16_SIZE, 0);
  stringToReal16(tmpStr3000, REGISTER_REAL16_DATA(destination));
}



void convertShortIntegerRegisterToReal34Register(calcRegister_t source, calcRegister_t destination) {
  longInteger_t tmp;
  uint64_t value = *(REGISTER_SHORT_INTEGER_DATA(source));

  if(shortIntegerMode == SIM_UNSIGN) {
    uIntToLongInteger(value, &tmp);
  }
  else {
    if(value & shortIntegerSignBit) { // Negative value
      if(shortIntegerMode == SIM_2COMPL) {
        value = ((~value) + 1) & shortIntegerMask;
      }
      else if(shortIntegerMode == SIM_1COMPL) {
        value = (~value) & shortIntegerMask;
      }
      else if(shortIntegerMode == SIM_SIGNMT) {
        value -= shortIntegerSignBit;
      }
      else {
        sprintf(errorMessage, "In function convertShortIntegerRegisterToReal34Register: %" FMT8U " is an unexpected value for shortIntegerMode!", shortIntegerMode);
        displayBugScreen(errorMessage);
      }

      uIntToLongInteger(value, &tmp);
      longIntegerSetNegativeSign(&tmp);
    }
    else { // Positive value
      uIntToLongInteger(value, &tmp);
    }
  }

  longIntegerToString(&tmp, tmpStr3000, 10);
  reallocateRegister(destination, dtReal34, REAL34_SIZE, 0);
  stringToReal34(tmpStr3000, REGISTER_REAL34_DATA(destination));
}



void convertShortIntegerRegisterToUInt64(calcRegister_t regist, int16_t *sign, uint64_t *value) {
  *value = *(REGISTER_SHORT_INTEGER_DATA(regist));

  if(shortIntegerMode == SIM_UNSIGN) {
    *sign = 0;
  }
  else {
    if(*value & shortIntegerSignBit) { // Negative value
      *sign = 1;

      if(shortIntegerMode == SIM_2COMPL) {
        *value = ((~*value) + 1) & shortIntegerMask;
      }
      else if(shortIntegerMode == SIM_1COMPL) {
        *value = (~*value) & shortIntegerMask;
      }
      else if(shortIntegerMode == SIM_SIGNMT) {
       *value -= shortIntegerSignBit;
      }
      else {
        sprintf(errorMessage, "In function convertShortIntegerRegisterToUInt64: %" FMT8U " is an unexpected value for shortIntegerMode!", shortIntegerMode);
        displayBugScreen(errorMessage);
        *sign = 0;
        *value = 0;
      }
    }
    else { // Positive value
      *sign = 0;
    }
  }
}



void convertShortIntegerRegisterLongIntegerRegister(calcRegister_t source, calcRegister_t destination) {
  longInteger_t tmp;
  uint64_t value = *(REGISTER_SHORT_INTEGER_DATA(source));

  if(shortIntegerMode == SIM_UNSIGN) {
    uIntToLongInteger(value, &tmp);
  }
  else {
    if(value & shortIntegerSignBit) { // Negative value
      if(shortIntegerMode == SIM_2COMPL) {
        value = ((~value) + 1) & shortIntegerMask;
      }
      else if(shortIntegerMode == SIM_1COMPL) {
        value = (~value) & shortIntegerMask;
      }
      else if(shortIntegerMode == SIM_SIGNMT) {
        value -= shortIntegerSignBit;
      }
      else {
        sprintf(errorMessage, "In function convertShortIntegerRegisterToRea16lRegister: %" FMT8U " is an unexpected value for shortIntegerMode!", shortIntegerMode);
        displayBugScreen(errorMessage);
      }

      uIntToLongInteger(value, &tmp);
      longIntegerSetNegativeSign(&tmp);
    }
    else {// Positive value
      uIntToLongInteger(value, &tmp);
    }
  }

  convertLongIntegerToLongIntegerRegister(&tmp, destination);
}



void convertUInt64ToShortIntegerRegister(int16_t sign, uint64_t value, uint32_t base, calcRegister_t regist) {
  if(sign) { // Negative value
    if(shortIntegerMode == SIM_2COMPL) {
      value = (~value) + 1;
    }
    else if(shortIntegerMode == SIM_1COMPL) {
      value = ~value;
    }
    else if(shortIntegerMode == SIM_SIGNMT) {
      value += shortIntegerSignBit;
    }
    else {
      sprintf(errorMessage, "In function convertUInt64ToShortIntegerRegister: %" FMT8U " is an unexpected value for shortIntegerMode!", shortIntegerMode);
      displayBugScreen(errorMessage);
      value = 0;
    }
  }

  reallocateRegister(result, dtShortInteger, SHORT_INTEGER_SIZE, base);
  *(REGISTER_SHORT_INTEGER_DATA(regist)) = value & shortIntegerMask;
}
