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
 * \file mathematics.h
 ***********************************************/

void   fnMagnitude                (uint16_t unusedParamButMandatory);

#include "arccos.h"
#include "arcsin.h"
#include "arctan.h"
#include "cos.h"
#include "gcd.h"
#include "lcm.h"
#include "ln.h"
#include "log10.h"
#include "log2.h"
#include "exp.h"
#include "sin.h"
#include "sqrt.h"
#include "tan.h"
#include "wp34s.h"

void   fn2Pow                     (uint16_t unusedParamButMandatory);
void   fn10Pow                    (uint16_t unusedParamButMandatory);
void   fnToPolar                  (uint16_t unusedParamButMandatory);
void   fnToRect                   (uint16_t unusedParamButMandatory);

void   fnFactorial                (uint16_t unusedParamButMandatory);
void   fnLnGamma                  (uint16_t unusedParamButMandatory);
void   fnGamma                    (uint16_t unusedParamButMandatory);

void   checkDms                   (real16_t *angleDms);

bool_t real16CompareGreaterThan   (const real16_t *number1, const real16_t *number2);
bool_t real34CompareGreaterThan   (const real34_t *number1, const real34_t *number2);
bool_t real51CompareGreaterThan   (const real51_t *number1, const real51_t *number2);
bool_t real16CompareGreaterEqual  (const real16_t *number1, const real16_t *number2);
bool_t real34CompareGreaterEqual  (const real34_t *number1, const real34_t *number2);
bool_t real16CompareEqual         (const real16_t *number1, const real16_t *number2);
bool_t real34CompareEqual         (const real34_t *number1, const real34_t *number2);
bool_t real51CompareEqual         (const real51_t *number1, const real51_t *number2);
bool_t real16CompareLessEqual     (const real16_t *number1, const real16_t *number2);
bool_t real34CompareLessEqual     (const real34_t *number1, const real34_t *number2);
bool_t real51CompareLessEqual     (const real51_t *number1, const real51_t *number2);
bool_t real16CompareLessThan      (const real16_t *number1, const real16_t *number2);
bool_t real34CompareLessThan      (const real34_t *number1, const real34_t *number2);
bool_t real51CompareLessThan      (const real51_t *number1, const real51_t *number2);
bool_t real16CompareAbsGreaterThan(const real16_t *number1, const real16_t *number2);
bool_t real34CompareAbsGreaterThan(const real34_t *number1, const real34_t *number2);
bool_t real16CompareAbsLessThan   (const real16_t *number1, const real16_t *number2);
bool_t real34CompareAbsLessThan   (const real34_t *number1, const real34_t *number2);
void   real16RectangularToPolar   (const real16_t *real16,      const real16_t *imag16,  real16_t *magnitude16, real16_t *theta16);
void   real16PolarToRectangular   (const real16_t *magnitude16, const real16_t *theta16, real16_t *real16,      real16_t *imag16);
void   real34RectangularToPolar   (const real34_t *real34,      const real34_t *imag34,  real34_t *magnitude34, real34_t *theta34);
void   real34PolarToRectangular   (const real34_t *magnitude34, const real34_t *theta34, real34_t *real34,      real34_t *imag34);
