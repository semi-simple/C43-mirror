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

void subToBeCoded(void);
void fnSubtract  (uint16_t unusedParamButMandatory);
void fnChangeSign(uint16_t unusedParamButMandatory);

void errorSub    (void);

//      RegYRegX
void subBigIBigI (void);
void subBigIRe16 (void);
void subBigICo16 (void);
void subBigITime (void);
void subBigIDate (void);
void subBigISmaI (void);
void subBigIRe34 (void);
void subBigICo34 (void);

//      RegYRegX
void subRe16BigI (void);
void subRe16Re16 (void);
void subRe16Co16 (void);
void subRe16Time (void);
void subRe16Date (void);
void subRe16SmaI (void);
void subRe16Re34 (void);
void subRe16Co34 (void);

//      RegYRegX
void subCo16BigI (void);
void subCo16Re16 (void);
void subCo16Co16 (void);
void subCo16SmaI (void);
void subCo16Re34 (void);
void subCo16Co34 (void);

//      RegYRegX
void subTimeBigI (void);
void subTimeRe16 (void);
void subTimeTime (void);
void subTimeRe34 (void);

//      RegYRegX
void subDateBigI (void);
void subDateRe16 (void);
void subDateDate (void);
void subDateRe34 (void);

//      RegYRegX
void subRm16Rm16 (void);
void subRm16Cm16 (void);

//      RegYRegX
void subCm16Rm16 (void);
void subCm16Cm16 (void);

//      RegYRegX
void subSmaIBigI (void);
void subSmaIRe16 (void);
void subSmaICo16 (void);
void subSmaISmaI (void);
void subSmaIRe34 (void);
void subSmaICo34 (void);

//      RegYRegX
void subRe34BigI (void);
void subRe34Re16 (void);
void subRe34Co16 (void);
void subRe34Time (void);
void subRe34Date (void);
void subRe34SmaI (void);
void subRe34Re34 (void);
void subRe34Co34 (void);

//      RegYRegX
void subCo34BigI (void);
void subCo34Re16 (void);
void subCo34Co16 (void);
void subCo34SmaI (void);
void subCo34Re34 (void);
void subCo34Co34 (void);
