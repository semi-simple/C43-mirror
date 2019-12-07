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

typedef struct {
  int32_t digits;      // Count of digits in the coefficient; >0
  int32_t exponent;    // Unadjusted exponent, unbiased, in
                       // range: -1999999997 through 999999999
  uint8_t bits;        // Indicator bits (see above)
                       // Coefficient, from least significant unit
  decNumberUnit lsu[(51+DECDPUN-1)/DECDPUN];
} real51_t; // used for trigonometric functions borrowed from WP34S

typedef struct {
  int32_t digits;      // Count of digits in the coefficient; >0
  int32_t exponent;    // Unadjusted exponent, unbiased, in
                       // range: -1999999997 through 999999999
  uint8_t bits;        // Indicator bits (see above)
                       // Coefficient, from least significant unit
  decNumberUnit lsu[(75+DECDPUN-1)/DECDPUN];
} real75_t; // used in SLVQ

typedef struct {
  int32_t digits;      // Count of digits in the coefficient; >0
  int32_t exponent;    // Unadjusted exponent, unbiased, in
                       // range: -1999999997 through 999999999
  uint8_t bits;        // Indicator bits (see above)
                       // Coefficient, from least significant unit
  decNumberUnit lsu[(459+DECDPUN-1)/DECDPUN];
} real459_t; // used for radian angle reduction

typedef struct {
  int32_t digits;      // Count of digits in the coefficient; >0
  int32_t exponent;    // Unadjusted exponent, unbiased, in
                       // range: -1999999997 through 999999999
  uint8_t bits;        // Indicator bits (see above)
                       // Coefficient, from least significant unit
  decNumberUnit lsu[(855+DECDPUN-1)/DECDPUN];
} real855_t;

#define realContext_t                                     decContext
#define real16_t                                          decDouble    // 16 digits
#define real34_t                                          decQuad      // 34 digits
#define real39_t                                          decNumber    // 39 digits: Intermediate calculations
#define real_t                                            decNumber
typedef struct {real16_t real, imag;}                     complex16_t;
typedef struct {real34_t real, imag;}                     complex34_t;
typedef struct {real39_t real, imag;}                     complex39_t;
typedef struct {real51_t real, imag;}                     complex51_t;
typedef struct {real75_t real, imag;}                     complex75_t;

#define REAL16_SIZE                                       sizeof(real16_t)
#define REAL34_SIZE                                       sizeof(real34_t)
#define REAL39_SIZE                                       sizeof(real39_t)
#define REAL51_SIZE                                       sizeof(real51_t)
#define REAL75_SIZE                                       sizeof(real75_t)
#define REAL459_SIZE                                      sizeof(real459_t)
#define REAL855_SIZE                                      sizeof(real855_t)
#define COMPLEX16_SIZE                                    sizeof(complex16_t)
#define COMPLEX34_SIZE                                    sizeof(complex34_t)
#define COMPLEX39_SIZE                                    sizeof(complex39_t)
#define COMPLEX51_SIZE                                    sizeof(complex51_t)
#define COMPLEX75_SIZE                                    sizeof(complex75_t)

#define POINTER_TO_LOCAL_FLAGS                            ((dataSize_t  *)(allLocalRegisterPointer))
#define POINTER_TO_LOCAL_REGISTER(a)                      ((registerDescriptor_t *)(allLocalRegisterPointer + sizeof(dataSize_t) + 4u*(a)))

#define POINTER_TO_NAMED_VARIABLE(a)                      ((registerDescriptor_t *)(allNamedVariablePointer      + 6u*(a)))
#define POINTER_TO_POINTER_TO_NAMED_VARIABLE_NAME(a)      ((uint16_t    *)(allNamedVariablePointer + 4u + 6u*(a)))
#define POINTER_TO_NAMED_VARIABLE_NAME(a)                 ((char        *)(*POINTER_TO_POINTER_TO_NAMED_VARIABLE_NAME(a) << 1))
#define RAM_REAL16(a)                                     ((real16_t    *)(ram + (a)))
#define RAM_REAL34(a)                                     ((real34_t    *)(ram + (a)))


#define REGISTER_DATA(a)                                  ((void        *)(getRegisterDataPointer(a)))
#define REGISTER_REAL16_DATA(a)                           ((real16_t    *)(getRegisterDataPointer(a)))
#define REGISTER_IMAG16_DATA(a)                           ((real16_t    *)(getRegisterDataPointer(a) + REAL16_SIZE))
#define REGISTER_COMPLEX16_DATA(a)                        ((complex16_t *)(getRegisterDataPointer(a)))
#define REGISTER_REAL34_DATA(a)                           ((real34_t    *)(getRegisterDataPointer(a)))
#define REGISTER_IMAG34_DATA(a)                           ((real34_t    *)(getRegisterDataPointer(a) + REAL34_SIZE))
#define REGISTER_COMPLEX34_DATA(a)                        ((complex34_t *)(getRegisterDataPointer(a)))

