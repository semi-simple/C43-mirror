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

#define real16_t                                          decDouble
#define real34_t                                          decQuad
#define real51_t                                          decNumber
typedef struct {
  int32_t digits;      /* Count of digits in the coefficient; >0    */
  int32_t exponent;    /* Unadjusted exponent, unbiased, in         */
                       /* range: -1999999997 through 999999999      */
  uint8_t bits;        /* Indicator bits (see above)                */
                       /* Coefficient, from least significant unit  */
  decNumberUnit lsu[(451+DECDPUN-1)/DECDPUN];
} real451_t;

typedef struct {real16_t x[2];}                           complex16_t;
typedef struct {real34_t x[2];}                           complex34_t;

#define REAL16_SIZE                                       sizeof(real16_t)
#define REAL34_SIZE                                       sizeof(real34_t)
#define COMPLEX16_SIZE                                    (2*sizeof(real16_t))
#define COMPLEX34_SIZE                                    (2*sizeof(real34_t))
#define REAL51_SIZE                                       sizeof(real51_t)
#define REAL451_SIZE                                      sizeof(real451_t)

#if (MEMORY_ALLOCATION_ALIGNMENT == 4)
  #define POINTER_TO_LOCAL_FLAGS                            ((uint32_t *)(allLocalRegisterPointer))
  #define POINTER_TO_LOCAL_REGISTER(a)                      ((uint32_t *)(allLocalRegisterPointer + 4u + 4u*(a)))
#else
  #define POINTER_TO_LOCAL_FLAGS                            ((uint16_t *)(allLocalRegisterPointer))
  #define POINTER_TO_LOCAL_REGISTER(a)                      ((uint32_t *)(allLocalRegisterPointer + 2u + 4u*(a)))
#endif

#define POINTER_TO_NAMED_VARIABLE(a)                      ((uint32_t *)(allNamedVariablePointer      + 6u*(a)))
#define POINTER_TO_POINTER_TO_NAMED_VARIABLE_NAME(a)      ((uint16_t *)(allNamedVariablePointer + 4u + 6u*(a)))
#define POINTER_TO_NAMED_VARIABLE_NAME(a)                 ((char     *)(*POINTER_TO_POINTER_TO_NAMED_VARIABLE_NAME(a) << 1))
#define RAM_REAL16(a)                                     ((real16_t *)(ram + (a)))
#define RAM_REAL34(a)                                     ((real34_t *)(ram + (a)))


#define REGISTER_DATA(a)                                  ((void        *)(getRegisterDataPointer(a)))
#define REGISTER_REAL16_DATA(a)                           ((real16_t    *)(getRegisterDataPointer(a)))
#define REGISTER_IMAG16_DATA(a)                           ((real16_t    *)(getRegisterDataPointer(a) + REAL16_SIZE))
#define REGISTER_COMPLEX16_DATA(a)                        ((complex16_t *)(getRegisterDataPointer(a)))
#define REGISTER_REAL34_DATA(a)                           ((real34_t    *)(getRegisterDataPointer(a)))
#define REGISTER_IMAG34_DATA(a)                           ((real34_t    *)(getRegisterDataPointer(a) + REAL34_SIZE))
#define REGISTER_COMPLEX34_DATA(a)                        ((complex34_t *)(getRegisterDataPointer(a)))

#if (MEMORY_ALLOCATION_ALIGNMENT == 4)
  #define REGISTER_STRING_DATA(a)                           ((char        *)(getRegisterDataPointer(a) + 4)) // Memory pointer to the string of a register
  #define REGISTER_LONG_INTEGER_DATA(a)                     ((uint8_t     *)(getRegisterDataPointer(a) + 4)) // Memory pointer to the long integer of a register
  #define REGISTER_DATA_MAX_LEN(a)                          ((uint32_t    *)(getRegisterDataPointer(a)))     // Memory pointer to the lenght of string or long integer
#else
  #define REGISTER_STRING_DATA(a)                           ((char        *)(getRegisterDataPointer(a) + 2)) // Memory pointer to the string of a register
  #define REGISTER_LONG_INTEGER_DATA(a)                     ((uint8_t     *)(getRegisterDataPointer(a) + 2)) // Memory pointer to the long integer of a register
  #define REGISTER_DATA_MAX_LEN(a)                          ((uint16_t    *)(getRegisterDataPointer(a)))     // Memory pointer to the lenght of string or long integer
#endif

