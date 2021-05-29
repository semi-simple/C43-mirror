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

// Converted to runfunction:
// runFunction(ITM_ SUB):  fnSubtract (0)
// runFunction(ITM_ ADD):  fnAdd (0)
// runFunction(ITM_ DIV):  fnDivide (0)
// runFunction(ITM_ MULT): fnMultiply (0)
// runFunction(ITM_ YX):   fnPower (0)
// runFunction(ITM_ XexY): fnSwapXY (0)
// runFunction(ITM_ CHS):  fnChangeSign (0)
// runFunction(ITM_ENTER)
// runFunction(ITM_RAN)
// runFunction(ITM_CLSIGMA): fnClSigma (0)
// runFunction(ITM_SIGMAx)
// runFunction(ITM_PLOT_XY)
// runFunction(ITM_TICKS)
// runFunction(ITM_NSIGMA)
// runFunction(ITM_ sin): fnSin (0)
// runFunction(ITM_ cos): fnCos (0)
// runFunction(ITM_ SQUARE): fnSquare (0)
// runFunction(ITM_ CLSTK): fnClearStack (0);
// runFunction(ITM_ SIGMAPLUS): fnSigma (1);
// runFunction(ITM_ ABS): fnMagnitude (0); 
// runFunction(ITM_ ANGLE): fnArg_all (0);
// runFunction(ITM_ DEG): fnAngularMode (amDegree);
// runFunction(ITM_ SQUAREROOTX): fnSquareRoot (0); 
// runFunction(ITM_ RAD);  fnAngularMode (AM_RADIAN);

//ITM_STO
//ITM_RCL

#include "c43Extensions/jm_graph.h"

#include "c43Extensions/addons.h"
#include "mathematics/comparisonReals.h"
#include "constantPointers.h"
#include "c43Extensions/graphs.h"
#include "c43Extensions/graphText.h"
#include "items.h"
#include "c43Extensions/jm.h"
#include "plotstat.h"
#include "registers.h"
#include "screen.h"
#include "stack.h"
#include "stats.h"
#include "statusBar.h"
#include "store.h"
#include "c43Extensions/xeqm.h"

#include "wp43s.h"


#define X 99
#define X_min 98


void Fn_Lbl_A(void) {                                   //Temporary RPN function
#ifndef TESTSUITE_BUILD

    fnStore(X);             // STO X
    fnStoreMin(X_min);
    fnRCL(X);               // X = X - Xmin + 0.3 
    fnRCL(X_min);
    runFunction(ITM_SUB);
    fnStrInputReal34("0.3");
    runFunction(ITM_ADD);
    fnStore(X);

    fnStrtoX( "(sin(x)/x + sin(10x)/5) / 2 + 2/x" ); fnStore(80);
    fnRCL(X);
    runFunction(ITM_sin);    // SIN
    fnRCL(X);               // RCL X
    runFunction(ITM_DIV);    // /
 
    fnRCL(X);               // RCL X
    fnStrInputReal34("10");  // 10
    runFunction(ITM_MULT);   // *
    runFunction(ITM_sin);    // SIN
    fnStrInputReal34("5");   // 5
    runFunction(ITM_DIV);    // /
 
    runFunction(ITM_ADD);    // +    

    fnStrInputReal34("2");   // 2
    runFunction(ITM_DIV);    // /

    fnStrInputReal34("2");   // 2
    fnRCL(X);               // RCL X
    runFunction(ITM_DIV);    // /
    runFunction(ITM_ADD);    // +    
    fnRCL(X);               //leaving y in Y and x in X
#endif
}



#define FN 3

void Fn_Lbl_B(void) {                                   //Temporary RPN function
#ifndef TESTSUITE_BUILD
    fnStore(X);             // STO 99   

		switch(FN) {
		  case 1: 
		    fnStrtoX("sin(x) + sin(x^2) = 0"); fnStore(80);
		    fnRCL(X);
		    runFunction(ITM_sin);                // SIN    
		    fnRCL(X);
		    runFunction(ITM_SQUARE);             // square
		    runFunction(ITM_sin);                // SIN
		    runFunction(ITM_ADD);                // +    
		    break;

		  case 2:
		    fnStrtoX("x^(1/3) + 1 = 0"); fnStore(80);
		    fnRCL(X);
		    doubleToXRegisterReal34(1);         //cube root
		    doubleToXRegisterReal34(3);
		    runFunction(ITM_DIV);
		    runFunction(ITM_YX);
		    doubleToXRegisterReal34(1);         //+constant
		    runFunction(ITM_ADD);
		    break;

		  case 3:
		    fnStrtoX("x^5 - x + 1 = 0"); fnStore(80);
		    fnRCL(X);
		    doubleToXRegisterReal34(5);
		    runFunction(ITM_YX);
		    fnRCL(X);
		    runFunction(ITM_SUB);
		    doubleToXRegisterReal34(1);
		    runFunction(ITM_ADD); 
		    break;

		  default:break;
		}

    fnRCL(X);               //leaving y in Y and x in X
#endif
}


