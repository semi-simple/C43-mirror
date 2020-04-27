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
 * \file mean.c
 ***********************************************/

#include "wp43s.h"

int checkMinimumDataPoints(uint32_t n) {
  real_t tempReal1;

  if(statisticalSumsPointer == NULL) {
    displayCalcErrorMessage(ERROR_NO_SUMMATION_DATA, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "There is no statistical data available!");
      showInfoDialog("In function checkMinimumDataPoints:", errorMessage, NULL, NULL);
    #endif
    return 0;
  }
  uInt32ToReal(n, &tempReal1);
  if (realCompareLessEqual(SIGMA_N, &tempReal1)) {
    displayCalcErrorMessage(ERROR_NO_SUMMATION_DATA, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "There is insufficient statistical data available!");
      showInfoDialog("In function checkMinimumDataPoints:", errorMessage, NULL, NULL);
    #endif
    return 0;
  }
  return 1;
}

/********************************************//**
 * \brief x bar ==> regX, regY
 * enables stack lift and refreshes the stack.
 * regX = MEAN x, regY = MEAN y
 *
 * \param[in] displayInfo int
 * \param[in] sumX real_t
 * \param[in] numberX real_t
 * \param[in] sumY real_t
 * \param[in] numberY real_t
 * \param[in] transform void (*)(const real_t *operand, real_t *result)
 * \return void
 ***********************************************/
static void calculateMean(int displayInfo, real_t *sumX, real_t *numberX, real_t *sumY, real_t *numberY, void (*transform)(const real_t *operand, real_t *result)) {
  real_t *mean, tempReal1, tempReal2;

  if(checkMinimumDataPoints(1)) {
    saveStack();

    liftStack();
    STACK_LIFT_ENABLE;
    liftStack();

    realDivide(sumX, numberX, &tempReal1, &ctxtReal39);
    if (transform != NULL) {
      (*transform)(&tempReal1, &tempReal2);
    }
    mean = transform == NULL ? &tempReal1 : &tempReal2;
    realToReal34(mean, REGISTER_REAL34_DATA(REGISTER_X));

    realDivide(sumY, numberY, &tempReal1, &ctxtReal39);
    if (transform != NULL) {
      (*transform)(&tempReal1, &tempReal2);
    }
    realToReal34(mean, REGISTER_REAL34_DATA(REGISTER_Y));

    temporaryInformation = displayInfo;
    refreshStack();
  }
}

/********************************************//**
 * \brief x bar ==> regX, regY
 * enables stack lift and refreshes the stack.
 * regX = MEAN x, regY = MEAN y
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnMeanXY(uint16_t unusedParamButMandatory) {
  calculateMean(TI_MEANX_MEANY, SIGMA_X, SIGMA_N, SIGMA_Y, SIGMA_N, NULL);
}

static void geometricMeanTransform(const real_t *operand, real_t *result) {
  realExp(operand, result, &ctxtReal39);
}

/********************************************//**
 * \brief x bar sub G ==> regX, regY
 * enables stack lift and refreshes the stack.
 * regX = GEOMTERIC MEAN x, regY = GEOMTERIC MEAN y
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnGeometricMeanXY(uint16_t unusedParamButMandatory) {
  calculateMean(TI_GEOMMEANX_GEOMMEANY, SIGMA_lnX, SIGMA_N, SIGMA_lnY, SIGMA_N, &geometricMeanTransform);
}

/********************************************//**
 * \brief x bar sub H ==> regX, regY
 * enables stack lift and refreshes the stack.
 * regX = HARMONIC MEAN x, regY = HARMONIC MEAN y
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnHarmonicMeanXY(uint16_t unusedParamButMandatory) {
  calculateMean(TI_HARMMEANX_HARMMEANY, SIGMA_N, SIGMA_1onX, SIGMA_N, SIGMA_1onY, NULL);
}

static void RMSMeanTransform(const real_t *operand, real_t *result) {
  realSquareRoot(operand, result, &ctxtReal39);
}

/********************************************//**
 * \brief x bar sub R sub M sub S ==> regX, regY
 * enables stack lift and refreshes the stack.
 * regX = RMS MEAN x, regY = RMS MEAN y
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnRMSMeanXY(uint16_t unusedParamButMandatory) {
  calculateMean(TI_RMSMEANX_RMSMEANY, SIGMA_X2, SIGMA_N, SIGMA_Y2, SIGMA_N, &RMSMeanTransform);
}

/********************************************//**
 * \brief x bar sub W ==> regX
 * enables stack lift and refreshes the stack.
 * regX = Weighted MEAN
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnWeightedMeanX(uint16_t unusedParamButMandatory) {
  real_t mean;

  if(statisticalSumsPointer == NULL) {
    displayCalcErrorMessage(ERROR_NO_SUMMATION_DATA, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "There is no statistical data available!");
      showInfoDialog("In function fnWeightedMeanX:", errorMessage, NULL, NULL);
    #endif
  }
  else {
    saveStack();

    liftStack();
    STACK_LIFT_ENABLE;

    realDivide(SIGMA_XY, SIGMA_Y, &mean, &ctxtReal39);
    realToReal34(&mean, REGISTER_REAL34_DATA(REGISTER_X));

    temporaryInformation = TI_WEIGHTEDMEANX;
    refreshStack();
  }
}