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
 * \file agm.c
 ***********************************************/

#include "mathematics/agm.h"

#include "constantPointers.h"
#include "debug.h"
#include "error.h"
#include "mathematics/comparisonReals.h"
#include "mathematics/multiplication.h"
#include "mathematics/toRect.h"
#include "mathematics/toPolar.h"
#include "registers.h"
#include "registerValueConversions.h"

#include "wp43s.h"


void fnAgm(uint16_t unusedButMandatoryParameter) {
  bool_t realInput=true;
  real_t aReal, bReal;
  real_t aImag, bImag;

  switch(getRegisterDataType(REGISTER_X)) {
    case dtLongInteger: convertLongIntegerRegisterToReal(REGISTER_X, &aReal, &ctxtReal39);
                        realZero(&aImag);
                        break;

    case dtReal34:      real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &aReal);
                        realZero(&aImag);
                        break;

    case dtComplex34:   real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &aReal);
                        real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &aImag);
                        realInput = false;
                        break;

    default:            displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
                        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                          sprintf(errorMessage, "cannot calculate AGM with %s in X", getRegisterDataTypeName(REGISTER_X, true, false));
                          moreInfoOnError("In function fnAgm:", errorMessage, NULL, NULL);
                        #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
                        return;
  }

  switch(getRegisterDataType(REGISTER_Y)) {
    case dtLongInteger: convertLongIntegerRegisterToReal(REGISTER_Y, &bReal, &ctxtReal39);
                        realZero(&bImag);
                        break;

    case dtReal34:      real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &bReal);
                        realZero(&bImag);
                        break;

    case dtComplex34:   real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &bReal);
                        real34ToReal(REGISTER_IMAG34_DATA(REGISTER_Y), &bImag);
                        realInput = false;
                        break;

    default:            displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_Y);
                        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                          sprintf(errorMessage, "cannot calculate AGM with %s in Y", getRegisterDataTypeName(REGISTER_Y, true, false));
                          moreInfoOnError("In function fnAgm:", errorMessage, NULL, NULL);
                        #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
                        return;
  }

  if(realInput && (realIsNegative(&aReal) || realIsNegative(&bReal))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      moreInfoOnError("In function fnAgm:", "cannot use negative X and Y as input of AGM", NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return;
  }

  if(!saveLastX()) return;

  if(realInput) {
    realAgm(&aReal, &bReal, &aReal, &ctxtReal39);

    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    realToReal34(&aReal, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else { // Complex input
    complexAgm(&aReal, &aImag, &bReal, &bImag, &aReal, &aImag, &ctxtReal39);

    reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
    realToReal34(&aReal, REGISTER_REAL34_DATA(REGISTER_X));
    realToReal34(&aImag, REGISTER_IMAG34_DATA(REGISTER_X));
  }

  adjustResult(REGISTER_X, true, true, REGISTER_X, -1, -1);
}

void realAgm(const real_t *a, const real_t *b, real_t *res, realContext_t *realContext) {
  real_t aReal, bReal, cReal;

  realCopy(a, &aReal);
  realCopy(b, &bReal);

  while(!realCompareEqual(&aReal, &bReal) && realIdenticalDigits(&aReal, &bReal) <= 34) {
    realAdd(&aReal, &bReal, &cReal, realContext);          // c = a + b
    realMultiply(&aReal, &bReal, &bReal, realContext);     // b = a * b
    realSquareRoot(&bReal, &bReal, realContext);           // b = sqrt(a * b)
    realMultiply(&cReal, const_1on2, &aReal, realContext); // a = (a + b) / 2
  }

  realCopy(&aReal, res);
}

void complexAgm(const real_t *ar, const real_t *ai, const real_t *br, const real_t *bi, real_t *resr, real_t *resi, realContext_t *realContext) {
  real_t aReal, bReal, cReal;
  real_t aImag, bImag, cImag;

  realCopy(ar, &aReal); realCopy(ai, &aImag);
  realCopy(br, &bReal); realCopy(bi, &bImag);

  while((!realCompareEqual(&aReal, &bReal) && realIdenticalDigits(&aReal, &bReal) <= 34) || (!realCompareEqual(&aImag, &bImag) && realIdenticalDigits(&aImag, &bImag) <= 34)) {
    realAdd(&aReal, &bReal, &cReal, realContext);                                   // c = a + b real part
    realAdd(&aImag, &bImag, &cImag, realContext);                                   // c = a + b imag part

    mulComplexComplex(&aReal, &aImag, &bReal, &bImag, &bReal, &bImag, realContext); // b = a * b

    // b = sqrt(a * b)
    realRectangularToPolar(&bReal, &bImag, &bReal, &bImag, realContext);
    realSquareRoot(&bReal, &bReal, realContext);
    realMultiply(&bImag, const_1on2, &bImag, realContext);
    realPolarToRectangular(&bReal, &bImag, &bReal, &bImag, realContext);

    realMultiply(&cReal, const_1on2, &aReal, realContext); // a = (a + b) / 2 real part
    realMultiply(&cImag, const_1on2, &aImag, realContext); // a = (a + b) / 2 imag part
  }

  realCopy(&aReal, resr); realCopy(&aImag, resi);
}
