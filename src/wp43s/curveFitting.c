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

    if(curveFitting & CF_LINEAR_FITTING)      printf("linear ");
    if(curveFitting & CF_EXPONENTIAL_FITTING) printf("exponential ");
    if(curveFitting & CF_LOGARITHMIC_FITTING) printf("logarithmic ");
    if(curveFitting & CF_POWER_FITTING)       printf("power ");
    if(curveFitting & CF_ROOT_FITTING)        printf("root ");
    if(curveFitting & CF_HYPERBOLIC_FITTING)  printf("hyperbolic ");
    if(curveFitting & CF_PARABOLIC_FITTING)   printf("parabolic ");
    if(curveFitting & CF_CAUCHY_FITTING)      printf("Cauchy peak ");
    if(curveFitting & CF_GAUSS_FITTING)       printf("Gauss peak ");
    if(curveFitting & CF_ORTHOGONAL_FITTING)  printf("Orthogonal ");

    if(numberOfOnes == 1) {
      printf("fitting model.\n");
    }
    else {
      printf("\nfitting models.\n");
    }
  #endif // PC_BUILD
}



void calc_BCD(void){                        //Aux terms, must be run before calc_AEFG
char ss[100];
  //        B = nn * sumx2y - sumx2 * sumy;
  int32ToReal(nn, &SS);
  realMultiply(&SS, SIGMA_X2Y, &SS, realContext);
  realMultiply(SIGMA_X2, SIGMA_Y, &TT, realContext);
  realSubtract(&SS, &TT, &BB, realContext);
  realToString(&BB,ss); B = strtof (ss, NULL);
  printf("§ B: %f %f\n",B,nn * sumx2y - sumx2 * sumy);

  //        C = nn * sumx3 - sumx2 * sumx;
  int32ToReal(nn, &SS);
  realMultiply(&SS, SIGMA_X3, &SS, realContext);
  realMultiply(SIGMA_X2, SIGMA_X, &TT, realContext);
  realSubtract(&SS, &TT, &CC, realContext);
  realToString(&CC,ss); C = strtof (ss, NULL);
  printf("§ C: %f %f\n",C,nn * sumx3 - sumx2 * sumx);

  //        D = nn * sumxy - sumx * sumy;
  int32ToReal(nn, &SS);
  realMultiply(&SS, SIGMA_XY, &SS, realContext);
  realMultiply(SIGMA_X, SIGMA_Y, &TT, realContext);
  realSubtract(&SS, &TT, &DD, realContext);
  realToString(&DD,ss); D = strtof (ss, NULL);
  printf("§ D: %f %f\n",D,nn * sumxy - sumx * sumy);
}



void calc_AEFG(void){                        //Aux terms, must be run after calc_BCD
char ss[100];
  //        A = nn * sumx2 - sumx * sumx;
  int32ToReal(nn, &SS);
  realMultiply(&SS, SIGMA_X2, &SS, realContext);
  realMultiply(SIGMA_X, SIGMA_X, &TT, realContext);
  realSubtract(&SS, &TT, &AA, realContext);
  realToString(&AA,ss); A = strtof (ss, NULL);
  printf("§ A: %f %f\n",A,nn * sumx2 - sumx * sumx);

  //        E = nn * sumx4 - sumx2 * sumx2;
  int32ToReal(nn, &SS);
  realMultiply(&SS, SIGMA_X4, &SS, realContext);
  realMultiply(SIGMA_X2, SIGMA_X2, &TT, realContext);
  realSubtract(&SS, &TT, &EE, realContext);
  realToString(&EE,ss); E = strtof (ss, NULL);
  printf("§ E: %f %f\n",E,nn * sumx4 - sumx2 * sumx2);

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
  printf("§ F: %f %f\n",F,(A*B - C*D) / (A*E - C*C));

  //        G = (D - F * C) / A;
  realMultiply(&FF, &CC, &SS, realContext);
  realSubtract(&DD, &SS, &SS, realContext);
  realDivide  (&SS,&AA,&GG,realContext);
  realToString(&GG,ss); G = strtof (ss, NULL);
  printf("§ G: %f %f\n",G,(D - F * C) / A);
}