#define REGISTER_SHORT_INTEGER_DATA(a)                    ((uint64_t    *)(getRegisterDataPointer(a)))
#define VARIABLE_REAL16_DATA(a)                           ((real16_t    *)(a))
#define VARIABLE_IMAG16_DATA(a)                           ((real16_t    *)((char *)(a) + REAL16_SIZE))
#define VARIABLE_COMPLEX16_DATA(a)                        ((complex16_t *)(a))
#define VARIABLE_REAL34_DATA(a)                           ((real34_t    *)(a))
#define VARIABLE_IMAG34_DATA(a)                           ((real34_t    *)((char *)(a) + REAL34_SIZE))
#define VARIABLE_COMPLEX34_DATA(a)                        ((complex34_t *)(a))


#define real16ToString(source, destination)               decDoubleToString        ((real16_t  *)source,      destination)
#define real34ToString(source, destination)               decQuadToString          ((real34_t  *)source,      destination)
#define real51ToString(source, destination)               decNumberToString        ((real51_t  *)source,      destination)
#define real451ToString(source, destination)              decNumberToString        ((decNumber *)source,      destination)
#define stringToReal16(source, destination)               decDoubleFromString      ((real16_t  *)destination, source, &ctxtReal16)
#define stringToReal34(source, destination)               decQuadFromString        ((real34_t  *)destination, source, &ctxtReal34)
#define stringToReal51(source, destination)               decNumberFromString      ((real51_t  *)destination, source, &ctxtReal51)
#define stringToReal451(source, destination)              decNumberFromString      ((decNumber *)destination, source, &ctxtReal451)
#define stringToReal51Ctxt(source, destination, ctxt)     decNumberFromString      ((real51_t  *)destination, source, ctxt)
#define int32ToReal16(source, destination)                decDoubleFromInt32       ((real16_t  *)destination, source)
#define int32ToReal34(source, destination)                decQuadFromInt32         ((real34_t  *)destination, source)
#define int32ToReal51(source, destination)                decNumberFromInt32       ((real51_t  *)destination, source)
#define uInt32ToReal16(source, destination)               decDoubleFromUInt32      ((real16_t  *)destination, source)
#define uInt32ToReal34(source, destination)               decQuadFromUInt32        ((real34_t  *)destination, source)
#define uInt32ToReal51(source, destination)               decNumberFromUInt32      ((real51_t  *)destination, source)


#define real16ToReal34(source, destination)               decDoubleToWider         ((real16_t *)source,      (real34_t  *)destination)
#define real34ToReal16(source, destination)               decDoubleFromWider       ((real16_t *)destination, (real34_t  *)source, &ctxtReal16)
#define real16ToReal51(source, destination)               decDoubleToNumber        ((real16_t *)source,      (real51_t  *)destination)
#define real34ToReal51(source, destination)               decQuadToNumber          ((real34_t *)source,      (real51_t  *)destination)
#define real34ToReal451(source, destination)              decQuadToNumber          ((real34_t *)source,      (decNumber *)destination)
#define real51ToReal16(source, destination)               decDoubleFromNumber      ((real16_t *)destination, (real51_t  *)source, &ctxtReal16)
#define real51ToReal34(source, destination)               decQuadFromNumber        ((real34_t *)destination, (real51_t  *)source, &ctxtReal34)
#define real451ToReal34(source, destination)              decQuadFromNumber        ((real34_t *)destination, (decNumber *)source, &ctxtReal34)
#define real16AbsToReal16(source, destination)            decDoubleCopyAbs         ((real16_t *)destination, (real16_t *)source)
#define real34AbsToReal34(source, destination)            decQuadCopyAbs           ((real34_t *)destination, (real34_t *)source)
#define real16ToUInt32(source)                            decDoubleToUInt32        ((real16_t *)source,      &ctxtReal16, DEC_ROUND_DOWN)
#define real34ToUInt32(source)                            decQuadToUInt32          ((real34_t *)source,      &ctxtReal34, DEC_ROUND_DOWN)
#define real51ToUInt32(source)                            decNumberToUInt32        ((real51_t *)source,      &ctxtReal51)
#define real16ToInt32(source)                             decDoubleToInt32         ((real16_t *)source,      &ctxtReal16, DEC_ROUND_DOWN)
#define real34ToInt32(source)                             decQuadToInt32           ((real34_t *)source,      &ctxtReal34, DEC_ROUND_DOWN)
#define real51ToInt32(source)                             decNumberToInt32         ((real51_t *)source,      &ctxtReal51)

