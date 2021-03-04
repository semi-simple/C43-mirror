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

#include "wp43s.h"



static bool_t besselGetParam(calcRegister_t regist, real_t *r, real_t *i, realContext_t *realContext) {
  switch(getRegisterDataType(regist)) {
    case dtComplex34:
      real34ToReal(REGISTER_REAL34_DATA(regist), r);
      real34ToReal(REGISTER_IMAG34_DATA(regist), i);
      return true;
    case dtReal34:
      if(getRegisterAngularMode(regist) == AM_NONE) {
        real34ToReal(REGISTER_REAL34_DATA(regist), r);
        realCopy(const_0, i);
        return true;
      }
      break;
    case dtLongInteger:
      convertLongIntegerRegisterToReal(regist, r, realContext);
      realCopy(const_0, i);
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
  real_t xr, xi, nr, ni, rr, ri;

  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(besselGetParam(REGISTER_X, &xr, &xi, &ctxtReal39) && besselGetParam(REGISTER_Y, &nr, &ni, &ctxtReal39)) {
    if(realIsZero(&xi) && realIsZero(&ni)) { // Real
      WP34S_BesselJ(&nr, &xr, &rr, &ctxtReal39);
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
      realToReal34(&rr, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else { // Complex
      WP34S_ComplexBesselJ(&nr, &ni, &xr, &xi, &rr, &ri, &ctxtReal39);
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
      realToReal34(&rr, REGISTER_REAL34_DATA(REGISTER_X));
      realToReal34(&ri, REGISTER_IMAG34_DATA(REGISTER_X));
    }
  }

  adjustResult(REGISTER_X, true, true, REGISTER_X, REGISTER_Y, -1);
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
		if(neg)
      realChangeSign(&term);
    realAdd(&term, res, &q, realContext);
		if(realCompareEqual(res, &q))
			return;
    realCopy(&q, res);
	}
  realCopy(const_NaN, res);
	return;
}

void WP34S_BesselJ(const real_t *alpha, const real_t *x, real_t *res, realContext_t *realContext) {
	real_t a;

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
	bessel(&a, x, true, res, realContext);
}

static void cmplx_bessel(const real_t *nx, const real_t *ny, const real_t *xx, const real_t *xy, bool_t neg, real_t *rx, real_t *ry, realContext_t *realContext) {
	real_t x2on4x, x2on4y, k, ux, uy;
	real_t t1, t2, a1, a2, b, dummy;
	int16_t i;

  divComplexComplex(xx, xy, const_2, const_0, &t1, &t2, realContext);
  PowerComplex(&t1, &t2, const_2, const_0, &x2on4x, &x2on4y, realContext);

  PowerComplex(&t1, &t2, nx, ny, &t1, &t2, realContext);
  realAdd(nx, const_1, &b, realContext);
  WP34S_ComplexGamma(&b, ny, &a1, &a2, realContext);
  divComplexComplex(&t1, &t2, &a1, &a2, &ux, &uy, realContext);

  realCopy(const_1, &k);
  realCopy(&ux, rx), realCopy(&uy, ry);

	for(i=0;i<1000;i++) {
    mulComplexComplex(&x2on4x, &x2on4y, &ux, &uy, &t1, &t2, realContext);
		if(neg) {
      realChangeSign(&t1), realChangeSign(&t2);
    }
    realAdd(nx, &k, &b, realContext);
    mulComplexComplex(&b, ny, &k, const_0, &a1, &a2, realContext);
    divComplexComplex(&t1, &t2, &a1, &a2, &ux, &uy, realContext);

    realAdd(&ux, rx, &a1, realContext), realAdd(&uy, ry, &a2, realContext);
    realSubtract(&a1, rx, &t1, realContext), realSubtract(&a2, ry, &t2, realContext);
    realRectangularToPolar(&t1, &t2, &b, &dummy, &ctxtReal39);
		if(realIsZero(&b))
			return;
    realCopy(&a1, rx), realCopy(&a2, ry);
    realAdd(&k, const_1, &k, realContext);
	}
  realCopy(const_NaN, rx), realCopy(const_NaN, ry);
}

void WP34S_ComplexBesselJ(const real_t *alphax, const real_t *alphay, const real_t *xx, const real_t *xy, real_t *rx, real_t *ry, realContext_t *realContext) {
	if(realIsZero(xy) && realIsZero(alphay)) {
		WP34S_BesselJ(alphax, xx, rx, realContext);
		if(realIsNaN(rx))
			realCopy(const_NaN, ry);
		else realZero(ry);
	} else if(realIsSpecial(alphax) || realIsSpecial(alphay) ||
			realIsSpecial(xx) || realIsSpecial(xy)) {
		realCopy(const_NaN, rx), realCopy(const_NaN, ry);
  }
	else
		cmplx_bessel(alphax, alphay, xx, xy, true, rx, ry, realContext);
}
