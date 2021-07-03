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
#include "mathematics/arctan.h"
#include "mathematics/comparisonReals.h"
#include "mathematics/division.h"
#include "mathematics/magnitude.h"
#include "mathematics/multiplication.h"
#include "mathematics/sin.h"
#include "mathematics/tan.h"
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
                          sprintf(errorMessage, "cannot calculate elliptic integral or Jacobi elliptic function with %s in X", getRegisterDataTypeName(REGISTER_X, true, false));
                          moreInfoOnError("In function jacobi_check_inputs:", errorMessage, NULL, NULL);
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
                          sprintf(errorMessage, "cannot calculate elliptic integral or Jacobi elliptic function with %s in Y", getRegisterDataTypeName(REGISTER_Y, true, false));
                          moreInfoOnError("In function jacobi_check_inputs:", errorMessage, NULL, NULL);
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

static void _ellipticF(const real_t *phi, const real_t *m, real_t *res, realContext_t *realContext) {
  // assumes 0 ≤ phi ≤ pi/2 and 0 ≤ m ≤ 1
  real_t v, k1, agm;
  int n;

  /* The formula used here is found at:
   * http://hp41programs.yolasite.com/ellipticf.php
   */
  realCopy(phi, &v);
  realSubtract(const_1, m, &k1, realContext);
  realSquareRoot(&k1, &k1, realContext);
  n = realAgmForF(const_1, &k1, &v, &agm, realContext);
  realDivide(&v, &agm, res, realContext);
  while(n-- > 0) {
    realMultiply(res, const_1on2, res, realContext);
  }
}
static void _ellipticF_1(const real_t *phi, const real_t *m, real_t *res, realContext_t *realContext) {
  if(realCompareLessEqual(phi, const_piOn4)) {
    _ellipticF(phi, m, res, realContext);
  }
  else {
    // Abramowitz & Stegun §17.4.13
    real_t psi, m1, k1, tanPhi, k;

    ellipticKE(m, &k, NULL, NULL, NULL, realContext);
    WP34S_Cvt2RadSinCosTan(phi, amRadian, &m1, &k1, &tanPhi, realContext);
    realSubtract(const_1, m, &m1, realContext);
    realSquareRoot(&m1, &k1, realContext);
    realDivide(const_1, &k1, &psi, realContext);
    realMultiply(&k1, &tanPhi, &psi, realContext);
    WP34S_Atan2(const_1, &psi, &psi, realContext);
    _ellipticF(&psi, &m1, res, realContext);
    realSubtract(&k, res, res, realContext);
  }
}
static void _ellipticF_2(const real_t *phi, const real_t *m, real_t *res, realContext_t *realContext) {
  // assumes phi is real and 0 ≤ m ≤ 1
  // Abramowitz & Stegun §17.4.1, §17.4.3
  real_t phi1, phiQuotient, phiRemainder;
  bool_t remainderNegative = false;

  realCopyAbs(phi, &phi1);
  realDivide(&phi1, const_pi, &phiQuotient, realContext);
  realToIntegralValue(&phiQuotient, &phiQuotient, DEC_ROUND_DOWN, realContext);
  realDivideRemainder(&phi1, const_pi, &phiRemainder, realContext);
  if(realCompareGreaterThan(&phiRemainder, const_piOn2)) {
    realAdd(&phiQuotient, const_1, &phiQuotient, realContext);
    realSubtract(const_pi, &phiRemainder, &phiRemainder, realContext);
    realSetPositiveSign(&phiRemainder);
    remainderNegative = true;
  }

  if(realIsZero(&phiQuotient)) {
    _ellipticF_1(&phiRemainder, m, res, realContext);
  }
  else {
    realAdd(&phiQuotient, &phiQuotient, &phiQuotient, realContext);
    ellipticKE(m, &phi1, NULL, NULL, NULL, realContext);
    _ellipticF_1(&phiRemainder, m, res, realContext);
    if(remainderNegative) realChangeSign(res);
    realFMA(&phiQuotient, &phi1, res, res, realContext);
  }

  if(realIsNegative(phi)) realChangeSign(res);
}
static void _ellipticF_3(const real_t *phi, const real_t *psi, const real_t *m, real_t *res, real_t *resi, realContext_t *realContext) {
  if(realIsZero(psi)) {
    _ellipticF_2(phi, m, res, realContext);
    if(resi) realZero(resi);
  }
  else if(realIsZero(phi)) {
    // Abramowitz & Stegun §17.4.8
    real_t m1, theta;

    realSubtract(const_1, m, &m1, realContext);
    WP34S_SinhCosh(psi, &theta, NULL, realContext);
    WP34S_Atan(&theta, &theta, realContext);
    _ellipticF_2(&theta, &m1, resi, realContext);
    if(resi) realZero(res);
  }
  else {
    // Abramowitz & Stegun §17.4.11
    real_t m1, lambda, mu, b, c, csc2Phi, tan2Phi, cot2Phi, sinh2Psi, cot2Lambda;

    realSubtract(const_1, m, &m1, realContext);

    WP34S_Cvt2RadSinCosTan(phi, amRadian, &csc2Phi, &cot2Phi, &tan2Phi, realContext);
    realDivide(const_1, &csc2Phi, &csc2Phi, realContext);
    realMultiply(&csc2Phi, &csc2Phi, &csc2Phi, realContext);
    realDivide(const_1, &tan2Phi, &cot2Phi, realContext);
    realMultiply(&tan2Phi, &tan2Phi, &tan2Phi, realContext);
    realMultiply(&cot2Phi, &cot2Phi, &cot2Phi, realContext);
    WP34S_SinhCosh(psi, &sinh2Psi, NULL, realContext);
    realMultiply(&sinh2Psi, &sinh2Psi, &sinh2Psi, realContext);

    realMultiply(m, &sinh2Psi, &b, realContext);
    realFMA(&b, &csc2Phi, &cot2Phi, &b, realContext);
    realSubtract(&b, &m1, &b, realContext);
    realChangeSign(&b);
    realMultiply(&m1, &cot2Phi, &c, realContext);
    realChangeSign(&c);

    realMultiply(const__4, &c, &cot2Lambda, realContext);
    realFMA(&b, &b, &cot2Lambda, &cot2Lambda, realContext);
    realSquareRoot(&cot2Lambda, &cot2Lambda, realContext);
    realSubtract(&cot2Lambda, &b, &cot2Lambda, realContext);
    realMultiply(&cot2Lambda, const_1on2, &cot2Lambda, realContext);

    realDivide(const_1, &cot2Lambda, &lambda, realContext);
    realSquareRoot(&lambda, &lambda, realContext);
    WP34S_Atan(&lambda, &lambda, realContext);

    realFMA(&tan2Phi, &cot2Lambda, const__1, &mu, realContext);
    realDivide(&mu, m, &mu, realContext);
    realSquareRoot(&mu, &mu, realContext);
    WP34S_Atan(&mu, &mu, realContext);

    _ellipticF_2(&lambda, m, res, realContext);
    _ellipticF_2(&mu, &m1, resi, realContext);
  }
}
static void _ellipticF_4(const real_t *phi, const real_t *psi, const real_t *m, real_t *res, real_t *resi, realContext_t *realContext) {
  if(realIsZero(m)) {
    // Abramowitz & Stegun §17.4.19-20
    realCopy(phi, res);
    realCopy(psi, resi);
  }
  /*else if(realCompareEqual(m, const_1)) {
    // Abramowitz & Stegun §17.4.21
    real_t s, c, t;
    realFMA(phi, const_1on2, const_piOn4, &t, realContext);
    WP34S_Cvt2RadSinCosTan(&t, amRadian, &s, &c, &t, realContext);
    WP34S_Ln(&t, res, realContext);
  }*/
  else if(realCompareGreaterThan(m, const_1)) {
    // Abramowitz & Stegun §17.4.15
    real_t k, m_1, theta, thetai, a, b;

    WP34S_Mod(phi, const1071_2pi, &theta, realContext);
    sinComplex(&theta, psi, &a, &b, realContext);
    realSquareRoot(m, &k, realContext);
    realDivide(const_1, m, &m_1, realContext);
    realMultiply(&k, &a, &a, realContext); realMultiply(&k, &b, &b, realContext);
    ArcsinComplex(&a, &b, &theta, &thetai, realContext);
              fflush(stdout);
    _ellipticF_3(&theta, &thetai, &m_1, res, resi, realContext);
    divComplexComplex(res, resi, &k, const_0, res, resi, realContext);
  }
  else if(realIsPositive(m)) {
    _ellipticF_3(phi, psi, m, res, resi, realContext);
  }
  else {
    // Abramowitz & Stegun §17.4.17
    real_t absm, mp1, rtmp1, k, ki, m2, phi1, phi1i, f, fi;

    realCopyAbs(m, &absm);
    realAdd(&absm, const_1, &mp1, realContext);
    realSquareRoot(&mp1, &rtmp1, realContext);

    realDivide(&absm, &mp1, &m2, realContext);
    ellipticKE(&m2, &k, &ki, NULL, NULL, realContext);

    realSubtract(const_piOn2, phi, &phi1, realContext); realMinus(psi, &phi1i, realContext);
    ellipticF(&phi1, &phi1i, &m2, &f, &fi, realContext); // recurses here
    
    realSubtract(&k, &f, &k, realContext); realSubtract(&ki, &fi, &ki, realContext);
    divComplexComplex(&k, &ki, &rtmp1, const_0, res, resi, realContext);
  }
}
static void _ellipticF_5(const real_t *phi, const real_t *psi, const real_t *m, real_t *res, real_t *resi, realContext_t *realContext) {
  // Abramowitz & Stegun §17.4.15
  if(realCompareLessEqual(phi, const_piOn4)) {
    _ellipticF_4(phi, psi, m, res, resi, realContext);
  }
  else {
    real_t psir, psii, m1, k1, k1i, tanPhi, tanPhiI, k, ki;

    ellipticKE(m, &k, &ki, NULL, NULL, realContext);
    TanComplex(phi, psi, &tanPhi, &tanPhiI, realContext);
    realSubtract(const_1, m, &m1, realContext);
    if(realIsNegative(&m1)) {
      realSetPositiveSign(&m1);
      realZero(&k1); realSquareRoot(&m1, &k1i, realContext);
      realSetNegativeSign(&m1);
    }
    else {
      realSquareRoot(&m1, &k1, realContext); realZero(&k1i);
    }
    mulComplexComplex(&k1, &k1i, &tanPhi, &tanPhiI, &psir, &psii, realContext);
    divRealComplex(const_1, &psir, &psii, &psir, &psii, realContext);
    ArctanComplex(&psir, &psii, &psir, &psii, realContext);
    _ellipticF_4(&psir, &psii, m, res, resi, realContext);
    realSubtract(&k, res, res, realContext); realSubtract(&ki, resi, resi, realContext);
  }
}

