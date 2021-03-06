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

	if(besselGetParam(REGISTER_X, &xr, &xi, &ctxtReal75) && besselGetParam(REGISTER_Y, &nr, &ni, &ctxtReal75)) {
		if(realIsZero(&xi) && realIsZero(&ni)) { // Real
			if(realIsAnInteger(&nr) || (!realIsNegative(&xr))) {
				WP34S_BesselJ(&nr, &xr, &rr, &ctxtReal75);
				reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
				realToReal34(&rr, REGISTER_REAL34_DATA(REGISTER_X));
			}
			else if(getSystemFlag(FLAG_CPXRES)) { // Real -> Complex
				realSetPositiveSign(&xr);
				WP34S_BesselJ(&nr, &xr, &rr, &ctxtReal75);
				WP34S_Mod(&nr, const_2, &ri, &ctxtReal75);
				realMultiply(&ri, const_pi, &ri, &ctxtReal75);
				realPolarToRectangular(&rr, &ri, &rr, &ri, &ctxtReal75);
				goto complex_result;
			}
			else {
				displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
				#if (EXTRA_INFO_ON_CALC_ERROR == 1)
					moreInfoOnError("In function fnBessel:", "negative argument for Bessel function of non-integer degree", NULL, NULL);
				#endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
			}
		}
		else { // Complex
			WP34S_ComplexBesselJ(&nr, &ni, &xr, &xi, &rr, &ri, &ctxtReal75);
complex_result:
			reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
			realToReal34(&rr, REGISTER_REAL34_DATA(REGISTER_X));
			realToReal34(&ri, REGISTER_IMAG34_DATA(REGISTER_X));
		}
	}

	adjustResult(REGISTER_X, true, true, REGISTER_X, REGISTER_Y, -1);
}


