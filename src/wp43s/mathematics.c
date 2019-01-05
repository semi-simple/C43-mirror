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
 * \file mathematics.c
 ***********************************************/

#include "wp43s.h"

const real34_t * const thresholdsDegrees[]  = {
  (real34_t *)"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x28\x00\x32", //  45
  (real34_t *)"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x6e", //  90
  (real34_t *)"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xc0\x40\x00\x26", // 180
  (real34_t *)"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x70\x00\x2e"  // 360
};
const real34_t * const thresholdsGradians[] = {
  (real34_t *)"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x36", //  50
  (real34_t *)"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x40\x00\x26", // 100
  (real34_t *)"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x40\x00\x2a", // 200
  (real34_t *)"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x40\x00\x32"  // 400
};
const real34_t * const thresholdsRadians[]  = {
  (real34_t *)"\xd7\x7b\x83\x25\xc2\xd6\xe8\xb1\x22\xbf\x33\x3f\xd2\x95\xff\x3d", // pi/4
  (real34_t *)"\xd1\xe7\xbc\x71\x68\x31\x65\xec\xb1\xf6\xa6\xe9\x0f\xef\xff\x25", // pi/2
  (real34_t *)"\x83\xe6\xb5\xda\xd0\x62\xe2\xb4\xfb\xb3\x53\xeb\x1a\xcc\xff\x2d", // pi
  (real34_t *)"\x06\x64\x6b\xbe\x5a\xad\xda\xa9\x6e\x3e\x87\x2d\xb3\xd2\xff\x39"  // 2pi
};
const real34_t * const thresholdsMultPi[]   = {
  (real34_t *)"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xa8\xff\x29", // 0.25
  (real34_t *)"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x80\xff\x35", // 0.5
  (real34_t *)"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xc0\xff\x25", // 1
  (real34_t *)"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xc0\xff\x29"  // 2
};
const real51_t * const gamma_consts[] = {
  (real51_t *)"\x33\x00\x00\x00\xd8\xff\xff\xff\x00\x00\x8a\x02\x96\x02\xba\x03\x5c\x03\x74\x00\xe7\x01\xb0\x03\x94\x00\x82\x02\x97\x00\xd4\x00\xe9\x01\x51\x02\x5d\x00\x8e\x00\x7a\x03\xbd\x00", // const_gammaC01 = +1.898901420935934892121516421489444871168609546626500000e+10
  (real51_t *)"\x33\x00\x00\x00\xd9\xff\xff\xff\x80\x00\x90\x01\x4e\x03\x29\x03\x7f\x03\x7f\x03\xab\x03\xca\x01\xa1\x01\x92\x01\x12\x00\xec\x00\x4c\x02\x17\x02\x5a\x00\xc8\x00\x9c\x00\x90\x00", // const_gammaC02 = -1.441562000905355882360184024174589398958958098464000000e+11
  (real51_t *)"\x33\x00\x00\x00\xd9\xff\xff\xff\x00\x00\xaa\x00\x2e\x00\x32\x01\xab\x00\x8f\x00\xe2\x00\x6e\x00\x77\x01\xb1\x03\xca\x01\xbc\x02\x2d\x03\x7e\x01\x01\x01\xc6\x01\x23\x00\xf0\x01", // const_gammaC03 = +4.960354542573828137004589453751102261431713060461700000e+11
  (real51_t *)"\x33\x00\x00\x00\xda\xff\xff\xff\x80\x00\x84\x03\x12\x03\x97\x02\xdb\x01\x5d\x03\x6c\x03\xf5\x01\x04\x03\xe1\x01\x6b\x01\x9c\x03\x41\x01\x4f\x03\x6b\x02\x28\x00\x7a\x01\x66\x00", // const_gammaC04 = -1.023780406198473219243634817725018768614756637869000000e+12
  (real51_t *)"\x33\x00\x00\x00\xda\xff\xff\xff\x00\x00\x64\x00\x98\x03\xa9\x02\x7e\x01\x14\x00\x37\x00\x3b\x00\xab\x01\x96\x01\x6d\x01\x6b\x01\x47\x01\x8b\x02\x81\x03\xd5\x02\x67\x01\x8d\x00", // const_gammaC05 = +1.413597258976513273633654064270590550203826819201000000e+12
  (real51_t *)"\x33\x00\x00\x00\xda\xff\xff\xff\x80\x00\x80\x02\x58\x02\xfa\x00\xba\x01\x6a\x02\x9a\x01\x54\x01\x4f\x03\xa0\x00\x50\x02\x19\x03\x47\x03\x23\x01\x14\x03\xe6\x02\x8a\x03\x89\x00", // const_gammaC06 = -1.379067427882918397935921608473404106184422506006400000e+12
  (real51_t *)"\x33\x00\x00\x00\xd9\xff\xff\xff\x00\x00\x3c\x00\x21\x03\x5a\x01\x39\x00\xfc\x01\xa0\x03\x6d\x00\x2a\x00\x4c\x00\x2b\x02\xce\x02\xa0\x02\x6d\x03\x3f\x00\xb5\x01\x34\x03\xd2\x03", // const_gammaC07 = +9.788204370638776727185550760421099285080573468010600000e+11
  (real51_t *)"\x33\x00\x00\x00\xd9\xff\xff\xff\x80\x00\x4e\x02\xa2\x01\x36\x03\x95\x02\x74\x02\x2a\x01\x73\x02\xd1\x03\x9f\x01\x75\x01\x3c\x01\x6f\x02\xad\x01\x5c\x00\xe4\x01\x83\x03\x00\x02", // const_gammaC08 = -5.128994840924296233163734159776272986286618224185900000e+11
  (real51_t *)"\x33\x00\x00\x00\xd9\xff\xff\xff\x00\x00\x0e\x01\xc5\x00\xf0\x01\x55\x01\x2b\x00\x17\x03\x4b\x00\xd3\x03\x9c\x02\x29\x02\x50\x00\x92\x01\xc3\x02\xc5\x01\xe9\x01\x41\x01\xc7\x00", // const_gammaC09 = +1.993214894537074020805536689790757910433414961972700000e+11
  (real51_t *)"\x33\x00\x00\x00\xd8\xff\xff\xff\x80\x00\xcc\x01\x4c\x02\x8a\x00\xf7\x01\x47\x01\x55\x00\x72\x03\x4a\x01\x22\x02\x92\x02\xcf\x01\xc1\x00\x1d\x01\x32\x00\xdc\x02\xbf\x01\x3c\x02", // const_gammaC10 = -5.724477320502851934636585463308820853275031385884600000e+10
  (real51_t *)"\x33\x00\x00\x00\xd8\xff\xff\xff\x00\x00\x5e\x01\x78\x01\x06\x00\x68\x02\x0c\x00\x62\x01\x34\x00\xb9\x02\xd9\x00\xd6\x01\xf1\x02\x2f\x03\xdb\x01\x7b\x02\x44\x02\xa5\x00\x78\x00", // const_gammaC11 = +1.201655806354758157534702176970523540126160063763500000e+10
  (real51_t *)"\x33\x00\x00\x00\xd7\xff\xff\xff\x80\x00\xf0\x00\x62\x00\x87\x03\x43\x01\xe4\x01\x63\x03\x55\x02\x0e\x01\xa5\x00\x68\x01\x65\x00\x43\x01\xf2\x02\xf7\x00\x12\x00\x85\x03\xb4\x00", // const_gammaC12 = -1.809010182477543231013601652705978674843239030982400000e+09
  (real51_t *)"\x33\x00\x00\x00\xd6\xff\xff\xff\x00\x00\xa2\x03\x10\x03\xc9\x01\xca\x01\xaa\x02\x16\x00\xae\x02\x6b\x02\x58\x02\xc6\x02\xa8\x01\xb1\x02\x82\x01\xc6\x00\xf2\x02\x56\x03\xbd\x00", // const_gammaC13 = +1.898547541983866894247106006196860226824584577849300000e+08
  (real51_t *)"\x33\x00\x00\x00\xd5\xff\xff\xff\x80\x00\xa2\x03\x51\x02\x6b\x00\xca\x02\xd7\x03\xde\x01\xad\x01\x5b\x02\x55\x01\xb4\x03\xae\x01\xef\x01\xec\x02\x7f\x00\x45\x01\xaa\x01\x85\x00", // const_gammaC14 = -1.334263251277484954309483416034294789837141075939300000e+07
  (real51_t *)"\x33\x00\x00\x00\xd3\xff\xff\xff\x00\x00\x3c\x00\xb4\x00\x85\x00\x1f\x02\x1f\x01\xc4\x03\x27\x02\x36\x02\xc8\x01\x38\x02\xdc\x01\xab\x00\x81\x00\x4e\x01\xa2\x03\x57\x01\x51\x02", // const_gammaC15 = +5.933439303341291714765684565665519642875431331800600000e+05
  (real51_t *)"\x33\x00\x00\x00\xd2\xff\xff\xff\x80\x00\x44\x02\x75\x03\xd9\x01\x23\x02\x71\x02\x64\x02\x7b\x00\x6e\x00\x43\x00\x6d\x03\x9b\x03\x0b\x02\xee\x01\x02\x00\xd8\x02\x20\x00\x9a\x00", // const_gammaC16 = -1.540327280024945239238770671101236126255474738855800000e+04
  (real51_t *)"\x33\x00\x00\x00\xd0\xff\xff\xff\x00\x00\xb2\x02\x9f\x00\xe7\x03\x03\x00\x49\x01\x40\x01\x3d\x01\xdd\x01\x87\x01\x4a\x01\x8e\x00\x9d\x01\x47\x03\x92\x01\xe2\x03\xc0\x01\xcf\x00", // const_gammaC17 = +2.074489944028394131423303914773173203290039991596900000e+02
  (real51_t *)"\x33\x00\x00\x00\xce\xff\xff\xff\x80\x00\xa2\x03\x0e\x00\xcf\x01\x0a\x02\xea\x02\xd0\x01\xde\x00\xab\x01\x1a\x02\xa5\x02\xea\x01\xda\x02\x4b\x01\x0f\x02\x4d\x03\xc2\x03\x78\x00", // const_gammaC18 = -1.209628455273317304906775384272224647465224630149300000e+00
  (real51_t *)"\x33\x00\x00\x00\xcb\xff\xff\xff\x00\x00\xb4\x00\x29\x02\xc2\x00\x68\x00\x16\x01\x84\x00\xc3\x02\xe9\x01\xfd\x02\x11\x01\x7c\x00\x99\x01\xdb\x00\xcd\x01\x75\x00\xc1\x03\xe2\x00", // const_gammaC19 = +2.269611174612194091242737654897071322781041945531800000e-03
  (real51_t *)"\x33\x00\x00\x00\xc7\xff\xff\xff\x80\x00\x0a\x00\x48\x03\xdc\x00\xa3\x00\x58\x00\xe9\x00\x08\x00\x8b\x03\x8b\x03\x48\x02\xae\x03\x6a\x02\x0e\x01\x72\x02\x4a\x02\x78\x03\x1e\x03", // const_gammaC20 = -7.988885866262706189425849079070082330881632208400100000e-07
  (real51_t *)"\x33\x00\x00\x00\xc3\xff\xff\xff\x00\x00\x8a\x02\x6a\x03\x24\x00\xc3\x03\xc7\x01\xb0\x00\x14\x00\x48\x02\xe3\x00\x00\x00\x6a\x00\x6e\x02\x48\x02\x07\x02\xba\x01\xde\x02\xa5\x00"  // const_gammaC21 = +1.657344425195846221060002275840201764559630368746500000e-11
};

