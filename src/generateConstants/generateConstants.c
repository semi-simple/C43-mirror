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

#if (IBM_DECIMAL == 1)
 #define DECNUMDIGITS 51
 #include "decimal128.h"
 #include "decimal64.h"
 #include "decDouble.h"
 #include "decQuad.h"
 #include "decNumberWrappers.h"
#endif

real16_t   real16;
real34_t   real34;
real51_t   real51;
decContext ctxtReal16, ctxtReal34, ctxtReal51;
char       whiteSpace[50], temp[1000];
char       defines[1000000], externalDeclarations[1000000]; // .h file
char       realArray[1000000], real16PointerDeclarations[1000000], real34PointerDeclarations[1000000], real51PointerDeclarations[1000000]; // .c file
FILE       *constantsc;
FILE       *constantsh;
int        index, c;

void generateConstant16Array(char *name, char *value) {
  stringToReal16(value, &real16);

  strcpy(whiteSpace, "                                        ");
  whiteSpace[13 - strlen(name)] = 0;

  strcat(externalDeclarations, "extern const real16_t * const const16_");
  strcat(externalDeclarations, name);
  strcat(externalDeclarations, ";\n");

  if(c <= 79) {
    sprintf(temp, "#define CONST_%02d %4d\n", c, index);
    strcat(defines, temp);
  }

  strcat(real16PointerDeclarations, "const real16_t * const const16_");
  strcat(real16PointerDeclarations, name);
  strcat(real16PointerDeclarations, whiteSpace);
  strcat(real16PointerDeclarations, " = (real16_t *)(constants + ");
  sprintf(temp, "%4d)", index);
  index += sizeof(real16_t);
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


void generateConstant34Array(char *name, char *value) {
  stringToReal34(value, &real34);

  strcpy(whiteSpace, "                                        ");
  whiteSpace[13 - strlen(name)] = 0;

  strcat(externalDeclarations, "extern const real34_t * const const34_");
  strcat(externalDeclarations, name);
  strcat(externalDeclarations, ";\n");

  strcat(real34PointerDeclarations, "const real34_t * const const34_");
  strcat(real34PointerDeclarations, name);
  strcat(real34PointerDeclarations, whiteSpace);
  strcat(real34PointerDeclarations, " = (real34_t *)(constants + ");
  sprintf(temp, "%4d)", index);
  index += sizeof(real34_t);
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


void generateConstant51Array(char *name, char *value) {
  stringToReal51(value, &real51);

  strcpy(whiteSpace, "                                        ");
  whiteSpace[13 - strlen(name)] = 0;

  strcat(externalDeclarations, "extern const real51_t * const const51_");
  strcat(externalDeclarations, name);
  strcat(externalDeclarations, ";\n");

  strcat(real51PointerDeclarations, "const real51_t * const const51_");
  strcat(real51PointerDeclarations, name);
  strcat(real51PointerDeclarations, whiteSpace);
  strcat(real51PointerDeclarations, " = (real51_t *)(constants + ");
  sprintf(temp, "%4d)", index);
  index += sizeof(real51_t);
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


void generateAllConstants(void) {
  // constants used by the program
  index = 0;

  strcat(realArray, "const uint8_t constants[] = {\n");

  // Physical and mathematical constants
  // each constant has 55 significant digits
  c = 0;
  generateConstant16Array("0_5",           "+5.000000000000000000000000000000000000000000000000000000e-01"); c++; // math constant 1/2
  generateConstant16Array("a",             "+3.652425000000000000000000000000000000000000000000000000e+02"); c++; // per definition
  generateConstant16Array("a0",            "+5.291772106700000000000000000000000000000000000000000000e-11"); c++;
  generateConstant16Array("aM",            "+3.844000000000000000000000000000000000000000000000000000e+08"); c++;
  generateConstant16Array("aEarth",        "+1.495979000000000000000000000000000000000000000000000000e+11"); c++;
  generateConstant16Array("c",             "+2.997924580000000000000000000000000000000000000000000000e+08"); c++; // per definition

  generateConstant16Array("c1",            "+3.741771790000000000000000000000000000000000000000000000e-16"); c++;
  generateConstant16Array("c2",            "+1.438777360000000000000000000000000000000000000000000000e-02"); c++;
  generateConstant16Array("e",             "+1.602176620800000000000000000000000000000000000000000000e-19"); c++;
  generateConstant16Array("eE",            "+2.718281828459045235360287471352662497757247093699959575e+00"); c++; // math constant e
  generateConstant16Array("eme",           "+1.758820024000000000000000000000000000000000000000000000e+11"); c++;
  generateConstant16Array("F",             "+9.648533289000000000000000000000000000000000000000000000e+04"); c++;

  generateConstant16Array("Falpha",        "+2.502907875095892822283902873218215786381271376727149977e+00"); c++; // math constant Falpha
  generateConstant16Array("Fdelta",        "+4.669201609102990671853203820466201617258185577475768633e+00"); c++; // math constant Fdelta
  generateConstant16Array("G",             "+6.674080000000000000000000000000000000000000000000000000e-11"); c++;
  generateConstant16Array("G0",            "+7.748091731000000000000000000000000000000000000000000000e-05"); c++;
  generateConstant16Array("GC",            "+9.159655941772190150546035149323841107741493742816721343e-01"); c++; // math constant Catalan
  generateConstant16Array("ge",            "-2.002319304361820000000000000000000000000000000000000000e+00"); c++;


  generateConstant16Array("GM",            "+3.986004418000000000000000000000000000000000000000000000e+14"); c++;
  generateConstant16Array("gEarth",        "+9.806650000000000000000000000000000000000000000000000000e+00"); c++; // per definition
  generateConstant16Array("Planck",        "+6.626070040000000000000000000000000000000000000000000000e-34"); c++;
  generateConstant16Array("PlanckOn2pi",   "+1.054571800000000000000000000000000000000000000000000000e-34"); c++;
  generateConstant16Array("k",             "+1.380648520000000000000000000000000000000000000000000000e-23"); c++;
  generateConstant16Array("KJ",            "+4.835978525000000000000000000000000000000000000000000000e+14"); c++;

  generateConstant16Array("lp",            "+1.616229000000000000000000000000000000000000000000000000e-35"); c++;
  generateConstant16Array("me",            "+9.109383560000000000000000000000000000000000000000000000e-31"); c++;
  generateConstant16Array("MM",            "+7.349000000000000000000000000000000000000000000000000000e+22"); c++;
  generateConstant16Array("mn",            "+1.674927471000000000000000000000000000000000000000000000e-27"); c++;
  generateConstant16Array("mnOnmp",        "+1.001378418980000000000000000000000000000000000000000000e+00"); c++;
  generateConstant16Array("Mp",            "+2.176470000000000000000000000000000000000000000000000000e-08"); c++;

  generateConstant16Array("mp",            "+1.672621898000000000000000000000000000000000000000000000e-27"); c++;
  generateConstant16Array("mpOnme",        "+1.836152673890000000000000000000000000000000000000000000e+03"); c++;
  generateConstant16Array("mu",            "+1.660539040000000000000000000000000000000000000000000000e-27"); c++;
  generateConstant16Array("muc2",          "+1.492418062000000000000000000000000000000000000000000000e-10"); c++;
  generateConstant16Array("mmu",           "+1.883531594000000000000000000000000000000000000000000000e-28"); c++;
  generateConstant16Array("mSun",          "+1.989100000000000000000000000000000000000000000000000000e+30"); c++;


  generateConstant16Array("mEarth",        "+5.973600000000000000000000000000000000000000000000000000e+24"); c++;
  generateConstant16Array("NA",            "+6.022140857000000000000000000000000000000000000000000000e+23"); c++;
  generateConstant16Array("NaN",           "Not a number"                                                 ); c++;
  generateConstant16Array("p0",            "+1.013250000000000000000000000000000000000000000000000000e+05"); c++; // per definition
  generateConstant16Array("R",             "+8.314459800000000000000000000000000000000000000000000000e+00"); c++;
  generateConstant16Array("re",            "+2.817940322700000000000000000000000000000000000000000000e-15"); c++;

  generateConstant16Array("RK",            "+2.581280745550000000000000000000000000000000000000000000e+04"); c++;
  generateConstant16Array("RM",            "+1.737530000000000000000000000000000000000000000000000000e+06"); c++;
  generateConstant16Array("RInfinity",     "+1.097373156850800000000000000000000000000000000000000000e+07"); c++;
  generateConstant16Array("RSun",          "+6.960000000000000000000000000000000000000000000000000000e+08"); c++;
  generateConstant16Array("REarth",        "+6.371010000000000000000000000000000000000000000000000000e+06"); c++;
  generateConstant16Array("Sa",            "+6.378137000000000000000000000000000000000000000000000000e+06"); c++; // per definition

  generateConstant16Array("Sb",            "+6.356752314200000000000000000000000000000000000000000000e+06"); c++;
  generateConstant16Array("Se2",           "+6.694379990140000000000000000000000000000000000000000000e-03"); c++;
  generateConstant16Array("Sep2",          "+6.739496742280000000000000000000000000000000000000000000e-03"); c++;
  generateConstant16Array("Sfm1",          "+2.982572235630000000000000000000000000000000000000000000e+02"); c++; // per definition
  generateConstant16Array("T0",            "+2.731500000000000000000000000000000000000000000000000000e+02"); c++; // per definition
  generateConstant16Array("TP",            "+1.416808000000000000000000000000000000000000000000000000e+32"); c++;


  generateConstant16Array("tp",            "+5.391160000000000000000000000000000000000000000000000000e-44"); c++;
  generateConstant16Array("Vm",            "+2.241396200000000000000000000000000000000000000000000000e-02"); c++;
  generateConstant16Array("Z0",            "+3.767303134617706554681984004203193082686235083524186552e+02"); c++; // mu0 * c
  generateConstant16Array("alpha",         "+7.297352566400000000000000000000000000000000000000000000e-03"); c++;
                                                                                                                         c++; // gamma = G
  generateConstant16Array("gammaEM",       "+5.772156649015328606065120900824024310421593359399235988e-01"); c++;  // math constant Euler-Mascheroni

  generateConstant16Array("gammap",        "+2.675221900000000000000000000000000000000000000000000000e+08"); c++;
  generateConstant16Array("epsilon0",      "+8.854187817620389850536563031710750260608370166599449808e-12"); c++; // per definition: 1 / (mu0 * c^2)
  generateConstant16Array("lambdaC",       "+2.426310236700000000000000000000000000000000000000000000e-12"); c++;
  generateConstant16Array("lambdaCn",      "+1.319590904810000000000000000000000000000000000000000000e-15"); c++;
  generateConstant16Array("lambdaCp",      "+1.321409853960000000000000000000000000000000000000000000e-15"); c++;
  generateConstant16Array("mu0",           "+1.256637061435917295385057353311801153678867759750042328e-06"); c++; // per definition: 4 * pi * 10^(-7)

  generateConstant16Array("muB",           "+9.274009994000000000000000000000000000000000000000000000e-24"); c++;
  generateConstant16Array("mue",           "-9.284764620000000000000000000000000000000000000000000000e-24"); c++;
  generateConstant16Array("mueOnmuB",      "-1.001159652180910000000000000000000000000000000000000000e+00"); c++;
  generateConstant16Array("mun",           "-9.662365000000000000000000000000000000000000000000000000e-27"); c++;
  generateConstant16Array("mup",           "+1.410606787300000000000000000000000000000000000000000000e-26"); c++;
  generateConstant16Array("muu",           "+5.050783699000000000000000000000000000000000000000000000e-27"); c++;


  generateConstant16Array("mumu",          "-4.490448260000000000000000000000000000000000000000000000e-26"); c++;
  generateConstant16Array("sigmaB",        "+5.670367000000000000000000000000000000000000000000000000e-08"); c++;
  generateConstant16Array("PHI",           "+1.618033988749894848204586834365638117720309179805762862e+00"); c++; // math constant phi = (1 + sqrt(5)) / 2
  generateConstant16Array("PHI0",          "+2.067833831000000000000000000000000000000000000000000000e-15"); c++;
  generateConstant16Array("omega",         "+7.292115000000000000000000000000000000000000000000000000e-05"); c++;
  generateConstant16Array("minusInfinity", "-9.999999999999999999999999999999999999999999999999999999e+9999"); c++; // math "constant"

  generateConstant16Array("plusInfinity",  "+9.999999999999999999999999999999999999999999999999999999e+9999"); c++; // math "constant"
  generateConstant16Array("0",             "+0.000000000000000000000000000000000000000000000000000000e+00"); c++;


  generateConstant16Array("1e_4",          "+1.000000000000000000000000000000000000000000000000000000e-04");
  generateConstant16Array("1",             "+1.000000000000000000000000000000000000000000000000000000e+00");
  generateConstant16Array("pi",            "+3.141592653589793238462643383279502884197169399375105821e+00");
  generateConstant16Array("2pi",           "+6.283185307179586476925286766559005768394338798750211642e+00");
  generateConstant16Array("10",            "+1.000000000000000000000000000000000000000000000000000000e+01");
  generateConstant16Array("20",            "+2.000000000000000000000000000000000000000000000000000000e+01");
  generateConstant16Array("32",            "+3.200000000000000000000000000000000000000000000000000000e+01");
  generateConstant16Array("1000",          "+1.000000000000000000000000000000000000000000000000000000e+03");
  generateConstant16Array("9999",          "+9.999000000000000000000000000000000000000000000000000000e+03");
  generateConstant16Array("1e6",           "+1.000000000000000000000000000000000000000000000000000000e+06");





  // All the formulas are 100% exact conversion formulas
  generateConstant34Array("FlozukToM3",    "+2.841306250000000000000000000000000000000000000000000000e-05"); // m^3     = flozuk * 4,54609e-3 / 160
  generateConstant34Array("FlozusToM3",    "+2.957352956250000000000000000000000000000000000000000000e-05"); // m^3     = flozus * 231 * 0.0254^3 / 128
  generateConstant34Array("PointsToM",     "+3.527777777777777777777777777777777777777777777777777778e-04"); // m       = pt * 0.0254 / 72
  generateConstant34Array("GalusToM3",     "+3.785411784000000000000000000000000000000000000000000000e-03"); // m^3     = galus * 321 * 0.0254^3
  generateConstant34Array("GalukToM3",     "+4.546090000000000000000000000000000000000000000000000000e-03"); // m^3     = galuk * 4,54609e-3
  generateConstant34Array("QuartToM3",     "+1.136522500000000000000000000000000000000000000000000000e-03"); // m^3     = quart * 4,54609e-3 / 4
  generateConstant34Array("InchesToM",     "+2.540000000000000000000000000000000000000000000000000000e-02"); // m       = inches * 0,0254
  generateConstant34Array("OzToKg",        "+2.834952312500000000000000000000000000000000000000000000e-02"); // kg      = oz * (0,45359237 / 16)
  generateConstant34Array("TrozToKg",      "+3.110347680000000000000000000000000000000000000000000000e-02"); // kg      = tr.oz * 0.45359237 * 175 / 12
  generateConstant34Array("FtToM",         "+3.048000000000000000000000000000000000000000000000000000e-01"); // m       = ft * 12 * 0.0254
  generateConstant34Array("SfeetToM",      "+3.048006096012192024384048768097536195072390144780289561e-01"); // m       = sfeetus * (1200 / 3937)
  generateConstant34Array("LbsToKg",       "+4.535923700000000000000000000000000000000000000000000000e-01"); // kg      = lbs * 0,45359237     (1 Lbs (pound) = 16 oz)
  generateConstant34Array("YardsToM",      "+9.144000000000000000000000000000000000000000000000000000e-01"); // m       = yards * 36 * 0.0254
  generateConstant34Array("CalToJ",        "+4.186800000000000000000000000000000000000000000000000000e+00"); // joules  = calories * 4,1868
  generateConstant34Array("StonesToKg",    "+6.350293180000000000000000000000000000000000000000000000e+00"); // kg      = stones * 14 * 0,45359237
  generateConstant34Array("LbfToN",        "+4.448221615260500000000000000000000000000000000000000000e+00"); // newtons = lbf * 9,80665 * 0,45359237
  generateConstant34Array("ShortcwtToKg",  "+4.535923700000000000000000000000000000000000000000000000e+01"); // kg      = short cwt * 100 * 0.45359237 (short cwt = short hundredweight)
  generateConstant34Array("CwtToKg",       "+5.080234544000000000000000000000000000000000000000000000e+01"); // kg      = cwt * 112 * 0,45359237       (cwt = long hundredWeight)
  generateConstant34Array("TorrToPa",      "+1.333223684210526315789473684210526315789473684210526316e+02"); // pascals = torr * 101325 / 760
  generateConstant34Array("HpmToW",        "+7.354987500000000000000000000000000000000000000000000000e+02"); // watts   = HPM * 75 * 9,80665
  generateConstant34Array("HpukToW",       "+7.456998715822702200000000000000000000000000000000000000e+02"); // watts   = HPUK * 550 * 0,3048 * 9,80665 * 0,45359237
  generateConstant34Array("ShorttonsToKg", "+9.071847400000000000000000000000000000000000000000000000e+02"); // kg      = short tons * 2000 * 0,45359237
  generateConstant34Array("HpeToW",        "+7.460000000000000000000000000000000000000000000000000000e+02"); // watts   = HPE * 746
  generateConstant34Array("TonsToKg",      "+1.016046908800000000000000000000000000000000000000000000e+03"); // kg      = tons * 2240 * 0,45359237
  generateConstant34Array("BtuToJ",        "+1.055055852620000000000000000000000000000000000000000000e+03"); // joules  = Btu * 1055,05585262
  generateConstant34Array("MiToM",         "+1.609344000000000000000000000000000000000000000000000000e+03"); // m       = miles * 63360 * 0.0254
  generateConstant34Array("NmiToM",        "+1.852000000000000000000000000000000000000000000000000000e+03"); // m       = nmi * 1852
  generateConstant34Array("InhgToPa",      "+3.386388157894736842105263157894736842105263157894736842e+03"); // pascals = in.Hg * 101325 / 760 * 25.4
  generateConstant34Array("WhToJ",         "+3.600000000000000000000000000000000000000000000000000000e+03"); // joules  = Wh * 3600
  generateConstant34Array("AccresToM2",    "+4.046856422400000000000000000000000000000000000000000000e+03"); // m^2 =     acres * 0.0254^2 * 12^2 * 43560
  generateConstant34Array("AccresusToM2",  "+4.046872609874252006568529266090790246096621225500515517e+03"); // m^2 =     acresus * (1200 / 3937)^2 * 43560
  generateConstant34Array("PsiToPa",       "+6.894757293168361336722673445346890693781387562775125550e+03"); // pascals = psi * 0.45359237 * 9.80665 / 0.0254^2
  generateConstant34Array("BarToPa",       "+1.000000000000000000000000000000000000000000000000000000e+05"); // pascals = bars * 100000
  generateConstant34Array("AtmToPa",       "+1.013250000000000000000000000000000000000000000000000000e+05"); // pascals = atm * 101325
  generateConstant34Array("YearToS",       "+3.155695200000000000000000000000000000000000000000000000e+07"); // seconds = years * (365,2425 * 24 * 3600)
  generateConstant34Array("AuToM",         "+1.495978707000000000000000000000000000000000000000000000e+11"); // m       = au * 149597870700
  generateConstant34Array("LyToM",         "+9.460730472580800000000000000000000000000000000000000000e+15"); // m       = ly * 299792458 * 3600 * 24 * 365.25
  generateConstant34Array("PcToM",         "+3.085677581491367278913937957796471610731921160409179801e+16"); // m       = pc * 149597870700 * 648000 / pi

  generateConstant34Array("NaN",           "Not a Number"                                                 );

  //generateConstant34Array("minusInfinity", "-9.999999999999999999999999999999999999999999999999999999e+9999");
  generateConstant34Array("minusPi",       "-3.141592653589793238462643383279502884197169399375105821e+00");
  generateConstant34Array("0",             "+0.000000000000000000000000000000000000000000000000000000e+00");
  generateConstant34Array("1e_24",         "+1.000000000000000000000000000000000000000000000000000000e-24");
  generateConstant34Array("1e_4",          "+1.000000000000000000000000000000000000000000000000000000e-04");
  generateConstant34Array("piOn200",       "+1.570796326794896619231321691639751442098584699687552910e-02"); // pi / 200
  generateConstant34Array("piOn180",       "+1.745329251994329576923690768488612713442871888541725456e-02"); // pi / 180
  generateConstant34Array("0_1",           "+1.000000000000000000000000000000000000000000000000000000e-01");
  //generateConstant34Array("0_25",          "+2.500000000000000000000000000000000000000000000000000000e-01");
  generateConstant34Array("0_5",           "+5.000000000000000000000000000000000000000000000000000000e-01");
  generateConstant34Array("egamma",      		"+5.772156649015328606065120900824024310421593359399235988e-01");
  generateConstant34Array("0_6",           "+6.000000000000000000000000000000000000000000000000000000e-01");
  generateConstant34Array("root2on2",      "+7.071067811865475244008443621048490392848359376884740366e-01");
  generateConstant34Array("piOn4",         "+7.853981633974483096156608458198757210492923498437764552e-01");
  generateConstant34Array("0_9",           "+9.000000000000000000000000000000000000000000000000000000e-01");
  generateConstant34Array("1",             "+1.000000000000000000000000000000000000000000000000000000e+00");
  generateConstant34Array("piOn2",         "+1.570796326794896619231321691639751442098584699687552910e+00");
  generateConstant34Array("1_8",           "+1.800000000000000000000000000000000000000000000000000000e+00");
  generateConstant34Array("2",             "+2.000000000000000000000000000000000000000000000000000000e+00");
  generateConstant34Array("3piOn4",        "+2.356194490192344928846982537459627163147877049531329366e+00");
  generateConstant34Array("3",             "+3.000000000000000000000000000000000000000000000000000000e+00");
  generateConstant34Array("pi",            "+3.141592653589793238462643383279502884197169399375105821e+00");
  generateConstant34Array("5",             "+5.000000000000000000000000000000000000000000000000000000e+00");
  generateConstant34Array("2pi",           "+6.283185307179586476925286766559005768394338798750211642e+00");
  generateConstant34Array("10",            "+1.000000000000000000000000000000000000000000000000000000e+01");
  generateConstant34Array("20",            "+2.000000000000000000000000000000000000000000000000000000e+01");
  generateConstant34Array("32",            "+3.200000000000000000000000000000000000000000000000000000e+01");
  generateConstant34Array("36",            "+3.600000000000000000000000000000000000000000000000000000e+01");
  //generateConstant34Array("45",            "+4.500000000000000000000000000000000000000000000000000000e+01");
  //generateConstant34Array("50",            "+5.000000000000000000000000000000000000000000000000000000e+01");
  generateConstant34Array("60",            "+6.000000000000000000000000000000000000000000000000000000e+01");
  //generateConstant34Array("90",            "+9.000000000000000000000000000000000000000000000000000000e+01");
  generateConstant34Array("100",           "+1.000000000000000000000000000000000000000000000000000000e+02");
  generateConstant34Array("180",           "+1.800000000000000000000000000000000000000000000000000000e+02");
  generateConstant34Array("200",           "+2.000000000000000000000000000000000000000000000000000000e+02");
  generateConstant34Array("204",           "+2.040000000000000000000000000000000000000000000000000000e+02");
  //generateConstant34Array("360",           "+3.600000000000000000000000000000000000000000000000000000e+02");
  //generateConstant34Array("400",           "+4.000000000000000000000000000000000000000000000000000000e+02");
  generateConstant34Array("9000",          "+9.000000000000000000000000000000000000000000000000000000e+03");
  generateConstant34Array("10000",         "+1.000000000000000000000000000000000000000000000000000000e+04");
  generateConstant34Array("1e6",           "+1.000000000000000000000000000000000000000000000000000000e+06");
  generateConstant34Array("plusInfinity",  "+9.999999999999999999999999999999999999999999999999999999e+9999");

  generateConstant51Array("minusInfinity", "-9.999999999999999999999999999999999999999999999999999999e+9999");
  generateConstant51Array("NaN",           "Not a Number"                                                 );
  generateConstant51Array("0",             "+0.000000000000000000000000000000000000000000000000000000e+00");
  generateConstant51Array("1e_37",         "+1.000000000000000000000000000000000000000000000000000000e-37");
  generateConstant51Array("0_5",           "+5.000000000000000000000000000000000000000000000000000000e-01");
  generateConstant51Array("ln2",           "+6.931471805599453094172321214581765680755001343602552541e+01");
  generateConstant51Array("root2on2",      "+7.071067811865475244008443621048490392848359376884740366e-01");
  generateConstant51Array("1",             "+1.000000000000000000000000000000000000000000000000000000e+00");
  generateConstant51Array("2",             "+2.000000000000000000000000000000000000000000000000000000e+00");
  generateConstant51Array("ln10",          "+2.302585092994045684017991454684364207601101488628772976e+00");
  generateConstant51Array("3",             "+3.000000000000000000000000000000000000000000000000000000e+00");
  generateConstant51Array("21",            "+2.100000000000000000000000000000000000000000000000000000e+01");
  generateConstant51Array("gammaR",      		"+2.311891000000000000000000000000000000000000000000000000e+01");
  generateConstant51Array("plusInfinity",  "+9.999999999999999999999999999999999999999999999999999999e+9999");

  // constants from the WP34S project
  generateConstant51Array("gammaC00",    		"+2.506628274631000502415765284810246218192434922852200000e+00");
  //generateConstant51Array("gammaC01",    		"+1.898901420935934892121516421489444871168609546626500000e+10");
  //generateConstant51Array("gammaC02",    	 "-1.441562000905355882360184024174589398958958098464000000e+11");
  //generateConstant51Array("gammaC03",    		"+4.960354542573828137004589453751102261431713060461700000e+11");
  //generateConstant51Array("gammaC04",    		"-1.023780406198473219243634817725018768614756637869000000e+12");
  //generateConstant51Array("gammaC05",    		"+1.413597258976513273633654064270590550203826819201000000e+12");
  //generateConstant51Array("gammaC06",    		"-1.379067427882918397935921608473404106184422506006400000e+12");
  //generateConstant51Array("gammaC07",    		"+9.788204370638776727185550760421099285080573468010600000e+11");
  //generateConstant51Array("gammaC08",    		"-5.128994840924296233163734159776272986286618224185900000e+11");
  //generateConstant51Array("gammaC09",    		"+1.993214894537074020805536689790757910433414961972700000e+11");
  //generateConstant51Array("gammaC10",    		"-5.724477320502851934636585463308820853275031385884600000e+10");
  //generateConstant51Array("gammaC11",    		"+1.201655806354758157534702176970523540126160063763500000e+10");
  //generateConstant51Array("gammaC12",    		"-1.809010182477543231013601652705978674843239030982400000e+09");
  //generateConstant51Array("gammaC13",    		"+1.898547541983866894247106006196860226824584577849300000e+08");
  //generateConstant51Array("gammaC14",    		"-1.334263251277484954309483416034294789837141075939300000e+07");
  //generateConstant51Array("gammaC15",    		"+5.933439303341291714765684565665519642875431331800600000e+05");
  //generateConstant51Array("gammaC16",    		"-1.540327280024945239238770671101236126255474738855800000e+04");
  //generateConstant51Array("gammaC17",    		"+2.074489944028394131423303914773173203290039991596900000e+02");
  //generateConstant51Array("gammaC18",    		"-1.209628455273317304906775384272224647465224630149300000e+00");
  //generateConstant51Array("gammaC19",    		"+2.269611174612194091242737654897071322781041945531800000e-03");
  //generateConstant51Array("gammaC20",    		"-7.988885866262706189425849079070082330881632208400100000e-07");
  //generateConstant51Array("gammaC21",    		"+1.657344425195846221060002275840201764559630368746500000e-11");

  strcat(realArray, "};\n");
}


int main(void) {
  decContextDefault(&ctxtReal16, DEC_INIT_DECDOUBLE);
  decContextDefault(&ctxtReal34, DEC_INIT_DECQUAD);
  decContextDefault(&ctxtReal51, DEC_INIT_DECQUAD);
  ctxtReal51.digits = 51;
  ctxtReal51.traps = 0;

  defines[0] = 0;
  externalDeclarations[0] = 0;
  strcat(externalDeclarations, "extern const uint8_t constants[];\n");
  realArray[0] = 0;
  real16PointerDeclarations[0] = 0;
  real34PointerDeclarations[0] = 0;
  real51PointerDeclarations[0] = 0;

  generateAllConstants(); // x86


  constantsh = fopen("src/wp43s/constantPointers.h", "wb");
  if(constantsh == NULL) {
    fprintf(stderr, "Cannot create file src/wp43s/constantPointers.h\n");
    exit(1);
  }

  fprintf(constantsh, "/* This file is part of 43S.\n");
  fprintf(constantsh, " *\n");
  fprintf(constantsh, " * 43S is free software: you can redistribute it and/or modify\n");
  fprintf(constantsh, " * it under the terms of the GNU General Public License as published by\n");
  fprintf(constantsh, " * the Free Software Foundation, either version 3 of the License, or\n");
  fprintf(constantsh, " * (at your option) any later version.\n");
  fprintf(constantsh, " *\n");
  fprintf(constantsh, " * 43S is distributed in the hope that it will be useful,\n");
  fprintf(constantsh, " * but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
  fprintf(constantsh, " * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n");
  fprintf(constantsh, " * GNU General Public License for more details.\n");
  fprintf(constantsh, " *\n");
  fprintf(constantsh, " * You should have received a copy of the GNU General Public License\n");
  fprintf(constantsh, " * along with 43S.  If not, see <http://www.gnu.org/licenses/>.\n");
  fprintf(constantsh, " */\n\n");

  fprintf(constantsh, "/********************************************//**\n");
  fprintf(constantsh, "* \\file constantPointers.h constants\n");
  fprintf(constantsh, "***********************************************/\n\n");

  fprintf(constantsh, "/************************************************************************************************\n");
  fprintf(constantsh, "* Do not edit this file manually! It's automagically generated by the program generateConstants *\n");
  fprintf(constantsh, "*************************************************************************************************/\n\n");

  fprintf(constantsh, defines);
  fprintf(constantsh, "\n");
  fprintf(constantsh, externalDeclarations);

  fclose(constantsh);



  constantsc = fopen("src/wp43s/constantPointers.c", "wb");
  if(constantsc == NULL) {
    fprintf(stderr, "Cannot create file src/wp43s/constantPointers.c\n");
    exit(1);
  }

  fprintf(constantsc, "/* This file is part of 43S.\n");
  fprintf(constantsc, " *\n");
  fprintf(constantsc, " * 43S is free software: you can redistribute it and/or modify\n");
  fprintf(constantsc, " * it under the terms of the GNU General Public License as published by\n");
  fprintf(constantsc, " * the Free Software Foundation, either version 3 of the License, or\n");
  fprintf(constantsc, " * (at your option) any later version.\n");
  fprintf(constantsc, " *\n");
  fprintf(constantsc, " * 43S is distributed in the hope that it will be useful,\n");
  fprintf(constantsc, " * but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
  fprintf(constantsc, " * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n");
  fprintf(constantsc, " * GNU General Public License for more details.\n");
  fprintf(constantsc, " *\n");
  fprintf(constantsc, " * You should have received a copy of the GNU General Public License\n");
  fprintf(constantsc, " * along with 43S.  If not, see <http://www.gnu.org/licenses/>.\n");
  fprintf(constantsc, " */\n\n");

  fprintf(constantsc, "/********************************************//**\n");
  fprintf(constantsc, "* \\file constantPointers.c constants\n");
  fprintf(constantsc, "***********************************************/\n\n");

  fprintf(constantsc, "/************************************************************************************************\n");
  fprintf(constantsc, "* Do not edit this file manually! It's automagically generated by the program generateConstants *\n");
  fprintf(constantsc, "*************************************************************************************************/\n\n");

  fprintf(constantsc, "#include \"wp43s.h\"\n\n");

  fprintf(constantsc, realArray);
  fprintf(constantsc, "\n");
  fprintf(constantsc, real16PointerDeclarations);
  fprintf(constantsc, "\n");
  fprintf(constantsc, real34PointerDeclarations);
  fprintf(constantsc, "\n");
  fprintf(constantsc, real51PointerDeclarations);

  fclose(constantsc);

  return 0;
}
