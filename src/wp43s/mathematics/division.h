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

void divCo39Co39(const real39_t *numerReal, const real39_t *numerImag, const real39_t *denomReal, const real39_t *denomImag, real39_t *quotientReal, real39_t *quotientImag);
void divCo51Co51(const real51_t *numerReal, const real51_t *numerImag, const real51_t *denomReal, const real51_t *denomImag, real51_t *quotientReal, real51_t *quotientImag);
void divCo75Co75(const real75_t *numerReal, const real75_t *numerImag, const real75_t *denomReal, const real75_t *denomImag, real75_t *quotientReal, real75_t *quotientImag);
void divRe39Co39(const real_t *numer, const real39_t *denomReal, const real39_t *denomImag, real39_t *quotientReal, real39_t *quotientImag);
void divRe51Co51(const real_t *numer, const real51_t *denomReal, const real51_t *denomImag, real51_t *quotientReal, real51_t *quotientImag);
void divRe75Co75(const real_t *numer, const real75_t *denomReal, const real75_t *denomImag, real75_t *quotientReal, real75_t *quotientImag);

//      RegYRegX
void divLonILonI(void);
void divLonIShoI(void);
void divLonIReal(void);
void divLonICplx(void);

//      RegYRegX
void divTimeLonI(void);
void divTimeShoI(void);
void divTimeReal(void);

//      RegYRegX
void divRemaLonI(void);
void divRemaShoI(void);
void divRemaReal(void);
void divRemaCplx(void);

//      RegYRegX
void divCxmaLonI(void);
void divCxmaShoI(void);
void divCxmaReal(void);
void divCxmaCplx(void);

//      RegYRegX
void divShoILonI(void);
void divShoIShoI(void);
void divShoIReal(void);
void divShoICplx(void);

//      RegYRegX
void divRealLonI(void);
void divRealShoI(void);
void divRealReal(void);
void divRealCplx(void);

//      RegYRegX
void divCplxLonI(void);
void divCplxShoI(void);
void divCplxReal(void);
void divCplxCplx(void);
