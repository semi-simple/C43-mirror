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
 * \file conversionAngles.c
 ***********************************************/

#include "wp43s.h"



void fnCvtToCurrentAngularMode(uint16_t fromAngularMode) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    convertBigIntegerRegisterToAngleRegister(REGISTER_X, REGISTER_X);
    convertAngleToInternal(REGISTER_ANGLE_DATA(REGISTER_X), fromAngularMode);
    setRegisterDataType(REGISTER_X, dtAngle);
    setRegisterAngularMode(REGISTER_X, fromAngularMode);
  }

  #if (ANGLE16 == 1)
    if(getRegisterDataType(REGISTER_X) == dtReal34) {
      convertRegister34To16(REGISTER_X);
    }
  #endif

  #if (ANGLE34 == 1)
    if(getRegisterDataType(REGISTER_X) == dtReal16) {
      convertRegister16To34(REGISTER_X);
    }
  #endif

  if(getRegisterDataType(REGISTER_X) == dtReal16 || getRegisterDataType(REGISTER_X) == dtReal34) {
    convertAngleToInternal(REGISTER_ANGLE_DATA(REGISTER_X), fromAngularMode);
    setRegisterDataType(REGISTER_X, dtAngle);
    setRegisterAngularMode(REGISTER_X, fromAngularMode);
  }

  if(getRegisterDataType(REGISTER_X) == dtAngle) {
    if(getRegisterDataInfo(REGISTER_X) == fromAngularMode) {
      setRegisterAngularMode(REGISTER_X, angularMode);
      refreshRegisterLine(REGISTER_X);
      return;
    }
    else {
      displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "the input angle is not tagged as %s!", getAngularModeName(fromAngularMode));
        showInfoDialog("In function fnCvtToCurrentAngularMode:", errorMessage, NULL, NULL);
      #endif

      restoreStack();
      refreshStack();
      return;
    }
  }

  displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "an angle tagged %s!", getAngularModeName(fromAngularMode));
    showInfoDialog("In function fnCvtToCurrentAngularMode:", "the input value must be a real16, a real34, a big integer", errorMessage, NULL);
  #endif
}



void fnCvtFromCurrentAngularMode(uint16_t toAngularMode) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    convertBigIntegerRegisterToAngleRegister(REGISTER_X, REGISTER_X);
    convertAngleToInternal(REGISTER_ANGLE_DATA(REGISTER_X), angularMode);
    setRegisterDataType(REGISTER_X, dtAngle);
    setRegisterAngularMode(REGISTER_X, angularMode);
  }

  #if (ANGLE16 == 1)
    if(getRegisterDataType(REGISTER_X) == dtReal34) {
      convertRegister34To16(REGISTER_X);
    }
  #endif

  #if (ANGLE34 == 1)
    if(getRegisterDataType(REGISTER_X) == dtReal16) {
      convertRegister16To34(REGISTER_X);
    }
  #endif

  if(getRegisterDataType(REGISTER_X) == dtReal16 || getRegisterDataType(REGISTER_X) == dtReal34) {
    convertAngleToInternal(REGISTER_ANGLE_DATA(REGISTER_X), angularMode);
    setRegisterDataType(REGISTER_X, dtAngle);
    setRegisterAngularMode(REGISTER_X, angularMode);
  }

  if(getRegisterDataType(REGISTER_X) == dtAngle) {
    convertAngleFromInternal(REGISTER_ANGLE_DATA(REGISTER_X), getRegisterAngularMode(REGISTER_X));
    setRegisterAngularMode(REGISTER_X, toAngularMode);
    convertAngleToInternal(REGISTER_ANGLE_DATA(REGISTER_X), angularMode);
    setRegisterAngularMode(REGISTER_X, toAngularMode);
    refreshRegisterLine(REGISTER_X);
  }
  else {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "an angle tagged %s!", getAngularModeName(angularMode));
      showInfoDialog("In function fnCvtFromCurrentAngularMode:", "the input value must be a real16, a real34, a big integer", errorMessage, NULL);
    #endif

    restoreStack();
    refreshStack();
  }
}



