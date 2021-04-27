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

#include "mathematics/squareRoot.h"

#include "constantPointers.h"
#include "debug.h"
#include "error.h"
#include "flags.h"
#include "fonts.h"
#include "integers.h"
#include "items.h"
#include "mathematics/toPolar.h"
#include "mathematics/toRect.h"
#include "matrix.h"
#include "registers.h"
#include "registerValueConversions.h"

#include "wp43s.h"



TO_QSPI void (* const Sqrt[NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS])(void) = {
// regX ==> 1            2         3         4          5          6          7          8           9             10
//          Long integer Real34    complex34 Time       Date       String     Real34 mat Complex34 m Short integer Config data
            sqrtLonI,    sqrtReal, sqrtCplx, sqrtError, sqrtError, sqrtError, sqrtRema,  sqrtCxma,   sqrtShoI,     sqrtError
};



/********************************************//**
 * \brief Data type error in sqrt
 *
 * \param void
 * \return void
 ***********************************************/
#if (EXTRA_INFO_ON_CALC_ERROR == 1)
void sqrtError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    sprintf(errorMessage, "cannot calculate sqrt for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    moreInfoOnError("In function fnSquareRoot:", errorMessage, NULL, NULL);
}
#endif // (EXTRA_INFO_ON_CALC_ERROR == 1)



/********************************************//**
 * \brief regX ==> regL and sqrt(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnSquareRoot(uint16_t unusedButMandatoryParameter) {
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
      real_t a;

      convertLongIntegerRegisterToReal(REGISTER_X, &a, &ctxtReal39);
      realSquareRoot(&a, &a, &ctxtReal39);
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
      realToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
    }
  }
  else if(getFlag(FLAG_CPXRES)) { // Negative value
    real_t a;

    convertLongIntegerRegisterToReal(REGISTER_X, &a, &ctxtReal39);
    reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
    realSetPositiveSign(&a);
    realSquareRoot(&a, &a, &ctxtReal39);
    real34Zero(REGISTER_REAL34_DATA(REGISTER_X));
    realToReal34(&a, REGISTER_IMAG34_DATA(REGISTER_X));
  }
  else {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, STD_SQUARE_ROOT STD_x_UNDER_ROOT " doesn't work on a negative long integer when flag I is not set!");
      moreInfoOnError("In function sqrtLonI:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }

  longIntegerFree(value);
}



void sqrtRema(void) {
  elementwiseRema(sqrtReal);
}



void sqrtCxma(void) {
  fnToBeCoded();
}



void sqrtShoI(void) {
  *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = WP34S_intSqrt(*(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)));
}



void sqrtReal(void) {
  if(real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_X)) && !getSystemFlag(FLAG_SPCRES)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      moreInfoOnError("In function sqrtReal:", "cannot use " STD_PLUS_MINUS STD_INFINITY " as X input of sqrt when flag D is not set", NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return;
  }

  real_t a;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &a);

  if(real34IsPositive(REGISTER_REAL34_DATA(REGISTER_X))) {
    realSquareRoot(&a, &a, &ctxtReal39);
    realToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
    setRegisterAngularMode(REGISTER_X, amNone);
  }
  else if(getFlag(FLAG_CPXRES)) {
    reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
    realSetPositiveSign(&a);
    realSquareRoot(&a, &a, &ctxtReal39);
    real34Zero(REGISTER_REAL34_DATA(REGISTER_X));
    realToReal34(&a, REGISTER_IMAG34_DATA(REGISTER_X));
  }
  else {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, STD_SQUARE_ROOT STD_x_UNDER_ROOT " doesn't work on a negative real when flag I is not set!");
      moreInfoOnError("In function sqrtReal:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
}



void sqrtCplx(void) {
  real_t a, b;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &a);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &b);

  realRectangularToPolar(&a, &b, &a, &b, &ctxtReal39);
  realSquareRoot(&a, &a, &ctxtReal39);
  realMultiply(&b, const_1on2, &b, &ctxtReal39);
  realPolarToRectangular(&a, &b, &a, &b, &ctxtReal39);

  realToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&b, REGISTER_IMAG34_DATA(REGISTER_X));
}
