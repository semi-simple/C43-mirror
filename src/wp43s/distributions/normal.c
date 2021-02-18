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
 * \file normal.c
 ***********************************************/

#include "wp43s.h"


static bool_t checkParamNormal(void) {
  if((getRegisterDataType(REGISTER_X) != dtReal34) || (getRegisterDataType(REGISTER_I) != dtReal34) || (getRegisterDataType(REGISTER_J) != dtReal34)) {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "Values in register X, I and J must be of the real type");
      moreInfoOnError("In function checkParamNormal:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return false;
  }
  else if(getSystemFlag(FLAG_SPCRES)) {
    return true;
  }
  else if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_J)) || real34IsNegative(REGISTER_REAL34_DATA(REGISTER_J))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      moreInfoOnError("In function checkParamNormal:", "cannot calculate for " STD_sigma " " STD_LESS_EQUAL " 0", NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return false;
  }
  return true;
}


void fnNormalP(uint16_t unusedButMandatoryParameter) {
  real_t val, tmp;

  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(checkParamNormal()) {
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &val);
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_I), &tmp);
    realSubtract(&val, &tmp, &val, &ctxtReal39);
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_J), &tmp);
    realDivide(&val, &tmp, &val, &ctxtReal39);
    WP34S_Pdf_Q(&val, &tmp, &ctxtReal39);
    realToReal34(&tmp, REGISTER_REAL34_DATA(REGISTER_X));
  }

  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
}


void fnNormalL(uint16_t unusedButMandatoryParameter) {
  real_t val, tmp;

  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(checkParamNormal()) {
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &val);
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_I), &tmp);
    realSubtract(&val, &tmp, &val, &ctxtReal39);
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_J), &tmp);
    realDivide(&val, &tmp, &val, &ctxtReal39);
    WP34S_Cdf_Q(&val, &tmp, &ctxtReal39);
    realToReal34(&tmp, REGISTER_REAL34_DATA(REGISTER_X));
  }

  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
}


void fnNormalR(uint16_t unusedButMandatoryParameter) {
  real_t val, tmp;

  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(checkParamNormal()) {
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &val);
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_I), &tmp);
    realSubtract(&val, &tmp, &val, &ctxtReal39);
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_J), &tmp);
    realDivide(&val, &tmp, &val, &ctxtReal39);
    WP34S_Cdfu_Q(&val, &tmp, &ctxtReal39);
    realToReal34(&tmp, REGISTER_REAL34_DATA(REGISTER_X));
  }

  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
}


