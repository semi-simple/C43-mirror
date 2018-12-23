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
typedef struct {real16_t x[2];}                           complex16_t;
typedef struct {real34_t x[2];}                           complex34_t;

#define REAL16_SIZE                                       sizeof(real16_t)
#define REAL34_SIZE                                       sizeof(real34_t)
#define REAL51_SIZE                                       sizeof(real51_t)
#define COMPLEX16_SIZE                                    (2*sizeof(real16_t))
#define COMPLEX34_SIZE                                    (2*sizeof(real34_t))

#define REAL16_POINTER(a)                                 ((real16_t *)(a))
#define REAL34_POINTER(a)                                 ((real34_t *)(a))
#define REAL51_POINTER(a)                                 ((real51_t *)(a))
#define COMPLEX16_POINTER(a)                              ((complex16_t *)(a))
#define COMPLEX16_IMAGINARY_PART_POINTER(a)               ((real16_t *)((char *)(a) + REAL16_SIZE))
#define COMPLEX34_POINTER(a)                              ((complex34_t *)(a))
#define COMPLEX34_IMAGINARY_PART_POINTER(a)               ((real34_t *)((char *)(a) + REAL34_SIZE))
#define POINTER_TO_REGISTER_DATA(a)                       ((uint8_t  *)(ram + getRegisterDataPointer(a)))     // Memory pointer to the data of a global register
#define POINTER_TO_REGISTER_STRING(a)                     ((char *    )(ram + getRegisterDataPointer(a) + 2)) // Memory pointer to the string of a global register
#define POINTER_TO_REGISTER_BIG_INTEGER(a)                ((uint8_t  *)(ram + getRegisterDataPointer(a) + 2)) // Memory pointer to the big integer of a global register
#define POINTER_TO_REGISTER_NAME(a)                       ((char     *)(ram + getRegisterNamePointer(a)))
#define POINTER_TO_LOCAL_FLAGS                            ((uint16_t *)(ram + allLocalRegisterPointer))
#define POINTER_TO_LOCAL_REGISTER(a)                      ((uint32_t *)(ram + allLocalRegisterPointer + 2u + 4u*(a)))
#define POINTER_TO_NAMED_REGISTER(a)                      ((uint32_t *)(ram + allNamedRegisterPointer      + 6u*(a)))
#define POINTER_TO_POINTER_TO_NAMED_REGISTER_NAME(a)      ((uint16_t *)(ram + allNamedRegisterPointer + 4u + 6u*(a)))
#define POINTER_TO_NAMED_REGISTER_NAME(a)                 ((char     *)(ram + (*POINTER_TO_POINTER_TO_NAMED_REGISTER_NAME(a) << 1)))
#define RAM(a)                                            ((uint8_t  *)(ram + (a)))


#define real16ToString(source, destination)               decDoubleToString        (REAL16_POINTER(source),      destination)
#define real34ToString(source, destination)               decQuadToString          (REAL34_POINTER(source),      destination)
#define real51ToString(source, destination)               decNumberToString        (REAL51_POINTER(source),      destination)
#define stringToReal16(source, destination)               decDoubleFromString      (REAL16_POINTER(destination), source, &ctxtReal16)
#define stringToReal34(source, destination)               decQuadFromString        (REAL34_POINTER(destination), source, &ctxtReal34)
#define stringToReal51(source, destination)               decNumberFromString      (REAL51_POINTER(destination), source, &ctxtReal51)
#define stringToReal51Ctxt(source, destination, ctxt)     decNumberFromString      (REAL51_POINTER(destination), source, ctxt)
#define int32ToReal16(source, destination)                decDoubleFromInt32       (REAL16_POINTER(destination), source)
#define int32ToReal34(source, destination)                decQuadFromInt32         (REAL34_POINTER(destination), source)
#define int32ToReal51(source, destination)                decNumberFromInt32       (REAL51_POINTER(destination), source)
#define uInt32ToReal16(source, destination)               decDoubleFromUInt32      (REAL16_POINTER(destination), source)
#define uInt32ToReal34(source, destination)               decQuadFromUInt32        (REAL34_POINTER(destination), source)
#define uInt32ToReal51(source, destination)               decNumberFromUInt32      (REAL51_POINTER(destination), source)


