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

/********************************************//** //JM
 * \file jm.c JM_TEST
 ***********************************************/

/* ADDITIONAL WP43C functions and routines */


#include "wp43s.h"



/********************************************//**
 * RESET TIME FOR SHIFT CANCELLING
 * THIS IS STANDALONE RESET FOR SHIFT TO BE SET BY EMU KEYS. IT ALSO GETS RESET IN KEYBOARD.C
 *
 * FROM keyboard.c
 ***********************************************/
void Reset_Shift_Mem(void) {                            //JM
#ifdef DMCP_BUILD                                       //JM TIMER DMCP SHIFTCANCEL
  now = sys_current_ms();                               //JM TIMER DMCP SHIFTCANCEL
  now_MEM = now;                                        //JM TIMER -- any last key pressed
#endif                                                  //JM
#ifdef PC_BUILD                                         //JM TIMER EMULATOR SHIFTCANCEL
  now = g_get_monotonic_time();                         //JM usec  //JM TIMER EMULATOR SHIFTCANCEL
  now_MEM = now;                                        //JM TIMER -- any last key pressed
#endif                                                  //JM
}


void R_shF(void) {
  if(shiftF) {
    shiftF = false;
    shiftStateChanged = true;
  }
}

void R_shG(void) {
  if(shiftG) {
    shiftG = false;
    shiftStateChanged = true;
  }
}

void S_shF(void) {
  if(!shiftF) {
    shiftF = true;
    shiftStateChanged = true;
  }
}

void S_shG(void) {
  if(!shiftG) {
    shiftG = true;
    shiftStateChanged = true;
  }
}




/********************************************//**
 * NOT TESTED YET. NOT WORKING. CALLED FROM ## in BASE
 *
 * FROM keyboard.c
 ***********************************************/
void fnBASE_Hash(uint16_t unusedParamButMandatory) {
  R_shF(); //shiftF = false;                   //JM
  S_shG(); //shiftG = true;                    //JM
  Reset_Shift_Mem();                //JM
//calcMode = CM_NIM;                //JM Trying to put the calculator into Number Input Mode

#ifdef PC_BUILD
  btnClicked(NULL, "01");
#endif
#ifdef DMCP_BUILD
  btnClicked(NULL, "01");
#endif
// addItemToNimBuffer(/*CHR_NUMBER_SIGN*/KEY_HASH); //Trying out different things
// The point is I am trying to do: 12 # 10, i.e. activate # while input buffer is active, like the true button.
}



/********************************************//**
 * \Set SIGFIG mode
 *
 * FROM DISPLAY.C
 ***********************************************/
void fnDisplayFormatSigFig(uint16_t displayFormatN) {             //JM SIGFIG
  UNITDisplay = false;                                            //JM SIGFIG display Reset
  SigFigMode = displayFormatN;                                    //JM SIGFIG
  fnRefreshRadioState(RB_DI, DF_SF);

  displayFormat = DF_FIX;
  displayFormatDigits = displayFormatN;
  displayRealAsFraction = false;
  refreshStack();
}                                                                 //JM SIGFIG



/********************************************//**
 * \Set UNIT mode
 *
 * FROM DISPLAY.C
 ***********************************************/
void fnDisplayFormatUnit(uint16_t displayFormatN) {               //JM UNIT
  SigFigMode = 0;                                                 //JM UNIT Sigfig works in FIX mode and it makes not sense in UNIT (ENG) mode
  UNITDisplay = true;                                             //JM UNIT display
  fnRefreshRadioState(RB_DI, DF_UN);

  displayFormat = DF_ENG;
  displayFormatDigits = displayFormatN;
  displayRealAsFraction = false;
  if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
    convertLongIntegerRegisterToReal16Register(REGISTER_X, REGISTER_X);
  }
  refreshStack();
}                                                                 //JM UNIT



/********************************************//**   //JM LastX
 * \brief Restores Last X and refreshes the stack   //JM LastX
 *                                                  //JM LastX
 * \param[in] unusedParamButMandatory uint16_t      //JM LastX
 * \return void                                     //JM LastX
 ***********************************************/   //JM LastX
void fnLastX(uint16_t unusedParamButMandatory) {    //JM LastX
  //  fnRecall(REGISTER_L);                         //JM LastX
  liftStack();                                      //JM LastX improved verison
  copySourceRegisterToDestRegister(REGISTER_L, REGISTER_X); //JM LastX improved verison
  refreshStack();                                   //JM LastX improved verison
}                                                   //JM LastX



/********************************************//**
 * \brief Sets/resets flag
 *
 * \param[in] jmConfig uint16_t
 * \return void
 ***********************************************/
void fnSetSetJM(uint16_t jmConfig) {                        //JM Set/Reset setting
  switch(jmConfig) {
  case JC_ERPN:                                             //JM eRPN
    eRPN = !eRPN;
    //fnInfo(eRPN);
    fnRefreshComboxState(CB_JC, JC_ERPN, eRPN);                                 //dr
    break;

  case JC_HOME_TRIPLE:                                      //JM HOME.3
    HOME3 = !HOME3;
    //fnInfo(HOME3);
    fnRefreshComboxState(CB_JC, JC_HOME_TRIPLE, HOME3);                         //dr
    break;

  case JC_SHFT_4s:                                          //JM SH_4s
    ShiftTimoutMode = !ShiftTimoutMode;
    //fnInfo(ShiftTimoutMode);
    fnRefreshComboxState(CB_JC, JC_SHFT_4s, ShiftTimoutMode);                   //dr
    break;

  case JC_BASE_HOME:                                        //JM HOME
    SH_BASE_HOME = !SH_BASE_HOME;
    //fnInfo(SH_BASE_HOME);
    fnRefreshComboxState(CB_JC, JC_BASE_HOME, SH_BASE_HOME);                    //dr
    break;

  case JC_BASE_MYMENU:                                      //JM MYMNU
    SH_BASE_MYMENU = !SH_BASE_MYMENU;
    //fnInfo(SH_BASE_MYMENU);
    fnRefreshComboxState(CB_JC, JC_BASE_MYMENU, SH_BASE_MYMENU);                //dr
    break;

  case JC_BASE_AHOME:                                       //JM aHOME
    SH_BASE_AHOME = !SH_BASE_AHOME;
    //fnInfo(SH_BASE_AHOME);
    fnRefreshComboxState(CB_JC, JC_BASE_AHOME, SH_BASE_AHOME);                  //dr
    break;

  case JC_BASE_MYA:                                         //JM MYa
    SH_BASE_MYA = !SH_BASE_MYA;
    //fnInfo(SH_BASE_MYA);
    fnRefreshComboxState(CB_JC, JC_BASE_MYA, SH_BASE_MYA);                      //dr
    break;
  
  case JC_SH_3T:                                            //JM SH.3T
    Home3TimerMode = !Home3TimerMode;
    //fnInfo(Home3TimerMode);
    fnRefreshComboxState(CB_JC, JC_SH_3T, Home3TimerMode);                      //dr
    break;

  default:
    break;
  }
}



