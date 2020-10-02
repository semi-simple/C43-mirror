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

 real34Matrix_t* openMatrixMIMPointer;

/********************************************//**
 * \brief creates new Matrix of size y->m x x ->n
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnNewMatrix(uint16_t unusedParamButMandatory) {

  uint32_t rows, cols;

  longInteger_t tmp_lgInt;

  convertLongIntegerRegisterToLongInteger(REGISTER_X, tmp_lgInt);
  longIntegerToUInt(tmp_lgInt, cols);

  convertLongIntegerRegisterToLongInteger(REGISTER_Y, tmp_lgInt);
  longIntegerToUInt(tmp_lgInt, rows);

  longIntegerFree(tmp_lgInt);

  uint32_t reg_size;

  reg_size = (rows * cols) * sizeof(real34_t) + sizeof(registerDescriptor_t);

  real34Matrix_t* matrix = malloc(reg_size);

  matrix->header.matrixColumns = cols;
  matrix->header.matrixRows = rows;

  for(uint32_t i = 0; i < rows * cols; i++) {
    int32_t zero_int = 0;
    real34_t* zero = malloc(REAL34_SIZE);
    int32ToReal34(zero_int, zero);
    matrix->vals[i] = zero;
  }

  calcMode = CM_MIM;
  openMatrixMIMPointer = matrix;

  showMatrix(false, 0, 0);

  fnDrop(0);
  fnDrop(0);

  //showSoftmenu(NULL, -MNU_M_EDIT, true);
  //showSoftmenuCurrentPart();
  //for each;
  //showString(content, font, x, y, inversNormal, leadingCols, endingCols);
  //displayCalcErrorMessage();

}

/********************************************//**
 * \brief Opens the Matrix Editor
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnEditMatrix(uint16_t unusedParamButMandatory) {
  showSoftmenu(NULL, -MNU_M_EDIT, true);
  showMatrix(true, 0, 0);
}


/********************************************//**
 * \brief Displays a Matrix
 *
 * \param[in] matrixEditMode bool_t
 * \param[in] cursor_row     int16_t
 * \param[in] cursor_col     int16_t
 * \return void
 ***********************************************/
void showMatrix(bool_t matrixEditMode, int16_t cursor_row, int16_t cursor_col) {
  int cols = openMatrixMIMPointer->header.matrixColumns;
  int rows = openMatrixMIMPointer->header.matrixRows;
  int16_t Y_POS = Y_POSITION_OF_REGISTER_X_LINE;


  if (matrixEditMode) {
    int16_t Y_POS = Y_POSITION_OF_REGISTER_X_LINE - NUMERIC_FONT_HEIGHT;
  }

  bool_t colVector = false;
  if (cols == 1) {
    colVector = true;
    cols = rows;
    rows = 1;
  }

  videoMode_t vm = vmNormal;
  for(int i = 0; i < rows; i++) {
    showString("[", &numericFont, 1, Y_POS - (rows -1 - i) * NUMERIC_FONT_HEIGHT, vmNormal, true, false);
    for(int j = 0; j< cols; j++) {
      real34ToDisplayString(openMatrixMIMPointer->vals[i*cols+j], AM_NONE, tmpStr3000, &numericFont, 5, 10, true, STD_SPACE_4_PER_EM);
      if (matrixEditMode && cursor_row == i && cursor_col == j) {
        vm = vmReverse;
      } else {
        vm = vmNormal;
      }
      showString(tmpStr3000, &numericFont, 5 + j * MATRIX_LINE_WIDTH_LARGE, Y_POS - (rows -1 -i) * NUMERIC_FONT_HEIGHT, vm, true, false);
    }
    showString("]", &numericFont, 10 + cols * MATRIX_LINE_WIDTH_LARGE, Y_POS - (rows -1 -i) * NUMERIC_FONT_HEIGHT, vmNormal, true, false);
    if (colVector == true) {
        showString("T", &numericFont, 20 + cols * MATRIX_LINE_WIDTH_LARGE, Y_POS - (rows -1 -i) * NUMERIC_FONT_HEIGHT, vmNormal, true, false);
    }
  }
}

void storeMatrixToRegister(int16_t regist) {

  uint32_t reg_size = (openMatrixMIMPointer->header.matrixColumns * openMatrixMIMPointer->header.matrixRows) * sizeof(real34_t) + sizeof(registerDescriptor_t);
  real34Matrix_t* matrix = malloc(reg_size);


  setSystemFlag(FLAG_ASLIFT);
  liftStack();
  clearSystemFlag(FLAG_ASLIFT);

  reallocateRegister(regist, dtReal34Matrix, reg_size, AM_NONE);
  xcopy(REGISTER_REAL34_MATRIX(regist), matrix, reg_size);
}



