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
 * \file zeta.c
 ***********************************************/

#include "wp43s.h"



void (* const Zeta[NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS])(void) = {
// regX ==> 1            2         3          4          5          6          7          8           9             10
//          Long integer Real34    Complex34  Time       Date       String     Real34 mat Complex34 m Short integer Config data
            zetaError,   zetaReal, zetaCplx,  zetaError, zetaError, zetaError, zetaError, zetaError,  zetaError,    zetaError
};



/********************************************//**
 * \brief Data type error in zeta
 *
 * \param void
 * \return void
 ***********************************************/
void zetaError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate Zeta for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    moreInfoOnError("In function fnZeta:", errorMessage, NULL, NULL);
  #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
}



/********************************************//**
 * \brief regX ==> regL and zeta(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnZeta(uint16_t unusedButMandatoryParameter) {
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  Zeta[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



void zetaReal(void) {
  real_t x, res;

  if(getRegisterAngularMode(REGISTER_X) == AM_NONE) {
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
    WP34S_Zeta(&x, &res, &ctxtReal39);
    realToReal34(&res, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    zetaError();
  }
}



void zetaCplx(void) {
  fnToBeCoded();
}
