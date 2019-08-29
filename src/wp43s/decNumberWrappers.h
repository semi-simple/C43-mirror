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
  decNumberUnit lsu[(451+DECDPUN-1)/DECDPUN];
} real451_t;

#define realIc_t                                          decNumber    // Intermediate calculations
#define real16_t                                          decDouble    // 16 digits
#define real34_t                                          decQuad      // 34 digits
typedef struct {real16_t real, imag;}                     complex16_t;
typedef struct {real34_t real, imag;}                     complex34_t;

#define REAL16_SIZE                                       sizeof(real16_t)
#define REAL34_SIZE                                       sizeof(real34_t)
#define REAL451_SIZE                                      sizeof(real451_t)
#define COMPLEX16_SIZE                                    sizeof(complex16_t)
#define COMPLEX34_SIZE                                    sizeof(complex34_t)

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
#define REGISTER_LONG_INTEGER_DATA(a)                     ((uint8_t     *)(getRegisterDataPointer(a) + sizeof(dataSize_t))) // Memory pointer to the long integer of a register
#define REGISTER_DATA_MAX_LEN(a)                          ((dataSize_t  *)(getRegisterDataPointer(a)))                      // Memory pointer to the lenght of string or long integer

#define REGISTER_SHORT_INTEGER_DATA(a)                    ((uint64_t    *)(getRegisterDataPointer(a)))
#define VARIABLE_REAL16_DATA(a)                           ((real16_t    *)(a))
#define VARIABLE_IMAG16_DATA(a)                           ((real16_t    *)((char *)(a) + REAL16_SIZE))
#define VARIABLE_COMPLEX16_DATA(a)                        ((complex16_t *)(a))
#define VARIABLE_REAL34_DATA(a)                           ((real34_t    *)(a))
#define VARIABLE_IMAG34_DATA(a)                           ((real34_t    *)((char *)(a) + REAL34_SIZE))
#define VARIABLE_COMPLEX34_DATA(a)                        ((complex34_t *)(a))



#define real16Add(operand1, operand2, res)                     decDoubleAdd             ((real16_t *)res, (real16_t *)operand1, (real16_t *)operand2, &ctxtReal16)
#define real16ChangeSign(operand)                              ((real16_t *)operand)->bytes[ 7] ^= 0x80
#define real16Compare(operand1, operand2, res)                 decDoubleCompare         ((real16_t *)res, (real16_t *)operand1, (real16_t *)operand2, &ctxtReal16)
//#define real16Copy(source, destination)                        decDoubleCopy            (destination, source)
//#define real16Copy(source, destination)                        memcpy(destination, source, REAL16_SIZE)
#define real16Copy(source, destination)                        {*(uint64_t *)destination = *(uint64_t *)source;}
#define real16CopyAbs(source, destination)                     decDoubleCopyAbs         (destination, source)
#define real16Divide(operand1, operand2, res)                  decDoubleDivide          ((real16_t *)res, (real16_t *)operand1, (real16_t *)operand2, &ctxtReal16)
#define real16FMA(factor1, factor2, term, res)                 decDoubleFMA             ((real16_t *)res, (real16_t *)factor1,  (real16_t *)factor2,  (real16_t  *)term, &ctxtReal16)
#define real16GetCoefficient(source, destination)              decDoubleGetCoefficient  ((real16_t *)source, (uint8_t *)(destination))
#define real16GetExponent(source)                              decDoubleGetExponent     ((real16_t *)source)
#define real16IsInfinite(source)                               decDoubleIsInfinite      ((real16_t *)source)
#define real16IsNaN(source)                                    decDoubleIsNaN           ((real16_t *)source)
//#define real16IsNegative(source)                               decDoubleIsNegative      ((real16_t *)source)
#define real16IsNegative(source)                               (((((real16_t *)source)->bytes[7]) & 0x80) == 0x80)
//#define real16IsPositive(source)                               decDoubleIsPositive      ((real16_t *)source)
#define real16IsPositive(source)                               (((((real16_t *)source)->bytes[7]) & 0x80) == 0x00)
#define real16IsSpecial(source)                                (decDoubleIsNaN((real16_t *)source) || decDoubleIsSignaling((real16_t *)source) || decDoubleIsInfinite((real16_t *)source))
#define real16IsZero(source)                                   decDoubleIsZero          ((real16_t *)source)
#define real16Multiply(operand1, operand2, res)                decDoubleMultiply        ((real16_t *)res, (real16_t *)operand1, (real16_t *)operand2, &ctxtReal16)
#define real16Remainder(operand1, operand2, res)               decDoubleRemainder       ((real16_t *)res, (real16_t *)operand1, (real16_t *)operand2, &ctxtReal16)
#define real16SetNegativeSign(operand)                         ((real16_t *)operand)->bytes[7] |= 0x80
#define real16SetPositiveSign(operand)                         ((real16_t *)operand)->bytes[7] &= 0x7F
#define real16Subtract(operand1, operand2, res)                decDoubleSubtract        ((real16_t *)res, (real16_t *)operand1, (real16_t *)operand2, &ctxtReal16)
#define real16ToRealIc(source, destination)                    decDoubleToNumber        ((real16_t *)source, (realIc_t *)destination)
#define real16ToInt32(source)                                  decDoubleToInt32         ((real16_t *)source, &ctxtReal16, DEC_ROUND_DOWN)
#define real16ToIntegralValue(source, destination)             decDoubleToIntegralValue ((real16_t *)destination, (real16_t *)source, &ctxtReal16, DEC_ROUND_DOWN)
#define real16ToReal34(source, destination)                    decDoubleToWider         ((real16_t *)source, (real34_t *)destination)
#define real16ToString(source, destination)                    decDoubleToString        ((real16_t *)source, destination)
//#define real16Zero(destination)                                decDoubleZero            (destination)
//#define real16Zero(destination)                                memcpy                   (destination, const16_0, REAL16_SIZE)
#define real16Zero(destination)                                {*(uint64_t *)destination = *(uint64_t *)const16_0;}

