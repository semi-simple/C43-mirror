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
 * \file gui.h
 ***********************************************/
#ifndef GUI_H
#define GUI_H

#include "defines.h"
#include "typeDefinitions.h"
#include <stdint.h>

#ifndef TESTSUITE_BUILD
  void btn_Clicked_Gen(bool_t shF, bool_t shG, char *st);
  void fnOff                       (uint16_t unsuedParamButMandatory);
  void calcModeNormal              (void);
  void calcModeAim                 (uint16_t unusedButMandatoryParameter);
  void calcModeNim                 (uint16_t unusedButMandatoryParameter);
  void enterAsmModeIfMenuIsACatalog(int16_t id);
  void leaveAsmMode                (void);
#endif // TESTSUITE_BUILD

#ifdef PC_BUILD
  void setupUI            (void);
  #if (SCREEN_800X480 == 0)
    void moveLabels       (void);     //JM Keep here: legacy of old gui
    void calcModeNormalGui(void);
    void calcModeAimGui   (void);
    void calcModeTamGui   (void);
    void refreshModeGui   (void);     //JM Added here to force icon update in Gui
  #endif // SCREEN_800X480
#endif // PC_BUILD

#ifndef DMCP_BUILD
  void strReplace         (char *haystack, const char *needle, const char *newNeedle);
#endif // DMCP_BUILD

#endif // GUI_H
