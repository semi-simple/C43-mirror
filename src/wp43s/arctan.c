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
 * \file arctan.c
 ***********************************************/

#include "wp43s.h"



void fnArctan(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnArctan");
  #endif

  dataType_t dataType = dtReal34;

  saveStack();

  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtReal16) {
    convertRegister16To34(REGISTER_X);
    dataType = dtReal16;
  }

  if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    dataType = dtReal16;
  }

  if(getRegisterDataType(REGISTER_X) == dtReal34) {
    WP34S_do_atan(REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)));
    convertRegisterAngleFromTo(REGISTER_X, AM_RADIAN, angularMode);

    if(dataType == dtReal16) {
      convertRegister34To16(REGISTER_X);
      if(angularMode == AM_DMS) {
        checkDms(REAL16_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)));
      }
    }

    temporaryInformation = TI_ANGLE;
    displayAngularMode = angularMode;
    refreshRegisterLine(REGISTER_X);
  }

  else {
    restoreStack();
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "X register is %s", getRegisterDataTypeName(REGISTER_X, true, false));
      showInfoDialog("In function fnArctan:", errorMessage, "is not supported for arctan(X)!", NULL);
    #endif
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnArctan");
  #endif
}
