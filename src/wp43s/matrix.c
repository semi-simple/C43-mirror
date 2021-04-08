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
real34Matrix_t       *openMatrixMIMPointer;
bool_t                matEditMode;

/********************************************//**
 * \brief creates new Matrix of size y->m x x ->n
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnNewMatrix(uint16_t unusedParamButMandatory) {

  //Get Size from REGISTER_X and REGISTER_Y
  uint32_t rows, cols;

  longInteger_t tmp_lgInt;

  convertLongIntegerRegisterToLongInteger(REGISTER_X, tmp_lgInt);
  longIntegerToUInt(tmp_lgInt, cols);

  convertLongIntegerRegisterToLongInteger(REGISTER_Y, tmp_lgInt);
  longIntegerToUInt(tmp_lgInt, rows);

  longIntegerFree(tmp_lgInt);

  //Allocate Memory for Matrix
  uint32_t reg_size;
  reg_size = (rows * cols) * sizeof(real34_t) + sizeof(dataBlock_t);
  real34Matrix_t* matrix = malloc(reg_size);

  matrix->header.matrixColumns = cols;
  matrix->header.matrixRows = rows;

  //Initialize with 0.
  for(uint32_t i = 0; i < rows * cols; i++) {
    int32_t zero_int = 0;
    real34_t zero;
    //real34_t* zero = malloc(REAL34_SIZE);
    int32ToReal34(zero_int, &zero);
    matrix->matrixElements[i] = zero;
  }

  //Drop X_Register and Y_Register
  fnDrop(0);
  fnDrop(0);

  storeMatrixToXRegister(matrix);
}

/********************************************//**
 * \brief Opens the Matrix Editor
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnEditMatrix(uint16_t unusedParamButMandatory) {
  calcMode = CM_MIM;

  getMatrixFromRegister(REGISTER_X);

  showSoftmenu(-MNU_M_EDIT);
  setIRegisterAsInt(true, 0);
  setJRegisterAsInt(true, 0);
  showMatrixEditor();
}


/********************************************//**
 * \brief Displays a Matrix
 *
 * \return void
 ***********************************************/
void showMatrixEditor() {

  int rows = openMatrixMIMPointer->header.matrixRows;
  int cols = openMatrixMIMPointer->header.matrixColumns;
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
      setIRegisterAsInt(true, rows);
    } else if (getIRegisterAsInt(true) == rows) {
      setIRegisterAsInt(true, 0);
    }

    if (getJRegisterAsInt(true) < 0) {
      setJRegisterAsInt(true, rows);
    } else if (getJRegisterAsInt(true) == cols) {
      setJRegisterAsInt(true, 0);
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
      real34ToDisplayString(&openMatrixMIMPointer->matrixElements[i*cols+j], amNone, tmpString, &numericFont, 5, 10, true, STD_SPACE_4_PER_EM);
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

  sprintf(tmpString, "%" PRIi16";%" PRIi16"= ", matSelRow+1, matSelCol+1);
  showString(tmpString, &numericFont, 1, Y_POS + NUMERIC_FONT_HEIGHT, vmNormal, true, false);
}

void mimEnter(void) {
}

void mimAddNumber(void) {

}

void storeMatrixToXRegister(real34Matrix_t *matrix) {

  setSystemFlag(FLAG_ASLIFT);
  liftStack();
  clearSystemFlag(FLAG_ASLIFT);

  //WORKING//
  //openMatrixMIMPointer = matrix;
  // END WORKING//

  reallocateRegister(REGISTER_X, dtReal34Matrix, TO_BLOCKS((matrix->header.matrixColumns * matrix->header.matrixRows) * sizeof(real34_t) + sizeof(dataBlock_t)), amNone);
  xcopy(REGISTER_REAL34_MATRIX(REGISTER_X), matrix, TO_BLOCKS((matrix->header.matrixColumns * matrix->header.matrixRows) * sizeof(real34_t) + sizeof(dataBlock_t)));
}

//real34Matrix_t * getMatrixFromRegister(calcRegister_t regist) {
void getMatrixFromRegister(calcRegister_t regist) {
  //uint32_t reg_size = (matrix->header.matrixColumns * matrix->header.matrixRows) * sizeof(real34_t) + sizeof(registerHeader_t);
  //real34Matrix_t* matrix = malloc(reg_size);

  if (getRegisterDataType(regist) != dtReal34Matrix) {
    #ifdef PC_BUILD
    sprintf(errorMessage, "DataType % " PRIu32, getRegisterDataType(regist));
    moreInfoOnError("In function getMatrixFromRegister:", errorMessage, "is not dataType dtRealMatrix.", "");
    #endif
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    return;
  }

  dataBlock_t *dblock           = REGISTER_REAL34_MATRIX_DBLOCK(regist);
  real34_t    *matrixElem     = REGISTER_REAL34_MATRIX_M_ELEMENTS(regist);
  /*
  dataBlock_t header;
  xcopy(&header, REGISTER_REAL34_MATRIX_DBLOCK(regist), TO_BLOCKS(sizeof(dataBlock_t)));

  real34_t* matixElements[dblock->matrixColumns * dblock->matrixRows];
  xcopy(matixElements, REGISTER_REAL34_MATRIX_M_ELEMENTS(regist), TO_BLOCKS((dblock->matrixColumns * dblock->matrixRows) * sizeof(real34_t)));
  //xcopy(&matixElements, REGISTER_REAL34_MATRIX_DBLOCK(regist) + sizeof(dataBlock_t), (header.matrixColumns * header.matrixRows) * sizeof(real34_t));

  real34Matrix_t* matrix = malloc((dblock->matrixColumns * dblock->matrixRows) * sizeof(real34_t) + sizeof(registerHeader_t));
  matrix->header.matrixColumns = header.matrixColumns;
  matrix->header.matrixRows = header.matrixRows;
  int r = dblock->matrixRows;
  int c = dblock->matrixColumns;
  int hr = matrix->header.matrixRows;
  int hc = matrix->header.matrixColumns;
  for(int i = 0; i < dblock->matrixColumns * dblock->matrixRows; i++) {
    matrix->matrixElements[i] = *matixElements[i];
  }
  */

  real34Matrix_t* matrix = malloc((dblock->matrixColumns * dblock->matrixRows) * sizeof(real34_t) + sizeof(registerHeader_t));

  matrix->header.matrixColumns = dblock->matrixColumns;
  matrix->header.matrixRows = dblock->matrixRows;

  int hr = matrix->header.matrixRows;
  int hc = matrix->header.matrixColumns;

  for(int i = 0; i < dblock->matrixColumns * dblock->matrixRows; i++) {
    matrix->matrixElements[i] = *matrixElem;
    matrixElem++;
  }


  //NOT WORKING//
  openMatrixMIMPointer = matrix;
  //END NOT WORKING//

  //real34Matrix_t* matrix = getRegisterDataPointer(regist);
  //return matrix;
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
#endif
