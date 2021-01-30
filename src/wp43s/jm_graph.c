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
 * along with 43S.  If not, see <htempp://www.gnu.org/licenses/>.
 */

/********************************************//** //JM
 * \file jmgraph.c Graphing module
 ***********************************************/

/* ADDITIONAL C43 functions and routines */


#include "wp43s.h"
#include "math.h"
#include "graphs.h"





void Fn_Lbl_A(void) {                                   //Temporary RPN function
#ifndef TESTSUITE_BUILD

    fnStore(99);             // STO 99
    fnStoreMin(98);
    fnRCL(99);
    fnRCL(98);
    fnSubtract(0);
    fnStrInputReal34("0.3");
    fnAdd(0);
    fnStore(99);

    // (sin(x)/x + sin(10x)/5) / 2 + 2/x 
    fnSin(0);                // SIN
    fnRCL(99);               // RCL 99
    fnDivide(0);             // /
 
    fnRCL(99);               // RCL 99
    fnStrInputReal34("10");  // 10
    fnMultiply(0);           // *
    fnSin(0);                // SIN
    fnStrInputReal34("5");   // 5
    fnDivide(0);             // /
 
    fnAdd(0);                // +    

    fnStrInputReal34("2");   // 2
    fnDivide(0);             // /

    fnStrInputReal34("2");   // 2
    fnRCL(99);               // RCL 99
    fnDivide(0);             // /
    fnAdd(0);                // +    
    fnRCL(99);               //leaving y in Y and x in X
#endif
}



void Fn_Lbl_B(void) {                                   //Temporary RPN function
#ifndef TESTSUITE_BUILD
    fnStore(99);             // STO 99   //sin(x) + sin(x^2) = 0
    fnSin(0);                // SIN    
    fnRCL(99);
    fnSquare(0);             // square
    fnSin(0);                // SIN
    fnAdd(0);                // +    


//fnStrInputReal34("1");         //cube root
//fnStrInputReal34("3");
//fnDivide(0);
//fnPower(0);
//
//

    fnRCL(99);               //leaving y in Y and x in X
#endif
}


void Fn_Lbl_C(void) {                                   //Temporary RPN function
#ifndef TESTSUITE_BUILD
    fnStore(99);             // STO 99

//Original DEMO3
//    runFunction(ITM_sinc);

    fnSquare(0);                   //x^2 + 1 = 0
    fnStrInputReal34("-1");
    fnSubtract(0);


    fnRCL(99);               //leaving y in Y and x in X
#endif
}







//FOR STAT GRANH. LEAVES ANSWER IN X & Y
void Fn_Lbl_D(void) {                                   //Temporary RPN function 0.1x 
#ifndef TESTSUITE_BUILD
    fnStore(99);             // STO 99                   ( 0.1x^2 + 0.15x + (rand-0.5) * 0.4 ) + 2.5
    runFunction(ITM_ENTER);  // ENTER          // /
    fnStrInputReal34("0.1"); // 0.1
    runFunction(ITM_MULT);   // *              // /
    runFunction(ITM_MULT);   // *              // /
    fnRCL(99);               // RCL 99
    fnStrInputReal34("0.15"); // 0.15
    runFunction(ITM_MULT);    // *             // /
    runFunction(ITM_ADD);     // +             // /

    runFunction(ITM_RAN);     // fnRandom(0);  //    
    fnStrInputReal34("-0.5"); // -0.5
    runFunction(ITM_ADD);     // +             // /

    fnStrInputReal34("0.4");  // 0.4
    runFunction(ITM_MULT);    // *             // /

    runFunction(ITM_ADD);     // +             // /
    fnStrInputReal34("2.5");  // 2.5
    runFunction(ITM_ADD);     // +             // /
    fnRCL(99);
#endif
}


