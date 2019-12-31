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
 * \file max.c
 ***********************************************/

#include "wp43s.h"



void fnMax(uint16_t unusedParamButMandatory) {
  real39_t x, y;
  longInteger_t lgIntX, lgIntY;


  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  switch(getRegisterDataType(REGISTER_X)) {
    case dtLongInteger:
      switch(getRegisterDataType(REGISTER_Y)) {
        case dtLongInteger:
          // X = long integer    Y = long integer
          convertLongIntegerRegisterToLongInteger(REGISTER_X, lgIntX);
          convertLongIntegerRegisterToLongInteger(REGISTER_Y, lgIntY);
          if(longIntegerCompare(lgIntY, lgIntX) > 0) {
            copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_X);
          }
          longIntegerFree(lgIntX);
          longIntegerFree(lgIntY);
          break;

        case dtShortInteger:
          // X = long integer    Y = short integer
          convertLongIntegerRegisterToLongInteger(REGISTER_X, lgIntX);
          convertShortIntegerRegisterToLongInteger(REGISTER_Y, lgIntY);
          if(longIntegerCompare(lgIntY, lgIntX) > 0) {
            copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_X);
          }
          longIntegerFree(lgIntX);
          longIntegerFree(lgIntY);
          break;

        case dtReal34:
          // X = long integer    Y = real34
          convertLongIntegerRegisterToReal(REGISTER_X, (real_t *)&x, &ctxtReal39);
          real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
          if(realCompareGreaterThan(&y, &x)) {
            copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_X);
          }
          break;

        default:
          displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
          #if (EXTRA_INFO_ON_CALC_ERROR == 1)
            sprintf(errorMessage, "cannot get the max from %s", getRegisterDataTypeName(REGISTER_X, true, false));
            sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "and %s", getRegisterDataTypeName(REGISTER_Y, true, false));
            showInfoDialog("In function fnMax:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
          #endif
      }
      break;

    case dtTime:
      if(getRegisterDataType(REGISTER_Y) == dtTime) {
        fnToBeCoded();
      }
      else {
        displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          sprintf(errorMessage, "cannot get the max from %s", getRegisterDataTypeName(REGISTER_X, true, false));
          sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "and %s", getRegisterDataTypeName(REGISTER_Y, true, false));
          showInfoDialog("In function fnMax:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
        #endif
      }
      break;

    case dtDate:
      if(getRegisterDataType(REGISTER_Y) == dtDate) {
        fnToBeCoded();
      }
      else {
        displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          sprintf(errorMessage, "cannot get the max from %s", getRegisterDataTypeName(REGISTER_X, true, false));
          sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "and %s", getRegisterDataTypeName(REGISTER_Y, true, false));
          showInfoDialog("In function fnMax:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
        #endif
      }
      break;

    case dtString:
      if(getRegisterDataType(REGISTER_Y) == dtString) {
        if(compareString(REGISTER_STRING_DATA(REGISTER_Y), REGISTER_STRING_DATA(REGISTER_X), CMP_EXTENSIVE) > 0) {
          copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_X);
        }
      }
      else {
        displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          sprintf(errorMessage, "cannot get the max from %s", getRegisterDataTypeName(REGISTER_X, true, false));
          sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "and %s", getRegisterDataTypeName(REGISTER_Y, true, false));
          showInfoDialog("In function fnMax:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
        #endif
      }
      break;

    case dtShortInteger:
      switch(getRegisterDataType(REGISTER_Y)) {
        case dtLongInteger:
          // X = short integer    Y = long integer
          convertShortIntegerRegisterToLongInteger(REGISTER_X, lgIntX);
          convertLongIntegerRegisterToLongInteger(REGISTER_Y, lgIntY);
          if(longIntegerCompare(lgIntY, lgIntX) > 0) {
            copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_X);
          }
          longIntegerFree(lgIntX);
          longIntegerFree(lgIntY);
          break;

        case dtShortInteger:
          // X = short integer    Y = short integer
          convertShortIntegerRegisterToLongInteger(REGISTER_X, lgIntX);
          convertShortIntegerRegisterToLongInteger(REGISTER_Y, lgIntY);
          if(longIntegerCompare(lgIntY, lgIntX) > 0) {
            copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_X);
          }
          longIntegerFree(lgIntX);
          longIntegerFree(lgIntY);
          break;

        case dtReal34:
          // X = short integer    Y = real34
          convertShortIntegerRegisterToReal(REGISTER_X, (real_t *)&x, &ctxtReal39);
          real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
          if(realCompareGreaterThan(&y, &x)) {
            copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_X);
          }
          break;

        default:
          displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
          #if (EXTRA_INFO_ON_CALC_ERROR == 1)
            sprintf(errorMessage, "cannot get the max from %s", getRegisterDataTypeName(REGISTER_X, true, false));
            sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "and %s", getRegisterDataTypeName(REGISTER_Y, true, false));
            showInfoDialog("In function fnMax:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
          #endif
      }
      break;

    case dtReal34:
      switch(getRegisterDataType(REGISTER_Y)) {
        case dtLongInteger:
          // X = real34    Y = long integer
          real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
          convertLongIntegerRegisterToReal(REGISTER_Y, (real_t *)&y, &ctxtReal39);
          if(realCompareGreaterThan(&y, &x)) {
            copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_X);
          }
          break;

        case dtShortInteger:
          // X = real34    Y = short integer
          real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
          convertShortIntegerRegisterToReal(REGISTER_Y, (real_t *)&y, &ctxtReal39);
          if(realCompareGreaterThan(&y, &x)) {
            copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_X);
          }
          break;

        case dtReal34:
          // X = real34    Y = real34
          if(real34CompareGreaterThan(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X))) {
            copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_X);
          }
          break;

        default:
          displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
          #if (EXTRA_INFO_ON_CALC_ERROR == 1)
            sprintf(errorMessage, "cannot get the max from %s", getRegisterDataTypeName(REGISTER_X, true, false));
            sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "and %s", getRegisterDataTypeName(REGISTER_Y, true, false));
            showInfoDialog("In function fnMax:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
          #endif
      }
      break;

    default:
      displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "cannot get the max from %s", getRegisterDataTypeName(REGISTER_X, true, false));
        sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "and %s", getRegisterDataTypeName(REGISTER_Y, true, false));
        showInfoDialog("In function fnMax:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
      #endif
  }

  adjustResult(REGISTER_X, true, false, REGISTER_X, -1, -1);
}
