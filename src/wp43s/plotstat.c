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


// This module is part of the C43 fork, and is copied here. 
// Do not change the shared functions otherwise the C43 fork will break. JM 2021-03-20

#define   zoomfactor 0.05      // default is 0.05, which is 5% space around the data points
#define   numberIntervals 1500  // default 100,
#define   fittedcurveboxes 0   // default 0 = smooth line

//#define STATDEBUG

#define ld long double

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
    int16_t temp; graphtype tempr;
    if (Aspect_Square) {
      tempr = ((x-x_min)/(x_max-x_min)*(graphtype)(SCREEN_HEIGHT_GRAPH-1.0));
      temp = (int16_t) tempr;
      if (temp>SCREEN_HEIGHT_GRAPH-1) {
        temp=SCREEN_HEIGHT_GRAPH-1;
      }
      else if (temp<0) {temp=0;}
      return temp+SCREEN_WIDTH-SCREEN_HEIGHT_GRAPH;
    } 
    else {  //FULL SCREEN
      tempr = ((x-x_min)/(x_max-x_min)*(graphtype)(SCREEN_WIDTH_GRAPH-1.0));
      temp = (int16_t) tempr;
      //printf("--> %d (%f %f)  ",temp, x_min,x_max);
      if (temp>SCREEN_WIDTH_GRAPH-1) {
        temp=SCREEN_WIDTH_GRAPH-1;
      }
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
    graphtype tempr;
    if (!Aspect_Square) minn = SCREEN_NONSQ_HMIN;
    else minn = 0;

    tempr = ((y-y_min)/(y_max-y_min)*(graphtype)(SCREEN_HEIGHT_GRAPH-1.0 - minn));
    temp = (int16_t) tempr;
    if (temp>SCREEN_HEIGHT_GRAPH-1 - minn) {
      temp=SCREEN_HEIGHT_GRAPH-1 - minn;
    }
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
    if(xo == xn && yo == yn) {
      if(vmNormal) placePixel(xn,yn); else removePixel(xn,yn);
      return;
    } 
    else
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
    printf("xzero=%d yzero=%d   \n",(int)xzero,(int)yzero);
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


void eformat (char* s02, char* s01, double inreal, uint8_t prec) {
  char s03[100]; char s04[100];
  if(fabs(inreal) > 1000000.0 || fabs(inreal) < 0.001) {
    sprintf(s03,"%.*e",prec,inreal);
  } else {
    sprintf(s03,"%.*f",prec,inreal);
  }
  strcpy(s04,s01);
  if(inreal > 0) strcat(s04," ");  //in place of negative sign
  strcat(s04,s03);
  strcpy(s02,s04);
}

double a2 = 0;
double a1 = 0;
double a0 = 0;
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
  
    graphtype dx = x_max-x_min;
    graphtype dy = y_max-y_min;
  
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
    
    x_min = x_min - dx * zoomfactor;
    y_min = y_min - dy * zoomfactor;
    x_max = x_max + dx * zoomfactor;
    y_max = y_max + dy * zoomfactor;
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
      if (!Aspect_Square) {
        minN_y = SCREEN_NONSQ_HMIN; 
        minN_x = 0;
      }
      else {
        minN_y = 0; 
        minN_x = SCREEN_WIDTH-SCREEN_HEIGHT_GRAPH;
      }
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
          printf("Not plotted point: (%u %u) ",xN,yN);
          if(!(xN < SCREEN_WIDTH_GRAPH ))  printf("x>>%u ",SCREEN_WIDTH_GRAPH); else
          if(!(xN > minN_x              )) printf("x<<%u ",minN_x);
          if(!(yN < SCREEN_HEIGHT_GRAPH))  printf("y>>%u ",SCREEN_HEIGHT_GRAPH); else
          if(!(yN > 1+minN_y            )) printf("y<<%u ",1+minN_y);
          printf("\n");
          //printf("Not plotted: xN=%d<SCREEN_WIDTH_GRAPH=%d && xN=%d>minN_x=%d && yN=%d<SCREEN_HEIGHT_GRAPH=%d && yN=%d>1+minN_y=%d\n",xN,SCREEN_WIDTH_GRAPH,xN,minN_x,yN,SCREEN_HEIGHT_GRAPH,yN,1+minN_y);
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


real_t        SS,TT,UU,aa0,aa1,aa2;
realContext_t *realContext = &ctxtReal75;
uint16_t      selection = 0;

#ifndef TESTSUITE_BUILD
  void drawline(){
    if(!selection) return;
    char ss[100];
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
    double    x = 0; 
    double    y = 0;

    for (ix = 0; ix <= numberIntervals; ++ix) {
      x = x_min + (x_max-x_min)/(double)numberIntervals * (double)ix;

      switch(selection) {
        case CF_LINEAR_FITTING: 
        case CF_ORTHOGONAL_FITTING: 
        case CF_ORTHOGONAL_FITTING+10000: 
          //         y = a1 * x + a0; 
          sprintf(ss,"%f",x); stringToReal(ss,&SS,realContext);
          realMultiply(&SS, &aa1, &UU, realContext);
          realAdd     (&UU, &aa0, &TT, realContext);
          realToString(&TT, ss); y = strtof (ss, NULL);
          //printf("XXX0 %f %f\n",y, a1 * x + a0);
          break;
        case CF_EXPONENTIAL_FITTING: 
          //         y = a0 * exp(a1 * x); 
          sprintf(ss,"%f",x); stringToReal(ss,&SS,realContext);
          realMultiply(&SS, &aa1, &UU, realContext);
          realExp     (&UU, &UU,       realContext);
          realMultiply(&UU, &aa0, &TT, realContext);
          realToString(&TT, ss); y = strtof (ss, NULL);
          //printf("XXX1 %f %f\n",y, a0 * exp(a1 * x));
          break;
        case CF_LOGARITHMIC_FITTING: 
          //         y = a0 + a1*log(x); 
          sprintf(ss,"%f",x); stringToReal(ss,&SS,realContext);
          WP34S_Ln    (&SS, &SS,       realContext);
          realMultiply(&SS, &aa1, &UU, realContext);
          realAdd     (&UU, &aa0, &TT, realContext);
          realToString(&TT, ss); y = strtof (ss, NULL);
          break;
        case CF_POWER_FITTING: 
          //         y = a0 * pow(x,a1);
          sprintf(ss,"%f",x); stringToReal(ss,&SS,realContext);
          realPower   (&SS, &aa1, &SS, realContext);
          realMultiply(&SS, &aa0, &TT, realContext);
          realToString(&TT, ss); y = strtof (ss, NULL);
          break;
        case CF_ROOT_FITTING: 
          //         y = a0 * pow(a1,1/x);
          sprintf(ss,"%f",x); stringToReal(ss,&SS,realContext);
          realDivide  (const_1, &SS, &SS, realContext);
          realPower   (&aa1, &SS, &SS, realContext);
          realMultiply(&SS, &aa0, &TT, realContext);
          realToString(&TT, ss); y = strtof (ss, NULL);
          break;
        case CF_HYPERBOLIC_FITTING: 
          //       y = 1 / (a1 * x + a0);
          sprintf(ss,"%f",x); stringToReal(ss,&SS,realContext);
          realMultiply(&SS, &aa1, &UU, realContext);
          realAdd     (&UU, &aa0, &TT, realContext);
          realDivide  (const_1, &TT, &TT, realContext);
          realToString(&TT, ss); y = strtof (ss, NULL);
          break;
        case CF_PARABOLIC_FITTING: 
          //          y = a2 * x * x + a1 * x + a0;
          sprintf(ss,"%f",x); stringToReal(ss,&SS,realContext);
          realMultiply(&SS, &SS , &TT, realContext);
          realMultiply(&TT, &aa2, &TT, realContext);
          realMultiply(&SS, &aa1, &UU, realContext);
          realAdd     (&TT, &UU,  &TT, realContext);
          realAdd     (&TT, &aa0, &TT, realContext);          
          realToString(&TT, ss); y = strtof (ss, NULL);
          break;
        case CF_GAUSS_FITTING:
          //          y = a0 * exp( (x-a1)*(x-a1)/a2 );
          sprintf(ss,"%f",x); stringToReal(ss,&SS,realContext);
          realSubtract(&SS, &aa1, &TT, realContext);
          realMultiply(&TT, &TT , &TT, realContext);
          realDivide  (&TT, &aa2, &TT, realContext);
          realExp     (&TT, &TT ,      realContext);
          realMultiply(&TT, &aa0, &TT, realContext);
          realToString(&TT, ss); y = strtof (ss, NULL);
          break;
        case CF_CAUCHY_FITTING:
          y = 1/(a0*(x+a1)*(x+a1)+a2);
        default:break;
      }
      xo = xN;
      yo = yN;
      xN = screen_window_x(x_min,x,x_max);
      yN = screen_window_y(y_min,y,y_max);

      if(ix > 0) {  //Allow for starting values to accumulate at ix = 0
        #ifdef STATDEBUG
          printf("plotting graph sample no %d ==>xmin:%f (x:%f) xmax:%f ymin:%f (y:%f) ymax:%f xN:%d yN:%d \n",ix,x_min,x,x_max,y_min,y,y_max,  xN,yN);
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
          if(fittedcurveboxes) plotbox(xn,yn);
          if(xo < SCREEN_WIDTH_GRAPH && xo > minN_x && yo < SCREEN_HEIGHT_GRAPH-tol && yo > minN_y) {
            #ifdef STATDEBUG
              printf("Plotting line to x=%d y=%d\n",xn,yn);
            #endif
            plotline(xo, yo, xn, yn);
          }
        } 
        else {
          #ifdef STATDEBUG
            printf("Not plotted line: (%u %u) ",xN,yN);
            if(!(xN < SCREEN_WIDTH_GRAPH ))  printf("x>>%u ",SCREEN_WIDTH_GRAPH); else
            if(!(xN > minN_x              )) printf("x<<%u ",minN_x);
            if(!(yN < SCREEN_HEIGHT_GRAPH))  printf("y>>%u ",SCREEN_HEIGHT_GRAPH); else
            if(!(yN > 1+minN_y            )) printf("y<<%u ",1+minN_y);
            printf("\n");
          #endif //STATDEBUG
        }
      }
    }

    #define autoinc 18 //text line spacing
    int16_t index = 0;
    if(selection!=0) {
      strcpy(ss,getCurveFitModeName(selection%10000));
      if(selection == CF_ORTHOGONAL_FITTING      ) strcat(ss,"(+)"); else
      if(selection == CF_ORTHOGONAL_FITTING+10000) strcat(ss,"(-)");
      showString(ss, &standardFont, 0, Y_POSITION_OF_REGISTER_Z_LINE + autoinc * index++ -5, vmNormal, true, true);
      sprintf(ss,"n=%d",(int)nn);    showString(ss, &standardFont, 0, Y_POSITION_OF_REGISTER_Z_LINE + autoinc*index++, vmNormal, true, true);
      eformat(ss,"a0=",a0,7);        showString(ss, &standardFont, 0, Y_POSITION_OF_REGISTER_Z_LINE + autoinc*index++, vmNormal, true, true);
      eformat(ss,"a1=",a1,7);        showString(ss, &standardFont, 0, Y_POSITION_OF_REGISTER_Z_LINE + autoinc*index++, vmNormal, true, true);

      if(selection == CF_PARABOLIC_FITTING || selection == CF_GAUSS_FITTING || selection == CF_CAUCHY_FITTING){ 
        eformat(ss,"a2=",a2,7);      showString(ss, &standardFont, 0, Y_POSITION_OF_REGISTER_Z_LINE + autoinc*index++, vmNormal, true, true);
      }
      eformat(ss,"r^2=",r*r,4);      showString(ss, &standardFont, 0, Y_POSITION_OF_REGISTER_Z_LINE + autoinc*index++, vmNormal, true, true);

      if((selection == CF_ORTHOGONAL_FITTING) || (selection == CF_ORTHOGONAL_FITTING+10000)) {
        eformat(ss,"smi^2=",smi2,4); showString(ss, &standardFont, 0, Y_POSITION_OF_REGISTER_Z_LINE + autoinc*index++, vmNormal, true, true);
      }
    }
  }
