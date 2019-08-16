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
  complex34_t temp;
  //calcRegister_t temp;

  switch(getRegisterDataType(regist)) {
    case dtReal16:
      real16ToReal34(REGISTER_REAL16_DATA(regist), VARIABLE_REAL34_DATA(&temp));
      reallocateRegister(regist, dtReal34, REAL34_SIZE, getRegisterTag(regist));
      real34Copy(&temp, REGISTER_REAL34_DATA(regist));
      break;

    case dtComplex16:
      real16ToReal34(REGISTER_REAL16_DATA(regist), VARIABLE_REAL34_DATA(&temp));
      real16ToReal34(REGISTER_IMAG16_DATA(regist), VARIABLE_IMAG34_DATA(&temp));
      reallocateRegister(regist, dtComplex34, COMPLEX34_SIZE, getRegisterTag(regist));
      complex34Copy(&temp, REGISTER_COMPLEX34_DATA(regist));
      break;

    case dtAngle16:
      real16ToReal34(REGISTER_REAL16_DATA(regist), VARIABLE_REAL34_DATA(&temp));
      reallocateRegister(regist, dtAngle34, REAL34_SIZE, getRegisterTag(regist));
      real34Copy(&temp, REGISTER_REAL34_DATA(regist));
      break;

    case dtReal34:
    case dtComplex34:
    case dtAngle34:
      break;

    default:
      displayBugScreen("In function convertRegister16To34: the register to convert must be real16, angle16 or complex16!");
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
  complex16_t temp;
  //calcRegister_t temp;

  switch(getRegisterDataType(regist)) {
    case dtReal16:
    case dtComplex16:
    case dtAngle16:
      break;

    case dtReal34:
      real34ToReal16(REGISTER_REAL34_DATA(regist), VARIABLE_REAL16_DATA(&temp));
      reallocateRegister(regist, dtReal16, REAL16_SIZE, getRegisterTag(regist));
      real16Copy(&temp, REGISTER_REAL16_DATA(regist));
      break;

    case dtComplex34:
      real34ToReal16(REGISTER_REAL34_DATA(regist), VARIABLE_REAL16_DATA(&temp));
      real34ToReal16(REGISTER_IMAG34_DATA(regist), VARIABLE_IMAG16_DATA(&temp));
      reallocateRegister(regist, dtComplex16, COMPLEX16_SIZE, getRegisterTag(regist));
      complex16Copy(&temp, REGISTER_COMPLEX16_DATA(regist));
      break;

    case dtAngle34:
      real34ToReal16(REGISTER_REAL34_DATA(regist), VARIABLE_REAL16_DATA(&temp));
      reallocateRegister(regist, dtAngle16, REAL16_SIZE, getRegisterTag(regist));
      real16Copy(&temp, REGISTER_REAL16_DATA(regist));
      break;

    default:
      displayBugScreen("In function convertRegister34To16: the register to convert must be real34, angle34 or complex34!");
  }
}



void convertLongIntegerToLongIntegerRegister(const longInteger_t lgInt, calcRegister_t regist) {
  reallocateRegister(regist, dtLongInteger, longIntegerSizeInBytes(lgInt), longIntegerSignTag(lgInt));
  memcpy(REGISTER_LONG_INTEGER_DATA(regist), lgInt->_mp_d, longIntegerSizeInBytes(lgInt));
}



void convertLongIntegerRegisterToLongInteger(calcRegister_t regist, longInteger_t lgInt) {
  longIntegerInitSizeInBits(lgInt, *REGISTER_DATA_MAX_LEN(regist) * 8);
  lgInt->_mp_size = *REGISTER_DATA_MAX_LEN(regist);

  memcpy(lgInt->_mp_d, REGISTER_LONG_INTEGER_DATA(regist), lgInt->_mp_size);

  if(getRegisterLongIntegerSign(regist) == LONG_INTEGER_NEGATIVE) {
    lgInt->_mp_size = -(lgInt->_mp_size / LIMB_SIZE);
  }
  else {
    lgInt->_mp_size = lgInt->_mp_size / LIMB_SIZE;
  }
}



