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
#ifdef OLDGRAPH
                    void graph_draw(uint8_t nbr, float x_min, float x_max, float y_min, float y_max, float tick_x, float tick_y, uint16_t xzero, uint8_t yzero) {
                      #ifndef TESTSUITE_BUILD
                      uint16_t cnt;
                      real_t tmpy;
                      uint8_t yo; 
                      uint8_t yn;
                      yn = 0;
                      double x; 
                      double y;
                      uint16_t x1;  //range 0-399
                      uint8_t  y1;  //range 0-239

                      for(y=y_min; y<=y_max; y+=tick_y) {
                        cnt = screen_window_y(y_min,y,y_max);
                        setBlackPixel(max(0,xzero-1),cnt); //tick
                        setBlackPixel(max(0,xzero-2),cnt); //tick
                        setBlackPixel(min(SCREEN_WIDTH_GRAPH-1,xzero+1),cnt); //tick
                        setBlackPixel(min(SCREEN_WIDTH_GRAPH-1,xzero+1),cnt); //tick
                      }  

                      //GRAPH
                      cnt = 0;
                      for(x=x_min; x<=x_max; x+=(x_max-x_min)/SCREEN_WIDTH_GRAPH) {

                        float a_ft = (x/( tick_x));          //Draw ticks
                        if(a_ft<0) { a_ft=-a_ft; }
                        int16_t a_int = (int) a_ft;
                        float a_frac = a_ft - a_int;
                        if(a_frac < (x_max-x_min)/300) {               //Frac < 6.6 % is deemed close enough
                          setBlackPixel(cnt,min(SCREEN_WIDTH_GRAPH-1,yzero+1)); //tick
                          setBlackPixel(cnt,max(SCREEN_MIN_GRAPH,yzero-1)); //tick
                          setBlackPixel(cnt,min(SCREEN_WIDTH_GRAPH-1,yzero+2)); //tick
                          setBlackPixel(cnt,max(SCREEN_MIN_GRAPH,yzero-2)); //tick
                          force_refresh();
                        }

                        //convert double to X register
                        reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
                        snprintf(tmpString, TMP_STR_LENGTH, "%.16e", x);
                        stringToReal34(tmpString, REGISTER_REAL34_DATA(REGISTER_X));

                        execute_rpn_function(nbr);

                        //Convert from X register to double
                        real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &tmpy);
                        realToString(&tmpy, tmpString);
                        y = strtof (tmpString, NULL);

                        yo = yn;   //old , new
                        yn = screen_window_y(y_min,y,y_max);

                        //printf("Calc: cnt = %d xy[%f %f]  yold->new(%d -> %d)\n",cnt, x, y, yo, yn);


                      if(cnt > 0) {       //Fill in all y coords if coords are skipped due to large dy/dx.
                          #ifndef TESTSUITE_BUILD
                          x1 = cnt-1;     //First half on cnt-1, second half on cnt. Not implemented yet., All on cnt-1.
                          #endif
                          if(yo > yn) {
                            for(y1=yo; y1!=yn; y1-=1) {
                              setBlackPixel(x1,y1);
                            }
                          } 
                          else if(yo < yn) {
                            for(y1=yo; y1!=yn; y1+=1) {
                            setBlackPixel(x1,y1);
                            }
                          } else {
                            setBlackPixel(x1,yn);
                          }
                        setBlackPixel(cnt,SCREEN_HEIGHT_GRAPH-1 );
                        setBlackPixel(cnt,SCREEN_MIN_GRAPH);
                        }
                      cnt++;                      
                      }
                    fnDrop(0);
                    #endif
                    }



                    void fnGraph_old (uint16_t selection){
                      #ifndef TESTSUITE_BUILD
                      uint16_t cnt;

                      //GRAPH RANGE
                      //  graph_xmin= -3*3.14150;  graph_xmax= 2*3.14159;
                      //  graph_ymin= -2;          graph_ymax= +2;

                      fnClearStack(0);

                      //GRAPH SETUP
                      calcMode = CM_BUG_ON_SCREEN;              //Hack to prevent calculator to restart operation. Used to view graph
                      clearScreen_old(false,true,true);

                      //GRAPH ZERO AXIS
                      uint8_t  yzero;
                      uint16_t xzero;
                      yzero = screen_window_y(graph_ymin,0,graph_ymax);
                      xzero = screen_window_x(graph_xmin,0,graph_xmax);

                      //DRAW AXIS
                      cnt = 0;  
                      while (cnt!=SCREEN_WIDTH_GRAPH-1) { 
                          setBlackPixel(cnt,yzero); 
                          cnt++; 
                        }
                      cnt = SCREEN_MIN_GRAPH;  
                      while (cnt!=SCREEN_HEIGHT_GRAPH-1) { 
                          setBlackPixel(xzero,cnt); 
                          cnt++; 
                        }

                      force_refresh();

                      float tick_int_x;
                      if(graph_dx == 0) {
                        tick_int_x = auto_tick((graph_xmax-graph_xmin)/20);
                      } else {
                        tick_int_x = graph_dx;
                      }

                      float tick_int_y;
                      if(graph_dy == 0) {
                        tick_int_y = auto_tick((graph_ymax-graph_ymin)/20);
                      } else {
                        tick_int_y = graph_dy;
                      }

                      snprintf(tmpString, TMP_STR_LENGTH, "x: %.3f/tick  y: %.3f/tick", tick_int_x,tick_int_y);
                      showString(tmpString, &standardFont, 1, Y_POSITION_OF_REGISTER_T_LINE, vmNormal, true, true);  //JM
                      graph_draw(selection, graph_xmin, graph_xmax, graph_ymin, graph_ymax, tick_int_x, tick_int_y, xzero, yzero);
                      #endif
                    }

                    //-----------------------------------------------------//-----------------------------------------------------

#endif //OLDGRAPH


//-----------------------------------------------------//-----------------------------------------------------

#define randnum(min, max)  ((rand() % (int)(((max) + 1) - (min))) + (min))


void fnGraph (uint16_t func){
  Aspect_Square = false;
  PLOT_INTG = false;
  PLOT_DIFF = false;
  PLOT_RMS  = false;
  fnClSigma(0);
  switch (func) 
  {

#ifdef OLDGRAPH
	  case 1:   fnGraph_old(3);
              break;
#endif //OLDGRAPH

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




