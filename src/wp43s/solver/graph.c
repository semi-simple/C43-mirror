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
 * along with 43S.  If not, see <htempp://www.gnu.org/licenses/>.
 */

/********************************************//** //JM
 * \file graph.c Graphing module
 ***********************************************/

#include "solver/graph.h"

#include "c43Extensions/jm.h"

#include "defines.h"
#include "mathematics/comparisonReals.h"
#include "constantPointers.h"
#include "solver/equation.h"
#include "flags.h"
#include "items.h"
#include "mathematics/invert.h"
#include "plotstat.h"
#include "registers.h"
#include "registerValueConversions.h"
#include "screen.h"
#include "statusBar.h"
#include "stack.h"
#include "stats.h"
#include "store.h"
#include "wp43s.h"
#include <math.h>

//Verbose directives can be simulataneously selected
#define VERBOSE_SOLVER00   // minimal text
#define VERBOSE_SOLVER0  // a lot less text
//#define VERBOSE_SOLVER1  // a lot less text
//#define VERBOSE_SOLVER2  // verbose a lot




static void execute_rpn_function(void){
  calcRegister_t regStats = findNamedVariable("xx");
  if(regStats != INVALID_VARIABLE) {
    fnStore(regStats);
    fnEqCalc(0);
    fnRCL(regStats);
    #if (defined VERBOSE_SOLVER0) && (defined PC_BUILD)
      printRegisterToConsole(REGISTER_X,">>> Solving xx=","");
      printRegisterToConsole(REGISTER_Y," f(xx)=","\n");
    #endif
  }
}



static void graph_eqn(float x_min, float x_max) {
  #ifndef TESTSUITE_BUILD
  double x; 
  runFunction(ITM_CLSTK);
  runFunction(ITM_RAD);

  for(x=x_min; x<=x_max; x+=0.99999*(x_max-x_min)/SCREEN_WIDTH_GRAPH*10) {    //Reduced the amount of sample data from 400 points to 40 points

    //convert double to X register
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    snprintf(tmpString, TMP_STR_LENGTH, "%.16e", x);
    stringToReal34(tmpString, REGISTER_REAL34_DATA(REGISTER_X));

    //leaving y in Y and x in X
    execute_rpn_function();
   	runFunction(ITM_SIGMAPLUS);
    #ifdef PC_BUILD
      if(lastErrorCode == 24) { printf("ERROR CODE CANNOT STAT COMPLEX RESULT, ignored\n"); lastErrorCode = 0;}
    #endif
  }
  runFunction(ITM_CLSTK);
  runFunction(ITM_SIGMAx);
  runFunction(ITM_PLOT_XY);
  #endif
}




static void doubleToXRegisterReal34(double x) { //Convert from double to X register REAL34
  setSystemFlag(FLAG_ASLIFT);
  liftStack();
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
  snprintf(tmpString, TMP_STR_LENGTH, "%.16e", x);
  stringToReal34(tmpString, REGISTER_REAL34_DATA(REGISTER_X));
  //adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
  setSystemFlag(FLAG_ASLIFT);
}




static double convert_to_double(calcRegister_t regist) { //Convert from X register to double
  double y;
  real_t tmpy;
  switch(getRegisterDataType(regist)) {
  case dtLongInteger:
    convertLongIntegerRegisterToReal(regist, &tmpy, &ctxtReal39);
    break;
  case dtReal34:
    real34ToReal(REGISTER_REAL34_DATA(regist), &tmpy);
    break;
  default:
    return 0;
    break;
  }
  realToString(&tmpy, tmpString);
  y = strtof(tmpString, NULL);
  return y;
}




int16_t osc = 0;
uint8_t DXR = 0, DYR = 0, DXI = 0, DYI = 0;


void check_osc(uint8_t ii){
   switch (ii & 0b00111111) {
     case 0b001111:
     case 0b011110:
     case 0b111100:
     case 0b010101:
     case 0b101010:

     case 0b011011:
     case 0b110110:
     case 0b101101: osc++;
     default:;
   }
   switch (ii) {
     case 0b01001001:
     case 0b10010010: osc++;
     default:;
   }
}


