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
 * \file comparisonReals.h
 ***********************************************/

bool_t real16CompareAbsGreaterThan (const real16_t *number1, const real16_t *number2);
bool_t real16CompareAbsLessThan    (const real16_t *number1, const real16_t *number2);
bool_t real16CompareEqual          (const real16_t *number1, const real16_t *number2);
bool_t real16CompareGreaterEqual   (const real16_t *number1, const real16_t *number2);
bool_t real16CompareGreaterThan    (const real16_t *number1, const real16_t *number2);
bool_t real16CompareLessEqual      (const real16_t *number1, const real16_t *number2);
bool_t real16CompareLessThan       (const real16_t *number1, const real16_t *number2);

bool_t real34CompareAbsGreaterThan (const real34_t *number1, const real34_t *number2);
bool_t real34CompareAbsGreaterEqual(const real34_t *number1, const real34_t *number2);
bool_t real34CompareAbsLessThan    (const real34_t *number1, const real34_t *number2);
bool_t real34CompareEqual          (const real34_t *number1, const real34_t *number2);
bool_t real34CompareGreaterEqual   (const real34_t *number1, const real34_t *number2);
bool_t real34CompareGreaterThan    (const real34_t *number1, const real34_t *number2);
bool_t real34CompareLessEqual      (const real34_t *number1, const real34_t *number2);
bool_t real34CompareLessThan       (const real34_t *number1, const real34_t *number2);

bool_t realIcCompareAbsGreaterThan (const realIc_t *number1, const realIc_t *number2);
bool_t realIcCompareAbsLessThan    (const realIc_t *number1, const realIc_t *number2);
bool_t realIcCompareEqual          (const realIc_t *number1, const realIc_t *number2);
bool_t realIcCompareGreaterEqual   (const realIc_t *number1, const realIc_t *number2);
bool_t realIcCompareGreaterThan    (const realIc_t *number1, const realIc_t *number2);
bool_t realIcCompareLessEqual      (const realIc_t *number1, const realIc_t *number2);
bool_t realIcCompareLessThan       (const realIc_t *number1, const realIc_t *number2);
