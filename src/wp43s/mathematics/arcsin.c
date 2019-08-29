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



void (* const arcsin[13])(void) = {
// regX ==> 1            2           3           4           5            6            7            8           9           10            11          12          13
//          Long integer Real16      Complex16   Angle16     Time         Date         String       Real16 mat  Complex16 m Short integer Real34      Complex34   Angle34
            arcsinLonI,  arcsinRe16, arcsinCo16, arcsinRe16, arcsinError, arcsinError, arcsinError, arcsinRm16, arcsinCm16, arcsinError,  arcsinRe34, arcsinCo34, arcsinRe34
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



/**********************************************************************
 * In all the functions below:
 * if X is a number then X = a + ib
 * The variables a and b are used for intermediate calculations
 ***********************************************************************/

void arcsinLonI(void) {
  realIc_t a;

  convertLongIntegerRegisterToRealIc(REGISTER_X, &a);
  if(realIcCompareAbsGreaterThan(&a, const_1)) {
    if(getFlag(FLAG_CPXRES)) {
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
      realIcToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
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

  reallocateRegister(REGISTER_X, dtAngle16, REAL16_SIZE, currentAngularMode);

  if(realIcIsZero(&a)) {
    real16Zero(REGISTER_REAL16_DATA(REGISTER_X));
  }
  else {
    realIcToReal16(const_0_5, REGISTER_REAL16_DATA(REGISTER_X));
    if(realIcIsNegative(&a)) {
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

  realIc_t a;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &a);
  setRegisterDataType(REGISTER_X, dtAngle16, currentAngularMode);

  if(realIcCompareAbsGreaterThan(&a, const_1)) {
    if(getFlag(FLAG_CPXRES)) {
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
      realIcToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
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

  WP34S_asin(&a, &a);
  convertAngleIcFromTo(&a, AM_RADIAN, currentAngularMode);
  realIcToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));

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

  realIc_t a, b, real, imag, magnitude, theta;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &a);
  real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_X), &b);

  // arcsin(z) = -i.ln(iz + sqrt(1 - z²))
  // calculate z²   real part
  realIcMultiply(&b, &b, &real);
  realIcChangeSign(&real);
  realIcFMA(&a, &a, &real, &real);

  // calculate z²   imaginary part
  realIcMultiply(&a, &b, &imag);
  realIcMultiply(&imag, const_2, &imag);

  // calculate 1 - z²
  realIcSubtract(const_1, &real, &real);
  realIcChangeSign(&imag);

  // calculate sqrt(1 - z²)
  realIcRectangularToPolar(&real, &imag, &magnitude, &theta);
  realIcSquareRoot(&magnitude, &magnitude);
  realIcMultiply(&theta, const_0_5, &theta);
  realIcPolarToRectangular(&magnitude, &theta, &real, &imag);

  // calculate iz + sqrt(1 - z²)
  realIcChangeSign(&b);
  realIcAdd(&real, &b, &real);
  realIcAdd(&imag, &a, &imag);

  // calculate ln(iz + sqrt(1 - z²))
  realIcRectangularToPolar(&real, &imag, &real, &a);
  WP34S_Ln(&real, &b);

  // calculate = -i.ln(iz + sqrt(1 - z²))
  realIcChangeSign(&b);

  realIcToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
  realIcToReal16(&b, REGISTER_IMAG16_DATA(REGISTER_X));
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

  realIc_t a;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &a);
  setRegisterDataType(REGISTER_X, dtAngle34, currentAngularMode);

  if(realIcCompareAbsGreaterThan(&a, const_1)) {
    if(getFlag(FLAG_CPXRES)) {
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
      realIcToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
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

  WP34S_asin(&a, &a);
  convertAngleIcFromTo(&a, AM_RADIAN, currentAngularMode);
  realIcToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));

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

  realIc_t a, b, real, imag;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &a);
  real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_X), &b);

  // arcsin(z) = -i.ln(iz + sqrt(1 - z²))
  // calculate z²   real part
  realIcMultiply(&b, &b, &real);
  realIcChangeSign(&real);
  realIcFMA(&a, &a, &real, &real);

  // calculate z²   imaginary part
  realIcMultiply(&a, &b, &imag);
  realIcMultiply(&imag, const_2, &imag);

  // calculate 1 - z²
  realIcSubtract(const_1, &real, &real);
  realIcChangeSign(&imag);

  // calculate sqrt(1 - z²)
  realIcRectangularToPolar(&real, &imag, &real, &imag);
  realIcSquareRoot(&real, &real);
  realIcMultiply(&imag, const_0_5, &imag);
  realIcPolarToRectangular(&real, &imag, &real, &imag);

  // calculate iz + sqrt(1 - z²)
  realIcChangeSign(&b);
  realIcAdd(&real, &b, &real);
  realIcAdd(&imag, &a, &imag);

  // calculate ln(iz + sqrt(1 - z²))
  realIcRectangularToPolar(&real, &imag, &a, &b);
  WP34S_Ln(&a, &a);

  // calculate = -i.ln(iz + sqrt(1 - z²))
  realIcChangeSign(&a);

  realIcToReal34(&b, REGISTER_REAL34_DATA(REGISTER_X));
  realIcToReal34(&a, REGISTER_IMAG34_DATA(REGISTER_X));
}
