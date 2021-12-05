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
#include "solver/equation.h"
#include "items.h"
#include "mathematics/invert.h"
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




#define VERBOSE_SOLVER0  // a lot less text
//#define VERBOSE_SOLVER1  // a lot less text
//#define VERBOSE_SOLVER2  // verbose a lot





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
  #ifndef SAVE_SPACE_DM42_4
    double ix, ix0, ix1;  
  #endif
  switch (func) 
  {

	  case  3:  graph_demo(randnum(4,6), graph_xmin, graph_xmax);
	            break;
    #ifndef SAVE_SPACE_DM42_4

    #endif //SAVE_SPACE_DM42_4

    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:  graph_demo(func-10, graph_xmin, graph_xmax);
              break;


    #ifndef SAVE_SPACE_DM42_4
      case 22:
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




