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



void (* const Curt[12])(void) = {
// regX ==> 1            2         3         4          5          6          7          8          9           10            11        12
//          Long integer Real16    Complex16 Angle16    Time       Date       String     Real16 mat Complex16 m Short integer Real34    Complex34
            curtLonI,    curtRe16, curtCo16, curtError, curtError, curtError, curtError, curtRm16,  curtCm16,   curtShoI,     curtRe34, curtCo34
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
    real39_t x;

    convertLongIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
    reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);
    if(realIsPositive(&x)) {
      realPower(&x, const_1on3, &x, &ctxtReal39);
    }
    else {
      realSetPositiveSign(&x);
      realPower(&x, const_1on3, &x, &ctxtReal39);
      realSetNegativeSign(&x);
    }
    realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
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

  if(real16IsInfinite(REGISTER_REAL16_DATA(REGISTER_X)) && !getFlag(FLAG_DANGER)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function curtRe16:", "cannot use " STD_PLUS_MINUS STD_INFINITY " as X input of curt when flag D is not set", NULL, NULL);
    #endif
    return;
  }

  real39_t x;

  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &x);

  if(realIsPositive(&x)) {
    realPower(&x, const_1on3, &x, &ctxtReal39);
  }
  else {
    realSetPositiveSign(&x);
    realPower(&x, const_1on3, &x, &ctxtReal39);
    realSetNegativeSign(&x);
  }
  realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



void curtCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function curtCo16:", "cannot use NaN as X input of curt", NULL, NULL);
    #endif
    return;
  }

  real39_t a, b;

  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &a);
  real16ToReal(REGISTER_IMAG16_DATA(REGISTER_X), &b);

  real39RectangularToPolar(&a, &b, &a, &b);
  realPower(&a, const_1on3, &a, &ctxtReal39);
  realMultiply(&b, const_1on3, &b, &ctxtReal39);
  real39PolarToRectangular(&a, &b, &a, &b);

  realToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
  realToReal16(&b, REGISTER_IMAG16_DATA(REGISTER_X));
}



void curtRm16(void) {
  fnToBeCoded();
}



void curtCm16(void) {
  fnToBeCoded();
}



void curtShoI(void) {
  real39_t x;
  int32_t cubeRoot;

  convertShortIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);

  if(realIsPositive(&x)) {
    realPower(&x, const_1on3, &x, &ctxtReal39);
  }
  else {
    realSetPositiveSign(&x);
    realPower(&x, const_1on3, &x, &ctxtReal39);
    realSetNegativeSign(&x);
  }

  realToInt32(&x, cubeRoot);

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

  if(real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_X)) && !getFlag(FLAG_DANGER)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function curtRe34:", "cannot use " STD_PLUS_MINUS STD_INFINITY " as X input of curt when flag D is not set", NULL, NULL);
    #endif
    return;
  }

  real39_t x;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);

  if(realIsPositive(&x)) {
    realPower(&x, const_1on3, &x, &ctxtReal39);
  }
  else {
    realSetPositiveSign(&x);
    realPower(&x, const_1on3, &x, &ctxtReal39);
    realSetNegativeSign(&x);
  }
  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



void curtCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function curtCo34:", "cannot use NaN as X input of curt", NULL, NULL);
    #endif
    return;
  }

  real39_t a, b;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &a);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &b);

  real39RectangularToPolar(&a, &b, &a, &b);
  realPower(&a, const_1on3, &a, &ctxtReal39);
  realMultiply(&b, const_1on3, &b, &ctxtReal39);
  real39PolarToRectangular(&a, &b, &a, &b);

  realToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&b, REGISTER_IMAG34_DATA(REGISTER_X));
}