#define REGISTER_STRING_DATA(a)                           ((char        *)(getRegisterDataPointer(a) + sizeof(dataSize_t))) // Memory pointer to the string of a register
#define REGISTER_DATA_MAX_LEN(a)                          ((dataSize_t  *)(getRegisterDataPointer(a)))                      // Memory pointer to the lenght of string or long integer

#define REGISTER_SHORT_INTEGER_DATA(a)                    ((uint64_t    *)(getRegisterDataPointer(a)))
#define VARIABLE_REAL16_DATA(a)                           ((real16_t    *)(a))
#define VARIABLE_IMAG16_DATA(a)                           ((real16_t    *)((char *)(a) + REAL16_SIZE))
#define VARIABLE_COMPLEX16_DATA(a)                        ((complex16_t *)(a))
#define VARIABLE_REAL34_DATA(a)                           ((real34_t    *)(a))
#define VARIABLE_IMAG34_DATA(a)                           ((real34_t    *)((char *)(a) + REAL34_SIZE))
#define VARIABLE_COMPLEX34_DATA(a)                        ((complex34_t *)(a))



#define complex16ChangeSign(operand)                           {real16ChangeSign((real16_t *)(operand)); \
                                                                real16ChangeSign((real16_t *)((char *)(operand) + REAL16_SIZE)); \
                                                               }
#define complex16Copy(source, destination)                     {*(uint64_t *)(destination) = *(uint64_t *)(source); \
                                                                *(((uint64_t *)(destination))+1) = *(((uint64_t *)(source))+1); \
                                                               }
#define int32ToReal16(source, destination)                     decDoubleFromInt32       ((real16_t *)(destination), source)
#define real16Add(operand1, operand2, res)                     decDoubleAdd             ((real16_t *)(res), (real16_t *)(operand1), (real16_t *)(operand2), &ctxtReal16)
#define real16ChangeSign(operand)                              ((real16_t *)(operand))->bytes[ 7] ^= 0x80
#define real16Compare(operand1, operand2, res)                 decDoubleCompare         ((real16_t *)(res), (real16_t *)(operand1), (real16_t *)(operand2), &ctxtReal16)
//#define real16Copy(source, destination)                        decDoubleCopy            (destination, source)
//#define real16Copy(source, destination)                        memcpy(destination, source, REAL16_SIZE)
#define real16Copy(source, destination)                        *(uint64_t *)(destination) = *(uint64_t *)(source);
#define real16CopyAbs(source, destination)                     decDoubleCopyAbs         (destination, source)
#define real16Divide(operand1, operand2, res)                  decDoubleDivide          ((real16_t *)(res), (real16_t *)(operand1), (real16_t *)(operand2), &ctxtReal16)
#define real16DivideRemainder(operand1, operand2, res)         decDoubleRemainder       ((real16_t *)(res), (real16_t *)(operand1), (real16_t *)(operand2), &ctxtReal16)
#define real16FMA(factor1, factor2, term, res)                 decDoubleFMA             ((real16_t *)(res), (real16_t *)(factor1),  (real16_t *)(factor2),  (real16_t *)(term), &ctxtReal16)
#define real16GetCoefficient(source, destination)              decDoubleGetCoefficient  ((real16_t *)(source), (uint8_t *)(destination))
#define real16GetExponent(source)                              decDoubleGetExponent     ((real16_t *)(source))
#define real16IsInfinite(source)                               decDoubleIsInfinite      ((real16_t *)(source))
#define real16IsNaN(source)                                    decDoubleIsNaN           ((real16_t *)(source))
#define real16IsNegative(source)                               (((((real16_t *)(source))->bytes[7]) & 0x80) == 0x80)
#define real16IsPositive(source)                               (((((real16_t *)(source))->bytes[7]) & 0x80) == 0x00)
#define real16IsSpecial(source)                                (decDoubleIsNaN((real16_t *)(source)) || decDoubleIsSignaling((real16_t *)(source)) || decDoubleIsInfinite((real16_t *)(source)))
#define real16IsZero(source)                                   decDoubleIsZero          ((real16_t *)(source))
#define real16Minus(operand, res)                              decDoubleMinus           ((real16_t *)(res), (real16_t *)(operand), &ctxtReal16)
#define real16Multiply(operand1, operand2, res)                decDoubleMultiply        ((real16_t *)(res), (real16_t *)(operand1), (real16_t *)(operand2), &ctxtReal16)
#define real16NextMinus(operand, res)                          decDoubleNextPlus        ((real16_t *)(res), (real16_t *)(operand), &ctxtReal16)
#define real16NextPlus(operand, res)                           decDoubleNextPlus        ((real16_t *)(res), (real16_t *)(operand), &ctxtReal16)
#define real16Plus(operand, res)                               decDoublePlus            ((real16_t *)(res), (real16_t *)(operand), &ctxtReal16)
#define real16SetNegativeSign(operand)                         ((real16_t *)(operand))->bytes[7] |= 0x80
#define real16SetPositiveSign(operand)                         ((real16_t *)(operand))->bytes[7] &= 0x7F
#define real16Subtract(operand1, operand2, res)                decDoubleSubtract        ((real16_t *)(res), (real16_t *)(operand1), (real16_t *)(operand2), &ctxtReal16)
#define real16ToReal(source, destination)                      decDoubleToNumber        ((real16_t *)(source), (real_t *)(destination))
#define real16ToInt32(source)                                  decDoubleToInt32         ((real16_t *)(source), &ctxtReal16, DEC_ROUND_DOWN)
#define real16ToIntegralValue(source, destination, mode)       decDoubleToIntegralValue ((real16_t *)(destination), (real16_t *)(source), &ctxtReal16, mode)
#define real16ToReal34(source, destination)                    decDoubleToWider         ((real16_t *)(source), (real34_t *)(destination))
#define real16ToString(source, destination)                    decDoubleToString        ((real16_t *)(source), destination)
//#define real16ToUInt32(source)                                 decDoubleToUInt32        ((real16_t *)(source), &ctxtReal16, DEC_ROUND_DOWN)
#define real16Zero(destination)                                decDoubleZero            (destination)
//#define real16Zero(destination)                                memcpy                   (destination, const16_0, REAL16_SIZE)
//#define real16Zero(destination)                                *(uint64_t *)(destination) = *(uint64_t *)const16_0;
#define uInt32ToReal16(source, destination)                    decDoubleFromUInt32      ((real16_t *)(destination), source)

