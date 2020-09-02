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
 * \file items.h
 ***********************************************/

#define NOPARAM       9876
#define CONFIRMED     9877
#define NOT_CONFIRMED 9878

// Stack Lift Status
#define SLS_ENABLED   0
#define SLS_DISABLED  1
#define SLS_UNCHANGED 2

// Undo Status
#define US_ENABLED    0 // The command saves the stack, the statistical sums, and the system flags for later UNDO
#define US_CANCEL     1 // The command cancels the last UNDO data
#define US_UNCHANGED  2 // The command leaves the existing UNDO data as is


#define ITM_NULL                         0
#define ITM_LBL                          1
#define ITM_GTO                          2
#define ITM_XEQ                          3
#define ITM_RTN                          4
#define ITM_ISE                          5
#define ITM_ISG                          6
#define ITM_ISZ                          7
#define ITM_DSE                          8
#define ITM_DSL                          9
#define ITM_DSZ                         10
#define ITM_XEQU                        11
#define ITM_XNE                         12
#define ITM_XEQUP0                      13
#define ITM_XEQUM0                      14
#define ITM_XAEQU                       15
#define ITM_XLT                         16
#define ITM_XLE                         17
#define ITM_XGE                         18
#define ITM_XGT                         19
#define ITM_FC                          20
#define ITM_FS                          21
#define ITM_EVEN                        22
#define ITM_ODD                         23
#define ITM_FPQ                         24
#define ITM_INT                         25
#define ITM_CPX                         26
#define ITM_MATR                        27
#define ITM_NAN                         28
#define ITM_REAL                        29
#define ITM_SPEC                        30
#define ITM_STRI                        31
#define ITM_PMINFINITY                  32
#define ITM_PRIME                       33
#define ITM_TOP                         34
#define ITM_ENTER                       35
#define ITM_XexY                        36
#define ITM_DROP                        37
#define ITM_DROPY                       38
#define ITM_Rup                         39
#define ITM_Rdown                       40
#define ITM_CLX                         41
#define ITM_FILL                        42
#define ITM_INPUT                       43
#define ITM_STO                         44
#define ITM_STOPLUS                     45
#define ITM_STOMINUS                    46
#define ITM_STOMULT                     47
#define ITM_STODIV                      48
#define ITM_STOMAX                      49
#define ITM_STOMIN                      50
#define ITM_RCL                         51
#define ITM_RCLPLUS                     52
#define ITM_RCLMINUS                    53
#define ITM_RCLMULT                     54
#define ITM_RCLDIV                      55
#define ITM_RCLMAX                      56
#define ITM_RCLMIN                      57
#define ITM_SQUARE                      58
#define ITM_CUBE                        59
#define ITM_YX                          60
#define ITM_SQUAREROOTX                 61
#define ITM_CUBEROOT                    62
#define ITM_XTHROOT                     63
#define ITM_2X                          64
#define ITM_EX                          65
#define ITM_EX1                         66
#define ITM_10x                         67
#define ITM_LOG2                        68
#define ITM_LN                          69
#define ITM_LN1X                        70
#define ITM_LOG10                       71
#define ITM_LOGXY                       72
#define ITM_1ONX                        73
#define ITM_cos                         74
#define ITM_cosh                        75
#define ITM_sin                         76
#define ITM_sinc                        77
#define ITM_sinh                        78
#define ITM_tan                         79
#define ITM_tanh                        80
#define ITM_arccos                      81
#define ITM_arcosh                      82
#define ITM_arcsin                      83
#define ITM_arsinh                      84
#define ITM_arctan                      85
#define ITM_artanh                      86
#define ITM_CEIL                        87
#define ITM_FLOOR                       88
#define ITM_GCD                         89
#define ITM_LCM                         90
#define ITM_DEC                         91
#define ITM_INC                         92
#define ITM_IP                          93
#define ITM_FP                          94
#define ITM_ADD                         95
#define ITM_SUB                         96
#define ITM_CHS                         97
#define ITM_MULT                        98
#define ITM_DIV                         99
#define ITM_CF                         100
#define ITM_IDIVR                      101
#define ITM_MOD                        102
#define ITM_MAX                        103
#define ITM_MIN                        104
#define ITM_MAGNITUDE                  105
#define ITM_NEIGHB                     106
#define ITM_NEXTP                      107
#define ITM_XFACT                      108
#define ITM_pi                         109

#define ITM_SF                         111
#define ITM_FF                         112















#define CST_01                         128
#define CST_02                         129
#define CST_03                         130
#define CST_04                         131
#define CST_05                         132
#define CST_06                         133
#define CST_07                         134
#define CST_08                         135
#define CST_09                         136
#define CST_10                         137
#define CST_11                         138
#define CST_12                         139
#define CST_13                         140
#define CST_14                         141
#define CST_15                         142
#define CST_16                         143
#define CST_17                         144
#define CST_18                         145
#define CST_19                         146
#define CST_20                         147
#define CST_21                         148
#define CST_22                         149
#define CST_23                         150
#define CST_24                         151
#define CST_25                         152
#define CST_26                         153
#define CST_27                         154
#define CST_28                         155
#define CST_29                         156
#define CST_30                         157
#define CST_31                         158
#define CST_32                         159
#define CST_33                         160
#define CST_34                         161
#define CST_35                         162
#define CST_36                         163
#define CST_37                         164
#define CST_38                         165
#define CST_39                         166
#define CST_40                         167
#define CST_41                         168
#define CST_42                         169
#define CST_43                         170
#define CST_44                         171
#define CST_45                         172
#define CST_46                         173
#define CST_47                         174
#define CST_48                         175
#define CST_49                         176
#define CST_50                         177
#define CST_51                         178
#define CST_52                         179
#define CST_53                         180
#define CST_54                         181
#define CST_55                         182
#define CST_56                         183
#define CST_57                         184
#define CST_58                         185
#define CST_59                         186
#define CST_60                         187
#define CST_61                         188
#define CST_62                         189
#define CST_63                         190
#define CST_64                         191
#define CST_65                         192
#define CST_66                         193
#define CST_67                         194
#define CST_68                         195
#define CST_69                         196
#define CST_70                         197
#define CST_71                         198
#define CST_72                         199
#define CST_73                         200
#define CST_74                         201
#define CST_75                         202
#define CST_76                         203
#define CST_77                         204
#define CST_78                         205
#define CST_79                         206
#define ITM_CNST                       207












#define ITM_CtoF                       220
#define ITM_FtoC                       221
#define ITM_DBtoPR                     222
#define ITM_DBtoPRb                    223
#define ITM_DBtoPRc                    224
#define ITM_DBtoFR                     225
#define ITM_DBtoFRb                    226
#define ITM_DBtoFRc                    227
#define ITM_PRtoDB                     228
#define ITM_PRtoDBb                    229
#define ITM_PRtoDBc                    230
#define ITM_FRtoDB                     231
#define ITM_FRtoDBb                    232
#define ITM_FRtoDBc                    233
#define ITM_ACtoM2                     234
#define ITM_ACtoM2b                    235
#define ITM_M2toAC                     236
#define ITM_M2toACb                    237
#define ITM_ACUStoM2                   238

#define ITM_M2toACUS                   240

#define ITM_ATMtoPA                    242
#define ITM_PAtoATM                    243
#define ITM_AUtoM                      244
#define ITM_MtoAU                      245
#define ITM_BARtoPA                    246
#define ITM_PAtoBAR                    247
#define ITM_BTUtoJ                     248
#define ITM_JtoBTU                     249
#define ITM_CALtoJ                     250
#define ITM_JtoCAL                     251
#define ITM_LBFFTtoNM                  252

#define ITM_NMtoLBFFT                  254

#define ITM_CWTtoKG                    256
#define ITM_KGtoCWT                    257
#define ITM_FTtoM                      258
#define ITM_MtoFT                      259
#define ITM_FTUStoM                    260
#define ITM_FTUStoMb                   261
#define ITM_FTUStoMc                   262
#define ITM_MtoFTUS                    263
#define ITM_MtoFTUSb                   264
#define ITM_MtoFTUSc                   265
#define ITM_FZUKtoM3                   266

#define ITM_M3toFZUK                   268

#define ITM_FZUStoM3                   270

#define ITM_M3toFZUS                   272

#define ITM_GLUKtoM3                   274
#define ITM_M3toGLUK                   275
#define ITM_GLUStoM3                   276
#define ITM_M3toGLUS                   277
#define ITM_HPEtoW                     278
#define ITM_WtoHPE                     279
#define ITM_HPMtoW                     280
#define ITM_WtoHPM                     281
#define ITM_HPUKtoW                    282
#define ITM_WtoHPUK                    283
#define ITM_IHGtoPA                    284
#define ITM_IHGtoPAb                   285
#define ITM_PAtoIHG                    286
#define ITM_PAtoIHGb                   287
#define ITM_INtoM                      288
#define ITM_MtoINCH                    289
#define ITM_WHtoJ                      290
#define ITM_JtoWH                      291
#define ITM_KGtoLBS                    292
#define ITM_LBStoKG                    293
#define ITM_KGtoOZ                     294
#define ITM_OZtoKG                     295
#define ITM_KGtoSCW                    296
#define ITM_KGtoSCWb                   297
#define ITM_SCWtoKG                    298
#define ITM_SCWtoKGb                   299
#define ITM_KGtoSTO                    300
#define ITM_KGtoSTOb                   301
#define ITM_STOtoKG                    302
#define ITM_STOtoKGb                   303
#define ITM_KGtoST                     304
#define ITM_KGtoSTb                    305
#define ITM_KGtoSTc                    306
#define ITM_STtoKG                     307
#define ITM_STtoKGb                    308
#define ITM_STtoKGc                    309
#define ITM_KGtoTON                    310
#define ITM_KGtoTONb                   311
#define ITM_KGtoTONc                   312
#define ITM_TONtoKG                    313
#define ITM_TONtoKGb                   314
#define ITM_TONtoKGc                   315
#define ITM_KGtoTRZ                    316
#define ITM_KGtoTRZb                   317
#define ITM_TRZtoKG                    318
#define ITM_TRZtoKGb                   319
#define ITM_LBFtoN                     320
#define ITM_NtoLBF                     321
#define ITM_LYtoM                      322
#define ITM_MtoLY                      323
#define ITM_MMHGtoPA                   324

