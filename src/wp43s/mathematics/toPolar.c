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
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnToPolar");
  #endif

  if(   (getRegisterDataType(REGISTER_X) == dtReal16 || getRegisterDataType(REGISTER_X) == dtReal34 || getRegisterDataType(REGISTER_X) == dtBigInteger)
     && (getRegisterDataType(REGISTER_Y) == dtReal16 || getRegisterDataType(REGISTER_Y) == dtReal34 || getRegisterDataType(REGISTER_Y) == dtBigInteger)) {
    bool_t real16 = false;

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

    real34_t real34, imag34;

    real34Copy(POINTER_TO_REGISTER_DATA(REGISTER_X), &real34);
    real34Copy(POINTER_TO_REGISTER_DATA(REGISTER_Y), &imag34);
    real34RectangularToPolar(&real34, &imag34, REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_Y)));

    if(real16) {
      convertRegister34To16(REGISTER_X);
      convertRegister34To16(REGISTER_Y);
    }

    temporaryInformation = TI_RADIUS_THETA;

    refreshRegisterLine(REGISTER_X);
    refreshRegisterLine(REGISTER_Y);
  }
  else {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X); // Invalid input data type for this operation
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot convert (%s, %s) to polar coordinates!", getDataTypeName(getRegisterDataType(REGISTER_X), false, false), getDataTypeName(getRegisterDataType(REGISTER_Y), false, false));
      showInfoDialog("In function fnToPolar:", errorMessage, NULL, NULL);
    #endif
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnToPolar");
  #endif
}



void real16RectangularToPolar(const real16_t *real16, const real16_t *imag16, real16_t *magnitude16, real16_t *theta16) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real16RectangularToPolar");
  #endif

  real34_t real34, imag34, magnitude34, theta34;

  real16ToReal34(real16, &real34);
  real16ToReal34(imag16, &imag34);

  real34RectangularToPolar(&real34, &imag34, &magnitude34, &theta34);

  real34ToReal16(&magnitude34, magnitude16);
  real34ToReal16(&theta34, theta16);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real16RectangularToPolar");
  #endif
}



void real34RectangularToPolar(const real34_t *real34, const real34_t *imag34, real34_t *magnitude34, real34_t *theta34) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real34RectangularToPolar");
  #endif

  if(real34IsZero(real34) && real34IsZero(imag34)) {
    real34Zero(magnitude34);
    real34Zero(theta34);
  }
  else if(real34IsZero(real34) && real34IsNegative(imag34)) {
    real34AbsToReal34(imag34, magnitude34);
    real34Copy(const34_piOn2, theta34);
    real34SetNegativeSign(theta34);
  }
  else if(real34IsZero(real34) && real34IsPositive(imag34)) {
    real34Copy(imag34, magnitude34);
    real34Copy(const34_piOn2, theta34);
  }
  else { // real34 != 0
    // Magnitude
    real34Multiply(real34, real34, magnitude34);
    real34FMA(imag34, imag34, magnitude34, magnitude34);
    real34SquareRoot(magnitude34, magnitude34);

    // Angle
    real34Divide(imag34, real34, theta34);
    WP34S_do_atan(theta34, theta34);
  }

  if(real34IsNegative(real34)) {
    if(real34IsNegative(imag34)) {
      real34Subtract(theta34, const34_pi, theta34);
      if(real34CompareLessEqual(theta34, const34_minusPi)) {
        real34Add(theta34, const34_2pi, theta34);
      }
    }
    else {
      real34Add(theta34, const34_pi, theta34);
      if(real34CompareGreaterThan(theta34, const34_pi)) {
        real34Subtract(theta34, const34_2pi, theta34);
      }
    }
  }

  convertAngle34FromTo(theta34, AM_RADIAN, angularMode);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real34RectangularToPolar");
  #endif
}