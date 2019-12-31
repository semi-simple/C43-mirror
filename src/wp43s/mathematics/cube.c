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



void (* const cube[9])(void) = {
// regX ==> 1            2         3         4          5          6          7          8           9
//          Long integer Real34    complex34 Time       Date       String     Real34 mat Complex34 m Short integer
            cubeLonI,    cubeReal, cubeCplx, cubeError, cubeError, cubeError, cubeRema,  cubeCxma,   cubeShoI
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



void cubeRema(void) {
  fnToBeCoded();
}



void cubeCxma(void) {
  fnToBeCoded();
}



void cubeShoI(void) {
  uint64_t square;
  square = WP34S_intMultiply(*(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)), *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)));
  *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = WP34S_intMultiply(square, *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)));
}



void cubeReal(void) {
  if(real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_X)) && !getFlag(FLAG_DANGER)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function cubeReal:", "cannot use " STD_PLUS_MINUS STD_INFINITY " as X input of ^3 when flag D is not set", NULL, NULL);
    #endif
    return;
  }

  real39_t x, xSquared;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);

  realMultiply(&x, &x, &xSquared, &ctxtReal39);
  realMultiply(&xSquared, &x, &x, &ctxtReal39);

  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



void cubeCplx(void) {
  real39_t a, b, aSquared, bSquared, aCubed, bCubed;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &a);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &b);

  // (a + bi)³ = a³ - 3ab²  +  (3ba² - b³)i
  // a² and a³
  realMultiply(&a,        &a, &aSquared, &ctxtReal39);
  realMultiply(&aSquared, &a, &aCubed, &ctxtReal39);

  // b² annd b³
  realMultiply(&b,        &b, &bSquared, &ctxtReal39);
  realMultiply(&bSquared, &b, &bCubed, &ctxtReal39);

  // real part
  realMultiply(&a,      const_3,   &a, &ctxtReal39);
  realMultiply(&a,      &bSquared, &a, &ctxtReal39);
  realSubtract(&aCubed, &a,        &a, &ctxtReal39);

  // imag part
  realMultiply(&b, const_3,   &b, &ctxtReal39);
  realMultiply(&b, &aSquared, &b, &ctxtReal39);
  realSubtract(&b, &bCubed,   &b, &ctxtReal39);

  realToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&b, REGISTER_IMAG34_DATA(REGISTER_X));
}
