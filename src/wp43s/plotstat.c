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
 * \file plotstat.c
 ***********************************************/

#include "wp43s.h"
#include "math.h"

//#define STATDEBUG

graphtype *gr_x;
graphtype *gr_y;
  
float  graph_dx;
float  graph_dy; 
bool_t extentx;
bool_t extenty;
bool_t jm_VECT;
bool_t jm_NVECT;
bool_t jm_SCALE;
bool_t Aspect_Square;
bool_t PLOT_LINE;
bool_t PLOT_CROSS;
bool_t PLOT_BOX;
bool_t PLOT_INTG;
bool_t PLOT_DIFF;
bool_t PLOT_RMS;
bool_t PLOT_SHADE;
bool_t PLOT_AXIS;
int8_t PLOT_ZMX;
int8_t PLOT_ZMY;


void statGraphReset(void){
  graph_dx      = 0;
  graph_dy      = 0;
  extentx       = true;
  extenty       = true;
  jm_VECT       = false;
  jm_NVECT      = false;
  jm_SCALE      = false;
  Aspect_Square = true;
  PLOT_LINE     = false;
  PLOT_CROSS    = false;
  PLOT_BOX      = true;
  PLOT_INTG     = false;
  PLOT_DIFF     = false;
  PLOT_RMS      = false;
  PLOT_SHADE    = false;
  PLOT_AXIS     = false;
  PLOT_ZMX      = 0;
  PLOT_ZMY      = 0;

  plotmode      = _SCAT;
  tick_int_x    = 0;
  tick_int_y    = 0;
}


