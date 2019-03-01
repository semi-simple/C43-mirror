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
 * \file toRect.c
 ***********************************************/

#include "wp43s.h"



void fnToRect(uint16_t unusedParamButMandatory) {
  if(   (getRegisterDataType(REGISTER_X) == dtReal16 || getRegisterDataType(REGISTER_X) == dtReal34 || getRegisterDataType(REGISTER_X) == dtLongInteger)
     && (getRegisterDataType(REGISTER_Y) == dtReal16 || getRegisterDataType(REGISTER_Y) == dtReal34 || getRegisterDataType(REGISTER_Y) == dtLongInteger || getRegisterDataType(REGISTER_Y) == dtAngle)) {
    bool_t real16 = (getRegisterDataType(REGISTER_X) != dtReal34 && getRegisterDataType(REGISTER_Y) != dtReal34);

    saveStack();

    if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
      convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    }
    else if(getRegisterDataType(REGISTER_X) == dtReal16) {
      convertRegister16To34(REGISTER_X);
    }

    if(getRegisterDataType(REGISTER_Y) == dtLongInteger) {
      convertLongIntegerRegisterToReal34Register(REGISTER_Y, REGISTER_Y);
    }
    else if(getRegisterDataType(REGISTER_Y) == dtReal16) {
      convertRegister16To34(REGISTER_Y);
    }

    if(getRegisterDataType(REGISTER_Y) != dtAngle) {
      #if (ANGLE16 == 1)
        convertRegister16To34(REGISTER_X);
      #endif
      convertAngleToInternal(REGISTER_ANGLE_DATA(REGISTER_X), angularMode);
    }

    if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function fnToPolar:", "cannot use NaN as an input of " STD_RIGHT_ARROW "Pol", NULL, NULL);
      #endif
      restoreStack();
      refreshStack();
      return;
    }

    real34_t magnitude34, theta34;

    real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &magnitude34);
    real34Copy(REGISTER_REAL34_DATA(REGISTER_Y), &theta34);
    real34PolarToRectangular(&magnitude34, &theta34, REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y)); // theta34 in internal units

    if(real16) {
      convertRegister34To16(REGISTER_X);
      convertRegister34To16(REGISTER_Y);
    }

    temporaryInformation = TI_X_Y;

    refreshRegisterLine(REGISTER_X);
    refreshRegisterLine(REGISTER_Y);
  }
  else {
    displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X); // Invalid input data type for this operation
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot convert (%s, %s) to rectangular coordinates!", getDataTypeName(getRegisterDataType(REGISTER_X), false, false), getDataTypeName(getRegisterDataType(REGISTER_Y), false, false));
      showInfoDialog("In function fnToRect:", errorMessage, NULL, NULL);
    #endif
  }
}



void real16PolarToRectangular(const real16_t *magnitude16, const real16_t *theta16, real16_t *real16, real16_t *imag16) {  // theta16 in internal units
  real34_t real34, imag34, magnitude34, theta34;

  real16ToReal34(magnitude16, &magnitude34);
  real16ToReal34(theta16, &theta34);

  real34PolarToRectangular(&magnitude34, &theta34, &real34, &imag34);  // theta34 in internal units

  real34ToReal16(&real34, real16);
  real34ToReal16(&imag34, imag16);
}



void real34PolarToRectangular(const real34_t *magnitude34, const real34_t *theta34, real34_t *real34, real34_t *imag34) {  // theta34 in internal units
  real34_t sin, cos;

  WP34S_cvt_2rad_sincos(&sin, &cos, theta34); // theta34 in internal units
  real34Multiply(magnitude34, &cos, real34);
  real34Multiply(magnitude34, &sin, imag34);
}
