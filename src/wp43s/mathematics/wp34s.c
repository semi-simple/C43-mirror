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
 * \file wp34s.c
 ***********************************************/

#include "wp43s.h"
#include "mathematics/gammaConstants.h"

const real34_t * const angle360[] = {
  (real34_t *)"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x70\x00\x2e", // 360 deg
  (real34_t *)"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x40\x00\x32", // 400 grad
  (real34_t *)"\x06\x64\x6b\xbe\x5a\xad\xda\xa9\x6e\x3e\x87\x2d\xb3\xd2\xff\x39", // 2pi rad
  (real34_t *)"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xc0\xff\x29"  // 2   multpi
};

const real34_t * const angle180[] = {
  (real34_t *)"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xc0\x40\x00\x26", // 180 deg
  (real34_t *)"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x40\x00\x2a", // 200 grad
  (real34_t *)"\x83\xe6\xb5\xda\xd0\x62\xe2\xb4\xfb\xb3\x53\xeb\x1a\xcc\xff\x2d", // pi  rad
  (real34_t *)"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xc0\xff\x25"  // 1   multpi
};

const real34_t * const angle90[]  = {
  (real34_t *)"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x6e", // 90 deg
  (real34_t *)"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x40\x00\x26", // 100 grad
  (real34_t *)"\xd1\xe7\xbc\x71\x68\x31\x65\xec\xb1\xf6\xa6\xe9\x0f\xef\xff\x25", // pi/2 rad
  (real34_t *)"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x80\xff\x35"  // 1/2 multpi
};

const real34_t * const angle45[]  = {
  (real34_t *)"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x28\x00\x32", // 45 deg
  (real34_t *)"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x36", // 50 grad
  (real34_t *)"\xd7\x7b\x83\x25\xc2\xd6\xe8\xb1\x22\xbf\x33\x3f\xd2\x95\xff\x3d", // pi/4 rad
  (real34_t *)"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xa8\xff\x29"  // 1/4 multpi
};


/******************************************************
 * This functions are borrowed from the WP34S project
 ******************************************************/

/* Have to be careful here to ensure that every function we call can handle
 * the increased size of the numbers we're using.
 */
void WP34S_cvt_2rad_sincos(real34_t *sin, real34_t *cos, const real34_t *angle, uint32_t angularMode) {
	 bool_t sinNeg = false, cosNeg = false, swap = false;
  real34_t angle34;
  real451_t angle451;

  real34Copy(angle, &angle34);

	 if(angularMode == AM_DMS) {
    angularMode = AM_DEGREE;
    convertAngle34FromTo(&angle34, AM_DMS, AM_DEGREE);
	 }

	 // sin(-x) = -sin(x), cos(-x) = cos(x)
	 if(real34IsNegative(&angle34)) {
	  	sinNeg = true;
	  	real34SetPositiveSign(&angle34);
	 }

 	real34ToReal451(&angle34, (decNumber *)&angle451);
 	switch(angularMode) {
    case AM_DEGREE:
    	 real451Remainder((decNumber *)&angle451, (decNumber *)const51_360, (decNumber *)&angle451); // mod(angle34, 360°) --> angle34
     	break;

    case AM_GRAD:
    	 real451Remainder((decNumber *)&angle451, (decNumber *)const51_400, (decNumber *)&angle451); // mod(angle34, 360°) --> angle34
     	break;

    case AM_RADIAN:
    	 real451Remainder((decNumber *)&angle451, (decNumber *)const451_2pi, (decNumber *)&angle451); // mod(angle34, 360°) --> angle34
     	break;

    case AM_MULTPI:
    	 real451Remainder((decNumber *)&angle451, (decNumber *)const51_2, (decNumber *)&angle451); // mod(angle34, 360°) --> angle34
     	break;

    default: {}
 	}

	 if(real451IsNaN(&angle451)) {
	   if(sin != NULL) {
	 	  real34Copy(const34_NaN, sin);
	 	 }
	   if(cos != NULL) {
	  	  real34Copy(const34_NaN, cos);
	  	}
   return;
	 }

	 real451ToReal34((decNumber *)&angle451, &angle34);

	 // sin(180+x) = -sin(x), cos(180+x) = -cos(x)
 	if(real34CompareGreaterEqual(&angle34, angle180[angularMode])) { // angle34 >= 180°
	  	real34Subtract(&angle34, angle180[angularMode], &angle34);     // angle34 - 180° --> angle34
	  	sinNeg = !sinNeg;
	  	cosNeg = !cosNeg;
  }

	 // sin(90+x) = cos(x), cos(90+x) = -sin(x)
	 if(real34CompareGreaterEqual(&angle34, angle90[angularMode])) { // angle34 >= 90°
		  real34Subtract(&angle34, angle90[angularMode], &angle34);     // angle34 - 90° --> angle34
		  swap = true;
		  cosNeg = !cosNeg;
  }

	 // sin(90-x) = cos(x), cos(90-x) = sin(x)
	 if(real34CompareEqual(&angle34, angle45[angularMode])) { // angle34 == 45°
	   if(sin != NULL) {
	 	  real34Copy(const34_root2on2, sin);
	 	 }
	   if(cos != NULL) {
	  	  real34Copy(const34_root2on2, cos);
	  	}
  }
	 else { // angle34 < 90
		  if(real34CompareGreaterThan(&angle34, angle45[angularMode])) { // angle34 > 45°
		   	real34Subtract(angle90[angularMode], &angle34, &angle34);    // 90° - angle34  --> angle34
			   swap = !swap;
   	}

    convertAngle34FromTo(&angle34, angularMode, AM_RADIAN);
		  WP34S_sincosTaylor(&angle34, swap?cos:sin, swap?sin:cos); // angle34 in radian
  }

  if(sin != NULL) {
    if(sinNeg) {
      real34SetNegativeSign(sin);
    }
		  if(real34IsZero(sin)) {
      real34SetPositiveSign(sin);
		  }
  }

  if(cos != NULL) {
    if(cosNeg) {
      real34SetNegativeSign(cos);
    }
		  if(real34IsZero(cos)) {
      real34SetPositiveSign(cos);
		  }
  }
}


/* Calculate sin and cos by Taylor series
 */
void WP34S_sincosTaylor(const real34_t *a, real34_t *sinOut, real34_t *cosOut) { // a in radian
 	real51_t angle, a2, t, j, z, one, sin, cos, compare;
 	int i, odd;
 	bool_t finSin = (sinOut == NULL), finCos = (cosOut == NULL);

 	real34ToReal51(a, &angle);
 	real51Multiply(&angle, &angle, &a2);
 	uInt32ToReal51(1, &j);
 	uInt32ToReal51(1, &t);
 	uInt32ToReal51(1, &sin);
 	uInt32ToReal51(1, &cos);
 	uInt32ToReal51(1, &one);

 	for(i=1; !(finSin && finCos) && i < 1000; i++) {
	  	odd = i & 1;

  		real51Add(&j, &one, &j);
	  	real51Divide(&a2, &j, &z);
  		real51Multiply(&t, &z, &t);

	  	if(!finCos) {
	   		real51Copy(&cos, &z);

		   	if(odd) {
			   	 real51Subtract(&cos, &t, &cos);
      }
			   else {
			   	 real51Add(&cos, &t, &cos);
      }

      real51Compare(&cos, &z, &compare);
		   	finCos = (real51ToInt32(&compare) == 0);
		  }

  		real51Add(&j, &one, &j);
		  real51Divide(&t, &j, &t);

	  	if(!finSin) {
		   	real51Copy(&sin, &z);

			   if(odd) {
		   	  real51Subtract(&sin, &t, &sin);
		   	}
		   	else {
		   	  real51Add(&sin, &t, &sin);
		    }

      real51Compare(&sin, &z, &compare);
		   	finSin = (real51ToInt32(&compare) == 0);
	  	}
	 }

	 if(sinOut != NULL) {
		  real51Multiply(&sin, &angle, &angle);
		  real51ToReal34(&angle, sinOut);
  }

 	if(cosOut != NULL) {
 		 real51ToReal34(&cos, cosOut);
		}
}



