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
 * \file toPolar.c
 ***********************************************/

#include "wp43s.h"



void fnToPolar(uint16_t unusedParamButMandatory) {
  if(   (getRegisterDataType(REGISTER_X) == dtReal16 || getRegisterDataType(REGISTER_X) == dtReal34 || getRegisterDataType(REGISTER_X) == dtBigInteger)
     && (getRegisterDataType(REGISTER_Y) == dtReal16 || getRegisterDataType(REGISTER_Y) == dtReal34 || getRegisterDataType(REGISTER_Y) == dtBigInteger)) {
    bool_t real16 = false;

    saveStack();

    if(getRegisterDataType(REGISTER_X) == dtBigInteger && getRegisterDataType(REGISTER_Y) == dtBigInteger) {
      convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
      convertBigIntegerRegisterToReal34Register(REGISTER_Y, REGISTER_Y);
      real16 = true;
    }

    else if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
      convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
      if(getRegisterDataType(REGISTER_Y) == dtReal16) {
        convertRegister16To34(REGISTER_Y);
        real16 = true;
      }
    }

    else if(getRegisterDataType(REGISTER_Y) == dtBigInteger) {
      convertBigIntegerRegisterToReal34Register(REGISTER_Y, REGISTER_Y);
      if(getRegisterDataType(REGISTER_X) == dtReal16) {
        convertRegister16To34(REGISTER_X);
        real16 = true;
      }
    }

    else if(getRegisterDataType(REGISTER_X) == dtReal16 && getRegisterDataType(REGISTER_Y) == dtReal34) {
      convertRegister16To34(REGISTER_X);
    }

    else if(getRegisterDataType(REGISTER_Y) == dtReal16 && getRegisterDataType(REGISTER_X) == dtReal34) {
      convertRegister16To34(REGISTER_Y);
    }

    if(getRegisterDataType(REGISTER_X) == dtReal16) {
      real16 = true;
      convertRegister16To34(REGISTER_X);
      convertRegister16To34(REGISTER_Y);
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

    real34_t real34, imag34;

    real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &real34);
    real34Copy(REGISTER_REAL34_DATA(REGISTER_Y), &imag34);
    real34RectangularToPolar(&real34, &imag34, REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y));

    if(real16) {
      convertRegister34To16(REGISTER_X);
    }

    #if (ANGLE16 == 1)
      convertRegister34To16(REGISTER_Y);
    #endif

    setRegisterDataType(REGISTER_Y, dtAngle);
    setRegisterAngularMode(REGISTER_Y, angularMode);

    temporaryInformation = TI_RADIUS_THETA;
    refreshRegisterLine(REGISTER_X);
    refreshRegisterLine(REGISTER_Y);
  }
  else {
    displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X); // Invalid input data type for this operation
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot convert (%s, %s) to polar coordinates!", getDataTypeName(getRegisterDataType(REGISTER_X), false, false), getDataTypeName(getRegisterDataType(REGISTER_Y), false, false));
      showInfoDialog("In function fnToPolar:", errorMessage, NULL, NULL);
    #endif
  }
}



void real16RectangularToPolar(const real16_t *real16, const real16_t *imag16, real16_t *magnitude16, real16_t *theta16) { // theta16 in internal units
  real34_t real34, imag34, magnitude34, theta34;

  real16ToReal34(real16, &real34);
  real16ToReal34(imag16, &imag34);

  real34RectangularToPolar(&real34, &imag34, &magnitude34, &theta34); // theta34 in internal units

  real34ToReal16(&magnitude34, magnitude16);
  real34ToReal16(&theta34, theta16);
}



void real34RectangularToPolar(const real34_t *real34, const real34_t *imag34, real34_t *magnitude34, real34_t *theta34) { // theta34 in internal units
  if(real34IsZero(real34)) {
    if(real34IsZero(imag34)) {
      real34Zero(magnitude34);
      real34Zero(theta34);
    }
    else if(real34IsNegative(imag34)) {
      real34AbsToReal34(imag34, magnitude34);
      real34Copy(const34_972, theta34); // 270°
    }
    else {
      real34Copy(imag34, magnitude34);
      real34Copy(const34_324, theta34); // 90°
    }
  }
  else { // real34 != 0
    // Magnitude
    real34Multiply(real34, real34, magnitude34);
    real34FMA(imag34, imag34, magnitude34, magnitude34);
    real34SquareRoot(magnitude34, magnitude34);

    // Angle
    real34Divide(imag34, real34, theta34);
    WP34S_do_atan2(imag34, real34, theta34);
    convertAngle34ToInternal(theta34, AM_RADIAN);
  }
}
