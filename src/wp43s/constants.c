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
 * \file constants.c List of constant description
 * see: https://physics.nist.gov/cuu/index.html
 ***********************************************/

#include "wp43s.h"

const constant_t physicalAndMathConstants[] = {
/*   0 */  {.label = "1/2",                                 .value = CONST_00}, // 0.5 trivial but helpfull
/*   1 */  {.label = "a",                                   .value = CONST_01}, // Gregorian year (days) per definition
/*   2 */  {.label = "a" STD_SUB_0,                         .value = CONST_02}, // Bohr radius
/*   3 */  {.label = "a" STD_SUB_M,                         .value = CONST_03}, // Semi major axis of the Moon's orbit
/*   4 */  {.label = "a" STD_SUB_EARTH,                     .value = CONST_04}, // Semi major axis of the Earth's orbit
/*   5 */  {.label = "c",                                   .value = CONST_05}, // Speed of light in vaccum (per definition)
/*   6 */  {.label = "c" STD_SUB_1,                         .value = CONST_06}, // 1st radiation constant
/*   7 */  {.label = "c" STD_SUB_2,                         .value = CONST_07}, // 2nd radiation constant
/*   8 */  {.label = "e",                                   .value = CONST_08}, // Electron charge
/*   9 */  {.label = "e" STD_SUB_E,                         .value = CONST_09}, // Euler's e
/*  10 */  {.label = "e/m" STD_SUB_e,                       .value = CONST_10}, // Electron charge to mass ratio
/*  11 */  {.label = "F",                                   .value = CONST_11}, // Faraday's constant,
/*  12 */  {.label = "F" STD_SUB_alpha,                     .value = CONST_12}, // Feigenbaum's alpha
/*  13 */  {.label = "F" STD_SUB_delta,                     .value = CONST_13}, // Feigenbaum's delta
/*  14 */  {.label = "G",                                   .value = CONST_14}, // Newtonian constant of gravitation
/*  15 */  {.label = "G" STD_SUB_0,                         .value = CONST_15}, // Conductance quantum
/*  16 */  {.label = "G" STD_SUB_C,                         .value = CONST_16}, // Catalan's constant
/*  17 */  {.label = "g" STD_SUB_e,                         .value = CONST_17}, // Landé's electron g-factor
/*  18 */  {.label = "GM" STD_SUB_EARTH,                    .value = CONST_18}, // Newtonian constant of gravitation times Earth's mass with its atmosphere included
/*  19 */  {.label = "g" STD_SUB_EARTH,                     .value = CONST_19}, // Standard Earth acceleration (per definition)
/*  20 */  {.label = STD_PLANCK,                            .value = CONST_20}, // Planch constant
/*  21 */  {.label = STD_PLANCK_2PI,                        .value = CONST_21}, // h/2pi
/*  22 */  {.label = "k",                                   .value = CONST_22}, // Bolzmann constant
/*  23 */  {.label = "K" STD_SUB_J,                         .value = CONST_23}, // Josephson constant
/*  24 */  {.label = "l" STD_SUB_P STD_SUB_L,               .value = CONST_24}, // Planck length
/*  25 */  {.label = "m" STD_SUB_e,                         .value = CONST_25}, // Electron mass
/*  26 */  {.label = "M" STD_SUB_M,                         .value = CONST_26}, // Mass of the Moon
/*  27 */  {.label = "m" STD_SUB_n,                         .value = CONST_27}, // Neutron mass
/*  28 */  {.label = "m" STD_SUB_n "/m" STD_SUB_p,          .value = CONST_28}, // Neutron to proton mass ratio
/*  29 */  {.label = "m" STD_SUB_p,                         .value = CONST_29}, // Proton mass
/*  30 */  {.label = "M" STD_SUB_P STD_SUB_L,               .value = CONST_30}, // Planck mass,
/*  31 */  {.label = "m" STD_SUB_p "/m" STD_SUB_e,          .value = CONST_31}, // Proton to electron mass ratio
/*  32 */  {.label = "m" STD_SUB_u,                         .value = CONST_32}, // Atomic mass unit
/*  33 */  {.label = "m" STD_SUB_u "c" STD_SUP_2,           .value = CONST_33}, // Energy equivalent of atomic mass unit
/*  34 */  {.label = "m" STD_SUB_mu,                        .value = CONST_34}, // Muon mass
/*  35 */  {.label = "M" STD_SUB_SUN,                       .value = CONST_35}, // Mass of the Sun
/*  36 */  {.label = "M" STD_SUB_EARTH,                     .value = CONST_36}, // Mass of the Earth
/*  37 */  {.label = "N" STD_SUB_A,                         .value = CONST_37}, // Avogadro's number
/*  38 */  {.label = "NaN",                                 .value = CONST_38}, // Not a Number
/*  39 */  {.label = "p" STD_SUB_0,                         .value = CONST_39}, // Standard atmospheric pressure (per definition)
/*  40 */  {.label = "R",                                   .value = CONST_40}, // Molar gas constant
/*  41 */  {.label = "r" STD_SUB_e,                         .value = CONST_41}, // Classical electron radius
/*  42 */  {.label = "R" STD_SUB_K,                         .value = CONST_42}, // von Klitzing constant
/*  43 */  {.label = "R" STD_SUB_M,                         .value = CONST_43}, // Mean radius of the Moon
/*  44 */  {.label = "R" STD_SUB_INFINITY,                  .value = CONST_44}, // Rydberg constant
/*  45 */  {.label = "R" STD_SUB_SUN,                       .value = CONST_45}, // Mean radius of the Sun
/*  46 */  {.label = "R" STD_SUB_EARTH,                     .value = CONST_46}, // Mean radius of the Earth
/*  47 */  {.label = "Sa",                                  .value = CONST_47}, // Semi-major axis of the model WGS84 used to define Earth's surface for GPS and other surveying purposes (per definition)
/*  48 */  {.label = "Sb",                                  .value = CONST_48}, // Semi-minor axis of WGS84
/*  49 */  {.label = "Se" STD_SUP_2,                        .value = CONST_49}, // 1st eccentricity squared of WGS84
/*  50 */  {.label = "Se'" STD_SUP_2,                       .value = CONST_50}, // 2nd eccentricity squared of WGS84
/*  51 */  {.label = "Sf" STD_SUP_MINUS_1,                  .value = CONST_51}, // Flattening parameter of WGS84 (per definition)
/*  52 */  {.label = "T" STD_SUB_0,                         .value = CONST_52}, // 0°C standard temperature (per definition)
/*  53 */  {.label = "T" STD_SUB_P,                         .value = CONST_53}, // Planck temperature
/*  54 */  {.label = "t" STD_SUB_P STD_SUB_L,               .value = CONST_54}, // Planck time
/*  55 */  {.label = "V" STD_SUB_m,                         .value = CONST_55}, // Molar volume of an ideal gas at standard conditions
/*  56 */  {.label = "Z" STD_SUB_0,                         .value = CONST_56}, // Characteristic impedance of vaccum
/*  57 */  {.label = STD_alpha,                             .value = CONST_57}, // Fine-structure constant
/*  58 */  {.label = STD_gamma,                             .value = CONST_14}, // Newtonian constant of gravitation
/*  59 */  {.label = STD_gamma "EM",                        .value = CONST_59}, // Euler-Mascheroni constant
/*  60 */  {.label = STD_gamma STD_SUB_p,                   .value = CONST_60}, // Proton gyromagnetic ratio
/*  61 */  {.label = STD_epsilon STD_SUB_0,                 .value = CONST_61}, // Electric constant or vaccum permittivity
/*  62 */  {.label = STD_lambda STD_SUB_C,                  .value = CONST_62}, // Compton wavelengths of electron
/*  63 */  {.label = STD_lambda STD_SUB_C STD_SUB_n,        .value = CONST_63}, // Compton wavelengths of neutron
/*  64 */  {.label = STD_lambda STD_SUB_C STD_SUB_p,        .value = CONST_64}, // Compton wavelengths of proton
/*  65 */  {.label = STD_mu STD_SUB_0,                      .value = CONST_65}, // Magnetic constant or caccum permeability
/*  66 */  {.label = STD_mu STD_SUB_B,                      .value = CONST_66}, // Bohr's magneton
/*  67 */  {.label = STD_mu STD_SUB_e,                      .value = CONST_67}, // Electron magnetic moment
/*  68 */  {.label = STD_mu STD_SUB_e "/" STD_mu STD_SUB_B, .value = CONST_68}, // Ratio of electron magnetic momment to Bohr's magnetron
/*  69 */  {.label = STD_mu STD_SUB_n,                      .value = CONST_69}, // Neutron magnetic moment
/*  70 */  {.label = STD_mu STD_SUB_p,                      .value = CONST_70}, // Proton magnetic moment
/*  71 */  {.label = STD_mu STD_SUB_u,                      .value = CONST_71}, // Nuclear magneton
/*  72 */  {.label = STD_mu STD_SUB_mu,                     .value = CONST_72}, // Muon magnetic moment
/*  73 */  {.label = STD_sigma STD_SUB_B,                   .value = CONST_73}, // Stefan-Boltzmann constant
/*  74 */  {.label = STD_PHI,                               .value = CONST_74}, // Golden ration
/*  75 */  {.label = STD_PHI STD_SUB_0,                     .value = CONST_75}, // Magnetic flux quantum
/*  76 */  {.label = STD_omega,                             .value = CONST_76}, // Angular velocity of the Earth according WGS84
/*  77 */  {.label = "-" STD_INFINITY,                      .value = CONST_77}, // -infinity
/*  78 */  {.label = STD_INFINITY,                          .value = CONST_78}, // infinity
/*  79 */  {.label = "#",                                   .value = CONST_79}  // See the very last command of the IOP
};



/********************************************//**
 * \brief Replaces the X content with the selected
 * constant. Enables \b stack \b lift and refreshes the stack
 *
 * \param[in] cst uint16_t Index of the constant to store in X
 * \return void
 ***********************************************/
void fnConstant(const uint16_t cst) {
  liftStack();
  real16Copy(constants + physicalAndMathConstants[cst].value, REGISTER_REAL16_DATA(REGISTER_X));
  refreshStack();
}



/********************************************//**
 * \brief Replaces the X content with the circumference
 * to diameter ratio pi: 3,1415926. Enables \b stack
 * \b lift and refreshes the stack.
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnPi(uint16_t unusedParamButMandatory) {
  liftStack();
  real16Copy(const16_pi, REGISTER_REAL16_DATA(REGISTER_X));
  refreshStack();
}