#define ITM_PAtoMMHG                   326

#define ITM_MItoM                      328
#define ITM_MtoMI                      329
#define ITM_MtoNMI                     330
#define ITM_NMItoM                     331
#define ITM_MtoPC                      332
#define ITM_PCtoM                      333
#define ITM_MtoPOINTb                  334


#define ITM_POINTtoM                   337


#define ITM_MtoYD                      340
#define ITM_YDtoM                      341
#define ITM_PSItoPA                    342
#define ITM_PAtoPSI                    343
#define ITM_PAtoTOR                    344
#define ITM_PAtoTORb                   345
#define ITM_TORtoPA                    346
#define ITM_TORtoPAb                   347
#define ITM_StoYEAR                    348
#define ITM_YEARtoS                    349
#define ITM_CARATtoKG                  350


#define ITM_KGtoCARAT                  353


#define ITM_QTtoM3                     356
#define ITM_M3toQT                     357
#define ITM_FATHOMtoM                  358


#define ITM_MtoFATHOM                  361


#define ITM_BARRELtoM3                 364

#define ITM_M3toBARREL                 366

#define ITM_ATMtoPAb                   368
#define ITM_PAtoATMb                   369
#define ITM_HECTAREtoM2                370
#define ITM_M2toHECTARE                371














#define ITM_FCC                        386
#define ITM_FCS                        387
#define ITM_FCF                        388
#define ITM_FSC                        389
#define ITM_FSS                        390
#define ITM_FSF                        391
#define ITM_NOT                        392
#define ITM_AND                        393
#define ITM_OR                         394
#define ITM_XOR                        395
#define ITM_NAND                       396
#define ITM_NOR                        397
#define ITM_XNOR                       398
#define ITM_BS                         399
#define ITM_BC                         400
#define ITM_CB                         401
#define ITM_SB                         402
#define ITM_FB                         403
#define ITM_RL                         404
#define ITM_RLC                        405
#define ITM_RR                         406
#define ITM_RRC                        407
#define ITM_SL                         408
#define ITM_SR                         409
#define ITM_ASR                        410
#define ITM_LJ                         411
#define ITM_RJ                         412
#define ITM_MASKL                      413
#define ITM_MASKR                      414
#define ITM_MIRROR                     415
#define ITM_NUMB                       416
#define ITM_SDL                        417
#define ITM_SDR                        418




#define ITM_SIGMAPLUS                  423
#define ITM_SIGMAMINUS                 424
#define ITM_NSIGMA                     425
#define ITM_SIGMAx                     426
#define ITM_SIGMAy                     427
#define ITM_SIGMAx2                    428
#define ITM_SIGMAx2y                   429
#define ITM_SIGMAy2                    430
#define ITM_SIGMAxy                    431
#define ITM_SIGMAlnxy                  432

#define ITM_SIGMAln2x                  434
#define ITM_SIGMAylnx                  435
#define ITM_SIGMAlny                   436

#define ITM_SIGMAxlny                  438

#define ITM_SIGMAx2ony                 440
#define ITM_SIGMA1onx                  441

#define ITM_SIGMAxony                  443


#define ITM_SIGMAx3                    446
#define ITM_SIGMAx4                    447





#define SFL_USB                        453
#define SFL_TDM24                      454
#define SFL_YMD                        455
#define SFL_DMY                        456
#define SFL_MDY                        457
#define SFL_CPXRES                     458
#define SFL_CPXj                       459
#define SFL_POLAR                      460
#define SFL_FRACT                      461
#define SFL_PROPFR                     462
#define SFL_DENANY                     463
#define SFL_DENFIX                     464
#define SFL_CARRY                      465
#define SFL_OVERFL                     466
#define SFL_LEAD0                      467
#define SFL_ALPHA                      468
#define SFL_alphaCAP                   469
#define SFL_RUNTIM                     470
#define SFL_RUNIO                      471
#define SFL_PRINT                      472
#define SFL_TRACE                      473
#define SFL_USER                       474
#define SFL_LOWBAT                     475
#define SFL_SLOW                       476
#define SFL_SPCRES                     477
#define SFL_SSIZE8                     478
#define SFL_QUIET                      479
#define SFL_DECIMP                     480
#define SFL_MULTx                      481
#define SFL_ALLENG                     482
#define SFL_GROW                       483
#define SFL_AUTOFF                     484
#define SFL_AUTXEQ                     485
#define SFL_PRTACT                     486
#define SFL_NUMIN                      487
#define SFL_ALPIN                      488
#define SFL_ASLIFT                     489
#define SFL_IGN1ER                     490
#define SFL_INTING                     491
#define SFL_SOLVING                    492
#define SFL_VMDISP                     493























#define ITM_ST_A                       517
#define ITM_ST_B                       518
#define ITM_ST_C                       519
#define ITM_ST_D                       520
#define ITM_ST_T                       521
#define ITM_ST_X                       522
#define ITM_ST_Y                       523
#define ITM_ST_Z                       524
#define ITM_INDIRECTION                525
#define ITM_REG_X                      526
#define ITM_REG_Y                      527
#define ITM_REG_Z                      528
#define ITM_REG_T                      529
#define ITM_REG_A                      530
#define ITM_REG_B                      531
#define ITM_REG_C                      532
#define ITM_REG_D                      533
#define ITM_REG_L                      534
#define ITM_REG_I                      535
#define ITM_REG_J                      536
#define ITM_REG_K                      537
#define CHR_0                          538
#define CHR_1                          539
#define CHR_2                          540
#define CHR_3                          541
#define CHR_4                          542
#define CHR_5                          543
#define CHR_6                          544
#define CHR_7                          545
#define CHR_8                          546
#define CHR_9                          547
#define CHR_A                          548
#define CHR_B                          549
#define CHR_C                          550
#define CHR_D                          551
#define CHR_E                          552
#define CHR_F                          553
#define CHR_G                          554
#define CHR_H                          555
#define CHR_I                          556
#define CHR_J                          557
#define CHR_K                          558
#define CHR_L                          559
#define CHR_M                          560
#define CHR_N                          561
#define CHR_O                          562
#define CHR_P                          563
#define CHR_Q                          564
#define CHR_R                          565
#define CHR_S                          566
#define CHR_T                          567
#define CHR_U                          568
#define CHR_V                          569
#define CHR_W                          570
#define CHR_X                          571
#define CHR_Y                          572
#define CHR_Z                          573
#define CHR_a                          574
#define CHR_b                          575
#define CHR_c                          576
#define CHR_d                          577
#define CHR_e                          578
#define CHR_f                          579
#define CHR_g                          580
#define CHR_h                          581
#define CHR_i                          582
#define CHR_j                          583
#define CHR_k                          584
#define CHR_l                          585
#define CHR_m                          586
#define CHR_n                          587
#define CHR_o                          588
#define CHR_p                          589
#define CHR_q                          590
#define CHR_r                          591
#define CHR_s                          592
#define CHR_t                          593
#define CHR_u                          594
#define CHR_v                          595
#define CHR_w                          596
#define CHR_x                          597
#define CHR_y                          598
#define CHR_z                          599
#define CHR_ALPHA                      600   //NOTE the RANGE STARTS HERE, with +36 for lower case

#define CHR_BETA                       602
#define CHR_GAMMA                      603
#define CHR_DELTA                      604
#define CHR_EPSILON                    605

#define CHR_ZETA                       607
#define CHR_ETA                        608

#define CHR_THETA                      610
#define CHR_IOTA                       611


#define CHR_IOTA_DIALYTIKA             614
#define CHR_KAPPA                      615
#define CHR_LAMBDA                     616
#define CHR_MU                         617
#define CHR_NU                         618
#define CHR_XI                         619
#define CHR_OMICRON                    620

#define CHR_PI                         622
#define CHR_RHO                        623
#define CHR_SIGMA                      624

#define CHR_TAU                        626
#define CHR_UPSILON                    627

#define CHR_UPSILON_DIALYTIKA          629

#define CHR_PHI                        631
#define CHR_CHI                        632
#define CHR_PSI                        633
#define CHR_OMEGA                      634   //NOTE the RANGE STOPS HERE, with +36 for lower case

#define CHR_alpha                      636
#define CHR_alpha_TONOS                637
#define CHR_beta                       638
#define CHR_gamma                      639
#define CHR_delta                      640
#define CHR_epsilon                    641
#define CHR_epsilon_TONOS              642
#define CHR_zeta                       643
#define CHR_eta                        644
#define CHR_eta_TONOS                  645
#define CHR_theta                      646
#define CHR_iota                       647
#define CHR_iotaTON                    648
#define CHR_iota_DIALYTIKA             649

#define CHR_kappa                      651
#define CHR_lambda                     652
#define CHR_mu                         653
#define CHR_nu                         654
#define CHR_xi                         655
#define CHR_omicron                    656
#define CHR_omicron_TONOS              657
#define CHR_pi                         658
#define CHR_rho                        659
#define CHR_sigma                      660
#define CHR_sigma_end                  661
#define CHR_tau                        662
#define CHR_upsilon                    663
#define CHR_upsilon_DIALYTIKA          664


#define CHR_phi                        667
#define CHR_chi                        668
#define CHR_psi                        669
#define CHR_omega                      670
#define CHR_omega_TONOS                671






