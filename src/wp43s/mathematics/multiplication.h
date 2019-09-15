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

void mulCoIcCoIc(const complexIc_t *factor1, const complexIc_t *factor2, complexIc_t *product);

//      RegYRegX
void mulLonILonI(void);
void mulLonIRe16(void);
void mulLonICo16(void);
void mulLonIAn16(void);
void mulLonITime(void);
void mulLonIRm16(void);
void mulLonICm16(void);
void mulLonIShoI(void);
void mulLonIRe34(void);
void mulLonICo34(void);
void mulLonIAn34(void);

//      RegYRegX
void mulRe16LonI(void);
void mulRe16Re16(void);
void mulRe16Co16(void);
void mulRe16An16(void);
void mulRe16Time(void);
void mulRe16Rm16(void);
void mulRe16Cm16(void);
void mulRe16ShoI(void);
void mulRe16Re34(void);
void mulRe16Co34(void);
void mulRe16An34(void);

//      RegYRegX
void mulCo16LonI(void);
void mulCo16Re16(void);
void mulCo16Co16(void);
void mulCo16An16(void);
void mulCo16Rm16(void);
void mulCo16Cm16(void);
void mulCo16ShoI(void);
void mulCo16Re34(void);
void mulCo16Co34(void);
void mulCo16An34(void);

//      RegYRegX
void mulAn16LonI(void);
void mulAn16Re16(void);
void mulAn16Co16(void);
void mulAn16An16(void);
void mulAn16Time(void);
void mulAn16Rm16(void);
void mulAn16Cm16(void);
void mulAn16ShoI(void);
void mulAn16Re34(void);
void mulAn16Co34(void);
void mulAn16An34(void);

//      RegYRegX
void mulTimeLonI(void);
void mulTimeRe16(void);
void mulTimeAn16(void);
void mulTimeShoI(void);
void mulTimeRe34(void);
void mulTimeAn34(void);

//      RegYRegX
void mulRm16LonI(void);
void mulRm16Re16(void);
void mulRm16Co16(void);
void mulRm16An16(void);
void mulRm16Rm16(void);
void mulRm16Cm16(void);
void mulRm16ShoI(void);
void mulRm16Re34(void);
void mulRm16Co34(void);
void mulRm16An34(void);

//      RegYRegX
void mulCm16LonI(void);
void mulCm16Re16(void);
void mulCm16Co16(void);
void mulCm16An16(void);
void mulCm16Rm16(void);
void mulCm16Cm16(void);
void mulCm16ShoI(void);
void mulCm16Re34(void);
void mulCm16Co34(void);
void mulCm16An34(void);

//      RegYRegX
void mulShoILonI(void);
void mulShoIRe16(void);
void mulShoICo16(void);
void mulShoIAn16(void);
void mulShoITime(void);
void mulShoIRm16(void);
void mulShoICm16(void);
void mulShoIShoI(void);
void mulShoIRe34(void);
void mulShoICo34(void);
void mulShoIAn34(void);

//      RegYRegX
void mulRe34LonI(void);
void mulRe34Re16(void);
void mulRe34Co16(void);
void mulRe34An16(void);
void mulRe34Time(void);
void mulRe34Rm16(void);
void mulRe34Cm16(void);
void mulRe34ShoI(void);
void mulRe34Re34(void);
void mulRe34Co34(void);
void mulRe34An34(void);

//      RegYRegX
void mulCo34LonI(void);
void mulCo34Re16(void);
void mulCo34Co16(void);
void mulCo34An16(void);
void mulCo34Rm16(void);
void mulCo34Cm16(void);
void mulCo34ShoI(void);
void mulCo34Re34(void);
void mulCo34Co34(void);
void mulCo34An34(void);

//      RegYRegX
void mulAn34LonI(void);
void mulAn34Re16(void);
void mulAn34Co16(void);
void mulAn34An16(void);
void mulAn34Time(void);
void mulAn34Rm16(void);
void mulAn34Cm16(void);
void mulAn34ShoI(void);
void mulAn34Re34(void);
void mulAn34Co34(void);
void mulAn34An34(void);
