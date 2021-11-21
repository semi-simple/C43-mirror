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
 * \file atan2.c
 ***********************************************/

#include "mathematics/atan2.h"

#include "conversionAngles.h"
#include "debug.h"
#include "error.h"
#include "mathematics/wp34s.h"
#include "realType.h"
#include "registers.h"
#include "registerValueConversions.h"

#include "wp43s.h"



TO_QSPI void (* const arctan2[NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS][NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS])(void) = {
// regX |    regY ==>   1              2              3           4           5           6           7           8           9             10
//      V               Long integer   Real34         Complex34   Time        Date        String      Real34 mat  Complex34 m Short integer Config data
/*  1 Long integer  */ {atan2LonILonI, atan2RealLonI, atan2Error, atan2Error, atan2Error, atan2Error, atan2Error, atan2Error, atan2Error,   atan2Error},
/*  2 Real34        */ {atan2LonIReal, atan2RealReal, atan2Error, atan2Error, atan2Error, atan2Error, atan2Error, atan2Error, atan2Error,   atan2Error},
/*  3 Complex34     */ {atan2Error,    atan2Error,    atan2Error, atan2Error, atan2Error, atan2Error, atan2Error, atan2Error, atan2Error,   atan2Error},
/*  4 Time          */ {atan2Error,    atan2Error,    atan2Error, atan2Error, atan2Error, atan2Error, atan2Error, atan2Error, atan2Error,   atan2Error},
/*  5 Date          */ {atan2Error,    atan2Error,    atan2Error, atan2Error, atan2Error, atan2Error, atan2Error, atan2Error, atan2Error,   atan2Error},
/*  6 String        */ {atan2Error,    atan2Error,    atan2Error, atan2Error, atan2Error, atan2Error, atan2Error, atan2Error, atan2Error,   atan2Error},
/*  7 Real34 mat    */ {atan2Error,    atan2Error,    atan2Error, atan2Error, atan2Error, atan2Error, atan2Error, atan2Error, atan2Error,   atan2Error},
/*  8 Complex34 mat */ {atan2Error,    atan2Error,    atan2Error, atan2Error, atan2Error, atan2Error, atan2Error, atan2Error, atan2Error,   atan2Error},
/*  9 Short integer */ {atan2Error,    atan2Error,    atan2Error, atan2Error, atan2Error, atan2Error, atan2Error, atan2Error, atan2Error,   atan2Error},
/* 10 Config data   */ {atan2Error,    atan2Error,    atan2Error, atan2Error, atan2Error, atan2Error, atan2Error, atan2Error, atan2Error,   atan2Error}
};



/********************************************//**
 * \brief Data type error in arctan
 *
 * \param void
 * \return void
 ***********************************************/
#if (EXTRA_INFO_ON_CALC_ERROR == 1)
  void atan2Error(void) {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    sprintf(errorMessage, "cannot calculate atan2 for %s and %s", getRegisterDataTypeName(REGISTER_Y, true, false), getRegisterDataTypeName(REGISTER_X, true, false));
    moreInfoOnError("In function fnAtan2:", errorMessage, NULL, NULL);
  }
#endif // (EXTRA_INFO_ON_CALC_ERROR == 1)



/********************************************//**
 * \brief regX ==> regL and atan2(regY, regX) ==> regX
 * Drops Y, enables stack lift and refreshes the stack
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnAtan2(uint16_t unusedButMandatoryParameter) {
  if(!saveLastX()) return;

  arctan2[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

  adjustResult(REGISTER_X, true, true, REGISTER_X, REGISTER_Y, -1);
}



void atan2LonILonI(void) {
  real_t y, x;

  convertLongIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  convertLongIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  if(realIsZero(&y) && realIsZero(&x) && !getSystemFlag(FLAG_SPCRES)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      moreInfoOnError("In function atan2RealReal:", "X = 0 and Y = 0", NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return;
  }
  WP34S_Atan2(&y, &x, &x, &ctxtReal39);
  convertAngleFromTo(&x, amRadian, currentAngularMode, &ctxtReal39);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
  convertRealToReal34ResultRegister(&x, REGISTER_X);

  setRegisterAngularMode(REGISTER_X, currentAngularMode);
}



void atan2RealLonI(void) {
  real_t y, x;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  convertLongIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  if(realIsZero(&y) && realIsZero(&x) && !getSystemFlag(FLAG_SPCRES)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      moreInfoOnError("In function atan2RealReal:", "X = 0 and Y = 0", NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return;
  }
  WP34S_Atan2(&y, &x, &x, &ctxtReal39);
  convertAngleFromTo(&x, amRadian, currentAngularMode, &ctxtReal39);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
  convertRealToReal34ResultRegister(&x, REGISTER_X);

  setRegisterAngularMode(REGISTER_X, currentAngularMode);
}



void atan2LonIReal(void) {
  real_t y, x;

  convertLongIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
  if(realIsZero(&y) && realIsZero(&x) && !getSystemFlag(FLAG_SPCRES)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      moreInfoOnError("In function atan2RealReal:", "X = 0 and Y = 0", NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return;
  }
  WP34S_Atan2(&y, &x, &x, &ctxtReal39);
  convertAngleFromTo(&x, amRadian, currentAngularMode, &ctxtReal39);
  convertRealToReal34ResultRegister(&x, REGISTER_X);

  setRegisterAngularMode(REGISTER_X, currentAngularMode);
}



void atan2RealReal(void) {
  real_t y, x;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
  if(realIsZero(&y) && realIsZero(&x) && !getSystemFlag(FLAG_SPCRES)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      moreInfoOnError("In function atan2RealReal:", "X = 0 and Y = 0", NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return;
  }
  WP34S_Atan2(&y, &x, &x, &ctxtReal39);
  convertAngleFromTo(&x, amRadian, currentAngularMode, &ctxtReal39);
  convertRealToReal34ResultRegister(&x, REGISTER_X);

  setRegisterAngularMode(REGISTER_X, currentAngularMode);
}