void Fn_Lbl_C(void) {                                   //Temporary RPN function
#ifndef TESTSUITE_BUILD
    fnStore(X);             // STO 99

//Original DEMO3
//    runFunction(ITM_sinc);

    fnStrtoX("x^2 + 1 = 0"); fnStore(80);
    fnRCL(X);
    runFunction(ITM_SQUARE);                   //x^2 + 1 = 0
    fnStrInputReal34("-1");
    runFunction(ITM_SUB);


    fnRCL(X);               //leaving y in Y and x in X
#endif
}







//FOR STAT GRANH. LEAVES ANSWER IN X & Y
void Fn_Lbl_D(void) {                                   //Temporary RPN function 0.1x 
#ifndef TESTSUITE_BUILD
    fnStore(X);             // STO 99

    fnStrtoX("( 0.1x^2 + 0.15x + (rand-0.5) * 0.4 ) + 2.5"); fnStore(80);
    fnRCL(X);

    runFunction(ITM_ENTER);  // ENTER          // /
    fnStrInputReal34("0.1"); // 0.1
    runFunction(ITM_MULT);   // *              // /
    runFunction(ITM_MULT);   // *              // /
    fnRCL(X);               // RCL 99
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
    fnRCL(X);
#endif
}


void Fn_Lbl_E(void) {                                   //Temporary RPN function
#ifndef TESTSUITE_BUILD
    fnStore(X);             // STO 99

    fnStrtoX(" ?? "); fnStore(80);
    fnRCL(X);

    fnStrInputReal34("0.6"); // 0.2
    runFunction(ITM_MULT);   // *             // /

    runFunction(ITM_RAN);    // fnRandom(0);                //    
    fnStrInputReal34("-0.5"); // -0.5
    runFunction(ITM_ADD);    // +             // /

    fnStrInputReal34("0.4"); // 0.4
    runFunction(ITM_MULT);   // *             // /

    runFunction(ITM_ADD);    // +             // /
    fnRCL(X);               //leaving y in Y and x in X
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


void Fn_XXEQ(void) {
  #ifndef TESTSUITE_BUILD
    fnRCL(81);
    fnXEQMXXEQ(0);
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
  } else
  if(nbr == 20) {
    Fn_XXEQ();
  }
}






//###################################################################################
//GRAPH
void graph_demo(uint8_t nbr, float x_min, float x_max) {
  #ifndef TESTSUITE_BUILD
  double x; 
  cancelFilename = true;
  runFunction(ITM_CLSTK);

  if(telltale != MEM_INITIALIZED) {
    graph_setupmemory();
    runFunction(ITM_CLSIGMA);
  }

  runFunction(ITM_RAD);

  //  for(x=x_min; x<=x_max; x+=(x_max-x_min)/SCREEN_WIDTH_GRAPH) {
  for(x=x_min; x<=x_max; x+=0.99999*(x_max-x_min)/SCREEN_WIDTH_GRAPH*10) {    //Reduced the amount of sample data from 400 points to 40 points

    //convert double to X register
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    snprintf(tmpString, TMP_STR_LENGTH, "%.16e", x);
    stringToReal34(tmpString, REGISTER_REAL34_DATA(REGISTER_X));

    //leaving y in Y and x in X
    execute_rpn_function(nbr);
   	runFunction(ITM_SIGMAPLUS);
    if(lastErrorCode == 24) { printf("ERROR CODE CANNOT STAT COMPLEX ignored\n"); lastErrorCode = 0;}
  }
  runFunction(ITM_CLSTK);
  runFunction(ITM_SIGMAx);
  runFunction(ITM_PLOT_XY);
  #endif
}




//#define VERBOSE_SOLVER1  // a lot less text
//#define VERBOSE_SOLVER2  // verbose a lot



