/* TomsFastMath, a fast ISO C bignum library.
 *
 * This project is meant to fill in where LibTomMath
 * falls short.  That is speed ;-)
 *
 * This project is public domain and free for all purposes.
 *
 * Tom St Denis, tomstdenis@gmail.com
 */
#include "tfm_private.h"

void fp_add(fp_int *a, fp_int *b, fp_int *c) {
  int     sa, sb;

  // get sign of both inputs
  sa = a->sign;
  sb = b->sign;

  // handle two cases, not four
  if (sa == sb) {
    // both positive or both negative
    // add their magnitudes, copy the sign
    c->sign = sa;
    s_fp_add (a, b, c);
  } else {
    // one positive, the other negative
    // subtract the one with the greater magnitude from
    // the one of the lesser magnitude.  The result gets
    // the sign of the one with the greater magnitude.
    if (fp_cmp_mag (a, b) == FP_LT) {
      c->sign = sb;
      s_fp_sub (b, a, c);
    } else {
      c->sign = sa;
      s_fp_sub (a, b, c);
    }
  }
}

// c = a + b
void fp_add_d(fp_int *a, fp_digit b, fp_int *c) {
   fp_int tmp;
   fp_set(&tmp, b);
   fp_add(a,&tmp,c);
}
/*
// d = a + b (mod c)
int fp_addmod(fp_int *a, fp_int *b, fp_int *c, fp_int *d) {
  fp_int tmp;
  fp_zero(&tmp);
  fp_add(a, b, &tmp);
  return fp_mod(&tmp, c, d);
}
*/
int fp_cmp(fp_int *a, fp_int *b) {
   if (a->sign == FP_NEG && b->sign == FP_ZPOS) {
      return FP_LT;
   } else if (a->sign == FP_ZPOS && b->sign == FP_NEG) {
      return FP_GT;
   } else {
      // compare digits
      if (a->sign == FP_NEG) {
         // if negative compare opposite direction
         return fp_cmp_mag(b, a);
      } else {
         return fp_cmp_mag(a, b);
      }
   }
}

// compare against a single digit
int fp_cmp_d(fp_int *a, fp_digit b) {
  // compare based on sign
  if ((b && a->used == 0) || a->sign == FP_NEG) {
    return FP_LT;
  }

  // compare based on magnitude
  if (a->used > 1) {
    return FP_GT;
  }

  // compare the only digit of a to b
  if (a->dp[0] > b) {
    return FP_GT;
  } else if (a->dp[0] < b) {
    return FP_LT;
  } else {
    return FP_EQ;
  }
}

int fp_cmp_mag(fp_int *a, fp_int *b) {
   int x;

   if (a->used > b->used) {
      return FP_GT;
   } else if (a->used < b->used) {
      return FP_LT;
   } else {
      for (x = a->used - 1; x >= 0; x--) {
          if (a->dp[x] > b->dp[x]) {
             return FP_GT;
          } else if (a->dp[x] < b->dp[x]) {
             return FP_LT;
          }
      }
   }
   return FP_EQ;
}

// c = a - b
void fp_sub(fp_int *a, fp_int *b, fp_int *c) {
  int     sa, sb;

  sa = a->sign;
  sb = b->sign;

  if (sa != sb) {
    // subtract a negative from a positive, OR
    // subtract a positive from a negative.
    // In either case, ADD their magnitudes,
    // and use the sign of the first number.
    c->sign = sa;
    s_fp_add (a, b, c);
  } else {
    // subtract a positive from a positive, OR
    // subtract a negative from a negative.
    // First, take the difference between their
    // magnitudes, then...
    if (fp_cmp_mag (a, b) != FP_LT) {
      // Copy the sign from the first
      c->sign = sa;
      // The first has a larger or equal magnitude
      s_fp_sub (a, b, c);
    } else {
      // The result has the *opposite* sign from
      // the first number.
      c->sign = (sa == FP_ZPOS) ? FP_NEG : FP_ZPOS;
      // The second has a larger magnitude
      s_fp_sub (b, a, c);
    }
  }
}

// c = a - b
void fp_sub_d(fp_int *a, fp_digit b, fp_int *c) {
   fp_int tmp;
   fp_set(&tmp, b);
   fp_sub(a, &tmp, c);
}
/*
// d = a - b (mod c)
int fp_submod(fp_int *a, fp_int *b, fp_int *c, fp_int *d) {
  fp_int tmp;
  fp_zero(&tmp);
  fp_sub(a, b, &tmp);
  return fp_mod(&tmp, c, d);
}
*/
// unsigned addition
void s_fp_add(fp_int *a, fp_int *b, fp_int *c) {
  int      x, y, oldused;
  register fp_word  t;

  y       = MAX(a->used, b->used);
  oldused = MIN(c->used, FP_SIZE);
  c->used = y;

  t = 0;
  for (x = 0; x < y; x++) {
      t         += ((fp_word)a->dp[x]) + ((fp_word)b->dp[x]);
      c->dp[x]   = (fp_digit)t;
      t        >>= DIGIT_BIT;
  }
  if (t != 0 && x < FP_SIZE) {
     c->dp[c->used++] = (fp_digit)t;
     ++x;
  }

  c->used = x;
  for (; x < oldused; x++) {
     c->dp[x] = 0;
  }
  fp_clamp(c);
}

// unsigned subtraction ||a|| >= ||b|| ALWAYS!
void s_fp_sub(fp_int *a, fp_int *b, fp_int *c) {
  int      x, oldbused, oldused;
  fp_word  t;

  oldused  = c->used;
  oldbused = b->used;
  c->used  = a->used;
  t       = 0;
  for (x = 0; x < oldbused; x++) {
     t         = ((fp_word)a->dp[x]) - (((fp_word)b->dp[x]) + t);
     c->dp[x]  = (fp_digit)t;
     t         = (t >> DIGIT_BIT)&1;
  }
  for (; x < a->used; x++) {
     t         = ((fp_word)a->dp[x]) - t;
     c->dp[x]  = (fp_digit)t;
     t         = (t >> DIGIT_BIT)&1;
   }
  for (; x < oldused; x++) {
     c->dp[x] = 0;
  }
  fp_clamp(c);
}
/*
int fp_radix_size(fp_int *a, int radix, int *size) {
  fp_int  t;
  fp_digit d;

  *size = 0;

  // check range of the radix
  if (radix < 2 || radix > 64) {
    return FP_VAL;
  }

  // quick out if its zero
  if (fp_iszero(a) == 1) {
     *size = 2;
     return FP_OKAY;
  }

  fp_init_copy(&t, a);

  // if it is negative output a -
  if (t.sign == FP_NEG) {
    (*size)++;
    t.sign = FP_ZPOS;
  }

  while (fp_iszero (&t) == FP_NO) {
    fp_div_d (&t, (fp_digit) radix, &t, &d);
    (*size)++;
  }

  // append a NULL so the string is properly terminated
  (*size)++;
  return FP_OKAY;

}
*/
int fp_read_radix(fp_int *a, const char *str, int radix) {
  int     y, neg;
  char    ch;

  // set the integer to the default of zero
  fp_zero (a);

  // make sure the radix is ok
  if (radix < 2 || radix > 64) {
    return FP_VAL;
  }

  // if the leading digit is a minus set the sign to negative.

  if (*str == '-') {
    ++str;
    neg = FP_NEG;
  } else {
    neg = FP_ZPOS;
  }

  // process each digit of the string
  while (*str) {
    // if the radix < 36 the conversion is case insensitive this allows
    // numbers like 1AB and 1ab to represent the same  value [e.g. in hex]

    ch = (char) ((radix <= 36) ? toupper ((int)*str) : *str);
    for (y = 0; y < 64; y++) {
      if (ch == fp_s_rmap[y]) {
         break;
      }
    }

    // if the char was found in the map
    // and is less than the given radix add it
     //to the number, otherwise exit the loop.

    if (y < radix) {
      fp_mul_d (a, (fp_digit) radix, a);
      fp_add_d (a, (fp_digit) y, a);
    } else {
      break;
    }
    ++str;
  }

  // set the sign only if a != 0
  if (fp_iszero(a) != FP_YES) {
     a->sign = neg;
  }
  return FP_OKAY;
}

/*void fp_read_signed_bin(fp_int *a, const unsigned char *b, int c) {
  // read magnitude
  fp_read_unsigned_bin (a, b + 1, c - 1);

  // first byte is 0 for positive, non-zero for negative
  if (b[0] == 0) {
     a->sign = FP_ZPOS;
  } else {
     a->sign = FP_NEG;
  }
}

void fp_read_unsigned_bin(fp_int *a, const unsigned char *b, int c) {
  // zero the int
  fp_zero (a);

  // If we know the endianness of this architecture, and we're using 32-bit fp_digits, we can optimize this
#if (defined(ENDIAN_LITTLE) || defined(ENDIAN_BIG)) && !defined(FP_64BIT)
  // But not for both simultaneously
#if defined(ENDIAN_LITTLE) && defined(ENDIAN_BIG)
#error Both ENDIAN_LITTLE and ENDIAN_BIG defined.
#endif
  {
     unsigned char *pd = (unsigned char *)a->dp;

     if ((unsigned)c > (FP_SIZE * sizeof(fp_digit))) {
        int excess = c - (FP_SIZE * sizeof(fp_digit));
        c -= excess;
        b += excess;
     }
     a->used = (c + sizeof(fp_digit) - 1)/sizeof(fp_digit);
     // read the bytes in
#ifdef ENDIAN_BIG
     {
       // Use Duff's device to unroll the loop.
       int idx = (c - 1) & ~3;
       switch (c % 4) {
       case 0:	do { pd[idx+0] = *b++;
       case 3:	     pd[idx+1] = *b++;
       case 2:	     pd[idx+2] = *b++;
       case 1:	     pd[idx+3] = *b++;
                     idx -= 4;
	 	        } while ((c -= 4) > 0);
       }
     }
#else
     for (c -= 1; c >= 0; c -= 1) {
       pd[c] = *b++;
     }
#endif
  }
#else
  // read the bytes in
  for (; c > 0; c--) {
     fp_mul_2d (a, 8, a);
     a->dp[0] |= *b++;
     a->used += 1;
  }
#endif
  fp_clamp (a);
}
*/
// reverse an array, used for radix code
void fp_reverse (unsigned char *s, int len) {
  int     ix, iy;
  unsigned char t;

  ix = 0;
  iy = len - 1;
  while (ix < iy) {
    t     = s[ix];
    s[ix] = s[iy];
    s[iy] = t;
    ++ix;
    --iy;
  }
}
/*
int fp_signed_bin_size(fp_int *a) {
  return 1 + fp_unsigned_bin_size (a);
}
*/
// chars used in radix conversions
const char *fp_s_rmap = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz+/";

// a:		pointer to fp_int representing the input number
// str:		output buffer
// radix:	number of character to use for encoding of the number
//
// The radix value can be in the range 2 to 64. This function converts number
// a into a string str. Please don't use this function because a too small
// chosen str buffer would lead to an overflow which can not be detected.
// Please use fp_toradix_n() instead.
//
// Return: FP_VAL on error, FP_OKAY on success.
int fp_toradix(fp_int *a, char *str, int radix) {
   return fp_toradix_n(a, str, radix, INT_MAX);
}

int fp_toradix_n(fp_int *a, char *str, int radix, int maxlen) {
   int digs;
   fp_int t;
   fp_digit d = 0;
   char *_s = str;

   // check range of the radix
   if (maxlen < 2 || radix < 2 || radix > 64)
      return FP_VAL;

   // quick check for zero
   if (fp_iszero(a) == FP_YES) {
      *str++ = '0';
      *str = '\0';
      return FP_OKAY;
   }

   fp_init_copy(&t, a);

   // if it is negative output a -
   if (t.sign == FP_NEG) {
      // we have to reverse our digits later... but not the - sign!!
      ++_s;

      // store the flag and mark the number as positive
      *str++ = '-';
      t.sign = FP_ZPOS;

      // subtract a char
      --maxlen;
   }

   digs = 0;
   while (fp_iszero (&t) == FP_NO) {
      if (--maxlen < 1) {
         // no more room
         break;
      }
      fp_div_d(&t, (fp_digit) radix, &t, &d);
      *str++ = fp_s_rmap[d];
      ++digs;
   }

   // reverse the digits of the string.  In this case _s points to the first digit [exluding the sign] of the number]

   fp_reverse((unsigned char *) _s, digs);

   // append a NULL so the string is properly terminated
   *str = '\0';

   if (maxlen < 1)
      return FP_VAL;
   return FP_OKAY;
}
/*
void fp_to_signed_bin(fp_int *a, unsigned char *b) {
  fp_to_unsigned_bin (a, b + 1);
  b[0] = (unsigned char) ((a->sign == FP_ZPOS) ? 0 : 1);
}

void fp_to_unsigned_bin(fp_int *a, unsigned char *b) {
  int     x;
  fp_int  t;

  fp_init_copy(&t, a);

  x = 0;
  while (fp_iszero (&t) == FP_NO) {
      b[x++] = (unsigned char) (t.dp[0] & 255);
      fp_div_2d (&t, 8, &t, NULL);
  }
  fp_reverse (b, x);
}

int fp_unsigned_bin_size(fp_int *a) {
  int     size = fp_count_bits (a);
  return (size / 8 + ((size & 7) != 0 ? 1 : 0));
}

static const int lnz[16] = {4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0};

// Counts the number of lsbs which are zero before the first zero bit
int fp_cnt_lsb(fp_int *a) {
   int x;
   fp_digit q, qq;

   // easy out
   if (fp_iszero(a) == 1) {
      return 0;
   }

   // scan lower digits until non-zero
   for (x = 0; x < a->used && a->dp[x] == 0; x++);
   q = a->dp[x];
   x *= DIGIT_BIT;

   // now scan this digit until a 1 is found
   if ((q & 1) == 0) {
      do {
         qq  = q & 15;
         x  += lnz[qq];
         q >>= 4;
      } while (qq == 0);
   }
   return x;
}
*/
int fp_count_bits (fp_int * a) {
  int     r;
  fp_digit q;

  // shortcut
  if (a->used == 0) {
    return 0;
  }

  // get number of digits and add that
  r = (a->used - 1) * DIGIT_BIT;

  // take the last digit and count the bits in it
  q = a->dp[a->used - 1];
  while (q > ((fp_digit) 0)) {
    ++r;
    q >>= ((fp_digit) 1);
  }
  return r;
}

// b = a/2
void fp_div_2(fp_int * a, fp_int * b) {
  int     x, oldused;

  oldused = b->used;
  b->used = a->used;
  {
    register fp_digit r, rr, *tmpa, *tmpb;

    // source alias
    tmpa = a->dp + b->used - 1;

    // dest alias
    tmpb = b->dp + b->used - 1;

    // carry
    r = 0;
    for (x = b->used - 1; x >= 0; x--) {
      // get the carry for the next iteration
      rr = *tmpa & 1;

      // shift the current digit, add in carry and store
      *tmpb-- = (*tmpa-- >> 1) | (r << (DIGIT_BIT - 1));

      // forward carry to next iteration
      r = rr;
    }

    // zero excess digits
    tmpb = b->dp + b->used;
    for (x = b->used; x < oldused; x++) {
      *tmpb++ = 0;
    }
  }
  b->sign = a->sign;
  fp_clamp (b);
}

// c = a / 2**b
void fp_div_2d(fp_int *a, int b, fp_int *c, fp_int *d) {
  fp_digit D, r, rr;
  int      x;
  fp_int   t;

  // if the shift count is <= 0 then we do no work
  if (b <= 0) {
    fp_copy (a, c);
    if (d != NULL) {
      fp_zero (d);
    }
    return;
  }

  fp_init(&t);

  // get the remainder
  if (d != NULL) {
    fp_mod_2d (a, b, &t);
  }

  // copy
  fp_copy(a, c);

  // shift by as many digits in the bit count
  if (b >= (int)DIGIT_BIT) {
    fp_rshd (c, b / DIGIT_BIT);
  }

  // shift any bit count < DIGIT_BIT
  D = (fp_digit) (b % DIGIT_BIT);
  if (D != 0) {
    register fp_digit *tmpc, mask, shift;

    // mask
    mask = (((fp_digit)1) << D) - 1;

    // shift for lsb
    shift = DIGIT_BIT - D;

    // alias
    tmpc = c->dp + (c->used - 1);

    // carry
    r = 0;
    for (x = c->used - 1; x >= 0; x--) {
      // get the lower  bits of this word in a temp
      rr = *tmpc & mask;

      // shift the current word and mix in the carry bits from the previous word
      *tmpc = (*tmpc >> D) | (r << shift);
      --tmpc;

      // set the carry to the carry bits of the current word found above
      r = rr;
    }
  }
  fp_clamp (c);
  if (d != NULL) {
    fp_copy (&t, d);
  }
}

void fp_lshd(fp_int *a, int x) {
   int y;

   // move up and truncate as required
   y = MIN(a->used + x - 1, (int)(FP_SIZE-1));

   // store new size
   a->used = y + 1;

   // move digits
   for (; y >= x; y--) {
       a->dp[y] = a->dp[y-x];
   }

   // zero lower digits
   for (; y >= 0; y--) {
       a->dp[y] = 0;
   }

   // clamp digits
   fp_clamp(a);
}

// c = a mod 2**d
void fp_mod_2d(fp_int *a, int b, fp_int *c) {
   int x;

   // zero if count less than or equal to zero
   if (b <= 0) {
      fp_zero(c);
      return;
   }

   // get copy of input
   fp_copy(a, c);

   // if 2**d is larger than we just return
   if (b >= (DIGIT_BIT * a->used)) {
      return;
   }

  // zero digits above the last digit of the modulus
  for (x = (b / DIGIT_BIT) + ((b % DIGIT_BIT) == 0 ? 0 : 1); x < c->used; x++) {
    c->dp[x] = 0;
  }
  // clear the digit that is not completely outside/inside the modulus
  c->dp[b / DIGIT_BIT] &= ~((fp_digit)0) >> (DIGIT_BIT - b);
  fp_clamp (c);
}

void fp_rshd(fp_int *a, int x) {
  int y;

  // too many digits just zero and return
  if (x >= a->used) {
     fp_zero(a);
     return;
  }

   // shift
   for (y = 0; y < a->used - x; y++) {
      a->dp[y] = a->dp[y+x];
   }

   // zero rest
   for (; y < a->used; y++) {
      a->dp[y] = 0;
   }

   // decrement count
   a->used -= x;
   fp_clamp(a);
}

// a/b => cb + d == a
int fp_div(fp_int *a, fp_int *b, fp_int *c, fp_int *d) {
  fp_int  q, x, y, t1, t2;
  int     n, t, i, norm, neg;

  // is divisor zero ?
  if (fp_iszero (b) == 1) {
    return FP_VAL;
  }

  // if a < b then q=0, r = a
  if (fp_cmp_mag (a, b) == FP_LT) {
    if (d != NULL) {
      fp_copy (a, d);
    }
    if (c != NULL) {
      fp_zero (c);
    }
    return FP_OKAY;
  }

  fp_init(&q);
  q.used = a->used + 2;

  fp_init(&t1);
  fp_init(&t2);
  fp_init_copy(&x, a);
  fp_init_copy(&y, b);

  // fix the sign
  neg = (a->sign == b->sign) ? FP_ZPOS : FP_NEG;
  x.sign = y.sign = FP_ZPOS;

  // normalize both x and y, ensure that y >= b/2, [b == 2**DIGIT_BIT]
  norm = fp_count_bits(&y) % DIGIT_BIT;
  if (norm < (int)(DIGIT_BIT-1)) {
     norm = (DIGIT_BIT-1) - norm;
     fp_mul_2d (&x, norm, &x);
     fp_mul_2d (&y, norm, &y);
  } else {
     norm = 0;
  }

  // note hac does 0 based, so if used==5 then its 0,1,2,3,4, e.g. use 4
  n = x.used - 1;
  t = y.used - 1;

  // while (x >= y*b**n-t) do { q[n-t] += 1; x -= y*b**{n-t} }
  fp_lshd (&y, n - t);                                             // y = y*b**{n-t}

  while (fp_cmp (&x, &y) != FP_LT) {
    ++(q.dp[n - t]);
    fp_sub (&x, &y, &x);
  }

  // reset y by shifting it back down
  fp_rshd (&y, n - t);

  // step 3. for i from n down to (t + 1)
  for (i = n; i >= (t + 1); i--) {
    if (i > x.used) {
      continue;
    }

    // step 3.1 if xi == yt then set q{i-t-1} to b-1, otherwise set q{i-t-1} to (xi*b + x{i-1})/yt
    if (x.dp[i] == y.dp[t]) {
      q.dp[i - t - 1] = ((((fp_word)1) << DIGIT_BIT) - 1);
    } else {
      fp_word tmp;
      tmp = ((fp_word) x.dp[i]) << ((fp_word) DIGIT_BIT);
      tmp |= ((fp_word) x.dp[i - 1]);
      tmp /= ((fp_word) y.dp[t]);
      q.dp[i - t - 1] = (fp_digit) (tmp);
    }

    // while (q{i-t-1} * (yt * b + y{t-1})) > xi * b**2 + xi-1 * b + xi-2
    // do q{i-t-1} -= 1;

    q.dp[i - t - 1] = (q.dp[i - t - 1] + 1);
    do {
      q.dp[i - t - 1] = (q.dp[i - t - 1] - 1);

      // find left hand
      fp_zero (&t1);
      t1.dp[0] = (t - 1 < 0) ? 0 : y.dp[t - 1];
      t1.dp[1] = y.dp[t];
      t1.used = 2;
      fp_mul_d (&t1, q.dp[i - t - 1], &t1);

      // find right hand
      t2.dp[0] = (i - 2 < 0) ? 0 : x.dp[i - 2];
      t2.dp[1] = (i - 1 < 0) ? 0 : x.dp[i - 1];
      t2.dp[2] = x.dp[i];
      t2.used = 3;
    } while (fp_cmp_mag(&t1, &t2) == FP_GT);

    // step 3.3 x = x - q{i-t-1} * y * b**{i-t-1}
    fp_mul_d (&y, q.dp[i - t - 1], &t1);
    fp_lshd  (&t1, i - t - 1);
    fp_sub   (&x, &t1, &x);

    // if x < 0 then { x = x + y*b**{i-t-1}; q{i-t-1} -= 1; }
    if (x.sign == FP_NEG) {
      fp_copy (&y, &t1);
      fp_lshd (&t1, i - t - 1);
      fp_add (&x, &t1, &x);
      q.dp[i - t - 1] = q.dp[i - t - 1] - 1;
    }
  }

  // now q is the quotient and x is the remainder [which we have to normalize]


  // get sign before writing to c
  x.sign = x.used == 0 ? FP_ZPOS : a->sign;

  if (c != NULL) {
    fp_clamp (&q);
    fp_copy (&q, c);
    c->sign = neg;
  }

  if (d != NULL) {
    fp_div_2d (&x, norm, &x, NULL);

// the following is a kludge, essentially we were seeing the right remainder but with excess digits that should have been zero

    for (i = b->used; i < x.used; i++) {
        x.dp[i] = 0;
    }
    fp_clamp(&x);
    fp_copy (&x, d);
  }

  return FP_OKAY;
}

static int s_is_power_of_two(fp_digit b, int *p) {
   int x;

   // fast return if no power of two
   if ((b==0) || (b & (b-1))) {
      return 0;
   }

   for (x = 0; x < DIGIT_BIT; x++) {
      if (b == (((fp_digit)1)<<x)) {
         *p = x;
         return 1;
      }
   }
   return 0;
}

// a/b => cb + d == a
int fp_div_d(fp_int *a, fp_digit b, fp_int *c, fp_digit *d) {
  fp_int   q;
  fp_word  w;
  fp_digit t;
  int      ix;

  // cannot divide by zero
  if (b == 0) {
     return FP_VAL;
  }

  // quick outs
  if (b == 1 || fp_iszero(a) == 1) {
     if (d != NULL) {
        *d = 0;
     }
     if (c != NULL) {
        fp_copy(a, c);
     }
     return FP_OKAY;
  }

  // power of two ?
  if (s_is_power_of_two(b, &ix) == 1) {
     if (d != NULL) {
        *d = a->dp[0] & ((((fp_digit)1)<<ix) - 1);
     }
     if (c != NULL) {
        fp_div_2d(a, ix, c, NULL);
     }
     return FP_OKAY;
  }

  // no easy answer [c'est la vie].  Just division
  fp_init(&q);

  q.used = a->used;
  q.sign = a->sign;
  w = 0;
  for (ix = a->used - 1; ix >= 0; ix--) {
     w = (w << ((fp_word)DIGIT_BIT)) | ((fp_word)a->dp[ix]);

     if (w >= b) {
        t = (fp_digit)(w / b);
        w -= ((fp_word)t) * ((fp_word)b);
      } else {
        t = 0;
      }
      q.dp[ix] = (fp_digit)t;
  }

  if (d != NULL) {
     *d = (fp_digit)w;
  }

  if (c != NULL) {
     fp_clamp(&q);
     fp_copy(&q, c);
  }

  return FP_OKAY;
}

