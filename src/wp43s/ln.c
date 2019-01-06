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



void (* const ln[12])(void) = {
//  regX ==> 1            2        3          4         5         6         7         8           9            10             11       12
//           Big integer  real16   complex16            Time      Date      String    real16 mat  complex16 m  Small integer  real34   complex34
             lnBigI,      lnRe16,  lnCo16,    errorLn,  errorLn,  errorLn,  errorLn,  lnRm16,     lnCm16,      lnSmaI,        lnRe34,  lnCo34
};



/********************************************//**
 * \brief Data type error in ln
 *
 * \param void
 * \return void
 ***********************************************/
void errorLn(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("errorLn");
  #endif

  displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate Ln for %s", getRegisterDataTypeName(op1, true, false));
    showInfoDialog("In function fnLn:", errorMessage, NULL, NULL);
  #endif

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("errorLn");
  #endif
}



/********************************************//**
 * \brief Error message for a valid operation to be coded
 *
 * \param void
 * \return void
 ***********************************************/
void lnToBeCoded(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("lnToBeCoded");
  #endif

  #ifdef PC_BUILD
    sprintf(errorMessage, "ln(%s)", getRegisterDataTypeName(op1, false, false));
    showInfoDialog("Operation to be coded:", errorMessage, NULL, NULL);
  #endif

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("lnToBeCoded");
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
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnLn");
  #endif

  if(ln[getRegisterDataType(REGISTER_X)] != errorLn) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    result = REGISTER_X;
    op1    = allocateTemporaryRegister();
    copySourceRegisterToDestRegister(REGISTER_X, op1);

    ln[getRegisterDataType(REGISTER_X)]();
    freeTemporaryRegister(op1);

    refreshStack();
  }
  else {
    errorLn();
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnLn");
  #endif
}



void lnBigI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("lnBigI");
  #endif

  real51_t real51;

  convertBigIntegerRegisterToReal34Register(op1, op1);
  if(real34IsZero(POINTER_TO_REGISTER_DATA(op1))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy(const16_minusInfinity, POINTER_TO_REGISTER_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function lnBigI:", "cannot calculate Ln(0)", NULL, NULL);
      #endif
    }
  }
  else if(real34IsPositive(POINTER_TO_REGISTER_DATA(op1))) { // Positive
    real34ToReal51(POINTER_TO_REGISTER_DATA(op1), &real51);
    WP34S_real51Ln(&real51, &real51);
    reallocateRegister(result, dtReal16, REAL16_SIZE, 0);
    real51ToReal16(&real51, POINTER_TO_REGISTER_DATA(result));
   }
  else if(getFlag(FLAG_CPXRES)) {
    real34SetPositiveSign(POINTER_TO_REGISTER_DATA(op1));
    real34ToReal51(POINTER_TO_REGISTER_DATA(op1), &real51);
    WP34S_real51Ln(&real51, &real51);
    reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);
    real51ToReal16(&real51, POINTER_TO_REGISTER_DATA(result));
    real16Copy(const16_pi, COMPLEX16_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(result)));
  }
  else if(getFlag(FLAG_DANGER)) {
    real16Copy(const16_NaN, POINTER_TO_REGISTER_DATA(result));
  }
  else {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function lnBigI:", "cannot calculate Ln of a negative number when CPXRES is not set!", NULL, NULL);
    #endif
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("lnBigI");
  #endif
}



void lnRe16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("lnRe16");
  #endif

  real51_t real51;

  if(real16IsZero(POINTER_TO_REGISTER_DATA(op1))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy(const16_minusInfinity, POINTER_TO_REGISTER_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function lnRe16:", "cannot calculate Ln(0)", NULL, NULL);
      #endif
    }
  }
  else if(real16IsPositive(POINTER_TO_REGISTER_DATA(op1))) { // Positive
    real16ToReal51(POINTER_TO_REGISTER_DATA(op1), &real51);
    WP34S_real51Ln(&real51, &real51);
    real51ToReal16(&real51, POINTER_TO_REGISTER_DATA(result));
  }
  else if(getFlag(FLAG_CPXRES)) {
    real16SetPositiveSign(POINTER_TO_REGISTER_DATA(op1));
    real16ToReal51(POINTER_TO_REGISTER_DATA(op1), &real51);
    WP34S_real51Ln(&real51, &real51);
    reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);
    real51ToReal16(&real51, POINTER_TO_REGISTER_DATA(result));
    real16Copy(const16_pi, COMPLEX16_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(result)));
  }
  else if(getFlag(FLAG_DANGER)) {
    real16Copy(const16_NaN, POINTER_TO_REGISTER_DATA(result));
  }
  else {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function lnRe16:", "cannot calculate Ln of a negative number when CPXRES is not set!", NULL, NULL);
    #endif
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("lnRe16");
  #endif
}



