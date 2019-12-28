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
 * \file generateConstants.c
 ***********************************************/

#define IBM_DECIMAL                                     1
#define NUMBER_OF_CONSTANTS_IN_CNST_CATALOG            79

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef __APPLE__
  // needed for chdir:
  #include<unistd.h>
#endif // __APPLE__

#if (IBM_DECIMAL == 1)
 #define DECNUMDIGITS 39
 #include "decimal128.h"
 #include "decimal64.h"
 #include "decDouble.h"
 #include "decQuad.h"
 #include "decNumberWrappers.h"
#endif

realContext_t ctxtReal34, ctxtReal39, ctxtReal51, ctxtReal459;

char       whiteSpace[50], temp[10000];
char       defines[1000000], externalDeclarations[1000000]; // .h file
char       realArray[1000000], realPointerDeclarations[1000000], real16PointerDeclarations[1000000], real34PointerDeclarations[1000000], real51PointerDeclarations[1000000], real459PointerDeclarations[1000000]; // .c file
FILE       *constantsC, *constantsH;
int        idx, c;

void generateConstantArray(char *name, char *value) {
  real39_t real39;

  printf("generateConstantArray: %-10.10s = %s\n", name, value);

  memset(&real39, 0, REAL39_SIZE);
  stringToReal(value, &real39, &ctxtReal39);

  strcpy(whiteSpace, "                                        ");
  whiteSpace[13 - strlen(name)] = 0;

  strcat(externalDeclarations, "extern const real_t * const const_");
  strcat(externalDeclarations, name);
  strcat(externalDeclarations, ";\n");

  if(c <= NUMBER_OF_CONSTANTS_IN_CNST_CATALOG) {
    sprintf(temp, "#define CONST_%02d %4d\n", c, idx);
    strcat(defines, temp);
  }

  strcat(realPointerDeclarations, "const real_t * const const_");
  strcat(realPointerDeclarations, name);
  strcat(realPointerDeclarations, whiteSpace);
  strcat(realPointerDeclarations, " = (real_t *)(constants + ");
  sprintf(temp, "%5d)", idx);
  idx += REAL39_SIZE;
  strcat(realPointerDeclarations, temp);
  strcat(realPointerDeclarations, ";\n");

  strcat(realArray, "  ");
  for(uint32_t i=0; i<REAL39_SIZE; i++) {
    sprintf(temp, "0x%02x,", *(((uint8_t *)(&real39)) + i));
    strcat(realArray, temp);
  }

  strcat(realArray, "  // const_");
  strcat(realArray, name);
  strcat(realArray, "\n");
}


void generateConstantArray34(char *name, char *value) {
  real34_t real34;

  printf("generateConstantArray34: %-10.10s = %s\n", name, value);

  memset(&real34, 0, REAL34_SIZE);
  stringToReal34(value, &real34);

  strcpy(whiteSpace, "                                        ");
  whiteSpace[9 - strlen(name)] = 0;

  strcat(externalDeclarations, "extern const real34_t * const const34_");
  strcat(externalDeclarations, name);
  strcat(externalDeclarations, ";\n");

  strcat(real34PointerDeclarations, "const real34_t * const const34_");
  strcat(real34PointerDeclarations, name);
  strcat(real34PointerDeclarations, whiteSpace);
  strcat(real34PointerDeclarations, " = (real34_t *)(constants + ");
  sprintf(temp, "%5d)", idx);
  idx += REAL34_SIZE;
  strcat(real34PointerDeclarations, temp);
  strcat(real34PointerDeclarations, ";\n");

  strcat(realArray, "  ");
  for(uint32_t i=0; i<REAL34_SIZE; i++) {
    sprintf(temp, "0x%02x,", *(((uint8_t *)(&real34)) + i));
    strcat(realArray, temp);
  }

  strcat(realArray, "  // const_");
  strcat(realArray, name);
  strcat(realArray, "\n");
}


void generateConstantArray51(char *name, char *value) {
  real51_t real51;

  printf("generateConstantArray51: %-10.10s = %s\n", name, value);

  memset(&real51, 0, REAL51_SIZE);
  stringToReal(value, &real51, &ctxtReal51);

  strcpy(whiteSpace, "                                        ");
  whiteSpace[13 - strlen(name)] = 0;

  strcat(externalDeclarations, "extern const real_t * const const_");
  strcat(externalDeclarations, name);
  strcat(externalDeclarations, ";\n");

  strcat(real51PointerDeclarations, "const real_t * const const_");
  strcat(real51PointerDeclarations, name);
  strcat(real51PointerDeclarations, whiteSpace);
  strcat(real51PointerDeclarations, " = (real_t *)(constants + ");
  sprintf(temp, "%5d)", idx);
  idx += REAL51_SIZE;
  strcat(real51PointerDeclarations, temp);
  strcat(real51PointerDeclarations, ";\n");

  strcat(realArray, "  ");
  for(uint32_t i=0; i<REAL51_SIZE; i++) {
    sprintf(temp, "0x%02x,", *(((uint8_t *)(&real51)) + i));
    strcat(realArray, temp);
  }

  strcat(realArray, "  // const_");
  strcat(realArray, name);
  strcat(realArray, "\n");
}


