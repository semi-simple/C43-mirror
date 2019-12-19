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
 * \file wp34s.c
 ***********************************************/

#include "wp43s.h"


/******************************************************
 * This functions are borrowed from the WP34S project
 ******************************************************/

/* Have to be careful here to ensure that every function we call can handle
 * the increased size of the numbers we're using.
 */
void WP34S_Cvt2RadSinCosTan(const real_t *an, uint32_t angularMode, real_t *sin39, real_t *cos39, real_t *tan39) {
  bool_t sinNeg = false, cosNeg = false, swap = false;
  real39_t angle;

  if(realIsNaN(an)) {
    if(sin39 != NULL) {
     realCopy(const_NaN, sin39);
    }
    if(cos39 != NULL) {
      realCopy(const_NaN, cos39);
    }
    if(tan39 != NULL) {
      realCopy(const_NaN, tan39);
    }
   return;
  }

  realPlus(an, &angle, &ctxtReal39);

  if(angularMode == AM_DMS) {
    angularMode = AM_DEGREE;
    convertAngle39FromTo(&angle, AM_DMS, AM_DEGREE);
  }

  // sin(-x) = -sin(x), cos(-x) = cos(x)
  if(realIsNegative(&angle)) {
    sinNeg = true;
    realSetPositiveSign(&angle);
  }

  switch(angularMode) {
    case AM_DEGREE:
      realDivideRemainder(&angle, const_360,    &angle, &ctxtReal39); // mod(angle, 360°) --> angle
      break;

    case AM_GRAD:
      realDivideRemainder(&angle, const_400,    &angle, &ctxtReal39); // mod(angle, 400g) --> angle
      break;

    case AM_RADIAN:
      WP34S_Mod(&angle,           const459_2pi, &angle);              // mod(angle, 2pi) --> angle
      break;

    case AM_MULTPI:
      realDivideRemainder(&angle, const_2,      &angle, &ctxtReal39); // mod(angle, 2) --> angle
      break;

    default: {}
  }

  // sin(180+x) = -sin(x), cos(180+x) = -cos(x)
  if(realCompareGreaterEqual(&angle, angle180 + angularMode)) {        // angle >= 180°
    realSubtract(&angle, angle180 + angularMode, &angle, &ctxtReal39); // angle - 180° --> angle
    sinNeg = !sinNeg;
    cosNeg = !cosNeg;
  }

  // sin(90+x) = cos(x), cos(90+x) = -sin(x)
  if(realCompareGreaterEqual(&angle, angle90 + angularMode)) {        // angle >= 90°
    realSubtract(&angle, angle90 + angularMode, &angle, &ctxtReal39); // angle - 90° --> angle
    swap = true;
    cosNeg = !cosNeg;
  }

  // sin(90-x) = cos(x), cos(90-x) = sin(x)
  if(realCompareEqual(&angle, angle45 + angularMode)) { // angle == 45°
    if(sin39 != NULL) {
     realCopy(const_root2on2, sin39);
    }
    if(cos39 != NULL) {
      realCopy(const_root2on2, cos39);
    }
    if(tan39 != NULL) {
      realCopy(const_1, tan39);
    }
  }
  else { // angle < 90
    if(realCompareGreaterThan(&angle, angle45 + angularMode)) {         // angle > 45°
      realSubtract(angle90 + angularMode, &angle, &angle, &ctxtReal39); // 90° - angle  --> angle
      swap = !swap;
    }

    convertAngle39FromTo(&angle, angularMode, AM_RADIAN);
    WP34S_SinCosTanTaylor(&angle, swap, swap?cos39:sin39, swap?sin39:cos39, tan39); // angle in radian
  }

  if(sin39 != NULL) {
    if(sinNeg) {
      realSetNegativeSign(sin39);
      if(tan39 != NULL) realSetNegativeSign(tan39);
    }
    if(realIsZero(sin39)) {
      realSetPositiveSign(sin39);
      if(tan39 != NULL) {
        realSetPositiveSign(tan39);
      }
    }
  }

  if(cos39 != NULL) {
    if(cosNeg) {
      realSetNegativeSign(cos39);
      if(tan39 != NULL) realChangeSign(tan39);
    }
    if(realIsZero(cos39)) {
      realSetPositiveSign(cos39);
    }
  }

  if(tan39 != NULL && realIsZero(cos39)) {
      realSetPositiveSign(tan39);
  }
}


/* Calculate sin, cos by Taylor series and tan by division
 */
