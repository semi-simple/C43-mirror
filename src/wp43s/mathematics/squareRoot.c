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



void (* const Sqrt[12])(void) = {
// regX ==> 1            2         3         4          5          6          7          8          9           10            11        12
//          Long integer Real16    Complex16 Angle16    Time       Date       String     Real16 mat Complex16 m Short integer Real34    Complex34
            sqrtLonI,    sqrtRe16, sqrtCo16, sqrtError, sqrtError, sqrtError, sqrtError, sqrtRm16,  sqrtCm16,   sqrtShoI,     sqrtRe34, sqrtCo34
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
      real39_t a;

      convertLongIntegerRegisterToReal(REGISTER_X, &a, &ctxtReal39);
      realSquareRoot(&a, &a, &ctxtReal39);
      reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);
      realToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
    }
  }
  else if(getFlag(FLAG_CPXRES)) { // Negative value
    real39_t a;

    convertLongIntegerRegisterToReal(REGISTER_X, &a, &ctxtReal39);
    reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);
    realSetPositiveSign(&a);
    realSquareRoot(&a, &a, &ctxtReal39);
    real16Zero(REGISTER_REAL16_DATA(REGISTER_X));
    realToReal16(&a, REGISTER_IMAG16_DATA(REGISTER_X));
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

  if(real16IsInfinite(REGISTER_REAL16_DATA(REGISTER_X)) && !getFlag(FLAG_DANGER)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function sqrtRe16:", "cannot use " STD_PLUS_MINUS STD_INFINITY " as X input of sqrt when flag D is not set", NULL, NULL);
    #endif
    return;
  }

  real39_t a;

  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &a);

  if(real16IsPositive(REGISTER_REAL16_DATA(REGISTER_X))) {
    realSquareRoot(&a, &a, &ctxtReal39);
    realToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
    setRegisterAngularMode(REGISTER_X, AM_NONE);
  }
  else if(getFlag(FLAG_CPXRES)) {
    reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);
    realSetPositiveSign(&a);
    realSquareRoot(&a, &a, &ctxtReal39);
    real16Zero(REGISTER_REAL16_DATA(REGISTER_X));
    realToReal16(&a, REGISTER_IMAG16_DATA(REGISTER_X));
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

  real39_t a, b;

  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &a);
  real16ToReal(REGISTER_IMAG16_DATA(REGISTER_X), &b);

  real39RectangularToPolar(&a, &b, &a, &b);
  realSquareRoot(&a, &a, &ctxtReal39);
  realMultiply(&b, const_0_5, &b, &ctxtReal39);
  real39PolarToRectangular(&a, &b, &a, &b);

  realToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
  realToReal16(&b, REGISTER_IMAG16_DATA(REGISTER_X));
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

  if(real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_X)) && !getFlag(FLAG_DANGER)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function sqrtRe34:", "cannot use " STD_PLUS_MINUS STD_INFINITY " as X input of sqrt when flag D is not set", NULL, NULL);
    #endif
    return;
  }

  real39_t a;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &a);

  if(real34IsPositive(REGISTER_REAL34_DATA(REGISTER_X))) {
    realSquareRoot(&a, &a, &ctxtReal39);
    realToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
    setRegisterAngularMode(REGISTER_X, AM_NONE);
  }
  else if(getFlag(FLAG_CPXRES)) {
    reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
    realSetPositiveSign(&a);
    realSquareRoot(&a, &a, &ctxtReal39);
    real34Zero(REGISTER_REAL34_DATA(REGISTER_X));
    realToReal34(&a, REGISTER_IMAG34_DATA(REGISTER_X));
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

  real39_t a, b;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &a);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &b);

  real39RectangularToPolar(&a, &b, &a, &b);
  realSquareRoot(&a, &a, &ctxtReal39);
  realMultiply(&b, const_0_5, &b, &ctxtReal39);
  real39PolarToRectangular(&a, &b, &a, &b);

  realToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&b, REGISTER_IMAG34_DATA(REGISTER_X));
}
