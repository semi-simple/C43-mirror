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
 * \file poisson.c
 ***********************************************/

#include "distributions/poisson.h"

#include "constantPointers.h"
#include "distributions/f.h"
#include "distributions/normal.h"
#include "error.h"
#include "fonts.h"
#include "mathematics/comparisonReals.h"
#include "mathematics/wp34s.h"
#include "registers.h"
#include "registerValueConversions.h"

#include "wp43s.h"


static bool_t checkParamPoisson(real_t *x, real_t *i) {
  if(   ((getRegisterDataType(REGISTER_X) != dtReal34) && (getRegisterDataType(REGISTER_X) != dtLongInteger))
     || ((getRegisterDataType(REGISTER_I) != dtReal34) && (getRegisterDataType(REGISTER_I) != dtLongInteger))) {
      displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "Values in register X and I must be of the real or long integer type");
        moreInfoOnError("In function checkParamPoisson:", errorMessage, NULL, NULL);
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
      moreInfoOnError("In function checkParamPoisson:", "cannot calculate for x < 0", NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return false;
  }
  else if(realIsZero(i) || realIsNegative(i)) {
    displayCalcErrorMessage(ERROR_INVALID_DISTRIBUTION_PARAM, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      moreInfoOnError("In function checkParamPoisson:", "the parameter must be " STD_lambda " > 0", NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return false;
  }
  return true;
}


void fnPoissonP(uint16_t unusedButMandatoryParameter) {
  real_t val, ans, prob;

  if(!saveLastX()) return;

  if(checkParamPoisson(&val, &prob)) {
    WP34S_Pdf_Poisson(&val, &prob, &ans, &ctxtReal39);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    convertRealToReal34ResultRegister(&ans, REGISTER_X);
  }

  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
}


void fnPoissonL(uint16_t unusedButMandatoryParameter) {
  real_t val, ans, prob;

  if(!saveLastX()) return;

  if(checkParamPoisson(&val, &prob)) {
    WP34S_Cdf_Poisson(&val, &prob, &ans, &ctxtReal39);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    convertRealToReal34ResultRegister(&ans, REGISTER_X);
  }

  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
}


void fnPoissonR(uint16_t unusedButMandatoryParameter) {
  real_t val, ans, prob;

  if(!saveLastX()) return;

  if(checkParamPoisson(&val, &prob)) {
    WP34S_Cdfu_Poisson(&val, &prob, &ans, &ctxtReal39);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    convertRealToReal34ResultRegister(&ans, REGISTER_X);
  }

  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
}


void fnPoissonI(uint16_t unusedButMandatoryParameter) {
  real_t val, ans, prob;

  if(!saveLastX()) return;

  if(checkParamPoisson(&val, &prob)) {
    if((!getSystemFlag(FLAG_SPCRES)) && (realCompareLessEqual(&val, const_0) || realCompareGreaterEqual(&val, const_1))) {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        moreInfoOnError("In function fnGeometricI:", "the argument must be 0 < x < 1", NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
    else {
      WP34S_Qf_Poisson(&val, &prob, &ans, &ctxtReal39);
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
      convertRealToReal34ResultRegister(&ans, REGISTER_X);
    }
  }

  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
}



/******************************************************
 * This functions are borrowed from the WP34S project
 ******************************************************/

/* Stack contains probability in Z, variance in Y and mean in X.
 * Returns a normal approximation in X.
 */
void WP34S_normal_moment_approx(const real_t *prob, const real_t *var, const real_t *mean, real_t *res, realContext_t *realContext) {
  real_t p, q, r;

  WP34S_qf_q_est(prob, &p, NULL, realContext);
  realPower(&p, const_2, &q, realContext);
  realSubtract(&q, const_1, &q, realContext);
  int32ToReal(6, &r);
  realDivide(&q, &r, &q, realContext);
  realDivide(&q, var, &q, realContext);
  realAdd(&p, &q, &p, realContext);
  realMultiply(&p, var, &p, realContext);
  realAdd(&p, mean, res, realContext);
}

/* One parameter Poission distribution
 * Real parameter lambda in I.
 */
void WP34S_Pdf_Poisson(const real_t *x, const real_t *lambda, real_t *res, realContext_t *realContext) {
  real_t p, q, r;

  if(realIsNegative(x) /* poission1_param */ || (!realIsAnInteger(x) /* pdf_poisson_xout */)) {
    realZero(res);
    return;
  }
  WP34S_Ln(lambda, &p, realContext);
  realMultiply(&p, x, &p, realContext);
  realSubtract(&p, lambda, &p, realContext);
  realAdd(x, const_1, &q, realContext);
  WP34S_LnGamma(&q, &r, realContext);
  realSubtract(&p, &r, &q, realContext); // ln(PDF) = x*ln(lambda) - lambda - lngamma(x+1)
  realExp(&q, res, realContext);
}

void WP34S_Cdfu_Poisson(const real_t *x, const real_t *lambda, real_t *res, realContext_t *realContext) {
  real_t p;

  if(realCompareLessEqual(lambda, const_0)) { // poission1_param
    realZero(res);
    return;
  }
  // cdfu_poisson_xout
  realToIntegralValue(x, &p, DEC_ROUND_CEILING, realContext);
  if(realCompareLessThan(&p, const_1)) {
    realCopy(const_1, res);
    return;
  }
  if(realIsInfinite(&p)) {
    realCopy(const_0, res);
    return;
  }
  WP34S_GammaP(lambda, &p, res, realContext, false, true);
}

void WP34S_Cdf_Poisson(const real_t *x, const real_t *lambda, real_t *res, realContext_t *realContext) {
  real_t p;

  if(realCompareLessEqual(lambda, const_0)) { // poission1_param
    realZero(res);
    return;
  }
  // cdf_poisson_xout
  realToIntegralValue(x, &p, DEC_ROUND_FLOOR, realContext);
  WP34S_Cdf_Poisson2(&p, lambda, res, realContext);
}
void WP34S_Cdf_Poisson2(const real_t *x, const real_t *lambda, real_t *res, realContext_t *realContext) {
  real_t p;

  // cdf_poisson
  if(realCompareLessThan(x, const_0)) {
    realCopy(const_0, res);
    return;
  }
  if(realIsInfinite(x)) {
    realCopy(const_1, res);
    return;
  }
  realAdd(x, const_1, &p, realContext);
  WP34S_GammaP(lambda, &p, res, realContext, true, true);
}

void WP34S_Qf_Poisson(const real_t *x, const real_t *lambda, real_t *res, realContext_t *realContext) {
  real_t p, q;

  if(realCompareLessEqual(lambda, const_0)) { // poission1_param
    realZero(res);
    return;
  }

  // qf_poisson_xout
  realSquareRoot(lambda, &p, realContext);
  WP34S_normal_moment_approx(x, &p, lambda, &q, realContext);
  WP34S_Qf_Newton(QF_NEWTON_POISSON, x, &q, lambda, NULL, NULL, res, realContext);
}