// c = a mod b, 0 <= c < b
int fp_mod(fp_int *a, fp_int *b, fp_int *c) {
   fp_int t;
   int    err;

   fp_zero(&t);
   if ((err = fp_div(a, b, NULL, &t)) != FP_OKAY) {
      return err;
   }
   if (t.sign != b->sign) {
      fp_add(&t, b, c);
   } else {
      fp_copy(&t, c);
  }
  return FP_OKAY;
}
/*
// c = a mod b, 0 <= c < b
int fp_mod_d(fp_int *a, fp_digit b, fp_digit *c) {
   return fp_div_d(a, b, NULL, c);
}
*/
// computes a = 2**b
void fp_2expt(fp_int *a, int b) {
   int     z;

   // zero a as per default
   fp_zero (a);

   if (b < 0) {
      return;
   }

   z = b / DIGIT_BIT;
   if (z >= FP_SIZE) {
      return;
   }

  // set the used count of where the bit will go
  a->used = z + 1;

  // put the single bit in its place
  a->dp[z] = ((fp_digit)1) << (b % DIGIT_BIT);
}
/*
#ifdef TFM_TIMING_RESISTANT

// timing resistant montgomery ladder based exptmod

   Based on work by Marc Joye, Sung-Ming Yen, "The Montgomery Powering Ladder", Cryptographic Hardware and Embedded Systems, CHES 2002

static int _fp_exptmod(fp_int * G, fp_int * X, fp_int * P, fp_int * Y) {
  fp_int   R[2];
  fp_digit buf, mp;
  int      err, bitcnt, digidx, y;

  // now setup montgomery
  if ((err = fp_montgomery_setup (P, &mp)) != FP_OKAY) {
     return err;
  }

  fp_init(&R[0]);
  fp_init(&R[1]);

  // now we need R mod m
  fp_montgomery_calc_normalization (&R[0], P);

  // now set R[0][1] to G * R mod m
  if (fp_cmp_mag(P, G) != FP_GT) {
     // G > P so we reduce it first
     fp_mod(G, P, &R[1]);
  } else {
     fp_copy(G, &R[1]);
  }
  fp_mulmod (&R[1], &R[0], P, &R[1]);

  // for j = t-1 downto 0 do
        r_!k = R0*R1; r_k = r_k^2


  // set initial mode and bit cnt
  bitcnt = 1;
  buf    = 0;
  digidx = X->used - 1;

  for (;;) {
    // grab next digit as required
    if (--bitcnt == 0) {
      // if digidx == -1 we are out of digits so break
      if (digidx == -1) {
        break;
      }
      // read next digit and reset bitcnt
      buf    = X->dp[digidx--];
      bitcnt = (int)DIGIT_BIT;
    }

    // grab the next msb from the exponent
    y     = (fp_digit)(buf >> (DIGIT_BIT - 1)) & 1;
    buf <<= (fp_digit)1;

    // do ops
    fp_mul(&R[0], &R[1], &R[y^1]); fp_montgomery_reduce(&R[y^1], P, mp);
    fp_sqr(&R[y], &R[y]);          fp_montgomery_reduce(&R[y], P, mp);
  }

   fp_montgomery_reduce(&R[0], P, mp);
   fp_copy(&R[0], Y);
   return FP_OKAY;
}

#else

// y = g**x (mod b) Some restrictions... x must be positive and < b

static int _fp_exptmod(fp_int * G, fp_int * X, fp_int * P, fp_int * Y) {
  fp_int   M[64], res;
  fp_digit buf, mp;
  int      err, bitbuf, bitcpy, bitcnt, mode, digidx, x, y, winsize;

  // find window size
  x = fp_count_bits (X);
  if (x <= 21) {
    winsize = 1;
  } else if (x <= 36) {
    winsize = 3;
  } else if (x <= 140) {
    winsize = 4;
  } else if (x <= 450) {
    winsize = 5;
  } else {
    winsize = 6;
  }

  // init M array
  memset(M, 0, sizeof(M));

  // now setup montgomery
  if ((err = fp_montgomery_setup (P, &mp)) != FP_OKAY) {
     return err;
  }

  // setup result
  fp_init(&res);

  // create M table
  // The M table contains powers of the input base, e.g. M[x] = G^x mod P
  // The first half of the table is not computed though accept for M[0] and M[1]

   // now we need R mod m
   fp_montgomery_calc_normalization (&res, P);

   // now set M[1] to G * R mod m
   if (fp_cmp_mag(P, G) != FP_GT) {
      // G > P so we reduce it first
      fp_mod(G, P, &M[1]);
   } else {
      fp_copy(G, &M[1]);
   }
   fp_mulmod (&M[1], &res, P, &M[1]);

  // compute the value at M[1<<(winsize-1)] by squaring M[1] (winsize-1) times
  fp_copy (&M[1], &M[1 << (winsize - 1)]);
  for (x = 0; x < (winsize - 1); x++) {
    fp_sqr (&M[1 << (winsize - 1)], &M[1 << (winsize - 1)]);
    fp_montgomery_reduce (&M[1 << (winsize - 1)], P, mp);
  }

  // create upper table
  for (x = (1 << (winsize - 1)) + 1; x < (1 << winsize); x++) {
    fp_mul(&M[x - 1], &M[1], &M[x]);
    fp_montgomery_reduce(&M[x], P, mp);
  }

  // set initial mode and bit cnt
  mode   = 0;
  bitcnt = 1;
  buf    = 0;
  digidx = X->used - 1;
  bitcpy = 0;
  bitbuf = 0;

  for (;;) {
    // grab next digit as required
    if (--bitcnt == 0) {
      // if digidx == -1 we are out of digits so break
      if (digidx == -1) {
        break;
      }
      // read next digit and reset bitcnt
      buf    = X->dp[digidx--];
      bitcnt = (int)DIGIT_BIT;
    }

    // grab the next msb from the exponent
    y     = (fp_digit)(buf >> (DIGIT_BIT - 1)) & 1;
    buf <<= (fp_digit)1;

    // if the bit is zero and mode == 0 then we ignore it
    // These represent the leading zero bits before the first 1 bit
    // in the exponent.  Technically this opt is not required but it
    // does lower the # of trivial squaring/reductions used

    if (mode == 0 && y == 0) {
      continue;
    }

    // if the bit is zero and mode == 1 then we square
    if (mode == 1 && y == 0) {
      fp_sqr(&res, &res);
      fp_montgomery_reduce(&res, P, mp);
      continue;
    }

    // else we add it to the window
    bitbuf |= (y << (winsize - ++bitcpy));
    mode    = 2;

    if (bitcpy == winsize) {
      // ok window is filled so square as required and multiply
      // square first
      for (x = 0; x < winsize; x++) {
        fp_sqr(&res, &res);
        fp_montgomery_reduce(&res, P, mp);
      }

      // then multiply
      fp_mul(&res, &M[bitbuf], &res);
      fp_montgomery_reduce(&res, P, mp);

      // empty window and reset
      bitcpy = 0;
      bitbuf = 0;
      mode   = 1;
    }
  }

  // if bits remain then square/multiply
  if (mode == 2 && bitcpy > 0) {
    // square then multiply if the bit is set
    for (x = 0; x < bitcpy; x++) {
      fp_sqr(&res, &res);
      fp_montgomery_reduce(&res, P, mp);

      // get next bit of the window
      bitbuf <<= 1;
      if ((bitbuf & (1 << winsize)) != 0) {
        // then multiply
        fp_mul(&res, &M[1], &res);
        fp_montgomery_reduce(&res, P, mp);
      }
    }
  }

  // fixup result if Montgomery reduction is used
  // recall that any value in a Montgomery system is
  // actually multiplied by R mod n.  So we have
  // to reduce one more time to cancel out the factor
  // of R.

  fp_montgomery_reduce(&res, P, mp);

  // swap res with Y
  fp_copy (&res, Y);
  return FP_OKAY;
}

#endif


int fp_exptmod(fp_int * G, fp_int * X, fp_int * P, fp_int * Y) {
   fp_int tmp;
   int    err;

#ifdef TFM_CHECK
   // prevent overflows
   if (P->used > (FP_SIZE/2)) {
      return FP_VAL;
   }
#endif

   // is X negative?
   if (X->sign == FP_NEG) {
      // yes, copy G and invmod it
      fp_copy(G, &tmp);
      if ((err = fp_invmod(&tmp, P, &tmp)) != FP_OKAY) {
         return err;
      }
      X->sign = FP_ZPOS;
      err =  _fp_exptmod(&tmp, X, P, Y);
      if (X != Y) {
         X->sign = FP_NEG;
      }
      return err;
   } else {
      // Positive exponent so just exptmod
      return _fp_exptmod(G, X, P, Y);
   }
}

const char *fp_ident(void) {
   static char buf[1024];

   memset(buf, 0, sizeof(buf));
   snprintf(buf, sizeof(buf)-1,
"TomsFastMath " TFM_VERSION_S "\n"
#if defined(TFM_IDENT_BUILD_DATE)
"Built on " __DATE__ " at " __TIME__ "\n"
#endif
"\n"
"Sizeofs\n"
"\tfp_digit = %lu\n"
"\tfp_word  = %lu\n"
"\n"
"FP_MAX_SIZE = %u\n"
"\n"
"Defines: \n"
#ifdef __i386__
" __i386__ "
#endif
#ifdef __x86_64__
" __x86_64__ "
#endif
#ifdef TFM_X86
" TFM_X86 "
#endif
#ifdef TFM_X86_64
" TFM_X86_64 "
#endif
#ifdef TFM_SSE2
" TFM_SSE2 "
#endif
#ifdef TFM_ARM
" TFM_ARM "
#endif
#ifdef TFM_PPC32
" TFM_PPC32 "
#endif
#ifdef TFM_AVR32
" TFM_AVR32 "
#endif
#ifdef TFM_ECC192
" TFM_ECC192 "
#endif
#ifdef TFM_ECC224
" TFM_ECC224 "
#endif
#ifdef TFM_ECC384
" TFM_ECC384 "
#endif
#ifdef TFM_ECC521
" TFM_ECC521 "
#endif

#ifdef TFM_NO_ASM
" TFM_NO_ASM "
#endif
#ifdef FP_64BIT
" FP_64BIT "
#endif
#ifdef TFM_HUGE
" TFM_HUGE "
#endif
"\n", (unsigned long)sizeof(fp_digit), (unsigned long)sizeof(fp_word), FP_MAX_SIZE);

   if (sizeof(fp_digit) == sizeof(fp_word)) {
      strncat(buf, "WARNING: sizeof(fp_digit) == sizeof(fp_word), this build is likely to not work properly.\n",
              sizeof(buf) - strlen(buf) - 1);
   }
   return buf;
}

#if FP_GEN_RANDOM_MAX == 0xffffffff
  #define FP_GEN_RANDOM_SHIFT  32
#elif FP_GEN_RANDOM_MAX == 32767
  // SHRT_MAX
  #define FP_GEN_RANDOM_SHIFT  15
#elif FP_GEN_RANDOM_MAX == 2147483647
  // INT_MAX
  #define FP_GEN_RANDOM_SHIFT  31
#elif !defined(FP_GEN_RANDOM_SHIFT)
#error Thou shalt define their own valid FP_GEN_RANDOM_SHIFT
#endif

// makes a pseudo-random int of a given size
static fp_digit fp_gen_random(void) {
  fp_digit d = 0, msk = 0;
  do {
    d <<= FP_GEN_RANDOM_SHIFT;
    d |= ((fp_digit) FP_GEN_RANDOM());
    msk <<= FP_GEN_RANDOM_SHIFT;
    msk |= FP_GEN_RANDOM_MAX;
  } while ((FP_MASK & msk) != FP_MASK);
  d &= FP_MASK;
  return d;
}

void fp_rand(fp_int *a, int digits) {
   fp_digit d;

   fp_zero(a);
   if (digits <= 0) {
     return;
   }

   // first place a random non-zero digit
   do {
     d = fp_gen_random();
   } while (d == 0);

   fp_add_d (a, d, a);

   while (--digits > 0) {
     fp_lshd (a, 1);
     fp_add_d (a, fp_gen_random(), a);
   }

   return;

}
*/
void fp_set(fp_int *a, fp_digit b) {
   fp_zero(a);
   a->dp[0] = b;
   a->used  = a->dp[0] ? 1 : 0;
}
/*
// computes a = B**n mod b without division or multiplication useful for normalizing numbers in a Montgomery system.
void fp_montgomery_calc_normalization(fp_int *a, fp_int *b) {
  int     x, bits;

  // how many bits of last digit does b use
  bits = fp_count_bits (b) % DIGIT_BIT;
  if (!bits) bits = DIGIT_BIT;

  // compute A = B^(n-1) * 2^(bits-1)
  if (b->used > 1) {
     fp_2expt (a, (b->used - 1) * DIGIT_BIT + bits - 1);
  } else {
     fp_set(a, 1);
     bits = 1;
  }

  // now compute C = A * B mod b
  for (x = bits - 1; x < (int)DIGIT_BIT; x++) {
    fp_mul_2 (a, a);
    if (fp_cmp_mag (a, b) != FP_LT) {
      s_fp_sub (a, b, a);
    }
  }
}

// ****************************************************************
#if defined(TFM_X86) && !defined(TFM_SSE2)
// x86-32 code

#define MONT_START
#define MONT_FINI
#define LOOP_END
#define LOOP_START \
   mu = c[x] * mp

#define INNERMUL                                          \
asm(                                                      \
   "movl %5,%%eax \n\t"                                   \
   "mull %4       \n\t"                                   \
   "addl %1,%%eax \n\t"                                   \
   "adcl $0,%%edx \n\t"                                   \
   "addl %%eax,%0 \n\t"                                   \
   "adcl $0,%%edx \n\t"                                   \
   "movl %%edx,%1 \n\t"                                   \
:"=g"(_c[LO]), "=r"(cy)                                   \
:"0"(_c[LO]), "1"(cy), "r"(mu), "r"(*tmpm++)              \
: "%eax", "%edx", "cc")

#define PROPCARRY                           \
asm(                                        \
   "addl   %1,%0    \n\t"                   \
   "setb   %%al     \n\t"                   \
   "movzbl %%al,%1 \n\t"                    \
:"=g"(_c[LO]), "=r"(cy)                     \
:"0"(_c[LO]), "1"(cy)                       \
: "%eax", "cc")

// ****************************************************************
#elif defined(TFM_X86_64)
// x86-64 code

#define MONT_START
#define MONT_FINI
#define LOOP_END
#define LOOP_START \
   mu = c[x] * mp

#define INNERMUL                                          \
asm(                                                      \
   "movq %5,%%rax \n\t"                                   \
   "mulq %4       \n\t"                                   \
   "addq %1,%%rax \n\t"                                   \
   "adcq $0,%%rdx \n\t"                                   \
   "addq %%rax,%0 \n\t"                                   \
   "adcq $0,%%rdx \n\t"                                   \
   "movq %%rdx,%1 \n\t"                                   \
:"=g"(_c[LO]), "=r"(cy)                                   \
:"0"(_c[LO]), "1"(cy), "r"(mu), "r"(*tmpm++)              \
: "%rax", "%rdx", "cc")

#define INNERMUL8 \
 asm(                  \
 "movq 0(%5),%%rax    \n\t"  \
 "movq 0(%2),%%r10    \n\t"  \
 "movq 0x8(%5),%%r11  \n\t"  \
 "mulq %4             \n\t"  \
 "addq %%r10,%%rax    \n\t"  \
 "adcq $0,%%rdx       \n\t"  \
 "movq 0x8(%2),%%r10  \n\t"  \
 "addq %3,%%rax       \n\t"  \
 "adcq $0,%%rdx       \n\t"  \
 "movq %%rax,0(%0)    \n\t"  \
 "movq %%rdx,%1       \n\t"  \
 \
 "movq %%r11,%%rax    \n\t"  \
 "movq 0x10(%5),%%r11 \n\t"  \
 "mulq %4             \n\t"  \
 "addq %%r10,%%rax    \n\t"  \
 "adcq $0,%%rdx       \n\t"  \
 "movq 0x10(%2),%%r10 \n\t"  \
 "addq %3,%%rax       \n\t"  \
 "adcq $0,%%rdx       \n\t"  \
 "movq %%rax,0x8(%0)  \n\t"  \
 "movq %%rdx,%1       \n\t"  \
 \
 "movq %%r11,%%rax    \n\t"  \
 "movq 0x18(%5),%%r11 \n\t"  \
 "mulq %4             \n\t"  \
 "addq %%r10,%%rax    \n\t"  \
 "adcq $0,%%rdx       \n\t"  \
 "movq 0x18(%2),%%r10 \n\t"  \
 "addq %3,%%rax       \n\t"  \
 "adcq $0,%%rdx       \n\t"  \
 "movq %%rax,0x10(%0) \n\t"  \
 "movq %%rdx,%1       \n\t"  \
 \
 "movq %%r11,%%rax    \n\t"  \
 "movq 0x20(%5),%%r11 \n\t"  \
 "mulq %4             \n\t"  \
 "addq %%r10,%%rax    \n\t"  \
 "adcq $0,%%rdx       \n\t"  \
 "movq 0x20(%2),%%r10 \n\t"  \
 "addq %3,%%rax       \n\t"  \
 "adcq $0,%%rdx       \n\t"  \
 "movq %%rax,0x18(%0) \n\t"  \
 "movq %%rdx,%1       \n\t"  \
 \
 "movq %%r11,%%rax    \n\t"  \
 "movq 0x28(%5),%%r11 \n\t"  \
 "mulq %4             \n\t"  \
 "addq %%r10,%%rax    \n\t"  \
 "adcq $0,%%rdx       \n\t"  \
 "movq 0x28(%2),%%r10 \n\t"  \
 "addq %3,%%rax       \n\t"  \
 "adcq $0,%%rdx       \n\t"  \
 "movq %%rax,0x20(%0) \n\t"  \
 "movq %%rdx,%1       \n\t"  \
 \
 "movq %%r11,%%rax    \n\t"  \
 "movq 0x30(%5),%%r11 \n\t"  \
 "mulq %4             \n\t"  \
 "addq %%r10,%%rax    \n\t"  \
 "adcq $0,%%rdx       \n\t"  \
 "movq 0x30(%2),%%r10 \n\t"  \
 "addq %3,%%rax       \n\t"  \
 "adcq $0,%%rdx       \n\t"  \
 "movq %%rax,0x28(%0) \n\t"  \
 "movq %%rdx,%1       \n\t"  \
 \
 "movq %%r11,%%rax    \n\t"  \
 "movq 0x38(%5),%%r11 \n\t"  \
 "mulq %4             \n\t"  \
 "addq %%r10,%%rax    \n\t"  \
 "adcq $0,%%rdx       \n\t"  \
 "movq 0x38(%2),%%r10 \n\t"  \
 "addq %3,%%rax       \n\t"  \
 "adcq $0,%%rdx       \n\t"  \
 "movq %%rax,0x30(%0) \n\t"  \
 "movq %%rdx,%1       \n\t"  \
 \
 "movq %%r11,%%rax    \n\t"  \
 "mulq %4             \n\t"  \
 "addq %%r10,%%rax    \n\t"  \
 "adcq $0,%%rdx       \n\t"  \
 "addq %3,%%rax       \n\t"  \
 "adcq $0,%%rdx       \n\t"  \
 "movq %%rax,0x38(%0) \n\t"  \
 "movq %%rdx,%1       \n\t"  \
 \
:"=r"(_c), "=r"(cy)                    \
: "0"(_c),  "1"(cy), "g"(mu), "r"(tmpm)\
: "%rax", "%rdx", "%r10", "%r11", "cc")


#define PROPCARRY                           \
asm(                                        \
   "addq   %1,%0    \n\t"                   \
   "setb   %%al     \n\t"                   \
   "movzbq %%al,%1 \n\t"                    \
:"=g"(_c[LO]), "=r"(cy)                     \
:"0"(_c[LO]), "1"(cy)                       \
: "%rax", "cc")

// ****************************************************************
#elif defined(TFM_SSE2)
// SSE2 code (assumes 32-bit fp_digits)
// XMM register assignments:
 * xmm0  *tmpm++, then Mu * (*tmpm++)
 * xmm1  c[x], then Mu
 * xmm2  mp
 * xmm3  cy
 * xmm4  _c[LO]


#define MONT_START \
   asm("movd %0,%%mm2"::"g"(mp))

#define MONT_FINI \
   asm("emms")

#define LOOP_START          \
asm(                        \
"movd %0,%%mm1        \n\t" \
"pxor %%mm3,%%mm3     \n\t" \
"pmuludq %%mm2,%%mm1  \n\t" \
:: "g"(c[x]))

// pmuludq on mmx registers does a 32x32->64 multiply.
#define INNERMUL               \
asm(                           \
   "movd %1,%%mm4        \n\t" \
   "movd %2,%%mm0        \n\t" \
   "paddq %%mm4,%%mm3    \n\t" \
   "pmuludq %%mm1,%%mm0  \n\t" \
   "paddq %%mm0,%%mm3    \n\t" \
   "movd %%mm3,%0        \n\t" \
   "psrlq $32, %%mm3     \n\t" \
:"=g"(_c[LO]) : "0"(_c[LO]), "g"(*tmpm++) );

#define INNERMUL8 \
asm(                           \
   "movd 0(%1),%%mm4     \n\t" \
   "movd 0(%2),%%mm0     \n\t" \
   "paddq %%mm4,%%mm3    \n\t" \
   "pmuludq %%mm1,%%mm0  \n\t" \
   "movd 4(%2),%%mm5     \n\t" \
   "paddq %%mm0,%%mm3    \n\t" \
   "movd 4(%1),%%mm6     \n\t" \
   "movd %%mm3,0(%0)     \n\t" \
   "psrlq $32, %%mm3     \n\t" \
\
   "paddq %%mm6,%%mm3    \n\t" \
   "pmuludq %%mm1,%%mm5  \n\t" \
   "movd 8(%2),%%mm6     \n\t" \
   "paddq %%mm5,%%mm3    \n\t" \
   "movd 8(%1),%%mm7     \n\t" \
   "movd %%mm3,4(%0)     \n\t" \
   "psrlq $32, %%mm3     \n\t" \
\
   "paddq %%mm7,%%mm3    \n\t" \
   "pmuludq %%mm1,%%mm6  \n\t" \
   "movd 12(%2),%%mm7    \n\t" \
   "paddq %%mm6,%%mm3    \n\t" \
   "movd 12(%1),%%mm5     \n\t" \
   "movd %%mm3,8(%0)     \n\t" \
   "psrlq $32, %%mm3     \n\t" \
\
   "paddq %%mm5,%%mm3    \n\t" \
   "pmuludq %%mm1,%%mm7  \n\t" \
   "movd 16(%2),%%mm5    \n\t" \
   "paddq %%mm7,%%mm3    \n\t" \
   "movd 16(%1),%%mm6    \n\t" \
   "movd %%mm3,12(%0)    \n\t" \
   "psrlq $32, %%mm3     \n\t" \
\
   "paddq %%mm6,%%mm3    \n\t" \
   "pmuludq %%mm1,%%mm5  \n\t" \
   "movd 20(%2),%%mm6    \n\t" \
   "paddq %%mm5,%%mm3    \n\t" \
   "movd 20(%1),%%mm7    \n\t" \
   "movd %%mm3,16(%0)    \n\t" \
   "psrlq $32, %%mm3     \n\t" \
\
   "paddq %%mm7,%%mm3    \n\t" \
   "pmuludq %%mm1,%%mm6  \n\t" \
   "movd 24(%2),%%mm7    \n\t" \
   "paddq %%mm6,%%mm3    \n\t" \
   "movd 24(%1),%%mm5     \n\t" \
   "movd %%mm3,20(%0)    \n\t" \
   "psrlq $32, %%mm3     \n\t" \
\
   "paddq %%mm5,%%mm3    \n\t" \
   "pmuludq %%mm1,%%mm7  \n\t" \
   "movd 28(%2),%%mm5    \n\t" \
   "paddq %%mm7,%%mm3    \n\t" \
   "movd 28(%1),%%mm6    \n\t" \
   "movd %%mm3,24(%0)    \n\t" \
   "psrlq $32, %%mm3     \n\t" \
\
   "paddq %%mm6,%%mm3    \n\t" \
   "pmuludq %%mm1,%%mm5  \n\t" \
   "paddq %%mm5,%%mm3    \n\t" \
   "movd %%mm3,28(%0)    \n\t" \
   "psrlq $32, %%mm3     \n\t" \
:"=r"(_c) : "0"(_c), "g"(tmpm) );

#define LOOP_END \
asm( "movd %%mm3,%0  \n" :"=r"(cy))

#define PROPCARRY                           \
asm(                                        \
   "addl   %1,%0    \n\t"                   \
   "setb   %%al     \n\t"                   \
   "movzbl %%al,%1 \n\t"                    \
:"=g"(_c[LO]), "=r"(cy)                     \
:"0"(_c[LO]), "1"(cy)                       \
: "%eax", "cc")

// ****************************************************************
#elif defined(TFM_ARM)
   // ARMv4 code

#define MONT_START
#define MONT_FINI
#define LOOP_END
#define LOOP_START \
   mu = c[x] * mp

#define INNERMUL                    \
asm(                                \
    " LDR    r0,%1            \n\t" \
    " ADDS   r0,r0,%0         \n\t" \
    " MOVCS  %0,#1            \n\t" \
    " MOVCC  %0,#0            \n\t" \
    " UMLAL  r0,%0,%3,%4      \n\t" \
    " STR    r0,%1            \n\t" \
:"=r"(cy),"=m"(_c[0]):"0"(cy),"r"(mu),"r"(*tmpm++),"1"(_c[0]):"r0","cc");

#define PROPCARRY                  \
asm(                               \
    " LDR   r0,%1            \n\t" \
    " ADDS  r0,r0,%0         \n\t" \
    " STR   r0,%1            \n\t" \
    " MOVCS %0,#1            \n\t" \
    " MOVCC %0,#0            \n\t" \
:"=r"(cy),"=m"(_c[0]):"0"(cy),"1"(_c[0]):"r0","cc");

// ****************************************************************
#elif defined(TFM_PPC32)

// PPC32
#define MONT_START
#define MONT_FINI
#define LOOP_END
#define LOOP_START \
   mu = c[x] * mp

#define INNERMUL                     \
asm(                                 \
   " mullw    16,%3,%4       \n\t"   \
   " mulhwu   17,%3,%4       \n\t"   \
   " addc     16,16,%2       \n\t"   \
   " addze    17,17          \n\t"   \
   " addc     %1,16,%5       \n\t"   \
   " addze    %0,17          \n\t"   \
:"=r"(cy),"=r"(_c[0]):"0"(cy),"r"(mu),"r"(tmpm[0]),"1"(_c[0]):"16", "17", "cc"); ++tmpm;

#define PROPCARRY                    \
asm(                                 \
   " addc     %1,%3,%2      \n\t"    \
   " xor      %0,%2,%2      \n\t"    \
   " addze    %0,%2         \n\t"    \
:"=r"(cy),"=r"(_c[0]):"0"(cy),"1"(_c[0]):"cc");

// ****************************************************************
#elif defined(TFM_PPC64)

// PPC64
#define MONT_START
#define MONT_FINI
#define LOOP_END
#define LOOP_START \
   mu = c[x] * mp

#define INNERMUL                     \
asm(                                 \
   " mulld    r16,%3,%4       \n\t"   \
   " mulhdu   r17,%3,%4       \n\t"   \
   " addc     r16,16,%0       \n\t"   \
   " addze    r17,r17          \n\t"   \
   " ldx      r18,0,%1        \n\t"   \
   " addc     r16,r16,r18       \n\t"   \
   " addze    %0,r17          \n\t"   \
   " sdx      r16,0,%1        \n\t"   \
:"=r"(cy),"=m"(_c[0]):"0"(cy),"r"(mu),"r"(tmpm[0]),"1"(_c[0]):"r16", "r17", "r18","cc"); ++tmpm;

#define PROPCARRY                    \
asm(                                 \
   " ldx      r16,0,%1       \n\t"    \
   " addc     r16,r16,%0      \n\t"    \
   " sdx      r16,0,%1       \n\t"    \
   " xor      %0,%0,%0      \n\t"    \
   " addze    %0,%0         \n\t"    \
:"=r"(cy),"=m"(_c[0]):"0"(cy),"1"(_c[0]):"r16","cc");

// ****************************************************************
#elif defined(TFM_AVR32)

// AVR32
#define MONT_START
#define MONT_FINI
#define LOOP_END
#define LOOP_START \
   mu = c[x] * mp

#define INNERMUL                    \
asm(                                \
    " ld.w   r2,%1            \n\t" \
    " add    r2,%0            \n\t" \
    " eor    r3,r3            \n\t" \
    " acr    r3               \n\t" \
    " macu.d r2,%3,%4         \n\t" \
    " st.w   %1,r2            \n\t" \
    " mov    %0,r3            \n\t" \
:"=r"(cy),"=r"(_c):"0"(cy),"r"(mu),"r"(*tmpm++),"1"(_c):"r2","r3");

#define PROPCARRY                    \
asm(                                 \
   " ld.w     r2,%1         \n\t"    \
   " add      r2,%0         \n\t"    \
   " st.w     %1,r2         \n\t"    \
   " eor      %0,%0         \n\t"    \
   " acr      %0            \n\t"    \
:"=r"(cy),"=r"(&_c[0]):"0"(cy),"1"(&_c[0]):"r2","cc");

// ****************************************************************
#elif defined(TFM_MIPS)

// MIPS
#define MONT_START
#define MONT_FINI
#define LOOP_END
#define LOOP_START \
   mu = c[x] * mp

#define INNERMUL                     \
asm(                                 \
   " multu    %3,%4          \n\t"   \
   " mflo     $12            \n\t"   \
   " mfhi     $13            \n\t"   \
   " addu     $12,$12,%0     \n\t"   \
   " sltu     $10,$12,%0     \n\t"   \
   " addu     $13,$13,$10    \n\t"   \
   " lw       $10,%1         \n\t"   \
   " addu     $12,$12,$10    \n\t"   \
   " sltu     $10,$12,$10    \n\t"   \
   " addu     %0,$13,$10     \n\t"   \
   " sw       $12,%1         \n\t"   \
:"=r"(cy),"=m"(_c[0]):"0"(cy),"r"(mu),"r"(tmpm[0]),"1"(_c[0]):"$10","$12","$13"); ++tmpm;

#define PROPCARRY                    \
asm(                                 \
   " lw       $10,%1        \n\t"    \
   " addu     $10,$10,%0    \n\t"    \
   " sw       $10,%1        \n\t"    \
   " sltu     %0,$10,%0     \n\t"    \
:"=r"(cy),"=m"(_c[0]):"0"(cy),"1"(_c[0]):"$10");

// ****************************************************************
#else

// ISO C code
#define MONT_START
#define MONT_FINI
#define LOOP_END
#define LOOP_START \
   mu = c[x] * mp

#define INNERMUL                                      \
   do { fp_word t;                                    \
   _c[0] = t  = ((fp_word)_c[0] + (fp_word)cy) +      \
                (((fp_word)mu) * ((fp_word)*tmpm++)); \
   cy = (t >> DIGIT_BIT);                             \
   } while (0)

#define PROPCARRY \
   do { fp_digit t = _c[0] += cy; cy = (t < cy); } while (0)

#endif
// ****************************************************************


#define LO  0

#ifdef TFM_SMALL_MONT_SET
#include "fp_mont_small.i"
#endif

// computes x/R == x (mod N) via Montgomery Reduction
void fp_montgomery_reduce(fp_int *a, fp_int *m, fp_digit mp) {
   fp_digit c[FP_SIZE], *_c, *tmpm, mu;
   int      oldused, x, y, pa;

   // bail if too large
   if (m->used > (FP_SIZE/2)) {
      return;
   }

#ifdef TFM_SMALL_MONT_SET
   if (m->used <= 16) {
      fp_montgomery_reduce_small(a, m, mp);
      return;
   }
#endif

#if defined(USE_MEMSET)
   // now zero the buff
   memset(c, 0, sizeof c);
#endif
   pa = m->used;

   // copy the input
   oldused = a->used;
   for (x = 0; x < oldused; x++) {
       c[x] = a->dp[x];
   }
#if !defined(USE_MEMSET)
   for (; x < 2*pa+1; x++) {
       c[x] = 0;
   }
#endif
   MONT_START;

   for (x = 0; x < pa; x++) {
       fp_digit cy = 0;
       // get Mu for this round
       LOOP_START;
       _c   = c + x;
       tmpm = m->dp;
       y = 0;
       #if defined(INNERMUL8)
        for (; y < (pa & ~7); y += 8) {
              INNERMUL8;
              _c   += 8;
              tmpm += 8;
           }
       #endif

       for (; y < pa; y++) {
          INNERMUL;
          ++_c;
       }
       LOOP_END;
       while (cy) {
           PROPCARRY;
           ++_c;
       }
  }

  // now copy out
  _c   = c + pa;
  tmpm = a->dp;
  for (x = 0; x < pa+1; x++) {
     *tmpm++ = *_c++;
  }

  for (; x < oldused; x++)   {
     *tmpm++ = 0;
  }

  MONT_FINI;

  a->used = pa+1;
  fp_clamp(a);

  // if A >= m then A = A - m
  if (fp_cmp_mag (a, m) != FP_LT) {
    s_fp_sub (a, m, a);
  }
}

// setups the montgomery reduction
int fp_montgomery_setup(fp_int *a, fp_digit *rho) {
  fp_digit x, b;

// fast inversion mod 2**k
// Based on the fact that
// XA = 1 (mod 2**n)  =>  (X(2-XA)) A = 1 (mod 2**2n)
//                    =>  2*X*A - X*X*A*A = 1
//                    =>  2*(1) - (1)     = 1

  b = a->dp[0];

  if ((b & 1) == 0) {
    return FP_VAL;
  }

  x = (((b + 2) & 4) << 1) + b; // here x*a==1 mod 2**4
  x *= 2 - b * x;               // here x*a==1 mod 2**8
  x *= 2 - b * x;               // here x*a==1 mod 2**16
  x *= 2 - b * x;               // here x*a==1 mod 2**32
#ifdef FP_64BIT
  x *= 2 - b * x;               // here x*a==1 mod 2**64
#endif

  // rho = -1/m mod b
  *rho = (((fp_word) 1 << ((fp_word) DIGIT_BIT)) - ((fp_word)x));

  return FP_OKAY;
}

void fp_mul_2(fp_int * a, fp_int * b) {
  int     x, oldused;

  oldused = b->used;
  b->used = a->used;

  {
    register fp_digit r, rr, *tmpa, *tmpb;

    // alias for source
    tmpa = a->dp;

    // alias for dest
    tmpb = b->dp;

    // carry
    r = 0;
    for (x = 0; x < a->used; x++) {

      // get what will be the *next* carry bit from the MSB of the current digit
      rr = *tmpa >> ((fp_digit)(DIGIT_BIT - 1));

      // now shift up this digit, add in the carry [from the previous]
      *tmpb++ = ((*tmpa++ << ((fp_digit)1)) | r);

      // copy the carry that would be from the source digit into the next iteration
      r = rr;
    }

    // new leading digit?
    if (r != 0 && b->used != (FP_SIZE-1)) {
      // add a MSB which is always 1 at this point
      *tmpb = 1;
      ++(b->used);
    }

    // now zero any excess digits on the destination that we didn't write to
    tmpb = b->dp + b->used;
    for (x = b->used; x < oldused; x++) {
      *tmpb++ = 0;
    }
  }
  b->sign = a->sign;
}
*/
// c = a * 2**d
void fp_mul_2d(fp_int *a, int b, fp_int *c) {
   fp_digit carry, carrytmp, shift;
   int x;

   // copy it
   fp_copy(a, c);

   // handle whole digits
   if (b >= DIGIT_BIT) {
      fp_lshd(c, b/DIGIT_BIT);
   }
   b %= DIGIT_BIT;

   // shift the digits
   if (b != 0) {
      carry = 0;
      shift = DIGIT_BIT - b;
      for (x = 0; x < c->used; x++) {
          carrytmp = c->dp[x] >> shift;
          c->dp[x] = (c->dp[x] << b) + carry;
          carry = carrytmp;
      }
      // store last carry if room
      if (carry && x < FP_SIZE) {
         c->dp[c->used++] = carry;
      }
   }
   fp_clamp(c);
}

// c = a * b
void fp_mul(fp_int *A, fp_int *B, fp_int *C) {
    int   y, old_used;

    old_used = C->used;

    // call generic if we're out of range
    if (A->used + B->used > FP_SIZE) {
       fp_mul_comba(A, B, C);
       goto clean;
    }

     y  = MAX(A->used, B->used);
     fp_mul_comba(A,B,C);
clean:
    for (y = C->used; y < old_used; y++) {
       C->dp[y] = 0;
    }
}

#if defined(TFM_PRESCOTT) && defined(TFM_SSE2)
   #undef TFM_SSE2
   #define TFM_X86
#endif

// these are the combas.  Worship them.
#if defined(TFM_X86)
// Generic x86 optimized code

// anything you need at the start
#define COMBA_START

// clear the chaining variables
#define COMBA_CLEAR \
   c0 = c1 = c2 = 0;

// forward the carry to the next digit
#define COMBA_FORWARD \
   do { c0 = c1; c1 = c2; c2 = 0; } while (0);

// store the first sum
#define COMBA_STORE(x) \
   x = c0;

// store the second sum [carry]
#define COMBA_STORE2(x) \
   x = c1;

// anything you need at the end
#define COMBA_FINI

// this should multiply i and j
#define MULADD(i, j)                                      \
asm(                                                      \
     "movl  %6,%%eax     \n\t"                            \
     "mull  %7           \n\t"                            \
     "addl  %%eax,%0     \n\t"                            \
     "adcl  %%edx,%1     \n\t"                            \
     "adcl  $0,%2        \n\t"                            \
     :"=r"(c0), "=r"(c1), "=r"(c2): "0"(c0), "1"(c1), "2"(c2), "m"(i), "m"(j)  :"%eax","%edx","cc");

#elif defined(TFM_X86_64)
// x86-64 optimized

// anything you need at the start
#define COMBA_START

// clear the chaining variables
#define COMBA_CLEAR \
   c0 = c1 = c2 = 0;

// forward the carry to the next digit
#define COMBA_FORWARD \
   do { c0 = c1; c1 = c2; c2 = 0; } while (0);

// store the first sum
#define COMBA_STORE(x) \
   x = c0;

// store the second sum [carry]
#define COMBA_STORE2(x) \
   x = c1;

// anything you need at the end
#define COMBA_FINI

// this should multiply i and j
#define MULADD(i, j)                                      \
asm  (                                                    \
     "movq  %6,%%rax     \n\t"                            \
     "mulq  %7           \n\t"                            \
     "addq  %%rax,%0     \n\t"                            \
     "adcq  %%rdx,%1     \n\t"                            \
     "adcq  $0,%2        \n\t"                            \
     :"=r"(c0), "=r"(c1), "=r"(c2): "0"(c0), "1"(c1), "2"(c2), "g"(i), "g"(j)  :"%rax","%rdx","cc");

#elif defined(TFM_SSE2)
// use SSE2 optimizations

// anything you need at the start
#define COMBA_START

// clear the chaining variables
#define COMBA_CLEAR \
   c0 = c1 = c2 = 0;

// forward the carry to the next digit
#define COMBA_FORWARD \
   do { c0 = c1; c1 = c2; c2 = 0; } while (0);

// store the first sum
#define COMBA_STORE(x) \
   x = c0;

// store the second sum [carry]
#define COMBA_STORE2(x) \
   x = c1;

// anything you need at the end
#define COMBA_FINI \
   asm("emms");

// this should multiply i and j
#define MULADD(i, j)                                     \
asm(                                                     \
    "movd  %6,%%mm0     \n\t"                            \
    "movd  %7,%%mm1     \n\t"                            \
    "pmuludq %%mm1,%%mm0\n\t"                            \
    "movd  %%mm0,%%eax  \n\t"                            \
    "psrlq $32,%%mm0    \n\t"                            \
    "addl  %%eax,%0     \n\t"                            \
    "movd  %%mm0,%%eax  \n\t"                            \
    "adcl  %%eax,%1     \n\t"                            \
    "adcl  $0,%2        \n\t"                            \
    :"=r"(c0), "=r"(c1), "=r"(c2): "0"(c0), "1"(c1), "2"(c2), "m"(i), "m"(j)  :"%eax","cc");

#elif defined(TFM_ARM)
// ARM code

#define COMBA_START

#define COMBA_CLEAR \
   c0 = c1 = c2 = 0;

#define COMBA_FORWARD \
   do { c0 = c1; c1 = c2; c2 = 0; } while (0);

#define COMBA_STORE(x) \
   x = c0;

#define COMBA_STORE2(x) \
   x = c1;

#define COMBA_FINI

#define MULADD(i, j)                                          \
asm(                                                          \
"  UMULL  r0,r1,%6,%7           \n\t"                         \
"  ADDS   %0,%0,r0              \n\t"                         \
"  ADCS   %1,%1,r1              \n\t"                         \
"  ADC    %2,%2,#0              \n\t"                         \
:"=r"(c0), "=r"(c1), "=r"(c2) : "0"(c0), "1"(c1), "2"(c2), "r"(i), "r"(j) : "r0", "r1", "cc");

#elif defined(TFM_PPC32)
// For 32-bit PPC

#define COMBA_START

#define COMBA_CLEAR \
   c0 = c1 = c2 = 0;

#define COMBA_FORWARD \
   do { c0 = c1; c1 = c2; c2 = 0; } while (0);

#define COMBA_STORE(x) \
   x = c0;

#define COMBA_STORE2(x) \
   x = c1;

#define COMBA_FINI

// untested: will mulhwu change the flags?  Docs say no
#define MULADD(i, j)              \
asm(                              \
   " mullw  16,%6,%7       \n\t" \
   " addc   %0,%0,16       \n\t" \
   " mulhwu 16,%6,%7       \n\t" \
   " adde   %1,%1,16       \n\t" \
   " addze  %2,%2          \n\t" \
:"=r"(c0), "=r"(c1), "=r"(c2):"0"(c0), "1"(c1), "2"(c2), "r"(i), "r"(j):"16");

#elif defined(TFM_PPC64)
// For 64-bit PPC

