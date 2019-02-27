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
 * \file imaginaryPart.c
 ***********************************************/

#include "wp43s.h"



void (* const imagPart[12])(void) = {
// regX ==> 1              2              3             4              5              6              7              8              9             10             11             12
//          Big integer    real16         complex16     Date           Time           Date           String         real16 mat     complex16 m   Small integer  real34         complex34
            imagPartError, imagPartError, imagPartCo16, imagPartError, imagPartError, imagPartError, imagPartError, imagPartError, imagPartCm16, imagPartError, imagPartError, imagPartCo34
};



/********************************************//**
 * \brief Data type error in Im
 *
 * \param void
 * \return void
 ***********************************************/
void imagPartError(void) {
  displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate Im for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnImaginaryPart:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief Error message for a valid operation to be coded
 *
 * \param void
 * \return void
 ***********************************************/
void imagPartToBeCoded(void) {
  #ifdef PC_BUILD
    sprintf(errorMessage, "Im(%s)", getRegisterDataTypeName(REGISTER_X, false, false));
    showInfoDialog("Operation to be coded:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and Im(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnImaginaryPart(uint16_t unusedParamButMandatory) {
  if(imagPart[getRegisterDataType(REGISTER_X)] != imagPartError) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    result = REGISTER_X;
    opX    = allocateTemporaryRegister();
    copySourceRegisterToDestRegister(REGISTER_X, opX);

    imagPart[getRegisterDataType(REGISTER_X)]();
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
    imagPartError();
  }
}



void imagPartCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function imagPartCo16:", "cannot use NaN as an input of Im", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtReal16, REAL16_SIZE, 0);
  real16Copy(REGISTER_IMAG16_DATA(opX), REGISTER_REAL16_DATA(result));
}



void imagPartCm16(void) {
  imagPartToBeCoded();
}


void imagPartCo34(void) {
  if(real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function imagPartCo34:", "cannot use NaN as an input of Im", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
  real34Copy(REGISTER_IMAG34_DATA(opX), REGISTER_REAL34_DATA(result));
}