void WP34S_do_atan(const real34_t *x, real34_t *angle) {
 	real34_t a, b, a2, t, j, z, last;
 	int doubles = 0;
 	int invert;
 	int n;
 	int neg = real34IsNegative(x);

  real34Copy(x, &a);

 	// arrange for a >= 0
 	if(neg) {
 		 real34ChangeSign(&a);
 	}

 	// reduce range to 0 <= a < 1, using atan(x) = pi/2 - atan(1/x)
 	invert = real34CompareGreaterThan(&a, const34_1);
 	if(invert) {
 		 real34Divide(const34_1, &a, &a);
 	}

 	// Range reduce to small enough limit to use taylor series using:
	 //  tan(x/2) = tan(x)/(1+sqrt(1+tan(x)^2))
	 for(n=0; n<1000; n++) {
	  	if(real34CompareLessEqual(&a, const34_0_1)) {
		  	 break;
		  }
		  doubles++;
		  // a = a/(1+sqrt(1+a^2)) -- at most 3 iterations.
	  	real34Multiply(&a, &a, &b);
		  real34Add(&b, const34_1, &b);
		  real34SquareRoot(&b, &b);
		  real34Add(&b, const34_1, &b);
		  real34Divide(&a, &b, &a);
	 }

	 // Now Taylor series
	 // tan(x) = x(1-x^2/3+x^4/5-x^6/7...)
	 // We calculate pairs of terms and stop when the estimate doesn't change
 	real34Copy(const34_3, angle);
 	real34Copy(const34_5, &j);
 	real34Multiply(&a, &a, &a2);	// a^2
 	real34Copy(&a2, &t);
 	real34Divide(&t, angle, angle);	// s = 1-t/3 -- first two terms
 	real34Subtract(const34_1, angle, angle);

 	do {	// Loop until there is no digits changed
	  	real34Copy(angle, &last);

  		real34Multiply(&t, &a2, &t);
	  	real34Divide(&t, &j, &z);
		  real34Add(angle, &z, angle);

		  real34Add(&j, const34_2, &j);

  		real34Multiply(&t, &a2, &t);
	  	real34Divide(&t, &j, &z);
		  real34Subtract(angle, &z, angle);

		  real34Add(&j, const34_2, &j);

	 } while(!real34CompareEqual(angle, &last));

	 real34Multiply(angle, &a, angle);

	 while(doubles) {
  		real34Add(angle, angle, angle);
  		doubles--;
	 }

 	if(invert) {
  	 real34Subtract(const34_piOn2, angle, angle);
  }

 	if(neg) {
 		 real34ChangeSign(angle);
 	}
}



void WP34S_do_atan2(const real34_t *y, const real34_t *x, real34_t *atan) {
	 real34_t r, t;
	 const int xNeg = real34IsNegative(x);
	 const int yNeg = real34IsNegative(y);

	 if(real34IsNaN(x) || real34IsNaN(y)) {
	   real34Copy(const34_NaN, atan);
		  return;
	 }

	 if(real34CompareEqual(y, const34_0)) {
		  if(yNeg) {
			   if(real34CompareEqual(x, const34_0)) {
				    if(xNeg) {
					     real34Copy(const34_pi, atan);
					     real34SetNegativeSign(atan);
				    }
				    else {
					     real34Copy(y, atan);
					   }
			   }
			   else if(xNeg) {
			     real34Copy(const34_pi, atan);
			     real34SetNegativeSign(atan);
			   }
      else {
			     real34Copy(y, atan);
			   }
		  }
    else {
			   if(real34CompareEqual(x, const34_0)) {
			    	if(xNeg) {
 			      real34Copy(const34_pi, atan);
 			    }
				    else {
					     real34Zero(atan);
					   }
			   }
			   else if(xNeg) {
		      real34Copy(const34_pi, atan);
		    }
		   	else {
			     real34Zero(atan);
			   }
		  }

		  return;
	 }

  if(real34CompareEqual(x, const34_0)) {
	  	real34Copy(const34_piOn2, atan);
	  	if(yNeg) {
      real34SetNegativeSign(atan);
    }

		  return;
	 }

	 if(real34IsInfinite(x)) {
	  	if(xNeg) {
		   	if(real34IsInfinite(y)) {
			    	real34Copy(const34_3piOn4, atan);
		    		if(yNeg) {
          real34SetNegativeSign(atan);
        }
			   }
			   else {
			    	real34Copy(const34_pi, atan);
			    	if(yNeg) {
          real34SetNegativeSign(atan);
        }
			   }
		  }
		  else {
		   	if(real34IsInfinite(y)) {
			    	real34Copy(const34_piOn4, atan);
	    			if(yNeg) {
          real34SetNegativeSign(atan);
        }
			   }
			   else {
		    		real34Zero(atan);
		    		if(yNeg) {
          real34SetNegativeSign(atan);
        }
			   }
		  }

		  return;
	 }

 	if(real34IsInfinite(y)) {
	  	real34Copy(const34_piOn2, atan);
	  	if(yNeg) {
      real34SetNegativeSign(atan);
    }

		  return;
	 }

 	real34Divide(y, x, &t);
	 WP34S_do_atan(&t, &r);
	 if(xNeg) {
	  	real34Copy(const34_pi, &t);
	  	if(yNeg) {
     real34SetNegativeSign(&t);
    }
	 }
	 else {
	 	 real34Zero(&t);
	 }

 	real34Add(&r, &t, atan);
 	if(real34CompareEqual(atan, const34_0) && yNeg) {
    real34SetNegativeSign(atan);
  }
}



void WP34S_do_asin(const real34_t *x, real34_t *angle) {
	 real34_t abx, z;

	 if(real34IsNaN(x)) {
	  	real34Copy(const34_NaN, angle);

	  	return;
	 }

  real34Copy(x, &abx);
	 real34SetPositiveSign(&abx);
	 if(real34CompareGreaterThan(&abx, const34_1)) {
	  	real34Copy(const34_NaN, angle);

	  	return;
	 }

	 // angle = 2*atan(x/(1+sqrt(1-x*x)))
	 real34Multiply(x, x, &z);
	 real34Subtract(const34_1, &z, &z);
  real34SquareRoot(&z, &z);
	 real34Add(&z, const34_1, &z);
	 real34Divide(x, &z, &z);
	 WP34S_do_atan(&z, &abx);
	 real34Add(&abx, &abx, angle);
}



void WP34S_do_acos(const real34_t *x, real34_t *angle) {
 	real34_t abx, z;

	 if(real34IsNaN(x)) {
	  	real34Copy(const34_NaN, angle);

	  	return;
	 }

  real34Copy(x, &abx);
	 real34SetPositiveSign(&abx);
	 if(real34CompareGreaterThan(&abx, const34_1)) {
	  	real34Copy(const34_NaN, angle);

	  	return;
	 }

	 // angle = 2*atan((1-x)/sqrt(1-x*x))
	 if(real34CompareEqual(x, const34_1)) {
	 	 real34Zero(angle);
	 }
	 else {
 	 	real34Multiply(x, x, &z);
  		real34Subtract(const34_1, &z, &z);
    real34SquareRoot(&z, &z);
  		real34Subtract(const34_1, x, &abx);
  		real34Divide(&abx, &z, &z);
  	 WP34S_do_atan(&z, &abx);
  	 real34Add(&abx, &abx, angle);
	 }
}



/* Check if a number is an integer.
 */
bool_t WP34S_is_int(const real34_t *x) {
 	real34_t r, y;

 	if(real34IsNaN(x)) {
 		 return false;
 	}
 	if(real34IsInfinite(x)) {
 		 return true;
 	}

 	real34ToIntegral(x, &y);
 	real34Subtract(x, &y, &r);

 	return real34CompareEqual(&r, const34_0);
}



static void WP34S_dn_LnGamma(const real34_t *x, real34_t *res) {
	 real51_t r, s, t, u, v, xin;
	 int32_t k;

	 real34ToReal51(x, &xin);
 	real51Zero(&s);
 	real51Add(&xin, const51_21, &t);
 	for(k=20; k>=0; k--) {
	  	real51Divide(gamma_consts[k], &t, &u);
	  	real51Subtract(&t, const51_1, &t);
	  	real51Add(&s, &u, &s);
  }

	 real51Add(&s, const51_gammaC00, &t);
	 real51Ln(&t, &s);

  //		r = z + g + .5;
	 real51Add(&xin, const51_gammaR, &r);

  //		r = log(R[0][0]) + (z+.5) * log(r) - r;
	 real51Ln(&r, &u);
	 real51Add(&xin, const51_0_5, &t);
	 real51Multiply(&u, &t, &v);

	 real51Subtract(&v, &r, &u);
	 real51Add(&u, &s, &r);
	 real51ToReal34(&r, res);
}



