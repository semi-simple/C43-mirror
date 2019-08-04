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
 * \file memory.c
 ***********************************************/

#include "wp43s.h"

int32_t getFreeRamMemory(void) {
  int32_t freeMem, i;

  freeMem = 0;
  for(i=0; i<numberOfFreeBlocks; i++) {
    freeMem += freeBlocks[i].size;
  }

  return freeMem << MEMORY_ALLOCATION_SHIFT;
}

#ifndef DMCP_BUILD
void debugMemory(void) {
  printf("WP43S owns %6" FMTSIZE " bytes and GMP owns %6" FMTSIZE " bytes (%" FMT32S " bytes free)\n", wp43sMem, gmpMem, getFreeRamMemory());
  printf("    Addr   Size\n");
  for(int i=0; i<numberOfFreeBlocks; i++) {
    printf("%2d%6u%7u\n", i, freeBlocks[i].address, freeBlocks[i].size);
  }
  printf("\n");
}
#endif

void *allocWp43s(size_t size) {
  //printf("allocWp43s\n");
  size = BLOCKS_TO_BYTES(BYTES_TO_BLOCKS(size));
  wp43sMem += size;

  //printf("WP43S claims %6" FMTSIZE " bytes\n", size);
  return wp43sAllocate(size);
}

void *reallocWp43s(void *memPtr, size_t oldSize, size_t newSize) {
  //printf("reallocWp43s\n");
  newSize = BLOCKS_TO_BYTES(BYTES_TO_BLOCKS(newSize));
  oldSize = BLOCKS_TO_BYTES(BYTES_TO_BLOCKS(oldSize));

  wp43sMem += newSize - oldSize;
  //printf("WP43S claims %6" FMTSIZE " bytes\n", newSize);
  //printf(" and frees %6" FMTSIZE " bytes\n", oldSize);

  return wp43sReallocate(memPtr, oldSize, newSize);
}

void freeWp43s(void *memPtr, size_t size) {
  //printf("freeWp43s\n");
  size = BLOCKS_TO_BYTES(BYTES_TO_BLOCKS(size));
  wp43sMem -= size;

  //printf("WP43S frees  %6" FMTSIZE " bytes\n", size);
  wp43sFree(memPtr, size);
}



void *allocGmp(size_t size) {
  //printf("allocGmp\n");
  size = BLOCKS_TO_BYTES(BYTES_TO_BLOCKS(size));
  gmpMem += size;

  //printf("GMP claimed %6" FMTSIZE " bytes and holds now %6" FMTSIZE " bytes\n", size, gmpMem);
  return wp43sAllocate(size);
}

void *reallocGmp(void *memPtr, size_t oldSize, size_t newSize) {
  //printf("reallocGmp\n");
  newSize = BLOCKS_TO_BYTES(BYTES_TO_BLOCKS(newSize));
  oldSize = BLOCKS_TO_BYTES(BYTES_TO_BLOCKS(oldSize));

  gmpMem += newSize - oldSize;
  //printf("GMP claimed %6" FMTSIZE " bytes, freed %6" FMTSIZE " bytes and holds now %6" FMTSIZE " bytes\n", newSize, oldSize, gmpMem);

  return wp43sReallocate(memPtr, oldSize, newSize);
}

void freeGmp(void *memPtr, size_t size) {
  //printf("freeGmp\n");
  size = BLOCKS_TO_BYTES(BYTES_TO_BLOCKS(size));
  gmpMem -= size;

  //printf("GMP freed   %6" FMTSIZE " bytes and holds now %6" FMTSIZE " bytes\n", size, gmpMem);
  wp43sFree(memPtr, size);
}



