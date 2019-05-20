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
 * \file wp34s.h
 ***********************************************/

/******************************************************
 * This functions are borrowed from the WP34S project
 ******************************************************/

void   WP34S_cvt_2rad_sincos(real34_t *sin, real34_t *cos, const real34_t *angle, uint32_t am);
void   WP34S_sincosTaylor   (const real34_t *angle, real34_t *sinOut, real34_t *cosOut); // angle in radian
void   WP34S_do_atan        (const real34_t *x, real34_t *angle);
void   WP34S_do_atan2       (const real34_t *y, const real34_t *x, real34_t *angle);
void   WP34S_do_asin        (const real34_t *x, real34_t *angle);
void   WP34S_do_acos        (const real34_t *x, real34_t *angle);
void   WP34S_real34Factorial(const real34_t *x, real34_t *res);
void   WP34S_real34LnGamma  (const real34_t *x, real34_t *res);
void   WP34S_real34Gamma    (const real34_t *x, real34_t *res);
void   WP34S_real51Ln       (const real51_t *x, real51_t *res);
bool_t WP34S_relative_error (const real51_t *x, const real51_t *y, const real51_t *tol);
