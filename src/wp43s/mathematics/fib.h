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
 * \file fib.h
 ***********************************************/

void fnFib   (uint16_t unusedParamButMandatory);
void fibError(void);
void fibLonI (void);
void fibReal (void);
void fibCplx (void);

void fibonacciReal(const real_t *n, real_t *res, realContext_t *realContext);
void fibonacciComplex(const real_t *nReal, const real_t *nImag, real_t *resReal, real_t *resImag, realContext_t *realContext);