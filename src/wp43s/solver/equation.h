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
 * \file equation.h
 ***********************************************/
#ifndef EQUATION_H
#define EQUATION_H

#include "typeDefinitions.h"
#include <stdint.h>

#define EQUATION_AIM_BUFFER 0xffff

#define EQUATION_NO_CURSOR  0xffff

void   fnEqNew        (uint16_t unusedButMandatoryParameter);
void   fnEqEdit       (uint16_t unusedButMandatoryParameter);
void   fnEqDelete     (uint16_t unusedButMandatoryParameter);
void   fnEqCursorLeft (uint16_t unusedButMandatoryParameter);
void   fnEqCursorRight(uint16_t unusedButMandatoryParameter);

/**
 * Stores a formula. This function reallocates memory for the formula.
 *
 * \param[in]  equationId       Equation ID
 * \param[in]  equationString   Equation in null-terminated string
 */
void   setEquation    (uint16_t equationId, const char *equationString);

/**
 * Deletes a formula. This function frees memory for the formula and shifts equation ID.
 *
 * \param[in]  equationId       Equation ID
 */
void   deleteEquation (uint16_t equationId);

/**
 * Shows a formula at the softmenu.
 *
 * \param[in]  equationId   Equation ID
 * \param[in]  startAt      From which character the formula shall be shown, in number of glyphs
 * \param[in]  cursorAt     Position of cursor, in number of glyphs, alternatively `EQUATION_NO_CURSOR` to hide cursor
 * \return                  true if the cursor is shown, false otherwise
 */
bool_t showEquation   (uint16_t equationId, uint16_t startAt, uint16_t cursorAt);

#endif // EQUATION_H