// common code for the [GAMMA] and LN[GAMMA]
static void WP34S_Gamma_LnGamma(const real34_t *xin, const bool_t calculateLn, real34_t *res) {
	 real34_t x, t;
	 int reflec = 0;

	 // Check for special cases
	 if(real34IsSpecial(xin)) {
	  	if(real34IsInfinite(xin) && !real34IsNegative(xin)) {
	  	  real34Copy(const34_plusInfinity, res);
		  	 return;
    }

 	  real34Copy(const34_NaN, res);
		  return;
	 }

	 // Handle x approximately zero case
	 real34Copy(xin, &t);
	 real34SetPositiveSign(&t);
	 if(real34CompareLessThan(&t, const34_1e_24)) {
	  	if(real34CompareEqual(xin, const34_0)) {
   	  real34Copy(const34_NaN, res);
      return;
    }
		  real34Divide(const34_1, xin, &x);
	  	real34Subtract(&x, const34_egamma, res);
	  	if(calculateLn) {
	  	 real34Ln(res, res);
	  	}
		  return;
	 }

	 // Correct our argument and begin the inversion if it is negative
	 if(real34CompareLessEqual(xin, const34_0)) {
	  	reflec = 1;
	  	real34Subtract(const34_1, xin, &t);
	  	if(WP34S_is_int(&t)) {
   	  real34Copy(const34_NaN, res);
	  		 return;
    }
	  	real34Subtract(&t, const34_1, &x);
	 }
	 else {
	  	real34Subtract(xin, const34_1, &x);

	 	 // Provide a fast path evaluation for positive integer arguments that aren't too large
	 	 // The threshold for overflow is 205! (i.e. 204! is within range and 205! isn't).
	 	 if(WP34S_is_int(&x) && !real34CompareEqual(xin, const34_0) && real34CompareLessEqual(&x, const34_204)) {
	 	  	real34Copy(const34_1, res);
	 	  	while(real34CompareGreaterThan(&x, const34_0)) {
	 	 	  	real34Multiply(res, &x, res);
	 	 		  real34Subtract(&x, const34_1, &x);
	 	 	 }
	 	 	 if(calculateLn) {
	 	 	 	 real34Ln(res, res);
	 	 	 	 return;
      }
	 	  	return;
	 	 }
	 }

 	WP34S_dn_LnGamma(&x, res);
 	if(!calculateLn) {
 	  real34Exp(res, res);
 	}

 	// Finally invert if we started with a negative argument
 	if(reflec) {
 	 	// figure out xin * PI mod 2PI
 	 	real34Remainder(xin, const34_2, &t);
 	 	real34Multiply(&t, const34_pi, &t);
 	 	WP34S_sincosTaylor(&t, &x, NULL); // t in radian
  		if(calculateLn) {
  		 	real34Divide(const34_pi, &x, &t);
  		 	real34Ln(&t, &t);
  		 	real34Subtract(&t, res, res);
  		}
  		else {
  	 		real34Multiply(&x, res, &t);
  	 		real34Divide(const34_pi, &t, res);
  	 }
  }
}



void WP34S_real34Gamma(const real34_t *xin, real34_t *res) {
	 real34_t x;

  real34Copy(xin, &x);
	 WP34S_Gamma_LnGamma(&x, false, res);
}



void WP34S_real34Factorial(const real34_t *xin, real34_t *res) {
	 real34_t x;

	 real34Add(xin, const34_1, &x);
	 WP34S_Gamma_LnGamma(&x, false, res);
}



// The log gamma function.
void WP34S_real34LnGamma(const real34_t *xin, real34_t *res) {
	 real34_t x;

  real34Copy(xin, &x);
	 WP34S_Gamma_LnGamma(&x, true, res);
}



/* Natural logarithm.
 *
 * Take advantage of the fact that we store our numbers in the form: m * 10^e
 * so log(m * 10^e) = log(m) + e * log(10)
 * do this so that m is always in the range 0.1 <= m < 2.  However if the number
 * is already in the range 0.5 .. 1.5, this step is skipped.
 *
 * Then use the fact that ln(x^2) = 2 * ln(x) to range reduce the mantissa
 * into 1/sqrt(2) <= m < 2.
 *
 * Finally, apply the series expansion:
 *   ln(x) = 2(a+a^3/3+a^5/5+...) where a=(x-1)/(x+1)
 * which converges quickly for an argument near unity.
 */
void WP34S_real51Ln(const real51_t *xin, real51_t *res) {
 	real51_t z, t, f, n, m, i, v, w, e;
 	int32_t expon;

 	if(real51IsSpecial(xin)) {
	  	if(real51IsNaN(xin) || real51IsNegative(xin)) {
  	   real51Copy(const51_NaN, res);
		  	 return;
    }

	   real51Copy(const51_plusInfinity, res);
	  	return;
  }

	 if(real51CompareLessEqual(xin, const51_0)) {
	  	if(real51IsNegative(xin)) {
  	   real51Copy(const51_NaN, res);
		  	 return;
    }

	   real51Copy(const51_minusInfinity, res);
		  return;
	 }

	 real51Copy(xin, &z);
	 real51Copy(const51_2, &f);
	 real51Subtract(xin, const51_1, &t);
	 real51Copy(&t, &v);
	 real51SetPositiveSign(&v);
	 if(real51CompareGreaterThan(&v, const51_0_5)) {
	  	expon = z.exponent + z.digits;
	  	z.exponent = -z.digits;
	 }
	 else {
		  expon = 0;
		}

  /* The too high case never happens
 	while(dn_le(const34_2, &z)) {
	  	real51Multiply(&f, const51_2, &f);
		  real51SquareRoot(&z, &z);
  } */

	 // Range reduce the value by repeated square roots.
	 // Making the constant here larger will reduce the number of later
	 // iterations at the expense of more square root operations.
	 while(real51CompareLessEqual(&z, const51_root2on2)) {
	  	real51Multiply(&f, const51_2, &f);
		  real51SquareRoot(&z, &z);
  }

 	real51Add(&z, const51_1, &t);
 	real51Subtract(&z, const51_1, &v);
 	real51Divide(&v, &t, &n);
 	real51Copy(&n, &v);
 	real51Multiply(&v, &v, &m);
 	real51Copy(const51_3, &i);

	 for(;;) {
  		real51Multiply(&m, &n, &n);
	  	real51Divide(&n, &i, &e);
	  	real51Add(&v, &e, &w);
	  	if(WP34S_relative_error(&w, &v, const51_1e_37)) {
	  		 break;
	  	}
	  	real51Copy(&w, &v);
	  	real51Add(&i, const51_2, &i);
	 }

	 real51Multiply(&f, &w, res);
	 if(expon == 0) {
  	 return;
  }

	 int32ToReal51(expon, &e);
	 real51Multiply(&e, const51_ln10, &w);
	 real51Add(res, &w, res);
}



void WP34S_real51Log(const real51_t *xin, const real51_t *base, real51_t *res) {
 	real51_t y;

 	if(real51IsSpecial(xin)) {
	  	if(real51IsNaN(xin) || real51IsNegative(xin)) {
  	   real51Copy(const34_NaN, res);
		  	 return;
    }

	   real51Copy(const51_plusInfinity, res);
	  	return;
  }

	 WP34S_real51Ln(xin, &y);

 	real51Divide(&y, base, res);
}



void WP34S_real34Log2(const real34_t *xin, real34_t *res) {
  real51_t x, r;

  real34ToReal51(xin, &x);
 	WP34S_real51Log(&x, const51_ln2, &r);
  real51ToReal34(&r, res);
}



void WP34S_real34Log10(const real34_t *xin, real34_t *res) {
  real51_t x, r;

  real34ToReal51(xin, &x);
 	WP34S_real51Log(&x, const51_ln10, &r);

  real51ToReal34(&r, res);
}



void WP34S_real34Logxy(const real34_t *yin, const real34_t *xin, real34_t *res) {
  real51_t x, r, lx;

  real34ToReal51(xin, &x);
	 WP34S_real51Ln(&x, &lx);

  real34ToReal51(yin, &x);
	 WP34S_real51Log(&x, &lx, &r);

  real51ToReal34(&r, res);
}



bool_t WP34S_relative_error(const real51_t *x, const real51_t *y, const real51_t *tol) {
 	real51_t a;

 	if(real51CompareEqual(x, const51_0)) {
 	  real51Copy(y, &a);
 	  real51SetPositiveSign(&a);
 		 return real51CompareLessThan(&a, tol);
  }

 	real51Subtract(x, y, &a);
 	real51Divide(&a, x, &a);
 	real51SetPositiveSign(&a);
 	return real51CompareLessThan(&a, tol);
}




























#ifdef DO_NOT_COMPILE
#include "decn.h"
#include "xeq.h"
#include "consts.h"
#include "complex.h"
#include "stats.h"
#include "int.h"
#include "serial.h"
#include "lcd.h"

// #define DUMP1
#ifdef DUMP1
#include <stdio.h>
static FILE *debugf = NULL;

static void open_debug(void) {
	if(debugf == NULL) {
		debugf = fopen("/dev/ttys001", "w");
	}
}
static void dump1(const decNumber *a, const char *msg) {
	char buf[2000], *b = buf;

	open_debug();
	if(decNumberIsNaN(a)) b= "NaN";
	else if(decNumberIsInfinite(a)) b = decNumberIsNegative(a)?"-inf":"inf";
	else
		decNumberToString(a, b);
	fprintf(debugf, "%s: %s\n", msg ? msg : "???", b);
	fflush(debugf);
}
#else
#define dump1(a,b)
#endif


#define MOD_DIGITS	450		/* Big enough for 1e384 mod short integer */
#define BIGMOD_DIGITS	820		/* Big enough for maxreal mod minreal */


/* Some basic conditional tests */
int dn_lt0(const decNumber *x) {
	return decNumberIsNegative(x) && ! decNumberIsZero(x);
}

int dn_le0(const decNumber *x) {
	return decNumberIsNegative(x) || decNumberIsZero(x);
}

