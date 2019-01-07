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
 * \file lcm.c
 ***********************************************/

#include "wp43s.h"



/********************************************//**
 * \brief regX ==> regL and LCM(regY, regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void fnLcm(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnLcm");
  #endif

  bigInteger_t iOp1, iOp2;

  if(getRegisterDataType(REGISTER_X) == dtSmallInteger && getRegisterDataType(REGISTER_Y) == dtSmallInteger) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    *(uint64_t *)(POINTER_TO_REGISTER_DATA(REGISTER_X)) = WP34S_intLCM(*(uint64_t *)(POINTER_TO_REGISTER_DATA(REGISTER_Y)), *(uint64_t *)(POINTER_TO_REGISTER_DATA(REGISTER_X)));
  }

  else if(getRegisterDataType(REGISTER_X) == dtBigInteger && getRegisterDataType(REGISTER_Y) == dtBigInteger) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    convertBigIntegerRegisterToBigInteger(REGISTER_Y, &iOp1);
    convertBigIntegerRegisterToBigInteger(REGISTER_X, &iOp2);
    bigIntegerLcm(&iOp1, &iOp2, &iOp1);

    convertBigIntegerToBigIntegerRegister(&iOp1, REGISTER_X);
  }

  else if(getRegisterDataType(REGISTER_X) == dtSmallInteger && getRegisterDataType(REGISTER_Y) == dtBigInteger) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    convertSmallIntegerRegisterBigIntegerRegister(REGISTER_X, REGISTER_X);

    convertBigIntegerRegisterToBigInteger(REGISTER_Y, &iOp1);
    convertBigIntegerRegisterToBigInteger(REGISTER_X, &iOp2);
    bigIntegerLcm(&iOp1, &iOp2, &iOp1);

    convertBigIntegerToBigIntegerRegister(&iOp1, REGISTER_X);
  }

  else if(getRegisterDataType(REGISTER_X) == dtBigInteger && getRegisterDataType(REGISTER_Y) == dtSmallInteger) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    convertSmallIntegerRegisterBigIntegerRegister(REGISTER_Y, REGISTER_Y);

    convertBigIntegerRegisterToBigInteger(REGISTER_Y, &iOp1);
    convertBigIntegerRegisterToBigInteger(REGISTER_X, &iOp2);
    bigIntegerLcm(&iOp1, &iOp2, &iOp1);

    convertBigIntegerToBigIntegerRegister(&iOp1, REGISTER_X);
  }

  else {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X); // Invalid input data type for this operation
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot get the LCM from %s and %s!", getDataTypeName(getRegisterDataType(REGISTER_Y), true, false), getDataTypeName(getRegisterDataType(REGISTER_X), true, false));
      showInfoDialog("In function fnLcm:", errorMessage, NULL, NULL);
    #endif
  }

  refreshStack();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnLcm");
  #endif
}
