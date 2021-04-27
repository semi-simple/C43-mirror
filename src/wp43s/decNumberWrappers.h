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
 * \file decNumberWrappers.h
 ***********************************************/
#ifndef DECNUMBERWRAPPERS_H
#define DECNUMBERWRAPPERS_H

#include "defines.h"

#if (IBM_DECIMAL == 1)

#include "decimal128.h"
#include "decimal64.h"
#include "decDouble.h"
#include "decQuad.h"
#include "decNumberWrappers.h"
extern int decGetInt(const decNumber *x); // Because decNumberToInt32 seems buggy! Needs more investigation

typedef struct {
  int32_t digits;      // Count of digits in the coefficient; >0
  int32_t exponent;    // Unadjusted exponent, unbiased, in
                       // range: -1999999997 through 999999999
  uint8_t bits;        // Indicator bits (see above)
                       // Coefficient, from least significant unit
  decNumberUnit lsu[(39+DECDPUN-1)/DECDPUN]; // 39 = 27 + 1*12
} real39_t; // used for trigonometric functions borrowed from WP34S


typedef struct {
  int32_t digits;      // Count of digits in the coefficient; >0
  int32_t exponent;    // Unadjusted exponent, unbiased, in
                       // range: -1999999997 through 999999999
  uint8_t bits;        // Indicator bits (see above)
                       // Coefficient, from least significant unit
  decNumberUnit lsu[(51+DECDPUN-1)/DECDPUN]; // 51 = 39 + 1*12
} real51_t; // used for trigonometric functions borrowed from WP34S


//typedef struct {
//  int32_t digits;      // Count of digits in the coefficient; >0
//  int32_t exponent;    // Unadjusted exponent, unbiased, in
//                       // range: -1999999997 through 999999999
//  uint8_t bits;        // Indicator bits (see above)
//                       // Coefficient, from least significant unit
//  decNumberUnit lsu[(75+DECDPUN-1)/DECDPUN]; // 75 = 39 + 3*12
//} real75_t; // used in SLVQ

typedef struct {
  int32_t digits;      // Count of digits in the coefficient; >0
  int32_t exponent;    // Unadjusted exponent, unbiased, in
                       // range: -1999999997 through 999999999
  uint8_t bits;        // Indicator bits (see above)
                       // Coefficient, from least significant unit
  decNumberUnit lsu[(1071+DECDPUN-1)/DECDPUN]; // 1071 = 39 + 86*12
} real1071_t; // used for radian angle reduction


//typedef struct {
//  int32_t digits;      // Count of digits in the coefficient; >0
//  int32_t exponent;    // Unadjusted exponent, unbiased, in
//                       // range: -1999999997 through 999999999
//  uint8_t bits;        // Indicator bits (see above)
//                       // Coefficient, from least significant unit
//  decNumberUnit lsu[(2139+DECDPUN-1)/DECDPUN]; // 2139 = 39 + 175*12
//} real2139_t;

#define realContext_t                                          decContext
#define real34_t                                               decQuad      // 34 digits and 128 bits = 16 bytes

typedef struct {
  real34_t real, imag;
} complex34_t;

#define real_t                                                 decNumber

#define REAL_SIZE                                              TO_BLOCKS(sizeof(real_t))
#define REAL34_SIZE                                            TO_BLOCKS(sizeof(real34_t))
#define REAL39_SIZE                                            TO_BLOCKS(sizeof(real39_t))
#define REAL51_SIZE                                            TO_BLOCKS(sizeof(real51_t))
#define REAL1071_SIZE                                          TO_BLOCKS(sizeof(real1071_t))
#define COMPLEX34_SIZE                                         TO_BLOCKS(sizeof(complex34_t))

#define POINTER_TO_LOCAL_REGISTER(a)                           ((registerHeader_t *)(currentLocalRegisters + (a)))

