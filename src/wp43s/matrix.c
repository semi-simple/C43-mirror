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
 * \file matrix.c
 ***********************************************/

#include "wp43s.h"

#ifndef TESTSUITE_BUILD
real34Matrix_t        openMatrixMIMPointer;
bool_t                matEditMode;

/********************************************//**
 * \brief creates new Matrix of size y->m x x ->n
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnNewMatrix(uint16_t unusedParamButMandatory) {
  uint32_t rows, cols;
  longInteger_t tmp_lgInt;
  real34Matrix_t matrix;

  //Get Size from REGISTER_X and REGISTER_Y
  if(((getRegisterDataType(REGISTER_X) != dtLongInteger) && (getRegisterDataType(REGISTER_X) != dtReal34)) ||
    ((getRegisterDataType(REGISTER_Y) != dtLongInteger) && (getRegisterDataType(REGISTER_Y) != dtReal34))) {
      displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "invalid data type %s and %s", getRegisterDataTypeName(REGISTER_Y, true, false), getRegisterDataTypeName(REGISTER_X, true, false));
        moreInfoOnError("In function fnNewMatrix:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      return;
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
      moreInfoOnError("In function fnNewMatrix:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return;
  }
  longIntegerToUInt(tmp_lgInt, cols);

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
      moreInfoOnError("In function fnNewMatrix:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return;
  }
  longIntegerToUInt(tmp_lgInt, rows);

  longIntegerFree(tmp_lgInt);

  //Initialize Memory for Matrix
  realMatrixInit(&matrix, rows, cols);

  //Drop X_Register and Y_Register
  fnDropY(NOPARAM);
  convertReal34MatrixToReal34MatrixRegister(&matrix, REGISTER_X);
  setSystemFlag(FLAG_ASLIFT);

  realMatrixFree(&matrix);
}

/********************************************//**
 * \brief Opens the Matrix Editor
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnEditMatrix(uint16_t unusedParamButMandatory) {
  if((getRegisterDataType(REGISTER_X) == dtReal34Matrix) || (getRegisterDataType(REGISTER_X) == dtComplex34Matrix)) {
    calcMode = CM_MIM;

    getMatrixFromRegister(REGISTER_X);

    showSoftmenu(-MNU_M_EDIT);
    setIRegisterAsInt(true, 0);
    setJRegisterAsInt(true, 0);
    aimBuffer[0] = 0;
    nimBufferDisplay[0] = 0;
    showMatrixEditor();
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #ifdef PC_BUILD
    sprintf(errorMessage, "DataType %" PRIu32, getRegisterDataType(REGISTER_X));
    moreInfoOnError("In function fnEditMatrix:", errorMessage, "is not a matrix.", "");
    #endif
  }
}



/********************************************//**
 * \brief Transpose matrix
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnTranspose(uint16_t unusedButMandatoryParameter) {
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtReal34Matrix) {
    real34Matrix_t x, res;

    convertReal34MatrixRegisterToReal34Matrix(REGISTER_X, &x);
    transposeRealMatrix(&x, &res);
    convertReal34MatrixToReal34MatrixRegister(&res, REGISTER_X);
    realMatrixFree(&res);
    realMatrixFree(&x);
  }
  else if(getRegisterDataType(REGISTER_X) == dtComplex34Matrix) {
    fnToBeCoded();
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #ifdef PC_BUILD
    sprintf(errorMessage, "DataType %" PRIu32, getRegisterDataType(REGISTER_X));
    moreInfoOnError("In function fnTranspose:", errorMessage, "is not a matrix.", "");
    #endif
  }

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}




/********************************************//**
 * \brief LU decomposition
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnLuDecomposition(uint16_t unusedParamButMandatory) {
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtReal34Matrix) {
    real34Matrix_t x, res;
    uint16_t *p;

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
      p = allocWp43s(x.header.matrixRows * sizeof(uint16_t));
      WP34S_LU_decomposition(&x, &res, p);
      if(res.matrixElements) {
        realMatrixFree(&x);
        realMatrixIdentity(&x, res.header.matrixColumns);
        for(uint16_t i = 0; i < res.header.matrixColumns; ++i) {
          realMatrixSwapRows(&x, &x, i, p[i]);
        }
        liftStack();
        convertReal34MatrixToReal34MatrixRegister(&res, REGISTER_X);
        convertReal34MatrixToReal34MatrixRegister(&x, REGISTER_Y);
        realMatrixFree(&res);
        setSystemFlag(FLAG_ASLIFT);
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

    realMatrixFree(&x);
  }
  else if(getRegisterDataType(REGISTER_X) == dtComplex34Matrix) {
    fnToBeCoded();
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #ifdef PC_BUILD
    sprintf(errorMessage, "DataType %" PRIu32, getRegisterDataType(REGISTER_X));
    moreInfoOnError("In function fnLuDecomposition:", errorMessage, "is not a matrix.", "");
    #endif
  }

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



/********************************************//**
 * \brief Determinant
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnDeterminant(uint16_t unusedParamButMandatory) {
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtReal34Matrix) {
    real34Matrix_t x;
    real34_t res;

    convertReal34MatrixRegisterToReal34Matrix(REGISTER_X, &x);

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
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
      real34Copy(&res, REGISTER_REAL34_DATA(REGISTER_X));
    }

    realMatrixFree(&x);
  }
  else if(getRegisterDataType(REGISTER_X) == dtComplex34Matrix) {
    fnToBeCoded();
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #ifdef PC_BUILD
    sprintf(errorMessage, "DataType %" PRIu32, getRegisterDataType(REGISTER_X));
    moreInfoOnError("In function fnLuDecomposition:", errorMessage, "is not a matrix.", "");
    #endif
  }

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



/********************************************//**
 * \brief Initialize a real matrix
 *
 * \param[out] matrix real34Matrix_t*
 * \param[in] rows uint16_t
 * \param[in] cols uint16_t
 * \return void
 ***********************************************/