void generateConstantArray459(char *name, char *value) {
  real459_t real459;

  printf("generateConstantArray459: %-9.9s = %s\n", name, value);

  memset(&real459, 0, REAL459_SIZE);
  stringToReal(value, &real459, &ctxtReal459);

  strcpy(whiteSpace, "                                        ");
  whiteSpace[10 - strlen(name)] = 0;

  strcat(externalDeclarations, "extern const real_t * const const459_");
  strcat(externalDeclarations, name);
  strcat(externalDeclarations, ";\n");

  strcat(real459PointerDeclarations, "const real_t * const const459_");
  strcat(real459PointerDeclarations, name);
  strcat(real459PointerDeclarations, whiteSpace);
  strcat(real459PointerDeclarations, " = (real_t *)(constants + ");
  sprintf(temp, "%5d)", idx);
  idx += REAL459_SIZE;
  strcat(real459PointerDeclarations, temp);
  strcat(real459PointerDeclarations, ";\n");

  strcat(realArray, "  ");
  for(uint32_t i=0; i<REAL459_SIZE; i++) {
    sprintf(temp, "0x%02x,", *(((uint8_t *)(&real459)) + i));
    strcat(realArray, temp);
  }

  strcat(realArray, "  // const_");
  strcat(realArray, name);
  strcat(realArray, "\n");
}