void WP34S_SinCosTanTaylor(const real_t *a39, bool_t swap, real_t *sinOut39, real_t *cosOut39, real_t *tanOut39) { // a in radian
  real51_t angle, a2, t, j, z, sin, cos, compare;
  int i, odd;
  bool_t finSin = (sinOut39 == NULL), finCos = (cosOut39 == NULL);
  int32_t cmp;

  realPlus(a39, &angle, &ctxtReal51);
  realMultiply(&angle, &angle, &a2, &ctxtReal51);
  uInt32ToReal(1, &j);
  uInt32ToReal(1, &t);
  uInt32ToReal(1, &sin);
  uInt32ToReal(1, &cos);

  for(i=1; !(finSin && finCos) && i < 1000; i++) {
    odd = i & 1;

    realAdd(&j, const_1, &j, &ctxtReal51);
    realDivide(&a2, &j, &z, &ctxtReal51);
    realMultiply(&t, &z, &t, &ctxtReal51);

    if(!finCos) {
      realCopy(&cos, &z);

      if(odd) {
        realSubtract(&cos, &t, &cos, &ctxtReal51);
      }
      else {
        realAdd(&cos, &t, &cos, &ctxtReal51);
      }

      realCompare(&cos, &z, &compare, &ctxtReal51);
      realToInt32(&compare, cmp);
      finCos = (cmp == 0);
    }

    realAdd(&j, const_1, &j, &ctxtReal51);
    realDivide(&t, &j, &t, &ctxtReal51);

    if(!finSin) {
      realCopy(&sin, &z);

      if(odd) {
        realSubtract(&sin, &t, &sin, &ctxtReal51);
      }
      else {
        realAdd(&sin, &t, &sin, &ctxtReal51);
      }

      realCompare(&sin, &z, &compare, &ctxtReal51);
      realToInt32(&compare, cmp);
      finSin = (cmp == 0);
    }
  }

  if(realIsZero(&cos)) {
    realSetPositiveSign(&cos);
  }

  if(realIsZero(&sin)) {
    realSetPositiveSign(&sin);
  }

  if(sinOut39 != NULL) {
    realMultiply(&sin, &angle, &sin, &ctxtReal51);
    realPlus(&sin, sinOut39, &ctxtReal39);
  }

  if(cosOut39 != NULL) {
    realPlus(&cos, cosOut39, &ctxtReal39);
  }

  if(tanOut39 != NULL) {
    if(sinOut39 == NULL || cosOut39 == NULL) {
      realCopy(const_NaN, tanOut39);
    }
    else {
      if(swap) {
        realDivide(&cos, &sin, tanOut39, &ctxtReal39);
      }
      else {
        realDivide(&sin, &cos, tanOut39, &ctxtReal39);
      }
    }
  }
}



void WP34S_Atan(const real_t *x, real_t *angle) {
  real39_t a, b, a2, t, j, z, last;
  int doubles = 0;
  int invert;
  int n;
  int neg = realIsNegative(x);

  realCopy(x, &a);

  // arrange for a >= 0
  if(neg) {
    realChangeSign(&a);
  }

  // reduce range to 0 <= a < 1, using atan(x) = pi/2 - atan(1/x)
  invert = realCompareGreaterThan(&a, const_1);
  if(invert) {
    realDivide(const_1, &a, &a, &ctxtReal39);
  }

  // Range reduce to small enough limit to use taylor series using:
  //  tan(x/2) = tan(x)/(1+sqrt(1+tan(x)²))
  for(n=0; n<1000; n++) {
    if(realCompareLessEqual(&a, const_1on10)) {
      break;
    }
    doubles++;
    // a = a/(1+sqrt(1+a²)) -- at most 3 iterations.
    realMultiply(&a, &a, &b, &ctxtReal39);
    realAdd(&b, const_1, &b, &ctxtReal39);
    realSquareRoot(&b, &b, &ctxtReal39);
    realAdd(&b, const_1, &b, &ctxtReal39);
    realDivide(&a, &b, &a, &ctxtReal39);
  }

  // Now Taylor series
  // atan(x) = x(1-x²/3+x⁴/5-x⁶/7...)
  // We calculate pairs of terms and stop when the estimate doesn't change
  realCopy(const_3, angle);
  realCopy(const_5, &j);
  realMultiply(&a, &a, &a2, &ctxtReal39); // a²
  realCopy(&a2, &t);
  realDivide(&t, angle, angle, &ctxtReal39); // s = 1-t/3 -- first two terms
  realSubtract(const_1, angle, angle, &ctxtReal39);

  do { // Loop until there is no digits changed
    realCopy(angle, &last);

    realMultiply(&t, &a2, &t, &ctxtReal39);
    realDivide(&t, &j, &z, &ctxtReal39);
    realAdd(angle, &z, angle, &ctxtReal39);

    realAdd(&j, const_2, &j, &ctxtReal39);

    realMultiply(&t, &a2, &t, &ctxtReal39);
    realDivide(&t, &j, &z, &ctxtReal39);
    realSubtract(angle, &z, angle, &ctxtReal39);

    realAdd(&j, const_2, &j, &ctxtReal39);

  } while(!realCompareEqual(angle, &last));

  realMultiply(angle, &a, angle, &ctxtReal39);

  while(doubles) {
    realAdd(angle, angle, angle, &ctxtReal39);
    doubles--;
  }

  if(invert) {
    realSubtract(const_piOn2, angle, angle, &ctxtReal39);
  }

  if(neg) {
    realChangeSign(angle);
  }
}