/********************************************//**
 * \brief Set Input_Default
 *
 * \param[in] inputDefault uint16_t
 * \return void
 ***********************************************/
void fnInDefault(uint16_t inputDefault) {
  Input_Default = inputDefault;

  fnRefreshRadioState(RB_ID, inputDefault);
}



/********************************************//**
 * \brief Set Norm_Key_00_VAR
 *
 * \param[in] sigmaAssign uint16_t
 * \return void
 ***********************************************/
void fnSigmaAssign(uint16_t sigmaAssign) {
  Norm_Key_00_VAR = sigmaAssign;

  fnRefreshRadioState(RB_SA, sigmaAssign);
}



/********************************************//**
 * \brief Displays TRUE/FALSE information
 *
 * \param[in] f bool_t
 * \return void
 ***********************************************/
void fnInfo(bool_t f) {
  temporaryInformation = f ? TI_TRUE : TI_FALSE;
  refreshRegisterLine(TAM_REGISTER_LINE);
  refreshRegisterLine(REGISTER_X);
}



/********************************************//**
 * \brief Show flag value
 * \param[in] jmConfig to display uint16_t
 * \return void
 ***********************************************/
void fnShowJM(uint16_t jmConfig) {
  longInteger_t mem;
  longIntegerInit(mem);
  liftStack();

  switch(jmConfig) {
  case JC_ERPN:
    if(eRPN) { stringToLongInteger("1",10,mem); }
    else { stringToLongInteger("0",10,mem); }
    break;
  
  case JC_BASE_HOME:
    if(SH_BASE_HOME) { stringToLongInteger("1",10,mem); }
    else { stringToLongInteger("0",10,mem); }
    break;
  
  case JC_BASE_MYMENU:
    if(SH_BASE_MYMENU) { stringToLongInteger("1",10,mem); }
    else { stringToLongInteger("0",10,mem); }
    break;
  
  case JC_BASE_AHOME:
    if(SH_BASE_AHOME) { stringToLongInteger("1",10,mem); }
    else { stringToLongInteger("0",10,mem); }
    break;
  
  case JC_BASE_MYA:
    if(SH_BASE_MYA) { stringToLongInteger("1",10,mem); }
    else { stringToLongInteger("0",10,mem); }
    break;
  
  case JM_INP_DFLT:
    if(Input_Default == ID_43S) { stringToLongInteger("0",10,mem); }
    else if(Input_Default == ID_SP) { stringToLongInteger("1",10,mem); }
    else if(Input_Default == ID_DP) { stringToLongInteger("2",10,mem); }
    else if(Input_Default == ID_CPXSP) { stringToLongInteger("3",10,mem); }
    else if(Input_Default == ID_CPXDP) { stringToLongInteger("4",10,mem); }
    break;

  default:
    break;
  }

  convertLongIntegerToLongIntegerRegister(mem, REGISTER_X);
  longIntegerFree(mem);

  refreshStack();
}



/********************************************//**
 * \brief Get item-value of assigned key to X
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnGetSigmaAssignToX(uint16_t unusedParamButMandatory) {
  char snum[10];
  longInteger_t mem;
  longIntegerInit(mem);
  liftStack();
  
  itoa(Norm_Key_00_VAR, snum, 10);
  stringToLongInteger(snum,10,mem);

  convertLongIntegerToLongIntegerRegister(mem, REGISTER_X);
  longIntegerFree(mem);

  refreshStack();
}



//JM CONFIGURE USER MODE - ASSIGN KEYS

/********************************************//**
 * \brief 
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnJM_GetXToNORMmode(uint16_t unusedParamButMandatory) {
  int16_t X_REG;
  longInteger_t lgInt;

  if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
    convertLongIntegerRegisterToLongInteger(REGISTER_X, lgInt);
    longIntegerToAllocatedString(lgInt, tmpStr3000, TMP_STR_LENGTH);
    X_REG = longIntegerToInt(lgInt);
    longIntegerFree(lgInt);
  //printf("Xreg %d\n", X_REG);
    Norm_Key_00_VAR = X_REG;
  }
}



/********************************************//**
 * \brief 
 *
 * \param[in] JM_KEY uint16_t
 * \return void
 ***********************************************/
void fnJMUSERmode(uint16_t JM_KEY) {
  int16_t X_REG;
  longInteger_t lgInt;

  if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
    convertLongIntegerRegisterToLongInteger(REGISTER_X, lgInt);
    longIntegerToAllocatedString(lgInt, tmpStr3000, TMP_STR_LENGTH);
    X_REG = longIntegerToInt(lgInt);
    longIntegerFree(lgInt);
  //printf("Xreg %d\n", X_REG);
    if (JM_KEY >= 256) {
      kbd_usr[JM_KEY - 256].primary = X_REG;
    //printf(".primary %d\n", kbd_usr[JM_KEY - 256].primary);
      Show_User_Keys();
    }
  }
}



/********************************************//**
 * \brief 
 *
 * \param[in] JM_KEY uint16_t
 * \return void
 ***********************************************/
void fnJMUSERmode_f(uint16_t JM_KEY) {
  int16_t X_REG;
  longInteger_t lgInt;

  if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
    convertLongIntegerRegisterToLongInteger(REGISTER_X, lgInt);
    longIntegerToAllocatedString(lgInt, tmpStr3000, TMP_STR_LENGTH);
    X_REG = longIntegerToInt(lgInt);
    longIntegerFree(lgInt);
  //printf("Xreg %d\n", X_REG);
    if(JM_KEY >= 256) {
      kbd_usr[JM_KEY - 256].fShifted = X_REG;
    //printf(".fShifted %d\n", kbd_usr[JM_KEY - 256].fShifted);
      Show_User_Keys();
    }
  }
}



/********************************************//**
 * \brief 
 *
 * \param[in] JM_KEY uint16_t
 * \return void
 ***********************************************/
