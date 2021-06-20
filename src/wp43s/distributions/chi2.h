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
 * \file chi2.h
 ***********************************************/
#ifndef CHI2_H
#define CHI2_H

#include "realType.h"
#include "typeDefinitions.h"
#include <stdint.h>

bool_t checkRegisterNoFP(calcRegister_t reg);

void fnChi2P(uint16_t unusedButMandatoryParameter);
void fnChi2L(uint16_t unusedButMandatoryParameter);
void fnChi2R(uint16_t unusedButMandatoryParameter);
void fnChi2I(uint16_t unusedButMandatoryParameter);

void WP34S_Pdf_Chi2 (const real_t *x, const real_t *k, real_t *res, realContext_t *realContext);
void WP34S_Cdfu_Chi2(const real_t *x, const real_t *k, real_t *res, realContext_t *realContext);
void WP34S_Cdf_Chi2 (const real_t *x, const real_t *k, real_t *res, realContext_t *realContext);
void WP34S_Qf_Chi2  (const real_t *x, const real_t *k, real_t *res, realContext_t *realContext);

#endif // CHI2_H
