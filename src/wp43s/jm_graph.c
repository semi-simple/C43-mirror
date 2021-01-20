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
    fnStore(99);             // STO 99
    fnSin(0);                // SIN    
    fnRCL(99);
    fnSquare(0);             // square
    fnSin(0);                // SIN
    fnAdd(0);                // +    
    fnRCL(99);               //leaving y in Y and x in X
#endif
}


void Fn_Lbl_C(void) {                                   //Temporary RPN function
#ifndef TESTSUITE_BUILD
    fnStore(99);             // STO 99
    runFunction(ITM_sinc);
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
  fnClSigma(0);
  switch (func) 
  {

	  case 3:   graph_demo(randnum(4,6), graph_xmin, graph_xmax);
	            break;
	  case 4:   graph_plotmem();
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




