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
  
  fnDrop(unusedParamButMandatory);
  fnDrop(unusedParamButMandatory);
  
  setSystemFlag(FLAG_ASLIFT);
  liftStack();
  clearSystemFlag(FLAG_ASLIFT);
  
  int32_t zero_int = 0;
  
  uint32_t reg_size;
  
  reg_size = (rows * cols) * sizeof(real34_t) + sizeof(registerDescriptor_t);
  
  real34Matrix_t* matrix = malloc(reg_size);
  
  matrix->header.matrixColumns = cols;
  matrix->header.matrixRows = rows;
  
  for(uint32_t i = 0; i < rows * cols; i++) {
    real34_t zero;
    int32ToReal34(zero_int, &zero);
    matrix->vals[i] = &zero;
  }
  
  reallocateRegister(REGISTER_X, dtReal34Matrix, reg_size, AM_NONE);
  xcopy(REGISTER_REAL34_MATRIX(REGISTER_X), matrix, reg_size);
}