void realMatrixInit(real34Matrix_t *matrix, uint16_t rows, uint16_t cols) {
  //Allocate Memory for Matrix
  matrix->matrixElements = allocWp43s(TO_BLOCKS((rows * cols) * sizeof(real34_t)));

  matrix->header.matrixColumns = cols;
  matrix->header.matrixRows = rows;

  //Initialize with 0.
  for(uint32_t i = 0; i < rows * cols; i++) {
    real34Copy(const34_0, &matrix->matrixElements[i]);
  }
}


/********************************************//**
 * \brief Free a real matrix
 *
 * \param[in] matrix real34Matrix_t *
 * \return void
 ***********************************************/
void realMatrixFree(real34Matrix_t *matrix) {
  uint16_t cols = matrix->header.matrixColumns;
  uint16_t rows = matrix->header.matrixRows;

  freeWp43s(matrix->matrixElements, TO_BLOCKS((rows * cols) * sizeof(real34_t)));
  matrix->matrixElements = NULL;
  matrix->header.matrixRows = matrix->header.matrixColumns = 0;
}


/********************************************//**
 * \brief Returns identity matrix of given size
 *
 * \param[in] matrix real34Matrix_t *
 * \return void
 ***********************************************/
void realMatrixIdentity(real34Matrix_t *matrix, uint16_t size) {
  realMatrixInit(matrix, size, size);
  for(uint16_t i = 0; i < size; ++i) {
    real34Copy(const34_1, &matrix->matrixElements[i * size + i]);
  }
}


/********************************************//**
 * \brief Displays a Matrix
 *
 * \return void
 ***********************************************/
