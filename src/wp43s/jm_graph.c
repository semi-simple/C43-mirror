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

/* ADDITIONAL WP43C functions and routines */


#include "wp43s.h"


//-----------------------------------------------------



void Fn_Lbl_A(void) {                                   //Temporary RPN function
	  fnAngularMode(AM_RADIAN);                           //Does not belong here -- it is repeated. It is convenient.
    fnStore(99);             // STO 99

    // (sin(x)/x + sin(10x)/5) / 2 + 2/x 
    fnSin(0);                // SIN
    fnRCL(99);               // RCL 99
    fnDivide(0);             // /
 
    fnRCL(99);               // RCL 99
    fnStrInput2("10");       // 10
    fnMultiply(0);           // *
    fnSin(0);                // SIN
    fnStrInput2("5");        // 5
    fnDivide(0);             // /
 
    fnAdd(0);                // +    

    fnStrInput2("2");        // 2
    fnDivide(0);             // /

    fnStrInput2("2");        // 2
    fnRCL(99);               // RCL 99
    fnDivide(0);             // /
    fnAdd(0);                // +    
}



void Fn_Lbl_B(void) {                                   //Temporary RPN function
	  fnAngularMode(AM_RADIAN);                           //Does not belong here -- it is repeated. It is convenient.
    fnStore(99);             // STO 99
    fnSin(0);                // SIN    
    fnRCL(99);
    fnSquare(0);             // square
    fnSin(0);                // SIN
    fnAdd(0);                // +    
}


void Fn_Lbl_C(void) {                                   //Temporary RPN function
    fnAngularMode(AM_RADIAN);                           //Does not belong here -- it is repeated. It is convenient.
    fnStore(99);             // STO 99
    fnSin(0);                // SIN    
    fnRCL(99);
    fnDivide(0);             // /

}



#ifndef TESTSUITE_BUILD
int16_t screen_window_x(float x_min, float x, float x_max) {
int16_t temp;
  temp = ((x-x_min)/(x_max-x_min)*SCREEN_WIDTH);
  if (temp>SCREEN_WIDTH-1) {temp=SCREEN_WIDTH-1;}
  else if (temp<0) {temp=0;}
  return temp;
}
#endif

int16_t screen_window_y(float y_min, float y, float y_max) {
int16_t temp;
  temp = (y-y_min)/(y_max-y_min)*(SCREEN_HEIGHT_GRAPH-1-SCREEN_MIN_GRAPH);
    if (temp>SCREEN_HEIGHT_GRAPH-SCREEN_MIN_GRAPH-1) {temp=SCREEN_HEIGHT_GRAPH-SCREEN_MIN_GRAPH-1;}
  else if (temp<0) {temp=0;}  
  return (SCREEN_HEIGHT_GRAPH - temp);
}


void graph_draw(uint8_t nbr, float x_min, float x_max, float y_min, float y_max, float tick_x, float tick_y, uint16_t xzero, uint8_t yzero) {
  #ifndef TESTSUITE_BUILD
  uint16_t cnt;
  real_t tmpy;
  uint8_t yo; 
  uint8_t yn;
  yn = 0;
  float x; 
  float y;
  uint16_t x1;  //range 0-399
  uint8_t  y1;  //range 0-239

  for(y=y_min; y<=y_max; y+=tick_y) {
    cnt = screen_window_y(y_min,y,y_max);
    setPixel(xzero-1,cnt); //tick
    setPixel(xzero-2,cnt); //tick
    setPixel(xzero+1,cnt); //tick
    setPixel(xzero+1,cnt); //tick
  }  

  //GRAPH
  cnt = 0;
  for(x=x_min; x<=x_max; x+=(x_max-x_min)/SCREEN_WIDTH) {

    float a_ft = (x/( tick_x));          //Draw ticks
    if(a_ft<0) { a_ft=-a_ft; }
    int16_t a_int = (int) a_ft;
    float a_frac = a_ft - a_int;
    if(a_frac < (x_max-x_min)/300) {               //Frac < 6.6 % is deemed close enough
      setPixel(cnt,yzero+1); //tick
      setPixel(cnt,yzero-1); //tick
      setPixel(cnt,yzero+2); //tick
      setPixel(cnt,yzero-2); //tick
      force_refresh();
    }

    //convert float to X register
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
    snprintf(tmpStr3000, sizeof(tmpStr3000), "%.16f", x);
    stringToReal34(tmpStr3000, REGISTER_REAL34_DATA(REGISTER_X));


    if(nbr == 1) {
      Fn_Lbl_A();
    } else
    if(nbr == 2) {
      Fn_Lbl_B();
    }
    if(nbr == 3) {
      Fn_Lbl_C();
    }

    //Convert from X register to float
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &tmpy);
    realToString(&tmpy, tmpStr3000);
    y = strtof (tmpStr3000, NULL);

    yo = yn;   //old , new
    yn = screen_window_y(y_min,y,y_max);

    //printf("Calc: cnt = %d xy[%f %f]  yold->new(%d -> %d)\n",cnt, x, y, yo, yn);
    cnt++;


  if(cnt > 0) {       //Fill in all y coords if coords are skipped due to large dy/dx.
      #ifndef TESTSUITE_BUILD
      x1 = cnt-1;     //First half on cnt-1, second half on cnt. Not implemented yet., All on cnt-1.
      #endif
      if(yo > yn) {
        for(y1=yo; y1!=yn; y1-=1) {
          setPixel(x1,y1);
        }
      } 
      else if(yo < yn) {
        for(y1=yo; y1!=yn; y1+=1) {
        setPixel(x1,y1);
        }
      } else {
        setPixel(x1,yn);
      }
    setPixel(cnt,SCREEN_HEIGHT_GRAPH- (0)       );
    setPixel(cnt,SCREEN_HEIGHT_GRAPH- (SCREEN_HEIGHT_GRAPH-SCREEN_MIN_GRAPH-1) );
    }
  }
