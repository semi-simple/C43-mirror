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
  uint32_t dataTypeX, dataTypeY;

  dataTypeX = getRegisterDataType(REGISTER_X);
  dataTypeY = getRegisterDataType(REGISTER_Y);

  if(   (dataTypeX == dtReal16 || dataTypeX == dtAngle16 || dataTypeX == dtReal34 || dataTypeX == dtAngle34 || dataTypeX == dtLongInteger)
     && (dataTypeY == dtReal16 || dataTypeY == dtAngle16 || dataTypeY == dtReal34 || dataTypeY == dtAngle34 || dataTypeY == dtLongInteger)) {
    if(   ((dataTypeX == dtReal16 || dataTypeX == dtAngle16) && real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)))
       || ((dataTypeX == dtReal34 || dataTypeX == dtAngle34) && real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)))
       || ((dataTypeY == dtReal16 || dataTypeY == dtAngle16) && real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y)))
       || ((dataTypeY == dtReal34 || dataTypeY == dtAngle34) && real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)))) {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function fnToRect:", "cannot use NaN as an input of " STD_RIGHT_ARROW "Rect", NULL, NULL);
      #endif
      return;
    }

    bool_t real16 = false;
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    if(dataTypeX == dtLongInteger && dataTypeY == dtLongInteger) {
      convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
      convertLongIntegerRegisterToReal34Register(REGISTER_Y, REGISTER_Y);
      convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_Y), currentAngularMode, AM_RADIAN);
      real16 = true;
    }

    else if(dataTypeX == dtLongInteger) {
      convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
      if(dataTypeY == dtReal16) {
        convertRegister16To34(REGISTER_Y);
        convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_Y), currentAngularMode, AM_RADIAN);
        real16 = true;
      }
      else if(dataTypeY == dtAngle16) {
        convertRegister16To34(REGISTER_Y);
        convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_Y), getRegisterAngularMode(REGISTER_Y), AM_RADIAN);
        real16 = true;
      }
      else {
        convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_Y), dataTypeY == dtAngle34 ? getRegisterAngularMode(REGISTER_Y) : currentAngularMode, AM_RADIAN);
      }
    }

    else if(dataTypeY == dtLongInteger) {
      convertLongIntegerRegisterToReal34Register(REGISTER_Y, REGISTER_Y);
      convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_Y), currentAngularMode, AM_RADIAN);
      if(dataTypeX == dtReal16 || dataTypeX == dtAngle16) {
        convertRegister16To34(REGISTER_X);
        real16 = true;
      }
    }

    else if((dataTypeX == dtReal16 || dataTypeX == dtAngle16) && (dataTypeY == dtReal34 || dataTypeY == dtAngle34)) {
      convertRegister16To34(REGISTER_X);
      convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_Y), dataTypeY == dtAngle34 ? getRegisterAngularMode(REGISTER_Y) : currentAngularMode, AM_RADIAN);
    }

    else if((dataTypeX == dtReal34 || dataTypeX == dtAngle34) && (dataTypeY == dtReal16 || dataTypeY == dtAngle16)) {
      convertRegister16To34(REGISTER_Y);
      convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_Y), dataTypeY == dtAngle16 ? getRegisterAngularMode(REGISTER_Y) : currentAngularMode, AM_RADIAN);
    }

    else {
      if((dataTypeX == dtReal16 || dataTypeX == dtAngle16) && (dataTypeY == dtReal16 || dataTypeY == dtAngle16)) {
        real16 = true;
        convertRegister16To34(REGISTER_X);
        convertRegister16To34(REGISTER_Y);
      }

      convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_Y), dataTypeY == dtAngle34 ? getRegisterAngularMode(REGISTER_Y) : currentAngularMode, AM_RADIAN);
    }

    real34_t magnitude34, theta34;

    real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &magnitude34);
    real34Copy(REGISTER_REAL34_DATA(REGISTER_Y), &theta34);
    real34PolarToRectangular(&magnitude34, &theta34, REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y)); // theta34 radian
    setRegisterDataType(REGISTER_X, dtReal34, TAG_NONE);
    setRegisterDataType(REGISTER_Y, dtReal34, TAG_NONE);

    if(real16) {
      convertRegister34To16(REGISTER_X);
      convertRegister34To16(REGISTER_Y);
    }

    temporaryInformation = TI_X_Y;

    refreshRegisterLine(REGISTER_X);
    refreshRegisterLine(REGISTER_Y);
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot convert (%s, %s) to rectangular coordinates!", getDataTypeName(getRegisterDataType(REGISTER_X), false, false), getDataTypeName(getRegisterDataType(REGISTER_Y), false, false));
      showInfoDialog("In function fnToRect:", errorMessage, NULL, NULL);
    #endif
  }
}



void real16PolarToRectangular(const real16_t *magnitude16, const real16_t *theta16, real16_t *real16, real16_t *imag16) {  // theta16 in radian
  real34_t real34, imag34, magnitude34, theta34;

  real16ToReal34(magnitude16, &magnitude34);
  real16ToReal34(theta16, &theta34);

  real34PolarToRectangular(&magnitude34, &theta34, &real34, &imag34);  // theta34 in radian

  real34ToReal16(&real34, real16);
  real34ToReal16(&imag34, imag16);
}



void real34PolarToRectangular(const real34_t *magnitude34, const real34_t *theta34, real34_t *real34, real34_t *imag34) {
  real34_t sin, cos;

  WP34S_cvt_2rad_sincos(&sin, &cos, theta34, AM_RADIAN);
  real34Multiply(magnitude34, &cos, real34);
  real34Multiply(magnitude34, &sin, imag34);
}