int dn_eq0(const decNumber *x) {
	return decNumberIsZero(x);
}

int dn_eq(const decNumber *x, const decNumber *y) {
	sincosNumber a;
	return decNumberIsZero(dn_compare(&a.n, x, y));
}

int dn_eq1(const decNumber *x) {
	return dn_eq(x, &const_1);
}

int dn_lt(const decNumber *x, const decNumber *y) {
	sincosNumber a;
	return dn_lt0(dn_compare(&a.n, x, y));
}

int dn_abs_lt(const decNumber *x, const decNumber *tol) {
	decNumber a;
	return dn_lt(dn_abs(&a, x), tol);
}

int absolute_error(const decNumber *x, const decNumber *y, const decNumber *tol) {
	decNumber a;
	return dn_abs_lt(dn_subtract(&a, x, y), tol);
}

const decNumber *convergence_threshold(void) {
	return is_dblmode() ? &const_1e_32 : &const_1e_24;
}


/* Some wrapper rountines to save space
 */
decNumber *dn_compare(decNumber *r, const decNumber *a, const decNumber *b) {
	return decNumberCompare(r, a, b, &Ctx);
}

decNumber *dn_min(decNumber *r, const decNumber *a, const decNumber *b) {
	return decNumberMin(r, a, b, &Ctx);
}

decNumber *dn_max(decNumber *r, const decNumber *a, const decNumber *b) {
	return decNumberMax(r, a, b, &Ctx);
}

decNumber *dn_abs(decNumber *r, const decNumber *a) {
	return decNumberAbs(r, a, &Ctx);
}

decNumber *dn_minus(decNumber *r, const decNumber *a) {
	return decNumberMinus(r, a, &Ctx);
}

decNumber *dn_plus(decNumber *r, const decNumber *a) {
	return decNumberPlus(r, a, &Ctx);
}

decNumber *dn_sqrt(decNumber *r, const decNumber *a) {
	return decNumberSquareRoot(r, a, &Ctx);
}

decNumber *dn_exp(decNumber *r, const decNumber *a) {
	return decNumberExp(r, a, &Ctx);
}


decNumber *dn_average(decNumber *r, const decNumber *a, const decNumber *b) {
	decNumber z;

	dn_add(&z, a, b);
	return dn_div2(r, &z);
}


#if 0
/* Define a table of short integers.
 * This should be equal or larger than any of the summation integers required in the
 * various series approximations to avoid needless computation.
 */
#define MAX_SMALL_INT	9
static const decNumber *const small_ints[MAX_SMALL_INT+1] = {
	&const_0, &const_1, &const_2, &const_3, &const_4,
	&const_5, &const_6, &const_7, &const_8, &const_9,
};

void ullint_to_dn(decNumber *x, unsigned long long int n) {
	/* Check to see if the number is small enough to be in our table */
	if(n <= MAX_SMALL_INT) {
		decNumberCopy(x, small_ints[n]);
	} else {
		/* Got to do this the long way */
		decNumber z;
		int shift = 0;

		decNumberZero(x);

		while(n != 0) {
			const int r = n % 10;
			n /= 10;
			if(r != 0) {
				dn_mulpow10(&z, small_ints[r], shift);
				dn_add(x, x, &z);
			}
			++shift;
		}
	}
}
#endif

void int_to_dn(decNumber *x, int n) {
	int sgn;

	/* Account for negatives */
	if(n < 0) {
		sgn = 1;
		n = -n;
	} else
		sgn = 0;

	ullint_to_dn(x, n);

	if(sgn)
		dn_minus(x, x);
}

int dn_to_int(const decNumber *x) {
	decNumber y;
#if 0
	char buf[64];

	decNumberRescale(&y, x, &const_0, &Ctx);
	decNumberToString(&y, buf);
	return s_to_i(buf);
#else
	return decGetInt(decNumberTrunc(&y, x));
#endif
}

unsigned long long int dn_to_ull(const decNumber *x, int *sgn) {
	decNumber y, z;
	char buf[64];

	decNumberTrunc(&z, x);
	decNumberRescale(&y, &z, &const_0, &Ctx);
	if(decNumberIsNegative(&z)) {
		dn_minus(&y, &y);
		*sgn = 1;
	} else
		*sgn = 0;
	decNumberToString(&y, buf);
	return s_to_ull(buf, 10);
}



decNumber *set_inf(decNumber *x) {
	return decNumberCopy(x, &const_inf);
}

decNumber *set_neginf(decNumber *x) {
	return decNumberCopy(x, &const__inf);
}


void decNumberPI(decNumber *pi) {
	decNumberCopy(pi, &const_PI);
}
void decNumberPIon2(decNumber *pion2) {
	decNumberCopy(pion2, &const_PIon2);
}

/* Utility routine that checks if the X register is even or odd or neither.
 * Returns positive if even, zero if odd, -1 for special, -2 for fractional.
 */
int is_even(const decNumber *x) {
	decNumber y, z;

	if(decNumberIsSpecial(x))
		return -1;
	dn_abs(&z, x);
	decNumberMod(&y, &z, &const_2);
	if(dn_eq0(&y))
		return 1;
	if(dn_eq1(&y))
		return 0;
	return -2;
}

decNumber *dn_1(decNumber *r) {
	return decNumberCopy(r, &const_1);
}

decNumber *dn__1(decNumber *r) {
	return decNumberCopy(r, &const__1);
}

decNumber *dn_p2(decNumber *r, const decNumber *x) {
	return dn_add(r, x, &const_2);
}

decNumber *dn_mul2(decNumber *r, const decNumber *x) {
	return dn_multiply(r, x, &const_2);
}

decNumber *dn_div2(decNumber *r, const decNumber *x) {
	return dn_multiply(r, x, &const_0_5);
}

decNumber *dn_mul100(decNumber *r, const decNumber *x) {
	return dn_mulpow10(r, x, 2);
}

decNumber *dn_mulPI(decNumber *r, const decNumber *x) {
	return dn_multiply(r, x, &const_PI);
}

decNumber *dn_mulpow10(decNumber *r, const decNumber *x, int p) {
	decNumberCopy(r, x);
	r->exponent += p;
	return r;
}

/* Mantissa of a number
 */
#ifdef INCLUDE_MANTISSA
decNumber *decNumberMantissa(decNumber *r, const decNumber *x) {
	if(decNumberIsSpecial(x))
		return set_NaN(r);
	if(dn_eq0(x))
		return decNumberCopy(r, x);
	decNumberCopy(r, x);
	r->exponent = 1 - r->digits;
	return r;
}

/* Exponenet of a number
 */
decNumber *decNumberExponent(decNumber *r, const decNumber *x) {
	if(decNumberIsSpecial(x))
		return set_NaN(r);
	if(dn_eq0(x))
		return decNumberZero(r);
	int_to_dn(r, x->exponent + x->digits - 1);
	return r;
}

/* 1 ULP
 */
static int realULP(decNumber *r, const decNumber *x) {
	int dblmode;
	int subnormal = 0;
	int expshift;
	int minexp;

	if(decNumberIsSpecial(x)) {
		if(decNumberIsInfinite(x))
			set_inf(r);
		else
			set_NaN(r);
		return 0;
	}

	dblmode = is_dblmode();

	if(dblmode) {
		expshift = DECIMAL128_Pmax;
		minexp = DECIMAL128_Emin - DECIMAL128_Pmax + 1;
		if(x->exponent < DECIMAL128_Emin)
			subnormal = 1;
	} else {
		expshift = DECIMAL64_Pmax;
		minexp = DECIMAL64_Emin - DECIMAL64_Pmax + 1;
		if(x->exponent < DECIMAL64_Emin)
			subnormal = 1;
	}

	dn_1(r);
	if(dn_eq0(x) || subnormal)
		r->exponent = minexp;
	else
		r->exponent = x->exponent + x->digits - expshift;
	return subnormal;
}

decNumber *decNumberULP(decNumber *r, const decNumber *x) {
	realULP(r, x);
	return r;
}

decNumber *decNumberNeighbour(decNumber *r, const decNumber *y, const decNumber *x) {
	decNumber ulp;
	int down, subnormal;

	if(decNumberIsNaN(y))
		return set_NaN(r);
	if(decNumberIsSpecial(x))
		return decNumberCopy(r, x);
	dn_compare(&ulp, y, x);
	if(dn_eq0(&ulp))
		return decNumberCopy(r, y);

	down = decNumberIsNegative(&ulp) ? 1 : 0;
	subnormal = realULP(&ulp, x);

	if(! subnormal && x->digits == 1 && x->lsu[0] == 1)
		ulp.exponent -= (! decNumberIsNegative(x)) == down;
	if(down)
		dn_subtract(r, x, &ulp);
	else
		dn_add(r, x, &ulp);
	return r;
}
#endif

/* Multiply Add: x + y * z
 */
