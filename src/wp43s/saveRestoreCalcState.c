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
 * \file saveRestoreCalcState.c
 ***********************************************/

#include "wp43s.h"

#ifdef PC_BUILD
#define BACKUP_VERSION 3  // 3 = displayAngularMode

void saveCalc(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("saveCalc");
  #endif

  size_t size;
  uint32_t backupVersion = BACKUP_VERSION;
  FILE *backup;

  backup = fopen("backup.bin", "wb");
  if (backup == NULL) {
    printf("Cannot save calc's memory in file backup.bin!\n");
    exit(0);
  }

  if(calcMode == CM_CONFIRMATION) {
    calcMode = previousCalcMode;
    refreshRegisterLine(REGISTER_X);
  }

  printf("Begin of calc's backup\n");


  size  = fwrite(&backupVersion,                      1, sizeof(backupVersion),                      backup); //printf("%8lu backupVersion\n",                      (unsigned long)size);
  size += fwrite(ram,                                 1, RAM_SIZE,                                   backup); //printf("%8lu ram\n",                                (unsigned long)size);
  size += fwrite(flags,                               1, sizeof(flags),                              backup); //printf("%8lu flags\n",                              (unsigned long)size);
  size += fwrite(tmpStr3000,                          1, TMP_STR_LENGTH,                             backup); //printf("%8lu tmpStr3000\n",                         (unsigned long)size);
  size += fwrite(errorMessage,                        1, ERROR_MESSAGE_LENGTH,                       backup); //printf("%8lu errorMessage\n",                       (unsigned long)size);
  size += fwrite(aimBuffer,                           1, AIM_BUFFER_LENGTH,                          backup); //printf("%8lu aimBuffer\n",                          (unsigned long)size);
  size += fwrite(nimBuffer,                           1, NIM_BUFFER_LENGTH,                          backup); //printf("%8lu nimBuffer\n",                          (unsigned long)size);
  size += fwrite(nimBufferDisplay,                    1, NIM_BUFFER_LENGTH,                          backup); //printf("%8lu nimBufferDisplay\n",                   (unsigned long)size);
  size += fwrite(tamBuffer,                           1, TAM_BUFFER_LENGTH,                          backup); //printf("%8lu tamBuffer\n",                          (unsigned long)size);
  size += fwrite(oldTime,                             1, 8,                                          backup); //printf("%8lu oldTime\n",                            (unsigned long)size);
  size += fwrite(dateTimeString,                      1, 12,                                         backup); //printf("%8lu dateTimeString\n",                     (unsigned long)size);
  size += fwrite(softmenuStack,                       1, sizeof(softmenuStack),                      backup); //printf("%8lu softmenuStack\n",                      (unsigned long)size);
  size += fwrite(reg,                                 1, sizeof(reg),                                backup); //printf("%8lu reg\n",                                (unsigned long)size);
  size += fwrite(savedStackRegister,                  1, sizeof(savedStackRegister),                 backup); //printf("%8lu savedStackRegister\n",                 (unsigned long)size);
  size += fwrite(tempRegister,                        1, sizeof(tempRegister),                       backup); //printf("%8lu tempRegister\n",                       (unsigned long)size);
  size += fwrite(kbd_usr,                             1, sizeof(kbd_usr),                            backup); //printf("%8lu kbd_usr\n",                            (unsigned long)size);
  size += fwrite(row,                                 1, sizeof(row),                                backup); //printf("%8lu row\n",                                (unsigned long)size);
  size += fwrite(transitionSystemOperation,           1, 4,                                          backup); //printf("%8lu transitionSystemOperation\n",          (unsigned long)size);
  size += fwrite(screenData,                          1, 4 * screenStride * SCREEN_HEIGHT,           backup); //printf("%8lu screenData\n",                         (unsigned long)size);
  size += fwrite(&tamFunction,                        1, sizeof(tamFunction),                        backup); //printf("%8lu tamFunction\n",                        (unsigned long)size);
  size += fwrite(&tamNumber,                          1, sizeof(tamNumber),                          backup); //printf("%8lu tamNumber\n",                          (unsigned long)size);
  size += fwrite(&tamNumberMin,                       1, sizeof(tamNumberMin),                       backup); //printf("%8lu tamNumberMin\n",                       (unsigned long)size);
  size += fwrite(&tamNumberMax,                       1, sizeof(tamNumberMax),                       backup); //printf("%8lu tamNumberMax\n",                       (unsigned long)size);
  size += fwrite(&tamDigit,                           1, sizeof(tamDigit),                           backup); //printf("%8lu tamDigit\n",                           (unsigned long)size);
  size += fwrite(&tamOperation,                       1, sizeof(tamOperation),                       backup); //printf("%8lu tamOperation\n",                       (unsigned long)size);
  size += fwrite(&tamLetteredRegister,                1, sizeof(tamLetteredRegister),                backup); //printf("%8lu tamLetteredRegister\n",                (unsigned long)size);
  size += fwrite(&tamCurrentOperation,                1, sizeof(tamCurrentOperation),                backup); //printf("%8lu tamCurrentOperation\n",                (unsigned long)size);
  size += fwrite(&result,                             1, sizeof(result),                             backup); //printf("%8lu result\n",                             (unsigned long)size);
  size += fwrite(&op1,                                1, sizeof(op1),                                backup); //printf("%8lu op1\n",                                (unsigned long)size);
  size += fwrite(&op2,                                1, sizeof(op2),                                backup); //printf("%8lu op2\n",                                (unsigned long)size);
  size += fwrite(&numberOfLocalRegisters,             1, sizeof(numberOfLocalRegisters),             backup); //printf("%8lu numberOfLocalRegisters\n",             (unsigned long)size);
  size += fwrite(&numberOfNamedRegisters,             1, sizeof(numberOfNamedRegisters),             backup); //printf("%8lu numberOfNamedRegisters\n",             (unsigned long)size);
  size += fwrite(&allLocalRegisterPointer,            1, sizeof(allLocalRegisterPointer),            backup); //printf("%8lu allLocalRegisterPointer\n",            (unsigned long)size);
  size += fwrite(&allNamedRegisterPointer,            1, sizeof(allNamedRegisterPointer),            backup); //printf("%8lu allNamedRegisterPointer\n",            (unsigned long)size);
  size += fwrite(&statisticalSumsPointer,             1, sizeof(statisticalSumsPointer),             backup); //printf("%8lu statisticalSumsPointer\n",             (unsigned long)size);
  size += fwrite(&firstFreeByte,                      1, sizeof(firstFreeByte),                      backup); //printf("%8lu firstFreeByte\n",                      (unsigned long)size);
  size += fwrite(&lastFreeByte,                       1, sizeof(lastFreeByte),                       backup); //printf("%8lu lastFreeByte\n",                       (unsigned long)size);
  size += fwrite(&programCounter,                     1, sizeof(programCounter),                     backup); //printf("%8lu programCounter\n",                     (unsigned long)size);
  size += fwrite(&xCursor,                            1, sizeof(xCursor),                            backup); //printf("%8lu xCursor\n",                            (unsigned long)size);
  size += fwrite(&yCursor,                            1, sizeof(yCursor),                            backup); //printf("%8lu yCursor\n",                            (unsigned long)size);
  size += fwrite(&firstGregorianDay,                  1, sizeof(firstGregorianDay),                  backup); //printf("%8lu firstGregorianDay\n",                  (unsigned long)size);
  size += fwrite(&denMax,                             1, sizeof(denMax),                             backup); //printf("%8lu denMax\n",                             (unsigned long)size);
  size += fwrite(&softmenuStackPointer,               1, sizeof(softmenuStackPointer),               backup); //printf("%8lu softmenuStackPointer\n",               (unsigned long)size);
  size += fwrite(&transitionSystemStatus,             1, sizeof(transitionSystemStatus),             backup); //printf("%8lu transitionSystemStatus\n",             (unsigned long)size);
  size += fwrite(&cursorBlinkCounter,                 1, sizeof(cursorBlinkCounter),                 backup); //printf("%8lu cursorBlinkCounter\n",                 (unsigned long)size);
  size += fwrite(&currentRegisterBrowserScreen,       1, sizeof(currentRegisterBrowserScreen),       backup); //printf("%8lu currentRegisterBrowserScreen\n",       (unsigned long)size);
  size += fwrite(&currentFntScr,                      1, sizeof(currentFntScr),                      backup); //printf("%8lu currentFntScr\n",                      (unsigned long)size);
  size += fwrite(&currentFlgScr,                      1, sizeof(currentFlgScr),                      backup); //printf("%8lu currentFlgScr\n",                      (unsigned long)size);
  size += fwrite(&displayFormat,                      1, sizeof(displayFormat),                      backup); //printf("%8lu displayFormat\n",                      (unsigned long)size);
  size += fwrite(&displayFormatDigits,                1, sizeof(displayFormatDigits),                backup); //printf("%8lu displayFormatDigits\n",                (unsigned long)size);
  size += fwrite(&smallIntegerWordSize,               1, sizeof(smallIntegerWordSize),               backup); //printf("%8lu smallIntegerWordSize\n",               (unsigned long)size);
  size += fwrite(&denominatorMode,                    1, sizeof(denominatorMode),                    backup); //printf("%8lu denominatorMode\n",                    (unsigned long)size);
  size += fwrite(&significantDigits,                  1, sizeof(significantDigits),                  backup); //printf("%8lu significantDigits\n",                  (unsigned long)size);
  size += fwrite(&smallIntegerMode,                   1, sizeof(smallIntegerMode),                   backup); //printf("%8lu smallIntegerMode\n",                   (unsigned long)size);
  size += fwrite(&angularMode,                        1, sizeof(angularMode),                        backup); //printf("%8lu angularMode\n",                        (unsigned long)size);
  size += fwrite(&displayAngularMode,                 1, sizeof(displayAngularMode),                 backup); //printf("%8lu displayAngularMode\n",                 (unsigned long)size);
  size += fwrite(&groupingGap,                        1, sizeof(groupingGap),                        backup); //printf("%8lu groupingGap\n",                        (unsigned long)size);
  size += fwrite(&dateFormat,                         1, sizeof(dateFormat),                         backup); //printf("%8lu dateFormat\n",                         (unsigned long)size);
  size += fwrite(&curveFitting,                       1, sizeof(curveFitting),                       backup); //printf("%8lu curveFitting\n",                       (unsigned long)size);
  size += fwrite(&roundingMode,                       1, sizeof(roundingMode),                       backup); //printf("%8lu roundingMode\n",                       (unsigned long)size);
  size += fwrite(&calcMode,                           1, sizeof(calcMode),                           backup); //printf("%8lu calcMode\n",                           (unsigned long)size);
  size += fwrite(&nextChar,                           1, sizeof(nextChar),                           backup); //printf("%8lu nextChar\n",                           (unsigned long)size);
  size += fwrite(&complexUnit,                        1, sizeof(complexUnit),                        backup); //printf("%8lu complexUnit\n",                        (unsigned long)size);
  size += fwrite(&displayLeadingZeros,                1, sizeof(displayLeadingZeros),                backup); //printf("%8lu displayLeadingZeros\n",                (unsigned long)size);
  size += fwrite(&productSign,                        1, sizeof(productSign),                        backup); //printf("%8lu productSign\n",                        (unsigned long)size);
  size += fwrite(&fractionType,                       1, sizeof(fractionType),                       backup); //printf("%8lu fractionType\n",                       (unsigned long)size);
  size += fwrite(&radixMark,                          1, sizeof(radixMark),                          backup); //printf("%8lu radixMark\n",                          (unsigned long)size);
  size += fwrite(&displayModeOverride,                1, sizeof(displayModeOverride),                backup); //printf("%8lu displayModeOverride\n",                (unsigned long)size);
  size += fwrite(&stackSize,                          1, sizeof(stackSize),                          backup); //printf("%8lu stackSize\n",                          (unsigned long)size);
  size += fwrite(&complexMode,                        1, sizeof(complexMode),                        backup); //printf("%8lu complexMode\n",                        (unsigned long)size);
  size += fwrite(&alphaCase,                          1, sizeof(alphaCase),                          backup); //printf("%8lu alphaCase\n",                          (unsigned long)size);
  size += fwrite(&hourGlassIconEnabled,               1, sizeof(hourGlassIconEnabled),               backup); //printf("%8lu hourGlassIconEnabled\n",               (unsigned long)size);
  size += fwrite(&watchIconEnabled,                   1, sizeof(watchIconEnabled),                   backup); //printf("%8lu watchIconEnabled\n",                   (unsigned long)size);
  size += fwrite(&userModeEnabled,                    1, sizeof(userModeEnabled),                    backup); //printf("%8lu userModeEnabled\n",                    (unsigned long)size);
  size += fwrite(&serialIOIconEnabled,                1, sizeof(serialIOIconEnabled),                backup); //printf("%8lu serialIOIconEnabled\n",                (unsigned long)size);
  size += fwrite(&printerIconEnabled,                 1, sizeof(printerIconEnabled),                 backup); //printf("%8lu printerIconEnabled\n",                 (unsigned long)size);
  size += fwrite(&batteryIconEnabled,                 1, sizeof(batteryIconEnabled),                 backup); //printf("%8lu batteryIconEnabled\n",                 (unsigned long)size);
  size += fwrite(&cursorEnabled,                      1, sizeof(cursorEnabled),                      backup); //printf("%8lu cursorEnabled\n",                      (unsigned long)size);
  size += fwrite(&cursorFont,                         1, sizeof(cursorFont),                         backup); //printf("%8lu cursorFont\n",                         (unsigned long)size);
  size += fwrite(&stackLiftEnabled,                   1, sizeof(stackLiftEnabled),                   backup); //printf("%8lu stackLiftEnabled\n",                   (unsigned long)size);
  size += fwrite(&savedStackLiftEnabled,              1, sizeof(savedStackLiftEnabled),              backup); //printf("%8lu savedStackLiftEnabled\n",              (unsigned long)size);
  size += fwrite(&shiftF,                             1, sizeof(shiftF),                             backup); //printf("%8lu shiftF\n",                             (unsigned long)size);
  size += fwrite(&shiftG,                             1, sizeof(shiftG),                             backup); //printf("%8lu shiftG\n",                             (unsigned long)size);
  //size += fwrite(&displayDateTime,                    1, sizeof(displayDateTime),                    backup); //printf("%8lu displayDateTime\n",                    (unsigned long)size);
  size += fwrite(&timeFormat,                         1, sizeof(timeFormat),                         backup); //printf("%8lu timeFormat\n",                         (unsigned long)size);
  size += fwrite(&tamMode,                            1, sizeof(tamMode),                            backup); //printf("%8lu tamMode\n",                            (unsigned long)size);
  size += fwrite(&rbrMode,                            1, sizeof(rbrMode),                            backup); //printf("%8lu rbrMode\n",                            (unsigned long)size);
  size += fwrite(&showContent,                        1, sizeof(showContent),                        backup); //printf("%8lu showContent\n",                        (unsigned long)size);
  size += fwrite(&numScreensNumericFont,              1, sizeof(numScreensNumericFont),              backup); //printf("%8lu numScreensNumericFont\n",              (unsigned long)size);
  size += fwrite(&numLinesNumericFont,                1, sizeof(numLinesNumericFont),                backup); //printf("%8lu numLinesNumericFont\n",                (unsigned long)size);
  size += fwrite(&numLinesStandardFont,               1, sizeof(numLinesStandardFont),               backup); //printf("%8lu numLinesStandardFont\n",               (unsigned long)size);
  size += fwrite(&numScreensStandardFont,             1, sizeof(numScreensStandardFont),             backup); //printf("%8lu numScreensStandardFont\n",             (unsigned long)size);
  size += fwrite(&previousCalcMode,                   1, sizeof(previousCalcMode),                   backup); //printf("%8lu previousCalcMode\n",                   (unsigned long)size);
  size += fwrite(&displayRealAsFraction,              1, sizeof(displayRealAsFraction),              backup); //printf("%8lu displayRealAsFraction\n",              (unsigned long)size);
  size += fwrite(&lastErrorCode,                      1, sizeof(lastErrorCode),                      backup); //printf("%8lu lastErrorCode\n",                      (unsigned long)size);
  size += fwrite(&nimNumberPart,                      1, sizeof(nimNumberPart),                      backup); //printf("%8lu nimNumberPart\n",                      (unsigned long)size);
  size += fwrite(&displayStack,                       1, sizeof(displayStack),                       backup); //printf("%8lu displayStack\n",                       (unsigned long)size);
  size += fwrite(&hexDigits,                          1, sizeof(hexDigits),                          backup); //printf("%8lu hexDigits\n",                          (unsigned long)size);
  size += fwrite(&errorMessageRegisterLine,           1, sizeof(errorMessageRegisterLine),           backup); //printf("%8lu errorMessageRegisterLine\n",           (unsigned long)size);
  size += fwrite(&errorRegisterLine,                  1, sizeof(errorRegisterLine),                  backup); //printf("%8lu errorRegisterLine\n",                  (unsigned long)size);
  size += fwrite(&smallIntegerMask,                   1, sizeof(smallIntegerMask),                   backup); //printf("%8lu smallIntegerMask\n",                   (unsigned long)size);
  size += fwrite(&smallIntegerSignBit,                1, sizeof(smallIntegerSignBit),                backup); //printf("%8lu smallIntegerSignBit\n",                (unsigned long)size);
  size += fwrite(&temporaryInformation,               1, sizeof(temporaryInformation),               backup); //printf("%8lu temporaryInformation\n",               (unsigned long)size);
  size += fwrite(&glyphNotFound,                      1, sizeof(glyphNotFound),                      backup); //printf("%8lu glyphNotFound\n",                      (unsigned long)size);
  size += fwrite(&allowScreenUpdate,                  1, sizeof(allowScreenUpdate),                  backup); //printf("%8lu allowScreenUpdate\n",                  (unsigned long)size);
  size += fwrite(&funcOK,                             1, sizeof(funcOK),                             backup); //printf("%8lu funcOK\n",                             (unsigned long)size);
  size += fwrite(&screenChange,                       1, sizeof(screenChange),                       backup); //printf("%8lu screenChange\n",                       (unsigned long)size);
  size += fwrite(&exponentSignLocation,               1, sizeof(exponentSignLocation),               backup); //printf("%8lu exponentSignLocation\n",               (unsigned long)size);
  size += fwrite(&imaginaryExponentSignLocation,      1, sizeof(imaginaryExponentSignLocation),      backup); //printf("%8lu imaginaryExponentSignLocation\n",      (unsigned long)size);
  size += fwrite(&imaginaryMantissaSignLocation,      1, sizeof(imaginaryMantissaSignLocation),      backup); //printf("%8lu imaginaryMantissaSignLocation\n",      (unsigned long)size);

  printf("%" FMT32U " bytes saved\n", (uint32_t)size);

  fclose(backup);
  printf("End of calc's backup\n");

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("saveCalc");
  #endif
}



