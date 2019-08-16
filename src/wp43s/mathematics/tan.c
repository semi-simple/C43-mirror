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



void (* const Tan[13])(void) = {
// regX ==> 1            2        3         4        5         6         7         8          9           10            11       12        13
//          Long integer Real16   Complex16 Angle16  Time      Date      String    Real16 mat Complex16 m Short integer Real34   Complex34 Angle34
            tanLonI,     tanRe16, tanCo16,  tanAn16, tanError, tanError, tanError, tanRm16,   tanCm16,    tanError,     tanRe34, tanCo34,  tanAn34
};



void longIntegerAngleReduction(calcRegister_t regist, uint8_t angularMode) {
  uint32_t oneTurn;

  switch(angularMode) {
    case AM_DEGREE:
    case AM_DMS:    oneTurn = 360; break;
    case AM_GRAD:   oneTurn = 400; break;
    case AM_MULTPI: oneTurn =   2; break;
    default:        oneTurn =   0;
  }

  if(oneTurn == 0) {
    convertLongIntegerRegisterToReal34Register(regist, regist);
  }
  else {
    longInteger_t angle;

    convertLongIntegerRegisterToLongInteger(regist, angle);
    reallocateRegister(regist, dtReal34, REAL34_SIZE, TAG_NONE);
    uInt32ToReal34(longIntegerModuloUInt(angle, oneTurn), REGISTER_REAL34_DATA(regist));
    longIntegerFree(angle);
  }
}



/********************************************//**
 * \brief Data type error in tan
 *
 * \param void
 * \return void
 ***********************************************/
void tanError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate Tan for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnTan:", errorMessage, NULL, NULL);
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
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  Tan[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



void tanLonI(void) {
  real34_t cos;

  longIntegerAngleReduction(REGISTER_X, currentAngularMode);

  WP34S_cvt_2rad_sincos(REGISTER_REAL34_DATA(REGISTER_X), &cos, REGISTER_REAL34_DATA(REGISTER_X), currentAngularMode);

  if(real34IsZero(&cos)) {
    if(getFlag(FLAG_DANGER)) {
      real34Divide(REGISTER_REAL34_DATA(REGISTER_X), &cos, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function tanLonI:", "X = " STD_PLUS_MINUS "90" STD_DEGREE, NULL, NULL);
      #endif
      return;
    }
  }
  else {
   real34Divide(REGISTER_REAL34_DATA(REGISTER_X), &cos, REGISTER_REAL34_DATA(REGISTER_X));
  }

  convertRegister34To16(REGISTER_X);
}



void tanRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function tanRe16:", "cannot use NaN as an input of tan", NULL, NULL);
    #endif
    return;
  }

  if(real16IsInfinite(REGISTER_REAL16_DATA(REGISTER_X))) {
    real16Copy(const16_NaN, REGISTER_REAL16_DATA(REGISTER_X));
  }
  else {
    real34_t cos;

    convertRegister16To34(REGISTER_X);
    WP34S_cvt_2rad_sincos(REGISTER_REAL34_DATA(REGISTER_X), &cos, REGISTER_REAL34_DATA(REGISTER_X), currentAngularMode);

    if(real34IsZero(&cos)) {
      if(getFlag(FLAG_DANGER)) {
      	 real34Divide(REGISTER_REAL34_DATA(REGISTER_X), &cos, REGISTER_REAL34_DATA(REGISTER_X));
      }
      else {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function tanRe16:", "X = " STD_PLUS_MINUS "90" STD_DEGREE, NULL, NULL);
        #endif
        return;
      }
    }
    else {
   	 real34Divide(REGISTER_REAL34_DATA(REGISTER_X), &cos, REGISTER_REAL34_DATA(REGISTER_X));
    }
    convertRegister34To16(REGISTER_X);
  }
}



void tanCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function tanCo16:", "cannot use NaN as an input of tan", NULL, NULL);
    #endif
    return;
  }

  // tan(z) = -i(exp(iz) - exp(-iz)) / (exp(iz) + exp(-iz))
  complex34_t iz, expIz, expMIz, numer;

  convertRegister16To34(REGISTER_X);

  // calculate iz
  real34Copy(REGISTER_REAL34_DATA(REGISTER_X), VARIABLE_IMAG34_DATA(&iz));
  real34Copy(REGISTER_IMAG34_DATA(REGISTER_X), VARIABLE_REAL34_DATA(&iz));
  real34ChangeSign(VARIABLE_REAL34_DATA(&iz));

  // calculate expIz = exp(iz)
  complex34Copy(VARIABLE_COMPLEX34_DATA(&iz), REGISTER_COMPLEX34_DATA(REGISTER_X));
  expCo34();
  complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_X), VARIABLE_COMPLEX34_DATA(&expIz));

  // calculate expMIz = exp(-iz)
  complex34ChangeSign(VARIABLE_COMPLEX34_DATA(&iz));
  complex34Copy(VARIABLE_COMPLEX34_DATA(&iz), REGISTER_COMPLEX34_DATA(REGISTER_X));
  expCo34();
  complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_X), VARIABLE_COMPLEX34_DATA(&expMIz));

  // calculate nummer =  exp(iz) - exp(-iz)
  real34Subtract(VARIABLE_REAL34_DATA(&expIz), VARIABLE_REAL34_DATA(&expMIz), VARIABLE_REAL34_DATA(&numer));
  real34Subtract(VARIABLE_IMAG34_DATA(&expIz), VARIABLE_IMAG34_DATA(&expMIz), VARIABLE_IMAG34_DATA(&numer));

  // calculate X = exp(iz) + exp(-iz)
  real34Add(VARIABLE_REAL34_DATA(&expIz), VARIABLE_REAL34_DATA(&expMIz), REGISTER_REAL34_DATA(REGISTER_X));
  real34Add(VARIABLE_IMAG34_DATA(&expIz), VARIABLE_IMAG34_DATA(&expMIz), REGISTER_IMAG34_DATA(REGISTER_X));

  // calculate X = (exp(iz) - exp(-iz)) / (exp(iz) + exp(-iz)) = numer / X    WARNING using iz, expIz and expMIz as temp variables below
    // Denominator = expMIz
    real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X), &expMIz);     // expMIz = c*c
    real34FMA(REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X), &expMIz, &expMIz); // expMIz = c*c + d*d

    // real part = iz
    real34Multiply(VARIABLE_REAL34_DATA(&numer), REGISTER_REAL34_DATA(REGISTER_X), &expIz);           // expIz = a*c
    real34FMA(VARIABLE_IMAG34_DATA(&numer), REGISTER_IMAG34_DATA(REGISTER_X), &expIz, &expIz);        // expIz = a*c + b*d
    real34Divide(&expIz, &expMIz, &iz);                                                              // iz = (a*c + b*d) / (c*c + d*d) = expIz / expMIz

    // imaginary part
    real34Multiply(VARIABLE_IMAG34_DATA(&numer), REGISTER_REAL34_DATA(REGISTER_X), &expIz);           // expIz = b*c
    real34ChangeSign(VARIABLE_REAL34_DATA(&numer)); // -a
    real34FMA(VARIABLE_REAL34_DATA(&numer), REGISTER_IMAG34_DATA(REGISTER_X), &expIz, &expIz);        // expIz = b*c - a*d
    real34Divide(&expIz, &expMIz, REGISTER_IMAG34_DATA(REGISTER_X));                                 // im(X) = (b*c - a*d) / (c*c + d*d) = expIz / expMIz

    // real part
    real34Copy(&iz, REGISTER_REAL34_DATA(REGISTER_X));                                               // re(X) = iz

  // calculate -i(exp(iz) - exp(-iz)) / (exp(iz) + exp(-iz))     WARNING using iz as a temp variable below
  real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &iz);
  real34Copy(REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
  real34ChangeSign(&iz);
  real34Copy(&iz, REGISTER_IMAG34_DATA(REGISTER_X));

  convertRegister34To16(REGISTER_X);
}



