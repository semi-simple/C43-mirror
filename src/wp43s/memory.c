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
    freeMem += freeBlocks[i].sizeInBlocks;
  }

  return freeMem << MEMORY_ALLOCATION_SHIFT;
}

#ifndef DMCP_BUILD
void debugMemory(void) {
  printf("WP43S owns %6" FMTSIZE " bytes and GMP owns %6" FMTSIZE " bytes (%" FMT32S " bytes free)\n", wp43sMemInBytes, gmpMemInBytes, getFreeRamMemory());
  printf("    Addr   Size\n");
  for(int i=0; i<numberOfFreeBlocks; i++) {
    printf("%2d%6u%7u\n", i, freeBlocks[i].address, freeBlocks[i].sizeInBlocks);
  }
  printf("\n");
}
#endif

void *allocWp43s(size_t sizeInBytes) {
  #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
    if(debugMemAllocation) printf("allocWp43s\n");
  #endif

  sizeInBytes = BLOCKS_TO_BYTES(BYTES_TO_BLOCKS(sizeInBytes));
  wp43sMemInBytes += sizeInBytes;

  #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
    if(debugMemAllocation) printf("WP43S claims %6" FMTSIZE " bytes\n", sizeInBytes);
  #endif

  return wp43sAllocate(sizeInBytes);
}

void *reallocWp43s(void *pcMemPtr, size_t oldSizeInBytes, size_t newSizeInBytes) {
  #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
    if(debugMemAllocation) printf("reallocWp43s\n");
  #endif

  newSizeInBytes = BLOCKS_TO_BYTES(BYTES_TO_BLOCKS(newSizeInBytes));
  oldSizeInBytes = BLOCKS_TO_BYTES(BYTES_TO_BLOCKS(oldSizeInBytes));

  wp43sMemInBytes += newSizeInBytes - oldSizeInBytes;

  #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
    if(debugMemAllocation) printf("WP43S claimed %6" FMTSIZE " bytes, freed %6" FMTSIZE " bytes and holds now %6" FMTSIZE " bytes\n", newSizeInBytes, oldSizeInBytes, wp43sMemInBytes);
  #endif

  return wp43sReallocate(pcMemPtr, oldSizeInBytes, newSizeInBytes);
}

void freeWp43s(void *pcMemPtr, size_t sizeInBytes) {
  #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
    if(debugMemAllocation) printf("freeWp43s\n");
  #endif

  sizeInBytes = BLOCKS_TO_BYTES(BYTES_TO_BLOCKS(sizeInBytes));
  wp43sMemInBytes -= sizeInBytes;

  #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
    if(debugMemAllocation) printf("WP43S frees  %6" FMTSIZE " bytes\n", sizeInBytes);
  #endif

  wp43sFree(pcMemPtr, sizeInBytes);
}



void *allocGmp(size_t sizeInBytes) {
  #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
    if(debugMemAllocation) printf("allocGmp\n");
  #endif

  sizeInBytes = BLOCKS_TO_BYTES(BYTES_TO_BLOCKS(sizeInBytes));
  gmpMemInBytes += sizeInBytes;

  #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
    if(debugMemAllocation) printf("GMP claimed %6" FMTSIZE " bytes and holds now %6" FMTSIZE " bytes\n", sizeInBytes, gmpMemInBytes);
  #endif

  return wp43sAllocate(sizeInBytes);
}

void *reallocGmp(void *pcMemPtr, size_t oldSizeInBytes, size_t newSizeInBytes) {
  #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
    if(debugMemAllocation) printf("reallocGmp\n");
  #endif

  newSizeInBytes = BLOCKS_TO_BYTES(BYTES_TO_BLOCKS(newSizeInBytes));
  oldSizeInBytes = BLOCKS_TO_BYTES(BYTES_TO_BLOCKS(oldSizeInBytes));

  gmpMemInBytes += newSizeInBytes - oldSizeInBytes;

  #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
    if(debugMemAllocation) printf("GMP claimed %6" FMTSIZE " bytes, freed %6" FMTSIZE " bytes and holds now %6" FMTSIZE " bytes\n", newSizeInBytes, oldSizeInBytes, gmpMemInBytes);
  #endif

  return wp43sReallocate(pcMemPtr, oldSizeInBytes, newSizeInBytes);
}

void freeGmp(void *pcMemPtr, size_t sizeInBytes) {
  #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
    if(debugMemAllocation) printf("freeGmp\n");
  #endif

  sizeInBytes = BLOCKS_TO_BYTES(BYTES_TO_BLOCKS(sizeInBytes));
  gmpMemInBytes -= sizeInBytes;

  #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
    if(debugMemAllocation) printf("GMP freed   %6" FMTSIZE " bytes and holds now %6" FMTSIZE " bytes\n", sizeInBytes, gmpMemInBytes);
  #endif

  wp43sFree(pcMemPtr, sizeInBytes);
}



