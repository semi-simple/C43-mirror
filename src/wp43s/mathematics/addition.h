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

void addToBeCoded(void);
void fnAdd       (uint16_t unusedParamButMandatory);

void errorAdd    (void);

//      RegYRegX
void addBigIBigI (void);
void addBigIRe16 (void);
void addBigICo16 (void);
void addBigITime (void);
void addBigIDate (void);
void addBigISmaI (void);
void addBigIRe34 (void);
void addBigICo34 (void);

//      RegYRegX
void addRe16BigI (void);
void addRe16Re16 (void);
void addRe16Co16 (void);
void addRe16Time (void);
void addRe16Date (void);
void addRe16SmaI (void);
void addRe16Re34 (void);
void addRe16Co34 (void);

//      RegYRegX
void addCo16BigI (void);
void addCo16Re16 (void);
void addCo16Co16 (void);
void addCo16SmaI (void);
void addCo16Re34 (void);
void addCo16Co34 (void);

//      RegYRegX
void addTimeBigI (void);
void addTimeRe16 (void);
void addTimeTime (void);
void addTimeRe34 (void);

//      RegYRegX
void addDateBigI (void);
void addDateRe16 (void);
void addDateRe34 (void);

//      RegYRegX
void addStriBigI (void);
void addStriRe16 (void);
void addStriCo16 (void);
void addStriTime (void);
void addStriStri (void);
void addStriRm16 (void);
void addStriCm16 (void);
void addStriDate (void);
void addStriSmaI (void);
void addStriRe34 (void);
void addStriCo34 (void);

//      RegYRegX
void addRm16Rm16 (void);
void addRm16Cm16 (void);

//      RegYRegX
void addCm16Rm16 (void);
void addCm16Cm16 (void);

//      RegYRegX
void addSmaIBigI (void);
void addSmaIRe16 (void);
void addSmaICo16 (void);
void addSmaISmaI (void);
void addSmaIRe34 (void);
void addSmaICo34 (void);

//      RegYRegX
void addRe34BigI (void);
void addRe34Re16 (void);
void addRe34Co16 (void);
void addRe34Time (void);
void addRe34Date (void);
void addRe34SmaI (void);
void addRe34Re34 (void);
void addRe34Co34 (void);

//      RegYRegX
void addCo34BigI (void);
void addCo34Re16 (void);
void addCo34Co16 (void);
void addCo34SmaI (void);
void addCo34Re34 (void);
void addCo34Co34 (void);
