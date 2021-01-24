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
//GRAPH  // Build test data 
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





void doubleToXRegisterReal34(double x) {
    setSystemFlag(FLAG_ASLIFT);   // 5
    liftStack();
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
    snprintf(tmpString, TMP_STR_LENGTH, "%.16e", x);
    stringToReal34(tmpString, REGISTER_REAL34_DATA(REGISTER_X));
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



double convert_to_double(calcRegister_t regist) {
  double y;
  real_t tmpy; //Convert from X register to double
    real34ToReal(REGISTER_REAL34_DATA(regist), &tmpy);
    realToString(&tmpy, tmpString);
    y = strtof (tmpString, NULL);
    return y;
  }


#define VERBOSE_SOLVER1  // PLOT the actual converging values and a lot less text
#define VERBOSE_SOLVER2  // PLOT dthe differences, verbose a lot



//###################################################################################
//GRAPH  // Build test data 
void graph_solver(uint8_t nbr, float x_min, float x_max) {
  #ifndef TESTSUITE_BUILD
  int16_t ix;
  int16_t oscillations = 0; 
  int16_t convergent = 0; 
  bool_t checkend = false;
  cancelFilename = true;
  fnClearStack(0);

  if(telltale != MEM_INITIALIZED) {
    graph_setupmemory();
    runFunction(ITM_CLSIGMA);
  }

  fnAngularMode(AM_RADIAN);


    doubleToXRegisterReal34(0.0);
    fnStore(85);   //                //85: x old difference
    fnStore(86);   //                //86: y old difference
    fnStore(87);   //                //87: y0
    fnStore(88);   //                //88: x0
    fnStore(90);   //                //90: x1
    fnStore(91);   //                //91: x2
    fnStore(92);   //                //92: y1
    fnStore(93);   //                //93: y2
    fnStore(94);   //                //94: temporary new x2
    fnStore(95);   //                //95: x difference
    fnStore(96);   //                //96: y difference
    fnStore(97);   //                //97: faxctor
    fnStore(98);   //                //98: tolerance
    doubleToXRegisterReal34(1.0);
    fnStore(97);   // factor
    fnStore(95);   //initial value for difference comparison must be larger than tolerance
    fnStore(96);   //initial value for difference comparison must be larger than tolerance
    doubleToXRegisterReal34(1E-30); 
    fnStore(98);   //tolerance


    double startval = 1;

    doubleToXRegisterReal34(startval); //leftmost graph value
    fnStore(90); //x1   

    execute_rpn_function(nbr); //leaving y in y
    fnSwapXY(0); // moving y to x
    fnStore(92); // initial y1

    doubleToXRegisterReal34(startval + (x_max-x_min) / SCREEN_WIDTH_GRAPH);  //first increment
    fnStore(91); // initial x2

  ix = 0;
  int16_t kicker = 0;

//#################################################### Iteration start ##########################################
  while(ix<400 && !checkend && !((real34CompareAbsLessThan(REGISTER_REAL34_DATA(95), REGISTER_REAL34_DATA(98))) && (real34CompareAbsLessThan(REGISTER_REAL34_DATA(96), REGISTER_REAL34_DATA(98)))) ) {

//assumes X2 is in R91

    int16_t osc = 0;
    if(real34IsNegative(REGISTER_REAL34_DATA(95))) {                 //  If x increment is oscilating 5 times, in the first 10 cycles, 
      if(real34IsPositive(REGISTER_REAL34_DATA(85)))                 //  it is assumed that it is unstable and needs to have a 
        osc++;                                              //  complex starting value
    }
    if(real34IsPositive(REGISTER_REAL34_DATA(96))) {                 //
      if(real34IsNegative(REGISTER_REAL34_DATA(86)))                 //
        osc++;                                              //
    }

    if(getRegisterDataType(95) == dtComplex34 && getRegisterDataType(85) == dtComplex34) {
      if(real34IsNegative(REGISTER_IMAG34_DATA(95))) {                 //  If x increment is oscilating 5 times, in the first 10 cycles, 
        if(real34IsPositive(REGISTER_IMAG34_DATA(85)))                 //  it is assumed that it is unstable and needs to have a 
          osc++;                                              //  complex starting value
      }
    }
    if(getRegisterDataType(96) == dtComplex34 && getRegisterDataType(86) == dtComplex34) {
      if(real34IsPositive(REGISTER_IMAG34_DATA(96))) {                 //
        if(real34IsNegative(REGISTER_IMAG34_DATA(86)))                 //
          osc++;                                              //
      }
    }

   if(osc > 0) oscillations++; else
      oscillations = max(0,oscillations-1);


    if(real34CompareAbsLessThan(REGISTER_REAL34_DATA(95), REGISTER_REAL34_DATA(85)) && real34CompareAbsLessThan(REGISTER_REAL34_DATA(96), REGISTER_REAL34_DATA(86)) ) {
        convergent++;
    } else {
      convergent = max(0,convergent-1);
    }

    #ifdef VERBOSE_SOLVER1
      printf("################################### ix= %d osc= %d  conv= %d ###########################################\n",ix, oscillations, convergent);
    #endif //VERBOSE_SOLVER1

//assumes X2 is in R91
    if(((oscillations > 6) && (ix > 10) && (convergent < 5) )
       || (oscillations == 0 && convergent > 6 && real34CompareAbsLessThan(REGISTER_REAL34_DATA(95), const34_1e_4) && (getRegisterDataType(95) == dtComplex34 ? real34CompareAbsLessThan(REGISTER_IMAG34_DATA(95), const34_1e_4) : 1 )  )
    ) {                                  //
      oscillations = 0;
      convergent = 0;
      #ifdef VERBOSE_SOLVER2
        printf("\n>>>>>>>>>>A ");printf_cpx(91);
      #endif
      fnRCL(91);
      doubleToXRegisterReal34( - ++kicker / 100.0);                           //
      fnSquareRoot(0);                                               //
      doubleToXRegisterReal34(1.0);                           //
      fnAdd(0);
      #ifdef VERBOSE_SOLVER1
        printf("Kicked %d ",kicker);printf_cpx(REGISTER_X);printf("\n");
      #endif
      fnMultiply(0);              //add i/10000 just to force it complex  //
      fnStore(91); //replace X2 value                                //
      #ifdef VERBOSE_SOLVER2
        printf(">>>>>>>>>>H x 1+i0.1 = ");printf_cpx(91); printf("\n");
      #endif
    }

/*
  if(convergent < 2) {
    if(ix > 300) {doubleToXRegisterReal34(0.2); fnStore(97);} else //divide factor by 10
      if(ix > 60) {doubleToXRegisterReal34(0.45); fnStore(97);} else//divide factor by 10
        if(ix > 30) {doubleToXRegisterReal34(0.9); fnStore(97);} //divide factor by 10
  } else {
    if(convergent < 1) {
      if(ix >50) {doubleToXRegisterReal34(0.15); fnStore(97);} else //divide factor by 10
        if(ix >10) {doubleToXRegisterReal34(0.5); fnStore(97);} //divide factor by 10
    }
  }
*/

    
    

    fnRCL(91);
    //leaving y in Y and x in X
    execute_rpn_function(nbr);


    checkend = checkend || ix==400 ||
               ( real34IsZero(REGISTER_REAL34_DATA(REGISTER_X)) && (getRegisterDataType(REGISTER_X) == dtComplex34 ? real34IsZero(REGISTER_IMAG34_DATA(REGISTER_X)) : 1 )  &&
                 real34IsZero(REGISTER_REAL34_DATA(REGISTER_Y)) && (getRegisterDataType(REGISTER_Y) == dtComplex34 ? real34IsZero(REGISTER_IMAG34_DATA(REGISTER_Y)) : 1 ) )
                || 
                real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || (getRegisterDataType(REGISTER_X) == dtComplex34 ? real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X)) : 0 ) ||
                real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || (getRegisterDataType(REGISTER_Y) == dtComplex34 ? real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y)) : 0 ) 
                ; 

    if(checkend) {
       printf("-->A %d %d %d %d %d %d %d %d \n",
        real34IsZero(REGISTER_REAL34_DATA(REGISTER_X)),
        real34IsZero(REGISTER_IMAG34_DATA(REGISTER_X)),
        real34IsZero(REGISTER_REAL34_DATA(REGISTER_Y)),
        real34IsZero(REGISTER_IMAG34_DATA(REGISTER_Y)),
        real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)),
        real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X)),
        real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)),
        real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))
        );
    }




    #ifdef VERBOSE_SOLVER2
      printf("   ix=%d checkend=%d X2=",ix, checkend); printf_cpx(REGISTER_X); printf(" Y2="); printf_cpx(REGISTER_Y); printf("\n");
    #else
    if(!checkend)
      if(getRegisterDataType(REGISTER_X) == dtReal34 && getRegisterDataType(REGISTER_Y) == dtReal34)  fnSigma(1);
    #endif //VERBOSE_SOLVER2

    copySourceRegisterToDestRegister(REGISTER_Y,93); //y2


    copySourceRegisterToDestRegister(95,85);  //store old values
    copySourceRegisterToDestRegister(96,86);  //store old values
    fnRCL(91);     fnRCL(90); fnSubtract(0);  //dx
    fnStore(95);  //store difference for later
    fnRCL(93);     fnRCL(92); fnSubtract(0);  //dy
    fnStore(96);  //store difference for later