#define CHR_A_MACRON                   678
#define CHR_A_ACUTE                    679
#define CHR_A_BREVE                    680
#define CHR_A_GRAVE                    681
#define CHR_A_DIARESIS                 682
#define CHR_A_TILDE                    683
#define CHR_A_CIRC                     684
#define CHR_A_RING                     685
#define CHR_AE                         686
#define CHR_A_OGONEK                   687
#define CHR_C_ACUTE                    688
#define CHR_C_CARON                    689
#define CHR_C_CEDILLA                  690
#define CHR_D_STROKE                   691
#define CHR_D_CARON                    692
#define CHR_E_MACRON                   693
#define CHR_E_ACUTE                    694
#define CHR_E_BREVE                    695
#define CHR_E_GRAVE                    696
#define CHR_E_DIARESIS                 697
#define CHR_E_CIRC                     698
#define CHR_E_OGONEK                   699
#define CHR_G_BREVE                    700

#define CHR_I_MACRON                   702
#define CHR_I_ACUTE                    703
#define CHR_I_BREVE                    704
#define CHR_I_GRAVE                    705
#define CHR_I_DIARESIS                 706
#define CHR_I_CIRC                     707
#define CHR_I_OGONEK                   708
#define CHR_I_DOT                      709
#define CHR_I_DOTLESS                  710
#define CHR_L_STROKE                   711
#define CHR_L_ACUTE                    712
#define CHR_L_APOSTROPHE               713
#define CHR_N_ACUTE                    714
#define CHR_N_CARON                    715
#define CHR_N_TILDE                    716
#define CHR_O_MACRON                   717
#define CHR_O_ACUTE                    718
#define CHR_O_BREVE                    719
#define CHR_O_GRAVE                    720
#define CHR_O_DIARESIS                 721
#define CHR_O_TILDE                    722
#define CHR_O_CIRC                     723
#define CHR_O_STROKE                   724
#define CHR_OE                         725


#define CHR_S_SHARP                    728
#define CHR_S_ACUTE                    729
#define CHR_S_CARON                    730
#define CHR_S_CEDILLA                  731
#define CHR_T_CARON                    732
#define CHR_T_CEDILLA                  733
#define CHR_U_MACRON                   734
#define CHR_U_ACUTE                    735
#define CHR_U_BREVE                    736
#define CHR_U_GRAVE                    737
#define CHR_U_DIARESIS                 738
#define CHR_U_TILDE                    739
#define CHR_U_CIRC                     740
#define CHR_U_RING                     741
#define CHR_W_CIRC                     742



#define CHR_Y_CIRC                     746
#define CHR_Y_ACUTE                    747
#define CHR_Y_DIARESIS                 748
#define CHR_Z_ACUTE                    749
#define CHR_Z_CARON                    750
#define CHR_Z_DOT                      751






#define CHR_a_MACRON                   758
#define CHR_a_ACUTE                    759
#define CHR_a_BREVE                    760
#define CHR_a_GRAVE                    761
#define CHR_a_DIARESIS                 762
#define CHR_a_TILDE                    763
#define CHR_a_CIRC                     764
#define CHR_a_RING                     765
#define CHR_ae                         766
#define CHR_a_OGONEK                   767
#define CHR_c_ACUTE                    768
#define CHR_c_CARON                    769
#define CHR_c_CEDILLA                  770
#define CHR_d_STROKE                   771
#define CHR_d_APOSTROPHE               772
#define CHR_e_MACRON                   773
#define CHR_e_ACUTE                    774
#define CHR_e_BREVE                    775
#define CHR_e_GRAVE                    776
#define CHR_e_DIARESIS                 777
#define CHR_e_CIRC                     778
#define CHR_e_OGONEK                   779
#define CHR_g_BREVE                    780
#define CHR_h_STROKE                   781
#define CHR_i_MACRON                   782
#define CHR_i_ACUTE                    783
#define CHR_i_BREVE                    784
#define CHR_i_GRAVE                    785
#define CHR_i_DIARESIS                 786
#define CHR_i_CIRC                     787
#define CHR_i_OGONEK                   788
#define CHR_i_DOT                      789
#define CHR_i_DOTLESS                  790
#define CHR_l_STROKE                   791
#define CHR_l_ACUTE                    792
#define CHR_l_APOSTROPHE               793
#define CHR_n_ACUTE                    794
#define CHR_n_CARON                    795
#define CHR_n_TILDE                    796
#define CHR_o_MACRON                   797
#define CHR_o_ACUTE                    798
#define CHR_o_BREVE                    799
#define CHR_o_GRAVE                    800
#define CHR_o_DIARESIS                 801
#define CHR_o_TILDE                    802
#define CHR_o_CIRC                     803
#define CHR_o_STROKE                   804
#define CHR_oe                         805
#define CHR_r_CARON                    806
#define CHR_r_ACUTE                    807
#define CHR_s_SHARP                    808
#define CHR_s_ACUTE                    809
#define CHR_s_CARON                    810
#define CHR_s_CEDILLA                  811
#define CHR_t_APOSTROPHE               812
#define CHR_t_CEDILLA                  813
#define CHR_u_MACRON                   814
#define CHR_u_ACUTE                    815
#define CHR_u_BREVE                    816
#define CHR_u_GRAVE                    817
#define CHR_u_DIARESIS                 818
#define CHR_u_TILDE                    819
#define CHR_u_CIRC                     820
#define CHR_u_RING                     821
#define CHR_w_CIRC                     822
#define CHR_x_BAR                      823
#define CHR_x_CIRC                     824
#define CHR_y_BAR                      825
#define CHR_y_CIRC                     826
#define CHR_y_ACUTE                    827
#define CHR_y_DIARESIS                 828
#define CHR_z_ACUTE                    829
#define CHR_z_CARON                    830
#define CHR_z_DOT                      831







#define CHR_SUB_alpha                  838
#define CHR_SUB_delta                  839
#define CHR_SUB_mu                     840
#define CHR_SUB_SUN                    841
#define CHR_SUB_SUN_b                  842
#define CHR_SUB_EARTH                  843

#define CHR_SUB_PLUS                   845
#define CHR_SUB_MINUS                  846
#define CHR_SUB_INFINITY               847
#define CHR_SUB_0                      848
#define CHR_SUB_1                      849
#define CHR_SUB_2                      850
#define CHR_SUB_3                      851
#define CHR_SUB_4                      852
#define CHR_SUB_5                      853
#define CHR_SUB_6                      854
#define CHR_SUB_7                      855
#define CHR_SUB_8                      856
#define CHR_SUB_9                      857
#define CHR_SUB_10                     858
#define CHR_SUB_A                      859
#define CHR_SUB_B                      860
#define CHR_SUB_C                      861
#define CHR_SUB_D                      862
#define CHR_SUB_E                      863
#define CHR_SUB_F                      864
#define CHR_SUB_G                      865
#define CHR_SUB_H                      866
#define CHR_SUB_I                      867
#define CHR_SUB_J                      868
#define CHR_SUB_K                      869
#define CHR_SUB_L                      870
#define CHR_SUB_M                      871
#define CHR_SUB_N                      872
#define CHR_SUB_O                      873

#define CHR_SUB_P                      874
#define CHR_SUB_Q                      875
#define CHR_SUB_R                      876
#define CHR_SUB_S                      877
#define CHR_SUB_T                      878
#define CHR_SUB_U                      879
#define CHR_SUB_V                      880
#define CHR_SUB_W                      881
#define CHR_SUB_X                      882
#define CHR_SUB_Y                      883
#define CHR_SUB_Z                      884
#define CHR_SUB_E_OUTLINE              885
#define CHR_SUB_a                      886
#define CHR_SUB_b                      887
#define CHR_SUB_c                      888
#define CHR_SUB_d                      889

#define CHR_SUB_e                      890
#define CHR_SUB_h                      891
#define CHR_SUB_i                      892
#define CHR_SUB_j                      893
#define CHR_SUB_k                      894
#define CHR_SUB_l                      895
#define CHR_SUB_m                      896
#define CHR_SUB_n                      897
#define CHR_SUB_o                      898
#define CHR_SUB_p                      899
#define CHR_SUB_q                      900
#define CHR_SUB_s                      901
#define CHR_SUB_t                      902
#define CHR_SUB_u                      903
#define CHR_SUB_v                      904
#define CHR_SUB_w                      905
#define CHR_SUB_x                      906
#define CHR_SUB_y                      907
#define CHR_SUB_z                      908
#define CHR_SUB_a_b                    909
#define CHR_SUB_e_b                    910
#define CHR_SUB_k_b                    911
#define CHR_SUB_l_b                    912
#define CHR_SUB_m_b                    913
#define CHR_SUB_n_b                    914
#define CHR_SUB_o_b                    915
#define CHR_SUB_p_b                    916
#define CHR_SUB_s_b                    917
#define CHR_SUB_u_b                    918
#define CHR_SUB_x_b                    919
#define CHR_SUP_PLUS                   920
#define CHR_SUP_MINUS                  921

#define CHR_SUP_INFINITY               923
#define CHR_SUP_ASTERISK               924
#define CHR_SUP_0                      925
#define CHR_SUP_1                      926
#define CHR_SUP_2                      927
#define CHR_SUP_3                      928
#define CHR_SUP_4                      929
#define CHR_SUP_5                      930
#define CHR_SUP_6                      931
#define CHR_SUP_7                      932
#define CHR_SUP_8                      933
#define CHR_SUP_9                      934

#define CHR_SUP_T                      936
#define CHR_SUP_a                      937
#define CHR_SUP_f                      938
#define CHR_SUP_g                      939
#define CHR_SUP_h                      940
#define CHR_SUP_r                      941
#define CHR_SUP_x                      942
#define CHR_SPACE                      943
#define CHR_EXCLAMATION_MARK           944
#define CHR_DOUBLE_QUOTE               945
#define CHR_NUMBER_SIGN                946
#define CHR_DOLLAR                     947
#define CHR_PERCENT                    948
#define CHR_AMPERSAND                  949
#define CHR_QUOTE                      950
#define CHR_LEFT_PARENTHESIS           951
#define CHR_RIGHT_PARENTHESIS          952
#define CHR_ASTERISK                   953

