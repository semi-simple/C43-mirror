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
 * \file ln.c
 ***********************************************/

#include "wp43s.h"



void (* const ln[13])(void) = {
// regX ==> 1            2       3         4       5        6        7        8          9           10            11      12        13
//          Long integer Real16  Complex16 Angle16 Time     Date     String   Real16 mat Complex16 m Short integer Real34  Complex34 Angle34
            lnLonI,      lnRe16, lnCo16,   lnAn16, lnError, lnError, lnError, lnRm16,    lnCm16,     lnShoI,       lnRe34, lnCo34,   lnAn34
};



/********************************************//**
 * \brief Data type error in ln
 *
 * \param void
 * \return void
 ***********************************************/
void lnError(void) {
  displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate Ln for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnLn:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and ln(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnLn(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  result = REGISTER_X;
  opX    = allocateTemporaryRegister();
  copySourceRegisterToDestRegister(REGISTER_X, opX);

  ln[getRegisterDataType(REGISTER_X)]();

  adjustResult(result, false, true, opX, -1, -1);
}



void lnLonI(void) {
  real51_t real51;

  convertLongIntegerRegisterToReal34Register(opX, opX);
  if(real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      reallocateRegister(result, dtReal16, REAL16_SIZE, TAG_NONE);
      real16Copy(const16_minusInfinity, REGISTER_REAL16_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function lnLonI:", "cannot calculate Ln(0)", NULL, NULL);
      #endif
    }
  }
  else if(real34IsPositive(REGISTER_REAL34_DATA(opX))) { // Positive
    real34ToReal51(REGISTER_REAL34_DATA(opX), &real51);
    WP34S_real51Ln(&real51, &real51);
    reallocateRegister(result, dtReal16, REAL16_SIZE, TAG_NONE);
    real51ToReal16(&real51, REGISTER_REAL16_DATA(result));
   }
  else if(getFlag(FLAG_CPXRES)) {
    real34SetPositiveSign(REGISTER_REAL34_DATA(opX));
    real34ToReal51(REGISTER_REAL34_DATA(opX), &real51);
    WP34S_real51Ln(&real51, &real51);
    reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, TAG_NONE);
    real51ToReal16(&real51, REGISTER_REAL16_DATA(result));
    real16Copy(const16_pi, REGISTER_IMAG16_DATA(result));
  }
  else if(getFlag(FLAG_DANGER)) {
    real16Copy(const16_NaN, REGISTER_REAL16_DATA(result));
  }
  else {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function lnLonI:", "cannot calculate Ln of a negative number when CPXRES is not set!", NULL, NULL);
    #endif
  }
}



void lnRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function lnRe16:", "cannot use NaN as an input of ln", NULL, NULL);
    #endif
    return;
  }

  real51_t real51;

  if(real16IsZero(REGISTER_REAL16_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy(const16_minusInfinity, REGISTER_REAL16_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function lnRe16:", "cannot calculate Ln(0)", NULL, NULL);
      #endif
    }
  }
  else if(real16IsPositive(REGISTER_REAL16_DATA(opX))) { // Positive
    real16ToReal51(REGISTER_REAL16_DATA(opX), &real51);
    WP34S_real51Ln(&real51, &real51);
    real51ToReal16(&real51, REGISTER_REAL16_DATA(result));
  }
  else if(getFlag(FLAG_CPXRES)) {
    real16SetPositiveSign(REGISTER_REAL16_DATA(opX));
    real16ToReal51(REGISTER_REAL16_DATA(opX), &real51);
    WP34S_real51Ln(&real51, &real51);
    reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, TAG_NONE);
    real51ToReal16(&real51, REGISTER_REAL16_DATA(result));
    real16Copy(const16_pi, REGISTER_IMAG16_DATA(result));
  }
  else if(getFlag(FLAG_DANGER)) {
    real16Copy(const16_NaN, REGISTER_REAL16_DATA(result));
  }
  else {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function lnRe16:", "cannot calculate Ln of a negative number when CPXRES is not set!", NULL, NULL);
    #endif
  }
}



void lnCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function lnCo16:", "cannot use NaN as an input of ln", NULL, NULL);
    #endif
    return;
  }

  if(real16IsZero(REGISTER_REAL16_DATA(opX)) && real16IsZero(REGISTER_IMAG16_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy(const16_NaN, REGISTER_REAL16_DATA(result));
      real16Copy(const16_NaN, REGISTER_IMAG16_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function lnCo16:", "cannot calculate Ln(0)", NULL, NULL);
      #endif
    }
  }
  else {
    real34_t magnitude34, theta34;

    convertRegister16To34(opX);
    reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
    real34RectangularToPolar(REGISTER_REAL34_DATA(opX), REGISTER_IMAG34_DATA(opX), &magnitude34, REGISTER_IMAG34_DATA(result));
    real34Ln(&magnitude34, REGISTER_REAL34_DATA(result));
    convertRegister34To16(result);
  }
}



void lnAn16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function lnAn16:", "cannot use NaN as an input of ln", NULL, NULL);
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
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function lnAn16:", "cannot calculate Ln(0)", NULL, NULL);
      #endif
    }
  }
  else if(real16IsPositive(REGISTER_REAL16_DATA(opX))) { // Positive
    real16ToReal51(REGISTER_REAL16_DATA(opX), &real51);
    WP34S_real51Ln(&real51, &real51);
    real51ToReal16(&real51, REGISTER_REAL16_DATA(result));
  }
  else if(getFlag(FLAG_CPXRES)) {
    real16SetPositiveSign(REGISTER_REAL16_DATA(opX));
    real16ToReal51(REGISTER_REAL16_DATA(opX), &real51);
    WP34S_real51Ln(&real51, &real51);
    reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, TAG_NONE);
    real51ToReal16(&real51, REGISTER_REAL16_DATA(result));
    real16Copy(const16_pi, REGISTER_IMAG16_DATA(result));
  }
  else if(getFlag(FLAG_DANGER)) {
    real16Copy(const16_NaN, REGISTER_REAL16_DATA(result));
  }
  else {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function lnAn16:", "cannot calculate Ln of a negative number when CPXRES is not set!", NULL, NULL);
    #endif
  }
}



