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
 * \file agm.h
 ***********************************************/
#ifndef AGM_H
#define AGM_H

#include "realType.h"
#include <stdint.h>

void   fnAgm         (uint16_t unusedButMandatoryParameter);

void   realAgm       (const real_t *a, const real_t *b, real_t *res, realContext_t *realContext);
void   complexAgm    (const real_t *ar, const real_t *ai, const real_t *br, const real_t *bi, real_t *resr, real_t *resi, realContext_t *realContext);
void   realAgmForE   (const real_t *a, const real_t *b, real_t *c, real_t *res, realContext_t *realContext);
void   complexAgmForE(const real_t *ar, const real_t *ai, const real_t *br, const real_t *bi, real_t *cr, real_t *ci, real_t *resr, real_t *resi, realContext_t *realContext);
size_t realAgmStep   (const real_t *a, const real_t *b, real_t *res, real_t *aStep, real_t *bStep, size_t bufSize, realContext_t *realContext);
#endif // AGM_H
