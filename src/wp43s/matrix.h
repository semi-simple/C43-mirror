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
 * \file matrix.h
 ***********************************************/

#ifndef TESTSUITE_BUILD
  #define MATRIX_LINE_WIDTH_LARGE      40
  //#define MATRIX_CHAR_LEN              30
  
  void              fnNewMatrix           (uint16_t unusedParamButMandatory);
  void              fnEditMatrix          (uint16_t unusedParamButMandatory);

  void realMatrixInit(real34Matrix_t *matrix, uint16_t rows, uint16_t cols);
  void realMatrixFree(real34Matrix_t *matrix);

  void showMatrixEditor(void);
  void mimEnter(void);
  void mimAddNumber(int16_t item);
  void mimFinalize(void);

  //void storeMatrixToXRegister(real34Matrix_t *matrix);
  void getMatrixFromRegister(calcRegister_t regist);

  int16_t getIRegisterAsInt(bool_t asArrayPointer);
  int16_t getJRegisterAsInt(bool_t asArrayPointer);
  void setIRegisterAsInt(bool_t asArrayPointer, int16_t toStore);
  void setJRegisterAsInt(bool_t asArrayPointer, int16_t toStore);
  //real34Matrix_t * getMatrixFromRegister(calcRegister_t regist);

  void addRealMatrices(const real34Matrix_t *y, const real34Matrix_t *x, real34Matrix_t *res);
  void subtractRealMatrices(const real34Matrix_t *y, const real34Matrix_t *x, real34Matrix_t *res);

  void multiplyRealMatrix(const real34Matrix_t *matrix, const real34_t *x, real34Matrix_t *res);
  void multiplyRealMatrices(const real34Matrix_t *y, const real34Matrix_t *x, real34Matrix_t *res);
#endif // TESTSUITE_BUILD
