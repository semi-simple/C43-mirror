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
  for(i=0; i<numberOfFreeMemoryRegions; i++) {
    freeMem += freeMemoryRegions[i].sizeInBlocks;
  }

  return TO_BYTES(freeMem);
}

#ifndef DMCP_BUILD
void debugMemory(void) {
  printf("WP43S owns %6" PRIu64 " bytes and GMP owns %6" PRIu64 " bytes (%" PRId32 " bytes free)\n", (uint64_t)wp43sMemInBytes, (uint64_t)gmpMemInBytes, getFreeRamMemory());
  printf("    Addr   Size\n");
  for(int i=0; i<numberOfFreeMemoryRegions; i++) {
    printf("%2d%6u%7u\n", i, freeMemoryRegions[i].address, freeMemoryRegions[i].sizeInBlocks);
  }
  printf("\n");
}
#endif





void *allocWp43s(size_t sizeInBytes) {
  //if(debugMemAllocation) printf("allocWp43s\n");

  sizeInBytes = TO_BYTES(TO_BLOCKS(sizeInBytes));
  wp43sMemInBytes += sizeInBytes;

  //if(debugMemAllocation) printf("WP43S claims %6" PRIu64 " bytes\n", sizeInBytes);
  return wp43sAllocate(sizeInBytes);
}

void *reallocWp43s(void *pcMemPtr, size_t oldSizeInBytes, size_t newSizeInBytes) {
  //if(debugMemAllocation) printf("reallocWp43s\n");

  newSizeInBytes = TO_BYTES(TO_BLOCKS(newSizeInBytes));
  oldSizeInBytes = TO_BYTES(TO_BLOCKS(oldSizeInBytes));

  wp43sMemInBytes += newSizeInBytes - oldSizeInBytes;

  //if(debugMemAllocation) printf("WP43S claimed %6" PRIu64 " bytes, freed %6" PRIu64 " bytes and holds now %6" PRIu64 " bytes\n", (uint64_t)newSizeInBytes, (uint64_t)oldSizeInBytes, (uint64_t)wp43sMemInBytes);
  return wp43sReallocate(pcMemPtr, oldSizeInBytes, newSizeInBytes);
}

void freeWp43s(void *pcMemPtr, size_t sizeInBytes) {
  //if(debugMemAllocation) printf("freeWp43s\n");

  sizeInBytes = TO_BYTES(TO_BLOCKS(sizeInBytes));
  wp43sMemInBytes -= sizeInBytes;

  //if(debugMemAllocation) printf("WP43S frees  %6" PRIu64 " bytes\n", sizeInBytes);
  wp43sFree(pcMemPtr, sizeInBytes);
}





void *allocGmp(size_t sizeInBytes) {
  //if(debugMemAllocation) printf("allocGmp\n");

  sizeInBytes = TO_BYTES(TO_BLOCKS(sizeInBytes));
  gmpMemInBytes += sizeInBytes;

  //if(debugMemAllocation) printf("GMP claimed %6" PRIu64 " bytes and holds now %6" PRIu64 " bytes\n", (uint64_t)sizeInBytes, (uint64_t)gmpMemInBytes);
  //return wp43sAllocate(sizeInBytes);
  return malloc(sizeInBytes);
}

void *reallocGmp(void *pcMemPtr, size_t oldSizeInBytes, size_t newSizeInBytes) {
  //if(debugMemAllocation) printf("reallocGmp\n");

  newSizeInBytes = TO_BYTES(TO_BLOCKS(newSizeInBytes));
  oldSizeInBytes = TO_BYTES(TO_BLOCKS(oldSizeInBytes));

  gmpMemInBytes += newSizeInBytes - oldSizeInBytes;

  //if(debugMemAllocation) printf("GMP claimed %6" PRIu64 " bytes, freed %6" PRIu64 " bytes and holds now %6" PRIu64 " bytes\n", (uint64_t)newSizeInBytes, (uint64_t)oldSizeInBytes, (uint64_t)gmpMemInBytes);
  //return wp43sReallocate(pcMemPtr, oldSizeInBytes, newSizeInBytes);
  return realloc(pcMemPtr, newSizeInBytes);
}

void freeGmp(void *pcMemPtr, size_t sizeInBytes) {
  //if(debugMemAllocation) printf("freeGmp\n");

  sizeInBytes = TO_BYTES(TO_BLOCKS(sizeInBytes));
  gmpMemInBytes -= sizeInBytes;

  //if(debugMemAllocation) printf("GMP freed   %6" PRIu64 " bytes and holds now %6" PRIu64 " bytes\n", (uint64_t)sizeInBytes, (uint64_t)gmpMemInBytes);
  //wp43sFree(pcMemPtr, sizeInBytes);
  free(pcMemPtr);
}





