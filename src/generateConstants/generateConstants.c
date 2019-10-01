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

#define IBM_DECIMAL    1

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// needed for chdir:
#include<unistd.h>

#define DIGITS_FOR_34_DIGITS_INTERMEDIATE_CALCULATIONS 39

#if (IBM_DECIMAL == 1)
 #define DECNUMDIGITS DIGITS_FOR_34_DIGITS_INTERMEDIATE_CALCULATIONS
 #include "decimal128.h"
 #include "decimal64.h"
 #include "decDouble.h"
 #include "decQuad.h"
 #include "decNumberWrappers.h"
#endif

decContext ctxtRealIc; // Intermediate calculations for 34 digits
decContext ctxtReal16, ctxtReal34, ctxtReal451;

char       whiteSpace[50], temp[1000];
char       defines[1000000], externalDeclarations[1000000]; // .h file
char       realArray[1000000], realPointerDeclarations[1000000], real16PointerDeclarations[1000000], real34PointerDeclarations[1000000], real451PointerDeclarations[1000000]; // .c file
FILE       *constantsC, *constantsH;
int        idx, c;

void generateConstantArray(char *name, char *value) {
  realIc_t realIc;

  memset(&realIc, 0, sizeof(realIc));
  stringToRealIc(value, &realIc);

  strcpy(whiteSpace, "                                        ");
  whiteSpace[13 - strlen(name)] = 0;

  strcat(externalDeclarations, "extern const realIc_t * const const_");
  strcat(externalDeclarations, name);
  strcat(externalDeclarations, ";\n");

  if(c <= 79) {
    sprintf(temp, "#define CONST_%02d %4d\n", c, idx);
    strcat(defines, temp);
  }

  strcat(realPointerDeclarations, "const realIc_t * const const_");
  strcat(realPointerDeclarations, name);
  strcat(realPointerDeclarations, whiteSpace);
  strcat(realPointerDeclarations, " = (realIc_t *)(constants + ");
  sprintf(temp, "%5d)", idx);
  idx += sizeof(realIc_t);
  strcat(realPointerDeclarations, temp);
  strcat(realPointerDeclarations, ";\n");

  strcat(realArray, "  ");
  for(uint32_t i=0; i<sizeof(realIc_t); i++) {
    sprintf(temp, "0x%02x,", *(((uint8_t *)(&realIc)) + i));
    strcat(realArray, temp);
  }

  strcat(realArray, "  // const_");
  strcat(realArray, name);
  strcat(realArray, "\n");
}


void generateConstantArray16(char *name, char *value) {
  real16_t real16;

  memset(&real16, 0, sizeof(real16_t));
  stringToReal16(value, &real16);

  strcpy(whiteSpace, "                                        ");
  whiteSpace[11 - strlen(name)] = 0;

  strcat(externalDeclarations, "extern const real16_t * const const16_");
  strcat(externalDeclarations, name);
  strcat(externalDeclarations, ";\n");

  strcat(real16PointerDeclarations, "const real16_t * const const16_");
  strcat(real16PointerDeclarations, name);
  strcat(real16PointerDeclarations, whiteSpace);
  strcat(real16PointerDeclarations, " = (real16_t *)(constants + ");
  sprintf(temp, "%5d)", idx);
  idx += REAL16_SIZE;
  strcat(real16PointerDeclarations, temp);
  strcat(real16PointerDeclarations, ";\n");

  strcat(realArray, "  ");
  for(uint32_t i=0; i<REAL16_SIZE; i++) {
    sprintf(temp, "0x%02x,", *(((uint8_t *)(&real16)) + i));
    strcat(realArray, temp);
  }

  strcat(realArray, "  // const_");
  strcat(realArray, name);
  strcat(realArray, "\n");
}


