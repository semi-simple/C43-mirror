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
 * \file ixyz.c
 ***********************************************/

#include "mathematics/ixyz.h"

#include "constantPointers.h"
#include "debug.h"
#include "error.h"
#include "fonts.h"
#include "items.h"
#include "mathematics/comparisonReals.h"
#include "mathematics/wp34s.h"
#include "registers.h"
#include "registerValueConversions.h"
#include "stack.h"

#include "wp43s.h"



static bool_t ixyzConvert(calcRegister_t reg, real_t *val) {
  switch(getRegisterDataType(reg)) {
    case dtLongInteger:
      convertLongIntegerRegisterToReal(reg, val, &ctxtReal39);
      return true;
    case dtReal34:
      real34ToReal(REGISTER_REAL34_DATA(reg), val);
      return true;
    default:
      return false;
  }
}



void fnIxyz(uint16_t unusedButMandatoryParameter) {
  real_t x, a, b, val;

  if(!saveLastX()) return;

  if(ixyzConvert(REGISTER_X, &x) && ixyzConvert(REGISTER_Y, &a) && ixyzConvert(REGISTER_Z, &b)) {
    if(realCompareGreaterEqual(&x, const_0) && realCompareLessEqual(&x, const_1) && realCompareGreaterThan(&a, const_0) && realCompareGreaterThan(&b, const_0)) {
      WP34S_betai(&b, &a, &x, &val, &ctxtReal39);
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
      realToReal34(&val, REGISTER_REAL34_DATA(REGISTER_X));
      fnDropY(NOPARAM);
      if(lastErrorCode == ERROR_NONE) {
        fnDropY(NOPARAM);
      }
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "not in 0 " STD_LESS_EQUAL " x " STD_LESS_EQUAL " 1, a > 0, b > 0");
        moreInfoOnError("In function fnIxyz:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot calculate Ixyz for %s, %s, %s",
        getRegisterDataTypeName(REGISTER_X, true, false),
        getRegisterDataTypeName(REGISTER_Y, true, false),
        getRegisterDataTypeName(REGISTER_Z, true, false));
      moreInfoOnError("In function fnIxyz:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }

  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
}
