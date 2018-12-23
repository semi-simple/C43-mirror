/* TomsFastMath, a fast ISO C bignum library.
 *
 * This project is meant to fill in where LibTomMath
 * falls short.  That is speed ;-)
 *
 * This project is public domain and free for all purposes.
 *
 * Tom St Denis, tomstdenis@gmail.com
 */
#ifndef TFM_PRIVATE_H_
#define TFM_PRIVATE_H_

#include "tfm.h"

/* VARIOUS LOW LEVEL STUFFS */
void s_fp_add(fp_int *a, fp_int *b, fp_int *c);
void s_fp_sub(fp_int *a, fp_int *b, fp_int *c);
void fp_reverse(unsigned char *s, int len);

void fp_mul_comba(fp_int *A, fp_int *B, fp_int *C);
void fp_mul_comba_small(fp_int *A, fp_int *B, fp_int *C);

void fp_sqr_comba(fp_int *A, fp_int *B);
void fp_sqr_comba_small(fp_int *A, fp_int *B);

extern const char *fp_s_rmap;

#endif