//###################################################################################
//SOLVER
static void graph_solver(float x_min, float x_max) {
#ifndef SAVE_SPACE_DM42_20
  #ifndef TESTSUITE_BUILD
  int16_t ix,ixd;
  int16_t oscillations = 0; 
  int16_t convergent = 0; 
  bool_t checkNaN = false;
  bool_t checkzero = false;
  osc = 0;
  DXR = 0, DYR = 0, DXI = 0, DYI = 0;

  fnStrInputReal34("1.0");
  runFunction(ITM_MULT);     //Convert to REAL
  #if (defined VERBOSE_SOLVER1) && (defined PC_BUILD)
    printRegisterToConsole(REGISTER_X,"XREG START: ","\n");
  #endif //VERBOSE_SOLVER1

  // Initialize all temporary registers
  // Registers 85 to 97; 98 and 99 are being used in the DEMO data programs
  calcRegister_t SREG_STARTX = 82;
  copySourceRegisterToDestRegister(REGISTER_X,SREG_STARTX);
  runFunction(ITM_TICKS);
  calcRegister_t SREG_TICKS = 83; fnStore(SREG_TICKS);
  doubleToXRegisterReal34(0.0);
  calcRegister_t SREG_TMP  = 84; fnStore(SREG_TMP);
  calcRegister_t SREG_Xold = 85; fnStore(SREG_Xold);   //85: x old difference
  calcRegister_t SREG_Yold = 86; fnStore(SREG_Yold);   //86: y old difference
  calcRegister_t SREG_Y0   = 87; fnStore(SREG_Y0);     //87: y0
  calcRegister_t SREG_X0   = 88; fnStore(SREG_X0);     //88: x0    //x0 stored but noever recalled
  calcRegister_t SREG_X1   = 89; fnStore(SREG_X1);     //90: x1
  calcRegister_t SREG_X2   = 90; fnStore(SREG_X2);     //91: x2
  calcRegister_t SREG_Y1   = 91; fnStore(SREG_Y1);     //92: y1
  calcRegister_t SREG_Y2   = 92; fnStore(SREG_Y2);     //93: y2
  calcRegister_t SREG_X2N  = 93; fnStore(SREG_X2N);    //94: temporary new x2
  calcRegister_t SREG_DX   = 94;                       //95: x difference
  calcRegister_t SREG_DY   = 95;                       //96: y difference
  calcRegister_t SREG_F    = 96;                       //97: faxctor
  calcRegister_t SREG_TOL  = 97;                       //98: tolerance
  doubleToXRegisterReal34(1.0);
  fnStore(SREG_F);                                     // factor
  fnStore(SREG_DX);                                    // initial value for difference comparison must be larger than tolerance
  fnStore(SREG_DY);                                    // initial value for difference comparison must be larger than tolerance
  doubleToXRegisterReal34(1E-30); 
  fnStore(SREG_TOL);                                   // tolerance

  fnRCL(SREG_STARTX);                                  // leftmost graph value
  fnStore(SREG_X1);                                    // initial x1   
  execute_rpn_function();                              // leaving y in y
  copySourceRegisterToDestRegister(REGISTER_Y,SREG_Y1);   // initial y1

  double startval = 1; 
  startval = convert_to_double(SREG_STARTX);    //********** X REGISTER IS STARTING POINT
  doubleToXRegisterReal34(startval + (x_max-x_min) / SCREEN_WIDTH_GRAPH);  //first auto increment is 1 pixel width on the graph
  fnStore(SREG_X2);                                    // initial x2

  ix = 0; ixd = 0;
  int16_t kicker = 0;

  #if (defined VERBOSE_SOLVER2) && (defined PC_BUILD)
    printf("INIT:   ix=%d ",ix); 
    printRegisterToConsole(SREG_X1,"X1= ","\n");
    printRegisterToConsole(SREG_X2,"X2= ","\n");
  #endif //VERBOSE_SOLVER2


  //Start STAT/GRAPH memory
  runFunction(ITM_CLSTK);
  runFunction(ITM_RAD);




//###############################################################################################################
//#################################################### Iteration start ##########################################
  while(ix<LIM && !checkNaN && !checkzero) 
  {
    #ifdef PC_BUILD
      if(lastErrorCode != 0) { 
        printf(">>>> ERROR CODE INITIALLY NON-ZERO = %d <<<<<\n",lastErrorCode); 
        return;
      }
    #endif

    //assumes X2 is in R91
    //Identify oscillations in real or imag: increment osc flag
//    osc = 0;

    if( (real34IsNegative(REGISTER_REAL34_DATA(SREG_DX)) && real34IsPositive(REGISTER_REAL34_DATA(SREG_Xold))) ||
        (real34IsPositive(REGISTER_REAL34_DATA(SREG_DX)) && real34IsNegative(REGISTER_REAL34_DATA(SREG_Xold))) ) {
        DXR = (DXR << 1) + 1;       
    } else DXR = DXR << 1;

    if( (real34IsNegative(REGISTER_REAL34_DATA(SREG_DY)) && real34IsPositive(REGISTER_REAL34_DATA(SREG_Yold))) ||
        (real34IsPositive(REGISTER_REAL34_DATA(SREG_DY)) && real34IsNegative(REGISTER_REAL34_DATA(SREG_Yold))) ) {
        DYR = (DYR << 1) + 1;       
    } else DYR = DYR << 1;
    
    if((getRegisterDataType(SREG_DX) == dtComplex34 && getRegisterDataType(SREG_Xold) == dtComplex34) &&
         ((real34IsNegative(REGISTER_IMAG34_DATA(SREG_DX)) && real34IsPositive(REGISTER_IMAG34_DATA(SREG_Xold))) ||
          (real34IsPositive(REGISTER_IMAG34_DATA(SREG_DX)) && real34IsNegative(REGISTER_IMAG34_DATA(SREG_Xold))) )) {
             DXI = (DXI << 1) + 1;       
    } else DXI = DXI << 1;
    
    if((getRegisterDataType(SREG_DY) == dtComplex34 && getRegisterDataType(SREG_Yold) == dtComplex34) &&
         ((real34IsNegative(REGISTER_IMAG34_DATA(SREG_DY)) && real34IsPositive(REGISTER_IMAG34_DATA(SREG_Yold))) ||
          (real34IsPositive(REGISTER_IMAG34_DATA(SREG_DY)) && real34IsNegative(REGISTER_IMAG34_DATA(SREG_Yold))) )) {
             DYI = (DYI << 1) + 1;       
    } else DYI = DYI << 1;

   check_osc(DXR);
   check_osc(DYR);
   check_osc(DXI);
   check_osc(DYI);

    //If osc flag is active, that is any delta polarity change, then increment oscillation count
    if(osc > 0) oscillations++; else
      oscillations = max(0,oscillations-1);


    //If converging, increment convergence counter
    if((!real34CompareAbsGreaterThan(REGISTER_REAL34_DATA(SREG_DX), REGISTER_REAL34_DATA(SREG_Xold)) && 
       (getRegisterDataType(SREG_DX) == dtComplex34 && getRegisterDataType(SREG_Xold) == dtComplex34 ? 
         !real34CompareAbsGreaterThan(REGISTER_IMAG34_DATA(SREG_DX), REGISTER_IMAG34_DATA(SREG_Xold)) : true))

       &&

       (!real34CompareAbsGreaterThan(REGISTER_REAL34_DATA(SREG_DY), REGISTER_REAL34_DATA(SREG_Yold)) &&
       (getRegisterDataType(SREG_DY) == dtComplex34 && getRegisterDataType(SREG_Yold) == dtComplex34 ?
         !real34CompareAbsGreaterThan(REGISTER_IMAG34_DATA(SREG_DY), REGISTER_IMAG34_DATA(SREG_Yold)) : true))
            ) {
        convergent++;
    } else {
      convergent = max(0,convergent-1);
    }

    #ifdef VERBOSE_SOLVER0
      printf("##### ix= %d osc= %d  conv= %d n",ix, oscillations, convergent);
    #endif //VERBOSE_SOLVER1
    #ifdef VERBOSE_SOLVER1
      printf("################################### ix= %d osc= %d  conv= %d ###########################################\n",ix, oscillations, convergent);
    #endif //VERBOSE_SOLVER1

    if(convergent > 6 && oscillations > 3) {
      convergent = 2;
      oscillations = 2;
      #ifdef PC_BUILD
        printf(">>> reset conv & oscillation\n");
      #endif
    }

// If increment is oscillating it is assumed that it is unstable and needs to have a complex starting value
    //assumes X2 is in R91
    if( (((oscillations >= 3) && (ixd > 9) && (convergent <= 2) )
       //|| (oscillations == 0 && convergent > 6 && real34CompareAbsLessThan(REGISTER_REAL34_DATA(SREG_DX), const34_1e_4) && (getRegisterDataType(SREG_DX) == dtComplex34 ? real34CompareAbsLessThan(REGISTER_IMAG34_DATA(SREG_DX), const34_1e_4) : 1 )  )

    )) {
      if( (kicker ==0) && (convergent <= 1) ) kicker = kicker +2;
      ixd = 0;
      oscillations = 0;
      convergent = 0;
      #if (defined VERBOSE_SOLVER2) && (defined PC_BUILD)
        printRegisterToConsole(SREG_X2,"\n>>>>>>>>>> from ","");
      #endif //VERBOSE_SOLVER2
      fnRCL(SREG_X2);

      //when kicker = 0, then factor is small negative real
      //after that, it is complex, in the first quardrant, multplied by a alrger number every time
      doubleToXRegisterReal34(  - (kicker +0.001) / 100.0);
      if(kicker > 0) {
        runFunction(ITM_SQUAREROOTX);
        doubleToXRegisterReal34(    (kicker+0.001) / 100.0);
        runFunction(ITM_SQUAREROOTX);
        runFunction(ITM_ADD);
        doubleToXRegisterReal34(pow(-2.0,kicker));
        runFunction(ITM_MULT);
      }

      #if ((defined VERBOSE_SOLVER00) || (defined VERBOSE_SOLVER0)) && (defined PC_BUILD)
        printf("------- Kicked oscillation, #%d, ", kicker);
        printRegisterToConsole(REGISTER_X," multiplied: ","\n");
      #endif  //VERBOSE_SOLVER1

      kicker++;

      runFunction(ITM_MULT);              //add just to force it complex  //
      fnStore(SREG_X2); //replace X2 value                                //

      #if (defined VERBOSE_SOLVER2) && (defined PC_BUILD)
        printRegisterToConsole(SREG_X2," to ","\n");
      #endif //VERBOSE_SOLVER2
    }


//@@@@@@@@@@@@@@@@@ CALCULATE NEW Y2, AND PLAUSIBILITY @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    fnRCL(SREG_X2);                                       // get (X2,Y2)
    execute_rpn_function();                               // leaving y2 in Y and x2 in X
    copySourceRegisterToDestRegister(REGISTER_Y,SREG_Y2); // y2

    #if (defined VERBOSE_SOLVER2) && (defined PC_BUILD)
      printf("    :   ix=%d",ix);
      printRegisterToConsole(SREG_X2," X2="," ");
      printRegisterToConsole(SREG_Y2," Y2=","\n");
    #endif //VERBOSE_SOLVER2

    // y2 in Y and x2 in X
    checkzero = checkzero || ( real34IsZero(REGISTER_REAL34_DATA(SREG_Y2)) && (getRegisterDataType(SREG_Y2) == dtComplex34 ? real34IsZero(REGISTER_IMAG34_DATA(SREG_Y2)) : 1 ) );
    checkNaN  = checkNaN  ||   real34IsNaN(REGISTER_REAL34_DATA(SREG_X2)) || (getRegisterDataType(SREG_X2) == dtComplex34 ? real34IsNaN(REGISTER_IMAG34_DATA(SREG_X2)) : 0 ) ||
                               real34IsNaN(REGISTER_REAL34_DATA(SREG_Y2)) || (getRegisterDataType(SREG_Y2) == dtComplex34 ? real34IsNaN(REGISTER_IMAG34_DATA(SREG_Y2)) : 0 ) ; 
    #ifdef VERBOSE_SOLVER1
    if(checkNaN || ix==LIM || checkzero) {
       printf("-->A Endflags zero: Y2r:%u Y2i:%u NaN: X2r:%u X2i:%u Y2r:%u Y2i%u \n",
        (uint16_t)real34IsZero(REGISTER_REAL34_DATA(SREG_Y2)),(uint16_t)real34IsZero(REGISTER_IMAG34_DATA(SREG_Y2)),
        (uint16_t)real34IsNaN (REGISTER_REAL34_DATA(SREG_X2)),(uint16_t)real34IsNaN (REGISTER_IMAG34_DATA(SREG_X2)),
        (uint16_t)real34IsNaN (REGISTER_REAL34_DATA(SREG_Y2)),(uint16_t)real34IsNaN (REGISTER_IMAG34_DATA(SREG_Y2))
        );
    }
    #endif //VERBOSE_SOLVER1
    #if (defined VERBOSE_SOLVER2) && (defined PC_BUILD)
      printf("   ix=%d checkend=%d X2=",ix, checkNaN || ix==LIM || checkzero); 
      printRegisterToConsole(SREG_X2,"","");
      printRegisterToConsole(SREG_Y2,"Y2=","\n");
    #endif //VERBOSE_SOLVER2

    if(checkzero || checkNaN) goto EndIteration;
//*************** DETERMINE DX and DY, to calculate the slope (or the inverse of the slope in this case) *******************






    copySourceRegisterToDestRegister(SREG_DX,SREG_Xold);  // store old DELTA values, for oscillation check
    copySourceRegisterToDestRegister(SREG_DY,SREG_Yold);  // store old DELTA values, for oscillation check

if(  (ix <=2)) {


    //Secant and Newton approximation methods
        if( !(ix>2 && convergent >= 2 && oscillations < 2) ) {

        //###########################
        //  normal Secant, 2-sample slope
        //  DX = X2 - X1 in YREGISTER
        //  DY = Y2 - Y1 in XREGISTER
        //###########################

          #if (defined VERBOSE_SOLVER00) || (defined VERBOSE_SOLVER0) || (defined VERBOSE_SOLVER1) || (defined VERBOSE_SOLVER2)
            printf("%3i ---------- Using normal Secant dydx 2-samples -\n",ix);
          #endif          	
	          fnRCL  (SREG_X2); fnRCL(SREG_X1); runFunction(ITM_SUB);      // dx
	          fnStore(SREG_DX);                                     // store difference for later
	          fnRCL  (SREG_Y2); fnRCL(SREG_Y1); runFunction(ITM_SUB);      // dy
	          fnStore(SREG_DY);                                     // store difference for later
	          //Leave DX in YREG, and DY in XREG, so DX/DY can be computed
	        
          }

        else {

        //###########################
        // normal secant with 3 sample slope
        //  DX = X2 - X1 in YREGISTER
        //  DY = Y2 - Y1 in XREGISTER
        //###########################
	          //  The second order accurate one-sided finite difference formula for the first derivative, formule 32, of
	          //  ChE 205 — Formulas for Numerical Differentiation
	          //  Handout 5 05/08/02:
	        
          #if (defined VERBOSE_SOLVER00) || (defined VERBOSE_SOLVER0) || (defined VERBOSE_SOLVER1) || (defined VERBOSE_SOLVER2)
            printf("%3i ---------- Using Secant with 3 samples dy/dx --\n",ix);
          #endif
	          fnRCL      (SREG_X2); fnRCL(SREG_X1); runFunction(ITM_SUB); //Determine x2-x1
	          fnStore    (SREG_DX);  //store difference DX for later
	          fnRCL      (SREG_X1);
	          runFunction(ITM_XexY);
	          runFunction(ITM_SUB);
	          fnStore    (SREG_X0);          //determine the new x0 by subtracting DX
	          execute_rpn_function(); //determine the new f(x0)
	          copySourceRegisterToDestRegister(REGISTER_Y,SREG_Y0); //set y0 to the result f(x0)
	          //do DX = 2 (x2-x1)
	          fnRCL      (SREG_DX);
	          doubleToXRegisterReal34(2);//calculate 2(x2-x1)
	          runFunction(ITM_MULT);             // DX = 2 delta x
	          //do DY = (fi−2 − 4fi−1 + 3fi)
	          fnRCL      (SREG_Y0);              //y0
	          fnRCL      (SREG_Y1);
	          doubleToXRegisterReal34(4); 
	          runFunction(ITM_MULT);
	          runFunction(ITM_SUB);               //-4.y1
	          fnRCL      (SREG_Y2);
	          doubleToXRegisterReal34(3);
	          runFunction(ITM_MULT); 
	          runFunction(ITM_ADD);                    //+3.y2
            fnStore    (SREG_DY);
	        /*-3-sample slope-*/  //Leave DX in YREG, and DY in XREG, so DX/DY can be computed
        }

      //###########################
      //  Start with DX and DY
      //  
      //  
      //###########################
        runFunction(ITM_DIV);  //get the inverse of the slope, dx/dy
        if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || (getRegisterDataType(REGISTER_X) == dtComplex34 ? real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X)) : 0 ) ) 
          doubleToXRegisterReal34(0);

        #if (defined VERBOSE_SOLVER2) && (defined PC_BUILD)
          fnInvert(0);
          printRegisterToConsole(REGISTER_X," SLOPE=","\n");
          fnInvert(0); 
        #endif

        fnRCL(SREG_Y1);      // determine increment in x
        runFunction(ITM_MULT);       // increment to x is: y1 . DX/DY
        fnRCL(SREG_F);       // factor to stabilize Newton method. factor=1 is straight. factor=0.1 converges 10x slower.
        runFunction(ITM_MULT);       // increment to x

        #if (defined VERBOSE_SOLVER1) && (defined PC_BUILD)
          printRegisterToConsole(SREG_F,"    Factor=        ","\n");
          printRegisterToConsole(SREG_X1,"    New X =        "," - ");
          printRegisterToConsole(REGISTER_X," - (",")\n");
        #endif

        fnRCL(SREG_X1);
        runFunction(ITM_XexY);
        runFunction(ITM_SUB);       // subtract as per Newton, x1 - f/f'
        fnStore(SREG_X2N);          // store temporarily to new x2n
} else





  {
    #if (defined VERBOSE_SOLVER00) || (defined VERBOSE_SOLVER0) || (defined VERBOSE_SOLVER1) || (defined VERBOSE_SOLVER2)
    printf("%3i ---------- Modified 3 point Secant ------------\n",ix);
    #endif //VERBOSE_SOLVER1
    //YREG
    fnRCL(SREG_Y2);fnRCL(SREG_Y1);runFunction(ITM_SUB);fnStore(SREG_DY);

    //XREG
    fnRCL(SREG_Y2);fnRCL(SREG_Y0);runFunction(ITM_SUB);
    fnRCL(SREG_X2);fnRCL(SREG_X0);runFunction(ITM_SUB);runFunction(ITM_DIV);fnStore(SREG_TMP);fnStore(SREG_DX);
    fnRCL(SREG_Y2);fnRCL(SREG_Y1);runFunction(ITM_SUB);runFunction(ITM_MULT);

    fnRCL(SREG_Y2);
    fnRCL(SREG_TMP);
    fnRCL(SREG_Y1);fnRCL(SREG_Y0);runFunction(ITM_SUB);
    fnRCL(SREG_X1);fnRCL(SREG_X0);runFunction(ITM_SUB);runFunction(ITM_DIV);
    runFunction(ITM_SUB);runFunction(ITM_MULT);

    runFunction(ITM_SUB);

    runFunction(ITM_DIV);  //get the slope, dy/dx
    if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || (getRegisterDataType(REGISTER_X) == dtComplex34 ? real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X)) : 0 ) ) 
      doubleToXRegisterReal34(0);



    #if (defined VERBOSE_SOLVER2) && (defined PC_BUILD)
      printRegisterToConsole(REGISTER_X," SLOPE=","\n");
    #endif //VERBOSE_SOLVER2

    fnRCL(SREG_Y0);              // determine increment in x
    runFunction(ITM_MULT);       // increment to x is: y1 . DX/DY
    fnRCL(SREG_F);               // factor to stabilize Newton method. factor=1 is straight. factor=0.1 converges 10x slower.
    runFunction(ITM_MULT);       // increment to x

    #if (defined VERBOSE_SOLVER1) && (defined PC_BUILD)
      printRegisterToConsole(SREG_F,"    Factor=        ","\n");
      printRegisterToConsole(SREG_X0,"    New X =        "," - (");
      printRegisterToConsole(REGISTER_X,"",")\n");
    #endif //VERBOSE_SOLVER1

    fnRCL(SREG_X0);
    runFunction(ITM_XexY);
    runFunction(ITM_SUB);        // subtract as per Newton, x1 - f/f'
    fnStore(SREG_X2N);           // store temporarily to new x2n
    }

    copySourceRegisterToDestRegister(SREG_Y1,SREG_Y0); //old y1 copied to y0
    copySourceRegisterToDestRegister(SREG_X1,SREG_X0); //old x1 copied to x0



  //Experimental bisection method to kick
  bool_t bisect = false;
   if(   ( (getRegisterDataType(SREG_Y0) == dtReal34) && 
           (getRegisterDataType(SREG_Y2) == dtReal34) && 
           (real34CompareAbsGreaterThan(REGISTER_REAL34_DATA(SREG_Y0),const34_1e_6)) && 
           (real34CompareAbsGreaterThan(REGISTER_REAL34_DATA(SREG_Y2),const34_1e_6)) &&
            (
              ( (real34IsNegative(REGISTER_REAL34_DATA(SREG_Y0))) && (real34IsPositive(REGISTER_REAL34_DATA(SREG_Y2))) ) ||
              ( (real34IsNegative(REGISTER_REAL34_DATA(SREG_Y2))) && (real34IsPositive(REGISTER_REAL34_DATA(SREG_Y0))) )
            ) &&

    	   !((real34CompareGreaterEqual(REGISTER_REAL34_DATA(SREG_X2N),REGISTER_REAL34_DATA(SREG_X0)) &&
    		 real34CompareGreaterEqual(REGISTER_REAL34_DATA(SREG_X2),REGISTER_REAL34_DATA(SREG_X2N)) ) ||
    		 (real34CompareLessEqual(REGISTER_REAL34_DATA(SREG_X2N),REGISTER_REAL34_DATA(SREG_X0)) &&
    		 real34CompareLessEqual(REGISTER_REAL34_DATA(SREG_X2),REGISTER_REAL34_DATA(SREG_X2N)) ) )

        ) 
      ) { bisect = true;
          #if (defined VERBOSE_SOLVER00) || (defined VERBOSE_SOLVER0) || (defined VERBOSE_SOLVER1) || (defined VERBOSE_SOLVER2)
            printf(" Using Bisection method: Y bracketed\n");
          #endif
        }
  if(bisect){
    fnRCL(SREG_X0);
    fnRCL(SREG_X2);
    runFunction(ITM_ADD);
    doubleToXRegisterReal34(2);
    runFunction(ITM_DIV); //Leaving (x1+x2)/2
    fnStore(SREG_X2N);   // store temporarily to new x2n
  }
