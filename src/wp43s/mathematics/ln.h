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
 * \file ln.h
 ***********************************************/

void fnLn   (uint16_t unusedParamButMandatory);
void lnError(void);
void lnCo39 (const complex39_t *x, complex39_t *ln);
void lnLonI (void);
void lnRe16 (void);
void lnCo16 (void);
void lnRm16 (void);
void lnCm16 (void);
void lnShoI (void);
void lnRe34 (void);
void lnCo34 (void);
