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
 * \file compare.c
 ***********************************************/

#include "mathematics/compare.h"

#include "constantPointers.h"
#include "debug.h"
#include "error.h"
#include "items.h"
#include "mathematics/comparisonReals.h"
#include "mathematics/round.h"
#include "mathematics/wp34s.h"
#include "registers.h"
#include "registerValueConversions.h"
#include "sort.h"
#include "stack.h"

#include "wp43s.h"



TO_QSPI void (* const cmpFunc[NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS][NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS])(calcRegister_t reg1, calcRegister_t reg2, int8_t *result) = {
// reg1 |    reg2 ==>    1                    2                    3          4                    5                    6                    7              8               9                    10
//      V                Long integer         Real34               Complex34  Time                 Date                 String               Real34 mat     Complex34 mat   Short integer        Config data
/*  1 Long integer  */ { registerCmpLonILonI, registerCmpLonIReal, NULL,      NULL,                NULL,                NULL,                NULL,          NULL,           registerCmpLonIShoI, NULL},
/*  2 Real34        */ { registerCmpRealLonI, registerCmpRealReal, NULL,      NULL,                NULL,                NULL,                NULL,          NULL,           registerCmpRealShoI, NULL},
/*  3 Complex34     */ { NULL,                NULL,                NULL,      NULL,                NULL,                NULL,                NULL,          NULL,           NULL,                NULL},
/*  4 Time          */ { NULL,                NULL,                NULL,      registerCmpTimeTime, NULL,                NULL,                NULL,          NULL,           NULL,                NULL},
/*  5 Date          */ { NULL,                NULL,                NULL,      NULL,                registerCmpDateDate, NULL,                NULL,          NULL,           NULL,                NULL},
/*  6 String        */ { NULL,                NULL,                NULL,      NULL,                NULL,                registerCmpStriStri, NULL,          NULL,           NULL,                NULL},
/*  7 Real34 mat    */ { NULL,                NULL,                NULL,      NULL,                NULL,                NULL,                NULL,          NULL,           NULL,                NULL},
/*  8 Complex34 mat */ { NULL,                NULL,                NULL,      NULL,                NULL,                NULL,                NULL,          NULL,           NULL,                NULL},
/*  9 Short integer */ { registerCmpShoILonI, registerCmpShoIReal, NULL,      NULL,                NULL,                NULL,                NULL,          NULL,           registerCmpShoIShoI, NULL},
/* 10 Config data   */ { NULL,                NULL,                NULL,      NULL,                NULL,                NULL,                NULL,          NULL,           NULL,                NULL}
};



bool_t registerCmp(calcRegister_t regist1, calcRegister_t regist2, int8_t *result) {
  void (*func)(calcRegister_t, calcRegister_t, int8_t*) = cmpFunc[getRegisterDataType(regist1)][getRegisterDataType(regist2)];

  if(func == NULL)
    return false;

  func(regist1, regist2, result);

  return true;
}



void registerCmpError(calcRegister_t regist1, calcRegister_t regist2) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot get compare: %s", getRegisterDataTypeName(regist1, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "and %s", getRegisterDataTypeName(regist2, true, false));
    moreInfoOnError("In function registerCmp:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
  #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
}



void registerCmpLonILonI(calcRegister_t regist1, calcRegister_t regist2, int8_t *result) {
  // regist1 = Long Integer, regist2 = Long Integer

  longInteger_t r1, r2;

  convertLongIntegerRegisterToLongInteger(regist1, r1);
  convertLongIntegerRegisterToLongInteger(regist2, r2);

  int8_t value = longIntegerCompare(r1, r2);

  longIntegerFree(r1);
  longIntegerFree(r2);

  *result = value;
}



void registerCmpLonIShoI(calcRegister_t regist1, calcRegister_t regist2, int8_t *result) {
  // regist1 = Long Integer, regist2 = Short Integer

  longInteger_t r1, r2;

  convertLongIntegerRegisterToLongInteger(regist1, r1);
  convertShortIntegerRegisterToLongInteger(regist2, r2);

  int8_t value = longIntegerCompare(r1, r2);

  longIntegerFree(r1);
  longIntegerFree(r2);

  *result = value;
}



void registerCmpLonIReal(calcRegister_t regist1, calcRegister_t regist2, int8_t *result) {
  // regist1 = Long Integer, regist2 = Real

  real_t r1, r2;

  convertLongIntegerRegisterToReal(regist1, &r1, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(regist2), &r2);

  if(realCompareEqual(&r1, &r2))
    *result = 0;
  else
    *result = realCompareGreaterThan(&r1, &r2) ? 1 : -1;
}



