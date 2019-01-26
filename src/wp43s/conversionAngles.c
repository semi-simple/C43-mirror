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

/*
void convertRegisterAngleFromTo(calcRegister_t regist, uint32_t fromAngularMode, uint32_t toAngularMode) {
  if(getRegisterDataType(regist) == dtBigInteger) {
    convertBigIntegerRegisterToReal34Register(regist, regist);
    convertAngle34FromTo(REGISTER_REAL34_DATA(regist), fromAngularMode, toAngularMode);
  }
  else if(getRegisterDataType(regist) == dtReal16) {
    convertRegister16To34(regist);
    convertAngle34FromTo(REGISTER_REAL34_DATA(regist), fromAngularMode, toAngularMode);
  }
  else if(getRegisterDataType(regist) == dtReal34) {
    convertAngle34FromTo(REGISTER_REAL34_DATA(regist), fromAngularMode, toAngularMode);
  }
  else {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "Cannot use %s for converting an angle!", getRegisterDataTypeName(regist, true, false));
      showInfoDialog("In function convertRegisterAngleFromTo:", errorMessage, NULL, NULL);
    #endif

    return;
  }

  setRegisterAngularMode(regist, toAngularMode);
  refreshRegisterLine(regist);
}



void convertAngle34FromTo(real34_t *angle34, uint32_t fromAngularMode, uint32_t toAngularMode) {
  if(fromAngularMode == AM_DEGREE) {
    if(toAngularMode == AM_DMS) {
      convertAngle34ToDms(angle34);
    }

    else if(toAngularMode == AM_GRAD) {
      real34Divide(angle34, const34_0_9, angle34); // 180/200
    }

    else if(toAngularMode == AM_MULTPI) {
      real34Divide(angle34, const34_180, angle34);
    }

    else if(toAngularMode == AM_RADIAN) {
      real34Multiply(angle34, const34_piOn180, angle34);
    }

    else if(toAngularMode != AM_DEGREE) {
      sprintf(errorMessage, "In function convertAngle34FromTo from AM_DEGREE: %" FMT32U " is an unexpected value for toAngularMode!", toAngularMode);
      displayBugScreen(errorMessage);
    }
  }

  else if(fromAngularMode == AM_DMS) {
    if(toAngularMode != AM_DMS) {
      convertAngle34FromDms(angle34);
    }

    if(toAngularMode == AM_GRAD) {
      real34Divide(angle34, const34_0_9, angle34); // 180/200
    }

    else if(toAngularMode == AM_MULTPI) {
      real34Divide(angle34, const34_180, angle34);
    }

    else if(toAngularMode == AM_RADIAN) {
      real34Multiply(angle34, const34_piOn180, angle34);
    }

    else if(toAngularMode != AM_DEGREE && toAngularMode != AM_DMS) {
      sprintf(errorMessage, "In function convertAngle34FromTo from AM_DMS: %" FMT32U " is an unexpected value for toAngularMode!", toAngularMode);
      displayBugScreen(errorMessage);
    }
  }

  else if(fromAngularMode == AM_GRAD) {
    if(toAngularMode == AM_DEGREE) {
      real34Multiply(angle34, const34_0_9, angle34); // 180/200
    }

    else if(toAngularMode == AM_DMS) {
      real34Multiply(angle34, const34_0_9, angle34); // 180/200
      convertAngle34ToDms(angle34);
    }

    else if(toAngularMode == AM_MULTPI) {
      real34Divide(angle34, const34_200, angle34);
    }

    else if(toAngularMode == AM_RADIAN) {
      real34Multiply(angle34, const34_piOn200, angle34);
    }

    else if(toAngularMode != AM_GRAD) {
      sprintf(errorMessage, "In function convertAngle34FromTo from AM_GRAD: %" FMT32U " is an unexpected value for toAngularMode!", toAngularMode);
      displayBugScreen(errorMessage);
    }
  }

  else if(fromAngularMode == AM_MULTPI) {
    if(toAngularMode == AM_DEGREE) {
      real34Multiply(angle34, const34_180, angle34);
    }

    else if(toAngularMode == AM_DMS) {
      real34Multiply(angle34, const34_180, angle34);
      convertAngle34ToDms(angle34);
    }

    else if(toAngularMode == AM_GRAD) {
      real34Multiply(angle34, const34_200, angle34);
    }

    else if(toAngularMode == AM_RADIAN) {
      real34Multiply(angle34, const34_pi, angle34);
    }

    else if(toAngularMode != AM_MULTPI) {
      sprintf(errorMessage, "In function convertAngle34FromTo from AM_MULTPI: %" FMT32U " is an unexpected value for toAngularMode!", toAngularMode);
      displayBugScreen(errorMessage);
    }
  }

  else if(fromAngularMode == AM_RADIAN) {
    if(toAngularMode == AM_DEGREE) {
     real34Divide(angle34, const34_piOn180, angle34);
    }

    else if(toAngularMode == AM_DMS) {
      real34Divide(angle34, const34_piOn180, angle34);
      convertAngle34ToDms(angle34);
    }

    else if(toAngularMode == AM_GRAD) {
      real34Divide(angle34, const34_piOn200, angle34);
    }

    else if(toAngularMode == AM_MULTPI) {
     real34Divide(angle34, const34_pi, angle34);
    }

    else if(toAngularMode != AM_RADIAN) {
      sprintf(errorMessage, "In function convertAngle34FromTo from AM_RADIAN: %" FMT32U " is an unexpected value for toAngularMode!", toAngularMode);
      displayBugScreen(errorMessage);
    }
  }

  else {
    sprintf(errorMessage, "In function convertAngle34FromTo: %" FMT32U " is an unexpected value for fromAngularMode!", fromAngularMode);
    displayBugScreen(errorMessage);
  }
}



void convertRegisterToDms(calcRegister_t regist) {
  convertAngle34ToDms(REGISTER_REAL34_DATA(regist));
}



void convertAngle34ToDms(real34_t *angle34) {
  int16_t  sign;

  real34_t temp0; // integer degrees
  real34_t temp1;

  sign = real34IsNegative(angle34) ? -1 : 1;     // sign = sign(regist)
  real34SetPositiveSign(angle34);                // regist = abs(regist)

  real34ToIntegral(angle34, &temp0);             // temp0 = intPart(regist)            : integral number of degrees
  real34Subtract(angle34, &temp0, angle34);      // regist = regist - intPart(regist)  : fractional part of degrees

  real34Multiply(angle34, const34_60, angle34);  // regist = regist * 60               : number of minutes
  real34ToIntegral(angle34, &temp1);             // temp1 = int(regist)                : integral number of minutes
  real34Subtract(angle34, &temp1, angle34);      // regist = regist - intPart(minutes) : fractional part of minutes
  real34Multiply(angle34, const34_60, angle34);  // regist = regist * 60               : number of seconds
  real34Divide(angle34, const34_10000, angle34); // regist = regist / 10000
  real34Divide(&temp1, const34_100, &temp1);     // temp1 = temp1 / 100
  real34Add(angle34, &temp1, angle34);           // regist = minutes/100 + seconds/10000
  real34Add(angle34, &temp0, angle34);           // regist = degrees + minutes/100 + seconds/10000

  if(sign == -1) {
    real34SetNegativeSign(angle34);
  }
}



void convertRegisterFromDms(calcRegister_t regist) {
  convertAngle34FromDms(REGISTER_REAL34_DATA(regist));
}



void convertAngle34FromDms(real34_t *angle34) {
  int16_t  sign;
  real34_t temp0; // integer degrees
  real34_t temp1;
  real34_t temp2; // for comparisons

  sign = real34IsNegative(angle34) ? -1 : 1;     // sign = sign(regist)
  real34SetPositiveSign(angle34);                // regist = abs(regist)

  real34ToIntegral(angle34, &temp0);             // temp0 = intPart(regist)    : integral number of degrees

  real34Subtract(const34_9000, &temp0, &temp2);
  if(real34IsNegative(&temp2)) {                 // temp0 > 9000 ?
    displayCalcErrorMessage(8, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      real34ToString(&temp0, errorMessage + ERROR_MESSAGE_LENGTH/2);
      sprintf(errorMessage, "degrees = %s", errorMessage + ERROR_MESSAGE_LENGTH/2);
      showInfoDialog("In function convertAngle34FromDms: the input value must be " STD_LESS_EQUAL " 9000!", errorMessage, NULL, NULL);
    #endif
  }

  real34Subtract(angle34, &temp0, angle34);      // regist = regist - intPart(regist) : fractional part of degrees

  real34Multiply(angle34, const34_100, angle34); // regist = regist * 100             : number of minutes
  real34ToIntegral(angle34, &temp1);             // temp1 = int(regist)                 : integral number of minutes

  real34Subtract(&temp1, const34_60, &temp2);
  if(!real34IsNegative(&temp2)) {                // temp1 >= 60 ?
    displayCalcErrorMessage(8, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      real34ToString(&temp1, errorMessage + ERROR_MESSAGE_LENGTH/2);
      sprintf(errorMessage, "minutes = %s", errorMessage + ERROR_MESSAGE_LENGTH/2);
      showInfoDialog("In function convertAngle34FromDms: the input value has minutes " STD_GREATER_EQUAL " 60!", errorMessage, NULL, NULL);
    #endif
  }

  real34Subtract(angle34, &temp1, angle34);      // regist = regist - intPart(minutes) : fractional part of minutes

  real34Subtract(angle34, const34_0_6, &temp2);
  if(!real34IsNegative(&temp2)) {                // regist >= 0.60
    displayCalcErrorMessage(8, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      real34Multiply(angle34, const34_100, angle34);
      real34ToString(angle34, errorMessage + ERROR_MESSAGE_LENGTH/2);
      real34Divide(angle34, const34_100, angle34);
      sprintf(errorMessage, "seconds = %s", errorMessage + ERROR_MESSAGE_LENGTH/2);
      showInfoDialog("In function convertAngle34FromDms: the input value has seconds " STD_GREATER_EQUAL " 60!", errorMessage, NULL, NULL);
    #endif
  }

  real34Divide(&temp1, const34_60, &temp1);      // temp1 = minutes/60
  real34Divide(angle34, const34_36, angle34);    // regist = regist / (3600/100)     : number of seconds
  real34Add(angle34, &temp0, angle34);           // regist = degrees + seconds/3600
  real34Add(angle34, &temp1, angle34);           // regist = degrees + minutes/60 + seconds/3600

  if(sign == -1) {
    real34SetNegativeSign(angle34);
  }
}
*/