#define stringToReal16(source, destination)                    decDoubleFromString      ((real16_t *)(destination), source, &ctxtReal16)








#define complex34ChangeSign(operand)                           {real34ChangeSign((real34_t *)(operand)); \
                                                                real34ChangeSign((real34_t *)((char *)(operand) + REAL34_SIZE)); \
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
//#define real34Copy(source, destination)                        memcpy(destination, source, REAL34_SIZE)
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
#define real34ToReal16(source, destination)                    decDoubleFromWider       ((real16_t *)(destination), (real34_t *)(source), &ctxtReal16)
#define real34ToReal(source, destination)                      decQuadToNumber          ((real34_t *)(source), (real_t *)(destination))
#define real34ToString(source, destination)                    decQuadToString          ((real34_t *)(source), destination)
#define real34ToUInt32(source)                                 decQuadToUInt32          ((real34_t *)(source), &ctxtReal34, DEC_ROUND_DOWN)
#define real34Zero(destination)                                decQuadZero              (destination)
//#define real34Zero(destination)                                memcpy                   (destination, const34_0, REAL34_SIZE)
/*#define real34Zero(destination)                                {  *(uint64_t *)(destination)     =   *(uint64_t *)const34_0; \
                                                                  *(((uint64_t *)(destination))+1) = *(((uint64_t *)const34_0)+1); \
                                                               }*/
#define stringToReal34(source, destination)                    decQuadFromString        ((real34_t *)(destination), source, &ctxtReal34)
#define uInt32ToReal34(source, destination)                    decQuadFromUInt32        ((real34_t *)(destination), source)








