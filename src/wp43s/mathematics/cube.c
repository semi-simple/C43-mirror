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
 * \file cube.c
 ***********************************************/

#include "wp43s.h"



void (* const cube[13])(void) = {
// regX ==> 1            2         3         4         5          6          7          8          9           10            11        12        13
//          Long integer Real16    Complex16 Angle16   Time       Date       String     Real16 mat Complex16 m Short integer Real34    complex34 Angle34
            cubeLonI,    cubeRe16, cubeCo16, cubeAn16, cubeError, cubeError, cubeError, cubeRm16,  cubeCm16,   cubeShoI,     cubeRe34, cubeCo34, cubeAn34
};



/********************************************//**
 * \brief Data type error in cubing
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void cubeError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot cube %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnCube:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and regX × regX × regX ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void fnCube(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  cube[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



void cubeLonI(void) {
  longInteger_t lgInt, cube;

  convertLongIntegerRegisterToLongInteger(REGISTER_X, lgInt);
  longIntegerInit(cube);
  longIntegerMultiply(lgInt, lgInt, cube);
  longIntegerMultiply(cube, lgInt, cube);
  convertLongIntegerToLongIntegerRegister(cube, REGISTER_X);
  longIntegerFree(lgInt);
  longIntegerFree(cube);
}



void cubeRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function cubeRe16:", "cannot use NaN as X input of ^2", NULL, NULL);
    #endif
    return;
  }

  real34_t square;

  convertRegister16To34(REGISTER_X);
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X), &square);
  real34Multiply(&square, REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
  convertRegister34To16(REGISTER_X);
}



void cubeCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function cubeCo16:", "cannot use NaN as X input of ^2", NULL, NULL);
    #endif
    return;
  }

  real34_t aSquared, bSquared, aCubed, bCubed;

  convertRegister16To34(REGISTER_X);

  // (a + bi)^3 = a^3 - 3ab²  +  (3ba² - b^3)i
  // a² and a^3
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X), &aSquared);
  real34Multiply(&aSquared,                        REGISTER_REAL34_DATA(REGISTER_X), &aCubed);

  // b² annd b^3
  real34Multiply(REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X), &bSquared);
  real34Multiply(&bSquared,                        REGISTER_IMAG34_DATA(REGISTER_X), &bCubed);

  // real part
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), const34_3,                        REGISTER_REAL34_DATA(REGISTER_X));
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), &bSquared,                        REGISTER_REAL34_DATA(REGISTER_X));
  real34Subtract(&aCubed,                          REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));

  // imag part
  real34Multiply(REGISTER_IMAG34_DATA(REGISTER_X), const34_3,                        REGISTER_IMAG34_DATA(REGISTER_X));
  real34Multiply(REGISTER_IMAG34_DATA(REGISTER_X), &aSquared,                        REGISTER_IMAG34_DATA(REGISTER_X));
  real34Subtract(REGISTER_IMAG34_DATA(REGISTER_X), &bCubed,                          REGISTER_IMAG34_DATA(REGISTER_X));

  convertRegister34To16(REGISTER_X);
}



void cubeAn16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function cubeAn16:", "cannot use NaN as X input of ^2", NULL, NULL);
    #endif
    return;
  }

  real34_t square;

  setRegisterDataType(REGISTER_X, dtReal16, TAG_NONE);
  convertRegister16To34(REGISTER_X);
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X), &square);
  real34Multiply(&square, REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
  convertRegister34To16(REGISTER_X);
}



void cubeRm16(void) {
  fnToBeCoded();
}



void cubeCm16(void) {
  fnToBeCoded();
}



void cubeShoI(void) {
  uint64_t square;
  square = WP34S_intMultiply(*(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)), *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)));
  *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = WP34S_intMultiply(square, *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)));
}



void cubeRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function cubeRe34:", "cannot use NaN as X input of ^2", NULL, NULL);
    #endif
    return;
  }

  real34_t square;

  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X), &square);
  real34Multiply(&square, REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
}



void cubeCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function cubeCo34:", "cannot use NaN as X input of ^2", NULL, NULL);
    #endif
    return;
  }

  real34_t aSquared, bSquared, aCubed, bCubed;

  // (a + bi)^3 = a^3 - 3ab²  +  (3ba² - b^3)i
  // a² and a^3
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X), &aSquared);
  real34Multiply(&aSquared,                        REGISTER_REAL34_DATA(REGISTER_X), &aCubed);

  // b² annd b^3
  real34Multiply(REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X), &bSquared);
  real34Multiply(&bSquared,                        REGISTER_IMAG34_DATA(REGISTER_X), &bCubed);

  // real part
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), const34_3,                        REGISTER_REAL34_DATA(REGISTER_X));
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), &bSquared,                        REGISTER_REAL34_DATA(REGISTER_X));
  real34Subtract(&aCubed,                          REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));

  // imag part
  real34Multiply(REGISTER_IMAG34_DATA(REGISTER_X), const34_3,                        REGISTER_IMAG34_DATA(REGISTER_X));
  real34Multiply(REGISTER_IMAG34_DATA(REGISTER_X), &aSquared,                        REGISTER_IMAG34_DATA(REGISTER_X));
  real34Subtract(REGISTER_IMAG34_DATA(REGISTER_X), &bCubed,                          REGISTER_IMAG34_DATA(REGISTER_X));
}



void cubeAn34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function cubeAn34:", "cannot use NaN as X input of ^2", NULL, NULL);
    #endif
    return;
  }

  real34_t square;

  setRegisterDataType(REGISTER_X, dtReal34, TAG_NONE);
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X), &square);
  real34Multiply(&square, REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
 }
