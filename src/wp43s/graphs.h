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

/********************************************//** //JM
 * \file jmgraph.c Graphing module
 ***********************************************/

/* ADDITIONAL C43 functions and routines */

//graphs.h


//*******************************
//#define graphtype float/*double*/      //Define the type of floating point used in the STATS memory. float uses 4 bytes per number
// graphtype:
//
// For normal 32-bit floating-point values, this corresponds to values in the range from 1.175494351 * 10^-38 to 3.40282347 * 10^+38.
//
// Array: 400 x 2 * 4 byte = 3200 bytes required for STATS Graph array, which corresponds to 800 blocks.
//
//*******************************
#define graphtypeno 1 //1=float & 2=double


#if   (graphtypeno == 1)
  #define graphtype float  
  #define FLoatingMax 1e38     //convenient round figures used for maxima and minima determination
  #define FLoatingMin -1e38
#elif (graphtypeno == 2)
  #define graphtype double
  #define FLoatingMax 1e308
  #define FLoatingMin -1e308
#endif


//Memory structure
extern graphtype *gr_x;
extern graphtype *gr_y;
graphtype    telltale;
uint16_t ix_count;
#define  LIM               400 //Number of points; MUST be multiple of 4
#define  MEM_INITIALIZED   199


extern float graph_dx;                                               //JM Graph
extern float graph_dy;                                               //JM Graph
extern bool_t extentx;                                              //JM Graph
extern bool_t extenty;                                              //JM Graph
extern bool_t jm_VECT;                                               //JM GRAPH setup
extern bool_t jm_NVECT;                                               //JM GRAPH setup
extern bool_t jm_SCALE;                                              //JM GRAPH setup
extern bool_t Aspect_Square;
extern bool_t PLOT_LINE;
extern bool_t PLOT_CROSS;
extern bool_t PLOT_BOX;
extern bool_t PLOT_INTG; 
extern bool_t PLOT_DIFF; 
extern bool_t PLOT_RMS; 
extern bool_t PLOT_SHADE; 

//Screen limits
#define SCREEN_MIN_GRAPH 20
#define SCREEN_HEIGHT_GRAPH SCREEN_HEIGHT
#define SCREEN_WIDTH_GRAPH  SCREEN_WIDTH

#define _VECT 0
#define _SCAT 1

int8_t plotmode;                            //      _SCAT

void graph_reset      (void);
void fnClGrf          (uint16_t unusedButMandatoryParameter);
void fnPline          (uint16_t unusedButMandatoryParameter);
void fnPcros          (uint16_t unusedButMandatoryParameter);
void fnPbox           (uint16_t unusedButMandatoryParameter);
void fnPintg          (uint16_t unusedButMandatoryParameter);
void fnPdiff          (uint16_t unusedButMandatoryParameter);
void fnPrms           (uint16_t unusedButMandatoryParameter);
void fnPvect          (uint16_t unusedButMandatoryParameter);
void fnPNvect         (uint16_t unusedButMandatoryParameter);
void fnScale          (uint16_t unusedButMandatoryParameter);
void fnPshade         (uint16_t unusedButMandatoryParameter);
void fnPx             (uint16_t unusedButMandatoryParameter);
void fnPy             (uint16_t unusedButMandatoryParameter);
void pixelline        (uint16_t xo, uint8_t yo, uint16_t xn, uint8_t yn, bool_t vmNormal);              // Plots line from xo,yo to xn,yn; uses temporary x1,y1
void plotline         (uint16_t xo, uint8_t yo, uint16_t xn, uint8_t yn);
void graph_setupmemory(void);
void graph_sigmaplus  (int8_t plusminus, real_t *xx, real_t *yy);   //Called from STAT module from fnSigma(), to store the x,y pair to the memory structure.
void graph_end        (void);
void fnListXY         (uint16_t unusedButMandatoryParameter);
void fnStatList       (void);
void graph_plotmem    (void);
void fnPlot           (uint16_t unusedButMandatoryParameter);
void fnPlotLS         (uint16_t unusedButMandatoryParameter);

#ifndef TESTSUITE_BUILD
int16_t screen_window_x(graphtype x_min, graphtype x, graphtype x_max);
int16_t screen_window_y(graphtype y_min, graphtype y, graphtype y_max);
#endif
float auto_tick(float tick_int_f);