fnDrop(0);
#endif
}


float auto_tick(float tick_int_f) {
    //Obtain scaling of ticks, to about 20 intervals left to right.
  //float tick_int_f = (x_max-x_min)/20;                                                 //printf("tick interval:%f ",tick_int_f);
  snprintf(tmpStr3000, sizeof(tmpStr3000), "%.1e", tick_int_f);
  char tx[4];
  tx[0] = tmpStr3000[0];
  tx[1] = tmpStr3000[1];
  tx[2] = tmpStr3000[2];
  tx[3] = 0;
  //printf("tick0 %f orgstr %s tx %s \n",tick_int_f, tmpStr3000, tx);
  tick_int_f = strtof (tx, NULL);                                        //printf("string:%s converted:%f \n",tmpStr3000, tick_int_f);
  //printf("tick1 %f orgstr %s tx %s \n",tick_int_f, tmpStr3000, tx);
  if(tick_int_f > 0   && tick_int_f <=  0.3)  {tmpStr3000[0] = '0'; tmpStr3000[2]='2'; } else
  if(tick_int_f > 0.3 && tick_int_f <=  0.6)  {tmpStr3000[0] = '0'; tmpStr3000[2]='5'; } else
  if(tick_int_f > 0.6 && tick_int_f <=  1.3)  {tmpStr3000[0] = '1'; tmpStr3000[2]='0'; } else
  if(tick_int_f > 1.3 && tick_int_f <=  1.7)  {tmpStr3000[0] = '1'; tmpStr3000[2]='5'; } else
  if(tick_int_f > 1.7 && tick_int_f <=  3.0)  {tmpStr3000[0] = '2'; tmpStr3000[2]='0'; } else
  if(tick_int_f > 3.0 && tick_int_f <=  6.5)  {tmpStr3000[0] = '5'; tmpStr3000[2]='0'; } else
  if(tick_int_f > 6.5 && tick_int_f <=  9.9)  {tmpStr3000[0] = '7'; tmpStr3000[2]='5'; }
  tick_int_f = strtof (tmpStr3000, NULL);                                        //printf("string:%s converted:%f \n",tmpStr3000, tick_int_f);
  //printf("tick2 %f str %s tx %s \n",tick_int_f, tmpStr3000, tx);
  return tick_int_f;
}



void fnGraph (uint16_t unusedParamButMandatory){
  #ifndef TESTSUITE_BUILD
  uint16_t cnt;

  //GRAPH RANGE
  //  graph_xmin= -3*3.14150;  graph_xmax= 2*3.14159;
  //  graph_ymin= -2;          graph_ymax= +2;


  fnClearStack(0);

  //GRAPH SETUP
  calcMode = CM_BUG_ON_SCREEN;              //Hack to prevent calculator to restart operation. Used to view graph
  clearScreen(false,true,true);

  //GRAPH ZERO AXIS
  uint8_t  yzero;
  uint16_t xzero;
  yzero = screen_window_y(graph_ymin,0,graph_ymax);
  xzero = screen_window_x(graph_xmin,0,graph_xmax);

  //DRAW AXIS
  cnt = 0;  
  while (cnt!=SCREEN_WIDTH-1) { 
      setPixel(cnt,yzero); 
      cnt++; 
    }
  cnt = SCREEN_MIN_GRAPH;  
  while (cnt!=SCREEN_HEIGHT_GRAPH-1) { 
      setPixel(xzero,cnt); 
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


  snprintf(tmpStr3000, sizeof(tmpStr3000), "x: %.3f/div  y: %.3f/div", tick_int_x,tick_int_y);
  showString(tmpStr3000, &standardFont, 1, Y_POSITION_OF_REGISTER_T_LINE, vmNormal, true, true);  //JM

//  graph_draw(1, graph_xmin, graph_xmax, graph_ymin, graph_ymax, tick_int_x, tick_int_y, xzero, yzero);
//  graph_draw(2, graph_xmin, graph_xmax, graph_ymin, graph_ymax, tick_int_x, tick_int_y, xzero, yzero);
  graph_draw(3, graph_xmin, graph_xmax, graph_ymin, graph_ymax, tick_int_x, tick_int_y, xzero, yzero);

  #endif
//printf("\n");
//-----------------------------------------------------
}
