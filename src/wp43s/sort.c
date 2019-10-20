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

/****************************************************//**
 * \file sort.c Comparing 2 strings, sorting strings
 *******************************************************/

#include "wp43s.h"

const char *sortingOrder =
 " "
 STD_SPACE_EM                  //"\xa0\x03"
 STD_SPACE_3_PER_EM            //"\xa0\x04"
 STD_SPACE_4_PER_EM            //"\xa0\x05"
 STD_SPACE_6_PER_EM            //"\xa0\x06"
 STD_SPACE_FIGURE              //"\xa0\x07"
 STD_SPACE_PUNCTUATION         //"\xa0\x08"
 STD_SPACE_HAIR                //"\xa0\x0a"

 "0"
 STD_DEGREE                    //"\x80\xb0"
 STD_SUP_0                     //"\xa0\x70"
 STD_SUB_0                     //"\xa0\x80"
 STD_ZERO                      //"\xa2\x0e"

 "1"
 STD_SUP_1_b                   //"\x80\xb9"
 STD_ONE                       //"\xa0\x27"
 STD_SUP_1                     //"\xa0\x71"
 STD_SUB_1                     //"\xa0\x81"
 STD_BASE_1                    //"\xa4\x60"

 "2"
 STD_SUP_2                     //"\x80\xb2"
 STD_SUB_2                     //"\xa0\x82"
 STD_BASE_2                    //"\xa4\x61"

 "3"
 STD_SUP_3                     //"\x80\xb3"
 STD_SUB_3                     //"\xa0\x83"
 STD_CUBE_ROOT                 //"\xa2\x1b"
 STD_BASE_3                    //"\xa4\x62"

 "4"
 STD_SUP_4                     //"\xa0\x74"
 STD_SUB_4                     //"\xa0\x84"
 STD_BASE_4                    //"\xa4\x63"

 "5"
 STD_SUP_5                     //"\xa0\x75"
 STD_SUB_5                     //"\xa0\x85"
 STD_BASE_5                    //"\xa4\x64"

 "6"
 STD_SUP_6                     //"\xa0\x76"
 STD_SUB_6                     //"\xa0\x86"
 STD_BASE_6                    //"\xa4\x65"

 "7"
 STD_SUP_7                     //"\xa0\x77"
 STD_SUB_7                     //"\xa0\x87"
 STD_BASE_7                    //"\xa4\x66"

 "8"
 STD_SUP_8                     //"\xa0\x78"
 STD_SUB_8                     //"\xa0\x88"
 STD_BASE_8                    //"\xa4\x67"

 "9"
 STD_SUP_9                     //"\xa0\x79"
 STD_SUB_9                     //"\xa0\x89"
 STD_BASE_9                    //"\xa4\x68"

 "A"
 STD_SUB_A                     //"\xa4\xb6"
 STD_A_GRAVE                   //"\x80\xc0"
 STD_A_ACUTE                   //"\x80\xc1"
 STD_A_CIRC                    //"\x80\xc2"
 STD_A_TILDE                   //"\x80\xc3"
 STD_A_DIARESIS                //"\x80\xc4"
 STD_A_RING                    //"\x80\xc5"
 STD_A_MACRON                  //"\x81\x00"
 STD_A_BREVE                   //"\x81\x02"
 STD_A_OGONEK                  //"\x81\x04"
 STD_AE                        //"\x80\xc6"
 STD_FOR_ALL                   //"\xa2\x00"

 "a"
 STD_SUP_a                     //"\x80\xaa"
 STD_SUB_a_b                   //"\xa0\x90"
 STD_SUB_a                     //"\xa4\x9c"
 STD_a_GRAVE                   //"\x80\xe0"
 STD_a_ACUTE                   //"\x80\xe1"
 STD_a_CIRC                    //"\x80\xe2"
 STD_a_TILDE                   //"\x80\xe3"
 STD_a_DIARESIS                //"\x80\xe4"
 STD_a_RING                    //"\x80\xe5"
 STD_a_MACRON                  //"\x81\x01"
 STD_a_BREVE                   //"\x81\x03"
 STD_a_OGONEK                  //"\x81\x05"
 STD_ae                        //"\x80\xe6"

 "B"
 STD_SUB_B                     //"\xa4\xb7"

 "b"
 STD_SUB_b                     //"\xa4\x9d"

 "C"
 STD_SUB_C                     //"\xa4\xb8"
 STD_C_CEDILLA                 //"\x80\xc7"
 STD_C_ACUTE                   //"\x81\x06"
 STD_C_CARON                   //"\x81\x0c"
 STD_COMPLEX_C                 //"\xa1\x02"
 STD_COMPLEMENT                //"\xa2\x01"
 STD_CONTAINS                  //"\xa2\x0b"
 STD_DOES_NOT_CONTAIN          //"\xa2\x0c"

 "c"
 STD_SUB_c                     //"\xa4\x9e"
 STD_c_CEDILLA                 //"\x80\xe7"
 STD_c_ACUTE                   //"\x81\x07"
 STD_c_CARON                   //"\x81\x0d"

 "D"
 STD_SUB_D                     //"\xa4\xb9"
 STD_D_CARON                   //"\x81\x0e"
 STD_D_STROKE                  //"\x81\x10"
 STD_ETH                       //"\x80\xd0"
 STD_PARTIAL_DIFF              //"\xa2\x02"

 "d"
 STD_SUB_d                     //"\xa4\x9f"
 STD_d_APOSTROPHE              //"\x81\x0f"
 STD_d_STROKE                  //"\x81\x11"
 STD_eth                       //"\x80\xf0"
 STD_PARTIAL_DIFF              //"\xa2\x02"

 "E"
 STD_SUB_E                     //"\xa4\xba"
 STD_E_GRAVE                   //"\x80\xc8"
 STD_E_ACUTE                   //"\x80\xc9"
 STD_E_CIRC                    //"\x80\xca"
 STD_E_DIARESIS                //"\x80\xcb"
 STD_E_MACRON                  //"\x81\x12"
 STD_E_BREVE                   //"\x81\x14"
 STD_E_DOT                     //"\x81\x16"
 STD_E_OGONEK                  //"\x81\x18"
 STD_E_CARON                   //"\x81\x1a"
 STD_SUB_E_OUTLINE             //"\xa0\x73"
 STD_THERE_EXISTS              //"\xa2\x03"
 STD_THERE_DOES_NOT_EXIST      //"\xa2\x04"
 STD_ELEMENT_OF                //"\xa2\x08"
 STD_NOT_ELEMENT_OF            //"\xa2\x09"

 "e"
 STD_SUB_e_b                   //"\xa0\x91"
 STD_SUB_e                     //"\xa4\xa0"
 STD_e_GRAVE                   //"\x80\xe8"
 STD_e_ACUTE                   //"\x80\xe9"
 STD_e_CIRC                    //"\x80\xea"
 STD_e_DIARESIS                //"\x80\xeb"
 STD_e_MACRON                  //"\x81\x13"
 STD_e_BREVE                   //"\x81\x15"
 STD_e_DOT                     //"\x81\x17"
 STD_e_OGONEK                  //"\x81\x19"
 STD_e_CARON                   //"\x81\x1b"

 "F"
 STD_SUB_F                     //"\xa4\xbb"

 "f"
 STD_SUP_f                     //"\xa4\xa1"

 "G"
 STD_SUB_G                     //"\xa4\xbc"
 STD_G_BREVE                   //"\x81\x1e"

 "g"
 STD_SUP_g                     //"\xa4\xa2"
 STD_g_BREVE                   //"\x81\x1f"

 "H"
 STD_SUB_H                     //"\xa4\xbd"

 "h"
 STD_SUB_h                     //"\xa0\x95"
 STD_SUP_h                     //"\xa4\xa3"
 STD_h_STROKE                  //"\x81\x27"
 STD_PLANCK                    //"\xa1\x0e"
 STD_PLANCK_2PI                //"\xa1\x0f"

 "I"
 STD_SUB_I                     //"\xa4\xbe"
 STD_I_GRAVE                   //"\x80\xcc"
 STD_I_ACUTE                   //"\x80\xcd"
 STD_I_CIRC                    //"\x80\xce"
 STD_I_DIARESIS                //"\x80\xcf"
 STD_I_MACRON                  //"\x81\x2a"
 STD_I_BREVE                   //"\x81\x2c"
 STD_I_OGONEK                  //"\x81\x2e"
 STD_I_DOT                     //"\x81\x30"

 "i"
 STD_SUB_i                     //"\xa4\xa4"
 STD_i_GRAVE                   //"\x80\xec"
 STD_i_ACUTE                   //"\x80\xed"
 STD_i_CIRC                    //"\x80\xee"
 STD_i_DIARESIS                //"\x80\xef"
 STD_i_MACRON                  //"\x81\x2b"
 STD_i_BREVE                   //"\x81\x2d"
 STD_i_OGONEK                  //"\x81\x2f"
 STD_i_DOTLESS                 //"\x81\x31"

 "J"
 STD_SUB_J                     //"\xa4\xbf"

 "j"
 STD_SUB_j                     //"\xa4\xa5"

 "K"
 STD_SUB_K                     //"\xa4\xc0"

 "k"
 STD_SUB_k_b                   //"\xa0\x96"
 STD_SUB_k                     //"\xa4\xa6"

 "L"
 STD_SUB_L                     //"\xa4\xc1"
 STD_L_ACUTE                   //"\x81\x39"
 STD_L_APOSTROPHE              //"\x81\x3d"
 STD_L_STROKE                  //"\x81\x41"

 "l"
 STD_SUB_l_b                   //"\xa0\x97"
 STD_SUB_l                     //"\xa4\xa7"
 STD_l_ACUTE                   //"\x81\x3a"
 STD_l_APOSTROPHE              //"\x81\x3e"
 STD_l_STROKE                  //"\x81\x42"

 "M"
 STD_SUB_M                     //"\xa4\xc2"

 "m"
 STD_SUB_m_b                   //"\xa0\x98"
 STD_SUB_m                     //"\xa4\xa8"

 "N"
 STD_SUB_N                     //"\xa4\xc3"
 STD_N_TILDE                   //"\x80\xd1"
 STD_N_ACUTE                   //"\x81\x43"
 STD_N_CARON                   //"\x81\x47"
 STD_NATURAL_N                 //"\xa1\x15"

 "n"
 STD_SUB_n_b                   //"\xa0\x99"
 STD_SUB_n                     //"\xa4\xa9"
 STD_n_TILDE                   //"\x80\xf1"
 STD_n_ACUTE                   //"\x81\x44"
 STD_n_CARON                   //"\x81\x48"

 "O"
 STD_SUB_O                     //"\xa4\xc4"
 STD_O_GRAVE                   //"\x80\xd2"
 STD_O_ACUTE                   //"\x80\xd3"
 STD_O_CIRC                    //"\x80\xd4"
 STD_O_TILDE                   //"\x80\xd5"
 STD_O_DIARESIS                //"\x80\xd6"
 STD_O_STROKE                  //"\x80\xd8"
 STD_O_MACRON                  //"\x81\x4c"
 STD_O_BREVE                   //"\x81\x4e"
 STD_OE                        //"\x81\x52"

 "o"
 STD_SUB_o_b                   //"\xa0\x92"
 STD_SUB_o                     //"\xa4\xaa"
 STD_o_GRAVE                   //"\x80\xf2"
 STD_o_ACUTE                   //"\x80\xf3"
 STD_o_CIRC                    //"\x80\xf4"
 STD_o_TILDE                   //"\x80\xf5"
 STD_o_DIARESIS                //"\x80\xf6"
 STD_o_STROKE                  //"\x80\xf8"
 STD_o_MACRON                  //"\x81\x4d"
 STD_o_BREVE                   //"\x81\x4f"
 STD_oe                        //"\x81\x53"

 "P"
 STD_SUB_P                     //"\xa4\xc5"

 "p"
 STD_SUB_p_b                   //"\xa0\x9a"
 STD_SUB_p                     //"\xa4\xab"

 "Q"
 STD_SUB_Q                     //"\xa4\xc6"
 STD_RATIONAL_Q                //"\xa1\x1a"

 "q"
 STD_SUB_q                     //"\xa4\xac"

 "R"
 STD_SUB_R                     //"\xa4\xc7"
 STD_R_ACUTE                   //"\x81\x54"
 STD_R_CARON                   //"\x81\x58"
 STD_REAL_R                    //"\xa1\x1d"

 "r"
 STD_SUP_r                     //"\xa4\xad"
 STD_r_ACUTE                   //"\x81\x55"
 STD_r_CARON                   //"\x81\x59"

 "S"
 STD_SUB_S                     //"\xa4\xc8"
 STD_S_ACUTE                   //"\x81\x5a"
 STD_S_CEDILLA                 //"\x81\x5e"
 STD_S_CARON                   //"\x81\x60"

 "s"
 STD_SUB_s_b                   //"\xa0\x9b"
 STD_SUB_s                     //"\xa4\xae"
 STD_s_ACUTE                   //"\x81\x5b"
 STD_s_CEDILLA                 //"\x81\x5f"
 STD_s_CARON                   //"\x81\x61"
 STD_s_SHARP                   //"\x80\xdf"

 "T"
 STD_SUP_T                     //"\xa4\xaf"
 STD_SUB_T                     //"\xa4\xc9"
 STD_T_CEDILLA                 //"\x81\x62"
 STD_T_CARON                   //"\x81\x64"
 STD_DOWN_TACK                 //"\xa2\xa4"
 STD_PERPENDICULAR             //"\xa2\xa5"

 "t"
 STD_SUB_t                     //"\xa0\x9c"
 STD_t_CEDILLA                 //"\x81\x63"
 STD_t_APOSTROPHE              //"\x81\x65"

 "U"
 STD_SUB_U                     //"\xa4\xca"
 STD_U_GRAVE                   //"\x80\xd9"
 STD_U_ACUTE                   //"\x80\xda"
 STD_U_CIRC                    //"\x80\xdb"
 STD_U_DIARESIS                //"\x80\xdc"
 STD_U_TILDE                   //"\x81\x68"
 STD_U_MACRON                  //"\x81\x6a"
 STD_U_BREVE                   //"\x81\x6c"
 STD_U_RING                    //"\x81\x6e"
 STD_U_OGONEK                  //"\x81\x72"
 STD_UNION                     //"\xa2\x2a"
 STD_USER_MODE                 //"\xa4\x2c"
 STD_UK                        //"\xa4\x2d"
 STD_US                        //"\xa4\x2e"

 "u"
 STD_SUB_u_b                   //"\x9d\x64"
 STD_SUB_u                     //"\xa4\xb0"
 STD_u_GRAVE                   //"\x80\xf9"
 STD_u_ACUTE                   //"\x80\xfa"
 STD_u_CIRC                    //"\x80\xfb"
 STD_u_DIARESIS                //"\x80\xfc"
 STD_u_TILDE                   //"\x81\x69"
 STD_u_MACRON                  //"\x81\x6b"
 STD_u_BREVE                   //"\x81\x6d"
 STD_u_RING                    //"\x81\x6f"
 STD_u_OGONEK                  //"\x81\x73"

 "V"
 STD_SUB_V                     //"\xa4\xcb"

 "v"
 STD_SUB_v                     //"\xa4\xb1"

 "W"
 STD_SUB_W                     //"\xa4\xcc"
 STD_W_CIRC                    //"\x81\x74"

 "w"
 STD_SUB_w                     //"\xa4\xb2"
 STD_w_CIRC                    //"\x81\x75"

 "X"
 STD_SUB_X                     //"\xa4\xcd"

 "x"
 STD_SUP_x                     //"\x9d\x61"
 STD_SUB_x_b                   //"\xa0\x93"
 STD_SUB_x                     //"\xa4\xb3"
 STD_x_BAR                     //"\x83\x78"
 STD_x_CIRC                    //"\x83\x79"
 STD_x_UNDER_ROOT              //"\x83\x7f"
 STD_xTH_ROOT                  //"\xa2\x1c"

 "Y"
 STD_SUB_Y                     //"\xa4\xce"
 STD_Y_ACUTE                   //"\x80\xdd"
 STD_Y_CIRC                    //"\x81\x76"
 STD_Y_DIARESIS                //"\x81\x78"

 "y"
 STD_SUB_y                     //"\xa4\xb4"
 STD_y_ACUTE                   //"\x80\xfd"
 STD_y_DIARESIS                //"\x80\xff"
 STD_y_CIRC                    //"\x81\x77"
 STD_y_UNDER_ROOT              //"\x82\x32"
 STD_y_BAR                     //"\x82\x33"

 "Z"
 STD_SUB_Z                     //"\xa4\xcf"
 STD_Z_ACUTE                   //"\x81\x79"
 STD_Z_DOT                     //"\x81\x7b"
 STD_Z_CARON                   //"\x81\x7d"
 STD_INTEGER_Z                 //"\xa1\x24"

 "z"
 STD_SUB_z                     //"\xa4\xb5"
 STD_z_ACUTE                   //"\x81\x7a"
 STD_z_DOT                     //"\x81\x7c"
 STD_z_CARON                   //"\x81\x7e"

 STD_ALPHA

 STD_alpha
 STD_SUB_alpha                 //"\xa0\x65"
 STD_alpha_TONOS               //"\x83\xac"

 STD_BETA

 STD_beta

 STD_GAMMA

 STD_gamma

 STD_DELTA

 STD_delta
 STD_SUB_delta                 //"\xa0\x66"

 STD_EPSILON

 STD_epsilon
 STD_epsilon_TONOS             //"\x83\xad"

  STD_DIGAMMA   //JM GREEK
  STD_digamma   //JM GREEK

 STD_ZETA

 STD_zeta

 STD_ETA

 STD_eta
 STD_eta_TONOS                 //"\x83\xae"

 STD_THETA

 STD_theta

 STD_IOTA
 STD_IOTA_DIALYTIKA            //"\x83\xaa"

 STD_iota
 STD_iota_DIALYTIKA_TONOS      //"\x83\x90"
 STD_iota_TONOS                //"\x83\xaf"
 STD_iota_DIALYTIKA            //"\x83\xca"

 STD_KAPPA

 STD_kappa

 STD_LAMBDA

 STD_lambda

 STD_MU

 STD_mu
 STD_mu_b                      //"\x80\xb5"
 STD_SUB_mu                    //"\xa0\x67"

 STD_NU

 STD_nu

 STD_XI

 STD_xi

 STD_OMICRON

 STD_omicron
 STD_omicron_TONOS             //"\x83\xcc"

 STD_PI
 STD_PRODUCT                   //"\xa2\x0f"

 STD_pi

 STD_QOPPA     //JM GREEK
 STD_qoppa     //JM GREEK

 STD_RHO

 STD_rho

 STD_SIGMA
 STD_SUM                       //"\xa2\x11"

 STD_sigma_end

 STD_sigma

 STD_TAU

 STD_tau

 STD_UPSILON
 STD_UPSILON_DIALYTIKA         //"\x83\xab"

 STD_upsilon
 STD_upsilon_DIALYTIKA_TONOS   //"\x83\xb0"
 STD_upsilon_DIALYTIKA         //"\x83\xcb"
 STD_upsilon_TONOS             //"\x83\xcd"

 STD_PHI

 STD_phi

 STD_CHI

 STD_chi

 STD_PSI

 STD_psi

 STD_OMEGA

 STD_omega
 STD_omega_TONOS               //"\x83\xce"

 STD_SAMPI     //JM GREEK
 STD_sampi     //JM GREEK

 "("

 ")"

 "+"
 STD_SUP_PLUS                  //"\xa0\x7a"
 STD_SUB_PLUS                  //"\xa0\x8a"

 "-"
 STD_SUP_MINUS                 //"\xa0\x7b"
 STD_SUB_MINUS                 //"\xa0\x8b"
 STD_MINUS_SIGN                //"\xa2\x12"

 STD_CROSS

 "/"
 STD_DIVISION                  //"\xa2\x15"
 STD_DIVIDE                    //"\x80\xf7"

 STD_PLUS_MINUS
 STD_MINUS_PLUS                //"\xa2\x13"

 ","
 STD_COMMA34                   //"\xa4\x29"

 "."
 STD_PERIOD34                  //"\xa4\x28"

 "!"
 STD_INVERTED_EXCLAMATION_MARK //"\x80\xa1"
 STD_NEG_EXCLAMATION_MARK      //"\xa4\x2f"

 "?"
 STD_INVERTED_QUESTION_MARK    //"\x80\xbf"

 ":"

 ";"

 "'"
 STD_LEFT_SINGLE_QUOTE         //"\xa0\x18"
 STD_RIGHT_SINGLE_QUOTE        //"\xa0\x19"
 STD_SINGLE_HIGH_QUOTE         //"\xa0\x1b"
 STD_SINGLE_LOW_QUOTE          //"\xa0\x1a"

 "\""
 STD_LEFT_DOUBLE_QUOTE         //"\xa0\x1c"
 STD_RIGHT_DOUBLE_QUOTE        //"\xa0\x1d"
 STD_DOUBLE_HIGH_QUOTE         //"\xa0\x1f"
 STD_DOUBLE_LOW_QUOTE          //"\xa0\x1e"

 STD_DOT
 STD_RING                      //"\xa2\x18"
 STD_BULLET                    //"\xa2\x19"

 "*"
 STD_SUP_ASTERISK              //"\xa0\x8f"
 STD_ASTERISK_b                //"\xa2\x17"

 "@"

 "_"

 "~"
 STD_TILDE_b                   //"\xa2\x3c"

 STD_RIGHT_ARROW
 STD_RIGHT_SHORT_ARROW         //"\xa1\xc0"

 STD_LEFT_ARROW

 STD_UP_ARROW
 STD_UP_ARROW_b                //"\xa1\xc8"
 STD_BST                       //"\xa1\xc9"
 STD_HOLLOW_UP_ARROW           //"\xa1\xe7"
 STD_INCREMENT                 //"\xa2\x06"

 STD_DOWN_ARROW
 STD_DOWN_ARROW_b              //"\xa1\xca"
 STD_SST                       //"\xa1\xcb"
 STD_HOLLOW_DOWN_ARROW         //"\xa1\xe9"
 STD_NABLA                     //"\xa2\x07"

 STD_LEFT_RIGHT_ARROWS
 STD_SERIAL_IO                 //"\xa1\x95"

 "<"
 STD_MUCH_LESS                 //"\xa2\x6a"
 STD_LEFT_DOUBLE_ANGLE         //"\x80\xab"

 STD_LESS_EQUAL

 "="

 STD_ALMOST_EQUAL
 STD_ASYMPOTICALLY_EQUAL       //"\xa2\x43"
 STD_COLON_EQUALS              //"\xa2\x54"
 STD_CORRESPONDS_TO            //"\xa2\x58"
 STD_ESTIMATES                 //"\xa2\x59"
 STD_IDENTICAL_TO              //"\xa2\x61"

 STD_NOT_EQUAL

 STD_GREATER_EQUAL

 ">"
 STD_MUCH_GREATER              //"\xa2\x6b"
 STD_RIGHT_DOUBLE_ANGLE        //"\x80\xbb"

 "%"

 "$"

 STD_EURO
 STD_CENT                      //"\x80\xa2"

 STD_POUND

 STD_YEN

 STD_SQUARE_ROOT

 STD_INTEGRAL
 STD_DOUBLE_INTEGRAL           //"\xa2\x2c"
 STD_CONTOUR_INTEGRAL          //"\xa2\x2e"
 STD_SURFACE_INTEGRAL          //"\xa2\x2f"

 STD_INFINITY
 STD_SUP_INFINITY              //"\xa0\x9e"
 STD_SUB_INFINITY              //"\xa0\x9f"

 "&"

 "\\"
 STD_SET_MINUS                 //"\xa2\x16"

 "^"

 "|"

 "["

 "]"

 "{"

 "}"

 STD_MEASURED_ANGLE
 STD_ANGLE                     //"\xa2\x20"

 STD_PRINTER

 "#"


 STD_SECTION                   //"\x80\xa7"
 STD_OVERFLOW_CARRY            //"\x80\xa9"
 STD_ORDINAL                   //"\x80\xba"
 STD_ONE_HALF                  //"\x80\xbc"
 STD_ONE_QUARTER               //"\x80\xbd"
 STD_ELLIPSIS                  //"\xa0\x26"
 STD_SUN                       //"\xa2\x99"
 STD_SUB_SUN                   //"\xa0\x68"
 STD_SUB_SUN_b                 //"\xa2\x9a"
 STD_SUB_EARTH                 //"\xa0\x69"
 STD_SUB_EARTH_b               //"\xa2\x95"
 STD_SUP_MINUS_1               //"\xa0\x72"
 STD_HAMBURGER                 //"\xa1\xcc"
 STD_UNDO                      //"\xa1\xcd"
 STD_EMPTY_SET                 //"\xa2\x05"
 STD_PROPORTIONAL              //"\xa2\x1d"
 STD_RIGHT_ANGLE               //"\xa2\x1f"
 STD_DIVIDES                   //"\xa2\x23"
 STD_DOES_NOT_DIVIDE           //"\xa2\x24"
 STD_PARALLEL                  //"\xa2\x25"
 STD_NOT_PARALLEL              //"\xa2\x26"
 STD_NOT                       //"\x80\xac"
 STD_AND                       //"\xa2\x27"
 STD_OR                        //"\xa2\x28"
 STD_XOR                       //"\xa2\xbb"
 STD_NAND                      //"\xa2\xbc"
 STD_NOR                       //"\xa2\xbd"
 STD_INTERSECTION              //"\xa2\x29"
 STD_RATIO                     //"\xa2\x36"
 STD_WATCH                     //"\xa3\x1a"
 STD_HOURGLASS                 //"\xa3\x1b"
 STD_MAT_TL                    //"\xa3\xa1"
 STD_MAT_ML                    //"\xa3\xa2"
 STD_MAT_BL                    //"\xa3\xa3"
 STD_MAT_TR                    //"\xa3\xa4"
 STD_MAT_MR                    //"\xa3\xa5"
 STD_MAT_BR                    //"\xa3\xa6"
 STD_OBLIQUE1                  //"\xa4\x22"
 STD_OBLIQUE2                  //"\xa4\x23"
 STD_OBLIQUE3                  //"\xa4\x25"
 STD_OBLIQUE4                  //"\xa4\x26"
 STD_CURSOR                    //"\xa4\x27"
 STD_BATTERY                   //"\xa4\x2a"
 STD_PGM_BEGIN                 //"\xa4\x2b"
 STD_BASE_10                   //"\xa4\x69"
 STD_BASE_11                   //"\xa4\x6a"
 STD_BASE_12                   //"\xa4\x6b"
 STD_BASE_13                   //"\xa4\x6c"
 STD_BASE_14                   //"\xa4\x6d"
 STD_BASE_15                   //"\xa4\x6e"
 STD_BASE_16                   //"\xa4\x6f"
 STD_SUB_10                    //"\xa4\x91"