void *wp43sAllocate(size_t sizeInBytes) {
  uint16_t sizeInBlocks;
  uint16_t minSizeInBlocks, minBlock;
  int i;
  void *pcMemPtr;

  if(sizeInBytes == 0) {
    sizeInBytes = 1;
  }
  sizeInBlocks = TO_BLOCKS(sizeInBytes);
  sizeInBytes = TO_BYTES(sizeInBlocks);
  //if(debugMemAllocation) printf("Allocating %" PRIu64 " bytes (%" PRIu16 " blocks)\n", sizeInBytes, sizeInBlocks);

  // Search the smalest hole where the claimed block fits
  //debugMemory();
  minSizeInBlocks = 65535u;
  minBlock = WP43S_NULL;
  for(i=0; i<numberOfFreeMemoryRegions; i++) {
    if(freeMemoryRegions[i].sizeInBlocks == sizeInBlocks) {
      //if(debugMemAllocation) printf("The block found is the size of the one claimed at address %u\n", freeMemoryRegions[i].address);
      pcMemPtr = TO_PCMEMPTR(freeMemoryRegions[i].address);
      xcopy(freeMemoryRegions + i, freeMemoryRegions + i + 1, (numberOfFreeMemoryRegions-i-1) * sizeof(freeMemoryRegion_t));
      numberOfFreeMemoryRegions--;
      //debugMemory();
      return pcMemPtr;
    }
    else if(freeMemoryRegions[i].sizeInBlocks > sizeInBlocks && freeMemoryRegions[i].sizeInBlocks < minSizeInBlocks) {
      minSizeInBlocks = freeMemoryRegions[i].sizeInBlocks;
      minBlock = i;
    }
  }

  if(minBlock == WP43S_NULL) {
    minSizeInBlocks = 0;
    for(i=0; i<numberOfFreeMemoryRegions; i++) {
      minSizeInBlocks += freeMemoryRegions[i].sizeInBlocks;
    }
    #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
      printf("\nOUT OF MEMORY\nMemory claimed: %" PRIu64 " bytes\nFragmented free memory: %u bytes\n", (uint64_t)sizeInBytes, TO_BYTES(minSizeInBlocks));
      exit(-3);
    #endif

    #ifdef DMCP_BUILD
      backToSystem(NOPARAM);
    #endif
  }

  //if(debugMemAllocation) printf("The block found is larger than the one claimed\n");
  pcMemPtr = TO_PCMEMPTR(freeMemoryRegions[minBlock].address);
  freeMemoryRegions[minBlock].address += sizeInBlocks;
  freeMemoryRegions[minBlock].sizeInBlocks -= sizeInBlocks;

  //debugMemory();
  return pcMemPtr;
}

void *wp43sReallocate(void *oldMemPtr, size_t oldSizeInBytes, size_t newSizeInBytes) {
  void *newMemPtr;

  // GMP never calls realloc with oldMemPtr beeing NULL
  if(oldSizeInBytes == 0) {
    oldSizeInBytes = 1;
  }

  if(newSizeInBytes == 0) {
    newSizeInBytes = 1;
  }

  newSizeInBytes = TO_BYTES(TO_BLOCKS(newSizeInBytes));
  oldSizeInBytes = TO_BYTES(TO_BLOCKS(oldSizeInBytes));
  //printf("Allocating %zd bytes and freeing %zd bytes\n", newSizeInBytes, oldSizeInBytes);

  newMemPtr = wp43sAllocate(newSizeInBytes);
  xcopy(newMemPtr, oldMemPtr, min(newSizeInBytes, oldSizeInBytes));
  wp43sFree(oldMemPtr, oldSizeInBytes);

  return newMemPtr;
}

