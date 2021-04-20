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
 * \file geometric.c
 ***********************************************/

#include "distributions/geometric.h"

#include "constantPointers.h"
#include "distributions/binomial.h"
#include "distributions/hyper.h"
#include "distributions/negBinom.h"
#include "distributions/poisson.h"
#include "error.h"
#include "fonts.h"
#include "mathematics/comparisonReals.h"
#include "mathematics/wp34s.h"
#include "registers.h"
#include "registerValueConversions.h"

#include "wp43s.h"


static bool_t checkParamGeometric(real_t *x, real_t *i) {
  if(   ((getRegisterDataType(REGISTER_X) != dtReal34) && (getRegisterDataType(REGISTER_X) != dtLongInteger))
     || ((getRegisterDataType(REGISTER_I) != dtReal34) && (getRegisterDataType(REGISTER_I) != dtLongInteger))) {
      displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "Values in register X and I must be of the real or long integer type");
        moreInfoOnError("In function checkParamGeometric:", errorMessage, NULL, NULL);
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

  if(getSystemFlag(FLAG_SPCRES)) {
    return true;
  }
  else if(realIsNegative(x)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      moreInfoOnError("In function checkParamGeometric:", "cannot calculate for x < 0", NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return false;
  }
  else if(realIsZero(i) || realIsNegative(i) || realCompareGreaterThan(i, const_1)) {
    displayCalcErrorMessage(ERROR_INVALID_DISTRIBUTION_PARAM, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      moreInfoOnError("In function checkParamGeometric:", "the parameter must be 0 < p " STD_LESS_EQUAL " 1", NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return false;
  }
  return true;
}


void fnGeometricP(uint16_t unusedButMandatoryParameter) {
  real_t val, ans, prob;

  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(checkParamGeometric(&val, &prob)) {
    WP34S_Pdf_Geom(&val, &prob, &ans, &ctxtReal39);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    realToReal34(&ans, REGISTER_REAL34_DATA(REGISTER_X));
  }

  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
}


void fnGeometricL(uint16_t unusedButMandatoryParameter) {
  real_t val, ans, prob;

  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(checkParamGeometric(&val, &prob)) {
    WP34S_Cdf_Geom(&val, &prob, &ans, &ctxtReal39);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    realToReal34(&ans, REGISTER_REAL34_DATA(REGISTER_X));
  }

  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
}


void fnGeometricR(uint16_t unusedButMandatoryParameter) {
  real_t val, ans, prob;

  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(checkParamGeometric(&val, &prob)) {
    WP34S_Cdfu_Geom(&val, &prob, &ans, &ctxtReal39);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    realToReal34(&ans, REGISTER_REAL34_DATA(REGISTER_X));
  }

  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
}


void fnGeometricI(uint16_t unusedButMandatoryParameter) {
  real_t val, ans, prob;

  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(checkParamGeometric(&val, &prob)) {
    if((!getSystemFlag(FLAG_SPCRES)) && (realCompareLessEqual(&val, const_0) || realCompareGreaterEqual(&val, const_1))) {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        moreInfoOnError("In function fnGeometricI:", "the argument must be 0 < x < 1", NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
    else {
      WP34S_Qf_Geom(&val, &prob, &ans, &ctxtReal39);
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
      realToReal34(&ans, REGISTER_REAL34_DATA(REGISTER_X));
    }
  }

  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
}



/******************************************************
 * This functions are borrowed from the WP34S project
 ******************************************************/

void WP34S_Pdf_Geom(const real_t *x, const real_t *p0, real_t *res, realContext_t *realContext) {
  real_t p;

  if(realIsNegative(x) || (!realIsAnInteger(x))) {
    realZero(res);
    return;
  }
  realMultiply(p0, const__1, &p, realContext);
  WP34S_Ln1P(&p, &p, realContext);
  realMultiply(x, &p, &p, realContext);
  realExp(&p, &p, realContext);
  realMultiply(&p, p0, res, realContext);
}

void WP34S_Cdfu_Geom(const real_t *x, const real_t *p0, real_t *res, realContext_t *realContext) {
  real_t p, q;

  realToIntegralValue(x, &p, DEC_ROUND_CEILING, realContext);
  if(realCompareLessThan(&p, const_1)) {
    realCopy(const_1, res);
    return;
  }
  if(realIsInfinite(&p)) {
    realCopy(const_0, res);
    return;
  }
  realSubtract(const_1, p0, &q, realContext);
  realPower(&q, &p, res, realContext);
}

void WP34S_Cdf_Geom(const real_t *x, const real_t *p0, real_t *res, realContext_t *realContext) {
  real_t p, q;

  if(realCompareLessThan(x, const_0)) {
    realCopy(const_0, res);
    return;
  }
  if(realIsInfinite(x)) {
    realCopy(const_1, res);
    return;
  }
  realToIntegralValue(x, &p, DEC_ROUND_FLOOR, realContext);
  realAdd(&p, const_1, &p, realContext);
  realMultiply(p0, const__1, &q, realContext);
  WP34S_Ln1P(&q, &q, realContext);
  realMultiply(&p, &q, &p, realContext);
  WP34S_ExpM1(&p, res, realContext);
  realChangeSign(res);
}

void WP34S_Qf_Geom(const real_t *x, const real_t *p0, real_t *res, realContext_t *realContext) {
  real_t p, q;

  if(realCompareLessEqual(x, const_0)) {
    realZero(res);
    return;
  }
  realMultiply(x, const__1, &p, realContext);
  WP34S_Ln1P(&p, &p, realContext);
  realMultiply(p0, const__1, &q, realContext);
  WP34S_Ln1P(&q, &q, realContext);
  realDivide(&p, &q, &p, realContext);
  realSubtract(&p, const_1, &p, realContext);
  realToIntegralValue(&p, &p, DEC_ROUND_FLOOR, realContext);
  WP34S_qf_discrete_final(QF_DISCRETE_CDF_GEOMETRIC, &p, x, p0, NULL, NULL, res, realContext);
}

void WP34S_qf_discrete_final(uint16_t dist, const real_t *r, const real_t *p, const real_t *i, const real_t *j, const real_t *k, real_t *res, realContext_t *realContext) {
  real_t q;

  switch(dist) { // qf_discrete_cdf
    case QF_DISCRETE_CDF_POISSON:
      WP34S_Cdf_Poisson2(r, i, &q, &ctxtReal51);
      break;
    case QF_DISCRETE_CDF_BINOMIAL:
      WP34S_Cdf_Binomial2(r, i, j, &q, &ctxtReal51);
      break;
    case QF_DISCRETE_CDF_GEOMETRIC:
      WP34S_Cdf_Geom(r, i, &q, &ctxtReal51);
      break;
    case QF_DISCRETE_CDF_NEGBINOM:
      cdf_NegBinomial2(r, i, j, &q, &ctxtReal51);
      break;
    case QF_DISCRETE_CDF_HYPERGEOMETRIC:
      cdf_Hypergeometric2(r, i, j, k, &q, &ctxtReal75);
      break;
    default: // unlikely
      realZero(&q);
  }
  realAdd(&q, const_0, &q, realContext);
  if(realCompareLessThan(&q, p)) {
    realAdd(r, const_1, res, realContext);
  }
  else { // qf_discrere_out
    realCopy(r, res);
  }
}
