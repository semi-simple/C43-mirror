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
 * \file softmenu.h
 ***********************************************/

uint8_t *getNthString           (uint8_t *ptr, int16_t n); // Starting with string 0 (the 1st string is returned for n=0)
void     fnDynamicMenu          (uint16_t unusedButMandatoryParameter);
#ifndef TESTSUITE_BUILD
  void   showSoftkey            (const char *label, int16_t xSoftkey, int16_t ySoftKey, videoMode_t videoMode, bool_t topLine, bool_t bottomLine, int8_t showCb, int16_t showValue);     //dr);
  void   showSoftmenuCurrentPart(void);
  void   showSoftmenu           (int16_t id);
  void   popSoftmenu            (void);
  bool_t currentSoftmenuScrolls (void);
  bool_t isAlphabeticSoftmenu   (void);
  bool_t isJMAlphaSoftmenu      (int16_t menuId);             //JM

  int16_t mm(int16_t id);                                     //JM
  const   int16_t menu_A_HOME[360];                           //JM

#endif // !TESTSUITE_BUILD

  void   setCatalogLastPos      (void);
  void   fnDumpMenus            (uint16_t unusedButMandatoryParameter);  //JM
