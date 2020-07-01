/*
 * HEIF codec.
 * Copyright (c) 2020 struktur AG, Dirk Farin <farin@struktur.de>
 *
 * This file is part of libheif.
 *
 * libheif is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * libheif is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libheif.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "nclx.h"


heif::primaries::primaries(float gx, float gy, float bx, float by, float rx, float ry, float wx, float wy) {
  defined = true;
  redX = rx;
  redY = ry;
  greenX = gx;
  greenY = gy;
  blueX = bx;
  blueY = by;
  whiteX = wx;
  whiteY = wy;
}


heif::primaries heif::get_colour_primaries(uint16_t primaries_idx) {
  switch (primaries_idx) {
    case 1:
      return {0.300f, 0.600f, 0.150f, 0.060f, 0.640f, 0.330f, 0.3127f, 0.3290f};
    case 4:
      return {0.21f, 0.71f, 0.14f, 0.08f, 0.67f, 0.33f, 0.310f, 0.316f};
    case 5:
      return {0.29f, 0.60f, 0.15f, 0.06f, 0.64f, 0.33f, 0.3127f, 0.3290f};
    case 6:
    case 7:
      return {0.310f, 0.595f, 0.155f, 0.070f, 0.630f, 0.340f, 0.3127f, 0.3290f};
    case 8:
      return {0.243f, 0.692f, 0.145f, 0.049f, 0.681f, 0.319f, 0.310f, 0.316f};
    case 9:
      return {0.170f, 0.797f, 0.131f, 0.046f, 0.708f, 0.292f, 0.3127f, 0.3290f};
    case 10:
      return {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.333333f, 0.33333f};
    case 11:
      return {0.265f, 0.690f, 0.150f, 0.060f, 0.680f, 0.320f, 0.314f, 0.351f};
    case 12:
      return {0.265f, 0.690f, 0.150f, 0.060f, 0.680f, 0.320f, 0.3127f, 0.3290f};
    case 22:
      return {0.295f, 0.605f, 0.155f, 0.077f, 0.630f, 0.340f, 0.3127f, 0.3290f};
    default:
      return {};
  }
}


heif::Kr_Kb heif::get_Kr_Kb(uint16_t matrix_coefficients_idx, uint16_t primaries_idx) {
  Kr_Kb result;

  if (matrix_coefficients_idx == 12 ||
      matrix_coefficients_idx == 13) {

    primaries p = get_colour_primaries(primaries_idx);
    float zr = 1 - (p.redX + p.redY);
    float zg = 1 - (p.greenX + p.greenY);
    float zb = 1 - (p.blueX + p.blueY);
    float zw = 1 - (p.whiteX + p.whiteY);

    float denom = p.whiteY * (p.redX * (p.greenY * zb - p.blueY * zg) + p.greenX * (p.blueY * zr - p.redY * zb) +
                              p.blueX * (p.redY * zg - p.greenY * zr));

    result.Kr = (p.redY * (p.whiteX * (p.greenY * zb - p.blueY * zg) + p.whiteY * (p.blueX * zg - p.greenX * zb) +
                           zw * (p.greenX * p.blueY - p.blueX * p.greenY))) / denom;
    result.Kb = (p.blueY * (p.whiteX * (p.redY * zg - p.greenY * zr) + p.whiteY * (p.greenX * zr - p.redX * zg) +
                            zw * (p.redX * p.greenY - p.greenX * p.redY))) / denom;
  } else
    switch (matrix_coefficients_idx) {
      case 1:
        result.Kr = 0.2126f;
        result.Kb = 0.0722f;
      case 4:
        result.Kr = 0.30f;
        result.Kb = 0.11f;
      case 5:
      case 6:
        result.Kr = 0.299f;
        result.Kb = 0.114f;
      case 7:
        result.Kr = 0.212f;
        result.Kb = 0.087f;
      case 9:
      case 10:
        result.Kr = 0.2627f;
        result.Kb = 0.0593f;
    default:
      ;
    }

  return result;
}