void registerCmpTimeTime(calcRegister_t regist1, calcRegister_t regist2, int8_t *result) {
  registerCmpRealReal(regist1, regist2, result);
}



void registerCmpDateDate(calcRegister_t regist1, calcRegister_t regist2, int8_t *result) {
  registerCmpRealReal(regist1, regist2, result);
}



void registerCmpStriStri(calcRegister_t regist1, calcRegister_t regist2, int8_t *result) {
 *result = compareString(REGISTER_STRING_DATA(regist1), REGISTER_STRING_DATA(regist2), CMP_EXTENSIVE);
}

void registerCmpShoILonI(calcRegister_t regist1, calcRegister_t regist2, int8_t *result) {
  // regist1 = Short Integer, regist2 = Long Integer

  longInteger_t r1, r2;

  convertShortIntegerRegisterToLongInteger(regist1, r1);
  convertLongIntegerRegisterToLongInteger(regist2, r2);

  int8_t value = longIntegerCompare(r1, r2);

  longIntegerFree(r1);
  longIntegerFree(r2);

  *result = value;
}



void registerCmpShoIShoI(calcRegister_t regist1, calcRegister_t regist2, int8_t *result) {
  // regist1 = Short Integer, regist2 = Short Integer

  longInteger_t r1, r2;

  convertShortIntegerRegisterToLongInteger(regist1, r1);
  convertShortIntegerRegisterToLongInteger(regist2, r2);

  int8_t value = longIntegerCompare(r1, r2);

  longIntegerFree(r1);
  longIntegerFree(r2);

  *result = value;
}



void registerCmpShoIReal(calcRegister_t regist1, calcRegister_t regist2, int8_t *result) {
  // regist1 = Short Integer, regist2 = Real

  real_t r1, r2;

  convertShortIntegerRegisterToReal(regist1, &r1, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(regist2), &r2);

  if(realCompareEqual(&r1, &r2))
    *result = 0;
  else
    *result = realCompareGreaterThan(&r1, &r2) ? 1 : -1;
}



void registerCmpRealLonI(calcRegister_t regist1, calcRegister_t regist2, int8_t *result) {
  // regist1 = Real, regist2 = Long Integer

  real_t r1, r2;

  real34ToReal(REGISTER_REAL34_DATA(regist1), &r1);
  convertLongIntegerRegisterToReal(regist2, &r2, &ctxtReal39);

  if(realCompareEqual(&r1, &r2))
    *result = 0;
  else
    *result = realCompareGreaterThan(&r1, &r2) ? 1 : -1;
}



void registerCmpRealShoI(calcRegister_t regist1, calcRegister_t regist2, int8_t *result) {
  // regist1 = Real, regist2 = Short Integer

  real_t r1, r2;

  real34ToReal(REGISTER_REAL34_DATA(regist1), &r1);
  convertShortIntegerRegisterToReal(regist2, &r2, &ctxtReal39);

  if(realCompareEqual(&r1, &r2))
    *result = 0;
  else
    *result = realCompareGreaterThan(&r1, &r2) ? 1 : -1;
}



void registerCmpRealReal(calcRegister_t regist1, calcRegister_t regist2, int8_t *result) {
  // regist1 = Real, regist2 = Real

  real_t r1, r2;

  real34ToReal(REGISTER_REAL34_DATA(regist1), &r1);
  real34ToReal(REGISTER_REAL34_DATA(regist2), &r2);

  if(realCompareEqual(&r1, &r2))
    *result = 0;
  else
    *result = realCompareGreaterThan(&r1, &r2) ? 1 : -1;
}



void registerMax(calcRegister_t regist1, calcRegister_t regist2, calcRegister_t dest) {
  int8_t result = 0;

  if(!registerCmp(regist1, regist2, &result))
    registerCmpError(regist1, regist2);
  else if(result != 0) {
    copySourceRegisterToDestRegister(result>0 ? regist1 : regist2, dest);
    if(getRegisterDataType(dest) == dtShortInteger) {
      *(REGISTER_SHORT_INTEGER_DATA(dest)) &= shortIntegerMask;
    }
  }
}



void registerMin(calcRegister_t regist1, calcRegister_t regist2, calcRegister_t dest) {
  int8_t result = 0;

  if(!registerCmp(regist1, regist2, &result))
    registerCmpError(regist1, regist2);
  else if(result != 0) {
    copySourceRegisterToDestRegister(result>0 ? regist2 : regist1, dest);
    if(getRegisterDataType(dest) == dtShortInteger) {
      *(REGISTER_SHORT_INTEGER_DATA(dest)) &= shortIntegerMask;
    }
  }
}



