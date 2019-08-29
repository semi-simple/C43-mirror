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
 * \file square.c
 ***********************************************/

#include "wp43s.h"



void (* const square[13])(void) = {
// regX ==> 1              2           3           4           5            6            7            8           9           10            11          12          13
//          Long integer   Real16      Complex16   Angle16     Time         Date         String       Real16 mat  Complex16 m Short integer Real34      Complex34   Angle34
            squareLonI,    squareRe16, squareCo16, squareAn16, squareError, squareError, squareError, squareRm16, squareCm16, squareShoI,   squareRe34, squareCo34, squareAn34
};



/********************************************//**
 * \brief Data type error in squaring
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void squareError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot square %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnSquare:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and regX × regX ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void fnSquare(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  square[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



void squareLonI(void) {
  longInteger_t lgInt;

  convertLongIntegerRegisterToLongInteger(REGISTER_X, lgInt);
  longIntegerMultiply(lgInt, lgInt, lgInt);
  convertLongIntegerToLongIntegerRegister(lgInt, REGISTER_X);
  longIntegerFree(lgInt);
}



void squareRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function squareRe16:", "cannot use NaN as X input of ^2", NULL, NULL);
    #endif
    return;
  }

  real16Multiply(REGISTER_REAL16_DATA(REGISTER_X), REGISTER_REAL16_DATA(REGISTER_X), REGISTER_REAL16_DATA(REGISTER_X));
}



void squareCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function squareCo16:", "cannot use NaN as X input of ^2", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, b, imagPart;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &a);
  real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_X), &b);

  // (a + bi)² = a² - b² + 2abi
  // Imaginary part
  realIcMultiply(&a, const_2, &imagPart);
  realIcMultiply(&b, &imagPart, &imagPart);

  // real part
  realIcMultiply(&a, &a, &a);
  realIcMultiply(&b, &b, &b);
  realIcSubtract(&a, &b, &a);

  realIcToReal16(&a,        REGISTER_REAL16_DATA(REGISTER_X));
  realIcToReal16(&imagPart, REGISTER_IMAG16_DATA(REGISTER_X));
}



void squareAn16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function squareAn16:", "cannot use NaN as X input of ^2", NULL, NULL);
    #endif
    return;
  }

  real16Multiply(REGISTER_REAL16_DATA(REGISTER_X), REGISTER_REAL16_DATA(REGISTER_X), REGISTER_REAL16_DATA(REGISTER_X));
  setRegisterDataType(REGISTER_X, dtReal16, TAG_NONE);
}



void squareRm16(void) {
  fnToBeCoded();
}



void squareCm16(void) {
  fnToBeCoded();
}



void squareShoI(void) {
  *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = WP34S_intMultiply(*(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)), *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)));
}



void squareRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function squareRe34:", "cannot use NaN as X input of ^2", NULL, NULL);
    #endif
    return;
  }

  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
}



void squareCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function squareCo34:", "cannot use NaN as X input of ^2", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, b, imagPart;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &a);
  real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_X), &b);

  // (a + bi)² = a² - b² + 2abi
  // Imaginary part
  realIcMultiply(&a, const_2, &imagPart);
  realIcMultiply(&b, &imagPart, &imagPart);

  // real part
  realIcMultiply(&a, &a, &a);
  realIcMultiply(&b, &b, &b);
  realIcSubtract(&a, &b, &a);

  realIcToReal34(&a,        REGISTER_REAL34_DATA(REGISTER_X));
  realIcToReal34(&imagPart, REGISTER_IMAG34_DATA(REGISTER_X));
}



void squareAn34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function squareAn34:", "cannot use NaN as X input of ^2", NULL, NULL);
    #endif
    return;
  }

  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterDataType(REGISTER_X, dtReal34, TAG_NONE);
}
