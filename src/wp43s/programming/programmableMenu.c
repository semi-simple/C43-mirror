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
#include "display.h"
#include "error.h"
#include "fonts.h"
#include "items.h"
#include "programming/lblGtoXeq.h"
#include "programming/manage.h"
#include "programming/nextStep.h"
#include "registers.h"
#include "softmenus.h"
#include "typeDefinitions.h"
#include "wp43s.h"

#ifdef TESTSUITE_BUILD

void fnKeyGtoXeq       (uint16_t unusedButMandatoryParameter) {}
void fnKeyGto          (uint16_t unusedButMandatoryParameter) {}
void fnKeyXeq          (uint16_t unusedButMandatoryParameter) {}
void fnProgrammableMenu(uint16_t unusedButMandatoryParameter) {}
void fnClearMenu       (uint16_t unusedButMandatoryParameter) {}

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
  uint16_t label        = _get2ndParamOfKey(opParam);

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



void fnProgrammableMenu(uint16_t unusedButMandatoryParameter) {
  /* Show the programmable menu */
  if(dynamicMenuItem == -1 || softmenu[softmenuStack[0].softmenuId].menuItem != -ITM_MENU) {
    showSoftmenu(-ITM_MENU);
  }

  /* Select the programmable menu */
  else if(programmableMenu.itemParam[dynamicMenuItem] != INVALID_VARIABLE) {
    int16_t prm = dynamicMenuItem;
    dynamicMenuItem = -1;
    popSoftmenu();
    runProgram(false, programmableMenu.itemParam[prm]);
  }

  /* EXIT key */
  else if(dynamicMenuItem == 20) {
    dynamicMenuItem = -1;
    popSoftmenu();
  }
}



void fnClearMenu(uint16_t unusedButMandatoryParameter) {
  for(int i = 0; i < 18; ++i) {
    programmableMenu.itemName[i][0] = 0;
    programmableMenu.itemParam[i] = INVALID_VARIABLE;
  }
  for(int i = 18; i < 21; ++i) {
    programmableMenu.itemParam[i] = INVALID_VARIABLE;
  }
}



static void _setCaption(uint16_t keyNum) {
  if(1 <= keyNum && keyNum <= 18) {
    char *ts = tmpString;
    switch(getRegisterDataType(REGISTER_K)) {
      case dtString:
        xcopy(tmpString, REGISTER_STRING_DATA(REGISTER_K), stringByteLength(REGISTER_STRING_DATA(REGISTER_K)) + 1);
        break;

      case dtLongInteger:
        longIntegerRegisterToDisplayString(REGISTER_K, tmpString, TMP_STR_LENGTH, SCREEN_WIDTH, 50, STD_SPACE_PUNCTUATION);
        break;

      case dtTime:
        timeToDisplayString(REGISTER_K, tmpString, false);
        break;

      case dtDate:
        dateToDisplayString(REGISTER_K, tmpString);
        break;

      case dtReal34Matrix:
        real34MatrixToDisplayString(REGISTER_K, tmpString);
        break;

      case dtComplex34Matrix:
        complex34MatrixToDisplayString(REGISTER_K, tmpString);
        break;

      case dtShortInteger:
        shortIntegerToDisplayString(REGISTER_K, tmpString, false);
        break;

      case dtReal34:
        real34ToDisplayString(REGISTER_REAL34_DATA(REGISTER_K), getRegisterAngularMode(REGISTER_K), tmpString, &standardFont, SCREEN_WIDTH, NUMBER_OF_DISPLAY_DIGITS, false, STD_SPACE_PUNCTUATION, true);
        break;

      case dtComplex34:
        complex34ToDisplayString(REGISTER_COMPLEX34_DATA(REGISTER_K), tmpString, &numericFont, SCREEN_WIDTH, NUMBER_OF_DISPLAY_DIGITS, false, STD_SPACE_PUNCTUATION, true);
        break;

      case dtConfig:
        xcopy(tmpString, "Configu", 8);
        break;

      default:
        tmpString[0] = 0;
    }

    for(int i = 0; i < 7 && *ts != 0; ++i) {
      ts += ((*ts) & 0x80) ? 2 : 1;
    }
    *ts = 0;

    xcopy(programmableMenu.itemName[keyNum - 1], tmpString, stringByteLength(tmpString) + 1);
  }
}



void keyGto(uint16_t keyNum, uint16_t label) {
  _setCaption(keyNum);
  programmableMenu.itemParam[keyNum - 1] = label & 0x7fff;
}



void keyXeq(uint16_t keyNum, uint16_t label) {
  _setCaption(keyNum);
  programmableMenu.itemParam[keyNum - 1] = label | 0x8000;
}

#endif // TESTSUITE_BUILD
