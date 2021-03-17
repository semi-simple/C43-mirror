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

#define STATDEBUG

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
    realToInt32(SIGMA_N, cnt);
    ix_count = cnt;                          //ix_count increments in VECT with Σ-, where SIGMA_N decrements with Σ- 
                                             //if VECT is changed mid-process, it will cause x_count to assume SIGMA_N, which  will throw away the last values stored.
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
  if (jm_NVECT) {
    return gr_y[i];
  }
  else {
    return gr_x[i];
  }
}

graphtype grf_y(int i) {
  if (jm_NVECT) {
    return gr_x[i];
  }
  else {
    return gr_y[i];
  }
}


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

static void plotrect(uint16_t a, uint8_t b, uint16_t c, uint8_t d) {                // Plots rectangle from xo,yo to xn,yn; uses temporary x1,y1
  plotline(a, b, c, b);
  plotline(a, b, a, d);
  plotline(c, d, c, b);
  plotline(c, d, a, d);
}

void plotbox_fat(uint16_t xn, uint8_t yn) {                                         // Plots line from xo,yo to xn,yn; uses temporary x1,y1
  plotrect(xn-3,yn-3,xn+3,yn+3);
  plotrect(xn-2,yn-2,xn+2,yn+2);
}
#endif //!TESTSUITE_BUILD


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
      tmpString[0]=(char)((uint8_t)0x80 | (uint8_t)0x22);
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


graphtype a2 = 0;
graphtype a1 = 0;
graphtype a0 = 0;
double r = 0;
double smi2 = 0;
int32_t nn;


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
    realToInt32(SIGMA_N, statnum);   
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


    //#################################################### vvv SCALING LOOP  vvv
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
    //##  ################################################## ^^^ SCALING LOOP ^^^
    #ifdef STATDEBUG
      printf("Axis1a: x: %f -> %f y: %f -> %f   \n",x_min, x_max, y_min, y_max);   
    #endif
  
    //Check and correct if min and max is swapped
    if(x_min>0 && x_min > x_max) {x_min = x_min - (-x_max+x_min)* 1.1;}
    if(x_min<0 && x_min > x_max) {x_min = x_min + (-x_max+x_min)* 1.1;}
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
  
    //Calc zoom scales    
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
    
    x_min = x_min - dx * 0.05;
    y_min = y_min - dy * 0.05;
    x_max = x_max + dx * 0.05;
    y_max = y_max + dy * 0.05;
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
  
  
    //#################################################### vvv MAIN GRAPH LOOP vvv
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
    //#################################################### ^^^ MAIN GRAPH LOOP ^^^          
  drawline();

  } else {
    displayCalcErrorMessage(ERROR_NO_SUMMATION_DATA, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "There is no statistical data available!");
      moreInfoOnError("In function graphPlotstat:", errorMessage, NULL, NULL);
    #endif
  }
#endif
}


