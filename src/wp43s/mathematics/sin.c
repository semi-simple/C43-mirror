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



void (* const Sin[NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS])(void) = {
// regX ==> 1            2        3         4         5         6         7          8           9             10
//          Long integer Real34   Complex34 Time      Date      String    Real34 mat Complex34 m Short integer Config data
            sinLonI,     sinReal, sinCplx,  sinError, sinError, sinError, sinRema,   sinCxma,    sinError,     sinError
};



/********************************************//**
 * \brief Data type error in sin
 *
 * \param void
 * \return void
 ***********************************************/
void sinError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate Sin for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    moreInfoOnError("In function fnSin:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and sin(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnSin(uint16_t unusedButMandatoryParameter) {
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  Sin[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



void sinComplex(const real_t *real, const real_t *imag, real_t *resReal, real_t *resImag, realContext_t *realContext) {
  // sin(a + ib) = sin(a)*cosh(b) + i*cos(a)*sinh(b)
  real_t sina, cosa, sinhb, coshb;

  WP34S_Cvt2RadSinCosTan(real, AM_RADIAN, &sina, &cosa, NULL, realContext);
  WP34S_SinhCosh(imag, &sinhb, &coshb, realContext);

  realMultiply(&sina, &coshb, resReal, realContext);
  realMultiply(&cosa, &sinhb, resImag, realContext);
}



void sinLonI(void) {
  real_t x;

  longIntegerAngleReduction(REGISTER_X, currentAngularMode, &x);
  WP34S_Cvt2RadSinCosTan(&x, currentAngularMode, &x, NULL, NULL, &ctxtReal39);

  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
}



void sinRema(void) {
  fnToBeCoded();
}



void sinCxma(void) {
  fnToBeCoded();
}



void sinReal(void) {
  if(real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_X))) {
    realToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    real_t x;
    uint32_t xAngularMode;

    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
    xAngularMode = getRegisterAngularMode(REGISTER_X);
    WP34S_Cvt2RadSinCosTan(&x, (xAngularMode == AM_NONE ? currentAngularMode : xAngularMode), &x, NULL, NULL, &ctxtReal39);
    realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  }
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



void sinCplx(void) {
  real_t zReal, zImag;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &zReal);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &zImag);

  sinComplex(&zReal, &zImag, &zReal, &zImag, &ctxtReal39);

  realToReal34(&zReal, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&zImag, REGISTER_IMAG34_DATA(REGISTER_X));
}