#define COMBA_START

#define COMBA_CLEAR \
   c0 = c1 = c2 = 0;

#define COMBA_FORWARD \
   do { c0 = c1; c1 = c2; c2 = 0; } while (0);

#define COMBA_STORE(x) \
   x = c0;

#define COMBA_STORE2(x) \
   x = c1;

#define COMBA_FINI

// untested: will mulhdu change the flags?  Docs say no
#define MULADD(i, j)              \
asm(                              \
   " mulld  r16,%6,%7       \n\t" \
   " addc   %0,%0,16       \n\t" \
   " mulhdu r16,%6,%7       \n\t" \
   " adde   %1,%1,16       \n\t" \
   " addze  %2,%2          \n\t" \
:"=r"(c0), "=r"(c1), "=r"(c2):"0"(c0), "1"(c1), "2"(c2), "r"(i), "r"(j):"r16");

#elif defined(TFM_AVR32)

// ISO C code

#define COMBA_START

#define COMBA_CLEAR \
   c0 = c1 = c2 = 0;

#define COMBA_FORWARD \
   do { c0 = c1; c1 = c2; c2 = 0; } while (0);

#define COMBA_STORE(x) \
   x = c0;

#define COMBA_STORE2(x) \
   x = c1;

#define COMBA_FINI

#define MULADD(i, j)             \
asm(                             \
   " mulu.d r2,%6,%7        \n\t"\
   " add    %0,r2           \n\t"\
   " adc    %1,%1,r3        \n\t"\
   " acr    %2              \n\t"\
:"=r"(c0), "=r"(c1), "=r"(c2):"0"(c0), "1"(c1), "2"(c2), "r"(i), "r"(j):"r2","r3");

#elif defined(TFM_MIPS)

#define COMBA_START

#define COMBA_CLEAR \
   c0 = c1 = c2 = 0;

#define COMBA_FORWARD \
   do { c0 = c1; c1 = c2; c2 = 0; } while (0);

#define COMBA_STORE(x) \
   x = c0;

#define COMBA_STORE2(x) \
   x = c1;

#define COMBA_FINI

#define MULADD(i, j)              \
asm(                              \
   " multu  %6,%7          \n\t"  \
   " mflo   $12            \n\t"  \
   " mfhi   $13            \n\t"  \
   " addu    %0,%0,$12     \n\t"  \
   " sltu   $12,%0,$12     \n\t"  \
   " addu    %1,%1,$13     \n\t"  \
   " sltu   $13,%1,$13     \n\t"  \
   " addu    %1,%1,$12     \n\t"  \
   " sltu   $12,%1,$12     \n\t"  \
   " addu    %2,%2,$13     \n\t"  \
   " addu    %2,%2,$12     \n\t"  \
:"=r"(c0), "=r"(c1), "=r"(c2):"0"(c0), "1"(c1), "2"(c2), "r"(i), "r"(j):"$12","$13");

#else
// ISO C code

#define COMBA_START

#define COMBA_CLEAR \
   c0 = c1 = c2 = 0;

#define COMBA_FORWARD \
   do { c0 = c1; c1 = c2; c2 = 0; } while (0);

#define COMBA_STORE(x) \
   x = c0;

#define COMBA_STORE2(x) \
   x = c1;

#define COMBA_FINI

#define MULADD(i, j)                                    \
   do { fp_word t;                                      \
   t = (fp_word)c0 + ((fp_word)i) * ((fp_word)j);       \
   c0 = t;                                              \
   t = (fp_word)c1 + (t >> DIGIT_BIT);                  \
   c1 = t;                                              \
   c2 += t >> DIGIT_BIT;                                \
   } while (0);

#endif

#ifndef TFM_DEFINES

// generic PxQ multiplier
void fp_mul_comba(fp_int *A, fp_int *B, fp_int *C) {
   int       ix, iy, iz, tx, ty, pa;
   fp_digit  c0, c1, c2, *tmpx, *tmpy;
   fp_int    tmp, *dst;

   COMBA_START;
   COMBA_CLEAR;

   // get size of output and trim
   pa = A->used + B->used;
   if (pa >= FP_SIZE) {
      pa = FP_SIZE-1;
   }

   if (A == C || B == C) {
      fp_zero(&tmp);
      dst = &tmp;
   } else {
      fp_zero(C);
      dst = C;
   }

   for (ix = 0; ix < pa; ix++) {
      // get offsets into the two bignums
      ty = MIN(ix, B->used-1);
      tx = ix - ty;

      // setup temp aliases
      tmpx = A->dp + tx;
      tmpy = B->dp + ty;

      // this is the number of times the loop will iterrate, essentially its
      // while (tx++ < a->used && ty-- >= 0) { ... }

      iy = MIN(A->used-tx, ty+1);

      // execute loop
      COMBA_FORWARD;
      for (iz = 0; iz < iy; ++iz) {
          fp_digit _tmpx = *tmpx++;
          fp_digit _tmpy = *tmpy--;
          MULADD(_tmpx, _tmpy);
      }

      // store term
      COMBA_STORE(dst->dp[ix]);
  }
  COMBA_FINI;

  dst->used = pa;
  dst->sign = A->sign ^ B->sign;
  fp_clamp(dst);
  fp_copy(dst, C);
}