#define real16ToReal34(source, destination)               decDoubleToWider         (REAL16_POINTER(source),      REAL34_POINTER(destination))
#define real34ToReal16(source, destination)               decDoubleFromWider       (REAL16_POINTER(destination), REAL34_POINTER(source), &ctxtReal16)
#define real16ToReal51(source, destination)               decDoubleToNumber        (REAL16_POINTER(source),      REAL51_POINTER(destination))
#define real34ToReal51(source, destination)               decQuadToNumber          (REAL34_POINTER(source),      REAL51_POINTER(destination))
#define real51ToReal16(source, destination)               decDoubleFromNumber      (REAL16_POINTER(destination), REAL51_POINTER(source), &ctxtReal16)
#define real51ToReal34(source, destination)               decQuadFromNumber        (REAL34_POINTER(destination), REAL51_POINTER(source), &ctxtReal34)
#define real16AbsToReal16(source, destination)            decDoubleCopyAbs         (REAL16_POINTER(destination), REAL16_POINTER(source))
#define real34AbsToReal34(source, destination)            decQuadCopyAbs           (REAL34_POINTER(destination), REAL34_POINTER(source))
#define real16ToUInt32(source)                            decDoubleToUInt32        (REAL16_POINTER(source),      &ctxtReal16, DEC_ROUND_DOWN)
#define real34ToUInt32(source)                            decQuadToUInt32          (REAL34_POINTER(source),      &ctxtReal34, DEC_ROUND_DOWN)
#define real51ToUInt32(source)                            decNumberToUInt32        (REAL51_POINTER(source),      &ctxtReal51)
#define real16ToInt32(source)                             decDoubleToInt32         (REAL16_POINTER(source),      &ctxtReal16, DEC_ROUND_DOWN)
#define real34ToInt32(source)                             decQuadToInt32           (REAL34_POINTER(source),      &ctxtReal34, DEC_ROUND_DOWN)
#define real51ToInt32(source)                             decNumberToInt32         (REAL51_POINTER(source),      &ctxtReal51)

#define real16ToIntegral(source, destination)             decDoubleToIntegralValue (REAL16_POINTER(destination), REAL16_POINTER(source), &ctxtReal16, DEC_ROUND_DOWN)
#define real34ToIntegral(source, destination)             decQuadToIntegralValue   (REAL34_POINTER(destination), REAL34_POINTER(source), &ctxtReal34, DEC_ROUND_DOWN)
#define real34ToIntegralRound(source, destination)        decDoubleToIntegralValue (REAL16_POINTER(destination), REAL16_POINTER(source), &ctxtReal16, DEC_ROUND_HALF_UP)