void WP34S_Atan2(const real_t *y, const real_t *x, real_t *atan) {
  real39_t r, t;
  const bool_t xNeg = realIsNegative(x);
  const bool_t yNeg = realIsNegative(y);

  if(realIsNaN(x) || realIsNaN(y)) {
    realCopy(const_NaN, atan);
    return;
  }

  if(realCompareEqual(y, const_0)) {
    if(yNeg) {
      if(realCompareEqual(x, const_0)) {
        if(xNeg) {
          realMinus(const_pi, atan, &ctxtReal39);
        }
        else {
          realCopy(y, atan);
        }
      }
      else if(xNeg) {
        realMinus(const_pi, atan, &ctxtReal39);
      }
      else {
        realCopy(y, atan);
      }
    }
    else {
      if(realCompareEqual(x, const_0)) {
        if(xNeg) {
          realCopy(const_pi, atan);
        }
        else {
          realZero(atan);
        }
      }
      else if(xNeg) {
        realCopy(const_pi, atan);
      }
      else {
        realZero(atan);
      }
    }
    return;
  }

  if(realCompareEqual(x, const_0)) {
    realCopy(const_piOn2, atan);
    if(yNeg) {
      realSetNegativeSign(atan);
    }
    return;
  }

  if(realIsInfinite(x)) {
    if(xNeg) {
      if(realIsInfinite(y)) {
        realCopy(const_3piOn4, atan);
        if(yNeg) {
          realSetNegativeSign(atan);
        }
      }
      else {
        realCopy(const_pi, atan);
        if(yNeg) {
          realSetNegativeSign(atan);
        }
      }
    }
    else {
      if(realIsInfinite(y)) {
        realCopy(const_piOn4, atan);
        if(yNeg) {
          realSetNegativeSign(atan);
        }
      }
      else {
        realZero(atan);
        if(yNeg) {
          realSetNegativeSign(atan);
        }
      }
    }
    return;
  }

  if(realIsInfinite(y)) {
    realCopy(const_piOn2, atan);
    if(yNeg) {
      realSetNegativeSign(atan);
    }
    return;
  }

  realDivide(y, x, &t, &ctxtReal39);
  WP34S_Atan(&t, &r);
  if(xNeg) {
    realCopy(const_pi, &t);
    if(yNeg) {
     realSetNegativeSign(&t);
    }
  }
  else {
    realZero(&t);
  }

  realAdd(&r, &t, atan, &ctxtReal39);
  if(realCompareEqual(atan, const_0) && yNeg) {
    realSetNegativeSign(atan);
  }
}



void WP34S_Asin(const real_t *x, real_t *angle) {
  real39_t abx, z;

  if(realIsNaN(x)) {
    realCopy(const_NaN, angle);
    return;
  }

  realCopyAbs(x, &abx);
  if(realCompareGreaterThan(&abx, const_1)) {
    realCopy(const_NaN, angle);
    return;
  }

  // angle = 2*atan(x/(1+sqrt(1-x*x)))
  realMultiply(x, x, &z, &ctxtReal39);
  realSubtract(const_1, &z, &z, &ctxtReal39);
  realSquareRoot(&z, &z, &ctxtReal39);
  realAdd(&z, const_1, &z, &ctxtReal39);
  realDivide(x, &z, &z, &ctxtReal39);
  WP34S_Atan(&z, &abx);
  realAdd(&abx, &abx, angle, &ctxtReal39);
}



void WP34S_Acos(const real_t *x, real_t *angle) {
  real39_t abx, z;

  if(realIsNaN(x)) {
    realCopy(const_NaN, angle);
    return;
  }

  realCopyAbs(x, &abx);
  if(realCompareGreaterThan(&abx, const_1)) {
    realCopy(const_NaN, angle);
    return;
  }

  // angle = 2*atan((1-x)/sqrt(1-x*x))
  if(realCompareEqual(x, const_1)) {
    realZero(angle);
  }
  else {
    realMultiply(x, x, &z, &ctxtReal39);
    realSubtract(const_1, &z, &z, &ctxtReal39);
    realSquareRoot(&z, &z, &ctxtReal39);
    realSubtract(const_1, x, &abx, &ctxtReal39);
    realDivide(&abx, &z, &z, &ctxtReal39);
    WP34S_Atan(&z, &abx);
    realAdd(&abx, &abx, angle, &ctxtReal39);
  }
}



bool_t WP34S_RelativeError51(const real_t *x, const real_t *y, const real_t *tol) {
  real51_t a;

  if(realCompareEqual(x, const_0)) {
    realCopyAbs(y, &a);
    return realCompareLessThan((real_t *)&a, tol);
  }

  realSubtract(x, y, &a, &ctxtReal51);
  realDivide(&a, x, &a, &ctxtReal51);
  realSetPositiveSign(&a);
  return realCompareLessThan((real_t *)&a, tol);
}