void *wp43sAllocate(size_t size) {
  uint16_t blocks = BYTES_TO_BLOCKS(size);
  uint16_t minSize, minBlock;
  int i;
  void *memPtr;

  size = BLOCKS_TO_BYTES(blocks);
  //printf("Allocating %zd bytes (%u blocks)\n", size, blocks);

  // Search the smalest hole where the claimed block fits
  minSize = 65535u;
  minBlock = 65535u;
  for(i=0; i<numberOfFreeBlocks; i++) {
    if(freeBlocks[i].size == blocks) {
      memPtr = RAMPTR_TO_MEMPTR(freeBlocks[i].address);
      memmove(ram + i*sizeof(freeBlock_t), ram + (i+1)*sizeof(freeBlock_t), (numberOfFreeBlocks-i-1)*sizeof(freeBlock_t));
      numberOfFreeBlocks--;
      //debugMemory();
      return memPtr;
    }
    else if(freeBlocks[i].size > blocks && freeBlocks[i].size < minSize) {
      minSize = freeBlocks[i].size;
      minBlock = i;
    }
  }

  if(minSize == 65535u) {
    minSize = 0;
    for(i=0; i<numberOfFreeBlocks; i++) {
      minSize += freeBlocks[i].size;
    }
    printf("\nOUT OF MEMORY\nMemory claimed: %" FMTSIZE " bytes\nFragmented free memory: %u bytes\n", size, minSize * MEMORY_ALLOCATION_ALIGNMENT);
    #ifdef PC_BUILD
      exit(0);
    #endif

    #ifdef DMCP_BUILD
      backToSystem(NOPARAM);
    #endif
  }

  memPtr = RAMPTR_TO_MEMPTR(freeBlocks[minBlock].address);
  freeBlocks[minBlock].address += blocks;
  freeBlocks[minBlock].size -= blocks;

  //debugMemory();
  return memPtr;
}

void *wp43sReallocate(void *oldMemPtr, size_t oldSize, size_t newSize) {
  void *newMemPtr;

  newSize = BLOCKS_TO_BYTES(BYTES_TO_BLOCKS(newSize));
  oldSize = BLOCKS_TO_BYTES(BYTES_TO_BLOCKS(oldSize));
  //printf("Allocating %zd bytes and freeing %zd bytes\n", newSize, oldSize);

  newMemPtr = wp43sAllocate(newSize);
  memmove(newMemPtr, oldMemPtr, min(newSize, oldSize));
  wp43sFree(oldMemPtr, oldSize);

  return newMemPtr;
}

void wp43sFree(void *memPtr, size_t size) {
  uint16_t ramPtr = MEMPTR_TO_RAMPTR(memPtr);
  uint16_t blocks = BYTES_TO_BLOCKS(size);
  uint16_t addr;
  int32_t i, j;
  bool_t done;

  size = BLOCKS_TO_BYTES(blocks);
  //printf("Freeing %zd bytes\n", size);

  done = false;

  // is the freed block just before an other free block?
  addr = ramPtr + blocks;
  for(i=0; i<numberOfFreeBlocks && freeBlocks[i].address<=addr; i++) {
    if(freeBlocks[i].address == addr) {
      freeBlocks[i].address = ramPtr;
      freeBlocks[i].size += blocks;
      blocks = freeBlocks[i].size;
      j = i;
      done = true;
      break;
    }
  }

  // is the freed block just after an other free block?
  for(i=0; i<numberOfFreeBlocks && freeBlocks[i].address+freeBlocks[i].size<=ramPtr; i++) {
    if(freeBlocks[i].address + freeBlocks[i].size == ramPtr) {
      freeBlocks[i].size += blocks;
      if(done) {
        memmove(ram + j*sizeof(freeBlock_t), ram + (j+1)*sizeof(freeBlock_t), (numberOfFreeBlocks-j-1)*sizeof(freeBlock_t));
        numberOfFreeBlocks--;
      }
      else {
        done = true;
      }
      break;
    }
  }

  // new free block
  if(!done) {
    if(numberOfFreeBlocks == MAX_FREE_BLOCKS) {
      printf("\n**********************************************************************\n");
      printf("* The maximum number of free memory blocks has been exceeded!        *\n");
      printf("* This number must be increased or the compaction function improved. *\n");
      printf("**********************************************************************\n");
      #ifdef PC_BUILD
        exit(-2);
      #endif

      #ifdef DMCP_BUILD
        backToSystem(NOPARAM);
      #endif
    }

    i = 0;
    while(i<numberOfFreeBlocks && freeBlocks[i].address < ramPtr) {
      i++;
    }

    if(i < numberOfFreeBlocks) {
      memmove(ram + (i+1)*sizeof(freeBlock_t), ram + i*sizeof(freeBlock_t), (numberOfFreeBlocks-i)*sizeof(freeBlock_t));
    }

    freeBlocks[i].address = ramPtr;
    freeBlocks[i].size = blocks;
    numberOfFreeBlocks++;
  }

  //debugMemory();
}