//Up to here the standard SECANT type slope is computed. If the following part is executed, a better slope is computed using 3 y values.


//Therefore, the second order accurate one-sided finite difference formula for the first derivative
//formule 32
//ChE 205 — Formulas for Numerical Differentiation
//Handout 5 05/08/02
if(ix>2 && convergent >= 2 && oscillations < 2) {
fnRCL(91); fnRCL(90); fnSubtract(0); //x2-x1
fnStore(95);  //store difference for later
fnRCL(90);
fnSwapXY(0);
fnSubtract(0);
fnStore(88); //make x0
execute_rpn_function(nbr);
copySourceRegisterToDestRegister(REGISTER_Y,87); //make y0
fnRCL(95); //x2-x1
doubleToXRegisterReal34(2);
fnMultiply(0); //2 delta x

fnRCL(87); //y0
fnRCL(92); //y1
doubleToXRegisterReal34(4);
fnMultiply(0);
fnSubtract(0);
fnRCL(93); //y2
doubleToXRegisterReal34(3);
fnMultiply(0);
fnAdd(0); //fi−2 − 4fi−1 + 3fi
}



    fnDivide(0);  //get the inverse of the slope

    #ifdef VERBOSE_SOLVER2
      fnInvert(0); printf(" SLOPE="); printf_cpx(REGISTER_X); printf("\n"); fnInvert(0); 
    #endif

    fnRCL(92);      //y1
    fnMultiply(0);  //increment to x
    fnRCL(97);      //factor to stabilize Newton method. factor=1 is straight. factor=0.1 converges 10x slower.
    fnMultiply(0);  //increment to x



    fnRCL(90);
    fnSwapXY(0);
    fnSubtract(0);  //subtract as per Newton, x1 - f/f'
    fnStore(94);    //to new x2


    copySourceRegisterToDestRegister(93,92); //old y2 copied to y1
    copySourceRegisterToDestRegister(91,90); //old x2 copied to x1

    fnRCL(95);    fnMagnitude(0); //difference dx
    fnRCL(96);    fnMagnitude(0); //difference dy


    checkend = checkend || 
               ( real34IsZero(REGISTER_REAL34_DATA(REGISTER_X)) && (getRegisterDataType(REGISTER_X) == dtComplex34 ? real34IsZero(REGISTER_IMAG34_DATA(REGISTER_X)) : 1 )  &&
                 real34IsZero(REGISTER_REAL34_DATA(REGISTER_Y)) && (getRegisterDataType(REGISTER_Y) == dtComplex34 ? real34IsZero(REGISTER_IMAG34_DATA(REGISTER_Y)) : 1 ) )
                || 
                real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || (getRegisterDataType(REGISTER_X) == dtComplex34 ? real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X)) : 0 ) ||
                real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || (getRegisterDataType(REGISTER_Y) == dtComplex34 ? real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y)) : 0 ) 
                ; 



    if(checkend) {
       printf("-->B %d %d %d %d %d %d %d %d \n",
        real34IsZero(REGISTER_REAL34_DATA(REGISTER_X)),
        real34IsZero(REGISTER_IMAG34_DATA(REGISTER_X)),
        real34IsZero(REGISTER_REAL34_DATA(REGISTER_Y)),
        real34IsZero(REGISTER_IMAG34_DATA(REGISTER_Y)),
        real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)),
        real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X)),
        real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)),
        real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))
        );
    }


    ix++;


    #ifdef VERBOSE_SOLVER2
      if(!checkend) {
        printf("   oldX2: ");printf_cpx(91); printf("\n");
        printf("   Y2   : ");printf_cpx(93); printf("\n");
        printf("   Difference X: ix=%d %d ",ix,real34CompareAbsLessThan(REGISTER_REAL34_DATA(95), REGISTER_REAL34_DATA(98)));printf_cpx(95); printf("\n");
        printf("   Difference Y: ix=%d %d ",ix,real34CompareAbsLessThan(REGISTER_REAL34_DATA(96), REGISTER_REAL34_DATA(98)));printf_cpx(96);
        printf("   Difference ABS: ");printf_cpx(REGISTER_X); printf("\n");
      }
    #endif //VERBOSE_SOLVER2

    copySourceRegisterToDestRegister(94,91);  //new x2    

    #ifdef VERBOSE_SOLVER1
      printf("   -------> newX2: ");printf_cpx(91); printf("\n");
    #endif //VERBOSE_SOLVER1

    #ifdef VERBOSE_SOLVER2      //plots the ix vs abs.difference under Verbose2
    if(!checkend) {
       //difference magnitude in Y
      doubleToXRegisterReal34((double)ix);
      if(getRegisterDataType(REGISTER_X) == dtReal34 && getRegisterDataType(REGISTER_Y) == dtReal34)  fnSigma(1);
    }
    #endif //VERBOSE_SOLVER2


  }
  fnClearStack(0);
  runFunction(ITM_NSIGMA);
  fnRCL(93); //y2
  fnRCL(91); //x2
  if( real34CompareAbsGreaterThan(REGISTER_REAL34_DATA(REGISTER_T), const34_1 )) runFunction(ITM_PLOT);
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