#endif //TESTSUITE_BUILD


void fnPlotClose(uint16_t unusedButMandatoryParameter){
  calcMode = CM_NORMAL;
  fnKeyExit(0);
}


void fnPlotStat(uint16_t unusedButMandatoryParameter){
#ifndef TESTSUITE_BUILD
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

  if(softmenu[softmenuStack[0].softmenuId].menuItem != -MNU_PLOT_STAT) {
    showSoftmenu(-MNU_PLOT_STAT);
  }
#endif //TESTSUITE_BUILD
}



double sumx     ;
double sumy     ;
double sumx2    ;
double sumx2y   ;
double sumy2    ;
double sumxy    ;
double sumlnxlny;
double sumx2lny ;
double sumlnx   ;
double sumln2x  ;
double sumylnx  ;
double sumlny   ;
double sumln2y  ;
double sumxlny  ;
double sumlnyonx;
double sumx2ony ;
double sum1onx  ;
double sum1onx2 ;
double sumxony  ;
double sum1ony  ;
double sum1ony2 ;
double sumx3    ;
double sumx4    ;
double maxy     ;
double A, B, C, D, E, F, G, H;
real_t AA,BB,CC,DD,EE,FF,GG,HH;

void calc_AEFG(void){                        //Must be run after calc_BCD
char ss[100];
  //        A = nn * sumx2 - sumx * sumx;
  int32ToReal(nn, &SS);
  realMultiply(&SS, SIGMA_X2, &SS, realContext);
  realMultiply(SIGMA_X, SIGMA_X, &TT, realContext);
  realSubtract(&SS, &TT, &AA, realContext);
  realToString(&AA,ss); A = strtof (ss, NULL);
  printf("§ A: %f %f\n",A,nn * sumx2 - sumx * sumx);

  //        E = nn * sumx4 - sumx2 * sumx2;
  int32ToReal(nn, &SS);
  realMultiply(&SS, SIGMA_X4, &SS, realContext);
  realMultiply(SIGMA_X2, SIGMA_X2, &TT, realContext);
  realSubtract(&SS, &TT, &EE, realContext);
  realToString(&EE,ss); E = strtof (ss, NULL);
  printf("§ E: %f %f\n",E,nn * sumx4 - sumx2 * sumx2);

  //USING COMPONENTS OF BCD
  //        F = (A*B - C*D) / (A*E - C*C);    //interchangably the a2 in PARABF
  realMultiply(&AA, &BB, &SS, realContext);
  realMultiply(&CC, &DD, &TT, realContext);
  realSubtract(&SS, &TT, &UU, realContext);
  realMultiply(&AA, &EE, &SS, realContext);
  realMultiply(&CC, &CC, &TT, realContext);
  realSubtract(&SS, &TT, &SS, realContext);
  realDivide  (&UU,&SS,&FF,realContext);
  realToString(&FF,ss); F = strtof (ss, NULL);
  printf("§ F: %f %f\n",F,(A*B - C*D) / (A*E - C*C));

  //        G = (D - F * C) / A;
  realMultiply(&FF, &CC, &SS, realContext);
  realSubtract(&DD, &SS, &SS, realContext);
  realDivide  (&SS,&AA,&GG,realContext);
  realToString(&GG,ss); G = strtof (ss, NULL);
  printf("§ G: %f %f\n",G,(D - F * C) / A);
}