void comparisonTypeError(uint16_t regist) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot compare %s and %s", getRegisterDataTypeName(REGISTER_X, true, false), getRegisterDataTypeName(regist, true, false));
    moreInfoOnError("In function comparisonTypeError:", errorMessage, NULL, NULL);
  #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  temporaryInformation = TI_FALSE;
}



#define COMPARE_MODE_LESS_THAN     1
#define COMPARE_MODE_EQUAL         2
#define COMPARE_MODE_LESS_EQUAL    3
#define COMPARE_MODE_GREATER_THAN  4
#define COMPARE_MODE_NOT_EQUAL     5
#define COMPARE_MODE_GREATER_EQUAL 6

static bool_t isEqualRealComplex(calcRegister_t registC, calcRegister_t registR) {
  int8_t result;

  if(!real34IsZero(REGISTER_IMAG34_DATA(registC))) {
    return false;
  }
  else if(registR == TEMP_REGISTER_1) { // real const 0 or 1
    return real34CompareEqual(REGISTER_REAL34_DATA(registC), REGISTER_REAL34_DATA(TEMP_REGISTER_1));
  }
  else {
    reallocateRegister(TEMP_REGISTER_1, dtReal34, REAL34_SIZE, amNone);
    real34Copy(REGISTER_REAL34_DATA(registC), REGISTER_REAL34_DATA(TEMP_REGISTER_1));
    registerCmp(TEMP_REGISTER_1, registR, &result);
    return (result == 0);
  }
}

static void compareRegisters(uint16_t regist, uint8_t mode) {
  int8_t result;

  if((regist < FIRST_LOCAL_REGISTER + currentNumberOfLocalRegisters) || (regist == TEMP_REGISTER_1)) {
    // Compare matrices
    if((mode == COMPARE_MODE_EQUAL || mode == COMPARE_MODE_NOT_EQUAL) && (getRegisterDataType(REGISTER_X) == dtReal34Matrix || getRegisterDataType(REGISTER_X) == dtComplex34Matrix) && (getRegisterDataType(regist) == dtReal34Matrix || getRegisterDataType(regist) == dtComplex34Matrix)) {
      fnToBeCoded();
    }

    // Compare complex numbers
    else if((mode == COMPARE_MODE_EQUAL || mode == COMPARE_MODE_NOT_EQUAL) && (getRegisterDataType(REGISTER_X) == dtComplex34 || getRegisterDataType(regist) == dtComplex34)) {
      if(getRegisterDataType(REGISTER_X) == getRegisterDataType(regist)) { // == dtComplex34
        temporaryInformation = (real34CompareEqual(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(regist)) && real34CompareEqual(REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(regist))) ? TI_TRUE : TI_FALSE;
      }
      else if(getRegisterDataType(REGISTER_X) == dtComplex34) {
        temporaryInformation = isEqualRealComplex(REGISTER_X, regist) ? TI_TRUE : TI_FALSE;
      }
      else {
        temporaryInformation = isEqualRealComplex(regist, REGISTER_X) ? TI_TRUE : TI_FALSE;
      }
      if(mode == COMPARE_MODE_NOT_EQUAL) {
        if(temporaryInformation == TI_TRUE) temporaryInformation = TI_FALSE;
        else                                temporaryInformation = TI_TRUE;
      }
    }

    // Other comparison
    else if(registerCmp(REGISTER_X, regist, &result)) {
      if(result < 0) {
        temporaryInformation = (mode & COMPARE_MODE_LESS_THAN) ? TI_TRUE : TI_FALSE;
      }
      else if(result > 0) {
        temporaryInformation = (mode & COMPARE_MODE_GREATER_THAN) ? TI_TRUE : TI_FALSE;
      }
      else {
        temporaryInformation = (mode & COMPARE_MODE_EQUAL) ? TI_TRUE : TI_FALSE;
      }
    }
    else {
      comparisonTypeError(regist);
    }
  }
  #ifdef PC_BUILD
    else {
      sprintf(errorMessage, "local register .%02d", regist - FIRST_LOCAL_REGISTER);
      moreInfoOnError("In function compareRegisters:", errorMessage, "is not defined!", NULL);
    }
  #endif // PC_BUILD
}


void fnXLessThan(uint16_t regist) {
  compareRegisters(regist, COMPARE_MODE_LESS_THAN);
}

void fnXLessEqual(uint16_t regist) {
  compareRegisters(regist, COMPARE_MODE_LESS_EQUAL);
}

void fnXGreaterThan(uint16_t regist) {
  compareRegisters(regist, COMPARE_MODE_GREATER_THAN);
}

