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
 * \file factorial.c
 ***********************************************/

#include "wp43s.h"



void fnFactorial(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnFactorial");
  #endif

  int8_t dataType = dtReal34;
  bigInteger_t result;

  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    convertBigIntegerRegisterToBigInteger(REGISTER_X, &result);
    if(bigIntegerIsNegative(&result) || bigIntegerCompareUInt(&result, 294) == BIG_INTEGER_GREATER_THAN) {
      convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
      dataType = dtReal16;
    }
    else {
      convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
      dataType = dtBigInteger;
    }
  }

  else if(getRegisterDataType(REGISTER_X) == dtReal16) {
    convertRegister16To34(REGISTER_X);
    dataType = dtReal16;
  }

  else if(getRegisterDataType(REGISTER_X) != dtReal34) {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X); // Invalid input data type for this operation
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot calculate factorial of %s!", getDataTypeName(getRegisterDataType(REGISTER_X), true, false));
      showInfoDialog("In function fnFactorial:", errorMessage, NULL, NULL);
    #endif

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("fnFactorial");
    #endif

    return;
  }

  if(dataType == dtBigInteger) {
    uint16_t counter;

    counter = bigIntegerToUInt(&result);
    if(counter == 0) {
      uIntToBigInteger(1, &result);
    }
    else {
      for(counter--; counter>0; counter--) {
        bigIntegerMultiplyUInt(&result, counter, &result);
      }
    }

    convertBigIntegerToBigIntegerRegister(&result, REGISTER_X);
  }
  else {
    WP34S_real34Factorial(REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)));
  }

  if(dataType == dtReal16) {
    convertRegister34To16(REGISTER_X);
  }

  refreshRegisterLine(REGISTER_X);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnFactorial");
  #endif
}
