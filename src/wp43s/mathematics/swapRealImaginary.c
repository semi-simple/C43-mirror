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



/********************************************//**
 * \brief Swaps the real part and the imaginary part of a complex
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnSwapRealImaginary(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnSwapRealImaginary");
  #endif

  if(getRegisterDataType(REGISTER_X) == dtComplex16) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    real16Copy(POINTER_TO_REGISTER_DATA(REGISTER_L) + REAL16_SIZE, POINTER_TO_REGISTER_DATA(REGISTER_X)               );
    real16Copy(POINTER_TO_REGISTER_DATA(REGISTER_L)               , POINTER_TO_REGISTER_DATA(REGISTER_X) + REAL16_SIZE);
    refreshStack();
  }
  else if(getRegisterDataType(REGISTER_X) == dtComplex34) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    real34Copy(POINTER_TO_REGISTER_DATA(REGISTER_L) + REAL34_SIZE, POINTER_TO_REGISTER_DATA(REGISTER_X)               );
    real34Copy(POINTER_TO_REGISTER_DATA(REGISTER_L)               , POINTER_TO_REGISTER_DATA(REGISTER_X) + REAL34_SIZE);
    refreshStack();
  }
  #ifdef PC_BUILD
  else if(getRegisterDataType(REGISTER_X) == dtComplex16Matrix) {
    showInfoDialog("In function fnSwapRealImaginary:", "Re" STD_LEFT_RIGHT_ARROWS "Im for a complex matrix:", "to be coded", NULL);
  }
  #endif
  else {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "Re" STD_LEFT_RIGHT_ARROWS "Im doesn't work on %s!", getRegisterDataTypeName(REGISTER_X, true, false));
      showInfoDialog("In function fnSwapRealImaginary:", errorMessage, NULL, NULL);
    #endif
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnSwapRealImaginary");
  #endif
}
