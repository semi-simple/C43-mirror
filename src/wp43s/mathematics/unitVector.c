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



void (* const unitVector[13])(void) = {
// regX ==> 1                2                3               4                5                6                7                8                9                10               11               12              13
//          Long integer     Real16           Complex16       Angle16          Time             Date             String           Real16 mat       Complex16 m      Short integer    Real34           Complex34       Angle34
            unitVectorError, unitVectorError, unitVectorCo16, unitVectorError, unitVectorError, unitVectorError, unitVectorError, unitVectorError, unitVectorError, unitVectorError, unitVectorError, unitVectorCo34, unitVectorError
};



/********************************************//**
 * \brief Data type error in unitVector
 *
 * \param void
 * \return void
 ***********************************************/
void unitVectorError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
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
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  unitVector[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



void unitVectorCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function unitVectorCo16:", "cannot use NaN as an input of unitVector", NULL, NULL);
    #endif
    return;
  }

  real16_t temp;

  real16Multiply(REGISTER_REAL16_DATA(REGISTER_X), REGISTER_REAL16_DATA(REGISTER_X), &temp);
  real16FMA(REGISTER_IMAG16_DATA(REGISTER_X), REGISTER_IMAG16_DATA(REGISTER_X), &temp, &temp);
  real16SquareRoot(&temp, &temp);
  real16Divide(REGISTER_REAL16_DATA(REGISTER_X), &temp, REGISTER_REAL16_DATA(REGISTER_X));
  real16Divide(REGISTER_IMAG16_DATA(REGISTER_X), &temp, REGISTER_IMAG16_DATA(REGISTER_X));
}



void unitVectorCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function unitVectorCo34:", "cannot use NaN as an input of unitVector", NULL, NULL);
    #endif
    return;
  }

  real34_t temp;

  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X), &temp);
  real34FMA(REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X), &temp, &temp);
  real34SquareRoot(&temp, &temp);
  real34Divide(REGISTER_REAL34_DATA(REGISTER_X), &temp, REGISTER_REAL34_DATA(REGISTER_X));
  real34Divide(REGISTER_IMAG34_DATA(REGISTER_X), &temp, REGISTER_IMAG34_DATA(REGISTER_X));
}
