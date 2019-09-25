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
//          Long integer   Real16         Complex16     Angle16        Time           Date           String         Real16 mat     Complex16 m   Short integer  Real34         Complex34
            realPartError, realPartError, realPartCo16, realPartError, realPartError, realPartError, realPartError, realPartError, realPartCm16, realPartError, realPartError, realPartCo34
};



/********************************************//**
 * \brief Data type error in Re
 *
 * \param void
 * \return void
 ***********************************************/
void realPartError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate Re for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnRealPart:", errorMessage, NULL, NULL);
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
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  realPart[getRegisterDataType(REGISTER_X)]();

  if(lastErrorCode == 0) {
    refreshRegisterLine(REGISTER_X);
  }
  else {
    restoreStack();
    refreshStack();
  }
}



void realPartCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function realPartCo16:", "cannot use NaN as X input of Re", NULL, NULL);
    #endif
    return;
  }

  real16_t realPart;

  real16Copy(REGISTER_REAL16_DATA(REGISTER_X), &realPart);
  reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);
  real16Copy(&realPart, REGISTER_REAL16_DATA(REGISTER_X));
}



void realPartCm16(void) {
  fnToBeCoded();
}


void realPartCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function realPartCo34:", "cannot use NaN as X input of Re", NULL, NULL);
    #endif
    return;
  }

  real34_t realPart;

  real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &realPart);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
  real34Copy(&realPart, REGISTER_REAL34_DATA(REGISTER_X));
}
