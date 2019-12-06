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
 * \file sin.c
 ***********************************************/

#include "wp43s.h"



void (* const Sin[12])(void) = {
// regX ==> 1            2        3         4         5         6         7         8          9           10            11       12
//          Long integer Real16   Complex16 Angle16   Time      Date      String    Real16 mat Complex16 m Short integer Real34   Complex34
            sinLonI,     sinRe16, sinCo16,  sinError, sinError, sinError, sinError, sinRm16,   sinCm16,    sinError,     sinRe34, sinCo34
};



/********************************************//**
 * \brief Data type error in sin
 *
 * \param void
 * \return void
 ***********************************************/
void sinError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate Sin for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnSin:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and sin(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnSin(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  Sin[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



void sinCo39(const complex39_t *z, complex39_t *res) {
  // sin(a + ib) = sin(a)*cosh(b) + i*cos(a)*sinh(b)
  real39_t sina, cosa, sinhb, coshb;

  WP34S_Cvt2RadSinCosTan(&z->real, AM_RADIAN, &sina, &cosa, NULL);
  WP34S_SinhCosh(&z->imag, &sinhb, &coshb);

  realMultiply(&sina, &coshb, &res->real, &ctxtReal39);
  realMultiply(&cosa, &sinhb, &res->imag, &ctxtReal39);
}



void sinLonI(void) {
  real39_t x;

  longIntegerAngleReduction(REGISTER_X, currentAngularMode, &x);
  WP34S_Cvt2RadSinCosTan(&x, currentAngularMode, &x, NULL, NULL);

  reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);
  realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
}



void sinRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function sinRe16:", "cannot use NaN as X input of sin", NULL, NULL);
    #endif
    return;
  }

  if(real16IsInfinite(REGISTER_REAL16_DATA(REGISTER_X))) {
    realToReal16(const_NaN, REGISTER_REAL16_DATA(REGISTER_X));
  }
  else {
    real39_t x;
    uint32_t xAngularMode;

    real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &x);
    xAngularMode = getRegisterAngularMode(REGISTER_X);
    WP34S_Cvt2RadSinCosTan(&x, (xAngularMode == AM_NONE ? currentAngularMode : xAngularMode), &x, NULL, NULL);
    realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
  }
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



void sinCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function sinCo16:", "cannot use NaN as X input of sin", NULL, NULL);
    #endif
    return;
  }

  complex39_t z;

  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &z.real);
  real16ToReal(REGISTER_IMAG16_DATA(REGISTER_X), &z.imag);

  sinCo39(&z, &z);

  realToReal16(&z.real, REGISTER_REAL16_DATA(REGISTER_X));
  realToReal16(&z.imag, REGISTER_IMAG16_DATA(REGISTER_X));
}



void sinRm16(void) {
  fnToBeCoded();
}



void sinCm16(void) {
  fnToBeCoded();
}



void sinRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function sinRe34:", "cannot use NaN as X input of sin", NULL, NULL);
    #endif
    return;
  }

  if(real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_X))) {
    realToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    real39_t x;
    uint32_t xAngularMode;

    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
    xAngularMode = getRegisterAngularMode(REGISTER_X);
    WP34S_Cvt2RadSinCosTan(&x, (xAngularMode == AM_NONE ? currentAngularMode : xAngularMode), &x, NULL, NULL);
    realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  }
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



void sinCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function sinCo34:", "cannot use NaN as X input of sin", NULL, NULL);
    #endif
    return;
  }

  complex39_t z;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &z.real);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &z.imag);

  sinCo39(&z, &z);

  realToReal34(&z.real, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&z.imag, REGISTER_IMAG34_DATA(REGISTER_X));
}
