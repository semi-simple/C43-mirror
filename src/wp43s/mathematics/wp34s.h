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

void   WP34S_Cvt2RadSinCosTan(const realIc_t *angle, uint32_t am, realIc_t *sin, realIc_t *cos, realIc_t *tan);
void   WP34S_SinCosTanTaylor (const realIc_t *angle, bool_t swap, realIc_t *sinOut, realIc_t *cosOut, realIc_t *tanOut); // angle in radian
void   WP34S_Atan            (const realIc_t *x, realIc_t *angle);
void   WP34S_Atan2           (const realIc_t *y, const realIc_t *x, realIc_t *angle);
void   WP34S_Asin            (const realIc_t *x, realIc_t *angle);
void   WP34S_Acos            (const realIc_t *x, realIc_t *angle);
void   WP34S_Factorial       (const realIc_t *x, realIc_t *res);
void   WP34S_LnGamma         (const realIc_t *x, realIc_t *res);
void   WP34S_Gamma           (const realIc_t *x, realIc_t *res);
void   WP34S_Ln              (const realIc_t *x, realIc_t *res);
void   WP34S_Log10           (const realIc_t *x, realIc_t *res);
void   WP34S_SinhCosh        (const realIc_t *x, realIc_t *sinOut, realIc_t *cosOut);
void   WP34S_Tanh            (const realIc_t *x, realIc_t *res);
void   WP34S_ArcSinh         (const realIc_t *x, realIc_t *res);
void   WP34S_ArcCosh         (const realIc_t *x, realIc_t *res);
void   WP34S_ArcTanh         (const realIc_t *x, realIc_t *res);
void   WP34S_Ln1P            (const realIc_t *x, realIc_t *res);
void   WP34S_ExpM1           (const realIc_t *x, realIc_t *res);
void   WP34S_ComplexGamma    (const complexIc_t *zin, complexIc_t *res);
void   WP34S_ComplexLnGamma  (const complexIc_t *zin, complexIc_t *res);
void   WP34S_Mod             (const realIc_t *x, const realIc_t *y, realIc_t *res);
void   WP34S_BigMod          (const realIc_t *x, const realIc_t *y, realIc_t *res);
bool_t WP34S_RelativeError   (const realIc_t *x, const realIc_t *y, const realIc_t *tol);