#ifdef INCLUDE_MULADD
decNumber *decNumberMAdd(decNumber *r, const decNumber *z, const decNumber *y, const decNumber *x) {
	decNumber t;

	dn_multiply(&t, x, y);
	return dn_add(r, &t, z);
}
#endif


/* Reciprocal of a number.
 * Division is correctly rounded so we just use that instead of coding
 * something special (that could be more efficient).
 */
decNumber *decNumberRecip(decNumber *r, const decNumber *x) {
	return dn_divide(r, &const_1, x);
}

/* Reciprocal of a function's result.
 * This routine calls the specified function and then multiplicatively
 * inverts its result.
 */
#if 0
static decNumber *dn_recip(decNumber *r, const decNumber *x,
		decNumber *(*func)(decNumber *r, const decNumber *x)) {
	decNumber z;

	(*func)(&z, x);
	return decNumberRecip(r, &z);
}
#endif

/* A plethora of round to integer functions to support the large variety
 * of possibilities in this area.  Start with a common utility function
 * that saves the current rounding mode, rounds as required and restores
 * the rounding mode properly.
 */
static decNumber *round2int(decNumber *r, const decNumber *x, enum rounding mode) {
	enum rounding a = Ctx.round;

	Ctx.round = mode;
	decNumberToIntegralValue(r, x, &Ctx);
	Ctx.round = a;
	return r;
}

/* Floor - truncate to minus infinity.
 */
decNumber *decNumberFloor(decNumber *r, const decNumber *x) {
	return round2int(r, x, DEC_ROUND_FLOOR);
}

/* Ceiling - truncate to plus infinity.
 */
decNumber *decNumberCeil(decNumber *r, const decNumber *x) {
	return round2int(r, x, DEC_ROUND_CEILING);
}

/* Trunc - truncate to zero.
 */
decNumber *decNumberTrunc(decNumber *r, const decNumber *x) {
	return round2int(r, x, DEC_ROUND_DOWN);
}

/* Round - round 0.5 up.
 */
decNumber *decNumberRound(decNumber *r, const decNumber *x) {
	return round2int(r, x, DEC_ROUND_HALF_UP);
}

/* Intg - round 0.5 even.
 */
static decNumber *decNumberIntg(decNumber *r, const decNumber *x) {
	return round2int(r, x, DEC_ROUND_HALF_EVEN);
}

/* Frac - round 0.5 up.
 */
decNumber *decNumberFrac(decNumber *r, const decNumber *x) {
	decNumber y;

	round2int(&y, x, DEC_ROUND_DOWN);
	return dn_subtract(r, x, &y);
}


static void dn_gcd(decNumber *r, const decNumber *x, const decNumber *y, int bigmod) {
	decNumber b, t;

	decNumberCopy(&b, y);
	decNumberCopy(r, x);
	while(! dn_eq0(&b)) {
		decNumberCopy(&t, &b);
		if(bigmod)
			decNumberBigMod(&b, r, &t);
		else
			decNumberMod(&b, r, &t);
		decNumberCopy(r, &t);
	}
}

static int dn_check_gcd(decNumber *r, const decNumber *x, const decNumber *y,
		decNumber *a, decNumber *b) {
	if(decNumberIsSpecial(x) || decNumberIsSpecial(y)) {
		if(decNumberIsNaN(x) || decNumberIsNaN(y))
			set_NaN(r);
		else
			set_inf(r);
	} else if(!is_int(x) || !is_int(y))
		set_NaN(r);
	else {
		dn_abs(a, x);
		dn_abs(b, y);
		return 0;
	}
	return 1;
}

decNumber *decNumberGCD(decNumber *r, const decNumber *x, const decNumber *y) {
	decNumber a, b;

	if(dn_check_gcd(r, x, y, &a, &b))
		return r;

	if(dn_eq0(x))
		decNumberCopy(r, &b);
	else if(dn_eq0(y))
		decNumberCopy(r, &a);
	else
		dn_gcd(r, &a, &b, 1);
	return r;
}

decNumber *decNumberLCM(decNumber *r, const decNumber *x, const decNumber *y) {
	decNumber gcd, a, b, t;

	if(dn_check_gcd(r, x, y, &a, &b))
		return r;

	if(dn_eq0(x) || dn_eq0(y))
		decNumberCopy(r, x);
	dn_gcd(&gcd, &a, &b, 1);
	dn_divide(&t, &a, &gcd);
	return dn_multiply(r, &t, &b);
}


/* The extra logarithm and power functions */

/* Raise y^x */
decNumber *dn_power_internal(decNumber *r, const decNumber *y, const decNumber *x, const decNumber *logy) {
	decNumber s, t, my;
	int isxint, xodd, ynegative;
	int negate = 0;

	if(dn_eq1(y))
		return dn_1(r);				// 1^x = 1

	if(dn_eq0(x))
		return dn_1(r);				// y^0 = 1

	if(decNumberIsNaN(x) || decNumberIsNaN(y))
		return set_NaN(r);

	if(dn_eq1(x))
		return decNumberCopy(r, y); 		// y^1 = y

	if(decNumberIsInfinite(x)) {
		int ylt1;
		if(dn_eq(y, &const__1))
			return dn_1(r);			// -1 ^ +/-inf = 1

		ylt1 = dn_abs_lt(y, &const_1);
		if(decNumberIsNegative(x)) {
			if(ylt1)
				return set_inf(r);		// y^-inf |y|<1 = +inf
			return decNumberZero(r);		// y^-inf |y|>1 = +0
		}
		if(ylt1)
			return decNumberZero(r);		// y^inf |y|<1 = +0
		return set_inf(r);				// y^inf |y|>1 = +inf
	}

	isxint = is_int(x);
	ynegative = decNumberIsNegative(y);
	if(decNumberIsInfinite(y)) {
		if(ynegative) {
			xodd = isxint && is_even(x) == 0;
			if(decNumberIsNegative(x)) {
				decNumberZero(r);		// -inf^x x<0 = +0
				if(xodd)			// -inf^x odd x<0 = -0
					return decNumberCopy(r, &const__0);
				return r;
			}
			if(xodd)
				return set_neginf(r);		// -inf^x odd x>0 = -inf
			return set_inf(r);			// -inf^x x>0 = +inf
		}
		if(decNumberIsNegative(x))
			return decNumberZero(r);		// +inf^x x<0 = +0
		return set_inf(r);				// +inf^x x>0 = +inf
	}

	if(dn_eq0(y)) {
		xodd = isxint && is_even(x) == 0;
		if(decNumberIsNegative(x)) {
			if(xodd && ynegative)
				return set_neginf(r);		// -0^x odd x<0 = -inf
			return set_inf(r);			// 0^x x<0 = +inf
		}
		if(xodd && ynegative)
			return decNumberCopy(r, &const__0);	// -0^x odd x>0 = -/+0
		return decNumberZero(r);			// 0^x x>0 = +0
	}

	if(ynegative) {
		if(!isxint)
			return set_NaN(r);			// y^x y<0, x not odd int = NaN
		if(is_even(x) == 0)				// y^x, y<0, x odd = - ((-y)^x)
			negate = 1;
		dn_minus(&my, y);
		y = &my;
	}
	if(logy == NULL) {
		dn_ln(&t, y);
		logy = &t;
	}
	dn_multiply(&s, logy, x);
	dn_exp(r, &s);
	if(negate)
		return dn_minus(r, r);
	return r;
}

decNumber *dn_power(decNumber *r, const decNumber *y, const decNumber *x) {
	return dn_power_internal(r, y, x, NULL);
}


/* ln(1+x) */
decNumber *decNumberLn1p(decNumber *r, const decNumber *x) {
	decNumber u, v, w;

	if(decNumberIsSpecial(x) || dn_eq0(x)) {
		return decNumberCopy(r, x);
	}
	dn_p1(&u, x);
	dn_m1(&v, &u);
	if(dn_eq0(&v)) {
		return decNumberCopy(r, x);
	}
	dn_divide(&w, x, &v);
	dn_ln(&v, &u);
	return dn_multiply(r, &v, &w);
}

/* exp(x)-1 */
decNumber *decNumberExpm1(decNumber *r, const decNumber *x) {
	decNumber u, v, w;

	if(decNumberIsSpecial(x)) {
		return decNumberCopy(r, x);
	}
	dn_exp(&u, x);
	dn_m1(&v, &u);
	if(dn_eq0(&v)) {
		return decNumberCopy(r, x);
	}
	if(dn_eq(&v, &const__1)) {
		return dn__1(r);
	}
	dn_multiply(&w, &v, x);
	dn_ln(&v, &u);
	return dn_divide(r, &w, &v);
}


decNumber *decNumberPow2(decNumber *r, const decNumber *x) {
	return dn_power_internal(r, &const_2, x, &const_ln2);
}

decNumber *decNumberPow10(decNumber *r, const decNumber *x) {
	return dn_power_internal(r, &const_10, x, &const_ln10);
}

