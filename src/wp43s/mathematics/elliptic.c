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
 * \file elliptic.c
 ***********************************************/

#include "elliptic.h"

#include "constantPointers.h"
#include "conversionAngles.h"
#include "debug.h"
#include "error.h"
#include "flags.h"
#include "mathematics/agm.h"
#include "mathematics/arcsin.h"
#include "mathematics/comparisonReals.h"
#include "mathematics/division.h"
#include "mathematics/magnitude.h"
#include "mathematics/wp34s.h"
#include "realType.h"
#include "registers.h"
#include "registerValueConversions.h"
#include "wp43s.h"

#define ELLIPTIC_N	16

static void _calc_real_elliptic(real_t *sn, real_t *cn, real_t *dn, const real_t *u, const real_t *m) {
  real_t a, b, e, f, g;
  real_t MU[ELLIPTIC_N], NU[ELLIPTIC_N], C[ELLIPTIC_N], D[ELLIPTIC_N];
  real_t sin_umu, cos_umu, t, r;
  int n = 0;

#define mu(n)	(MU + (n))
#define nu(n)	(NU + (n))
#define c(n)	(C + (n))
#define d(n)	(D + (n))

  if (realIsNegative(m) || realCompareLessThan(const_1, m)) {
    realCopy(const_NaN, sn);
    realCopy(const_NaN, cn);
    realCopy(const_NaN, dn);
    return;
  }
  if (realCompareLessThan(m, const_1e_32)) {
    WP34S_Cvt2RadSinCosTan(u, amRadian, sn, cn, NULL, &ctxtReal39);
    realCopy(const_1, dn);
    return;
  }
  realSubtract(m, const_1, &a, &ctxtReal39);
  if (realCompareAbsLessThan(&a, const_1e_32)) {
    WP34S_SinhCosh(u, &a, &b, &ctxtReal39);
    realDivide(const_1, &b, cn, &ctxtReal39);
    realMultiply(&a, cn, sn, &ctxtReal39);
    realCopy(cn, dn);
    return;
  }
  realCopy(const_1, mu(0));
  realSubtract(const_1, m, &a, &ctxtReal39);
  realSquareRoot(&a, nu(0), &ctxtReal39);
  for (;;) {
    realAdd(mu(n), nu(n), &g, &ctxtReal39);
    realCopyAbs(&g, &a);
    realMultiply(&a, const_1e32, &b, &ctxtReal39);
    realAdd(&b, &b, &a, &ctxtReal39);
    realSubtract(mu(n), nu(n), &e, &ctxtReal39);
    realCopyAbs(&e, &f);
    //if (realCompareGreaterThan(&a, &f))
    //  break;
    realMultiply(&g, const_1on2, mu(n+1), &ctxtReal39);
    realMultiply(mu(n), nu(n), &a, &ctxtReal39);
    realSquareRoot(&a, nu(n+1), &ctxtReal39);
    n++;
    if (n >= ELLIPTIC_N-1)
      break;
  }

  realMultiply(u, mu(n), &a, &ctxtReal39);
  WP34S_Cvt2RadSinCosTan(&a, amRadian, &sin_umu, &cos_umu, NULL, &ctxtReal39);
  realCopyAbs(&cos_umu, &b);
  //if (realCompareAbsLessThan(&sin_umu, &b))
  //  realDivide(&sin_umu, &cos_umu, &t, &ctxtReal39);
  //else
    realDivide(&cos_umu, &sin_umu, &t, &ctxtReal39);
  if(realIsZero(&sin_umu)) {
    realCopy(const_0, sn);
    realCopy(const_1, cn);
    realCopy(const_1, dn);
    return;
  }

  realMultiply(mu(n), &t, c(n), &ctxtReal39);
  realCopy(const_1, d(n));

  while (n > 0) {
    n--;
    realMultiply(d(n+1), c(n+1), c(n), &ctxtReal39);
    realMultiply(c(n+1), c(n+1), &a, &ctxtReal39);
    realDivide(&a, mu(n+1), &r, &ctxtReal39);
    realAdd(&r, nu(n), &a, &ctxtReal39);
    realAdd(&r, mu(n), &b, &ctxtReal39);
    realDivide(&a, &b, d(n), &ctxtReal39);
  }
  complexMagnitude(const_1, c(0), &f, &ctxtReal39);
  if (realIsNegative(&e)) {
    realSubtract(const_1, m, &a, &ctxtReal39);
    realSquareRoot(&a, &g, &ctxtReal39);
    realDivide(&g, d(0), dn, &ctxtReal39);

    realDivide(dn, &f, cn, &ctxtReal39);
    if (realIsNegative(&cos_umu))
      realChangeSign(cn);

    realDivide(c(0), &g, &a, &ctxtReal39);
    realMultiply(cn, &a, sn, &ctxtReal39);
  } else {
    realCopy(d(0), dn);

    realDivide(const_1, &f, sn, &ctxtReal39);
    if (realIsNegative(&sin_umu))
      realChangeSign(sn);
    realMultiply(c(0), sn, cn, &ctxtReal39);
  }
#undef mu
#undef nu
#undef c
#undef d
}