void fnCvtDegToRad(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    convertBigIntegerRegisterToAngleRegister(REGISTER_X, REGISTER_X);
    convertAngleToInternal(REGISTER_ANGLE_DATA(REGISTER_X), AM_DEGREE);
    setRegisterDataType(REGISTER_X, dtAngle);
    setRegisterAngularMode(REGISTER_X, AM_DEGREE);
  }

  #if (ANGLE16 == 1)
    if(getRegisterDataType(REGISTER_X) == dtReal34) {
      convertRegister34To16(REGISTER_X);
    }
  #endif

  #if (ANGLE34 == 1)
    if(getRegisterDataType(REGISTER_X) == dtReal16) {
      convertRegister16To34(REGISTER_X);
    }
  #endif

  if(getRegisterDataType(REGISTER_X) == dtReal16 || getRegisterDataType(REGISTER_X) == dtReal34) {
    convertAngleToInternal(REGISTER_ANGLE_DATA(REGISTER_X), AM_DEGREE);
    setRegisterDataType(REGISTER_X, dtAngle);
    setRegisterAngularMode(REGISTER_X, AM_DEGREE);
  }

  if(getRegisterDataType(REGISTER_X) == dtAngle) {
    if(getRegisterDataInfo(REGISTER_X) == AM_DEGREE) {
      setRegisterAngularMode(REGISTER_X, AM_RADIAN);
      refreshRegisterLine(REGISTER_X);
      return;
    }
    else {
      displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function fnCvtDegToRad:", "the input angle is not tagged as degree!", NULL, NULL);
      #endif

      restoreStack();
      refreshStack();
      return;
    }
  }

  displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    showInfoDialog("In function fnCvtDegToRad:", "the input value must be a real16, a real34, a big integer", "or an angle tagged degree!", NULL);
  #endif
}



void fnCvtDegToDms(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    convertAngleToInternal(REGISTER_ANGLE_DATA(REGISTER_X), AM_DEGREE);
    setRegisterDataType(REGISTER_X, dtAngle);
    setRegisterAngularMode(REGISTER_X, AM_DEGREE);
  }

  #if (ANGLE16 == 1)
    if(getRegisterDataType(REGISTER_X) == dtReal34) {
      convertRegister34To16(REGISTER_X);
    }
  #endif

  #if (ANGLE34 == 1)
    if(getRegisterDataType(REGISTER_X) == dtReal16) {
      convertRegister16To34(REGISTER_X);
    }
  #endif

  if(getRegisterDataType(REGISTER_X) == dtReal16 || getRegisterDataType(REGISTER_X) == dtReal34) {
    convertAngleToInternal(REGISTER_ANGLE_DATA(REGISTER_X), AM_DEGREE);
    setRegisterDataType(REGISTER_X, dtAngle);
    setRegisterAngularMode(REGISTER_X, AM_DEGREE);
  }

  if(getRegisterDataType(REGISTER_X) == dtAngle) {
    if(getRegisterDataInfo(REGISTER_X) == AM_DEGREE) {
      setRegisterAngularMode(REGISTER_X, AM_DMS);
      refreshRegisterLine(REGISTER_X);
      return;
    }
    else {
      displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function fnCvtDegToDms:", "the input angle is not tagged as degree!", NULL, NULL);
      #endif

      restoreStack();
      refreshStack();
      return;
    }
  }

  displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    showInfoDialog("In function fnCvtDegToDms:", "the input value must be a real16, a real34, a big integer", "or an angle tagged degree!", NULL);
  #endif
}



void fnCvtDmsToDeg(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    convertBigIntegerRegisterToAngleRegister(REGISTER_X, REGISTER_X);
    convertAngleToInternal(REGISTER_ANGLE_DATA(REGISTER_X), AM_DMS);
    setRegisterDataType(REGISTER_X, dtAngle);
    setRegisterAngularMode(REGISTER_X, AM_DMS);
  }

  #if (ANGLE16 == 1)
    if(getRegisterDataType(REGISTER_X) == dtReal34) {
      convertRegister34To16(REGISTER_X);
    }
  #endif

  #if (ANGLE34 == 1)
    if(getRegisterDataType(REGISTER_X) == dtReal16) {
      convertRegister16To34(REGISTER_X);
    }
  #endif

  if(getRegisterDataType(REGISTER_X) == dtReal16 || getRegisterDataType(REGISTER_X) == dtReal34) {
    convertAngleToInternal(REGISTER_ANGLE_DATA(REGISTER_X), AM_DMS);
    setRegisterDataType(REGISTER_X, dtAngle);
    setRegisterAngularMode(REGISTER_X, AM_DMS);
  }

  if(getRegisterDataType(REGISTER_X) == dtAngle) {
    if(getRegisterDataInfo(REGISTER_X) == AM_DMS) {
      setRegisterAngularMode(REGISTER_X, AM_DEGREE);
      refreshRegisterLine(REGISTER_X);
      return;
    }
    else {
      displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function fnCvtDmsToDeg:", "the input angle is not tagged as d.ms!", NULL, NULL);
      #endif

      restoreStack();
      refreshStack();
      return;
    }
  }

  displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    showInfoDialog("In function fnCvtDmsToDeg:", "the input value must be a real16, a real34, a big integer", "or an angle tagged d.ms!", NULL);
  #endif
}



