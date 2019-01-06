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
 * \file cos.c
 ***********************************************/

#include "wp43s.h"



void fnCos(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCos");
  #endif

  bool_t real16 = getRegisterDataType(REGISTER_X) == dtReal16;

  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    real16 = true;
  }

  if(getRegisterDataType(REGISTER_X) == dtReal16 || getRegisterDataType(REGISTER_X) == dtReal34) {
    if(real16) {
      convertRegister16To34(REGISTER_X);
    }

    if(real34IsSpecial(POINTER_TO_REGISTER_DATA(REGISTER_X))) {
      real34Copy(const34_NaN, POINTER_TO_REGISTER_DATA(REGISTER_X));
    }
    else {
    	 WP34S_cvt_2rad_sincos(NULL, REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), REGISTER_X);
    	 setRegisterDataType(REGISTER_X, dtReal34);
    	 setRegisterDataInfo(REGISTER_X, 0);
    }

    if(real16) {
      convertRegister34To16(REGISTER_X);
    }

    roundRegister(REGISTER_X);
    refreshRegisterLine(REGISTER_X);
  }

  else {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "X register is %s", getRegisterDataTypeName(REGISTER_X, true, false));
      showInfoDialog("In function fnCos:", errorMessage, "is not supported for cos(X)!", NULL);
    #endif
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCos");
  #endif
}
