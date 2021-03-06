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

#include "keyboard.h"

#include "assign.h"
#include "bufferize.h"
#include "charString.h"
#include "constants.h"
#include "debug.h"
#include "display.h"
#include "error.h"
#include "flags.h"
#include "gui.h"
#include "items.h"
#include "matrix.h"
#include "c43Extensions/jm.h"
#include "c43Extensions/keyboardTweak.h"
#include "memory.h"
#include "plotstat.h"
#include "programming/manage.h"
#include "programming/nextStep.h"
#include "programming/programmableMenu.h"
#include "recall.h"
#include "registers.h"
#include "screen.h"
#include "softmenus.h"
#include "solver/equation.h"
#include "sort.h"
#include "stack.h"
#include "stats.h"
#include "timer.h"
#include "ui/tam.h"
#if (REAL34_WIDTH_TEST == 1)
  #include "registerValueConversions.h"
#endif // (REAL34_WIDTH_TEST == 1)

#include "wp43s.h"

#ifndef TESTSUITE_BUILD
  int16_t determineFunctionKeyItem(const char *data, int16_t itemShift) { //Added itemshift param JM
    int16_t item = ITM_NOP;

    dynamicMenuItem = -1;

    //int16_t itemShift = (shiftF ? 6 : (shiftG ? 12 : 0));    //removed JM
    int16_t fn = *(data) - '0';
    const softmenu_t *sm;
    int16_t row, menuId = softmenuStack[0].softmenuId;
    int16_t firstItem = softmenuStack[0].firstItem;

    switch(-softmenu[menuId].menuItem) {
      case MNU_MyMenu:
        dynamicMenuItem = firstItem + itemShift + (fn - 1);
        item = userMenuItems[dynamicMenuItem].item;
        break;

      case MNU_MyAlpha:
        dynamicMenuItem = firstItem + itemShift + (fn - 1);
        item = userAlphaItems[dynamicMenuItem].item;
        break;

      case MNU_DYNAMIC:
        dynamicMenuItem = firstItem + itemShift + (fn - 1);
        item = userMenus[currentUserMenu].menuItem[dynamicMenuItem].item;
        break;

      case MNU_PROG:
        dynamicMenuItem = firstItem + itemShift + (fn - 1);
        if(tam.function == ITM_GTOP) {
          item = ITM_GTOP;
        }
        else {
          item = (dynamicMenuItem >= dynamicSoftmenu[menuId].numItems ? ITM_NOP : MNU_DYNAMIC);
        }
        break;

      case MNU_VAR:
        dynamicMenuItem = firstItem + itemShift + (fn - 1);
        item = (dynamicMenuItem >= dynamicSoftmenu[menuId].numItems ? ITM_NOP : MNU_DYNAMIC);
        break;

      case MNU_MVAR:
        dynamicMenuItem = firstItem + itemShift + (fn - 1);
        if(currentMvarLabel != INVALID_VARIABLE) {
          item = (dynamicMenuItem >= dynamicSoftmenu[menuId].numItems ? ITM_NOP : ITM_SOLVE_VAR);
        }
        else if((currentSolverStatus & SOLVER_STATUS_USES_FORMULA) && (currentSolverStatus & SOLVER_STATUS_INTERACTIVE) && dynamicMenuItem == 5) {
          item = ITM_CALC;
        } 
        else if((currentSolverStatus & SOLVER_STATUS_USES_FORMULA) && (currentSolverStatus & SOLVER_STATUS_INTERACTIVE) && dynamicMenuItem == 4) {
          item = ITM_DRAW;
        }
        else if((currentSolverStatus & SOLVER_STATUS_USES_FORMULA) && (currentSolverStatus & SOLVER_STATUS_INTERACTIVE) && dynamicMenuItem == 3) {
          item = ITM_CPXSLV;
        }
        else if((currentSolverStatus & SOLVER_STATUS_USES_FORMULA) && (currentSolverStatus & SOLVER_STATUS_INTERACTIVE) && *getNthString(dynamicSoftmenu[softmenuStack[0].softmenuId].menuContent, dynamicMenuItem) == 0) {
          item = ITM_NOP;
        }
        else {
          item = (dynamicMenuItem >= dynamicSoftmenu[menuId].numItems ? ITM_NOP : ITM_SOLVE_VAR);
        }
        break;

      case MNU_MATRS:
      case MNU_STRINGS:
      case MNU_DATES:
      case MNU_TIMES:
      case MNU_ANGLES:
      case MNU_SINTS:
      case MNU_LINTS:
      case MNU_REALS:
      case MNU_CPXS:
        dynamicMenuItem = firstItem + itemShift + (fn - 1);
        item = (dynamicMenuItem >= dynamicSoftmenu[menuId].numItems ? ITM_NOP : ITM_RCL);
        break;

      case MNU_RAM:
      //case MNU_FLASH:
        dynamicMenuItem = firstItem + itemShift + (fn - 1);
        item = (dynamicMenuItem >= dynamicSoftmenu[menuId].numItems ? ITM_NOP : ITM_XEQ);
        break;

      case MNU_MENUS:
        dynamicMenuItem = firstItem + itemShift + (fn - 1);
        item = ITM_NOP;
        if(dynamicMenuItem < dynamicSoftmenu[menuId].numItems) {
          for(uint32_t i = 0; softmenu[i].menuItem < 0; ++i) {
            if(compareString((char *)getNthString(dynamicSoftmenu[menuId].menuContent, dynamicMenuItem), indexOfItems[-softmenu[i].menuItem].itemCatalogName, CMP_NAME) == 0) {
              item = softmenu[i].menuItem;
            }
          }
          for(uint32_t i = 0; i < numberOfUserMenus; ++i) {
            if(compareString((char *)getNthString(dynamicSoftmenu[menuId].menuContent, dynamicMenuItem), userMenus[i].menuName, CMP_NAME) == 0) {
              item = -MNU_DYNAMIC;
              currentUserMenu = i;
            }
          }
        }
        break;

      case ITM_MENU:
        dynamicMenuItem = firstItem + itemShift + (fn - 1);
        item = ITM_MENU;
        break;

      case MNU_EQN:
        if(numberOfFormulae == 0 && (firstItem + itemShift + (fn - 1)) > 0) {
          break;
        }
        /* fallthrough */

      default:
        sm = &softmenu[menuId];
        row = min(3, (sm->numItems + modulo(firstItem - sm->numItems, 6))/6 - firstItem/6) - 1;
        if(itemShift/6 <= row && firstItem + itemShift + (fn - 1) < sm->numItems) {
          item = (sm->softkeyItem)[firstItem + itemShift + (fn - 1)] % 10000;

          if(item == ITM_PROD_SIGN) {
            item = (getSystemFlag(FLAG_MULTx) ? ITM_DOT : ITM_CROSS);
          }
        }
    }

    if(calcMode == CM_ASSIGN && item != ITM_NOP && item != ITM_NULL) {
      switch(-softmenu[menuId].menuItem) {
        case MNU_PROG:
        case MNU_RAM:
        case MNU_FLASH:
          return findNamedLabel((char *)getNthString(dynamicSoftmenu[menuId].menuContent, dynamicMenuItem)) - FIRST_LABEL + ASSIGN_LABELS;
        case MNU_VAR:
        case MNU_MATRS:
        case MNU_STRINGS:
        case MNU_DATES:
        case MNU_TIMES:
        case MNU_ANGLES:
        case MNU_SINTS:
        case MNU_LINTS:
        case MNU_REALS:
        case MNU_CPXS:
          return findNamedVariable((char *)getNthString(dynamicSoftmenu[menuId].menuContent, dynamicMenuItem)) - FIRST_NAMED_VARIABLE + ASSIGN_NAMED_VARIABLES;
        case MNU_MENUS:
          if(item == -MNU_DYNAMIC) {
            for(int32_t i = 0; i < numberOfUserMenus; ++i) {
              if(compareString((char *)getNthString(dynamicSoftmenu[menuId].menuContent, dynamicMenuItem), userMenus[i].menuName, CMP_NAME) == 0) {
                return ASSIGN_USER_MENU - i;
              }
            }
            displayBugScreen("In function determineFunctionKeyItem: nonexistent menu specified!");
            return item;
          }
          else {
            return item;
          }
        default:
          return item;
      }
    }
    else {
      return item;
    }
  }



  #ifdef PC_BUILD
    void btnFnClicked(GtkWidget *notUsed, gpointer data) {
//      GdkEvent mouseButton; //JM
//      mouseButton.button.button = 1; //JM
      executeFunction(data, 0);
    }
  #endif
    
  #ifdef DMCP_BUILD
    void btnFnClicked(void *unused, void *data) {
//      btnFnPressed(data); //JM
//      btnFnReleased(data); //JM
      executeFunction(data, 0);
    }
  #endif // DMCP_BUILD



  #ifdef PC_BUILD
    void btnFnClickedP(GtkWidget *notUsed, gpointer data) { //JM Added this portion to be able to go to NOP on emulator
      GdkEvent mouseButton;
      mouseButton.button.button = 1;
      mouseButton.type = 0;
      btnFnPressed(notUsed, &mouseButton, data);
    }

    void btnFnClickedR(GtkWidget *notUsed, gpointer data) { //JM Added this portion to be able to go to NOP on emulator
      GdkEvent mouseButton;
      mouseButton.button.button = 1;
      mouseButton.type = 0;
      btnFnReleased(notUsed, &mouseButton, data);
    }
  #endif // PC_BUILD


    void execAutoRepeat(uint16_t key) {
#ifdef DMCP_BUILD
      char charKey[6];
      sprintf(charKey, "%02d", key -1);

      fnTimerStart(TO_AUTO_REPEAT, key, KEY_AUTOREPEAT_PERIOD);

      btnClicked(NULL, (char *)charKey);
//    btnPressed(charKey);
      refreshLcd();
      lcd_refresh_dma();
#endif
    }


