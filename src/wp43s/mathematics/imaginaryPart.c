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
 * \file imaginaryPart.c
 ***********************************************/

#include "mathematics/imaginaryPart.h"

#include "constantPointers.h"
#include "debug.h"
#include "error.h"
#include "items.h"
#include "registers.h"

#include "wp43s.h"



TO_QSPI void (* const imagPart[NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS])(void) = {
// regX ==> 1              2              3             4              5              6              7              8             9              10
//          Long integer   Real34         complex34     Time           Date           String         Real34 mat     Complex34 m   Short integer  Config data
            imagPartError, imagPartReal,  imagPartCplx, imagPartError, imagPartError, imagPartError, imagPartError, imagPartCxma, imagPartError, imagPartError
};



/********************************************//**
 * \brief Data type error in Im
 *
 * \param void
 * \return void
 ***********************************************/
#if (EXTRA_INFO_ON_CALC_ERROR == 1)
void imagPartError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    sprintf(errorMessage, "cannot calculate Im for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    moreInfoOnError("In function fnImaginaryPart:", errorMessage, NULL, NULL);
}
#endif // (EXTRA_INFO_ON_CALC_ERROR == 1)



/********************************************//**
 * \brief regX ==> regL and Im(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnImaginaryPart(uint16_t unusedButMandatoryParameter) {
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
  imagPart[getRegisterDataType(REGISTER_X)]();
}



void imagPartCxma(void) {
  fnToBeCoded();
}


void imagPartCplx(void) {
  real34_t ip;

  real34Copy(REGISTER_IMAG34_DATA(REGISTER_X), &ip);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
  real34Copy(&ip, REGISTER_REAL34_DATA(REGISTER_X));
}


void imagPartReal(void) {
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
  realToReal34(const_0, REGISTER_REAL34_DATA(REGISTER_X));
}