/********************************************//**
 * \brief regX ==> regL and GDC(regY, regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void fnGcd(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnGcd");
  #endif

  bigInteger_t iOp1, iOp2;

  saveStack();

  if(getRegisterDataType(REGISTER_X) == dtSmallInteger && getRegisterDataType(REGISTER_Y) == dtSmallInteger) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    *(uint64_t *)(POINTER_TO_REGISTER_DATA(REGISTER_X)) = WP34S_intGCD(*(uint64_t *)(POINTER_TO_REGISTER_DATA(REGISTER_Y)), *(uint64_t *)(POINTER_TO_REGISTER_DATA(REGISTER_X)));
  }

  else if(getRegisterDataType(REGISTER_X) == dtBigInteger && getRegisterDataType(REGISTER_Y) == dtBigInteger) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    convertBigIntegerRegisterToBigInteger(REGISTER_Y, &iOp1);
    convertBigIntegerRegisterToBigInteger(REGISTER_X, &iOp2);
    bigIntegerGcd(&iOp1, &iOp2, &iOp1);

    convertBigIntegerToBigIntegerRegister(&iOp1, REGISTER_X);
  }

  else if(getRegisterDataType(REGISTER_X) == dtSmallInteger && getRegisterDataType(REGISTER_Y) == dtBigInteger) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    convertSmallIntegerRegisterBigIntegerRegister(REGISTER_X, REGISTER_X);

    convertBigIntegerRegisterToBigInteger(REGISTER_Y, &iOp1);
    convertBigIntegerRegisterToBigInteger(REGISTER_X, &iOp2);
    bigIntegerGcd(&iOp1, &iOp2, &iOp1);

    convertBigIntegerToBigIntegerRegister(&iOp1, REGISTER_X);
  }

  else if(getRegisterDataType(REGISTER_X) == dtBigInteger && getRegisterDataType(REGISTER_Y) == dtSmallInteger) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    convertSmallIntegerRegisterBigIntegerRegister(REGISTER_Y, REGISTER_Y);

    convertBigIntegerRegisterToBigInteger(REGISTER_Y, &iOp1);
    convertBigIntegerRegisterToBigInteger(REGISTER_X, &iOp2);
    bigIntegerGcd(&iOp1, &iOp2, &iOp1);

    convertBigIntegerToBigIntegerRegister(&iOp1, REGISTER_X);
  }

  else {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X); // Invalid input data type for this operation
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot get the GDC from %s and %s!", getDataTypeName(getRegisterDataType(REGISTER_Y), true, false), getDataTypeName(getRegisterDataType(REGISTER_X), true, false));
      showInfoDialog("In function fnGdc:", errorMessage, NULL, NULL);
    #endif
  }

  if(lastErrorCode == 0) {
    fnDropY(NOPARAM);
  }
  else {
    restoreStack();
  }

  refreshStack();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnGcd");
  #endif
}



/********************************************//**
 * \brief regX ==> regL and LCM(regY, regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void fnLcm(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnLcm");
  #endif

  bigInteger_t iOp1, iOp2;

  if(getRegisterDataType(REGISTER_X) == dtSmallInteger && getRegisterDataType(REGISTER_Y) == dtSmallInteger) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    *(uint64_t *)(POINTER_TO_REGISTER_DATA(REGISTER_X)) = WP34S_intLCM(*(uint64_t *)(POINTER_TO_REGISTER_DATA(REGISTER_Y)), *(uint64_t *)(POINTER_TO_REGISTER_DATA(REGISTER_X)));
  }

  else if(getRegisterDataType(REGISTER_X) == dtBigInteger && getRegisterDataType(REGISTER_Y) == dtBigInteger) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    convertBigIntegerRegisterToBigInteger(REGISTER_Y, &iOp1);
    convertBigIntegerRegisterToBigInteger(REGISTER_X, &iOp2);
    bigIntegerLcm(&iOp1, &iOp2, &iOp1);

    convertBigIntegerToBigIntegerRegister(&iOp1, REGISTER_X);
  }

  else if(getRegisterDataType(REGISTER_X) == dtSmallInteger && getRegisterDataType(REGISTER_Y) == dtBigInteger) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    convertSmallIntegerRegisterBigIntegerRegister(REGISTER_X, REGISTER_X);

    convertBigIntegerRegisterToBigInteger(REGISTER_Y, &iOp1);
    convertBigIntegerRegisterToBigInteger(REGISTER_X, &iOp2);
    bigIntegerLcm(&iOp1, &iOp2, &iOp1);

    convertBigIntegerToBigIntegerRegister(&iOp1, REGISTER_X);
  }

  else if(getRegisterDataType(REGISTER_X) == dtBigInteger && getRegisterDataType(REGISTER_Y) == dtSmallInteger) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    convertSmallIntegerRegisterBigIntegerRegister(REGISTER_Y, REGISTER_Y);

    convertBigIntegerRegisterToBigInteger(REGISTER_Y, &iOp1);
    convertBigIntegerRegisterToBigInteger(REGISTER_X, &iOp2);
    bigIntegerLcm(&iOp1, &iOp2, &iOp1);

    convertBigIntegerToBigIntegerRegister(&iOp1, REGISTER_X);
  }

  else {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X); // Invalid input data type for this operation
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot get the LCM from %s and %s!", getDataTypeName(getRegisterDataType(REGISTER_Y), true, false), getDataTypeName(getRegisterDataType(REGISTER_X), true, false));
      showInfoDialog("In function fnLcm:", errorMessage, NULL, NULL);
    #endif
  }

  refreshStack();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnLcm");
  #endif
}



/********************************************//**
 * \brief regX ==> regL and sqrt(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void fnSquareRoot(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnSquareRoot");
  #endif

  if(getRegisterDataType(REGISTER_X) == dtSmallInteger) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    *(uint64_t *)(POINTER_TO_REGISTER_DATA(REGISTER_X)) = WP34S_intSqrt(*(uint64_t *)(POINTER_TO_REGISTER_DATA(REGISTER_X)));
  }

  else if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);

    if(!real34IsNegative(POINTER_TO_REGISTER_DATA(REGISTER_X))) {
		    real34SquareRoot(POINTER_TO_REGISTER_DATA(REGISTER_X), POINTER_TO_REGISTER_DATA(REGISTER_X));
      convertRegister34To16(REGISTER_X);
    }
    else if(getFlag(FLAG_CPXRES)) {
      real34_t real34;

      real34Copy(POINTER_TO_REGISTER_DATA(REGISTER_X), &real34);
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, 0);
      real34SetPositiveSign(&real34);
		    real34SquareRoot(&real34, COMPLEX34_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)));
      real34Zero(POINTER_TO_REGISTER_DATA(REGISTER_X));
      convertRegister34To16(REGISTER_X);
    }
    else {
      displayCalcErrorMessage(1, REGISTER_T, REGISTER_X); // 1 = argument exceeds functions domain
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, STD_SQUARE_ROOT STD_x_UNDER_ROOT " doesn't work on a negative big integer when flag I is not set!");
        showInfoDialog("In function fnSquareRoot:", errorMessage, NULL, NULL);
      #endif
    }

    convertRegister34To16(REGISTER_X);
  }

  else if(getRegisterDataType(REGISTER_X) == dtReal16) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    convertRegister16To34(REGISTER_X);

    if(!real34IsNegative(POINTER_TO_REGISTER_DATA(REGISTER_X))) {
		    real34SquareRoot(POINTER_TO_REGISTER_DATA(REGISTER_X), POINTER_TO_REGISTER_DATA(REGISTER_X));
      convertRegister34To16(REGISTER_X);
    }
    else if(getFlag(FLAG_CPXRES)) {
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, 0);
      real34Copy(POINTER_TO_REGISTER_DATA(REGISTER_L), POINTER_TO_REGISTER_DATA(REGISTER_X));
      real34SetPositiveSign(POINTER_TO_REGISTER_DATA(REGISTER_X));

		    real34SquareRoot(POINTER_TO_REGISTER_DATA(REGISTER_X), COMPLEX34_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)));

      real34Zero(POINTER_TO_REGISTER_DATA(REGISTER_X));
      convertRegister34To16(REGISTER_X);
    }
    else {
      displayCalcErrorMessage(1, REGISTER_T, REGISTER_X); // 1 = argument exceeds functions domain
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, STD_SQUARE_ROOT STD_x_UNDER_ROOT " doesn't work on a negative real when flag I is not set!");
        showInfoDialog("In function fnSquareRoot:", errorMessage, NULL, NULL);
      #endif
    }
  }

  else if(getRegisterDataType(REGISTER_X) == dtReal34) {
    if(!real34IsNegative(POINTER_TO_REGISTER_DATA(REGISTER_X))) {
      copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
		    real34SquareRoot(POINTER_TO_REGISTER_DATA(REGISTER_X), POINTER_TO_REGISTER_DATA(REGISTER_X));
    }
    else if(getFlag(FLAG_CPXRES)) {
      copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, 0);
      real34Copy(POINTER_TO_REGISTER_DATA(REGISTER_L), POINTER_TO_REGISTER_DATA(REGISTER_X));
      real34SetPositiveSign(POINTER_TO_REGISTER_DATA(REGISTER_X));

		    real34SquareRoot(POINTER_TO_REGISTER_DATA(REGISTER_X), COMPLEX34_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)));

      real34Zero(POINTER_TO_REGISTER_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(1, REGISTER_T, REGISTER_X); // 1 = argument exceeds functions domain
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, STD_SQUARE_ROOT STD_x_UNDER_ROOT " doesn't work on a negative real when flag I is not set!");
        showInfoDialog("In function fnSquareRoot:", errorMessage, NULL, NULL);
      #endif
    }
  }

  else if(getRegisterDataType(REGISTER_X) == dtComplex16) {
    real16_t magnitude16, theta16;
    real34_t magnitude34;

    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    real16RectangularToPolar(REAL16_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), COMPLEX16_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), &magnitude16, &theta16);
    real16ToReal34(&magnitude16, &magnitude34);
    real34SquareRoot(&magnitude34, &magnitude34);
    real34ToReal16(&magnitude34, &magnitude16);
    real16Multiply(&theta16, const16_0_5, &theta16);
    convertAngle16FromTo(&theta16, angularMode, AM_RADIAN);
    real16PolarToRectangular(&magnitude16, &theta16, REAL16_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), COMPLEX16_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)));
  }

  else if(getRegisterDataType(REGISTER_X) == dtComplex34) {
    real34_t magnitude34, theta34;

    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    real34RectangularToPolar(REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), COMPLEX34_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), &magnitude34, &theta34);
    real34SquareRoot(&magnitude34, &magnitude34);
    real34Multiply(&theta34, const34_0_5, &theta34);
    convertAngle34FromTo(&theta34, angularMode, AM_RADIAN);
    real34PolarToRectangular(&magnitude34, &theta34, REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), COMPLEX34_IMAGINARY_PART_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)));
  }

  else {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X); // Invalid input data type for this operation
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot extract the square root of %s!", getDataTypeName(getRegisterDataType(REGISTER_X), true, false));
      showInfoDialog("In function fnSquareRoot:", errorMessage, NULL, NULL);
    #endif
  }

  refreshRegisterLine(REGISTER_X);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnSquareRoot");
  #endif
}



/********************************************//**
 * \brief Returns the absolute value of an integer or a real and the magnitude of a complex
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnMagnitude(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnMagnitude");
  #endif

  if(getRegisterDataType(REGISTER_X) == dtComplex16) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, 0);
    real16Multiply(POINTER_TO_REGISTER_DATA(REGISTER_L), POINTER_TO_REGISTER_DATA(REGISTER_L), POINTER_TO_REGISTER_DATA(REGISTER_X));
    real16FMA(POINTER_TO_REGISTER_DATA(REGISTER_L) + REAL16_SIZE, POINTER_TO_REGISTER_DATA(REGISTER_L) + REAL16_SIZE, POINTER_TO_REGISTER_DATA(REGISTER_X), POINTER_TO_REGISTER_DATA(REGISTER_X));
    real16SquareRoot(POINTER_TO_REGISTER_DATA(REGISTER_X), POINTER_TO_REGISTER_DATA(REGISTER_X));
  }

  else if(getRegisterDataType(REGISTER_X) == dtComplex34) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, 0);
    real34Multiply(POINTER_TO_REGISTER_DATA(REGISTER_L), POINTER_TO_REGISTER_DATA(REGISTER_L), POINTER_TO_REGISTER_DATA(REGISTER_X));
    real34FMA(POINTER_TO_REGISTER_DATA(REGISTER_L) + REAL34_SIZE, POINTER_TO_REGISTER_DATA(REGISTER_L) + REAL34_SIZE, POINTER_TO_REGISTER_DATA(REGISTER_X), POINTER_TO_REGISTER_DATA(REGISTER_X));
    real34SquareRoot(POINTER_TO_REGISTER_DATA(REGISTER_X), POINTER_TO_REGISTER_DATA(REGISTER_X));
  }

  else if(getRegisterDataType(REGISTER_X) == dtReal16) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    real16SetPositiveSign(POINTER_TO_REGISTER_DATA(REGISTER_X));
  }

  else if(getRegisterDataType(REGISTER_X) == dtReal34) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    real34SetPositiveSign(POINTER_TO_REGISTER_DATA(REGISTER_X));
  }

  else if(getRegisterDataType(REGISTER_X) == dtSmallInteger) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    *(uint64_t *)(POINTER_TO_REGISTER_DATA(REGISTER_X)) = WP34S_intAbs(*(uint64_t *)(POINTER_TO_REGISTER_DATA(REGISTER_X)));
  }

  else if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    setRegisterSign(REGISTER_X, 0);
  }

  else {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "|x| doesn't work on %s!", getRegisterDataTypeName(REGISTER_X, true, false));
      showInfoDialog("In function fnMagnitude:", errorMessage, NULL, NULL);
    #endif
  }

  refreshRegisterLine(REGISTER_X);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnMagnitude");
  #endif
}



/********************************************//**
 * \brief regX ==> regL and 2^regX ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void fn2Pow(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fn2Pow");
  #endif

  if(getRegisterDataType(REGISTER_X) == dtSmallInteger) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    *(uint64_t *)(POINTER_TO_REGISTER_DATA(REGISTER_X)) = WP34S_int2pow(*(uint64_t *)(POINTER_TO_REGISTER_DATA(REGISTER_X)));
  }

  else if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    int16_t exponent;
    bigInteger_t power;

    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    convertBigIntegerRegisterToBigInteger(REGISTER_X, &power);
    if(bigIntegerCompareUInt(&power, MAX_BIG_INTEGER_SIZE_IN_BITS) != BIG_INTEGER_GREATER_THAN) {
      exponent = power.dp[0];

      bigInteger2Exp(exponent, &power);
      convertBigIntegerToBigIntegerRegister(&power, REGISTER_X);
    }
    else {
      displayCalcErrorMessage(4, REGISTER_T, REGISTER_X);
      #if(EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "2^ this value would result in a value exceeding %" FMT16S " bits!", MAX_BIG_INTEGER_SIZE_IN_BITS);
        bigIntegerToString(&power, tmpStr3000, 10);
        showInfoDialog("In function fn2Pow:", errorMessage, tmpStr3000, NULL);
      #endif
    }
  }

  else if(getRegisterDataType(REGISTER_X) == dtReal16) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    op1 = allocateTemporaryRegister();
    reallocateRegister(op1, dtReal16, REAL16_SIZE, 0);
    real16Copy(const16_2, POINTER_TO_REGISTER_DATA(op1));

    op2 = allocateTemporaryRegister();
    copySourceRegisterToDestRegister(REGISTER_X, op2);

    result = REGISTER_X;
    powRe16Re16();
  }

  else if(getRegisterDataType(REGISTER_X) == dtReal34) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    op1 = allocateTemporaryRegister();
    reallocateRegister(op1, dtReal34, REAL34_SIZE, 0);
    real34Copy(const34_2, POINTER_TO_REGISTER_DATA(op1));

    op2 = allocateTemporaryRegister();
    copySourceRegisterToDestRegister(REGISTER_X, op2);

    result = REGISTER_X;
    powRe34Re34();
  }

  else {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X); // Invalid input data type for this operation
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot calculate the power of 2 of %s!", getDataTypeName(getRegisterDataType(REGISTER_X), true, false));
      showInfoDialog("In function fn2Pow:", errorMessage, NULL, NULL);
    #endif
  }

  refreshRegisterLine(REGISTER_X);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fn2Pow");
  #endif
}



/********************************************//**
 * \brief regX ==> regL and 10^regX ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void fn10Pow(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fn10Pow");
  #endif

  if(getRegisterDataType(REGISTER_X) == dtSmallInteger) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    *(uint64_t *)(POINTER_TO_REGISTER_DATA(REGISTER_X)) = WP34S_int10pow(*(uint64_t *)(POINTER_TO_REGISTER_DATA(REGISTER_X)));
  }

  else if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    int16_t exponent;

    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    bigInteger_t temp;
    convertBigIntegerRegisterToBigInteger(REGISTER_X, &temp);
    exponent = (bigIntegerCompareUInt(&temp, 1234) == BIG_INTEGER_GREATER_THAN ? 1234 : temp.dp[0]);

    uIntToBigInteger(10, &temp);
    op1 = allocateTemporaryRegister();
    convertBigIntegerToBigIntegerRegister(&temp, op1);

    uIntToBigInteger(exponent, &temp);
    op2 = allocateTemporaryRegister();
    convertBigIntegerToBigIntegerRegister(&temp, op2);

    result = allocateTemporaryRegister();
    powBigIBigI();

    copySourceRegisterToDestRegister(result, REGISTER_X);
    freeTemporaryRegister(result);
  }

  else if(getRegisterDataType(REGISTER_X) == dtReal16) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    op1 = allocateTemporaryRegister();
    reallocateRegister(op1, dtReal16, REAL16_SIZE, 0);
    real16Copy(const16_10, POINTER_TO_REGISTER_DATA(op1));

    op2 = allocateTemporaryRegister();
    copySourceRegisterToDestRegister(REGISTER_X, op2);

    result = REGISTER_X;
    powRe16Re16();
  }

  else if(getRegisterDataType(REGISTER_X) == dtReal34) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    op1 = allocateTemporaryRegister();
    reallocateRegister(op1, dtReal34, REAL34_SIZE, 0);
    real34Copy(const34_10, POINTER_TO_REGISTER_DATA(op1));

    op2 = allocateTemporaryRegister();
    copySourceRegisterToDestRegister(REGISTER_X, op2);

    result = REGISTER_X;
    powRe34Re34();
  }

  else {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X); // Invalid input data type for this operation
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot calculate the power of 2 of %s!", getDataTypeName(getRegisterDataType(REGISTER_X), true, false));
      showInfoDialog("In function fn2Pow:", errorMessage, NULL, NULL);
    #endif
  }

  refreshRegisterLine(REGISTER_X);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fn10Pow");
  #endif
}



void fnSin(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnSin");
  #endif

  bool_t real16 = getRegisterDataType(REGISTER_X) == dtReal16;

  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    real16 = true;
  }

  if(getRegisterDataType(REGISTER_X) == dtReal16 || getRegisterDataType(REGISTER_X) == dtReal34) {
    if(real16) {
      convertRegister16To34(REGISTER_X);
    }

    if(real34IsSpecial(POINTER_TO_REGISTER_DATA(REGISTER_X))) {
      real34Copy(const34_NaN, POINTER_TO_REGISTER_DATA(REGISTER_X));
    }
    else {
    	 WP34S_cvt_2rad_sincos(REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), NULL, REGISTER_X);
    	 setRegisterDataType(REGISTER_X, dtReal34);
    	 setRegisterDataInfo(REGISTER_X, 0);
    }

    if(real16) {
      convertRegister34To16(REGISTER_X);
    }

    roundRegister(REGISTER_X);
    refreshRegisterLine(REGISTER_X);
  }

  else {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "X register is %s", getRegisterDataTypeName(REGISTER_X, true, false));
      showInfoDialog("In function fnSin:", errorMessage, "is not supported for cos(X)!", NULL);
    #endif
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnSin");
  #endif
}



void fnCos(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnCos");
  #endif

  bool_t real16 = getRegisterDataType(REGISTER_X) == dtReal16;

  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    real16 = true;
  }

  if(getRegisterDataType(REGISTER_X) == dtReal16 || getRegisterDataType(REGISTER_X) == dtReal34) {
    if(real16) {
      convertRegister16To34(REGISTER_X);
    }

    if(real34IsSpecial(POINTER_TO_REGISTER_DATA(REGISTER_X))) {
      real34Copy(const34_NaN, POINTER_TO_REGISTER_DATA(REGISTER_X));
    }
    else {
    	 WP34S_cvt_2rad_sincos(NULL, REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), REGISTER_X);
    	 setRegisterDataType(REGISTER_X, dtReal34);
    	 setRegisterDataInfo(REGISTER_X, 0);
    }

    if(real16) {
      convertRegister34To16(REGISTER_X);
    }

    roundRegister(REGISTER_X);
    refreshRegisterLine(REGISTER_X);
  }

  else {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "X register is %s", getRegisterDataTypeName(REGISTER_X, true, false));
      showInfoDialog("In function fnCos:", errorMessage, "is not supported for cos(X)!", NULL);
    #endif
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnCos");
  #endif
}


void fnTan(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnTan");
  #endif

  bool_t real16 = getRegisterDataType(REGISTER_X) == dtReal16;

  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    real16 = true;
  }

  if(getRegisterDataType(REGISTER_X) == dtReal16 || getRegisterDataType(REGISTER_X) == dtReal34) {
    if(real16) {
      convertRegister16To34(REGISTER_X);
    }

    if(real34IsSpecial(POINTER_TO_REGISTER_DATA(REGISTER_X))) {
      real34Copy(const34_NaN, POINTER_TO_REGISTER_DATA(REGISTER_X));
    }
    else {
      real34_t cos;
    	 WP34S_cvt_2rad_sincos(REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), &cos, REGISTER_X);
    	 setRegisterDataType(REGISTER_X, dtReal34);
    	 setRegisterDataInfo(REGISTER_X, 0);
    	 real34Divide(REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), &cos, REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)));
    }

    if(real16) {
      convertRegister34To16(REGISTER_X);
    }

    roundRegister(REGISTER_X);
    refreshRegisterLine(REGISTER_X);
  }

  else {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "X register is %s", getRegisterDataTypeName(REGISTER_X, true, false));
      showInfoDialog("In function fnTan:", errorMessage, "is not supported for cos(X)!", NULL);
    #endif
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnTan");
  #endif
}



/* Have to be careful here to ensure that every function we call can handle
 * the increased size of the numbers we're using.
 */