void calc_BCD(void){                        //Must be run before calc_AEFG
char ss[100];
  //        B = nn * sumx2y - sumx2 * sumy;
  int32ToReal(nn, &SS);
  realMultiply(&SS, SIGMA_X2Y, &SS, realContext);
  realMultiply(SIGMA_X2, SIGMA_Y, &TT, realContext);
  realSubtract(&SS, &TT, &BB, realContext);
  realToString(&BB,ss); B = strtof (ss, NULL);
  printf("§ B: %f %f\n",B,nn * sumx2y - sumx2 * sumy);

  //        C = nn * sumx3 - sumx2 * sumx;
  int32ToReal(nn, &SS);
  realMultiply(&SS, SIGMA_X3, &SS, realContext);
  realMultiply(SIGMA_X2, SIGMA_X, &TT, realContext);
  realSubtract(&SS, &TT, &CC, realContext);
  realToString(&CC,ss); C = strtof (ss, NULL);
  printf("§ C: %f %f\n",C,nn * sumx3 - sumx2 * sumx);

  //        D = nn * sumxy - sumx * sumy;
  int32ToReal(nn, &SS);
  realMultiply(&SS, SIGMA_XY, &SS, realContext);
  realMultiply(SIGMA_X, SIGMA_Y, &TT, realContext);
  realSubtract(&SS, &TT, &DD, realContext);
  realToString(&DD,ss); D = strtof (ss, NULL);
  printf("§ D: %f %f\n",D,nn * sumxy - sumx * sumy);
}


