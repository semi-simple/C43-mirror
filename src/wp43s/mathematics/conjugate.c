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
 * \file conjugate.c
 ***********************************************/

#include "wp43s.h"



void (* const conjugate[12])(void) = {
// regX ==> 1            2           3          4           5           6           7           8           9            10             11          12
//          Big integer  real16      complex16  Date        Time        Date        String      real16 mat  complex16 m  Small integer  real34      complex34
            conjError,   conjError,  conjCo16,  conjError,  conjError,  conjError,  conjError,  conjError,  conjCm16,    conjError,     conjError,  conjCo34
};



/********************************************//**
 * \brief Data type error in exp
 *
 * \param void
 * \return void
 ***********************************************/
void conjError(void) {
  displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate conj for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnConjugate:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief Error message for a valid operation to be coded
 *
 * \param void
 * \return void
 ***********************************************/
void conjToBeCoded(void) {
  #ifdef PC_BUILD
    sprintf(errorMessage, "conjugate(%s)", getRegisterDataTypeName(REGISTER_X, false, false));
    showInfoDialog("Operation to be coded:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and conj(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnConjugate(uint16_t unusedParamButMandatory) {
  if(conjugate[getRegisterDataType(REGISTER_X)] != conjError) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    result = REGISTER_X;
    conjugate[getRegisterDataType(REGISTER_X)]();

    if(lastErrorCode != 0) {
      restoreStack();
      refreshStack();
    }
    else {
      refreshRegisterLine(REGISTER_X);
    }
  }
  else {
    conjError();
  }
}



void conjCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(result)) || real16IsNaN(REGISTER_IMAG16_DATA(result))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function conjCo16:", "cannot use NaN as an input of conjugate", NULL, NULL);
    #endif
    return;
  }

  real16ChangeSign(REGISTER_IMAG16_DATA(result));
}



void conjCm16(void) {
  conjToBeCoded();
}



void conjCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(result)) || real34IsNaN(REGISTER_IMAG34_DATA(result))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function conjCo34:", "cannot use NaN as an input of conjugate", NULL, NULL);
    #endif
    return;
  }

  real34ChangeSign(REGISTER_IMAG34_DATA(result));
}
