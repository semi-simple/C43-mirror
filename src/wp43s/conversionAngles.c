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


void convertRegisterAngleFromTo(calcRegister_t regist, uint32_t fromAngularMode, uint32_t toAngularMode) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("convertRegisterAngleFromTo");
  #endif

  if(getRegisterDataType(regist) == dtBigInteger) {
    convertBigIntegerRegisterToReal16Register(regist, regist);
    convertAngle16FromTo(REAL16_POINTER(POINTER_TO_REGISTER_DATA(regist)), fromAngularMode, toAngularMode);
  }
  else if(getRegisterDataType(regist) == dtReal16) {
    convertAngle16FromTo(REAL16_POINTER(POINTER_TO_REGISTER_DATA(regist)), fromAngularMode, toAngularMode);
  }
  else if(getRegisterDataType(regist) == dtReal34) {
    convertAngle34FromTo(REAL34_POINTER(POINTER_TO_REGISTER_DATA(regist)), fromAngularMode, toAngularMode);
  }
  else {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "Cannot use %s for converting an angle!", getRegisterDataTypeName(regist, true, false));
      showInfoDialog("In function convertRegisterAngleFromTo:", errorMessage, NULL, NULL);
    #endif

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("convertRegisterAngleFromTo");
    #endif

    return;
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("convertRegisterAngleFromTo");
  #endif
}



void convertAngle34FromTo(real34_t *angle34, uint32_t fromAngularMode, uint32_t toAngularMode) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("convertAngle34FromTo");
  #endif

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

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("convertAngle34FromTo");
  #endif
}



void convertAngle16FromTo(real16_t *angle16, uint32_t fromAngularMode, uint32_t toAngularMode) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("convertAngle16FromTo");
  #endif

  real34_t angle34;
  real16ToReal34(angle16, &angle34);
  convertAngle34FromTo(&angle34, fromAngularMode, toAngularMode);
  real34ToReal16(&angle34, angle16);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("convertAngle16FromTo");
  #endif
}



void convertRegisterToDms(calcRegister_t regist) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("convertRegisterToDms");
  #endif

  convertAngle34ToDms(REAL34_POINTER(POINTER_TO_REGISTER_DATA(regist)));

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("convertRegisterToDms");
  #endif
}



void convertAngle34ToDms(real34_t *angle34) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("convertAngle34ToDms");
  #endif

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

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("convertAngle34ToDms");
  #endif
}



void convertRegisterFromDms(calcRegister_t regist) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("convertRegisterFromDms");
  #endif

  convertAngle34FromDms(REAL34_POINTER(POINTER_TO_REGISTER_DATA(regist)));

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("convertRegisterFromDms");
  #endif
}



void convertAngle34FromDms(real34_t *angle34) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("convertAngle34FromDms");
  #endif

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

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("convertAngle34FromDms");
  #endif
}



void fnCvtFromAngularMode(uint16_t fromAngularMode) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCvtFromAngularMode");
  #endif

  if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    convertBigIntegerRegisterToReal16Register(REGISTER_X, REGISTER_X);
  }

  if(getRegisterDataType(REGISTER_X) != dtReal16 && getRegisterDataType(REGISTER_X) != dtReal34) {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function fnCvtFromAngularMode:", "the input value must be a real16, a real34 or a big integer!", NULL, NULL);
    #endif
  }

  convertRegisterAngleFromTo(REGISTER_X, fromAngularMode, angularMode);
  roundRegister(REGISTER_X);
  temporaryInformation = TI_ANGLE;
  displayAngularMode = angularMode;
  refreshRegisterLine(REGISTER_X);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCvtFromAngularMode");
  #endif
}



void fnCvtToAngularMode(uint16_t toAngularMode) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCvtToAngularMode");
  #endif

  if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    convertBigIntegerRegisterToReal16Register(REGISTER_X, REGISTER_X);
  }

  if(getRegisterDataType(REGISTER_X) != dtReal16 && getRegisterDataType(REGISTER_X) != dtReal34) {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function fnCvtToAngularMode:", "the input value must be a real16, a real34 or a big integer!", NULL, NULL);
    #endif
  }

  convertRegisterAngleFromTo(REGISTER_X, angularMode, toAngularMode);
  roundRegister(REGISTER_X);
  temporaryInformation = TI_ANGLE;
  displayAngularMode = toAngularMode;
  refreshRegisterLine(REGISTER_X);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCvtToAngularMode");
  #endif
}