void fnCvtRadToDeg(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    convertBigIntegerRegisterToAngleRegister(REGISTER_X, REGISTER_X);
    convertAngleToInternal(REGISTER_ANGLE_DATA(REGISTER_X), AM_RADIAN);
    setRegisterDataType(REGISTER_X, dtAngle);
    setRegisterAngularMode(REGISTER_X, AM_RADIAN);
  }

  #if (ANGLE16 == 1)
    if(getRegisterDataType(REGISTER_X) == dtReal34) {
      convertRegister34To16(REGISTER_X);
    }
  #endif

  #if (ANGLE34 == 1)
    if(getRegisterDataType(REGISTER_X) == dtReal16) {
      convertRegister16To34(REGISTER_X);
    }
  #endif

  if(getRegisterDataType(REGISTER_X) == dtReal16 || getRegisterDataType(REGISTER_X) == dtReal34) {
    convertAngleToInternal(REGISTER_ANGLE_DATA(REGISTER_X), AM_RADIAN);
    setRegisterDataType(REGISTER_X, dtAngle);
    setRegisterAngularMode(REGISTER_X, AM_RADIAN);
  }

  if(getRegisterDataType(REGISTER_X) == dtAngle) {
    if(getRegisterDataInfo(REGISTER_X) == AM_RADIAN) {
      setRegisterAngularMode(REGISTER_X, AM_DEGREE);
      refreshRegisterLine(REGISTER_X);
      return;
    }
    else {
      displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function fnCvtRadToDeg:", "the input angle is not tagged as radian!", NULL, NULL);
      #endif

      restoreStack();
      refreshStack();
      return;
    }
  }

  displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    showInfoDialog("In function fnCvtRadToDeg:", "the input value must be a real16, a real34, a big integer", "or an angle tagged radian!", NULL);
  #endif
}



void checkDms34(real34_t *angleDms) {
  int16_t  sign;
  real34_t degrees, minutes, seconds;

  sign = real34IsNegative(angleDms) ? -1 : 1;
  real34SetPositiveSign(angleDms);

  real34ToIntegral(angleDms, &degrees);
  real34Subtract(angleDms, &degrees, angleDms);

  real34Multiply(angleDms, const34_100, angleDms);
  real34ToIntegral(angleDms, &minutes);
  real34Subtract(angleDms, &minutes, angleDms);

  real34Multiply(angleDms, const34_100, &seconds);

  if(real34CompareGreaterEqual(&seconds, const34_60)) {
    real34Subtract(&seconds, const34_60, &seconds);
    real34Add(&minutes, const34_1, &minutes);
  }

  if(real34CompareGreaterEqual(&minutes, const34_60)) {
    real34Subtract(&minutes, const34_60, &minutes);
    real34Add(&degrees, const34_1, &degrees);
  }

  real34Divide(&minutes, const34_100, &minutes);
  real34Add(&degrees, &minutes, angleDms);
  real34Divide(&seconds, const34_10000, &seconds);
  real34Add(angleDms, &seconds, angleDms);

  if(sign == -1) {
    real34SetNegativeSign(angleDms);
  }
}



void convertAngle16ToInternal(real16_t *angle16, uint32_t fromAngularMode) {
  real34_t angle34;

  real16ToReal34(angle16, &angle34);
  convertAngle34ToInternal(&angle34, fromAngularMode);
  real34ToReal16(&angle34, angle16);
}



