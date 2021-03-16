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
 * \file opmod.c
 ***********************************************/

#include "wp43s.h"


TO_QSPI void (* const opModL[NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS][NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS])(uint16_t) = {
// regX |    regY ==>   1                  2                  3           4           5           6           7           8           9                  10
//      V               Long integer       Real34             Complex34   Time        Date        String      Real34 mat  Complex34 m Short integer      Config data
/*  1 Long integer  */ {opModLonILonILonI, opModLonIRealLonI, opModError, opModError, opModError, opModError, opModError, opModError, opModLonIShoILonI, opModError},
/*  2 Real34        */ {opModLonILonIReal, opModLonIRealReal, opModError, opModError, opModError, opModError, opModError, opModError, opModLonIShoIReal, opModError},
/*  3 Complex34     */ {opModError,        opModError,        opModError, opModError, opModError, opModError, opModError, opModError, opModError,        opModError},
/*  4 Time          */ {opModError,        opModError,        opModError, opModError, opModError, opModError, opModError, opModError, opModError,        opModError},
/*  5 Date          */ {opModError,        opModError,        opModError, opModError, opModError, opModError, opModError, opModError, opModError,        opModError},
/*  6 String        */ {opModError,        opModError,        opModError, opModError, opModError, opModError, opModError, opModError, opModError,        opModError},
/*  7 Real34 mat    */ {opModError,        opModError,        opModError, opModError, opModError, opModError, opModError, opModError, opModError,        opModError},
/*  8 Complex34 mat */ {opModError,        opModError,        opModError, opModError, opModError, opModError, opModError, opModError, opModError,        opModError},
/*  9 Short integer */ {opModLonILonIShoI, opModLonIRealShoI, opModError, opModError, opModError, opModError, opModError, opModError, opModLonIShoIShoI, opModError},
/* 10 Config data   */ {opModError,        opModError,        opModError, opModError, opModError, opModError, opModError, opModError, opModError,        opModError}
};
TO_QSPI void (* const opModS[NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS][NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS])(uint16_t) = {
// regX |    regY ==>   1                  2                  3           4           5           6           7           8           9                  10
//      V               Long integer       Real34             Complex34   Time        Date        String      Real34 mat  Complex34 m Short integer      Config data
/*  1 Long integer  */ {opModShoILonILonI, opModShoIRealLonI, opModError, opModError, opModError, opModError, opModError, opModError, opModShoIShoILonI, opModError},
/*  2 Real34        */ {opModShoILonIReal, opModShoIRealReal, opModError, opModError, opModError, opModError, opModError, opModError, opModShoIShoIReal, opModError},
/*  3 Complex34     */ {opModError,        opModError,        opModError, opModError, opModError, opModError, opModError, opModError, opModError,        opModError},
/*  4 Time          */ {opModError,        opModError,        opModError, opModError, opModError, opModError, opModError, opModError, opModError,        opModError},
/*  5 Date          */ {opModError,        opModError,        opModError, opModError, opModError, opModError, opModError, opModError, opModError,        opModError},
/*  6 String        */ {opModError,        opModError,        opModError, opModError, opModError, opModError, opModError, opModError, opModError,        opModError},
/*  7 Real34 mat    */ {opModError,        opModError,        opModError, opModError, opModError, opModError, opModError, opModError, opModError,        opModError},
/*  8 Complex34 mat */ {opModError,        opModError,        opModError, opModError, opModError, opModError, opModError, opModError, opModError,        opModError},
/*  9 Short integer */ {opModShoILonIShoI, opModShoIRealShoI, opModError, opModError, opModError, opModError, opModError, opModError, opModShoIShoIShoI, opModError},
/* 10 Config data   */ {opModError,        opModError,        opModError, opModError, opModError, opModError, opModError, opModError, opModError,        opModError}
};
TO_QSPI void (* const opModR[NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS][NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS])(uint16_t) = {
// regX |    regY ==>   1                  2                  3           4           5           6           7           8           9                  10
//      V               Long integer       Real34             Complex34   Time        Date        String      Real34 mat  Complex34 m Short integer      Config data
/*  1 Long integer  */ {opModRealLonILonI, opModRealRealLonI, opModError, opModError, opModError, opModError, opModError, opModError, opModRealShoILonI, opModError},
/*  2 Real34        */ {opModRealLonIReal, opModRealRealReal, opModError, opModError, opModError, opModError, opModError, opModError, opModRealShoIReal, opModError},
/*  3 Complex34     */ {opModError,        opModError,        opModError, opModError, opModError, opModError, opModError, opModError, opModError,        opModError},
/*  4 Time          */ {opModError,        opModError,        opModError, opModError, opModError, opModError, opModError, opModError, opModError,        opModError},
/*  5 Date          */ {opModError,        opModError,        opModError, opModError, opModError, opModError, opModError, opModError, opModError,        opModError},
/*  6 String        */ {opModError,        opModError,        opModError, opModError, opModError, opModError, opModError, opModError, opModError,        opModError},
/*  7 Real34 mat    */ {opModError,        opModError,        opModError, opModError, opModError, opModError, opModError, opModError, opModError,        opModError},
/*  8 Complex34 mat */ {opModError,        opModError,        opModError, opModError, opModError, opModError, opModError, opModError, opModError,        opModError},
/*  9 Short integer */ {opModRealLonIShoI, opModRealRealShoI, opModError, opModError, opModError, opModError, opModError, opModError, opModRealShoIShoI, opModError},
/* 10 Config data   */ {opModError,        opModError,        opModError, opModError, opModError, opModError, opModError, opModError, opModError,        opModError}
};