bool_t lastshiftF = false;
bool_t lastshiftG = false;

  #ifdef PC_BUILD
    void btnFnPressed(GtkWidget *notUsed, GdkEvent *event, gpointer data) {
      if(event->type == GDK_DOUBLE_BUTTON_PRESS || event->type == GDK_TRIPLE_BUTTON_PRESS) { // return unprocessed for double or triple click
        return;
      }
      if(event->button.button == 2) { // Middle click
        shiftF = true;
        shiftG = false;
      }
      if(event->button.button == 3) { // Right click
        shiftF = false;
        shiftG = true;
      }
  #endif // PC_BUILD
  #ifdef DMCP_BUILD
    void btnFnPressed(void *data) {
  #endif // DMCP_BUILD
      if(programRunStop == PGM_RUNNING || programRunStop == PGM_PAUSED) {
        setLastKeyCode((*((char *)data) - '0') + 37);
      }
      else {
        lastKeyCode = 0;
      }

      if(programRunStop == PGM_PAUSED) {
        programRunStop = PGM_KEY_PRESSED_WHILE_PAUSED;
        return;
      }

      lastshiftF = shiftF;
      lastshiftG = shiftG;

      if(calcMode == CM_ASSIGN && itemToBeAssigned != 0) {
        int16_t item = determineFunctionKeyItem_C43((char *)data);

        switch(-softmenu[softmenuStack[0].softmenuId].menuItem) {
          case MNU_MENUS:
            if(item <= ASSIGN_USER_MENU) {
              currentUserMenu = ASSIGN_USER_MENU - item;
              item = -MNU_DYNAMIC;
            }
            /* fallthrough */
          case MNU_CATALOG:
          case MNU_CHARS:
          case MNU_PROGS:
          case MNU_VARS:
            #if (FN_KEY_TIMEOUT_TO_NOP == 1)
              showFunctionName(item, 1000); // 1000ms = 1s
            #else // (FN_KEY_TIMEOUT_TO_NOP == 0)
              showFunctionNameItem = item;
            #endif // (FN_KEY_TIMEOUT_TO_NOP == 1)
            break;
          default:
            updateAssignTamBuffer();
        }
      }
      else if(calcMode != CM_REGISTER_BROWSER && calcMode != CM_FLAG_BROWSER && calcMode != CM_FONT_BROWSER) {
        int16_t item = determineFunctionKeyItem_C43((char *)data);

    //    resetShiftState();                                 //JM still needs the shifts active prior to cancelling them
        if(item != ITM_NOP /*&& item != ITM_NULL*/) {        //JM still need to run the longpress even if no function populated in FN, ie NOP or NULL
          lastErrorCode = 0;

          if(calcMode != CM_ASSIGN   &&   indexOfItems[item].func == addItemToBuffer

             && !( (item>=ITM_0 && item <=ITM_9) || item == ITM_EXPONENT || -softmenu[softmenuStack[0].softmenuId].menuItem == MNU_EQ_EDIT ) 

            ) { //JM added conditions, toherwise digits in menus do not work
            
            // If we are in the catalog then a normal key press should affect the Alpha Selection Buffer to choose
            // an item from the catalog, but a function key press should put the item in the AIM (or TAM) buffer
            // Use this variable to distinguish between the two
            fnKeyInCatalog = 1;
            resetShiftState();   //JM moved down here from above

            if(calcMode == CM_PEM && !tam.mode) {
              if(getSystemFlag(FLAG_ALPHA)) {
                pemAlpha(item);
              }
              else {
                pemAddNumber(item);
              }
              hourGlassIconEnabled = false;
            }
            else {
              fnKeyInCatalog = 1;
              addItemToBuffer(item);
              fnKeyInCatalog = 0;
            }
            if(calcMode == CM_EIM) {
              while(softmenu[softmenuStack[0].softmenuId].menuItem != -MNU_EQ_EDIT) {
                popSoftmenu();
              }
            }
            refreshScreen();
          }

          else {
/*
    //    #if(FN_KEY_TIMEOUT_TO_NOP == 1)                    //JM vv Rmove the possibility for error by removing code that may conflict with the state machine
    //      showFunctionName(item, 1000); // 1000ms = 1s
    //    #else // (FN_KEY_TIMEOUT_TO_NOP == 0)
    //    showFunctionNameItem = item;
*/
            btnFnPressed_StateMachine(NULL, data);        //JM ^^ This calls original state analysing btnFnPressed routing, which is now renamed to "statemachine" in keyboardtweaks
/*
    //    #endif // (FN_KEY_TIMEOUT_TO_NOP == 1)
*/
          }
        }
        else {
          showFunctionNameItem = ITM_NOP;
        }
      }
    }



  #ifdef PC_BUILD
    void btnFnReleased(GtkWidget *notUsed, GdkEvent *event, gpointer data) {
  #endif // PC_BUILD
  #ifdef DMCP_BUILD
    void btnFnReleased(void *data) {
  #endif // DMCP_BUILD
    if(programRunStop == PGM_KEY_PRESSED_WHILE_PAUSED) {
      programRunStop = PGM_RESUMING;
      return;
    }
    if(calcMode != CM_REGISTER_BROWSER && calcMode != CM_FLAG_BROWSER && calcMode != CM_FONT_BROWSER) {
      if(calcMode == CM_ASSIGN && itemToBeAssigned != 0) {
        switch(-softmenu[softmenuStack[0].softmenuId].menuItem) {
          case MNU_MyMenu:
            assignToMyMenu((*((uint8_t *)data) - '1') + (shiftG ? 12 : shiftF ? 6 : 0));
            calcMode = previousCalcMode;
            shiftF = shiftG = false;
            refreshScreen();
            return;
          case MNU_MyAlpha:
            assignToMyAlpha((*((uint8_t *)data) - '1') + (shiftG ? 12 : shiftF ? 6 : 0));
            calcMode = previousCalcMode;
            shiftF = shiftG = false;
            refreshScreen();
            return;
          case MNU_DYNAMIC:
            if(itemToBeAssigned < 0) {
              displayCalcErrorMessage(ERROR_CANNOT_ASSIGN_HERE, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
              #ifdef PC_BUILD
                moreInfoOnError("In function btnFnReleased:", "cannot assign submenu", indexOfItems[-itemToBeAssigned].itemCatalogName, "in user-created menu.");
              #endif
            }
            else {
              assignToUserMenu((*((uint8_t *)data) - '1') + (shiftG ? 12 : shiftF ? 6 : 0));
            }
            calcMode = previousCalcMode;
            shiftF = shiftG = false;
            refreshScreen();
            return;
          case MNU_CATALOG:
          case MNU_CHARS:
          case MNU_PROGS:
          case MNU_VARS:
          case MNU_MENUS:
            break;
          default:
            displayCalcErrorMessage(ERROR_CANNOT_ASSIGN_HERE, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
            #ifdef PC_BUILD
              moreInfoOnError("In function btnFnReleased:", "the menu", indexOfItems[-softmenu[softmenuStack[0].softmenuId].menuItem].itemCatalogName, "is write-protected.");
            #endif
            calcMode = previousCalcMode;
            shiftF = shiftG = false;
            refreshScreen();
            return;
        }
      }


      btnFnReleased_StateMachine(NULL, data);            //This function does the longpress differentiation, and calls ExecuteFunctio below, via fnbtnclicked
    }

    fnTimerStop(TO_3S_CTFF);      //dr
    fnTimerStop(TO_CL_LONG);      //dr
  }

  /********************************************//**
   * \brief Executes one function from a softmenu
   * \return void
   ***********************************************/
  void executeFunction(const char *data, int16_t item_) {
    int16_t item = ITM_NOP;
    if(calcMode != CM_REGISTER_BROWSER && calcMode != CM_FLAG_BROWSER && calcMode != CM_FONT_BROWSER) {
  
      if(data[0] == 0) { item = item_; }
      else             { item = determineFunctionKeyItem_C43((char *)data); }

      resetShiftState();                               //shift cancelling delayed to this point after state machine


//TOCHECK: JM Changed showFunctionNameItem to item below, due to something 43S did to the showfunction sequencing
      if(/*showFunctionNameItem*/item != 0 || softmenuStack[0].softmenuId == 0) {  //JM added C43 condition, for FN keys operating on no menu present
/* //JM vv Rmove the possibility for error by removing code that may conflict with the state machine
        item = showFunctionNameItem;
      #if (FN_KEY_TIMEOUT_TO_NOP == 1)
        hideFunctionName();
      #else
*/
        showFunctionNameItem = 0;
/*
      #endif // (FN_KEY_TIMEOUT_TO_NOP == 1)
*/
        //printf("%d--\n",calcMode);
        
        if(calcMode != CM_CONFIRMATION && data[0] != 0 && !running_program_jm) { //JM data is used if operation is from the real keyboard. item is used directly if called from XEQM
          lastErrorCode = 0;

          if(item < 0) { // softmenu
            if(calcMode == CM_ASSIGN && itemToBeAssigned == 0 && softmenu[softmenuStack[0].softmenuId].menuItem == -MNU_MENUS) {
              itemToBeAssigned = item;
            }
            else {
              showSoftmenu(item);
              if(item == -MNU_Solver && lastErrorCode != 0) {
                popSoftmenu();
                currentSolverStatus &= ~SOLVER_STATUS_INTERACTIVE;
              }
            }
            refreshScreen();
            return;
          }
          if(tam.mode && catalog && (tam.digitsSoFar || tam.function == ITM_BESTF || tam.function == ITM_CNST || (!tam.indirect && (tam.mode == TM_VALUE || tam.mode == TM_VALUE_CHB || (tam.mode == TM_KEY && !tam.keyInputFinished))))) {
            // disabled
          }
          else if(tam.function == ITM_GTOP && catalog == CATALOG_PROG) {
            runFunction(item);
            tamLeaveMode();
            hourGlassIconEnabled = false;
            refreshScreen();
            return;
          }
          else if(calcMode == CM_PEM && catalog && catalog != CATALOG_MVAR) { // TODO: is that correct
            if(indexOfItems[item].func == fnGetSystemFlag && (tam.mode == TM_FLAGR || tam.mode == TM_FLAGW) && !tam.indirect) {
              tam.value = (indexOfItems[item].param & 0xff);
              tam.alpha = true;
              addStepInProgram(tamOperation());
              tamLeaveMode();
              hourGlassIconEnabled = false;
            }
            else if(tam.mode) {
              const char *itmLabel = dynmenuGetLabel(dynamicMenuItem);
              uint16_t nameLength = stringByteLength(itmLabel);
              xcopy(aimBuffer, itmLabel, nameLength + 1);
              tam.alpha = true;
              addStepInProgram(tamOperation());
              tamLeaveMode();
              hourGlassIconEnabled = false;
            }
            else {
              runFunction(item);
            }
            refreshScreen();
            return;
          }

          // If we are in the catalog then a normal key press should affect the Alpha Selection Buffer to choose
          // an item from the catalog, but a function key press should put the item in the AIM (or TAM) buffer
          // Use this variable to distinguish between the two
          fnKeyInCatalog = 1;
          if(tam.mode && catalog && (tam.digitsSoFar || tam.function == ITM_BESTF || tam.function == ITM_CNST || (!tam.indirect && (tam.mode == TM_VALUE || tam.mode == TM_VALUE_CHB)))) {
            // disabled
          }
          else if(tam.mode && (!tam.alpha || isAlphabeticSoftmenu())) {
            addItemToBuffer(item);
          }
  //          else if((calcMode == CM_NORMAL || calcMode == CM_AIM) && isAlphabeticSoftmenu()) {
  //            if(calcMode == CM_NORMAL) {
  //              fnAim(NOPARAM);
  //            }
  //            addItemToBuffer(item);  //DIS HIERDIE EEN WAT DIE MENU LAAT TEXT IPV COMMANDS UITGOOI. 
  //KYK HIER. TOFIX
  //CLASH WITH ARROWS !!
  //          }
          else if(calcMode == CM_EIM && catalog && catalog != CATALOG_MVAR) {
            addItemToBuffer(item);
            while(softmenu[softmenuStack[0].softmenuId].menuItem != -MNU_EQ_EDIT) {
              popSoftmenu();
            }
          }
          else if((calcMode == CM_NORMAL || calcMode == CM_NIM) && (ITM_0<=item && item<=ITM_F) && (!catalog || catalog == CATALOG_MVAR)) {
            addItemToNimBuffer(item);
          }
          else if((calcMode == CM_NIM) && ((item==ITM_DRG || item == ITM_DMS2 || item == ITM_dotD) && !catalog)) {   //JM
            addItemToNimBuffer(item);
          }                                                                                      //JM
          else if(calcMode == CM_MIM && softmenu[softmenuStack[0].softmenuId].menuItem != -MNU_M_EDIT && item != ITM_CC) { //JM added ITM_CC to let it work in matrix edit
            addItemToBuffer(item);
          }
          else if(item > 0) { // function
            if(calcMode == CM_NIM && item != ITM_CC && item!=ITM_HASH_JM && item!=ITM_toHMS && item!=ITM_ms) {  //JMNIM Allow NIM not closed, so that JMNIM can change the bases without ierrors thrown 
              closeNim();
              if(calcMode != CM_NIM) {
                if(indexOfItems[item].func == fnConstant) {
                  setSystemFlag(FLAG_ASLIFT);
                }
              }
            }
            if(calcMode == CM_AIM && !isAlphabeticSoftmenu()) {
              closeAim();
            }
            if(tam.alpha) {
              tamLeaveMode();
            }

            if(lastErrorCode == 0) {
              if(temporaryInformation == TI_VIEW) {
                temporaryInformation = TI_NO_INFO;
                updateMatrixHeightCache();
              }
              else {
                temporaryInformation = TI_NO_INFO;
              }
              if(programRunStop == PGM_WAITING) {
                programRunStop = PGM_STOPPED;
              }
              if(calcMode == CM_ASSIGN && itemToBeAssigned == 0 && item != ITM_NOP) {
                itemToBeAssigned = item;
              }
              else {
                runFunction(item);
              }
            }
          }
          fnKeyInCatalog = 0;
        }
      }
    }
  #ifdef PC_BUILD
    printf(">>>  refreshScreen3 from keyboard.c executeFunction\n");
  #endif
    refreshScreen();
  }


bool_t allowShiftsToClearError = false;
#define stringToKeyNumber(data)         ((*((char *)data) - '0')*10 + *(((char *)data)+1) - '0')

  int16_t determineItem(const char *data) {
    int16_t result;
    const calcKey_t *key;

    dynamicMenuItem = -1;
//.    key = getSystemFlag(FLAG_USER) ? (kbd_usr + (*data - '0')*10 + *(data+1) - '0') : (kbd_std + (*data - '0')*10 + *(data+1) - '0');  //Latest one, remove and replace below
//.    key = getSystemFlag(FLAG_USER) && ((calcMode == CM_NORMAL) || (calcMode == CM_NIM)) ? (kbd_usr + stringToKeyNumber(data)) : (kbd_std + stringToKeyNumber(data));    //JM Added (calcMode == CM_NORMAL) to prevent user substitution in AIM and TAM

    int8_t key_no = stringToKeyNumber(data);

  #ifdef PC_BUILD
    char tmp[200]; sprintf(tmp,"^^^^^^^keyboard.c: determineitem: key_no: %d:",key_no); jm_show_comment(tmp);
  #endif //PC_BUILD

//.    if(kbd_usr[36].primaryTam == ITM_EXIT1) //opposite keyboard V43 LT, 43S, V43 RT
      key = getSystemFlag(FLAG_USER) ? (kbd_usr + key_no) : (kbd_std + key_no);
//.    else
//.      key = getSystemFlag(FLAG_USER) && ((calcMode == CM_NORMAL) || (calcMode == CM_AIM) || (calcMode == CM_NIM) || (calcMode == CM_EIM) || (calcMode == CM_PLOT_STAT) || (calcMode == CM_GRAPH) || (calcMode == CM_LISTXY)) ? (kbd_usr + key_no) : (kbd_std + key_no);    //JM Added (calcMode == CM_NORMAL) to prevent user substitution in AIM and TAM

    fnTimerExec(TO_FN_EXEC);                                  //dr execute queued fn

  #ifdef PC_BUILD
    sprintf(tmp,"^^^^^^^keyboard.c: determineitem: key->primary1: %d:",key->primary); jm_show_comment(tmp);
  #endif //PC_BUILD

    switch(key->primary) {                              //JMSHOW vv
      case      ITM_UP1:
      case      ITM_DOWN1: break;                       //JM SHOWregis unchanged
      default:  SHOWregis = 9999; break;     
    }                                                   //JMSHOW ^^
    //printf("###\n"); //JMEXEC

    Setup_MultiPresses( key->primary );

  #ifdef PC_BUILD
    sprintf(tmp,"^^^^^^^keyboard.c: determineitem: key->primary2: %d:",key->primary); jm_show_comment(tmp);
  #endif //PC_BUILD

    // Shift f pressed and JM REMOVED shift g not active
    if(key->primary == ITM_SHIFTf && (calcMode == CM_NORMAL || calcMode == CM_AIM || calcMode == CM_NIM  || calcMode == CM_MIM || calcMode == CM_EIM || calcMode == CM_PEM || calcMode == CM_PLOT_STAT || calcMode == CM_GRAPH || calcMode == CM_ASSIGN)) {    //JM Mode added
      if(temporaryInformation == TI_SHOW_REGISTER || temporaryInformation == TI_SHOW_REGISTER_BIG || temporaryInformation == TI_SHOW_REGISTER_SMALL) allowShiftsToClearError = true; //JM
      if(temporaryInformation == TI_VIEW) {
        temporaryInformation = TI_NO_INFO;
        updateMatrixHeightCache();
      }
      else {
        temporaryInformation = TI_NO_INFO;
      }
      if(lastErrorCode != 0) allowShiftsToClearError = true;                                                                                         //JM shifts
      if(programRunStop == PGM_WAITING) {
        programRunStop = PGM_STOPPED;
      }
      lastErrorCode = 0;

      fnTimerStop(TO_FG_LONG);                                //dr
      fnTimerStop(TO_FG_TIMR);                                //dr
  
      shiftF = !shiftF;
      shiftG = false;                                         //JM no shifted menu on g-shift-key as in WP43S
      lastshiftF = shiftF;
      lastshiftG = shiftG;
      showShiftState();
      #ifdef PC_BUILD
        if(calcMode == CM_AIM || calcMode == CM_EIM) refreshModeGui();
      #endif

      return ITM_NOP;
    }

    // Shift g pressed and JM REMOVED shift f not active
    else if(key->primary == ITM_SHIFTg && (calcMode == CM_NORMAL || calcMode == CM_AIM || calcMode == CM_NIM || calcMode == CM_MIM || calcMode == CM_EIM || calcMode == CM_PEM || calcMode == CM_PLOT_STAT || calcMode == CM_GRAPH || calcMode == CM_ASSIGN)) {
      if(temporaryInformation == TI_SHOW_REGISTER || temporaryInformation == TI_SHOW_REGISTER_BIG || temporaryInformation == TI_SHOW_REGISTER_SMALL) allowShiftsToClearError = true; //JM
      if(temporaryInformation == TI_VIEW) {
        temporaryInformation = TI_NO_INFO;
        updateMatrixHeightCache();
      }
      else {
        temporaryInformation = TI_NO_INFO;
      }
      if(lastErrorCode != 0) allowShiftsToClearError = true;                                                                                         //JM shifts
      if(programRunStop == PGM_WAITING) {
        programRunStop = PGM_STOPPED;
      }
      lastErrorCode = 0;

      fnTimerStop(TO_FG_LONG);                                //dr
      fnTimerStop(TO_FG_TIMR);                                //dr

      shiftG = !shiftG;
      shiftF = false;                                         //JM no shifted menu on g-shift-key as in WP43S
      lastshiftF = shiftF;
      lastshiftG = shiftG;
      showShiftState();
      #ifdef PC_BUILD
        if(calcMode == CM_AIM || calcMode == CM_EIM) refreshModeGui();
      #endif

      return ITM_NOP;
    }

    // JM Shift f pressed  //JM shifts change f/g to a single function key toggle to match DM42 keyboard
    // JM Inserted new section and removed old f and g key processing sections
    else if(key->primary == KEY_fg && (calcMode == CM_NORMAL || calcMode == CM_AIM || calcMode == CM_NIM  || calcMode == CM_MIM || calcMode == CM_EIM || calcMode == CM_PEM || (calcMode == CM_PLOT_STAT) || calcMode == CM_GRAPH || calcMode == CM_ASSIGN)) {   //JM shifts
      Shft_timeouts = true;                         //JM SHIFT NEW
      fnTimerStart(TO_FG_LONG, TO_FG_LONG, JM_TO_FG_LONG);    //vv dr
      if(ShiftTimoutMode) {
        fnTimerStart(TO_FG_TIMR, TO_FG_TIMR, JM_SHIFT_TIMER); //^^
      }
      if(temporaryInformation == TI_VIEW || temporaryInformation == TI_SHOW_REGISTER || temporaryInformation == TI_SHOW_REGISTER_BIG || temporaryInformation == TI_SHOW_REGISTER_SMALL) allowShiftsToClearError = true; //JM
      if(temporaryInformation == TI_VIEW) {
        temporaryInformation = TI_NO_INFO;
        updateMatrixHeightCache();
      }
      else {
        temporaryInformation = TI_NO_INFO;
      }
      if(lastErrorCode != 0) allowShiftsToClearError = true;                                                                                         //JM shifts
      lastErrorCode = 0;                                                                                                      //JM shifts

      fg_processing_jm();

      lastshiftF = shiftF;
      lastshiftG = shiftG;
      showShiftState();                                                                                                         //JM shifts
      #ifdef PC_BUILD
        if(calcMode == CM_AIM || calcMode == CM_EIM) refreshModeGui();
      #endif

      return ITM_NOP;
    }  

  #ifdef PC_BUILD
    sprintf(tmp,"^^^^^^^keyboard.c: determineitem: key->primary3: %d:",key->primary); jm_show_comment(tmp);
  #endif //PC_BUILD

                                                                                                                         //JM shifts
    if((calcMode == CM_NIM || calcMode == CM_NORMAL) && lastIntegerBase >= 11 && (key_no >= 0 && key_no <= 5 )) {               //JMNIM vv Added direct A-F for hex entry
      result = shiftF ? key->fShifted :
               shiftG ? key->gShifted :
                        key->primaryAim;
      //printf(">>> ???????????? keys key:%d result:%d Calmode:%d, nimbuffer:%s, lastbase:%d, nimnumberpart:%d\n",key_no, result, calcMode,nimBuffer,lastIntegerBase, nimNumberPart);
      return result;  
    }
    else                                                                                                                        //JM^^

    if(calcMode == CM_AIM || (catalog && catalog != CATALOG_MVAR && calcMode != CM_NIM) || calcMode == CM_EIM || tam.alpha || (calcMode == CM_ASSIGN && (previousCalcMode == CM_AIM || previousCalcMode == CM_EIM))) {
      result = shiftF ? key->fShiftedAim :
               shiftG ? key->gShiftedAim :
                        key->primaryAim;

    }
    else if(tam.mode) {
      result = key->primaryTam; // No shifted function in TAM
    }
    else if(calcMode == CM_NORMAL || calcMode == CM_NIM || calcMode == CM_MIM || calcMode == CM_FONT_BROWSER || calcMode == CM_FLAG_BROWSER || calcMode == CM_REGISTER_BROWSER || calcMode == CM_BUG_ON_SCREEN || calcMode == CM_CONFIRMATION || calcMode == CM_PEM || calcMode == CM_PLOT_STAT || calcMode == CM_ASSIGN || calcMode == CM_TIMER || calcMode == CM_GRAPH || calcMode == CM_LISTXY) {
      result = shiftF ? key->fShifted :
               shiftG ? key->gShifted :
                        key->primary;
    }
    else {
      displayBugScreen("In function determineItem: item was not determined!");
      result = 0;
    }

  #ifdef PC_BUILD
    sprintf(tmp,"^^^^^^^keyboard.c: determineitem: result1: %d:",result); jm_show_comment(tmp);
  #endif //PC_BUILD

    Check_Assign_in_progress(&result, key_no);  //JM

  #ifdef PC_BUILD
    sprintf(tmp,"^^^^^^^keyboard.c: determineitem: result2: %d:",result); jm_show_comment(tmp);
  #endif //PC_BUILD

    Check_MultiPresses(&result, key_no);        //JM

  #ifdef PC_BUILD
    sprintf(tmp,"^^^^^^^keyboard.c: determineitem: result3: %d:",result); jm_show_comment(tmp);
  #endif //PC_BUILD

    if(result == ITM_PROD_SIGN) {
      result = (getSystemFlag(FLAG_MULTx) ? ITM_CROSS : ITM_DOT);
    }

    resetShiftState();

    return result;
  }



  #ifdef PC_BUILD
    void btnClicked(GtkWidget *notUsed, gpointer data) {
      GdkEvent mouseButton;
      mouseButton.button.button = 1;
      mouseButton.type = 0;

      btnPressed(notUsed, &mouseButton, data);
      btnReleased(notUsed, &mouseButton, data);
  }
  #endif // PC_BUILD

  #ifdef DMCP_BUILD
    void btnClicked(void *unused, void *data) {
      btnPressed(data);
      btnReleased(data);
    }
  #endif // DMCP_BUILD



  #ifdef PC_BUILD
  void btnClickedP(GtkWidget *w, gpointer data) {                          //JM PRESSED FOR KEYBOARD F REPEAT
    GdkEvent mouseButton;
    mouseButton.button.button = 1;
    mouseButton.type = 0;
    btnPressed(w, &mouseButton, data);
  }

  void btnClickedR(GtkWidget *w, gpointer data) {                          //JM PRESSED FOR KEYBOARD F REPEAT
    GdkEvent mouseButton;
    mouseButton.button.button = 1;
    btnReleased(w, &mouseButton, data);
  }
  #endif


  bool_t checkShifts(const char *data) {
    const calcKey_t *key;

    int8_t key_no = stringToKeyNumber(data);

    key = getSystemFlag(FLAG_USER) && ((calcMode == CM_NORMAL) || (calcMode == CM_AIM) || (calcMode == CM_EIM) || (calcMode == CM_NIM)) ? (kbd_usr + key_no) : (kbd_std + key_no);

    if(key->primary == ITM_SHIFTf || key->primary == ITM_SHIFTg || key->primary == KEY_fg) {
      return true;
    }
    else {
      return false;
    }
  }


bool_t nimWhenButtonPressed = false;                  //PHM eRPN 2021-07

  /********************************************//**
   * \brief A calc button was pressed
   *
   * \param notUsed GtkWidget*
   * \param data gpointer pointer to a string containing the key number pressed: 00=1/x, ..., 36=EXIT
   * \return void
   ***********************************************/




  #ifdef PC_BUILD
    void btnPressed(GtkWidget *notUsed, GdkEvent *event, gpointer data) {
      nimWhenButtonPressed = (calcMode == CM_NIM);                  //PHM eRPN 2021-07

     int keyCode = (*((char *)data) - '0')*10 + *(((char *)data) + 1) - '0';
      if(programRunStop == PGM_RUNNING || programRunStop == PGM_PAUSED) {
        setLastKeyCode(keyCode + 1);
      }
      else {
        lastKeyCode = 0;
      }

      if(event->type == GDK_DOUBLE_BUTTON_PRESS || event->type == GDK_TRIPLE_BUTTON_PRESS) { // return unprocessed for double or triple click
        return;
      }
      if(event->button.button == 2) { // Middle click
        shiftF = true;
        shiftG = false;
      }
      if(event->button.button == 3) { // Right click
        shiftF = false;
        shiftG = true;
      }
      lastshiftF = shiftF;
      lastshiftG = shiftG;
      int16_t item = determineItem((char *)data);
      if(programRunStop == PGM_RUNNING || programRunStop == PGM_PAUSED) {
        if((item == ITM_RS || item == ITM_EXIT1) && !getSystemFlag(FLAG_INTING) && !getSystemFlag(FLAG_SOLVING)) {
          programRunStop = PGM_WAITING;
          showFunctionNameItem = 0;
        }
        else if(programRunStop == PGM_PAUSED) {
          programRunStop = PGM_KEY_PRESSED_WHILE_PAUSED;
        }
        return;
      }

      if(getSystemFlag(FLAG_USER)) {
        int keyStateCode = (getSystemFlag(FLAG_ALPHA) ? 3 : 0) + (shiftG ? 2 : shiftF ? 1 : 0);
        char *funcParam = (char *)getNthString((uint8_t *)userKeyLabel, keyCode * 6 + keyStateCode);
        xcopy(tmpString, funcParam, stringByteLength(funcParam) + 1);
      }
      else {
        *tmpString = 0;
      }

      showFunctionNameItem = 0;
      #ifdef PC_BUILD
        char tmp[200]; sprintf(tmp,"^^^^btnPressed START item=%d data=\'%s\'",item,(char *)data); jm_show_comment(tmp);
      #endif //PC_BUILD

      if(item != ITM_NOP && item != ITM_NULL) {
        #ifdef PC_BUILD
          sprintf(tmp,"keyboard.c: btnPressed 1--> processKeyAction(%d) which is str:%s\n",item,(char *)data); jm_show_calc_state(tmp);
        #endif
        processKeyAction(item);
        #ifdef PC_BUILD
          sprintf(tmp,"keyboard.c: btnPressed 2--> processKeyAction(%d) which is str:%s\n",item,(char *)data); jm_show_calc_state(tmp);
        #endif
        if(!keyActionProcessed) {
          showFunctionName(item, 1000); // 1000ms = 1s
        }
      }
      #ifdef PC_BUILD
        sprintf(tmp,"^^^^btnPressed End item=%d:\'%s\' showFunctionNameItem=%d\n",item,(char *)data,showFunctionNameItem); jm_show_comment(tmp);
      #endif //PC_BUILD
    }

    char key[3] = {0, 0, 0};
    static void convertXYToKey(int x, int y) {
      int xMin, xMax, yMin, yMax;
      key[0] = 0;
      key[1] = 0;
      key[2] = 0;

      for(int i=0; i<43; i++) {
        xMin = calcKeyboard[i].x;
        yMin = calcKeyboard[i].y;
        if(i == 10 && currentBezel == 2 && (tam.mode == TM_LABEL || (tam.mode == TM_SOLVE && (tam.function != ITM_SOLVE || calcMode != CM_PEM)) || (tam.mode == TM_KEY && tam.keyInputFinished))) {
          xMax = xMin + calcKeyboard[10].width[3];
          yMax = yMin + calcKeyboard[10].height[3];
        }
        else {
          xMax = xMin + calcKeyboard[i].width[currentBezel];
          yMax = yMin + calcKeyboard[i].height[currentBezel];
        }

        if(   xMin <= x && x <= xMax
           && yMin <= y && y <= yMax) {
          if(i < 6) { // Function key
            key[0] = '1' + i;
          }
          else {
            key[0] = '0' + (i - 6)/10;
            key[1] = '0' + (i - 6)%10;
          }
          break;
        }
      }

      //printf("key = <%s>\n", key);
    }

    void frmCalcMouseButtonPressed(GtkWidget *notUsed, GdkEvent *event, gpointer data) {
      if(key[0] == 0) { // The previous click must be released
        convertXYToKey((int)event->button.x, (int)event->button.y);
        if(key[0] == 0) {
          return;
        }

        if(key[1] == 0) { // Soft function key
          btnFnPressed(NULL, event, (gpointer)key);
        }
        else { // Not a soft function key
          btnPressed(NULL, event, (gpointer)key);
        }
      }
    }

    void frmCalcMouseButtonReleased(GtkWidget *notUsed, GdkEvent *event, gpointer data) {
      if(key[0] == 0) {
        return;
      }

      if(key[1] == 0) { // Soft function key
        btnFnReleased(NULL, event, (gpointer)key);
      }
      else { // Not a soft function key
        btnReleased(NULL, event, (gpointer)key);
      }

      key[0] = 0;
    }
  #endif // PC_BUILD


  #ifdef DMCP_BUILD
    void btnPressed(void *data) {
      nimWhenButtonPressed = (calcMode == CM_NIM);                  //PHM eRPN 2021-07
      int16_t item;
      int keyCode = (*((char *)data) - '0')*10 + *(((char *)data) + 1) - '0';

      if(programRunStop == PGM_RUNNING || programRunStop == PGM_PAUSED) {
        lastKeyCode = keyCode;
      }
      else {
        lastKeyCode = 0;
      }

//      if(keyAutoRepeat) {            // AUTOREPEAT
//        //beep(880, 50);
//        item = previousItem;
//      }
//      else {
        lastshiftF = shiftF;
        lastshiftG = shiftG;
        item = determineItem((char *)data);
//      previousItem = item;
//    }

      if(getSystemFlag(FLAG_USER)) {
        int keyStateCode = (getSystemFlag(FLAG_ALPHA) ? 3 : 0) + (shiftG ? 2 : shiftF ? 1 : 0);
        char *funcParam = (char *)getNthString((uint8_t *)userKeyLabel, keyCode * 6 + keyStateCode);
        xcopy(tmpString, funcParam, stringByteLength(funcParam) + 1);
      }
      else {
        *tmpString = 0;
      }

      showFunctionNameItem = 0;
      if(item != ITM_NOP && item != ITM_NULL) {
        processKeyAction(item);
        if(!keyActionProcessed) {
          showFunctionName(item, 1000); // 1000ms = 1s
        }
      }
    }
  #endif // DMCP_BUILD



  #ifdef PC_BUILD
    void btnReleased(GtkWidget *notUsed, GdkEvent *event, gpointer data) {
      jm_show_calc_state("##### keyboard.c: btnReleased begin");
  #endif // PC_BUILD
  #ifdef DMCP_BUILD
    void btnReleased(void *data) {
  #endif // DMCP_BUILD
      int16_t item;
      Shft_timeouts = false;                         //JM SHIFT NEW
      JM_auto_longpress_enabled = 0;                 //JM TIMER CLRCLSTK ON LONGPRESS

      if(programRunStop == PGM_KEY_PRESSED_WHILE_PAUSED) {
        programRunStop = PGM_RESUMING;
        return;
      }

      if(calcMode == CM_ASSIGN && itemToBeAssigned != 0 && tamBuffer[0] == 0) {
        assignToKey((char *)data);
        calcMode = previousCalcMode;
        shiftF = shiftG = false;
        refreshScreen();
      }
      else if(showFunctionNameItem != 0) {
        item = showFunctionNameItem;
        #ifdef PC_BUILD
          char tmp[200]; sprintf(tmp,"^^^^btnReleased %d:\'%s\'",item,(char *)data); jm_show_comment(tmp);
        #endif //PC_BUILD

        if(calcMode == CM_NIM && item == ITM_SQUAREROOTX) closeNim();      //JM moved here, from bufferize see JMCLOSE

        fnTimerStop(TO_3S_CTFF);      //dr

        hideFunctionName();
        if(item < 0) {
          showSoftmenu(item);
        }
        else {
          int keyCode = (*((char *)data) - '0')*10 + *(((char *)data) + 1) - '0';
          int keyStateCode = (getSystemFlag(FLAG_ALPHA) ? 3 : 0) + (shiftG ? 2 : shiftF ? 1 : 0);
          char *funcParam = (char *)getNthString((uint8_t *)userKeyLabel, keyCode * 6 + keyStateCode);

          #ifdef PC_BUILD
            if(item == ITM_RS || item == ITM_XEQ) key[0] = 0;
          #endif // PC_BUILD

          if(item != ITM_NOP && tam.alpha && indexOfItems[item].func != addItemToBuffer) {
            // We are in TAM mode so need to cancel first (equivalent to EXIT)
            tamLeaveMode();
          }
          if(item == ITM_RCL && getSystemFlag(FLAG_USER) && funcParam[0] != 0) {
            calcRegister_t var = findNamedVariable(funcParam);
            if(var != INVALID_VARIABLE) {
              reallyRunFunction(item, var);
            }
            else {
              displayCalcErrorMessage(ERROR_UNDEF_SOURCE_VAR, ERR_REGISTER_LINE, REGISTER_X);
              #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                sprintf(errorMessage, "string '%s' is not a named variable", funcParam);
                moreInfoOnError("In function btnReleased:", errorMessage, NULL, NULL);
              #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
            }
          }
          else if(item == ITM_XEQ && getSystemFlag(FLAG_USER) && funcParam[0] != 0) {
            calcRegister_t label = findNamedLabel(funcParam);
            if(label != INVALID_VARIABLE) {
              reallyRunFunction(item, label);
            }
            else {
              displayCalcErrorMessage(ERROR_LABEL_NOT_FOUND, ERR_REGISTER_LINE, REGISTER_X);
              #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                sprintf(errorMessage, "string '%s' is not a named label", funcParam);
                moreInfoOnError("In function btnReleased:", errorMessage, NULL, NULL);
              #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
            }
          }
          else {
            runFunction(item);
          }
        }
      }
  #ifdef DMCP_BUILD
//      else if(keyAutoRepeat) {         // AUTOREPEAT
//        btnPressed(data);
//      }
  #endif // DMCP_BUILD

      if(allowShiftsToClearError || !checkShifts((char *)data)) {
        #ifdef PC_BUILD
          char tmp[200]; sprintf(tmp,">>> btnReleased (%s):   refreshScreen from keyboard.c  which is the main normal place for it.", (char *)data); jm_show_comment(tmp);
          jm_show_calc_state("      ##### keyboard.c: btnReleased end");
        #endif //PC_BUILD
        refreshScreen(); //JM PROBLEM. THIS MUST BE REMOVED FOR MOST CASES
      }
      allowShiftsToClearError = false;

      fnTimerStop(TO_CL_LONG);    //dr
    }




#endif //!TESTSUITE_BUILD
  void leavePem(void) {
    if(freeProgramBytes >= 4) { // Push the programs to the end of RAM
      uint32_t newProgramSize = (uint32_t)((uint8_t *)(ram + RAM_SIZE) - beginOfProgramMemory) - (freeProgramBytes & 0xfffc);
      currentStep        += (freeProgramBytes & 0xfffc);
      firstDisplayedStep += (freeProgramBytes & 0xfffc);
      freeProgramBytes &= 0x03;
      resizeProgramMemory(TO_BLOCKS(newProgramSize));
      scanLabelsAndPrograms();
    }
  }

#ifndef TESTSUITE_BUILD






bool_t lowercaseselected;

  static void processAimInput(int16_t item) {
    int16_t item1 = 0;
#ifdef PC_BUILD
    char tmp[200]; sprintf(tmp,"^^^^processAimInput:AIM %d nextChar=%d",item,nextChar); jm_show_comment(tmp);
#endif //PC_BUILD

    if(keyReplacements(item, &item1, numLock, lastshiftF, lastshiftG) > 0) {  //JMvv
      if(item1 > 0) {
        addItemToBuffer(item1);
        keyActionProcessed = true;
      }
    }
    //JM^^

    /*1*/ else if(lowercaseselected && (ITM_A <= item && item <= ITM_Z)) {
      addItemToBuffer(item + 26);
      keyActionProcessed = true;
    }

    else if(!lowercaseselected && (ITM_A <= item && item <= ITM_Z)) {  //JM
      addItemToBuffer(item);
      keyActionProcessed = true;
    }

    else if(!lowercaseselected && (ITM_a <= item && item <= ITM_z)) {  //JM
      addItemToBuffer(item - 26);
      keyActionProcessed = true;
    }

    else if(lowercaseselected && (ITM_a <= item && item <= ITM_z)) {  //JM
      addItemToBuffer(item);
      keyActionProcessed = true;
    }

    else if(item == ITM_COLON || item == ITM_COMMA || item == ITM_QUESTION_MARK || item == ITM_SPACE || item == ITM_UNDERSCORE) {  //JM vv DIRECT LETTERS
      addItemToBuffer(item);
      keyActionProcessed = true;
    } //JM ^^

    /*2*/ else if(lowercaseselected && ((ITM_ALPHA <= item && item <= ITM_OMEGA) || (ITM_QOPPA <= item && item <= ITM_SAMPI))) {  //JM GREEK
      addItemToBuffer(item); 
      //addItemToBuffer(item + 36); //JM Remove the ability to shift to lower cap greek
      keyActionProcessed = true;
    }

    else if(!lowercaseselected && ((ITM_ALPHA <= item && item <= ITM_OMEGA) || (ITM_QOPPA <= item && item <= ITM_SAMPI))) {  //JM GREEK
      addItemToBuffer(item);
      keyActionProcessed = true;
    }

    /*
    if(alphaCase == AC_LOWER && (ITM_A <= item && item <= ITM_Z)) {
      addItemToBuffer(item + 26);
      keyActionProcessed = true;
    }

    else if(alphaCase == AC_LOWER && (ITM_ALPHA <= item && item <= ITM_OMEGA)) {
      addItemToBuffer(item + 36);
      keyActionProcessed = true;
    }

    else if(item == ITM_DOWN_ARROW) {
      nextChar = NC_SUBSCRIPT;
      keyActionProcessed = true;
    }

    else if(item == ITM_UP_ARROW) {
      nextChar = NC_SUPERSCRIPT;
      keyActionProcessed = true;
    }

    else if(indexOfItems[item].func == addItemToBuffer) {
      addItemToBuffer(item);
      keyActionProcessed = true;
    }

    if(keyActionProcessed) {
      refreshScreen();
    }

*/

    /*3*/ else if(item == ITM_DOWN_ARROW) {
      if(nextChar == NC_NORMAL) nextChar = NC_SUBSCRIPT; else if(nextChar == NC_SUPERSCRIPT) nextChar = NC_NORMAL; //JM stack the SUP/NORMAL/SUB
      keyActionProcessed = true;
    }

    /*4*/ else if(item == ITM_UP_ARROW) {
      if(nextChar == NC_NORMAL) nextChar = NC_SUPERSCRIPT; else if(nextChar == NC_SUBSCRIPT) nextChar = NC_NORMAL; //JM stack the SUP/NORMAL/SUB
      keyActionProcessed = true;
    }

    /*5*/ else if(indexOfItems[item].func == addItemToBuffer) {
      addItemToBuffer(item);
      keyActionProcessed = true;
    }

    /*6*/ if(keyActionProcessed) {
      refreshScreen();
    }

  #ifdef PC_BUILD
    sprintf(tmp,"^^^^processAimInput:AIM:end %d",item); jm_show_comment(tmp);
  #endif //PC_BUILD
  }




  void processKeyAction(int16_t item) {
    keyActionProcessed = false;
    lowercaseselected = ((alphaCase == AC_LOWER && !lastshiftF) || (alphaCase == AC_UPPER && lastshiftF /*&& !numLock*/)); //JM remove last !numlock if you want the shift, during numlock, to produce lower case

    if(lastErrorCode != 0 && item != ITM_EXIT1 && item != ITM_BACKSPACE) {
      lastErrorCode = 0;
    }

    if(temporaryInformation == TI_VIEW) {
      temporaryInformation = TI_NO_INFO;
      updateMatrixHeightCache();
    }
    else {
      temporaryInformation = TI_NO_INFO;
    }
    if(programRunStop == PGM_WAITING) {
      programRunStop = PGM_STOPPED;
    }

    #if (REAL34_WIDTH_TEST == 1)
      longInteger_t lgInt;
      longIntegerInit(lgInt);
    #endif // (REAL34_WIDTH_TEST == 1)

    switch(item) {
      case ITM_BACKSPACE:
//removed altogether. Is it to allow action on release?
//        keyActionProcessed = true;   //JM move this to before fnKeyBackspace to allow fnKeyBackspace to cancel it if needed to allow this function via timing out to NOP, and this is incorporated with the CLRDROP
//        fnKeyBackspace(NOPARAM);
        if(calcMode == CM_NIM || calcMode == CM_AIM || calcMode == CM_EIM) refreshRegisterLine(NIM_REGISTER_LINE);// else //JM No if needed, it does nothing if not in NIM. TO DISPLAY NUMBER KEYPRESS DIRECTLY AFTER PRESS, NOT ONLY UPON RELEASE          break;
        break;

      case ITM_UP1:
        keyActionProcessed = true;   //JM swapped to before fnKeyUp to be able to check if key was processed below. Chose to process it here, as fnKeyUp does not have access to item.
        fnKeyUp(NOPARAM);
        if(!keyActionProcessed) {    //JMvv
          keyActionProcessed = true;
          addItemToBuffer(ITM_UP_ARROW);    //Let the arrows produce arrow up and arrow down in ALPHA mode
        }                            //JM^^
        #if (REAL34_WIDTH_TEST == 1)
          if(++largeur > SCREEN_WIDTH) largeur--;
          uIntToLongInteger(largeur, lgInt);
          convertLongIntegerToLongIntegerRegister(lgInt, REGISTER_Z);
        #endif // (REAL34_WIDTH_TEST == 1)
        break;

      case ITM_DOWN1:
        keyActionProcessed = true;   //swapped to before fnKeyUp to be able to check if key was processed below. Chose to process it here, as fnKeyUp does not have access to item.
        fnKeyDown(NOPARAM);
        if(!keyActionProcessed){     //JM
           keyActionProcessed = true;
           addItemToBuffer(ITM_DOWN_ARROW);    //Let the arrows produce arrow up and arrow down in ALPHA mode
        }                            //JM^^
        #if (REAL34_WIDTH_TEST == 1)
          if(--largeur < 20) largeur++;
          uIntToLongInteger(largeur, lgInt);
          convertLongIntegerToLongIntegerRegister(lgInt, REGISTER_Z);
        #endif // (REAL34_WIDTH_TEST == 1)
        break;


      case ITM_BST:                  //JMvv used for arrows in AIM
        if(calcMode == CM_AIM) {
          keyActionProcessed = true;
          fnT_ARROW(ITM_T_LEFT_ARROW);
        }
        else if(calcMode == CM_GRAPH || calcMode == CM_PLOT_STAT || calcMode == CM_LISTXY) {
          keyActionProcessed = true;
        }
        if(!keyActionProcessed){
          keyActionProcessed = true;
          addItemToBuffer(ITM_BST);
        }
        break;

      case ITM_SST:
        if(calcMode == CM_AIM) {
          keyActionProcessed = true;
          fnT_ARROW(ITM_T_RIGHT_ARROW);
        }
        else if(calcMode == CM_GRAPH  || calcMode == CM_PLOT_STAT || calcMode == CM_LISTXY) {
          keyActionProcessed = true;
        }
        if(!keyActionProcessed){
          keyActionProcessed = true;
          addItemToBuffer(ITM_SST);
        }     
        break;                       //JM^^


      case ITM_EXIT1:
        fnKeyExit(NOPARAM);
        keyActionProcessed = true;
        break;

      case ITM_CC:
      case ITM_dotD:
        if(calcMode == CM_ASSIGN) {
          if(itemToBeAssigned == 0) {
            itemToBeAssigned = item;
          }
          else {
            tamBuffer[0] = 0;
          }
          keyActionProcessed = true;
        }
        else if(calcMode == CM_REGISTER_BROWSER || calcMode == CM_FLAG_BROWSER || calcMode == CM_FONT_BROWSER || calcMode == CM_TIMER) {
          keyActionProcessed = true;
        }
        else if(calcMode == CM_PEM && item == ITM_dotD && aimBuffer[0] == 0) {
          addStepInProgram(ITM_toREAL);
          keyActionProcessed = true;
        }
        break;

      case ITM_ENTER:
        if(calcMode == CM_ASSIGN) {
          if(itemToBeAssigned == 0) {
            itemToBeAssigned = item;
          }
          keyActionProcessed = true;
        }
        else if(calcMode == CM_REGISTER_BROWSER || calcMode == CM_FLAG_BROWSER || calcMode == CM_FONT_BROWSER) {
          keyActionProcessed = true;
        }
        else if(tam.mode) {
          tamProcessInput(ITM_ENTER);
//          if(tamFunction == ITM_toINT && item == ITM_ENTER) {     //JMvv
//            //addItemToBuffer(item);
//            tamTransitionSystem(TT_CHB10);
//          }                                                       //JM^^
          keyActionProcessed = true;
        }
        else if(calcMode == CM_NIM) {                             //JMvv
          addItemToBuffer(item);
          keyActionProcessed = true;
        }                                                         //JM^^
        break;


//TOFIX: Is this still required? vv
      case CHR_numL:   //JMvv
        if(!numLock)  { processKeyAction(CHR_num); } 
        keyActionProcessed = true;
        break;

      case CHR_numU:
        if(numLock)  { processKeyAction(CHR_num); } 
        keyActionProcessed = true;
        break;

      case CHR_num:                           
        alphaCase = AC_UPPER;
        numLock = !numLock;
        if(!numLock) { nextChar = NC_NORMAL;}
        showAlphaModeonGui(); //dr JM, see keyboardtweaks
        keyActionProcessed = true;
        break;

      case CHR_caseUP:
        if(numLock)  { } else
        if(alphaCase == AC_LOWER)  { processKeyAction(CHR_case); } else
        if(alphaCase == AC_UPPER)  { processKeyAction(CHR_numL); }
        nextChar = NC_NORMAL;
        keyActionProcessed = true;
        break;

      case CHR_caseDN:
        if(numLock)  { alphaCase = AC_UPPER; processKeyAction(CHR_numU); } else
        if(alphaCase == AC_UPPER)  { processKeyAction(CHR_case); } 
        nextChar = NC_NORMAL;
        keyActionProcessed = true;
        break;

      case CHR_case:
        numLock = false;
        int16_t sm = softmenu[softmenuStack[0].softmenuId].menuItem;                                      //JMvv
        nextChar = NC_NORMAL;
        if(alphaCase == AC_LOWER) {
          alphaCase = AC_UPPER;
          if(sm == -MNU_alpha_omega || sm == -MNU_ALPHAintl) {
            softmenuStack[0].softmenuId--; // Switch case menu
          }
        }
        else {
          alphaCase = AC_LOWER;
          if(sm == -MNU_ALPHA_OMEGA || sm == -MNU_ALPHAINTL) {
            softmenuStack[0].softmenuId++; // Switch case menu
          }
        }
        showAlphaModeonGui(); //dr JM, see keyboardtweaks
        keyActionProcessed = true;
        break;                                                                                                               //JM^^
 //TOFIX ^^




      default:
      {
        if(calcMode == CM_ASSIGN && itemToBeAssigned != 0 && item == ITM_USERMODE) {
          while(softmenuStack[0].softmenuId > 1) {
            popSoftmenu();
          }
          if(previousCalcMode == CM_AIM) {
            softmenuStack[0].softmenuId = 1;
            #if defined(PC_BUILD) && (SCREEN_800X480 == 0)
              calcModeAimGui();
            #endif // PC_BUILD && (SCREEN_800X480 == 0)
          }
          else {
            leaveAsmMode();
          }
          keyActionProcessed = true;
        }
        else if(calcMode == CM_ASSIGN && itemToBeAssigned == 0 && item == ITM_USERMODE) {
          tamEnterMode(ITM_ASSIGN);
          calcMode = previousCalcMode;
          keyActionProcessed = true;
        }
        else if((calcMode != CM_PEM || !getSystemFlag(FLAG_ALPHA)) && catalog && catalog != CATALOG_MVAR) {
          if(ITM_A <= item && item <= ITM_Z && lowercaseselected) {
            addItemToBuffer(item + 26);
            keyActionProcessed = true;
          }

          else if(((ITM_ALPHA <= item && item <= ITM_OMEGA) || (ITM_QOPPA <= item && item <= ITM_SAMPI)) && lowercaseselected) {  //JM GREEK
            addItemToBuffer(item + 36);
            keyActionProcessed = true;
          }

          else if(item == ITM_DOWN_ARROW || item == ITM_UP_ARROW) {
            addItemToBuffer(item);
            keyActionProcessed = true;
          }
          break;
        }
        else if(tam.mode) {
          if(tam.alpha) {
            processAimInput(item);
          } else {
            addItemToBuffer(item);
            keyActionProcessed = true;
          }
          break;
        }
        else {
          switch(calcMode) {
            case CM_NORMAL:
              #ifdef PC_BUILD_VERBOSE0
                 #ifdef PC_BUILD
                   printf("$"); //####
                 #endif
              #endif
              if(item == ITM_EXPONENT || item == ITM_PERIOD || (ITM_0 <= item && item <= ITM_9)) {
                addItemToNimBuffer(item);
                keyActionProcessed = true;
              }
              // Following commands do not timeout to NOP
              else if(item == ITM_UNDO || item == ITM_BST || item == ITM_SST || item == ITM_PR || item == ITM_AIM) {
                runFunction(item);
                keyActionProcessed = true;
              }
              break;

            case CM_AIM:
            case CM_EIM:
              #ifdef PC_BUILD_VERBOSE0
                 #ifdef PC_BUILD
                   printf("^"); //####
                 #endif
              #endif
              processAimInput(item);
              refreshRegisterLine(AIM_REGISTER_LINE);   //JM  No if needed, it does nothing if not in NIM. TO DISPLAY NUMBER KEYPRESS DIRECTLY AFTER PRESS, NOT ONLY UPON RELEASE          break;
              break;

            case CM_NIM:
              #ifdef PC_BUILD_VERBOSE0
                #ifdef PC_BUILD
                  printf("&"); //####
                #endif
              #endif
              keyActionProcessed = true;
              addItemToNimBuffer(item);

              if( ((ITM_0 <= item && item <= ITM_9) || ((ITM_A <= item && item <= ITM_F) && lastIntegerBase >= 11) ) || item == ITM_CHS || item == ITM_EXPONENT || item == ITM_PERIOD) {   //JMvv Direct keypresses; //JMNIM Added direct A-F for hex entry
                refreshRegisterLine(REGISTER_X);
              }                                                                                   //JM^^
              break;

            case CM_MIM:
              addItemToBuffer(item);
              keyActionProcessed = true;
              break;

            case CM_REGISTER_BROWSER:
              if(item == ITM_PERIOD) {
                rbr1stDigit = true;
                if(rbrMode == RBR_GLOBAL) {
                  if(currentLocalRegisters != NULL) {
                    rbrMode = RBR_LOCAL;
                    currentRegisterBrowserScreen = FIRST_LOCAL_REGISTER;
                  }
                  else if(numberOfNamedVariables > 0) {
                    rbrMode = RBR_NAMED;
                    currentRegisterBrowserScreen = FIRST_NAMED_VARIABLE;
                  }
                }
                else if(rbrMode == RBR_LOCAL) {
                  if(numberOfNamedVariables > 0) {
                    rbrMode = RBR_NAMED;
                    currentRegisterBrowserScreen = FIRST_NAMED_VARIABLE;
                  }
                  else {
                    rbrMode = RBR_GLOBAL;
                    currentRegisterBrowserScreen = REGISTER_X;
                  }
                }
                else if(rbrMode == RBR_NAMED) {
                  rbrMode = RBR_GLOBAL;
                  currentRegisterBrowserScreen = REGISTER_X;
                }
              }
              else if(item == ITM_RS) {
                rbr1stDigit = true;
                showContent = !showContent;
              }
              else if(item == ITM_RCL) {
                rbr1stDigit = true;
                if(rbrMode == RBR_GLOBAL || rbrMode == RBR_LOCAL) {
                  calcMode = previousCalcMode;
                  fnRecall(currentRegisterBrowserScreen);
                  setSystemFlag(FLAG_ASLIFT);
                }
                else if(rbrMode == RBR_NAMED) {
                }
              }
              else if(ITM_0 <= item && item <= ITM_9 && (rbrMode == RBR_GLOBAL || rbrMode == RBR_LOCAL)) {
                if(rbr1stDigit) {
                  rbr1stDigit = false;
                  rbrRegister = item - ITM_0;
                }
                else {
                  rbr1stDigit = true;
                  rbrRegister = rbrRegister*10 + item - ITM_0;

                  if(rbrMode == RBR_GLOBAL) {
                    currentRegisterBrowserScreen = rbrRegister;
                  }
                  else {
                    rbrRegister = (rbrRegister >= currentNumberOfLocalRegisters ? 0 : rbrRegister);
                    currentRegisterBrowserScreen = FIRST_LOCAL_REGISTER + rbrRegister;
                  }
                }
              }

              keyActionProcessed = true;
              break;

            case CM_FLAG_BROWSER:
            case CM_FONT_BROWSER:
            case CM_ERROR_MESSAGE:
            case CM_BUG_ON_SCREEN:
              keyActionProcessed = true;
              break;

            case CM_GRAPH:
            case CM_PLOT_STAT:
              if(item == ITM_EXIT1 || item == ITM_BACKSPACE) {
                fnPlotClose(0);
              }
              break;

            case CM_LISTXY:                     //JM VV
              if(item == ITM_EXIT1 || item == ITM_BACKSPACE) {
//                calcMode = previousCalcMode;
                fnPlotClose(0);
              }
              keyActionProcessed = true;
              break;                            //JM ^^


            case CM_CONFIRMATION:
              if(item == ITEM_CONF_Y || item == ITM_XEQ || item == ITM_ENTER) { // Yes or XEQ or ENTER
                calcMode = previousCalcMode;
                confirmedFunction(CONFIRMED);
              }

              else if(item == ITEM_CONF_N || item == ITM_EXIT1) { // No
                calcMode = previousCalcMode;
              }

              else {
                temporaryInformation = TI_ARE_YOU_SURE;
              }

              keyActionProcessed = true;
              break;

            case CM_PEM:
              if(item == ITM_PR) {
                leavePem();
                calcModeNormal();
                keyActionProcessed = true;
              }
              else if(item == ITM_OFF) {
                fnOff(NOPARAM);
                keyActionProcessed = true;
              }
              else if(aimBuffer[0] != 0 && !getSystemFlag(FLAG_ALPHA) && (item == ITM_toINT || (nimNumberPart == NP_INT_BASE && item == ITM_RCL))) {
                pemAddNumber(item);
                keyActionProcessed = true;
              }
              else if(item == ITM_RS) {
                addStepInProgram(ITM_STOP);
                keyActionProcessed = true;
              }
              break;

            case CM_ASSIGN:
              if(item == ITM_OFF) {
                fnOff(NOPARAM);
                keyActionProcessed = true;
              }
              else if(item > 0 && itemToBeAssigned == 0) {
                itemToBeAssigned = item;
                keyActionProcessed = true;
              }
              else if(item != 0 && itemToBeAssigned != 0) {
                switch(item) {
                  case ITM_ENTER:
                  case ITM_SHIFTf:
                  case ITM_SHIFTg:
                  case KEY_fg:        //JM
                  case ITM_USERMODE:
                  case -MNU_CATALOG:
                  case -MNU_CHARS:
                  case -MNU_PROGS:
                  case -MNU_VARS:
                  case -MNU_MENUS:
                  case ITM_EXIT1:
                  case ITM_OFF:
                  case ITM_BACKSPACE:
                    break;
                  default:
                    tamBuffer[0] = 0;
                    keyActionProcessed = true;
                }
              }
              break;

            case CM_TIMER:
              printf("ITEM: %d\n", item);
              switch(item) {
                case ITM_RS:
                  fnStartStopTimerApp();
                  break;
                case ITM_0:
                case ITM_1:
                case ITM_2:
                case ITM_3:
                case ITM_4:
                case ITM_5:
                case ITM_6:
                case ITM_7:
                case ITM_8:
                case ITM_9:
                  fnDigitKeyTimerApp(item - ITM_0);
                  break;
                case ITM_PERIOD:
                  fnDotTimerApp();
                  break;
                case ITM_ADD:
                  fnPlusTimerApp();
                  break;
                case ITM_RCL:
                  runFunction(ITM_TIMER_RCL);
                  break;
              }
              keyActionProcessed = true;
              break;

            default:
              sprintf(errorMessage, "In function processKeyAction: %" PRIu8 " is an unexpected value while processing calcMode!", calcMode);
              displayBugScreen(errorMessage);
          }
        }
    #ifdef RECORDLOG
      #ifdef PC_BUILD
        if(keyActionProcessed) {                         //JMEXEC
          capture_sequence("keyActionProcessed:", item);  //JMEXEC
        }                                                //JMEXEC
      #endif
    #endif
      }
    }
    #if (REAL34_WIDTH_TEST == 1)
      longIntegerFree(lgInt);
    #endif // (REAL34_WIDTH_TEST == 1)
  }



  static void menuUp(void) {
    int16_t menuId = softmenuStack[0].softmenuId;
    int16_t sm = softmenu[menuId].menuItem;
    if((sm == -MNU_alpha_omega || sm == -MNU_ALPHAintl) && alphaCase == AC_LOWER && arrowCasechange) {  //JMcase
      alphaCase = AC_UPPER;
      showAlphaModeonGui(); //dr JM, see keyboardtweaks
      softmenuStack[0].softmenuId--; // Switch to the upper case menu
    }
    else if((sm == -MNU_ALPHADOT || sm == -MNU_ALPHAMATH || sm == -MNU_ALPHA) && alphaCase == AC_LOWER && arrowCasechange) {  //JMcase
      alphaCase = AC_UPPER;
      showAlphaModeonGui(); //dr JM, see keyboardtweaks
    }
    else {
      int16_t itemShift = (catalog == CATALOG_NONE ? 18 : 6);

      if((softmenuStack[0].firstItem + itemShift) < (menuId < NUMBER_OF_DYNAMIC_SOFTMENUS ? dynamicSoftmenu[menuId].numItems : softmenu[menuId].numItems)) {
        softmenuStack[0].firstItem += itemShift;
        //JM Include or exclude HOME menu screens  //JMHOME
        #define A1 3   //HAAKON //jm_HOME_MIR
        #define A2 9   //HAAKON
        #define B1 10  //NIGEL  //jm_HOME_SUM
        #define B2 11  //NIGEL
        #define C1 12  //JACO   //jm_HOME_FIX
        #define C2 18  //JACO
        //printf("^^--1:%d %d %d menuId:%d item:%d  \n",jm_HOME_MIR,jm_HOME_SUM,jm_HOME_FIX,sm,softmenuStack[softmenuStackPointer].firstItem/18);
        if(!jm_HOME_MIR && sm == -MNU_HOME && softmenuStack[0].firstItem == A1*18) {softmenuStack[0].firstItem = (A2+1)*18;} 
        if(!jm_HOME_SUM && sm == -MNU_HOME && softmenuStack[0].firstItem == B1*18) {softmenuStack[0].firstItem = (B2+1)*18;} 
        if(!jm_HOME_FIX && sm == -MNU_HOME && softmenuStack[0].firstItem == C1*18) {softmenuStack[0].firstItem = (C2+1)*18;}
        //printf("^^--2:      menuId:%d item:%d  \n",sm,softmenuStack[softmenuStackPointer].firstItem/18);
      }
      else {
        softmenuStack[0].firstItem = 0;
      }

      setCatalogLastPos();
    }
  }



  static void menuDown(void) {
    int16_t menuId = softmenuStack[0].softmenuId;
    int16_t sm = softmenu[menuId].menuItem;
    if((sm == -MNU_ALPHA_OMEGA || sm == -MNU_ALPHAINTL) && alphaCase == AC_UPPER && arrowCasechange) {  //JMcase
      alphaCase = AC_LOWER;
      showAlphaModeonGui(); //dr JM, see keyboardtweaks
      softmenuStack[0].softmenuId++; // Switch to the lower case menu
    }
    else if((sm == -MNU_ALPHADOT || sm == -MNU_ALPHAMATH || sm == -MNU_ALPHA) && alphaCase == AC_UPPER && arrowCasechange) {  //JMcase
      alphaCase = AC_LOWER;
      showAlphaModeonGui(); //dr JM, see keyboardtweaks
    }
    else {
      int16_t itemShift = (catalog == CATALOG_NONE ? 18 : 6);

      if((softmenuStack[0].firstItem - itemShift) >= 0) {
        softmenuStack[0].firstItem -= itemShift;
        //JM Include or exclude HOME menu screens  //JMHOME
        //printf("vv--1:%d %d %d menuId:%d item:%d  \n",jm_HOME_MIR,jm_HOME_SUM,jm_HOME_FIX,sm,softmenuStack[softmenuStackPointer].firstItem/18);
        if(!jm_HOME_FIX && sm == -MNU_HOME && softmenuStack[0].firstItem == C2*18) {softmenuStack[0].firstItem = (C1-1)*18;}
        if(!jm_HOME_SUM && sm == -MNU_HOME && softmenuStack[0].firstItem == B2*18) {softmenuStack[0].firstItem = (B1-1)*18;} 
        if(!jm_HOME_MIR && sm == -MNU_HOME && softmenuStack[0].firstItem == A2*18) {softmenuStack[0].firstItem = (A1-1)*18;}
        printf("vv--2:      menuId:%d item:%d  \n",sm,softmenuStack[0].firstItem/18);
      }
      else if((softmenuStack[0].firstItem - itemShift) >= -5) {
        softmenuStack[0].firstItem = 0;
      }
      else {
        if(menuId < NUMBER_OF_DYNAMIC_SOFTMENUS) {
          softmenuStack[0].firstItem = ((dynamicSoftmenu[menuId].numItems - 1)/6) / (itemShift/6) * itemShift;
        }
        else {
          softmenuStack[0].firstItem = ((       softmenu[menuId].numItems - 1)/6) / (itemShift/6) * itemShift;
        }
      }
      setCatalogLastPos();
    }
  }
#endif // !TESTSUITE_BUILD



void fnKeyEnter(uint16_t unusedButMandatoryParameter) {
    doRefreshSoftMenu = true;     //dr
  #ifndef TESTSUITE_BUILD
    switch(calcMode) {
      case CM_NORMAL:
        if(!eRPN || !nimWhenButtonPressed) {                 //vv PHM eRPN 2021-07
          setSystemFlag(FLAG_ASLIFT);
          saveForUndo();
          if(lastErrorCode == ERROR_RAM_FULL) goto undo_disabled;

          liftStack();
          if(lastErrorCode == ERROR_RAM_FULL) goto ram_full;
          copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_X);
        }
        if(eRPN) {
          setSystemFlag(FLAG_ASLIFT);
        }
        else {                                               //^^ PHM eRPN 2021-07
          clearSystemFlag(FLAG_ASLIFT);
        }                                                    //PHM eRPN 2021-07
        break;

      case CM_AIM:
        if(softmenuStack[0].softmenuId == mm_MNU_ALPHA || softmenu[softmenuStack[0].softmenuId].menuItem == -MNU_T_EDIT) {     //JMvv
          popSoftmenu();
        }                                                     //JM^^

        calcModeNormal();
        popSoftmenu();

        if(aimBuffer[0] == 0) {
          undo();
        }
        else {
          int16_t len = stringByteLength(aimBuffer) + 1;

          reallocateRegister(REGISTER_X, dtString, TO_BLOCKS(len), amNone);
          xcopy(REGISTER_STRING_DATA(REGISTER_X), aimBuffer, len);

          if(!eRPN) {                                  //PHM eRPN 2021-07
            setSystemFlag(FLAG_ASLIFT);
            saveForUndo();
            if(lastErrorCode == ERROR_RAM_FULL) goto undo_disabled;
            liftStack();
            if(lastErrorCode == ERROR_RAM_FULL) goto ram_full;
            clearSystemFlag(FLAG_ASLIFT);

            copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_X);
            if(lastErrorCode == ERROR_RAM_FULL) goto ram_full;
            aimBuffer[0] = 0;
          }
          else {
            setSystemFlag(FLAG_ASLIFT);
            aimBuffer[0] = 0;              //PHM JM Keeping the structure like 43S, to be able to pick up changes from their side easier
          }
        }
        break;

      case CM_MIM:
        mimEnter(false);
        break;

/* 43S code not in use: PHM. JM Keeping the structure like in 43S, to be able to pick up changes
      case CM_NIM:
        closeNim();

        if(calcMode != CM_NIM && lastErrorCode == 0) {
          setSystemFlag(FLAG_ASLIFT);
          saveForUndo();
          if(lastErrorCode == ERROR_RAM_FULL) goto undo_disabled;
          liftStack();
          if(lastErrorCode == ERROR_RAM_FULL) goto ram_full;
          clearSystemFlag(FLAG_ASLIFT);
          copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_X);
          if(lastErrorCode == ERROR_RAM_FULL) goto ram_full;
        }
        break;
--- */

      case CM_EIM:
        if(aimBuffer[0] != 0) {
          setEquation(currentFormula, aimBuffer);
        }
        if(softmenu[softmenuStack[0].softmenuId].menuItem == -MNU_EQ_EDIT) {
          calcModeNormal();
          if(allFormulae[currentFormula].pointerToFormulaData == WP43S_NULL) deleteEquation(currentFormula);
        }
        popSoftmenu();
        break;

      case CM_REGISTER_BROWSER:
      case CM_FLAG_BROWSER:
      case CM_FONT_BROWSER:
      case CM_ERROR_MESSAGE:
      case CM_BUG_ON_SCREEN:
      case CM_PLOT_STAT:
      case CM_LISTXY:                     //JM
      case CM_GRAPH:                      //JM
        break;

      case CM_TIMER:
        fnEnterTimerApp();
        break;

      case CM_CONFIRMATION:
        calcMode = previousCalcMode;
        confirmedFunction(CONFIRMED);
        break;

      default:
        sprintf(errorMessage, "In function fnKeyEnter: unexpected calcMode value (%" PRIu8 ") while processing key ENTER!", calcMode);
        displayBugScreen(errorMessage);
    }
    return;

undo_disabled:
    temporaryInformation = TI_UNDO_DISABLED;
    return;

ram_full:
    displayCalcErrorMessage(ERROR_RAM_FULL, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    fnUndo(NOPARAM);
    return;
  #endif // !TESTSUITE_BUILD
}



void fnKeyExit(uint16_t unusedButMandatoryParameter) {
  #ifndef TESTSUITE_BUILD
    if(lastErrorCode == 0 && softmenu[softmenuStack[0].softmenuId].menuItem == -MNU_MVAR) {
      currentSolverStatus &= ~SOLVER_STATUS_INTERACTIVE;
    }

    int16_t tmp1 = softmenu[softmenuStack[0].softmenuId].menuItem;            //JM
    int16_t tmp2 = softmenu[softmenuStack[1].softmenuId].menuItem;            //JM
    int16_t tmp3 = softmenu[softmenuStack[2].softmenuId].menuItem;            //JM
    doRefreshSoftMenu = true;     //dr
  
  #ifdef PC_BUILD
    jm_show_calc_state("fnKeyExit");
  #endif

    if(tmp1 == -MNU_SYSFL) {                                                       //JM auto recover out of SYSFL
      numberOfTamMenusToPop = 2;
      tamLeaveMode();
      return;
    }

    if(catalog) {
      if(lastErrorCode != 0) {
        lastErrorCode = 0;
      }
      else {
        leaveAsmMode();
        popSoftmenu();
        if(tam.mode) {
          numberOfTamMenusToPop--;
        }
      }
      return;
    }

    if(tam.mode) {
      if(numberOfTamMenusToPop > 1) {
        popSoftmenu();
        numberOfTamMenusToPop--;
      }
      else {
        tamLeaveMode();
      }
      return;
    }

    if(softmenu[softmenuStack[0].softmenuId].menuItem == -ITM_MENU) {
      dynamicMenuItem = 20;
      fnProgrammableMenu(NOPARAM);
      return;
    }

    switch(calcMode) {
      case CM_NORMAL:
        if(lastErrorCode != 0) {
          lastErrorCode = 0;
        }
        else {
          if(softmenuStack[0].softmenuId <= 1) { // MyMenu or MyAlpha is displayed
            currentInputVariable = INVALID_VARIABLE;
          }
          else {                  //jm: this is where 43S cleared an error
            popSoftmenu();
          }
        }
        break;

      case CM_AIM:
//TOFIX vv
        if(tmp1 == -MNU_ALPHA || tmp1 == -MNU_T_EDIT) {  //JM
          if(tmp1 == -MNU_T_EDIT && tmp2 == -MNU_ALPHA && tmp3 == -MNU_XXEQ) {popSoftmenu();}       //JM auto double pop
          softmenuStack[0].softmenuId = 1;               //JM
        }                                                //JM
//TOFIX ^^
        if(running_program_jm || softmenuStack[0].softmenuId <= 1) { // MyMenu or MyAlpha is displayed
          closeAim();
          saveForUndo();
          if(lastErrorCode == ERROR_RAM_FULL) goto undo_disabled;
        }
        else {
          popSoftmenu();
        }
        break;

      case CM_NIM:
        addItemToNimBuffer(ITM_EXIT1);
        break;

      case CM_MIM:
        if(softmenu[softmenuStack[0].softmenuId].menuItem == -MNU_M_EDIT) {
          mimEnter(true);
          if(matrixIndex == findNamedVariable("STATS")) calcSigma(0);
          mimFinalize();
          calcModeNormal();
          updateMatrixHeightCache();
        }
        popSoftmenu(); // close softmenu dedicated for the MIM
        break;

      case CM_PEM:
        if(softmenuStack[0].softmenuId > 1) { // not MyMenu and not MyAlpha
          popSoftmenu();
          break;
        }

        aimBuffer[0] = 0;
        leavePem();
        calcModeNormal();
        saveForUndo();
        if(lastErrorCode == ERROR_RAM_FULL) goto undo_disabled;
        break;

      case CM_EIM:
        if(lastErrorCode != 0) {
          lastErrorCode = 0;
        }
        else {
          if(softmenu[softmenuStack[0].softmenuId].menuItem == -MNU_EQ_EDIT) {
            calcModeNormal();
            if(allFormulae[currentFormula].pointerToFormulaData == WP43S_NULL) deleteEquation(currentFormula);
          }
          popSoftmenu();
        }
        break;

      case CM_REGISTER_BROWSER:
      case CM_FLAG_BROWSER:
      case CM_FONT_BROWSER:
        rbr1stDigit = true;
        calcMode = previousCalcMode;
        break;

      case CM_TIMER:
        if(lastErrorCode != 0) {
          lastErrorCode = 0;
        }
        else {
          fnLeaveTimerApp();
        }
        break;

      case CM_BUG_ON_SCREEN:
        calcMode = previousCalcMode;
        break;


      case CM_LISTXY:                      //JM vv
        calcMode = previousCalcMode;
        softmenuStack[0].firstItem = 0;
        fnUndo(0);
        break;                              //JM ^^



      case CM_GRAPH:
      case CM_PLOT_STAT:
        lastPlotMode = PLOT_NOTHING;
        plotSelection = 0;
        calcMode = CM_NORMAL;
        fnUndo(0);
        popSoftmenu();
        break;

      case CM_CONFIRMATION:
        calcMode = previousCalcMode;
        temporaryInformation = TI_NO_INFO;
        if(programRunStop == PGM_WAITING) {
          programRunStop = PGM_STOPPED;
        }
        break;

      case CM_ASSIGN:
        if(softmenuStack[0].softmenuId <= 1 && softmenuStack[1].softmenuId <= 1) { // MyMenu or MyAlpha is displayed
          calcMode = previousCalcMode;
        }
        else {
          popSoftmenu();
        }
        break;

      default:
        sprintf(errorMessage, "In function fnKeyExit: unexpected calcMode value (%" PRIu8 ") while processing key EXIT!", calcMode);
        displayBugScreen(errorMessage);
    }

    last_CM = 253; //Force redraw   //JMvvv Show effect of Exit immediately
    refreshScreen();                //JM^^^
    return;

undo_disabled:
    temporaryInformation = TI_UNDO_DISABLED;
    return;
  #endif // !TESTSUITE_BUILD
}



/********************************************//**
 * \brief Processing CC key
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnKeyCC(uint16_t complex_Type) {    //JM Using 'unusedButMandatoryParameter' complex_Type=KEY_COMPLEX
    doRefreshSoftMenu = true;     //dr
  #ifndef TESTSUITE_BUILD
    uint32_t dataTypeX;
    uint32_t dataTypeY;

    // The switch statement is broken up here, due to multiple conditions.                      //JM
    if((calcMode == CM_NIM) && (complex_Type == KEY_COMPLEX)) addItemToNimBuffer(ITM_EXIT1);    //JM Allow COMPLEX to be used from NIM
    if(calcMode == CM_NORMAL || ((calcMode == CM_NIM) && (complex_Type == KEY_COMPLEX))) {      //JM
      dataTypeX = getRegisterDataType(REGISTER_X);
      dataTypeY = getRegisterDataType(REGISTER_Y);

      if(   (dataTypeX == dtReal34 || dataTypeX == dtLongInteger)
         && (dataTypeY == dtReal34 || dataTypeY == dtLongInteger)) {
        runFunction(ITM_REtoCX);
      }
      else if(dataTypeX == dtComplex34) {
        runFunction(ITM_CXtoRE);
      }
      else if(dataTypeX == dtReal34Matrix && dataTypeY == dtReal34Matrix) {
        runFunction(ITM_REtoCX);
      }
      else if(dataTypeX == dtComplex34Matrix) {
        runFunction(ITM_CXtoRE);
      }
      else {
        displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X); // Invalid input data type for this operation
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "You cannot use CC with %s in X and %s in Y!", getDataTypeName(getRegisterDataType(REGISTER_X), true, false), getDataTypeName(getRegisterDataType(REGISTER_Y), true, false));
        moreInfoOnError("In function fnKeyCC:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      }
      return;                            //JM
    }

    switch(calcMode) {                     //JM
      case CM_NIM:
        addItemToNimBuffer(ITM_CC);
        break;

      case CM_MIM:
        mimAddNumber(ITM_CC);
        break;

      case CM_PEM:
        if(aimBuffer[0] != 0 && !getSystemFlag(FLAG_ALPHA)) {
          pemAddNumber(ITM_CC);
        }
        break;

      case CM_EIM:
      case CM_REGISTER_BROWSER:
      case CM_FLAG_BROWSER:
      case CM_FONT_BROWSER:
      case CM_PLOT_STAT:
      case CM_TIMER:
      case CM_LISTXY:                     //JM
      case CM_GRAPH:                      //JM
        break;

      default:
        sprintf(errorMessage, "In function fnKeyCC: unexpected calcMode value (%" PRIu8 ") while processing key CC!", calcMode);
        displayBugScreen(errorMessage);
    }
  #endif // !TESTSUITE_BUILD
}



void fnKeyBackspace(uint16_t unusedButMandatoryParameter) {
  #ifndef TESTSUITE_BUILD
    uint16_t lg;
#ifndef SAVE_SPACE_DM42_10
    uint8_t *nextStep;
#endif //SAVE_SPACE_DM42_10
    
    if(tam.mode) {
      tamProcessInput(ITM_BACKSPACE);
      return;
    }

    switch(calcMode) {
      case CM_NORMAL:
        if(lastErrorCode != 0) {
          lastErrorCode = 0;
        }
        else {
          runFunction(ITM_CLX);
        }
        break;

      case CM_AIM:
        if(catalog && catalog != CATALOG_MVAR) {
          if(stringByteLength(aimBuffer) > 0) {
            lg = stringLastGlyph(aimBuffer);
            aimBuffer[lg] = 0;
            xCursor = showString(aimBuffer, &standardFont, 1, Y_POSITION_OF_AIM_LINE + 6, vmNormal, true, true);
          }
        }
        else if(stringByteLength(aimBuffer) > 0) {

#ifdef TEXT_MULTILINE_EDIT
//JMCURSORvv SPLIT STRING AT CURSOR POSITION
          uint8_t T_cursorPos_tmp;
          T_cursorPos_tmp = aimBuffer[T_cursorPos];
          aimBuffer[T_cursorPos] = 0;                  //break it at the current cursor
          lg = stringLastGlyph(aimBuffer);             //find beginning of last glyoh, to delete
          aimBuffer[lg] = 0;                           //delete it
          aimBuffer[T_cursorPos] = T_cursorPos_tmp;    //Restore broken glyph in middle at break point
          uint16_t ix = 0;
          while(aimBuffer[ix+T_cursorPos] != 0) {      //copy second part to append to first part
            aimBuffer[ix+lg] = aimBuffer[ix+T_cursorPos];
            ix++;
          }
          aimBuffer[ix+lg]=0;                          //end new buffer
          //printf("newXCursor=%d  T_cursorPos=%d  stringLastGlyph(aimBuffer)=%d\n",newXCursor,T_cursorPos,stringLastGlyph(aimBuffer));
          if(T_cursorPos <= 1 + stringLastGlyph(aimBuffer)) {
            fnT_ARROW(ITM_T_LEFT_ARROW);                               //move cursor one left
          }          
//JMCURSOR^^
#else
          lg = stringLastGlyph(aimBuffer);
          aimBuffer[lg] = 0;
#endif //TEXT_MULTILINE_EDIT

        }
        break;

      case CM_NIM:
        addItemToNimBuffer(ITM_BACKSPACE);
        break;

      case CM_MIM:
        mimAddNumber(ITM_BACKSPACE);
        break;

      case CM_EIM:
        if(xCursor > 0) {
          char *srcPos = aimBuffer;
          char *dstPos = aimBuffer;
          char *lstPos = aimBuffer + stringNextGlyph(aimBuffer, stringLastGlyph(aimBuffer));
          --xCursor;
          for(uint32_t i = 0; i < xCursor; ++i) dstPos += (*dstPos & 0x80) ? 2 : 1;
          srcPos = dstPos + ((*dstPos & 0x80) ? 2 : 1);
          for(; srcPos <= lstPos;) *(dstPos++) = *(srcPos++);
        }
        break;

      //case CM_ASM_OVER_NORMAL:
      //  addItemToBuffer(ITM_BACKSPACE);
      //  break;

      case CM_REGISTER_BROWSER:
      case CM_FLAG_BROWSER:
      case CM_FONT_BROWSER:
        calcMode = previousCalcMode;
        break;

      case CM_BUG_ON_SCREEN:
      case CM_PLOT_STAT:
      case CM_GRAPH:                 //TODO maybe move down to the next case
        calcMode = previousCalcMode;
        break;

      case CM_LISTXY:                     //JM vv
        calcMode = previousCalcMode;
        keyActionProcessed = true;
        break;                            //JM ^^

      case CM_CONFIRMATION:
        calcMode = previousCalcMode;
        temporaryInformation = TI_NO_INFO;
        if(programRunStop == PGM_WAITING) {
          programRunStop = PGM_STOPPED;
        }
        break;

      case CM_PEM:
        #ifndef SAVE_SPACE_DM42_10
        if(getSystemFlag(FLAG_ALPHA)) {
          pemAlpha(ITM_BACKSPACE);
          if(aimBuffer[0] == 0 && !getSystemFlag(FLAG_ALPHA) && currentLocalStepNumber > 1) {
            currentStep = findPreviousStep(currentStep);
            --currentLocalStepNumber;
          }
        }
        else if(aimBuffer[0] == 0) {
          nextStep = findNextStep(currentStep);
          if(*currentStep != 255 || *(currentStep + 1) != 255) { // Not the last END
            deleteStepsFromTo(currentStep, nextStep);
          }
          if(currentLocalStepNumber > 1) {
            currentStep = findPreviousStep(currentStep);
            --currentLocalStepNumber;
          }
        }
        else {
          pemAddNumber(ITM_BACKSPACE);
          if(aimBuffer[0] == 0 && currentLocalStepNumber > 1) {
            currentStep = findPreviousStep(currentStep);
            --currentLocalStepNumber;
          }
        }
        #endif //SAVE_SPACE_DM42_10
        break;

      case CM_ASSIGN:
        if(itemToBeAssigned == 0) {
          calcMode = previousCalcMode;
        }
        else {
          itemToBeAssigned = 0;
        }
        break;

      case CM_TIMER:
        if(lastErrorCode != 0) {
          lastErrorCode = 0;
        }
        else {
          fnBackspaceTimerApp();
        }
        break;

      default:
        sprintf(errorMessage, "In function fnKeyBackspace: unexpected calcMode value (%" PRIu8 ") while processing key BACKSPACE!", calcMode);
        displayBugScreen(errorMessage);
    }
  #endif // !TESTSUITE_BUILD
}



void fnKeyUp(uint16_t unusedButMandatoryParameter) {
    doRefreshSoftMenu = true;     //dr

  #ifndef TESTSUITE_BUILD  
    int16_t menuId = softmenuStack[0].softmenuId; //JM
    bool_t activatescroll = (calcMode == CM_NORMAL) &&  (//JM
            ((menuId == 0) && jm_NO_BASE_SCREEN) ||
            ((menuId == 0) && (softmenu[menuId].numItems<=18)) ||
            ((menuId >= NUMBER_OF_DYNAMIC_SOFTMENUS) && (softmenu[menuId].numItems<=18)) );

    if(activatescroll && !tam.mode) { //JMSHOW vv
      fnShow_SCROLL(1); 
      return;
    }                              //JMSHOW ^^

    if(tam.mode && !catalog) {
      if(tam.alpha) {
        resetAlphaSelectionBuffer();
        if(currentSoftmenuScrolls()) {
          menuUp();
        }
        else {
          alphaCase = AC_UPPER;
        }
      }
      else {
        addItemToBuffer(ITM_Max);
      }
      return;
    }

    if(softmenu[softmenuStack[0].softmenuId].menuItem == -ITM_MENU) {
      dynamicMenuItem = 18;
      fnProgrammableMenu(NOPARAM);
      return;
    }

    switch(calcMode) {
      case CM_NORMAL:
      case CM_AIM:
      case CM_NIM:
      case CM_EIM:
      case CM_PLOT_STAT:
      case CM_GRAPH:                  //JM
        doRefreshSoftMenu = true;     //jm
        resetAlphaSelectionBuffer();
      
        //JM Arrow up and down if no menu other than AHOME of MyA       //JMvv
        if(!arrowCasechange && calcMode == CM_AIM && isJMAlphaSoftmenu(menuId)) {
          fnT_ARROW(ITM_UP1);
        }
              //ignoring the base menu, MY_ALPHA_MENU below
              // make this keyActionProcessed = false; to have arrows up and down placed in bufferize
              // make arrowCasechnage true
                                                                       //JM^^
        else

        if(currentSoftmenuScrolls()) {
          menuUp();
        }
        else if(calcMode == CM_NORMAL) {
          fnBst(NOPARAM);
          #ifdef DMCP_BUILD
            lcd_refresh();
          #else // !DMCP_BUILD
            refreshLcd(NULL);
          #endif // DMCP_BUILD
        }
        if(softmenu[softmenuStack[0].softmenuId].menuItem == -MNU_PLOT_LR){
          fnPlotStat(PLOT_NXT);
        }
        else if(softmenu[softmenuStack[0].softmenuId].menuItem == -MNU_EQN){
          if(currentFormula == 0) currentFormula = numberOfFormulae;
          --currentFormula;
        }
        else {
          //alphaCase = AC_UPPER;
        }
        break;

      case CM_REGISTER_BROWSER:
        rbr1stDigit = true;
        if(rbrMode == RBR_GLOBAL) {
          currentRegisterBrowserScreen = modulo(currentRegisterBrowserScreen + 1, FIRST_LOCAL_REGISTER);
        }
        else if(rbrMode == RBR_LOCAL) {
          currentRegisterBrowserScreen = modulo(currentRegisterBrowserScreen - FIRST_LOCAL_REGISTER + 1, currentNumberOfLocalRegisters) + FIRST_LOCAL_REGISTER;
        }
        else if(rbrMode == RBR_NAMED) {
          currentRegisterBrowserScreen = modulo(currentRegisterBrowserScreen - FIRST_NAMED_VARIABLE + 1, numberOfNamedVariables) + FIRST_NAMED_VARIABLE;
        }
        else {
          sprintf(errorMessage, "In function fnKeyUp: unexpected case while processing key UP! %" PRIu8 " is an unexpected value for rbrMode.", rbrMode);
          displayBugScreen(errorMessage);
        }
        break;

      case CM_FLAG_BROWSER:
        currentFlgScr--;                          //JM removed the 3-x part
        break;

      case CM_FONT_BROWSER:
        if(currentFntScr >= 2) {
          currentFntScr--;
        }
        break;

      case CM_PEM:
        resetAlphaSelectionBuffer();
        if(getSystemFlag(FLAG_ALPHA) && alphaCase == AC_LOWER) {
          alphaCase = AC_UPPER;
          if(softmenu[softmenuStack[0].softmenuId].menuItem == -MNU_alpha_omega || softmenu[softmenuStack[0].softmenuId].menuItem == -MNU_ALPHAintl) {
            softmenuStack[0].softmenuId--; // Switch to the upper case menu
          }
        }
        else if(currentSoftmenuScrolls()) {
          menuUp();
        }
        else {
          if(aimBuffer[0] != 0) {
            if(getSystemFlag(FLAG_ALPHA)) pemCloseAlphaInput();
            else                          pemCloseNumberInput();
            aimBuffer[0] = 0;
            --currentLocalStepNumber;
            currentStep = findPreviousStep(currentStep);
          }
          fnBst(NOPARAM);
        }
        break;

      case CM_LISTXY:                     //JM vv
        ListXYposition += 10;
        break;                            //JM ^^
          
      case CM_MIM:
      case CM_ASSIGN:
        if(currentSoftmenuScrolls()) {
          menuUp();
        }
        break;

      case CM_TIMER:
        fnUpTimerApp();
        break;

      default:
        sprintf(errorMessage, "In function fnKeyUp: unexpected calcMode value (%" PRIu8 ") while processing key UP!", calcMode);
        displayBugScreen(errorMessage);
    }
  #endif // !TESTSUITE_BUILD
}



void fnKeyDown(uint16_t unusedButMandatoryParameter) {
    doRefreshSoftMenu = true;     //dr

#ifndef TESTSUITE_BUILD
    int16_t menuId = softmenuStack[0].softmenuId; //JM
    bool_t activatescroll = (calcMode == CM_NORMAL) &&  (//JM
          ((menuId == 0) && jm_NO_BASE_SCREEN) ||
          ((menuId == 0) && (softmenu[menuId].numItems<=18)) ||
          ((menuId >= NUMBER_OF_DYNAMIC_SOFTMENUS) && (softmenu[menuId].numItems<=18)) );
    if(activatescroll && !tam.mode) { //JMSHOW vv
      fnShow_SCROLL(2);
      return;
    }                             //JMSHOW ^^

    if(tam.mode && !catalog) {
      if(tam.alpha) {
        resetAlphaSelectionBuffer();
        if(currentSoftmenuScrolls()) {
          menuDown();
        }
        else {
          alphaCase = AC_LOWER;
        }
      }
      else {
        addItemToBuffer(ITM_Min);
      }
      return;
    }

    if(softmenu[softmenuStack[0].softmenuId].menuItem == -ITM_MENU) {
      dynamicMenuItem = 19;
      fnProgrammableMenu(NOPARAM);
      return;
    }

    switch(calcMode) {
      case CM_NORMAL:
      case CM_AIM:
      case CM_NIM:
      case CM_EIM:
      case CM_PLOT_STAT:
      case CM_GRAPH:                  //JM
        doRefreshSoftMenu = true;     //jm
        resetAlphaSelectionBuffer();

        //JM Arrow up and down if AHOME of MyA       //JMvv
        if(!arrowCasechange && calcMode == CM_AIM && isJMAlphaSoftmenu(menuId)) {
          fnT_ARROW(ITM_DOWN1);
        }
              //ignoring the base menu, MY_ALPHA_MENU below
              // make this keyActionProcessed = false; to have arrows up and down placed in bufferize
              // make arrowCasechnage true
                                                                       //JM^^
        else

        if(currentSoftmenuScrolls()) {
          menuDown();
        }
        else if(calcMode == CM_NORMAL) {
          fnSst(NOPARAM);
        }
        if(softmenu[softmenuStack[0].softmenuId].menuItem == -MNU_PLOT_LR){
          fnPlotStat(PLOT_REV); //REVERSE
        }
        else if(softmenu[softmenuStack[0].softmenuId].menuItem == -MNU_EQN){
          ++currentFormula;
          if(currentFormula == numberOfFormulae) currentFormula = 0;
        }
        else {
          //alphaCase = AC_LOWER;
        }
        break;

      case CM_REGISTER_BROWSER:
        rbr1stDigit = true;
        if(rbrMode == RBR_GLOBAL) {
          currentRegisterBrowserScreen = modulo(currentRegisterBrowserScreen - 1, FIRST_LOCAL_REGISTER);
        }
        else if(rbrMode == RBR_LOCAL) {
          currentRegisterBrowserScreen = modulo(currentRegisterBrowserScreen - FIRST_LOCAL_REGISTER - 1, currentNumberOfLocalRegisters) + FIRST_LOCAL_REGISTER;
        }
        else if(rbrMode == RBR_NAMED) {
          currentRegisterBrowserScreen = modulo(currentRegisterBrowserScreen - 1000 - 1, numberOfNamedVariables) + 1000;
        }
        else {
          sprintf(errorMessage, "In function fnKeyDown: unexpected case while processing key DOWN! %" PRIu8 " is an unexpected value for rbrMode.", rbrMode);
          displayBugScreen(errorMessage);
        }
        break;

      case CM_FLAG_BROWSER:
        currentFlgScr++;                          //JM removed the 3-x part
        break;

      case CM_FONT_BROWSER:
        if(currentFntScr < numScreensNumericFont + numScreensStandardFont) {
          currentFntScr++;
        }
        break;

      case CM_PEM:
        resetAlphaSelectionBuffer();
        if(getSystemFlag(FLAG_ALPHA) && alphaCase == AC_UPPER) {
          alphaCase = AC_LOWER;
          if(softmenu[softmenuStack[0].softmenuId].menuItem == -MNU_ALPHA_OMEGA || softmenu[softmenuStack[0].softmenuId].menuItem == -MNU_ALPHAINTL) {
            softmenuStack[0].softmenuId++; // Switch to the lower case menu
          }
        }
        else if(currentSoftmenuScrolls()) {
          menuDown();
        }
        else {
          if(aimBuffer[0] != 0) {
            if(getSystemFlag(FLAG_ALPHA)) pemCloseAlphaInput();
            else                          pemCloseNumberInput();
            aimBuffer[0] = 0;
            --currentLocalStepNumber;
            currentStep = findPreviousStep(currentStep);
          }
          fnSst(NOPARAM);
        }
        break;

      case CM_LISTXY:                     //JM vv
        ListXYposition -= 10;
        break;                            //JM ^^
          
      case CM_MIM:
      case CM_ASSIGN:
        if(currentSoftmenuScrolls()) {
          menuDown();
        }
        break;

      case CM_TIMER:
        fnDownTimerApp();
        break;

      default:
        sprintf(errorMessage, "In function fnKeyDown: unexpected calcMode value (%" PRIu8 ") while processing key DOWN!", calcMode);
        displayBugScreen(errorMessage);
    }
  #endif // !TESTSUITE_BUILD
}



void fnKeyDotD(uint16_t unusedButMandatoryParameter) {
  #ifndef TESTSUITE_BUILD
    switch(calcMode) {
      case CM_NORMAL:
        constantFractionsOn = false; //JM
        if(getSystemFlag(FLAG_FRACT)) {
          clearSystemFlag(FLAG_FRACT);
        }
        else {
          runFunction(ITM_toREAL);
        }
        break;

      case CM_NIM:
        addItemToNimBuffer(ITM_dotD);
        break;

      case CM_REGISTER_BROWSER:
      case CM_FLAG_BROWSER:
      case CM_FONT_BROWSER:
      case CM_PLOT_STAT:
      case CM_GRAPH:
      case CM_MIM:
      case CM_EIM:
      case CM_TIMER:
      case CM_LISTXY:                     //JM
        break;

      default:
        sprintf(errorMessage, "In function fnKeyDotD: unexpected calcMode value (%" PRIu8 ") while processing key .d!", calcMode);
        displayBugScreen(errorMessage);
    }
  #endif // !TESTSUITE_BUILD
}



void fnKeyAngle(uint16_t unusedButMandatoryParameter) {
  #ifndef TESTSUITE_BUILD
    switch(calcMode) {
      case CM_NORMAL:
        if(getRegisterDataType(REGISTER_X) == dtReal34Matrix) {
          runFunction(ITM_VANGLE);
        }
        else {
          runFunction(ITM_ARG);
        }
        break;

      case CM_NIM:
        addItemToNimBuffer(ITM_ANGLE);
        break;

      case CM_REGISTER_BROWSER:
      case CM_FLAG_BROWSER:
      case CM_FONT_BROWSER:
      case CM_PLOT_STAT:
      case CM_GRAPH:
      case CM_MIM:
      case CM_EIM:
      case CM_TIMER:
        break;

      case CM_ASSIGN:
        if(itemToBeAssigned == 0) {
          itemToBeAssigned = ITM_ANGLE;
        }
        break;

      default:
        sprintf(errorMessage, "In function fnKeyAngle: unexpected calcMode value (%" PRIu8 ") while processing key .d!", calcMode);
        displayBugScreen(errorMessage);
    }
  #endif // !TESTSUITE_BUILD
}



void setLastKeyCode(int key) {
  if(1 <= key && key <= 43) {
    if     (key <=  6) lastKeyCode = key      + 20;
    else if(key <= 12) lastKeyCode = key -  6 + 30;
    else if(key <= 17) lastKeyCode = key - 12 + 40;
    else if(key <= 22) lastKeyCode = key - 17 + 50;
    else if(key <= 27) lastKeyCode = key - 22 + 60;
    else if(key <= 32) lastKeyCode = key - 27 + 70;
    else if(key <= 37) lastKeyCode = key - 32 + 80;
    else if(key <= 43) lastKeyCode = key - 37 + 10; // function keys
  }
}