void fnNormalI(uint16_t unusedButMandatoryParameter) {
  real_t val, tmp;

  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(checkParamNormal()) {
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &val);
    if((!getSystemFlag(FLAG_SPCRES)) && (realCompareLessEqual(&val, const_0) || realCompareGreaterEqual(&val, const_1))) {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        moreInfoOnError("In function fnNormalI:", "the argument must be 0 < x < 1", NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
    else {
      WP34S_Qf_Q(&val, &tmp, &ctxtReal39);
      real34ToReal(REGISTER_REAL34_DATA(REGISTER_J), &val);
      realMultiply(&tmp, &val, &tmp, &ctxtReal39);
      real34ToReal(REGISTER_REAL34_DATA(REGISTER_I), &val);
      realAdd(&tmp, &val, &tmp, &ctxtReal39);
      realToReal34(&tmp, REGISTER_REAL34_DATA(REGISTER_X));
    }
  }

  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
}



/******************************************************
 * This functions are borrowed from the WP34S project
 ******************************************************/

static void cdf_q(const real_t *x, real_t *res, realContext_t *realContext, bool_t upper) {
  real_t p;

  if(upper) {
    if(realIsNegative(x)) goto cdfu_q_flip;
    cdf_q_flip:
    realPower(x, const_2, res, realContext);
    realDivide(res, const_2, res, realContext);
    WP34S_GammaP(res, const_1on2, res, realContext, true, false);
    realDivide(res, const_2, res, realContext);
    realSquareRoot(const_pi, &p, realContext);
    realDivide(res, &p, res, realContext);
    return;
  }
  else {
    if(realIsNegative(x)) goto cdf_q_flip;
    cdfu_q_flip:
    realPower(x, const_2, res, realContext);
    realDivide(res, const_2, res, realContext);
    WP34S_GammaP(res, const_1on2, res, realContext, false, false);
    realSquareRoot(const_pi, &p, realContext);
    realDivide(res, &p, res, realContext);
    realAdd(res, const_1, res, realContext);
    realDivide(res, const_2, res, realContext);
    return;
  }
}

void WP34S_Pdf_Q(const real_t *x, real_t *res, realContext_t *realContext) {
  real_t p;
  realPower(x, const_2, res, realContext);
  realDivide(res, const_2, res, realContext);
  realChangeSign(res);
  realExp(res, res, realContext);
  realSquareRoot(const_2pi, &p, realContext);
  realDivide(res, &p, res, realContext);
}

void WP34S_Cdfu_Q(const real_t *x, real_t *res, realContext_t *realContext) {
  cdf_q(x, res, realContext, true);
}

void WP34S_Cdf_Q(const real_t *x, real_t *res, realContext_t *realContext) {
  cdf_q(x, res, realContext, false);
}

/* This routine that returns a signed guess for the Normal quantile.
 * GNQ takes any 0 < p < 1 and returns a positive or negative estimate
 * for the Normal quantile.
 */
void WP34S_qf_q_est(const real_t *x, real_t *res, real_t* resY, realContext_t *realContext) {
  real_t p, q, r, s;
  bool_t isSmall = false;

  // qf_q_int_est
  realMultiply(x, const__1, &p, realContext);
  realAdd(&p, const_1, &p, realContext);
  if(realCompareLessThan(x, &p)) {
    isSmall = true;
    realCopy(x, &p);
  }

  realDivide(const_2, const_10, &q, realContext);

  if(realCompareLessThan(&p, &q)) {
    WP34S_Ln(&p, &q, realContext);
    realMultiply(&q, const_2, &q, realContext);
    realChangeSign(&q);
    realSubtract(&q, const_1, &r, realContext);
    realMultiply(&r, const_2pi, &r, realContext);
    realSquareRoot(&r, &r, realContext);
    realMultiply(&r, &p, &r, realContext);
    WP34S_Ln(&r, &r, realContext);
    realMultiply(&r, const_2, &r, realContext);
    realChangeSign(&r);
    realSquareRoot(&r, &r, realContext);
    int32ToReal(264, &s);
    realDivide(&s, const_1000, &s, realContext);
    realDivide(&s, &q, &q, realContext);
    realAdd(&q, &r, &q, realContext);
  }
  else { // qf_q_mid
    realMultiply(&p, const__1, &q, realContext);
    realAdd(&q, const_1on2, &q, realContext);
    realSquareRoot(const_2pi, &r, realContext);
    realMultiply(&q, &r, &q, realContext);
    realPower(&q, const_3, &r, realContext);
    realDivide(&r, const_5, &r, realContext);
    realAdd(&q, &r, &q, realContext);
  }

  realCopy(&q, res);
  realCopy(&p, resY);
  // qf_q_signfix
  if(isSmall)
    realChangeSign(res);
}

void WP34S_Qf_Q(const real_t *x, real_t *res, realContext_t *realContext) {
  real_t p, q, r, s, reg0;
  bool_t half = false;
  int32_t loops;
  WP34S_qf_q_est(x, &p, &reg0, realContext);
  if(realIsNegative(&p)) half = true;
  loops = 2;
  realSetPositiveSign(&p);
  realAdd(&p, const_1, &q, realContext);
  roundToSignificantDigits(&q, &q, 3, realContext);
  if(realCompareEqual(&q, const_1)) {
    --loops;
    realMultiply(&p, const_1e8, &q, realContext);
    realMultiply(&q, const_1e8, &q, realContext);
    if(realCompareGreaterThan(&q, const_1))
      goto qf_q_calc;
  }
  else {
    qf_q_calc:
    do { // qf_q_refine
      if(realCompareGreaterEqual(&p, const_1)) {
        WP34S_Cdfu_Q(&p, &q, realContext);
        realSubtract(&q, &reg0, &q, realContext);
      }
      else { // qf_q_small
        realPower(&p, const_2, &q, realContext);
        realDivide(&q, const_2, &q, realContext);
        WP34S_GammaP(&q, const_1on2, &r, realContext, false, true);
        realDivide(&r, const_2, &q, realContext);
        realChangeSign(&q);
        realSubtract(const_1on2, &reg0, &r, realContext);
        realAdd(&q, &r, &q, realContext);
      }
      // qf_q_common
      WP34S_Pdf_Q(&p, &r, realContext);
      realDivide(&q, &r, &q, realContext);
      realPower(&q, const_3, &r, realContext);
      realPower(&p, const_2, &s, realContext);
      realMultiply(&s, const_2, &s, realContext);
      realAdd(&s, const_1, &s, realContext);
      realMultiply(&r, &s, &r, realContext);
      int32ToReal(6, &s);
      realDivide(&r, &s, &r, realContext);
      realDivide(&p, const_2, &s, realContext);
      realMultiply(&s, &q, &s, realContext);
      realMultiply(&s, &q, &s, realContext);
      realAdd(&s, &r, &r, realContext);
      realAdd(&r, &q, &q, realContext);
      realAdd(&q, &p, &p, realContext);
      // SHOW_CONVERGENCE
    } while(--loops > 0);
  }
  // qf_q_out
  if(half) realChangeSign(&p);
  realCopy(&p, res);
}