void Fn_Lbl_E(void) {                                   //Temporary RPN function
#ifndef TESTSUITE_BUILD
    fnStore(99);             // STO 99
    fnStrInputReal34("0.6"); // 0.2
    runFunction(ITM_MULT);   // *             // /

    runFunction(ITM_RAN);    // fnRandom(0);                //    
    fnStrInputReal34("-0.5"); // -0.5
    runFunction(ITM_ADD);    // +             // /

    fnStrInputReal34("0.4"); // 0.4
    runFunction(ITM_MULT);   // *             // /

    runFunction(ITM_ADD);    // +             // /
    fnRCL(99);               //leaving y in Y and x in X
#endif
}


void Fn_Lbl_F(void) {                                   //Temporary RPN function
#ifndef TESTSUITE_BUILD
    fnStrInputReal34("-1"); // -1
    runFunction(ITM_ADD);    // +             // /

    fnStore(99);             // STO 99
    fnStrInputReal34("12"); // 12
    runFunction(ITM_MULT);   // *             // /

    runFunction(ITM_sin);    // fnRandom(0);                //    
    fnRCL(99);               //leaving y in Y and x in X
    fnStrInputReal34("12"); // 12
    runFunction(ITM_MULT);   // *             // /
    runFunction(ITM_cos);    // fnRandom(0);                //    
#endif
}



void execute_rpn_function(int16_t nbr){
  #ifdef STATDEBUG
  printf("nbr=%d   x=%s \n",nbr,tmpString);
  #endif

  if(nbr == 1) {
    Fn_Lbl_A();
  } else
  if(nbr == 2) {
    Fn_Lbl_B();
  } else
  if(nbr == 3) {
    Fn_Lbl_C();
  } else
  if(nbr == 4) {
    Fn_Lbl_D();
  } else
  if(nbr == 5) {
    Fn_Lbl_E();
  } else
  if(nbr == 6) {
    Fn_Lbl_F();
  }
}






//###################################################################################
//GRAPH
void graph_demo(uint8_t nbr, float x_min, float x_max) {
  #ifndef TESTSUITE_BUILD
  double x; 
  cancelFilename = true;
  fnClearStack(0);

  if(telltale != MEM_INITIALIZED) {
    graph_setupmemory();
    runFunction(ITM_CLSIGMA);
  }

  fnAngularMode(AM_RADIAN);

  //  for(x=x_min; x<=x_max; x+=(x_max-x_min)/SCREEN_WIDTH_GRAPH) {
  for(x=x_min; x<=x_max; x+=0.99999*(x_max-x_min)/SCREEN_WIDTH_GRAPH*10) {    //Reduced the amount of sample data from 400 points to 40 points

    //convert double to X register
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
    snprintf(tmpString, TMP_STR_LENGTH, "%.16e", x);
    stringToReal34(tmpString, REGISTER_REAL34_DATA(REGISTER_X));

    //leaving y in Y and x in X
    execute_rpn_function(nbr);
    fnSigma(1);
  }
  fnClearStack(0);
  runFunction(ITM_SIGMAx);
  runFunction(ITM_PLOT);
  #endif
}




void printf_cpx(calcRegister_t regist) {
  if(getRegisterDataType(regist) == dtReal34 || getRegisterDataType(regist) == dtComplex34) {
    real34ToString(REGISTER_REAL34_DATA(regist), tmpString);
    if(strchr(tmpString, '.') == NULL && strchr(tmpString, 'E') == NULL) {
      strcat(tmpString, ".");
    }
    printf("Reg(%d) REAL = %s ",regist, tmpString);
  }
  if(getRegisterDataType(regist) == dtComplex34) {
    real34ToString(REGISTER_IMAG34_DATA(regist), tmpString);
    if(strchr(tmpString, '.') == NULL && strchr(tmpString, 'E') == NULL) {
      strcat(tmpString, ".");
    }
    printf("IMAG = %s ",tmpString);
  }
  if(getRegisterDataType(regist) != dtReal34 && getRegisterDataType(regist) != dtComplex34) printf("Neither real nor complex");
}