static void calc_real_elliptic(real_t *sn, real_t *cn, real_t *dn, const real_t *u, const real_t *m) {
  real_t s_n, c_n, d_n;

  if (sn == NULL) sn = &s_n;
  if (cn == NULL) cn = &c_n;
  if (dn == NULL) dn = &d_n;

  if(realCompareLessThan(const_1, m)) {
    real_t k, uk, m_1;
    realSquareRoot(m, &k, &ctxtReal39);
    realMultiply(&k, u, &uk, &ctxtReal39);
    realDivide(const_1, m, &m_1, &ctxtReal39);
    _calc_real_elliptic(sn, dn, cn, &uk, &m_1);
    realDivide(sn, &k, sn, &ctxtReal39);
  }
  else if(realIsNegative(m)) {
    real_t mu, mu1, v, s, c, d;
    realSubtract(const_1, m, &mu1, &ctxtReal39);
    realDivide(const_1, &mu1, &mu1, &ctxtReal39);
    realMultiply(&mu1, m, &mu, &ctxtReal39);
    realChangeSign(&mu);
    realSquareRoot(&mu1, &mu1, &ctxtReal39);
    realDivide(u, &mu1, &v, &ctxtReal39);
    _calc_real_elliptic(&s, &c, &d, &v, &mu);
    realDivide(&s, &d, sn, &ctxtReal39);
    realDivide(&c, &d, cn, &ctxtReal39);
    realDivide(const_1, &d, dn, &ctxtReal39);
    realMultiply(sn, &mu1, sn, &ctxtReal39);
  }
  else {
    _calc_real_elliptic(sn, cn, dn, u, m);
  }
}

static void elliptic_setup_cpx_real(real_t *r,
                                    real_t *snuk, real_t *cnuk, real_t *dnuk,
                                    real_t *snvki, real_t *cnvki, real_t *dnvki,
                                    const real_t *u, const real_t *v,
                                    const real_t *m) {
  real_t m_1;
  realSubtract(const_1, m, &m_1, &ctxtReal39);

  calc_real_elliptic(snuk, cnuk, dnuk, u, m);
  calc_real_elliptic(snvki, cnvki, dnvki, v, &m_1);

  realMultiply(m, snuk, r, &ctxtReal39);
  realMultiply(r, snuk, r, &ctxtReal39);
  realMultiply(r, snvki, r, &ctxtReal39);
  realMultiply(r, snvki, r, &ctxtReal39);
  realFMA(cnvki, cnvki, r, r, &ctxtReal39);
}

static int jacobi_check_inputs(real_t *m, real_t *uReal, real_t *uImag, bool_t *realInput) {
  *realInput = true;

  switch(getRegisterDataType(REGISTER_X)) {
    case dtLongInteger: convertLongIntegerRegisterToReal(REGISTER_X, uReal, &ctxtReal39);
                        realZero(uImag);
                        break;

    case dtReal34:      real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), uReal);
                        realZero(uImag);
                        break;

    case dtComplex34:   real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), uReal);
                        real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), uImag);
                        *realInput = false;
                        break;

    default:            displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
                        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                          sprintf(errorMessage, "cannot calculate Jacobi elliptic function with %s in X", getRegisterDataTypeName(REGISTER_X, true, false));
                          moreInfoOnError("In function fnAgm:", errorMessage, NULL, NULL);
                        #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
                        return 0;
  }

  switch(getRegisterDataType(REGISTER_Y)) {
    case dtLongInteger: convertLongIntegerRegisterToReal(REGISTER_Y, m, &ctxtReal39);
                        break;

    case dtReal34:      real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), m);
                        break;

    default:            displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_Y);
                        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                          sprintf(errorMessage, "cannot calculate Jacobi elliptic function with %s in Y", getRegisterDataTypeName(REGISTER_Y, true, false));
                          moreInfoOnError("In function fnAgm:", errorMessage, NULL, NULL);
                        #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
                        return 0;
  }
  return 1;
}