void WP34S_cvt_2rad_sincos(real34_t *sin, real34_t *cos, calcRegister_t angle) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("WP34S_cvt_2rad_sincos");
  #endif

	 const real34_t * const *thresholds=NULL;
	 bool_t sinNeg = false, cosNeg = false, swap = false;

  //printReal34ToConsole(thresholdsDegrees[0]); printf("\n");
  //printReal34ToConsole(thresholdsDegrees[1]); printf("\n");
  //printReal34ToConsole(thresholdsDegrees[2]); printf("\n");
  //printReal34ToConsole(thresholdsDegrees[3]); printf("\n");
  //printReal34ToConsole(thresholdsGradians[0]); printf("\n");
  //printReal34ToConsole(thresholdsGradians[1]); printf("\n");
  //printReal34ToConsole(thresholdsGradians[2]); printf("\n");
  //printReal34ToConsole(thresholdsGradians[3]); printf("\n");
  //printReal34ToConsole(thresholdsRadians[0]); printf("\n");
  //printReal34ToConsole(thresholdsRadians[1]); printf("\n");
  //printReal34ToConsole(thresholdsRadians[2]); printf("\n");
  //printReal34ToConsole(thresholdsRadians[3]); printf("\n");
  //printReal34ToConsole(thresholdsMultPi[0]); printf("\n");
  //printReal34ToConsole(thresholdsMultPi[1]); printf("\n");
  //printReal34ToConsole(thresholdsMultPi[2]); printf("\n");
  //printReal34ToConsole(thresholdsMultPi[3]); printf("\n");

	 if(angularMode == AM_RADIAN) {
	   thresholds = thresholdsRadians;
	 }
	 else if(angularMode == AM_GRAD) {
    thresholds = thresholdsGradians;
  }
	 else if(angularMode == AM_DEGREE) {
    thresholds = thresholdsDegrees;
  }
	 else if(angularMode == AM_DMS) {
    convertRegisterAngleFromTo(angle, AM_DMS, AM_DEGREE);
    thresholds = thresholdsDegrees;
  }
	 else if(angularMode == AM_MULTPI) {
    thresholds = thresholdsMultPi;
  }
  else {
    sprintf(errorMessage, "In function WP34S_cvt_2rad_sincos: %" FMT8U " is an unexpected value for angularMode!", angularMode);
    displayBugScreen(errorMessage);
  }

	 // sin(-x) = -sin(x), cos(-x) = cos(x)
	 if(real34IsNegative(POINTER_TO_REGISTER_DATA(angle))) {
	  	sinNeg = true;
	  	real34ChangeSign(POINTER_TO_REGISTER_DATA(angle));
	 }

	 real34Remainder(POINTER_TO_REGISTER_DATA(angle), thresholds[3], POINTER_TO_REGISTER_DATA(angle)); // mod(angle, 360) --> angle

	 // sin(180+x) = -sin(x), cos(180+x) = -cos(x)
 	if(real34CompareGreaterEqual(REAL34_POINTER(POINTER_TO_REGISTER_DATA(angle)), thresholds[2])) {// angle >= 180
	  	real34Subtract(POINTER_TO_REGISTER_DATA(angle), thresholds[2], POINTER_TO_REGISTER_DATA(angle)); // angle - 180 --> angle
	  	sinNeg = !sinNeg;
	  	cosNeg = !cosNeg;
  }

	 // sin(90+x) = cos(x), cos(90+x) = -sin(x)
	 if(real34CompareGreaterEqual(REAL34_POINTER(POINTER_TO_REGISTER_DATA(angle)), thresholds[1])) { // angle >= 90
		  real34Subtract(POINTER_TO_REGISTER_DATA(angle), thresholds[1], POINTER_TO_REGISTER_DATA(angle)); // angle - 90 --> angle
		  swap = true;
		  cosNeg = !cosNeg;
  }

	 // sin(90-x) = cos(x), cos(90-x) = sin(x)
	 if(real34CompareEqual(REAL34_POINTER(POINTER_TO_REGISTER_DATA(angle)), thresholds[0])) {// angle == 45
	   if(sin != NULL) {
	 	  real34Copy(const34_root2on2, sin);
	 	 }
	   if(cos != NULL) {
	  	  real34Copy(const34_root2on2, cos);
	  	}
  }
	 else { // angle < 90
		  if(real34CompareGreaterThan(REAL34_POINTER(POINTER_TO_REGISTER_DATA(angle)), thresholds[0])) { // angle > 45
		   	real34Subtract(thresholds[1], POINTER_TO_REGISTER_DATA(angle), POINTER_TO_REGISTER_DATA(angle)); // 90 - angle  --> angle
			   swap = !swap;
   	}
    convertRegisterAngleFromTo(angle, angularMode == AM_DMS ? AM_DEGREE : angularMode, AM_RADIAN);
		  WP34S_sincosTaylor(REAL34_POINTER(POINTER_TO_REGISTER_DATA(angle)), swap?cos:sin, swap?sin:cos);
  }

  if(sin != NULL && sinNeg) {
    real34SetNegativeSign(sin);
  }

  if(cos != NULL && cosNeg) {
    real34SetNegativeSign(cos);
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("WP34S_cvt_2rad_sincos");
  #endif
}


/* Calculate sin and cos by Taylor series
 */
void WP34S_sincosTaylor(const real34_t *a, real34_t *sinOut, real34_t *cosOut) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("WP34S_sincosTaylor");
  #endif

 	real51_t angle, a2, t, j, z, one, sin, cos, compare;
 	int i, odd;
 	bool_t finSin = (sinOut == NULL), finCos = (cosOut == NULL);

 	real34ToReal51(a, &angle);
 	real51Multiply(&angle, &angle, &a2);
 	uInt32ToReal51(1, &j);
 	uInt32ToReal51(1, &t);
 	uInt32ToReal51(1, &sin);
 	uInt32ToReal51(1, &cos);
 	uInt32ToReal51(1, &one);

 	for(i=1; !(finSin && finCos) && i < 1000; i++) {
	  	odd = i & 1;

  		real51Add(&j, &one, &j);
	  	real51Divide(&a2, &j, &z);
  		real51Multiply(&t, &z, &t);

	  	if(!finCos) {
	   		real51Copy(&cos, &z);

		   	if(odd) {
			   	 real51Subtract(&cos, &t, &cos);
      }
			   else {
			   	 real51Add(&cos, &t, &cos);
      }

      real51Compare(&cos, &z, &compare);
		   	finCos = (real51ToInt32(&compare) == 0);
		  }

  		real51Add(&j, &one, &j);
		  real51Divide(&t, &j, &t);

	  	if(!finSin) {
		   	real51Copy(&sin, &z);

			   if(odd) {
		   	  real51Subtract(&sin, &t, &sin);
		   	}
		   	else {
		   	  real51Add(&sin, &t, &sin);
		    }

      real51Compare(&sin, &z, &compare);
		   	finSin = (real51ToInt32(&compare) == 0);
	  	}
	 }

	 if(sinOut != NULL) {
		  real51Multiply(&sin, &angle, &angle);
		  real51ToReal34(&angle, sinOut);
  }

 	if(cosOut != NULL) {
 		 real51ToReal34(&cos, cosOut);
		}

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("WP34S_sincosTaylor");
  #endif
}



void fnArctan(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnArctan");
  #endif

  dataType_t dataType = dtReal34;

  saveStack();

  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtReal16) {
    convertRegister16To34(REGISTER_X);
    dataType = dtReal16;
  }

  if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    dataType = dtReal16;
  }

  if(getRegisterDataType(REGISTER_X) == dtReal34) {
    WP34S_do_atan(REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)));
    convertRegisterAngleFromTo(REGISTER_X, AM_RADIAN, angularMode);

    if(dataType == dtReal16) {
      convertRegister34To16(REGISTER_X);
      if(angularMode == AM_DMS) {
        checkDms(REAL16_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)));
      }
    }

    temporaryInformation = TI_ANGLE;
    displayAngularMode = angularMode;
    refreshRegisterLine(REGISTER_X);
  }

  else {
    restoreStack();
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "X register is %s", getRegisterDataTypeName(REGISTER_X, true, false));
      showInfoDialog("In function fnArctan:", errorMessage, "is not supported for arctan(X)!", NULL);
    #endif
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnArctan");
  #endif
}



void WP34S_do_atan(const real34_t *x, real34_t *angle) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("WP34S_do_atan");
  #endif

 	real34_t a, b, a2, t, j, z, last;
 	int doubles = 0;
 	int invert;
 	int n;
 	int neg = real34IsNegative(x);

  real34Copy(x, &a);

 	// arrange for a >= 0
 	if(neg) {
 		 real34ChangeSign(&a);
 	}

 	// reduce range to 0 <= a < 1, using atan(x) = pi/2 - atan(1/x)
 	invert = real34CompareGreaterThan(&a, const34_1);
 	if(invert) {
 		 real34Divide(const34_1, &a, &a);
 	}

 	// Range reduce to small enough limit to use taylor series using:
	 //  tan(x/2) = tan(x)/(1+sqrt(1+tan(x)^2))
	 for(n=0; n<1000; n++) {
	  	if(real34CompareLessEqual(&a, const34_0_1)) {
		  	 break;
		  }
		  doubles++;
		  // a = a/(1+sqrt(1+a^2)) -- at most 3 iterations.
	  	real34Multiply(&a, &a, &b);
		  real34Add(&b, const34_1, &b);
		  real34SquareRoot(&b, &b);
		  real34Add(&b, const34_1, &b);
		  real34Divide(&a, &b, &a);
	 }

	 // Now Taylor series
	 // tan(x) = x(1-x^2/3+x^4/5-x^6/7...)
	 // We calculate pairs of terms and stop when the estimate doesn't change
 	real34Copy(const34_3, angle);
 	real34Copy(const34_5, &j);
 	real34Multiply(&a, &a, &a2);	// a^2
 	real34Copy(&a2, &t);
 	real34Divide(&t, angle, angle);	// s = 1-t/3 -- first two terms
 	real34Subtract(const34_1, angle, angle);

 	do {	// Loop until there is no digits changed
	  	real34Copy(angle, &last);

  		real34Multiply(&t, &a2, &t);
	  	real34Divide(&t, &j, &z);
		  real34Add(angle, &z, angle);

		  real34Add(&j, const34_2, &j);

  		real34Multiply(&t, &a2, &t);
	  	real34Divide(&t, &j, &z);
		  real34Subtract(angle, &z, angle);

		  real34Add(&j, const34_2, &j);

	 } while(!real34CompareEqual(angle, &last));

	 real34Multiply(angle, &a, angle);

	 while(doubles) {
  		real34Add(angle, angle, angle);
  		doubles--;
	 }

 	if(invert) {
  	 real34Subtract(const34_piOn2, angle, angle);
  }

 	if(neg) {
 		 real34ChangeSign(angle);
 	}

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("WP34S_do_atan");
  #endif
}



void WP34S_do_atan2(const real34_t *y, const real34_t *x, real34_t *atan) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("WP34S_do_atan2");
  #endif

	 real34_t r, t;
	 const int xNeg = real34IsNegative(x);
	 const int yNeg = real34IsNegative(y);

	 if(real34IsNaN(x) || real34IsNaN(y)) {
	   real34Copy(const34_NaN, atan);

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("WP34S_do_atan2");
    #endif

		  return;
	 }

	 if(real34CompareEqual(y, const34_0)) {
		  if(yNeg) {
			   if(real34CompareEqual(x, const34_0)) {
				    if(xNeg) {
					     real34Copy(const34_pi, atan);
					     real34SetNegativeSign(atan);
				    }
				    else {
					     real34Copy(y, atan);
					   }
			   }
			   else if(xNeg) {
			     real34Copy(const34_pi, atan);
			     real34SetNegativeSign(atan);
			   }
      else {
			     real34Copy(y, atan);
			   }
		  }
    else {
			   if(real34CompareEqual(x, const34_0)) {
			    	if(xNeg) {
 			      real34Copy(const34_pi, atan);
 			    }
				    else {
					     real34Zero(atan);
					   }
			   }
			   else if(xNeg) {
		      real34Copy(const34_pi, atan);
		    }
		   	else {
			     real34Zero(atan);
			   }
		  }

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("WP34S_do_atan2");
    #endif

		  return;
	 }

  if(real34CompareEqual(x, const34_0)) {
	  	real34Copy(const34_piOn2, atan);
	  	if(yNeg) {
      real34SetNegativeSign(atan);
    }

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("WP34S_do_atan2");
    #endif

		  return;
	 }

	 if(real34IsInfinite(x)) {
	  	if(xNeg) {
		   	if(real34IsInfinite(y)) {
			    	real34Copy(const34_3piOn4, atan);
		    		if(yNeg) {
          real34SetNegativeSign(atan);
        }
			   }
			   else {
			    	real34Copy(const34_pi, atan);
			    	if(yNeg) {
          real34SetNegativeSign(atan);
        }
			   }
		  }
		  else {
		   	if(real34IsInfinite(y)) {
			    	real34Copy(const34_piOn4, atan);
	    			if(yNeg) {
          real34SetNegativeSign(atan);
        }
			   }
			   else {
		    		real34Zero(atan);
		    		if(yNeg) {
          real34SetNegativeSign(atan);
        }
			   }
		  }

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("WP34S_do_atan2");
    #endif

		  return;
	 }

 	if(real34IsInfinite(y)) {
	  	real34Copy(const34_piOn2, atan);
	  	if(yNeg) {
      real34SetNegativeSign(atan);
    }

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("WP34S_do_atan2");
    #endif

		  return;
	 }

 	real34Divide(y, x, &t);
	 WP34S_do_atan(&t, &r);
	 if(xNeg) {
	  	real34Copy(const34_pi, &t);
	  	if(yNeg) {
     real34SetNegativeSign(&t);
    }
	 }
	 else {
	 	 real34Zero(&t);
	 }

 	real34Add(&r, &t, atan);
 	if(real34CompareEqual(atan, const34_0) && yNeg) {
    real34SetNegativeSign(atan);
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("WP34S_do_atan2");
  #endif
}



void fnArccos(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnArccos");
  #endif

  dataType_t dataType = dtReal34;

  saveStack();

  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtReal16) {
    convertRegister16To34(REGISTER_X);
    dataType = dtReal16;
  }

  if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    dataType = dtReal16;
  }

  if(getRegisterDataType(REGISTER_X) == dtReal34) {
    if(real34CompareAbsGreaterThan(REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), const34_1)) {
      restoreStack();
      displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function fnArccos:", "|X| > 1", NULL, NULL);
      #endif
    }
    else {
      WP34S_do_acos(REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)));
      convertRegisterAngleFromTo(REGISTER_X, AM_RADIAN, angularMode);

      if(dataType == dtReal16) {
        convertRegister34To16(REGISTER_X);
        if(angularMode == AM_DMS) {
          checkDms(REAL16_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)));
        }
      }

      temporaryInformation = TI_ANGLE;
      displayAngularMode = angularMode;
      refreshRegisterLine(REGISTER_X);
    }
  }

  else {
    restoreStack();
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "X register is %s", getRegisterDataTypeName(REGISTER_X, true, false));
      showInfoDialog("In function fnArccos:", errorMessage, "is not supported for arccos(X)!", NULL);
    #endif
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnArccos");
  #endif
}



