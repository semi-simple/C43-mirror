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



void longIntegerAngleReduction(calcRegister_t regist, uint8_t angularMode, realIc_t *reducedAngle) {
  uint32_t oneTurn;

  switch(angularMode) {
    case AM_DEGREE:
    case AM_DMS:    oneTurn = 360; break;
    case AM_GRAD:   oneTurn = 400; break;
    case AM_MULTPI: oneTurn =   2; break;
    default:        oneTurn =   0;
  }

  if(oneTurn == 0) {
    convertLongIntegerRegisterToRealIc(regist, reducedAngle);
  }
  else {
    longInteger_t angle;

    convertLongIntegerRegisterToLongInteger(regist, angle);
    uInt32ToRealIc(longIntegerModuloUInt(angle, oneTurn), reducedAngle);
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



void tanCoIc(const complexIc_t *zin, complexIc_t *zout) {
  //                sin(a)*cosh(b) + i*cos(a)*sinh(b)
  // tan(a + ib) = -----------------------------------
  //                cos(a)*cosh(b) - i*sin(a)*sinh(b)
  realIc_t sina, cosa, sinhb, coshb;
  complexIc_t numer, denom;

  WP34S_Cvt2RadSinCosTan(&zin->real, AM_RADIAN, &sina, &cosa, NULL);
  WP34S_SinhCosh(&zin->imag, &sinhb, &coshb);

  realIcMultiply(&sina, &coshb, &numer.real);
  realIcMultiply(&cosa, &sinhb, &numer.imag);

  realIcMultiply(&cosa, &coshb, &denom.real);
  realIcMultiply(&sina, &sinhb, &denom.imag);
  realIcChangeSign(&denom.imag);

  divCoIcCoIc(&numer, &denom, zout);
}



void tanLonI(void) {
  realIc_t sin, cos, tan;

  longIntegerAngleReduction(REGISTER_X, currentAngularMode, &tan);
  WP34S_Cvt2RadSinCosTan(&tan, currentAngularMode, &sin, &cos, &tan);

  if(realIcIsZero(&cos) && !getFlag(FLAG_DANGER)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function tanLonI:", "X = " STD_PLUS_MINUS "90" STD_DEGREE, NULL, NULL);
    #endif
    return;
  }

  reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, TAG_NONE);
  realIcToReal16(&tan, REGISTER_REAL16_DATA(REGISTER_X));
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
    realIcToReal16(const_NaN, REGISTER_REAL16_DATA(REGISTER_X));
  }
  else {
    realIc_t sin, cos, tan;

    real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &tan);
    WP34S_Cvt2RadSinCosTan(&tan, currentAngularMode, &sin, &cos, &tan);

    if(realIcIsZero(&cos) && !getFlag(FLAG_DANGER)) {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function tanRe16:", "X = " STD_PLUS_MINUS "90" STD_DEGREE, NULL, NULL);
      #endif
      return;
    }
    else {
      realIcToReal16(&tan, REGISTER_REAL16_DATA(REGISTER_X));
    }
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

  complexIc_t z;

  real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &z.real);
  real16ToRealIc(REGISTER_IMAG16_DATA(REGISTER_X), &z.imag);

  tanCoIc(&z, &z);

  realIcToReal16(&z.real, REGISTER_REAL16_DATA(REGISTER_X));
  realIcToReal16(&z.imag, REGISTER_IMAG16_DATA(REGISTER_X));
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
    realIcToReal16(const_NaN, REGISTER_REAL16_DATA(REGISTER_X));
  }
  else {
    realIc_t sin, cos, tan;

    real16ToRealIc(REGISTER_REAL16_DATA(REGISTER_X), &tan);
    WP34S_Cvt2RadSinCosTan(&tan, getRegisterAngularMode(REGISTER_X), &sin, &cos, &tan);

    if(realIcIsZero(&cos) && !getFlag(FLAG_DANGER)) {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function tanAn16:", "X = " STD_PLUS_MINUS "90" STD_DEGREE, NULL, NULL);
      #endif
      return;
    }
    else {
      realIcToReal16(&tan, REGISTER_REAL16_DATA(REGISTER_X));
    }
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
    realIcToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    realIc_t sin, cos, tan;

    real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &tan);
    WP34S_Cvt2RadSinCosTan(&tan, currentAngularMode, &sin, &cos, &tan);

    if(realIcIsZero(&cos) && !getFlag(FLAG_DANGER)) {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function tanRe34:", "X = " STD_PLUS_MINUS "90" STD_DEGREE, NULL, NULL);
      #endif
      return;
    }
    else {
      realIcToReal34(&tan, REGISTER_REAL34_DATA(REGISTER_X));
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

  complexIc_t z;

  real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &z.real);
  real34ToRealIc(REGISTER_IMAG34_DATA(REGISTER_X), &z.imag);

  tanCoIc(&z, &z);

  realIcToReal34(&z.real, REGISTER_REAL34_DATA(REGISTER_X));
  realIcToReal34(&z.imag, REGISTER_IMAG34_DATA(REGISTER_X));
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
    realIcToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    realIc_t sin, cos, tan;

    real34ToRealIc(REGISTER_REAL34_DATA(REGISTER_X), &tan);
    WP34S_Cvt2RadSinCosTan(&tan, getRegisterAngularMode(REGISTER_X), &sin, &cos, &tan);

    if(realIcIsZero(&cos) && !getFlag(FLAG_DANGER)) {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function tanAn34:", "X = " STD_PLUS_MINUS "90" STD_DEGREE, NULL, NULL);
      #endif
      return;
    }
    else {
      realIcToReal34(&tan, REGISTER_REAL34_DATA(REGISTER_X));
    }
  }

  setRegisterDataType(REGISTER_X, dtReal34, TAG_NONE);
}
