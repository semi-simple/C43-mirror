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
 * \file conversionAngles.h
 ***********************************************/



void     fnCvtToCurrentAngularMode  (uint16_t fromAngularMode);
void     fnCvtFromCurrentAngularMode(uint16_t toAngularMode);
void     fnCvtDegToRad              (uint16_t unusedParamButMandatory);
void     fnCvtDegToDms              (uint16_t unusedParamButMandatory);
void     fnCvtDmsToDeg              (uint16_t unusedParamButMandatory);
void     fnCvtRadToDeg              (uint16_t unusedParamButMandatory);
void     convertAngle34FromTo       (real34_t *angle34, uint32_t fromAngularMode, uint32_t toAngularMode);
void     convertAngleFromTo         (real_t *angle, uint32_t fromAngularMode, uint32_t toAngularMode, realContext_t *realContext);
void     checkDms34                 (real34_t *angleDms);
uint32_t getInfiniteComplexAngle    (real_t *x, real_t *y);
void     setInfiniteComplexAngle    (uint32_t angle, real_t *x, real_t *y);