//###################################################################################
//SOLVER
void graph_solver(uint8_t nbr, float x_min, float x_max) {
#ifndef SAVE_SPACE_DM42_3
  #ifndef TESTSUITE_BUILD
  int16_t ix;
  int16_t oscillations = 0; 
  int16_t convergent = 0; 
  bool_t checkNaN = false;
  bool_t checkzero = false;
  cancelFilename = true;

  fnStrInputReal34("1.0");
  runFunction(ITM_MULT);     //Convert to REAL
  #ifdef VERBOSE_SOLVER1
    printf("XREG START: "); printf_cpx(REGISTER_X); printf("\n");
  #endif //VERBOSE_SOLVER1

  // Initialize all temporary registers
  // Registers 85 to 97; 98 and 99 are being used in the DEMO data programs
  calcRegister_t SREG_STARTX = 82;
  copySourceRegisterToDestRegister(REGISTER_X,SREG_STARTX);
  runFunction(ITM_TICKS);
  calcRegister_t SREG_TICKS = 83; fnStore(SREG_TICKS);
  doubleToXRegisterReal34(0.0);
  calcRegister_t SREG_TMP  = 84; fnStore(SREG_TMP);
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

  fnRCL(SREG_STARTX);                                  // leftmost graph value
  fnStore(SREG_X1);                                    // initial x1   
  execute_rpn_function(nbr);                           // leaving y in y
  copySourceRegisterToDestRegister(REGISTER_Y,SREG_Y1);   // initial y1

  double startval = 1; 
  startval = convert_to_double(SREG_STARTX);    //********** X REGISTER IS STARTING POINT
  doubleToXRegisterReal34(startval + (x_max-x_min) / SCREEN_WIDTH_GRAPH);  //first auto increment is 1 pixel width on the graph
  fnStore(SREG_X2);                                    // initial x2

  ix = 0;
  int16_t kicker = 0;

  #ifdef VERBOSE_SOLVER2
    printf("INIT:   ix=%d X1=",ix); printf_cpx(SREG_X1); printf("\n");
    printf("INIT:   ix=%d X2=",ix); printf_cpx(SREG_X2); printf("\n");
  #endif //VERBOSE_SOLVER2


  //Start STAT/GRAPH memory
  runFunction(ITM_CLSTK);
  if(telltale != MEM_INITIALIZED) {
    graph_setupmemory();
    runFunction(ITM_CLSIGMA);
  }
  runFunction(ITM_RAD);




//###############################################################################################################
//#################################################### Iteration start ##########################################
  while(ix<400 && !checkNaN && !checkzero) 
  {
    if(lastErrorCode != 0) { printf(">>>> ERROR CODE INITIALLY NON-ZERO = %d <<<<<\n",lastErrorCode); return;}

#define CCC
#ifdef CCC
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
      #endif //VERBOSE_SOLVER2
      fnRCL(SREG_X2);
      doubleToXRegisterReal34( - ++kicker / 100.0);                           //
      runFunction(ITM_SQUAREROOTX);                                              //
      doubleToXRegisterReal34(1.0);                           //
      runFunction(ITM_ADD);
      #ifdef VERBOSE_SOLVER1
        printf("Kicked %d ",kicker);printf_cpx(REGISTER_X);printf("\n");
      #endif  //VERBOSE_SOLVER1
      runFunction(ITM_MULT);              //add i/10000 just to force it complex  //
      fnStore(SREG_X2); //replace X2 value                                //
      #ifdef VERBOSE_SOLVER2
        printf(">>>>>>>>>>H x 1+i0.1 = ");printf_cpx(SREG_X2); printf("\n");
      #endif //VERBOSE_SOLVER2
    }


/*
//reduce the step size using the f factor
    if(convergent > 5 && oscillations == 0) {
      if(ix > 300) {doubleToXRegisterReal34(0.2); fnStore(SREG_F);} else //divide factor by 10
        if(ix > 60) {doubleToXRegisterReal34(0.45); fnStore(SREG_F);} else//divide factor by 10
          if(ix > 30) {doubleToXRegisterReal34(0.9); fnStore(SREG_F);} //divide factor by 10
    } else {
      if(oscillations > 5) {
        if(ix >50) {doubleToXRegisterReal34(0.15); fnStore(SREG_F);} else //divide factor by 10
          if(ix >10) {doubleToXRegisterReal34(0.5); fnStore(SREG_F);} //divide factor by 10
      }
    }
*/


#endif //CCC

//@@@@@@@@@@@@@@@@@ CALCULATE NEW Y2, AND PLAUSIBILITY @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    fnRCL(SREG_X2);                                       // get (X2,Y2)
    execute_rpn_function(nbr);                            // leaving y2 in Y and x2 in X
    copySourceRegisterToDestRegister(REGISTER_Y,SREG_Y2); // y2

    #ifdef VERBOSE_SOLVER2
      printf("    :   ix=%d X1=",ix); printf_cpx(SREG_X1); printf(" Y1="); printf_cpx(SREG_Y1); printf("\n");
      printf("    :   ix=%d X2=",ix); printf_cpx(SREG_X2); printf(" Y2="); printf_cpx(SREG_Y2); printf("\n");
    #endif //VERBOSE_SOLVER2

    // y2 in Y and x2 in X
    checkzero = checkzero || ( real34IsZero(REGISTER_REAL34_DATA(SREG_Y2)) && (getRegisterDataType(SREG_Y2) == dtComplex34 ? real34IsZero(REGISTER_IMAG34_DATA(SREG_Y2)) : 1 ) );
    checkNaN  = checkNaN  ||   real34IsNaN(REGISTER_REAL34_DATA(SREG_X2)) || (getRegisterDataType(SREG_X2) == dtComplex34 ? real34IsNaN(REGISTER_IMAG34_DATA(SREG_X2)) : 0 ) ||
                               real34IsNaN(REGISTER_REAL34_DATA(SREG_Y2)) || (getRegisterDataType(SREG_Y2) == dtComplex34 ? real34IsNaN(REGISTER_IMAG34_DATA(SREG_Y2)) : 0 ) ; 
    #ifdef VERBOSE_SOLVER1
    if(checkNaN || ix==400 || checkzero) {
       printf("-->A Endflags zero: Y2r:%u Y2i:%u NaN: X2r:%u X2i:%u Y2r:%u Y2i%u \n",
        (uint16_t)real34IsZero(REGISTER_REAL34_DATA(SREG_Y2)),(uint16_t)real34IsZero(REGISTER_IMAG34_DATA(SREG_Y2)),
        (uint16_t)real34IsNaN (REGISTER_REAL34_DATA(SREG_X2)),(uint16_t)real34IsNaN (REGISTER_IMAG34_DATA(SREG_X2)),
        (uint16_t)real34IsNaN (REGISTER_REAL34_DATA(SREG_Y2)),(uint16_t)real34IsNaN (REGISTER_IMAG34_DATA(SREG_Y2))
        );
    }
    #endif //VERBOSE_SOLVER1
    #ifdef VERBOSE_SOLVER2
      printf("   ix=%d checkend=%d X2=",ix, checkNaN || ix==400 || checkzero); printf_cpx(SREG_X2); printf(" Y2="); printf_cpx(SREG_Y2); printf("\n");
    #endif //VERBOSE_SOLVER2


    //if(!checkend) //THIS PLOTS (x2,y2)
    //  if(getRegisterDataType(REGISTER_X) == dtReal34 && getRegisterDataType(REGISTER_Y) == dtReal34)  runFunction(ITM_SIGMAPLUS);


    if(checkzero || checkNaN) goto EndIteration;
//*************** DETERMINE DX and DY, to calculate the slope (or the inverse of the slope in this case) *******************







if(  (ix <=2) /*&& (oscillations <= 2)*/   /*|| (getRegisterDataType(SREG_X2N) == dtComplex34 || getRegisterDataType(SREG_X2N) == dtComplex34)*/ )
{
    copySourceRegisterToDestRegister(SREG_DX,SREG_Xold);  // store old DELTA values, for oscillation check
    copySourceRegisterToDestRegister(SREG_DY,SREG_Yold);  // store old DELTA values, for oscillation check

    //Secant and Newton approximation methods
        if( !(ix>2 && convergent >= 2 && oscillations < 2) )
        {
//###########################
//  normal Secant
//  DX = X2 - X1 in YREGISTER
//  DY = Y2 - Y1 in XREGISTER
//###########################
          #ifdef VERBOSE_SOLVER1
            printf("---------- Using normal Secant dydx\n");
          #endif          	
	        /*-2-sample slope-*/  fnRCL  (SREG_X2); fnRCL(SREG_X1); runFunction(ITM_SUB);      // dx
	        /*-2-sample slope-*/  fnStore(SREG_DX);                                     // store difference for later
	        /*-2-sample slope-*/  fnRCL  (SREG_Y2); fnRCL(SREG_Y1); runFunction(ITM_SUB);      // dy
	        /*-2-sample slope-*/  fnStore(SREG_DY);                                     // store difference for later
	        /*-2-sample slope-*/  //Leave DX in YREG, and DY in XREG, so DX/DY can be computed
	        /*-2-sample slope-*/
          }

        else 
        {
//###########################
// normal secant with 3 sample slope
//  DX = X2 - X1 in YREGISTER
//  DY = Y2 - Y1 in XREGISTER
//###########################
	        /*-3-sample slope-*/  //  The second order accurate one-sided finite difference formula for the first derivative, formule 32, of
	        /*-3-sample slope-*/  //  ChE 205 — Formulas for Numerical Differentiation
	        /*-3-sample slope-*/  //  Handout 5 05/08/02:
	        /*-3-sample slope-*/
          #ifdef VERBOSE_SOLVER1
            printf("---------- Using Secant with special 3 sample dy/dx\n");
          #endif
	        /*-3-sample slope-*/  fnRCL      (SREG_X2); fnRCL(SREG_X1); runFunction(ITM_SUB); //Determine x2-x1
	        /*-3-sample slope-*/  fnStore    (SREG_DX);  //store difference DX for later
	        /*-3-sample slope-*/  fnRCL      (SREG_X1);
	        /*-3-sample slope-*/  runFunction(ITM_XexY);
	        /*-3-sample slope-*/  runFunction(ITM_SUB);
	        /*-3-sample slope-*/  fnStore    (SREG_X0);          //determine the new x0 by subtracting DX
	        /*-3-sample slope-*/  execute_rpn_function(nbr); //determine the new f(x0)
	        /*-3-sample slope-*/  copySourceRegisterToDestRegister(REGISTER_Y,SREG_Y0); //set y0 to the result f(x0)
	        /*-3-sample slope-*/  //do DX = 2 (x2-x1)
	        /*-3-sample slope-*/  fnRCL      (SREG_DX);
	        /*-3-sample slope-*/  doubleToXRegisterReal34(2);//calculate 2(x2-x1)
	        /*-3-sample slope-*/  runFunction(ITM_MULT);             // DX = 2 delta x
	        /*-3-sample slope-*/  //do DY = (fi−2 − 4fi−1 + 3fi)
	        /*-3-sample slope-*/  fnRCL      (SREG_Y0);              //y0
	        /*-3-sample slope-*/  fnRCL      (SREG_Y1);
	        /*-3-sample slope-*/  doubleToXRegisterReal34(4); 
	        /*-3-sample slope-*/  runFunction(ITM_MULT);
	        /*-3-sample slope-*/  runFunction(ITM_SUB);               //-4.y1
	        /*-3-sample slope-*/  fnRCL      (SREG_Y2);
	        /*-3-sample slope-*/  doubleToXRegisterReal34(3);
	        /*-3-sample slope-*/  runFunction(ITM_MULT); 
	        /*-3-sample slope-*/  runFunction(ITM_ADD);                    //+3.y2
                                  fnStore    (SREG_DY);
	        /*-3-sample slope-*/  //Leave DX in YREG, and DY in XREG, so DX/DY can be computed
	        /*-3-sample slope-*/
        }

//###########################
//  Start with DX and DY
//  
//  
//###########################
        runFunction(ITM_DIV);  //get the inverse of the slope, dx/dy
        if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || (getRegisterDataType(REGISTER_X) == dtComplex34 ? real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X)) : 0 ) ) 
          doubleToXRegisterReal34(0);

        #ifdef VERBOSE_SOLVER2
          fnInvert(0); printf(" SLOPE="); printf_cpx(REGISTER_X); printf("\n"); fnInvert(0); 
        #endif

        fnRCL(SREG_Y1);      // determine increment in x
        runFunction(ITM_MULT);       // increment to x is: y1 . DX/DY
        fnRCL(SREG_F);       // factor to stabilize Newton method. factor=1 is straight. factor=0.1 converges 10x slower.
        runFunction(ITM_MULT);       // increment to x

        #ifdef VERBOSE_SOLVER1
          printf("    Factor=        "); printf_cpx(SREG_F);printf("\n");
          printf("    New X =        "); printf_cpx(SREG_X1); printf(" - ("); printf_cpx(REGISTER_X); printf(")\n"); 
        #endif

        fnRCL(SREG_X1);
        runFunction(ITM_XexY);
        runFunction(ITM_SUB);       // subtract as per Newton, x1 - f/f'
        fnStore(SREG_X2N);          // store temporarily to new x2n
} else