void fnCvtDegToRad(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCvtDegToRad");
  #endif

  if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    convertBigIntegerRegisterToReal16Register(REGISTER_X, REGISTER_X);
  }

  if(getRegisterDataType(REGISTER_X) != dtReal16 && getRegisterDataType(REGISTER_X) != dtReal34) {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function fnCvtDegToRad:", "the input value must be a real16, a real34 or a big integer!", NULL, NULL);
    #endif
  }

  convertRegisterAngleFromTo(REGISTER_X, AM_DEGREE, AM_RADIAN);
  roundRegister(REGISTER_X);
  temporaryInformation = TI_ANGLE;
  displayAngularMode = AM_RADIAN;
  refreshRegisterLine(REGISTER_X);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCvtDegToRad");
  #endif
}



void fnCvtDegToDms(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCvtDegToDms");
  #endif

  if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    convertBigIntegerRegisterToReal16Register(REGISTER_X, REGISTER_X);
  }

  if(getRegisterDataType(REGISTER_X) != dtReal16 && getRegisterDataType(REGISTER_X) != dtReal34) {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function fnCvtDegToRad:", "the input value must be a real16, a real34 or a big integer!", NULL, NULL);
    #endif
  }

  convertRegisterAngleFromTo(REGISTER_X, AM_DEGREE, AM_DMS);
  roundRegister(REGISTER_X);
  temporaryInformation = TI_ANGLE;
  displayAngularMode = AM_DMS;
  refreshRegisterLine(REGISTER_X);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCvtDegToDms");
  #endif
}



void fnCvtRadToDeg(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCvtRadToDeg");
  #endif

  if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    convertBigIntegerRegisterToReal16Register(REGISTER_X, REGISTER_X);
  }

  if(getRegisterDataType(REGISTER_X) != dtReal16 && getRegisterDataType(REGISTER_X) != dtReal34) {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function fnCvtDegToRad:", "the input value must be a real16, a real34 or a big integer!", NULL, NULL);
    #endif
  }

  convertRegisterAngleFromTo(REGISTER_X, AM_RADIAN, AM_DEGREE);
  roundRegister(REGISTER_X);
  temporaryInformation = TI_ANGLE;
  displayAngularMode = AM_DEGREE;
  refreshRegisterLine(REGISTER_X);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCvtRadToDeg");
  #endif
}



void checkDms16(real16_t *angleDms) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("checkDms16");
  #endif

  int16_t  sign;
  real16_t degrees, minutes, seconds;

  sign = real16IsNegative(angleDms) ? -1 : 1;
  real16SetPositiveSign(angleDms);

  real16ToIntegral(angleDms, &degrees);
  real16Subtract(angleDms, &degrees, angleDms);

  real16Multiply(angleDms, const16_100, angleDms);
  real16ToIntegral(angleDms, &minutes);
  real16Subtract(angleDms, &minutes, angleDms);

  real16Multiply(angleDms, const16_100, &seconds);

  if(real16CompareGreaterEqual(&seconds, const16_60)) {
    real16Subtract(&seconds, const16_60, &seconds);
    real16Add(&minutes, const16_1, &minutes);
  }

  if(real16CompareGreaterEqual(&minutes, const16_60)) {
    real16Subtract(&minutes, const16_60, &minutes);
    real16Add(&degrees, const16_1, &degrees);
  }

  real16Divide(&minutes, const16_100, &minutes);
  real16Add(&degrees, &minutes, angleDms);
  real16Divide(&seconds, const16_10000, &seconds);
  real16Add(angleDms, &seconds, angleDms);

  if(sign == -1) {
    real16SetNegativeSign(angleDms);
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("checkDms16");
  #endif
}



void checkDms34(real34_t *angleDms) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("checkDms34");
  #endif

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

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("checkDms34");
  #endif
}