void fnCvtFromAngularMode(uint16_t fromAngularMode) {
  if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
  }

  if(getRegisterDataType(REGISTER_X) == dtReal16) {
    convertRegister16To34(REGISTER_X);
  }

  if(getRegisterDataType(REGISTER_X) == dtAngle) {
    if(getRegisterDataInfo(REGISTER_X) == fromAngularMode) {
      setRegisterAngularMode(REGISTER_X, angularMode);
      return;
    }
    else {
      displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "the input angle is not tagged as %s!", getAngularModeName(fromAngularMode));
        showInfoDialog("In function fnCvtFromAngularMode:", errorMessage, NULL, NULL);
      #endif

      return;
    }
  }

  if(getRegisterDataType(REGISTER_X) != dtReal34) {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "an angle tagged %s!", getAngularModeName(fromAngularMode));
      showInfoDialog("In function fnCvtFromAngularMode:", "the input value must be a real16, a real34, a big integer", errorMessage, NULL);
    #endif

    return;
  }

  convertAngle34ToInternal(REGISTER_REAL34_DATA(REGISTER_X), fromAngularMode);
  setRegisterAngularMode(REGISTER_X, angularMode);
  refreshRegisterLine(REGISTER_X);
}



void fnCvtToAngularMode(uint16_t toAngularMode) {
  if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
  }

  if(getRegisterDataType(REGISTER_X) == dtReal16) {
    convertRegister16To34(REGISTER_X);
  }

  if(getRegisterDataType(REGISTER_X) == dtAngle) {
    if(getRegisterDataInfo(REGISTER_X) == angularMode) {
      setRegisterAngularMode(REGISTER_X, toAngularMode);
      return;
    }
    else {
      displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "the input angle is not tagged as %s!", getAngularModeName(angularMode));
        showInfoDialog("In function fnCvtToAngularMode:", errorMessage, NULL, NULL);
      #endif

      return;
    }
  }

  if(getRegisterDataType(REGISTER_X) != dtReal34) {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "an angle tagged %s!", getAngularModeName(angularMode));
      showInfoDialog("In function fnCvtToAngularMode:", "the input value must be a real16, a real34, a big integer", errorMessage, NULL);
    #endif

    return;
  }

  convertAngle34ToInternal(REGISTER_REAL34_DATA(REGISTER_X), angularMode);
  setRegisterDataType(REGISTER_X, dtAngle);
  setRegisterAngularMode(REGISTER_X, toAngularMode);
  refreshRegisterLine(REGISTER_X);
}



