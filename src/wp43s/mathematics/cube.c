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



void (* const cube[12])(void) = {
// regX ==> 1            2         3         4          5          6          7          8          9           10            11        12
//          Long integer Real16    Complex16 Angle16    Time       Date       String     Real16 mat Complex16 m Short integer Real34    complex34
            cubeLonI,    cubeRe16, cubeCo16, cubeError, cubeError, cubeError, cubeError, cubeRm16,  cubeCm16,   cubeShoI,     cubeRe34, cubeCo34
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
  longInteger_t x, cube;

  convertLongIntegerRegisterToLongInteger(REGISTER_X, x);
  longIntegerInit(cube);
  longIntegerMultiply(x, x, cube);
  longIntegerMultiply(cube, x, cube);
  convertLongIntegerToLongIntegerRegister(cube, REGISTER_X);
  longIntegerFree(x);
  longIntegerFree(cube);
}



void cubeRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function cubeRe16:", "cannot use NaN as X input of ^3", NULL, NULL);
    #endif
    return;
  }

  realIc_t x, xSquared;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &x);

  realIcMultiply(&x, &x, &xSquared);
  realIcMultiply(&xSquared, &x, &x);

  realIcToReal16(&x, REGISTER_REAL16_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



void cubeCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function cubeCo16:", "cannot use NaN as X input of ^3", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, b, aSquared, bSquared, aCubed, bCubed;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &a);
  real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_X), &b);

  // (a + bi)³ = a³ - 3ab²  +  (3ba² - b³)i
  // a² and a³
  realIcMultiply(&a,        &a, &aSquared);
  realIcMultiply(&aSquared, &a, &aCubed);

  // b² and b³
  realIcMultiply(&b,        &b, &bSquared);
  realIcMultiply(&bSquared, &b, &bCubed);

  // real part
  realIcMultiply(&a,      const_3,   &a);
  realIcMultiply(&a,      &bSquared, &a);
  realIcSubtract(&aCubed, &a,        &a);

  // imag part
  realIcMultiply(&b, const_3,   &b);
  realIcMultiply(&b, &aSquared, &b);
  realIcSubtract(&b, &bCubed,   &b);

  realIcToReal16(&a, REGISTER_REAL16_DATA(REGISTER_X));
  realIcToReal16(&b, REGISTER_IMAG16_DATA(REGISTER_X));
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
      showInfoDialog("In function cubeRe34:", "cannot use NaN as X input of ^3", NULL, NULL);
    #endif
    return;
  }

  realIc_t x, xSquared;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &x);

  realIcMultiply(&x, &x, &xSquared);
  realIcMultiply(&xSquared, &x, &x);

  realIcToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



void cubeCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function cubeCo34:", "cannot use NaN as X input of ^3", NULL, NULL);
    #endif
    return;
  }

  realIc_t a, b, aSquared, bSquared, aCubed, bCubed;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &a);
  real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_X), &b);

  // (a + bi)³ = a³ - 3ab²  +  (3ba² - b³)i
  // a² and a³
  realIcMultiply(&a,        &a, &aSquared);
  realIcMultiply(&aSquared, &a, &aCubed);

  // b² annd b³
  realIcMultiply(&b,        &b, &bSquared);
  realIcMultiply(&bSquared, &b, &bCubed);

  // real part
  realIcMultiply(&a,      const_3,   &a);
  realIcMultiply(&a,      &bSquared, &a);
  realIcSubtract(&aCubed, &a,        &a);

  // imag part
  realIcMultiply(&b, const_3,   &b);
  realIcMultiply(&b, &aSquared, &b);
  realIcSubtract(&b, &bCubed,   &b);

  realIcToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
  realIcToReal34(&b, REGISTER_IMAG34_DATA(REGISTER_X));
}