static void jacobi_sn_am(bool_t calculateAmplitude) {
  bool_t realInput;
  real_t m, uReal, uImag;
  real_t a, b, denom, snuk, cnuk, dnuk, snvki, cnvki, dnvki;
  real_t rReal, rImag;

  if (!jacobi_check_inputs(&m, &uReal, &uImag, &realInput))
    return;

  if(!saveLastX()) return;

  if (realInput) {
    calc_real_elliptic(&rReal, NULL, NULL, &uReal, &m);
    if(calculateAmplitude) {
      WP34S_Asin(&rReal, &rReal, &ctxtReal39);
      convertAngleFromTo(&rReal, amRadian, currentAngularMode, &ctxtReal39);
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, currentAngularMode);
    }
    else {
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    }
    realToReal34(&rReal, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    // SN(u + i v, m) = sn(u, m) . dn(v, 1-m) / denom
    //                  + i . cn(u, m) . dn(u, m) . sn(v, 1-m) . cn(v, 1-m) / denom
    // where
    //    denom = cn(v, 1-m)^2 + m * dn(u, m)^2 * sn(v, 1-m)^2

    elliptic_setup_cpx_real(&denom, &snuk, &cnuk, &dnuk,
                            &snvki, &cnvki, &dnvki,
                            &uReal, &uImag, &m);

    realMultiply(&snuk, &dnvki, &a, &ctxtReal39);
    realDivide(&a, &denom, &rReal, &ctxtReal39);

    realMultiply(&cnuk, &dnuk, &a, &ctxtReal39);
    realMultiply(&a, &snvki, &b, &ctxtReal39);
    realMultiply(&b, &cnvki, &a, &ctxtReal39);
    realDivide(&a, &denom, &rImag, &ctxtReal39);

    if(calculateAmplitude) ArcsinComplex(&rReal, &rImag, &rReal, &rImag, &ctxtReal39);

    reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
    realToReal34(&rReal, REGISTER_REAL34_DATA(REGISTER_X));
    realToReal34(&rImag, REGISTER_IMAG34_DATA(REGISTER_X));
  }

  adjustResult(REGISTER_X, true, true, REGISTER_X, -1, -1);
}

void fnJacobiSn(uint16_t unusedButMandatoryParameter) {
  jacobi_sn_am(false);
}

