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
 * \file tan.c
 ***********************************************/

#include "wp43s.h"



void (* const Tan[12])(void) = {
//  regX ==> 1            2         3          4          5          6          7          8            9             10              11        12
//           Big integer  real16    complex16             Time       Date       String     real16 mat   complex16 m   Small integer   real34    complex34
             tanBigI,     tanRe16,  tanCo16,   errorTan,  errorTan,  errorTan,  errorTan,  tanRm16,     tanCm16,      errorTan,       tanRe34,  tanCo34
};



/********************************************//**
 * \brief Data type error in tan
 *
 * \param void
 * \return void
 ***********************************************/
void errorTan(void) {
  displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate Tan for %s", getRegisterDataTypeName(op1, true, false));
    showInfoDialog("In function fnTan:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief Error message for a valid operation to be coded
 *
 * \param void
 * \return void
 ***********************************************/
void tanToBeCoded(void) {
  #ifdef PC_BUILD
    sprintf(errorMessage, "tan(%s)", getRegisterDataTypeName(op1, false, false));
    showInfoDialog("Operation to be coded:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and tan(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnTan(uint16_t unusedParamButMandatory) {
  if(Tan[getRegisterDataType(REGISTER_X)] != errorTan) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    result = REGISTER_X;
    op1    = allocateTemporaryRegister();
    copySourceRegisterToDestRegister(REGISTER_X, op1);

    Tan[getRegisterDataType(REGISTER_X)]();
    freeTemporaryRegister(op1);

    refreshStack();
  }
  else {
    errorTan();
  }
}



void tanBigI(void) {
  real34_t cos;

  convertBigIntegerRegisterToReal34Register(op1, op1);
  reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
  WP34S_cvt_2rad_sincos(REAL34_POINTER(POINTER_TO_REGISTER_DATA(result)), &cos, op1);
	 real34Divide(REGISTER_REAL34_DATA(REGISTER_X), &cos, REGISTER_REAL34_DATA(REGISTER_X));
  convertRegister34To16(result);
}



void tanRe16(void) {
  if(real16IsSpecial(POINTER_TO_REGISTER_DATA(op1))) {
    real16Copy(const16_NaN, POINTER_TO_REGISTER_DATA(result));
  }
  else {
    real34_t cos;

    convertRegister16To34(op1);
    reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
    WP34S_cvt_2rad_sincos(REAL34_POINTER(POINTER_TO_REGISTER_DATA(result)), &cos, op1);
  	 real34Divide(REGISTER_REAL34_DATA(REGISTER_X), &cos, REGISTER_REAL34_DATA(REGISTER_X));
    convertRegister34To16(result);
  }
}



void tanCo16(void) {
  // tan(z) = -i(exp(iz) - exp(-iz)) / (exp(iz) + exp(-iz))
  complex34_t iz, expIz, expMIz, numer;

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
  complex34Copy(REGISTER_COMPLEX34_DATA(result), VARIABLE_COMPLEX34_DATA(expMIz));

  // calculate exp(iz) - exp(-iz)
  op2 = allocateTemporaryRegister();
  reallocateRegister(op2, dtComplex34, COMPLEX34_SIZE, 0);
  complex34Copy(VARIABLE_COMPLEX34_DATA(expIz),  REGISTER_COMPLEX34_DATA(op1));
  complex34Copy(VARIABLE_COMPLEX34_DATA(expMIz), REGISTER_COMPLEX34_DATA(op2));
  subCo34Co34();
  complex34Copy(REGISTER_COMPLEX34_DATA(result), VARIABLE_COMPLEX34_DATA(numer));

  // calculate exp(iz) + exp(-iz)
  addCo34Co34();

  // calculate (exp(iz) - exp(-iz)) / (exp(iz) + exp(-iz))
  complex34Copy(VARIABLE_COMPLEX34_DATA(numer),  REGISTER_COMPLEX34_DATA(op1));
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(op2));
  divCo34Co34();

  // calculate -i(exp(iz) - exp(-iz)) / (exp(iz) + exp(-iz))
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(op1));
  real34Copy(REGISTER_REAL34_DATA(op1), REGISTER_IMAG34_DATA(result));
  real34Copy(REGISTER_IMAG34_DATA(op1), REGISTER_REAL34_DATA(result));
  real34ChangeSign(REGISTER_IMAG34_DATA(result));

  freeTemporaryRegister(op2);
  convertRegister34To16(result);
}



void tanRm16(void) {
  tanToBeCoded();
}



void tanCm16(void) {
  tanToBeCoded();
}



void tanRe34(void) {
  if(real34IsSpecial(POINTER_TO_REGISTER_DATA(op1))) {
    real34Copy(const34_NaN, POINTER_TO_REGISTER_DATA(result));
  }
  else {
    real34_t cos;

    WP34S_cvt_2rad_sincos(REAL34_POINTER(POINTER_TO_REGISTER_DATA(result)), &cos, op1);
  	 real34Divide(REGISTER_REAL34_DATA(REGISTER_X), &cos, REGISTER_REAL34_DATA(REGISTER_X));
  }
}



void tanCo34(void) {
  // tan(z) = -i(exp(iz) - exp(-iz)) / (exp(iz) + exp(-iz))
  complex34_t iz, expIz, expMIz, numer;

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
  complex34Copy(REGISTER_COMPLEX34_DATA(result), VARIABLE_COMPLEX34_DATA(expMIz));

  // calculate exp(iz) - exp(-iz)
  op2 = allocateTemporaryRegister();
  reallocateRegister(op2, dtComplex34, COMPLEX34_SIZE, 0);
  complex34Copy(VARIABLE_COMPLEX34_DATA(expIz),  REGISTER_COMPLEX34_DATA(op1));
  complex34Copy(VARIABLE_COMPLEX34_DATA(expMIz), REGISTER_COMPLEX34_DATA(op2));
  subCo34Co34();
  complex34Copy(REGISTER_COMPLEX34_DATA(result), VARIABLE_COMPLEX34_DATA(numer));

  // calculate exp(iz) + exp(-iz)
  addCo34Co34();

  // calculate (exp(iz) - exp(-iz)) / (exp(iz) + exp(-iz))
  complex34Copy(VARIABLE_COMPLEX34_DATA(numer),  REGISTER_COMPLEX34_DATA(op1));
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(op2));
  divCo34Co34();

  // calculate -i(exp(iz) - exp(-iz)) / (exp(iz) + exp(-iz))
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(op1));
  real34Copy(REGISTER_REAL34_DATA(op1), REGISTER_IMAG34_DATA(result));
  real34Copy(REGISTER_IMAG34_DATA(op1), REGISTER_REAL34_DATA(result));
  real34ChangeSign(REGISTER_IMAG34_DATA(result));

  freeTemporaryRegister(op2);
}
