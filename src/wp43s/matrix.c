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

#include "matrix.h"

#include <string.h>
#include "bufferize.h"
#include "charString.h"
#include "constantPointers.h"
#include "conversionAngles.h"
#include "debug.h"
#include "defines.h"
#include "display.h"
#include "error.h"
#include "flags.h"
#include "fonts.h"
#include "gui.h"
#include "items.h"
#include "longIntegerType.h"
#include "mathematics/comparisonReals.h"
#include "mathematics/division.h"
#include "mathematics/magnitude.h"
#include "mathematics/multiplication.h"
#include "mathematics/toPolar.h"
#include "mathematics/toRect.h"
#include "mathematics/wp34s.h"
#include "memory.h"
#include "realType.h"
#include "registers.h"
#include "registerValueConversions.h"
#include "screen.h"
#include "softmenus.h"
#include "stack.h"
#include "statusBar.h"
#include "store.h"
#include "ui/tam.h"

#include "wp43s.h"

#ifndef TESTSUITE_BUILD
any34Matrix_t         openMatrixMIMPointer;
bool_t                matEditMode;
uint16_t              scrollRow;
uint16_t              scrollColumn;
uint16_t              tmpRow;
uint16_t              matrixIndex = INVALID_VARIABLE;



static bool_t getArg(calcRegister_t regist, real_t *arg) {
  if(getRegisterDataType(regist) == dtLongInteger) {
    convertLongIntegerRegisterToReal(regist, arg, &ctxtReal39);
  }
  else if(getRegisterDataType(regist) == dtReal34) {
    real34ToReal(REGISTER_REAL34_DATA(regist), arg);
    realToIntegralValue(arg, arg, DEC_ROUND_DOWN, &ctxtReal39);
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot accept %s as the argument", getRegisterDataTypeName(regist, true, false));
      moreInfoOnError("In function getArg:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return false;
  }
  return true;
}

static bool_t getDimensionArg(uint32_t *rows, uint32_t *cols) {
  longInteger_t tmp_lgInt;

  //Get Size from REGISTER_X and REGISTER_Y
  if(((getRegisterDataType(REGISTER_X) != dtLongInteger) && (getRegisterDataType(REGISTER_X) != dtReal34)) ||
    ((getRegisterDataType(REGISTER_Y) != dtLongInteger) && (getRegisterDataType(REGISTER_Y) != dtReal34))) {
      displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "invalid data type %s and %s", getRegisterDataTypeName(REGISTER_Y, true, false), getRegisterDataTypeName(REGISTER_X, true, false));
        moreInfoOnError("In function getDimensionArg:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      return false;
  }

  if(getRegisterDataType(REGISTER_X) == dtLongInteger)
    convertLongIntegerRegisterToLongInteger(REGISTER_X, tmp_lgInt);
  else // dtReal34
    convertReal34ToLongInteger(REGISTER_REAL34_DATA(REGISTER_X), tmp_lgInt, DEC_ROUND_DOWN);
  if(longIntegerIsNegativeOrZero(tmp_lgInt)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      char strbuf[32];
      longIntegerToAllocatedString(tmp_lgInt, strbuf, 32);
      sprintf(errorMessage, "invalid number of columns");
      moreInfoOnError("In function getDimensionArg:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return false;
  }
  longIntegerToUInt(tmp_lgInt, *cols);

  if(getRegisterDataType(REGISTER_Y) == dtLongInteger)
    convertLongIntegerRegisterToLongInteger(REGISTER_Y, tmp_lgInt);
  else // dtReal34
    convertReal34ToLongInteger(REGISTER_REAL34_DATA(REGISTER_Y), tmp_lgInt, DEC_ROUND_DOWN);
  if(longIntegerIsNegativeOrZero(tmp_lgInt)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      char strbuf[32];
      longIntegerToAllocatedString(tmp_lgInt, strbuf, 32);
      sprintf(errorMessage, "invalid number of rows");
      moreInfoOnError("In function getDimensionArg:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return false;
  }
  longIntegerToUInt(tmp_lgInt, *rows);

  longIntegerFree(tmp_lgInt);
  return true;
}



static bool_t swapRowsReal(real34Matrix_t *matrix) {
  real_t ry, rx, rrows;
  uint16_t a, b;

  int32ToReal(matrix->header.matrixRows, &rrows);

  if((!getArg(REGISTER_Y, &ry)) || (!getArg(REGISTER_X, &rx))) return false;

  realToInt32(&ry, a);
  realToInt32(&rx, b);
  if(realIsPositive(&rx) && realIsPositive(&ry) && realCompareLessEqual(&rx, &rrows) && realCompareLessEqual(&ry, &rrows)) {
    if(!realCompareEqual(&ry, &rx))
      realMatrixSwapRows(matrix, matrix, a - 1, b - 1);
  }
  else {
    displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "rows %" PRIu16 " and/or %" PRIu16 " out of range", a, b);
      moreInfoOnError("In function swapRowsReal:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return false;
  }

  return true;
}

static bool_t swapRowsComplex(complex34Matrix_t *matrix) {
  real_t ry, rx, rrows;
  uint16_t a, b;

  int32ToReal(matrix->header.matrixRows, &rrows);

  if((!getArg(REGISTER_Y, &ry)) || (!getArg(REGISTER_X, &rx))) return false;

  realToInt32(&ry, a);
  realToInt32(&rx, b);
  if(realIsPositive(&rx) && realIsPositive(&ry) && realCompareLessEqual(&rx, &rrows) && realCompareLessEqual(&ry, &rrows)) {
    if(!realCompareEqual(&ry, &rx))
      complexMatrixSwapRows(matrix, matrix, a - 1, b - 1);
  }
  else {
    displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "rows %" PRIu16 " and/or %" PRIu16 " out of range", a, b);
      moreInfoOnError("In function swapRowsComplex:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return false;
  }

  return true;
}



static bool_t getMatrixReal(real34Matrix_t *matrix) {
  real_t ry, rx, rrows, rcols;
  uint16_t a, b, r, c;

  const int16_t i = getIRegisterAsInt(true);
  const int16_t j = getJRegisterAsInt(true);

  int32ToReal(matrix->header.matrixRows    - i, &rrows);
  int32ToReal(matrix->header.matrixColumns - j, &rcols);

  if((!getArg(REGISTER_Y, &ry)) || (!getArg(REGISTER_X, &rx))) return false;

  realToInt32(&ry, a);
  realToInt32(&rx, b);
  if(realIsPositive(&rx) && realIsPositive(&ry) && realCompareLessEqual(&rx, &rcols) && realCompareLessEqual(&ry, &rrows)) {
    real34Matrix_t mat;
    fnDropY(NOPARAM);
    if(lastErrorCode == ERROR_NONE) {
      if(initMatrixRegister(REGISTER_X, a, b, false)) {
        linkToRealMatrixRegister(REGISTER_X, &mat);
        for(r = 0; r < a; ++r)
          for(c = 0; c < b; ++c)
            real34Copy(&matrix->matrixElements[(r + i) * matrix->header.matrixColumns + c + j], &mat.matrixElements[r * b + c]);
      }
      else {
        lastErrorCode = ERROR_RAM_FULL;
        return false;
      }
    }
  }
  else {
    displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "%" PRIu16 " " STD_CROSS " %" PRIu16 " out of range", a, b);
      moreInfoOnError("In function getMatrixReal:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return false;
  }

  return false;
}

static bool_t getMatrixComplex(complex34Matrix_t *matrix) {
  real_t ry, rx, rrows, rcols;
  uint16_t a, b, r, c;

  const int16_t i = getIRegisterAsInt(true);
  const int16_t j = getJRegisterAsInt(true);

  int32ToReal(matrix->header.matrixRows    - i, &rrows);
  int32ToReal(matrix->header.matrixColumns - j, &rcols);

  if((!getArg(REGISTER_Y, &ry)) || (!getArg(REGISTER_X, &rx))) return false;

  realToInt32(&ry, a);
  realToInt32(&rx, b);
  if(realIsPositive(&rx) && realIsPositive(&ry) && realCompareLessEqual(&rx, &rcols) && realCompareLessEqual(&ry, &rrows)) {
    complex34Matrix_t mat;
    fnDropY(NOPARAM);
    if(lastErrorCode == ERROR_NONE) {
      if(initMatrixRegister(REGISTER_X, a, b, true)) {
        linkToComplexMatrixRegister(REGISTER_X, &mat);
        for(r = 0; r < a; ++r)
          for(c = 0; c < b; ++c)
            complex34Copy(&matrix->matrixElements[(r + i) * matrix->header.matrixColumns + c + j], &mat.matrixElements[r * b + c]);
      }
      else lastErrorCode = ERROR_RAM_FULL;
    }
  }
  else {
    displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "%" PRIu16 " " STD_CROSS " %" PRIu16 " out of range", a, b);
      moreInfoOnError("In function getMatrixComplex:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return false;
  }

  return false;
}



static bool_t putMatrixReal(real34Matrix_t *matrix) {
  uint16_t r, c;
  real34Matrix_t mat;

  const int16_t i = getIRegisterAsInt(true);
  const int16_t j = getJRegisterAsInt(true);

  if(getRegisterDataType(REGISTER_X) != dtReal34Matrix) {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "%s is not a real matrix", getRegisterDataTypeName(REGISTER_X, true, false));
      moreInfoOnError("In function putMatrixReal:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return false;
  }

  linkToRealMatrixRegister(REGISTER_X, &mat);
  if((mat.header.matrixRows + i) <= matrix->header.matrixRows && (mat.header.matrixColumns + j) <= matrix->header.matrixColumns) {
    for(r = 0; r < mat.header.matrixRows; ++r)
      for(c = 0; c < mat.header.matrixColumns; ++c)
        real34Copy(&mat.matrixElements[r * mat.header.matrixColumns + c], &matrix->matrixElements[(r + i) * matrix->header.matrixColumns + c + j]);
  }
  else {
    displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "%" PRIu16 " " STD_CROSS " %" PRIu16 " out of range", mat.header.matrixRows, mat.header.matrixColumns);
      moreInfoOnError("In function putMatrixReal:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return false;
  }

  return true;
}

static bool_t putMatrixComplex(complex34Matrix_t *matrix) {
  uint16_t r, c;
  complex34Matrix_t mat;

  const int16_t i = getIRegisterAsInt(true);
  const int16_t j = getJRegisterAsInt(true);

  if(getRegisterDataType(REGISTER_X) != dtComplex34Matrix) {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "%s is not a complex matrix", getRegisterDataTypeName(REGISTER_X, true, false));
      moreInfoOnError("In function putMatrixComplex:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return false;
  }

  linkToComplexMatrixRegister(REGISTER_X, &mat);
  if((mat.header.matrixRows + i) <= matrix->header.matrixRows && (mat.header.matrixColumns + j) <= matrix->header.matrixColumns) {
    for(r = 0; r < mat.header.matrixRows; ++r)
      for(c = 0; c < mat.header.matrixColumns; ++c)
        complex34Copy(&mat.matrixElements[r * mat.header.matrixColumns + c], &matrix->matrixElements[(r + i) * matrix->header.matrixColumns + c + j]);
  }
  else {
    displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "%" PRIu16 " " STD_CROSS " %" PRIu16 " out of range", mat.header.matrixRows, mat.header.matrixColumns);
      moreInfoOnError("In function putMatrixComplex:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return false;
  }

  return true;
}



static bool_t incIReal(real34Matrix_t *matrix) {
  setIRegisterAsInt(true, getIRegisterAsInt(true) + 1);
  wrapIJ(matrix->header.matrixRows, matrix->header.matrixColumns);
  return false;
}

static bool_t decIReal(real34Matrix_t *matrix) {
  setIRegisterAsInt(true, getIRegisterAsInt(true) - 1);
  wrapIJ(matrix->header.matrixRows, matrix->header.matrixColumns);
  return false;
}

static bool_t incJReal(real34Matrix_t *matrix) {
  setJRegisterAsInt(true, getJRegisterAsInt(true) + 1);
  if(wrapIJ(matrix->header.matrixRows, matrix->header.matrixColumns)) {
    insRowRealMatrix(matrix, matrix->header.matrixRows);
    return true;
  }
  else {
    return false;
  }
}

static bool_t decJReal(real34Matrix_t *matrix) {
  setJRegisterAsInt(true, getJRegisterAsInt(true) - 1);
  wrapIJ(matrix->header.matrixRows, matrix->header.matrixColumns);
  return false;
}

static bool_t incIComplex(complex34Matrix_t *matrix) {
  return incIReal((real34Matrix_t *)matrix);
}

static bool_t decIComplex(complex34Matrix_t *matrix) {
  return decIReal((real34Matrix_t *)matrix);
}

static bool_t incJComplex(complex34Matrix_t *matrix) {
  setJRegisterAsInt(true, getJRegisterAsInt(true) + 1);
  if(wrapIJ(matrix->header.matrixRows, matrix->header.matrixColumns)) {
    insRowComplexMatrix(matrix, matrix->header.matrixRows);
    return true;
  }
  else {
    return false;
  }
}

static bool_t decJComplex(complex34Matrix_t *matrix) {
  return decJReal((real34Matrix_t *)matrix);
}
#endif // TESTSUITE_BUILD



void fnNewMatrix(uint16_t unusedParamButMandatory) {
#ifndef TESTSUITE_BUILD
  uint32_t rows, cols;

  if(!getDimensionArg(&rows, &cols)) return;

  if(!saveLastX()) return;

  //Initialize Memory for Matrix
  if(initMatrixRegister(REGISTER_X, rows, cols, false)) {
    setSystemFlag(FLAG_ASLIFT);
  }
  else {
    displayCalcErrorMessage(ERROR_NOT_ENOUGH_MEMORY_FOR_NEW_MATRIX, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "Not enough memory for a %" PRIu32 STD_CROSS "%" PRIu32 " matrix", rows, cols);
      moreInfoOnError("In function fnNewMatrix:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return;
  }

  adjustResult(REGISTER_X, true, false, REGISTER_X, REGISTER_Y, -1);
#endif // TESTSUITE_BUILD
}

void fnEditMatrix(uint16_t regist) {
#ifndef TESTSUITE_BUILD
  const uint16_t reg = (regist == NOPARAM) ? REGISTER_X : regist;
  if((getRegisterDataType(reg) == dtReal34Matrix) || (getRegisterDataType(reg) == dtComplex34Matrix)) {
    calcMode = CM_MIM;
    matrixIndex = reg;

    getMatrixFromRegister(reg);

    setIRegisterAsInt(true, 0);
    setJRegisterAsInt(true, 0);
    aimBuffer[0] = 0;
    nimBufferDisplay[0] = 0;
    scrollRow = scrollColumn = 0;
    showMatrixEditor();
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #ifdef PC_BUILD
    sprintf(errorMessage, "DataType %" PRIu32, getRegisterDataType(reg));
    moreInfoOnError("In function fnEditMatrix:", errorMessage, "is not a matrix.", "");
    #endif
  }
#endif // TESTSUITE_BUILD
}



void fnOldMatrix(uint16_t unusedParamButMandatory) {
#ifndef TESTSUITE_BUILD
  if(calcMode == CM_MIM) {
    aimBuffer[0] = 0;
    nimBufferDisplay[0] = 0;
    hideCursor();
    cursorEnabled = false;

    if(getRegisterDataType(matrixIndex) == dtReal34Matrix) {
      if(openMatrixMIMPointer.realMatrix.matrixElements) realMatrixFree(&openMatrixMIMPointer.realMatrix);
      convertReal34MatrixRegisterToReal34Matrix(matrixIndex, &openMatrixMIMPointer.realMatrix);
    }
    else {
      if(openMatrixMIMPointer.complexMatrix.matrixElements) complexMatrixFree(&openMatrixMIMPointer.complexMatrix);
      convertComplex34MatrixRegisterToComplex34Matrix(matrixIndex, &openMatrixMIMPointer.complexMatrix);
    }
  }
  else {
    displayCalcErrorMessage(ERROR_OPERATION_UNDEFINED, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #ifdef PC_BUILD
    sprintf(errorMessage, "works in MIM only");
    moreInfoOnError("In function fnOldMatrix:", errorMessage, NULL, NULL);
    #endif
  }
#endif // TESTSUITE_BUILD
}



void fnGoToElement(uint16_t unusedParamButMandatory) {
#ifndef TESTSUITE_BUILD
  if(calcMode == CM_MIM) {
    mimEnter(false);
    runFunction(ITM_M_GOTO_ROW);
  }
  else {
    displayCalcErrorMessage(ERROR_OPERATION_UNDEFINED, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #ifdef PC_BUILD
      sprintf(errorMessage, "works in MIM only");
      moreInfoOnError("In function fnGoToElement:", errorMessage, NULL, NULL);
    #endif
  }
#endif // TESTSUITE_BUILD
}

void fnGoToRow(uint16_t row) {
#ifndef TESTSUITE_BUILD
  if(calcMode == CM_MIM) {
    tmpRow = row;
  }
  else {
    displayCalcErrorMessage(ERROR_OPERATION_UNDEFINED, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #ifdef PC_BUILD
      sprintf(errorMessage, "works in MIM only");
      moreInfoOnError("In function fnGoToRow:", errorMessage, NULL, NULL);
    #endif
  }
#endif // TESTSUITE_BUILD
}

void fnGoToColumn(uint16_t col) {
#ifndef TESTSUITE_BUILD
  if(calcMode == CM_MIM) {
    if(tmpRow == 0 || tmpRow > openMatrixMIMPointer.header.matrixRows || col == 0 || col > openMatrixMIMPointer.header.matrixColumns) {
      displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "(%" PRIu16 ", %" PRIu16 ") out of range", tmpRow, col);
        moreInfoOnError("In function putGoToColumn:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
    else {
      if(getRegisterDataType(matrixIndex) == dtReal34Matrix) {
        convertReal34MatrixToReal34MatrixRegister(&openMatrixMIMPointer.realMatrix, matrixIndex);
      }
      else {
        convertComplex34MatrixToComplex34MatrixRegister(&openMatrixMIMPointer.complexMatrix, matrixIndex);
      }
      setIRegisterAsInt(false, tmpRow);
      setJRegisterAsInt(false, col);
    }
    #if defined(PC_BUILD) && (SCREEN_800X480 == 0)
      calcModeNormalGui();
    #endif // PC_BUILD && (SCREEN_800X480 == 0)
  }
  else {
    displayCalcErrorMessage(ERROR_OPERATION_UNDEFINED, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #ifdef PC_BUILD
      sprintf(errorMessage, "works in MIM only");
      moreInfoOnError("In function fnGoToColumn:", errorMessage, NULL, NULL);
    #endif
  }
#endif // TESTSUITE_BUILD
}



void fnSetGrowMode(uint16_t growFlag) {
  if(growFlag) {
    setSystemFlag(FLAG_GROW);
  }
  else {
    clearSystemFlag(FLAG_GROW);
  }
}



void fnIncDecI(uint16_t mode) {
#ifndef TESTSUITE_BUILD
  callByIndexedMatrix((mode == DEC_FLAG) ? decIReal : incIReal, (mode == DEC_FLAG) ? decIComplex : incIComplex);
#endif // TESTSUITE_BUILD
}



void fnIncDecJ(uint16_t mode) {
#ifndef TESTSUITE_BUILD
  callByIndexedMatrix((mode == DEC_FLAG) ? decJReal : incJReal, (mode == DEC_FLAG) ? decJComplex : incJComplex);
#endif // TESTSUITE_BUILD
}



void fnInsRow(uint16_t unusedParamButMandatory) {
#ifndef TESTSUITE_BUILD
  if(calcMode == CM_MIM) {
    mimEnter(false);
    if(getRegisterDataType(matrixIndex) == dtReal34Matrix) {
      insRowRealMatrix(&openMatrixMIMPointer.realMatrix, getIRegisterAsInt(true));
    }
    else {
      insRowComplexMatrix(&openMatrixMIMPointer.complexMatrix, getIRegisterAsInt(true));
    }
    mimEnter(true);
  }
  else {
    displayCalcErrorMessage(ERROR_OPERATION_UNDEFINED, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #ifdef PC_BUILD
      sprintf(errorMessage, "works in MIM only");
      moreInfoOnError("In function fnGoToElement:", errorMessage, NULL, NULL);
    #endif
  }
#endif // TESTSUITE_BUILD
}



void fnDelRow(uint16_t unusedParamButMandatory) {
#ifndef TESTSUITE_BUILD
  if(calcMode == CM_MIM) {
    mimEnter(false);
    if(openMatrixMIMPointer.header.matrixRows > 1) {
      if(getRegisterDataType(matrixIndex) == dtReal34Matrix) {
        delRowRealMatrix(&openMatrixMIMPointer.realMatrix, getIRegisterAsInt(true));
      }
      else {
        delRowComplexMatrix(&openMatrixMIMPointer.complexMatrix, getIRegisterAsInt(true));
      }
    }
    mimEnter(true);
  }
  else {
    displayCalcErrorMessage(ERROR_OPERATION_UNDEFINED, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #ifdef PC_BUILD
      sprintf(errorMessage, "works in MIM only");
      moreInfoOnError("In function fnGoToElement:", errorMessage, NULL, NULL);
    #endif
  }
#endif // TESTSUITE_BUILD
}



void fnSetMatrixDimensions(uint16_t regist) {
#ifndef TESTSUITE_BUILD
  uint32_t y, x;

  if(!getDimensionArg(&y, &x)) {
  }
  else if(redimMatrixRegister(regist, y, x)) {
  }
  else {
    displayCalcErrorMessage(ERROR_NOT_ENOUGH_MEMORY_FOR_NEW_MATRIX, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "Not enough memory for a %" PRIu32 STD_CROSS "%" PRIu32 " matrix", y, x);
      moreInfoOnError("In function fnSetMatrixDimensions:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return;
  }
#endif // TESTSUITE_BUILD
}




void fnGetMatrixDimensions(uint16_t unusedButMandatoryParameter) {
#ifndef TESTSUITE_BUILD
  if(!saveLastX()) return;

  if(getRegisterDataType(REGISTER_X) == dtReal34Matrix || getRegisterDataType(REGISTER_X) == dtComplex34Matrix) {
    const uint16_t rows = REGISTER_DATA(REGISTER_X)->matrixRows;
    const uint16_t cols = REGISTER_DATA(REGISTER_X)->matrixColumns;
    longInteger_t li;

    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    liftStack();
    reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);

    longIntegerInit(li);
    uIntToLongInteger(rows, li);
    convertLongIntegerToLongIntegerRegister(li, REGISTER_Y);
    uIntToLongInteger(cols, li);
    convertLongIntegerToLongIntegerRegister(li, REGISTER_X);
    longIntegerFree(li);
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #ifdef PC_BUILD
    sprintf(errorMessage, "DataType %" PRIu32, getRegisterDataType(REGISTER_X));
    moreInfoOnError("In function fnGetMatrixDimensions:", errorMessage, "is not a matrix.", "");
    #endif
  }

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
#endif // TESTSUITE_BUILD
}




void fnTranspose(uint16_t unusedButMandatoryParameter) {
#ifndef TESTSUITE_BUILD
  if(!saveLastX()) return;

  if(getRegisterDataType(REGISTER_X) == dtReal34Matrix) {
    real34Matrix_t x;

    linkToRealMatrixRegister(REGISTER_X, &x);
    transposeRealMatrix(&x, &x);
    REGISTER_REAL34_MATRIX_DBLOCK(REGISTER_X)->matrixRows    = x.header.matrixRows;
    REGISTER_REAL34_MATRIX_DBLOCK(REGISTER_X)->matrixColumns = x.header.matrixColumns;
  }
  else if(getRegisterDataType(REGISTER_X) == dtComplex34Matrix) {
    complex34Matrix_t x;

    linkToComplexMatrixRegister(REGISTER_X, &x);
    transposeComplexMatrix(&x, &x);
    REGISTER_REAL34_MATRIX_DBLOCK(REGISTER_X)->matrixRows    = x.header.matrixRows;
    REGISTER_REAL34_MATRIX_DBLOCK(REGISTER_X)->matrixColumns = x.header.matrixColumns;
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #ifdef PC_BUILD
    sprintf(errorMessage, "DataType %" PRIu32, getRegisterDataType(REGISTER_X));
    moreInfoOnError("In function fnTranspose:", errorMessage, "is not a matrix.", "");
    #endif
  }

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
#endif // TESTSUITE_BUILD
}




void fnLuDecomposition(uint16_t unusedParamButMandatory) {
#ifndef TESTSUITE_BUILD
  if(!saveLastX()) return;

  if(getRegisterDataType(REGISTER_X) == dtReal34Matrix) {
    real34Matrix_t x, l, u;
    uint16_t *p, i, j;

    convertReal34MatrixRegisterToReal34Matrix(REGISTER_X, &x);

    if(x.header.matrixRows != x.header.matrixColumns) {
      displayCalcErrorMessage(ERROR_MATRIX_MISMATCH, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "not a square matrix (%d" STD_CROSS "%d)",
                x.header.matrixRows, x.header.matrixColumns);
        moreInfoOnError("In function fnLuDecomposition:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
    else {
      if((p = allocWp43s(x.header.matrixRows * sizeof(uint16_t)))) {
        WP34S_LU_decomposition(&x, &l, p);
        if(l.matrixElements) {
          copyRealMatrix(&l, &u);
          if(u.matrixElements) {
            for(i = 0; i < l.header.matrixRows; ++i) {
              for(j = i; j < l.header.matrixColumns; ++j) {
                real34Copy(i == j ? const34_1 : const34_0, &l.matrixElements[i * l.header.matrixColumns + j]);
              }
            }
            for(i = 1; i < u.header.matrixRows; ++i) {
              for(j = 0; j < i; ++j) {
                real34Copy(const34_0, &u.matrixElements[i * u.header.matrixColumns + j]);
              }
            }
            realMatrixFree(&x);
            realMatrixIdentity(&x, l.header.matrixColumns);
            for(uint16_t i = 0; i < l.header.matrixColumns; ++i) {
              realMatrixSwapRows(&x, &x, i, p[i]);
            }
            transposeRealMatrix(&x, &x);
            liftStack();
            liftStack();
            convertReal34MatrixToReal34MatrixRegister(&x, REGISTER_Z);
            if(lastErrorCode == ERROR_NONE) {
              convertReal34MatrixToReal34MatrixRegister(&l, REGISTER_Y);
              if(lastErrorCode == ERROR_NONE) {
                convertReal34MatrixToReal34MatrixRegister(&u, REGISTER_X);
              }
            }
            setSystemFlag(FLAG_ASLIFT);
            realMatrixFree(&u);
          }
          realMatrixFree(&l);
        }
        else {
          displayCalcErrorMessage(ERROR_SINGULAR_MATRIX, ERR_REGISTER_LINE, REGISTER_X);
          #if (EXTRA_INFO_ON_CALC_ERROR == 1)
            sprintf(errorMessage, "attempt to LU-decompose a singular matrix");
            moreInfoOnError("In function fnLuDecomposition:", errorMessage, NULL, NULL);
          #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
        }
        freeWp43s(p, x.header.matrixRows * sizeof(uint16_t));
      }
      else lastErrorCode = ERROR_RAM_FULL;
    }

    realMatrixFree(&x);
  }
  else if(getRegisterDataType(REGISTER_X) == dtComplex34Matrix) {
    complex34Matrix_t x, l, u;
    real34Matrix_t pivot;
    uint16_t *p, i, j;

    convertComplex34MatrixRegisterToComplex34Matrix(REGISTER_X, &x);

    if(x.header.matrixRows != x.header.matrixColumns) {
      displayCalcErrorMessage(ERROR_MATRIX_MISMATCH, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "not a square matrix (%d" STD_CROSS "%d)",
                x.header.matrixRows, x.header.matrixColumns);
        moreInfoOnError("In function fnLuDecomposition:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
    else {
      if((p = allocWp43s(x.header.matrixRows * sizeof(uint16_t)))) {
        complex_LU_decomposition(&x, &l, p);
        if(l.matrixElements) {
          copyComplexMatrix(&l, &u);
          if(u.matrixElements) {
            for(i = 0; i < l.header.matrixRows; ++i) {
              for(j = i; j < l.header.matrixColumns; ++j) {
                real34Copy(i == j ? const34_1 : const34_0, VARIABLE_REAL34_DATA(&l.matrixElements[i * l.header.matrixColumns + j]));
                real34Copy(                     const34_0, VARIABLE_IMAG34_DATA(&l.matrixElements[i * l.header.matrixColumns + j]));
              }
            }
            for(i = 1; i < u.header.matrixRows; ++i) {
              for(j = 0; j < i; ++j) {
                real34Copy(const34_0, VARIABLE_REAL34_DATA(&u.matrixElements[i * u.header.matrixColumns + j]));
                real34Copy(const34_0, VARIABLE_IMAG34_DATA(&u.matrixElements[i * u.header.matrixColumns + j]));
              }
            }
            realMatrixIdentity(&pivot, l.header.matrixColumns);
            if(lastErrorCode == ERROR_NONE) {
              for(uint16_t i = 0; i < l.header.matrixColumns; ++i) {
                realMatrixSwapRows(&pivot, &pivot, i, p[i]);
              }
              transposeRealMatrix(&pivot, &pivot);
              liftStack();
              liftStack();
              setSystemFlag(FLAG_ASLIFT);
              convertReal34MatrixToReal34MatrixRegister(&pivot, REGISTER_Z);
              if(lastErrorCode == ERROR_NONE) {
                convertComplex34MatrixToComplex34MatrixRegister(&l, REGISTER_Y);
                if(lastErrorCode == ERROR_NONE) {
                  convertComplex34MatrixToComplex34MatrixRegister(&u, REGISTER_X);
                }
              }
              realMatrixFree(&pivot);
            }
            else lastErrorCode = ERROR_RAM_FULL;
            complexMatrixFree(&u);
          }
          else lastErrorCode = ERROR_RAM_FULL;
          complexMatrixFree(&l);
        }
        else {
          displayCalcErrorMessage(ERROR_SINGULAR_MATRIX, ERR_REGISTER_LINE, REGISTER_X);
          #if (EXTRA_INFO_ON_CALC_ERROR == 1)
            sprintf(errorMessage, "attempt to LU-decompose a singular matrix");
            moreInfoOnError("In function fnLuDecomposition:", errorMessage, NULL, NULL);
          #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
        }
        freeWp43s(p, x.header.matrixRows * sizeof(uint16_t));
      }
      else lastErrorCode = ERROR_RAM_FULL;
    }

    complexMatrixFree(&x);
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #ifdef PC_BUILD
    sprintf(errorMessage, "DataType %" PRIu32, getRegisterDataType(REGISTER_X));
    moreInfoOnError("In function fnLuDecomposition:", errorMessage, "is not a matrix.", "");
    #endif
  }

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
#endif // TESTSUITE_BUILD
}



void fnDeterminant(uint16_t unusedParamButMandatory) {
#ifndef TESTSUITE_BUILD
  if(!saveLastX()) return;

  if(getRegisterDataType(REGISTER_X) == dtReal34Matrix) {
    real34Matrix_t x;
    real34_t res;

    linkToRealMatrixRegister(REGISTER_X, &x);

    if(x.header.matrixRows != x.header.matrixColumns) {
      displayCalcErrorMessage(ERROR_MATRIX_MISMATCH, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "not a square matrix (%d" STD_CROSS "%d)",
                x.header.matrixRows, x.header.matrixColumns);
        moreInfoOnError("In function fnDeterminant:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
    else {
      detRealMatrix(&x, &res);
      if(lastErrorCode == ERROR_NONE) {
        reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
        real34Copy(&res, REGISTER_REAL34_DATA(REGISTER_X));
      }
    }
  }
  else if(getRegisterDataType(REGISTER_X) == dtComplex34Matrix) {
    complex34Matrix_t x;
    real34_t res_r, res_i;

    linkToComplexMatrixRegister(REGISTER_X, &x);

    if(x.header.matrixRows != x.header.matrixColumns) {
      displayCalcErrorMessage(ERROR_MATRIX_MISMATCH, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "not a square matrix (%d" STD_CROSS "%d)",
                x.header.matrixRows, x.header.matrixColumns);
        moreInfoOnError("In function fnDeterminant:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
    else {
      detComplexMatrix(&x, &res_r, &res_i);
      if(lastErrorCode == ERROR_NONE) {
        reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
        real34Copy(&res_r, REGISTER_REAL34_DATA(REGISTER_X));
        real34Copy(&res_i, REGISTER_IMAG34_DATA(REGISTER_X));
      }
    }
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #ifdef PC_BUILD
    sprintf(errorMessage, "DataType %" PRIu32, getRegisterDataType(REGISTER_X));
    moreInfoOnError("In function fnLuDecomposition:", errorMessage, "is not a matrix.", "");
    #endif
  }

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
#endif // TESTSUITE_BUILD
}



void fnInvertMatrix(uint16_t unusedParamButMandatory) {
#ifndef TESTSUITE_BUILD
  if(!saveLastX()) return;

  if(getRegisterDataType(REGISTER_X) == dtReal34Matrix) {
    real34Matrix_t x, res;

    linkToRealMatrixRegister(REGISTER_X, &x);

    if(x.header.matrixRows != x.header.matrixColumns) {
      displayCalcErrorMessage(ERROR_MATRIX_MISMATCH, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "not a square matrix (%d" STD_CROSS "%d)",
                x.header.matrixRows, x.header.matrixColumns);
        moreInfoOnError("In function fnInvertMatrix:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
    else {
      WP34S_matrix_inverse(&x, &res);
      if(lastErrorCode == ERROR_NONE) {
        if(res.matrixElements) {
          convertReal34MatrixToReal34MatrixRegister(&res, REGISTER_X);
          realMatrixFree(&res);
          setSystemFlag(FLAG_ASLIFT);
        }
        else {
          displayCalcErrorMessage(ERROR_SINGULAR_MATRIX, ERR_REGISTER_LINE, REGISTER_X);
          #if (EXTRA_INFO_ON_CALC_ERROR == 1)
            sprintf(errorMessage, "attempt to invert a singular matrix");
            moreInfoOnError("In function fnInvertMatrix:", errorMessage, NULL, NULL);
          #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
        }
      }
      else {
        temporaryInformation = TI_NO_INFO;
      }
    }
  }
  else if(getRegisterDataType(REGISTER_X) == dtComplex34Matrix) {
    complex34Matrix_t x, res;

    linkToComplexMatrixRegister(REGISTER_X, &x);

    if(x.header.matrixRows != x.header.matrixColumns) {
      displayCalcErrorMessage(ERROR_MATRIX_MISMATCH, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "not a square matrix (%d" STD_CROSS "%d)",
                x.header.matrixRows, x.header.matrixColumns);
        moreInfoOnError("In function fnInvertMatrix:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
    else {
      complex_matrix_inverse(&x, &res);
      if(lastErrorCode == ERROR_NONE) {
        if(res.matrixElements) {
          convertComplex34MatrixToComplex34MatrixRegister(&res, REGISTER_X);
          complexMatrixFree(&res);
          setSystemFlag(FLAG_ASLIFT);
        }
        else {
          displayCalcErrorMessage(ERROR_SINGULAR_MATRIX, ERR_REGISTER_LINE, REGISTER_X);
          #if (EXTRA_INFO_ON_CALC_ERROR == 1)
            sprintf(errorMessage, "attempt to invert a singular matrix");
            moreInfoOnError("In function fnInvertMatrix:", errorMessage, NULL, NULL);
          #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
        }
      }
      else {
        temporaryInformation = TI_NO_INFO;
      }
    }
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #ifdef PC_BUILD
    sprintf(errorMessage, "DataType %" PRIu32, getRegisterDataType(REGISTER_X));
    moreInfoOnError("In function fnInvertMatrix:", errorMessage, "is not a matrix.", "");
    #endif
  }

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
#endif // TESTSUITE_BUILD
}



void fnEuclideanNorm(uint16_t unusedParamButMandatory) {
#ifndef TESTSUITE_BUILD
  if(!saveLastX()) return;

  if(getRegisterDataType(REGISTER_X) == dtReal34Matrix) {
    real34Matrix_t matrix;
    real34_t sum;

    linkToRealMatrixRegister(REGISTER_X, &matrix);

    euclideanNormRealMatrix(&matrix, &sum);

    // `matrix` invalidates here
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    real34Copy(&sum, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else if(getRegisterDataType(REGISTER_X) == dtComplex34Matrix) {
    complex34Matrix_t matrix;
    real34_t sum;

    linkToComplexMatrixRegister(REGISTER_X, &matrix);

    euclideanNormComplexMatrix(&matrix, &sum);

    // `matrix` invalidates here
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    real34Copy(&sum, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #ifdef PC_BUILD
    sprintf(errorMessage, "DataType %" PRIu32, getRegisterDataType(REGISTER_X));
    moreInfoOnError("In function fnInvertMatrix:", errorMessage, "is not a matrix.", "");
    #endif
  }

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
#endif // TESTSUITE_BUILD
}



void fnRowSum(uint16_t unusedParamButMandatory) {
#ifndef TESTSUITE_BUILD
  if(!saveLastX()) return;

  if(getRegisterDataType(REGISTER_X) == dtReal34Matrix) {
    real34Matrix_t x, res;
    real_t sum, elem;
    linkToRealMatrixRegister(REGISTER_X, &x);

    if(realMatrixInit(&res, x.header.matrixRows, 1)) {
      for(uint16_t i = 0; i < x.header.matrixRows; ++i) {
        realZero(&sum);
        for(uint16_t j = 0; j < x.header.matrixColumns; ++j) {
          real34ToReal(&x.matrixElements[i * x.header.matrixColumns + j], &elem);
          realAdd(&sum, &elem, &sum, &ctxtReal39);
        }
        realToReal34(&sum, &res.matrixElements[i]);
      }

      convertReal34MatrixToReal34MatrixRegister(&res, REGISTER_X);
      realMatrixFree(&res);
    }
    else lastErrorCode = ERROR_RAM_FULL;
  }
  else if(getRegisterDataType(REGISTER_X) == dtComplex34Matrix) {
    complex34Matrix_t x, res;
    real_t sumr, sumi, elem;
    linkToComplexMatrixRegister(REGISTER_X, &x);

    if(complexMatrixInit(&res, x.header.matrixRows, 1)) {
      for(uint16_t i = 0; i < x.header.matrixRows; ++i) {
        realZero(&sumr); realZero(&sumi);
        for(uint16_t j = 0; j < x.header.matrixColumns; ++j) {
          real34ToReal(VARIABLE_REAL34_DATA(&x.matrixElements[i * x.header.matrixColumns + j]), &elem);
          realAdd(&sumr, &elem, &sumr, &ctxtReal39);
          real34ToReal(VARIABLE_IMAG34_DATA(&x.matrixElements[i * x.header.matrixColumns + j]), &elem);
          realAdd(&sumi, &elem, &sumi, &ctxtReal39);
        }
        realToReal34(&sumr, VARIABLE_REAL34_DATA(&res.matrixElements[i]));
        realToReal34(&sumi, VARIABLE_IMAG34_DATA(&res.matrixElements[i]));
      }

      convertComplex34MatrixToComplex34MatrixRegister(&res, REGISTER_X);
      complexMatrixFree(&res);
    }
    else lastErrorCode = ERROR_RAM_FULL;
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #ifdef PC_BUILD
    sprintf(errorMessage, "DataType %" PRIu32, getRegisterDataType(REGISTER_X));
    moreInfoOnError("In function fnInvertMatrix:", errorMessage, "is not a matrix.", "");
    #endif
  }

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
#endif // TESTSUITE_BUILD
}



void fnRowNorm(uint16_t unusedParamButMandatory) {
#ifndef TESTSUITE_BUILD
  if(!saveLastX()) return;

  if(getRegisterDataType(REGISTER_X) == dtReal34Matrix) {
    real34Matrix_t x;
    real_t norm, sum, elem;
    linkToRealMatrixRegister(REGISTER_X, &x);

    realZero(&norm);
    for(uint16_t i = 0; i < x.header.matrixRows; ++i) {
      realZero(&sum);
      for(uint16_t j = 0; j < x.header.matrixColumns; ++j) {
        real34ToReal(&x.matrixElements[i * x.header.matrixColumns + j], &elem);
        realSetPositiveSign(&elem);
        realAdd(&sum, &elem, &sum, &ctxtReal39);
      }
      if(realCompareGreaterThan(&sum, &norm))
        realCopy(&sum, &norm);
    }

    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    convertRealToReal34ResultRegister(&norm, REGISTER_X);
  }
  else if(getRegisterDataType(REGISTER_X) == dtComplex34Matrix) {
    complex34Matrix_t x;
    real_t norm, sum, elem, imag;
    linkToComplexMatrixRegister(REGISTER_X, &x);

    realZero(&norm);
    for(uint16_t i = 0; i < x.header.matrixRows; ++i) {
      realZero(&sum);
      for(uint16_t j = 0; j < x.header.matrixColumns; ++j) {
        real34ToReal(VARIABLE_REAL34_DATA(&x.matrixElements[i * x.header.matrixColumns + j]), &elem);
        real34ToReal(VARIABLE_IMAG34_DATA(&x.matrixElements[i * x.header.matrixColumns + j]), &imag);
        complexMagnitude(&elem, &imag, &elem, &ctxtReal39);
        realAdd(&sum, &elem, &sum, &ctxtReal39);
      }
      if(realCompareGreaterThan(&sum, &norm))
        realCopy(&sum, &norm);
    }

    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    convertRealToReal34ResultRegister(&norm, REGISTER_X);
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #ifdef PC_BUILD
    sprintf(errorMessage, "DataType %" PRIu32, getRegisterDataType(REGISTER_X));
    moreInfoOnError("In function fnInvertMatrix:", errorMessage, "is not a matrix.", "");
    #endif
  }

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
#endif // TESTSUITE_BUILD
}



void fnVectorAngle(uint16_t unusedParamButMandatory) {
#ifndef TESTSUITE_BUILD
  if(!saveLastX()) return;

  if(getRegisterDataType(REGISTER_X) == dtReal34Matrix && getRegisterDataType(REGISTER_Y) == dtReal34Matrix) {
    real34Matrix_t x, y;
    real34_t res;
    linkToRealMatrixRegister(REGISTER_X, &x);
    linkToRealMatrixRegister(REGISTER_Y, &y);

    if((realVectorSize(&y) < 2) || (realVectorSize(&x) < 2) || (realVectorSize(&y) > 3) || (realVectorSize(&x) > 3) || (realVectorSize(&y) != realVectorSize(&x))) {
      displayCalcErrorMessage(ERROR_MATRIX_MISMATCH, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "invalid numbers of elements of %d" STD_CROSS "%d-matrix to %d" STD_CROSS "%d-matrix",
                x.header.matrixRows, x.header.matrixColumns,
                y.header.matrixRows, y.header.matrixColumns);
        moreInfoOnError("In function fnVectorAngle:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
    else {
      vectorAngle(&y, &x, &res);
      convertAngle34FromTo(&res, amRadian, currentAngularMode);
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, currentAngularMode);
      real34Copy(&res, REGISTER_REAL34_DATA(REGISTER_X));
    }
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #ifdef PC_BUILD
    sprintf(errorMessage, "DataType %" PRIu32, getRegisterDataType(REGISTER_X));
    moreInfoOnError("In function fnVectorAngle:", errorMessage, "is not a real matrix.", "");
    #endif
  }

  adjustResult(REGISTER_X, true, true, REGISTER_X, REGISTER_Y, -1);
#endif // TESTSUITE_BUILD
}



void fnIndexMatrix(uint16_t regist) {
#ifndef TESTSUITE_BUILD
  if((getRegisterDataType(regist) == dtReal34Matrix) || (getRegisterDataType(regist) == dtComplex34Matrix)) {
    matrixIndex = regist;
    setIRegisterAsInt(false, 1);
    setJRegisterAsInt(false, 1);
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #ifdef PC_BUILD
    sprintf(errorMessage, "DataType %" PRIu32, getRegisterDataType(regist));
    moreInfoOnError("In function fnIndexMatrix:", errorMessage, "is not a matrix.", "");
    #endif
  }
#endif // TESTSUITE_BUILD
}



void fnGetMatrix(uint16_t unusedParamButMandatory) {
#ifndef TESTSUITE_BUILD
  callByIndexedMatrix(getMatrixReal, getMatrixComplex);
#endif // TESTSUITE_BUILD
}



void fnPutMatrix(uint16_t unusedParamButMandatory) {
#ifndef TESTSUITE_BUILD
  callByIndexedMatrix(putMatrixReal, putMatrixComplex);
#endif // TESTSUITE_BUILD
}



void fnSwapRows(uint16_t unusedParamButMandatory) {
#ifndef TESTSUITE_BUILD
  callByIndexedMatrix(swapRowsReal, swapRowsComplex);
#endif // TESTSUITE_BUILD
}



void fnSimultaneousLinearEquation(uint16_t numberOfUnknowns) {
#ifndef TESTSUITE_BUILD
  if(allocateNamedMatrix("Mat_A", numberOfUnknowns, numberOfUnknowns) != INVALID_VARIABLE) {
    if(allocateNamedMatrix("Mat_B", numberOfUnknowns, 1) != INVALID_VARIABLE) {
      if(allocateNamedMatrix("Mat_X", numberOfUnknowns, 1) != INVALID_VARIABLE) {
        showSoftmenu(-MNU_SIMQ);
        showSoftmenu(-MNU_TAM);
        numberOfTamMenusToPop = 1;
      }
    }
  }
#endif // TESTSUITE_BUILD
}

void fnEditLinearEquationMatrixA(uint16_t unusedParamButMandatory) {
#ifndef TESTSUITE_BUILD
  fnEditMatrix(findNamedVariable("Mat_A"));
#endif // TESTSUITE_BUILD
}

void fnEditLinearEquationMatrixB(uint16_t unusedParamButMandatory) {
#ifndef TESTSUITE_BUILD
  fnEditMatrix(findNamedVariable("Mat_B"));
#endif // TESTSUITE_BUILD
}

void fnEditLinearEquationMatrixX(uint16_t unusedParamButMandatory) {
#ifndef TESTSUITE_BUILD
  if(findNamedVariable("Mat_A") == INVALID_VARIABLE || findNamedVariable("Mat_B") == INVALID_VARIABLE || findNamedVariable("Mat_X") == INVALID_VARIABLE) {
    displayCalcErrorMessage(ERROR_UNDEF_SOURCE_VAR, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "At least one of Mat_A, Mat_B or Mat_X is missing");
      moreInfoOnError("In function fnEditLinearEquationMatrixX:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
  else if(getRegisterDataType(findNamedVariable("Mat_A")) == dtReal34Matrix && getRegisterDataType(findNamedVariable("Mat_B")) == dtReal34Matrix) {
    real34Matrix_t a, b, x;
    linkToRealMatrixRegister(findNamedVariable("Mat_A"), &a);
    linkToRealMatrixRegister(findNamedVariable("Mat_B"), &b);
    WP34S_matrix_linear_eqn(&a, &b, &x);
    if(x.matrixElements) {
      convertReal34MatrixToReal34MatrixRegister(&x, findNamedVariable("Mat_X"));
      realMatrixFree(&x);
    }
  }
  else if(getRegisterDataType(findNamedVariable("Mat_A")) == dtReal34Matrix && getRegisterDataType(findNamedVariable("Mat_B")) == dtComplex34Matrix) {
    complex34Matrix_t a, b, x;
    convertReal34MatrixRegisterToComplex34Matrix(findNamedVariable("Mat_A"), &a);
    linkToComplexMatrixRegister(findNamedVariable("Mat_B"), &b);
    complex_matrix_linear_eqn(&a, &b, &x);
    if(x.matrixElements) {
      convertComplex34MatrixToComplex34MatrixRegister(&x, findNamedVariable("Mat_X"));
      complexMatrixFree(&x);
    }
    complexMatrixFree(&a);
  }
  else if(getRegisterDataType(findNamedVariable("Mat_A")) == dtComplex34Matrix && getRegisterDataType(findNamedVariable("Mat_B")) == dtReal34Matrix) {
    complex34Matrix_t a, b, x;
    linkToComplexMatrixRegister(findNamedVariable("Mat_A"), &a);
    convertReal34MatrixRegisterToComplex34Matrix(findNamedVariable("Mat_B"), &b);
    complex_matrix_linear_eqn(&a, &b, &x);
    if(x.matrixElements) {
      convertComplex34MatrixToComplex34MatrixRegister(&x, findNamedVariable("Mat_X"));
      complexMatrixFree(&x);
    }
    complexMatrixFree(&b);
  }
  else if(getRegisterDataType(findNamedVariable("Mat_A")) == dtComplex34Matrix && getRegisterDataType(findNamedVariable("Mat_B")) == dtComplex34Matrix) {
    complex34Matrix_t a, b, x;
    linkToComplexMatrixRegister(findNamedVariable("Mat_A"), &a);
    linkToComplexMatrixRegister(findNamedVariable("Mat_B"), &b);
    complex_matrix_linear_eqn(&a, &b, &x);
    if(x.matrixElements) {
      convertComplex34MatrixToComplex34MatrixRegister(&x, findNamedVariable("Mat_X"));
      complexMatrixFree(&x);
    }
  }
  if(lastErrorCode == ERROR_NONE) {
    liftStack();
    copySourceRegisterToDestRegister(findNamedVariable("Mat_X"), REGISTER_X);
    popSoftmenu();
  }
#endif // TESTSUITE_BUILD
}



void fnQrDecomposition(uint16_t unusedParamButMandatory) {
#ifndef TESTSUITE_BUILD
  if(!saveLastX()) return;

  if(getRegisterDataType(REGISTER_X) == dtReal34Matrix) {
    real34Matrix_t x, q, r;

    linkToRealMatrixRegister(REGISTER_X, &x);

    if(x.header.matrixRows != x.header.matrixColumns) {
      displayCalcErrorMessage(ERROR_MATRIX_MISMATCH, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "not a square matrix (%d" STD_CROSS "%d)",
                x.header.matrixRows, x.header.matrixColumns);
        moreInfoOnError("In function fnQrDecomposition:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
    else {
      real_QR_decomposition(&x, &q, &r);
      setSystemFlag(FLAG_ASLIFT);
      liftStack();
      convertReal34MatrixToReal34MatrixRegister(&q, REGISTER_Y);
      convertReal34MatrixToReal34MatrixRegister(&r, REGISTER_X);
      realMatrixFree(&q);
      realMatrixFree(&r);
    }
  }
  else if(getRegisterDataType(REGISTER_X) == dtComplex34Matrix) {
    complex34Matrix_t x, q, r;

    linkToComplexMatrixRegister(REGISTER_X, &x);

    if(x.header.matrixRows != x.header.matrixColumns) {
      displayCalcErrorMessage(ERROR_MATRIX_MISMATCH, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "not a square matrix (%d" STD_CROSS "%d)",
                x.header.matrixRows, x.header.matrixColumns);
        moreInfoOnError("In function fnQrDecomposition:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
    else {
      complex_QR_decomposition(&x, &q, &r);
      setSystemFlag(FLAG_ASLIFT);
      liftStack();
      convertComplex34MatrixToComplex34MatrixRegister(&q, REGISTER_Y);
      convertComplex34MatrixToComplex34MatrixRegister(&r, REGISTER_X);
      complexMatrixFree(&q);
      complexMatrixFree(&r);
    }
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #ifdef PC_BUILD
    sprintf(errorMessage, "DataType %" PRIu32, getRegisterDataType(REGISTER_X));
    moreInfoOnError("In function fnQrDecomposition:", errorMessage, "is not a matrix.", "");
    #endif
  }

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
#endif // TESTSUITE_BUILD
}



void fnEigenvalues(uint16_t unusedParamButMandatory) {
#ifndef TESTSUITE_BUILD
  if(!saveLastX()) return;

  if(getRegisterDataType(REGISTER_X) == dtReal34Matrix) {
    real34Matrix_t x, res, ires;

    linkToRealMatrixRegister(REGISTER_X, &x);

    if(x.header.matrixRows != x.header.matrixColumns && x.header.matrixRows >= 2) {
      displayCalcErrorMessage(ERROR_MATRIX_MISMATCH, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "rectangular or single-element matrix or (%d" STD_CROSS "%d)",
                x.header.matrixRows, x.header.matrixColumns);
        moreInfoOnError("In function fnEigenvalues:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
    else {
      hourGlassIconEnabled = true;
      showHideHourGlass();
      #ifdef DMCP_BUILD
        lcd_refresh();
      #else // !DMCP_BUILD
        refreshLcd(NULL);
      #endif // DMCP_BUILD

      setSystemFlag(FLAG_ASLIFT);
      liftStack();
      ires.header.matrixRows = ires.header.matrixColumns = 0;
      ires.matrixElements = NULL;
      realEigenvalues(&x, &res, &ires);
      if(lastErrorCode == ERROR_NONE) {
        if(ires.matrixElements) {
          complex34Matrix_t cres;
          if(complexMatrixInit(&cres, res.header.matrixRows, res.header.matrixColumns)) {
            for(uint32_t i = 0; i < x.header.matrixRows * x.header.matrixColumns; i++) {
              real34Copy(&res.matrixElements[i],  VARIABLE_REAL34_DATA(&cres.matrixElements[i]));
              real34Copy(&ires.matrixElements[i], VARIABLE_IMAG34_DATA(&cres.matrixElements[i]));
            }
            convertComplex34MatrixToComplex34MatrixRegister(&cres, REGISTER_X);
            realMatrixFree(&ires);
            complexMatrixFree(&cres);
          }
          else lastErrorCode = ERROR_RAM_FULL;
        }
        else {
          convertReal34MatrixToReal34MatrixRegister(&res, REGISTER_X);
        }
        realMatrixFree(&res);
      }
    }
  }
  else if(getRegisterDataType(REGISTER_X) == dtComplex34Matrix) {
    complex34Matrix_t x, res;

    linkToComplexMatrixRegister(REGISTER_X, &x);

    if(x.header.matrixRows != x.header.matrixColumns && x.header.matrixRows >= 2) {
      displayCalcErrorMessage(ERROR_MATRIX_MISMATCH, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "rectangular or single-element matrix or (%d" STD_CROSS "%d)",
                x.header.matrixRows, x.header.matrixColumns);
        moreInfoOnError("In function fnEigenvalues:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
    else {
      hourGlassIconEnabled = true;
      showHideHourGlass();
      #ifdef DMCP_BUILD
        lcd_refresh();
      #else // !DMCP_BUILD
        refreshLcd(NULL);
      #endif // DMCP_BUILD

      setSystemFlag(FLAG_ASLIFT);
      liftStack();
      complexEigenvalues(&x, &res);
      convertComplex34MatrixToComplex34MatrixRegister(&res, REGISTER_X);
      complexMatrixFree(&res);
    }
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #ifdef PC_BUILD
    sprintf(errorMessage, "DataType %" PRIu32, getRegisterDataType(REGISTER_X));
    moreInfoOnError("In function fnEigenvalues:", errorMessage, "is not a matrix.", "");
    #endif
  }

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
#endif // TESTSUITE_BUILD
}

void fnEigenvectors(uint16_t unusedParamButMandatory) {
#ifndef TESTSUITE_BUILD
  if(!saveLastX()) return;

  if(getRegisterDataType(REGISTER_X) == dtReal34Matrix) {
    real34Matrix_t x, res, ires;

    linkToRealMatrixRegister(REGISTER_X, &x);

    if(x.header.matrixRows != x.header.matrixColumns && x.header.matrixRows >= 2) {
      displayCalcErrorMessage(ERROR_MATRIX_MISMATCH, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "rectangular or single-element matrix or (%d" STD_CROSS "%d)",
                x.header.matrixRows, x.header.matrixColumns);
        moreInfoOnError("In function fnEigenvectors:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
    else {
      hourGlassIconEnabled = true;
      showHideHourGlass();
      #ifdef DMCP_BUILD
        lcd_refresh();
      #else // !DMCP_BUILD
        refreshLcd(NULL);
      #endif // DMCP_BUILD

      setSystemFlag(FLAG_ASLIFT);
      liftStack();
      ires.header.matrixRows = ires.header.matrixColumns = 0;
      ires.matrixElements = NULL;
      realEigenvectors(&x, &res, &ires);
      if(ires.matrixElements) {
        complex34Matrix_t cres;
        if(complexMatrixInit(&cres, res.header.matrixRows, res.header.matrixColumns)) {
          for(uint32_t i = 0; i < x.header.matrixRows * x.header.matrixColumns; i++) {
            real34Copy(&res.matrixElements[i],  VARIABLE_REAL34_DATA(&cres.matrixElements[i]));
            real34Copy(&ires.matrixElements[i], VARIABLE_IMAG34_DATA(&cres.matrixElements[i]));
          }
          convertComplex34MatrixToComplex34MatrixRegister(&cres, REGISTER_X);
          realMatrixFree(&ires);
          complexMatrixFree(&cres);
        }
        else lastErrorCode = ERROR_RAM_FULL;
      }
      else {
        convertReal34MatrixToReal34MatrixRegister(&res, REGISTER_X);
      }
      realMatrixFree(&res);
    }
  }
  else if(getRegisterDataType(REGISTER_X) == dtComplex34Matrix) {
    complex34Matrix_t x, res;

    linkToComplexMatrixRegister(REGISTER_X, &x);

    if(x.header.matrixRows != x.header.matrixColumns && x.header.matrixRows >= 2) {
      displayCalcErrorMessage(ERROR_MATRIX_MISMATCH, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "rectangular or single-element matrix or (%d" STD_CROSS "%d)",
                x.header.matrixRows, x.header.matrixColumns);
        moreInfoOnError("In function fnEigenvectors:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
    else {
      hourGlassIconEnabled = true;
      showHideHourGlass();
      #ifdef DMCP_BUILD
        lcd_refresh();
      #else // !DMCP_BUILD
        refreshLcd(NULL);
      #endif // DMCP_BUILD

      setSystemFlag(FLAG_ASLIFT);
      liftStack();
      complexEigenvectors(&x, &res);
      convertComplex34MatrixToComplex34MatrixRegister(&res, REGISTER_X);
      complexMatrixFree(&res);
    }
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #ifdef PC_BUILD
    sprintf(errorMessage, "DataType %" PRIu32, getRegisterDataType(REGISTER_X));
    moreInfoOnError("In function fnEigenvectors:", errorMessage, "is not a matrix.", "");
    #endif
  }

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
#endif // TESTSUITE_BUILD
}



#ifndef TESTSUITE_BUILD
bool_t realMatrixInit(real34Matrix_t *matrix, uint16_t rows, uint16_t cols) {
  //Allocate Memory for Matrix
  const size_t neededSize = TO_BLOCKS((rows * cols) * sizeof(real34_t));
  if(!isMemoryBlockAvailable(neededSize)) {
    matrix->header.matrixColumns = matrix->header.matrixRows = 0;
    matrix->matrixElements = NULL;
    return false;
  }
  matrix->matrixElements = allocWp43s(neededSize);

  matrix->header.matrixColumns = cols;
  matrix->header.matrixRows = rows;

  //Initialize with 0.
  for(uint32_t i = 0; i < rows * cols; i++) {
    real34Copy(const34_0, &matrix->matrixElements[i]);
  }
  return true;
}


void realMatrixFree(real34Matrix_t *matrix) {
  uint16_t cols = matrix->header.matrixColumns;
  uint16_t rows = matrix->header.matrixRows;

  freeWp43s(matrix->matrixElements, TO_BLOCKS((rows * cols) * sizeof(real34_t)));
  matrix->matrixElements = NULL;
  matrix->header.matrixRows = matrix->header.matrixColumns = 0;
}


void realMatrixIdentity(real34Matrix_t *matrix, uint16_t size) {
  if(realMatrixInit(matrix, size, size)) {
    for(uint16_t i = 0; i < size; ++i) {
      real34Copy(const34_1, &matrix->matrixElements[i * size + i]);
    }
  }
  else lastErrorCode = ERROR_RAM_FULL;
}


void realMatrixRedim(real34Matrix_t *matrix, uint16_t rows, uint16_t cols) {
  real34Matrix_t newMatrix;
  uint32_t elements;

  if(realMatrixInit(&newMatrix, rows, cols)) {
    elements = matrix->header.matrixRows * matrix->header.matrixColumns;
    if(elements > rows * cols) elements = rows * cols;
    for(uint32_t i = 0; i < elements; ++i)
      real34Copy(&matrix->matrixElements[i], &newMatrix.matrixElements[i]);
    realMatrixFree(matrix);
    matrix->header.matrixRows = newMatrix.header.matrixRows;
    matrix->header.matrixColumns = newMatrix.header.matrixColumns;
    matrix->matrixElements = newMatrix.matrixElements;
  }
  else lastErrorCode = ERROR_RAM_FULL;
}


bool_t complexMatrixInit(complex34Matrix_t *matrix, uint16_t rows, uint16_t cols) {
  //Allocate Memory for Matrix
  const size_t neededSize = TO_BLOCKS((rows * cols) * sizeof(complex34_t));
  if(!isMemoryBlockAvailable(neededSize)) {
    matrix->header.matrixColumns = matrix->header.matrixRows = 0;
    matrix->matrixElements = NULL;
    return false;
  }
  matrix->matrixElements = allocWp43s(neededSize);

  matrix->header.matrixColumns = cols;
  matrix->header.matrixRows = rows;

  //Initialize with 0.
  for(uint32_t i = 0; i < rows * cols; i++) {
    real34Copy(const34_0, VARIABLE_REAL34_DATA(&matrix->matrixElements[i]));
    real34Copy(const34_0, VARIABLE_IMAG34_DATA(&matrix->matrixElements[i]));
  }
  return true;
}


void complexMatrixFree(complex34Matrix_t *matrix) {
  uint16_t cols = matrix->header.matrixColumns;
  uint16_t rows = matrix->header.matrixRows;

  freeWp43s(matrix->matrixElements, TO_BLOCKS((rows * cols) * sizeof(complex34_t)));
  matrix->matrixElements = NULL;
  matrix->header.matrixRows = matrix->header.matrixColumns = 0;
}


void complexMatrixIdentity(complex34Matrix_t *matrix, uint16_t size) {
  if(complexMatrixInit(matrix, size, size)) {
    for(uint16_t i = 0; i < size; ++i) {
      real34Copy(const34_1, VARIABLE_REAL34_DATA(&matrix->matrixElements[i * size + i]));
      real34Copy(const34_0, VARIABLE_IMAG34_DATA(&matrix->matrixElements[i * size + i]));
    }
  }
  else lastErrorCode = ERROR_RAM_FULL;
}


void complexMatrixRedim(complex34Matrix_t *matrix, uint16_t rows, uint16_t cols) {
  complex34Matrix_t newMatrix;
  uint32_t elements;

  if(complexMatrixInit(&newMatrix, rows, cols)) {
    elements = matrix->header.matrixRows * matrix->header.matrixColumns;
    if(elements > rows * cols) elements = rows * cols;
    for(uint32_t i = 0; i < elements; ++i) {
      complex34Copy(&matrix->matrixElements[i], &newMatrix.matrixElements[i]);
    }
    complexMatrixFree(matrix);
    matrix->header.matrixRows = newMatrix.header.matrixRows;
    matrix->header.matrixColumns = newMatrix.header.matrixColumns;
    matrix->matrixElements = newMatrix.matrixElements;
  }
  else lastErrorCode = ERROR_RAM_FULL;
}


void showMatrixEditor() {
  int rows = openMatrixMIMPointer.header.matrixRows;
  int cols = openMatrixMIMPointer.header.matrixColumns;
  int16_t width = 0;

  for(int i = 0; i < SOFTMENU_STACK_SIZE; i++) {
    if(softmenu[softmenuStack[i].softmenuId].menuItem == -MNU_M_EDIT) {
      width = 1;
      break;
    }
  }
  if(width == 0) {
    showSoftmenu(-MNU_M_EDIT);
  }
  #if defined(PC_BUILD) && (SCREEN_800X480 == 0)
  if(softmenu[softmenuStack[0].softmenuId].menuItem == -MNU_M_EDIT) {
    calcModeNormalGui();
  }
  #endif // PC_BUILD && (SCREEN_800X480 == 0)

  bool_t colVector = false;
  if(cols == 1 && rows > 1) {
    colVector = true;
    cols = rows;
    rows = 1;
  }

  if(wrapIJ(colVector ? cols : rows, colVector ? 1 : cols)) {
    if(getRegisterDataType(matrixIndex) == dtReal34Matrix) {
      insRowRealMatrix(&openMatrixMIMPointer.realMatrix, rows);
      convertReal34MatrixToReal34MatrixRegister(&openMatrixMIMPointer.realMatrix, matrixIndex);
    }
    else {
      insRowComplexMatrix(&openMatrixMIMPointer.complexMatrix, rows);
      convertComplex34MatrixToComplex34MatrixRegister(&openMatrixMIMPointer.complexMatrix, matrixIndex);
    }
  }

  int16_t matSelRow = colVector ? getJRegisterAsInt(true) : getIRegisterAsInt(true);
  int16_t matSelCol = colVector ? getIRegisterAsInt(true) : getJRegisterAsInt(true);

  if(matSelRow == 0 || rows <= 5) {
    scrollRow = 0;
  }
  else if(matSelRow == rows - 1) {
    scrollRow = matSelRow - 4;
  }
  else if(matSelRow < scrollRow + 1) {
    scrollRow = matSelRow - 1;
  }
  else if(matSelRow > scrollRow + 3) {
    scrollRow = matSelRow - 3;
  }

  if(getRegisterDataType(matrixIndex) == dtReal34Matrix)
    showRealMatrix(&openMatrixMIMPointer.realMatrix, 0);
  else
    showComplexMatrix(&openMatrixMIMPointer.complexMatrix, 0);

  sprintf(tmpString, "%" PRIi16";%" PRIi16"=%s%s", (int16_t)(colVector ? matSelCol+1 : matSelRow+1), (int16_t)(colVector ? 1 : matSelCol+1), (aimBuffer[0] == 0 || aimBuffer[0] == '-') ? "" : " ", nimBufferDisplay);
  width = stringWidth(tmpString, &numericFont, true, true) + 1;
  if(aimBuffer[0] == 0) {
    if(getRegisterDataType(matrixIndex) == dtReal34Matrix)
      real34ToDisplayString(&openMatrixMIMPointer.realMatrix.matrixElements[matSelRow*cols+matSelCol], amNone, &tmpString[strlen(tmpString)], &numericFont, SCREEN_WIDTH - width, NUMBER_OF_DISPLAY_DIGITS, true, STD_SPACE_4_PER_EM, true);
    else
      complex34ToDisplayString(&openMatrixMIMPointer.complexMatrix.matrixElements[matSelRow*cols+matSelCol], &tmpString[strlen(tmpString)], &numericFont, SCREEN_WIDTH - width, NUMBER_OF_DISPLAY_DIGITS, true, STD_SPACE_4_PER_EM, true);

    showString(tmpString, &numericFont, 0, Y_POSITION_OF_NIM_LINE, vmNormal, true, false);
  }
  else {
    displayNim(tmpString, "", 0, 0);
  }
}

void mimEnter(bool_t commit) {
  int cols = openMatrixMIMPointer.header.matrixColumns;
  int16_t row = getIRegisterAsInt(true);
  int16_t col = getJRegisterAsInt(true);

  if(aimBuffer[0] != 0) {
    if(getRegisterDataType(matrixIndex) == dtReal34Matrix) {
      real34_t *real34Ptr = &openMatrixMIMPointer.realMatrix.matrixElements[row * cols + col];

      if(nimNumberPart == NP_FRACTION_DENOMINATOR) {
        closeNimWithFraction(real34Ptr);
      }
      else if(nimNumberPart == NP_COMPLEX_INT_PART || nimNumberPart == NP_COMPLEX_FLOAT_PART || nimNumberPart == NP_COMPLEX_EXPONENT) {
        complex34_t *complex34Ptr;
        complex34Matrix_t cxma;
        convertReal34MatrixToComplex34Matrix(&openMatrixMIMPointer.realMatrix, &cxma);
        realMatrixFree(&openMatrixMIMPointer.realMatrix);
        convertComplex34MatrixToComplex34MatrixRegister(&cxma, matrixIndex);
        openMatrixMIMPointer.complexMatrix.header.matrixRows = cxma.header.matrixRows;
        openMatrixMIMPointer.complexMatrix.header.matrixColumns = cxma.header.matrixColumns;
        openMatrixMIMPointer.complexMatrix.matrixElements = cxma.matrixElements;
        complex34Ptr = &openMatrixMIMPointer.complexMatrix.matrixElements[row * cols + col];
        closeNimWithComplex(VARIABLE_REAL34_DATA(complex34Ptr), VARIABLE_IMAG34_DATA(complex34Ptr));
      }
      else {
        stringToReal34(aimBuffer, real34Ptr);
      }
    }
    else {
      complex34_t *complex34Ptr = &openMatrixMIMPointer.complexMatrix.matrixElements[row * cols + col];

      if(nimNumberPart == NP_FRACTION_DENOMINATOR) {
        closeNimWithFraction(VARIABLE_REAL34_DATA(complex34Ptr));
        real34Zero(VARIABLE_IMAG34_DATA(complex34Ptr));
      }
      else if(nimNumberPart == NP_COMPLEX_INT_PART || nimNumberPart == NP_COMPLEX_FLOAT_PART || nimNumberPart == NP_COMPLEX_EXPONENT) {
        closeNimWithComplex(VARIABLE_REAL34_DATA(complex34Ptr), VARIABLE_IMAG34_DATA(complex34Ptr));
      }
      else {
        stringToReal34(aimBuffer, VARIABLE_REAL34_DATA(complex34Ptr));
        real34Zero(VARIABLE_IMAG34_DATA(complex34Ptr));
      }
    }

    aimBuffer[0] = 0;
    nimBufferDisplay[0] = 0;
    hideCursor();
    cursorEnabled = false;

    setSystemFlag(FLAG_ASLIFT);
  }
  if(commit) {
    if(getRegisterDataType(matrixIndex) == dtReal34Matrix)
      convertReal34MatrixToReal34MatrixRegister(&openMatrixMIMPointer.realMatrix, matrixIndex);
    else
      convertComplex34MatrixToComplex34MatrixRegister(&openMatrixMIMPointer.complexMatrix, matrixIndex);
  }
  updateMatrixHeightCache();
}

void mimAddNumber(int16_t item) {
  const int cols = openMatrixMIMPointer.header.matrixColumns;
  const int16_t row = getIRegisterAsInt(true);
  const int16_t col = getJRegisterAsInt(true);

  switch(item) {
    case ITM_EXPONENT :
      if(aimBuffer[0] == 0) {
        aimBuffer[0] = '+';
        aimBuffer[1] = '1';
        aimBuffer[2] = '.';
        aimBuffer[3] = 0;
        nimNumberPart = NP_REAL_FLOAT_PART;
        cursorEnabled = true;
        cursorFont = &numericFont;
        lastIntegerBase = 0;
      }
      break;

    case ITM_PERIOD :
      if(aimBuffer[0] == 0) {
        aimBuffer[0] = '+';
        aimBuffer[1] = '0';
        aimBuffer[2] = 0;
        nimNumberPart = NP_INT_10;
        cursorEnabled = true;
        cursorFont = &numericFont;
        lastIntegerBase = 0;
      }
      break;

    case ITM_0 :
    case ITM_1 :
    case ITM_2 :
    case ITM_3 :
    case ITM_4 :
    case ITM_5 :
    case ITM_6 :
    case ITM_7 :
    case ITM_8 :
    case ITM_9 :
      if(aimBuffer[0] == 0) {
        aimBuffer[0] = '+';
        aimBuffer[1] = 0;
        nimNumberPart = NP_INT_10;
        cursorEnabled = true;
        cursorFont = &numericFont;
        lastIntegerBase = 0;
      }
      break;

    case ITM_BACKSPACE :
      if(aimBuffer[0] == 0) {
        const int cols = openMatrixMIMPointer.header.matrixColumns;
        const int16_t row = getIRegisterAsInt(true);
        const int16_t col = getJRegisterAsInt(true);

        if(getRegisterDataType(matrixIndex) == dtReal34Matrix) {
          real34Zero(&openMatrixMIMPointer.realMatrix.matrixElements[row * cols + col]);
        }
        else {
          real34Zero(VARIABLE_REAL34_DATA(&openMatrixMIMPointer.complexMatrix.matrixElements[row * cols + col]));
          real34Zero(VARIABLE_IMAG34_DATA(&openMatrixMIMPointer.complexMatrix.matrixElements[row * cols + col]));
        }
        setSystemFlag(FLAG_ASLIFT);
        return;
      }
      else if((aimBuffer[0] == '+') && (aimBuffer[1] != 0) && (aimBuffer[2] == 0)) {
        aimBuffer[1] = 0;
        hideCursor();
        cursorEnabled = false;
      }
      break;

    case ITM_CHS :
      if(aimBuffer[0] == 0) {
        if(getRegisterDataType(matrixIndex) == dtReal34Matrix) {
          real34ChangeSign(&openMatrixMIMPointer.realMatrix.matrixElements[row * cols + col]);
        }
        else {
          real34ChangeSign(VARIABLE_REAL34_DATA(&openMatrixMIMPointer.complexMatrix.matrixElements[row * cols + col]));
          real34ChangeSign(VARIABLE_IMAG34_DATA(&openMatrixMIMPointer.complexMatrix.matrixElements[row * cols + col]));
        }
        setSystemFlag(FLAG_ASLIFT);
        return;
      }
      break;

    case ITM_CC :
      if(aimBuffer[0] == 0) return;
      break;

    case ITM_CONSTpi :
      if(aimBuffer[0] == 0) {
        if(getRegisterDataType(matrixIndex) == dtReal34Matrix) {
          realToReal34(const_pi, &openMatrixMIMPointer.realMatrix.matrixElements[row * cols + col]);
        }
        else {
          realToReal34(const_pi, VARIABLE_REAL34_DATA(&openMatrixMIMPointer.complexMatrix.matrixElements[row * cols + col]));
          real34Zero(VARIABLE_IMAG34_DATA(&openMatrixMIMPointer.complexMatrix.matrixElements[row * cols + col]));
        }
      }
      else if(nimNumberPart == NP_COMPLEX_INT_PART && aimBuffer[strlen(aimBuffer) - 1] == 'i') {
        strcat(aimBuffer, "3.141592653589793238462643383279503");
        reallyRunFunction(ITM_ENTER, NOPARAM);
      }
      return;

    default:
      return;
  }
  addItemToNimBuffer(item);
  calcMode = CM_MIM;
}

void mimRunFunction(int16_t func, uint16_t param) {
  int16_t i = getIRegisterAsInt(true);
  int16_t j = getJRegisterAsInt(true);
  bool_t isComplex = (getRegisterDataType(matrixIndex) == dtComplex34Matrix);
  real34_t re, im, re1, im1;
  bool_t converted = false;
  bool_t liftStackFlag = getSystemFlag(FLAG_ASLIFT);

  if(isComplex) {
    real34Copy(VARIABLE_REAL34_DATA(&openMatrixMIMPointer.complexMatrix.matrixElements[i * openMatrixMIMPointer.header.matrixColumns + j]), &re1);
    real34Copy(VARIABLE_IMAG34_DATA(&openMatrixMIMPointer.complexMatrix.matrixElements[i * openMatrixMIMPointer.header.matrixColumns + j]), &im1);
  }
  else {
    real34Copy(&openMatrixMIMPointer.realMatrix.matrixElements[i * openMatrixMIMPointer.header.matrixColumns + j], &re1);
    real34Zero(&im1);
  }

  lastErrorCode = ERROR_NONE;
  mimEnter(true);
  clearSystemFlag(FLAG_ASLIFT);

  if(isComplex) {
    real34Copy(VARIABLE_REAL34_DATA(&openMatrixMIMPointer.complexMatrix.matrixElements[i * openMatrixMIMPointer.header.matrixColumns + j]), &re);
    real34Copy(VARIABLE_IMAG34_DATA(&openMatrixMIMPointer.complexMatrix.matrixElements[i * openMatrixMIMPointer.header.matrixColumns + j]), &im);
  }
  else {
    real34Copy(&openMatrixMIMPointer.realMatrix.matrixElements[i * openMatrixMIMPointer.header.matrixColumns + j], &re);
    real34Zero(&im);
  }
  if(isComplex) {
    reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
    real34Copy(&re, REGISTER_REAL34_DATA(REGISTER_X));
    real34Copy(&im, REGISTER_IMAG34_DATA(REGISTER_X));
  }
  else {
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    real34Copy(&re, REGISTER_REAL34_DATA(REGISTER_X));
  }

  reallyRunFunction(func, param);

  switch(getRegisterDataType(REGISTER_X)) {
    case dtLongInteger:
      convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
      break;
    case dtShortInteger:
      convertShortIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
      break;
    case dtReal34:
    case dtComplex34:
      break;
    default:
      lastErrorCode = ERROR_INVALID_DATA_TYPE_FOR_OP;
  }

  if(lastErrorCode == ERROR_NONE) {
    if(isComplex && getRegisterDataType(REGISTER_X) == dtComplex34) {
      complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_X), &openMatrixMIMPointer.complexMatrix.matrixElements[i * openMatrixMIMPointer.header.matrixColumns + j]);
    }
    else if(isComplex) {
      real34Copy(REGISTER_REAL34_DATA(REGISTER_X), VARIABLE_REAL34_DATA(&openMatrixMIMPointer.complexMatrix.matrixElements[i * openMatrixMIMPointer.header.matrixColumns + j]));
      real34Zero(                                  VARIABLE_IMAG34_DATA(&openMatrixMIMPointer.complexMatrix.matrixElements[i * openMatrixMIMPointer.header.matrixColumns + j]));
    }
    else if(getRegisterDataType(REGISTER_X) == dtComplex34) { // Convert to a complex matrix
      complex34Matrix_t cxma;
      complex34_t ans;

      complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_X), &ans);
      converted = true;
      convertReal34MatrixToComplex34Matrix(&openMatrixMIMPointer.realMatrix, &cxma);
      realMatrixFree(&openMatrixMIMPointer.realMatrix);
      convertComplex34MatrixToComplex34MatrixRegister(&cxma, matrixIndex);
      openMatrixMIMPointer.complexMatrix.header.matrixRows = cxma.header.matrixRows;
      openMatrixMIMPointer.complexMatrix.header.matrixColumns = cxma.header.matrixColumns;
      openMatrixMIMPointer.complexMatrix.matrixElements = cxma.matrixElements;

      complex34Copy(&ans, &openMatrixMIMPointer.complexMatrix.matrixElements[i * openMatrixMIMPointer.header.matrixColumns + j]);
    }
    else {
      real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &openMatrixMIMPointer.realMatrix.matrixElements[i * openMatrixMIMPointer.header.matrixColumns + j]);
    }
  }

  if(matrixIndex == REGISTER_X && !converted) {
    if(isComplex) {
      complex34Matrix_t linkedMatrix;
      convertComplex34MatrixToComplex34MatrixRegister(&openMatrixMIMPointer.complexMatrix, REGISTER_X);
      linkToComplexMatrixRegister(REGISTER_X, &linkedMatrix);
      real34Copy(&re1, VARIABLE_REAL34_DATA(&linkedMatrix.matrixElements[i * linkedMatrix.header.matrixColumns + j]));
      real34Copy(&im1, VARIABLE_IMAG34_DATA(&linkedMatrix.matrixElements[i * linkedMatrix.header.matrixColumns + j]));
    }
    else {
      real34Matrix_t linkedMatrix;
      convertReal34MatrixToReal34MatrixRegister(&openMatrixMIMPointer.realMatrix, REGISTER_X);
      linkToRealMatrixRegister(REGISTER_X, &linkedMatrix);
      real34Copy(&re1, &linkedMatrix.matrixElements[i * linkedMatrix.header.matrixColumns + j]);
    }
  }

  if(liftStackFlag) setSystemFlag(FLAG_ASLIFT);

  updateMatrixHeightCache();
  #ifdef PC_BUILD
    refreshLcd(NULL);
  #endif // PC_BUILD
}

void mimFinalize(void) {
  if(getRegisterDataType(matrixIndex) == dtReal34Matrix) {
    if(openMatrixMIMPointer.realMatrix.matrixElements)
      realMatrixFree(&openMatrixMIMPointer.realMatrix);
  }
  else if(getRegisterDataType(matrixIndex) == dtComplex34Matrix) {
    if(openMatrixMIMPointer.complexMatrix.matrixElements)
      complexMatrixFree(&openMatrixMIMPointer.complexMatrix);
  }
  matrixIndex = INVALID_VARIABLE;
}

void mimRestore(void) {
  uint16_t idx = matrixIndex;
  mimFinalize();
  if(idx != INVALID_VARIABLE) {
    getMatrixFromRegister(idx);
    matrixIndex = idx;
  }
}


void showRealMatrix(const real34Matrix_t *matrix, int16_t prefixWidth) {
  int rows = matrix->header.matrixRows;
  int cols = matrix->header.matrixColumns;
  int16_t Y_POS = Y_POSITION_OF_REGISTER_X_LINE;
  int16_t X_POS = 0;
  int16_t totalWidth = 0, width = 0;
  const font_t *font;
  int16_t fontHeight = NUMERIC_FONT_HEIGHT;
  int16_t maxWidth = MATRIX_LINE_WIDTH - prefixWidth;
  int16_t colWidth[MATRIX_MAX_COLUMNS] = {}, rPadWidth[MATRIX_MAX_ROWS * MATRIX_MAX_COLUMNS] = {};
  const bool_t forEditor = matrix == &openMatrixMIMPointer.realMatrix;
  const uint16_t sRow = forEditor ? scrollRow : 0;
  uint16_t sCol = forEditor ? scrollColumn : 0;
  const uint16_t tmpDisplayFormat = displayFormat;
  const uint8_t tmpDisplayFormatDigits = displayFormatDigits;

  Y_POS = Y_POSITION_OF_REGISTER_X_LINE - NUMERIC_FONT_HEIGHT;

  bool_t colVector = false;
  if(cols == 1 && rows > 1) {
    colVector = true;
    cols = rows;
    rows = 1;
  }

  uint16_t maxCols = cols > MATRIX_MAX_COLUMNS ? MATRIX_MAX_COLUMNS : cols;
  const uint16_t maxRows = rows > MATRIX_MAX_ROWS ? MATRIX_MAX_ROWS : rows;
  if(maxCols + sCol >= cols) maxCols = cols - sCol;

  int16_t matSelRow = colVector ? getJRegisterAsInt(true) : getIRegisterAsInt(true);
  int16_t matSelCol = colVector ? getIRegisterAsInt(true) : getJRegisterAsInt(true);

  videoMode_t vm = vmNormal;

  font = &numericFont;
  if(rows >= (forEditor ? 4 : 5)){
smallFont:
    font = &standardFont;
    fontHeight = STANDARD_FONT_HEIGHT;
    Y_POS = Y_POSITION_OF_REGISTER_X_LINE - STANDARD_FONT_HEIGHT + 2;
    //maxWidth = MATRIX_LINE_WIDTH_SMALL * 4 - 20;
  }

  if(!forEditor) Y_POS += REGISTER_LINE_HEIGHT;
  const bool_t rightEllipsis = (cols > maxCols) && (cols > maxCols + sCol);
  const bool_t leftEllipsis = (sCol > 0);
  int16_t digits;

  if(prefixWidth > 0) Y_POS = Y_POSITION_OF_REGISTER_T_LINE - REGISTER_LINE_HEIGHT + 1 + maxRows * fontHeight;

  int16_t baseWidth = (leftEllipsis ? stringWidth(STD_ELLIPSIS " ", font, true, true) : 0) +
    (rightEllipsis ? stringWidth(" " STD_ELLIPSIS, font, true, true) : 0);
  int16_t mtxWidth = getRealMatrixColumnWidths(matrix, prefixWidth, font, colWidth, rPadWidth, &digits, maxCols);
  bool_t noFix = (mtxWidth < 0);
  mtxWidth = abs(mtxWidth);
  totalWidth = baseWidth + mtxWidth;
  if(displayFormat == DF_ALL && noFix) {
    displayFormat = getSystemFlag(FLAG_ALLENG) ? DF_ENG : DF_SCI;
    displayFormatDigits = digits;
  }
  if(totalWidth > maxWidth || leftEllipsis) {
    if(font == &numericFont) {
      displayFormat = tmpDisplayFormat;
      displayFormatDigits = tmpDisplayFormatDigits;
      goto smallFont;
    }
    else {
      displayFormat = DF_SCI;
      displayFormatDigits = 3;
      mtxWidth = getRealMatrixColumnWidths(matrix, prefixWidth, font, colWidth, rPadWidth, &digits, maxCols);
      noFix = (mtxWidth < 0);
      mtxWidth = abs(mtxWidth);
      totalWidth = baseWidth + mtxWidth;
      if(totalWidth > maxWidth) {
        maxCols--;
        goto smallFont;
      }
    }
  }
  if(forEditor) {
    if((matSelCol < sCol) && leftEllipsis) {
      scrollColumn--;
      sCol--;
      goto smallFont;
    }
    else if((matSelCol >= sCol + maxCols) && rightEllipsis) {
      scrollColumn++;
      sCol++;
      goto smallFont;
    }
  }
  for(int j = 0; j < maxCols; j++) baseWidth += colWidth[j] + stringWidth(STD_SPACE_FIGURE, font, true, true);
  baseWidth -= stringWidth(STD_SPACE_FIGURE, font, true, true);

  if(!forEditor) X_POS = SCREEN_WIDTH - ((colVector ? stringWidth("[]" STD_SUP_T, font, true, true) : stringWidth("[]", font, true, true)) + baseWidth) - (font == &standardFont ? 0 : 1);

  if(forEditor) {
    clearRegisterLine(REGISTER_X, true, true);
    clearRegisterLine(REGISTER_Y, true, true);
    if(rows >= 2) clearRegisterLine(REGISTER_Z, true, true);
    if(rows >= (font == &standardFont ? 4 : 3)) clearRegisterLine(REGISTER_T, true, true);
  }
  else if(prefixWidth > 0) {
    clearRegisterLine(REGISTER_T, true, true);
    if(rows >= 2) clearRegisterLine(REGISTER_Z, true, true);
    if(rows >= (font == &standardFont ? 4 : 3)) clearRegisterLine(REGISTER_Y, true, true);
    if(rows == 4 && font != &standardFont) clearRegisterLine(REGISTER_X, true, true);
  }

  for(int i = 0; i < maxRows; i++) {
    int16_t colX = stringWidth("[", font, true, true);
    showString((maxRows == 1) ? "[" : (i == 0) ? STD_MAT_TL : (i + 1 == maxRows) ? STD_MAT_BL : STD_MAT_ML, font, X_POS + 1, Y_POS - (maxRows -1 - i) * fontHeight, vmNormal, true, false);
    if(leftEllipsis) {
      showString(STD_ELLIPSIS " ", font, X_POS + stringWidth("[", font, true, true), Y_POS - (maxRows -1 -i) * fontHeight, vmNormal, true, false);
      colX += stringWidth(STD_ELLIPSIS " ", font, true, true);
    }
    for(int j = 0; j < maxCols + (rightEllipsis ? 1 : 0); j++) {
      if(((i == maxRows - 1) && (rows > maxRows + sRow)) || ((j == maxCols) && rightEllipsis) || ((i == 0) && (sRow > 0))) {
        strcpy(tmpString, " " STD_ELLIPSIS);
        vm = vmNormal;
      }
      else {
        real34ToDisplayString(&matrix->matrixElements[(i+sRow)*cols+j+sCol], amNone, tmpString, font, colWidth[j], displayFormat == DF_ALL ? digits : 15, true, STD_SPACE_4_PER_EM, true);
        if(forEditor && matSelRow == (i + sRow) && matSelCol == (j + sCol)) {
          lcd_fill_rect(X_POS + colX, Y_POS - (maxRows -1 -i) * fontHeight, colWidth[j], font == &numericFont ? 32 : 20, 0xFF);
          vm = vmReverse;
        } else {
          vm = vmNormal;
        }
      }
      width = stringWidth(tmpString, font, true, true) + 1;
      showString(tmpString, font, X_POS + colX + (((j == maxCols) && rightEllipsis) ? -stringWidth(" ", font, true, true) : (colWidth[j] - width) - rPadWidth[i * MATRIX_MAX_COLUMNS + j]), Y_POS - (maxRows -1 -i) * fontHeight, vm, true, false);
      colX += colWidth[j] + stringWidth(STD_SPACE_FIGURE, font, true, true);
    }
    showString((maxRows == 1) ? "]" : (i == 0) ? STD_MAT_TR : (i + 1 == maxRows) ? STD_MAT_BR : STD_MAT_MR, font, X_POS + stringWidth("[", font, true, true) + baseWidth, Y_POS - (maxRows -1 -i) * fontHeight, vmNormal, true, false);
    if(colVector == true) {
      showString(STD_SUP_T, font, X_POS + stringWidth("[]", font, true, true) + baseWidth, Y_POS - (maxRows -1 -i) * fontHeight, vmNormal, true, false);
    }
  }

  displayFormat = tmpDisplayFormat;
  displayFormatDigits = tmpDisplayFormatDigits;

}

int16_t getRealMatrixColumnWidths(const real34Matrix_t *matrix, int16_t prefixWidth, const font_t *font, int16_t *colWidth, int16_t *rPadWidth, int16_t *digits, uint16_t maxCols) {
  const bool_t colVector = matrix->header.matrixColumns == 1 && matrix->header.matrixRows > 1;
  const int rows = colVector ? 1 : matrix->header.matrixRows;
  const int cols = colVector ? matrix->header.matrixRows : matrix->header.matrixColumns;
  const int maxRows = rows > MATRIX_MAX_ROWS ? MATRIX_MAX_ROWS : rows;
  const bool_t forEditor = matrix == &openMatrixMIMPointer.realMatrix;
  const uint16_t sRow = forEditor ? scrollRow : 0;
  const uint16_t sCol = forEditor ? scrollColumn : 0;
  const int16_t maxWidth = MATRIX_LINE_WIDTH - prefixWidth;
  int16_t totalWidth = 0, width = 0;
  int16_t maxRightWidth[MATRIX_MAX_COLUMNS] = {};
  int16_t maxLeftWidth[MATRIX_MAX_COLUMNS] = {};
  const int16_t exponentOutOfRange = 0x4000;
  bool_t noFix = false; const int16_t dspDigits = displayFormatDigits;

  begin:
  for(int k = 15; k >= 1; k--) {
    if(displayFormat == DF_ALL) *digits = k;
    if(displayFormat == DF_ALL && noFix) { // something like SCI
      displayFormat = getSystemFlag(FLAG_ALLENG) ? DF_ENG : DF_SCI;
      displayFormatDigits = k;
    }
    for(int i = 0; i < maxRows; i++) {
      for(int j = 0; j < maxCols; j++) {
        real34_t r34Val;
        real34Copy(&matrix->matrixElements[(i+sRow)*cols+j+sCol], &r34Val);
        real34SetPositiveSign(&r34Val);
        real34ToDisplayString(&r34Val, amNone, tmpString, font, maxWidth, displayFormat == DF_ALL ? k : 15, true, STD_SPACE_4_PER_EM, true);
        if(displayFormat == DF_ALL && !noFix && strstr(tmpString, STD_SUB_10)) { // something like SCI
          noFix = true;
          totalWidth = 0;
          for(int p = 0; p < MATRIX_MAX_COLUMNS; ++p)
            maxRightWidth[p] = maxLeftWidth[p] = 0;
          goto begin; // redo
        }
        width = stringWidth(tmpString, font, true, true) + 1;
        rPadWidth[i * MATRIX_MAX_COLUMNS + j] = 0;
        if(strstr(tmpString, ".") || strstr(tmpString, ",")) {
          for(char *xStr = tmpString; *xStr != 0; xStr++) {
            if(((displayFormat != DF_ENG && (displayFormat != DF_ALL || !getSystemFlag(FLAG_ALLENG))) && (*xStr == '.' || *xStr == ',')) ||
               ((displayFormat == DF_ENG || (displayFormat == DF_ALL && getSystemFlag(FLAG_ALLENG))) && xStr[0] == (char)0x80 && (xStr[1] == (char)0x87 || xStr[1] == (char)0xd7))) {
              rPadWidth[i * MATRIX_MAX_COLUMNS + j] = stringWidth(xStr, font, true, true) + 1;
              if(maxRightWidth[j] < rPadWidth[i * MATRIX_MAX_COLUMNS + j]) maxRightWidth[j] = rPadWidth[i * MATRIX_MAX_COLUMNS + j];
              break;
            }
          }
          if(maxLeftWidth[j] < (width - rPadWidth[i * MATRIX_MAX_COLUMNS + j])) maxLeftWidth[j] = (width - rPadWidth[i * MATRIX_MAX_COLUMNS + j]);
        }
        else {
          rPadWidth[i * MATRIX_MAX_COLUMNS + j] = width | exponentOutOfRange;
        }
      }
    }
    for(int i = 0; i < maxRows; i++) {
      for(int j = 0; j < maxCols; j++) {
        if(rPadWidth[i * MATRIX_MAX_COLUMNS + j] & exponentOutOfRange) {
          if((maxLeftWidth[j] + maxRightWidth[j]) < (rPadWidth[i * MATRIX_MAX_COLUMNS + j] & (~exponentOutOfRange))) {
            maxLeftWidth[j] = (rPadWidth[i * MATRIX_MAX_COLUMNS + j] & (~exponentOutOfRange)) - maxRightWidth[j];
          }
        }
      }
    }
    for(int i = 0; i < maxRows; i++) {
      for(int j = 0; j < maxCols; j++) {
        if(rPadWidth[i * MATRIX_MAX_COLUMNS + j] & exponentOutOfRange) {
          rPadWidth[i * MATRIX_MAX_COLUMNS + j] = 0;
        }
        else {
          rPadWidth[i * MATRIX_MAX_COLUMNS + j] -= maxRightWidth[j];
          rPadWidth[i * MATRIX_MAX_COLUMNS + j] *= -1;
        }
      }
    }
    for(int j = 0; j < maxCols; j++) {
      colWidth[j] = (maxLeftWidth[j] + maxRightWidth[j]);
      totalWidth += colWidth[j] + stringWidth(STD_SPACE_FIGURE, font, true, true) * 2;
    }
    totalWidth -= stringWidth(STD_SPACE_FIGURE, font, true, true);
    if(noFix) {
      displayFormat = DF_ALL;
      displayFormatDigits = dspDigits;
    }
    if(displayFormat != DF_ALL) {
      break;
    }
    else if(totalWidth <= maxWidth) {
      *digits = k;
      break;
    }
    else if(k > 1) {
      totalWidth = 0;
      for(int j = 0; j < maxCols; j++) {
        maxRightWidth[j] = 0;
        maxLeftWidth[j] = 0;
      }
    }
  }
  return totalWidth * (noFix ? -1 : 1);
}


void showComplexMatrix(const complex34Matrix_t *matrix, int16_t prefixWidth) {
  int rows = matrix->header.matrixRows;
  int cols = matrix->header.matrixColumns;
  int16_t Y_POS = Y_POSITION_OF_REGISTER_X_LINE;
  int16_t X_POS = 0;
  int16_t totalWidth = 0, width = 0;
  const font_t *font;
  int16_t fontHeight = NUMERIC_FONT_HEIGHT;
  int16_t maxWidth = MATRIX_LINE_WIDTH - prefixWidth;
  int16_t colWidth[MATRIX_MAX_COLUMNS] = {}, colWidth_r[MATRIX_MAX_COLUMNS] = {}, colWidth_i[MATRIX_MAX_COLUMNS] = {};
  int16_t rPadWidth_r[MATRIX_MAX_ROWS * MATRIX_MAX_COLUMNS] = {}, rPadWidth_i[MATRIX_MAX_ROWS * MATRIX_MAX_COLUMNS] = {};
  const bool_t forEditor = matrix == &openMatrixMIMPointer.complexMatrix;
  const uint16_t sRow = forEditor ? scrollRow : 0;
  uint16_t sCol = forEditor ? scrollColumn : 0;
  const uint16_t tmpDisplayFormat = displayFormat;
  const int16_t tmpExponentLimit = exponentLimit;
  const uint8_t tmpDisplayFormatDigits = displayFormatDigits;
  const bool_t tmpMultX = getSystemFlag(FLAG_MULTx);

  Y_POS = Y_POSITION_OF_REGISTER_X_LINE - NUMERIC_FONT_HEIGHT;

  bool_t colVector = false;
  if(cols == 1 && rows > 1) {
    colVector = true;
    cols = rows;
    rows = 1;
  }

  int maxCols = cols > MATRIX_MAX_ROWS ? MATRIX_MAX_ROWS : cols;
  const int maxRows = rows > MATRIX_MAX_COLUMNS ? MATRIX_MAX_COLUMNS : rows;

  int16_t matSelRow = colVector ? getJRegisterAsInt(true) : getIRegisterAsInt(true);
  int16_t matSelCol = colVector ? getIRegisterAsInt(true) : getJRegisterAsInt(true);

  videoMode_t vm = vmNormal;
  if(maxCols + sCol >= cols) maxCols = cols - sCol;

  font = &numericFont;
  if(rows >= (forEditor ? 4 : 5)) {
smallFont:
    font = &standardFont;
    fontHeight = STANDARD_FONT_HEIGHT;
    Y_POS = Y_POSITION_OF_REGISTER_X_LINE - STANDARD_FONT_HEIGHT + 2;
    //maxWidth = MATRIX_LINE_WIDTH_SMALL * 4 - 20;
  }

  if(!forEditor) Y_POS += REGISTER_LINE_HEIGHT;
  bool_t rightEllipsis = (cols > maxCols) && (cols > maxCols + sCol);
  bool_t leftEllipsis = (sCol > 0);
  int16_t digits;

  if(prefixWidth > 0) Y_POS = Y_POSITION_OF_REGISTER_T_LINE - REGISTER_LINE_HEIGHT + 1 + maxRows * fontHeight;

  int16_t baseWidth = (leftEllipsis ? stringWidth(STD_ELLIPSIS " ", font, true, true) : 0) +
    (rightEllipsis ? stringWidth(STD_ELLIPSIS, font, true, true) : 0);
  totalWidth = baseWidth + getComplexMatrixColumnWidths(matrix, prefixWidth, font, colWidth, colWidth_r, colWidth_i, rPadWidth_r, rPadWidth_i, &digits, maxCols);
  if(totalWidth > maxWidth || leftEllipsis) {
    if(font == &numericFont) {
      goto smallFont;
    }
    else if(exponentLimit > 99) {
      exponentLimit = 99;
      goto smallFont;
    }
    else {
      displayFormat = DF_SCI;
      displayFormatDigits = 2;
      clearSystemFlag(FLAG_MULTx);
      totalWidth = baseWidth + getComplexMatrixColumnWidths(matrix, prefixWidth, font, colWidth, colWidth_r, colWidth_i, rPadWidth_r, rPadWidth_i, &digits, maxCols);
      if(totalWidth > maxWidth) {
        maxCols--;
        goto smallFont;
      }
    }
  }
  if(forEditor) {
    if(matSelCol < sCol) {
      scrollColumn--;
      sCol--;
      goto smallFont;
    }
    else if(matSelCol >= sCol + maxCols) {
      scrollColumn++;
      sCol++;
      goto smallFont;
    }
  }
  for(int j = 0; j < maxCols; j++) baseWidth += colWidth[j] + stringWidth(STD_SPACE_FIGURE, font, true, true);
  baseWidth -= stringWidth(STD_SPACE_FIGURE, font, true, true);

  if(!forEditor) X_POS = SCREEN_WIDTH - ((colVector ? stringWidth("[]" STD_SUP_T, font, true, true) : stringWidth("[]", font, true, true)) + baseWidth) - (font == &standardFont ? 0 : 1);

  if(forEditor) {
    clearRegisterLine(REGISTER_X, true, true);
    clearRegisterLine(REGISTER_Y, true, true);
    if(rows >= 2) clearRegisterLine(REGISTER_Z, true, true);
    if(rows >= 3) clearRegisterLine(REGISTER_T, true, true);
  }
  else if(prefixWidth > 0) {
    clearRegisterLine(REGISTER_T, true, true);
    if(rows >= 2) clearRegisterLine(REGISTER_Z, true, true);
    if(rows >= (font == &standardFont ? 4 : 3)) clearRegisterLine(REGISTER_Y, true, true);
    if(rows == 4 && font != &standardFont) clearRegisterLine(REGISTER_X, true, true);
  }

  for(int i = 0; i < maxRows; i++) {
    int16_t colX = stringWidth("[", font, true, true);
    showString((maxRows == 1) ? "[" : (i == 0) ? STD_MAT_TL : (i + 1 == maxRows) ? STD_MAT_BL : STD_MAT_ML, font, X_POS + 1, Y_POS - (maxRows -1 - i) * fontHeight, vmNormal, true, false);
    if(leftEllipsis) {
      showString(STD_ELLIPSIS " ", font, X_POS + stringWidth("[", font, true, true), Y_POS - (maxRows -1 -i) * fontHeight, vmNormal, true, false);
      colX += stringWidth(STD_ELLIPSIS " ", font, true, true);
    }
    for(int j = 0; j < maxCols + (rightEllipsis ? 1 : 0); j++) {
      real34_t re, im;
      if(getSystemFlag(FLAG_POLAR)) { // polar mode
        real_t x, y;
        real34ToReal(VARIABLE_REAL34_DATA(&matrix->matrixElements[(i+sRow)*cols+j+sCol]), &x);
        real34ToReal(VARIABLE_IMAG34_DATA(&matrix->matrixElements[(i+sRow)*cols+j+sCol]), &y);
        realRectangularToPolar(&x, &y, &x, &y, &ctxtReal39);
        convertAngleFromTo(&y, amRadian, currentAngularMode, &ctxtReal39);
        realToReal34(&x, &re);
        realToReal34(&y, &im);
      }
      else { // rectangular mode
        real34Copy(VARIABLE_REAL34_DATA(&matrix->matrixElements[(i+sRow)*cols+j+sCol]), &re);
        real34Copy(VARIABLE_IMAG34_DATA(&matrix->matrixElements[(i+sRow)*cols+j+sCol]), &im);
      }

      if(((i == maxRows - 1) && (rows > maxRows + sRow)) || ((j == maxCols) && rightEllipsis) || ((i == 0) && (sRow > 0))) {
        strcpy(tmpString, STD_ELLIPSIS);
        vm = vmNormal;
      }
      else {
        tmpString[0] = 0;
        real34ToDisplayString(&re, amNone, tmpString, font, colWidth_r[j], displayFormat == DF_ALL ? digits : 15, true, STD_SPACE_4_PER_EM, true);
        if(forEditor && matSelRow == (i + sRow) && matSelCol == (j + sCol)) {
          lcd_fill_rect(X_POS + colX, Y_POS - (maxRows -1 -i) * fontHeight, colWidth[j], font == &numericFont ? 32 : 20, 0xFF);
          vm = vmReverse;
        } else {
          vm = vmNormal;
        }
      }
      width = stringWidth(tmpString, font, true, true) + 1;
      showString(tmpString, font, X_POS + colX + (((j == maxCols) && rightEllipsis) ? stringWidth(STD_SPACE_FIGURE, font, true, true) - width : (colWidth_r[j] - width) - rPadWidth_r[i * MATRIX_MAX_COLUMNS + j]), Y_POS - (maxRows -1 -i) * fontHeight, vm, true, false);
      if(strcmp(tmpString, STD_ELLIPSIS) != 0) {
        bool_t neg = real34IsNegative(&im);
        int16_t cpxUnitWidth;

        if(getSystemFlag(FLAG_POLAR)) { // polar mode
          strcpy(tmpString, STD_SPACE_4_PER_EM STD_MEASURED_ANGLE STD_SPACE_4_PER_EM);
        }
        else { // rectangular mode
          strcpy(tmpString, "+");
          strcat(tmpString, COMPLEX_UNIT);
          strcat(tmpString, PRODUCT_SIGN);
        }
        cpxUnitWidth = width = stringWidth(tmpString, font, true, true);
        if(!getSystemFlag(FLAG_POLAR)) {
          if(neg) {
            tmpString[0] = '-';
            real34SetPositiveSign(&im);
          }
        }
        showString(tmpString, font, X_POS + colX + colWidth_r[j] + (width - stringWidth(tmpString, font, true, true)), Y_POS - (maxRows -1 -i) * fontHeight, vm, true, false);

        real34ToDisplayString(&im, getSystemFlag(FLAG_POLAR) ? currentAngularMode : amNone, tmpString, font, colWidth_i[j], displayFormat == DF_ALL ? digits : 15, true, STD_SPACE_4_PER_EM, false);
        width = stringWidth(tmpString, font, true, true) + 1;
        showString(tmpString, font, X_POS + colX + colWidth_r[j] + cpxUnitWidth + (((j == maxCols - 1) && rightEllipsis) ? 0 : (colWidth_i[j] - width) - rPadWidth_i[i * MATRIX_MAX_COLUMNS + j]), Y_POS - (maxRows -1 -i) * fontHeight, vm, true, false);
      }
      colX += colWidth[j] + stringWidth(STD_SPACE_FIGURE, font, true, true);
    }
    showString((maxRows == 1) ? "]" : (i == 0) ? STD_MAT_TR : (i + 1 == maxRows) ? STD_MAT_BR : STD_MAT_MR, font, X_POS + stringWidth("[", font, true, true) + baseWidth, Y_POS - (maxRows -1 -i) * fontHeight, vmNormal, true, false);
    if(colVector == true) {
      showString(STD_SUP_T, font, X_POS + stringWidth("[]", font, true, true) + baseWidth, Y_POS - (maxRows -1 -i) * fontHeight, vmNormal, true, false);
    }
  }

  displayFormat = tmpDisplayFormat;
  displayFormatDigits = tmpDisplayFormatDigits;
  exponentLimit = tmpExponentLimit;
  if(tmpMultX) setSystemFlag(FLAG_MULTx);

}

int16_t getComplexMatrixColumnWidths(const complex34Matrix_t *matrix, int16_t prefixWidth, const font_t *font, int16_t *colWidth, int16_t *colWidth_r, int16_t *colWidth_i, int16_t *rPadWidth_r, int16_t *rPadWidth_i, int16_t *digits, uint16_t maxCols) {
  const bool_t colVector = matrix->header.matrixColumns == 1 && matrix->header.matrixRows > 1;
  const int rows = colVector ? 1 : matrix->header.matrixRows;
  const int cols = colVector ? matrix->header.matrixRows : matrix->header.matrixColumns;
  const int maxRows = rows > MATRIX_MAX_ROWS ? MATRIX_MAX_ROWS : rows;
  const bool_t forEditor = matrix == &openMatrixMIMPointer.complexMatrix;
  const uint16_t sRow = forEditor ? scrollRow : 0;
  const uint16_t sCol = forEditor ? scrollColumn : 0;
  const int16_t maxWidth = MATRIX_LINE_WIDTH - prefixWidth;
  int16_t totalWidth = 0, width = 0;
  int16_t maxRightWidth_r[MATRIX_MAX_COLUMNS] = {};
  int16_t maxLeftWidth_r[MATRIX_MAX_COLUMNS] = {};
  int16_t maxRightWidth_i[MATRIX_MAX_COLUMNS] = {};
  int16_t maxLeftWidth_i[MATRIX_MAX_COLUMNS] = {};
  const int16_t exponentOutOfRange = 0x4000;

  uint16_t cpxUnitWidth;
  if(getSystemFlag(FLAG_POLAR)) { // polar mode
    strcpy(tmpString, STD_SPACE_4_PER_EM STD_MEASURED_ANGLE STD_SPACE_4_PER_EM);
  }
  else { // rectangular mode
    strcpy(tmpString, "+");
    strcat(tmpString, COMPLEX_UNIT);
    strcat(tmpString, PRODUCT_SIGN);
  }
  cpxUnitWidth = stringWidth(tmpString, font, true, true);

  for(int k = 15; k >= 1; k--) {
    if(displayFormat == DF_ALL) *digits = k;
    for(int i = 0; i < maxRows; i++) {
      for(int j = 0; j < maxCols; j++) {
        complex34_t c34Val;
        complex34Copy(&matrix->matrixElements[(i+sRow)*cols+j+sCol], &c34Val);
        if(getSystemFlag(FLAG_POLAR)) { // polar mode
          real_t x, y;
          real34ToReal(VARIABLE_REAL34_DATA(&c34Val), &x);
          real34ToReal(VARIABLE_IMAG34_DATA(&c34Val), &y);
          realRectangularToPolar(&x, &y, &x, &y, &ctxtReal39);
          convertAngleFromTo(&y, amRadian, currentAngularMode, &ctxtReal39);
          realToReal34(&x, VARIABLE_REAL34_DATA(&c34Val));
          realToReal34(&y, VARIABLE_IMAG34_DATA(&c34Val));
        }

        rPadWidth_r[i * MATRIX_MAX_COLUMNS + j] = 0;
        real34SetPositiveSign(VARIABLE_REAL34_DATA(&c34Val));
        real34ToDisplayString(VARIABLE_REAL34_DATA(&c34Val), amNone, tmpString, font, maxWidth, displayFormat == DF_ALL ? k : 15, true, STD_SPACE_4_PER_EM, true);
        width = stringWidth(tmpString, font, true, true) + 1;
        if(strstr(tmpString, ".") || strstr(tmpString, ",")) {
          for(char *xStr = tmpString; *xStr != 0; xStr++) {
            if(((displayFormat != DF_ENG && (displayFormat != DF_ALL || !getSystemFlag(FLAG_ALLENG))) && (*xStr == '.' || *xStr == ',')) ||
               ((displayFormat == DF_ENG || (displayFormat == DF_ALL && getSystemFlag(FLAG_ALLENG))) && xStr[0] == (char)0x80 && (xStr[1] == (char)0x87 || xStr[1] == (char)0xd7))) {
              rPadWidth_r[i * MATRIX_MAX_COLUMNS + j] = stringWidth(xStr, font, true, true) + 1;
              if(maxRightWidth_r[j] < rPadWidth_r[i * MATRIX_MAX_COLUMNS + j]) maxRightWidth_r[j] = rPadWidth_r[i * MATRIX_MAX_COLUMNS + j];
              break;
            }
          }
          if(maxLeftWidth_r[j] < (width - rPadWidth_r[i * MATRIX_MAX_COLUMNS + j])) maxLeftWidth_r[j] = (width - rPadWidth_r[i * MATRIX_MAX_COLUMNS + j]);
        }
        else {
          rPadWidth_r[i * MATRIX_MAX_COLUMNS + j] = width | exponentOutOfRange;
        }

        rPadWidth_i[i * MATRIX_MAX_COLUMNS + j] = 0;
        if(!getSystemFlag(FLAG_POLAR))real34SetPositiveSign(VARIABLE_IMAG34_DATA(&c34Val));
        real34ToDisplayString(VARIABLE_IMAG34_DATA(&c34Val), getSystemFlag(FLAG_POLAR) ? currentAngularMode : amNone, tmpString, font, maxWidth, displayFormat == DF_ALL ? k : 15, true, STD_SPACE_4_PER_EM, false);
        width = stringWidth(tmpString, font, true, true) + 1;
        if(strstr(tmpString, ".") || strstr(tmpString, ",")) {
          for(char *xStr = tmpString; *xStr != 0; xStr++) {
            if(((displayFormat != DF_ENG && (displayFormat != DF_ALL || !getSystemFlag(FLAG_ALLENG))) && (*xStr == '.' || *xStr == ',')) ||
               ((displayFormat == DF_ENG || (displayFormat == DF_ALL && getSystemFlag(FLAG_ALLENG))) && xStr[0] == (char)0x80 && (xStr[1] == (char)0x87 || xStr[1] == (char)0xd7))) {
              rPadWidth_i[i * MATRIX_MAX_COLUMNS + j] = stringWidth(xStr, font, true, true) + 1;
              if(maxRightWidth_i[j] < rPadWidth_i[i * MATRIX_MAX_COLUMNS + j]) maxRightWidth_i[j] = rPadWidth_i[i * MATRIX_MAX_COLUMNS + j];
              break;
            }
          }
          if(maxLeftWidth_i[j] < (width - rPadWidth_i[i * MATRIX_MAX_COLUMNS + j])) maxLeftWidth_i[j] = (width - rPadWidth_i[i * MATRIX_MAX_COLUMNS + j]);
        }
        else {
          rPadWidth_i[i * MATRIX_MAX_COLUMNS + j] = width | exponentOutOfRange;
        }
      }
    }
    for(int i = 0; i < maxRows; i++) {
      for(int j = 0; j < maxCols; j++) {
        if(rPadWidth_r[i * MATRIX_MAX_COLUMNS + j] & exponentOutOfRange) {
          if((maxLeftWidth_r[j] + maxRightWidth_r[j]) < (rPadWidth_r[i * MATRIX_MAX_COLUMNS + j] & (~exponentOutOfRange))) {
            maxLeftWidth_r[j] = (rPadWidth_r[i * MATRIX_MAX_COLUMNS + j] & (~exponentOutOfRange)) - maxRightWidth_r[j];
          }
        }
        if(rPadWidth_i[i * MATRIX_MAX_COLUMNS + j] & exponentOutOfRange) {
          if((maxLeftWidth_i[j] + maxRightWidth_i[j]) < (rPadWidth_i[i * MATRIX_MAX_COLUMNS + j] & (~exponentOutOfRange))) {
            maxLeftWidth_i[j] = (rPadWidth_i[i * MATRIX_MAX_COLUMNS + j] & (~exponentOutOfRange)) - maxRightWidth_i[j];
          }
        }
      }
    }
    for(int i = 0; i < maxRows; i++) {
      for(int j = 0; j < maxCols; j++) {
        if(rPadWidth_r[i * MATRIX_MAX_COLUMNS + j] & exponentOutOfRange) {
          rPadWidth_r[i * MATRIX_MAX_COLUMNS + j] = 0;
        }
        else {
          rPadWidth_r[i * MATRIX_MAX_COLUMNS + j] -= maxRightWidth_r[j];
          rPadWidth_r[i * MATRIX_MAX_COLUMNS + j] *= -1;
        }
        if(rPadWidth_i[i * MATRIX_MAX_COLUMNS + j] & exponentOutOfRange) {
          rPadWidth_i[i * MATRIX_MAX_COLUMNS + j] = 0;
        }
        else {
          rPadWidth_i[i * MATRIX_MAX_COLUMNS + j] -= maxRightWidth_i[j];
          rPadWidth_i[i * MATRIX_MAX_COLUMNS + j] *= -1;
        }
      }
    }
    for(int j = 0; j < maxCols; j++) {
      colWidth_r[j] = maxLeftWidth_r[j] + maxRightWidth_r[j];
      colWidth_i[j] = maxLeftWidth_i[j] + maxRightWidth_i[j];
      colWidth[j] = colWidth_r[j] + (colWidth_i[j] > 0 ? (cpxUnitWidth + colWidth_i[j]) : 0);
      totalWidth += colWidth[j] + stringWidth(STD_SPACE_FIGURE, font, true, true) * 2;
    }
    totalWidth -= stringWidth(STD_SPACE_FIGURE, font, true, true);
    if(displayFormat != DF_ALL) {
      break;
    }
    else if(totalWidth <= maxWidth) {
      *digits = k;
      break;
    }
    else if(k > 1) {
      totalWidth = 0;
      for(int j = 0; j < maxCols; j++) {
        maxRightWidth_r[j] = 0;
        maxLeftWidth_r[j] = 0;
        maxRightWidth_i[j] = 0;
        maxLeftWidth_i[j] = 0;
      }
    }
  }
  return totalWidth;
}


/*
void storeMatrixToXRegister(real34Matrix_t *matrix) {

  setSystemFlag(FLAG_ASLIFT);
  liftStack();
  clearSystemFlag(FLAG_ASLIFT);

  //WORKING//
  //openMatrixMIMPointer = matrix;
  // END WORKING//

  convertReal34MatrixToReal34MatrixRegister(matrix, REGISTER_X);
}
*/

void getMatrixFromRegister(calcRegister_t regist) {
  if(getRegisterDataType(regist) == dtReal34Matrix) {
    real34Matrix_t matrix;

    if(openMatrixMIMPointer.realMatrix.matrixElements) realMatrixFree(&openMatrixMIMPointer.realMatrix);
    convertReal34MatrixRegisterToReal34Matrix(regist, &matrix);

    openMatrixMIMPointer.realMatrix = matrix;
  }
  else if(getRegisterDataType(regist) == dtComplex34Matrix) {
    complex34Matrix_t matrix;

    if(openMatrixMIMPointer.complexMatrix.matrixElements) complexMatrixFree(&openMatrixMIMPointer.complexMatrix);
    convertComplex34MatrixRegisterToComplex34Matrix(regist, &matrix);

    openMatrixMIMPointer.complexMatrix = matrix;
  }
  else {
    #ifdef PC_BUILD
    sprintf(errorMessage, "DataType %" PRIu32, getRegisterDataType(regist));
    moreInfoOnError("In function getMatrixFromRegister:", errorMessage, "is not dataType dtRealMatrix.", "");
    #endif
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    return;
  }

}



bool_t initMatrixRegister(calcRegister_t regist, uint16_t rows, uint16_t cols, bool_t complex) {
  const size_t neededSize = (rows * cols) * (complex ? COMPLEX34_SIZE : REAL34_SIZE);
  reallocateRegister(regist, complex ? dtComplex34Matrix : dtReal34Matrix, neededSize, amNone);
  if(regist == INVALID_VARIABLE) {
    return false;
  }
  else if(lastErrorCode == ERROR_NONE) {
    // REGISTER_COMPLEX34_MATRIX_DBLOCK is same as REGISTER_REAL34_MATRIX_DBLOCK
    REGISTER_REAL34_MATRIX_DBLOCK(regist)->matrixRows    = rows;
    REGISTER_REAL34_MATRIX_DBLOCK(regist)->matrixColumns = cols;
    if(complex) {
      for(uint16_t i = 0; i < rows * cols; ++i) {
        real34Zero(VARIABLE_REAL34_DATA(REGISTER_COMPLEX34_MATRIX_M_ELEMENTS(regist) + i));
        real34Zero(VARIABLE_REAL34_DATA(REGISTER_COMPLEX34_MATRIX_M_ELEMENTS(regist) + i));
      }
    }
    else {
      for(uint16_t i = 0; i < rows * cols; ++i) {
        real34Zero(REGISTER_REAL34_MATRIX_M_ELEMENTS(regist) + i);
      }
    }
    return true;
  }
  else {
    return false;
  }
}

bool_t redimMatrixRegister(calcRegister_t regist, uint16_t rows, uint16_t cols) {
  const uint16_t origRows = REGISTER_REAL34_MATRIX_DBLOCK(regist)->matrixRows, origCols = REGISTER_REAL34_MATRIX_DBLOCK(regist)->matrixColumns;
  if(regist == INVALID_VARIABLE) {
    return false;
  }
  else if(getRegisterDataType(regist) == dtReal34Matrix) {
    const size_t oldSize = (origRows * origCols) * REAL34_SIZE;
    const size_t newSize = (rows     * cols    ) * REAL34_SIZE;
    if(oldSize >= newSize) {
      if(oldSize > newSize)
        freeWp43s(REGISTER_REAL34_MATRIX_M_ELEMENTS(regist) + rows * cols, oldSize - newSize);
      REGISTER_REAL34_MATRIX_DBLOCK(regist)->matrixRows    = rows;
      REGISTER_REAL34_MATRIX_DBLOCK(regist)->matrixColumns = cols;
      return true;
    }
    else {
      real34Matrix_t newMatrix;
      convertReal34MatrixRegisterToReal34Matrix(regist, &newMatrix);
      if(lastErrorCode == ERROR_NONE) {
        reallocateRegister(regist, dtReal34Matrix, newSize, amNone);
        if(lastErrorCode == ERROR_NONE) {
          for(uint16_t i = 0; i < origRows * origCols; ++i) {
            real34Copy(newMatrix.matrixElements + i, REGISTER_REAL34_MATRIX_M_ELEMENTS(regist) + i);
          }
          for(uint16_t i = origRows * origCols; i < rows * cols; ++i) {
            real34Zero(REGISTER_REAL34_MATRIX_M_ELEMENTS(regist) + i);
          }
          realMatrixFree(&newMatrix);
          return true;
        }
        else {
          realMatrixFree(&newMatrix);
          return false;
        }
      }
      else return false;
    }
  }
  else if(getRegisterDataType(regist) == dtComplex34Matrix) {
    const size_t oldSize = (origRows * origCols) * COMPLEX34_SIZE;
    const size_t newSize = (rows     * cols    ) * COMPLEX34_SIZE;
    if(oldSize >= newSize) {
      if(oldSize > newSize)
        freeWp43s(REGISTER_COMPLEX34_MATRIX_M_ELEMENTS(regist) + rows * cols, oldSize - newSize);
      REGISTER_COMPLEX34_MATRIX_DBLOCK(regist)->matrixRows    = rows;
      REGISTER_COMPLEX34_MATRIX_DBLOCK(regist)->matrixColumns = cols;
      return true;
    }
    else {
      complex34Matrix_t newMatrix;
      convertComplex34MatrixRegisterToComplex34Matrix(regist, &newMatrix);
      if(lastErrorCode == ERROR_NONE) {
        reallocateRegister(regist, dtComplex34Matrix, newSize, amNone);
        if(lastErrorCode == ERROR_NONE) {
          for(uint16_t i = 0; i < origRows * origCols; ++i) {
            complex34Copy(newMatrix.matrixElements + i, REGISTER_COMPLEX34_MATRIX_M_ELEMENTS(regist) + i);
          }
          for(uint16_t i = origRows * origCols; i < rows * cols; ++i) {
            real34Zero(VARIABLE_REAL34_DATA(REGISTER_REAL34_MATRIX_M_ELEMENTS(regist) + i));
            real34Zero(VARIABLE_IMAG34_DATA(REGISTER_REAL34_MATRIX_M_ELEMENTS(regist) + i));
          }
          complexMatrixFree(&newMatrix);
          return true;
        }
        else {
          complexMatrixFree(&newMatrix);
          return false;
        }
      }
      else return false;
    }
  }
  else return initMatrixRegister(regist, rows, cols, false);
}

calcRegister_t allocateNamedMatrix(const char *name, uint16_t rows, uint16_t cols) {
  const calcRegister_t regist = findOrAllocateNamedVariable(name);
  if(regist == INVALID_VARIABLE) {
    return INVALID_VARIABLE;
  }
  else if(redimMatrixRegister(regist, rows, cols)) {
    return regist;
  }
  else {
    return INVALID_VARIABLE;
  }
}

bool_t appendRowAtMatrixRegister(calcRegister_t regist) {
  const uint16_t rows = REGISTER_REAL34_MATRIX_DBLOCK(regist)->matrixRows, cols = REGISTER_REAL34_MATRIX_DBLOCK(regist)->matrixColumns;
  if(regist == INVALID_VARIABLE) {
    return false;
  }
  else if(getRegisterDataType(regist) == dtReal34Matrix || getRegisterDataType(regist) == dtComplex34Matrix) {
    return redimMatrixRegister(regist, rows + 1, cols);
  }
  else return false;
}

//Row of Matrix
int16_t getIRegisterAsInt(bool_t asArrayPointer) {
  int16_t ret;
  longInteger_t tmp_lgInt;

  if(getRegisterDataType(REGISTER_I) == dtLongInteger)
    convertLongIntegerRegisterToLongInteger(REGISTER_I, tmp_lgInt);
  else if(getRegisterDataType(REGISTER_I) == dtReal34)
    convertReal34ToLongInteger(REGISTER_REAL34_DATA(REGISTER_I), tmp_lgInt, DEC_ROUND_DOWN);
  else
    longIntegerInit(tmp_lgInt);
  longIntegerToInt(tmp_lgInt, ret);

  longIntegerFree(tmp_lgInt);

  if (asArrayPointer) ret--;

  return ret;
}

//Col of Matrix
int16_t getJRegisterAsInt(bool_t asArrayPointer) {
  int16_t ret;
  longInteger_t tmp_lgInt;

  if(getRegisterDataType(REGISTER_J) == dtLongInteger)
    convertLongIntegerRegisterToLongInteger(REGISTER_J, tmp_lgInt);
  else if(getRegisterDataType(REGISTER_J) == dtReal34)
    convertReal34ToLongInteger(REGISTER_REAL34_DATA(REGISTER_J), tmp_lgInt, DEC_ROUND_DOWN);
  else
    longIntegerInit(tmp_lgInt);
  longIntegerToInt(tmp_lgInt, ret);

  longIntegerFree(tmp_lgInt);

  if (asArrayPointer) ret--;

  return ret;
}

//Row of Matrix
void setIRegisterAsInt(bool_t asArrayPointer, int16_t toStore) {
  if (asArrayPointer) toStore++;
  longInteger_t tmp_lgInt;
  longIntegerInit(tmp_lgInt);

  intToLongInteger(toStore, tmp_lgInt);
  convertLongIntegerToLongIntegerRegister(tmp_lgInt, REGISTER_I);

  longIntegerFree(tmp_lgInt);
}

//ColOfMatrix
void setJRegisterAsInt(bool_t asArrayPointer, int16_t toStore) {
  if (asArrayPointer) toStore++;
  longInteger_t tmp_lgInt;
  longIntegerInit(tmp_lgInt);

  intToLongInteger(toStore, tmp_lgInt);
  convertLongIntegerToLongIntegerRegister(tmp_lgInt, REGISTER_J);

  longIntegerFree(tmp_lgInt);
}

bool_t wrapIJ(uint16_t rows, uint16_t cols) {
  if(getIRegisterAsInt(true) < 0) {
    setIRegisterAsInt(true, rows - 1);
    setJRegisterAsInt(true, (getJRegisterAsInt(true) == 0) ? cols - 1 : getJRegisterAsInt(true) - 1);
  } else if(getIRegisterAsInt(true) == rows) {
    setIRegisterAsInt(true, 0);
    setJRegisterAsInt(true, (getJRegisterAsInt(true) == cols - 1) ? 0 : getJRegisterAsInt(true) + 1);
  }

  if(getJRegisterAsInt(true) < 0) {
    setJRegisterAsInt(true, cols - 1);
    setIRegisterAsInt(true, (getIRegisterAsInt(true) == 0) ? rows - 1 : getIRegisterAsInt(true) - 1);
  } else if(getJRegisterAsInt(true) == cols) {
    setJRegisterAsInt(true, 0);
    setIRegisterAsInt(true, ((!getSystemFlag(FLAG_GROW)) && (getIRegisterAsInt(true) == rows - 1)) ? 0 : getIRegisterAsInt(true) + 1);
  }

  return getIRegisterAsInt(true) == rows;
}



/* Duplicate */
void copyRealMatrix(const real34Matrix_t *matrix, real34Matrix_t *res) {
  const uint16_t rows = matrix->header.matrixRows;
  const uint16_t cols = matrix->header.matrixColumns;
  int32_t i;

  if(realMatrixInit(res, rows, cols)) {
    for(i = 0; i < cols * rows; ++i) {
      real34Copy(&matrix->matrixElements[i], &res->matrixElements[i]);
    }
  }
  else lastErrorCode = ERROR_RAM_FULL;
}

void copyComplexMatrix(const complex34Matrix_t *matrix, complex34Matrix_t *res) {
  const uint16_t rows = matrix->header.matrixRows;
  const uint16_t cols = matrix->header.matrixColumns;
  int32_t i;

  if(complexMatrixInit(res, rows, cols)) {
    for(i = 0; i < cols * rows; ++i) {
      complex34Copy(&matrix->matrixElements[i], &res->matrixElements[i]);
    }
  }
  else lastErrorCode = ERROR_RAM_FULL;
}



/* Link to real matrix register (data not copied) */
void linkToRealMatrixRegister(calcRegister_t regist, real34Matrix_t *linkedMatrix) {
  linkedMatrix->header.matrixRows    = REGISTER_REAL34_MATRIX_DBLOCK(regist)->matrixRows;
  linkedMatrix->header.matrixColumns = REGISTER_REAL34_MATRIX_DBLOCK(regist)->matrixColumns;
  linkedMatrix->matrixElements       = REGISTER_REAL34_MATRIX_M_ELEMENTS(regist);
}

/* Link to complex matrix register (data not copied) */
void linkToComplexMatrixRegister(calcRegister_t regist, complex34Matrix_t *linkedMatrix) {
  linkedMatrix->header.matrixRows    = REGISTER_COMPLEX34_MATRIX_DBLOCK(regist)->matrixRows;
  linkedMatrix->header.matrixColumns = REGISTER_COMPLEX34_MATRIX_DBLOCK(regist)->matrixColumns;
  linkedMatrix->matrixElements       = REGISTER_COMPLEX34_MATRIX_M_ELEMENTS(regist);
}



/* Insert a row */
void insRowRealMatrix(real34Matrix_t *matrix, uint16_t beforeRowNo) {
  const uint16_t rows = matrix->header.matrixRows;
  const uint16_t cols = matrix->header.matrixColumns;
  int32_t i;
  real34Matrix_t newMat;

  if(realMatrixInit(&newMat, rows + 1, cols)) {
    for(i = 0; i < beforeRowNo * cols; ++i) {
      real34Copy(&matrix->matrixElements[i], &newMat.matrixElements[i]);
    }
    for(i = 0; i < cols; ++i) {
      real34Copy(const34_0, &newMat.matrixElements[beforeRowNo * cols + i]);
    }
    for(i = beforeRowNo * cols; i < cols * rows; ++i) {
      real34Copy(&matrix->matrixElements[i], &newMat.matrixElements[i + cols]);
    }

    realMatrixFree(matrix);
    matrix->header.matrixRows    = newMat.header.matrixRows;
    matrix->header.matrixColumns = newMat.header.matrixColumns;
    matrix->matrixElements       = newMat.matrixElements;
  }
  else lastErrorCode = ERROR_RAM_FULL;
}

void insRowComplexMatrix(complex34Matrix_t *matrix, uint16_t beforeRowNo) {
  const uint16_t rows = matrix->header.matrixRows;
  const uint16_t cols = matrix->header.matrixColumns;
  int32_t i;
  complex34Matrix_t newMat;

  if(complexMatrixInit(&newMat, rows + 1, cols)) {
    for(i = 0; i < beforeRowNo * cols; ++i) {
      complex34Copy(&matrix->matrixElements[i], &newMat.matrixElements[i]);
    }
    for(i = 0; i < cols; ++i) {
      real34Copy(const34_0, VARIABLE_REAL34_DATA(&newMat.matrixElements[beforeRowNo * cols + i]));
      real34Copy(const34_0, VARIABLE_IMAG34_DATA(&newMat.matrixElements[beforeRowNo * cols + i]));
    }
    for(i = beforeRowNo * cols; i < cols * rows; ++i) {
      complex34Copy(&matrix->matrixElements[i], &newMat.matrixElements[i + cols]);
    }

    complexMatrixFree(matrix);
    matrix->header.matrixRows    = newMat.header.matrixRows;
    matrix->header.matrixColumns = newMat.header.matrixColumns;
    matrix->matrixElements       = newMat.matrixElements;
  }
  else lastErrorCode = ERROR_RAM_FULL;
}



/* Delete a row */
void delRowRealMatrix(real34Matrix_t *matrix, uint16_t beforeRowNo) {
  const uint16_t rows = matrix->header.matrixRows;
  const uint16_t cols = matrix->header.matrixColumns;
  int32_t i;
  real34Matrix_t newMat;

  if(realMatrixInit(&newMat, rows - 1, cols)) {
    for(i = 0; i < beforeRowNo * cols; ++i) {
      real34Copy(&matrix->matrixElements[i], &newMat.matrixElements[i]);
    }
    for(i = (beforeRowNo + 1) * cols; i < cols * rows; ++i) {
      real34Copy(&matrix->matrixElements[i], &newMat.matrixElements[i - cols]);
    }

    realMatrixFree(matrix);
    matrix->header.matrixRows    = newMat.header.matrixRows;
    matrix->header.matrixColumns = newMat.header.matrixColumns;
    matrix->matrixElements       = newMat.matrixElements;
  }
  else lastErrorCode = ERROR_RAM_FULL;
}

void delRowComplexMatrix(complex34Matrix_t *matrix, uint16_t beforeRowNo) {
  const uint16_t rows = matrix->header.matrixRows;
  const uint16_t cols = matrix->header.matrixColumns;
  int32_t i;
  complex34Matrix_t newMat;

  if(complexMatrixInit(&newMat, rows - 1, cols)) {
    for(i = 0; i < beforeRowNo * cols; ++i) {
      complex34Copy(&matrix->matrixElements[i], &newMat.matrixElements[i]);
    }
    for(i = (beforeRowNo + 1) * cols; i < cols * rows; ++i) {
      complex34Copy(&matrix->matrixElements[i], &newMat.matrixElements[i - cols]);
    }

    complexMatrixFree(matrix);
    matrix->header.matrixRows    = newMat.header.matrixRows;
    matrix->header.matrixColumns = newMat.header.matrixColumns;
    matrix->matrixElements       = newMat.matrixElements;
  }
  else lastErrorCode = ERROR_RAM_FULL;
}


/* Transpose */
void transposeRealMatrix(const real34Matrix_t *matrix, real34Matrix_t *res) {
  const uint16_t rows = matrix->header.matrixRows;
  const uint16_t cols = matrix->header.matrixColumns;
  int32_t i, j;

  if(matrix != res) {
    if(realMatrixInit(res, cols, rows)) {
      for(i = 0; i < rows; ++i) {
        for(j = 0; j < cols; ++j) {
          real34Copy(&matrix->matrixElements[i * cols + j], &res->matrixElements[j * rows + i]);
        }
      }
    }
    else lastErrorCode = ERROR_RAM_FULL;
  }
  else {
    real34Matrix_t tmp;
    copyRealMatrix(matrix, &tmp);
    if(tmp.matrixElements) {
      for(i = 0; i < rows; ++i) {
        for(j = 0; j < cols; ++j) {
          real34Copy(&tmp.matrixElements[i * cols + j], &res->matrixElements[j * rows + i]);
        }
      }
      realMatrixFree(&tmp);
      res->header.matrixRows    = cols;
      res->header.matrixColumns = rows;
    }
    else lastErrorCode = ERROR_RAM_FULL;
  }
}

void transposeComplexMatrix(const complex34Matrix_t *matrix, complex34Matrix_t *res) {
  const uint16_t rows = matrix->header.matrixRows;
  const uint16_t cols = matrix->header.matrixColumns;
  int32_t i, j;

  if(matrix != res) {
    if(complexMatrixInit(res, cols, rows)) {
      for(i = 0; i < rows; ++i) {
        for(j = 0; j < cols; ++j) {
          complex34Copy(&matrix->matrixElements[i * cols + j], &res->matrixElements[j * rows + i]);
        }
      }
    }
    else lastErrorCode = ERROR_RAM_FULL;
  }
  else {
    complex34Matrix_t tmp;
    copyComplexMatrix(matrix, &tmp);
    if(tmp.matrixElements) {
      for(i = 0; i < rows; ++i) {
        for(j = 0; j < cols; ++j) {
          complex34Copy(&tmp.matrixElements[i * cols + j], &res->matrixElements[j * rows + i]);
        }
      }
      complexMatrixFree(&tmp);
      res->header.matrixRows    = cols;
      res->header.matrixColumns = rows;
    }
    else lastErrorCode = ERROR_RAM_FULL;
  }
}



/* Addition and subtraction */
static void addSubRealMatrices(const real34Matrix_t *y, const real34Matrix_t *x, bool_t subtraction, real34Matrix_t *res) {
  const uint16_t rows = y->header.matrixRows;
  const uint16_t cols = y->header.matrixColumns;
  int32_t i;

  if((y->header.matrixColumns != x->header.matrixColumns) || (y->header.matrixRows != x->header.matrixRows)) {
    res->matrixElements = NULL; // Matrix mismatch
    res->header.matrixRows = res->header.matrixColumns = 0;
    return;
  }

  if((y != res) && (x != res)) {
    if(!realMatrixInit(res, rows, cols)) {
      lastErrorCode = ERROR_RAM_FULL;
      return;
    }
  }
  for(i = 0; i < cols * rows; ++i) {
    if(subtraction) {
      real34Subtract(&y->matrixElements[i], &x->matrixElements[i], &res->matrixElements[i]);
    }
    else {
      real34Add(&y->matrixElements[i], &x->matrixElements[i], &res->matrixElements[i]);
    }
  }
}

void addRealMatrices(const real34Matrix_t *y, const real34Matrix_t *x, real34Matrix_t *res) {
  addSubRealMatrices(y, x, false, res);
}

void subtractRealMatrices(const real34Matrix_t *y, const real34Matrix_t *x, real34Matrix_t *res) {
  addSubRealMatrices(y, x, true, res);
}

static void addSubComplexMatrices(const complex34Matrix_t *y, const complex34Matrix_t *x, bool_t subtraction, complex34Matrix_t *res) {
  const uint16_t rows = y->header.matrixRows;
  const uint16_t cols = y->header.matrixColumns;
  int32_t i;

  if((y->header.matrixColumns != x->header.matrixColumns) || (y->header.matrixRows != x->header.matrixRows)) {
    res->matrixElements = NULL; // Matrix mismatch
    res->header.matrixRows = res->header.matrixColumns = 0;
    return;
  }

  if((y == res) || (x == res) || complexMatrixInit(res, rows, cols)) {
    for(i = 0; i < cols * rows; ++i) {
      if(subtraction) {
        real34Subtract(VARIABLE_REAL34_DATA(&y->matrixElements[i]), VARIABLE_REAL34_DATA(&x->matrixElements[i]), VARIABLE_REAL34_DATA(&res->matrixElements[i]));
        real34Subtract(VARIABLE_IMAG34_DATA(&y->matrixElements[i]), VARIABLE_IMAG34_DATA(&x->matrixElements[i]), VARIABLE_IMAG34_DATA(&res->matrixElements[i]));
      }
      else {
        real34Add(VARIABLE_REAL34_DATA(&y->matrixElements[i]), VARIABLE_REAL34_DATA(&x->matrixElements[i]), VARIABLE_REAL34_DATA(&res->matrixElements[i]));
        real34Add(VARIABLE_IMAG34_DATA(&y->matrixElements[i]), VARIABLE_IMAG34_DATA(&x->matrixElements[i]), VARIABLE_IMAG34_DATA(&res->matrixElements[i]));
      }
    }
  }
  else lastErrorCode = ERROR_RAM_FULL;
}

void addComplexMatrices(const complex34Matrix_t *y, const complex34Matrix_t *x, complex34Matrix_t *res) {
  addSubComplexMatrices(y, x, false, res);
}

void subtractComplexMatrices(const complex34Matrix_t *y, const complex34Matrix_t *x, complex34Matrix_t *res) {
  addSubComplexMatrices(y, x, true, res);
}



/* Multiplication */
void multiplyRealMatrix(const real34Matrix_t *matrix, const real34_t *x, real34Matrix_t *res) {
  const uint16_t rows = matrix->header.matrixRows;
  const uint16_t cols = matrix->header.matrixColumns;
  int32_t i;

  if(matrix == res || realMatrixInit(res, rows, cols)) {
    for(i = 0; i < cols * rows; ++i) {
      real34Multiply(&matrix->matrixElements[i], x, &res->matrixElements[i]);
    }
  }
  else lastErrorCode = ERROR_RAM_FULL;
}

void _multiplyRealMatrix(const real34Matrix_t *matrix, const real_t *x, real34Matrix_t *res, realContext_t *realContext) {
  const uint16_t rows = matrix->header.matrixRows;
  const uint16_t cols = matrix->header.matrixColumns;
  int32_t i;
  real_t y;

  if(matrix == res || realMatrixInit(res, rows, cols)) {
    for(i = 0; i < cols * rows; ++i) {
      real34ToReal(&matrix->matrixElements[i], &y);
      realMultiply(&y, x, &y, realContext);
      realToReal34(&y, &res->matrixElements[i]);
    }
  }
  else lastErrorCode = ERROR_RAM_FULL;
}

void multiplyRealMatrices(const real34Matrix_t *y, const real34Matrix_t *x, real34Matrix_t *res) {
  const uint16_t rows = y->header.matrixRows;
  const uint16_t iter = y->header.matrixColumns;
  const uint16_t cols = x->header.matrixColumns;
  int32_t i, j, k;
  real_t sum, prod, p, q;

  if(y->header.matrixColumns != x->header.matrixRows) {
    res->matrixElements = NULL; // Matrix mismatch
    res->header.matrixRows = res->header.matrixColumns = 0;
    return;
  }

  if(realMatrixInit(res, rows, cols)) {
    for(i = 0; i < rows; ++i) {
      for(j = 0; j < cols; ++j) {
        realCopy(const_0, &sum);
        realCopy(const_0, &prod);
        for(k = 0; k < iter; ++k) {
          real34ToReal(&y->matrixElements[i * iter + k], &p);
          real34ToReal(&x->matrixElements[k * cols + j], &q);
          realMultiply(&p, &q, &prod, &ctxtReal39);
          realAdd(&sum, &prod, &sum, &ctxtReal39);
        }
        realToReal34(&sum, &res->matrixElements[i * cols + j]);
      }
    }
  }
  else lastErrorCode = ERROR_RAM_FULL;
}

void multiplyComplexMatrix(const complex34Matrix_t *matrix, const real34_t *xr, const real34_t *xi, complex34Matrix_t *res) {
  real_t _xr, _xi;

  real34ToReal(xr, &_xr); real34ToReal(xi, &_xi);
  _multiplyComplexMatrix(matrix, &_xr, &_xi, res, &ctxtReal39);
}

void _multiplyComplexMatrix(const complex34Matrix_t *matrix, const real_t *xr, const real_t *xi, complex34Matrix_t *res, realContext_t *realContext) {
  const uint16_t rows = matrix->header.matrixRows;
  const uint16_t cols = matrix->header.matrixColumns;
  int32_t i;
  real_t yr, yi;

  if(matrix == res || complexMatrixInit(res, rows, cols)) {
    for(i = 0; i < cols * rows; ++i) {
      real34ToReal(VARIABLE_REAL34_DATA(&matrix->matrixElements[i]), &yr);
      real34ToReal(VARIABLE_IMAG34_DATA(&matrix->matrixElements[i]), &yi);
      mulComplexComplex(&yr, &yi, xr, xi, &yr, &yi, &ctxtReal39);
      realToReal34(&yr, VARIABLE_REAL34_DATA(&res->matrixElements[i]));
      realToReal34(&yi, VARIABLE_IMAG34_DATA(&res->matrixElements[i]));
    }
  }
  else lastErrorCode = ERROR_RAM_FULL;
}

void multiplyComplexMatrices(const complex34Matrix_t *y, const complex34Matrix_t *x, complex34Matrix_t *res) {
  const uint16_t rows = y->header.matrixRows;
  const uint16_t iter = y->header.matrixColumns;
  const uint16_t cols = x->header.matrixColumns;
  int32_t i, j, k;
  real_t sumr, prodr, pr, qr;
  real_t sumi, prodi, pi, qi;

  if(y->header.matrixColumns != x->header.matrixRows) {
    res->matrixElements = NULL; // Matrix mismatch
    res->header.matrixRows = res->header.matrixColumns = 0;
    return;
  }

  if(complexMatrixInit(res, rows, cols)) {
    for(i = 0; i < rows; ++i) {
      for(j = 0; j < cols; ++j) {
        realCopy(const_0, &sumr);  realCopy(const_0, &sumi);
        realCopy(const_0, &prodr); realCopy(const_0, &prodi);
        for(k = 0; k < iter; ++k) {
          real34ToReal(VARIABLE_REAL34_DATA(&y->matrixElements[i * iter + k]), &pr);
          real34ToReal(VARIABLE_IMAG34_DATA(&y->matrixElements[i * iter + k]), &pi);
          real34ToReal(VARIABLE_REAL34_DATA(&x->matrixElements[k * cols + j]), &qr);
          real34ToReal(VARIABLE_IMAG34_DATA(&x->matrixElements[k * cols + j]), &qi);
          mulComplexComplex(&pr, &pi, &qr, &qi, &prodr, &prodi, &ctxtReal39);
          realAdd(&sumr, &prodr, &sumr, &ctxtReal39);
          realAdd(&sumi, &prodi, &sumi, &ctxtReal39);
        }
        realToReal34(&sumr, VARIABLE_REAL34_DATA(&res->matrixElements[i * cols + j]));
        realToReal34(&sumi, VARIABLE_IMAG34_DATA(&res->matrixElements[i * cols + j]));
      }
    }
  }
  else lastErrorCode = ERROR_RAM_FULL;
}



/* Euclidean (Frobenius) norm */
static void _euclideanNormRealMatrix(const real34Matrix_t *matrix, real_t *res, realContext_t *realContext) {
  real_t elem;

  realZero(res);
  for(int i = 0; i < matrix->header.matrixRows * matrix->header.matrixColumns; ++i) {
    real34ToReal(&matrix->matrixElements[i], &elem);
    realFMA(&elem, &elem, res, res, realContext);
  }
  realSquareRoot(res, res, realContext);
}
void euclideanNormRealMatrix(const real34Matrix_t *matrix, real34_t *res) {
  real_t sum;

  _euclideanNormRealMatrix(matrix, &sum, &ctxtReal39);
  realToReal34(&sum, res);
}

void euclideanNormComplexMatrix(const complex34Matrix_t *matrix, real34_t *res) {
  real_t elem, sum;

  realZero(&sum);
  for(int i = 0; i < matrix->header.matrixRows * matrix->header.matrixColumns; ++i) {
    real34ToReal(VARIABLE_REAL34_DATA(&matrix->matrixElements[i]), &elem);
    realFMA(&elem, &elem, &sum, &sum, &ctxtReal39);
    real34ToReal(VARIABLE_IMAG34_DATA(&matrix->matrixElements[i]), &elem);
    realFMA(&elem, &elem, &sum, &sum, &ctxtReal39);
  }
  realSquareRoot(&sum, &sum, &ctxtReal39);
  realToReal34(&sum, res);
}



/* Vectors */
uint16_t realVectorSize(const real34Matrix_t *matrix) {
  if((matrix->header.matrixColumns != 1) && (matrix->header.matrixRows != 1))
    return 0;
  else
    return matrix->header.matrixColumns * matrix->header.matrixRows;
}

static void _dotRealVectors(const real34Matrix_t *y, const real34Matrix_t *x, real_t *res, realContext_t *realContext) {
  const uint16_t elements = realVectorSize(y);
  int32_t i;
  real_t sum, p, q;

  realCopy(const_0, &sum);
  for(i = 0; i < elements; ++i) {
    real34ToReal(&y->matrixElements[i], &p);
    real34ToReal(&x->matrixElements[i], &q);
    realFMA(&p, &q, &sum, &sum, realContext);
  }
  realCopy(&sum, res);
}
void dotRealVectors(const real34Matrix_t *y, const real34Matrix_t *x, real34_t *res) {
  real_t p;

  if((realVectorSize(y) == 0) || (realVectorSize(x) == 0) || (realVectorSize(y) != realVectorSize(x))) {
    realToReal34(const_NaN, res); // Not a vector or mismatched
    return;
  }

  _dotRealVectors(y, x, &p, &ctxtReal39);
  realToReal34(&p, res);
}

void crossRealVectors(const real34Matrix_t *y, const real34Matrix_t *x, real34Matrix_t *res) {
  const uint16_t elementsY = realVectorSize(y);
  const uint16_t elementsX = realVectorSize(x);
  real_t a1, a2, a3, b1, b2, b3, p, q;

  if((elementsY == 0) || (elementsX == 0) || (elementsY > 3) || (elementsX > 3)) {
    return; // Not a vector or mismatched
  }

  real34ToReal(                 &y->matrixElements[0]            , &a1);
  real34ToReal(elementsY >= 2 ? &y->matrixElements[1] : const34_0, &a2);
  real34ToReal(elementsY >= 3 ? &y->matrixElements[2] : const34_0, &a3);

  real34ToReal(                 &x->matrixElements[0]            , &b1);
  real34ToReal(elementsX >= 2 ? &x->matrixElements[1] : const34_0, &b2);
  real34ToReal(elementsX >= 3 ? &x->matrixElements[2] : const34_0, &b3);

  if(realMatrixInit(res, 1, 3)) {
    realMultiply(&a2, &b3, &p, &ctxtReal39); realMultiply(&a3, &b2, &q, &ctxtReal39);
    realSubtract(&p, &q, &p, &ctxtReal39); realToReal34(&p, &res->matrixElements[0]);

    realMultiply(&a3, &b1, &p, &ctxtReal39); realMultiply(&a1, &b3, &q, &ctxtReal39);
    realSubtract(&p, &q, &p, &ctxtReal39); realToReal34(&p, &res->matrixElements[1]);

    realMultiply(&a1, &b2, &p, &ctxtReal39); realMultiply(&a2, &b1, &q, &ctxtReal39);
    realSubtract(&p, &q, &p, &ctxtReal39); realToReal34(&p, &res->matrixElements[2]);
  }
  else lastErrorCode = ERROR_RAM_FULL;
}

uint16_t complexVectorSize(const complex34Matrix_t *matrix) {
  return realVectorSize((const real34Matrix_t *)matrix);
}

void dotComplexVectors(const complex34Matrix_t *y, const complex34Matrix_t *x, real34_t *res_r, real34_t *res_i) {
  const uint16_t elements = complexVectorSize(y);
  int32_t i;
  real_t sumr, prodr, pr, qr;
  real_t sumi, prodi, pi, qi;

  if((complexVectorSize(y) == 0) || (complexVectorSize(x) == 0) || (complexVectorSize(y) != complexVectorSize(x))) {
    realToReal34(const_NaN, res_r); realToReal34(const_NaN, res_i); // Not a vector or mismatched
    return;
  }

  realCopy(const_0, &sumr);  realCopy(const_0, &sumi);
  realCopy(const_0, &prodr); realCopy(const_0, &prodi);
  for(i = 0; i < elements; ++i) {
    real34ToReal(VARIABLE_REAL34_DATA(&y->matrixElements[i]), &pr); real34ToReal(VARIABLE_IMAG34_DATA(&y->matrixElements[i]), &pi);
    real34ToReal(VARIABLE_REAL34_DATA(&x->matrixElements[i]), &qr); real34ToReal(VARIABLE_IMAG34_DATA(&x->matrixElements[i]), &qi);
    mulComplexComplex(&pr, &pi, &qr, &qi, &prodr, &prodi, &ctxtReal39);
    realAdd(&sumr, &prodr, &sumr, &ctxtReal39);
    realAdd(&sumi, &prodi, &sumi, &ctxtReal39);
  }
  realToReal34(&sumr, res_r);
  realToReal34(&sumi, res_i);
}

void crossComplexVectors(const complex34Matrix_t *y, const complex34Matrix_t *x, complex34Matrix_t *res) {
  const uint16_t elementsY = complexVectorSize(y);
  const uint16_t elementsX = complexVectorSize(x);
  real_t a1r, a2r, a3r, b1r, b2r, b3r, pr, qr;
  real_t a1i, a2i, a3i, b1i, b2i, b3i, pi, qi;

  if((elementsY == 0) || (elementsX == 0) || (elementsY > 3) || (elementsX > 3)) {
    return; // Not a vector or mismatched
  }

  real34ToReal(                 VARIABLE_REAL34_DATA(&y->matrixElements[0])            , &a1r);
  real34ToReal(                 VARIABLE_IMAG34_DATA(&y->matrixElements[0])            , &a1i);
  real34ToReal(elementsY >= 2 ? VARIABLE_REAL34_DATA(&y->matrixElements[1]) : const34_0, &a2r);
  real34ToReal(elementsY >= 2 ? VARIABLE_IMAG34_DATA(&y->matrixElements[1]) : const34_0, &a2i);
  real34ToReal(elementsY >= 3 ? VARIABLE_REAL34_DATA(&y->matrixElements[2]) : const34_0, &a3r);
  real34ToReal(elementsY >= 3 ? VARIABLE_IMAG34_DATA(&y->matrixElements[2]) : const34_0, &a3i);

  real34ToReal(                 VARIABLE_REAL34_DATA(&x->matrixElements[0])            , &b1r);
  real34ToReal(                 VARIABLE_IMAG34_DATA(&x->matrixElements[0])            , &b1i);
  real34ToReal(elementsX >= 2 ? VARIABLE_REAL34_DATA(&x->matrixElements[1]) : const34_0, &b2r);
  real34ToReal(elementsX >= 2 ? VARIABLE_IMAG34_DATA(&x->matrixElements[1]) : const34_0, &b2i);
  real34ToReal(elementsX >= 3 ? VARIABLE_REAL34_DATA(&x->matrixElements[2]) : const34_0, &b3r);
  real34ToReal(elementsX >= 3 ? VARIABLE_IMAG34_DATA(&x->matrixElements[2]) : const34_0, &b3i);

  if(complexMatrixInit(res, 1, 3)) {
    mulComplexComplex(&a2r, &a2i, &b3r, &b3i, &pr, &pi, &ctxtReal39);
    mulComplexComplex(&a3r, &a3i, &b2r, &b2i, &qr, &qi, &ctxtReal39);
    realSubtract(&pr, &qr, &pr, &ctxtReal39), realSubtract(&pi, &qi, &pi, &ctxtReal39);
    realToReal34(&pr, VARIABLE_REAL34_DATA(&res->matrixElements[0]));
    realToReal34(&pi, VARIABLE_IMAG34_DATA(&res->matrixElements[0]));

    mulComplexComplex(&a3r, &a3i, &b1r, &b1i, &pr, &pi, &ctxtReal39);
    mulComplexComplex(&a1r, &a1i, &b3r, &b3i, &qr, &qi, &ctxtReal39);
    realSubtract(&pr, &qr, &pr, &ctxtReal39), realSubtract(&pi, &qi, &pi, &ctxtReal39);
    realToReal34(&pr, VARIABLE_REAL34_DATA(&res->matrixElements[1]));
    realToReal34(&pi, VARIABLE_IMAG34_DATA(&res->matrixElements[1]));

    mulComplexComplex(&a1r, &a1i, &b2r, &b2i, &pr, &pi, &ctxtReal39);
    mulComplexComplex(&a2r, &a2i, &b1r, &b1i, &qr, &qi, &ctxtReal39);
    realSubtract(&pr, &qr, &pr, &ctxtReal39), realSubtract(&pi, &qi, &pi, &ctxtReal39);
    realToReal34(&pr, VARIABLE_REAL34_DATA(&res->matrixElements[2]));
    realToReal34(&pi, VARIABLE_IMAG34_DATA(&res->matrixElements[2]));
  }
  else lastErrorCode = ERROR_RAM_FULL;
}

void vectorAngle(const real34Matrix_t *y, const real34Matrix_t *x, real34_t *radians) {
  const uint16_t elements = realVectorSize(y);
  real_t a, b;

  if((elements == 0) || (realVectorSize(x) == 0) || (elements != realVectorSize(x))) {
    realToReal34(const_NaN, radians); // Not a vector or mismatched
    return;
  }

  if(elements == 2 || elements == 3) {
    _dotRealVectors(y, x, &a, &ctxtReal39);
    _euclideanNormRealMatrix(y, &b, &ctxtReal39);
    realDivide(&a, &b, &a, &ctxtReal39);
    _euclideanNormRealMatrix(x, &b, &ctxtReal39);
    realDivide(&a, &b, &a, &ctxtReal39);
    WP34S_Acos(&a, &a, &ctxtReal39);
    realToReal34(&a, radians);
  }
  else {
    realToReal34(const_NaN, radians);
  }
}



/* LU decomposition routine borrowed from WP 34s */
void WP34S_LU_decomposition(const real34Matrix_t *matrix, real34Matrix_t *lu, uint16_t *p) {
  int i, j, k;
  int pvt;
  real_t max, t, u;
  real_t *tmpMat;

  const uint16_t m = matrix->header.matrixRows;
  const uint16_t n = matrix->header.matrixColumns;

  if(matrix->header.matrixRows != matrix->header.matrixColumns) {
    if(matrix != lu) {
      lu->matrixElements = NULL; // Matrix is not square
      lu->header.matrixRows = lu->header.matrixColumns = 0;
    }
    return;
  }

  if((tmpMat = allocWp43s(m * n * REAL_SIZE))) {
    if(matrix != lu) copyRealMatrix(matrix, lu);

    if(lu->matrixElements) {
      for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
          real34ToReal(&lu->matrixElements[i * n + j], &tmpMat[i * n + j]);
        }
      }

      for(k = 0; k < n; k++) {
        /* Find the pivot row */
        pvt = k;
        realCopy(&tmpMat[k * n + k], &u);
        realCopyAbs(&u, &max);
        for(j = k + 1; j < n; j++) {
          realCopy(&tmpMat[j * n + k], &t);
          realCopyAbs(&t, &u);
          if (realCompareGreaterThan(&u, &max)) {
            realCopy(&u, &max);
            pvt = j;
          }
        }
        if(p != NULL)
          *p++ = pvt;

        /* pivot if required */
        if(pvt != k) {
          for(i = 0; i < n; i++) {
            realCopy(&tmpMat[k   * n + i], &t                  );
            realCopy(&tmpMat[pvt * n + i], &tmpMat[k   * n + i]);
            realCopy(&t,                   &tmpMat[pvt * n + i]);
          }
        }

        /* Check for singular */
        realCopy(&tmpMat[k * n + k], &t);
        if(realIsZero(&t)) {
          realMatrixFree(lu);
          return;
        }

        /* Find the lower triangular elements for column k */
        for(i = k + 1; i < n; i++) {
          realCopy(&tmpMat[k * n + k], &t);
          realCopy(&tmpMat[i * n + k], &u);
          realDivide(&u, &t, &max, &ctxtReal39);
          realCopy(&max, &tmpMat[i * n + k]);
        }
        /* Update the upper triangular elements */
        for(i = k + 1; i < n; i++) {
          for(j = k + 1; j < n; j++) {
            realCopy(&tmpMat[i * n + k], &t);
            realCopy(&tmpMat[k * n + j], &u);
            realMultiply(&t, &u, &max, &ctxtReal39);
            realCopy(&tmpMat[i * n + j], &t);
            realSubtract(&t, &max, &u, &ctxtReal39);
            realCopy(&u, &tmpMat[i * n + j]);
          }
        }
      }

      for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
          realToReal34(&tmpMat[i * n + j], &lu->matrixElements[i * n + j]);
        }
      }
    }
    else lastErrorCode = ERROR_RAM_FULL;

    freeWp43s(tmpMat, m * n * REAL_SIZE);
  }
  else {
    if(matrix != lu) {
      lu->matrixElements = NULL;
      lu->header.matrixRows = lu->header.matrixColumns = 0;
    }
    lastErrorCode = ERROR_RAM_FULL;
  }
}

static bool_t luCpxMat(real_t *tmpMat, uint16_t size, uint16_t *p, realContext_t *realContext) {
  int i, j, k;
  int pvt;
  real_t max, t, u, v;

  const uint16_t n = size;

  for(k = 0; k < n; k++) {
    /* Find the pivot row */
    pvt = k;
    complexMagnitude(&tmpMat[(k * n + k) * 2], &tmpMat[(k * n + k) * 2 + 1], &max, realContext);
    for(j = k + 1; j < n; j++) {
      complexMagnitude(&tmpMat[(j * n + k) * 2], &tmpMat[(j * n + k) * 2 + 1], &u, realContext);
      if(realCompareGreaterThan(&u, &max)) {
        realCopy(&u, &max);
        pvt = j;
      }
    }
    if(p != NULL)
      *p++ = pvt;

    /* pivot if required */
    if(pvt != k) {
      for(i = 0; i < n; i++) {
        realCopy(&tmpMat[(k   * n + i) * 2    ], &t                            );
        realCopy(&tmpMat[(pvt * n + i) * 2    ], &tmpMat[(k   * n + i) * 2    ]);
        realCopy(&t,                             &tmpMat[(pvt * n + i) * 2    ]);
        realCopy(&tmpMat[(k   * n + i) * 2 + 1], &t                            );
        realCopy(&tmpMat[(pvt * n + i) * 2 + 1], &tmpMat[(k   * n + i) * 2 + 1]);
        realCopy(&t,                             &tmpMat[(pvt * n + i) * 2 + 1]);
      }
    }

    /* Check for singular */
    if(realIsZero(&tmpMat[(k * n + k) * 2]) && realIsZero(&tmpMat[(k * n + k) * 2 + 1])) {
      return false;
    }

    /* Find the lower triangular elements for column k */
    for(i = k + 1; i < n; i++) {
      divComplexComplex(&tmpMat[(i * n + k) * 2], &tmpMat[(i * n + k) * 2 + 1], &tmpMat[(k * n + k) * 2], &tmpMat[(k * n + k) * 2 + 1], &t, &u, realContext);
      realCopy(&t, &tmpMat[(i * n + k) * 2    ]);
      realCopy(&u, &tmpMat[(i * n + k) * 2 + 1]);
    }
    /* Update the upper triangular elements */
    for(i = k + 1; i < n; i++) {
      for(j = k + 1; j < n; j++) {
        mulComplexComplex(&tmpMat[(i * n + k) * 2], &tmpMat[(i * n + k) * 2 + 1], &tmpMat[(k * n + j) * 2], &tmpMat[(k * n + j) * 2 + 1], &t, &u, realContext);
        realCopy(&tmpMat[(i * n + j) * 2], &v), realCopy(&tmpMat[(i * n + j) * 2 + 1], &max);
        realSubtract(&v,   &t, &tmpMat[(i * n + j) * 2    ], realContext);
        realSubtract(&max, &u, &tmpMat[(i * n + j) * 2 + 1], realContext);
      }
    }
  }

  return true;
}

void complex_LU_decomposition(const complex34Matrix_t *matrix, complex34Matrix_t *lu, uint16_t *p) {
  int i, j;
  real_t *tmpMat;

  const uint16_t m = matrix->header.matrixRows;
  const uint16_t n = matrix->header.matrixColumns;

  if(matrix->header.matrixRows != matrix->header.matrixColumns) {
    if(matrix != lu) {
      lu->matrixElements = NULL; // Matrix is not square
      lu->header.matrixRows = lu->header.matrixColumns = 0;
    }
    return;
  }

  if((tmpMat = allocWp43s(m * n * REAL_SIZE * 2))) {
    if(matrix != lu) copyComplexMatrix(matrix, lu);

    if(lu->matrixElements) {
      for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
          real34ToReal(VARIABLE_REAL34_DATA(&lu->matrixElements[i * n + j]), &tmpMat[(i * n + j) * 2    ]);
          real34ToReal(VARIABLE_IMAG34_DATA(&lu->matrixElements[i * n + j]), &tmpMat[(i * n + j) * 2 + 1]);
        }
      }

      if(luCpxMat(tmpMat, n, p, &ctxtReal39)) {
        for(i = 0; i < n; i++) {
          for(j = 0; j < n; j++) {
            realToReal34(&tmpMat[(i * n + j) * 2    ], VARIABLE_REAL34_DATA(&lu->matrixElements[i * n + j]));
            realToReal34(&tmpMat[(i * n + j) * 2 + 1], VARIABLE_IMAG34_DATA(&lu->matrixElements[i * n + j]));
          }
        }
      }
    }
    else lastErrorCode = ERROR_RAM_FULL;

    freeWp43s(tmpMat, m * n * REAL_SIZE * 2);
  }
  else {
    if(matrix != lu) {
      lu->matrixElements = NULL; // Matrix is not square
      lu->header.matrixRows = lu->header.matrixColumns = 0;
    }
    lastErrorCode = ERROR_RAM_FULL;
  }
}



/* Swap 2 rows */
void realMatrixSwapRows(const real34Matrix_t *matrix, real34Matrix_t *res, uint16_t a, uint16_t b) {
  const uint16_t rows = matrix->header.matrixRows;
  const uint16_t cols = matrix->header.matrixColumns;
  uint16_t i;
  real34_t t;

  if(matrix != res) copyRealMatrix(matrix, res);
  if(res->matrixElements) {
    if((a < rows) && (b < rows) && (a != b)) {
      for(i = 0; i < cols; i++) {
        real34Copy(&res->matrixElements[a * cols + i], &t);
        real34Copy(&res->matrixElements[b * cols + i], &res->matrixElements[a * cols + i]);
        real34Copy(&t,                                 &res->matrixElements[b * cols + i]);
      }
    }
  }
  else lastErrorCode = ERROR_RAM_FULL;
}

void complexMatrixSwapRows(const complex34Matrix_t *matrix, complex34Matrix_t *res, uint16_t a, uint16_t b) {
  const uint16_t rows = matrix->header.matrixRows;
  const uint16_t cols = matrix->header.matrixColumns;
  uint16_t i;
  real34_t t;

  if(matrix != res) copyComplexMatrix(matrix, res);
  if(res->matrixElements) {
    if((a < rows) && (b < rows) && (a != b)) {
      for(i = 0; i < cols; i++) {
        real34Copy(VARIABLE_REAL34_DATA(&res->matrixElements[a * cols + i]), &t);
        real34Copy(VARIABLE_REAL34_DATA(&res->matrixElements[b * cols + i]), VARIABLE_REAL34_DATA(&res->matrixElements[a * cols + i]));
        real34Copy(&t,                                                       VARIABLE_REAL34_DATA(&res->matrixElements[b * cols + i]));
        real34Copy(VARIABLE_IMAG34_DATA(&res->matrixElements[a * cols + i]), &t);
        real34Copy(VARIABLE_IMAG34_DATA(&res->matrixElements[b * cols + i]), VARIABLE_IMAG34_DATA(&res->matrixElements[a * cols + i]));
        real34Copy(&t,                                                       VARIABLE_IMAG34_DATA(&res->matrixElements[b * cols + i]));
      }
    }
  }
  else lastErrorCode = ERROR_RAM_FULL;
}



/* Determinant */
void detRealMatrix(const real34Matrix_t *matrix, real34_t *res) {
  const uint16_t n = matrix->header.matrixColumns;
  uint16_t *p;
  real34Matrix_t lu;
  real_t t, u;

  if(matrix->header.matrixRows != n) {
    realToReal34(const_NaN, res);
    return;
  }

  if((p = allocWp43s(TO_BLOCKS(matrix->header.matrixRows * sizeof(uint16_t))))) {
    WP34S_LU_decomposition(matrix, &lu, p);
    realCopy(const_1, &t);
    if(lu.matrixElements) {
      for(uint16_t i = 0; i < n; ++i) {
        real34ToReal(&lu.matrixElements[i * n + i], &u);
        //if(p[i] != i) realChangeSign(&u);
        realMultiply(&t, &u, &t, &ctxtReal51);
      }
      realToReal34(&t, res);
    }
    else { // singular
      real34Copy(const34_0, res);
    }

    freeWp43s(p, TO_BLOCKS(matrix->header.matrixRows * sizeof(uint16_t)));
  }
  else lastErrorCode = ERROR_RAM_FULL;
}

void detComplexMatrix(const complex34Matrix_t *matrix, real34_t *res_r, real34_t *res_i) {
  const uint16_t n = matrix->header.matrixColumns;
  uint16_t *p;
  complex34Matrix_t lu;
  real_t tr, ti, ur, ui;

  if(matrix->header.matrixRows != n) {
    realToReal34(const_NaN, res_r);
    realToReal34(const_NaN, res_i);
    return;
  }

  if((p = allocWp43s(TO_BLOCKS(matrix->header.matrixRows * sizeof(uint16_t))))) {
    complex_LU_decomposition(matrix, &lu, p);
    realCopy(const_1, &tr), realCopy(const_0, &ti);
    if(lu.matrixElements) {
      for(uint16_t i = 0; i < n; ++i) {
        real34ToReal(VARIABLE_REAL34_DATA(&lu.matrixElements[i * n + i]), &ur);
        real34ToReal(VARIABLE_IMAG34_DATA(&lu.matrixElements[i * n + i]), &ui);
        mulComplexComplex(&tr, &ti, &ur, &ui, &tr, &ti, &ctxtReal51);
      }
      realToReal34(&tr, res_r); realToReal34(&ti, res_i);
    }
    else { // singular
      real34Copy(const34_0, res_r); real34Copy(const34_0, res_i);
    }

    freeWp43s(p, TO_BLOCKS(matrix->header.matrixRows * sizeof(uint16_t)));
  }
  else lastErrorCode = ERROR_RAM_FULL;
}



/* Solve the linear equation Ax = b.
 * We do this by utilising the LU decomposition passed in in A and solving
 * the linear equation Ly = b for y, where L is the lower diagonal triangular
 * matrix with unity along the diagonal.  Then we solve the linear system
 * Ux = y, where U is the upper triangular matrix.
 */
static void WP34S_matrix_pivoting_solve(const real34Matrix_t *LU, real_t *b, uint16_t *pivot, real_t *x, realContext_t *realContext) {
  const uint16_t n = LU->header.matrixColumns;
  uint16_t i, k;
  real_t r, t;

  /* Solve the first linear equation Ly = b */
  for(k = 0; k < n; k++) {
    if(k != pivot[k]) {
      real_t swap;
      realCopy(&b[k], &swap);
      realCopy(&b[pivot[k]], &b[k]);
      realCopy(&swap, &b[pivot[k]]);
    }
    realCopy(&b[k], x + k);
    for(i = 0; i < k; i++) {
      real34ToReal(&LU->matrixElements[k * n + i], &r);
      realMultiply(&r, x + i, &t, realContext);
      realSubtract(x + k, &t, x + k, realContext);
    }
  }

  /* Solve the second linear equation Ux = y */
  for(k = n; k > 0; k--) {
    --k;
    for(i = k + 1; i < n; i++) {
      real34ToReal(&LU->matrixElements[k * n + i], &r);
      realMultiply(&r, x + i, &t, realContext);
      realSubtract(x + k, &t, x + k, realContext);
    }
    real34ToReal(&LU->matrixElements[k * n + k], &r);
    realDivide(x + k, &r, x + k, realContext);
    ++k;
  }
}

static void complex_matrix_pivoting_solve(const real_t *LU, uint16_t n, real_t *b, uint16_t *pivot, real_t *x, realContext_t *realContext) {
  uint16_t i, k;
  real_t rr, ri, tr, ti;

  /* Solve the first linear equation Ly = b */
  for(k = 0; k < n; k++) {
    if(k != pivot[k]) {
      real_t swap;
      realCopy(&b[k * 2], &swap);
      realCopy(&b[pivot[k] * 2], &b[k * 2]);
      realCopy(&swap, &b[pivot[k] * 2]);
      realCopy(&b[k * 2 + 1], &swap);
      realCopy(&b[pivot[k] * 2 + 1], &b[k * 2 + 1]);
      realCopy(&swap, &b[pivot[k] * 2 + 1]);
    }
    realCopy(&b[k * 2    ], x + (k * 2    ));
    realCopy(&b[k * 2 + 1], x + (k * 2 + 1));
    for(i = 0; i < k; i++) {
      realCopy(LU + (k * n + i) * 2,     &rr);
      realCopy(LU + (k * n + i) * 2 + 1, &ri);
      mulComplexComplex(&rr, &ri, x + i * 2, x + (i * 2 + 1), &tr, &ti, realContext);
      realSubtract(x + (k * 2    ), &tr, x + (k * 2    ), realContext);
      realSubtract(x + (k * 2 + 1), &ti, x + (k * 2 + 1), realContext);
    }
  }

  /* Solve the second linear equation Ux = y */
  for(k = n; k > 0; k--) {
    --k;
    for(i = k + 1; i < n; i++) {
      realCopy(LU + (k * n + i) * 2,     &rr);
      realCopy(LU + (k * n + i) * 2 + 1, &ri);
      mulComplexComplex(&rr, &ri, x + i * 2, x + (i * 2 + 1), &tr, &ti, realContext);
      realSubtract(x + (k * 2    ), &tr, x + (k * 2    ), realContext);
      realSubtract(x + (k * 2 + 1), &ti, x + (k * 2 + 1), realContext);
    }
    realCopy(LU + (k * n + k) * 2,     &rr);
    realCopy(LU + (k * n + k) * 2 + 1, &ri);
    divComplexComplex(x + (k * 2), x + (k * 2 + 1), &rr, &ri, x + (k * 2), x + (k * 2 + 1), realContext);
    ++k;
  }
}

/* Invert a matrix
 * Do this by calculating the LU decomposition and solving lots of systems
 * of linear equations.
 */
void WP34S_matrix_inverse(const real34Matrix_t *matrix, real34Matrix_t *res) {
  const uint16_t n = matrix->header.matrixColumns;
  real_t *x;
  real34Matrix_t lu;
  uint16_t *pivots;
  uint16_t i, j;
  real_t *b;

  if(matrix->header.matrixRows != matrix->header.matrixColumns) {
    if(matrix != res) {
      res->matrixElements = NULL; // Matrix is not square
      res->header.matrixRows = res->header.matrixColumns = 0;
    }
    return;
  }

  if((pivots = allocWp43s(TO_BLOCKS(matrix->header.matrixRows * sizeof(uint16_t))))) {
    WP34S_LU_decomposition(matrix, &lu, pivots);
    if(lu.matrixElements == NULL) {
      freeWp43s(pivots, TO_BLOCKS(matrix->header.matrixRows * sizeof(uint16_t)));
      if(matrix != res) {
        res->matrixElements = NULL; // Singular matrix
        res->header.matrixRows = res->header.matrixColumns = 0;
      }
      return;
    }

    {
      real34_t maxVal, minVal;
      real_t p, q;
      real34CopyAbs(&lu.matrixElements[0], &maxVal);
      real34CopyAbs(&lu.matrixElements[0], &minVal);
      for(i = 1; i < n; ++i) {
        if(real34CompareAbsLessThan(&lu.matrixElements[i * n + i], &minVal))
          real34CopyAbs(&lu.matrixElements[i * n + i], &minVal);
        if(real34CompareAbsGreaterThan(&lu.matrixElements[i * n + i], &maxVal))
          real34CopyAbs(&lu.matrixElements[i * n + i], &maxVal);
      }
      real34ToReal(&maxVal, &p);
      real34ToReal(&minVal, &q);
      WP34S_Log10(&p, &p, &ctxtReal39);
      WP34S_Log10(&q, &q, &ctxtReal39);
      realSubtract(&p, &q, &p, &ctxtReal39);
      int32ToReal(33 - displayFormatDigits, &q);
      if(realCompareLessEqual(&q, &p)) {
        temporaryInformation = TI_INACCURATE;
      }
    }

    if(matrix != res) copyRealMatrix(matrix, res);

    if(res->matrixElements) {
      if((x = allocWp43s(res->header.matrixRows * REAL_SIZE))) {
        if((b = allocWp43s(res->header.matrixRows * REAL_SIZE))) {
          for(i = 0; i < n; i++) {
            for(j = 0; j < n; j++)
              realCopy((i == j) ? const_1 : const_0, &b[j]);
            WP34S_matrix_pivoting_solve(&lu, b, pivots, x, &ctxtReal39);
            for(j = 0; j < n; j++)
              realToReal34(x + j, &res->matrixElements[j * n + i]);
          }
          realMatrixFree(&lu);
          freeWp43s(b, res->header.matrixRows * REAL_SIZE);
        }
        else lastErrorCode = ERROR_RAM_FULL;
        freeWp43s(x, res->header.matrixRows * REAL_SIZE);
      }
      else lastErrorCode = ERROR_RAM_FULL;
    }
    else lastErrorCode = ERROR_RAM_FULL;
    freeWp43s(pivots, TO_BLOCKS(matrix->header.matrixRows * sizeof(uint16_t)));
  }
  else lastErrorCode = ERROR_RAM_FULL;
}

static bool_t invCpxMat(real_t *matrix, uint16_t n, realContext_t *realContext) {
  real_t *x;
  real_t *lu;
  uint16_t *pivots;
  uint16_t i, j;
  real_t *b;

  if((lu = allocWp43s(n * n * REAL_SIZE * 2))) {
    if((pivots = allocWp43s(TO_BLOCKS(n * sizeof(uint16_t))))) {
      for(i = 0; i < n * n * 2; i++) realCopy(matrix + i, lu + i);
      if(!luCpxMat(lu, n, pivots, realContext)) {
        freeWp43s(lu, n * n * REAL_SIZE * 2);
        freeWp43s(pivots, TO_BLOCKS(n * sizeof(uint16_t)));
        return false;
      }

      {
        real_t maxVal, minVal;
        real_t p, q;
        realCopy(lu,     &p);
        realCopy(lu + 1, &q);
        complexMagnitude(&p, &q, &p, realContext);
        realCopy(&p, &maxVal);
        realCopy(&p, &minVal);
        for(i = 1; i < n; ++i) {
          realCopy(lu + (i * n + i) * 2,     &p);
          realCopy(lu + (i * n + i) * 2 + 1, &q);
          complexMagnitude(&p, &q, &p, realContext);
          if(realCompareLessThan(&p, &minVal))
            real34Copy(&p, &minVal);
          if(realCompareGreaterThan(&p, &maxVal))
            real34Copy(&p, &maxVal);
        }
        WP34S_Log10(&maxVal, &p, realContext);
        WP34S_Log10(&minVal, &q, realContext);
        realSubtract(&p, &q, &p, realContext);
        int32ToReal(33 - displayFormatDigits, &q);
        if(realCompareLessEqual(&q, &p)) {
          temporaryInformation = TI_INACCURATE;
        }
      }

      if((x = allocWp43s(n * REAL_SIZE * 2))) {
        if((b = allocWp43s(n * REAL_SIZE * 2))) {
          for(i = 0; i < n; i++) {
            for(j = 0; j < n; j++) {
              realCopy((i == j) ? const_1 : const_0, &b[j * 2    ]);
              realCopy(                     const_0, &b[j * 2 + 1]);
            }
            complex_matrix_pivoting_solve(lu, n, b, pivots, x, realContext);
            for(j = 0; j < n; j++) {
              realCopy(x + j * 2,     matrix + (j * n + i) * 2    );
              realCopy(x + j * 2 + 1, matrix + (j * n + i) * 2 + 1);
            }
          }
          freeWp43s(b, n * REAL_SIZE * 2);
        }
        else lastErrorCode = ERROR_RAM_FULL;
        freeWp43s(x, n * REAL_SIZE * 2);
      }
      else lastErrorCode = ERROR_RAM_FULL;
      freeWp43s(pivots, TO_BLOCKS(n * sizeof(uint16_t)));
    }
    else lastErrorCode = ERROR_RAM_FULL;
    freeWp43s(lu, n * n * REAL_SIZE * 2);
  }
  else lastErrorCode = ERROR_RAM_FULL;
  return lastErrorCode == ERROR_NONE;
}

void complex_matrix_inverse(const complex34Matrix_t *matrix, complex34Matrix_t *res) {
  const uint16_t n = matrix->header.matrixColumns;
  real_t *tmpMat;
  uint16_t i, j;

  if(matrix->header.matrixRows != matrix->header.matrixColumns) {
    if(matrix != res) {
      res->matrixElements = NULL; // Matrix is not square
      res->header.matrixRows = res->header.matrixColumns = 0;
    }
    return;
  }

  if((tmpMat = allocWp43s(n * n * REAL_SIZE * 2))) {
    for(i = 0; i < n; i++) {
      for(j = 0; j < n; j++) {
        real34ToReal(VARIABLE_REAL34_DATA(&matrix->matrixElements[i * n + j]), &tmpMat[(i * n + j) * 2    ]);
        real34ToReal(VARIABLE_IMAG34_DATA(&matrix->matrixElements[i * n + j]), &tmpMat[(i * n + j) * 2 + 1]);
      }
    }

    if(invCpxMat(tmpMat, n, &ctxtReal39)) {
      if(matrix != res) copyComplexMatrix(matrix, res);
      if(res->matrixElements) {
        for(i = 0; i < n; i++) {
          for(j = 0; j < n; j++) {
            realToReal34(&tmpMat[(i * n + j) * 2    ], VARIABLE_REAL34_DATA(&res->matrixElements[i * n + j]));
            realToReal34(&tmpMat[(i * n + j) * 2 + 1], VARIABLE_IMAG34_DATA(&res->matrixElements[i * n + j]));
          }
        }
      }
      else lastErrorCode = ERROR_RAM_FULL;
    }

    freeWp43s(tmpMat, n * n * REAL_SIZE * 2);
  }
  else lastErrorCode = ERROR_RAM_FULL;
}



/* Division */
void divideRealMatrix(const real34Matrix_t *matrix, const real34_t *x, real34Matrix_t *res) {
  const uint16_t rows = matrix->header.matrixRows;
  const uint16_t cols = matrix->header.matrixColumns;
  int32_t i;

  if(matrix == res || realMatrixInit(res, rows, cols)) {
    for(i = 0; i < cols * rows; ++i) {
      real34Divide(&matrix->matrixElements[i], x, &res->matrixElements[i]);
    }
  }
  else lastErrorCode = ERROR_RAM_FULL;
}

void _divideRealMatrix(const real34Matrix_t *matrix, const real_t *x, real34Matrix_t *res, realContext_t *realContext) {
  const uint16_t rows = matrix->header.matrixRows;
  const uint16_t cols = matrix->header.matrixColumns;
  int32_t i;
  real_t y;

  if(matrix == res || realMatrixInit(res, rows, cols)) {
    for(i = 0; i < cols * rows; ++i) {
      real34ToReal(&matrix->matrixElements[i], &y);
      realDivide(&y, x, &y, realContext);
      realToReal34(&y, &res->matrixElements[i]);
    }
  }
  else lastErrorCode = ERROR_RAM_FULL;
}

void divideRealMatrices(const real34Matrix_t *y, const real34Matrix_t *x, real34Matrix_t *res) {
  real34Matrix_t invX;

  if(y->header.matrixColumns != x->header.matrixRows || x->header.matrixRows != x->header.matrixColumns) {
    res->matrixElements = NULL; // Matrix mismatch
    res->header.matrixRows = res->header.matrixColumns = 0;
    return;
  }

  WP34S_matrix_inverse(x, &invX);
  if(lastErrorCode == ERROR_NONE && invX.matrixElements) {
    multiplyRealMatrices(y, &invX, res);
  }
  else {
    if(y != res && x != res) {
      res->matrixElements = NULL; // Singular matrix
      res->header.matrixRows = res->header.matrixColumns = 0;
    }
  }
  realMatrixFree(&invX);
}

void divideComplexMatrix(const complex34Matrix_t *matrix, const real34_t *xr, const real34_t *xi, complex34Matrix_t *res) {
  real_t _xr, _xi;

  real34ToReal(xr, &_xr); real34ToReal(xi, &_xi);
  _divideComplexMatrix(matrix, &_xr, &_xi, res, &ctxtReal39);
}

void _divideComplexMatrix(const complex34Matrix_t *matrix, const real_t *xr, const real_t *xi, complex34Matrix_t *res, realContext_t *realContext) {
  const uint16_t rows = matrix->header.matrixRows;
  const uint16_t cols = matrix->header.matrixColumns;
  int32_t i;
  real_t yr, yi;

  if(matrix == res || complexMatrixInit(res, rows, cols)) {
    for(i = 0; i < cols * rows; ++i) {
      real34ToReal(VARIABLE_REAL34_DATA(&matrix->matrixElements[i]), &yr);
      real34ToReal(VARIABLE_IMAG34_DATA(&matrix->matrixElements[i]), &yi);
      divComplexComplex(&yr, &yi, xr, xi, &yr, &yi, realContext);
      realToReal34(&yr, VARIABLE_REAL34_DATA(&res->matrixElements[i]));
      realToReal34(&yi, VARIABLE_IMAG34_DATA(&res->matrixElements[i]));
    }
  }
  else lastErrorCode = ERROR_RAM_FULL;
}

void divideComplexMatrices(const complex34Matrix_t *y, const complex34Matrix_t *x, complex34Matrix_t *res) {
  complex34Matrix_t invX;

  if(y->header.matrixColumns != x->header.matrixRows || x->header.matrixRows != x->header.matrixColumns) {
    res->matrixElements = NULL; // Matrix mismatch
    res->header.matrixRows = res->header.matrixColumns = 0;
    return;
  }

  complex_matrix_inverse(x, &invX);
  if(lastErrorCode == ERROR_NONE && invX.matrixElements) {
    multiplyComplexMatrices(y, &invX, res);
  }
  else {
    if(y != res && x != res) {
      res->matrixElements = NULL; // Singular matrix
      res->header.matrixRows = res->header.matrixColumns = 0;
    }
  }
  complexMatrixFree(&invX);
}



/* Solve a system of linear equations Ac = b
 */
void WP34S_matrix_linear_eqn(const real34Matrix_t *a, const real34Matrix_t *b, real34Matrix_t *r) {
  const uint16_t n = a->header.matrixRows;
  int i;
  real34Matrix_t mat;
  real_t *cv = NULL;
  uint16_t *pivots = NULL;
  real_t *bv = NULL;

  if(r != a && r != b) {
    r->matrixElements = NULL;
    r->header.matrixRows = r->header.matrixColumns = 0;
  }

  if(a->header.matrixColumns != n) {
    displayCalcErrorMessage(ERROR_MATRIX_MISMATCH, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "not a square matrix (%d" STD_CROSS "%d)",
              n, a->header.matrixColumns);
      moreInfoOnError("In function WP34S_matrix_linear_eqn:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return;
  }
  if(b->header.matrixRows != n || b->header.matrixColumns != 1) {
    displayCalcErrorMessage(ERROR_MATRIX_MISMATCH, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "not a column vector or size mismatch (%d" STD_CROSS "%d)",
              b->header.matrixRows, b->header.matrixColumns);
      moreInfoOnError("In function WP34S_matrix_linear_eqn:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return;
  }

  /* Everything is happy so far -- decompose */
  if((pivots = allocWp43s(TO_BLOCKS(sizeof(uint16_t) * n)))) {
    WP34S_LU_decomposition(a, &mat, pivots);
    if(lastErrorCode != ERROR_NONE) {
      freeWp43s(&pivots, TO_BLOCKS(sizeof(uint16_t) * n));
      return;
    }
    if(!mat.matrixElements) {
      freeWp43s(&pivots, TO_BLOCKS(sizeof(uint16_t) * n));
      displayCalcErrorMessage(ERROR_SINGULAR_MATRIX, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "attempt to LU-decompose a singular matrix");
        moreInfoOnError("In function WP34S_matrix_linear_eqn:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      return;
    }

    /* And solve */
    if(r == a)
      realMatrixFree(r);
    if((r == b) || (realMatrixInit(r, n, 1))) {
      if((cv = allocWp43s(n * REAL_SIZE))) {
        if((bv = allocWp43s(n * REAL_SIZE))) {
          for(i = 0; i < n; i++) {
            real34ToReal(&b->matrixElements[i] , bv + i);
            pivots[i] = i;
          }
          WP34S_matrix_pivoting_solve(&mat, bv, pivots, cv, &ctxtReal39);
          for(i = 0; i < n; i++)
            realToReal34(cv + i, &r->matrixElements[i]);
          freeWp43s(bv, n * REAL_SIZE);
        }
        else lastErrorCode = ERROR_RAM_FULL;
        freeWp43s(cv, n * REAL_SIZE);
      }
      else lastErrorCode = ERROR_RAM_FULL;
    }
    else lastErrorCode = ERROR_RAM_FULL;
    freeWp43s(pivots, TO_BLOCKS(sizeof(uint16_t) * n));
  }
  else lastErrorCode = ERROR_RAM_FULL;
}

#if 0
void complex_matrix_linear_eqn(const complex34Matrix_t *a, const complex34Matrix_t *b, complex34Matrix_t *r) {
  const uint16_t n = a->header.matrixRows;
  int i;
  complex34Matrix_t mat;
  real_t *cv = NULL;
  uint16_t *pivots = NULL;
  real_t *bv = NULL;

  if(r != a && r != b) {
    r->matrixElements = NULL;
    r->header.matrixRows = r->header.matrixColumns = 0;
  }

  if(a->header.matrixColumns != n) {
    displayCalcErrorMessage(ERROR_MATRIX_MISMATCH, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "not a square matrix (%d" STD_CROSS "%d)",
              n, a->header.matrixColumns);
      moreInfoOnError("In function complex_matrix_linear_eqn:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return;
  }
  if(b->header.matrixRows != n || b->header.matrixColumns != 1) {
    displayCalcErrorMessage(ERROR_MATRIX_MISMATCH, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "not a column vector or size mismatch (%d" STD_CROSS "%d)",
              b->header.matrixRows, b->header.matrixColumns);
      moreInfoOnError("In function complex_matrix_linear_eqn:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return;
  }

  /* Everything is happy so far -- decompose */
  if((pivots = allocWp43s(TO_BLOCKS(sizeof(uint16_t) * n)))) {
    complex_LU_decomposition(a, &mat, pivots);
    if(lastErrorCode != ERROR_NONE) {
      freeWp43s(&pivots, TO_BLOCKS(sizeof(uint16_t) * n));
      return;
    }
    if(!mat.matrixElements) {
      freeWp43s(&pivots, TO_BLOCKS(sizeof(uint16_t) * n));
      displayCalcErrorMessage(ERROR_SINGULAR_MATRIX, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "attempt to LU-decompose a singular matrix");
        moreInfoOnError("In function complex_matrix_linear_eqn:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      return;
    }

    /* And solve */
    if(r == a)
      complexMatrixFree(r);
    if(r == b || complexMatrixInit(r, n, 1)) {
      if((cv = allocWp43s(n * REAL_SIZE * 2))) {
        if((bv = allocWp43s(n * REAL_SIZE * 2))) {
          for(i = 0; i < n; i++) {
            real34ToReal(VARIABLE_REAL34_DATA(&b->matrixElements[i]) , bv + i * 2    );
            real34ToReal(VARIABLE_IMAG34_DATA(&b->matrixElements[i]) , bv + i * 2 + 1);
            pivots[i] = i;
          }
          complex_matrix_pivoting_solve(&mat, bv, pivots, cv, &ctxtReal39);
          for(i = 0; i < n; i++) {
            realToReal34(cv + i * 2    , VARIABLE_REAL34_DATA(&r->matrixElements[i]));
            realToReal34(cv + i * 2 + 1, VARIABLE_IMAG34_DATA(&r->matrixElements[i]));
          }
          freeWp43s(bv, n * REAL_SIZE * 2);
        }
        else lastErrorCode = ERROR_RAM_FULL;
        freeWp43s(cv, n * REAL_SIZE * 2);
      }
      else lastErrorCode = ERROR_RAM_FULL;
    }
    else lastErrorCode = ERROR_RAM_FULL;
    freeWp43s(pivots, TO_BLOCKS(sizeof(uint16_t) * n));
  }
  else lastErrorCode = ERROR_RAM_FULL;
}
#endif

void complex_matrix_linear_eqn(const complex34Matrix_t *a, const complex34Matrix_t *b, complex34Matrix_t *r) {
  complex34Matrix_t inv_a;

  if(a->header.matrixColumns != a->header.matrixRows) {
    displayCalcErrorMessage(ERROR_MATRIX_MISMATCH, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "not a square matrix (%d" STD_CROSS "%d)",
              a->header.matrixRows, a->header.matrixColumns);
      moreInfoOnError("In function complex_matrix_linear_eqn:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return;
  }
  if(b->header.matrixRows != a->header.matrixRows || b->header.matrixColumns != 1) {
    displayCalcErrorMessage(ERROR_MATRIX_MISMATCH, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "not a column vector or size mismatch (%d" STD_CROSS "%d)",
              b->header.matrixRows, b->header.matrixColumns);
      moreInfoOnError("In function complex_matrix_linear_eqn:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return;
  }

  complex_matrix_inverse(a, &inv_a);
  if(lastErrorCode != ERROR_NONE) {
    return;
  }
  if(!inv_a.matrixElements) {
    displayCalcErrorMessage(ERROR_SINGULAR_MATRIX, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "attempt to invert a singular matrix");
      moreInfoOnError("In function complex_matrix_linear_eqn:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return;
  }

  multiplyComplexMatrices(&inv_a, b, r);
  complexMatrixFree(&inv_a);
}
#endif



/* Routines for calculating eigenpairs */

#ifndef TESTSUITE_BUILD
static void adjCpxMat(const real_t *x, uint16_t size, real_t *res) {
  int32_t i, j;
  for(i = 0; i < size; ++i) {
    for(j = 0; j < size; ++j) {
      realCopy(x + (i * size + j) * 2,     res + (j * size + i) * 2    );
      realCopy(x + (i * size + j) * 2 + 1, res + (j * size + i) * 2 + 1);
      realChangeSign(res + (j * size + i) * 2 + 1);
    }
  }
}

static void mulCpxMat(const real_t *y, const real_t *x, uint16_t size, real_t *res, realContext_t *realContext) {
  int32_t i, j, k;
  real_t *sumr, prodr;
  real_t *sumi, prodi;

  for(i = 0; i < size; ++i) {
    for(j = 0; j < size; ++j) {
      sumr = res + (i * size + j) * 2;
      sumi = sumr + 1;
      realCopy(const_0, sumr);   realCopy(const_0, sumi);
      realCopy(const_0, &prodr); realCopy(const_0, &prodi);
      for(k = 0; k < size; ++k) {
        if(realIsZero(y + (i * size + k) * 2 + 1) && realIsZero(x + (k * size + j) * 2 + 1)) {
          realMultiply(y + (i * size + k) * 2, x + (k * size + j) * 2, &prodr, realContext);
        }
        else {
          mulComplexComplex(y + (i * size + k) * 2, y + (i * size + k) * 2 + 1,
                            x + (k * size + j) * 2, x + (k * size + j) * 2 + 1,
                            &prodr, &prodi, realContext);
        }
        realAdd(sumr, &prodr, sumr, realContext);
        realAdd(sumi, &prodi, sumi, realContext);
      }
    }
  }
}

static void QR_decomposition_householder(const real_t *mat, uint16_t size, real_t *q, real_t *r, realContext_t *realContext) {
  uint32_t i, j, k;

  real_t *bulk;
  real_t *matq, *matr;

  real_t *v, *qq, *qt, *newMat, sum, m, t;

  // Allocate
  if((bulk = allocWp43s((size * size * 5 + size) * REAL_SIZE * 2))) {
    matr = bulk;
    matq = bulk + (size * size * 2);

    qq     = bulk + 2 * (size * size * 2);
    qt     = bulk + 3 * (size * size * 2);
    newMat = bulk + 4 * (size * size * 2);
    v      = bulk + 5 * (size * size * 2);

    // Copy mat to matr
    for(i = 0; i < size * size; i++) {
      realCopy(mat + i * 2,     matr + i * 2    );
      realCopy(mat + i * 2 + 1, matr + i * 2 + 1);
    }

    // Initialize Q
    for(i = 0; i < size * size; i++) {
      realCopy(const_0, matq + i * 2    );
      realCopy(const_0, matq + i * 2 + 1);
    }
    for(i = 0; i < size; i++) {
      realCopy(const_1, matq + (i * size + i) * 2);
    }

    // Calculate
    for(j = 0; j < (size - 1u); ++j) {
      // Column vector of submatrix
      realCopy(const_0, &sum);
      for(i = 0; i < (size - j); i++) {
        realCopy(matr + ((i + j) * size + j) * 2,     v + i * 2    );
        realCopy(matr + ((i + j) * size + j) * 2 + 1, v + i * 2 + 1);
        realFMA(v + i * 2,     v + i * 2,     &sum, &sum, realContext);
        realFMA(v + i * 2 + 1, v + i * 2 + 1, &sum, &sum, realContext);
      }
      realSquareRoot(&sum, &sum, realContext);

      // Calculate u = x - alpha e1
      if(realIsZero(v + 1)) {
        realSubtract(v, &sum, v, realContext);
      }
      else {
        realRectangularToPolar(v, v + 1, &m, &t, realContext);
        realPolarToRectangular(&sum, &t, &m, &t, realContext);
        realSubtract(v, &m, v, realContext);
        realAdd(v + 1, &t, v + 1, realContext);
      }

      // Euclidean norm
      realCopy(const_0, &sum);
      for(i = 0; i < (size - j); i++) {
        realFMA(v + i * 2,     v + i * 2,     &sum, &sum, realContext);
        realFMA(v + i * 2 + 1, v + i * 2 + 1, &sum, &sum, realContext);
      }
      realSquareRoot(&sum, &sum, realContext);

      // Calculate v = u / ||u||
      for(i = 0; i < (size - j); i++) {
        if(realIsZero(&sum)) {
          realCopy(v + i * 2,     &m);
          realCopy(v + i * 2 + 1, &t);
        }
        else if(realIsZero(v + i * 2 + 1)) {
          realDivide(v + i * 2, &sum, &m, realContext);
          realCopy(const_0, &t);
        }
        else {
          divComplexComplex(v + i * 2, v + i * 2 + 1, &sum, const_0, &m, &t, realContext);
        }
        realCopy(&m, v + i * 2    );
        realCopy(&t, v + i * 2 + 1);
      }

      // Initialize Q
      for(i = 0; i < size * size; i++) {
        realCopy(const_0, qq + i * 2    );
        realCopy(const_0, qq + i * 2 + 1);
      }
      for(i = 0; i < size; i++) {
        realCopy(const_1, qq + (i * size + i) * 2);
      }

      // Q -= 2vv*
      for(i = 0; i < (size - j); i++) {
        for(k = 0; k < (size - j); k++) {
          const uint32_t qe = (i + j) * size + k + j;
          realSubtract(const_0, (v + k * 2 + 1), &sum, realContext);
          if(realIsZero(v + i * 2 + 1) && realIsZero(&sum)) {
            realMultiply(v + i * 2, v + k * 2, &m, realContext);
            realCopy(const_0, &t);
          }
          else {
            mulComplexComplex((v + i * 2), (v + i * 2 + 1), (v + k * 2), &sum, &m, &t, realContext);
          }
          realMultiply(&m, const_2, &m, realContext);
          realMultiply(&t, const_2, &t, realContext);
          realSubtract(qq + qe * 2 ,    &m, qq + qe * 2,     realContext);
          realSubtract(qq + qe * 2 + 1, &t, qq + qe * 2 + 1, realContext);
        }
      }

      // Calculate R
      mulCpxMat(qq, matr, size, newMat, realContext);
      for(i = 0; i < size * size; i++) {
        realCopy(newMat + i * 2,     matr + i * 2    );
        realCopy(newMat + i * 2 + 1, matr + i * 2 + 1);
      }

      // Calculate Q
      adjCpxMat(qq, size, qt);
      mulCpxMat(matq, qt, size, newMat, realContext);
      for(i = 0; i < size * size; i++) {
        realCopy(newMat + i * 2,     matq + i * 2    );
        realCopy(newMat + i * 2 + 1, matq + i * 2 + 1);
      }
    }

    // Make sure R is triangular
    for(j = 0; j < (size - 1u); j++) {
      for(i = j + 1; i < size; i++) {
        realCopy(const_0, matr + (i * size + j) * 2    );
        realCopy(const_0, matr + (i * size + j) * 2 + 1);
      }
    }

    // Copy results
    for(i = 0; i < size * size; i++) {
      realCopy(matq + i * 2,     q + i * 2    );
      realCopy(matq + i * 2 + 1, q + i * 2 + 1);
      realCopy(matr + i * 2,     r + i * 2    );
      realCopy(matr + i * 2 + 1, r + i * 2 + 1);
    }

    // Cleanup
    freeWp43s(bulk, (size * size * 5 + size) * REAL_SIZE * 2);
  }
  else lastErrorCode = ERROR_RAM_FULL;
}
#endif // TESTSUITE_BUILD

void real_QR_decomposition(const real34Matrix_t *matrix, real34Matrix_t *q, real34Matrix_t *r) {
#ifndef TESTSUITE_BUILD
  if(matrix->header.matrixRows == matrix->header.matrixColumns) {
    real_t *mat, *matq, *matr;
    uint32_t i;

    // Allocate
    if((mat = allocWp43s(matrix->header.matrixRows * matrix->header.matrixColumns * REAL_SIZE * 2 * 3))) {
      matq = mat + matrix->header.matrixRows * matrix->header.matrixColumns * 2;
      matr = mat + matrix->header.matrixRows * matrix->header.matrixColumns * 2 * 2;

      // Convert real34 to real
      for(i = 0; i < matrix->header.matrixRows * matrix->header.matrixColumns; i++) {
        real34ToReal(&matrix->matrixElements[i], mat + i * 2);
        realCopy(const_0, mat + i * 2 + 1);
      }

      // Calculate
      QR_decomposition_householder(mat, matrix->header.matrixRows, matq, matr, &ctxtReal39);
      if(lastErrorCode == ERROR_NONE) {
        // Write back
        if(realMatrixInit(q, matrix->header.matrixRows, matrix->header.matrixRows)) {
          for(i = 0; i < matrix->header.matrixRows * matrix->header.matrixRows; i++) {
            realToReal34(matq + i * 2, &q->matrixElements[i]);
          }
          if(realMatrixInit(r, matrix->header.matrixRows, matrix->header.matrixRows)) {
            for(i = 0; i < matrix->header.matrixRows * matrix->header.matrixRows; i++) {
              realToReal34(matr + i * 2, &r->matrixElements[i]);
            }
          }
          else lastErrorCode = ERROR_RAM_FULL;
        }
        else lastErrorCode = ERROR_RAM_FULL;
      }
      else lastErrorCode = ERROR_RAM_FULL;

      // Cleanup
      freeWp43s(mat, matrix->header.matrixRows * matrix->header.matrixColumns * REAL_SIZE * 2 * 3);
    }
    else lastErrorCode = ERROR_RAM_FULL;
  }
#endif // TESTSUITE_BUILD
}

void complex_QR_decomposition(const complex34Matrix_t *matrix, complex34Matrix_t *q, complex34Matrix_t *r) {
#ifndef TESTSUITE_BUILD
  if(matrix->header.matrixRows == matrix->header.matrixColumns) {
    real_t *mat, *matq, *matr;
    uint32_t i;

    // Allocate
    if((mat = allocWp43s(matrix->header.matrixRows * matrix->header.matrixColumns * REAL_SIZE * 2 * 3))) {
      matq = mat + matrix->header.matrixRows * matrix->header.matrixColumns * 2;
      matr = mat + matrix->header.matrixRows * matrix->header.matrixColumns * 2 * 2;

      for(i = 0; i < matrix->header.matrixRows * matrix->header.matrixColumns; i++) {
        real34ToReal(VARIABLE_REAL34_DATA(&matrix->matrixElements[i]), mat + i * 2    );
        real34ToReal(VARIABLE_IMAG34_DATA(&matrix->matrixElements[i]), mat + i * 2 + 1);
      }

      // Calculate
      QR_decomposition_householder(mat, matrix->header.matrixRows, matq, matr, &ctxtReal39);

      // Write back
      if(complexMatrixInit(q, matrix->header.matrixRows, matrix->header.matrixRows)) {
        for(i = 0; i < matrix->header.matrixRows * matrix->header.matrixColumns; i++) {
          realToReal34(matq + i * 2,     VARIABLE_REAL34_DATA(&q->matrixElements[i]));
          realToReal34(matq + i * 2 + 1, VARIABLE_IMAG34_DATA(&q->matrixElements[i]));
        }
        if(complexMatrixInit(r, matrix->header.matrixRows, matrix->header.matrixRows)) {
          for(i = 0; i < matrix->header.matrixRows * matrix->header.matrixColumns; i++) {
            realToReal34(matr + i * 2,     VARIABLE_REAL34_DATA(&r->matrixElements[i]));
            realToReal34(matr + i * 2 + 1, VARIABLE_IMAG34_DATA(&r->matrixElements[i]));
          }
        }
        else lastErrorCode = ERROR_RAM_FULL;
      }
      else lastErrorCode = ERROR_RAM_FULL;

      // Cleanup
      freeWp43s(mat, matrix->header.matrixRows * matrix->header.matrixColumns * REAL_SIZE * 2 * 3);
    }
    else lastErrorCode = ERROR_RAM_FULL;
  }
#endif // TESTSUITE_BUILD
}

#ifndef TESTSUITE_BUILD
static void calculateEigenvalues22(const real_t *mat, uint16_t size, real_t *t1r, real_t *t1i, real_t *t2r, real_t *t2i, realContext_t *realContext) {
  // Calculate eigenvalue of 2x2-submatrix
  // Characteristic equation of A = [[a b] [c d]] : t^2 - trace(A) t +      det(A) = 0
  //                                                t^2 -  (a + d) t + (a d - b c) = 0
  //                                            t = ((a + d) ± √(a^2 + 2 a d + d^2 - 4 (a d - b c))) / 2
  //                                                ((a + d) ± √(a^2         + d^2 - 2 a d + 4 b c)) / 2
  const real_t *ar, *ai, *br, *bi, *cr, *ci, *dr, *di;
  real_t tmpR, tmpI, discrR, discrI;

  ar = mat + ((size - 2) * size + (size - 2)) * 2; ai = ar + 1;
  br = mat + ((size - 2) * size + (size - 1)) * 2; bi = br + 1;
  cr = mat + ((size - 1) * size + (size - 2)) * 2; ci = cr + 1;
  dr = mat + ((size - 1) * size + (size - 1)) * 2; di = dr + 1;

  // a^2
  if(realIsZero(ai))
    realMultiply(ar, ar, &discrR, realContext), realZero(&discrI);
  else
    mulComplexComplex(ar, ai, ar, ai, &discrR, &discrI, realContext);

  // d^2
  if(realIsZero(di))
    realMultiply(dr, dr, &tmpR, realContext), realZero(&tmpI);
  else
    mulComplexComplex(dr, di, dr, di, &tmpR, &tmpI, realContext);
  realAdd(&discrR, &tmpR, &discrR, realContext), realAdd(&discrI, &tmpI, &discrI, realContext);

  // -2ad
  if(realIsZero(ai) && realIsZero(di)) {
    realMultiply(ar, dr, &tmpR, realContext), realZero(&tmpI);
    realMultiply(&tmpR, const_2, &tmpR, realContext);
  }
  else {
    mulComplexComplex(ar, ai, dr, di, &tmpR, &tmpI, realContext);
    realMultiply(&tmpR, const_2, &tmpR, realContext);
    realMultiply(&tmpI, const_2, &tmpI, realContext);
  }
  realSubtract(&discrR, &tmpR, &discrR, realContext), realSubtract(&discrI, &tmpI, &discrI, realContext);

  // 4bc
  if(realIsZero(bi) && realIsZero(ci)) {
    realMultiply(br, cr, &tmpR, realContext), realZero(&tmpI);
    realMultiply(&tmpR, const_4, &tmpR, realContext);
  }
  else {
    mulComplexComplex(br, bi, cr, ci, &tmpR, &tmpI, realContext);
    realMultiply(&tmpR, const_4, &tmpR, realContext);
    realMultiply(&tmpI, const_4, &tmpI, realContext);
  }
  realAdd(&discrR, &tmpR, &discrR, realContext), realAdd(&discrI, &tmpI, &discrI, realContext);

  // sqrt
  if(realIsZero(&discrI) && !realIsNegative(&discrR)) {
    realSquareRoot(&discrR, t1r, realContext);
    realCopy(t1r, t2r); realChangeSign(t2r);
    realZero(t1i); realZero(t2i);
  }
  else if(realIsZero(&discrI)) {
    realCopy(&discrR, t1i); realSetPositiveSign(t1i);
    realSquareRoot(t1i, t1i, realContext);
    realCopy(t1i, t2i); realChangeSign(t2i);
    realZero(t1r); realZero(t2r);
  }
  else {
    realRectangularToPolar(&discrR, &discrI, t1r, t1i, realContext);
    realSquareRoot(t1r, t1r, realContext);
    realMultiply(t1i, const_1on2, t1i, realContext);
    realPolarToRectangular(t1r, t1i, t1r, t1i, realContext);
    realCopy(t1r, t2r); realChangeSign(t2r);
    realCopy(t1i, t2i); realChangeSign(t2i);
  }

  // +a +d /2
  realAdd(t1r, ar, t1r, realContext), realAdd(t1i, ai, t1i, realContext);
  realAdd(t1r, dr, t1r, realContext), realAdd(t1i, di, t1i, realContext);
  realAdd(t2r, ar, t2r, realContext), realAdd(t2i, ai, t2i, realContext);
  realAdd(t2r, dr, t2r, realContext), realAdd(t2i, di, t2i, realContext);
  realDivide(t1r, const_2, t1r, realContext), realDivide(t1i, const_2, t1i, realContext);
  realDivide(t2r, const_2, t2r, realContext), realDivide(t2i, const_2, t2i, realContext);
}

static void calculateQrShift(const real_t *mat, uint16_t size, real_t *re, real_t *im, realContext_t *realContext) {
  real_t t1r, t1i, t2r, t2i;
  real_t tmp, tmpR, tmpI;
  const real_t *dr, *di;

  dr = mat + ((size - 1) * size + (size - 1)) * 2; di = dr + 1;

  calculateEigenvalues22(mat, size, &t1r, &t1i, &t2r, &t2i, realContext);

  // Choose shift parameter
  realSubtract(&t1r, dr, &tmpR, realContext), realSubtract(&t1i, di, &tmpI, realContext);
  complexMagnitude(&tmpR, &tmpI, &tmpR, realContext);
  realSubtract(&t2r, dr, &tmp, realContext), realSubtract(&t2i, di, &tmpI, realContext);
  complexMagnitude(&tmp, &tmpI, &tmp, realContext);

  if(realCompareLessThan(&tmpR, &tmp)) {
    realCopy(&t1r, re); realCopy(&t1i, im);
  }
  else {
    realCopy(&t2r, re); realCopy(&t2i, im);
  }
}

static void sortEigenvalues(real_t *eig, uint16_t size, uint16_t begin_a, uint16_t begin_b, uint16_t end_b, realContext_t *realContext) {
  const uint16_t end_a = begin_b - 1;

  if(size < 2) { // ... trivial
    return;
  }
  else if(begin_a == end_b) { // ... trivial
    return;
  }
  else if(size == 2) { // simply compare
    complexMagnitude(eig,     eig + 1, eig + 2, realContext);
    complexMagnitude(eig + 6, eig + 7, eig + 4, realContext);
    if(realCompareLessThan(eig + 2, eig + 4)) {
      realCopy(eig,     eig + 2); realCopy(eig + 1, eig + 3);
      realCopy(eig + 6, eig    ); realCopy(eig + 7, eig + 1);
      realCopy(eig + 2, eig + 6); realCopy(eig + 1, eig + 7);
    }
  }
  else {
    uint16_t a = begin_a, b = begin_b;
    sortEigenvalues(eig, size, begin_a, (begin_a + end_a + 2) / 2, end_a, realContext);
    sortEigenvalues(eig, size, begin_b, (begin_b + end_b + 2) / 2, end_b, realContext);
    for(uint16_t i = begin_a; i <= end_b; i++) {
      complexMagnitude(eig + (i * size + i) * 2, eig + (i * size + i) * 2 + 1, eig + (i * size + (i + 1) % size) * 2, realContext);
    }
    for(uint16_t i = begin_a; i <= end_b; i++) {
      if(a > end_a) {
        realCopy(eig + (b * size + b) * 2,     eig + (i * size + (i + 2) % size) * 2    );
        realCopy(eig + (b * size + b) * 2 + 1, eig + (i * size + (i + 2) % size) * 2 + 1);
        ++b;
      }
      else if(b > end_b) {
        realCopy(eig + (a * size + a) * 2,     eig + (i * size + (i + 2) % size) * 2    );
        realCopy(eig + (a * size + a) * 2 + 1, eig + (i * size + (i + 2) % size) * 2 + 1);
        ++a;
      }
      else if(realCompareLessThan(eig + (a * size + (a + 1) % size) * 2, eig + (b * size + (b + 1) % size) * 2)) {
        realCopy(eig + (b * size + b) * 2,     eig + (i * size + (i + 2) % size) * 2    );
        realCopy(eig + (b * size + b) * 2 + 1, eig + (i * size + (i + 2) % size) * 2 + 1);
        ++b;
      }
      else {
        realCopy(eig + (a * size + a) * 2,     eig + (i * size + (i + 2) % size) * 2    );
        realCopy(eig + (a * size + a) * 2 + 1, eig + (i * size + (i + 2) % size) * 2 + 1);
        ++a;
      }
    }
    for(uint16_t i = begin_a; i <= end_b; i++) {
      realCopy(eig + (i * size + (i + 2) % size) * 2,     eig + (i * size + i) * 2    );
      realCopy(eig + (i * size + (i + 2) % size) * 2 + 1, eig + (i * size + i) * 2 + 1);
    }
  }
}
static void calculateEigenvalues(real_t *a, real_t *q, real_t *r, real_t *eig, uint16_t size, bool_t shifted, realContext_t *realContext) {
  real_t shiftRe, shiftIm;
  uint16_t i, j;
  bool_t converged;

  if(size == 2) {
    calculateEigenvalues22(a, size, eig, eig + 1, eig + 6, eig + 7, realContext);
  }
  else {
    while(true) {
      if(shifted) {
        calculateQrShift(a, size, &shiftRe, &shiftIm, realContext);
        for(i = 0; i < size; i++) {
          realSubtract(a + (i * size + i) * 2,     &shiftRe, a + (i * size + i) * 2,     realContext);
          realSubtract(a + (i * size + i) * 2 + 1, &shiftIm, a + (i * size + i) * 2 + 1, realContext);
        }
      }
      QR_decomposition_householder(a, size, q, r, realContext);
      mulCpxMat(r, q, size, eig, realContext);
      if(shifted) {
        for(i = 0; i < size; i++) {
          realAdd(a   + (i * size + i) * 2,     &shiftRe, a   + (i * size + i) * 2,     realContext);
          realAdd(a   + (i * size + i) * 2 + 1, &shiftIm, a   + (i * size + i) * 2 + 1, realContext);
          realAdd(eig + (i * size + i) * 2,     &shiftRe, eig + (i * size + i) * 2,     realContext);
          realAdd(eig + (i * size + i) * 2 + 1, &shiftIm, eig + (i * size + i) * 2 + 1, realContext);
        }
      }

      converged = true;
      for(i = 0; i < size; i++) {
        if(realIsNaN(eig + i * 2) || realIsNaN(eig + i * 2 + 1)) {
          for(j = 0; j < size * size; j++) {
            realCopy(a + j * 2,     eig + j * 2    );
            realCopy(a + j * 2 + 1, eig + j * 2 + 1);
          }
          converged = true;
          break;
        }
        else if(!WP34S_RelativeError(a + (i * size + i) * 2, eig + (i * size + i) * 2, const_1e_37, realContext) || !WP34S_RelativeError(a + (i * size + i) * 2 + 1, eig + (i * size + i) * 2 + 1, const_1e_37, realContext)) {
          converged = false;
        }
      }
      if(converged) {
        break;
      }
      else {
        for(i = 0; i < size * size; i++) {
          realCopy(eig + i * 2,     a + i * 2    );
          realCopy(eig + i * 2 + 1, a + i * 2 + 1);
        }
      }
    }
    shifted = false;
  }
  sortEigenvalues(eig, size, 0, (size + 1) / 2, size - 1, realContext);
}

static void calculateEigenvectors(const any34Matrix_t *matrix, bool_t isComplex, real_t *a, real_t *q, real_t *r, real_t *eig, realContext_t *realContext) {
  // Call after the eigenvalues are calculated!
  const uint16_t size = matrix->header.matrixRows;
  uint16_t i, j, k, l, mult, multIter, multTotal;
  bool_t isIndeterminate = false;
  bool_t tmpFlag = false;

  if(matrix->header.matrixRows == matrix->header.matrixColumns) {
    for(i = 0; i < size * size * 2; i++) realZero(r + i);
    for(k = 0; k < size; k++) {
      // Round to 34 digits
      realAdd(eig + (k * size + k) * 2,     const_0, eig + (k * size + k) * 2,     &ctxtReal34);
      realAdd(eig + (k * size + k) * 2 + 1, const_0, eig + (k * size + k) * 2 + 1, &ctxtReal34);
    }

    for(k = 0; k < size; k++) {
      // Restore the original matrix
      if(isComplex) {
        for(i = 0; i < size * size; i++) {
          real34ToReal(VARIABLE_REAL34_DATA(&matrix->complexMatrix.matrixElements[i]), a + i * 2    );
          real34ToReal(VARIABLE_IMAG34_DATA(&matrix->complexMatrix.matrixElements[i]), a + i * 2 + 1);
        }
      }
      else {
        for(i = 0; i < size * size; i++) {
          real34ToReal(&matrix->realMatrix.matrixElements[i], a + i * 2);
          realZero(a + i * 2 + 1);
        }
      }

      // Check for multiples
      mult = k; multIter = 0; multTotal = 0;
      for(i = 0; i < k; i++) {
        if(realCompareEqual(eig + (i * size + i) * 2, eig + (k * size + k) * 2) && realCompareEqual(eig + (i * size + i) * 2 + 1, eig + (k * size + k) * 2 + 1)) {
          mult = i;
          multIter++;
          multTotal++;
        }
      }
      for(i = k; i < size; i++) {
        if(realCompareEqual(eig + (i * size + i) * 2, eig + (k * size + k) * 2) && realCompareEqual(eig + (i * size + i) * 2 + 1, eig + (k * size + k) * 2 + 1)) {
          multTotal++;
        }
      }

      // Make the equation matrices
      for(i = 0; i < size; i++) {
        // Subtract an eigenvalue
        realSubtract(a + (i * size + i) * 2,     eig + (k * size + k) * 2,     a + (i * size + i) * 2,     realContext);
        realSubtract(a + (i * size + i) * 2 + 1, eig + (k * size + k) * 2 + 1, a + (i * size + i) * 2 + 1, realContext);
        realCopy(const_0, q + i * 2    );
        realCopy(const_0, q + i * 2 + 1);
        // Singular (a variant not mentioned)
        isIndeterminate = true;
        for(j = 0; j < size; j++) {
          if(!realIsZero(a + (j * size + i) * 2) || !realIsZero(a + (j * size + i) * 2 + 1))
            isIndeterminate = false;
        }
        if(isIndeterminate) {
          for(j = 0; j < size; j++) {
            realCopy(const_0, a + (i * size + j) * 2    );
            realCopy(const_0, a + (i * size + j) * 2 + 1);
          }
          realCopy(const_1, a + (i * size + i) * 2);
          multTotal--;
        }
      }
      if(mult == k) { // distinct
        for(j = 0; j < size; j++) {
          realCopy(const_0, a + ((size - 1) * size + j) * 2    );
          realCopy(const_0, a + ((size - 1) * size + j) * 2 + 1);
        }
        realCopy(const_1, a + ((size - 1) * size + (size - 1)) * 2);
        realCopy(const_1, q + (size - 1) * 2    );
        realCopy(const_0, q + (size - 1) * 2 + 1);
      }
      else if(multIter < multTotal) { // multiples
        i = size - 1;
        for(l = 1; l <= multIter + 1; ++l) {
          --i;
          isIndeterminate = true; tmpFlag = false;
          for(j = 0; j < size; j++) {
            if(isIndeterminate && !tmpFlag && realCompareEqual(a + (j * size + i) * 2, const_1) && realIsZero(a + (j * size + i) * 2 + 1))
              tmpFlag = true;
            else if(!realIsZero(a + (j * size + i) * 2) || !realIsZero(a + (j * size + i) * 2 + 1))
              isIndeterminate = false;
          }
          if(isIndeterminate && tmpFlag) --l;
        }
        for(j = 0; j < size; j++) {
          realCopy(const_0, a + (i * size + j) * 2    );
          realCopy(const_0, a + (i * size + j) * 2 + 1);
        }
        realCopy(const_1, a + (i * size + i) * 2);
        realCopy(const_1, q + (size - 1) * 2    );
        realCopy(const_0, q + (size - 1) * 2 + 1);
      }
      else { // orphan elements
        i = size - 1;
        for(l = 1; l <= multIter - multTotal + 1; ++l) {
          --i;
          isIndeterminate = true; tmpFlag = false;
          for(j = 0; j < size; j++) {
            if(isIndeterminate && !tmpFlag && realCompareEqual(a + (j * size + i) * 2, const_1) && realIsZero(a + (j * size + i) * 2 + 1))
              tmpFlag = true;
            else if(!realIsZero(a + (j * size + i) * 2) || !realIsZero(a + (j * size + i) * 2 + 1))
              isIndeterminate = false;
          }
          if(!isIndeterminate || !tmpFlag) --l;
        }
        for(j = 0; j < size; j++) {
          realCopy(const_0, a + ((size - 1) * size + j) * 2    );
          realCopy(const_0, a + ((size - 1) * size + j) * 2 + 1);
        }
        realCopy(const_1, a + ((size - 1) * size + (size - 1)) * 2);
        realCopy(const_1, q + i * 2    );
        realCopy(const_0, q + i * 2 + 1);
      }

      // Solve linear equations from the submatrix
      if(invCpxMat(a, size, &ctxtReal51)) {
        for(i = 0; i < size; ++i) {
          real_t sumr, sumi, prodr, prodi;
          realCopy(const_0, &sumr);  realCopy(const_0, &sumi);
          realCopy(const_0, &prodr); realCopy(const_0, &prodi);
          for(j = 0; j < size; ++j) {
            mulComplexComplex(a + (i * size + j) * 2, a + (i * size + j) * 2 + 1,
              q + j * 2, q + j * 2 + 1,
              &prodr, &prodi, realContext);
            realAdd(&sumr, &prodr, &sumr, realContext);
            realAdd(&sumi, &prodi, &sumi, realContext);
          }
          realCopy(&sumr, r + (i * size + k) * 2    );
          realCopy(&sumi, r + (i * size + k) * 2 + 1);
        }
      }
    }
  }
}
#endif // TESTSUITE_BUILD

void realEigenvalues(const real34Matrix_t *matrix, real34Matrix_t *res, real34Matrix_t *ires) {
#ifndef TESTSUITE_BUILD
  const uint16_t size = matrix->header.matrixRows;
  real_t *bulk, *a, *q, *r, *eig;
  uint16_t i;
  bool_t isComplex;
  bool_t shifted = true;

  if(matrix->header.matrixRows == matrix->header.matrixColumns) {
    if((bulk = allocWp43s(size * size * REAL_SIZE * 2 * 4))) {
      a   = bulk;
      q   = bulk + size * size * 2;
      r   = bulk + size * size * 2 * 2;
      eig = bulk + size * size * 2 * 3;

      // Convert real34 to real
      for(i = 0; i < size * size; i++) {
        real34ToReal(&matrix->matrixElements[i], a + i * 2);
        realZero(a + i * 2 + 1);
      }

      // Calculate
      calculateEigenvalues(a, q, r, eig, size, shifted, &ctxtReal75);
      shifted = false;

      // Check imaginary part (mutually conjugate complex roots are possible in real quadratic equations)
      isComplex = false;
      for(i = 0; i < size; i++) {
        if(!realIsZero(eig + (i * size + i) * 2 + 1)) {
          isComplex = true;
          break;
        }
      }

      // Write back
      if(matrix == res || realMatrixInit(res, size, size)) {
        for(i = 0; i < size; i++) {
          realToReal34(eig + (i * size + i) * 2, &res->matrixElements[i * size + i]);
        }
        if(isComplex && (ires != NULL)) {
          if(matrix == ires || res == ires || realMatrixInit(ires, size, size)) {
            for(i = 0; i < size; i++) {
              realToReal34(eig + (i * size + i) * 2 + 1, &ires->matrixElements[i * size + i]);
            }
          }
          else lastErrorCode = ERROR_RAM_FULL;
        }
      }
      else lastErrorCode = ERROR_RAM_FULL;

      freeWp43s(bulk, size * size * REAL_SIZE * 2 * 4);
    }
    else lastErrorCode = ERROR_RAM_FULL;
  }
#endif // TESTSUITE_BUILD
}

void complexEigenvalues(const complex34Matrix_t *matrix, complex34Matrix_t *res) {
#ifndef TESTSUITE_BUILD
  const uint16_t size = matrix->header.matrixRows;
  real_t *bulk, *a, *q, *r, *eig;
  uint16_t i;
  bool_t shifted = true;

  if(matrix->header.matrixRows == matrix->header.matrixColumns) {
    if((bulk = allocWp43s(size * size * REAL_SIZE * 2 * 4))) {
      a   = bulk;
      q   = bulk + size * size * 2;
      r   = bulk + size * size * 2 * 2;
      eig = bulk + size * size * 2 * 3;

      // Convert real34 to real
      for(i = 0; i < size * size; i++) {
        real34ToReal(VARIABLE_REAL34_DATA(&matrix->matrixElements[i]), a + i * 2    );
        real34ToReal(VARIABLE_IMAG34_DATA(&matrix->matrixElements[i]), a + i * 2 + 1);
      }

      // Calculate
      calculateEigenvalues(a, q, r, eig, size, shifted, &ctxtReal75);
      shifted = false;

      // Write back
      if(matrix == res || complexMatrixInit(res, size, size)) {
        for(i = 0; i < size; i++) {
          realToReal34(eig + (i * size + i) * 2,     VARIABLE_REAL34_DATA(&res->matrixElements[i * size + i]));
          realToReal34(eig + (i * size + i) * 2 + 1, VARIABLE_IMAG34_DATA(&res->matrixElements[i * size + i]));
        }
      }
      else lastErrorCode = ERROR_RAM_FULL;

      freeWp43s(bulk, size * size * REAL_SIZE * 2 * 4);
    }
    else lastErrorCode = ERROR_RAM_FULL;
  }
#endif // TESTSUITE_BUILD
}

void realEigenvectors(const real34Matrix_t *matrix, real34Matrix_t *res, real34Matrix_t *ires) {
#ifndef TESTSUITE_BUILD
  const uint16_t size = matrix->header.matrixRows;
  real_t *bulk, *a, *q, *r, *eig;
  uint16_t i, j;
  bool_t isComplex;
  bool_t shifted = true;

  if(matrix->header.matrixRows == matrix->header.matrixColumns) {
    if((bulk = allocWp43s(size * size * REAL_SIZE * 2 * 4))) {
      a   = bulk;
      q   = bulk + size * size * 2;
      r   = bulk + size * size * 2 * 2;
      eig = bulk + size * size * 2 * 3;

      // Convert real34 to real
      for(i = 0; i < size * size; i++) {
        real34ToReal(&matrix->matrixElements[i], a + i * 2);
        realZero(a + i * 2 + 1);
      }

      // Calculate eigenvalues
      calculateEigenvalues(a, q, r, eig, size, shifted, &ctxtReal75);
      shifted = false;
      calculateEigenvectors((any34Matrix_t *)matrix, false, a, q, r, eig, &ctxtReal75);

      // Check imaginary part (mutually conjugate complex roots are possible in real quadratic equations)
      isComplex = false;
      for(i = 0; i < size * size; i++) {
        if(!realIsZero(r + i * 2 + 1)) {
          isComplex = true;
          break;
        }
      }

      // Normalize
      for(j = 0; j < size; j++) {
        real_t sum;
        realZero(&sum);
        for(i = 0; i < size; i++) {
          realFMA(r + (i * size + j) * 2,     r + (i * size + j) * 2,     &sum, &sum, &ctxtReal75);
          realFMA(r + (i * size + j) * 2 + 1, r + (i * size + j) * 2 + 1, &sum, &sum, &ctxtReal75);
        }
        realSquareRoot(&sum, &sum, &ctxtReal75);
        if(!realIsZero(&sum) && !realIsSpecial(&sum)) {
          for(i = 0; i < size; i++) {
            realDivide(r + (i * size + j) * 2,     &sum, r + (i * size + j) * 2,     &ctxtReal75);
            realDivide(r + (i * size + j) * 2 + 1, &sum, r + (i * size + j) * 2 + 1, &ctxtReal75);
          }
        }
      }

      // Write back
      if(matrix == res || realMatrixInit(res, size, size)) {
        for(i = 0; i < size * size; i++) {
          realToReal34(r + i * 2, &res->matrixElements[i]);
        }
        if(isComplex && (ires != NULL)) {
          if(matrix == ires || res == ires || realMatrixInit(ires, size, size)) {
            for(i = 0; i < size * size; i++) {
              realToReal34(r + i * 2 + 1, &ires->matrixElements[i]);
            }
          }
          else lastErrorCode = ERROR_RAM_FULL;
        }
      }
      else lastErrorCode = ERROR_RAM_FULL;

      freeWp43s(bulk, size * size * REAL_SIZE * 2 * 4);
    }
    else lastErrorCode = ERROR_RAM_FULL;
  }
#endif // TESTSUITE_BUILD
}

void complexEigenvectors(const complex34Matrix_t *matrix, complex34Matrix_t *res) {
#ifndef TESTSUITE_BUILD
  const uint16_t size = matrix->header.matrixRows;
  real_t *bulk, *a, *q, *r, *eig;
  uint16_t i;
  bool_t shifted = true;

  if(matrix->header.matrixRows == matrix->header.matrixColumns) {
    if((bulk = allocWp43s(size * size * REAL_SIZE * 2 * 4))) {
      a   = bulk;
      q   = bulk + size * size * 2;
      r   = bulk + size * size * 2 * 2;
      eig = bulk + size * size * 2 * 3;

      // Convert real34 to real
      for(i = 0; i < size * size; i++) {
        real34ToReal(VARIABLE_REAL34_DATA(&matrix->matrixElements[i]), a + i * 2    );
        real34ToReal(VARIABLE_IMAG34_DATA(&matrix->matrixElements[i]), a + i * 2 + 1);
        realZero(a + i * 2 + 1);
      }

      // Calculate eigenvalues
      calculateEigenvalues(a, q, r, eig, size, shifted, &ctxtReal75);
      shifted = false;
      calculateEigenvectors((any34Matrix_t *)matrix, false, a, q, r, eig, &ctxtReal75);

      // Write back
      if(matrix == res || complexMatrixInit(res, size, size)) {
        for(i = 0; i < size * size; i++) {
          realToReal34(r + i * 2,     VARIABLE_REAL34_DATA(&res->matrixElements[i]));
          realToReal34(r + i * 2 + 1, VARIABLE_IMAG34_DATA(&res->matrixElements[i]));
        }
      }
      else lastErrorCode = ERROR_RAM_FULL;

      freeWp43s(bulk, size * size * REAL_SIZE * 2 * 4);
    }
    else lastErrorCode = ERROR_RAM_FULL;
  }
#endif // TESTSUITE_BUILD
}


/* Elementwise function call */
void elementwiseRema(void (*f)(void)) {
#ifndef TESTSUITE_BUILD
  real34Matrix_t x;
  const bool_t cpxres = getSystemFlag(FLAG_CPXRES);

  clearSystemFlag(FLAG_CPXRES);
  convertReal34MatrixRegisterToReal34Matrix(REGISTER_X, &x);

  for(int i = 0; i < x.header.matrixRows * x.header.matrixColumns; ++i) {
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    real34Copy(&x.matrixElements[i], REGISTER_REAL34_DATA(REGISTER_X));
    f();
    if(getRegisterDataType(REGISTER_X) == dtLongInteger)
      convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &x.matrixElements[i]);
  }

  convertReal34MatrixToReal34MatrixRegister(&x, REGISTER_X);
  if(cpxres) setSystemFlag(FLAG_CPXRES);

  realMatrixFree(&x);
#endif // TESTSUITE_BUILD
}

void elementwiseRema_UInt16(void (*f)(uint16_t), uint16_t param) {
#ifndef TESTSUITE_BUILD
  real34Matrix_t x;
  const bool_t cpxres = getSystemFlag(FLAG_CPXRES);

  clearSystemFlag(FLAG_CPXRES);
  convertReal34MatrixRegisterToReal34Matrix(REGISTER_X, &x);

  for(int i = 0; i < x.header.matrixRows * x.header.matrixColumns; ++i) {
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    real34Copy(&x.matrixElements[i], REGISTER_REAL34_DATA(REGISTER_X));
    f(param);
    if(getRegisterDataType(REGISTER_X) == dtLongInteger)
      convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &x.matrixElements[i]);
  }

  convertReal34MatrixToReal34MatrixRegister(&x, REGISTER_X);
  if(cpxres) setSystemFlag(FLAG_CPXRES);

  realMatrixFree(&x);
#endif // TESTSUITE_BUILD
}

void elementwiseRemaLonI(void (*f)(void)) {
#ifndef TESTSUITE_BUILD
  real34Matrix_t y;
  longInteger_t x;
  const bool_t cpxres = getSystemFlag(FLAG_CPXRES);

  clearSystemFlag(FLAG_CPXRES);
  convertReal34MatrixRegisterToReal34Matrix(REGISTER_Y, &y);
  convertLongIntegerRegisterToLongInteger(REGISTER_X, x);

  for(int i = 0; i < y.header.matrixRows * y.header.matrixColumns; ++i) {
    reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);
    real34Copy(&y.matrixElements[i], REGISTER_REAL34_DATA(REGISTER_Y));
    convertLongIntegerToLongIntegerRegister(x, REGISTER_X);
    f();
    if(getRegisterDataType(REGISTER_X) == dtLongInteger)
      convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &y.matrixElements[i]);
  }

  convertReal34MatrixToReal34MatrixRegister(&y, REGISTER_X);
  if(cpxres) setSystemFlag(FLAG_CPXRES);

  longIntegerFree(x);
  realMatrixFree(&y);
#endif // TESTSUITE_BUILD
}

void elementwiseRemaReal(void (*f)(void)) {
#ifndef TESTSUITE_BUILD
  real34Matrix_t y;
  real34_t x;
  const bool_t cpxres = getSystemFlag(FLAG_CPXRES);

  clearSystemFlag(FLAG_CPXRES);
  convertReal34MatrixRegisterToReal34Matrix(REGISTER_Y, &y);
  real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &x);

  for(int i = 0; i < y.header.matrixRows * y.header.matrixColumns; ++i) {
    reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    real34Copy(&y.matrixElements[i], REGISTER_REAL34_DATA(REGISTER_Y));
    real34Copy(&x, REGISTER_REAL34_DATA(REGISTER_X));
    f();
    if(getRegisterDataType(REGISTER_X) == dtLongInteger)
      convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &y.matrixElements[i]);
  }

  convertReal34MatrixToReal34MatrixRegister(&y, REGISTER_X);
  if(cpxres) setSystemFlag(FLAG_CPXRES);

  realMatrixFree(&y);
#endif // TESTSUITE_BUILD
}

void elementwiseRemaShoI(void (*f)(void)) {
#ifndef TESTSUITE_BUILD
  real34Matrix_t y;
  uint64_t x; uint32_t base; int16_t sign;
  const bool_t cpxres = getSystemFlag(FLAG_CPXRES);

  clearSystemFlag(FLAG_CPXRES);
  convertReal34MatrixRegisterToReal34Matrix(REGISTER_Y, &y);
  convertShortIntegerRegisterToUInt64(REGISTER_X, &sign, &x);
  base = getRegisterShortIntegerBase(REGISTER_X);

  for(int i = 0; i < y.header.matrixRows * y.header.matrixColumns; ++i) {
    reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);
    real34Copy(&y.matrixElements[i], REGISTER_REAL34_DATA(REGISTER_Y));
    convertUInt64ToShortIntegerRegister(sign, x, base, REGISTER_X);
    f();
    if(getRegisterDataType(REGISTER_X) == dtLongInteger)
      convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &y.matrixElements[i]);
  }

  convertReal34MatrixToReal34MatrixRegister(&y, REGISTER_X);
  if(cpxres) setSystemFlag(FLAG_CPXRES);

  realMatrixFree(&y);
#endif // TESTSUITE_BUILD
}



void elementwiseCxma(void (*f)(void)) {
#ifndef TESTSUITE_BUILD
  complex34Matrix_t x;
  const bool_t cpxres = getSystemFlag(FLAG_CPXRES);

  clearSystemFlag(FLAG_CPXRES);
  convertComplex34MatrixRegisterToComplex34Matrix(REGISTER_X, &x);

  for(int i = 0; i < x.header.matrixRows * x.header.matrixColumns; ++i) {
    reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
    complex34Copy(&x.matrixElements[i], REGISTER_COMPLEX34_DATA(REGISTER_X));
    f();
    if(getRegisterDataType(REGISTER_X) == dtLongInteger)
      convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    if(getRegisterDataType(REGISTER_X) == dtLongInteger || getRegisterDataType(REGISTER_X) == dtReal34) {
      real34_t val;
      real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &val);
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
      real34Copy(&val, REGISTER_REAL34_DATA(REGISTER_X));
      real34Zero(REGISTER_IMAG34_DATA(REGISTER_X));
    }
    complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_X), &x.matrixElements[i]);
  }

  convertComplex34MatrixToComplex34MatrixRegister(&x, REGISTER_X);
  if(cpxres) setSystemFlag(FLAG_CPXRES);

  complexMatrixFree(&x);
#endif // TESTSUITE_BUILD
}

void elementwiseCxma_UInt16(void (*f)(uint16_t), uint16_t param) {
#ifndef TESTSUITE_BUILD
  complex34Matrix_t x;
  const bool_t cpxres = getSystemFlag(FLAG_CPXRES);

  clearSystemFlag(FLAG_CPXRES);
  convertComplex34MatrixRegisterToComplex34Matrix(REGISTER_X, &x);

  for(int i = 0; i < x.header.matrixRows * x.header.matrixColumns; ++i) {
    reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
    complex34Copy(&x.matrixElements[i], REGISTER_COMPLEX34_DATA(REGISTER_X));
    f(param);
    if(getRegisterDataType(REGISTER_X) == dtLongInteger)
      convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    if(getRegisterDataType(REGISTER_X) == dtLongInteger || getRegisterDataType(REGISTER_X) == dtReal34) {
      real34_t val;
      real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &val);
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
      real34Copy(&val, REGISTER_REAL34_DATA(REGISTER_X));
      real34Zero(REGISTER_IMAG34_DATA(REGISTER_X));
    }
    complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_X), &x.matrixElements[i]);
  }

  convertComplex34MatrixToComplex34MatrixRegister(&x, REGISTER_X);
  if(cpxres) setSystemFlag(FLAG_CPXRES);

  complexMatrixFree(&x);
#endif // TESTSUITE_BUILD
}

void elementwiseCxmaLonI(void (*f)(void)) {
#ifndef TESTSUITE_BUILD
  complex34Matrix_t y;
  longInteger_t x;
  const bool_t cpxres = getSystemFlag(FLAG_CPXRES);

  clearSystemFlag(FLAG_CPXRES);
  convertComplex34MatrixRegisterToComplex34Matrix(REGISTER_Y, &y);
  convertLongIntegerRegisterToLongInteger(REGISTER_X, x);

  for(int i = 0; i < y.header.matrixRows * y.header.matrixColumns; ++i) {
    reallocateRegister(REGISTER_Y, dtComplex34, COMPLEX34_SIZE, amNone);
    complex34Copy(&y.matrixElements[i], REGISTER_COMPLEX34_DATA(REGISTER_Y));
    convertLongIntegerToLongIntegerRegister(x, REGISTER_X);
    f();
    if(getRegisterDataType(REGISTER_X) == dtLongInteger)
      convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    if(getRegisterDataType(REGISTER_X) == dtLongInteger || getRegisterDataType(REGISTER_X) == dtReal34) {
      real34_t val;
      real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &val);
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
      real34Copy(&val, REGISTER_REAL34_DATA(REGISTER_X));
      real34Zero(REGISTER_IMAG34_DATA(REGISTER_X));
    }
    complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_X), &y.matrixElements[i]);
  }

  convertComplex34MatrixToComplex34MatrixRegister(&y, REGISTER_X);
  if(cpxres) setSystemFlag(FLAG_CPXRES);

  longIntegerFree(x);
  complexMatrixFree(&y);
#endif // TESTSUITE_BUILD
}

void elementwiseCxmaReal(void (*f)(void)) {
#ifndef TESTSUITE_BUILD
  complex34Matrix_t y;
  real34_t x;
  const bool_t cpxres = getSystemFlag(FLAG_CPXRES);

  clearSystemFlag(FLAG_CPXRES);
  convertComplex34MatrixRegisterToComplex34Matrix(REGISTER_Y, &y);
  real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &x);

  for(int i = 0; i < y.header.matrixRows * y.header.matrixColumns; ++i) {
    reallocateRegister(REGISTER_Y, dtComplex34, COMPLEX34_SIZE, amNone);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    complex34Copy(&y.matrixElements[i], REGISTER_COMPLEX34_DATA(REGISTER_Y));
    real34Copy(&x, REGISTER_REAL34_DATA(REGISTER_X));
    f();
    if(getRegisterDataType(REGISTER_X) == dtLongInteger)
      convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    if(getRegisterDataType(REGISTER_X) == dtLongInteger || getRegisterDataType(REGISTER_X) == dtReal34) {
      real34_t val;
      real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &val);
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
      real34Copy(&val, REGISTER_REAL34_DATA(REGISTER_X));
      real34Zero(REGISTER_IMAG34_DATA(REGISTER_X));
    }
    complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_X), &y.matrixElements[i]);
  }

  convertComplex34MatrixToComplex34MatrixRegister(&y, REGISTER_X);
  if(cpxres) setSystemFlag(FLAG_CPXRES);

  complexMatrixFree(&y);
#endif // TESTSUITE_BUILD
}

void elementwiseCxmaShoI(void (*f)(void)) {
#ifndef TESTSUITE_BUILD
  complex34Matrix_t y;
  uint64_t x; uint32_t base; int16_t sign;
  const bool_t cpxres = getSystemFlag(FLAG_CPXRES);

  clearSystemFlag(FLAG_CPXRES);
  convertComplex34MatrixRegisterToComplex34Matrix(REGISTER_Y, &y);
  convertShortIntegerRegisterToUInt64(REGISTER_X, &sign, &x);
  base = getRegisterShortIntegerBase(REGISTER_X);

  for(int i = 0; i < y.header.matrixRows * y.header.matrixColumns; ++i) {
    reallocateRegister(REGISTER_Y, dtComplex34, COMPLEX34_SIZE, amNone);
    complex34Copy(&y.matrixElements[i], REGISTER_COMPLEX34_DATA(REGISTER_Y));
    convertUInt64ToShortIntegerRegister(sign, x, base, REGISTER_X);
    f();
    if(getRegisterDataType(REGISTER_X) == dtLongInteger)
      convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    if(getRegisterDataType(REGISTER_X) == dtLongInteger || getRegisterDataType(REGISTER_X) == dtReal34) {
      real34_t val;
      real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &val);
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
      real34Copy(&val, REGISTER_REAL34_DATA(REGISTER_X));
      real34Zero(REGISTER_IMAG34_DATA(REGISTER_X));
    }
    complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_X), &y.matrixElements[i]);
  }

  convertComplex34MatrixToComplex34MatrixRegister(&y, REGISTER_X);
  if(cpxres) setSystemFlag(FLAG_CPXRES);

  complexMatrixFree(&y);
#endif // TESTSUITE_BUILD
}

void elementwiseCxmaCplx(void (*f)(void)) {
#ifndef TESTSUITE_BUILD
  complex34Matrix_t y;
  complex34_t x;
  const bool_t cpxres = getSystemFlag(FLAG_CPXRES);

  clearSystemFlag(FLAG_CPXRES);
  convertComplex34MatrixRegisterToComplex34Matrix(REGISTER_Y, &y);
  complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_X), &x);

  for(int i = 0; i < y.header.matrixRows * y.header.matrixColumns; ++i) {
    reallocateRegister(REGISTER_Y, dtComplex34, COMPLEX34_SIZE, amNone);
    reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
    complex34Copy(&y.matrixElements[i], REGISTER_COMPLEX34_DATA(REGISTER_Y));
    complex34Copy(&x, REGISTER_COMPLEX34_DATA(REGISTER_X));
    f();
    if(getRegisterDataType(REGISTER_X) == dtLongInteger)
      convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    if(getRegisterDataType(REGISTER_X) == dtLongInteger || getRegisterDataType(REGISTER_X) == dtReal34) {
      real34_t val;
      real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &val);
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
      real34Copy(&val, REGISTER_REAL34_DATA(REGISTER_X));
      real34Zero(REGISTER_IMAG34_DATA(REGISTER_X));
    }
    complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_X), &y.matrixElements[i]);
  }

  convertComplex34MatrixToComplex34MatrixRegister(&y, REGISTER_X);
  if(cpxres) setSystemFlag(FLAG_CPXRES);

  complexMatrixFree(&y);
#endif // TESTSUITE_BUILD
}



#ifndef TESTSUITE_BUILD
void callByIndexedMatrix(bool_t (*real_f)(real34Matrix_t *), bool_t (*complex_f)(complex34Matrix_t *)) {
  const int16_t i = getIRegisterAsInt(true);
  const int16_t j = getJRegisterAsInt(true);

  if(matrixIndex == INVALID_VARIABLE || !regInRange(matrixIndex)) {
    displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "Cannot execute, destination register is out of range: %d", matrixIndex);
      moreInfoOnError("In function callByIndexedMatrix:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
  else if(getRegisterDataType(matrixIndex) == dtReal34Matrix) {
    real34Matrix_t mat;
    convertReal34MatrixRegisterToReal34Matrix(matrixIndex, &mat);
    if(i < 0 || i >= mat.header.matrixRows || j < 0 || j >= mat.header.matrixColumns) {
      displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "Cannot execute: element (%" PRId16 ", %" PRId16 ") out of range", (int16_t)(i + 1), (int16_t)(j + 1));
        moreInfoOnError("In function callByIndexedMatrix:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
    else {
      if(real_f(&mat)) convertReal34MatrixToReal34MatrixRegister(&mat, matrixIndex);
    }
    realMatrixFree(&mat);
  }
  else if(getRegisterDataType(matrixIndex) == dtComplex34Matrix) {
    complex34Matrix_t mat;
    convertComplex34MatrixRegisterToComplex34Matrix(matrixIndex, &mat);
    if(i < 0 || i >= mat.header.matrixRows || j < 0 || j >= mat.header.matrixColumns) {
      displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "Cannot execute: element (%" PRId16 ", %" PRId16 ") out of range", (int16_t)(i + 1), (int16_t)(j + 1));
        moreInfoOnError("In function callByIndexedMatrix:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
    else {
      if(complex_f(&mat)) convertComplex34MatrixToComplex34MatrixRegister(&mat, matrixIndex);
    }
    complexMatrixFree(&mat);
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "Cannot execute: something other than a matrix is indexed %s", getRegisterDataTypeName(REGISTER_X, true, false));
      moreInfoOnError("In function callByIndexedMatrix:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
}
#endif // TESTSUITE_BUILD
