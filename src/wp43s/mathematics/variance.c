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
 * \file variance.c
 ***********************************************/

#include "wp43s.h"

/* Standard deviations and standard errors.
 *
 * The computation involves subtraction of large numbers that quite possible
 * are close to each other.  Because of this, the start of the computation is
 * performed using a FMA for the difference.
 */
static void do_stddev(const real_t *sumXX, const real_t *sumX, const real_t *numberX, int sample, int rootn, int exp, int regIndex) {
  real_t tempReal1, tempReal2, tempReal3;
  const real_t *p = numberX;
  realContext_t *realContext = &ctxtReal75; // Summation data with 75 digits

  realDivide(sumX, numberX, &tempReal2, realContext);
  realChangeSign(&tempReal2);
  realFMA(sumX, &tempReal2, sumXX, &tempReal1, realContext);
  if(sample) {
    realSubtract(numberX, const_1, &tempReal3, realContext);
    p = &tempReal3;
  }
  realDivide(&tempReal1, p, &tempReal2, realContext);
  if(realIsNegative(&tempReal2) || realIsZero(&tempReal2)) {
    realZero(&tempReal1);
  }
  else {
    realSquareRoot(&tempReal2, &tempReal1, realContext);
  }

  p = &tempReal1;
  if(rootn) {
    realSquareRoot(numberX, &tempReal2, realContext);
    realDivide(&tempReal1, &tempReal2, &tempReal3, realContext);
    p = &tempReal3;
  }
  if(exp) {
    realExp(p, &tempReal2, realContext);
    p = &tempReal2;
  }
  realToReal34(p, REGISTER_REAL34_DATA(regIndex));
}

static void calculateStandardDeviation(const real_t *sumX2, const real_t *sumX,
                                       const real_t *number,
                                       const real_t *sumY2, const real_t *sumY,
                                       int sample, int rootn, int exp,
                                       int displayInfo) {
  if(checkMinimumDataPoints(const_2)) {
    liftStack();
    setSystemFlag(FLAG_ASLIFT);
    liftStack();

    do_stddev(sumX2, sumX, number, sample, rootn, exp, REGISTER_X);
    do_stddev(sumY2, sumY, number, sample, rootn, exp, REGISTER_Y);

    temporaryInformation = displayInfo;
  }
}

/* Standard deviation (sample and population) and standard error for arithmetic mean */
void fnSampleStdDev(uint16_t unusedButMandatoryParameter) {
  calculateStandardDeviation(SIGMA_X2, SIGMA_X, SIGMA_N, SIGMA_Y2, SIGMA_Y, 1, 0, 0, TI_SAMPLSTDDEV);
}

void fnPopulationStdDev(uint16_t unusedButMandatoryParameter) {
  calculateStandardDeviation(SIGMA_X2, SIGMA_X, SIGMA_N, SIGMA_Y2, SIGMA_Y, 0, 0, 0, TI_POPLSTDDEV);
}

void fnStandardError(uint16_t unusedButMandatoryParameter) {
  calculateStandardDeviation(SIGMA_X2, SIGMA_X, SIGMA_N, SIGMA_Y2, SIGMA_Y, 1, 1, 0, TI_STDERR);
}

/* Standard deviation (sample and population) and standard error for geometric mean */
void fnGeometricSampleStdDev(uint16_t unusedButMandatoryParameter) {
  calculateStandardDeviation(SIGMA_ln2X, SIGMA_lnX, SIGMA_N, SIGMA_ln2Y, SIGMA_lnY, 1, 0, 1, TI_GEOMSAMPLSTDDEV);
}

void fnGeometricPopulationStdDev(uint16_t unusedButMandatoryParameter) {
  calculateStandardDeviation(SIGMA_ln2X, SIGMA_lnX, SIGMA_N, SIGMA_ln2Y, SIGMA_lnY, 0, 0, 1, TI_GEOMPOPLSTDDEV);
}

void fnGeometricStandardError(uint16_t unusedButMandatoryParameter) {
  calculateStandardDeviation(SIGMA_ln2X, SIGMA_lnX, SIGMA_N, SIGMA_ln2Y, SIGMA_lnY, 1, 1, 1, TI_GEOMSTDERR);
}

// Weighted standard deviation, standard error
static void calculateWeightedStandardDeviation(int sample, int rootn, int exp, int display) {
  if(checkMinimumDataPoints(const_2)) {
    liftStack();
    setSystemFlag(FLAG_ASLIFT);
    do_stddev(SIGMA_X2Y, SIGMA_XY, SIGMA_Y, sample, rootn, exp, REGISTER_X);

    temporaryInformation = display;
  }
}

void fnWeightedSampleStdDev(uint16_t unusedButMandatoryParameter) {
  calculateWeightedStandardDeviation(1, 0, 0, TI_WEIGHTEDSAMPLSTDDEV);
}

void fnWeightedPopulationStdDev(uint16_t unusedButMandatoryParameter) {
  calculateWeightedStandardDeviation(0, 0, 0, TI_WEIGHTEDPOPLSTDDEV);
}

void fnWeightedStandardError(uint16_t unusedButMandatoryParameter) {
  calculateWeightedStandardDeviation(1, 1, 0, TI_WEIGHTEDSTDERR);
}


