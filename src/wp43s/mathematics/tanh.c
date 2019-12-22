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
 * \file tanh.c
 ***********************************************/

#include "wp43s.h"



void (* const Tanh[12])(void) = {
// regX ==> 1             2         3          4          5          6          7          8           9            10             11        12
//          Long integer  Real16    Complex16  Angle16    Time       Date       String     Real16 mat  Complex16 m  Short integer  Real34    Complex34
            tanhLonI,     tanhRe16, tanhCo16,  tanhError, tanhError, tanhError, tanhError, tanhRm16,   tanhCm16,    tanhError,     tanhRe34, tanhCo34
};



/********************************************//**
 * \brief Data type error in tanh
 *
 * \param void
 * \return void
 ***********************************************/
void tanhError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate Tan for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnTanh:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and tanh(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnTanh(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  Tanh[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



void tanhLonI(void) {
  real39_t x;

  convertLongIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  WP34S_Tanh(&x, &x);

  reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);
  realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
}



void tanhRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function tanhRe16:", "cannot use NaN as an input of tanh", NULL, NULL);
    #endif
    return;
  }

  if(real16IsInfinite(REGISTER_REAL16_DATA(REGISTER_X)) && !getFlag(FLAG_DANGER)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function tanhRe16:", "cannot use " STD_PLUS_MINUS STD_INFINITY " as X input of tanh when flag D is not set", NULL, NULL);
    #endif
    return;
  }

  real39_t x;

  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &x);
  WP34S_Tanh(&x, &x);
  realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



void tanhCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function tanhCo16:", "cannot use NaN as an input of tanh", NULL, NULL);
    #endif
    return;
  }

  // tanh(a + i b) = (tanh(a) + i tan(b)) / (1 + i tanh(a) tan(b))
  real39_t a, b, sina, cosa;
  real39_t numerReal, denomReal;
  real39_t numerImag, denomImag;

  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &a);
  real16ToReal(REGISTER_IMAG16_DATA(REGISTER_X), &b);

  if(realIsZero(&b)) {
    WP34S_Tanh(&a, &numerReal);
    realZero(&numerImag);
  }
  else {
    WP34S_Tanh(&a, &numerReal);
    WP34S_Cvt2RadSinCosTan(&b, AM_RADIAN, &sina, &cosa, &numerImag);

    realCopy(const_1, &denomReal);
    realMultiply(&numerReal, &numerImag, &denomImag, &ctxtReal39);

    divCo39Co39(&numerReal, &numerImag, &denomReal, &denomImag, &numerReal, &numerImag);
  }

  realToReal16(&numerReal, REGISTER_REAL16_DATA(REGISTER_X));
  realToReal16(&numerImag, REGISTER_IMAG16_DATA(REGISTER_X));
}



void tanhRm16(void) {
  fnToBeCoded();
}



void tanhCm16(void) {
  fnToBeCoded();
}



void tanhRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function tanhRe34:", "cannot use NaN as an input of tanh", NULL, NULL);
    #endif
    return;
  }

  if(real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_X)) && !getFlag(FLAG_DANGER)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function tanhRe34:", "cannot use " STD_PLUS_MINUS STD_INFINITY " as X input of tanh when flag D is not set", NULL, NULL);
    #endif
    return;
  }

  real39_t x;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
  WP34S_Tanh(&x, &x);
  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



void tanhCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function tanhCo34:", "cannot use NaN as an input of tanh", NULL, NULL);
    #endif
    return;
  }

  // tanh(a + i b) = (tanh(a) + i tan(b)) / (1 + i tanh(a) tan(b))
  real39_t a, b, sina, cosa;
  real39_t numerReal, denomReal;
  real39_t numerImag, denomImag;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &a);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &b);

  if(realIsZero(&b)) {
    WP34S_Tanh(&a, &numerReal);
    realZero(&numerImag);
  }
  else {
    WP34S_Tanh(&a, &numerReal);
    WP34S_Cvt2RadSinCosTan(&b, AM_RADIAN, &sina, &cosa, &numerImag);

    realCopy(const_1, &denomReal);
    realMultiply(&numerReal, &numerImag, &denomImag, &ctxtReal39);

    divCo39Co39(&numerReal, &numerImag, &denomReal, &denomImag, &numerReal, &numerImag);
  }

  realToReal34(&numerReal, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&numerImag, REGISTER_IMAG34_DATA(REGISTER_X));
}