void fnJMUSERmode_g(uint16_t JM_KEY) {
  int16_t X_REG;
  longInteger_t lgInt;

  if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
    convertLongIntegerRegisterToLongInteger(REGISTER_X, lgInt);
    longIntegerToAllocatedString(lgInt, tmpStr3000, TMP_STR_LENGTH);
    X_REG = longIntegerToInt(lgInt);
    longIntegerFree(lgInt);
  //printf("Xreg %d\n", X_REG);
    if(JM_KEY >= 256) {
      kbd_usr[JM_KEY - 256].gShifted = X_REG;
      //printf(".gShifted %d\n", kbd_usr[JM_KEY - 256].gShifted);
      Show_User_Keys();
    }
  }
}



/********************************************//**
 * RPN PROGRAM.
 *
 * \param[in] JM_OPCODE uint16_t
 * \return void
 ***********************************************/
void fnJM(uint16_t JM_OPCODE) {
  uint16_t cm;

  if(JM_OPCODE == 1) {                                          // JM_OPCODE = 1 : Parallel, not using the stack, destroying I, J & K
    saveStack();
                                                                //                    * DO THE PARALLEL FUNCTION XY / (X+Y)
                                                                //                    * Drops X and Y, enables stack lift and refreshes the stack
                                                                //                    * Leaves answer in X and update Last X
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_I);   // STO I
    copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_J);   // STO Y into J
    fnMultiply(0);                                              // *
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_K);   // STO K
    fnDrop(0);                                                  // DROP
    fnRecall(REGISTER_I);                                       // RCL I
    STACK_LIFT_ENABLE;
    fnRecall(REGISTER_J);                                       // RCL J
    fnAdd(0);                                                   // +
    fnRecall(REGISTER_K);                                       // RCL K
    fnSwapXY(0);                                                // X<>Y
    fnDivide(0);                                                // /
    copySourceRegisterToDestRegister(REGISTER_I, REGISTER_L);   // STO I into L (To update LAST X)
    refreshStack();
  }
  else

  if(JM_OPCODE == 2) {                                          // JM_OPCODE = 2 : Angle from complex number.
    saveStack();
                                                                //                    * Using the stack, push once.
                                                                //                    * Leaves angle in X
    cm = complexMode;                                           // STO POLAR MODE
    STACK_LIFT_ENABLE;
/*  btnClicked(NULL, "28");  // 1     // Get unity complex number & multiply with X, for the sole reason to convert X to complex type.
    btnClicked(NULL, "14");  // CHS   //   I do not know how to set up the unity complex vector ootherwise
    btnClicked(NULL, "02");  // sqrt  // Force i
    shiftF = true;           // .     // Force i.i
    shiftG = false;          // .
    Reset_Shift_Mem();       // .
    btnClicked(NULL, "02");  // sq.   // Force -1
    btnClicked(NULL, "14");  // -1    // Force 1
    btnClicked(NULL, "26");  // *.    // Multiply with X  */

    liftStack();                                                // Prepare for new X
//  setRegisterDataType(REGISTER_X, dtComplex16, AM_NONE);      // Convert X to Complex16
    reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);
    stringToReal16("0", REGISTER_REAL16_DATA(REGISTER_X));      // Set X real = 0
    stringToReal16("0", REGISTER_IMAG16_DATA(REGISTER_X));      // Set X imag = 0
    fnAdd(0);                                                   // +
    complexMode = CM_POLAR;                                     // SET POLAR
    fnComplexCCCC(0);                                           // COMPLEX
    fnDropY(0);                                                 // DROP Y
    complexMode = cm;                                           // RCL POLAR MODE
    refreshStack();
  }
  else

  if(JM_OPCODE == 3) {                                          //operator a
    saveStack();
    STACK_LIFT_ENABLE;
    liftStack();
    reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);
    stringToReal16("-0.5", REGISTER_REAL16_DATA(REGISTER_X));
    stringToReal16("0.8660254037844386", REGISTER_IMAG16_DATA(REGISTER_X));   //4676372317075293618347140262690519031402790348972596650845440001854057309
    refreshStack();
  }
  else

  if(JM_OPCODE == 4) {                                          //operater a sq
     saveStack();
     STACK_LIFT_ENABLE;
     liftStack();
     reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);
     stringToReal16("-0.5", REGISTER_REAL16_DATA(REGISTER_X));
     stringToReal16("-0.8660254037844386", REGISTER_IMAG16_DATA(REGISTER_X));
     refreshStack();
  }
  else

  if(JM_OPCODE == 5) {                                          //Operator j
    saveStack();
    STACK_LIFT_ENABLE;
    liftStack();
    reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);
    stringToReal16("0", REGISTER_REAL16_DATA(REGISTER_X));
    stringToReal16("1", REGISTER_IMAG16_DATA(REGISTER_X));
    refreshStack();
  }
  else

  if(JM_OPCODE == 6) {                                          //Delta to Star   ZYX to ZYX; destroys IJKL & 99
    saveStack();
    STACK_LIFT_ENABLE;
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_I);   // STO I
    copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_J);   // STO J
    copySourceRegisterToDestRegister(REGISTER_Z, REGISTER_K);   // STO K
    fnAdd(0);                                                   // +
    fnSwapXY(0);                                                // X<>Y

    fnAdd(0);                                                   // +
    copySourceRegisterToDestRegister(REGISTER_X, 99);           // STO L
    STACK_LIFT_ENABLE;
    fnRecall(REGISTER_K);                                       // RCL I
    STACK_LIFT_ENABLE;
    fnRecall(REGISTER_J);                                       // RCL J     // z = (zx yz) / (x+y+z)
    fnMultiply(0);                                              // *
    fnSwapXY(0);                                                // X<>Y
    fnDivide(0);                                                // /

    STACK_LIFT_ENABLE;
    fnRecall(99);                                               // RCL L
    STACK_LIFT_ENABLE;
    fnRecall(REGISTER_I);                                       // RCL J
    STACK_LIFT_ENABLE;
    fnRecall(REGISTER_J);                                       // RCL K     // y = (xy yz) / (x+y+z)
    fnMultiply(0);                                              // *
    fnSwapXY(0);                                                // X<>Y
    fnDivide(0);                                                // /

    STACK_LIFT_ENABLE;
    fnRecall(99);                                               // RCL L
    STACK_LIFT_ENABLE;
    fnRecall(REGISTER_I);                                       // RCL I
    STACK_LIFT_ENABLE;
    fnRecall(REGISTER_K);                                       // RCL K     // z = (xy zx) / (x+y+z)
    fnMultiply(0);                                              // *
    fnSwapXY(0);                                                // X<>Y
    fnDivide(0);                                                // /

    copySourceRegisterToDestRegister(REGISTER_I, REGISTER_L);   // STO

    temporaryInformation = TI_ABC;
    refreshRegisterLine(REGISTER_X);
    refreshRegisterLine(REGISTER_Y);
    refreshRegisterLine(REGISTER_Z);
  }
  else

  if(JM_OPCODE == 7) {                                          //Star to Delta ZYX to ZYX; destroys IJKL & 99
    saveStack();
    STACK_LIFT_ENABLE;
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_I);   // STO I
    copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_J);   // STO J
    copySourceRegisterToDestRegister(REGISTER_Z, REGISTER_K);   // STO K

    fnMultiply(0);                          //IJ                // *
    fnSwapXY(0);
    STACK_LIFT_ENABLE;
    fnRecall(REGISTER_I);                                       // RCL J
    fnMultiply(0);                          //IK                // *
    fnAdd(0);
    STACK_LIFT_ENABLE;
    fnRecall(REGISTER_J);                                       // RCL J
    STACK_LIFT_ENABLE;
    fnRecall(REGISTER_K);                                       // RCL K
    fnMultiply(0);                          //JK                // *
    fnAdd(0);
    copySourceRegisterToDestRegister(REGISTER_X, 99);  // STO K
                                                                // RCL J    zx = () / y
    STACK_LIFT_ENABLE;
    fnRecall(REGISTER_J);                                       // RCL K
    fnDivide(0);                                                // *

    STACK_LIFT_ENABLE;
    fnRecall(99);                                               // RCL J    yz = () / x
    STACK_LIFT_ENABLE;
    fnRecall(REGISTER_I);                                       // RCL K
    fnDivide(0);                                                // *

    STACK_LIFT_ENABLE;
    fnRecall(99);                                               // RCL J    xy = () / z
    STACK_LIFT_ENABLE;
    fnRecall(REGISTER_K);                                       // RCL K
    fnDivide(0);                                                // *

    copySourceRegisterToDestRegister(REGISTER_I, REGISTER_L);   // STO

    temporaryInformation = TI_ABBCCA;
    refreshRegisterLine(REGISTER_X);
    refreshRegisterLine(REGISTER_Y);
    refreshRegisterLine(REGISTER_Z);
  }
  else

  if(JM_OPCODE == 8) {                                          //SYMMETRICAL COMP to ABC   ZYX to ZYX; destroys IJKL & 99
    saveStack();
    STACK_LIFT_ENABLE;
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_I);   // STO I  //A2
    copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_J);   // STO J  //A1
    copySourceRegisterToDestRegister(REGISTER_Z, REGISTER_K);   // STO K  //A0
    fnAdd(0);                                                   // +
    fnAdd(0);                                                   // + Va = Vao + Va1 +Va2

    STACK_LIFT_ENABLE;
    liftStack();
    reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);
    stringToReal16("-0.5", REGISTER_REAL16_DATA(REGISTER_X));
    stringToReal16("0.8660254037844386", REGISTER_IMAG16_DATA(REGISTER_X));   //4676372317075293618347140262690519031402790348972596650845440001854057309
    refreshStack();
    STACK_LIFT_ENABLE;
    fnRecall(REGISTER_I);                                       // A2
    fnMultiply(0);                                              // * a
    STACK_LIFT_ENABLE;
    liftStack();
    reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);
    stringToReal16("-0.5", REGISTER_REAL16_DATA(REGISTER_X));
    stringToReal16("-0.8660254037844386", REGISTER_IMAG16_DATA(REGISTER_X));   //4676372317075293618347140262690519031402790348972596650845440001854057309
    refreshStack();
    STACK_LIFT_ENABLE;
    fnRecall(REGISTER_J);                                       // A1
    fnMultiply(0);                                              // * aa
    fnAdd(0);                                                   // +
    STACK_LIFT_ENABLE;
    fnRecall(REGISTER_K);                                       // A0
    fnAdd(0);                                                   // + Vb = Vao + aaVa1 +aVa2

    STACK_LIFT_ENABLE;
    liftStack();
    reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);
    stringToReal16("-0.5", REGISTER_REAL16_DATA(REGISTER_X));
    stringToReal16("-0.8660254037844386", REGISTER_IMAG16_DATA(REGISTER_X));   //4676372317075293618347140262690519031402790348972596650845440001854057309
    refreshStack();
    STACK_LIFT_ENABLE;
    fnRecall(REGISTER_I);                                       // A2
    fnMultiply(0);                                              // * a
    STACK_LIFT_ENABLE;
    liftStack();
    reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);
    stringToReal16("-0.5", REGISTER_REAL16_DATA(REGISTER_X));
    stringToReal16("0.8660254037844386", REGISTER_IMAG16_DATA(REGISTER_X));   //4676372317075293618347140262690519031402790348972596650845440001854057309
    refreshStack();
    STACK_LIFT_ENABLE;
    fnRecall(REGISTER_J);                                       // A1
    fnMultiply(0);                                              // * aa
    fnAdd(0);                                                   // +
    STACK_LIFT_ENABLE;
    fnRecall(REGISTER_K);                                       // A0
    fnAdd(0);                                                   // + Vb = Vao + aaVa1 +aVa2

    copySourceRegisterToDestRegister(REGISTER_I, REGISTER_L);   // STO

    temporaryInformation = TI_ABC;
    refreshRegisterLine(REGISTER_X);
    refreshRegisterLine(REGISTER_Y);
    refreshRegisterLine(REGISTER_Z);
  }
  else

  if(JM_OPCODE == 9) {                                          //ABC to SYMMETRICAL COMP   ZYX to ZYX; destroys IJKL & 99
    saveStack();
    STACK_LIFT_ENABLE;
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_I);  // STO I  //c
    copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_J);  // STO J  //b
    copySourceRegisterToDestRegister(REGISTER_Z, REGISTER_K);  // STO K  //a
    fnAdd(0);                                                  // +
    fnAdd(0);                                                  // + Va0 = (Va + Vb +Vc)/3
    liftStack();
    reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);
    stringToReal16("3", REGISTER_REAL16_DATA(REGISTER_X));
    stringToReal16("0", REGISTER_IMAG16_DATA(REGISTER_X));      //4676372317075293618347140262690519031402790348972596650845440001854057309
    refreshStack();
    copySourceRegisterToDestRegister(REGISTER_X, 99);           // STO
    fnDivide(0);


    STACK_LIFT_ENABLE;
    liftStack();
    reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);
    stringToReal16("-0.5", REGISTER_REAL16_DATA(REGISTER_X));
    stringToReal16("0.8660254037844386", REGISTER_IMAG16_DATA(REGISTER_X));   //4676372317075293618347140262690519031402790348972596650845440001854057309
    refreshStack();
    STACK_LIFT_ENABLE;
    fnRecall(REGISTER_J);                                       // VB
    fnMultiply(0);                                              // * a
    STACK_LIFT_ENABLE;
    liftStack();
    reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);
    stringToReal16("-0.5", REGISTER_REAL16_DATA(REGISTER_X));
    stringToReal16("-0.8660254037844386", REGISTER_IMAG16_DATA(REGISTER_X));   //4676372317075293618347140262690519031402790348972596650845440001854057309
    refreshStack();
    STACK_LIFT_ENABLE;
    fnRecall(REGISTER_I);                                       // VC
    fnMultiply(0);                                              // * aa
    fnAdd(0);                                                   // +
    STACK_LIFT_ENABLE;
    fnRecall(REGISTER_K);                                       // VA
    fnAdd(0);                                                   // + V1 = (VA +aVb +aaVc) /3
    fnRecall(99);                                               // 3
    fnDivide(0);                                                // /


    STACK_LIFT_ENABLE;
    liftStack();
    reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);
    stringToReal16("-0.5", REGISTER_REAL16_DATA(REGISTER_X));
    stringToReal16("-0.8660254037844386", REGISTER_IMAG16_DATA(REGISTER_X));   //4676372317075293618347140262690519031402790348972596650845440001854057309
    refreshStack();
    STACK_LIFT_ENABLE;
    fnRecall(REGISTER_J);                                       // VB
    fnMultiply(0);                                              // * a
    STACK_LIFT_ENABLE;
    liftStack();
    reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);
    stringToReal16("-0.5", REGISTER_REAL16_DATA(REGISTER_X));
    stringToReal16("0.8660254037844386", REGISTER_IMAG16_DATA(REGISTER_X));   //4676372317075293618347140262690519031402790348972596650845440001854057309
    refreshStack();
    STACK_LIFT_ENABLE;
    fnRecall(REGISTER_I);                                       // VC
    fnMultiply(0);                                              // * aa
    fnAdd(0);                                                   // +
    STACK_LIFT_ENABLE;
    fnRecall(REGISTER_K);                                       // VA
    fnAdd(0);                                                   // + V1 = (VA +aVb +aaVc) /3
    fnRecall(99);                                               // 3
    fnDivide(0);                                                // /

    copySourceRegisterToDestRegister(REGISTER_I, REGISTER_L);   // STO


    temporaryInformation = TI_012;
    refreshRegisterLine(REGISTER_X);
    refreshRegisterLine(REGISTER_Y);
    refreshRegisterLine(REGISTER_Z);
  }
  else

  if(JM_OPCODE == 10) {                                         //e^theta.j j
    saveStack();
    STACK_LIFT_ENABLE;
    liftStack();
    reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);
    stringToReal16("0", REGISTER_REAL16_DATA(REGISTER_X));
    stringToReal16("1", REGISTER_IMAG16_DATA(REGISTER_X));
    fnMultiply(0);                                              // * aa
    fnExp(0);
    refreshStack();
  }
  else

  if(JM_OPCODE == 11) {                                         //STO Z
    saveStack();
    STACK_LIFT_ENABLE;                                          //  Registers: Z:90-92  V:93-95  I:96-98  XYZ
    copySourceRegisterToDestRegister(REGISTER_X, 90);
    copySourceRegisterToDestRegister(REGISTER_Y, 91);
    copySourceRegisterToDestRegister(REGISTER_Z, 92);
  }
  else

  if(JM_OPCODE == 13) {                                         //STO V
    saveStack();
    STACK_LIFT_ENABLE;                                          //  Registers: Z:90-92  V:93-95  I:96-98  XYZ
    copySourceRegisterToDestRegister(REGISTER_X, 93);
    copySourceRegisterToDestRegister(REGISTER_Y, 94);
    copySourceRegisterToDestRegister(REGISTER_Z, 95);
  }
  else

  if(JM_OPCODE == 15) {                                         //STO I
    saveStack();
    STACK_LIFT_ENABLE;                                          //  Registers: Z:90-92  V:93-95  I:96-98  XYZ
    copySourceRegisterToDestRegister(REGISTER_X, 96);
    copySourceRegisterToDestRegister(REGISTER_Y, 97);
    copySourceRegisterToDestRegister(REGISTER_Z, 98);
  }
  else

  if(JM_OPCODE == 12) {                                         //RCL Z
    saveStack();
    STACK_LIFT_ENABLE;                                          //  Registers: Z:90-92  V:93-95  I:96-98  XYZ
    fnRecall(92);
    fnRecall(91);
    fnRecall(90);
  }
  else

  if(JM_OPCODE == 14) {                                         //RCL V
    saveStack();
    STACK_LIFT_ENABLE;                                          //  Registers: Z:90-92  V:93-95  I:96-98  XYZ
    fnRecall(95);
    fnRecall(94);
    fnRecall(93);
  }
  else

  if(JM_OPCODE == 16) {                                         //RCL I
    saveStack();
    STACK_LIFT_ENABLE;                                          //  Registers: Z:90-92  V:93-95  I:96-98  XYZ
    fnRecall(98);
    fnRecall(97);
    fnRecall(96);
  }
  else

  if(JM_OPCODE == 17) {                                         // V/I
    saveStack();
    STACK_LIFT_ENABLE;                                          //  Registers: Z:90-92  V:93-95  I:96-98  XYZ
    fnRecall(95);
    fnRecall(98);
    fnDivide(0);
    fnRecall(94);
    fnRecall(97);
    fnDivide(0);
    fnRecall(93);
    fnRecall(96);
    fnDivide(0);
    refreshStack();
  }
  else

  if(JM_OPCODE == 18) {                                         // IZ
    saveStack();
    STACK_LIFT_ENABLE;
    fnRecall(98);
    fnRecall(92);
    fnMultiply(0);
    fnRecall(97);
    fnRecall(91);
    fnMultiply(0);
    fnRecall(96);
    fnRecall(91);
    fnMultiply(0);
    refreshStack();
  }
  else

  if(JM_OPCODE == 19) {                                         // V/Z
    saveStack();
    STACK_LIFT_ENABLE;
    fnRecall(95);
    fnRecall(92);
    fnDivide(0);
    fnRecall(94);
    fnRecall(91);
    fnDivide(0);
    fnRecall(93);
    fnRecall(90);
    fnDivide(0);
    refreshStack();
  }
  else

  if(JM_OPCODE == 20) {                                         //Copy Create X>ABC
    saveStack();
    STACK_LIFT_ENABLE;
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_I);

    fnRecall(REGISTER_I);                                       //
    STACK_LIFT_ENABLE;
    liftStack();
    reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);
    stringToReal16("-0.5", REGISTER_REAL16_DATA(REGISTER_X));
    stringToReal16("0.8660254037844386", REGISTER_IMAG16_DATA(REGISTER_X));   //4676372317075293618347140262690519031402790348972596650845440001854057309
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_J);
    fnMultiply(0);

    fnRecall(REGISTER_I);                                       //
    STACK_LIFT_ENABLE;
    liftStack();
    reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, AM_NONE);
    stringToReal16("-0.5", REGISTER_REAL16_DATA(REGISTER_X));
    stringToReal16("-0.8660254037844386", REGISTER_IMAG16_DATA(REGISTER_X));   //4676372317075293618347140262690519031402790348972596650845440001854057309
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_J);
    fnMultiply(0);

    refreshStack();
  }
  else {

  }
}



