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
 * \file gcd.c
 ***********************************************/

#include "wp43s.h"



void (* const gcd[12][12])(void) = {
// regX |    regY ==>    1            2         3          4         5         6         7         8           9            10            11        12
//      V                Big integer  real16    complex16  angle34   Time      Date      String    real16 mat  complex16 m  Small integer real34    complex34
/*  1 Big integer    */ {gcdBigIBigI, gcdError, gcdError,  gcdError, gcdError, gcdError, gcdError, gcdError,   gcdError,    gcdSmaIBigI,  gcdError, gcdError},
/*  2 real16         */ {gcdError,    gcdError, gcdError,  gcdError, gcdError, gcdError, gcdError, gcdError,   gcdError,    gcdError,     gcdError, gcdError},
/*  3 complex16      */ {gcdError,    gcdError, gcdError,  gcdError, gcdError, gcdError, gcdError, gcdError,   gcdError,    gcdError,     gcdError, gcdError},
/*  4 angle34        */ {gcdError,    gcdError, gcdError,  gcdError, gcdError, gcdError, gcdError, gcdError,   gcdError,    gcdError,     gcdError, gcdError},
/*  5 Time           */ {gcdError,    gcdError, gcdError,  gcdError, gcdError, gcdError, gcdError, gcdError,   gcdError,    gcdError,     gcdError, gcdError},
/*  6 Date           */ {gcdError,    gcdError, gcdError,  gcdError, gcdError, gcdError, gcdError, gcdError,   gcdError,    gcdError,     gcdError, gcdError},
/*  7 String         */ {gcdError,    gcdError, gcdError,  gcdError, gcdError, gcdError, gcdError, gcdError,   gcdError,    gcdError,     gcdError, gcdError},
/*  8 real16 mat     */ {gcdError,    gcdError, gcdError,  gcdError, gcdError, gcdError, gcdError, gcdError,   gcdError,    gcdError,     gcdError, gcdError},
/*  9 complex16 mat  */ {gcdError,    gcdError, gcdError,  gcdError, gcdError, gcdError, gcdError, gcdError,   gcdError,    gcdError,     gcdError, gcdError},
/* 10 Small integer  */ {gcdBigISmaI, gcdError, gcdError,  gcdError, gcdError, gcdError, gcdError, gcdError,   gcdError,    gcdSmaISmaI,  gcdError, gcdError},
/* 11 real34         */ {gcdError,    gcdError, gcdError,  gcdError, gcdError, gcdError, gcdError, gcdError,   gcdError,    gcdError,     gcdError, gcdError},
/* 12 complex34      */ {gcdError,    gcdError, gcdError,  gcdError, gcdError, gcdError, gcdError, gcdError,   gcdError,    gcdError,     gcdError, gcdError}
};



/********************************************//**
 * \brief Data type error in gcd
 *
 * \param void
 * \return void
 ***********************************************/
void gcdError(void) {
  displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate gcd (%s, %s)", getRegisterDataTypeName(REGISTER_Y, true, false), getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnGcd:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and GDC(regY, regX) ==> regX
 * Drops Y, enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnGcd(uint16_t unusedParamButMandatory) {
  if(gcd[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)] != gcdError) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    result = REGISTER_X;
    opY    = allocateTemporaryRegister();
    opX    = allocateTemporaryRegister();
    copySourceRegisterToDestRegister(REGISTER_Y, opY);
    copySourceRegisterToDestRegister(REGISTER_X, opX);

    gcd[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();
    freeTemporaryRegister(opY);
    freeTemporaryRegister(opX);

    if(lastErrorCode == 0) {
      fnDropY(NOPARAM);
    }
    else {
      restoreStack();
    }

    refreshStack();
  }
  else {
    gcdError();
  }
}



void gcdBigIBigI(void) {
  bigInteger_t iOp1, iOp2;

  convertBigIntegerRegisterToBigInteger(opY, &iOp1);
  iOp1.sign = 0;
  convertBigIntegerRegisterToBigInteger(opX, &iOp2);
  iOp2.sign = 0;

  bigIntegerGcd(&iOp1, &iOp2, &iOp1);

  convertBigIntegerToBigIntegerRegister(&iOp1, result);
}



void gcdBigISmaI(void) {
  bigInteger_t iOp1, iOp2;

  convertBigIntegerRegisterToBigInteger(opY, &iOp1);
  iOp1.sign = 0;
  convertSmallIntegerRegisterBigIntegerRegister(opX, opX);
  convertBigIntegerRegisterToBigInteger(opX, &iOp2);
  iOp2.sign = 0;

  bigIntegerGcd(&iOp1, &iOp2, &iOp1);

  convertBigIntegerToBigIntegerRegister(&iOp1, result);
}



void gcdSmaIBigI(void) {
  bigInteger_t iOp1, iOp2;

  convertBigIntegerRegisterToBigInteger(opX, &iOp1);
  iOp1.sign = 0;
  convertSmallIntegerRegisterBigIntegerRegister(opY, opY);
  convertBigIntegerRegisterToBigInteger(opY, &iOp2);
  iOp2.sign = 0;

  bigIntegerGcd(&iOp1, &iOp2, &iOp1);

  convertBigIntegerToBigIntegerRegister(&iOp1, result);
}



void gcdSmaISmaI(void) {
  *(REGISTER_SMALL_INTEGER_DATA(result)) = WP34S_intGCD(*(REGISTER_SMALL_INTEGER_DATA(opY)), *(REGISTER_SMALL_INTEGER_DATA(opX)));
}
