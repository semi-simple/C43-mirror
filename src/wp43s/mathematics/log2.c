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
 * \file log2.c
 ***********************************************/

#include "wp43s.h"



void (* const logBase2[12])(void) = {
//  regX ==> 1            2          3          4           5           6           7           8           9            10             11         12
//           Big integer  real16     complex16              Time        Date        String      real16 mat  complex16 m  Small integer  real34     complex34
             log2BigI,    log2Re16,  log2Co16,  errorLog2,  errorLog2,  errorLog2,  errorLog2,  log2Rm16,   log2Cm16,    log2SmaI,      log2Re34,  log2Co34
};



/********************************************//**
 * \brief Data type error in log2
 *
 * \param void
 * \return void
 ***********************************************/
void errorLog2(void) {
  displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate log2 for %s", getRegisterDataTypeName(opX, true, false));
    showInfoDialog("In function fnLog2:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief Error message for a valid operation to be coded
 *
 * \param void
 * \return void
 ***********************************************/
void log2ToBeCoded(void) {
  #ifdef PC_BUILD
    sprintf(errorMessage, "log2(%s)", getRegisterDataTypeName(opX, false, false));
    showInfoDialog("Operation to be coded:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and log2(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnLog2(uint16_t unusedParamButMandatory) {
  if(logBase2[getRegisterDataType(REGISTER_X)] != errorLog2) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    result = REGISTER_X;
    opX    = allocateTemporaryRegister();
    copySourceRegisterToDestRegister(REGISTER_X, opX);

    logBase2[getRegisterDataType(REGISTER_X)]();
    freeTemporaryRegister(opX);

    refreshStack();
  }
  else {
    errorLog2();
  }
}



void log2BigI(void) {
  bigInteger_t value;

  convertBigIntegerRegisterToBigInteger(opX, &value);

  int32_t signX = value.sign;
  bigIntegerSetPositiveSign(&value);

  if(bigIntegerIsZero(&value) || signX) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function log2BigI: Cannot calculate the log" STD_SUB_2 " of a number " STD_LESS_EQUAL " 0!", NULL, NULL, NULL);
    #endif

    bigIntegerSetZero(&value);
    convertBigIntegerToBigIntegerRegister(&value, result);
    return;
  }

  uint32_t log2 = 0;

  if(!bigIntegerIsZero(&value)) {
    bigIntegerDivide2(&value, &value);
    while(!bigIntegerIsZero(&value)) {
      log2++;
      bigIntegerDivide2(&value, &value);
    }
  }

  uIntToBigInteger(log2, &value);
  convertBigIntegerToBigIntegerRegister(&value, result);
}



void log2Re16(void) {
  real51_t real51;

  if(real16IsZero(REGISTER_REAL16_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy(const16_minusInfinity, REGISTER_REAL16_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function log2Re16:", "cannot calculate Ln(0)", NULL, NULL);
      #endif
    }
  }
  else if(real16IsPositive(REGISTER_REAL16_DATA(opX))) { // Positive
    real16ToReal51(REGISTER_REAL16_DATA(opX), &real51);
    WP34S_real51Ln(&real51, &real51);
    real51Divide(&real51, const51_ln2, &real51);
    real51ToReal16(&real51, REGISTER_REAL16_DATA(result));
  }
  else if(getFlag(FLAG_CPXRES)) {
    real16SetPositiveSign(REGISTER_REAL16_DATA(opX));
    real16ToReal51(REGISTER_REAL16_DATA(opX), &real51);
    WP34S_real51Ln(&real51, &real51);
    real51Divide(&real51, const51_ln2, &real51);
    reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);
    real51ToReal16(&real51, REGISTER_REAL16_DATA(result));
    real16Copy(const16_pi, REGISTER_IMAG16_DATA(result));
    real16Divide(REGISTER_IMAG16_DATA(result), const16_2, REGISTER_IMAG16_DATA(result));
  }
  else if(getFlag(FLAG_DANGER)) {
    real16Copy(const16_NaN, REGISTER_REAL16_DATA(result));
  }
  else {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function log2Re16:", "cannot calculate Ln of a negative number when CPXRES is not set!", NULL, NULL);
    #endif
  }
}



void log2Co16(void) {
  if(real16IsZero(REGISTER_REAL16_DATA(opX)) && real16IsZero(REGISTER_IMAG16_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy(const16_NaN, REGISTER_REAL16_DATA(result));
      real16Copy(const16_NaN, REGISTER_IMAG16_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function log10Co16:", "cannot calculate Ln(0)", NULL, NULL);
      #endif
    }
  }
  else {
    real34_t magnitude34, theta34;
    real51_t real51;
    uint8_t savedAngularMode;

    savedAngularMode = angularMode;
    angularMode = AM_RADIAN;
    convertRegister16To34(opX);
    real34RectangularToPolar(REGISTER_REAL34_DATA(opX), REGISTER_IMAG34_DATA(opX), &magnitude34, &theta34);
    real34ToReal51(&magnitude34, &real51);
    real51Ln(&real51, &real51);
    real51Divide(&real51, const51_ln2, &real51);
    real51ToReal16(&real51, REGISTER_REAL16_DATA(result));
    convertAngle34FromInternal(&theta34, AM_RADIAN);
    real34Divide(&theta34, const34_ln2, &theta34);
    real34ToReal16(&theta34, REGISTER_IMAG16_DATA(result));
    angularMode = savedAngularMode;
  }
}



void log2Rm16(void) {
  log2ToBeCoded();
}



void log2Cm16(void) {
  log2ToBeCoded();
}



void log2SmaI(void) {
  *(REGISTER_SMALL_INTEGER_DATA(result)) = WP34S_intLog2(*(REGISTER_SMALL_INTEGER_DATA(opX)));
}



void log2Re34(void) {
  real51_t real51;

  if(real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const34_minusInfinity, REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function log2Re34:", "cannot calculate Ln(0)", NULL, NULL);
      #endif
    }
  }
  else if(real34IsPositive(REGISTER_REAL34_DATA(opX))) { // Positive
    real34ToReal51(REGISTER_REAL34_DATA(opX), &real51);
    WP34S_real51Ln(&real51, &real51);
    real51Divide(&real51, const51_ln2, &real51);
    real51ToReal34(&real51, REGISTER_REAL34_DATA(result));
  }
  else if(getFlag(FLAG_CPXRES)) {
    real34SetPositiveSign(REGISTER_REAL34_DATA(opX));
    real34ToReal51(REGISTER_REAL34_DATA(opX), &real51);
    WP34S_real51Ln(&real51, &real51);
    real51Divide(&real51, const51_ln2, &real51);
    reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
    real51ToReal34(&real51, REGISTER_REAL34_DATA(result));
    real34Copy(const34_pi, REGISTER_IMAG34_DATA(result));
    real34Divide(REGISTER_IMAG34_DATA(result), const34_ln2, REGISTER_IMAG34_DATA(result));
  }
  else if(getFlag(FLAG_DANGER)) {
    real34Copy(const34_NaN, REGISTER_REAL34_DATA(result));
  }
  else {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function log2Re34:", "cannot calculate Ln of a negative number when CPXRES is not set!", NULL, NULL);
    #endif
  }
}



void log2Co34(void) {
  if(real34IsZero(REGISTER_REAL34_DATA(opX)) && real34IsZero(REGISTER_IMAG34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const34_NaN, REGISTER_REAL34_DATA(result));
      real34Copy(const34_NaN, REGISTER_IMAG34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function log10Co34:", "cannot calculate Ln(0)", NULL, NULL);
      #endif
    }
  }
  else {
    real34_t magnitude34, theta34;
    real51_t real51;
    uint8_t savedAngularMode;

    savedAngularMode = angularMode;
    angularMode = AM_RADIAN;
    real34RectangularToPolar(REGISTER_REAL34_DATA(opX), REGISTER_IMAG34_DATA(opX), &magnitude34, &theta34);
    real34ToReal51(&magnitude34, &real51);
    real51Ln(&real51, &real51);
    real51Divide(&real51, const51_ln2, &real51);
    real51ToReal34(&real51, REGISTER_REAL34_DATA(result));
    convertAngle34FromInternal(&theta34, AM_RADIAN);
    real34Divide(&theta34, const34_ln2, &theta34);
    real34Copy(&theta34, REGISTER_IMAG34_DATA(result));
    angularMode = savedAngularMode;
  }
}
