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
#include "mathematics/multiplication.h"
#include "mathematics/wp34s.h"
#include "realType.h"
#include "registers.h"
#include "registerValueConversions.h"
#include "wp43s.h"

#define ELLIPTIC_N	16

static void _calc_real_elliptic(real_t *sn, real_t *cn, real_t *dn, const real_t *u, const real_t *m, realContext_t *realContext) {
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
    WP34S_Cvt2RadSinCosTan(u, amRadian, sn, cn, NULL, realContext);
    realCopy(const_1, dn);
    return;
  }
  realSubtract(m, const_1, &a, realContext);
  if (realCompareAbsLessThan(&a, const_1e_32)) {
    WP34S_SinhCosh(u, &a, &b, realContext);
    realDivide(const_1, &b, cn, realContext);
    realMultiply(&a, cn, sn, realContext);
    realCopy(cn, dn);
    return;
  }

  realSubtract(const_1, m, &a, realContext);
  realSquareRoot(&a, &a, realContext);
  n = realAgmStep(const_1, &a, &b, MU, NU, ELLIPTIC_N, realContext);
  realSubtract(mu(n-1), nu(n-1), &e, realContext);

  realMultiply(u, mu(n), &a, realContext);
  WP34S_Cvt2RadSinCosTan(&a, amRadian, &sin_umu, &cos_umu, NULL, realContext);
  realCopyAbs(&cos_umu, &b);
  //if (realCompareAbsLessThan(&sin_umu, &b))
  //  realDivide(&sin_umu, &cos_umu, &t, realContext);
  //else
    realDivide(&cos_umu, &sin_umu, &t, realContext);
  if(realIsZero(&sin_umu)) {
    realCopy(const_0, sn);
    realCopy(const_1, cn);
    realCopy(const_1, dn);
    return;
  }

  realMultiply(mu(n), &t, c(n), realContext);
  realCopy(const_1, d(n));

  while (n > 0) {
    n--;
    realMultiply(d(n+1), c(n+1), c(n), realContext);
    realMultiply(c(n+1), c(n+1), &a, realContext);
    realDivide(&a, mu(n+1), &r, realContext);
    realAdd(&r, nu(n), &a, realContext);
    realAdd(&r, mu(n), &b, realContext);
    realDivide(&a, &b, d(n), realContext);
  }
  complexMagnitude(const_1, c(0), &f, realContext);
  if (realIsNegative(&e)) {
    realSubtract(const_1, m, &a, realContext);
    realSquareRoot(&a, &g, realContext);
    realDivide(&g, d(0), dn, realContext);

    realDivide(dn, &f, cn, realContext);
    if (realIsNegative(&cos_umu))
      realChangeSign(cn);

    realDivide(c(0), &g, &a, realContext);
    realMultiply(cn, &a, sn, realContext);
  } else {
    realCopy(d(0), dn);

    realDivide(const_1, &f, sn, realContext);
    if (realIsNegative(&sin_umu))
      realChangeSign(sn);
    realMultiply(c(0), sn, cn, realContext);
  }
#undef mu
#undef nu
#undef c
#undef d
}

static void calc_real_elliptic(real_t *sn, real_t *cn, real_t *dn, const real_t *u, const real_t *m, realContext_t *realContext) {
  real_t s_n, c_n, d_n;

  if (sn == NULL) sn = &s_n;
  if (cn == NULL) cn = &c_n;
  if (dn == NULL) dn = &d_n;

  if(realCompareLessThan(const_1, m)) {
    real_t k, uk, m_1;
    realSquareRoot(m, &k, realContext);
    realMultiply(&k, u, &uk, realContext);
    realDivide(const_1, m, &m_1, realContext);
    _calc_real_elliptic(sn, dn, cn, &uk, &m_1, realContext);
    realDivide(sn, &k, sn, realContext);
  }
  else if(realIsNegative(m)) {
    real_t mu, mu1, v, s, c, d;
    realSubtract(const_1, m, &mu1, realContext);
    realDivide(const_1, &mu1, &mu1, realContext);
    realMultiply(&mu1, m, &mu, realContext);
    realChangeSign(&mu);
    realSquareRoot(&mu1, &mu1, realContext);
    realDivide(u, &mu1, &v, realContext);
    _calc_real_elliptic(&s, &c, &d, &v, &mu, realContext);
    realDivide(&s, &d, sn, realContext);
    realDivide(&c, &d, cn, realContext);
    realDivide(const_1, &d, dn, realContext);
    realMultiply(sn, &mu1, sn, realContext);
  }
  else {
    _calc_real_elliptic(sn, cn, dn, u, m, realContext);
  }
}