void ellipticF(const real_t *phi, const real_t *psi, const real_t *m, real_t *res, real_t *resi, realContext_t *realContext) {
  // Abramowitz & Stegun §17.4.1, §17.4.3
  real_t phi1, phiQuotient, phiRemainder, psi1;
  bool_t remainderNegative = false;

  realCopyAbs(phi, &phi1); realCopy(psi, &psi1);
  realDivide(&phi1, const_pi, &phiQuotient, realContext);
  realToIntegralValue(&phiQuotient, &phiQuotient, DEC_ROUND_DOWN, realContext);
  realDivideRemainder(&phi1, const_pi, &phiRemainder, realContext);
  if(realCompareGreaterThan(&phiRemainder, const_piOn2)) {
    realAdd(&phiQuotient, const_1, &phiQuotient, realContext);
    realSubtract(const_pi, &phiRemainder, &phiRemainder, realContext);
    realSetPositiveSign(&phiRemainder);
    realChangeSign(&psi1);
    remainderNegative = true;
  }

  if(realIsZero(&phiQuotient)) {
    _ellipticF_5(&phiRemainder, &psi1, m, res, resi, realContext);
  }
  else {
    realAdd(&phiQuotient, &phiQuotient, &phiQuotient, realContext);
    _ellipticF_5(&phiRemainder, &psi1, m, res, resi, realContext);
    ellipticKE(m, &phi1, &psi1, NULL, NULL, realContext);
    if(remainderNegative) {
      realChangeSign(res);
      realChangeSign(resi);
    }
    realFMA(&phiQuotient, &phi1, res, res, realContext);
    realFMA(&phiQuotient, &psi1, resi, resi, realContext);
  }

  if(realIsNegative(phi)) realChangeSign(res);
}

