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
            arcsinBigI,  arcsinRe16,  arcsinCo16,  errorArcsin,  errorArcsin,  errorArcsin,  errorArcsin,  arcsinRm16,     arcsinCm16,  errorArcsin,   arcsinRe34,  arcsinCo34
};



/********************************************//**
 * \brief Data type error in arcsin
 *
 * \param void
 * \return void
 ***********************************************/
void errorArcsin(void) {
  displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate arcsin for %s", getRegisterDataTypeName(op1, true, false));
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
    sprintf(errorMessage, "arcsin(%s)", getRegisterDataTypeName(op1, false, false));
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
  if(arcsin[getRegisterDataType(REGISTER_X)] != errorArcsin) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    result = REGISTER_X;
    op1    = allocateTemporaryRegister();
    copySourceRegisterToDestRegister(REGISTER_X, op1);

    arcsin[getRegisterDataType(REGISTER_X)]();
    freeTemporaryRegister(op1);

    if(lastErrorCode != 0) {
      restoreStack();
    }

    refreshStack();
  }
  else {
    errorArcsin();
  }
}



void arcsinBigI(void) {
 convertBigIntegerRegisterToReal16Register(op1, op1);
  if(real16CompareAbsGreaterThan(REGISTER_REAL16_DATA(op1), const16_1)) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function fnArcsin:", "|X| > 1", NULL, NULL);
    #endif
  }
  else {
    reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
    if(real16IsZero(REGISTER_REAL16_DATA(op1))) {
      real34Zero(REGISTER_REAL34_DATA(result));
    }
    else {
      real34Copy(const34_0_5, REGISTER_REAL34_DATA(result));
      if(real16IsNegative(REGISTER_REAL16_DATA(op1))) {
        real34ChangeSign(REGISTER_REAL34_DATA(result));
      }
    }
    convertAngle34FromTo(REGISTER_REAL34_DATA(result), AM_MULTPI, angularMode);
    temporaryInformation = TI_ANGLE;
    convertRegister34To16(result);
  }
}



void arcsinRe16(void) {
  if(real16CompareAbsGreaterThan(REGISTER_REAL16_DATA(op1), const16_1)) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function fnArcsin:", "|X| > 1", NULL, NULL);
    #endif
  }
  else {
    convertRegister16To34(op1);
    reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
    WP34S_do_asin(REGISTER_REAL34_DATA(op1), REGISTER_REAL34_DATA(result));
    convertRegisterAngleFromTo(REGISTER_X, AM_RADIAN, angularMode);
    convertRegister34To16(result);
    if(angularMode == AM_DMS) {
      checkDms16(REGISTER_REAL16_DATA(result));
    }
    temporaryInformation = TI_ANGLE;
  }
}



void arcsinCo16(void) {
  // arcsin(z) = -i.ln(iz + sqtr(1 - z*z))
  complex34_t iz;

  convertRegister16To34(op1);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);

  // calculate iz
  real34Copy(REGISTER_REAL34_DATA(op1), VARIABLE_IMAG34_DATA(iz));
  real34Copy(REGISTER_IMAG34_DATA(op1), VARIABLE_REAL34_DATA(iz));
  real34ChangeSign(VARIABLE_REAL34_DATA(iz));

  // calculate z*z
  op2 = allocateTemporaryRegister();
  reallocateRegister(op2, dtComplex34, COMPLEX34_SIZE, 0);
  complex34Copy(REGISTER_COMPLEX34_DATA(op1), REGISTER_COMPLEX34_DATA(op2));
  mulCo34Co34();

  // calculate 1 - z*z
  real34Copy(const34_1, REGISTER_REAL34_DATA(op1));
  real34Zero(REGISTER_IMAG34_DATA(op1));
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(op2));
  subCo34Co34();

  // calculate sqrt(1 - z*z)
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(op1));
  sqrtCo34();

  // calculate iz + sqrt(1 - z*z)
  complex34Copy(VARIABLE_COMPLEX34_DATA(iz), REGISTER_COMPLEX34_DATA(op1));
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(op2));
  addCo34Co34();
  freeTemporaryRegister(op2);

  // calculate ln(iz + sqtr(1 - z*z))
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(op1));
  lnCo34();

  // calculate = -i.ln(iz + sqtr(1 - z*z))
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(op1));
  real34Copy(REGISTER_REAL34_DATA(op1), REGISTER_IMAG34_DATA(result));
  real34Copy(REGISTER_IMAG34_DATA(op1), REGISTER_REAL34_DATA(result));
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
  if(real34CompareAbsGreaterThan(REGISTER_REAL34_DATA(op1), const34_1)) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function fnArcsin:", "|X| > 1", NULL, NULL);
    #endif
  }
  else {
    WP34S_do_asin(REGISTER_REAL34_DATA(op1), REGISTER_REAL34_DATA(result));
    convertRegisterAngleFromTo(REGISTER_X, AM_RADIAN, angularMode);
    convertRegister34To16(result);
    if(angularMode == AM_DMS) {
      checkDms16(REGISTER_REAL16_DATA(result));
    }
    temporaryInformation = TI_ANGLE;
  }
}



void arcsinCo34(void) {
  // arcsin(z) = -i.ln(iz + sqtr(1 - z*z))
  complex34_t iz;

  // calculate iz
  real34Copy(REGISTER_REAL34_DATA(op1), VARIABLE_IMAG34_DATA(iz));
  real34Copy(REGISTER_IMAG34_DATA(op1), VARIABLE_REAL34_DATA(iz));
  real34ChangeSign(VARIABLE_REAL34_DATA(iz));

  // calculate z*z
  op2 = allocateTemporaryRegister();
  reallocateRegister(op2, dtComplex34, COMPLEX34_SIZE, 0);
  complex34Copy(REGISTER_COMPLEX34_DATA(op1), REGISTER_COMPLEX34_DATA(op2));
  mulCo34Co34();

  // calculate 1 - z*z
  real34Copy(const34_1, REGISTER_REAL34_DATA(op1));
  real34Zero(REGISTER_IMAG34_DATA(op1));
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(op2));
  subCo34Co34();

  // calculate sqrt(1 - z*z)
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(op1));
  sqrtCo34();

  // calculate iz + sqrt(1 - z*z)
  complex34Copy(VARIABLE_COMPLEX34_DATA(iz), REGISTER_COMPLEX34_DATA(op1));
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(op2));
  addCo34Co34();
  freeTemporaryRegister(op2);

  // calculate ln(iz + sqtr(1 - z*z))
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(op1));
  lnCo34();

  // calculate = -i.ln(iz + sqtr(1 - z*z))
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(op1));
  real34Copy(REGISTER_REAL34_DATA(op1), REGISTER_IMAG34_DATA(result));
  real34Copy(REGISTER_IMAG34_DATA(op1), REGISTER_REAL34_DATA(result));
  real34ChangeSign(REGISTER_IMAG34_DATA(result));
}