#define CHR_PLUS                       955
#define CHR_COMMA                      956
#define CHR_MINUS                      957
#define CHR_PERIOD                     958
#define CHR_SLASH                      959
#define CHR_COLON                      960
#define CHR_SEMICOLON                  961
#define CHR_LESS_THAN                  962
#define CHR_EQUAL                      963
#define CHR_GREATER_THAN               964
#define CHR_QUESTION_MARK              965
#define CHR_AT                         966
#define CHR_LEFT_SQUARE_BRACKET        967
#define CHR_BACK_SLASH                 968
#define CHR_RIGHT_SQUARE_BRACKET       969
#define CHR_CIRCUMFLEX                 970
#define CHR_UNDERSCORE                 971
#define CHR_LEFT_CURLY_BRACKET         972
#define CHR_PIPE                       973
#define CHR_RIGHT_CURLY_BRACKET        974
#define CHR_TILDE                      975
#define CHR_INVERTED_EXCLAMATION_MARK  976
#define CHR_CENT                       977
#define CHR_POUND                      978
#define CHR_YEN                        979
#define CHR_SECTION                    980
#define CHR_OVERFLOW_CARRY             981
#define CHR_LEFT_DOUBLE_ANGLE          982
#define CHR_NOT                        983
#define CHR_DEGREE                     984
#define CHR_PLUS_MINUS                 985
#define CHR_mu_b                       986
#define CHR_DOT                        987
#define CHR_ORDINAL                    988
#define CHR_RIGHT_DOUBLE_ANGLE         989
#define CHR_ONE_HALF                   990
#define CHR_ONE_QUARTER                991

#define CHR_ETH                        993
#define CHR_CROSS                      994
#define CHR_eth                        995
#define CHR_DIVIDE                     996
#define CHR_E_DOT                      997
#define CHR_e_DOT                      998
#define CHR_E_CARON                    999
#define CHR_e_CARON                   1000
#define CHR_R_ACUTE                   1001
#define CHR_R_CARON                   1002
#define CHR_U_OGONEK                  1003
#define CHR_u_OGONEK                  1004
#define CHR_y_UNDER_ROOT              1005
#define CHR_x_UNDER_ROOT              1006
#define CHR_SPACE_EM                  1007
#define CHR_SPACE_3_PER_EM            1008
#define CHR_SPACE_4_PER_EM            1009
#define CHR_SPACE_6_PER_EM            1010
#define CHR_SPACE_FIGURE              1011
#define CHR_SPACE_PUNCTUATION         1012
#define CHR_SPACE_HAIR                1013
#define CHR_LEFT_SINGLE_QUOTE         1014
#define CHR_RIGHT_SINGLE_QUOTE        1015
#define CHR_SINGLE_LOW_QUOTE          1016
#define CHR_SINGLE_HIGH_QUOTE         1017


#define CHR_DOUBLE_LOW_QUOTE          1020
#define CHR_DOUBLE_HIGH_QUOTE         1021
#define CHR_ELLIPSIS                  1022
#define CHR_ONE                       1023
#define CHR_EURO                      1024
#define CHR_COMPLEX_C                 1025
#define CHR_PLANCK                    1026
#define CHR_PLANCK_2PI                1027
#define CHR_NATURAL_N                 1028
#define CHR_RATIONAL_Q                1029
#define CHR_REAL_R                    1030
#define CHR_LEFT_ARROW                1031
#define CHR_UP_ARROW                  1032

#define CHR_RIGHT_ARROW               1034
#define CHR_DOWN_ARROW                1035

#define CHR_SERIAL_IO                 1037
#define CHR_RIGHT_SHORT_ARROW         1038
#define CHR_LEFT_RIGHT_ARROWS         1039
#define CHR_BST                       1040
#define CHR_SST                       1041
#define CHR_HAMBURGER                 1042
#define CHR_UNDO                      1043
#define CHR_FOR_ALL                   1044
#define CHR_COMPLEMENT                1045
#define CHR_PARTIAL_DIFF              1046
#define CHR_THERE_EXISTS              1047
#define CHR_THERE_DOES_NOT_EXIST      1048
#define CHR_EMPTY_SET                 1049
#define CHR_INCREMENT                 1050
#define CHR_NABLA                     1051
#define CHR_ELEMENT_OF                1052
#define CHR_NOT_ELEMENT_OF            1053
#define CHR_CONTAINS                  1054
#define CHR_DOES_NOT_CONTAIN          1055
#define CHR_ZERO                      1056
#define CHR_PRODUCT                   1057


#define CHR_MINUS_PLUS                1060


#define CHR_RING                      1063
#define CHR_BULLET                    1064
#define CHR_SQUARE_ROOT               1065
#define CHR_CUBE_ROOT                 1066
#define CHR_xTH_ROOT                  1067
#define CHR_PROPORTIONAL              1068
#define CHR_INFINITY                  1069
#define CHR_RIGHT_ANGLE               1070
#define CHR_ANGLE                     1071
#define CHR_MEASURED_ANGLE            1072
#define CHR_DIVIDES                   1073

#define CHR_PARALLEL                  1075
#define CHR_NOT_PARALLEL              1076
#define CHR_AND                       1077
#define CHR_OR                        1078
#define CHR_INTERSECTION              1079
#define CHR_UNION                     1080
#define CHR_INTEGRAL                  1081
#define CHR_DOUBLE_INTEGRAL           1082
#define CHR_CONTOUR_INTEGRAL          1083
#define CHR_SURFACE_INTEGRAL          1084
#define CHR_RATIO                     1085
#define CHR_CHECK_MARK                1086
#define CHR_ASYMPOTICALLY_EQUAL       1087
#define CHR_ALMOST_EQUAL              1088
#define CHR_COLON_EQUALS              1089
#define CHR_CORRESPONDS_TO            1090
#define CHR_ESTIMATES                 1091
#define CHR_NOT_EQUAL                 1092
#define CHR_IDENTICAL_TO              1093
#define CHR_LESS_EQUAL                1094

#define CHR_MUCH_LESS                 1096
#define CHR_MUCH_GREATER              1097
#define CHR_SUN                       1098
#define CHR_DOWN_TACK                 1099
#define CHR_PERPENDICULAR             1100
#define CHR_XOR                       1101
#define CHR_NAND                      1102
#define CHR_NOR                       1103
#define CHR_WATCH                     1104
#define CHR_HOURGLASS                 1105
#define CHR_PRINTER                   1106
#define CHR_MAT_TL                    1107
#define CHR_MAT_ML                    1108
#define CHR_MAT_BL                    1109
#define CHR_MAT_TR                    1110
#define CHR_MAT_MR                    1111
#define CHR_MAT_BR                    1112
#define CHR_OBLIQUE1                  1113
#define CHR_OBLIQUE2                  1114
#define CHR_OBLIQUE3                  1115
#define CHR_OBLIQUE4                  1116
#define CHR_CURSOR                    1117
#define CHR_PERIOD34                  1118
#define CHR_COMMA34                   1119
#define CHR_BATTERY                   1120
#define CHR_PGM_BEGIN                 1121
#define CHR_USER_MODE                 1122
#define CHR_UK                        1123
#define CHR_US                        1124
#define CHR_NEG_EXCLAMATION_MARK      1125
#define CHR_ex                        1126
#define ITM_Max                       1127
#define ITM_Min                       1128
#define ITM_Config                    1129
#define ITM_Stack                     1130
#define ITM_dddEL                     1131
#define ITM_dddIJ                     1132
#define ITM_0P                        1133
#define ITM_1P                        1134
#define ITM_EXPONENT                  1135
#define ITM_MA11                      1136
#define ITM_MA12                      1137
#define ITM_MA13                      1138
#define ITM_MA14                      1139
#define ITM_MA15                      1140
#define ITM_MA16                      1141
#define ITM_MA21                      1142
#define ITM_MA22                      1143
#define ITM_MA23                      1144
#define ITM_MA24                      1145
#define ITM_MA25                      1146
#define ITM_MA26                      1147
#define ITM_MA31                      1148
#define ITM_MA32                      1149
#define ITM_MA33                      1150
#define ITM_MA34                      1151
#define ITM_MA35                      1152
#define ITM_MA36                      1153
#define ITM_HEX                       1154










#define ITM_REGI                      1165
#define ITM_REGJ                      1166
#define ITM_REGK                      1167
#define ITM_REGL                      1168
#define ITM_REGA                      1169
#define ITM_ACC                       1170
#define ITM_REGB                      1171
#define ITM_REGC                      1172
#define ITM_REGD                      1173
#define ITM_FV                        1174
#define ITM_IPCA                      1175
#define ITM_MATA                      1176
#define ITM_MATB                      1177
#define ITM_NPER                      1178
#define ITM_PERonA                    1179
#define ITM_PMT                       1180
#define ITM_PV                        1181
#define ITM_REGS                      1182
#define ITM_ULIM                      1183
#define ITM_DLIM                      1184
#define CHR_1185                      1185
#define CHR_1186                      1186
#define CHR_1187                      1187
#define CHR_1188                      1188
#define CHR_1189                      1189








