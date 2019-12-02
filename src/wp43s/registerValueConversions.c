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

    case dtReal34:
    case dtComplex34:
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

  switch(getRegisterDataType(regist)) {
    case dtReal16:
    case dtComplex16:
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

    default:
      displayBugScreen("In function convertRegister34To16: the register to convert must be real34, angle34 or complex34!");
  }
}



void convertLongIntegerToLongIntegerRegister(const longInteger_t lgInt, calcRegister_t regist) {
  reallocateRegister(regist, dtLongInteger, longIntegerSizeInBytes(lgInt), longIntegerSignTag(lgInt));
  memcpy(REGISTER_LONG_INTEGER_DATA(regist), lgInt->_mp_d, longIntegerSizeInBytes(lgInt));
}



void convertLongIntegerRegisterToLongInteger(calcRegister_t regist, longInteger_t lgInt) {
  longIntegerInitSizeInBits(lgInt, max(*REGISTER_DATA_MAX_LEN(regist) * 8, 64));
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
  longIntegerToAllocatedString(lgInt, tmpStr3000, TMP_STR_LENGTH);
  longIntegerFree(lgInt);
  reallocateRegister(destination, dtReal16, REAL16_SIZE, AM_NONE);
  stringToReal16(tmpStr3000, REGISTER_REAL16_DATA(destination));
}



void convertLongIntegerRegisterToReal34Register(calcRegister_t source, calcRegister_t destination) {
  longInteger_t lgInt;

  convertLongIntegerRegisterToLongInteger(source, lgInt);
  longIntegerToAllocatedString(lgInt, tmpStr3000, TMP_STR_LENGTH);
  longIntegerFree(lgInt);
  reallocateRegister(destination, dtReal34, REAL34_SIZE, AM_NONE);
  stringToReal34(tmpStr3000, REGISTER_REAL34_DATA(destination));
}



void convertLongIntegerRegisterToReal16(calcRegister_t source, real16_t *destination) {
  longInteger_t lgInt;

  convertLongIntegerRegisterToLongInteger(source, lgInt);
  longIntegerToAllocatedString(lgInt, tmpStr3000, TMP_STR_LENGTH);
  longIntegerFree(lgInt);
  stringToReal16(tmpStr3000, destination);
}



void convertLongIntegerRegisterToReal34(calcRegister_t source, real34_t *destination) {
  longInteger_t lgInt;

  convertLongIntegerRegisterToLongInteger(source, lgInt);
  longIntegerToAllocatedString(lgInt, tmpStr3000, TMP_STR_LENGTH);
  longIntegerFree(lgInt);
  stringToReal34(tmpStr3000, destination);
}



void convertLongIntegerRegisterToReal(calcRegister_t source, real_t *destination, realContext_t *ctxt) {
  longInteger_t lgInt;

  convertLongIntegerRegisterToLongInteger(source, lgInt);
  longIntegerToAllocatedString(lgInt, tmpStr3000, TMP_STR_LENGTH);
  longIntegerFree(lgInt);
  stringToReal(tmpStr3000, destination, ctxt);
}



void convertLongIntegerToShortIntegerRegister(longInteger_t lgInt, uint32_t base, calcRegister_t destination) {
  reallocateRegister(destination, dtShortInteger, SHORT_INTEGER_SIZE, base);
  *(REGISTER_SHORT_INTEGER_DATA(destination)) = *(uint64_t *)(lgInt->_mp_d) & shortIntegerMask;
  if(longIntegerIsNegative(lgInt)) {
    *(REGISTER_SHORT_INTEGER_DATA(destination)) = WP34S_intChs(*(REGISTER_SHORT_INTEGER_DATA(destination)));
  }
}



void convertLongIntegerRegisterToShortIntegerRegister(calcRegister_t source, calcRegister_t destination) {
  longInteger_t lgInt;

  convertLongIntegerRegisterToLongInteger(source, lgInt);
  convertLongIntegerToShortIntegerRegister(lgInt, 10, destination);
  longIntegerFree(lgInt);
}



void convertShortIntegerRegisterToReal16Register(calcRegister_t source, calcRegister_t destination) {
  uint64_t value;
  int16_t sign;
  real39_t hiWord, lowWord;

  convertShortIntegerRegisterToUInt64(source, &sign, &value);

  uInt32ToReal(value >> 32, &hiWord);
  uInt32ToReal(value & 0x00000000ffffffff, &lowWord);
  realFMA(&hiWord, const_2p32, &lowWord, &lowWord, &ctxtReal39);

  reallocateRegister(destination, dtReal16, REAL16_SIZE, AM_NONE);
  realToReal16(&lowWord, REGISTER_REAL16_DATA(destination));

  if(sign) {
    real16SetNegativeSign(REGISTER_REAL16_DATA(destination));
  }
}



void convertShortIntegerRegisterToReal34Register(calcRegister_t source, calcRegister_t destination) {
  uint64_t value;
  int16_t sign;
  real34_t lowWord;

  convertShortIntegerRegisterToUInt64(source, &sign, &value);

  uInt32ToReal34(value >> 32, REGISTER_REAL34_DATA(destination));
  uInt32ToReal34(value & 0x00000000ffffffff, &lowWord);
  real34FMA(REGISTER_REAL34_DATA(destination), const_2p32, &lowWord, REGISTER_REAL34_DATA(destination));

  if(sign) {
    real34SetNegativeSign(REGISTER_REAL34_DATA(destination));
  }
}