void convertLongIntegerRegisterToReal16Register(calcRegister_t source, calcRegister_t destination) {
  longInteger_t lgInt;

  convertLongIntegerRegisterToLongInteger(source, lgInt);
  longIntegerToAllocatedString(lgInt, tmpStr3000, 10);
  longIntegerFree(lgInt);
  reallocateRegister(destination, dtReal16, REAL16_SIZE, TAG_NONE);
  stringToReal16(tmpStr3000, REGISTER_REAL16_DATA(destination));
}



void convertLongIntegerRegisterToShortIntegerRegister(calcRegister_t source, calcRegister_t destination) {
  longInteger_t lgInt;

  convertLongIntegerRegisterToLongInteger(source, lgInt);
  reallocateRegister(destination, dtShortInteger, SHORT_INTEGER_SIZE, 10);
  *(REGISTER_SHORT_INTEGER_DATA(destination)) = longIntegerToUInt(lgInt) & shortIntegerMask;
  if(longIntegerIsNegative(lgInt)) {
    *(REGISTER_SHORT_INTEGER_DATA(destination)) = WP34S_intChs(*(REGISTER_SHORT_INTEGER_DATA(destination)));
  }

  longIntegerFree(lgInt);
}



void convertLongIntegerRegisterToReal34Register(calcRegister_t source, calcRegister_t destination) {
  longInteger_t lgInt;

  convertLongIntegerRegisterToLongInteger(source, lgInt);
  longIntegerToAllocatedString(lgInt, tmpStr3000, 10);
  longIntegerFree(lgInt);
  reallocateRegister(destination, dtReal34, REAL34_SIZE, TAG_NONE);
  stringToReal34(tmpStr3000, REGISTER_REAL34_DATA(destination));
}



void convertShortIntegerRegisterToReal16Register(calcRegister_t source, calcRegister_t destination) {
  longInteger_t lgInt;
  uint64_t value = *(REGISTER_SHORT_INTEGER_DATA(source));

  longIntegerInit(lgInt);
  if(shortIntegerMode == SIM_UNSIGN) {
    uIntToLongInteger(value, lgInt);
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

      uIntToLongInteger(value, lgInt);
      longIntegerSetNegativeSign(lgInt);
    }
    else { // Positive value
      uIntToLongInteger(value, lgInt);
    }
  }

  longIntegerToAllocatedString(lgInt, tmpStr3000, 10);
  longIntegerFree(lgInt);
  reallocateRegister(destination, dtReal16, REAL16_SIZE, TAG_NONE);
  stringToReal16(tmpStr3000, REGISTER_REAL16_DATA(destination));
}



void convertShortIntegerRegisterToReal34Register(calcRegister_t source, calcRegister_t destination) {
  longInteger_t lgInt;
  uint64_t value = *(REGISTER_SHORT_INTEGER_DATA(source));

  longIntegerInit(lgInt);
  if(shortIntegerMode == SIM_UNSIGN) {
    uIntToLongInteger(value, lgInt);
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

      uIntToLongInteger(value, lgInt);
      longIntegerSetNegativeSign(lgInt);
    }
    else { // Positive value
      uIntToLongInteger(value, lgInt);
    }
  }

  longIntegerToAllocatedString(lgInt, tmpStr3000, 10);
  longIntegerFree(lgInt);
  reallocateRegister(destination, dtReal34, REAL34_SIZE, TAG_NONE);
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
  longInteger_t lgInt;
  uint64_t value = *(REGISTER_SHORT_INTEGER_DATA(source));

  longIntegerInit(lgInt);
  if(shortIntegerMode == SIM_UNSIGN) {
    uIntToLongInteger(value, lgInt);
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

      uIntToLongInteger(value, lgInt);
      longIntegerSetNegativeSign(lgInt);
    }
    else {// Positive value
      uIntToLongInteger(value, lgInt);
    }
  }

  convertLongIntegerToLongIntegerRegister(lgInt, destination);
  longIntegerFree(lgInt);
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

  reallocateRegister(REGISTER_X, dtShortInteger, SHORT_INTEGER_SIZE, base);
  *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = value & shortIntegerMask;
}
