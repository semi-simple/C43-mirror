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
 * \file mathematics.c
 ***********************************************/

#include "wp43s.h"



bool_t real16CompareGreaterThan(const real16_t *number1, const real16_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real16CompareGreaterThan");
  #endif

  real16_t compare;

  real16Compare(number1, number2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real16CompareGreaterThan");
  #endif

  return real16ToInt32(&compare) > 0;
}



bool_t real34CompareGreaterThan(const real34_t *number1, const real34_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real34CompareGreaterThan");
  #endif

  real34_t compare;

  real34Compare(number1, number2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real34CompareGreaterThan");
  #endif

  return real34ToInt32(&compare) > 0;
}



bool_t real51CompareGreaterThan(const real51_t *number1, const real51_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real51CompareGreaterThan");
  #endif

  real51_t compare;

  real51Compare(number1, number2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real51CompareGreaterThan");
  #endif

  return real51ToInt32(&compare) > 0;
}



bool_t real16CompareGreaterEqual(const real16_t *number1, const real16_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real16CompareGreaterEqual");
  #endif

  real16_t compare;

  real16Compare(number1, number2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real16CompareGreaterEqual");
  #endif

  return real16ToInt32(&compare) >= 0;
}



bool_t real34CompareGreaterEqual(const real34_t *number1, const real34_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real34CompareGreaterEqual");
  #endif

  real34_t compare;

  real34Compare(number1, number2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real34CompareGreaterEqual");
  #endif

  return real34ToInt32(&compare) >= 0;
}



bool_t real16CompareEqual(const real16_t *number1, const real16_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real16CompareEqual");
  #endif

  real16_t compare;

  real16Compare(number1, number2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real16CompareEqual");
  #endif

  return real16ToInt32(&compare) == 0;
}



bool_t real34CompareEqual(const real34_t *number1, const real34_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real34CompareEqual");
  #endif

  real34_t compare;

  real34Compare(number1, number2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real34CompareEqual");
  #endif

  return real34ToInt32(&compare) == 0;
}



bool_t real51CompareEqual(const real51_t *number1, const real51_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real51CompareEqual");
  #endif

  real51_t compare;

  real51Compare(number1, number2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real51CompareEqual");
  #endif

  return (real51ToInt32(&compare) == 0);
}



bool_t real16CompareLessEqual(const real16_t *number1, const real16_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real16CompareLessEqual");
  #endif

  real16_t compare;

  real16Compare(number1, number2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real16CompareLessEqual");
  #endif

  return real16ToInt32(&compare) <= 0;
}



bool_t real34CompareLessEqual(const real34_t *number1, const real34_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real34CompareLessEqual");
  #endif

  real34_t compare;

  real34Compare(number1, number2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real34CompareLessEqual");
  #endif

  return real34ToInt32(&compare) <= 0;
}



bool_t real51CompareLessEqual(const real51_t *number1, const real51_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real51CompareLessEqual");
  #endif

  real51_t compare;

  real51Compare(number1, number2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real51CompareLessEqual");
  #endif

  return real51ToInt32(&compare) <= 0;
}



bool_t real16CompareLessThan(const real16_t *number1, const real16_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real16CompareLessThan");
  #endif

  real16_t compare;

  real16Compare(number1, number2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real16CompareLessThan");
  #endif

  return real16ToInt32(&compare) < 0;
}



bool_t real34CompareLessThan(const real34_t *number1, const real34_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real34CompareLessThan");
  #endif

  real34_t compare;

  real34Compare(number1, number2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real34CompareLessThan");
  #endif

  return real34ToInt32(&compare) < 0;
}



bool_t real51CompareLessThan(const real51_t *number1, const real51_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real51CompareLessThan");
  #endif

  real51_t compare;

  real51Compare(number1, number2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real51CompareLessThan");
  #endif

  return real51ToInt32(&compare) < 0;
}



bool_t real16CompareAbsGreaterThan(const real16_t *number1, const real16_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real16CompareAbsGreaterThan");
  #endif

  real16_t compare, num1, num2;

  real16Copy(number1, &num1);
  real16Copy(number2, &num2);
  real16SetPositiveSign(&num1);
  real16SetPositiveSign(&num2);
  real16Compare(&num1, &num2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real16CompareAbsGreaterThan");
  #endif

  return real16ToInt32(&compare) > 0;
}



bool_t real34CompareAbsGreaterThan(const real34_t *number1, const real34_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real34CompareAbsGreaterThan");
  #endif

  real34_t compare, num1, num2;

  real34Copy(number1, &num1);
  real34Copy(number2, &num2);
  real34SetPositiveSign(&num1);
  real34SetPositiveSign(&num2);
  real34Compare(&num1, &num2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real34CompareAbsGreaterThan");
  #endif

  return real34ToInt32(&compare) > 0;
}



bool_t real16CompareAbsLessThan(const real16_t *number1, const real16_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real16CompareAbsLessThan");
  #endif

  real16_t compare, num1, num2;

  real16Copy(number1, &num1);
  real16Copy(number2, &num2);
  real16SetPositiveSign(&num1);
  real16SetPositiveSign(&num2);
  real16Compare(&num1, &num2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real16CompareAbsLessThan");
  #endif

  return real16ToInt32(&compare) < 0;
}



bool_t real34CompareAbsLessThan(const real34_t *number1, const real34_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real34CompareAbsLessThan");
  #endif

  real34_t compare, num1, num2;

  real34Copy(number1, &num1);
  real34Copy(number2, &num2);
  real34SetPositiveSign(&num1);
  real34SetPositiveSign(&num2);
  real34Compare(&num1, &num2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real34CompareAbsLessThan");
  #endif

  return real34ToInt32(&compare) < 0;
}
