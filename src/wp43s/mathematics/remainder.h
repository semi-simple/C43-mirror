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
 * \file remainder.h
 ***********************************************/

void fnRmd(uint16_t unusedParamButMandatory);
void rmdError(void);

//      RegYRegX
void rmdLonILonI(void);
void rmdLonIRe16(void);
void rmdLonIShoI(void);
void rmdLonIRe34(void);

//      RegYRegX
void rmdRe16LonI(void);
void rmdRe16Re16(void);
void rmdRe16ShoI(void);
void rmdRe16Re34(void);

//      RegYRegX
void rmdShoILonI(void);
void rmdShoIRe16(void);
void rmdShoIShoI(void);
void rmdShoIRe34(void);

//      RegYRegX
void rmdRe34LonI(void);
void rmdRe34Re16(void);
void rmdRe34ShoI(void);
void rmdRe34Re34(void);
