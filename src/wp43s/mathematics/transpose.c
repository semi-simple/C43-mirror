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
 * \file transpose.c
 ***********************************************/

#include "wp43s.h"



TO_QSPI void (* const Transpose[NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS])(void) = {
// regX ==> 1               2               3               4               5               6               7              8              9               10
//          Long integer    Real34          Complex34       Time            Date            String          Real34 mat     Complex34 m    Short integer   Config data
            transposeError, transposeError, transposeError, transposeError, transposeError, transposeError, transposeRema, transposeCxma, transposeError, transposeError
};



/********************************************//**
 * \brief Data type error in transpose
 *
 * \param void
 * \return void
 ***********************************************/
#if (EXTRA_INFO_ON_CALC_ERROR == 1)
void transposeError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    sprintf(errorMessage, "cannot calculate transpose for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    moreInfoOnError("In function fnTranspose:", errorMessage, NULL, NULL);
}
#endif // (EXTRA_INFO_ON_CALC_ERROR == 1)



/********************************************//**
 * \brief regX ==> regL and tr(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnTranspose(uint16_t unusedButMandatoryParameter) {
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  Transpose[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



void transposeRema(void) {
#ifndef TESTSUITE_BUILD
  real34Matrix_t x, res;

  convertReal34MatrixRegisterToReal34Matrix(REGISTER_X, &x);
  transposeRealMatrix(&x, &res);
  convertReal34MatrixToReal34MatrixRegister(&res, REGISTER_X);
  realMatrixFree(&res);
  realMatrixFree(&x);
#endif // TESTSUITE_BUILD
}



void transposeCxma(void) {
  fnToBeCoded();
}
