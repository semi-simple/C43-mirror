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
 * \file realPart.c
 ***********************************************/

#include "wp43s.h"



void (* const realPart[12])(void) = {
// regX ==> 1              2              3             4              5              6              7              8              9             10             11             12
//          Big integer    real16         complex16     Date           Time           Date           String         real16 mat     complex16 m   Small integer  real34         complex34
            realPartError, realPartError, realPartCo16, realPartError, realPartError, realPartError, realPartError, realPartError, realPartCm16, realPartError, realPartError, realPartCo34
};



/********************************************//**
 * \brief Data type error in Re
 *
 * \param void
 * \return void
 ***********************************************/
void realPartError(void) {
  displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate Re for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnRealPart:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief Error message for a valid operation to be coded
 *
 * \param void
 * \return void
 ***********************************************/
void realPartToBeCoded(void) {
  #ifdef PC_BUILD
    sprintf(errorMessage, "Re(%s)", getRegisterDataTypeName(REGISTER_X, false, false));
    showInfoDialog("Operation to be coded:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and Re(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnRealPart(uint16_t unusedParamButMandatory) {
  if(realPart[getRegisterDataType(REGISTER_X)] != realPartError) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    result = REGISTER_X;
    opX    = allocateTemporaryRegister();
    copySourceRegisterToDestRegister(REGISTER_X, opX);

    realPart[getRegisterDataType(REGISTER_X)]();
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
    realPartError();
  }
}



void realPartCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function realPartCo16:", "cannot use NaN as an input of Re", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtReal16, REAL16_SIZE, 0);
  real16Copy(REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));
}



void realPartCm16(void) {
  realPartToBeCoded();
}


void realPartCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function realPartCo34:", "cannot use NaN as an input of Re", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
  real34Copy(REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}
