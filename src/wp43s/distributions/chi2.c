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
 * \file chi2.c
 ***********************************************/

#include "distributions/chi2.h"

#include "constantPointers.h"
#include "distributions/normal.h"
#include "error.h"
#include "fonts.h"
#include "mathematics/comparisonReals.h"
#include "mathematics/wp34s.h"
#include "registers.h"
#include "registerValueConversions.h"

#include "wp43s.h"


bool_t checkRegisterNoFP(calcRegister_t reg) {
  real34_t flooredI;

  if(getRegisterDataType(reg) == dtLongInteger) {
    return true;
  }
  else if(getRegisterDataType(reg) == dtReal34) {
    real34ToIntegralValue(REGISTER_REAL34_DATA(reg), &flooredI, DEC_ROUND_FLOOR);
    return real34CompareEqual(REGISTER_REAL34_DATA(reg), &flooredI);
  }
  else {
    return false;
  }
}
static bool_t checkParamChi2(real_t *x, real_t *i) {
  if(   ((getRegisterDataType(REGISTER_X) != dtReal34) && (getRegisterDataType(REGISTER_X) != dtLongInteger))
     || ((getRegisterDataType(REGISTER_I) != dtReal34) && (getRegisterDataType(REGISTER_I) != dtLongInteger))) {
      displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "Values in register X and I must be of the real or long integer type");
        moreInfoOnError("In function checkParamChi2:", errorMessage, NULL, NULL);
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

  if(!checkRegisterNoFP(REGISTER_I)) {
    displayCalcErrorMessage(ERROR_INVALID_DISTRIBUTION_PARAM, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      moreInfoOnError("In function checkParamChi2:", "k is not an integer", NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return false;
  }
  else if(getSystemFlag(FLAG_SPCRES)) {
    return true;
  }
  else if(realIsNegative(x)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      moreInfoOnError("In function checkParamChi2:", "cannot calculate for x < 0", NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return false;
  }
  else if(realIsZero(i) || realIsNegative(i)) {
    displayCalcErrorMessage(ERROR_INVALID_DISTRIBUTION_PARAM, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      moreInfoOnError("In function checkParamChi2:", "cannot calculate for k " STD_LESS_EQUAL " 0", NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return false;
  }
  return true;
}


void fnChi2P(uint16_t unusedButMandatoryParameter) {
  real_t val, ans, dof;

  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(checkParamChi2(&val, &dof)) {
    WP34S_Pdf_Chi2(&val, &dof, &ans, &ctxtReal39);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    realToReal34(&ans, REGISTER_REAL34_DATA(REGISTER_X));
  }

  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
}


void fnChi2L(uint16_t unusedButMandatoryParameter) {
  real_t val, ans, dof;

  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(checkParamChi2(&val, &dof)) {
    WP34S_Cdf_Chi2(&val, &dof, &ans, &ctxtReal39);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    realToReal34(&ans, REGISTER_REAL34_DATA(REGISTER_X));
  }

  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
}


void fnChi2R(uint16_t unusedButMandatoryParameter) {
  real_t val, ans, dof;

  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(checkParamChi2(&val, &dof)) {
    WP34S_Cdfu_Chi2(&val, &dof, &ans, &ctxtReal39);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    realToReal34(&ans, REGISTER_REAL34_DATA(REGISTER_X));
  }

  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
}


void fnChi2I(uint16_t unusedButMandatoryParameter) {
  real_t val, ans, dof;

  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(checkParamChi2(&val, &dof)) {
    if((!getSystemFlag(FLAG_SPCRES)) && (realCompareLessEqual(&val, const_0) || realCompareGreaterEqual(&val, const_1))) {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        moreInfoOnError("In function fnChi2I:", "the argument must be 0 < x < 1", NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
    else {
      WP34S_Qf_Chi2(&val, &dof, &ans, &ctxtReal39);
      if(realIsNaN(&ans)) {
        displayCalcErrorMessage(ERROR_NO_ROOT_FOUND, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          moreInfoOnError("In function fnChi2I:", "WP34S_Qf_Chi2 did not converge", NULL, NULL);
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

void WP34S_Pdf_Chi2(const real_t *x, const real_t *k, real_t *res, realContext_t *realContext) {
  real_t p, q, r, s;

  if(realCompareLessEqual(x, const_0)) {
    realCopy(const_0, res);
    return;
  }

  realDivide(k, const_2, &p, realContext);
  WP34S_Ln(x, &q, realContext);
  realDivide(x, const_2, &r, realContext);
  realChangeSign(&r);
  realSubtract(&p, const_1, &s, realContext);
  realMultiply(&q, &s, &q, realContext);
  realAdd(&r, &q, &q, realContext);
  WP34S_LnGamma(&p, &r, realContext);
  realSubtract(&q, &r, &q, realContext);
  realMultiply(&p, const_ln2, &r, realContext);
  realSubtract(&q, &r, &q, realContext);
  realExp(&q, res, realContext);
}

void WP34S_Cdfu_Chi2(const real_t *x, const real_t *k, real_t *res, realContext_t *realContext) {
  real_t p, q;

  if(realCompareLessEqual(x, const_0)) {
    realCopy(const_1, res);
    return;
  }
  if(realIsInfinite(x)) {
    realCopy(const_0, res);
    return;
  }

  realDivide(x, const_2, &p, realContext);
  realDivide(k, const_2, &q, realContext);
  WP34S_GammaP(&p, &q, res, realContext, true, true);
}

void WP34S_Cdf_Chi2(const real_t *x, const real_t *k, real_t *res, realContext_t *realContext) {
  real_t p, q;

  if(realCompareLessEqual(x, const_0)) {
    realCopy(const_0, res);
    return;
  }
  if(realIsInfinite(x)) {
    realCopy(const_1, res);
    return;
  }

  realDivide(x, const_2, &p, realContext);
  realDivide(k, const_2, &q, realContext);
  WP34S_GammaP(&p, &q, res, realContext, false, true);
}

void WP34S_Qf_Chi2(const real_t *x, const real_t *k, real_t *res, realContext_t *realContext) {
  real_t p, q, r, s, t, reg0;
  int32_t loops;

  if(realCompareEqual(x, const_0)) {
    realCopy(const_0, res);
  }

  realCopy(x, &reg0);
  loops = 6;
  int32ToReal(19, &p); p.exponent -= 1;
  realCopy(realCompareEqual(k, const_1) ? const_0 : k, &q);
  realChangeSign(&q);
  realPower(&p, &q, &p, realContext);
  realDivide(&p, const_pi, &p, realContext);
  if(realCompareGreaterEqual(&reg0, &p)) {
    WP34S_qf_q_est(&reg0, &q, NULL, realContext);
    int32ToReal(222, &s); s.exponent -= 3;
    realDivide(&s, k, &s, realContext);
    realSquareRoot(&s, &r, realContext);
    realMultiply(&q, &r, &q, realContext);
    realAdd(&q, const_1, &q, realContext);
    realSubtract(&q, &s, &q, realContext);
    realPower(&q, const_3, &q, realContext);
    realMultiply(&q, k, &q, realContext);
    realMultiply(const_eE, k, &r, realContext);
    int32ToReal(8, &t);
    realAdd(&r, &t, &r, realContext);
    if(realCompareGreaterEqual(&q, &r)) {
      realDivide(&q, const_2, &q, realContext);
      WP34S_Ln(&q, &q, realContext);
      realDivide(k, const_2, &t, realContext);
      realSubtract(&t, const_1, &t, realContext);
      realMultiply(&q, &t, &q, realContext);
      realChangeSign(&q);
      realMultiply(&reg0, const__1, &t, realContext);
      WP34S_Ln1P(&t, &t, realContext);
      realAdd(&q, &t, &q, realContext);
      realDivide(k, const_2, &t, realContext);
      WP34S_LnGamma(&t, &t, realContext);
      realAdd(&q, &t, &q, realContext);
      realMultiply(&q, const_2, &q, realContext);
      realChangeSign(&q);
    }
  }
  else { // chi2_q_low
    realDivide(&reg0, k, &q, realContext);
    realDivide(&q, const_2, &q, realContext);
    WP34S_Ln(&q, &q, realContext);
    realDivide(k, const_2, &r, realContext);
    WP34S_LnGamma(&r, &r, realContext);
    realAdd(&q, &r, &q, realContext);
    realMultiply(&q, const_2, &q, realContext);
    realDivide(&q, k, &q, realContext);
    realExp(&q, &q, realContext);
    realMultiply(&q, const_2, &q, realContext);
  }

  do { // chi2_q_refine
    if(realCompareLessThan(&q, k)) {
      WP34S_Cdf_Chi2(&q, k, &p, realContext);
      realDivide(&p, &reg0, &r, realContext);
      WP34S_Ln(&r, &r, realContext);
    }
    else { // chi2_q_big
      realSubtract(const_1, &reg0, &r, realContext);
      WP34S_Cdfu_Chi2(&q, k, &s, realContext);
      realSubtract(const_1, &s, &p, realContext);
      realSubtract(&r, &s, &r, realContext);
      realDivide(&r, &reg0, &r, realContext);
      WP34S_Ln1P(&r, &r, realContext);
    }
    // chi2_q_common
    WP34S_Pdf_Chi2(&q, k, &s, realContext);
    realDivide(&s, &p, &p, realContext);
    realDivide(&r, &p, &r, realContext);
    realSubtract(k, const_2, &s, realContext);
    realSubtract(&s, &q, &s, realContext);
    realDivide(&s, &q, &s, realContext);
    realMultiply(&p, const_2, &p, realContext);
    realSubtract(&s, &p, &p, realContext);
    int32ToReal(4, &s);
    realDivide(&p, &s, &p, realContext);
    realMultiply(&p, &r, &p, realContext);
    realChangeSign(&p);
    realAdd(&p, const_1, &p, realContext);
    realDivide(&r, &p, &r, realContext);
    realChangeSign(&r);
    realAdd(&q, &r, &p, realContext);
    // SHOW_CONVERGENCE
    realCopy(const_1, &r); r.exponent -= 32 /*14*/;
    if(WP34S_RelativeError(&p, &q, &r, realContext)) {
      realCopy(&p, res);
      return;
    }
    realCopy(&p, &q);
  } while(--loops > 0);

    realCopy(const_NaN, res); // ERR 20
}
