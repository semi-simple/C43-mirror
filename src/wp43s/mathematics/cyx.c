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
 * \file cyx.c
 ***********************************************/

#include "wp43s.h"

#include "cyx.h"

static void cyxDataTypeError(void);

void (* const cyx[9][9])(void) = {
// regX |    regY ==>    1                 2                 3                 4                 5                 6                 7                 8                 9
//      V                Long integer      Real34            Complex34         Time              Date              String            Real34 mat        Complex34 mat     Short integer
/*  1 Long integer  */ { cyxLonILonI,      cyxRealLonI,      cyxCplxLonI,      cyxDataTypeError, cyxDataTypeError, cyxDataTypeError, cyxDataTypeError, cyxDataTypeError, cyxShoILonI      },
/*  2 Real34        */ { cyxLonIReal,      cyxRealReal,      cyxCplxReal,      cyxDataTypeError, cyxDataTypeError, cyxDataTypeError, cyxDataTypeError, cyxDataTypeError, cyxShoIReal      },
/*  3 Complex34     */ { cyxLonICplx,      cyxRealCplx,      cyxCplxCplx,      cyxDataTypeError, cyxDataTypeError, cyxDataTypeError, cyxDataTypeError, cyxDataTypeError, cyxShoICplx      },
/*  4 Time          */ { cyxDataTypeError, cyxDataTypeError, cyxDataTypeError, cyxDataTypeError, cyxDataTypeError, cyxDataTypeError, cyxDataTypeError, cyxDataTypeError, cyxDataTypeError },
/*  5 Date          */ { cyxDataTypeError, cyxDataTypeError, cyxDataTypeError, cyxDataTypeError, cyxDataTypeError, cyxDataTypeError, cyxDataTypeError, cyxDataTypeError, cyxDataTypeError },
/*  6 String        */ { cyxDataTypeError, cyxDataTypeError, cyxDataTypeError, cyxDataTypeError, cyxDataTypeError, cyxDataTypeError, cyxDataTypeError, cyxDataTypeError, cyxDataTypeError },
/*  7 Real34 mat    */ { cyxDataTypeError, cyxDataTypeError, cyxDataTypeError, cyxDataTypeError, cyxDataTypeError, cyxDataTypeError, cyxDataTypeError, cyxDataTypeError, cyxDataTypeError },
/*  8 Complex34 mat */ { cyxDataTypeError, cyxDataTypeError, cyxDataTypeError, cyxDataTypeError, cyxDataTypeError, cyxDataTypeError, cyxDataTypeError, cyxDataTypeError, cyxDataTypeError },
/*  9 Short integer */ { cyxLonIShoI,      cyxRealShoI,      cyxCplxShoI,      cyxDataTypeError, cyxDataTypeError, cyxDataTypeError, cyxDataTypeError, cyxDataTypeError, cyxShoIShoI      }
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
		showInfoDialog("In function fnCyx/fnPyx:", errorMessage, NULL, NULL);   \
	} while(0)

#else // EXTRA_INFO_ON_CALC_ERROR != 1

#define EXTRA_INFO_MESSAGE(msg)

#endif // EXTRA_INFO_ON_CALC_ERROR


static void cyxError(uint16_t error)
{
    switch(error)
    {
        case DOMAIN_ERROR:
            displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
            EXTRA_INFO_MESSAGE("cannot calculate Cyx, y and x must be greater or equal than zero.");
            break;

        case INVALID_DATA_CMP_ERROR:
            displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
            EXTRA_INFO_MESSAGE("cannot calculate Cyx, y must be greater or equal than x.");
            break;

        case DATA_OUT_OF_RANGE_ERROR:
            displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
            EXTRA_INFO_MESSAGE("cannot calculate Cyx, the limit for Long is 450 and for Short is 20.");
            break;
    }
}

