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
 * \file 2pow.c
 ***********************************************/

#include "wp43s.h"



/********************************************//**
 * \brief regX ==> regL and 2^regX ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void fn2Pow(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fn2Pow");
  #endif

  if(getRegisterDataType(REGISTER_X) == dtSmallInteger) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    *(uint64_t *)(POINTER_TO_REGISTER_DATA(REGISTER_X)) = WP34S_int2pow(*(uint64_t *)(POINTER_TO_REGISTER_DATA(REGISTER_X)));
  }

  else if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    int16_t exponent;
    bigInteger_t power;

    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    convertBigIntegerRegisterToBigInteger(REGISTER_X, &power);
    if(bigIntegerCompareUInt(&power, MAX_BIG_INTEGER_SIZE_IN_BITS) != BIG_INTEGER_GREATER_THAN) {
      exponent = power.dp[0];

      bigInteger2Exp(exponent, &power);
      convertBigIntegerToBigIntegerRegister(&power, REGISTER_X);
    }
    else {
      displayCalcErrorMessage(4, REGISTER_T, REGISTER_X);
      #if(EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "2^ this value would result in a value exceeding %" FMT16S " bits!", MAX_BIG_INTEGER_SIZE_IN_BITS);
        bigIntegerToString(&power, tmpStr3000, 10);
        showInfoDialog("In function fn2Pow:", errorMessage, tmpStr3000, NULL);
      #endif
    }
  }

  else if(getRegisterDataType(REGISTER_X) == dtReal16) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    op1 = allocateTemporaryRegister();
    reallocateRegister(op1, dtReal16, REAL16_SIZE, 0);
    real16Copy(const16_2, POINTER_TO_REGISTER_DATA(op1));

    op2 = allocateTemporaryRegister();
    copySourceRegisterToDestRegister(REGISTER_X, op2);

    result = REGISTER_X;
    powRe16Re16();
  }

  else if(getRegisterDataType(REGISTER_X) == dtReal34) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    op1 = allocateTemporaryRegister();
    reallocateRegister(op1, dtReal34, REAL34_SIZE, 0);
    real34Copy(const34_2, POINTER_TO_REGISTER_DATA(op1));

    op2 = allocateTemporaryRegister();
    copySourceRegisterToDestRegister(REGISTER_X, op2);

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

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fn2Pow");
  #endif
}
