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
#define real34Ln(operand, res) {real34ToReal((real34_t *)operand, &real39); WP34S_Ln(&real39, &real39); realToReal34(&real39, (real34_t *)res);}



static void initStatisticalSums(void) {
  if(statisticalSumsPointer == NULL) {
    statisticalSumsPointer = allocWp43s(STATISTICAL_SUMS * REAL34_SIZE);
    for(int32_t sum=0; sum<STATISTICAL_SUMS; sum++) {
      real34Zero((real34_t *)(statisticalSumsPointer + REAL34_SIZE*sum));
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
  real34_t tmpReal1, tmpReal2, tmpReal3;
  real39_t real39;

  if(   (getRegisterDataType(REGISTER_X) == dtLongInteger || getRegisterDataType(REGISTER_X) == dtReal34)
     && (getRegisterDataType(REGISTER_Y) == dtLongInteger || getRegisterDataType(REGISTER_Y) == dtReal34)) {

    if(statisticalSumsPointer == NULL) {
      initStatisticalSums();
    }

    if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
      convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    }

    if(getRegisterDataType(REGISTER_Y) == dtLongInteger) {
     convertLongIntegerRegisterToReal34Register(REGISTER_Y, REGISTER_Y);
    }

    if(plusMinus == 1) { // SIGMA+
      // n
      real34Add(statisticalSumsPointer,                  const34_1,                             statisticalSumsPointer);

      // sigma x
      real34Add(statisticalSumsPointer + REAL34_SIZE*1,  REGISTER_REAL34_DATA(REGISTER_X),      statisticalSumsPointer + REAL34_SIZE*1);

      // sigma y
      real34Add(statisticalSumsPointer + REAL34_SIZE*2,  REGISTER_REAL34_DATA(REGISTER_Y),      statisticalSumsPointer + REAL34_SIZE*2);

      // sigma x²
      real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X), &tmpReal1);
      real34Add(statisticalSumsPointer + REAL34_SIZE*3,  &tmpReal1,                             statisticalSumsPointer + REAL34_SIZE*3);

      // sigma x³
      real34Multiply(&tmpReal1, REGISTER_REAL34_DATA(REGISTER_X), &tmpReal2);
      real34Add(statisticalSumsPointer + REAL34_SIZE*21,  &tmpReal2,                            statisticalSumsPointer + REAL34_SIZE*21);

      // sigma x⁴
      real34Multiply(&tmpReal2, REGISTER_REAL34_DATA(REGISTER_X), &tmpReal2);
      real34Add(statisticalSumsPointer + REAL34_SIZE*22,  &tmpReal2,                            statisticalSumsPointer + REAL34_SIZE*22);

      // sigma x²y
      real34Multiply(&tmpReal1, REGISTER_REAL34_DATA(REGISTER_Y), &tmpReal2);
      real34Add(statisticalSumsPointer + REAL34_SIZE*4,  &tmpReal2,                             statisticalSumsPointer + REAL34_SIZE*4);

      // sigma x²/y
      real34Divide(&tmpReal1, REGISTER_REAL34_DATA(REGISTER_Y), &tmpReal2);
      real34Add(statisticalSumsPointer + REAL34_SIZE*15,  &tmpReal2,                            statisticalSumsPointer + REAL34_SIZE*15);

      // sigma 1/x²
      real34Divide(const34_1, &tmpReal1, &tmpReal2);
      real34Add(statisticalSumsPointer + REAL34_SIZE*17,  &tmpReal2,                            statisticalSumsPointer + REAL34_SIZE*17);

      // sigma y²
      real34Multiply(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_Y), &tmpReal1);
      real34Add(statisticalSumsPointer + REAL34_SIZE*5,  &tmpReal1,                             statisticalSumsPointer + REAL34_SIZE*5);

      // sigma 1/y²
      real34Divide(const34_1, &tmpReal1, &tmpReal2);
      real34Add(statisticalSumsPointer + REAL34_SIZE*20,  &tmpReal2,                            statisticalSumsPointer + REAL34_SIZE*20);

      // sigma xy
      real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y), &tmpReal1);
      real34Add(statisticalSumsPointer + REAL34_SIZE*6,  &tmpReal1,                             statisticalSumsPointer + REAL34_SIZE*6);

      // sigma ln(x)
      real34Ln(REGISTER_REAL34_DATA(REGISTER_X), &tmpReal1);
      real34Copy(&tmpReal1 ,&tmpReal3);
      real34Add(statisticalSumsPointer + REAL34_SIZE*8,  &tmpReal1,                             statisticalSumsPointer + REAL34_SIZE*8);


      // sigma ln²(x)
      real34Multiply(&tmpReal1, &tmpReal1, &tmpReal2);
      real34Add(statisticalSumsPointer + REAL34_SIZE*9,  &tmpReal2,                             statisticalSumsPointer + REAL34_SIZE*9);

      // sigma yln(x)
      real34Multiply(&tmpReal1, REGISTER_REAL34_DATA(REGISTER_Y), &tmpReal1);
      real34Add(statisticalSumsPointer + REAL34_SIZE*10, &tmpReal1,                             statisticalSumsPointer + REAL34_SIZE*10);

      // sigma ln(y)
      real34Ln(REGISTER_REAL34_DATA(REGISTER_Y), &tmpReal1);
      real34Add(statisticalSumsPointer + REAL34_SIZE*11, &tmpReal1,                             statisticalSumsPointer + REAL34_SIZE*11);

      // sigma ln(x)×ln(y)
      real34Multiply(&tmpReal3, &tmpReal1, &tmpReal3);
      real34Add(statisticalSumsPointer + REAL34_SIZE*7,  &tmpReal3,                             statisticalSumsPointer + REAL34_SIZE*7);

      // sigma ln(y)/x
      real34Divide(&tmpReal1, REGISTER_REAL34_DATA(REGISTER_X), &tmpReal2);
      real34Add(statisticalSumsPointer + REAL34_SIZE*14, &tmpReal2,                             statisticalSumsPointer + REAL34_SIZE*14);

      // sigma ln²(y)
      real34Multiply(&tmpReal1, &tmpReal1, &tmpReal2);
      real34Add(statisticalSumsPointer + REAL34_SIZE*12, &tmpReal2,                             statisticalSumsPointer + REAL34_SIZE*12);

      // sigma xln(y)
      real34Multiply(&tmpReal1, REGISTER_REAL34_DATA(REGISTER_X), &tmpReal1);
      real34Add(statisticalSumsPointer + REAL34_SIZE*13, &tmpReal1,                             statisticalSumsPointer + REAL34_SIZE*13);

      // sigma 1/x
      real34Divide(const34_1, REGISTER_REAL34_DATA(REGISTER_X), &tmpReal1);
      real34Add(statisticalSumsPointer + REAL34_SIZE*16,  &tmpReal1,                            statisticalSumsPointer + REAL34_SIZE*16);

      // sigma x/y
      real34Divide(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y), &tmpReal1);
      real34Add(statisticalSumsPointer + REAL34_SIZE*18,  &tmpReal1,                            statisticalSumsPointer + REAL34_SIZE*18);

      // sigma 1/y
      real34Divide(const34_1, REGISTER_REAL34_DATA(REGISTER_Y), &tmpReal1);
      real34Add(statisticalSumsPointer + REAL34_SIZE*19,  &tmpReal1,                            statisticalSumsPointer + REAL34_SIZE*19);
    }
    else { // SIGMA-
      // n
      real34Subtract(statisticalSumsPointer,  const34_1,                                        statisticalSumsPointer);

      // sigma x
      real34Subtract(statisticalSumsPointer + REAL34_SIZE*1,  REGISTER_REAL34_DATA(REGISTER_X), statisticalSumsPointer + REAL34_SIZE*1);

      // sigma y
      real34Subtract(statisticalSumsPointer + REAL34_SIZE*2,  REGISTER_REAL34_DATA(REGISTER_Y), statisticalSumsPointer + REAL34_SIZE*2);

      // sigma x²
      real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X), &tmpReal1);
      real34Subtract(statisticalSumsPointer + REAL34_SIZE*3,  &tmpReal1,                        statisticalSumsPointer + REAL34_SIZE*3);

      // sigma x³
      real34Multiply(&tmpReal1, REGISTER_REAL34_DATA(REGISTER_X), &tmpReal2);
      real34Subtract(statisticalSumsPointer + REAL34_SIZE*21,  &tmpReal2,                       statisticalSumsPointer + REAL34_SIZE*21);

      // sigma x⁴
      real34Multiply(&tmpReal2, REGISTER_REAL34_DATA(REGISTER_X), &tmpReal2);
      real34Subtract(statisticalSumsPointer + REAL34_SIZE*22,  &tmpReal2,                       statisticalSumsPointer + REAL34_SIZE*22);

      // sigma x²y
      real34Multiply(&tmpReal1, REGISTER_REAL34_DATA(REGISTER_Y), &tmpReal2);
      real34Subtract(statisticalSumsPointer + REAL34_SIZE*4,  &tmpReal2,                        statisticalSumsPointer + REAL34_SIZE*4);

      // sigma x²/y
      real34Divide(&tmpReal1, REGISTER_REAL34_DATA(REGISTER_Y), &tmpReal2);
      real34Subtract(statisticalSumsPointer + REAL34_SIZE*15,  &tmpReal2,                       statisticalSumsPointer + REAL34_SIZE*15);

      // sigma 1/x²
      real34Divide(const34_1, &tmpReal1, &tmpReal2);
      real34Subtract(statisticalSumsPointer + REAL34_SIZE*17,  &tmpReal2,                       statisticalSumsPointer + REAL34_SIZE*17);

      // sigma y²
      real34Multiply(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_Y), &tmpReal1);
      real34Subtract(statisticalSumsPointer + REAL34_SIZE*5,  &tmpReal1,                        statisticalSumsPointer + REAL34_SIZE*5);

      // sigma 1/y²
      real34Divide(const34_1, &tmpReal1, &tmpReal2);
      real34Subtract(statisticalSumsPointer + REAL34_SIZE*20,  &tmpReal2,                       statisticalSumsPointer + REAL34_SIZE*20);

      // sigma xy
      real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y), &tmpReal1);
      real34Subtract(statisticalSumsPointer + REAL34_SIZE*6,  &tmpReal1,                        statisticalSumsPointer + REAL34_SIZE*6);

      // sigma ln(x)
      real34Ln(REGISTER_REAL34_DATA(REGISTER_X), &tmpReal1);
      real34Copy(&tmpReal1 ,&tmpReal3);
      real34Subtract(statisticalSumsPointer + REAL34_SIZE*8,  &tmpReal1,                        statisticalSumsPointer + REAL34_SIZE*8);

      // sigma ln²(x)
      real34Multiply(&tmpReal1, &tmpReal1, &tmpReal2);
      real34Subtract(statisticalSumsPointer + REAL34_SIZE*9,  &tmpReal2,                        statisticalSumsPointer + REAL34_SIZE*9);

      // sigma yln(x)
      real34Multiply(&tmpReal1, REGISTER_REAL34_DATA(REGISTER_Y), &tmpReal1);
      real34Subtract(statisticalSumsPointer + REAL34_SIZE*10, &tmpReal1,                        statisticalSumsPointer + REAL34_SIZE*10);

      // sigma ln(y)
      real34Ln(REGISTER_REAL34_DATA(REGISTER_Y), &tmpReal1);
      real34Subtract(statisticalSumsPointer + REAL34_SIZE*11, &tmpReal1,                        statisticalSumsPointer + REAL34_SIZE*11);

      // sigma ln(x)×ln(y)
      real34Multiply(&tmpReal3, &tmpReal1, &tmpReal3);
      real34Subtract(statisticalSumsPointer + REAL34_SIZE*7,  &tmpReal3,                        statisticalSumsPointer + REAL34_SIZE*7);

      // sigma ln(y)/x
      real34Divide(&tmpReal1, REGISTER_REAL34_DATA(REGISTER_X), &tmpReal2);
      real34Subtract(statisticalSumsPointer + REAL34_SIZE*14, &tmpReal2,                        statisticalSumsPointer + REAL34_SIZE*14);

      // sigma ln²(y)
      real34Multiply(&tmpReal1, &tmpReal1, &tmpReal2);
      real34Subtract(statisticalSumsPointer + REAL34_SIZE*12, &tmpReal2,                        statisticalSumsPointer + REAL34_SIZE*12);

      // sigma xln(y)
      real34Multiply(&tmpReal1, REGISTER_REAL34_DATA(REGISTER_X), &tmpReal1);
      real34Subtract(statisticalSumsPointer + REAL34_SIZE*13, &tmpReal1,                        statisticalSumsPointer + REAL34_SIZE*13);

      // sigma 1/x
      real34Divide(const34_1, REGISTER_REAL34_DATA(REGISTER_X), &tmpReal1);
      real34Subtract(statisticalSumsPointer + REAL34_SIZE*16,  &tmpReal1,                       statisticalSumsPointer + REAL34_SIZE*16);

      // sigma x/y
      real34Divide(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y), &tmpReal1);
      real34Subtract(statisticalSumsPointer + REAL34_SIZE*18,  &tmpReal1,                       statisticalSumsPointer + REAL34_SIZE*18);

      // sigma 1/y
      real34Divide(const34_1, REGISTER_REAL34_DATA(REGISTER_Y), &tmpReal1);
      real34Subtract(statisticalSumsPointer + REAL34_SIZE*19,  &tmpReal1,                       statisticalSumsPointer + REAL34_SIZE*19);
    }

    temporaryInformation = TI_STATISTIC_SUMS;

    refreshStack();
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X); // Invalid input data type for this operation
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot use (%s, %s) as statistical data!", getDataTypeName(getRegisterDataType(REGISTER_X), false, false), getDataTypeName(getRegisterDataType(REGISTER_Y), false, false));
      showInfoDialog("In function fnSigma:", errorMessage, NULL, NULL);
    #endif
  }
