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
    longInteger_t nn0, nn1, nn2;

    longIntegerInit(nn0);
    longIntegerInit(nn1);
    longIntegerInit(nn2);

    if(longIntegerIsZero(value)) {
      uIntToLongInteger(0, nn1);
    }
    else {
      // n0 = value / 2 + 1;
      longIntegerDivideUInt(value, 2, nn0);
      longIntegerAddUInt(nn0, 1, nn0);

      // n1 = value / n0 + n0 / 2;
      longIntegerDivide(value, nn0, nn1);
      longIntegerDivideUInt(nn0, 2, nn2);
      longIntegerAdd(nn1, nn2, nn1);
      while(longIntegerCompare(nn0, nn1) > 0) {
        //n0 = n1;
        longIntegerToLongInteger(nn1, nn0);

        //n1 = (n0 + value / n0) / 2;
        longIntegerDivide(value, nn0, nn1);
        longIntegerAdd(nn1, nn0, nn1);
        longIntegerDivideUInt(nn1, 2, nn1);
      }

      // n0 = n1 * n1;
      longIntegerMultiply(nn1, nn1, nn0);
      if(longIntegerCompare(nn0, value) > 0) {
        longIntegerSubtractUInt(nn1, 1, nn1);
      }

      if(longIntegerCompare(nn0, value) == 0) {
        convertLongIntegerToLongIntegerRegister(nn1, REGISTER_X);
        longIntegerFree(value);
        longIntegerFree(nn2);
        longIntegerFree(nn1);
        longIntegerFree(nn0);
        return;
      }
      else {
        convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
        real34SquareRoot(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
        convertRegister34To16(REGISTER_X);
      }
    }

    longIntegerFree(nn2);
    longIntegerFree(nn1);
    longIntegerFree(nn0);
  }
  else if(getFlag(FLAG_CPXRES)) { // Negative value
    real34_t real34;

    convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &real34);
    reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
    real34SetPositiveSign(&real34);
    real34SquareRoot(&real34, REGISTER_IMAG34_DATA(REGISTER_X));
    real34Zero(REGISTER_REAL34_DATA(REGISTER_X));
    convertRegister34To16(REGISTER_X);
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

  convertRegister16To34(REGISTER_X);

  if(real34IsPositive(REGISTER_REAL34_DATA(REGISTER_X))) {
    real34SquareRoot(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
    convertRegister34To16(REGISTER_X);
  }
  else if(getFlag(FLAG_CPXRES)) {
    real34_t temp;

    real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &temp);
    real34SetPositiveSign(&temp);
    real34SquareRoot(&temp, &temp);

    reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
    real34Zero(REGISTER_REAL34_DATA(REGISTER_X));
    real34Copy(&temp, REGISTER_IMAG34_DATA(REGISTER_X));
    convertRegister34To16(REGISTER_X);
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

  real34_t magnitude34, theta34;

  convertRegister16To34(REGISTER_X);
  real34RectangularToPolar(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X), &magnitude34, &theta34);
  real34SquareRoot(&magnitude34, &magnitude34);
  real34Multiply(&theta34, const34_0_5, &theta34);
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  real34PolarToRectangular(&magnitude34, &theta34, REGISTER_REAL34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X)); // theta34 in RADIAN
  convertRegister34To16(REGISTER_X);
}



void sqrtAn16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function sqrtAn16:", "cannot use NaN as X input of sqrt", NULL, NULL);
    #endif
    return;
  }

  setRegisterDataType(REGISTER_X, dtReal16, TAG_NONE);
  convertRegister16To34(REGISTER_X);

  if(real34IsPositive(REGISTER_REAL34_DATA(REGISTER_X))) {
    real34SquareRoot(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
    convertRegister34To16(REGISTER_X);
  }
  else if(getFlag(FLAG_CPXRES)) {
    real34_t temp;

    real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &temp);
    real34SetPositiveSign(&temp);
    real34SquareRoot(&temp, &temp);

    reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
    real34Zero(REGISTER_REAL34_DATA(REGISTER_X));
    real34Copy(&temp, REGISTER_IMAG34_DATA(REGISTER_X));
    convertRegister34To16(REGISTER_X);
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

  if(real34IsPositive(REGISTER_REAL34_DATA(REGISTER_X))) {
    real34SquareRoot(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
  }
  else if(getFlag(FLAG_CPXRES)) {
    real34_t temp;

    real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &temp);
    real34SetPositiveSign(&temp);
    real34SquareRoot(&temp, &temp);

    reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
    real34Zero(REGISTER_REAL34_DATA(REGISTER_X));
    real34Copy(&temp, REGISTER_IMAG34_DATA(REGISTER_X));
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

  real34_t magnitude34, theta34;

  real34RectangularToPolar(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X), &magnitude34, &theta34);
  real34SquareRoot(&magnitude34, &magnitude34);
  real34Multiply(&theta34, const34_0_5, &theta34);
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  real34PolarToRectangular(&magnitude34, &theta34, REGISTER_REAL34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X)); // theta34 in RADIAN
}



void sqrtAn34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function sqrtAn34:", "cannot use NaN as X input of sqrt", NULL, NULL);
    #endif
    return;
  }

  setRegisterDataType(REGISTER_X, dtReal34, TAG_NONE);

  if(real34IsPositive(REGISTER_REAL34_DATA(REGISTER_X))) {
    real34SquareRoot(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
  }
  else if(getFlag(FLAG_CPXRES)) {
    real34_t temp;

    real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &temp);
    real34SetPositiveSign(&temp);
    real34SquareRoot(&temp, &temp);

    reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
    real34Zero(REGISTER_REAL34_DATA(REGISTER_X));
    real34Copy(&temp, REGISTER_IMAG34_DATA(REGISTER_X));
  }
  else {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, STD_SQUARE_ROOT STD_x_UNDER_ROOT " doesn't work on a negative real when flag I is not set!");
      showInfoDialog("In function sqrtAn34:", errorMessage, NULL, NULL);
    #endif
  }
}
