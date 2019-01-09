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



/********************************************//**
 * \brief Returns the real part of a complex or a complex matrix
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnRealPart(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnRealPart");
  #endif

  if(getRegisterDataType(REGISTER_X) == dtComplex16) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, 0);
    real16Copy(POINTER_TO_REGISTER_DATA(REGISTER_L), POINTER_TO_REGISTER_DATA(REGISTER_X));
    refreshStack();
  }
  else if(getRegisterDataType(REGISTER_X) == dtComplex34) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, 0);
    real34Copy(POINTER_TO_REGISTER_DATA(REGISTER_L), POINTER_TO_REGISTER_DATA(REGISTER_X));
    refreshStack();
  }
  #ifdef PC_BUILD
  else if(getRegisterDataType(REGISTER_X) == dtComplex16Matrix) {
    showInfoDialog("In function fnRealPart:", "Re for a complex matrix:", "to be coded", NULL);
  }
  #endif
  else {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "Re doesn't work on %s!", getRegisterDataTypeName(REGISTER_X, true, false));
      showInfoDialog("In function fnRealPart:", errorMessage, NULL, NULL);
    #endif
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnRealPart");
  #endif
}