decNumber *decNumberPow_1(decNumber *r, const decNumber *x) {
	int even = is_even(x);
	decNumber t, u;

	if(even == 1)
		return dn_1(r);
	if(even == 0)
		return dn__1(r);
	decNumberMod(&u, x, &const_2);
	dn_mulPI(&t, &u);
	sincosTaylor(&t, NULL, r);
	return r;
}

/* Square - this almost certainly could be done more efficiently
 */
decNumber *decNumberSquare(decNumber *r, const decNumber *x) {
	return dn_multiply(r, x, x);
}

/* Cube - again could be done more efficiently */
decNumber *decNumberCube(decNumber *r, const decNumber *x) {
	decNumber z;

	decNumberSquare(&z, x);
	return dn_multiply(r, &z, x);
}
Fait un makefile bordel de merde
/* Cube root */
decNumber *decNumberCubeRoot(decNumber *r, const decNumber *x) {
	decNumber third, t;

	decNumberRecip(&third, &const_3);

	if(decNumberIsNegative(x)) {
		dn_minus(r, x);
		dn_power(&t, r, &third);
		return dn_minus(r, &t);
	}
	return dn_power(r, x, &third);
}

#ifdef INCLUDE_XROOT
decNumber *decNumberXRoot(decNumber *r, const decNumber *a, const decNumber *b) {
	decNumber s, t;

	decNumberRecip(&s, b);

	if(decNumberIsNegative(a)) {
		if(is_even(b) == 0) {
			dn_minus(r, a);
			dn_power(&t, r, &s);
			return dn_minus(r, &t);
		}
		return set_NaN(r);
	}
	return dn_power(r, a, &s);
}
#endif



decNumber *decNumberBigMod(decNumber *res, const decNumber *x, const decNumber *y) {
	/* Declare a structure large enough to hold a really long number.
	 * This structure is likely to be larger than is required.
	 */
	struct {
		decNumber n;
		decNumberUnit extra[((BIGMOD_DIGITS-DECNUMDIGITS+DECDPUN-1)/DECDPUN)];
	} out;

	int digits = Ctx.digits;

	Ctx.digits = BIGMOD_DIGITS;
	decNumberRemainder(&out.n, x, y, &Ctx);
	Ctx.digits = digits;

#ifdef INCLUDE_MOD41
	decNumberPlus(res, &out.n, &Ctx);
	if(XeqOpCode == (OP_DYA | OP_MOD41) && decNumberIsNegative(x) != decNumberIsNegative(y) && !dn_eq0(res))
		dn_add(res, res, y);
	return res;
#else
	return 	decNumberPlus(res, &out.n, &Ctx);
#endif
}


/*
 *  Common helper for angle conversions
 *  Checks the present mode and converts accordingly
 */
static decNumber *decNumberDRG_internal(decNumber *res, const decNumber *x, s_opcode op) {
#define DRG(op,mode) (((op) << 2) | (mode))

	switch (DRG(argKIND(op), get_trig_mode())) {

	case DRG(OP_2DEG,  TRIG_RAD):
	case DRG(OP_RAD2,  TRIG_DEG):
		return decNumberR2D(res, x);

	case DRG(OP_2DEG,  TRIG_GRAD):
	case DRG(OP_GRAD2, TRIG_DEG):
		return decNumberG2D(res, x);

	case DRG(OP_2RAD,  TRIG_DEG):
	case DRG(OP_DEG2,  TRIG_RAD):
		return decNumberD2R(res, x);

	case DRG(OP_2RAD,  TRIG_GRAD):
	case DRG(OP_GRAD2, TRIG_RAD):
		return decNumberG2R(res, x);

	case DRG(OP_2GRAD, TRIG_DEG):
	case DRG(OP_DEG2,  TRIG_GRAD):
		return decNumberD2G(res, x);

	case DRG(OP_2GRAD, TRIG_RAD):
	case DRG(OP_RAD2,  TRIG_GRAD):
		return decNumberR2G(res, x);

	default:
		return decNumberCopy(res, x);
	}
#undef DRG
}

static void cvt_rad2(decNumber *res, const decNumber *x) {
	decNumberDRG_internal(res, x, OP_RAD2);
}

/* Calculate sin and cos of the given number in radians.
 * We need to do some range reduction to guarantee that our Taylor series
 * converges rapidly.
 */
void dn_sincos(const decNumber *v, decNumber *sinv, decNumber *cosv)
{
	decNumber x;

	if(decNumberIsSpecial(v))
		cmplx_NaN(sinv, cosv);
	else {
		decNumberMod(&x, v, &const_2PI);
		sincosTaylor(&x, sinv, cosv);
	}
}



/* Have to be careful here to ensure that every function we call can handle
 * the increased size of the numbers we're using.
 */
decNumber *decNumberTan(decNumber *res, const decNumber *x) {
	sincosNumber x2, s, c;

	if(decNumberIsSpecial(x))
		return set_NaN(res);
	else {
		const int digits = Ctx.digits;
		Ctx.digits = SINCOS_DIGITS;
		cvt_2rad_sincos(&s.n, &c.n, x);
		if(decNumberIsZero(&c.n))
			return set_NaN(res);
		dn_divide(&x2.n, &s.n, &c.n);
		Ctx.digits = digits;
		return dn_plus(res, &x2.n);
	}
}

#if 0
decNumber *decNumberSec(decNumber *res, const decNumber *x) {
	return dn_recip(res, x, &decNumberCos);
}

decNumber *decNumberCosec(decNumber *res, const decNumber *x) {
	return dn_recip(res, x, &decNumberSin);
}

decNumber *decNumberCot(decNumber *res, const decNumber *x) {
	return dn_recip(res, x, &decNumberTan);
}
#endif

decNumber *decNumberSinc(decNumber *res, const decNumber *x) {
	decNumber s;

	decNumberSquare(&s, x);
	if(dn_eq1(dn_p1(&s, &s)))
		return dn_1(res);
	decNumberMod(res, x, &const_2PI);
	sincosTaylor(res, &s, NULL);
	return dn_divide(res, &s, x);
}




decNumber *decNumberArcTan2(decNumber *res, const decNumber *a, const decNumber *b) {
	decNumber z;

	do_atan2(&z, a, b);
	cvt_rad2(res, &z);
	return res;
}

void op_r2p(enum nilop op) {
	decNumber x, y, rx, ry;

	getXY(&x, &y);
	cmplxToPolar(&rx, &ry, &x, &y);
	cvt_rad2(&y, &ry);
	setlastX();
	setXY(&rx, &y);
#ifdef RP_PREFIX
	RectPolConv = 1;
#endif
}

void op_p2r(enum nilop op) {
	decNumber x, y, t, range, angle;

	getXY(&range, &angle);
	decNumberCos(&t, &angle);
	dn_multiply(&x, &t, &range);
	decNumberSin(&t, &angle);
	dn_multiply(&y, &t, &range);
	setlastX();
	setXY(&x, &y);
#ifdef RP_PREFIX
	RectPolConv = 2;
#endif
}


/* Hyperbolic functions.
 * We start with a utility routine that calculates sinh and cosh.
 * We do the sihn as (e^x - 1) (e^x + 1) / (2 e^x) for numerical stability
 * reasons if the value of x is smallish.
 */
void dn_sinhcosh(const decNumber *x, decNumber *sinhv, decNumber *coshv) {
	decNumber t, u, v;

	if(sinhv != NULL) {
		if(dn_abs_lt(x, &const_0_5)) {
			decNumberExpm1(&u, x);
			dn_div2(&t, &u);
			dn_inc(&u);
			dn_divide(&v, &t, &u);
			dn_inc(&u);
			dn_multiply(sinhv, &u, &v);
		} else {
			dn_exp(&u, x);			// u = e^x
			decNumberRecip(&v, &u);		// v = e^-x
			dn_subtract(&t, &u, &v);	// r = e^x - e^-x
			dn_div2(sinhv, &t);
		}
	}
	if(coshv != NULL) {
		dn_exp(&u, x);			// u = e^x
		decNumberRecip(&v, &u);		// v = e^-x
		dn_average(coshv, &v, &u);	// r = (e^x + e^-x)/2
	}
}

decNumber *decNumberSinh(decNumber *res, const decNumber *x) {
	if(decNumberIsSpecial(x)) {
		if(decNumberIsNaN(x))
			return set_NaN(res);
		return decNumberCopy(res, x);
	}
	dn_sinhcosh(x, res, NULL);
	return res;
}

decNumber *decNumberCosh(decNumber *res, const decNumber *x) {
	if(decNumberIsSpecial(x)) {
		if(decNumberIsNaN(x))
			return set_NaN(res);
		return set_inf(res);
	}
	dn_sinhcosh(x, NULL, res);
	return res;
}

decNumber *decNumberTanh(decNumber *res, const decNumber *x) {
	decNumber a, b;

	if(decNumberIsNaN(x))
		return set_NaN(res);
	if(!dn_abs_lt(x, &const_100)) {
		if(decNumberIsNegative(x))
			return dn__1(res);
		return dn_1(res);
	}
	dn_add(&a, x, x);
	decNumberExpm1(&b, &a);
	dn_p2(&a, &b);
	return dn_divide(res, &b, &a);
}


