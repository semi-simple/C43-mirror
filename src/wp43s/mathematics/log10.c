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
 * \file log10.c
 ***********************************************/

#include "wp43s.h"



void (* const logBase10[12])(void) = {
//  regX ==> 1            2           3           4            5            6            7            8           9            10             11          12
//           Big integer  real16      complex16                Time         Date         String       real16 mat  complex16 m  Small integer  real34      complex34
             log10BigI,   log10Re16,  log10Co16,  log10Error,  log10Error,  log10Error,  log10Error,  log10Rm16,  log10Cm16,   log10SmaI,     log10Re34,  log10Co34
};



/********************************************//**
 * \brief Data type error in log10
 *
 * \param void
 * \return void
 ***********************************************/
void log10Error(void) {
  displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate log10 for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnLog10:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief Error message for a valid operation to be coded
 *
 * \param void
 * \return void
 ***********************************************/
void log10ToBeCoded(void) {
  #ifdef PC_BUILD
    sprintf(errorMessage, "log10(%s)", getRegisterDataTypeName(REGISTER_X, false, false));
    showInfoDialog("Operation to be coded:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and log10(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnLog10(uint16_t unusedParamButMandatory) {
  if(logBase10[getRegisterDataType(REGISTER_X)] != log10Error) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    result = REGISTER_X;
    opX    = allocateTemporaryRegister();
    copySourceRegisterToDestRegister(REGISTER_X, opX);

    logBase10[getRegisterDataType(REGISTER_X)]();
    freeTemporaryRegister(opX);

    if(lastErrorCode != 0) {
      restoreStack();
      refreshStack();
    }
    else {
      refreshRegisterLine(REGISTER_X);
    }
  }
  else {
    log10Error();
  }
}



void log10BigI(void) {
  bigInteger_t value;

  convertBigIntegerRegisterToBigInteger(opX, &value);

  int32_t signX = value.sign;
  bigIntegerSetPositiveSign(&value);

  if(bigIntegerIsZero(&value) || signX) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function log10BigI: Cannot calculate the log" STD_SUB_10 " of a number " STD_LESS_EQUAL " 0!", NULL, NULL, NULL);
    #endif

    bigIntegerSetZero(&value);
    convertBigIntegerToBigIntegerRegister(&value, result);
    return;
  }

  uint32_t log10 = 0;
  uint64_t remainder;

  if(!bigIntegerIsZero(&value)) {
    bigIntegerDivideUInt(&value, 10, &value, &remainder);
    while(!bigIntegerIsZero(&value)) {
      log10++;
      bigIntegerDivideUInt(&value, 10, &value, &remainder);
    }
  }

  uIntToBigInteger(log10, &value);
  convertBigIntegerToBigIntegerRegister(&value, REGISTER_X);
}



void log10Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function log10Re16:", "cannot use NaN as an input of log" STD_SUB_10, NULL, NULL);
    #endif
    return;
  }

  real51_t real51;

  if(real16IsZero(REGISTER_REAL16_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy(const16_minusInfinity, REGISTER_REAL16_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function log10Re16:", "cannot calculate log" STD_SUB_10, NULL, NULL);
      #endif
    }
  }
  else if(real16IsPositive(REGISTER_REAL16_DATA(opX))) { // Positive
    real16ToReal51(REGISTER_REAL16_DATA(opX), &real51);
    WP34S_real51Ln(&real51, &real51);
    real51Divide(&real51, const51_ln10, &real51);
    real51ToReal16(&real51, REGISTER_REAL16_DATA(result));
  }
  else if(getFlag(FLAG_CPXRES)) {
    real16SetPositiveSign(REGISTER_REAL16_DATA(opX));
    real16ToReal51(REGISTER_REAL16_DATA(opX), &real51);
    WP34S_real51Ln(&real51, &real51);
    real51Divide(&real51, const51_ln10, &real51);
    reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);
    real51ToReal16(&real51, REGISTER_REAL16_DATA(result));
    real16Copy(const16_pi, REGISTER_IMAG16_DATA(result));
    real16Divide(REGISTER_IMAG16_DATA(result), const16_ln10, REGISTER_IMAG16_DATA(result));
  }
  else if(getFlag(FLAG_DANGER)) {
    real16Copy(const16_NaN, REGISTER_REAL16_DATA(result));
  }
  else {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function log10Re16:", "cannot calculate log" STD_SUB_10 " of a negative number when CPXRES is not set!", NULL, NULL);
    #endif
  }
}