void wp43sFree(void *pcMemPtr, size_t sizeInBytes) {
  uint16_t ramPtr, sizeInBlocks, addr;
  int32_t i, j;
  bool_t done;

  // GMP never calls free with pcMemPtr beeing NULL
  if(pcMemPtr == NULL) {
    return;
  }

  if(sizeInBytes == 0) {
    sizeInBytes = 1;
  }
  ramPtr = TO_WP43SMEMPTR(pcMemPtr);
  sizeInBlocks = TO_BLOCKS(sizeInBytes);
  //printf("Freeing %zd bytes\n", TO_BYTES(sizeInBlocks));

  done = false;

  // is the freed block just before an other free block?
  addr = ramPtr + sizeInBlocks;
  for(i=0; i<numberOfFreeMemoryRegions && freeMemoryRegions[i].address<=addr; i++) {
    if(freeMemoryRegions[i].address == addr) {
      freeMemoryRegions[i].address = ramPtr;
      freeMemoryRegions[i].sizeInBlocks += sizeInBlocks;
      sizeInBlocks = freeMemoryRegions[i].sizeInBlocks;
      j = i;
      done = true;
      break;
    }
  }

  // is the freed block just after an other free block?
  for(i=0; i<numberOfFreeMemoryRegions && freeMemoryRegions[i].address+freeMemoryRegions[i].sizeInBlocks<=ramPtr; i++) {
    if(freeMemoryRegions[i].address + freeMemoryRegions[i].sizeInBlocks == ramPtr) {
      freeMemoryRegions[i].sizeInBlocks += sizeInBlocks;
      if(done) {
        xcopy(freeMemoryRegions + j, freeMemoryRegions + j + 1, (numberOfFreeMemoryRegions-j-1) * sizeof(freeMemoryRegion_t));
        numberOfFreeMemoryRegions--;
      }
      else {
        done = true;
      }
      break;
    }
  }

  // new free block
  if(!done) {
    if(numberOfFreeMemoryRegions == MAX_FREE_REGION) {
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
    while(i<numberOfFreeMemoryRegions && freeMemoryRegions[i].address < ramPtr) {
      i++;
    }

    if(i < numberOfFreeMemoryRegions) {
      xcopy(freeMemoryRegions + i + 1, freeMemoryRegions + i, (numberOfFreeMemoryRegions-i) * sizeof(freeMemoryRegion_t));
    }

    freeMemoryRegions[i].address = ramPtr;
    freeMemoryRegions[i].sizeInBlocks = sizeInBlocks;
    numberOfFreeMemoryRegions++;
  }

  //debugMemory();
}


void resizeProgramMemory(uint16_t newSizeInBlocks) {
  uint16_t currentSizeInBlocks = RAM_SIZE - freeMemoryRegions[numberOfFreeMemoryRegions - 1].address - freeMemoryRegions[numberOfFreeMemoryRegions - 1].sizeInBlocks;

  printf("currentSizeInBlocks = %u    newSizeInBlocks = %u\n",currentSizeInBlocks, newSizeInBlocks);
  if(newSizeInBlocks == currentSizeInBlocks) { // Nothing to do
    return;
  }

  if(newSizeInBlocks > currentSizeInBlocks) { // Increase program memory size
    if(newSizeInBlocks - currentSizeInBlocks > freeMemoryRegions[numberOfFreeMemoryRegions - 1].sizeInBlocks) {
      #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
        int32_t freeMemory = 0;
        for(int32_t i=0; i<numberOfFreeMemoryRegions; i++) {
          freeMemory += freeMemoryRegions[i].sizeInBlocks;
        }
        printf("\nOUT OF MEMORY\nMemory claimed: %" PRIu64 " bytes\nFragmented free memory: %u bytes\n", (uint64_t)TO_BYTES(newSizeInBlocks - currentSizeInBlocks), TO_BYTES(freeMemory));
        exit(-3);
      #endif

      #ifdef DMCP_BUILD
        backToSystem(NOPARAM);
      #endif
    }
    else {
      freeMemoryRegions[numberOfFreeMemoryRegions - 1].sizeInBlocks -= newSizeInBlocks - currentSizeInBlocks;
      xcopy(programMemoryPointer + TO_BYTES(newSizeInBlocks - currentSizeInBlocks), programMemoryPointer, newSizeInBlocks);
      programMemoryPointer += TO_BYTES(newSizeInBlocks - currentSizeInBlocks);
      listPrograms();
    }
  }
  else { // Decrease program memory size
    freeMemoryRegions[numberOfFreeMemoryRegions - 1].sizeInBlocks += currentSizeInBlocks - newSizeInBlocks;
    xcopy(programMemoryPointer + TO_BYTES(currentSizeInBlocks - newSizeInBlocks), programMemoryPointer, newSizeInBlocks);
    programMemoryPointer += TO_BYTES(currentSizeInBlocks - newSizeInBlocks);
    listPrograms();
  }

  exit(0);
}
