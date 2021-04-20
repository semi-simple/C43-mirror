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
 * \file bessel.c
 ***********************************************/

#include "mathematics/bessel.h"

#include "constantPointers.h"
#include "debug.h"
#include "error.h"
#include "mathematics/arccosh.h"
#include "mathematics/comparisonReals.h"
#include "mathematics/division.h"
#include "mathematics/multiplication.h"
#include "mathematics/toRect.h"
#include "mathematics/wp34s.h"
#include "memory.h"
#include "registers.h"
#include "registerValueConversions.h"

#include "wp43s.h"



static bool_t besselGetParam(calcRegister_t regist, real_t *r, realContext_t *realContext) {
  switch(getRegisterDataType(regist)) {
    case dtReal34:
      if(getRegisterAngularMode(regist) == amNone) {
       real34ToReal(REGISTER_REAL34_DATA(regist), r);
       return true;
      }
      break;
    case dtLongInteger:
      convertLongIntegerRegisterToReal(regist, r, realContext);
      return true;
  }
  displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate Bessel function for %s and %s", getRegisterDataTypeName(REGISTER_X, true, false), getRegisterDataTypeName(REGISTER_Y, true, false));
    moreInfoOnError("In function fnBessel:", errorMessage, NULL, NULL);
  #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  return false;
}

