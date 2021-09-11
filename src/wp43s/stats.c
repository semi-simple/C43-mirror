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

#include "stats.h"

#include "constantPointers.h"
#include "debug.h"
#include "error.h"
#include "flags.h"
#include "fonts.h"
#include "mathematics/comparisonReals.h"
#include "mathematics/wp34s.h"
#include "matrix.h"
#include "memory.h"
#include "plotstat.h"
#include "registers.h"
#include "registerValueConversions.h"
#include "stack.h"

#include "wp43s.h"


#ifndef TESTSUITE_BUILD
static bool_t isStatsMatrix(uint16_t *rows) {
  *rows = 0;
  calcRegister_t regStats = findNamedVariable("STATS");
  if(regStats == INVALID_VARIABLE) return false; else {
    if(getRegisterDataType(regStats) != dtReal34Matrix) return false; else {
      real34Matrix_t stats;
      linkToRealMatrixRegister(regStats, &stats);
      *rows = stats.header.matrixRows;
      if (stats.header.matrixColumns != 2) return false;
    }
  }
  return true;
}


static void addSigma(real_t *x, real_t *y) {
  real_t tmpReal1, tmpReal2, tmpReal3;
  realContext_t *realContext = &ctxtReal75; // Summation data with 75 digits

  // xmin
  if(realCompareLessThan(x, SIGMA_XMIN)) {
    realCopy(x, SIGMA_XMIN);
  }

  // xmax
  if(realCompareGreaterThan(x, SIGMA_XMAX)) {
    realCopy(x, SIGMA_XMAX);
  }

  // ymin
  if(realCompareLessThan(y, SIGMA_YMIN)) {
    realCopy(y, SIGMA_YMIN);
  }

  // ymax
  if(realCompareGreaterThan(y, SIGMA_YMAX)) {
    realCopy(y, SIGMA_YMAX);
  }

  // n
  realAdd(SIGMA_N, const_1, SIGMA_N, realContext);

  // sigma x
  realAdd(SIGMA_X, x, SIGMA_X, realContext);

  // sigma y
  realAdd(SIGMA_Y, y, SIGMA_Y, realContext);

  // sigma x²
  realMultiply(x, x, &tmpReal1, realContext);
  realAdd(SIGMA_X2, &tmpReal1, SIGMA_X2, realContext);

  // sigma x³
  realMultiply(&tmpReal1, x, &tmpReal2, realContext);
  realAdd(SIGMA_X3, &tmpReal2, SIGMA_X3, realContext);

  // sigma x⁴
  realMultiply(&tmpReal2, x, &tmpReal2, realContext);
  realAdd(SIGMA_X4, &tmpReal2, SIGMA_X4, realContext);

  // sigma x²y
  realMultiply(&tmpReal1, y, &tmpReal2, realContext);
  realAdd(SIGMA_X2Y, &tmpReal2, SIGMA_X2Y, realContext);

  // sigma x²/y
  realDivide(&tmpReal1, y, &tmpReal2, realContext);
  realAdd(SIGMA_X2onY, &tmpReal2, SIGMA_X2onY, realContext);

  // sigma 1/x²
  realDivide(const_1, &tmpReal1, &tmpReal2, realContext);
  realAdd(SIGMA_1onX2, &tmpReal2, SIGMA_1onX2, realContext);

  // sigma y²
  realMultiply(y, y, &tmpReal1, realContext);
  realAdd(SIGMA_Y2, &tmpReal1, SIGMA_Y2, realContext);

  // sigma 1/y²
  realDivide(const_1, &tmpReal1, &tmpReal2, realContext);
  realAdd(SIGMA_1onY2, &tmpReal2, SIGMA_1onY2, realContext);

  // sigma xy
  realMultiply(x, y, &tmpReal1, realContext);
  realAdd(SIGMA_XY, &tmpReal1, SIGMA_XY, realContext);

  // sigma ln(x)
  WP34S_Ln(x, &tmpReal1, realContext);
  realCopy(&tmpReal1 ,&tmpReal3);
  realAdd(SIGMA_lnX, &tmpReal1, SIGMA_lnX, realContext);

  // sigma ln²(x)
  realMultiply(&tmpReal1, &tmpReal1, &tmpReal2, realContext);
  realAdd(SIGMA_ln2X, &tmpReal2, SIGMA_ln2X, realContext);

  // sigma yln(x)
  realMultiply(&tmpReal1, y, &tmpReal1, realContext);
  realAdd(SIGMA_YlnX, &tmpReal1, SIGMA_YlnX, realContext);

  // sigma ln(y)
  WP34S_Ln(y, &tmpReal1, realContext);
  realAdd(SIGMA_lnY, &tmpReal1, SIGMA_lnY, realContext);

  // sigma ln(x)×ln(y)
  realMultiply(&tmpReal3, &tmpReal1, &tmpReal3, realContext);
  realAdd(SIGMA_lnXlnY, &tmpReal3, SIGMA_lnXlnY, realContext);

  // sigma ln(y)/x
  realDivide(&tmpReal1, x, &tmpReal2, realContext);
  realAdd(SIGMA_lnYonX, &tmpReal2, SIGMA_lnYonX, realContext);

  // sigma ln²(y)
  realMultiply(&tmpReal1, &tmpReal1, &tmpReal2, realContext);
  realAdd(SIGMA_ln2Y, &tmpReal2, SIGMA_ln2Y, realContext);

  // sigma xln(y)
  realMultiply(&tmpReal1, x, &tmpReal1, realContext);
  realAdd(SIGMA_XlnY, &tmpReal1, SIGMA_XlnY, realContext);

  // sigma x²ln(y)
  realMultiply(&tmpReal1, x, &tmpReal1, realContext);
  realAdd(SIGMA_X2lnY, &tmpReal1, SIGMA_X2lnY, realContext);

  // sigma 1/x
  realDivide(const_1, x, &tmpReal1, realContext);
  realAdd(SIGMA_1onX, &tmpReal1, SIGMA_1onX, realContext);

  // sigma x/y
  realDivide(x, y, &tmpReal1, realContext);
  realAdd(SIGMA_XonY, &tmpReal1, SIGMA_XonY, realContext);

  // sigma 1/y
  realDivide(const_1, y, &tmpReal1, realContext);
  realAdd(SIGMA_1onY, &tmpReal1, SIGMA_1onY, realContext);

  graph_sigmaplus(+1, x, y);
}
#endif //TESTSUITE_|BUILD



