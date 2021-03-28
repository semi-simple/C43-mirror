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

#include "wp43s.h"
#include "math.h"

/********************************************//**
 * \brief Sets the curve fitting mode
 *
 * \param[in] curveFitting uint16_t Curve fitting mode
 * \return void
 *
 ***********************************************/
void fnCurveFitting(uint16_t curveFitting) {
    lrSelection = curveFitting;
    processCurvefitSelection(curveFitting);

  #ifdef PC_BUILD
    uint16_t numberOfOnes;

    numberOfOnes = curveFitting - ((curveFitting >> 1) & 0x5555);
    numberOfOnes = (numberOfOnes & 0x3333) + ((numberOfOnes >> 2) & 0x3333);
    numberOfOnes = (numberOfOnes + (numberOfOnes >> 4)) & 0x0f0f;
    numberOfOnes = (uint16_t)(numberOfOnes * 0x0101) >> 8;

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

real_t AA,BB,CC,DD,EE,FF,GG,HH;             // Curve aux fitting variables
real_t SS,TT,UU,aa0,aa1,aa2;                // Curve fitting variables
realContext_t *realContext;

double a0,a1,a2,smi,r;                      //vv Temporary, used as cross check of the math; can be removed later
double A, B, C, D, E, F, G, H;
int32_t nn;
double sumx;
double sumy;
double sumx2;
double sumx2y;
double sumy2;
double sumxy;
double sumlnxlny;
double sumx2lny;
double sumlnx;
double sumln2x;
double sumylnx;
double sumlny;
double sumln2y;
double sumxlny;
double sumlnyonx;
double sumx2ony;
double sum1onx;
double sum1onx2;
double sumxony;
double sum1ony;
double sum1ony2;
double sumx3;
double sumx4;
double maxy;                           //^^

void calc_BCD(void){                        //Aux terms, calc_BCD must be run before calc_AEFG
realContext = &ctxtReal75;
real_t SS,TT;
char ss[100];
  //        B = nn * sumx2y - sumx2 * sumy;
  realMultiply(SIGMA_N, SIGMA_X2Y, &SS, realContext);
  realMultiply(SIGMA_X2, SIGMA_Y, &TT, realContext);
  realSubtract(&SS, &TT, &BB, realContext);
  realToString(&BB,ss); B = strtof (ss, NULL);
  #ifdef PC_BUILD
    printf("§ B: %f %f\n",B,nn * sumx2y - sumx2 * sumy);
  #endif //PC_BUILD

  //        C = nn * sumx3 - sumx2 * sumx;
  realMultiply(SIGMA_N, SIGMA_X3, &SS, realContext);
  realMultiply(SIGMA_X2, SIGMA_X, &TT, realContext);
  realSubtract(&SS, &TT, &CC, realContext);
  realToString(&CC,ss); C = strtof (ss, NULL);
  #ifdef PC_BUILD
    printf("§ C: %f %f\n",C,nn * sumx3 - sumx2 * sumx);
  #endif //PC_BUILD

  //        D = nn * sumxy - sumx * sumy;
  realMultiply(SIGMA_N, SIGMA_XY, &SS, realContext);
  realMultiply(SIGMA_X, SIGMA_Y, &TT, realContext);
  realSubtract(&SS, &TT, &DD, realContext);
  realToString(&DD,ss); D = strtof (ss, NULL);
  #ifdef PC_BUILD
    printf("§ D: %f %f\n",D,nn * sumxy - sumx * sumy);
  #endif //PC_BUILD
}



void calc_AEFG(void){                        //Aux terms, calc_AEFG must be run after calc_BCD
realContext = &ctxtReal75;
real_t SS,TT,UU;
char ss[100];
  //        A = nn * sumx2 - sumx * sumx;
  realMultiply(SIGMA_N, SIGMA_X2, &SS, realContext);
  realMultiply(SIGMA_X, SIGMA_X, &TT, realContext);
  realSubtract(&SS, &TT, &AA, realContext);
  realToString(&AA,ss); A = strtof (ss, NULL);
  #ifdef PC_BUILD
    printf("§ A: %f %f\n",A,nn * sumx2 - sumx * sumx);
  #endif //PC_BUILD

  //        E = nn * sumx4 - sumx2 * sumx2;
  realMultiply(SIGMA_N, SIGMA_X4, &SS, realContext);
  realMultiply(SIGMA_X2, SIGMA_X2, &TT, realContext);
  realSubtract(&SS, &TT, &EE, realContext);
  realToString(&EE,ss); E = strtof (ss, NULL);
  #ifdef PC_BUILD
    printf("§ E: %f %f\n",E,nn * sumx4 - sumx2 * sumx2);
  #endif //PC_BUILD

  //USING COMPONENTS OF BCD
  //        F = (A*B - C*D) / (A*E - C*C);    //interchangably the a2 in PARABF
  realMultiply(&AA, &BB, &SS, realContext);
  realMultiply(&CC, &DD, &TT, realContext);
  realSubtract(&SS, &TT, &UU, realContext);
  realMultiply(&AA, &EE, &SS, realContext);
  realMultiply(&CC, &CC, &TT, realContext);
  realSubtract(&SS, &TT, &SS, realContext);
  realDivide  (&UU,&SS,&FF,realContext);
  realToString(&FF,ss); F = strtof (ss, NULL);
  #ifdef PC_BUILD
    printf("§ F: %f %f\n",F,(A*B - C*D) / (A*E - C*C));
  #endif //PC_BUILD

  //        G = (D - F * C) / A;
  realMultiply(&FF, &CC, &SS, realContext);
  realSubtract(&DD, &SS, &SS, realContext);
  realDivide  (&SS,&AA,&GG,realContext);
  realToString(&GG,ss); G = strtof (ss, NULL);
  #ifdef PC_BUILD
    printf("§ G: %f %f\n",G,(D - F * C) / A);
  #endif //PC_BUILD
}



void processCurvefitSelection(uint16_t selection){
    #ifdef PC_BUILD
      printf("processCurvefitSelection %u\n",selection);
    #endif

    uint16_t ix,jx;               //only a single graph can be displayed at once, so retain the single lowest bit, and clear the higher order bits.
    jx = 0;
    for(ix=0; ix!=15; ix++) {
      jx = selection & (2 << ix);
      if(jx) break;
    }
    selection = jx;


    realContext = &ctxtReal75;    //Use 75 as the sums can reach high values and the accuracy of the regressionn depends on this. Could arguably be optimized.
    char ss[100];
    real_t VV,WW;
    real_t S_X,S_Y,S_XY,RR_,SMI_,M_X,M_Y;
    double a1a, a1b, a0a, a0b;

    realToInt32(SIGMA_N, nn);                                        // Temporary, for ease of checking using double type
    realToString(SIGMA_X ,     ss); sumx      = strtof (ss, NULL);
    realToString(SIGMA_Y ,     ss); sumy      = strtof (ss, NULL);
    realToString(SIGMA_X2,     ss); sumx2     = strtof (ss, NULL);
    realToString(SIGMA_X2Y,    ss); sumx2y    = strtof (ss, NULL);
    realToString(SIGMA_Y2,     ss); sumy2     = strtof (ss, NULL);
    realToString(SIGMA_XY,     ss); sumxy     = strtof (ss, NULL);
    realToString(SIGMA_lnXlnY, ss); sumlnxlny = strtof (ss, NULL);
    realToString(SIGMA_lnX ,   ss); sumlnx    = strtof (ss, NULL);
    realToString(SIGMA_ln2X,   ss); sumln2x   = strtof (ss, NULL);
    realToString(SIGMA_YlnX,   ss); sumylnx   = strtof (ss, NULL);
    realToString(SIGMA_lnY ,   ss); sumlny    = strtof (ss, NULL);
    realToString(SIGMA_ln2Y,   ss); sumln2y   = strtof (ss, NULL);
    realToString(SIGMA_XlnY,   ss); sumxlny   = strtof (ss, NULL);
    realToString(SIGMA_X2lnY,  ss); sumx2lny  = strtof (ss, NULL);
    realToString(SIGMA_lnYonX, ss); sumlnyonx = strtof (ss, NULL);
    realToString(SIGMA_X2onY,  ss); sumx2ony  = strtof (ss, NULL);
    realToString(SIGMA_1onX,   ss); sum1onx   = strtof (ss, NULL);
    realToString(SIGMA_1onX2,  ss); sum1onx2  = strtof (ss, NULL);
    realToString(SIGMA_XonY,   ss); sumxony   = strtof (ss, NULL);
    realToString(SIGMA_1onY,   ss); sum1ony   = strtof (ss, NULL);
    realToString(SIGMA_1onY2,  ss); sum1ony2  = strtof (ss, NULL);
    realToString(SIGMA_X3,     ss); sumx3     = strtof (ss, NULL);
    realToString(SIGMA_X4,     ss); sumx4     = strtof (ss, NULL);
    realToString(SIGMA_YMAX,   ss); maxy      = strtof (ss, NULL);   // ^^

    realDivide(SIGMA_X,SIGMA_N,&M_X,&ctxtReal39);
    realDivide(SIGMA_Y,SIGMA_N,&M_Y,&ctxtReal39);
    fnStatR   (&RR_, &S_XY, &S_X, &S_Y);
    fnStatSMI (&SMI_);
    #ifdef PC_BUILD
      realToString(&SMI_, ss);  smi = strtof (ss, NULL);   // sx*sx*sy*sy*(1.0-r*r)/(sx*sx+r*r*sy*sy); 
    #endif

    #ifdef PC_BUILD
      double x_,y_,sx,sy,sxy;
      realToString(&M_X,  ss); x_  = strtof (ss, NULL);
      realToString(&M_Y,  ss); y_  = strtof (ss, NULL);
      realToString(&RR_,  ss); r   = strtof (ss, NULL);      // sxy / (sx * sy);
      realToString(&S_X,  ss); sx  = strtof (ss, NULL);
      realToString(&S_Y,  ss); sy  = strtof (ss, NULL);
      realToString(&S_XY, ss); sxy = strtof (ss, NULL);    // (1.0/(nn*(nn-1.0))) * ((nn*sumxy-sumx*sumy));
      printf("\nSelection:%i\n",selection);
      printf(">>> x_=%f\n",x_);
      printf(">>> y_=%f\n",y_);
      printf(">>> sx=%f\n",sx);
      printf(">>> sy=%f\n",sy);
      printf(">>> sxy=%f\n",sxy);
      printf(">>> r = %f\n",r);
      printf(">>> smi=%f\n",smi);
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
        realDivide(&SS,&TT,&aa0,realContext);

        realToString(&aa0,ss); a0 = strtof (ss, NULL);
        #ifdef PC_BUILD
          printf("§ a0: %f %f\n",a0,(sumx2 * sumy  - sumx * sumxy) / (nn * sumx2 - sumx * sumx));
        #endif //PC_BUILD

        //      a1 = (nn  * sumxy - sumx * sumy ) / (nn * sumx2 - sumx * sumx);
        realMultiply(SIGMA_N,SIGMA_XY,&SS,realContext);
        realMultiply(SIGMA_X,SIGMA_Y,&TT,realContext);
        realSubtract(&SS,&TT,&SS,realContext);
        realMultiply(SIGMA_N,SIGMA_X2,&TT,realContext);
        realMultiply(SIGMA_X,SIGMA_X,&UU,realContext);
        realSubtract(&TT,&UU,&TT,realContext);
        realDivide(&SS,&TT,&aa1,realContext);

        realToString(&aa1,ss); a1 = strtof (ss, NULL);
        #ifdef PC_BUILD
          printf("§ a1: %f %f\n",a1,(nn  * sumxy - sumx * sumy ) / (nn * sumx2 - sumx * sumx));
        #endif //PC_BUILD

        smi = 0;  //smi = sqrt(sx*sx*sy*sy*(1-r*r)/(sx*sx+r*r*sy*sy)); 

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

        realDivide(&SS,&TT,&RR_,realContext);
        realDivide(&RR_,&UU,&RR_,realContext);            //r

        realToString(&RR_, ss); r = strtof (ss, NULL);
        #ifdef PC_BUILD
          printf("§ r: %f %f\n",r, (nn * sumxy - sumx * sumy) / (sqrt (nn * sumx2 - sumx * sumx) * sqrt(nn * sumy2 - sumy * sumy) ));
        #endif //PC_BUILD


        #ifdef STATDEBUG
          printf("##### Linear %i a0=%f a1=%f \n",(int)nn, a0, a1);
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
        realDivide(&SS,&TT,&aa0,realContext);
        realExp(&aa0,&aa0,realContext);

        realToString(&aa0,ss); a0 = strtof (ss, NULL);
        #ifdef PC_BUILD
          printf("§ a0: %f %f\n",a0,exp( (sumx2 * sumlny  - sumx * sumxlny) / (nn * sumx2 - sumx * sumx) ));
        #endif //PC_BUILD

        //a1 = (nn  * sumxlny - sumx * sumlny ) / (nn * sumx2 - sumx * sumx);
        realMultiply(SIGMA_N,SIGMA_XlnY,&SS,realContext);
        realMultiply(SIGMA_X,SIGMA_lnY,&TT,realContext);
        realSubtract(&SS,&TT,&SS,realContext);
        realMultiply(SIGMA_N,SIGMA_X2,&TT,realContext);
        realMultiply(SIGMA_X,SIGMA_X,&UU,realContext);
        realSubtract(&TT,&UU,&TT,realContext);
        realDivide(&SS,&TT,&aa1,realContext);

        realToString(&aa1,ss); a1 = strtof (ss, NULL);
        #ifdef PC_BUILD
          printf("§ a1: %f %f\n",a1,(nn  * sumxlny - sumx * sumlny ) / (nn * sumx2 - sumx * sumx));
        #endif //PC_BUILD

        smi = 0;  //smi = sqrt(sx*sx*sy*sy*(1-r*r)/(sx*sx+r*r*sy*sy)); 

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

        realDivide(&SS,&TT,&RR_,realContext);
        realDivide(&RR_,&UU,&RR_,realContext);            //r

        realToString(&RR_, ss); r = strtof (ss, NULL);
        #ifdef PC_BUILD
          printf("§ r: %f %f\n",r, (nn * sumxlny - sumx*sumlny) / (sqrt(nn*sumx2-sumx*sumx) * sqrt(nn*sumln2y-sumlny*sumlny))); //(rEXP));
        #endif //PC_BUILD
        #ifdef STATDEBUG
          printf("##### EXPF %i a0=%f a1=%f \n",(int)nn, a0, a1);
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
        realDivide(&SS,&TT,&aa0,realContext);

        realToString(&aa0,ss); a0 = strtof (ss, NULL);
        #ifdef PC_BUILD
          printf("§ a0: %f %f\n",a0,(sumln2x * sumy  - sumlnx * sumylnx) / (nn * sumln2x - sumlnx * sumlnx));
        #endif //PC_BUILD

        //a1 = (nn  * sumylnx - sumlnx * sumy ) / (nn * sumln2x - sumlnx * sumlnx);;
        realMultiply(SIGMA_N,SIGMA_YlnX,&SS,realContext);
        realMultiply(SIGMA_lnX,SIGMA_Y,&TT,realContext);
        realSubtract(&SS,&TT,&SS,realContext);
        realMultiply(SIGMA_N,SIGMA_ln2X,&TT,realContext);
        realMultiply(SIGMA_lnX,SIGMA_lnX,&UU,realContext);
        realSubtract(&TT,&UU,&TT,realContext);
        realDivide(&SS,&TT,&aa1,realContext);

        realToString(&aa1,ss); a1 = strtof (ss, NULL);
        #ifdef PC_BUILD
          printf("§ a1: %f %f\n",a1,(nn  * sumylnx - sumlnx * sumy ) / (nn * sumln2x - sumlnx * sumlnx));
        #endif //PC_BUILD

        smi = 0;  //smi = sqrt(sx*sx*sy*sy*(1-r*r)/(sx*sx+r*r*sy*sy)); 

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

        realDivide(&SS,&TT,&RR_,realContext);
        realDivide(&RR_,&UU,&RR_,realContext);            //r

        realToString(&RR_, ss); r = strtof (ss, NULL);
        #ifdef PC_BUILD
          printf("§ r: %f %f\n",r, (nn * sumylnx - sumlnx*sumy) / (sqrt(nn*sumln2x-sumlnx*sumlnx) * sqrt(nn*sumy2-sumy*sumy))); //(rLOG));
        #endif //PC_BUILD
        #ifdef STATDEBUG
          printf("##### LOGF %i a0=%f a1=%f \n",(int)nn, a0, a1);
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
        realDivide(&SS,&TT,&aa0,realContext);
        realExp(&aa0,&aa0,realContext);

        realToString(&aa0,ss); a0 = strtof (ss, NULL);
        #ifdef PC_BUILD
          printf("§ a0: %f %f\n",a0,exp( (sumln2x * sumlny  - sumlnx * sumlnxlny) / (nn * sumln2x - sumlnx * sumlnx) ));
        #endif //PC_BUILD

        //a1 = (nn  * sumlnxlny - sumlnx * sumlny ) / (nn * sumln2x - sumlnx * sumlnx);
        realMultiply(SIGMA_N,SIGMA_lnXlnY,&SS,realContext);
        realMultiply(SIGMA_lnX,SIGMA_lnY,&TT,realContext);
        realSubtract(&SS,&TT,&SS,realContext);
        realMultiply(SIGMA_N,SIGMA_ln2X,&TT,realContext);
        realMultiply(SIGMA_lnX,SIGMA_lnX,&UU,realContext);
        realSubtract(&TT,&UU,&TT,realContext);
        realDivide(&SS,&TT,&aa1,realContext);

        realToString(&aa1,ss); a1 = strtof (ss, NULL);
        #ifdef PC_BUILD
          printf("§ a1: %f %f\n",a1, exp( (sumln2x * sumlny  - sumlnx * sumlnxlny) / (nn * sumln2x - sumlnx * sumlnx)  ));
        #endif //PC_BUILD

        smi = 0;  //smi = sqrt(sx*sx*sy*sy*(1-r*r)/(sx*sx+r*r*sy*sy)); 

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

        realDivide(&SS,&TT,&RR_,realContext);
        realDivide(&RR_,&UU,&RR_,realContext);            //r

        realToString(&RR_, ss); r = strtof (ss, NULL);
        #ifdef PC_BUILD
          printf("§ r: %f %f\n",r, (nn * sumlnxlny - sumlnx*sumlny) / (sqrt(nn*sumln2x-sumlnx*sumlnx) * sqrt(nn*sumln2y-sumlny*sumlny))); //(rEXP));
        #endif //PC_BUILD
        #ifdef STATDEBUG
          printf("##### POWERF %i a0=%f a1=%f \n",nn, a0, a1);
        #endif
        break;


      case CF_ROOT_FITTING :
              A = nn * sum1onx2 - sum1onx * sum1onx;
              B = 1.0/A * (sum1onx2 * sumlny - sum1onx * sumlnyonx);
              C = 1.0/A * (nn * sumlnyonx - sum1onx * sumlny);

        realMultiply(SIGMA_N,SIGMA_1onX2,&SS,realContext);
        realMultiply(SIGMA_1onX,SIGMA_1onX,&AA,realContext);

        realMultiply(const_1,&AA,&SS,realContext);
        realMultiply(SIGMA_1onX2,SIGMA_lnY,&TT,realContext);
        realMultiply(SIGMA_1onX,SIGMA_lnYonX,&UU,realContext);
        realSubtract(&TT,&UU,&TT,realContext);
        realMultiply(&SS,&TT,&BB,realContext);

        realMultiply(SIGMA_N,SIGMA_lnYonX,&TT,realContext);
        realMultiply(SIGMA_1onX,SIGMA_lnY,&UU,realContext);
        realSubtract(&TT,&UU,&TT,realContext);
        realMultiply(&SS,&TT,&CC,realContext);

        //      a0 = exp (B);
        realExp(&BB,&aa0,realContext);

        realToString(&aa0,ss); a0 = strtof (ss, NULL);
        #ifdef PC_BUILD
          printf("§ a0: %f %f\n",a0, exp (B) );
        #endif //PC_BUILD

        //      a1 = exp (C);
        realExp(&CC,&aa1,realContext);

        realToString(&aa1,ss); a1 = strtof (ss, NULL);
        #ifdef PC_BUILD
          printf("§ a1: %f %f\n",a1,  exp (C) );
        #endif //PC_BUILD

        //     r = sqrt ( (B * sumlny + C * sumlnyonx - 1.0/nn * sumlny * sumlny) / (sumln2y - 1.0/nn * sumlny * sumlny) ); //(rROOT)
        realMultiply(&BB,SIGMA_lnY,&SS,realContext);
        realMultiply(&CC,SIGMA_lnYonX,&TT,realContext);
        realAdd     (&SS,&TT,&SS,realContext); //t1+t2
        realDivide  (const_1,SIGMA_N,&TT,realContext);
        realMultiply(&TT,SIGMA_lnY,&TT,realContext);
        realMultiply(&TT,SIGMA_lnY,&TT,realContext); //t3
        realSubtract(&SS,&TT,&SS,realContext); //t1+t2-t3
        realSubtract(SIGMA_ln2Y,&TT,&TT,realContext);
        realDivide  (&SS,&TT,&RR_,realContext);

        realToString(&RR_, ss); r = strtof (ss, NULL);
        #ifdef PC_BUILD
          printf("§ r: %f %f\n",r, sqrt ( (B * sumlny + C * sumlnyonx - 1.0/nn * sumlny * sumlny) / (sumln2y - 1.0/nn * sumlny * sumlny) )); //(rEXP));
        #endif //PC_BUILD

        #ifdef STATDEBUG
          printf("##### ROOTF %i a0=%f a1=%f \n",(int)nn, a0, a1);
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
        realDivide  (&SS,&TT,&aa0,realContext);

        realToString(&aa0,ss); a0 = strtof (ss, NULL);
        #ifdef PC_BUILD
          printf("§ a0: %f %f\n",a0,(sumx2 * sum1ony - sumx * sumxony) / (nn*sumx2 - sumx * sumx));
        #endif //PC_BUILD

        //     a1 = (nn * sumxony - sumx * sum1ony) / (nn * sumx2 - sumx * sumx);
        realMultiply(SIGMA_N, SIGMA_XonY, &SS, realContext);
        realMultiply(SIGMA_X, SIGMA_1onY, &TT, realContext);
        realSubtract(&SS,&TT,&SS,realContext);
        realMultiply(SIGMA_N,SIGMA_X2,&TT,realContext);
        realMultiply(SIGMA_X, SIGMA_X, &UU, realContext);
        realSubtract(&TT,&UU,&TT,realContext);
        realDivide  (&SS,&TT,&aa1,realContext);

        realToString(&aa1,ss); a1 = strtof (ss, NULL);
        #ifdef PC_BUILD
          printf("§ a1: %f %f\n",a1,(nn * sumxony - sumx * sum1ony) / (nn * sumx2 - sumx * sumx));
        #endif //PC_BUILD

        //     r = sqrt ( (a0 * sum1ony + a1 * sumxony - 1.0/nn * sum1ony * sum1ony) / (sum1ony2 - 1.0/nn * sum1ony * sum1ony ) ); //(rHYP)
        realMultiply(&aa0, SIGMA_1onY, &SS, realContext);
        realMultiply(&aa1, SIGMA_XonY, &TT, realContext);
        realAdd     (&SS,&TT,&SS,realContext);   //ss=t1+t2
        realDivide  (const_1,SIGMA_N,&TT,realContext);
        realMultiply(&TT, SIGMA_1onY, &TT, realContext);
        realMultiply(&TT, SIGMA_1onY, &TT, realContext); //tt=t3
        realSubtract(&SS,&TT,&SS,realContext);   //t1+t2-t3
        realSubtract(SIGMA_1onY2,&TT,&UU,realContext); //use t3
        realDivide  (&SS,&UU,&SS,realContext);
        realSquareRoot(&SS,&RR_,realContext);

        realToString(&RR_,ss); r = strtof (ss, NULL);
        #ifdef PC_BUILD
          printf("§ r: %f %f\n",r,sqrt ( (a0 * sum1ony + a1 * sumxony - 1.0/nn * sum1ony * sum1ony) / (sum1ony2 - 1.0/nn * sum1ony * sum1ony ))  );
        #endif //PC_BUILD

        #ifdef STATDEBUG
          printf("##### HYPF %i a0=%f a1=%f \n",(int)nn, a0, a1);
        #endif
        break;


      case CF_PARABOLIC_FITTING :
        #ifdef PC_BUILD
          printf("Parabolic fit:\n");
        #endif
        calc_BCD();
        calc_AEFG();

        //      a2 = F; //a2 = (A*B - C*D) / (A*E - C*C) = F. Not in ReM, but the formula is correct and prevents duplicate code.
        realCopy (&FF,&aa2);
        realToString(&aa2,ss); a2 = strtof (ss, NULL);
        #ifdef PC_BUILD
          printf("§ a2: %f %f\n",a2,F);
        #endif //PC_BUILD

        //      a1 = G; //a1 = (D - a2 * C) / A = G; Not in ReM, but the formula is correct and prevents duplicate code.
        realCopy (&GG,&aa1);
        realToString(&aa1,ss); a1 = strtof (ss, NULL);
        #ifdef PC_BUILD
          printf("§ a1: %f %f\n",a1,(D - a2 * C) / A);
        #endif //PC_BUILD

        //      a0 = 1.0/nn * (sumy - a2 * sumx2 - a1 * sumx);
        realMultiply(&FF, SIGMA_X2, &TT, realContext);
        realSubtract(SIGMA_Y , &TT, &HH, realContext);
        realMultiply(&GG, SIGMA_X, &TT, realContext);
        realSubtract(&HH, &TT, &HH, realContext);
        realDivide  (&HH,SIGMA_N,&aa0,realContext);
        realToString(&aa0,ss); a0 = strtof (ss, NULL);
        #ifdef PC_BUILD
          printf("§ a0: %f %f\n",a0,1.0/nn * (sumy - a2 * sumx2 - a1 * sumx));
        #endif //PC_BUILD

        //      r = sqrt( (a0 * sumy + a1 * sumxy + a2 * sumx2y - 1.0/nn * sumy * sumy) / (sumy2 - 1.0/nn * sumy * sumy) );
        realMultiply(&aa0, SIGMA_Y, &SS, realContext);
        realMultiply(&aa1, SIGMA_XY, &TT, realContext);
        realAdd     (&SS, &TT,&SS,realContext);
        realMultiply(&aa2, SIGMA_X2Y, &TT, realContext);
        realAdd     (&SS, &TT,&SS,realContext); //interim in SS
        realMultiply(SIGMA_Y, SIGMA_Y, &TT, realContext);
        realDivide  (&TT, SIGMA_N,&UU,realContext);
        realSubtract(&SS, &UU,&SS,realContext); //top in SS
        realSubtract(SIGMA_Y2, &UU,&TT,realContext);
        realDivide  (&SS, &TT,&SS,realContext); //R^2
        realSquareRoot(&SS, &SS, realContext);
        realToString(&SS, ss); r = strtof (ss, NULL);
        #ifdef PC_BUILD
          printf("§ r^2: %f r: %f %f\n",r*r, r,sqrt( (a0 * sumy + a1 * sumxy + a2 * sumx2y - 1.0/nn * sumy * sumy) / (sumy2 - 1.0/nn * sumy * sumy) ) );
        #endif //PC_BUILD
        #ifdef STATDEBUG
          printf("##### PARABF %i a0=%f a1=%f a2=%f\n",(int)nn, a0, a1, a2);
        #endif
        break;


      case CF_GAUSS_FITTING :
        #ifdef PC_BUILD
          printf("Gauss fit:\n");
        #endif //PC_BUILD
        calc_BCD();
        calc_AEFG();

        //        H = (1.0)/nn * (sumlny - F * sumx2 - G * sumx);
        realMultiply(&FF, SIGMA_X2, &TT, realContext);
        realSubtract(SIGMA_lnY, &TT, &HH, realContext);
        realMultiply(&GG, SIGMA_X, &TT, realContext);
        realSubtract(&HH, &TT, &HH, realContext);
        realDivide  (&HH,SIGMA_N,&HH,realContext);
        realToString(&HH,ss); H = strtof (ss, NULL);
        #ifdef PC_BUILD
          printf("§ H: %f %f\n",H,(1.0)/nn * (sumlny - F * sumx2 - G * sumx));
        #endif //PC_BUILD

        //        a2 = (1.0)/F;
        realDivide  (const_1,&FF,&aa2,realContext);
        realToString(&aa2,ss); a2 = strtof (ss, NULL);
        #ifdef PC_BUILD
          printf("§ a2: %f %f\n",a2,(1.0)/F);
        #endif //PC_BUILD

        //        a1 = -G/(2.0) * a2;
        realDivide  (&GG,const__1,&TT,realContext);
        realDivide  (&TT,const_2 ,&TT,realContext);
        realMultiply(&TT, &aa2, &aa1, realContext);
        realToString(&aa1,ss); a1 = strtof (ss, NULL);
        #ifdef PC_BUILD
          printf("§ a1: %f %f\n",a1,-G/(2.0) * a2);
        #endif //PC_BUILD

        //GAUSS  a0 = exp (H - F * a1 * a1); //maxy;
        realMultiply(&aa1,&aa1,&SS,realContext);
        realMultiply(&SS,&FF,&SS,realContext);
        realSubtract(&HH,&SS,&aa0,realContext);
        realExp(&aa0,&aa0,realContext);           
        realToString(&aa0,ss); a0 = strtof (ss, NULL);
        #ifdef PC_BUILD
          printf("§ a0: %f %f\n",a0, exp (H - F * a1 * a1));
        #endif //PC_BUILD

        #ifdef QUICK_CHECK_MAX_OVERRIDE
                //        a0 = maxy;  // exp (H - F * a1 * a1); //maxy;
                realCopy    (SIGMA_YMAX,&aa0);
                realToString(&aa0,ss); a0 = strtof (ss, NULL);
                printf("§ a0: %f %f\n",a0,maxy);
        #endif //AAA

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
        realDivide  (&SS, &TT,&SS,realContext); //R^2
        realSquareRoot(&SS, &SS, realContext);
        realToString(&SS, ss); r = strtof (ss, NULL);
        #ifdef PC_BUILD
          printf("§ r^2: %f r: %f %f\n",r*r, r,sqrt ( ( H * sumlny + G * sumxlny + F * sumx2lny - 1.0/nn * sumlny * sumlny ) / (sumln2y - 1.0/nn * sumlny * sumlny) ));
        #endif //PC_BUILD        
        #ifdef STATDEBUG
          printf("A%f B%f C%f D%f E%f F%f G%f H%f   a0:%f a1:%f a2:%f r:%f r^2:%f\n",A,B,C,D,E,F,G,H,a0,a1,a2,r,r*r);
        #endif
        #ifdef STATDEBUG
          printf("##### GAUSSF %i a0=%f a1=%f a2=%f\n",(int)nn, a0, a1, a2);
        #endif
        break;


      case CF_CAUCHY_FITTING :
        #ifdef PC_BUILD
          printf("Cauchy fit:\n");
        #endif //PC_BUILD

        //      B = nn * sumx2ony - sumx2 * sum1ony;        
        realMultiply(SIGMA_N, SIGMA_X2onY, &SS, realContext);
        realMultiply(SIGMA_X2, SIGMA_1onY, &TT, realContext);
        realSubtract(&SS, &TT, &BB, realContext);
        realToString(&BB,ss); B = strtof (ss, NULL);
        #ifdef PC_BUILD
          printf("§ B: %f %f\n",B,nn * sumx2ony - sumx2 * sum1ony);
        #endif //PC_BUILD

        //      C = nn * sumx3 - sumx * sumx2;                     //vv C copied from PARABF. Exactly the same
        realMultiply(SIGMA_N, SIGMA_X3, &SS, realContext);
        realMultiply(SIGMA_X2, SIGMA_X, &TT, realContext);
        realSubtract(&SS, &TT, &CC, realContext);
        realToString(&CC,ss); C = strtof (ss, NULL);
        #ifdef PC_BUILD
          printf("§ C: %f %f\n",C,nn * sumx3 - sumx * sumx2);
        #endif //PC_BUILD                                          //^^

        //      D = nn * sumxony - sumx * sum1ony;
        realMultiply(SIGMA_N, SIGMA_XonY, &SS, realContext);
        realMultiply(SIGMA_X, SIGMA_1onY, &TT, realContext);
        realSubtract(&SS, &TT, &DD, realContext);
        realToString(&DD,ss); D = strtof (ss, NULL);
        #ifdef PC_BUILD
          printf("§ D: %f %f\n",D,nn * sumxony - sumx * sum1ony);
        #endif //PC_BUILD

        calc_AEFG();

        //    H = 1.0/nn * (sum1ony - R12 * sumx - R13 * sumx2);
        //old GAUSS CHANGED TO 1/y ; F and G left:      H = (1.0)/nn * (sum1ony - F * sumx2 - G * sumx);
        realMultiply(&FF, SIGMA_X2, &TT, realContext);
        realSubtract(SIGMA_1onY , &TT, &HH, realContext);
        realMultiply(&GG, SIGMA_X, &TT, realContext);
        realSubtract(&HH, &TT, &HH, realContext);
        realDivide  (&HH,SIGMA_N,&HH,realContext);
        realToString(&HH,ss); H = strtof (ss, NULL);
        #ifdef PC_BUILD
          printf("§ H: %f %f\n",H,(1.0)/nn * (sumlny - F * sumx2 - G * sumx));
        #endif //PC_BUILD

        //        a0 = F;
        realCopy    (&FF,&aa0);
        realToString(&aa0,ss); a0 = strtof (ss, NULL);
        #ifdef PC_BUILD
          printf("§ a0: %f %f\n",a0,F);
        #endif //PC_BUILD

        //        a1 = G/2.0 * a0;
        realDivide  (&GG,const_2,&TT,realContext);
        realMultiply(&TT, &aa0, &aa1, realContext);
        realToString(&aa1,ss); a1 = strtof (ss, NULL);
        #ifdef PC_BUILD
          printf("§ a1: %f %f\n",a1,G/2.0 * a0);
        #endif //PC_BUILD

        //      a2 = H - F * a1 * a1;
        realMultiply(&aa1,&aa1,&SS,realContext);
        realMultiply(&SS,&FF,&SS,realContext);
        realSubtract(&HH,&SS,&aa2,realContext);
        realToString(&aa0,ss); a2 = strtof (ss, NULL);
        #ifdef PC_BUILD
          printf("§ a2: %f %f\n",a2, H - F * a1 * a1);
        #endif //PC_BUILD

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
        realDivide  (&SS, &TT,&SS,realContext); //R^2
        realSquareRoot(&SS, &SS, realContext);
        realToString(&SS, ss); r = strtof (ss, NULL);
        #ifdef PC_BUILD
          printf("§ r: %f %f\n",r, sqrt ( (H * sum1ony + G * sumxony + F * sumx2ony - 1.0/nn * sum1ony * sum1ony) / (sum1ony2 - 1.0/nn * sum1ony * sum1ony) ));
        #endif //PC_BUILD

        #ifdef STATDEBUG
          printf("##### CAUCHY %i a0=%f a1=%f a2=%f\n",(int)nn, a0, a1, a2);
        #endif
        break;


      case CF_ORTHOGONAL_FITTING :
      case CF_ORTHOGONAL_FITTING+10000 :
        // a1a = 1.0 / (2.0 * sxy) * ( sy * sy - sx * sx + sqrt( (sy * sy - sx * sx) * (sy * sy - sx * sx) + 4 * sxy * sxy) );
        // a0a  = y_ - a1a * x_;
        // a1b = 1.0 / (2.0 * sxy) * ( sy * sy - sx * sx - sqrt( (sy * sy - sx * sx) * (sy * sy - sx * sx) + 4 * sxy * sxy) );
        // a0b = y_ - a1b * x_;
        realMultiply(&S_Y,&S_Y,&SS,realContext);
        realMultiply(&S_X,&S_X,&TT,realContext);
        realSubtract(&SS,&TT,&UU,realContext);    //keep  uu = sy2-sx2
        realMultiply(&UU,&UU,&VV,realContext);
        realMultiply(&S_XY,&S_XY,&WW,realContext);
        realMultiply(&VV,const_2,&WW,realContext);
        realMultiply(&VV,const_2,&WW,realContext);
        realAdd     (&WW,&VV,&VV,realContext);
        realSquareRoot(&VV,&VV,realContext);  //sqrt term

        realDivide(&UU,const_2,&UU,realContext);  //term1
        realDivide(&UU,&S_XY,&UU,realContext);

        realDivide(&VV,const_2,&VV,realContext);  //term2
        realDivide(&VV,&S_XY,&VV,realContext);

        if(selection == CF_ORTHOGONAL_FITTING) {
          realAdd     (&UU,&VV,&aa1,realContext);   //a1
          realMultiply(&aa1,&M_X,&SS,realContext);
          realSubtract(&M_Y,&SS,&aa0,realContext);  //a0
          realToString(&aa1, ss); a1a = strtof (ss, NULL); a1 = a1a;
          realToString(&aa0, ss); a0a = strtof (ss, NULL); a0 = a0a;
          #ifdef PC_BUILD
            printf("§ a1: %f %f\n",a1,1.0 / (2.0 * sxy) * ( sy * sy - sx * sx + sqrt( (sy * sy - sx * sx) * (sy * sy - sx * sx) + 4 * sxy * sxy) ));
            printf("§ a0: %f %f\n",a0,y_ - a1a * x_);
          #endif //PC_BUILD
        }
        if(selection == CF_ORTHOGONAL_FITTING + 10000) {
          realSubtract(&UU,&VV,&aa1,realContext);   //a1
          realToString(&aa1, ss); a1b = strtof (ss, NULL);
          realSubtract(&M_Y,&SS,&aa0,realContext);  //a0
          realToString(&aa1, ss); a1b = strtof (ss, NULL); a1 = a1b;
          realToString(&aa0, ss); a0b = strtof (ss, NULL); a0 = a0b;
          #ifdef PC_BUILD
            printf("§ a1: %f %f\n",a1,1.0 / (2.0 * sxy) * ( sy * sy - sx * sx - sqrt( (sy * sy - sx * sx) * (sy * sy - sx * sx) + 4 * sxy * sxy) ));
            printf("§ a0: %f %f\n",a0,y_ - a1b * x_);
          #endif //PC_BUILD
        }

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

        realDivide(&SS,&TT,&RR_,realContext);
        realDivide(&RR_,&UU,&RR_,realContext);            //r

        realToString(&RR_, ss); r = strtof (ss, NULL);
        #ifdef PC_BUILD
          printf("§ r: %f %f\n",r, (nn * sumxy - sumx * sumy) / (sqrt (nn * sumx2 - sumx * sumx) * sqrt(nn * sumy2 - sumy * sumy) ));
        #endif //PC_BUILD
        #ifdef STATDEBUG
          printf("##### ORTHOF %i a0=%f a1=%f smi=%f\n",(int)nn, a0b, a1b, smi);
        #endif
        break;


      default: break;
    }
  }


