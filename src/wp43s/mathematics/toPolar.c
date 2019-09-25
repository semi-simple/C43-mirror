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
        showInfoDialog("In function fnToPolar:", "cannot use NaN as an input of " STD_RIGHT_ARROW "Pol", NULL, NULL);
      #endif
      return;
    }

    realIc_t x, y;
    bool_t real16 = true;

    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    switch(dataTypeX) {
      case dtLongInteger: convertLongIntegerRegisterToRealIc(REGISTER_X, &x);   break;
      case dtReal16:      real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &x); break;
      case dtReal34:      real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &x); real16 = false; break;
      default: {
        sprintf(errorMessage, "In function fnToPolar: %" FMT32U " is an unexpected dataTypeX value!", dataTypeX);
        displayBugScreen(errorMessage);
      }
    }

    switch(dataTypeY) {
      case dtLongInteger: convertLongIntegerRegisterToRealIc(REGISTER_Y, &y);   break;
      case dtReal16:      real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_Y), &y); break;
      case dtReal34:      real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_Y), &y); real16 = false; break;
      default: {
        sprintf(errorMessage, "In function fnToPolar: %" FMT32U " is an unexpected dataTypeY value!", dataTypeY);
        displayBugScreen(errorMessage);
      }
    }

    realIcRectangularToPolar(&x, &y, &x, &y);
    convertAngleIcFromTo(&y, AM_RADIAN, currentAngularMode);

    if(real16) {
      reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);
      reallocateRegister(REGISTER_Y, dtReal16, REAL16_SIZE, currentAngularMode);
      realIcToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
      realIcToReal16(&y, REGISTER_REAL16_DATA(REGISTER_Y));
    }
    else {
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
      reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, currentAngularMode);
      realIcToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      realIcToReal34(&y, REGISTER_REAL34_DATA(REGISTER_Y));
    }

    temporaryInformation = TI_RADIUS_THETA;
    refreshRegisterLine(REGISTER_X);
    refreshRegisterLine(REGISTER_Y);
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot convert (%s, %s) to polar coordinates!", getDataTypeName(getRegisterDataType(REGISTER_X), false, false), getDataTypeName(getRegisterDataType(REGISTER_Y), false, false));
      showInfoDialog("In function fnToPolar:", errorMessage, NULL, NULL);
    #endif
  }
}



// The theta16 output angle is in radian
void real16RectangularToPolar(const real16_t *real16, const real16_t *imag16, real16_t *magnitude16, real16_t *theta16) {
  realIc_t real, imag, magnitude, theta;

  real16ToRealIc(real16, &real);
  real16ToRealIc(imag16, &imag);

  realIcRectangularToPolar(&real, &imag, &magnitude, &theta); // theta in radian

  realIcToReal16(&magnitude, magnitude16);
  realIcToReal16(&theta, theta16);
}



// The theta34 output angle is in radian
void real34RectangularToPolar(const real34_t *real34, const real34_t *imag34, real34_t *magnitude34, real34_t *theta34) {
  realIc_t real, imag, magnitude, theta;

  real34ToRealIc(real34, &real);
  real34ToRealIc(imag34, &imag);

  realIcRectangularToPolar(&real, &imag, &magnitude, &theta); // theta in radian

  realIcToReal34(&magnitude, magnitude34);
  realIcToReal34(&theta, theta34);
}



void realIcRectangularToPolar(const realIc_t *re, const realIc_t *im, realIc_t *magnitude, realIc_t *theta) { // theta is in ]-pi, pi]
  if(realIcIsZero(re)) {
    if(realIcIsZero(im)) {
      realIcZero(magnitude);
      realIcZero(theta);
    }
    else if(realIcIsNegative(im)) {
      realIcCopyAbs(im, magnitude);
      realIcCopy(const_piOn2, theta); //  90°
      realIcChangeSign(theta);        // -90°
    }
    else {
      realIcCopy(im, magnitude);
      realIcCopy(const_piOn2, theta); // 90°
    }
  }
  else { // real != 0
    realIc_t real, imag;

    realIcCopy(re, &real);
    realIcCopy(im, &imag);

    // Magnitude
    realIcMultiply(&real, &real, magnitude);
    realIcFMA(&imag, &imag, magnitude, magnitude);
    realIcSquareRoot(magnitude, magnitude);

    // Angle
    realIcDivide(&imag, &real, theta);
    WP34S_Atan2(&imag, &real, theta);
  }
}