void lnRm16(void) {
  fnToBeCoded();
}



void lnCm16(void) {
  fnToBeCoded();
}



void lnShoI(void) {
  real51_t real51;

  convertShortIntegerRegisterToReal34Register(opX, opX);
  if(real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy(const16_minusInfinity, REGISTER_REAL16_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function lnShoI:", "cannot calculate Ln(0)", NULL, NULL);
      #endif
    }
  }
  else if(real34IsPositive(REGISTER_REAL34_DATA(opX))) { // Positive
    real34ToReal51(REGISTER_REAL34_DATA(opX), &real51);
    WP34S_real51Ln(&real51, &real51);
    reallocateRegister(result, dtReal16, REAL16_SIZE, TAG_NONE);
    real51ToReal16(&real51, REGISTER_REAL16_DATA(result));
   }
  else if(getFlag(FLAG_CPXRES)) {
    real34SetPositiveSign(REGISTER_REAL34_DATA(opX));
    real34ToReal51(REGISTER_REAL34_DATA(opX), &real51);
    WP34S_real51Ln(&real51, &real51);
    reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, TAG_NONE);
    real51ToReal16(&real51, REGISTER_REAL16_DATA(result));
    real16Copy(const16_pi, REGISTER_IMAG16_DATA(result));
  }
  else if(getFlag(FLAG_DANGER)) {
    real16Copy(const16_NaN, REGISTER_REAL34_DATA(result));
  }
  else {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function lnShoI:", "cannot calculate Ln of a negative number when CPXRES is not set!", NULL, NULL);
    #endif
  }
}



void lnRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function lnRe34:", "cannot use NaN as an input of ln", NULL, NULL);
    #endif
    return;
  }

  real51_t real51;

  if(real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const34_minusInfinity, REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function lnRe34:", "cannot calculate Ln(0)", NULL, NULL);
      #endif
    }
  }
  else if(real34IsPositive(REGISTER_REAL34_DATA(opX))) { // Positive
    real34ToReal51(REGISTER_REAL34_DATA(opX), &real51);
    WP34S_real51Ln(&real51, &real51);
    real51ToReal34(&real51, REGISTER_REAL34_DATA(result));
  }
  else if(getFlag(FLAG_CPXRES)) {
    real34SetPositiveSign(REGISTER_REAL34_DATA(opX));
    real34ToReal51(REGISTER_REAL34_DATA(opX), &real51);
    WP34S_real51Ln(&real51, &real51);
    reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
    real51ToReal34(&real51, REGISTER_REAL34_DATA(result));
    real34Copy(const34_pi, REGISTER_IMAG34_DATA(result));
  }
  else if(getFlag(FLAG_DANGER)) {
    real34Copy(const34_NaN, REGISTER_REAL34_DATA(result));
  }
  else {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function lnRe34:", "cannot calculate Ln of a negative number when CPXRES is not set!", NULL, NULL);
    #endif
  }
}



void lnCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function lnCo34:", "cannot use NaN as an input of ln", NULL, NULL);
    #endif
    return;
  }

  if(real34IsZero(REGISTER_REAL34_DATA(opX)) && real34IsZero(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const34_NaN, REGISTER_REAL34_DATA(result));
      real34Copy(const34_NaN, REGISTER_IMAG34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function lnCo34:", "cannot calculate Ln(0)", NULL, NULL);
      #endif
    }
  }
  else {
    real34_t magnitude34, theta34;

    real34RectangularToPolar(REGISTER_REAL34_DATA(opX), REGISTER_IMAG34_DATA(opX), &magnitude34, &theta34);
    real34Ln(&magnitude34, REGISTER_REAL34_DATA(result));
    real34Copy(&theta34, REGISTER_IMAG34_DATA(result));
  }
}



void lnAn34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function lnAn34:", "cannot use NaN as an input of ln", NULL, NULL);
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
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function lnAn34:", "cannot calculate Ln(0)", NULL, NULL);
      #endif
    }
  }
  else if(real34IsPositive(REGISTER_REAL34_DATA(opX))) { // Positive
    real34ToReal51(REGISTER_REAL34_DATA(opX), &real51);
    WP34S_real51Ln(&real51, &real51);
    real51ToReal34(&real51, REGISTER_REAL34_DATA(result));
  }
  else if(getFlag(FLAG_CPXRES)) {
    real34SetPositiveSign(REGISTER_REAL34_DATA(opX));
    real34ToReal51(REGISTER_REAL34_DATA(opX), &real51);
    WP34S_real51Ln(&real51, &real51);
    reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
    real51ToReal34(&real51, REGISTER_REAL34_DATA(result));
    real34Copy(const34_pi, REGISTER_IMAG34_DATA(result));
  }
  else if(getFlag(FLAG_DANGER)) {
    real34Copy(const34_NaN, REGISTER_REAL34_DATA(result));
  }
  else {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function lnAn34:", "cannot calculate Ln of a negative number when CPXRES is not set!", NULL, NULL);
    #endif
  }
}
