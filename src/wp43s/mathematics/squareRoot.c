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
 * \file sqrt.c
 ***********************************************/

#include "wp43s.h"



void (* const Sqrt[13])(void) = {
// regX ==> 1            2         3         4         5          6          7          8          9           10            11        12        13
//          Long integer Real16    Complex16 Angle16   Time       Date       String     Real16 mat Complex16 m Short integer Real34    Complex34 Angle34
            sqrtLonI,    sqrtRe16, sqrtCo16, sqrtAn16, sqrtError, sqrtError, sqrtError, sqrtRm16,  sqrtCm16,   sqrtShoI,     sqrtRe34, sqrtCo34, sqrtAn34
};



/********************************************//**
 * \brief Data type error in sqrt
 *
 * \param void
 * \return void
 ***********************************************/
void sqrtError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate sqrt for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnSquareRoot:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and sqrt(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnSquareRoot(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  Sqrt[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



void sqrtLonI(void) {
  longInteger_t value;

  convertLongIntegerRegisterToLongInteger(REGISTER_X, value);

  if(longIntegerIsPositiveOrZero(value)) {
    if(longIntegerPerfectSquare(value)) { // value is a perfect square
      longIntegerSquareRoot(value, value);
      convertLongIntegerToLongIntegerRegister(value, REGISTER_X);
    }
    else {
      realIc_t a;

      convertLongIntegerRegisterToRealIc(REGISTER_X, &a);
      realIcSquareRoot(&a, &a);
      reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, TAG_NONE);
      realIcToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
    }
  }
  else if(getFlag(FLAG_CPXRES)) { // Negative value
    realIc_t a;

    convertLongIntegerRegisterToRealIc(REGISTER_X, &a);
    reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, TAG_NONE);
    realIcSetPositiveSign(&a);
    realIcSquareRoot(&a, &a);
    real16Zero(REGISTER_REAL16_DATA(REGISTER_X));
    realIcToReal16(&a, REGISTER_IMAG16_DATA(REGISTER_X));
  }
  else {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, STD_SQUARE_ROOT STD_x_UNDER_ROOT " doesn't work on a negative long integer when flag I is not set!");
      showInfoDialog("In function sqrtLonI:", errorMessage, NULL, NULL);
    #endif
  }

  longIntegerFree(value);
}



void sqrtRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function sqrtRe16:", "cannot use NaN as X input of sqrt", NULL, NULL);
    #endif
    return;
  }

  realIc_t a;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &a);

  if(real16IsPositive(REGISTER_REAL16_DATA(REGISTER_X))) {
    realIcSquareRoot(&a, &a);
    realIcToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
  }
  else if(getFlag(FLAG_CPXRES)) {
    reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, TAG_NONE);
    realIcSetPositiveSign(&a);
    realIcSquareRoot(&a, &a);
    real16Zero(REGISTER_REAL16_DATA(REGISTER_X));
    realIcToReal16(&a, REGISTER_IMAG16_DATA(REGISTER_X));
  }
  else {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, STD_SQUARE_ROOT STD_x_UNDER_ROOT " doesn't work on a negative real when flag I is not set!");
      showInfoDialog("In function sqrtRe16:", errorMessage, NULL, NULL);
    #endif
  }
}



void sqrtCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function sqrtCo16:", "cannot use NaN as X input of sqrt", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, b;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &a);
  real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_X), &b);

  realIcRectangularToPolar(&a, &b, &a, &b);
  realIcSquareRoot(&a, &a);
  realIcMultiply(&b, const_0_5, &b);
  realIcPolarToRectangular(&a, &b, &a, &b);

  realIcToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
  realIcToReal16(&b, REGISTER_IMAG16_DATA(REGISTER_X));
}



void sqrtAn16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function sqrtAn16:", "cannot use NaN as X input of sqrt", NULL, NULL);
    #endif
    return;
  }

  realIc_t a;

  setRegisterDataType(REGISTER_X, dtReal16, TAG_NONE);
  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &a);

  if(real16IsPositive(REGISTER_REAL16_DATA(REGISTER_X))) {
    realIcSquareRoot(&a, &a);
    realIcToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
  }
  else if(getFlag(FLAG_CPXRES)) {
    reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, TAG_NONE);
    realIcSetPositiveSign(&a);
    realIcSquareRoot(&a, &a);
    real16Zero(REGISTER_REAL16_DATA(REGISTER_X));
    realIcToReal16(&a, REGISTER_IMAG16_DATA(REGISTER_X));
  }
  else {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, STD_SQUARE_ROOT STD_x_UNDER_ROOT " doesn't work on a negative real when flag I is not set!");
      showInfoDialog("In function sqrtAn16:", errorMessage, NULL, NULL);
    #endif
  }
}



void sqrtRm16(void) {
  fnToBeCoded();
}



void sqrtCm16(void) {
  fnToBeCoded();
}



void sqrtShoI(void) {
  *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = WP34S_intSqrt(*(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)));
}



void sqrtRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function sqrtRe34:", "cannot use NaN as X input of sqrt", NULL, NULL);
    #endif
    return;
  }

  realIc_t a;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &a);

  if(real34IsPositive(REGISTER_REAL34_DATA(REGISTER_X))) {
    realIcSquareRoot(&a, &a);
    realIcToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else if(getFlag(FLAG_CPXRES)) {
    reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
    realIcSetPositiveSign(&a);
    realIcSquareRoot(&a, &a);
    real34Zero(REGISTER_REAL34_DATA(REGISTER_X));
    realIcToReal34(&a, REGISTER_IMAG34_DATA(REGISTER_X));
  }
  else {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, STD_SQUARE_ROOT STD_x_UNDER_ROOT " doesn't work on a negative real when flag I is not set!");
      showInfoDialog("In function sqrtRe34:", errorMessage, NULL, NULL);
    #endif
  }
}



void sqrtCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function sqrtCo34:", "cannot use NaN as X input of sqrt", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, b;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &a);
  real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_X), &b);

  realIcRectangularToPolar(&a, &b, &a, &b);
  realIcSquareRoot(&a, &a);
  realIcMultiply(&b, const_0_5, &b);
  realIcPolarToRectangular(&a, &b, &a, &b);

  realIcToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
  realIcToReal34(&b, REGISTER_IMAG34_DATA(REGISTER_X));
}



void sqrtAn34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function sqrtAn34:", "cannot use NaN as X input of sqrt", NULL, NULL);
    #endif
    return;
  }

  realIc_t a;

  setRegisterDataType(REGISTER_X, dtReal34, TAG_NONE);
  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &a);

  if(real34IsPositive(REGISTER_REAL34_DATA(REGISTER_X))) {
    realIcSquareRoot(&a, &a);
    realIcToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else if(getFlag(FLAG_CPXRES)) {
    reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
    realIcSetPositiveSign(&a);
    realIcSquareRoot(&a, &a);
    real34Zero(REGISTER_REAL34_DATA(REGISTER_X));
    realIcToReal34(&a, REGISTER_IMAG34_DATA(REGISTER_X));
  }
  else {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, STD_SQUARE_ROOT STD_x_UNDER_ROOT " doesn't work on a negative real when flag I is not set!");
      showInfoDialog("In function sqrtAn34:", errorMessage, NULL, NULL);
    #endif
  }
}
