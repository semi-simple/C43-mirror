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



bool_t real16CompareGreaterThan(const real16_t *number1, const real16_t *number2) {
  real16_t compare;

  real16Compare(number1, number2, &compare);
  return real16ToInt32(&compare) > 0;
}



bool_t real34CompareGreaterThan(const real34_t *number1, const real34_t *number2) {
  real34_t compare;

  real34Compare(number1, number2, &compare);
  return real34ToInt32(&compare) > 0;
}



bool_t real51CompareGreaterThan(const real51_t *number1, const real51_t *number2) {
  real51_t compare;

  real51Compare(number1, number2, &compare);
  return real51ToInt32(&compare) > 0;
}



bool_t real16CompareGreaterEqual(const real16_t *number1, const real16_t *number2) {
  real16_t compare;

  real16Compare(number1, number2, &compare);
  return real16ToInt32(&compare) >= 0;
}



bool_t real34CompareGreaterEqual(const real34_t *number1, const real34_t *number2) {
  real34_t compare;

  real34Compare(number1, number2, &compare);
  return real34ToInt32(&compare) >= 0;
}



bool_t real16CompareEqual(const real16_t *number1, const real16_t *number2) {
  real16_t compare;

  real16Compare(number1, number2, &compare);
  return real16ToInt32(&compare) == 0;
}



bool_t real34CompareEqual(const real34_t *number1, const real34_t *number2) {
  real34_t compare;

  real34Compare(number1, number2, &compare);
  return real34ToInt32(&compare) == 0;
}



bool_t real51CompareEqual(const real51_t *number1, const real51_t *number2) {
  real51_t compare;

  real51Compare(number1, number2, &compare);
  return (real51ToInt32(&compare) == 0);
}



bool_t real16CompareLessEqual(const real16_t *number1, const real16_t *number2) {
  real16_t compare;

  real16Compare(number1, number2, &compare);
  return real16ToInt32(&compare) <= 0;
}



bool_t real34CompareLessEqual(const real34_t *number1, const real34_t *number2) {
  real34_t compare;

  real34Compare(number1, number2, &compare);
  return real34ToInt32(&compare) <= 0;
}



bool_t real51CompareLessEqual(const real51_t *number1, const real51_t *number2) {
  real51_t compare;

  real51Compare(number1, number2, &compare);
  return real51ToInt32(&compare) <= 0;
}



bool_t real16CompareLessThan(const real16_t *number1, const real16_t *number2) {
  real16_t compare;

  real16Compare(number1, number2, &compare);
  return real16ToInt32(&compare) < 0;
}



bool_t real34CompareLessThan(const real34_t *number1, const real34_t *number2) {
  real34_t compare;

  real34Compare(number1, number2, &compare);
  return real34ToInt32(&compare) < 0;
}



bool_t real51CompareLessThan(const real51_t *number1, const real51_t *number2) {
  real51_t compare;

  real51Compare(number1, number2, &compare);
  return real51ToInt32(&compare) < 0;
}



bool_t real16CompareAbsGreaterThan(const real16_t *number1, const real16_t *number2) {
  real16_t compare, num1, num2;

  real16Copy(number1, &num1);
  real16Copy(number2, &num2);
  real16SetPositiveSign(&num1);
  real16SetPositiveSign(&num2);
  real16Compare(&num1, &num2, &compare);
  return real16ToInt32(&compare) > 0;
}



bool_t real34CompareAbsGreaterThan(const real34_t *number1, const real34_t *number2) {
  real34_t compare, num1, num2;

  real34Copy(number1, &num1);
  real34Copy(number2, &num2);
  real34SetPositiveSign(&num1);
  real34SetPositiveSign(&num2);
  real34Compare(&num1, &num2, &compare);
  return real34ToInt32(&compare) > 0;
}



bool_t real16CompareAbsLessThan(const real16_t *number1, const real16_t *number2) {
  real16_t compare, num1, num2;

  real16Copy(number1, &num1);
  real16Copy(number2, &num2);
  real16SetPositiveSign(&num1);
  real16SetPositiveSign(&num2);
  real16Compare(&num1, &num2, &compare);
  return real16ToInt32(&compare) < 0;
}



bool_t real34CompareAbsLessThan(const real34_t *number1, const real34_t *number2) {
  real34_t compare, num1, num2;

  real34Copy(number1, &num1);
  real34Copy(number2, &num2);
  real34SetPositiveSign(&num1);
  real34SetPositiveSign(&num2);
  real34Compare(&num1, &num2, &compare);
  return real34ToInt32(&compare) < 0;
}