#endif
/*
#define TFM_DEFINES

void fp_mul_comba_small(fp_int *A, fp_int *B, fp_int *C) {
   fp_digit c0, c1, c2, at[32];
   switch (MAX(A->used, B->used)) {

   case 1:
      memcpy(at, A->dp, 1 * sizeof(fp_digit));
      memcpy(at+1, B->dp, 1 * sizeof(fp_digit));
      COMBA_START;

      COMBA_CLEAR;
      // 0
      MULADD(at[0], at[1]);
      COMBA_STORE(C->dp[0]);
      COMBA_STORE2(C->dp[1]);
      C->used = 2;
      C->sign = A->sign ^ B->sign;
      fp_clamp(C);
      COMBA_FINI;
      break;

   case 2:
      memcpy(at, A->dp, 2 * sizeof(fp_digit));
      memcpy(at+2, B->dp, 2 * sizeof(fp_digit));
      COMBA_START;

      COMBA_CLEAR;
      // 0
      MULADD(at[0], at[2]);
      COMBA_STORE(C->dp[0]);
      // 1
      COMBA_FORWARD;
      MULADD(at[0], at[3]);       MULADD(at[1], at[2]);
      COMBA_STORE(C->dp[1]);
      // 2
      COMBA_FORWARD;
      MULADD(at[1], at[3]);
      COMBA_STORE(C->dp[2]);
      COMBA_STORE2(C->dp[3]);
      C->used = 4;
      C->sign = A->sign ^ B->sign;
      fp_clamp(C);
      COMBA_FINI;
      break;

   case 3:
      memcpy(at, A->dp, 3 * sizeof(fp_digit));
      memcpy(at+3, B->dp, 3 * sizeof(fp_digit));
      COMBA_START;

      COMBA_CLEAR;
      // 0
      MULADD(at[0], at[3]);
      COMBA_STORE(C->dp[0]);
      // 1
      COMBA_FORWARD;
      MULADD(at[0], at[4]);       MULADD(at[1], at[3]);
      COMBA_STORE(C->dp[1]);
      // 2
      COMBA_FORWARD;
      MULADD(at[0], at[5]);       MULADD(at[1], at[4]);       MULADD(at[2], at[3]);
      COMBA_STORE(C->dp[2]);
      // 3
      COMBA_FORWARD;
      MULADD(at[1], at[5]);       MULADD(at[2], at[4]);
      COMBA_STORE(C->dp[3]);
      // 4
      COMBA_FORWARD;
      MULADD(at[2], at[5]);
      COMBA_STORE(C->dp[4]);
      COMBA_STORE2(C->dp[5]);
      C->used = 6;
      C->sign = A->sign ^ B->sign;
      fp_clamp(C);
      COMBA_FINI;
      break;

   case 4:
      memcpy(at, A->dp, 4 * sizeof(fp_digit));
      memcpy(at+4, B->dp, 4 * sizeof(fp_digit));
      COMBA_START;

      COMBA_CLEAR;
      // 0
      MULADD(at[0], at[4]);
      COMBA_STORE(C->dp[0]);
      // 1
      COMBA_FORWARD;
      MULADD(at[0], at[5]);       MULADD(at[1], at[4]);
      COMBA_STORE(C->dp[1]);
      // 2
      COMBA_FORWARD;
      MULADD(at[0], at[6]);       MULADD(at[1], at[5]);       MULADD(at[2], at[4]);
      COMBA_STORE(C->dp[2]);
      // 3
      COMBA_FORWARD;
      MULADD(at[0], at[7]);       MULADD(at[1], at[6]);       MULADD(at[2], at[5]);       MULADD(at[3], at[4]);
      COMBA_STORE(C->dp[3]);
      // 4
      COMBA_FORWARD;
      MULADD(at[1], at[7]);       MULADD(at[2], at[6]);       MULADD(at[3], at[5]);
      COMBA_STORE(C->dp[4]);
      // 5
      COMBA_FORWARD;
      MULADD(at[2], at[7]);       MULADD(at[3], at[6]);
      COMBA_STORE(C->dp[5]);
      // 6
      COMBA_FORWARD;
      MULADD(at[3], at[7]);
      COMBA_STORE(C->dp[6]);
      COMBA_STORE2(C->dp[7]);
      C->used = 8;
      C->sign = A->sign ^ B->sign;
      fp_clamp(C);
      COMBA_FINI;
      break;

   case 5:
      memcpy(at, A->dp, 5 * sizeof(fp_digit));
      memcpy(at+5, B->dp, 5 * sizeof(fp_digit));
      COMBA_START;

      COMBA_CLEAR;
      // 0
      MULADD(at[0], at[5]);
      COMBA_STORE(C->dp[0]);
      // 1
      COMBA_FORWARD;
      MULADD(at[0], at[6]);       MULADD(at[1], at[5]);
      COMBA_STORE(C->dp[1]);
      // 2
      COMBA_FORWARD;
      MULADD(at[0], at[7]);       MULADD(at[1], at[6]);       MULADD(at[2], at[5]);
      COMBA_STORE(C->dp[2]);
      // 3
      COMBA_FORWARD;
      MULADD(at[0], at[8]);       MULADD(at[1], at[7]);       MULADD(at[2], at[6]);       MULADD(at[3], at[5]);
      COMBA_STORE(C->dp[3]);
      // 4
      COMBA_FORWARD;
      MULADD(at[0], at[9]);       MULADD(at[1], at[8]);       MULADD(at[2], at[7]);       MULADD(at[3], at[6]);       MULADD(at[4], at[5]);
      COMBA_STORE(C->dp[4]);
      // 5
      COMBA_FORWARD;
      MULADD(at[1], at[9]);       MULADD(at[2], at[8]);       MULADD(at[3], at[7]);       MULADD(at[4], at[6]);
      COMBA_STORE(C->dp[5]);
      // 6
      COMBA_FORWARD;
      MULADD(at[2], at[9]);       MULADD(at[3], at[8]);       MULADD(at[4], at[7]);
      COMBA_STORE(C->dp[6]);
      // 7
      COMBA_FORWARD;
      MULADD(at[3], at[9]);       MULADD(at[4], at[8]);
      COMBA_STORE(C->dp[7]);
      // 8
      COMBA_FORWARD;
      MULADD(at[4], at[9]);
      COMBA_STORE(C->dp[8]);
      COMBA_STORE2(C->dp[9]);
      C->used = 10;
      C->sign = A->sign ^ B->sign;
      fp_clamp(C);
      COMBA_FINI;
      break;

   case 6:
      memcpy(at, A->dp, 6 * sizeof(fp_digit));
      memcpy(at+6, B->dp, 6 * sizeof(fp_digit));
      COMBA_START;

      COMBA_CLEAR;
      // 0
      MULADD(at[0], at[6]);
      COMBA_STORE(C->dp[0]);
      // 1
      COMBA_FORWARD;
      MULADD(at[0], at[7]);       MULADD(at[1], at[6]);
      COMBA_STORE(C->dp[1]);
      // 2
      COMBA_FORWARD;
      MULADD(at[0], at[8]);       MULADD(at[1], at[7]);       MULADD(at[2], at[6]);
      COMBA_STORE(C->dp[2]);
      // 3
      COMBA_FORWARD;
      MULADD(at[0], at[9]);       MULADD(at[1], at[8]);       MULADD(at[2], at[7]);       MULADD(at[3], at[6]);
      COMBA_STORE(C->dp[3]);
      // 4
      COMBA_FORWARD;
      MULADD(at[0], at[10]);       MULADD(at[1], at[9]);       MULADD(at[2], at[8]);       MULADD(at[3], at[7]);       MULADD(at[4], at[6]);
      COMBA_STORE(C->dp[4]);
      // 5
      COMBA_FORWARD;
      MULADD(at[0], at[11]);       MULADD(at[1], at[10]);       MULADD(at[2], at[9]);       MULADD(at[3], at[8]);       MULADD(at[4], at[7]);       MULADD(at[5], at[6]);
      COMBA_STORE(C->dp[5]);
      // 6
      COMBA_FORWARD;
      MULADD(at[1], at[11]);       MULADD(at[2], at[10]);       MULADD(at[3], at[9]);       MULADD(at[4], at[8]);       MULADD(at[5], at[7]);
      COMBA_STORE(C->dp[6]);
      // 7
      COMBA_FORWARD;
      MULADD(at[2], at[11]);       MULADD(at[3], at[10]);       MULADD(at[4], at[9]);       MULADD(at[5], at[8]);
      COMBA_STORE(C->dp[7]);
      // 8
      COMBA_FORWARD;
      MULADD(at[3], at[11]);       MULADD(at[4], at[10]);       MULADD(at[5], at[9]);
      COMBA_STORE(C->dp[8]);
      // 9
      COMBA_FORWARD;
      MULADD(at[4], at[11]);       MULADD(at[5], at[10]);
      COMBA_STORE(C->dp[9]);
      // 10
      COMBA_FORWARD;
      MULADD(at[5], at[11]);
      COMBA_STORE(C->dp[10]);
      COMBA_STORE2(C->dp[11]);
      C->used = 12;
      C->sign = A->sign ^ B->sign;
      fp_clamp(C);
      COMBA_FINI;
      break;

   case 7:
      memcpy(at, A->dp, 7 * sizeof(fp_digit));
      memcpy(at+7, B->dp, 7 * sizeof(fp_digit));
      COMBA_START;

      COMBA_CLEAR;
      // 0
      MULADD(at[0], at[7]);
      COMBA_STORE(C->dp[0]);
      // 1
      COMBA_FORWARD;
      MULADD(at[0], at[8]);       MULADD(at[1], at[7]);
      COMBA_STORE(C->dp[1]);
      // 2
      COMBA_FORWARD;
      MULADD(at[0], at[9]);       MULADD(at[1], at[8]);       MULADD(at[2], at[7]);
      COMBA_STORE(C->dp[2]);
      // 3
      COMBA_FORWARD;
      MULADD(at[0], at[10]);       MULADD(at[1], at[9]);       MULADD(at[2], at[8]);       MULADD(at[3], at[7]);
      COMBA_STORE(C->dp[3]);
      // 4
      COMBA_FORWARD;
      MULADD(at[0], at[11]);       MULADD(at[1], at[10]);       MULADD(at[2], at[9]);       MULADD(at[3], at[8]);       MULADD(at[4], at[7]);
      COMBA_STORE(C->dp[4]);
      // 5
      COMBA_FORWARD;
      MULADD(at[0], at[12]);       MULADD(at[1], at[11]);       MULADD(at[2], at[10]);       MULADD(at[3], at[9]);       MULADD(at[4], at[8]);       MULADD(at[5], at[7]);
      COMBA_STORE(C->dp[5]);
      // 6
      COMBA_FORWARD;
      MULADD(at[0], at[13]);       MULADD(at[1], at[12]);       MULADD(at[2], at[11]);       MULADD(at[3], at[10]);       MULADD(at[4], at[9]);       MULADD(at[5], at[8]);       MULADD(at[6], at[7]);
      COMBA_STORE(C->dp[6]);
      // 7
      COMBA_FORWARD;
      MULADD(at[1], at[13]);       MULADD(at[2], at[12]);       MULADD(at[3], at[11]);       MULADD(at[4], at[10]);       MULADD(at[5], at[9]);       MULADD(at[6], at[8]);
      COMBA_STORE(C->dp[7]);
      // 8
      COMBA_FORWARD;
      MULADD(at[2], at[13]);       MULADD(at[3], at[12]);       MULADD(at[4], at[11]);       MULADD(at[5], at[10]);       MULADD(at[6], at[9]);
      COMBA_STORE(C->dp[8]);
      // 9
      COMBA_FORWARD;
      MULADD(at[3], at[13]);       MULADD(at[4], at[12]);       MULADD(at[5], at[11]);       MULADD(at[6], at[10]);
      COMBA_STORE(C->dp[9]);
      // 10
      COMBA_FORWARD;
      MULADD(at[4], at[13]);       MULADD(at[5], at[12]);       MULADD(at[6], at[11]);
      COMBA_STORE(C->dp[10]);
      // 11
      COMBA_FORWARD;
      MULADD(at[5], at[13]);       MULADD(at[6], at[12]);
      COMBA_STORE(C->dp[11]);
      // 12
      COMBA_FORWARD;
      MULADD(at[6], at[13]);
      COMBA_STORE(C->dp[12]);
      COMBA_STORE2(C->dp[13]);
      C->used = 14;
      C->sign = A->sign ^ B->sign;
      fp_clamp(C);
      COMBA_FINI;
      break;

   case 8:
      memcpy(at, A->dp, 8 * sizeof(fp_digit));
      memcpy(at+8, B->dp, 8 * sizeof(fp_digit));
      COMBA_START;

      COMBA_CLEAR;
      // 0
      MULADD(at[0], at[8]);
      COMBA_STORE(C->dp[0]);
      // 1
      COMBA_FORWARD;
      MULADD(at[0], at[9]);       MULADD(at[1], at[8]);
      COMBA_STORE(C->dp[1]);
      // 2
      COMBA_FORWARD;
      MULADD(at[0], at[10]);       MULADD(at[1], at[9]);       MULADD(at[2], at[8]);
      COMBA_STORE(C->dp[2]);
      // 3
      COMBA_FORWARD;
      MULADD(at[0], at[11]);       MULADD(at[1], at[10]);       MULADD(at[2], at[9]);       MULADD(at[3], at[8]);
      COMBA_STORE(C->dp[3]);
      // 4
      COMBA_FORWARD;
      MULADD(at[0], at[12]);       MULADD(at[1], at[11]);       MULADD(at[2], at[10]);       MULADD(at[3], at[9]);       MULADD(at[4], at[8]);
      COMBA_STORE(C->dp[4]);
      // 5
      COMBA_FORWARD;
      MULADD(at[0], at[13]);       MULADD(at[1], at[12]);       MULADD(at[2], at[11]);       MULADD(at[3], at[10]);       MULADD(at[4], at[9]);       MULADD(at[5], at[8]);
      COMBA_STORE(C->dp[5]);
      // 6
      COMBA_FORWARD;
      MULADD(at[0], at[14]);       MULADD(at[1], at[13]);       MULADD(at[2], at[12]);       MULADD(at[3], at[11]);       MULADD(at[4], at[10]);       MULADD(at[5], at[9]);       MULADD(at[6], at[8]);
      COMBA_STORE(C->dp[6]);
      // 7
      COMBA_FORWARD;
      MULADD(at[0], at[15]);       MULADD(at[1], at[14]);       MULADD(at[2], at[13]);       MULADD(at[3], at[12]);       MULADD(at[4], at[11]);       MULADD(at[5], at[10]);       MULADD(at[6], at[9]);       MULADD(at[7], at[8]);
      COMBA_STORE(C->dp[7]);
      // 8
      COMBA_FORWARD;
      MULADD(at[1], at[15]);       MULADD(at[2], at[14]);       MULADD(at[3], at[13]);       MULADD(at[4], at[12]);       MULADD(at[5], at[11]);       MULADD(at[6], at[10]);       MULADD(at[7], at[9]);
      COMBA_STORE(C->dp[8]);
      // 9
      COMBA_FORWARD;
      MULADD(at[2], at[15]);       MULADD(at[3], at[14]);       MULADD(at[4], at[13]);       MULADD(at[5], at[12]);       MULADD(at[6], at[11]);       MULADD(at[7], at[10]);
      COMBA_STORE(C->dp[9]);
      // 10
      COMBA_FORWARD;
      MULADD(at[3], at[15]);       MULADD(at[4], at[14]);       MULADD(at[5], at[13]);       MULADD(at[6], at[12]);       MULADD(at[7], at[11]);
      COMBA_STORE(C->dp[10]);
      // 11
      COMBA_FORWARD;
      MULADD(at[4], at[15]);       MULADD(at[5], at[14]);       MULADD(at[6], at[13]);       MULADD(at[7], at[12]);
      COMBA_STORE(C->dp[11]);
      // 12
      COMBA_FORWARD;
      MULADD(at[5], at[15]);       MULADD(at[6], at[14]);       MULADD(at[7], at[13]);
      COMBA_STORE(C->dp[12]);
      // 13
      COMBA_FORWARD;
      MULADD(at[6], at[15]);       MULADD(at[7], at[14]);
      COMBA_STORE(C->dp[13]);
      // 14
      COMBA_FORWARD;
      MULADD(at[7], at[15]);
      COMBA_STORE(C->dp[14]);
      COMBA_STORE2(C->dp[15]);
      C->used = 16;
      C->sign = A->sign ^ B->sign;
      fp_clamp(C);
      COMBA_FINI;
      break;

   case 9:
      memcpy(at, A->dp, 9 * sizeof(fp_digit));
      memcpy(at+9, B->dp, 9 * sizeof(fp_digit));
      COMBA_START;

      COMBA_CLEAR;
      // 0
      MULADD(at[0], at[9]);
      COMBA_STORE(C->dp[0]);
      // 1
      COMBA_FORWARD;
      MULADD(at[0], at[10]);       MULADD(at[1], at[9]);
      COMBA_STORE(C->dp[1]);
      // 2
      COMBA_FORWARD;
      MULADD(at[0], at[11]);       MULADD(at[1], at[10]);       MULADD(at[2], at[9]);
      COMBA_STORE(C->dp[2]);
      // 3
      COMBA_FORWARD;
      MULADD(at[0], at[12]);       MULADD(at[1], at[11]);       MULADD(at[2], at[10]);       MULADD(at[3], at[9]);
      COMBA_STORE(C->dp[3]);
      // 4
      COMBA_FORWARD;
      MULADD(at[0], at[13]);       MULADD(at[1], at[12]);       MULADD(at[2], at[11]);       MULADD(at[3], at[10]);       MULADD(at[4], at[9]);
      COMBA_STORE(C->dp[4]);
      // 5
      COMBA_FORWARD;
      MULADD(at[0], at[14]);       MULADD(at[1], at[13]);       MULADD(at[2], at[12]);       MULADD(at[3], at[11]);       MULADD(at[4], at[10]);       MULADD(at[5], at[9]);
      COMBA_STORE(C->dp[5]);
      // 6
      COMBA_FORWARD;
      MULADD(at[0], at[15]);       MULADD(at[1], at[14]);       MULADD(at[2], at[13]);       MULADD(at[3], at[12]);       MULADD(at[4], at[11]);       MULADD(at[5], at[10]);       MULADD(at[6], at[9]);
      COMBA_STORE(C->dp[6]);
      // 7
      COMBA_FORWARD;
      MULADD(at[0], at[16]);       MULADD(at[1], at[15]);       MULADD(at[2], at[14]);       MULADD(at[3], at[13]);       MULADD(at[4], at[12]);       MULADD(at[5], at[11]);       MULADD(at[6], at[10]);       MULADD(at[7], at[9]);
      COMBA_STORE(C->dp[7]);
      // 8
      COMBA_FORWARD;
      MULADD(at[0], at[17]);       MULADD(at[1], at[16]);       MULADD(at[2], at[15]);       MULADD(at[3], at[14]);       MULADD(at[4], at[13]);       MULADD(at[5], at[12]);       MULADD(at[6], at[11]);       MULADD(at[7], at[10]);       MULADD(at[8], at[9]);
      COMBA_STORE(C->dp[8]);
      // 9
      COMBA_FORWARD;
      MULADD(at[1], at[17]);       MULADD(at[2], at[16]);       MULADD(at[3], at[15]);       MULADD(at[4], at[14]);       MULADD(at[5], at[13]);       MULADD(at[6], at[12]);       MULADD(at[7], at[11]);       MULADD(at[8], at[10]);
      COMBA_STORE(C->dp[9]);
      // 10
      COMBA_FORWARD;
      MULADD(at[2], at[17]);       MULADD(at[3], at[16]);       MULADD(at[4], at[15]);       MULADD(at[5], at[14]);       MULADD(at[6], at[13]);       MULADD(at[7], at[12]);       MULADD(at[8], at[11]);
      COMBA_STORE(C->dp[10]);
      // 11
      COMBA_FORWARD;
      MULADD(at[3], at[17]);       MULADD(at[4], at[16]);       MULADD(at[5], at[15]);       MULADD(at[6], at[14]);       MULADD(at[7], at[13]);       MULADD(at[8], at[12]);
      COMBA_STORE(C->dp[11]);
      // 12
      COMBA_FORWARD;
      MULADD(at[4], at[17]);       MULADD(at[5], at[16]);       MULADD(at[6], at[15]);       MULADD(at[7], at[14]);       MULADD(at[8], at[13]);
      COMBA_STORE(C->dp[12]);
      // 13
      COMBA_FORWARD;
      MULADD(at[5], at[17]);       MULADD(at[6], at[16]);       MULADD(at[7], at[15]);       MULADD(at[8], at[14]);
      COMBA_STORE(C->dp[13]);
      // 14
      COMBA_FORWARD;
      MULADD(at[6], at[17]);       MULADD(at[7], at[16]);       MULADD(at[8], at[15]);
      COMBA_STORE(C->dp[14]);
      // 15
      COMBA_FORWARD;
      MULADD(at[7], at[17]);       MULADD(at[8], at[16]);
      COMBA_STORE(C->dp[15]);
      // 16
      COMBA_FORWARD;
      MULADD(at[8], at[17]);
      COMBA_STORE(C->dp[16]);
      COMBA_STORE2(C->dp[17]);
      C->used = 18;
      C->sign = A->sign ^ B->sign;
      fp_clamp(C);
      COMBA_FINI;
      break;

   case 10:
      memcpy(at, A->dp, 10 * sizeof(fp_digit));
      memcpy(at+10, B->dp, 10 * sizeof(fp_digit));
      COMBA_START;

      COMBA_CLEAR;
      // 0
      MULADD(at[0], at[10]);
      COMBA_STORE(C->dp[0]);
      // 1
      COMBA_FORWARD;
      MULADD(at[0], at[11]);       MULADD(at[1], at[10]);
      COMBA_STORE(C->dp[1]);
      // 2
      COMBA_FORWARD;
      MULADD(at[0], at[12]);       MULADD(at[1], at[11]);       MULADD(at[2], at[10]);
      COMBA_STORE(C->dp[2]);
      // 3
      COMBA_FORWARD;
      MULADD(at[0], at[13]);       MULADD(at[1], at[12]);       MULADD(at[2], at[11]);       MULADD(at[3], at[10]);
      COMBA_STORE(C->dp[3]);
      // 4
      COMBA_FORWARD;
      MULADD(at[0], at[14]);       MULADD(at[1], at[13]);       MULADD(at[2], at[12]);       MULADD(at[3], at[11]);       MULADD(at[4], at[10]);
      COMBA_STORE(C->dp[4]);
      // 5
      COMBA_FORWARD;
      MULADD(at[0], at[15]);       MULADD(at[1], at[14]);       MULADD(at[2], at[13]);       MULADD(at[3], at[12]);       MULADD(at[4], at[11]);       MULADD(at[5], at[10]);
      COMBA_STORE(C->dp[5]);
      // 6
      COMBA_FORWARD;
      MULADD(at[0], at[16]);       MULADD(at[1], at[15]);       MULADD(at[2], at[14]);       MULADD(at[3], at[13]);       MULADD(at[4], at[12]);       MULADD(at[5], at[11]);       MULADD(at[6], at[10]);
      COMBA_STORE(C->dp[6]);
      // 7
      COMBA_FORWARD;
      MULADD(at[0], at[17]);       MULADD(at[1], at[16]);       MULADD(at[2], at[15]);       MULADD(at[3], at[14]);       MULADD(at[4], at[13]);       MULADD(at[5], at[12]);       MULADD(at[6], at[11]);       MULADD(at[7], at[10]);
      COMBA_STORE(C->dp[7]);
      // 8
      COMBA_FORWARD;
      MULADD(at[0], at[18]);       MULADD(at[1], at[17]);       MULADD(at[2], at[16]);       MULADD(at[3], at[15]);       MULADD(at[4], at[14]);       MULADD(at[5], at[13]);       MULADD(at[6], at[12]);       MULADD(at[7], at[11]);       MULADD(at[8], at[10]);
      COMBA_STORE(C->dp[8]);
      // 9
      COMBA_FORWARD;
      MULADD(at[0], at[19]);       MULADD(at[1], at[18]);       MULADD(at[2], at[17]);       MULADD(at[3], at[16]);       MULADD(at[4], at[15]);       MULADD(at[5], at[14]);       MULADD(at[6], at[13]);       MULADD(at[7], at[12]);       MULADD(at[8], at[11]);       MULADD(at[9], at[10]);
      COMBA_STORE(C->dp[9]);
      // 10
      COMBA_FORWARD;
      MULADD(at[1], at[19]);       MULADD(at[2], at[18]);       MULADD(at[3], at[17]);       MULADD(at[4], at[16]);       MULADD(at[5], at[15]);       MULADD(at[6], at[14]);       MULADD(at[7], at[13]);       MULADD(at[8], at[12]);       MULADD(at[9], at[11]);
      COMBA_STORE(C->dp[10]);
      // 11
      COMBA_FORWARD;
      MULADD(at[2], at[19]);       MULADD(at[3], at[18]);       MULADD(at[4], at[17]);       MULADD(at[5], at[16]);       MULADD(at[6], at[15]);       MULADD(at[7], at[14]);       MULADD(at[8], at[13]);       MULADD(at[9], at[12]);
      COMBA_STORE(C->dp[11]);
      // 12
      COMBA_FORWARD;
      MULADD(at[3], at[19]);       MULADD(at[4], at[18]);       MULADD(at[5], at[17]);       MULADD(at[6], at[16]);       MULADD(at[7], at[15]);       MULADD(at[8], at[14]);       MULADD(at[9], at[13]);
      COMBA_STORE(C->dp[12]);
      // 13
      COMBA_FORWARD;
      MULADD(at[4], at[19]);       MULADD(at[5], at[18]);       MULADD(at[6], at[17]);       MULADD(at[7], at[16]);       MULADD(at[8], at[15]);       MULADD(at[9], at[14]);
      COMBA_STORE(C->dp[13]);
      // 14
      COMBA_FORWARD;
      MULADD(at[5], at[19]);       MULADD(at[6], at[18]);       MULADD(at[7], at[17]);       MULADD(at[8], at[16]);       MULADD(at[9], at[15]);
      COMBA_STORE(C->dp[14]);
      // 15
      COMBA_FORWARD;
      MULADD(at[6], at[19]);       MULADD(at[7], at[18]);       MULADD(at[8], at[17]);       MULADD(at[9], at[16]);
      COMBA_STORE(C->dp[15]);
      // 16
      COMBA_FORWARD;
      MULADD(at[7], at[19]);       MULADD(at[8], at[18]);       MULADD(at[9], at[17]);
      COMBA_STORE(C->dp[16]);
      // 17
      COMBA_FORWARD;
      MULADD(at[8], at[19]);       MULADD(at[9], at[18]);
      COMBA_STORE(C->dp[17]);
      // 18
      COMBA_FORWARD;
      MULADD(at[9], at[19]);
      COMBA_STORE(C->dp[18]);
      COMBA_STORE2(C->dp[19]);
      C->used = 20;
      C->sign = A->sign ^ B->sign;
      fp_clamp(C);
      COMBA_FINI;
      break;

   case 11:
      memcpy(at, A->dp, 11 * sizeof(fp_digit));
      memcpy(at+11, B->dp, 11 * sizeof(fp_digit));
      COMBA_START;

      COMBA_CLEAR;
      // 0
      MULADD(at[0], at[11]);
      COMBA_STORE(C->dp[0]);
      // 1
      COMBA_FORWARD;
      MULADD(at[0], at[12]);       MULADD(at[1], at[11]);
      COMBA_STORE(C->dp[1]);
      // 2
      COMBA_FORWARD;
      MULADD(at[0], at[13]);       MULADD(at[1], at[12]);       MULADD(at[2], at[11]);
      COMBA_STORE(C->dp[2]);
      // 3
      COMBA_FORWARD;
      MULADD(at[0], at[14]);       MULADD(at[1], at[13]);       MULADD(at[2], at[12]);       MULADD(at[3], at[11]);
      COMBA_STORE(C->dp[3]);
      // 4
      COMBA_FORWARD;
      MULADD(at[0], at[15]);       MULADD(at[1], at[14]);       MULADD(at[2], at[13]);       MULADD(at[3], at[12]);       MULADD(at[4], at[11]);
      COMBA_STORE(C->dp[4]);
      // 5
      COMBA_FORWARD;
      MULADD(at[0], at[16]);       MULADD(at[1], at[15]);       MULADD(at[2], at[14]);       MULADD(at[3], at[13]);       MULADD(at[4], at[12]);       MULADD(at[5], at[11]);
      COMBA_STORE(C->dp[5]);
      // 6
      COMBA_FORWARD;
      MULADD(at[0], at[17]);       MULADD(at[1], at[16]);       MULADD(at[2], at[15]);       MULADD(at[3], at[14]);       MULADD(at[4], at[13]);       MULADD(at[5], at[12]);       MULADD(at[6], at[11]);
      COMBA_STORE(C->dp[6]);
      // 7
      COMBA_FORWARD;
      MULADD(at[0], at[18]);       MULADD(at[1], at[17]);       MULADD(at[2], at[16]);       MULADD(at[3], at[15]);       MULADD(at[4], at[14]);       MULADD(at[5], at[13]);       MULADD(at[6], at[12]);       MULADD(at[7], at[11]);
      COMBA_STORE(C->dp[7]);
      // 8
      COMBA_FORWARD;
      MULADD(at[0], at[19]);       MULADD(at[1], at[18]);       MULADD(at[2], at[17]);       MULADD(at[3], at[16]);       MULADD(at[4], at[15]);       MULADD(at[5], at[14]);       MULADD(at[6], at[13]);       MULADD(at[7], at[12]);       MULADD(at[8], at[11]);
      COMBA_STORE(C->dp[8]);
      // 9
      COMBA_FORWARD;
      MULADD(at[0], at[20]);       MULADD(at[1], at[19]);       MULADD(at[2], at[18]);       MULADD(at[3], at[17]);       MULADD(at[4], at[16]);       MULADD(at[5], at[15]);       MULADD(at[6], at[14]);       MULADD(at[7], at[13]);       MULADD(at[8], at[12]);       MULADD(at[9], at[11]);
      COMBA_STORE(C->dp[9]);
      // 10
      COMBA_FORWARD;
      MULADD(at[0], at[21]);       MULADD(at[1], at[20]);       MULADD(at[2], at[19]);       MULADD(at[3], at[18]);       MULADD(at[4], at[17]);       MULADD(at[5], at[16]);       MULADD(at[6], at[15]);       MULADD(at[7], at[14]);       MULADD(at[8], at[13]);       MULADD(at[9], at[12]);       MULADD(at[10], at[11]);
      COMBA_STORE(C->dp[10]);
      // 11
      COMBA_FORWARD;
      MULADD(at[1], at[21]);       MULADD(at[2], at[20]);       MULADD(at[3], at[19]);       MULADD(at[4], at[18]);       MULADD(at[5], at[17]);       MULADD(at[6], at[16]);       MULADD(at[7], at[15]);       MULADD(at[8], at[14]);       MULADD(at[9], at[13]);       MULADD(at[10], at[12]);
      COMBA_STORE(C->dp[11]);
      // 12
      COMBA_FORWARD;
      MULADD(at[2], at[21]);       MULADD(at[3], at[20]);       MULADD(at[4], at[19]);       MULADD(at[5], at[18]);       MULADD(at[6], at[17]);       MULADD(at[7], at[16]);       MULADD(at[8], at[15]);       MULADD(at[9], at[14]);       MULADD(at[10], at[13]);
      COMBA_STORE(C->dp[12]);
      // 13
      COMBA_FORWARD;
      MULADD(at[3], at[21]);       MULADD(at[4], at[20]);       MULADD(at[5], at[19]);       MULADD(at[6], at[18]);       MULADD(at[7], at[17]);       MULADD(at[8], at[16]);       MULADD(at[9], at[15]);       MULADD(at[10], at[14]);
      COMBA_STORE(C->dp[13]);
      // 14
      COMBA_FORWARD;
      MULADD(at[4], at[21]);       MULADD(at[5], at[20]);       MULADD(at[6], at[19]);       MULADD(at[7], at[18]);       MULADD(at[8], at[17]);       MULADD(at[9], at[16]);       MULADD(at[10], at[15]);
      COMBA_STORE(C->dp[14]);
      // 15
      COMBA_FORWARD;
      MULADD(at[5], at[21]);       MULADD(at[6], at[20]);       MULADD(at[7], at[19]);       MULADD(at[8], at[18]);       MULADD(at[9], at[17]);       MULADD(at[10], at[16]);
      COMBA_STORE(C->dp[15]);
      // 16
      COMBA_FORWARD;
      MULADD(at[6], at[21]);       MULADD(at[7], at[20]);       MULADD(at[8], at[19]);       MULADD(at[9], at[18]);       MULADD(at[10], at[17]);
      COMBA_STORE(C->dp[16]);
      // 17
      COMBA_FORWARD;
      MULADD(at[7], at[21]);       MULADD(at[8], at[20]);       MULADD(at[9], at[19]);       MULADD(at[10], at[18]);
      COMBA_STORE(C->dp[17]);
      // 18
      COMBA_FORWARD;
      MULADD(at[8], at[21]);       MULADD(at[9], at[20]);       MULADD(at[10], at[19]);
      COMBA_STORE(C->dp[18]);
      // 19
      COMBA_FORWARD;
      MULADD(at[9], at[21]);       MULADD(at[10], at[20]);
      COMBA_STORE(C->dp[19]);
      // 20
      COMBA_FORWARD;
      MULADD(at[10], at[21]);
      COMBA_STORE(C->dp[20]);
      COMBA_STORE2(C->dp[21]);
      C->used = 22;
      C->sign = A->sign ^ B->sign;
      fp_clamp(C);
      COMBA_FINI;
      break;

   case 12:
      memcpy(at, A->dp, 12 * sizeof(fp_digit));
      memcpy(at+12, B->dp, 12 * sizeof(fp_digit));
      COMBA_START;

      COMBA_CLEAR;
      // 0
      MULADD(at[0], at[12]);
      COMBA_STORE(C->dp[0]);
      // 1
      COMBA_FORWARD;
      MULADD(at[0], at[13]);       MULADD(at[1], at[12]);
      COMBA_STORE(C->dp[1]);
      // 2
      COMBA_FORWARD;
      MULADD(at[0], at[14]);       MULADD(at[1], at[13]);       MULADD(at[2], at[12]);
      COMBA_STORE(C->dp[2]);
      // 3
      COMBA_FORWARD;
      MULADD(at[0], at[15]);       MULADD(at[1], at[14]);       MULADD(at[2], at[13]);       MULADD(at[3], at[12]);
      COMBA_STORE(C->dp[3]);
      // 4
      COMBA_FORWARD;
      MULADD(at[0], at[16]);       MULADD(at[1], at[15]);       MULADD(at[2], at[14]);       MULADD(at[3], at[13]);       MULADD(at[4], at[12]);
      COMBA_STORE(C->dp[4]);
      // 5
      COMBA_FORWARD;
      MULADD(at[0], at[17]);       MULADD(at[1], at[16]);       MULADD(at[2], at[15]);       MULADD(at[3], at[14]);       MULADD(at[4], at[13]);       MULADD(at[5], at[12]);
      COMBA_STORE(C->dp[5]);
      // 6
      COMBA_FORWARD;
      MULADD(at[0], at[18]);       MULADD(at[1], at[17]);       MULADD(at[2], at[16]);       MULADD(at[3], at[15]);       MULADD(at[4], at[14]);       MULADD(at[5], at[13]);       MULADD(at[6], at[12]);
      COMBA_STORE(C->dp[6]);
      // 7
      COMBA_FORWARD;
      MULADD(at[0], at[19]);       MULADD(at[1], at[18]);       MULADD(at[2], at[17]);       MULADD(at[3], at[16]);       MULADD(at[4], at[15]);       MULADD(at[5], at[14]);       MULADD(at[6], at[13]);       MULADD(at[7], at[12]);
      COMBA_STORE(C->dp[7]);
      // 8
      COMBA_FORWARD;
      MULADD(at[0], at[20]);       MULADD(at[1], at[19]);       MULADD(at[2], at[18]);       MULADD(at[3], at[17]);       MULADD(at[4], at[16]);       MULADD(at[5], at[15]);       MULADD(at[6], at[14]);       MULADD(at[7], at[13]);       MULADD(at[8], at[12]);
      COMBA_STORE(C->dp[8]);
      // 9
      COMBA_FORWARD;
      MULADD(at[0], at[21]);       MULADD(at[1], at[20]);       MULADD(at[2], at[19]);       MULADD(at[3], at[18]);       MULADD(at[4], at[17]);       MULADD(at[5], at[16]);       MULADD(at[6], at[15]);       MULADD(at[7], at[14]);       MULADD(at[8], at[13]);       MULADD(at[9], at[12]);
      COMBA_STORE(C->dp[9]);
      // 10
      COMBA_FORWARD;
      MULADD(at[0], at[22]);       MULADD(at[1], at[21]);       MULADD(at[2], at[20]);       MULADD(at[3], at[19]);       MULADD(at[4], at[18]);       MULADD(at[5], at[17]);       MULADD(at[6], at[16]);       MULADD(at[7], at[15]);       MULADD(at[8], at[14]);       MULADD(at[9], at[13]);       MULADD(at[10], at[12]);
      COMBA_STORE(C->dp[10]);
      // 11
      COMBA_FORWARD;
      MULADD(at[0], at[23]);       MULADD(at[1], at[22]);       MULADD(at[2], at[21]);       MULADD(at[3], at[20]);       MULADD(at[4], at[19]);       MULADD(at[5], at[18]);       MULADD(at[6], at[17]);       MULADD(at[7], at[16]);       MULADD(at[8], at[15]);       MULADD(at[9], at[14]);       MULADD(at[10], at[13]);       MULADD(at[11], at[12]);
      COMBA_STORE(C->dp[11]);
      // 12
      COMBA_FORWARD;
      MULADD(at[1], at[23]);       MULADD(at[2], at[22]);       MULADD(at[3], at[21]);       MULADD(at[4], at[20]);       MULADD(at[5], at[19]);       MULADD(at[6], at[18]);       MULADD(at[7], at[17]);       MULADD(at[8], at[16]);       MULADD(at[9], at[15]);       MULADD(at[10], at[14]);       MULADD(at[11], at[13]);
      COMBA_STORE(C->dp[12]);
      // 13
      COMBA_FORWARD;
      MULADD(at[2], at[23]);       MULADD(at[3], at[22]);       MULADD(at[4], at[21]);       MULADD(at[5], at[20]);       MULADD(at[6], at[19]);       MULADD(at[7], at[18]);       MULADD(at[8], at[17]);       MULADD(at[9], at[16]);       MULADD(at[10], at[15]);       MULADD(at[11], at[14]);
      COMBA_STORE(C->dp[13]);
      // 14
      COMBA_FORWARD;
      MULADD(at[3], at[23]);       MULADD(at[4], at[22]);       MULADD(at[5], at[21]);       MULADD(at[6], at[20]);       MULADD(at[7], at[19]);       MULADD(at[8], at[18]);       MULADD(at[9], at[17]);       MULADD(at[10], at[16]);       MULADD(at[11], at[15]);
      COMBA_STORE(C->dp[14]);
      // 15
      COMBA_FORWARD;
      MULADD(at[4], at[23]);       MULADD(at[5], at[22]);       MULADD(at[6], at[21]);       MULADD(at[7], at[20]);       MULADD(at[8], at[19]);       MULADD(at[9], at[18]);       MULADD(at[10], at[17]);       MULADD(at[11], at[16]);
      COMBA_STORE(C->dp[15]);
      // 16
      COMBA_FORWARD;
      MULADD(at[5], at[23]);       MULADD(at[6], at[22]);       MULADD(at[7], at[21]);       MULADD(at[8], at[20]);       MULADD(at[9], at[19]);       MULADD(at[10], at[18]);       MULADD(at[11], at[17]);
      COMBA_STORE(C->dp[16]);
      // 17
      COMBA_FORWARD;
      MULADD(at[6], at[23]);       MULADD(at[7], at[22]);       MULADD(at[8], at[21]);       MULADD(at[9], at[20]);       MULADD(at[10], at[19]);       MULADD(at[11], at[18]);
      COMBA_STORE(C->dp[17]);
      // 18
      COMBA_FORWARD;
      MULADD(at[7], at[23]);       MULADD(at[8], at[22]);       MULADD(at[9], at[21]);       MULADD(at[10], at[20]);       MULADD(at[11], at[19]);
      COMBA_STORE(C->dp[18]);
      // 19
      COMBA_FORWARD;
      MULADD(at[8], at[23]);       MULADD(at[9], at[22]);       MULADD(at[10], at[21]);       MULADD(at[11], at[20]);
      COMBA_STORE(C->dp[19]);
      // 20
      COMBA_FORWARD;
      MULADD(at[9], at[23]);       MULADD(at[10], at[22]);       MULADD(at[11], at[21]);
      COMBA_STORE(C->dp[20]);
      // 21
      COMBA_FORWARD;
      MULADD(at[10], at[23]);       MULADD(at[11], at[22]);
      COMBA_STORE(C->dp[21]);
      // 22
      COMBA_FORWARD;
      MULADD(at[11], at[23]);
      COMBA_STORE(C->dp[22]);
      COMBA_STORE2(C->dp[23]);
      C->used = 24;
      C->sign = A->sign ^ B->sign;
      fp_clamp(C);
      COMBA_FINI;
      break;

   case 13:
      memcpy(at, A->dp, 13 * sizeof(fp_digit));
      memcpy(at+13, B->dp, 13 * sizeof(fp_digit));
      COMBA_START;

      COMBA_CLEAR;
      // 0
      MULADD(at[0], at[13]);
      COMBA_STORE(C->dp[0]);
      // 1
      COMBA_FORWARD;
      MULADD(at[0], at[14]);       MULADD(at[1], at[13]);
      COMBA_STORE(C->dp[1]);
      // 2
      COMBA_FORWARD;
      MULADD(at[0], at[15]);       MULADD(at[1], at[14]);       MULADD(at[2], at[13]);
      COMBA_STORE(C->dp[2]);
      // 3
      COMBA_FORWARD;
      MULADD(at[0], at[16]);       MULADD(at[1], at[15]);       MULADD(at[2], at[14]);       MULADD(at[3], at[13]);
      COMBA_STORE(C->dp[3]);
      // 4
      COMBA_FORWARD;
      MULADD(at[0], at[17]);       MULADD(at[1], at[16]);       MULADD(at[2], at[15]);       MULADD(at[3], at[14]);       MULADD(at[4], at[13]);
      COMBA_STORE(C->dp[4]);
      // 5
      COMBA_FORWARD;
      MULADD(at[0], at[18]);       MULADD(at[1], at[17]);       MULADD(at[2], at[16]);       MULADD(at[3], at[15]);       MULADD(at[4], at[14]);       MULADD(at[5], at[13]);
      COMBA_STORE(C->dp[5]);
      // 6
      COMBA_FORWARD;
      MULADD(at[0], at[19]);       MULADD(at[1], at[18]);       MULADD(at[2], at[17]);       MULADD(at[3], at[16]);       MULADD(at[4], at[15]);       MULADD(at[5], at[14]);       MULADD(at[6], at[13]);
      COMBA_STORE(C->dp[6]);
      // 7
      COMBA_FORWARD;
      MULADD(at[0], at[20]);       MULADD(at[1], at[19]);       MULADD(at[2], at[18]);       MULADD(at[3], at[17]);       MULADD(at[4], at[16]);       MULADD(at[5], at[15]);       MULADD(at[6], at[14]);       MULADD(at[7], at[13]);
      COMBA_STORE(C->dp[7]);
      // 8
      COMBA_FORWARD;
      MULADD(at[0], at[21]);       MULADD(at[1], at[20]);       MULADD(at[2], at[19]);       MULADD(at[3], at[18]);       MULADD(at[4], at[17]);       MULADD(at[5], at[16]);       MULADD(at[6], at[15]);       MULADD(at[7], at[14]);       MULADD(at[8], at[13]);
      COMBA_STORE(C->dp[8]);
      // 9
      COMBA_FORWARD;
      MULADD(at[0], at[22]);       MULADD(at[1], at[21]);       MULADD(at[2], at[20]);       MULADD(at[3], at[19]);       MULADD(at[4], at[18]);       MULADD(at[5], at[17]);       MULADD(at[6], at[16]);       MULADD(at[7], at[15]);       MULADD(at[8], at[14]);       MULADD(at[9], at[13]);
      COMBA_STORE(C->dp[9]);
      // 10
      COMBA_FORWARD;
      MULADD(at[0], at[23]);       MULADD(at[1], at[22]);       MULADD(at[2], at[21]);       MULADD(at[3], at[20]);       MULADD(at[4], at[19]);       MULADD(at[5], at[18]);       MULADD(at[6], at[17]);       MULADD(at[7], at[16]);       MULADD(at[8], at[15]);       MULADD(at[9], at[14]);       MULADD(at[10], at[13]);
      COMBA_STORE(C->dp[10]);
      // 11
      COMBA_FORWARD;
      MULADD(at[0], at[24]);       MULADD(at[1], at[23]);       MULADD(at[2], at[22]);       MULADD(at[3], at[21]);       MULADD(at[4], at[20]);       MULADD(at[5], at[19]);       MULADD(at[6], at[18]);       MULADD(at[7], at[17]);       MULADD(at[8], at[16]);       MULADD(at[9], at[15]);       MULADD(at[10], at[14]);       MULADD(at[11], at[13]);
      COMBA_STORE(C->dp[11]);
      // 12
      COMBA_FORWARD;
      MULADD(at[0], at[25]);       MULADD(at[1], at[24]);       MULADD(at[2], at[23]);       MULADD(at[3], at[22]);       MULADD(at[4], at[21]);       MULADD(at[5], at[20]);       MULADD(at[6], at[19]);       MULADD(at[7], at[18]);       MULADD(at[8], at[17]);       MULADD(at[9], at[16]);       MULADD(at[10], at[15]);       MULADD(at[11], at[14]);       MULADD(at[12], at[13]);
      COMBA_STORE(C->dp[12]);
      // 13
      COMBA_FORWARD;
      MULADD(at[1], at[25]);       MULADD(at[2], at[24]);       MULADD(at[3], at[23]);       MULADD(at[4], at[22]);       MULADD(at[5], at[21]);       MULADD(at[6], at[20]);       MULADD(at[7], at[19]);       MULADD(at[8], at[18]);       MULADD(at[9], at[17]);       MULADD(at[10], at[16]);       MULADD(at[11], at[15]);       MULADD(at[12], at[14]);
      COMBA_STORE(C->dp[13]);
      // 14
      COMBA_FORWARD;
      MULADD(at[2], at[25]);       MULADD(at[3], at[24]);       MULADD(at[4], at[23]);       MULADD(at[5], at[22]);       MULADD(at[6], at[21]);       MULADD(at[7], at[20]);       MULADD(at[8], at[19]);       MULADD(at[9], at[18]);       MULADD(at[10], at[17]);       MULADD(at[11], at[16]);       MULADD(at[12], at[15]);
      COMBA_STORE(C->dp[14]);
      // 15
      COMBA_FORWARD;
      MULADD(at[3], at[25]);       MULADD(at[4], at[24]);       MULADD(at[5], at[23]);       MULADD(at[6], at[22]);       MULADD(at[7], at[21]);       MULADD(at[8], at[20]);       MULADD(at[9], at[19]);       MULADD(at[10], at[18]);       MULADD(at[11], at[17]);       MULADD(at[12], at[16]);
      COMBA_STORE(C->dp[15]);
      // 16
      COMBA_FORWARD;
      MULADD(at[4], at[25]);       MULADD(at[5], at[24]);       MULADD(at[6], at[23]);       MULADD(at[7], at[22]);       MULADD(at[8], at[21]);       MULADD(at[9], at[20]);       MULADD(at[10], at[19]);       MULADD(at[11], at[18]);       MULADD(at[12], at[17]);
      COMBA_STORE(C->dp[16]);
      // 17
      COMBA_FORWARD;
      MULADD(at[5], at[25]);       MULADD(at[6], at[24]);       MULADD(at[7], at[23]);       MULADD(at[8], at[22]);       MULADD(at[9], at[21]);       MULADD(at[10], at[20]);       MULADD(at[11], at[19]);       MULADD(at[12], at[18]);
      COMBA_STORE(C->dp[17]);
      // 18
      COMBA_FORWARD;
      MULADD(at[6], at[25]);       MULADD(at[7], at[24]);       MULADD(at[8], at[23]);       MULADD(at[9], at[22]);       MULADD(at[10], at[21]);       MULADD(at[11], at[20]);       MULADD(at[12], at[19]);
      COMBA_STORE(C->dp[18]);
      // 19
      COMBA_FORWARD;
      MULADD(at[7], at[25]);       MULADD(at[8], at[24]);       MULADD(at[9], at[23]);       MULADD(at[10], at[22]);       MULADD(at[11], at[21]);       MULADD(at[12], at[20]);
      COMBA_STORE(C->dp[19]);
      // 20
      COMBA_FORWARD;
      MULADD(at[8], at[25]);       MULADD(at[9], at[24]);       MULADD(at[10], at[23]);       MULADD(at[11], at[22]);       MULADD(at[12], at[21]);
      COMBA_STORE(C->dp[20]);
      // 21
      COMBA_FORWARD;
      MULADD(at[9], at[25]);       MULADD(at[10], at[24]);       MULADD(at[11], at[23]);       MULADD(at[12], at[22]);
      COMBA_STORE(C->dp[21]);
      // 22
      COMBA_FORWARD;
      MULADD(at[10], at[25]);       MULADD(at[11], at[24]);       MULADD(at[12], at[23]);
      COMBA_STORE(C->dp[22]);
      // 23
      COMBA_FORWARD;
      MULADD(at[11], at[25]);       MULADD(at[12], at[24]);
      COMBA_STORE(C->dp[23]);
      // 24
      COMBA_FORWARD;
      MULADD(at[12], at[25]);
      COMBA_STORE(C->dp[24]);
      COMBA_STORE2(C->dp[25]);
      C->used = 26;
      C->sign = A->sign ^ B->sign;
      fp_clamp(C);
      COMBA_FINI;
      break;

   case 14:
      memcpy(at, A->dp, 14 * sizeof(fp_digit));
      memcpy(at+14, B->dp, 14 * sizeof(fp_digit));
      COMBA_START;

      COMBA_CLEAR;
      // 0
      MULADD(at[0], at[14]);
      COMBA_STORE(C->dp[0]);
      // 1
      COMBA_FORWARD;
      MULADD(at[0], at[15]);       MULADD(at[1], at[14]);
      COMBA_STORE(C->dp[1]);
      // 2
      COMBA_FORWARD;
      MULADD(at[0], at[16]);       MULADD(at[1], at[15]);       MULADD(at[2], at[14]);
      COMBA_STORE(C->dp[2]);
      // 3
      COMBA_FORWARD;
      MULADD(at[0], at[17]);       MULADD(at[1], at[16]);       MULADD(at[2], at[15]);       MULADD(at[3], at[14]);
      COMBA_STORE(C->dp[3]);
      // 4
      COMBA_FORWARD;
      MULADD(at[0], at[18]);       MULADD(at[1], at[17]);       MULADD(at[2], at[16]);       MULADD(at[3], at[15]);       MULADD(at[4], at[14]);
      COMBA_STORE(C->dp[4]);
      // 5
      COMBA_FORWARD;
      MULADD(at[0], at[19]);       MULADD(at[1], at[18]);       MULADD(at[2], at[17]);       MULADD(at[3], at[16]);       MULADD(at[4], at[15]);       MULADD(at[5], at[14]);
      COMBA_STORE(C->dp[5]);
      // 6
      COMBA_FORWARD;
      MULADD(at[0], at[20]);       MULADD(at[1], at[19]);       MULADD(at[2], at[18]);       MULADD(at[3], at[17]);       MULADD(at[4], at[16]);       MULADD(at[5], at[15]);       MULADD(at[6], at[14]);
      COMBA_STORE(C->dp[6]);
      // 7
      COMBA_FORWARD;
      MULADD(at[0], at[21]);       MULADD(at[1], at[20]);       MULADD(at[2], at[19]);       MULADD(at[3], at[18]);       MULADD(at[4], at[17]);       MULADD(at[5], at[16]);       MULADD(at[6], at[15]);       MULADD(at[7], at[14]);
      COMBA_STORE(C->dp[7]);
      // 8
      COMBA_FORWARD;
      MULADD(at[0], at[22]);       MULADD(at[1], at[21]);       MULADD(at[2], at[20]);       MULADD(at[3], at[19]);       MULADD(at[4], at[18]);       MULADD(at[5], at[17]);       MULADD(at[6], at[16]);       MULADD(at[7], at[15]);       MULADD(at[8], at[14]);
      COMBA_STORE(C->dp[8]);
      // 9
      COMBA_FORWARD;
      MULADD(at[0], at[23]);       MULADD(at[1], at[22]);       MULADD(at[2], at[21]);       MULADD(at[3], at[20]);       MULADD(at[4], at[19]);       MULADD(at[5], at[18]);       MULADD(at[6], at[17]);       MULADD(at[7], at[16]);       MULADD(at[8], at[15]);       MULADD(at[9], at[14]);
      COMBA_STORE(C->dp[9]);
      // 10
      COMBA_FORWARD;
      MULADD(at[0], at[24]);       MULADD(at[1], at[23]);       MULADD(at[2], at[22]);       MULADD(at[3], at[21]);       MULADD(at[4], at[20]);       MULADD(at[5], at[19]);       MULADD(at[6], at[18]);       MULADD(at[7], at[17]);       MULADD(at[8], at[16]);       MULADD(at[9], at[15]);       MULADD(at[10], at[14]);
      COMBA_STORE(C->dp[10]);
      // 11
      COMBA_FORWARD;
      MULADD(at[0], at[25]);       MULADD(at[1], at[24]);       MULADD(at[2], at[23]);       MULADD(at[3], at[22]);       MULADD(at[4], at[21]);       MULADD(at[5], at[20]);       MULADD(at[6], at[19]);       MULADD(at[7], at[18]);       MULADD(at[8], at[17]);       MULADD(at[9], at[16]);       MULADD(at[10], at[15]);       MULADD(at[11], at[14]);
      COMBA_STORE(C->dp[11]);
      // 12
      COMBA_FORWARD;
      MULADD(at[0], at[26]);       MULADD(at[1], at[25]);       MULADD(at[2], at[24]);       MULADD(at[3], at[23]);       MULADD(at[4], at[22]);       MULADD(at[5], at[21]);       MULADD(at[6], at[20]);       MULADD(at[7], at[19]);       MULADD(at[8], at[18]);       MULADD(at[9], at[17]);       MULADD(at[10], at[16]);       MULADD(at[11], at[15]);       MULADD(at[12], at[14]);
      COMBA_STORE(C->dp[12]);
      // 13
      COMBA_FORWARD;
      MULADD(at[0], at[27]);       MULADD(at[1], at[26]);       MULADD(at[2], at[25]);       MULADD(at[3], at[24]);       MULADD(at[4], at[23]);       MULADD(at[5], at[22]);       MULADD(at[6], at[21]);       MULADD(at[7], at[20]);       MULADD(at[8], at[19]);       MULADD(at[9], at[18]);       MULADD(at[10], at[17]);       MULADD(at[11], at[16]);       MULADD(at[12], at[15]);       MULADD(at[13], at[14]);
      COMBA_STORE(C->dp[13]);
      // 14
      COMBA_FORWARD;
      MULADD(at[1], at[27]);       MULADD(at[2], at[26]);       MULADD(at[3], at[25]);       MULADD(at[4], at[24]);       MULADD(at[5], at[23]);       MULADD(at[6], at[22]);       MULADD(at[7], at[21]);       MULADD(at[8], at[20]);       MULADD(at[9], at[19]);       MULADD(at[10], at[18]);       MULADD(at[11], at[17]);       MULADD(at[12], at[16]);       MULADD(at[13], at[15]);
      COMBA_STORE(C->dp[14]);
      // 15
      COMBA_FORWARD;
      MULADD(at[2], at[27]);       MULADD(at[3], at[26]);       MULADD(at[4], at[25]);       MULADD(at[5], at[24]);       MULADD(at[6], at[23]);       MULADD(at[7], at[22]);       MULADD(at[8], at[21]);       MULADD(at[9], at[20]);       MULADD(at[10], at[19]);       MULADD(at[11], at[18]);       MULADD(at[12], at[17]);       MULADD(at[13], at[16]);
      COMBA_STORE(C->dp[15]);
      // 16
      COMBA_FORWARD;
      MULADD(at[3], at[27]);       MULADD(at[4], at[26]);       MULADD(at[5], at[25]);       MULADD(at[6], at[24]);       MULADD(at[7], at[23]);       MULADD(at[8], at[22]);       MULADD(at[9], at[21]);       MULADD(at[10], at[20]);       MULADD(at[11], at[19]);       MULADD(at[12], at[18]);       MULADD(at[13], at[17]);
      COMBA_STORE(C->dp[16]);
      // 17
      COMBA_FORWARD;
      MULADD(at[4], at[27]);       MULADD(at[5], at[26]);       MULADD(at[6], at[25]);       MULADD(at[7], at[24]);       MULADD(at[8], at[23]);       MULADD(at[9], at[22]);       MULADD(at[10], at[21]);       MULADD(at[11], at[20]);       MULADD(at[12], at[19]);       MULADD(at[13], at[18]);
      COMBA_STORE(C->dp[17]);
      // 18
      COMBA_FORWARD;
      MULADD(at[5], at[27]);       MULADD(at[6], at[26]);       MULADD(at[7], at[25]);       MULADD(at[8], at[24]);       MULADD(at[9], at[23]);       MULADD(at[10], at[22]);       MULADD(at[11], at[21]);       MULADD(at[12], at[20]);       MULADD(at[13], at[19]);
      COMBA_STORE(C->dp[18]);
      // 19
      COMBA_FORWARD;
      MULADD(at[6], at[27]);       MULADD(at[7], at[26]);       MULADD(at[8], at[25]);       MULADD(at[9], at[24]);       MULADD(at[10], at[23]);       MULADD(at[11], at[22]);       MULADD(at[12], at[21]);       MULADD(at[13], at[20]);
      COMBA_STORE(C->dp[19]);
      // 20
      COMBA_FORWARD;
      MULADD(at[7], at[27]);       MULADD(at[8], at[26]);       MULADD(at[9], at[25]);       MULADD(at[10], at[24]);       MULADD(at[11], at[23]);       MULADD(at[12], at[22]);       MULADD(at[13], at[21]);
      COMBA_STORE(C->dp[20]);
      // 21
      COMBA_FORWARD;
      MULADD(at[8], at[27]);       MULADD(at[9], at[26]);       MULADD(at[10], at[25]);       MULADD(at[11], at[24]);       MULADD(at[12], at[23]);       MULADD(at[13], at[22]);
      COMBA_STORE(C->dp[21]);
      // 22
      COMBA_FORWARD;
      MULADD(at[9], at[27]);       MULADD(at[10], at[26]);       MULADD(at[11], at[25]);       MULADD(at[12], at[24]);       MULADD(at[13], at[23]);
      COMBA_STORE(C->dp[22]);
      // 23
      COMBA_FORWARD;
      MULADD(at[10], at[27]);       MULADD(at[11], at[26]);       MULADD(at[12], at[25]);       MULADD(at[13], at[24]);
      COMBA_STORE(C->dp[23]);
      // 24
      COMBA_FORWARD;
      MULADD(at[11], at[27]);       MULADD(at[12], at[26]);       MULADD(at[13], at[25]);
      COMBA_STORE(C->dp[24]);
      // 25
      COMBA_FORWARD;
      MULADD(at[12], at[27]);       MULADD(at[13], at[26]);
      COMBA_STORE(C->dp[25]);
      // 26
      COMBA_FORWARD;
      MULADD(at[13], at[27]);
      COMBA_STORE(C->dp[26]);
      COMBA_STORE2(C->dp[27]);
      C->used = 28;
      C->sign = A->sign ^ B->sign;
      fp_clamp(C);
      COMBA_FINI;
      break;

   case 15:
      memcpy(at, A->dp, 15 * sizeof(fp_digit));
      memcpy(at+15, B->dp, 15 * sizeof(fp_digit));
      COMBA_START;

      COMBA_CLEAR;
      // 0
      MULADD(at[0], at[15]);
      COMBA_STORE(C->dp[0]);
      // 1
      COMBA_FORWARD;
      MULADD(at[0], at[16]);       MULADD(at[1], at[15]);
      COMBA_STORE(C->dp[1]);
      // 2
      COMBA_FORWARD;
      MULADD(at[0], at[17]);       MULADD(at[1], at[16]);       MULADD(at[2], at[15]);
      COMBA_STORE(C->dp[2]);
      // 3
      COMBA_FORWARD;
      MULADD(at[0], at[18]);       MULADD(at[1], at[17]);       MULADD(at[2], at[16]);       MULADD(at[3], at[15]);
      COMBA_STORE(C->dp[3]);
      // 4
      COMBA_FORWARD;
      MULADD(at[0], at[19]);       MULADD(at[1], at[18]);       MULADD(at[2], at[17]);       MULADD(at[3], at[16]);       MULADD(at[4], at[15]);
      COMBA_STORE(C->dp[4]);
      // 5
      COMBA_FORWARD;
      MULADD(at[0], at[20]);       MULADD(at[1], at[19]);       MULADD(at[2], at[18]);       MULADD(at[3], at[17]);       MULADD(at[4], at[16]);       MULADD(at[5], at[15]);
      COMBA_STORE(C->dp[5]);
      // 6
      COMBA_FORWARD;
      MULADD(at[0], at[21]);       MULADD(at[1], at[20]);       MULADD(at[2], at[19]);       MULADD(at[3], at[18]);       MULADD(at[4], at[17]);       MULADD(at[5], at[16]);       MULADD(at[6], at[15]);
      COMBA_STORE(C->dp[6]);
      // 7
      COMBA_FORWARD;
      MULADD(at[0], at[22]);       MULADD(at[1], at[21]);       MULADD(at[2], at[20]);       MULADD(at[3], at[19]);       MULADD(at[4], at[18]);       MULADD(at[5], at[17]);       MULADD(at[6], at[16]);       MULADD(at[7], at[15]);
      COMBA_STORE(C->dp[7]);
      // 8
      COMBA_FORWARD;
      MULADD(at[0], at[23]);       MULADD(at[1], at[22]);       MULADD(at[2], at[21]);       MULADD(at[3], at[20]);       MULADD(at[4], at[19]);       MULADD(at[5], at[18]);       MULADD(at[6], at[17]);       MULADD(at[7], at[16]);       MULADD(at[8], at[15]);
      COMBA_STORE(C->dp[8]);
      // 9
      COMBA_FORWARD;
      MULADD(at[0], at[24]);       MULADD(at[1], at[23]);       MULADD(at[2], at[22]);       MULADD(at[3], at[21]);       MULADD(at[4], at[20]);       MULADD(at[5], at[19]);       MULADD(at[6], at[18]);       MULADD(at[7], at[17]);       MULADD(at[8], at[16]);       MULADD(at[9], at[15]);
      COMBA_STORE(C->dp[9]);
      // 10
      COMBA_FORWARD;
      MULADD(at[0], at[25]);       MULADD(at[1], at[24]);       MULADD(at[2], at[23]);       MULADD(at[3], at[22]);       MULADD(at[4], at[21]);       MULADD(at[5], at[20]);       MULADD(at[6], at[19]);       MULADD(at[7], at[18]);       MULADD(at[8], at[17]);       MULADD(at[9], at[16]);       MULADD(at[10], at[15]);
      COMBA_STORE(C->dp[10]);
      // 11
      COMBA_FORWARD;
      MULADD(at[0], at[26]);       MULADD(at[1], at[25]);       MULADD(at[2], at[24]);       MULADD(at[3], at[23]);       MULADD(at[4], at[22]);       MULADD(at[5], at[21]);       MULADD(at[6], at[20]);       MULADD(at[7], at[19]);       MULADD(at[8], at[18]);       MULADD(at[9], at[17]);       MULADD(at[10], at[16]);       MULADD(at[11], at[15]);
      COMBA_STORE(C->dp[11]);
      // 12
      COMBA_FORWARD;
      MULADD(at[0], at[27]);       MULADD(at[1], at[26]);       MULADD(at[2], at[25]);       MULADD(at[3], at[24]);       MULADD(at[4], at[23]);       MULADD(at[5], at[22]);       MULADD(at[6], at[21]);       MULADD(at[7], at[20]);       MULADD(at[8], at[19]);       MULADD(at[9], at[18]);       MULADD(at[10], at[17]);       MULADD(at[11], at[16]);       MULADD(at[12], at[15]);
      COMBA_STORE(C->dp[12]);
      // 13
      COMBA_FORWARD;
      MULADD(at[0], at[28]);       MULADD(at[1], at[27]);       MULADD(at[2], at[26]);       MULADD(at[3], at[25]);       MULADD(at[4], at[24]);       MULADD(at[5], at[23]);       MULADD(at[6], at[22]);       MULADD(at[7], at[21]);       MULADD(at[8], at[20]);       MULADD(at[9], at[19]);       MULADD(at[10], at[18]);       MULADD(at[11], at[17]);       MULADD(at[12], at[16]);       MULADD(at[13], at[15]);
      COMBA_STORE(C->dp[13]);
      // 14
      COMBA_FORWARD;
      MULADD(at[0], at[29]);       MULADD(at[1], at[28]);       MULADD(at[2], at[27]);       MULADD(at[3], at[26]);       MULADD(at[4], at[25]);       MULADD(at[5], at[24]);       MULADD(at[6], at[23]);       MULADD(at[7], at[22]);       MULADD(at[8], at[21]);       MULADD(at[9], at[20]);       MULADD(at[10], at[19]);       MULADD(at[11], at[18]);       MULADD(at[12], at[17]);       MULADD(at[13], at[16]);       MULADD(at[14], at[15]);
      COMBA_STORE(C->dp[14]);
      // 15
      COMBA_FORWARD;
      MULADD(at[1], at[29]);       MULADD(at[2], at[28]);       MULADD(at[3], at[27]);       MULADD(at[4], at[26]);       MULADD(at[5], at[25]);       MULADD(at[6], at[24]);       MULADD(at[7], at[23]);       MULADD(at[8], at[22]);       MULADD(at[9], at[21]);       MULADD(at[10], at[20]);       MULADD(at[11], at[19]);       MULADD(at[12], at[18]);       MULADD(at[13], at[17]);       MULADD(at[14], at[16]);
      COMBA_STORE(C->dp[15]);
      // 16
      COMBA_FORWARD;
      MULADD(at[2], at[29]);       MULADD(at[3], at[28]);       MULADD(at[4], at[27]);       MULADD(at[5], at[26]);       MULADD(at[6], at[25]);       MULADD(at[7], at[24]);       MULADD(at[8], at[23]);       MULADD(at[9], at[22]);       MULADD(at[10], at[21]);       MULADD(at[11], at[20]);       MULADD(at[12], at[19]);       MULADD(at[13], at[18]);       MULADD(at[14], at[17]);
      COMBA_STORE(C->dp[16]);
      // 17
      COMBA_FORWARD;
      MULADD(at[3], at[29]);       MULADD(at[4], at[28]);       MULADD(at[5], at[27]);       MULADD(at[6], at[26]);       MULADD(at[7], at[25]);       MULADD(at[8], at[24]);       MULADD(at[9], at[23]);       MULADD(at[10], at[22]);       MULADD(at[11], at[21]);       MULADD(at[12], at[20]);       MULADD(at[13], at[19]);       MULADD(at[14], at[18]);
      COMBA_STORE(C->dp[17]);
      // 18
      COMBA_FORWARD;
      MULADD(at[4], at[29]);       MULADD(at[5], at[28]);       MULADD(at[6], at[27]);       MULADD(at[7], at[26]);       MULADD(at[8], at[25]);       MULADD(at[9], at[24]);       MULADD(at[10], at[23]);       MULADD(at[11], at[22]);       MULADD(at[12], at[21]);       MULADD(at[13], at[20]);       MULADD(at[14], at[19]);
      COMBA_STORE(C->dp[18]);
      // 19
      COMBA_FORWARD;
      MULADD(at[5], at[29]);       MULADD(at[6], at[28]);       MULADD(at[7], at[27]);       MULADD(at[8], at[26]);       MULADD(at[9], at[25]);       MULADD(at[10], at[24]);       MULADD(at[11], at[23]);       MULADD(at[12], at[22]);       MULADD(at[13], at[21]);       MULADD(at[14], at[20]);
      COMBA_STORE(C->dp[19]);
      // 20
      COMBA_FORWARD;
      MULADD(at[6], at[29]);       MULADD(at[7], at[28]);       MULADD(at[8], at[27]);       MULADD(at[9], at[26]);       MULADD(at[10], at[25]);       MULADD(at[11], at[24]);       MULADD(at[12], at[23]);       MULADD(at[13], at[22]);       MULADD(at[14], at[21]);
      COMBA_STORE(C->dp[20]);
      // 21
      COMBA_FORWARD;
      MULADD(at[7], at[29]);       MULADD(at[8], at[28]);       MULADD(at[9], at[27]);       MULADD(at[10], at[26]);       MULADD(at[11], at[25]);       MULADD(at[12], at[24]);       MULADD(at[13], at[23]);       MULADD(at[14], at[22]);
      COMBA_STORE(C->dp[21]);
      // 22
      COMBA_FORWARD;
      MULADD(at[8], at[29]);       MULADD(at[9], at[28]);       MULADD(at[10], at[27]);       MULADD(at[11], at[26]);       MULADD(at[12], at[25]);       MULADD(at[13], at[24]);       MULADD(at[14], at[23]);
      COMBA_STORE(C->dp[22]);
      // 23
      COMBA_FORWARD;
      MULADD(at[9], at[29]);       MULADD(at[10], at[28]);       MULADD(at[11], at[27]);       MULADD(at[12], at[26]);       MULADD(at[13], at[25]);       MULADD(at[14], at[24]);
      COMBA_STORE(C->dp[23]);
      // 24
      COMBA_FORWARD;
      MULADD(at[10], at[29]);       MULADD(at[11], at[28]);       MULADD(at[12], at[27]);       MULADD(at[13], at[26]);       MULADD(at[14], at[25]);
      COMBA_STORE(C->dp[24]);
      // 25
      COMBA_FORWARD;
      MULADD(at[11], at[29]);       MULADD(at[12], at[28]);       MULADD(at[13], at[27]);       MULADD(at[14], at[26]);
      COMBA_STORE(C->dp[25]);
      // 26
      COMBA_FORWARD;
      MULADD(at[12], at[29]);       MULADD(at[13], at[28]);       MULADD(at[14], at[27]);
      COMBA_STORE(C->dp[26]);
      // 27
      COMBA_FORWARD;
      MULADD(at[13], at[29]);       MULADD(at[14], at[28]);
      COMBA_STORE(C->dp[27]);
      // 28
      COMBA_FORWARD;
      MULADD(at[14], at[29]);
      COMBA_STORE(C->dp[28]);
      COMBA_STORE2(C->dp[29]);
      C->used = 30;
      C->sign = A->sign ^ B->sign;
      fp_clamp(C);
      COMBA_FINI;
      break;

   case 16:
      memcpy(at, A->dp, 16 * sizeof(fp_digit));
      memcpy(at+16, B->dp, 16 * sizeof(fp_digit));
      COMBA_START;

      COMBA_CLEAR;
      // 0
      MULADD(at[0], at[16]);
      COMBA_STORE(C->dp[0]);
      // 1
      COMBA_FORWARD;
      MULADD(at[0], at[17]);       MULADD(at[1], at[16]);
      COMBA_STORE(C->dp[1]);
      // 2
      COMBA_FORWARD;
      MULADD(at[0], at[18]);       MULADD(at[1], at[17]);       MULADD(at[2], at[16]);
      COMBA_STORE(C->dp[2]);
      // 3
      COMBA_FORWARD;
      MULADD(at[0], at[19]);       MULADD(at[1], at[18]);       MULADD(at[2], at[17]);       MULADD(at[3], at[16]);
      COMBA_STORE(C->dp[3]);
      // 4
      COMBA_FORWARD;
      MULADD(at[0], at[20]);       MULADD(at[1], at[19]);       MULADD(at[2], at[18]);       MULADD(at[3], at[17]);       MULADD(at[4], at[16]);
      COMBA_STORE(C->dp[4]);
      // 5
      COMBA_FORWARD;
      MULADD(at[0], at[21]);       MULADD(at[1], at[20]);       MULADD(at[2], at[19]);       MULADD(at[3], at[18]);       MULADD(at[4], at[17]);       MULADD(at[5], at[16]);
      COMBA_STORE(C->dp[5]);
      // 6
      COMBA_FORWARD;
      MULADD(at[0], at[22]);       MULADD(at[1], at[21]);       MULADD(at[2], at[20]);       MULADD(at[3], at[19]);       MULADD(at[4], at[18]);       MULADD(at[5], at[17]);       MULADD(at[6], at[16]);
      COMBA_STORE(C->dp[6]);
      // 7
      COMBA_FORWARD;
      MULADD(at[0], at[23]);       MULADD(at[1], at[22]);       MULADD(at[2], at[21]);       MULADD(at[3], at[20]);       MULADD(at[4], at[19]);       MULADD(at[5], at[18]);       MULADD(at[6], at[17]);       MULADD(at[7], at[16]);
      COMBA_STORE(C->dp[7]);
      // 8
      COMBA_FORWARD;
      MULADD(at[0], at[24]);       MULADD(at[1], at[23]);       MULADD(at[2], at[22]);       MULADD(at[3], at[21]);       MULADD(at[4], at[20]);       MULADD(at[5], at[19]);       MULADD(at[6], at[18]);       MULADD(at[7], at[17]);       MULADD(at[8], at[16]);
      COMBA_STORE(C->dp[8]);
      // 9
      COMBA_FORWARD;
      MULADD(at[0], at[25]);       MULADD(at[1], at[24]);       MULADD(at[2], at[23]);       MULADD(at[3], at[22]);       MULADD(at[4], at[21]);       MULADD(at[5], at[20]);       MULADD(at[6], at[19]);       MULADD(at[7], at[18]);       MULADD(at[8], at[17]);       MULADD(at[9], at[16]);
      COMBA_STORE(C->dp[9]);
      // 10
      COMBA_FORWARD;
      MULADD(at[0], at[26]);       MULADD(at[1], at[25]);       MULADD(at[2], at[24]);       MULADD(at[3], at[23]);       MULADD(at[4], at[22]);       MULADD(at[5], at[21]);       MULADD(at[6], at[20]);       MULADD(at[7], at[19]);       MULADD(at[8], at[18]);       MULADD(at[9], at[17]);       MULADD(at[10], at[16]);
      COMBA_STORE(C->dp[10]);
      // 11
      COMBA_FORWARD;
      MULADD(at[0], at[27]);       MULADD(at[1], at[26]);       MULADD(at[2], at[25]);       MULADD(at[3], at[24]);       MULADD(at[4], at[23]);       MULADD(at[5], at[22]);       MULADD(at[6], at[21]);       MULADD(at[7], at[20]);       MULADD(at[8], at[19]);       MULADD(at[9], at[18]);       MULADD(at[10], at[17]);       MULADD(at[11], at[16]);
      COMBA_STORE(C->dp[11]);
      // 12
      COMBA_FORWARD;
      MULADD(at[0], at[28]);       MULADD(at[1], at[27]);       MULADD(at[2], at[26]);       MULADD(at[3], at[25]);       MULADD(at[4], at[24]);       MULADD(at[5], at[23]);       MULADD(at[6], at[22]);       MULADD(at[7], at[21]);       MULADD(at[8], at[20]);       MULADD(at[9], at[19]);       MULADD(at[10], at[18]);       MULADD(at[11], at[17]);       MULADD(at[12], at[16]);
      COMBA_STORE(C->dp[12]);
      // 13
      COMBA_FORWARD;
      MULADD(at[0], at[29]);       MULADD(at[1], at[28]);       MULADD(at[2], at[27]);       MULADD(at[3], at[26]);       MULADD(at[4], at[25]);       MULADD(at[5], at[24]);       MULADD(at[6], at[23]);       MULADD(at[7], at[22]);       MULADD(at[8], at[21]);       MULADD(at[9], at[20]);       MULADD(at[10], at[19]);       MULADD(at[11], at[18]);       MULADD(at[12], at[17]);       MULADD(at[13], at[16]);
      COMBA_STORE(C->dp[13]);
      // 14
      COMBA_FORWARD;
      MULADD(at[0], at[30]);       MULADD(at[1], at[29]);       MULADD(at[2], at[28]);       MULADD(at[3], at[27]);       MULADD(at[4], at[26]);       MULADD(at[5], at[25]);       MULADD(at[6], at[24]);       MULADD(at[7], at[23]);       MULADD(at[8], at[22]);       MULADD(at[9], at[21]);       MULADD(at[10], at[20]);       MULADD(at[11], at[19]);       MULADD(at[12], at[18]);       MULADD(at[13], at[17]);       MULADD(at[14], at[16]);
      COMBA_STORE(C->dp[14]);
      // 15
      COMBA_FORWARD;
      MULADD(at[0], at[31]);       MULADD(at[1], at[30]);       MULADD(at[2], at[29]);       MULADD(at[3], at[28]);       MULADD(at[4], at[27]);       MULADD(at[5], at[26]);       MULADD(at[6], at[25]);       MULADD(at[7], at[24]);       MULADD(at[8], at[23]);       MULADD(at[9], at[22]);       MULADD(at[10], at[21]);       MULADD(at[11], at[20]);       MULADD(at[12], at[19]);       MULADD(at[13], at[18]);       MULADD(at[14], at[17]);       MULADD(at[15], at[16]);
      COMBA_STORE(C->dp[15]);
      // 16
      COMBA_FORWARD;
      MULADD(at[1], at[31]);       MULADD(at[2], at[30]);       MULADD(at[3], at[29]);       MULADD(at[4], at[28]);       MULADD(at[5], at[27]);       MULADD(at[6], at[26]);       MULADD(at[7], at[25]);       MULADD(at[8], at[24]);       MULADD(at[9], at[23]);       MULADD(at[10], at[22]);       MULADD(at[11], at[21]);       MULADD(at[12], at[20]);       MULADD(at[13], at[19]);       MULADD(at[14], at[18]);       MULADD(at[15], at[17]);
      COMBA_STORE(C->dp[16]);
      // 17
      COMBA_FORWARD;
      MULADD(at[2], at[31]);       MULADD(at[3], at[30]);       MULADD(at[4], at[29]);       MULADD(at[5], at[28]);       MULADD(at[6], at[27]);       MULADD(at[7], at[26]);       MULADD(at[8], at[25]);       MULADD(at[9], at[24]);       MULADD(at[10], at[23]);       MULADD(at[11], at[22]);       MULADD(at[12], at[21]);       MULADD(at[13], at[20]);       MULADD(at[14], at[19]);       MULADD(at[15], at[18]);
      COMBA_STORE(C->dp[17]);
      // 18
      COMBA_FORWARD;
      MULADD(at[3], at[31]);       MULADD(at[4], at[30]);       MULADD(at[5], at[29]);       MULADD(at[6], at[28]);       MULADD(at[7], at[27]);       MULADD(at[8], at[26]);       MULADD(at[9], at[25]);       MULADD(at[10], at[24]);       MULADD(at[11], at[23]);       MULADD(at[12], at[22]);       MULADD(at[13], at[21]);       MULADD(at[14], at[20]);       MULADD(at[15], at[19]);
      COMBA_STORE(C->dp[18]);
      // 19
      COMBA_FORWARD;
      MULADD(at[4], at[31]);       MULADD(at[5], at[30]);       MULADD(at[6], at[29]);       MULADD(at[7], at[28]);       MULADD(at[8], at[27]);       MULADD(at[9], at[26]);       MULADD(at[10], at[25]);       MULADD(at[11], at[24]);       MULADD(at[12], at[23]);       MULADD(at[13], at[22]);       MULADD(at[14], at[21]);       MULADD(at[15], at[20]);
      COMBA_STORE(C->dp[19]);
      // 20
      COMBA_FORWARD;
      MULADD(at[5], at[31]);       MULADD(at[6], at[30]);       MULADD(at[7], at[29]);       MULADD(at[8], at[28]);       MULADD(at[9], at[27]);       MULADD(at[10], at[26]);       MULADD(at[11], at[25]);       MULADD(at[12], at[24]);       MULADD(at[13], at[23]);       MULADD(at[14], at[22]);       MULADD(at[15], at[21]);
      COMBA_STORE(C->dp[20]);
      // 21
      COMBA_FORWARD;
      MULADD(at[6], at[31]);       MULADD(at[7], at[30]);       MULADD(at[8], at[29]);       MULADD(at[9], at[28]);       MULADD(at[10], at[27]);       MULADD(at[11], at[26]);       MULADD(at[12], at[25]);       MULADD(at[13], at[24]);       MULADD(at[14], at[23]);       MULADD(at[15], at[22]);
      COMBA_STORE(C->dp[21]);
      // 22
      COMBA_FORWARD;
      MULADD(at[7], at[31]);       MULADD(at[8], at[30]);       MULADD(at[9], at[29]);       MULADD(at[10], at[28]);       MULADD(at[11], at[27]);       MULADD(at[12], at[26]);       MULADD(at[13], at[25]);       MULADD(at[14], at[24]);       MULADD(at[15], at[23]);
      COMBA_STORE(C->dp[22]);
      // 23
      COMBA_FORWARD;
      MULADD(at[8], at[31]);       MULADD(at[9], at[30]);       MULADD(at[10], at[29]);       MULADD(at[11], at[28]);       MULADD(at[12], at[27]);       MULADD(at[13], at[26]);       MULADD(at[14], at[25]);       MULADD(at[15], at[24]);
      COMBA_STORE(C->dp[23]);
      // 24
      COMBA_FORWARD;
      MULADD(at[9], at[31]);       MULADD(at[10], at[30]);       MULADD(at[11], at[29]);       MULADD(at[12], at[28]);       MULADD(at[13], at[27]);       MULADD(at[14], at[26]);       MULADD(at[15], at[25]);
      COMBA_STORE(C->dp[24]);
      // 25
      COMBA_FORWARD;
      MULADD(at[10], at[31]);       MULADD(at[11], at[30]);       MULADD(at[12], at[29]);       MULADD(at[13], at[28]);       MULADD(at[14], at[27]);       MULADD(at[15], at[26]);
      COMBA_STORE(C->dp[25]);
      // 26
      COMBA_FORWARD;
      MULADD(at[11], at[31]);       MULADD(at[12], at[30]);       MULADD(at[13], at[29]);       MULADD(at[14], at[28]);       MULADD(at[15], at[27]);
      COMBA_STORE(C->dp[26]);
      // 27
      COMBA_FORWARD;
      MULADD(at[12], at[31]);       MULADD(at[13], at[30]);       MULADD(at[14], at[29]);       MULADD(at[15], at[28]);
      COMBA_STORE(C->dp[27]);
      // 28
      COMBA_FORWARD;
      MULADD(at[13], at[31]);       MULADD(at[14], at[30]);       MULADD(at[15], at[29]);
      COMBA_STORE(C->dp[28]);
      // 29
      COMBA_FORWARD;
      MULADD(at[14], at[31]);       MULADD(at[15], at[30]);
      COMBA_STORE(C->dp[29]);
      // 30
      COMBA_FORWARD;
      MULADD(at[15], at[31]);
      COMBA_STORE(C->dp[30]);
      COMBA_STORE2(C->dp[31]);
      C->used = 32;
      C->sign = A->sign ^ B->sign;
      fp_clamp(C);
      COMBA_FINI;
      break;
   }
}
*/
// c = a * b
void fp_mul_d(fp_int *a, fp_digit b, fp_int *c) {
   fp_word  w;
   int      x, oldused;

   oldused = c->used;
   c->used = a->used;
   c->sign = a->sign;
   w       = 0;
   for (x = 0; x < a->used; x++) {
       w         = ((fp_word)a->dp[x]) * ((fp_word)b) + w;
       c->dp[x]  = (fp_digit)w;
       w         = w >> DIGIT_BIT;
   }
   if (w != 0 && (a->used != FP_SIZE)) {
      c->dp[c->used++] = w;
      ++x;
   }
   for (; x < oldused; x++) {
      c->dp[x] = 0;
   }
   fp_clamp(c);
}
/*
// d = a * b (mod c)
int fp_mulmod(fp_int *a, fp_int *b, fp_int *c, fp_int *d) {
  fp_int tmp;
  fp_zero(&tmp);
  fp_mul(a, b, &tmp);
  return fp_mod(&tmp, c, d);
}
*/
// c = (a, b)
void fp_gcd(fp_int *a, fp_int *b, fp_int *c) {
   fp_int u, v, r;

   // either zero than gcd is the largest
   if (fp_iszero (a) == 1 && fp_iszero (b) == 0) {
     fp_abs (b, c);
     return;
   }
   if (fp_iszero (a) == 0 && fp_iszero (b) == 1) {
     fp_abs (a, c);
     return;
   }

   // optimized.  At this point if a == 0 then b must equal zero too
   if (fp_iszero (a) == 1) {
     fp_zero(c);
     return;
   }

   // sort inputs
   if (fp_cmp_mag(a, b) != FP_LT) {
      fp_init_copy(&u, a);
      fp_init_copy(&v, b);
   } else {
      fp_init_copy(&u, b);
      fp_init_copy(&v, a);
   }

   fp_zero(&r);
   while (fp_iszero(&v) == FP_NO) {
      fp_mod(&u, &v, &r);
      fp_copy(&v, &u);
      fp_copy(&r, &v);
   }
   fp_copy(&u, c);
}
/*
static int fp_invmod_slow (fp_int * a, fp_int * b, fp_int * c) {
  fp_int  x, y, u, v, A, B, C, D;
  int     res;

  // b cannot be negative
  if (b->sign == FP_NEG || fp_iszero(b) == 1) {
    return FP_VAL;
  }

  // init temps
  fp_init(&x);    fp_init(&y);
  fp_init(&u);    fp_init(&v);
  fp_init(&A);    fp_init(&B);
  fp_init(&C);    fp_init(&D);

  // x = a, y = b
  if ((res = fp_mod(a, b, &x)) != FP_OKAY) {
      return res;
  }
  fp_copy(b, &y);

  // 2. [modified] if x,y are both even then return an error!
  if (fp_iseven (&x) == 1 && fp_iseven (&y) == 1) {
    return FP_VAL;
  }

  // 3. u=x, v=y, A=1, B=0, C=0,D=1
  fp_copy (&x, &u);
  fp_copy (&y, &v);
  fp_set (&A, 1);
  fp_set (&D, 1);

top:
  // 4.  while u is even do
  while (fp_iseven (&u) == 1) {
    // 4.1 u = u/2
    fp_div_2 (&u, &u);

    // 4.2 if A or B is odd then
    if (fp_isodd (&A) == 1 || fp_isodd (&B) == 1) {
      // A = (A+y)/2, B = (B-x)/2
      fp_add (&A, &y, &A);
      fp_sub (&B, &x, &B);
    }
    // A = A/2, B = B/2
    fp_div_2 (&A, &A);
    fp_div_2 (&B, &B);
  }

  // 5.  while v is even do
  while (fp_iseven (&v) == 1) {
    // 5.1 v = v/2
    fp_div_2 (&v, &v);

    // 5.2 if C or D is odd then
    if (fp_isodd (&C) == 1 || fp_isodd (&D) == 1) {
      // C = (C+y)/2, D = (D-x)/2
      fp_add (&C, &y, &C);
      fp_sub (&D, &x, &D);
    }
    // C = C/2, D = D/2
    fp_div_2 (&C, &C);
    fp_div_2 (&D, &D);
  }

  // 6.  if u >= v then
  if (fp_cmp (&u, &v) != FP_LT) {
    // u = u - v, A = A - C, B = B - D
    fp_sub (&u, &v, &u);
    fp_sub (&A, &C, &A);
    fp_sub (&B, &D, &B);
  } else {
    // v - v - u, C = C - A, D = D - B
    fp_sub (&v, &u, &v);
    fp_sub (&C, &A, &C);
    fp_sub (&D, &B, &D);
  }

  // if not zero goto step 4
  if (fp_iszero (&u) == 0)
    goto top;

  // now a = C, b = D, gcd == g*v

  // if v != 1 then there is no inverse
  if (fp_cmp_d (&v, 1) != FP_EQ) {
    return FP_VAL;
  }

  // if its too low
  while (fp_cmp_d(&C, 0) == FP_LT) {
      fp_add(&C, b, &C);
  }

  // too big
  while (fp_cmp_mag(&C, b) != FP_LT) {
      fp_sub(&C, b, &C);
  }

  // C is now the inverse
  fp_copy(&C, c);
  return FP_OKAY;
}

// c = 1/a (mod b) for odd b only
int fp_invmod(fp_int *a, fp_int *b, fp_int *c) {
  fp_int  x, y, u, v, B, D;
  int     neg;

  // 2. [modified] b must be odd
  if (fp_iseven (b) == FP_YES) {
    return fp_invmod_slow(a,b,c);
  }

  // init all our temps
  fp_init(&x);  fp_init(&y);
  fp_init(&u);  fp_init(&v);
  fp_init(&B);  fp_init(&D);

  // x == modulus, y == value to invert
  fp_copy(b, &x);

  // we need y = |a|
  fp_abs(a, &y);

  // 3. u=x, v=y, A=1, B=0, C=0,D=1
  fp_copy(&x, &u);
  fp_copy(&y, &v);
  fp_set (&D, 1);

top:
  // 4.  while u is even do
  while (fp_iseven (&u) == FP_YES) {
    // 4.1 u = u/2
    fp_div_2 (&u, &u);

    // 4.2 if B is odd then
    if (fp_isodd (&B) == FP_YES) {
      fp_sub (&B, &x, &B);
    }
    // B = B/2
    fp_div_2 (&B, &B);
  }

  // 5.  while v is even do
  while (fp_iseven (&v) == FP_YES) {
    // 5.1 v = v/2
    fp_div_2 (&v, &v);

    // 5.2 if D is odd then
    if (fp_isodd (&D) == FP_YES) {
      // D = (D-x)/2
      fp_sub (&D, &x, &D);
    }
    // D = D/2
    fp_div_2 (&D, &D);
  }

  // 6.  if u >= v then
  if (fp_cmp (&u, &v) != FP_LT) {
    // u = u - v, B = B - D
    fp_sub (&u, &v, &u);
    fp_sub (&B, &D, &B);
  } else {
    // v - v - u, D = D - B
    fp_sub (&v, &u, &v);
    fp_sub (&D, &B, &D);
  }

  // if not zero goto step 4
  if (fp_iszero (&u) == FP_NO) {
    goto top;
  }

  // now a = C, b = D, gcd == g*v

  // if v != 1 then there is no inverse
  if (fp_cmp_d (&v, 1) != FP_EQ) {
    return FP_VAL;
  }

  // b is now the inverse
  neg = a->sign;
  while (D.sign == FP_NEG) {
    fp_add (&D, b, &D);
  }
  fp_copy (&D, c);
  c->sign = neg;
  return FP_OKAY;
}

int fp_isprime(fp_int *a) {
  return fp_isprime_ex(a, 8);
}

// a few primes
static const fp_digit primes[FP_PRIME_SIZE] = {
  0x0002, 0x0003, 0x0005, 0x0007, 0x000B, 0x000D, 0x0011, 0x0013,
  0x0017, 0x001D, 0x001F, 0x0025, 0x0029, 0x002B, 0x002F, 0x0035,
  0x003B, 0x003D, 0x0043, 0x0047, 0x0049, 0x004F, 0x0053, 0x0059,
  0x0061, 0x0065, 0x0067, 0x006B, 0x006D, 0x0071, 0x007F, 0x0083,
  0x0089, 0x008B, 0x0095, 0x0097, 0x009D, 0x00A3, 0x00A7, 0x00AD,
  0x00B3, 0x00B5, 0x00BF, 0x00C1, 0x00C5, 0x00C7, 0x00D3, 0x00DF,
  0x00E3, 0x00E5, 0x00E9, 0x00EF, 0x00F1, 0x00FB, 0x0101, 0x0107,
  0x010D, 0x010F, 0x0115, 0x0119, 0x011B, 0x0125, 0x0133, 0x0137,

  0x0139, 0x013D, 0x014B, 0x0151, 0x015B, 0x015D, 0x0161, 0x0167,
  0x016F, 0x0175, 0x017B, 0x017F, 0x0185, 0x018D, 0x0191, 0x0199,
  0x01A3, 0x01A5, 0x01AF, 0x01B1, 0x01B7, 0x01BB, 0x01C1, 0x01C9,
  0x01CD, 0x01CF, 0x01D3, 0x01DF, 0x01E7, 0x01EB, 0x01F3, 0x01F7,
  0x01FD, 0x0209, 0x020B, 0x021D, 0x0223, 0x022D, 0x0233, 0x0239,
  0x023B, 0x0241, 0x024B, 0x0251, 0x0257, 0x0259, 0x025F, 0x0265,
  0x0269, 0x026B, 0x0277, 0x0281, 0x0283, 0x0287, 0x028D, 0x0293,
  0x0295, 0x02A1, 0x02A5, 0x02AB, 0x02B3, 0x02BD, 0x02C5, 0x02CF,

  0x02D7, 0x02DD, 0x02E3, 0x02E7, 0x02EF, 0x02F5, 0x02F9, 0x0301,
  0x0305, 0x0313, 0x031D, 0x0329, 0x032B, 0x0335, 0x0337, 0x033B,
  0x033D, 0x0347, 0x0355, 0x0359, 0x035B, 0x035F, 0x036D, 0x0371,
  0x0373, 0x0377, 0x038B, 0x038F, 0x0397, 0x03A1, 0x03A9, 0x03AD,
  0x03B3, 0x03B9, 0x03C7, 0x03CB, 0x03D1, 0x03D7, 0x03DF, 0x03E5,
  0x03F1, 0x03F5, 0x03FB, 0x03FD, 0x0407, 0x0409, 0x040F, 0x0419,
  0x041B, 0x0425, 0x0427, 0x042D, 0x043F, 0x0443, 0x0445, 0x0449,
  0x044F, 0x0455, 0x045D, 0x0463, 0x0469, 0x047F, 0x0481, 0x048B,

  0x0493, 0x049D, 0x04A3, 0x04A9, 0x04B1, 0x04BD, 0x04C1, 0x04C7,
  0x04CD, 0x04CF, 0x04D5, 0x04E1, 0x04EB, 0x04FD, 0x04FF, 0x0503,
  0x0509, 0x050B, 0x0511, 0x0515, 0x0517, 0x051B, 0x0527, 0x0529,
  0x052F, 0x0551, 0x0557, 0x055D, 0x0565, 0x0577, 0x0581, 0x058F,
  0x0593, 0x0595, 0x0599, 0x059F, 0x05A7, 0x05AB, 0x05AD, 0x05B3,
  0x05BF, 0x05C9, 0x05CB, 0x05CF, 0x05D1, 0x05D5, 0x05DB, 0x05E7,
  0x05F3, 0x05FB, 0x0607, 0x060D, 0x0611, 0x0617, 0x061F, 0x0623,
  0x062B, 0x062F, 0x063D, 0x0641, 0x0647, 0x0649, 0x064D, 0x0653
};

int fp_isprime_ex(fp_int *a, int t) {
   fp_int   b;
   fp_digit d;
   int      r, res;

   if (t <= 0 || t > FP_PRIME_SIZE) {
     return FP_NO;
   }

   // do trial division
   for (r = 0; r < 256; r++) {
       fp_mod_d(a, primes[r], &d);
       if (d == 0) {
          return FP_NO;
       }
   }

   // now do 't' miller rabins
   fp_init(&b);
   for (r = 0; r < t; r++) {
       fp_set(&b, primes[r]);
       fp_prime_miller_rabin(a, &b, &res);
       if (res == FP_NO) {
          return FP_NO;
       }
   }
   return FP_YES;
}
*/
// c = [a, b]
void fp_lcm(fp_int *a, fp_int *b, fp_int *c) {
   fp_int  t1, t2;

   fp_init(&t1);
   fp_init(&t2);
   fp_gcd(a, b, &t1);
   if (fp_cmp_mag(a, b) == FP_GT) {
      fp_div(a, &t1, &t2, NULL);
      fp_mul(b, &t2, c);
   } else {
      fp_div(b, &t1, &t2, NULL);
      fp_mul(a, &t2, c);
   }
}
/*
// Miller-Rabin test of "a" to the base of "b" as described in
// HAC pp. 139 Algorithm 4.24
//
// Sets result to 0 if definitely composite or 1 if probably prime.
// Randomly the chance of error is no more than 1/4 and often
// very much lower.
void fp_prime_miller_rabin (fp_int * a, fp_int * b, int *result) {
  fp_int  n1, y, r;
  int     s, j;

  // default
  *result = FP_NO;

  // ensure b > 1
  if (fp_cmp_d(b, 1) != FP_GT) {
     return;
  }

  // get n1 = a - 1
  fp_init_copy(&n1, a);
  fp_sub_d(&n1, 1, &n1);

  // set 2**s * r = n1
  fp_init_copy(&r, &n1);

  // count the number of least significant bits which are zero

  s = fp_cnt_lsb(&r);

  // now divide n - 1 by 2**s
  fp_div_2d (&r, s, &r, NULL);

  // compute y = b**r mod a
  fp_init(&y);
  fp_exptmod(b, &r, a, &y);

  // if y != 1 and y != n1 do
  if (fp_cmp_d (&y, 1) != FP_EQ && fp_cmp (&y, &n1) != FP_EQ) {
    j = 1;
    // while j <= s-1 and y != n1
    while ((j <= (s - 1)) && fp_cmp (&y, &n1) != FP_EQ) {
      fp_sqrmod (&y, a, &y);

      // if y == 1 then composite
      if (fp_cmp_d (&y, 1) == FP_EQ) {
         return;
      }
      ++j;
    }

    // if y != n1 then composite
    if (fp_cmp (&y, &n1) != FP_EQ) {
       return;
    }
  }

  // probably prime now
  *result = FP_YES;
}

// This is possibly the mother of all prime generation functions, muahahahahaha!
int fp_prime_random_ex(fp_int *a, int t, int size, int flags, tfm_prime_callback cb, void *dat) {
   unsigned char *tmp, maskAND, maskOR_msb, maskOR_lsb;
   int res, err, bsize, maskOR_msb_offset;

   // sanity check the input
   if (size <= 1 || cb == NULL || t <= 0 || t > FP_PRIME_SIZE) {
      return FP_VAL;
   }

   // TFM_PRIME_SAFE implies TFM_PRIME_BBS
   if (flags & TFM_PRIME_SAFE) {
      flags |= TFM_PRIME_BBS;
   }

   // calc the byte size
   bsize = (size>>3)+(size&7?1:0);

   // we need a buffer of bsize bytes
   tmp = malloc(bsize);
   if (tmp == NULL) {
      return FP_MEM;
   }

   // calc the maskAND value for the MSbyte
   maskAND = 0xFF >> ((8 - (size & 7)) & 7);

   // calc the maskOR_msb
   maskOR_msb        = 0;
   maskOR_msb_offset = (size - 2) >> 3;
   if (flags & TFM_PRIME_2MSB_ON) {
      maskOR_msb     |= 1 << ((size - 2) & 7);
   } else if (flags & TFM_PRIME_2MSB_OFF) {
      maskAND        &= ~(1 << ((size - 2) & 7));
   }

   // get the maskOR_lsb
   maskOR_lsb         = 1;
   if (flags & TFM_PRIME_BBS) {
      maskOR_lsb     |= 3;
   }

   do {
      // read the bytes
      if (cb(tmp, bsize, dat) != bsize) {
         err = FP_VAL;
         goto error;
      }

      // work over the MSbyte
      tmp[0]    &= maskAND;
      tmp[0]    |= 1 << ((size - 1) & 7);

      // mix in the maskORs
      tmp[maskOR_msb_offset]   |= maskOR_msb;
      tmp[bsize-1]             |= maskOR_lsb;

      // read it in
      fp_read_unsigned_bin(a, tmp, bsize);

      // is it prime?
      res = fp_isprime_ex(a, t);
      if (res == FP_NO) continue;

      if (flags & TFM_PRIME_SAFE) {
         // see if (a-1)/2 is prime
         fp_sub_d(a, 1, a);
         fp_div_2(a, a);

         // is it prime?
         res = fp_isprime_ex(a, t);
      }
   } while (res == FP_NO);

   if (flags & TFM_PRIME_SAFE) {
      // restore a to the original value
      fp_mul_2(a, a);
      fp_add_d(a, 1, a);
   }

   err = FP_OKAY;
error:
   free(tmp);
   return err;
}
*/
// b = a*a
void fp_sqr(fp_int *A, fp_int *B) {
    int     y, old_used;

    old_used = B->used;

    // call generic if we're out of range
    if (A->used + A->used > FP_SIZE) {
       fp_sqr_comba(A, B);
       goto clean;
    }

    y = A->used;
    if (y <= 16) {
      fp_sqr_comba_small(A,B);
      goto clean;
    }
    fp_sqr_comba(A, B);
clean:
    for (y = B->used; y < old_used; y++) {
       B->dp[y] = 0;
    }
}

