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
 * \file exponential.c
 ***********************************************/

#include "distributions/exponential.h"

#include "constantPointers.h"
#include "error.h"
#include "fonts.h"
#include "mathematics/comparisonReals.h"
#include "mathematics/wp34s.h"
#include "registers.h"
#include "registerValueConversions.h"

#include "wp43s.h"


static bool_t checkParamExponential(real_t *x, real_t *i) {
  if(   ((getRegisterDataType(REGISTER_X) != dtReal34) && (getRegisterDataType(REGISTER_X) != dtLongInteger))
     || ((getRegisterDataType(REGISTER_I) != dtReal34) && (getRegisterDataType(REGISTER_I) != dtLongInteger))) {
      displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "Values in register X and I must be of the real or long integer type");
        moreInfoOnError("In function checkParamExponential:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      return false;
  }

  if(getRegisterDataType(REGISTER_X) == dtReal34) {
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), x);
  }
  else { // long integer
    convertLongIntegerRegisterToReal(REGISTER_X, x, &ctxtReal39);
  }

  if(getRegisterDataType(REGISTER_I) == dtReal34) {
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_I), i);
  }
  else {// long integer
    convertLongIntegerRegisterToReal(REGISTER_I, i, &ctxtReal39);
  }

  if(getSystemFlag(FLAG_SPCRES)) {
    return true;
  }
  else if(realIsNegative(x)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      moreInfoOnError("In function checkParamExponential:", "cannot calculate for x < 0", NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return false;
  }
  else if(realIsZero(i) || realIsNegative(i)) {
    displayCalcErrorMessage(ERROR_INVALID_DISTRIBUTION_PARAM, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      moreInfoOnError("In function checkParamExponential:", "cannot calculate for " STD_lambda " " STD_LESS_EQUAL " 0", NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return false;
  }
  return true;
}


void fnExponentialP(uint16_t unusedButMandatoryParameter) {
  real_t val, ans, dof;

  if(!saveLastX()) return;

  if(checkParamExponential(&val, &dof)) {
    WP34S_Pdf_Expon(&val, &dof, &ans, &ctxtReal39);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    realToReal34(&ans, REGISTER_REAL34_DATA(REGISTER_X));
  }

  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
}


void fnExponentialL(uint16_t unusedButMandatoryParameter) {
  real_t val, ans, dof;

  if(!saveLastX()) return;

  if(checkParamExponential(&val, &dof)) {
    WP34S_Cdf_Expon(&val, &dof, &ans, &ctxtReal39);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    realToReal34(&ans, REGISTER_REAL34_DATA(REGISTER_X));
  }

  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
}


void fnExponentialR(uint16_t unusedButMandatoryParameter) {
  real_t val, ans, dof;

  if(!saveLastX()) return;

  if(checkParamExponential(&val, &dof)) {
    WP34S_Cdfu_Expon(&val, &dof, &ans, &ctxtReal39);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    realToReal34(&ans, REGISTER_REAL34_DATA(REGISTER_X));
  }

  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
}


void fnExponentialI(uint16_t unusedButMandatoryParameter) {
  real_t val, ans, dof;

  if(!saveLastX()) return;

  if(checkParamExponential(&val, &dof)) {
    if((!getSystemFlag(FLAG_SPCRES)) && (realCompareLessEqual(&val, const_0) || realCompareGreaterEqual(&val, const_1))) {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        moreInfoOnError("In function fnExponentialI:", "the argument must be 0 < x < 1", NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
    else {
      WP34S_Qf_Expon(&val, &dof, &ans, &ctxtReal39);
      if(realIsNaN(&ans)) {
        displayCalcErrorMessage(ERROR_NO_ROOT_FOUND, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          moreInfoOnError("In function fnExponentialI:", "WP34S_Qf_Expon did not converge", NULL, NULL);
        #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      }
      else {
        reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
        realToReal34(&ans, REGISTER_REAL34_DATA(REGISTER_X));
      }
    }
  }

  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
}



/******************************************************
 * This functions are borrowed from the WP34S project
 ******************************************************/

void WP34S_Pdf_Expon(const real_t *x, const real_t *lambda, real_t *res, realContext_t *realContext) {
  if(realCompareLessEqual(x, const_0)) {
    realCopy(const_0, res);
    return;
  }
  if(realIsSpecial(lambda)) {
    realZero(res); /* Can only be infinite which has zero probability */
    return;
  }
  realMultiply(x, lambda, res, realContext);
  realChangeSign(res);
  realExp(res, res, realContext);
  realMultiply(res, lambda, res, realContext);
}

void WP34S_Cdfu_Expon(const real_t *x, const real_t *lambda, real_t *res, realContext_t *realContext) {
  if(realCompareLessEqual(x, const_0)) {
    realCopy(const_1, res);
    return;
  }
  if(realIsSpecial(lambda)) {
    realCopy(const_plusInfinity, res);
    return;
  }
  realMultiply(x, lambda, res, realContext);
  if(realCompareLessThan(res, const_0)) {
    realCopy(const_1, res);
    return;
  }
  realChangeSign(res);
  realExp(res, res, realContext);
}

void WP34S_Cdf_Expon(const real_t *x, const real_t *lambda, real_t *res, realContext_t *realContext) {
  if(realCompareLessEqual(x, const_0)) {
    realCopy(const_0, res);
    return;
  }
  if(realIsSpecial(lambda)) {
    realCopy(const_plusInfinity, res);
    return;
  }
  realMultiply(x, lambda, res, realContext);
  if(realCompareLessThan(res, const_0)) {
    realCopy(const_1, res);
    return;
  }
  realChangeSign(res);
  WP34S_ExpM1(res, res, realContext);
  realChangeSign(res);
}

void WP34S_Qf_Expon(const real_t *x, const real_t *lambda, real_t *res, realContext_t *realContext) {
  real_t p;

  realSubtract(const_0, x, &p, realContext), WP34S_Ln1P(&p, res, realContext);
  realDivide(res, lambda, res, realContext);
  realChangeSign(res);
}
