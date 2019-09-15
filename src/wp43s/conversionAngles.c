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
      convertLongIntegerRegisterToReal16Register(REGISTER_X, REGISTER_X);
      convertAngle16FromTo(REGISTER_REAL16_DATA(REGISTER_X), fromAngularMode, currentAngularMode);
      setRegisterDataType(REGISTER_X, dtAngle16, currentAngularMode);
      break;

    case dtReal16:
      if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtToCurrentAngularMode:", "cannot use NaN as X input of fnCvtToCurrentAngularMode", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }

      convertAngle16FromTo(REGISTER_REAL16_DATA(REGISTER_X), fromAngularMode, currentAngularMode);
      setRegisterDataType(REGISTER_X, dtAngle16, currentAngularMode);
      break;

    case dtReal34:
      if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtToCurrentAngularMode:", "cannot use NaN as X input of fnCvtToCurrentAngularMode", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }

      convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), fromAngularMode, currentAngularMode);
      setRegisterDataType(REGISTER_X, dtAngle34, currentAngularMode);
      break;

    case dtAngle16:
      if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtToCurrentAngularMode:", "cannot use NaN as X input of fnCvtToCurrentAngularMode", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }

      if(getRegisterAngularMode(REGISTER_X) == fromAngularMode) {
        convertAngle16FromTo(REGISTER_REAL16_DATA(REGISTER_X), fromAngularMode, currentAngularMode);
        setRegisterDataType(REGISTER_X, dtAngle16, currentAngularMode);
      }
      else {
        displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          sprintf(errorMessage, "input angle16 must be tagged %s!", getAngularModeName(fromAngularMode));
          showInfoDialog("In function fnCvtToCurrentAngularMode:", errorMessage, NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }
      break;

    case dtAngle34:
      if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtToCurrentAngularMode:", "cannot use NaN as X input of fnCvtToCurrentAngularMode", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }

      if(getRegisterAngularMode(REGISTER_X) == fromAngularMode) {
        convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), fromAngularMode, currentAngularMode);
        setRegisterDataType(REGISTER_X, dtAngle34, currentAngularMode);
      }
      else {
        displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
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
      displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
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
      convertLongIntegerRegisterToReal16Register(REGISTER_X, REGISTER_X);
      convertAngle16FromTo(REGISTER_REAL16_DATA(REGISTER_X), currentAngularMode, toAngularMode);
      setRegisterDataType(REGISTER_X, dtAngle16, toAngularMode);
      break;

    case dtReal16:
      if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtFromCurrentAngularMode:", "cannot use NaN as X input of fnCvtFromCurrentAngularMode", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }

      convertAngle16FromTo(REGISTER_REAL16_DATA(REGISTER_X), currentAngularMode, toAngularMode);
      setRegisterDataType(REGISTER_X, dtAngle16, toAngularMode);
      break;

    case dtReal34:
      if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtFromCurrentAngularMode:", "cannot use NaN as X input of fnCvtFromCurrentAngularMode", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }

      convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), currentAngularMode, toAngularMode);
      setRegisterDataType(REGISTER_X, dtAngle34, toAngularMode);
      break;

    case dtAngle16:
      if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtFromCurrentAngularMode:", "cannot use NaN as X input of fnCvtFromCurrentAngularMode", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }

      convertAngle16FromTo(REGISTER_REAL16_DATA(REGISTER_X), getRegisterAngularMode(REGISTER_X), toAngularMode);
      setRegisterDataType(REGISTER_X, dtAngle16, toAngularMode);
      break;

    case dtAngle34:
      if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtFromCurrentAngularMode:", "cannot use NaN as X input of fnCvtFromCurrentAngularMode", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }

      convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), getRegisterAngularMode(REGISTER_X), toAngularMode);
      setRegisterDataType(REGISTER_X, dtAngle34, toAngularMode);
      break;

    default:
      displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
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
      convertLongIntegerRegisterToReal16Register(REGISTER_X, REGISTER_X);
      convertAngle16FromTo(REGISTER_REAL16_DATA(REGISTER_X), AM_DEGREE, AM_RADIAN);
      setRegisterDataType(REGISTER_X, dtAngle16, AM_RADIAN);
      break;

    case dtReal16:
      if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtDegToRad:", "cannot use NaN as X input of fnCvtDegToRad", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }

      convertAngle16FromTo(REGISTER_REAL16_DATA(REGISTER_X), AM_DEGREE, AM_RADIAN);
      setRegisterDataType(REGISTER_X, dtAngle16, AM_RADIAN);
      break;

    case dtReal34:
      if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtDegToRad:", "cannot use NaN as X input of fnCvtDegToRad", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }

      convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), AM_DEGREE, AM_RADIAN);
      setRegisterDataType(REGISTER_X, dtAngle34, AM_RADIAN);
      break;

    case dtAngle16:
      if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtDegToRad:", "cannot use NaN as X input of fnCvtDegToRad", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }

      if(getRegisterAngularMode(REGISTER_X) == AM_DEGREE) {
        convertAngle16FromTo(REGISTER_REAL16_DATA(REGISTER_X), AM_DEGREE, AM_RADIAN);
        setRegisterDataType(REGISTER_X, dtAngle16, AM_RADIAN);
      }
      else {
        displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtDegToRad:", "cannot use an angle16 not tagged degree as an input of fnCvtDegToRad", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }
      break;

    case dtAngle34:
      if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtDegToRad:", "cannot use NaN as X input of fnCvtDegToRad", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }

      if(getRegisterAngularMode(REGISTER_X) == AM_DEGREE) {
        convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), AM_DEGREE, AM_RADIAN);
        setRegisterDataType(REGISTER_X, dtAngle34, AM_RADIAN);
      }
      else {
        displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtDegToRad:", "cannot use an angle34 not tagged degree as an input of fnCvtDegToRad", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }
      break;

    default:
      displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
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
      convertLongIntegerRegisterToReal16Register(REGISTER_X, REGISTER_X);
      convertAngle16FromTo(REGISTER_REAL16_DATA(REGISTER_X), AM_RADIAN, AM_DEGREE);
      setRegisterDataType(REGISTER_X, dtAngle16, AM_DEGREE);
      break;

    case dtReal16:
      if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtRadToDeg:", "cannot use NaN as X input of fnCvtRadToDeg", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }

      convertAngle16FromTo(REGISTER_REAL16_DATA(REGISTER_X), AM_RADIAN, AM_DEGREE);
      setRegisterDataType(REGISTER_X, dtAngle16, AM_DEGREE);
      break;

    case dtReal34:
      if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtRadToDeg:", "cannot use NaN as X input of fnCvtRadToDeg", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }

      convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), AM_RADIAN, AM_DEGREE);
      setRegisterDataType(REGISTER_X, dtAngle34, AM_DEGREE);
      break;

    case dtAngle16:
      if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtRadToDeg:", "cannot use NaN as X input of fnCvtRadToDeg", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }

      if(getRegisterAngularMode(REGISTER_X) == AM_RADIAN) {
        convertAngle16FromTo(REGISTER_REAL16_DATA(REGISTER_X), AM_RADIAN, AM_DEGREE);
        setRegisterDataType(REGISTER_X, dtAngle16, AM_DEGREE);
      }
      else {
        displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtRadToDeg:", "cannot use an angle16 not tagged degree as an input of fnCvtRadToDeg", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }
      break;

    case dtAngle34:
      if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtRadToDeg:", "cannot use NaN as X input of fnCvtRadToDeg", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }

      if(getRegisterAngularMode(REGISTER_X) == AM_RADIAN) {
        convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), AM_RADIAN, AM_DEGREE);
        setRegisterDataType(REGISTER_X, dtAngle34, AM_DEGREE);
      }
      else {
        displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtRadToDeg:", "cannot use an angle34 not tagged degree as an input of fnCvtRadToDeg", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }
      break;

    default:
      displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
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
      convertLongIntegerRegisterToReal16Register(REGISTER_X, REGISTER_X);
      convertAngle16FromTo(REGISTER_REAL16_DATA(REGISTER_X), AM_DEGREE, AM_DMS);
      setRegisterDataType(REGISTER_X, dtAngle16, AM_DMS);
      break;

    case dtReal16:
      if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtDegToDms:", "cannot use NaN as X input of fnCvtDegToDms", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }

      convertAngle16FromTo(REGISTER_REAL16_DATA(REGISTER_X), AM_DEGREE, AM_DMS);
      setRegisterDataType(REGISTER_X, dtAngle16, AM_DMS);
      break;

    case dtReal34:
      if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtDegToDms:", "cannot use NaN as X input of fnCvtDegToDms", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }

      convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), AM_DEGREE, AM_DMS);
      setRegisterDataType(REGISTER_X, dtAngle34, AM_DMS);
      break;

    case dtAngle16:
      if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtDegToDms:", "cannot use NaN as X input of fnCvtDegToDms", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }

      if(getRegisterAngularMode(REGISTER_X) == AM_DEGREE) {
        convertAngle16FromTo(REGISTER_REAL16_DATA(REGISTER_X), AM_DEGREE, AM_DMS);
        setRegisterDataType(REGISTER_X, dtAngle16, AM_DMS);
      }
      else {
        displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtDegToDms:", "cannot use an angle16 not tagged degree as an input of fnCvtDegToDms", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }
      break;

    case dtAngle34:
      if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtDegToDms:", "cannot use NaN as X input of fnCvtDegToDms", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }

      if(getRegisterAngularMode(REGISTER_X) == AM_DEGREE) {
        convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), AM_DEGREE, AM_DMS);
        setRegisterDataType(REGISTER_X, dtAngle34, AM_DMS);
      }
      else {
        displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtDegToDms:", "cannot use an angle34 not tagged degree as an input of fnCvtDegToDms", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }
      break;

    default:
      displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
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
      convertLongIntegerRegisterToReal16Register(REGISTER_X, REGISTER_X);
      convertAngle16FromTo(REGISTER_REAL16_DATA(REGISTER_X), AM_DMS, AM_DEGREE);
      setRegisterDataType(REGISTER_X, dtAngle16, AM_DEGREE);
      break;

    case dtReal16:
      if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtRadToDeg:", "cannot use NaN as X input of fnCvtRadToDeg", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }

      convertAngle16FromTo(REGISTER_REAL16_DATA(REGISTER_X), AM_DMS, AM_DEGREE);
      setRegisterDataType(REGISTER_X, dtAngle16, AM_DEGREE);
      break;

    case dtReal34:
      if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtRadToDeg:", "cannot use NaN as X input of fnCvtRadToDeg", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }

      convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), AM_DMS, AM_DEGREE);
      setRegisterDataType(REGISTER_X, dtAngle34, AM_DEGREE);
      break;

    case dtAngle16:
      if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtRadToDeg:", "cannot use NaN as X input of fnCvtRadToDeg", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }

      if(getRegisterAngularMode(REGISTER_X) == AM_DMS) {
        convertAngle16FromTo(REGISTER_REAL16_DATA(REGISTER_X), AM_DMS, AM_DEGREE);
        setRegisterDataType(REGISTER_X, dtAngle16, AM_DEGREE);
      }
      else {
        displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtRadToDeg:", "cannot use an angle16 not tagged degree as an input of fnCvtRadToDeg", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }
      break;

    case dtAngle34:
      if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtRadToDeg:", "cannot use NaN as X input of fnCvtRadToDeg", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }

      if(getRegisterAngularMode(REGISTER_X) == AM_DMS) {
        convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), AM_DMS, AM_DEGREE);
        setRegisterDataType(REGISTER_X, dtAngle34, AM_DEGREE);
      }
      else {
        displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtRadToDeg:", "cannot use an angle34 not tagged degree as an input of fnCvtRadToDeg", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }
      break;

    default:
      displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
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



void convertAngle16FromTo(real16_t *angle16, uint32_t fromAngularMode, uint32_t toAngularMode) {
  realIc_t angle;

  real16ToRealIc(angle16, &angle);
  convertAngleIcFromTo(&angle, fromAngularMode, toAngularMode);
  realIcToReal16(&angle, angle16);
}



void convertAngle34FromTo(real34_t *angle34, uint32_t fromAngularMode, uint32_t toAngularMode) {
  realIc_t angle;

  real34ToRealIc(angle34, &angle);
  convertAngleIcFromTo(&angle, fromAngularMode, toAngularMode);
  realIcToReal34(&angle, angle34);
}



void convertAngleIcFromTo(realIc_t *angle, uint32_t fromAngularMode, uint32_t toAngularMode) {
  int16_t sign;
  bool_t toDms;
  realIc_t degrees, minutes, seconds;

  if(fromAngularMode == AM_DMS) {
    // Convert angle from DMS to DEGREE
    sign = realIcIsNegative(angle) ? -1 : 1;
    realIcSetPositiveSign(angle);

    realIcToIntegralValue(angle, &degrees);

    realIcSubtract(angle, &degrees, angle);
    realIcMultiply(angle, const_100, angle);

    realIcToIntegralValue(angle, &minutes);

    realIcSubtract(angle, &minutes, angle);
    realIcMultiply(angle, const_100, &seconds);

    if(realIcCompareGreaterEqual(&seconds, const_60)) {
      realIcSubtract(&seconds, const_60, &seconds);
      realIcAdd(&minutes, const_1, &minutes);
    }

    if(realIcCompareGreaterEqual(&minutes, const_60)) {
      realIcSubtract(&minutes, const_60, &minutes);
      realIcAdd(&degrees, const_1, &degrees);
    }

    realIcDivide(&minutes, const_60,   &minutes);
    realIcDivide(&seconds, const_3600, &seconds);

    realIcAdd(&degrees, &minutes, angle);
    realIcAdd(angle,    &seconds, angle);

    if(sign == -1) {
      realIcSetNegativeSign(angle);
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
        case AM_GRAD:   realIcDivide(  angle, const_9on10,   angle); break;
        case AM_RADIAN: realIcDivide(  angle, const_180onPi, angle); break;
        case AM_MULTPI: realIcDivide(  angle, const_180,     angle); break;
        default: {}
      }
      break;

    case AM_GRAD:
      switch(toAngularMode) {
        case AM_DEGREE: realIcMultiply(angle, const_9on10,   angle); break;
        case AM_RADIAN: realIcDivide(  angle, const_200onPi, angle); break;
        case AM_MULTPI: realIcDivide(  angle, const_200,     angle); break;
        default: {}
      }
      break;

    case AM_RADIAN:
      switch(toAngularMode) {
        case AM_DEGREE: realIcMultiply(angle, const_180onPi, angle); break;
        case AM_GRAD:   realIcMultiply(angle, const_200onPi, angle); break;
        case AM_MULTPI: realIcDivide(  angle, const_pi,      angle); break;
        default: {}
      }
      break;

    case AM_MULTPI:
      switch(toAngularMode) {
        case AM_DEGREE: realIcMultiply(angle, const_180,     angle); break;
        case AM_GRAD:   realIcMultiply(angle, const_200,     angle); break;
        case AM_RADIAN: realIcMultiply(angle, const_pi,      angle); break;
        default: {}
      }
      break;

    default: {}
  }

  if(toDms) {
    // Convert angle from DEGREE to DMS
    sign = realIcIsNegative(angle) ? -1 : 1;
    realIcSetPositiveSign(angle);

    realIcToIntegralValue(angle, &degrees);

    realIcSubtract(angle, &degrees, angle);
    realIcMultiply(angle, const_60, angle);

    realIcToIntegralValue(angle, &minutes);

    realIcSubtract(angle, &minutes, angle);
    realIcMultiply(angle, const_60, &seconds);

    realIcDivide(&minutes, const_100,   &minutes);
    realIcDivide(&seconds, const_10000, &seconds);

    realIcAdd(&degrees, &minutes, angle);
    realIcAdd(angle,    &seconds, angle);

    if(sign == -1) {
      realIcSetNegativeSign(angle);
    }
  }
}



void checkDms16(real16_t *angle16Dms) {
  int16_t  sign;
  realIc_t angleDms, degrees, minutes, seconds;

  real16ToRealIc(angle16Dms, &angleDms);

  sign = realIcIsNegative(&angleDms) ? -1 : 1;
  realIcSetPositiveSign(&angleDms);

  realIcToIntegralValue(&angleDms, &degrees);
  realIcSubtract(&angleDms, &degrees, &angleDms);

  realIcMultiply(&angleDms, const_100, &angleDms);
  realIcToIntegralValue(&angleDms, &minutes);
  realIcSubtract(&angleDms, &minutes, &angleDms);

  realIcMultiply(&angleDms, const_100, &seconds);

  if(realIcCompareGreaterEqual(&seconds, const_60)) {
    realIcSubtract(&seconds, const_60, &seconds);
    realIcAdd(&minutes, const_1, &minutes);
  }

  if(realIcCompareGreaterEqual(&minutes, const_60)) {
    realIcSubtract(&minutes, const_60, &minutes);
    realIcAdd(&degrees, const_1, &degrees);
  }

  realIcDivide(&minutes, const_100, &minutes);
  realIcAdd(&degrees, &minutes, &angleDms);
  realIcDivide(&seconds, const_10000, &seconds);
  realIcAdd(&angleDms, &seconds, &angleDms);

  if(sign == -1) {
    realIcSetNegativeSign(&angleDms);
  }

  realIcToReal16(&angleDms, angle16Dms);
}



void checkDms34(real34_t *angle34Dms) {
  int16_t  sign;
  realIc_t angleDms, degrees, minutes, seconds;

  real34ToRealIc(angle34Dms, &angleDms);

  sign = realIcIsNegative(&angleDms) ? -1 : 1;
  realIcSetPositiveSign(&angleDms);

  realIcToIntegralValue(&angleDms, &degrees);
  realIcSubtract(&angleDms, &degrees, &angleDms);

  realIcMultiply(&angleDms, const_100, &angleDms);
  realIcToIntegralValue(&angleDms, &minutes);
  realIcSubtract(&angleDms, &minutes, &angleDms);

  realIcMultiply(&angleDms, const_100, &seconds);

  if(realIcCompareGreaterEqual(&seconds, const_60)) {
    realIcSubtract(&seconds, const_60, &seconds);
    realIcAdd(&minutes, const_1, &minutes);
  }

  if(realIcCompareGreaterEqual(&minutes, const_60)) {
    realIcSubtract(&minutes, const_60, &minutes);
    realIcAdd(&degrees, const_1, &degrees);
  }

  realIcDivide(&minutes, const_100, &minutes);
  realIcAdd(&degrees, &minutes, &angleDms);
  realIcDivide(&seconds, const_10000, &seconds);
  realIcAdd(&angleDms, &seconds, &angleDms);

  if(sign == -1) {
    realIcSetNegativeSign(&angleDms);
  }

  realIcToReal34(&angleDms, angle34Dms);
}