static void debugShowReal(const char *label, const real_t *val) {
	char valtxt[256];
	realToString(val, valtxt);
	printf("%s %s\n", label, valtxt);
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
		if(k % 2 == 0) realCopy(&p, &pp);
		else           realCopy(&q, &qq);

		realAdd(&k21, const_2, &k21, realContext);
		realMultiply(&k21, &k21, &k21sq, realContext);
		realSubtract(&mu, &k21sq, &k21sq, realContext);
		realMultiply(&nm, &k21sq, &nm, realContext);
		realDivide(&nm, &z8, &nm, realContext);
		int32ToReal(k, &tmp);
		realDivide(&nm, &tmp, &nm, realContext);

		if(k % 4 < 2)
			realAdd(     k % 2 ? &q : &p, &nm, k % 2 ? &q : &p, realContext);
		else
			realSubtract(k % 2 ? &q : &p, &nm, k % 2 ? &q : &p, realContext);

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

static void bessel_complex_asymptotic_large_x(const real_t *alpha_r, const real_t *alpha_i, const real_t *x_r, const real_t *x_i, real_t *res_r, real_t *res_i, realContext_t *realContext) {
	real_t pr, qr, ppr, qqr, chiR, sr, cr, muR, z8r, nmR;
	real_t pi, qi, ppi, qqi, chiI, si, ci, muI, z8i, nmI;
	real_t k21, k21sq, tmp;
	int32_t k;

	mulComplexComplex(alpha_r, alpha_i, const_2, const_0, &chiR, &chiI, realContext);
	realAdd(&chiR, const_1, &chiR, realContext);
	mulComplexComplex(&chiR, &chiI, const_piOn4, const_0, &chiR, &chiI, realContext);
	realSubtract(x_r, &chiR, &chiR, realContext), realSubtract(x_i, &chiI, &chiI, realContext);

	sinComplex(&chiR, &chiI, &sr, &si, realContext);
	cosComplex(&chiR, &chiI, &cr, &ci, realContext);

	int32ToReal(4, &tmp);
	mulComplexComplex(alpha_r, alpha_i, alpha_r, alpha_i, &muR, &muI, realContext);
	mulComplexComplex(&muR, &muI, &tmp, const_0, &muR, &muI, realContext);

	realCopy(const_1, &pr), realCopy(const_0, &pi);
	realSubtract(&muR, const_1, &qr, realContext), realCopy(&muI, &qi);
	int32ToReal(8, &tmp);
	mulComplexComplex(x_r, x_i, &tmp, const_0, &z8r, &z8i, realContext);
	divComplexComplex(&qr, &qi, &z8r, &z8i, &qr, &qi, realContext);
	realCopy(const_1, &k21);
	realCopy(&qr, &nmR), realCopy(&qi, &nmI);
	realCopy(const_1, &qqr), realZero(&qqi);
	for(k = 2; k < 1000; ++k) {
		if(k % 2 == 0) {
			realCopy(&pr, &ppr), realCopy(&pi, &ppi);
		}
		else {
			realCopy(&qr, &qqr), realCopy(&qi, &qqi);
		}

		realAdd(&k21, const_2, &k21, realContext);
		realMultiply(&k21, &k21, &k21sq, realContext);
		realSubtract(&muR, &k21sq, &k21sq, realContext);
		mulComplexComplex(&nmR, &nmI, &k21sq, &muI, &nmR, &nmI, realContext);
		divComplexComplex(&nmR, &nmI, &z8r, &z8i, &nmR, &nmI, realContext);
		int32ToReal(k, &tmp);
		divComplexComplex(&nmR, &nmI, &tmp, const_0, &nmR, &nmI, realContext);

		if(k % 4 < 2){
			realAdd(k % 2 ? &qr : &pr, &nmR, k % 2 ? &qr : &pr, realContext);
			realAdd(k % 2 ? &qi : &pi, &nmI, k % 2 ? &qi : &pi, realContext);
		}
		else {
			realSubtract(k % 2 ? &qr : &pr, &nmR, k % 2 ? &qr : &pr, realContext);
			realSubtract(k % 2 ? &qi : &pi, &nmI, k % 2 ? &qi : &pi, realContext);
		}

		realCopy(const_1, &tmp), tmp.exponent -= 73;
		if(WP34S_RelativeError(&pr, &ppr, &tmp, realContext) &&
			WP34S_RelativeError( &pi, &ppi, &tmp, realContext) &&
			WP34S_RelativeError( &qr, &qqr, &tmp, realContext) &&
			WP34S_RelativeError( &qi, &qqi, &tmp, realContext)) {
				break;
		}
	}

	mulComplexComplex(&pr, &pi, &cr, &ci, &pr, &pi, realContext);
	mulComplexComplex(&qr, &qi, &sr, &si, &qr, &qi, realContext);
	realSubtract(&pr, &qr, &pr, realContext), realSubtract(&pi, &qi, &pi, realContext);

	realDivide(const_2, const_pi, &qr, realContext);
	divRealComplex(&qr, x_r, x_i, &qr, &qi, realContext);
	PowerComplex(&qr, &qi, const_1on2, const_0, &qr, &qi, realContext);
	mulComplexComplex(&pr, &pi, &qr, &qi, res_r, res_i, realContext);
	if(realIsZero(x_r)) realZero(res_r); // purely imaginary argument
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
	if(realCompareAbsGreaterThan(x, const_90))
		bessel_asymptotic_large_x(&a, x, res, realContext);
	else
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
	real_t r, theta;
	realRectangularToPolar(xx, xy, &r, &theta, realContext);
	if(realIsZero(xy) && realIsZero(alphay)) {
		WP34S_BesselJ(alphax, xx, rx, realContext);
		if(realIsNaN(rx))
			realCopy(const_NaN, ry);
		else realZero(ry);
	} else if(realIsSpecial(alphax) || realIsSpecial(alphay) ||
			realIsSpecial(xx) || realIsSpecial(xy)) {
		realCopy(const_NaN, rx), realCopy(const_NaN, ry);
	}
	else if(realCompareAbsGreaterThan(&r, const_90))
		bessel_complex_asymptotic_large_x(alphax, alphay, xx, xy, rx, ry, realContext);
	else
		cmplx_bessel(alphax, alphay, xx, xy, true, rx, ry, realContext);
}