void lnCo16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("lnCo16");
  #endif

  if(real16IsZero(POINTER_TO_REGISTER_DATA(op1)) && real16IsZero(COMPLEX16_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(op1)))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy(const16_NaN, POINTER_TO_REGISTER_DATA(result));
      real16Copy(const16_NaN, COMPLEX16_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(result)));
    }
    else {
      displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function lnCo16:", "cannot calculate Ln(0)", NULL, NULL);
      #endif
    }
  }
  else {
    real16_t magnitude, theta;
    real51_t real51;
    uint8_t savedAngularMode;

    savedAngularMode = angularMode;
    angularMode = AM_RADIAN;
    real16RectangularToPolar(REAL16_POINTER(POINTER_TO_REGISTER_DATA(op1)), COMPLEX16_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(op1)), &magnitude, &theta);
    real16ToReal51(&magnitude, &real51);
    real51Ln(&real51, &real51);
    real51ToReal16(&real51, POINTER_TO_REGISTER_DATA(result));
    real16Copy(&theta, COMPLEX16_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(result)));
    angularMode = savedAngularMode;
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("lnCo16");
  #endif
}



void lnRm16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("lnRm16");
  #endif

  lnToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("lnRm16");
  #endif
}



void lnCm16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("lnCm16");
  #endif

  lnToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("lnCm16");
  #endif
}



void lnSmaI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("lnSmaI");
  #endif

  real51_t real51;

  convertSmallIntegerRegisterToReal34Register(op1, op1);
  if(real34IsZero(POINTER_TO_REGISTER_DATA(op1))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy(const16_minusInfinity, POINTER_TO_REGISTER_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function lnSmaI:", "cannot calculate Ln(0)", NULL, NULL);
      #endif
    }
  }
  else if(real34IsPositive(POINTER_TO_REGISTER_DATA(op1))) { // Positive
    real34ToReal51(POINTER_TO_REGISTER_DATA(op1), &real51);
    WP34S_real51Ln(&real51, &real51);
    reallocateRegister(result, dtReal16, REAL16_SIZE, 0);
    real51ToReal16(&real51, POINTER_TO_REGISTER_DATA(result));
   }
  else if(getFlag(FLAG_CPXRES)) {
    real34SetPositiveSign(POINTER_TO_REGISTER_DATA(op1));
    real34ToReal51(POINTER_TO_REGISTER_DATA(op1), &real51);
    WP34S_real51Ln(&real51, &real51);
    reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);
    real51ToReal16(&real51, POINTER_TO_REGISTER_DATA(result));
    real16Copy(const16_pi, COMPLEX16_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(result)));
  }
  else if(getFlag(FLAG_DANGER)) {
    real16Copy(const16_NaN, POINTER_TO_REGISTER_DATA(result));
  }
  else {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function lnSmaI:", "cannot calculate Ln of a negative number when CPXRES is not set!", NULL, NULL);
    #endif
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("lnSmaI");
  #endif
}



void lnRe34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("lnRe34");
  #endif

  real51_t real51;

  if(real34IsZero(POINTER_TO_REGISTER_DATA(op1))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const34_minusInfinity, POINTER_TO_REGISTER_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function lnRe16:", "cannot calculate Ln(0)", NULL, NULL);
      #endif
    }
  }
  else if(real34IsPositive(POINTER_TO_REGISTER_DATA(op1))) { // Positive
    real34ToReal51(POINTER_TO_REGISTER_DATA(op1), &real51);
    WP34S_real51Ln(&real51, &real51);
    real51ToReal34(&real51, POINTER_TO_REGISTER_DATA(result));
  }
  else if(getFlag(FLAG_CPXRES)) {
    real34SetPositiveSign(POINTER_TO_REGISTER_DATA(op1));
    real34ToReal51(POINTER_TO_REGISTER_DATA(op1), &real51);
    WP34S_real51Ln(&real51, &real51);
    reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
    real51ToReal34(&real51, POINTER_TO_REGISTER_DATA(result));
    real34Copy(const34_pi, COMPLEX34_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(result)));
  }
  else if(getFlag(FLAG_DANGER)) {
    real34Copy(const34_NaN, POINTER_TO_REGISTER_DATA(result));
  }
  else {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function lnRe16:", "cannot calculate Ln of a negative number when CPXRES is not set!", NULL, NULL);
    #endif
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("lnRe34");
  #endif
}



void lnCo34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("lnCo34");
  #endif

  if(real34IsZero(POINTER_TO_REGISTER_DATA(op1)) && real34IsZero(COMPLEX34_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(op1)))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy(const34_NaN, POINTER_TO_REGISTER_DATA(result));
      real16Copy(const34_NaN, COMPLEX34_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(result)));
    }
    else {
      displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function lnCo34:", "cannot calculate Ln(0)", NULL, NULL);
      #endif
    }
  }
  else {
    real34_t magnitude, theta;
    real51_t real51;
    uint8_t savedAngularMode;

    savedAngularMode = angularMode;
    angularMode = AM_RADIAN;
    real34RectangularToPolar(REAL34_POINTER(POINTER_TO_REGISTER_DATA(op1)), COMPLEX34_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(op1)), &magnitude, &theta);
    real34ToReal51(&magnitude, &real51);
    real51Ln(&real51, &real51);
    real51ToReal34(&real51, POINTER_TO_REGISTER_DATA(result));
    real34Copy(&theta, COMPLEX34_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(result)));
    angularMode = savedAngularMode;
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("lnCo34");
  #endif
}