;



 static int16_t rankOfGlyph(const char *glyph) {
   int16_t nbrBytes, pos;
   int32_t i;

   if(*glyph & 0x80) {
     nbrBytes = 2;
   }
   else {
     nbrBytes = 1;
   }

   pos = 0;
   for(i=0; i<SORT_STRING_LEN_GLYPHS; i++) {
     if(nbrBytes == 1 && *glyph == *(sortingOrder + pos)) return pos;
     if(nbrBytes == 2 && *glyph == *(sortingOrder + pos) && *(glyph + 1) == *(sortingOrder + pos + 1)) return pos;
     pos = stringNextGlyph(sortingOrder, pos);
   }

   if(nbrBytes == 1) {
     printf("Glyph \\x%02x is missing in the sortingOrder string (defined in sort.c)\n", *(unsigned char *)glyph);
   }
   else {
     printf("Glyph \\x%02x\\x%02x is missing in the sortingOrder string (defined in sort.c)\n", *(unsigned char *)glyph, *(unsigned char *)(glyph + 1));
   }
   printf("Search for that string in file fonts.h\n");

   exit(-1);
 }



static void stringCleanup(uint8_t *str, int32_t *lg) {
  int16_t pos;
  int32_t i;

  pos = 0;
  for(i=0; i<*lg; i++) {
    /*if(*(str + pos) == 0xa0 && *(str + pos + 1) >= 0x03 && *(str + pos + 1) <= 0x0a) { // All spaces
      *(str + pos) = ' ';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa0 && *(str + pos + 1) == 0x70) // STD_SUP_0
            || (*(str + pos) == 0xa0 && *(str + pos + 1) == 0x80) // STD_SUB_0
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xb0) // STD_DEGREE
            || (*(str + pos) == 0xa2 && *(str + pos + 1) == 0x0e) // STD_ZERO
           ) {
      *(str + pos) = '0';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa0 && *(str + pos + 1) == 0x71) // STD_SUP_1
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xb9) // STD_SUP_1_b
            || (*(str + pos) == 0xa0 && *(str + pos + 1) == 0x81) // STD_SUB_1
            || (*(str + pos) == 0xa0 && *(str + pos + 1) == 0x27) // STD_ONE
           ) {
      *(str + pos) = '1';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0x80 && *(str + pos + 1) == 0xb2) // STD_SUP_2
            || (*(str + pos) == 0xa0 && *(str + pos + 1) == 0x82) // STD_SUB_2
           ) {
      *(str + pos) = '2';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0x80 && *(str + pos + 1) == 0xb3) // STD_SUP_3
            || (*(str + pos) == 0xa0 && *(str + pos + 1) == 0x83) // STD_SUB_3
           ) {
      *(str + pos) = '3';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa0 && *(str + pos + 1) == 0x74) // STD_SUP_4
            || (*(str + pos) == 0xa0 && *(str + pos + 1) == 0x84) // STD_SUB_4
           ) {
      *(str + pos) = '4';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa0 && *(str + pos + 1) == 0x75) // STD_SUP_5
            || (*(str + pos) == 0xa0 && *(str + pos + 1) == 0x85) // STD_SUB_5
           ) {
      *(str + pos) = '5';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa0 && *(str + pos + 1) == 0x76) // STD_SUP_6
            || (*(str + pos) == 0xa0 && *(str + pos + 1) == 0x86) // STD_SUB_6
           ) {
      *(str + pos) = '6';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa0 && *(str + pos + 1) == 0x77) // STD_SUP_7
            || (*(str + pos) == 0xa0 && *(str + pos + 1) == 0x87) // STD_SUB_7
           ) {
      *(str + pos) = '7';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa0 && *(str + pos + 1) == 0x78) // STD_SUP_8
            || (*(str + pos) == 0xa0 && *(str + pos + 1) == 0x88) // STD_SUB_8
           ) {
      *(str + pos) = '8';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa0 && *(str + pos + 1) == 0x79) // STD_SUP_9
            || (*(str + pos) == 0xa0 && *(str + pos + 1) == 0x89) // STD_SUB_9
           ) {
      *(str + pos) = '9';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0x91) // STD_SUB_10
           ) {
      *(str + pos) = '1';
      *(str + pos + 1) = '0';
      (*lg)++;
    }
    else if(   (*(str + pos) == 0xa0 && *(str + pos + 1) == 0x8a) // STD_SUB_PLUS
            || (*(str + pos) == 0xa0 && *(str + pos + 1) == 0x7a) // STD_SUP_PLUS
           ) {
      *(str + pos) = '+';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa0 && *(str + pos + 1) == 0x8b) // STD_SUB_MINUS
            || (*(str + pos) == 0xa0 && *(str + pos + 1) == 0x7b) // STD_SUP_MINUS
            || (*(str + pos) == 0xa2 && *(str + pos + 1) == 0x12) // STD_MINUS_SIGN
           ) {
      *(str + pos) = '-';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa0 && *(str + pos + 1) == 0x8f) // STD_SUP_ASTERISK
            || (*(str + pos) == 0xa2 && *(str + pos + 1) == 0x17) // STD_ASTERISK_b
           ) {
      *(str + pos) = '*';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa2 && *(str + pos + 1) == 0x18) // STD_RING
            || (*(str + pos) == 0xa2 && *(str + pos + 1) == 0x19) // STD_BULLET
           ) {
      *(str + pos) = '.';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa0 && *(str + pos + 1) == 0x72) // STD_SUP_MINUS_1
           ) {
      *(str + pos) = '-';
      *(str + pos + 1) = '1';
      (*lg)++;
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0x2d) // STD_UK
           ) {
      *(str + pos) = 'U';
      *(str + pos + 1) = 'K';
      (*lg)++;
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0x2e) // STD_US
           ) {
      *(str + pos) = 'U';
      *(str + pos + 1) = 'S';
      (*lg)++;
    }
    else if(   (*(str + pos) == 0xa2 && *(str + pos + 1) == 0x1b) // STD_CUBE_ROOT
           ) {
      memmove(str + pos + 1, str + pos, stringByteLength((char *)str + pos) + 1);
      *(str + pos) = '3';
      *(str + pos + 1) = 0xa2;
      *(str + pos + 2) = 0x1a;
      (*lg)++;
    }
    else if(   (*(str + pos) == 0xa2 && *(str + pos + 1) == 0x1c) // STD_xTH_ROOT
           ) {
      memmove(str + pos + 1, str + pos, stringByteLength((char *)str + pos) + 1);
      *(str + pos) = 'x';
      *(str + pos + 1) = 0xa2;
      *(str + pos + 2) = 0x1a;
      (*lg)++;
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0xb6) // STD_SUB_A
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xc0) // STD_A_GRAVE
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xc1) // STD_A_ACUTE
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xc2) // STD_A_CIRC
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xc3) // STD_A_TILDE
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xc4) // STD_A_DIARESIS
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xc5) // STD_A_RING
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x00) // STD_A_MACRON
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x02) // STD_A_BREVE
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x04) // STD_A_OGONEK
           ) {
      *(str + pos) = 'A';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0x80 && *(str + pos + 1) == 0xaa) // STD_SUP_a
            || (*(str + pos) == 0xa0 && *(str + pos + 1) == 0x90) // STD_SUB_a_b
            || (*(str + pos) == 0xa4 && *(str + pos + 1) == 0x9c) // STD_SUB_a
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xe0) // STD_a_GRAVE
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xe1) // STD_a_ACUTE
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xe2) // STD_a_CIRC
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xe3) // STD_a_TILDE
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xe4) // STD_a_DIARESIS
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xe5) // STD_a_RING
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x01) // STD_a_MACRON
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x03) // STD_a_BREVE
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x05) // STD_a_OGONEK
           ) {
      *(str + pos) = 'a';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0xb7) // STD_SUB_B
           ) {
      *(str + pos) = 'B';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0x9d) // STD_SUB_b
           ) {
      *(str + pos) = 'b';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0xb8) // STD_SUB_C
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xc7) // STD_C_CEDILLA
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x06) // STD_C_ACUTE
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x0c) // STD_C_CARON
           ) {
      *(str + pos) = 'C';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0x9e) // STD_SUB_c
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xe7) // STD_c_CEDILLA
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x07) // STD_c_ACUTE
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x0d) // STD_c_CARON
           ) {
      *(str + pos) = 'c';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0xb6) // STD_SUB_D
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x0e) // STD_D_CARON
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x10) // STD_D_STROKE
           ) {
      *(str + pos) = 'D';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0x9f) // STD_SUB_d
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x11) // STD_d_STROKE
           ) {
      *(str + pos) = 'd';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0xba) // STD_SUB_E
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xc8) // STD_E_GRAVE
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xc9) // STD_E_ACUTE
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xca) // STD_E_CIRC
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xcb) // STD_E_DIARESIS
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x12) // STD_E_MACRON
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x14) // STD_E_BREVE
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x16) // STD_E_DOT
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x18) // STD_E_OGONEK
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x1a) // STD_E_CARON
            || (*(str + pos) == 0xa0 && *(str + pos + 1) == 0x73) // STD_SUB_E_OUTLINE
           ) {
      *(str + pos) = 'E';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0xa0) // STD_SUB_e
            || (*(str + pos) == 0xa0 && *(str + pos + 1) == 0x91) // STD_SUB_e_b
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xe8) // STD_e_GRAVE
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xe9) // STD_e_ACUTE
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xea) // STD_e_CIRC
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xeb) // STD_e_DIARESIS
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x13) // STD_e_MACRON
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x15) // STD_e_BREVE
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x17) // STD_e_DOT
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x19) // STD_e_OGONEK
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x1b) // STD_e_CARON
           ) {
      *(str + pos) = 'e';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0xbb) // STD_SUB_F
           ) {
      *(str + pos) = 'F';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0xa1) // STD_SUB_f
           ) {
      *(str + pos) = 'f';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0xbc) // STD_SUB_G
           ) {
      *(str + pos) = 'G';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0xa2) // STD_SUB_g
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x1f) // STD_g_BREVE
           ) {
      *(str + pos) = 'g';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0xbd) // STD_SUB_H
           ) {
      *(str + pos) = 'H';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa1 && *(str + pos + 1) == 0x0e) // STD_PLANCK
            || (*(str + pos) == 0xa1 && *(str + pos + 1) == 0x0f) // STD_PLANCK_2PI
            || (*(str + pos) == 0xa0 && *(str + pos + 1) == 0x95) // STD_SUB_h
            || (*(str + pos) == 0xa4 && *(str + pos + 1) == 0xa3) // STD_SUP_h
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x27) // STD_h_STROKE
           ) {
      *(str + pos) = 'h';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0xbe) // STD_SUB_I
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xcc) // STD_I_GRAVE
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xcd) // STD_I_ACUTE
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xce) // STD_I_CIRC
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xcf) // STD_I_DIARESIS
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x2a) // STD_I_MACRON
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x2c) // STD_I_BREVE
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x2e) // STD_I_OGONEK
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x30) // STD_I_DOT
           ) {
      *(str + pos) = 'I';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0xa4) // STD_SUB_i
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xec) // STD_i_GRAVE
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xed) // STD_i_ACUTE
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xee) // STD_i_CIRC
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xef) // STD_i_DIARESIS
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x2b) // STD_i_MACRON
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x2d) // STD_i_BREVE
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x2f) // STD_i_OGONEK
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x31) // STD_i_DOTLESS
           ) {
      *(str + pos) = 'i';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0xbf) // STD_SUB_J
           ) {
      *(str + pos) = 'J';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0xa5) // STD_SUB_j
           ) {
      *(str + pos) = 'j';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0xc0) // STD_SUB_K
           ) {
      *(str + pos) = 'K';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0xa6) // STD_SUB_k
            || (*(str + pos) == 0xa0 && *(str + pos + 1) == 0x96) // STD_SUB_k_b
           ) {
      *(str + pos) = 'k';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0xc1) // STD_SUB_L
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x39) // STD_L_ACUTE
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x41) // STD_L_STROKE
           ) {
      *(str + pos) = 'L';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0xa7) // STD_SUB_l
            || (*(str + pos) == 0xa0 && *(str + pos + 1) == 0x97) // STD_SUB_l_b
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x3a) // STD_l_ACUTE
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x42) // STD_l_STROKE
           ) {
      *(str + pos) = 'l';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0xc2) // STD_SUB_M
           ) {
      *(str + pos) = 'M';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0xa8) // STD_SUB_m
            || (*(str + pos) == 0xa0 && *(str + pos + 1) == 0x98) // STD_SUB_m_b
           ) {
      *(str + pos) = 'm';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0xc3) // STD_SUB_N
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xd1) // STD_N_TILDE
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x43) // STD_N_ACUTE
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x47) // STD_N_CARON
           ) {
      *(str + pos) = 'N';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0xa9) // STD_SUB_n
            || (*(str + pos) == 0xa0 && *(str + pos + 1) == 0x99) // STD_SUB_n_b
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xf1) // STD_n_TILDE
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x44) // STD_n_ACUTE
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x48) // STD_n_CARON
           ) {
      *(str + pos) = 'n';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0xc4) // STD_SUB_O
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xd2) // STD_O_GRAVE
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xd3) // STD_O_ACUTE
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xd4) // STD_O_CIRC
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xd5) // STD_O_TILDE
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xd6) // STD_O_DIARESIS
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xd8) // STD_O_STROKE
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x4c) // STD_O_MACRON
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x4e) // STD_O_BREVE
           ) {
      *(str + pos) = 'O';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa0 && *(str + pos + 1) == 0x92) // STD_SUB_o_b
            || (*(str + pos) == 0xa4 && *(str + pos + 1) == 0xaa) // STD_SUB_o
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xf2) // STD_o_GRAVE
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xf3) // STD_o_ACUTE
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xf4) // STD_o_CIRC
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xf5) // STD_o_TILDE
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xf6) // STD_o_DIARESIS
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xf8) // STD_o_STROKE
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x4d) // STD_o_MACRON
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x4f) // STD_o_BREVE
           ) {
      *(str + pos) = 'o';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0xc5) // STD_SUB_P
           ) {
      *(str + pos) = 'P';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0xab) // STD_SUB_p
            || (*(str + pos) == 0xa0 && *(str + pos + 1) == 0x9a) // STD_SUB_p_b
           ) {
      *(str + pos) = 'p';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0xc6) // STD_SUB_Q
           ) {
      *(str + pos) = 'Q';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0xac) // STD_SUB_q
           ) {
      *(str + pos) = 'q';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0xc7) // STD_SUB_R
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x54) // STD_R_ACUTE
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x58) // STD_R_CARON
           ) {
      *(str + pos) = 'R';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0xad) // STD_SUP_r
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x55) // STD_r_ACUTE
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x59) // STD_r_CARON
           ) {
      *(str + pos) = 'r';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0xc8) // STD_SUB_S
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x5a) // STD_S_ACUTE
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x5e) // STD_S_CEDILLA
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x60) // STD_S_CARON
           ) {
      *(str + pos) = 'S';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0xae) // STD_SUB_s
            || (*(str + pos) == 0xa0 && *(str + pos + 1) == 0x9b) // STD_SUB_s_b
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x5b) // STD_s_ACUTE
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x5f) // STD_s_CEDILLA
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x61) // STD_s_CARON
           ) {
      *(str + pos) = 's';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0xaf) // STD_SUP_T
            || (*(str + pos) == 0xa4 && *(str + pos + 1) == 0xc9) // STD_SUB_T
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x62) // STD_T_CEDILLA
           ) {
      *(str + pos) = 'T';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa0 && *(str + pos + 1) == 0x9c) // STD_SUB_t
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x63) // STD_t_CEDILLA
           ) {
      *(str + pos) = 't';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0xca) // STD_SUB_U
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xd9) // STD_U_GRAVE
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xda) // STD_U_ACUTE
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xdb) // STD_U_CIRC
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xdc) // STD_U_DIARESIS
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x68) // STD_U_TILDE
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x6a) // STD_U_MACRON
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x6c) // STD_U_BREVE
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x6e) // STD_U_RING
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x72) // STD_U_OGONEK
           ) {
      *(str + pos) = 'U';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0xb0) // STD_SUB_u
            || (*(str + pos) == 0x9d && *(str + pos + 1) == 0x64) // STD_SUB_u_b
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xf9) // STD_u_GRAVE
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xfa) // STD_u_ACUTE
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xfb) // STD_u_CIRC
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xfc) // STD_u_DIARESIS
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x69) // STD_u_TILDE
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x6b) // STD_u_MACRON
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x6d) // STD_u_BREVE
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x6f) // STD_u_RING
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x73) // STD_u_OGONEK
           ) {
      *(str + pos) = 'u';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0xcb) // STD_SUB_V
           ) {
      *(str + pos) = 'V';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0xb1) // STD_SUB_v
           ) {
      *(str + pos) = 'v';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0xcc) // STD_SUB_W
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x74) // STD_W_CIRC
           ) {
      *(str + pos) = 'W';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0xb2) // STD_SUB_w
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x75) // STD_w_CIRC
           ) {
      *(str + pos) = 'w';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0xcd) // STD_SUB_X
           ) {
      *(str + pos) = 'X';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0x9d && *(str + pos + 1) == 0x61) // STD_SUP_x
            || (*(str + pos) == 0xa0 && *(str + pos + 1) == 0x93) // STD_SUB_x_b
            || (*(str + pos) == 0xa4 && *(str + pos + 1) == 0xb3) // STD_SUB_x
            || (*(str + pos) == 0x83 && *(str + pos + 1) == 0x79) // STD_x_CIRC
            || (*(str + pos) == 0x83 && *(str + pos + 1) == 0x78) // STD_x_BAR
            || (*(str + pos) == 0x83 && *(str + pos + 1) == 0x7f) // STD_x_UNDER_ROOT
           ) {
      *(str + pos) = 'x';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0xce) // STD_SUB_Y
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xdd) // STD_Y_ACUTE
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x76) // STD_Y_CIRC
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x78) // STD_Y_DIARESIS
           ) {
      *(str + pos) = 'Y';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0xb4) // STD_SUB_y
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x76) // STD_y_CIRC
            || (*(str + pos) == 0x82 && *(str + pos + 1) == 0x33) // STD_y_BAR
            || (*(str + pos) == 0x82 && *(str + pos + 1) == 0x32) // STD_y_UNDER_ROOT
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xfd) // STD_y_ACUTE
            || (*(str + pos) == 0x80 && *(str + pos + 1) == 0xff) // STD_y_DIARESIS
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x77) // STD_y_CIRC
           ) {
      *(str + pos) = 'y';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0xcf) // STD_SUB_Z
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x79) // STD_Z_ACUTE
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x7b) // STD_Z_DOT
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x7d) // STD_Z_CARON
           ) {
      *(str + pos) = 'Z';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa4 && *(str + pos + 1) == 0xb5) // STD_SUB_z
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x7a) // STD_z_ACUTE
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x7c) // STD_z_DOT
            || (*(str + pos) == 0x81 && *(str + pos + 1) == 0x7e) // STD_z_CARON
           ) {
      *(str + pos) = 'z';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0xa2 && *(str + pos + 1) == 0x3c) // STD_TILDE_b
           ) {
      *(str + pos) = '~';
      memmove(str + pos + 1, str + pos + 2, stringByteLength((char *)str + pos + 2) + 1);
    }
    else if(   (*(str + pos) == 0x80 && *(str + pos + 1) == 0xc6) // STD_AE
           ) {
      *(str + pos) = 'A';
      *(str + pos + 1) = 'E';
      (*lg)++;
    }
    else if(   (*(str + pos) == 0x80 && *(str + pos + 1) == 0xe6) // STD_ae
           ) {
      *(str + pos) = 'a';
      *(str + pos + 1) = 'e';
      (*lg)++;
    }
    else if(   (*(str + pos) == 0x81 && *(str + pos + 1) == 0x52) // STD_OE
           ) {
      *(str + pos) = 'O';
      *(str + pos + 1) = 'E';
      (*lg)++;
    }
    else if(   (*(str + pos) == 0x81 && *(str + pos + 1) == 0x53) // STD_oe
           ) {
      *(str + pos) = 'o';
      *(str + pos + 1) = 'e';
      (*lg)++;
    }
    else if(   (*(str + pos) == 0x80 && *(str + pos + 1) == 0xc6) // STD_s_SHARP
           ) {
      *(str + pos) = 's';
      *(str + pos + 1) = 's';
      (*lg)++;
    }
    else if(   (*(str + pos) == 0x81 && *(str + pos + 1) == 0x0f) // STD_d_APOSTROPHE
           ) {
      *(str + pos) = 'd';
      *(str + pos + 1) = '\'';
      (*lg)++;
    }
    else if(   (*(str + pos) == 0x81 && *(str + pos + 1) == 0x3d) // STD_L_APOSTROPHE
           ) {
      *(str + pos) = 'L';
      *(str + pos + 1) = '\'';
      (*lg)++;
    }
    else if(   (*(str + pos) == 0x81 && *(str + pos + 1) == 0x3e) // STD_l_APOSTROPHE
           ) {
      *(str + pos) = 'l';
      *(str + pos + 1) = '\'';
      (*lg)++;
    }
    else if(   (*(str + pos) == 0x81 && *(str + pos + 1) == 0x65) // STD_t_APOSTROPHE
           ) {
      *(str + pos) = 't';
      *(str + pos + 1) = '\'';
      (*lg)++;
    }
    else if(   (*(str + pos) == 0x80 && *(str + pos + 1) == 0xbc) // STD_ONE_HALF
           ) {
      memmove(str + pos + 1, str + pos, stringByteLength((char *)str + pos) + 1);
      *(str + pos) = '1';
      *(str + pos + 1) = '/';
      *(str + pos + 2) = '2';
      (*lg)++;
    }
    else if(   (*(str + pos) == 0x80 && *(str + pos + 1) == 0xbd) // STD_ONE_QUARTER
           ) {
      memmove(str + pos + 1, str + pos, stringByteLength((char *)str + pos) + 1);
      *(str + pos) = '1';
      *(str + pos + 1) = '/';
      *(str + pos + 2) = '4';
      (*lg)++;
    }
    else if(   (*(str + pos) == 0xa0 && *(str + pos + 1) == 0x65) // STD_SUB_alpha
            || (*(str + pos) == 0x83 && *(str + pos + 1) == 0xac) // STD_alpha_TONOS
           ) {
      *(str + pos) = 0x83;
      *(str + pos + 1) = 0xb1;
    }
    else if(   (*(str + pos) == 0xa0 && *(str + pos + 1) == 0x66) // STD_SUB_delta
           ) {
      *(str + pos) = 0x83;
      *(str + pos + 1) = 0xb4;
    }
    else if(   (*(str + pos) == 0x83 && *(str + pos + 1) == 0xad) // STD_epsilon_TONOS
           ) {
      *(str + pos) = 0x83;
      *(str + pos + 1) = 0xb5;
    }
    else if(   (*(str + pos) == 0x83 && *(str + pos + 1) == 0xae) // STD_eta_TONOS
           ) {
      *(str + pos) = 0x83;
      *(str + pos + 1) = 0xb7;
    }
    else if(   (*(str + pos) == 0x83 && *(str + pos + 1) == 0xaa) // STD_IOTA_DIALYTIKA
           ) {
      *(str + pos) = 0x83;
      *(str + pos + 1) = 0x99;
    }
    else if(   (*(str + pos) == 0x83 && *(str + pos + 1) == 0x90) // STD_iota_DIALYTIKA_TONOS
            || (*(str + pos) == 0x83 && *(str + pos + 1) == 0xaf) // STD_iota_TONOS
            || (*(str + pos) == 0x83 && *(str + pos + 1) == 0xca) // STD_iota_DIALYTIKA
           ) {
      *(str + pos) = 0x83;
      *(str + pos + 1) = 0xb9;
    }
    else if(   (*(str + pos) == 0x83 && *(str + pos + 1) == 0xcc) // STD_omicron_TONOS
           ) {
      *(str + pos) = 0x83;
      *(str + pos + 1) = 0xbf;
    }
    else if(   (*(str + pos) == 0xa2 && *(str + pos + 1) == 0x0f) // STD_PRODUCT -> STD_PI
           ) {
      *(str + pos) = 0x83;
      *(str + pos + 1) = 0xc0;
    }
    else if(   (*(str + pos) == 0xa2 && *(str + pos + 1) == 0x11) // STD_SUM -> STD_SIGMA
           ) {
      *(str + pos) = 0x83;
      *(str + pos + 1) = 0xc3;
    }
    else if(   (*(str + pos) == 0x83 && *(str + pos + 1) == 0xab) // STD_UPSILON_DIALYTIKA
           ) {
      *(str + pos) = 0x83;
      *(str + pos + 1) = 0xa5;
    }
    else if(   (*(str + pos) == 0x83 && *(str + pos + 1) == 0xb0) // STD_upsilon_DIALYTIKA_TONOS
            || (*(str + pos) == 0x83 && *(str + pos + 1) == 0xcb) // STD_upsilon_DIALYTIKA
            || (*(str + pos) == 0x83 && *(str + pos + 1) == 0xcd) // STD_upsilon_TONOS
           ) {
      *(str + pos) = 0x83;
      *(str + pos + 1) = 0xc5;
    }
    else if(   (*(str + pos) == 0x80 && *(str + pos + 1) == 0xb5) // STD_mu_b
            || (*(str + pos) == 0xa0 && *(str + pos + 1) == 0x67) // STD_SUB_mu
           ) {
      *(str + pos) = 0x83;
      *(str + pos + 1) = 0xbc;
    }
    else if(   (*(str + pos) == 0x83 && *(str + pos + 1) == 0xce) // STD_omega_TONOS
           ) {
      *(str + pos) = 0x83;
      *(str + pos + 1) = 0xc9;
    }
    else if(   (*(str + pos) == 0xa2 && *(str + pos + 1) == 0x95) // STD_SUB_EARTH_b
           ) {
      *(str + pos) = 0xa0;
      *(str + pos + 1) = 0x69;
    }
    else if(   (*(str + pos) == 0xa0 && *(str + pos + 1) == 0x68) // STD_SUB_SUN
            || (*(str + pos) == 0xa2 && *(str + pos + 1) == 0x9a) // STD_SUB_SUN_b
           ) {
      *(str + pos) = 0xa2;
      *(str + pos + 1) = 0x99;
    }
    else if(   (*(str + pos) == 0xa0 && *(str + pos + 1) == 0x9e) // STD_SUP_INFINITY
            || (*(str + pos) == 0xa0 && *(str + pos + 1) == 0x9f) // STD_SUB_INFINITY
           ) {
      *(str + pos) = 0xa2;
      *(str + pos + 1) = 0x1e;
    }
    else if(   (*(str + pos) == 0xa1 && *(str + pos + 1) == 0xc8) // STD_UP_ARROW_b
           ) {
      *(str + pos) = 0xa1;
      *(str + pos + 1) = 0x91;
    }
    else if(   (*(str + pos) == 0xa1 && *(str + pos + 1) == 0xca) // STD_DOWN_ARROW_b
           ) {
      *(str + pos) = 0xa1;
      *(str + pos + 1) = 0x93;
    }*/

    pos = stringNextGlyph((char *)str, pos);
  }
}



int32_t compareString(const char *stra, const char *strb) {
  int32_t lgg1, lgg2, i;
  int16_t pos1, pos2, rank1, rank2;
  char str1[50], str2[50];

  lgg1 = stringGlyphLength(stra);
  lgg2 = stringGlyphLength(strb);

  memcpy(str1, stra, stringByteLength(stra) + 1);
  memcpy(str2, strb, stringByteLength(strb) + 1);

  stringCleanup((uint8_t *)str1, &lgg1);
  stringCleanup((uint8_t *)str2, &lgg2);

  pos1 = 0;
  pos2 = 0;
  for(i=0; i<min(lgg1, lgg2); i++) {
    rank1 = rankOfGlyph(str1 + pos1);
    rank2 = rankOfGlyph(str2 + pos2);

    if(rank1 < rank2) return -1;
    if(rank1 > rank2) return 1;

    pos1 = stringNextGlyph(str1, pos1);
    pos2 = stringNextGlyph(str2, pos2);
  }

  if(lgg1 < lgg2) return -1;
  if(lgg1 > lgg2) return 1;

  return 0;
}