void restoreCalc(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("restoreCalc");
  #endif

  size_t size;
  uint32_t backupVersion;
  FILE *backup;

  backup = fopen("backup.bin", "rb");
  if (backup == NULL) {
    printf("Cannot restore calc's memory from file backup.bin! Performing RESET\n");
    fnReset(CONFIRMED);

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("restoreCalc");
    #endif

    return;
  }

  size  = fread(&backupVersion,                      1, sizeof(backupVersion),                      backup); //printf("%8lu backupVersion\n",                      (unsigned long)size);
  if(backupVersion != BACKUP_VERSION) {
    fclose(backup);

    printf("Cannot restore calc's memory from file backup.bin! File backup.bin is from another backup version. Performing RESET\n");
    fnReset(CONFIRMED);

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("restoreCalc");
    #endif

    return;
  }
  else {
    printf("Begin of calc's restoration\n");

    size += fread(ram,                                 1, RAM_SIZE,                                   backup); //printf("%8lu ram\n",                                (unsigned long)size);
    size += fread(flags,                               1, sizeof(flags),                              backup); //printf("%8lu flags\n",                              (unsigned long)size);
    size += fread(tmpStr3000,                          1, TMP_STR_LENGTH,                             backup); //printf("%8lu tmpStr3000\n",                         (unsigned long)size);
    size += fread(errorMessage,                        1, ERROR_MESSAGE_LENGTH,                       backup); //printf("%8lu errorMessage\n",                       (unsigned long)size);
    size += fread(aimBuffer,                           1, AIM_BUFFER_LENGTH,                          backup); //printf("%8lu aimBuffer\n",                          (unsigned long)size);
    size += fread(nimBuffer,                           1, NIM_BUFFER_LENGTH,                          backup); //printf("%8lu nimBuffer\n",                          (unsigned long)size);
    size += fread(nimBufferDisplay,                    1, NIM_BUFFER_LENGTH,                          backup); //printf("%8lu nimBufferDisplay\n",                   (unsigned long)size);
    size += fread(tamBuffer,                           1, TAM_BUFFER_LENGTH,                          backup); //printf("%8lu tamBuffer\n",                          (unsigned long)size);
    size += fread(oldTime,                             1, 8,                                          backup); //printf("%8lu oldTime\n",                            (unsigned long)size);
    size += fread(dateTimeString,                      1, 12,                                         backup); //printf("%8lu dateTimeString\n",                     (unsigned long)size);
    size += fread(softmenuStack,                       1, sizeof(softmenuStack),                      backup); //printf("%8lu softmenuStack\n",                      (unsigned long)size);
    size += fread(reg,                                 1, sizeof(reg),                                backup); //printf("%8lu reg\n",                                (unsigned long)size);
    size += fread(savedStackRegister,                  1, sizeof(savedStackRegister),                 backup); //printf("%8lu savedStackRegister\n",                 (unsigned long)size);
    size += fread(tempRegister,                        1, sizeof(tempRegister),                       backup); //printf("%8lu tempRegister\n",                       (unsigned long)size);
    size += fread(kbd_usr,                             1, sizeof(kbd_usr),                            backup); //printf("%8lu kbd_usr\n",                            (unsigned long)size);
    size += fread(row,                                 1, sizeof(row),                                backup); //printf("%8lu row\n",                                (unsigned long)size);
    size += fread(transitionSystemOperation,           1, 4,                                          backup); //printf("%8lu transitionSystemOperation\n",          (unsigned long)size);
    size += fread(screenData,                          1, 4 * screenStride * SCREEN_HEIGHT,           backup); //printf("%8lu screenData\n",                         (unsigned long)size);
    size += fread(&tamFunction,                        1, sizeof(tamFunction),                        backup); //printf("%8lu tamFunction\n",                        (unsigned long)size);
    size += fread(&tamNumber    ,                      1, sizeof(tamNumber),                          backup); //printf("%8lu tamNumber\n",                          (unsigned long)size);
    size += fread(&tamNumberMin,                       1, sizeof(tamNumberMin),                       backup); //printf("%8lu tamNumberMin\n",                       (unsigned long)size);
    size += fread(&tamNumberMax,                       1, sizeof(tamNumberMax),                       backup); //printf("%8lu tamNumberMax\n",                       (unsigned long)size);
    size += fread(&tamDigit,                           1, sizeof(tamDigit),                           backup); //printf("%8lu tamDigit\n",                           (unsigned long)size);
    size += fread(&tamOperation,                       1, sizeof(tamOperation),                       backup); //printf("%8lu tamOperation\n",                       (unsigned long)size);
    size += fread(&tamLetteredRegister,                1, sizeof(tamLetteredRegister),                backup); //printf("%8lu tamLetteredRegister\n",                (unsigned long)size);
    size += fread(&tamCurrentOperation,                1, sizeof(tamCurrentOperation),                backup); //printf("%8lu tamCurrentOperation\n",                (unsigned long)size);
    size += fread(&result,                             1, sizeof(result),                             backup); //printf("%8lu result\n",                             (unsigned long)size);
    size += fread(&op1,                                1, sizeof(op1),                                backup); //printf("%8lu op1\n",                                (unsigned long)size);
    size += fread(&op2,                                1, sizeof(op2),                                backup); //printf("%8lu op2\n",                                (unsigned long)size);
    size += fread(&numberOfLocalRegisters,             1, sizeof(numberOfLocalRegisters),             backup); //printf("%8lu numberOfLocalRegisters\n",             (unsigned long)size);
    size += fread(&numberOfNamedRegisters,             1, sizeof(numberOfNamedRegisters),             backup); //printf("%8lu numberOfNamedRegisters\n",             (unsigned long)size);
    size += fread(&allLocalRegisterPointer,            1, sizeof(allLocalRegisterPointer),            backup); //printf("%8lu allLocalRegisterPointer\n",            (unsigned long)size);
    size += fread(&allNamedRegisterPointer,            1, sizeof(allNamedRegisterPointer),            backup); //printf("%8lu allNamedRegisterPointer\n",            (unsigned long)size);
    size += fread(&statisticalSumsPointer,             1, sizeof(statisticalSumsPointer),             backup); //printf("%8lu statisticalSumsPointer\n",             (unsigned long)size);
    size += fread(&firstFreeByte,                      1, sizeof(firstFreeByte),                      backup); //printf("%8lu firstFreeByte\n",                      (unsigned long)size);
    size += fread(&lastFreeByte,                       1, sizeof(lastFreeByte),                       backup); //printf("%8lu lastFreeByte\n",                       (unsigned long)size);
    size += fread(&programCounter,                     1, sizeof(programCounter),                     backup); //printf("%8lu programCounter\n",                     (unsigned long)size);
    size += fread(&xCursor,                            1, sizeof(xCursor),                            backup); //printf("%8lu xCursor\n",                            (unsigned long)size);
    size += fread(&yCursor,                            1, sizeof(yCursor),                            backup); //printf("%8lu yCursor\n",                            (unsigned long)size);
    size += fread(&firstGregorianDay,                  1, sizeof(firstGregorianDay),                  backup); //printf("%8lu firstGregorianDay\n",                  (unsigned long)size);
    size += fread(&denMax,                             1, sizeof(denMax),                             backup); //printf("%8lu denMax\n",                             (unsigned long)size);
    size += fread(&softmenuStackPointer,               1, sizeof(softmenuStackPointer),               backup); //printf("%8lu softmenuStackPointer\n",               (unsigned long)size);
    size += fread(&transitionSystemStatus,             1, sizeof(transitionSystemStatus),             backup); //printf("%8lu transitionSystemStatus\n",             (unsigned long)size);
    size += fread(&cursorBlinkCounter,                 1, sizeof(cursorBlinkCounter),                 backup); //printf("%8lu cursorBlinkCounter\n",                 (unsigned long)size);
    size += fread(&currentRegisterBrowserScreen,       1, sizeof(currentRegisterBrowserScreen),       backup); //printf("%8lu currentRegisterBrowserScreen\n",       (unsigned long)size);
    size += fread(&currentFntScr,                      1, sizeof(currentFntScr),                      backup); //printf("%8lu currentFntScr\n",                      (unsigned long)size);
    size += fread(&currentFlgScr,                      1, sizeof(currentFlgScr),                      backup); //printf("%8lu currentFlgScr\n",                      (unsigned long)size);
    size += fread(&displayFormat,                      1, sizeof(displayFormat),                      backup); //printf("%8lu displayFormat\n",                      (unsigned long)size);
    size += fread(&displayFormatDigits,                1, sizeof(displayFormatDigits),                backup); //printf("%8lu displayFormatDigits\n",                (unsigned long)size);
    size += fread(&smallIntegerWordSize,               1, sizeof(smallIntegerWordSize),               backup); //printf("%8lu smallIntegerWordSize\n",               (unsigned long)size);
    size += fread(&denominatorMode,                    1, sizeof(denominatorMode),                    backup); //printf("%8lu denominatorMode\n",                    (unsigned long)size);
    size += fread(&significantDigits,                  1, sizeof(significantDigits),                  backup); //printf("%8lu significantDigits\n",                  (unsigned long)size);
    size += fread(&smallIntegerMode,                   1, sizeof(smallIntegerMode),                   backup); //printf("%8lu smallIntegerMode\n",                   (unsigned long)size);
    size += fread(&angularMode,                        1, sizeof(angularMode),                        backup); //printf("%8lu angularMode\n",                        (unsigned long)size);
    size += fread(&displayAngularMode,                 1, sizeof(displayAngularMode),                 backup); //printf("%8lu displayAngularMode\n",                 (unsigned long)size);
    size += fread(&groupingGap,                        1, sizeof(groupingGap),                        backup); //printf("%8lu groupingGap\n",                        (unsigned long)size);
    size += fread(&dateFormat,                         1, sizeof(dateFormat),                         backup); //printf("%8lu dateFormat\n",                         (unsigned long)size);
    size += fread(&curveFitting,                       1, sizeof(curveFitting),                       backup); //printf("%8lu curveFitting\n",                       (unsigned long)size);
    size += fread(&roundingMode,                       1, sizeof(roundingMode),                       backup); //printf("%8lu roundingMode\n",                       (unsigned long)size);
    size += fread(&calcMode,                           1, sizeof(calcMode),                           backup); //printf("%8lu calcMode\n",                           (unsigned long)size);
    size += fread(&nextChar,                           1, sizeof(nextChar),                           backup); //printf("%8lu nextChar\n",                           (unsigned long)size);
    size += fread(&complexUnit,                        1, sizeof(complexUnit),                        backup); //printf("%8lu complexUnit\n",                        (unsigned long)size);
    size += fread(&displayLeadingZeros,                1, sizeof(displayLeadingZeros),                backup); //printf("%8lu displayLeadingZeros\n",                (unsigned long)size);
    size += fread(&productSign,                        1, sizeof(productSign),                        backup); //printf("%8lu productSign\n",                        (unsigned long)size);
    size += fread(&fractionType,                       1, sizeof(fractionType),                       backup); //printf("%8lu fractionType\n",                       (unsigned long)size);
    size += fread(&radixMark,                          1, sizeof(radixMark),                          backup); //printf("%8lu radixMark\n",                          (unsigned long)size);
    size += fread(&displayModeOverride,                1, sizeof(displayModeOverride),                backup); //printf("%8lu displayModeOverride\n",                (unsigned long)size);
    size += fread(&stackSize,                          1, sizeof(stackSize),                          backup); //printf("%8lu stackSize\n",                          (unsigned long)size);
    size += fread(&complexMode,                        1, sizeof(complexMode),                        backup); //printf("%8lu complexMode\n",                        (unsigned long)size);
    size += fread(&alphaCase,                          1, sizeof(alphaCase),                          backup); //printf("%8lu alphaCase\n",                          (unsigned long)size);
    size += fread(&hourGlassIconEnabled,               1, sizeof(hourGlassIconEnabled),               backup); //printf("%8lu hourGlassIconEnabled\n",               (unsigned long)size);
    size += fread(&watchIconEnabled,                   1, sizeof(watchIconEnabled),                   backup); //printf("%8lu watchIconEnabled\n",                   (unsigned long)size);
    size += fread(&userModeEnabled,                    1, sizeof(userModeEnabled),                    backup); //printf("%8lu userModeEnabled\n",                    (unsigned long)size);
    size += fread(&serialIOIconEnabled,                1, sizeof(serialIOIconEnabled),                backup); //printf("%8lu serialIOIconEnabled\n",                (unsigned long)size);
    size += fread(&printerIconEnabled,                 1, sizeof(printerIconEnabled),                 backup); //printf("%8lu printerIconEnabled\n",                 (unsigned long)size);
    size += fread(&batteryIconEnabled,                 1, sizeof(batteryIconEnabled),                 backup); //printf("%8lu batteryIconEnabled\n",                 (unsigned long)size);
    size += fread(&cursorEnabled,                      1, sizeof(cursorEnabled),                      backup); //printf("%8lu cursorEnabled\n",                      (unsigned long)size);
    size += fread(&cursorFont,                         1, sizeof(cursorFont),                         backup); //printf("%8lu cursorFont\n",                         (unsigned long)size);
    size += fread(&stackLiftEnabled,                   1, sizeof(stackLiftEnabled),                   backup); //printf("%8lu stackLiftEnabled\n",                   (unsigned long)size);
    size += fread(&savedStackLiftEnabled,              1, sizeof(savedStackLiftEnabled),              backup); //printf("%8lu savedStackLiftEnabled\n",              (unsigned long)size);
    size += fread(&shiftF,                             1, sizeof(shiftF),                             backup); //printf("%8lu shiftF\n",                             (unsigned long)size);
    size += fread(&shiftG,                             1, sizeof(shiftG),                             backup); //printf("%8lu shiftG\n",                             (unsigned long)size);
    //size += fread(&displayDateTime,                    1, sizeof(displayDateTime),                    backup); //printf("%8lu displayDateTime\n",                    (unsigned long)size);
    size += fread(&timeFormat,                         1, sizeof(timeFormat),                         backup); //printf("%8lu timeFormat\n",                         (unsigned long)size);
    size += fread(&tamMode,                            1, sizeof(tamMode),                            backup); //printf("%8lu tamMode\n",                            (unsigned long)size);
    size += fread(&rbrMode,                            1, sizeof(rbrMode),                            backup); //printf("%8lu rbrMode\n",                            (unsigned long)size);
    size += fread(&showContent,                        1, sizeof(showContent),                        backup); //printf("%8lu showContent\n",                        (unsigned long)size);
    size += fread(&numScreensNumericFont,              1, sizeof(numScreensNumericFont),              backup); //printf("%8lu numScreensNumericFont\n",              (unsigned long)size);
    size += fread(&numLinesNumericFont,                1, sizeof(numLinesNumericFont),                backup); //printf("%8lu numLinesNumericFont\n",                (unsigned long)size);
    size += fread(&numLinesStandardFont,               1, sizeof(numLinesStandardFont),               backup); //printf("%8lu numLinesStandardFont\n",               (unsigned long)size);
    size += fread(&numScreensStandardFont,             1, sizeof(numScreensStandardFont),             backup); //printf("%8lu numScreensStandardFont\n",             (unsigned long)size);
    size += fread(&previousCalcMode,                   1, sizeof(previousCalcMode),                   backup); //printf("%8lu previousCalcMode\n",                   (unsigned long)size);
    size += fread(&displayRealAsFraction,              1, sizeof(displayRealAsFraction),              backup); //printf("%8lu displayRealAsFraction\n",              (unsigned long)size);
    size += fread(&lastErrorCode,                      1, sizeof(lastErrorCode),                      backup); //printf("%8lu lastErrorCode\n",                      (unsigned long)size);
    size += fread(&nimNumberPart,                      1, sizeof(nimNumberPart),                      backup); //printf("%8lu nimNumberPart\n",                      (unsigned long)size);
    size += fread(&displayStack,                       1, sizeof(displayStack),                       backup); //printf("%8lu displayStack\n",                       (unsigned long)size);
    size += fread(&hexDigits,                          1, sizeof(hexDigits),                          backup); //printf("%8lu hexDigits\n",                          (unsigned long)size);
    size += fread(&errorMessageRegisterLine,           1, sizeof(errorMessageRegisterLine),           backup); //printf("%8lu errorMessageRegisterLine\n",           (unsigned long)size);
    size += fread(&errorRegisterLine,                  1, sizeof(errorRegisterLine),                  backup); //printf("%8lu errorRegisterLine\n",                  (unsigned long)size);
    size += fread(&smallIntegerMask,                   1, sizeof(smallIntegerMask),                   backup); //printf("%8lu smallIntegerMask\n",                   (unsigned long)size);
    size += fread(&smallIntegerSignBit,                1, sizeof(smallIntegerSignBit),                backup); //printf("%8lu smallIntegerSignBit\n",                (unsigned long)size);
    size += fread(&temporaryInformation,               1, sizeof(temporaryInformation),               backup); //printf("%8lu temporaryInformation\n",               (unsigned long)size);

    size += fread(&glyphNotFound,                      1, sizeof(glyphNotFound),                      backup); //printf("%8lu glyphNotFound\n",                      (unsigned long)size);
    glyphNotFound.data   = malloc(38);
    strcpy(glyphNotFound.data, "\xff\xf8\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\xff\xf8");

    size += fread(&allowScreenUpdate,                  1, sizeof(allowScreenUpdate),                  backup); //printf("%8lu allowScreenUpdate\n",                  (unsigned long)size);
    size += fread(&funcOK,                             1, sizeof(funcOK),                             backup); //printf("%8lu funcOK\n",                             (unsigned long)size);
    size += fread(&screenChange,                       1, sizeof(screenChange),                       backup); //printf("%8lu screenChange\n",                       (unsigned long)size);
    size += fread(&exponentSignLocation,               1, sizeof(exponentSignLocation),               backup); //printf("%8lu exponentSignLocation\n",               (unsigned long)size);
    size += fread(&imaginaryExponentSignLocation,      1, sizeof(imaginaryExponentSignLocation),      backup); //printf("%8lu imaginaryExponentSignLocation\n",      (unsigned long)size);
    size += fread(&imaginaryMantissaSignLocation,      1, sizeof(imaginaryMantissaSignLocation),      backup); //printf("%8lu imaginaryMantissaSignLocation\n",      (unsigned long)size);

    printf("%" FMT32U " bytes restored\n", (uint32_t)size);

    fclose(backup);
    printf("End of calc's restoration\n");

    if(calcMode == CM_NORMAL)                calcModeNormalGui();
    else if(calcMode == CM_AIM)              calcModeAimGui();
    else if(calcMode == CM_TAM)              calcModeTamGui();
    else if(calcMode == CM_NIM)              calcModeNormalGui();
    else if(calcMode == CM_REGISTER_BROWSER) calcModeNormalGui();
    else if(calcMode == CM_FLAG_BROWSER)     calcModeNormalGui();
    else if(calcMode == CM_FONT_BROWSER)     calcModeNormalGui();
    else {
      sprintf(errorMessage, "In function restoreCalc: %" FMT8U " is an unexpected value for calcMode", calcMode);
      displayBugScreen(errorMessage);
    }

    getTimeString(dateTimeString);
    oldTime[0] = 0;

    if(stackLiftEnabled) {
      #if (STACK_LIFT_DEBUG == 1)
        stackLiftEnable();
      #else
        stackLiftEnabled = true;
      #endif
    }
    else {
      #if (STACK_LIFT_DEBUG == 1)
        stackLiftDisable();
      #else
        stackLiftEnabled = false;
      #endif
    }
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("restoreCalc");
  #endif
}
#endif