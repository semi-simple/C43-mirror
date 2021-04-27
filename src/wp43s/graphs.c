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

#include "graphs.h"

#include "charString.h"
#include "constantPointers.h"
#include "error.h"
#include "graphText.h"
#include "items.h"
#include "math.h"
#include "plotstat.h"
#include "radioButtonCatalog.h"
#include "registers.h"
#include "screen.h"
#include "softmenus.h"
#include "stats.h"
#include "statusBar.h"
#include <string.h>

#include "wp43s.h"

//#define STATDEBUG

bool_t    invalid_intg = true;
bool_t    invalid_diff = true;
bool_t    invalid_rms  = true;


void graph_reset(void){
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
  PLOT_SHADE    = false;
  PLOT_AXIS     = true;
  PLOT_ZMX      = 0;
  PLOT_ZMY      = 0;
  plotmode      = _SCAT;
}


void fnClGrf(uint16_t unusedButMandatoryParameter) {
  graph_reset();
  fnClSigma(0);
  fnRefreshState();                //jm
}


void fnPline(uint16_t unusedButMandatoryParameter) {
  PLOT_LINE = !PLOT_LINE;
  fnRefreshState();                //jm
  fnPlot(0);
}


void fnPcros(uint16_t unusedButMandatoryParameter) {
  PLOT_CROSS = !PLOT_CROSS;
  if(PLOT_CROSS) PLOT_BOX = false;
  fnRefreshState();                //jm
  fnPlot(0);
}


void fnPbox (uint16_t unusedButMandatoryParameter) {
  PLOT_BOX = !PLOT_BOX;
  if(PLOT_BOX) PLOT_CROSS = false;
  fnRefreshState();                //jm
  fnPlot(0);
}

void fnPintg (uint16_t unusedButMandatoryParameter) {
  PLOT_INTG = !PLOT_INTG;
  if(!PLOT_INTG) PLOT_SHADE = false;
  fnRefreshState();                //jm
  fnPlot(0);
}

void fnPdiff (uint16_t unusedButMandatoryParameter) {
  PLOT_DIFF = !PLOT_DIFF;
  fnRefreshState();                //jm
  fnPlot(0);
}

void fnPrms (uint16_t unusedButMandatoryParameter) {
  PLOT_RMS = !PLOT_RMS;
  fnRefreshState();                //jm
  fnPlot(0);
}

void fnPzoom (uint16_t param) {
  if(param == 1) {
    if(PLOT_ZMX < 0+3) PLOT_ZMX++; else PLOT_ZMX = 0-1;
    fnRefreshState();                //jm
    fnPlot(0);
  } else
  if(param == 2) {
    if(PLOT_ZMY < 0+3) PLOT_ZMY++; else PLOT_ZMY = 0-1;
    fnRefreshState();
    fnPlot(0);
  }
}

void fnPvect (uint16_t unusedButMandatoryParameter) {
  jm_VECT = !jm_VECT;
  if(jm_VECT) {jm_NVECT = false;}
  fnRefreshState();                //jm
  fnPlot(0);
}    

void fnPNvect (uint16_t unusedButMandatoryParameter) {
  jm_NVECT = !jm_NVECT;
  if(jm_NVECT) {jm_VECT = false;}
  fnRefreshState();                //jm
  fnPlot(0);
}    

void fnScale (uint16_t unusedButMandatoryParameter) {
  jm_SCALE = !jm_SCALE;
  fnRefreshState();                //jm
  fnPlot(0);
}    

void fnPshade (uint16_t unusedButMandatoryParameter) {
  PLOT_SHADE = !PLOT_SHADE;
  if(PLOT_SHADE) PLOT_INTG = true;
  fnRefreshState();                //jm
  fnPlot(0);
}    

void fnPx (uint16_t unusedButMandatoryParameter) {
  extentx = !extentx;
  fnRefreshState();                //jm
  fnPlot(0);
}

void fnPy (uint16_t unusedButMandatoryParameter) {
  extenty = !extenty;
  fnRefreshState();                //jm
  fnPlot(0);
}