void print_stck(){
  printf_cpx(REGISTER_T);printf("\n");
  printf_cpx(REGISTER_Z);printf("\n");
  printf_cpx(REGISTER_Y);printf("\n");
  printf_cpx(REGISTER_X);printf("\n");
}
//###################################################################################



void doubleToXRegisterReal34(double x) {             //Convert from double to X register REAL34
    setSystemFlag(FLAG_ASLIFT);
    liftStack();
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
    snprintf(tmpString, TMP_STR_LENGTH, "%.16e", x);
    stringToReal34(tmpString, REGISTER_REAL34_DATA(REGISTER_X));
}



double convert_to_double(calcRegister_t regist) {    //Convert from X register to double
  double y;
  real_t tmpy;
    real34ToReal(REGISTER_REAL34_DATA(regist), &tmpy);
    realToString(&tmpy, tmpString);
    y = strtof (tmpString, NULL);
    return y;
  }



#define VERBOSE_SOLVER1  // a lot less text
//#define VERBOSE_SOLVER2  // verbose a lot



//###################################################################################
//SOLVER
void graph_solver(uint8_t nbr, float x_min, float x_max) {
  #ifndef TESTSUITE_BUILD
  int16_t ix;
  int16_t oscillations = 0; 
  int16_t convergent = 0; 
  bool_t checkend = false;
  bool_t checkzero = false;
  cancelFilename = true;

  double startval = 1; startval = convert_to_double(REGISTER_X);    //********** X REGISTER IS STARTING POINT

  calcRegister_t SREG_XB1 = 83; fnStore(SREG_XB1);   //83: Bisecting method
  calcRegister_t SREG_XB2 = 84; fnStore(SREG_XB2);   //84: Bisecting method
  fnStore(SREG_XB1);

  fnClearStack(0);
  if(telltale != MEM_INITIALIZED) {
    graph_setupmemory();
    runFunction(ITM_CLSIGMA);
  }
  fnAngularMode(AM_RADIAN);


  // Initialize all temporary registers
  // Registers 85 to 97; 98 and 99 are being used in the DEMO data programs
  doubleToXRegisterReal34(0.0);
  calcRegister_t SREG_Xold = 85; fnStore(SREG_Xold);   //85: x old difference
  calcRegister_t SREG_Yold = 86; fnStore(SREG_Yold);   //86: y old difference
  calcRegister_t SREG_Y0   = 87; fnStore(SREG_Y0);     //87: y0
  calcRegister_t SREG_X0   = 88; fnStore(SREG_X0);     //88: x0    //x0 stored but noever recalled
  calcRegister_t SREG_X1   = 89; fnStore(SREG_X1);     //90: x1
  calcRegister_t SREG_X2   = 90; fnStore(SREG_X2);     //91: x2
  calcRegister_t SREG_Y1   = 91; fnStore(SREG_Y1);     //92: y1
  calcRegister_t SREG_Y2   = 92; fnStore(SREG_Y2);     //93: y2
  calcRegister_t SREG_X2N  = 93; fnStore(SREG_X2N);    //94: temporary new x2
  calcRegister_t SREG_DX   = 94;                       //95: x difference
  calcRegister_t SREG_DY   = 95;                       //96: y difference
  calcRegister_t SREG_F    = 96;                       //97: faxctor
  calcRegister_t SREG_TOL  = 97;                       //98: tolerance
  doubleToXRegisterReal34(1.0);
  fnStore(SREG_F);                                     // factor
  fnStore(SREG_DX);                                    // initial value for difference comparison must be larger than tolerance
  fnStore(SREG_DY);                                    // initial value for difference comparison must be larger than tolerance
  doubleToXRegisterReal34(1E-30); 
  fnStore(SREG_TOL);                                   // tolerance

  doubleToXRegisterReal34(startval);                   // leftmost graph value
  fnStore(SREG_X1);                                       // initial x1   
  execute_rpn_function(nbr);                           // leaving y in y
  copySourceRegisterToDestRegister(REGISTER_Y,SREG_Y1);   // initial y1
  doubleToXRegisterReal34(startval + (x_max-x_min) / SCREEN_WIDTH_GRAPH);  //first auto increment is 1 pixel width on the graph
  fnStore(SREG_X2);                                    // initial x2

  ix = 0;
  int16_t kicker = 0;

  #ifdef VERBOSE_SOLVER2
    printf("INIT:   ix=%d X2=",ix); printf_cpx(SREG_X2); printf(" Y2="); printf("\n");
  #endif //VERBOSE_SOLVER2


//###############################################################################################################
//#################################################### Iteration start ##########################################
  while(ix<400 && !checkend) 
  {
    //assumes X2 is in R91
    //Identify oscillations in real or imag: increment osc flag
    int16_t osc = 0;
    if(real34IsNegative(REGISTER_REAL34_DATA(SREG_DX))) {                 //    If x increment is oscilating
      if(real34IsPositive(REGISTER_REAL34_DATA(SREG_Xold)))                 //
        osc++;                                                              //
    }
    if(real34IsPositive(REGISTER_REAL34_DATA(SREG_DY))) {                 //
      if(real34IsNegative(REGISTER_REAL34_DATA(SREG_Yold)))                 //
        osc++;                                                              //
    }
    if(getRegisterDataType(SREG_DX) == dtComplex34 && getRegisterDataType(SREG_Xold) == dtComplex34) {
      if(real34IsNegative(REGISTER_IMAG34_DATA(SREG_DX))) {               //  If x increment is oscilating
        if(real34IsPositive(REGISTER_IMAG34_DATA(SREG_Xold)))               //
          osc++;                                                            //
      }
    }
    if(getRegisterDataType(SREG_DY) == dtComplex34 && getRegisterDataType(SREG_Yold) == dtComplex34) {
      if(real34IsPositive(REGISTER_IMAG34_DATA(SREG_DY))) {               //
        if(real34IsNegative(REGISTER_IMAG34_DATA(SREG_Yold)))               //
          osc++;                                                            //
      }
    }

    //If osc flag is acrive, that is any oscillation, then increment oscillation count
    if(osc > 0) oscillations++; else
      oscillations = max(0,oscillations-1);

    //If converging, increment convergence counter
    if(real34CompareAbsLessThan(REGISTER_REAL34_DATA(SREG_DX), REGISTER_REAL34_DATA(SREG_Xold)) && real34CompareAbsLessThan(REGISTER_REAL34_DATA(SREG_DY), REGISTER_REAL34_DATA(SREG_Yold)) ) {
        convergent++;
    } else {
      convergent = max(0,convergent-1);
    }

    #ifdef VERBOSE_SOLVER1
      printf("################################### ix= %d osc= %d  conv= %d ###########################################\n",ix, oscillations, convergent);
    #endif //VERBOSE_SOLVER1


    // If increment is oscillating it is assumed that it is unstable and needs to have a complex starting value

    //assumes X2 is in R91
    if(((oscillations > 6) && (ix > 10) && (convergent < 5) )
       || (oscillations == 0 && convergent > 6 && real34CompareAbsLessThan(REGISTER_REAL34_DATA(SREG_DX), const34_1e_4) && (getRegisterDataType(SREG_DX) == dtComplex34 ? real34CompareAbsLessThan(REGISTER_IMAG34_DATA(SREG_DX), const34_1e_4) : 1 )  )
    ) {                                  //
      oscillations = 0;
      convergent = 0;
      #ifdef VERBOSE_SOLVER2
        printf("\n>>>>>>>>>>A ");printf_cpx(SREG_X2);
      #endif
      fnRCL(SREG_X2);
      doubleToXRegisterReal34( - ++kicker / 100.0);                           //
      fnSquareRoot(0);                                               //
      doubleToXRegisterReal34(1.0);                           //
      fnAdd(0);
      #ifdef VERBOSE_SOLVER1
        printf("Kicked %d ",kicker);printf_cpx(REGISTER_X);printf("\n");
      #endif
      fnMultiply(0);              //add i/10000 just to force it complex  //
      fnStore(SREG_X2); //replace X2 value                                //
      #ifdef VERBOSE_SOLVER2
        printf(">>>>>>>>>>H x 1+i0.1 = ");printf_cpx(SREG_X2); printf("\n");
      #endif
    }



  // If convergent, reduce the step sife using the f factor
  /*
    if(convergent < 2) {
      if(ix > 300) {doubleToXRegisterReal34(0.2); fnStore(SREG_F);} else //divide factor by 10
        if(ix > 60) {doubleToXRegisterReal34(0.45); fnStore(SREG_F);} else//divide factor by 10
          if(ix > 30) {doubleToXRegisterReal34(0.9); fnStore(SREG_F);} //divide factor by 10
    } else {
      if(convergent < 1) {
        if(ix >50) {doubleToXRegisterReal34(0.15); fnStore(SREG_F);} else //divide factor by 10
          if(ix >10) {doubleToXRegisterReal34(0.5); fnStore(SREG_F);} //divide factor by 10
      }
    }
  */



    fnRCL(SREG_X2);                                       // get (X2,Y2)
    execute_rpn_function(nbr);                            // leaving y2 in Y and x2 in X
    copySourceRegisterToDestRegister(REGISTER_Y,SREG_Y2); // y2


    // y2 in Y and x2 in X
    checkzero = checkzero || 
              ( real34IsZero(REGISTER_REAL34_DATA(SREG_Y2)) && (getRegisterDataType(SREG_Y2) == dtComplex34 ? real34IsZero(REGISTER_IMAG34_DATA(SREG_Y2)) : 1 ) );
    checkend = checkend || ix==400 ||
                checkzero || 
                real34IsNaN(REGISTER_REAL34_DATA(SREG_X2)) || (getRegisterDataType(SREG_X2) == dtComplex34 ? real34IsNaN(REGISTER_IMAG34_DATA(SREG_X2)) : 0 ) ||
                real34IsNaN(REGISTER_REAL34_DATA(SREG_Y2)) || (getRegisterDataType(SREG_Y2) == dtComplex34 ? real34IsNaN(REGISTER_IMAG34_DATA(SREG_Y2)) : 0 ) ; 
    #ifdef VERBOSE_SOLVER1
    if(checkend) {
       printf("-->A Endflags zero: Y2r:%u Y2i:%u NaN: X2r:%u X2i:%u Y2r:%u Y2i%u \n",
        (uint16_t)real34IsZero(REGISTER_REAL34_DATA(SREG_Y2)),
        (uint16_t)real34IsZero(REGISTER_IMAG34_DATA(SREG_Y2)),
        (uint16_t)real34IsNaN (REGISTER_REAL34_DATA(SREG_X2)),
        (uint16_t)real34IsNaN (REGISTER_IMAG34_DATA(SREG_X2)),
        (uint16_t)real34IsNaN (REGISTER_REAL34_DATA(SREG_Y2)),
        (uint16_t)real34IsNaN (REGISTER_IMAG34_DATA(SREG_Y2))
        );
    }
    #endif
    #ifdef VERBOSE_SOLVER2
      printf("   ix=%d checkend=%d X2=",ix, checkend); printf_cpx(SREG_X2); printf(" Y2="); printf_cpx(SREG_Y2); printf("\n");
    #endif //VERBOSE_SOLVER2


    //if(!checkend) //THIS PLOTS (x2,y2)
    //  if(getRegisterDataType(REGISTER_X) == dtReal34 && getRegisterDataType(REGISTER_Y) == dtReal34)  fnSigma(1);


    copySourceRegisterToDestRegister(SREG_DX,SREG_Xold);  // store old values
    copySourceRegisterToDestRegister(SREG_DY,SREG_Yold);  // store old values



    //DETERMINE DX and DY, to calculate the slope (or the inverse of the slope in this case)

#define AAA 1
if(AAA == 1) {

    if( !(ix>2 && convergent >= 2 && oscillations < 2) )
      {
    /*-2-sample slope-*/  fnRCL  (SREG_X2); fnRCL(SREG_X1); fnSubtract(0);      // dx
    /*-2-sample slope-*/  fnStore(SREG_DX);                                     // store difference for later
    /*-2-sample slope-*/  fnRCL  (SREG_Y2); fnRCL(SREG_Y1); fnSubtract(0);      // dy
    /*-2-sample slope-*/  fnStore(SREG_DY);                                     // store difference for later
    /*-2-sample slope-*/  //Leave DX in YREG, and DY in XREG, so DX/DY can be computed
    /*-2-sample slope-*/
      }


    //**** Up to here the standard SECANT type slope is computed. If the following part is executed, a better slope is computed using 3 y values.
    else 
      {
    /*-3-sample slope-*/  //The second order accurate one-sided finite difference formula for the first derivative, formule 32, of
    /*-3-sample slope-*/  //  ChE 205 — Formulas for Numerical Differentiation
    /*-3-sample slope-*/  //  Handout 5 05/08/02:
    /*-3-sample slope-*/
      #ifdef VERBOSE_SOLVER1
        fnInvert(0); printf(" Using 3 sample dydx\n");
      #endif
    /*-3-sample slope-*/  fnRCL  (SREG_X2); fnRCL(SREG_X1); fnSubtract(0); //Determine x2-x1
    /*-3-sample slope-*/  fnStore(SREG_DX);  //store difference DX for later
    /*-3-sample slope-*/  fnRCL  (SREG_X1);
    /*-3-sample slope-*/  fnSwapXY(0);
    /*-3-sample slope-*/  fnSubtract(0);
    /*-3-sample slope-*/  fnStore(SREG_X0);          //determine the new x0 by subtracting DX
    /*-3-sample slope-*/  execute_rpn_function(nbr); //determine the new f(x0)
    /*-3-sample slope-*/  copySourceRegisterToDestRegister(REGISTER_Y,SREG_Y0); //set y0 to the result f(x0)
    /*-3-sample slope-*/  //do DX = 2 (x2-x1)
    /*-3-sample slope-*/  fnRCL(SREG_DX);
    /*-3-sample slope-*/  doubleToXRegisterReal34(2);//calculate 2(x2-x1)
    /*-3-sample slope-*/  fnMultiply(0);             // DX = 2 delta x
    /*-3-sample slope-*/  //do DY = (fi−2 − 4fi−1 + 3fi)
    /*-3-sample slope-*/  fnRCL(SREG_Y0);              //y0
    /*-3-sample slope-*/  fnRCL(SREG_Y1);
    /*-3-sample slope-*/  doubleToXRegisterReal34(4); 
    /*-3-sample slope-*/  fnMultiply(0);
    /*-3-sample slope-*/  fnSubtract(0);               //-4.y1
    /*-3-sample slope-*/  fnRCL(SREG_Y2);
    /*-3-sample slope-*/  doubleToXRegisterReal34(3);
    /*-3-sample slope-*/  fnMultiply(0); 
    /*-3-sample slope-*/  fnAdd(0);                    //+3.y2
    /*-3-sample slope-*/  //Leave DX in YREG, and DY in XREG, so DX/DY can be computed
    /*-3-sample slope-*/
       }


    fnDivide(0);  //get the inverse of the slope dx/dy

    #ifdef VERBOSE_SOLVER2
      fnInvert(0); printf(" SLOPE="); printf_cpx(REGISTER_X); printf("\n"); fnInvert(0); 
    #endif

    fnRCL(SREG_Y1);      // determine increment in x
    fnMultiply(0);       // increment to x is: y1 . DX/DY
    fnRCL(SREG_F);       // factor to stabilize Newton method. factor=1 is straight. factor=0.1 converges 10x slower.
    fnMultiply(0);       // increment to x
} 
else
{
//Experimental bisection method
    fnRCL(SREG_Y2);
    fnRCL(SREG_Y1);
    fnMultiply(0);


    if(
      (real34IsNegative(REGISTER_REAL34_DATA(REGISTER_X)) && getRegisterDataType(REGISTER_X) == dtReal34) ||  
      (
        (getRegisterDataType(REGISTER_X) == dtComplex34) &&
        (
          (real34IsPositive(REGISTER_REAL34_DATA(REGISTER_X)) && real34IsNegative(REGISTER_IMAG34_DATA(REGISTER_X)) )
           ||
          (real34IsNegative(REGISTER_REAL34_DATA(REGISTER_X)) && real34IsPositive(REGISTER_IMAG34_DATA(REGISTER_X)) )
        )
      )
    ) {}



}

    #ifdef VERBOSE_SOLVER1
      printf("    New X =        "); printf_cpx(SREG_X1); printf(" - ("); printf_cpx(REGISTER_X); printf(")\n"); 
    #endif



    fnRCL(SREG_X1);
    fnSwapXY(0);
    fnSubtract(0);       // subtract as per Newton, x1 - f/f'
    fnStore(SREG_X2N);   // store temporarily to new x2n

    copySourceRegisterToDestRegister(SREG_Y2,SREG_Y1); //old y2 copied to y1
    copySourceRegisterToDestRegister(SREG_X2,SREG_X1); //old x2 copied to x1

    fnRCL(SREG_DY);    fnMagnitude(0); //difference dy
    fnRCL(SREG_DX);    fnMagnitude(0); //difference dx

    checkzero = checkzero || 
                ( real34IsZero(REGISTER_REAL34_DATA(REGISTER_X)) && (getRegisterDataType(REGISTER_X) == dtComplex34 ? real34IsZero(REGISTER_IMAG34_DATA(REGISTER_X)) : 1 )  &&
                  real34IsZero(REGISTER_REAL34_DATA(REGISTER_Y)) && (getRegisterDataType(REGISTER_Y) == dtComplex34 ? real34IsZero(REGISTER_IMAG34_DATA(REGISTER_Y)) : 1 ) ) ||
                ( (real34CompareAbsLessThan(REGISTER_REAL34_DATA(SREG_DX), REGISTER_REAL34_DATA(SREG_TOL))) && 
                  (real34CompareAbsLessThan(REGISTER_REAL34_DATA(SREG_DY), REGISTER_REAL34_DATA(SREG_TOL))) &&
                  (getRegisterDataType(SREG_DX) == dtComplex34 ? real34CompareAbsLessThan(REGISTER_IMAG34_DATA(SREG_DX), REGISTER_REAL34_DATA(SREG_TOL)) : 1 ) &&
                  (getRegisterDataType(SREG_DY) == dtComplex34 ? real34CompareAbsLessThan(REGISTER_IMAG34_DATA(SREG_DY), REGISTER_REAL34_DATA(SREG_TOL)) : 1 ) ) ;

    checkend = checkend || checkzero || 
                real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || (getRegisterDataType(REGISTER_X) == dtComplex34 ? real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X)) : 0 ) ||
                real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || (getRegisterDataType(REGISTER_Y) == dtComplex34 ? real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y)) : 0 ) ; 



    #ifdef VERBOSE_SOLVER1
    if(checkzero) {
      printf("--B1 Checkzero\n");
    }
    if(checkend) {
      printf("-->B2 Endflags zero: |DXr|:%u |DXi|:%u |DYr|:%u |DYi|:%u NaN: |DXr|:%u |DXi|:%u |DYr|:%u |DYi|%u \n",
        (uint16_t)real34IsZero(REGISTER_REAL34_DATA(REGISTER_X)),
        (uint16_t)real34IsZero(REGISTER_IMAG34_DATA(REGISTER_X)),
        (uint16_t)real34IsZero(REGISTER_REAL34_DATA(REGISTER_Y)),
        (uint16_t)real34IsZero(REGISTER_IMAG34_DATA(REGISTER_Y)),
        (uint16_t)real34IsNaN (REGISTER_REAL34_DATA(REGISTER_X)),
        (uint16_t)real34IsNaN (REGISTER_IMAG34_DATA(REGISTER_X)),
        (uint16_t)real34IsNaN (REGISTER_REAL34_DATA(REGISTER_Y)),
        (uint16_t)real34IsNaN (REGISTER_IMAG34_DATA(REGISTER_Y))
        );
    }
    #endif

    ix++;


    #ifdef VERBOSE_SOLVER2
      if(!checkend) {
        printf("   oldX2: ");printf_cpx(SREG_X2); printf("\n");
        printf("   Y2   : ");printf_cpx(SREG_Y2); printf("\n");
        printf("   ix=%d |DX|<TOL:%d ",ix,real34CompareAbsLessThan(REGISTER_REAL34_DATA(SREG_DX), REGISTER_REAL34_DATA(SREG_TOL)));printf_cpx(SREG_DX); printf("\n");
        printf("   ix=%d |DY|<TOL:%d ",ix,real34CompareAbsLessThan(REGISTER_REAL34_DATA(SREG_DY), REGISTER_REAL34_DATA(SREG_TOL)));printf_cpx(SREG_DY);
        printf("   DY: ");printf_cpx(REGISTER_Y); printf("\n");
      }
    #endif //VERBOSE_SOLVER2

    copySourceRegisterToDestRegister(SREG_X2N,SREG_X2);  //new x2    

    #ifdef VERBOSE_SOLVER1
      printf("   -------> newX2: ");printf_cpx(SREG_X2); printf("\n");
    #endif //VERBOSE_SOLVER1


    //plots the ix vs abs.difference
    if(!checkend || checkzero) {
      doubleToXRegisterReal34((double)ix);
      if(getRegisterDataType(REGISTER_X) == dtReal34 && getRegisterDataType(REGISTER_Y) == dtReal34)  fnSigma(1);
    }

  }
  fnClearStack(0);
  runFunction(ITM_NSIGMA);
  fnRCL(SREG_Y2);
  fnRCL(SREG_X2);
  //if( real34CompareAbsGreaterThan(REGISTER_REAL34_DATA(REGISTER_Z), const34_1 )) runFunction(ITM_PLOT);
  #endif
}