void opModError(uint16_t mode) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot RMD %s %s %s", getRegisterDataTypeName(REGISTER_Z, true, false), mode == OPMOD_POWER ? "^" : "*", getRegisterDataTypeName(REGISTER_Y, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "by %s", getRegisterDataTypeName(REGISTER_X, true, false));
    moreInfoOnError("In function fnOpMod:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
  #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
}

void opModOutOfDomain(uint16_t unusedButMandatoryParameter) {
  displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "At least one of the arguments is out of the domain or noninteger: Z > 0, Y > 0, X > 1");
    moreInfoOnError("In function fnOpMod:", errorMessage, NULL, NULL);
  #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
}


void fnMulMod(uint16_t unusedButMandatoryParameter) {
  fnOpMod(OPMOD_MULTIPLY);
}

void fnExpMod(uint16_t unusedButMandatoryParameter) {
  fnOpMod(OPMOD_POWER);
}

void fnOpMod(uint16_t mode) {
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  switch(getRegisterDataType(REGISTER_Z)) {
    case dtLongInteger:
      opModL[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)](mode);
      break;
    case dtShortInteger:
      opModS[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)](mode);
      break;
    case dtReal34:
      opModR[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)](mode);
      break;
    default:
      opModError(mode);
  }
  adjustResult(REGISTER_X, true, false, REGISTER_X, REGISTER_Y, REGISTER_Z);
  if(lastErrorCode == ERROR_NONE) {
    fnDropY(NOPARAM);
  }
}


static bool_t checkReal(calcRegister_t regist) {
  real_t r;

  real34ToReal(REGISTER_REAL34_DATA(regist), &r);

  if(realIsAnInteger(&r) && (getRegisterAngularMode(regist) == amNone)) {
    convertRealToLongIntegerRegister(&r, regist, DEC_ROUND_FLOOR);
    return true;
  }
  else {
    opModOutOfDomain(NOPARAM);
    return false;
  }
}



// Adapted from WP34s integer-mode function to long integer
void longInteger_mulmod(const longInteger_t a, const longInteger_t b, const longInteger_t c, longInteger_t res) {
  longInteger_t x, y, bb;

  longIntegerInit(x);
  longIntegerInit(y);
  longIntegerInit(bb);

  longIntegerSetZero(x);
  longIntegerModulo(a, c, y);
  longIntegerCopy(b, bb);

  while(longIntegerCompareInt(bb, 0) > 0) {
    if(longIntegerIsOdd(bb)) {
      longIntegerAdd(x, y, x);
      longIntegerModulo(x, c, x);
    }
    longIntegerAdd(y, y, y);
    longIntegerModulo(y, c, y);
    longIntegerDivideUInt(bb, 2u, bb);
  }
  longIntegerModulo(x, c, res);

  longIntegerFree(x);
  longIntegerFree(y);
  longIntegerFree(bb);
}
void longInteger_expmod(const longInteger_t a, const longInteger_t b, const longInteger_t c, longInteger_t res) {
  longInteger_t x, y, bb;

  longIntegerInit(x);
  longIntegerInit(y);
  longIntegerInit(bb);

  intToLongInteger(1, x);
  longIntegerModulo(a, c, y);
  longIntegerCopy(b, bb);

  while(longIntegerCompareInt(bb, 0) > 0) {
    if(longIntegerIsOdd(bb)) longInteger_mulmod(x, y, c, x);
    longInteger_mulmod(y, y, c, y);
    longIntegerDivideUInt(bb, 2u, bb);
  }
  longIntegerModulo(x, c, res);

  longIntegerFree(x);
  longIntegerFree(y);
  longIntegerFree(bb);
}



