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
 * \file cos.c
 ***********************************************/

#include "wp43s.h"



void (* const Cos[13])(void) = {
// regX ==> 1            2        3         4        5         6         7         8          9           10            11       12        13
//          Long integer Real16   Complex16 Angle16  Time      Date      String    Real16 mat Complex16 m Short integer Real34   Complex34 Angle34
            cosLonI,     cosRe16, cosCo16,  cosAn16, cosError, cosError, cosError, cosRm16,   cosCm16,    cosError,     cosRe34, cosCo34,  cosAn34
};



/********************************************//**
 * \brief Data type error in cos
 *
 * \param void
 * \return void
 ***********************************************/
void cosError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate Cos for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnCos:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and cos(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnCos(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  Cos[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



void cosLonI(void) {
  realIc_t cos;

  longIntegerAngleReduction(REGISTER_X, currentAngularMode, &cos);
  WP34S_cvt_2rad_sincos(NULL, &cos, &cos, currentAngularMode);

  reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, TAG_NONE);
  realIcToReal16(&cos, REGISTER_REAL16_DATA(REGISTER_X));
}



void cosRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function cosRe16:", "cannot use NaN as X input of cos", NULL, NULL);
    #endif
    return;
  }

  if(real16IsInfinite(REGISTER_REAL16_DATA(REGISTER_X))) {
    realIcToReal16(const_NaN, REGISTER_REAL16_DATA(REGISTER_X));
  }
  else {
    realIc_t a;

    real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &a);
    WP34S_cvt_2rad_sincos(NULL, &a, &a, currentAngularMode);
    realIcToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
  }
}



void cosCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function cosCo16:", "cannot use NaN as X input of cos", NULL, NULL);
    #endif
    return;
  }

  // cos(a + ib) = cosh(b)*cos(a) - i*sin(a)*sinh(b)
  //
  //           e^b + e^(-b)                  e^b - e^(-b)
  // cosh(b)= --------------       sinh(b)= --------------
  //                2                             2
  //
  // cos(a + ib) = (e^b + e^(-b))*cos(a)/2 + i*(e^(-b) - e^-b)*sin(a)/2

  realIc_t expIm, expMIm, a, b, sin, cos;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &a);
  real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_X), &b);

  // expIm = e^b  and expMIm = e^(-b)
  realIcExp(&b, &expIm);
  realIcChangeSign(&b);
  realIcExp(&b, &expMIm);

  // Calculate cos and sin
  WP34S_cvt_2rad_sincos(&sin, &cos, &a, AM_RADIAN);

  // real part = (e^b + e^(-b))*cos(a)/2
  realIcAdd(&expIm, &expMIm, &a);
  realIcMultiply(&a, &cos, &a);
  realIcMultiply(&a, const_0_5, &a);

  // imaginary part = (e^(-b) - e^b)*sin(a)/2
  realIcSubtract(&expMIm, &expIm, &b);
  realIcMultiply(&b, &sin, &b);
  realIcMultiply(&b, const_0_5, &b);

  realIcToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
  realIcToReal16(&b, REGISTER_IMAG16_DATA(REGISTER_X));
}



void cosAn16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function cosAn16:", "cannot use NaN as X input of cos", NULL, NULL);
    #endif
    return;
  }

  if(real16IsInfinite(REGISTER_REAL16_DATA(REGISTER_X))) {
    realIcToReal16(const_NaN, REGISTER_REAL16_DATA(REGISTER_X));
  }
  else {
    realIc_t a;

    real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &a);
    WP34S_cvt_2rad_sincos(NULL, &a, &a, getRegisterAngularMode(REGISTER_X));
    realIcToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
  }

  setRegisterDataType(REGISTER_X, dtReal16, TAG_NONE);
}



void cosRm16(void) {
  fnToBeCoded();
}



void cosCm16(void) {
  fnToBeCoded();
}



void cosRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function cosRe34:", "cannot use NaN as X input of cos", NULL, NULL);
    #endif
    return;
  }

  if(real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_X))) {
    realIcToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    realIc_t a;

    real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &a);
    WP34S_cvt_2rad_sincos(NULL, &a, &a, currentAngularMode);
    realIcToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
  }
}



void cosCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function cosCo34:", "cannot use NaN as X input of cos", NULL, NULL);
    #endif
    return;
  }

  // cos(a + ib) = cosh(b)*cos(a) - i*sin(a)*sinh(b)
  //
  //           e^b + e^(-b)                  e^b - e^(-b)
  // cosh(b)= --------------       sinh(b)= --------------
  //                2                             2
  //
  // cos(a + ib) = (e^b + e^(-b))*cos(a)/2 + i*(e^(-b) - e^-b)*sin(a)/2

  realIc_t expIm, expMIm, a, b, sin, cos;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &a);
  real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_X), &b);

  // expIm = e^b  and expMIm = e^(-b)
  realIcExp(&b, &expIm);
  realIcChangeSign(&b);
  realIcExp(&b, &expMIm);

  // Calculate cos and sin
  WP34S_cvt_2rad_sincos(&sin, &cos, &a, AM_RADIAN);

  // real part = (e^b + e^(-b))*cos(a)/2
  realIcAdd(&expIm, &expMIm, &a);
  realIcMultiply(&a, &cos, &a);
  realIcMultiply(&a, const_0_5, &a);

  // imaginary part = (e^(-b) - e^b)*sin(a)/2
  realIcSubtract(&expMIm, &expIm, &b);
  realIcMultiply(&b, &sin, &b);
  realIcMultiply(&b, const_0_5, &b);

  realIcToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
  realIcToReal34(&b, REGISTER_IMAG34_DATA(REGISTER_X));
}



void cosAn34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function cosAn34:", "cannot use NaN as X input of cos", NULL, NULL);
    #endif
    return;
  }

  if(real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_X))) {
    realIcToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    realIc_t a;

    real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &a);
    WP34S_cvt_2rad_sincos(NULL, &a, &a, getRegisterAngularMode(REGISTER_X));
    realIcToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
  }

  setRegisterDataType(REGISTER_X, dtReal34, TAG_NONE);
}
