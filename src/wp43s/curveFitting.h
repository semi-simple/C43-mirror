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
 * \file curveFitting.h
 ***********************************************/

#include "wp43s.h"


double a0,a1,a2, x_, y_,sx,sy,sxy,smi,r;
real_t AA,BB,CC,DD,EE,FF,GG,HH;  //Curve aux fitting variables
real_t SS,TT,UU,aa0,aa1,aa2;     //Curve fitting variables
uint16_t selection;              //Currently selected plot
realContext_t *realContext;

void fnCurveFitting           (uint16_t curveFitting);
void processCurvefitSelection (void);


//Temporary, for ease of checking using double type
double A, B, C, D, E, F, G, H;
int32_t nn      ;
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

