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
bool_t invalid_intg = true;
bool_t invalid_diff = true;
  


void graph_reset(){
  graph_dx      = 0;
  graph_dy      = 0;
  extentx       = false;
  extenty       = false;
  jm_VECT       = false;
  jm_NVECT      = false;
  jm_SCALE      = false;
  Aspect_Square = true;
  PLOT_LINE     = true;
  PLOT_CROSS    = false;
  PLOT_BOX      = false;
  PLOT_INTG     = false;
  PLOT_DIFF     = false;
  PLOT_RMS      = false;
}


void fnPline(uint16_t unusedParamButMandatory) {
  PLOT_LINE = !PLOT_LINE;
  fnRefreshComboxState(CB_JC, JC_PLINE, PLOT_LINE);                //jm
  fnPlot(0);
}


void fnPcros(uint16_t unusedParamButMandatory) {
  PLOT_CROSS = !PLOT_CROSS;
  if(PLOT_CROSS) PLOT_BOX = false;
  fnRefreshComboxState(CB_JC, JC_PCROS, PLOT_CROSS);                //jm
  fnPlot(0);
}


void fnPbox (uint16_t unusedParamButMandatory) {
  PLOT_BOX = !PLOT_BOX;
  if(PLOT_BOX) PLOT_CROSS = false;
  fnRefreshComboxState(CB_JC, JC_PBOX, PLOT_BOX);                //jm
  fnPlot(0);
}

void fnPintg (uint16_t unusedParamButMandatory) {
  PLOT_INTG = !PLOT_INTG;
  fnRefreshComboxState(CB_JC, JC_INTG, PLOT_INTG);                //jm
  fnPlot(0);
}

void fnPdiff (uint16_t unusedParamButMandatory) {
  PLOT_DIFF = !PLOT_DIFF;
  fnRefreshComboxState(CB_JC, JC_DIFF, PLOT_DIFF);                //jm
  fnPlot(0);
}

void fnPrms (uint16_t unusedParamButMandatory) {
  PLOT_RMS = !PLOT_RMS;
  fnRefreshComboxState(CB_JC, JC_RMS, PLOT_RMS);                //jm
  fnPlot(0);
}

void fnPvect (uint16_t unusedParamButMandatory) {
  jm_VECT = !jm_VECT;
  if(jm_VECT) {jm_NVECT = false;}
  fnRefreshComboxState(CB_JC, JC_VECT, jm_VECT);                //jm
  fnPlot(0);
}    

void fnPNvect (uint16_t unusedParamButMandatory) {
  jm_NVECT = !jm_NVECT;
  if(jm_NVECT) {jm_VECT = false;}
  fnRefreshComboxState(CB_JC, JC_NVECT, jm_NVECT);                //jm
  fnPlot(0);
}    

void fnScale (uint16_t unusedParamButMandatory) {
  jm_SCALE = !jm_SCALE;
  fnRefreshComboxState(CB_JC, JC_SCALE, jm_SCALE);                //jm
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
  if(calcMode != CM_GRAPH){previousCalcMode = calcMode;}
  if(previousCalcMode == CM_GRAPH) {
    previousCalcMode = CM_NORMAL;
  }
  calcMode = CM_GRAPH;
  #ifndef TESTSUITE_BUILD
    if(softmenu[softmenuStack[softmenuStackPointer - 1].softmenu].menuId != -MNU_PLOT) {
      showSoftmenu(NULL, -MNU_PLOT, true);                         //JM MENU Prevent resetting the softmenu to the default no 1 page position
    }
  #endif
  doRefreshSoftMenu = true;             //Plot graph is part of refreshScreen
}

void fnPlotLS(uint16_t unusedParamButMandatory) {
  Aspect_Square = false;
  if(calcMode != CM_GRAPH){previousCalcMode = calcMode;}
  if(previousCalcMode == CM_GRAPH) {
    previousCalcMode = CM_NORMAL;
  }
  calcMode = CM_GRAPH;
  doRefreshSoftMenu = true;             //Plot graph is part of refreshScreen
}