void fnXGreaterEqual(uint16_t regist) {
  compareRegisters(regist, COMPARE_MODE_GREATER_EQUAL);
}

void fnXEqualsTo(uint16_t regist) {
  compareRegisters(regist, COMPARE_MODE_EQUAL);
}

void fnXNotEqual(uint16_t regist) {
  compareRegisters(regist, COMPARE_MODE_NOT_EQUAL);
}

static void almostEqualMatrix(uint16_t regist) {
  fnToBeCoded();
}

static void almostEqualScalar(uint16_t regist) {
  real34_t val1r, val1i, val2r, val2i;

  real34Copy(const34_0, &val1i);
  real34Copy(const34_0, &val2i);

  if(getRegisterDataType(REGISTER_X) == dtComplex34) {
    real34Copy(REGISTER_IMAG34_DATA(REGISTER_X), &val1i);
  }
  real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &val1r);
  if(getRegisterDataType(regist) == dtComplex34) {
    real34Copy(REGISTER_IMAG34_DATA(regist), &val2i);
  }
  real34Copy(REGISTER_REAL34_DATA(regist), &val2r);

  switch(getRegisterDataType(REGISTER_X)) {
    case dtComplex34:       roundCplx(); break;
    case dtReal34:          roundReal(); break;
    default: /* dtTime */   roundTime(); break;
  }
  if(regist != TEMP_REGISTER_1) {
    fnSwapX(regist);
    switch(getRegisterDataType(REGISTER_X)) {
      case dtComplex34:       roundCplx(); break;
      case dtReal34:          roundReal(); break;
      default: /* dtTime */   roundTime(); break;
    }
    fnSwapX(regist);
  }

  compareRegisters(regist, COMPARE_MODE_EQUAL);

  if (getRegisterDataType(REGISTER_X) == dtComplex34) {
    real34Copy(&val1i, REGISTER_IMAG34_DATA(REGISTER_X));
  }
  real34Copy(&val1r, REGISTER_REAL34_DATA(REGISTER_X));
  if(getRegisterDataType(regist) == dtComplex34) {
    real34Copy(&val2i, REGISTER_IMAG34_DATA(regist));
  }
  real34Copy(&val2r, REGISTER_REAL34_DATA(regist));
}

void fnXAlmostEqual(uint16_t regist) {
  if(getRegisterDataType(REGISTER_X) == dtReal34Matrix || getRegisterDataType(REGISTER_X) == dtComplex34Matrix) {
    if(getRegisterDataType(regist) == dtReal34Matrix || getRegisterDataType(regist) == dtComplex34Matrix) {
      almostEqualMatrix(regist);
    }
    else {
      comparisonTypeError(regist);
    }
  }
  else if(getRegisterDataType(REGISTER_X) == dtReal34 || getRegisterDataType(REGISTER_X) == dtComplex34 || getRegisterDataType(REGISTER_X) == dtTime) {
    if(getRegisterDataType(regist) == dtReal34 || getRegisterDataType(regist) == dtComplex34 || getRegisterDataType(regist) == dtTime) {
      almostEqualScalar(regist);
    }
    else {
      comparisonTypeError(regist);
    }
  }
  else {
    comparisonTypeError(regist);
  }
}


#undef COMPARE_MODE_LESS_THAN
#undef COMPARE_MODE_EQUAL
#undef COMPARE_MODE_LESS_EQUAL
#undef COMPARE_MODE_GREATER_THAN
#undef COMPARE_MODE_NOT_EQUAL
#undef COMPARE_MODE_GREATER_EQUAL



void fnIsConverged(uint16_t mode) {
  real_t x, y, tol;
  if(getRegisterDataType(REGISTER_X) == dtReal34 && getRegisterDataType(REGISTER_Y) == dtReal34) {
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
    realCopy(const_1, &tol);
    tol.exponent -= ((mode & 0x03) == 0) ? 14 : ((mode & 0x03) == 1) ? 24 : 32;
    if(realIsSpecial(&x) || realIsSpecial(&y)) {
      temporaryInformation = (mode & 0x10) ? TI_FALSE : TI_TRUE;
    }
    else if(mode & 0x04) {
      temporaryInformation = WP34S_AbsoluteError(&x, &y, &tol, &ctxtReal39) ? TI_TRUE : TI_FALSE;
    }
    else {
      temporaryInformation = WP34S_RelativeError(&x, &y, &tol, &ctxtReal39) ? TI_TRUE : TI_FALSE;
    }
  }
  else {
    comparisonTypeError(REGISTER_Y);
  }
}
