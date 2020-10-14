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
 * \file graphs.c Graphing module
 ***********************************************/

/* C43 functions and routines written by JM, adapted from C43 to WP43S by JM */


#include "wp43s.h"
#include "math.h"

//#define STATDEBUG


//Note: graph_xmin, graph_xmax set from X.FN GRAPH
float    tick_int_x;
float    tick_int_y;
float    x_min; 
float    x_max;
float    y_min;
float    y_max;
uint16_t xzero;
uint8_t  yzero;



void graph_reset(){
  graph_dx      = 0;
  graph_dy      = 0;
  extentx       = false;
  extenty       = false;
  jm_VECT       = false;
  Aspect_Square = true;
  PLOT_LINE     = false;
  PLOT_CROSS    = false;
  PLOT_BOX      = true;
}


void fnPline(uint16_t unusedParamButMandatory) {
  PLOT_LINE = !PLOT_LINE;
  fnRefreshComboxState(CB_JC, JC_PLINE, PLOT_LINE);                //jm
  fnPlot(0);
}


void fnPcros(uint16_t unusedParamButMandatory) {
  PLOT_CROSS = !PLOT_CROSS;
  fnRefreshComboxState(CB_JC, JC_PCROS, PLOT_CROSS);                //jm
  fnPlot(0);
}


void fnPbox (uint16_t unusedParamButMandatory) {
  PLOT_BOX = !PLOT_BOX;
  fnRefreshComboxState(CB_JC, JC_PBOX, PLOT_BOX);                //jm
  fnPlot(0);
}

void fnPvect (uint16_t unusedParamButMandatory) {
  jm_VECT = !jm_VECT;
  fnRefreshComboxState(CB_JC, JC_VECT, jm_VECT);                //jm
  fnPlot(0);
}    

void fnPx (uint16_t unusedParamButMandatory) {
  extentx = !extentx;
  fnRefreshComboxState(CB_JC, JC_EXTENTX, extentx);                //jm
  fnPlot(0);
}

void fnPy (uint16_t unusedParamButMandatory) {
  extenty = !extenty;
  fnRefreshComboxState(CB_JC, JC_EXTENTY, extenty);                //jm
  fnPlot(0);
}

void fnPlot(uint16_t unusedParamButMandatory) {
  Aspect_Square = true;
  calcMode = CM_GRAPH;
  #ifndef TESTSUITE_BUILD
    showSoftmenu(NULL, -MNU_PLOT, true);
  #endif
  doRefreshSoftMenu = true;             //Plot graph is part of refreshScreen
}

void fnPlotLS(uint16_t unusedParamButMandatory) {
  Aspect_Square = false;
  calcMode = CM_GRAPH;
  doRefreshSoftMenu = true;             //Plot graph is part of refreshScreen
}

void fnPdemo1(uint16_t unusedParamButMandatory) {

PLOT_LINE = true;
PLOT_CROSS= false;
PLOT_BOX  = false;
extentx   = false;
extenty   = false;
jm_VECT   = false;
Aspect_Square = true;

//GRAPH  // Build test data 
float x_min = -1;
float x_max = 1;
  #ifndef TESTSUITE_BUILD
  double x; 
  fnClearStack(0);

  if(telltale != MEM_INITIALIZED) {
    graph_setupmemory();
    runFunction(ITM_CLSIGMA);
  }

  fnAngularMode(AM_MULTPI);

  //  for(x=x_min; x<=x_max; x+=(x_max-x_min)/SCREEN_WIDTH_GRAPH) {
  for(x=x_min; x<=x_max; x+=0.99999*(x_max-x_min)/SCREEN_WIDTH_GRAPH*10) {    //Reduxced the amount of sample data from 400 points to 40 points

    //convert double to X register
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
    snprintf(tmpStr3000, sizeof(tmpStr3000), "%.16e", x);
    stringToReal34(tmpStr3000, REGISTER_REAL34_DATA(REGISTER_X));

    //leaving y in Y and x in X
    //execute_rpn_function(0);
    fnSigma(1);
  }
  fnClearStack(0);
  runFunction(ITM_SIGMAx);
  #endif
}







