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
 * \file conversions.h
 ***********************************************/


typedef enum {
  multiply,
  divide
} multiplyDivide_t;

// Temperature
void fnCvtCToF                 (uint16_t unusedParamButMandatory);
void fnCvtFToC                 (uint16_t unusedParamButMandatory);

// Duration
void fnCvtYearS                (uint16_t multiplyDivide);

// Energy
void fnCvtCalJ                 (uint16_t multiplyDivide);
void fnCvtBtuJ                 (uint16_t multiplyDivide);
void fnCvtWhJ                  (uint16_t multiplyDivide);

// Power
void fnCvtHpeW                 (uint16_t multiplyDivide);
void fnCvtHpukW                (uint16_t multiplyDivide);
void fnCvtHpmW                 (uint16_t multiplyDivide);

// Force
void fnCvtLbfN                 (uint16_t multiplyDivide);

// Pressure
void fnCvtBarPa                (uint16_t multiplyDivide);
void fnCvtPsiPa                (uint16_t multiplyDivide);
void fnCvtInhgPa               (uint16_t multiplyDivide);
void fnCvtTorrPa               (uint16_t multiplyDivide);
void fnCvtAtmPa                (uint16_t multiplyDivide);

// Mass
void fnCvtLbsKg                (uint16_t multiplyDivide);
void fnCvtCwtKg                (uint16_t multiplyDivide);
void fnCvtOzKg                 (uint16_t multiplyDivide);
void fnCvtStonesKg             (uint16_t multiplyDivide);
void fnCvtShortcwtKg           (uint16_t multiplyDivide);
void fnCvtTrozKg               (uint16_t multiplyDivide);
void fnCvtTonsKg               (uint16_t multiplyDivide);
void fnCvtShorttonsKg          (uint16_t multiplyDivide);

// Length
void fnCvtAuM                  (uint16_t multiplyDivide);
void fnCvtMiM                  (uint16_t multiplyDivide);
void fnCvtLyM                  (uint16_t multiplyDivide);
void fnCvtNmiM                 (uint16_t multiplyDivide);
void fnCvtFtM                  (uint16_t multiplyDivide);
void fnCvtPcM                  (uint16_t multiplyDivide);
void fnCvtInchesM              (uint16_t multiplyDivide);
void fnCvtSfeetM               (uint16_t multiplyDivide);
void fnCvtYardsM               (uint16_t multiplyDivide);
void fnCvtPointsM              (uint16_t multiplyDivide);

// Volume
void fnCvtGalukM3              (uint16_t multiplyDivide);
void fnCvtGalusM3              (uint16_t multiplyDivide);
void fnCvtFlozukM3             (uint16_t multiplyDivide);
void fnCvtFlozusM3             (uint16_t multiplyDivide);
void fnCvtQuartM3              (uint16_t multiplyDivide);

// Area
void fnCvtAcresM2              (uint16_t multiplyDivide);
void fnCvtAcresusM2            (uint16_t multiplyDivide);

// ...
void fnCvtRatioDb              (uint16_t multiplyDivide);
void fnCvtDbRatio              (uint16_t multiplyDivide);

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