void fnListXY(uint16_t unusedParamButMandatory) {
  #ifndef TESTSUITE_BUILD
    if(telltale == MEM_INITIALIZED) {
      calcMode = CM_LISTXY; //Used to view graph/listing
      ListXYposition = 0;
    }
  #endif
}


void graph_setupmemory(void) {
  int i;
  if(telltale != MEM_INITIALIZED) {
    gr_x = (float/*double*/*)malloc(LIM * sizeof(float/*double*/)); 
    gr_y = (float/*double*/*)malloc(LIM * sizeof(float/*double*/)); 
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


float/*double*/ grf_x(int i) {
  if (jm_NVECT) {return gr_y[i];}
  else {return gr_x[i];}
}

float/*double*/ grf_y(int i) {
  if (jm_NVECT) {return gr_x[i];}
  else {return gr_y[i];}
}



void graph_sigmaplus(int8_t plusminus, real_t *xx, real_t *yy) {    //Called from STAT module from fnSigma(), to store the x,y pair to the memory structure.
  int16_t cnt;
  float/*double*/ x; 
  float/*double*/ y;

  if(jm_VECT || jm_NVECT) {plotmode = _VECT;} else {plotmode = _SCAT;}

  if(telltale != MEM_INITIALIZED) {
    graph_setupmemory();
//    runFunction(ITM_CLSIGMA);
  }


  //Convert from X register to float/*double*/
  realToString(yy, tmpStr3000);
  y = strtof (tmpStr3000, NULL);

  //printf("y=%f ",y);

  //Convert from X register to float/*double*/
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
  int16_t screen_window_x(float/*double*/ x_min, float/*double*/ x, float/*double*/ x_max) {
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
        if(temp<0 || temp>399) {printf("In function screen_window_x X EXCEEDED %d",temp);}
      #endif
      return temp;
    }
  }

  int16_t screen_window_y(float/*double*/ y_min, float/*double*/ y, float/*double*/ y_max) {
  int16_t temp, minn;
    if (!Aspect_Square) minn = SCREEN_MIN_GRAPH;
    else minn = 0;

    temp = (y-y_min)/(y_max-y_min)*(SCREEN_HEIGHT_GRAPH-1 - minn);
      if (temp>SCREEN_HEIGHT_GRAPH-1 - minn) {temp=SCREEN_HEIGHT_GRAPH-1 - minn;}
    else if (temp<0) {temp=0;}

    #ifdef PC_BUILD
      if(SCREEN_HEIGHT_GRAPH-1 - temp<0 || SCREEN_HEIGHT_GRAPH-1 - temp>239) {printf("In function screen_window_y Y EXCEEDED %d %d",temp,SCREEN_HEIGHT_GRAPH-1 - temp);}
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
      for(y=Y_POSITION_OF_REGISTER_T_LINE /*+ 20*/; y<171; y++) {  //To just above the softmenu
        for(x=0; x<SCREEN_WIDTH-SCREEN_HEIGHT_GRAPH; x++) {
         if(!(y>171-5 && x<20)) clearPixel(x, y);                     //To allow to scroll up/down not to be deleted
        }
      }
    } 

    else {
      for(y=/*SCREEN_MIN_GRAPH*/0; y<SCREEN_HEIGHT_GRAPH; y++) {
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


      void plotdeltabig(uint16_t xn, uint8_t yn) {              // Plots ldifferential sign; uses temporary x1,y1
        plotline(xn+0,yn-2,     xn+0+5,yn-2+8);
        plotline(xn+0+5,yn-2+8, xn+0-5,yn-2+8);
        plotline(xn+0-5,yn-2+8, xn+0,yn-2    );
      }


      void plotdelta(uint16_t xn, uint8_t yn) {              // Plots ldifferential sign; uses temporary x1,y1
        placePixel(xn+0,yn-2);                               //   PLOT a delta
        placePixel(xn-1,yn-1);
        placePixel(xn-1,yn+0);
        placePixel(xn-2,yn+1);
        placePixel(xn-2,yn+2);
        placePixel(xn+1,yn-1);
        placePixel(xn+1,yn-0);
        placePixel(xn+2,yn+1);
        placePixel(xn+2,yn+2);
        placePixel(xn-1,yn+2);
        placePixel(xn  ,yn+2);
        placePixel(xn+1,yn+2);
      }

      void plotintbig(uint16_t xn, uint8_t yn) {                // Plots integral sign; uses temporary x1,y1
        plotline(xn-0,yn-2  ,     xn+3,yn-2);
        plotline(xn-0,yn-2+1,     xn+3,yn-2+1);
        plotline(xn-3,yn-2+8,     xn+0,yn-2+8);
        plotline(xn-3,yn-2+9,     xn+0,yn-2+9);

        plotline(xn+0,yn-2+7,     xn+0,yn-2+0);
        plotline(xn+1,yn-2+7,     xn+1,yn-2+0);
      }

      void plotint(uint16_t xn, uint8_t yn) {                // Plots integral sign; uses temporary x1,y1
        placePixel(xn,yn);                                   //   PLOT a I
        placePixel(xn,yn-1);
        placePixel(xn,yn-2);
        placePixel(xn,yn+1);
        placePixel(xn,yn+2);
        placePixel(xn+1,yn-2);
        placePixel(xn-1,yn+2);
      }

      void plotcross(uint16_t xn, uint8_t yn) {              // Plots line from xo,yo to xn,yn; uses temporary x1,y1
        placePixel(xn,yn);                                   //   PLOT a cross
        placePixel(xn-1,yn-1);
        placePixel(xn-1,yn+1);
        placePixel(xn+1,yn-1);
        placePixel(xn+1,yn+1);
        placePixel(xn-2,yn-2);
        placePixel(xn-2,yn+2);
        placePixel(xn+2,yn-2);
        placePixel(xn+2,yn+2);
      }

      void plotbox(uint16_t xn, uint8_t yn) {                // Plots line from xo,yo to xn,yn; uses temporary x1,y1
        placePixel(xn-2,yn-2);                               //   PLOT a box
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


      void plotrect(uint16_t a, uint8_t b, uint16_t c, uint8_t d) {                // Plots rectangle from xo,yo to xn,yn; uses temporary x1,y1
        plotline(a, b, c, b);
        plotline(a, b, a, d);
        plotline(c, d, c, b);
        plotline(c, d, a, d);
      }


      void plottriangle(uint16_t a, uint8_t b, uint16_t c, uint8_t d) {                // Plots rectangle from xo,yo to xn,yn; uses temporary x1,y1
        plotline(a, b, c, b);
        plotline(a, b, c, d);
        plotline(c, d, c, b);
      }




#endif



//###################################################################################
float auto_tick(float tick_int_f) {
    //Obtain scaling of ticks, to about 20 intervals left to right.
  //float/*double*/ tick_int_f = (x_max-x_min)/20;                                                 //printf("tick interval:%f ",tick_int_f);
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


  //GRAPH ZERO AXIS
  yzero = screen_window_y(y_min,0,y_max);
  xzero = screen_window_x(x_min,0,x_max);

  int16_t minnx, minny;
  if (!Aspect_Square) {
    minny = SCREEN_MIN_GRAPH;
    minnx = 0;
  }
  else {
    minny = 0;
    minnx = SCREEN_WIDTH-SCREEN_HEIGHT_GRAPH;
  }

  tmpStr3000[0]=0;                                  //If the axis is on the edge supress it, and label accordingly
  uint8_t axisdisp = 
           (!( yzero == SCREEN_HEIGHT_GRAPH-1  || yzero == minny) ? 2:0) + 
           (!(  xzero == SCREEN_WIDTH-1        || xzero == minnx) ? 1:0) ;
  switch( axisdisp )  {
    case 0: strcpy(tmpStr3000,"          "); break;
    case 1: snprintf(tmpStr3000, sizeof(tmpStr3000), "y-axis x 0"); break;
    case 2: snprintf(tmpStr3000, sizeof(tmpStr3000), "x-axis y 0"); break;
    case 3: snprintf(tmpStr3000, sizeof(tmpStr3000), "axis 0.0 "); break;
    default:break;
  }


  //Change to the small characters and fabricate a small = char
    ii = 0;
    oo = 0;
    outstr[0]=0;
    while (tmpStr3000[ii] != 0) {
      convertDigits();
      ii++;
    }
  outstr[oo]=0;
  ii=showString(outstr, &standardFont, 1, ypos, vmNormal, true, true);  //JM
  if(tmpStr3000[ stringByteLength(tmpStr3000)-1 ] == '0') {
    #define sp 15
    plotline((uint16_t)(ii-17),(uint8_t)(ypos+2+sp),(uint16_t)(ii-11),(uint8_t)(ypos+2+sp));
    plotline((uint16_t)(ii-17),(uint8_t)(ypos+1+sp),(uint16_t)(ii-11),(uint8_t)(ypos+1+sp));
    plotline((uint16_t)(ii-17),(uint8_t)(ypos-1+sp),(uint16_t)(ii-11),(uint8_t)(ypos-1+sp));
    plotline((uint16_t)(ii-17),(uint8_t)(ypos-2+sp),(uint16_t)(ii-11),(uint8_t)(ypos-2+sp));
  }
  ypos += 48;
  


  if(PLOT_INTG && !invalid_intg) {
    snprintf(tmpStr3000, sizeof(tmpStr3000), "  Trapezium integral");
    miniC = 1;
    showString(tmpStr3000, &numericFont, 1, ypos, vmNormal, true, true);  //JM
    miniC = 0;
    plotintbig( 5, ypos+4+4-2 );
    ypos += 20;
  }

  if(PLOT_DIFF && !invalid_diff) {
    snprintf(tmpStr3000, sizeof(tmpStr3000), "  Num. differential");
    miniC = 1;
    showString(tmpStr3000, &numericFont, 1, ypos, vmNormal, true, true);  //JM
    miniC = 0;
    plotdeltabig( 6, ypos+4+4-2);
    ypos += 20;
  }



  force_refresh();



  //SEPARATING LINE IF SQUARE
  cnt = minny;
  while (cnt!=SCREEN_HEIGHT_GRAPH) { 
    if (Aspect_Square) {
        setPixel(minnx-1,cnt);   
        setPixel(minnx-2,cnt);   
    }
    cnt++; 
  }

  #ifdef STATDEBUG
    printf("xzero=%d yzero=%d   \n",xzero,yzero);
  #endif


  float/*double*/ x; 
  float/*double*/ y;

  if( !(yzero == SCREEN_HEIGHT_GRAPH-1 || yzero == minny)) {
    //DRAW XAXIS
    if (Aspect_Square) {
      cnt = minnx;
    } else {
      cnt = 0;
    }  
    while (cnt!=SCREEN_WIDTH_GRAPH-1) { 
      setPixel(cnt,yzero); 
      cnt++; 
    }

   force_refresh();

   for(x=0; x<=x_max; x+=tick_int_x) {       //draw x ticks
      cnt = screen_window_x(x_min,x,x_max);
        setPixel(cnt,min(yzero+1,SCREEN_HEIGHT_GRAPH-1)); //tick
        setPixel(cnt,max(yzero-1,minny)); //tick
     }
    for(x=0; x>=x_min; x+=-tick_int_x) {       //draw x ticks
      cnt = screen_window_x(x_min,x,x_max);
        setPixel(cnt,min(yzero+1,SCREEN_HEIGHT_GRAPH-1)); //tick
        setPixel(cnt,max(yzero-1,minny)); //tick
     }
    for(x=0; x<=x_max; x+=tick_int_x*5) {       //draw x ticks
      cnt = screen_window_x(x_min,x,x_max);
        setPixel(cnt,min(yzero+2,SCREEN_HEIGHT_GRAPH-1)); //tick
        setPixel(cnt,max(yzero-2,minny)); //tick
        setPixel(cnt,min(yzero+3,SCREEN_HEIGHT_GRAPH-1)); //tick
        setPixel(cnt,max(yzero-3,minny)); //tick
     }
    for(x=0; x>=x_min; x+=-tick_int_x*5) {       //draw x ticks
      cnt = screen_window_x(x_min,x,x_max);
        setPixel(cnt,min(yzero+2,SCREEN_HEIGHT_GRAPH-1)); //tick
        setPixel(cnt,max(yzero-2,minny)); //tick
        setPixel(cnt,min(yzero+3,SCREEN_HEIGHT_GRAPH-1)); //tick
        setPixel(cnt,max(yzero-3,minny)); //tick
     }
  }



  if( !(xzero == SCREEN_WIDTH-1 || xzero == minnx)) {

    //Write North arrow
    if(jm_NVECT) {
      showString("N", &standardFont, xzero-4, minny+14, vmNormal, true, true);
      tmpStr3000[0]=0x80 | 0x22;
      tmpStr3000[1]=0x06;
      tmpStr3000[2]=0;
      showString(tmpStr3000, &standardFont, xzero-4, minny+0, vmNormal, true, true);
    }

    //DRAW YAXIS
    cnt = minny;
    while (cnt!=SCREEN_HEIGHT_GRAPH) { 
      setPixel(xzero,cnt); 
      cnt++; 
    }

    force_refresh();

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
    printf("pixelline: xo,yo,xn,yn: %d %d   %d %d \n",xo,yo,xn,yn);
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

  uint16_t cnt, ix, statnum;
  uint16_t xo, xn, xN; 
  uint8_t yo, yn, yN;
  float/*double*/ x; 
  float/*double*/ y;
  float/*double*/ sx, sy;
  float/*double*/ ddx = FLoatingMax;
  float/*double*/ ddy = FLoatingMax;
  float/*double*/ inty = 0;
  float/*double*/ inty0 = 0;
  float/*double*/ inty_off = 0;

//printf("TEST %d %d\n",screen_window_x(-0.405573,0.45,0.689633), screen_window_y(-0.405573,0.45,0.689633));
//printf("TEST %d %d\n",screen_window_x(0,1,1), screen_window_y(0,1,1));


  statnum = 0;

  graph_axis();                        //Draw the axis on any uncontrolled scale to start. Maybe optimize by remembering if there is an image on screen Otherwise double axis draw.

  if(jm_VECT || jm_NVECT) {plotmode = _VECT;} else {plotmode = _SCAT;}

  if(telltale == MEM_INITIALIZED) {

    runFunction(ITM_NSIGMA);

    if(plotmode != _VECT) {
      realToInt32(SIGMA_N, statnum);
    } else {
      statnum = ix_count;
    }
   
    runFunction(ITM_DROP);

    #ifdef STATDEBUG
    printf("statnum n=%d\n",statnum);
    #endif 
  }

    runFunction(ITM_XRMS);
    runFunction(ITM_DROP);
    real34ToString(REGISTER_REAL34_DATA(REGISTER_X), tmpStr3000);
    inty_off = strtof (tmpStr3000, NULL);
    runFunction(ITM_DROP);



  if(telltale == MEM_INITIALIZED && statnum >= 2) {
    //GRAPH SETUP
//    if(!Aspect_Square) {
//      clearScreen();
  //    refreshStatusBar();
//    }

    //AUTOSCALE
    x_min = FLoatingMax;
    x_max = FLoatingMin;
    y_min = FLoatingMax;
    y_max = FLoatingMin;
    #ifdef STATDEBUG
    printf("Axis0: x: %f -> %f y: %f -> %f   \n",x_min, x_max, y_min, y_max);   
    #endif
    if(plotmode != _VECT) {

      invalid_intg = false;                                                      //integral scale
      invalid_diff = false;                                                      //Differential ddy scale

      if(PLOT_DIFF || PLOT_INTG) {
        inty = inty_off;                                                          //  integral starting constant co-incides with graph
        for (ix = 0; (ix < LIM && ix < statnum); ++ix) { 
          if(ix !=0){
            ddx = grf_x(ix) - grf_x(ix-1);                                            //used in DIFF and INT
            if(ddx<=0) {                                                              //Cannot get slop or area if x is not growing in positive dierection
      		    x_min = FLoatingMax;
      		    x_max = FLoatingMin;
      		    y_min = FLoatingMax;
      		    y_max = FLoatingMin;
      		    invalid_diff = true;
              invalid_intg = true;
      		    break;
            } else {          	
              if(PLOT_DIFF) {
                if(ddx != 0) ddy = (grf_y(ix) - grf_y(ix-1)) / ddx; else ddy = FLoatingMax;  //Differential
                if(ddy < y_min) {y_min = ddy;}
                if(ddy > y_max) {y_max = ddy;}
                if(grf_x(ix) < x_min) {x_min = grf_x(ix);}
                if(grf_x(ix) > x_max) {x_max = grf_x(ix);}
              }
              if(PLOT_INTG) {
                inty = inty + (grf_y(ix) + grf_y(ix-1)) / 2 * ddx;                    //integral
                if(inty < y_min) {y_min = inty;}
                if(inty > y_max) {y_max = inty;}
                if(grf_x(ix) < x_min) {x_min = grf_x(ix);}
                if(grf_x(ix) > x_max) {x_max = grf_x(ix);}
              }
            }
          } 
        }
      }

      #ifdef STATDEBUG
      printf("Axis0b1: x: %f -> %f y: %f -> %f  %d \n",x_min, x_max, y_min, y_max, invalid_diff);
      #endif

      if(PLOT_BOX || PLOT_LINE || PLOT_CROSS || !(PLOT_DIFF || PLOT_INTG)) {  //XXXX
        for(cnt=0; (cnt < LIM && cnt < statnum); cnt++) {
          #ifdef STATDEBUG
          printf("Axis0a: x: %f y: %f   \n",grf_x(cnt), grf_y(cnt));   
          #endif
          if(grf_x(cnt) < x_min) {x_min = grf_x(cnt);}
          if(grf_x(cnt) > x_max) {x_max = grf_x(cnt);}
          if(grf_y(cnt) < y_min) {y_min = grf_y(cnt);}
          if(grf_y(cnt) > y_max) {y_max = grf_y(cnt);}
          #ifdef STATDEBUG
          printf("Axis0b: x: %f -> %f y: %f -> %f   \n",x_min, x_max, y_min, y_max);   
          #endif
        }
      }
    } 

    else {                 //VECTOR
      sx =0;
      sy =0;
      for(cnt=0; (cnt < LIM && cnt < statnum); cnt++) {            //### Note XXX E- will stuff up statnum!
        sx = sx + grf_x(cnt);
        sy = sy + grf_y(cnt);
        if(sx < x_min) {x_min = sx;}
        if(sx > x_max) {x_max = sx;}
        if(sy < y_min) {y_min = sy;}
        if(sy > y_max) {y_max = sy;}
      }      
    }

    //Manipulate the obtained axes positions

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

    //Cause scales to be the same    
    if(jm_SCALE) {
      x_min = min(x_min,y_min);
      x_max = max(x_max,y_max);
      y_min = x_min;
      y_max = x_max;
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
    printf("Axis3a: x: %f -> %f y: %f -> %f   \n",x_min, x_max, y_min, y_max);   
    #endif

    graph_axis();

    #ifdef STATDEBUG
    printf("Axis3b: x: %f -> %f y: %f -> %f   \n",x_min, x_max, y_min, y_max);   
    #endif


    if(plotmode != _VECT) {
      yn = screen_window_y(y_min,grf_y(0),y_max);
      xn = screen_window_x(x_min,grf_x(0),x_max);
      xN = xn;
      yN = yn;
    } else {
      yn = screen_window_y(y_min,0,y_max);
      xn = screen_window_x(x_min,0,x_max);
      xN = xn;
      yN = yn;      
    }

    #ifdef STATDEBUG
    printf("Axis3c: x: %f -> %f y: %f -> %f   \n",x_min, x_max, y_min, y_max);   
    #endif

    sx = 0;
    sy = 0;
    //GRAPH
    ix = 0;
    inty = inty_off;                                                         //  integral starting constant co-incides with graph
    for (ix = 0; (ix < LIM && ix < statnum); ++ix) {
      if(plotmode != _VECT) {

        x = 0;
        y = 0;

        if(ix !=0 && ( (PLOT_DIFF && !invalid_diff) || (PLOT_INTG && !invalid_intg))){                                                               //Differential ddy
          ddx = grf_x(ix) - grf_x(ix-1);
          if(ddx != 0) ddy = (grf_y(ix) - grf_y(ix-1)) / ddx; else ddy = FLoatingMax;
          inty0 = inty;
          inty = inty + (grf_y(ix) + grf_y(ix-1)) / 2 * ddx;                      //integral
          x = (grf_x(ix) + grf_x(ix-1))/2;
          if(PLOT_DIFF) y = ddy;
          if(PLOT_INTG) y = inty;
        }

        if(PLOT_BOX || PLOT_LINE || PLOT_CROSS) {
          x = grf_x(ix);
          y = grf_y(ix);
        }

      } else {
        sx = sx + grf_x(ix);
        sy = sy + grf_y(ix);
        x = sx;
        y = sy;
      }
      xo = xN;
      yo = yN;
      xN = screen_window_x(x_min,x,x_max);
      yN = screen_window_y(y_min,y,y_max);
      #ifdef STATDEBUG
      printf("plotting graph table[%d] = x:%f y:%f ddy:%f inty:%f xN:%d yN:%d ",ix,x,y,ddy,inty,  xN,yN);
      printf(" ... x-ddx/2=%d ddy=%d inty=%d\n",screen_window_x( x_min, x-ddx/2, x_max), screen_window_y( y_min, ddy, y_max), screen_window_y( y_min, inty, y_max) );
      #endif

      int16_t minny,minnx;
      if (!Aspect_Square) {minny = SCREEN_MIN_GRAPH; minnx = 0;}
      else {minny = 0; minnx = SCREEN_WIDTH-SCREEN_HEIGHT_GRAPH;}
      if(xN<SCREEN_WIDTH_GRAPH && xN>minnx && yN<SCREEN_HEIGHT_GRAPH && yN>minny) {
  //      yo = yn;                              //old , new, to be able to draw a line between samples
        yn = yN;
  //      xo = xn;
        xn = xN;

        #ifdef STATDEBUG
          printf("invalid_diff=%d invalid_intg=%d \n",invalid_diff,invalid_intg);
        #endif


        if(plotmode != _VECT) {

          #ifdef STATDEBUG
            printf("Not _VECT\n");
          #endif

          if(PLOT_CROSS) {
            #ifdef STATDEBUG
              printf("Plotting cross to x=%d y=%d\n",xn,yn);
            #endif
            plotcross(xn,yn);
          }

          if(PLOT_BOX) {
            #ifdef STATDEBUG
              printf("Plotting box to x=%d y=%d\n",xn,yn);
            #endif
            plotbox(xn,yn);
          }
          //else placePixel(xn,yn);

          if(PLOT_DIFF && !invalid_diff && ix != 0) {
            #ifdef STATDEBUG
              printf("Plotting Delta x=%f dy=%f \n",x-ddx/2,ddy);
            #endif
            plotdelta( screen_window_x( x_min, x-ddx/2, x_max), screen_window_y( y_min, ddy, y_max) );
          }

          if(PLOT_INTG && !invalid_intg && ix !=0) {
            #ifdef STATDEBUG
              printf("Plotting Integral x=%f intg(x)=%f\n",x-ddx/2,inty);
            #endif
            uint16_t xN0   =screen_window_x( x_min, grf_x(ix-1), x_max);
            uint16_t xN1   =screen_window_x( x_min, grf_x(ix), x_max);
            uint16_t yNoff =screen_window_y( y_min, inty_off, y_max);
            uint16_t yN0   =screen_window_y( y_min, inty0, y_max);
            uint16_t yNintg=screen_window_y( y_min, inty, y_max);

            uint16_t xAvg=((xN0+xN) >> 1);
            
            if(abs((int16_t)(xN-xN0)>=4)) {plotint( xAvg, yNintg );} else
                                          {placePixel( xAvg, yNintg );}

              //printf("%d %d %d \n",xN0,xN,abs((int16_t)(xN-xN0)));
            if(abs((int16_t)(xN-xN0)>=6)) {plotline(xN, yNintg, xAvg+2, yNintg);plotline(xAvg-2, yNintg, xN0, yNintg);} else
            if(abs((int16_t)(xN-xN0)>=4)) {plotline(xN, yNintg, xAvg+2, yNintg);plotline(xAvg-2, yNintg, xN0, yNintg);}
//            plotline(xN, yNintg, xN0, yNintg);

//            plotline(xN, yNintg, xN0, yNintg);

            plotrect(xN0,yNoff,xN,yN0);
            plotrect(xN0,yN0,xN,yNintg);
            plotline(xN0,yN0,xN,yNintg);


          }

        } else {
          #ifdef STATDEBUG
            printf("Plotting arrow\n");
          #endif
          plotarrow(xo, yo, xn, yn);
        }

        if(PLOT_LINE) {
          #ifdef STATDEBUG
            printf("Plotting line to x=%d y=%d\n",xn,yn);
          #endif
          plotline(xo, yo, xn, yn);
        }

      } else {
        #ifdef PC_BUILD
          printf("Not plotted: ");
          if(!(xN<SCREEN_WIDTH_GRAPH )) printf("xN<SCREEN_WIDTH_GRAPH; ");
          if(!(xN>minnx              )) printf("xN>minnx; ");
          if(!(yN<SCREEN_HEIGHT_GRAPH)) printf("yN<SCREEN_HEIGHT_GRAPH");
          if(!(yN>1+minny            )) printf("yN>1+minny; ");
                    printf("Not plotted: xN=%d<SCREEN_WIDTH_GRAPH=%d && xN=%d>minnx=%d && yN=%d<SCREEN_HEIGHT_GRAPH=%d && yN=%d>1+minny=%d\n",xN,SCREEN_WIDTH_GRAPH,xN,minnx,yN,SCREEN_HEIGHT_GRAPH,yN,1+minny);
        #endif
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


void fnStatList() {
  #ifndef TESTSUITE_BUILD
  char tmpstr1[100], tmpstr2[100];
  int16_t ix, ixx, statnum;

    if(telltale == MEM_INITIALIZED) {
      clearScreen();
      refreshStatusBar();
    }

  if(jm_VECT || jm_NVECT) {plotmode = _VECT;} else {plotmode = _SCAT;}

  if(telltale == MEM_INITIALIZED) {
    runFunction(ITM_NSIGMA);
    if(plotmode != _VECT) {
      realToInt32(SIGMA_N, statnum);
      sprintf(tmpStr3000, "Stat data: N = %d",statnum);
    } else {
      statnum = ix_count;
      sprintf(tmpStr3000, "Stat data: Vector IndexCount = %d",statnum);
    }

    runFunction(ITM_DROP);
    print_linestr(tmpStr3000,true);
    #ifdef STATDEBUG
      printf("Stat data %d - %d (%s)\n",statnum-1, max(0, statnum-1-6), tmpStr3000 );
    #endif

    if( statnum - 0 - 1 + ListXYposition > statnum-1) ListXYposition = 0; else
    if( statnum - (min(10,min(LIM, statnum))-1) - 1 + ListXYposition < 0) ListXYposition = - (statnum - (min(10,min(LIM, statnum))-1) - 1);

    for (ix = 0; (ix < min(10,min(LIM, statnum))); ++ix) {
      ixx = statnum - ix - 1 + ListXYposition;

      if(((fabs(grf_x(ixx)) > 0.000999 || grf_x(ixx) == 0) && fabs(grf_x(ixx)) < 1000000)) 
        sprintf(tmpstr1,"[%3d] x%19.7f, ",ixx+1, grf_x(ixx));
      else
        sprintf(tmpstr1,"[%3d] x%19.7e, ",ixx+1, round(grf_x(ixx)*1e10)/1e10);

      if(((fabs(grf_y(ixx)) > 0.000999 || grf_y(ixx) == 0) && fabs(grf_y(ixx)) < 1000000))
        sprintf(tmpstr2,"y%19.7f", grf_y(ixx));
      else
        sprintf(tmpstr2,"y%19.7e", round(grf_y(ixx)*1e10)/1e10);

      strcat(tmpstr1,tmpstr2);

      print_numberstr(tmpstr1,false);
      #ifdef STATDEBUG
        printf("%d:%s\n",ixx,tmpstr1);
      #endif
    }
  }
  #endif
}