void fnArcsin(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnArcsin");
  #endif

  dataType_t dataType = dtReal34;

  saveStack();

  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtReal16) {
    convertRegister16To34(REGISTER_X);
    dataType = dtReal16;
  }

  if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    dataType = dtReal16;
  }

  if(getRegisterDataType(REGISTER_X) == dtReal34) {
    if(real34CompareAbsGreaterThan(REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), const34_1)) {
      restoreStack();
      displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function fnArcsin:", "|X| > 1", NULL, NULL);
      #endif
    }
    else {
      WP34S_do_asin(REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)));
      convertRegisterAngleFromTo(REGISTER_X, AM_RADIAN, angularMode);

      if(dataType == dtReal16) {
        convertRegister34To16(REGISTER_X);
        if(angularMode == AM_DMS) {
          checkDms(REAL16_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)));
        }
      }

      temporaryInformation = TI_ANGLE;
      displayAngularMode = angularMode;
      refreshRegisterLine(REGISTER_X);
    }
  }

  else {
    restoreStack();
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "X register is %s", getRegisterDataTypeName(REGISTER_X, true, false));
      showInfoDialog("In function fnArcsin:", errorMessage, "is not supported for arcsin(X)!", NULL);
    #endif
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnArcsin");
  #endif
}



void checkDms(real16_t *angleDms) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("checkDms");
  #endif

  int16_t  sign;
  real16_t degrees, minutes, seconds;

  sign = real16IsNegative(angleDms) ? -1 : 1;
  real16SetPositiveSign(angleDms);

  real16ToIntegral(angleDms, &degrees);
  real16Subtract(angleDms, &degrees, angleDms);

  real16Multiply(angleDms, const16_100, angleDms);
  real16ToIntegral(angleDms, &minutes);
  real16Subtract(angleDms, &minutes, angleDms);

  real16Multiply(angleDms, const16_100, &seconds);

  if(real16CompareGreaterEqual(&seconds, const16_60)) {
    real16Subtract(&seconds, const16_60, &seconds);
    real16Add(&minutes, const16_1, &minutes);
  }

  if(real16CompareGreaterEqual(&minutes, const16_60)) {
    real16Subtract(&minutes, const16_60, &minutes);
    real16Add(&degrees, const16_1, &degrees);
  }

  real16Divide(&minutes, const16_100, &minutes);
  real16Add(&degrees, &minutes, angleDms);
  real16Divide(&seconds, const16_10000, &seconds);
  real16Add(angleDms, &seconds, angleDms);

  if(sign == -1) {
    real34SetNegativeSign(angleDms);
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("checkDms");
  #endif
}



void WP34S_do_asin(const real34_t *x, real34_t *angle) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("WP34S_do_asin");
  #endif

	 real34_t abx, z;

	 if(real34IsNaN(x)) {
	  	real34Copy(const34_NaN, angle);

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("WP34S_do_asin");
    #endif

	  	return;
	 }

  real34Copy(x, &abx);
	 real34SetPositiveSign(&abx);
	 if(real34CompareGreaterThan(&abx, const34_1)) {
	  	real34Copy(const34_NaN, angle);

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("WP34S_do_asin");
    #endif

	  	return;
	 }

	 // angle = 2*atan(x/(1+sqrt(1-x*x)))
	 real34Multiply(x, x, &z);
	 real34Subtract(const34_1, &z, &z);
  real34SquareRoot(&z, &z);
	 real34Add(&z, const34_1, &z);
	 real34Divide(x, &z, &z);
	 WP34S_do_atan(&z, &abx);
	 real34Add(&abx, &abx, angle);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("WP34S_do_asin");
  #endif
}



void WP34S_do_acos(const real34_t *x, real34_t *angle) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("WP34S_do_acos");
  #endif

 	real34_t abx, z;

	 if(real34IsNaN(x)) {
	  	real34Copy(const34_NaN, angle);

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("WP34S_do_acos");
    #endif

	  	return;
	 }

  real34Copy(x, &abx);
	 real34SetPositiveSign(&abx);
	 if(real34CompareGreaterThan(&abx, const34_1)) {
	  	real34Copy(const34_NaN, angle);

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("WP34S_do_acos");
    #endif

	  	return;
	 }

	 // angle = 2*atan((1-x)/sqrt(1-x*x))
	 if(real34CompareEqual(x, const34_1)) {
	 	 real34Zero(angle);
	 }
	 else {
 	 	real34Multiply(x, x, &z);
  		real34Subtract(const34_1, &z, &z);
    real34SquareRoot(&z, &z);
  		real34Subtract(const34_1, x, &abx);
  		real34Divide(&abx, &z, &z);
  	 WP34S_do_atan(&z, &abx);
  	 real34Add(&abx, &abx, angle);
	 }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("WP34S_do_acos");
  #endif
}



void fnToPolar(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnToPolar");
  #endif

  if(   (getRegisterDataType(REGISTER_X) == dtReal16 || getRegisterDataType(REGISTER_X) == dtReal34 || getRegisterDataType(REGISTER_X) == dtBigInteger)
     && (getRegisterDataType(REGISTER_Y) == dtReal16 || getRegisterDataType(REGISTER_Y) == dtReal34 || getRegisterDataType(REGISTER_Y) == dtBigInteger)) {
    bool_t real16 = false;

    if(getRegisterDataType(REGISTER_X) == dtBigInteger && getRegisterDataType(REGISTER_Y) == dtBigInteger) {
      convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
      convertBigIntegerRegisterToReal34Register(REGISTER_Y, REGISTER_Y);
      real16 = true;
    }

    else if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
      convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
      if(getRegisterDataType(REGISTER_Y) == dtReal16) {
        convertRegister16To34(REGISTER_Y);
        real16 = true;
      }
    }

    else if(getRegisterDataType(REGISTER_Y) == dtBigInteger) {
      convertBigIntegerRegisterToReal34Register(REGISTER_Y, REGISTER_Y);
      if(getRegisterDataType(REGISTER_X) == dtReal16) {
        convertRegister16To34(REGISTER_X);
        real16 = true;
      }
    }

    else if(getRegisterDataType(REGISTER_X) == dtReal16 && getRegisterDataType(REGISTER_Y) == dtReal34) {
      convertRegister16To34(REGISTER_X);
    }

    else if(getRegisterDataType(REGISTER_Y) == dtReal16 && getRegisterDataType(REGISTER_X) == dtReal34) {
      convertRegister16To34(REGISTER_Y);
    }

    if(getRegisterDataType(REGISTER_X) == dtReal16) {
      real16 = true;
      convertRegister16To34(REGISTER_X);
      convertRegister16To34(REGISTER_Y);
    }

    real34_t real34, imag34;

    real34Copy(POINTER_TO_REGISTER_DATA(REGISTER_X), &real34);
    real34Copy(POINTER_TO_REGISTER_DATA(REGISTER_Y), &imag34);
    real34RectangularToPolar(&real34, &imag34, REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_Y)));

    if(real16) {
      convertRegister34To16(REGISTER_X);
      convertRegister34To16(REGISTER_Y);
    }

    temporaryInformation = TI_RADIUS_THETA;

    refreshRegisterLine(REGISTER_X);
    refreshRegisterLine(REGISTER_Y);
  }
  else {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X); // Invalid input data type for this operation
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot convert (%s, %s) to polar coordinates!", getDataTypeName(getRegisterDataType(REGISTER_X), false, false), getDataTypeName(getRegisterDataType(REGISTER_Y), false, false));
      showInfoDialog("In function fnToPolar:", errorMessage, NULL, NULL);
    #endif
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnToPolar");
  #endif
}



void fnToRect(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnToRect");
  #endif

  if(   (getRegisterDataType(REGISTER_X) == dtReal16 || getRegisterDataType(REGISTER_X) == dtReal34 || getRegisterDataType(REGISTER_X) == dtBigInteger)
     && (getRegisterDataType(REGISTER_Y) == dtReal16 || getRegisterDataType(REGISTER_Y) == dtReal34 || getRegisterDataType(REGISTER_Y) == dtBigInteger)) {
    bool_t real16 = false;

    if(getRegisterDataType(REGISTER_X) == dtBigInteger && getRegisterDataType(REGISTER_Y) == dtBigInteger) {
      convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
      convertBigIntegerRegisterToReal34Register(REGISTER_Y, REGISTER_Y);
      real16 = true;
    }

    else if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
      convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
      if(getRegisterDataType(REGISTER_Y) == dtReal16) {
        convertRegister16To34(REGISTER_Y);
        real16 = true;
      }
    }

    else if(getRegisterDataType(REGISTER_Y) == dtBigInteger) {
      convertBigIntegerRegisterToReal34Register(REGISTER_Y, REGISTER_Y);
      if(getRegisterDataType(REGISTER_X) == dtReal16) {
        convertRegister16To34(REGISTER_X);
        real16 = true;
      }
    }

    else if(getRegisterDataType(REGISTER_X) == dtReal16 && getRegisterDataType(REGISTER_Y) == dtReal34) {
      convertRegister16To34(REGISTER_X);
    }

    else if(getRegisterDataType(REGISTER_Y) == dtReal16 && getRegisterDataType(REGISTER_X) == dtReal34) {
      convertRegister16To34(REGISTER_Y);
    }

    if(getRegisterDataType(REGISTER_X) == dtReal16) {
      real16 = true;
      convertRegister16To34(REGISTER_X);
      convertRegister16To34(REGISTER_Y);
    }

    real34_t magnitude34, theta34;

    real34Copy(POINTER_TO_REGISTER_DATA(REGISTER_X), &magnitude34);
    real34Copy(POINTER_TO_REGISTER_DATA(REGISTER_Y), &theta34);
    convertAngle34FromTo(&theta34, angularMode, AM_RADIAN);
    real34PolarToRectangular(&magnitude34, &theta34, REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_Y)));

    if(real16) {
      convertRegister34To16(REGISTER_X);
      convertRegister34To16(REGISTER_Y);
    }

    refreshRegisterLine(REGISTER_X);
    refreshRegisterLine(REGISTER_Y);
  }
  else {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X); // Invalid input data type for this operation
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot convert (%s, %s) to rectangular coordinates!", getDataTypeName(getRegisterDataType(REGISTER_X), false, false), getDataTypeName(getRegisterDataType(REGISTER_Y), false, false));
      showInfoDialog("In function fnToRect:", errorMessage, NULL, NULL);
    #endif
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnToRect");
  #endif
}



void fnExp(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnExp");
  #endif

  bool_t real16 = false;

  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    real16 = true;
  }

  else if(getRegisterDataType(REGISTER_X) == dtReal16) {
    convertRegister16To34(REGISTER_X);
    real16 = true;
  }

  else if(getRegisterDataType(REGISTER_X) != dtReal34) {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X); // Invalid input data type for this operation
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot exponentiate %s!", getDataTypeName(getRegisterDataType(REGISTER_X), true, false));
      showInfoDialog("In function fnExp:", errorMessage, NULL, NULL);
    #endif

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("fnExp");
    #endif

    return;
  }

  real34Exp(POINTER_TO_REGISTER_DATA(REGISTER_X), POINTER_TO_REGISTER_DATA(REGISTER_X));

  if(real16) {
    convertRegister34To16(REGISTER_X);
  }

  refreshRegisterLine(REGISTER_X);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnExp");
  #endif
}



void fnFactorial(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnFactorial");
  #endif

  int8_t dataType = dtReal34;
  bigInteger_t result;

  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    convertBigIntegerRegisterToBigInteger(REGISTER_X, &result);
    if(bigIntegerIsNegative(&result) || bigIntegerCompareUInt(&result, 294) == BIG_INTEGER_GREATER_THAN) {
      convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
      dataType = dtReal16;
    }
    else {
      convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
      dataType = dtBigInteger;
    }
  }

  else if(getRegisterDataType(REGISTER_X) == dtReal16) {
    convertRegister16To34(REGISTER_X);
    dataType = dtReal16;
  }

  else if(getRegisterDataType(REGISTER_X) != dtReal34) {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X); // Invalid input data type for this operation
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot calculate factorial of %s!", getDataTypeName(getRegisterDataType(REGISTER_X), true, false));
      showInfoDialog("In function fnFactorial:", errorMessage, NULL, NULL);
    #endif

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("fnFactorial");
    #endif

    return;
  }

  if(dataType == dtBigInteger) {
    uint16_t counter;

    counter = bigIntegerToUInt(&result);
    if(counter == 0) {
      uIntToBigInteger(1, &result);
    }
    else {
      for(counter--; counter>0; counter--) {
        bigIntegerMultiplyUInt(&result, counter, &result);
      }
    }

    convertBigIntegerToBigIntegerRegister(&result, REGISTER_X);
  }
  else {
    WP34S_real34Factorial(REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)));
  }

  if(dataType == dtReal16) {
    convertRegister34To16(REGISTER_X);
  }

  refreshRegisterLine(REGISTER_X);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnFactorial");
  #endif
}



void fnLnGamma(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnLnGamma");
  #endif

  bool_t real16 = false;

  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    real16 = true;
  }

  else if(getRegisterDataType(REGISTER_X) == dtReal16) {
    convertRegister16To34(REGISTER_X);
    real16 = true;
  }

  else if(getRegisterDataType(REGISTER_X) != dtReal34) {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X); // Invalid input data type for this operation
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot calculate ln" STD_GAMMA " of %s!", getDataTypeName(getRegisterDataType(REGISTER_X), true, false));
      showInfoDialog("In function fnLnGamma:", errorMessage, NULL, NULL);
    #endif

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("fnLnGamma");
    #endif

    return;
  }

  WP34S_real34LnGamma(REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)));

  if(real16) {
   convertRegister34To16(REGISTER_X);
  }

  refreshRegisterLine(REGISTER_X);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnLnGamma");
  #endif
}



void fnGamma(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnGamma");
  #endif

  bool_t real16 = false;

  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtBigInteger) {
    convertBigIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    real16 = true;
  }

  else if(getRegisterDataType(REGISTER_X) == dtReal16) {
    convertRegister16To34(REGISTER_X);
    real16 = true;
  }

  else if(getRegisterDataType(REGISTER_X) != dtReal34) {
    displayCalcErrorMessage(24, REGISTER_T, REGISTER_X); // Invalid input data type for this operation
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot calculate " STD_GAMMA " of %s!", getDataTypeName(getRegisterDataType(REGISTER_X), true, false));
      showInfoDialog("In function fnGamma:", errorMessage, NULL, NULL);
    #endif

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("fnGamma");
    #endif

    return;
  }

  WP34S_real34Gamma(REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)), REAL34_POINTER(POINTER_TO_REGISTER_DATA(REGISTER_X)));

  if(real16) {
    convertRegister34To16(REGISTER_X);
  }

  refreshRegisterLine(REGISTER_X);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnGamma");
  #endif
}



/* Check if a number is an integer.
 */
bool_t WP34S_is_int(const real34_t *x) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("WP34S_is_int");
  #endif

 	real34_t r, y;

 	if(real34IsNaN(x)) {

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("WP34S_is_int");
    #endif

 		 return false;
 	}
 	if(real34IsInfinite(x)) {

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("WP34S_is_int");
    #endif

 		 return true;
 	}

 	real34ToIntegral(x, &y);
 	real34Subtract(x, &y, &r);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("WP34S_is_int");
  #endif

 	return real34CompareEqual(&r, const34_0);
}