#define real16ToIntegral(source, destination)             decDoubleToIntegralValue (destination, (real16_t *)source, &ctxtReal16, DEC_ROUND_DOWN)
#define real34ToIntegral(source, destination)             decQuadToIntegralValue   (destination, (real34_t *)source, &ctxtReal34, DEC_ROUND_DOWN)
#define real16ToIntegralRound(source, destination)        decDoubleToIntegralValue (destination, (real16_t *)source, &ctxtReal16, DEC_ROUND_HALF_UP)
#define real34ToIntegralRound(source, destination)        decQuadToIntegralValue   (destination, (real34_t *)source, &ctxtReal34, DEC_ROUND_HALF_UP)


#define real16IsInfinite(source)                          decDoubleIsInfinite      ((real16_t  *)source)
#define real34IsInfinite(source)                          decQuadIsInfinite        ((real34_t  *)source)
#define real16IsNaN(source)                               decDoubleIsNaN           ((real16_t  *)source)
#define real34IsNaN(source)                               decQuadIsNaN             ((real34_t  *)source)
#define real51IsNaN(source)                               decNumberIsNaN           ((real51_t  *)source)
#define real451IsNaN(source)                              decNumberIsNaN           ((decNumber *)source)
#define real16IsZero(source)                              decDoubleIsZero          ((real16_t  *)source)
#define real34IsZero(source)                              decQuadIsZero            ((real34_t  *)source)
#define real51IsZero(source)                              decNumberIsZero          ((real51_t  *)source)
#define real16IsNegative(source)                          decDoubleIsNegative      ((real16_t  *)source)
#define real34IsNegative(source)                          decQuadIsNegative        ((real34_t  *)source)
#define real51IsNegative(source)                          decNumberIsNegative      ((real51_t  *)source)
#define real16IsPositive(source)                          decDoubleIsPositive      ((real16_t  *)source)
#define real34IsPositive(source)                          decQuadIsPositive        ((real34_t  *)source)
#define real16IsSpecial(source)                           (decDoubleIsNaN((real16_t *)source) || decDoubleIsSignaling((real16_t *)source) || decDoubleIsInfinite((real16_t *)source))
#define real34IsSpecial(source)                           (decQuadIsNaN((real34_t *)source)   || decQuadIsSignaling((real34_t *)source)   || decQuadIsInfinite((real34_t *)source))
#define real51IsSpecial(source)                           (decNumberIsSpecial((real51_t *)source))
#define real16GetCoefficient(source, destination)         decDoubleGetCoefficient  ((real16_t *)source, (uint8_t *)(destination))
#define real34GetCoefficient(source, destination)         decQuadGetCoefficient    ((real34_t *)source, (uint8_t *)(destination))
#define real16GetExponent(source)                         decDoubleGetExponent     ((real16_t *)source)
#define real34GetExponent(source)                         decQuadGetExponent       ((real34_t *)source)


