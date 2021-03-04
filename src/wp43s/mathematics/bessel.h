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
 * \file bessel.h
 ***********************************************/

void fnBessel            (uint16_t unusedButMandatoryParameter);
void WP34S_BesselJ       (const real_t *alpha, const real_t *x, real_t *res, realContext_t *realContext);
void WP34S_ComplexBesselJ(const real_t *alphax, const real_t *alphay, const real_t *xx, const real_t *xy, real_t *rx, real_t *ry, realContext_t *realContext);
