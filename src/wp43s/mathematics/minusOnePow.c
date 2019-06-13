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
 * \file minusOnePow.c
 ***********************************************/

#include "wp43s.h"



void (* const m1Pow[13])(void) = {
// regX ==> 1            2          3          4          5           6           7           8          9            10            11         12         13
//          Long integer Real16     Complex16  Angle16    Time        Date        String      Real16 mat Complex16 m  Short integer Real34     Complex34  Angle34
            m1PowLonI,   m1PowRe16, m1PowCo16, m1PowRe16, m1PowError, m1PowError, m1PowError, m1PowRm16, m1PowCm16,   m1PowShoI,    m1PowRe34, m1PowCo34, m1PowAn34
};



/********************************************//**
 * \brief Data type error in exp
 *
 * \param void
 * \return void
 ***********************************************/
void m1PowError(void) {
  displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate (-1)" STD_SUP_x " for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnM1Pow:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and 10^regX ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnM1Pow(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  result = REGISTER_X;
  opX    = allocateTemporaryRegister();
  copySourceRegisterToDestRegister(REGISTER_X, opX);

  m1Pow[getRegisterDataType(REGISTER_X)]();

  adjustResult(result, false, true, opX, -1, -1);
}



void m1PowLonI(void) {
  longInteger_t tmp;

  uIntToLongInteger(1, &tmp);

  if(longIntegerIsOdd(LONG_INTEGER_POINTER(REGISTER_LONG_INTEGER_DATA(opX)))) {
    longIntegerChangeSign(&tmp);
  }

  convertLongIntegerToLongIntegerRegister(&tmp, result);
}



void m1PowRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function m1PowRe16:", "cannot use NaN as an input of 2^", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opX);
  reallocateRegister(result, dtReal34, REAL34_SIZE, TAG_NONE);

  real34Multiply(const34_pi, REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));

  WP34S_cvt_2rad_sincos(NULL, REGISTER_REAL34_DATA(result), REGISTER_REAL34_DATA(result), AM_RADIAN);
  convertRegister34To16(result);
}



void m1PowCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function m1PowCo16:", "cannot use NaN as an input of 2^", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opX);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

  real34Multiply(const34_pi, REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(opX));
  real34Multiply(const34_pi, REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(opX));

  uint8_t savedAngularMode = currentAngularMode;
  currentAngularMode = AM_RADIAN;
  cosCo34();
  currentAngularMode = savedAngularMode;
  convertRegister34To16(result);
}



void m1PowRm16(void) {
  fnToBeCoded();
}



void m1PowCm16(void) {
  fnToBeCoded();
}



void m1PowShoI(void) {
  *(REGISTER_SHORT_INTEGER_DATA(result)) = WP34S_int_1pow(*(REGISTER_SHORT_INTEGER_DATA(opX)));
}



void m1PowRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function m1PowRe34:", "cannot use NaN as an input of 2^", NULL, NULL);
    #endif
    return;
  }

  real34Multiply(const34_pi, REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  WP34S_cvt_2rad_sincos(NULL, REGISTER_REAL34_DATA(result), REGISTER_REAL34_DATA(result), AM_RADIAN);
}



void m1PowCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function m1PowCo34:", "cannot use NaN as an input of 2^", NULL, NULL);
    #endif
    return;
  }

  real34Multiply(const34_pi, REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(opX));
  real34Multiply(const34_pi, REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(opX));

  uint8_t savedAngularMode = currentAngularMode;
  currentAngularMode = AM_RADIAN;
  cosCo34();
  currentAngularMode = savedAngularMode;
}



void m1PowAn34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function m1PowAn34:", "cannot use NaN as an input of 2^", NULL, NULL);
    #endif
    return;
  }

  setRegisterDataType(result, dtReal34, TAG_NONE);
  real34Multiply(const34_pi, REGISTER_REAL34_DATA(result), REGISTER_REAL34_DATA(result));
  WP34S_cvt_2rad_sincos(NULL, REGISTER_REAL34_DATA(result), REGISTER_REAL34_DATA(result), AM_RADIAN);
}
