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
 * \file unitVector.c
 ***********************************************/

#include "wp43s.h"



/********************************************//**
 * \brief Unit vector of a complex or a matrix
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnUnitVector(uint16_t unusedParamButMandatory) {
  if(getRegisterDataType(REGISTER_X) == dtComplex16) {
    real16_t temp;

    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    real16Multiply(REGISTER_REAL16_DATA(REGISTER_X), REGISTER_REAL16_DATA(REGISTER_X), &temp);
    real16FMA(REGISTER_IMAG16_DATA(REGISTER_X), REGISTER_IMAG16_DATA(REGISTER_X), &temp, &temp);
    real16SquareRoot(&temp, &temp);
    real16Divide(REGISTER_REAL16_DATA(REGISTER_X), &temp, REGISTER_REAL16_DATA(REGISTER_X));
    real16Divide(REGISTER_IMAG16_DATA(REGISTER_X), &temp, REGISTER_IMAG16_DATA(REGISTER_X));

    refreshStack();
  }
  else if(getRegisterDataType(REGISTER_X) == dtComplex34) {
    real34_t temp;

    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X), &temp);
    real34FMA(REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X), &temp, &temp);
    real34SquareRoot(&temp, &temp);
    real34Divide(REGISTER_REAL34_DATA(REGISTER_X), &temp, REGISTER_REAL34_DATA(REGISTER_X));
    real34Divide(REGISTER_IMAG34_DATA(REGISTER_X), &temp, REGISTER_IMAG34_DATA(REGISTER_X));

    refreshStack();
  }
  #ifdef PC_BUILD
  else if(getRegisterDataType(REGISTER_X) == dtReal16Matrix || getRegisterDataType(REGISTER_X) == dtComplex16Matrix) {
    showInfoDialog("In function fnUnitVector:", "UNITV for matrix:", "to be coded", NULL);
  }
  #endif
  else {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "UNITV doesn't work an %s!", getRegisterDataTypeName(REGISTER_X, true, false));
      showInfoDialog("In function fnUnitVector:", errorMessage, NULL, NULL);
    #endif
  }
}