{
#ifdef VERBOSE_SOLVER1
printf("---------- Modified 3 point Secant ----------");
#endif //VERBOSE_SOLVER1
//YREG
fnRCL(SREG_Y2);fnRCL(SREG_Y1);runFunction(ITM_SUB);fnStore(SREG_DY);

//XREG
fnRCL(SREG_Y2);fnRCL(SREG_Y0);runFunction(ITM_SUB);
fnRCL(SREG_X2);fnRCL(SREG_X0);runFunction(ITM_SUB);runFunction(ITM_DIV);fnStore(SREG_TMP);fnStore(SREG_DX);
fnRCL(SREG_Y2);fnRCL(SREG_Y1);runFunction(ITM_SUB);runFunction(ITM_MULT);

fnRCL(SREG_Y2);
fnRCL(SREG_TMP);
fnRCL(SREG_Y1);fnRCL(SREG_Y0);runFunction(ITM_SUB);
fnRCL(SREG_X1);fnRCL(SREG_X0);runFunction(ITM_SUB);runFunction(ITM_DIV);
runFunction(ITM_SUB);runFunction(ITM_MULT);

runFunction(ITM_SUB);

runFunction(ITM_DIV);  //get the slope, dy/dx
if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || (getRegisterDataType(REGISTER_X) == dtComplex34 ? real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X)) : 0 ) ) 
  doubleToXRegisterReal34(0);



