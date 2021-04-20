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

#include "mathematics/comparisonReals.h"

#include "constantPointers.h"
#include "registerValueConversions.h"

#include "wp43s.h"



bool_t real34CompareAbsGreaterThan(const real34_t *number1, const real34_t *number2) {
  real34_t num1, num2;

  real34CopyAbs(number1, &num1);
  real34CopyAbs(number2, &num2);
  real34Compare(&num1, &num2, &num2);
  return real34ToInt32(&num2) > 0;
}



bool_t real34CompareAbsLessThan(const real34_t *number1, const real34_t *number2) {
  real34_t num1, num2;

  real34CopyAbs(number1, &num1);
  real34CopyAbs(number2, &num2);
  real34Compare(&num1, &num2, &num2);
  return real34ToInt32(&num2) < 0;
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


/* never used*/ //: put back JM
bool_t real34CompareGreaterThan(const real34_t *number1, const real34_t *number2) {
  real34_t compare;

  real34Compare(number1, number2, &compare);
  return real34ToInt32(&compare) > 0;
}
/**/


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



bool_t realCompareAbsGreaterThan(const real_t *number1, const real_t *number2) {
  real_t num1, num2;
  int32_t cmp;

  realCopyAbs(number1, &num1);
  realCopyAbs(number2, &num2);
  realCompare(&num1, &num2, &num2, &ctxtReal75);
  realToInt32(&num2, cmp);
  return cmp > 0;
}


/*
bool_t realCompareAbsGreaterEqual(const real_t *number1, const real_t *number2) {
  real_t num1, num2;
  int32_t cmp;

  realCopyAbs(number1, &num1);
  realCopyAbs(number2, &num2);
  realCompare(&num1, &num2, &num2, &ctxtReal75);
  realToInt32(&num2, cmp);
  return cmp >= 0;
}
*/


bool_t realCompareAbsLessThan(const real_t *number1, const real_t *number2) {
  real_t num1, num2;
  int32_t cmp;

  realCopyAbs(number1, &num1);
  realCopyAbs(number2, &num2);
  realCompare(&num1, &num2, &num2, &ctxtReal75);
  realToInt32(&num2, cmp);
  return cmp < 0;
}



bool_t realCompareEqual(const real_t *number1, const real_t *number2) {
  real_t compare;
  int32_t cmp;

  realCompare(number1, number2, &compare, &ctxtReal75);
  realToInt32(&compare, cmp);
  return cmp == 0;
}



bool_t realCompareGreaterEqual(const real_t *number1, const real_t *number2) {
  real_t compare;
  int32_t cmp;

  realCompare(number1, number2, &compare, &ctxtReal75);
  realToInt32(&compare, cmp);
  return cmp >= 0;
}



bool_t realCompareGreaterThan(const real_t *number1, const real_t *number2) {
  real_t compare;
  int32_t cmp;

  realCompare(number1, number2, &compare, &ctxtReal75);
  realToInt32(&compare, cmp);
  return cmp > 0;
}



bool_t realCompareLessEqual(const real_t *number1, const real_t *number2) {
  real_t compare;
  int32_t cmp;

  realCompare(number1, number2, &compare, &ctxtReal75);
  realToInt32(&compare, cmp);
  return cmp <= 0;
}



bool_t realCompareLessThan(const real_t *number1, const real_t *number2) {
  real_t compare;
  int32_t cmp;

  realCompare(number1, number2, &compare, &ctxtReal75);
  realToInt32(&compare, cmp);
  return cmp < 0;
}



bool_t real34IsAnInteger(const real34_t *x) {
  real34_t y;

  if(real34IsNaN(x) || real34IsInfinite(x)) {
    return false;
  }

  real34ToIntegralValue(x, &y, DEC_ROUND_DOWN);
  real34Subtract(x, &y, &y);

  return real34CompareEqual(&y, const34_0);
}



bool_t realIsAnInteger(const real_t *x) {
  real_t y;

  if(realIsNaN(x)) {
    return false;
  }

  if(realIsInfinite(x)) {
    return true;
  }

  realToIntegralValue(x, &y, DEC_ROUND_DOWN, &ctxtReal75);
  realSubtract(x, &y, &y, &ctxtReal75);

  return realCompareEqual(&y, const_0);
}



int16_t realIdenticalDigits(real_t *a, real_t *b) {
  int16_t counter, smallest;

  if(realGetExponent(a) != realGetExponent(b)) {
    return 0;
  }

  realGetCoefficient(a, tmpString);
  realGetCoefficient(b, tmpString + TMP_STR_LENGTH/2);
  smallest = min(a->digits, b->digits);
  counter = 0;

  while(counter < smallest && tmpString[counter] == tmpString[TMP_STR_LENGTH/2 + counter]) {
    counter++;
  }

  return counter;
}
