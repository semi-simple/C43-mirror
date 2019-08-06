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



void (* const logBase2[13])(void) = {
// regX ==> 1            2         3         4         5          6          7          8          9           10            11        12        13
//          Long integer Real16    Complex16 Angle16   Time       Date       String     Real16 mat Complex16 m Short integer Real34    Complex34 Angle34
            log2LonI,    log2Re16, log2Co16, log2An16, log2Error, log2Error, log2Error, log2Rm16,  log2Cm16,   log2ShoI,     log2Re34, log2Co34, log2An34
};



/********************************************//**
 * \brief Data type error in log2
 *
 * \param void
 * \return void
 ***********************************************/
void log2Error(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate log2 for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnLog2:", errorMessage, NULL, NULL);
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
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  result = REGISTER_X;
  opX    = allocateTemporaryRegister();
  copySourceRegisterToDestRegister(REGISTER_X, opX);

  logBase2[getRegisterDataType(REGISTER_X)]();

  adjustResult(result, false, true, opX, -1, -1);
}



void log2LonI(void) {
  real51_t real51;

  convertLongIntegerRegisterToReal34Register(opX, opX);
  if(real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      reallocateRegister(result, dtReal16, REAL16_SIZE, TAG_NONE);
      real16Copy(const16_minusInfinity, REGISTER_REAL16_DATA(result));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function log2LonI:", "cannot calculate log2(0)", NULL, NULL);
      #endif
    }
  }
  else if(real34IsPositive(REGISTER_REAL34_DATA(opX))) { // Positive
    real34ToReal51(REGISTER_REAL34_DATA(opX), &real51);
    WP34S_real51Ln(&real51, &real51);
    real51Divide(&real51, const51_ln2, &real51);
    reallocateRegister(result, dtReal16, REAL16_SIZE, TAG_NONE);
    real51ToReal16(&real51, REGISTER_REAL16_DATA(result));
   }
  else if(getFlag(FLAG_CPXRES)) {
    real34SetPositiveSign(REGISTER_REAL34_DATA(opX));
    real34ToReal51(REGISTER_REAL34_DATA(opX), &real51);
    WP34S_real51Ln(&real51, &real51);
    real51Divide(&real51, const51_ln2, &real51);
    reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, TAG_NONE);
    real51ToReal16(&real51, REGISTER_REAL16_DATA(result));
    real16Copy(const16_pi, REGISTER_IMAG16_DATA(result));
    real16Divide(REGISTER_IMAG16_DATA(result), const16_ln2, REGISTER_IMAG16_DATA(result));
  }
  else if(getFlag(FLAG_DANGER)) {
    real16Copy(const16_NaN, REGISTER_REAL16_DATA(result));
  }
  else {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function log2LonI:", "cannot calculate log2 of a negative number when CPXRES is not set!", NULL, NULL);
    #endif
  }
}



void log2Re16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function log2Re16:", "cannot use NaN as an input of log" STD_SUB_2, NULL, NULL);
    #endif
    return;
  }

  real51_t real51;

  if(real16IsZero(REGISTER_REAL16_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy(const16_minusInfinity, REGISTER_REAL16_DATA(result));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function log2Re16:", "cannot calculate log" STD_SUB_2, NULL, NULL);
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
    reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, TAG_NONE);
    real51ToReal16(&real51, REGISTER_REAL16_DATA(result));
    real16Copy(const16_pi, REGISTER_IMAG16_DATA(result));
    real16Divide(REGISTER_IMAG16_DATA(result), const16_ln2, REGISTER_IMAG16_DATA(result));
  }
  else if(getFlag(FLAG_DANGER)) {
    real16Copy(const16_NaN, REGISTER_REAL16_DATA(result));
  }
  else {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function log2Re16:", "cannot calculate Ln of a negative number when CPXRES is not set!", NULL, NULL);
    #endif
  }
}



void log2Co16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function log2Co16:", "cannot use NaN as an input of log" STD_SUB_2, NULL, NULL);
    #endif
    return;
  }

  if(real16IsZero(REGISTER_REAL16_DATA(opX)) && real16IsZero(REGISTER_IMAG16_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy(const16_NaN, REGISTER_REAL16_DATA(result));
      real16Copy(const16_NaN, REGISTER_IMAG16_DATA(result));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function log10Co16:", "cannot calculate log" STD_SUB_2, NULL, NULL);
      #endif
    }
  }
  else {
    real34_t magnitude34, theta34;
    real51_t real51;

    convertRegister16To34(opX);
    real34RectangularToPolar(REGISTER_REAL34_DATA(opX), REGISTER_IMAG34_DATA(opX), &magnitude34, &theta34);
    real34ToReal51(&magnitude34, &real51);
    real51Ln(&real51, &real51);
    real51Divide(&real51, const51_ln2, &real51);
    real51ToReal16(&real51, REGISTER_REAL16_DATA(result));
    real34Divide(&theta34, const34_ln2, &theta34);
    real34ToReal16(&theta34, REGISTER_IMAG16_DATA(result));
  }
}



