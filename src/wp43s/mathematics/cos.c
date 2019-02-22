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
 * \file cos.c
 ***********************************************/

#include "wp43s.h"



void (* const Cos[12])(void) = {
// regX ==> 1            2         3          4         5          6          7          8            9             10              11        12
//          Big integer  real16    complex16  angle     Time       Date       String     real16 mat   complex16 m   Small integer   real34    complex34
            cosBigI,     cosRe16,  cosCo16,   cosAngl,  cosError,  cosError,  cosError,  cosRm16,     cosCm16,      cosError,       cosRe34,  cosCo34
};



/********************************************//**
 * \brief Data type error in cos
 *
 * \param void
 * \return void
 ***********************************************/
void cosError(void) {
  displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate Cos for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnCos:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief Error message for a valid operation to be coded
 *
 * \param void
 * \return void
 ***********************************************/
void cosToBeCoded(void) {
  #ifdef PC_BUILD
    sprintf(errorMessage, "cos(%s)", getRegisterDataTypeName(REGISTER_X, false, false));
    showInfoDialog("Operation to be coded:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and cos(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnCos(uint16_t unusedParamButMandatory) {
  if(Cos[getRegisterDataType(REGISTER_X)] != cosError) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    result = REGISTER_X;
    opX    = allocateTemporaryRegister();
    copySourceRegisterToDestRegister(REGISTER_X, opX);

    Cos[getRegisterDataType(REGISTER_X)]();
    freeTemporaryRegister(opX);

    if(lastErrorCode != 0) {
      restoreStack();
      refreshStack();
    }
    else {
      refreshRegisterLine(REGISTER_X);
    }
  }
  else {
    cosError();
  }
}



void cosBigI(void) {
  convertBigIntegerRegisterToReal34Register(opX, opX);
  convertAngle34ToInternal(REGISTER_REAL34_DATA(opX), angularMode);
  reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
  WP34S_cvt_2rad_sincos(NULL, REGISTER_REAL34_DATA(result), REGISTER_REAL34_DATA(opX)); // opX in internal units
  convertRegister34To16(result);
}



void cosRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function cosRe16:", "cannot use NaN as an input of cos", NULL, NULL);
    #endif
    return;
  }

  if(real16IsSpecial(REGISTER_REAL16_DATA(opX))) {
    real16Copy(const16_NaN, REGISTER_REAL34_DATA(result));
  }
  else {
    convertRegister16To34(opX);
    convertAngle34ToInternal(REGISTER_REAL34_DATA(opX), angularMode);
    reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
    WP34S_cvt_2rad_sincos(NULL, REGISTER_REAL34_DATA(result), REGISTER_REAL34_DATA(opX)); // opX in internal units
    convertRegister34To16(result);
  }
}



void cosCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function cosCo16:", "cannot use NaN as an input of cos", NULL, NULL);
    #endif
    return;
  }

  // cos(z) = (exp(iz) + exp(-iz)) / 2
  complex34_t iz, expIz;

  convertRegister16To34(opX);
  convertRegister16To34(result);

  // calculate iz
  real34Copy(REGISTER_REAL34_DATA(opX), VARIABLE_IMAG34_DATA(&iz));
  real34Copy(REGISTER_IMAG34_DATA(opX), VARIABLE_REAL34_DATA(&iz));
  real34ChangeSign(VARIABLE_REAL34_DATA(&iz));

  // calculate exp(iz)
  complex34Copy(VARIABLE_COMPLEX34_DATA(&iz), REGISTER_COMPLEX34_DATA(opX));
  expCo34();
  complex34Copy(REGISTER_COMPLEX34_DATA(result), VARIABLE_COMPLEX34_DATA(&expIz));

  // calculate exp(-iz)
  complex34ChangeSign(VARIABLE_COMPLEX34_DATA(&iz));
  complex34Copy(VARIABLE_COMPLEX34_DATA(&iz), REGISTER_COMPLEX34_DATA(opX));
  expCo34();

  // calculate exp(iz) - exp(-iz)
  opY = allocateTemporaryRegister();
  reallocateRegister(opY, dtComplex34, COMPLEX34_SIZE, 0);
  complex34Copy(VARIABLE_COMPLEX34_DATA(&expIz), REGISTER_COMPLEX34_DATA(opY));
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(opX));
  addCo34Co34();

  // calculate (exp(iz) + exp(-iz)) / 2
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(opY));
  real34Copy(const34_2, REGISTER_REAL34_DATA(opX));
  real34Zero(REGISTER_IMAG34_DATA(opX));
  divCo34Co34();

  freeTemporaryRegister(opY);
  convertRegister34To16(result);
}



void cosAngl(void) {
  if(angleIsNaN(REGISTER_ANGLE_DATA(opX))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function cosAngl:", "cannot use NaN as an input of cos", NULL, NULL);
    #endif
    return;
  }

  #if (ANGLE16 == 1)
    convertRegister16To34(opX);
  #endif
  reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
  WP34S_cvt_2rad_sincos(NULL, REGISTER_REAL34_DATA(result), REGISTER_REAL34_DATA(opX)); // opX in internal units
  setRegisterDataType(result, dtReal34);
  convertRegister34To16(result);
}



void cosRm16(void) {
  cosToBeCoded();
}



void cosCm16(void) {
  cosToBeCoded();
}



void cosRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function cosRe34:", "cannot use NaN as an input of cos", NULL, NULL);
    #endif
    return;
  }

  if(real34IsSpecial(REGISTER_REAL34_DATA(opX))) {
    real34Copy(const34_NaN, REGISTER_REAL34_DATA(result));
  }
  else {
    convertAngle34ToInternal(REGISTER_REAL34_DATA(opX), angularMode);
    WP34S_cvt_2rad_sincos(NULL, REGISTER_REAL34_DATA(result), REGISTER_REAL34_DATA(opX)); // opX in internal units
  }
}



void cosCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function cosCo34:", "cannot use NaN as an input of cos", NULL, NULL);
    #endif
    return;
  }

  // cos(z) = (exp(iz) + exp(-iz)) / 2
  complex34_t iz, expIz;

  // calculate iz
  real34Copy(REGISTER_REAL34_DATA(opX), VARIABLE_IMAG34_DATA(&iz));
  real34Copy(REGISTER_IMAG34_DATA(opX), VARIABLE_REAL34_DATA(&iz));
  real34ChangeSign(VARIABLE_REAL34_DATA(&iz));

  // calculate exp(iz)
  complex34Copy(VARIABLE_COMPLEX34_DATA(&iz), REGISTER_COMPLEX34_DATA(opX));
  expCo34();
  complex34Copy(REGISTER_COMPLEX34_DATA(result), VARIABLE_COMPLEX34_DATA(&expIz));

  // calculate exp(-iz)
  complex34ChangeSign(VARIABLE_COMPLEX34_DATA(&iz));
  complex34Copy(VARIABLE_COMPLEX34_DATA(&iz), REGISTER_COMPLEX34_DATA(opX));
  expCo34();

  // calculate exp(iz) + exp(-iz)
  opY = allocateTemporaryRegister();
  reallocateRegister(opY, dtComplex34, COMPLEX34_SIZE, 0);
  complex34Copy(VARIABLE_COMPLEX34_DATA(&expIz), REGISTER_COMPLEX34_DATA(opY));
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(opX));
  addCo34Co34();

  // calculate (exp(iz) - exp(-iz)) / 2i
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(opY));
  real34Copy(const34_2, REGISTER_REAL34_DATA(opX));
  real34Zero(REGISTER_IMAG34_DATA(opX));
  divCo34Co34();

  freeTemporaryRegister(opY);
}
