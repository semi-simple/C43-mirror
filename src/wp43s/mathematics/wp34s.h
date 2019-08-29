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

void   WP34S_cvt_2rad_sincos(realIc_t *sin, realIc_t *cos, const realIc_t *angle, uint32_t am);
void   WP34S_sincosTaylor   (const realIc_t *angle, realIc_t *sinOut, realIc_t *cosOut); // angle in radian
void   WP34S_atan           (const realIc_t *x, realIc_t *angle);
void   WP34S_atan2          (const realIc_t *y, const realIc_t *x, realIc_t *angle);
void   WP34S_asin           (const realIc_t *x, realIc_t *angle);
void   WP34S_acos           (const realIc_t *x, realIc_t *angle);
void   WP34S_Factorial      (const realIc_t *x, realIc_t *res);
void   WP34S_LnGamma        (const realIc_t *x, realIc_t *res);
void   WP34S_Gamma          (const realIc_t *x, realIc_t *res);
void   WP34S_Ln             (const realIc_t *x, realIc_t *res);
void   WP34S_Log10          (const realIc_t *x, realIc_t *res);
bool_t WP34S_relative_error (const realIc_t *x, const realIc_t *y, const realIc_t *tol);
