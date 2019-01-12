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


// Angle
void convertRegisterAngleFromTo(calcRegister_t regist, uint32_t fromAngularMode, uint32_t toAngularMode);
void convertRegisterToDms      (calcRegister_t regist);
void convertRegisterFromDms    (calcRegister_t regist);
void convertAngle16FromTo      (real16_t *angle16, uint32_t fromAngularMode, uint32_t toAngularMode);
void convertAngle34FromTo      (real34_t *angle34, uint32_t fromAngularMode, uint32_t toAngularMode);
void convertAngle34ToDms       (real34_t *angle34);
void convertAngle34FromDms     (real34_t *angle34);
void fnCvtFromAngularMode      (uint16_t fromAngularMode);
void fnCvtToAngularMode        (uint16_t toAngularMode);
void fnCvtDegToRad             (uint16_t unusedParamButMandatory);
void fnCvtDegToDms             (uint16_t unusedParamButMandatory);
void fnCvtRadToDeg             (uint16_t unusedParamButMandatory);
void checkDms16                (real16_t *angleDms);
void checkDms34                (real34_t *angleDms);
