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

#ifndef MATRIX_H
#define MATRIX_H

#include "typeDefinitions.h"

#ifndef TESTSUITE_BUILD
  #define MATRIX_LINE_WIDTH_LARGE      120
  #define MATRIX_LINE_WIDTH_SMALL      90
  //#define MATRIX_CHAR_LEN              30
#endif // TESTSUITE_BUILD

void       fnNewMatrix                    (uint16_t unusedParamButMandatory);
void       fnEditMatrix                   (uint16_t unusedParamButMandatory);

void       fnTranspose                    (uint16_t unusedParamButMandatory);
void       fnLuDecomposition              (uint16_t unusedParamButMandatory);
void       fnDeterminant                  (uint16_t unusedParamButMandatory);
void       fnInvertMatrix                 (uint16_t unusedParamButMandatory);

#ifndef TESTSUITE_BUILD
  void     realMatrixInit                 (real34Matrix_t *matrix, uint16_t rows, uint16_t cols);
  void     realMatrixFree                 (real34Matrix_t *matrix);
  void     realMatrixIdentity             (real34Matrix_t *matrix, uint16_t size);

  void     showMatrixEditor               (void);
  void     mimEnter                       (void);
  void     mimAddNumber                   (int16_t item);
  void     mimFinalize                    (void);
  void     mimRestore                     (void);

  void     getMatrixFromRegister          (calcRegister_t regist);

  int16_t  getIRegisterAsInt              (bool_t asArrayPointer);
  int16_t  getJRegisterAsInt              (bool_t asArrayPointer);
  void     setIRegisterAsInt              (bool_t asArrayPointer, int16_t toStore);
  void     setJRegisterAsInt              (bool_t asArrayPointer, int16_t toStore);

  void     copyRealMatrix                 (const real34Matrix_t *matrix, real34Matrix_t *res);
  void     transposeRealMatrix            (const real34Matrix_t *matrix, real34Matrix_t *res);

  void     addRealMatrices                (const real34Matrix_t *y, const real34Matrix_t *x, real34Matrix_t *res);
  void     subtractRealMatrices           (const real34Matrix_t *y, const real34Matrix_t *x, real34Matrix_t *res);

  void     multiplyRealMatrix             (const real34Matrix_t *matrix, const real34_t *x, real34Matrix_t *res);
  void     multiplyRealMatrices           (const real34Matrix_t *y, const real34Matrix_t *x, real34Matrix_t *res);

  uint16_t realVectorSize                 (const real34Matrix_t *matrix);
  void     dotRealVectors                 (const real34Matrix_t *y, const real34Matrix_t *x, real34_t *res);
  void     crossRealVectors               (const real34Matrix_t *y, const real34Matrix_t *x, real34Matrix_t *res);

  void     WP34S_LU_decomposition         (const real34Matrix_t *matrix, real34Matrix_t *lu, uint16_t *p);
  void     realMatrixSwapRows             (const real34Matrix_t *matrix, real34Matrix_t *res, uint16_t a, uint16_t b);
  void     detRealMatrix                  (const real34Matrix_t *matrix, real34_t *res);
  void     WP34S_matrix_inverse           (const real34Matrix_t *matrix, real34Matrix_t *res);
  void     divideRealMatrix               (const real34Matrix_t *matrix, const real34_t *x, real34Matrix_t *res);
  void     divideRealMatrices             (const real34Matrix_t *y, const real34Matrix_t *x, real34Matrix_t *res);
#endif // TESTSUITE_BUILD

void       elementwiseRema                (void (*f)(void));
void       elementwiseRemaLonI            (void (*f)(void));
void       elementwiseRemaReal            (void (*f)(void));
void       elementwiseRemaShoI            (void (*f)(void));

#endif // MATRIX_H