static void WP34S_dn_LnGamma(const real34_t *x, real34_t *res) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("WP34S_dn_LnGamma");
  #endif

	 real51_t r, s, t, u, v, xin;
	 int32_t k;

	 real34ToReal51(x, &xin);
 	real51Zero(&s);
 	real51Add(&xin, const51_21, &t);
 	for(k=20; k>=0; k--) {
	  	real51Divide(gamma_consts[k], &t, &u);
	  	real51Subtract(&t, const51_1, &t);
	  	real51Add(&s, &u, &s);
  }

	 real51Add(&s, const51_gammaC00, &t);
	 real51Ln(&t, &s);

  //		r = z + g + .5;
	 real51Add(&xin, const51_gammaR, &r);

  //		r = log(R[0][0]) + (z+.5) * log(r) - r;
	 real51Ln(&r, &u);
	 real51Add(&xin, const51_0_5, &t);
	 real51Multiply(&u, &t, &v);

	 real51Subtract(&v, &r, &u);
	 real51Add(&u, &s, &r);
	 real51ToReal34(&r, res);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("WP34S_dn_LnGamma");
  #endif
}



// common code for the [GAMMA] and LN[GAMMA]
static void WP34S_Gamma_LnGamma(const real34_t *xin, const bool_t calculateLn, real34_t *res) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("WP34S_Gamma_LnGamma");
  #endif

	 real34_t x, t;
	 int reflec = 0;

	 // Check for special cases
	 if(real34IsSpecial(xin)) {
	  	if(real34IsInfinite(xin) && !real34IsNegative(xin)) {
	  	  real34Copy(const34_plusInfinity, res);

      #if (LOG_FUNCTIONS == 1)
        leavingFunction("WP34S_Gamma_LnGamma");
      #endif

		  	 return;
    }

 	  real34Copy(const34_NaN, res);

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("WP34S_Gamma_LnGamma");
    #endif

		  return;
	 }

	 // Handle x approximately zero case
	 real34Copy(xin, &t);
	 real34SetPositiveSign(&t);
	 if(real34CompareLessThan(&t, const34_1e_24)) {
	  	if(real34CompareEqual(xin, const34_0)) {
   	  real34Copy(const34_NaN, res);

      #if (LOG_FUNCTIONS == 1)
        leavingFunction("WP34S_Gamma_LnGamma");
      #endif

      return;
    }
		  real34Divide(const34_1, xin, &x);
	  	real34Subtract(&x, const34_egamma, res);
	  	if(calculateLn) {
	  	 real34Ln(res, res);
	  	}

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("WP34S_Gamma_LnGamma");
    #endif

		  return;
	 }

	 // Correct our argument and begin the inversion if it is negative
	 if(real34CompareLessEqual(xin, const34_0)) {
	  	reflec = 1;
	  	real34Subtract(const34_1, xin, &t);
	  	if(WP34S_is_int(&t)) {
   	  real34Copy(const34_NaN, res);

      #if (LOG_FUNCTIONS == 1)
        leavingFunction("WP34S_Gamma_LnGamma");
      #endif

	  		 return;
    }
	  	real34Subtract(&t, const34_1, &x);
	 }
	 else {
	  	real34Subtract(xin, const34_1, &x);

	 	 // Provide a fast path evaluation for positive integer arguments that aren't too large
	 	 // The threshold for overflow is 205! (i.e. 204! is within range and 205! isn't).
	 	 if(WP34S_is_int(&x) && !real34CompareEqual(xin, const34_0) && real34CompareLessEqual(&x, const34_204)) {
	 	  	real34Copy(const34_1, res);
	 	  	while(real34CompareGreaterThan(&x, const34_0)) {
	 	 	  	real34Multiply(res, &x, res);
	 	 		  real34Subtract(&x, const34_1, &x);
	 	 	 }
	 	 	 if(calculateLn) {
	 	 	 	 real34Ln(res, res);

        #if (LOG_FUNCTIONS == 1)
          leavingFunction("WP34S_Gamma_LnGamma");
        #endif

	 	 	 	 return;
      }

      #if (LOG_FUNCTIONS == 1)
        leavingFunction("WP34S_Gamma_LnGamma");
      #endif

	 	  	return;
	 	 }
	 }

 	WP34S_dn_LnGamma(&x, res);
 	if(!calculateLn) {
 	  real34Exp(res, res);
 	}

 	// Finally invert if we started with a negative argument
 	if(reflec) {
 	 	// figure out xin * PI mod 2PI
 	 	real34Remainder(xin, const34_2, &t);
 	 	real34Multiply(&t, const34_pi, &t);
 	 	WP34S_sincosTaylor(&t, &x, NULL);
  		if(calculateLn) {
  		 	real34Divide(const34_pi, &x, &t);
  		 	real34Ln(&t, &t);
  		 	real34Subtract(&t, res, res);
  		}
  		else {
  	 		real34Multiply(&x, res, &t);
  	 		real34Divide(&const34_pi, &t, res);
  	 }
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("WP34S_Gamma_LnGamma");
  #endif
}



void WP34S_real34Gamma(const real34_t *xin, real34_t *res) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("WP34S_real34Gamma");
  #endif

	 WP34S_Gamma_LnGamma(xin, false, res);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("WP34S_real34Gamma");
  #endif
}



void WP34S_real34Factorial(const real34_t *xin, real34_t *res) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("WP34S_real34Factorial");
  #endif

	 real34_t x;

	 real34Add(xin, const34_1, &x);
	 WP34S_real34Gamma(&x, res);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("WP34S_real34Factorial");
  #endif
}



// The log gamma function.
void WP34S_real34LnGamma(const real34_t *xin, real34_t *res) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("WP34S_real34LnGamma");
  #endif

	 WP34S_Gamma_LnGamma(xin, true, res);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("WP34S_real34LnGamma");
  #endif
}



/* Natural logarithm.
 *
 * Take advantage of the fact that we store our numbers in the form: m * 10^e
 * so log(m * 10^e) = log(m) + e * log(10)
 * do this so that m is always in the range 0.1 <= m < 2.  However if the number
 * is already in the range 0.5 .. 1.5, this step is skipped.
 *
 * Then use the fact that ln(x^2) = 2 * ln(x) to range reduce the mantissa
 * into 1/sqrt(2) <= m < 2.
 *
 * Finally, apply the series expansion:
 *   ln(x) = 2(a+a^3/3+a^5/5+...) where a=(x-1)/(x+1)
 * which converges quickly for an argument near unity.
 */
void WP34S_real51Ln(const real51_t *xin, real51_t *res) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("WP34S_real51Ln");
  #endif

 	real51_t z, t, f, n, m, i, v, w, e;
 	int32_t expon;

 	if(real51IsSpecial(xin)) {
	  	if(real51IsNaN(xin) || real51IsNegative(xin)) {
  	   real51Copy(const51_NaN, res);

      #if (LOG_FUNCTIONS == 1)
        leavingFunction("WP34S_real51Ln");
      #endif

		  	 return;
    }

	   real51Copy(const51_plusInfinity, res);

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("WP34S_real51Ln");
    #endif

	  	return;
  }

	 if(real51CompareLessEqual(xin, const51_0)) {
	  	if(real51IsNegative(xin)) {
  	   real51Copy(const51_NaN, res);

      #if (LOG_FUNCTIONS == 1)
        leavingFunction("WP34S_real51Ln");
      #endif

		  	 return;
    }

	   real51Copy(const51_minusInfinity, res);

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("WP34S_real51Ln");
    #endif

		  return;
	 }

	 real51Copy(xin, &z);
	 real51Copy(const51_2, &f);
	 real51Subtract(xin, const51_1, &t);
	 real51Copy(&t, &v);
	 real51SetPositiveSign(&v);
	 if(real51CompareGreaterThan(&v, const51_0_5)) {
	  	expon = z.exponent + z.digits;
	  	z.exponent = -z.digits;
	 }
	 else {
		  expon = 0;
		}

  /* The too high case never happens
 	while(dn_le(const34_2, &z)) {
	  	real51Multiply(&f, const51_2, &f);
		  real51SquareRoot(&z, &z);
  } */

	 // Range reduce the value by repeated square roots.
	 // Making the constant here larger will reduce the number of later
	 // iterations at the expense of more square root operations.
	 while(real51CompareLessEqual(&z, const51_root2on2)) {
	  	real51Multiply(&f, const51_2, &f);
		  real51SquareRoot(&z, &z);
  }

 	real51Add(&z, const51_1, &t);
 	real51Subtract(&z, const51_1, &v);
 	real51Divide(&v, &t, &n);
 	real51Copy(&n, &v);
 	real51Multiply(&v, &v, &m);
 	real51Copy(const51_3, &i);

	 for(;;) {
  		real51Multiply(&m, &n, &n);
	  	real51Divide(&n, &i, &e);
	  	real51Add(&v, &e, &w);
	  	if(WP34S_relative_error(&w, &v, const51_1e_37)) {
	  		 break;
	  	}
	  	real51Copy(&w, &v);
	  	real51Add(&i, const51_2, &i);
	 }

	 real51Multiply(&f, &w, res);
	 if(expon == 0) {

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("WP34S_real51Ln");
    #endif

  	 return;
  }

	 int32ToReal51(expon, &e);
	 real51Multiply(&e, const51_ln10, &w);
	 real51Add(res, &w, res);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("WP34S_real51Ln");
  #endif
}



void WP34S_real51Log(const real51_t *xin, const real51_t *base, real51_t *res) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("WP34S_real51Log");
  #endif

 	real51_t y;

 	if(real51IsSpecial(xin)) {
	  	if(real51IsNaN(xin) || real51IsNegative(xin)) {
  	   real51Copy(const34_NaN, res);

      #if (LOG_FUNCTIONS == 1)
        leavingFunction("WP34S_real51Log");
      #endif

		  	 return;
    }

	   real51Copy(const51_plusInfinity, res);

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("WP34S_real51Log");
    #endif

	  	return;
  }

	 WP34S_real51Ln(xin, &y);

 	real51Divide(&y, base, res);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("WP34S_real51Log");
  #endif
}



void real16RectangularToPolar(const real16_t *real16, const real16_t *imag16, real16_t *magnitude16, real16_t *theta16) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real16RectangularToPolar");
  #endif

  real34_t real34, imag34, magnitude34, theta34;

  real16ToReal34(real16, &real34);
  real16ToReal34(imag16, &imag34);

  real34RectangularToPolar(&real34, &imag34, &magnitude34, &theta34);

  real34ToReal16(&magnitude34, magnitude16);
  real34ToReal16(&theta34, theta16);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real16RectangularToPolar");
  #endif
}



void real16PolarToRectangular(const real16_t *magnitude16, const real16_t *theta16, real16_t *real16, real16_t *imag16) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real16PolarToRectangular");
  #endif

  real34_t real34, imag34, magnitude34, theta34;

  real16ToReal34(magnitude16, &magnitude34);
  real16ToReal34(theta16, &theta34);

  real34PolarToRectangular(&magnitude34, &theta34, &real34, &imag34);

  real34ToReal16(&real34, real16);
  real34ToReal16(&imag34, imag16);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real16PolarToRectangular");
  #endif
}



void real34RectangularToPolar(const real34_t *real34, const real34_t *imag34, real34_t *magnitude34, real34_t *theta34) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real34RectangularToPolar");
  #endif

  if(real34IsZero(real34) && real34IsZero(imag34)) {
    real34Zero(magnitude34);
    real34Zero(theta34);
  }
  else if(real34IsZero(real34) && real34IsNegative(imag34)) {
    real34AbsToReal34(imag34, magnitude34);
    real34Copy(const34_piOn2, theta34);
    real34SetNegativeSign(theta34);
  }
  else if(real34IsZero(real34) && real34IsPositive(imag34)) {
    real34Copy(imag34, magnitude34);
    real34Copy(const34_piOn2, theta34);
  }
  else { // real34 != 0
    // Magnitude
    real34Multiply(real34, real34, magnitude34);
    real34FMA(imag34, imag34, magnitude34, magnitude34);
    real34SquareRoot(magnitude34, magnitude34);

    // Angle
    real34Divide(imag34, real34, theta34);
    WP34S_do_atan(theta34, theta34);
  }

  if(real34IsNegative(real34)) {
    if(real34IsNegative(imag34)) {
      real34Subtract(theta34, const34_pi, theta34);
      if(real34CompareLessEqual(theta34, const34_minusPi)) {
        real34Add(theta34, const34_2pi, theta34);
      }
    }
    else {
      real34Add(theta34, const34_pi, theta34);
      if(real34CompareGreaterThan(theta34, const34_pi)) {
        real34Subtract(theta34, const34_2pi, theta34);
      }
    }
  }

  convertAngle34FromTo(theta34, AM_RADIAN, angularMode);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real34RectangularToPolar");
  #endif
}



void real34PolarToRectangular(const real34_t *magnitude34, const real34_t *theta34, real34_t *real34, real34_t *imag34) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real34PolarToRectangular");
  #endif

  real34_t sin, cos;

  WP34S_sincosTaylor(theta34, &sin, &cos);
  real34Multiply(magnitude34, &cos, real34);
  real34Multiply(magnitude34, &sin, imag34);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real34PolarToRectangular");
  #endif
}



void WP34S_real34Log2(const real34_t *xin, real34_t *res) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("WP34S_real34Log2");
  #endif

  real51_t x, r;

  real34ToReal51(xin, &x);
 	WP34S_real51Log(&x, const51_ln2, &r);
  real51ToReal34(&r, res);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("WP34S_real34Log2");
  #endif
}



void WP34S_real34Log10(const real34_t *xin, real34_t *res) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("WP34S_real34Log10");
  #endif

  real51_t x, r;

  real34ToReal51(xin, &x);
 	WP34S_real51Log(&x, const51_ln10, &r);

  real51ToReal34(&r, res);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("WP34S_real34Log10");
  #endif
}



void WP34S_real34Logxy(const real34_t *yin, const real34_t *xin, real34_t *res) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("WP34S_real34Logxy");
  #endif

  real51_t x, r, lx;

  real34ToReal51(xin, &x);
	 WP34S_real51Ln(&x, &lx);

  real34ToReal51(yin, &x);
	 WP34S_real51Log(&x, &lx, &r);

  real51ToReal34(&r, res);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("WP34S_real34Logxy");
  #endif
}



bool_t WP34S_relative_error(const real51_t *x, const real51_t *y, const real51_t *tol) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("WP34S_relative_error");
  #endif

 	real51_t a;

 	if(real51CompareEqual(x, const51_0)) {
 	  real51Copy(y, &a);
 	  real51SetPositiveSign(&a);

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("WP34S_relative_error");
    #endif

 		 return real51CompareLessThan(&a, tol);
  }

 	real51Subtract(x, y, &a);
 	real51Divide(&a, x, &a);
 	real51SetPositiveSign(&a);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("WP34S_relative_error");
  #endif

 	return real51CompareLessThan(&a, tol);
}



