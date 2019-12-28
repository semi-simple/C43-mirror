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
void mulLonITime(void);
void mulLonIRema(void);
void mulLonICxma(void);
void mulLonIShoI(void);
void mulLonIReal(void);
void mulLonICplx(void);

//      RegYRegX
void mulTimeLonI(void);
void mulTimeShoI(void);
void mulTimeReal(void);

//      RegYRegX
void mulRemaLonI(void);
void mulRemaRema(void);
void mulRemaCxma(void);
void mulRemaShoI(void);
void mulRemaReal(void);
void mulRemaCplx(void);

//      RegYRegX
void mulCxmaLonI(void);
void mulCxmaRema(void);
void mulCxmaCxma(void);
void mulCxmaShoI(void);
void mulCxmaReal(void);
void mulCxmaCplx(void);

//      RegYRegX
void mulShoILonI(void);
void mulShoITime(void);
void mulShoIRema(void);
void mulShoICxma(void);
void mulShoIShoI(void);
void mulShoIReal(void);
void mulShoICplx(void);

//      RegYRegX
void mulRealLonI(void);
void mulRealTime(void);
void mulRealRema(void);
void mulRealCxma(void);
void mulRealShoI(void);
void mulRealReal(void);
void mulRealCplx(void);

//      RegYRegX
void mulCplxLonI(void);
void mulCplxRema(void);
void mulCplxCxma(void);
void mulCplxShoI(void);
void mulCplxReal(void);
void mulCplxCplx(void);
