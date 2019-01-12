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



void (* const cube[12])(void) = {
// regX ==> 1             2             3             4           5           6           7           8             9             10             11            12
//          Big integer   real16        complex16     Angle       Time        Angle       String      real16 mat    complex16 m   Small integer  real34        complex34
            mulBigIBigI,  mulRe16Re16,  mulCo16Co16,  errorCube,  errorCube,  errorCube,  errorCube,  mulRm16Rm16,  mulCm16Cm16,  mulSmaISmaI,   mulRe34Re34,  mulCo34Co34
};



/********************************************//**
 * \brief Data type error in cubing
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void errorCube(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("errorCube");
  #endif

  displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot cube %s", getRegisterDataTypeName(op1, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "by %s", getRegisterDataTypeName(op2, true, false));
    showInfoDialog("In function fnCube:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
  #endif

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("errorCube");
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
  if(cube[getRegisterDataType(REGISTER_X)] != errorCube) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    result = REGISTER_X;
    op1    = allocateTemporaryRegister();
    op2    = allocateTemporaryRegister();

    copySourceRegisterToDestRegister(REGISTER_X, op1);
    copySourceRegisterToDestRegister(REGISTER_X, op2);
    cube[getRegisterDataType(op1)]();

    copySourceRegisterToDestRegister(REGISTER_X, op1);
    copySourceRegisterToDestRegister(REGISTER_L, op2);
    cube[getRegisterDataType(op1)]();

    freeTemporaryRegister(op1);
    freeTemporaryRegister(op2);

    refreshStack();
  }
  else {
    errorCube();
  }
}