void fnPlotReset(uint16_t unusedButMandatoryParameter) {
  graph_dx      = 0;
  graph_dy      = 0;
  extentx       = true;
  extenty       = true;
  jm_VECT       = false;
  jm_NVECT      = false;
  jm_SCALE      = false;
  //Aspect_Square = true;
  PLOT_LINE     = true;
  PLOT_CROSS    = true;
  PLOT_BOX      = false;
  PLOT_INTG     = false;
  PLOT_DIFF     = false;
  PLOT_RMS      = false;
  PLOT_SHADE    = false;
  PLOT_AXIS     = true;
  PLOT_ZMX      = 0;
  PLOT_ZMY      = 0;
  PLOT_ZOOM     = 0;

  plotmode      = _SCAT;      //VECTOR or SCATTER
  tick_int_x    = 0;          //to show axis: tick_in_x & y = 10, PLOT_AXIS = true
  tick_int_y    = 0;

  fnPlot(0);

}


void fnPlot(uint16_t unusedButMandatoryParameter) {
  #ifdef DMCP_BUILD
    lcd_refresh();
  #else // !DMCP_BUILD
    refreshLcd(NULL);
  #endif // DMCP_BUILD
  PLOT_AXIS = true;
  hourGlassIconEnabled = true;
  showHideHourGlass();
  Aspect_Square = true;
  if(calcMode != CM_GRAPH && calcMode != CM_PLOT_STAT){previousCalcMode = calcMode;}
  if(previousCalcMode == CM_GRAPH || previousCalcMode == CM_PLOT_STAT) {
    previousCalcMode = CM_NORMAL;
  }
  calcMode = CM_GRAPH;
  #ifndef TESTSUITE_BUILD
    if(softmenu[softmenuStack[0].softmenuId].menuItem != -MNU_PLOT) {
      showSoftmenu(-MNU_PLOT);                         //JM MENU Prevent resetting the softmenu to the default no 1 page position
    }
  #endif
  doRefreshSoftMenu = true;             //Plot graph is part of refreshScreen
}

void fnPlotLS(uint16_t unusedButMandatoryParameter) {
  PLOT_AXIS = true;
  Aspect_Square = false;
  if(calcMode != CM_GRAPH && calcMode != CM_PLOT_STAT){previousCalcMode = calcMode;}
  if(previousCalcMode == CM_GRAPH || previousCalcMode == CM_PLOT_STAT) {
    previousCalcMode = CM_NORMAL;
  }
  calcMode = CM_GRAPH;
  doRefreshSoftMenu = true;             //Plot graph is part of refreshScreen
}

void fnListXY(uint16_t unusedButMandatoryParameter) {
  #ifndef TESTSUITE_BUILD
    if(telltale == MEM_INITIALIZED) {
      calcMode = CM_LISTXY; //Used to view graph/listing
      ListXYposition = 0;
    }
  #endif
}