void generateAllConstants(void) {
  // constants used by the program
  idx = 0;

  strcat(realArray, "const uint8_t constants[] = {\n");

  // Physical and mathematical constants
  // each constant has 55 significant digits
  c = 0;
  generateConstantArray("1on2",          "+5.000000000000000000000000000000000000000000000000000000e-01"); c++; //  0 math constant 1/2
  generateConstantArray("a",             "+3.652425000000000000000000000000000000000000000000000000e+02"); c++; //  1 per definition
  generateConstantArray("a0",            "+5.291772109030000000000000000000000000000000000000000000e-11"); c++; //  2
  generateConstantArray("aMoon",         "+3.844000000000000000000000000000000000000000000000000000e+08"); c++; //  3
  generateConstantArray("aEarth",        "+1.495979000000000000000000000000000000000000000000000000e+11"); c++; //  4
  generateConstantArray("c",             "+2.997924580000000000000000000000000000000000000000000000e+08"); c++; //  5 per definition

  generateConstantArray("c1",            "+3.741771852192758011367155555929985138219953097124061418e-16"); c++; //  6
  generateConstantArray("c2",            "+1.438776877503933802146671601543911595199069423148099191e-02"); c++; //  7
  generateConstantArray("e",             "+1.602176634000000000000000000000000000000000000000000000e-19"); c++; //  8
  generateConstantArray("eE",            "+2.718281828459045235360287471352662497757247093699959575e+00"); c++; //  9 math constant e
  generateConstantArray("F",             "+9.648533212331001840000000000000000000000000000000000000e+04"); c++; // 10
  generateConstantArray("Falpha",        "+2.502907875095892822283902873218215786381271376727149977e+00"); c++; // 11 math constant Falpha

  generateConstantArray("Fdelta",        "+4.669201609102990671853203820466201617258185577475768633e+00"); c++; // 12 math constant Fdelta
  generateConstantArray("G",             "+6.674300000000000000000000000000000000000000000000000000e-11"); c++; // 13
  generateConstantArray("G0",            "+7.748091729863650646680823323308763943587286047673370920e-05"); c++; // 14
  generateConstantArray("GC",            "+9.159655941772190150546035149323841107741493742816721343e-01"); c++; // 15 math constant Catalan
  generateConstantArray("ge",            "-2.002319304362560000000000000000000000000000000000000000e+00"); c++; // 16
  generateConstantArray("GM",            "+3.986004418000000000000000000000000000000000000000000000e+14"); c++; // 17


  generateConstantArray("gEarth",        "+9.806650000000000000000000000000000000000000000000000000e+00"); c++; // 18 per definition
  generateConstantArray("Planck",        "+6.626070150000000000000000000000000000000000000000000000e-34"); c++; // 19
  generateConstantArray("PlanckOn2pi",   "+1.054571817646156391262428003302280744722826330020413122e-34"); c++; // 20
  generateConstantArray("k",             "+1.380649000000000000000000000000000000000000000000000000e-23"); c++; // 21
  generateConstantArray("KJ",            "+4.835978484169836324476582850545281353533511866004014461e+14"); c++; // 22
  generateConstantArray("lPL",           "+1.616255000000000000000000000000000000000000000000000000e-35"); c++; // 23

  generateConstantArray("me",            "+9.109383701500000000000000000000000000000000000000000000e-31"); c++; // 24
  generateConstantArray("MMoon",         "+7.349000000000000000000000000000000000000000000000000000e+22"); c++; // 25
  generateConstantArray("mn",            "+1.674927498040000000000000000000000000000000000000000000e-27"); c++; // 26
  generateConstantArray("mnOnmp",        "+1.001378418980000000000000000000000000000000000000000000e+00"); c++; // 27
  generateConstantArray("mp",            "+1.672621923690000000000000000000000000000000000000000000e-27"); c++; // 28
  generateConstantArray("mPL",           "+2.176435000000000000000000000000000000000000000000000000e-08"); c++; // 29

  generateConstantArray("mpOnme",        "+1.836152673430000000000000000000000000000000000000000000e+03"); c++; // 30
  generateConstantArray("mu",            "+1.660539066600000000000000000000000000000000000000000000e-27"); c++; // 31
  generateConstantArray("muc2",          "+1.492418085600000000000000000000000000000000000000000000e-10"); c++; // 32
  generateConstantArray("mmu",           "+1.883531627000000000000000000000000000000000000000000000e-28"); c++; // 33
  generateConstantArray("mSun",          "+1.989100000000000000000000000000000000000000000000000000e+30"); c++; // 34
  generateConstantArray("mEarth",        "+5.973600000000000000000000000000000000000000000000000000e+24"); c++; // 35


  generateConstantArray("NA",            "+6.022140760000000000000000000000000000000000000000000000e+23"); c++; // 36
  generateConstantArray("NaN",           "Not a number"                                                 ); c++; // 37
  generateConstantArray("p0",            "+1.013250000000000000000000000000000000000000000000000000e+05"); c++; // 38 per definition
  generateConstantArray("R",             "+8.314462618153240000000000000000000000000000000000000000e+00"); c++; // 39
  generateConstantArray("re",            "+2.817940326200000000000000000000000000000000000000000000e-15"); c++; // 40
  generateConstantArray("RK",            "+2.581280745930450666004551670608744304245727322140342177e+04"); c++; // 41

  generateConstantArray("RMoon",         "+1.737530000000000000000000000000000000000000000000000000e+06"); c++; // 42
  generateConstantArray("RInfinity",     "+1.097373156816000000000000000000000000000000000000000000e+07"); c++; // 43
  generateConstantArray("RSun",          "+6.960000000000000000000000000000000000000000000000000000e+08"); c++; // 44
  generateConstantArray("REarth",        "+6.371010000000000000000000000000000000000000000000000000e+06"); c++; // 45
  generateConstantArray("Sa",            "+6.378137000000000000000000000000000000000000000000000000e+06"); c++; // 46 per definition
  generateConstantArray("Sb",            "+6.356752314200000000000000000000000000000000000000000000e+06"); c++; // 47

  generateConstantArray("Se2",           "+6.694379990140000000000000000000000000000000000000000000e-03"); c++; // 48
  generateConstantArray("Sep2",          "+6.739496742280000000000000000000000000000000000000000000e-03"); c++; // 49
  generateConstantArray("Sfm1",          "+2.982572235630000000000000000000000000000000000000000000e+02"); c++; // 50 per definition
  generateConstantArray("T0",            "+2.731500000000000000000000000000000000000000000000000000e+02"); c++; // 51 per definition
  generateConstantArray("TP",            "+1.416785000000000000000000000000000000000000000000000000e+32"); c++; // 52
  generateConstantArray("tPL",           "+5.391245000000000000000000000000000000000000000000000000e-44"); c++; // 53


  generateConstantArray("Vm",            "+2.241396954501413773501110288675055514433752775721687639e-02"); c++; // 54
  generateConstantArray("Z0",            "+3.767303134617706554681984004203193082686235083524186552e+02"); c++; // 55 mu0 * c
  generateConstantArray("alpha",         "+7.297352569300000000000000000000000000000000000000000000e-03"); c++; // 56
  generateConstantArray("gamma",         "+9.806650000000000000000000000000000000000000000000000000e+00"); c++; // 57 gamma = gEarth --> is this duplicate constant needed?
  generateConstantArray("gammaEM",       "+5.772156649015328606065120900824024310421593359399235988e-01"); c++; // 58 math constant Euler-Mascheroni
  generateConstantArray("gammap",        "+2.675221874400000000000000000000000000000000000000000000e+08"); c++; // 59

  generateConstantArray("DELTAvcs",      "+9.192631770000000000000000000000000000000000000000000000e+09"); c++; // 60 per definition
  generateConstantArray("epsilon0",      "+8.854187812800000000000000000000000000000000000000000000e-12"); c++; // 61
  generateConstantArray("lambdaC",       "+2.426310238670000000000000000000000000000000000000000000e-12"); c++; // 62
  generateConstantArray("lambdaCn",      "+1.319590905810000000000000000000000000000000000000000000e-15"); c++; // 63
  generateConstantArray("lambdaCp",      "+1.321409855390000000000000000000000000000000000000000000e-15"); c++; // 64
  generateConstantArray("mu0",           "+1.256637062120000000000000000000000000000000000000000000e-06"); c++; // 65

  generateConstantArray("muB",           "+9.274010078000000000000000000000000000000000000000000000e-24"); c++; // 66
  generateConstantArray("mue",           "-9.284764704300000000000000000000000000000000000000000000e-24"); c++; // 67
  generateConstantArray("mueOnmuB",      "-1.001159652181280000000000000000000000000000000000000000e+00"); c++; // 68
  generateConstantArray("mun",           "-9.662365000000000000000000000000000000000000000000000000e-27"); c++; // 69
  generateConstantArray("mup",           "+1.410606797360000000000000000000000000000000000000000000e-26"); c++; // 70
  generateConstantArray("muu",           "+5.050783746100000000000000000000000000000000000000000000e-27"); c++; // 71


  generateConstantArray("mumu",          "-4.490448300000000000000000000000000000000000000000000000e-26"); c++; // 72
  generateConstantArray("sigmaB",        "+5.670374419184429453970996731889230875840122970291303682e-08"); c++; // 73
  generateConstantArray("PHI",           "+1.618033988749894848204586834365638117720309179805762862e+00"); c++; // 74 math constant phi = (1 + sqrt(5)) / 2
  generateConstantArray("PHI0",          "+2.067833848461929323081115412147497340171545654934323552e-15"); c++; // 75
  generateConstantArray("omega",         "+7.292115000000000000000000000000000000000000000000000000e-05"); c++; // 76
  generateConstantArray("minusInfinity", "-9.999999999999999999999999999999999999999999999999999999e+9999"); c++; // 77 math "constant"

  generateConstantArray("plusInfinity",  "+9.999999999999999999999999999999999999999999999999999999e+9999"); c++; // 78 math "constant"
  generateConstantArray("0",             "0"); c++;

  // Array angle180
  generateConstantArray("180",           "+1.800000000000000000000000000000000000000000000000000000e+02");
  generateConstantArray("200",           "+2.000000000000000000000000000000000000000000000000000000e+02");
  generateConstantArray("pi",            "+3.141592653589793238462643383279502884197169399375105821e+00");
  generateConstantArray("1",             "+1.000000000000000000000000000000000000000000000000000000e+00");

  // Array angle90
  generateConstantArray("90",            "+9.000000000000000000000000000000000000000000000000000000e+01");
  generateConstantArray("100",           "+1.000000000000000000000000000000000000000000000000000000e+02");
  generateConstantArray("piOn2",         "+1.570796326794896619231321691639751442098584699687552910e+00");
  generateConstantArray("0_5",           "+5.000000000000000000000000000000000000000000000000000000e-01");

  // Array angle45
  generateConstantArray("45",            "+4.500000000000000000000000000000000000000000000000000000e+01");
  generateConstantArray("50",            "+5.000000000000000000000000000000000000000000000000000000e+01");
  generateConstantArray("piOn4",         "+7.853981633974483096156608458198757210492923498437764552e-01");
  generateConstantArray("1on4",          "+2.500000000000000000000000000000000000000000000000000000e-01");

  // All the formulas are 100% exact conversion formulas
  generateConstantArray("PointToM",      "+3.527777777777777777777777777777777777777777777777777778e-04"); // m      = pt * 0.0254 / 72
  generateConstantArray("InchToM",       "+2.540000000000000000000000000000000000000000000000000000e-02"); // m      = inch * 0,0254
  generateConstantArray("FtToM",         "+3.048000000000000000000000000000000000000000000000000000e-01"); // m      = ft * 12 * 0.0254
  generateConstantArray("SfeetToM",      "+3.048006096012192024384048768097536195072390144780289561e-01"); // m      = sfeetus * (1200 / 3937)
  generateConstantArray("YardToM",       "+9.144000000000000000000000000000000000000000000000000000e-01"); // m      = yard * 3 * 12 * 0.0254
  generateConstantArray("FathomToM",     "+1.828800000000000000000000000000000000000000000000000000e+00"); // m      = fathom * 6 * 12 * 0.0254
  generateConstantArray("MiToM",         "+1.609344000000000000000000000000000000000000000000000000e+03"); // m      = mile * 63360 * 0.0254
  generateConstantArray("NmiToM",        "+1.852000000000000000000000000000000000000000000000000000e+03"); // m      = nmi * 1852
  generateConstantArray("AuToM",         "+1.495978707000000000000000000000000000000000000000000000e+11"); // m      = au * 149597870700
  generateConstantArray("LyToM",         "+9.460730472580800000000000000000000000000000000000000000e+15"); // m      = ly * 299792458 * 3600 * 24 * 365.25
  generateConstantArray("PcToM",         "+3.085677581491367278913937957796471610731921160409179801e+16"); // m      = pc * 149597870700 * 648000 / pi

  generateConstantArray("AccreToM2",     "+4.046856422400000000000000000000000000000000000000000000e+03"); // m²     = acre * 0.0254² * 12² * 43560
  generateConstantArray("AccreusToM2",   "+4.046872609874252006568529266090790246096621225500515517e+03"); // m²     = acreus * (1200 / 3937)² * 43560

  generateConstantArray("FlozukToM3",    "+2.841306250000000000000000000000000000000000000000000000e-05"); // m³     = flozuk * 4,54609e-3 / 160
  generateConstantArray("FlozusToM3",    "+2.957352956250000000000000000000000000000000000000000000e-05"); // m³     = flozus * 231 * 0.0254³ / 128
  generateConstantArray("GalusToM3",     "+3.785411784000000000000000000000000000000000000000000000e-03"); // m³     = galus * 231 * 0.0254³
  generateConstantArray("GalukToM3",     "+4.546090000000000000000000000000000000000000000000000000e-03"); // m³     = galuk * 4,54609e-3
  generateConstantArray("QuartToM3",     "+1.136522500000000000000000000000000000000000000000000000e-03"); // m³     = quart * 4,54609e-3 / 4
  generateConstantArray("BarrelToM3",    "+1.589872949280000000000000000000000000000000000000000000e-01"); // m³     = barrel * 42 * 231 * 0.0254³

  generateConstantArray("CaratToKg",     "+2.000000000000000000000000000000000000000000000000000000e-04"); // kg     = carat * 0.0002
  generateConstantArray("OzToKg",        "+2.834952312500000000000000000000000000000000000000000000e-02"); // kg     = oz * (0,45359237 / 16)
  generateConstantArray("TrozToKg",      "+3.110347680000000000000000000000000000000000000000000000e-02"); // kg     = tr.oz * 0.45359237 * 175 / 12
  generateConstantArray("LbToKg",        "+4.535923700000000000000000000000000000000000000000000000e-01"); // kg     = lb * 0,45359237     (1 Lbs (pound) = 16 oz)
  generateConstantArray("StoneToKg",     "+6.350293180000000000000000000000000000000000000000000000e+00"); // kg     = stone * 14 * 0,45359237
  generateConstantArray("ShortcwtToKg",  "+4.535923700000000000000000000000000000000000000000000000e+01"); // kg     = short cwt * 100 * 0.45359237 (short cwt = short hundredweight)
  generateConstantArray("CwtToKg",       "+5.080234544000000000000000000000000000000000000000000000e+01"); // kg     = cwt * 112 * 0,45359237       (cwt = long hundredWeight)
  generateConstantArray("ShorttonToKg",  "+9.071847400000000000000000000000000000000000000000000000e+02"); // kg     = short ton * 2000 * 0,45359237
  generateConstantArray("TonToKg",       "+1.016046908800000000000000000000000000000000000000000000e+03"); // kg     = ton * 2240 * 0,45359237

  generateConstantArray("CalToJ",        "+4.186800000000000000000000000000000000000000000000000000e+00"); // joule  = calorie * 4,1868
  generateConstantArray("BtuToJ",        "+1.055055852620000000000000000000000000000000000000000000e+03"); // joule  = Btu * 1055,05585262
  generateConstantArray("WhToJ",         "+3.600000000000000000000000000000000000000000000000000000e+03"); // joule  = Wh * 3600

  generateConstantArray("LbfToN",        "+4.448221615260500000000000000000000000000000000000000000e+00"); // newton = lbf * 9,80665 * 0,45359237

  generateConstantArray("TorrToPa",      "+1.333223684210526315789473684210526315789473684210526316e+02"); // pascal = torr * 101325 / 760
  generateConstantArray("InhgToPa",      "+3.386388157894736842105263157894736842105263157894736842e+03"); // pascal = in.Hg * 101325 / 760 * 25.4
  generateConstantArray("PsiToPa",       "+6.894757293168361336722673445346890693781387562775125550e+03"); // pascal = psi * 0.45359237 * 9.80665 / 0.0254²
  generateConstantArray("BarToPa",       "+1.000000000000000000000000000000000000000000000000000000e+05"); // pascal = bar  * 100000
  generateConstantArray("AtmToPa",       "+1.013250000000000000000000000000000000000000000000000000e+05"); // pascal = atm * 101325

  generateConstantArray("HpmToW",        "+7.354987500000000000000000000000000000000000000000000000e+02"); // watt   = HPM * 75 * 9,80665
  generateConstantArray("HpukToW",       "+7.456998715822702200000000000000000000000000000000000000e+02"); // watt   = HPUK * 550 * 0,3048 * 9,80665 * 0,45359237
  generateConstantArray("HpeToW",        "+7.460000000000000000000000000000000000000000000000000000e+02"); // watt   = HPE * 746

  generateConstantArray("YearToS",       "+3.155695200000000000000000000000000000000000000000000000e+07"); // second = year  * (365,2425 * 24 * 3600)

  generateConstantArray("_4",            "-4.000000000000000000000000000000000000000000000000000000e+00");
  generateConstantArray("_1",            "-1.000000000000000000000000000000000000000000000000000000e+00");
  generateConstantArray("1e_49",         "+1.000000000000000000000000000000000000000000000000000000e-49");
  generateConstantArray("1e_37",         "+1.000000000000000000000000000000000000000000000000000000e-37");
  generateConstantArray("1e_24",         "+1.000000000000000000000000000000000000000000000000000000e-24");
  generateConstantArray("1e_6",          "+1.000000000000000000000000000000000000000000000000000000e-06");
  generateConstantArray("1e_4",          "+1.000000000000000000000000000000000000000000000000000000e-04");
  generateConstantArray("1on10",         "+1.000000000000000000000000000000000000000000000000000000e-01");
  generateConstantArray("1on3",          "+3.333333333333333333333333333333333333333333333333333333e-01");
  generateConstantArray("egamma",        "+5.772156649015328606065120900824024310421593359399235988e-01");
  generateConstantArray("ln2",           "+6.931471805599453094172321214581765680755001343602552541e-01");
  generateConstantArray("root2on2",      "+7.071067811865475244008443621048490392848359376884740366e-01");
  generateConstantArray("9on10",         "+9.000000000000000000000000000000000000000000000000000000e-01");
  generateConstantArray("3on2",          "+1.500000000000000000000000000000000000000000000000000000e+00");
  generateConstantArray("9on5",          "+1.800000000000000000000000000000000000000000000000000000e+00");
  generateConstantArray("2",             "+2.000000000000000000000000000000000000000000000000000000e+00");
  generateConstantArray("ln10",          "+2.302585092994045684017991454684364207601101488628772976e+00");
  generateConstantArray("3piOn4",        "+2.356194490192344928846982537459627163147877049531329366e+00");
  generateConstantArray("3",             "+3.000000000000000000000000000000000000000000000000000000e+00");
  generateConstantArray("5",             "+5.000000000000000000000000000000000000000000000000000000e+00");
  generateConstantArray("2pi",           "+6.283185307179586476925286766559005768394338798750211642e+00");
  generateConstantArray("10",            "+1.000000000000000000000000000000000000000000000000000000e+01");
  generateConstantArray("20",            "+2.000000000000000000000000000000000000000000000000000000e+01");
  //generateConstantArray("21",            "+2.100000000000000000000000000000000000000000000000000000e+01"); // used for Lanczos N=22
  //generateConstantArray("gammaR",        "+2.311891000000000000000000000000000000000000000000000000e+01"); // used for Lanczos N=22
  generateConstantArray("29",            "+2.900000000000000000000000000000000000000000000000000000e+01"); // used for Lanczos N=30
  generateConstantArray("gammaR",        "+3.143188335932791233062744140625000000000000000000000000e+01"); // used for Lanczos N=30
  generateConstantArray("32",            "+3.200000000000000000000000000000000000000000000000000000e+01");
  generateConstantArray("36",            "+3.600000000000000000000000000000000000000000000000000000e+01");
  generateConstantArray("47",            "+4.700000000000000000000000000000000000000000000000000000e+01");
  generateConstantArray("180onPi",       "+5.729577951308232087679815481410517033240547246656432155e+01");
  generateConstantArray("60",            "+6.000000000000000000000000000000000000000000000000000000e+01");
  generateConstantArray("200onPi",       "+6.366197723675813430755350534900574481378385829618257950e+01");
  generateConstantArray("205",           "+2.050000000000000000000000000000000000000000000000000000e+02");
  generateConstantArray("360",           "+3.600000000000000000000000000000000000000000000000000000e+02");
  generateConstantArray("400",           "+4.000000000000000000000000000000000000000000000000000000e+02");
  generateConstantArray("1000",          "+1.000000000000000000000000000000000000000000000000000000e+03");
  generateConstantArray("3600",          "+3.600000000000000000000000000000000000000000000000000000e+03");
  generateConstantArray("9999",          "+9.999000000000000000000000000000000000000000000000000000e+03");
  generateConstantArray("10000",         "+1.000000000000000000000000000000000000000000000000000000e+04");
  generateConstantArray("1e6",           "+1.000000000000000000000000000000000000000000000000000000e+06");
  generateConstantArray("1e8",           "+1.000000000000000000000000000000000000000000000000000000e+08");
  generateConstantArray("1e9",           "+1.000000000000000000000000000000000000000000000000000000e+09");
  generateConstantArray("2p32",          "+4.294967296000000000000000000000000000000000000000000000e+09");

  // Lanczos's coefficients calculated for N=22 and G=22.61891 using Toth's program: https://www.vttoth.com/CMS/projects/41
  // source: https://www.vttoth.com/FILES/lanczos.tgz
  //                                                   1    1    2    2    3    3    4    4    5    5
  //                                         1    5    0    5    0    5    0    5    0    5    0    5
  //generateConstantArray51("gammaC00",      "+2.506628274631000502415765284811045253330102106110384424e+00");
  //generateConstantArray51("gammaC01",      "+1.898901420935934892121516421489444871166098343657373978e+10");
  //generateConstantArray51("gammaC02",      "-1.441562000905355882360184024174589398850864438036062023e+11");
  //generateConstantArray51("gammaC03",      "+4.960354542573828137004589453751102249851958972199579195e+11");
  //generateConstantArray51("gammaC04",      "-1.023780406198473219243634817725018714004771971677300816e+12");
  //generateConstantArray51("gammaC05",      "+1.413597258976513273633654064270589122622864275842333381e+12");
  //generateConstantArray51("gammaC06",      "-1.379067427882918397935921608473380694063139188889206483e+12");
  //generateConstantArray51("gammaC07",      "+9.788204370638776727185550760418500178668333538359412666e+11");
  //generateConstantArray51("gammaC08",      "-5.128994840924296233163734159755732045598563235099171971e+11");
  //generateConstantArray51("gammaC09",      "+1.993214894537074020805536689671109703586865664821761460e+11");
  //generateConstantArray51("gammaC10",      "-5.724477320502851934636585458046006238313974208001884541e+10");
  //generateConstantArray51("gammaC11",      "+1.201655806354758157534702159196092100664925734696554628e+10");
  //generateConstantArray51("gammaC12",      "-1.809010182477543231013601186822450869655092323236977417e+09");
  //generateConstantArray51("gammaC13",      "+1.898547541983866894247096477096985200814067754731731207e+08");
  //generateConstantArray51("gammaC14",      "-1.334263251277484954309331237987833882618884749172456742e+07");
  //generateConstantArray51("gammaC15",      "+5.933439303341291714746800542914306113010567634244078777e+05");
  //generateConstantArray51("gammaC16",      "-1.540327280024945239058720773875327176489146552268675093e+04");
  //generateConstantArray51("gammaC17",      "+2.074489944028394118495377261887808208026742582699496038e+02");
  //generateConstantArray51("gammaC18",      "-1.209628455273316629072046046890209041606257952008366823e+00");
  //generateConstantArray51("gammaC19",      "+2.269611174611951432929578313072997499699731150734389541e-03");
  //generateConstantArray51("gammaC20",      "-7.988885865727861701217352063783045960010829242532606098e-07");
  //generateConstantArray51("gammaC21",      "+1.657343879782420104047477447896382420026771040379552772e-11");

  // A better set of Lanczos constants?
  // https://www.boost.org/doc/libs/1_71_0/boost/math/special_functions/lanczos.hpp
  // https://www.boost.org/doc/libs/1_71_0/libs/math/doc/html/math_toolkit/lanczos.html
  // Lanczos's coefficients calculated for N=30 and G=30.9318857491016387939453125 using Toth's program: https://www.vttoth.com/CMS/projects/41
  // source: https://www.vttoth.com/FILES/lanczos.tgz
  generateConstantArray51("gammaC00",      "+2.506628274631000502415765284811045253006986740036318756703152698088652909808141644264478158114643");
  generateConstantArray51("gammaC01",      "+90795313438554.12585760728412902207185494121818510422990562390199419594778421509725059250660477478");
  generateConstantArray51("gammaC02",      "-966788810976616.3165238613163922965882171247384377771690792930892852741666385482155753752741846215");
  generateConstantArray51("gammaC03",      "+4800584619652903.130520636355077553806266769741879659498830407152738371167934130745600044623388068");
  generateConstantArray51("gammaC04",      "-14765868961297540.27064904092821149556100711736690133148186310232345861145647352272488652687903457");
  generateConstantArray51("gammaC05",      "+31520009430682538.17457937985920649010992386050798974102315423368438398460245896094052856122203775");
  generateConstantArray51("gammaC06",      "-49582365010416661.09326008795178786489253227385559918969410907133030684376335533671631031226780693");
  generateConstantArray51("gammaC07",      "+59568927402677002.04904143939675965126139196724493177865501398609411221897535112902272116361691451");
  generateConstantArray51("gammaC08",      "-55906342816448463.66899438215049258933959227649607458054232171494476786360919120606779433224203661");
  generateConstantArray51("gammaC09",      "+41579705761882718.81836872026846865595091094440150772658809279820390684724850793073271091274939857");
  generateConstantArray51("gammaC10",      "-24720590950576683.15802797249415999638807846332716128189191944706094647468094435934791589404852933");
  generateConstantArray51("gammaC11",      "+11801286985635343.58575847383935274746074763562173560062003882605616928057523864988435228756829000");
  generateConstantArray51("gammaC12",      "-4528196760270806.995369621666200554735964133084677928395568191894066774034949226537269118163329489");
  generateConstantArray51("gammaC13",      "+1393637064384668.917157722908202033726675908093527246704252104958408303381715387740313008845141904");
  generateConstantArray51("gammaC14",      "-342325016766888.5479936955467723387327600568400337988293619211103486673308239810777313598627315000");
  generateConstantArray51("gammaC15",      "+66578560073752.67246403368251720854813222816576859988564202476567602534436134110254400104404942690");
  generateConstantArray51("gammaC16",      "-10138544793022.41604607361389789580035078467043905508354626409172360042771928364449573017241522124");
  generateConstantArray51("gammaC17",      "+1190960562789.022438954352946350698116714288406439572882388541923119338089933765849690628214181895");
  generateConstantArray51("gammaC18",      "-105857166683.4123935373722836022279857082446927458625016646619942749346232137590435016079220044575");
  generateConstantArray51("gammaC19",      "+6945291327.796451124796820522079456356395826950647769057580507091796596412483352351183618549540841");
  generateConstantArray51("gammaC20",      "-325841232.8464260929688459624630603325071713695605050282752915466327762939975965122976740632863654");
  generateConstantArray51("gammaC21",      "+10491089.04183867431328778246866673594812558539972730107563825597306012268992151854201423652554125");
  generateConstantArray51("gammaC22",      "-219652.9521075940388150629624539015343302845601375714110391179944415326942002537588845815284375311");
  generateConstantArray51("gammaC23",      "+2782.640500292405732331071582785081107276742114500310058012342169019346915906467717358069437084413");
  generateConstantArray51("gammaC24",      "-19.31769482393126228999283928711817461826517031491190754697778125203682359806685621504868658022287");
  generateConstantArray51("gammaC25",      "+0.063813226038364736490677427053094184825560153837400637303741419820260651820063721448175876091640");
  generateConstantArray51("gammaC26",      "-0.000081213345051388882076245151519040492731624505633194015094047296992491188162952243561414155660");
  generateConstantArray51("gammaC27",      "+0.000000028425884712306176285600873249949436913248124688912080334562388569408577988201187292514310");
  generateConstantArray51("gammaC28",      "-0.000000000001515686855682081535390179935440192138970398946754350022046202956603870401666503058594");
  generateConstantArray51("gammaC29",      "+0.000000000000000003727184110478268994444830355775133199862144652804253659035860755294511160286895");

  generateConstantArray459("2pi",        "+6.2831853071795864769252867665590057683943387987502116419498891846156328125724179972560696506842341359"
                                            "6429617302656461329418768921910116446345071881625696223490056820540387704221111928924589790986076392"
                                            "8857621951331866892256951296467573566330542403818291297133846920697220908653296426787214520498282547"
                                            "4491740132126311763497630418419256585081834307287357851807200226610610976409330427682939038830232188"
                                            "66114540731519183906184372234763865223586210237096148924759925499134703771505449782455876366e+00");   // 493 digits

  generateConstantArray34("0",           "+0.000000000000000000000000000000000000000000000000000000e+00");
  generateConstantArray34("1e_6",        "+1.000000000000000000000000000000000000000000000000000000e-06");
  generateConstantArray34("1e_4",        "+1.000000000000000000000000000000000000000000000000000000e-04");
  generateConstantArray34("1on2",        "+5.000000000000000000000000000000000000000000000000000000e-01");
  generateConstantArray34("1",           "+1.000000000000000000000000000000000000000000000000000000e+00");
  generateConstantArray34("2pi",         "+6.283185307179586476925286766559005768394338798750211642e+00");
  generateConstantArray34("1000",        "+1.000000000000000000000000000000000000000000000000000000e+03");
  generateConstantArray34("1e6",         "+1.000000000000000000000000000000000000000000000000000000e+06");

  strcat(realArray, "};\n");
}


