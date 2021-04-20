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
 * \file zeta.h
 ***********************************************/
#ifndef ZETA_H
#define ZETA_H

#include "decNumberWrappers.h"
#include "defines.h"
#include <stdint.h>

void fnZeta     (uint16_t unusedButMandatoryParameter);
#if (EXTRA_INFO_ON_CALC_ERROR == 1)
void zetaError  (void);
#else // (EXTRA_INFO_ON_CALC_ERROR == 1)
#define zetaError typeError
#endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
void zetaReal   (void);
void zetaCplx   (void);
void ComplexZeta(const real_t *xReal, const real_t *xImag, real_t *resReal, real_t *resImag, realContext_t *realContext);

#endif // ZETA_H