//#############################################


    copySourceRegisterToDestRegister(SREG_Y2,SREG_Y1); //old y2 copied to y1
    copySourceRegisterToDestRegister(SREG_X2,SREG_X1); //old x2 copied to x1

    fnRCL(SREG_DY);   runFunction(ITM_ABS); //difference |dy| is in Y
    fnRCL(SREG_DX);   runFunction(ITM_ABS); //difference |dx| is in X


EndIteration:

    checkzero = checkzero || 
                ( real34IsZero(REGISTER_REAL34_DATA(REGISTER_X)) && (getRegisterDataType(REGISTER_X) == dtComplex34 ? real34IsZero(REGISTER_IMAG34_DATA(REGISTER_X)) : 1 )  &&
                  real34IsZero(REGISTER_REAL34_DATA(REGISTER_Y)) && (getRegisterDataType(REGISTER_Y) == dtComplex34 ? real34IsZero(REGISTER_IMAG34_DATA(REGISTER_Y)) : 1 ) ) ||
                ( (real34CompareAbsLessThan(REGISTER_REAL34_DATA(SREG_DX), REGISTER_REAL34_DATA(SREG_TOL))) && 
                  (real34CompareAbsLessThan(REGISTER_REAL34_DATA(SREG_DY), REGISTER_REAL34_DATA(SREG_TOL))) &&
                  (getRegisterDataType(SREG_DX) == dtComplex34 ? real34CompareAbsLessThan(REGISTER_IMAG34_DATA(SREG_DX), REGISTER_REAL34_DATA(SREG_TOL)) : 1 ) &&
                  (getRegisterDataType(SREG_DY) == dtComplex34 ? real34CompareAbsLessThan(REGISTER_IMAG34_DATA(SREG_DY), REGISTER_REAL34_DATA(SREG_TOL)) : 1 ) ) ;

    checkNaN = checkNaN || 
                real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || (getRegisterDataType(REGISTER_X) == dtComplex34 ? real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X)) : 0 ) ||
                real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || (getRegisterDataType(REGISTER_Y) == dtComplex34 ? real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y)) : 0 ) ; 


    #if (defined VERBOSE_SOLVER0) || (defined VERBOSE_SOLVER1)
    if(checkzero) {
      printf("--B1 Checkzero\n");
    }
    if(checkNaN || ix==LIM || checkzero) {
      printf("-->B2 Endflags zero: |DXr|:%u |DXr<TOL|:%u  |DXi|:%u |DYr<TOL|:%u |DYr|:%u |DYi|:%u NaN: |DXr|:%u |DXi|:%u |DYr|:%u |DYi|%u \n",
        (uint16_t)real34IsZero(REGISTER_REAL34_DATA(REGISTER_X)),
        (uint16_t)(real34CompareAbsLessThan(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(SREG_TOL))),
        (uint16_t)real34IsZero(REGISTER_IMAG34_DATA(REGISTER_X)),
        (uint16_t)real34IsZero(REGISTER_REAL34_DATA(REGISTER_Y)),
        (uint16_t)(real34CompareAbsLessThan(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(SREG_TOL))),
        (uint16_t)real34IsZero(REGISTER_IMAG34_DATA(REGISTER_Y)),
        (uint16_t)real34IsNaN (REGISTER_REAL34_DATA(REGISTER_X)),
        (uint16_t)real34IsNaN (REGISTER_IMAG34_DATA(REGISTER_X)),
        (uint16_t)real34IsNaN (REGISTER_REAL34_DATA(REGISTER_Y)),
        (uint16_t)real34IsNaN (REGISTER_IMAG34_DATA(REGISTER_Y))
        );
    }
    #endif //VERBOSE_SOLVER

    ix++; ixd++;


    #if (defined VERBOSE_SOLVER2) && (defined PC_BUILD)
      if(!checkNaN && !(ix==LIM) && !checkzero) {
        printf("END     ix=%d |DX|<TOL:%d ",ix,real34CompareAbsLessThan(REGISTER_REAL34_DATA(SREG_DX), REGISTER_REAL34_DATA(SREG_TOL)));
        printRegisterToConsole(SREG_DX,"","\n");
        printf("END     ix=%d |DY|<TOL:%d ",ix,real34CompareAbsLessThan(REGISTER_REAL34_DATA(SREG_DY), REGISTER_REAL34_DATA(SREG_TOL)));
        printRegisterToConsole(SREG_DY,"","\n");
        printRegisterToConsole(REGISTER_Y,"END     DY=","\n");

      }
    #endif //VERBOSE_SOLVER2

    #if (defined VERBOSE_SOLVER1) && (defined PC_BUILD)
      printf("               DX: ");printRegisterToConsole(SREG_DX,"","\n");
      printf("               DY: ");printRegisterToConsole(SREG_DY,"","\n");
      printf("               X0: ");printRegisterToConsole(SREG_X0,"","\n");
      printf("               X1: ");printRegisterToConsole(SREG_X1,"","\n");
      printf("               X2: ");printRegisterToConsole(SREG_X2,"","\n");
      printf("   -------> newX2: ");printRegisterToConsole(SREG_X2N,"","\n");
    #endif //VERBOSE_SOLVER1


    #if (defined VERBOSE_SOLVER0) && (defined PC_BUILD)
      printRegisterToConsole(SREG_DX,">>> DX=","");
      printRegisterToConsole(SREG_DX," DY=","\n");
    #endif



    copySourceRegisterToDestRegister(SREG_X2N,SREG_X2);  //new x2    

    //plots the ix vs abs.difference
    // |dy| is still in Y
    // replace X with ix
    // plot (ix,|dy|)
    if((!checkNaN && !(ix==LIM)) || checkzero) {
      doubleToXRegisterReal34((double)ix);
      if(getRegisterDataType(REGISTER_X) == dtReal34 && getRegisterDataType(REGISTER_Y) == dtReal34)  {
        runFunction(ITM_SIGMAPLUS);
        #ifdef PC_BUILD
          if(lastErrorCode == 24) { 
          	#ifdef VERBOSE_SOLVER1
          	printf("ERROR CODE CANNOT STAT COMPLEX ignored\n"); 
          	#endif //VERBOSE_SOLVER1
          	lastErrorCode = 0;
          }
        #endif
      }
    }

      #ifdef DMCP_BUILD
        lcd_refresh();
      #else // !DMCP_BUILD
        refreshLcd(NULL);
      #endif // DMCP_BUILD

  }


