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
 * \file 10pow.c
 ***********************************************/

#include "wp43s.h"



/********************************************//**
 * \brief regX ==> regL and 10^regX ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void fn10Pow(uint16_t unusedParamButMandatory) {
  if(getRegisterDataType(REGISTER_X) == dtSmallInteger) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    *(REGISTER_SMALL_INTEGER_DATA(REGISTER_X)) = WP34S_int10pow(*(REGISTER_SMALL_INTEGER_DATA(REGISTER_X)));
  }

  else if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    int16_t exponent;

    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    bigInteger_t temp;
    convertBigIntegerRegisterToBigInteger(REGISTER_X, &temp);
    exponent = (bigIntegerCompareUInt(&temp, 1234) == BIG_INTEGER_GREATER_THAN ? 1234 : temp.dp[0]);

    uIntToBigInteger(10, &temp);
    opY = allocateTemporaryRegister();
    convertBigIntegerToBigIntegerRegister(&temp, opY);

    uIntToBigInteger(exponent, &temp);
    opX = allocateTemporaryRegister();
    convertBigIntegerToBigIntegerRegister(&temp, opX);

    result = allocateTemporaryRegister();
    powBigIBigI();

    copySourceRegisterToDestRegister(result, REGISTER_X);
    freeTemporaryRegister(result);
  }

  else if(getRegisterDataType(REGISTER_X) == dtReal16) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    opY = allocateTemporaryRegister();
    reallocateRegister(opY, dtReal16, REAL16_SIZE, 0);
    real16Copy(const16_10, REGISTER_REAL16_DATA(opY));

    opX = allocateTemporaryRegister();
    copySourceRegisterToDestRegister(REGISTER_X, opX);

    result = REGISTER_X;
    powRe16Re16();
  }

  else if(getRegisterDataType(REGISTER_X) == dtReal34) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    opY = allocateTemporaryRegister();
    reallocateRegister(opY, dtReal34, REAL34_SIZE, 0);
    real34Copy(const34_10, REGISTER_REAL34_DATA(opY));

    opX = allocateTemporaryRegister();
    copySourceRegisterToDestRegister(REGISTER_X, opX);

    result = REGISTER_X;
    powRe34Re34();
  }

  else {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X); // Invalid input data type for this operation
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot calculate the power of 2 of %s!", getDataTypeName(getRegisterDataType(REGISTER_X), true, false));
      showInfoDialog("In function fn2Pow:", errorMessage, NULL, NULL);
    #endif
  }

  refreshRegisterLine(REGISTER_X);
}