#define real16Zero(destination)                           decDoubleZero            ((real16_t  *)destination)
#define real34Zero(destination)                           decQuadZero              ((real34_t  *)destination)
#define real51Zero(destination)                           decNumberZero            ((real51_t  *)destination)
#define real16Add(operand1, operand2, res)                decDoubleAdd             ((real16_t  *)res, (real16_t  *)operand1, (real16_t  *)operand2, &ctxtReal16)
#define real34Add(operand1, operand2, res)                decQuadAdd               ((real34_t  *)res, (real34_t  *)operand1, (real34_t  *)operand2, &ctxtReal34)
#define real51Add(operand1, operand2, res)                decNumberAdd             ((real51_t  *)res, (real51_t  *)operand1, (real51_t  *)operand2, &ctxtReal51)
#define real16Subtract(operand1, operand2, res)           decDoubleSubtract        ((real16_t  *)res, (real16_t  *)operand1, (real16_t  *)operand2, &ctxtReal16)
#define real34Subtract(operand1, operand2, res)           decQuadSubtract          ((real34_t  *)res, (real34_t  *)operand1, (real34_t  *)operand2, &ctxtReal34)
#define real51Subtract(operand1, operand2, res)           decNumberSubtract        ((real51_t  *)res, (real51_t  *)operand1, (real51_t  *)operand2, &ctxtReal51)
#define real16Multiply(operand1, operand2, res)           decDoubleMultiply        ((real16_t  *)res, (real16_t  *)operand1, (real16_t  *)operand2, &ctxtReal16)
#define real34Multiply(operand1, operand2, res)           decQuadMultiply          ((real34_t  *)res, (real34_t  *)operand1, (real34_t  *)operand2, &ctxtReal34)
#define real51Multiply(operand1, operand2, res)           decNumberMultiply        ((real51_t  *)res, (real51_t  *)operand1, (real51_t  *)operand2, &ctxtReal51)
#define real16Divide(operand1, operand2, res)             decDoubleDivide          ((real16_t  *)res, (real16_t  *)operand1, (real16_t  *)operand2, &ctxtReal16)
#define real34Divide(operand1, operand2, res)             decQuadDivide            ((real34_t  *)res, (real34_t  *)operand1, (real34_t  *)operand2, &ctxtReal34)
#define real51Divide(operand1, operand2, res)             decNumberDivide          ((real51_t  *)res, (real51_t  *)operand1, (real51_t  *)operand2, &ctxtReal51)
#define real16CompareTotal(operand1, operand2, res)       decDoubleCompareTotal    ((real16_t  *)res, (real16_t  *)operand1, (real16_t  *)operand2)
#define real34CompareTotal(operand1, operand2, res)       decQuadCompareTotal      ((real34_t  *)res, (real34_t  *)operand1, (real34_t  *)operand2)
#define real51CompareTotal(operand1, operand2, res)       decNumberCompareTotal    ((real51_t  *)res, (real51_t  *)operand1, (real51_t  *)operand2, &ctxtReal51)
#define real16Compare(operand1, operand2, res)            decDoubleCompare         ((real16_t  *)res, (real16_t  *)operand1, (real16_t  *)operand2, &ctxtReal16)
#define real34Compare(operand1, operand2, res)            decQuadCompare           ((real34_t  *)res, (real34_t  *)operand1, (real34_t  *)operand2, &ctxtReal34)
#define real51Compare(operand1, operand2, res)            decNumberCompare         ((real51_t  *)res, (real51_t  *)operand1, (real51_t  *)operand2, &ctxtReal51)
#define real16Remainder(operand1, operand2, res)          decDoubleRemainder       ((real16_t  *)res, (real16_t  *)operand1, (real16_t  *)operand2, &ctxtReal16)
#define real34Remainder(operand1, operand2, res)          decQuadRemainder         ((real34_t  *)res, (real34_t  *)operand1, (real34_t  *)operand2, &ctxtReal34)
#define real451Remainder(operand1, operand2, res)         decNumberRemainder       ((decNumber *)res, (decNumber *)operand1, (decNumber *)operand2, &ctxtReal451)
#define real16FMA(factor1, factor2, term, res)            decDoubleFMA             ((real16_t  *)res, (real16_t  *)factor1,  (real16_t  *)factor2,  (real16_t  *)term, &ctxtReal16)
#define real34FMA(factor1, factor2, term, res)            decQuadFMA               ((real34_t  *)res, (real34_t  *)factor1,  (real34_t  *)factor2,  (real34_t  *)term, &ctxtReal34)




#define real16ChangeSign(operand)                         ((real16_t *)operand)->bytes[ 7] ^= 0x80
#define real34ChangeSign(operand)                         ((real34_t *)operand)->bytes[15] ^= 0x80
#define real51ChangeSign(operand)                         ((real51_t *)operand)->bits      ^= 0x80
#define real16SetPositiveSign(operand)                    ((real16_t *)operand)->bytes[ 7] &= 0x7F
#define real34SetPositiveSign(operand)                    ((real34_t *)operand)->bytes[15] &= 0x7F
#define real51SetPositiveSign(operand)                    ((real51_t *)operand)->bits      &= 0x7F
#define real16SetNegativeSign(operand)                    ((real16_t *)operand)->bytes[ 7] |= 0x80
#define real34SetNegativeSign(operand)                    ((real34_t *)operand)->bytes[15] |= 0x80
#define real51SetNegativeSign(operand)                    ((real51_t *)operand)->bits      |= 0x80