void WP34S_Ln51(const real_t *xin, real_t *res) {
  real51_t z, t, f, n, m, i, v, w, e;
  int32_t expon;

  if(realIsSpecial(xin)) {
    if(realIsNaN(xin) || realIsNegative(xin)) {
      realCopy(const_NaN, res);
      return;
    }

    realCopy(const_plusInfinity, res);
    return;
  }

  if(realCompareLessEqual(xin, const_0)) {
    if(realIsNegative(xin)) {
      realCopy(const_NaN, res);
      return;
    }

    realCopy(const_minusInfinity, res);
    return;
  }

  realPlus(xin, &z, &ctxtReal51);
  realPlus(const_2, &f, &ctxtReal51);
  realSubtract(xin, const_1, &t, &ctxtReal51);
  realCopy(&t, &v);
  realSetPositiveSign(&v);
  if(realCompareGreaterThan((real_t *)&v, const_1on2)) {
    expon = z.exponent + z.digits;
    z.exponent = -z.digits;
  }
  else {
    expon = 0;
  }

  // Range reduce the value by repeated square roots.
  // Making the constant here larger will reduce the number of later
  // iterations at the expense of more square root operations.
  while(realCompareLessEqual((real_t *)&z, const_root2on2)) {
    realMultiply(&f, const_2, &f, &ctxtReal51);
    realSquareRoot(&z, &z, &ctxtReal51);
  }

  realAdd(&z, const_1, &t, &ctxtReal51);
  realSubtract(&z, const_1, &v, &ctxtReal51);
  realDivide(&v, &t, &n, &ctxtReal51);
  realCopy(&n, &v);
  realMultiply(&v, &v, &m, &ctxtReal51);
  realCopy(const_3, &i);

  for(;;) {
    realMultiply(&m, &n, &n, &ctxtReal51);
    realDivide(&n, &i, &e, &ctxtReal51);
    realAdd(&v, &e, &w, &ctxtReal51);
    if(WP34S_RelativeError((real_t *)&w, (real_t *)&v, const_1e_49)) {
      break;
    }
    realCopy(&w, &v);
    realAdd(&i, const_2, &i, &ctxtReal51);
  }

  realMultiply(&f, &w, res, &ctxtReal51);
  if(expon == 0) {
    return;
  }

  int32ToReal(expon, &e);
  realMultiply(&e, const_ln10, &w, &ctxtReal51);
  realAdd(res, &w, res, &ctxtReal39);
}


static void WP34S_Calc_Gamma_LnGamma_Lanczos(const real_t *xin, real_t *res, bool_t calculateLnGamma) {
  real51_t r, s, t, u, v, x;
  int32_t k;

  realSubtract(xin, const_1, &x, &ctxtReal51);
  realZero(&s);
  realAdd(&x, const_29, &t, &ctxtReal51);
  for(k=28; k>=0; k--) {
    realDivide((real_t *)(((real51_t *)gammaLanczosCoefficients) + k), &t, &u, &ctxtReal51);
    realSubtract(&t, const_1, &t, &ctxtReal51);
    realAdd(&s, &u, &s, &ctxtReal51);
  }

  realAdd(&s, const_gammaC00, &t, &ctxtReal51);
  realLn(&t, &s, &ctxtReal51);

  //  r = z + g + 0.5;
  realAdd(&x, const_gammaR, &r, &ctxtReal51); // const_gammaR is g + 0.5

  //  r = log(R[0][0]) + (z+0.5) * log(r) - r;
  realLn(&r, &u, &ctxtReal51);
  realAdd(&x, const_1on2, &t, &ctxtReal51);
  realMultiply(&u, &t, &v, &ctxtReal51);

  realSubtract(&v, &r, &u, &ctxtReal51);

  if(calculateLnGamma) {
    realAdd(&u, &s, res, &ctxtReal39);
  }
  else {
    realAdd(&u, &s, &x, &ctxtReal51);
    realExp(&x, res, &ctxtReal39);
  }
}


