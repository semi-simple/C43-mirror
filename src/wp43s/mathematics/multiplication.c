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
 * \file multiplication.c
 ***********************************************/

#include "wp43s.h"



void (* const multiplication[9][9])(void) = {
// regX |    regY ==>   1            2            3            4            5         6         7            8            9
//      V               Long integer Real34       Complex34    Time         Date      String    Real34 mat   Complex34 m  Short integer
/*  1 Long integer  */ {mulLonILonI, mulRealLonI, mulCplxLonI, mulTimeLonI, mulError, mulError, mulRemaLonI, mulCxmaLonI, mulShoILonI},
/*  2 Real34        */ {mulLonIReal, mulRealReal, mulCplxReal, mulTimeReal, mulError, mulError, mulRemaReal, mulCxmaReal, mulShoIReal},
/*  3 Complex34     */ {mulLonICplx, mulRealCplx, mulCplxCplx, mulError,    mulError, mulError, mulRemaCplx, mulCxmaCplx, mulShoICplx},
/*  4 Time          */ {mulLonITime, mulRealTime, mulError,    mulError,    mulError, mulError, mulError,    mulError,    mulShoITime},
/*  5 Date          */ {mulError,    mulError,    mulError,    mulError,    mulError, mulError, mulError,    mulError,    mulError   },
/*  6 String        */ {mulError,    mulError,    mulError,    mulError,    mulError, mulError, mulError,    mulError,    mulError   },
/*  7 Real34 mat    */ {mulLonIRema, mulRealRema, mulCplxRema, mulError,    mulError, mulError, mulRemaRema, mulCxmaRema, mulShoIRema},
/*  8 Complex34 mat */ {mulLonICxma, mulRealCxma, mulCplxCxma, mulError,    mulError, mulError, mulRemaCxma, mulCxmaCxma, mulShoICxma},
/*  9 Short integer */ {mulLonIShoI, mulRealShoI, mulCplxShoI, mulTimeShoI, mulError, mulError, mulRemaShoI, mulCxmaShoI, mulShoIShoI}
};



/********************************************//**
 * \brief Data type error in multiplication
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void mulError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot multiply %s", getRegisterDataTypeName(REGISTER_Y, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "by %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnMultiply:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and regY × regX ==> regX
 * Drops Y, enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void fnMultiply(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  multiplication[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

  adjustResult(REGISTER_X, true, true, REGISTER_X, REGISTER_Y, -1);
}



void mulCo39Co39(const real39_t *factor1Real, const real39_t *factor1Imag, const real39_t *factor2Real, const real39_t *factor2Imag, real39_t *productReal, real39_t *productImag) {
  real39_t a, b, c, d;

  realCopy(factor1Real, &a);
  realCopy(factor1Imag, &b);
  realCopy(factor2Real, &c);
  realCopy(factor2Imag, &d);

  // imaginary part
  realMultiply(&a, &d, productImag, &ctxtReal39);         // a*d
  realFMA(&b, &c, productImag, productImag, &ctxtReal39); // a*d + b*c

  // real part
  realChangeSign(&b);                                     // -b
  realMultiply(&a, &c, productReal, &ctxtReal39);         // a*c
  realFMA(&b, &d, productReal, productReal, &ctxtReal39); // a*c - b*d
}



void mulCo51Co51(const real51_t *factor1Real, const real51_t *factor1Imag, const real51_t *factor2Real, const real51_t *factor2Imag, real51_t *productReal, real51_t *productImag) {
  real51_t a, b, c, d;

  realCopy(factor1Real, &a);
  realCopy(factor1Imag, &b);
  realCopy(factor2Real, &c);
  realCopy(factor2Imag, &d);

  // imaginary part
  realMultiply(&a, &d, productImag, &ctxtReal51);         // a*d
  realFMA(&b, &c, productImag, productImag, &ctxtReal51); // a*d + b*c

  // real part
  realChangeSign(&b);                                     // -b
  realMultiply(&a, &c, productReal, &ctxtReal51);         // a*c
  realFMA(&b, &d, productReal, productReal, &ctxtReal51); // a*c - b*d
}



void mulCo75Co75(const real75_t *factor1Real, const real75_t *factor1Imag, const real75_t *factor2Real, const real75_t *factor2Imag, real75_t *productReal, real75_t *productImag) {
  real75_t a, b, c, d;

  realCopy(factor1Real, &a);
  realCopy(factor1Imag, &b);
  realCopy(factor2Real, &c);
  realCopy(factor2Imag, &d);

  // imaginary part
  realMultiply(&a, &d, productImag, &ctxtReal75);         // a*d
  realFMA(&b, &c, productImag, productImag, &ctxtReal75); // a*d + b*c

  // real part
  realChangeSign(&b);                                     // -b
  realMultiply(&a, &c, productReal, &ctxtReal75);         // a*c
  realFMA(&b, &d, productReal, productReal, &ctxtReal75); // a*c - b*d
}



/******************************************************************************************************************************************************************************************/
/* long integer × ...                                                                                                                                                                     */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(long integer) × X(long integer) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void mulLonILonI(void) {
  longInteger_t y, x;

  convertLongIntegerRegisterToLongInteger(REGISTER_Y, y);
  convertLongIntegerRegisterToLongInteger(REGISTER_X, x);

  longIntegerMultiply(y, x, x);

  convertLongIntegerToLongIntegerRegister(x, REGISTER_X);

  longIntegerFree(y);
  longIntegerFree(x);
}