void graph_setupmemory(void) {
  int i;
  if(telltale != MEM_INITIALIZED) {
    gr_x = (double*)malloc(LIM * sizeof(double)); 
    gr_y = (double*)malloc(LIM * sizeof(double)); 
    telltale = MEM_INITIALIZED;
    ix_count = 0;
  }
  if ((telltale != MEM_INITIALIZED) || (gr_x == NULL || gr_y == NULL)) { 
  #ifdef PC_BUILD
     moreInfoOnError("In function graph_setupmemory:", "error allocating memory for graph!", NULL, NULL);
     exit(1);
  #endif
  } 
  
  if((telltale==MEM_INITIALIZED) && (gr_x != NULL) && (gr_y != NULL)){
    for (i = 0; i < LIM; ++i) { 
      gr_x[i] = 0;
      gr_y[i] = 0; 
    }
  }   
  
}


void graph_end(void) {
  free(gr_x);
  free(gr_y);
  telltale = 0;
}







void graph_sigmaplus(int8_t plusminus, real_t *xx, real_t *yy) {    //Called from STAT module from fnSigma(), to store the x,y pair to the memory structure.
  int16_t cnt;
  double x; 
  double y;

  if(jm_VECT) {plotmode = _VECT;} else {plotmode = _SCAT;}

  if(telltale != MEM_INITIALIZED) {
    graph_setupmemory();
//    runFunction(ITM_CLSIGMA);
  }


  //Convert from X register to double
  realToString(yy, tmpStr3000);
  y = strtof (tmpStr3000, NULL);

  //printf("y=%f ",y);

  //Convert from X register to double
  realToString(xx, tmpStr3000);
  x = strtof (tmpStr3000, NULL);

  //printf("x=%f ",x);

  #ifndef TESTSUITE_BUILD
  export_xy_to_file(x,y);     //Write to CSV file
  #endif

  if(plotmode == _VECT ) {
    ix_count++;               //Only used for VECT
    cnt = ix_count;
  } else {
    //Convert from real to int
    realToInt32(SIGMA_N, cnt);
    ix_count = cnt;                          //ix_count increments in VECT with Σ-, where SIGMA_N decrements with Σ- 
                                             //if VECT is changed mid-process, it will cause x_count to assume SIGMA_N, which  will throw away the last values stored.
//    realToString(SIGMA_N, tmpStr3000);
//    cnt = stringToInt16 (tmpStr3000);
    #ifdef STATDEBUG
    printf("Count: %s, %d\n",tmpStr3000,cnt);
    #endif

  }
  //printf("Adding to graph table[%d] = x:%f y:%f\n",cnt,x,y);

  if(plusminus == 1) {
    gr_x[cnt-1]=x;
    gr_y[cnt-1]=y;
    #ifdef STATDEBUG
    printf("Index: [%d]=(%f,%f)\n",cnt-1,x,y);
    #endif
  } else {
    if(plusminus == -1) {
      if(plotmode == _VECT ) {
        gr_x[cnt-1]=-x;
        gr_y[cnt-1]=-y;
        #ifdef STATDEBUG
        printf("Index: [%d]=(%f,%f)\n",cnt-1,-x,-y);
        #endif
      } else {
        // Non-vector mode TODO
      }
    }
  }
}


//###################################################################################
#ifndef TESTSUITE_BUILD
  int16_t screen_window_x(double x_min, double x, double x_max) {
    int16_t temp;
    if (Aspect_Square) {
      temp = (x-x_min)/(x_max-x_min)*(SCREEN_HEIGHT_GRAPH-1);
      if (temp>SCREEN_HEIGHT_GRAPH-1) {temp=SCREEN_HEIGHT_GRAPH-1;}
      else if (temp<0) {temp=0;}
      return temp+SCREEN_WIDTH-SCREEN_HEIGHT_GRAPH;
    } 
    else {  //FULL SCREEN
      temp = ((x-x_min)/(x_max-x_min)*(SCREEN_WIDTH_GRAPH-1));
      //printf("--> %d (%f %f)  ",temp, x_min,x_max);
      if (temp>SCREEN_WIDTH_GRAPH-1) {temp=SCREEN_WIDTH_GRAPH-1;}
      else if (temp<0) {temp=0;}
      //printf("--> %d \n",temp);
      #ifdef PC_BUILD
        if(temp<0 || temp>399) {printf("In function screen_window_x JM X EXCEEDED %d",temp);}
      #endif
      return temp;
    }
  }

  int16_t screen_window_y(double y_min, double y, double y_max) {
  int16_t temp, minn;
    if (!Aspect_Square) minn = SCREEN_MIN_GRAPH;
    else minn = 0;

    temp = (y-y_min)/(y_max-y_min)*(SCREEN_HEIGHT_GRAPH-1 - minn);
      if (temp>SCREEN_HEIGHT_GRAPH-1 - minn) {temp=SCREEN_HEIGHT_GRAPH-1 - minn;}
    else if (temp<0) {temp=0;}

    #ifdef PC_BUILD
      if(SCREEN_HEIGHT_GRAPH-1 - temp<0 || SCREEN_HEIGHT_GRAPH-1 - temp>239) {printf("In function screen_window_y JM Y EXCEEDED %d %d",temp,SCREEN_HEIGHT_GRAPH-1 - temp);}
    #endif
    return (SCREEN_HEIGHT_GRAPH-1 - temp);
  }