void fnJacobiCn(uint16_t unusedButMandatoryParameter) {
  bool_t realInput;
  real_t m, uReal, uImag;
  real_t a, b, denom, snuk, cnuk, dnuk, snvki, cnvki, dnvki;
  real_t rReal, rImag;

  if (!jacobi_check_inputs(&m, &uReal, &uImag, &realInput))
    return;

  if(!saveLastX()) return;

  if (realInput) {
    calc_real_elliptic(NULL, &rReal, NULL, &uReal, &m);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    realToReal34(&rReal, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    // CN(u + i v, m) = cn(u, m) . cn(v, 1-m) / denom
    //                  - i . sn(u, m) . dn(u, m) . sn(v, 1-m) . dn(v, 1-m) / denom
    // where
    //    denom = cn(v, 1-m)^2 + m * dn(u, m)^2 * sn(v, 1-m)^2

    elliptic_setup_cpx_real(&denom, &snuk, &cnuk, &dnuk,
                            &snvki, &cnvki, &dnvki,
                            &uReal, &uImag, &m);

    realMultiply(&cnuk, &cnvki, &a, &ctxtReal39);
    realDivide(&a, &denom, &rReal, &ctxtReal39);

    realMultiply(&snuk, &dnuk, &a, &ctxtReal39);
    realMultiply(&a, &snvki, &b, &ctxtReal39);
    realMultiply(&b, &dnvki, &a, &ctxtReal39);
    realDivide(&a, &denom, &b, &ctxtReal39);
    realMinus(&b, &rImag, &ctxtReal39);

    reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
    realToReal34(&rReal, REGISTER_REAL34_DATA(REGISTER_X));
    realToReal34(&rImag, REGISTER_IMAG34_DATA(REGISTER_X));
  }

  adjustResult(REGISTER_X, true, true, REGISTER_X, -1, -1);
}

void fnJacobiDn(uint16_t unusedButMandatoryParameter) {
  bool_t realInput;
  real_t m, uReal, uImag;
  real_t a, b, denom, snuk, cnuk, dnuk, snvki, cnvki, dnvki;
  real_t rReal, rImag;

  if (!jacobi_check_inputs(&m, &uReal, &uImag, &realInput))
    return;

  if(!saveLastX()) return;

  if (realInput) {
    calc_real_elliptic(NULL, NULL, &rReal, &uReal, &m);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    realToReal34(&rReal, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    // DN(u + i v, m) = dn(u, m) . cn(v, 1-m) . dn(v, 1-m) / denom
    //                  - i . m . sn(u, m) . cn(u, m) . sn(v, 1-m) / denom
    // where
    //    denom = cn(v, 1-m)^2 + m * dn(u, m)^2 * sn(v, 1-m)^2

    elliptic_setup_cpx_real(&denom, &snuk, &cnuk, &dnuk,
                            &snvki, &cnvki, &dnvki,
                            &uReal, &uImag, &m);

    realMultiply(&dnuk, &cnvki, &a, &ctxtReal39);
    realMultiply(&a, &dnvki, &b, &ctxtReal39);
    realDivide(&b, &denom, &rReal, &ctxtReal39);

    realMinus(&m, &b, &ctxtReal39);
    realMultiply(&b, &snuk, &a, &ctxtReal39);
    realMultiply(&a, &cnuk, &b, &ctxtReal39);
    realMultiply(&b, &snvki, &a, &ctxtReal39);
    realDivide(&a, &denom, &rImag, &ctxtReal39);

    reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
    realToReal34(&rReal, REGISTER_REAL34_DATA(REGISTER_X));
    realToReal34(&rImag, REGISTER_IMAG34_DATA(REGISTER_X));
  }

  adjustResult(REGISTER_X, true, true, REGISTER_X, -1, -1);
}

void fnJacobiAmplitude(uint16_t unusedButMandatoryParameter) {
  jacobi_sn_am(true);
}

void fnEllipticK(uint16_t unusedButMandatoryParameter) {
  real_t m, a, b;

  if(!saveLastX()) return;

  switch(getRegisterDataType(REGISTER_X)) {
    case dtLongInteger: convertLongIntegerRegisterToReal(REGISTER_X, &m, &ctxtReal39);
                        break;

    case dtReal34:      real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &m);
                        break;

    //case dtComplex34:   // intentionally left unimplemented

    default:            displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
                        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                          sprintf(errorMessage, "cannot calculate elliptic integral K with %s in X", getRegisterDataTypeName(REGISTER_X, true, false));
                          moreInfoOnError("In function fnEllipticK:", errorMessage, NULL, NULL);
                        #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
                        return;
  }

  if(realCompareLessEqual(&m, const_1)) {
    if(realCompareEqual(&m, const_1)) {
      realCopy(const_plusInfinity, &b);
    }
    else if(realCompareEqual(&m, const_0)) {
      realCopy(const_piOn2, &b);
    }
    else {
      realSubtract(const_1, &m, &b, &ctxtReal39);
      realSquareRoot(&b, &b, &ctxtReal39);
      realAgm(const_1, &b, &b, &ctxtReal39);
      realDivide(const_piOn2, &b, &b, &ctxtReal39);
    }

    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    realToReal34(&b, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else if(getFlag(FLAG_CPXRES)) {
    realSubtract(&m, const_1, &b, &ctxtReal39);
    realSquareRoot(&b, &b, &ctxtReal39);
    complexAgm(const_1, const_0, const_0, &b, &a, &b, &ctxtReal39);
    divRealComplex(const_piOn2, &a, &b, &a, &b, &ctxtReal39);

    reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
    realToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
    realToReal34(&b, REGISTER_IMAG34_DATA(REGISTER_X));
  }
  else {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "Cannot calculate K(m) for m > 1 if CPXRES is not set");
      moreInfoOnError("In function fnEllipticK:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }

  adjustResult(REGISTER_X, true, true, REGISTER_X, -1, -1);
}
