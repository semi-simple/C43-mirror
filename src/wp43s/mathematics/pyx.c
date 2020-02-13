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
 * \file pyx.c
 ***********************************************/

#include "wp43s.h"

#include "pyx.h"

static void pyxDataTypeError(void);

void (* const pyx[9][9])(void) = {
// regX |    regY ==>    1                 2                 3                 4                 5                 6                 7                 8                 9
//      V                Long integer      Real34            Complex34         Time              Date              String            Real34 mat        Complex34 mat     Short integer
/*  1 Long integer  */ { pyxLonILonI,      pyxRealLonI,      pyxCplxLonI,      pyxDataTypeError, pyxDataTypeError, pyxDataTypeError, pyxDataTypeError, pyxDataTypeError, pyxShoILonI      },
/*  2 Real34        */ { pyxLonIReal,      pyxRealReal,      pyxCplxReal,      pyxDataTypeError, pyxDataTypeError, pyxDataTypeError, pyxDataTypeError, pyxDataTypeError, pyxShoIReal      },
/*  3 Complex34     */ { pyxLonICplx,      pyxRealCplx,      pyxCplxCplx,      pyxDataTypeError, pyxDataTypeError, pyxDataTypeError, pyxDataTypeError, pyxDataTypeError, pyxShoICplx      },
/*  4 Time          */ { pyxDataTypeError, pyxDataTypeError, pyxDataTypeError, pyxDataTypeError, pyxDataTypeError, pyxDataTypeError, pyxDataTypeError, pyxDataTypeError, pyxDataTypeError },
/*  5 Date          */ { pyxDataTypeError, pyxDataTypeError, pyxDataTypeError, pyxDataTypeError, pyxDataTypeError, pyxDataTypeError, pyxDataTypeError, pyxDataTypeError, pyxDataTypeError },
/*  6 String        */ { pyxDataTypeError, pyxDataTypeError, pyxDataTypeError, pyxDataTypeError, pyxDataTypeError, pyxDataTypeError, pyxDataTypeError, pyxDataTypeError, pyxDataTypeError },
/*  7 Real34 mat    */ { pyxDataTypeError, pyxDataTypeError, pyxDataTypeError, pyxDataTypeError, pyxDataTypeError, pyxDataTypeError, pyxDataTypeError, pyxDataTypeError, pyxDataTypeError },
/*  8 Complex34 mat */ { pyxDataTypeError, pyxDataTypeError, pyxDataTypeError, pyxDataTypeError, pyxDataTypeError, pyxDataTypeError, pyxDataTypeError, pyxDataTypeError, pyxDataTypeError },
/*  9 Short integer */ { pyxLonIShoI,      pyxRealShoI,      pyxCplxShoI,      pyxDataTypeError, pyxDataTypeError, pyxDataTypeError, pyxDataTypeError, pyxDataTypeError, pyxShoIShoI      }
};

//=============================================================================
// Error handling
//-----------------------------------------------------------------------------

#define DOMAIN_ERROR			    1
#define INVALID_DATA_CMP_ERROR		2
#define DATA_OUT_OF_RANGE_ERROR		3

#if (EXTRA_INFO_ON_CALC_ERROR == 1)

#define EXTRA_INFO_MESSAGE(msg) 								                \
	do { 																        \
		sprintf(errorMessage, msg);								                \
		showInfoDialog("In function fnPyx:", errorMessage, NULL, NULL);   \
	} while(0)

#else // EXTRA_INFO_ON_CALC_ERROR != 1

#define EXTRA_INFO_MESSAGE(msg)

#endif // EXTRA_INFO_ON_CALC_ERROR
	    
static void pyxError(uint16_t error)
{
    switch(error)
    {
        case DOMAIN_ERROR:
            displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
            EXTRA_INFO_MESSAGE("cannot calculate Pyx, y and x must be greater or equal than zero.");
            break;

        case INVALID_DATA_CMP_ERROR:
            displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
            EXTRA_INFO_MESSAGE("cannot calculate Pyx, y must be greater or equal than x.");
            break;

        case DATA_OUT_OF_RANGE_ERROR:
            displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
            EXTRA_INFO_MESSAGE("cannot calculate Pyx, the limit for Long is 450 and for Short is 20.");
            break;
    }
}

