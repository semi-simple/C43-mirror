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

#define BIG_INTEGER_LESS_THAN                                   FP_LT
#define BIG_INTEGER_GREATER_THAN                                FP_GT
#define BIG_INTEGER_EQUAL                                       FP_EQ

#define bigInteger_t                                            fp_int
#define BIG_INTEGER(a)                                          ((bigInteger_t *)(a))


#define bigIntegerToString(source, destination, radix)          fp_toradix  (source, destination, radix)
#define uIntToBigInteger(source, destination)                   fp_set      (destination, source)

#define stringToBigInteger(source, radix, destination)          fp_read_radix(destination, source, radix)

//#define bigIntegerChangeSign(op1, op2)                          fp_neg      (op2, op1)
#define bigIntegerSetPositiveSign(op)                           (op)->sign = 0
#define bigIntegerSetNegativeSign(op)                           {(op)->sign = 1; while((op)->used && (op)->dp[(op)->used-1] == 0) --((op)->used); (op)->sign = (op)->used ? (op)->sign : FP_ZPOS;}
#define bigIntegerSetZero(op)                                   fp_init     (op)
#define bigIntegerIsZero(op)                                    fp_iszero   (op)
//#define bigIntegerIsEven(op)                                    fp_iseven   (op)
#define bigIntegerIsOdd(op)                                     fp_isodd    (op)
//#define bigIntegerCopy(source, destination)                     fp_copy     (source, destination)

#define bigIntegerCompare(op1, op2)                             fp_cmp      (op1, op2)
//#define bigIntegerDivide(op1, op2, result)                      fp_div      (op1, op2, result, NULL)      /* op1/op2 => result*op2 + remainder == op1 */
#define bigIntegerDivideRemainder(op1, op2, result, remainder)  fp_div      (op1, op2, result, remainder)   /* op1/op2 => result*op2 + remainder == op1 */
//#define bigIntegerModulo(op1, op2, result)                      fp_mod      (op1, op2, result)            /* result = op1 mod op2, 0 <= result < op2 */
#define bigInteger2Exp(exp, result)                             fp_2expt    (result, exp)                   /* result = 2^exp */
#define bigIntegerDivide2(op, result)                           fp_div_2    (op, result)                    /* result = op / 2 */
//#define bigIntegerRightShift(op, number)                        fp_rshd     (op, number);
//#define bigIntegerMultiply2(op, result)                         fp_mul_2    (op, result);                   /* result = op * 2 */
//#define bigIntegerLeftShift(op, number)                         fp_lshd     (op, number);

#define bigIntegerCompareUInt(op, uint)                         fp_cmp_d    (op, uint)
#define bigIntegerAddUInt(op, uint, result)                     fp_add_d    (op, uint, result)
#define bigIntegerSubtractUInt(op, uint, result)                fp_sub_d    (op, uint, result)
//#define bigIntegerMultiplyUInt(op, uint, result)                fp_mul_d    (op, uint, result)
#define bigIntegerDivideUInt(op, uint, result, remainder)       fp_div_d    (op, uint, result, (fp_digit *)remainder) /* op/uint => result*uint + remainder == op */
//#define bigIntegerModuloUInt(op, uint, result)                  fp_mod_d    (op, uint, result)                      /* result = op mod uint, 0 <= result < uint */

#define bigIntegerGcd(op1, op2, result)                         fp_gcd      (op1, op2, result)
#define bigIntegerLcm(op1, op2, result)                         fp_lcm      (op1, op2, result)

// The defines below are functions now
//#define bigIntegerMultiply(op1, op2, result)                    fp_mul      (op1, op2, result)
//#define bigIntegerAdd(op1, op2, result)                         fp_add      (op1, op2, result)
//#define bigIntegerSubtract(op1, op2, result)                    fp_sub      (op1, op2, result)
//#define bigIntegerSquare(op, result)                            fp_sqr      (op,  result)
