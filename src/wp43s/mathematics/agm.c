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

#include "wp43s.h"


void fnAgm(uint16_t unusedParamButMandatory) {
  bool_t result16=true, realInput=true;
  complex39_t a, b, c;

  switch(getRegisterDataType(REGISTER_X)) {
    case dtLongInteger: convertLongIntegerRegisterToReal(REGISTER_X, (real_t *)&a.real, &ctxtReal39);
                        realZero(&a.imag);
                        break;

    case dtReal16:      real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &a.real);
                        realZero(&a.imag);
                        break;

    case dtComplex16:   real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &a.real);
                        real16ToReal(REGISTER_IMAG16_DATA(REGISTER_X), &a.imag);
                        realInput = false;
                        break;

    case dtReal34:      real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &a.real);
                        realZero(&a.imag);
                        result16 = false;
                        break;

    case dtComplex34:   real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &a.real);
                        real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &a.imag);
                        realInput = false;
                        result16 = false;
                        break;

    default:            displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
                        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                          sprintf(errorMessage, "cannot calculate AGM with %s in X", getRegisterDataTypeName(REGISTER_X, true, false));
                          showInfoDialog("In function fnAgm:", errorMessage, NULL, NULL);
                        #endif
                        return;
  }

  switch(getRegisterDataType(REGISTER_Y)) {
    case dtLongInteger: convertLongIntegerRegisterToReal(REGISTER_Y, (real_t *)&b.real, &ctxtReal39);
                        realZero(&b.imag);
                        break;

    case dtReal16:      real16ToReal(REGISTER_REAL16_DATA(REGISTER_Y), &b.real);
                        realZero(&b.imag);
                        break;

    case dtComplex16:   real16ToReal(REGISTER_REAL16_DATA(REGISTER_Y), &b.real);
                        real16ToReal(REGISTER_IMAG16_DATA(REGISTER_Y), &b.imag);
                        realInput = false;
                        break;

    case dtReal34:      real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &b.real);
                        realZero(&b.imag);
                        result16 = false;
                        break;

    case dtComplex34:   real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &b.real);
                        real34ToReal(REGISTER_IMAG34_DATA(REGISTER_Y), &b.imag);
                        realInput = false;
                        result16 = false;
                        break;

    default:            displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_Y);
                        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                          sprintf(errorMessage, "cannot calculate AGM with %s in Y", getRegisterDataTypeName(REGISTER_Y, true, false));
                          showInfoDialog("In function fnAgm:", errorMessage, NULL, NULL);
                        #endif
                        return;
  }

  if(    realIsNaN(&a.real) || realIsNaN(&a.imag)
      || realIsNaN(&b.real) || realIsNaN(&b.imag)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function fnAgm:", "cannot use NaN as X or Y input of AGM", NULL, NULL);
    #endif
    return;
  }

  if(realInput && (realIsNegative(&a.real) || realIsNegative(&b.real))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function fnAgm:", "cannot use negative X and Y as input of AGM", NULL, NULL);
    #endif
    return;
  }

  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(realInput) {
    while(realIdenticalDigits(&a.real, &b.real) <= (result16 ? 16 : 34)) {
      realAdd(&a.real, &b.real, &c.real, &ctxtReal39);         // c = a + b
      realMultiply(&a.real, &b.real, &b.real, &ctxtReal39);    // b = a * b
      realSquareRoot(&b.real, &b.real, &ctxtReal39);           // b = sqrt(a * b)
      realMultiply(&c.real, const_1on2, &a.real, &ctxtReal39); // a = (a + b) / 2
    }

    if(result16) {
      reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);
      realToReal16(&a.real, REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
      realToReal34(&a.real, REGISTER_REAL34_DATA(REGISTER_X));
    }
  }
  else { // Complex input
    while(realIdenticalDigits(&a.real, &b.real) <= (result16 ? 16 : 34) || realIdenticalDigits(&a.imag, &b.imag) <= (result16 ? 16 : 34)) {
      realAdd(&a.real, &b.real, &c.real, &ctxtReal39);         // c = a + b real part
      realAdd(&a.imag, &b.imag, &c.imag, &ctxtReal39);         // c = a + b imag part

      mulCo39Co39(&a, &b, &b);                                 // b = a * b

      // b = sqrt(a * b)
      real39RectangularToPolar(&b.real, &b.imag, &b.real, &b.imag);
      realSquareRoot(&b.real, &b.real, &ctxtReal39);
      realMultiply(&b.imag, const_0_5, &b.imag, &ctxtReal39);
      real39PolarToRectangular(&b.real, &b.imag, &b.real, &b.imag);

      realMultiply(&c.real, const_1on2, &a.real, &ctxtReal39); // a = (a + b) / 2 real part
      realMultiply(&c.imag, const_1on2, &a.imag, &ctxtReal39); // a = (a + b) / 2 imag part
    }

    if(result16) {
      reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);
      realToReal16(&a.real, REGISTER_REAL16_DATA(REGISTER_X));
      realToReal16(&a.imag, REGISTER_IMAG16_DATA(REGISTER_X));
    }
    else {
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
      realToReal34(&a.real, REGISTER_REAL34_DATA(REGISTER_X));
      realToReal34(&a.imag, REGISTER_IMAG34_DATA(REGISTER_X));
    }
  }

  adjustResult(REGISTER_X, true, true, REGISTER_X, -1, -1);
}