void opModLonILonILonI(uint16_t mode) {
  longInteger_t x, y, z;

  convertLongIntegerRegisterToLongInteger(REGISTER_X, x);
  convertLongIntegerRegisterToLongInteger(REGISTER_Y, y);
  convertLongIntegerRegisterToLongInteger(REGISTER_Z, z);

  if((longIntegerCompareInt(x, 1) > 0) && longIntegerIsPositive(y) && longIntegerIsPositive(z)) {
    if(mode == OPMOD_POWER) longInteger_expmod(z, y, x, x);
    else                    longInteger_mulmod(z, y, x, x);
    convertLongIntegerToLongIntegerRegister(x, REGISTER_X);
  }
  else {
    opModOutOfDomain(mode);
  }

  longIntegerFree(x);
  longIntegerFree(y);
  longIntegerFree(z);
}
void opModLonILonIShoI(uint16_t mode) {
  convertShortIntegerRegisterToLongIntegerRegister(REGISTER_X, REGISTER_X);
  opModLonILonILonI(mode);
}
void opModLonILonIReal(uint16_t mode) {
  if(checkReal(REGISTER_X)) {
    opModLonILonILonI(mode);
    convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
  }
}

void opModLonIShoILonI(uint16_t mode) {
  convertShortIntegerRegisterToLongIntegerRegister(REGISTER_Y, REGISTER_Y);
  opModLonILonILonI(mode);
}
void opModLonIShoIShoI(uint16_t mode) {
  convertShortIntegerRegisterToLongIntegerRegister(REGISTER_X, REGISTER_X);
  convertShortIntegerRegisterToLongIntegerRegister(REGISTER_Y, REGISTER_Y);
  opModLonILonILonI(mode);
}
void opModLonIShoIReal(uint16_t mode) {
  convertShortIntegerRegisterToLongIntegerRegister(REGISTER_Y, REGISTER_Y);
  if(checkReal(REGISTER_X)) {
    opModLonILonILonI(mode);
    convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
  }
}

void opModLonIRealLonI(uint16_t mode) {
  if(checkReal(REGISTER_Y)) {
    opModLonILonILonI(mode);
    convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
  }
}
void opModLonIRealShoI(uint16_t mode) {
  convertShortIntegerRegisterToLongIntegerRegister(REGISTER_X, REGISTER_X);
  if(checkReal(REGISTER_Y)) {
    opModLonILonILonI(mode);
    convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
  }
}
void opModLonIRealReal(uint16_t mode) {
  if(checkReal(REGISTER_Y) && checkReal(REGISTER_X)) {
    opModLonILonILonI(mode);
    convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
  }
}



void opModShoILonILonI(uint16_t mode) {
  convertShortIntegerRegisterToLongIntegerRegister(REGISTER_Z, REGISTER_Z);
  opModLonILonILonI(mode);
}
void opModShoILonIShoI(uint16_t mode) {
  const uint32_t base = getRegisterShortIntegerBase(REGISTER_Z);
  convertShortIntegerRegisterToLongIntegerRegister(REGISTER_X, REGISTER_X);
  convertShortIntegerRegisterToLongIntegerRegister(REGISTER_Z, REGISTER_Z);
  opModLonILonILonI(mode);
  if(mode == OPMOD_MULTIPLY) {
    convertLongIntegerRegisterToShortIntegerRegister(REGISTER_X, REGISTER_X);
    setRegisterTag(REGISTER_X, base);
  }
}
void opModShoILonIReal(uint16_t mode) {
  convertShortIntegerRegisterToLongIntegerRegister(REGISTER_Z, REGISTER_Z);
  if(checkReal(REGISTER_X)) {
    opModLonILonILonI(mode);
    convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
  }
}