bool_t checkMinimumDataPoints(const real_t *n) {
  if(statisticalSumsPointer == NULL) {
    displayCalcErrorMessage(ERROR_NO_SUMMATION_DATA, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      moreInfoOnError("In function checkMinimumDataPoints:", "There is no statistical data available!", NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return false;
  }

  if(realCompareLessThan(SIGMA_N, n)) {
    displayCalcErrorMessage(ERROR_TOO_FEW_DATA, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      moreInfoOnError("In function checkMinimumDataPoints:", "There is insufficient statistical data available!", NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return false;
  }

  return true;
}


static void clearStatisticalSums(void) {
  if(statisticalSumsPointer) {
    for(int32_t sum=0; sum<NUMBER_OF_STATISTICAL_SUMS - 4; sum++) {
      realZero((real_t *)(statisticalSumsPointer + REAL_SIZE * sum));
    }
    realCopy(const_plusInfinity,  SIGMA_XMIN);
    realCopy(const_plusInfinity,  SIGMA_YMIN);
    realCopy(const_minusInfinity, SIGMA_XMAX);
    realCopy(const_minusInfinity, SIGMA_YMAX);
  }
}

void initStatisticalSums(void) {
  if(statisticalSumsPointer == NULL) {
    statisticalSumsPointer = allocWp43s(NUMBER_OF_STATISTICAL_SUMS * REAL_SIZE);
    clearStatisticalSums();
    }
    else {
      lastErrorCode = ERROR_RAM_FULL;
    }
  if(telltale != MEM_INITIALIZED) {
    graph_setupmemory();
  }
}



void calcSigma(void) {
#ifndef TESTSUITE_BUILD
  clearStatisticalSums();
  calcRegister_t regStats = findNamedVariable("STATS");
  if(regStats != INVALID_VARIABLE) {

    real34Matrix_t stats;
    linkToRealMatrixRegister(regStats, &stats);
    const uint16_t rows = stats.header.matrixRows, cols = stats.header.matrixColumns;

#ifdef PC_BUILD
printf(">>> Recalculating sums:\n");                                  //temporary debugging matrix display
#endif
    real_t x, y;
    for(uint16_t i = 0; i < rows; i++) {
      real34ToReal(&stats.matrixElements[i * cols    ], &x);
      real34ToReal(&stats.matrixElements[i * cols + 1], &y);
#ifdef PC_BUILD
printReal34ToConsole(&stats.matrixElements[i * cols    ],">>> x:",", ");  //temporary debugging matrix display
printReal34ToConsole(&stats.matrixElements[i * cols +1 ],"y:","\n");  //temporary debugging matrix display
#endif
      addSigma(&x, &y);
    }
  }
#endif //TESTSUITE_BUILD
}


#ifndef TESTSUITE_BUILD
static void AddtoStatsMatrix(real_t *x, real_t *y) {
  uint16_t rows = 0, cols;
  calcRegister_t regStats = findNamedVariable("STATS");
  if(!isStatsMatrix(&rows)) {
    regStats = allocateNamedMatrix("STATS", 1, 2);
  }
  else {
    if(appendRowAtMatrixRegister(regStats)) {
      regStats = findNamedVariable("STATS");
    }
    else {
      regStats = INVALID_VARIABLE;
    }
  }
  if(regStats != INVALID_VARIABLE) {
    real34Matrix_t stats;
    linkToRealMatrixRegister(regStats, &stats);
    rows = stats.header.matrixRows;
    cols = stats.header.matrixColumns;
    realToReal34(x, &stats.matrixElements[(rows-1) * cols    ]);
    realToReal34(y, &stats.matrixElements[(rows-1) * cols + 1]);
  }
  else {
    displayCalcErrorMessage(ERROR_NOT_ENOUGH_MEMORY_FOR_NEW_MATRIX, ERR_REGISTER_LINE, REGISTER_X); // Invalid input data type for this operation
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "additional matrix line not added");
      moreInfoOnError("In function AddtoStatsMatrix:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
}



static void removeFromStatsMatrix(void) {
  uint16_t rows = 0;
  if(!isStatsMatrix(&rows)) {
    displayCalcErrorMessage(ERROR_NO_SUMMATION_DATA, ERR_REGISTER_LINE, REGISTER_X); // Invalid input data type for this operation
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "no STATS matrix");
      moreInfoOnError("In function removeFromStatsMatrix:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return;
  }

  calcRegister_t regStats = findNamedVariable("STATS");
  if(rows <= 1) {
    fnClSigma(0); 
    return;
  }
  else {
    if(!redimMatrixRegister(regStats, rows - 1, 2)) 
      regStats = INVALID_VARIABLE; 
    else
    {
      calcSigma();
    }
  }
  if(regStats == INVALID_VARIABLE) {
    displayCalcErrorMessage(ERROR_NOT_ENOUGH_MEMORY_FOR_NEW_MATRIX, ERR_REGISTER_LINE, REGISTER_X); // Invalid input data type for this operation
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "matrix/line not removed");
      moreInfoOnError("In function removeFromStatsMatrix:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
}
#endif //TESTSUITE_BUILD


void fnClSigma(uint16_t unusedButMandatoryParameter) {
  calcRegister_t regStats = findNamedVariable("STATS");
  clearRegister(regStats);                  // this should change to delete the named variable STATS once the delete function is available. Until then write 0.0 into STATS.
  graph_end();                              // release plot memory
  lrChosen = 0;                             // linear regression selection
  lastPlotMode = PLOT_NOTHING;              // last selected  plotmode
  plotSelection = 0;                        // Currently selected linear regression mode
  PLOT_ZOOM = 0;                            // Currently selected plot zoom level
  if(statisticalSumsPointer != NULL) {
    freeWp43s(statisticalSumsPointer, NUMBER_OF_STATISTICAL_SUMS * REAL_SIZE);
    statisticalSumsPointer = NULL;
  }
}



void fnSigma(uint16_t plusMinus) {
#ifndef TESTSUITE_BUILD
  real_t x, y;
  realContext_t *realContext = &ctxtReal75; // Summation data with 75 digits

  lrChosen = 0;

  if(plusMinus != 1) { // SIGMA-
    removeFromStatsMatrix();
    if(statisticalSumsPointer != NULL) temporaryInformation = TI_STATISTIC_SUMS;
    //graph_sigmaplus(-1, &x, &y);
  } 
  else {
    if(   (getRegisterDataType(REGISTER_X) == dtLongInteger || getRegisterDataType(REGISTER_X) == dtReal34)
       && (getRegisterDataType(REGISTER_Y) == dtLongInteger || getRegisterDataType(REGISTER_Y) == dtReal34)) {

      if(statisticalSumsPointer == NULL) {
        initStatisticalSums();
        if(lastErrorCode != ERROR_NONE) return;
      }

      if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
        convertLongIntegerRegisterToReal(REGISTER_X, &x, realContext);
        convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
      }
      else {
        real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
      }

      if(getRegisterDataType(REGISTER_Y) == dtLongInteger) {
        convertLongIntegerRegisterToReal(REGISTER_Y, &y, realContext);
        convertLongIntegerRegisterToReal34Register(REGISTER_Y, REGISTER_Y);
      }
      else {
        real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
      }

      addSigma(&x, &y);
      AddtoStatsMatrix(&x, &y);


      temporaryInformation = TI_STATISTIC_SUMS;
    }
    else if(getRegisterDataType(REGISTER_X) == dtReal34Matrix && plusMinus == 1) {
      real34Matrix_t matrix;
      linkToRealMatrixRegister(REGISTER_X, &matrix);

      if(matrix.header.matrixColumns == 2) {
        if(statisticalSumsPointer == NULL) {
          initStatisticalSums();
          if(lastErrorCode != ERROR_NONE) return;
        }

        if(!saveLastX()) return;
        for(uint16_t i = 0; i < matrix.header.matrixRows; ++i) {
          real34ToReal(&matrix.matrixElements[i * 2    ], &y);
          real34ToReal(&matrix.matrixElements[i * 2 + 1], &x);
          addSigma(&x, &y);
          AddtoStatsMatrix(&x, &y);
        }

        liftStack();
        reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);
        convertRealToReal34ResultRegister(&y, REGISTER_Y);
        reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
        convertRealToReal34ResultRegister(&x, REGISTER_X);
        temporaryInformation = TI_STATISTIC_SUMS;
      }
      else {
        displayCalcErrorMessage(ERROR_MATRIX_MISMATCH, ERR_REGISTER_LINE, REGISTER_X); // Invalid input data type for this operation
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          sprintf(errorMessage, "cannot use %" PRIu16 STD_CROSS "%" PRId16 "-matrix as statistical data!", matrix.header.matrixRows, matrix.header.matrixColumns);
          moreInfoOnError("In function fnSigma:", errorMessage, NULL, NULL);
        #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      }
    }
    else {
      displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X); // Invalid input data type for this operation
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "cannot use (%s, %s) as statistical data!", getDataTypeName(getRegisterDataType(REGISTER_X), false, false), getDataTypeName(getRegisterDataType(REGISTER_Y), false, false));
        moreInfoOnError("In function fnSigma:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
  }
#endif // TESTSUITE_BUILD
}



void fnStatSum(uint16_t sum) {
  if(checkMinimumDataPoints(const_1)) {
    liftStack();
    realToReal34((real_t *)(statisticalSumsPointer + REAL_SIZE * sum), REGISTER_REAL34_DATA(REGISTER_X));
  }
}



void fnSumXY(uint16_t unusedButMandatoryParameter) {
  if(checkMinimumDataPoints(const_1)) {
    liftStack();
    setSystemFlag(FLAG_ASLIFT);
    liftStack();

    convertRealToReal34ResultRegister(SIGMA_X, REGISTER_X);
    convertRealToReal34ResultRegister(SIGMA_Y, REGISTER_Y);

    temporaryInformation = TI_SUMX_SUMY;
  }
}



void fnXmin(uint16_t unusedButMandatoryParameter) {
  if(checkMinimumDataPoints(const_1)) {
    liftStack();
    setSystemFlag(FLAG_ASLIFT);
    liftStack();

    convertRealToReal34ResultRegister(SIGMA_XMIN, REGISTER_X);
    convertRealToReal34ResultRegister(SIGMA_YMIN, REGISTER_Y);

    temporaryInformation = TI_XMIN_YMIN;
  }
}



void fnXmax(uint16_t unusedButMandatoryParameter) {
  if(checkMinimumDataPoints(const_1)) {
    liftStack();
    setSystemFlag(FLAG_ASLIFT);
    liftStack();

    convertRealToReal34ResultRegister(SIGMA_XMAX, REGISTER_X);
    convertRealToReal34ResultRegister(SIGMA_YMAX, REGISTER_Y);

    temporaryInformation = TI_XMAX_YMAX;
  }
}
