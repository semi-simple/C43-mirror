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
  complexIc_t a, b, c, r, x1, x2;

  switch(getRegisterDataType(REGISTER_X)) {
    case dtLongInteger: convertLongIntegerRegisterToRealIc(REGISTER_X, &c.real);
                        realIcZero(&c.imag);
                        break;

    case dtReal16:      real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &c.real);
                        realIcZero(&c.imag);
                        break;

    case dtComplex16:   real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &c.real);
                        real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_X), &c.imag);
                        realCoefs = false;
                        break;

    case dtReal34:      real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &c.real);
                        realIcZero(&c.imag);
                        result16 = false;
                        break;

    case dtComplex34:   real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &c.real);
                        real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_X), &c.imag);
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
    case dtLongInteger: convertLongIntegerRegisterToRealIc(REGISTER_Y, &b.real);
                        realIcZero(&b.imag);
                        break;

    case dtReal16:      real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &b.real);
                        realIcZero(&b.imag);
                        break;

    case dtComplex16:   real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &b.real);
                        real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_Y), &b.imag);
                        realCoefs = false;
                        break;

    case dtReal34:      real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &b.real);
                        realIcZero(&b.imag);
                        result16 = false;
                        break;

    case dtComplex34:   real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &b.real);
                        real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_Y), &b.imag);
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
    case dtLongInteger: convertLongIntegerRegisterToRealIc(REGISTER_Z, &a.real);
                        realIcZero(&a.imag);
                        break;

    case dtReal16:      real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Z), &a.real);
                        realIcZero(&a.imag);
                        break;

    case dtComplex16:   real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Z), &a.real);
                        real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_Z), &a.imag);
                        realCoefs = false;
                        break;

    case dtReal34:      real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Z), &a.real);
                        realIcZero(&a.imag);
                        result16 = false;
                        break;

    case dtComplex34:   real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Z), &a.real);
                        real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_Z), &a.imag);
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

  if(    realIcIsNaN(&a.real) || realIcIsNaN(&a.imag)
      || realIcIsNaN(&b.real) || realIcIsNaN(&b.imag)
      || realIcIsNaN(&c.real) || realIcIsNaN(&c.imag)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function fnSlqv:", "cannot use NaN as X, Y, or Z input of SLVQ", NULL, NULL);
    #endif
    return;
  }

  if(   realIcIsZero(&a.real) && realIcIsZero(&a.imag)
     && realIcIsZero(&b.real) && realIcIsZero(&b.imag)) {
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
    if(realIcIsZero(&a.real)) {
      // bx + c = 0   (b is not 0 here)

      // r = b²
      realIcMultiply(&b.real, &b.real, &r.real);
      realIcZero(&r.imag);

      // x1 = x2 = -c/b
      realIcDivide(&c.real, &b.real, &x1.real);
      realIcChangeSign(&x1.real);
      realIcCopy(&x1.real, &x2.real);
    }
    else {
      // ax² + bx + c = 0   (a is not 0 here)

      // r = b² - 4ac
      realIcMultiply(const__4, &a.real, &r.real);
      realIcMultiply(&c.real, &r.real, &r.real);
      realIcFMA(&b.real, &b.real, &r.real, &r.real);
      realIcZero(&r.imag);

      if(realIcIsPositive(&r.real)) { // real roots
        // x1 = (-b - sign(b)*sqrt(r)) / 2a
        realIcSquareRoot(&r.real, &x1.real);
        if(realIcIsPositive(&b.real)) {
          realIcChangeSign(&x1.real);
        }
        realIcSubtract(&x1.real, &b.real, &x1.real);
        realIcMultiply(&x1.real, const_1on2, &x1.real);
        realIcDivide(&x1.real, &a.real, &x1.real);

        // x2 = c / ax1
        realIcDivide(&c.real, &a.real, &x2.real);
        realIcDivide(&x2.real, &x1.real, &x2.real);
      }
      else { // complex roots
        realRoots = false;

        // x1 = (-b - sign(b)*sqrt(r)) / 2a
        realIcMinus(&r.real, &x1.real);
        realIcSquareRoot(&x1.real, &x1.imag);
        realIcZero(&x1.real);
        if(realIcIsPositive(&b.real)) {
          realIcChangeSign(&x1.imag);
        }
        realIcSubtract(&x1.real, &b.real, &x1.real);
        realIcSubtract(&x1.imag, &b.imag, &x1.imag);
        realIcMultiply(&x1.real, const_1on2, &x1.real);
        realIcMultiply(&x1.imag, const_1on2, &x1.imag);
        realIcDivide(&x1.real, &a.real, &x1.real);
        realIcDivide(&x1.imag, &a.real, &x1.imag);

        // x2 = conj(x1)
        complexIcCopy(&x1, &x2);
        realIcChangeSign(&x2.imag);
      }
    }
  }
  else { // Complex coefficients
    if(realIcIsZero(&a.real) && realIcIsZero(&a.imag)) {
      // bx + c = 0   (b is not 0 here)

      // r = b²
      mulCoIcCoIc(&b, &b, &r);

      // x1 = x2 = -c/b
      divCoIcCoIc(&c, &b, &x1);
      realIcChangeSign(&x1.real);
      realIcChangeSign(&x1.imag);
      complexIcCopy(&x1, &x2);
    }
    else {
      // ax² + bx + c = 0   (a is not 0 here)

      // r = b² - 4ac
      realIcMultiply(const__4, &a.real, &r.real);
      realIcMultiply(const__4, &a.imag, &r.imag);
      mulCoIcCoIc(&c, &r, &r);
      mulCoIcCoIc(&b, &b, &x1);
      realIcAdd(&x1.real, &r.real, &r.real);
      realIcAdd(&x1.imag, &r.imag, &r.imag);

      // x1 = (-b + sqrt(r)) / 2a
      complexIcCopy(&r, &x1);
      realIcRectangularToPolar(&x1.real, &x1.imag, &x1.real, &x1.imag);
      realIcSquareRoot(&x1.real, &x1.real);
      realIcMultiply(&x1.imag, const_0_5, &x1.imag);
      realIcPolarToRectangular(&x1.real, &x1.imag, &x1.real, &x1.imag);

      realIcSubtract(&x1.real, &b.real, &x1.real);
      realIcSubtract(&x1.imag, &b.imag, &x1.imag);
      realIcMultiply(&x1.real, const_1on2, &x1.real);
      realIcMultiply(&x1.imag, const_1on2, &x1.imag);
      divCoIcCoIc(&x1, &a, &x1);

      // x2 = c / ax1
      divCoIcCoIc(&c, &a, &x2);
      divCoIcCoIc(&x2, &x1, &x2);
    }
  }

  if(realRoots) {
    if(result16) {
      reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);
      reallocateRegister(REGISTER_Y, dtReal16, REAL16_SIZE, AM_NONE);
      reallocateRegister(REGISTER_Z, dtReal16, REAL16_SIZE, AM_NONE);
      realIcToReal16(&x1.real, REGISTER_REAL16_DATA(REGISTER_X));
      realIcToReal16(&x2.real, REGISTER_REAL16_DATA(REGISTER_Y));
      realIcToReal16(&r.real,  REGISTER_REAL16_DATA(REGISTER_Z));
    }
    else { // realRoots && !result16
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
      reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, AM_NONE);
      reallocateRegister(REGISTER_Z, dtReal34, REAL34_SIZE, AM_NONE);
      realIcToReal34(&x1.real, REGISTER_REAL34_DATA(REGISTER_X));
      realIcToReal34(&x2.real, REGISTER_REAL34_DATA(REGISTER_Y));
      realIcToReal34(&r.real,  REGISTER_REAL34_DATA(REGISTER_Z));
    }
  }
  else { // !realRoots
    if(result16) {
      reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);
      reallocateRegister(REGISTER_Y, dtComplex16, COMPLEX16_SIZE, AM_NONE);
      realIcToReal16(&x1.real, REGISTER_REAL16_DATA(REGISTER_X));
      realIcToReal16(&x1.imag, REGISTER_IMAG16_DATA(REGISTER_X));
      realIcToReal16(&x2.real, REGISTER_REAL16_DATA(REGISTER_Y));
      realIcToReal16(&x2.imag, REGISTER_IMAG16_DATA(REGISTER_Y));
      realIcToReal16(&r.real,  REGISTER_REAL16_DATA(REGISTER_Z));
      if(realIcIsZero(&r.imag)) { // r is real
        reallocateRegister(REGISTER_Z, dtReal16, REAL16_SIZE, AM_NONE);
        realIcToReal16(&r.real, REGISTER_REAL16_DATA(REGISTER_Z));
      }
      else {
        reallocateRegister(REGISTER_Z, dtComplex16, COMPLEX16_SIZE, AM_NONE);
        realIcToReal16(&r.real, REGISTER_REAL16_DATA(REGISTER_Z));
        realIcToReal16(&r.imag, REGISTER_IMAG16_DATA(REGISTER_Z));
      }
    }
    else { // !realRoots && !result16
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
      reallocateRegister(REGISTER_Y, dtComplex34, COMPLEX34_SIZE, AM_NONE);
      realIcToReal34(&x1.real, REGISTER_REAL34_DATA(REGISTER_X));
      realIcToReal34(&x1.imag, REGISTER_IMAG34_DATA(REGISTER_X));
      realIcToReal34(&x2.real, REGISTER_REAL34_DATA(REGISTER_Y));
      realIcToReal34(&x2.imag, REGISTER_IMAG34_DATA(REGISTER_Y));
      realIcToReal34(&r.real,  REGISTER_REAL34_DATA(REGISTER_Z));
      if(realIcIsZero(&r.imag)) { // r is real
        reallocateRegister(REGISTER_Z, dtReal34, REAL34_SIZE, AM_NONE);
        realIcToReal34(&r.real, REGISTER_REAL34_DATA(REGISTER_Z));
      }
      else {
        reallocateRegister(REGISTER_Z, dtComplex34, COMPLEX34_SIZE, AM_NONE);
        realIcToReal34(&r.real, REGISTER_REAL34_DATA(REGISTER_Z));
        realIcToReal34(&r.imag, REGISTER_IMAG34_DATA(REGISTER_Z));
      }
    }
  }

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
  adjustResult(REGISTER_Y, false, true, REGISTER_Y, -1, -1);
  adjustResult(REGISTER_Z, false, true, REGISTER_Z, -1, -1);
}