uint16_t selection = 0;
#ifndef TESTSUITE_BUILD
void drawline(){
  #ifdef STATDEBUG
    printf("plotting line: a2 %f a1 %f a0 %f\n",a2,a1,a0);
  #endif
  if((selection==0 && a2 == 0 && a1 == 0 && a0 == 0)) {
    #ifdef STATDEBUG
      printf("return\n");
    #endif
    return;
  }
  uint16_t  ix;
  uint16_t  xo, xn, xN = 0; 
  uint8_t   yo, yn, yN = 0;
  #define   numberIntervals 50
  graphtype dx = (x_max-x_min)/numberIntervals;
  graphtype x = 0; 
  graphtype y = 0;


    for (ix = 0; ix <= numberIntervals; ++ix) {
      x = x_min + dx * ix;
      switch(selection) {
        case CF_LINEAR_FITTING: 
        case CF_ORTHOGONAL_FITTING: 
        case CF_ORTHOGONAL_FITTING+10000: 
          y = a1 * x + a0; 
          break;
        case CF_EXPONENTIAL_FITTING: 
          y = a0 * exp(a1 * x); 
          break;
        case CF_LOGARITHMIC_FITTING: 
          y = a0 + a1*log(x); 
          break;
        case CF_POWER_FITTING: 
          y = a0 * pow(x,a1);
          break;
        case CF_ROOT_FITTING: 
          y = a0 * pow(a1,1/x);
          break;
        case CF_HYPERBOLIC_FITTING: 
          y = 1 / (a1 * x + a0);
          break;
        case CF_PARABOLIC_FITTING: 
          y = a2 * x * x + a1 * x + a0;
          break;
        case CF_GAUSS_FITTING:
          y = a0 * exp((x-a1)*(x-a1)/a2);
          break;
        case CF_CAUCHY_FITTING:
          y = 1/(a0*(x+a1)*(x+a1)+a2);
        default:break;
      }
      xo = xN;
      yo = yN;
      xN = screen_window_x(x_min,x,x_max);
      yN = screen_window_y(y_min,y,y_max);

      if(ix > 0) {  //Allow for starting values to accumulate
        #ifdef STATDEBUG
        printf("plotting graph sample no %d ==> x:%f y:%f xN:%d yN:%d ",ix,x,y,  xN,yN);
        #endif
        int16_t minN_y,minN_x;
        if (!Aspect_Square) {minN_y = SCREEN_NONSQ_HMIN; minN_x = 0;}
        else {minN_y = 0; minN_x = SCREEN_WIDTH-SCREEN_HEIGHT_GRAPH;}

        #define tol 4
        if(xN<SCREEN_WIDTH_GRAPH && xN>minN_x && yN<SCREEN_HEIGHT_GRAPH-tol && yN>minN_y) {
          yn = yN;
          xn = xN;
          #ifdef STATDEBUG
            printf("Plotting box to x=%d y=%d\n",xn,yn);
          #endif
          plotbox(xn,yn);
          if(xo<SCREEN_WIDTH_GRAPH && xo>minN_x && yo<SCREEN_HEIGHT_GRAPH-tol && yo>minN_y) {
            #ifdef STATDEBUG
              printf("Plotting line to x=%d y=%d\n",xn,yn);
            #endif
            plotline(xo, yo, xn, yn);
          }
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

    #define autoinc 18
    int16_t index = 0;
    char ss[100];
    if(selection!=0) {
      strcpy(ss,getCurveFitModeName(selection%10000));
      if(selection == CF_ORTHOGONAL_FITTING) strcat(ss,"(+)"); else
        if(selection == CF_ORTHOGONAL_FITTING+10000) strcat(ss,"(-)");
      showString(ss, &standardFont, 0, Y_POSITION_OF_REGISTER_Z_LINE + autoinc*index++ -5, vmNormal, true, true);
      sprintf(ss,"n =%d",(int)nn);       showString(ss, &standardFont, 0, Y_POSITION_OF_REGISTER_Z_LINE + autoinc*index++, vmNormal, true, true);
      sprintf(ss,"a0=%4f",a0);      showString(ss, &standardFont, 0, Y_POSITION_OF_REGISTER_Z_LINE + autoinc*index++, vmNormal, true, true);
      sprintf(ss,"a1=%4f",a1);      showString(ss, &standardFont, 0, Y_POSITION_OF_REGISTER_Z_LINE + autoinc*index++, vmNormal, true, true);
      if(selection == CF_PARABOLIC_FITTING || selection == CF_GAUSS_FITTING || selection == CF_CAUCHY_FITTING){ 
        sprintf(ss,"a2=%4f",a2);    showString(ss, &standardFont, 0, Y_POSITION_OF_REGISTER_Z_LINE + autoinc*index++, vmNormal, true, true);
      }
      sprintf(ss,"r^2=%4f",r*r);    showString(ss, &standardFont, 0, Y_POSITION_OF_REGISTER_Z_LINE + autoinc*index++, vmNormal, true, true);
      if((selection == CF_ORTHOGONAL_FITTING) ||(selection == CF_ORTHOGONAL_FITTING+10000)) {
        sprintf(ss,"smi^2=%4f",smi2); showString(ss, &standardFont, 0, Y_POSITION_OF_REGISTER_Z_LINE + autoinc*index++, vmNormal, true, true);
      }
    }
  }
#endif //TESTSUITE_BUILD


void fnPlotClose(uint16_t unusedButMandatoryParameter){
  calcMode = CM_NORMAL;
  fnKeyExit(0);
}


void fnPlotStat(uint16_t unusedButMandatoryParameter){
  statGraphReset(); 
  calcMode = CM_PLOT_STAT;
  selection = 0;
  r = 0;
  smi2 = 0;

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


void fnPlotRegLine(uint16_t unusedButMandatoryParameter){
    if(selection == 0                                     ) selection = CF_ORTHOGONAL_FITTING ;     else
      if(selection == CF_ORTHOGONAL_FITTING               ) selection = CF_ORTHOGONAL_FITTING+10000;else
        if(selection == CF_ORTHOGONAL_FITTING+10000       ) selection = CF_LINEAR_FITTING     ;     else
          if(selection == CF_LINEAR_FITTING               ) selection = CF_EXPONENTIAL_FITTING;     else
            if(selection == CF_EXPONENTIAL_FITTING        ) selection = CF_LOGARITHMIC_FITTING;     else
              if(selection == CF_LOGARITHMIC_FITTING      ) selection = CF_POWER_FITTING      ;     else
                if(selection == CF_POWER_FITTING          ) selection = CF_ROOT_FITTING       ;     else
                  if(selection == CF_ROOT_FITTING         ) selection = CF_HYPERBOLIC_FITTING ;     else
                    if(selection == CF_HYPERBOLIC_FITTING ) selection = CF_CAUCHY_FITTING     ;     else
                      if(selection == CF_CAUCHY_FITTING   ) selection = CF_GAUSS_FITTING      ;     else
                        if(selection == CF_GAUSS_FITTING  ) selection = 0                     ;     else
                           selection = 0;
  

    char ss[100];
    realToInt32(SIGMA_N, nn);  
    realToString(SIGMA_X ,     ss); double sumx      = strtof (ss, NULL);
    realToString(SIGMA_Y ,     ss); double sumy      = strtof (ss, NULL);
    realToString(SIGMA_X2,     ss); double sumx2     = strtof (ss, NULL);
    realToString(SIGMA_X2Y,    ss); double sumx2y   = strtof (ss, NULL);
    realToString(SIGMA_Y2,     ss); double sumy2     = strtof (ss, NULL);
    realToString(SIGMA_XY,     ss); double sumxy     = strtof (ss, NULL);
    realToString(SIGMA_lnXlnY, ss); double sumlnxlny = strtof (ss, NULL);
    realToString(SIGMA_lnX ,   ss); double sumlnx    = strtof (ss, NULL);
    realToString(SIGMA_ln2X,   ss); double sumln2x   = strtof (ss, NULL);
    realToString(SIGMA_YlnX,   ss); double sumylnx   = strtof (ss, NULL);
    realToString(SIGMA_lnY ,   ss); double sumlny    = strtof (ss, NULL);
    realToString(SIGMA_ln2Y,   ss); double sumln2y   = strtof (ss, NULL);
    realToString(SIGMA_XlnY,   ss); double sumxlny   = strtof (ss, NULL);
    realToString(SIGMA_lnYonX, ss); double sumlnyonx = strtof (ss, NULL);
    realToString(SIGMA_X2onY,  ss); double sumx2ony  = strtof (ss, NULL);
    realToString(SIGMA_1onX,   ss); double sum1onx   = strtof (ss, NULL);
    realToString(SIGMA_1onX2,  ss); double sum1onx2  = strtof (ss, NULL);
    realToString(SIGMA_XonY,   ss); double sumxony   = strtof (ss, NULL);
    realToString(SIGMA_1onY,   ss); double sum1ony   = strtof (ss, NULL);
    realToString(SIGMA_1onY2,  ss); double sum1ony2  = strtof (ss, NULL);
    realToString(SIGMA_X3,     ss); double sumx3   = strtof (ss, NULL);
    realToString(SIGMA_X4,     ss); double sumx4   = strtof (ss, NULL);




    fnMeanXY(0);
    real34ToString(REGISTER_REAL34_DATA(REGISTER_X), ss); double x_ = strtof (ss, NULL);
    real34ToString(REGISTER_REAL34_DATA(REGISTER_Y), ss); double y_ = strtof (ss, NULL);
    fnSampleStdDev(0);
    real34ToString(REGISTER_REAL34_DATA(REGISTER_X), ss); double sx = strtof (ss, NULL); //sqrt(1.0/(nn*(nn-1.0)) * ( nn *sumx2 - sumx*sumx) );
    real34ToString(REGISTER_REAL34_DATA(REGISTER_Y), ss); double sy = strtof (ss, NULL); //sqrt(1.0/(nn*(nn-1.0)) * ( nn *sumy2 - sumy*sumy) );
    double sxy = (1.0/(nn*(nn-1.0))) * ((nn*sumxy-sumx*sumy));

    double a1a, a1b, a0a, a0b, A, B, C, D, E, F, G, H;


    switch(selection) {
      case CF_LINEAR_FITTING :
        a0 = (sumx2 * sumy  - sumx * sumxy) / (nn * sumx2 - sumx * sumx);
        a1 = (nn  * sumxy - sumx * sumy ) / (nn * sumx2 - sumx * sumx);
        r  = (nn * sumxy - sumx*sumy) / (sqrt(nn*sumx2-sumx*sumx) * sqrt(nn*sumy2-sumy*sumy));
        smi2 = sx*sx*sy*sy*(1-r*r)/(sx*sx+r*r*sy*sy); 
        #ifdef STATDEBUG
          printf("##### Linear %i a0=%f a1=%f \n",nn, a0, a1);
        #endif
        break;

      case CF_EXPONENTIAL_FITTING :
        a0 = exp( (sumx2 * sumlny  - sumx * sumxlny) / (nn * sumx2 - sumx * sumx) );
        a1 = (nn  * sumxlny - sumx * sumlny ) / (nn * sumx2 - sumx * sumx);
        r = (nn * sumxlny - sumx*sumlny) / (sqrt(nn*sumx2-sumx*sumx) * sqrt(nn*sumln2y-sumlny*sumlny)); //(rEXP)
        smi2 = sx*sx*sy*sy*(1-r*r)/(sx*sx+r*r*sy*sy); 
        #ifdef STATDEBUG
          printf("##### EXPF %i a0=%f a1=%f \n",nn, a0, a1);
        #endif
        break;

      case CF_LOGARITHMIC_FITTING :
        a0 = (sumln2x * sumy  - sumlnx * sumylnx) / (nn * sumln2x - sumlnx * sumlnx);
        a1 = (nn  * sumylnx - sumlnx * sumy ) / (nn * sumln2x - sumlnx * sumlnx);
        r = (nn * sumylnx - sumlnx*sumy) / (sqrt(nn*sumln2x-sumlnx*sumlnx) * sqrt(nn*sumy2-sumy*sumy)); //(rLOG)
        smi2 = sx*sx*sy*sy*(1-r*r)/(sx*sx+r*r*sy*sy); 
        #ifdef STATDEBUG
          printf("##### LOGF %i a0=%f a1=%f \n",nn, a0, a1);
        #endif
        break;

      case CF_POWER_FITTING :
        a0 = exp( (sumln2x * sumlny  - sumlnx * sumlnxlny) / (nn * sumln2x - sumlnx * sumlnx) );
        a1 = (nn  * sumlnxlny - sumlnx * sumlny ) / (nn * sumln2x - sumlnx * sumlnx);
        r = (nn * sumlnxlny - sumlnx*sumlny) / (sqrt(nn*sumln2x-sumlnx*sumlnx) * sqrt(nn*sumln2y-sumlny*sumlny)); //(rPOW)
        smi2 = sx*sx*sy*sy*(1-r*r)/(sx*sx+r*r*sy*sy); 
        #ifdef STATDEBUG
          printf("##### POWERF %i a0=%f a1=%f \n",nn, a0, a1);
        #endif
        break;

      case CF_ROOT_FITTING :
        A = nn * sum1onx2 - sum1onx * sum1onx;
        B = 1.0/A * (sum1onx2 * sumlny - sum1onx * sumlnyonx);
        C = 1.0/A * (nn * sumlnyonx - sum1onx * sumlny);

        a0 = exp (B);
        a1 = exp (C);
        r = sqrt ( (B * sumlny + C * sumlnyonx - 1/nn * sumlny * sumlny) / (sumlny*sumlny - 1/nn * sumlny * sumlny) ); //(rROOT)
        smi2 = sx*sx*sy*sy*(1-r*r)/(sx*sx+r*r*sy*sy); 
        #ifdef STATDEBUG
          printf("##### ROOTF %i a0=%f a1=%f \n",nn, a0, a1);
        #endif
        break;

      case CF_HYPERBOLIC_FITTING :
        a0 = (sumx2 * sum1ony - sumx * sumxony) / (nn*sumx2 - sumx * sumx);
        a1 = (nn * sumxony - sumx * sum1ony) / (nn * sumx2 - sumx * sumx);
        r = sqrt ( (a0 * sum1ony + a1 * sumxony - 1/nn * sum1ony * sum1ony) / (sum1ony2 - 1/nn * sum1ony * sum1ony ) ); //(rHYP)
        smi2 = sx*sx*sy*sy*(1-r*r)/(sx*sx+r*r*sy*sy); 
        #ifdef STATDEBUG
          printf("##### HYPF %i a0=%f a1=%f \n",nn, a0, a1);
        #endif
        break;

      case CF_PARABOLIC_FITTING :
        A = nn * sumx2 - sumx * sumx;
        B = nn * sumx2y - sumx2 * sumy;
        C = nn * sumx3 - sumx2 * sumx;
        D = nn * sumxy - sumx * sumy;
        E = nn * sumx4 - sumx2 * sumx2;
        a2 = (A*B - C*D) / (A*E - C*C);
        a1 = (D - a2 * C) / A;
        a0 = 1.0/nn * (sumy - a2 * sumx2 - a1 * sumx);
        r = sqrt( (a0 * sumy + a1 * sumxy + a2 * sumx2y - 1/nn * sumy * sumy) / (sumy2 - 1/nn * sumy * sumy) );
        smi2 = sx*sx*sy*sy*(1-r*r)/(sx*sx+r*r*sy*sy); 
        #ifdef STATDEBUG
          printf("##### PARABF %i a0=%f a1=%f a2=%f\n",nn, a0, a1, a2);
        #endif
        break;

      case CF_GAUSS_FITTING :
        A = nn * sumx2 - sumx * sumx;
printf("A: %u %f %f\n",(int)nn,sumx2,sumx);
        B = nn * sumx2y - sumx2 * sumy;
printf("B: %f %f %f\n",sumx2y,sumx2,sumy);
        C = nn * sumx3 - sumx2 * sumx;
        D = nn * sumxy - sumx * sumy;
        E = nn * sumx4 - sumx2 * sumx2;
        F = (A*B - C*D) / (A*E - C*C);
        G = (D - F * C) / A;
        H = 1.0/nn * (sumlny - F * sumx2 - G * sumx);
        a2 = 1.0/F;
        a1 = -G/2.0 * a2;
        a0 = exp (H - F * a1 * a1);
        printf("A%f B%f C%f D%f E%f F%f G%f H%f   a0:%f a1:%f a2:%f\n",A,B,C,D,E,F,G,H,a0,a1,a2);
r=-1;
//error in the F term        r  = sqrt ( ( H * sumlny + G * sumxlny + F * sumx2lny - 1/nn * sumlny * sumlny ) / (sumlny * sumlny - 1/nn * sumlny * sumlny) );
        smi2 = sx*sx*sy*sy*(1-r*r)/(sx*sx+r*r*sy*sy); 
        #ifdef STATDEBUG
          printf("##### GAUSSF %i a0=%f a1=%f a2=%f\n",nn, a0, a1, a2);
        #endif
        break;

      case CF_CAUCHY_FITTING :
//Unknown constants.
#define R12 1.0
#define R13 1.0

        A = nn * sumx2 - sumx * sumx;
        B = nn * sumx2ony - sumx2 * sum1ony;
        C = nn * sumx3 - sumx * sumx2;
        D = nn * sumxony - sumx * sum1ony;
        E = nn * sumx4 - sumx2 * sumx2;
        F = (A*B - C*D) / (A*E - C*C);
        G = (D - F * C) / A;
        H = 1.0/nn * (sum1ony - R12 * sumx - R13 * sumx2);
        a0 = F;
        a1 = G/2.0 * a0;
        a2 = H - F * a1 * a1;
        r  = sqrt ( (H * sum1ony + G * sumxony + F * sumx2ony - 1/nn * sum1ony * sum1ony) / (sum1ony2 - 1/nn * sum1ony * sum1ony) );
        smi2 = sx*sx*sy*sy*(1-r*r)/(sx*sx+r*r*sy*sy); 
        #ifdef STATDEBUG
          printf("##### GAUSSF %i a0=%f a1=%f a2=%f\n",nn, a0, a1, a2);
        #endif
        break;

      case CF_ORTHOGONAL_FITTING :
        a1a = 1.0 / (2.0 * sxy) * ( sy * sy - sx * sx + sqrt( (sy * sy - sx * sx) * (sy * sy - sx * sx) + 4 * sxy * sxy) );
        a0a  = y_ - a1a * x_;
        //r = sxy / (sx * sy);
        r  = (nn * sumxy - sumx * sumy) / (sqrt (nn * sumx2 - sumx * sumx) * sqrt(nn * sumy2 - sumy * sumy) );
        smi2 = sx*sx*sy*sy*(1-r*r)/(sx*sx+r*r*sy*sy); 
        a1 = a1a;
        a0 = a0a;
        #ifdef STATDEBUG
          printf("##### ORTHOF %i a0=%f a1=%f smi^2=%f\n",nn, a0a, a1a, smi2);
        #endif
        break;

      case CF_ORTHOGONAL_FITTING+10000 :
        a1b = 1.0 / (2.0 * sxy) * ( sy * sy - sx * sx - sqrt( (sy * sy - sx * sx) * (sy * sy - sx * sx) + 4 * sxy * sxy) );
        a0b  = y_ - a1b * x_;
        //r = sxy / (sx * sy);
        r  = (nn * sumxy - sumx * sumy) / (sqrt( nn * sumx2 - sumx * sumx) * sqrt( nn * sumy2 - sumy * sumy));
        smi2 = sx*sx*sy*sy*(1-r*r)/(sx*sx+r*r*sy*sy); 
        a1 = a1b;
        a0 = a0b;
        #ifdef STATDEBUG
          printf("##### ORTHOF %i a0=%f a1=%f smi^2=%f\n",nn, a0b, a1b, smi2);
        #endif
        break;

      default: break;
    }

    graph_axis_draw();                        //Draw the axis 
//  graphPlotstat();
//  drawline();
}




void fnStatDemo(uint16_t unusedButMandatoryParameter){
  runFunction(ITM_CLSIGMA);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1171",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("11",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1171.05",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("6",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1171.1",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("16",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1171.15",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("11",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1171.2",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("8",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1171.25",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("10",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1171.3",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("14",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1171.35",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("11",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1171.4",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("9",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1171.45",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("11",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1171.5",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("13",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1171.55",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("12",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1171.6",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("11",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1171.65",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("10",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1171.7",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("10",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1171.75",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("11",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1171.8",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("14",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1171.85",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("6",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1171.9",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("5",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1171.95",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("11",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1172",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("14",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1172.05",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("9",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1172.1",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("6",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1172.15",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("13",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1172.2",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("12",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1172.25",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("11",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1172.3",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("8",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1172.35",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("3",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1172.4",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("9",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1172.45",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("12",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1172.5",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("17",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1172.55",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("9",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1172.6",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("12",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1172.65",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("18",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1172.7",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("26",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1172.75",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("38",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1172.8",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("55",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1172.85",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("80",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1172.9",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("105",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1172.95",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("197",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1173",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("217",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1173.05",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("302",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1173.1",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("361",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1173.15",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("417",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1173.2",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("485",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1173.25",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("502",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1173.3",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("446",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1173.35",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("415",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1173.4",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("333",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1173.45",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("247",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1173.5",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("209",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1173.55",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("158",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1173.6",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("100",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1173.65",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("62",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1173.7",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("36",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1173.75",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("38",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1173.8",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("14",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1173.85",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("11",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1173.9",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("11",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1173.95",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("10",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1174",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("13",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1174.05",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("15",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1174.1",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("12",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1174.15",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("8",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1174.2",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("9",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1174.25",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("8",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1174.3",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("10",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1174.35",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("7",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1174.4",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("12",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1174.45",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("6",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1174.5",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("11",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1174.55",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("6",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1174.6",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("13",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1174.65",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("15",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1174.7",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("16",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1174.75",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("13",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1174.8",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("11",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1174.85",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("8",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1174.9",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("9",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1174.95",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("12",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1175",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("3",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1175.05",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("8",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1175.1",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("12",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1175.15",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("4",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1175.2",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("5",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1175.25",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("11",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1175.3",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("12",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1175.35",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("7",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1175.4",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("11",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1175.45",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("9",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1175.5",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("12",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1175.55",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("14",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1175.6",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("12",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1175.65",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("10",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1175.7",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("10",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1175.75",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("6",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1175.8",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("9",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1175.85",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("7",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1175.9",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("13",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1175.95",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("10",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
  }






void fnStatDemo1(uint16_t unusedButMandatoryParameter){
  int8_t ix;
  runFunction(ITM_CLSIGMA);
  srand((unsigned int)time(NULL));

      for(ix=0; ix!=10; ix++) {
        runFunction(ITM_RAN);
        setSystemFlag(FLAG_ASLIFT);
        liftStack();
        reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
        int32ToReal34(2000+ix/2,REGISTER_REAL34_DATA(REGISTER_X));
        runFunction(ITM_ADD);

        runFunction(ITM_RAN);
        setSystemFlag(FLAG_ASLIFT);
        liftStack();
        reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
        int32ToReal34(ix,REGISTER_REAL34_DATA(REGISTER_X));
        runFunction(ITM_ADD);

        runFunction(ITM_SIGMAPLUS);
        }


}