/********************************************//**
 * \brief CONVERT DATA TYPES UP
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnJMup(uint16_t unusedParamButMandatory) {
  // >>
  /*
  if Angle mode: change to SP or DP as applicable using .d.
  If SHORTINT: change to SP
  if SP: change to DP
  if DP: change to LONGINT
  if ComplexSP change to ComplexDP
  */
  saveStack();
  int32_t dataTypeX = getRegisterDataType(REGISTER_X);

  if((dataTypeX == dtReal16 || dataTypeX == dtReal34) && getRegisterAngularMode(REGISTER_X) != AM_NONE) {
    R_shF(); //shiftF = false;             //JM. Execur .d
    S_shG(); //shiftG = true;              //JM
    Reset_Shift_Mem();          //JM
#ifdef PC_BUILD
    btnClicked(NULL, "03");     //JM changed from 02
#endif
#ifdef DMCP_BUILD
    btnClicked(NULL, "03");     //JM changed from 02
#endif
  }
  else

  if(dataTypeX == dtShortInteger) {
    convertShortIntegerRegisterToReal16Register(REGISTER_X, REGISTER_X);
  }
  else

  if(dataTypeX == dtReal16 || dataTypeX == dtComplex16) {
    fnConvertXToReal34(0);
  }
  else

  if(dataTypeX == dtReal34) {
    JM_convertReal34ToLongInteger(NOT_CONFIRMED);
  }

  refreshStack();
}



