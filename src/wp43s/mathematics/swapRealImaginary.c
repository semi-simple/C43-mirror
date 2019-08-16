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



void (* const swapReIm[13])(void) = {
// regX ==> 1              2              3             4              5              6              7              8              9             10             11             12            13
//          Long integer   Real16         Complex16     Angle16        Time           Date           String         Real16 mat     Complex16 m   Short integer  Real34         Complex34     Angle34
            swapReImError, swapReImError, swapReImCo16, swapReImError, swapReImError, swapReImError, swapReImError, swapReImError, swapReImCm16, swapReImError, swapReImError, swapReImCo34, swapReImError
};



/********************************************//**
 * \brief Data type error in exp
 *
 * \param void
 * \return void
 ***********************************************/
void swapReImError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
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

  swapReIm[getRegisterDataType(REGISTER_X)]();

  if(lastErrorCode == 0) {
    refreshRegisterLine(REGISTER_X);
  }
  else {
    restoreStack();
    refreshStack();
  }
}



void swapReImCo16(void) {
  real16_t temp;

  real16Copy(REGISTER_IMAG16_DATA(REGISTER_X), &temp);
  real16Copy(REGISTER_REAL16_DATA(REGISTER_X), REGISTER_IMAG16_DATA(REGISTER_X));
  real16Copy(&temp,                            REGISTER_REAL16_DATA(REGISTER_X));
}



void swapReImCm16(void) {
  fnToBeCoded();
}


void swapReImCo34(void) {
  real34_t temp;

  real34Copy(REGISTER_IMAG34_DATA(REGISTER_X), &temp);
  real34Copy(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X));
  real34Copy(&temp,                            REGISTER_REAL34_DATA(REGISTER_X));
}