//Iterations end

  runFunction(ITM_CLSTK);
  fnStrtoX("; Xo= ");
  fnRCL(SREG_STARTX);
  runFunction(ITM_ADD);
  runFunction(ITM_ADD);
  

  runFunction(ITM_TICKS);       //Calculate time used
  fnRCL(SREG_TICKS);
  runFunction(ITM_CHS);
  runFunction(ITM_ADD);
  doubleToXRegisterReal34(10);
  runFunction(ITM_DIV);
  fnStrtoX(" s for ");
  runFunction(ITM_ADD);

  runFunction(ITM_NSIGMA);     //Check number of iterations
  fnStrtoX(" iterations");
  runFunction(ITM_ADD);

  runFunction(ITM_ADD);

  fnStrtoX(" f(xx)= ");
  fnRCL(SREG_Y2);
  runFunction(ITM_ADD);
  fnStrtoX(" at xx= ");
  runFunction(ITM_ADD);
  
  fnRCL(SREG_X2);
  
//  if( ix * 1.0  >  LIM * 0.5) runFunction(ITM_PLOT_XY);
  #endif

#endif //SAVE_SPACE_DM42_20
}



//-----------------------------------------------------//-----------------------------------------------------


void fnEqSolvGraph (uint16_t func) {
  hourGlassIconEnabled = true;
  showHideHourGlass();
  #ifdef DMCP_BUILD
    lcd_refresh();
  #else // !DMCP_BUILD
    refreshLcd(NULL);
  #endif // DMCP_BUILD


  Aspect_Square = false;
  PLOT_INTG = false;
  PLOT_DIFF = false;
  PLOT_RMS  = false;
  PLOT_AXIS = true;
  PLOT_ZMX  = 0;
  PLOT_ZMY  = 0;
  fnClSigma(0);
//  runFunction(ITM_CLSIGMA);

  switch (func) 
  {
    #ifndef SAVE_SPACE_DM42_21
      case EQ_SOLVE:
                //calcRegister_t SREG_STARTX = 82; see above
                fnStore(82);
                graph_solver(graph_xmin, graph_xmax);
                break;
      case EQ_PLOT: {
              double ix1 = convert_to_double(REGISTER_X);
              double ix0 = convert_to_double(REGISTER_Y);
              if(ix1>ix0 && !(ix1==ix0)) {
                graph_xmin = ix0;
                graph_xmax = ix1;
              }
              graph_eqn(graph_xmin, graph_xmax);
              break;
            }

      #endif //SAVE_SPACE_DM42_21
  	  default:;
     }
}








