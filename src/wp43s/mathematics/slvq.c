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
 * \file slvq.c
 ***********************************************/

#include "wp43s.h"


/********************************************//**
 * \brief (c, b, a) ==> (x1, x2, r) c ==> regL
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnSlvq(uint16_t unusedParamButMandatory) {
  bool_t result16=true, realCoefs=true, realRoots=true;
  complex75_t a, b, c, r, x1, x2;

  switch(getRegisterDataType(REGISTER_X)) {
    case dtLongInteger: convertLongIntegerRegisterToReal(REGISTER_X, (real_t *)&c.real, &ctxtReal75);
                        realZero(&c.imag);
                        break;

    case dtReal16:      real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &c.real);
                        realZero(&c.imag);
                        break;

    case dtComplex16:   real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &c.real);
                        real16ToReal(REGISTER_IMAG16_DATA(REGISTER_X), &c.imag);
                        realCoefs = false;
                        break;

    case dtReal34:      real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &c.real);
                        realZero(&c.imag);
                        result16 = false;
                        break;

    case dtComplex34:   real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &c.real);
                        real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &c.imag);
                        realCoefs = false;
                        result16 = false;
                        break;

    default:            displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
                        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                          sprintf(errorMessage, "cannot SLVQ with %s in X", getRegisterDataTypeName(REGISTER_X, true, false));
                          showInfoDialog("In function fnSlqv:", errorMessage, NULL, NULL);
                        #endif
                        return;
  }

  switch(getRegisterDataType(REGISTER_Y)) {
    case dtLongInteger: convertLongIntegerRegisterToReal(REGISTER_Y, (real_t *)&b.real, &ctxtReal75);
                        realZero(&b.imag);
                        break;

    case dtReal16:      real16ToReal(REGISTER_REAL16_DATA(REGISTER_Y), &b.real);
                        realZero(&b.imag);
                        break;

    case dtComplex16:   real16ToReal(REGISTER_REAL16_DATA(REGISTER_Y), &b.real);
                        real16ToReal(REGISTER_IMAG16_DATA(REGISTER_Y), &b.imag);
                        realCoefs = false;
                        break;

    case dtReal34:      real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &b.real);
                        realZero(&b.imag);
                        result16 = false;
                        break;

    case dtComplex34:   real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &b.real);
                        real34ToReal(REGISTER_IMAG34_DATA(REGISTER_Y), &b.imag);
                        realCoefs = false;
                        result16 = false;
                        break;

    default:            displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_Y);
                        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                          sprintf(errorMessage, "cannot SLVQ with %s in Y", getRegisterDataTypeName(REGISTER_Y, true, false));
                          showInfoDialog("In function fnSlqv:", errorMessage, NULL, NULL);
                        #endif
                        return;
  }

  switch(getRegisterDataType(REGISTER_Z)) {
    case dtLongInteger: convertLongIntegerRegisterToReal(REGISTER_Z, (real_t *)&a.real, &ctxtReal75);
                        realZero(&a.imag);
                        break;

    case dtReal16:      real16ToReal(REGISTER_REAL16_DATA(REGISTER_Z), &a.real);
                        realZero(&a.imag);
                        break;

    case dtComplex16:   real16ToReal(REGISTER_REAL16_DATA(REGISTER_Z), &a.real);
                        real16ToReal(REGISTER_IMAG16_DATA(REGISTER_Z), &a.imag);
                        realCoefs = false;
                        break;

    case dtReal34:      real34ToReal(REGISTER_REAL34_DATA(REGISTER_Z), &a.real);
                        realZero(&a.imag);
                        result16 = false;
                        break;

    case dtComplex34:   real34ToReal(REGISTER_REAL34_DATA(REGISTER_Z), &a.real);
                        real34ToReal(REGISTER_IMAG34_DATA(REGISTER_Z), &a.imag);
                        realCoefs = false;
                        result16 = false;
                        break;

    default:            displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_Z);
                        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                          sprintf(errorMessage, "cannot SLVQ with %s in Z", getRegisterDataTypeName(REGISTER_Z, true, false));
                          showInfoDialog("In function fnSlqv:", errorMessage, NULL, NULL);
                        #endif
                        return;
  }

  if(    realIsNaN(&a.real) || realIsNaN(&a.imag)
      || realIsNaN(&b.real) || realIsNaN(&b.imag)
      || realIsNaN(&c.real) || realIsNaN(&c.imag)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function fnSlqv:", "cannot use NaN as X, Y, or Z input of SLVQ", NULL, NULL);
    #endif
    return;
  }

  if(   realIsZero(&a.real) && realIsZero(&a.imag)
     && realIsZero(&b.real) && realIsZero(&b.imag)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function fnSlqv:", "cannot use 0 for Y and Z as input of SLVQ", NULL, NULL);
    #endif
    return;
  }

  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(realCoefs == false) {
    realRoots = false;
  }

  if(realCoefs) {
    if(realIsZero(&a.real)) {
      // bx + c = 0   (b is not 0 here)

      // r = b²
      realMultiply(&b.real, &b.real, &r.real, &ctxtReal75);
      realZero(&r.imag);

      // x1 = x2 = -c/b
      realDivide(&c.real, &b.real, &x1.real, &ctxtReal75);
      realChangeSign(&x1.real);
      realCopy(&x1.real, &x2.real);
    }
    else if(realIsZero(&c.real)) {
      // ax² + bx = x(ax + b) = 0   (a is not 0 here)

      // r = b²
      realMultiply(&b.real, &b.real, &r.real, &ctxtReal75);
      realZero(&r.imag);

      // x1 = 0
      realZero(&x1.real);

      // x2 = -b/a
      realDivide(&b.real, &a.real, &x2.real, &ctxtReal75);
      realChangeSign(&x2.real);
    }
    else {
      // ax² + bx + c = 0   (a and c are not 0 here)

      // r = b² - 4ac
      realMultiply(const__4, &a.real, &r.real, &ctxtReal75);
      realMultiply(&c.real, &r.real, &r.real, &ctxtReal75);
      realFMA(&b.real, &b.real, &r.real, &r.real, &ctxtReal75);
      realZero(&r.imag);

      if(realIsPositive(&r.real)) { // real roots
        // x1 = (-b - sign(b)*sqrt(r)) / 2a
        realSquareRoot(&r.real, &x1.real, &ctxtReal75);
        if(realIsPositive(&b.real)) {
          realChangeSign(&x1.real);
        }
        realSubtract(&x1.real, &b.real, &x1.real, &ctxtReal75);
        realMultiply(&x1.real, const_1on2, &x1.real, &ctxtReal75);
        realDivide(&x1.real, &a.real, &x1.real, &ctxtReal75);

        // x2 = c / ax1  (x1 connot be 0 here)
        realDivide(&c.real, &a.real, &x2.real, &ctxtReal75);
        realDivide(&x2.real, &x1.real, &x2.real, &ctxtReal75);
      }
      else { // complex roots
        realRoots = false;

        // x1 = (-b - sign(b)*sqrt(r)) / 2a
        realMinus(&r.real, &x1.real, &ctxtReal75);
        realSquareRoot(&x1.real, &x1.imag, &ctxtReal75);
        realZero(&x1.real);
        if(realIsPositive(&b.real)) {
          realChangeSign(&x1.imag);
        }
        realSubtract(&x1.real, &b.real, &x1.real, &ctxtReal75);
        realSubtract(&x1.imag, &b.imag, &x1.imag, &ctxtReal75);
        realMultiply(&x1.real, const_1on2, &x1.real, &ctxtReal75);
        realMultiply(&x1.imag, const_1on2, &x1.imag, &ctxtReal75);
        realDivide(&x1.real, &a.real, &x1.real, &ctxtReal75);
        realDivide(&x1.imag, &a.real, &x1.imag, &ctxtReal75);

        // x2 = conj(x1)
        complexCopy(&x1, &x2, COMPLEX75_SIZE);
        realChangeSign(&x2.imag);
      }
    }
  }
  else { // Complex coefficients
    if(realIsZero(&a.real) && realIsZero(&a.imag)) {
      // bx + c = 0   (b is not 0 here)

      // r = b²
      mulCo75Co75(&b, &b, &r);

      // x1 = x2 = -c/b
      divCo75Co75(&c, &b, &x1);
      realChangeSign(&x1.real);
      realChangeSign(&x1.imag);
      complexCopy(&x1, &x2, COMPLEX75_SIZE);
    }
    else if(realIsZero(&c.real) && realIsZero(&c.imag)) {
      // ax² + bx = x(ax + b) = 0   (a is not 0 here)

      // r = b²
      mulCo75Co75(&b, &b, &r);

      // x1 = 0
      realZero(&x1.real);
      realZero(&x1.imag);

      // x2 = -b/a
      divCo75Co75(&b, &a, &x2);
      realChangeSign(&x2.real);
      realChangeSign(&x2.imag);
    }
    else {
      // ax² + bx + c = 0   (a and c are not 0 here)

      // r = b² - 4ac
      realMultiply(const__4, &a.real, &r.real, &ctxtReal75);
      realMultiply(const__4, &a.imag, &r.imag, &ctxtReal75);
      mulCo75Co75(&c, &r, &r);
      mulCo75Co75(&b, &b, &x1);
      realAdd(&x1.real, &r.real, &r.real, &ctxtReal75);
      realAdd(&x1.imag, &r.imag, &r.imag, &ctxtReal75);

      // x1 = (-b + sqrt(r)) / 2a
      complexCopy(&r, &x1, COMPLEX75_SIZE);
      real75RectangularToPolar((real_t *)&x1.real, (real_t *)&x1.imag, (real_t *)&x1.real, (real_t *)&x1.imag);
      realSquareRoot(&x1.real, &x1.real, &ctxtReal75);
      realMultiply(&x1.imag, const_0_5, &x1.imag, &ctxtReal75);
      real75PolarToRectangular((real_t *)&x1.real, (real_t *)&x1.imag, (real_t *)&x1.real, (real_t *)&x1.imag);

      realSubtract(&x1.real, &b.real, &x1.real, &ctxtReal75);
      realSubtract(&x1.imag, &b.imag, &x1.imag, &ctxtReal75);
      realMultiply(&x1.real, const_1on2, &x1.real, &ctxtReal75);
      realMultiply(&x1.imag, const_1on2, &x1.imag, &ctxtReal75);
      divCo75Co75(&x1, &a, &x1);

      // x2 = c / ax1  (x1 connot be 0 here)
      divCo75Co75(&c, &a, &x2);
      divCo75Co75(&x2, &x1, &x2);
    }
  }

  if(realRoots) {
    if(result16) {
      reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);
      reallocateRegister(REGISTER_Y, dtReal16, REAL16_SIZE, AM_NONE);
      reallocateRegister(REGISTER_Z, dtReal16, REAL16_SIZE, AM_NONE);
      realToReal16(&x1.real, REGISTER_REAL16_DATA(REGISTER_X));
      realToReal16(&x2.real, REGISTER_REAL16_DATA(REGISTER_Y));
      realToReal16(&r.real,  REGISTER_REAL16_DATA(REGISTER_Z));
    }
    else { // realRoots && !result16
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
      reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, AM_NONE);
      reallocateRegister(REGISTER_Z, dtReal34, REAL34_SIZE, AM_NONE);
      realToReal34(&x1.real, REGISTER_REAL34_DATA(REGISTER_X));
      realToReal34(&x2.real, REGISTER_REAL34_DATA(REGISTER_Y));
      realToReal34(&r.real,  REGISTER_REAL34_DATA(REGISTER_Z));
    }
  }
  else { // !realRoots
    if(result16) {
      if(realIsZero(&x1.imag)) { // x1 is real
        reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);
        realToReal16(&x1.real, REGISTER_REAL16_DATA(REGISTER_X));
      }
      else {
        reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);
        realToReal16(&x1.real, REGISTER_REAL16_DATA(REGISTER_X));
        realToReal16(&x1.imag, REGISTER_IMAG16_DATA(REGISTER_X));
      }

      if(realIsZero(&x2.imag)) { // x2 is real
        reallocateRegister(REGISTER_Y, dtReal16, REAL16_SIZE, AM_NONE);
        realToReal16(&x2.real, REGISTER_REAL16_DATA(REGISTER_Y));
      }
      else {
        reallocateRegister(REGISTER_Y, dtComplex16, COMPLEX16_SIZE, AM_NONE);
        realToReal16(&x2.real, REGISTER_REAL16_DATA(REGISTER_Y));
        realToReal16(&x2.imag, REGISTER_IMAG16_DATA(REGISTER_Y));
      }

      if(realIsZero(&r.imag)) { // r is real
        reallocateRegister(REGISTER_Z, dtReal16, REAL16_SIZE, AM_NONE);
        realToReal16(&r.real, REGISTER_REAL16_DATA(REGISTER_Z));
      }
      else {
        reallocateRegister(REGISTER_Z, dtComplex16, COMPLEX16_SIZE, AM_NONE);
        realToReal16(&r.real, REGISTER_REAL16_DATA(REGISTER_Z));
        realToReal16(&r.imag, REGISTER_IMAG16_DATA(REGISTER_Z));
      }
    }
    else { // !realRoots && !result16
      if(realIsZero(&x1.imag)) { // x1 is real
        reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
        realToReal34(&x1.real, REGISTER_REAL34_DATA(REGISTER_X));
      }
      else {
        reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
        realToReal34(&x1.real, REGISTER_REAL34_DATA(REGISTER_X));
        realToReal34(&x1.imag, REGISTER_IMAG34_DATA(REGISTER_X));
      }

      if(realIsZero(&x2.imag)) { // x2 is real
        reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, AM_NONE);
        realToReal34(&x2.real, REGISTER_REAL34_DATA(REGISTER_Y));
      }
      else {
        reallocateRegister(REGISTER_Y, dtComplex34, COMPLEX34_SIZE, AM_NONE);
        realToReal34(&x2.real, REGISTER_REAL34_DATA(REGISTER_Y));
        realToReal34(&x2.imag, REGISTER_IMAG34_DATA(REGISTER_Y));
      }

      if(realIsZero(&r.imag)) { // r is real
        reallocateRegister(REGISTER_Z, dtReal34, REAL34_SIZE, AM_NONE);
        realToReal34(&r.real, REGISTER_REAL34_DATA(REGISTER_Z));
      }
      else {
        reallocateRegister(REGISTER_Z, dtComplex34, COMPLEX34_SIZE, AM_NONE);
        realToReal34(&r.real, REGISTER_REAL34_DATA(REGISTER_Z));
        realToReal34(&r.imag, REGISTER_IMAG34_DATA(REGISTER_Z));
      }
    }
  }

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
  adjustResult(REGISTER_Y, false, true, REGISTER_Y, -1, -1);
  adjustResult(REGISTER_Z, false, true, REGISTER_Z, -1, -1);
}