void fnPopulationCovariance(uint16_t unusedButMandatoryParameter){    //COVxy
  realContext_t *realContext = &ctxtReal75; // Summation data with 75 digits
  real_t SS,TT;
  if(checkMinimumDataPoints(const_2)) {
    liftStack();
    setSystemFlag(FLAG_ASLIFT);

    realMultiply(SIGMA_N,SIGMA_XY, &SS, realContext);
    realMultiply(SIGMA_X,SIGMA_Y, &TT, realContext);
    realSubtract(&SS,&TT,&SS,realContext);
    realDivide(&SS,SIGMA_N,&SS,realContext);

    realDivide(&SS,SIGMA_N,&SS,realContext);
    realToReal34(&SS, REGISTER_REAL34_DATA(REGISTER_X));

    temporaryInformation = TI_COV;
  }
}
void fnSampleCovariance(uint16_t unusedButMandatoryParameter){    //sxy
  realContext_t *realContext = &ctxtReal75; // Summation data with 75 digits
  real_t SS,TT;
  if(checkMinimumDataPoints(const_2)) {
    liftStack();
    setSystemFlag(FLAG_ASLIFT);

    realMultiply(SIGMA_N,SIGMA_XY, &SS, realContext);
    realMultiply(SIGMA_X,SIGMA_Y, &TT, realContext);
    realSubtract(&SS,&TT,&SS,realContext);
    realDivide(&SS,SIGMA_N,&SS,realContext);

    realSubtract(SIGMA_N,const_1,&TT,realContext);
    realDivide(&SS,&TT,&SS,realContext);
    realToReal34(&SS, REGISTER_REAL34_DATA(REGISTER_X));

    temporaryInformation = TI_SXY;
  }
}
void fnCoefficientDetermination(uint16_t unusedButMandatoryParameter){  //r
  realContext_t *realContext = &ctxtReal75; // Summation data with 75 digits
  real_t SS,TT,UU;
  if(checkMinimumDataPoints(const_2)) {

    realMultiply(SIGMA_N,SIGMA_XY, &SS, realContext);    //do sxy
    realMultiply(SIGMA_X,SIGMA_Y, &TT, realContext);
    realSubtract(&SS,&TT,&SS,realContext);
    realDivide(&SS,SIGMA_N,&SS,realContext);

    realSubtract(SIGMA_N,const_1,&TT,realContext);
    realDivide(&SS,&TT,&SS,realContext);

    realMultiply(SIGMA_N,SIGMA_X2, &TT, realContext);    //do sx
    realMultiply(SIGMA_X,SIGMA_X, &UU, realContext);
    realSubtract(&TT,&UU,&TT,realContext);
    realDivide(&TT,SIGMA_N,&TT,realContext);
    realSubtract(SIGMA_N,const_1,&UU,realContext);
    realDivide(&TT,&UU,&TT,realContext);
    realSquareRoot(&TT,&TT,realContext);                 //this is sx

    realDivide(&SS,&TT,&SS,realContext);                 //this is sxy/sx

    realMultiply(SIGMA_N,SIGMA_Y2, &TT, realContext);    //do sy
    realMultiply(SIGMA_Y,SIGMA_Y, &UU, realContext);
    realSubtract(&TT,&UU,&TT,realContext);
    realDivide(&TT,SIGMA_N,&TT,realContext);
    realSubtract(SIGMA_N,const_1,&UU,realContext);
    realDivide(&TT,&UU,&TT,realContext);
    realSquareRoot(&TT,&TT,realContext);                 //this is sy

    realDivide(&SS,&TT,&SS,realContext);                 //this is sxy/sx/sy

    liftStack();
    setSystemFlag(FLAG_ASLIFT);
    realToReal34(&SS, REGISTER_REAL34_DATA(REGISTER_X));

    temporaryInformation = TI_CORR;
  }
}

void fnMinExpStdDev(uint16_t unusedButMandatoryParameter){ //smi
  realContext_t *realContext = &ctxtReal75; // Summation data with 75 digits
  real_t RR,SS,TT,UU,SX2,SY2;
  if(checkMinimumDataPoints(const_2)) {

    realMultiply(SIGMA_N,SIGMA_X2, &TT, realContext);    //do sx
    realMultiply(SIGMA_X,SIGMA_X, &UU, realContext);
    realSubtract(&TT,&UU,&TT,realContext);
    realDivide(&TT,SIGMA_N,&TT,realContext);

    realSubtract(SIGMA_N,const_1,&UU,realContext);
    realDivide(&TT,&UU,&SS,realContext);
    realMultiply(&SS,&SS,&SX2,realContext);               //   --> sx^2

    realMultiply(SIGMA_N,SIGMA_Y2, &TT, realContext);     //do sy
    realMultiply(SIGMA_Y,SIGMA_Y, &UU, realContext);
    realSubtract(&TT,&UU,&TT,realContext);
    realDivide(&TT,SIGMA_N,&TT,realContext);

    realSubtract(SIGMA_N,const_1,&UU,realContext);
    realDivide(&TT,&UU,&TT,realContext);
    realMultiply(&TT,&TT,&SY2,realContext);               //  -->sy^2

    fnCoefficientDetermination(0);
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X),&RR);   //r
    realMultiply(&RR,&RR,&RR,realContext);                //r^2

    realSubtract(const_1,&RR,&SS,realContext);
    realMultiply(&RR,&SY2,&TT,realContext);
    realSubtract(&SX2,&TT,&TT,realContext);
    realDivide(&SS,&TT,&UU,realContext);

    realMultiply(&UU,&SX2,&UU,realContext);
    realMultiply(&UU,&SY2,&UU,realContext);              //smi2
    realSquareRoot(&UU, &UU, &ctxtReal39);

    realToReal34(&UU, REGISTER_REAL34_DATA(REGISTER_X)); //Place in X, which was pushed with r

    temporaryInformation = TI_SMI;
  }
}

