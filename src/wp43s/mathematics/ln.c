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
 * \file ln.c
 ***********************************************/

#include "mathematics/ln.h"

#include "constantPointers.h"
#include "debug.h"
#include "error.h"
#include "flags.h"
#include "fonts.h"
#include "items.h"
#include "mathematics/toPolar.h"
#include "mathematics/wp34s.h"
#include "matrix.h"
#include "registers.h"
#include "registerValueConversions.h"

#include "wp43s.h"



TO_QSPI void (* const ln[NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS])(void) = {
// regX ==> 1            2       3         4        5        6        7          8           9             10
//          Long integer Real34  complex34 Time     Date     String   Real34 mat Complex34 m Short integer Config data
            lnLonI,      lnReal, lnCplx,   lnError, lnError, lnError, lnRema,    lnCxma,     lnShoI,       lnError
};



/********************************************//**
 * \brief Data type error in ln
 *
 * \param void
 * \return void
 ***********************************************/
#if (EXTRA_INFO_ON_CALC_ERROR == 1)
  void lnError(void) {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    sprintf(errorMessage, "cannot calculate Ln for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    moreInfoOnError("In function fnLn:", errorMessage, NULL, NULL);
  }
#endif // (EXTRA_INFO_ON_CALC_ERROR == 1)



/********************************************//**
 * \brief regX ==> regL and ln(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnLn(uint16_t unusedButMandatoryParameter) {
  if(!saveLastX()) return;

  ln[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



void lnComplex(const real_t *real, const real_t *imag, real_t *lnReal, real_t *lnImag, realContext_t *realContext) {
  if(realIsZero(real) && realIsZero(imag)) {
    realCopy(const_minusInfinity, lnReal);
    realZero(lnImag);
  }
  else {
    realRectangularToPolar(real, imag, lnReal, lnImag, realContext);
    WP34S_Ln(lnReal, lnReal, realContext);
  }
}



/**********************************************************************
 * In all the functions below:
 * if X is a number then X = a + ib
 * The variables a and b are used for intermediate calculations
 ***********************************************************************/

void lnLonI(void) {
  longInteger_t lgInt;

  convertLongIntegerRegisterToLongInteger(REGISTER_X, lgInt);

  if(longIntegerIsZero(lgInt)) {
    if(getSystemFlag(FLAG_SPCRES)) {
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
      realToReal34(const_minusInfinity, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        moreInfoOnError("In function lnLonI:", "cannot calculate Ln(0)", NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
  }
  else {
    real_t x;

    convertLongIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);

    if(longIntegerIsPositive(lgInt)) {
      WP34S_Ln(&x, &x, &ctxtReal39);
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
     }
    else if(getFlag(FLAG_CPXRES)) {
      realSetPositiveSign(&x);
      WP34S_Ln(&x, &x, &ctxtReal39);
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      realToReal34(const_pi, REGISTER_IMAG34_DATA(REGISTER_X));
    }
    else if(getSystemFlag(FLAG_SPCRES)) {
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
      realToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        moreInfoOnError("In function lnLonI:", "cannot calculate Ln of a negative number when CPXRES is not set!", NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
  }

  longIntegerFree(lgInt);
}



void lnRema(void) {
  elementwiseRema(lnReal);
}



void lnCxma(void) {
  elementwiseCxma(lnCplx);
}



void lnShoI(void) {
  real_t x;

  convertShortIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);

  if(realIsZero(&x)) {
    if(getSystemFlag(FLAG_SPCRES)) {
      realToReal34(const_minusInfinity, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        moreInfoOnError("In function lnShoI:", "cannot calculate Ln(0)", NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
  }
  else {
    if(realIsPositive(&x)) {
      WP34S_Ln(&x, &x, &ctxtReal39);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
     }
    else if(getFlag(FLAG_CPXRES)) {
      realSetPositiveSign(&x);
      WP34S_Ln(&x, &x, &ctxtReal39);
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      realToReal34(const_pi, REGISTER_IMAG34_DATA(REGISTER_X));
    }
    else if(getSystemFlag(FLAG_SPCRES)) {
      realToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        moreInfoOnError("In function lnShoI:", "cannot calculate Ln of a negative number when CPXRES is not set!", NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
  }
}



void lnReal(void) {
  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(getSystemFlag(FLAG_SPCRES)) {
      realToReal34(const_minusInfinity, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        moreInfoOnError("In function lnReal:", "cannot calculate Ln(0)", NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
  }

  else if(real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(!getSystemFlag(FLAG_SPCRES)) {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        moreInfoOnError("In function lnReal:", "cannot use " STD_PLUS_MINUS STD_INFINITY " as X input of ln when flag D is not set", NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      return;
    }
    else if(getFlag(FLAG_CPXRES)) {
      if(real34IsPositive(REGISTER_REAL34_DATA(REGISTER_X))) {
        realToReal34(const_plusInfinity, REGISTER_REAL34_DATA(REGISTER_X));
      }
      else {
        reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
        realToReal34(const_plusInfinity, REGISTER_REAL34_DATA(REGISTER_X));
        realToReal34(const_pi, REGISTER_IMAG34_DATA(REGISTER_X));
      }
    }
    else {
      realToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
    }
  }

  else {
    real_t x;

    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
    if(real34IsPositive(REGISTER_REAL34_DATA(REGISTER_X))) {
      WP34S_Ln(&x, &x, &ctxtReal39);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
     }
    else if(getFlag(FLAG_CPXRES)) {
      realSetPositiveSign(&x);
      WP34S_Ln(&x, &x, &ctxtReal39);
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      realToReal34(const_pi, REGISTER_IMAG34_DATA(REGISTER_X));
    }
    else if(getSystemFlag(FLAG_SPCRES)) {
      realToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        moreInfoOnError("In function lnReal:", "cannot calculate Ln of a negative number when CPXRES is not set!", NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
  }
  setRegisterAngularMode(REGISTER_X, amNone);
}



void lnCplx(void) {
  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X)) && real34IsZero(REGISTER_IMAG34_DATA(REGISTER_X))) {
    if(getSystemFlag(FLAG_SPCRES)) {
      realToReal34(const_minusInfinity, REGISTER_REAL34_DATA(REGISTER_X));
      real34Zero(REGISTER_IMAG34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        moreInfoOnError("In function lnCplx:", "cannot calculate Ln(0)", NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
  }
  else {
    real_t xReal, xImag;

    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &xReal);
    real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &xImag);

    lnComplex(&xReal, &xImag, &xReal, &xImag, &ctxtReal39);

    realToReal34(&xReal, REGISTER_REAL34_DATA(REGISTER_X));
    realToReal34(&xImag, REGISTER_IMAG34_DATA(REGISTER_X));
  }
}
