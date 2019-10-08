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
 * \file idiv.h
 ***********************************************/

void fnIDiv(uint16_t unusedParamButMandatory);
void idivError(void);

//      RegYRegX
void idivLonILonI(void);
void idivLonIRe16(void);
void idivLonIShoI(void);
void idivLonIRe34(void);

//      RegYRegX
void idivRe16LonI(void);
void idivRe16Re16(void);
void idivRe16ShoI(void);
void idivRe16Re34(void);

//      RegYRegX
void idivShoILonI(void);
void idivShoIRe16(void);
void idivShoIShoI(void);
void idivShoIRe34(void);

//      RegYRegX
void idivRe34LonI(void);
void idivRe34Re16(void);
void idivRe34ShoI(void);
void idivRe34Re34(void);