#define POINTER_TO_NAMED_VARIABLE(a)                           ((registerHeader_t *)(allNamedVariablePointer + 1 + 2u*(a)))
#define POINTER_TO_POINTER_TO_NAMED_VARIABLE_NAME(a)           ((dataBlock_t *)(allNamedVariablePointer + 2u + 2u*(a)))
#define POINTER_TO_NAMED_VARIABLE_NAME(a)                      ((char        *)(*POINTER_TO_POINTER_TO_NAMED_VARIABLE_NAME(a) << 1))


#define REGISTER_DATA(a)                                       ((dataBlock_t *)(getRegisterDataPointer(a)))
#define REGISTER_REAL34_DATA(a)                                ((real34_t    *)(getRegisterDataPointer(a)))
#define REGISTER_IMAG34_DATA(a)                                ((real34_t    *)(getRegisterDataPointer(a) + REAL34_SIZE))
#define REGISTER_COMPLEX34_DATA(a)                             ((complex34_t *)(getRegisterDataPointer(a)))

#define REGISTER_STRING_DATA(a)                                ((char        *)(getRegisterDataPointer(a) + 1)) // Memory pointer to the string of a register

#define REGISTER_CONFIG_DATA(a)                                ((dtConfigDescriptor_t *)(getRegisterDataPointer(a)))

#define REGISTER_REAL34_MATRIX_DBLOCK(a)                      ((dataBlock_t *)(getRegisterDataPointer(a)))
#define REGISTER_REAL34_MATRIX_M_ELEMENTS(a)                  ((real34_t *)((void *)getRegisterDataPointer(a) + sizeof(dataBlock_t)))
#define REGISTER_REAL34_MATRIX(a)                             ((real34Matrix_t *)(getRegisterDataPointer(a)))

#define REGISTER_SHORT_INTEGER_DATA(a)                         ((uint64_t    *)(getRegisterDataPointer(a)))
#define VARIABLE_REAL34_DATA(a)                                ((real34_t    *)(a))
#define VARIABLE_IMAG34_DATA(a)                                ((real34_t    *)((dataBlock_t *)(a) + REAL34_SIZE))
#define VARIABLE_COMPLEX34_DATA(a)                             ((complex34_t *)(a))


#define complex34ChangeSign(operand)                           {real34ChangeSign((real34_t *)(operand)); \
                                                                real34ChangeSign((real34_t *)((dataBlock_t *)(operand) + REAL34_SIZE)); \
                                                               }
#define complex34Copy(source, destination)                     {  *(uint64_t *)(destination)     =   *(uint64_t *)(source); \
                                                                *(((uint64_t *)(destination))+1) = *(((uint64_t *)(source))+1); \
                                                                *(((uint64_t *)(destination))+2) = *(((uint64_t *)(source))+2); \
                                                                *(((uint64_t *)(destination))+3) = *(((uint64_t *)(source))+3); \
                                                               }
#define int32ToReal34(source, destination)                     decQuadFromInt32         ((real34_t *)(destination), source)
#define real34Add(operand1, operand2, res)                     decQuadAdd               ((real34_t *)(res), (real34_t *)(operand1), (real34_t *)(operand2), &ctxtReal34)
#define real34ChangeSign(operand)                              ((real34_t *)(operand))->bytes[15] ^= 0x80
#define real34Compare(operand1, operand2, res)                 decQuadCompare           ((real34_t *)(res), (real34_t *)(operand1), (real34_t *)(operand2), &ctxtReal34)
//#define real34Copy(source, destination)                        decQuadCopy            (destination, source)
//#define real34Copy(source, destination)                        xcopy(destination, source, TO_BYTES(REAL34_SIZE))
#define real34Copy(source, destination)                        {*(uint64_t *)(destination) = *(uint64_t *)(source); \
                                                                *(((uint64_t *)(destination))+1) = *(((uint64_t *)(source))+1); \
                                                               }
