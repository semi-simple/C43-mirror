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

real_t RR,SMI,aa0,aa1,aa2; //L.R. variables


//#define STATDEBUG

graphtype *gr_x;
graphtype *gr_y;
graphtype telltale;
uint16_t  ix_count;
  
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
uint8_t PLOT_ZOOM;

int8_t   plotmode;    //VECTOR or SCATTER
float    tick_int_x;
float    tick_int_y;
float    x_min; 
float    x_max;
float    y_min;
float    y_max;
uint32_t xzero;
uint32_t yzero;



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
  PLOT_ZOOM     = 0;

  plotmode      = _SCAT;
  tick_int_x    = 0;        //to show axis: tick_in_x & y = 10, PLOT_AXIS = true
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
  graphtype x,y;
  realToInt32(SIGMA_N, cnt);

  if(cnt <= LIM) {
    if(jm_VECT || jm_NVECT) {
      plotmode = _VECT;
    } else {
      plotmode = _SCAT;
    }

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
  else {
      printf("In function SUM+ or SUM-:%u There is insufficient plot memory available, but stats totals still accumulate!\n",plusminus);
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


void graphAxisDraw (void){
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
      #ifdef STATDEBUG
        printf("cnt=%d   \n",(int)cnt);
      #endif
      cnt++; 
    }

   force_refresh1();

   if(0<x_max && 0>x_min) {
     for(x=0; x<=x_max; x+=tick_int_x) {                         //draw x ticks
        #ifdef STATDEBUG
          printf(">> x=%d   \n",(int)x);
        #endif
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
        #ifdef STATDEBUG
          printf(">>>x=%d   \n",(int)x);
        #endif
        cnt = screen_window_x(x_min,x,x_max);
        //printf(">>>>>A %f %d ",x,cnt);
          setBlackPixel(cnt,min(yzero+1,SCREEN_HEIGHT_GRAPH-1)); //tick
          setBlackPixel(cnt,max(yzero-1,minny));                 //tick
       }
      for(x=x_min; x<=x_max; x+=tick_int_x*5) {                  //draw x ticks
        #ifdef STATDEBUG
          printf(">>>>x=%d   \n",(int)x);
        #endif
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
        #ifdef STATDEBUG
          printf(">>y=%d   \n",(int)y);
        #endif
        cnt = screen_window_y(y_min,y,y_max);
        setBlackPixel(max(xzero-1,0),cnt);                    //tick
        setBlackPixel(min(xzero+1,SCREEN_WIDTH_GRAPH-1),cnt); //tick
      }  
      for(y=0; y>=y_min; y+=-tick_int_y) {                    //draw y ticks
        #ifdef STATDEBUG
          printf(">>>y=%d   \n",(int)y);
        #endif
        cnt = screen_window_y(y_min,y,y_max);
        setBlackPixel(max(xzero-1,0),cnt);                    //tick
        setBlackPixel(min(xzero+1,SCREEN_WIDTH_GRAPH-1),cnt); //tick
      }  
      for(y=0; y<=y_max; y+=tick_int_y*5) {                   //draw y ticks
        #ifdef STATDEBUG
          printf(">>>>y=%d   \n",(int)y);
        #endif
        cnt = screen_window_y(y_min,y,y_max);
        setBlackPixel(max(xzero-2,0),cnt);                    //tick
        setBlackPixel(min(xzero+2,SCREEN_WIDTH_GRAPH-1),cnt); //tick
        setBlackPixel(max(xzero-3,0),cnt);                    //tick
        setBlackPixel(min(xzero+3,SCREEN_WIDTH_GRAPH-1),cnt); //tick
      }  
      for(y=0; y>=y_min; y+=-tick_int_y*5) {                  //draw y ticks
        #ifdef STATDEBUG
          printf(">>>>>y=%d   \n",(int)y);
        #endif
        cnt = screen_window_y(y_min,y,y_max);
        setBlackPixel(max(xzero-2,0),cnt);                    //tick
        setBlackPixel(min(xzero+2,SCREEN_WIDTH_GRAPH-1),cnt); //tick
        setBlackPixel(max(xzero-3,0),cnt);                    //tick
        setBlackPixel(min(xzero+3,SCREEN_WIDTH_GRAPH-1),cnt); //tick
      }  
    } else {
      for(y=y_min; y<=y_max; y+=tick_int_y) {                 //draw y ticks
        #ifdef STATDEBUG
          printf(">>>>>>>y=%d   \n",(int)y);
        #endif
        cnt = screen_window_y(y_min,y,y_max);
        setBlackPixel(max(xzero-1,0),cnt);                    //tick
        setBlackPixel(min(xzero+1,SCREEN_WIDTH_GRAPH-1),cnt); //tick
      }  
      for(y=y_min; y<=y_max; y+=tick_int_y*5) {               //draw y ticks
        #ifdef STATDEBUG
          printf(">>>>>>>>y=%d   \n",(int)y);
        #endif
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

void graphPlotstat(uint16_t selection){
  #ifdef PC_BUILD
    printf("#####>>> graphPlotstat: selection:%u:%s  lastplotmode:%u  lrSelection:%u lrChosen:%u\n",selection, getCurveFitModeName(selection), lastPlotMode, lrSelection, lrChosen);
  #endif //PC_BUILD
  #ifndef TESTSUITE_BUILD
  uint16_t  cnt, ix, statnum;
  uint16_t  xo, xn, xN; 
  uint8_t   yo, yn, yN;
  graphtype x; 
  graphtype y;

  statnum = 0;
  graphAxisDraw();                        //Draw the axis on any uncontrolled scale to start. Maybe optimize by remembering if there is an image on screen Otherwise double axis draw.
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
    
    x_min = x_min - dx * zoomfactor * (pow(4.0,(PLOT_ZOOM & 0x03)));
    y_min = y_min - dy * zoomfactor * (pow(4.0,(PLOT_ZOOM & 0x03)));
    x_max = x_max + dx * zoomfactor * (pow(4.0,(PLOT_ZOOM & 0x03)));
    y_max = y_max + dy * zoomfactor * (pow(4.0,(PLOT_ZOOM & 0x03)));
    #ifdef STATDEBUG
      printf("Axis3a: x: %f -> %f y: %f -> %f   \n",x_min, x_max, y_min, y_max);   
    #endif
  
    graphAxisDraw();
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



  char ss[50];
  double rr=0;
  double smi=0;

  if(selection != 0) {
    processCurvefitSelection(selection, &RR, &SMI, &aa0, &aa1, &aa2);
    realMultiply(&RR,&RR,&RR,&ctxtReal39);
    realToString(&RR,ss); rr = strtof (ss, NULL);
    realToString(&SMI,ss); smi = strtof (ss, NULL);
  }

  drawline(selection, rr, smi, &aa0, &aa1, &aa2);

  } else {
    displayCalcErrorMessage(ERROR_NO_SUMMATION_DATA, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "There is no statistical data available!");
      moreInfoOnError("In function graphPlotstat:", errorMessage, NULL, NULL);
    #endif
  }
#endif
}


#ifndef TESTSUITE_BUILD
  void drawline(uint16_t selection, double rr, double smi, real_t *aa0, real_t *aa1, real_t *aa2){
    if(!selection) return;
    #ifdef PC_BUILD
      printf("#####>>> drawline: selection:%u:%s  lastplotmode:%u  lrSelection:%u lrChosen:%u\n",selection, getCurveFitModeName(selection), lastPlotMode, lrSelection, lrChosen);
    #endif //PC_BUILD
    #ifndef USEFLOAT
      real_t SS,TT,UU;
    #endif //USEFLOAT
    double a0,a1,a2;
    int32_t nn;
    char ss[100];
    realToString(aa0, ss); a0 = strtof (ss, NULL);
    realToString(aa1, ss); a1 = strtof (ss, NULL);
    realToString(aa2, ss); a2 = strtof (ss, NULL);
    realToInt32(SIGMA_N, nn);  

    #ifdef PC_BUILD
      printf("plotting line: a2 %f a1 %f a0 %f\n",a2,a1,a0);
    #endif
    if((selection==0 && a2 == 0 && a1 == 0 && a0 == 0)) {
      #ifdef STATDEBUG
        printf("return\n");
      #endif
      return;
    }
    uint16_t  ix;
    uint16_t  xo = 0, xn, xN = 0; 
    uint8_t   yo = 0, yn, yN = 0;
    double    x = 0; 
    double    y = 0;
    int16_t   Intervals = numberIntervals * 10; //increase resulution in beginning and end of graph, to get a better starting and ending point in y

    for (ix = 0; ix <= Intervals; ++ix) {       //Variable accuracy line plot

      if(ix <= 1) Intervals = numberIntervals * 10; else
        if(ix >= Intervals-1) Intervals = numberIntervals * 10; else
          if(ix == 1) Intervals = numberIntervals;
      if(ix > 2 && ix < Intervals-2 && abs(yN-yo) > 10 && Intervals == numberIntervals){
        ix--;
        Intervals = numberIntervals * 15;       //increase accuracy and time to complete, if a jump in y is found
      } 
      x = x_min + (x_max-x_min)/(double)Intervals * (double)ix;

      switch(selection) {
        case CF_LINEAR_FITTING: 
        case CF_ORTHOGONAL_FITTING:
          #ifdef USEFLOAT 
            y = a1 * x + a0; 
          #else //USEFLOAT
            sprintf(ss,"%f",x); stringToReal(ss,&SS,&ctxtRealShort);
            realMultiply(&SS, &aa1, &UU, &ctxtRealShort);
            realAdd     (&UU, &aa0, &TT, &ctxtRealShort);
            realToString(&TT, ss); y = strtof (ss, NULL);
            //printf("XXX0 %f %f\n",y, a1 * x + a0);
          #endif //USEFLOAT
          break;
        case CF_EXPONENTIAL_FITTING: 
          #ifdef USEFLOAT 
            y = a0 * exp(a1 * x); 
          #else //USEFLOAT
            sprintf(ss,"%f",x); stringToReal(ss,&SS,&ctxtRealShort);
            realMultiply(&SS, &aa1, &UU, &ctxtRealShort);
            realExp     (&UU, &UU,       &ctxtRealShort);
            realMultiply(&UU, &aa0, &TT, &ctxtRealShort);
            realToString(&TT, ss); y = strtof (ss, NULL);
            //printf("XXX1 %f %f\n",y, a0 * exp(a1 * x));
          #endif //USEFLOAT
          break;
        case CF_LOGARITHMIC_FITTING: 
          #ifdef USEFLOAT 
            y = a0 + a1*log(x); 
          #else //USEFLOAT
            sprintf(ss,"%f",x); stringToReal(ss,&SS,&ctxtRealShort);
            WP34S_Ln    (&SS, &SS,       &ctxtRealShort);
            realMultiply(&SS, &aa1, &UU, &ctxtRealShort);
            realAdd     (&UU, &aa0, &TT, &ctxtRealShort);
            realToString(&TT, ss); y = strtof (ss, NULL);
          #endif //USEFLOAT
          break;
        case CF_POWER_FITTING: 
          #ifdef USEFLOAT 
            y = a0 * pow(x,a1);
          #else //USEFLOAT
            sprintf(ss,"%f",x); stringToReal(ss,&SS,&ctxtRealShort);
            realPower   (&SS, &aa1, &SS, &ctxtRealShort);
            realMultiply(&SS, &aa0, &TT, &ctxtRealShort);
            realToString(&TT, ss); y = strtof (ss, NULL);
          #endif //USEFLOAT
          break;
        case CF_ROOT_FITTING: 
          #ifdef USEFLOAT 
            y = a0 * pow(a1,1/x);
          #else //USEFLOAT
            sprintf(ss,"%f",x); stringToReal(ss,&SS,&ctxtRealShort);
            realDivide  (const_1, &SS, &SS, &ctxtRealShort);
            realPower   (&aa1, &SS, &SS, &ctxtRealShort);    //very very slow with a1=0.9982, probably in the 0.4 < x < 1.0 area
            realMultiply(&SS, &aa0, &SS, &ctxtRealShort);
            realToString(&SS, ss); y = strtof (ss, NULL);
          #endif //USEFLOAT
          break;
        case CF_HYPERBOLIC_FITTING: 
          #ifdef USEFLOAT 
            y = 1 / (a1 * x + a0);
          #else //USEFLOAT
            sprintf(ss,"%f",x); stringToReal(ss,&SS,&ctxtRealShort);
            realMultiply(&SS, &aa1, &UU, &ctxtRealShort);
            realAdd     (&UU, &aa0, &TT, &ctxtRealShort);
            realDivide  (const_1, &TT, &TT, &ctxtRealShort);
            realToString(&TT, ss); y = strtof (ss, NULL);
          #endif //USEFLOAT
          break;
        case CF_PARABOLIC_FITTING: 
          #ifdef USEFLOAT 
            y = a2 * x * x + a1 * x + a0;
          #else //USEFLOAT
            sprintf(ss,"%f",x); stringToReal(ss,&SS,&ctxtRealShort);
            realMultiply(&SS, &SS , &TT, &ctxtRealShort);
            realMultiply(&TT, &aa2, &TT, &ctxtRealShort);
            realMultiply(&SS, &aa1, &UU, &ctxtRealShort);
            realAdd     (&TT, &UU,  &TT, &ctxtRealShort);
            realAdd     (&TT, &aa0, &TT, &ctxtRealShort);          
            realToString(&TT, ss); y = strtof (ss, NULL);
          #endif //USEFLOAT
          break;
        case CF_GAUSS_FITTING:
          #ifdef USEFLOAT 
            y = a0 * exp( (x-a1)*(x-a1)/a2 );
          #else //USEFLOAT
            sprintf(ss,"%f",x); stringToReal(ss,&SS,&ctxtRealShort);
            realSubtract(&SS, &aa1, &TT, &ctxtRealShort);
            realMultiply(&TT, &TT , &TT, &ctxtRealShort);
            realDivide  (&TT, &aa2, &TT, &ctxtRealShort);
            realExp     (&TT, &TT ,      &ctxtRealShort);
            realMultiply(&TT, &aa0, &TT, &ctxtRealShort);
            realToString(&TT, ss); y = strtof (ss, NULL);
          #endif //USEFLOAT
          break;
        case CF_CAUCHY_FITTING:
          #ifdef USEFLOAT 
            y = 1/(a0*(x+a1)*(x+a1)+a2);
          #else //USEFLOAT
            sprintf(ss,"%f",x); stringToReal(ss,&SS,&ctxtRealShort);
            realAdd     (&SS, &aa1, &TT, &ctxtRealShort);
            realMultiply(&TT, &TT , &TT, &ctxtRealShort);
            realMultiply(&TT, &aa0, &TT, &ctxtRealShort);
            realAdd     (&TT, &aa2, &TT, &ctxtRealShort);
            realDivide  (const_1, &TT, &TT, &ctxtRealShort);
            realToString(&TT, ss); y = strtof (ss, NULL);
          #endif //USEFLOAT
        default:break;
      }
      xo = xN;
      yo = yN;
      xN = screen_window_x(x_min,x,x_max);
      yN = screen_window_y(y_min,y,y_max);

      if(ix > 0) {  //Allow for starting values to accumulate in the registers at ix = 0
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


    #ifdef PC_BUILD
      printf("#####>>> Labels selection:%u:%s  lastplotmode:%u  lrSelection:%u lrChosen:%u\n",selection, getCurveFitModeName(selection), lastPlotMode, lrSelection, lrChosen);
    #endif //PC_BUILD

    #define autoinc 19 //text line spacing
    #define autoshift -5 //text line spacing
    int16_t index = -1;
    if(selection!=0) {
      strcpy(ss,eatSpaces(getCurveFitModeName(selection)));
      if(lrCountOnes(lrSelection)>1 && selection == lrChosen) strcat(ss,lrChosen == 0 ? "" : STD_SUP_ASTERISK);
        showString(ss, &standardFont, 0, Y_POSITION_OF_REGISTER_Z_LINE + autoinc * index++ -10 +autoshift, vmNormal, false, false);
      strcpy(ss,"y=");
      strcat(ss,getCurveFitModeFormula(selection));
        showString(ss, &standardFont, 0, Y_POSITION_OF_REGISTER_Z_LINE + autoinc * index++ -7 +autoshift, vmNormal, false, false);
      sprintf(ss,"n=%d",(int)nn);
        showString(ss, &standardFont, 0, Y_POSITION_OF_REGISTER_Z_LINE + autoinc*index++ -2 +autoshift, vmNormal, false, false);
      eformat(ss,"a" STD_SUB_0 "=",a0,7);
        showString(ss, &standardFont, 0, Y_POSITION_OF_REGISTER_Z_LINE + autoinc*index++ -4 +autoshift, vmNormal, false, false);
      eformat(ss,"a" STD_SUB_1 "=",a1,7);
        showString(ss, &standardFont, 0, Y_POSITION_OF_REGISTER_Z_LINE + autoinc*index++ -3 +autoshift, vmNormal, false, false);

      if(selection == CF_PARABOLIC_FITTING || selection == CF_GAUSS_FITTING || selection == CF_CAUCHY_FITTING) { 
        eformat(ss,"a" STD_SUB_2 "=",a2,7);
        showString(ss, &standardFont, 0, Y_POSITION_OF_REGISTER_Z_LINE + autoinc*index++ -2 +autoshift, vmNormal, false, false);
      }
      eformat(ss,"r" STD_SUP_2 "=",rr,4);
        showString(ss, &standardFont, 0, Y_POSITION_OF_REGISTER_Z_LINE + autoinc*index++ +autoshift, vmNormal, false, false);

      if(selection == CF_ORTHOGONAL_FITTING) {
        eformat(ss,"s" STD_SUB_m STD_SUB_i "=",smi,4); 
        showString(ss, &standardFont, 0, Y_POSITION_OF_REGISTER_Z_LINE + autoinc*index++ -2 +autoshift, vmNormal, false, false);
      }
    }
  }
#endif //TESTSUITE_BUILD



void fnPlotClose(uint16_t unusedButMandatoryParameter){
  lastPlotMode = PLOT_NOTHING;
  selection = 0;
  calcMode = CM_NORMAL;
  fnKeyExit(0);
  fnUndo(0);
}


void fnPlotCloseSmi(uint16_t unusedButMandatoryParameter){
  lastPlotMode = PLOT_NOTHING;
  selection = 0;
  calcMode = CM_NORMAL;
  fnKeyExit(0);
  fnUndo(0);
  fnMinExpStdDev(0);
}


void fnPlotStat(uint16_t plotMode){
#ifdef STATDEBUG
  printf("fnPlotStat1: selection = %u; Plotmode=%u\n",selection,plotMode);
#endif //STATDEBUG
#ifdef PC_BUILD
  printf("#####>>> fnPlotStat1: selection:%u:%s  Plotmode:%u lastplotmode:%u  lrSelection:%u lrChosen:%u\n",selection, getCurveFitModeName(selection), plotMode, lastPlotMode, lrSelection, lrChosen);
#endif //PC_BUILD


#ifndef TESTSUITE_BUILD
  if (!(lastPlotMode == PLOT_NOTHING || lastPlotMode == PLOT_START)) plotMode = lastPlotMode;
  calcMode = CM_PLOT_STAT;
  statGraphReset(); 
  if(plotMode == PLOT_START) selection = 0;

  hourGlassIconEnabled = true;
  showHideHourGlass();

  #ifdef DMCP_BUILD
    lcd_refresh();
  #else // !DMCP_BUILD
    refreshLcd(NULL);
  #endif // DMCP_BUILD

  switch(plotMode) {
    case PLOT_FIT:
         #ifdef STATDEBUG
           printf("################# PLOT_FIT: Push PLOT_LR menu; selection = %u\n",selection);
         #endif //STATDEBUG
         if(softmenu[softmenuStack[0].softmenuId].menuItem != -MNU_PLOT_LR) showSoftmenu(-MNU_PLOT_LR);
         break;
    case PLOT_ORTHOF:
    case PLOT_CYCLEALL:
    case PLOT_START:
         #ifdef STATDEBUG
           printf("################# PLOT_START, PLOT_ORTHOF, PLOT_CYCLEALL): Push PLOT_STAT menu; selection = %u\n",selection);
         #endif //STATDEBUG
         if(softmenu[softmenuStack[0].softmenuId].menuItem != -MNU_PLOT_STAT) showSoftmenu(-MNU_PLOT_STAT);
         break;
    case PLOT_NOTHING:
         break;
    default: break;
  }

  if(plotMode != PLOT_START) fnPlotRegLine(plotMode);
  else lastPlotMode = plotMode;
#endif //TESTSUITE_BUILD
}


void fnPlotRegLine(uint16_t plotMode){
  #ifdef STATDEBUG
    printf("fnPlotRegLine: selection = %u\n",selection);
  #endif //STATDEBUG
  if(plotMode == PLOT_START) plotMode = PLOT_ORTHOF; /*PLOT_CYCLEALL*/
  switch(plotMode) {
    case PLOT_CYCLEALL:
      lastPlotMode = PLOT_CYCLEALL;
      //Cycling through all graphs, starting with ORTHOF, in this sequence
      if(selection == 0                           ) selection = CF_ORTHOGONAL_FITTING ;     else
      if(selection == CF_ORTHOGONAL_FITTING       ) selection = CF_LINEAR_FITTING     ;     else
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
      break;

    case PLOT_ORTHOF: 
      selection = CF_ORTHOGONAL_FITTING;
      break;

    case PLOT_FIT:
      //Show data and one curve fit selected: Scans lrSelection from LSB and stop after the first one is found. If a chosen curve is there, override.
      //printf("#####X %u %u \n",selection, lrSelection);
//      tmpSelect = lrChosen == 0 ? (lrSelection == 0 ? 1023 : lrSelection) : lrChosen;
      selection = selection << 1;
      if(selection == 0) selection = 1;
      while((selection != ( (lrSelection == 0 ? 1023 : lrSelection) & selection)) && selection < 1024){
        //printf("#####Z %u %u \n",selection, lrSelection);
        selection = (selection) << 1;
      }
      if(selection >= 1024) selection = 0;  //purposely change to zero graph display
      break;

    case PLOT_NOTHING:
    case PLOT_START:
    default:break;
  }
}


void fnPlotZoom(uint16_t unusedButMandatoryParameter){
   PLOT_ZOOM++;
   printf("### %u\n",(PLOT_ZOOM & 0x04));
   void refreshScreen(void);
  
}



//DEMO: Arbitrary distribution to test. Close to a Normal.
void fnStatDemo0(uint16_t unusedButMandatoryParameter){
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


//DEMO: Randomized linear
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

//DEMO: 4 points to simulate a distribution, from p105 of OM
void fnStatDemo105(uint16_t unusedButMandatoryParameter){
  #ifndef TESTSUITE_BUILD
  selection = 0;
  runFunction(ITM_CLSIGMA);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("2",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("30",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("2.5",REGISTER_REAL34_DATA(REGISTER_X));stringToReal34("50",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("3.5",REGISTER_REAL34_DATA(REGISTER_X));stringToReal34("90",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("4",REGISTER_REAL34_DATA(REGISTER_X));stringToReal34("130",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("4.5",REGISTER_REAL34_DATA(REGISTER_X));stringToReal34("150",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
  #endif //TESTSUITE_BUILD
  }

//DEMO: points to simulate a distribution, from p107 of OM
void fnStatDemo107(uint16_t unusedButMandatoryParameter){
  #ifndef TESTSUITE_BUILD
  selection = 0;
  runFunction(ITM_CLSIGMA);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1945",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("696",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1955",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("1330",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1965",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("1750",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1971",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("2243",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1973",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("2484",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1950",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("994",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1960",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("1512",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1970",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("2162",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1972",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("2382",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
  #endif //TESTSUITE_BUILD
  }

//DEMO:  points to simulate a distribution, from p109 of OM
void fnStatDemo109(uint16_t unusedButMandatoryParameter){
  #ifndef TESTSUITE_BUILD
  selection = 0;
  runFunction(ITM_CLSIGMA);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("4.63",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("20",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("5.78",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("40",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("6.61",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("60",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("7.21",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("80",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("7.78",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
  #endif //TESTSUITE_BUILD
  }



void fnStatDemo2(uint16_t unusedButMandatoryParameter){
  #ifndef TESTSUITE_BUILD
  selection = 0;
  runFunction(ITM_CLSIGMA);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-0.1",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.0905",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("1.0000",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("+0.1",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.0905",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0.01",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.8",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
  #endif //TESTSUITE_BUILD
  }

