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
 * \file curveFitting.c
 ***********************************************/

#include "curveFitting.h"

#include "constantPointers.h"
#include "debug.h"
#include "error.h"
#include "flags.h"
#include "items.h"
#include "longIntegerType.h"
#include "mathematics/comparisonReals.h"
#include "mathematics/variance.h"
#include "mathematics/xthRoot.h"
#include "mathematics/wp34s.h"
#include "plotstat.h"
#include "registers.h"
#include "registerValueConversions.h"
#include "stack.h"
#include "stats.h"
#include <math.h>

#include "wp43s.h"


//#define STAT_DISPLAY_ABCDEFG                      //to display helper functions A-H


static real_t RR, RR2, RRMAX, SMI, aa0, aa1, aa2;  // Curve fitting variables, available to the different functions
realContext_t *realContext;
realContext_t *realContextForecast;

#if defined STAT_DISPLAY_ABCDEFG
  double A, B, C, D, E, F, G, H;
#endif
#if defined STATDEBUG && defined PC_BUILD
  char ss[200];
#endif

/********************************************//**
 * \brief Sets the curve fitting mode
 *
 * \param[in] curveFitting uint16_t Curve fitting mode
 *
 *
 * \return void
 * \Input from defines, is "1" to exclude a method, examples:
 * \LinF=CF_LINEAR_FITTING = 1. This "1" excludes LinF
 * \448 = 1 1100 0000, Excluding 3 param models.
 * \0   = 0 0000 0000, Exlcluding nothing
 * \510 = 1 1111 1110, Excludes everything except LINF (default)
 * \511 not allowed from keyboard, as it is internally used to allow ORTHOF.
 *
 * \The internal representation reverses the logic, i.e. ones represent allowed methods
 ***********************************************/


void fnCurveFittingReset(uint16_t curveFitting) {     // JM vv
  lrSelection = CF_LINEAR_FITTING;
  lrChosen = 0;                               // lrChosen    is used to indicate if there was a L.R. selection. Can be only one bit.
}



void fnCurveFitting(uint16_t curveFitting) {
  curveFitting = curveFitting & 0x01FF;
  temporaryInformation = TI_STATISTIC_LR;

  if(curveFitting < 0x01FF) {                 // note curveFitting >= 0
    curveFitting = (~curveFitting) & 0x01FF;  // see above
  }
  else if (curveFitting == 0x01FF) {
    curveFitting = 0x0200;                    // see above
  }
  else {
    curveFitting = 0;                         // illegal value, therefore defaulting to none
  }

   printf(">>>%u  %u\n",curveFitting,lrCountOnes(curveFitting));
   if(lrCountOnes(curveFitting) == 1) {         //Added experimental, toggle bits of the lrselection word
     lrSelection = lrSelection ^ curveFitting;  //Added  "
   } else                                       //Added  "

     lrSelection = curveFitting;                    // lrSelection is used to store the BestF method, in inverse, i.e. 1 indicating allowed method
  
  lrChosen = 0;                               // lrChosen    is used to indicate if there was a L.R. selection. Can be only one bit.

  #ifdef PC_BUILD
    uint16_t numberOfOnes;
    numberOfOnes = lrCountOnes(curveFitting);

    if(numberOfOnes == 1) {
      printf("Use the ");
    }
    else {
      printf("Use the best fitting model out of\n");
    }

    printf("%s",getCurveFitModeNames(curveFitting));
    if(numberOfOnes == 1) {
      printf(" fitting model.\n");
    }
    else {
      printf("\nfitting models.\n");
    }
  #endif // PC_BUILD
}



/********************************************//**
 * \brief Sets X to the set L.R.
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnCurveFittingLR(uint16_t unusedButMandatoryParameter) {
  longInteger_t lr;
  liftStack();
  longIntegerInit(lr);
  uIntToLongInteger((~lrSelection) & 0x01FF, lr);           // Input mask 01 1111 1111 EXCLUDES 10 0000 0000, which is ORTHOF, as it is not in the OM
  convertLongIntegerToLongIntegerRegister(lr, REGISTER_X);
  longIntegerFree(lr);
}


uint16_t lrCountOnes(uint16_t curveFitting) { // count the number of allowed methods
    uint16_t numberOfOnes;
    numberOfOnes = curveFitting - ((curveFitting >> 1) & 0x5555);
    numberOfOnes = (numberOfOnes & 0x3333) + ((numberOfOnes >> 2) & 0x3333);
    numberOfOnes = (numberOfOnes + (numberOfOnes >> 4)) & 0x0f0f;
    numberOfOnes = (uint16_t)(numberOfOnes * 0x0101) >> 8;
    return numberOfOnes; 
}


uint16_t minLRDataPoints(uint16_t selection){
  if (selection > 1023)     return 65535; else
    if(selection & 448)       return 3;     else
      if(selection & (63+512))  return 2;     else
        return 65535; //if 0

//  switch(selection) {
//    case CF_LINEAR_FITTING      /*   1 */ :
//    case CF_EXPONENTIAL_FITTING /*   2 */ :
//    case CF_LOGARITHMIC_FITTING /*   4 */ :
//    case CF_POWER_FITTING       /*   8 */ :
//    case CF_ROOT_FITTING        /*  16 */ :
//    case CF_HYPERBOLIC_FITTING  /*  32 */ : return 2; break;
//    case CF_PARABOLIC_FITTING   /*  64 */ :
//    case CF_CAUCHY_FITTING      /* 128 */ :
//    case CF_GAUSS_FITTING       /* 256 */ : return 3; break;
//    case CF_ORTHOGONAL_FITTING  /* 512 */ : return 2; break;                      //ORTHOF ASSESS (2 points minimum)
//    default : return 0xFFFF; break;
//  }

}


/********************************************//**
 * \brief Finds the best curve fit
 *
 * \param[in] curveFitting uint16_t Curve fitting mode. Binary positions indicate which curves to be considered.
 * \Do not convert from 0 to 511 here. Conversion only done after input.
 *  
 * \default of 0 is defined in ReM to be the same as 511
 *
 * \return void
 ***********************************************/
