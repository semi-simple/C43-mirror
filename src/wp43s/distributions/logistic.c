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
 * \file logistic.c
 ***********************************************/

#include "distributions/logistic.h"

#include "constantPointers.h"
#include "error.h"
#include "fonts.h"
#include "mathematics/comparisonReals.h"
#include "mathematics/wp34s.h"
#include "registers.h"
#include "registerValueConversions.h"

#include "wp43s.h"


static bool_t checkParamLogistic(real_t *x, real_t *i, real_t *j) {
  if(   ((getRegisterDataType(REGISTER_X) != dtReal34) && (getRegisterDataType(REGISTER_X) != dtLongInteger))
     || ((getRegisterDataType(REGISTER_I) != dtReal34) && (getRegisterDataType(REGISTER_I) != dtLongInteger))
     || ((getRegisterDataType(REGISTER_J) != dtReal34) && (getRegisterDataType(REGISTER_J) != dtLongInteger))) {
      displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "Values in register X, I and J must be of the real or long integer type");
        moreInfoOnError("In function checkParamLogistic:", errorMessage, NULL, NULL);
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
  else { // long integer
    convertLongIntegerRegisterToReal(REGISTER_I, i, &ctxtReal39);
  }

  if(getRegisterDataType(REGISTER_J) == dtReal34) {
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_J), j);
  }
  else { // long integer
    convertLongIntegerRegisterToReal(REGISTER_J, j, &ctxtReal39);
  }

  if(getSystemFlag(FLAG_SPCRES)) {
    return true;
  }
  else if(realIsZero(j) || realIsNegative(j)) {
    displayCalcErrorMessage(ERROR_INVALID_DISTRIBUTION_PARAM, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      moreInfoOnError("In function checkParamLogistic:", "cannot calculate for " STD_sigma " " STD_LESS_EQUAL " 0", NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return false;
  }
  return true;
}


void fnLogisticP(uint16_t unusedButMandatoryParameter) {
  real_t val, mu, s, ans;

  if(!saveLastX()) return;

  if(checkParamLogistic(&val, &mu, &s)) {
    WP34S_Pdf_Logit(&val, &mu, &s, &ans, &ctxtReal39);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    convertRealToReal34ResultRegister(&ans, REGISTER_X);
  }

  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
}

void fnLogisticL(uint16_t unusedButMandatoryParameter) {
  real_t val, mu, s, ans;

  if(!saveLastX()) return;

  if(checkParamLogistic(&val, &mu, &s)) {
    WP34S_Cdf_Logit(&val, &mu, &s, &ans, &ctxtReal39);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    convertRealToReal34ResultRegister(&ans, REGISTER_X);
  }

  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
}

void fnLogisticR(uint16_t unusedButMandatoryParameter) {
  real_t val, mu, s, ans;

  if(!saveLastX()) return;

  if(checkParamLogistic(&val, &mu, &s)) {
    WP34S_Cdfu_Logit(&val, &mu, &s, &ans, &ctxtReal39);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    convertRealToReal34ResultRegister(&ans, REGISTER_X);
  }

  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
}

void fnLogisticI(uint16_t unusedButMandatoryParameter) {
  real_t val, mu, s, ans;

  if(!saveLastX()) return;

  if(checkParamLogistic(&val, &mu, &s)) {
    if((!getSystemFlag(FLAG_SPCRES)) && (realCompareLessEqual(&val, const_0) || realCompareGreaterEqual(&val, const_1))) {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        moreInfoOnError("In function fnLogisticI:", "the argument must be 0 < x < 1", NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
    else {
      WP34S_Qf_Logit(&val, &mu, &s, &ans, &ctxtReal39);
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
      convertRealToReal34ResultRegister(&ans, REGISTER_X);
    }
  }

  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
}



/******************************************************
 * This functions are borrowed from the WP34S project
 ******************************************************/

static void cdf_logit_common(const real_t *x, real_t *res, realContext_t *realContext) {
  real_t p;

  WP34S_Tanh(x, &p, realContext);
  realAdd(&p, const_1, &p, realContext);
  realDivide(&p, const_2, res, realContext);
}

/* Extra the logistic rescaled parameter (x-J) / 2K */
static void logistic_param(const real_t *x, const real_t *mu, const real_t *s, real_t *res, realContext_t *realContext) {
  real_t p;

  realSubtract(x, mu, res, realContext);
  realAdd(s, s, &p, realContext);
  realDivide(res, &p, res, realContext);
}

void WP34S_Pdf_Logit(const real_t *x, const real_t *mu, const real_t *s, real_t *res, realContext_t *realContext) {
  real_t xx, p;
  logistic_param(x, mu, s, &xx, realContext);
  if(realIsSpecial(&xx)) {
    realZero(res);
    return;
  }
  WP34S_SinhCosh(&xx, NULL, &p, realContext);
  realPower(&p, const_2, &p, realContext);
  realMultiply(&p, s, &p, realContext);
  realMultiply(&p, const_4, &p, realContext);
  realDivide(const_1, &p, res, realContext);
}

void WP34S_Cdfu_Logit(const real_t *x, const real_t *mu, const real_t *s, real_t *res, realContext_t *realContext) {
  real_t xx;
  logistic_param(x, mu, s, &xx, realContext);
  if(realIsSpecial(&xx)) {
    realCopy(realIsNegative(&xx) ? const_0 : const_1, res);
    return;
  }
  //if(!realIsPositive(&xx)) { // WP34S returns 0 wrongly when X <= 0
  //  realCopy(const_0, res);
  //  return;
  //}
  realChangeSign(&xx);
  cdf_logit_common(&xx, res, realContext);
}

void WP34S_Cdf_Logit(const real_t *x, const real_t *mu, const real_t *s, real_t *res, realContext_t *realContext) {
  /* (1 + tanh( (x-J) / 2K)) / 2 */
  real_t xx;
  logistic_param(x, mu, s, &xx, realContext);
  if(realIsSpecial(&xx)) {
    realCopy(realIsNegative(&xx) ? const_0 : const_1, res);
    return;
  }
  //if(!realIsPositive(&xx)) { // WP34S returns 0 wrongly when X <= 0
  //  realCopy(const_0, res);
  //  return;
  //}
  cdf_logit_common(&xx, res, realContext);
}

void WP34S_Qf_Logit(const real_t *x, const real_t *mu, const real_t *s, real_t *res, realContext_t *realContext) {
  /* archtanh(2p - 1) * 2K + J */
  real_t p, q;

  realAdd(x, x, &p, realContext);
  realSubtract(&p, const_1, &p, realContext);
  WP34S_ArcTanh(&p, &q, realContext);
  realAdd(&q, &q, &p, realContext);
  realMultiply(&p, s, &p, realContext);
  realAdd(&p, mu, res, realContext);
}