#if defined(TFM_PRESCOTT) && defined(TFM_SSE2)
   #undef TFM_SSE2
   #define TFM_X86
#endif

#if defined(TFM_X86)

// x86-32 optimized

#define COMBA_START

#define CLEAR_CARRY \
   c0 = c1 = c2 = 0;

#define COMBA_STORE(x) \
   x = c0;

#define COMBA_STORE2(x) \
   x = c1;

#define CARRY_FORWARD \
   do { c0 = c1; c1 = c2; c2 = 0; } while (0);

#define COMBA_FINI

#define SQRADD(i, j)                                      \
asm(                                            \
     "movl  %6,%%eax     \n\t"                            \
     "mull  %%eax        \n\t"                            \
     "addl  %%eax,%0     \n\t"                            \
     "adcl  %%edx,%1     \n\t"                            \
     "adcl  $0,%2        \n\t"                            \
     :"=r"(c0), "=r"(c1), "=r"(c2): "0"(c0), "1"(c1), "2"(c2), "m"(i) :"%eax","%edx","cc");

#define SQRADD2(i, j)                                     \
asm(                                            \
     "movl  %6,%%eax     \n\t"                            \
     "mull  %7           \n\t"                            \
     "addl  %%eax,%0     \n\t"                            \
     "adcl  %%edx,%1     \n\t"                            \
     "adcl  $0,%2        \n\t"                            \
     "addl  %%eax,%0     \n\t"                            \
     "adcl  %%edx,%1     \n\t"                            \
     "adcl  $0,%2        \n\t"                            \
     :"=r"(c0), "=r"(c1), "=r"(c2): "0"(c0), "1"(c1), "2"(c2), "m"(i), "m"(j)  :"%eax","%edx","cc");