#define real16IsInfinite(source)                          decDoubleIsInfinite      (REAL16_POINTER(source))
#define real34IsInfinite(source)                          decQuadIsInfinite        (REAL34_POINTER(source))
#define real16IsNaN(source)                               decDoubleIsNaN           (REAL16_POINTER(source))
#define real34IsNaN(source)                               decQuadIsNaN             (REAL34_POINTER(source))
#define real51IsNaN(source)                               decNumberIsNaN           (REAL51_POINTER(source))
#define real16IsZero(source)                              decDoubleIsZero          (REAL16_POINTER(source))
#define real34IsZero(source)                              decQuadIsZero            (REAL34_POINTER(source))
#define real51IsZero(source)                              decNumberIsZero          (REAL51_POINTER(source))
#define real16IsNegative(source)                          decDoubleIsNegative      (REAL16_POINTER(source))
#define real34IsNegative(source)                          decQuadIsNegative        (REAL34_POINTER(source))
#define real51IsNegative(source)                          decNumberIsNegative      (REAL51_POINTER(source))
#define real16IsPositive(source)                          decDoubleIsPositive      (REAL16_POINTER(source))
#define real34IsPositive(source)                          decQuadIsPositive        (REAL34_POINTER(source))
#define real16IsSpecial(source)                           (decDoubleIsNaN(REAL16_POINTER(source)) || decDoubleIsSignaling(REAL16_POINTER(source)) || decDoubleIsInfinite(REAL16_POINTER(source)))
#define real34IsSpecial(source)                           (decQuadIsNaN(REAL34_POINTER(source))   || decQuadIsSignaling(REAL34_POINTER(source))   || decQuadIsInfinite(REAL34_POINTER(source)))
#define real51IsSpecial(source)                           (decNumberIsSpecial(REAL51_POINTER(source)))
#define real16GetCoefficient(source, destination)         decDoubleGetCoefficient  (REAL16_POINTER(source), (uint8_t *)(destination))
#define real34GetCoefficient(source, destination)         decQuadGetCoefficient    (REAL34_POINTER(source), (uint8_t *)(destination))
#define real16GetExponent(source)                         decDoubleGetExponent     (REAL16_POINTER(source))
#define real34GetExponent(source)                         decQuadGetExponent       (REAL34_POINTER(source))


