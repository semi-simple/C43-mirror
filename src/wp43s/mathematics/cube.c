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
 * \file cube.c
 ***********************************************/

#include "wp43s.h"



void (* const cube[13])(void) = {
// regX ==> 1            2            3            4            5          6          7          8            9            10            11           12           13
//          Long integer Real16       Complex16    Angle16      Time       Angle      String     Real16 mat   Complex16 m  Short integer Real34       complex34    Angle34
            mulLonILonI, mulRe16Re16, mulCo16Co16, mulRe16Re16, cubeError, cubeError, cubeError, mulRm16Rm16, mulCm16Cm16, mulShoIShoI,  mulRe34Re34, mulCo34Co34, mulRe34Re34
};



/********************************************//**
 * \brief Data type error in cubing
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void cubeError(void) {
  displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot cube %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnCube:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and regX × regX × regX ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void fnCube(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  result = REGISTER_X;
  opX    = allocateTemporaryRegister();
  opY    = allocateTemporaryRegister();

  copySourceRegisterToDestRegister(REGISTER_X, opX);
  copySourceRegisterToDestRegister(REGISTER_X, opY);
  cube[getRegisterDataType(opX)]();

  copySourceRegisterToDestRegister(REGISTER_X, opX);
  copySourceRegisterToDestRegister(REGISTER_L, opY);
  cube[getRegisterDataType(opX)]();

  adjustResult(result, false, true, opX, opY, -1);
}
