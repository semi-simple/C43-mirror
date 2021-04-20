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
 * \file cxToRe.c
 ***********************************************/

#include "mathematics/cxToRe.h"

#include "debug.h"
#include "error.h"
#include "conversionAngles.h"
#include "flags.h"
#include "mathematics/toPolar.h"
#include "registers.h"
#include "stack.h"

#include "wp43s.h"


/********************************************//**
 * \brief regX ==> regL and re(regX) ==> regY, im(regX) ==> regX or magnitude(regX) ==> regY, angle(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnCxToRe(uint16_t unusedButMandatoryParameter) {
  uint32_t dataTypeX = getRegisterDataType(REGISTER_X);

  if(dataTypeX == dtComplex34) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);

    setSystemFlag(FLAG_ASLIFT);
    if(getSystemFlag(FLAG_POLAR)) { // polar mode
      liftStack();
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
      real34RectangularToPolar(REGISTER_REAL34_DATA(REGISTER_L), REGISTER_IMAG34_DATA(REGISTER_L), REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X)); // X in radians
      convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), amRadian, currentAngularMode);
      setRegisterAngularMode(REGISTER_X, currentAngularMode);
      temporaryInformation = TI_THETA_RADIUS;
    }
    else { // rectangular mode
      real34Copy(REGISTER_REAL34_DATA(REGISTER_L), REGISTER_REAL34_DATA(REGISTER_X));
      liftStack();
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
      real34Copy(REGISTER_IMAG34_DATA(REGISTER_L), REGISTER_REAL34_DATA(REGISTER_X));
      temporaryInformation = TI_RE_IM;
    }
  }

  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X); // Invalid input data type for this operation
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "You cannot use Cx->Re with %s in X!", getDataTypeName(getRegisterDataType(REGISTER_X), true, false));
      moreInfoOnError("In function fnCxToRe:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
}
