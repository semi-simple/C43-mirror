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
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  switch(getRegisterDataType(REGISTER_X)) {
    case dtLongInteger:
      if(fromAngularMode == currentAngularMode) {
        convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
        setRegisterAngularMode(REGISTER_X, currentAngularMode);
      }
      else {
        displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          sprintf(errorMessage, "input angle (longint) default must be ADM = %s!", getAngularModeName(currentAngularMode));
          moreInfoOnError("In function fnCvtToCurrentAngularMode:", errorMessage, NULL, NULL);
        #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
        undo();
        return;
      }
      break;

    case dtReal34:
      if(getRegisterAngularMode(REGISTER_X) == fromAngularMode) {
        convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), fromAngularMode, currentAngularMode);
        setRegisterAngularMode(REGISTER_X, currentAngularMode);
      }
      else
        if(fromAngularMode == currentAngularMode) {
          setRegisterAngularMode(REGISTER_X, currentAngularMode);
        }
        else {
          displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
          #if (EXTRA_INFO_ON_CALC_ERROR == 1)
            sprintf(errorMessage, "input angle34 must be tagged %s!", getAngularModeName(fromAngularMode));
            moreInfoOnError("In function fnCvtToCurrentAngularMode:", errorMessage, NULL, NULL);
          #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
          undo();
          return;
        }
      break;

    default:
      displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "%s cannot be converted to an angle!", getRegisterDataTypeName(REGISTER_X, true, false));
        moreInfoOnError("In function fnCvtToCurrentAngularMode:", "the input value must be a long integer, a real34 or an angle34", errorMessage, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      undo();
      return;
  }
}



void fnCvtFromCurrentAngularMode(uint16_t toAngularMode) {
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  switch(getRegisterDataType(REGISTER_X)) {
    case dtLongInteger:
      convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
      convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), currentAngularMode, toAngularMode);
      setRegisterAngularMode(REGISTER_X, toAngularMode);
      break;

    case dtReal34:
      convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), getRegisterAngularMode(REGISTER_X) == AM_NONE ? currentAngularMode : getRegisterAngularMode(REGISTER_X), toAngularMode);
      setRegisterAngularMode(REGISTER_X, toAngularMode);
      break;

    default:
      displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "%s cannot be converted to an angle!", getRegisterDataTypeName(REGISTER_X, true, false));
        moreInfoOnError("In function fnCvtFromCurrentAngularMode:", "the input value must be a long integer, a real16, a real34, an angle16 or an angle34", errorMessage, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      undo();
      return;
  }
}



void fnCvtDegToRad(uint16_t unusedButMandatoryParameter) {
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
          moreInfoOnError("In function fnCvtDegToRad:", "cannot use an angle34 not tagged degree as an input of fnCvtDegToRad", NULL, NULL);
        #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
        undo();
        return;
      }
      break;

    default:
      displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "%s cannot be converted to an angle!", getRegisterDataTypeName(REGISTER_X, true, false));
        moreInfoOnError("In function fnCvtDegToRad:", "the input value must be a real16, a real34, a long integer", errorMessage, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      undo();
      return;
  }
}



void fnCvtRadToDeg(uint16_t unusedButMandatoryParameter) {
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
          moreInfoOnError("In function fnCvtRadToDeg:", "cannot use an angle34 not tagged degree as an input of fnCvtRadToDeg", NULL, NULL);
        #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
        undo();
        return;
      }
      break;

    default:
      displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "%s cannot be converted to an angle!", getRegisterDataTypeName(REGISTER_X, true, false));
        moreInfoOnError("In function fnCvtRadToDeg:", "the input value must be a real16, a real34, a long integer", errorMessage, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      undo();
      return;
  }
}



void fnCvtDegToDms(uint16_t unusedButMandatoryParameter) {
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
          moreInfoOnError("In function fnCvtDegToDms:", "cannot use an angle34 not tagged degree as an input of fnCvtDegToDms", NULL, NULL);
        #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
        undo();
        return;
      }
      break;

    default:
      displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "%s cannot be converted to an angle!", getRegisterDataTypeName(REGISTER_X, true, false));
        moreInfoOnError("In function fnCvtDegToDms:", "the input value must be a real16, a real34, a long integer", errorMessage, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      undo();
      return;
  }
}



void fnCvtDmsToDeg(uint16_t unusedButMandatoryParameter) {
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
          moreInfoOnError("In function fnCvtDmsToDeg:", "cannot use an angle34 not tagged degree as an input of fnCvtDmsToDeg", NULL, NULL);
        #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
        undo();
        return;
      }
      break;

    default:
      displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "%s cannot be converted to an angle!", getRegisterDataTypeName(REGISTER_X, true, false));
        moreInfoOnError("In function fnCvtDmsToDeg:", "the input value must be a real16, a real34, a long integer", errorMessage, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      undo();
      return;
  }
}



void convertAngle34FromTo(real34_t *angle34, uint32_t fromAngularMode, uint32_t toAngularMode) {
  real_t angle;

  real34ToReal(angle34, &angle);
  convertAngleFromTo(&angle, fromAngularMode, toAngularMode, &ctxtReal39);
  realToReal34(&angle, angle34);
}