#define real16Zero(destination)                           decDoubleZero            (REAL16_POINTER(destination))
#define real34Zero(destination)                           decQuadZero              (REAL34_POINTER(destination))
#define real51Zero(destination)                           decNumberZero            (REAL51_POINTER(destination))
#define real16Add(operand1, operand2, res)                decDoubleAdd             (REAL16_POINTER(res), REAL16_POINTER(operand1), REAL16_POINTER(operand2), &ctxtReal16)
#define real34Add(operand1, operand2, res)                decQuadAdd               (REAL34_POINTER(res), REAL34_POINTER(operand1), REAL34_POINTER(operand2), &ctxtReal34)
#define real51Add(operand1, operand2, res)                decNumberAdd             (REAL51_POINTER(res), REAL51_POINTER(operand1), REAL51_POINTER(operand2), &ctxtReal51)
#define real16Subtract(operand1, operand2, res)           decDoubleSubtract        (REAL16_POINTER(res), REAL16_POINTER(operand1), REAL16_POINTER(operand2), &ctxtReal16)
#define real34Subtract(operand1, operand2, res)           decQuadSubtract          (REAL34_POINTER(res), REAL34_POINTER(operand1), REAL34_POINTER(operand2), &ctxtReal34)
#define real51Subtract(operand1, operand2, res)           decNumberSubtract        (REAL51_POINTER(res), REAL51_POINTER(operand1), REAL51_POINTER(operand2), &ctxtReal51)
#define real16Multiply(operand1, operand2, res)           decDoubleMultiply        (REAL16_POINTER(res), REAL16_POINTER(operand1), REAL16_POINTER(operand2), &ctxtReal16)
#define real34Multiply(operand1, operand2, res)           decQuadMultiply          (REAL34_POINTER(res), REAL34_POINTER(operand1), REAL34_POINTER(operand2), &ctxtReal34)
#define real51Multiply(operand1, operand2, res)           decNumberMultiply        (REAL51_POINTER(res), REAL51_POINTER(operand1), REAL51_POINTER(operand2), &ctxtReal51)
#define real16Divide(operand1, operand2, res)             decDoubleDivide          (REAL16_POINTER(res), REAL16_POINTER(operand1), REAL16_POINTER(operand2), &ctxtReal16)
#define real34Divide(operand1, operand2, res)             decQuadDivide            (REAL34_POINTER(res), REAL34_POINTER(operand1), REAL34_POINTER(operand2), &ctxtReal34)
#define real51Divide(operand1, operand2, res)             decNumberDivide          (REAL51_POINTER(res), REAL51_POINTER(operand1), REAL51_POINTER(operand2), &ctxtReal51)
#define real16CompareTotal(operand1, operand2, res)       decDoubleCompareTotal    (REAL16_POINTER(res), REAL16_POINTER(operand1), REAL16_POINTER(operand2))
#define real34CompareTotal(operand1, operand2, res)       decQuadCompareTotal      (REAL34_POINTER(res), REAL34_POINTER(operand1), REAL34_POINTER(operand2))
#define real51CompareTotal(operand1, operand2, res)       decNumberCompareTotal    (REAL51_POINTER(res), REAL51_POINTER(operand1), REAL51_POINTER(operand2), &ctxtReal51)
#define real16Compare(operand1, operand2, res)            decDoubleCompare         (REAL16_POINTER(res), REAL16_POINTER(operand1), REAL16_POINTER(operand2), &ctxtReal16)
#define real34Compare(operand1, operand2, res)            decQuadCompare           (REAL34_POINTER(res), REAL34_POINTER(operand1), REAL34_POINTER(operand2), &ctxtReal34)
#define real51Compare(operand1, operand2, res)            decNumberCompare         (REAL51_POINTER(res), REAL51_POINTER(operand1), REAL51_POINTER(operand2), &ctxtReal51)
#define real16Remainder(operand1, operand2, res)          decDoubleRemainder       (REAL16_POINTER(res), REAL16_POINTER(operand1), REAL16_POINTER(operand2), &ctxtReal16)
#define real34Remainder(operand1, operand2, res)          decQuadRemainder         (REAL34_POINTER(res), REAL34_POINTER(operand1), REAL34_POINTER(operand2), &ctxtReal34)
#define real16FMA(factor1, factor2, term, res)            decDoubleFMA             (REAL16_POINTER(res), REAL16_POINTER(factor1),  REAL16_POINTER(factor2),  REAL16_POINTER(term), &ctxtReal16)
#define real34FMA(factor1, factor2, term, res)            decQuadFMA               (REAL34_POINTER(res), REAL34_POINTER(factor1),  REAL34_POINTER(factor2),  REAL34_POINTER(term), &ctxtReal34)
#define real16Remainder(operand1, operand2, res)          decDoubleRemainder       (REAL16_POINTER(res), REAL16_POINTER(operand1), REAL16_POINTER(operand2), &ctxtReal16)
#define real34Remainder(operand1, operand2, res)          decQuadRemainder         (REAL34_POINTER(res), REAL34_POINTER(operand1), REAL34_POINTER(operand2), &ctxtReal34)




#define real16ChangeSign(operand)                         (REAL16_POINTER(operand))->bytes[ 7] ^= 0x80
#define real34ChangeSign(operand)                         (REAL34_POINTER(operand))->bytes[15] ^= 0x80
#define real51ChangeSign(operand)                         (REAL51_POINTER(operand))->bits      ^= 0x80
#define real16SetPositiveSign(operand)                    (REAL16_POINTER(operand))->bytes[ 7] &= 0x7F
#define real34SetPositiveSign(operand)                    (REAL34_POINTER(operand))->bytes[15] &= 0x7F
#define real51SetPositiveSign(operand)                    (REAL51_POINTER(operand))->bits      &= 0x7F
#define real16SetNegativeSign(operand)                    (REAL16_POINTER(operand))->bytes[ 7] |= 0x80
#define real34SetNegativeSign(operand)                    (REAL34_POINTER(operand))->bytes[15] |= 0x80
#define real51SetNegativeSign(operand)                    (REAL51_POINTER(operand))->bits      |= 0x80


