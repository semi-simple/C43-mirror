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

void divToBeCoded(void);
void fnDivide    (uint16_t unusedParamButMandatory);
void fnInvert    (uint16_t unusedParamButMandatory);

void errorDiv    (void);

//      RegYRegX
void divBigIBigI (void);
void divBigIRe16 (void);
void divBigICo16 (void);
void divBigISmaI (void);
void divBigIRe34 (void);
void divBigICo34 (void);

//      RegYRegX
void divRe16BigI (void);
void divRe16Re16 (void);
void divRe16Co16 (void);
void divRe16SmaI (void);
void divRe16Re34 (void);
void divRe16Co34 (void);

//      RegYRegX
void divCo16BigI (void);
void divCo16Re16 (void);
void divCo16Co16 (void);
void divCo16SmaI (void);
void divCo16Re34 (void);
void divCo16Co34 (void);

//      RegYRegX
void divTimeBigI (void);
void divTimeRe16 (void);
void divTimeSmaI (void);
void divTimeRe34 (void);

//      RegYRegX
void divRm16BigI (void);
void divRm16Re16 (void);
void divRm16Co16 (void);
void divRm16SmaI (void);
void divRm16Re34 (void);
void divRm16Co34 (void);

//      RegYRegX
void divCm16BigI (void);
void divCm16Re16 (void);
void divCm16Co16 (void);
void divCm16SmaI (void);
void divCm16Re34 (void);
void divCm16Co34 (void);

//      RegYRegX
void divSmaIBigI (void);
void divSmaIRe16 (void);
void divSmaICo16 (void);
void divSmaISmaI (void);
void divSmaIRe34 (void);
void divSmaICo34 (void);

//      RegYRegX
void divRe34BigI (void);
void divRe34Re16 (void);
void divRe34Co16 (void);
void divRe34SmaI (void);
void divRe34Re34 (void);
void divRe34Co34 (void);

//      RegYRegX
void divCo34BigI (void);
void divCo34Re16 (void);
void divCo34Co16 (void);
void divCo34SmaI (void);
void divCo34Re34 (void);
void divCo34Co34 (void);
