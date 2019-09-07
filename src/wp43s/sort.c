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
 * \file sort.c Comparing 2 strings, sorting strings
 ***********************************************/

#include "wp43s.h"

const char sortingOrder[] =
 " "
     STD_SPACE_EM
     STD_SPACE_3_PER_EM
     STD_SPACE_4_PER_EM
     STD_SPACE_6_PER_EM
     STD_SPACE_FIGURE
     STD_SPACE_HAIR
     STD_SPACE_PUNCTUATION


 "0"
     STD_SUP_0
     STD_SUB_0

 "1"
     STD_SUP_1
     STD_SUB_1
     STD_SUB_10

 "2"
     STD_SUP_2
     STD_SUB_2

 "3"
     STD_SUP_3
     STD_SUB_3
     STD_CUBIC_ROOT

 "4"
     STD_SUP_4
     STD_SUB_4

 "5"
     STD_SUP_5
     STD_SUB_5

 "6"
     STD_SUP_6
     STD_SUB_6

 "7"
     STD_SUP_7
     STD_SUB_7

 "8"
     STD_SUP_8
     STD_SUB_8

 "9"
     STD_SUP_9
     STD_SUB_9


 "A"
     STD_SUB_A
 "a"
     STD_SUP_a
     STD_SUB_a

 "B"
     STD_SUB_B
 "b"
     STD_SUB_b

 "C"
     STD_SUB_C
 "c"
     STD_SUB_c

 "D"
     STD_SUB_D

 "d"
     STD_SUB_d

 "E"
     STD_SUB_E
 "e"
     STD_SUB_e

 "F"
     STD_SUB_F
 "f"
     STD_SUP_f

 "G"
     STD_SUB_G
 "g"
     STD_SUP_g

 "H"
     STD_SUB_H
 "h"
     STD_SUP_h
     STD_SUB_h
     STD_PLANCK
     STD_PLANCK_2PI

 "I"
     STD_SUB_I
 "i"
     STD_SUB_i

 "J"
     STD_SUB_J
 "j"
     STD_SUB_j

 "K"
     STD_SUB_K
 "k"
     STD_SUB_k

 "L"
     STD_SUB_L
 "l"
     STD_SUB_l

 "M"
     STD_SUB_M
 "m"
     STD_SUB_m

 "N"
     STD_SUB_N
 "n"
     STD_SUB_n

 "O"
     STD_SUB_O
 "o"
     STD_SUB_o


 "P"
     STD_SUB_P
 "p"
     STD_SUB_p

 "Q"
     STD_SUB_Q
 "q"
     STD_SUB_q

 "R"
     STD_SUB_R
 "r"
     STD_SUP_r

 "S"
     STD_SUB_S
 "s"
     STD_SUB_s

 "T"
     STD_SUP_T
     STD_SUB_T
 "t"
     STD_SUB_t


 "U"
     STD_SUB_U
     STD_UK
     STD_US
 "u"
     STD_SUB_u

 "V"
     STD_SUB_V
 "v"
     STD_SUB_v

 "W"
     STD_SUB_W
 "w"
     STD_SUB_w

 "X"
     STD_SUB_X
 "x"
     STD_SUP_x
     STD_SUB_x
     STD_xTH_ROOT
     STD_x_CIRC
     STD_x_BAR
     STD_x_UNDER_ROOT

 "Y"
     STD_SUB_Y
 "y"
     STD_SUB_y
     STD_y_CIRC
     STD_y_BAR
     STD_y_UNDER_ROOT

 "Z"
     STD_SUB_Z
 "z"
     STD_SUB_z



 STD_ALPHA
 STD_alpha
     STD_SUB_alpha

 STD_BETA
 STD_beta

 STD_GAMMA
 STD_gamma


 STD_DELTA
 STD_delta
     STD_SUB_delta

 STD_EPSILON
 STD_epsilon
     STD_epsilon_TONOS

  STD_DIGAMMA   //JM GREEK
  STD_digamma   //JM GREEK

 STD_ZETA
 STD_zeta

 STD_ETA
 STD_eta
     STD_eta_TONOS

 STD_THETA
 STD_theta

 STD_IOTA
     STD_IOTA_DIALYTIKA
 STD_iota
     STD_iota_TONOS
     STD_iota_DIALYTIKA_TONOS
     STD_iota_DIALYTIKA

 STD_KAPPA
 STD_kappa

 STD_LAMBDA
 STD_lambda


 STD_MU
 STD_mu
     STD_SUB_mu

 STD_NU
 STD_nu

 STD_XI
 STD_xi

 STD_OMICRON
 STD_omicron
     STD_omicron_TONOS

 STD_PI
 STD_pi

 STD_QOPPA     //JM GREEK
 STD_qoppa     //JM GREEK

 STD_RHO
 STD_rho

 STD_SIGMA
 STD_sigma_end
     STD_sigma

 STD_TAU
 STD_tau

 STD_UPSILON
     STD_UPSILON_DIALYTIKA
 STD_upsilon
     STD_upsilon_TONOS
     STD_upsilon_DIALYTIKA
     STD_upsilon_DIALYTIKA_TONOS

 STD_PHI
 STD_phi

 STD_CHI
 STD_chi

 STD_PSI
 STD_psi

 STD_OMEGA
 STD_omega
     STD_omega_TONOS

  STD_SAMPI     //JM GREEK
  STD_sampi     //JM GREEK


 "()"

 "+"
     STD_SUP_PLUS
     STD_SUB_PLUS

 "-"
     STD_SUP_MINUS
     STD_SUB_MINUS
     STD_SUP_MINUS_1

 STD_CROSS

 "/"

 STD_PLUS_MINUS

 ","
     STD_COMMA34

 "."
     STD_PERIOD34

 "!"

 "?"

 ":"

 ";"

 "'"

 "\""

 STD_DOT

 "*"
     STD_SUP_ASTERISK

 "@"

 "_"

 "~"

 STD_RIGHT_ARROW

 STD_LEFT_ARROW

 STD_UP_ARROW

 STD_DOWN_ARROW

 STD_LEFT_RIGHT_ARROWS

 "<"

 STD_LESS_EQUAL

 "="

 STD_ALMOST_EQUAL

 STD_NOT_EQUAL

 STD_GREATER_EQUAL

 ">"

 "%"

 "$"

 STD_EURO

 "£"

 STD_YEN

 STD_SQUARE_ROOT

 STD_INTEGRAL
     STD_DOUBLE_INTEGRAL
     STD_CONTOUR_INTEGRAL
     STD_SURFACE_INTEGRAL

 STD_INFINITY
     STD_SUP_INFINITY
     STD_SUB_INFINITY

 "&"

 "\\"

 "^"

 "|"

 "[]"

 "{}"

 STD_PRINTER

 "#"

 STD_SUB_EARTH
 STD_SUB_EARTH_b

 STD_SUN
     STD_SUB_SUN
     STD_SUB_SUN_b
 ;

void test(void) {
  for(int i=1; i<LAST_ITEM; i++) {
  }
}