void generateConstantArray34(char *name, char *value) {
  real34_t real34;

  memset(&real34, 0, sizeof(real34_t));
  stringToReal34(value, &real34);

  strcpy(whiteSpace, "                                        ");
  whiteSpace[11 - strlen(name)] = 0;

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


void generateConstantArray451(char *name, char *value) {
  real451_t real451;

  memset(&real451, 0, sizeof(real451_t));
  stringToRealIc(value, &real451);

  strcpy(whiteSpace, "                                        ");
  whiteSpace[10 - strlen(name)] = 0;

  strcat(externalDeclarations, "extern const realIc_t * const const451_");
  strcat(externalDeclarations, name);
  strcat(externalDeclarations, ";\n");

  strcat(real451PointerDeclarations, "const realIc_t * const const451_");
  strcat(real451PointerDeclarations, name);
  strcat(real451PointerDeclarations, whiteSpace);
  strcat(real451PointerDeclarations, " = (realIc_t *)(constants + ");
  sprintf(temp, "%5d)", idx);
  idx += sizeof(real451_t);
  strcat(real451PointerDeclarations, temp);
  strcat(real451PointerDeclarations, ";\n");

  strcat(realArray, "  ");
  for(uint32_t i=0; i<sizeof(real451_t); i++) {
    sprintf(temp, "0x%02x,", *(((uint8_t *)(&real451)) + i));
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
  generateConstantArray("1on2",          "+5.000000000000000000000000000000000000000000000000000000e-01"); c++; // math constant 1/2
  generateConstantArray("a",             "+3.652425000000000000000000000000000000000000000000000000e+02"); c++; // per definition
  generateConstantArray("a0",            "+5.291772109030000000000000000000000000000000000000000000e-11"); c++;
  generateConstantArray("aM",            "+3.844000000000000000000000000000000000000000000000000000e+08"); c++;
  generateConstantArray("aEarth",        "+1.495979000000000000000000000000000000000000000000000000e+11"); c++;
  generateConstantArray("c",             "+2.997924580000000000000000000000000000000000000000000000e+08"); c++; // per definition

  generateConstantArray("c1",            "+3.741771852192758011367155555929985138219953097124061418e-16"); c++;
  generateConstantArray("c2",            "+1.438776877503933802146671601543911595199069423148099191e-02"); c++;
  generateConstantArray("e",             "+1.602176634000000000000000000000000000000000000000000000e-19"); c++;
  generateConstantArray("eE",            "+2.718281828459045235360287471352662497757247093699959575e+00"); c++; // math constant e
  generateConstantArray("eme",           "+1.758820024000000000000000000000000000000000000000000000e+11"); c++;
  generateConstantArray("F",             "+9.6485332123310018400000000000000000000000000000000000000+04"); c++;

  generateConstantArray("Falpha",        "+2.502907875095892822283902873218215786381271376727149977e+00"); c++; // math constant Falpha
  generateConstantArray("Fdelta",        "+4.669201609102990671853203820466201617258185577475768633e+00"); c++; // math constant Fdelta
  generateConstantArray("G",             "+6.674300000000000000000000000000000000000000000000000000e-11"); c++;
  generateConstantArray("G0",            "+7.748091729863650646680823323308763943587286047673370920e-05"); c++;
  generateConstantArray("GC",            "+9.159655941772190150546035149323841107741493742816721343e-01"); c++; // math constant Catalan
  generateConstantArray("ge",            "-2.002319304362560000000000000000000000000000000000000000e+00"); c++;


  generateConstantArray("GM",            "+3.986004418000000000000000000000000000000000000000000000e+14"); c++;
  generateConstantArray("gEarth",        "+9.806650000000000000000000000000000000000000000000000000e+00"); c++; // per definition
  generateConstantArray("Planck",        "+6.626070150000000000000000000000000000000000000000000000e-34"); c++;
  generateConstantArray("PlanckOn2pi",   "+1.054571817646156391262428003302280744722826330020413122e-34"); c++;
  generateConstantArray("k",             "+1.380649000000000000000000000000000000000000000000000000e-23"); c++;
  generateConstantArray("KJ",            "+4.835978484169836324476582850545281353533511866004014461e+14"); c++;

  generateConstantArray("lp",            "+1.616255000000000000000000000000000000000000000000000000e-35"); c++;
  generateConstantArray("me",            "+9.109383701500000000000000000000000000000000000000000000e-31"); c++;
  generateConstantArray("MM",            "+7.349000000000000000000000000000000000000000000000000000e+22"); c++;
  generateConstantArray("mn",            "+1.674927498040000000000000000000000000000000000000000000e-27"); c++;
  generateConstantArray("mnOnmp",        "+1.001378418980000000000000000000000000000000000000000000e+00"); c++;
  generateConstantArray("mPL",           "+2.176435000000000000000000000000000000000000000000000000e-08"); c++;

  generateConstantArray("mp",            "+1.672621923690000000000000000000000000000000000000000000e-27"); c++;
  generateConstantArray("mpOnme",        "+1.836152673430000000000000000000000000000000000000000000e+03"); c++;
  generateConstantArray("mu",            "+1.660539066600000000000000000000000000000000000000000000e-27"); c++;
  generateConstantArray("muc2",          "+1.492418085600000000000000000000000000000000000000000000e-10"); c++;
  generateConstantArray("mmu",           "+1.883531627000000000000000000000000000000000000000000000e-28"); c++;
  generateConstantArray("mSun",          "+1.989100000000000000000000000000000000000000000000000000e+30"); c++;


  generateConstantArray("mEarth",        "+5.973600000000000000000000000000000000000000000000000000e+24"); c++;
  generateConstantArray("NA",            "+6.022140760000000000000000000000000000000000000000000000e+23"); c++;
  generateConstantArray("NaN",           "Not a number"                                                 ); c++;
  generateConstantArray("p0",            "+1.013250000000000000000000000000000000000000000000000000e+05"); c++; // per definition
  generateConstantArray("R",             "+8.314462618153240000000000000000000000000000000000000000e+00"); c++;
  generateConstantArray("re",            "+2.817940326200000000000000000000000000000000000000000000e-15"); c++;

  generateConstantArray("RK",            "+2.581280745930450666004551670608744304245727322140342177e+04"); c++;
  generateConstantArray("RM",            "+1.737530000000000000000000000000000000000000000000000000e+06"); c++;
  generateConstantArray("RInfinity",     "+1.097373156816000000000000000000000000000000000000000000e+07"); c++;
  generateConstantArray("RSun",          "+6.960000000000000000000000000000000000000000000000000000e+08"); c++;
  generateConstantArray("REarth",        "+6.371010000000000000000000000000000000000000000000000000e+06"); c++;
  generateConstantArray("Sa",            "+6.378137000000000000000000000000000000000000000000000000e+06"); c++; // per definition

  generateConstantArray("Sb",            "+6.356752314200000000000000000000000000000000000000000000e+06"); c++;
  generateConstantArray("Se2",           "+6.694379990140000000000000000000000000000000000000000000e-03"); c++;
  generateConstantArray("Sep2",          "+6.739496742280000000000000000000000000000000000000000000e-03"); c++;
  generateConstantArray("Sfm1",          "+2.982572235630000000000000000000000000000000000000000000e+02"); c++; // per definition
  generateConstantArray("T0",            "+2.731500000000000000000000000000000000000000000000000000e+02"); c++; // per definition
  generateConstantArray("TP",            "+1.416785000000000000000000000000000000000000000000000000e+32"); c++;


  generateConstantArray("tp",            "+5.391245000000000000000000000000000000000000000000000000e-44"); c++;
  generateConstantArray("Vm",            "+2.241396954501413773501110288675055514433752775721687639e-02"); c++;
  generateConstantArray("Z0",            "+3.767303134617706554681984004203193082686235083524186552e+02"); c++; // mu0 * c
  generateConstantArray("alpha",         "+7.297352569300000000000000000000000000000000000000000000e-03"); c++;
  generateConstantArray("gamma",         "+9.806650000000000000000000000000000000000000000000000000e+00"); c++; // gamma = gEarth --> is this duplicate constant needed?
  generateConstantArray("gammaEM",       "+5.772156649015328606065120900824024310421593359399235988e-01"); c++; // math constant Euler-Mascheroni

  generateConstantArray("gammap",        "+2.675221874400000000000000000000000000000000000000000000e+08"); c++;
  generateConstantArray("epsilon0",      "+8.854187812800000000000000000000000000000000000000000000e-12"); c++;
  generateConstantArray("lambdaC",       "+2.426310238670000000000000000000000000000000000000000000e-12"); c++;
  generateConstantArray("lambdaCn",      "+1.319590905810000000000000000000000000000000000000000000e-15"); c++;
  generateConstantArray("lambdaCp",      "+1.321409855390000000000000000000000000000000000000000000e-15"); c++;
  generateConstantArray("mu0",           "+1.256637062120000000000000000000000000000000000000000000e-06"); c++;

  generateConstantArray("muB",           "+9.274010078000000000000000000000000000000000000000000000e-24"); c++;
  generateConstantArray("mue",           "-9.284764704300000000000000000000000000000000000000000000e-24"); c++;
  generateConstantArray("mueOnmuB",      "-1.001159652181280000000000000000000000000000000000000000e+00"); c++;
  generateConstantArray("mun",           "-9.662365000000000000000000000000000000000000000000000000e-27"); c++;
  generateConstantArray("mup",           "+1.410606797360000000000000000000000000000000000000000000e-26"); c++;
  generateConstantArray("muu",           "+5.050783746100000000000000000000000000000000000000000000e-27"); c++;


  generateConstantArray("mumu",          "-4.490448300000000000000000000000000000000000000000000000e-26"); c++;
  generateConstantArray("sigmaB",        "+5.670374419184429453970996731889230875840122970291303682e-08"); c++;
  generateConstantArray("PHI",           "+1.618033988749894848204586834365638117720309179805762862e+00"); c++; // math constant phi = (1 + sqrt(5)) / 2
  generateConstantArray("PHI0",          "+2.067833848461929323081115412147497340171545654934323552e-15"); c++;
  generateConstantArray("omega",         "+7.292115000000000000000000000000000000000000000000000000e-05"); c++;
  generateConstantArray("minusInfinity", "-9.999999999999999999999999999999999999999999999999999999e+9999"); c++; // math "constant"

  generateConstantArray("plusInfinity",  "+9.999999999999999999999999999999999999999999999999999999e+9999"); c++; // math "constant"
  generateConstantArray("0",             "0"); c++;

  // constants from the WP34S project for gamma the function evaluation
  generateConstantArray("gammaC00",     	"+2.506628274631000502415765284810246218192434922852200000e+00");
  generateConstantArray("gammaC01",      "+1.898901420935934892121516421489444871168609546626500000e+10");
  generateConstantArray("gammaC02",      "-1.441562000905355882360184024174589398958958098464000000e+11");
  generateConstantArray("gammaC03",      "+4.960354542573828137004589453751102261431713060461700000e+11");
  generateConstantArray("gammaC04",      "-1.023780406198473219243634817725018768614756637869000000e+12");
  generateConstantArray("gammaC05",      "+1.413597258976513273633654064270590550203826819201000000e+12");
  generateConstantArray("gammaC06",      "-1.379067427882918397935921608473404106184422506006400000e+12");
  generateConstantArray("gammaC07",      "+9.788204370638776727185550760421099285080573468010600000e+11");
  generateConstantArray("gammaC08",      "-5.128994840924296233163734159776272986286618224185900000e+11");
  generateConstantArray("gammaC09",      "+1.993214894537074020805536689790757910433414961972700000e+11");
  generateConstantArray("gammaC10",      "-5.724477320502851934636585463308820853275031385884600000e+10");
  generateConstantArray("gammaC11",      "+1.201655806354758157534702176970523540126160063763500000e+10");
  generateConstantArray("gammaC12",      "-1.809010182477543231013601652705978674843239030982400000e+09");
  generateConstantArray("gammaC13",      "+1.898547541983866894247106006196860226824584577849300000e+08");
  generateConstantArray("gammaC14",      "-1.334263251277484954309483416034294789837141075939300000e+07");
  generateConstantArray("gammaC15",      "+5.933439303341291714765684565665519642875431331800600000e+05");
  generateConstantArray("gammaC16",      "-1.540327280024945239238770671101236126255474738855800000e+04");
  generateConstantArray("gammaC17",      "+2.074489944028394131423303914773173203290039991596900000e+02");
  generateConstantArray("gammaC18",      "-1.209628455273317304906775384272224647465224630149300000e+00");
  generateConstantArray("gammaC19",      "+2.269611174612194091242737654897071322781041945531800000e-03");
  generateConstantArray("gammaC20",      "-7.988885866262706189425849079070082330881632208400100000e-07");
  generateConstantArray("gammaC21",      "+1.657344425195846221060002275840201764559630368746500000e-11");

  // Array angle180
  generateConstantArray("180",           "+1.800000000000000000000000000000000000000000000000000000e+02");
  generateConstantArray("200",          	"+2.000000000000000000000000000000000000000000000000000000e+02");
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

  generateConstantArray("_1",            "-1.000000000000000000000000000000000000000000000000000000e+00");
  generateConstantArray("1e_37",         "+1.000000000000000000000000000000000000000000000000000000e-37");
  generateConstantArray("1e_24",         "+1.000000000000000000000000000000000000000000000000000000e-24");
  generateConstantArray("1e_6",          "+1.000000000000000000000000000000000000000000000000000000e-06");
  generateConstantArray("1e_4",          "+1.000000000000000000000000000000000000000000000000000000e-04");
  generateConstantArray("1on10",         "+1.000000000000000000000000000000000000000000000000000000e-01");
  generateConstantArray("1on3",          "+3.333333333333333333333333333333333333333333333333333333e-01");
  generateConstantArray("egamma",       	"+5.772156649015328606065120900824024310421593359399235988e-01");
  generateConstantArray("ln2",           "+6.931471805599453094172321214581765680755001343602552541e-01");
  generateConstantArray("root2on2",      "+7.071067811865475244008443621048490392848359376884740366e-01");
  generateConstantArray("9on10",        	"+9.000000000000000000000000000000000000000000000000000000e-01");
  generateConstantArray("9on5",          "+1.800000000000000000000000000000000000000000000000000000e+00");
  generateConstantArray("2",             "+2.000000000000000000000000000000000000000000000000000000e+00");
  generateConstantArray("ln10",          "+2.302585092994045684017991454684364207601101488628772976e+00");
  generateConstantArray("3piOn4",        "+2.356194490192344928846982537459627163147877049531329366e+00");
  generateConstantArray("3",             "+3.000000000000000000000000000000000000000000000000000000e+00");
  generateConstantArray("5",             "+5.000000000000000000000000000000000000000000000000000000e+00");
  generateConstantArray("2pi",           "+6.283185307179586476925286766559005768394338798750211642e+00");
  generateConstantArray("10",            "+1.000000000000000000000000000000000000000000000000000000e+01");
  generateConstantArray("20",            "+2.000000000000000000000000000000000000000000000000000000e+01");
  generateConstantArray("21",            "+2.100000000000000000000000000000000000000000000000000000e+01");
  generateConstantArray("gammaR",       	"+2.311891000000000000000000000000000000000000000000000000e+01");
  generateConstantArray("32",            "+3.200000000000000000000000000000000000000000000000000000e+01");
  generateConstantArray("36",            "+3.600000000000000000000000000000000000000000000000000000e+01");
  generateConstantArray("47",            "+4.700000000000000000000000000000000000000000000000000000e+01");
  generateConstantArray("180onPi",      	"+5.729577951308232087679815481410517033240547246656432155e+01");
  generateConstantArray("60",            "+6.000000000000000000000000000000000000000000000000000000e+01");
  generateConstantArray("200onPi",      	"+6.366197723675813430755350534900574481378385829618257950e+01");
  generateConstantArray("204",           "+2.040000000000000000000000000000000000000000000000000000e+02");
  generateConstantArray("360",           "+3.600000000000000000000000000000000000000000000000000000e+02");
  generateConstantArray("400",           "+4.000000000000000000000000000000000000000000000000000000e+02");
  generateConstantArray("1000",          "+1.000000000000000000000000000000000000000000000000000000e+03");
  generateConstantArray("3600",         	"+3.600000000000000000000000000000000000000000000000000000e+03");
  generateConstantArray("9999",          "+9.999000000000000000000000000000000000000000000000000000e+03");
  generateConstantArray("10000",         "+1.000000000000000000000000000000000000000000000000000000e+04");
  generateConstantArray("1e6",           "+1.000000000000000000000000000000000000000000000000000000e+06");
  generateConstantArray("2p32",          "+4.294967296000000000000000000000000000000000000000000000e+09");

  generateConstantArray451("2pi",      		"+6.2831853071795864769252867665590057683943387987502116419498891846156328125724179972560696506842341359"
                                            "6429617302656461329418768921910116446345071881625696223490056820540387704221111928924589790986076392"
                                            "8857621951331866892256951296467573566330542403818291297133846920697220908653296426787214520498282547"
                                            "4491740132126311763497630418419256585081834307287357851807200226610610976409330427682939038830232188"
                                            "66114540731519183906184372234763865223586210237096148924759925499134703771505449782455876366e+00");

  generateConstantArray16("0",           "+0.000000000000000000000000000000000000000000000000000000e+00");
  generateConstantArray16("1e_4",        "+1.000000000000000000000000000000000000000000000000000000e-04");
  generateConstantArray16("1",           "+1.000000000000000000000000000000000000000000000000000000e+00");
  generateConstantArray16("2pi",         "+6.283185307179586476925286766559005768394338798750211642e+00");
  generateConstantArray16("1000",        "+1.000000000000000000000000000000000000000000000000000000e+03");
  generateConstantArray16("1e6",         "+1.000000000000000000000000000000000000000000000000000000e+06");

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

  decContextDefault(&ctxtRealIc,   DEC_INIT_DECQUAD);
  decContextDefault(&ctxtReal16,   DEC_INIT_DECDOUBLE);
  decContextDefault(&ctxtReal34,   DEC_INIT_DECQUAD);
  decContextDefault(&ctxtReal451,  DEC_INIT_DECQUAD);
  ctxtRealIc.digits = DIGITS_FOR_34_DIGITS_INTERMEDIATE_CALCULATIONS;
  ctxtRealIc.traps  = 0;
  ctxtReal451.digits  = 451;
  ctxtReal451.traps   = 0;

  defines[0] = 0;
  externalDeclarations[0] = 0;
  strcat(externalDeclarations, "extern const uint8_t constants[];\n");
  realArray[0] = 0;
  realPointerDeclarations[0]    = 0;
  real16PointerDeclarations[0]  = 0;
  real34PointerDeclarations[0]  = 0;
  real451PointerDeclarations[0] = 0;

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
  fprintf(constantsC, "%s", real451PointerDeclarations);
  fprintf(constantsC, "\n");
  fprintf(constantsC, "%s", real16PointerDeclarations);
  fprintf(constantsC, "\n");
  fprintf(constantsC, "%s", real34PointerDeclarations);

  fclose(constantsC);

  return 0;
}
