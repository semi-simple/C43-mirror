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



void (* const gcd[13][13])(void) = {
// regX |    regY ==>   1            2         3         4         5         6         7         8          9           10            11        12        13
//      V               Long integer Real16    Complex16 Angle16   Time      Date      String    Real16 mat Complex16 m Short integer Real34    Complex34 Angle34
/*  1 Long integer  */ {gcdLonILonI, gcdError, gcdError, gcdError, gcdError, gcdError, gcdError, gcdError,  gcdError,   gcdShoILonI,  gcdError, gcdError, gcdError},
/*  2 Real16        */ {gcdError,    gcdError, gcdError, gcdError, gcdError, gcdError, gcdError, gcdError,  gcdError,   gcdError,     gcdError, gcdError, gcdError},
/*  3 Complex16     */ {gcdError,    gcdError, gcdError, gcdError, gcdError, gcdError, gcdError, gcdError,  gcdError,   gcdError,     gcdError, gcdError, gcdError},
/*  4 Angle16       */ {gcdError,    gcdError, gcdError, gcdError, gcdError, gcdError, gcdError, gcdError,  gcdError,   gcdError,     gcdError, gcdError, gcdError},
/*  5 Time          */ {gcdError,    gcdError, gcdError, gcdError, gcdError, gcdError, gcdError, gcdError,  gcdError,   gcdError,     gcdError, gcdError, gcdError},
/*  6 Date          */ {gcdError,    gcdError, gcdError, gcdError, gcdError, gcdError, gcdError, gcdError,  gcdError,   gcdError,     gcdError, gcdError, gcdError},
/*  7 String        */ {gcdError,    gcdError, gcdError, gcdError, gcdError, gcdError, gcdError, gcdError,  gcdError,   gcdError,     gcdError, gcdError, gcdError},
/*  8 Real16 mat    */ {gcdError,    gcdError, gcdError, gcdError, gcdError, gcdError, gcdError, gcdError,  gcdError,   gcdError,     gcdError, gcdError, gcdError},
/*  9 Complex16 mat */ {gcdError,    gcdError, gcdError, gcdError, gcdError, gcdError, gcdError, gcdError,  gcdError,   gcdError,     gcdError, gcdError, gcdError},
/* 10 Short integer */ {gcdLonIShoI, gcdError, gcdError, gcdError, gcdError, gcdError, gcdError, gcdError,  gcdError,   gcdShoIShoI,  gcdError, gcdError, gcdError},
/* 11 Real34        */ {gcdError,    gcdError, gcdError, gcdError, gcdError, gcdError, gcdError, gcdError,  gcdError,   gcdError,     gcdError, gcdError, gcdError},
/* 12 Complex34     */ {gcdError,    gcdError, gcdError, gcdError, gcdError, gcdError, gcdError, gcdError,  gcdError,   gcdError,     gcdError, gcdError, gcdError},
/* 13 Angle34       */ {gcdError,    gcdError, gcdError, gcdError, gcdError, gcdError, gcdError, gcdError,  gcdError,   gcdError,     gcdError, gcdError, gcdError}
};



/********************************************//**
 * \brief Data type error in gcd
 *
 * \param void
 * \return void
 ***********************************************/
void gcdError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
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
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  gcd[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

  if(lastErrorCode == 0) {
    fnDropY(NOPARAM);
  }
  else {
    restoreStack();
  }

  refreshStack();
}



void gcdLonILonI(void) {
  longInteger_t liX, liY;

  convertLongIntegerRegisterToLongInteger(REGISTER_Y, liY);
  longIntegerSetPositiveSign(liY);
  convertLongIntegerRegisterToLongInteger(REGISTER_X, liX);
  longIntegerSetPositiveSign(liX);

  longIntegerGcd(liY, liX, liX);

  convertLongIntegerToLongIntegerRegister(liX, REGISTER_X);

  longIntegerFree(liX);
  longIntegerFree(liY);
}



void gcdLonIShoI(void) {
  longInteger_t liX, liY;

  convertLongIntegerRegisterToLongInteger(REGISTER_Y, liY);
  longIntegerSetPositiveSign(liY);
  convertShortIntegerRegisterLongIntegerRegister(REGISTER_X, REGISTER_X);
  convertLongIntegerRegisterToLongInteger(REGISTER_X, liX);
  longIntegerSetPositiveSign(liX);

  longIntegerGcd(liY, liX, liX);

  convertLongIntegerToLongIntegerRegister(liX, REGISTER_X);

  longIntegerFree(liX);
  longIntegerFree(liY);
}



void gcdShoILonI(void) {
  longInteger_t liX, liY;

  convertLongIntegerRegisterToLongInteger(REGISTER_X, liX);
  longIntegerSetPositiveSign(liX);
  convertShortIntegerRegisterLongIntegerRegister(REGISTER_Y, REGISTER_Y);
  convertLongIntegerRegisterToLongInteger(REGISTER_Y, liY);
  longIntegerSetPositiveSign(liY);

  longIntegerGcd(liY, liX, liX);

  convertLongIntegerToLongIntegerRegister(liX, REGISTER_X);

  longIntegerFree(liX);
  longIntegerFree(liY);
}



void gcdShoIShoI(void) {
  *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = WP34S_intGCD(*(REGISTER_SHORT_INTEGER_DATA(REGISTER_Y)), *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)));
}
