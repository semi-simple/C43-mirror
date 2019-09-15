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

    realIc_t x, y;
    bool_t real16 = true;

    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    switch(dataTypeX) {
      case dtLongInteger: convertLongIntegerRegisterToRealIc(REGISTER_X, &x);   break;
      case dtReal16:
      case dtAngle16:     real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &x); break;
      case dtReal34:
      case dtAngle34:     real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &x); real16 = false; break;
      default: {
        sprintf(errorMessage, "In function fnToRect: %" FMT32U " is an unexpected dataTypeX value!", dataTypeX);
        displayBugScreen(errorMessage);
      }
    }

    switch(dataTypeY) {
      case dtLongInteger: convertLongIntegerRegisterToRealIc(REGISTER_Y, &y);
                          convertAngleIcFromTo(&y, currentAngularMode, AM_RADIAN);
                          break;

      case dtReal16:      real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &y);
                          convertAngleIcFromTo(&y, currentAngularMode, AM_RADIAN);
                          break;

      case dtAngle16:     real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &y);
                          convertAngleIcFromTo(&y, getRegisterAngularMode(REGISTER_Y), AM_RADIAN);
                          break;

      case dtReal34:      real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &y);
                          convertAngleIcFromTo(&y, currentAngularMode, AM_RADIAN);
                          real16 = false;
                          break;

      case dtAngle34:     real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &y);
                          convertAngleIcFromTo(&y, getRegisterAngularMode(REGISTER_Y), AM_RADIAN);
                          real16 = false;
                          break;

      default: {
        sprintf(errorMessage, "In function fnToRect: %" FMT32U " is an unexpected dataTypeY value!", dataTypeY);
        displayBugScreen(errorMessage);
      }
    }

    realIcPolarToRectangular(&x, &y, &x, &y);

    if(real16) {
      reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, TAG_NONE);
      reallocateRegister(REGISTER_Y, dtReal16, REAL16_SIZE, TAG_NONE);
      realIcToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
      realIcToReal16(&y, REGISTER_REAL16_DATA(REGISTER_Y));
    }
    else {
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, TAG_NONE);
      reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, TAG_NONE);
      realIcToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      realIcToReal34(&y, REGISTER_REAL34_DATA(REGISTER_Y));
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
  realIc_t real, imag, magnitude, theta;

  real16ToRealIc(magnitude16, &magnitude);
  real16ToRealIc(theta16, &theta);

  realIcPolarToRectangular(&magnitude, &theta, &real, &imag);  // theta in radian

  realIcToReal16(&real, real16);
  realIcToReal16(&imag, imag16);
}



void real34PolarToRectangular(const real34_t *magnitude34, const real34_t *theta34, real34_t *real34, real34_t *imag34) {
  realIc_t real, imag, magnitude, theta;

  real34ToRealIc(magnitude34, &magnitude);
  real34ToRealIc(theta34, &theta);

  realIcPolarToRectangular(&magnitude, &theta, &real, &imag);  // theta in radian

  realIcToReal34(&real, real34);
  realIcToReal34(&imag, imag34);
}



void realIcPolarToRectangular(const realIc_t *mag, const realIc_t *theta, realIc_t *real, realIc_t *imag) {
  realIc_t sin, cos, magnitude;

  realIcCopy(mag, &magnitude);

  WP34S_cvt_2rad_sincostan(theta, AM_RADIAN, &sin, &cos, NULL);
  realIcMultiply(&magnitude, &cos, real);
  realIcMultiply(&magnitude, &sin, imag);
}