void opModShoIShoILonI(uint16_t mode) {
  convertShortIntegerRegisterToLongIntegerRegister(REGISTER_Y, REGISTER_Y);
  convertShortIntegerRegisterToLongIntegerRegister(REGISTER_Z, REGISTER_Z);
  opModLonILonILonI(mode);
}
void opModShoIShoIShoI(uint16_t mode) {
  uint64_t x, y, z;
  int16_t sx, sy, sz;

  convertShortIntegerRegisterToUInt64(REGISTER_X, &sx, &x);
  convertShortIntegerRegisterToUInt64(REGISTER_Y, &sy, &y);
  convertShortIntegerRegisterToUInt64(REGISTER_Z, &sz, &z);

  if((!sx) && (x > 1) && (!sy) && (!sz)) {
    if(mode == OPMOD_POWER) x = WP34S_expmod(z, y, x);
    else                    x = WP34S_mulmod(z, y, x);
    convertUInt64ToShortIntegerRegister(0, x, getRegisterShortIntegerBase(REGISTER_Z), REGISTER_X);
  }
  else {
    opModOutOfDomain(mode);
  }
}
void opModShoIShoIReal(uint16_t mode) {
  convertShortIntegerRegisterToLongIntegerRegister(REGISTER_Y, REGISTER_Y);
  convertShortIntegerRegisterToLongIntegerRegister(REGISTER_Z, REGISTER_Z);
  if(checkReal(REGISTER_X)) {
    opModLonILonILonI(mode);
    convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
  }
}

void opModShoIRealLonI(uint16_t mode) {
  convertShortIntegerRegisterToLongIntegerRegister(REGISTER_Z, REGISTER_Z);
  if(checkReal(REGISTER_Y)) {
    opModLonILonILonI(mode);
    convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
  }
}
void opModShoIRealShoI(uint16_t mode) {
  convertShortIntegerRegisterToLongIntegerRegister(REGISTER_X, REGISTER_X);
  convertShortIntegerRegisterToLongIntegerRegister(REGISTER_Z, REGISTER_Z);
  if(checkReal(REGISTER_Y)) {
    opModLonILonILonI(mode);
    convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
  }
}
void opModShoIRealReal(uint16_t mode) {
  convertShortIntegerRegisterToLongIntegerRegister(REGISTER_Z, REGISTER_Z);
  if(checkReal(REGISTER_Y) && checkReal(REGISTER_X)) {
    opModLonILonILonI(mode);
    convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
  }
}



void opModRealLonILonI(uint16_t mode) {
  if(checkReal(REGISTER_Z)) {
    opModLonILonILonI(mode);
    convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
  }
}
void opModRealLonIShoI(uint16_t mode) {
  convertShortIntegerRegisterToLongIntegerRegister(REGISTER_X, REGISTER_X);
  if(checkReal(REGISTER_Z)) {
    opModLonILonILonI(mode);
    convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
  }
}
void opModRealLonIReal(uint16_t mode) {
  if(checkReal(REGISTER_Z) && checkReal(REGISTER_X)) {
    opModLonILonILonI(mode);
    convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
  }
}

void opModRealShoILonI(uint16_t mode) {
  convertShortIntegerRegisterToLongIntegerRegister(REGISTER_Y, REGISTER_Y);
  if(checkReal(REGISTER_Z)) {
    opModLonILonILonI(mode);
    convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
  }
}
void opModRealShoIShoI(uint16_t mode) {
  convertShortIntegerRegisterToLongIntegerRegister(REGISTER_X, REGISTER_X);
  convertShortIntegerRegisterToLongIntegerRegister(REGISTER_Y, REGISTER_Y);
  if(checkReal(REGISTER_Z)) {
    opModLonILonILonI(mode);
    convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
  }
}
void opModRealShoIReal(uint16_t mode) {
  convertShortIntegerRegisterToLongIntegerRegister(REGISTER_Y, REGISTER_Y);
  if(checkReal(REGISTER_Z) && checkReal(REGISTER_X)) {
    opModLonILonILonI(mode);
    convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
  }
}

void opModRealRealLonI(uint16_t mode) {
  if(checkReal(REGISTER_Z) && checkReal(REGISTER_Y)) {
    opModLonILonILonI(mode);
    convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
  }
}
void opModRealRealShoI(uint16_t mode) {
  convertShortIntegerRegisterToLongIntegerRegister(REGISTER_X, REGISTER_X);
  if(checkReal(REGISTER_Z) && checkReal(REGISTER_Y)) {
    opModLonILonILonI(mode);
    convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
  }
}
void opModRealRealReal(uint16_t mode) {
  if(checkReal(REGISTER_Z) && checkReal(REGISTER_Y) && checkReal(REGISTER_X)) {
    opModLonILonILonI(mode);
    convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
  }
}
