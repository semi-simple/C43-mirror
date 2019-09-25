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
 * \file parallel.h
 ***********************************************/

void fnParallel      (uint16_t unusedParamButMandatory);
void parallelError   (void);

//      RegYRegX
void parallelLonILonI(void);
void parallelLonIRe16(void);
void parallelLonICo16(void);
void parallelLonIRe34(void);
void parallelLonICo34(void);

//      RegYRegX
void parallelRe16LonI(void);
void parallelRe16Re16(void);
void parallelRe16Co16(void);
void parallelRe16Re34(void);
void parallelRe16Co34(void);

//      RegYRegX
void parallelCo16LonI(void);
void parallelCo16Re16(void);
void parallelCo16Co16(void);
void parallelCo16Re34(void);
void parallelCo16Co34(void);

//      RegYRegX
void parallelRe34LonI(void);
void parallelRe34Re16(void);
void parallelRe34Co16(void);
void parallelRe34Re34(void);
void parallelRe34Co34(void);

//      RegYRegX
void parallelCo34LonI(void);
void parallelCo34Re16(void);
void parallelCo34Co16(void);
void parallelCo34Re34(void);
void parallelCo34Co34(void);
