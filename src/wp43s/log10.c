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
             log10BigI,   log10Re16,  log10Co16,  errorLog10,  errorLog10,  errorLog10,  errorLog10,  log10Rm16,  log10Cm16,   log10SmaI,     log10Re34,  log10Co34
};



/********************************************//**
 * \brief Data type error in log10
 *
 * \param void
 * \return void
 ***********************************************/
void errorLog10(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("errorLog10");
  #endif

  displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate log10 for %s", getRegisterDataTypeName(op1, true, false));
    showInfoDialog("In function fnLog10:", errorMessage, NULL, NULL);
  #endif

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("errorLog10");
  #endif
}



/********************************************//**
 * \brief Error message for a valid operation to be coded
 *
 * \param void
 * \return void
 ***********************************************/
void log10ToBeCoded(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("log10ToBeCoded");
  #endif

  #ifdef PC_BUILD
    sprintf(errorMessage, "log10(%s)", getRegisterDataTypeName(op1, false, false));
    showInfoDialog("Operation to be coded:", errorMessage, NULL, NULL);
  #endif

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("log10ToBeCoded");
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
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnLog10");
  #endif

  if(logBase10[getRegisterDataType(REGISTER_X)] != errorLog10) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    result = REGISTER_X;
    op1    = allocateTemporaryRegister();
    copySourceRegisterToDestRegister(REGISTER_X, op1);

    logBase10[getRegisterDataType(REGISTER_X)]();
    freeTemporaryRegister(op1);

    refreshStack();
  }
  else {
    errorLog10();
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnLog10");
  #endif
}



void log10BigI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("log10BigI");
  #endif

  bigInteger_t value;

  convertBigIntegerRegisterToBigInteger(op1, &value);

  int32_t signX = value.sign;
  bigIntegerSetPositiveSign(&value);

  if(bigIntegerIsZero(&value) || signX) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function log10BigI: Cannot calculate the log" STD_SUB_10 " of a number " STD_LESS_EQUAL " 0!", NULL, NULL, NULL);
    #endif

    bigIntegerSetZero(&value);
    convertBigIntegerToBigIntegerRegister(&value, result);

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("log10BigI");
    #endif

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

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("log10BigI");
  #endif
}



void log10Re16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("log10Re16");
  #endif

  real51_t real51;

  if(real16IsZero(POINTER_TO_REGISTER_DATA(op1))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy(const16_minusInfinity, POINTER_TO_REGISTER_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function log10Re16:", "cannot calculate Ln(0)", NULL, NULL);
      #endif
    }
  }
  else if(real16IsPositive(POINTER_TO_REGISTER_DATA(op1))) { // Positive
    real16ToReal51(POINTER_TO_REGISTER_DATA(op1), &real51);
    WP34S_real51Ln(&real51, &real51);
    real51Divide(&real51, const51_ln10, &real51);
    real51ToReal16(&real51, POINTER_TO_REGISTER_DATA(result));
  }
  else if(getFlag(FLAG_CPXRES)) {
    real16SetPositiveSign(POINTER_TO_REGISTER_DATA(op1));
    real16ToReal51(POINTER_TO_REGISTER_DATA(op1), &real51);
    WP34S_real51Ln(&real51, &real51);
    real51Divide(&real51, const51_ln10, &real51);
    reallocateRegister(result, dtComplex16, COMPLEX16_SIZE, 0);
    real51ToReal16(&real51, POINTER_TO_REGISTER_DATA(result));
    real16Copy(const16_pi, COMPLEX16_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(result)));
    real16Divide(COMPLEX16_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(result)), const16_ln10, COMPLEX16_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(result)));
  }
  else if(getFlag(FLAG_DANGER)) {
    real16Copy(const16_NaN, POINTER_TO_REGISTER_DATA(result));
  }
  else {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function log10Re16:", "cannot calculate Ln of a negative number when CPXRES is not set!", NULL, NULL);
    #endif
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("log10Re16");
  #endif
}



