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
 * \file programmableMenu.c
 ***********************************************/

#include "programming/programmableMenu.h"

#include "charString.h"
#include "debug.h"
#include "defines.h"
#include "error.h"
#include "fonts.h"
#include "items.h"
#include "programming/manage.h"
#include "programming/nextStep.h"
#include "registers.h"
#include "typeDefinitions.h"
#include "wp43s.h"

#ifdef TESTSUITE_BUILD

void fnKeyGtoXeq(uint16_t unusedButMandatoryParameter) {}
void fnKeyGto   (uint16_t unusedButMandatoryParameter) {}
void fnKeyXeq   (uint16_t unusedButMandatoryParameter) {}

#else // TESTSUITE_BUILD

static void _getStringLabelOrVariableName(uint8_t *stringAddress) {
  uint8_t stringLength = *(uint8_t *)(stringAddress++);
  xcopy(tmpStringLabelOrVariableName, stringAddress, stringLength);
  tmpStringLabelOrVariableName[stringLength] = 0;
}

static uint16_t _indirectRegister(uint8_t *paramAddress) {
  uint8_t opParam = *(uint8_t *)paramAddress;
  if(opParam <= LAST_LOCAL_REGISTER) { // Local register from .00 to .98
    int16_t realParam = indirectAddressing(opParam, false, 0, 99);
    if(realParam < 9999) return realParam;
  }
  else {
    sprintf(tmpString, "\nIn function _indirectRegister: " STD_RIGHT_ARROW " %u is not a valid parameter!", opParam);
  }
  return INVALID_VARIABLE;
}

static uint16_t _indirectVariable(uint8_t *stringAddress) {
  calcRegister_t regist;
  _getStringLabelOrVariableName(stringAddress);
  regist = findNamedVariable(tmpStringLabelOrVariableName);
  if(regist != INVALID_VARIABLE) {
    int16_t realParam = indirectAddressing(regist, false, 0, 99);
    if(realParam < 9999) return realParam;
  }
  else {
    displayCalcErrorMessage(ERROR_UNDEF_SOURCE_VAR, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "string '%s' is not a named variable", tmpStringLabelOrVariableName);
      moreInfoOnError("In function _indirectVariable:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
  return INVALID_VARIABLE;
}

static uint16_t _get2ndParamOfKey(uint8_t *paramAddress) {
  uint8_t opParam = *(uint8_t *)(paramAddress++);

  if(opParam <= 109) { // Local label from 00 to 99 or from A to J
    return opParam;
  }
  else if(opParam == STRING_LABEL_VARIABLE) {
    _getStringLabelOrVariableName(paramAddress);
    calcRegister_t label = findNamedLabel(tmpStringLabelOrVariableName);
    if(label != INVALID_VARIABLE) {
      return label;
    }
    else {
      displayCalcErrorMessage(ERROR_LABEL_NOT_FOUND, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "string '%s' is not a named label", tmpStringLabelOrVariableName);
        moreInfoOnError("In function get2ndParamOfKey:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
  }
  else if(opParam == INDIRECT_REGISTER) {
    return _indirectRegister(paramAddress);
  }
  else if(opParam == INDIRECT_VARIABLE) {
    return _indirectVariable(paramAddress);
  }
  else {
    sprintf(tmpString, "\nIn function get2ndParamOfKey: %u is not a valid parameter!", opParam);
  }
  return INVALID_VARIABLE;
}

void fnKeyGtoXeq(uint16_t keyNum) {
  uint8_t *secondParam  = findKey2ndParam(currentStep);
  uint8_t *opParam      = secondParam + 1;
  uint8_t *paramAddress = opParam + 1;
  uint16_t label        = _get2ndParamOfKey(paramAddress);

  if(*secondParam == ITM_XEQ) {
    keyXeq(keyNum, label);
  }
  else {
    keyGto(keyNum, label);
  }
}

void fnKeyGto(uint16_t unusedButMandatoryParameter) {
  keyGto(tam.key, tam.value);
}

void fnKeyXeq(uint16_t unusedButMandatoryParameter) {
  keyXeq(tam.key, tam.value);
}



void keyGto(uint16_t keyNum, uint16_t label) {
  
}



void keyXeq(uint16_t keyNum, uint16_t label) {
  
}

#endif // TESTSUITE_BUILD
