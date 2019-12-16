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
 * \file arcsin.c
 ***********************************************/

#include "wp43s.h"



void (* const arcsin[12])(void) = {
// regX ==> 1            2           3           4            5            6            7            8           9           10            11          12
//          Long integer Real16      Complex16   Angle16      Time         Date         String       Real16 mat  Complex16 m Short integer Real34      Complex34
            arcsinLonI,  arcsinRe16, arcsinCo16, arcsinError, arcsinError, arcsinError, arcsinError, arcsinRm16, arcsinCm16, arcsinError,  arcsinRe34, arcsinCo34
};



/********************************************//**
 * \brief Data type error in arcsin
 *
 * \param void
 * \return void
 ***********************************************/
void arcsinError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate arcsin for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnArcsin:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and arcsin(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnArcsin(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  arcsin[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



void arcsinLonI(void) {
  real39_t x;

  convertLongIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  if(realCompareAbsGreaterThan(&x, const_1)) {
    if(getFlag(FLAG_CPXRES)) {
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      real34Zero(REGISTER_IMAG34_DATA(REGISTER_X));
      arcsinCo34();
      convertRegister34To16(REGISTER_X);
      return;
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function arcsinLonI:", "|X| > 1", "and CPXRES is not set!", NULL);
      #endif
      return;
    }
  }

  reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, currentAngularMode);

  if(realIsZero(&x)) {
    real16Zero(REGISTER_REAL16_DATA(REGISTER_X));
  }
  else {
    realToReal16(const_0_5, REGISTER_REAL16_DATA(REGISTER_X));
    if(realIsNegative(&x)) {
      real16ChangeSign(REGISTER_REAL16_DATA(REGISTER_X));
    }
    convertAngle16FromTo(REGISTER_REAL16_DATA(REGISTER_X), AM_MULTPI, currentAngularMode);
  }
}



void arcsinRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function arcsinRe16:", "cannot use NaN as X input of arcsin", NULL, NULL);
    #endif
    return;
  }

  real39_t x;

  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &x);
  setRegisterAngularMode(REGISTER_X, currentAngularMode);

  if(realCompareAbsGreaterThan(&x, const_1)) {
    if(getFlag(FLAG_CPXRES)) {
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      real34Zero(REGISTER_IMAG34_DATA(REGISTER_X));
      arcsinCo34();
      convertRegister34To16(REGISTER_X);
      return;
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function arcsinRe16:", "|X| > 1", "and CPXRES is not set!", NULL);
      #endif
      return;
    }
  }

  WP34S_Asin(&x, &x);
  convertAngle39FromTo(&x, AM_RADIAN, currentAngularMode);
  realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));

  if(currentAngularMode == AM_DMS) {
    checkDms16(REGISTER_REAL16_DATA(REGISTER_X));
  }
}



void arcsinCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function arcsinCo16:", "cannot use NaN as X input of arcsin", NULL, NULL);
    #endif
    return;
  }

  real39_t a, b, real, imag, magnitude, theta;

  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &a);
  real16ToReal(REGISTER_IMAG16_DATA(REGISTER_X), &b);

  // arcsin(z) = -i.ln(iz + sqrt(1 - z²))
  // calculate z²   real part
  realMultiply(&b, &b, &real, &ctxtReal39);
  realChangeSign(&real);
  realFMA(&a, &a, &real, &real, &ctxtReal39);

  // calculate z²   imaginary part
  realMultiply(&a, &b, &imag, &ctxtReal39);
  realMultiply(&imag, const_2, &imag, &ctxtReal39);

  // calculate 1 - z²
  realSubtract(const_1, &real, &real, &ctxtReal39);
  realChangeSign(&imag);

  // calculate sqrt(1 - z²)
  real39RectangularToPolar(&real, &imag, &magnitude, &theta);
  realSquareRoot(&magnitude, &magnitude, &ctxtReal39);
  realMultiply(&theta, const_0_5, &theta, &ctxtReal39);
  real39PolarToRectangular(&magnitude, &theta, &real, &imag);

  // calculate iz + sqrt(1 - z²)
  realChangeSign(&b);
  realAdd(&real, &b, &real, &ctxtReal39);
  realAdd(&imag, &a, &imag, &ctxtReal39);

  // calculate ln(iz + sqrt(1 - z²))
  real39RectangularToPolar(&real, &imag, &real, &a);
  WP34S_Ln(&real, &b);

  // calculate = -i.ln(iz + sqrt(1 - z²))
  realChangeSign(&b);

  realToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
  realToReal16(&b, REGISTER_IMAG16_DATA(REGISTER_X));
}



void arcsinRm16(void) {
  fnToBeCoded();
}



void arcsinCm16(void) {
  fnToBeCoded();
}



void arcsinRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function arcsinRe34:", "cannot use NaN as X input of arcsin", NULL, NULL);
    #endif
    return;
  }

  real39_t x;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
  setRegisterAngularMode(REGISTER_X, currentAngularMode);

  if(realCompareAbsGreaterThan(&x, const_1)) {
    if(getFlag(FLAG_CPXRES)) {
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      real34Zero(REGISTER_IMAG34_DATA(REGISTER_X));
      arcsinCo34();
      return;
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function arcsinRe34:", "|X| > 1", "and CPXRES is not set!", NULL);
      #endif
      return;
    }
  }

  WP34S_Asin(&x, &x);
  convertAngle39FromTo(&x, AM_RADIAN, currentAngularMode);
  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));

  if(currentAngularMode == AM_DMS) {
    checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
  }
}



void arcsinCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function arcsinCo34:", "cannot use NaN as X input of arcsin", NULL, NULL);
    #endif
    return;
  }

  real39_t a, b, real, imag;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &a);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &b);

  // arcsin(z) = -i.ln(iz + sqrt(1 - z²))
  // calculate z²   real part
  realMultiply(&b, &b, &real, &ctxtReal39);
  realChangeSign(&real);
  realFMA(&a, &a, &real, &real, &ctxtReal39);

  // calculate z²   imaginary part
  realMultiply(&a, &b, &imag, &ctxtReal39);
  realMultiply(&imag, const_2, &imag, &ctxtReal39);

  // calculate 1 - z²
  realSubtract(const_1, &real, &real, &ctxtReal39);
  realChangeSign(&imag);

  // calculate sqrt(1 - z²)
  real39RectangularToPolar(&real, &imag, &real, &imag);
  realSquareRoot(&real, &real, &ctxtReal39);
  realMultiply(&imag, const_0_5, &imag, &ctxtReal39);
  real39PolarToRectangular(&real, &imag, &real, &imag);

  // calculate iz + sqrt(1 - z²)
  realChangeSign(&b);
  realAdd(&real, &b, &real, &ctxtReal39);
  realAdd(&imag, &a, &imag, &ctxtReal39);

  // calculate ln(iz + sqrt(1 - z²))
  real39RectangularToPolar(&real, &imag, &a, &b);
  WP34S_Ln(&a, &a);

  // calculate = -i.ln(iz + sqrt(1 - z²))
  realChangeSign(&a);

  realToReal34(&b, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&a, REGISTER_IMAG34_DATA(REGISTER_X));
}