void fnBessel(uint16_t unusedButMandatoryParameter) {
  real_t x, n, r, a;

  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(besselGetParam(REGISTER_X, &x, &ctxtReal75) && besselGetParam(REGISTER_Y, &n, &ctxtReal75)) {
    if(realIsAnInteger(&n) || (!realIsNegative(&x))) {
      WP34S_BesselJ(&n, &x, &r, &ctxtReal75);
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
      realToReal34(&r, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else if(getSystemFlag(FLAG_CPXRES)) { // Real -> Complex
      realSetPositiveSign(&x);
      WP34S_BesselJ(&n, &x, &r, &ctxtReal75);
      WP34S_Mod(&n, const_2, &a, &ctxtReal75);
      realMultiply(&a, const_pi, &a, &ctxtReal75);
      realPolarToRectangular(&r, &a, &r, &a, &ctxtReal75);
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
      realToReal34(&r, REGISTER_REAL34_DATA(REGISTER_X));
      realToReal34(&a, REGISTER_IMAG34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        moreInfoOnError("In function fnBessel:", "negative argument for Bessel function of non-integer degree", NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
  }

  adjustResult(REGISTER_X, true, true, REGISTER_X, REGISTER_Y, -1);
}



// Hankel's asymptotic expansion (based on Abramowitz and Steven, p.364)
static void bessel_asymptotic_large_x(const real_t *alpha, const real_t *x, real_t *res, realContext_t *realContext) {
  real_t p, q, pp, qq, chi, sChi, cChi, mu, z8, k21, k21sq, nm, tmp;
  int32_t k;

  realMultiply(alpha, const_2, &chi, realContext);
  realAdd(&chi, const_1, &chi, realContext);
  realMultiply(&chi, const_piOn4, &chi, realContext);
  WP34S_Mod(x, const1071_2pi, &tmp, realContext);
  realSubtract(&tmp, &chi, &tmp, realContext);
  WP34S_Mod(&tmp, const1071_2pi, &chi, realContext);

  WP34S_SinCosTanTaylor(&chi, false, &sChi, &cChi, NULL, realContext);

  int32ToReal(4, &tmp);
  realMultiply(alpha, alpha, &mu, realContext);
  realMultiply(&mu, &tmp, &mu, realContext);

  realCopy(const_1, &p);
  realSubtract(&mu, const_1, &q, realContext);
  int32ToReal(8, &tmp);
  realMultiply(x, &tmp, &z8, realContext);
  realDivide(&q, &z8, &q, realContext);
  realCopy(const_1, &k21);
  realCopy(&q, &nm), realCopy(const_1, &qq);
  for(k = 2; k < 1000; ++k) {
    if(k % 2 == 0) {
      realCopy(&p, &pp);
    }
    else {
      realCopy(&q, &qq);
    }

    realAdd(&k21, const_2, &k21, realContext);
    realMultiply(&k21, &k21, &k21sq, realContext);
    realSubtract(&mu, &k21sq, &k21sq, realContext);
    realMultiply(&nm, &k21sq, &nm, realContext);
    realDivide(&nm, &z8, &nm, realContext);
    int32ToReal(k, &tmp);
    realDivide(&nm, &tmp, &nm, realContext);

    if(k % 4 < 2) {
      realAdd(     k % 2 ? &q : &p, &nm, k % 2 ? &q : &p, realContext);
    }
    else {
      realSubtract(k % 2 ? &q : &p, &nm, k % 2 ? &q : &p, realContext);
    }

    realCopy(const_1, &tmp), tmp.exponent -= 73;
    if(WP34S_RelativeError(&p, &pp, &tmp, realContext) && WP34S_RelativeError(&q, &qq, &tmp, realContext)) {
      break;
    }
  }

  realMultiply(&p, &cChi, &p, realContext);
  realMultiply(&q, &sChi, &q, realContext);
  realSubtract(&p, &q, &p, realContext);

  realDivide(const_2, const_pi, &q, realContext);
  realDivide(&q, x, &q, realContext);
  realSquareRoot(&q, &q, realContext);
  realMultiply(&p, &q, res, realContext);
}



// Polynomial U[k] (based on Abramowitz and Steven, p.366)
#define NUMBER_OF_COEFF   100
#define COEFF_BUFFER_SIZE (REAL_SIZE * NUMBER_OF_COEFF)
static void u_k(uint32_t k, const real_t *coeff/*array*/, const real_t *t_r, const real_t *t_i, real_t *res_r, real_t *res_i, realContext_t *realContext) {
  real_t t_n_r, t_n_i, tmp_r, tmp_i;
  uint32_t i;

  realCopy(t_r, &t_n_r), realCopy(t_i, &t_n_i);
  realCopy(const_0, res_r), realCopy(const_0, res_i);
  for(i = 1; i < k; ++i) {
    mulComplexComplex(&t_n_r, &t_n_i, t_r, t_i, &t_n_r, &t_n_i, realContext);
  }
  for(i = 0; i < NUMBER_OF_COEFF; ++i) {
    if(realIsZero(&coeff[i])) {
      break;
    }
    mulComplexComplex(&coeff[i], const_0, &t_n_r, &t_n_i, &tmp_r, &tmp_i, realContext);
    realAdd(res_r, &tmp_r, res_r, realContext), realAdd(res_i, &tmp_i, res_i, realContext);
    mulComplexComplex(&t_n_r, &t_n_i, t_r, t_i, &t_n_r, &t_n_i, realContext);
    mulComplexComplex(&t_n_r, &t_n_i, t_r, t_i, &t_n_r, &t_n_i, realContext);
  }
}
static void Sigma_u_k(const real_t *nu, const real_t *t_r, const real_t *t_i, bool_t odd, bool_t even, real_t *res_r, real_t *res_i, realContext_t *realContext) {
  real_t *coeff_current, *coeff_deriv, *coeff_next, *coeff_tmpptr = NULL;
  real_t nu_k, tmp, tmp2, prev_r, prev_i;
  uint32_t i, j;

  // TODO: may be allocate that memory on the heap (or on the stack)
  coeff_current = (real_t *)allocWp43s(COEFF_BUFFER_SIZE);
  coeff_deriv   = (real_t *)allocWp43s(COEFF_BUFFER_SIZE);
  coeff_next    = (real_t *)allocWp43s(COEFF_BUFFER_SIZE);

  realCopy(const_0, &prev_r), realCopy(const_0, &prev_i);
  realCopy(even ? const_1 : const_0, res_r), realCopy(const_0, res_i);
  realCopy(nu, &nu_k);

  int32ToReal(24, &tmp);
  realDivide(const_3, &tmp, &coeff_current[0], realContext);
  realDivide(const_5, &tmp, &coeff_current[1], realContext);
  realChangeSign(&coeff_current[1]);
  for(i = 2; i < NUMBER_OF_COEFF; ++i) {
    realZero(&coeff_current[i]);
  }

  for(i = 0; i < NUMBER_OF_COEFF; ++i) {
    realZero(&coeff_deriv[i]);
    realZero(&coeff_next[i]);
  }

  for(i = 1; i < NUMBER_OF_COEFF; ++i) {
   if(((i % 2 == 1) && odd) || ((i % 2 == 0) && even)) {
     u_k(i, coeff_current, t_r, t_i, &tmp, &tmp2, realContext);
     divComplexComplex(&tmp, &tmp2, &nu_k, const_0, &tmp, &tmp2, realContext);
     if((!realIsSpecial(&tmp)) && !realIsSpecial(&tmp2)) {
       realAdd(res_r, &tmp, res_r, realContext), realAdd(res_i, &tmp2, res_i, realContext);
     }
     realCopy(const_1, &tmp), tmp.exponent -= 73;
     if(WP34S_RelativeError(res_r, &prev_r, &tmp, realContext) && WP34S_RelativeError(res_i, &prev_i, &tmp, realContext)) {
       break;
     }
     realCopy(res_r, &prev_r), realCopy(res_i, &prev_i);
   }

   // for the next iteration
   realMultiply(&nu_k, nu, &nu_k, realContext);

   for(j = 0; j < NUMBER_OF_COEFF; ++j) { // coefficients of the derivative
     int32ToReal(i + j * 2, &tmp);
     realMultiply(&coeff_current[j], &tmp, &coeff_deriv[j], realContext);
     if(realIsZero(&coeff_deriv[j])) {
       break;
     }
   }
   for(j = 0; j < NUMBER_OF_COEFF; ++j) { // x^2
     realCopy(&coeff_deriv[j], &coeff_next[j]);
     if(realIsZero(&coeff_deriv[j])) {
       break;
     }
   }
   for(j = 1; j < NUMBER_OF_COEFF; ++j) { // -x^4
     realSubtract(&coeff_next[j], &coeff_deriv[j - 1], &coeff_next[j], realContext);
     if(realIsZero(&coeff_deriv[j - 1])) {
       break;
     }
   }
   for(j = 0; j < NUMBER_OF_COEFF; ++j) { // /2
     realMultiply(&coeff_next[j], const_1on2, &coeff_next[j], realContext);
     if(realIsZero(&coeff_next[j])) {
       break;
     }
   }

   for(j = 0; j < NUMBER_OF_COEFF; ++j) { // 5x
     realMultiply(&coeff_current[j], const_5, &coeff_deriv[j], realContext);
     if(realIsZero(&coeff_deriv[j])) {
       break;
     }
   }
   for(j = 1; j < NUMBER_OF_COEFF; ++j) { // (1-5t^2)x
     realSubtract(&coeff_current[j], &coeff_deriv[j - 1], &coeff_current[j], realContext);
     if(realIsZero(&coeff_deriv[j - 1])) {
       break;
     }
   }
   for(j = 0; j < NUMBER_OF_COEFF; ++j) { // integrate
     int32ToReal(i + j * 2 + 1, &tmp);
     realDivide(&coeff_current[j], &tmp, &coeff_deriv[j], realContext);
     if(realIsZero(&coeff_deriv[j])) {
       break;
     }
   }
   int32ToReal(8, &tmp);
   for(j = 0; j < NUMBER_OF_COEFF; ++j) { // 1/8
     realDivide(&coeff_deriv[j], &tmp, &tmp2, realContext);
     realAdd(&coeff_next[j], &tmp2, &coeff_next[j], realContext);
     if(realIsZero(&coeff_deriv[j])) {
       break;
     }
   }

   coeff_tmpptr = coeff_current;
   coeff_current = coeff_next;
   coeff_next = coeff_tmpptr;
   coeff_tmpptr = NULL;
  }
  freeWp43s(coeff_current, COEFF_BUFFER_SIZE);
  freeWp43s(coeff_deriv  , COEFF_BUFFER_SIZE);
  freeWp43s(coeff_next   , COEFF_BUFFER_SIZE);
}
#undef COEFF_BUFFER_SIZE
#undef NUMBER_OF_COEFF

// Debye's asymptotic expansion (based on Abramowitz and Steven, p.366)
static void bessel_asymptotic_large_order_hyp(const real_t *nu, const real_t *x, real_t *res, realContext_t *realContext) {
  real_t alpha, tanh_alpha, coefficient, itrval, t, tmp;

  // nu * sech(alpha) = nu / cosh(alpha) = x
  realDivide(nu, x, &alpha, realContext);
  realArcosh(&alpha, &alpha, realContext);

  // coefficient numerator
  WP34S_Tanh(&alpha, &tanh_alpha, realContext);
  realSubtract(&tanh_alpha, &alpha, &coefficient, realContext);
  realMultiply(nu, &coefficient, &coefficient, realContext);
  realExp(&coefficient, &coefficient, realContext);

  // coefficient denominator
  realMultiply(const_2pi, nu, &tmp, realContext);
  realMultiply(&tmp, &tanh_alpha, &tmp, realContext);
  realSquareRoot(&tmp, &tmp, realContext);
  realDivide(&coefficient, &tmp, &coefficient, realContext);

  realDivide(const_1, &tanh_alpha, &t, realContext);
  Sigma_u_k(nu, &t, const_0, true, true, &itrval, &tmp, realContext);
  realMultiply(&coefficient, &itrval, res, realContext);
}

static void bessel_asymptotic_large_order_trig(const real_t *nu, const real_t *x, real_t *res, realContext_t *realContext) {
  real_t beta, sin_beta, cos_beta, tan_beta, cot_beta, coefficient, psi, sin_psi, cos_psi, lr, li, mr, mi;

  // nu * sec(beta) = nu / cos(beta) = x
  realDivide(nu, x, &cos_beta, realContext);
  WP34S_Acos(&cos_beta, &beta, realContext);

  realMultiply(&cos_beta, &cos_beta, &sin_beta, realContext); // cos²β
  realSubtract(const_1, &sin_beta, &sin_beta, realContext); // sin²β
  realSquareRoot(&sin_beta, &sin_beta, realContext); // sinβ
  realDivide(&sin_beta, &cos_beta, &tan_beta, realContext); // tanβ

  // coefficient
  realDivide(const_2, const_pi, &coefficient, realContext);
  realDivide(&coefficient, nu, &coefficient, realContext);
  realDivide(&coefficient, &tan_beta, &coefficient, realContext);
  realSquareRoot(&coefficient, &coefficient, realContext);

  // psi = nu * (tan(beta) - beta) - pi/4
  realSubtract(&tan_beta, &beta, &psi, realContext);
  realMultiply(&psi, nu, &psi, realContext);
  realSubtract(&psi, const_piOn4, &psi, realContext);
  WP34S_Mod(&psi, const1071_2pi, &psi, realContext);
  WP34S_SinCosTanTaylor(&psi, false, &sin_psi, &cos_psi, NULL, realContext);

  realDivide(const_1, &tan_beta, &cot_beta, realContext);
  Sigma_u_k(nu, const_0, &cot_beta, false, true, &lr, &li, realContext);
  Sigma_u_k(nu, const_0, &cot_beta, true, false, &mr, &mi, realContext);
  mulComplexComplex(&mr, &mi, const_0, const__1, &mr, &mi, realContext);
  // li == mi == 0 ?

  realMultiply(&lr, &cos_psi, &lr, realContext);
  realMultiply(&mr, &sin_psi, &mr, realContext);
  realAdd(&lr, &mr, res, realContext);
  realMultiply(res, &coefficient, res, realContext);
}



// Recurrence relations (Abramowitz and Steven, p.361)
static void plusMinus(bool_t subtracting, const real_t *a, const real_t *b, real_t *res, realContext_t *realContext) {
  if(subtracting) {
    realSubtract(a, b, res, realContext);
  }
  else {
    realAdd(a, b, res, realContext);
  }
}
static void bessel_recur(const real_t *nu, const real_t *x, bool_t descending, real_t *res, realContext_t *realContext) {
  real_t jnx, jn_1x, alpha, floor_nu;

  realToIntegralValue(nu, &floor_nu, DEC_ROUND_FLOOR, realContext);
  plusMinus(!descending, nu, &floor_nu, &alpha, realContext);
  WP34S_BesselJ(&alpha, x, &jn_1x, realContext);
  plusMinus(descending, &alpha, const_1, &alpha, realContext);
  WP34S_BesselJ(&alpha, x, &jnx, realContext);
  while(realCompareLessThan(descending ? nu : &alpha, descending ? &alpha : nu)) {
    realMultiply(const_2, &alpha, res, realContext);
    realDivide(res, x, res, realContext);
    realMultiply(res, &jnx, res, realContext);
    realSubtract(res, &jn_1x, res, realContext);

    plusMinus(descending, &alpha, const_1, &alpha, realContext);
    realCopy(&jnx, &jn_1x);
    realCopy(res, &jnx);
  }
}



/* The code below is ported from the WP 34s repository,
 * but never implemented in it. */
static void bessel(const real_t *alpha, const real_t *x, bool_t neg, real_t *res, realContext_t *realContext) {
  real_t q, r, m;
  real_t x2on4, term, gfac;
  int16_t n;

  realDivide(x, const_2, &q, realContext);				// q = x/2
  realPower(&q, const_2, &x2on4, realContext);		// factor each time around
  realPower(&q, alpha, &r, realContext);		// (x/2)^(2m+alpha)

  realAdd(alpha, const_1, &gfac, realContext);
  WP34S_Gamma(&gfac, &q, realContext);
  realDivide(&r, &q, &term, realContext);
  realCopy(&term, res);			// first term in series

  realZero(&m);

  for(n=0; n<1000; n++) {
    realMultiply(&term, &x2on4, &q, realContext);
    realAdd(&m, const_1, &m, realContext);			// m = m+1
    realDivide(&q, &m, &r, realContext);
    realDivide(&r, &gfac, &term, realContext);
    realAdd(&gfac, const_1, &gfac, realContext);
    if(neg) {
      realChangeSign(&term);
    }
    realAdd(&term, res, &q, realContext);
    if(realCompareEqual(res, &q)) {
      return;
    }
    realCopy(&q, res);
  }
  realCopy(const_NaN, res);
  return;
}

void WP34S_BesselJ(const real_t *alpha, const real_t *x, real_t *res, realContext_t *realContext) {
  real_t a, beta, gamma;

  if(realIsNaN(alpha) || realIsSpecial(x)) {
    realCopy(const_NaN, res);
    return;
  }
  if(realIsZero(x)) {
    if(realIsZero(alpha)) {
      realCopy(const_1, res);
      return;
    }
    realZero(res);
    return;
  }
  realCopy(alpha, &a);
  if(realIsNegative(alpha) && realIsAnInteger(alpha)) {
    realSetPositiveSign(&a);
  }
  realSubtract(const_1, const_1on4, &beta, realContext);
  realMultiply(&a, &beta, &beta, realContext);
  realAdd(const_1, const_1on4, &gamma, realContext);
  realMultiply(&a, &gamma, &gamma, realContext);
  if(realCompareGreaterThan(&a, const_90)) {
    if(realCompareAbsLessThan(x, &beta)) {
      bessel_asymptotic_large_order_hyp(&a, x, res, realContext);
    }
    else if(realCompareAbsGreaterThan(x, &gamma)) {
      bessel_asymptotic_large_order_trig(&a, x, res, realContext);
    }
    else {
      bessel_recur(&a, x, realCompareAbsLessThan(x, &a), res, realContext);
    }
  }
  else if(realCompareAbsGreaterThan(x, const_90) && realCompareAbsGreaterThan(x, &gamma)) {
    bessel_asymptotic_large_x(&a, x, res, realContext);
  }
  else {
    bessel(&a, x, true, res, realContext);
  }
}
