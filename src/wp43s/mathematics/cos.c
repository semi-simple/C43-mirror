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



void (* const Cos[9])(void) = {
// regX ==> 1            2        3         4         5         6         7          8           9
//          Long integer Real34   Complex34 Time      Date      String    Real34 mat Complex34 m Short integer
            cosLonI,     cosReal, cosCplx,  cosError, cosError, cosError, cosRema,   cosCxma,    cosError
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



void cosCo39(const real39_t *zReal, const real39_t *zImag, real39_t *resReal, real39_t *resImag) {
  // cos(a + i b) = cos(a)*cosh(b) - i*sin(a)*sinh(b)
  real39_t sina, cosa, sinhb, coshb;

  WP34S_Cvt2RadSinCosTan(zReal, AM_RADIAN, &sina, &cosa, NULL);
  WP34S_SinhCosh(zImag, &sinhb, &coshb);

  realMultiply(&cosa, &coshb, resReal, &ctxtReal39);
  realMultiply(&sina, &sinhb, resImag, &ctxtReal39);
  realChangeSign(resImag);
}



void cosLonI(void) {
  real39_t x;

  longIntegerAngleReduction(REGISTER_X, currentAngularMode, &x);
  WP34S_Cvt2RadSinCosTan(&x, currentAngularMode, NULL, &x, NULL);

  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
}



void cosRema(void) {
  fnToBeCoded();
}



void cosCxma(void) {
  fnToBeCoded();
}



void cosReal(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function cosReal:", "cannot use NaN as X input of cos", NULL, NULL);
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
    WP34S_Cvt2RadSinCosTan(&x, (xAngularMode == AM_NONE ? currentAngularMode : xAngularMode), NULL, &x, NULL);
    realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  }
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



void cosCplx(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function cosCplx:", "cannot use NaN as X input of cos", NULL, NULL);
    #endif
    return;
  }

  real39_t zReal, zImag;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &zReal);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &zImag);

  cosCo39(&zReal, &zImag, &zReal, &zImag);

  realToReal34(&zReal, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&zImag, REGISTER_IMAG34_DATA(REGISTER_X));
}