#define complex16ChangeSign(operand)                           {real16ChangeSign((real16_t *)(operand)); real16ChangeSign((real16_t *)((char *)(operand) + REAL16_SIZE));}
#define complex16Copy(source, destination)                     {*(uint64_t *)destination = *(uint64_t *)source; *(((uint64_t *)destination)+1) = *(((uint64_t *)source)+1);}
#define complex16SetPositiveSign(operand)                      {real16SetPositiveSign((real16_t *)(operand)); real16SetPositiveSign((real16_t *)((char *)(operand) + REAL16_SIZE));}
#define complex16SetNegativeSign(operand)                      {real16SetNegativeSign((real16_t *)(operand)); real16SetNegativeSign((real16_t *)((char *)(operand) + REAL16_SIZE));}



#define real34Add(operand1, operand2, res)                     decQuadAdd               ((real34_t *)res, (real34_t *)operand1, (real34_t *)operand2, &ctxtReal34)
#define real34ChangeSign(operand)                              ((real34_t *)operand)->bytes[15] ^= 0x80
#define real34Compare(operand1, operand2, res)                 decQuadCompare           ((real34_t *)res, (real34_t *)operand1, (real34_t *)operand2, &ctxtReal34)
//#define real34Copy(source, destination)                        decQuadCopy            (destination, source)
//#define real34Copy(source, destination)                        memcpy(destination, source, REAL34_SIZE)
#define real34Copy(source, destination)                        {*(uint64_t *)destination = *(uint64_t *)source; *(((uint64_t *)destination)+1) = *(((uint64_t *)source)+1);}
#define real34CopyAbs(source, destination)                     decQuadCopyAbs           (destination, source)
#define real34Divide(operand1, operand2, res)                  decQuadDivide            ((real34_t *)res, (real34_t *)operand1, (real34_t *)operand2, &ctxtReal34)
#define real34FMA(factor1, factor2, term, res)                 decQuadFMA               ((real34_t *)res, (real34_t *)factor1,  (real34_t *)factor2,  (real34_t *)term, &ctxtReal34)
#define real34GetCoefficient(source, destination)              decQuadGetCoefficient    ((real34_t *)source, (uint8_t *)(destination))
#define real34GetExponent(source)                              decQuadGetExponent       ((real34_t *)source)
#define real34IsInfinite(source)                               decQuadIsInfinite        ((real34_t *)source)
#define real34IsNaN(source)                                    decQuadIsNaN             ((real34_t *)source)
//#define real34IsNegative(source)                               decQuadIsNegative        ((real34_t *)source)
#define real34IsNegative(source)                               (((((real34_t *)source)->bytes[15]) & 0x80) == 0x80)
//#define real34IsPositive(source)                               decQuadIsPositive        ((real34_t *)source)
#define real34IsPositive(source)                               (((((real34_t *)source)->bytes[15]) & 0x80) == 0x00)
#define real34IsSpecial(source)                                (decQuadIsNaN((real34_t *)source)   || decQuadIsSignaling((real34_t *)source)   || decQuadIsInfinite((real34_t *)source))
#define real34IsZero(source)                                   decQuadIsZero            ((real34_t  *)source)
#define real34Multiply(operand1, operand2, res)                decQuadMultiply          ((real34_t *)res, (real34_t *)operand1, (real34_t *)operand2, &ctxtReal34)
#define real34Remainder(operand1, operand2, res)               decQuadRemainder         ((real34_t *)res, (real34_t *)operand1, (real34_t *)operand2, &ctxtReal34)
#define real34SetNegativeSign(operand)                         ((real34_t *)operand)->bytes[15] |= 0x80
#define real34SetPositiveSign(operand)                         ((real34_t *)operand)->bytes[15] &= 0x7F
#define real34Subtract(operand1, operand2, res)                decQuadSubtract          ((real34_t *)res, (real34_t *)operand1, (real34_t *)operand2, &ctxtReal34)
#define real34ToInt32(source)                                  decQuadToInt32           ((real34_t *)source, &ctxtReal34, DEC_ROUND_DOWN)
#define real34ToIntegralValue(source, destination)             decQuadToIntegralValue   ((real34_t *)destination, (real34_t *)source, &ctxtReal34, DEC_ROUND_DOWN)
#define real34ToReal16(source, destination)                    decDoubleFromWider       ((real16_t *)destination, (real34_t *)source, &ctxtReal16)
#define real34ToRealIc(source, destination)                    decQuadToNumber          ((real34_t *)source, (realIc_t *)destination)
#define real34ToString(source, destination)                    decQuadToString          ((real34_t *)source, destination)
//#define real16Zero(destination)                                decQuadZero              (destination)
//#define real34Zero(destination)                                memcpy                   (destination, const34_0, REAL34_SIZE)
#define real34Zero(destination)                                {*(uint64_t *)destination = *(uint64_t *)const34_0; *(((uint64_t *)destination)+1) = *(((uint64_t *)const34_0)+1);}

