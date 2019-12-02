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
 * \file comparisonReals.c
 ***********************************************/

#include "wp43s.h"



bool_t real16CompareAbsGreaterThan(const real16_t *number1, const real16_t *number2) {
  real16_t compare, num1, num2;

  real16CopyAbs(number1, &num1);
  real16CopyAbs(number2, &num2);
  real16Compare(&num1, &num2, &compare);
  return real16ToInt32(&compare) > 0;
}



bool_t real16CompareAbsLessThan(const real16_t *number1, const real16_t *number2) {
  real16_t compare, num1, num2;

  real16CopyAbs(number1, &num1);
  real16CopyAbs(number2, &num2);
  real16Compare(&num1, &num2, &compare);
  return real16ToInt32(&compare) < 0;
}



bool_t real16CompareEqual(const real16_t *number1, const real16_t *number2) {
  real16_t compare;

  real16Compare(number1, number2, &compare);
  return real16ToInt32(&compare) == 0;
}



bool_t real16CompareGreaterEqual(const real16_t *number1, const real16_t *number2) {
  real16_t compare;

  real16Compare(number1, number2, &compare);
  return real16ToInt32(&compare) >= 0;
}



bool_t real16CompareGreaterThan(const real16_t *number1, const real16_t *number2) {
  real16_t compare;

  real16Compare(number1, number2, &compare);
  return real16ToInt32(&compare) > 0;
}



bool_t real16CompareLessEqual(const real16_t *number1, const real16_t *number2) {
  real16_t compare;

  real16Compare(number1, number2, &compare);
  return real16ToInt32(&compare) <= 0;
}



bool_t real16CompareLessThan(const real16_t *number1, const real16_t *number2) {
  real16_t compare;

  real16Compare(number1, number2, &compare);
  return real16ToInt32(&compare) < 0;
}



bool_t real34CompareAbsGreaterThan(const real34_t *number1, const real34_t *number2) {
  real34_t compare, num1, num2;

  real34CopyAbs(number1, &num1);
  real34CopyAbs(number2, &num2);
  real34Compare(&num1, &num2, &compare);
  return real34ToInt32(&compare) > 0;
}



bool_t real34CompareAbsLessThan(const real34_t *number1, const real34_t *number2) {
  real34_t compare, num1, num2;

  real34CopyAbs(number1, &num1);
  real34CopyAbs(number2, &num2);
  real34Compare(&num1, &num2, &compare);
  return real34ToInt32(&compare) < 0;
}



bool_t real34CompareEqual(const real34_t *number1, const real34_t *number2) {
  real34_t compare;

  real34Compare(number1, number2, &compare);
  return real34ToInt32(&compare) == 0;
}



bool_t real34CompareGreaterEqual(const real34_t *number1, const real34_t *number2) {
  real34_t compare;

  real34Compare(number1, number2, &compare);
  return real34ToInt32(&compare) >= 0;
}



bool_t real34CompareGreaterThan(const real34_t *number1, const real34_t *number2) {
  real34_t compare;

  real34Compare(number1, number2, &compare);
  return real34ToInt32(&compare) > 0;
}



bool_t real34CompareLessEqual(const real34_t *number1, const real34_t *number2) {
  real34_t compare;

  real34Compare(number1, number2, &compare);
  return real34ToInt32(&compare) <= 0;
}



bool_t real34CompareLessThan(const real34_t *number1, const real34_t *number2) {
  real34_t compare;

  real34Compare(number1, number2, &compare);
  return real34ToInt32(&compare) < 0;
}



bool_t real39CompareAbsGreaterThan(const real_t *number1, const real_t *number2) {
  real39_t compare, num1, num2;
  int32_t cmp;

  realCopyAbs(number1, &num1);
  realCopyAbs(number2, &num2);
  realCompare(&num1, &num2, &compare, &ctxtReal39);
  realToInt32(&compare, cmp);
  return cmp > 0;
}



bool_t real39CompareAbsGreaterEqual(const real_t *number1, const real_t *number2) {
  real39_t compare, num1, num2;
  int32_t cmp;

  realCopyAbs(number1, &num1);
  realCopyAbs(number2, &num2);
  realCompare(&num1, &num2, &compare, &ctxtReal39);
  realToInt32(&compare, cmp);
  return cmp >= 0;
}



bool_t real39CompareAbsLessThan(const real_t *number1, const real_t *number2) {
  real39_t compare, num1, num2;
  int32_t cmp;

  realCopyAbs(number1, &num1);
  realCopyAbs(number2, &num2);
  realCompare(&num1, &num2, &compare, &ctxtReal39);
  realToInt32(&compare, cmp);
  return cmp < 0;
}



bool_t real39CompareEqual(const real_t *number1, const real_t *number2) {
  real39_t compare;
  int32_t cmp;

  realCompare(number1, number2, &compare, &ctxtReal39);
  realToInt32(&compare, cmp);
  return cmp == 0;
}



bool_t real39CompareGreaterEqual(const real_t *number1, const real_t *number2) {
  real39_t compare;
  int32_t cmp;

  realCompare(number1, number2, &compare, &ctxtReal39);
  realToInt32(&compare, cmp);
  return cmp >= 0;
}



bool_t real39CompareGreaterThan(const real_t *number1, const real_t *number2) {
  real39_t compare;
  int32_t cmp;

  realCompare(number1, number2, &compare, &ctxtReal39);
  realToInt32(&compare, cmp);
  return cmp > 0;
}



bool_t real39CompareLessEqual(const real_t *number1, const real_t *number2) {
  real39_t compare;
  int32_t cmp;

  realCompare(number1, number2, &compare, &ctxtReal39);
  realToInt32(&compare, cmp);
  return cmp <= 0;
}



bool_t real39CompareLessThan(const real_t *number1, const real_t *number2) {
  real39_t compare;
  int32_t cmp;

  realCompare(number1, number2, &compare, &ctxtReal39);
  realToInt32(&compare, cmp);
  return cmp < 0;
}



bool_t real16IsAnInteger(const real16_t *x) {
  real16_t r, y;

  if(real16IsNaN(x)) {
    return false;
  }

  if(real16IsInfinite(x)) {
    return true;
  }

  real16ToIntegralValue(x, &y, DEC_ROUND_DOWN);
  real16Subtract(x, &y, &r);

  return real16CompareEqual(&r, const16_0);
}



bool_t real34IsAnInteger(const real34_t *x) {
  real34_t r, y;

  if(real34IsNaN(x)) {
    return false;
  }

  if(real34IsInfinite(x)) {
    return true;
  }

  real34ToIntegralValue(x, &y, DEC_ROUND_DOWN);
  real34Subtract(x, &y, &r);

  return real34CompareEqual(&r, const34_0);
}



bool_t real39IsAnInteger(const real_t *x) {
  real39_t r, y;

  if(realIsNaN(x)) {
    return false;
  }

  if(realIsInfinite(x)) {
    return true;
  }

  realToIntegralValue(x, &y, DEC_ROUND_DOWN, &ctxtReal39);
  realSubtract(x, &y, &r, &ctxtReal39);

  return real39CompareEqual(&r, const_0);
}
