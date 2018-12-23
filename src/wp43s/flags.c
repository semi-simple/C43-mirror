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
 * \file flags.c
 ***********************************************/

#include "wp43s.h"

/********************************************//**
 * \brief Returns the status of a flag
 *
 * \param[in] f uint16_t
 * \return bool_t
 ***********************************************/
bool_t getFlag(uint16_t f) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("getFlag");
  #endif

  if(f < NUMBER_OF_LOCAL_FLAGS) {

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("getFlag");
    #endif

    return (flags[f/16] & (1u << (f%16))) != 0;
  }
  else {
    if(numberOfLocalRegisters > 0) {
      f -= NUMBER_OF_LOCAL_FLAGS;
      if(f < 16) {

        #if (LOG_FUNCTIONS == 1)
          leavingFunction("getFlag");
        #endif

        return (*POINTER_TO_LOCAL_FLAGS & (1u << f)) != 0;
      }
      else {
        sprintf(errorMessage, "In function getFlag: local flag %" FMT16U " is not defined! Must be from 0 to 15.", f);
        displayBugScreen(errorMessage);
      }
    }
    #ifdef PC_BUILD
    else {
      showInfoDialog("In function getFlag:", "no local flags defined!", "To do so, you can find LocR here:", "[g] [P.FN] [g] [F5]");
    }
    #endif
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("getFlag");
  #endif

  return false;
 }



/********************************************//**
 * \brief Sets a flag
 *
 * \param[in] f uint16_t
 * \return void
 ***********************************************/
void setFlag(uint16_t f) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("setFlag");
  #endif

  if(f < NUMBER_OF_LOCAL_FLAGS) {
    flags[f/16] |= 1u << (f%16);

    if(f == FLAG_CPXRES) {
      showRealComplexResult();
    }
    else if(f == FLAG_OVERFLOW || f == FLAG_CARRY) {
      showOverflowCarry();
    }
  }
  else {
    if(numberOfLocalRegisters > 0) {
      f -= NUMBER_OF_LOCAL_FLAGS;
      if(f < 16) {
        *POINTER_TO_LOCAL_FLAGS |=  (1u << f);
      }
      else {
        sprintf(errorMessage, "In function setFlag: local flag %" FMT16U " is not defined! Must be from 0 to 15.", f);
        displayBugScreen(errorMessage);
      }
    }
    #ifdef PC_BUILD
    else {
      showInfoDialog("In function setFlag:", "no local flags defined!", "To do so, you can find LocR here:", "[g] [P.FN] [g] [F5]");
    }
    #endif
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("setFlag");
  #endif
}



/********************************************//**
 * \brief Clears a flag
 *
 * \param[in] f uint16_t
 * \return void
 ***********************************************/
void clearFlag(uint16_t f) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("clearFlag");
  #endif

  if(f < NUMBER_OF_LOCAL_FLAGS) {
    flags[f/16] &= ~(1u << (f%16));

    if(f == FLAG_CPXRES) {
      showRealComplexResult();
    }
    else if(f == FLAG_OVERFLOW || f == FLAG_CARRY) {
      showOverflowCarry();
    }
  }
  else {
    if(numberOfLocalRegisters > 0) {
      f -= NUMBER_OF_LOCAL_FLAGS;
      if(f < 16) {
        *POINTER_TO_LOCAL_FLAGS &= ~(1u << f);
      }
      else {
        sprintf(errorMessage, "In function clearFlag: local flag %" FMT16U " is not defined! Must be from 0 to 15.", f);
        displayBugScreen(errorMessage);
      }
    }
    #ifdef PC_BUILD
    else {
     showInfoDialog("In function clearFlag:", "no local flags defined!", "To do so, you can find LocR here:", "[g] [P.FN] [g] [F5]");
    }
   #endif
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("clearFlag");
  #endif
}



/********************************************//**
 * \brief Flips a flag
 *
 * \param[in] f uint16_t
 * \return void
 ***********************************************/
void flipFlag(uint16_t f) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("flipFlag");
  #endif

  if(f < NUMBER_OF_LOCAL_FLAGS) {
    flags[f/16] ^=  1u << (f%16);

    if(f == FLAG_CPXRES) {
      showRealComplexResult();
    }
    else if(f == FLAG_OVERFLOW || f == FLAG_CARRY) {
      showOverflowCarry();
    }
  }
  else {
    if(numberOfLocalRegisters > 0) {
      f -= NUMBER_OF_LOCAL_FLAGS;
      if(f < 16) {
        *POINTER_TO_LOCAL_FLAGS ^=  (1u << f);
      }
      else {
        sprintf(errorMessage, "In function flipFlag: local flag %" FMT16U " is not defined! Must be from 0 to 15.", f);
        displayBugScreen(errorMessage);
      }
    }
    #ifdef PC_BUILD
    else {
      showInfoDialog("In function flipFlag:", "no local flags defined!", "To do so, you can find LocR here:", "[g] [P.FN] [g] [F5]");
    }
    #endif
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("flipFlag");
  #endif
}