#define complex34ChangeSign(operand)                           {real34ChangeSign((real34_t *)(operand)); real34ChangeSign((real34_t *)((char *)(operand) + REAL34_SIZE));}
#define complex34Copy(source, destination)                     {*(uint64_t *)destination = *(uint64_t *)source; *(((uint64_t *)destination)+1) = *(((uint64_t *)source)+1); *(((uint64_t *)destination)+2) = *(((uint64_t *)source)+2); *(((uint64_t *)destination)+3) = *(((uint64_t *)source)+3);}
#define complex34SetPositiveSign(operand)                      {real34SetPositiveSign((real34_t *)(operand)); real34SetPositiveSign((real34_t *)((char *)(operand) + REAL34_SIZE));}
#define complex34SetNegativeSign(operand)                      {real34SetNegativeSign((real34_t *)(operand)); real34SetNegativeSign((real34_t *)((char *)(operand) + REAL34_SIZE));}



#define real451Remainder(operand1, operand2, res)              decNumberRemainder       ((realIc_t *)res, (realIc_t *)operand1, (realIc_t *)operand2, &ctxtReal451)



#define realIcAdd(operand1, operand2, res)                     decNumberAdd             ((realIc_t *)res, (realIc_t *)operand1, (realIc_t *)operand2, &ctxtRealIc)
#define realIcChangeSign(operand)                              ((realIc_t *)operand)->bits      ^= 0x80
#define realIcCompare(operand1, operand2, res)                 decNumberCompare         ((realIc_t *)res, (realIc_t *)operand1, (realIc_t *)operand2, &ctxtRealIc)
#define realIcCompareTotal(operand1, operand2, res)            decNumberCompareTotal    ((realIc_t *)res, (realIc_t *)operand1, (realIc_t *)operand2, &ctxtRealIc)
#define realIcCopy(source, destination)                        decNumberCopy            ((realIc_t *)destination, (realIc_t *)source)
#define realIcCopyAbs(source, destination)                     decNumberCopyAbs         ((realIc_t *)destination, (realIc_t *)source)
#define realIcDivide(operand1, operand2, res)                  decNumberDivide          ((realIc_t *)res, (realIc_t *)operand1, (realIc_t *)operand2, &ctxtRealIc)
#define realIcExp(operand, res)                                decNumberExp             ((realIc_t *)res, (realIc_t *)operand, &ctxtRealIc)
#define realIcFMA(factor1, factor2, term, res)                 decNumberFMA             ((realIc_t *)res, (realIc_t *)factor1,  (realIc_t *)factor2,  (realIc_t *)term, &ctxtRealIc)
#define realIcIsInfinite(source)                               decNumberIsInfinite      ((realIc_t *)source)
#define realIcIsNaN(source)                                    decNumberIsNaN           ((realIc_t *)source)
//#define realIcIsNegative(source)                               decNumberIsNegative      ((realIc_t *)source)
#define realIcIsNegative(source)                               (((((realIc_t *)source)->bits) & 0x80) == 0x80)
#define realIcIsPositive(source)                               (((((realIc_t *)source)->bits) & 0x80) == 0x00)
#define realIcIsSpecial(source)                                decNumberIsSpecial       ((realIc_t *)source)
#define realIcIsZero(source)                                   decNumberIsZero          ((realIc_t *)source)
//#define realIcLn(operand, res)                                 decNumberLn              ((realIc_t *)res, (realIc_t *)operand, &ctxtRealIc)
//#define realIcLog10(operand, res)                              decNumberLog10           ((realIc_t *)res, (realIc_t *)operand, &ctxtRealIc)
#define realIcMultiply(operand1, operand2, res)                decNumberMultiply        ((realIc_t *)res, (realIc_t *)operand1, (realIc_t *)operand2, &ctxtRealIc)
#define realIcPlus(operand, res)                               decNumberPlus            ((realIc_t *)res, (realIc_t *)operand, &ctxtRealIc)
#define realIcPower(operand1, operand2, res)                   decNumberPower           ((realIc_t *)res, (realIc_t *)operand1, (realIc_t *)operand2, &ctxtRealIc)
#define realIcRemainder(operand1, operand2, res)               decNumberRemainder       ((realIc_t *)res, (realIc_t *)operand1, (realIc_t *)operand2, &ctxtRealIc)
#define realIcSetNegativeSign(operand)                         ((realIc_t *)operand)->bits |= 0x80
#define realIcSetPositiveSign(operand)                         ((realIc_t *)operand)->bits &= 0x7F
#define realIcSquareRoot(operand, res)                         decNumberSquareRoot      ((realIc_t *)res, (realIc_t *)operand, &ctxtRealIc)
#define realIcSubtract(operand1, operand2, res)                decNumberSubtract        ((realIc_t *)res, (realIc_t *)operand1, (realIc_t *)operand2, &ctxtRealIc)
#define realIcToInt32(source)                                  decNumberToInt32         ((realIc_t *)source, &ctxtRealIc)
//#define realIcToIntegralValue(source, destination)             decNumberToIntegralValue ((realIc_t *)destination, (realIc_t *)source, &ctxtRealIc)
#define realIcToIntegralValue(source, destination)             {real34_t real34; decQuadFromNumber(&real34, (realIc_t *)source, &ctxtReal34); decQuadToIntegralValue(&real34, &real34, &ctxtReal34, DEC_ROUND_DOWN); decQuadToNumber(&real34, (realIc_t *)destination);}
#define realIcToReal16(source, destination)                    decDoubleFromNumber      ((real16_t *)destination, (realIc_t *)source, &ctxtReal16)
#define realIcToReal34(source, destination)                    decQuadFromNumber        ((real34_t *)destination, (realIc_t *)source, &ctxtReal34)
#define realIcToString(source, destination)                    decNumberToString        ((realIc_t *)source, destination)
#define realIcToUInt32(source)                                 decNumberToUInt32        ((realIc_t *)source, &ctxtRealIc)
#define realIcZero(destination)                                decNumberZero            ((realIc_t *)destination)

#define int32ToReal16(source, destination)                     decDoubleFromInt32       ((real16_t *)destination, source)
#define int32ToRealIc(source, destination)                     decNumberFromInt32       ((realIc_t *)destination, source)
#define stringToRealIc(source, destination)                    decNumberFromString      ((realIc_t *)destination, source, &ctxtRealIc)
#define stringToReal16(source, destination)                    decDoubleFromString      ((real16_t *)destination, source, &ctxtReal16)
#define stringToReal34(source, destination)                    decQuadFromString        ((real34_t *)destination, source, &ctxtReal34)
#define stringToReal451(source, destination)                   decQuadFromString        ((real34_t *)destination, source, &ctxtReal451)
#define uInt32ToRealIc(source, destination)                    decNumberFromUInt32      ((realIc_t *)destination, source)
