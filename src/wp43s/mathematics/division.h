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
 * \file division.h
 ***********************************************/

void fnDivide(uint16_t unusedParamButMandatory);
void divError(void);

void divCo39Co39(const complex39_t *numer, const complex39_t *denom, complex39_t *quotient);
void divCo51Co51(const complex51_t *numer, const complex51_t *denom, complex51_t *quotient);
void divCo75Co75(const complex75_t *numer, const complex75_t *denom, complex75_t *quotient);
void divRe39Co39(const real_t *numer, const complex39_t *denom, complex39_t *quotient);
void divRe51Co51(const real_t *numer, const complex51_t *denom, complex51_t *quotient);
void divRe75Co75(const real_t *numer, const complex75_t *denom, complex75_t *quotient);

//      RegYRegX
void divLonILonI(void);
void divLonIRe16(void);
void divLonICo16(void);
void divLonIShoI(void);
void divLonIRe34(void);
void divLonICo34(void);

//      RegYRegX
void divRe16LonI(void);
void divRe16Re16(void);
void divRe16Co16(void);
void divRe16ShoI(void);
void divRe16Re34(void);
void divRe16Co34(void);

//      RegYRegX
void divCo16LonI(void);
void divCo16Re16(void);
void divCo16Co16(void);
void divCo16ShoI(void);
void divCo16Re34(void);
void divCo16Co34(void);

//      RegYRegX
void divTimeLonI(void);
void divTimeRe16(void);
void divTimeShoI(void);
void divTimeRe34(void);

//      RegYRegX
void divRm16LonI(void);
void divRm16Re16(void);
void divRm16Co16(void);
void divRm16ShoI(void);
void divRm16Re34(void);
void divRm16Co34(void);

//      RegYRegX
void divCm16LonI(void);
void divCm16Re16(void);
void divCm16Co16(void);
void divCm16ShoI(void);
void divCm16Re34(void);
void divCm16Co34(void);

//      RegYRegX
void divShoILonI(void);
void divShoIRe16(void);
void divShoICo16(void);
void divShoIShoI(void);
void divShoIRe34(void);
void divShoICo34(void);

//      RegYRegX
void divRe34LonI(void);
void divRe34Re16(void);
void divRe34Co16(void);
void divRe34ShoI(void);
void divRe34Re34(void);
void divRe34Co34(void);

//      RegYRegX
void divCo34LonI(void);
void divCo34Re16(void);
void divCo34Co16(void);
void divCo34ShoI(void);
void divCo34Re34(void);
void divCo34Co34(void);