#define MNU_BINOM                     1197
#define ITM_BINOMP                    1198
#define ITM_BINOM                     1199
#define ITM_BINOMU                    1200
#define ITM_BINOMM1                   1201
#define MNU_CAUCH                     1202
#define ITM_CAUCHP                    1203
#define ITM_CAUCH                     1204
#define ITM_CAUCHU                    1205
#define ITM_CAUCHM1                   1206
#define MNU_EXPON                     1207
#define ITM_EXPONP                    1208
#define ITM_EXPON                     1209
#define ITM_EXPONU                    1210
#define ITM_EXPONM1                   1211
#define MNU_F                         1212
#define ITM_FPX                       1213
#define ITM_FX                        1214
#define ITM_FUX                       1215
#define ITM_FM1P                      1216
#define MNU_GEOM                      1217
#define ITM_GEOMP                     1218
#define ITM_GEOM                      1219
#define ITM_GEOMU                     1220
#define ITM_GEOMM1                    1221
#define MNU_HYPER                     1222
#define ITM_HYPERP                    1223
#define ITM_HYPER                     1224
#define ITM_HYPERU                    1225
#define ITM_HYPERM1                   1226
#define MNU_LGNRM                     1227
#define ITM_LGNRMP                    1228
#define ITM_LGNRM                     1229
#define ITM_LGNRMU                    1230
#define ITM_LGNRMM1                   1231
#define MNU_LOGIS                     1232
#define ITM_LOGISP                    1233
#define ITM_LOGIS                     1234
#define ITM_LOGISU                    1235
#define ITM_LOGISM1                   1236
#define MNU_NBIN                      1237
#define ITM_NBINP                     1238
#define ITM_NBIN                      1239
#define ITM_NBINU                     1240
#define ITM_NBINM1                    1241
#define MNU_NORML                     1242
#define ITM_NORMLP                    1243
#define ITM_NORML                     1244
#define ITM_NORMLU                    1245
#define ITM_NORMLM1                   1246
#define MNU_POISS                     1247
#define ITM_POISSP                    1248
#define ITM_POISS                     1249
#define ITM_POISSU                    1250
#define ITM_POISSM1                   1251
#define MNU_T                         1252
#define ITM_TPX                       1253
#define ITM_TX                        1254
#define ITM_TUX                       1255
#define ITM_TM1P                      1256
#define MNU_WEIBL                     1257
#define ITM_WEIBLP                    1258
#define ITM_WEIBL                     1259
#define ITM_WEIBLU                    1260
#define ITM_WEIBLM1                   1261
#define MNU_CHI2                      1262
#define ITM_chi2Px                    1263
#define ITM_chi2x                     1264
#define ITM_chi2ux                    1265
#define ITM_chi2M1                    1266





















#define ITM_BESTF                     1287
#define ITM_EXPF                      1288
#define ITM_LINF                      1289
#define ITM_LOGF                      1290
#define ITM_ORTHOF                    1291
#define ITM_POWERF                    1292
#define ITM_GAUSSF                    1293
#define ITM_CAUCHF                    1294

#define ITM_PARABF                    1295
#define ITM_HYPF                      1296
#define ITM_ROOTF                     1297





#define MNU_ADV                       1303
#define MNU_ANGLES                    1304
#define MNU_PRINT                     1305
#define MNU_CONVA                     1306
#define MNU_BITS                      1307
#define MNU_CATALOG                   1308
#define MNU_CHARS                     1309
#define MNU_CLK                       1310
#define MNU_REGIST                    1311
#define MNU_CLR                       1312
#define MNU_CONST                     1313
#define MNU_CPX                       1314
#define MNU_CPXS                      1315
#define MNU_DATES                     1316
#define MNU_DIGITS                    1317
#define MNU_DSP                       1318
#define MNU_EQN                       1319
#define MNU_EXP                       1320
#define MNU_CONVE                     1321
#define MNU_FCNS                      1322
#define MNU_FIN                       1323
#define MNU_SINTS                     1324
#define MNU_FLAGS                     1325
#define MNU_FLASH                     1326
#define MNU_1STDERIV                  1327
#define MNU_2NDDERIV                  1328
#define MNU_CONVFP                    1329
#define MNU_LINTS                     1330
#define MNU_INFO                      1331
#define MNU_INTS                      1332
#define MNU_IO                        1333
#define MNU_LOOP                      1334
#define MNU_MATRS                     1335
#define MNU_MATX                      1336
#define MNU_MENUS                     1337
#define MNU_MODE                      1338
#define MNU_SIMQ                      1339
#define MNU_M_EDIT                    1340
#define MNU_MyMenu                    1341
#define MNU_MyAlpha                   1342
#define MNU_CONVM                     1343
#define MNU_ORTHOG                    1344
#define MNU_PARTS                     1345
#define MNU_PROB                      1346
#define MNU_PROGS                     1347
#define MNU_PFN                       1348
#define MNU_PFN2                      1349
#define MNU_CONVP                     1350
#define MNU_RAM                       1351
#define MNU_REALS                     1352
#define MNU_Solver                    1353
#define MNU_STAT                      1354
#define MNU_STK                       1355
#define MNU_STRING                    1356
#define MNU_TEST                      1357
#define MNU_TIMES                     1358
#define MNU_TRI                       1359
#define MNU_TVM                       1360
#define MNU_UNITCONV                  1361
#define MNU_VARS                      1362
#define MNU_CONVV                     1363
#define MNU_XFN                       1364
#define MNU_CONVX                     1365
#define MNU_ALPHAINTL                 1366
#define MNU_ALPHAMATH                 1367
#define MNU_ALPHAFN                   1368
#define MNU_ALPHA_OMEGA               1369
#define MNU_ALPHADOT                  1370
#define MNU_SYSFL                     1371
#define MNU_Sf                        1372
#define MNU_Sfdx                      1373
#define MNU_ANGLECONV                 1374
#define MNU_alpha_omega               1375
#define MNU_ALPHAintl                 1376
#define MNU_TAM                       1377
#define MNU_TAMCMP                    1378
#define MNU_TAMSTORCL                 1379
#define MNU_SUMS                      1380
#define MNU_VAR                       1381
#define MNU_TAMFLAG                   1382
#define MNU_TAMSHUFFLE                1383










#define ITM_1COMPL                    1394
#define ITM_SCRDMP                    1395
#define ITM_2COMPL                    1396
#define ITM_ABS                       1397
#define ITM_AGM                       1398
#define ITM_AGRAPH                    1399
#define ITM_ALL                       1400
#define ITM_ASSIGN                    1401
#define ITM_BACK                      1402
#define ITM_BATT                      1403
#define ITM_BEEP                      1404
#define ITM_BEGINP                    1405
#define ITM_BN                        1406
#define ITM_BNS                       1407
#define ITM_CASE                      1408
#define ITM_CLALL                     1409
#define ITM_CLCVAR                    1410
#define ITM_CLFALL                    1411
#define ITM_TGLFRT                    1412
#define ITM_CLLCD                     1413
#define ITM_CLMENU                    1414
#define ITM_CLP                       1415
#define ITM_CLPALL                    1416
#define ITM_CLREGS                    1417
#define ITM_CLSTK                     1418
#define ITM_CLSIGMA                   1419
#define ITM_COMB                      1420
#define ITM_CONJ                      1421
#define ITM_CONVG                     1422
#define ITM_CORR                      1423
#define ITM_COV                       1424
#define ITM_BESTFQ                    1425
#define ITM_CROSS                     1426
#define ITM_CXtoRE                    1427
#define ITM_DATE                      1428
#define ITM_DATEto                    1429
#define ITM_DAY                       1430
#define ITM_DBLR                      1431
#define ITM_DBLCROSS                  1432
#define ITM_DBLSLASH                  1433
#define ITM_DECOMP                    1434
#define ITM_DEG                       1435
#define ITM_DEGto                     1436
#define ITM_SA                        1437
#define ITM_DENMAX                    1438
#define ITM_DOT                       1439
#define ITM_DSTACK                    1440
#define ITM_DMS                       1441
#define ITM_DMSto                     1442
#define ITM_DMY                       1443
#define ITM_DtoJ                      1444
#define ITM_DtoR                      1445
#define ITM_EIGVAL                    1446
#define ITM_EIGVEC                    1447
#define ITM_END                       1448
#define ITM_ENDP                      1449
#define ITM_ENG                       1450

#define ITM_ENORM                     1452
#define ITM_ENTRY                     1453
#define ITM_EQ_DEL                    1454
#define ITM_EQ_EDI                    1455
#define ITM_EQ_NEW                    1456
#define ITM_ERF                       1457
#define ITM_ERFC                      1458
#define ITM_ERR                       1459
#define ITM_EXITALL                   1460
#define ITM_EXPT                      1461
#define ITM_FIB                       1462
#define ITM_FIX                       1463
#define ITM_FLASH                     1464
#define ITM_FQX                       1465
#define ITM_FDQX                      1466
#define ITM_GAP                       1467
#define ITM_GD                        1468
#define ITM_GDM1                      1469
#define ITM_GRAD                      1470
#define ITM_GRADto                    1471
#define ITM_GTOP                      1472
#define ITM_HN                        1473
#define ITM_HNP                       1474
#define ITM_IM                        1475
#define ITM_INDEX                     1476
#define ITM_IXYZ                      1477
#define ITM_IGAMMAP                   1478