#ifdef VERBOSE_SOLVER2
  printf(" SLOPE="); printf_cpx(REGISTER_X); printf("\n");
#endif //VERBOSE_SOLVER2
fnRCL(SREG_Y0);              // determine increment in x
runFunction(ITM_MULT);       // increment to x is: y1 . DX/DY
fnRCL(SREG_F);               // factor to stabilize Newton method. factor=1 is straight. factor=0.1 converges 10x slower.
runFunction(ITM_MULT);       // increment to x
#ifdef VERBOSE_SOLVER1
  printf("    Factor=        "); printf_cpx(SREG_F);printf("\n");
  printf("    New X =        "); printf_cpx(SREG_X0); printf(" - ("); printf_cpx(REGISTER_X); printf(")\n"); 
#endif //VERBOSE_SOLVER1

fnRCL(SREG_X0);
runFunction(ITM_XexY);
runFunction(ITM_SUB);        // subtract as per Newton, x1 - f/f'
fnStore(SREG_X2N);           // store temporarily to new x2n
}



copySourceRegisterToDestRegister(SREG_Y1,SREG_Y0); //old y1 copied to y0
copySourceRegisterToDestRegister(SREG_X1,SREG_X0); //old x1 copied to x0





  //Experimental bisection method to kick
  bool_t bisect = false;
