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
 * \file arccos.c
 ***********************************************/

#include "wp43s.h"



void (* const arccos[13])(void) = {
// regX ==> 1            2           3           4           5            6            7            8           9           10            11          12          13
//          Long integer Real16      Complex16   Angle16     Time         Date         String       Real16 mat  Complex16 m Short integer Real34      Complex34   Angle34
            arccosLonI,  arccosRe16, arccosCo16, arccosRe16, arccosError, arccosError, arccosError, arccosRm16, arccosCm16, arccosError,  arccosRe34, arccosCo34, arccosRe34
};



/********************************************//**
 * \brief Data type error in arccos
 *
 * \param void
 * \return void
 ***********************************************/
void arccosError(void) {
  displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate arccos for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnArccos:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and arccos(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnArccos(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  result = REGISTER_X;
  opX    = allocateTemporaryRegister();
  copySourceRegisterToDestRegister(REGISTER_X, opX);

  arccos[getRegisterDataType(REGISTER_X)]();

  adjustResult(result, false, true, opX, -1, -1);
}



void arccosLonI(void) {
 convertLongIntegerRegisterToReal16Register(opX, opX);
  if(real16CompareAbsGreaterThan(REGISTER_REAL16_DATA(opX), const16_1)) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function arccosLonI:", "|X| > 1", NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(result, dtReal16, REAL16_SIZE, currentAngularMode);

  if(real16IsZero(REGISTER_REAL16_DATA(opX))) {
    real16Copy(const16_0_5, REGISTER_REAL16_DATA(result));
    convertAngle16FromTo(REGISTER_REAL16_DATA(result), AM_MULTPI, currentAngularMode);
  }
  else {
    if(real16IsNegative(REGISTER_REAL16_DATA(opX))) {
      real16Copy(const16_1, REGISTER_REAL16_DATA(result));
      convertAngle16FromTo(REGISTER_REAL16_DATA(result), AM_MULTPI, currentAngularMode);
    }
    else{
      real16Zero(REGISTER_REAL16_DATA(result));
    }
  }

  setRegisterDataType(result, dtAngle16, currentAngularMode);
}



void arccosRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function arccosRe16:", "cannot use NaN as an input of arccos", NULL, NULL);
    #endif
    return;
  }

  if(real16CompareAbsGreaterThan(REGISTER_REAL16_DATA(opX), const16_1)) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function arccosRe16:", "|X| > 1", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opX);
  reallocateRegister(result, dtReal34, REAL34_SIZE, currentAngularMode);
  WP34S_do_acos(REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  convertAngle34FromTo(REGISTER_REAL34_DATA(result), AM_RADIAN, currentAngularMode);
  convertRegister34To16(result);
  setRegisterDataType(result, dtAngle16, currentAngularMode);

  if(currentAngularMode == AM_DMS) {
    checkDms16(REGISTER_REAL16_DATA(result));
  }
}



void arccosCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function arccosCo16:", "cannot use NaN as an input of arccos", NULL, NULL);
    #endif
    return;
  }

  // arccos(z) = -i.ln(z + sqtr(z*z - 1))
  complex34_t z;

  convertRegister16To34(opX);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, TAG_NONE);

  // store z
  complex34Copy(REGISTER_COMPLEX34_DATA(opX), &z);

  // calculate z*z
  opY = allocateTemporaryRegister();
  reallocateRegister(opY, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  complex34Copy(REGISTER_COMPLEX34_DATA(opX), REGISTER_COMPLEX34_DATA(opY));
  mulCo34Co34();

  // calculate z*z - 1
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(opY));
  real34Copy(const34_1, REGISTER_REAL34_DATA(opX));
  real34Zero(REGISTER_IMAG34_DATA(opX));
  subCo34Co34();

  // calculate sqrt(z*z - 1)
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(opX));
  sqrtCo34();

  // calculate z + sqrt(z*z - 1)
  complex34Copy(VARIABLE_COMPLEX34_DATA(&z), REGISTER_COMPLEX34_DATA(opY));
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(opX));
  addCo34Co34();
  freeTemporaryRegister(opY);

  // calculate ln(z + sqtr(1 - z*z))
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(opX));
  lnCo34();

  // calculate = -i.ln(iz + sqtr(1 - z*z))
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(opX));
  real34Copy(REGISTER_REAL34_DATA(opX), REGISTER_IMAG34_DATA(result));
  real34Copy(REGISTER_IMAG34_DATA(opX), REGISTER_REAL34_DATA(result));
  real34ChangeSign(REGISTER_IMAG34_DATA(result));

  convertRegister34To16(result);
}



void arccosRm16(void) {
  fnToBeCoded();
}



void arccosCm16(void) {
  fnToBeCoded();
}



void arccosRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function arccosRe34:", "cannot use NaN as an input of arccos", NULL, NULL);
    #endif
    return;
  }

  if(real34CompareAbsGreaterThan(REGISTER_REAL34_DATA(opX), const34_1)) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function arccosRe34:", "|X| > 1", NULL, NULL);
    #endif
    return;
  }

  WP34S_do_acos(REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  convertAngle34FromTo(REGISTER_REAL34_DATA(result), AM_RADIAN, currentAngularMode);
  setRegisterTag(result, currentAngularMode);
  setRegisterDataType(result, dtAngle34, currentAngularMode);

  if(currentAngularMode == AM_DMS) {
    checkDms34(REGISTER_REAL34_DATA(result));
  }
}



void arccosCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function arccosCo34:", "cannot use NaN as an input of arccos", NULL, NULL);
    #endif
    return;
  }

  // arccos(z) = -i.ln(z + sqtr(z*z - 1))
  complex34_t z;

  // store z
  complex34Copy(REGISTER_COMPLEX34_DATA(opX), &z);

  // calculate z*z
  opY = allocateTemporaryRegister();
  reallocateRegister(opY, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
  complex34Copy(REGISTER_COMPLEX34_DATA(opX), REGISTER_COMPLEX34_DATA(opY));
  mulCo34Co34();

  // calculate z*z - 1
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(opY));
  real34Copy(const34_1, REGISTER_REAL34_DATA(opX));
  real34Zero(REGISTER_IMAG34_DATA(opX));
  subCo34Co34();

  // calculate sqrt(1 - z*z)
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(opX));
  sqrtCo34();

  // calculate z + sqrt(z*z - 1)
  complex34Copy(VARIABLE_COMPLEX34_DATA(&z), REGISTER_COMPLEX34_DATA(opY));
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(opX));
  addCo34Co34();
  freeTemporaryRegister(opY);

  // calculate ln(iz + sqtr(1 - z*z))
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(opX));
  lnCo34();

  // calculate = -i.ln(iz + sqtr(1 - z*z))
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(opX));
  real34Copy(REGISTER_REAL34_DATA(opX), REGISTER_IMAG34_DATA(result));
  real34Copy(REGISTER_IMAG34_DATA(opX), REGISTER_REAL34_DATA(result));
  real34ChangeSign(REGISTER_IMAG34_DATA(result));
}