void processCurvefitSelection(){
    char ss[100];
    double a1a, a1b, a0a, a0b;
    switch(selection) {
      case CF_LINEAR_FITTING :
        a0 = (sumx2 * sumy  - sumx * sumxy) / (nn * sumx2 - sumx * sumx);
//TODO REAL CONVERSION
  sprintf(ss,"%f",a0); stringToReal(ss,&aa0,realContext);
        a1 = (nn  * sumxy - sumx * sumy ) / (nn * sumx2 - sumx * sumx);
//TODO REAL CONVERSION
  sprintf(ss,"%f",a1); stringToReal(ss,&aa1,realContext);
        r  = (nn * sumxy - sumx*sumy) / (sqrt(nn*sumx2-sumx*sumx) * sqrt(nn*sumy2-sumy*sumy));
        smi = sqrt(sx*sx*sy*sy*(1-r*r)/(sx*sx+r*r*sy*sy)); 
        #ifdef STATDEBUG
          printf("##### Linear %i a0=%f a1=%f \n",(int)nn, a0, a1);
        #endif
        break;

      case CF_EXPONENTIAL_FITTING :
        a0 = exp( (sumx2 * sumlny  - sumx * sumxlny) / (nn * sumx2 - sumx * sumx) );
//TODO REAL CONVERSION
  sprintf(ss,"%f",a0); stringToReal(ss,&aa0,realContext);
        a1 = (nn  * sumxlny - sumx * sumlny ) / (nn * sumx2 - sumx * sumx);
//TODO REAL CONVERSION
  sprintf(ss,"%f",a1); stringToReal(ss,&aa1,realContext);
        r = (nn * sumxlny - sumx*sumlny) / (sqrt(nn*sumx2-sumx*sumx) * sqrt(nn*sumln2y-sumlny*sumlny)); //(rEXP)
        smi = sqrt(sx*sx*sy*sy*(1-r*r)/(sx*sx+r*r*sy*sy)); 
        #ifdef STATDEBUG
          printf("##### EXPF %i a0=%f a1=%f \n",(int)nn, a0, a1);
        #endif
        break;

      case CF_LOGARITHMIC_FITTING :
        a0 = (sumln2x * sumy  - sumlnx * sumylnx) / (nn * sumln2x - sumlnx * sumlnx);
//TODO REAL CONVERSION
  sprintf(ss,"%f",a0); stringToReal(ss,&aa0,realContext);
        a1 = (nn  * sumylnx - sumlnx * sumy ) / (nn * sumln2x - sumlnx * sumlnx);
//TODO REAL CONVERSION
  sprintf(ss,"%f",a1); stringToReal(ss,&aa1,realContext);
        r = (nn * sumylnx - sumlnx*sumy) / (sqrt(nn*sumln2x-sumlnx*sumlnx) * sqrt(nn*sumy2-sumy*sumy)); //(rLOG)
        smi = sqrt(sx*sx*sy*sy*(1-r*r)/(sx*sx+r*r*sy*sy)); 
        #ifdef STATDEBUG
          printf("##### LOGF %i a0=%f a1=%f \n",(int)nn, a0, a1);
        #endif
        break;

      case CF_POWER_FITTING :
        a0 = exp( (sumln2x * sumlny  - sumlnx * sumlnxlny) / (nn * sumln2x - sumlnx * sumlnx) );
//TODO REAL CONVERSION
  sprintf(ss,"%f",a0); stringToReal(ss,&aa0,realContext);
        a1 = (nn  * sumlnxlny - sumlnx * sumlny ) / (nn * sumln2x - sumlnx * sumlnx);
//TODO REAL CONVERSION
  sprintf(ss,"%f",a1); stringToReal(ss,&aa1,realContext);
        r = (nn * sumlnxlny - sumlnx*sumlny) / (sqrt(nn*sumln2x-sumlnx*sumlnx) * sqrt(nn*sumln2y-sumlny*sumlny)); //(rPOW)
        #ifdef STATDEBUG
          printf("##### POWERF %i a0=%f a1=%f \n",nn, a0, a1);
        #endif
        break;

      case CF_ROOT_FITTING :
        A = nn * sum1onx2 - sum1onx * sum1onx;
        B = 1.0/A * (sum1onx2 * sumlny - sum1onx * sumlnyonx);
        C = 1.0/A * (nn * sumlnyonx - sum1onx * sumlny);

        a0 = exp (B);
//TODO REAL CONVERSION
  sprintf(ss,"%f",a0); stringToReal(ss,&aa0,realContext);
        a1 = exp (C);
//TODO REAL CONVERSION
  sprintf(ss,"%f",a1); stringToReal(ss,&aa1,realContext);
        r = sqrt ( (B * sumlny + C * sumlnyonx - 1.0/nn * sumlny * sumlny) / (sumlny*sumlny - 1.0/nn * sumlny * sumlny) ); //(rROOT)
        #ifdef STATDEBUG
          printf("##### ROOTF %i a0=%f a1=%f \n",(int)nn, a0, a1);
        #endif
        break;

      case CF_HYPERBOLIC_FITTING :
        a0 = (sumx2 * sum1ony - sumx * sumxony) / (nn*sumx2 - sumx * sumx);
//TODO REAL CONVERSION
  sprintf(ss,"%f",a0); stringToReal(ss,&aa0,realContext);
        a1 = (nn * sumxony - sumx * sum1ony) / (nn * sumx2 - sumx * sumx);
//TODO REAL CONVERSION
  sprintf(ss,"%f",a1); stringToReal(ss,&aa1,realContext);
        r = sqrt ( (a0 * sum1ony + a1 * sumxony - 1.0/nn * sum1ony * sum1ony) / (sum1ony2 - 1.0/nn * sum1ony * sum1ony ) ); //(rHYP)
        #ifdef STATDEBUG
          printf("##### HYPF %i a0=%f a1=%f \n",(int)nn, a0, a1);
        #endif
        break;

      case CF_PARABOLIC_FITTING :
        printf("Parabolic fit:\n");
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
        int32ToReal (nn, &SS);
        realDivide  (&HH,&SS,&aa0,realContext);
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
        int32ToReal (nn,  &UU);
        realDivide  (&TT, &UU,&UU,realContext);
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
        int32ToReal (nn, &SS);
        realDivide  (&HH,&SS,&HH,realContext);
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

        //        r  = sqrt ( ( H * sumlny + G * sumxlny + F * sumx2lny - 1.0/nn * sumlny * sumlny ) / (sumlny * sumlny - 1.0/nn * sumlny * sumlny) );
        realMultiply(&HH, SIGMA_lnY, &SS, realContext);
        realMultiply(&GG, SIGMA_XlnY, &TT, realContext);
        realAdd     (&SS, &TT,&SS,realContext);
        realMultiply(&FF, SIGMA_X2lnY, &TT, realContext);
        realAdd     (&SS, &TT,&SS,realContext); //interim in SS
        realMultiply(SIGMA_lnY, SIGMA_lnY, &TT, realContext);
        int32ToReal (nn,  &UU);
        realDivide  (&TT, &UU,&UU,realContext);
        realSubtract(&SS, &UU,&SS,realContext); //top in SS

        realSubtract(&TT, &UU,&TT,realContext); //use both TT and UU from above
        realDivide  (&SS, &TT,&SS,realContext); //R^2
        realSquareRoot(&SS, &SS, realContext);
        realToString(&SS, ss); r = strtof (ss, NULL);
        #ifdef PC_BUILD
          printf("§ r^2: %f r: %f %f\n",r*r, r,sqrt ( ( H * sumlny + G * sumxlny + F * sumx2lny - 1.0/nn * sumlny * sumlny ) / (sumlny * sumlny - 1.0/nn * sumlny * sumlny) ));
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

//Unknown constants.
// R12 Assuming G
// R13 Assuming F

    //    r  = sqrt ( (H * sum1ony + G * sumxony + F * sumx2ony - 1.0/nn * sum1ony * sum1ony) / (sum1ony2 - 1.0/nn * sum1ony * sum1ony) );

        
        //      B = nn * sumx2ony - sumx2 * sum1ony;        
        int32ToReal(nn, &SS);
        realMultiply(&SS, SIGMA_X2onY, &SS, realContext);
        realMultiply(SIGMA_X2, SIGMA_1onY, &TT, realContext);
        realSubtract(&SS, &TT, &BB, realContext);
        realToString(&BB,ss); B = strtof (ss, NULL);
        #ifdef PC_BUILD
          printf("§ B: %f %f\n",B,nn * sumx2ony - sumx2 * sum1ony);
        #endif //PC_BUILD

        //      C = nn * sumx3 - sumx * sumx2;                     //C copied from PARABF. Exactly the same
        int32ToReal(nn, &SS);
        realMultiply(&SS, SIGMA_X3, &SS, realContext);
        realMultiply(SIGMA_X2, SIGMA_X, &TT, realContext);
        realSubtract(&SS, &TT, &CC, realContext);
        realToString(&CC,ss); C = strtof (ss, NULL);
        #ifdef PC_BUILD
          printf("§ C: %f %f\n",C,nn * sumx3 - sumx * sumx2);
        #endif //PC_BUILD

        //      D = nn * sumxony - sumx * sum1ony;
        int32ToReal(nn, &SS);
        realMultiply(&SS, SIGMA_XonY, &SS, realContext);
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
        int32ToReal (nn, &SS);
        realDivide  (&HH,&SS,&HH,realContext);
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
        #ifdef STATDEBUG
          printf("##### CAUCHY %i a0=%f a1=%f a2=%f\n",(int)nn, a0, a1, a2);
        #endif
        break;


      case CF_ORTHOGONAL_FITTING :
        a1a = 1.0 / (2.0 * sxy) * ( sy * sy - sx * sx + sqrt( (sy * sy - sx * sx) * (sy * sy - sx * sx) + 4 * sxy * sxy) );
        a0a  = y_ - a1a * x_;
        //r = sxy / (sx * sy);
        r  = (nn * sumxy - sumx * sumy) / (sqrt (nn * sumx2 - sumx * sumx) * sqrt(nn * sumy2 - sumy * sumy) );
        a1 = a1a;
  sprintf(ss,"%f",a1); stringToReal(ss,&aa1,realContext);
//TODO REAL CONVERSION
        a0 = a0a;
  sprintf(ss,"%f",a0); stringToReal(ss,&aa0,realContext);
//TODO REAL CONVERSION
        #ifdef STATDEBUG
          printf("##### ORTHOF %i a0=%f a1=%f smi=%f\n",(int)nn, a0a, a1a, smi);
        #endif
        break;

      case CF_ORTHOGONAL_FITTING+10000 :
        a1b = 1.0 / (2.0 * sxy) * ( sy * sy - sx * sx - sqrt( (sy * sy - sx * sx) * (sy * sy - sx * sx) + 4 * sxy * sxy) );
        a0b  = y_ - a1b * x_;
        //r = sxy / (sx * sy);
        r  = (nn * sumxy - sumx * sumy) / (sqrt( nn * sumx2 - sumx * sumx) * sqrt( nn * sumy2 - sumy * sumy));
        a1 = a1b;
  sprintf(ss,"%f",a1); stringToReal(ss,&aa1,realContext);
//TODO REAL CONVERSION
        a0 = a0b;
  sprintf(ss,"%f",a0); stringToReal(ss,&aa0,realContext);
//TODO REAL CONVERSION
        #ifdef STATDEBUG
          printf("##### ORTHOF %i a0=%f a1=%f smi=%f\n",(int)nn, a0b, a1b, smi);
        #endif
        break;

      default: break;
    }
  }


