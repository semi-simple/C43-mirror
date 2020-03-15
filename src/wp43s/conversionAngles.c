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

  switch(getRegisterDataType(REGISTER_X)) {
    case dtLongInteger:
      convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
      convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), fromAngularMode, currentAngularMode);
      setRegisterAngularMode(REGISTER_X, currentAngularMode);
      break;

    case dtReal34:
      if(getRegisterAngularMode(REGISTER_X) == fromAngularMode) {
        convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), fromAngularMode, currentAngularMode);
        setRegisterAngularMode(REGISTER_X, currentAngularMode);
      }
      else {
        displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          sprintf(errorMessage, "input angle34 must be tagged %s!", getAngularModeName(fromAngularMode));
          showInfoDialog("In function fnCvtToCurrentAngularMode:", errorMessage, NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }
      break;

    default:
      displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "%s cannot be converted to an angle!", getRegisterDataTypeName(REGISTER_X, true, false));
        showInfoDialog("In function fnCvtToCurrentAngularMode:", "the input value must be a long integer, a real16, a real34, an angle16 or an angle34", errorMessage, NULL);
      #endif
      restoreStack();
      refreshStack();
      return;
  }

  refreshRegisterLine(REGISTER_X);
}



void fnCvtFromCurrentAngularMode(uint16_t toAngularMode) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  switch(getRegisterDataType(REGISTER_X)) {
    case dtLongInteger:
      convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
      convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), currentAngularMode, toAngularMode);
      setRegisterAngularMode(REGISTER_X, toAngularMode);
      break;

    case dtReal34:
      convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), getRegisterAngularMode(REGISTER_X), toAngularMode);
      setRegisterAngularMode(REGISTER_X, toAngularMode);
      break;

    default:
      displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "%s cannot be converted to an angle!", getRegisterDataTypeName(REGISTER_X, true, false));
        showInfoDialog("In function fnCvtFromCurrentAngularMode:", "the input value must be a long integer, a real16, a real34, an angle16 or an angle34", errorMessage, NULL);
      #endif
      restoreStack();
      refreshStack();
      return;
  }

  refreshRegisterLine(REGISTER_X);
}



void fnCvtDegToRad(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  switch(getRegisterDataType(REGISTER_X)) {
    case dtLongInteger:
      convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
      convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), AM_DEGREE, AM_RADIAN);
      setRegisterAngularMode(REGISTER_X, AM_RADIAN);
      break;

    case dtReal34:
      if(getRegisterAngularMode(REGISTER_X) == AM_DEGREE || getRegisterAngularMode(REGISTER_X) == AM_NONE) {
        convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), AM_DEGREE, AM_RADIAN);
        setRegisterAngularMode(REGISTER_X, AM_RADIAN);
      }
      else {
        displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtDegToRad:", "cannot use an angle34 not tagged degree as an input of fnCvtDegToRad", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }
      break;

    default:
      displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "%s cannot be converted to an angle!", getRegisterDataTypeName(REGISTER_X, true, false));
        showInfoDialog("In function fnCvtDegToRad:", "the input value must be a real16, a real34, a long integer", errorMessage, NULL);
      #endif
      restoreStack();
      refreshStack();
      return;
  }

  refreshRegisterLine(REGISTER_X);
}



void fnCvtRadToDeg(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  switch(getRegisterDataType(REGISTER_X)) {
    case dtLongInteger:
      convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
      convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), AM_RADIAN, AM_DEGREE);
      setRegisterAngularMode(REGISTER_X, AM_DEGREE);
      break;

    case dtReal34:
      if(getRegisterAngularMode(REGISTER_X) == AM_RADIAN || getRegisterAngularMode(REGISTER_X) == AM_NONE) {
        convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), AM_RADIAN, AM_DEGREE);
        setRegisterAngularMode(REGISTER_X, AM_DEGREE);
      }
      else {
        displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtRadToDeg:", "cannot use an angle34 not tagged degree as an input of fnCvtRadToDeg", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }
      break;

    default:
      displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "%s cannot be converted to an angle!", getRegisterDataTypeName(REGISTER_X, true, false));
        showInfoDialog("In function fnCvtRadToDeg:", "the input value must be a real16, a real34, a long integer", errorMessage, NULL);
      #endif
      restoreStack();
      refreshStack();
      return;
  }

  refreshRegisterLine(REGISTER_X);
}



