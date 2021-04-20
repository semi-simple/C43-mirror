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
 * \file arctan.h
 ***********************************************/
#ifndef ARCTAN_H
#define ARCTAN_H

#include "decNumberWrappers.h"
#include "defines.h"
#include <stdint.h>

void fnArctan   (uint16_t unusedButMandatoryParameter);
#if (EXTRA_INFO_ON_CALC_ERROR == 1)
void arctanError(void);
#else // (EXTRA_INFO_ON_CALC_ERROR == 1)
#define arctanError typeError
#endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
void arctanLonI (void);
void arctanRema (void);
void arctanCxma (void);
void arctanReal (void);
void arctanCplx (void);

uint8_t ArctanComplex(real_t *xReal, real_t *xImag, real_t *rReal, real_t *rImag, realContext_t *realContext);

#endif // ARCTAN_H