void fnCvtDegToRad(uint16_t unusedParamButMandatory) {
  if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
  }

  if(getRegisterDataType(REGISTER_X) == dtReal16) {
    convertRegister16To34(REGISTER_X);
  }

  if(getRegisterDataType(REGISTER_X) == dtAngle) {
    if(getRegisterDataInfo(REGISTER_X) == AM_DEGREE) {
      setRegisterAngularMode(REGISTER_X, AM_RADIAN);
      return;
    }
    else {
      displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function fnCvtDegToRad:", "the input angle is not tagged as degree!", NULL, NULL);
      #endif

      return;
    }
  }

  if(getRegisterDataType(REGISTER_X) != dtReal34) {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function fnCvtDegToRad:", "the input value must be a real16, a real34, a big integer", "an angle tagged degree!", NULL);
    #endif

    return;
  }

  convertAngle34ToInternal(REGISTER_REAL34_DATA(REGISTER_X), AM_DEGREE);
  setRegisterAngularMode(REGISTER_X, AM_RADIAN);
  refreshRegisterLine(REGISTER_X);
}



void fnCvtDegToDms(uint16_t unusedParamButMandatory) {
  if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
  }

  if(getRegisterDataType(REGISTER_X) == dtReal16) {
    convertRegister16To34(REGISTER_X);
  }

  if(getRegisterDataType(REGISTER_X) == dtAngle) {
    if(getRegisterDataInfo(REGISTER_X) == AM_DEGREE) {
      setRegisterAngularMode(REGISTER_X, AM_DMS);
      return;
    }
    else {
      displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function fnCvtDegToDms:", "the input angle is not tagged as degree!", NULL, NULL);
      #endif

      return;
    }
  }

  if(getRegisterDataType(REGISTER_X) != dtReal34) {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function fnCvtDegToDms:", "the input value must be a real16, a real34, a big integer", "an angle tagged degree!", NULL);
    #endif

    return;
  }

  convertAngle34ToInternal(REGISTER_REAL34_DATA(REGISTER_X), AM_DEGREE);
  setRegisterAngularMode(REGISTER_X, AM_DMS);
  refreshRegisterLine(REGISTER_X);
}