void fnCvtDegToDms(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  switch(getRegisterDataType(REGISTER_X)) {
    case dtLongInteger:
      convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
      convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), AM_DEGREE, AM_DMS);
      setRegisterAngularMode(REGISTER_X, AM_DMS);
      break;

    case dtReal34:
      if(getRegisterAngularMode(REGISTER_X) == AM_DEGREE || getRegisterAngularMode(REGISTER_X) == AM_NONE) {
        convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), AM_DEGREE, AM_DMS);
        setRegisterAngularMode(REGISTER_X, AM_DMS);
      }
      else {
        displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtDegToDms:", "cannot use an angle34 not tagged degree as an input of fnCvtDegToDms", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }
      break;

    default:
      displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "%s cannot be converted to an angle!", getRegisterDataTypeName(REGISTER_X, true, false));
        showInfoDialog("In function fnCvtDegToDms:", "the input value must be a real16, a real34, a long integer", errorMessage, NULL);
      #endif
      restoreStack();
      refreshStack();
      return;
  }

  refreshRegisterLine(REGISTER_X);
}



void fnCvtDmsToDeg(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  switch(getRegisterDataType(REGISTER_X)) {
    case dtLongInteger:
      convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
      convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), AM_DMS, AM_DEGREE);
      setRegisterAngularMode(REGISTER_X, AM_DEGREE);
      break;

    case dtReal34:
      if(getRegisterAngularMode(REGISTER_X) == AM_DMS || getRegisterAngularMode(REGISTER_X) == AM_NONE) {
        convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), AM_DMS, AM_DEGREE);
        setRegisterAngularMode(REGISTER_X, AM_DEGREE);
      }
      else {
        displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtRadToDeg:", "cannot use an angle34 not tagged degree as an input of fnCvtRadToDeg", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }
      break;

    default:
      displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "%s cannot be converted to an angle!", getRegisterDataTypeName(REGISTER_X, true, false));
        showInfoDialog("In function fnCvtRadToDeg:", "the input value must be a real16, a real34, a long integer", errorMessage, NULL);
      #endif
      restoreStack();
      refreshStack();
      return;
  }

  refreshRegisterLine(REGISTER_X);
}



void convertAngle34FromTo(real34_t *angle34, uint32_t fromAngularMode, uint32_t toAngularMode) {
  real39_t angle;

  real34ToReal(angle34, &angle);
  convertAngle39FromTo(&angle, fromAngularMode, toAngularMode);
  realToReal34(&angle, angle34);
}



