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

void fnMultiply (uint16_t unusedParamButMandatory);
void mulError   (void);

void mulCo39Co39(const real39_t *factor1Real, const real39_t *factor1Imag, const real39_t *factor2Real, const real39_t *factor2Imag, real39_t *productReal, real39_t *productImag);
void mulCo51Co51(const real51_t *factor1Real, const real51_t *factor1Imag, const real51_t *factor2Real, const real51_t *factor2Imag, real51_t *productReal, real51_t *productImag);
void mulCo75Co75(const real75_t *factor1Real, const real75_t *factor1Imag, const real75_t *factor2Real, const real75_t *factor2Imag, real75_t *productReal, real75_t *productImag);

//      RegYRegX
void mulLonILonI(void);
void mulLonIRe16(void);
void mulLonICo16(void);
void mulLonITime(void);
void mulLonIRm16(void);
void mulLonICm16(void);
void mulLonIShoI(void);
void mulLonIRe34(void);
void mulLonICo34(void);

//      RegYRegX
void mulRe16LonI(void);
void mulRe16Re16(void);
void mulRe16Co16(void);
void mulRe16Time(void);
void mulRe16Rm16(void);
void mulRe16Cm16(void);
void mulRe16ShoI(void);
void mulRe16Re34(void);
void mulRe16Co34(void);

//      RegYRegX
void mulCo16LonI(void);
void mulCo16Re16(void);
void mulCo16Co16(void);
void mulCo16Rm16(void);
void mulCo16Cm16(void);
void mulCo16ShoI(void);
void mulCo16Re34(void);
void mulCo16Co34(void);

//      RegYRegX
void mulTimeLonI(void);
void mulTimeRe16(void);
void mulTimeShoI(void);
void mulTimeRe34(void);

//      RegYRegX
void mulRm16LonI(void);
void mulRm16Re16(void);
void mulRm16Co16(void);
void mulRm16Rm16(void);
void mulRm16Cm16(void);
void mulRm16ShoI(void);
void mulRm16Re34(void);
void mulRm16Co34(void);

//      RegYRegX
void mulCm16LonI(void);
void mulCm16Re16(void);
void mulCm16Co16(void);
void mulCm16Rm16(void);
void mulCm16Cm16(void);
void mulCm16ShoI(void);
void mulCm16Re34(void);
void mulCm16Co34(void);

//      RegYRegX
void mulShoILonI(void);
void mulShoIRe16(void);
void mulShoICo16(void);
void mulShoITime(void);
void mulShoIRm16(void);
void mulShoICm16(void);
void mulShoIShoI(void);
void mulShoIRe34(void);
void mulShoICo34(void);

//      RegYRegX
void mulRe34LonI(void);
void mulRe34Re16(void);
void mulRe34Co16(void);
void mulRe34Time(void);
void mulRe34Rm16(void);
void mulRe34Cm16(void);
void mulRe34ShoI(void);
void mulRe34Re34(void);
void mulRe34Co34(void);

//      RegYRegX
void mulCo34LonI(void);
void mulCo34Re16(void);
void mulCo34Co16(void);
void mulCo34Rm16(void);
void mulCo34Cm16(void);
void mulCo34ShoI(void);
void mulCo34Re34(void);
void mulCo34Co34(void);
