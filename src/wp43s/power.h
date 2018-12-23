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

void powToBeCoded(void);
void fnPower     (uint16_t unusedParamButMandatory);

void errorPow    (void);

//      RegYRegX
void powBigIBigI (void);
void powBigIRe16 (void);
void powBigICo16 (void);
void powBigISmaI (void);
void powBigIRe34 (void);
void powBigICo34 (void);

//      RegYRegX
void powRe16BigI (void);
void powRe16Re16 (void);
void powRe16Co16 (void);
void powRe16SmaI (void);
void powRe16Re34 (void);
void powRe16Co34 (void);

//      RegYRegX
void powCo16BigI (void);
void powCo16Re16 (void);
void powCo16Co16 (void);
void powCo16SmaI (void);
void powCo16Re34 (void);
void powCo16Co34 (void);

//      RegYRegX
void powRm16BigI (void);
void powRm16Re16 (void);
void powRm16SmaI (void);
void powRm16Re34 (void);

//      RegYRegX
void powCm16BigI (void);
void powCm16Re16 (void);
void powCm16SmaI (void);
void powCm16Re34 (void);

//      RegYRegX
void powSmaIBigI (void);
void powSmaIRe16 (void);
void powSmaICo16 (void);
void powSmaISmaI (void);
void powSmaIRe34 (void);
void powSmaICo34 (void);

//      RegYRegX
void powRe34BigI (void);
void powRe34Re16 (void);
void powRe34Co16 (void);
void powRe34SmaI (void);
void powRe34Re34 (void);
void powRe34Co34 (void);

//      RegYRegX
void powCo34BigI (void);
void powCo34Re16 (void);
void powCo34Co16 (void);
void powCo34SmaI (void);
void powCo34Re34 (void);
void powCo34Co34 (void);