void convertShortIntegerRegisterToReal(calcRegister_t source, real_t *destination, realContext_t *ctxt) {
  uint64_t value;
  int16_t sign;
  real39_t lowWord;

  convertShortIntegerRegisterToUInt64(source, &sign, &value);

  uInt32ToReal(value >> 32, destination);
  uInt32ToReal(value & 0x00000000ffffffff, &lowWord);
  realFMA(destination, const_2p32, &lowWord, destination, ctxt);

  if(sign) {
    realSetNegativeSign(destination);
  }
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



void convertShortIntegerRegisterToLongInteger(calcRegister_t source, longInteger_t lgInt) {
  uint64_t value;
  int16_t sign;

  convertShortIntegerRegisterToUInt64(source, &sign, &value);

  longIntegerInit(lgInt);
  uIntToLongInteger(value >> 32, lgInt);
  longIntegerLeftShift(lgInt, 32, lgInt);
  longIntegerAddUInt(lgInt, value & 0x00000000ffffffff, lgInt);

  if(sign) {
    longIntegerChangeSign(lgInt);
  }
}



void convertShortIntegerRegisterToLongIntegerRegister(calcRegister_t source, calcRegister_t destination) {
  longInteger_t lgInt;

  convertShortIntegerRegisterToLongInteger(source, lgInt);

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



void convertReal16ToLongInteger(real16_t *real16, longInteger_t lgInt, enum rounding roundingMode) {
  uint8_t bcd[DECDOUBLE_Pmax];
  int32_t sign, exponent;
  //longInteger_t coef;

  real16ToIntegralValue(real16, real16, roundingMode);
  sign = real16GetCoefficient(real16, bcd);
  exponent = real16GetExponent(real16);

  //longIntegerInit(coef);
  uIntToLongInteger(bcd[0], lgInt);

  for(int i=1; i<DECDOUBLE_Pmax; i++) {
    longIntegerMultiplyUInt(lgInt, 10, lgInt);
    longIntegerAddUInt(lgInt, bcd[i], lgInt);
  }

  //longIntegerPowerUIntUInt(10, exponent, coef);
  //longIntegerMultiply(lgInt, coef, lgInt);
  while(exponent > 0) {
    longIntegerMultiplyUInt(lgInt, 10, lgInt);
    exponent--;
  }

  if(sign) {
    longIntegerChangeSign(lgInt);
  }

  convertLongIntegerToLongIntegerRegister(lgInt, REGISTER_X);
  //longIntegerFree(coef);
}



void convertReal16ToLongIntegerRegister(real16_t *real16, calcRegister_t dest, enum rounding roundingMode) {
  longInteger_t lgInt;

  longIntegerInit(lgInt);

  convertReal16ToLongInteger(real16, lgInt, roundingMode);
  convertLongIntegerToLongIntegerRegister(lgInt, dest);

  longIntegerFree(lgInt);
}



void convertReal34ToLongInteger(real34_t *real34, longInteger_t lgInt, enum rounding roundingMode) {
  uint8_t bcd[DECQUAD_Pmax];
  int32_t sign, exponent;
  //longInteger_t coef;

  real34ToIntegralValue(real34, real34, roundingMode);
  sign = real34GetCoefficient(real34, bcd);
  exponent = real34GetExponent(real34);

  //longIntegerInit(coef);
  uIntToLongInteger(bcd[0], lgInt);

  for(int i=1; i<DECQUAD_Pmax; i++) {
    longIntegerMultiplyUInt(lgInt, 10, lgInt);
    longIntegerAddUInt(lgInt, bcd[i], lgInt);
  }

  //longIntegerPowerUIntUInt(10, exponent, coef);
  //longIntegerMultiply(lgInt, coef, lgInt);
  while(exponent > 0) {
    longIntegerMultiplyUInt(lgInt, 10, lgInt);
    exponent--;
  }

  if(sign) {
    longIntegerChangeSign(lgInt);
  }

  convertLongIntegerToLongIntegerRegister(lgInt, REGISTER_X);
  //longIntegerFree(coef);
}



void convertReal34ToLongIntegerRegister(real34_t *real34, calcRegister_t dest, enum rounding roundingMode) {
  longInteger_t lgInt;

  longIntegerInit(lgInt);

  convertReal34ToLongInteger(real34, lgInt, roundingMode);
  convertLongIntegerToLongIntegerRegister(lgInt, dest);

  longIntegerFree(lgInt);
}



void convertReal39ToLongInteger(real_t *real39, longInteger_t lgInt, enum rounding roundingMode) {
  uint8_t bcd[39];
  int32_t sign, exponent;
  //longInteger_t coef;

  realToIntegralValue(real39, real39, roundingMode, &ctxtReal39);
  realGetCoefficient(real39, bcd);
  sign = (realIsNegative(real39) ? 1 : 0);
  exponent = realGetExponent(real39) - real39->digits;

  //longIntegerInit(coef);
  uIntToLongInteger(bcd[0], lgInt);

  for(int i=1; i<real39->digits; i++) {
    longIntegerMultiplyUInt(lgInt, 10, lgInt);
    longIntegerAddUInt(lgInt, bcd[i], lgInt);
  }

  //longIntegerPowerUIntUInt(10, exponent, coef);
  //longIntegerMultiply(lgInt, coef, lgInt);
  while(exponent > 0) {
    longIntegerMultiplyUInt(lgInt, 10, lgInt);
    exponent--;
  }

  if(sign) {
    longIntegerChangeSign(lgInt);
  }

  convertLongIntegerToLongIntegerRegister(lgInt, REGISTER_X);
  //longIntegerFree(coef);
}



void convertRealToLongIntegerRegister(real_t *real39, calcRegister_t dest, enum rounding roundingMode) {
  longInteger_t lgInt;

  longIntegerInit(lgInt);

  convertReal39ToLongInteger(real39, lgInt, roundingMode);
  convertLongIntegerToLongIntegerRegister(lgInt, dest);

  longIntegerFree(lgInt);
}