static void elliptic_setup_cpx_real(real_t *r,
                                    real_t *snuk, real_t *cnuk, real_t *dnuk,
                                    real_t *snvki, real_t *cnvki, real_t *dnvki,
                                    const real_t *u, const real_t *v,
                                    const real_t *m, realContext_t *realContext) {
  real_t m_1;
  realSubtract(const_1, m, &m_1, realContext);

  calc_real_elliptic(snuk, cnuk, dnuk, u, m, realContext);
  calc_real_elliptic(snvki, cnvki, dnvki, v, &m_1, realContext);

  realMultiply(m, snuk, r, realContext);
  realMultiply(r, snuk, r, realContext);
  realMultiply(r, snvki, r, realContext);
  realMultiply(r, snvki, r, realContext);
  realFMA(cnvki, cnvki, r, r, realContext);
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


void jacobiElliptic(const real_t *u, const real_t *m, real_t *am, real_t *sn, real_t *cn, real_t *dn, realContext_t *realContext) {
  real_t _sn;
  calc_real_elliptic(sn ? sn : &_sn, cn, dn, u, m, realContext);
  if(am) {
    WP34S_Asin(sn ? sn : &_sn, am, realContext);
  }
}

void jacobiComplexAm(const real_t *ur, const real_t *ui, const real_t *m, real_t *rr, real_t *ri, realContext_t *realContext) {
  jacobiComplexSn(ur, ui, m, rr, ri, realContext);
  ArcsinComplex(rr, ri, rr, ri, realContext);
}

void jacobiComplexSn(const real_t *ur, const real_t *ui, const real_t *m, real_t *rr, real_t *ri, realContext_t *realContext) {
  // SN(u + i v, m) = sn(u, m) . dn(v, 1-m) / denom
  //                  + i . cn(u, m) . dn(u, m) . sn(v, 1-m) . cn(v, 1-m) / denom
  // where
  //    denom = cn(v, 1-m)^2 + m * dn(u, m)^2 * sn(v, 1-m)^2

  real_t a, b, denom, snuk, cnuk, dnuk, snvki, cnvki, dnvki;
  elliptic_setup_cpx_real(&denom, &snuk, &cnuk, &dnuk, &snvki, &cnvki, &dnvki, ur, ui, m, realContext);

  realMultiply(&snuk, &dnvki, &a, realContext);
  realDivide(&a, &denom, rr, realContext);

  realMultiply(&cnuk, &dnuk, &a, realContext);
  realMultiply(&a, &snvki, &b, realContext);
  realMultiply(&b, &cnvki, &a, realContext);
  realDivide(&a, &denom, ri, realContext);
}

void jacobiComplexCn(const real_t *ur, const real_t *ui, const real_t *m, real_t *rr, real_t *ri, realContext_t *realContext) {
  // CN(u + i v, m) = cn(u, m) . cn(v, 1-m) / denom
  //                  - i . sn(u, m) . dn(u, m) . sn(v, 1-m) . dn(v, 1-m) / denom
  // where
  //    denom = cn(v, 1-m)^2 + m * dn(u, m)^2 * sn(v, 1-m)^2

  real_t a, b, denom, snuk, cnuk, dnuk, snvki, cnvki, dnvki;
  elliptic_setup_cpx_real(&denom, &snuk, &cnuk, &dnuk, &snvki, &cnvki, &dnvki, ur, ui, m, realContext);

  realMultiply(&cnuk, &cnvki, &a, realContext);
  realDivide(&a, &denom, rr, realContext);

  realMultiply(&snuk, &dnuk, &a, realContext);
  realMultiply(&a, &snvki, &b, realContext);
  realMultiply(&b, &dnvki, &a, realContext);
  realDivide(&a, &denom, &b, realContext);
  realMinus(&b, ri, realContext);
}

void jacobiComplexDn(const real_t *ur, const real_t *ui, const real_t *m, real_t *rr, real_t *ri, realContext_t *realContext) {
  // DN(u + i v, m) = dn(u, m) . cn(v, 1-m) . dn(v, 1-m) / denom
  //                  - i . m . sn(u, m) . cn(u, m) . sn(v, 1-m) / denom
  // where
  //    denom = cn(v, 1-m)^2 + m * dn(u, m)^2 * sn(v, 1-m)^2

  real_t a, b, denom, snuk, cnuk, dnuk, snvki, cnvki, dnvki;
  elliptic_setup_cpx_real(&denom, &snuk, &cnuk, &dnuk, &snvki, &cnvki, &dnvki, ur, ui, m, realContext);

  realMultiply(&dnuk, &cnvki, &a, realContext);
  realMultiply(&a, &dnvki, &b, realContext);
  realDivide(&b, &denom, rr, realContext);

  realMinus(m, &b, realContext);
  realMultiply(&b, &snuk, &a, realContext);
  realMultiply(&a, &cnuk, &b, realContext);
  realMultiply(&b, &snvki, &a, realContext);
  realDivide(&a, &denom, ri, realContext);
}

void ellipticKE(const real_t *m, real_t *k, real_t *ki, real_t *e, real_t *ei, realContext_t *realContext) {
  real_t a, b, cr, ci;

  if(k) realCopy(const_NaN, k);
  if(ki) realZero(ki);
  if(e) realCopy(const_NaN, e);
  if(ei) realZero(ei);

  if(realCompareLessEqual(m, const_1)) {
    if(realCompareEqual(m, const_1)) {
      if(k) realCopy(const_plusInfinity, k);
      if(e) realCopy(const_1, e);
    }
    else if(realCompareEqual(m, const_0)) {
      if(k) realCopy(const_piOn2, k);
      if(e) realCopy(const_piOn2, e);
    }
    else {
      realSubtract(const_1, m, &b, &ctxtReal39);
      realSquareRoot(&b, &b, &ctxtReal39);
      realCopy(m, &a);
      realAgmForE(const_1, &b, &a, &b, &ctxtReal39);
      realDivide(const_piOn2, &b, &b, &ctxtReal39);
      if(k) realCopy(&b, k);

      if(e) {
        realSubtract(&a, const_1, &a, &ctxtReal39);
        realChangeSign(&a);
        realMultiply(&a, &b, &b, &ctxtReal39);
        realCopy(&b, e);
      }
    }
  }
  else if(ki || ei) {
    if(k) realCopy(const_NaN, k);
    if(e) realCopy(const_NaN, e);

    realSubtract(m, const_1, &b, realContext);
    realSquareRoot(&b, &b, realContext);
    realCopy(m, &cr); realZero(&ci);
    complexAgmForE(const_1, const_0, const_0, &b, &cr, &ci, &a, &b, realContext);
    divRealComplex(const_piOn2, &a, &b, &a, &b, realContext);
    if(k && ki) {
      realCopy(&a, k); realCopy(&b, ki);
    }

    if(e && ei) {
      realSubtract(&cr, const_1, &cr, realContext);
      realChangeSign(&cr); realChangeSign(&ci);
      mulComplexComplex(&a, &b, &cr, &ci, &a, &b, realContext);
      realCopy(&a, e); realCopy(&b, ei);
    }
  }
}


void fnJacobiSn(uint16_t unusedButMandatoryParameter) {
  bool_t realInput;
  real_t m, uReal, uImag;
  real_t rReal, rImag;

  if (!jacobi_check_inputs(&m, &uReal, &uImag, &realInput))
    return;

  if(!saveLastX()) return;

  if (realInput) {
    jacobiElliptic(&uReal, &m, NULL, &rReal, NULL, NULL, &ctxtReal39);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    realToReal34(&rReal, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    jacobiComplexSn(&uReal, &uImag, &m, &rReal, &rImag, &ctxtReal39);
    reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
    realToReal34(&rReal, REGISTER_REAL34_DATA(REGISTER_X));
    realToReal34(&rImag, REGISTER_IMAG34_DATA(REGISTER_X));
  }

  adjustResult(REGISTER_X, true, true, REGISTER_X, -1, -1);
}

void fnJacobiCn(uint16_t unusedButMandatoryParameter) {
  bool_t realInput;
  real_t m, uReal, uImag;
  real_t rReal, rImag;

  if (!jacobi_check_inputs(&m, &uReal, &uImag, &realInput))
    return;

  if(!saveLastX()) return;

  if (realInput) {
    jacobiElliptic(&uReal, &m, NULL, NULL, &rReal, NULL, &ctxtReal39);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    realToReal34(&rReal, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    jacobiComplexCn(&uReal, &uImag, &m, &rReal, &rImag, &ctxtReal39);
    reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
    realToReal34(&rReal, REGISTER_REAL34_DATA(REGISTER_X));
    realToReal34(&rImag, REGISTER_IMAG34_DATA(REGISTER_X));
  }

  adjustResult(REGISTER_X, true, true, REGISTER_X, -1, -1);
}

void fnJacobiDn(uint16_t unusedButMandatoryParameter) {
  bool_t realInput;
  real_t m, uReal, uImag;
  real_t rReal, rImag;

  if (!jacobi_check_inputs(&m, &uReal, &uImag, &realInput))
    return;

  if(!saveLastX()) return;

  if (realInput) {
    jacobiElliptic(&uReal, &m, NULL, NULL, NULL, &rReal, &ctxtReal39);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    realToReal34(&rReal, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    jacobiComplexDn(&uReal, &uImag, &m, &rReal, &rImag, &ctxtReal39);
    reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
    realToReal34(&rReal, REGISTER_REAL34_DATA(REGISTER_X));
    realToReal34(&rImag, REGISTER_IMAG34_DATA(REGISTER_X));
  }

  adjustResult(REGISTER_X, true, true, REGISTER_X, -1, -1);
}

void fnJacobiAmplitude(uint16_t unusedButMandatoryParameter) {
  bool_t realInput;
  real_t m, uReal, uImag;
  real_t rReal, rImag;

  if (!jacobi_check_inputs(&m, &uReal, &uImag, &realInput))
    return;

  if(!saveLastX()) return;

  if (realInput) {
    jacobiElliptic(&uReal, &m, &rReal, NULL, NULL, NULL, &ctxtReal39);
    convertAngleFromTo(&rReal, amRadian, currentAngularMode, &ctxtReal39);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, currentAngularMode);
    realToReal34(&rReal, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    jacobiComplexAm(&uReal, &uImag, &m, &rReal, &rImag, &ctxtReal39);
    reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
    realToReal34(&rReal, REGISTER_REAL34_DATA(REGISTER_X));
    realToReal34(&rImag, REGISTER_IMAG34_DATA(REGISTER_X));
  }

  adjustResult(REGISTER_X, true, true, REGISTER_X, -1, -1);
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
    ellipticKE(&m, &b, NULL, NULL, NULL, &ctxtReal39);

    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    realToReal34(&b, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else if(getFlag(FLAG_CPXRES)) {
    ellipticKE(&m, &a, &b, NULL, NULL, &ctxtReal39);

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

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}

void fnEllipticE(uint16_t unusedButMandatoryParameter) {
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
                          moreInfoOnError("In function fnEllipticE:", errorMessage, NULL, NULL);
                        #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
                        return;
  }

  if(realCompareLessEqual(&m, const_1)) {
    ellipticKE(&m, NULL, NULL, &b, NULL, &ctxtReal39);

    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    realToReal34(&b, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else if(getFlag(FLAG_CPXRES)) {
    ellipticKE(&m, NULL, NULL, &a, &b, &ctxtReal39);

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

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}