void tanAn16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function tanAn16:", "cannot use NaN as an input of tan", NULL, NULL);
    #endif
    return;
  }

  if(real16IsInfinite(REGISTER_REAL16_DATA(REGISTER_X))) {
    real16Copy(const16_NaN, REGISTER_REAL16_DATA(REGISTER_X));
  }
  else {
    real34_t cos;

    convertRegister16To34(REGISTER_X);
    WP34S_cvt_2rad_sincos(REGISTER_REAL34_DATA(REGISTER_X), &cos, REGISTER_REAL34_DATA(REGISTER_X), getRegisterAngularMode(REGISTER_X));

    if(real34IsZero(&cos)) {
      if(getFlag(FLAG_DANGER)) {
      	 real34Divide(REGISTER_REAL34_DATA(REGISTER_X), &cos, REGISTER_REAL34_DATA(REGISTER_X));
      }
      else {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function tanAn16:", "X = " STD_PLUS_MINUS "90" STD_DEGREE, NULL, NULL);
        #endif
        return;
      }
    }
    else {
   	 real34Divide(REGISTER_REAL34_DATA(REGISTER_X), &cos, REGISTER_REAL34_DATA(REGISTER_X));
    }
    convertRegister34To16(REGISTER_X);
  }

  setRegisterDataType(REGISTER_X, dtReal16, TAG_NONE);
}



void tanRm16(void) {
  fnToBeCoded();
}



void tanCm16(void) {
  fnToBeCoded();
}



void tanRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function tanRe34:", "cannot use NaN as an input of tan", NULL, NULL);
    #endif
    return;
  }

  if(real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_X))) {
    real34Copy(const34_NaN, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    real34_t cos;

    WP34S_cvt_2rad_sincos(REGISTER_REAL34_DATA(REGISTER_X), &cos, REGISTER_REAL34_DATA(REGISTER_X), currentAngularMode);

    if(real34IsZero(&cos)) {
      if(getFlag(FLAG_DANGER)) {
      	 real34Divide(REGISTER_REAL34_DATA(REGISTER_X), &cos, REGISTER_REAL34_DATA(REGISTER_X));
      }
      else {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function tanRe34:", "X = " STD_PLUS_MINUS "90" STD_DEGREE, NULL, NULL);
        #endif
      }
    }
    else {
   	 real34Divide(REGISTER_REAL34_DATA(REGISTER_X), &cos, REGISTER_REAL34_DATA(REGISTER_X));
    }
  }
}



void tanCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function tanCo34:", "cannot use NaN as an input of tan", NULL, NULL);
    #endif
    return;
  }

  // tan(z) = -i(exp(iz) - exp(-iz)) / (exp(iz) + exp(-iz))
  complex34_t iz, expIz, expMIz, numer;

  // calculate iz
  real34Copy(REGISTER_REAL34_DATA(REGISTER_X), VARIABLE_IMAG34_DATA(&iz));
  real34Copy(REGISTER_IMAG34_DATA(REGISTER_X), VARIABLE_REAL34_DATA(&iz));
  real34ChangeSign(VARIABLE_REAL34_DATA(&iz));

  // calculate expIz = exp(iz)
  complex34Copy(VARIABLE_COMPLEX34_DATA(&iz), REGISTER_COMPLEX34_DATA(REGISTER_X));
  expCo34();
  complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_X), VARIABLE_COMPLEX34_DATA(&expIz));

  // calculate expMIz = exp(-iz)
  complex34ChangeSign(VARIABLE_COMPLEX34_DATA(&iz));
  complex34Copy(VARIABLE_COMPLEX34_DATA(&iz), REGISTER_COMPLEX34_DATA(REGISTER_X));
  expCo34();
  complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_X), VARIABLE_COMPLEX34_DATA(&expMIz));

  // calculate nummer =  exp(iz) - exp(-iz)
  real34Subtract(VARIABLE_REAL34_DATA(&expIz), VARIABLE_REAL34_DATA(&expMIz), VARIABLE_REAL34_DATA(&numer));
  real34Subtract(VARIABLE_IMAG34_DATA(&expIz), VARIABLE_IMAG34_DATA(&expMIz), VARIABLE_IMAG34_DATA(&numer));

  // calculate X = exp(iz) + exp(-iz)
  real34Add(VARIABLE_REAL34_DATA(&expIz), VARIABLE_REAL34_DATA(&expMIz), REGISTER_REAL34_DATA(REGISTER_X));
  real34Add(VARIABLE_IMAG34_DATA(&expIz), VARIABLE_IMAG34_DATA(&expMIz), REGISTER_IMAG34_DATA(REGISTER_X));

  // calculate X = (exp(iz) - exp(-iz)) / (exp(iz) + exp(-iz)) = numer / X    WARNING using iz, expIz and expMIz as temp variables below
    // Denominator = expMIz
    real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X), &expMIz);     // expMIz = c*c
    real34FMA(REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X), &expMIz, &expMIz); // expMIz = c*c + d*d

    // real part = iz
    real34Multiply(VARIABLE_REAL34_DATA(&numer), REGISTER_REAL34_DATA(REGISTER_X), &expIz);           // expIz = a*c
    real34FMA(VARIABLE_IMAG34_DATA(&numer), REGISTER_IMAG34_DATA(REGISTER_X), &expIz, &expIz);        // expIz = a*c + b*d
    real34Divide(&expIz, &expMIz, &iz);                                                              // iz = (a*c + b*d) / (c*c + d*d) = expIz / expMIz

    // imaginary part
    real34Multiply(VARIABLE_IMAG34_DATA(&numer), REGISTER_REAL34_DATA(REGISTER_X), &expIz);           // expIz = b*c
    real34ChangeSign(VARIABLE_REAL34_DATA(&numer)); // -a
    real34FMA(VARIABLE_REAL34_DATA(&numer), REGISTER_IMAG34_DATA(REGISTER_X), &expIz, &expIz);        // expIz = b*c - a*d
    real34Divide(&expIz, &expMIz, REGISTER_IMAG34_DATA(REGISTER_X));                                 // im(X) = (b*c - a*d) / (c*c + d*d) = expIz / expMIz

    // real part
    real34Copy(&iz, REGISTER_REAL34_DATA(REGISTER_X));                                               // re(X) = iz

  // calculate -i(exp(iz) - exp(-iz)) / (exp(iz) + exp(-iz))     WARNING using iz as a temp variable below
  real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &iz);
  real34Copy(REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
  real34ChangeSign(&iz);
  real34Copy(&iz, REGISTER_IMAG34_DATA(REGISTER_X));
}



void tanAn34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function tanAn34:", "cannot use NaN as an input of tan", NULL, NULL);
    #endif
    return;
  }

  if(real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_X))) {
    real34Copy(const34_NaN, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    real34_t cos;

    WP34S_cvt_2rad_sincos(REGISTER_REAL34_DATA(REGISTER_X), &cos, REGISTER_REAL34_DATA(REGISTER_X), getRegisterAngularMode(REGISTER_X));

    if(real34IsZero(&cos)) {
      if(getFlag(FLAG_DANGER)) {
      	 real34Divide(REGISTER_REAL34_DATA(REGISTER_X), &cos, REGISTER_REAL34_DATA(REGISTER_X));
      }
      else {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function tanAn34:", "X = " STD_PLUS_MINUS "90" STD_DEGREE, NULL, NULL);
        #endif
      }
    }
    else {
   	 real34Divide(REGISTER_REAL34_DATA(REGISTER_X), &cos, REGISTER_REAL34_DATA(REGISTER_X));
    }
  }

  setRegisterDataType(REGISTER_X, dtReal34, TAG_NONE);
}
