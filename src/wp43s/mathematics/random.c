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
 * \file random.c
 ***********************************************/

#include "wp43s.h"

//////////////////////////////////////////////////////////
// Fast Random Integer Generation in an Interval
// DANIEL LEMIRE, Université du Québec (TELUQ), Canada
// https://arxiv.org/abs/1805.10941
uint32_t boundedRand(uint32_t s) { // random integer in [0 , s)
  uint32_t x = pcg32_random_r(&pcg32_global);
  uint64_t m = (uint64_t)x * (uint64_t)s;
  uint32_t l = (uint32_t) m;

  if(l < s) {
    uint32_t t = -s % s;
    while(l < t) {
      x = pcg32_random_r(&pcg32_global);
      m = (uint64_t) x *(uint64_t)s;
      l = (uint32_t)m;
    }
  }
  return m >> 32;
}



/////////////////////////////////////////////////////////////////////////////
// Method for pseudo random number generation: http://www.pcg-random.org/

void fnRandom(uint16_t unusedParamButMandatory) {
  real39_t x1, x2;

  saveStack();

  uInt32ToReal(boundedRand(100000000),  &x1);
  uInt32ToReal(boundedRand(100000000),  &x2);
  realFMA(const_1e8, &x1, &x2, &x1, &ctxtReal39);
  uInt32ToReal(boundedRand(1000000000), &x2);
  realFMA(const_1e9, &x1, &x2, &x1, &ctxtReal39);
  uInt32ToReal(boundedRand(1000000000), &x2);
  realFMA(const_1e9, &x1, &x2, &x1, &ctxtReal39);
  x1.exponent -= 34;

  liftStack();
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
  realToReal34(&x1, REGISTER_REAL34_DATA(REGISTER_X));

  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
  refreshRegisterLine(REGISTER_Y);
  refreshRegisterLine(REGISTER_Z);
  refreshRegisterLine(REGISTER_T);
}



void fnSeed(uint16_t unusedParamButMandatory) {
  uint64_t seed=0, sequ=0;

  if(getRegisterDataType(REGISTER_X) == dtReal34) {
    if(!real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
      seed = *(uint64_t *)getRegisterDataPointer(REGISTER_X);          // First 64 bits of the real34_t
      sequ = *(((uint64_t *)getRegisterDataPointer(REGISTER_X)) + 1);  // Last  64 bits of the real34_t
    }
  }
  else if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
    if(getRegisterLongIntegerSign(REGISTER_X) != LONG_INTEGER_ZERO) {
      seed = *(uint64_t *)getRegisterDataPointer(REGISTER_X);          // First 64 bits of the long integer
      sequ = *(((uint64_t *)getRegisterDataPointer(REGISTER_X)) + 1);  // Second 64 bits of the long integer
    }
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot use %s as a seed for the RNG!\n it must be a SP or DP real or a long integer", getDataTypeName(getRegisterDataType(REGISTER_X), true, false));
      showInfoDialog("In function fnSeed:", errorMessage, NULL, NULL);
    #endif
    return;
  }

  if(seed == 0 && sequ == 0) {
    #ifdef TESTSUITE_BUILD
      seed = 0xDeadBeef;
      sequ = 0xBadCafeFace;
    #else
      seed = (((uint64_t)getUptimeMs()) << 32) + (uint64_t)getFreeRamMemory();
      sequ = (((uint64_t)getUptimeMs()) << 32) + (uint64_t)getFreeFlash();
    #endif
  }

  pcg32_srandom(seed, sequ);
}
