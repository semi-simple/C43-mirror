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
 * \file constants.c List of constant description
 * see: https://physics.nist.gov/cuu/index.html
 ***********************************************/

#include "wp43s.h"



/********************************************//**
 * \brief Replaces the X content with the selected
 * constant. Enables \b stack \b lift and refreshes the stack
 *
 * \param[in] cnst uint16_t Index of the constant to store in X
 * \return void
 ***********************************************/
void fnConstant(const uint16_t cnst) {
  liftStack();

  if(cnst < NUMBER_OF_CONSTANTS_39) { // 39 digit constants
    realToReal34(constants + cnst*REAL39_SIZE, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else if(cnst < NUMBER_OF_CONSTANTS_39 + NUMBER_OF_CONSTANTS_51) { // 51 digit constants (gamma coefficients)
    realToReal34(constants + NUMBER_OF_CONSTANTS_39 * REAL39_SIZE
                           + (cnst - NUMBER_OF_CONSTANTS_39) * REAL51_SIZE, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else if(cnst < NUMBER_OF_CONSTANTS_39 + NUMBER_OF_CONSTANTS_51 + NUMBER_OF_CONSTANTS_1071) { // 1071 digit constant
    realToReal34(constants + NUMBER_OF_CONSTANTS_39 * REAL39_SIZE + NUMBER_OF_CONSTANTS_51 * REAL51_SIZE
                           + (cnst - NUMBER_OF_CONSTANTS_39 - NUMBER_OF_CONSTANTS_51) * REAL1071_SIZE, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else { // 34 digit constants
    real34Copy(constants + NUMBER_OF_CONSTANTS_39 * REAL39_SIZE + NUMBER_OF_CONSTANTS_51 * REAL51_SIZE + NUMBER_OF_CONSTANTS_1071 * REAL1071_SIZE
                         + (cnst - NUMBER_OF_CONSTANTS_39 - NUMBER_OF_CONSTANTS_51 - NUMBER_OF_CONSTANTS_1071) * REAL34_SIZE, REGISTER_REAL34_DATA(REGISTER_X));
  }

  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);

  refreshRegisterLine(REGISTER_Y);
  refreshRegisterLine(REGISTER_Z);
  refreshRegisterLine(REGISTER_T);
}



/********************************************//**
 * \brief Replaces the X content with the circumference
 * to diameter ratio pi: 3,1415926. Enables \b stack
 * \b lift and refreshes the stack.
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnPi(uint16_t unusedParamButMandatory) {
  liftStack();

  realToReal34(const_pi, REGISTER_REAL34_DATA(REGISTER_X));
  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);

  refreshRegisterLine(REGISTER_Y);
  refreshRegisterLine(REGISTER_Z);
  refreshRegisterLine(REGISTER_T);
}