#define real34CopyAbs(source, destination)                     decQuadCopyAbs           (destination, source)
#define real34Divide(operand1, operand2, res)                  decQuadDivide            ((real34_t *)(res), (real34_t *)(operand1), (real34_t *)(operand2), &ctxtReal34)
#define real34DivideRemainder(operand1, operand2, res)         decQuadRemainder         ((real34_t *)(res), (real34_t *)(operand1), (real34_t *)(operand2), &ctxtReal34)
#define real34FMA(factor1, factor2, term, res)                 decQuadFMA               ((real34_t *)(res), (real34_t *)(factor1),  (real34_t *)(factor2),  (real34_t *)(term), &ctxtReal34)
#define real34GetCoefficient(source, destination)              decQuadGetCoefficient    ((real34_t *)(source), (uint8_t *)(destination))
#define real34GetExponent(source)                              decQuadGetExponent       ((real34_t *)(source))
#define real34IsInfinite(source)                               decQuadIsInfinite        ((real34_t *)(source))
#define real34IsNaN(source)                                    decQuadIsNaN             ((real34_t *)(source))
#define real34IsNegative(source)                               (((((real34_t *)(source))->bytes[15]) & 0x80) == 0x80)
#define real34IsPositive(source)                               (((((real34_t *)(source))->bytes[15]) & 0x80) == 0x00)
#define real34IsSpecial(source)                                (decQuadIsNaN((real34_t *)(source))   || decQuadIsSignaling((real34_t *)(source))   || decQuadIsInfinite((real34_t *)(source)))
#define real34IsZero(source)                                   decQuadIsZero            ((real34_t *)(source))
#define real34Minus(operand, res)                              decQuadMinus             ((real34_t *)(res), (real34_t *)(operand), &ctxtReal34)
#define real34Multiply(operand1, operand2, res)                decQuadMultiply          ((real34_t *)(res), (real34_t *)(operand1), (real34_t *)(operand2), &ctxtReal34)
#define real34NextMinus(operand, res)                          decQuadNextPlus          ((real34_t *)(res), (real34_t *)(operand), &ctxtReal34)
#define real34NextPlus(operand, res)                           decQuadNextPlus          ((real34_t *)(res), (real34_t *)(operand), &ctxtReal34)
#define real34Plus(operand, res)                               decQuadPlus              ((real34_t *)(res), (real34_t *)(operand), &ctxtReal34)
#define real34SetNegativeSign(operand)                         ((real34_t *)(operand))->bytes[15] |= 0x80
#define real34SetPositiveSign(operand)                         ((real34_t *)(operand))->bytes[15] &= 0x7F
#define real34Subtract(operand1, operand2, res)                decQuadSubtract          ((real34_t *)(res), (real34_t *)(operand1), (real34_t *)(operand2), &ctxtReal34)
#define real34ToInt32(source)                                  decQuadToInt32           ((real34_t *)(source), &ctxtReal34, DEC_ROUND_DOWN)
#define real34ToIntegralValue(source, destination, mode)       decQuadToIntegralValue   ((real34_t *)(destination), (real34_t *)(source), &ctxtReal34, mode)
#define real34ToReal(source, destination)                      decQuadToNumber          ((real34_t *)(source), destination)
#define real34ToString(source, destination)                    decQuadToString          ((real34_t *)(source), destination)
#define real34ToUInt32(source)                                 decQuadToUInt32          ((real34_t *)(source), &ctxtReal34, DEC_ROUND_DOWN)
#define real34Zero(destination)                                decQuadZero              (destination)
//#define real34Zero(destination)                                xcopy                    (destination, const34_0, TO_BYTES(REAL34_SIZE))
/*#define real34Zero(destination)                                {  *(uint64_t *)(destination)     =   *(uint64_t *)const34_0; \
                                                                  *(((uint64_t *)(destination))+1) = *(((uint64_t *)const34_0)+1); \
                                                               }*/
#define stringToReal34(source, destination)                    decQuadFromString        ((real34_t *)(destination), source, &ctxtReal34)
#define uInt32ToReal34(source, destination)                    decQuadFromUInt32        ((real34_t *)(destination), source)








