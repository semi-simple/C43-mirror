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
 * \file cubeRoot.c
 ***********************************************/

#include "wp43s.h"



void (* const Curt[13])(void) = {
// regX ==> 1            2         3         4         5          6          7          8          9           10            11        12        13
//          Long integer Real16    Complex16 Angle16   Time       Date       String     Real16 mat Complex16 m Short integer Real34    Complex34 Angle34
            curtLonI,    curtRe16, curtCo16, curtAn16, curtError, curtError, curtError, curtRm16,  curtCm16,   curtShoI,     curtRe34, curtCo34, curtAn34
};



/********************************************//**
 * \brief Data type error in curt
 *
 * \param void
 * \return void
 ***********************************************/
void curtError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate curt for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnCubeRoot:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and curt(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnCubeRoot(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  Curt[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



void curtLonI(void) {
  longInteger_t value;

  convertLongIntegerRegisterToLongInteger(REGISTER_X, value);

  if(longIntegerRoot(value, 3, value)) {
    convertLongIntegerToLongIntegerRegister(value, REGISTER_X);
  }
  else {
    realIc_t a;

    convertLongIntegerRegisterToRealIc(REGISTER_X, &a);
    realIcPower(&a, const_1on3, &a);
    reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, TAG_NONE);
    realIcToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
  }

  longIntegerFree(value);
}



void curtRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function curtRe16:", "cannot use NaN as X input of curt", NULL, NULL);
    #endif
    return;
  }

  realIc_t a;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &a);

  if(realIcIsPositive(&a)) {
    realIcPower(&a, const_1on3, &a);
  }
  else {
    realIcSetPositiveSign(&a);
    realIcPower(&a, const_1on3, &a);
    realIcSetNegativeSign(&a);
  }
  realIcToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
}



void curtCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function curtCo16:", "cannot use NaN as X input of curt", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, b;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &a);
  real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_X), &b);

  realIcRectangularToPolar(&a, &b, &a, &b);
  realIcPower(&a, const_1on3, &a);
  realIcMultiply(&b, const_1on3, &b);
  realIcPolarToRectangular(&a, &b, &a, &b);

  realIcToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
  realIcToReal16(&b, REGISTER_IMAG16_DATA(REGISTER_X));
}



void curtAn16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function curtAn16:", "cannot use NaN as X input of curt", NULL, NULL);
    #endif
    return;
  }

  realIc_t a;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &a);
  setRegisterDataType(REGISTER_X, dtReal16, TAG_NONE);

  if(realIcIsPositive(&a)) {
    realIcPower(&a, const_1on3, &a);
  }
  else {
    realIcSetPositiveSign(&a);
    realIcPower(&a, const_1on3, &a);
    realIcSetNegativeSign(&a);
  }
  realIcToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
}



void curtRm16(void) {
  fnToBeCoded();
}



void curtCm16(void) {
  fnToBeCoded();
}



void curtShoI(void) {
  realIc_t a;
  int32_t cubeRoot;

  convertShortIntegerRegisterToRealIc(REGISTER_X, &a);

  if(realIcIsPositive(&a)) {
    realIcPower(&a, const_1on3, &a);
  }
  else {
    realIcSetPositiveSign(&a);
    realIcPower(&a, const_1on3, &a);
    realIcSetNegativeSign(&a);
  }

  realIcToIntegralValue(&a, &a); // TODO: doesn't work without this line
  realIcToInt32(&a, cubeRoot);

  if(cubeRoot >= 0) {
    *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = WP34S_build_value((int64_t)cubeRoot, 0);
  }
  else {
    *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = WP34S_build_value(-(int64_t)cubeRoot, 1);
  }
}



void curtRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function curtRe34:", "cannot use NaN as X input of curt", NULL, NULL);
    #endif
    return;
  }

  realIc_t a;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &a);

  if(realIcIsPositive(&a)) {
    realIcPower(&a, const_1on3, &a);
  }
  else {
    realIcSetPositiveSign(&a);
    realIcPower(&a, const_1on3, &a);
    realIcSetNegativeSign(&a);
  }
  realIcToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
}



void curtCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function curtCo34:", "cannot use NaN as X input of curt", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, b;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &a);
  real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_X), &b);

  realIcRectangularToPolar(&a, &b, &a, &b);
  realIcPower(&a, const_1on3, &a);
  realIcMultiply(&b, const_1on3, &b);
  realIcPolarToRectangular(&a, &b, &a, &b);

  realIcToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
  realIcToReal34(&b, REGISTER_IMAG34_DATA(REGISTER_X));
}



void curtAn34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function curtAn34:", "cannot use NaN as X input of curt", NULL, NULL);
    #endif
    return;
  }

  realIc_t a;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &a);
  setRegisterDataType(REGISTER_X, dtReal34, TAG_NONE);

  if(realIcIsPositive(&a)) {
    realIcPower(&a, const_1on3, &a);
  }
  else {
    realIcSetPositiveSign(&a);
    realIcPower(&a, const_1on3, &a);
    realIcSetNegativeSign(&a);
  }
  realIcToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
}