#define complex16ChangeSign(operand)                      real16ChangeSign(operand);      real16ChangeSign((operand) + REAL16_SIZE)
#define complex34ChangeSign(operand)                      real34ChangeSign(operand);      real34ChangeSign((operand) + REAL34_SIZE)
#define complex16SetPositiveSign(operand)                 real16SetPositiveSign(operand); real16SetPositiveSign((operand) + REAL16_SIZE)
#define complex34SetPositiveSign(operand)                 real34SetPositiveSign(operand); real34SetPositiveSign((operand) + REAL34_SIZE)
#define complex16SetNegativeSign(operand)                 real16SetNegativeSign(operand); real16SetNegativeSign((operand) + REAL16_SIZE)
#define complex34SetNegativeSign(operand)                 real34SetNegativeSign(operand); real34SetNegativeSign((operand) + REAL34_SIZE)


#define real16Copy(source, destination)                   memcpy(destination, source, REAL16_SIZE)
#define real34Copy(source, destination)                   memcpy(destination, source, REAL34_SIZE)
#define real51Copy(source, destination)                   decNumberCopy(REAL51_POINTER(destination), REAL51_POINTER(source))
#define complex16Copy(source, destination)                memcpy(destination, source, COMPLEX16_SIZE)
#define complex34Copy(source, destination)                memcpy(destination, source, COMPLEX34_SIZE)


#define real51Log10(operand, res)                         decNumberLog10           (REAL51_POINTER(res), REAL51_POINTER(operand), &ctxtReal51)
#define real51Power(operand1, operand2, res)              decNumberPower           (REAL51_POINTER(res), REAL51_POINTER(operand1), REAL51_POINTER(operand2), &ctxtReal51)
#define real51SquareRoot(operand, res)                    decNumberSquareRoot      (REAL51_POINTER(res), REAL51_POINTER(operand), &ctxtReal51)
#define real51Exp(operand, res)                           decNumberExp             (REAL51_POINTER(res), REAL51_POINTER(operand), &ctxtReal51)
#define real51Ln(operand, res)                            decNumberLn              (REAL51_POINTER(res), REAL51_POINTER(operand), &ctxtReal51)


#define real16Log10(operand, res)                         {real51_t real51;           real16ToReal51(operand,  &real51);                                      real51Log10(&real51, &real51);             real51ToReal16(&real51,  res);}
#define real16Power(operand1, operand2, res)              {real51_t real51a, real51b; real16ToReal51(operand1, &real51a); real16ToReal51(operand2, &real51b); real51Power(&real51a, &real51b, &real51b); real51ToReal16(&real51b, res);}
#define real16SquareRoot(operand, res)                    {real51_t real51;           real16ToReal51(operand,  &real51);                                      real51SquareRoot(&real51, &real51);        real51ToReal16(&real51,  res);}
#define real16Exp(operand, res)                           {real51_t real51;           real16ToReal51(operand,  &real51);                                      real51Exp(&real51, &real51);               real51ToReal16(&real51,  res);}
#define real16Ln(operand, res)                            {real51_t real51;           real16ToReal51(operand,  &real51);                                      real51Ln(&real51, &real51);                real51ToReal16(&real51,  res);}


#define real34Log10(operand, res)                         {real51_t real51;           real34ToReal51(operand,  &real51);                                      real51Log10(&real51, &real51);             real51ToReal34(&real51,  res);}
#define real34Power(operand1, operand2, res)              {real51_t real51a, real51b; real34ToReal51(operand1, &real51a); real34ToReal51(operand2, &real51b); real51Power(&real51a, &real51b, &real51b); real51ToReal34(&real51b, res);}
#define real34SquareRoot(operand, res)                    {real51_t real51;           real34ToReal51(operand,  &real51);                                      real51SquareRoot(&real51, &real51);        real51ToReal34(&real51,  res);}
#define real34Exp(operand, res)                           {real51_t real51;           real34ToReal51(operand,  &real51);                                      real51Exp(&real51, &real51);               real51ToReal34(&real51,  res);}
#define real34Ln(operand, res)                            {real51_t real51;           real34ToReal51(operand,  &real51);                                      real51Ln(&real51, &real51);                real51ToReal34(&real51,  res);}