void ellipticE(const real_t *phi, const real_t *psi, const real_t *m, real_t *res, real_t *resi, realContext_t *realContext) {
  real_t k, ki, e, ei, f, fi, z, zi;

  ellipticKE(m, &k, &ki, &e, &ei, realContext);
  ellipticF(phi, psi, m, &f, &fi, realContext);
  jacobiZeta(phi, psi, m, &z, &zi, realContext);
  divComplexComplex(&e, &ei, &k, &ki, res, resi, realContext);
  mulComplexComplex(res, resi, &f, &fi, res, resi, realContext);
  realAdd(res, &z, res, realContext); realAdd(resi, &zi, resi, realContext);
}

void jacobiZeta(const real_t *phi, const real_t *psi, const real_t *m, real_t *res, real_t *resi, realContext_t *realContext) {
  real_t k, v, vi, agm, agmi;

  realCopy(phi, &v); realCopy(psi, &vi);
  realSubtract(const_1, m, &k, realContext);
  if(realIsNegative(&k)) {
    realSetPositiveSign(&k);
    realSquareRoot(&k, &k, realContext);
    complexAgmForZ(const_1, const_0, const_0, &k, &v, &vi, &agm, &agmi, realContext);
    realCopy(&v, res); realCopy(&vi, resi);
  }
  else if(realIsZero(&vi)) {
    realSquareRoot(&k, &k, realContext);
    realAgmForZ(const_1, &k, &v, &agm, realContext);
    realCopy(&v, res); realZero(resi);
  }
  else {
    realSquareRoot(&k, &k, realContext);
    complexAgmForZ(const_1, const_0, &k, const_0, &v, &vi, &agm, &agmi, realContext);
    realCopy(&v, res); realCopy(&vi, resi);
  }
}

