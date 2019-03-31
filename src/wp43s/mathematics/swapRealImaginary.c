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
 * \file swapRealImaginary.c
 ***********************************************/

#include "wp43s.h"



void (* const swapReIm[12])(void) = {
// regX ==> 1              2              3             4              5              6              7              8              9             10             11             12
//          Long integer   real16         complex16     Date           Time           Date           String         real16 mat     complex16 m   Short integer  real34         complex34
            swapReImError, swapReImError, swapReImCo16, swapReImError, swapReImError, swapReImError, swapReImError, swapReImError, swapReImCm16, swapReImError, swapReImError, swapReImCo34
};



/********************************************//**
 * \brief Data type error in exp
 *
 * \param void
 * \return void
 ***********************************************/
void swapReImError(void) {
  displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate Re" STD_LEFT_RIGHT_ARROWS "Im for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnSwapRealImaginary:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and Re<>IM(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnSwapRealImaginary(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  result = REGISTER_X;
  opX    = allocateTemporaryRegister();
  copySourceRegisterToDestRegister(REGISTER_X, opX);

  swapReIm[getRegisterDataType(REGISTER_X)]();
  freeTemporaryRegister(opX);

  if(lastErrorCode == 0) {
    refreshRegisterLine(REGISTER_X);
  }
  else {
    restoreStack();
    refreshStack();
  }
}



void swapReImCo16(void) {
  real16Copy(REGISTER_IMAG16_DATA(opX), REGISTER_REAL16_DATA(result));
  real16Copy(REGISTER_REAL16_DATA(opX), REGISTER_IMAG16_DATA(result));
}



void swapReImCm16(void) {
  fnToBeCoded();
}


void swapReImCo34(void) {
  real34Copy(REGISTER_IMAG34_DATA(opX), REGISTER_REAL34_DATA(result));
  real34Copy(REGISTER_REAL34_DATA(opX), REGISTER_IMAG34_DATA(result));
}
