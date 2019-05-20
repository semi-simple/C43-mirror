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

#include "wp43s.h"



void (* const fp[13])(void) = {
// regX ==> 1            2       3         4       5        6        7        8          9           10            11      12        13
//          Long integer Real16  Complex16 Angle16 Time     Date     String   Real16 mat Complex16 m Short integer Real34  Complex34 Angle34
            fpLonI,      fpRe16, fpError,  fpRe16, fpError, fpError, fpError, fpRm16,    fpError,    fpShoI,       fpRe34, fpError,  fpRe34
};



/********************************************//**
 * \brief Data type error in FP
 *
 * \param void
 * \return void
 ***********************************************/
void fpError(void) {
  displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate FP for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnFp:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and FP(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnFp(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  result = REGISTER_X;
  opX    = allocateTemporaryRegister();
  copySourceRegisterToDestRegister(REGISTER_X, opX);

  fp[getRegisterDataType(REGISTER_X)]();
  freeTemporaryRegister(opX);

  if(lastErrorCode == 0) {
    refreshRegisterLine(REGISTER_X);
  }
  else {
    restoreStack();
    refreshStack();
  }
}



void fpLonI(void) {
  longInteger_t temp;

  longIntegerSetZero(&temp);
  convertLongIntegerToLongIntegerRegister(&temp, result);
}



void fpRe16(void) {
  real16_t integerPart;

  real16ToIntegral(REGISTER_REAL16_DATA(opX), &integerPart);
  real16Subtract(REGISTER_REAL16_DATA(opX), &integerPart ,REGISTER_REAL16_DATA(result));
}



void fpRm16(void) {
  fnToBeCoded();
}



void fpRe34(void) {
  real34_t integerPart;

  real34ToIntegral(REGISTER_REAL34_DATA(opX), &integerPart);
  real34Subtract(REGISTER_REAL34_DATA(opX), &integerPart ,REGISTER_REAL34_DATA(result));
}



void fpShoI(void) {
  *(REGISTER_SHORT_INTEGER_DATA(result)) = 0;
}
