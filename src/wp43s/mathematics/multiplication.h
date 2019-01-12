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
 * \file multiplication.h
 ***********************************************/

void mulToBeCoded(void);
void fnMultiply  (uint16_t unusedParamButMandatory);

void errorMul    (void);

//      RegYRegX
void mulBigIBigI (void);
void mulBigIRe16 (void);
void mulBigICo16 (void);
void mulBigITime (void);
void mulBigIRm16 (void);
void mulBigICm16 (void);
void mulBigISmaI (void);
void mulBigIRe34 (void);
void mulBigICo34 (void);

//      RegYRegX
void mulRe16BigI (void);
void mulRe16Re16 (void);
void mulRe16Co16 (void);
void mulRe16Time (void);
void mulRe16Rm16 (void);
void mulRe16Cm16 (void);
void mulRe16SmaI (void);
void mulRe16Re34 (void);
void mulRe16Co34 (void);

//      RegYRegX
void mulCo16BigI (void);
void mulCo16Re16 (void);
void mulCo16Co16 (void);
void mulCo16Rm16 (void);
void mulCo16Cm16 (void);
void mulCo16SmaI (void);
void mulCo16Re34 (void);
void mulCo16Co34 (void);

//      RegYRegX
void mulTimeBigI (void);
void mulTimeRe16 (void);
void mulTimeSmaI (void);
void mulTimeRe34 (void);

//      RegYRegX
void mulRm16BigI (void);
void mulRm16Re16 (void);
void mulRm16Co16 (void);
void mulRm16Rm16 (void);
void mulRm16Cm16 (void);
void mulRm16SmaI (void);
void mulRm16Re34 (void);
void mulRm16Co34 (void);

//      RegYRegX
void mulCm16BigI (void);
void mulCm16Re16 (void);
void mulCm16Co16 (void);
void mulCm16Rm16 (void);
void mulCm16Cm16 (void);
void mulCm16SmaI (void);
void mulCm16Re34 (void);
void mulCm16Co34 (void);

//      RegYRegX
void mulSmaIBigI (void);
void mulSmaIRe16 (void);
void mulSmaICo16 (void);
void mulSmaITime (void);
void mulSmaIRm16 (void);
void mulSmaICm16 (void);
void mulSmaISmaI (void);
void mulSmaIRe34 (void);
void mulSmaICo34 (void);

//      RegYRegX
void mulRe34BigI (void);
void mulRe34Re16 (void);
void mulRe34Co16 (void);
void mulRe34Time (void);
void mulRe34Rm16 (void);
void mulRe34Cm16 (void);
void mulRe34SmaI (void);
void mulRe34Re34 (void);
void mulRe34Co34 (void);

//      RegYRegX
void mulCo34BigI (void);
void mulCo34Re16 (void);
void mulCo34Co16 (void);
void mulCo34Rm16 (void);
void mulCo34Cm16 (void);
void mulCo34SmaI (void);
void mulCo34Re34 (void);
void mulCo34Co34 (void);
