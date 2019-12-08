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
 * \file ulp.c
 ***********************************************/

#include "wp43s.h"


void fnUlp(uint16_t unusedParamButMandatory) {
  real16_t x16;
  real34_t x34;
  longInteger_t lgInt;

  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  switch(getRegisterDataType(REGISTER_X)) {
    case dtLongInteger:
      longIntegerInit(lgInt);
      intToLongInteger(1, lgInt);
      convertLongIntegerToLongIntegerRegister(lgInt, REGISTER_X);
      longIntegerFree(lgInt);
      break;

    case dtReal16:
      if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsInfinite(REGISTER_REAL16_DATA(REGISTER_X))) {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnUlp:", "cannot use NaN as X or ±∞ input of ULP?", NULL, NULL);
        #endif
      }

      real16NextPlus(REGISTER_REAL16_DATA(REGISTER_X), &x16);
      if(real16IsInfinite(&x16)) {
        real16NextMinus(REGISTER_REAL16_DATA(REGISTER_X), &x16);
        real16Subtract(REGISTER_REAL16_DATA(REGISTER_X), &x16, REGISTER_REAL16_DATA(REGISTER_X));
      }
      else {
        real16Subtract(&x16, REGISTER_REAL16_DATA(REGISTER_X), REGISTER_REAL16_DATA(REGISTER_X));
      }

      setRegisterAngularMode(REGISTER_X, AM_NONE);
      break;

    case dtShortInteger:
      longIntegerInit(lgInt);
      intToLongInteger(1, lgInt);
      convertLongIntegerToShortIntegerRegister(lgInt, getRegisterShortIntegerBase(REGISTER_X), REGISTER_X);
      longIntegerFree(lgInt);
      break;

    case dtReal34:
      if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_X))) {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnUlp:", "cannot use NaN as X or ±∞ input of ULP?", NULL, NULL);
        #endif
      }

      real34NextPlus(REGISTER_REAL34_DATA(REGISTER_X), &x34);
      if(real34IsInfinite(&x34)) {
        real34NextMinus(REGISTER_REAL34_DATA(REGISTER_X), &x34);
        real34Subtract(REGISTER_REAL34_DATA(REGISTER_X), &x34, REGISTER_REAL34_DATA(REGISTER_X));
      }
      else {
        real34Subtract(&x34, REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
      }

      setRegisterAngularMode(REGISTER_X, AM_NONE);
      break;

    default:
      displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "cannot calculate ULP? with %s in X", getRegisterDataTypeName(REGISTER_X, true, false));
        showInfoDialog("In function fnUlp:", errorMessage, NULL, NULL);
      #endif
      return;
  }

  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
}
