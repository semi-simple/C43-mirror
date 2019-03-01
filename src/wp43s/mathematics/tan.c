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
 * \file tan.c
 ***********************************************/

#include "wp43s.h"



void (* const Tan[12])(void) = {
//  regX ==> 1             2         3          4         5          6          7          8            9             10              11        12
//           Long integer  real16    complex16  angle     Time       Date       String     real16 mat   complex16 m   Short integer   real34    complex34
             tanLonI,      tanRe16,  tanCo16,   tanAngl,  errorTan,  errorTan,  errorTan,  tanRm16,     tanCm16,      errorTan,       tanRe34,  tanCo34
};



/********************************************//**
 * \brief Data type error in tan
 *
 * \param void
 * \return void
 ***********************************************/
void errorTan(void) {
  displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate Tan for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnTan:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief Error message for a valid operation to be coded
 *
 * \param void
 * \return void
 ***********************************************/
void tanToBeCoded(void) {
  #ifdef PC_BUILD
    sprintf(errorMessage, "tan(%s)", getRegisterDataTypeName(REGISTER_X, false, false));
    showInfoDialog("Operation to be coded:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and tan(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnTan(uint16_t unusedParamButMandatory) {
  if(Tan[getRegisterDataType(REGISTER_X)] != errorTan) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    result = REGISTER_X;
    opX    = allocateTemporaryRegister();
    copySourceRegisterToDestRegister(REGISTER_X, opX);

    Tan[getRegisterDataType(REGISTER_X)]();
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
    errorTan();
  }
}



void tanLonI(void) {
  real34_t cos;

  convertLongIntegerRegisterToReal34Register(opX, opX);
  convertAngle34ToInternal(REGISTER_REAL34_DATA(opX), angularMode);
  reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
  WP34S_cvt_2rad_sincos(REGISTER_REAL34_DATA(result), &cos, REGISTER_REAL34_DATA(opX)); // opX in internal units
  if(real34IsZero(&cos)) {
    if(getFlag(FLAG_DANGER)) {
      real34Divide(REGISTER_REAL34_DATA(result), &cos, REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function tanRe34:", "X = " STD_PLUS_MINUS "90" STD_DEGREE, NULL, NULL);
      #endif
      return;
    }
  }
  else {
   real34Divide(REGISTER_REAL34_DATA(result), &cos, REGISTER_REAL34_DATA(result));
  }
  convertRegister34To16(result);
}



void tanRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function tanRe16:", "cannot use NaN as an input of tan", NULL, NULL);
    #endif
    return;
  }

  if(real16IsSpecial(REGISTER_REAL16_DATA(opX))) {
    real16Copy(const16_NaN, REGISTER_REAL16_DATA(result));
  }
  else {
    real34_t cos;

    convertRegister16To34(opX);
    convertAngle34ToInternal(REGISTER_REAL34_DATA(opX), angularMode);
    reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
    WP34S_cvt_2rad_sincos(REGISTER_REAL34_DATA(result), &cos, REGISTER_REAL34_DATA(opX)); // opX in internal units
    if(real34IsZero(&cos)) {
      if(getFlag(FLAG_DANGER)) {
      	 real34Divide(REGISTER_REAL34_DATA(result), &cos, REGISTER_REAL34_DATA(result));
      }
      else {
        displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function tanRe16:", "X = " STD_PLUS_MINUS "90" STD_DEGREE, NULL, NULL);
        #endif
        return;
      }
    }
    else {
   	 real34Divide(REGISTER_REAL34_DATA(result), &cos, REGISTER_REAL34_DATA(result));
    }
    convertRegister34To16(result);
  }
}



void tanCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function tanCo16:", "cannot use NaN as an input of tan", NULL, NULL);
    #endif
    return;
  }

  // tan(z) = -i(exp(iz) - exp(-iz)) / (exp(iz) + exp(-iz))
  complex34_t iz, expIz, expMIz, numer;

  convertRegister16To34(opX);
  convertRegister16To34(result);

  // calculate iz
  real34Copy(REGISTER_REAL34_DATA(opX), VARIABLE_IMAG34_DATA(&iz));
  real34Copy(REGISTER_IMAG34_DATA(opX), VARIABLE_REAL34_DATA(&iz));
  real34ChangeSign(VARIABLE_REAL34_DATA(&iz));

  // calculate exp(iz)
  complex34Copy(VARIABLE_COMPLEX34_DATA(&iz), REGISTER_COMPLEX34_DATA(opX));
  expCo34();
  complex34Copy(REGISTER_COMPLEX34_DATA(result), VARIABLE_COMPLEX34_DATA(&expIz));

  // calculate exp(-iz)
  complex34ChangeSign(VARIABLE_COMPLEX34_DATA(&iz));
  complex34Copy(VARIABLE_COMPLEX34_DATA(&iz), REGISTER_COMPLEX34_DATA(opX));
  expCo34();
  complex34Copy(REGISTER_COMPLEX34_DATA(result), VARIABLE_COMPLEX34_DATA(&expMIz));

  // calculate exp(iz) - exp(-iz)
  opY = allocateTemporaryRegister();
  reallocateRegister(opY, dtComplex34, COMPLEX34_SIZE, 0);
  complex34Copy(VARIABLE_COMPLEX34_DATA(&expIz),  REGISTER_COMPLEX34_DATA(opY));
  complex34Copy(VARIABLE_COMPLEX34_DATA(&expMIz), REGISTER_COMPLEX34_DATA(opX));
  subCo34Co34();
  complex34Copy(REGISTER_COMPLEX34_DATA(result), VARIABLE_COMPLEX34_DATA(&numer));

  // calculate exp(iz) + exp(-iz)
  addCo34Co34();

  // calculate (exp(iz) - exp(-iz)) / (exp(iz) + exp(-iz))
  complex34Copy(VARIABLE_COMPLEX34_DATA(&numer), REGISTER_COMPLEX34_DATA(opY));
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(opX));
  divCo34Co34();

  // calculate -i(exp(iz) - exp(-iz)) / (exp(iz) + exp(-iz))
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(opY));
  real34Copy(REGISTER_REAL34_DATA(opY), REGISTER_IMAG34_DATA(result));
  real34Copy(REGISTER_IMAG34_DATA(opY), REGISTER_REAL34_DATA(result));
  real34ChangeSign(REGISTER_IMAG34_DATA(result));

  freeTemporaryRegister(opY);
  convertRegister34To16(result);
}