void showMatrixEditor() {

  int rows = openMatrixMIMPointer.header.matrixRows;
  int cols = openMatrixMIMPointer.header.matrixColumns;
  int16_t Y_POS = Y_POSITION_OF_REGISTER_X_LINE;

  Y_POS = Y_POSITION_OF_REGISTER_X_LINE - NUMERIC_FONT_HEIGHT;

  bool_t colVector = false;
  if (cols == 1) {
    colVector = true;
    cols = rows;
    rows = 1;
  }

  //tbd: Implement right behavior (add row, add col on change)
  if (!getSystemFlag(FLAG_GROW)) {
    if (getIRegisterAsInt(true) < 0) {
      setIRegisterAsInt(true, rows - 1);
      setJRegisterAsInt(true, (getJRegisterAsInt(true) == 0) ? cols - 1 : getJRegisterAsInt(true) - 1);
    } else if (getIRegisterAsInt(true) == rows) {
      setIRegisterAsInt(true, 0);
      setJRegisterAsInt(true, (getJRegisterAsInt(true) == cols - 1) ? 0 : getJRegisterAsInt(true) + 1);
    }

    if (getJRegisterAsInt(true) < 0) {
      setJRegisterAsInt(true, cols - 1);
      setIRegisterAsInt(true, (getIRegisterAsInt(true) == 0) ? rows - 1 : getIRegisterAsInt(true) - 1);
    } else if (getJRegisterAsInt(true) == cols) {
      setJRegisterAsInt(true, 0);
      setIRegisterAsInt(true, (getIRegisterAsInt(true) == rows - 1) ? 0 : getIRegisterAsInt(true) + 1);
    }
  }
  else {
    //GROW tbd
  }

  int16_t matSelRow = getIRegisterAsInt(true);
  int16_t matSelCol = getJRegisterAsInt(true);

  videoMode_t vm = vmNormal;
  for(int i = 0; i < rows; i++) {
    showString("[", &numericFont, 1, Y_POS - (rows -1 - i) * NUMERIC_FONT_HEIGHT, vmNormal, true, false);
    for(int j = 0; j< cols; j++) {
      real34ToDisplayString(&openMatrixMIMPointer.matrixElements[i*cols+j], amNone, tmpString, &numericFont, 5, 10, true, STD_SPACE_4_PER_EM);
      if (matSelRow == i && matSelCol == j) {
        vm = vmReverse;
      } else {
        vm = vmNormal;
      }
      showString(tmpString, &numericFont, 5 + j * MATRIX_LINE_WIDTH_LARGE, Y_POS - (rows -1 -i) * NUMERIC_FONT_HEIGHT, vm, true, false);
    }
    showString("]", &numericFont, 10 + cols * MATRIX_LINE_WIDTH_LARGE, Y_POS - (rows -1 -i) * NUMERIC_FONT_HEIGHT, vmNormal, true, false);
    if (colVector == true) {
        showString("T", &numericFont, 20 + cols * MATRIX_LINE_WIDTH_LARGE, Y_POS - (rows -1 -i) * NUMERIC_FONT_HEIGHT, vmNormal, true, false);
    }
  }

  sprintf(tmpString, "%" PRIi16";%" PRIi16"= %s", matSelRow+1, matSelCol+1, nimBufferDisplay);
  if(aimBuffer[0] == 0)
    real34ToDisplayString(&openMatrixMIMPointer.matrixElements[matSelRow*cols+matSelCol], amNone, &tmpString[strlen(tmpString)], &numericFont, 5, 10, true, STD_SPACE_4_PER_EM);
  xCursor = showString(tmpString, &numericFont, 1, Y_POS + NUMERIC_FONT_HEIGHT, vmNormal, true, false) + 3;
  yCursor = Y_POSITION_OF_NIM_LINE;
}

void mimEnter(void) {
  int cols = openMatrixMIMPointer.header.matrixColumns;
  int16_t row = getIRegisterAsInt(true);
  int16_t col = getJRegisterAsInt(true);

  if(aimBuffer[0] != 0) {
    stringToReal34(aimBuffer, &openMatrixMIMPointer.matrixElements[row * cols + col]);

    aimBuffer[0] = 0;
    nimBufferDisplay[0] = 0;
    hideCursor();
    cursorEnabled = false;

    convertReal34MatrixToReal34MatrixRegister(&openMatrixMIMPointer, REGISTER_X);
    setSystemFlag(FLAG_ASLIFT);
  }
}

void mimAddNumber(int16_t item) {
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
      else if(nimNumberPart == NP_REAL_FLOAT_PART) {
        return; // fractional element is unsupported
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
        int cols = openMatrixMIMPointer.header.matrixColumns;
        int16_t row = getIRegisterAsInt(true);
        int16_t col = getJRegisterAsInt(true);

        real34ChangeSign(&openMatrixMIMPointer.matrixElements[row * cols + col]);

        convertReal34MatrixToReal34MatrixRegister(&openMatrixMIMPointer, REGISTER_X);
        setSystemFlag(FLAG_ASLIFT);
        return;
      }
      break;

    default:
      return;
  }
  addItemToNimBuffer(item);
  calcMode = CM_MIM;
}

void mimFinalize(void) {
  if(openMatrixMIMPointer.matrixElements) {
    realMatrixFree(&openMatrixMIMPointer);
  }
}

void mimRestore(void) {
  mimFinalize();
  getMatrixFromRegister(REGISTER_X);
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
  real34Matrix_t matrix;

  if (getRegisterDataType(regist) != dtReal34Matrix) {
    #ifdef PC_BUILD
    sprintf(errorMessage, "DataType %" PRIu32, getRegisterDataType(regist));
    moreInfoOnError("In function getMatrixFromRegister:", errorMessage, "is not dataType dtRealMatrix.", "");
    #endif
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    return;
  }

  if(openMatrixMIMPointer.matrixElements) realMatrixFree(&openMatrixMIMPointer);
  convertReal34MatrixRegisterToReal34Matrix(regist, &matrix);


  //NOT WORKING//
  openMatrixMIMPointer = matrix;
  //END NOT WORKING//
}