#define complexCopy(source, destination, size)                 memcpy                   (destination, source, size)
#define int32ToReal(source, destination)                       decNumberFromInt32       ((real_t *)(destination), source)
#define realAdd(operand1, operand2, res, ctxt)                 decNumberAdd             ((real_t *)(res), (real_t *)(operand1), (real_t *)(operand2), ctxt)
#define realChangeSign(operand)                                ((real_t *)(operand))->bits ^= 0x80
#define realCompare(operand1, operand2, res, ctxt)             decNumberCompare         ((real_t *)(res), (real_t *)(operand1), (real_t *)(operand2), ctxt)
#define realCopy(source, destination)                          decNumberCopy            ((real_t *)(destination), (real_t *)(source))
#define realCopyAbs(source, destination)                       decNumberCopyAbs         ((real_t *)(destination), (real_t *)(source))
#define realDivide(operand1, operand2, res, ctxt)              decNumberDivide          ((real_t *)(res), (real_t *)(operand1), (real_t *)(operand2), ctxt)
#define realDivideRemainder(operand1, operand2, res, ctxt)     decNumberRemainder       ((real_t *)(res), (real_t *)(operand1), (real_t *)(operand2), ctxt)
#define realExp(operand, res, ctxt)                            decNumberExp             ((real_t *)(res), (real_t *)(operand), ctxt)
#define realFMA(factor1, factor2, term, res, ctxt)             decNumberFMA             ((real_t *)(res), (real_t *)(factor1),  (real_t *)(factor2),  (real_t *)(term), ctxt)
#define realGetCoefficient(source, destination)                decNumberGetBCD          ((real_t *)(source), (uint8_t *)(destination))
#define realGetExponent(source)                                (((real_t *)(source))->digits)
#define realIsInfinite(source)                                 decNumberIsInfinite      ((real_t *)(source))
#define realIsNaN(source)                                      decNumberIsNaN           ((real_t *)(source))
#define realIsNegative(source)                                 (((((real_t *)(source))->bits) & 0x80) == 0x80)
#define realIsPositive(source)                                 (((((real_t *)(source))->bits) & 0x80) == 0x00)
#define realIsSpecial(source)                                  decNumberIsSpecial       ((real_t *)(source))
#define realIsZero(source)                                     decNumberIsZero          ((real_t *)(source))
#define realMinus(operand, res, ctxt)                          decNumberMinus           ((real_t *)(res), (real_t *)(operand), ctxt)
#define realMultiply(operand1, operand2, res, ctxt)            decNumberMultiply        ((real_t *)(res), (real_t *)(operand1), (real_t *)(operand2), ctxt)
#define realNextToward(from, toward, res, ctxt)                decNumberNextToward      ((real_t *)(res), (real_t *)(from),     (real_t *)(toward),   ctxt)
#define realPlus(operand, res, ctxt)                           decNumberPlus            ((real_t *)(res), (real_t *)(operand), ctxt)
#define realPower(operand1, operand2, res, ctxt)               decNumberPower           ((real_t *)(res), (real_t *)(operand1), (real_t *)(operand2), ctxt)
#define realSetNegativeSign(operand)                           ((real_t *)(operand))->bits |= 0x80
#define realSetPositiveSign(operand)                           ((real_t *)(operand))->bits &= 0x7F
#define realSign(source)                                       ((((real_t *)(source))->bits) & 0x80) // 0x80=negative and 0x00=positive
#define realSquareRoot(operand, res, ctxt)                     decNumberSquareRoot      ((real_t *)(res), (real_t *)(operand), ctxt)
#define realSubtract(operand1, operand2, res, ctxt)            decNumberSubtract        ((real_t *)(res), (real_t *)(operand1), (real_t *)(operand2), ctxt)
#define realToInt32(source, destination)                       {enum rounding savedRoundingMode; \
                                                                real51_t tmp; \
                                                                savedRoundingMode = ctxtReal51.round; \
                                                                ctxtReal51.round = DEC_ROUND_DOWN; \
                                                                decNumberRescale((real_t *)&tmp, (real_t *)(source), const_0, &ctxtReal51); \
                                                                (destination) = decNumberToInt32((real_t *)&tmp, &ctxtReal51); \
                                                                ctxtReal51.round = savedRoundingMode; \
                                                               }
#define realToIntegralValue(source, destination, mode, ctxt)   {enum rounding savedRoundingMode; \
                                                                savedRoundingMode = (ctxt)->round; \
                                                                (ctxt)->round = mode; \
                                                                decNumberToIntegralValue((real_t *)(destination), (real_t *)(source), ctxt); \
                                                                (ctxt)->round = savedRoundingMode; \
                                                               }
#define realToReal16(source, destination)                      decDoubleFromNumber      ((real16_t *)(destination), (real_t *)(source), &ctxtReal16)
#define realToReal34(source, destination)                      decQuadFromNumber        ((real34_t *)(destination), (real_t *)(source), &ctxtReal34)
#define realToString(source, destination)                      decNumberToString        ((real_t *)(source), destination)
#define realToUInt32(source, destination)                      {enum rounding savedRoundingMode; \
                                                                real51_t tmp; \
                                                                savedRoundingMode = ctxtReal51.round; \
                                                                ctxtReal51.round = DEC_ROUND_DOWN; \
                                                                decNumberRescale((real_t *)&tmp, (real_t *)(source), const_0, &ctxtReal51); \
                                                                (destination) = decNumberToUInt32((real_t *)&tmp, &ctxtReal51); \
                                                                ctxtReal51.round = savedRoundingMode; \
                                                               }
#define realZero(destination)                                  decNumberZero            ((real_t *)(destination))
#define stringToReal(source, destination, ctxt)                decNumberFromString      ((real_t *)(destination), source, ctxt)
#define uInt32ToReal(source, destination)                      decNumberFromUInt32      ((real_t *)(destination), source)