#define SQRADDSC(i, j)                                    \
asm(                                                     \
     "movl  %3,%%eax     \n\t"                            \
     "mull  %4           \n\t"                            \
     "movl  %%eax,%0     \n\t"                            \
     "movl  %%edx,%1     \n\t"                            \
     "xorl  %2,%2        \n\t"                            \
     :"=r"(sc0), "=r"(sc1), "=r"(sc2): "g"(i), "g"(j) :"%eax","%edx","cc");

#define SQRADDAC(i, j)                                    \
asm(                                                     \
     "movl  %6,%%eax     \n\t"                            \
     "mull  %7           \n\t"                            \
     "addl  %%eax,%0     \n\t"                            \
     "adcl  %%edx,%1     \n\t"                            \
     "adcl  $0,%2        \n\t"                            \
     :"=r"(sc0), "=r"(sc1), "=r"(sc2): "0"(sc0), "1"(sc1), "2"(sc2), "g"(i), "g"(j) :"%eax","%edx","cc");

#define SQRADDDB                                          \
asm(                                                     \
     "addl %6,%0         \n\t"                            \
     "adcl %7,%1         \n\t"                            \
     "adcl %8,%2         \n\t"                            \
     "addl %6,%0         \n\t"                            \
     "adcl %7,%1         \n\t"                            \
     "adcl %8,%2         \n\t"                            \
     :"=r"(c0), "=r"(c1), "=r"(c2) : "0"(c0), "1"(c1), "2"(c2), "r"(sc0), "r"(sc1), "r"(sc2) : "cc");

#elif defined(TFM_X86_64)
// x86-64 optimized

#define COMBA_START

#define CLEAR_CARRY \
   c0 = c1 = c2 = 0;

#define COMBA_STORE(x) \
   x = c0;

#define COMBA_STORE2(x) \
   x = c1;

#define CARRY_FORWARD \
   do { c0 = c1; c1 = c2; c2 = 0; } while (0);

#define COMBA_FINI

#define SQRADD(i, j)                                      \
asm(                                                     \
     "movq  %6,%%rax     \n\t"                            \
     "mulq  %%rax        \n\t"                            \
     "addq  %%rax,%0     \n\t"                            \
     "adcq  %%rdx,%1     \n\t"                            \
     "adcq  $0,%2        \n\t"                            \
     :"=r"(c0), "=r"(c1), "=r"(c2): "0"(c0), "1"(c1), "2"(c2), "x"(i) :"%rax","%rdx","cc");

#define SQRADD2(i, j)                                     \
asm(                                                     \
     "movq  %6,%%rax     \n\t"                            \
     "mulq  %7           \n\t"                            \
     "addq  %%rax,%0     \n\t"                            \
     "adcq  %%rdx,%1     \n\t"                            \
     "adcq  $0,%2        \n\t"                            \
     "addq  %%rax,%0     \n\t"                            \
     "adcq  %%rdx,%1     \n\t"                            \
     "adcq  $0,%2        \n\t"                            \
     :"=r"(c0), "=r"(c1), "=r"(c2): "0"(c0), "1"(c1), "2"(c2), "g"(i), "g"(j)  :"%rax","%rdx","cc");

#define SQRADDSC(i, j)                                    \
asm(                                                     \
     "movq  %3,%%rax     \n\t"                            \
     "mulq  %4           \n\t"                            \
     "movq  %%rax,%0     \n\t"                            \
     "movq  %%rdx,%1     \n\t"                            \
     "xorq  %2,%2        \n\t"                            \
     :"=r"(sc0), "=r"(sc1), "=r"(sc2): "g"(i), "g"(j) :"%rax","%rdx","cc");

#define SQRADDAC(i, j)                                                         \
asm(                                                     \
     "movq  %6,%%rax     \n\t"                            \
     "mulq  %7           \n\t"                            \
     "addq  %%rax,%0     \n\t"                            \
     "adcq  %%rdx,%1     \n\t"                            \
     "adcq  $0,%2        \n\t"                            \
     :"=r"(sc0), "=r"(sc1), "=r"(sc2): "0"(sc0), "1"(sc1), "2"(sc2), "g"(i), "g"(j) :"%rax","%rdx","cc");

#define SQRADDDB                                          \
asm(                                                     \
     "addq %6,%0         \n\t"                            \
     "adcq %7,%1         \n\t"                            \
     "adcq %8,%2         \n\t"                            \
     "addq %6,%0         \n\t"                            \
     "adcq %7,%1         \n\t"                            \
     "adcq %8,%2         \n\t"                            \
     :"=r"(c0), "=r"(c1), "=r"(c2) : "0"(c0), "1"(c1), "2"(c2), "r"(sc0), "r"(sc1), "r"(sc2) : "cc");

#elif defined(TFM_SSE2)

// SSE2 Optimized
#define COMBA_START

#define CLEAR_CARRY \
   c0 = c1 = c2 = 0;

#define COMBA_STORE(x) \
   x = c0;

#define COMBA_STORE2(x) \
   x = c1;

#define CARRY_FORWARD \
   do { c0 = c1; c1 = c2; c2 = 0; } while (0);

#define COMBA_FINI \
   asm("emms");

#define SQRADD(i, j)                                      \
asm(                                            \
     "movd  %6,%%mm0     \n\t"                            \
     "pmuludq %%mm0,%%mm0\n\t"                            \
     "movd  %%mm0,%%eax  \n\t"                            \
     "psrlq $32,%%mm0    \n\t"                            \
     "addl  %%eax,%0     \n\t"                            \
     "movd  %%mm0,%%eax  \n\t"                            \
     "adcl  %%eax,%1     \n\t"                            \
     "adcl  $0,%2        \n\t"                            \
     :"=r"(c0), "=r"(c1), "=r"(c2): "0"(c0), "1"(c1), "2"(c2), "m"(i) :"%eax","cc");

#define SQRADD2(i, j)                                     \
asm(                                            \
     "movd  %6,%%mm0     \n\t"                            \
     "movd  %7,%%mm1     \n\t"                            \
     "pmuludq %%mm1,%%mm0\n\t"                            \
     "movd  %%mm0,%%eax  \n\t"                            \
     "psrlq $32,%%mm0    \n\t"                            \
     "movd  %%mm0,%%edx  \n\t"                            \
     "addl  %%eax,%0     \n\t"                            \
     "adcl  %%edx,%1     \n\t"                            \
     "adcl  $0,%2        \n\t"                            \
     "addl  %%eax,%0     \n\t"                            \
     "adcl  %%edx,%1     \n\t"                            \
     "adcl  $0,%2        \n\t"                            \
     :"=r"(c0), "=r"(c1), "=r"(c2): "0"(c0), "1"(c1), "2"(c2), "m"(i), "m"(j)  :"%eax","%edx","cc");

#define SQRADDSC(i, j)                                                         \
asm(                                            \
     "movd  %6,%%mm0     \n\t"                            \
     "movd  %7,%%mm1     \n\t"                            \
     "pmuludq %%mm1,%%mm0\n\t"                            \
     "movd  %%mm0,%0     \n\t"                            \
     "psrlq $32,%%mm0    \n\t"                            \
     "movd  %%mm0,%1     \n\t"                            \
     "xorl  %2,%2        \n\t"                            \
     :"=r"(sc0), "=r"(sc1), "=r"(sc2): "0"(sc0), "1"(sc1), "2"(sc2), "m"(i), "m"(j));

#define SQRADDAC(i, j)                                                         \
asm(                                            \
     "movd  %6,%%mm0     \n\t"                            \
     "movd  %7,%%mm1     \n\t"                            \
     "pmuludq %%mm1,%%mm0\n\t"                            \
     "movd  %%mm0,%%eax  \n\t"                            \
     "psrlq $32,%%mm0    \n\t"                            \
     "movd  %%mm0,%%edx  \n\t"                            \
     "addl  %%eax,%0     \n\t"                            \
     "adcl  %%edx,%1     \n\t"                            \
     "adcl  $0,%2        \n\t"                            \
     :"=r"(sc0), "=r"(sc1), "=r"(sc2): "0"(sc0), "1"(sc1), "2"(sc2), "m"(i), "m"(j)  :"%eax","%edx","cc");

#define SQRADDDB                                          \
asm(                                                     \
     "addl %6,%0         \n\t"                            \
     "adcl %7,%1         \n\t"                            \
     "adcl %8,%2         \n\t"                            \
     "addl %6,%0         \n\t"                            \
     "adcl %7,%1         \n\t"                            \
     "adcl %8,%2         \n\t"                            \
     :"=r"(c0), "=r"(c1), "=r"(c2) : "0"(c0), "1"(c1), "2"(c2), "r"(sc0), "r"(sc1), "r"(sc2) : "cc");

#elif defined(TFM_ARM)

// ARM code

#define COMBA_START

#define CLEAR_CARRY \
   c0 = c1 = c2 = 0;

#define COMBA_STORE(x) \
   x = c0;

#define COMBA_STORE2(x) \
   x = c1;

#define CARRY_FORWARD \
   do { c0 = c1; c1 = c2; c2 = 0; } while (0);

#define COMBA_FINI

// multiplies point i and j, updates carry "c1" and digit c2
#define SQRADD(i, j)                                             \
asm(                                                             \
"  UMULL  r0,r1,%6,%6              \n\t"                         \
"  ADDS   %0,%0,r0                 \n\t"                         \
"  ADCS   %1,%1,r1                 \n\t"                         \
"  ADC    %2,%2,#0                 \n\t"                         \
:"=r"(c0), "=r"(c1), "=r"(c2) : "0"(c0), "1"(c1), "2"(c2), "r"(i) : "r0", "r1", "cc");

// for squaring some of the terms are doubled...
#define SQRADD2(i, j)                                            \
asm(                                                             \
"  UMULL  r0,r1,%6,%7              \n\t"                         \
"  ADDS   %0,%0,r0                 \n\t"                         \
"  ADCS   %1,%1,r1                 \n\t"                         \
"  ADC    %2,%2,#0                 \n\t"                         \
"  ADDS   %0,%0,r0                 \n\t"                         \
"  ADCS   %1,%1,r1                 \n\t"                         \
"  ADC    %2,%2,#0                 \n\t"                         \
:"=r"(c0), "=r"(c1), "=r"(c2) : "0"(c0), "1"(c1), "2"(c2), "r"(i), "r"(j) : "r0", "r1", "cc");

#define SQRADDSC(i, j)                                           \
asm(                                                             \
"  UMULL  %0,%1,%6,%7              \n\t"                         \
"  SUB    %2,%2,%2                 \n\t"                         \
:"=r"(sc0), "=r"(sc1), "=r"(sc2) : "0"(sc0), "1"(sc1), "2"(sc2), "r"(i), "r"(j) : "cc");

#define SQRADDAC(i, j)                                           \
asm(                                                             \
"  UMULL  r0,r1,%6,%7              \n\t"                         \
"  ADDS   %0,%0,r0                 \n\t"                         \
"  ADCS   %1,%1,r1                 \n\t"                         \
"  ADC    %2,%2,#0                 \n\t"                         \
:"=r"(sc0), "=r"(sc1), "=r"(sc2) : "0"(sc0), "1"(sc1), "2"(sc2), "r"(i), "r"(j) : "r0", "r1", "cc");

#define SQRADDDB                                                 \
asm(                                                             \
"  ADDS  %0,%0,%3                     \n\t"                      \
"  ADCS  %1,%1,%4                     \n\t"                      \
"  ADC   %2,%2,%5                     \n\t"                      \
"  ADDS  %0,%0,%3                     \n\t"                      \
"  ADCS  %1,%1,%4                     \n\t"                      \
"  ADC   %2,%2,%5                     \n\t"                      \
:"=r"(c0), "=r"(c1), "=r"(c2) : "r"(sc0), "r"(sc1), "r"(sc2), "0"(c0), "1"(c1), "2"(c2) : "cc");

#elif defined(TFM_PPC32)

// PPC32

#define COMBA_START

#define CLEAR_CARRY \
   c0 = c1 = c2 = 0;

#define COMBA_STORE(x) \
   x = c0;

#define COMBA_STORE2(x) \
   x = c1;

#define CARRY_FORWARD \
   do { c0 = c1; c1 = c2; c2 = 0; } while (0);

#define COMBA_FINI

// multiplies point i and j, updates carry "c1" and digit c2
#define SQRADD(i, j)             \
asm(                             \
   " mullw  16,%6,%6       \n\t" \
   " addc   %0,%0,16       \n\t" \
   " mulhwu 16,%6,%6       \n\t" \
   " adde   %1,%1,16       \n\t" \
   " addze  %2,%2          \n\t" \
:"=r"(c0), "=r"(c1), "=r"(c2):"0"(c0), "1"(c1), "2"(c2), "r"(i):"16","cc");

// for squaring some of the terms are doubled...
#define SQRADD2(i, j)            \
asm(                             \
   " mullw  16,%6,%7       \n\t" \
   " mulhwu 17,%6,%7       \n\t" \
   " addc   %0,%0,16       \n\t" \
   " adde   %1,%1,17       \n\t" \
   " addze  %2,%2          \n\t" \
   " addc   %0,%0,16       \n\t" \
   " adde   %1,%1,17       \n\t" \
   " addze  %2,%2          \n\t" \
:"=r"(c0), "=r"(c1), "=r"(c2):"0"(c0), "1"(c1), "2"(c2), "r"(i), "r"(j):"16", "17","cc");

#define SQRADDSC(i, j)            \
asm(                              \
   " mullw  %0,%6,%7        \n\t" \
   " mulhwu %1,%6,%7        \n\t" \
   " xor    %2,%2,%2        \n\t" \
:"=r"(sc0), "=r"(sc1), "=r"(sc2):"0"(sc0), "1"(sc1), "2"(sc2), "r"(i),"r"(j) : "cc");

#define SQRADDAC(i, j)           \
asm(                             \
   " mullw  16,%6,%7       \n\t" \
   " addc   %0,%0,16       \n\t" \
   " mulhwu 16,%6,%7       \n\t" \
   " adde   %1,%1,16       \n\t" \
   " addze  %2,%2          \n\t" \
:"=r"(sc0), "=r"(sc1), "=r"(sc2):"0"(sc0), "1"(sc1), "2"(sc2), "r"(i), "r"(j):"16", "cc");

#define SQRADDDB                  \
asm(                              \
   " addc   %0,%0,%3        \n\t" \
   " adde   %1,%1,%4        \n\t" \
   " adde   %2,%2,%5        \n\t" \
   " addc   %0,%0,%3        \n\t" \
   " adde   %1,%1,%4        \n\t" \
   " adde   %2,%2,%5        \n\t" \
:"=r"(c0), "=r"(c1), "=r"(c2) : "r"(sc0), "r"(sc1), "r"(sc2), "0"(c0), "1"(c1), "2"(c2) : "cc");

#elif defined(TFM_PPC64)
// PPC64

#define COMBA_START

#define CLEAR_CARRY \
   c0 = c1 = c2 = 0;

#define COMBA_STORE(x) \
   x = c0;

#define COMBA_STORE2(x) \
   x = c1;

#define CARRY_FORWARD \
   do { c0 = c1; c1 = c2; c2 = 0; } while (0);

#define COMBA_FINI

// multiplies point i and j, updates carry "c1" and digit c2
#define SQRADD(i, j)             \
asm(                             \
   " mulld  r16,%6,%6       \n\t" \
   " addc   %0,%0,r16       \n\t" \
   " mulhdu r16,%6,%6       \n\t" \
   " adde   %1,%1,r16       \n\t" \
   " addze  %2,%2          \n\t" \
:"=r"(c0), "=r"(c1), "=r"(c2):"0"(c0), "1"(c1), "2"(c2), "r"(i):"r16","cc");

// for squaring some of the terms are doubled...
#define SQRADD2(i, j)            \
asm(                             \
   " mulld  r16,%6,%7       \n\t" \
   " mulhdu r17,%6,%7       \n\t" \
   " addc   %0,%0,r16       \n\t" \
   " adde   %1,%1,r17       \n\t" \
   " addze  %2,%2          \n\t" \
   " addc   %0,%0,r16       \n\t" \
   " adde   %1,%1,r17       \n\t" \
   " addze  %2,%2          \n\t" \
:"=r"(c0), "=r"(c1), "=r"(c2):"0"(c0), "1"(c1), "2"(c2), "r"(i), "r"(j):"r16", "r17","cc");

#define SQRADDSC(i, j)            \
asm(                              \
   " mulld  %0,%6,%7        \n\t" \
   " mulhdu %1,%6,%7        \n\t" \
   " xor    %2,%2,%2        \n\t" \
:"=r"(sc0), "=r"(sc1), "=r"(sc2):"0"(sc0), "1"(sc1), "2"(sc2), "r"(i),"r"(j) : "cc");

#define SQRADDAC(i, j)           \
asm(                             \
   " mulld  r16,%6,%7       \n\t" \
   " addc   %0,%0,r16       \n\t" \
   " mulhdu r16,%6,%7       \n\t" \
   " adde   %1,%1,r16       \n\t" \
   " addze  %2,%2          \n\t" \
:"=r"(sc0), "=r"(sc1), "=r"(sc2):"0"(sc0), "1"(sc1), "2"(sc2), "r"(i), "r"(j):"r16", "cc");

#define SQRADDDB                  \
asm(                              \
   " addc   %0,%0,%3        \n\t" \
   " adde   %1,%1,%4        \n\t" \
   " adde   %2,%2,%5        \n\t" \
   " addc   %0,%0,%3        \n\t" \
   " adde   %1,%1,%4        \n\t" \
   " adde   %2,%2,%5        \n\t" \
:"=r"(c0), "=r"(c1), "=r"(c2) : "r"(sc0), "r"(sc1), "r"(sc2), "0"(c0), "1"(c1), "2"(c2) : "cc");


#elif defined(TFM_AVR32)

// AVR32

#define COMBA_START

#define CLEAR_CARRY \
   c0 = c1 = c2 = 0;

#define COMBA_STORE(x) \
   x = c0;

#define COMBA_STORE2(x) \
   x = c1;

#define CARRY_FORWARD \
   do { c0 = c1; c1 = c2; c2 = 0; } while (0);

#define COMBA_FINI

// multiplies point i and j, updates carry "c1" and digit c2
#define SQRADD(i, j)             \
asm(                             \
   " mulu.d r2,%6,%6       \n\t" \
   " add    %0,%0,r2       \n\t" \
   " adc    %1,%1,r3       \n\t" \
   " acr    %2             \n\t" \
:"=r"(c0), "=r"(c1), "=r"(c2):"0"(c0), "1"(c1), "2"(c2), "r"(i):"r2","r3");

// for squaring some of the terms are doubled...
#define SQRADD2(i, j)            \
asm(                             \
   " mulu.d r2,%6,%7       \n\t" \
   " add    %0,%0,r2       \n\t" \
   " adc    %1,%1,r3       \n\t" \
   " acr    %2,            \n\t" \
   " add    %0,%0,r2       \n\t" \
   " adc    %1,%1,r3       \n\t" \
   " acr    %2,            \n\t" \
:"=r"(c0), "=r"(c1), "=r"(c2):"0"(c0), "1"(c1), "2"(c2), "r"(i), "r"(j):"r2", "r3");

#define SQRADDSC(i, j)            \
asm(                              \
   " mulu.d r2,%6,%7        \n\t" \
   " mov    %0,r2           \n\t" \
   " mov    %1,r3           \n\t" \
   " eor    %2,%2           \n\t" \
:"=r"(sc0), "=r"(sc1), "=r"(sc2):"0"(sc0), "1"(sc1), "2"(sc2), "r"(i),"r"(j) : "r2", "r3");

#define SQRADDAC(i, j)           \
asm(                             \
   " mulu.d r2,%6,%7       \n\t" \
   " add    %0,%0,r2       \n\t" \
   " adc    %1,%1,r3       \n\t" \
   " acr    %2             \n\t" \
:"=r"(sc0), "=r"(sc1), "=r"(sc2):"0"(sc0), "1"(sc1), "2"(sc2), "r"(i), "r"(j):"r2", "r3");

#define SQRADDDB                  \
asm(                              \
   " add    %0,%0,%3        \n\t" \
   " adc    %1,%1,%4        \n\t" \
   " adc    %2,%2,%5        \n\t" \
   " add    %0,%0,%3        \n\t" \
   " adc    %1,%1,%4        \n\t" \
   " adc    %2,%2,%5        \n\t" \
:"=r"(c0), "=r"(c1), "=r"(c2) : "r"(sc0), "r"(sc1), "r"(sc2), "0"(c0), "1"(c1), "2"(c2) : "cc");

#elif defined(TFM_MIPS)

// MIPS

#define COMBA_START

#define CLEAR_CARRY \
   c0 = c1 = c2 = 0;

#define COMBA_STORE(x) \
   x = c0;

#define COMBA_STORE2(x) \
   x = c1;

#define CARRY_FORWARD \
   do { c0 = c1; c1 = c2; c2 = 0; } while (0);

#define COMBA_FINI

// multiplies point i and j, updates carry "c1" and digit c2
#define SQRADD(i, j)              \
asm(                              \
   " multu  %6,%6          \n\t"  \
   " mflo   $12            \n\t"  \
   " mfhi   $13            \n\t"  \
   " addu    %0,%0,$12     \n\t"  \
   " sltu   $12,%0,$12     \n\t"  \
   " addu    %1,%1,$13     \n\t"  \
   " sltu   $13,%1,$13     \n\t"  \
   " addu    %1,%1,$12     \n\t"  \
   " sltu   $12,%1,$12     \n\t"  \
   " addu    %2,%2,$13     \n\t"  \
   " addu    %2,%2,$12     \n\t"  \
:"=r"(c0), "=r"(c1), "=r"(c2):"0"(c0), "1"(c1), "2"(c2), "r"(i):"$12","$13");

// for squaring some of the terms are doubled...
#define SQRADD2(i, j)            \
asm(                             \
   " multu  %6,%7          \n\t" \
   " mflo   $12            \n\t" \
   " mfhi   $13            \n\t" \
                                 \
   " addu    %0,%0,$12     \n\t" \
   " sltu   $14,%0,$12     \n\t" \
   " addu    %1,%1,$13     \n\t" \
   " sltu   $15,%1,$13     \n\t" \
   " addu    %1,%1,$14     \n\t" \
   " sltu   $14,%1,$14     \n\t" \
   " addu    %2,%2,$15     \n\t" \
   " addu    %2,%2,$14     \n\t" \
                                 \
   " addu    %0,%0,$12     \n\t" \
   " sltu   $14,%0,$12     \n\t" \
   " addu    %1,%1,$13     \n\t" \
   " sltu   $15,%1,$13     \n\t" \
   " addu    %1,%1,$14     \n\t" \
   " sltu   $14,%1,$14     \n\t" \
   " addu    %2,%2,$15     \n\t" \
   " addu    %2,%2,$14     \n\t" \
:"=r"(c0), "=r"(c1), "=r"(c2):"0"(c0), "1"(c1), "2"(c2), "r"(i), "r"(j):"$12", "$13", "$14", "$15");

#define SQRADDSC(i, j)            \
asm(                              \
   " multu  %6,%7          \n\t"  \
   " mflo   %0             \n\t"  \
   " mfhi   %1             \n\t"  \
   " xor    %2,%2,%2       \n\t"  \
:"=r"(sc0), "=r"(sc1), "=r"(sc2):"0"(sc0), "1"(sc1), "2"(sc2), "r"(i),"r"(j) : "cc");

#define SQRADDAC(i, j)           \
asm(                             \
   " multu  %6,%7          \n\t" \
   " mflo   $12            \n\t" \
   " mfhi   $13            \n\t" \
   " addu    %0,%0,$12     \n\t" \
   " sltu   $12,%0,$12     \n\t" \
   " addu    %1,%1,$13     \n\t" \
   " sltu   $13,%1,$13     \n\t" \
   " addu    %1,%1,$12     \n\t" \
   " sltu   $12,%1,$12     \n\t" \
   " addu    %2,%2,$13     \n\t" \
   " addu    %2,%2,$12     \n\t" \
:"=r"(sc0), "=r"(sc1), "=r"(sc2):"0"(sc0), "1"(sc1), "2"(sc2), "r"(i), "r"(j):"$12", "$13", "$14");

#define SQRADDDB                  \
asm(                              \
   " addu    %0,%0,%3       \n\t" \
   " sltu   $10,%0,%3       \n\t" \
   " addu    %1,%1,$10      \n\t" \
   " sltu   $10,%1,$10      \n\t" \
   " addu    %1,%1,%4       \n\t" \
   " sltu   $11,%1,%4       \n\t" \
   " addu    %2,%2,$10      \n\t" \
   " addu    %2,%2,$11      \n\t" \
   " addu    %2,%2,%5       \n\t" \
                                  \
   " addu    %0,%0,%3       \n\t" \
   " sltu   $10,%0,%3       \n\t" \
   " addu    %1,%1,$10      \n\t" \
   " sltu   $10,%1,$10      \n\t" \
   " addu    %1,%1,%4       \n\t" \
   " sltu   $11,%1,%4       \n\t" \
   " addu    %2,%2,$10      \n\t" \
   " addu    %2,%2,$11      \n\t" \
   " addu    %2,%2,%5       \n\t" \
:"=r"(c0), "=r"(c1), "=r"(c2) : "r"(sc0), "r"(sc1), "r"(sc2), "0"(c0), "1"(c1), "2"(c2) : "$10", "$11");

#else

#define TFM_ISO

// ISO C portable code

#define COMBA_START

#define CLEAR_CARRY \
   c0 = c1 = c2 = 0;

#define COMBA_STORE(x) \
   x = c0;

#define COMBA_STORE2(x) \
   x = c1;

#define CARRY_FORWARD \
   do { c0 = c1; c1 = c2; c2 = 0; } while (0);

#define COMBA_FINI

// multiplies point i and j, updates carry "c1" and digit c2
#define SQRADD(i, j)                                 \
   do { fp_word t;                                   \
   t = c0 + ((fp_word)i) * ((fp_word)j);  c0 = t;    \
   t = c1 + (t >> DIGIT_BIT);             c1 = t; c2 += t >> DIGIT_BIT; \
   } while (0);