#ifdef __APPLE__
int main(int argc, char* argv[]) {
    // we take the directory where the application is as the root for this application.
    // in argv[0] is the application itself. We strip the name of the app by searching for the last '/':
    if (argc>=1) {
      char *curdir = malloc(1000);
      // find last /:
      char *s = strrchr(argv[0], '/');
      if (s != 0) {
        // take the directory before the appname:
        strncpy(curdir, argv[0], s-argv[0]);
        chdir(curdir);
        free(curdir);
      }
    }
#else
int main(void) {
#endif

  decContextDefault(&ctxtReal34,   DEC_INIT_DECQUAD);
  decContextDefault(&ctxtReal39,   DEC_INIT_DECQUAD);
  decContextDefault(&ctxtReal51,   DEC_INIT_DECQUAD);
  decContextDefault(&ctxtReal459,  DEC_INIT_DECQUAD);
  ctxtReal39.digits = 39;
  ctxtReal39.traps  = 0;
  ctxtReal51.digits = 51;
  ctxtReal51.traps  = 0;
  ctxtReal459.digits  = 450;
  ctxtReal459.traps   = 0;

  defines[0] = 0;
  externalDeclarations[0] = 0;
  strcat(externalDeclarations, "extern const uint8_t constants[];\n");
  realArray[0] = 0;
  realPointerDeclarations[0]    = 0;
  real16PointerDeclarations[0]  = 0;
  real34PointerDeclarations[0]  = 0;
  real459PointerDeclarations[0] = 0;

  generateAllConstants();


  constantsH = fopen("src/wp43s/constantPointers.h", "wb");
  if(constantsH == NULL) {
    fprintf(stderr, "Cannot create file src/wp43s/constantPointers.h\n");
    exit(1);
  }

  fprintf(constantsH, "/* This file is part of 43S.\n");
  fprintf(constantsH, " *\n");
  fprintf(constantsH, " * 43S is free software: you can redistribute it and/or modify\n");
  fprintf(constantsH, " * it under the terms of the GNU General Public License as published by\n");
  fprintf(constantsH, " * the Free Software Foundation, either version 3 of the License, or\n");
  fprintf(constantsH, " * (at your option) any later version.\n");
  fprintf(constantsH, " *\n");
  fprintf(constantsH, " * 43S is distributed in the hope that it will be useful,\n");
  fprintf(constantsH, " * but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
  fprintf(constantsH, " * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n");
  fprintf(constantsH, " * GNU General Public License for more details.\n");
  fprintf(constantsH, " *\n");
  fprintf(constantsH, " * You should have received a copy of the GNU General Public License\n");
  fprintf(constantsH, " * along with 43S.  If not, see <http://www.gnu.org/licenses/>.\n");
  fprintf(constantsH, " */\n\n");

  fprintf(constantsH, "/********************************************//**\n");
  fprintf(constantsH, "* \\file constantPointers.h constants\n");
  fprintf(constantsH, "***********************************************/\n\n");

  fprintf(constantsH, "/************************************************************************************************\n");
  fprintf(constantsH, "* Do not edit this file manually! It's automagically generated by the program generateConstants *\n");
  fprintf(constantsH, "*************************************************************************************************/\n\n");

  fprintf(constantsH, "%s", defines);
  fprintf(constantsH, "\n");
  fprintf(constantsH, "%s", externalDeclarations);

  fclose(constantsH);



  constantsC = fopen("src/wp43s/constantPointers.c", "wb");
  if(constantsC == NULL) {
    fprintf(stderr, "Cannot create file src/wp43s/constantPointers.c\n");
    exit(1);
  }

  fprintf(constantsC, "/* This file is part of 43S.\n");
  fprintf(constantsC, " *\n");
  fprintf(constantsC, " * 43S is free software: you can redistribute it and/or modify\n");
  fprintf(constantsC, " * it under the terms of the GNU General Public License as published by\n");
  fprintf(constantsC, " * the Free Software Foundation, either version 3 of the License, or\n");
  fprintf(constantsC, " * (at your option) any later version.\n");
  fprintf(constantsC, " *\n");
  fprintf(constantsC, " * 43S is distributed in the hope that it will be useful,\n");
  fprintf(constantsC, " * but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
  fprintf(constantsC, " * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n");
  fprintf(constantsC, " * GNU General Public License for more details.\n");
  fprintf(constantsC, " *\n");
  fprintf(constantsC, " * You should have received a copy of the GNU General Public License\n");
  fprintf(constantsC, " * along with 43S.  If not, see <http://www.gnu.org/licenses/>.\n");
  fprintf(constantsC, " */\n\n");

  fprintf(constantsC, "/********************************************//**\n");
  fprintf(constantsC, "* \\file constantPointers.c constants\n");
  fprintf(constantsC, "***********************************************/\n\n");

  fprintf(constantsC, "/************************************************************************************************\n");
  fprintf(constantsC, "* Do not edit this file manually! It's automagically generated by the program generateConstants *\n");
  fprintf(constantsC, "*************************************************************************************************/\n\n");

  fprintf(constantsC, "#include \"wp43s.h\"\n\n");

  fprintf(constantsC, "%s", realArray);
  fprintf(constantsC, "\n");
  fprintf(constantsC, "%s", realPointerDeclarations);
  fprintf(constantsC, "\n");
  fprintf(constantsC, "%s", real51PointerDeclarations);
  fprintf(constantsC, "\n");
  fprintf(constantsC, "%s", real459PointerDeclarations);
  fprintf(constantsC, "\n");
  fprintf(constantsC, "%s", real16PointerDeclarations);
  fprintf(constantsC, "\n");
  fprintf(constantsC, "%s", real34PointerDeclarations);

  fclose(constantsC);

  return 0;
}
