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
 * \file magnitude.c
 ***********************************************/

#include "wp43s.h"



/********************************************//**
 * \brief Returns the absolute value of an integer or a real and the magnitude of a complex
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnMagnitude(uint16_t unusedParamButMandatory) {
  if(getRegisterDataType(REGISTER_X) == dtComplex16) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, 0);
    real16Multiply(REGISTER_REAL16_DATA(REGISTER_L), REGISTER_REAL16_DATA(REGISTER_L), REGISTER_REAL16_DATA(REGISTER_X));
    real16FMA(REGISTER_IMAG16_DATA(REGISTER_L), REGISTER_IMAG16_DATA(REGISTER_L), REGISTER_REAL16_DATA(REGISTER_X), REGISTER_REAL16_DATA(REGISTER_X));
    real16SquareRoot(REGISTER_REAL16_DATA(REGISTER_X), REGISTER_REAL16_DATA(REGISTER_X));
  }

  else if(getRegisterDataType(REGISTER_X) == dtComplex34) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, 0);
    real34Multiply(REGISTER_REAL34_DATA(REGISTER_L), REGISTER_REAL34_DATA(REGISTER_L), REGISTER_REAL34_DATA(REGISTER_X));
    real34FMA(REGISTER_IMAG34_DATA(REGISTER_L), REGISTER_IMAG34_DATA(REGISTER_L), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
    real34SquareRoot(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
  }

  else if(getRegisterDataType(REGISTER_X) == dtReal16) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    real16SetPositiveSign(REGISTER_REAL16_DATA(REGISTER_X));
  }

  else if(getRegisterDataType(REGISTER_X) == dtReal34) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    real34SetPositiveSign(REGISTER_REAL34_DATA(REGISTER_X));
  }

  else if(getRegisterDataType(REGISTER_X) == dtSmallInteger) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    *(REGISTER_SMALL_INTEGER_DATA(REGISTER_X)) = WP34S_intAbs(*(REGISTER_SMALL_INTEGER_DATA(REGISTER_X)));
  }

  else if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    setRegisterSign(REGISTER_X, 0);
  }

  else {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "|x| doesn't work on %s!", getRegisterDataTypeName(REGISTER_X, true, false));
      showInfoDialog("In function fnMagnitude:", errorMessage, NULL, NULL);
    #endif
  }

  refreshRegisterLine(REGISTER_X);
}