#endif


//###################################################################################
void placePixel(uint16_t x, uint8_t y) {
#ifndef TESTSUITE_BUILD
  int16_t minn;
  if (!Aspect_Square) minn = SCREEN_MIN_GRAPH;
  else minn = 0;
    
  if(x<SCREEN_WIDTH_GRAPH && x>0 && y<SCREEN_HEIGHT_GRAPH && y>1+minn) {
    setPixel(x,y);
  }
#endif
}

void removePixel(uint16_t x, uint8_t y) {
#ifndef TESTSUITE_BUILD
  int16_t minn;
  if (!Aspect_Square) minn = SCREEN_MIN_GRAPH;
  else minn = 0;

  if(x<SCREEN_WIDTH_GRAPH && x>0 && y<SCREEN_HEIGHT_GRAPH && y>1+minn) {
    clearPixel(x,y);
  }
#endif
}

void clearScreenPixels() {
  #ifdef PC_BUILD
    int16_t x, y;

    if (Aspect_Square) {
      for(y=0; y<SCREEN_HEIGHT_GRAPH; y++) {
        for(x=SCREEN_WIDTH-SCREEN_HEIGHT_GRAPH; x<SCREEN_WIDTH; x++) {
         clearPixel(x, y);
        }
      }
      for(y=SCREEN_MIN_GRAPH; y<171; y++) {
        for(x=0; x<SCREEN_WIDTH-SCREEN_HEIGHT_GRAPH; x++) {
         clearPixel(x, y);
        }
      }
    } else {
      for(y=SCREEN_MIN_GRAPH; y<SCREEN_HEIGHT_GRAPH; y++) {
        for(x=0; x<SCREEN_WIDTH; x++) {
         clearPixel(x, y);
        }
      }
    }  


  #endif

  #if DMCP_BUILD
    if (Aspect_Square)
      lcd_fill_rect(SCREEN_WIDTH-SCREEN_HEIGHT_GRAPH, 0, SCREEN_HEIGHT_GRAPH, SCREEN_HEIGHT_GRAPH, 0);
    else
      lcd_fill_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT_GRAPH, 0);

  #endif
}                                                       //JM ^^


