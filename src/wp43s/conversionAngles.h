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



#if (ANGLE16 == 1)
  #define convertAngleToInternal(angle16, toAngularMode)     convertAngle16ToInternal(angle16, toAngularMode)
  #define convertAngleFromInternal(angle16, fromAngularMode) convertAngle16FromInternal(angle16, fromAngularMode)
#endif
#if (ANGLE34 == 1)
  #define convertAngleToInternal(angle34, toAngularMode)     convertAngle34ToInternal(angle34, toAngularMode)
  #define convertAngleFromInternal(angle34, fromAngularMode) convertAngle34FromInternal(angle34, fromAngularMode)
#endif

void fnCvtToCurrentAngularMode  (uint16_t fromAngularMode);
void fnCvtFromCurrentAngularMode(uint16_t toAngularMode);
void fnCvtDegToRad              (uint16_t unusedParamButMandatory);
void fnCvtDegToDms              (uint16_t unusedParamButMandatory);
void fnCvtDmsToDeg              (uint16_t unusedParamButMandatory);
void fnCvtRadToDeg              (uint16_t unusedParamButMandatory);
void checkDms34                 (real34_t *angleDms);
void convertAngle16ToInternal   (real16_t *angle16, uint32_t toAngularMode);
void convertAngle34ToInternal   (real34_t *angle34, uint32_t toAngularMode);
void convertAngle16FromInternal (real16_t *angle16, uint32_t fromAngularMode);
void convertAngle34FromInternal (real34_t *angle34, uint32_t fromAngularMode);