/********************************************//**
 * \brief Y(long integer) × X(time) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void mulLonITime(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(time) × X(long integer) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void mulTimeLonI(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(long integer) × X(real16 matrix) ==> X(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulLonIRema(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real16 matrix) × X(long integer) ==> X(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRemaLonI(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(long integer) × X(complex16 matrix) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulLonICxma(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(complex16 matrix) × X(long integer) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCxmaLonI(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(long integer) × X(short integer) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void mulLonIShoI(void) {
  longInteger_t y, x;

  convertShortIntegerRegisterToLongIntegerRegister(REGISTER_X, REGISTER_X);

  convertLongIntegerRegisterToLongInteger(REGISTER_Y, y);
  convertLongIntegerRegisterToLongInteger(REGISTER_X, x);

  longIntegerMultiply(y, x, x);

  convertLongIntegerToLongIntegerRegister(x, REGISTER_X);

  longIntegerFree(y);
  longIntegerFree(x);
}



/********************************************//**
 * \brief Y(short integer) × X(long integer) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void mulShoILonI(void) {
  longInteger_t y, x;

  convertShortIntegerRegisterToLongInteger(REGISTER_Y, y);
  convertLongIntegerRegisterToLongInteger(REGISTER_X, x);

  longIntegerMultiply(y, x, x);

  convertLongIntegerToLongIntegerRegister(x, REGISTER_X);

  longIntegerFree(y);
  longIntegerFree(x);
}



/********************************************//**
 * \brief Y(long integer) × X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulLonIReal(void) {
  real39_t y, x;
  uint32_t xAngularMode;

  convertLongIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
  xAngularMode = getRegisterAngularMode(REGISTER_X);

  if(xAngularMode == AM_NONE) {
    realMultiply(&y, &x, &x, &ctxtReal39);
    realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    if(currentAngularMode == AM_DMS) {
      convertAngle39FromTo(&x, xAngularMode, AM_DEGREE);
      realMultiply(&y, &x, &x, &ctxtReal39);
      convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      convertAngle39FromTo(&x, xAngularMode, currentAngularMode);
      realMultiply(&y, &x, &x, &ctxtReal39);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
    }
   setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(real34) × X(long integer) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRealLonI(void) {
  real39_t y, x;
  uint32_t yAngularMode;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  yAngularMode = getRegisterAngularMode(REGISTER_Y);
  convertLongIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);

  if(yAngularMode == AM_NONE) {
    realMultiply(&y, &x, &x, &ctxtReal39);
    realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    if(currentAngularMode == AM_DMS) {
      convertAngle39FromTo(&y, yAngularMode, AM_DEGREE);
      realMultiply(&y, &x, &x, &ctxtReal39);
      convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      convertAngle39FromTo(&y, yAngularMode, currentAngularMode);
      realMultiply(&y, &x, &x, &ctxtReal39);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
    }
    setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(long integer) × X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulLonICplx(void) {
  real39_t a, c, d;

  convertLongIntegerRegisterToReal(REGISTER_Y, &a, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &c);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &d);

  realMultiply(&c, &a, &c, &ctxtReal39);
  realMultiply(&d, &a, &d, &ctxtReal39);

  realToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&d, REGISTER_IMAG34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex34) × X(long integer) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCplxLonI(void) {
  real39_t a, b, c;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &a);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_Y), &b);
  convertLongIntegerRegisterToReal(REGISTER_X, &c, &ctxtReal39);

  realMultiply(&a, &c, &a, &ctxtReal39);
  realMultiply(&b, &c, &b, &ctxtReal39);

  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  realToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&b, REGISTER_IMAG34_DATA(REGISTER_X));
}



/******************************************************************************************************************************************************************************************/
/* time × ...                                                                                                                                                                             */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(time) × X(short integer) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void mulTimeShoI(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(short integer) × X(time) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void mulShoITime(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(time) × X(real34) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void mulTimeReal(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real34) × X(time) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRealTime(void) {
  fnToBeCoded();
}



/******************************************************************************************************************************************************************************************/
/* time × ...                                                                                                                                                                             */
/******************************************************************************************************************************************************************************************/



/******************************************************************************************************************************************************************************************/
/* string × ...                                                                                                                                                                           */
/******************************************************************************************************************************************************************************************/



/******************************************************************************************************************************************************************************************/
/* real16 matrix × ...                                                                                                                                                                    */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(real16 matrix) × X(real16 matrix) ==> X(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRemaRema(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real16 matrix) × X(complex16 matrix) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRemaCxma(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(complex16 matrix) × X(real16 matrix) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCxmaRema(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real16 matrix) × X(short integer) ==> X(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRemaShoI(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(short integer) × X(real16 matrix) ==> X(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulShoIRema(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real16 matrix) × X(real34) ==> X(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRemaReal(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real34) × X(real16 matrix) ==> X(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRealRema(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real16 matrix) × X(complex34) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRemaCplx(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(complex34) × X(real16 matrix) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCplxRema(void) {
  fnToBeCoded();
}



/******************************************************************************************************************************************************************************************/
/* complex16 matrix × ...                                                                                                                                                                 */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(complex16 matrix) × X(complex16 matrix) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCxmaCxma(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(complex16 matrix) × X(short integer) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCxmaShoI(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(short integer) × X(complex16 matrix) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulShoICxma(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(complex16 matrix) × X(real34) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCxmaReal(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real34) × X(complex16 matrix) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRealCxma(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(complex16 matrix) × X(complex34) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCxmaCplx(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(complex34) × X(complex16 matrix) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCplxCxma(void) {
  fnToBeCoded();
}



/******************************************************************************************************************************************************************************************/
/* short integer × ...                                                                                                                                                                    */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(short integer) × X(short integer) ==> X(short integer)
 *
 * \param void
 * \return void
 ***********************************************/
void mulShoIShoI(void) {
  setRegisterTag(REGISTER_X, getRegisterTag(REGISTER_Y));
  *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = WP34S_intMultiply(*(REGISTER_SHORT_INTEGER_DATA(REGISTER_Y)), *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)));
}



/********************************************//**
 * \brief Y(short integer) × X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulShoIReal(void) {
  real39_t y, x;
  uint32_t xAngularMode;

  convertShortIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
  xAngularMode = getRegisterAngularMode(REGISTER_X);

  if(xAngularMode == AM_NONE) {
    realMultiply(&y, &x, &x, &ctxtReal39);
    realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    if(currentAngularMode == AM_DMS) {
      convertAngle39FromTo(&x, xAngularMode, AM_DEGREE);
      realMultiply(&y, &x, &x, &ctxtReal39);
      convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      convertAngle39FromTo(&x, xAngularMode, currentAngularMode);
      realMultiply(&y, &x, &x, &ctxtReal39);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
    }
   setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(real34) × X(short integer) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRealShoI(void) {
  real39_t y, x;
  uint32_t yAngularMode;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  yAngularMode = getRegisterAngularMode(REGISTER_Y);
  convertShortIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);

  if(yAngularMode == AM_NONE) {
    realMultiply(&y, &x, &x, &ctxtReal39);
    realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    if(currentAngularMode == AM_DMS) {
      convertAngle39FromTo(&y, yAngularMode, AM_DEGREE);
      realMultiply(&y, &x, &x, &ctxtReal39);
      convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      convertAngle39FromTo(&y, yAngularMode, currentAngularMode);
      realMultiply(&y, &x, &x, &ctxtReal39);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
    }
    setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(short integer) × X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulShoICplx(void) {
  convertShortIntegerRegisterToReal34Register(REGISTER_Y, REGISTER_Y);
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X)); // real part
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X)); // imaginary part
}



/********************************************//**
 * \brief Y(complex34) × X(short integer) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCplxShoI(void) {
  convertShortIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y)); // real part
  real34Multiply(REGISTER_IMAG34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_Y)); // imaginary part
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_Y), REGISTER_COMPLEX34_DATA(REGISTER_X));
}



/******************************************************************************************************************************************************************************************/
/* real34 × ...                                                                                                                                                                           */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(real34) × X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRealReal(void) {
  uint32_t yAngularMode, xAngularMode;

  yAngularMode = getRegisterAngularMode(REGISTER_Y);
  xAngularMode = getRegisterAngularMode(REGISTER_X);

  if(yAngularMode == AM_NONE && xAngularMode == AM_NONE) {
    real34Multiply(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
  }
  else if(yAngularMode != AM_NONE && xAngularMode != AM_NONE) {
    real34Multiply(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
    setRegisterAngularMode(REGISTER_X, AM_NONE);
  }
  else {
    real39_t y, x;

    if(yAngularMode == AM_NONE) {
      yAngularMode = currentAngularMode;
    }
    else if(xAngularMode == AM_NONE) {
      xAngularMode = currentAngularMode;
    }

    real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);

    if(currentAngularMode == AM_DMS) {
      convertAngle39FromTo(&y, yAngularMode, AM_DEGREE);
      convertAngle39FromTo(&x, xAngularMode, AM_DEGREE);

      realMultiply(&y, &x, &x, &ctxtReal39);

      convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
    }
    else { //current angular mode is not DMS
      convertAngle39FromTo(&y, yAngularMode, currentAngularMode);
      convertAngle39FromTo(&x, xAngularMode, currentAngularMode);

      realMultiply(&y, &x, &x, &ctxtReal39);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
    }
    setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(real34) × X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRealCplx(void) {
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X)); // real part
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X)); // imaginary part
}



/********************************************//**
 * \brief Y(complex34) × X(real34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCplxReal(void) {
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y)); // real part
  real34Multiply(REGISTER_IMAG34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_Y)); // imaginary part
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_Y), REGISTER_COMPLEX34_DATA(REGISTER_X)); // imaginary part
}



/******************************************************************************************************************************************************************************************/
/* complex34 × ...                                                                                                                                                                        */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(complex34) × X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCplxCplx(void) {
  real39_t yReal, yImag, xReal, xImag;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &yReal);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_Y), &yImag);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &xReal);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &xImag);

  mulCo39Co39(&yReal, &yImag, &xReal, &xImag, &xReal, &xImag);

  realToReal34(&xReal, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&xImag, REGISTER_IMAG34_DATA(REGISTER_X));
}