// for squaring some of the terms are doubled...
#define SQRADD2(i, j)                                                 \
   do { fp_word t;                                                    \
   t  = ((fp_word)i) * ((fp_word)j);                                  \
   tt = (fp_word)c0 + t;                 c0 = tt;                              \
   tt = (fp_word)c1 + (tt >> DIGIT_BIT); c1 = tt; c2 += tt >> DIGIT_BIT;       \
   tt = (fp_word)c0 + t;                 c0 = tt;                              \
   tt = (fp_word)c1 + (tt >> DIGIT_BIT); c1 = tt; c2 += tt >> DIGIT_BIT;       \
   } while (0);

#define SQRADDSC(i, j)                                                         \
   do { fp_word t;                                                             \
      t =  ((fp_word)i) * ((fp_word)j);                                        \
      sc0 = (fp_digit)t; sc1 = (t >> DIGIT_BIT); sc2 = 0;                      \
   } while (0);

#define SQRADDAC(i, j)                                                         \
   do { fp_word t;                                                             \
   t = sc0 + ((fp_word)i) * ((fp_word)j);  sc0 = t;                            \
   t = sc1 + (t >> DIGIT_BIT);             sc1 = t; sc2 += t >> DIGIT_BIT;     \
   } while (0);

#define SQRADDDB                                                               \
   do { fp_word t;                                                             \
   t = ((fp_word)sc0) + ((fp_word)sc0) + c0; c0 = t;                                                 \
   t = ((fp_word)sc1) + ((fp_word)sc1) + c1 + (t >> DIGIT_BIT); c1 = t;                              \
   c2 = c2 + ((fp_word)sc2) + ((fp_word)sc2) + (t >> DIGIT_BIT);                                     \
   } while (0);

#endif

#define TFM_DEFINES

// generic comba squarer
void fp_sqr_comba(fp_int *A, fp_int *B) {
  int       pa, ix, iz;
  fp_digit  c0, c1, c2;
  fp_int    tmp, *dst;
#ifdef TFM_ISO
  fp_word   tt;
#endif

  // get size of output and trim
  pa = A->used + A->used;
  if (pa >= FP_SIZE) {
     pa = FP_SIZE-1;
  }

  // number of output digits to produce
  COMBA_START;
  CLEAR_CARRY;

  if (A == B) {
     fp_zero(&tmp);
     dst = &tmp;
  } else {
     fp_zero(B);
     dst = B;
  }

  for (ix = 0; ix < pa; ix++) {
      int      tx, ty, iy;
      fp_digit *tmpy, *tmpx;

      // get offsets into the two bignums
      ty = MIN(A->used-1, ix);
      tx = ix - ty;

      // setup temp aliases
      tmpx = A->dp + tx;
      tmpy = A->dp + ty;

      // this is the number of times the loop will iterrate,
      // while (tx++ < a->used && ty-- >= 0) { ... }
      iy = MIN(A->used-tx, ty+1);

      // now for squaring tx can never equal ty
      // we halve the distance since they approach
      // at a rate of 2x and we have to round because
      // odd cases need to be executed
      iy = MIN(iy, (ty-tx+1)>>1);

      // forward carries
      CARRY_FORWARD;

      // execute loop
      for (iz = 0; iz < iy; iz++) {
          fp_digit _tmpx = *tmpx++;
          fp_digit _tmpy = *tmpy--;
          SQRADD2(_tmpx, _tmpy);
      }

      // even columns have the square term in them
      if ((ix&1) == 0) {
          fp_digit _a_dp = A->dp[ix>>1];
          SQRADD(_a_dp, A->dp[ix>>1]);
      }

      // store it
      COMBA_STORE(dst->dp[ix]);
  }

  COMBA_FINI;

  // setup dest
  dst->used = pa;
  fp_clamp (dst);
  if (dst != B) {
     fp_copy(dst, B);
  }
}