static void heumanLambda(const real_t *phi, const real_t *psi, const real_t *m, real_t *res, real_t *resi, realContext_t *realContext) {
  // Abramowitz & Stegun §17.4.39-40
  real_t k, ki, k1, k1i, z, zi, f, fi, m1;

  realSubtract(const_1, m, &m1, realContext);
  ellipticKE(m, &k, &ki, NULL, NULL, realContext);
  ellipticKE(&m1, &k1, &k1i, NULL, NULL, realContext);
  ellipticF(phi, psi, &m1, &f, &fi, realContext);
  jacobiZeta(phi, psi, &m1, &z, &zi, realContext);

  divComplexComplex(&f, &fi, &k1, &k1i, res, resi, realContext);
  mulComplexComplex(&k, &ki, &z, &zi, &z, &zi, realContext);
  divComplexComplex(&z, &zi, const_piOn2, const_0, &z, &zi, realContext);
  realAdd(res, &z, res, realContext); realAdd(resi, &zi, resi, realContext);
}

static void rcSqrt(const real_t *x, real_t *r, real_t *i, realContext_t *realContext) {
  real_t xx;

  realCopy(x, &xx);
  if(realIsNegative(&xx)) {
    realSetPositiveSign(&xx);
    realSquareRoot(&xx, i, realContext);
    realZero(r);
  }
  else {
    realSquareRoot(&xx, r, realContext);
    realZero(i);
  }
}