#define int32ToReal(source, destination)                       decNumberFromInt32       (destination, source)
#define realAdd(operand1, operand2, res, ctxt)                 decNumberAdd             (res, operand1, operand2, ctxt)
#define realChangeSign(operand)                                (operand)->bits ^= 0x80
#define realCompare(operand1, operand2, res, ctxt)             decNumberCompare         (res, operand1, operand2, ctxt)
#define realCopy(source, destination)                          decNumberCopy            (destination, source)
#define realCopyAbs(source, destination)                       decNumberCopyAbs         (destination, source)
#define realDivide(operand1, operand2, res, ctxt)              decNumberDivide          (res, operand1, operand2, ctxt)
#define realDivideRemainder(operand1, operand2, res, ctxt)     decNumberRemainder       (res, operand1, operand2, ctxt)
#define realExp(operand, res, ctxt)                            decNumberExp             (res, operand, ctxt)
#define realFMA(factor1, factor2, term, res, ctxt)             decNumberFMA             (res, factor1,  factor2,  term, ctxt)
#define realGetCoefficient(source, destination)                decNumberGetBCD          (source, (uint8_t *)(destination))
//#define realGetExponent(source)                                ((source)->digits)
#define realGetExponent(source)                                ((source)->digits + (source)->exponent - 1)
#define realGetSign(source)                                    (((source)->bits) & 0x80) // 0x80=negative and 0x00=positive
#define realIsInfinite(source)                                 decNumberIsInfinite      (source)
#define realIsNaN(source)                                      decNumberIsNaN           (source)
#define realIsNegative(source)                                 ((((source)->bits) & 0x80) == 0x80)
#define realIsPositive(source)                                 ((((source)->bits) & 0x80) == 0x00)
#define realIsSpecial(source)                                  decNumberIsSpecial       (source)
#define realIsZero(source)                                     decNumberIsZero          (source)
#define realMinus(operand, res, ctxt)                          decNumberMinus           (res, operand, ctxt)
#define realMultiply(operand1, operand2, res, ctxt)            decNumberMultiply        (res, operand1, operand2, ctxt)
#define realNextToward(from, toward, res, ctxt)                decNumberNextToward      (res, from,     toward,   ctxt)
#define realPlus(operand, res, ctxt)                           decNumberPlus            (res, operand, ctxt)
#define realPower(operand1, operand2, res, ctxt)               decNumberPower           (res, operand1, operand2, ctxt)
#define realSetNegativeSign(operand)                           (operand)->bits |= 0x80
#define realSetPositiveSign(operand)                           (operand)->bits &= 0x7F
#define realSquareRoot(operand, res, ctxt)                     decNumberSquareRoot      (res, operand, ctxt)
#define realSubtract(operand1, operand2, res, ctxt)            decNumberSubtract        (res, operand1, operand2, ctxt)
#define realToInt32(source, destination)                       {enum rounding savedRoundingMode; \
                                                                real_t tmp; \
                                                                savedRoundingMode = ctxtReal39.round; \
                                                                ctxtReal39.round = DEC_ROUND_DOWN; \
                                                                ctxtReal39.status = 0; \
                                                                decNumberToIntegralValue(&tmp, source, &ctxtReal39); \
                                                                destination = decGetInt(&tmp); \
                                                                ctxtReal39.round = savedRoundingMode; \
                                                               }
#define realToReal34(source, destination)                      decQuadFromNumber        ((real34_t *)(destination), source, &ctxtReal34)
#define realToString(source, destination)                      decNumberToString        ((real_t *)(source), destination)
#define realZero(destination)                                  decNumberZero            (destination)
#define stringToReal(source, destination, ctxt)                decNumberFromString      (destination, source, ctxt)
#define uInt32ToReal(source, destination)                      decNumberFromUInt32      (destination, source)

#endif // (IBM_DECIMAL == 1)

#endif // DECNUMBERWRAPPERS_H