#define ITM_IGAMMAQ                   1479
#define ITM_IPLUS                     1480
#define ITM_IMINUS                    1481
#define ITM_JYX                       1482
#define ITM_JPLUS                     1483
#define ITM_JMINUS                    1484
#define ITM_JonG                      1485
#define ITM_JtoD                      1486
#define ITM_KEY                       1487
#define ITM_KEYG                      1488
#define ITM_KEYX                      1489
#define ITM_KEYQ                      1490
#define ITM_KTYP                      1491
#define ITM_LASTX                     1492
#define ITM_LBLQ                      1493
#define ITM_LEAP                      1494
#define ITM_Ln                        1495
#define ITM_LNALPHA                   1496
#define ITM_LNBETA                    1497
#define ITM_LNGAMMA                   1498
#define ITM_LOAD                      1499
#define ITM_LOADP                     1500
#define ITM_LOADR                     1501
#define ITM_LOADSS                    1502
#define ITM_LOADSIGMA                 1503
#define ITM_LocR                      1504
#define ITM_LocRQ                     1505
#define ITM_LR                        1506
#define ITM_MANT                      1507
#define ITM_MATX                      1508
#define ITM_MEM                       1509
#define ITM_MENU                      1510
#define ITM_MONTH                     1511
#define ITM_MSG                       1512
#define ITM_MULPI                     1513
#define ITM_MVAR                      1514
#define ITM_M_DELR                    1515
#define ITM_M_DIM                     1516
#define ITM_M_DIMQ                    1517
#define ITM_MDY                       1518
#define ITM_M_EDI                     1519
#define ITM_M_EDIN                    1520
#define ITM_M_GET                     1521
#define ITM_M_GOTO                    1522
#define ITM_M_GROW                    1523
#define ITM_M_INSR                    1524
#define ITM_M_LU                      1525
#define ITM_M_NEW                     1526
#define ITM_M_OLD                     1527
#define ITM_M_PUT                     1528
#define ITM_M_RR                      1529
#define ITM_M_SQR                     1530
#define ITM_M_WRAP                    1531
#define ITM_NOP                       1532
#define ITM_OFF                       1533
#define ITM_PAUSE                     1534
#define ITM_PERM                      1535
#define ITM_PGMINT                    1536
#define ITM_PGMSLV                    1537
#define ITM_PIXEL                     1538
#define ITM_PLOT                      1539
#define ITM_PN                        1540
#define ITM_POINT                     1541
#define ITM_LOADV                     1542
#define ITM_POPLR                     1543
#define ITM_PRCL                      1544
#define ITM_PSTO                      1545
#define ITM_PUTK                      1546
#define ITM_RAD                       1547
#define ITM_RADto                     1548
#define ITM_RAN                       1549
#define ITM_RBR                       1550
#define ITM_RCLCFG                    1551
#define ITM_RCLEL                     1552
#define ITM_RCLIJ                     1553
#define ITM_RCLS                      1554
#define ITM_RDP                       1555
#define ITM_RE                        1556
#define ITM_RECV                      1557
#define ITM_RESET                     1558
#define ITM_REtoCX                    1559
#define ITM_REexIM                    1560
#define ITM_RM                        1561
#define ITM_RMQ                       1562
#define ITM_RMD                       1563
#define ITM_RNORM                     1564
#define ITM_ROUND                     1565
#define ITM_ROUNDI                    1566
#define ITM_RSD                       1567
#define ITM_RSUM                      1568
#define ITM_RTNP1                     1569
#define ITM_R_CLR                     1570
#define ITM_R_COPY                    1571
#define ITM_R_SORT                    1572
#define ITM_R_SWAP                    1573
#define ITM_RtoD                      1574
#define ITM_S                         1575
#define ITM_SAVE                      1576
#define ITM_SCI                       1577
#define ITM_SDIGS                     1578
#define ITM_SEED                      1579
#define ITM_SEND                      1580
#define ITM_SETCHN                    1581
#define ITM_SETDAT                    1582
#define ITM_SETEUR                    1583
#define ITM_SETIND                    1584
#define ITM_SETJPN                    1585
#define ITM_SETSIG                    1586
#define ITM_SETTIM                    1587
#define ITM_SETUK                     1588
#define ITM_SETUSA                    1589
#define ITM_SIGN                      1590
#define ITM_SIGNMT                    1591
#define ITM_SIM_EQ                    1592
#define ITM_SKIP                      1593
#define ITM_SLVQ                      1594
#define ITM_SM                        1595
#define ITM_SMODE                     1596
#define ITM_SMW                       1597
#define ITM_SOLVE                     1598
#define ITM_SSIZE                     1599
#define ITM_STATUS                    1600
#define ITM_STOCFG                    1601
#define ITM_STOEL                     1602
#define ITM_STOIJ                     1603
#define ITM_STOP                      1604
#define ITM_STOS                      1605
#define ITM_SUM                       1606
#define ITM_SW                        1607
#define ITM_SXY                       1608
#define ITM_TDISP                     1609
#define ITM_TICKS                     1610
#define ITM_TIME                      1611
#define ITM_TIMER                     1612
#define ITM_TN                        1613
#define ITM_TONE                      1614
#define ITM_Tex                       1615
#define ITM_ULP                       1616
#define ITM_UN                        1617
#define ITM_UNITV                     1618
#define ITM_UNSIGN                    1619
#define ITM_VARMNU                    1620
#define ITM_VERS                      1621
#define ITM_VIEW                      1622
#define ITM_WDAY                      1623
#define ITM_WHO                       1624
#define ITM_WM                        1625
#define ITM_WP                        1626
#define ITM_WM1                       1627
#define ITM_WSIZE                     1628
#define ITM_WSIZEQ                    1629
#define ITM_XBAR                      1630
#define ITM_XG                        1631
#define ITM_XW                        1632
#define ITM_XCIRC                     1633
#define ITM_XtoDATE                   1634
#define ITM_XtoALPHA                  1635
#define ITM_Xex                       1636
#define ITM_YEAR                      1637
#define ITM_YCIRC                     1638
#define ITM_YMD                       1639
#define ITM_Yex                       1640
#define ITM_Zex                       1641
#define ITM_ALPHALENG                 1642
#define ITM_XMAX                      1643
#define ITM_XMIN                      1644
#define ITM_ALPHAPOS                  1645
#define ITM_ALPHARL                   1646
#define ITM_ALPHARR                   1647
#define ITM_ALPHASL                   1648
#define ITM_ALPHASR                   1649
#define ITM_ALPHAtoX                  1650
#define ITM_BETAXY                    1651
#define ITM_gammaXY                   1652
#define ITM_GAMMAXY                   1653
#define ITM_GAMMAX                    1654
#define ITM_deltaX                    1655
#define ITM_DELTAPC                   1656
#define ITM_epsilon                   1657
#define ITM_epsilonM                  1658
#define ITM_epsilonP                  1659
#define ITM_zetaX                     1660
#define ITM_PI                        1661
#define ITM_SIGMA                     1662
#define ITM_sigma                     1663
#define ITM_sigmaw                    1664
#define ITM_RANI                      1665
#define ITM_PRINTERX                  1666
#define ITM_RANGE                     1667
#define ITM_GETRANGE                  1668
#define ITM_M1X                       1669
#define ITM_XMOD                      1670
#define ITM_toDATE                    1671
#define ITM_toDEG                     1672
#define ITM_toDMS                     1673


#define ITM_toGRAD                    1674
#define ITM_toHR                      1675
#define ITM_toHMS                     1676
#define ITM_toINT                     1677   // JM #

#define ITM_toMULpi                   1678
#define ITM_toPOL                     1679
#define ITM_toRAD                     1680
#define ITM_toREAL                    1681
#define ITM_toREC                     1682
#define ITM_DtoDMS                    1683
#define ITM_ex                        1684
#define ITM_PC                        1685
#define ITM_PCMRR                     1686
#define ITM_PCT                       1687
#define ITM_PCSIGMA                   1688
#define ITM_PCPMG                     1689
#define ITM_INTEGRAL                  1690
#define ITM_PMOD                      1691
#define ITM_M_DET                     1692
#define ITM_PARALLEL                  1693
#define ITM_M_TRANSP                  1694
#define ITM_M_INV                     1695
#define ITM_ANGLE                     1696
#define ITM_MULPIto                   1697
#define ITM_PRINTERADV                1698
#define ITM_PRINTERCHAR               1699
#define ITM_PRINTERDLAY               1700
#define ITM_PRINTERLCD                1701
#define ITM_PRINTERMODE               1702
#define ITM_PRINTERPROG               1703
#define ITM_PRINTERR                  1704

#define ITM_PRINTERSTK                1706
#define ITM_PRINTERTAB                1707
#define ITM_PRINTERUSER               1708
#define ITM_PRINTERWIDTH              1709

#define ITM_PRINTERHASH               1711
#define ITM_FBR                       1712
#define KEY_UNDO                      1713
#define ITM_PR                        1714
#define ITM_RS                        1715
#define ITM_Not                       1716
#define ITM_yet                       1717
#define ITM_defined                   1718
#define KEY_USERMODE                  1719
#define KEY_CC                        1720
#define KEY_f                         1721
#define KEY_g                         1722
#define KEY_UP                        1723
#define KEY_BST                       1724
#define KEY_DOWN                      1725
#define KEY_SST                       1726
#define KEY_EXIT                      1727
#define KEY_BACKSPACE                 1728
#define KEY_PRTX                      1729
#define ITM_AIM                       1730
#define KEY_dotD                      1731
#define ITM_SHOW                      1732
#define ITM_SYSTEM                    1733
#define ITM_DMStoD                    1734
#define ITM_VANGLE                    1735
#define ITM_XH                        1736
#define ITM_XRMS                      1737
#define ITM_ACOS                      1738
#define ITM_ASIN                      1739
#define ITM_ATAN                      1740
#define ITM_DET                       1741
#define ITM_INVRT                     1742
#define ITM_TRANS                     1743
#define ITM_XIN                       1744
#define ITM_XOUT                      1745

































































































































































































































































