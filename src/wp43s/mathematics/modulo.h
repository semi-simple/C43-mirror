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
 * \file modulo.h
 ***********************************************/

void fnMod(uint16_t unusedParamButMandatory);
void modError(void);

//      RegYRegX
void modLonILonI(void);
void modLonIRe16(void);
void modLonIShoI(void);
void modLonIRe34(void);

//      RegYRegX
void modRe16LonI(void);
void modRe16Re16(void);
void modRe16ShoI(void);
void modRe16Re34(void);

//      RegYRegX
void modShoILonI(void);
void modShoIRe16(void);
void modShoIShoI(void);
void modShoIRe34(void);

//      RegYRegX
void modRe34LonI(void);
void modRe34Re16(void);
void modRe34ShoI(void);
void modRe34Re34(void);