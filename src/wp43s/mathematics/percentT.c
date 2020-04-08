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
 * \file percentT.c
 ***********************************************/

#include "wp43s.h"

static void percentTDataTypeError(void);

void (* const percentT[9][9])(void) = {
// regX |    regY ==>    1                       2                       3                       4                       5                       6                       7                       8                       9
//      V                Long integer            Real34                  Complex34               Time                    Date                    String                  Real34 mat              Complex34 mat           Short integer
/*  1 Long integer  */ { percentTLonILonI,       percentTRealLonI,       percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError },
/*  2 Real34        */ { percentTLonIReal,       percentTRealReal,       percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError },
/*  3 Complex34     */ { percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError },
/*  4 Time          */ { percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError },
/*  5 Date          */ { percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError },
/*  6 String        */ { percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError },
/*  7 Real34 mat    */ { percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError },
/*  8 Complex34 mat */ { percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError },
/*  9 Short integer */ { percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError,  percentTDataTypeError }
};

//=============================================================================
// Error handling
//-----------------------------------------------------------------------------

/********************************************//**
 * \brief Data type error in %T
 *
 * \param void
 * \return void
 ***********************************************/
static void percentTDataTypeError(void) {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);

#if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot raise %s", getRegisterDataTypeName(REGISTER_Y, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "to %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnPercentT:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
#endif
}

//=============================================================================
// Main function
//-----------------------------------------------------------------------------

/********************************************//**
 * \brief regX ==> regL and PercentT(regX, RegY) ==> regX
 * enables stack lift and refreshes the stack.
 * Calculate x*y/100
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnPercentT(uint16_t unusedParamButMandatory) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    percentT[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

    adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}

//=============================================================================
// T% calculation functions
//-----------------------------------------------------------------------------

static bool_t percentTReal(real_t *xReal, real_t *yReal, real_t *rReal, realContext_t *realContext) {
    bool_t result = true;
    /*
     * Check x and y
     */
    if(realIsZero(xReal) && realIsZero(yReal)) {
        if(getFlag(FLAG_DANGER)) {
            realCopy(const_NaN, rReal);
        }
        else {
            displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
#if (EXTRA_INFO_ON_CALC_ERROR == 1)
            showInfoDialog("In function fnPercentT:", "cannot divide x=0 by y=0", NULL, NULL);
#endif
            result = false;
        }
    }
    else if(realIsZero(yReal))
    {
        if(getFlag(FLAG_DANGER)) {
            realCopy((realIsPositive(xReal) ? const_plusInfinity : const_minusInfinity),rReal);
        }
        else {
            displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
#if (EXTRA_INFO_ON_CALC_ERROR == 1)
            showInfoDialog("In function fnPercentT:", "cannot divide a real by y=0", NULL, NULL);
#endif
            result = false;
        }
    }
    else {
        realMultiply(xReal, const_100, rReal, realContext); // rReal = xReal * 100.0
        realDivide(rReal, yReal, rReal, realContext);       // rReal = xReal * 100.0 / yReal
    }

    return result;
}

/********************************************//**
 * \brief PercentT(Y(long integer), X(long integer)) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void percentTLonILonI(void) {
    real_t xReal, yReal;
    real_t rReal;

    convertLongIntegerRegisterToReal(REGISTER_X, &xReal, &ctxtReal39);
    convertLongIntegerRegisterToReal(REGISTER_Y, &yReal, &ctxtReal39);

    if(percentTReal(&xReal, &yReal, &rReal, &ctxtReal34)) {
        reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
        realToReal34(&rReal, REGISTER_REAL34_DATA(REGISTER_X));
        setRegisterAngularMode(REGISTER_X, AM_NONE);
    }
}

/********************************************//**
 * \brief PercentT(Y(long integer), X(real34)) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void percentTLonIReal(void) {
    real_t xReal, yReal;
    real_t rReal;

    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &xReal);
    convertLongIntegerRegisterToReal(REGISTER_Y, &yReal, &ctxtReal39);

    if(percentTReal(&xReal, &yReal, &rReal, &ctxtReal34)) {
        realToReal34(&rReal, REGISTER_REAL34_DATA(REGISTER_X));
    }
}

/********************************************//**
 * \brief PercentT(Y(real34), X(long integer)) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void percentTRealLonI(void) {
    real_t xReal, yReal;
    real_t rReal;

    convertLongIntegerRegisterToReal(REGISTER_X, &xReal, &ctxtReal39);
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &yReal);

    if(percentTReal(&xReal, &yReal, &rReal, &ctxtReal34)) {
        reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
        realToReal34(&rReal, REGISTER_REAL34_DATA(REGISTER_X));
        setRegisterAngularMode(REGISTER_X, AM_NONE);
    }
}

/********************************************//**
 * \brief PercentT(Y(real34), X(real34)) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void percentTRealReal(void) {
    real_t xReal, yReal;
    real_t rReal;

    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &xReal);
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &yReal);

    if(percentTReal(&xReal, &yReal, &rReal, &ctxtReal34)) {
        realToReal34(&rReal, REGISTER_REAL34_DATA(REGISTER_X));
    }
}
