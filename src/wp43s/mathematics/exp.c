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
//  regX ==> 1            2         3          4          5          6          7          8            9             10              11        12
//           Big integer  real16    complex16             Time       Date       String     real16 mat   complex16 m   Small integer   real34    complex34
             expBigI,     expRe16,  expCo16,   errorExp,  errorExp,  errorExp,  errorExp,  expRm16,     expCm16,      expSmaI,        expRe34,  expCo34
};



/********************************************//**
 * \brief Data type error in exp
 *
 * \param void
 * \return void
 ***********************************************/
void errorExp(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("errorExp");
  #endif

  displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate Exp for %s", getRegisterDataTypeName(op1, true, false));
    showInfoDialog("In function fnExp:", errorMessage, NULL, NULL);
  #endif

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("errorExp");
  #endif
}



/********************************************//**
 * \brief Error message for a valid operation to be coded
 *
 * \param void
 * \return void
 ***********************************************/
void expToBeCoded(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("expToBeCoded");
  #endif

  #ifdef PC_BUILD
    sprintf(errorMessage, "exp(%s)", getRegisterDataTypeName(op1, false, false));
    showInfoDialog("Operation to be coded:", errorMessage, NULL, NULL);
  #endif

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("expToBeCoded");
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
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnExp");
  #endif

  if(Exp[getRegisterDataType(REGISTER_X)] != errorExp) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    result = REGISTER_X;
    op1    = allocateTemporaryRegister();
    copySourceRegisterToDestRegister(REGISTER_X, op1);

    Exp[getRegisterDataType(REGISTER_X)]();
    freeTemporaryRegister(op1);

    refreshStack();
  }
  else {
    errorExp();
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnExp");
  #endif
}



void expBigI(void) {
  real34_t real34;

  convertBigIntegerRegisterToReal34Register(op1, op1);
  real34Exp(POINTER_TO_REGISTER_DATA(op1), &real34);
  reallocateRegister(result, dtReal16, REAL16_SIZE, 0);
  real34ToReal16(&real34, POINTER_TO_REGISTER_DATA(result));
}



void expRe16(void) {
  real34_t real34;

  convertRegister16To34(op1);
  real34Exp(POINTER_TO_REGISTER_DATA(op1), &real34);
  real34ToReal16(&real34, POINTER_TO_REGISTER_DATA(result));
}



void expCo16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("expCo16");
  #endif

  expToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("expCo16");
  #endif
}



void expRm16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("expRm16");
  #endif

  expToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("expRm16");
  #endif
}



void expCm16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("expCm16");
  #endif

  expToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("expCm16");
  #endif
}



void expSmaI(void) {
  real34_t real34;

  convertSmallIntegerRegisterToReal34Register(op1, op1);
  real34Exp(POINTER_TO_REGISTER_DATA(op1), &real34);
  reallocateRegister(result, dtReal16, REAL16_SIZE, 0);
  real34ToReal16(&real34, POINTER_TO_REGISTER_DATA(result));
}



void expRe34(void) {
  real34Exp(POINTER_TO_REGISTER_DATA(op1), POINTER_TO_REGISTER_DATA(result));
}



void expCo34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("expCo34");
  #endif

  expToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("expCo34");
  #endif
}
