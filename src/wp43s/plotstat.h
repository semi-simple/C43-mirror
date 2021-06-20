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

/**
 * \file plot.h
 ***********************************************/
#ifndef PLOTSTAT_H
#define PLOTSTAT_H

#include "realType.h"
#include "typeDefinitions.h"
#include <stdint.h>

#define   LIM              400 //100     //Number of points; MUST be multiple of 4
#define   zoomfactor     0.05f     // default is 0.05, which is 5% space around the data points. Use 0.05 * 40 for wide view
#define   numberIntervals   50     // default 50, auto increase if jumps found
#define   fittedcurveboxes   0     // default 0 = smooth line
#define   USEFLOATING useFLOAT     // useFLOAT is faster than 
                                   // useREAL4 for graph reproduction
                                   // Note: if useREAL4, then see plotstat.c TODO create REAL from x (double) if REALS will be used
#define   FLoatingMax    1e38f     //convenient round figures used for maxima and minima determination
#define   FLoatingMin    -1e38f


//Memory structure
extern    float *gr_x;
extern    float *gr_y;
extern    float telltale;
extern    uint16_t  ix_count;
#define   MEM_INITIALIZED 199


//Graph options
extern  float    graph_dx;
extern  float    graph_dy;
extern  bool_t   roundedTicks;
extern  bool_t   extentx;
extern  bool_t   extenty;
extern  bool_t   PLOT_VECT;
extern  bool_t   PLOT_NVECT;
extern  bool_t   PLOT_SCALE;
extern  bool_t   Aspect_Square;
extern  bool_t   PLOT_LINE;
extern  bool_t   PLOT_CROSS;
extern  bool_t   PLOT_BOX;
extern  bool_t   PLOT_INTG; 
extern  bool_t   PLOT_DIFF; 
extern  bool_t   PLOT_RMS; 
extern  bool_t   PLOT_SHADE; 
extern  bool_t   PLOT_AXIS;
extern  int8_t   PLOT_ZMX;
extern  int8_t   PLOT_ZMY;
extern  uint8_t  PLOT_ZOOM;

#define _VECT 0
#define _SCAT 1

extern  int8_t   plotmode;    //VECTOR or SCATTER
extern  float    tick_int_x;
extern  float    tick_int_y;
extern  float    x_min; 
extern  float    x_max;
extern  float    y_min;
extern  float    y_max;
extern  uint32_t xzero;
extern  uint32_t yzero;



//Screen limits
#define SCREEN_MIN_GRAPH    20
#define SCREEN_HEIGHT_GRAPH SCREEN_HEIGHT
#define SCREEN_WIDTH_GRAPH  SCREEN_WIDTH
#define SCREEN_NONSQ_HMIN   0



//Utility functions
void    placePixel         (uint32_t x, uint32_t y);
void    removePixel        (uint32_t x, uint32_t y);
void    clearScreenPixels  ();
void    plotcross          (uint16_t xn, uint8_t yn);              // Plots line from xo,yo to xn,yn; uses temporary x1,y1
void    plotbox            (uint16_t xn, uint8_t yn);                // Plots line from xo,yo to xn,yn; uses temporary x1,y1
void    pixelline          (uint16_t xo, uint8_t yo, uint16_t xn, uint8_t yn, bool_t vmNormal);              // Plots line from xo,yo to xn,yn; uses temporary x1,y1
void    plotline           (uint16_t xo, uint8_t yo, uint16_t xn, uint8_t yn);
void    plotline2          (uint16_t xo, uint8_t yo, uint16_t xn, uint8_t yn);
void    graphAxisDraw      (void);
void    realToFloat        (const real_t *vv, float *v);
void    realToDouble1       (const real_t *vv, double *v);
void    graph_axis         (void);
float   auto_tick          (float tick_int_f);	


//graph functions
void    graph_setupmemory  (void);
void    graph_sigmaplus    (int8_t plusminus, real_t *xx, real_t *yy);   //Called from STAT module from fnSigma(), to store the x,y pair to the memory structure.
void    graph_end          (void);
float grf_x(int i);
float grf_y(int i);

#ifndef TESTSUITE_BUILD
int16_t screen_window_x(float x_min, float x, float x_max);
int16_t screen_window_y(float y_min, float y, float y_max);
#endif

void    fnStatDemo0        (uint16_t unusedButMandatoryParameter);
void    fnStatDemo1        (uint16_t unusedButMandatoryParameter);
void    fnStatDemo2        (uint16_t unusedButMandatoryParameter);
void    fnStatDemo105      (uint16_t unusedButMandatoryParameter);
void    fnStatDemo107      (uint16_t unusedButMandatoryParameter);
void    fnStatDemo109      (uint16_t unusedButMandatoryParameter);
void    graphPlotstat      (uint16_t selection);
void    graphDrawLRline    (uint16_t selection);
void    fnPlotClose        (uint16_t unusedButMandatoryParameter);
void    fnPlotCloseSmi     (uint16_t unusedButMandatoryParameter);
void    fnPlotStat         (uint16_t unusedButMandatoryParameter);
void    fnPlotZoom         (uint16_t unusedButMandatoryParameter);

#endif // PLOTSTAT_H 
