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
 * \file invert.c
 ***********************************************/

#include "wp43s.h"



void (* const invert[13])(void) = {
// regX ==> 1            2            3            4            5            6            7            8           9           10            11           12           13
//          Long integer Real16       Complex16    Angle16      Time         Date         String       Real16 mat  Complex16 m Short integer Real34       Complex34    Angle34
            divRe16LonI, divRe16Re16, divRe16Co16, divRe16An16, invertError, invertError, invertError, invertRm16, invertCm16, invertError,  divRe16Re34, divRe16Co34, divRe16An34
};



/********************************************//**
 * \brief Data type error in invert
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void invertError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot invert %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnInvert:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and 1 ÷ regX ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void fnInvert(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  result = REGISTER_X;
  opY    = allocateTemporaryRegister();
  reallocateRegister(opY, dtReal16, REAL16_SIZE, TAG_NONE);
  real16Copy(const16_1, REGISTER_REAL16_DATA(opY));
  opX    = allocateTemporaryRegister();
  copySourceRegisterToDestRegister(REGISTER_X, opX);

  invert[getRegisterDataType(REGISTER_X)]();

  adjustResult(result, false, true, opX, opY, -1);
}



void invertRm16(void) {
  fnToBeCoded();
}



void invertCm16(void) {
  fnToBeCoded();
}