// common code for the [GAMMA] and LN[GAMMA]
static void WP34S_Gamma_LnGamma(const real_t *xin, const bool_t calculateLnGamma, real_t *res) {
  real39_t x, t;
  bool_t reflect = false;

  // Check for special cases
  if(realIsSpecial(xin)) {
    if(realIsInfinite(xin) && realIsPositive(xin)) {
      realCopy(const_plusInfinity, res);
      return;
    }

    realCopy(const_NaN, res);
    return;
  }

  // Handle x approximately zero case
  if(realCompareAbsLessThan(xin, const_1e_24)) {
    if(realIsZero(xin)) {
      realCopy(const_NaN, res);
      return;
    }
    realDivide(const_1, xin, &x, &ctxtReal39);
    realSubtract(&x, const_egamma, res, &ctxtReal39);
    if(calculateLnGamma) {
      WP34S_Ln(res, res);
    }
    return;
  }

  // Correct our argument and begin the inversion if it is negative
  if(realCompareLessEqual(xin, const_0)) {
    reflect = true;
    realSubtract(const_1, xin, &t, &ctxtReal39); // t = 1 - xin
    if(realIsAnInteger(&t)) {
      realCopy(const_NaN, res);
      return;
    }
  }
  else {
    // Provide a fast path evaluation for positive integer arguments that aren't too large
    // The threshold for overflow is 205! (i.e. 204! is within range and 205! isn't).
    if(realIsAnInteger(xin) && realCompareLessEqual(xin, const_205)) {
      realSubtract(xin, const_1, &x, &ctxtReal39); // x = xin - 1
      realPlus(const_1, res, &ctxtReal39);
      while(realCompareGreaterEqual(&x, const_2)) {
        realMultiply(res, &x, res, &ctxtReal39);
        realSubtract(&x, const_1, &x, &ctxtReal39);
      }
      if(calculateLnGamma) {
        WP34S_Ln(res, res);
      }
      return;
    }
    realPlus(xin, &t, &ctxtReal39); // t = xin
  }

  WP34S_Calc_Gamma_LnGamma_Lanczos(&t, res, calculateLnGamma); // t=1-xin or t=xin and res=gamma(xin) or res=lngamma(xin) or res=gamma(1-xin) or res=lngamma(1-xin)

  if(reflect) {
    // figure out xin * PI mod 2PI
    realDivideRemainder(xin, const_2, &t, &ctxtReal39);
    realMultiply(&t, const_pi, &t, &ctxtReal39);      // t = xin·pi
    WP34S_SinCosTanTaylor(&t, false, &x, NULL, NULL); // x = sin(xin·pi)

    if(calculateLnGamma) {
      realDivide(const_pi, &x, &t, &ctxtReal39);      // t = pi / sin(pi·xin)
      realLn(&t, &t, &ctxtReal39);                    // t = ln(pi / sin(pi·xin))
      realSubtract(&t, res, res, &ctxtReal39);        // res = ln(pi / sin(pi·xin)) - lngamma(1-xin)
    }
    else {
      realMultiply(&x, res, &t, &ctxtReal39);         // t = sin(pi·xin) × gamma(1-xin)
      realDivide(const_pi, &t, res, &ctxtReal39);     // res = pi / (sin(pi·xin)·gamma(1-xin))
    }
  }
}



void WP34S_Factorial(const real_t *xin, real_t *res) {
  real39_t x;

  realAdd(xin, const_1, &x, &ctxtReal39);
  WP34S_Gamma_LnGamma(&x, false, res);
}



void WP34S_Gamma(const real_t *xin, real_t *res) {
  real39_t x;

  realCopy(xin, &x);
  WP34S_Gamma_LnGamma(&x, false, res);
}



void WP34S_LnGamma(const real_t *xin, real_t *res) {
  real39_t x;

  realCopy(xin, &x);
  WP34S_Gamma_LnGamma(&x, true, res);
}



/* Natural logarithm.
 *
 * Take advantage of the fact that we store our numbers in the form: m * 10^e
 * so log(m * 10^e) = log(m) + e * log(10)
 * do this so that m is always in the range 0.1 <= m < 2.  However if the number
 * is already in the range 0.5 .. 1.5, this step is skipped.
 *
 * Then use the fact that ln(x²) = 2 * ln(x) to range reduce the mantissa
 * into 1/sqrt(2) <= m < 2.
 *
 * Finally, apply the series expansion:
 *   ln(x) = 2(a+a³/3+a⁵/5+...) where a=(x-1)/(x+1)
 * which converges quickly for an argument near unity.
 */
void WP34S_Ln(const real_t *xin, real_t *res) {
  real39_t z, t, f, n, m, i, v, w, e;
  int32_t expon;

  if(realIsSpecial(xin)) {
    if(realIsNaN(xin) || realIsNegative(xin)) {
      realCopy(const_NaN, res);
      return;
    }

    realCopy(const_plusInfinity, res);
    return;
  }

  if(realCompareLessEqual(xin, const_0)) {
    if(realIsNegative(xin)) {
      realCopy(const_NaN, res);
      return;
    }

    realCopy(const_minusInfinity, res);
    return;
  }

  realCopy(xin, &z);
  realCopy(const_2, &f);
  realSubtract(xin, const_1, &t, &ctxtReal39);
  realCopy(&t, &v);
  realSetPositiveSign(&v);
  if(realCompareGreaterThan(&v, const_1on2)) {
    expon = z.exponent + z.digits;
    z.exponent = -z.digits;
  }
  else {
    expon = 0;
  }

  /* The too high case never happens
  while(dn_le(const_2, &z)) {
    realMultiply(&f, const_2, &f, &ctxtReal39);
    realSquareRoot(&z, &z, &ctxtReal39);
  } */

  // Range reduce the value by repeated square roots.
  // Making the constant here larger will reduce the number of later
  // iterations at the expense of more square root operations.
  while(realCompareLessEqual(&z, const_root2on2)) {
    realMultiply(&f, const_2, &f, &ctxtReal39);
    realSquareRoot(&z, &z, &ctxtReal39);
  }

  realAdd(&z, const_1, &t, &ctxtReal39);
  realSubtract(&z, const_1, &v, &ctxtReal39);
  realDivide(&v, &t, &n, &ctxtReal39);
  realCopy(&n, &v);
  realMultiply(&v, &v, &m, &ctxtReal39);
  realCopy(const_3, &i);

  for(;;) {
    realMultiply(&m, &n, &n, &ctxtReal39);
    realDivide(&n, &i, &e, &ctxtReal39);
    realAdd(&v, &e, &w, &ctxtReal39);
    if(WP34S_RelativeError(&w, &v, const_1e_37)) {
      break;
    }
    realCopy(&w, &v);
    realAdd(&i, const_2, &i, &ctxtReal39);
  }

  realMultiply(&f, &w, res, &ctxtReal39);
  if(expon == 0) {
    return;
  }

  int32ToReal(expon, &e);
  realMultiply(&e, const_ln10, &w, &ctxtReal39);
  realAdd(res, &w, res, &ctxtReal39);
}