void log10Co16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("log10Co16");
  #endif

  if(real16IsZero(POINTER_TO_REGISTER_DATA(op1)) && real16IsZero(COMPLEX16_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(op1)))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy(const16_NaN, POINTER_TO_REGISTER_DATA(result));
      real16Copy(const16_NaN, COMPLEX16_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(result)));
    }
    else {
      displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function log10Co16:", "cannot calculate Ln(0)", NULL, NULL);
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
    real51Divide(&real51, const51_ln10, &real51);
    real51ToReal16(&real51, POINTER_TO_REGISTER_DATA(result));
    real16Copy(&theta, COMPLEX16_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(result)));
    real16Divide(COMPLEX16_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(result)), const16_ln10, COMPLEX16_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(result)));
    angularMode = savedAngularMode;
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("log10Co16");
  #endif
}



void log10Rm16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("log10Rm16");
  #endif

  log10ToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("log10Rm16");
  #endif
}



void log10Cm16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("log10Cm16");
  #endif

  log10ToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("log10Cm16");
  #endif
}



void log10SmaI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("log10SmaI");
  #endif

  *(uint64_t *)(POINTER_TO_REGISTER_DATA(result)) = WP34S_intLog10(*(uint64_t *)(POINTER_TO_REGISTER_DATA(op1)));

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("log10SmaI");
  #endif
}



void log10Re34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("log10Re34");
  #endif

  real51_t real51;

  if(real34IsZero(POINTER_TO_REGISTER_DATA(op1))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const34_minusInfinity, POINTER_TO_REGISTER_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function log10Re34:", "cannot calculate Ln(0)", NULL, NULL);
      #endif
    }
  }
  else if(real34IsPositive(POINTER_TO_REGISTER_DATA(op1))) { // Positive
    real34ToReal51(POINTER_TO_REGISTER_DATA(op1), &real51);
    WP34S_real51Ln(&real51, &real51);
    real51Divide(&real51, const51_ln10, &real51);
    real51ToReal34(&real51, POINTER_TO_REGISTER_DATA(result));
  }
  else if(getFlag(FLAG_CPXRES)) {
    real34SetPositiveSign(POINTER_TO_REGISTER_DATA(op1));
    real34ToReal51(POINTER_TO_REGISTER_DATA(op1), &real51);
    WP34S_real51Ln(&real51, &real51);
    real51Divide(&real51, const51_ln10, &real51);
    reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
    real51ToReal34(&real51, POINTER_TO_REGISTER_DATA(result));
    real34Copy(const34_pi, COMPLEX34_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(result)));
    real34Divide(COMPLEX34_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(result)), const34_ln10, COMPLEX34_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(result)));
  }
  else if(getFlag(FLAG_DANGER)) {
    real34Copy(const34_NaN, POINTER_TO_REGISTER_DATA(result));
  }
  else {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function log10Re34:", "cannot calculate Ln of a negative number when CPXRES is not set!", NULL, NULL);
    #endif
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("log10Re34");
  #endif
}



void log10Co34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("log10Co34");
  #endif

  if(real34IsZero(POINTER_TO_REGISTER_DATA(op1)) && real34IsZero(COMPLEX34_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(op1)))) {
    if(getFlag(FLAG_DANGER)) {
      real16Copy(const34_NaN, POINTER_TO_REGISTER_DATA(result));
      real16Copy(const34_NaN, COMPLEX34_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(result)));
    }
    else {
      displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function log10Co34:", "cannot calculate Ln(0)", NULL, NULL);
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
    real51Divide(&real51, const51_ln10, &real51);
    real51ToReal34(&real51, POINTER_TO_REGISTER_DATA(result));
    real34Copy(&theta, COMPLEX34_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(result)));
    real34Divide(COMPLEX34_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(result)), const34_ln10, COMPLEX34_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(result)));
    angularMode = savedAngularMode;
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("log10Co34");
  #endif
}
