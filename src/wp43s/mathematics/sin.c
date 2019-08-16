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
 * \file sin.c
 ***********************************************/

#include "wp43s.h"



void (* const Sin[13])(void) = {
// regX ==> 1            2        3         4        5         6         7         8          9           10            11       12        13
//          Long integer Real16   Complex16 Angle16  Time      Date      String    Real16 mat Complex16 m Short integer Real34   Complex34 Angle16
            sinLonI,     sinRe16, sinCo16,  sinAn16, sinError, sinError, sinError, sinRm16,   sinCm16,    sinError,     sinRe34, sinCo34,  sinAn34
};



/********************************************//**
 * \brief Data type error in sin
 *
 * \param void
 * \return void
 ***********************************************/
void sinError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate Sin for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnSin:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and sin(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnSin(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  Sin[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



void sinLonI(void) {
  longIntegerAngleReduction(REGISTER_X, currentAngularMode);
  WP34S_cvt_2rad_sincos(REGISTER_REAL34_DATA(REGISTER_X), NULL, REGISTER_REAL34_DATA(REGISTER_X), currentAngularMode);
  convertRegister34To16(REGISTER_X);
}



void sinRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function sinRe16:", "cannot use NaN as X input of sin", NULL, NULL);
    #endif
    return;
  }

  if(real16IsInfinite(REGISTER_REAL16_DATA(REGISTER_X))) {
    real16Copy(const16_NaN, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    convertRegister16To34(REGISTER_X);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, TAG_NONE);
    WP34S_cvt_2rad_sincos(REGISTER_REAL34_DATA(REGISTER_X), NULL, REGISTER_REAL34_DATA(REGISTER_X), currentAngularMode);
    convertRegister34To16(REGISTER_X);
  }
}



void sinCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function sinCo16:", "cannot use NaN as X input of sin", NULL, NULL);
    #endif
    return;
  }

  // sin(z) = (exp(iz) - exp(-iz)) / 2i
  complex34_t iz, expIz;

  convertRegister16To34(REGISTER_X);

  // calculate iz
  real34Copy(REGISTER_REAL34_DATA(REGISTER_X), VARIABLE_IMAG34_DATA(&iz));
  real34Copy(REGISTER_IMAG34_DATA(REGISTER_X), VARIABLE_REAL34_DATA(&iz));
  real34ChangeSign(VARIABLE_REAL34_DATA(&iz));

  // calculate expIz = exp(iz)
  complex34Copy(VARIABLE_COMPLEX34_DATA(&iz), REGISTER_COMPLEX34_DATA(REGISTER_X));
  expCo34();
  complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_X), VARIABLE_COMPLEX34_DATA(&expIz));

  // calculate X = exp(-iz)
  complex34ChangeSign(VARIABLE_COMPLEX34_DATA(&iz));
  complex34Copy(VARIABLE_COMPLEX34_DATA(&iz), REGISTER_COMPLEX34_DATA(REGISTER_X));
  expCo34();

  // calculate X = exp(iz) - exp(-iz)
  real34Subtract(VARIABLE_REAL34_DATA(&expIz), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
  real34Subtract(VARIABLE_IMAG34_DATA(&expIz), REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X));

  // calculate X = (exp(iz) - exp(-iz)) / 2
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), const34_0_5, REGISTER_REAL34_DATA(REGISTER_X));
  real34Multiply(REGISTER_IMAG34_DATA(REGISTER_X), const34_0_5, REGISTER_IMAG34_DATA(REGISTER_X));

  // calculate X = (exp(iz) - exp(-iz)) / 2i
  real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &iz);
  real34ChangeSign(&iz);
  real34Copy(REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
  real34Copy(&iz, REGISTER_IMAG34_DATA(REGISTER_X));

  convertRegister34To16(REGISTER_X);
}



void sinAn16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function sinAn16:", "cannot use NaN as X input of sin", NULL, NULL);
    #endif
    return;
  }

  if(real16IsInfinite(REGISTER_REAL16_DATA(REGISTER_X))) {
    real16Copy(const16_NaN, REGISTER_REAL16_DATA(REGISTER_X));
  }
  else {
    convertRegister16To34(REGISTER_X);
    WP34S_cvt_2rad_sincos(REGISTER_REAL34_DATA(REGISTER_X), NULL, REGISTER_REAL34_DATA(REGISTER_X), getRegisterAngularMode(REGISTER_X));
    convertRegister34To16(REGISTER_X);
  }

  setRegisterDataType(REGISTER_X, dtReal16, TAG_NONE);
}



void sinRm16(void) {
  fnToBeCoded();
}



void sinCm16(void) {
  fnToBeCoded();
}



void sinRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function sinRe34:", "cannot use NaN as X input of sin", NULL, NULL);
    #endif
    return;
  }

  if(real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_X))) {
    real34Copy(const34_NaN, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    WP34S_cvt_2rad_sincos(REGISTER_REAL34_DATA(REGISTER_X), NULL, REGISTER_REAL34_DATA(REGISTER_X), currentAngularMode);
  }
}



void sinCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function sinCo34:", "cannot use NaN as X input of sin", NULL, NULL);
    #endif
    return;
  }

  // sin(z) = (exp(iz) - exp(-iz)) / 2i
  complex34_t iz, expIz;

  // calculate iz
  real34Copy(REGISTER_REAL34_DATA(REGISTER_X), VARIABLE_IMAG34_DATA(&iz));
  real34Copy(REGISTER_IMAG34_DATA(REGISTER_X), VARIABLE_REAL34_DATA(&iz));
  real34ChangeSign(VARIABLE_REAL34_DATA(&iz));

  // calculate expIz = exp(iz)
  complex34Copy(VARIABLE_COMPLEX34_DATA(&iz), REGISTER_COMPLEX34_DATA(REGISTER_X));
  expCo34();
  complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_X), VARIABLE_COMPLEX34_DATA(&expIz));

  // calculate X = exp(-iz)
  complex34ChangeSign(VARIABLE_COMPLEX34_DATA(&iz));
  complex34Copy(VARIABLE_COMPLEX34_DATA(&iz), REGISTER_COMPLEX34_DATA(REGISTER_X));
  expCo34();

  // calculate X = exp(iz) - exp(-iz)
  real34Subtract(VARIABLE_REAL34_DATA(&expIz), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
  real34Subtract(VARIABLE_IMAG34_DATA(&expIz), REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X));

  // calculate X = (exp(iz) - exp(-iz)) / 2
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), const34_0_5, REGISTER_REAL34_DATA(REGISTER_X));
  real34Multiply(REGISTER_IMAG34_DATA(REGISTER_X), const34_0_5, REGISTER_IMAG34_DATA(REGISTER_X));

  // calculate X = (exp(iz) - exp(-iz)) / 2i
  real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &iz);
  real34ChangeSign(&iz);
  real34Copy(REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
  real34Copy(&iz, REGISTER_IMAG34_DATA(REGISTER_X));
}



void sinAn34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function sinAn34:", "cannot use NaN as X input of sin", NULL, NULL);
    #endif
    return;
  }

  if(real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_X))) {
    real34Copy(const34_NaN, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    WP34S_cvt_2rad_sincos(REGISTER_REAL34_DATA(REGISTER_X), NULL, REGISTER_REAL34_DATA(REGISTER_X), getRegisterAngularMode(REGISTER_X));
  }

  setRegisterDataType(REGISTER_X, dtReal34, TAG_NONE);
}