#if 0
decNumber *decNumberSech(decNumber *res, const decNumber *x) {
	return dn_recip(res, x, &decNumberCosh);
}

decNumber *decNumberCosech(decNumber *res, const decNumber *x) {
	return dn_recip(res, x, &decNumberSinh);
}

decNumber *decNumberCoth(decNumber *res, const decNumber *x) {
	return dn_recip(res, x, &decNumberTanh);
}
#endif

decNumber *decNumberArcSinh(decNumber *res, const decNumber *x) {
	decNumber y, z;

	decNumberSquare(&y, x);		// y = x^2
	dn_p1(&z, &y);			// z = x^2 + 1
	dn_sqrt(&y, &z);		// y = sqrt(x^2+1)
	dn_inc(&y);			// y = sqrt(x^2+1)+1
	dn_divide(&z, x, &y);
	dn_inc(&z);
	dn_multiply(&y, x, &z);
	return decNumberLn1p(res, &y);
}


decNumber *decNumberArcCosh(decNumber *res, const decNumber *x) {
	decNumber z;

	decNumberSquare(res, x);	// r = x^2
	dn_m1(&z, res);			// z = x^2 + 1
	dn_sqrt(res, &z);		// r = sqrt(x^2+1)
	dn_add(&z, res, x);		// z = x + sqrt(x^2+1)
	return dn_ln(res, &z);
}

decNumber *decNumberArcTanh(decNumber *res, const decNumber *x) {
	decNumber y, z;

	if(decNumberIsNaN(x))
		return set_NaN(res);
	dn_abs(&y, x);
	if(dn_eq1(&y)) {
		if(decNumberIsNegative(x))
			return set_neginf(res);
		return set_inf(res);
	}
	// Not the obvious formula but more stable...
	dn_1m(&z, x);
	dn_divide(&y, x, &z);
	dn_mul2(&z, &y);
	decNumberLn1p(&y, &z);
	return dn_div2(res, &y);
}


decNumber *decNumberD2R(decNumber *res, const decNumber *x) {
	return dn_multiply(res, x, &const_PIon180);
}

decNumber *decNumberR2D(decNumber *res, const decNumber *x) {
	return dn_divide(res, x, &const_PIon180);
}


decNumber *decNumberG2R(decNumber *res, const decNumber *x) {
	return dn_multiply(res, x, &const_PIon200);
}

decNumber *decNumberR2G(decNumber *res, const decNumber *x) {
	return dn_divide(res, x, &const_PIon200);
}

decNumber *decNumberG2D(decNumber *res, const decNumber *x) {
	return dn_multiply(res, x, &const_0_9);
}

decNumber *decNumberD2G(decNumber *res, const decNumber *x) {
	return dn_divide(res, x, &const_0_9);
}

decNumber *decNumberDRG(decNumber *res, const decNumber *x) {
	return decNumberDRG_internal(res, x, XeqOpCode);
}

/* Check the arguments a little and perform the computation of
 * ln(permutation) which is common across both our callers.
 *
 * This is the real version.
 */
enum perm_opts { PERM_INVALID=0, PERM_INTG, PERM_NORMAL };
static enum perm_opts perm_helper(decNumber *r, const decNumber *x, const decNumber *y) {
	decNumber n, s;

	if(decNumberIsSpecial(x) || decNumberIsSpecial(y) || dn_lt0(x) || dn_lt0(y)) {
		if(decNumberIsInfinite(x) && !decNumberIsInfinite(y))
			set_inf(r);
		else
			set_NaN(r);
		return PERM_INVALID;
	}
	dn_p1(&n, x);				// x+1
	decNumberLnGamma(&s, &n);		// lnGamma(x+1) = Ln x!

	dn_subtract(r, &n, y);	// x-y+1
	if(dn_le0(r)) {
		set_NaN(r);
		return PERM_INVALID;
	}
	decNumberLnGamma(&n, r);		// LnGamma(x-y+1) = Ln (x-y)!
	dn_subtract(r, &s, &n);

	if(is_int(x) && is_int(y))
		return PERM_INTG;
	return PERM_NORMAL;
}


/* Calculate permutations:
 * C(x, y) = P(x, y) / y! = x! / ( (x-y)! y! )
 */
decNumber *decNumberComb(decNumber *res, const decNumber *x, const decNumber *y) {
	decNumber r, n, s;
	const enum perm_opts code = perm_helper(&r, x, y);

	if(code != PERM_INVALID) {
		dn_p1(&n, y);				// y+1
		decNumberLnGamma(&s, &n);		// LnGamma(y+1) = Ln y!
		dn_subtract(&n, &r, &s);

		dn_exp(res, &n);
		if(code == PERM_INTG)
			decNumberIntg(res, res);
	} else
		decNumberCopy(res, &r);
	return res;
}

/* Calculate permutations:
 * P(x, y) = x! / (x-y)!
 */
decNumber *decNumberPerm(decNumber *res, const decNumber *x, const decNumber *y) {
	decNumber t;
	const enum perm_opts code = perm_helper(&t, x, y);

	if(code != PERM_INVALID) {
		dn_exp(res, &t);
		if(code == PERM_INTG)
			decNumberIntg(res, res);
	} else
		decNumberCopy(res, &t);
	return res;
}

#ifdef _DEBUG_
#include <stdio.h>
char dump[DECNUMDIGITS + 10];
#define DUMP(d, s) ((int) decNumberToString(d, dump), fprintf(f, s "=%s\n", dump))

#endif

const decNumber *const gamma_consts[21] = {
	&const_gammaC01, &const_gammaC02, &const_gammaC03,
	&const_gammaC04, &const_gammaC05, &const_gammaC06,
	&const_gammaC07, &const_gammaC08, &const_gammaC09,
	&const_gammaC10, &const_gammaC11, &const_gammaC12,
	&const_gammaC13, &const_gammaC14, &const_gammaC15,
	&const_gammaC16, &const_gammaC17, &const_gammaC18,
	&const_gammaC19, &const_gammaC20, &const_gammaC21,
};

// lnBeta(x, y) = lngamma(x) + lngamma(y) - lngamma(x+y)
decNumber *decNumberLnBeta(decNumber *res, const decNumber *x, const decNumber *y) {
	decNumber s, t, u;

	decNumberLnGamma(&s, x);
	busy();
	decNumberLnGamma(&t, y);
	busy();
	dn_add(&u, &s, &t);
	dn_add(&s, x, y);
	decNumberLnGamma(&t, &s);
	dn_subtract(res, &u, &t);
	return res;
}

decNumber *decNumberHMS2HR(decNumber *res, const decNumber *xin) {
	decNumber m, s, t, x;
	const int neg = decNumberIsNegative(xin);

	// decode hhhh.mmss...
	dn_abs(&x, xin);
	decNumberFrac(&t, &x);			// t = .mmss
	dn_mul100(&s, &t);			// s = mm.ss
	decNumberTrunc(&m, &s);			// m = mm
	decNumberFrac(&t, &s);			// t = .ss
	dn_multiply(&s, &t, &const_1on60);	// s = ss.sss / 60
	dn_mulpow10(&s, &s, 2);
	dn_add(&t, &m, &s);			// s = mm + ss.sss / 60
	dn_multiply(&m, &t, &const_1on60);
	decNumberTrunc(&s, &x);			// s = hh
	dn_add(res, &m, &s);
	if(neg) dn_minus(res, res);
	return res;
}

decNumber *decNumberHR2HMS(decNumber *res, const decNumber *xin) {
	decNumber m, s, t, x;
	const int neg = decNumberIsNegative(xin);

	dn_abs(&x, xin);
	decNumberFrac(&t, &x);			// t = .mmssss
	dn_multiply(&s, &t, &const_60);		// s = mm.ssss
	decNumberTrunc(&m, &s);			// m = mm
	decNumberFrac(&t, &s);			// t = .ssss
	dn_multiply(&s, &t, &const_0_6);	// scale down by 60/100
	dn_add(&t, &s, &m);			// t = mm.ss
	dn_mulpow10(&m, &t, -2);		// t = .mmss
	decNumberTrunc(&s, &x);			// s = hh
	dn_add(&t, &m, &s);			// t = hh.mmss = result

	// Round to the appropriate number of digits for the result
	decNumberRoundDigits(&t, &t, is_dblmode() ? 34 : 16, DEC_ROUND_HALF_EVEN);

	// Now fix any rounding/carry issues
	dn_mulpow10(&s, &t, 2);			// hhmm.ssss
	decNumberFrac(&m, &s);			// .ssss
	if(dn_ge(&m, &const_0_6))
		dn_add(&s, &s, &const_0_4);
	dn_mulpow10(res, &s, -2);		// hh.mmssss
	decNumberFrac(&m, res);
	if(dn_ge(&m, &const_0_6))
		dn_add(res, res, &const_0_4);
	if(neg) dn_minus(res, res);
	return res;
}

