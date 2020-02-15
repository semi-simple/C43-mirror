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

#define INVALID_DATA_TYPE_ERROR		0
#define INVALID_DATA_ERROR			1
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


static void cpyxError(uint16_t error)
{
	switch(error)
	{
		case INVALID_DATA_TYPE_ERROR:
			displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
			EXTRA_INFO_MESSAGE("cannot calculate Cyx/Pyx, y and x must be integers");
		break;

		case INVALID_DATA_ERROR:
			displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
			EXTRA_INFO_MESSAGE("cannot calculate Cyx/Pyx, y and x must be greater or equal than zero.");
		break;

		case INVALID_DATA_CMP_ERROR:
			displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
			EXTRA_INFO_MESSAGE("cannot calculate Cyx/Pyx, y must be greater or equal than x.");
		break;

		case DATA_OUT_OF_RANGE_ERROR:
		    displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
		    EXTRA_INFO_MESSAGE("cannot calculate Cyx/Pyx, the limit for Long is 450 and for Short is 20.");
		break;
	}
}

#define IS_LONG_INTEGER(reg)	(getRegisterDataType(reg)==dtLongInteger)
#define IS_SHORT_INTEGER(reg)	(getRegisterDataType(reg)==dtShortInteger)
#define IS_INTEGER(reg) 		(IS_LONG_INTEGER(reg) || IS_SHORT_INTEGER(reg))

/********************************************//**
 * \brief regX ==> regL and Cxy(regX, RegY) ==> regX
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

	if(!IS_INTEGER(REGISTER_X) || !IS_INTEGER(REGISTER_Y))
        cpyxError(INVALID_DATA_TYPE_ERROR);
 	else
 	{
		if(IS_LONG_INTEGER(REGISTER_X) || IS_LONG_INTEGER(REGISTER_Y))
			cyxLonI();
		else
			cyxShoI();

		adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
		adjustResult(REGISTER_Y, true, true, REGISTER_Y, -1, -1);
	}
}

/*
 * Calculate Cyx
 *
 * This function is called when one or both of X and Y are long integer(s).
 */
void cyxLonI(void)
{
	longInteger_t x, y;

	if(IS_SHORT_INTEGER(REGISTER_X))
		convertShortIntegerRegisterToLongInteger(REGISTER_X, x);
	else
		convertLongIntegerRegisterToLongInteger(REGISTER_X, x);

	if(IS_SHORT_INTEGER(REGISTER_Y))
		convertShortIntegerRegisterToLongInteger(REGISTER_Y, y);
	else
		convertLongIntegerRegisterToLongInteger(REGISTER_Y, y);

	if(longIntegerIsNegative(x) || longIntegerIsNegative(y))
        cpyxError(INVALID_DATA_ERROR);
	else if(longIntegerCompareUInt(x, 450) > 0 || longIntegerCompareUInt(y, 450) > 0)
        cpyxError(DATA_OUT_OF_RANGE_ERROR);
	else if(longIntegerCompare(y, x) < 0)
        cpyxError(INVALID_DATA_CMP_ERROR);
	else
	{
		longInteger_t t;

		longIntegerInit(t);
   		longIntegerSubtract(y, x, t);						// t = y-x
    	longIntegerFactorial(longIntegerToUInt(t), t);		// t = (y-x)!

    	longIntegerFactorial(longIntegerToUInt(y), y);		// y = y!
    	longIntegerFactorial(longIntegerToUInt(x), x);		// x = x!

    	longIntegerMultiply(x, t, t);						// t = x!(y-x)!
    	longIntegerDivide(y, t, t);							// t = y! / [x! * (y -x)!]

		convertLongIntegerToLongIntegerRegister(t, REGISTER_X);
		longIntegerFree(t);
	}

	longIntegerFree(x);
    longIntegerFree(y);
}

/*
 * Calculate Cyx
 *
 * This function is called when both X and Y are short integers.
 */