#define complex16ChangeSign(operand)                      {real16ChangeSign((real16_t *)(operand));      real16ChangeSign((real16_t *)((char *)(operand) + REAL16_SIZE));}
#define complex34ChangeSign(operand)                      {real34ChangeSign((real34_t *)(operand));      real34ChangeSign((real34_t *)((char *)(operand) + REAL34_SIZE));}
#define complex16SetPositiveSign(operand)                 {real16SetPositiveSign((real16_t *)(operand)); real16SetPositiveSign((real16_t *)((char *)(operand) + REAL16_SIZE));}
#define complex34SetPositiveSign(operand)                 {real34SetPositiveSign((real34_t *)(operand)); real34SetPositiveSign((real34_t *)((char *)(operand) + REAL34_SIZE));}
#define complex16SetNegativeSign(operand)                 {real16SetNegativeSign((real16_t *)(operand)); real16SetNegativeSign((real16_t *)((char *)(operand) + REAL16_SIZE));}
#define complex34SetNegativeSign(operand)                 {real34SetNegativeSign((real34_t *)(operand)); real34SetNegativeSign((real34_t *)((char *)(operand) + REAL34_SIZE));}


#define real16Copy(source, destination)                   memcpy((real16_t *)destination, (real16_t *)source, REAL16_SIZE)
#define real34Copy(source, destination)                   memcpy((real34_t *)destination, (real34_t *)source, REAL34_SIZE)
#define real51Copy(source, destination)                   decNumberCopy((decNumber *)(destination), (decNumber *)(source))
#define real451Copy(source, destination)                  decNumberCopy((decNumber *)(destination), (decNumber *)(source))
#define complex16Copy(source, destination)                memcpy((complex16_t *)destination, (complex16_t *)source, COMPLEX16_SIZE)
#define complex34Copy(source, destination)                memcpy((complex34_t *)destination, (complex34_t *)source, COMPLEX34_SIZE)


#define real51Log10(operand, res)                         decNumberLog10           ((real51_t *)res, (real51_t *)operand, &ctxtReal51)
#define real51Power(operand1, operand2, res)              decNumberPower           ((real51_t *)res, (real51_t *)operand1, (real51_t *)operand2, &ctxtReal51)
#define real51SquareRoot(operand, res)                    decNumberSquareRoot      ((real51_t *)res, (real51_t *)operand, &ctxtReal51)
#define real51Exp(operand, res)                           decNumberExp             ((real51_t *)res, (real51_t *)operand, &ctxtReal51)
#define real51Ln(operand, res)                            decNumberLn              ((real51_t *)res, (real51_t *)operand, &ctxtReal51)


#define real16Log10(operand, res)                         {real51_t real51;           real16ToReal51((real16_t *)operand,  &real51);                                      real51Log10(&real51, &real51);             real51ToReal16(&real51,  (real16_t *)res);}
#define real16Power(operand1, operand2, res)              {real51_t real51a, real51b; real16ToReal51((real16_t *)operand1, &real51a); real16ToReal51(operand2, &real51b); real51Power(&real51a, &real51b, &real51b); real51ToReal16(&real51b, (real16_t *)res);}
#define real16SquareRoot(operand, res)                    {real51_t real51;           real16ToReal51((real16_t *)operand,  &real51);                                      real51SquareRoot(&real51, &real51);        real51ToReal16(&real51,  (real16_t *)res);}
#define real16Exp(operand, res)                           {real51_t real51;           real16ToReal51((real16_t *)operand,  &real51);                                      real51Exp(&real51, &real51);               real51ToReal16(&real51,  (real16_t *)res);}
#define real16Ln(operand, res)                            {real51_t real51;           real16ToReal51((real16_t *)operand,  &real51);                                      real51Ln(&real51, &real51);                real51ToReal16(&real51,  (real16_t *)res);}


#define real34Log10(operand, res)                         {real51_t real51;           real34ToReal51((real34_t *)operand,  &real51);                                      real51Log10(&real51, &real51);             real51ToReal34(&real51,  (real34_t *)res);}
#define real34Power(operand1, operand2, res)              {real51_t real51a, real51b; real34ToReal51((real34_t *)operand1, &real51a); real34ToReal51(operand2, &real51b); real51Power(&real51a, &real51b, &real51b); real51ToReal34(&real51b, (real34_t *)res);}
#define real34SquareRoot(operand, res)                    {real51_t real51;           real34ToReal51((real34_t *)operand,  &real51);                                      real51SquareRoot(&real51, &real51);        real51ToReal34(&real51,  (real34_t *)res);}
#define real34Exp(operand, res)                           {real51_t real51;           real34ToReal51((real34_t *)operand,  &real51);                                      real51Exp(&real51, &real51);               real51ToReal34(&real51,  (real34_t *)res);}
#define real34Ln(operand, res)                            {real51_t real51;           real34ToReal51((real34_t *)operand,  &real51);                                      real51Ln(&real51, &real51);                real51ToReal34(&real51,  (real34_t *)res);}