//-----------------------------------------------------//-----------------------------------------------------
                    //-----------------------------------------------------//-----------------------------------------------------
//NOTE SETPIXEL OPERANDS NOT CHANGED TO 32b
//-----------------------------------------------------//-----------------------------------------------------

#define randnum(min, max)  ((rand() % (int)(((max) + 1) - (min))) + (min))


void fnGraph (uint16_t func){

  hourGlassIconEnabled = true;
  showHideHourGlass();
  #ifdef DMCP_BUILD
    lcd_refresh();
  #else // !DMCP_BUILD
    refreshLcd(NULL);
  #endif // DMCP_BUILD


  Aspect_Square = false;
  PLOT_INTG = false;
  PLOT_DIFF = false;
  PLOT_RMS  = false;
  PLOT_ZMX  = 0;
  PLOT_ZMY  = 0;
  fnClSigma(0);
  switch (func) 
  {

	  case  3:  graph_demo(randnum(4,6), graph_xmin, graph_xmax);
	            break;
    
    case  9:  graph_solver(2, graph_xmin, graph_xmax);
              break;
    case 10:  graph_solver(3, graph_xmin, graph_xmax);
              break;

    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:  graph_demo(func-10, graph_xmin, graph_xmax);
              break;

	  default:;
   }
}




