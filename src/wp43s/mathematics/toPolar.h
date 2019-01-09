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
 * \file toPolar.h
 ***********************************************/



void fnToPolar               (uint16_t unusedParamButMandatory);
void real16RectangularToPolar(const real16_t *real16, const real16_t *imag16,  real16_t *magnitude16, real16_t *theta16);
void real34RectangularToPolar(const real34_t *real34, const real34_t *imag34,  real34_t *magnitude34, real34_t *theta34);
