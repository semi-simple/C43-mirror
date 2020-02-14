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
 * \file cpyx.h
 ***********************************************/

void fnCyx(uint16_t unusedParamButMandatory);
void fnPyx(uint16_t unusedParamButMandatory);

/*
 * When flag!=0 => Cyx
 *      flag==0 => Pyx
 */
void cpyxLonILonI(uint16_t flag);
void cpyxLonIReal(uint16_t flag);
void cpyxLonICplx(uint16_t flag);
void cpyxLonIShoI(uint16_t flag);

void cpyxRealLonI(uint16_t flag);
void cpyxRealReal(uint16_t flag);
void cpyxRealCplx(uint16_t flag);
void cpyxRealShoI(uint16_t flag);

void cpyxCplxLonI(uint16_t flag);
void cpyxCplxReal(uint16_t flag);
void cpyxCplxCplx(uint16_t flag);
void cpyxCplxShoI(uint16_t flag);

void cpyxShoILonI(uint16_t flag);
void cpyxShoIReal(uint16_t flag);
void cpyxShoICplx(uint16_t flag);
void cpyxShoIShoI(uint16_t flag);