decNumber *decNumberHMSAdd(decNumber *res, const decNumber *x, const decNumber *y) {
	decNumber a, b, c;

	decNumberHMS2HR(&a, x);
	decNumberHMS2HR(&b, y);
	dn_add(&c, &a, &b);
	decNumberHR2HMS(res, &c);
	return res;
}

decNumber *decNumberHMSSub(decNumber *res, const decNumber *x, const decNumber *y) {
	decNumber a, b, c;

	decNumberHMS2HR(&a, x);
	decNumberHMS2HR(&b, y);
	dn_subtract(&c, &a, &b);
	decNumberHR2HMS(res, &c);
	return res;
}

/* Logical operations on decNumbers.
 * We treat 0 as false and non-zero as true.
 */
static int dn2bool(const decNumber *x) {
	return dn_eq0(x)?0:1;
}

static decNumber *bool2dn(decNumber *res, int l) {
	if(l)
		dn_1(res);
	else
		decNumberZero(res);
	return res;
}

decNumber *decNumberNot(decNumber *res, const decNumber *x) {
	return bool2dn(res, !dn2bool(x));
}

/*
 *  Execute the logical operations via a truth table.
 *  Each nibble encodes a single operation.
 */
decNumber *decNumberBooleanOp(decNumber *res, const decNumber *x, const decNumber *y) {
	const unsigned int TRUTH_TABLE = 0x9176E8;
	const unsigned int bit = ((argKIND(XeqOpCode) - OP_LAND) << 2) + (dn2bool(y) << 1) + dn2bool(x);
	return bool2dn(res, (TRUTH_TABLE >> bit) & 1);
}


/*
 * Round a number to a given number of digits and with a given mode
 */
decNumber *decNumberRoundDigits(decNumber *res, const decNumber *x, const int digits, const enum rounding round) {
	enum rounding default_round = Ctx.round;
	int default_digits = Ctx.digits;

	Ctx.round = round;
	Ctx.digits = digits;
	decNumberPlus(res, x, &Ctx);
	Ctx.digits = default_digits;
	Ctx.round = default_round;
	return res;
}


/*
 *  Round to display accuracy
 */
decNumber *decNumberRnd(decNumber *res, const decNumber *x) {
	decNumber t, u;
#if defined(INCLUDE_SIGFIG_MODE)
	int numdig;
	enum display_modes dmode;
#else
	int numdig = UState.dispdigs + 1;
	enum display_modes dmode = (enum display_modes) UState.dispmode;
#endif

	if(decNumberIsSpecial(x))
		return decNumberCopy(res, x);

	if(UState.fract) {
		decNumber2Fraction(&t, &u, x);
		return dn_divide(res, &t, &u);
	}

#if defined(INCLUDE_SIGFIG_MODE)
	dmode = get_dispmode_digs(&numdig);
	numdig++;
	if(dmode == MODE_STD) {
		// to fit new definition of std_round_fix in display.c
		dmode = std_round_fix(x, &numdig, dmode, numdig-1);
		numdig = DISPLAY_DIGITS;
 	}
#else
	if(dmode == MODE_STD) {
		dmode = std_round_fix(x);
		numdig = DISPLAY_DIGITS;
	}
#endif

	if(dmode == MODE_FIX)
		/* FIX is different since the number of digits changes */
		return decNumberRoundDecimals(res, x, numdig-1, DEC_ROUND_HALF_UP);

	return decNumberRoundDigits(res, x, numdig, DEC_ROUND_HALF_UP);
}

decNumber *decNumberRoundDecimals(decNumber *r, const decNumber *x, const int n, const enum rounding round) {
	decNumber t;
#if 0
	/* The slow but always correct way */
	decNumber u, p10;

	int_to_dn(&u, n);
	decNumberPow10(&p10, &u);
	dn_multiply(&t, x, &p10);
	round2int(&u, &t, round);
	return dn_divide(r, &u, &p10);
#else
	/* The much faster way but relying on base 10 numbers with exponents */
	if(decNumberIsSpecial(x))
		return decNumberCopy(r, x);

	decNumberCopy(&t, x);
	t.exponent += n;
	round2int(r, &t, round);
	r->exponent -= n;
	return r;
#endif
}

static decNumber *gser(decNumber *res, const decNumber *a, const decNumber *x, const decNumber *gln) {
	decNumber ap, del, sum, t, u;
	int i;

	if(dn_le0(x))
		return decNumberZero(res);
	decNumberCopy(&ap, a);
	decNumberRecip(&sum, a);
	decNumberCopy(&del, &sum);
	for(i=0; i<1000; i++) {
		dn_inc(&ap);
		dn_divide(&t, x, &ap);
		dn_multiply(&del, &del, &t);
		dn_add(&t, &sum, &del);
		if(dn_eq(&t, &sum))
			break;
		decNumberCopy(&sum, &t);
	}
	dn_ln(&t, x);
	dn_multiply(&u, &t, a);
	dn_subtract(&t, &u, x);
	dn_subtract(&u, &t, gln);
	dn_exp(&t, &u);
	return dn_multiply(res, &sum, &t);
}

static void gcheckSmall(decNumber *v)
{
	const decNumber * const threshold = &const_1e_10000;

	if(dn_abs_lt(v, threshold))
		decNumberCopy(v, threshold);
}

static decNumber *gcf(decNumber *res, const decNumber *a, const decNumber *x, const decNumber *gln) {
	decNumber an, b, c, d, h, t, u, v, i;
	int n;

	dn_p1(&t, x);
	gcheckSmall(dn_subtract(&b, &t, a));		// b = (x+1) - a
	set_inf(&c);
	decNumberRecip(&d, &b);
	decNumberCopy(&h, &d);
	decNumberZero(&i);
	for(n=0; n<1000; n++) {
		dn_inc(&i);
		dn_subtract(&t, a, &i);		// t = a-i
		dn_multiply(&an, &i, &t);		// an = -i (i-a)
		dn_p2(&b, &b);
		dn_multiply(&t, &an, &d);
		dn_add(&v, &t, &b);
		gcheckSmall(&v);
		decNumberRecip(&d, &v);
		dn_divide(&t, &an, &c);
		dn_add(&c, &b, &t);
		gcheckSmall(&c);
		dn_multiply(&t, &d, &c);
		dn_multiply(&u, &h, &t);
		if(dn_eq(&h, &u))
			break;
		decNumberCopy(&h, &u);
	}
	dn_ln(&t, x);
	dn_multiply(&u, &t, a);
	dn_subtract(&t, &u, x);
	dn_subtract(&u, &t, gln);
	dn_exp(&t, &u);
	return dn_multiply(res, &t, &h);
}

decNumber *decNumberGammap(decNumber *res, const decNumber *x, const decNumber *a) {
	decNumber z, lga;
	const int op = XeqOpCode - (OP_DYA | OP_GAMMAg);
	const int regularised = op & 2;
	const int upper = op & 1;

	if(decNumberIsNegative(x) || dn_le0(a) ||
			decNumberIsNaN(x) || decNumberIsNaN(a) || decNumberIsInfinite(a)) {
		return set_NaN(res);
	}
	if(decNumberIsInfinite(x)) {
		if(upper) {
			if(regularised)
				return dn_1(res);
			return decNumberGamma(res, a);
		}
		return decNumberZero(res);
	}

	dn_p1(&lga, a);
	dn_compare(&z, x, &lga);
	if(regularised)
		decNumberLnGamma(&lga, a);
	else
		decNumberZero(&lga);
	if(decNumberIsNegative(&z)) {
		/* Deal with a difficult case by using the other expansion */
		if(dn_gt(a, &const_9000) && dn_gt(x, dn_multiply(&z, a, &const_0_995)))
			goto use_cf;
		gser(res, a, x, &lga);
		if(upper)
			goto invert;
	} else {
use_cf:		gcf(res, a, x, &lga);
		if(! upper)
			goto invert;
	}
	return res;

invert:	if(regularised)
		return dn_1m(res, res);
	decNumberGamma(&z, a);
	return dn_subtract(res, &z, res);
}

#ifdef INCLUDE_FACTOR
decNumber *decFactor(decNumber *r, const decNumber *x) {
	int sgn;
	unsigned long long int i;

	if(decNumberIsSpecial(x) || ! is_int(x))
		return set_NaN(r);
	i = dn_to_ull(x, &sgn);
	ullint_to_dn(r, doFactor(i));
	if(sgn)
		dn_minus(r, r);
	return r;
}
#endif

#ifdef INCLUDE_USER_IO
decNumber *decRecv(decNumber *r, const decNumber *x) {
	int to;

	if(decNumberIsSpecial(x) || decNumberIsNegative(x)) {
		to = -1;
		if(decNumberIsInfinite(x) && ! decNumberIsNegative(x))
			to = 0x7fffffff;
	} else
		to = dn_to_int(x);
	int_to_dn(r, recv_byte(to));
	return r;
}
#endif
#endif