#ifndef TESTSUITE_BUILD
      void plotarrow(uint16_t xo, uint8_t yo, uint16_t xn, uint8_t yn) {              // Plots line from xo,yo to xn,yn; uses temporary x1,y1
        float dx, dy, ddx, dydx, zz, zzz;
        dydx = yn-yo;
        ddx = xn-xo;
        zz  = sqrt(dydx*dydx+ddx*ddx);
        zzz = 3;
        dy  = dydx * (zzz/zz);
        dx  = ddx * (zzz/zz);
        #ifdef STATDEBUG
        printf("%d %d  %d %d  ddx=%f, dydx=%f, zz=%f  zzz=%f, dx=%f, dy=%f \n",xo, yo, xn, yn, ddx,dydx,zz,zzz,dx,dy);
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

      void plotrms(uint16_t xn, uint8_t yn) {                // Plots line from xo,yo to xn,yn; uses temporary x1,y1
        placePixel(xn+1,yn-1);                               //   PLOT a box
        placePixel(xn-1,yn-1);
        placePixel(xn-0,yn-1);
        placePixel(xn+1,yn);                               //   PLOT a box
        placePixel(xn-1,yn);
        placePixel(xn-0,yn);
        placePixel(xn+1,yn+1);                               //   PLOT a box
        placePixel(xn-1,yn+1);
        placePixel(xn-0,yn+1);

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


    
  void convertDigits(char * refstr, uint16_t ii, uint16_t * oo, char * outstr) {
    switch (refstr[ii]) {
      case  48: 
      case  49: 
      case  50: 
      case  51: 
      case  52: 
      case  53: 
      case  54: 
      case  55: 
      case  56: 
      case  57: outstr[(*oo)++] = 0xa0; outstr[(*oo)++] = refstr[ii] + (0x80 - 48); break; //.
      case 120: outstr[(*oo)++] = 0xa4; outstr[(*oo)++] = 0xb3; break; //x
      case 121: outstr[(*oo)++] = 0xa4; outstr[(*oo)++] = 0xb4; break; //y
      case 'a': outstr[(*oo)++] = 0xa4; outstr[(*oo)++] = 0x9c; break; //a
      case 's': outstr[(*oo)++] = 0xa4; outstr[(*oo)++] = 0xae; break; //s
      case  58: outstr[(*oo)++] = 0xa2; outstr[(*oo)++] = 0x36; break; //:
      case  43: outstr[(*oo)++] = 0xa0; outstr[(*oo)++] = 0x8a; break; //+
      case  45: outstr[(*oo)++] = 0xa0; outstr[(*oo)++] = 0x8b; break; //-
      case  46: outstr[(*oo)++] = 0xa0; outstr[(*oo)++] = 0x1a; break; //.
      case  47: outstr[(*oo)++] = 0xa4; outstr[(*oo)++] = 0x25; break; ///
      case 116: outstr[(*oo)++] = 0xa0; outstr[(*oo)++] = 0x9c; break; //t
      case 105: outstr[(*oo)++] = 0xa4; outstr[(*oo)++] = 0xa4; break; //i
      case 99 : outstr[(*oo)++] = 0xa4; outstr[(*oo)++] = 0x9e; break; //c
      case 107: outstr[(*oo)++] = 0xa4; outstr[(*oo)++] = 0xa6; break; //k
      default : outstr[(*oo)++] = refstr[ii];
    }
  }

void graph_text(void){
  #ifndef TESTSUITE_BUILD
  uint32_t ypos = Y_POSITION_OF_REGISTER_T_LINE -11 + 12 * 5;
  uint16_t ii;
  static uint16_t oo;
  static char outstr[300];

  snprintf(tmpString, TMP_STR_LENGTH, "y %.3f/tick  ",tick_int_y);
  ii = 0;
  oo = 0;
  outstr[0]=0;
  while (tmpString[ii] != 0) {
    convertDigits(tmpString, ii,&oo,outstr);
    ii++;
  }
  outstr[oo]=0;
  showString(outstr, &standardFont, 1, ypos, vmNormal, true, true);  //JM
  ypos -= 12;


  snprintf(tmpString, TMP_STR_LENGTH, "x %.3f/tick  ", tick_int_x);
  ii = 0;
  oo = 0;
  outstr[0]=0;
  while (tmpString[ii] != 0) {
    convertDigits(tmpString, ii,&oo,outstr);
    ii++;
  }
  outstr[oo]=0;
  showString(outstr, &standardFont, 1, ypos, vmNormal, true, true);  //JM
  ypos -= 12;

  uint32_t minnx, minny;
  if (!Aspect_Square) {
    minny = SCREEN_NONSQ_HMIN;
    minnx = 0;
  }
  else {
    minny = 0;
    minnx = SCREEN_WIDTH-SCREEN_HEIGHT_GRAPH;
  }
  tmpString[0]=0;                                  //If the axis is on the edge supress it, and label accordingly
  uint8_t axisdisp = 
           (!( yzero == SCREEN_HEIGHT_GRAPH-1  || yzero == minny) ? 2:0) + 
           (!(  xzero == SCREEN_WIDTH-1        || xzero == minnx) ? 1:0) ;
  switch( axisdisp )  {
    case 0: strcpy(tmpString,"          "); break;
    case 1: snprintf(tmpString, TMP_STR_LENGTH, "y-axis x 0"); break;
    case 2: snprintf(tmpString, TMP_STR_LENGTH, "x-axis y 0"); break;
    case 3: snprintf(tmpString, TMP_STR_LENGTH, "axis 0.0 "); break;
    default:break;
  }


  //Change to the small characters and fabricate a small = char
    ii = 0;
    oo = 0;
    outstr[0]=0;
    while (tmpString[ii] != 0) {
      convertDigits(tmpString, ii,&oo,outstr);
      ii++;
    }
  outstr[oo]=0;
  ii=showString(outstr, &standardFont, 1, ypos, vmNormal, true, true);  //JM
  if(tmpString[ stringByteLength(tmpString)-1 ] == '0') {
    #define sp 15
    plotline((uint16_t)(ii-17),(uint8_t)(ypos+2+sp),(uint16_t)(ii-11),(uint8_t)(ypos+2+sp));
    plotline((uint16_t)(ii-17),(uint8_t)(ypos+1+sp),(uint16_t)(ii-11),(uint8_t)(ypos+1+sp));
    plotline((uint16_t)(ii-17),(uint8_t)(ypos-1+sp),(uint16_t)(ii-11),(uint8_t)(ypos-1+sp));
    plotline((uint16_t)(ii-17),(uint8_t)(ypos-2+sp),(uint16_t)(ii-11),(uint8_t)(ypos-2+sp));
  }
  ypos += 48;
  


  if(PLOT_INTG && !invalid_intg) {
    snprintf(tmpString, TMP_STR_LENGTH, "  Trapezoid integral");
    showStringC43(tmpString, numSmall, nocompress, 1, ypos, vmNormal, true, true);  //JM
    plotintbig( 5, ypos+4+4-2 );
    plotrect(5+4-1, (ypos+4+4-2+2)-1, 5+4+2, (ypos+4+4-2+2)+2);
    ypos += 20;
  }

  if(PLOT_DIFF && !invalid_diff) {
    snprintf(tmpString, TMP_STR_LENGTH, "  Num. differential");
    showStringC43(tmpString, numSmall, nocompress, 1, ypos, vmNormal, true, true);  //JM
    plotdeltabig( 6, ypos+4+4-2);
    ypos += 20;
  }

  if(PLOT_RMS && !invalid_rms) {
    snprintf(tmpString, TMP_STR_LENGTH, "  RMSy");
    showStringC43(tmpString, numSmall, nocompress, 1, ypos, vmNormal, true, true);  //JM
    plotrms( 6, ypos+4+4-2);
    plotrect(6-1, (ypos+4+4-2)-1, 6+2, (ypos+4+4-2)+2);
    ypos += 20;
  }


  force_refresh();


  #endif
}





//####################################################
//######### PLOT MEM #################################
//####################################################

void graph_plotmem(void) {

#if defined STATDEBUG && defined PC_BUILD
  uint16_t i;
  int16_t cnt1;
  realToInt32(SIGMA_N, cnt1);
  printf("Stored values\n");
  for (i = 0; i < LIM && i < cnt1; ++i) { 
    printf("i = %3u x = %9f; y = %9f\n",i,gr_x[i],gr_y[i]);
  }
#endif //STATDEBUG


  #ifndef TESTSUITE_BUILD

  uint16_t cnt, ix, statnum;
  uint16_t xo, xn, xN; 
  uint8_t yo, yn, yN;
  graphtype x; 
  graphtype y;
  graphtype sx, sy;
  graphtype ddx = FLoatingMax;
  graphtype dydx = FLoatingMax;
  graphtype inty = 0;
  graphtype inty0 = 0;
  graphtype inty_off = 0;
  graphtype rmsy = 0;

//printf("TEST %d %d\n",screen_window_x(-0.405573,0.45,0.689633), screen_window_y(-0.405573,0.45,0.689633));
//printf("TEST %d %d\n",screen_window_x(0,1,1), screen_window_y(0,1,1));




  statnum = 0;

  graph_axis();                        //Draw the axis on any uncontrolled scale to start. Maybe optimize by remembering if there is an image on screen Otherwise double axis draw.
  if(PLOT_AXIS) graph_text();

  if(jm_VECT || jm_NVECT) {plotmode = _VECT;} else {plotmode = _SCAT;}

  if(telltale == MEM_INITIALIZED && checkMinimumDataPoints(const_2)) {

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
    runFunction(ITM_XRMS);                                       //Determine the RMS of the y for an arbitrary integral offset
    runFunction(ITM_DROP);
    real34ToString(REGISTER_REAL34_DATA(REGISTER_X), tmpString);
    inty_off = strtof (tmpString, NULL);
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
      invalid_diff = false;                                                      //Differential dydx scale
      invalid_rms  = false;                                                      //RMSy

//#################################################### vvv SCALING LOOP DIFF INTG RMS vvv #########################
/**/      if(PLOT_DIFF || PLOT_INTG || PLOT_RMS) {
/**/        inty = inty_off;                                                          //  integral starting constant co-incides with graph
/**/
/**/        for (ix = 0; (ix < LIM && ix < statnum); ++ix) { 
/**/          if(ix !=0){
/**/            ddx = grf_x(ix) - grf_x(ix-1);                                            //used in DIFF and INT
/**/            if(ddx<=0) {                                                              //Cannot get slop or area if x is not growing in positive dierection
/**/      		    x_min = FLoatingMax;
/**/      		    x_max = FLoatingMin;
/**/      		    y_min = FLoatingMax;
/**/      		    y_max = FLoatingMin;
/**/      		    invalid_diff = true;
/**/                invalid_intg = true;
/**/                invalid_rms  = true;
/**/      		    break;
/**/            } else {          	
/**/              if(grf_x(ix) < x_min) {x_min = grf_x(ix);}
/**/              if(grf_x(ix) > x_max) {x_max = grf_x(ix);}
/**/              if(PLOT_DIFF) {
/**/                //plotDiff(); //dydx                                            //Differential
                    if(ddx != 0) {
                      if(ix == 1)                                 // only two samples available
                        dydx = (grf_y(ix) - grf_y(ix-1)) / ddx;   // Differential
                      else if(ix >= 2)                            // ix >= 2 three samples available 0 1 2
                        dydx = ( grf_y(ix-2) - 4.0 * grf_y(ix-1) + 3.0 * grf_y(ix) ) / 2.0 / ddx; //ChE 205 — Formulas for Numerical Differentiation, formule 32
                    } 
                    else dydx = FLoatingMax;

/**/                if(dydx < y_min) {y_min = dydx;}
/**/                if(dydx > y_max) {y_max = dydx;}
/**/              }
/**/              if(PLOT_INTG) {
/**/                //plotInt();   //inty                                          //integral
/**/                inty = inty + (grf_y(ix) + grf_y(ix-1)) / 2 * ddx;
                    if(inty < y_min) {y_min = inty;}
/**/                if(inty > y_max) {y_max = inty;}
/**/              }
/**/              if(PLOT_RMS) {
/**/                //plotRms();   //inty                                          //integral
                    rmsy = sqrt (
                    ( (grf_x(ix-1)-grf_x(0)) * rmsy * rmsy +
                      ( ddx * (grf_y(ix) + grf_y(ix-1) ) / 2   *   (grf_y(ix) + grf_y(ix-1) ) / 2) 
                        )  / (grf_x(ix)-grf_x(0)));

/**/                if(rmsy < y_min) {y_min = rmsy;}
/**/                if(rmsy > y_max) {y_max = rmsy;}
/**/              }
/**/
/**/            }
/**/          } 
/**/        }
/**/      }
//#################################################### ^^^ SCALING LOOP ^^^ #########################


      #ifdef STATDEBUG
      printf("Axis0b1: x: %f -> %f y: %f -> %f  %d \n",x_min, x_max, y_min, y_max, invalid_diff);
      #endif

//#################################################### vvv SCALING LOOP  vvv #########################
/**/       if(PLOT_BOX || PLOT_LINE || PLOT_CROSS || !(PLOT_DIFF || PLOT_INTG)) {  //XXXX
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
/**/      }
/**/    } 
/**/
/**/    else {                 //VECTOR
/**/      sx =0;
/**/      sy =0;
/**/      for(cnt=0; (cnt < LIM && cnt < statnum); cnt++) {            //### Note XXX E- will stuff up statnum!
/**/        sx = sx + grf_x(cnt);
/**/        sy = sy + grf_y(cnt);
/**/        if(sx < x_min) {x_min = sx;}
/**/        if(sx > x_max) {x_max = sx;}
/**/        if(sy < y_min) {y_min = sy;}
/**/        if(sy > y_max) {y_max = sy;}
/**/      }      
/**/    }
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
   
    x_min = x_min - dx * 0.015*3;
    y_min = y_min - dy * 0.015*3;
    x_max = x_max + dx * 0.015*3;
    y_max = y_max + dy * 0.015*3;


    #ifdef STATDEBUG
    printf("Axis3a: x: %f -> %f y: %f -> %f   \n",x_min, x_max, y_min, y_max);   
    #endif

    graph_axis();
    if(PLOT_AXIS) graph_text();

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
    rmsy = 0;

//#################################################### vvv MAIN GRAPH LOOP vvv #########################
    for (ix = 0; (ix < LIM && ix < statnum); ++ix) {
      if(plotmode != _VECT) {

        x = 0;
        y = 0;

        if(ix !=0 && ( (PLOT_DIFF && !invalid_diff) || (PLOT_INTG && !invalid_intg) || (PLOT_RMS && !invalid_rms) )) {                                                               //Differential dydx
          ddx = grf_x(ix) - grf_x(ix-1);
          //plotDiff();   //dydx                                          //Differential
          if(ddx != 0) {
            if(ix == 1)                                 // only two samples available
              dydx = (grf_y(ix) - grf_y(ix-1)) / ddx;   // Differential
            else if(ix >= 2)                            // ix >= 2 three samples available 0 1 2
              dydx = ( grf_y(ix-2) - 4.0 * grf_y(ix-1) + 3.0 * grf_y(ix) ) / 2.0 / ddx; //ChE 205 — Formulas for Numerical Differentiation, formule 32
          } 
          else dydx = FLoatingMax;

          inty0 = inty;
          //plotInt();    //inty                                         //integral
          inty = inty + (grf_y(ix) + grf_y(ix-1)) / 2 * ddx;

          //plotRms();    //RMSy
          rmsy = sqrt (
            ( 
              (grf_x(ix-1)-grf_x(0)) * rmsy * rmsy +
           (  ddx * (grf_y(ix) + grf_y(ix-1) ) / 2   *   (grf_y(ix) + grf_y(ix-1) ) / 2) 
                                                                                       )  / (grf_x(ix)-grf_x(0)));

          x = (grf_x(ix) + grf_x(ix-1))/2;
          if(PLOT_DIFF) y = dydx;                 //y is the default graph
          if(PLOT_RMS)  y = rmsy;                 //y is the default graph
          if(PLOT_INTG) y = inty;                 //y is the default graph
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
      printf("plotting graph table[%d] = x:%f y:%f dydx:%f inty:%f xN:%d yN:%d ",ix,x,y,dydx,inty,  xN,yN);
      printf(" ... x-ddx/2=%d dydx=%d inty=%d\n",screen_window_x( x_min, x-ddx/2, x_max), screen_window_y( y_min, dydx, y_max), screen_window_y( y_min, inty, y_max) );
      #endif

      int16_t minN_y,minN_x;
      if (!Aspect_Square) {minN_y = SCREEN_NONSQ_HMIN; minN_x = 0;}
      else {minN_y = 0; minN_x = SCREEN_WIDTH-SCREEN_HEIGHT_GRAPH;}
      if(xN<SCREEN_WIDTH_GRAPH && xN>minN_x && yN<SCREEN_HEIGHT_GRAPH && yN>minN_y) {
  //      yo = yn;                              //old , new, to be able to draw a line between samples
        yn = yN;
  //      xo = xn;
        xn = xN;

        #ifdef STATDEBUG
          printf("invalid_diff=%d invalid_intg=%d invalid_rms=%d \n",invalid_diff,invalid_intg,invalid_rms);
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
              printf("Plotting Delta x=%f dy=%f \n",x-ddx/2,dydx);
            #endif
            plotdelta( screen_window_x( x_min, x-ddx/2, x_max), screen_window_y( y_min, dydx, y_max) );
          }


          if(PLOT_RMS && !invalid_rms && ix != 0) {
            #ifdef STATDEBUG
              printf("Plotting RMSy x=%f rmsy=%f \n",x-ddx/2,rmsy);
            #endif
            plotrms( screen_window_x( x_min, x-ddx/2, x_max), screen_window_y( y_min, rmsy, y_max) );
          }



          if(PLOT_INTG && !invalid_intg && ix !=0) {
            #ifdef STATDEBUG
              printf("Plotting Integral x=%f intg(x)=%f\n",x-ddx/2,inty);
            #endif
            uint16_t xN0   =screen_window_x( x_min, grf_x(ix-1), x_max);
//          uint16_t xN1   =screen_window_x( x_min, grf_x(ix), x_max);
            uint16_t yNoff =screen_window_y( y_min, inty_off, y_max);
            uint16_t yN0   =screen_window_y( y_min, inty0, y_max);
            uint16_t yNintg=screen_window_y( y_min, inty, y_max);
            uint16_t xAvg  =((xN0+xN) >> 1);
            
            if(abs((int16_t)(xN-xN0)>=6)) {plotint( xAvg, yNintg );} else
                                          {//placePixel( xAvg, yNintg );
                                           plotrect(xAvg-1, yNintg-1, xAvg+1, yNintg+1);}

            if(abs((int16_t)(xN-xN0)>=6)) {plotline(xN, yNintg, xAvg+2, yNintg);plotline(xAvg-2, yNintg, xN0, yNintg);} else
            if(abs((int16_t)(xN-xN0)>=4)) {plotline(xN, yNintg, xAvg+2, yNintg);plotline(xAvg-2, yNintg, xN0, yNintg);}

            if(PLOT_SHADE) {
              if(abs((int16_t)(xN-xN0)>=6)) {
                plotrect(xN0,yNoff,xN,yN0);
                plotrect(xN0,yN0,xN,yNintg);
                plotline(xN0,yN0,xN,yNintg);
              } else {
                plotrect(xN0,yNoff,xN,yNintg);
                plotrect(xN0,yN0,xN,yNintg);
              }
            }


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
          if(!(xN>minN_x              )) printf("xN>minN_x; ");
          if(!(yN<SCREEN_HEIGHT_GRAPH)) printf("yN<SCREEN_HEIGHT_GRAPH");
          if(!(yN>1+minN_y            )) printf("yN>1+minN_y; ");
                    printf("Not plotted: xN=%d<SCREEN_WIDTH_GRAPH=%d && xN=%d>minN_x=%d && yN=%d<SCREEN_HEIGHT_GRAPH=%d && yN=%d>1+minN_y=%d\n",xN,SCREEN_WIDTH_GRAPH,xN,minN_x,yN,SCREEN_HEIGHT_GRAPH,yN,1+minN_y);
        #endif
      }
    }
//#################################################### ^^^ MAIN GRAPH LOOP ^^^ #########################


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
      sprintf(tmpString, "Stat data: N = %d",statnum);
    } else {
      statnum = ix_count;
      sprintf(tmpString, "Stat data: Vector IndexCount = %d",statnum);
    }

    runFunction(ITM_DROP);
    print_linestr(tmpString,true);
    #ifdef STATDEBUG
      printf("Stat data %d - %d (%s)\n",statnum-1, max(0, statnum-1-6), tmpString );
    #endif

    if( statnum - 0 - 1 + ListXYposition > statnum-1) ListXYposition = 0; else
    if( statnum - (min(10,min(LIM, statnum))-1) - 1 + ListXYposition < 0) ListXYposition = - (statnum - (min(10,min(LIM, statnum))-1) - 1);

    for (ix = 0; (ix < min(10,min(LIM, statnum))); ++ix) {
      ixx = statnum - ix - 1 + ListXYposition;

      if(((fabs(grf_x(ixx)) > 0.000999 || grf_x(ixx) == 0) && fabs(grf_x(ixx)) < 1000000)) 
        sprintf(tmpstr1,"[%3d] x%19.7f, ",ixx+1, grf_x(ixx));
      else
        sprintf(tmpstr1,"[%3d] x%19.7e, ",ixx+1, grf_x(ixx)); //round(grf_x(ixx)*1e10)/1e10);

      if(((fabs(grf_y(ixx)) > 0.000999 || grf_y(ixx) == 0) && fabs(grf_y(ixx)) < 1000000))
        sprintf(tmpstr2,"y%19.7f", grf_y(ixx));
      else
        sprintf(tmpstr2,"y%19.7e", grf_y(ixx)); //round(grf_y(ixx)*1e10)/1e10);

      strcat(tmpstr1,tmpstr2);

      print_numberstr(tmpstr1,false);
      #ifdef STATDEBUG
        printf("%d:%s\n",ixx,tmpstr1);
      #endif
    }
  }
  #endif
}





