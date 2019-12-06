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
 * \file arccos.c
 ***********************************************/

#include "wp43s.h"



void (* const arccos[12])(void) = {
// regX ==> 1            2           3           4            5            6            7            8           9           10            11          12
//          Long integer Real16      Complex16   Angle16      Time         Date         String       Real16 mat  Complex16 m Short integer Real34      Complex34
            arccosLonI,  arccosRe16, arccosCo16, arccosError, arccosError, arccosError, arccosError, arccosRm16, arccosCm16, arccosError,  arccosRe34, arccosCo34
};



/********************************************//**
 * \brief Data type error in arccos
 *
 * \param void
 * \return void
 ***********************************************/
void arccosError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate arccos for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnArccos:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and arccos(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnArccos(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  arccos[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



void arccosLonI(void) {
  real39_t x;

  convertLongIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  if(real39CompareAbsGreaterThan(&x, const_1)) {
    if(getFlag(FLAG_CPXRES)) {
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      real34Zero(REGISTER_IMAG34_DATA(REGISTER_X));
      arccosCo34();
      convertRegister34To16(REGISTER_X);
      return;
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function arccosLonI:", "|X| > 1", "and CPXRES is not set!", NULL);
      #endif
      return;
    }
  }

  reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, currentAngularMode);

  if(realIsZero(&x)) {
    realToReal16(const_0_5, REGISTER_REAL16_DATA(REGISTER_X));
    convertAngle16FromTo(REGISTER_REAL16_DATA(REGISTER_X), AM_MULTPI, currentAngularMode);
  }
  else {
    if(realIsNegative(&x)) {
      realToReal16(const_1, REGISTER_REAL16_DATA(REGISTER_X));
      convertAngle16FromTo(REGISTER_REAL16_DATA(REGISTER_X), AM_MULTPI, currentAngularMode);
    }
    else{
      real34Zero(REGISTER_REAL34_DATA(REGISTER_X));
    }
  }
}



void arccosRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function arccosRe16:", "cannot use NaN as X input of arccos", NULL, NULL);
    #endif
    return;
  }

  real39_t x;

  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &x);
  setRegisterAngularMode(REGISTER_X, currentAngularMode);

  if(real39CompareAbsGreaterThan(&x, const_1)) {
    if(getFlag(FLAG_CPXRES)) {
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      real34Zero(REGISTER_IMAG34_DATA(REGISTER_X));
      arccosCo34();
      convertRegister34To16(REGISTER_X);
      return;
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function arccosRe16:", "|X| > 1", "and CPXRES is not set!", NULL);
      #endif
      return;
    }
  }
  WP34S_Acos(&x, &x);
  convertAngle39FromTo(&x, AM_RADIAN, currentAngularMode);
  realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));

  if(currentAngularMode == AM_DMS) {
    checkDms16(REGISTER_REAL16_DATA(REGISTER_X));
  }
}



void arccosCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function arccosCo16:", "cannot use NaN as X input of arccos", NULL, NULL);
    #endif
    return;
  }

  real39_t a, b, real, imag;

  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &a);
  real16ToReal(REGISTER_IMAG16_DATA(REGISTER_X), &b);

  // arccos(z) = -i.ln(z + sqrt(z² - 1))
  // calculate z²   real part
  realMultiply(&b, &b, &real, &ctxtReal39);
  realChangeSign(&real);
  realFMA(&a, &a, &real, &real, &ctxtReal39);

  // calculate z²   imaginary part
  realMultiply(&a, &b, &imag, &ctxtReal39);
  realMultiply(&imag, const_2, &imag, &ctxtReal39);

  // calculate z² - 1
  realSubtract(&real, const_1, &real, &ctxtReal39);

  // calculate sqrt(z² - 1)
  real39RectangularToPolar(&real, &imag, &real, &imag);
  realSquareRoot(&real, &real, &ctxtReal39);
  realMultiply(&imag, const_0_5, &imag, &ctxtReal39);
  real39PolarToRectangular(&real, &imag, &real, &imag);

  // calculate z + sqrt(z² - 1)
  realAdd(&a, &real, &real, &ctxtReal39);
  realAdd(&b, &imag, &imag, &ctxtReal39);

  // calculate ln(z + sqtr(z² - 1))
  real39RectangularToPolar(&real, &imag, &a, &b);
  WP34S_Ln(&a, &a);

  // calculate = -i.ln(z + sqtr(z² - 1))
  realChangeSign(&a);

  realToReal16(&b, REGISTER_REAL16_DATA(REGISTER_X));
  realToReal16(&a, REGISTER_IMAG16_DATA(REGISTER_X));
}



void arccosRm16(void) {
  fnToBeCoded();
}



void arccosCm16(void) {
  fnToBeCoded();
}



void arccosRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function arccosRe34:", "cannot use NaN as X input of arccos", NULL, NULL);
    #endif
    return;
  }

  real39_t x;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
  setRegisterAngularMode(REGISTER_X, currentAngularMode);

  if(real39CompareAbsGreaterThan(&x, const_1)) {
    if(getFlag(FLAG_CPXRES)) {
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      real34Zero(REGISTER_IMAG34_DATA(REGISTER_X));
      arccosCo34();
      return;
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function arccosRe34:", "|X| > 1", "and CPXRES is not set!", NULL);
      #endif
      return;
    }
  }
  WP34S_Acos(&x, &x);
  convertAngle39FromTo(&x, AM_RADIAN, currentAngularMode);
  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));

  if(currentAngularMode == AM_DMS) {
    checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
  }
}



void arccosCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function arccosCo34:", "cannot use NaN as X input of arccos", NULL, NULL);
    #endif
    return;
  }

  real39_t a, b, real, imag;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &a);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &b);

  // arccos(z) = -i.ln(z + sqrt(z² - 1))
  // calculate z²   real part
  realMultiply(&b, &b, &real, &ctxtReal39);
  realChangeSign(&real);
  realFMA(&a, &a, &real, &real, &ctxtReal39);

  // calculate z²   imaginary part
  realMultiply(&a, &b, &imag, &ctxtReal39);
  realMultiply(&imag, const_2, &imag, &ctxtReal39);

  // calculate z² - 1
  realSubtract(&real, const_1, &real, &ctxtReal39);

  // calculate sqrt(z² - 1)
  real39RectangularToPolar(&real, &imag, &real, &imag);
  realSquareRoot(&real, &real, &ctxtReal39);
  realMultiply(&imag, const_0_5, &imag, &ctxtReal39);
  real39PolarToRectangular(&real, &imag, &real, &imag);

  // calculate z + sqrt(z² - 1)
  realAdd(&a, &real, &real, &ctxtReal39);
  realAdd(&b, &imag, &imag, &ctxtReal39);

  // calculate ln(z + sqtr(z² - 1))
  real39RectangularToPolar(&real, &imag, &a, &b);
  WP34S_Ln(&a, &a);

  // calculate = -i.ln(z + sqtr(z² - 1))
  realChangeSign(&a);

  realToReal34(&b, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&a, REGISTER_IMAG34_DATA(REGISTER_X));
}
