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
 * \file not.c
 ***********************************************/

#include "wp43s.h"



void (* const not[12])(void) = {
// regX ==> 1            2         3          4          5          6          7          8           9            10             11        12
//          Long integer  real16    complex16  angle      Time       Date       String     real16 mat  complex16 m  Short integer  real34    complex34
            notLonI,     notRe16,  notError,  notError,  notError,  notError,  notError,  notError,   notError,    notShoI,       notRe34,  notError
};



/********************************************//**
 * \brief Data type error in logical not
 *
 * \param void
 * \return void
 ***********************************************/
void notError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate not(%s)", getRegisterDataTypeName(REGISTER_X, false, false));
    showInfoDialog("In function notError:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and not(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnLogicalNot(uint16_t unusedParamButMandatory) {
  if(not[getRegisterDataType(REGISTER_X)] != notError) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    not[getRegisterDataType(REGISTER_X)]();

    if(lastErrorCode != 0) {
      restoreStack();
      refreshStack();
    }
    else {
      refreshRegisterLine(REGISTER_X);
    }
  }
  else {
    notError();
  }
}



void notLonI(void) {
  longInteger_t res;

  convertLongIntegerRegisterToLongInteger(REGISTER_X, res);

  if(longIntegerIsZero(res)) {
    uIntToLongInteger(1, res);
  }
  else {
    uIntToLongInteger(0, res);
  }

  convertLongIntegerToLongIntegerRegister(res, REGISTER_X);

  longIntegerFree(res);
}



void notRe16(void) {
  longInteger_t res;

  longIntegerInit(res);
  if(real16IsZero(REGISTER_REAL16_DATA(REGISTER_X))) {
    uIntToLongInteger(1, res);
  }

  convertLongIntegerToLongIntegerRegister(res, REGISTER_X);

  longIntegerFree(res);
}



void notShoI(void) {
  *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = ~(*(REGISTER_SHORT_INTEGER_DATA(REGISTER_X))) & shortIntegerMask;
}



void notRe34(void) {
  longInteger_t res;

  longIntegerInit(res);
  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    uIntToLongInteger(1, res);
  }

  convertLongIntegerToLongIntegerRegister(res, REGISTER_X);

  longIntegerFree(res);
}