void fnPlotRegLine(uint16_t unusedButMandatoryParameter){
    //Cycling through the graphs, starting with ORTHOF
    if(selection == 0                           ) selection = CF_ORTHOGONAL_FITTING ;     else
    if(selection == CF_ORTHOGONAL_FITTING       ) selection = CF_ORTHOGONAL_FITTING+10000;else
    if(selection == CF_ORTHOGONAL_FITTING+10000 ) selection = CF_LINEAR_FITTING     ;     else
    if(selection == CF_LINEAR_FITTING           ) selection = CF_EXPONENTIAL_FITTING;     else
    if(selection == CF_EXPONENTIAL_FITTING      ) selection = CF_PARABOLIC_FITTING  ;     else
    if(selection == CF_PARABOLIC_FITTING        ) selection = CF_LOGARITHMIC_FITTING;     else
    if(selection == CF_LOGARITHMIC_FITTING      ) selection = CF_POWER_FITTING      ;     else
    if(selection == CF_POWER_FITTING            ) selection = CF_ROOT_FITTING       ;     else
    if(selection == CF_ROOT_FITTING             ) selection = CF_HYPERBOLIC_FITTING ;     else
    if(selection == CF_HYPERBOLIC_FITTING       ) selection = CF_CAUCHY_FITTING     ;     else
    if(selection == CF_CAUCHY_FITTING           ) selection = CF_GAUSS_FITTING      ;     else
    if(selection == CF_GAUSS_FITTING            ) selection = 0                     ;     else
       selection = 0;
  
    char ss[100];
    realToInt32(SIGMA_N, nn);  
    realToString(SIGMA_X ,     ss); sumx      = strtof (ss, NULL);
    realToString(SIGMA_Y ,     ss); sumy      = strtof (ss, NULL);
    realToString(SIGMA_X2,     ss); sumx2     = strtof (ss, NULL);
    realToString(SIGMA_X2Y,    ss); sumx2y    = strtof (ss, NULL);
    realToString(SIGMA_Y2,     ss); sumy2     = strtof (ss, NULL);
    realToString(SIGMA_XY,     ss); sumxy     = strtof (ss, NULL);
    realToString(SIGMA_lnXlnY, ss); sumlnxlny = strtof (ss, NULL);
    realToString(SIGMA_lnX ,   ss); sumlnx    = strtof (ss, NULL);
    realToString(SIGMA_ln2X,   ss); sumln2x   = strtof (ss, NULL);
    realToString(SIGMA_YlnX,   ss); sumylnx   = strtof (ss, NULL);
    realToString(SIGMA_lnY ,   ss); sumlny    = strtof (ss, NULL);
    realToString(SIGMA_ln2Y,   ss); sumln2y   = strtof (ss, NULL);
    realToString(SIGMA_XlnY,   ss); sumxlny   = strtof (ss, NULL);
    realToString(SIGMA_X2lnY,  ss); sumx2lny  = strtof (ss, NULL);
    realToString(SIGMA_lnYonX, ss); sumlnyonx = strtof (ss, NULL);
    realToString(SIGMA_X2onY,  ss); sumx2ony  = strtof (ss, NULL);
    realToString(SIGMA_1onX,   ss); sum1onx   = strtof (ss, NULL);
    realToString(SIGMA_1onX2,  ss); sum1onx2  = strtof (ss, NULL);
    realToString(SIGMA_XonY,   ss); sumxony   = strtof (ss, NULL);
    realToString(SIGMA_1onY,   ss); sum1ony   = strtof (ss, NULL);
    realToString(SIGMA_1onY2,  ss); sum1ony2  = strtof (ss, NULL);
    realToString(SIGMA_X3,     ss); sumx3     = strtof (ss, NULL);
    realToString(SIGMA_X4,     ss); sumx4     = strtof (ss, NULL);
    realToString(SIGMA_YMAX,   ss); maxy      = strtof (ss, NULL);

    fnMeanXY(0);
    real34ToString(REGISTER_REAL34_DATA(REGISTER_X), ss); double x_ = strtof (ss, NULL);
    real34ToString(REGISTER_REAL34_DATA(REGISTER_Y), ss); double y_ = strtof (ss, NULL);

    fnSampleStdDev(0);
    real34ToString(REGISTER_REAL34_DATA(REGISTER_X), ss); double sx = strtof (ss, NULL); //sqrt(1.0/(nn*(nn-1.0)) * ( nn *sumx2 - sumx*sumx) );
    real34ToString(REGISTER_REAL34_DATA(REGISTER_Y), ss); double sy = strtof (ss, NULL); //sqrt(1.0/(nn*(nn-1.0)) * ( nn *sumy2 - sumy*sumy) );
    double sxy  = (1.0/(nn*(nn-1.0))) * ((nn*sumxy-sumx*sumy));
           smi2 = sx*sx*sy*sy*(1.0-r*r)/(sx*sx+r*r*sy*sy); 
           r    = sxy / (sx * sy);
    printf(">>> sx=%f\n",sx);
    printf(">>> sy=%f\n",sy);
    printf(">>> sxy=%f\n",sxy);
    printf(">>> smi2=%f\n",smi2);
    printf(">>> r = %f\n",r);


    double a1a, a1b, a0a, a0b;

    switch(selection) {
      case CF_LINEAR_FITTING :
        a0 = (sumx2 * sumy  - sumx * sumxy) / (nn * sumx2 - sumx * sumx);
  sprintf(ss,"%f",a0); stringToReal(ss,&aa0,realContext);
        a1 = (nn  * sumxy - sumx * sumy ) / (nn * sumx2 - sumx * sumx);
  sprintf(ss,"%f",a1); stringToReal(ss,&aa1,realContext);
        r  = (nn * sumxy - sumx*sumy) / (sqrt(nn*sumx2-sumx*sumx) * sqrt(nn*sumy2-sumy*sumy));
        smi2 = sx*sx*sy*sy*(1-r*r)/(sx*sx+r*r*sy*sy); 
        #ifdef STATDEBUG
          printf("##### Linear %i a0=%f a1=%f \n",(int)nn, a0, a1);
        #endif
        break;

      case CF_EXPONENTIAL_FITTING :
        a0 = exp( (sumx2 * sumlny  - sumx * sumxlny) / (nn * sumx2 - sumx * sumx) );
  sprintf(ss,"%f",a0); stringToReal(ss,&aa0,realContext);
        a1 = (nn  * sumxlny - sumx * sumlny ) / (nn * sumx2 - sumx * sumx);
  sprintf(ss,"%f",a1); stringToReal(ss,&aa1,realContext);
        r = (nn * sumxlny - sumx*sumlny) / (sqrt(nn*sumx2-sumx*sumx) * sqrt(nn*sumln2y-sumlny*sumlny)); //(rEXP)
        smi2 = sx*sx*sy*sy*(1-r*r)/(sx*sx+r*r*sy*sy); 
        #ifdef STATDEBUG
          printf("##### EXPF %i a0=%f a1=%f \n",(int)nn, a0, a1);
        #endif
        break;

      case CF_LOGARITHMIC_FITTING :
        a0 = (sumln2x * sumy  - sumlnx * sumylnx) / (nn * sumln2x - sumlnx * sumlnx);
  sprintf(ss,"%f",a0); stringToReal(ss,&aa0,realContext);
        a1 = (nn  * sumylnx - sumlnx * sumy ) / (nn * sumln2x - sumlnx * sumlnx);
  sprintf(ss,"%f",a1); stringToReal(ss,&aa1,realContext);
        r = (nn * sumylnx - sumlnx*sumy) / (sqrt(nn*sumln2x-sumlnx*sumlnx) * sqrt(nn*sumy2-sumy*sumy)); //(rLOG)
        smi2 = sx*sx*sy*sy*(1-r*r)/(sx*sx+r*r*sy*sy); 
        #ifdef STATDEBUG
          printf("##### LOGF %i a0=%f a1=%f \n",(int)nn, a0, a1);
        #endif
        break;

      case CF_POWER_FITTING :
        a0 = exp( (sumln2x * sumlny  - sumlnx * sumlnxlny) / (nn * sumln2x - sumlnx * sumlnx) );
  sprintf(ss,"%f",a0); stringToReal(ss,&aa0,realContext);
        a1 = (nn  * sumlnxlny - sumlnx * sumlny ) / (nn * sumln2x - sumlnx * sumlnx);
  sprintf(ss,"%f",a1); stringToReal(ss,&aa1,realContext);
        r = (nn * sumlnxlny - sumlnx*sumlny) / (sqrt(nn*sumln2x-sumlnx*sumlnx) * sqrt(nn*sumln2y-sumlny*sumlny)); //(rPOW)
        #ifdef STATDEBUG
          printf("##### POWERF %i a0=%f a1=%f \n",nn, a0, a1);
        #endif
        break;

      case CF_ROOT_FITTING :
        A = nn * sum1onx2 - sum1onx * sum1onx;
        B = 1.0/A * (sum1onx2 * sumlny - sum1onx * sumlnyonx);
        C = 1.0/A * (nn * sumlnyonx - sum1onx * sumlny);

        a0 = exp (B);
  sprintf(ss,"%f",a0); stringToReal(ss,&aa0,realContext);
        a1 = exp (C);
  sprintf(ss,"%f",a1); stringToReal(ss,&aa1,realContext);
        r = sqrt ( (B * sumlny + C * sumlnyonx - 1.0/nn * sumlny * sumlny) / (sumlny*sumlny - 1.0/nn * sumlny * sumlny) ); //(rROOT)
        #ifdef STATDEBUG
          printf("##### ROOTF %i a0=%f a1=%f \n",(int)nn, a0, a1);
        #endif
        break;

      case CF_HYPERBOLIC_FITTING :
        a0 = (sumx2 * sum1ony - sumx * sumxony) / (nn*sumx2 - sumx * sumx);
  sprintf(ss,"%f",a0); stringToReal(ss,&aa0,realContext);
        a1 = (nn * sumxony - sumx * sum1ony) / (nn * sumx2 - sumx * sumx);
  sprintf(ss,"%f",a1); stringToReal(ss,&aa1,realContext);
        r = sqrt ( (a0 * sum1ony + a1 * sumxony - 1.0/nn * sum1ony * sum1ony) / (sum1ony2 - 1.0/nn * sum1ony * sum1ony ) ); //(rHYP)
        #ifdef STATDEBUG
          printf("##### HYPF %i a0=%f a1=%f \n",(int)nn, a0, a1);
        #endif
        break;

      case CF_PARABOLIC_FITTING :
        printf("Parabolic fit:\n");
        calc_BCD();
        calc_AEFG();

        //      a2 = F; //a2 = (A*B - C*D) / (A*E - C*C) = F. Not in ReM, but the formula is correct and prevents duplicate code.
        realCopy (&FF,&aa2);
        realToString(&aa2,ss); a2 = strtof (ss, NULL);
        printf("§ a2: %f %f\n",a2,F);

        //      a1 = G; //a1 = (D - a2 * C) / A = G; Not in ReM, but the formula is correct and prevents duplicate code.
        realCopy (&GG,&aa1);
        realToString(&aa1,ss); a1 = strtof (ss, NULL);
        printf("§ a1: %f %f\n",a1,(D - a2 * C) / A);

        //      a0 = 1.0/nn * (sumy - a2 * sumx2 - a1 * sumx);
        realMultiply(&FF, SIGMA_X2, &TT, realContext);
        realSubtract(SIGMA_Y , &TT, &HH, realContext);
        realMultiply(&GG, SIGMA_X, &TT, realContext);
        realSubtract(&HH, &TT, &HH, realContext);
        int32ToReal (nn, &SS);
        realDivide  (&HH,&SS,&aa0,realContext);
        realToString(&aa0,ss); a0 = strtof (ss, NULL);
        printf("§ a0: %f %f\n",a0,1.0/nn * (sumy - a2 * sumx2 - a1 * sumx));

        //      r = sqrt( (a0 * sumy + a1 * sumxy + a2 * sumx2y - 1.0/nn * sumy * sumy) / (sumy2 - 1.0/nn * sumy * sumy) );
        realMultiply(&aa0, SIGMA_Y, &SS, realContext);
        realMultiply(&aa1, SIGMA_XY, &TT, realContext);
        realAdd     (&SS, &TT,&SS,realContext);
        realMultiply(&aa2, SIGMA_X2Y, &TT, realContext);
        realAdd     (&SS, &TT,&SS,realContext); //interim in SS
        realMultiply(SIGMA_Y, SIGMA_Y, &TT, realContext);
        int32ToReal (nn,  &UU);
        realDivide  (&TT, &UU,&UU,realContext);
        realSubtract(&SS, &UU,&SS,realContext); //top in SS
        realSubtract(SIGMA_Y2, &UU,&TT,realContext);
        realDivide  (&SS, &TT,&SS,realContext); //R^2
        realSquareRoot(&SS, &SS, realContext);
        realToString(&SS, ss); r = strtof (ss, NULL);
        printf("§ r^2: %f r: %f %f\n",r*r, r,sqrt( (a0 * sumy + a1 * sumxy + a2 * sumx2y - 1.0/nn * sumy * sumy) / (sumy2 - 1.0/nn * sumy * sumy) ) );

        #ifdef STATDEBUG
          printf("##### PARABF %i a0=%f a1=%f a2=%f\n",(int)nn, a0, a1, a2);
        #endif
        break;

      case CF_GAUSS_FITTING :
        printf("Gauss fit:\n");
        calc_BCD();
        calc_AEFG();

        //        H = (1.0)/nn * (sumlny - F * sumx2 - G * sumx);
        realMultiply(&FF, SIGMA_X2, &TT, realContext);
        realSubtract(SIGMA_lnY, &TT, &HH, realContext);
        realMultiply(&GG, SIGMA_X, &TT, realContext);
        realSubtract(&HH, &TT, &HH, realContext);
        int32ToReal (nn, &SS);
        realDivide  (&HH,&SS,&HH,realContext);
        realToString(&HH,ss); H = strtof (ss, NULL);
        printf("§ H: %f %f\n",H,(1.0)/nn * (sumlny - F * sumx2 - G * sumx));

        //        a2 = (1.0)/F;
        realDivide  (const_1,&FF,&aa2,realContext);
        realToString(&aa2,ss); a2 = strtof (ss, NULL);
        printf("§ a2: %f %f\n",a2,(1.0)/F);

        //        a1 = -G/(2.0) * a2;
        realDivide  (&GG,const__1,&TT,realContext);
        realDivide  (&TT,const_2 ,&TT,realContext);
        realMultiply(&TT, &aa2, &aa1, realContext);
        realToString(&aa1,ss); a1 = strtof (ss, NULL);
        printf("§ a1: %f %f\n",a1,-G/(2.0) * a2);

#ifdef AAA
        //        a0 = maxy;  // exp (H - F * a1 * a1); //maxy;
        realCopy    (SIGMA_YMAX,&aa0);
        realToString(&aa0,ss); a0 = strtof (ss, NULL);
        printf("§ a0: %f %f\n",a0,maxy);
#endif //AAA

        //GAUSS  a0 = exp (H - F * a1 * a1); //maxy;
        realMultiply(&aa1,&aa1,&SS,realContext);
        realMultiply(&SS,&FF,&SS,realContext);
        realSubtract(&HH,&SS,&aa0,realContext);
        realExp(&aa0,&aa0,realContext);           
        realToString(&aa0,ss); a0 = strtof (ss, NULL);
        printf("§ a0: %f %f\n",a0, exp (H - F * a1 * a1));

        //        r  = sqrt ( ( H * sumlny + G * sumxlny + F * sumx2lny - 1.0/nn * sumlny * sumlny ) / (sumlny * sumlny - 1.0/nn * sumlny * sumlny) );
        realMultiply(&HH, SIGMA_lnY, &SS, realContext);
        realMultiply(&GG, SIGMA_XlnY, &TT, realContext);
        realAdd     (&SS, &TT,&SS,realContext);
        realMultiply(&FF, SIGMA_X2lnY, &TT, realContext);
        realAdd     (&SS, &TT,&SS,realContext); //interim in SS
        realMultiply(SIGMA_lnY, SIGMA_lnY, &TT, realContext);
        int32ToReal (nn,  &UU);
        realDivide  (&TT, &UU,&UU,realContext);
        realSubtract(&SS, &UU,&SS,realContext); //top in SS

        realSubtract(&TT, &UU,&TT,realContext); //use both TT and UU from above
        realDivide  (&SS, &TT,&SS,realContext); //R^2
        realSquareRoot(&SS, &SS, realContext);
        realToString(&SS, ss); r = strtof (ss, NULL);
        printf("§ r^2: %f r: %f %f\n",r*r, r,sqrt ( ( H * sumlny + G * sumxlny + F * sumx2lny - 1.0/nn * sumlny * sumlny ) / (sumlny * sumlny - 1.0/nn * sumlny * sumlny) ));
        

        #ifdef STATDEBUG
          printf("A%f B%f C%f D%f E%f F%f G%f H%f   a0:%f a1:%f a2:%f r:%f r^2:%f\n",A,B,C,D,E,F,G,H,a0,a1,a2,r,r*r);
        #endif
        #ifdef STATDEBUG
          printf("##### GAUSSF %i a0=%f a1=%f a2=%f\n",(int)nn, a0, a1, a2);
        #endif
        break;





      case CF_CAUCHY_FITTING :
        printf("Cauchy fit:\n");

//Unknown constants.
// R12 Assuming G
// R13 Assuming F

    //    r  = sqrt ( (H * sum1ony + G * sumxony + F * sumx2ony - 1.0/nn * sum1ony * sum1ony) / (sum1ony2 - 1.0/nn * sum1ony * sum1ony) );

        
        //      B = nn * sumx2ony - sumx2 * sum1ony;        
        int32ToReal(nn, &SS);
        realMultiply(&SS, SIGMA_X2onY, &SS, realContext);
        realMultiply(SIGMA_X2, SIGMA_1onY, &TT, realContext);
        realSubtract(&SS, &TT, &BB, realContext);
        realToString(&BB,ss); B = strtof (ss, NULL);
        printf("§ B: %f %f\n",B,nn * sumx2ony - sumx2 * sum1ony);

        //      C = nn * sumx3 - sumx * sumx2;                     //C copied from PARABF. Exactly the same
        int32ToReal(nn, &SS);
        realMultiply(&SS, SIGMA_X3, &SS, realContext);
        realMultiply(SIGMA_X2, SIGMA_X, &TT, realContext);
        realSubtract(&SS, &TT, &CC, realContext);
        realToString(&CC,ss); C = strtof (ss, NULL);
        printf("§ C: %f %f\n",C,nn * sumx3 - sumx * sumx2);

        //      D = nn * sumxony - sumx * sum1ony;
        int32ToReal(nn, &SS);
        realMultiply(&SS, SIGMA_XonY, &SS, realContext);
        realMultiply(SIGMA_X, SIGMA_1onY, &TT, realContext);
        realSubtract(&SS, &TT, &DD, realContext);
        realToString(&DD,ss); D = strtof (ss, NULL);
        printf("§ D: %f %f\n",D,nn * sumxony - sumx * sum1ony);

        calc_AEFG();

        //    H = 1.0/nn * (sum1ony - R12 * sumx - R13 * sumx2);
        //old GAUSS CHANGED TO 1/y ; F and G left:      H = (1.0)/nn * (sum1ony - F * sumx2 - G * sumx);
        realMultiply(&FF, SIGMA_X2, &TT, realContext);
        realSubtract(SIGMA_1onY , &TT, &HH, realContext);
        realMultiply(&GG, SIGMA_X, &TT, realContext);
        realSubtract(&HH, &TT, &HH, realContext);
        int32ToReal (nn, &SS);
        realDivide  (&HH,&SS,&HH,realContext);
        realToString(&HH,ss); H = strtof (ss, NULL);
        printf("§ H: %f %f\n",H,(1.0)/nn * (sumlny - F * sumx2 - G * sumx));



      //  a0 = F;
        realCopy    (&FF,&aa0);
//realCopy(SIGMA_YMAX,&aa0);
        realToString(&aa0,ss); a0 = strtof (ss, NULL);
        printf("§ a0: %f %f\n",a0,F);

//        a1 = G/2.0 * a0;
        realDivide  (&GG,const_2,&TT,realContext);
        realMultiply(&TT, &aa0, &aa1, realContext);
        realToString(&aa1,ss); a1 = strtof (ss, NULL);
        printf("§ a1: %f %f\n",a1,G/2.0 * a0);

  //      a2 = H - F * a1 * a1;
        realMultiply(&aa1,&aa1,&SS,realContext);
        realMultiply(&SS,&FF,&SS,realContext);
        realSubtract(&HH,&SS,&aa2,realContext);
        realToString(&aa0,ss); a2 = strtof (ss, NULL);
        printf("§ a2: %f %f\n",a2, H - F * a1 * a1);



        #ifdef STATDEBUG
          printf("##### CAUCHY %i a0=%f a1=%f a2=%f\n",(int)nn, a0, a1, a2);
        #endif
        break;

      case CF_ORTHOGONAL_FITTING :
        a1a = 1.0 / (2.0 * sxy) * ( sy * sy - sx * sx + sqrt( (sy * sy - sx * sx) * (sy * sy - sx * sx) + 4 * sxy * sxy) );
        a0a  = y_ - a1a * x_;
        //r = sxy / (sx * sy);
        r  = (nn * sumxy - sumx * sumy) / (sqrt (nn * sumx2 - sumx * sumx) * sqrt(nn * sumy2 - sumy * sumy) );
        a1 = a1a;
  sprintf(ss,"%f",a1); stringToReal(ss,&aa1,realContext);
        a0 = a0a;
  sprintf(ss,"%f",a0); stringToReal(ss,&aa0,realContext);
        #ifdef STATDEBUG
          printf("##### ORTHOF %i a0=%f a1=%f smi^2=%f\n",(int)nn, a0a, a1a, smi2);
        #endif
        break;

      case CF_ORTHOGONAL_FITTING+10000 :
        a1b = 1.0 / (2.0 * sxy) * ( sy * sy - sx * sx - sqrt( (sy * sy - sx * sx) * (sy * sy - sx * sx) + 4 * sxy * sxy) );
        a0b  = y_ - a1b * x_;
        //r = sxy / (sx * sy);
        r  = (nn * sumxy - sumx * sumy) / (sqrt( nn * sumx2 - sumx * sumx) * sqrt( nn * sumy2 - sumy * sumy));
        a1 = a1b;
  sprintf(ss,"%f",a1); stringToReal(ss,&aa1,realContext);
        a0 = a0b;
  sprintf(ss,"%f",a0); stringToReal(ss,&aa0,realContext);
        #ifdef STATDEBUG
          printf("##### ORTHOF %i a0=%f a1=%f smi^2=%f\n",(int)nn, a0b, a1b, smi2);
        #endif
        break;

      default: break;
    }
    graph_axis_draw();                        //Draw the axis 
}


