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

/* ADDITIONAL WP43C functions and routines */

//graph.h



#define SCREEN_MIN_GRAPH 20
#define SCREEN_HEIGHT_GRAPH SCREEN_HEIGHT
#define LIM 400   


void fnGraph          (uint16_t func);

void graph_sigmaplus(void);
void graph_sigmaminus(void);

void graph_setupmemory(void);
void graph_end        (void);


//TODO
void autoscale(void);
void defaultscale(void);
