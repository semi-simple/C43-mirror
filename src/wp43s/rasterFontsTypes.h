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

typedef struct
 {
  uint16_t charCode;
  int16_t  rowsBeforeGlyph;
  int16_t  rowsGlyph;
  int16_t  rowsAfterGlyph;
  int16_t  linesAboveGlyph;
  int16_t  linesGlyph;
  int16_t  linesBelowGlyph;
  //int16_t  dataLength;
  char     *data;
 } glyph_t;

typedef struct
 {
  char    name[19];
  int16_t ascender;
  int16_t descender;
  int16_t numberOfGlyphs;
  glyph_t glyphs[];
 } font_t;
