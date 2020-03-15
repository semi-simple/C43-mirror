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
 * \file stats.c Statistical functions
 ***********************************************/

#include "wp43s.h"
#define real34Ln(operand, res) {real34ToReal((real34_t *)operand, &real39); WP34S_Ln(&real39, &real39); realToReal34(&real39, (real34_t *)res);}



static void initStatisticalSums(void) {
  if(statisticalSumsPointer == NULL) {
    statisticalSumsPointer = allocWp43s(NUMBER_OF_STATISTICAL_SUMS * REAL34_SIZE);
    for(int32_t sum=0; sum<NUMBER_OF_STATISTICAL_SUMS; sum++) {
      real34Zero((real34_t *)(statisticalSumsPointer + REAL34_SIZE*sum));
    }
    graph_setupmemory(); //JMGRAPH
  }
}



/********************************************//**
 * \brief Adds a value to the statistic registers
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnSigma(uint16_t plusMinus) {
  real34_t tmpReal1, tmpReal2, tmpReal3;
  real39_t real39;

  if(   (getRegisterDataType(REGISTER_X) == dtLongInteger || getRegisterDataType(REGISTER_X) == dtReal34)
     && (getRegisterDataType(REGISTER_Y) == dtLongInteger || getRegisterDataType(REGISTER_Y) == dtReal34)) {

    if(statisticalSumsPointer == NULL) {
      initStatisticalSums();
    }

    if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
      convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    }

    if(getRegisterDataType(REGISTER_Y) == dtLongInteger) {
     convertLongIntegerRegisterToReal34Register(REGISTER_Y, REGISTER_Y);
    }

    if(plusMinus == 1) { // SIGMA+
      // n
      real34Add(SIGMA_N, const34_1, SIGMA_N);

      // sigma x
      real34Add(SIGMA_X, REGISTER_REAL34_DATA(REGISTER_X), SIGMA_X);

      // sigma y
      real34Add(SIGMA_Y, REGISTER_REAL34_DATA(REGISTER_Y), SIGMA_Y);

      // sigma x²
      real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X), &tmpReal1);
      real34Add(SIGMA_X2, &tmpReal1, SIGMA_X2);

      // sigma x³
      real34Multiply(&tmpReal1, REGISTER_REAL34_DATA(REGISTER_X), &tmpReal2);
      real34Add(SIGMA_X3, &tmpReal2, SIGMA_X3);

      // sigma x⁴
      real34Multiply(&tmpReal2, REGISTER_REAL34_DATA(REGISTER_X), &tmpReal2);
      real34Add(SIGMA_X4, &tmpReal2, SIGMA_X4);

      // sigma x²y
      real34Multiply(&tmpReal1, REGISTER_REAL34_DATA(REGISTER_Y), &tmpReal2);
      real34Add(SIGMA_X2Y, &tmpReal2, SIGMA_X2Y);

      // sigma x²/y
      real34Divide(&tmpReal1, REGISTER_REAL34_DATA(REGISTER_Y), &tmpReal2);
      real34Add(SIGMA_X2onY, &tmpReal2, SIGMA_X2onY);

      // sigma 1/x²
      real34Divide(const34_1, &tmpReal1, &tmpReal2);
      real34Add(SIGMA_1onX2, &tmpReal2, SIGMA_1onX2);

      // sigma y²
      real34Multiply(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_Y), &tmpReal1);
      real34Add(SIGMA_Y2, &tmpReal1, SIGMA_Y2);

      // sigma 1/y²
      real34Divide(const34_1, &tmpReal1, &tmpReal2);
      real34Add(SIGMA_1onY2, &tmpReal2, SIGMA_1onY2);

      // sigma xy
      real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y), &tmpReal1);
      real34Add(SIGMA_XY, &tmpReal1, SIGMA_XY);

      // sigma ln(x)
      real34Ln(REGISTER_REAL34_DATA(REGISTER_X), &tmpReal1);
      real34Copy(&tmpReal1 ,&tmpReal3);
      real34Add(SIGMA_lnX, &tmpReal1, SIGMA_lnX);

      // sigma ln²(x)
      real34Multiply(&tmpReal1, &tmpReal1, &tmpReal2);
      real34Add(SIGMA_ln2X, &tmpReal2, SIGMA_ln2X);

      // sigma yln(x)
      real34Multiply(&tmpReal1, REGISTER_REAL34_DATA(REGISTER_Y), &tmpReal1);
      real34Add(SIGMA_YlnX, &tmpReal1, SIGMA_YlnX);

      // sigma ln(y)
      real34Ln(REGISTER_REAL34_DATA(REGISTER_Y), &tmpReal1);
      real34Add(SIGMA_lnY, &tmpReal1, SIGMA_lnY);

      // sigma ln(x)×ln(y)
      real34Multiply(&tmpReal3, &tmpReal1, &tmpReal3);
      real34Add(SIGMA_lnXlnY, &tmpReal3, SIGMA_lnXlnY);

      // sigma ln(y)/x
      real34Divide(&tmpReal1, REGISTER_REAL34_DATA(REGISTER_X), &tmpReal2);
      real34Add(SIGMA_lnYonX, &tmpReal2, SIGMA_lnYonX);

      // sigma ln²(y)
      real34Multiply(&tmpReal1, &tmpReal1, &tmpReal2);
      real34Add(SIGMA_ln2Y, &tmpReal2, SIGMA_ln2Y);

      // sigma xln(y)
      real34Multiply(&tmpReal1, REGISTER_REAL34_DATA(REGISTER_X), &tmpReal1);
      real34Add(SIGMA_XlnY, &tmpReal1, SIGMA_XlnY);

      // sigma 1/x
      real34Divide(const34_1, REGISTER_REAL34_DATA(REGISTER_X), &tmpReal1);
      real34Add(SIGMA_1onX, &tmpReal1, SIGMA_1onX);

      // sigma x/y
      real34Divide(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y), &tmpReal1);
      real34Add(SIGMA_XonY, &tmpReal1, SIGMA_XonY);

      // sigma 1/y
      real34Divide(const34_1, REGISTER_REAL34_DATA(REGISTER_Y), &tmpReal1);
      real34Add(SIGMA_1onY, &tmpReal1, SIGMA_1onY);

      graph_sigmaplus(); //JMGRAPH

    }
    else { // SIGMA-
      // n
      real34Subtract(SIGMA_N, const34_1, SIGMA_N);

      // sigma x
      real34Subtract(SIGMA_X, REGISTER_REAL34_DATA(REGISTER_X), SIGMA_X);

      // sigma y
      real34Subtract(SIGMA_Y, REGISTER_REAL34_DATA(REGISTER_Y), SIGMA_Y);

      // sigma x²
      real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X), &tmpReal1);
      real34Subtract(SIGMA_X2, &tmpReal1, SIGMA_X2);

      // sigma x³
      real34Multiply(&tmpReal1, REGISTER_REAL34_DATA(REGISTER_X), &tmpReal2);
      real34Subtract(SIGMA_X3, &tmpReal2, SIGMA_X3);

      // sigma x⁴
      real34Multiply(&tmpReal2, REGISTER_REAL34_DATA(REGISTER_X), &tmpReal2);
      real34Subtract(SIGMA_X4, &tmpReal2, SIGMA_X4);

      // sigma x²y
      real34Multiply(&tmpReal1, REGISTER_REAL34_DATA(REGISTER_Y), &tmpReal2);
      real34Subtract(SIGMA_X2Y, &tmpReal2, SIGMA_X2Y);

      // sigma x²/y
      real34Divide(&tmpReal1, REGISTER_REAL34_DATA(REGISTER_Y), &tmpReal2);
      real34Subtract(SIGMA_X2onY, &tmpReal2, SIGMA_X2onY);

      // sigma 1/x²
      real34Divide(const34_1, &tmpReal1, &tmpReal2);
      real34Subtract(SIGMA_1onX2, &tmpReal2, SIGMA_1onX2);

      // sigma y²
      real34Multiply(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_Y), &tmpReal1);
      real34Subtract(SIGMA_Y2, &tmpReal1, SIGMA_Y2);

      // sigma 1/y²
      real34Divide(const34_1, &tmpReal1, &tmpReal2);
      real34Subtract(SIGMA_1onY2, &tmpReal2, SIGMA_1onY2);

      // sigma xy
      real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y), &tmpReal1);
      real34Subtract(SIGMA_XY, &tmpReal1, SIGMA_XY);

      // sigma ln(x)
      real34Ln(REGISTER_REAL34_DATA(REGISTER_X), &tmpReal1);
      real34Copy(&tmpReal1 ,&tmpReal3);
      real34Subtract(SIGMA_lnX, &tmpReal1, SIGMA_lnX);

      // sigma ln²(x)
      real34Multiply(&tmpReal1, &tmpReal1, &tmpReal2);
      real34Subtract(SIGMA_ln2X, &tmpReal2, SIGMA_ln2X);

      // sigma yln(x)
      real34Multiply(&tmpReal1, REGISTER_REAL34_DATA(REGISTER_Y), &tmpReal1);
      real34Subtract(SIGMA_YlnX, &tmpReal1, SIGMA_YlnX);

      // sigma ln(y)
      real34Ln(REGISTER_REAL34_DATA(REGISTER_Y), &tmpReal1);
      real34Subtract(SIGMA_lnY, &tmpReal1, SIGMA_lnY);

      // sigma ln(x)×ln(y)
      real34Multiply(&tmpReal3, &tmpReal1, &tmpReal3);
      real34Subtract(SIGMA_lnXlnY, &tmpReal3, SIGMA_lnXlnY);

      // sigma ln(y)/x
      real34Divide(&tmpReal1, REGISTER_REAL34_DATA(REGISTER_X), &tmpReal2);
      real34Subtract(SIGMA_lnYonX, &tmpReal2, SIGMA_lnYonX);

      // sigma ln²(y)
      real34Multiply(&tmpReal1, &tmpReal1, &tmpReal2);
      real34Subtract(SIGMA_ln2Y, &tmpReal2, SIGMA_ln2Y);

      // sigma xln(y)
      real34Multiply(&tmpReal1, REGISTER_REAL34_DATA(REGISTER_X), &tmpReal1);
      real34Subtract(SIGMA_XlnY, &tmpReal1, SIGMA_XlnY);

      // sigma 1/x
      real34Divide(const34_1, REGISTER_REAL34_DATA(REGISTER_X), &tmpReal1);
      real34Subtract(SIGMA_1onX, &tmpReal1, SIGMA_1onX);

      // sigma x/y
      real34Divide(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y), &tmpReal1);
      real34Subtract(SIGMA_XonY, &tmpReal1, SIGMA_XonY);

      // sigma 1/y
      real34Divide(const34_1, REGISTER_REAL34_DATA(REGISTER_Y), &tmpReal1);
      real34Subtract(SIGMA_1onY, &tmpReal1, SIGMA_1onY);

      graph_sigmaminus(); //JMGRAPH
    }

    temporaryInformation = TI_STATISTIC_SUMS;

    refreshStack();
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X); // Invalid input data type for this operation
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot use (%s, %s) as statistical data!", getDataTypeName(getRegisterDataType(REGISTER_X), false, false), getDataTypeName(getRegisterDataType(REGISTER_Y), false, false));
      showInfoDialog("In function fnSigma:", errorMessage, NULL, NULL);
    #endif
  }
//for(int i=0; i<NUMBER_OF_STATISTICAL_SUMS; i++) {
//printf("sum %02d ", i); printReal34ToConsole(((real34_t *)statisticalSumsPointer) + i); printf("\n");
//}
}



void fnStatSum(uint16_t sum) {
  if(statisticalSumsPointer == NULL) {
    displayCalcErrorMessage(ERROR_NO_SUMMATION_DATA, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "There is no statistical data available!");
      showInfoDialog("In function fnStatSum:", errorMessage, NULL, NULL);
    #endif
  }
  else {
   liftStack();
   real34Copy(statisticalSumsPointer + REAL34_SIZE*sum, REGISTER_REAL34_DATA(REGISTER_X));
   refreshStack();
  }
}

/********************************************//**
 * \brief SUM ==> regX, regY
 * enables stack lift and refreshes the stack.
 * regX = SUM x, regY = SUM y
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnSumXY(uint16_t unusedParamButMandatory) {
  if(statisticalSumsPointer == NULL) {
    displayCalcErrorMessage(ERROR_NO_SUMMATION_DATA, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "There is no statistical data available!");
      showInfoDialog("In function fnSumXY:", errorMessage, NULL, NULL);
    #endif
  }
  else {
    saveStack();

    liftStack();
    STACK_LIFT_ENABLE;
    liftStack();

    real34Copy(SIGMA_X, REGISTER_REAL34_DATA(REGISTER_X));
    real34Copy(SIGMA_Y, REGISTER_REAL34_DATA(REGISTER_Y));

    temporaryInformation = TI_SUMX_SUMY;
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
  if(statisticalSumsPointer == NULL) {
    displayCalcErrorMessage(ERROR_NO_SUMMATION_DATA, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "There is no statistical data available!");
      showInfoDialog("In function fnMeanXY:", errorMessage, NULL, NULL);
    #endif
  }
  else {
    saveStack();

    liftStack();
    STACK_LIFT_ENABLE;
    liftStack();

    real34Divide(SIGMA_X, SIGMA_N, REGISTER_REAL34_DATA(REGISTER_X));
    real34Divide(SIGMA_Y, SIGMA_N, REGISTER_REAL34_DATA(REGISTER_Y));

    temporaryInformation = TI_MEANX_MEANY;
    refreshStack();
  }
}
