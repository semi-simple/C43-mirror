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
 * \file reToCx.c
 ***********************************************/

#include "wp43s.h"


/********************************************//**
 * \brief regX ==> regL and regY+i*regX ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnReToCx(uint16_t unusedParamButMandatory) {
  uint32_t dataTypeX = getRegisterDataType(REGISTER_X);
  uint32_t dataTypeY = getRegisterDataType(REGISTER_Y);
  bool_t xIsAReal;

  if(    (dataTypeX == dtReal16 || dataTypeX == dtReal34 || dataTypeX == dtLongInteger)
      && (dataTypeY == dtReal16 || dataTypeY == dtReal34 || dataTypeY == dtLongInteger)) {

    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    fnSetFlag(FLAG_CPXRES);

    xIsAReal = true;
    if(complexMode == CM_POLAR) {
      if(dataTypeX == dtReal16 && getRegisterAngularMode(REGISTER_X) != AM_NONE) {
        convertAngle16FromTo(REGISTER_REAL16_DATA(REGISTER_X), getRegisterAngularMode(REGISTER_X), AM_RADIAN);
        setRegisterAngularMode(REGISTER_X, AM_NONE);
        dataTypeX = dtReal16;
        xIsAReal = false;
      }
      else if(dataTypeX == dtReal34 && getRegisterAngularMode(REGISTER_X) != AM_NONE) {
        convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), getRegisterAngularMode(REGISTER_X), AM_RADIAN);
        setRegisterAngularMode(REGISTER_X, AM_NONE);
        dataTypeX = dtReal34;
        xIsAReal = false;
      }
    }

    if(dataTypeX == dtLongInteger) {
      if(dataTypeY == dtReal16 || dataTypeY == dtLongInteger) {
        convertLongIntegerRegisterToReal16Register(REGISTER_X, REGISTER_X);
        dataTypeX = dtReal16;
      }
      else { // dataTypeY == dtReal34
        convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
        dataTypeX = dtReal34;
      }
    }

    if(dataTypeY == dtLongInteger) {
      if(dataTypeX == dtReal16) {
        convertLongIntegerRegisterToReal16Register(REGISTER_Y, REGISTER_Y);
        dataTypeY = dtReal16;
      }
      else { // dataTypeX == dtReal34
        convertLongIntegerRegisterToReal34Register(REGISTER_Y, REGISTER_Y);
        dataTypeY = dtReal34;
      }
    }

    if(dataTypeX == dtReal16 && dataTypeY == dtReal34) {
      real34_t temp;

      real16ToReal34(REGISTER_REAL16_DATA(REGISTER_X), &temp);
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
      real34Copy(&temp, REGISTER_REAL34_DATA(REGISTER_X));
      dataTypeX = dtReal34;
    }

    if(dataTypeY == dtReal16 && dataTypeX == dtReal34) {
      real34_t temp;

      real16ToReal34(REGISTER_REAL16_DATA(REGISTER_Y), &temp);
      reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, AM_NONE);
      real34Copy(&temp, REGISTER_REAL34_DATA(REGISTER_Y));
    }

    if(dataTypeX == dtReal16) {
      complex16_t temp;

      real16Copy(REGISTER_REAL16_DATA(REGISTER_Y), &temp);
      real16Copy(REGISTER_REAL16_DATA(REGISTER_X), VARIABLE_IMAG16_DATA(&temp));
      reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);

      if(complexMode == CM_POLAR) {
        if(real16CompareEqual(VARIABLE_REAL16_DATA(&temp), const16_0)) {
          real16Zero(VARIABLE_IMAG16_DATA(&temp));
        }
        else {
          real16_t magnitude16, theta16;

          real16Copy(VARIABLE_REAL16_DATA(&temp), &magnitude16);
          real16Copy(VARIABLE_IMAG16_DATA(&temp), &theta16);
          if(xIsAReal) {
            convertAngle16FromTo(&theta16, currentAngularMode, AM_RADIAN);
          }
          if(real16CompareLessThan(&magnitude16, const16_0)) {
            real16SetPositiveSign(&magnitude16);
            real16Add(&theta16, const_pi, &theta16);
            real16DivideRemainder(&theta16, const_2pi, &theta16);
          }
          real16PolarToRectangular(&magnitude16, &theta16, REGISTER_REAL16_DATA(REGISTER_X), REGISTER_IMAG16_DATA(REGISTER_X)); // theta16 in radian
        }
      }
      else {
        complex16Copy(&temp, REGISTER_COMPLEX16_DATA(REGISTER_X));
      }
    }
    else { //dataTypeX == dtReal34
      complex34_t temp;

      real34Copy(REGISTER_REAL34_DATA(REGISTER_Y), &temp);
      real34Copy(REGISTER_REAL34_DATA(REGISTER_X), VARIABLE_IMAG34_DATA(&temp));
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);

      if(complexMode == CM_POLAR) {
        if(real34CompareEqual(VARIABLE_REAL34_DATA(&temp), const34_0)) {
          real34Zero(VARIABLE_IMAG34_DATA(&temp));
        }
        else {
          real34_t magnitude34, theta34;

          real34Copy(VARIABLE_REAL34_DATA(&temp), &magnitude34);
          real34Copy(VARIABLE_IMAG34_DATA(&temp), &theta34);
          if(xIsAReal) {
            convertAngle34FromTo(&theta34, currentAngularMode, AM_RADIAN);
          }
          if(real34CompareLessThan(&magnitude34, const34_0)) {
            real34SetPositiveSign(&magnitude34);
            real34Add(&theta34, const_pi, &theta34);
            real34DivideRemainder(&theta34, const34_2pi, &theta34);
          }
          real34PolarToRectangular(&magnitude34, &theta34, REGISTER_REAL34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X)); // theta34 in radian
        }
      }
      else {
        complex34Copy(&temp, REGISTER_COMPLEX34_DATA(REGISTER_X));
      }
    }

    fnDropY(NOPARAM);
    STACK_LIFT_ENABLE;
    refreshStack();
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X); // Invalid input data type for this operation
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "You cannot use Re->Cx with %s in X and %s in Y!", getDataTypeName(getRegisterDataType(REGISTER_X), true, false), getDataTypeName(getRegisterDataType(REGISTER_Y), true, false));
      showInfoDialog("In function fnReToCx:", errorMessage, NULL, NULL);
    #endif
  }
}
