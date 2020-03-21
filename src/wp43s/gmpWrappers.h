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
 * \file gmpWrappers.h
 ***********************************************/

#define longInteger_t                                                     mpz_t
#define LONG_INTEGER_POINTER(a)                                           ((longInteger_t *)(a))
#define REGISTER_LONG_INTEGER_DATA(a)                                     ((longInteger_t *)(getRegisterDataPointer(a) + sizeof(dataSize_t))) // Memory pointer to the long integer of a register
#define LIMB_SIZE                                                         sizeof(mp_limb_t)
#define LONG_INTEGER_ZERO                                                 0 // 0
#define LONG_INTEGER_NEGATIVE                                             1 // -
#define LONG_INTEGER_POSITIVE                                             2 // +
#define longIntegerSizeInBytes(li)                                        (abs((li)->_mp_size) * LIMB_SIZE)


#define longIntegerInit(op)                                               mpz_init(op);
#define longIntegerInitSizeInBits(op, bits)                               mpz_init2(op, bits);
//#define longIntegerToAllocatedString(source, destination)                 mpz_get_str(destination, 10, source)
#define uIntToLongInteger(source, destination)                            mpz_set_ui(destination, source)
#define intToLongInteger(source, destination)                             mpz_set_si(destination, source)
#define longIntegerToUInt(op)                                             mpz_get_ui(op)
#define longIntegerToInt(op)                                              mpz_get_si(op)
#define longIntegerToLongInteger(source, destination)                     mpz_set(destination, source)

#define stringToLongInteger(source, radix, destination)                   mpz_set_str(destination, source, radix)

#define longIntegerChangeSign(op)                                         {(op)->_mp_size =    -((op)->_mp_size);}
#define longIntegerSetPositiveSign(op)                                    {(op)->_mp_size =  abs((op)->_mp_size);}
#define longIntegerSetNegativeSign(op)                                    {(op)->_mp_size = -abs((op)->_mp_size);}
#define longIntegerSetZero(op)                                            {mpz_clear(op); mpz_init(op);}
#define longIntegerIsZero(op)                                             ((*(longInteger_t *)(op))->_mp_size == 0)
#define longIntegerIsPositive(op)                                         ((op)->_mp_size >  0)
#define longIntegerIsPositiveOrZero(op)                                   ((op)->_mp_size >= 0)
#define longIntegerIsNegative(op)                                         ((op)->_mp_size <  0)
#define longIntegerIsNegativeOrZero(op)                                   ((op)->_mp_size <= 0)
#define longIntegerIsEven(op)                                             mpz_even_p(op)
#define longIntegerIsOdd(op)                                              mpz_odd_p(op)
#define longIntegerSign(op)                                               mpz_sgn(op)
#define longIntegerSignTag(op)                                            ((op)->_mp_size == 0 ? LONG_INTEGER_ZERO : ((op)->_mp_size > 0 ? LONG_INTEGER_POSITIVE : LONG_INTEGER_NEGATIVE))
#define longIntegerBits(op)                                               mpz_sizeinbase(op, 2)
#define longIntegerBase10Digits(op)                                       mpz_sizeinbase(op, 10)
#define longIntegerProbabPrime(op)                                        mpz_probab_prime_p(op, 15); // 0=composite 1=probably prime 2=prime  A composite number will be identified as a prime with a probability of less than 4^(-15)
#define longIntegerFree(op)                                               mpz_clear(op)

#define longIntegerCompare(op1, op2)                                      mpz_cmp       (op1, op2)
#define longIntegerDivide(op1, op2, result)                               mpz_tdiv_q    (result, op1, op2)            /* op1/op2 => result*op2 + remainder == op1 */
#define longIntegerDivideQuotientRemainder(op1, op2, quotient, remainder) mpz_tdiv_qr   (quotient, remainder, op1, op2) /* op1/op2 => quotient*op2 + remainder == op1 */
#define longIntegerDivideRemainder(op1, op2, remainder)                   mpz_tdiv_r    (remainder, op1, op2)
#define longIntegerModulo(op1, op2, result)                               mpz_mod       (result, op1, op2)
#define longInteger2Pow(exp, result)                                      mpz_setbit    (result, exp)                 /* result = 2^exp (result MUST be 0 before)*/
//#define longIntegerDivide2(op, result)                                    fp_div_2    (op, result)                    /* result = op / 2 */
//#define longIntegerRightShift(op, number)                                 fp_rshd     (op, number);
//#define longIntegerMultiply2(op, result)                                  mpz_mul_2exp  (op, op, 1);                  /* result = op * 2 */
#define longIntegerSquareRoot(op, result)                                 mpz_sqrt      (result, op)
#define longIntegerRoot(op, n, result)                                    mpz_root      (result, op, n)
#define longIntegerPerfectSquare(op)                                      mpz_perfect_square_p(op)
#define longIntegerMultiply2(op, result)                                  mpz_mul_2exp  (result, op, 1);              /* result = op * 2 */
#define longIntegerLeftShift(op, number, result)                          mpz_mul_2exp  (result, op, number);         /* result = op * 2^number */



#define longIntegerCompareUInt(op, uint)                                  mpz_cmp_ui    (op, uint)
#define longIntegerCompareInt(op, sint)                                   mpz_cmp_si    (op, sint)
#define longIntegerAddUInt(op, uint, result)                              mpz_add_ui    (result, op, uint)
#define longIntegerSubtractUInt(op, uint, result)                         mpz_sub_ui    (result, op, uint)
#define longIntegerMultiplyUInt(op, uint, result)                         mpz_mul_ui    (result, op, uint)
#define longIntegerDivideRemainderUInt(op, uint, result, remainder)       mpz_tdiv_qr_ui(remainder, result, op, uint)    /* op/uint => result*uint + remainder == op */
#define longIntegerDivideUInt(op, uint, result)                           mpz_tdiv_q_ui (result, op, uint)               /* op/uint => result*uint + remainder == op */
#define longIntegerPowerUIntUInt(base, exponent, result)                  mpz_ui_pow_ui (result, base, exponent)         /* result = base ^ exponent */
#define longIntegerPowerModuloSec(base, exponent, modulo, result)         mpz_powm_sec  (result, base, exponent, modulo) /* result = base ^ exponent */
#define longIntegerModuloUInt(op, uint)                                   mpz_fdiv_ui   (op, uint)                       /* result = op mod uint, 0 <= result < uint */

#define longIntegerGcd(op1, op2, result)                                  mpz_gcd     (result, op1, op2)
#define longIntegerLcm(op1, op2, result)                                  mpz_lcm     (result, op1, op2)

#define longIntegerFactorial(op, result)                                  mpz_fac_ui(result, op)

// The defines below are functions now
//#define longIntegerMultiply(op1, op2, result)                             fp_mul      (op1, op2, result)
//#define longIntegerAdd(op1, op2, result)                                  fp_add      (op1, op2, result)
//#define longIntegerSubtract(op1, op2, result)                             fp_sub      (op1, op2, result)
//#define longIntegerSquare(op, result)                                     fp_sqr      (op,  result)
