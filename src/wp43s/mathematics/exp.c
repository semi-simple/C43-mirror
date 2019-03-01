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



void (* const Exp[12])(void) = {
// regX ==> 1            2         3          4          5          6          7          8           9            10             11        12
//          Long integer real16    complex16  Date       Time       Date       String     real16 mat  complex16 m  Short integer  real34    complex34
            expLonI,     expRe16,  expCo16,   expError,  expError,  expError,  expError,  expRm16,    expCm16,     expShoI,       expRe34,  expCo34
};



/********************************************//**
 * \brief Data type error in exp
 *
 * \param void
 * \return void
 ***********************************************/
void expError(void) {
  displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate Exp for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnExp:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief Error message for a valid operation to be coded
 *
 * \param void
 * \return void
 ***********************************************/
void expToBeCoded(void) {
  #ifdef PC_BUILD
    sprintf(errorMessage, "exp(%s)", getRegisterDataTypeName(REGISTER_X, false, false));
    showInfoDialog("Operation to be coded:", errorMessage, NULL, NULL);
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
  if(Exp[getRegisterDataType(REGISTER_X)] != expError) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    result = REGISTER_X;
    opX    = allocateTemporaryRegister();
    copySourceRegisterToDestRegister(REGISTER_X, opX);

    Exp[getRegisterDataType(REGISTER_X)]();
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
    expError();
  }
}



void expLonI(void) {
  real34_t real34;

  convertLongIntegerRegisterToReal34Register(opX, opX);
  real34Exp(REGISTER_REAL34_DATA(opX), &real34);
  reallocateRegister(result, dtReal16, REAL16_SIZE, 0);
  real34ToReal16(&real34, REGISTER_REAL16_DATA(result));
}



void expRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function expRe16:", "cannot use NaN as an input of exp", NULL, NULL);
    #endif
    return;
  }

  real34_t real34;

  convertRegister16To34(opX);
  real34Exp(REGISTER_REAL34_DATA(opX), &real34);
  real34ToReal16(&real34, REGISTER_REAL16_DATA(result));
}



void expCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function expCo16:", "cannot use NaN as an input of exp", NULL, NULL);
    #endif
    return;
  }

  real34_t factor, real34, imag34;
  uint8_t savedAngularMode;

  convertRegister16To34(opX);
  real34Exp(REGISTER_REAL34_DATA(opX), &factor);
  savedAngularMode = angularMode;
  angularMode = AM_RADIAN;
  convertAngle34ToInternal(REGISTER_IMAG34_DATA(opX), AM_RADIAN);
  real34PolarToRectangular(const34_1, REGISTER_IMAG34_DATA(opX), &real34, &imag34); // X in internal units
  angularMode = savedAngularMode;
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
  real34Multiply(&factor, &real34, REGISTER_REAL34_DATA(result));
  real34Multiply(&factor, &imag34, REGISTER_IMAG34_DATA(result));
  convertRegister34To16(result);
}



void expRm16(void) {
  expToBeCoded();
}



void expCm16(void) {
  expToBeCoded();
}



void expShoI(void) {
  real34_t real34;

  convertShortIntegerRegisterToReal34Register(opX, opX);
  real34Exp(REGISTER_REAL34_DATA(opX), &real34);
  reallocateRegister(result, dtReal16, REAL16_SIZE, 0);
  real34ToReal16(&real34, REGISTER_REAL16_DATA(result));
}



void expRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function expRe34:", "cannot use NaN as an input of exp", NULL, NULL);
    #endif
    return;
  }

  real34Exp(REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



void expCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function expCo34:", "cannot use NaN as an input of exp", NULL, NULL);
    #endif
    return;
  }

  real34_t factor, real34, imag34;
  uint8_t savedAngularMode;

  real34Exp(REGISTER_REAL34_DATA(opX), &factor);
  savedAngularMode = angularMode;
  angularMode = AM_RADIAN;
  convertAngle34ToInternal(REGISTER_IMAG34_DATA(opX), AM_RADIAN);
  real34PolarToRectangular(const34_1, REGISTER_IMAG34_DATA(opX), &real34, &imag34); // X in internal units
  angularMode = savedAngularMode;
  real34Multiply(&factor, &real34, REGISTER_REAL34_DATA(result));
  real34Multiply(&factor, &imag34, REGISTER_IMAG34_DATA(result));
}
