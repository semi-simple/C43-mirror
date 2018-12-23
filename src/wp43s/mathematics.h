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

void   fnGcd                      (uint16_t unusedParamButMandatory);
void   fnLcm                      (uint16_t unusedParamButMandatory);
void   fnSquareRoot               (uint16_t unusedParamButMandatory);
void   fnMagnitude                (uint16_t unusedParamButMandatory);
void   fnLog2                     (uint16_t unusedParamButMandatory);
void   fnLog10                    (uint16_t unusedParamButMandatory);
void   fn2Pow                     (uint16_t unusedParamButMandatory);
void   fn10Pow                    (uint16_t unusedParamButMandatory);
void   fnSin                      (uint16_t unusedParamButMandatory);
void   fnCos                      (uint16_t unusedParamButMandatory);
void   fnTan                      (uint16_t unusedParamButMandatory);
void   fnArctan                   (uint16_t unusedParamButMandatory);
void   fnArcsin                   (uint16_t unusedParamButMandatory);
void   fnArccos                   (uint16_t unusedParamButMandatory);
void   fnToPolar                  (uint16_t unusedParamButMandatory);
void   fnToRect                   (uint16_t unusedParamButMandatory);
void   fnExp                      (uint16_t unusedParamButMandatory);
void   fnLn                       (uint16_t unusedParamButMandatory);
void   fnFactorial                (uint16_t unusedParamButMandatory);
void   fnLnGamma                  (uint16_t unusedParamButMandatory);
void   fnGamma                    (uint16_t unusedParamButMandatory);

void   WP34S_cvt_2rad_sincos      (real34_t *sin, real34_t *cos, calcRegister_t angle);
void   WP34S_sincosTaylor         (const real34_t *a, real34_t *sinOut, real34_t *cosOut);
void   WP34S_do_atan              (const real34_t *x, real34_t *angle);
void   WP34S_do_asin              (const real34_t *x, real34_t *angle);
void   WP34S_do_acos              (const real34_t *x, real34_t *angle);
void   WP34S_real34Factorial      (const real34_t *x, real34_t *res);
void   WP34S_real34LnGamma        (const real34_t *x, real34_t *res);
void   WP34S_real34Gamma          (const real34_t *x, real34_t *res);
void   WP34S_real51Ln             (const real51_t *x, real51_t *res);
bool_t WP34S_relative_error       (const real51_t *x, const real51_t *y, const real51_t *tol);
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
