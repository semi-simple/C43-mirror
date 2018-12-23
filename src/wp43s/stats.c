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
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("initStatisticalSums");
  #endif

  if(statisticalSumsPointer == 0) {
    statisticalSumsPointer = allocateMemory(14 * REAL16_SIZE);
    for(int32_t sum=0; sum<14; sum++) {
      real16Zero(ram + statisticalSumsPointer + REAL16_SIZE*sum);
    }
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("initStatisticalSums");
  #endif
}



/********************************************//**
 * \brief Adds a value to the statistic registers
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnSigma(uint16_t plusMinus) {
  real16_t tmpReal16, square;

  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnSigma");
  #endif

  if(   (getRegisterDataType(REGISTER_X) == dtBigInteger || getRegisterDataType(REGISTER_X) == dtReal16 || getRegisterDataType(REGISTER_X) == dtReal34)
     && (getRegisterDataType(REGISTER_Y) == dtBigInteger || getRegisterDataType(REGISTER_Y) == dtReal16 || getRegisterDataType(REGISTER_Y) == dtReal34)) {
    if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
      convertBigIntegerRegisterToReal16Register(REGISTER_X, REGISTER_X);
    }

    if(getRegisterDataType(REGISTER_X) == dtReal34) {
      convertRegister34To16(REGISTER_X);
    }

    if(getRegisterDataType(REGISTER_Y) == dtBigInteger) {
     convertBigIntegerRegisterToReal16Register(REGISTER_Y, REGISTER_Y);
    }

    if(getRegisterDataType(REGISTER_Y) == dtReal34) {
      convertRegister34To16(REGISTER_Y);
    }

    if(statisticalSumsPointer == 0) {
      initStatisticalSums();
    }

    if(plusMinus == 1) { // SIGMA+
      // n
      real16Add(RAM(statisticalSumsPointer),                  const16_1,                            RAM(statisticalSumsPointer));

      // sigma x
      real16Add(RAM(statisticalSumsPointer + REAL16_SIZE*1),  POINTER_TO_REGISTER_DATA(REGISTER_X), RAM(statisticalSumsPointer + REAL16_SIZE*1));

      // sigma y
      real16Add(RAM(statisticalSumsPointer + REAL16_SIZE*2),  POINTER_TO_REGISTER_DATA(REGISTER_Y), RAM(statisticalSumsPointer + REAL16_SIZE*2));

      // sigma x²
      real16Multiply(POINTER_TO_REGISTER_DATA(REGISTER_X), POINTER_TO_REGISTER_DATA(REGISTER_X), &tmpReal16);
      real16Add(RAM(statisticalSumsPointer + REAL16_SIZE*3),  &tmpReal16,                           RAM(statisticalSumsPointer + REAL16_SIZE*3));

      // sigma x²y
      real16Multiply(&tmpReal16, POINTER_TO_REGISTER_DATA(REGISTER_Y), &tmpReal16);
      real16Add(RAM(statisticalSumsPointer + REAL16_SIZE*4),  &tmpReal16,                           RAM(statisticalSumsPointer + REAL16_SIZE*4));

      // sigma y²
      real16Multiply(POINTER_TO_REGISTER_DATA(REGISTER_Y), POINTER_TO_REGISTER_DATA(REGISTER_Y), &tmpReal16);
      real16Add(RAM(statisticalSumsPointer + REAL16_SIZE*5),  &tmpReal16,                           RAM(statisticalSumsPointer + REAL16_SIZE*5));

      // sigma xy
      real16Multiply(POINTER_TO_REGISTER_DATA(REGISTER_X), POINTER_TO_REGISTER_DATA(REGISTER_Y), &tmpReal16);
      real16Add(RAM(statisticalSumsPointer + REAL16_SIZE*6),  &tmpReal16,                           RAM(statisticalSumsPointer + REAL16_SIZE*6));

      // sigma ln(x)ln(y)
      real16Ln(POINTER_TO_REGISTER_DATA(REGISTER_X), &tmpReal16);
      real16Ln(POINTER_TO_REGISTER_DATA(REGISTER_Y), &square);
      real16Multiply(&tmpReal16, &square, &tmpReal16);
      real16Add(RAM(statisticalSumsPointer + REAL16_SIZE*7),  &tmpReal16,                           RAM(statisticalSumsPointer + REAL16_SIZE*7));

      // sigma ln(x)
      real16Ln(POINTER_TO_REGISTER_DATA(REGISTER_X), &tmpReal16);
      real16Add(RAM(statisticalSumsPointer + REAL16_SIZE*8),  &tmpReal16,                           RAM(statisticalSumsPointer + REAL16_SIZE*8));

      // sigma ln²(x)
      real16Multiply(&tmpReal16, &tmpReal16, &square);
      real16Add(RAM(statisticalSumsPointer + REAL16_SIZE*9),  &square,                              RAM(statisticalSumsPointer + REAL16_SIZE*9));

      // sigma yln(x)
      real16Multiply(&tmpReal16, POINTER_TO_REGISTER_DATA(REGISTER_Y), &tmpReal16);
      real16Add(RAM(statisticalSumsPointer + REAL16_SIZE*10), &tmpReal16,                           RAM(statisticalSumsPointer + REAL16_SIZE*10));

      // sigma ln(y)
      real16Ln(POINTER_TO_REGISTER_DATA(REGISTER_Y), &tmpReal16);
      real16Add(RAM(statisticalSumsPointer + REAL16_SIZE*11), &tmpReal16,                           RAM(statisticalSumsPointer + REAL16_SIZE*11));

      // sigma ln²(y)
      real16Multiply(&tmpReal16, &tmpReal16, &square);
      real16Add(RAM(statisticalSumsPointer + REAL16_SIZE*12), &square,                              RAM(statisticalSumsPointer + REAL16_SIZE*12));

      // sigma xln(y)
      real16Multiply(&tmpReal16, POINTER_TO_REGISTER_DATA(REGISTER_X), &tmpReal16);
      real16Add(RAM(statisticalSumsPointer + REAL16_SIZE*13), &tmpReal16,                           RAM(statisticalSumsPointer + REAL16_SIZE*13));
    }
    else {
      // n
      real16Subtract(RAM(statisticalSumsPointer),  const16_1,                            RAM(statisticalSumsPointer));

      // sigma x
      real16Subtract(RAM(statisticalSumsPointer + REAL16_SIZE*1),  POINTER_TO_REGISTER_DATA(REGISTER_X), RAM(statisticalSumsPointer + REAL16_SIZE*1));

      // sigma y
      real16Subtract(RAM(statisticalSumsPointer + REAL16_SIZE*2),  POINTER_TO_REGISTER_DATA(REGISTER_X), RAM(statisticalSumsPointer + REAL16_SIZE*2));

      // sigma x^2
      real16Multiply(POINTER_TO_REGISTER_DATA(REGISTER_X), POINTER_TO_REGISTER_DATA(REGISTER_X), &tmpReal16);
      real16Subtract(RAM(statisticalSumsPointer + REAL16_SIZE*3),  &tmpReal16,                           RAM(statisticalSumsPointer + REAL16_SIZE*3));

      // sigma x²y
      real16Multiply(&tmpReal16, POINTER_TO_REGISTER_DATA(REGISTER_Y), &tmpReal16);
      real16Subtract(RAM(statisticalSumsPointer + REAL16_SIZE*4),  &tmpReal16,                           RAM(statisticalSumsPointer + REAL16_SIZE*4));

      // sigma y^2
      real16Multiply(POINTER_TO_REGISTER_DATA(REGISTER_Y), POINTER_TO_REGISTER_DATA(REGISTER_Y), &tmpReal16);
      real16Subtract(RAM(statisticalSumsPointer + REAL16_SIZE*5),  &tmpReal16,                           RAM(statisticalSumsPointer + REAL16_SIZE*5));

      // sigma xy
      real16Multiply(POINTER_TO_REGISTER_DATA(REGISTER_X), POINTER_TO_REGISTER_DATA(REGISTER_Y), &tmpReal16);
      real16Subtract(RAM(statisticalSumsPointer + REAL16_SIZE*6),  &tmpReal16,                           RAM(statisticalSumsPointer + REAL16_SIZE*6));

      // sigma ln(x)ln(y)
      real16Ln(POINTER_TO_REGISTER_DATA(REGISTER_X), &tmpReal16);
      real16Ln(POINTER_TO_REGISTER_DATA(REGISTER_Y), &square);
      real16Multiply(&tmpReal16, &square, &tmpReal16);
      real16Subtract(RAM(statisticalSumsPointer + REAL16_SIZE*7),  &tmpReal16,                           RAM(statisticalSumsPointer + REAL16_SIZE*7));

      // sigma ln(x)
      real16Ln(POINTER_TO_REGISTER_DATA(REGISTER_X), &tmpReal16);
      real16Subtract(RAM(statisticalSumsPointer + REAL16_SIZE*8),  &tmpReal16,                           RAM(statisticalSumsPointer + REAL16_SIZE*8));

      // sigma ln²(x)
      real16Multiply(&tmpReal16, &tmpReal16, &square);
      real16Subtract(RAM(statisticalSumsPointer + REAL16_SIZE*9),  &square,                              RAM(statisticalSumsPointer + REAL16_SIZE*9));

      // sigma yln(x)
      real16Multiply(&tmpReal16, POINTER_TO_REGISTER_DATA(REGISTER_Y), &tmpReal16);
      real16Subtract(RAM(statisticalSumsPointer + REAL16_SIZE*10), &tmpReal16,                           RAM(statisticalSumsPointer + REAL16_SIZE*10));

      // sigma ln(y)
      real16Ln(POINTER_TO_REGISTER_DATA(REGISTER_Y), &tmpReal16);
      real16Subtract(RAM(statisticalSumsPointer + REAL16_SIZE*11), &tmpReal16,                           RAM(statisticalSumsPointer + REAL16_SIZE*11));

      // sigma ln²(y)
      real16Multiply(&tmpReal16, &tmpReal16, &square);
      real16Subtract(RAM(statisticalSumsPointer + REAL16_SIZE*12), &square,                              RAM(statisticalSumsPointer + REAL16_SIZE*12));

      // sigma xln(y)
      real16Multiply(&tmpReal16, POINTER_TO_REGISTER_DATA(REGISTER_X), &tmpReal16);
      real16Subtract(RAM(statisticalSumsPointer + REAL16_SIZE*13), &tmpReal16,                           RAM(statisticalSumsPointer + REAL16_SIZE*13));
    }

    temporaryInformation = TI_STATISTIC_SUMS;

    refreshStack();
  }
  else {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X); // Invalid input data type for this operation
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot use (%s, %s) as statistical data!", getDataTypeName(getRegisterDataType(REGISTER_X), false, false), getDataTypeName(getRegisterDataType(REGISTER_Y), false, false));
      showInfoDialog("In function fnSigma:", errorMessage, NULL, NULL);
    #endif
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnSigma");
  #endif
}



void fnStatSum(uint16_t sum) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnStatSum");
  #endif

  liftStack(dtReal16, REAL16_SIZE);

  if(statisticalSumsPointer == 0) {
   real16Zero(POINTER_TO_REGISTER_DATA(REGISTER_X));
  }
  else {
   real16Copy(RAM(statisticalSumsPointer + REAL16_SIZE*sum), POINTER_TO_REGISTER_DATA(REGISTER_X));
  }

  refreshStack();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnStatSum");
  #endif
}