#define ITM_SYS_FREE_RAM              2000   //dr
#define ITM_ERPN                      2001   //JM eRPN
#define ITM_HOMEx3                    2002   //JM HOME.3
#define ITM_SHTIM                     2003   //JM SHIFT CANCEL
#define MNU_HOME                      2004   //JM HOME
#define ITM_SIGFIG                    2005   //JM SIGFIG
#define MNU_ALPHA                     2006   //JM ALPHA
#define MNU_BASE                      2007   //JM BASE
#define ITM_2BIN                      2008   //JM BASE
#define ITM_2OCT                      2009   //JM BASE
#define ITM_2DEC                      2010   //JM BASE
#define ITM_2HEX                      2011   //JM BASE
#define ITM_WS8                       2012   //JM BASE
#define ITM_WS16                      2013   //JM BASE
#define ITM_WS32                      2014   //JM BASE
#define ITM_WS64                      2015   //JM BASE
#define ITM_UNIT                      2016   //JM UNIT
#define ITM_SH_ERPN                   2017   //JM SHOW
#define ITM_CB_CPXRES                 2018   //JM cb ComplexResult
#define ITM_CB_LEADING_ZERO           2019   //JM cb LeadingZeros
#define CHR_QOPPA                     2020   //JM GREEK   //NOTE the RANGE STARTS HERE, with +36 for lower case
#define CHR_DIGAMMA                   2021   //JM GREEK
#define CHR_SAMPI                     2022   //JM GREEK   //NOTE the RANGE STOPS HERE, with +36 for lower case
#define CHR_1599                      2023   //JM SPARE
#define CHR_1600                      2024   //JM SPARE
#define CHR_1601                      2025   //JM SPARE
#define CHR_1602                      2026   //JM SPARE
#define CHR_1603                      2027   //JM SPARE
#define CHR_1604                      2028   //JM SPARE
#define CHR_1605                      2029   //JM SPARE
#define CHR_1606                      2030   //JM SPARE
#define CHR_1607                      2031   //JM SPARE
#define CHR_1608                      2032   //JM SPARE
#define CHR_1609                      2033   //JM SPARE
#define CHR_1610                      2034   //JM SPARE
#define CHR_1611                      2035   //JM SPARE
#define CHR_1612                      2036   //JM SPARE
#define CHR_1613                      2037   //JM SPARE
#define CHR_1614                      2038   //JM SPARE
#define CHR_1615                      2039   //JM SPARE
#define CHR_1616                      2040   //JM SPARE
#define CHR_1617                      2041   //JM SPARE
#define CHR_1618                      2042   //JM SPARE
#define CHR_1619                      2043   //JM SPARE
#define CHR_1620                      2044   //JM SPARE
#define CHR_1621                      2045   //JM SPARE
#define CHR_1622                      2046   //JM SPARE
#define CHR_1623                      2047   //JM SPARE
#define CHR_1624                      2048   //JM SPARE
#define CHR_1625                      2049   //JM SPARE
#define CHR_1626                      2050   //JM SPARE
#define CHR_1627                      2051   //JM SPARE
#define CHR_1628                      2052   //JM SPARE
#define CHR_1629                      2053   //JM SPARE
#define CHR_1630                      2054   //JM SPARE
#define CHR_1631                      2055   //JM SPARE
#define CHR_qoppa                     2056   //JM GREEK
#define CHR_digamma                   2057   //JM GREEK
#define CHR_sampi                     2058   //JM GREEK
#define CHR_case                      2059   //JM CAPS
#define Not_used1737                  2060
#define MNU_XEQ                       2061   //JM EXEC
#define ITM_op_a                      2062   //JM OPERATORS
#define ITM_op_a2                     2063   //JM OPERATORS
#define ITM_op_j                      2064   //JM OPERATORS
#define ITM_BASE_HOME                 2065   //JM BASEMENU
#define ITM_PGMTST                    2066   //JM Generic program test
#define ITM_BASE_AHOME                2067   //JM BASEMENU
#define ITM_H_SUMRY                   2068   //JMHOME
#define ITM_H_REPLCA                  2069   //JMHOME
#define ITM_H_FIXED                   2070   //JMHOME
#define ITM_CB_SPCRES                 2071
#define ITM_USER_V43                  2072   //V43
#define ITM_EE_D2Y                    2073   //JM EE
#define ITM_EE_Y2D                    2074   //JM EE
#define ITM_EE_A2S                    2075   //JM EE
#define ITM_EE_S2A                    2076   //JM EE
#define MNU_EE                        2077   //JM EE
#define ITM_EE_EXP_TH                 2078   //JM EE
#define ITM_EE_STO_Z                  2079   //JM EE
#define ITM_EE_RCL_Z                  2080   //JM EE
#define ITM_EE_STO_V                  2081   //JM EE
#define ITM_EE_RCL_V                  2082   //JM EE
#define ITM_EE_STO_I                  2083   //JM EE
#define ITM_EE_RCL_I                  2084   //JM EE
#define ITM_EE_STO_V_I                2085   //JM EE
#define ITM_EE_STO_IR                 2086   //JM EE
#define ITM_EE_STO_V_Z                2087   //JM EE
#define ITM_EE_X2BAL                  2088   //JM EE
#define KEY_COMPLEX                   2089   //JM CPX
#define ITM_RNG                       2090   //JM RANGE
#define Not_used1768                  2091
#define Not_used1769                  2092
#define ITM_HOMEx3T                   2093   //JM HOME.3T
#define STAT_DEMO                     2094   //LOAD STATS GRAPH
#define MNU_ASN                       2095   //JM USER
#define K_00U                         2096   //JM USER MODE TEST
#define Kf00U                         2097   //JM USER MODE TEST
#define Kg00U                         2098   //JM USER MODE TEST
#define K_01U                         2099   //JM USER MODE TEST
#define Kf01U                         2100   //JM USER MODE TEST
#define Kg01U                         2101   //JM USER MODE TEST
#define K_02U                         2102   //JM USER MODE TEST
#define Kf02U                         2103   //JM USER MODE TEST
#define Kg02U                         2104   //JM USER MODE TEST
#define K_03U                         2105   //JM USER MODE TEST
#define Kf03U                         2106   //JM USER MODE TEST
#define Kg03U                         2107   //JM USER MODE TEST
#define K_04U                         2108   //JM USER MODE TEST
#define Kf04U                         2109   //JM USER MODE TEST
#define Kg04U                         2110   //JM USER MODE TEST
#define K_05U                         2111   //JM USER MODE TEST
#define Kf05U                         2112   //JM USER MODE TEST
#define Kg05U                         2113   //JM USER MODE TEST
#define K_06U                         2114   //JM USER MODE TEST
#define Kf06U                         2115   //JM USER MODE TEST
#define Kg06U                         2116   //JM USER MODE TEST
#define K_07U                         2117   //JM USER MODE TEST
#define Kf07U                         2118   //JM USER MODE TEST
#define Kg07U                         2119   //JM USER MODE TEST
#define K_08U                         2120   //JM USER MODE TEST
#define Kf08U                         2121   //JM USER MODE TEST
#define Kg08U                         2122   //JM USER MODE TEST
#define K_09U                         2123   //JM USER MODE TEST
#define Kf09U                         2124   //JM USER MODE TEST
#define Kg09U                         2125   //JM USER MODE TEST
#define K_10U                         2126   //JM USER MODE TEST
#define Kf10U                         2127   //JM USER MODE TEST
#define Kg10U                         2128   //JM USER MODE TEST
#define K_11U                         2129   //JM USER MODE TEST
#define Kf11U                         2130   //JM USER MODE TEST
#define Kg11U                         2131   //JM USER MODE TEST
#define K_12U                         2132   //JM USER MODE TEST
#define Kf12U                         2133   //JM USER MODE TEST
#define Kg12U                         2134   //JM USER MODE TEST
#define K_13U                         2135   //JM USER MODE TEST
#define Kf13U                         2136   //JM USER MODE TEST
#define Kg13U                         2137   //JM USER MODE TEST
#define K_14U                         2138   //JM USER MODE TEST
#define Kf14U                         2139   //JM USER MODE TEST
#define Kg14U                         2140   //JM USER MODE TEST
#define K_15U                         2141   //JM USER MODE TEST
#define Kf15U                         2142   //JM USER MODE TEST
#define Kg15U                         2143   //JM USER MODE TEST
#define K_16U                         2144   //JM USER MODE TEST
#define Kf16U                         2145   //JM USER MODE TEST
#define Kg16U                         2146   //JM USER MODE TEST
#define K_17U                         2147   //JM USER MODE TEST
#define Kf17U                         2148   //JM USER MODE TEST
#define Kg17U                         2149   //JM USER MODE TEST
#define K_18U                         2150   //JM USER MODE TEST
#define Kf18U                         2151   //JM USER MODE TEST
#define Kg18U                         2152   //JM USER MODE TEST
#define K_19U                         2153   //JM USER MODE TEST
#define Kf19U                         2154   //JM USER MODE TEST
#define Kg19U                         2155   //JM USER MODE TEST
#define K_20U                         2156   //JM USER MODE TEST
#define Kf20U                         2157   //JM USER MODE TEST
#define Kg20U                         2158   //JM USER MODE TEST
#define K_21U                         2159   //JM USER MODE TEST
#define Kf21U                         2160   //JM USER MODE TEST
#define Kg21U                         2161   //JM USER MODE TEST
#define K_22U                         2162   //JM USER MODE TEST
#define Kf22U                         2163   //JM USER MODE TEST
#define Kg22U                         2164   //JM USER MODE TEST
#define K_23U                         2165   //JM USER MODE TEST
#define Kf23U                         2166   //JM USER MODE TEST
#define Kg23U                         2167   //JM USER MODE TEST
#define K_24U                         2168   //JM USER MODE TEST
#define Kf24U                         2169   //JM USER MODE TEST
#define Kg24U                         2170   //JM USER MODE TEST
#define K_25U                         2171   //JM USER MODE TEST
#define Kf25U                         2172   //JM USER MODE TEST
#define Kg25U                         2173   //JM USER MODE TEST
#define K_26U                         2174   //JM USER MODE TEST
#define Kf26U                         2175   //JM USER MODE TEST
#define Kg26U                         2176   //JM USER MODE TEST
#define K_27U                         2177   //JM USER MODE TEST
#define Kf27U                         2178   //JM USER MODE TEST
#define Kg27U                         2179   //JM USER MODE TEST
#define K_28U                         2180   //JM USER MODE TEST
#define Kf28U                         2181   //JM USER MODE TEST
#define Kg28U                         2182   //JM USER MODE TEST
#define K_29U                         2183   //JM USER MODE TEST
#define Kf29U                         2184   //JM USER MODE TEST
#define Kg29U                         2185   //JM USER MODE TEST
#define K_30U                         2186   //JM USER MODE TEST
#define Kf30U                         2187   //JM USER MODE TEST
#define Kg30U                         2188   //JM USER MODE TEST
#define K_31U                         2189   //JM USER MODE TEST
#define Kf31U                         2190   //JM USER MODE TEST
#define Kg31U                         2191   //JM USER MODE TEST
#define K_32U                         2192   //JM USER MODE TEST
#define Kf32U                         2193   //JM USER MODE TEST
#define Kg32U                         2194   //JM USER MODE TEST
#define K_33U                         2195   //JM USER MODE TEST
#define Kf33U                         2196   //JM USER MODE TEST
#define Kg33U                         2197   //JM USER MODE TEST
#define K_34U                         2198   //JM USER MODE TEST
#define Kf34U                         2199   //JM USER MODE TEST
#define Kg34U                         2200   //JM USER MODE TEST
#define K_35U                         2201   //JM USER MODE TEST
#define Kf35U                         2202   //JM USER MODE TEST
#define Kg35U                         2203   //JM USER MODE TEST
#define K_36U                         2204   //JM USER MODE TEST
#define Kf36U                         2205   //JM USER MODE TEST
#define Kg36U                         2206   //JM USER MODE TEST
#define KEY_fg                        2207
#define ITM_USER_DEFAULTS             2208   //JM USER
#define ITM_USER_COMPLEX              2209   //JM USER
#define ITM_USER_SHIFTS               2210   //JM USER
#define ITM_USER_RESET                2211   //JM USER
#define ITM_U_KEY_USER                2212   //JM USER
#define MNU_INL_TST                   2213   //INLINE_TEST
#define ITM_U_KEY_CC                  2214   //JM USER





























































































































