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



void (* const m1Pow[12])(void) = {
// regX ==> 1            2          3          4           5           6           7           8          9            10            11         12
//          Long integer Real16     Complex16  Angle16     Time        Date        String      Real16 mat Complex16 m  Short integer Real34     Complex34
            m1PowLonI,   m1PowRe16, m1PowCo16, m1PowError, m1PowError, m1PowError, m1PowError, m1PowRm16, m1PowCm16,   m1PowShoI,    m1PowRe34, m1PowCo34
};



/********************************************//**
 * \brief Data type error in exp
 *
 * \param void
 * \return void
 ***********************************************/
void m1PowError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
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

  m1Pow[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



/**********************************************************************
 * In all the functions below:
 * if X is a number then X = a + ib
 * The variables a and b are used for intermediate calculations
 ***********************************************************************/

void m1PowLonI(void) {
  longInteger_t lgInt, exponent;

  longIntegerInit(lgInt);
  uIntToLongInteger(1, lgInt);

  convertLongIntegerRegisterToLongInteger(REGISTER_X, exponent);
  if(longIntegerIsOdd(exponent)) {
    longIntegerChangeSign(lgInt);
  }

  convertLongIntegerToLongIntegerRegister(lgInt, REGISTER_X);

  longIntegerFree(lgInt);
  longIntegerFree(exponent);
}



void m1PowRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function m1PowRe16:", "cannot use NaN as X input of 2^", NULL, NULL);
    #endif
    return;
  }

  realIc_t x;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &x);

  realIcMultiply(const_pi, &x, &x);
  WP34S_Cvt2RadSinCosTan(&x, AM_RADIAN, NULL, &x, NULL);

  realIcToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



void m1PowCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function m1PowCo16:", "cannot use NaN as X input of 2^", NULL, NULL);
    #endif
    return;
  }

  realIc_t real;

  convertRegister16To34(REGISTER_X);

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &real);
  realIcMultiply(const_pi, &real, &real);
  realIcToReal34(&real, REGISTER_REAL34_DATA(REGISTER_X));

  real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_X), &real);
  realIcMultiply(const_pi, &real, &real);
  realIcToReal34(&real, REGISTER_IMAG34_DATA(REGISTER_X));

  uint8_t savedAngularMode = currentAngularMode;
  currentAngularMode = AM_RADIAN;
  cosCo34();
  currentAngularMode = savedAngularMode;
  convertRegister34To16(REGISTER_X);
}



void m1PowRm16(void) {
  fnToBeCoded();
}



void m1PowCm16(void) {
  fnToBeCoded();
}



void m1PowShoI(void) {
  *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = WP34S_int_1pow(*(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)));
}



void m1PowRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function m1PowRe34:", "cannot use NaN as X input of 2^", NULL, NULL);
    #endif
    return;
  }

  realIc_t x;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &x);

  realIcMultiply(const_pi, &x, &x);
  WP34S_Cvt2RadSinCosTan(&x, AM_RADIAN, NULL, &x, NULL);

  realIcToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



void m1PowCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function m1PowCo34:", "cannot use NaN as X input of 2^", NULL, NULL);
    #endif
    return;
  }

  realIc_t real;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &real);
  realIcMultiply(const_pi, &real, &real);
  realIcToReal34(&real, REGISTER_REAL34_DATA(REGISTER_X));

  real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_X), &real);
  realIcMultiply(const_pi, &real, &real);
  realIcToReal34(&real, REGISTER_IMAG34_DATA(REGISTER_X));

  uint8_t savedAngularMode = currentAngularMode;
  currentAngularMode = AM_RADIAN;
  cosCo34();
  currentAngularMode = savedAngularMode;
}