void fnCvtDmsToDeg(uint16_t unusedParamButMandatory) {
  if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
  }

  if(getRegisterDataType(REGISTER_X) == dtReal16) {
    convertRegister16To34(REGISTER_X);
  }

  if(getRegisterDataType(REGISTER_X) == dtAngle) {
    if(getRegisterDataInfo(REGISTER_X) == AM_DMS) {
      setRegisterAngularMode(REGISTER_X, AM_DEGREE);
      return;
    }
    else {
      displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function fnCvtDmsToDeg:", "the input angle is not tagged as d.ms!", NULL, NULL);
      #endif

      return;
    }
  }

  if(getRegisterDataType(REGISTER_X) != dtReal34) {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function fnCvtDmsToDeg:", "the input value must be a real16, a real34, a big integer", "an angle tagged d.ms!", NULL);
    #endif

    return;
  }

  convertAngle34ToInternal(REGISTER_REAL34_DATA(REGISTER_X), AM_DMS);
  setRegisterAngularMode(REGISTER_X, AM_DEGREE);
  refreshRegisterLine(REGISTER_X);
}



void fnCvtRadToDeg(uint16_t unusedParamButMandatory) {
  if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
  }

  if(getRegisterDataType(REGISTER_X) == dtReal16) {
    convertRegister16To34(REGISTER_X);
  }

  if(getRegisterDataType(REGISTER_X) == dtAngle) {
    if(getRegisterDataInfo(REGISTER_X) == AM_RADIAN) {
      setRegisterAngularMode(REGISTER_X, AM_DEGREE);
      return;
    }
    else {
      displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function fnCvtRadToDeg:", "the input angle is not tagged as radian!", NULL, NULL);
      #endif

      return;
    }
  }

  if(getRegisterDataType(REGISTER_X) != dtReal34) {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function fnCvtRadToDeg:", "the input value must be a real16, a real34, a big integer", "an angle tagged radian!", NULL);
    #endif

    return;
  }

  convertAngle34ToInternal(REGISTER_REAL34_DATA(REGISTER_X), AM_RADIAN);
  setRegisterAngularMode(REGISTER_X, AM_DEGREE);
  refreshRegisterLine(REGISTER_X);
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
