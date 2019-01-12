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
//  regX ==> 1            2         3          4          5          6          7          8            9             10              11        12
//           Big integer  real16    complex16             Time       Date       String     real16 mat   complex16 m   Small integer   real34    complex34
             cosBigI,     cosRe16,  cosCo16,   errorCos,  errorCos,  errorCos,  errorCos,  cosRm16,     cosCm16,      errorCos,       cosRe34,  cosCo34
};



/********************************************//**
 * \brief Data type error in cos
 *
 * \param void
 * \return void
 ***********************************************/
void errorCos(void) {
  displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate Cos for %s", getRegisterDataTypeName(op1, true, false));
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
    sprintf(errorMessage, "cos(%s)", getRegisterDataTypeName(op1, false, false));
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
  if(Cos[getRegisterDataType(REGISTER_X)] != errorCos) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    result = REGISTER_X;
    op1    = allocateTemporaryRegister();
    copySourceRegisterToDestRegister(REGISTER_X, op1);

    Cos[getRegisterDataType(REGISTER_X)]();
    freeTemporaryRegister(op1);

    refreshStack();
  }
  else {
    errorCos();
  }
}



void cosBigI(void) {
  convertBigIntegerRegisterToReal34Register(op1, op1);
  reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
  WP34S_cvt_2rad_sincos(NULL, REAL34_POINTER(POINTER_TO_REGISTER_DATA(result)), op1);
  convertRegister34To16(result);
}



void cosRe16(void) {
  if(real16IsSpecial(POINTER_TO_REGISTER_DATA(op1))) {
    real16Copy(const16_NaN, POINTER_TO_REGISTER_DATA(result));
  }
  else {
    convertRegister16To34(op1);
    reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
    WP34S_cvt_2rad_sincos(NULL, REAL34_POINTER(POINTER_TO_REGISTER_DATA(result)), op1);
    convertRegister34To16(result);
  }
}



void cosCo16(void) {
  // cos(z) = (exp(iz) + exp(-iz)) / 2
  complex34_t iz, expIz;

  convertRegister16To34(op1);
  convertRegister16To34(result);

  // calculate iz
  real34Copy(REGISTER_REAL34_DATA(op1), VARIABLE_IMAG34_DATA(iz));
  real34Copy(REGISTER_IMAG34_DATA(op1), VARIABLE_REAL34_DATA(iz));
  real34ChangeSign(VARIABLE_REAL34_DATA(iz));

  // calculate exp(iz)
  complex34Copy(VARIABLE_COMPLEX34_DATA(iz), REGISTER_COMPLEX34_DATA(op1));
  expCo34();
  complex34Copy(REGISTER_COMPLEX34_DATA(result), VARIABLE_COMPLEX34_DATA(expIz));

  // calculate exp(-iz)
  complex34ChangeSign(VARIABLE_COMPLEX34_DATA(iz));
  complex34Copy(VARIABLE_COMPLEX34_DATA(iz), REGISTER_COMPLEX34_DATA(op1));
  expCo34();

  // calculate exp(iz) - exp(-iz)
  op2 = allocateTemporaryRegister();
  reallocateRegister(op2, dtComplex34, COMPLEX34_SIZE, 0);
  complex34Copy(VARIABLE_COMPLEX34_DATA(expIz),  REGISTER_COMPLEX34_DATA(op1));
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(op2));
  addCo34Co34();

  // calculate (exp(iz) + exp(-iz)) / 2
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(op1));
  real34Copy(const34_2, REGISTER_REAL34_DATA(op2));
  real34Zero(REGISTER_IMAG34_DATA(op2));
  divCo34Co34();

  freeTemporaryRegister(op2);
  convertRegister34To16(result);
}



void cosRm16(void) {
  cosToBeCoded();
}



void cosCm16(void) {
  cosToBeCoded();
}



void cosRe34(void) {
  if(real34IsSpecial(POINTER_TO_REGISTER_DATA(op1))) {
    real34Copy(const34_NaN, POINTER_TO_REGISTER_DATA(result));
  }
  else {
    WP34S_cvt_2rad_sincos(NULL, REAL34_POINTER(POINTER_TO_REGISTER_DATA(result)), op1);
  }
}



void cosCo34(void) {
  // cos(z) = (exp(iz) + exp(-iz)) / 2
  complex34_t iz, expIz;

  // calculate iz
  real34Copy(REGISTER_REAL34_DATA(op1), VARIABLE_IMAG34_DATA(iz));
  real34Copy(REGISTER_IMAG34_DATA(op1), VARIABLE_REAL34_DATA(iz));
  real34ChangeSign(VARIABLE_REAL34_DATA(iz));

  // calculate exp(iz)
  complex34Copy(VARIABLE_COMPLEX34_DATA(iz), REGISTER_COMPLEX34_DATA(op1));
  expCo34();
  complex34Copy(REGISTER_COMPLEX34_DATA(result), VARIABLE_COMPLEX34_DATA(expIz));

  // calculate exp(-iz)
  complex34ChangeSign(VARIABLE_COMPLEX34_DATA(iz));
  complex34Copy(VARIABLE_COMPLEX34_DATA(iz), REGISTER_COMPLEX34_DATA(op1));
  expCo34();

  // calculate exp(iz) + exp(-iz)
  op2 = allocateTemporaryRegister();
  reallocateRegister(op2, dtComplex34, COMPLEX34_SIZE, 0);
  complex34Copy(VARIABLE_COMPLEX34_DATA(expIz),  REGISTER_COMPLEX34_DATA(op1));
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(op2));
  addCo34Co34();

  // calculate (exp(iz) - exp(-iz)) / 2i
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(op1));
  real34Copy(const34_2, REGISTER_REAL34_DATA(op2));
  real34Zero(REGISTER_IMAG34_DATA(op2));
  divCo34Co34();

  freeTemporaryRegister(op2);
}
