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
 * \file arcsinh.c
 ***********************************************/

#include "wp43s.h"



void (* const arcsinh[13])(void) = {
// regX ==> 1             2            3            4            5             6             7             8            9            10             11           12           13
//          Long integer  Real16       Complex16    Angle16      Time          Date          String        Real16 mat   Complex16 m  Short integer  Real34       Complex34    Angle34
            arcsinhLonI,  arcsinhRe16, arcsinhCo16, arcsinhRe16, arcsinhError, arcsinhError, arcsinhError, arcsinhRm16, arcsinhCm16, arcsinhError,  arcsinhRe34, arcsinhCo34, arcsinhRe34
};



/********************************************//**
 * \brief Data type error in arcsinh
 *
 * \param void
 * \return void
 ***********************************************/
void arcsinhError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate arcsinh for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnArcsinh:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and arcsinh(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnArcsinh(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  arcsinh[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



/**********************************************************************
 * In all the functions below:
 * if X is a number then X = a + ib
 * The variables a and b are used for intermediate calculations
 ***********************************************************************/

void arcsinhLonI(void) {
  realIc_t a, aSquared;

  convertLongIntegerRegisterToRealIc(REGISTER_X, &a);
  reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, currentAngularMode);

  // arcsinh(a) = ln(a + sqrt(a² + 1))
  realIcMultiply(&a, &a, &aSquared);
  realIcAdd(&aSquared, const_1, &aSquared);
  realIcSquareRoot(&aSquared, &aSquared);
  realIcAdd(&aSquared, &a, &a);
  WP34S_Ln(&a, &a);

  realIcToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
}



void arcsinhRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function arcsinhRe16:", "cannot use NaN as X input of arcsinh", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, aSquared;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &a);
  setRegisterDataType(REGISTER_X, dtReal16, TAG_NONE);

  // arcsinh(a) = ln(a + sqrt(a² + 1))
  realIcMultiply(&a, &a, &aSquared);
  realIcAdd(&aSquared, const_1, &aSquared);
  realIcSquareRoot(&aSquared, &aSquared);
  realIcAdd(&aSquared, &a, &a);
  WP34S_Ln(&a, &a);

  realIcToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
}



void arcsinhCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function arcsinhCo16:", "cannot use NaN as X input of arcsinh", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, b, real, imag;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &a);
  real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_X), &b);

  // arcsinh(z) = ln(z + sqrt(z² + 1))
  // calculate z²   real part
  realIcMultiply(&b, &b, &real);
  realIcChangeSign(&real);
  realIcFMA(&a, &a, &real, &real);

  // calculate z²   imaginary part
  realIcMultiply(&a, &b, &imag);
  realIcMultiply(&imag, const_2, &imag);

  // calculate z² + 1
  realIcAdd(&real, const_1, &real);

  // calculate sqrt(z² + 1)
  realIcRectangularToPolar(&real, &imag, &real, &imag);
  realIcSquareRoot(&real, &real);
  realIcMultiply(&imag, const_0_5, &imag);
  realIcPolarToRectangular(&real, &imag, &real, &imag);

  // calculate z + sqrt(z² + 1)
  realIcAdd(&a, &real, &real);
  realIcAdd(&b, &imag, &imag);

  // calculate ln(z + sqtr(z² + 1))
  realIcRectangularToPolar(&real, &imag, &a, &b);
  WP34S_Ln(&a, &a);

  realIcToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
  realIcToReal16(&b, REGISTER_IMAG16_DATA(REGISTER_X));
}



void arcsinhRm16(void) {
  fnToBeCoded();
}



void arcsinhCm16(void) {
  fnToBeCoded();
}



void arcsinhRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function arcsinhRe34:", "cannot use NaN as X input of arcsinh", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, aSquared;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &a);
  setRegisterDataType(REGISTER_X, dtReal34, TAG_NONE);

  // arcsinh(a) = ln(a + sqrt(a² + 1))
  realIcMultiply(&a, &a, &aSquared);
  realIcAdd(&aSquared, const_1, &aSquared);
  realIcSquareRoot(&aSquared, &aSquared);
  realIcAdd(&aSquared, &a, &a);
  WP34S_Ln(&a, &a);

  realIcToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
}



void arcsinhCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function arcsinhCo34:", "cannot use NaN as X input of arcsinh", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, b, real, imag;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &a);
  real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_X), &b);

  // arcsinh(z) = ln(z + sqrt(z² + 1))
  // calculate z²   real part
  realIcMultiply(&b, &b, &real);
  realIcChangeSign(&real);
  realIcFMA(&a, &a, &real, &real);

  // calculate z²   imaginary part
  realIcMultiply(&a, &b, &imag);
  realIcMultiply(&imag, const_2, &imag);

  // calculate z² + 1
  realIcAdd(&real, const_1, &real);

  // calculate sqrt(z² + 1)
  realIcRectangularToPolar(&real, &imag, &real, &imag);
  realIcSquareRoot(&real, &real);
  realIcMultiply(&imag, const_0_5, &imag);
  realIcPolarToRectangular(&real, &imag, &real, &imag);

  // calculate z + sqrt(z² + 1)
  realIcAdd(&a, &real, &real);
  realIcAdd(&b, &imag, &imag);

  // calculate ln(z + sqtr(z² + 1))
  realIcRectangularToPolar(&real, &imag, &a, &b);
  WP34S_Ln(&a, &a);

  realIcToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
  realIcToReal34(&b, REGISTER_IMAG34_DATA(REGISTER_X));
}