static void _ellipticPi_1(const real_t *n, const real_t *m, real_t *res, real_t *resi, realContext_t *realContext) {
  // Abramowitz & Stegun §17.7.6
  real_t epsilon, epsiloni, delta1, delta1i, k, ki, z, zi, p;

  realDivide(n, m, &epsilon, realContext);
  rcSqrt(&epsilon, &epsilon, &epsiloni, realContext);
  ArcsinComplex(&epsilon, &epsiloni, &epsilon, &epsiloni, realContext);

  realSubtract(const_1, n, &delta1, realContext);
  realDivide(n, &delta1, &delta1, realContext);
  realSubtract(m, n, &p, realContext);
  realDivide(&delta1, &p, &delta1, realContext);
  rcSqrt(&delta1, &delta1, &delta1i, realContext);

  ellipticKE(m, &k, &ki, NULL, NULL, realContext);
  jacobiZeta(&epsilon, &epsiloni, m, &z, &zi, realContext);
  mulComplexComplex(&k, &ki, &z, &zi, res, resi, realContext);
  mulComplexComplex(res, resi, &delta1, &delta1i, res, resi, realContext);
  realAdd(res, &k, res, realContext); realAdd(resi, &ki, resi, realContext);
}
static void _ellipticPi_2(const real_t *n, const real_t *m, real_t *res, real_t *resi, realContext_t *realContext) {
  // Abramowitz & Stegun §17.7.7, §17.7.9
  real_t nn, k, ki;

  realDivide(m, n, &nn, realContext);
  ellipticKE(m, &k, &ki, NULL, NULL, realContext);
  _ellipticPi_1(&nn, m, res, resi, realContext);
  realSubtract(&k, res, res, realContext); realSubtract(&ki, resi, resi, realContext);
}
static void _ellipticPi_3(const real_t *n, const real_t *m, real_t *res, real_t *resi, realContext_t *realContext) {
  // Abramowitz & Stegun §17.7.14
  real_t epsilon, epsiloni, delta2, delta2i, k, ki, n1, m1, p;

  realSubtract(const_1, n, &n1, realContext);
  realSubtract(const_1, m, &m1, realContext);

  realDivide(&n1, &m1, &epsilon, realContext);
  rcSqrt(&epsilon, &epsilon, &epsiloni, realContext);
  ArcsinComplex(&epsilon, &epsiloni, &epsilon, &epsiloni, realContext);

  realDivide(n, &n1, &delta2, realContext);
  realSubtract(n, m, &p, realContext);
  realDivide(&delta2, &p, &delta2, realContext);
  rcSqrt(&delta2, &delta2, &delta2i, realContext);

  ellipticKE(m, &k, &ki, NULL, NULL, realContext);

  heumanLambda(&epsilon, &epsiloni, m, res, resi, realContext);
  realSubtract(const_1, res, res, realContext); realChangeSign(resi);
  mulComplexComplex(&delta2, &delta2i, res, resi, res, resi, realContext);
  realFMA(const_piOn2, res, &k, res, realContext); realFMA(const_piOn2, resi, &ki, resi, realContext);
}
static void _ellipticPi_4(const real_t *n, const real_t *m, real_t *res, real_t *resi, realContext_t *realContext) {
  // Abramowitz & Stegun §17.7.15, §17.7.17
  real_t nn, k, ki, mmn, n1;

  realSubtract(m, n, &mmn, realContext);
  realSubtract(const_1, n, &n1, realContext);
  realDivide(&mmn, &n1, &nn, realContext);
  ellipticKE(m, &k, &ki, NULL, NULL, realContext);

  _ellipticPi_3(&nn, m, res, resi, realContext);
  divComplexComplex(res, resi, &mmn, const_0, res, resi, realContext);
  divComplexComplex(res, resi, &n1, const_0, res, resi, realContext);
  realMultiply(m, res, res, realContext); realMultiply(m, resi, resi, realContext);
  realMultiply(n, res, res, realContext); realMultiply(n, resi, resi, realContext);

  divComplexComplex(&k, &ki, &mmn, const_0, &k, &ki, realContext);
  realMultiply(m, &k, &k, realContext); realMultiply(m, &ki, &ki, realContext);

  realSubtract(&k, res, res, realContext); realSubtract(&ki, resi, resi, realContext);
}