void log10Co16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function log10Co16:", "cannot use NaN as an input of log" STD_SUB_10, NULL, NULL);
    #endif
    return;
  }

  if(real16IsZero(REGISTER_REAL16_DATA(opX)) && real16IsZero(REGISTER_IMAG16_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy(const16_NaN, REGISTER_REAL16_DATA(result));
      real16Copy(const16_NaN, REGISTER_IMAG16_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function log10Co16:", "cannot calculate log" STD_SUB_10, NULL, NULL);
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
    real51Divide(&real51, const51_ln10, &real51);
    real51ToReal16(&real51, REGISTER_REAL16_DATA(result));
    convertAngle34FromInternal(&theta34, AM_RADIAN);
    real34Divide(&theta34, const34_ln10, &theta34);
    real34ToReal16(&theta34, REGISTER_IMAG16_DATA(result));
    angularMode = savedAngularMode;
  }
}



void log10Rm16(void) {
  log10ToBeCoded();
}



void log10Cm16(void) {
  log10ToBeCoded();
}



void log10SmaI(void) {
  *(REGISTER_SMALL_INTEGER_DATA(result)) = WP34S_intLog10(*(REGISTER_SMALL_INTEGER_DATA(opX)));
}



void log10Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function log10Re34:", "cannot use NaN as an input of log" STD_SUB_10, NULL, NULL);
    #endif
    return;
  }

  real51_t real51;

  if(real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const34_minusInfinity, REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function log10Re34:", "cannot calculate log" STD_SUB_10, NULL, NULL);
      #endif
    }
  }
  else if(real34IsPositive(REGISTER_REAL34_DATA(opX))) { // Positive
    real34ToReal51(REGISTER_REAL34_DATA(opX), &real51);
    WP34S_real51Ln(&real51, &real51);
    real51Divide(&real51, const51_ln10, &real51);
    real51ToReal34(&real51, REGISTER_REAL34_DATA(result));
  }
  else if(getFlag(FLAG_CPXRES)) {
    real34SetPositiveSign(REGISTER_REAL34_DATA(opX));
    real34ToReal51(REGISTER_REAL34_DATA(opX), &real51);
    WP34S_real51Ln(&real51, &real51);
    real51Divide(&real51, const51_ln10, &real51);
    reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
    real51ToReal34(&real51, REGISTER_REAL34_DATA(result));
    real34Copy(const34_pi, REGISTER_IMAG34_DATA(result));
    real34Divide(REGISTER_IMAG34_DATA(result), const34_ln10, REGISTER_IMAG34_DATA(result));
  }
  else if(getFlag(FLAG_DANGER)) {
    real34Copy(const34_NaN, REGISTER_REAL34_DATA(result));
  }
  else {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function log10Re34:", "cannot calculate log" STD_SUB_10 " of a negative number when CPXRES is not set!", NULL, NULL);
    #endif
  }
}



void log10Co34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function log10Co34:", "cannot use NaN as an input of log" STD_SUB_10, NULL, NULL);
    #endif
    return;
  }

  if(real34IsZero(REGISTER_REAL34_DATA(opX)) && real34IsZero(REGISTER_IMAG34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const34_NaN, REGISTER_REAL34_DATA(result));
      real34Copy(const34_NaN, REGISTER_IMAG34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function log10Co34:", "cannot calculate log" STD_SUB_10, NULL, NULL);
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
    real51Divide(&real51, const51_ln10, &real51);
    real51ToReal34(&real51, REGISTER_REAL34_DATA(result));
    convertAngle34FromInternal(&theta34, AM_RADIAN);
    real34Divide(&theta34, const34_ln10, &theta34);
    real34Copy(&theta34, REGISTER_IMAG34_DATA(result));
    angularMode = savedAngularMode;
  }
}