#define ITM_TEST                      2215   //INLINE_TEST
#define ITM_U_KEY_MM                  2216   //JM USER
#define ITM_GET_TEST_BS               2217   //INLINE_TEST
#define ITM_U_KEY_SIGMA               2218   //JM USER
#define ITM_SET_TEST_BS               2219   //INLINE_TEST
#define ITM_U_KEY_PRGM                2220   //JM USER
#define ITM_RI                        2221   //JM ROUND
#define ITM_U_KEY_ALPHA               2222   //JM USER
#define ITM_SH_NORM_E                 2223   //JM USER NORMAL
#define ITM_JM_ASN                    2224   //JM ASSIGN
#define ITM_JM_SEEK                   2225   //JM ASSIGN
#define MNU_ASN_N                     2226   //JM USER NORMAL
#define ITM_GET_NORM_E                2227   //JM USER NORMAL
#define ITM_INP_DEF_43S               2228   //JM INPUT DEFAULT
#define ITM_FG_LINE                   2229   //JM FG              **
#define ITM_INP_DEF_DP                2230   //JM INPUT DEFAULT
#define ITM_SH_INP_DEF                2231   //JM INPUT DEFAULT
#define ITM_FG_DOTS                   2232   //JM FG              **
#define ITM_INP_DEF_CPXDP             2233   //JM INPUT DEFAULT
#define ITM_G_DOUBLETAP               2234   //JM FG              **
#define ITM_INP_DEF_SI                2235   //JM INPUT DEFAULT
#define ITM_INP_DEF_LI                2236   //JM INPUT DEFAULT
#define ITM_GRAPH                     2237   //JM GRAPHING
#define MNU_A_Z                       2238   //  34
#define MNU_a_z                       2239   //  875
#define MNU_GRAPH                     2240   //JM GRAPH
#define ITM_GRF_X0                    2241   //JM GRAPH
#define ITM_GRF_X1                    2242   //JM GRAPH
#define ITM_GRF_Y0                    2243   //JM GRAPH
#define ITM_GRF_Y1                    2244   //JM GRAPH
#define ITM_GRF_DX                    2245   //JM GRAPH
#define ITM_GRF_DY                    2246   //JM GRAPH
#define ITM_GRF_HLP                   2247   //JM GRAPH
#define ITM_LI                        2248
#define ITM_ms                        2249   //JM DMS HMS
#define ITM_SI_p                      2250   //JM PRE UNIT
#define ITM_SI_n                      2251   //JM PRE UNIT
#define ITM_SI_u                      2252   //JM PRE UNIT
#define ITM_SI_m                      2253   //JM PRE UNIT
#define ITM_SI_k                      2254   //JM PRE UNIT
#define ITM_SI_M                      2255   //JM PRE UNIT
#define ITM_USER_ALPHA                2256   //JM USER
#define ITM_USER_GSHFT                2257   //JM USER
#define ITM_HASH_JM                   2258
#define ITM_DEMO1                     2259   //JM USER
#define ITM_DEMO2                     2260   //JM USER
#define ITM_DEMO3                     2261   //JM USER
#define ITM_DEMO4                     2262   //JM USER
#define ITM_DEMO5                     2263   //JM USER
#define ITM_DEMO6                     2264   //JM USER
#define MNU_ST_GRAPH                  2265   //JM USER
#define ITM_VECT                      2266   //JMVECT
#define ITM_LARGELI                   2267
#define ITM_P_ALLREGS                 2268   //JM Print all regs
#define ITM_SI_f                      2269   //JM PRE UNIT
#define ITM_SI_G                      2270   //JM PRE UNIT
#define ITM_SI_T                      2271   //JM PRE UNIT
#define ITM_USER_CC                   2272   //JM USER
#define ITM_USER_MYM                  2273   //JM USER
#define ITM_USER_PRGM                 2274   //JM USER
#define ITM_USER_USER                 2275   //JM USER
#define ITM_USER_SIGMAPLUS            2276   //JM USER
#define ITM_LISTXY                    2277   //JMSTAT
#define ITM_toPOL2                    2278   //JM
#define ITM_toREC2                    2279   //JM
#define ITM_CFG                       2280   //JM Replacements
#define ITM_CLK12                     2281   //JM Replacements
#define ITM_CLK24                     2282   //JM Replacements
#define ITM_CPXI                      2283
#define ITM_CPXJ                      2284
#define ITM_MULTCR                    2285
#define ITM_MULTDOT                   2286
#define ITM_SSIZE4                    2287
#define ITM_POLAR                     2288   //JM Replacements
#define ITM_RDXCOM                    2289   //JM Replacements
#define ITM_RDXPER                    2290   //JM Replacements
#define ITM_SSIZE8                    2291
#define ITM_RECT                      2292   //JM Replacements
#define ITM_FLGSV                     2293
#define ITM_SCIOVR                    2294   //JM Replacements
#define ITM_ENGOVR                    2295   //JM Replacements
#define ITM_USER_V43MIN               2296   //V43
#define ITM_USER_SHIFTS2              2297   //V43
#define ITM_USER_HOME                 2298   //JM USER
#define ITM_USER_WP43S                2299   //V43
#define ITM_USER_DM42                 2300   //V43
#define MNU_ASN_U                     2301   //V43
#define ITM_X_P1                      2302   //JM EXEC
#define ITM_X_P2                      2303   //JM EXEC
#define ITM_X_P3                      2304   //JM EXEC
#define ITM_X_P4                      2305   //JM EXEC
#define ITM_X_P5                      2306   //JM EXEC
#define ITM_X_P6                      2307   //JM EXEC
#define ITM_X_f1                      2308   //JM EXEC
#define ITM_X_f2                      2309   //JM EXEC
#define ITM_X_f3                      2310   //JM EXEC
#define ITM_X_f4                      2311   //JM EXEC
#define ITM_X_f5                      2312   //JM EXEC
#define ITM_X_f6                      2313   //JM EXEC
#define ITM_X_g1                      2314   //JM EXEC
#define ITM_X_g2                      2315   //JM EXEC
#define ITM_X_g3                      2316   //JM EXEC
#define ITM_X_g4                      2317   //JM EXEC
#define ITM_X_g5                      2318   //JM EXEC
#define ITM_X_g6                      2319   //JM EXEC
#define ITM_ROUND2                    2320
#define ITM_ROUNDI2                   2321
#define ITM_USER_C43                  2322
#define ITM_eRPN_ON                   2323
#define ITM_eRPN_OFF                  2324
#define ITM_T_LEFT_ARROW              2325
#define ITM_T_RIGHT_ARROW             2326
#define ITM_T_LLEFT_ARROW             2327
#define ITM_T_RRIGHT_ARROW            2328
#define ITM_DMPMNU                    2329
#define ITM_XNEW                      2330
#define MNU_T_EDIT                    2331
#define ITM_XSAVE                     2332
#define ITM_XLOAD                     2333
#define ITM_XEDIT                     2334
#define ITM_XXEQ                      2335
#define MNU_XXEQ                      2336
#define ITM_EXTX                      2337
#define ITM_EXTY                      2338
#define ITM_DEG2                      2339
#define ITM_DMS2                      2340
#define ITM_GRAD2                     2341
#define ITM_MULPI2                    2342
#define ITM_RAD2                      2343
#define ITM_HMS2                      2344


































#define LAST_ITEM                     2345   //JM

#define CHR_PROD_SIGN                 9999

/********************************************//**
 * \typedef item_t
 * \brief Structure keeping the information for one item
 ***********************************************/
#define CAT_NONE  0
#define CAT_FNCT  1 // Function
#define CAT_MENU  2 // Menu
#define CAT_CNST  3 // Constant
#define CAT_FREE  4 // To identify and find the free items
#define CAT_REGS  5 // Registers
#define CAT_RVAR  6 // Reserved variable
#define CAT_DUPL  7 // Duplicate of another item e.g. acus->m^2
#define CAT_SYFL  8 // System flags
#define CAT_AINT  9 // Upper case alpha_INTL
#define CAT_aint 10 // Lower case alpha_intl

typedef struct {
  void     (*func)(uint16_t); ///< Function called to execute the item
  uint16_t param;             ///< 1st parameter to the above function
  char     *itemCatalogName;  ///< Name of the item in the catalogs
  char     *itemSoftmenuName; ///< Representation of the item in the menus or on the keyboard
  uint16_t tamMin;            ///< Minimul value for TAM argument
  uint16_t tamMax;            ///< Maximal value for TAM argument
  char     catalog;           ///< Menu of CATALOG in which the item is located: see #define CAT_*
  uint8_t  stackLiftStatus;   ///< Stack lift status after item execution.
  uint8_t  undoStatus;        ///< Undo status after item execution.
} item_t;

void reallyRunFunction(int16_t func, uint16_t param);
void runFunction      (int16_t func);
void fnToBeCoded      (void);
void itemToBeCoded    (uint16_t unusedParamButMandatory);
void fnNop            (uint16_t unusedParamButMandatory);
