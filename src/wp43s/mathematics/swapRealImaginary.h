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
 * \file swapRealImaginary.h
 ***********************************************/
#ifndef SWAPREALIMAGINARY_H
#define SWAPREALIMAGINARY_H

#include "defines.h"
#include <stdint.h>

void fnSwapRealImaginary(uint16_t unusedButMandatoryParameter);
#if (EXTRA_INFO_ON_CALC_ERROR == 1)
void swapReImError      (void);
#else // (EXTRA_INFO_ON_CALC_ERROR == 1)
#define swapReImError typeError
#endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
void swapReImCxma       (void);
void swapReImCplx       (void);
void swapRealReal       (void);
void swapLongintReal    (void);

#endif // SWAPREALIMAGINARY_H
