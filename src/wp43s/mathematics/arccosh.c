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
 * \file arccosh.c
 ***********************************************/

#include "wp43s.h"



void (* const arccosh[13])(void) = {
// regX ==> 1             2            3            4            5             6             7             8            9            10             11           12           13
//          Long integer  Real16       Complex16    Angle16      Time          Date          String        Real16 mat   Complex16 m  Short integer  Real34       Complex34    Angle34
            arccoshLonI,  arccoshRe16, arccoshCo16, arccoshRe16, arccoshError, arccoshError, arccoshError, arccoshRm16, arccoshCm16, arccoshError,  arccoshRe34, arccoshCo34, arccoshRe34
};



/********************************************//**
 * \brief Data type error in arccosh
 *
 * \param void
 * \return void
 ***********************************************/
void arccoshError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate arccosh for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnArccos:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and arccosh(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnArccosh(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  arccosh[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



/**********************************************************************
 * In all the functions below:
 * if X is a number then X = a + ib
 * The variables a and b are used for intermediate calculations
 ***********************************************************************/

void arccoshLonI(void) {
  realIc_t a, aSquared;

  convertLongIntegerRegisterToRealIc(REGISTER_X, &a);
  if(realIcCompareLessThan(&a, const_1)) {
    if(getFlag(FLAG_CPXRES)) {
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
      realIcToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
      real34Zero(REGISTER_IMAG34_DATA(REGISTER_X));
      arccoshCo34();
      convertRegister34To16(REGISTER_X);
      return;
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function arccoshLonI:", "X < 1", "and CPXRES is not set!", NULL);
      #endif
      return;
    }
  }

  reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, currentAngularMode);

  // arccosh(a) = ln(a + sqrt(a² - 1))
  realIcMultiply(&a, &a, &aSquared);
  realIcSubtract(&aSquared, const_1, &aSquared);
  realIcSquareRoot(&aSquared, &aSquared);
  realIcAdd(&aSquared, &a, &a);
  WP34S_Ln(&a, &a);

  realIcToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
}



void arccoshRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function arccoshRe16:", "cannot use NaN as X input of arccosh", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, aSquared;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &a);
  setRegisterDataType(REGISTER_X, dtReal16, TAG_NONE);

  if(realIcCompareLessThan(&a, const_1)) {
    if(getFlag(FLAG_CPXRES)) {
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
      realIcToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
      real34Zero(REGISTER_IMAG34_DATA(REGISTER_X));
      arccoshCo34();
      convertRegister34To16(REGISTER_X);
      return;
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function arccoshRe16:", "X < 1", "and CPXRES is not set!", NULL);
      #endif
      return;
    }
  }

  // arccosh(a) = ln(a + sqrt(a² - 1))
  realIcMultiply(&a, &a, &aSquared);
  realIcSubtract(&aSquared, const_1, &aSquared);
  realIcSquareRoot(&aSquared, &aSquared);
  realIcAdd(&aSquared, &a, &a);
  WP34S_Ln(&a, &a);

  realIcToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
}



void arccoshCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function arccoshCo16:", "cannot use NaN as X input of arccosh", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, b, real, imag;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &a);
  real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_X), &b);

  // arccosh(z) = ln(z + sqrt(z² - 1))
  // calculate z²   real part
  realIcMultiply(&b, &b, &real);
  realIcChangeSign(&real);
  realIcFMA(&a, &a, &real, &real);

  // calculate z²   imaginary part
  realIcMultiply(&a, &b, &imag);
  realIcMultiply(&imag, const_2, &imag);

  // calculate z² - 1
  realIcSubtract(&real, const_1, &real);

  // calculate sqrt(z² - 1)
  realIcRectangularToPolar(&real, &imag, &real, &imag);
  realIcSquareRoot(&real, &real);
  realIcMultiply(&imag, const_0_5, &imag);
  realIcPolarToRectangular(&real, &imag, &real, &imag);

  // calculate z + sqrt(z² - 1)
  realIcAdd(&a, &real, &real);
  realIcAdd(&b, &imag, &imag);

  // calculate ln(z + sqtr(z² - 1))
  realIcRectangularToPolar(&real, &imag, &a, &b);
  WP34S_Ln(&a, &a);

  realIcToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
  realIcToReal16(&b, REGISTER_IMAG16_DATA(REGISTER_X));
}



void arccoshRm16(void) {
  fnToBeCoded();
}



void arccoshCm16(void) {
  fnToBeCoded();
}



void arccoshRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function arccoshRe34:", "cannot use NaN as X input of arccosh", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, aSquared;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &a);
  setRegisterDataType(REGISTER_X, dtReal34, TAG_NONE);

  if(realIcCompareLessThan(&a, const_1)) {
    if(getFlag(FLAG_CPXRES)) {
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
      realIcToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
      real34Zero(REGISTER_IMAG34_DATA(REGISTER_X));
      arccoshCo34();
      return;
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function arccoshRe34:", "X < 1", "and CPXRES is not set!", NULL);
      #endif
      return;
    }
  }

  // arccosh(a) = ln(a + sqrt(a² - 1))
  realIcMultiply(&a, &a, &aSquared);
  realIcSubtract(&aSquared, const_1, &aSquared);
  realIcSquareRoot(&aSquared, &aSquared);
  realIcAdd(&aSquared, &a, &a);
  WP34S_Ln(&a, &a);

  realIcToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
}



void arccoshCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function arccoshCo34:", "cannot use NaN as X input of arccosh", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, b, real, imag;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &a);
  real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_X), &b);

  // arccosh(z) = ln(z + sqrt(z² - 1))
  // calculate z²   real part
  realIcMultiply(&b, &b, &real);
  realIcChangeSign(&real);
  realIcFMA(&a, &a, &real, &real);

  // calculate z²   imaginary part
  realIcMultiply(&a, &b, &imag);
  realIcMultiply(&imag, const_2, &imag);

  // calculate z² - 1
  realIcSubtract(&real, const_1, &real);

  // calculate sqrt(z² - 1)
  realIcRectangularToPolar(&real, &imag, &real, &imag);
  realIcSquareRoot(&real, &real);
  realIcMultiply(&imag, const_0_5, &imag);
  realIcPolarToRectangular(&real, &imag, &real, &imag);

  // calculate z + sqrt(z² - 1)
  realIcAdd(&a, &real, &real);
  realIcAdd(&b, &imag, &imag);

  // calculate ln(z + sqtr(z² - 1))
  realIcRectangularToPolar(&real, &imag, &a, &b);
  WP34S_Ln(&a, &a);

  realIcToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
  realIcToReal34(&b, REGISTER_IMAG34_DATA(REGISTER_X));
}
