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
 * \file tanh.c
 ***********************************************/

#include "wp43s.h"



void (* const Tanh[13])(void) = {
// regX ==> 1             2         3          4         5          6          7          8           9            10             11        12         13
//          Long integer  Real16    Complex16  Angle16   Time       Date       String     Real16 mat  Complex16 m  Short integer  Real34    Complex34  Angle34
            tanhLonI,     tanhRe16, tanhCo16,  tanhAn16, tanhError, tanhError, tanhError, tanhRm16,   tanhCm16,    tanhError,     tanhRe34, tanhCo34,  tanhAn34
};



/********************************************//**
 * \brief Data type error in tanh
 *
 * \param void
 * \return void
 ***********************************************/
void tanhError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate Tan for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnTanh:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and tanh(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnTanh(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  Tanh[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



void tanhLonI(void) {
  realIc_t tanh;

  convertLongIntegerRegisterToRealIc(REGISTER_X, &tanh);
  WP34S_tanh(&tanh, &tanh);

  reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, TAG_NONE);
  realIcToReal16(&tanh, REGISTER_REAL16_DATA(REGISTER_X));
}



void tanhRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function tanhRe16:", "cannot use NaN as an input of tanh", NULL, NULL);
    #endif
    return;
  }

  realIc_t tanh;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &tanh);
  WP34S_tanh(&tanh, &tanh);
  realIcToReal16(&tanh, REGISTER_REAL16_DATA(REGISTER_X));
}



void tanhCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function tanhCo16:", "cannot use NaN as an input of tanh", NULL, NULL);
    #endif
    return;
  }

  // tanh(a + i b) = (tanh(a) + i tan(b)) / (1 + i tanh(a) tan(b))
  realIc_t a, b, sa, ca;
  complexIc_t numer, denom;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &a);
  real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_X), &b);

  if(realIcIsZero(&b)) {
    WP34S_tanh(&a, &numer.real);
    realIcZero(&numer.imag);
  }
  else {
    WP34S_tanh(&a, &numer.real);
    WP34S_cvt_2rad_sincostan(&b, AM_RADIAN, &sa, &ca, &numer.imag);

    realIcCopy(const_1, &denom.real);
    realIcMultiply(&numer.real, &numer.imag, &denom.imag);

    divCoIcCoIc(&numer, &denom, &numer);
  }

  realIcToReal16(&numer.real, REGISTER_REAL16_DATA(REGISTER_X));
  realIcToReal16(&numer.imag, REGISTER_IMAG16_DATA(REGISTER_X));
}



void tanhAn16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function tanhAn16:", "cannot use NaN as an input of tanh", NULL, NULL);
    #endif
    return;
  }

  realIc_t tanh;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &tanh);
  setRegisterDataType(REGISTER_X, dtReal16, TAG_NONE);
  WP34S_tanh(&tanh, &tanh);
  realIcToReal16(&tanh, REGISTER_REAL16_DATA(REGISTER_X));
}



void tanhRm16(void) {
  fnToBeCoded();
}



void tanhCm16(void) {
  fnToBeCoded();
}



void tanhRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function tanhRe34:", "cannot use NaN as an input of tanh", NULL, NULL);
    #endif
    return;
  }

  realIc_t tanh;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &tanh);
  WP34S_tanh(&tanh, &tanh);
  realIcToReal34(&tanh, REGISTER_REAL34_DATA(REGISTER_X));
}



void tanhCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function tanhCo34:", "cannot use NaN as an input of tanh", NULL, NULL);
    #endif
    return;
  }

  // tanh(a + i b) = (tanh(a) + i tan(b)) / (1 + i tanh(a) tan(b))
  realIc_t a, b, sa, ca;
  complexIc_t numer, denom;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &a);
  real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_X), &b);

  if(realIcIsZero(&b)) {
    WP34S_tanh(&a, &numer.real);
    realIcZero(&numer.imag);
  }
  else {
    WP34S_tanh(&a, &numer.real);
    WP34S_cvt_2rad_sincostan(&b, AM_RADIAN, &sa, &ca, &numer.imag);

    realIcCopy(const_1, &denom.real);
    realIcMultiply(&numer.real, &numer.imag, &denom.imag);

    divCoIcCoIc(&numer, &denom, &numer);
  }

  realIcToReal34(&numer.real, REGISTER_REAL34_DATA(REGISTER_X));
  realIcToReal34(&numer.imag, REGISTER_IMAG34_DATA(REGISTER_X));
}



void tanhAn34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function tanhAn34:", "cannot use NaN as an input of tanh", NULL, NULL);
    #endif
    return;
  }

  realIc_t tanh;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &tanh);
  setRegisterDataType(REGISTER_X, dtReal34, TAG_NONE);
  WP34S_tanh(&tanh, &tanh);
  realIcToReal34(&tanh, REGISTER_REAL34_DATA(REGISTER_X));
}
