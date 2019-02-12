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



void (* const unitVector[12])(void) = {
//  regX ==> 1                2                3               4                5                6                7                8                9                10               11               12
//           Big integer      real16           complex16       Date             Time             Date             String           real16 mat       complex16 m      Small integer    real34           complex34
             errorUnitVector, errorUnitVector, unitVectorCo16, errorUnitVector, errorUnitVector, errorUnitVector, errorUnitVector, errorUnitVector, errorUnitVector, errorUnitVector, errorUnitVector, unitVectorCo34
};



/********************************************//**
 * \brief Data type error in unitVector
 *
 * \param void
 * \return void
 ***********************************************/
void errorUnitVector(void) {
  displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate the unit vector of %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnUnitVector:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and unitVector(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnUnitVector(uint16_t unusedParamButMandatory) {
  if(unitVector[getRegisterDataType(REGISTER_X)] != errorSign) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    result = REGISTER_X;
    opX    = allocateTemporaryRegister();
    copySourceRegisterToDestRegister(REGISTER_X, opX);

    unitVector[getRegisterDataType(REGISTER_X)]();
    freeTemporaryRegister(opX);

    refreshStack();
  }
  else {
    errorSign();
  }
}



void unitVectorCo16(void) {
  real16_t temp;

  real16Multiply(REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(opX), &temp);
  real16FMA(REGISTER_IMAG16_DATA(opX), REGISTER_IMAG16_DATA(opX), &temp, &temp);
  real16SquareRoot(&temp, &temp);
  real16Divide(REGISTER_REAL16_DATA(opX), &temp, REGISTER_REAL16_DATA(result));
  real16Divide(REGISTER_IMAG16_DATA(opX), &temp, REGISTER_IMAG16_DATA(result));
}



void unitVectorCo34(void) {
  real34_t temp;

  real34Multiply(REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(opX), &temp);
  real34FMA(REGISTER_IMAG34_DATA(opX), REGISTER_IMAG34_DATA(opX), &temp, &temp);
  real34SquareRoot(&temp, &temp);
  real34Divide(REGISTER_REAL34_DATA(opX), &temp, REGISTER_REAL34_DATA(result));
  real34Divide(REGISTER_IMAG34_DATA(opX), &temp, REGISTER_IMAG34_DATA(result));
}