/********************************************//**
 * \brief CONVERT DATA TYPES DOWN
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnJMdown(uint16_t unusedParamButMandatory) {
  // <<
  /*
  if Angle mode: change to SP or DP, as applicable using .d
  If LONGINT: change to DP
  if DP: change to SP
  if SP: change to ShortInt
  if ComplexDP change to ComplexSP
  */
  saveStack();
  int32_t dataTypeX = getRegisterDataType(REGISTER_X);

  if((dataTypeX == dtReal16 || dataTypeX == dtReal34) && getRegisterAngularMode(REGISTER_X) != AM_NONE) {
    R_shF(); //shiftF = false;             //JM. Execur .d
    S_shG(); //shiftG = true;              //JM
    Reset_Shift_Mem();          //JM
#ifdef PC_BUILD
    btnClicked(NULL, "03");     //JM changed from 02
#endif
#ifdef DMCP_BUILD
    btnClicked(NULL, "03");     //JM changed from 02
#endif
  }
  else

  if(dataTypeX == dtLongInteger) {
//  fnConvertXToReal34(0);
    convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
  }
  else

  if(dataTypeX == dtReal34 || dataTypeX == dtComplex34) {
    fnConvertXToReal16(0);
  }
  else

  if(dataTypeX == dtReal16) {
    JM_convertReal16ToShortInteger(NOT_CONFIRMED);
  }

  refreshStack();
}



