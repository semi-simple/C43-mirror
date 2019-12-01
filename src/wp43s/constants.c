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
 * \param[in] cst uint16_t Index of the constant to store in X
 * \return void
 ***********************************************/
void fnConstant(const uint16_t cst) {
  liftStack();
  realToReal16((real_t *)constants + cst, REGISTER_REAL16_DATA(REGISTER_X));
  refreshStack();
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
  realToReal16(const_pi, REGISTER_REAL16_DATA(REGISTER_X));
  refreshStack();
}
