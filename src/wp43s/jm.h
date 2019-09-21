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

//wp43s.h


/* ADDITIONAL WP43C functions and routines */


/*********   MAIN OPTION SELECTION                  */
//#define JM_LAYOUT_2_DM42_STRICT  //DM42 compatible layout
#define JM_LAYOUT_1A               //Preferred layout



//This section must be part of both Layout1 and Layout2 and can be taken out of the main code here
#define JM_MULTISHIFT          //MULTISHIFT AND CLRDROP
#define JM_SHIFT_TIMER 4000    //ms
#define JM_CLRDROP_TIMER 500   //ms
#define JM_SHIFT_TIMER_LOOP JM_SHIFT_TIMER/100 //4000/100=40     40x100 ms = 4 second


//keyboard.c   
uint8_t softmenuStackPointer_MEM; //For popping on and off the HOME menu


//keyboard.c  screen.c
bool_t JM_auto_drop_activated;
bool_t JM_auto_drop_enabled;      //JM TIMER CLRDROP
uint8_t JM_SHIFT_RESET;           //JM non-stored non-changeable mode

//keyboard.c
#ifdef DMCP_BUILD                 //JM TIMER variable tmp mem, to check expired time
uint32_t now_MEM;
#endif
#ifdef PC_BUILD
gint64 now_MEM;
#endif

#ifdef DMCP_BUILD                                 //JM TIMER DMCP SHIFTCANCEL
uint32_t now;                                     //JM TIMER DMCP SHIFTCANCEL
#endif                                            //JM TIMER DMCP SHIFTCANCEL
#ifdef PC_BUILD                                   //JM TIMER EMULATOR SHIFTCANCEL
gint64 now;                                       //JM usec  //JM TIMER EMULATOR SHIFTCANCEL
#endif                                            //JM TIMER DMCP SHIFTCANCEL



// Confirmation Y or N changed from original WP43S because the alpha keys order changed
#define ITEM_CONF_Y CHR_2 
#define ITEM_CONF_N ITM_CHS

// Define the second tagline
#define WHO2       "WP" STD_SPACE_3_PER_EM "43C" STD_SPACE_3_PER_EM "2019.Sep.08" STD_SPACE_3_PER_EM "DM42" STD_SPACE_3_PER_EM "Compatible," STD_SPACE_3_PER_EM "by" STD_SPACE_3_PER_EM "Jaymos"   //JM ID

// Define variables that are saved with the config
extern bool_t eRPN;                                                  //JM eRPN Create a flag to enable or disable eRPN. See bufferize.c
extern bool_t HOME3;                                                 //JM HOME Create a flag to enable or disable triple shift HOME3.
extern bool_t SHTIM;                                                 //JM HOME Create a flag to enable or disable SHIFT TIMER CANCEL.
extern bool_t UNITDisplay;                                           //JM UNIT Create a flag to enable or disable unit display 
extern uint8_t SigFigMode;                                           //J SIGFIG 

// Additional routines needed in jm.c
void fnSeteRPN(uint16_t unusedParamButMandatory);
void fnSetHOME3(uint16_t unusedParamButMandatory);
void fnSetSetJM(uint16_t What);
void fnInfo(uint16_t Info);

void fnJM(uint16_t JM_OPCODE);


//items.c
void fnDisplayFormatSigFig(uint16_t displayFormatN);
void fnDisplayFormatUnit(uint16_t displayFormatN);

//keyboard.c
void JM_DOT(int16_t xx, int16_t yy);
void Reset_Shift_Mem(void);
void fnBASE_Hash(uint16_t unusedParamButMandatory);


//display.c
void exponentToUnitDisplayString(int32_t exponent, char *displayString, bool_t nimMode);

//stack.c
void fnLastX   (uint16_t unusedParamButMandatory);            //JM LastX


void fnShowJM  (uint16_t What);



