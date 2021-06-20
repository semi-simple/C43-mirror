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
 * \file subtraction.h
 ***********************************************/
#ifndef SUBTRACTION_H
#define SUBTRACTION_H

#include "defines.h"
#include <stdint.h>

void fnSubtract (uint16_t unusedButMandatoryParameter);
#if (EXTRA_INFO_ON_CALC_ERROR == 1)
void subError   (void);
#else // (EXTRA_INFO_ON_CALC_ERROR == 1)
#define subError typeError
#endif // (EXTRA_INFO_ON_CALC_ERROR == 1)

//      RegYRegX
void subLonILonI(void);
void subLonITime(void);
void subLonIShoI(void);
void subLonIReal(void);
void subLonICplx(void);

//      RegYRegX
void subTimeLonI(void);
void subTimeTime(void);
void subTimeReal(void);

//      RegYRegX
void subDateLonI(void);
void subDateDate(void);
void subDateReal(void);

//      RegYRegX
void subRemaRema(void);
void subRemaCxma(void);

//      RegYRegX
void subCxmaRema(void);
void subCxmaCxma(void);

//      RegYRegX
void subShoILonI(void);
void subShoIShoI(void);
void subShoIReal(void);
void subShoICplx(void);

//      RegYRegX
void subRealLonI(void);
void subRealTime(void);
void subRealShoI(void);
void subRealReal(void);
void subRealCplx(void);

//      RegYRegX
void subCplxLonI(void);
void subCplxShoI(void);
void subCplxReal(void);
void subCplxCplx(void);

#endif // SUBTRACTION_H