/********************************************//**
 * \brief Sets/resets USER
 *
 * \param[in] jmConfig uint16_t
 * \return void
 ***********************************************/
void fnUserJM(uint16_t jmUser) {
  switch (jmUser) {
  case USER_DEFAULTS:                                           //USER_DEFAULTS FOR USER: E+ CC
    kbd_usr[0].primary     = KEY_CC;
    kbd_usr[0].gShifted    = KEY_TYPCON_UP;
    kbd_usr[0].fShifted    = KEY_TYPCON_DN;
    Norm_Key_00_VAR        = KEY_CC;
    Show_User_Keys();
    break;

  case USER_COMPLEX:                                            //USER_COMPLEX FOR USER: U^ ENTER^ CC
    kbd_usr[12].gShifted   = KEY_CC;
    kbd_usr[0].primary     = -MNU_MyMenu;
    kbd_usr[0].gShifted    = KEY_TYPCON_UP;
    kbd_usr[0].fShifted    = KEY_TYPCON_DN;
    Norm_Key_00_VAR        = -MNU_MyMenu;
    Show_User_Keys();
    break;

  case USER_SHIFTS:                                             //USER_SHIFTS 25          //JM Sectioon to be put on a menu
    kbd_usr[0].primary     = KEY_USERMODE;
    kbd_usr[9].primary     = -MNU_TRI;
    kbd_usr[9].fShifted    = KEY_USERMODE;
    kbd_usr[9].gShifted    = ITM_RTN;
    kbd_usr[10].primary    = KEY_f;
    kbd_usr[10].fShifted   = ITM_NULL;
    kbd_usr[10].gShifted   = ITM_NULL;
    kbd_usr[11].primary    = KEY_g;
    kbd_usr[11].fShifted   = ITM_NULL;
    kbd_usr[11].gShifted   = ITM_NULL;
    Norm_Key_00_VAR        = KEY_USERMODE;
    Show_User_Keys();
    break;

  case USER_RESET:                                              //USER_RESET 26
    memcpy(kbd_usr, kbd_std, sizeof(kbd_std));
    Norm_Key_00_VAR        = ITM_SIGMAPLUS;
    Show_User_Keys();
    break;

/*
  case JM_ASSIGN:                 //A non 0 and non 32766 value means the FN NUMBER is in JM_ASSIGN, AND KEYBOARD.C will wait for a key to be assigned to                                     //USER_RESET 27
    JM_ASN_MODE = KEY_CC;         //TEMPORARY TEST FUNCTION
    break;
*/

  case JM_SEEK_FN:      //32766 in KEYBOARD.C will wait for a key. SEEK FUNCTION,         //USER_RESET 27
    JM_ASN_MODE = 32766;
#ifndef TESTSUITE_BUILD
    clearScreen(false,true,false);
    showString("Select function from keys: EXIT Aborts", &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(REGISTER_Z - REGISTER_X), vmNormal, true, true);
#endif
    break;
  
  default:
    break;
  }
}



