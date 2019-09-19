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
 * \file sinh.c
 ***********************************************/

#include "wp43s.h"



void (* const Sinh[13])(void) = {
// regX ==> 1             2         3          4         5          6          7          8           9            10             11        12         13
//          Long integer  Real16    Complex16  Angle16   Time       Date       String     Real16 mat  Complex16 m  Short integer  Real34    Complex34  Angle16
            sinhLonI,     sinhRe16, sinhCo16,  sinhAn16, sinhError, sinhError, sinhError, sinhRm16,   sinhCm16,    sinhError,     sinhRe34, sinhCo34,  sinhAn34
};



/********************************************//**
 * \brief Data type error in sinh
 *
 * \param void
 * \return void
 ***********************************************/
void sinhError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate Sin for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnSinh:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and sinh(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnSinh(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  Sinh[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



void sinhLonI(void) {
  realIc_t sinh;

  convertLongIntegerRegisterToRealIc(REGISTER_X, &sinh);
  WP34S_SinhCosh(&sinh, &sinh, NULL);

  reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, TAG_NONE);
  realIcToReal16(&sinh, REGISTER_REAL16_DATA(REGISTER_X));
}



void sinhRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function sinhRe16:", "cannot use NaN as X input of sinh", NULL, NULL);
    #endif
    return;
  }

  realIc_t a;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &a);
  WP34S_SinhCosh(&a, &a, NULL);
  realIcToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
}



void sinhCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function sinhCo16:", "cannot use NaN as X input of sinh", NULL, NULL);
    #endif
    return;
  }

  // sinh(a + i b) = sinh(a) cos(b) + i cosh(a) sin(b)
  realIc_t a, b, sha, cha, sb, cb;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &a);
  real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_X), &b);

 	WP34S_SinhCosh(&a, &sha, &cha);
 	WP34S_Cvt2RadSinCosTan(&b, AM_RADIAN, &sb, &cb, NULL);

 	realIcMultiply(&sha, &cb, &a);
 	realIcMultiply(&cha, &sb, &b);

  realIcToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
  realIcToReal16(&b, REGISTER_IMAG16_DATA(REGISTER_X));
}



void sinhAn16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function sinhAn16:", "cannot use NaN as X input of sinh", NULL, NULL);
    #endif
    return;
  }

  realIc_t a;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &a);
  setRegisterDataType(REGISTER_X, dtReal16, TAG_NONE);
  WP34S_SinhCosh(&a, &a, NULL);
  realIcToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
}



void sinhRm16(void) {
  fnToBeCoded();
}



void sinhCm16(void) {
  fnToBeCoded();
}



void sinhRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function sinhRe34:", "cannot use NaN as X input of sinh", NULL, NULL);
    #endif
    return;
  }

  realIc_t a;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &a);
  WP34S_SinhCosh(&a, &a, NULL);
  realIcToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
}



void sinhCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function sinhCo34:", "cannot use NaN as X input of sinh", NULL, NULL);
    #endif
    return;
  }

  // sinh(a + i b) = sinh(a) cos(b) + i cosh(a) sin(b)
  realIc_t a, b, sha, cha, sb, cb;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &a);
  real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_X), &b);

 	WP34S_SinhCosh(&a, &sha, &cha);
 	WP34S_Cvt2RadSinCosTan(&b, AM_RADIAN, &sb, &cb, NULL);

 	realIcMultiply(&sha, &cb, &a);
 	realIcMultiply(&cha, &sb, &b);

  realIcToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
  realIcToReal34(&b, REGISTER_IMAG34_DATA(REGISTER_X));
}



void sinhAn34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function sinhAn34:", "cannot use NaN as X input of sinh", NULL, NULL);
    #endif
    return;
  }

  realIc_t a;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &a);
  setRegisterDataType(REGISTER_X, dtReal34, TAG_NONE);
  WP34S_SinhCosh(&a, &a, NULL);
  realIcToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
}