void log2An16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function log2An16:", "cannot use NaN as an input of log" STD_SUB_2, NULL, NULL);
    #endif
    return;
  }

  real51_t real51;

  setRegisterDataType(result, dtReal16, TAG_NONE);

  if(real16IsZero(REGISTER_REAL16_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy(const16_minusInfinity, REGISTER_REAL16_DATA(result));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function log2An16:", "cannot calculate log" STD_SUB_2, NULL, NULL);
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
    reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, TAG_NONE);
    real51ToReal16(&real51, REGISTER_REAL16_DATA(result));
    real16Copy(const16_pi, REGISTER_IMAG16_DATA(result));
    real16Divide(REGISTER_IMAG16_DATA(result), const16_ln2, REGISTER_IMAG16_DATA(result));
  }
  else if(getFlag(FLAG_DANGER)) {
    real16Copy(const16_NaN, REGISTER_REAL16_DATA(result));
  }
  else {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function log2An16:", "cannot calculate Ln of a negative number when CPXRES is not set!", NULL, NULL);
    #endif
  }
}



void log2Rm16(void) {
  fnToBeCoded();
}



void log2Cm16(void) {
  fnToBeCoded();
}



void log2ShoI(void) {
  *(REGISTER_SHORT_INTEGER_DATA(result)) = WP34S_intLog2(*(REGISTER_SHORT_INTEGER_DATA(opX)));
}



void log2Re34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function log2Re34:", "cannot use NaN as an input of log" STD_SUB_2, NULL, NULL);
    #endif
    return;
  }

  real51_t real51;

  if(real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const34_minusInfinity, REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function log2Re34:", "cannot calculate log" STD_SUB_2, NULL, NULL);
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
    reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
    real51ToReal34(&real51, REGISTER_REAL34_DATA(result));
    real34Copy(const34_pi, REGISTER_IMAG34_DATA(result));
    real34Divide(REGISTER_IMAG34_DATA(result), const34_ln2, REGISTER_IMAG34_DATA(result));
  }
  else if(getFlag(FLAG_DANGER)) {
    real34Copy(const34_NaN, REGISTER_REAL34_DATA(result));
  }
  else {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function log2Re34:", "cannot calculate log" STD_SUB_2 " of a negative number when CPXRES is not set!", NULL, NULL);
    #endif
  }
}



void log2Co34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function log2Co34:", "cannot use NaN as an input of log" STD_SUB_2, NULL, NULL);
    #endif
    return;
  }

  if(real34IsZero(REGISTER_REAL34_DATA(opX)) && real34IsZero(REGISTER_IMAG34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const34_NaN, REGISTER_REAL34_DATA(result));
      real34Copy(const34_NaN, REGISTER_IMAG34_DATA(result));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function log10Co34:", "cannot calculate log" STD_SUB_2, NULL, NULL);
      #endif
    }
  }
  else {
    real34_t magnitude34, theta34;
    real51_t real51;

    real34RectangularToPolar(REGISTER_REAL34_DATA(opX), REGISTER_IMAG34_DATA(opX), &magnitude34, &theta34);
    real34ToReal51(&magnitude34, &real51);
    real51Ln(&real51, &real51);
    real51Divide(&real51, const51_ln2, &real51);
    real51ToReal34(&real51, REGISTER_REAL34_DATA(result));
    real34Divide(&theta34, const34_ln2, &theta34);
    real34Copy(&theta34, REGISTER_IMAG34_DATA(result));
  }
}



void log2An34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function log2An34:", "cannot use NaN as an input of log" STD_SUB_2, NULL, NULL);
    #endif
    return;
  }

  real51_t real51;

  setRegisterDataType(result, dtReal34, TAG_NONE);

  if(real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const34_minusInfinity, REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function log2An34:", "cannot calculate log" STD_SUB_2, NULL, NULL);
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
    reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
    real51ToReal34(&real51, REGISTER_REAL34_DATA(result));
    real34Copy(const34_pi, REGISTER_IMAG34_DATA(result));
    real34Divide(REGISTER_IMAG34_DATA(result), const34_ln2, REGISTER_IMAG34_DATA(result));
  }
  else if(getFlag(FLAG_DANGER)) {
    real34Copy(const34_NaN, REGISTER_REAL34_DATA(result));
  }
  else {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function log2An34:", "cannot calculate log" STD_SUB_2 " of a negative number when CPXRES is not set!", NULL, NULL);
    #endif
  }
}
