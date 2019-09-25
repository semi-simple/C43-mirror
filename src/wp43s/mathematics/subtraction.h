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
 * \file subtraction.h
 ***********************************************/

void fnSubtract (uint16_t unusedParamButMandatory);
void subError   (void);

//      RegYRegX
void subLonILonI(void);
void subLonIRe16(void);
void subLonICo16(void);
void subLonITime(void);
void subLonIDate(void);
void subLonIShoI(void);
void subLonIRe34(void);
void subLonICo34(void);

//      RegYRegX
void subRe16LonI(void);
void subRe16Re16(void);
void subRe16Co16(void);
void subRe16Time(void);
void subRe16Date(void);
void subRe16ShoI(void);
void subRe16Re34(void);
void subRe16Co34(void);

//      RegYRegX
void subCo16LonI(void);
void subCo16Re16(void);
void subCo16Co16(void);
void subCo16ShoI(void);
void subCo16Re34(void);
void subCo16Co34(void);

//      RegYRegX
void subTimeLonI(void);
void subTimeRe16(void);
void subTimeTime(void);
void subTimeRe34(void);

//      RegYRegX
void subDateLonI(void);
void subDateRe16(void);
void subDateDate(void);
void subDateRe34(void);

//      RegYRegX
void subRm16Rm16(void);
void subRm16Cm16(void);

//      RegYRegX
void subCm16Rm16(void);
void subCm16Cm16(void);

//      RegYRegX
void subShoILonI(void);
void subShoIRe16(void);
void subShoICo16(void);
void subShoIShoI(void);
void subShoIRe34(void);
void subShoICo34(void);

//      RegYRegX
void subRe34LonI(void);
void subRe34Re16(void);
void subRe34Co16(void);
void subRe34Time(void);
void subRe34Date(void);
void subRe34ShoI(void);
void subRe34Re34(void);
void subRe34Co34(void);

//      RegYRegX
void subCo34LonI(void);
void subCo34Re16(void);
void subCo34Co16(void);
void subCo34ShoI(void);
void subCo34Re34(void);
void subCo34Co34(void);