/*
  if( ((getRegisterDataType(SREG_Y1) == dtComplex34) || (getRegisterDataType(SREG_Y2) == dtComplex34)) ) {
    doubleToXRegisterReal34(-180+20);  //Z -90
    doubleToXRegisterReal34( 180-20);  //Y 180
    fnRCL(SREG_Y2);
    fnRCL(SREG_Y1);
    runFunction(ITM_MULT);
    #ifdef VERBOSE_SOLVER2
      printf(" Bisection complex product evaluated:");printf_cpx(REGISTER_X);printf("\n");
    #endif
    runFunction(ITM_DEG);
    runFunction(ITM_ANGLE);
    if(  real34CompareLessThan(REGISTER_REAL34_DATA(REGISTER_X),REGISTER_REAL34_DATA(REGISTER_Z)) || 
         real34CompareGreaterThan(REGISTER_REAL34_DATA(REGISTER_X),REGISTER_REAL34_DATA(REGISTER_Y))
      ) 
    {
      bisect = true;
      #ifdef VERBOSE_SOLVER1
          printf(" Bisection complex product complies=");printf_cpx(REGISTER_X);printf(" degrees\n");
      #endif
    }
  }
*/
   if( //real34IsNaN(REGISTER_REAL34_DATA(SREG_DX)) || real34IsInfinite(REGISTER_REAL34_DATA(SREG_DX)) ||

         ( (getRegisterDataType(SREG_Y0) == dtReal34) && 
           (getRegisterDataType(SREG_Y2) == dtReal34) && 
           (real34CompareAbsGreaterThan(REGISTER_REAL34_DATA(SREG_Y0),const34_1e_6)) && 
           (real34CompareAbsGreaterThan(REGISTER_REAL34_DATA(SREG_Y2),const34_1e_6)) &&
            (
              ( (real34IsNegative(REGISTER_REAL34_DATA(SREG_Y0))) && (real34IsPositive(REGISTER_REAL34_DATA(SREG_Y2))) ) ||
              ( (real34IsNegative(REGISTER_REAL34_DATA(SREG_Y2))) && (real34IsPositive(REGISTER_REAL34_DATA(SREG_Y0))) )
            ) &&

    	   !((real34CompareGreaterEqual(REGISTER_REAL34_DATA(SREG_X2N),REGISTER_REAL34_DATA(SREG_X0)) &&
    		 real34CompareGreaterEqual(REGISTER_REAL34_DATA(SREG_X2),REGISTER_REAL34_DATA(SREG_X2N)) ) ||
    		 (real34CompareLessEqual(REGISTER_REAL34_DATA(SREG_X2N),REGISTER_REAL34_DATA(SREG_X0)) &&
    		 real34CompareLessEqual(REGISTER_REAL34_DATA(SREG_X2),REGISTER_REAL34_DATA(SREG_X2N)) ) )

        ) 
      )
    {
      bisect = true;
      #ifdef VERBOSE_SOLVER1
        printf(" Using Bisection method: Y bracketed\n");
      #endif
    }
  if(bisect){
    fnRCL(SREG_X0);
    fnRCL(SREG_X2);
    runFunction(ITM_ADD);
    doubleToXRegisterReal34(2);
    runFunction(ITM_DIV); //Leaving (x1+x2)/2
    fnStore(SREG_X2N);   // store temporarily to new x2n
  }




//#############################################




    copySourceRegisterToDestRegister(SREG_Y2,SREG_Y1); //old y2 copied to y1
    copySourceRegisterToDestRegister(SREG_X2,SREG_X1); //old x2 copied to x1

    fnRCL(SREG_DY);   runFunction(ITM_ABS); //difference dy
    fnRCL(SREG_DX);   runFunction(ITM_ABS); //difference dx


