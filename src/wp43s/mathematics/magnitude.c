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
 * \file magnitude.c
 ***********************************************/

#include "wp43s.h"



void (* const magnitude[12])(void) = {
// regX ==> 1              2              3              4               5               6               7               8              9              10             11             12
//          Long integer   Real16         Complex16      Angle16         Time            Date            String          Real16 mat     Complex16 m    Short integer  Real34         Complex34
            magnitudeLonI, magnitudeRe16, magnitudeCo16, magnitudeError, magnitudeError, magnitudeError, magnitudeError, magnitudeRm16, magnitudeCm16, magnitudeShoI, magnitudeRe34, magnitudeCo34
};



/********************************************//**
 * \brief Data type error in |x|
 *
 * \param void
 * \return void
 ***********************************************/
void magnitudeError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate |x| for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnMagnitude:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief Returns the absolute value of an integer or a real and the magnitude of a complex
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnMagnitude(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  magnitude[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
}



void magnitudeLonI(void) {
  setRegisterLongIntegerSign(REGISTER_X, LONG_INTEGER_POSITIVE);
}



void magnitudeRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function magnitudeRe16:", "cannot use NaN as X input of |x|", NULL, NULL);
    #endif
    return;
  }

  real16SetPositiveSign(REGISTER_REAL16_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



void magnitudeCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function magnitudeCo16:", "cannot use NaN as X input of |x|", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, b;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &a);
  real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_X), &b);
  reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);

  realIcMultiply(&a, &a, &a);
  realIcFMA(&b, &b, &a, &a);
  realIcSquareRoot(&a, &a);

  realIcToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
}



void magnitudeRm16(void) {
  fnToBeCoded();
}



void magnitudeCm16(void) {
  fnToBeCoded();
}



void magnitudeShoI(void) {
  *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = WP34S_intAbs(*(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)));
}



void magnitudeRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function magnitudeRe34:", "cannot use NaN as X input of |x|", NULL, NULL);
    #endif
    return;
  }

  real34SetPositiveSign(REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



void magnitudeCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function magnitudeCo34:", "cannot use NaN as X input of |x|", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, b;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &a);
  real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_X), &b);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);

  realIcMultiply(&a, &a, &a);
  realIcFMA(&b, &b, &a, &a);
  realIcSquareRoot(&a, &a);

  realIcToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
}
