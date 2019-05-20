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
 * \file power.h
 ***********************************************/

void fnPower    (uint16_t unusedParamButMandatory);
void powError   (void);

//      RegYRegX
void powLonILonI(void);
void powLonIRe16(void);
void powLonICo16(void);
void powLonIAn16(void);
void powLonIShoI(void);
void powLonIRe34(void);
void powLonICo34(void);
void powLonIAn34(void);

//      RegYRegX
void powRe16LonI(void);
void powRe16Re16(void);
void powRe16Co16(void);
void powRe16An16(void);
void powRe16ShoI(void);
void powRe16Re34(void);
void powRe16Co34(void);
void powRe16An34(void);

//      RegYRegX
void powCo16LonI(void);
void powCo16Re16(void);
void powCo16Co16(void);
void powCo16An16(void);
void powCo16ShoI(void);
void powCo16Re34(void);
void powCo16Co34(void);
void powCo16An34(void);

//      RegYRegX
void powAn16LonI(void);
void powAn16Re16(void);
void powAn16Co16(void);
void powAn16An16(void);
void powAn16ShoI(void);
void powAn16Re34(void);
void powAn16Co34(void);
void powAn16An34(void);

//      RegYRegX
void powRm16LonI(void);
void powRm16Re16(void);
void powRm16Co16(void);
void powRm16An16(void);
void powRm16ShoI(void);
void powRm16Re34(void);
void powRm16Co34(void);
void powRm16An34(void);

//      RegYRegX
void powCm16LonI(void);
void powCm16Re16(void);
void powCm16Co16(void);
void powCm16An16(void);
void powCm16ShoI(void);
void powCm16Re34(void);
void powCm16Co34(void);
void powCm16An34(void);

//      RegYRegX
void powShoILonI(void);
void powShoIRe16(void);
void powShoICo16(void);
void powShoIAn16(void);
void powShoIShoI(void);
void powShoIRe34(void);
void powShoICo34(void);
void powShoIAn34(void);

//      RegYRegX
void powRe34LonI(void);
void powRe34Re16(void);
void powRe34Co16(void);
void powRe34An16(void);
void powRe34ShoI(void);
void powRe34Re34(void);
void powRe34Co34(void);
void powRe34An34(void);

//      RegYRegX
void powCo34LonI(void);
void powCo34Re16(void);
void powCo34Co16(void);
void powCo34An16(void);
void powCo34ShoI(void);
void powCo34Re34(void);
void powCo34Co34(void);
void powCo34An34(void);

//      RegYRegX
void powAn34LonI(void);
void powAn34Re16(void);
void powAn34Co16(void);
void powAn34An16(void);
void powAn34ShoI(void);
void powAn34Re34(void);
void powAn34Co34(void);
void powAn34An34(void);
