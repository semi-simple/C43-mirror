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
 * \file cosh.c
 ***********************************************/

#include "wp43s.h"



void (* const Cosh[13])(void) = {
// regX ==> 1             2         3          4         5          6          7          8           9            10             11        12         13
//          Long integer  Real16    Complex16  Angle16   Time       Date       String     Real16 mat  Complex16 m  Short integer  Real34    Complex34  Angle34
            coshLonI,     coshRe16, coshCo16,  coshAn16, coshError, coshError, coshError, coshRm16,   coshCm16,    coshError,     coshRe34, coshCo34,  coshAn34
};



/********************************************//**
 * \brief Data type error in cosh
 *
 * \param void
 * \return void
 ***********************************************/
void coshError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate Cos for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnCosh:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and cosh(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnCosh(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  Cosh[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



void coshLonI(void) {
  realIc_t cosh;

  convertLongIntegerRegisterToRealIc(REGISTER_X, &cosh);
  WP34S_sinhcosh(&cosh, NULL, &cosh);

  reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, TAG_NONE);
  realIcToReal16(&cosh, REGISTER_REAL16_DATA(REGISTER_X));
}



void coshRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function coshRe16:", "cannot use NaN as X input of cosh", NULL, NULL);
    #endif
    return;
  }

  realIc_t a;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &a);
  WP34S_sinhcosh(&a, NULL, &a);
  realIcToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
}



void coshCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function coshCo16:", "cannot use NaN as X input of cosh", NULL, NULL);
    #endif
    return;
  }

  // cosh(a + i b) = cosh(a) cos(b) + i sinh(a) sin(b)
  realIc_t a, b, sha, cha, sb, cb;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &a);
  real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_X), &b);

 	WP34S_sinhcosh(&a, &sha, &cha);
 	WP34S_cvt_2rad_sincostan(&b, AM_RADIAN, &sb, &cb, NULL);

 	realIcMultiply(&cha, &cb, &a);
 	realIcMultiply(&sha, &sb, &b);

  realIcToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
  realIcToReal16(&b, REGISTER_IMAG16_DATA(REGISTER_X));
}



void coshAn16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function coshAn16:", "cannot use NaN as X input of cosh", NULL, NULL);
    #endif
    return;
  }

  realIc_t a;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &a);
  setRegisterDataType(REGISTER_X, dtReal16, TAG_NONE);
  WP34S_sinhcosh(&a, NULL, &a);
  realIcToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
}



void coshRm16(void) {
  fnToBeCoded();
}



void coshCm16(void) {
  fnToBeCoded();
}



void coshRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function coshRe34:", "cannot use NaN as X input of cosh", NULL, NULL);
    #endif
    return;
  }

  realIc_t a;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &a);
  WP34S_sinhcosh(&a, NULL, &a);
  realIcToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
}



void coshCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function coshCo34:", "cannot use NaN as X input of cosh", NULL, NULL);
    #endif
    return;
  }

  // cosh(a + i b) = cosh(a) cos(b) + i sinh(a) sin(b)
  realIc_t a, b, sha, cha, sb, cb;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &a);
  real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_X), &b);

 	WP34S_sinhcosh(&a, &sha, &cha);
 	WP34S_cvt_2rad_sincostan(&b, AM_RADIAN, &sb, &cb, NULL);

 	realIcMultiply(&cha, &cb, &a);
 	realIcMultiply(&sha, &sb, &b);

  realIcToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
  realIcToReal34(&b, REGISTER_IMAG34_DATA(REGISTER_X));
}



void coshAn34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function coshAn34:", "cannot use NaN as X input of cosh", NULL, NULL);
    #endif
    return;
  }

  realIc_t a;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &a);
  setRegisterDataType(REGISTER_X, dtReal34, TAG_NONE);
  WP34S_sinhcosh(&a, NULL, &a);
  realIcToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
}
