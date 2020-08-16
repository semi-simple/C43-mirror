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
 * \file curveFitting.c
 ***********************************************/

#include "wp43s.h"

/********************************************//**
 * \brief Sets the curve fitting mode
 *
 * \param[in] curveFitting uint16_t Curve fitting mode
 * \return void
 *
 ***********************************************/
void fnCurveFitting(uint16_t curveFitting) {
  #ifdef PC_BUILD
    uint16_t numberOfOnes;

    numberOfOnes = curveFitting - ((curveFitting >> 1) & 0x5555);
    numberOfOnes = (numberOfOnes & 0x3333) + ((numberOfOnes >> 2) & 0x3333);
    numberOfOnes = (numberOfOnes + (numberOfOnes >> 4)) & 0x0f0f;
    numberOfOnes = (uint16_t)(numberOfOnes * 0x0101) >> 8;

    if(numberOfOnes == 1) {
      printf("Use the ");
    }
    else {
      printf("Use the best fitting model out of\n");
    }

    if(curveFitting & CF_LINEAR_FITTING)      printf("linear ");
    if(curveFitting & CF_EXPONENTIAL_FITTING) printf("exponential ");
    if(curveFitting & CF_LOGARITHMIC_FITTING) printf("logarithmic ");
    if(curveFitting & CF_POWER_FITTING)       printf("power ");
    if(curveFitting & CF_ROOT_FITTING)        printf("root ");
    if(curveFitting & CF_HYPERBOLIC_FITTING)  printf("hyperbolic ");
    if(curveFitting & CF_PARABOLIC_FITTING)   printf("parabolic ");
    if(curveFitting & CF_CAUCHY_FITTING)      printf("Cauchy peak ");
    if(curveFitting & CF_GAUSS_FITTING)       printf("Gauss peak ");
    if(curveFitting & CF_ORTHOGONAL_FITTING)  printf("Orthogonal ");

    if(numberOfOnes == 1) {
      printf("fitting model.\n");
    }
    else {
      printf("\nfitting models.\n");
    }
  #endif // PC_BUILD
  fnRefreshRadioState(RB_CF, cf);                                               //dr
}

