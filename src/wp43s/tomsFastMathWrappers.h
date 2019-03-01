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
 * \file tomsFastMathWrappers.h
 ***********************************************/

#define LONG_INTEGER_LESS_THAN                                   FP_LT
#define LONG_INTEGER_GREATER_THAN                                FP_GT
#define LONG_INTEGER_EQUAL                                       FP_EQ

#define longInteger_t                                            fp_int
#define LONG_INTEGER_POINTER(a)                                  ((longInteger_t *)(a))


#define longIntegerToString(source, destination, radix)          fp_toradix  (source, destination, radix)
#define uIntToLongInteger(source, destination)                   fp_set      (destination, source)
#define longIntegerToUInt(op)                                    ((op)->dp[0])

#define stringToLongInteger(source, radix, destination)          fp_read_radix(destination, source, radix)

#define longIntegerChangeSign(op)                                {(op)->sign ^= 1;}
#define longIntegerSetPositiveSign(op)                           {(op)->sign = 0;}
#define longIntegerSetNegativeSign(op)                           {(op)->sign = 1;}
#define longIntegerSetZero(op)                                   fp_init     (op)
#define longIntegerIsZero(op)                                    fp_iszero   (op)
#define longIntegerIsPositive(op)                                ((op)->sign == 0)
#define longIntegerIsNegative(op)                                ((op)->sign == 1)
#define longIntegerIsEven(op)                                    (((op)->dp[0] & 1) == 0)
#define longIntegerIsOdd(op)                                     (((op)->dp[0] & 1) == 1)
#define longIntegerCopy(source, destination)                     fp_copy     (source, destination)

#define longIntegerCompare(op1, op2)                             fp_cmp      (op1, op2)
#define longIntegerDivide(op1, op2, result)                      fp_div      (op1, op2, result, NULL)      /* op1/op2 => result*op2 + remainder == op1 */
#define longIntegerDivideRemainder(op1, op2, result, remainder)  fp_div      (op1, op2, result, remainder)   /* op1/op2 => result*op2 + remainder == op1 */
//#define longIntegerModulo(op1, op2, result)                      fp_mod      (op1, op2, result)            /* result = op1 mod op2, 0 <= result < op2 */
#define longInteger2Exp(exp, result)                             fp_2expt    (result, exp)                   /* result = 2^exp */
#define longIntegerDivide2(op, result)                           fp_div_2    (op, result)                    /* result = op / 2 */
//#define longIntegerRightShift(op, number)                        fp_rshd     (op, number);
#define longIntegerMultiply2(op, result)                         fp_mul_2    (op, result);                   /* result = op * 2 */
//#define longIntegerLeftShift(op, number)                         fp_lshd     (op, number);

#define longIntegerCompareUInt(op, uint)                         fp_cmp_d    (op, uint)
#define longIntegerAddUInt(op, uint, result)                     fp_add_d    (op, uint, result)
#define longIntegerSubtractUInt(op, uint, result)                fp_sub_d    (op, uint, result)
#define longIntegerMultiplyUInt(op, uint, result)                fp_mul_d    (op, uint, result)
#define longIntegerDivideUInt(op, uint, result, remainder)       fp_div_d    (op, uint, result, (fp_digit *)remainder) /* op/uint => result*uint + remainder == op */
//#define longIntegerModuloUInt(op, uint, result)                  fp_mod_d    (op, uint, result)                      /* result = op mod uint, 0 <= result < uint */

#define longIntegerGcd(op1, op2, result)                         fp_gcd      (op1, op2, result)
#define longIntegerLcm(op1, op2, result)                         fp_lcm      (op1, op2, result)

// The defines below are functions now
//#define longIntegerMultiply(op1, op2, result)                    fp_mul      (op1, op2, result)
//#define longIntegerAdd(op1, op2, result)                         fp_add      (op1, op2, result)
//#define longIntegerSubtract(op1, op2, result)                    fp_sub      (op1, op2, result)
//#define longIntegerSquare(op, result)                            fp_sqr      (op,  result)