//Row of Matric
int16_t getIRegisterAsInt(bool_t asArrayPointer) {
  int16_t ret;
  longInteger_t tmp_lgInt;

  convertLongIntegerRegisterToLongInteger(REGISTER_I, tmp_lgInt);
  longIntegerToInt(tmp_lgInt, ret);

  longIntegerFree(tmp_lgInt);

  if (asArrayPointer) ret--;

  return ret;
}

//Col of Matrix
int16_t getJRegisterAsInt(bool_t asArrayPointer) {
  int16_t ret;
  longInteger_t tmp_lgInt;

  convertLongIntegerRegisterToLongInteger(REGISTER_J, tmp_lgInt);
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



/* Duplicate */
void copyRealMatrix(const real34Matrix_t *matrix, real34Matrix_t *res) {
  const uint16_t rows = matrix->header.matrixRows;
  const uint16_t cols = matrix->header.matrixColumns;
  int32_t i;

  realMatrixInit(res, rows, cols);
  for(i = 0; i < cols * rows; ++i) {
    real34Copy(&matrix->matrixElements[i], &res->matrixElements[i]);
  }
}



/* Transpose */
void transposeRealMatrix(const real34Matrix_t *matrix, real34Matrix_t *res) {
  const uint16_t rows = matrix->header.matrixRows;
  const uint16_t cols = matrix->header.matrixColumns;
  int32_t i, j;

  realMatrixInit(res, cols, rows);
  for(i = 0; i < rows; ++i) {
    for(j = 0; j < cols; ++j) {
      real34Copy(&matrix->matrixElements[i * cols + j], &res->matrixElements[j * rows + i]);
    }
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

  if((y != res) && (x != res)) realMatrixInit(res, rows, cols);
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



/* Multiplication */
void multiplyRealMatrix(const real34Matrix_t *matrix, const real34_t *x, real34Matrix_t *res) {
  const uint16_t rows = matrix->header.matrixRows;
  const uint16_t cols = matrix->header.matrixColumns;
  int32_t i;

  if(matrix != res) realMatrixInit(res, rows, cols);
  for(i = 0; i < cols * rows; ++i) {
    real34Multiply(&matrix->matrixElements[i], x, &res->matrixElements[i]);
  }
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

  realMatrixInit(res, rows, cols);
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



/* Vectors */
uint16_t realVectorSize(const real34Matrix_t *matrix) {
  if((matrix->header.matrixColumns != 1) && (matrix->header.matrixRows != 1))
    return 0;
  else
    return matrix->header.matrixColumns * matrix->header.matrixRows;
}

void dotRealVectors(const real34Matrix_t *y, const real34Matrix_t *x, real34_t *res) {
  const uint16_t elements = realVectorSize(y);
  int32_t i;
  real_t sum, prod, p, q;

  if((realVectorSize(y) == 0) || (realVectorSize(x) == 0) || (realVectorSize(y) != realVectorSize(x))) {
    realToReal34(const_NaN, res); // Not a vector or mismatched
    return;
  }

  realCopy(const_0, &sum);
  realCopy(const_0, &prod);
  for(i = 0; i < elements; ++i) {
    real34ToReal(&y->matrixElements[i], &p);
    real34ToReal(&x->matrixElements[i], &q);
    realMultiply(&p, &q, &prod, &ctxtReal39);
    realAdd(&sum, &prod, &sum, &ctxtReal39);
  }
  realToReal34(&sum, res);
}

void crossRealVectors(const real34Matrix_t *y, const real34Matrix_t *x, real34Matrix_t *res) {
  const uint16_t elementsY = realVectorSize(y);
  const uint16_t elementsX = realVectorSize(x);
  real_t a1, a2, a3, b1, b2, b3, p, q;

  if((elementsY == 0) || (elementsX == 0) || (elementsY > 3) || (elementsX > 3)) {
    realToReal34(const_NaN, res); // Not a vector or mismatched
    return;
  }

  real34ToReal(                 &y->matrixElements[0]            , &a1);
  real34ToReal(elementsY >= 2 ? &y->matrixElements[1] : const34_0, &a2);
  real34ToReal(elementsY >= 3 ? &y->matrixElements[2] : const34_0, &a3);

  real34ToReal(                 &x->matrixElements[0]            , &b1);
  real34ToReal(elementsX >= 2 ? &x->matrixElements[1] : const34_0, &b2);
  real34ToReal(elementsX >= 3 ? &x->matrixElements[2] : const34_0, &b3);

  realMatrixInit(res, 1, 3);

  realMultiply(&a2, &b3, &p, &ctxtReal39); realMultiply(&a3, &b2, &q, &ctxtReal39);
  realSubtract(&p, &q, &p, &ctxtReal39); realToReal34(&p, &res->matrixElements[0]);

  realMultiply(&a3, &b1, &p, &ctxtReal39); realMultiply(&a1, &b3, &q, &ctxtReal39);
  realSubtract(&p, &q, &p, &ctxtReal39); realToReal34(&p, &res->matrixElements[1]);

  realMultiply(&a1, &b2, &p, &ctxtReal39); realMultiply(&a2, &b1, &q, &ctxtReal39);
  realSubtract(&p, &q, &p, &ctxtReal39); realToReal34(&p, &res->matrixElements[2]);
}



/* LU decomposition routine borrowed from WP 34s */
void WP34S_LU_decomposition(const real34Matrix_t *matrix, real34Matrix_t *lu, uint16_t *p) {
  int i, j, k;
  int pvt;
  real_t max, t, u;

  const uint16_t n = matrix->header.matrixColumns;

  if(matrix->header.matrixRows != matrix->header.matrixColumns) {
    if(matrix != lu) {
      lu->matrixElements = NULL; // Matrix is not square
      lu->header.matrixRows = lu->header.matrixColumns = 0;
    }
    return;
  }

  if(matrix != lu) copyRealMatrix(matrix, lu);

  for(k = 0; k < n; k++) {
    /* Find the pivot row */
    pvt = k;
    real34ToReal(&lu->matrixElements[k * n + k], &u);
    realCopyAbs(&u, &max);
    for(j = k + 1; j < n; j++) {
      real34ToReal(&lu->matrixElements[j * n + k], &t);
      realCopyAbs(&t, &u);
      if (realCompareGreaterThan(&u, &max)) {
        realCopy(&u, &max);
        pvt = j;
      }
    }
    if(p != NULL)
      *p++ = pvt;

    /* pivot if required */
    if(pvt != k)
      realMatrixSwapRows(lu, lu, k, pvt);

    /* Check for singular */
    real34ToReal(&lu->matrixElements[k * n + k], &t);
    if(realIsZero(&t)) {
      realMatrixFree(lu);
      return;
    }

    /* Find the lower triangular elements for column k */
    for(i = k + 1; i < n; i++) {
      real34ToReal(&lu->matrixElements[k * n + k], &t);
      real34ToReal(&lu->matrixElements[i * n + k], &u);
      realDivide(&u, &t, &max, &ctxtReal39);
      realToReal34(&max, &lu->matrixElements[i * n + k]);
    }
    /* Update the upper triangular elements */
    for(i = k + 1; i < n; i++) {
      for(j = k + 1; j < n; j++) {
        real34ToReal(&lu->matrixElements[i * n + k], &t);
        real34ToReal(&lu->matrixElements[k * n + j], &u);
        realMultiply(&t, &u, &max, &ctxtReal39);
        real34ToReal(&lu->matrixElements[i * n + j], &t);
        realSubtract(&t, &max, &u, &ctxtReal39);
        realToReal34(&u, &lu->matrixElements[i * n + j]);
      }
    }
  }
}



/* Swap 2 rows */
void realMatrixSwapRows(const real34Matrix_t *matrix, real34Matrix_t *res, uint16_t a, uint16_t b) {
  const uint16_t rows = matrix->header.matrixRows;
  const uint16_t cols = matrix->header.matrixColumns;
  uint16_t i;
  real34_t t;

  if(matrix != res) copyRealMatrix(matrix, res);
  if((a < rows) && (b < rows) && (a != b)) {
    for(i = 0; i < cols; i++) {
      real34Copy(&res->matrixElements[a * cols + i], &t);
      real34Copy(&res->matrixElements[b * cols + i], &res->matrixElements[a * cols + i]);
      real34Copy(&t,                                 &res->matrixElements[b * cols + i]);
    }
  }
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

  p = allocWp43s(matrix->header.matrixRows * sizeof(uint16_t));

  WP34S_LU_decomposition(matrix, &lu, p);
  realCopy(const_1, &t);
  if(lu.matrixElements) {
    for(uint16_t i = 0; i < n; ++i) {
      real34ToReal(&lu.matrixElements[i * n + i], &u);
      if(p[i] != i) realChangeSign(&u);
      realMultiply(&t, &u, &t, &ctxtReal39);
    }
    realToReal34(&t, res);
  }
  else { // singular
    real34Copy(const34_0, res);
  }

  freeWp43s(p, matrix->header.matrixRows * sizeof(uint16_t));
}
#endif