void ellipticPi(const real_t *n, const real_t *m, real_t *res, real_t *resi, realContext_t *realContext) {
  if(realIsZero(n)) {
    ellipticKE(m, res, resi, NULL, NULL, realContext);
  }
  else if(realIsZero(m)) {
    // Abramowitz & Stegun §17.7.20
    if(realCompareLessThan(n, const_1)) {
      //      lim       (Arctan(sqrt(1-n) tan(phi)) / sqrt(1-n))
      //  phi->(pi/2)-0                                          = (pi/2) sqrt(1-n) / n
      realSubtract(const_1, n, res, realContext);
      rcSqrt(res, res, resi, realContext);
      divComplexComplex(res, resi, n, const_0, res, resi, realContext);
      realMultiply(const_piOn2, res, res, realContext); realMultiply(const_piOn2, resi, resi, realContext);
    }
    else if(realCompareGreaterThan(n, const_1)) {
      //      lim       (Arctanh(sqrt(n-1) tan(phi)) / sqrt(n-1))
      //  phi->(pi/2)-0                                          = -(pi/2) i / sqrt(n-1)
      realSubtract(n, const_1, res, realContext);
      rcSqrt(res, res, resi, realContext);
      divComplexComplex(const_0, const_1, res, resi, res, resi, realContext);
      realMultiply(const_piOn2, res, res, realContext); realMultiply(const_piOn2, resi, resi, realContext);
      realChangeSign(res); realChangeSign(resi);
    }
    else { // n = 1
      realCopy(const_NaN, res); realCopy(const_0, resi); // unsigned infinity
    }
  }
  else if(realCompareEqual(m, const_1)) {
    if(realCompareLessThan(n, const_1)) {
      realCopy(const_plusInfinity, res); realCopy(const_0, resi);
    }
    else if(realCompareGreaterThan(n, const_1)) {
      realCopy(const_minusInfinity, res); realCopy(const_0, resi);
    }
    else { // n = 1
      realCopy(const_NaN, res); realCopy(const_0, resi); // unsigned infinity
    }
  }
  else if(realCompareEqual(m, n)) {
    // Abramowitz & Stegun §17.7.24
    real_t cos2Alpha, e, ei;

    realSubtract(const_1, m, &cos2Alpha, realContext);
    ellipticKE(m, NULL, NULL, &e, &ei, realContext);
    divComplexComplex(&e, &ei, &cos2Alpha, const_0, res, resi, realContext);
  }
  else if(realCompareEqual(m, const_1)) {
    realCopy(const_NaN, res); realCopy(const_0, resi); // unsigned infinity
  }
  else if(realCompareGreaterThan(n, const_1)) {
    _ellipticPi_2(n, m, res, resi, realContext);
  }
  else if(realIsNegative(n)) {
    _ellipticPi_4(n, m, res, resi, realContext);
  }
  else if(realCompareLessThan(n, m)) {
    _ellipticPi_1(n, m, res, resi, realContext);
  }
  else {
    _ellipticPi_3(n, m, res, resi, realContext);
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

void fnEllipticPi(uint16_t unusedButMandatoryParameter) {
  real_t m, ur, ui, rr, ri;
  bool_t realInput;

  if(!jacobi_check_inputs(&m, &ur, &ui, &realInput)) return;

  if(!saveLastX()) return;

  if(realIsNegative(&m) || realCompareGreaterEqual(&m, const_1)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "m is out of range (must in 0 ≤ m < 1)");
      moreInfoOnError("In function fnEllipticPi:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
  else if(realInput) {
    ellipticPi(&ur, &m, &rr, &ri, &ctxtReal39);
    if(realIsZero(&ri)) {
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
      realToReal34(&rr, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else if(getFlag(FLAG_CPXRES)) {
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
      realToReal34(&rr, REGISTER_REAL34_DATA(REGISTER_X));
      realToReal34(&ri, REGISTER_IMAG34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "Π(n|m) cannot return complex result without CPXRES set");
        moreInfoOnError("In function fnEllipticPi:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot calculate elliptic integral with %s in X", getRegisterDataTypeName(REGISTER_X, true, false));
      moreInfoOnError("In function fnEllipticPi:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }

  adjustResult(REGISTER_X, true, true, REGISTER_X, -1, -1);
}
