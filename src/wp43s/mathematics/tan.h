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
 * \file tan.h
 ***********************************************/

void fnTan                    (uint16_t unusedParamButMandatory);
void longIntegerAngleReduction(calcRegister_t regist, uint8_t angularMode, realIc_t *reducedAngle);
void tanError                 (void);
void tanCoIc                  (const complexIc_t *zin, complexIc_t *zout);
void tanLonI                  (void);
void tanRe16                  (void);
void tanCo16                  (void);
void tanAngl                  (void);
void tanRm16                  (void);
void tanCm16                  (void);
void tanRe34                  (void);
void tanCo34                  (void);