/********************************************//**
 * \brief Data type error in pyx
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
static void pyxDataTypeError(void)
{
    displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);

#if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot raise %s", getRegisterDataTypeName(REGISTER_Y, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "to %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnPyx:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
#endif
}

//=============================================================================
// Main function
//-----------------------------------------------------------------------------

/********************************************//**
 * \brief regX ==> regL and Pyx(regX, RegY) ==> regX
 * enables stack lift and refreshes the stack.
 * C(n,k) = n! / [k! * (n-k)!]
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnPyx(uint16_t unusedParamButMandatory)
{
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    pyx[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

    adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
    adjustResult(REGISTER_Y, true, true, REGISTER_Y, -1, -1);
}

//=============================================================================
// Pyx(Y(long integer), *)
//-----------------------------------------------------------------------------

/********************************************//**
 * \brief Pyx(Y(long integer), X(long integer)) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void pyxLonILonI(void)
{
    longInteger_t x, y;

    convertLongIntegerRegisterToLongInteger(REGISTER_X, x);
    convertLongIntegerRegisterToLongInteger(REGISTER_Y, y);

    if(longIntegerIsNegative(x) || longIntegerIsNegative(y))
        pyxError(DOMAIN_ERROR);
    else if(longIntegerCompareUInt(x, 450) > 0 || longIntegerCompareUInt(y, 450) > 0)
        pyxError(DATA_OUT_OF_RANGE_ERROR);
    else if(longIntegerCompare(y, x) < 0)
        pyxError(INVALID_DATA_CMP_ERROR);
    else
    {
        longInteger_t t;

        longIntegerInit(t);

        longIntegerSubtract(y, x, t);						// t = y-x
        longIntegerFactorial(longIntegerToUInt(t), t);		// t = (y-x)!

        longIntegerFactorial(longIntegerToUInt(y), y);		// y = y!

        longIntegerDivide(y, t, t);							// t = y! / (y -x)!

        convertLongIntegerToLongIntegerRegister(t, REGISTER_X);
        longIntegerFree(t);
    }

    longIntegerFree(x);
    longIntegerFree(y);
}

/********************************************//**
 * \brief Pyx(Y(long integer), X(real34)) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void pyxLonIReal(void)
{
    real39_t x, y;

    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
    convertLongIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);

    if(real34IsNegative(&x) || real34IsNegative(&y))
        pyxError(DOMAIN_ERROR);
    else
    {
        real39_t t;

        real34Subtract(&y, &x, &t);     // t = y-x
        WP34S_Factorial(&t, &t);        // t = (y-x)!

        WP34S_Factorial(&y, &y);        // y = y!

        real34Divide(&y, &t, &t);       // t = y! / (y-x)!

        realToReal34(&t, REGISTER_REAL34_DATA(REGISTER_X));
        setRegisterAngularMode(REGISTER_X, AM_NONE);
    }
}

/********************************************//**
 * \brief Pyx(Y(long integer), X(complex34)) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void pyxLonICplx(void)
{
    real39_t xReal, xImag, yReal, yImag;

    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &xReal);
    real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &xImag);

    convertLongIntegerRegisterToReal(REGISTER_Y, &yReal, &ctxtReal39);


    real39_t tReal, tImag;

    realSubtract(&yReal, &xReal, &tReal, &ctxtReal39);              // t = y-x
    realSubtract(&yImag, &xImag, &tImag, &ctxtReal39);

    realAdd(&tReal, const_1, &tReal, &ctxtReal39);                  // t = t+1
    WP34S_ComplexGamma(&tReal, &tImag, &tReal, &tImag);             // t = Gamma(t+1) = (y-x)!

    realAdd(&yReal, const34_1, &yReal, &ctxtReal39);                // y = y+1
    WP34S_ComplexGamma(&yReal, const_0, &yReal, &yImag);            // y = Gamma(y+1) = y!

    divCo39Co39(&yReal, const_0, &tReal, &tImag, &tReal, &tImag);   // t = y! / (y-x)!

    realToReal34(&tReal, REGISTER_REAL34_DATA(REGISTER_X));
    realToReal34(&tImag, REGISTER_IMAG34_DATA(REGISTER_X));
}

/********************************************//**
 * \brief Pyx(Y(long integer), X(short integer)) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void pyxLonIShoI(void)
{
    longInteger_t x, y;

    convertShortIntegerRegisterToLongInteger(REGISTER_X, x);
    convertLongIntegerRegisterToLongInteger(REGISTER_Y, y);

    if(longIntegerIsNegative(x) || longIntegerIsNegative(y))
        pyxError(DOMAIN_ERROR);
    else if(longIntegerCompareUInt(x, 450) > 0 || longIntegerCompareUInt(y, 450) > 0)
        pyxError(DATA_OUT_OF_RANGE_ERROR);
    else if(longIntegerCompare(y, x) < 0)
        pyxError(INVALID_DATA_CMP_ERROR);
    else
    {
        longInteger_t t;

        longIntegerInit(t);

        longIntegerSubtract(y, x, t);						// t = y-x
        longIntegerFactorial(longIntegerToUInt(t), t);		// t = (y-x)!

        longIntegerFactorial(longIntegerToUInt(y), y);		// y = y!

        longIntegerDivide(y, t, t);							// t = y! / (y-x)!

        convertLongIntegerToLongIntegerRegister(t, REGISTER_X);
        longIntegerFree(t);
    }

    longIntegerFree(x);
    longIntegerFree(y);
}

//=============================================================================
// Pyx(Y(real34), *)
//-----------------------------------------------------------------------------

/********************************************//**
 * \brief Pyx(Y(real34), X(long integer)) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void pyxRealLonI()
{
    fnToBeCoded();

}

/********************************************//**
 * \brief Pyx(Y(real34), X(real34)) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void pyxRealReal(void)
{
    fnToBeCoded();

}

/********************************************//**
 * \brief Pyx(Y(real34), X(complex34)) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void pyxRealCplx(void)
{
    fnToBeCoded();

}

/********************************************//**
 * \brief Pyx(Y(real34), X(short integer)) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void pyxRealShoI(void)
{
    fnToBeCoded();

}

//=============================================================================
// Pyx(Y(complex34), *)
//-----------------------------------------------------------------------------

/********************************************//**
 * \brief Pyx(Y(complex34), X(long integer)) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void pyxCplxLonI(void)
{
    fnToBeCoded();

}

/********************************************//**
 * \brief Pyx(Y(complex34), X(real34)) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void pyxCplxReal(void)
{
    fnToBeCoded();

}

/********************************************//**
 * \brief Pyx(Y(complex34), X(complex34)) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void pyxCplxCplx(void)
{
    fnToBeCoded();

}

/********************************************//**
 * \brief Pyx(Y(complex34), X(short integer)) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void pyxCplxShoI(void)
{
    fnToBeCoded();

}

//=============================================================================
// Pyx(Y(short integer), *)
//-----------------------------------------------------------------------------

/********************************************//**
 * \brief Pyx(Y(short integer), X(long integer)) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void pyxShoILonI(void)
{
    longInteger_t x, y;

    convertLongIntegerRegisterToLongInteger(REGISTER_X, x);
    convertShortIntegerRegisterToLongInteger(REGISTER_Y, y);

    if(longIntegerIsNegative(x) || longIntegerIsNegative(y))
        pyxError(DOMAIN_ERROR);
    else if(longIntegerCompareUInt(x, 450) > 0 || longIntegerCompareUInt(y, 450) > 0)
        pyxError(DATA_OUT_OF_RANGE_ERROR);
    else if(longIntegerCompare(y, x) < 0)
        pyxError(INVALID_DATA_CMP_ERROR);
    else
    {
        longInteger_t t;

        longIntegerInit(t);

        longIntegerSubtract(y, x, t);						// t = y-x

        longIntegerFactorial(longIntegerToUInt(t), t);		// t = (y-x)!

        longIntegerFactorial(longIntegerToUInt(y), y);		// y = y!

        longIntegerDivide(y, t, t);							// t = y! / (y -x)!

        convertLongIntegerToLongIntegerRegister(t, REGISTER_X);
        longIntegerFree(t);
    }

    longIntegerFree(x);
    longIntegerFree(y);
}

/********************************************//**
 * \brief Pyx(Y(short integer), X(real34)) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void pyxShoIReal(void)
{
    fnToBeCoded();

}

/********************************************//**
 * \brief Pyx(Y(short integer), X(complex34)) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void pyxShoICplx(void)
{
    fnToBeCoded();

}

/********************************************//**
 * \brief Pyx(Y(short integer), X(short integer)) ==> X(short integer)
 *
 * \param void
 * \return void
 ***********************************************/
void pyxShoIShoI(void)
{
    int16_t x_sign, y_sign;
    uint64_t x_value, y_value;

    convertShortIntegerRegisterToUInt64(REGISTER_X, &x_sign, &x_value);
    convertShortIntegerRegisterToUInt64(REGISTER_Y, &y_sign, &y_value);

    if(x_sign==1 || y_sign==1)
        pyxError(DOMAIN_ERROR);
    else if(x_value > 20 || y_value > 20)
        pyxError(DATA_OUT_OF_RANGE_ERROR);
    else if(y_value < x_value)
        pyxError(INVALID_DATA_CMP_ERROR);
    else
    {
        uint64_t value = fact_uint64(y_value) / fact_uint64(y_value - x_value);

        if (value > shortIntegerMask)
            fnSetFlag(FLAG_OVERFLOW);

        convertUInt64ToShortIntegerRegister(0, value, getRegisterTag(REGISTER_X), REGISTER_X);
    }
}

