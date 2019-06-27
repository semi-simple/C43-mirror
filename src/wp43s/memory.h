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
 * \file memory.h
 ***********************************************/

typedef struct {
  uint16_t address;
  uint16_t size;
} freeBlock_t;

void    *wp43sAllocate  (size_t size);
void    *wp43sReallocate(void *memPtr, size_t oldSize, size_t newSize);
void    wp43sFree       (void *memPtr, size_t size);

// The 6 followoing functions are only there to know who allocates and frees memory
void    *allocWp43s     (size_t size);
void    *reallocWp43s   (void *memPtr, size_t oldSize, size_t newSize);
void    freeWp43s       (void *memPtr, size_t size);

void    *allocGmp       (size_t size);
void    *reallocGmp     (void *memPtr, size_t oldSize, size_t newSize);
void    freeGmp         (void *memPtr, size_t size);

int32_t getFreeRamMemory(void);

#ifndef DMCP_BUILD
  void    debugMemory(void);
#endif