void cyxShoI(void)
{
	int16_t x_sign, y_sign;
	uint64_t x_value, y_value;

	convertShortIntegerRegisterToUInt64(REGISTER_X, &x_sign, &x_value);
	convertShortIntegerRegisterToUInt64(REGISTER_Y, &y_sign, &y_value);

	if(x_sign==1 || y_sign==1)
        cpyxError(INVALID_DATA_ERROR);
	else if(x_value > 20 || y_value > 20)
        cpyxError(DATA_OUT_OF_RANGE_ERROR);
	else if(y_value < x_value)
        cpyxError(INVALID_DATA_CMP_ERROR);
	else
	{
		uint64_t value = fact_uint64(y_value) / ( fact_uint64(x_value) * fact_uint64(y_value - x_value));

		if(value > shortIntegerMask)
			fnSetFlag(FLAG_OVERFLOW);

  		convertUInt64ToShortIntegerRegister(0, value, getRegisterTag(REGISTER_X), REGISTER_X);
	}
}

/********************************************//**
 * \brief regX ==> regL and Pxy(regX, RegY) ==> regX
 * enables stack lift and refreshes the stack.
 * P(n,k) = n! / (n-k)!
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnPyx(uint16_t unusedParamButMandatory)
{
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    if(!IS_INTEGER(REGISTER_X) || !IS_INTEGER(REGISTER_Y))
        cpyxError(INVALID_DATA_TYPE_ERROR);
    else
    {
        if(IS_LONG_INTEGER(REGISTER_X) || IS_LONG_INTEGER(REGISTER_Y))
            pyxLonI();
        else
            pyxShoI();

        adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
        adjustResult(REGISTER_Y, true, true, REGISTER_Y, -1, -1);
    }
}

/*
 * Calculate Pyx
 *
 * This function is called when one or both of X and Y are long integer(s).
 */
void pyxLonI(void)
{
    longInteger_t x, y;

    if(IS_SHORT_INTEGER(REGISTER_X))
        convertShortIntegerRegisterToLongInteger(REGISTER_X, x);
    else
        convertLongIntegerRegisterToLongInteger(REGISTER_X, x);

    if(IS_SHORT_INTEGER(REGISTER_Y))
        convertShortIntegerRegisterToLongInteger(REGISTER_Y, y);
    else
        convertLongIntegerRegisterToLongInteger(REGISTER_Y, y);

    if(longIntegerIsNegative(x) || longIntegerIsNegative(y))
        cpyxError(INVALID_DATA_ERROR);
    else if(longIntegerCompareUInt(x, 450) > 0 || longIntegerCompareUInt(y, 450) > 0)
        cpyxError(DATA_OUT_OF_RANGE_ERROR);
    else if(longIntegerCompare(y, x) < 0)
        cpyxError(INVALID_DATA_CMP_ERROR);
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

/*
 * Calculate Pyx
 *
 * This function is called when both X and Y are short integers.
 */
void pyxShoI(void)
{
    int16_t x_sign, y_sign;
    uint64_t x_value, y_value;

    convertShortIntegerRegisterToUInt64(REGISTER_X, &x_sign, &x_value);
    convertShortIntegerRegisterToUInt64(REGISTER_Y, &y_sign, &y_value);

    if(x_sign==1 || y_sign==1)
        cpyxError(INVALID_DATA_ERROR);
    else if(x_value > 20 || y_value > 20)
        cpyxError(DATA_OUT_OF_RANGE_ERROR);
    else if(y_value < x_value)
        cpyxError(INVALID_DATA_CMP_ERROR);
    else
    {
        uint64_t value = fact_uint64(y_value) / fact_uint64(y_value - x_value);

        if(value > shortIntegerMask)
            fnSetFlag(FLAG_OVERFLOW);

        convertUInt64ToShortIntegerRegister(0, value, getRegisterTag(REGISTER_X), REGISTER_X);
    }
}