void WP34S_Log(const real_t *xin, const real_t *base, real_t *res) {
  real39_t y;

  if(realIsSpecial(xin)) {
    if(realIsNaN(xin) || realIsNegative(xin)) {
      realCopy(const_NaN, res);
      return;
    }

    realCopy(const_plusInfinity, res);
    return;
  }

  WP34S_Ln(xin, &y);

  realDivide(&y, base, res, &ctxtReal39);
}



void WP34S_Log2(const real_t *xin, real_t *res) {
  WP34S_Log(xin, const_ln2, res);
}



void WP34S_Log10(const real_t *xin, real_t *res) {
  WP34S_Log(xin, const_ln10, res);
}



void WP34S_Logxy(const real_t *yin, const real_t *xin, real_t *res) {
  real39_t lx;

  WP34S_Ln(xin, &lx);
  WP34S_Log(yin, &lx, res);
}



bool_t WP34S_RelativeError(const real_t *x, const real_t *y, const real_t *tol) {
  real39_t a;

  if(realCompareEqual(x, const_0)) {
    realCopyAbs(y, &a);
    return realCompareLessThan(&a, tol);
  }

  realSubtract(x, y, &a, &ctxtReal39);
  realDivide(&a, x, &a, &ctxtReal39);
  realSetPositiveSign(&a);
  return realCompareLessThan(&a, tol);
}


/* Hyperbolic functions.
 * We start with a utility routine that calculates sinh and cosh.
 * We do the sihn as (e^x - 1) (e^x + 1) / (2 e^x) for numerical stability
 * reasons if the value of x is smallish.
 */
void WP34S_SinhCosh(const real_t *x, real_t *sinhOut, real_t *coshOut) {
  real39_t t, u, v;

  if(sinhOut != NULL) {
    if(realCompareAbsLessThan(x, const_1on2)) {
      WP34S_ExpM1(x, &u);                           // u = e^x - 1
      realMultiply(&u, const_1on2, &t, &ctxtReal39);           // t = (e^x - 1) / 2

      realAdd(&u, const_1, &u, &ctxtReal39);                   // u = e^x
      realDivide(&t, &u, &v, &ctxtReal39);                     // v = (e^x - 1) / 2e^x

      realAdd(&u, const_1, &u, &ctxtReal39);                   // u = e^x + 1
      realMultiply(&u, &v, sinhOut, &ctxtReal39);              // sinhOut = (e^x - 1)(e^x + 1) / 2e^x
    }
    else {
      realExp(x, &u, &ctxtReal39);                             // u = e^x
      realDivide(const_1, &u, &v, &ctxtReal39);                // v = e^-x
      realSubtract(&u, &v, sinhOut, &ctxtReal39);              // sinhOut = (e^x + e^-x)
      realMultiply(sinhOut, const_1on2, sinhOut, &ctxtReal39); // sinhOut = (e^x + e^-x)/2
    }
  }
  if(coshOut != NULL) {
   realExp(x, &u, &ctxtReal39);                                // u = e^x
   realDivide(const_1, &u, &v, &ctxtReal39);                   // v = e^-x
   realAdd(&u, &v, coshOut, &ctxtReal39);                      // coshOut = (e^x + e^-x)
   realMultiply(coshOut, const_1on2, coshOut, &ctxtReal39);    // coshOut = (e^x + e^-x)/2
  }
}



void WP34S_Tanh(const real_t *x, real_t *res) {
  if(realCompareAbsGreaterThan(x, const_47)) { // equals 1 to 39 digits
    realCopy((realIsPositive(x) ? const_1 : const__1), res);
  }
  else {
    real39_t a, b;

    realAdd(x, x, &a, &ctxtReal39);        // a = 2x
    WP34S_ExpM1(&a, &b);                   // b = exp(2x) - 1
    realAdd(&b, const_2, &a, &ctxtReal39); // a = exp(2x) - 1 + 2 = exp(2x) + 1
    realDivide(&b, &a, res, &ctxtReal39);  // res = (exp(2x) - 1) / (exp(2x) + 1)
  }
}



