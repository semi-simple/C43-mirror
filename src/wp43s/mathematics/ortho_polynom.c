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
 * \file ortho_polynom.c
 ***********************************************/

#include "mathematics/ortho_polynom.h"

#include "constantPointers.h"
#include "error.h"
#include "items.h"
#include "mathematics/comparisonReals.h"
#include "mathematics/wp34s.h"
#include "registers.h"
#include "registerValueConversions.h"
#include "stack.h"

#include "wp43s.h"


static bool_t getOrthoPolyParam(calcRegister_t regist, real_t *val, realContext_t *realContext) {
  switch(getRegisterDataType(regist)) {
    case dtLongInteger:
      convertLongIntegerRegisterToReal(regist, val, realContext);
      return true;
    case dtReal34:
      if(getRegisterAngularMode(regist) == amNone) {
        real34ToReal(REGISTER_REAL34_DATA(regist), val);
        return true;
      }
      /* fallthrough */
    default:
      displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, regist);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "Incompatible type for orthogonal polynomial.");
        moreInfoOnError("In function fnOrthoPoly:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      return false;
  }
}

void fnOrthoPoly(uint16_t kind) {
  real_t x, y, z, ans;

  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
  if(getOrthoPolyParam(REGISTER_X, &x, &ctxtReal39) && getOrthoPolyParam(REGISTER_Y, &y, &ctxtReal39)) {
    realCopy(const_0, &z);
    if((kind != ORTHOPOLY_LAGUERRE_L_ALPHA) || getOrthoPolyParam(REGISTER_Z, &z, &ctxtReal39)) {
      if(realIsSpecial(&y) || realIsNegative(&y) || (!realIsAnInteger(&y)) || realCompareLessEqual(&z, const__1)) {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          moreInfoOnError("In function fnOrthoPoly:", "Y must be a nonnegative integer.", kind == ORTHOPOLY_LAGUERRE_L_ALPHA ? "In addition, Z must be greater than -1." : NULL, NULL);
        #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      }
      else {
        WP34S_OrthoPoly(kind, &x, &y, &z, &ans, &ctxtReal39);
        reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
        realToReal34(&ans, REGISTER_REAL34_DATA(REGISTER_X));
        if(kind == ORTHOPOLY_LAGUERRE_L_ALPHA) fnDropY(NOPARAM);
      }
    }
  }
  adjustResult(REGISTER_X, true, false, REGISTER_X, REGISTER_Y, -1);
}

void fnHermite(uint16_t unusedButMandatoryParameter) {
  fnOrthoPoly(ORTHOPOLY_HERMITE_HE);
}
void fnHermiteP(uint16_t unusedButMandatoryParameter) {
  fnOrthoPoly(ORTHOPOLY_HERMITE_H);
}
void fnLaguerre(uint16_t unusedButMandatoryParameter) {
  fnOrthoPoly(ORTHOPOLY_LAGUERRE_L);
}
void fnLaguerreAlpha(uint16_t unusedButMandatoryParameter) {
  fnOrthoPoly(ORTHOPOLY_LAGUERRE_L_ALPHA);
}
void fnLegendre(uint16_t unusedButMandatoryParameter) {
  fnOrthoPoly(ORTHOPOLY_LEGENDRE_P);
}
void fnChebyshevT(uint16_t unusedButMandatoryParameter) {
  fnOrthoPoly(ORTHOPOLY_CHEBYSHEV_T);
}
void fnChebyshevU(uint16_t unusedButMandatoryParameter) {
  fnOrthoPoly(ORTHOPOLY_CHEBYSHEV_U);
}