void Show_User_Keys(void) {
  userModeEnabled = false;
  toggleUserMode();
}



void fnKEYSELECT(void) {                                        //JM ASSIGN - REMEMBER NEXT KEYBOARD FUNCTION
  if(JM_ASN_MODE == KEY_EXIT || JM_ASN_MODE == KEY_BACKSPACE) {
    JM_ASN_MODE = 0;
#ifndef TESTSUITE_BUILD
    showString("Abandoned or illegal function", &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(REGISTER_Y - REGISTER_X), vmNormal, true, true);
#endif
  }
  else {
#ifndef TESTSUITE_BUILD
    showString("Select key: top 4 lines excl. FN1-6 & [<-],", &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(REGISTER_Y - REGISTER_X), vmNormal, true, true);
    showString("incl. [/] [*] [-] [+] [R/S].   EXIT aborts.", &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(REGISTER_X - REGISTER_X), vmNormal, true, true);
#endif
    userModeEnabled = true;                                     //JM Get out ouf USER MODE to select key in next step
    toggleUserMode();
  }
}



//JM Check if JM ASSIGN IS IN PROGRESS AND CAPTURE THE FUNCTION AND KEY TO BE ASSIGNED
//gets here only after valid function and any key is selected
void fnASSIGN(int16_t JM_ASN_MODE, int16_t tempkey) {           //JM ASSIGN - REMEMBER NEXT KEYBOARD FUNCTION
  switch (tempkey) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 21:
    case 26:
    case 31:
    case 35:
    case 36:

      //JM_convertIntegerToShortIntegerRegister(tempkey, 10, REGISTER_X);
      //JM_convertIntegerToShortIntegerRegister(JM_ASN_MODE, 10, REGISTER_X);
      if(shiftF) {
        (kbd_usr + tempkey)->fShifted = JM_ASN_MODE;  //Assign function into keyboard array
      }
      else if(shiftG) {
        (kbd_usr + tempkey)->gShifted = JM_ASN_MODE;  //Assign function into keyboard array
      }
      else {
        (kbd_usr + tempkey)->primary = JM_ASN_MODE;   //Assign function into keyboard array
      }

      Show_User_Keys();
      refreshStack();
      break;
    default:
#ifndef TESTSUITE_BUILD
        clearScreen(false,true,false);
        showString("Invalid key", &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(REGISTER_Z - REGISTER_X), vmNormal, true, true);
#endif
    break;
  }
}



void JM_convertReal16ToShortInteger(uint16_t confirmation) {
  if(!real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    real16_t x;
    real16ToIntegralValue(REGISTER_REAL16_DATA(REGISTER_X), &x, DEC_ROUND_DOWN);
    real16Subtract(REGISTER_REAL16_DATA(REGISTER_X), &x , &x);
    if(real16IsZero(&x)) { confirmation = CONFIRMED; }
    if(confirmation == NOT_CONFIRMED) {
      setConfirmationMode(JM_convertReal16ToShortInteger);
    }
    else {
      convertReal16ToLongIntegerRegister(REGISTER_REAL16_DATA(REGISTER_X), REGISTER_X, DEC_ROUND_DOWN);

    //setRegisterTag(REGISTER_X,10);
      longInteger_t lgInt;
      convertLongIntegerRegisterToLongInteger(REGISTER_X, lgInt);
      convertLongIntegerToShortIntegerRegister(lgInt, 10, REGISTER_X);
      longIntegerFree(lgInt);

    //convertLongIntegerRegisterToShortIntegerRegister(REGISTER_X, REGISTER_X);
    }
  }
}



void JM_convertReal34ToLongInteger(uint16_t confirmation) {
  if(!real34IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    real34_t x;
    real34ToIntegralValue(REGISTER_REAL34_DATA(REGISTER_X), &x, DEC_ROUND_DOWN);
    real34Subtract(REGISTER_REAL16_DATA(REGISTER_X), &x , &x);
    if(real34IsZero(&x)) { confirmation = CONFIRMED; }
    if(confirmation == NOT_CONFIRMED) {
      setConfirmationMode(JM_convertReal34ToLongInteger);
    }
    else {
      convertReal34ToLongIntegerRegister(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_X, DEC_ROUND_DOWN);
    }
  }
}



void JM_convertIntegerToShortIntegerRegister(int16_t inp, uint32_t base, calcRegister_t destination) {
  char snum[10];
  itoa(inp, snum, base);
  longInteger_t mem;
  longIntegerInit(mem);
  liftStack();
  stringToLongInteger(snum,base,mem);
  convertLongIntegerToShortIntegerRegister(mem, base, destination);
  setRegisterShortIntegerBase(destination, base);
  longIntegerFree(mem);
  refreshStack();
}

/*char snum[7];                                                 //JM  -- PLACE RESULT ON THE STACK
  itoa(determineItem(key), snum, 10);
  longInteger_t mem;
  longIntegerInit(mem);
  liftStack();
  stringToLongInteger(snum,10,mem);
  convertLongIntegerToLongIntegerRegister(mem, REGISTER_X);
  longIntegerFree(mem);
  refreshStack();
*/



/** integer to string
 * C++ version 0.4 char* style "itoa":
 * Written by Lukás Chmela
 * Released under GPLv3.
 */
char* itoa(int value, char* result, int base) {
    // check that the base if valid
    if (base < 2 || base > 16) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}



/* JM UNIT********************************************//**                                                JM UNIT
 * \brief Adds the power of 10 using numeric font to displayString                                        JM UNIT
 *        Converts to units like m, M, k, etc.                                                            JM UNIT
 * \param[out] displayString char*     Result string                                                      JM UNIT
 * \param[in]  exponent int32_t Power of 10 to format                                                     JM UNIT
 * \return void                                                                                           JM UNIT
 ***********************************************                                                          JM UNIT */