void fnStatDemo(uint16_t unusedButMandatoryParameter){
  #ifndef TESTSUITE_BUILD
  selection = 0;
  runFunction(ITM_CLSIGMA);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99574829932",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("11",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99579081633",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("6",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99583333333",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("16",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99587585034",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("11",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99591836735",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("8",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99596088435",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("10",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99600340136",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("14",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99604591837",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("11",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99608843537",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("9",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99613095238",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("11",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99617346939",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("13",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99621598639",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("12",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99625850340",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("11",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99630102041",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("10",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99634353741",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("10",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99638605442",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("11",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99642857143",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("14",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99647108844",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("6",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99651360544",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("5",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99655612245",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("11",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99659863946",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("14",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99664115646",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("9",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99668367347",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("6",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99672619048",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("13",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99676870748",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("12",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99681122449",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("11",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99685374150",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("8",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99689625850",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("3",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99693877551",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("9",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99698129252",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("12",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99702380952",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("17",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99706632653",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("9",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99710884354",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("12",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99715136054",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("18",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99719387755",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("26",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99723639456",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("38",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99727891156",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("55",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99732142857",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("80",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99736394558",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("105",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99740646259",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("197",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99744897959",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("217",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99749149660",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("302",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99753401361",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("361",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99757653061",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("417",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99761904762",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("485",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99766156463",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("502",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99770408163",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("446",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99774659864",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("415",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99778911565",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("333",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99783163265",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("247",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99787414966",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("209",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99791666667",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("158",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99795918367",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("100",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99800170068",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("62",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99804421769",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("36",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99808673469",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("38",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99812925170",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("14",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99817176871",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("11",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99821428571",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("11",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99825680272",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("10",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99829931973",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("13",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99834183673",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("15",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99838435374",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("12",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99842687075",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("8",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99846938776",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("9",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99851190476",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("8",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99855442177",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("10",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99859693878",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("7",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99863945578",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("12",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99868197279",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("6",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99872448980",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("11",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99876700680",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("6",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99880952381",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("13",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99885204082",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("15",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99889455782",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("16",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99893707483",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("13",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99897959184",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("11",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99902210884",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("8",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99906462585",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("9",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99910714286",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("12",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99914965986",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("3",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99919217687",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("8",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99923469388",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("12",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99927721088",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("4",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99931972789",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("5",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99936224490",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("11",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99940476190",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("12",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99944727891",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("7",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99948979592",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("11",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99953231293",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("9",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99957482993",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("12",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99961734694",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("14",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99965986395",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("12",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99970238095",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("10",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99974489796",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("10",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99978741497",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("6",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99982993197",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("9",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99987244898",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("7",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99991496599",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("13",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000.99995748299",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("10",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
  #endif //TESTSUITE_BUILD
  }


void fnStatDemo1(uint16_t unusedButMandatoryParameter){
#ifndef TESTSUITE_BUILD
  int8_t ix;
  runFunction(ITM_CLSIGMA);
  selection = 0;
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
#endif //TESTSUITE_BUILD
}