void fp_sqr_comba_small(fp_int *A, fp_int *B) {
   fp_digit *a, b[32], c0, c1, c2, sc0, sc1, sc2;
   #ifdef TFM_ISO
     fp_word tt;
   #endif
   switch (A->used) {
   case 1:
      a = A->dp;
      COMBA_START;

      // clear carries
      CLEAR_CARRY;

      // output 0
      SQRADD(a[0],a[0]);
      COMBA_STORE(b[0]);
      COMBA_STORE2(b[1]);
      COMBA_FINI;

      B->used = 2;
      B->sign = FP_ZPOS;
      memcpy(B->dp, b, 2 * sizeof(fp_digit));
      fp_clamp(B);
      break;

   case 2:
      a = A->dp;
      COMBA_START;

      // clear carries
      CLEAR_CARRY;

      // output 0
      SQRADD(a[0],a[0]);
      COMBA_STORE(b[0]);

      // output 1
      CARRY_FORWARD;
      SQRADD2(a[0], a[1]);
      COMBA_STORE(b[1]);

      // output 2
      CARRY_FORWARD;
      SQRADD(a[1], a[1]);
      COMBA_STORE(b[2]);
      COMBA_STORE2(b[3]);
      COMBA_FINI;

      B->used = 4;
      B->sign = FP_ZPOS;
      memcpy(B->dp, b, 4 * sizeof(fp_digit));
      fp_clamp(B);
      break;

   case 3:
      a = A->dp;
      COMBA_START;

      // clear carries
      CLEAR_CARRY;

      // output 0
      SQRADD(a[0],a[0]);
      COMBA_STORE(b[0]);

      // output 1
      CARRY_FORWARD;
      SQRADD2(a[0], a[1]);
      COMBA_STORE(b[1]);

      // output 2
      CARRY_FORWARD;
      SQRADD2(a[0], a[2]);    SQRADD(a[1], a[1]);
      COMBA_STORE(b[2]);

      // output 3
      CARRY_FORWARD;
      SQRADD2(a[1], a[2]);
      COMBA_STORE(b[3]);

      // output 4
      CARRY_FORWARD;
      SQRADD(a[2], a[2]);
      COMBA_STORE(b[4]);
      COMBA_STORE2(b[5]);
      COMBA_FINI;

      B->used = 6;
      B->sign = FP_ZPOS;
      memcpy(B->dp, b, 6 * sizeof(fp_digit));
      fp_clamp(B);
      break;

   case 4:
      a = A->dp;
      COMBA_START;

      // clear carries
      CLEAR_CARRY;

      // output 0
      SQRADD(a[0],a[0]);
      COMBA_STORE(b[0]);

      // output 1
      CARRY_FORWARD;
      SQRADD2(a[0], a[1]);
      COMBA_STORE(b[1]);

      // output 2
      CARRY_FORWARD;
      SQRADD2(a[0], a[2]);    SQRADD(a[1], a[1]);
      COMBA_STORE(b[2]);

      // output 3
      CARRY_FORWARD;
      SQRADD2(a[0], a[3]);    SQRADD2(a[1], a[2]);
      COMBA_STORE(b[3]);

      // output 4
      CARRY_FORWARD;
      SQRADD2(a[1], a[3]);    SQRADD(a[2], a[2]);
      COMBA_STORE(b[4]);

      // output 5
      CARRY_FORWARD;
      SQRADD2(a[2], a[3]);
      COMBA_STORE(b[5]);

      // output 6
      CARRY_FORWARD;
      SQRADD(a[3], a[3]);
      COMBA_STORE(b[6]);
      COMBA_STORE2(b[7]);
      COMBA_FINI;

      B->used = 8;
      B->sign = FP_ZPOS;
      memcpy(B->dp, b, 8 * sizeof(fp_digit));
      fp_clamp(B);
      break;

   case 5:
      a = A->dp;
      COMBA_START;

      // clear carries
      CLEAR_CARRY;

      // output 0
      SQRADD(a[0],a[0]);
      COMBA_STORE(b[0]);

      // output 1
      CARRY_FORWARD;
      SQRADD2(a[0], a[1]);
      COMBA_STORE(b[1]);

      // output 2
      CARRY_FORWARD;
      SQRADD2(a[0], a[2]);    SQRADD(a[1], a[1]);
      COMBA_STORE(b[2]);

      // output 3
      CARRY_FORWARD;
      SQRADD2(a[0], a[3]);    SQRADD2(a[1], a[2]);
      COMBA_STORE(b[3]);

      // output 4
      CARRY_FORWARD;
      SQRADD2(a[0], a[4]);    SQRADD2(a[1], a[3]);    SQRADD(a[2], a[2]);
      COMBA_STORE(b[4]);

      // output 5
      CARRY_FORWARD;
      SQRADD2(a[1], a[4]);    SQRADD2(a[2], a[3]);
      COMBA_STORE(b[5]);

      // output 6
      CARRY_FORWARD;
      SQRADD2(a[2], a[4]);    SQRADD(a[3], a[3]);
      COMBA_STORE(b[6]);

      // output 7
      CARRY_FORWARD;
      SQRADD2(a[3], a[4]);
      COMBA_STORE(b[7]);

      // output 8
      CARRY_FORWARD;
      SQRADD(a[4], a[4]);
      COMBA_STORE(b[8]);
      COMBA_STORE2(b[9]);
      COMBA_FINI;

      B->used = 10;
      B->sign = FP_ZPOS;
      memcpy(B->dp, b, 10 * sizeof(fp_digit));
      fp_clamp(B);
      break;

   case 6:
      a = A->dp;
      COMBA_START;

      // clear carries
      CLEAR_CARRY;

      // output 0
      SQRADD(a[0],a[0]);
      COMBA_STORE(b[0]);

      // output 1
      CARRY_FORWARD;
      SQRADD2(a[0], a[1]);
      COMBA_STORE(b[1]);

      // output 2
      CARRY_FORWARD;
      SQRADD2(a[0], a[2]);    SQRADD(a[1], a[1]);
      COMBA_STORE(b[2]);

      // output 3
      CARRY_FORWARD;
      SQRADD2(a[0], a[3]);    SQRADD2(a[1], a[2]);
      COMBA_STORE(b[3]);

      // output 4
      CARRY_FORWARD;
      SQRADD2(a[0], a[4]);    SQRADD2(a[1], a[3]);    SQRADD(a[2], a[2]);
      COMBA_STORE(b[4]);

      // output 5
      CARRY_FORWARD;
   SQRADDSC(a[0], a[5]); SQRADDAC(a[1], a[4]); SQRADDAC(a[2], a[3]); SQRADDDB;
      COMBA_STORE(b[5]);

      // output 6
      CARRY_FORWARD;
      SQRADD2(a[1], a[5]);    SQRADD2(a[2], a[4]);    SQRADD(a[3], a[3]);
      COMBA_STORE(b[6]);

      // output 7
      CARRY_FORWARD;
      SQRADD2(a[2], a[5]);    SQRADD2(a[3], a[4]);
      COMBA_STORE(b[7]);

      // output 8
      CARRY_FORWARD;
      SQRADD2(a[3], a[5]);    SQRADD(a[4], a[4]);
      COMBA_STORE(b[8]);

      // output 9
      CARRY_FORWARD;
      SQRADD2(a[4], a[5]);
      COMBA_STORE(b[9]);

      // output 10
      CARRY_FORWARD;
      SQRADD(a[5], a[5]);
      COMBA_STORE(b[10]);
      COMBA_STORE2(b[11]);
      COMBA_FINI;

      B->used = 12;
      B->sign = FP_ZPOS;
      memcpy(B->dp, b, 12 * sizeof(fp_digit));
      fp_clamp(B);
      break;

   case 7:
      a = A->dp;
      COMBA_START;

      // clear carries
      CLEAR_CARRY;

      // output 0
      SQRADD(a[0],a[0]);
      COMBA_STORE(b[0]);

      // output 1
      CARRY_FORWARD;
      SQRADD2(a[0], a[1]);
      COMBA_STORE(b[1]);

      // output 2
      CARRY_FORWARD;
      SQRADD2(a[0], a[2]);    SQRADD(a[1], a[1]);
      COMBA_STORE(b[2]);

      // output 3
      CARRY_FORWARD;
      SQRADD2(a[0], a[3]);    SQRADD2(a[1], a[2]);
      COMBA_STORE(b[3]);

      // output 4
      CARRY_FORWARD;
      SQRADD2(a[0], a[4]);    SQRADD2(a[1], a[3]);    SQRADD(a[2], a[2]);
      COMBA_STORE(b[4]);

      // output 5
      CARRY_FORWARD;
   SQRADDSC(a[0], a[5]); SQRADDAC(a[1], a[4]); SQRADDAC(a[2], a[3]); SQRADDDB;
      COMBA_STORE(b[5]);

      // output 6
      CARRY_FORWARD;
   SQRADDSC(a[0], a[6]); SQRADDAC(a[1], a[5]); SQRADDAC(a[2], a[4]); SQRADDDB; SQRADD(a[3], a[3]);
      COMBA_STORE(b[6]);

      // output 7
      CARRY_FORWARD;
   SQRADDSC(a[1], a[6]); SQRADDAC(a[2], a[5]); SQRADDAC(a[3], a[4]); SQRADDDB;
      COMBA_STORE(b[7]);

      // output 8
      CARRY_FORWARD;
      SQRADD2(a[2], a[6]);    SQRADD2(a[3], a[5]);    SQRADD(a[4], a[4]);
      COMBA_STORE(b[8]);

      // output 9
      CARRY_FORWARD;
      SQRADD2(a[3], a[6]);    SQRADD2(a[4], a[5]);
      COMBA_STORE(b[9]);

      // output 10
      CARRY_FORWARD;
      SQRADD2(a[4], a[6]);    SQRADD(a[5], a[5]);
      COMBA_STORE(b[10]);

      // output 11
      CARRY_FORWARD;
      SQRADD2(a[5], a[6]);
      COMBA_STORE(b[11]);

      // output 12
      CARRY_FORWARD;
      SQRADD(a[6], a[6]);
      COMBA_STORE(b[12]);
      COMBA_STORE2(b[13]);
      COMBA_FINI;

      B->used = 14;
      B->sign = FP_ZPOS;
      memcpy(B->dp, b, 14 * sizeof(fp_digit));
      fp_clamp(B);
      break;

   case 8:
      a = A->dp;
      COMBA_START;

      // clear carries
      CLEAR_CARRY;

      // output 0
      SQRADD(a[0],a[0]);
      COMBA_STORE(b[0]);

      // output 1
      CARRY_FORWARD;
      SQRADD2(a[0], a[1]);
      COMBA_STORE(b[1]);

      // output 2
      CARRY_FORWARD;
      SQRADD2(a[0], a[2]);    SQRADD(a[1], a[1]);
      COMBA_STORE(b[2]);

      // output 3
      CARRY_FORWARD;
      SQRADD2(a[0], a[3]);    SQRADD2(a[1], a[2]);
      COMBA_STORE(b[3]);

      // output 4
      CARRY_FORWARD;
      SQRADD2(a[0], a[4]);    SQRADD2(a[1], a[3]);    SQRADD(a[2], a[2]);
      COMBA_STORE(b[4]);

      // output 5
      CARRY_FORWARD;
   SQRADDSC(a[0], a[5]); SQRADDAC(a[1], a[4]); SQRADDAC(a[2], a[3]); SQRADDDB;
      COMBA_STORE(b[5]);

      // output 6
      CARRY_FORWARD;
   SQRADDSC(a[0], a[6]); SQRADDAC(a[1], a[5]); SQRADDAC(a[2], a[4]); SQRADDDB; SQRADD(a[3], a[3]);
      COMBA_STORE(b[6]);

      // output 7
      CARRY_FORWARD;
   SQRADDSC(a[0], a[7]); SQRADDAC(a[1], a[6]); SQRADDAC(a[2], a[5]); SQRADDAC(a[3], a[4]); SQRADDDB;
      COMBA_STORE(b[7]);

      // output 8
      CARRY_FORWARD;
   SQRADDSC(a[1], a[7]); SQRADDAC(a[2], a[6]); SQRADDAC(a[3], a[5]); SQRADDDB; SQRADD(a[4], a[4]);
      COMBA_STORE(b[8]);

      // output 9
      CARRY_FORWARD;
   SQRADDSC(a[2], a[7]); SQRADDAC(a[3], a[6]); SQRADDAC(a[4], a[5]); SQRADDDB;
      COMBA_STORE(b[9]);

      // output 10
      CARRY_FORWARD;
      SQRADD2(a[3], a[7]);    SQRADD2(a[4], a[6]);    SQRADD(a[5], a[5]);
      COMBA_STORE(b[10]);

      // output 11
      CARRY_FORWARD;
      SQRADD2(a[4], a[7]);    SQRADD2(a[5], a[6]);
      COMBA_STORE(b[11]);

      // output 12
      CARRY_FORWARD;
      SQRADD2(a[5], a[7]);    SQRADD(a[6], a[6]);
      COMBA_STORE(b[12]);

      // output 13
      CARRY_FORWARD;
      SQRADD2(a[6], a[7]);
      COMBA_STORE(b[13]);

      // output 14
      CARRY_FORWARD;
      SQRADD(a[7], a[7]);
      COMBA_STORE(b[14]);
      COMBA_STORE2(b[15]);
      COMBA_FINI;

      B->used = 16;
      B->sign = FP_ZPOS;
      memcpy(B->dp, b, 16 * sizeof(fp_digit));
      fp_clamp(B);
      break;

   case 9:
      a = A->dp;
      COMBA_START;

      // clear carries
      CLEAR_CARRY;

      // output 0
      SQRADD(a[0],a[0]);
      COMBA_STORE(b[0]);

      // output 1
      CARRY_FORWARD;
      SQRADD2(a[0], a[1]);
      COMBA_STORE(b[1]);

      // output 2
      CARRY_FORWARD;
      SQRADD2(a[0], a[2]);    SQRADD(a[1], a[1]);
      COMBA_STORE(b[2]);

      // output 3
      CARRY_FORWARD;
      SQRADD2(a[0], a[3]);    SQRADD2(a[1], a[2]);
      COMBA_STORE(b[3]);

      // output 4
      CARRY_FORWARD;
      SQRADD2(a[0], a[4]);    SQRADD2(a[1], a[3]);    SQRADD(a[2], a[2]);
      COMBA_STORE(b[4]);

      // output 5
      CARRY_FORWARD;
   SQRADDSC(a[0], a[5]); SQRADDAC(a[1], a[4]); SQRADDAC(a[2], a[3]); SQRADDDB;
      COMBA_STORE(b[5]);

      // output 6
      CARRY_FORWARD;
   SQRADDSC(a[0], a[6]); SQRADDAC(a[1], a[5]); SQRADDAC(a[2], a[4]); SQRADDDB; SQRADD(a[3], a[3]);
      COMBA_STORE(b[6]);

      // output 7
      CARRY_FORWARD;
   SQRADDSC(a[0], a[7]); SQRADDAC(a[1], a[6]); SQRADDAC(a[2], a[5]); SQRADDAC(a[3], a[4]); SQRADDDB;
      COMBA_STORE(b[7]);

      // output 8
      CARRY_FORWARD;
   SQRADDSC(a[0], a[8]); SQRADDAC(a[1], a[7]); SQRADDAC(a[2], a[6]); SQRADDAC(a[3], a[5]); SQRADDDB; SQRADD(a[4], a[4]);
      COMBA_STORE(b[8]);

      // output 9
      CARRY_FORWARD;
   SQRADDSC(a[1], a[8]); SQRADDAC(a[2], a[7]); SQRADDAC(a[3], a[6]); SQRADDAC(a[4], a[5]); SQRADDDB;
      COMBA_STORE(b[9]);

      // output 10
      CARRY_FORWARD;
   SQRADDSC(a[2], a[8]); SQRADDAC(a[3], a[7]); SQRADDAC(a[4], a[6]); SQRADDDB; SQRADD(a[5], a[5]);
      COMBA_STORE(b[10]);

      // output 11
      CARRY_FORWARD;
   SQRADDSC(a[3], a[8]); SQRADDAC(a[4], a[7]); SQRADDAC(a[5], a[6]); SQRADDDB;
      COMBA_STORE(b[11]);

      // output 12
      CARRY_FORWARD;
      SQRADD2(a[4], a[8]);    SQRADD2(a[5], a[7]);    SQRADD(a[6], a[6]);
      COMBA_STORE(b[12]);

      // output 13
      CARRY_FORWARD;
      SQRADD2(a[5], a[8]);    SQRADD2(a[6], a[7]);
      COMBA_STORE(b[13]);

      // output 14
      CARRY_FORWARD;
      SQRADD2(a[6], a[8]);    SQRADD(a[7], a[7]);
      COMBA_STORE(b[14]);

      // output 15
      CARRY_FORWARD;
      SQRADD2(a[7], a[8]);
      COMBA_STORE(b[15]);

      // output 16
      CARRY_FORWARD;
      SQRADD(a[8], a[8]);
      COMBA_STORE(b[16]);
      COMBA_STORE2(b[17]);
      COMBA_FINI;

      B->used = 18;
      B->sign = FP_ZPOS;
      memcpy(B->dp, b, 18 * sizeof(fp_digit));
      fp_clamp(B);
      break;

   case 10:
      a = A->dp;
      COMBA_START;

      // clear carries
      CLEAR_CARRY;

      // output 0
      SQRADD(a[0],a[0]);
      COMBA_STORE(b[0]);

      // output 1
      CARRY_FORWARD;
      SQRADD2(a[0], a[1]);
      COMBA_STORE(b[1]);

      // output 2
      CARRY_FORWARD;
      SQRADD2(a[0], a[2]);    SQRADD(a[1], a[1]);
      COMBA_STORE(b[2]);

      // output 3
      CARRY_FORWARD;
      SQRADD2(a[0], a[3]);    SQRADD2(a[1], a[2]);
      COMBA_STORE(b[3]);

      // output 4
      CARRY_FORWARD;
      SQRADD2(a[0], a[4]);    SQRADD2(a[1], a[3]);    SQRADD(a[2], a[2]);
      COMBA_STORE(b[4]);

      // output 5
      CARRY_FORWARD;
   SQRADDSC(a[0], a[5]); SQRADDAC(a[1], a[4]); SQRADDAC(a[2], a[3]); SQRADDDB;
      COMBA_STORE(b[5]);

      // output 6
      CARRY_FORWARD;
   SQRADDSC(a[0], a[6]); SQRADDAC(a[1], a[5]); SQRADDAC(a[2], a[4]); SQRADDDB; SQRADD(a[3], a[3]);
      COMBA_STORE(b[6]);

      // output 7
      CARRY_FORWARD;
   SQRADDSC(a[0], a[7]); SQRADDAC(a[1], a[6]); SQRADDAC(a[2], a[5]); SQRADDAC(a[3], a[4]); SQRADDDB;
      COMBA_STORE(b[7]);

      // output 8
      CARRY_FORWARD;
   SQRADDSC(a[0], a[8]); SQRADDAC(a[1], a[7]); SQRADDAC(a[2], a[6]); SQRADDAC(a[3], a[5]); SQRADDDB; SQRADD(a[4], a[4]);
      COMBA_STORE(b[8]);

      // output 9
      CARRY_FORWARD;
   SQRADDSC(a[0], a[9]); SQRADDAC(a[1], a[8]); SQRADDAC(a[2], a[7]); SQRADDAC(a[3], a[6]); SQRADDAC(a[4], a[5]); SQRADDDB;
      COMBA_STORE(b[9]);

      // output 10
      CARRY_FORWARD;
   SQRADDSC(a[1], a[9]); SQRADDAC(a[2], a[8]); SQRADDAC(a[3], a[7]); SQRADDAC(a[4], a[6]); SQRADDDB; SQRADD(a[5], a[5]);
      COMBA_STORE(b[10]);

      // output 11
      CARRY_FORWARD;
   SQRADDSC(a[2], a[9]); SQRADDAC(a[3], a[8]); SQRADDAC(a[4], a[7]); SQRADDAC(a[5], a[6]); SQRADDDB;
      COMBA_STORE(b[11]);

      // output 12
      CARRY_FORWARD;
   SQRADDSC(a[3], a[9]); SQRADDAC(a[4], a[8]); SQRADDAC(a[5], a[7]); SQRADDDB; SQRADD(a[6], a[6]);
      COMBA_STORE(b[12]);

      // output 13
      CARRY_FORWARD;
   SQRADDSC(a[4], a[9]); SQRADDAC(a[5], a[8]); SQRADDAC(a[6], a[7]); SQRADDDB;
      COMBA_STORE(b[13]);

      // output 14
      CARRY_FORWARD;
      SQRADD2(a[5], a[9]);    SQRADD2(a[6], a[8]);    SQRADD(a[7], a[7]);
      COMBA_STORE(b[14]);

      // output 15
      CARRY_FORWARD;
      SQRADD2(a[6], a[9]);    SQRADD2(a[7], a[8]);
      COMBA_STORE(b[15]);

      // output 16
      CARRY_FORWARD;
      SQRADD2(a[7], a[9]);    SQRADD(a[8], a[8]);
      COMBA_STORE(b[16]);

      // output 17
      CARRY_FORWARD;
      SQRADD2(a[8], a[9]);
      COMBA_STORE(b[17]);

      // output 18
      CARRY_FORWARD;
      SQRADD(a[9], a[9]);
      COMBA_STORE(b[18]);
      COMBA_STORE2(b[19]);
      COMBA_FINI;

      B->used = 20;
      B->sign = FP_ZPOS;
      memcpy(B->dp, b, 20 * sizeof(fp_digit));
      fp_clamp(B);
      break;

   case 11:
      a = A->dp;
      COMBA_START;

      // clear carries
      CLEAR_CARRY;

      // output 0
      SQRADD(a[0],a[0]);
      COMBA_STORE(b[0]);

      // output 1
      CARRY_FORWARD;
      SQRADD2(a[0], a[1]);
      COMBA_STORE(b[1]);

      // output 2
      CARRY_FORWARD;
      SQRADD2(a[0], a[2]);    SQRADD(a[1], a[1]);
      COMBA_STORE(b[2]);

      // output 3
      CARRY_FORWARD;
      SQRADD2(a[0], a[3]);    SQRADD2(a[1], a[2]);
      COMBA_STORE(b[3]);

      // output 4
      CARRY_FORWARD;
      SQRADD2(a[0], a[4]);    SQRADD2(a[1], a[3]);    SQRADD(a[2], a[2]);
      COMBA_STORE(b[4]);

      // output 5
      CARRY_FORWARD;
   SQRADDSC(a[0], a[5]); SQRADDAC(a[1], a[4]); SQRADDAC(a[2], a[3]); SQRADDDB;
      COMBA_STORE(b[5]);

      // output 6
      CARRY_FORWARD;
   SQRADDSC(a[0], a[6]); SQRADDAC(a[1], a[5]); SQRADDAC(a[2], a[4]); SQRADDDB; SQRADD(a[3], a[3]);
      COMBA_STORE(b[6]);

      // output 7
      CARRY_FORWARD;
   SQRADDSC(a[0], a[7]); SQRADDAC(a[1], a[6]); SQRADDAC(a[2], a[5]); SQRADDAC(a[3], a[4]); SQRADDDB;
      COMBA_STORE(b[7]);

      // output 8
      CARRY_FORWARD;
   SQRADDSC(a[0], a[8]); SQRADDAC(a[1], a[7]); SQRADDAC(a[2], a[6]); SQRADDAC(a[3], a[5]); SQRADDDB; SQRADD(a[4], a[4]);
      COMBA_STORE(b[8]);

      // output 9
      CARRY_FORWARD;
   SQRADDSC(a[0], a[9]); SQRADDAC(a[1], a[8]); SQRADDAC(a[2], a[7]); SQRADDAC(a[3], a[6]); SQRADDAC(a[4], a[5]); SQRADDDB;
      COMBA_STORE(b[9]);

      // output 10
      CARRY_FORWARD;
   SQRADDSC(a[0], a[10]); SQRADDAC(a[1], a[9]); SQRADDAC(a[2], a[8]); SQRADDAC(a[3], a[7]); SQRADDAC(a[4], a[6]); SQRADDDB; SQRADD(a[5], a[5]);
      COMBA_STORE(b[10]);

      // output 11
      CARRY_FORWARD;
   SQRADDSC(a[1], a[10]); SQRADDAC(a[2], a[9]); SQRADDAC(a[3], a[8]); SQRADDAC(a[4], a[7]); SQRADDAC(a[5], a[6]); SQRADDDB;
      COMBA_STORE(b[11]);

      // output 12
      CARRY_FORWARD;
   SQRADDSC(a[2], a[10]); SQRADDAC(a[3], a[9]); SQRADDAC(a[4], a[8]); SQRADDAC(a[5], a[7]); SQRADDDB; SQRADD(a[6], a[6]);
      COMBA_STORE(b[12]);

      // output 13
      CARRY_FORWARD;
   SQRADDSC(a[3], a[10]); SQRADDAC(a[4], a[9]); SQRADDAC(a[5], a[8]); SQRADDAC(a[6], a[7]); SQRADDDB;
      COMBA_STORE(b[13]);

      // output 14
      CARRY_FORWARD;
   SQRADDSC(a[4], a[10]); SQRADDAC(a[5], a[9]); SQRADDAC(a[6], a[8]); SQRADDDB; SQRADD(a[7], a[7]);
      COMBA_STORE(b[14]);

      // output 15
      CARRY_FORWARD;
   SQRADDSC(a[5], a[10]); SQRADDAC(a[6], a[9]); SQRADDAC(a[7], a[8]); SQRADDDB;
      COMBA_STORE(b[15]);

      // output 16
      CARRY_FORWARD;
      SQRADD2(a[6], a[10]);    SQRADD2(a[7], a[9]);    SQRADD(a[8], a[8]);
      COMBA_STORE(b[16]);

      // output 17
      CARRY_FORWARD;
      SQRADD2(a[7], a[10]);    SQRADD2(a[8], a[9]);
      COMBA_STORE(b[17]);

      // output 18
      CARRY_FORWARD;
      SQRADD2(a[8], a[10]);    SQRADD(a[9], a[9]);
      COMBA_STORE(b[18]);

      // output 19
      CARRY_FORWARD;
      SQRADD2(a[9], a[10]);
      COMBA_STORE(b[19]);

      // output 20
      CARRY_FORWARD;
      SQRADD(a[10], a[10]);
      COMBA_STORE(b[20]);
      COMBA_STORE2(b[21]);
      COMBA_FINI;

      B->used = 22;
      B->sign = FP_ZPOS;
      memcpy(B->dp, b, 22 * sizeof(fp_digit));
      fp_clamp(B);
      break;

   case 12:
      a = A->dp;
      COMBA_START;

      // clear carries
      CLEAR_CARRY;

      // output 0
      SQRADD(a[0],a[0]);
      COMBA_STORE(b[0]);

      // output 1
      CARRY_FORWARD;
      SQRADD2(a[0], a[1]);
      COMBA_STORE(b[1]);

      // output 2
      CARRY_FORWARD;
      SQRADD2(a[0], a[2]);    SQRADD(a[1], a[1]);
      COMBA_STORE(b[2]);

      // output 3
      CARRY_FORWARD;
      SQRADD2(a[0], a[3]);    SQRADD2(a[1], a[2]);
      COMBA_STORE(b[3]);

      // output 4
      CARRY_FORWARD;
      SQRADD2(a[0], a[4]);    SQRADD2(a[1], a[3]);    SQRADD(a[2], a[2]);
      COMBA_STORE(b[4]);

      // output 5
      CARRY_FORWARD;
   SQRADDSC(a[0], a[5]); SQRADDAC(a[1], a[4]); SQRADDAC(a[2], a[3]); SQRADDDB;
      COMBA_STORE(b[5]);

      // output 6
      CARRY_FORWARD;
   SQRADDSC(a[0], a[6]); SQRADDAC(a[1], a[5]); SQRADDAC(a[2], a[4]); SQRADDDB; SQRADD(a[3], a[3]);
      COMBA_STORE(b[6]);

      // output 7
      CARRY_FORWARD;
   SQRADDSC(a[0], a[7]); SQRADDAC(a[1], a[6]); SQRADDAC(a[2], a[5]); SQRADDAC(a[3], a[4]); SQRADDDB;
      COMBA_STORE(b[7]);

      // output 8
      CARRY_FORWARD;
   SQRADDSC(a[0], a[8]); SQRADDAC(a[1], a[7]); SQRADDAC(a[2], a[6]); SQRADDAC(a[3], a[5]); SQRADDDB; SQRADD(a[4], a[4]);
      COMBA_STORE(b[8]);

      // output 9
      CARRY_FORWARD;
   SQRADDSC(a[0], a[9]); SQRADDAC(a[1], a[8]); SQRADDAC(a[2], a[7]); SQRADDAC(a[3], a[6]); SQRADDAC(a[4], a[5]); SQRADDDB;
      COMBA_STORE(b[9]);

      // output 10
      CARRY_FORWARD;
   SQRADDSC(a[0], a[10]); SQRADDAC(a[1], a[9]); SQRADDAC(a[2], a[8]); SQRADDAC(a[3], a[7]); SQRADDAC(a[4], a[6]); SQRADDDB; SQRADD(a[5], a[5]);
      COMBA_STORE(b[10]);

      // output 11
      CARRY_FORWARD;
   SQRADDSC(a[0], a[11]); SQRADDAC(a[1], a[10]); SQRADDAC(a[2], a[9]); SQRADDAC(a[3], a[8]); SQRADDAC(a[4], a[7]); SQRADDAC(a[5], a[6]); SQRADDDB;
      COMBA_STORE(b[11]);

      // output 12
      CARRY_FORWARD;
   SQRADDSC(a[1], a[11]); SQRADDAC(a[2], a[10]); SQRADDAC(a[3], a[9]); SQRADDAC(a[4], a[8]); SQRADDAC(a[5], a[7]); SQRADDDB; SQRADD(a[6], a[6]);
      COMBA_STORE(b[12]);

      // output 13
      CARRY_FORWARD;
   SQRADDSC(a[2], a[11]); SQRADDAC(a[3], a[10]); SQRADDAC(a[4], a[9]); SQRADDAC(a[5], a[8]); SQRADDAC(a[6], a[7]); SQRADDDB;
      COMBA_STORE(b[13]);

      // output 14
      CARRY_FORWARD;
   SQRADDSC(a[3], a[11]); SQRADDAC(a[4], a[10]); SQRADDAC(a[5], a[9]); SQRADDAC(a[6], a[8]); SQRADDDB; SQRADD(a[7], a[7]);
      COMBA_STORE(b[14]);

      // output 15
      CARRY_FORWARD;
   SQRADDSC(a[4], a[11]); SQRADDAC(a[5], a[10]); SQRADDAC(a[6], a[9]); SQRADDAC(a[7], a[8]); SQRADDDB;
      COMBA_STORE(b[15]);

      // output 16
      CARRY_FORWARD;
   SQRADDSC(a[5], a[11]); SQRADDAC(a[6], a[10]); SQRADDAC(a[7], a[9]); SQRADDDB; SQRADD(a[8], a[8]);
      COMBA_STORE(b[16]);

      // output 17
      CARRY_FORWARD;
   SQRADDSC(a[6], a[11]); SQRADDAC(a[7], a[10]); SQRADDAC(a[8], a[9]); SQRADDDB;
      COMBA_STORE(b[17]);

      // output 18
      CARRY_FORWARD;
      SQRADD2(a[7], a[11]);    SQRADD2(a[8], a[10]);    SQRADD(a[9], a[9]);
      COMBA_STORE(b[18]);

      // output 19
      CARRY_FORWARD;
      SQRADD2(a[8], a[11]);    SQRADD2(a[9], a[10]);
      COMBA_STORE(b[19]);

      // output 20
      CARRY_FORWARD;
      SQRADD2(a[9], a[11]);    SQRADD(a[10], a[10]);
      COMBA_STORE(b[20]);

      // output 21
      CARRY_FORWARD;
      SQRADD2(a[10], a[11]);
      COMBA_STORE(b[21]);

      // output 22
      CARRY_FORWARD;
      SQRADD(a[11], a[11]);
      COMBA_STORE(b[22]);
      COMBA_STORE2(b[23]);
      COMBA_FINI;

      B->used = 24;
      B->sign = FP_ZPOS;
      memcpy(B->dp, b, 24 * sizeof(fp_digit));
      fp_clamp(B);
      break;

   case 13:
      a = A->dp;
      COMBA_START;

      // clear carries
      CLEAR_CARRY;

      // output 0
      SQRADD(a[0],a[0]);
      COMBA_STORE(b[0]);

      // output 1
      CARRY_FORWARD;
      SQRADD2(a[0], a[1]);
      COMBA_STORE(b[1]);

      // output 2
      CARRY_FORWARD;
      SQRADD2(a[0], a[2]);    SQRADD(a[1], a[1]);
      COMBA_STORE(b[2]);

      // output 3
      CARRY_FORWARD;
      SQRADD2(a[0], a[3]);    SQRADD2(a[1], a[2]);
      COMBA_STORE(b[3]);

      // output 4
      CARRY_FORWARD;
      SQRADD2(a[0], a[4]);    SQRADD2(a[1], a[3]);    SQRADD(a[2], a[2]);
      COMBA_STORE(b[4]);

      // output 5
      CARRY_FORWARD;
   SQRADDSC(a[0], a[5]); SQRADDAC(a[1], a[4]); SQRADDAC(a[2], a[3]); SQRADDDB;
      COMBA_STORE(b[5]);

      // output 6
      CARRY_FORWARD;
   SQRADDSC(a[0], a[6]); SQRADDAC(a[1], a[5]); SQRADDAC(a[2], a[4]); SQRADDDB; SQRADD(a[3], a[3]);
      COMBA_STORE(b[6]);

      // output 7
      CARRY_FORWARD;
   SQRADDSC(a[0], a[7]); SQRADDAC(a[1], a[6]); SQRADDAC(a[2], a[5]); SQRADDAC(a[3], a[4]); SQRADDDB;
      COMBA_STORE(b[7]);

      // output 8
      CARRY_FORWARD;
   SQRADDSC(a[0], a[8]); SQRADDAC(a[1], a[7]); SQRADDAC(a[2], a[6]); SQRADDAC(a[3], a[5]); SQRADDDB; SQRADD(a[4], a[4]);
      COMBA_STORE(b[8]);

      // output 9
      CARRY_FORWARD;
   SQRADDSC(a[0], a[9]); SQRADDAC(a[1], a[8]); SQRADDAC(a[2], a[7]); SQRADDAC(a[3], a[6]); SQRADDAC(a[4], a[5]); SQRADDDB;
      COMBA_STORE(b[9]);

      // output 10
      CARRY_FORWARD;
   SQRADDSC(a[0], a[10]); SQRADDAC(a[1], a[9]); SQRADDAC(a[2], a[8]); SQRADDAC(a[3], a[7]); SQRADDAC(a[4], a[6]); SQRADDDB; SQRADD(a[5], a[5]);
      COMBA_STORE(b[10]);

      // output 11
      CARRY_FORWARD;
   SQRADDSC(a[0], a[11]); SQRADDAC(a[1], a[10]); SQRADDAC(a[2], a[9]); SQRADDAC(a[3], a[8]); SQRADDAC(a[4], a[7]); SQRADDAC(a[5], a[6]); SQRADDDB;
      COMBA_STORE(b[11]);

      // output 12
      CARRY_FORWARD;
   SQRADDSC(a[0], a[12]); SQRADDAC(a[1], a[11]); SQRADDAC(a[2], a[10]); SQRADDAC(a[3], a[9]); SQRADDAC(a[4], a[8]); SQRADDAC(a[5], a[7]); SQRADDDB; SQRADD(a[6], a[6]);
      COMBA_STORE(b[12]);

      // output 13
      CARRY_FORWARD;
   SQRADDSC(a[1], a[12]); SQRADDAC(a[2], a[11]); SQRADDAC(a[3], a[10]); SQRADDAC(a[4], a[9]); SQRADDAC(a[5], a[8]); SQRADDAC(a[6], a[7]); SQRADDDB;
      COMBA_STORE(b[13]);

      // output 14
      CARRY_FORWARD;
   SQRADDSC(a[2], a[12]); SQRADDAC(a[3], a[11]); SQRADDAC(a[4], a[10]); SQRADDAC(a[5], a[9]); SQRADDAC(a[6], a[8]); SQRADDDB; SQRADD(a[7], a[7]);
      COMBA_STORE(b[14]);

      // output 15
      CARRY_FORWARD;
   SQRADDSC(a[3], a[12]); SQRADDAC(a[4], a[11]); SQRADDAC(a[5], a[10]); SQRADDAC(a[6], a[9]); SQRADDAC(a[7], a[8]); SQRADDDB;
      COMBA_STORE(b[15]);

      // output 16
      CARRY_FORWARD;
   SQRADDSC(a[4], a[12]); SQRADDAC(a[5], a[11]); SQRADDAC(a[6], a[10]); SQRADDAC(a[7], a[9]); SQRADDDB; SQRADD(a[8], a[8]);
      COMBA_STORE(b[16]);

      // output 17
      CARRY_FORWARD;
   SQRADDSC(a[5], a[12]); SQRADDAC(a[6], a[11]); SQRADDAC(a[7], a[10]); SQRADDAC(a[8], a[9]); SQRADDDB;
      COMBA_STORE(b[17]);

      // output 18
      CARRY_FORWARD;
   SQRADDSC(a[6], a[12]); SQRADDAC(a[7], a[11]); SQRADDAC(a[8], a[10]); SQRADDDB; SQRADD(a[9], a[9]);
      COMBA_STORE(b[18]);

      // output 19
      CARRY_FORWARD;
   SQRADDSC(a[7], a[12]); SQRADDAC(a[8], a[11]); SQRADDAC(a[9], a[10]); SQRADDDB;
      COMBA_STORE(b[19]);

      // output 20
      CARRY_FORWARD;
      SQRADD2(a[8], a[12]);    SQRADD2(a[9], a[11]);    SQRADD(a[10], a[10]);
      COMBA_STORE(b[20]);

      // output 21
      CARRY_FORWARD;
      SQRADD2(a[9], a[12]);    SQRADD2(a[10], a[11]);
      COMBA_STORE(b[21]);

      // output 22
      CARRY_FORWARD;
      SQRADD2(a[10], a[12]);    SQRADD(a[11], a[11]);
      COMBA_STORE(b[22]);

      // output 23
      CARRY_FORWARD;
      SQRADD2(a[11], a[12]);
      COMBA_STORE(b[23]);

      // output 24
      CARRY_FORWARD;
      SQRADD(a[12], a[12]);
      COMBA_STORE(b[24]);
      COMBA_STORE2(b[25]);
      COMBA_FINI;

      B->used = 26;
      B->sign = FP_ZPOS;
      memcpy(B->dp, b, 26 * sizeof(fp_digit));
      fp_clamp(B);
      break;

   case 14:
      a = A->dp;
      COMBA_START;

      // clear carries
      CLEAR_CARRY;

      // output 0
      SQRADD(a[0],a[0]);
      COMBA_STORE(b[0]);

      // output 1
      CARRY_FORWARD;
      SQRADD2(a[0], a[1]);
      COMBA_STORE(b[1]);

      // output 2
      CARRY_FORWARD;
      SQRADD2(a[0], a[2]);    SQRADD(a[1], a[1]);
      COMBA_STORE(b[2]);

      // output 3
      CARRY_FORWARD;
      SQRADD2(a[0], a[3]);    SQRADD2(a[1], a[2]);
      COMBA_STORE(b[3]);

      // output 4
      CARRY_FORWARD;
      SQRADD2(a[0], a[4]);    SQRADD2(a[1], a[3]);    SQRADD(a[2], a[2]);
      COMBA_STORE(b[4]);

      // output 5
      CARRY_FORWARD;
   SQRADDSC(a[0], a[5]); SQRADDAC(a[1], a[4]); SQRADDAC(a[2], a[3]); SQRADDDB;
      COMBA_STORE(b[5]);

      // output 6
      CARRY_FORWARD;
   SQRADDSC(a[0], a[6]); SQRADDAC(a[1], a[5]); SQRADDAC(a[2], a[4]); SQRADDDB; SQRADD(a[3], a[3]);
      COMBA_STORE(b[6]);

      // output 7
      CARRY_FORWARD;
   SQRADDSC(a[0], a[7]); SQRADDAC(a[1], a[6]); SQRADDAC(a[2], a[5]); SQRADDAC(a[3], a[4]); SQRADDDB;
      COMBA_STORE(b[7]);

      // output 8
      CARRY_FORWARD;
   SQRADDSC(a[0], a[8]); SQRADDAC(a[1], a[7]); SQRADDAC(a[2], a[6]); SQRADDAC(a[3], a[5]); SQRADDDB; SQRADD(a[4], a[4]);
      COMBA_STORE(b[8]);

      // output 9
      CARRY_FORWARD;
   SQRADDSC(a[0], a[9]); SQRADDAC(a[1], a[8]); SQRADDAC(a[2], a[7]); SQRADDAC(a[3], a[6]); SQRADDAC(a[4], a[5]); SQRADDDB;
      COMBA_STORE(b[9]);

      // output 10
      CARRY_FORWARD;
   SQRADDSC(a[0], a[10]); SQRADDAC(a[1], a[9]); SQRADDAC(a[2], a[8]); SQRADDAC(a[3], a[7]); SQRADDAC(a[4], a[6]); SQRADDDB; SQRADD(a[5], a[5]);
      COMBA_STORE(b[10]);

      // output 11
      CARRY_FORWARD;
   SQRADDSC(a[0], a[11]); SQRADDAC(a[1], a[10]); SQRADDAC(a[2], a[9]); SQRADDAC(a[3], a[8]); SQRADDAC(a[4], a[7]); SQRADDAC(a[5], a[6]); SQRADDDB;
      COMBA_STORE(b[11]);

      // output 12
      CARRY_FORWARD;
   SQRADDSC(a[0], a[12]); SQRADDAC(a[1], a[11]); SQRADDAC(a[2], a[10]); SQRADDAC(a[3], a[9]); SQRADDAC(a[4], a[8]); SQRADDAC(a[5], a[7]); SQRADDDB; SQRADD(a[6], a[6]);
      COMBA_STORE(b[12]);

      // output 13
      CARRY_FORWARD;
   SQRADDSC(a[0], a[13]); SQRADDAC(a[1], a[12]); SQRADDAC(a[2], a[11]); SQRADDAC(a[3], a[10]); SQRADDAC(a[4], a[9]); SQRADDAC(a[5], a[8]); SQRADDAC(a[6], a[7]); SQRADDDB;
      COMBA_STORE(b[13]);

      // output 14
      CARRY_FORWARD;
   SQRADDSC(a[1], a[13]); SQRADDAC(a[2], a[12]); SQRADDAC(a[3], a[11]); SQRADDAC(a[4], a[10]); SQRADDAC(a[5], a[9]); SQRADDAC(a[6], a[8]); SQRADDDB; SQRADD(a[7], a[7]);
      COMBA_STORE(b[14]);

      // output 15
      CARRY_FORWARD;
   SQRADDSC(a[2], a[13]); SQRADDAC(a[3], a[12]); SQRADDAC(a[4], a[11]); SQRADDAC(a[5], a[10]); SQRADDAC(a[6], a[9]); SQRADDAC(a[7], a[8]); SQRADDDB;
      COMBA_STORE(b[15]);

      // output 16
      CARRY_FORWARD;
   SQRADDSC(a[3], a[13]); SQRADDAC(a[4], a[12]); SQRADDAC(a[5], a[11]); SQRADDAC(a[6], a[10]); SQRADDAC(a[7], a[9]); SQRADDDB; SQRADD(a[8], a[8]);
      COMBA_STORE(b[16]);

      // output 17
      CARRY_FORWARD;
   SQRADDSC(a[4], a[13]); SQRADDAC(a[5], a[12]); SQRADDAC(a[6], a[11]); SQRADDAC(a[7], a[10]); SQRADDAC(a[8], a[9]); SQRADDDB;
      COMBA_STORE(b[17]);

      // output 18
      CARRY_FORWARD;
   SQRADDSC(a[5], a[13]); SQRADDAC(a[6], a[12]); SQRADDAC(a[7], a[11]); SQRADDAC(a[8], a[10]); SQRADDDB; SQRADD(a[9], a[9]);
      COMBA_STORE(b[18]);

      // output 19
      CARRY_FORWARD;
   SQRADDSC(a[6], a[13]); SQRADDAC(a[7], a[12]); SQRADDAC(a[8], a[11]); SQRADDAC(a[9], a[10]); SQRADDDB;
      COMBA_STORE(b[19]);

      // output 20
      CARRY_FORWARD;
   SQRADDSC(a[7], a[13]); SQRADDAC(a[8], a[12]); SQRADDAC(a[9], a[11]); SQRADDDB; SQRADD(a[10], a[10]);
      COMBA_STORE(b[20]);

      // output 21
      CARRY_FORWARD;
   SQRADDSC(a[8], a[13]); SQRADDAC(a[9], a[12]); SQRADDAC(a[10], a[11]); SQRADDDB;
      COMBA_STORE(b[21]);

      // output 22
      CARRY_FORWARD;
      SQRADD2(a[9], a[13]);    SQRADD2(a[10], a[12]);    SQRADD(a[11], a[11]);
      COMBA_STORE(b[22]);

      // output 23
      CARRY_FORWARD;
      SQRADD2(a[10], a[13]);    SQRADD2(a[11], a[12]);
      COMBA_STORE(b[23]);

      // output 24
      CARRY_FORWARD;
      SQRADD2(a[11], a[13]);    SQRADD(a[12], a[12]);
      COMBA_STORE(b[24]);

      // output 25
      CARRY_FORWARD;
      SQRADD2(a[12], a[13]);
      COMBA_STORE(b[25]);

      // output 26
      CARRY_FORWARD;
      SQRADD(a[13], a[13]);
      COMBA_STORE(b[26]);
      COMBA_STORE2(b[27]);
      COMBA_FINI;

      B->used = 28;
      B->sign = FP_ZPOS;
      memcpy(B->dp, b, 28 * sizeof(fp_digit));
      fp_clamp(B);
      break;

   case 15:
      a = A->dp;
      COMBA_START;

      // clear carries
      CLEAR_CARRY;

      // output 0
      SQRADD(a[0],a[0]);
      COMBA_STORE(b[0]);

      // output 1
      CARRY_FORWARD;
      SQRADD2(a[0], a[1]);
      COMBA_STORE(b[1]);

      // output 2
      CARRY_FORWARD;
      SQRADD2(a[0], a[2]);    SQRADD(a[1], a[1]);
      COMBA_STORE(b[2]);

      // output 3
      CARRY_FORWARD;
      SQRADD2(a[0], a[3]);    SQRADD2(a[1], a[2]);
      COMBA_STORE(b[3]);

      // output 4
      CARRY_FORWARD;
      SQRADD2(a[0], a[4]);    SQRADD2(a[1], a[3]);    SQRADD(a[2], a[2]);
      COMBA_STORE(b[4]);

      // output 5
      CARRY_FORWARD;
   SQRADDSC(a[0], a[5]); SQRADDAC(a[1], a[4]); SQRADDAC(a[2], a[3]); SQRADDDB;
      COMBA_STORE(b[5]);

      // output 6
      CARRY_FORWARD;
   SQRADDSC(a[0], a[6]); SQRADDAC(a[1], a[5]); SQRADDAC(a[2], a[4]); SQRADDDB; SQRADD(a[3], a[3]);
      COMBA_STORE(b[6]);

      // output 7
      CARRY_FORWARD;
   SQRADDSC(a[0], a[7]); SQRADDAC(a[1], a[6]); SQRADDAC(a[2], a[5]); SQRADDAC(a[3], a[4]); SQRADDDB;
      COMBA_STORE(b[7]);

      // output 8
      CARRY_FORWARD;
   SQRADDSC(a[0], a[8]); SQRADDAC(a[1], a[7]); SQRADDAC(a[2], a[6]); SQRADDAC(a[3], a[5]); SQRADDDB; SQRADD(a[4], a[4]);
      COMBA_STORE(b[8]);

      // output 9
      CARRY_FORWARD;
   SQRADDSC(a[0], a[9]); SQRADDAC(a[1], a[8]); SQRADDAC(a[2], a[7]); SQRADDAC(a[3], a[6]); SQRADDAC(a[4], a[5]); SQRADDDB;
      COMBA_STORE(b[9]);

      // output 10
      CARRY_FORWARD;
   SQRADDSC(a[0], a[10]); SQRADDAC(a[1], a[9]); SQRADDAC(a[2], a[8]); SQRADDAC(a[3], a[7]); SQRADDAC(a[4], a[6]); SQRADDDB; SQRADD(a[5], a[5]);
      COMBA_STORE(b[10]);

      // output 11
      CARRY_FORWARD;
   SQRADDSC(a[0], a[11]); SQRADDAC(a[1], a[10]); SQRADDAC(a[2], a[9]); SQRADDAC(a[3], a[8]); SQRADDAC(a[4], a[7]); SQRADDAC(a[5], a[6]); SQRADDDB;
      COMBA_STORE(b[11]);

      // output 12
      CARRY_FORWARD;
   SQRADDSC(a[0], a[12]); SQRADDAC(a[1], a[11]); SQRADDAC(a[2], a[10]); SQRADDAC(a[3], a[9]); SQRADDAC(a[4], a[8]); SQRADDAC(a[5], a[7]); SQRADDDB; SQRADD(a[6], a[6]);
      COMBA_STORE(b[12]);

      // output 13
      CARRY_FORWARD;
   SQRADDSC(a[0], a[13]); SQRADDAC(a[1], a[12]); SQRADDAC(a[2], a[11]); SQRADDAC(a[3], a[10]); SQRADDAC(a[4], a[9]); SQRADDAC(a[5], a[8]); SQRADDAC(a[6], a[7]); SQRADDDB;
      COMBA_STORE(b[13]);

      // output 14
      CARRY_FORWARD;
   SQRADDSC(a[0], a[14]); SQRADDAC(a[1], a[13]); SQRADDAC(a[2], a[12]); SQRADDAC(a[3], a[11]); SQRADDAC(a[4], a[10]); SQRADDAC(a[5], a[9]); SQRADDAC(a[6], a[8]); SQRADDDB; SQRADD(a[7], a[7]);
      COMBA_STORE(b[14]);

      // output 15
      CARRY_FORWARD;
   SQRADDSC(a[1], a[14]); SQRADDAC(a[2], a[13]); SQRADDAC(a[3], a[12]); SQRADDAC(a[4], a[11]); SQRADDAC(a[5], a[10]); SQRADDAC(a[6], a[9]); SQRADDAC(a[7], a[8]); SQRADDDB;
      COMBA_STORE(b[15]);

      // output 16
      CARRY_FORWARD;
   SQRADDSC(a[2], a[14]); SQRADDAC(a[3], a[13]); SQRADDAC(a[4], a[12]); SQRADDAC(a[5], a[11]); SQRADDAC(a[6], a[10]); SQRADDAC(a[7], a[9]); SQRADDDB; SQRADD(a[8], a[8]);
      COMBA_STORE(b[16]);

      // output 17
      CARRY_FORWARD;
   SQRADDSC(a[3], a[14]); SQRADDAC(a[4], a[13]); SQRADDAC(a[5], a[12]); SQRADDAC(a[6], a[11]); SQRADDAC(a[7], a[10]); SQRADDAC(a[8], a[9]); SQRADDDB;
      COMBA_STORE(b[17]);

      // output 18
      CARRY_FORWARD;
   SQRADDSC(a[4], a[14]); SQRADDAC(a[5], a[13]); SQRADDAC(a[6], a[12]); SQRADDAC(a[7], a[11]); SQRADDAC(a[8], a[10]); SQRADDDB; SQRADD(a[9], a[9]);
      COMBA_STORE(b[18]);

      // output 19
      CARRY_FORWARD;
   SQRADDSC(a[5], a[14]); SQRADDAC(a[6], a[13]); SQRADDAC(a[7], a[12]); SQRADDAC(a[8], a[11]); SQRADDAC(a[9], a[10]); SQRADDDB;
      COMBA_STORE(b[19]);

      // output 20
      CARRY_FORWARD;
   SQRADDSC(a[6], a[14]); SQRADDAC(a[7], a[13]); SQRADDAC(a[8], a[12]); SQRADDAC(a[9], a[11]); SQRADDDB; SQRADD(a[10], a[10]);
      COMBA_STORE(b[20]);

      // output 21
      CARRY_FORWARD;
   SQRADDSC(a[7], a[14]); SQRADDAC(a[8], a[13]); SQRADDAC(a[9], a[12]); SQRADDAC(a[10], a[11]); SQRADDDB;
      COMBA_STORE(b[21]);

      // output 22
      CARRY_FORWARD;
   SQRADDSC(a[8], a[14]); SQRADDAC(a[9], a[13]); SQRADDAC(a[10], a[12]); SQRADDDB; SQRADD(a[11], a[11]);
      COMBA_STORE(b[22]);

      // output 23
      CARRY_FORWARD;
   SQRADDSC(a[9], a[14]); SQRADDAC(a[10], a[13]); SQRADDAC(a[11], a[12]); SQRADDDB;
      COMBA_STORE(b[23]);

      // output 24
      CARRY_FORWARD;
      SQRADD2(a[10], a[14]);    SQRADD2(a[11], a[13]);    SQRADD(a[12], a[12]);
      COMBA_STORE(b[24]);

      // output 25
      CARRY_FORWARD;
      SQRADD2(a[11], a[14]);    SQRADD2(a[12], a[13]);
      COMBA_STORE(b[25]);

      // output 26
      CARRY_FORWARD;
      SQRADD2(a[12], a[14]);    SQRADD(a[13], a[13]);
      COMBA_STORE(b[26]);

      // output 27
      CARRY_FORWARD;
      SQRADD2(a[13], a[14]);
      COMBA_STORE(b[27]);

      // output 28
      CARRY_FORWARD;
      SQRADD(a[14], a[14]);
      COMBA_STORE(b[28]);
      COMBA_STORE2(b[29]);
      COMBA_FINI;

      B->used = 30;
      B->sign = FP_ZPOS;
      memcpy(B->dp, b, 30 * sizeof(fp_digit));
      fp_clamp(B);
      break;

   case 16:
      a = A->dp;
      COMBA_START;

      // clear carries
      CLEAR_CARRY;

      // output 0
      SQRADD(a[0],a[0]);
      COMBA_STORE(b[0]);

      // output 1
      CARRY_FORWARD;
      SQRADD2(a[0], a[1]);
      COMBA_STORE(b[1]);

      // output 2
      CARRY_FORWARD;
      SQRADD2(a[0], a[2]);    SQRADD(a[1], a[1]);
      COMBA_STORE(b[2]);

      // output 3
      CARRY_FORWARD;
      SQRADD2(a[0], a[3]);    SQRADD2(a[1], a[2]);
      COMBA_STORE(b[3]);

      // output 4
      CARRY_FORWARD;
      SQRADD2(a[0], a[4]);    SQRADD2(a[1], a[3]);    SQRADD(a[2], a[2]);
      COMBA_STORE(b[4]);

      // output 5
      CARRY_FORWARD;
   SQRADDSC(a[0], a[5]); SQRADDAC(a[1], a[4]); SQRADDAC(a[2], a[3]); SQRADDDB;
      COMBA_STORE(b[5]);

      // output 6
      CARRY_FORWARD;
   SQRADDSC(a[0], a[6]); SQRADDAC(a[1], a[5]); SQRADDAC(a[2], a[4]); SQRADDDB; SQRADD(a[3], a[3]);
      COMBA_STORE(b[6]);

      // output 7
      CARRY_FORWARD;
   SQRADDSC(a[0], a[7]); SQRADDAC(a[1], a[6]); SQRADDAC(a[2], a[5]); SQRADDAC(a[3], a[4]); SQRADDDB;
      COMBA_STORE(b[7]);

      // output 8
      CARRY_FORWARD;
   SQRADDSC(a[0], a[8]); SQRADDAC(a[1], a[7]); SQRADDAC(a[2], a[6]); SQRADDAC(a[3], a[5]); SQRADDDB; SQRADD(a[4], a[4]);
      COMBA_STORE(b[8]);

      // output 9
      CARRY_FORWARD;
   SQRADDSC(a[0], a[9]); SQRADDAC(a[1], a[8]); SQRADDAC(a[2], a[7]); SQRADDAC(a[3], a[6]); SQRADDAC(a[4], a[5]); SQRADDDB;
      COMBA_STORE(b[9]);

      // output 10
      CARRY_FORWARD;
   SQRADDSC(a[0], a[10]); SQRADDAC(a[1], a[9]); SQRADDAC(a[2], a[8]); SQRADDAC(a[3], a[7]); SQRADDAC(a[4], a[6]); SQRADDDB; SQRADD(a[5], a[5]);
      COMBA_STORE(b[10]);

      // output 11
      CARRY_FORWARD;
   SQRADDSC(a[0], a[11]); SQRADDAC(a[1], a[10]); SQRADDAC(a[2], a[9]); SQRADDAC(a[3], a[8]); SQRADDAC(a[4], a[7]); SQRADDAC(a[5], a[6]); SQRADDDB;
      COMBA_STORE(b[11]);

      // output 12
      CARRY_FORWARD;
   SQRADDSC(a[0], a[12]); SQRADDAC(a[1], a[11]); SQRADDAC(a[2], a[10]); SQRADDAC(a[3], a[9]); SQRADDAC(a[4], a[8]); SQRADDAC(a[5], a[7]); SQRADDDB; SQRADD(a[6], a[6]);
      COMBA_STORE(b[12]);

      // output 13
      CARRY_FORWARD;
   SQRADDSC(a[0], a[13]); SQRADDAC(a[1], a[12]); SQRADDAC(a[2], a[11]); SQRADDAC(a[3], a[10]); SQRADDAC(a[4], a[9]); SQRADDAC(a[5], a[8]); SQRADDAC(a[6], a[7]); SQRADDDB;
      COMBA_STORE(b[13]);

      // output 14
      CARRY_FORWARD;
   SQRADDSC(a[0], a[14]); SQRADDAC(a[1], a[13]); SQRADDAC(a[2], a[12]); SQRADDAC(a[3], a[11]); SQRADDAC(a[4], a[10]); SQRADDAC(a[5], a[9]); SQRADDAC(a[6], a[8]); SQRADDDB; SQRADD(a[7], a[7]);
      COMBA_STORE(b[14]);

      // output 15
      CARRY_FORWARD;
   SQRADDSC(a[0], a[15]); SQRADDAC(a[1], a[14]); SQRADDAC(a[2], a[13]); SQRADDAC(a[3], a[12]); SQRADDAC(a[4], a[11]); SQRADDAC(a[5], a[10]); SQRADDAC(a[6], a[9]); SQRADDAC(a[7], a[8]); SQRADDDB;
      COMBA_STORE(b[15]);

      // output 16
      CARRY_FORWARD;
   SQRADDSC(a[1], a[15]); SQRADDAC(a[2], a[14]); SQRADDAC(a[3], a[13]); SQRADDAC(a[4], a[12]); SQRADDAC(a[5], a[11]); SQRADDAC(a[6], a[10]); SQRADDAC(a[7], a[9]); SQRADDDB; SQRADD(a[8], a[8]);
      COMBA_STORE(b[16]);

      // output 17
      CARRY_FORWARD;
   SQRADDSC(a[2], a[15]); SQRADDAC(a[3], a[14]); SQRADDAC(a[4], a[13]); SQRADDAC(a[5], a[12]); SQRADDAC(a[6], a[11]); SQRADDAC(a[7], a[10]); SQRADDAC(a[8], a[9]); SQRADDDB;
      COMBA_STORE(b[17]);

      // output 18
      CARRY_FORWARD;
   SQRADDSC(a[3], a[15]); SQRADDAC(a[4], a[14]); SQRADDAC(a[5], a[13]); SQRADDAC(a[6], a[12]); SQRADDAC(a[7], a[11]); SQRADDAC(a[8], a[10]); SQRADDDB; SQRADD(a[9], a[9]);
      COMBA_STORE(b[18]);

      // output 19
      CARRY_FORWARD;
   SQRADDSC(a[4], a[15]); SQRADDAC(a[5], a[14]); SQRADDAC(a[6], a[13]); SQRADDAC(a[7], a[12]); SQRADDAC(a[8], a[11]); SQRADDAC(a[9], a[10]); SQRADDDB;
      COMBA_STORE(b[19]);

      // output 20
      CARRY_FORWARD;
   SQRADDSC(a[5], a[15]); SQRADDAC(a[6], a[14]); SQRADDAC(a[7], a[13]); SQRADDAC(a[8], a[12]); SQRADDAC(a[9], a[11]); SQRADDDB; SQRADD(a[10], a[10]);
      COMBA_STORE(b[20]);

      // output 21
      CARRY_FORWARD;
   SQRADDSC(a[6], a[15]); SQRADDAC(a[7], a[14]); SQRADDAC(a[8], a[13]); SQRADDAC(a[9], a[12]); SQRADDAC(a[10], a[11]); SQRADDDB;
      COMBA_STORE(b[21]);

      // output 22
      CARRY_FORWARD;
   SQRADDSC(a[7], a[15]); SQRADDAC(a[8], a[14]); SQRADDAC(a[9], a[13]); SQRADDAC(a[10], a[12]); SQRADDDB; SQRADD(a[11], a[11]);
      COMBA_STORE(b[22]);

      // output 23
      CARRY_FORWARD;
   SQRADDSC(a[8], a[15]); SQRADDAC(a[9], a[14]); SQRADDAC(a[10], a[13]); SQRADDAC(a[11], a[12]); SQRADDDB;
      COMBA_STORE(b[23]);

      // output 24
      CARRY_FORWARD;
   SQRADDSC(a[9], a[15]); SQRADDAC(a[10], a[14]); SQRADDAC(a[11], a[13]); SQRADDDB; SQRADD(a[12], a[12]);
      COMBA_STORE(b[24]);

      // output 25
      CARRY_FORWARD;
   SQRADDSC(a[10], a[15]); SQRADDAC(a[11], a[14]); SQRADDAC(a[12], a[13]); SQRADDDB;
      COMBA_STORE(b[25]);

      // output 26
      CARRY_FORWARD;
      SQRADD2(a[11], a[15]);    SQRADD2(a[12], a[14]);    SQRADD(a[13], a[13]);
      COMBA_STORE(b[26]);

      // output 27
      CARRY_FORWARD;
      SQRADD2(a[12], a[15]);    SQRADD2(a[13], a[14]);
      COMBA_STORE(b[27]);

      // output 28
      CARRY_FORWARD;
      SQRADD2(a[13], a[15]);    SQRADD(a[14], a[14]);
      COMBA_STORE(b[28]);

      // output 29
      CARRY_FORWARD;
      SQRADD2(a[14], a[15]);
      COMBA_STORE(b[29]);

      // output 30
      CARRY_FORWARD;
      SQRADD(a[15], a[15]);
      COMBA_STORE(b[30]);
      COMBA_STORE2(b[31]);
      COMBA_FINI;

      B->used = 32;
      B->sign = FP_ZPOS;
      memcpy(B->dp, b, 32 * sizeof(fp_digit));
      fp_clamp(B);
      break;

}
}
/*
// c = a * a (mod b)
int fp_sqrmod(fp_int *a, fp_int *b, fp_int *c) {
  fp_int tmp;
  fp_zero(&tmp);
  fp_sqr(a, &tmp);
  return fp_mod(&tmp, b, c);
}
*/