bool_t real16CompareGreaterThan(const real16_t *number1, const real16_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real16CompareGreaterThan");
  #endif

  real16_t compare;

  real16Compare(number1, number2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real16CompareGreaterThan");
  #endif

  return real16ToInt32(&compare) > 0;
}



bool_t real34CompareGreaterThan(const real34_t *number1, const real34_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real34CompareGreaterThan");
  #endif

  real34_t compare;

  real34Compare(number1, number2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real34CompareGreaterThan");
  #endif

  return real34ToInt32(&compare) > 0;
}



bool_t real51CompareGreaterThan(const real51_t *number1, const real51_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real51CompareGreaterThan");
  #endif

  real51_t compare;

  real51Compare(number1, number2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real51CompareGreaterThan");
  #endif

  return real51ToInt32(&compare) > 0;
}



bool_t real16CompareGreaterEqual(const real16_t *number1, const real16_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real16CompareGreaterEqual");
  #endif

  real16_t compare;

  real16Compare(number1, number2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real16CompareGreaterEqual");
  #endif

  return real16ToInt32(&compare) >= 0;
}



bool_t real34CompareGreaterEqual(const real34_t *number1, const real34_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real34CompareGreaterEqual");
  #endif

  real34_t compare;

  real34Compare(number1, number2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real34CompareGreaterEqual");
  #endif

  return real34ToInt32(&compare) >= 0;
}



bool_t real16CompareEqual(const real16_t *number1, const real16_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real16CompareEqual");
  #endif

  real16_t compare;

  real16Compare(number1, number2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real16CompareEqual");
  #endif

  return real16ToInt32(&compare) == 0;
}



bool_t real34CompareEqual(const real34_t *number1, const real34_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real34CompareEqual");
  #endif

  real34_t compare;

  real34Compare(number1, number2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real34CompareEqual");
  #endif

  return real34ToInt32(&compare) == 0;
}



bool_t real51CompareEqual(const real51_t *number1, const real51_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real51CompareEqual");
  #endif

  real51_t compare;

  real51Compare(number1, number2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real51CompareEqual");
  #endif

  return (real51ToInt32(&compare) == 0);
}



bool_t real16CompareLessEqual(const real16_t *number1, const real16_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real16CompareLessEqual");
  #endif

  real16_t compare;

  real16Compare(number1, number2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real16CompareLessEqual");
  #endif

  return real16ToInt32(&compare) <= 0;
}



bool_t real34CompareLessEqual(const real34_t *number1, const real34_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real34CompareLessEqual");
  #endif

  real34_t compare;

  real34Compare(number1, number2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real34CompareLessEqual");
  #endif

  return real34ToInt32(&compare) <= 0;
}



bool_t real51CompareLessEqual(const real51_t *number1, const real51_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real51CompareLessEqual");
  #endif

  real51_t compare;

  real51Compare(number1, number2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real51CompareLessEqual");
  #endif

  return real51ToInt32(&compare) <= 0;
}



bool_t real16CompareLessThan(const real16_t *number1, const real16_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real16CompareLessThan");
  #endif

  real16_t compare;

  real16Compare(number1, number2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real16CompareLessThan");
  #endif

  return real16ToInt32(&compare) < 0;
}



bool_t real34CompareLessThan(const real34_t *number1, const real34_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real34CompareLessThan");
  #endif

  real34_t compare;

  real34Compare(number1, number2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real34CompareLessThan");
  #endif

  return real34ToInt32(&compare) < 0;
}



bool_t real51CompareLessThan(const real51_t *number1, const real51_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real51CompareLessThan");
  #endif

  real51_t compare;

  real51Compare(number1, number2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real51CompareLessThan");
  #endif

  return real51ToInt32(&compare) < 0;
}



bool_t real16CompareAbsGreaterThan(const real16_t *number1, const real16_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real16CompareAbsGreaterThan");
  #endif

  real16_t compare, num1, num2;

  real16Copy(number1, &num1);
  real16Copy(number2, &num2);
  real16SetPositiveSign(&num1);
  real16SetPositiveSign(&num2);
  real16Compare(&num1, &num2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real16CompareAbsGreaterThan");
  #endif

  return real16ToInt32(&compare) > 0;
}



bool_t real34CompareAbsGreaterThan(const real34_t *number1, const real34_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real34CompareAbsGreaterThan");
  #endif

  real34_t compare, num1, num2;

  real34Copy(number1, &num1);
  real34Copy(number2, &num2);
  real34SetPositiveSign(&num1);
  real34SetPositiveSign(&num2);
  real34Compare(&num1, &num2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real34CompareAbsGreaterThan");
  #endif

  return real34ToInt32(&compare) > 0;
}



bool_t real16CompareAbsLessThan(const real16_t *number1, const real16_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real16CompareAbsLessThan");
  #endif

  real16_t compare, num1, num2;

  real16Copy(number1, &num1);
  real16Copy(number2, &num2);
  real16SetPositiveSign(&num1);
  real16SetPositiveSign(&num2);
  real16Compare(&num1, &num2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real16CompareAbsLessThan");
  #endif

  return real16ToInt32(&compare) < 0;
}



bool_t real34CompareAbsLessThan(const real34_t *number1, const real34_t *number2) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("real34CompareAbsLessThan");
  #endif

  real34_t compare, num1, num2;

  real34Copy(number1, &num1);
  real34Copy(number2, &num2);
  real34SetPositiveSign(&num1);
  real34SetPositiveSign(&num2);
  real34Compare(&num1, &num2, &compare);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("real34CompareAbsLessThan");
  #endif

  return real34ToInt32(&compare) < 0;
}




























#ifdef NE_PAS_COMPILER
#include "decn.h"
#include "xeq.h"
#include "consts.h"
#include "complex.h"
#include "stats.h"
#include "int.h"
#include "serial.h"
#include "lcd.h"

// #define DUMP1
#ifdef DUMP1
#include <stdio.h>
static FILE *debugf = NULL;

static void open_debug(void) {
	if(debugf == NULL) {
		debugf = fopen("/dev/ttys001", "w");
	}
}
static void dump1(const decNumber *a, const char *msg) {
	char buf[2000], *b = buf;

	open_debug();
	if(decNumberIsNaN(a)) b= "NaN";
	else if(decNumberIsInfinite(a)) b = decNumberIsNegative(a)?"-inf":"inf";
	else
		decNumberToString(a, b);
	fprintf(debugf, "%s: %s\n", msg ? msg : "???", b);
	fflush(debugf);
}
#else
#define dump1(a,b)
#endif


#define MOD_DIGITS	450		/* Big enough for 1e384 mod small integer */
#define BIGMOD_DIGITS	820		/* Big enough for maxreal mod minreal */


/* Some basic conditional tests */
int dn_lt0(const decNumber *x) {
	return decNumberIsNegative(x) && ! decNumberIsZero(x);
}

int dn_le0(const decNumber *x) {
	return decNumberIsNegative(x) || decNumberIsZero(x);
}

int dn_eq0(const decNumber *x) {
	return decNumberIsZero(x);
}

int dn_eq(const decNumber *x, const decNumber *y) {
	sincosNumber a;
	return decNumberIsZero(dn_compare(&a.n, x, y));
}

int dn_eq1(const decNumber *x) {
	return dn_eq(x, &const_1);
}

int dn_lt(const decNumber *x, const decNumber *y) {
	sincosNumber a;
	return dn_lt0(dn_compare(&a.n, x, y));
}

int dn_abs_lt(const decNumber *x, const decNumber *tol) {
	decNumber a;
	return dn_lt(dn_abs(&a, x), tol);
}

int absolute_error(const decNumber *x, const decNumber *y, const decNumber *tol) {
	decNumber a;
	return dn_abs_lt(dn_subtract(&a, x, y), tol);
}

const decNumber *convergence_threshold(void) {
	return is_dblmode() ? &const_1e_32 : &const_1e_24;
}


/* Some wrapper rountines to save space
 */
decNumber *dn_compare(decNumber *r, const decNumber *a, const decNumber *b) {
	return decNumberCompare(r, a, b, &Ctx);
}

decNumber *dn_min(decNumber *r, const decNumber *a, const decNumber *b) {
	return decNumberMin(r, a, b, &Ctx);
}

decNumber *dn_max(decNumber *r, const decNumber *a, const decNumber *b) {
	return decNumberMax(r, a, b, &Ctx);
}

decNumber *dn_abs(decNumber *r, const decNumber *a) {
	return decNumberAbs(r, a, &Ctx);
}

decNumber *dn_minus(decNumber *r, const decNumber *a) {
	return decNumberMinus(r, a, &Ctx);
}

decNumber *dn_plus(decNumber *r, const decNumber *a) {
	return decNumberPlus(r, a, &Ctx);
}

decNumber *dn_sqrt(decNumber *r, const decNumber *a) {
	return decNumberSquareRoot(r, a, &Ctx);
}

decNumber *dn_exp(decNumber *r, const decNumber *a) {
	return decNumberExp(r, a, &Ctx);
}


decNumber *dn_average(decNumber *r, const decNumber *a, const decNumber *b) {
	decNumber z;

	dn_add(&z, a, b);
	return dn_div2(r, &z);
}


#if 0
/* Define a table of small integers.
 * This should be equal or larger than any of the summation integers required in the
 * various series approximations to avoid needless computation.
 */
#define MAX_SMALL_INT	9
static const decNumber *const small_ints[MAX_SMALL_INT+1] = {
	&const_0, &const_1, &const_2, &const_3, &const_4,
	&const_5, &const_6, &const_7, &const_8, &const_9,
};

void ullint_to_dn(decNumber *x, unsigned long long int n) {
	/* Check to see if the number is small enough to be in our table */
	if(n <= MAX_SMALL_INT) {
		decNumberCopy(x, small_ints[n]);
	} else {
		/* Got to do this the long way */
		decNumber z;
		int shift = 0;

		decNumberZero(x);

		while(n != 0) {
			const int r = n % 10;
			n /= 10;
			if(r != 0) {
				dn_mulpow10(&z, small_ints[r], shift);
				dn_add(x, x, &z);
			}
			++shift;
		}
	}
}
#endif

void int_to_dn(decNumber *x, int n) {
	int sgn;

	/* Account for negatives */
	if(n < 0) {
		sgn = 1;
		n = -n;
	} else
		sgn = 0;

	ullint_to_dn(x, n);

	if(sgn)
		dn_minus(x, x);
}

int dn_to_int(const decNumber *x) {
	decNumber y;
#if 0
	char buf[64];

	decNumberRescale(&y, x, &const_0, &Ctx);
	decNumberToString(&y, buf);
	return s_to_i(buf);
#else
	return decGetInt(decNumberTrunc(&y, x));
#endif
}

unsigned long long int dn_to_ull(const decNumber *x, int *sgn) {
	decNumber y, z;
	char buf[64];

	decNumberTrunc(&z, x);
	decNumberRescale(&y, &z, &const_0, &Ctx);
	if(decNumberIsNegative(&z)) {
		dn_minus(&y, &y);
		*sgn = 1;
	} else
		*sgn = 0;
	decNumberToString(&y, buf);
	return s_to_ull(buf, 10);
}



decNumber *set_inf(decNumber *x) {
	return decNumberCopy(x, &const_inf);
}

decNumber *set_neginf(decNumber *x) {
	return decNumberCopy(x, &const__inf);
}


void decNumberPI(decNumber *pi) {
	decNumberCopy(pi, &const_PI);
}
void decNumberPIon2(decNumber *pion2) {
	decNumberCopy(pion2, &const_PIon2);
}

/* Utility routine that checks if the X register is even or odd or neither.
 * Returns positive if even, zero if odd, -1 for special, -2 for fractional.
 */
int is_even(const decNumber *x) {
	decNumber y, z;

	if(decNumberIsSpecial(x))
		return -1;
	dn_abs(&z, x);
	decNumberMod(&y, &z, &const_2);
	if(dn_eq0(&y))
		return 1;
	if(dn_eq1(&y))
		return 0;
	return -2;
}

decNumber *dn_1(decNumber *r) {
	return decNumberCopy(r, &const_1);
}

decNumber *dn__1(decNumber *r) {
	return decNumberCopy(r, &const__1);
}

decNumber *dn_p2(decNumber *r, const decNumber *x) {
	return dn_add(r, x, &const_2);
}

decNumber *dn_mul2(decNumber *r, const decNumber *x) {
	return dn_multiply(r, x, &const_2);
}

decNumber *dn_div2(decNumber *r, const decNumber *x) {
	return dn_multiply(r, x, &const_0_5);
}

decNumber *dn_mul100(decNumber *r, const decNumber *x) {
	return dn_mulpow10(r, x, 2);
}

decNumber *dn_mulPI(decNumber *r, const decNumber *x) {
	return dn_multiply(r, x, &const_PI);
}

decNumber *dn_mulpow10(decNumber *r, const decNumber *x, int p) {
	decNumberCopy(r, x);
	r->exponent += p;
	return r;
}

/* Mantissa of a number
 */
#ifdef INCLUDE_MANTISSA
decNumber *decNumberMantissa(decNumber *r, const decNumber *x) {
	if(decNumberIsSpecial(x))
		return set_NaN(r);
	if(dn_eq0(x))
		return decNumberCopy(r, x);
	decNumberCopy(r, x);
	r->exponent = 1 - r->digits;
	return r;
}

/* Exponenet of a number
 */
decNumber *decNumberExponent(decNumber *r, const decNumber *x) {
	if(decNumberIsSpecial(x))
		return set_NaN(r);
	if(dn_eq0(x))
		return decNumberZero(r);
	int_to_dn(r, x->exponent + x->digits - 1);
	return r;
}

/* 1 ULP
 */
static int realULP(decNumber *r, const decNumber *x) {
	int dblmode;
	int subnormal = 0;
	int expshift;
	int minexp;

	if(decNumberIsSpecial(x)) {
		if(decNumberIsInfinite(x))
			set_inf(r);
		else
			set_NaN(r);
		return 0;
	}

	dblmode = is_dblmode();

	if(dblmode) {
		expshift = DECIMAL128_Pmax;
		minexp = DECIMAL128_Emin - DECIMAL128_Pmax + 1;
		if(x->exponent < DECIMAL128_Emin)
			subnormal = 1;
	} else {
		expshift = DECIMAL64_Pmax;
		minexp = DECIMAL64_Emin - DECIMAL64_Pmax + 1;
		if(x->exponent < DECIMAL64_Emin)
			subnormal = 1;
	}

	dn_1(r);
	if(dn_eq0(x) || subnormal)
		r->exponent = minexp;
	else
		r->exponent = x->exponent + x->digits - expshift;
	return subnormal;
}

decNumber *decNumberULP(decNumber *r, const decNumber *x) {
	realULP(r, x);
	return r;
}

decNumber *decNumberNeighbour(decNumber *r, const decNumber *y, const decNumber *x) {
	decNumber ulp;
	int down, subnormal;

	if(decNumberIsNaN(y))
		return set_NaN(r);
	if(decNumberIsSpecial(x))
		return decNumberCopy(r, x);
	dn_compare(&ulp, y, x);
	if(dn_eq0(&ulp))
		return decNumberCopy(r, y);

	down = decNumberIsNegative(&ulp) ? 1 : 0;
	subnormal = realULP(&ulp, x);

	if(! subnormal && x->digits == 1 && x->lsu[0] == 1)
		ulp.exponent -= (! decNumberIsNegative(x)) == down;
	if(down)
		dn_subtract(r, x, &ulp);
	else
		dn_add(r, x, &ulp);
	return r;
}
#endif

/* Multiply Add: x + y * z
 */
#ifdef INCLUDE_MULADD
decNumber *decNumberMAdd(decNumber *r, const decNumber *z, const decNumber *y, const decNumber *x) {
	decNumber t;

	dn_multiply(&t, x, y);
	return dn_add(r, &t, z);
}
#endif


/* Reciprocal of a number.
 * Division is correctly rounded so we just use that instead of coding
 * something special (that could be more efficient).
 */
decNumber *decNumberRecip(decNumber *r, const decNumber *x) {
	return dn_divide(r, &const_1, x);
}

/* Reciprocal of a function's result.
 * This routine calls the specified function and then multiplicatively
 * inverts its result.
 */
#if 0
static decNumber *dn_recip(decNumber *r, const decNumber *x,
		decNumber *(*func)(decNumber *r, const decNumber *x)) {
	decNumber z;

	(*func)(&z, x);
	return decNumberRecip(r, &z);
}
#endif

/* A plethora of round to integer functions to support the large variety
 * of possibilities in this area.  Start with a common utility function
 * that saves the current rounding mode, rounds as required and restores
 * the rounding mode properly.
 */
static decNumber *round2int(decNumber *r, const decNumber *x, enum rounding mode) {
	enum rounding a = Ctx.round;

	Ctx.round = mode;
	decNumberToIntegralValue(r, x, &Ctx);
	Ctx.round = a;
	return r;
}

/* Floor - truncate to minus infinity.
 */
decNumber *decNumberFloor(decNumber *r, const decNumber *x) {
	return round2int(r, x, DEC_ROUND_FLOOR);
}

/* Ceiling - truncate to plus infinity.
 */
decNumber *decNumberCeil(decNumber *r, const decNumber *x) {
	return round2int(r, x, DEC_ROUND_CEILING);
}

/* Trunc - truncate to zero.
 */
decNumber *decNumberTrunc(decNumber *r, const decNumber *x) {
	return round2int(r, x, DEC_ROUND_DOWN);
}

/* Round - round 0.5 up.
 */
decNumber *decNumberRound(decNumber *r, const decNumber *x) {
	return round2int(r, x, DEC_ROUND_HALF_UP);
}

/* Intg - round 0.5 even.
 */
static decNumber *decNumberIntg(decNumber *r, const decNumber *x) {
	return round2int(r, x, DEC_ROUND_HALF_EVEN);
}

/* Frac - round 0.5 up.
 */
decNumber *decNumberFrac(decNumber *r, const decNumber *x) {
	decNumber y;

	round2int(&y, x, DEC_ROUND_DOWN);
	return dn_subtract(r, x, &y);
}


static void dn_gcd(decNumber *r, const decNumber *x, const decNumber *y, int bigmod) {
	decNumber b, t;

	decNumberCopy(&b, y);
	decNumberCopy(r, x);
	while(! dn_eq0(&b)) {
		decNumberCopy(&t, &b);
		if(bigmod)
			decNumberBigMod(&b, r, &t);
		else
			decNumberMod(&b, r, &t);
		decNumberCopy(r, &t);
	}
}

static int dn_check_gcd(decNumber *r, const decNumber *x, const decNumber *y,
		decNumber *a, decNumber *b) {
	if(decNumberIsSpecial(x) || decNumberIsSpecial(y)) {
		if(decNumberIsNaN(x) || decNumberIsNaN(y))
			set_NaN(r);
		else
			set_inf(r);
	} else if(!is_int(x) || !is_int(y))
		set_NaN(r);
	else {
		dn_abs(a, x);
		dn_abs(b, y);
		return 0;
	}
	return 1;
}

decNumber *decNumberGCD(decNumber *r, const decNumber *x, const decNumber *y) {
	decNumber a, b;

	if(dn_check_gcd(r, x, y, &a, &b))
		return r;

	if(dn_eq0(x))
		decNumberCopy(r, &b);
	else if(dn_eq0(y))
		decNumberCopy(r, &a);
	else
		dn_gcd(r, &a, &b, 1);
	return r;
}

decNumber *decNumberLCM(decNumber *r, const decNumber *x, const decNumber *y) {
	decNumber gcd, a, b, t;

	if(dn_check_gcd(r, x, y, &a, &b))
		return r;

	if(dn_eq0(x) || dn_eq0(y))
		decNumberCopy(r, x);
	dn_gcd(&gcd, &a, &b, 1);
	dn_divide(&t, &a, &gcd);
	return dn_multiply(r, &t, &b);
}


/* The extra logarithm and power functions */

/* Raise y^x */
decNumber *dn_power_internal(decNumber *r, const decNumber *y, const decNumber *x, const decNumber *logy) {
	decNumber s, t, my;
	int isxint, xodd, ynegative;
	int negate = 0;

	if(dn_eq1(y))
		return dn_1(r);				// 1^x = 1

	if(dn_eq0(x))
		return dn_1(r);				// y^0 = 1

	if(decNumberIsNaN(x) || decNumberIsNaN(y))
		return set_NaN(r);

	if(dn_eq1(x))
		return decNumberCopy(r, y); 		// y^1 = y

	if(decNumberIsInfinite(x)) {
		int ylt1;
		if(dn_eq(y, &const__1))
			return dn_1(r);			// -1 ^ +/-inf = 1

		ylt1 = dn_abs_lt(y, &const_1);
		if(decNumberIsNegative(x)) {
			if(ylt1)
				return set_inf(r);		// y^-inf |y|<1 = +inf
			return decNumberZero(r);		// y^-inf |y|>1 = +0
		}
		if(ylt1)
			return decNumberZero(r);		// y^inf |y|<1 = +0
		return set_inf(r);				// y^inf |y|>1 = +inf
	}

	isxint = is_int(x);
	ynegative = decNumberIsNegative(y);
	if(decNumberIsInfinite(y)) {
		if(ynegative) {
			xodd = isxint && is_even(x) == 0;
			if(decNumberIsNegative(x)) {
				decNumberZero(r);		// -inf^x x<0 = +0
				if(xodd)			// -inf^x odd x<0 = -0
					return decNumberCopy(r, &const__0);
				return r;
			}
			if(xodd)
				return set_neginf(r);		// -inf^x odd x>0 = -inf
			return set_inf(r);			// -inf^x x>0 = +inf
		}
		if(decNumberIsNegative(x))
			return decNumberZero(r);		// +inf^x x<0 = +0
		return set_inf(r);				// +inf^x x>0 = +inf
	}

	if(dn_eq0(y)) {
		xodd = isxint && is_even(x) == 0;
		if(decNumberIsNegative(x)) {
			if(xodd && ynegative)
				return set_neginf(r);		// -0^x odd x<0 = -inf
			return set_inf(r);			// 0^x x<0 = +inf
		}
		if(xodd && ynegative)
			return decNumberCopy(r, &const__0);	// -0^x odd x>0 = -/+0
		return decNumberZero(r);			// 0^x x>0 = +0
	}

	if(ynegative) {
		if(!isxint)
			return set_NaN(r);			// y^x y<0, x not odd int = NaN
		if(is_even(x) == 0)				// y^x, y<0, x odd = - ((-y)^x)
			negate = 1;
		dn_minus(&my, y);
		y = &my;
	}
	if(logy == NULL) {
		dn_ln(&t, y);
		logy = &t;
	}
	dn_multiply(&s, logy, x);
	dn_exp(r, &s);
	if(negate)
		return dn_minus(r, r);
	return r;
}

decNumber *dn_power(decNumber *r, const decNumber *y, const decNumber *x) {
	return dn_power_internal(r, y, x, NULL);
}


/* ln(1+x) */
decNumber *decNumberLn1p(decNumber *r, const decNumber *x) {
	decNumber u, v, w;

	if(decNumberIsSpecial(x) || dn_eq0(x)) {
		return decNumberCopy(r, x);
	}
	dn_p1(&u, x);
	dn_m1(&v, &u);
	if(dn_eq0(&v)) {
		return decNumberCopy(r, x);
	}
	dn_divide(&w, x, &v);
	dn_ln(&v, &u);
	return dn_multiply(r, &v, &w);
}

/* exp(x)-1 */
decNumber *decNumberExpm1(decNumber *r, const decNumber *x) {
	decNumber u, v, w;

	if(decNumberIsSpecial(x)) {
		return decNumberCopy(r, x);
	}
	dn_exp(&u, x);
	dn_m1(&v, &u);
	if(dn_eq0(&v)) {
		return decNumberCopy(r, x);
	}
	if(dn_eq(&v, &const__1)) {
		return dn__1(r);
	}
	dn_multiply(&w, &v, x);
	dn_ln(&v, &u);
	return dn_divide(r, &w, &v);
}


decNumber *decNumberPow2(decNumber *r, const decNumber *x) {
	return dn_power_internal(r, &const_2, x, &const_ln2);
}

decNumber *decNumberPow10(decNumber *r, const decNumber *x) {
	return dn_power_internal(r, &const_10, x, &const_ln10);
}

decNumber *decNumberPow_1(decNumber *r, const decNumber *x) {
	int even = is_even(x);
	decNumber t, u;

	if(even == 1)
		return dn_1(r);
	if(even == 0)
		return dn__1(r);
	decNumberMod(&u, x, &const_2);
	dn_mulPI(&t, &u);
	sincosTaylor(&t, NULL, r);
	return r;
}

/* Square - this almost certainly could be done more efficiently
 */
decNumber *decNumberSquare(decNumber *r, const decNumber *x) {
	return dn_multiply(r, x, x);
}

/* Cube - again could be done more efficiently */
decNumber *decNumberCube(decNumber *r, const decNumber *x) {
	decNumber z;

	decNumberSquare(&z, x);
	return dn_multiply(r, &z, x);
}
Fait un makefile bordel de merde
/* Cube root */
decNumber *decNumberCubeRoot(decNumber *r, const decNumber *x) {
	decNumber third, t;

	decNumberRecip(&third, &const_3);

	if(decNumberIsNegative(x)) {
		dn_minus(r, x);
		dn_power(&t, r, &third);
		return dn_minus(r, &t);
	}
	return dn_power(r, x, &third);
}

#ifdef INCLUDE_XROOT
decNumber *decNumberXRoot(decNumber *r, const decNumber *a, const decNumber *b) {
	decNumber s, t;

	decNumberRecip(&s, b);

	if(decNumberIsNegative(a)) {
		if(is_even(b) == 0) {
			dn_minus(r, a);
			dn_power(&t, r, &s);
			return dn_minus(r, &t);
		}
		return set_NaN(r);
	}
	return dn_power(r, a, &s);
}
#endif



decNumber *decNumberBigMod(decNumber *res, const decNumber *x, const decNumber *y) {
	/* Declare a structure large enough to hold a really long number.
	 * This structure is likely to be larger than is required.
	 */
	struct {
		decNumber n;
		decNumberUnit extra[((BIGMOD_DIGITS-DECNUMDIGITS+DECDPUN-1)/DECDPUN)];
	} out;

	int digits = Ctx.digits;

	Ctx.digits = BIGMOD_DIGITS;
	decNumberRemainder(&out.n, x, y, &Ctx);
	Ctx.digits = digits;

#ifdef INCLUDE_MOD41
	decNumberPlus(res, &out.n, &Ctx);
	if(XeqOpCode == (OP_DYA | OP_MOD41) && decNumberIsNegative(x) != decNumberIsNegative(y) && !dn_eq0(res))
		dn_add(res, res, y);
	return res;
#else
	return 	decNumberPlus(res, &out.n, &Ctx);
#endif
}


/*
 *  Common helper for angle conversions
 *  Checks the present mode and converts accordingly
 */
static decNumber *decNumberDRG_internal(decNumber *res, const decNumber *x, s_opcode op) {
#define DRG(op,mode) (((op) << 2) | (mode))

	switch (DRG(argKIND(op), get_trig_mode())) {

	case DRG(OP_2DEG,  TRIG_RAD):
	case DRG(OP_RAD2,  TRIG_DEG):
		return decNumberR2D(res, x);

	case DRG(OP_2DEG,  TRIG_GRAD):
	case DRG(OP_GRAD2, TRIG_DEG):
		return decNumberG2D(res, x);

	case DRG(OP_2RAD,  TRIG_DEG):
	case DRG(OP_DEG2,  TRIG_RAD):
		return decNumberD2R(res, x);

	case DRG(OP_2RAD,  TRIG_GRAD):
	case DRG(OP_GRAD2, TRIG_RAD):
		return decNumberG2R(res, x);

	case DRG(OP_2GRAD, TRIG_DEG):
	case DRG(OP_DEG2,  TRIG_GRAD):
		return decNumberD2G(res, x);

	case DRG(OP_2GRAD, TRIG_RAD):
	case DRG(OP_RAD2,  TRIG_GRAD):
		return decNumberR2G(res, x);

	default:
		return decNumberCopy(res, x);
	}
#undef DRG
}

static void cvt_rad2(decNumber *res, const decNumber *x) {
	decNumberDRG_internal(res, x, OP_RAD2);
}

/* Calculate sin and cos of the given number in radians.
 * We need to do some range reduction to guarantee that our Taylor series
 * converges rapidly.
 */
void dn_sincos(const decNumber *v, decNumber *sinv, decNumber *cosv)
{
	decNumber x;

	if(decNumberIsSpecial(v))
		cmplx_NaN(sinv, cosv);
	else {
		decNumberMod(&x, v, &const_2PI);
		sincosTaylor(&x, sinv, cosv);
	}
}



/* Have to be careful here to ensure that every function we call can handle
 * the increased size of the numbers we're using.
 */
decNumber *decNumberTan(decNumber *res, const decNumber *x) {
	sincosNumber x2, s, c;

	if(decNumberIsSpecial(x))
		return set_NaN(res);
	else {
		const int digits = Ctx.digits;
		Ctx.digits = SINCOS_DIGITS;
		cvt_2rad_sincos(&s.n, &c.n, x);
		if(decNumberIsZero(&c.n))
			return set_NaN(res);
		dn_divide(&x2.n, &s.n, &c.n);
		Ctx.digits = digits;
		return dn_plus(res, &x2.n);
	}
}

#if 0
decNumber *decNumberSec(decNumber *res, const decNumber *x) {
	return dn_recip(res, x, &decNumberCos);
}

decNumber *decNumberCosec(decNumber *res, const decNumber *x) {
	return dn_recip(res, x, &decNumberSin);
}

decNumber *decNumberCot(decNumber *res, const decNumber *x) {
	return dn_recip(res, x, &decNumberTan);
}
#endif

decNumber *decNumberSinc(decNumber *res, const decNumber *x) {
	decNumber s;

	decNumberSquare(&s, x);
	if(dn_eq1(dn_p1(&s, &s)))
		return dn_1(res);
	decNumberMod(res, x, &const_2PI);
	sincosTaylor(res, &s, NULL);
	return dn_divide(res, &s, x);
}




decNumber *decNumberArcTan2(decNumber *res, const decNumber *a, const decNumber *b) {
	decNumber z;

	do_atan2(&z, a, b);
	cvt_rad2(res, &z);
	return res;
}

void op_r2p(enum nilop op) {
	decNumber x, y, rx, ry;

	getXY(&x, &y);
	cmplxToPolar(&rx, &ry, &x, &y);
	cvt_rad2(&y, &ry);
	setlastX();
	setXY(&rx, &y);
#ifdef RP_PREFIX
	RectPolConv = 1;
#endif
}

void op_p2r(enum nilop op) {
	decNumber x, y, t, range, angle;

	getXY(&range, &angle);
	decNumberCos(&t, &angle);
	dn_multiply(&x, &t, &range);
	decNumberSin(&t, &angle);
	dn_multiply(&y, &t, &range);
	setlastX();
	setXY(&x, &y);
#ifdef RP_PREFIX
	RectPolConv = 2;
#endif
}


/* Hyperbolic functions.
 * We start with a utility routine that calculates sinh and cosh.
 * We do the sihn as (e^x - 1) (e^x + 1) / (2 e^x) for numerical stability
 * reasons if the value of x is smallish.
 */
void dn_sinhcosh(const decNumber *x, decNumber *sinhv, decNumber *coshv) {
	decNumber t, u, v;

	if(sinhv != NULL) {
		if(dn_abs_lt(x, &const_0_5)) {
			decNumberExpm1(&u, x);
			dn_div2(&t, &u);
			dn_inc(&u);
			dn_divide(&v, &t, &u);
			dn_inc(&u);
			dn_multiply(sinhv, &u, &v);
		} else {
			dn_exp(&u, x);			// u = e^x
			decNumberRecip(&v, &u);		// v = e^-x
			dn_subtract(&t, &u, &v);	// r = e^x - e^-x
			dn_div2(sinhv, &t);
		}
	}
	if(coshv != NULL) {
		dn_exp(&u, x);			// u = e^x
		decNumberRecip(&v, &u);		// v = e^-x
		dn_average(coshv, &v, &u);	// r = (e^x + e^-x)/2
	}
}

decNumber *decNumberSinh(decNumber *res, const decNumber *x) {
	if(decNumberIsSpecial(x)) {
		if(decNumberIsNaN(x))
			return set_NaN(res);
		return decNumberCopy(res, x);
	}
	dn_sinhcosh(x, res, NULL);
	return res;
}

decNumber *decNumberCosh(decNumber *res, const decNumber *x) {
	if(decNumberIsSpecial(x)) {
		if(decNumberIsNaN(x))
			return set_NaN(res);
		return set_inf(res);
	}
	dn_sinhcosh(x, NULL, res);
	return res;
}

decNumber *decNumberTanh(decNumber *res, const decNumber *x) {
	decNumber a, b;

	if(decNumberIsNaN(x))
		return set_NaN(res);
	if(!dn_abs_lt(x, &const_100)) {
		if(decNumberIsNegative(x))
			return dn__1(res);
		return dn_1(res);
	}
	dn_add(&a, x, x);
	decNumberExpm1(&b, &a);
	dn_p2(&a, &b);
	return dn_divide(res, &b, &a);
}


#if 0
decNumber *decNumberSech(decNumber *res, const decNumber *x) {
	return dn_recip(res, x, &decNumberCosh);
}

decNumber *decNumberCosech(decNumber *res, const decNumber *x) {
	return dn_recip(res, x, &decNumberSinh);
}

decNumber *decNumberCoth(decNumber *res, const decNumber *x) {
	return dn_recip(res, x, &decNumberTanh);
}
#endif

decNumber *decNumberArcSinh(decNumber *res, const decNumber *x) {
	decNumber y, z;

	decNumberSquare(&y, x);		// y = x^2
	dn_p1(&z, &y);			// z = x^2 + 1
	dn_sqrt(&y, &z);		// y = sqrt(x^2+1)
	dn_inc(&y);			// y = sqrt(x^2+1)+1
	dn_divide(&z, x, &y);
	dn_inc(&z);
	dn_multiply(&y, x, &z);
	return decNumberLn1p(res, &y);
}


decNumber *decNumberArcCosh(decNumber *res, const decNumber *x) {
	decNumber z;

	decNumberSquare(res, x);	// r = x^2
	dn_m1(&z, res);			// z = x^2 + 1
	dn_sqrt(res, &z);		// r = sqrt(x^2+1)
	dn_add(&z, res, x);		// z = x + sqrt(x^2+1)
	return dn_ln(res, &z);
}

decNumber *decNumberArcTanh(decNumber *res, const decNumber *x) {
	decNumber y, z;

	if(decNumberIsNaN(x))
		return set_NaN(res);
	dn_abs(&y, x);
	if(dn_eq1(&y)) {
		if(decNumberIsNegative(x))
			return set_neginf(res);
		return set_inf(res);
	}
	// Not the obvious formula but more stable...
	dn_1m(&z, x);
	dn_divide(&y, x, &z);
	dn_mul2(&z, &y);
	decNumberLn1p(&y, &z);
	return dn_div2(res, &y);
}


decNumber *decNumberD2R(decNumber *res, const decNumber *x) {
	return dn_multiply(res, x, &const_PIon180);
}

decNumber *decNumberR2D(decNumber *res, const decNumber *x) {
	return dn_divide(res, x, &const_PIon180);
}


decNumber *decNumberG2R(decNumber *res, const decNumber *x) {
	return dn_multiply(res, x, &const_PIon200);
}

decNumber *decNumberR2G(decNumber *res, const decNumber *x) {
	return dn_divide(res, x, &const_PIon200);
}

decNumber *decNumberG2D(decNumber *res, const decNumber *x) {
	return dn_multiply(res, x, &const_0_9);
}

decNumber *decNumberD2G(decNumber *res, const decNumber *x) {
	return dn_divide(res, x, &const_0_9);
}

decNumber *decNumberDRG(decNumber *res, const decNumber *x) {
	return decNumberDRG_internal(res, x, XeqOpCode);
}

/* Check the arguments a little and perform the computation of
 * ln(permutation) which is common across both our callers.
 *
 * This is the real version.
 */
enum perm_opts { PERM_INVALID=0, PERM_INTG, PERM_NORMAL };
static enum perm_opts perm_helper(decNumber *r, const decNumber *x, const decNumber *y) {
	decNumber n, s;

	if(decNumberIsSpecial(x) || decNumberIsSpecial(y) || dn_lt0(x) || dn_lt0(y)) {
		if(decNumberIsInfinite(x) && !decNumberIsInfinite(y))
			set_inf(r);
		else
			set_NaN(r);
		return PERM_INVALID;
	}
	dn_p1(&n, x);				// x+1
	decNumberLnGamma(&s, &n);		// lnGamma(x+1) = Ln x!

	dn_subtract(r, &n, y);	// x-y+1
	if(dn_le0(r)) {
		set_NaN(r);
		return PERM_INVALID;
	}
	decNumberLnGamma(&n, r);		// LnGamma(x-y+1) = Ln (x-y)!
	dn_subtract(r, &s, &n);

	if(is_int(x) && is_int(y))
		return PERM_INTG;
	return PERM_NORMAL;
}


/* Calculate permutations:
 * C(x, y) = P(x, y) / y! = x! / ( (x-y)! y! )
 */
decNumber *decNumberComb(decNumber *res, const decNumber *x, const decNumber *y) {
	decNumber r, n, s;
	const enum perm_opts code = perm_helper(&r, x, y);

	if(code != PERM_INVALID) {
		dn_p1(&n, y);				// y+1
		decNumberLnGamma(&s, &n);		// LnGamma(y+1) = Ln y!
		dn_subtract(&n, &r, &s);

		dn_exp(res, &n);
		if(code == PERM_INTG)
			decNumberIntg(res, res);
	} else
		decNumberCopy(res, &r);
	return res;
}

/* Calculate permutations:
 * P(x, y) = x! / (x-y)!
 */
decNumber *decNumberPerm(decNumber *res, const decNumber *x, const decNumber *y) {
	decNumber t;
	const enum perm_opts code = perm_helper(&t, x, y);

	if(code != PERM_INVALID) {
		dn_exp(res, &t);
		if(code == PERM_INTG)
			decNumberIntg(res, res);
	} else
		decNumberCopy(res, &t);
	return res;
}

#ifdef _DEBUG_
#include <stdio.h>
char dump[DECNUMDIGITS + 10];
#define DUMP(d, s) ((int) decNumberToString(d, dump), fprintf(f, s "=%s\n", dump))

#endif

const decNumber *const gamma_consts[21] = {
	&const_gammaC01, &const_gammaC02, &const_gammaC03,
	&const_gammaC04, &const_gammaC05, &const_gammaC06,
	&const_gammaC07, &const_gammaC08, &const_gammaC09,
	&const_gammaC10, &const_gammaC11, &const_gammaC12,
	&const_gammaC13, &const_gammaC14, &const_gammaC15,
	&const_gammaC16, &const_gammaC17, &const_gammaC18,
	&const_gammaC19, &const_gammaC20, &const_gammaC21,
};

// lnBeta(x, y) = lngamma(x) + lngamma(y) - lngamma(x+y)
decNumber *decNumberLnBeta(decNumber *res, const decNumber *x, const decNumber *y) {
	decNumber s, t, u;

	decNumberLnGamma(&s, x);
	busy();
	decNumberLnGamma(&t, y);
	busy();
	dn_add(&u, &s, &t);
	dn_add(&s, x, y);
	decNumberLnGamma(&t, &s);
	dn_subtract(res, &u, &t);
	return res;
}

decNumber *decNumberHMS2HR(decNumber *res, const decNumber *xin) {
	decNumber m, s, t, x;
	const int neg = decNumberIsNegative(xin);

	// decode hhhh.mmss...
	dn_abs(&x, xin);
	decNumberFrac(&t, &x);			// t = .mmss
	dn_mul100(&s, &t);			// s = mm.ss
	decNumberTrunc(&m, &s);			// m = mm
	decNumberFrac(&t, &s);			// t = .ss
	dn_multiply(&s, &t, &const_1on60);	// s = ss.sss / 60
	dn_mulpow10(&s, &s, 2);
	dn_add(&t, &m, &s);			// s = mm + ss.sss / 60
	dn_multiply(&m, &t, &const_1on60);
	decNumberTrunc(&s, &x);			// s = hh
	dn_add(res, &m, &s);
	if(neg) dn_minus(res, res);
	return res;
}

decNumber *decNumberHR2HMS(decNumber *res, const decNumber *xin) {
	decNumber m, s, t, x;
	const int neg = decNumberIsNegative(xin);

	dn_abs(&x, xin);
	decNumberFrac(&t, &x);			// t = .mmssss
	dn_multiply(&s, &t, &const_60);		// s = mm.ssss
	decNumberTrunc(&m, &s);			// m = mm
	decNumberFrac(&t, &s);			// t = .ssss
	dn_multiply(&s, &t, &const_0_6);	// scale down by 60/100
	dn_add(&t, &s, &m);			// t = mm.ss
	dn_mulpow10(&m, &t, -2);		// t = .mmss
	decNumberTrunc(&s, &x);			// s = hh
	dn_add(&t, &m, &s);			// t = hh.mmss = result

	// Round to the appropriate number of digits for the result
	decNumberRoundDigits(&t, &t, is_dblmode() ? 34 : 16, DEC_ROUND_HALF_EVEN);

	// Now fix any rounding/carry issues
	dn_mulpow10(&s, &t, 2);			// hhmm.ssss
	decNumberFrac(&m, &s);			// .ssss
	if(dn_ge(&m, &const_0_6))
		dn_add(&s, &s, &const_0_4);
	dn_mulpow10(res, &s, -2);		// hh.mmssss
	decNumberFrac(&m, res);
	if(dn_ge(&m, &const_0_6))
		dn_add(res, res, &const_0_4);
	if(neg) dn_minus(res, res);
	return res;
}

decNumber *decNumberHMSAdd(decNumber *res, const decNumber *x, const decNumber *y) {
	decNumber a, b, c;

	decNumberHMS2HR(&a, x);
	decNumberHMS2HR(&b, y);
	dn_add(&c, &a, &b);
	decNumberHR2HMS(res, &c);
	return res;
}

decNumber *decNumberHMSSub(decNumber *res, const decNumber *x, const decNumber *y) {
	decNumber a, b, c;

	decNumberHMS2HR(&a, x);
	decNumberHMS2HR(&b, y);
	dn_subtract(&c, &a, &b);
	decNumberHR2HMS(res, &c);
	return res;
}

/* Logical operations on decNumbers.
 * We treat 0 as false and non-zero as true.
 */
static int dn2bool(const decNumber *x) {
	return dn_eq0(x)?0:1;
}

static decNumber *bool2dn(decNumber *res, int l) {
	if(l)
		dn_1(res);
	else
		decNumberZero(res);
	return res;
}

decNumber *decNumberNot(decNumber *res, const decNumber *x) {
	return bool2dn(res, !dn2bool(x));
}

/*
 *  Execute the logical operations via a truth table.
 *  Each nibble encodes a single operation.
 */
decNumber *decNumberBooleanOp(decNumber *res, const decNumber *x, const decNumber *y) {
	const unsigned int TRUTH_TABLE = 0x9176E8;
	const unsigned int bit = ((argKIND(XeqOpCode) - OP_LAND) << 2) + (dn2bool(y) << 1) + dn2bool(x);
	return bool2dn(res, (TRUTH_TABLE >> bit) & 1);
}


/*
 * Round a number to a given number of digits and with a given mode
 */
decNumber *decNumberRoundDigits(decNumber *res, const decNumber *x, const int digits, const enum rounding round) {
	enum rounding default_round = Ctx.round;
	int default_digits = Ctx.digits;

	Ctx.round = round;
	Ctx.digits = digits;
	decNumberPlus(res, x, &Ctx);
	Ctx.digits = default_digits;
	Ctx.round = default_round;
	return res;
}


/*
 *  Round to display accuracy
 */
decNumber *decNumberRnd(decNumber *res, const decNumber *x) {
	decNumber t, u;
#if defined(INCLUDE_SIGFIG_MODE)
	int numdig;
	enum display_modes dmode;
#else
	int numdig = UState.dispdigs + 1;
	enum display_modes dmode = (enum display_modes) UState.dispmode;
#endif

	if(decNumberIsSpecial(x))
		return decNumberCopy(res, x);

	if(UState.fract) {
		decNumber2Fraction(&t, &u, x);
		return dn_divide(res, &t, &u);
	}

#if defined(INCLUDE_SIGFIG_MODE)
	dmode = get_dispmode_digs(&numdig);
	numdig++;
	if(dmode == MODE_STD) {
		// to fit new definition of std_round_fix in display.c
		dmode = std_round_fix(x, &numdig, dmode, numdig-1);
		numdig = DISPLAY_DIGITS;
 	}
#else
	if(dmode == MODE_STD) {
		dmode = std_round_fix(x);
		numdig = DISPLAY_DIGITS;
	}
#endif

	if(dmode == MODE_FIX)
		/* FIX is different since the number of digits changes */
		return decNumberRoundDecimals(res, x, numdig-1, DEC_ROUND_HALF_UP);

	return decNumberRoundDigits(res, x, numdig, DEC_ROUND_HALF_UP);
}

decNumber *decNumberRoundDecimals(decNumber *r, const decNumber *x, const int n, const enum rounding round) {
	decNumber t;
#if 0
	/* The slow but always correct way */
	decNumber u, p10;

	int_to_dn(&u, n);
	decNumberPow10(&p10, &u);
	dn_multiply(&t, x, &p10);
	round2int(&u, &t, round);
	return dn_divide(r, &u, &p10);
#else
	/* The much faster way but relying on base 10 numbers with exponents */
	if(decNumberIsSpecial(x))
		return decNumberCopy(r, x);

	decNumberCopy(&t, x);
	t.exponent += n;
	round2int(r, &t, round);
	r->exponent -= n;
	return r;
#endif
}

static decNumber *gser(decNumber *res, const decNumber *a, const decNumber *x, const decNumber *gln) {
	decNumber ap, del, sum, t, u;
	int i;

	if(dn_le0(x))
		return decNumberZero(res);
	decNumberCopy(&ap, a);
	decNumberRecip(&sum, a);
	decNumberCopy(&del, &sum);
	for(i=0; i<1000; i++) {
		dn_inc(&ap);
		dn_divide(&t, x, &ap);
		dn_multiply(&del, &del, &t);
		dn_add(&t, &sum, &del);
		if(dn_eq(&t, &sum))
			break;
		decNumberCopy(&sum, &t);
	}
	dn_ln(&t, x);
	dn_multiply(&u, &t, a);
	dn_subtract(&t, &u, x);
	dn_subtract(&u, &t, gln);
	dn_exp(&t, &u);
	return dn_multiply(res, &sum, &t);
}

static void gcheckSmall(decNumber *v)
{
	const decNumber * const threshold = &const_1e_10000;

	if(dn_abs_lt(v, threshold))
		decNumberCopy(v, threshold);
}

static decNumber *gcf(decNumber *res, const decNumber *a, const decNumber *x, const decNumber *gln) {
	decNumber an, b, c, d, h, t, u, v, i;
	int n;

	dn_p1(&t, x);
	gcheckSmall(dn_subtract(&b, &t, a));		// b = (x+1) - a
	set_inf(&c);
	decNumberRecip(&d, &b);
	decNumberCopy(&h, &d);
	decNumberZero(&i);
	for(n=0; n<1000; n++) {
		dn_inc(&i);
		dn_subtract(&t, a, &i);		// t = a-i
		dn_multiply(&an, &i, &t);		// an = -i (i-a)
		dn_p2(&b, &b);
		dn_multiply(&t, &an, &d);
		dn_add(&v, &t, &b);
		gcheckSmall(&v);
		decNumberRecip(&d, &v);
		dn_divide(&t, &an, &c);
		dn_add(&c, &b, &t);
		gcheckSmall(&c);
		dn_multiply(&t, &d, &c);
		dn_multiply(&u, &h, &t);
		if(dn_eq(&h, &u))
			break;
		decNumberCopy(&h, &u);
	}
	dn_ln(&t, x);
	dn_multiply(&u, &t, a);
	dn_subtract(&t, &u, x);
	dn_subtract(&u, &t, gln);
	dn_exp(&t, &u);
	return dn_multiply(res, &t, &h);
}

decNumber *decNumberGammap(decNumber *res, const decNumber *x, const decNumber *a) {
	decNumber z, lga;
	const int op = XeqOpCode - (OP_DYA | OP_GAMMAg);
	const int regularised = op & 2;
	const int upper = op & 1;

	if(decNumberIsNegative(x) || dn_le0(a) ||
			decNumberIsNaN(x) || decNumberIsNaN(a) || decNumberIsInfinite(a)) {
		return set_NaN(res);
	}
	if(decNumberIsInfinite(x)) {
		if(upper) {
			if(regularised)
				return dn_1(res);
			return decNumberGamma(res, a);
		}
		return decNumberZero(res);
	}

	dn_p1(&lga, a);
	dn_compare(&z, x, &lga);
	if(regularised)
		decNumberLnGamma(&lga, a);
	else
		decNumberZero(&lga);
	if(decNumberIsNegative(&z)) {
		/* Deal with a difficult case by using the other expansion */
		if(dn_gt(a, &const_9000) && dn_gt(x, dn_multiply(&z, a, &const_0_995)))
			goto use_cf;
		gser(res, a, x, &lga);
		if(upper)
			goto invert;
	} else {
use_cf:		gcf(res, a, x, &lga);
		if(! upper)
			goto invert;
	}
	return res;

invert:	if(regularised)
		return dn_1m(res, res);
	decNumberGamma(&z, a);
	return dn_subtract(res, &z, res);
}

#ifdef INCLUDE_FACTOR
decNumber *decFactor(decNumber *r, const decNumber *x) {
	int sgn;
	unsigned long long int i;

	if(decNumberIsSpecial(x) || ! is_int(x))
		return set_NaN(r);
	i = dn_to_ull(x, &sgn);
	ullint_to_dn(r, doFactor(i));
	if(sgn)
		dn_minus(r, r);
	return r;
}
#endif

#ifdef INCLUDE_USER_IO
decNumber *decRecv(decNumber *r, const decNumber *x) {
	int to;

	if(decNumberIsSpecial(x) || decNumberIsNegative(x)) {
		to = -1;
		if(decNumberIsInfinite(x) && ! decNumberIsNegative(x))
			to = 0x7fffffff;
	} else
		to = dn_to_int(x);
	int_to_dn(r, recv_byte(to));
	return r;
}
#endif
#endif
