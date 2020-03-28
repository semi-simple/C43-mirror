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
 * \file decomp.c
 ***********************************************/

#include "wp43s.h"

static void (*const Decomp[9])(void) = {
// regX ==> 1            2           3            4            5            6            7            8            9
//          Long integer Real34      Complex34    Time         Date         String       Real34 mat   Complex34 m  Short integer
            decompError, decompReal, decompError, decompError, decompError, decompError, decompError, decompError, decompError
};


/********************************************//**
 * \brief regX ==> regL and DECOMP(regX) ==> regX, regY
 * enables stack lift and refreshes the stack.
 * Decomposes x (after converting it to an improper fraction, if applicable), returning a stack with
 * [denominator(x), numerator(x)]
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnDecomp(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  Decomp[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
  adjustResult(REGISTER_Y, false, true, REGISTER_Y, -1, -1);
}

void decompError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);

  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
  sprintf(errorMessage, "cannot calculate Decomp for %s", getRegisterDataTypeName(REGISTER_X, true, false));
  showInfoDialog("In function fnDecomp:", errorMessage, NULL, NULL);
  #endif
}

/* This function calculates the rest of  division of two real numbers.
 * result = MOD(a, b) = a % b
 */
static void modReal(real_t *a, real_t *b, real_t *result, realContext_t *realContext) {
  WP34S_Mod(a, b, result, realContext);

  if(!realIsZero(result) && realSign(b) != realSign(a)) {
    realAdd(result, a, result, realContext);
  }
}

/* This funtion calculates the GCD of two real numbers.
 * result = GCD(x, y)
 */
static void gcdReal(real_t *x, real_t *y, real_t *result, realContext_t *realContext) {
  real_t b, t;

  realCopy(y, &b);                        // b = y
  realCopy(x, result);                    // result = x

  while(!realIsZero(&b)) {                // b != 0
    realCopy(&b, &t);                     // t = b;
    modReal(result, &t, &b, realContext); // b = result mod t
    realCopy(&t, result);                 // result = t
  }
}

#define MAX_EXPANSION_ITERATIONS 1000

void decompReal(void) {
  real_t x;
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);

  if(realIsNaN(&x)) {                                                 // x == NaN
    realToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));        // Denominator = NaN
    realToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_Y));        // Numerator = NaN
  }
  else if(realIsInfinite(&x)) {                                       // x == Inf
    realToReal34(const_0, REGISTER_REAL34_DATA(REGISTER_X));          // Denominator = 0
    realToReal34(realIsNegative(&x) ? const__1 : const_1, REGISTER_REAL34_DATA(REGISTER_Y)); // Numerator = +/- 1
  }
  else {
    real_t d;                                                         // Denominator
    real_t n;                                                         // Numerator
    real_t maxD, z, dOld, t, s;

    int32ToReal(denMax, &maxD);                                       // maxD = (Real)denMax
    realCopyAbs(&x, &z);                                              // z = abs(x)

    if(denominatorMode == DM_ANY) {
      realCopy(const_0, &dOld);                                       // dOld = 0
      realCopy(const_1, &d);                                          // d = 1
      /*
       * Do a partial fraction expansion until the denominator is too large
       */
      for(int32_t i = 0; i < MAX_EXPANSION_ITERATIONS; i++) {
        realToIntegralValue(&z, &t, DEC_ROUND_DOWN, &ctxtReal39);     // t = trunc(z)
        realSubtract(&z, &t, &s, &ctxtReal39);                        // s = z - t

        if(realIsZero(&s))                                            // s == 0
          break;

        realDivide(const_1, &s, &z, &ctxtReal39);                     // z = 1/s
        realToIntegralValue(&z, &s, DEC_ROUND_DOWN, &ctxtReal39);     // s = trunc(z)
        realMultiply(&s, &d, &t, &ctxtReal39);                        // t = s * d
        realAdd(&t, &dOld, &s, &ctxtReal39);                          // s = t + dOld (s is new denominator estimate)

        if(realCompareLessThan(&maxD, &s))                            // maxD < s
          break;

        realCopy(&d, &dOld);                                          // dOld = d
        realCopy(&s, &d);                                             // d = s
      }
    }
    else
      realCopy(&maxD, &d);                                            // d = maxD

    realMultiply(&x, &d, &t, &ctxtReal39);                            // t = x * d
    realToIntegralValue(&t, &n, DEC_ROUND_HALF_UP, &ctxtReal39);      // n = round(t)

    if(denominatorMode == DM_FAC) {
      if(realIsZero(&n)) {                                            // n == 0
        realCopy(const_1, &d);                                        // d = 1
      }
      else {
        gcdReal(&n, &d, &t, &ctxtReal39);                             // t = gcd(n, d)
        realDivide(&n, &t, &n, &ctxtReal39);                          // n = n/t
        realDivide(&d, &t, &d, &ctxtReal39);                          // d = d/t
      }

    }

    convertRealToLongIntegerRegister(&n, REGISTER_Y, DEC_ROUND_DOWN); // register y = n
    convertRealToLongIntegerRegister(&d, REGISTER_X, DEC_ROUND_DOWN); // register x = d
  }
}
