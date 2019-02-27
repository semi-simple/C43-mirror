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
 * \file arcsin.c
 ***********************************************/

#include "wp43s.h"



void (* const arcsin[12])(void) = {
// regX ==> 1            2            3            4             5             6             7             8               9            10             11           12
//          Big integer  real16       complex16    angle         Time          Date          String        real16 mat      complex16 m  Small integer  real34       complex34
            arcsinBigI,  arcsinRe16,  arcsinCo16,  arcsinError,  arcsinError,  arcsinError,  arcsinError,  arcsinRm16,     arcsinCm16,  arcsinError,   arcsinRe34,  arcsinCo34
};



/********************************************//**
 * \brief Data type error in arcsin
 *
 * \param void
 * \return void
 ***********************************************/
void arcsinError(void) {
  displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate arcsin for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnArcsin:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief Error message for a valid operation to be coded
 *
 * \param void
 * \return void
 ***********************************************/
void arcsinToBeCoded(void) {
  #ifdef PC_BUILD
    sprintf(errorMessage, "arcsin(%s)", getRegisterDataTypeName(REGISTER_X, false, false));
    showInfoDialog("Operation to be coded:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and arcsin(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnArcsin(uint16_t unusedParamButMandatory) {
  if(arcsin[getRegisterDataType(REGISTER_X)] != arcsinError) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    result = REGISTER_X;
    opX    = allocateTemporaryRegister();
    copySourceRegisterToDestRegister(REGISTER_X, opX);

    arcsin[getRegisterDataType(REGISTER_X)]();
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
    arcsinError();
  }
}



void arcsinBigI(void) {
 convertBigIntegerRegisterToReal34Register(opX, opX);
  if(real34CompareAbsGreaterThan(REGISTER_REAL34_DATA(opX), const34_1)) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function fnArcsin:", "|X| > 1", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
  if(real34IsZero(REGISTER_REAL34_DATA(opX))) {
    real34Zero(REGISTER_REAL34_DATA(result));
  }
  else {
    real34Copy(const34_0_5, REGISTER_REAL34_DATA(result));
    if(real34IsNegative(REGISTER_REAL34_DATA(opX))) {
      real34ChangeSign(REGISTER_REAL34_DATA(result));
    }
  }
  convertAngle34ToInternal(REGISTER_REAL34_DATA(result), AM_MULTPI);
  #if (ANGLE16 == 1)
    convertRegister34To16(result);
  #endif
  setRegisterDataType(result, dtAngle);
  setRegisterAngularMode(result, angularMode);
}



void arcsinRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function arcsinRe16:", "cannot use NaN as an input of arcsin", NULL, NULL);
    #endif
    return;
  }

  if(real16CompareAbsGreaterThan(REGISTER_REAL16_DATA(opX), const16_1)) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function fnArcsin:", "|X| > 1", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opX);
  reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
  WP34S_do_asin(REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  convertAngle34ToInternal(REGISTER_REAL34_DATA(result), AM_RADIAN);
  #if (ANGLE16 == 1)
    convertRegister34To16(result);
  #endif
  setRegisterDataType(result, dtAngle);
  setRegisterAngularMode(result, angularMode);
}



void arcsinCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function arcsinCo16:", "cannot use NaN as an input of arcsin", NULL, NULL);
    #endif
    return;
  }

  // arcsin(z) = -i.ln(iz + sqtr(1 - z*z))
  complex34_t iz;

  convertRegister16To34(opX);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);

  // calculate iz
  real34Copy(REGISTER_REAL34_DATA(opX), VARIABLE_IMAG34_DATA(&iz));
  real34Copy(REGISTER_IMAG34_DATA(opX), VARIABLE_REAL34_DATA(&iz));
  real34ChangeSign(VARIABLE_REAL34_DATA(&iz));

  // calculate z*z
  opY = allocateTemporaryRegister();
  reallocateRegister(opY, dtComplex34, COMPLEX34_SIZE, 0);
  complex34Copy(REGISTER_COMPLEX34_DATA(opX), REGISTER_COMPLEX34_DATA(opY));
  mulCo34Co34();

  // calculate 1 - z*z
  real34Copy(const34_1, REGISTER_REAL34_DATA(opY));
  real34Zero(REGISTER_IMAG34_DATA(opY));
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(opX));
  subCo34Co34();

  // calculate sqrt(1 - z*z)
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(opX));
  sqrtCo34();

  // calculate iz + sqrt(1 - z*z)
  complex34Copy(VARIABLE_COMPLEX34_DATA(&iz), REGISTER_COMPLEX34_DATA(opY));
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(opX));
  addCo34Co34();
  freeTemporaryRegister(opY);

  // calculate ln(iz + sqtr(1 - z*z))
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(opX));
  lnCo34();

  // calculate = -i.ln(iz + sqtr(1 - z*z))
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(opX));
  real34Copy(REGISTER_REAL34_DATA(opX), REGISTER_IMAG34_DATA(result));
  real34Copy(REGISTER_IMAG34_DATA(opX), REGISTER_REAL34_DATA(result));
  real34ChangeSign(REGISTER_IMAG34_DATA(result));

  convertRegister34To16(result);
}



void arcsinRm16(void) {
  arcsinToBeCoded();
}



void arcsinCm16(void) {
  arcsinToBeCoded();
}



void arcsinRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function arcsinRe34:", "cannot use NaN as an input of arcsin", NULL, NULL);
    #endif
    return;
  }

  if(real34CompareAbsGreaterThan(REGISTER_REAL34_DATA(opX), const34_1)) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function fnArcsin:", "|X| > 1", NULL, NULL);
    #endif
    return;
  }

  WP34S_do_asin(REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  convertAngle34ToInternal(REGISTER_REAL34_DATA(result), AM_RADIAN);
  #if (ANGLE16 == 1)
    convertRegister34To16(result);
  #endif
  setRegisterDataType(result, dtAngle);
  setRegisterAngularMode(result, angularMode);
}



void arcsinCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function arcsinCo34:", "cannot use NaN as an input of arcsin", NULL, NULL);
    #endif
    return;
  }

  // arcsin(z) = -i.ln(iz + sqtr(1 - z*z))
  complex34_t iz;

  // calculate iz
  real34Copy(REGISTER_REAL34_DATA(opX), VARIABLE_IMAG34_DATA(&iz));
  real34Copy(REGISTER_IMAG34_DATA(opX), VARIABLE_REAL34_DATA(&iz));
  real34ChangeSign(VARIABLE_REAL34_DATA(&iz));

  // calculate z*z
  opY = allocateTemporaryRegister();
  reallocateRegister(opY, dtComplex34, COMPLEX34_SIZE, 0);
  complex34Copy(REGISTER_COMPLEX34_DATA(opX), REGISTER_COMPLEX34_DATA(opY));
  mulCo34Co34();

  // calculate 1 - z*z
  real34Copy(const34_1, REGISTER_REAL34_DATA(opY));
  real34Zero(REGISTER_IMAG34_DATA(opY));
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(opX));
  subCo34Co34();

  // calculate sqrt(1 - z*z)
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(opX));
  sqrtCo34();

  // calculate iz + sqrt(1 - z*z)
  complex34Copy(VARIABLE_COMPLEX34_DATA(&iz), REGISTER_COMPLEX34_DATA(opY));
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(opX));
  addCo34Co34();
  freeTemporaryRegister(opY);

  // calculate ln(iz + sqtr(1 - z*z))
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(opX));
  lnCo34();

  // calculate = -i.ln(iz + sqtr(1 - z*z))
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(opX));
  real34Copy(REGISTER_REAL34_DATA(opX), REGISTER_IMAG34_DATA(result));
  real34Copy(REGISTER_IMAG34_DATA(opX), REGISTER_REAL34_DATA(result));
  real34ChangeSign(REGISTER_IMAG34_DATA(result));
}