void WP34S_ArcSinh(const real_t *x, real_t *res) {
  real39_t a;

  realMultiply(x, x, &a, &ctxtReal39);   // a = x²
  realAdd(&a, const_1, &a, &ctxtReal39); // a = x² + 1
  realSquareRoot(&a, &a, &ctxtReal39);   // a = sqrt(x²+1)
  realAdd(&a, const_1, &a, &ctxtReal39); // a = sqrt(x²+1)+1
  realDivide(x, &a, &a, &ctxtReal39);    // a = x / (sqrt(x²+1)+1)
  realAdd(&a, const_1, &a, &ctxtReal39); // a = x / (sqrt(x²+1)+1) + 1
  realMultiply(x, &a, &a, &ctxtReal39);  // y = x * (x / (sqrt(x²+1)+1) + 1)
  WP34S_Ln1P(&a, res);                  // res = ln(1 + (x * (x / (sqrt(x²+1)+1) + 1)))
}



void WP34S_ArcCosh(const real_t *xin, real_t *res) {
  real39_t x, z;

  realCopy(xin, &x);
  realMultiply(&x, &x, res, &ctxtReal39);      // res = x²
  realSubtract(res, const_1, &z, &ctxtReal39); // z = x² - 1
  realSquareRoot(&z, res, &ctxtReal39);        // res = sqrt(x²-1)
  realAdd(res, &x, &z, &ctxtReal39);           // z = x + sqrt(x²-1)
  WP34S_Ln(&z, res);                           // res = ln(x + sqrt(x²-1))
}



void WP34S_ArcTanh(const real_t *x, real_t *res) {
  real39_t y, z;

  // Not the obvious formula but more stable...
  realSubtract(const_1, x, &z, &ctxtReal39);      // z = 1-x
  realDivide(x, &z, &y, &ctxtReal39);             // y = x / (1-x)
  realMultiply(&y, const_2, &z, &ctxtReal39);     // z = 2x / (1-x)
  WP34S_Ln1P(&z, &y);                             // y = ln(1 + 2x / (1-x))
  realMultiply(&y, const_1on2, res, &ctxtReal39); // res = ln(1 + 2x / (1-x)) / 2
}



/* ln(1+x) */
void WP34S_Ln1P(const real_t *x, real_t *res) {
  real39_t u, v, w;

  if(realIsZero(x)) {
    realZero(res);
  }
  else {
    realAdd(x, const_1, &u, &ctxtReal39);       // u = x+1
    realSubtract(&u, const_1, &v, &ctxtReal39); // v = x
    if(realIsZero(&v)) {
      realCopy(x, res);
    }
    else {
      realDivide(x, &v, &w, &ctxtReal39);
      WP34S_Ln(&u, &v);
      realMultiply(&v, &w, res, &ctxtReal39);
    }
  }
}



/* exp(x)-1 */
void WP34S_ExpM1(const real_t *x, real_t *res) {
  real39_t u, v, w;

  realExp(x, &u, &ctxtReal39);
  realSubtract(&u, const_1, &v, &ctxtReal39);
  if(realIsZero(&v)) { // |x| is very little
    realCopy(x, res);
  }
  else {
    if(realCompareEqual(&v, const__1)) {
      realCopy(const__1, res);
    }
    else {
      realMultiply(&v, x, &w, &ctxtReal39);
      WP34S_Ln(&u, &v);
      realDivide(&w, &v, res, &ctxtReal39);
    }
  }
}



void WP34S_CalcComplexLnGamma_Lanczos(const real39_t *zReal, const real39_t *zImag, real39_t *resReal, real39_t *resImag) {
  real51_t rReal, sReal, tReal, uReal, vReal;
  real51_t rImag, sImag, tImag, uImag, vImag;
  int k;

  realZero(&uReal);
  realZero(&uImag);
  realAdd(zReal, const_29, &tReal, &ctxtReal51);
  realPlus(zImag, &tImag, &ctxtReal51);
  for (k=28; k>=0; k--) {
    divRe51Co51((real_t *)((real51_t *)gammaLanczosCoefficients + k), &tReal, &tImag, &sReal, &sImag);
    realSubtract(&tReal, const_1, &tReal, &ctxtReal51);
    realAdd(&uReal, &sReal, &uReal, &ctxtReal51);
    realAdd(&uImag, &sImag, &uImag, &ctxtReal51);
  }
  realAdd(&uReal, const_gammaC00, &tReal, &ctxtReal51);
  realCopy(&uImag, &tImag);
  lnCo51(&tReal, &tImag, &sReal, &sImag);  // (s1, s2)

  realAdd(zReal, const_gammaR, &rReal, &ctxtReal51);
  realCopy(zImag, &rImag);
  lnCo51(&rReal, &rImag, &uReal, &uImag);

  realAdd(zReal, const_1on2, &tReal, &ctxtReal51);
  realCopy(zImag, &tImag);
  mulCo51Co51(&tReal, &tImag, &uReal, &uImag, &vReal, &vImag);

  realSubtract(&vReal, &rReal, &uReal, &ctxtReal51);
  realSubtract(&vImag, zImag, &uImag, &ctxtReal51);

  realAdd(&uReal, &sReal, resReal, &ctxtReal39);
  realAdd(&uImag, &sImag, resImag, &ctxtReal39);
}