void *wp43sAllocate(size_t sizeInBytes) {
  uint16_t sizeInBlocks = BYTES_TO_BLOCKS(sizeInBytes);
  uint16_t minSizeInBlocks, minBlock;
  int i;
  void *pcMemPtr;

  sizeInBytes = BLOCKS_TO_BYTES(sizeInBlocks);
  #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
    if(debugMemAllocation) {
      //printf("Allocating %" FMTSIZE " bytes (%" FMT16U " blocks)\n", sizeInBytes, sizeInBlocks);
    }
  #endif

  // Search the smalest hole where the claimed block fits
  //debugMemory();
  minSizeInBlocks = 65535u;
  minBlock = 65535u;
  for(i=0; i<numberOfFreeBlocks; i++) {
    if(freeBlocks[i].sizeInBlocks == sizeInBlocks) {
      #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
        if(debugMemAllocation) {
          //printf("The block found is the size of the one claimed at address %u\n", freeBlocks[i].address);
        }
      #endif
      pcMemPtr = WP43SMEMPTR_TO_PCMEMPTR(freeBlocks[i].address);
      xcopy(freeBlocks + i, freeBlocks + i + 1, (numberOfFreeBlocks-i-1) * sizeof(freeBlock_t));
      numberOfFreeBlocks--;
      //debugMemory();
      return pcMemPtr;
    }
    else if(freeBlocks[i].sizeInBlocks > sizeInBlocks && freeBlocks[i].sizeInBlocks < minSizeInBlocks) {
      minSizeInBlocks = freeBlocks[i].sizeInBlocks;
      minBlock = i;
    }
  }

  if(minSizeInBlocks == 65535u) {
    minSizeInBlocks = 0;
    for(i=0; i<numberOfFreeBlocks; i++) {
      minSizeInBlocks += freeBlocks[i].sizeInBlocks;
    }
    #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
      printf("\nOUT OF MEMORY\nMemory claimed: %" FMTSIZE " bytes\nFragmented free memory: %u bytes\n", sizeInBytes, minSizeInBlocks * MEMORY_ALLOCATION_ALIGNMENT);
      exit(-3);
    #endif

    #ifdef DMCP_BUILD
      backToSystem(NOPARAM);
    #endif
  }

  #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
    if(debugMemAllocation) {
      //printf("The block found is larger than the one claimed\n");
    }
  #endif
  pcMemPtr = WP43SMEMPTR_TO_PCMEMPTR(freeBlocks[minBlock].address);
  freeBlocks[minBlock].address += sizeInBlocks;
  freeBlocks[minBlock].sizeInBlocks -= sizeInBlocks;

  //debugMemory();
  return pcMemPtr;
}

void *wp43sReallocate(void *oldMemPtr, size_t oldSizeInBytes, size_t newSizeInBytes) {
  void *newMemPtr;

  newSizeInBytes = BLOCKS_TO_BYTES(BYTES_TO_BLOCKS(newSizeInBytes));
  oldSizeInBytes = BLOCKS_TO_BYTES(BYTES_TO_BLOCKS(oldSizeInBytes));
  //printf("Allocating %zd bytes and freeing %zd bytes\n", newSizeInBytes, oldSizeInBytes);

  newMemPtr = wp43sAllocate(newSizeInBytes);
  xcopy(newMemPtr, oldMemPtr, min(newSizeInBytes, oldSizeInBytes));
  wp43sFree(oldMemPtr, oldSizeInBytes);

  return newMemPtr;
}

void wp43sFree(void *pcMemPtr, size_t sizeInBytes) {
  uint16_t ramPtr = PCMEMPTR_TO_WP43SMEMPTR(pcMemPtr);
  uint16_t sizeInBlocks = BYTES_TO_BLOCKS(sizeInBytes);
  uint16_t addr;
  int32_t i, j;
  bool_t done;

  sizeInBytes = BLOCKS_TO_BYTES(sizeInBlocks);
  //printf("Freeing %zd bytes\n", sizeInBytes);

  done = false;

  // is the freed block just before an other free block?
  addr = ramPtr + sizeInBlocks;
  for(i=0; i<numberOfFreeBlocks && freeBlocks[i].address<=addr; i++) {
    if(freeBlocks[i].address == addr) {
      freeBlocks[i].address = ramPtr;
      freeBlocks[i].sizeInBlocks += sizeInBlocks;
      sizeInBlocks = freeBlocks[i].sizeInBlocks;
      j = i;
      done = true;
      break;
    }
  }

  // is the freed block just after an other free block?
  for(i=0; i<numberOfFreeBlocks && freeBlocks[i].address+freeBlocks[i].sizeInBlocks<=ramPtr; i++) {
    if(freeBlocks[i].address + freeBlocks[i].sizeInBlocks == ramPtr) {
      freeBlocks[i].sizeInBlocks += sizeInBlocks;
      if(done) {
        xcopy(freeBlocks + j, freeBlocks + j + 1, (numberOfFreeBlocks-j-1) * sizeof(freeBlock_t));
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
      #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
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
      xcopy(freeBlocks + i + 1, freeBlocks + i, (numberOfFreeBlocks-i) * sizeof(freeBlock_t));
    }

    freeBlocks[i].address = ramPtr;
    freeBlocks[i].sizeInBlocks = sizeInBlocks;
    numberOfFreeBlocks++;
  }

  //debugMemory();
}