void convertAngleFromTo(real_t *angle, uint32_t fromAngularMode, uint32_t toAngularMode, realContext_t *realContext) {
  int16_t sign;
  bool_t toDms;
  real_t degrees, minutes, seconds;

  if(fromAngularMode == AM_DMS) {
    // Convert angle from DMS to DEGREE
    sign = realIsNegative(angle) ? -1 : 1;
    realSetPositiveSign(angle);

    decContextClearStatus(&ctxtReal34, DEC_Invalid_operation);
    realToIntegralValue(angle, &degrees, DEC_ROUND_DOWN, realContext);

    realSubtract(angle, &degrees, angle, realContext);
    realMultiply(angle, const_100, angle, realContext);

    realToIntegralValue(angle, &minutes, DEC_ROUND_DOWN, realContext);

    realSubtract(angle, &minutes, angle, realContext);
    realMultiply(angle, const_100, &seconds, realContext);

    if(realCompareGreaterEqual(&seconds, const_60)) {
      realSubtract(&seconds, const_60, &seconds, realContext);
      realAdd(&minutes, const_1, &minutes, realContext);
    }

    if(realCompareGreaterEqual(&minutes, const_60)) {
      realSubtract(&minutes, const_60, &minutes, realContext);
      realAdd(&degrees, const_1, &degrees, realContext);
    }

    realDivide(&minutes, const_60,   &minutes, realContext);
    realDivide(&seconds, const_3600, &seconds, realContext);

    realAdd(&degrees, &minutes, angle, realContext);
    realAdd(angle,    &seconds, angle, realContext);

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
        case AM_GRAD:   realDivide(  angle, const_9on10,   angle, realContext); break;
        case AM_RADIAN: realDivide(  angle, const_180onPi, angle, realContext); break;
        case AM_MULTPI: realDivide(  angle, const_180,     angle, realContext); break;
        default: {}
      }
      break;

    case AM_GRAD:
      switch(toAngularMode) {
        case AM_DEGREE: realMultiply(angle, const_9on10,   angle, realContext); break;
        case AM_RADIAN: realDivide(  angle, const_200onPi, angle, realContext); break;
        case AM_MULTPI: realDivide(  angle, const_200,     angle, realContext); break;
        default: {}
      }
      break;

    case AM_RADIAN:
      switch(toAngularMode) {
        case AM_DEGREE: realMultiply(angle, const_180onPi, angle, realContext); break;
        case AM_GRAD:   realMultiply(angle, const_200onPi, angle, realContext); break;
        case AM_MULTPI: realDivide(  angle, const_pi,      angle, realContext); break;
        default: {}
      }
      break;

    case AM_MULTPI:
      switch(toAngularMode) {
        case AM_DEGREE: realMultiply(angle, const_180,     angle, realContext); break;
        case AM_GRAD:   realMultiply(angle, const_200,     angle, realContext); break;
        case AM_RADIAN: realMultiply(angle, const_pi,      angle, realContext); break;
        default: {}
      }
      break;

    default: {}
  }

  if(toDms) {
    // Convert angle from DEGREE to DMS
    sign = realIsNegative(angle) ? -1 : 1;
    realSetPositiveSign(angle);

    realToIntegralValue(angle, &degrees, DEC_ROUND_DOWN, realContext);

    realSubtract(angle, &degrees, angle, realContext);
    realMultiply(angle, const_60, angle, realContext);

    realToIntegralValue(angle, &minutes, DEC_ROUND_DOWN, realContext);

    realSubtract(angle, &minutes, angle, realContext);
    realMultiply(angle, const_60, &seconds, realContext);

    realDivide(&minutes, const_100,   &minutes, realContext);
    realDivide(&seconds, const_10000, &seconds, realContext);

    realAdd(&degrees, &minutes, angle, realContext);
    realAdd(angle,    &seconds, angle, realContext);

    if(sign == -1) {
      realSetNegativeSign(angle);
    }
  }
}



void checkDms34(real34_t *angle34Dms) {
  int16_t  sign;
  real_t angleDms, degrees, minutes, seconds;

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



uint32_t getInfiniteComplexAngle(real_t *x, real_t *y) {
  if(!realIsInfinite(x)) {
    if(realIsPositive(y))
      return 2; // 90°
    else
      return 6; // -90° or 270°
  }

  if(!realIsInfinite(y)) {
    if(realIsPositive(x))
      return 0; // 0°
    else
      return 4; // -180° or 180°
  }

  // At this point, x and y are infinite
  if(realIsPositive(x)) {
    if(realIsPositive(y))
      return 1; // 45°
    else
      return 7; // -45° or 315°
  }

  // At this point, x is negative
  if(realIsPositive(y))
    return 3; // 135°
  else
    return 5; // -135° or 225°
}



void setInfiniteComplexAngle(uint32_t angle, real_t *x, real_t *y) {
  switch(angle) {
    case 3:
    case 4:
    case 5:  realCopy(const_minusInfinity, x);
             break;

    case 2:
    case 6:  realZero(x);
             break;

    default: realCopy(const_plusInfinity, x);
             break;
  }

  switch(angle) {
    case 5:
    case 6:
    case 7:  realCopy(const_minusInfinity, y);
             break;

    case 0:
    case 4:  realZero(y);
             break;

    default: realCopy(const_plusInfinity, y);
             break;
  }
}