void convertAngle39FromTo(real_t *angle, uint32_t fromAngularMode, uint32_t toAngularMode) {
  int16_t sign;
  bool_t toDms;
  real39_t degrees, minutes, seconds;

  if(fromAngularMode == AM_DMS) {
    // Convert angle from DMS to DEGREE
    sign = realIsNegative(angle) ? -1 : 1;
    realSetPositiveSign(angle);

    decContextClearStatus(&ctxtReal34, DEC_Invalid_operation);
    realToIntegralValue(angle, &degrees, DEC_ROUND_DOWN, &ctxtReal39);

    realSubtract(angle, &degrees, angle, &ctxtReal39);
    realMultiply(angle, const_100, angle, &ctxtReal39);

    realToIntegralValue(angle, &minutes, DEC_ROUND_DOWN, &ctxtReal39);

    realSubtract(angle, &minutes, angle, &ctxtReal39);
    realMultiply(angle, const_100, &seconds, &ctxtReal39);

    if(realCompareGreaterEqual(&seconds, const_60)) {
      realSubtract(&seconds, const_60, &seconds, &ctxtReal39);
      realAdd(&minutes, const_1, &minutes, &ctxtReal39);
    }

    if(realCompareGreaterEqual(&minutes, const_60)) {
      realSubtract(&minutes, const_60, &minutes, &ctxtReal39);
      realAdd(&degrees, const_1, &degrees, &ctxtReal39);
    }

    realDivide(&minutes, const_60,   &minutes, &ctxtReal39);
    realDivide(&seconds, const_3600, &seconds, &ctxtReal39);

    realAdd(&degrees, &minutes, angle, &ctxtReal39);
    realAdd(angle,    &seconds, angle, &ctxtReal39);

    if(sign == -1) {
      realSetNegativeSign(angle);
    }

    fromAngularMode = AM_DEGREE;
  }

  if(toAngularMode == AM_DMS) {
    toAngularMode = AM_DEGREE;
    toDms = true;
  }
  else {
    toDms = false;
  }

  switch(fromAngularMode) {
    case AM_DEGREE:
      switch(toAngularMode) {
        case AM_GRAD:   realDivide(  angle, const_9on10,   angle, &ctxtReal39); break;
        case AM_RADIAN: realDivide(  angle, const_180onPi, angle, &ctxtReal39); break;
        case AM_MULTPI: realDivide(  angle, const_180,     angle, &ctxtReal39); break;
        default: {}
      }
      break;

    case AM_GRAD:
      switch(toAngularMode) {
        case AM_DEGREE: realMultiply(angle, const_9on10,   angle, &ctxtReal39); break;
        case AM_RADIAN: realDivide(  angle, const_200onPi, angle, &ctxtReal39); break;
        case AM_MULTPI: realDivide(  angle, const_200,     angle, &ctxtReal39); break;
        default: {}
      }
      break;

    case AM_RADIAN:
      switch(toAngularMode) {
        case AM_DEGREE: realMultiply(angle, const_180onPi, angle, &ctxtReal39); break;
        case AM_GRAD:   realMultiply(angle, const_200onPi, angle, &ctxtReal39); break;
        case AM_MULTPI: realDivide(  angle, const_pi,      angle, &ctxtReal39); break;
        default: {}
      }
      break;

    case AM_MULTPI:
      switch(toAngularMode) {
        case AM_DEGREE: realMultiply(angle, const_180,     angle, &ctxtReal39); break;
        case AM_GRAD:   realMultiply(angle, const_200,     angle, &ctxtReal39); break;
        case AM_RADIAN: realMultiply(angle, const_pi,      angle, &ctxtReal39); break;
        default: {}
      }
      break;

    default: {}
  }

  if(toDms) {
    // Convert angle from DEGREE to DMS
    sign = realIsNegative(angle) ? -1 : 1;
    realSetPositiveSign(angle);

    realToIntegralValue(angle, &degrees, DEC_ROUND_DOWN, &ctxtReal39);

    realSubtract(angle, &degrees, angle, &ctxtReal39);
    realMultiply(angle, const_60, angle, &ctxtReal39);

    realToIntegralValue(angle, &minutes, DEC_ROUND_DOWN, &ctxtReal39);

    realSubtract(angle, &minutes, angle, &ctxtReal39);
    realMultiply(angle, const_60, &seconds, &ctxtReal39);

    realDivide(&minutes, const_100,   &minutes, &ctxtReal39);
    realDivide(&seconds, const_10000, &seconds, &ctxtReal39);

    realAdd(&degrees, &minutes, angle, &ctxtReal39);
    realAdd(angle,    &seconds, angle, &ctxtReal39);

    if(sign == -1) {
      realSetNegativeSign(angle);
    }
  }
}



void checkDms34(real34_t *angle34Dms) {
  int16_t  sign;
  real39_t angleDms, degrees, minutes, seconds;

  real34ToReal(angle34Dms, &angleDms);

  sign = realIsNegative(&angleDms) ? -1 : 1;
  realSetPositiveSign(&angleDms);

  realToIntegralValue(&angleDms, &degrees, DEC_ROUND_DOWN, &ctxtReal39);
  realSubtract(&angleDms, &degrees, &angleDms, &ctxtReal39);

  realMultiply(&angleDms, const_100, &angleDms, &ctxtReal39);
  realToIntegralValue(&angleDms, &minutes, DEC_ROUND_DOWN, &ctxtReal39);
  realSubtract(&angleDms, &minutes, &angleDms, &ctxtReal39);

  realMultiply(&angleDms, const_100, &seconds, &ctxtReal39);

  if(realCompareGreaterEqual(&seconds, const_60)) {
    realSubtract(&seconds, const_60, &seconds, &ctxtReal39);
    realAdd(&minutes, const_1, &minutes, &ctxtReal39);
  }

  if(realCompareGreaterEqual(&minutes, const_60)) {
    realSubtract(&minutes, const_60, &minutes, &ctxtReal39);
    realAdd(&degrees, const_1, &degrees, &ctxtReal39);
  }

  realDivide(&minutes, const_100, &minutes, &ctxtReal39);
  realAdd(&degrees, &minutes, &angleDms, &ctxtReal39);
  realDivide(&seconds, const_10000, &seconds, &ctxtReal39);
  realAdd(&angleDms, &seconds, &angleDms, &ctxtReal39);

  if(sign == -1) {
    realSetNegativeSign(&angleDms);
  }

  realToReal34(&angleDms, angle34Dms);
}
