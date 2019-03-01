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
 * \file addition.h
 ***********************************************/

void fnAdd       (uint16_t unusedParamButMandatory);
void addError    (void);
void addToBeCoded(void);

//      RegYRegX
void addLonILonI (void);
void addLonIRe16 (void);
void addLonICo16 (void);
void addLonIAngl (void);
void addLonITime (void);
void addLonIDate (void);
void addLonIShoI (void);
void addLonIRe34 (void);
void addLonICo34 (void);

//      RegYRegX
void addRe16LonI (void);
void addRe16Re16 (void);
void addRe16Co16 (void);
void addRe16Angl (void);
void addRe16Time (void);
void addRe16Date (void);
void addRe16ShoI (void);
void addRe16Re34 (void);
void addRe16Co34 (void);

//      RegYRegX
void addCo16LonI (void);
void addCo16Re16 (void);
void addCo16Co16 (void);
void addCo16ShoI (void);
void addCo16Re34 (void);
void addCo16Co34 (void);

//      RegYRegX
void addAnglLonI (void);
void addAnglRe16 (void);
void addAnglAngl (void);
void addAnglShoI (void);
void addAnglRe34 (void);

//      RegYRegX
void addTimeLonI (void);
void addTimeRe16 (void);
void addTimeTime (void);
void addTimeRe34 (void);

//      RegYRegX
void addDateLonI (void);
void addDateRe16 (void);
void addDateRe34 (void);

//      RegYRegX
void addStriLonI (void);
void addStriRe16 (void);
void addStriCo16 (void);
void addStriAngl (void);
void addStriTime (void);
void addStriStri (void);
void addStriRm16 (void);
void addStriCm16 (void);
void addStriDate (void);
void addStriShoI (void);
void addStriRe34 (void);
void addStriCo34 (void);

//      RegYRegX
void addRm16Rm16 (void);
void addRm16Cm16 (void);

//      RegYRegX
void addCm16Rm16 (void);
void addCm16Cm16 (void);

//      RegYRegX
void addShoILonI (void);
void addShoIRe16 (void);
void addShoICo16 (void);
void addShoIAngl (void);
void addShoIShoI (void);
void addShoIRe34 (void);
void addShoICo34 (void);

//      RegYRegX
void addRe34LonI (void);
void addRe34Re16 (void);
void addRe34Co16 (void);
void addRe34Angl (void);
void addRe34Time (void);
void addRe34Date (void);
void addRe34ShoI (void);
void addRe34Re34 (void);
void addRe34Co34 (void);

//      RegYRegX
void addCo34LonI (void);
void addCo34Re16 (void);
void addCo34Co16 (void);
void addCo34ShoI (void);
void addCo34Re34 (void);
void addCo34Co34 (void);