/********************************************//**
 * \brief Data type error in Cyx
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
static void cyxDataTypeError(void)
{
    displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);

#if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot raise %s", getRegisterDataTypeName(REGISTER_Y, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "to %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnCyx:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
#endif
}

//=============================================================================
// Calculation function
//-----------------------------------------------------------------------------

static void cyxReal(real39_t *y, real39_t *x, real39_t *result)
{
    real34Subtract(y, x, result);       // t = y-x
    WP34S_Factorial(result, result);    // t = (y-x)!

    WP34S_Factorial(y, y);              // y = y!
    WP34S_Factorial(x, x);              // x = x!

    real34Multiply(x, result, result);  // t = x! * (y-x)!

    real34Divide(y, result, result);    // t = y! / [x! * (y-x)!]
}

//=============================================================================
// Main function
//-----------------------------------------------------------------------------

/********************************************//**
 * \brief regX ==> regL and cyx(regX, RegY) ==> regX
 * enables stack lift and refreshes the stack.
 * C(n,k) = n! / [k! * (n-k)!]
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnCyx(uint16_t unusedParamButMandatory) 
{
	saveStack();
	copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    cyx[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

    adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
    adjustResult(REGISTER_Y, true, true, REGISTER_Y, -1, -1);
}

//=============================================================================
// Cyx(Y(long integer), *)
//-----------------------------------------------------------------------------

void cyxLong(longInteger_t *y, longInteger_t *x, longInteger_t *result)
{
    longIntegerSubtract(*y, *x, *result);						// t = y-x
    longIntegerFactorial(longIntegerToUInt(*result), *result);	// t = (y-x)!

    longIntegerFactorial(longIntegerToUInt(*y), *y);            // y = y!

    longIntegerFactorial(longIntegerToUInt(*x), *x);		    // x = x!

    longIntegerMultiply(*x, *result, *result);					// t = x! * (y-x)!

    longIntegerDivide(*y, *result, *result);					// t = y! / [x! * (y -x)!]
}

/********************************************//**
 * \brief Cyx(Y(long integer), X(long integer)) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void cyxLonILonI(void)
{
    longInteger_t x, y;

    convertLongIntegerRegisterToLongInteger(REGISTER_X, x);
    convertLongIntegerRegisterToLongInteger(REGISTER_Y, y);
    
    if(longIntegerIsNegative(x) || longIntegerIsNegative(y))
        cyxError(DOMAIN_ERROR);
    else if(longIntegerCompareUInt(x, 450) > 0 || longIntegerCompareUInt(y, 450) > 0)
        cyxError(DATA_OUT_OF_RANGE_ERROR);
    else if(longIntegerCompare(y, x) < 0)
        cyxError(INVALID_DATA_CMP_ERROR);
    else
    {
        longInteger_t t;

        longIntegerInit(t);

        cyxLong(&y, &x, &t);

//        longIntegerSubtract(y, x, t);						// t = y-x
//        longIntegerFactorial(longIntegerToUInt(t), t);		// t = (y-x)!
//
//        longIntegerFactorial(longIntegerToUInt(y), y);		// y = y!
//
//        longIntegerFactorial(longIntegerToUInt(x), x);		// x = x!
//
//        longIntegerMultiply(x, t, t);						// t = x! * (y-x)!
//
//        longIntegerDivide(y, t, t);							// t = y! / [x! * (y -x)!]

        convertLongIntegerToLongIntegerRegister(t, REGISTER_X);
        longIntegerFree(t);
    }

    longIntegerFree(x);
    longIntegerFree(y);
}

/********************************************//**
 * \brief Cyx(Y(long integer), X(real34)) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void cyxLonIReal(void)
{
    real39_t x, y;

    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
    convertLongIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);

    if(real34IsNegative(&x) || real34IsNegative(&y))
        cyxError(DOMAIN_ERROR);
    else
    {
        real39_t t;

        cyxReal(&y, &x, &t);

        realToReal34(&t, REGISTER_REAL34_DATA(REGISTER_X));
        setRegisterAngularMode(REGISTER_X, AM_NONE);
    }
}

/********************************************//**
 * \brief Cyx(Y(long integer), X(complex34)) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void cyxLonICplx(void)
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

    realAdd(&xReal, const_1, &xReal, &ctxtReal39);                  // x = x+1
    WP34S_ComplexGamma(&xReal, &xImag, &xReal, &xImag);             // x = Gamma(x+1) = x!

    realAdd(&yReal, const34_1, &yReal, &ctxtReal39);                // y = y+1
    WP34S_ComplexGamma(&yReal, const_0, &yReal, &yImag);            // y = Gamma(y+1) = y!

    mulCo39Co39(&tReal, &tImag, &xReal, &xImag, &tReal, &tImag);    // t = x! * (y-x)!
    divCo39Co39(&yReal, const_0, &tReal, &tImag, &tReal, &tImag);   // t = y! / [x! * (y-x)!]

    realToReal34(&tReal, REGISTER_REAL34_DATA(REGISTER_X));
    realToReal34(&tImag, REGISTER_IMAG34_DATA(REGISTER_X));
}

/********************************************//**
 * \brief Cyx(Y(long integer), X(short integer)) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void cyxLonIShoI(void)
{
    longInteger_t x, y;

    convertShortIntegerRegisterToLongInteger(REGISTER_X, x);
    convertLongIntegerRegisterToLongInteger(REGISTER_Y, y);

    if(longIntegerIsNegative(x) || longIntegerIsNegative(y))
        cyxError(DOMAIN_ERROR);
    else if(longIntegerCompareUInt(x, 450) > 0 || longIntegerCompareUInt(y, 450) > 0)
        cyxError(DATA_OUT_OF_RANGE_ERROR);
    else if(longIntegerCompare(y, x) < 0)
        cyxError(INVALID_DATA_CMP_ERROR);
    else
    {
        longInteger_t t;

        longIntegerInit(t);

        longIntegerSubtract(y, x, t);						// t = y-x
        longIntegerFactorial(longIntegerToUInt(t), t);		// t = (y-x)!

        longIntegerFactorial(longIntegerToUInt(y), y);		// y = y!

        longIntegerFactorial(longIntegerToUInt(x), x);		// x = x!

        longIntegerMultiply(x, t, t);						// t = x! * (y-x)!
        longIntegerDivide(y, t, t);							// t = y! / [x! * (y -x)!]

        convertLongIntegerToLongIntegerRegister(t, REGISTER_X);
        longIntegerFree(t);
    }

    longIntegerFree(x);
    longIntegerFree(y);
}

//=============================================================================
// Cyx(Y(real34), *)
//-----------------------------------------------------------------------------

/********************************************//**
 * \brief Cyx(Y(real34), X(long integer)) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void cyxRealLonI()
{
    real39_t x, y;

    real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
    convertLongIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);

    if(real34IsNegative(&x) || real34IsNegative(&y))
        cyxError(DOMAIN_ERROR);
    else
    {
        real39_t t;

        real34Subtract(&y, &x, &t);     // t = y-x
        WP34S_Factorial(&t, &t);        // t = (y-x)!

        WP34S_Factorial(&y, &y);        // y = y!

        WP34S_Factorial(&x, &x);        // x = x!

        real34Multiply(&x, &t, &t);     // t = x! * (y-x)!

        real34Divide(&y, &t, &t);       // t = y! / [x! * (y-x)!]

        realToReal34(&t, REGISTER_REAL34_DATA(REGISTER_X));
        setRegisterAngularMode(REGISTER_X, AM_NONE);
    }

}

/********************************************//**
 * \brief Cyx(Y(real34), X(real34)) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void cyxRealReal(void)
{
    fnToBeCoded();

}

/********************************************//**
 * \brief Cyx(Y(real34), X(complex34)) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void cyxRealCplx(void)
{
    fnToBeCoded();

}

/********************************************//**
 * \brief Cyx(Y(real34), X(short integer)) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void cyxRealShoI(void)
{
    fnToBeCoded();

}

//=============================================================================
// Cyx(Y(complex34), *)
//-----------------------------------------------------------------------------

/********************************************//**
 * \brief Cyx(Y(complex34), X(long integer)) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void cyxCplxLonI(void)
{
    fnToBeCoded();

}

/********************************************//**
 * \brief Cyx(Y(complex34), X(real34)) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void cyxCplxReal(void)
{
    fnToBeCoded();

}

/********************************************//**
 * \brief Cyx(Y(complex34), X(complex34)) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void cyxCplxCplx(void)
{
    fnToBeCoded();

}

/********************************************//**
 * \brief Cyx(Y(complex34), X(short integer)) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void cyxCplxShoI(void)
{
    fnToBeCoded();

}

//=============================================================================
// Cyx(Y(short integer), *)
//-----------------------------------------------------------------------------

/********************************************//**
 * \brief Cyx(Y(short integer), X(long integer)) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void cyxShoILonI(void)
{
    longInteger_t x, y;

    convertLongIntegerRegisterToLongInteger(REGISTER_X, x);
    convertShortIntegerRegisterToLongInteger(REGISTER_Y, y);

    if(longIntegerIsNegative(x) || longIntegerIsNegative(y))
        cyxError(DOMAIN_ERROR);
    else if(longIntegerCompareUInt(x, 450) > 0 || longIntegerCompareUInt(y, 450) > 0)
        cyxError(DATA_OUT_OF_RANGE_ERROR);
    else if(longIntegerCompare(y, x) < 0)
        cyxError(INVALID_DATA_CMP_ERROR);
    else
    {
        longInteger_t t;

        longIntegerInit(t);

        longIntegerSubtract(y, x, t);						// t = y-x
        longIntegerFactorial(longIntegerToUInt(t), t);		// t = (y-x)!

        longIntegerFactorial(longIntegerToUInt(y), y);		// y = y!

        longIntegerFactorial(longIntegerToUInt(x), x);		// x = x!

        longIntegerMultiply(x, t, t);						// t = x! * (y-x)!

        longIntegerDivide(y, t, t);							// t = y! / [x! * (y -x)!]

        convertLongIntegerToLongIntegerRegister(t, REGISTER_X);
        longIntegerFree(t);
    }

    longIntegerFree(x);
    longIntegerFree(y);
}

/********************************************//**
 * \brief Cyx(Y(short integer), X(real34)) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void cyxShoIReal(void)
{
    fnToBeCoded();

}

/********************************************//**
 * \brief Cyx(Y(short integer), X(complex34)) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void cyxShoICplx(void)
{
    fnToBeCoded();

}

/********************************************//**
 * \brief Cyx(Y(short integer), X(short integer)) ==> X(short integer)
 *
 * \param void
 * \return void
 ***********************************************/
void cyxShoIShoI(void)
{
	int16_t x_sign, y_sign;
	uint64_t x_value, y_value;

	convertShortIntegerRegisterToUInt64(REGISTER_X, &x_sign, &x_value);
	convertShortIntegerRegisterToUInt64(REGISTER_Y, &y_sign, &y_value);

    if(x_sign==1 || y_sign==1)
        cyxError(DOMAIN_ERROR);
    else if(x_value > 20 || y_value > 20)
        cyxError(DATA_OUT_OF_RANGE_ERROR);
    else if(y_value < x_value)
        cyxError(INVALID_DATA_CMP_ERROR);
    else
    {
        uint64_t value = fact_uint64(y_value) / ( fact_uint64(x_value) * fact_uint64(y_value - x_value));

        if(value > shortIntegerMask)
            fnSetFlag(FLAG_OVERFLOW);

        convertUInt64ToShortIntegerRegister(0, value, getRegisterTag(REGISTER_X), REGISTER_X);
    }
}