EndIteration:

    checkzero = checkzero || 
                ( real34IsZero(REGISTER_REAL34_DATA(REGISTER_X)) && (getRegisterDataType(REGISTER_X) == dtComplex34 ? real34IsZero(REGISTER_IMAG34_DATA(REGISTER_X)) : 1 )  &&
                  real34IsZero(REGISTER_REAL34_DATA(REGISTER_Y)) && (getRegisterDataType(REGISTER_Y) == dtComplex34 ? real34IsZero(REGISTER_IMAG34_DATA(REGISTER_Y)) : 1 ) ) ||
                ( (real34CompareAbsLessThan(REGISTER_REAL34_DATA(SREG_DX), REGISTER_REAL34_DATA(SREG_TOL))) && 
                  (real34CompareAbsLessThan(REGISTER_REAL34_DATA(SREG_DY), REGISTER_REAL34_DATA(SREG_TOL))) &&
                  (getRegisterDataType(SREG_DX) == dtComplex34 ? real34CompareAbsLessThan(REGISTER_IMAG34_DATA(SREG_DX), REGISTER_REAL34_DATA(SREG_TOL)) : 1 ) &&
                  (getRegisterDataType(SREG_DY) == dtComplex34 ? real34CompareAbsLessThan(REGISTER_IMAG34_DATA(SREG_DY), REGISTER_REAL34_DATA(SREG_TOL)) : 1 ) ) ;

    checkNaN = checkNaN || 
                real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || (getRegisterDataType(REGISTER_X) == dtComplex34 ? real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X)) : 0 ) ||
                real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || (getRegisterDataType(REGISTER_Y) == dtComplex34 ? real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y)) : 0 ) ; 



    #ifdef VERBOSE_SOLVER1
    if(checkzero) {
      printf("--B1 Checkzero\n");
    }
    if(checkNaN || ix==400 || checkzero) {
      printf("-->B2 Endflags zero: |DXr|:%u |DXr<TOL|:%u  |DXi|:%u |DYr<TOL|:%u |DYr|:%u |DYi|:%u NaN: |DXr|:%u |DXi|:%u |DYr|:%u |DYi|%u \n",
        (uint16_t)real34IsZero(REGISTER_REAL34_DATA(REGISTER_X)),
        (uint16_t)(real34CompareAbsLessThan(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(SREG_TOL))),
        (uint16_t)real34IsZero(REGISTER_IMAG34_DATA(REGISTER_X)),
        (uint16_t)real34IsZero(REGISTER_REAL34_DATA(REGISTER_Y)),
        (uint16_t)(real34CompareAbsLessThan(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(SREG_TOL))),
        (uint16_t)real34IsZero(REGISTER_IMAG34_DATA(REGISTER_Y)),
        (uint16_t)real34IsNaN (REGISTER_REAL34_DATA(REGISTER_X)),
        (uint16_t)real34IsNaN (REGISTER_IMAG34_DATA(REGISTER_X)),
        (uint16_t)real34IsNaN (REGISTER_REAL34_DATA(REGISTER_Y)),
        (uint16_t)real34IsNaN (REGISTER_IMAG34_DATA(REGISTER_Y))
        );
    }
    #endif //VERBOSE_SOLVER2

    ix++;


    #ifdef VERBOSE_SOLVER2
      if(!checkNaN && !(ix==400) && !checkzero) {
        printf("END     ix=%d |DX|<TOL:%d ",ix,real34CompareAbsLessThan(REGISTER_REAL34_DATA(SREG_DX), REGISTER_REAL34_DATA(SREG_TOL)));printf_cpx(SREG_DX); printf("\n");
        printf("END     ix=%d |DY|<TOL:%d ",ix,real34CompareAbsLessThan(REGISTER_REAL34_DATA(SREG_DY), REGISTER_REAL34_DATA(SREG_TOL)));printf_cpx(SREG_DY); printf("\n");
        printf("END     DY=");printf_cpx(REGISTER_Y); printf("\n");
      }
    #endif //VERBOSE_SOLVER2

    #ifdef VERBOSE_SOLVER1
      printf("               DX: ");printf_cpx(SREG_DX); printf("\n");
      printf("               DY: ");printf_cpx(SREG_DY); printf("\n");
      printf("               X0: ");printf_cpx(SREG_X0); printf("\n");
      printf("               X1: ");printf_cpx(SREG_X1); printf("\n");
      printf("               X2: ");printf_cpx(SREG_X2); printf("\n");
      printf("   -------> newX2: ");printf_cpx(SREG_X2N); printf("\n");
    #endif //VERBOSE_SOLVER1


    copySourceRegisterToDestRegister(SREG_X2N,SREG_X2);  //new x2    


    //plots the ix vs abs.difference
    if((!checkNaN && !(ix==400)) || checkzero) {
      doubleToXRegisterReal34((double)ix);
      if(getRegisterDataType(REGISTER_X) == dtReal34 && getRegisterDataType(REGISTER_Y) == dtReal34)  {
        runFunction(ITM_SIGMAPLUS);
        if(lastErrorCode == 24) { 
        	#ifdef VERBOSE_SOLVER1
        	printf("ERROR CODE CANNOT STAT COMPLEX ignored\n"); 
        	#endif //VERBOSE_SOLVER1
        	lastErrorCode = 0;}
      }
    }

      #ifdef DMCP_BUILD
        lcd_refresh();
      #else // !DMCP_BUILD
        refreshLcd(NULL);
      #endif // DMCP_BUILD

  }
  runFunction(ITM_CLSTK);
  bool_t running_temp = running_program_jm; //prevent temporary info on subtracts
  running_program_jm = false;
  fnRCL(80);
  fnStrtoX("; Xo= ");
  fnRCL(SREG_STARTX);
  runFunction(ITM_ADD);
  runFunction(ITM_ADD);
  

  runFunction(ITM_TICKS);       //Calculate time used
  fnRCL(SREG_TICKS);
  runFunction(ITM_CHS);
  runFunction(ITM_ADD);
  doubleToXRegisterReal34(10);
  runFunction(ITM_DIV);
  fnStrtoX(" s for ");
  runFunction(ITM_ADD);

  runFunction(ITM_NSIGMA);     //Check number of iterations
  fnStrtoX(" iterations");
  runFunction(ITM_ADD);

  runFunction(ITM_ADD);

  fnStrtoX(" f(x)= ");
  fnRCL(SREG_Y2);
  runFunction(ITM_ADD);
  fnStrtoX(" at x= ");
  runFunction(ITM_ADD);
  
  fnRCL(SREG_X2);
  
  if( real34CompareAbsGreaterThan(REGISTER_REAL34_DATA(REGISTER_Z), const34_2pi )) runFunction(ITM_PLOT_XY);
  running_program_jm =  running_temp;
  #endif

