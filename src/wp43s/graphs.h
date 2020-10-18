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

//Memory structure
float/*double*/*   gr_x;
float/*double*/*   gr_y;
float/*double*/    telltale;
uint16_t ix_count;
#define  LIM               400                 //Number of points
#define  MEM_INITIALIZED   199


extern float graph_dx;                                               //JM Graph
extern float graph_dy;                                               //JM Graph
extern bool_t extentx;                                              //JM Graph
extern bool_t extenty;                                              //JM Graph
extern bool_t jm_VECT;                                               //JM GRAPH setup
extern bool_t jm_NVECT;                                               //JM GRAPH setup
extern bool_t Aspect_Square;
extern bool_t PLOT_LINE;
extern bool_t PLOT_CROSS;
extern bool_t PLOT_BOX;


//Screen limits
#define SCREEN_MIN_GRAPH 20
#define SCREEN_HEIGHT_GRAPH SCREEN_HEIGHT
#define SCREEN_WIDTH_GRAPH  SCREEN_WIDTH

#define _VECT 0
#define _SCAT 1

int8_t plotmode;                            //      _SCAT

void graph_reset      ();
void fnPline          (uint16_t unusedParamButMandatory);
void fnPcros          (uint16_t unusedParamButMandatory);
void fnPbox           (uint16_t unusedParamButMandatory);
void fnPvect          (uint16_t unusedParamButMandatory);
void fnPNvect         (uint16_t unusedParamButMandatory);
void fnPx             (uint16_t unusedParamButMandatory);
void fnPy             (uint16_t unusedParamButMandatory);
void pixelline        (uint16_t xo, uint8_t yo, uint16_t xn, uint8_t yn, bool_t vmNormal);              // Plots line from xo,yo to xn,yn; uses temporary x1,y1
void plotline         (uint16_t xo, uint8_t yo, uint16_t xn, uint8_t yn);
void graph_setupmemory(void);
void graph_sigmaplus  (int8_t plusminus, real_t *xx, real_t *yy);   //Called from STAT module from fnSigma(), to store the x,y pair to the memory structure.
void graph_end        (void);
void fnStatList       (uint16_t unusedParamButMandatory);
void graph_plotmem    (void);
void fnPlot           (uint16_t unusedParamButMandatory);
void fnPlotLS         (uint16_t unusedParamButMandatory);


#ifndef TESTSUITE_BUILD
int16_t screen_window_x(float/*double*/ x_min, float/*double*/ x, float/*double*/ x_max);
int16_t screen_window_y(float/*double*/ y_min, float/*double*/ y, float/*double*/ y_max);
#endif
float auto_tick(float tick_int_f);
