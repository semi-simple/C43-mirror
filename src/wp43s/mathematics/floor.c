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
 * \file floor.c
 ***********************************************/

#include "wp43s.h"



void (* const Floor[12])(void) = {
// regX ==> 1            2          3           4           5           6           7           8           9            10            11         12
//          Long integer Real16     Complex16   Angle16     Time        Date        String      Real16 mat  Complex16 m  Short integer Real34     Complex34
            floorLonI,   floorRe16, floorError, floorError, floorError, floorError, floorError, floorRm16,  floorError,  floorError,   floorRe34, floorError
};



/********************************************//**
 * \brief Data type error in floor
 *
 * \param void
 * \return void
 ***********************************************/
void floorError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate floor for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnFloor:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and floor(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnFloor(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  Floor[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
}



void floorLonI(void) {
}



void floorRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function floorRe16:", "cannot use NaN as X input of floor", NULL, NULL);
    #endif
    return;
  }

  if(real16IsInfinite(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function floorRe16:", "cannot use " STD_PLUS_MINUS STD_INFINITY " as X input of floor", NULL, NULL);
    #endif
    return;
  }

  convertReal16ToLongIntegerRegister(REGISTER_REAL16_DATA(REGISTER_X), REGISTER_X, DEC_ROUND_FLOOR);
}



void floorRm16(void) {
  fnToBeCoded();
}



void floorRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function floorRe34:", "cannot use NaN as X input of sin", NULL, NULL);
    #endif
    return;
  }

  if(real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function floorRe34:", "cannot use " STD_PLUS_MINUS STD_INFINITY " as X input of floor", NULL, NULL);
    #endif
    return;
  }

  convertReal34ToLongIntegerRegister(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_X, DEC_ROUND_FLOOR);
}
