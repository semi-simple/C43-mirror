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
  bool_t realCoefs=true, realRoots=true;
  real75_t aReal, bReal, cReal, rReal, x1Real, x2Real;
  real75_t aImag, bImag, cImag, rImag, x1Imag, x2Imag;

  switch(getRegisterDataType(REGISTER_X)) {
    case dtLongInteger: convertLongIntegerRegisterToReal(REGISTER_X, (real_t *)&cReal, &ctxtReal75);
                        realZero(&cImag);
                        break;

    case dtReal34:      real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &cReal);
                        realZero(&cImag);
                        break;

    case dtComplex34:   real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &cReal);
                        real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &cImag);
                        realCoefs = false;
                        break;

    default:            displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
                        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                          sprintf(errorMessage, "cannot SLVQ with %s in X", getRegisterDataTypeName(REGISTER_X, true, false));
                          showInfoDialog("In function fnSlqv:", errorMessage, NULL, NULL);
                        #endif
                        return;
  }

  switch(getRegisterDataType(REGISTER_Y)) {
    case dtLongInteger: convertLongIntegerRegisterToReal(REGISTER_Y, (real_t *)&bReal, &ctxtReal75);
                        realZero(&bImag);
                        break;

    case dtReal34:      real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &bReal);
                        realZero(&bImag);
                        break;

    case dtComplex34:   real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &bReal);
                        real34ToReal(REGISTER_IMAG34_DATA(REGISTER_Y), &bImag);
                        realCoefs = false;
                        break;

    default:            displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_Y);
                        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                          sprintf(errorMessage, "cannot SLVQ with %s in Y", getRegisterDataTypeName(REGISTER_Y, true, false));
                          showInfoDialog("In function fnSlqv:", errorMessage, NULL, NULL);
                        #endif
                        return;
  }

  switch(getRegisterDataType(REGISTER_Z)) {
    case dtLongInteger: convertLongIntegerRegisterToReal(REGISTER_Z, (real_t *)&aReal, &ctxtReal75);
                        realZero(&aImag);
                        break;

    case dtReal34:      real34ToReal(REGISTER_REAL34_DATA(REGISTER_Z), &aReal);
                        realZero(&aImag);
                        break;

    case dtComplex34:   real34ToReal(REGISTER_REAL34_DATA(REGISTER_Z), &aReal);
                        real34ToReal(REGISTER_IMAG34_DATA(REGISTER_Z), &aImag);
                        realCoefs = false;
                        break;

    default:            displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_Z);
                        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                          sprintf(errorMessage, "cannot SLVQ with %s in Z", getRegisterDataTypeName(REGISTER_Z, true, false));
                          showInfoDialog("In function fnSlqv:", errorMessage, NULL, NULL);
                        #endif
                        return;
  }

  if(    realIsNaN(&aReal) || realIsNaN(&aImag)
      || realIsNaN(&bReal) || realIsNaN(&bImag)
      || realIsNaN(&cReal) || realIsNaN(&cImag)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function fnSlqv:", "cannot use NaN as X, Y, or Z input of SLVQ", NULL, NULL);
    #endif
    return;
  }

  if(   realIsZero(&aReal) && realIsZero(&aImag)
     && realIsZero(&bReal) && realIsZero(&bImag)) {
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
    if(realIsZero(&aReal)) {
      // bx + c = 0   (b is not 0 here)

      // r = b²
      realMultiply(&bReal, &bReal, &rReal, &ctxtReal75);
      realZero(&rImag);

      // x1 = x2 = -c/b
      realDivide(&cReal, &bReal, &x1Real, &ctxtReal75);
      realChangeSign(&x1Real);
      realCopy(&x1Real, &x2Real);
    }
    else if(realIsZero(&cReal)) {
      // ax² + bx = x(ax + b) = 0   (a is not 0 here)

      // r = b²
      realMultiply(&bReal, &bReal, &rReal, &ctxtReal75);
      realZero(&rImag);

      // x1 = 0
      realZero(&x1Real);

      // x2 = -b/a
      realDivide(&bReal, &aReal, &x2Real, &ctxtReal75);
      realChangeSign(&x2Real);
    }
    else {
      // ax² + bx + c = 0   (a and c are not 0 here)

      // r = b² - 4ac
      realMultiply(const__4, &aReal, &rReal, &ctxtReal75);
      realMultiply(&cReal, &rReal, &rReal, &ctxtReal75);
      realFMA(&bReal, &bReal, &rReal, &rReal, &ctxtReal75);
      realZero(&rImag);

      if(realIsPositive(&rReal)) { // real roots
        // x1 = (-b - sign(b)*sqrt(r)) / 2a
        realSquareRoot(&rReal, &x1Real, &ctxtReal75);
        if(realIsPositive(&bReal)) {
          realChangeSign(&x1Real);
        }
        realSubtract(&x1Real, &bReal, &x1Real, &ctxtReal75);
        realMultiply(&x1Real, const_1on2, &x1Real, &ctxtReal75);
        realDivide(&x1Real, &aReal, &x1Real, &ctxtReal75);

        // x2 = c / ax1  (x1 connot be 0 here)
        realDivide(&cReal, &aReal, &x2Real, &ctxtReal75);
        realDivide(&x2Real, &x1Real, &x2Real, &ctxtReal75);
      }
      else { // complex roots
        realRoots = false;

        // x1 = (-b - sign(b)*sqrt(r)) / 2a
        realMinus(&rReal, &x1Real, &ctxtReal75);
        realSquareRoot(&x1Real, &x1Imag, &ctxtReal75);
        realZero(&x1Real);
        if(realIsPositive(&bReal)) {
          realChangeSign(&x1Imag);
        }
        realSubtract(&x1Real, &bReal, &x1Real, &ctxtReal75);
        realSubtract(&x1Imag, &bImag, &x1Imag, &ctxtReal75);
        realMultiply(&x1Real, const_1on2, &x1Real, &ctxtReal75);
        realMultiply(&x1Imag, const_1on2, &x1Imag, &ctxtReal75);
        realDivide(&x1Real, &aReal, &x1Real, &ctxtReal75);
        realDivide(&x1Imag, &aReal, &x1Imag, &ctxtReal75);

        // x2 = conj(x1)
        realCopy(&x1Real, &x2Real);
        realCopy(&x1Imag, &x2Imag);
        realChangeSign(&x2Imag);
      }
    }
  }
  else { // Complex coefficients
    if(realIsZero(&aReal) && realIsZero(&aImag)) {
      // bx + c = 0   (b is not 0 here)

      // r = b²
      mulCo75Co75(&bReal, &bImag, &bReal, &bImag, &rReal, &rImag);

      // x1 = x2 = -c/b
      divCo75Co75(&cReal, &cImag, &bReal, &bImag, &x1Real, &x1Imag);
      realChangeSign(&x1Real);
      realChangeSign(&x1Imag);
      realCopy(&x1Real, &x2Real);
      realCopy(&x1Imag, &x2Imag);
    }
    else if(realIsZero(&cReal) && realIsZero(&cImag)) {
      // ax² + bx = x(ax + b) = 0   (a is not 0 here)

      // r = b²
      mulCo75Co75(&bReal, &bImag, &bReal, &bImag, &rReal, &rImag);

      // x1 = 0
      realZero(&x1Real);
      realZero(&x1Imag);

      // x2 = -b/a
      divCo75Co75(&bReal, &bImag, &aReal, &aImag, &x2Real, &x2Imag);
      realChangeSign(&x2Real);
      realChangeSign(&x2Imag);
    }
    else {
      // ax² + bx + c = 0   (a and c are not 0 here)

      // r = b² - 4ac
      realMultiply(const__4, &aReal, &rReal, &ctxtReal75);
      realMultiply(const__4, &aImag, &rImag, &ctxtReal75);
      mulCo75Co75(&cReal, &cImag, &rReal, &rImag, &rReal, &rImag);
      mulCo75Co75(&bReal, &bImag, &bReal, &bImag, &x1Real, &x1Imag);
      realAdd(&x1Real, &rReal, &rReal, &ctxtReal75);
      realAdd(&x1Imag, &rImag, &rImag, &ctxtReal75);

      // x1 = (-b + sqrt(r)) / 2a
      realCopy(&rReal, &x1Real);
      realCopy(&rImag, &x1Imag);
      real75RectangularToPolar((real_t *)&x1Real, (real_t *)&x1Imag, (real_t *)&x1Real, (real_t *)&x1Imag);
      realSquareRoot(&x1Real, &x1Real, &ctxtReal75);
      realMultiply(&x1Imag, const_1on2, &x1Imag, &ctxtReal75);
      real75PolarToRectangular((real_t *)&x1Real, (real_t *)&x1Imag, (real_t *)&x1Real, (real_t *)&x1Imag);

      realSubtract(&x1Real, &bReal, &x1Real, &ctxtReal75);
      realSubtract(&x1Imag, &bImag, &x1Imag, &ctxtReal75);
      realMultiply(&x1Real, const_1on2, &x1Real, &ctxtReal75);
      realMultiply(&x1Imag, const_1on2, &x1Imag, &ctxtReal75);
      divCo75Co75(&x1Real, &x1Imag, &aReal, &aImag, &x1Real, &x1Imag);

      // x2 = c / ax1  (x1 connot be 0 here)
      divCo75Co75(&cReal, &cImag, &aReal, &aImag, &x2Real, &x2Imag);
      divCo75Co75(&x2Real, &x2Imag, &x1Real, &x1Imag, &x2Real, &x2Imag);
    }
  }

  if(realRoots) {
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
    reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, AM_NONE);
    reallocateRegister(REGISTER_Z, dtReal34, REAL34_SIZE, AM_NONE);
    realToReal34(&x1Real, REGISTER_REAL34_DATA(REGISTER_X));
    realToReal34(&x2Real, REGISTER_REAL34_DATA(REGISTER_Y));
    realToReal34(&rReal,  REGISTER_REAL34_DATA(REGISTER_Z));
  }
  else { // !realRoots
    if(realIsZero(&x1Imag)) { // x1 is real
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
      realToReal34(&x1Real, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
      realToReal34(&x1Real, REGISTER_REAL34_DATA(REGISTER_X));
      realToReal34(&x1Imag, REGISTER_IMAG34_DATA(REGISTER_X));
    }

    if(realIsZero(&x2Imag)) { // x2 is real
      reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, AM_NONE);
      realToReal34(&x2Real, REGISTER_REAL34_DATA(REGISTER_Y));
    }
    else {
      reallocateRegister(REGISTER_Y, dtComplex34, COMPLEX34_SIZE, AM_NONE);
      realToReal34(&x2Real, REGISTER_REAL34_DATA(REGISTER_Y));
      realToReal34(&x2Imag, REGISTER_IMAG34_DATA(REGISTER_Y));
    }

    if(realIsZero(&rImag)) { // r is real
      reallocateRegister(REGISTER_Z, dtReal34, REAL34_SIZE, AM_NONE);
      realToReal34(&rReal, REGISTER_REAL34_DATA(REGISTER_Z));
    }
    else {
      reallocateRegister(REGISTER_Z, dtComplex34, COMPLEX34_SIZE, AM_NONE);
      realToReal34(&rReal, REGISTER_REAL34_DATA(REGISTER_Z));
      realToReal34(&rImag, REGISTER_IMAG34_DATA(REGISTER_Z));
    }
  }

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
  adjustResult(REGISTER_Y, false, true, REGISTER_Y, -1, -1);
  adjustResult(REGISTER_Z, false, true, REGISTER_Z, -1, -1);
}
