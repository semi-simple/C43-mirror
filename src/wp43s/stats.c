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



static void initStatisticalSums(void) {
  if(statisticalSumsPointer == 0) {
    statisticalSumsPointer = allocateMemory(14 * REAL34_SIZE);
    for(int32_t sum=0; sum<14; sum++) {
      real34Zero((real34_t *)(ram + statisticalSumsPointer + REAL34_SIZE*sum));
    }
  }
}



/********************************************//**
 * \brief Adds a value to the statistic registers
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnSigma(uint16_t plusMinus) {
  real34_t tmpReal34, square;

  if(   (getRegisterDataType(REGISTER_X) == dtBigInteger || getRegisterDataType(REGISTER_X) == dtReal16 || getRegisterDataType(REGISTER_X) == dtReal34)
     && (getRegisterDataType(REGISTER_Y) == dtBigInteger || getRegisterDataType(REGISTER_Y) == dtReal16 || getRegisterDataType(REGISTER_Y) == dtReal34)) {
    if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
      convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    }

    if(getRegisterDataType(REGISTER_X) == dtReal16) {
      convertRegister16To34(REGISTER_X);
    }

    if(getRegisterDataType(REGISTER_Y) == dtBigInteger) {
     convertBigIntegerRegisterToReal34Register(REGISTER_Y, REGISTER_Y);
    }

    if(getRegisterDataType(REGISTER_Y) == dtReal16) {
      convertRegister16To34(REGISTER_Y);
    }

    if(statisticalSumsPointer == 0) {
      initStatisticalSums();
    }

    if(plusMinus == 1) { // SIGMA+
      // n
      real34Add(RAM_REAL34(statisticalSumsPointer),                  const34_1,                             RAM_REAL34(statisticalSumsPointer));

      // sigma x
      real34Add(RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*1),  REGISTER_REAL34_DATA(REGISTER_X),      RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*1));

      // sigma y
      real34Add(RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*2),  REGISTER_REAL34_DATA(REGISTER_Y),      RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*2));

      // sigma x²
      real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X), &tmpReal34);
      real34Add(RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*3),  &tmpReal34,                            RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*3));

      // sigma x²y
      real34Multiply(&tmpReal34, REGISTER_REAL34_DATA(REGISTER_Y), &tmpReal34);
      real34Add(RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*4),  &tmpReal34,                            RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*4));

      // sigma y²
      real34Multiply(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_Y), &tmpReal34);
      real34Add(RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*5),  &tmpReal34,                            RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*5));

      // sigma xy
      real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y), &tmpReal34);
      real34Add(RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*6),  &tmpReal34,                            RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*6));

      // sigma ln(x)ln(y)
      real34Ln(REGISTER_REAL34_DATA(REGISTER_X), &tmpReal34);
      real34Ln(REGISTER_REAL34_DATA(REGISTER_Y), &square);
      real34Multiply(&tmpReal34, &square, &tmpReal34);
      real34Add(RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*7),  &tmpReal34,                            RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*7));

      // sigma ln(x)
      real34Ln(REGISTER_REAL34_DATA(REGISTER_X), &tmpReal34);
      real34Add(RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*8),  &tmpReal34,                            RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*8));

      // sigma ln²(x)
      real34Multiply(&tmpReal34, &tmpReal34, &square);
      real34Add(RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*9),  &square,                               RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*9));

      // sigma yln(x)
      real34Multiply(&tmpReal34, REGISTER_REAL34_DATA(REGISTER_Y), &tmpReal34);
      real34Add(RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*10), &tmpReal34,                            RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*10));

      // sigma ln(y)
      real34Ln(REGISTER_REAL34_DATA(REGISTER_Y), &tmpReal34);
      real34Add(RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*11), &tmpReal34,                            RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*11));

      // sigma ln²(y)
      real34Multiply(&tmpReal34, &tmpReal34, &square);
      real34Add(RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*12), &square,                               RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*12));

      // sigma xln(y)
      real34Multiply(&tmpReal34, REGISTER_REAL34_DATA(REGISTER_X), &tmpReal34);
      real34Add(RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*13), &tmpReal34,                            RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*13));
    }
    else {
      // n
      real34Subtract(RAM_REAL34(statisticalSumsPointer),  const34_1,                                        RAM_REAL34(statisticalSumsPointer));

      // sigma x
      real34Subtract(RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*1),  REGISTER_REAL34_DATA(REGISTER_X), RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*1));

      // sigma y
      real34Subtract(RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*2),  REGISTER_REAL34_DATA(REGISTER_X), RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*2));

      // sigma x^2
      real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X), &tmpReal34);
      real34Subtract(RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*3),  &tmpReal34,                       RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*3));

      // sigma x²y
      real34Multiply(&tmpReal34, REGISTER_REAL34_DATA(REGISTER_Y), &tmpReal34);
      real34Subtract(RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*4),  &tmpReal34,                       RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*4));

      // sigma y^2
      real34Multiply(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_Y), &tmpReal34);
      real34Subtract(RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*5),  &tmpReal34,                       RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*5));

      // sigma xy
      real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y), &tmpReal34);
      real34Subtract(RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*6),  &tmpReal34,                       RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*6));

      // sigma ln(x)ln(y)
      real34Ln(REGISTER_REAL34_DATA(REGISTER_X), &tmpReal34);
      real34Ln(REGISTER_REAL34_DATA(REGISTER_Y), &square);
      real34Multiply(&tmpReal34, &square, &tmpReal34);
      real34Subtract(RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*7),  &tmpReal34,                       RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*7));

      // sigma ln(x)
      real34Ln(REGISTER_REAL34_DATA(REGISTER_X), &tmpReal34);
      real34Subtract(RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*8),  &tmpReal34,                       RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*8));

      // sigma ln²(x)
      real34Multiply(&tmpReal34, &tmpReal34, &square);
      real34Subtract(RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*9),  &square,                          RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*9));

      // sigma yln(x)
      real34Multiply(&tmpReal34, REGISTER_REAL34_DATA(REGISTER_Y), &tmpReal34);
      real34Subtract(RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*10), &tmpReal34,                       RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*10));

      // sigma ln(y)
      real34Ln(REGISTER_REAL34_DATA(REGISTER_Y), &tmpReal34);
      real34Subtract(RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*11), &tmpReal34,                       RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*11));

      // sigma ln²(y)
      real34Multiply(&tmpReal34, &tmpReal34, &square);
      real34Subtract(RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*12), &square,                          RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*12));

      // sigma xln(y)
      real34Multiply(&tmpReal34, REGISTER_REAL34_DATA(REGISTER_X), &tmpReal34);
      real34Subtract(RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*13), &tmpReal34,                       RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*13));
    }

    temporaryInformation = TI_STATISTIC_SUMS;

    convertRegister34To16(REGISTER_X);
    convertRegister34To16(REGISTER_Y);
    refreshStack();
  }
  else {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X); // Invalid input data type for this operation
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot use (%s, %s) as statistical data!", getDataTypeName(getRegisterDataType(REGISTER_X), false, false), getDataTypeName(getRegisterDataType(REGISTER_Y), false, false));
      showInfoDialog("In function fnSigma:", errorMessage, NULL, NULL);
    #endif
  }
}



void fnStatSum(uint16_t sum) {
  liftStack();
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, 0);

  if(statisticalSumsPointer == 0) {
   real34Zero(REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
   real34Copy(RAM_REAL34(statisticalSumsPointer + REAL34_SIZE*sum), REGISTER_REAL34_DATA(REGISTER_X));
  }

  convertRegister34To16(REGISTER_X);
  refreshStack();
}
