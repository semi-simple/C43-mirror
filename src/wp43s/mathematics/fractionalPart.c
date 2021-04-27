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
 * \file fractionalPart.c
 ***********************************************/

#include "mathematics/fractionalPart.h"

#include "debug.h"
#include "error.h"
#include "items.h"
#include "matrix.h"
#include "registers.h"
#include "registerValueConversions.h"

#include "wp43s.h"



TO_QSPI void (* const fp[NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS])(void) = {
// regX ==> 1            2       3         4        5        6        7          8           9             10
//          Long integer Real34  Complex34 Time     Date     String   Real34 mat Complex34 m Short integer Config data
            fpLonI,      fpReal, fpError,  fpError, fpError, fpError, fpRema,    fpError,    fpShoI,       fpError
};



/********************************************//**
 * \brief Data type error in FP
 *
 * \param void
 * \return void
 ***********************************************/
#if (EXTRA_INFO_ON_CALC_ERROR == 1)
void fpError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    sprintf(errorMessage, "cannot calculate FP for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    moreInfoOnError("In function fnFp:", errorMessage, NULL, NULL);
}
#endif // (EXTRA_INFO_ON_CALC_ERROR == 1)



/********************************************//**
 * \brief regX ==> regL and FP(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnFp(uint16_t unusedButMandatoryParameter) {
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
  fp[getRegisterDataType(REGISTER_X)]();
}



void fpLonI(void) {
  longInteger_t x;

  longIntegerInit(x); // Set to 0
  convertLongIntegerToLongIntegerRegister(x, REGISTER_X);
  longIntegerFree(x);
}



void fpRema(void) {
  elementwiseRema(fpReal);
}



void fpShoI(void) {
  *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = 0;
}



void fpReal(void) {
  real34_t x;

  real34ToIntegralValue(REGISTER_REAL34_DATA(REGISTER_X), &x, DEC_ROUND_DOWN);
  real34Subtract(REGISTER_REAL34_DATA(REGISTER_X), &x ,REGISTER_REAL34_DATA(REGISTER_X));
}