void graph_setupmemory(void) {
  int i;
  if(telltale != MEM_INITIALIZED) {
    gr_x = (graphtype*)malloc(LIM * sizeof(graphtype)); 
    memset(gr_x, 0,           LIM * sizeof(graphtype));
    gr_y = (graphtype*)malloc(LIM * sizeof(graphtype)); 
    memset(gr_y, 0,           LIM * sizeof(graphtype));
    telltale = MEM_INITIALIZED;
    ix_count = 0;
  }
  if ((telltale != MEM_INITIALIZED) || (gr_x == NULL || gr_y == NULL)) { 
  #ifdef PC_BUILD
     moreInfoOnError("In function graph_setupmemory:", "error allocating memory for graph!", NULL, NULL);
     exit(1);
  #endif
  } else
  {
  #ifdef PC_BUILD
    printf("^^@@ Two arrays of %u bytes each created, i.e. %u blocks total\n",(uint32_t) (LIM * sizeof(graphtype)), (uint32_t)(2 * LIM * sizeof(graphtype) / 4));
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
  graphtype x; 
  graphtype y;

  if(jm_VECT || jm_NVECT) {plotmode = _VECT;} else {plotmode = _SCAT;}

  if(telltale != MEM_INITIALIZED) {
    graph_setupmemory();
//    runFunction(ITM_CLSIGMA);
  }


  //Convert from X register to graphtype
  realToString(yy, tmpString);
  y = strtof (tmpString, NULL);
  //printf("y=%f ",y);

  //Convert from X register to graphtype
  realToString(xx, tmpString);
  x = strtof (tmpString, NULL);
  //printf("x=%f ",x);

  #ifndef TESTSUITE_BUILD
  //export_xy_to_file(x,y);                    //Write to CSV file
  #endif

  if(plotmode == _VECT ) {
    ix_count++;                              //Only used for VECT
    cnt = ix_count;
  } else {
    //Convert from real to int
    realToInt32(SIGMA_N, cnt);
    ix_count = cnt;                          //ix_count increments in VECT with Σ-, where SIGMA_N decrements with Σ- 
                                             //if VECT is changed mid-process, it will cause x_count to assume SIGMA_N, which  will throw away the last values stored.
//    realToString(SIGMA_N, tmpString);
//    cnt = stringToInt16 (tmpString);
    #ifdef STATDEBUG
    printf("Count: %s, %d\n",tmpString,cnt);
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


graphtype grf_x(int i) {
  if (jm_NVECT) {return gr_y[i];}
  else {return gr_x[i];}
}

graphtype grf_y(int i) {
  if (jm_NVECT) {return gr_x[i];}
  else {return gr_y[i];}
}


//###################################################################################
#ifndef TESTSUITE_BUILD
  int16_t screen_window_x(graphtype x_min, graphtype x, graphtype x_max) {
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

  int16_t screen_window_y(graphtype y_min, graphtype y, graphtype y_max) {
  int16_t temp, minn;
    if (!Aspect_Square) minn = SCREEN_NONSQ_HMIN;
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
void placePixel(uint32_t x, uint32_t y) {
#ifndef TESTSUITE_BUILD
  uint32_t minn;
  if (!Aspect_Square) minn = SCREEN_NONSQ_HMIN;
  else minn = 0;
    
  if(x<SCREEN_WIDTH_GRAPH && x>0 && y<SCREEN_HEIGHT_GRAPH && y>1+minn) {
    setBlackPixel(x,y);
  }
#endif //!TESTSUITE_BUILD
}

void removePixel(uint32_t x, uint32_t y) {
#ifndef TESTSUITE_BUILD
  uint32_t minn;
  if (!Aspect_Square) minn = SCREEN_NONSQ_HMIN;
  else minn = 0;

  if(x<SCREEN_WIDTH_GRAPH && x>0 && y<SCREEN_HEIGHT_GRAPH && y>1+minn) {
    setWhitePixel(x,y);
  }
#endif //!TESTSUITE_BUILD
}

void clearScreenPixels(void) {
#ifndef TESTSUITE_BUILD
  if (Aspect_Square) {
    lcd_fill_rect(SCREEN_WIDTH-SCREEN_HEIGHT_GRAPH, 0, SCREEN_HEIGHT_GRAPH, SCREEN_HEIGHT_GRAPH, 0);
    lcd_fill_rect(0, Y_POSITION_OF_REGISTER_T_LINE, SCREEN_WIDTH-SCREEN_HEIGHT_GRAPH, 171-5-Y_POSITION_OF_REGISTER_T_LINE+1, 0);
    lcd_fill_rect(19, 171-5, SCREEN_WIDTH-SCREEN_HEIGHT_GRAPH-19+1, 5, 0);
  }
  else
    lcd_fill_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT_GRAPH, 0);
#endif //!TESTSUITE_BUILD
}

#ifndef TESTSUITE_BUILD
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

void plotbox_fat(uint16_t xn, uint8_t yn) {                // Plots line from xo,yo to xn,yn; uses temporary x1,y1
  void plotrect(uint16_t a, uint8_t b, uint16_t c, uint8_t d) {                // Plots rectangle from xo,yo to xn,yn; uses temporary x1,y1
    plotline(a, b, c, b);
    plotline(a, b, a, d);
    plotline(c, d, c, b);
    plotline(c, d, a, d);
  }
  plotrect(xn-3,yn-3,xn+3,yn+3);
  plotrect(xn-2,yn-2,xn+2,yn+2);
}


#endif //!TESTSUITE_BUILD


//###################################################################################
void plotline(uint16_t xo, uint8_t yo, uint16_t xn, uint8_t yn) {                   // Plots line from xo,yo to xn,yn; uses temporary x1,y1
   pixelline(xo,yo,xn,yn,1);
 }


void pixelline(uint16_t xo, uint8_t yo, uint16_t xn, uint8_t yn, bool_t vmNormal) { // Plots line from xo,yo to xn,yn; uses temporary x1,y1
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
//###################################################################################

void force_refresh1(void) {
#ifdef PC_BUILD
  gtk_widget_queue_draw(screen);
  //FULL UPDATE (UGLY)
  #ifdef FULLUPDATE
     refresh_gui();
  #endif //FULLUPDATE
#endif //PC_BUILD
#if DMCP_BUILD
  lcd_forced_refresh();
#endif //PC_BUILD
}


void graph_axis_draw (void){
  #ifndef TESTSUITE_BUILD
  uint32_t cnt;

  //GRAPH RANGE: Typical
  //  graph_xmin= -3*3.14150;  graph_xmax= 2*3.14159;
  //  graph_ymin= -2;          graph_ymax= +2;


  clearScreenPixels();



  //GRAPH ZERO AXIS
  yzero = screen_window_y(y_min,0,y_max);
  xzero = screen_window_x(x_min,0,x_max);


  uint32_t minnx, minny;
  if (!Aspect_Square) {
    minny = SCREEN_NONSQ_HMIN;
    minnx = 0;
  }
  else {
    minny = 0;
    minnx = SCREEN_WIDTH-SCREEN_HEIGHT_GRAPH;
  }


  //SEPARATING LINE IF SQUARE
  cnt = minny;
  while (cnt!=SCREEN_HEIGHT_GRAPH) { 
    if (Aspect_Square) {
        setBlackPixel(minnx-1,cnt);   
        setBlackPixel(minnx-2,cnt);   
    }
    cnt++; 
  }

  #ifdef STATDEBUG
    printf("xzero=%d yzero=%d   \n",xzero,yzero);
  #endif


  graphtype x; 
  graphtype y;

  if( PLOT_AXIS && !(yzero == SCREEN_HEIGHT_GRAPH-1 || yzero == minny)) {
    //DRAW XAXIS
    if (Aspect_Square) {
      cnt = minnx;
    } else {
      cnt = 0;
    }

    while (cnt!=SCREEN_WIDTH_GRAPH-1) { 
      setBlackPixel(cnt,yzero); 
      cnt++; 
    }

   force_refresh1();

   if(0<x_max && 0>x_min) {
     for(x=0; x<=x_max; x+=tick_int_x) {                         //draw x ticks
        cnt = screen_window_x(x_min,x,x_max);
        //printf(">>>>>A %f %d ",x,cnt);
          setBlackPixel(cnt,min(yzero+1,SCREEN_HEIGHT_GRAPH-1)); //tick
          setBlackPixel(cnt,max(yzero-1,minny));                 //tick
       }
      for(x=0; x>=x_min; x+=-tick_int_x) {                       //draw x ticks
        cnt = screen_window_x(x_min,x,x_max);
        //printf(">>>>>B %f %d ",x,cnt);
          setBlackPixel(cnt,min(yzero+1,SCREEN_HEIGHT_GRAPH-1)); //tick
          setBlackPixel(cnt,max(yzero-1,minny));                 //tick
       }
      for(x=0; x<=x_max; x+=tick_int_x*5) {                      //draw x ticks
        cnt = screen_window_x(x_min,x,x_max);
          setBlackPixel(cnt,min(yzero+2,SCREEN_HEIGHT_GRAPH-1)); //tick
          setBlackPixel(cnt,max(yzero-2,minny));                 //tick
          setBlackPixel(cnt,min(yzero+3,SCREEN_HEIGHT_GRAPH-1)); //tick
          setBlackPixel(cnt,max(yzero-3,minny));                 //tick
       }
      for(x=0; x>=x_min; x+=-tick_int_x*5) {                     //draw x ticks
        cnt = screen_window_x(x_min,x,x_max);
          setBlackPixel(cnt,min(yzero+2,SCREEN_HEIGHT_GRAPH-1)); //tick
          setBlackPixel(cnt,max(yzero-2,minny));                 //tick
          setBlackPixel(cnt,min(yzero+3,SCREEN_HEIGHT_GRAPH-1)); //tick
          setBlackPixel(cnt,max(yzero-3,minny));                 //tick
       }
   } else {
     for(x=x_min; x<=x_max; x+=tick_int_x) {                     //draw x ticks
        cnt = screen_window_x(x_min,x,x_max);
        //printf(">>>>>A %f %d ",x,cnt);
          setBlackPixel(cnt,min(yzero+1,SCREEN_HEIGHT_GRAPH-1)); //tick
          setBlackPixel(cnt,max(yzero-1,minny));                 //tick
       }
      for(x=x_min; x<=x_max; x+=tick_int_x*5) {                  //draw x ticks
        cnt = screen_window_x(x_min,x,x_max);
          setBlackPixel(cnt,min(yzero+2,SCREEN_HEIGHT_GRAPH-1)); //tick
          setBlackPixel(cnt,max(yzero-2,minny));                 //tick
          setBlackPixel(cnt,min(yzero+3,SCREEN_HEIGHT_GRAPH-1)); //tick
          setBlackPixel(cnt,max(yzero-3,minny));                 //tick
       }
     }
   }



  if( PLOT_AXIS && !(xzero == SCREEN_WIDTH-1 || xzero == minnx)) {
    //Write North arrow
    if(jm_NVECT) {
      showString("N", &standardFont, xzero-4, minny+14, vmNormal, true, true);
      tmpString[0]=0x80 | 0x22;
      tmpString[1]=0x06;
      tmpString[2]=0;
      showString(tmpString, &standardFont, xzero-4, minny+0, vmNormal, true, true);
    }

    //DRAW YAXIS
    lcd_fill_rect(xzero,minny,1,SCREEN_HEIGHT_GRAPH-minny,0xFF);

    force_refresh1();
    if(0<y_max && 0>y_min) {
      for(y=0; y<=y_max; y+=tick_int_y) {                     //draw y ticks
        cnt = screen_window_y(y_min,y,y_max);
        setBlackPixel(max(xzero-1,0),cnt);                    //tick
        setBlackPixel(min(xzero+1,SCREEN_WIDTH_GRAPH-1),cnt); //tick
      }  
      for(y=0; y>=y_min; y+=-tick_int_y) {                    //draw y ticks
        cnt = screen_window_y(y_min,y,y_max);
        setBlackPixel(max(xzero-1,0),cnt);                    //tick
        setBlackPixel(min(xzero+1,SCREEN_WIDTH_GRAPH-1),cnt); //tick
      }  
      for(y=0; y<=y_max; y+=tick_int_y*5) {                   //draw y ticks
        cnt = screen_window_y(y_min,y,y_max);
        setBlackPixel(max(xzero-2,0),cnt);                    //tick
        setBlackPixel(min(xzero+2,SCREEN_WIDTH_GRAPH-1),cnt); //tick
        setBlackPixel(max(xzero-3,0),cnt);                    //tick
        setBlackPixel(min(xzero+3,SCREEN_WIDTH_GRAPH-1),cnt); //tick
      }  
      for(y=0; y>=y_min; y+=-tick_int_y*5) {                  //draw y ticks
        cnt = screen_window_y(y_min,y,y_max);
        setBlackPixel(max(xzero-2,0),cnt);                    //tick
        setBlackPixel(min(xzero+2,SCREEN_WIDTH_GRAPH-1),cnt); //tick
        setBlackPixel(max(xzero-3,0),cnt);                    //tick
        setBlackPixel(min(xzero+3,SCREEN_WIDTH_GRAPH-1),cnt); //tick
      }  
    } else {
      for(y=y_min; y<=y_max; y+=tick_int_y) {                 //draw y ticks
        cnt = screen_window_y(y_min,y,y_max);
        setBlackPixel(max(xzero-1,0),cnt);                    //tick
        setBlackPixel(min(xzero+1,SCREEN_WIDTH_GRAPH-1),cnt); //tick
      }  
      for(y=y_min; y<=y_max; y+=tick_int_y*5) {               //draw y ticks
        cnt = screen_window_y(y_min,y,y_max);
        setBlackPixel(max(xzero-2,0),cnt);                    //tick
        setBlackPixel(min(xzero+2,SCREEN_WIDTH_GRAPH-1),cnt); //tick
        setBlackPixel(max(xzero-3,0),cnt);                    //tick
        setBlackPixel(min(xzero+3,SCREEN_WIDTH_GRAPH-1),cnt); //tick
      }  
    }
  }
  force_refresh1();
  #endif
}
//###################################################################################

double a0 = 0;
double a1 = 0;
uint8_t dt = CF_LINEAR_FITTING;



void graphPlotstat(void){
  #ifndef TESTSUITE_BUILD

  uint16_t  cnt, ix, statnum;
  uint16_t  xo, xn, xN; 
  uint8_t   yo, yn, yN;
  graphtype x; 
  graphtype y;

  statnum = 0;

  graph_axis_draw();                        //Draw the axis on any uncontrolled scale to start. Maybe optimize by remembering if there is an image on screen Otherwise double axis draw.
  plotmode = _SCAT;

  if(telltale == MEM_INITIALIZED && checkMinimumDataPoints(const_2)) {

//    runFunction(ITM_NSIGMA);
    realToInt32(SIGMA_N, statnum);   
//    runFunction(ITM_DROP);

    #ifdef STATDEBUG
    printf("statnum n=%d\n",statnum);
    #endif 
  }

  if(telltale == MEM_INITIALIZED && statnum >= 2) {
    //AUTOSCALE
    x_min = FLoatingMax;
    x_max = FLoatingMin;
    y_min = FLoatingMax;
    y_max = FLoatingMin;
    #ifdef STATDEBUG
    printf("Axis0: x: %f -> %f y: %f -> %f   \n",x_min, x_max, y_min, y_max);   
    #endif


//#################################################### vvv SCALING LOOP  vvv #########################
/**/        for(cnt=0; (cnt < LIM && cnt < statnum); cnt++) {
/**/          #ifdef STATDEBUG
/**/          printf("Axis0a: x: %f y: %f   \n",grf_x(cnt), grf_y(cnt));   
/**/          #endif
/**/          if(grf_x(cnt) < x_min) {x_min = grf_x(cnt);}
/**/          if(grf_x(cnt) > x_max) {x_max = grf_x(cnt);}
/**/          if(grf_y(cnt) < y_min) {y_min = grf_y(cnt);}
/**/          if(grf_y(cnt) > y_max) {y_max = grf_y(cnt);}
/**/          #ifdef STATDEBUG
/**/          printf("Axis0b: x: %f -> %f y: %f -> %f   \n",x_min, x_max, y_min, y_max);   
/**/          #endif
/**/        }
//#################################################### ^^^ SCALING LOOP ^^^ #########################


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

    if(PLOT_ZMX != 0) {
      x_min = pow(2.0,-PLOT_ZMX) * x_min;
      x_max = pow(2.0,-PLOT_ZMX) * x_max;
    }

    if(PLOT_ZMY != 0) {
      y_min = pow(2.0,-PLOT_ZMY) * y_min;
      y_max = pow(2.0,-PLOT_ZMY) * y_max;      
    }

    #ifdef STATDEBUG
    printf("Axis2: x: %f -> %f y: %f -> %f   \n",x_min, x_max, y_min, y_max);   
    #endif

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
   
    x_min = x_min - dx * 0.015*3;
    y_min = y_min - dy * 0.015*3;
    x_max = x_max + dx * 0.015*3;
    y_max = y_max + dy * 0.015*3;

    #ifdef STATDEBUG
    printf("Axis3a: x: %f -> %f y: %f -> %f   \n",x_min, x_max, y_min, y_max);   
    #endif

    graph_axis_draw();

    yn = screen_window_y(y_min,grf_y(0),y_max);
    xn = screen_window_x(x_min,grf_x(0),x_max);
    xN = xn;
    yN = yn;

    #ifdef STATDEBUG
    printf("Axis3c: x: %f -> %f y: %f -> %f   \n",x_min, x_max, y_min, y_max);   
    #endif

    //GRAPH
    ix = 0;

//#################################################### vvv MAIN GRAPH LOOP vvv #########################
        for (ix = 0; (ix < LIM && ix < statnum); ++ix) {
          x = grf_x(ix);
          y = grf_y(ix);
          xo = xN;
          yo = yN;
          xN = screen_window_x(x_min,x,x_max);
          yN = screen_window_y(y_min,y,y_max);
    
          #ifdef STATDEBUG
          printf("plotting graph table[%d] = x:%f y:%f xN:%d yN:%d ",ix,x,y,  xN,yN);
          #endif
    
          int16_t minN_y,minN_x;
          if (!Aspect_Square) {minN_y = SCREEN_NONSQ_HMIN; minN_x = 0;}
          else {minN_y = 0; minN_x = SCREEN_WIDTH-SCREEN_HEIGHT_GRAPH;}
          if(xN<SCREEN_WIDTH_GRAPH && xN>minN_x && yN<SCREEN_HEIGHT_GRAPH && yN>minN_y) {
            yn = yN;
            xn = xN;
        
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
              plotbox_fat(xn,yn);
            }

            if(PLOT_LINE) {
              #ifdef STATDEBUG
                printf("Plotting line to x=%d y=%d\n",xn,yn);
              #endif
              plotline(xo, yo, xn, yn);
            }
          } 
          else {
            #ifdef PC_BUILD
              printf("Not plotted: ");
              if(!(xN<SCREEN_WIDTH_GRAPH )) printf("xN<SCREEN_WIDTH_GRAPH; ");
              if(!(xN>minN_x              )) printf("xN>minN_x; ");
              if(!(yN<SCREEN_HEIGHT_GRAPH)) printf("yN<SCREEN_HEIGHT_GRAPH");
              if(!(yN>1+minN_y            )) printf("yN>1+minN_y; ");
                        printf("Not plotted: xN=%d<SCREEN_WIDTH_GRAPH=%d && xN=%d>minN_x=%d && yN=%d<SCREEN_HEIGHT_GRAPH=%d && yN=%d>1+minN_y=%d\n",xN,SCREEN_WIDTH_GRAPH,xN,minN_x,yN,SCREEN_HEIGHT_GRAPH,yN,1+minN_y);
            #endif
          }
        }

        //#################################################### ^^^ MAIN GRAPH LOOP ^^^ #########################
        
        drawline(dt, a1, a0);

      } else {
        displayCalcErrorMessage(ERROR_NO_SUMMATION_DATA, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          sprintf(errorMessage, "There is no statistical data available!");
          moreInfoOnError("In function graph_plotmem:", errorMessage, NULL, NULL);
        #endif
      }
#endif
}






//###################################################################################
void fnPlotStat(uint16_t unusedButMandatoryParameter){
  statGraphReset(); 
	calcMode = CM_PLOT_STAT;

  #ifdef DMCP_BUILD
    lcd_refresh();
  #else // !DMCP_BUILD
    refreshLcd(NULL);
  #endif // DMCP_BUILD

  hourGlassIconEnabled = true;
  showHideHourGlass();

  #ifndef TESTSUITE_BUILD
    if(softmenu[softmenuStack[0].softmenuId].menuItem != -MNU_PLOT_STAT) {
      showSoftmenu(-MNU_PLOT_STAT);
    }
  #endif
}



void drawline(int8_t fittype, graphtype a1, graphtype a0) {
  #ifdef STATDEBUG
    printf("plotting line: %f x + %f\n",m,c);
  #endif
  uint16_t  ix;
  uint16_t  xo, xn, xN = 0; 
  uint8_t   yo, yn, yN = 0;
  #define   numberIntervals 50
  graphtype dx = (x_max-x_min)/numberIntervals;
  graphtype x = 0; 
  graphtype y = 0;


    for (ix = 0; ix <= numberIntervals; ++ix) {
      switch(fittype) {
        case CF_LINEAR_FITTING: 
          x = x_min + dx * ix;
          y = a1 * x + a0; 
          break;
        case CF_EXPONENTIAL_FITTING: 
          x = x_min + dx * ix;
          y = a0 * exp(a1 * x); 
          break;
        default:break;
      }
      xo = xN;
      yo = yN;
      xN = screen_window_x(x_min,x,x_max);
      yN = screen_window_y(y_min,y,y_max);

      if(ix != 0) {  
        #ifdef STATDEBUG
        printf("plotting graph sample no %d ==> x:%f y:%f xN:%d yN:%d ",ix,x,y,  xN,yN);
        #endif
        int16_t minN_y,minN_x;
        if (!Aspect_Square) {minN_y = SCREEN_NONSQ_HMIN; minN_x = 0;}
        else {minN_y = 0; minN_x = SCREEN_WIDTH-SCREEN_HEIGHT_GRAPH;}
        if(xN<SCREEN_WIDTH_GRAPH && xN>minN_x && yN<SCREEN_HEIGHT_GRAPH && yN>minN_y) {
          yn = yN;
          xn = xN;
          #ifdef STATDEBUG
            printf("Plotting box to x=%d y=%d\n",xn,yn);
          #endif
          plotbox(xn,yn);
          #ifdef STATDEBUG
            printf("Plotting line to x=%d y=%d\n",xn,yn);
          #endif
          plotline(xo, yo, xn, yn);
        } 
        else {
          #ifdef PC_BUILD
            printf("Not plotted: ");
            if(!(xN<SCREEN_WIDTH_GRAPH ))  printf("xN<SCREEN_WIDTH_GRAPH; ");
            if(!(xN>minN_x              )) printf("xN>minN_x; ");
            if(!(yN<SCREEN_HEIGHT_GRAPH))  printf("yN<SCREEN_HEIGHT_GRAPH");
            if(!(yN>1+minN_y            )) printf("yN>1+minN_y; ");
            printf("Not plotted: xN=%d<SCREEN_WIDTH_GRAPH=%d && xN=%d>minN_x=%d && yN=%d<SCREEN_HEIGHT_GRAPH=%d && yN=%d>1+minN_y=%d\n",xN,SCREEN_WIDTH_GRAPH,xN,minN_x,yN,SCREEN_HEIGHT_GRAPH,yN,1+minN_y);
          #endif
        }
      }
    }
  }



void fnPlotRegLine(uint16_t unusedButMandatoryParameter){
int32_t nn;
char ss[100];

/*
#define CF_LINEAR_FITTING                          1
#define CF_EXPONENTIAL_FITTING                     2
#define CF_LOGARITHMIC_FITTING                     4
#define CF_POWER_FITTING                           8
#define CF_ROOT_FITTING                           16
#define CF_HYPERBOLIC_FITTING                     32
#define CF_PARABOLIC_FITTING                      64
#define CF_CAUCHY_FITTING                        128
#define CF_GAUSS_FITTING                         256
#define CF_ORTHOGONAL_FITTING                    512
*/

double sx2,sy,sx,sxy;
    dt = CF_LINEAR_FITTING;
    realToInt32(SIGMA_N, nn);   
    realToString(SIGMA_X2, ss); sx2 = strtof (ss, NULL);
    realToString(SIGMA_Y , ss); sy  = strtof (ss, NULL);
    realToString(SIGMA_X , ss); sx  = strtof (ss, NULL);
    realToString(SIGMA_XY, ss); sxy = strtof (ss, NULL);
    a0 = (sx2 * sy  - sx * sxy) / (nn * sx2 - sx * sx);
    a1 = (nn  * sxy - sx * sy ) / (nn * sx2 - sx * sx);
 //   #ifdef STATDEBUG
      printf("##### Linear %i a0=%f a1=%f \n",nn, a0, a1);
   // #endif

double slny,sxlny,a0exp,a1exp;
    dt = CF_EXPONENTIAL_FITTING;
    realToInt32(SIGMA_N, nn);   
    realToString(SIGMA_X2, ss); sx2 = strtof (ss, NULL);
    realToString(SIGMA_X , ss); sx  = strtof (ss, NULL);
    realToString(SIGMA_lnY , ss); slny  = strtof (ss, NULL);
    realToString(SIGMA_XlnY, ss); sxlny = strtof (ss, NULL);
    a0exp = (sx2 * slny  - sx * sxlny) / (nn * sx2 - sx * sx);
    a1exp = (nn  * sxlny - sx * slny ) / (nn * sx2 - sx * sx);
    a0 = exp(a0exp);
    a1 = a1exp;
//    #ifdef STATDEBUG
      printf("##### EXPF %i a0=%f a1=%f \n",nn, a0, a1);
  //  #endif



}