void fnProcessLRfind(uint16_t curveFitting){
  int32_t nn;
  real_t NN;

  realToInt32(SIGMA_N, nn);  
  realCopy(const_0,&aa0);
  realCopy(const_0,&aa1);
  realCopy(const_0,&aa2);
  #ifdef PC_BUILD
	  printf("Processing for best fit: %s\n",getCurveFitModeNames(curveFitting));
	#endif //PC_BUILD
  realCopy(const__4,&RRMAX);
  uint16_t s = 0;       //default
  uint16_t ix,jx;                  //only a single graph can be evaluated at once, so retain the single lowest bit, and clear the higher order bits.
  jx = 0;
  for(ix=0; ix<10; ix++) {         //up to 2^9 inclusive of 512 which is ORTHOF. The ReM is respectedby usage of 0 only, not by manual selection.
    jx = curveFitting & ((1 << ix));
    if(jx) {
      #ifdef PC_BUILD
        printf("processCurvefitSelection curveFitting:%u sweep:%u %s\n",curveFitting,jx,getCurveFitModeNames(jx));
      #endif
      
      if(nn >= (int32_t)minLRDataPoints(jx)) {
        processCurvefitSelection(jx,&RR,&SMI, &aa0, &aa1, &aa2);
        realMultiply(&RR,&RR,&RR2,&ctxtReal39);

        if(realCompareGreaterThan(&RR2, &RRMAX) && realCompareLessEqual(&RR2, const_1)) { //Only consider L.R. models where R^2<=1
          realCopy(&RR2,&RRMAX);
        	s = jx;
        }
      }
    }
  }
  if(lrCountOnes(s) > 1) s = 0; //error condition, cannot have >1 solutions, do not do L.R.

  #ifdef PC_BUILD
	  if(s != 0) printf("Found best fit: %u %s\n",s,getCurveFitModeNames(s)); else
      printf("Found no fit: %u\n",s);
	#endif //PC_BUILD

  if(nn >= (int32_t)minLRDataPoints(s)) {
    processCurvefitSelection(s,&RR,&SMI, &aa0, &aa1, &aa2);
    lrChosen = s;

    temporaryInformation = TI_LR;
    if(s == CF_CAUCHY_FITTING || s == CF_GAUSS_FITTING || s == CF_PARABOLIC_FITTING) {
  	  liftStack();
  	  setSystemFlag(FLAG_ASLIFT);
  	  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
  	  realToReal34(&aa2, REGISTER_REAL34_DATA(REGISTER_X));
  	}
    liftStack();
    setSystemFlag(FLAG_ASLIFT);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    realToReal34(&aa1, REGISTER_REAL34_DATA(REGISTER_X));
    liftStack();
    setSystemFlag(FLAG_ASLIFT);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    realToReal34(&aa0, REGISTER_REAL34_DATA(REGISTER_X));
  } else {
    if(minLRDataPoints(s) == 65535) {
      displayCalcErrorMessage(ERROR_TOO_FEW_DATA, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        moreInfoOnError("In function fnProcessLRfind:", "There is insufficient statistical data to do L.R., possibly due to data manipulation!", NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    } else {
      uInt32ToReal((uint32_t)minLRDataPoints(s),&NN);
      checkMinimumDataPoints(&NN);              //Report an error
    }
  }
}



//0010 0011 1111    //0x23F     (internal is inverted: '1' means enabled)
//0001 1100 0000    //0x1C0
void fnProcessLR (uint16_t unusedButMandatoryParameter){
  if(checkMinimumDataPoints(const_2)) {
    fnProcessLRfind(lrSelection);
  }
}



void calc_BCD(real_t *BB, real_t *CC, real_t *DD){                        //Aux terms, calc_BCD must be run before calc_AEFG
realContext = &ctxtReal75;
real_t SS,TT;
  //        B = nn * sumx2y - sumx2 * sumy;
  realMultiply(SIGMA_N, SIGMA_X2Y, &SS, realContext);
  realMultiply(SIGMA_X2, SIGMA_Y, &TT, realContext);
  realSubtract(&SS, &TT, BB, realContext);

  //        C = nn * sumx3 - sumx2 * sumx;
  realMultiply(SIGMA_N, SIGMA_X3, &SS, realContext);
  realMultiply(SIGMA_X2, SIGMA_X, &TT, realContext);
  realSubtract(&SS, &TT, CC, realContext);

  //        D = nn * sumxy - sumx * sumy;
  realMultiply(SIGMA_N, SIGMA_XY, &SS, realContext);
  realMultiply(SIGMA_X, SIGMA_Y, &TT, realContext);
  realSubtract(&SS, &TT, DD, realContext);
}



void calc_AEFG(real_t *AA, real_t *BB, real_t *CC, real_t *DD, real_t *EE, real_t *FF, real_t *GG){                        //Aux terms, calc_AEFG must be run after calc_BCD
realContext = &ctxtReal75;
real_t SS,TT,UU;
  //        A = nn * sumx2 - sumx * sumx;
  realMultiply(SIGMA_N, SIGMA_X2, &SS, realContext);
  realMultiply(SIGMA_X, SIGMA_X, &TT, realContext);
  realSubtract(&SS, &TT, AA, realContext);

  //        E = nn * sumx4 - sumx2 * sumx2;
  realMultiply(SIGMA_N, SIGMA_X4, &SS, realContext);
  realMultiply(SIGMA_X2, SIGMA_X2, &TT, realContext);
  realSubtract(&SS, &TT, EE, realContext);

  //USING COMPONENTS OF BCD
  //        F = (A*B - C*D) / (A*E - C*C);    //interchangably the a2 in PARABF
  realMultiply(AA, BB, &SS, realContext);
  realMultiply(CC, DD, &TT, realContext);
  realSubtract(&SS, &TT, &UU, realContext);
  realMultiply(AA, EE, &SS, realContext);
  realMultiply(CC, CC, &TT, realContext);
  realSubtract(&SS, &TT, &SS, realContext);
  realDivide  (&UU,&SS,FF,realContext);

  //        G = (D - F * C) / A;
  realMultiply(FF, CC, &SS, realContext);
  realSubtract(DD, &SS, &SS, realContext);
  realDivide  (&SS,AA,GG,realContext);
}



/********************************************//**
 * \brief Calculates the curve fitting parameters r, smi, a0, a1, a2
 *
 * \param[in] curveFitting uint16_t Curve fitting mode, and pointers to the real variables
 * \return void
 *
 ***********************************************/
void processCurvefitSelection(uint16_t selection, real_t *RR_, real_t *SMI_, real_t *aa0, real_t *aa1, real_t *aa2){
    real_t AA,BB,CC,DD,EE,FF,GG,HH,RR2;   // Curve aux fitting variables
    real_t SS,TT,UU;                  // Temporary curve fitting variables
    uint16_t ix,jx;               //only a single graph can be displayed at once, so retain the single lowest bit, and clear the higher order bits if ever control comes here with multpile graph selections
    jx = 0;
    for(ix=0; ix!=10; ix++) {     //up to 2^9 inclusive of 512 which is ORTHOF. The ReM is respectedby usage of 0 only, not by manual selection.
      jx = selection & ((1 << ix));
      if(jx) break;
    }
    selection = jx;
    #if (defined STATDEBUG || defined STAT_DISPLAY_ABCDEFG) && defined PC_BUILD
      printf("processCurvefitSelection selection:%u, reduced selection to:%u\n",selection,jx);
    #endif

    realContext = &ctxtReal75;    //Use 75 as the sums can reach high values and the accuracy of the regressionn depends on this. Could arguably be optimized.
    real_t VV,WW;
    real_t S_X,S_Y,S_XY,M_X,M_Y;
    realDivide(SIGMA_X,SIGMA_N,&M_X,realContext); //&ctxtReal39);     //vv
    realDivide(SIGMA_Y,SIGMA_N,&M_Y,realContext); //&ctxtReal39);
    fnStatR   (RR_, &S_XY, &S_X, &S_Y);
    fnStatSMI (SMI_);

    #if defined STAT_DISPLAY_ABCDEFG && defined PC_BUILD
      double v;
    #endif

    switch(selection) {
      case CF_LINEAR_FITTING :
        //      a0 = (sumx2 * sumy  - sumx * sumxy) / (nn * sumx2 - sumx * sumx);
        realMultiply(SIGMA_X2,SIGMA_Y,&SS,realContext);
        realMultiply(SIGMA_X,SIGMA_XY,&TT,realContext);
        realSubtract(&SS,&TT,&SS,realContext);
        realMultiply(SIGMA_N,SIGMA_X2,&TT,realContext);
        realMultiply(SIGMA_X,SIGMA_X,&UU,realContext);
        realSubtract(&TT,&UU,&TT,realContext);
        realDivide(&SS,&TT,aa0,realContext);

        //      a1 = (nn  * sumxy - sumx * sumy ) / (nn * sumx2 - sumx * sumx);
        realMultiply(SIGMA_N,SIGMA_XY,&SS,realContext);
        realMultiply(SIGMA_X,SIGMA_Y,&TT,realContext);
        realSubtract(&SS,&TT,&SS,realContext);
        realMultiply(SIGMA_N,SIGMA_X2,&TT,realContext);
        realMultiply(SIGMA_X,SIGMA_X,&UU,realContext);
        realSubtract(&TT,&UU,&TT,realContext);
        realDivide(&SS,&TT,aa1,realContext);

        //       r  = (nn * sumxy - sumx * sumy) / (sqrt (nn * sumx2 - sumx * sumx) * sqrt(nn * sumy2 - sumy * sumy) );
        realMultiply(SIGMA_N,SIGMA_XY,&SS,realContext);
        realMultiply(SIGMA_X,SIGMA_Y,&TT,realContext);
        realSubtract(&SS,&TT,&SS,realContext);          //SS is top

        realMultiply(SIGMA_N,SIGMA_X2,&TT,realContext);
        realMultiply(SIGMA_X,SIGMA_X,&UU,realContext);
        realSubtract(&TT,&UU,&TT,realContext);
        realSquareRoot(&TT,&TT,realContext);            //TT is bottom, factor 1

        realMultiply(SIGMA_N,SIGMA_Y2,&UU,realContext);
        realMultiply(SIGMA_Y,SIGMA_Y,&VV,realContext);
        realSubtract(&UU,&VV,&UU,realContext);
        realSquareRoot(&UU,&UU,realContext);            //UU is bottom factor 2

        realDivide(&SS,&TT,RR_,realContext);
        realDivide(RR_,&UU,RR_,realContext);            //r

        #if defined STATDEBUG && defined PC_BUILD
          printf("##### Linear\n");
          formatRealDebug(ss, aa1); printf("§§ A1: %s\n",ss);
          formatRealDebug(ss, aa0); printf("§§ A0: %s\n",ss);
          formatRealDebug(ss, RR_); printf("§§ r: %s\n",ss);
        #endif
        break;


      case CF_EXPONENTIAL_FITTING :        
        //      a0 = exp( (sumx2 * sumlny  - sumx * sumxlny) / (nn * sumx2 - sumx * sumx) );
        realMultiply(SIGMA_X2,SIGMA_lnY,&SS,realContext);
        realMultiply(SIGMA_X,SIGMA_XlnY,&TT,realContext);
        realSubtract(&SS,&TT,&SS,realContext);
        realMultiply(SIGMA_N,SIGMA_X2,&TT,realContext);
        realMultiply(SIGMA_X,SIGMA_X,&UU,realContext);
        realSubtract(&TT,&UU,&TT,realContext);
        realDivide(&SS,&TT,aa0,realContext);
        realExp(aa0,aa0,realContext);

        //a1 = (nn  * sumxlny - sumx * sumlny ) / (nn * sumx2 - sumx * sumx);
        realMultiply(SIGMA_N,SIGMA_XlnY,&SS,realContext);
        realMultiply(SIGMA_X,SIGMA_lnY,&TT,realContext);
        realSubtract(&SS,&TT,&SS,realContext);
        realMultiply(SIGMA_N,SIGMA_X2,&TT,realContext);
        realMultiply(SIGMA_X,SIGMA_X,&UU,realContext);
        realSubtract(&TT,&UU,&TT,realContext);
        realDivide(&SS,&TT,aa1,realContext);

        //      r = (nn * sumxlny - sumx*sumlny) / (sqrt(nn*sumx2-sumx*sumx) * sqrt(nn*sumln2y-sumlny*sumlny)); //(rEXP)
        realMultiply(SIGMA_N,SIGMA_XlnY,&SS,realContext);
        realMultiply(SIGMA_X,SIGMA_lnY,&TT,realContext);
        realSubtract(&SS,&TT,&SS,realContext);          //SS is top

        realMultiply(SIGMA_N,SIGMA_X2,&TT,realContext);
        realMultiply(SIGMA_X,SIGMA_X,&UU,realContext);
        realSubtract(&TT,&UU,&TT,realContext);
        realSquareRoot(&TT,&TT,realContext);            //TT is bottom, factor 1

        realMultiply(SIGMA_N,SIGMA_ln2Y,&UU,realContext);
        realMultiply(SIGMA_lnY,SIGMA_lnY,&VV,realContext);
        realSubtract(&UU,&VV,&UU,realContext);
        realSquareRoot(&UU,&UU,realContext);            //UU is bottom factor 2

        realDivide(&SS,&TT,RR_,realContext);
        realDivide(RR_,&UU,RR_,realContext);            //r

        #if defined STATDEBUG && defined PC_BUILD
          printf("##### EXPF\n");
          formatRealDebug(ss, aa1); printf("§§ A1: %s\n",ss);
          formatRealDebug(ss, aa0); printf("§§ A0: %s\n",ss);
          formatRealDebug(ss, RR_); printf("§§ r: %s\n",ss);
        #endif
        break;


      case CF_LOGARITHMIC_FITTING :
        //      a0 = (sumln2x * sumy  - sumlnx * sumylnx) / (nn * sumln2x - sumlnx * sumlnx);
        realMultiply(SIGMA_ln2X,SIGMA_Y,&SS,realContext);
        realMultiply(SIGMA_lnX,SIGMA_YlnX,&TT,realContext);
        realSubtract(&SS,&TT,&SS,realContext);
        realMultiply(SIGMA_N,SIGMA_ln2X,&TT,realContext);
        realMultiply(SIGMA_lnX,SIGMA_lnX,&UU,realContext);
        realSubtract(&TT,&UU,&TT,realContext);
        realDivide(&SS,&TT,aa0,realContext);

        //a1 = (nn  * sumylnx - sumlnx * sumy ) / (nn * sumln2x - sumlnx * sumlnx);;
        realMultiply(SIGMA_N,SIGMA_YlnX,&SS,realContext);
        realMultiply(SIGMA_lnX,SIGMA_Y,&TT,realContext);
        realSubtract(&SS,&TT,&SS,realContext);
        realMultiply(SIGMA_N,SIGMA_ln2X,&TT,realContext);
        realMultiply(SIGMA_lnX,SIGMA_lnX,&UU,realContext);
        realSubtract(&TT,&UU,&TT,realContext);
        realDivide(&SS,&TT,aa1,realContext);

        //      r = (nn * sumylnx - sumlnx*sumy) / (sqrt(nn*sumln2x-sumlnx*sumlnx) * sqrt(nn*sumy2-sumy*sumy)); //(rLOG)
        realMultiply(SIGMA_N,SIGMA_YlnX,&SS,realContext);
        realMultiply(SIGMA_lnX,SIGMA_Y,&TT,realContext);
        realSubtract(&SS,&TT,&SS,realContext);          //SS is top

        realMultiply(SIGMA_N,SIGMA_ln2X,&TT,realContext);
        realMultiply(SIGMA_lnX,SIGMA_lnX,&UU,realContext);
        realSubtract(&TT,&UU,&TT,realContext);
        realSquareRoot(&TT,&TT,realContext);            //TT is bottom, factor 1

        realMultiply(SIGMA_N,SIGMA_Y2,&UU,realContext);
        realMultiply(SIGMA_Y,SIGMA_Y,&VV,realContext);
        realSubtract(&UU,&VV,&UU,realContext);
        realSquareRoot(&UU,&UU,realContext);            //UU is bottom factor 2

        realDivide(&SS,&TT,RR_,realContext);
        realDivide(RR_,&UU,RR_,realContext);            //r

        #if defined STATDEBUG && defined PC_BUILD
          printf("##### LOGF\n");
          formatRealDebug(ss, aa1); printf("§§ A1: %s\n",ss);
          formatRealDebug(ss, aa0); printf("§§ A0: %s\n",ss);
          formatRealDebug(ss, RR_); printf("§§ r: %s\n",ss);
        #endif
        break;


      case CF_POWER_FITTING :
        //      a0 = exp( (sumln2x * sumlny  - sumlnx * sumlnxlny) / (nn * sumln2x - sumlnx * sumlnx)  );
        realMultiply(SIGMA_ln2X,SIGMA_lnY,&SS,realContext);
        realMultiply(SIGMA_lnX,SIGMA_lnXlnY,&TT,realContext);
        realSubtract(&SS,&TT,&SS,realContext);
        realMultiply(SIGMA_N,SIGMA_ln2X,&TT,realContext);
        realMultiply(SIGMA_lnX,SIGMA_lnX,&UU,realContext);
        realSubtract(&TT,&UU,&TT,realContext);
        realDivide(&SS,&TT,aa0,realContext);
        realExp(aa0,aa0,realContext);

        //a1 = (nn  * sumlnxlny - sumlnx * sumlny ) / (nn * sumln2x - sumlnx * sumlnx);
        realMultiply(SIGMA_N,SIGMA_lnXlnY,&SS,realContext);
        realMultiply(SIGMA_lnX,SIGMA_lnY,&TT,realContext);
        realSubtract(&SS,&TT,&SS,realContext);
        realMultiply(SIGMA_N,SIGMA_ln2X,&TT,realContext);
        realMultiply(SIGMA_lnX,SIGMA_lnX,&UU,realContext);
        realSubtract(&TT,&UU,&TT,realContext);
        realDivide(&SS,&TT,aa1,realContext);

        //      r = (nn * sumlnxlny - sumlnx*sumlny) / (sqrt(nn*sumln2x-sumlnx*sumlnx) * sqrt(nn*sumln2y-sumlny*sumlny)); //(rEXP)
        realMultiply(SIGMA_N,SIGMA_lnXlnY,&SS,realContext);
        realMultiply(SIGMA_lnX,SIGMA_lnY,&TT,realContext);
        realSubtract(&SS,&TT,&SS,realContext);          //SS is top

        realMultiply(SIGMA_N,SIGMA_ln2X,&TT,realContext);
        realMultiply(SIGMA_lnX,SIGMA_lnX,&UU,realContext);
        realSubtract(&TT,&UU,&TT,realContext);
        realSquareRoot(&TT,&TT,realContext);            //TT is bottom, factor 1

        realMultiply(SIGMA_N,SIGMA_ln2Y,&UU,realContext);
        realMultiply(SIGMA_lnY,SIGMA_lnY,&VV,realContext);
        realSubtract(&UU,&VV,&UU,realContext);
        realSquareRoot(&UU,&UU,realContext);            //UU is bottom factor 2

        realDivide(&SS,&TT,RR_,realContext);
        realDivide(RR_,&UU,RR_,realContext);            //r

        #if defined STATDEBUG && defined PC_BUILD
          printf("##### POWERF\n");
          formatRealDebug(ss, aa1); printf("§§ A1: %s\n",ss);
          formatRealDebug(ss, aa0); printf("§§ A0: %s\n",ss);
          formatRealDebug(ss, RR_); printf("§§ r: %s\n",ss);
        #endif
        break;


      case CF_ROOT_FITTING :
        //      A = nn * sum1onx2 - sum1onx * sum1onx;
        //      B = 1.0/A * (sum1onx2 * sumlny - sum1onx * sumlnyonx);
        //      C = 1.0/A * (nn * sumlnyonx - sum1onx * sumlny);
        realMultiply(SIGMA_N,SIGMA_1onX2,&SS,realContext);
        realMultiply(SIGMA_1onX,SIGMA_1onX,&AA,realContext);
        realSubtract(&SS,&AA,&AA,realContext);                 //err fixed

        realDivide(const_1,&AA,&SS,realContext);               //err fixed
        realMultiply(SIGMA_1onX2,SIGMA_lnY,&TT,realContext);
        realMultiply(SIGMA_1onX,SIGMA_lnYonX,&UU,realContext);
        realSubtract(&TT,&UU,&TT,realContext);
        realMultiply(&SS,&TT,&BB,realContext);

        realMultiply(SIGMA_N,SIGMA_lnYonX,&TT,realContext);
        realMultiply(SIGMA_1onX,SIGMA_lnY,&UU,realContext);
        realSubtract(&TT,&UU,&TT,realContext);
        realMultiply(&SS,&TT,&CC,realContext);                 //err due to SS above

        //      a0 = exp (B);
        realExp(&BB,aa0,realContext);

        //      a1 = exp (C);
        realExp(&CC,aa1,realContext);

        //     r = sqrt ( (B * sumlny + C * sumlnyonx - 1.0/nn * sumlny * sumlny) / (sumln2y - 1.0/nn * sumlny * sumlny) ); //(rROOT)
        realMultiply(&BB,SIGMA_lnY,&SS,realContext);
        realMultiply(&CC,SIGMA_lnYonX,&TT,realContext);
        realAdd     (&SS,&TT,&SS,realContext); //t1+t2
        realDivide  (const_1,SIGMA_N,&TT,realContext);
        realMultiply(&TT,SIGMA_lnY,&TT,realContext);
        realMultiply(&TT,SIGMA_lnY,&TT,realContext); //t3
        realSubtract(&SS,&TT,&SS,realContext); //t1+t2-t3

        realSubtract(SIGMA_ln2Y,&TT,&TT,realContext);
        realDivide  (&SS,&TT,&RR2,realContext);
        realSquareRoot(&RR2,RR_,realContext);

        #if defined STATDEBUG && defined PC_BUILD
          printf("##### ROOTF\n");
          formatRealDebug(ss, aa1); printf("§§ A1: %s\n",ss);
          formatRealDebug(ss, aa0); printf("§§ A0: %s\n",ss);
          formatRealDebug(ss, RR_); printf("§§ r: %s\n",ss);
        #endif
        #if defined STAT_DISPLAY_ABCDEFG && defined PC_BUILD
          realToDouble1(SIGMA_N, &v); printf("§§ n: %f\n",v);
          realToDouble1(&AA, &v); printf("§§ AA: %f\n",v);
          realToDouble1(&BB, &v); printf("§§ BB: %f\n",v);
          realToDouble1(&CC, &v); printf("§§ CC: %f\n",v);
          realToDouble1(aa1, &v); printf("§§ A1: %f\n",v);
          realToDouble1(aa0, &v); printf("§§ A0: %f\n",v);
          realToDouble1(RR_, &v); printf("§§ r:  %f\n",v);
          realToDouble1(&RR2,&v); printf("§§ r^2:%f\n",v);
        #endif


        break;


      case CF_HYPERBOLIC_FITTING :
        //      a0 = (sumx2 * sum1ony - sumx * sumxony) / (nn*sumx2 - sumx * sumx);
        realMultiply(SIGMA_X2, SIGMA_1onY, &SS, realContext);
        realMultiply(SIGMA_X, SIGMA_XonY, &TT, realContext);
        realSubtract(&SS,&TT,&SS,realContext);
        realMultiply(SIGMA_N,SIGMA_X2,&TT,realContext);
        realMultiply(SIGMA_X, SIGMA_X, &UU, realContext);
        realSubtract(&TT,&UU,&TT,realContext);
        realDivide  (&SS,&TT,aa0,realContext);

        //     a1 = (nn * sumxony - sumx * sum1ony) / (nn * sumx2 - sumx * sumx);
        realMultiply(SIGMA_N, SIGMA_XonY, &SS, realContext);
        realMultiply(SIGMA_X, SIGMA_1onY, &TT, realContext);
        realSubtract(&SS,&TT,&SS,realContext);
        realMultiply(SIGMA_N,SIGMA_X2,&TT,realContext);
        realMultiply(SIGMA_X, SIGMA_X, &UU, realContext);
        realSubtract(&TT,&UU,&TT,realContext);
        realDivide  (&SS,&TT,aa1,realContext);

        //     r = sqrt ( (a0 * sum1ony + a1 * sumxony - 1.0/nn * sum1ony * sum1ony) / (sum1ony2 - 1.0/nn * sum1ony * sum1ony ) ); //(rHYP)
        realMultiply(aa0, SIGMA_1onY, &SS, realContext);
        realMultiply(aa1, SIGMA_XonY, &TT, realContext);
        realAdd     (&SS,&TT,&SS,realContext);   //ss=t1+t2
        realDivide  (const_1,SIGMA_N,&TT,realContext);
        realMultiply(&TT, SIGMA_1onY, &TT, realContext);
        realMultiply(&TT, SIGMA_1onY, &TT, realContext); //tt=t3
        realSubtract(&SS,&TT,&SS,realContext);   //t1+t2-t3
        realSubtract(SIGMA_1onY2,&TT,&UU,realContext); //use t3
        realDivide  (&SS,&UU,&RR2,realContext);
        realSquareRoot(&RR2,RR_,realContext);

        #if defined STATDEBUG && defined PC_BUILD
          printf("##### HYPF\n");
          formatRealDebug(ss, aa1); printf("§§ A1: %s\n",ss);
          formatRealDebug(ss, aa0); printf("§§ A0: %s\n",ss);
          formatRealDebug(ss, &RR2); printf("§§ r^2: %s\n",ss);
          formatRealDebug(ss, RR_); printf("§§ r: %s\n",ss);
        #endif
        break;


      case CF_PARABOLIC_FITTING :
        calc_BCD(&BB,&CC,&DD);
        calc_AEFG(&AA,&BB,&CC,&DD,&EE,&FF,&GG);

        //      a2 = F; //a2 = (A*B - C*D) / (A*E - C*C) = F. Not in ReM, but the formula is correct and prevents duplicate code.
        realCopy (&FF,aa2);

        //      a1 = G; //a1 = (D - a2 * C) / A = G; Not in ReM, but the formula is correct and prevents duplicate code.
        realCopy (&GG,aa1);

        //      a0 = 1.0/nn * (sumy - a2 * sumx2 - a1 * sumx);
        realMultiply(&FF, SIGMA_X2, &TT, realContext);
        realSubtract(SIGMA_Y , &TT, &HH, realContext);
        realMultiply(&GG, SIGMA_X, &TT, realContext);
        realSubtract(&HH, &TT, &HH, realContext);
        realDivide  (&HH,SIGMA_N,aa0,realContext);

        //      r = sqrt( (a0 * sumy + a1 * sumxy + a2 * sumx2y - 1.0/nn * sumy * sumy) / (sumy2 - 1.0/nn * sumy * sumy) );
        realMultiply(aa0, SIGMA_Y, &SS, realContext);
        realMultiply(aa1, SIGMA_XY, &TT, realContext);
        realAdd     (&SS, &TT,&SS,realContext);
        realMultiply(aa2, SIGMA_X2Y, &TT, realContext);
        realAdd     (&SS, &TT,&SS,realContext); //interim in SS
        realMultiply(SIGMA_Y, SIGMA_Y, &TT, realContext);
        realDivide  (&TT, SIGMA_N,&UU,realContext);
        realSubtract(&SS, &UU,&SS,realContext); //top in SS
        realSubtract(SIGMA_Y2, &UU,&TT,realContext);
        realDivide  (&SS, &TT,&RR2,realContext); //R^2
        realSquareRoot(&RR2, RR_, realContext);

        #if defined STAT_DISPLAY_ABCDEFG && defined PC_BUILD
          realToDouble1(SIGMA_N, &v); printf("§§ n: %f\n",v);
          realToDouble1(&AA, &v); printf("§§ AA: %f\n",v);
          realToDouble1(&BB, &v); printf("§§ BB: %f\n",v);
          realToDouble1(&CC, &v); printf("§§ CC: %f\n",v);
          realToDouble1(&DD, &v); printf("§§ DD: %f\n",v);
          realToDouble1(&EE, &v); printf("§§ EE: %f\n",v);
          realToDouble1(&FF, &v); printf("§§ FF: %f\n",v);
          realToDouble1(&GG, &v); printf("§§ GG: %f\n",v);
          realToDouble1(&HH, &v); printf("§§ HH: %f\n",v);
          realToDouble1(aa2, &v); printf("§§ A2: %f\n",v);
          realToDouble1(aa1, &v); printf("§§ A1: %f\n",v);
          realToDouble1(aa0, &v); printf("§§ A0: %f\n",v);
          realToDouble1(RR_, &v); printf("§§ r:  %f\n",v);
          realToDouble1(&RR2,&v); printf("§§ r^2:%f\n",v);
        #endif
        #if defined STATDEBUG && defined PC_BUILD
          printf("##### PARABF\n");
          formatRealDebug(ss, aa2); printf("§§ A2: %s\n",ss);
          formatRealDebug(ss, aa1); printf("§§ A1: %s\n",ss);
          formatRealDebug(ss, aa0); printf("§§ A0: %s\n",ss);
          formatRealDebug(ss, &RR2); printf("§§ r^2: %s\n",ss);
          formatRealDebug(ss, RR_); printf("§§ r: %s\n",ss);
        #endif
        break;



      case CF_GAUSS_FITTING :

        //        B = n.sumx2lny - sumx2 . sumlny
        realMultiply(SIGMA_N, SIGMA_X2lnY, &SS, realContext);
        realMultiply(SIGMA_X2, SIGMA_lnY, &TT, realContext);
        realSubtract(&SS,&TT,&BB,realContext);

        //        C = nn * sumx3 - sumx2 * sumx;              //Copy from parabola
        realMultiply(SIGMA_N, SIGMA_X3, &SS, realContext);
        realMultiply(SIGMA_X2, SIGMA_X, &TT, realContext);
        realSubtract(&SS, &TT, &CC, realContext);

        //        D = n.sumxlny - sumx . sumlny
        realMultiply(SIGMA_N, SIGMA_XlnY, &SS, realContext);
        realMultiply(SIGMA_X, SIGMA_lnY, &TT, realContext);
        realSubtract(&SS,&TT,&DD,realContext);
       

        calc_AEFG(&AA,&BB,&CC,&DD,&EE,&FF,&GG);

        //        H = (1.0)/nn * (sumlny - F * sumx2 - G * sumx);
        realMultiply(&FF, SIGMA_X2, &TT, realContext);
        realSubtract(SIGMA_lnY, &TT, &HH, realContext);
        realMultiply(&GG, SIGMA_X, &TT, realContext);
        realSubtract(&HH, &TT, &HH, realContext);
        realDivide  (&HH,SIGMA_N,&HH,realContext);

        //        a2 = (1.0)/F;
        realDivide  (const_1,&FF,aa2,realContext);

        //        a1 = -G/(2.0) * a2;
        realDivide  (&GG,const__1,&TT,realContext);
        realDivide  (&TT,const_2 ,&TT,realContext);
        realMultiply(&TT, aa2, aa1, realContext);

        //GAUSS  a0 = exp (H - F * a1 * a1); //maxy;
        realMultiply(aa1,aa1,&SS,realContext);
        realMultiply(&SS,&FF,&SS,realContext);
        realSubtract(&HH,&SS,aa0,realContext);
        realExp(aa0,aa0,realContext);           

        //        r  = sqrt ( ( H * sumlny + G * sumxlny + F * sumx2lny - 1.0/nn * sumlny * sumlny ) / (sumln2y - 1.0/nn * sumlny * sumlny) );
        realMultiply(&HH, SIGMA_lnY, &SS, realContext);
        realMultiply(&GG, SIGMA_XlnY, &TT, realContext);
        realAdd     (&SS, &TT,&SS,realContext);
        realMultiply(&FF, SIGMA_X2lnY, &TT, realContext);
        realAdd     (&SS, &TT,&SS,realContext); //interim in SS
        realMultiply(SIGMA_lnY, SIGMA_lnY, &TT, realContext);
        realDivide  (&TT, SIGMA_N,&UU,realContext);
        realSubtract(&SS, &UU,&SS,realContext); //top in SS

        realSubtract(SIGMA_ln2Y, &UU,&TT,realContext); //use UU from above
        realDivide  (&SS, &TT,&RR2,realContext); //R^2
        realSquareRoot(&RR2, RR_, realContext);

        #if defined STAT_DISPLAY_ABCDEFG && defined PC_BUILD
          realToDouble1(SIGMA_N, &v); printf("§§ n: %f\n",v);
          realToDouble1(&AA, &v); printf("§§ AA: %f\n",v);
          realToDouble1(&BB, &v); printf("§§ BB: %f\n",v);
          realToDouble1(&CC, &v); printf("§§ CC: %f\n",v);
          realToDouble1(&DD, &v); printf("§§ DD: %f\n",v);
          realToDouble1(&EE, &v); printf("§§ EE: %f\n",v);
          realToDouble1(&FF, &v); printf("§§ FF: %f\n",v);
          realToDouble1(&GG, &v); printf("§§ GG: %f\n",v);
          realToDouble1(&HH, &v); printf("§§ HH: %f\n",v);
          realToDouble1(aa2, &v); printf("§§ A2: %f\n",v);
          realToDouble1(aa1, &v); printf("§§ A1: %f\n",v);
          realToDouble1(aa0, &v); printf("§§ A0: %f\n",v);
          realToDouble1(RR_, &v); printf("§§ r:  %f\n",v);
          realToDouble1(&RR2,&v); printf("§§ r^2:%f\n",v);
        #endif
        #if defined STATDEBUG && defined PC_BUILD
          printf("##### GAUSSF\n");
          formatRealDebug(ss, aa2); printf("§§ A2: %s\n",ss);
          formatRealDebug(ss, aa1); printf("§§ A1: %s\n",ss);
          formatRealDebug(ss, aa0); printf("§§ A0: %s\n",ss);
          formatRealDebug(ss, &RR2); printf("§§ r^2: %s\n",ss);
          formatRealDebug(ss, RR_); printf("§§ r: %s\n",ss);
        #endif
        break;


      case CF_CAUCHY_FITTING :
        //      B = nn * sumx2ony - sumx2 * sum1ony;        
        realMultiply(SIGMA_N, SIGMA_X2onY, &SS, realContext);
        realMultiply(SIGMA_X2, SIGMA_1onY, &TT, realContext);
        realSubtract(&SS, &TT, &BB, realContext);

        //      C = nn * sumx3 - sumx * sumx2;                     //vv C copied from PARABF. Exactly the same
        realMultiply(SIGMA_N, SIGMA_X3, &SS, realContext);
        realMultiply(SIGMA_X2, SIGMA_X, &TT, realContext);
        realSubtract(&SS, &TT, &CC, realContext);

        //      D = nn * sumxony - sumx * sum1ony;
        realMultiply(SIGMA_N, SIGMA_XonY, &SS, realContext);
        realMultiply(SIGMA_X, SIGMA_1onY, &TT, realContext);
        realSubtract(&SS, &TT, &DD, realContext);

        calc_AEFG(&AA,&BB,&CC,&DD,&EE,&FF,&GG);

        //    H = 1.0/nn * (sum1ony - R12 * sumx - R13 * sumx2);
        //old GAUSS CHANGED TO 1/y ; F and G left:      H = (1.0)/nn * (sum1ony - F * sumx2 - G * sumx);
        realMultiply(&FF, SIGMA_X2, &TT, realContext);
        realSubtract(SIGMA_1onY , &TT, &HH, realContext);
        realMultiply(&GG, SIGMA_X, &TT, realContext);
        realSubtract(&HH, &TT, &HH, realContext);
        realDivide  (&HH,SIGMA_N,&HH,realContext);

        //        a0 = F;
        realCopy    (&FF,aa0);

        //        a1 = G/2.0 * a0;
        realDivide  (&GG,const_2,&TT,realContext);
        realDivide  (&TT, aa0, aa1, realContext);         //err

        //      a2 = H - F * a1 * a1;
        realMultiply(aa1,aa1,&SS,realContext);
        realMultiply(&SS,&FF,&SS,realContext);
        realSubtract(&HH,&SS,aa2,realContext);

        //    r  = sqrt ( (H * sum1ony + G * sumxony + F * sumx2ony - 1.0/nn * sum1ony * sum1ony) / (sum1ony2 - 1.0/nn * sum1ony * sum1ony) );
        realMultiply(&HH, SIGMA_1onY, &SS, realContext);
        realMultiply(&GG, SIGMA_XonY, &TT, realContext);
        realAdd     (&SS, &TT,&SS,realContext);
        realMultiply(&FF, SIGMA_X2onY, &TT, realContext);
        realAdd     (&SS, &TT,&SS,realContext); //interim in SS
        realMultiply(SIGMA_1onY, SIGMA_1onY, &TT, realContext);
        realDivide  (&TT, SIGMA_N,&UU,realContext);
        realSubtract(&SS, &UU,&SS,realContext); //top in SS

        realSubtract(SIGMA_1onY2, &UU,&TT,realContext); //use UU from above
        realDivide  (&SS, &TT,&RR2,realContext); //R^2
        realSquareRoot(&RR2, RR_, realContext);

        #if defined STAT_DISPLAY_ABCDEFG && defined PC_BUILD
          realToDouble1(SIGMA_N, &v); printf("§§ n: %f\n",v);
          realToDouble1(&AA, &v); printf("§§ AA: %f\n",v);
          realToDouble1(&BB, &v); printf("§§ BB: %f\n",v);
          realToDouble1(&CC, &v); printf("§§ CC: %f\n",v);
          realToDouble1(&DD, &v); printf("§§ DD: %f\n",v);
          realToDouble1(&EE, &v); printf("§§ EE: %f\n",v);
          realToDouble1(&FF, &v); printf("§§ FF: %f\n",v);
          realToDouble1(&GG, &v); printf("§§ GG: %f\n",v);
          realToDouble1(&HH, &v); printf("§§ HH: %f\n",v);
          realToDouble1(aa2, &v); printf("§§ A2: %f\n",v);
          realToDouble1(aa1, &v); printf("§§ A1: %f\n",v);
          realToDouble1(aa0, &v); printf("§§ A0: %f\n",v);
          realToDouble1(RR_, &v); printf("§§ r:  %f\n",v);
          realToDouble1(&RR2,&v); printf("§§ r^2:%f\n",v);
        #endif
        #if defined STATDEBUG && defined PC_BUILD
          printf("##### CAUCHYF\n");
          formatRealDebug(ss, aa2); printf("§§ A2: %s\n",ss);
          formatRealDebug(ss, aa1); printf("§§ A1: %s\n",ss);
          formatRealDebug(ss, aa0); printf("§§ A0: %s\n",ss);
          formatRealDebug(ss, &RR2); printf("§§ r^2: %s\n",ss);
          formatRealDebug(ss, RR_); printf("§§ r: %s\n",ss);
        #endif
        break;


      case CF_ORTHOGONAL_FITTING :
        // a1 = 1.0 / (2.0 * sxy) * ( sy * sy - sx * sx + sqrt( (sy * sy - sx * sx) * (sy * sy - sx * sx) + 4 * sxy * sxy) );
        // a0  = y_ - a1 * x_;
        realMultiply(&S_Y,&S_Y,&SS,realContext);
        realMultiply(&S_X,&S_X,&TT,realContext);
        realSubtract(&SS,&TT,&UU,realContext);    //keep  uu = sy2-sx2
        realMultiply(&UU,&UU,&VV,realContext);
        realMultiply(&S_XY,&S_XY,&WW,realContext);
        realMultiply(&WW,const_2,&WW,realContext);
        realMultiply(&WW,const_2,&WW,realContext);
        realAdd     (&WW,&VV,&VV,realContext);
        realSquareRoot(&VV,&VV,realContext);  //sqrt term

        realDivide(&UU,const_2,&UU,realContext);  //term1
        realDivide(&UU,&S_XY,&UU,realContext);

        realDivide(&VV,const_2,&VV,realContext);  //term2
        realDivide(&VV,&S_XY,&VV,realContext);

        realAdd     (&UU,&VV,aa1,realContext);   //a1
        realMultiply(aa1,&M_X,&SS,realContext);
        realSubtract(&M_Y,&SS,aa0,realContext);  //a0
          
        //r is copied from LINF
        //       r  = (nn * sumxy - sumx * sumy) / (sqrt (nn * sumx2 - sumx * sumx) * sqrt(nn * sumy2 - sumy * sumy) );
        realMultiply(SIGMA_N,SIGMA_XY,&SS,realContext);
        realMultiply(SIGMA_X,SIGMA_Y,&TT,realContext);
        realSubtract(&SS,&TT,&SS,realContext);          //SS is top

        realMultiply(SIGMA_N,SIGMA_X2,&TT,realContext);
        realMultiply(SIGMA_X,SIGMA_X,&UU,realContext);
        realSubtract(&TT,&UU,&TT,realContext);
        realSquareRoot(&TT,&TT,realContext);            //TT is bottom, factor 1

        realMultiply(SIGMA_N,SIGMA_Y2,&UU,realContext);
        realMultiply(SIGMA_Y,SIGMA_Y,&VV,realContext);
        realSubtract(&UU,&VV,&UU,realContext);
        realSquareRoot(&UU,&UU,realContext);            //UU is bottom factor 2

        realDivide(&SS,&TT,RR_,realContext);
        realDivide(RR_,&UU,RR_,realContext);            //r

        #if defined STATDEBUG && defined PC_BUILD
          formatRealDebug(ss, &S_X); printf("§§ S_X: %s\n",ss);
          formatRealDebug(ss, &S_Y); printf("§§ S_Y: %s\n",ss);
          formatRealDebug(ss, &S_XY); printf("§§ SXY: %s\n",ss);
          formatRealDebug(ss, &M_X); printf("§§ M_X: %s\n",ss);
          formatRealDebug(ss, &M_Y); printf("§§ M_Y: %s\n",ss);
          formatRealDebug(ss, SIGMA_N ); printf("§§ SIGMA_N : %s\n",ss);
          formatRealDebug(ss, SIGMA_X ); printf("§§ SIGMA_X : %s\n",ss);
          formatRealDebug(ss, SIGMA_Y ); printf("§§ SIGMA_Y : %s\n",ss);
          formatRealDebug(ss, SIGMA_XY); printf("§§ SIGMA_XY: %s\n",ss);
          formatRealDebug(ss, SIGMA_X2); printf("§§ SIGMA_X2: %s\n",ss);
          formatRealDebug(ss, SIGMA_Y2); printf("§§ SIGMA_Y2: %s\n",ss);
        #endif

        #if defined STATDEBUG && defined PC_BUILD
          printf("##### ORTHOF\n");
          formatRealDebug(ss, aa1); printf("§§ A1: %s\n",ss);
          formatRealDebug(ss, aa0); printf("§§ A0: %s\n",ss);
          formatRealDebug(ss, RR_); printf("§§ r: %s\n",ss);
        #endif
        break;

      default: break;
    }
  }


  void yIsFnx(uint8_t USEFLOAT, uint16_t selection, double x, double *y, double a0, double a1, double a2, real_t *XX, real_t *YY, real_t *RR, real_t *SMI, real_t *aa0, real_t *aa1, real_t *aa2){
      *y = 0;
      float yf;
      realCopy(const_0,YY);
      real_t SS,TT,UU;
      if(USEFLOAT == useREAL4) {
        realContextForecast = &ctxtRealShort;
      } else 
      if(USEFLOAT == useREAL39) {
        realContextForecast = &ctxtReal39;
      }
      switch(selection) {
        case CF_LINEAR_FITTING: 
        case CF_ORTHOGONAL_FITTING:
          if(USEFLOAT == 0) { 
            *y = a1 * x + a0; 
          } else {
            realMultiply(XX, aa1, &UU, realContextForecast);
            realAdd     (&UU, aa0, YY, realContextForecast);
            realToFloat/*Double*/(YY, &yf); *y = (double)yf;
          }
          break;
        case CF_EXPONENTIAL_FITTING: 
          if(USEFLOAT == 0) { 
            *y = a0 * exp(a1 * x); 
          } else {
            realMultiply(XX, aa1, &UU, realContextForecast);
            realExp     (&UU, &UU,     realContextForecast);
            realMultiply(&UU, aa0, YY, realContextForecast);
            realToFloat/*Double*/(YY, &yf); *y = (double)yf;
          }
          break;
        case CF_LOGARITHMIC_FITTING: 
          if(USEFLOAT == 0) { 
            *y = a0 + a1*log(x); 
          } else {
            WP34S_Ln    (XX, &SS,       realContextForecast);
            realMultiply(&SS, aa1, &UU, realContextForecast);
            realAdd     (&UU, aa0, YY,  realContextForecast);
            realToFloat/*Double*/(YY, &yf); *y = (double)yf;
          }
          break;
        case CF_POWER_FITTING: 
          if(USEFLOAT == 0) { 
            *y = a0 * pow(x,a1);
          } else {
            realPower   (XX, aa1, &SS, realContextForecast);
            realMultiply(&SS, aa0, YY, realContextForecast);
            realToFloat/*Double*/(YY, &yf); *y = (double)yf;
          }
          break;
        case CF_ROOT_FITTING: 
          if(USEFLOAT == 0) { 
            *y = a0 * pow(a1,1/x);
          } else {
            realDivide  (const_1, XX, &SS, realContextForecast);
            realPower   (aa1, &SS, &SS,    realContextForecast);    //very very slow with a1=0.9982, probably in the 0.4 < x < 1.0 area
            realMultiply(&SS, aa0, YY,     realContextForecast);
            realToFloat/*Double*/(YY, &yf); *y = (double)yf;
          }
          break;
        case CF_HYPERBOLIC_FITTING: 
          if(USEFLOAT == 0) { 
            *y = 1 / (a1 * x + a0);
          } else {
            realMultiply(XX, aa1, &UU,     realContextForecast);
            realAdd     (&UU, aa0, &TT,    realContextForecast);
            realDivide  (const_1, &TT, YY, realContextForecast);
            realToFloat/*Double*/(YY, &yf); *y = (double)yf;
          }
          break;
        case CF_PARABOLIC_FITTING: 
          if(USEFLOAT == 0) { 
            *y = a2 * x * x + a1 * x + a0;
          } else {
            realMultiply(XX, XX , &TT,   realContextForecast);
            realMultiply(&TT, aa2, &TT,  realContextForecast);
            realMultiply(XX, aa1, &UU,   realContextForecast);
            realAdd     (&TT, &UU,  &TT, realContextForecast);
            realAdd     (&TT, aa0,  YY , realContextForecast);          
            realToFloat/*Double*/(YY, &yf); *y = (double)yf;
          }
          break;
        case CF_GAUSS_FITTING:
          if(USEFLOAT == 0) { 
            *y = a0 * exp( (x-a1)*(x-a1)/a2 );
          } else {
            realSubtract(XX, aa1, &TT,   realContextForecast);
            realMultiply(&TT, &TT , &TT, realContextForecast);
            realDivide  (&TT, aa2, &TT,  realContextForecast);
            realExp     (&TT, &TT ,      realContextForecast);
            realMultiply(&TT, aa0, YY,   realContextForecast);
            realToFloat/*Double*/(YY, &yf); *y = (double)yf;
          }
          break;
        case CF_CAUCHY_FITTING:
          if(USEFLOAT == 0) { 
            *y = 1/(a0*(x+a1)*(x+a1)+a2);
          } else {
            realAdd     (XX, aa1, &TT,   realContextForecast);
            realMultiply(&TT, &TT , &TT, realContextForecast);
            realMultiply(&TT, aa0, &TT,  realContextForecast);
            realAdd     (&TT, aa2, &TT,  realContextForecast);
            realDivide  (const_1, &TT, YY, realContextForecast);
            realToFloat/*Double*/(YY, &yf); *y = (double)yf;
          }
        default:break;
      }
  }


  void fnYIsFnx(uint16_t unusedButMandatoryParameter){
    real_t XX,YY,RR,SMI,aa0,aa1,aa2;
    double x=-99,y = 0,a0=-99,a1=-99,a2=-99;
    realCopy(const_0,&aa0);
    realCopy(const_0,&aa1);
    realCopy(const_0,&aa2);
    if(checkMinimumDataPoints(const_2)) {
      if(lrChosen == 0) {                    //if lrChosen contains something, the stat data exists, otherwise set it to linear. lrSelection still has 1 at this point, i.e. the * will not appear.
        lrChosen = CF_LINEAR_FITTING;
      }
      processCurvefitSelection(lrChosen, &RR, &SMI, &aa0, &aa1, &aa2);
      if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
        convertLongIntegerRegisterToReal34Register (REGISTER_X, REGISTER_X);
      }
      if(getRegisterDataType(REGISTER_X) == dtReal34) {
        real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &XX);
        yIsFnx(useREAL39, lrChosen, x, &y, a0, a1, a2, &XX, &YY, &RR, &SMI, &aa0, &aa1, &aa2);
        realToReal34(&YY,REGISTER_REAL34_DATA(REGISTER_X));

        setSystemFlag(FLAG_ASLIFT);
        temporaryInformation = TI_CALCY;
      } 
      else {
        displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          sprintf(errorMessage, "data type %s cannot be used with L.R.!", getRegisterDataTypeName(REGISTER_X, false, false));
          moreInfoOnError("In function fnYIsFnx:", errorMessage, NULL, NULL);
        #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      }
    }
  }




  void xIsFny(uint16_t selection, uint8_t rootNo, real_t *XX, real_t *YY, real_t *RR, real_t *SMI, real_t *aa0, real_t *aa1, real_t *aa2){
      realCopy(const_0,XX);
      real_t SS,TT,UU;
      realContextForecast = &ctxtReal39;
      switch(selection) {
        case CF_LINEAR_FITTING: 
        case CF_ORTHOGONAL_FITTING:
          //x = (y - a0) / a1;
          realSubtract(YY, aa0, &UU, realContextForecast);
          realDivide  (&UU,aa1, &TT, realContextForecast);
          realCopy    (&TT,XX);
          temporaryInformation = TI_CALCX;
          break;
        case CF_EXPONENTIAL_FITTING:
          realDivide(YY,aa0,&UU,realContextForecast);
          WP34S_Ln(&UU, &UU, realContextForecast);
          realDivide(&UU,aa1,XX,realContextForecast);
          temporaryInformation = TI_CALCX;
          break;
        case CF_LOGARITHMIC_FITTING:
          realSubtract(YY,aa0,&UU,realContextForecast);
          realDivide(&UU,aa1,&UU,realContextForecast);
          realExp(&UU,XX,realContextForecast);
          temporaryInformation = TI_CALCX;
          break;
        case CF_POWER_FITTING:
          realDivide(YY,aa0,&UU,realContextForecast);
          xthRootReal(&UU,aa1,realContextForecast);             //Note X-register gets written here
          real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), XX);
          temporaryInformation = TI_CALCX;
          break;
        case CF_ROOT_FITTING:
          WP34S_Ln(YY,YY,realContextForecast);
          WP34S_Ln(aa0,&UU,realContextForecast);
          realSubtract(YY,&UU,YY,realContextForecast);
          WP34S_Ln(aa1,&UU,realContextForecast);
          realDivide(&UU,YY,XX,realContextForecast);
          temporaryInformation = TI_CALCX;
          break;
        case CF_HYPERBOLIC_FITTING:
          realDivide(const_1,YY,&UU,realContextForecast);
          realSubtract(&UU,aa0,&UU,realContextForecast);
          realDivide(&UU,aa1,XX,realContextForecast);
          temporaryInformation = TI_CALCX;
          break;
        case CF_PARABOLIC_FITTING:
          // (1/(2.a2)) . ( -a1 +- sqrt(a1.a1 - 4a2.(a0 - y) ) )
          realSubtract(YY,aa0,&UU,realContextForecast);
          realMultiply(const_2,&UU,&UU,realContextForecast);
          realMultiply(const_2,&UU,&UU,realContextForecast);
          realMultiply(aa2,&UU,&UU,realContextForecast);
          realMultiply(aa1,aa1,&TT,realContextForecast);
          realAdd     (&UU,&TT,&UU,realContextForecast);        //swapped terms around minus, therefore add
          realSquareRoot(&UU,&UU,realContextForecast);

          realSubtract(const_0,aa1,&SS,realContextForecast);
          if(rootNo == 1)
            realSubtract(&SS,&UU,&SS,realContextForecast);      //This term could be Add due to plus and minus
          if(rootNo == 2)
            realAdd   (&SS,&UU,&SS,realContextForecast);        //This term could be Add due to plus and minus

          realDivide(&SS,const_2,&SS,realContextForecast);
          realDivide(&SS,aa2,XX,realContextForecast);
          temporaryInformation = TI_CALCX2;
          break;

        case CF_CAUCHY_FITTING:
          realDivide(const_1,YY,&UU,realContextForecast);
          realSubtract(&UU,aa2,&UU,realContextForecast);
          realDivide(&UU,aa0,&UU,realContextForecast);
          realSquareRoot(&UU,&UU,realContextForecast);
          realSubtract(const_0,aa1,&SS,realContextForecast);
          if(rootNo == 1)
            realSubtract(&SS,&UU,XX,realContextForecast);
          if(rootNo == 2)
            realAdd   (&SS,&UU,XX,realContextForecast);
          temporaryInformation = TI_CALCX2;
          break;

        case CF_GAUSS_FITTING:
          realDivide(YY,aa0,&UU,realContextForecast);
          WP34S_Ln(&UU,&UU,realContextForecast);
          realMultiply(&UU,aa2,&UU,realContextForecast);
          realSquareRoot(&UU,&UU,realContextForecast);
          if(rootNo == 1)
            realSubtract(aa1,&UU,XX,realContextForecast);
          if(rootNo == 2)
            realAdd   (aa1,&UU,XX,realContextForecast);
          temporaryInformation = TI_CALCX2;
          break;

        default:break;
      }
  }



  void fnXIsFny(uint16_t unusedButMandatoryParameter){
  real_t XX,YY,RR,SMI,aa0,aa1,aa2;
  realCopy(const_0,&aa0);
  realCopy(const_0,&aa1);
  realCopy(const_0,&aa2);
  if(checkMinimumDataPoints(const_2)) {
    if(lrChosen == 0) {                    //if lrChosen contains something, the stat data exists, otherwise set it to linear. lrSelection still has 1 at this point, i.e. the * will not appear.
      lrChosen = CF_LINEAR_FITTING;
    }
    processCurvefitSelection(lrChosen, &RR, &SMI, &aa0, &aa1, &aa2);
    if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
      convertLongIntegerRegisterToReal34Register (REGISTER_X, REGISTER_X);
    }
    if(getRegisterDataType(REGISTER_X) == dtReal34) {
      real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &YY);
      xIsFny(lrChosen, 1, &XX, &YY, &RR, &SMI, &aa0, &aa1, &aa2);
      realToReal34(&XX,REGISTER_REAL34_DATA(REGISTER_X));

      if(lrChosen == CF_PARABOLIC_FITTING || lrChosen == CF_GAUSS_FITTING || lrChosen == CF_CAUCHY_FITTING) {
        xIsFny(lrChosen, 2, &XX, &YY, &RR, &SMI, &aa0, &aa1, &aa2);        
        liftStack();
        setSystemFlag(FLAG_ASLIFT);
        reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
        realToReal34(&XX,REGISTER_REAL34_DATA(REGISTER_X));
      }

      setSystemFlag(FLAG_ASLIFT);
    } else {
      displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "data type %s cannot be used with L.R.!", getRegisterDataTypeName(REGISTER_X, false, false));
        moreInfoOnError("In function fnXIsFny:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
  }
}