static void WP34S_ComplexGammaLnGamma(const real39_t *zReal, const real39_t *zImag, const bool_t calculateLnGamma, real39_t *resReal, real39_t *resImag) {
  real39_t sinPiZReal, tReal, uReal, xReal;
  real39_t sinPiZImag, tImag, uImag, xImag;
  bool_t reflect = false;

  // Check for special cases
/*  if(decNumberIsSpecial(xin) || decNumberIsSpecial(yin)) {
    if(decNumberIsNaN(xin) || decNumberIsNaN(yin))
      cmplx_NaN(rx, ry);
    else {
      if(decNumberIsInfinite(xin)) {
        if(decNumberIsInfinite(yin))
          cmplx_NaN(rx, ry);
        else if(decNumberIsNegative(xin))
          cmplx_NaN(rx, ry);
        else {
          set_inf(rx);
          decNumberZero(ry);
        }
      }
      else {
        decNumberZero(rx);
        decNumberZero(ry);
      }
    }
    return;
  }
*/
  // Correct our argument and begin the inversion if it is negative
  if(realIsNegative(zReal)) {
    reflect = true;
    realSubtract(const_1, zReal, &tReal, &ctxtReal39);
    if(realIsZero(zImag) && realIsAnInteger(&tReal)) {
      realCopy(const_NaN, resReal);
      realCopy(const_NaN, resImag);
      return;
    }
    realSubtract(&tReal, const_1, &xReal, &ctxtReal39);
    realMinus(zImag, &xImag, &ctxtReal39);
  }
  else {
    realSubtract(zReal, const_1, &xReal, &ctxtReal39);
    realCopy(zImag, &xImag);
  }

  // Sum the series
  WP34S_CalcComplexLnGamma_Lanczos(&xReal, &xImag, resReal, resImag);
  if(!calculateLnGamma) {
    expCo39(resReal, resImag, resReal, resImag);
  }

  // Finally invert if we started with a negative argument
  if(reflect) {
    realMultiply(zReal, const_pi, &tReal, &ctxtReal39);
    realMultiply(zImag, const_pi, &tImag, &ctxtReal39);
    sinCo39(&tReal, &tImag, &sinPiZReal, &sinPiZImag);
    if(!calculateLnGamma) {
      mulCo39Co39(&sinPiZReal, &sinPiZImag, resReal, resImag, &uReal, &uImag);
      divRe39Co39(const_pi, &uReal, &uImag, resReal, resImag);
    }
    else {
      divRe39Co39(const_pi, &sinPiZReal, &sinPiZImag, &uReal, &uImag);
      lnCo39(&uReal, &uImag, &tReal, &tImag);
      realSubtract(&tReal, resReal, resReal, &ctxtReal39);
      realSubtract(&tImag, resImag, resImag, &ctxtReal39);
    }
  }
}


void WP34S_ComplexGamma(const real39_t *zinReal, const real39_t *zinImag, real39_t *resReal, real39_t *resImag) {
  real39_t zReal, zImag;

  realCopy(zinReal, &zReal);
  realCopy(zinImag, &zImag);
  WP34S_ComplexGammaLnGamma(&zReal, &zImag, false, resReal, resImag);
}

void WP34S_ComplexLnGamma(const real39_t *zinReal, const real39_t *zinImag, real39_t *resReal, real39_t *resImag) {
  real39_t zReal, zImag;

  realCopy(zinReal, &zReal);
  realCopy(zinImag, &zImag);
  WP34S_ComplexGammaLnGamma(&zReal, &zImag, true, resReal, resImag);
}



void WP34S_Mod(const real_t *x, const real_t *y, real_t *res) {
 /* Declare a structure large enough to hold a really long number.
  * This structure is likely to be larger than is required.
  */
 real459_t out;

 realDivideRemainder(x, y, &out, &ctxtReal459);
 realPlus(&out, res, &ctxtReal39);
}


void WP34S_BigMod(const real_t *x, const real_t *y, real_t *res) {
 /* Declare a structure large enough to hold a really long number.
  * This structure is likely to be larger than is required.
  */
 real855_t out;

 realDivideRemainder(x, y, &out, &ctxtReal855);
 realPlus(&out, res, &ctxtReal39);
}


void WP34S_real39Mantissa(const real_t *x, real_t *res) {
	if(decNumberIsSpecial(x)) {
		 realCopy(const_NaN, res);
		 return;
	}

	if(realIsZero(x)) {
 		realZero(res);
 		return;
	}

	realCopy(x, res);
	res->exponent = 1 - res->digits;
}


int32_t WP34S_real39Exponent(const real_t *x) {
 	if(realIsSpecial(x)) {
 		 return -99999;
 	}

 	if(realIsZero(x)) {
 	 	return 0;
 	}

 	return x->exponent + x->digits - 1;
}