//for(int i=0; i<STATISTICAL_SUMS; i++) {
//printf("sum %02d ", i); printReal34ToConsole(((real34_t *)statisticalSumsPointer) + i); printf("\n");
//}
}



void fnStatSum(uint16_t sum) {
  liftStack();
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);

  if(statisticalSumsPointer == NULL) {
   real34Zero(REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
   real34Copy(statisticalSumsPointer + REAL34_SIZE*sum, REGISTER_REAL34_DATA(REGISTER_X));
  }

  refreshStack();
}

/********************************************//**
 * \brief regX ==> regL and SUM ==> regX, regY
 * enables stack lift and refreshes the stack.
 * regX = SUM x, regY = SUM y
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnSumXY(uint16_t unusedParamButMandatory) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
    reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, AM_NONE);

    if(statisticalSumsPointer == NULL) {
        real34Zero(REGISTER_REAL34_DATA(REGISTER_X));
        real34Zero(REGISTER_REAL34_DATA(REGISTER_Y));
    } else {
        real34Copy(statisticalSumsPointer + REAL34_SIZE * 1, REGISTER_REAL34_DATA(REGISTER_X));  // sigma x
        real34Copy(statisticalSumsPointer + REAL34_SIZE * 2, REGISTER_REAL34_DATA(REGISTER_Y));  // sigma y
    }

    temporaryInformation = TI_SUMX_SUMY;
    refreshStack();
}