//###################################################################################
float auto_tick(float tick_int_f) {
    //Obtain scaling of ticks, to about 20 intervals left to right.
  //double tick_int_f = (x_max-x_min)/20;                                                 //printf("tick interval:%f ",tick_int_f);
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


//###################################################################################
void graph_axis (void){
  #ifndef TESTSUITE_BUILD
  uint16_t cnt;
  uint16_t ypos = Y_POSITION_OF_REGISTER_T_LINE -11 + 12 * 5;

  //GRAPH RANGE
  //  graph_xmin= -3*3.14150;  graph_xmax= 2*3.14159;
  //  graph_ymin= -2;          graph_ymax= +2;


  clearScreenPixels();

  graph_dx = 0; //XXX override manual setting from GRAPH to auto, temporarily. Can program these to fixed values.
  graph_dy = 0;

  if(graph_dx == 0) {
    tick_int_x = auto_tick((x_max-x_min)/20);
  } else {
    tick_int_x = graph_dx;
  }

  if(graph_dy == 0) {
    tick_int_y = auto_tick((y_max-y_min)/20);
  } else {
    tick_int_y = graph_dy;
  }


  uint16_t ii;
  uint16_t oo;
  char outstr[300];
  void convertDigits(void) {
    switch (tmpStr3000[ii]) {
      case  48: outstr[oo++] = 0xa0; outstr[oo++] = 0x80; break; //.
      case  49: outstr[oo++] = 0xa0; outstr[oo++] = 0x81; break; //.
      case  50: outstr[oo++] = 0xa0; outstr[oo++] = 0x82; break; //.
      case  51: outstr[oo++] = 0xa0; outstr[oo++] = 0x83; break; //.
      case  52: outstr[oo++] = 0xa0; outstr[oo++] = 0x84; break; //.
      case  53: outstr[oo++] = 0xa0; outstr[oo++] = 0x85; break; //.
      case  54: outstr[oo++] = 0xa0; outstr[oo++] = 0x86; break; //.
      case  55: outstr[oo++] = 0xa0; outstr[oo++] = 0x87; break; //.
      case  56: outstr[oo++] = 0xa0; outstr[oo++] = 0x88; break; //.
      case  57: outstr[oo++] = 0xa0; outstr[oo++] = 0x89; break; //.

      case 120: outstr[oo++] = 0xa4; outstr[oo++] = 0xb3; break; //x
      case 121: outstr[oo++] = 0xa4; outstr[oo++] = 0xb4; break; //y
      case 'a': outstr[oo++] = 0xa4; outstr[oo++] = 0x9c; break; //a
      case 's': outstr[oo++] = 0xa4; outstr[oo++] = 0xae; break; //s
      case  58: outstr[oo++] = 0xa2; outstr[oo++] = 0x36; break; //:
      case  43: outstr[oo++] = 0xa0; outstr[oo++] = 0x8a; break; //+
      case  45: outstr[oo++] = 0xa0; outstr[oo++] = 0x8b; break; //-
      case  46: outstr[oo++] = 0xa0; outstr[oo++] = 0x1a; break; //.
      case  47: outstr[oo++] = 0xa4; outstr[oo++] = 0x25; break; ///
      case 116: outstr[oo++] = 0xa0; outstr[oo++] = 0x9c; break; //t
      case 105: outstr[oo++] = 0xa4; outstr[oo++] = 0xa4; break; //i
      case 99 : outstr[oo++] = 0xa4; outstr[oo++] = 0x9e; break; //c
      case 107: outstr[oo++] = 0xa4; outstr[oo++] = 0xa6; break; //k
      default : outstr[oo++] = tmpStr3000[ii];
    }
  }


  snprintf(tmpStr3000, sizeof(tmpStr3000), "y %.3f/tick  ",tick_int_y);
  ii = 0;
  oo = 0;
  outstr[0]=0;
  while (tmpStr3000[ii] != 0) {
    convertDigits();
    ii++;
  }
  outstr[oo]=0;
  showString(outstr, &standardFont, 1, ypos, vmNormal, true, true);  //JM
  ypos -= 12;


  snprintf(tmpStr3000, sizeof(tmpStr3000), "x %.3f/tick  ", tick_int_x);
  ii = 0;
  oo = 0;
  outstr[0]=0;
  while (tmpStr3000[ii] != 0) {
    convertDigits();
    ii++;
  }
  outstr[oo]=0;
  showString(outstr, &standardFont, 1, ypos, vmNormal, true, true);  //JM
  ypos -= 12;



  snprintf(tmpStr3000, sizeof(tmpStr3000), "axis    0,0");
  ii = 0;
  oo = 0;
  outstr[0]=0;
  while (tmpStr3000[ii] != 0) {
    convertDigits();
    ii++;
  }
  outstr[oo]=0;
  showString(outstr, &standardFont, 1, ypos, vmNormal, true, true);  //JM
  ypos -= 12;



  //GRAPH ZERO AXIS
  yzero = screen_window_y(y_min,0,y_max);
  xzero = screen_window_x(x_min,0,x_max);

  //DRAW AXIS
  if (Aspect_Square) {
    cnt = SCREEN_WIDTH-SCREEN_HEIGHT_GRAPH;
  } else {
    cnt = 0;
  }  
  while (cnt!=SCREEN_WIDTH_GRAPH-1) { 
      setPixel(cnt,yzero); 
      cnt++; 
    }

  int16_t minn;
  if (!Aspect_Square) minn = SCREEN_MIN_GRAPH;
  else minn = 0;


  cnt = minn;  
  while (cnt!=SCREEN_HEIGHT_GRAPH) { 
      setPixel(xzero,cnt); 

      if (Aspect_Square) {
        setPixel(SCREEN_WIDTH-SCREEN_HEIGHT_GRAPH-1,cnt);   
        setPixel(SCREEN_WIDTH-SCREEN_HEIGHT_GRAPH-2,cnt);   
      }
      cnt++; 
    }



  force_refresh();



  double x; 
  double y;
  for(y=0; y<=y_max; y+=tick_int_y) {       //draw y ticks
    cnt = screen_window_y(y_min,y,y_max);
    setPixel(max(xzero-1,0),cnt); //tick
    setPixel(min(xzero+1,SCREEN_WIDTH_GRAPH-1),cnt); //tick
  }  
  for(y=0; y>=y_min; y+=-tick_int_y) {       //draw y ticks
    cnt = screen_window_y(y_min,y,y_max);
    setPixel(max(xzero-1,0),cnt); //tick
    setPixel(min(xzero+1,SCREEN_WIDTH_GRAPH-1),cnt); //tick
  }  

  for(x=0; x<=x_max; x+=tick_int_x) {       //draw x ticks
    cnt = screen_window_x(x_min,x,x_max);
      setPixel(cnt,min(yzero+1,SCREEN_HEIGHT_GRAPH-1)); //tick
      setPixel(cnt,max(yzero-1,minn)); //tick
   }
  for(x=0; x>=x_min; x+=-tick_int_x) {       //draw x ticks
    cnt = screen_window_x(x_min,x,x_max);
      setPixel(cnt,min(yzero+1,SCREEN_HEIGHT_GRAPH-1)); //tick
      setPixel(cnt,max(yzero-1,minn)); //tick
   }

  for(y=0; y<=y_max; y+=tick_int_y*5) {       //draw y ticks
    cnt = screen_window_y(y_min,y,y_max);
    setPixel(max(xzero-2,0),cnt); //tick
    setPixel(min(xzero+2,SCREEN_WIDTH_GRAPH-1),cnt); //tick
    setPixel(max(xzero-3,0),cnt); //tick
    setPixel(min(xzero+3,SCREEN_WIDTH_GRAPH-1),cnt); //tick
  }  
  for(y=0; y>=y_min; y+=-tick_int_y*5) {       //draw y ticks
    cnt = screen_window_y(y_min,y,y_max);
    setPixel(max(xzero-2,0),cnt); //tick
    setPixel(min(xzero+2,SCREEN_WIDTH_GRAPH-1),cnt); //tick
    setPixel(max(xzero-3,0),cnt); //tick
    setPixel(min(xzero+3,SCREEN_WIDTH_GRAPH-1),cnt); //tick
  }  

  for(x=0; x<=x_max; x+=tick_int_x*5) {       //draw x ticks
    cnt = screen_window_x(x_min,x,x_max);
      setPixel(cnt,min(yzero+2,SCREEN_HEIGHT_GRAPH-1)); //tick
      setPixel(cnt,max(yzero-2,minn)); //tick
      setPixel(cnt,min(yzero+3,SCREEN_HEIGHT_GRAPH-1)); //tick
      setPixel(cnt,max(yzero-3,minn)); //tick
   }
  for(x=0; x>=x_min; x+=-tick_int_x*5) {       //draw x ticks
    cnt = screen_window_x(x_min,x,x_max);
      setPixel(cnt,min(yzero+2,SCREEN_HEIGHT_GRAPH-1)); //tick
      setPixel(cnt,max(yzero-2,minn)); //tick
      setPixel(cnt,min(yzero+3,SCREEN_HEIGHT_GRAPH-1)); //tick
      setPixel(cnt,max(yzero-3,minn)); //tick
   }

  force_refresh();
  #endif
}


void plotline(uint16_t xo, uint8_t yo, uint16_t xn, uint8_t yn) {              // Plots line from xo,yo to xn,yn; uses temporary x1,y1
   pixelline(xo,yo,xn,yn,1);
 }


void pixelline(uint16_t xo, uint8_t yo, uint16_t xn, uint8_t yn, bool_t vmNormal) {              // Plots line from xo,yo to xn,yn; uses temporary x1,y1
    uint16_t x1;  //range 0-399
    uint8_t  y1;  //range 0-239
    #ifdef STATDEBUG
    printf("%d %d   %d %d \n",xo,yo,xn,yn);
    #endif
    if(xo > xn) {
      for(x1=xo; x1!=xn; x1-=1) {
        y1 = yo + (x1-xo)*(yn-yo)/(xn-xo);
        if(vmNormal) placePixel(x1,y1); else removePixel(x1,y1);
      }
    } 
    else if(xo < xn) {
      for(x1=xo; x1!=xn; x1+=1) {
        y1 = yo + (x1-xo)*(yn-yo)/(xn-xo);
        if(vmNormal) placePixel(x1,y1); else removePixel(x1,y1);
      }
    }
    if(yo > yn) {
      for(y1=yo; y1!=yn; y1-=1) {
        x1 = xo + (y1-yo)*(xn-xo)/(yn-yo);
        if(vmNormal) placePixel(x1,y1); else removePixel(x1,y1);
      }
    } 
    else if(yo < yn) {
      for(y1=yo; y1!=yn; y1+=1) {
        x1 = xo + (y1-yo)*(xn-xo)/(yn-yo);
        if(vmNormal) placePixel(x1,y1); else removePixel(x1,y1);
      }
    } else {
        if(vmNormal) placePixel(xn,yn); else removePixel(xn,yn);
    }
  }


//####################################################
//######### PLOT MEM #################################
//####################################################

void graph_plotmem(void) {
  #ifndef TESTSUITE_BUILD

      void plotarrow(uint16_t xo, uint8_t yo, uint16_t xn, uint8_t yn) {              // Plots line from xo,yo to xn,yn; uses temporary x1,y1
        float dx, dy, ddx, ddy, zz, zzz;
        ddy = yn-yo;
        ddx = xn-xo;
        zz  = sqrt(ddy*ddy+ddx*ddx);
        zzz = 3;
        dy  = ddy * (zzz/zz);
        dx  = ddx * (zzz/zz);
        #ifdef STATDEBUG
        printf("%d %d  %d %d  ddx=%f, ddy=%f, zz=%f  zzz=%f, dx=%f, dy=%f \n",xo, yo, xn, yn, ddx,ddy,zz,zzz,dx,dy);
        #endif
        if (!(xo==xn && yo==yn)){
          plotline(xn+(-3*dx +dy), yn+(-3*dy -dx), xn, yn);
          plotline(xn+(-3*dx -dy), yn+(-3*dy +dx), xn, yn);
        } else {
          placePixel(xn,yn);
        }
      }


      void plotcross(uint16_t xn, uint8_t yn) {              // Plots line from xo,yo to xn,yn; uses temporary x1,y1
        placePixel(xn,yn);                     //PLOT a cross
        placePixel(xn-1,yn-1);
        placePixel(xn-1,yn+1);
        placePixel(xn+1,yn-1);
        placePixel(xn+1,yn+1);
        placePixel(xn-2,yn-2);
        placePixel(xn-2,yn+2);
        placePixel(xn+2,yn-2);
        placePixel(xn+2,yn+2);
      }

      void plotbox(uint16_t xn, uint8_t yn) {              // Plots line from xo,yo to xn,yn; uses temporary x1,y1
        placePixel(xn-2,yn-2);
        placePixel(xn-2,yn-1);
        placePixel(xn-1,yn-2);
        placePixel(xn-2,yn+2);
        placePixel(xn-2,yn+1);
        placePixel(xn-1,yn+2);
        placePixel(xn+2,yn-2);
        placePixel(xn+1,yn-2);
        placePixel(xn+2,yn-1);
        placePixel(xn+2,yn+2);
        placePixel(xn+2,yn+1);
        placePixel(xn+1,yn+2);
      }


  uint16_t cnt, ix, statnum;
  uint16_t xo, xn, xN; 
  uint8_t yo, yn, yN;
  double x; 
  double y;
  double sx, sy;

  statnum = 0;
  if(jm_VECT) {plotmode = _VECT;} else {plotmode = _SCAT;}

  if(telltale == MEM_INITIALIZED) {

    runFunction(ITM_NSIGMA);

    if(plotmode != _VECT) {
      //Convert from real to int
      realToInt32(SIGMA_N, statnum);
      //realToString(SIGMA_N, tmpStr3000);
      //statnum = stringToInt16 (tmpStr3000);
    } else {
      statnum = ix_count;
    }
   
    runFunction(ITM_DROP);

    #ifdef STATDEBUG
    printf("n=%d\n",statnum);
    #endif 

  }


  if(telltale == MEM_INITIALIZED && statnum >= 2) {
    //GRAPH SETUP
//    if(!Aspect_Square) {
//      clearScreen();
  //    refreshStatusBar();
//    }

    //AUTOSCALE
    x_min = 1e38;
    x_max = -1e38;
    y_min = 1e38;
    y_max = -1e38;
    #ifdef STATDEBUG
    printf("Axis0: x: %f -> %f y: %f -> %f   \n",x_min, x_max, y_min, y_max);   
    #endif
    if(plotmode != _VECT) {
      for(cnt=0; (cnt < LIM && cnt < statnum); cnt++) {
        #ifdef STATDEBUG
        printf("Axis0a: x: %f y: %f   \n",gr_x[cnt], gr_y[cnt]);   
        #endif
        if(gr_x[cnt] < x_min) {x_min = gr_x[cnt];}
        if(gr_x[cnt] > x_max) {x_max = gr_x[cnt];}
        if(gr_y[cnt] < y_min) {y_min = gr_y[cnt];}
        if(gr_y[cnt] > y_max) {y_max = gr_y[cnt];}
        #ifdef STATDEBUG
        printf("Axis0b: x: %f -> %f y: %f -> %f   \n",x_min, x_max, y_min, y_max);   
        #endif
      }
    } else {
      sx =0;
      sy =0;
      for(cnt=0; (cnt < LIM && cnt < statnum); cnt++) {            //### Note XXX E- will stuff up statnum!
        sx = sx + gr_x[cnt];
        sy = sy + gr_y[cnt];
        if(sx < x_min) {x_min = sx;}
        if(sx > x_max) {x_max = sx;}
        if(sy < y_min) {y_min = sy;}
        if(sy > y_max) {y_max = sy;}
      }      
    }


    #ifdef STATDEBUG
    printf("Axis1a: x: %f -> %f y: %f -> %f   \n",x_min, x_max, y_min, y_max);   
    #endif

    //Check and correct if min and max is swapped
    if(x_min>0 && x_min > x_max*0.99) {x_min = x_min - (-x_max+x_min)* 1.1;}
    if(x_min<0 && x_min > x_max*0.99) {x_min = x_min + (-x_max+x_min)* 1.1;}


    #ifdef STATDEBUG
    printf("Axis1b: x: %f -> %f y: %f -> %f   \n",x_min, x_max, y_min, y_max);   
    #endif


    //Always include the 0 axis
    if(!extentx) {
      if(x_min>0 && x_max>0) {if(x_min<=x_max) {x_min = -0.05*x_max;} else {x_min = 0;}}
      if(x_min<0 && x_max<0) {if(x_min>=x_max) {x_min = -0.05*x_max;} else {x_max = 0;}}
    }
    if(!extenty) {
      if(y_min>0 && y_max>0) {if(y_min<=y_max) {y_min = -0.05*y_max;} else {y_min = 0;}}
      if(y_min<0 && y_max<0) {if(y_min>=y_max) {y_min = -0.05*y_max;} else {y_max = 0;}}
    }
    
    #ifdef STATDEBUG
    printf("Axis2: x: %f -> %f y: %f -> %f   \n",x_min, x_max, y_min, y_max);   
    #endif

    //Create a visible axis on the edge
//    x_min = 1.05 * x_min;
//    x_max = 1.05 * x_max;
//    y_min = 1.05 * y_min;
//    y_max = 1.05 * y_max;

    float dx = x_max-x_min;
    float dy = y_max-y_min;

    if (dy == 0) {
      dy = 1;
      y_max = y_min + dy/2;
      y_min = y_max - dy;
    }
    if (dx == 0) {
      dx = 1;
      x_max = x_min + dx/2;
      x_min = x_max - dx;
    }
   
    x_min = x_min - dx * 0.015;
    y_min = y_min - dy * 0.015;
    x_max = x_max + dx * 0.015;
    y_max = y_max + dy * 0.015;


    #ifdef STATDEBUG
    printf("Axis3: x: %f -> %f y: %f -> %f   \n",x_min, x_max, y_min, y_max);   
    #endif

    graph_axis();

    if(plotmode != _VECT) {
      yn = screen_window_y(y_min,gr_y[0],y_max);
      xn = screen_window_x(x_min,gr_x[0],x_max);
      xN = xn;
      yN = yn;
    } else {
      yn = screen_window_y(y_min,0,y_max);
      xn = screen_window_x(x_min,0,x_max);
      xN = xn;
      yN = yn;      
    }

    sx = 0;
    sy = 0;
    //GRAPH
    ix = 0;
    for (ix = 0; (ix < LIM && ix < statnum); ++ix) {
      if(plotmode != _VECT) {
        x = gr_x[ix];
        y = gr_y[ix];
      } else {
        sx = sx + gr_x[ix];
        sy = sy + gr_y[ix];
        x = sx;
        y = sy;
      }
      #ifdef STATDEBUG
      printf("plotting graph table[%d] = x:%f y:%f \n",ix,x,y);
      #endif
      xo = xN;
      yo = yN;
      xN = screen_window_x(x_min,x,x_max);
      yN = screen_window_y(y_min,y,y_max);

      int16_t minny,minnx;
      if (!Aspect_Square) {minny = SCREEN_MIN_GRAPH; minnx = 0;}
      else {minny = 0; minnx = SCREEN_WIDTH-SCREEN_HEIGHT_GRAPH;}
      if(xN<SCREEN_WIDTH_GRAPH && xN>minnx && yN<SCREEN_HEIGHT_GRAPH && yN>1+minny) {
  //      yo = yn;                              //old , new, to be able to draw a line between samples
        yn = yN;
  //      xo = xn;
        xn = xN;


        if(plotmode != _VECT) {

          if(PLOT_CROSS)
            plotcross(xn,yn);
          else if(PLOT_BOX)
            plotbox(xn,yn);
          else placePixel(xn,yn);

        } else {
          plotarrow(xo, yo, xn, yn);
        }

        if(PLOT_LINE) plotline(xo, yo, xn, yn);

      }
    }


  } else {
    displayCalcErrorMessage(ERROR_NO_SUMMATION_DATA, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "There is no statistical data available!");
      moreInfoOnError("In function graph_plotmem:", errorMessage, NULL, NULL);
    #endif
  }
#endif
}




//-----------------------------------------------------//-----------------------------------------------------


void fnStatList(uint16_t unusedParamButMandatory) {
  char tmpstr1[100], tmpstr2[100];
  int16_t ix, ixx, statnum;

  #ifndef TESTSUITE_BUILD
  if(telltale == MEM_INITIALIZED) {
    //GRAPH SETUP
    calcMode = CM_BUG_ON_SCREEN;              //Hack to prevent calculator to restart operation. Used to view graph
    clearScreen();
    refreshStatusBar();
  }
  #endif

  if(jm_VECT) {plotmode = _VECT;} else {plotmode = _SCAT;}

  if(telltale == MEM_INITIALIZED) {

    #ifndef TESTSUITE_BUILD
    runFunction(ITM_NSIGMA);
    #endif

    if(plotmode != _VECT) {
      //Convert from real to int
      realToInt32(SIGMA_N, statnum);
      //realToString(SIGMA_N, tmpStr3000);
      //statnum = stringToInt16 (tmpStr3000);
    } else {
      statnum = ix_count;
    }

    #ifndef TESTSUITE_BUILD
    runFunction(ITM_DROP);
    #endif
   
    print_linestr("Stat data",true);
    #ifdef STATDEBUG
    printf("Stat data %d - %d\n",statnum-1, max(0, statnum-1-6) );
    #endif

    for (ix = 0; (ix < min(10,min(LIM, statnum))); ++ix) {
      ixx = statnum - ix - 1;

      if((fabs(gr_x[ixx]) > 0.0000001 && fabs(gr_x[ixx]) < 1000000)) 
        sprintf(tmpstr1,"[%3d] x%19.7f, ",ixx+1, gr_x[ixx]);
      else
        sprintf(tmpstr1,"[%3d] x%19.7e, ",ixx+1, round(gr_x[ixx]*1e10)/1e10);

      if((fabs(gr_y[ixx]) > 0.0000001 && fabs(gr_y[ixx]) < 1000000))
        sprintf(tmpstr2,"y%19.7f", gr_y[ixx]);
      else
        sprintf(tmpstr2,"y%19.7e", round(gr_y[ixx]*1e10)/1e10);

      strcat(tmpstr1,tmpstr2);

      print_numberstr(tmpstr1,false);
      #ifdef STATDEBUG
      printf("%d:%s\n",ixx,tmpstr1);
      #endif
    }
  }
}