#endif //SAVE_SPACE_DM42_3
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
  PLOT_AXIS = true;
  PLOT_ZMX  = 0;
  PLOT_ZMY  = 0;
  fnClSigma(0);   //For some strange reason runFunction(ITM_CLSIGMA); does not work in this position !!! Cannot solve the riddle, rarther just call the function directly

  running_program_jm = true;
  double ix, ix0, ix1;  
  switch (func) 
  {

	  case  3:  graph_demo(randnum(4,6), graph_xmin, graph_xmax);
	            break;
    #ifndef SAVE_SPACE_DM42_4

      case  9:  graph_solver(2, graph_xmin, graph_xmax);
                break;
      case 10:  graph_solver(3, graph_xmin, graph_xmax);
                break;
    #endif //SAVE_SPACE_DM42_4

    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:  graph_demo(func-10, graph_xmin, graph_xmax);
              break;

    #ifndef SAVE_SPACE_DM42_4
      case 20:  //SOLVE

                //This is the desired sequence: FORMULA_TEXT, RPN_FORMULA, starting value in X.

                fnStore(82);
                fnDrop(0);

                ix = convert_to_double(REGISTER_X);
                //printf("#### Recalling register %u\n",(int)(ix));
                fnDrop(0);
                fnRCL((int)(ix+1));
                fnStore(80);
                fnRCL((int)(ix));
                fnStore(81);

                fnRCL(82);
                graph_solver(20, graph_xmin, graph_xmax);
                break;
      #endif //SAVE_SPACE_DM42_4
    case 21: //PLOT

              ix1 = convert_to_double(REGISTER_X);
              ix0 = convert_to_double(REGISTER_Y);
              printf("#### ix0=%f ix1=%f\n",ix0,ix1);
              if(ix1>ix0 && !(ix1==ix0)) {
                graph_xmin = ix0;
                graph_xmax = ix1;
              }

              ix = convert_to_double(REGISTER_Z);
              printf("#### Recalling register %u\n",(int)(ix));

              fnDrop(0);
              fnDrop(0);
              fnDrop(0);
              fnRCL((int)(ix+1));
              fnStore(80);
              fnRCL((int)(ix));
              fnStore(81);

              graph_demo(20, graph_xmin, graph_xmax);
              fnRCL(80);
              break;

    case 22:
      #ifndef SAVE_SPACE_DM42_4
                fnStrtoX("DEMO: Rnn Xo X.SOLV:     ");
                fnStrtoX("DEMO: Rnn X0 X1 X.PLOT:  ");
                fnStrtoX("(R=10,12,14,16,18) (X0=0 & X1=0 default range)");

                fnStrtoX("XEQC43 STO 99 X^3 RCL 99  X^2 20 * + RCL 99 3 * + 300 -   RCL 99");
                fnStore(10);
                fnDrop(0);
                fnStrtoX("X^3 + 20X^2 + 3X - 300 = 0");
                fnStore(11);
                fnDrop(0);

                fnStrtoX("XEQC43 STO 99 SIN RCL 99 X^2 + RCL 99");
                fnStore(12);
                fnDrop(0);
                fnStrtoX("sin(x) + sin(x^2) = 0");
                fnStore(13);
                fnDrop(0);

                fnStrtoX("XEQC43 STO 99 1 ENTER 3 / Y^X 1 + RCL 99");
                fnStore(14);
                fnDrop(0);
                fnStrtoX("x^(1/3) + 1 = 0");
                fnStore(15);
                fnDrop(0);

                fnStrtoX("XEQC43 STO 99 5 Y^X RCL 99 - 1 + RCL 99");
                fnStore(16);
                fnDrop(0);
                fnStrtoX("x^5 - x + 1 = 0");
                fnStore(17);
                fnDrop(0);

                fnStrtoX("XEQC43 STO 99 X^2 1 + RCL 99");
                fnStore(18);
                fnDrop(0);
                fnStrtoX("x^2 + 1 = 0");
                fnStore(19);
                fnDrop(0);

                break;
      #endif //SAVE_SPACE_DM42_4
  	  default:;
     }

  running_program_jm = false;

}




