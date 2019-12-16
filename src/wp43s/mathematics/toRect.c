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

  if(   (dataTypeX == dtReal16 || dataTypeX == dtReal34 || dataTypeX == dtLongInteger)
     && (dataTypeY == dtReal16 || dataTypeY == dtReal34 || dataTypeY == dtLongInteger)) {
    if(   (dataTypeX == dtReal16 && real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)))
       || (dataTypeX == dtReal34 && real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)))
       || (dataTypeY == dtReal16 && real16IsNaN(REGISTER_REAL16_DATA(REGISTER_Y)))
       || (dataTypeY == dtReal34 && real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)))) {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function fnToRect:", "cannot use NaN as an input of " STD_RIGHT_ARROW "Rect", NULL, NULL);
      #endif
      return;
    }

    real39_t x, y;
    bool_t real16 = true;
    uint32_t yAngularMode;

    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    switch(dataTypeX) {
      case dtLongInteger: convertLongIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);      break;
      case dtReal16:      real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &x);                 break;
      case dtReal34:      real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x); real16 = false; break;
      default: {
        sprintf(errorMessage, "In function fnToRect: %" FMT32U " is an unexpected dataTypeX value!", dataTypeX);
        displayBugScreen(errorMessage);
      }
    }

    yAngularMode = getRegisterAngularMode(REGISTER_Y);
    if(yAngularMode == AM_NONE) {
      yAngularMode = currentAngularMode;
    }

    switch(dataTypeY) {
      case dtLongInteger: convertLongIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
                          convertAngle39FromTo(&y, currentAngularMode, AM_RADIAN);
                          break;

      case dtReal16:      real16ToReal(REGISTER_REAL16_DATA(REGISTER_Y), &y);
                          convertAngle39FromTo(&y, yAngularMode, AM_RADIAN);
                          break;

      case dtReal34:      real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
                          convertAngle39FromTo(&y, yAngularMode, AM_RADIAN);
                          real16 = false;
                          break;

      default: {
        sprintf(errorMessage, "In function fnToRect: %" FMT32U " is an unexpected dataTypeY value!", dataTypeY);
        displayBugScreen(errorMessage);
      }
    }

    real39PolarToRectangular(&x, &y, &x, &y);

    if(real16) {
      reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);
      reallocateRegister(REGISTER_Y, dtReal16, REAL16_SIZE, AM_NONE);
      realToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
      realToReal16(&y, REGISTER_REAL16_DATA(REGISTER_Y));
    }
    else {
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
      reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, AM_NONE);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      realToReal34(&y, REGISTER_REAL34_DATA(REGISTER_Y));
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
  real39_t real, imag, magnitude, theta;

  real16ToReal(magnitude16, &magnitude);
  real16ToReal(theta16, &theta);

  real39PolarToRectangular(&magnitude, &theta, &real, &imag);  // theta in radian

  realToReal16(&real, real16);
  realToReal16(&imag, imag16);
}



void real34PolarToRectangular(const real34_t *magnitude34, const real34_t *theta34, real34_t *real34, real34_t *imag34) {
  real39_t real, imag, magnitude, theta;

  real34ToReal(magnitude34, &magnitude);
  real34ToReal(theta34, &theta);

  real39PolarToRectangular(&magnitude, &theta, &real, &imag);  // theta in radian

  realToReal34(&real, real34);
  realToReal34(&imag, imag34);
}



void real39PolarToRectangular(const real_t *mag, const real_t *theta, real_t *real, real_t *imag) {
  real39_t sin, cos, magnitude;

  realCopy(mag, &magnitude);

  WP34S_Cvt2RadSinCosTan(theta, AM_RADIAN, &sin, &cos, NULL);
  realMultiply(&magnitude, &cos, real, &ctxtReal39);
  realMultiply(&magnitude, &sin, imag, &ctxtReal39);
}



void real51PolarToRectangular(const real_t *mag, const real_t *theta, real_t *real, real_t *imag) {
  real51_t sin, cos, magnitude;

  realCopy(mag, &magnitude);

  WP34S_Cvt2RadSinCosTan(theta, AM_RADIAN, (real_t *)&sin, (real_t *)&cos, NULL);
  realMultiply(&magnitude, &cos, real, &ctxtReal51);
  realMultiply(&magnitude, &sin, imag, &ctxtReal51);
}



void real75PolarToRectangular(const real_t *mag, const real_t *theta, real_t *real, real_t *imag) {
  real75_t sin, cos, magnitude;

  realCopy(mag, &magnitude);

  WP34S_Cvt2RadSinCosTan(theta, AM_RADIAN, (real_t *)&sin, (real_t *)&cos, NULL);
  realMultiply(&magnitude, &cos, real, &ctxtReal75);
  realMultiply(&magnitude, &sin, imag, &ctxtReal75);
}