void tanAngl(void) {
  if(angleIsNaN(REGISTER_ANGLE_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function tanAngl:", "cannot use NaN as an input of tan", NULL, NULL);
    #endif
    return;
  }

  real34_t cos;

  #if (ANGLE16 == 1)
    convertRegister16To34(opX);
  #endif
  reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
  WP34S_cvt_2rad_sincos(REGISTER_REAL34_DATA(result), &cos, REGISTER_REAL34_DATA(opX)); // opX in internal units
  if(real34IsZero(&cos)) {
    if(getFlag(FLAG_DANGER)) {
      real34Divide(REGISTER_REAL34_DATA(result), &cos, REGISTER_REAL34_DATA(result));
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function tanRe34:", "X = " STD_PLUS_MINUS "90" STD_DEGREE, NULL, NULL);
      #endif
    }
  }
  else {
   real34Divide(REGISTER_REAL34_DATA(result), &cos, REGISTER_REAL34_DATA(result));
  }
  convertRegister34To16(result);
}



void tanRm16(void) {
  tanToBeCoded();
}



void tanCm16(void) {
  tanToBeCoded();
}



void tanRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function tanRe34:", "cannot use NaN as an input of tan", NULL, NULL);
    #endif
    return;
  }

  if(real34IsSpecial(REGISTER_REAL34_DATA(opX))) {
    real34Copy(const34_NaN, REGISTER_REAL34_DATA(result));
  }
  else {
    real34_t cos;

    convertAngle34ToInternal(REGISTER_REAL34_DATA(opX), angularMode);
    WP34S_cvt_2rad_sincos(REGISTER_REAL34_DATA(result), &cos, REGISTER_REAL34_DATA(opX)); // opX in internal units
    if(real34IsZero(&cos)) {
      if(getFlag(FLAG_DANGER)) {
      	 real34Divide(REGISTER_REAL34_DATA(result), &cos, REGISTER_REAL34_DATA(result));
      }
      else {
        displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function tanRe34:", "X = " STD_PLUS_MINUS "90" STD_DEGREE, NULL, NULL);
        #endif
      }
    }
    else {
   	 real34Divide(REGISTER_REAL34_DATA(result), &cos, REGISTER_REAL34_DATA(result));
    }
  }
}



void tanCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function tanCo34:", "cannot use NaN as an input of tan", NULL, NULL);
    #endif
    return;
  }

  // tan(z) = -i(exp(iz) - exp(-iz)) / (exp(iz) + exp(-iz))
  complex34_t iz, expIz, expMIz, numer;

  // calculate iz
  real34Copy(REGISTER_REAL34_DATA(opX), VARIABLE_IMAG34_DATA(&iz));
  real34Copy(REGISTER_IMAG34_DATA(opX), VARIABLE_REAL34_DATA(&iz));
  real34ChangeSign(VARIABLE_REAL34_DATA(&iz));

  // calculate exp(iz)
  complex34Copy(VARIABLE_COMPLEX34_DATA(&iz), REGISTER_COMPLEX34_DATA(opX));
  expCo34();
  complex34Copy(REGISTER_COMPLEX34_DATA(result), VARIABLE_COMPLEX34_DATA(&expIz));

  // calculate exp(-iz)
  complex34ChangeSign(VARIABLE_COMPLEX34_DATA(&iz));
  complex34Copy(VARIABLE_COMPLEX34_DATA(&iz), REGISTER_COMPLEX34_DATA(opX));
  expCo34();
  complex34Copy(REGISTER_COMPLEX34_DATA(result), VARIABLE_COMPLEX34_DATA(&expMIz));

  // calculate exp(iz) - exp(-iz)
  opY = allocateTemporaryRegister();
  reallocateRegister(opY, dtComplex34, COMPLEX34_SIZE, 0);
  complex34Copy(VARIABLE_COMPLEX34_DATA(&expIz),  REGISTER_COMPLEX34_DATA(opY));
  complex34Copy(VARIABLE_COMPLEX34_DATA(&expMIz), REGISTER_COMPLEX34_DATA(opX));
  subCo34Co34();
  complex34Copy(REGISTER_COMPLEX34_DATA(result), VARIABLE_COMPLEX34_DATA(&numer));

  // calculate exp(iz) + exp(-iz)
  addCo34Co34();

  // calculate (exp(iz) - exp(-iz)) / (exp(iz) + exp(-iz))
  complex34Copy(VARIABLE_COMPLEX34_DATA(&numer),  REGISTER_COMPLEX34_DATA(opY));
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(opX));
  divCo34Co34();

  // calculate -i(exp(iz) - exp(-iz)) / (exp(iz) + exp(-iz))
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(opY));
  real34Copy(REGISTER_REAL34_DATA(opY), REGISTER_IMAG34_DATA(result));
  real34Copy(REGISTER_IMAG34_DATA(opY), REGISTER_REAL34_DATA(result));
  real34ChangeSign(REGISTER_IMAG34_DATA(result));

  freeTemporaryRegister(opY);
}