void exponentToUnitDisplayString(int32_t exponent, char *displayString, bool_t nimMode) {               //JM UNIT
       if(exponent == -15) { displayString[0] = ' '; displayString[1] = 'f'; displayString[2] = 0; }    //JM UNIT
  else if(exponent == -12) { displayString[0] = ' '; displayString[1] = 'p'; displayString[2] = 0; }    //JM UNIT
  else if(exponent == -9 ) { displayString[0] = ' '; displayString[1] = 'n'; displayString[2] = 0; }    //JM UNIT
  else if(exponent == -6 ) { displayString[0] = ' '; displayString[1] = STD_mu[0]; displayString[2] = STD_mu[1]; displayString[3] = 0; }   //JM UNIT
  else if(exponent == -3 ) { displayString[0] = ' '; displayString[1] = 'm'; displayString[2] = 0; }    //JM UNIT
  else if(exponent ==  3 ) { displayString[0] = ' '; displayString[1] = 'k'; displayString[2] = 0; }    //JM UNIT
  else if(exponent ==  6 ) { displayString[0] = ' '; displayString[1] = 'M'; displayString[2] = 0; }    //JM UNIT
  else if(exponent ==  9 ) { displayString[0] = ' '; displayString[1] = 'G'; displayString[2] = 0; }    //JM UNIT
  else if(exponent == 12 ) { displayString[0] = ' '; displayString[1] = 'T'; displayString[2] = 0; }    //JM UNIT
  else {                                                                                                //JM UNIT
    strcpy(displayString, PRODUCT_SIGN);                                                                //JM UNIT Below, copy of
    displayString += 2;                                                                                 //JM UNIT exponentToDisplayString in display.c
    strcpy(displayString, STD_SUB_10);                                                                  //JM UNIT
    displayString += 2;                                                                                 //JM UNIT
    displayString[0] = 0;                                                                               //JM UNIT
    if(nimMode) {                                                                                       //JM UNIT
      if(exponent != 0) {                                                                               //JM UNIT
        supNumberToDisplayString(exponent, displayString, NULL, false);                                       //JM UNIT
      }                                                                                                 //JM UNIT
    }                                                                                                   //JM UNIT
    else {                                                                                              //JM UNIT
      supNumberToDisplayString(exponent, displayString, NULL, false);                                         //JM UNIT
    }                                                                                                   //JM UNIT
  }                                                                                                     //JM UNIT
}                                                                                                       //JM UNIT



//*********************

//JM\/\/\/\/

bool_t userModeEnabledMEM;



/********************************************//**
 * \brief 
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnComplexCCCC_CPX(uint16_t unusedParamButMandatory) {      //JM HARDWAIRED FOR f[COMPLEX]
  userModeEnabledMEM = userModeEnabled;
  userModeEnabled = false;

#ifdef JM_LAYOUT_1A               //JM LAYOUT 1A. CHALLENGE.
  S_shF(); //shiftF = true;                  //JM
  R_shG(); //shiftG = false;                 //JM
  Reset_Shift_Mem();              //JM
  #ifdef PC_BUILD
  btnClicked(NULL, "12");         //JM changed from 02
  #endif
  #ifdef DMCP_BUILD
  btnClicked(NULL, "12");         //JM changed from 02
  #endif
#endif

#ifdef JM_LAYOUT_2_DM42_STRICT    //JM LAYOUT 2. DM42 STRICT.
  S_shF(); //shiftF = true;                  //JM
  R_shG(); //shiftG = false;                 //JM
  Reset_Shift_Mem();              //JM
  #ifdef PC_BUILD
  btnClicked(NULL, "06");         //JM changed from 02
  #endif

  #ifdef DMCP_BUILD
  btnClicked(NULL, "06");         //JM changed from 02
  #endif
#endif
  userModeEnabled = userModeEnabledMEM;
}



/********************************************//**
 * \brief 
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnComplexCCCC_CC1(uint16_t unusedParamButMandatory) {      //FOR CC1  HARDWIRED FOR TOP LEFT BUTTON
  userModeEnabledMEM = userModeEnabled;
  userModeEnabled = true;
  S_shF(); //shiftF = true;                  //JM
  R_shG(); //shiftG = false;                 //JM
  Reset_Shift_Mem();              //JM
#ifdef PC_BUILD
  btnClicked(NULL, "00");         //JM changed from 02
#endif
#ifdef DMCP_BUILD
  btnClicked(NULL, "00");         //JM changed from 02
#endif
  userModeEnabled = userModeEnabledMEM;
}



/********************************************//**
 * \brief 
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnComplexCCCC_CC(uint16_t unusedParamButMandatory) {       //FOR CC  HARDWIRED FOR TOP LEFT BUTTON
  userModeEnabledMEM = userModeEnabled;
  userModeEnabled = true;
  R_shF(); //shiftF = false;                  //JM
  R_shG(); //shiftG = false;                 //JM
  Reset_Shift_Mem();              //JM
  #ifdef PC_BUILD
    btnClicked(NULL, "00");       //JM changed from 02
  #endif
  #ifdef DMCP_BUILD
    btnClicked(NULL, "00");       //JM changed from 02
  #endif
  userModeEnabled = userModeEnabledMEM;
}
//JM^^^^^^^



/*
void ItemBrowser(uint16_t unusedParamButMandatory) {
  int16_t registerNameWidth;

  if(currentRegisterBrowserScreen == 9999) { // Init
    currentRegisterBrowserScreen = 0;
    rbrMode = RBR_GLOBAL;
    showContent = true;
    rbr1stDigit = true;
    calcMode = CM_REGISTER_BROWSER;
  }

  if(currentRegisterBrowserScreen < 9999) {
    clearScreen(false, true, true);

    int16_t regist;
    for(int16_t row=0; row<10; row++) {
      regist = (currentRegisterBrowserScreen + row); // % FIRST_LOCAL_REGISTER;

      itoa(regist, tmpStr3000, 10);
      registerNameWidth = showString(tmpStr3000, &standardFont, 1, 219-22*row, vmNormal, false, true);

      strcpy(tmpStr3000, "'");
      strcpy(tmpStr3000, indexOfItems[regist].itemPrinted);
      strcat(tmpStr3000, "'");
      if(stringWidth(tmpStr3000, &standardFont, false, true) >= SCREEN_WIDTH - 12 - registerNameWidth) { // 12 is the width of STD_ELLIPSIS
        tmpStr3000[stringLastGlyph(tmpStr3000)] = 0;
        while(stringWidth(tmpStr3000, &standardFont, false, true) >= SCREEN_WIDTH - 12 - registerNameWidth) { // 12 is the width of STD_ELLIPSIS
          tmpStr3000[stringLastGlyph(tmpStr3000)] = 0;
        }
        strcat(tmpStr3000 + stringByteLength(tmpStr3000), STD_ELLIPSIS);
      }
      showString(tmpStr3000, &standardFont, SCREEN_WIDTH - stringWidth(tmpStr3000, &standardFont, false, true) - 1, 219-22*row, vmNormal, false, true);
    }
  }
}
*/