void convertAngle34ToInternal(real34_t *angle34, uint32_t fromAngularMode) {
  if(fromAngularMode == AM_DEGREE || fromAngularMode == AM_DMS) {
    if(fromAngularMode == AM_DEGREE) {
      real34Multiply(angle34, const34_3_6, angle34); // 1296/360  1296=36*36
    }
    else {
      bool_t negativeAngle = false;

      if(real34IsNegative(angle34)) {
        if(!real34IsZero(angle34)) {
          negativeAngle = true;
        }
        real34SetPositiveSign(angle34);
      }

      real34_t degrees, minutes;

      real34ToIntegral(angle34, &degrees);           // degrees = intPart(angle34)           : integral number of degrees
      real34Subtract(angle34, &degrees, angle34);    // angle34 = angle34 - intPart(angle34) : fractional part of degrees

      real34Multiply(angle34, const34_100, angle34); // angle34 = angle34 * 100              : number of minutes
      real34ToIntegral(angle34, &minutes);           // minutes = int(angle34)               : integral number of minutes
      real34Subtract(angle34, &minutes, angle34);    // angle34 = angle34 - intPart(minutes) : fractional part of minutes
      real34Multiply(angle34, const34_100, angle34); // angle34 = angle34 * 100              : number of seconds

      real34Multiply(angle34, const34_1e_3, angle34);      // seconds
      real34FMA(&minutes, const34_0_06, angle34, angle34); // minutes
      real34FMA(&degrees, const34_3_6, angle34, angle34);  // degrees

      if(negativeAngle && !real34IsZero(angle34)) {
        real34SetNegativeSign(angle34);
      }
    }
  }

  else if(fromAngularMode == AM_GRAD) {
    real34Multiply(angle34, const34_3_24, angle34); // 1296/400  1296=36*36
  }

  else if(fromAngularMode == AM_MULTPI) {
    real34Multiply(angle34, const34_648, angle34); // 1296/2  1296=36*36
  }

  else if(fromAngularMode == AM_RADIAN) {
    real34Multiply(angle34, const34_648onPi, angle34); // 1296/2/pi  1296=36*36
  }

  else {
    sprintf(errorMessage, "In function convertAngle34ToInternal: %" FMT32U " is an unexpected value for fromAngularMode!", fromAngularMode);
    displayBugScreen(errorMessage);
  }
}



void convertAngle16FromInternal(real16_t *angle16, uint32_t toAngularMode) {
  real34_t angle34;

  real16ToReal34(angle16, &angle34);
  convertAngle34FromInternal(&angle34, toAngularMode);
  real34ToReal16(&angle34, angle16);
}



void convertAngle34FromInternal(real34_t *angle34, uint32_t toAngularMode) {
  if(toAngularMode == AM_DEGREE) {
    real34Divide(angle34, const34_3_6, angle34); // 1296/360  1296=36*36
  }

  else if(toAngularMode == AM_DMS) {
    real34_t degrees, minutes;

    real34Divide(angle34, const34_3_6, angle34); // 1296/360  1296=36*36

    real34ToIntegral(angle34, &degrees);           // degrees = intPart(angle34)           : integral number of degrees
    real34Subtract(angle34, &degrees, angle34);    // angle34 = angle34 - intPart(angle34) : fractional part of degrees

    real34Multiply(angle34, const34_60, angle34);  // angle34 = angle34 * 100              : number of minutes
    real34ToIntegral(angle34, &minutes);           // minutes = int(angle34)               : integral number of minutes
    real34Subtract(angle34, &minutes, angle34);    // angle34 = angle34 - intPart(minutes) : fractional part of minutes
    real34Multiply(angle34, const34_60, angle34);  // angle34 = angle34 * 100              : number of seconds

    real34Divide(angle34, const34_10000, angle34);       // seconds
    real34FMA(&minutes, const34_1e_2, angle34, angle34); // minutes
    real34Add(&degrees, angle34, angle34);               // degrees
  }

  else if(toAngularMode == AM_GRAD) {
    real34Divide(angle34, const34_3_24, angle34); // 1296/400  1296=36*36
  }

  else if(toAngularMode == AM_MULTPI) {
    real34Divide(angle34, const34_648, angle34); // 1296/2  1296=36*36
  }

  else if(toAngularMode == AM_RADIAN) {
    real34Divide(angle34, const34_648onPi, angle34); // 1296/2/pi  1296=36*36
  }

  else {
    sprintf(errorMessage, "In function convertAngle34FromInternal: %" FMT32U " is an unexpected value for toAngularMode!", toAngularMode);
    displayBugScreen(errorMessage);
  }
}
