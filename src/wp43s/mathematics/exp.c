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
 * \file exp.c
 ***********************************************/

#include "wp43s.h"



void (* const Exp[13])(void) = {
// regX ==> 1            2        3         4        5         6         7         8          9           10            11       12        13
//          Long integer Real16   Complex16 Anfle16  Time      Date      String    Real16 mat Complex16 m Short integer Real34   Complex34 Angle34
            expLonI,     expRe16, expCo16,  expAn16, expError, expError, expError, expRm16,   expCm16,    expShoI,      expRe34, expCo34,  expAn34
};



/********************************************//**
 * \brief Data type error in exp
 *
 * \param void
 * \return void
 ***********************************************/
void expError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate Exp for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnExp:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and exp(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnExp(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  Exp[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



/**********************************************************************
 * In all the functions below:
 * if X is a number then X = a + ib
 * The variables a and b are used for intermediate calculations
 ***********************************************************************/

void expLonI(void) {
  realIc_t a;

  convertLongIntegerRegisterToRealIc(REGISTER_X, &a);
  realIcExp(&a, &a);
  reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, TAG_NONE);
  realIcToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
}



void expRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function expRe16:", "cannot use NaN as X input of exp", NULL, NULL);
    #endif
    return;
  }

  realIc_t a;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &a);
  realIcExp(&a, &a);
  realIcToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
}



void expCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function expCo16:", "cannot use NaN as X input of exp", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, b, factor;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &a);
  real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_X), &b);

  realIcExp(&a, &factor);
  realIcPolarToRectangular(const_1, &b, &a, &b);
  realIcMultiply(&factor, &a, &a);
  realIcMultiply(&factor, &b, &b);

  realIcToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
  realIcToReal16(&b, REGISTER_IMAG16_DATA(REGISTER_X));
}



void expAn16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function expAn16:", "cannot use NaN as X input of exp", NULL, NULL);
    #endif
    return;
  }

  realIc_t a;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &a);
  realIcExp(&a, &a);
  realIcToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
  setRegisterDataType(REGISTER_X, dtReal16, TAG_NONE);
}



void expRm16(void) {
  fnToBeCoded();
}



void expCm16(void) {
  fnToBeCoded();
}



void expShoI(void) {
  realIc_t a;

  convertShortIntegerRegisterToRealIc(REGISTER_X, &a);
  realIcExp(&a, &a);
  reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, TAG_NONE);
  realIcToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
}



void expRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function expRe34:", "cannot use NaN as X input of exp", NULL, NULL);
    #endif
    return;
  }

  realIc_t a;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &a);
  realIcExp(&a, &a);
  realIcToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
}



void expCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function expCo34:", "cannot use NaN as X input of exp", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, b, factor;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &a);
  real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_X), &b);

  realIcExp(&a, &factor);
  realIcPolarToRectangular(const_1, &b, &a, &b);
  realIcMultiply(&factor, &a, &a);
  realIcMultiply(&factor, &b, &b);

  realIcToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
  realIcToReal34(&b, REGISTER_IMAG34_DATA(REGISTER_X));
}



void expAn34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function expAn34:", "cannot use NaN as X input of exp", NULL, NULL);
    #endif
    return;
  }

  realIc_t a;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &a);
  realIcExp(&a, &a);
  realIcToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterDataType(REGISTER_X, dtReal34, TAG_NONE);
}
