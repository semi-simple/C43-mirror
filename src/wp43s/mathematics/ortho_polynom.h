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
 * \file ortho_polynom.h
 ***********************************************/
#ifndef ORTHO_POLYNOM_H
#define ORTHO_POLYNOM_H

#include <stdint.h>

void fnOrthoPoly    (uint16_t kind);
void fnHermite      (uint16_t unusedButMandatoryParameter);
void fnHermiteP     (uint16_t unusedButMandatoryParameter);
void fnLaguerre     (uint16_t unusedButMandatoryParameter);
void fnLaguerreAlpha(uint16_t unusedButMandatoryParameter);
void fnLegendre     (uint16_t unusedButMandatoryParameter);
void fnChebyshevT   (uint16_t unusedButMandatoryParameter);
void fnChebyshevU   (uint16_t unusedButMandatoryParameter);

#endif // ORTHO_POLYNOM_H
