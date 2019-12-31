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
  real39_t x, y;

  dataTypeX = getRegisterDataType(REGISTER_X);
  dataTypeY = getRegisterDataType(REGISTER_Y);

  if((dataTypeX == dtReal34 || dataTypeX == dtLongInteger) && (dataTypeY == dtReal34 || dataTypeY == dtLongInteger)) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    switch(dataTypeX) {
      case dtLongInteger: convertLongIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39); break;
      case dtReal34:      real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);            break;
      default: {
        sprintf(errorMessage, "In function fnToPolar: %" FMT32U " is an unexpected dataTypeX value!", dataTypeX);
        displayBugScreen(errorMessage);
      }
    }

    switch(dataTypeY) {
      case dtLongInteger: convertLongIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39); break;
      case dtReal34:      real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);            break;
      default: {
        sprintf(errorMessage, "In function fnToPolar: %" FMT32U " is an unexpected dataTypeY value!", dataTypeY);
        displayBugScreen(errorMessage);
      }
    }

    real39RectangularToPolar(&x, &y, &x, &y);
    convertAngle39FromTo(&y, AM_RADIAN, currentAngularMode);

    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
    reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, currentAngularMode);
    realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
    realToReal34(&y, REGISTER_REAL34_DATA(REGISTER_Y));

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



// The theta34 output angle is in radian
void real34RectangularToPolar(const real34_t *real34, const real34_t *imag34, real34_t *magnitude34, real34_t *theta34) {
  real39_t real, imag, magnitude, theta;

  real34ToReal(real34, &real);
  real34ToReal(imag34, &imag);

  real39RectangularToPolar(&real, &imag, &magnitude, &theta); // theta in radian

  realToReal34(&magnitude, magnitude34);
  realToReal34(&theta, theta34);
}



void real39RectangularToPolar(const real_t *re, const real_t *im, real_t *magnitude, real_t *theta) { // theta is in ]-pi, pi]
  if(realIsZero(re)) {
    if(realIsZero(im)) {
      realZero(magnitude);
      realZero(theta);
    }
    else if(realIsNegative(im)) {
      realCopyAbs(im, magnitude);
      realMinus(const_piOn2, theta, &ctxtReal39); //  -90°
    }
    else {
      realCopy(im, magnitude);
      realCopy(const_piOn2, theta); // 90°
    }
  }
  else { // real != 0
    real39_t real, imag;

    realCopy(re, &real);
    realCopy(im, &imag);

    // Magnitude
    realMultiply(&real, &real, magnitude, &ctxtReal39);
    realFMA(&imag, &imag, magnitude, magnitude, &ctxtReal39);
    realSquareRoot(magnitude, magnitude, &ctxtReal39);

    // Angle
    WP34S_Atan2(&imag, &real, theta);
  }
}



void real51RectangularToPolar(const real_t *re, const real_t *im, real_t *magnitude, real_t *theta) { // theta is in ]-pi, pi]
  if(realIsZero(re)) {
    if(realIsZero(im)) {
      realZero(magnitude);
      realZero(theta);
    }
    else if(realIsNegative(im)) {
      realCopyAbs(im, magnitude);
      realMinus(const_piOn2, theta, &ctxtReal51); //  -90°
    }
    else {
      realCopy(im, magnitude);
      realCopy(const_piOn2, theta); // 90°
    }
  }
  else { // real != 0
    real51_t real, imag;

    realCopy(re, &real);
    realCopy(im, &imag);

    // Magnitude
    realMultiply(&real, &real, magnitude, &ctxtReal51);
    realFMA(&imag, &imag, magnitude, magnitude, &ctxtReal51);
    realSquareRoot(magnitude, magnitude, &ctxtReal51);

    // Angle
    WP34S_Atan2((real_t *)&imag, (real_t *)&real, theta);
  }
}



void real75RectangularToPolar(const real_t *re, const real_t *im, real_t *magnitude, real_t *theta) { // theta is in ]-pi, pi]
  if(realIsZero(re)) {
    if(realIsZero(im)) {
      realZero(magnitude);
      realZero(theta);
    }
    else if(realIsNegative(im)) {
      realCopyAbs(im, magnitude);
      realMinus(const_piOn2, theta, &ctxtReal75); //  -90°
    }
    else {
      realCopy(im, magnitude);
      realCopy(const_piOn2, theta); // 90°
    }
  }
  else { // real != 0
    real75_t real, imag;

    realCopy(re, &real);
    realCopy(im, &imag);

    // Magnitude
    realMultiply(&real, &real, magnitude, &ctxtReal75);
    realFMA(&imag, &imag, magnitude, magnitude, &ctxtReal75);
    realSquareRoot(magnitude, magnitude, &ctxtReal75);

    // Angle
    WP34S_Atan2((real_t *)&imag, (real_t *)&real, theta);
  }
}
