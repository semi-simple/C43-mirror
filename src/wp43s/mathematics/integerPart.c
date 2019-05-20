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
 * \file integerPart.c
 ***********************************************/

#include "wp43s.h"



void (* const ip[13])(void) = {
// regX ==> 1            2       3         4       5        6        7        8          9           10            11      12        13
//          Long integer Real16  Complex16 Angle16 Time     Date     String   Real16 mat Complex16 m Short integer Real34  Complex34 Angle34
            ipLonI,      ipRe16, ipError,  ipRe16, ipError, ipError, ipError, ipRm16,    ipError,    ipShoI,       ipRe34, ipError,  ipRe34
};



/********************************************//**
 * \brief Data type error in IP
 *
 * \param void
 * \return void
 ***********************************************/
void ipError(void) {
  displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate IP for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnIp:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and IP(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnIp(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  result = REGISTER_X;
  opX    = allocateTemporaryRegister();
  copySourceRegisterToDestRegister(REGISTER_X, opX);

  ip[getRegisterDataType(REGISTER_X)]();
  freeTemporaryRegister(opX);

  if(lastErrorCode == 0) {
    refreshRegisterLine(REGISTER_X);
  }
  else {
    restoreStack();
    refreshStack();
  }
}



void ipLonI(void) {
}



void ipRe16(void) {
  real16ToIntegral(REGISTER_REAL16_DATA(opX), REGISTER_REAL16_DATA(result));
}



void ipRm16(void) {
  fnToBeCoded();
}



void ipRe34(void) {
  real34ToIntegral(REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



void ipShoI(void) {
}
