/*
 * Copyright (c) 2016, Alliance for Open Media. All rights reserved
 *
 * This source code is subject to the terms of the BSD 2 Clause License and
 * the Alliance for Open Media Patent License 1.0. If the BSD 2 Clause License
 * was not distributed with this source code in the LICENSE file, you can
 * obtain it at www.aomedia.org/license/software. If the Alliance for Open
 * Media Patent License 1.0 was not distributed with this source code in the
 * PATENTS file, you can obtain it at www.aomedia.org/license/patent.
 */

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <assert.h>

#include "./av1_rtcd.h"
#include "av1/common/warped_motion.h"

/* clang-format off */
static const int error_measure_lut[512] = {
  // pow 0.7
  16384, 16339, 16294, 16249, 16204, 16158, 16113, 16068,
  16022, 15977, 15932, 15886, 15840, 15795, 15749, 15703,
  15657, 15612, 15566, 15520, 15474, 15427, 15381, 15335,
  15289, 15242, 15196, 15149, 15103, 15056, 15010, 14963,
  14916, 14869, 14822, 14775, 14728, 14681, 14634, 14587,
  14539, 14492, 14445, 14397, 14350, 14302, 14254, 14206,
  14159, 14111, 14063, 14015, 13967, 13918, 13870, 13822,
  13773, 13725, 13676, 13628, 13579, 13530, 13481, 13432,
  13383, 13334, 13285, 13236, 13187, 13137, 13088, 13038,
  12988, 12939, 12889, 12839, 12789, 12739, 12689, 12639,
  12588, 12538, 12487, 12437, 12386, 12335, 12285, 12234,
  12183, 12132, 12080, 12029, 11978, 11926, 11875, 11823,
  11771, 11719, 11667, 11615, 11563, 11511, 11458, 11406,
  11353, 11301, 11248, 11195, 11142, 11089, 11036, 10982,
  10929, 10875, 10822, 10768, 10714, 10660, 10606, 10552,
  10497, 10443, 10388, 10333, 10279, 10224, 10168, 10113,
  10058, 10002,  9947,  9891,  9835,  9779,  9723,  9666,
  9610, 9553, 9497, 9440, 9383, 9326, 9268, 9211,
  9153, 9095, 9037, 8979, 8921, 8862, 8804, 8745,
  8686, 8627, 8568, 8508, 8449, 8389, 8329, 8269,
  8208, 8148, 8087, 8026, 7965, 7903, 7842, 7780,
  7718, 7656, 7593, 7531, 7468, 7405, 7341, 7278,
  7214, 7150, 7086, 7021, 6956, 6891, 6826, 6760,
  6695, 6628, 6562, 6495, 6428, 6361, 6293, 6225,
  6157, 6089, 6020, 5950, 5881, 5811, 5741, 5670,
  5599, 5527, 5456, 5383, 5311, 5237, 5164, 5090,
  5015, 4941, 4865, 4789, 4713, 4636, 4558, 4480,
  4401, 4322, 4242, 4162, 4080, 3998, 3916, 3832,
  3748, 3663, 3577, 3490, 3402, 3314, 3224, 3133,
  3041, 2948, 2854, 2758, 2661, 2562, 2461, 2359,
  2255, 2148, 2040, 1929, 1815, 1698, 1577, 1452,
  1323, 1187, 1045,  894,  731,  550,  339,    0,
  339,  550,  731,  894, 1045, 1187, 1323, 1452,
  1577, 1698, 1815, 1929, 2040, 2148, 2255, 2359,
  2461, 2562, 2661, 2758, 2854, 2948, 3041, 3133,
  3224, 3314, 3402, 3490, 3577, 3663, 3748, 3832,
  3916, 3998, 4080, 4162, 4242, 4322, 4401, 4480,
  4558, 4636, 4713, 4789, 4865, 4941, 5015, 5090,
  5164, 5237, 5311, 5383, 5456, 5527, 5599, 5670,
  5741, 5811, 5881, 5950, 6020, 6089, 6157, 6225,
  6293, 6361, 6428, 6495, 6562, 6628, 6695, 6760,
  6826, 6891, 6956, 7021, 7086, 7150, 7214, 7278,
  7341, 7405, 7468, 7531, 7593, 7656, 7718, 7780,
  7842, 7903, 7965, 8026, 8087, 8148, 8208, 8269,
  8329, 8389, 8449, 8508, 8568, 8627, 8686, 8745,
  8804, 8862, 8921, 8979, 9037, 9095, 9153, 9211,
  9268, 9326, 9383, 9440, 9497, 9553, 9610, 9666,
  9723,  9779,  9835,  9891,  9947, 10002, 10058, 10113,
  10168, 10224, 10279, 10333, 10388, 10443, 10497, 10552,
  10606, 10660, 10714, 10768, 10822, 10875, 10929, 10982,
  11036, 11089, 11142, 11195, 11248, 11301, 11353, 11406,
  11458, 11511, 11563, 11615, 11667, 11719, 11771, 11823,
  11875, 11926, 11978, 12029, 12080, 12132, 12183, 12234,
  12285, 12335, 12386, 12437, 12487, 12538, 12588, 12639,
  12689, 12739, 12789, 12839, 12889, 12939, 12988, 13038,
  13088, 13137, 13187, 13236, 13285, 13334, 13383, 13432,
  13481, 13530, 13579, 13628, 13676, 13725, 13773, 13822,
  13870, 13918, 13967, 14015, 14063, 14111, 14159, 14206,
  14254, 14302, 14350, 14397, 14445, 14492, 14539, 14587,
  14634, 14681, 14728, 14775, 14822, 14869, 14916, 14963,
  15010, 15056, 15103, 15149, 15196, 15242, 15289, 15335,
  15381, 15427, 15474, 15520, 15566, 15612, 15657, 15703,
  15749, 15795, 15840, 15886, 15932, 15977, 16022, 16068,
  16113, 16158, 16204, 16249, 16294, 16339, 16384, 16384,
};
/* clang-format on */

static ProjectPointsFunc get_project_points_type(TransformationType type) {
  switch (type) {
    case HOMOGRAPHY: return project_points_homography;
    case AFFINE: return project_points_affine;
    case ROTZOOM: return project_points_rotzoom;
    case TRANSLATION: return project_points_translation;
    default: assert(0); return NULL;
  }
}

void project_points_translation(int32_t *mat, int *points, int *proj,
                                const int n, const int stride_points,
                                const int stride_proj, const int subsampling_x,
                                const int subsampling_y) {
  int i;
  for (i = 0; i < n; ++i) {
    const int x = *(points++), y = *(points++);
    if (subsampling_x)
      *(proj++) = ROUND_POWER_OF_TWO_SIGNED(
          ((x * (1 << (WARPEDMODEL_PREC_BITS + 1))) + mat[0]),
          WARPEDDIFF_PREC_BITS + 1);
    else
      *(proj++) = ROUND_POWER_OF_TWO_SIGNED(
          ((x * (1 << WARPEDMODEL_PREC_BITS)) + mat[0]), WARPEDDIFF_PREC_BITS);
    if (subsampling_y)
      *(proj++) = ROUND_POWER_OF_TWO_SIGNED(
          ((y * (1 << (WARPEDMODEL_PREC_BITS + 1))) + mat[1]),
          WARPEDDIFF_PREC_BITS + 1);
    else
      *(proj++) = ROUND_POWER_OF_TWO_SIGNED(
          ((y * (1 << WARPEDMODEL_PREC_BITS))) + mat[1], WARPEDDIFF_PREC_BITS);
    points += stride_points - 2;
    proj += stride_proj - 2;
  }
}

void project_points_rotzoom(int32_t *mat, int *points, int *proj, const int n,
                            const int stride_points, const int stride_proj,
                            const int subsampling_x, const int subsampling_y) {
  int i;
  for (i = 0; i < n; ++i) {
    const int x = *(points++), y = *(points++);
    if (subsampling_x)
      *(proj++) = ROUND_POWER_OF_TWO_SIGNED(
          mat[2] * 2 * x + mat[3] * 2 * y + mat[0] +
              (mat[2] + mat[3] - (1 << WARPEDMODEL_PREC_BITS)) / 2,
          WARPEDDIFF_PREC_BITS + 1);
    else
      *(proj++) = ROUND_POWER_OF_TWO_SIGNED(mat[2] * x + mat[3] * y + mat[0],
                                            WARPEDDIFF_PREC_BITS);
    if (subsampling_y)
      *(proj++) = ROUND_POWER_OF_TWO_SIGNED(
          -mat[3] * 2 * x + mat[2] * 2 * y + mat[1] +
              (-mat[3] + mat[2] - (1 << WARPEDMODEL_PREC_BITS)) / 2,
          WARPEDDIFF_PREC_BITS + 1);
    else
      *(proj++) = ROUND_POWER_OF_TWO_SIGNED(-mat[3] * x + mat[2] * y + mat[1],
                                            WARPEDDIFF_PREC_BITS);
    points += stride_points - 2;
    proj += stride_proj - 2;
  }
}

void project_points_affine(int32_t *mat, int *points, int *proj, const int n,
                           const int stride_points, const int stride_proj,
                           const int subsampling_x, const int subsampling_y) {
  int i;
  for (i = 0; i < n; ++i) {
    const int x = *(points++), y = *(points++);
    if (subsampling_x)
      *(proj++) = ROUND_POWER_OF_TWO_SIGNED(
          mat[2] * 2 * x + mat[3] * 2 * y + mat[0] +
              (mat[2] + mat[3] - (1 << WARPEDMODEL_PREC_BITS)) / 2,
          WARPEDDIFF_PREC_BITS + 1);
    else
      *(proj++) = ROUND_POWER_OF_TWO_SIGNED(mat[2] * x + mat[3] * y + mat[0],
                                            WARPEDDIFF_PREC_BITS);
    if (subsampling_y)
      *(proj++) = ROUND_POWER_OF_TWO_SIGNED(
          mat[4] * 2 * x + mat[5] * 2 * y + mat[1] +
              (mat[4] + mat[5] - (1 << WARPEDMODEL_PREC_BITS)) / 2,
          WARPEDDIFF_PREC_BITS + 1);
    else
      *(proj++) = ROUND_POWER_OF_TWO_SIGNED(mat[4] * x + mat[5] * y + mat[1],
                                            WARPEDDIFF_PREC_BITS);
    points += stride_points - 2;
    proj += stride_proj - 2;
  }
}

void project_points_homography(int32_t *mat, int *points, int *proj,
                               const int n, const int stride_points,
                               const int stride_proj, const int subsampling_x,
                               const int subsampling_y) {
  int i;
  int64_t x, y, Z;
  int64_t xp, yp;
  for (i = 0; i < n; ++i) {
    x = *(points++), y = *(points++);
    x = (subsampling_x ? 4 * x + 1 : 2 * x);
    y = (subsampling_y ? 4 * y + 1 : 2 * y);

    Z = (mat[6] * x + mat[7] * y + (1 << (WARPEDMODEL_ROW3HOMO_PREC_BITS + 1)));
    xp = (mat[2] * x + mat[3] * y + 2 * mat[0]) *
         (1 << (WARPEDPIXEL_PREC_BITS + WARPEDMODEL_ROW3HOMO_PREC_BITS -
                WARPEDMODEL_PREC_BITS));
    yp = (mat[4] * x + mat[5] * y + 2 * mat[1]) *
         (1 << (WARPEDPIXEL_PREC_BITS + WARPEDMODEL_ROW3HOMO_PREC_BITS -
                WARPEDMODEL_PREC_BITS));

    xp = xp > 0 ? (xp + Z / 2) / Z : (xp - Z / 2) / Z;
    yp = yp > 0 ? (yp + Z / 2) / Z : (yp - Z / 2) / Z;

    if (subsampling_x) xp = (xp - (1 << (WARPEDPIXEL_PREC_BITS - 1))) / 2;
    if (subsampling_y) yp = (yp - (1 << (WARPEDPIXEL_PREC_BITS - 1))) / 2;
    *(proj++) = xp;
    *(proj++) = yp;

    points += stride_points - 2;
    proj += stride_proj - 2;
  }
}

// 'points' are at original scale, output 'proj's are scaled up by
// 1 << WARPEDPIXEL_PREC_BITS
void project_points(WarpedMotionParams *wm_params, int *points, int *proj,
                    const int n, const int stride_points, const int stride_proj,
                    const int subsampling_x, const int subsampling_y) {
  switch (wm_params->wmtype) {
    case AFFINE:
      project_points_affine(wm_params->wmmat, points, proj, n, stride_points,
                            stride_proj, subsampling_x, subsampling_y);
      break;
    case ROTZOOM:
      project_points_rotzoom(wm_params->wmmat, points, proj, n, stride_points,
                             stride_proj, subsampling_x, subsampling_y);
      break;
    case HOMOGRAPHY:
      project_points_homography(wm_params->wmmat, points, proj, n,
                                stride_points, stride_proj, subsampling_x,
                                subsampling_y);
      break;
    default: assert(0 && "Invalid warped motion type!"); return;
  }
}

static const int16_t
    filter_ntap[WARPEDPIXEL_PREC_SHIFTS][WARPEDPIXEL_FILTER_TAPS] = {
      { 0, 0, 128, 0, 0, 0 },      { 0, -1, 128, 2, -1, 0 },
      { 1, -3, 127, 4, -1, 0 },    { 1, -4, 126, 6, -2, 1 },
      { 1, -5, 126, 8, -3, 1 },    { 1, -6, 125, 11, -4, 1 },
      { 1, -7, 124, 13, -4, 1 },   { 2, -8, 123, 15, -5, 1 },
      { 2, -9, 122, 18, -6, 1 },   { 2, -10, 121, 20, -6, 1 },
      { 2, -11, 120, 22, -7, 2 },  { 2, -12, 119, 25, -8, 2 },
      { 3, -13, 117, 27, -8, 2 },  { 3, -13, 116, 29, -9, 2 },
      { 3, -14, 114, 32, -10, 3 }, { 3, -15, 113, 35, -10, 2 },
      { 3, -15, 111, 37, -11, 3 }, { 3, -16, 109, 40, -11, 3 },
      { 3, -16, 108, 42, -12, 3 }, { 4, -17, 106, 45, -13, 3 },
      { 4, -17, 104, 47, -13, 3 }, { 4, -17, 102, 50, -14, 3 },
      { 4, -17, 100, 52, -14, 3 }, { 4, -18, 98, 55, -15, 4 },
      { 4, -18, 96, 58, -15, 3 },  { 4, -18, 94, 60, -16, 4 },
      { 4, -18, 91, 63, -16, 4 },  { 4, -18, 89, 65, -16, 4 },
      { 4, -18, 87, 68, -17, 4 },  { 4, -18, 85, 70, -17, 4 },
      { 4, -18, 82, 73, -17, 4 },  { 4, -18, 80, 75, -17, 4 },
      { 4, -18, 78, 78, -18, 4 },  { 4, -17, 75, 80, -18, 4 },
      { 4, -17, 73, 82, -18, 4 },  { 4, -17, 70, 85, -18, 4 },
      { 4, -17, 68, 87, -18, 4 },  { 4, -16, 65, 89, -18, 4 },
      { 4, -16, 63, 91, -18, 4 },  { 4, -16, 60, 94, -18, 4 },
      { 3, -15, 58, 96, -18, 4 },  { 4, -15, 55, 98, -18, 4 },
      { 3, -14, 52, 100, -17, 4 }, { 3, -14, 50, 102, -17, 4 },
      { 3, -13, 47, 104, -17, 4 }, { 3, -13, 45, 106, -17, 4 },
      { 3, -12, 42, 108, -16, 3 }, { 3, -11, 40, 109, -16, 3 },
      { 3, -11, 37, 111, -15, 3 }, { 2, -10, 35, 113, -15, 3 },
      { 3, -10, 32, 114, -14, 3 }, { 2, -9, 29, 116, -13, 3 },
      { 2, -8, 27, 117, -13, 3 },  { 2, -8, 25, 119, -12, 2 },
      { 2, -7, 22, 120, -11, 2 },  { 1, -6, 20, 121, -10, 2 },
      { 1, -6, 18, 122, -9, 2 },   { 1, -5, 15, 123, -8, 2 },
      { 1, -4, 13, 124, -7, 1 },   { 1, -4, 11, 125, -6, 1 },
      { 1, -3, 8, 126, -5, 1 },    { 1, -2, 6, 126, -4, 1 },
      { 0, -1, 4, 127, -3, 1 },    { 0, -1, 2, 128, -1, 0 },
    };

static int32_t do_ntap_filter(int32_t *p, int x) {
  int i;
  int32_t sum = 0;
  for (i = 0; i < WARPEDPIXEL_FILTER_TAPS; ++i) {
    sum += p[i - WARPEDPIXEL_FILTER_TAPS / 2 + 1] * filter_ntap[x][i];
  }
  return sum;
}

static int32_t do_cubic_filter(int32_t *p, int x) {
  if (x == 0) {
    return p[0] * (1 << WARPEDPIXEL_FILTER_BITS);
  } else if (x == (1 << WARPEDPIXEL_PREC_BITS)) {
    return p[1] * (1 << WARPEDPIXEL_FILTER_BITS);
  } else {
    const int64_t v1 = (int64_t)x * x * x * (3 * (p[0] - p[1]) + p[2] - p[-1]);
    const int64_t v2 = x * x * (2 * p[-1] - 5 * p[0] + 4 * p[1] - p[2]);
    const int64_t v3 = x * (p[1] - p[-1]);
    const int64_t v4 = 2 * p[0];
    return (int32_t)ROUND_POWER_OF_TWO_SIGNED(
        (v4 * (1 << (3 * WARPEDPIXEL_PREC_BITS))) +
            (v3 * (1 << (2 * WARPEDPIXEL_PREC_BITS))) +
            (v2 * (1 << WARPEDPIXEL_PREC_BITS)) + v1,
        3 * WARPEDPIXEL_PREC_BITS + 1 - WARPEDPIXEL_FILTER_BITS);
  }
}

static INLINE void get_subcolumn(int taps, uint8_t *ref, int32_t *col,
                                 int stride, int x, int y_start) {
  int i;
  for (i = 0; i < taps; ++i) {
    col[i] = ref[(i + y_start) * stride + x];
  }
}

static uint8_t bi_ntap_filter(uint8_t *ref, int x, int y, int stride) {
  int32_t val, arr[WARPEDPIXEL_FILTER_TAPS];
  int k;
  int i = (int)x >> WARPEDPIXEL_PREC_BITS;
  int j = (int)y >> WARPEDPIXEL_PREC_BITS;
  for (k = 0; k < WARPEDPIXEL_FILTER_TAPS; ++k) {
    int32_t arr_temp[WARPEDPIXEL_FILTER_TAPS];
    get_subcolumn(WARPEDPIXEL_FILTER_TAPS, ref, arr_temp, stride,
                  i + k + 1 - WARPEDPIXEL_FILTER_TAPS / 2,
                  j + 1 - WARPEDPIXEL_FILTER_TAPS / 2);
    arr[k] = do_ntap_filter(arr_temp + WARPEDPIXEL_FILTER_TAPS / 2 - 1,
                            y - (j * (1 << WARPEDPIXEL_PREC_BITS)));
  }
  val = do_ntap_filter(arr + WARPEDPIXEL_FILTER_TAPS / 2 - 1,
                       x - (i * (1 << WARPEDPIXEL_PREC_BITS)));
  val = ROUND_POWER_OF_TWO_SIGNED(val, WARPEDPIXEL_FILTER_BITS * 2);
  return (uint8_t)clip_pixel(val);
}

static uint8_t bi_cubic_filter(uint8_t *ref, int x, int y, int stride) {
  int32_t val, arr[4];
  int k;
  int i = (int)x >> WARPEDPIXEL_PREC_BITS;
  int j = (int)y >> WARPEDPIXEL_PREC_BITS;
  for (k = 0; k < 4; ++k) {
    int32_t arr_temp[4];
    get_subcolumn(4, ref, arr_temp, stride, i + k - 1, j - 1);
    arr[k] =
        do_cubic_filter(arr_temp + 1, y - (j * (1 << WARPEDPIXEL_PREC_BITS)));
  }
  val = do_cubic_filter(arr + 1, x - (i * (1 << WARPEDPIXEL_PREC_BITS)));
  val = ROUND_POWER_OF_TWO_SIGNED(val, WARPEDPIXEL_FILTER_BITS * 2);
  return (uint8_t)clip_pixel(val);
}

static uint8_t bi_linear_filter(uint8_t *ref, int x, int y, int stride) {
  const int ix = x >> WARPEDPIXEL_PREC_BITS;
  const int iy = y >> WARPEDPIXEL_PREC_BITS;
  const int sx = x - (ix * (1 << WARPEDPIXEL_PREC_BITS));
  const int sy = y - (iy * (1 << WARPEDPIXEL_PREC_BITS));
  int32_t val;
  val = ROUND_POWER_OF_TWO_SIGNED(
      ref[iy * stride + ix] * (WARPEDPIXEL_PREC_SHIFTS - sy) *
              (WARPEDPIXEL_PREC_SHIFTS - sx) +
          ref[iy * stride + ix + 1] * (WARPEDPIXEL_PREC_SHIFTS - sy) * sx +
          ref[(iy + 1) * stride + ix] * sy * (WARPEDPIXEL_PREC_SHIFTS - sx) +
          ref[(iy + 1) * stride + ix + 1] * sy * sx,
      WARPEDPIXEL_PREC_BITS * 2);
  return (uint8_t)clip_pixel(val);
}

static uint8_t warp_interpolate(uint8_t *ref, int x, int y, int width,
                                int height, int stride) {
  int ix = x >> WARPEDPIXEL_PREC_BITS;
  int iy = y >> WARPEDPIXEL_PREC_BITS;
  int sx = x - (ix * (1 << WARPEDPIXEL_PREC_BITS));
  int sy = y - (iy * (1 << WARPEDPIXEL_PREC_BITS));
  int32_t v;

  if (ix < 0 && iy < 0)
    return ref[0];
  else if (ix < 0 && iy >= height - 1)
    return ref[(height - 1) * stride];
  else if (ix >= width - 1 && iy < 0)
    return ref[width - 1];
  else if (ix >= width - 1 && iy >= height - 1)
    return ref[(height - 1) * stride + (width - 1)];
  else if (ix < 0) {
    v = ROUND_POWER_OF_TWO_SIGNED(
        ref[iy * stride] * (WARPEDPIXEL_PREC_SHIFTS - sy) +
            ref[(iy + 1) * stride] * sy,
        WARPEDPIXEL_PREC_BITS);
    return clip_pixel(v);
  } else if (iy < 0) {
    v = ROUND_POWER_OF_TWO_SIGNED(
        ref[ix] * (WARPEDPIXEL_PREC_SHIFTS - sx) + ref[ix + 1] * sx,
        WARPEDPIXEL_PREC_BITS);
    return clip_pixel(v);
  } else if (ix >= width - 1) {
    v = ROUND_POWER_OF_TWO_SIGNED(
        ref[iy * stride + width - 1] * (WARPEDPIXEL_PREC_SHIFTS - sy) +
            ref[(iy + 1) * stride + width - 1] * sy,
        WARPEDPIXEL_PREC_BITS);
    return clip_pixel(v);
  } else if (iy >= height - 1) {
    v = ROUND_POWER_OF_TWO_SIGNED(
        ref[(height - 1) * stride + ix] * (WARPEDPIXEL_PREC_SHIFTS - sx) +
            ref[(height - 1) * stride + ix + 1] * sx,
        WARPEDPIXEL_PREC_BITS);
    return clip_pixel(v);
  } else if (ix >= WARPEDPIXEL_FILTER_TAPS / 2 - 1 &&
             iy >= WARPEDPIXEL_FILTER_TAPS / 2 - 1 &&
             ix < width - WARPEDPIXEL_FILTER_TAPS / 2 &&
             iy < height - WARPEDPIXEL_FILTER_TAPS / 2) {
    return bi_ntap_filter(ref, x, y, stride);
  } else if (ix >= 1 && iy >= 1 && ix < width - 2 && iy < height - 2) {
    return bi_cubic_filter(ref, x, y, stride);
  } else {
    return bi_linear_filter(ref, x, y, stride);
  }
}

// For warping, we really use a 6-tap filter, but we do blocks of 8 pixels
// at a time. The zoom/rotation/shear in the model are applied to the
// "fractional" position of each pixel, which therefore varies within
// [-1, 2) * WARPEDPIXEL_PREC_SHIFTS.
// We need an extra 2 taps to fit this in, for a total of 8 taps.
/* clang-format off */
const int16_t warped_filter[WARPEDPIXEL_PREC_SHIFTS * 3][8] = {
  // [-1, 0)
  { 0,   0, 128,   0,   0, 0, 0, 0 }, { 0, - 1, 128,   2, - 1, 0, 0, 0 },
  { 1, - 3, 127,   4, - 1, 0, 0, 0 }, { 1, - 4, 126,   6, - 2, 1, 0, 0 },
  { 1, - 5, 126,   8, - 3, 1, 0, 0 }, { 1, - 6, 125,  11, - 4, 1, 0, 0 },
  { 1, - 7, 124,  13, - 4, 1, 0, 0 }, { 2, - 8, 123,  15, - 5, 1, 0, 0 },
  { 2, - 9, 122,  18, - 6, 1, 0, 0 }, { 2, -10, 121,  20, - 6, 1, 0, 0 },
  { 2, -11, 120,  22, - 7, 2, 0, 0 }, { 2, -12, 119,  25, - 8, 2, 0, 0 },
  { 3, -13, 117,  27, - 8, 2, 0, 0 }, { 3, -13, 116,  29, - 9, 2, 0, 0 },
  { 3, -14, 114,  32, -10, 3, 0, 0 }, { 3, -15, 113,  35, -10, 2, 0, 0 },
  { 3, -15, 111,  37, -11, 3, 0, 0 }, { 3, -16, 109,  40, -11, 3, 0, 0 },
  { 3, -16, 108,  42, -12, 3, 0, 0 }, { 4, -17, 106,  45, -13, 3, 0, 0 },
  { 4, -17, 104,  47, -13, 3, 0, 0 }, { 4, -17, 102,  50, -14, 3, 0, 0 },
  { 4, -17, 100,  52, -14, 3, 0, 0 }, { 4, -18,  98,  55, -15, 4, 0, 0 },
  { 4, -18,  96,  58, -15, 3, 0, 0 }, { 4, -18,  94,  60, -16, 4, 0, 0 },
  { 4, -18,  91,  63, -16, 4, 0, 0 }, { 4, -18,  89,  65, -16, 4, 0, 0 },
  { 4, -18,  87,  68, -17, 4, 0, 0 }, { 4, -18,  85,  70, -17, 4, 0, 0 },
  { 4, -18,  82,  73, -17, 4, 0, 0 }, { 4, -18,  80,  75, -17, 4, 0, 0 },
  { 4, -18,  78,  78, -18, 4, 0, 0 }, { 4, -17,  75,  80, -18, 4, 0, 0 },
  { 4, -17,  73,  82, -18, 4, 0, 0 }, { 4, -17,  70,  85, -18, 4, 0, 0 },
  { 4, -17,  68,  87, -18, 4, 0, 0 }, { 4, -16,  65,  89, -18, 4, 0, 0 },
  { 4, -16,  63,  91, -18, 4, 0, 0 }, { 4, -16,  60,  94, -18, 4, 0, 0 },
  { 3, -15,  58,  96, -18, 4, 0, 0 }, { 4, -15,  55,  98, -18, 4, 0, 0 },
  { 3, -14,  52, 100, -17, 4, 0, 0 }, { 3, -14,  50, 102, -17, 4, 0, 0 },
  { 3, -13,  47, 104, -17, 4, 0, 0 }, { 3, -13,  45, 106, -17, 4, 0, 0 },
  { 3, -12,  42, 108, -16, 3, 0, 0 }, { 3, -11,  40, 109, -16, 3, 0, 0 },
  { 3, -11,  37, 111, -15, 3, 0, 0 }, { 2, -10,  35, 113, -15, 3, 0, 0 },
  { 3, -10,  32, 114, -14, 3, 0, 0 }, { 2, - 9,  29, 116, -13, 3, 0, 0 },
  { 2, - 8,  27, 117, -13, 3, 0, 0 }, { 2, - 8,  25, 119, -12, 2, 0, 0 },
  { 2, - 7,  22, 120, -11, 2, 0, 0 }, { 1, - 6,  20, 121, -10, 2, 0, 0 },
  { 1, - 6,  18, 122, - 9, 2, 0, 0 }, { 1, - 5,  15, 123, - 8, 2, 0, 0 },
  { 1, - 4,  13, 124, - 7, 1, 0, 0 }, { 1, - 4,  11, 125, - 6, 1, 0, 0 },
  { 1, - 3,   8, 126, - 5, 1, 0, 0 }, { 1, - 2,   6, 126, - 4, 1, 0, 0 },
  { 0, - 1,   4, 127, - 3, 1, 0, 0 }, { 0, - 1,   2, 128, - 1, 0, 0, 0 },

  // [0, 1)
  { 0,  0,   0, 128,   0,   0,  0,  0}, { 0,  1,  -2, 128,   2,  -1,  0,  0},
  { 0,  1,  -3, 127,   4,  -2,  1,  0}, { 0,  1,  -5, 127,   6,  -2,  1,  0},
  { 0,  2,  -6, 126,   8,  -3,  1,  0}, {-1,  2,  -7, 126,  11,  -4,  2, -1},
  {-1,  3,  -8, 125,  13,  -5,  2, -1}, {-1,  3, -10, 124,  16,  -6,  3, -1},
  {-1,  4, -11, 123,  18,  -7,  3, -1}, {-1,  4, -12, 122,  20,  -7,  3, -1},
  {-1,  4, -13, 121,  23,  -8,  3, -1}, {-2,  5, -14, 120,  25,  -9,  4, -1},
  {-1,  5, -15, 119,  27, -10,  4, -1}, {-1,  5, -16, 118,  30, -11,  4, -1},
  {-2,  6, -17, 116,  33, -12,  5, -1}, {-2,  6, -17, 114,  35, -12,  5, -1},
  {-2,  6, -18, 113,  38, -13,  5, -1}, {-2,  7, -19, 111,  41, -14,  6, -2},
  {-2,  7, -19, 110,  43, -15,  6, -2}, {-2,  7, -20, 108,  46, -15,  6, -2},
  {-2,  7, -20, 106,  49, -16,  6, -2}, {-2,  7, -21, 104,  51, -16,  7, -2},
  {-2,  7, -21, 102,  54, -17,  7, -2}, {-2,  8, -21, 100,  56, -18,  7, -2},
  {-2,  8, -22,  98,  59, -18,  7, -2}, {-2,  8, -22,  96,  62, -19,  7, -2},
  {-2,  8, -22,  94,  64, -19,  7, -2}, {-2,  8, -22,  91,  67, -20,  8, -2},
  {-2,  8, -22,  89,  69, -20,  8, -2}, {-2,  8, -22,  87,  72, -21,  8, -2},
  {-2,  8, -21,  84,  74, -21,  8, -2}, {-2,  8, -22,  82,  77, -21,  8, -2},
  {-2,  8, -21,  79,  79, -21,  8, -2}, {-2,  8, -21,  77,  82, -22,  8, -2},
  {-2,  8, -21,  74,  84, -21,  8, -2}, {-2,  8, -21,  72,  87, -22,  8, -2},
  {-2,  8, -20,  69,  89, -22,  8, -2}, {-2,  8, -20,  67,  91, -22,  8, -2},
  {-2,  7, -19,  64,  94, -22,  8, -2}, {-2,  7, -19,  62,  96, -22,  8, -2},
  {-2,  7, -18,  59,  98, -22,  8, -2}, {-2,  7, -18,  56, 100, -21,  8, -2},
  {-2,  7, -17,  54, 102, -21,  7, -2}, {-2,  7, -16,  51, 104, -21,  7, -2},
  {-2,  6, -16,  49, 106, -20,  7, -2}, {-2,  6, -15,  46, 108, -20,  7, -2},
  {-2,  6, -15,  43, 110, -19,  7, -2}, {-2,  6, -14,  41, 111, -19,  7, -2},
  {-1,  5, -13,  38, 113, -18,  6, -2}, {-1,  5, -12,  35, 114, -17,  6, -2},
  {-1,  5, -12,  33, 116, -17,  6, -2}, {-1,  4, -11,  30, 118, -16,  5, -1},
  {-1,  4, -10,  27, 119, -15,  5, -1}, {-1,  4,  -9,  25, 120, -14,  5, -2},
  {-1,  3,  -8,  23, 121, -13,  4, -1}, {-1,  3,  -7,  20, 122, -12,  4, -1},
  {-1,  3,  -7,  18, 123, -11,  4, -1}, {-1,  3,  -6,  16, 124, -10,  3, -1},
  {-1,  2,  -5,  13, 125,  -8,  3, -1}, {-1,  2,  -4,  11, 126,  -7,  2, -1},
  { 0,  1,  -3,   8, 126,  -6,  2,  0}, { 0,  1,  -2,   6, 127,  -5,  1,  0},
  { 0,  1,  -2,   4, 127,  -3,  1,  0}, { 0,  0,  -1,   2, 128,  -2,  1,  0},

  // [1, 2)
  { 0, 0, 0,   0, 128,   0,   0, 0 }, { 0, 0, 0, - 1, 128,   2, - 1, 0 },
  { 0, 0, 1, - 3, 127,   4, - 1, 0 }, { 0, 0, 1, - 4, 126,   6, - 2, 1 },
  { 0, 0, 1, - 5, 126,   8, - 3, 1 }, { 0, 0, 1, - 6, 125,  11, - 4, 1 },
  { 0, 0, 1, - 7, 124,  13, - 4, 1 }, { 0, 0, 2, - 8, 123,  15, - 5, 1 },
  { 0, 0, 2, - 9, 122,  18, - 6, 1 }, { 0, 0, 2, -10, 121,  20, - 6, 1 },
  { 0, 0, 2, -11, 120,  22, - 7, 2 }, { 0, 0, 2, -12, 119,  25, - 8, 2 },
  { 0, 0, 3, -13, 117,  27, - 8, 2 }, { 0, 0, 3, -13, 116,  29, - 9, 2 },
  { 0, 0, 3, -14, 114,  32, -10, 3 }, { 0, 0, 3, -15, 113,  35, -10, 2 },
  { 0, 0, 3, -15, 111,  37, -11, 3 }, { 0, 0, 3, -16, 109,  40, -11, 3 },
  { 0, 0, 3, -16, 108,  42, -12, 3 }, { 0, 0, 4, -17, 106,  45, -13, 3 },
  { 0, 0, 4, -17, 104,  47, -13, 3 }, { 0, 0, 4, -17, 102,  50, -14, 3 },
  { 0, 0, 4, -17, 100,  52, -14, 3 }, { 0, 0, 4, -18,  98,  55, -15, 4 },
  { 0, 0, 4, -18,  96,  58, -15, 3 }, { 0, 0, 4, -18,  94,  60, -16, 4 },
  { 0, 0, 4, -18,  91,  63, -16, 4 }, { 0, 0, 4, -18,  89,  65, -16, 4 },
  { 0, 0, 4, -18,  87,  68, -17, 4 }, { 0, 0, 4, -18,  85,  70, -17, 4 },
  { 0, 0, 4, -18,  82,  73, -17, 4 }, { 0, 0, 4, -18,  80,  75, -17, 4 },
  { 0, 0, 4, -18,  78,  78, -18, 4 }, { 0, 0, 4, -17,  75,  80, -18, 4 },
  { 0, 0, 4, -17,  73,  82, -18, 4 }, { 0, 0, 4, -17,  70,  85, -18, 4 },
  { 0, 0, 4, -17,  68,  87, -18, 4 }, { 0, 0, 4, -16,  65,  89, -18, 4 },
  { 0, 0, 4, -16,  63,  91, -18, 4 }, { 0, 0, 4, -16,  60,  94, -18, 4 },
  { 0, 0, 3, -15,  58,  96, -18, 4 }, { 0, 0, 4, -15,  55,  98, -18, 4 },
  { 0, 0, 3, -14,  52, 100, -17, 4 }, { 0, 0, 3, -14,  50, 102, -17, 4 },
  { 0, 0, 3, -13,  47, 104, -17, 4 }, { 0, 0, 3, -13,  45, 106, -17, 4 },
  { 0, 0, 3, -12,  42, 108, -16, 3 }, { 0, 0, 3, -11,  40, 109, -16, 3 },
  { 0, 0, 3, -11,  37, 111, -15, 3 }, { 0, 0, 2, -10,  35, 113, -15, 3 },
  { 0, 0, 3, -10,  32, 114, -14, 3 }, { 0, 0, 2, - 9,  29, 116, -13, 3 },
  { 0, 0, 2, - 8,  27, 117, -13, 3 }, { 0, 0, 2, - 8,  25, 119, -12, 2 },
  { 0, 0, 2, - 7,  22, 120, -11, 2 }, { 0, 0, 1, - 6,  20, 121, -10, 2 },
  { 0, 0, 1, - 6,  18, 122, - 9, 2 }, { 0, 0, 1, - 5,  15, 123, - 8, 2 },
  { 0, 0, 1, - 4,  13, 124, - 7, 1 }, { 0, 0, 1, - 4,  11, 125, - 6, 1 },
  { 0, 0, 1, - 3,   8, 126, - 5, 1 }, { 0, 0, 1, - 2,   6, 126, - 4, 1 },
  { 0, 0, 0, - 1,   4, 127, - 3, 1 }, { 0, 0, 0, - 1,   2, 128, - 1, 0 },
};
/* clang-format on */

#if CONFIG_AOM_HIGHBITDEPTH
static INLINE void highbd_get_subcolumn(int taps, uint16_t *ref, int32_t *col,
                                        int stride, int x, int y_start) {
  int i;
  for (i = 0; i < taps; ++i) {
    col[i] = ref[(i + y_start) * stride + x];
  }
}

static uint16_t highbd_bi_ntap_filter(uint16_t *ref, int x, int y, int stride,
                                      int bd) {
  int32_t val, arr[WARPEDPIXEL_FILTER_TAPS];
  int k;
  int i = (int)x >> WARPEDPIXEL_PREC_BITS;
  int j = (int)y >> WARPEDPIXEL_PREC_BITS;
  for (k = 0; k < WARPEDPIXEL_FILTER_TAPS; ++k) {
    int32_t arr_temp[WARPEDPIXEL_FILTER_TAPS];
    highbd_get_subcolumn(WARPEDPIXEL_FILTER_TAPS, ref, arr_temp, stride,
                         i + k + 1 - WARPEDPIXEL_FILTER_TAPS / 2,
                         j + 1 - WARPEDPIXEL_FILTER_TAPS / 2);
    arr[k] = do_ntap_filter(arr_temp + WARPEDPIXEL_FILTER_TAPS / 2 - 1,
                            y - (j * (1 << WARPEDPIXEL_PREC_BITS)));
  }
  val = do_ntap_filter(arr + WARPEDPIXEL_FILTER_TAPS / 2 - 1,
                       x - (i * (1 << WARPEDPIXEL_PREC_BITS)));
  val = ROUND_POWER_OF_TWO_SIGNED(val, WARPEDPIXEL_FILTER_BITS * 2);
  return (uint16_t)clip_pixel_highbd(val, bd);
}

static uint16_t highbd_bi_cubic_filter(uint16_t *ref, int x, int y, int stride,
                                       int bd) {
  int32_t val, arr[4];
  int k;
  int i = (int)x >> WARPEDPIXEL_PREC_BITS;
  int j = (int)y >> WARPEDPIXEL_PREC_BITS;
  for (k = 0; k < 4; ++k) {
    int32_t arr_temp[4];
    highbd_get_subcolumn(4, ref, arr_temp, stride, i + k - 1, j - 1);
    arr[k] =
        do_cubic_filter(arr_temp + 1, y - (j * (1 << WARPEDPIXEL_PREC_BITS)));
  }
  val = do_cubic_filter(arr + 1, x - (i * (1 << WARPEDPIXEL_PREC_BITS)));
  val = ROUND_POWER_OF_TWO_SIGNED(val, WARPEDPIXEL_FILTER_BITS * 2);
  return (uint16_t)clip_pixel_highbd(val, bd);
}

static uint16_t highbd_bi_linear_filter(uint16_t *ref, int x, int y, int stride,
                                        int bd) {
  const int ix = x >> WARPEDPIXEL_PREC_BITS;
  const int iy = y >> WARPEDPIXEL_PREC_BITS;
  const int sx = x - (ix * (1 << WARPEDPIXEL_PREC_BITS));
  const int sy = y - (iy * (1 << WARPEDPIXEL_PREC_BITS));
  int32_t val;
  val = ROUND_POWER_OF_TWO_SIGNED(
      ref[iy * stride + ix] * (WARPEDPIXEL_PREC_SHIFTS - sy) *
              (WARPEDPIXEL_PREC_SHIFTS - sx) +
          ref[iy * stride + ix + 1] * (WARPEDPIXEL_PREC_SHIFTS - sy) * sx +
          ref[(iy + 1) * stride + ix] * sy * (WARPEDPIXEL_PREC_SHIFTS - sx) +
          ref[(iy + 1) * stride + ix + 1] * sy * sx,
      WARPEDPIXEL_PREC_BITS * 2);
  return (uint16_t)clip_pixel_highbd(val, bd);
}

static uint16_t highbd_warp_interpolate(uint16_t *ref, int x, int y, int width,
                                        int height, int stride, int bd) {
  int ix = x >> WARPEDPIXEL_PREC_BITS;
  int iy = y >> WARPEDPIXEL_PREC_BITS;
  int sx = x - (ix * (1 << WARPEDPIXEL_PREC_BITS));
  int sy = y - (iy * (1 << WARPEDPIXEL_PREC_BITS));
  int32_t v;

  if (ix < 0 && iy < 0)
    return ref[0];
  else if (ix < 0 && iy > height - 1)
    return ref[(height - 1) * stride];
  else if (ix > width - 1 && iy < 0)
    return ref[width - 1];
  else if (ix > width - 1 && iy > height - 1)
    return ref[(height - 1) * stride + (width - 1)];
  else if (ix < 0) {
    v = ROUND_POWER_OF_TWO_SIGNED(
        ref[iy * stride] * (WARPEDPIXEL_PREC_SHIFTS - sy) +
            ref[(iy + 1) * stride] * sy,
        WARPEDPIXEL_PREC_BITS);
    return clip_pixel_highbd(v, bd);
  } else if (iy < 0) {
    v = ROUND_POWER_OF_TWO_SIGNED(
        ref[ix] * (WARPEDPIXEL_PREC_SHIFTS - sx) + ref[ix + 1] * sx,
        WARPEDPIXEL_PREC_BITS);
    return clip_pixel_highbd(v, bd);
  } else if (ix > width - 1) {
    v = ROUND_POWER_OF_TWO_SIGNED(
        ref[iy * stride + width - 1] * (WARPEDPIXEL_PREC_SHIFTS - sy) +
            ref[(iy + 1) * stride + width - 1] * sy,
        WARPEDPIXEL_PREC_BITS);
    return clip_pixel_highbd(v, bd);
  } else if (iy > height - 1) {
    v = ROUND_POWER_OF_TWO_SIGNED(
        ref[(height - 1) * stride + ix] * (WARPEDPIXEL_PREC_SHIFTS - sx) +
            ref[(height - 1) * stride + ix + 1] * sx,
        WARPEDPIXEL_PREC_BITS);
    return clip_pixel_highbd(v, bd);
  } else if (ix >= WARPEDPIXEL_FILTER_TAPS / 2 - 1 &&
             iy >= WARPEDPIXEL_FILTER_TAPS / 2 - 1 &&
             ix < width - WARPEDPIXEL_FILTER_TAPS / 2 &&
             iy < height - WARPEDPIXEL_FILTER_TAPS / 2) {
    return highbd_bi_ntap_filter(ref, x, y, stride, bd);
  } else if (ix >= 1 && iy >= 1 && ix < width - 2 && iy < height - 2) {
    return highbd_bi_cubic_filter(ref, x, y, stride, bd);
  } else {
    return highbd_bi_linear_filter(ref, x, y, stride, bd);
  }
}

static INLINE int highbd_error_measure(int err, int bd) {
  const int b = bd - 8;
  const int bmask = (1 << b) - 1;
  const int v = (1 << b);
  int e1, e2;
  err = abs(err);
  e1 = err >> b;
  e2 = err & bmask;
  return error_measure_lut[255 + e1] * (v - e2) +
         error_measure_lut[256 + e1] * e2;
}

static void highbd_warp_plane_old(WarpedMotionParams *wm, uint8_t *ref8,
                                  int width, int height, int stride,
                                  uint8_t *pred8, int p_col, int p_row,
                                  int p_width, int p_height, int p_stride,
                                  int subsampling_x, int subsampling_y,
                                  int x_scale, int y_scale, int bd,
                                  int ref_frm) {
  int i, j;
  ProjectPointsFunc projectpoints = get_project_points_type(wm->wmtype);
  uint16_t *pred = CONVERT_TO_SHORTPTR(pred8);
  uint16_t *ref = CONVERT_TO_SHORTPTR(ref8);
  if (projectpoints == NULL) return;
  for (i = p_row; i < p_row + p_height; ++i) {
    for (j = p_col; j < p_col + p_width; ++j) {
      int in[2], out[2];
      in[0] = j;
      in[1] = i;
      projectpoints(wm->wmmat, in, out, 1, 2, 2, subsampling_x, subsampling_y);
      out[0] = ROUND_POWER_OF_TWO_SIGNED(out[0] * x_scale, 4);
      out[1] = ROUND_POWER_OF_TWO_SIGNED(out[1] * y_scale, 4);
      if (ref_frm)
        pred[(j - p_col) + (i - p_row) * p_stride] = ROUND_POWER_OF_TWO(
            pred[(j - p_col) + (i - p_row) * p_stride] +
                highbd_warp_interpolate(ref, out[0], out[1], width, height,
                                        stride, bd),
            1);
      else
        pred[(j - p_col) + (i - p_row) * p_stride] = highbd_warp_interpolate(
            ref, out[0], out[1], width, height, stride, bd);
    }
  }
}

// Note: For an explanation of the warp algorithm, see the comment
// above warp_plane()
static void highbd_warp_plane(WarpedMotionParams *wm, uint8_t *ref8, int width,
                              int height, int stride, uint8_t *pred8, int p_col,
                              int p_row, int p_width, int p_height,
                              int p_stride, int subsampling_x,
                              int subsampling_y, int x_scale, int y_scale,
                              int bd, int ref_frm) {
  if (wm->wmtype == ROTZOOM) {
    wm->wmmat[5] = wm->wmmat[2];
    wm->wmmat[4] = -wm->wmmat[3];
  }
  if (wm->wmtype == ROTZOOM || wm->wmtype == AFFINE) {
    int32_t tmp[15 * 8];
    int i, j, k, l, m;
    int32_t *mat = wm->wmmat;
    int32_t alpha, beta, gamma, delta;

    if (mat[2] == 0) {
      // assert(0 &&
      //   "Warped motion model is incompatible with new warp filter");
      warp_plane_old(wm, ref, width, height, stride, pred, p_col, p_row,
                     p_width, p_height, p_stride, subsampling_x, subsampling_y,
                     x_scale, y_scale, ref_frm);
      return;
    }

    alpha = mat[2] - (1 << WARPEDMODEL_PREC_BITS);
    beta = mat[3];
    gamma = ((int64_t)mat[4] << WARPEDMODEL_PREC_BITS) / mat[2];
    delta = mat[5] - (((int64_t)mat[3] * mat[4] + (mat[2] / 2)) / mat[2]) -
            (1 << WARPEDMODEL_PREC_BITS);
    uint16_t *ref = CONVERT_TO_SHORTPTR(ref8);
    uint16_t *pred = CONVERT_TO_SHORTPTR(pred8);

    if ((4 * abs(alpha) + 7 * abs(beta) > (1 << WARPEDMODEL_PREC_BITS)) ||
        (4 * abs(gamma) + 4 * abs(delta) > (1 << WARPEDMODEL_PREC_BITS))) {
      // assert(0 &&
      //   "Warped motion model is incompatible with new warp filter");
      highbd_warp_plane_old(wm, ref8, width, height, stride, pred8, p_col,
                            p_row, p_width, p_height, p_stride, subsampling_x,
                            subsampling_y, x_scale, y_scale, bd, ref_frm);
      return;
    }

    for (i = p_row; i < p_row + p_height; i += 8) {
      for (j = p_col; j < p_col + p_width; j += 8) {
        int32_t x4, y4, ix4, sx4, iy4, sy4;
        if (subsampling_x)
          x4 = ROUND_POWER_OF_TWO_SIGNED(
              mat[2] * 2 * (j + 4) + mat[3] * 2 * (i + 4) + mat[0] +
                  (mat[2] + mat[3] - (1 << WARPEDMODEL_PREC_BITS)) / 2,
              1);
        else
          x4 = mat[2] * (j + 4) + mat[3] * (i + 4) + mat[0];

        if (subsampling_y)
          y4 = ROUND_POWER_OF_TWO_SIGNED(
              mat[4] * 2 * (j + 4) + mat[5] * 2 * (i + 4) + mat[1] +
                  (mat[4] + mat[5] - (1 << WARPEDMODEL_PREC_BITS)) / 2,
              1);
        else
          y4 = mat[4] * (j + 4) + mat[5] * (i + 4) + mat[1];

        ix4 = x4 >> WARPEDMODEL_PREC_BITS;
        sx4 = x4 & ((1 << WARPEDMODEL_PREC_BITS) - 1);
        iy4 = y4 >> WARPEDMODEL_PREC_BITS;
        sy4 = y4 & ((1 << WARPEDMODEL_PREC_BITS) - 1);

        // Horizontal filter
        for (k = -7; k < 8; ++k) {
          int iy = iy4 + k;
          if (iy < 0)
            iy = 0;
          else if (iy > height - 1)
            iy = height - 1;

          for (l = -4; l < 4; ++l) {
            int ix = ix4 + l;
            int sx = ROUND_POWER_OF_TWO_SIGNED(sx4 + alpha * l + beta * k,
                                               WARPEDDIFF_PREC_BITS);
            const int16_t *coeffs = warped_filter[sx + WARPEDPIXEL_PREC_SHIFTS];
            int32_t sum = 0;
            for (m = 0; m < 8; ++m) {
              if (ix + m - 3 < 0)
                sum += ref[iy * stride] * coeffs[m];
              else if (ix + m - 3 > width - 1)
                sum += ref[iy * stride + width - 1] * coeffs[m];
              else
                sum += ref[iy * stride + ix + m - 3] * coeffs[m];
            }
            tmp[(k + 7) * 8 + (l + 4)] = sum;
          }
        }

        // Vertical filter
        for (k = -4; k < AOMMIN(4, p_row + p_height - i - 4); ++k) {
          for (l = -4; l < AOMMIN(4, p_col + p_width - j - 4); ++l) {
            uint16_t *p =
                &pred[(i - p_row + k + 4) * p_stride + (j - p_col + l + 4)];
            int sy = ROUND_POWER_OF_TWO_SIGNED(sy4 + gamma * l + delta * k,
                                               WARPEDDIFF_PREC_BITS);
            const int16_t *coeffs = warped_filter[sy + WARPEDPIXEL_PREC_SHIFTS];
            int32_t sum = 0;
            for (m = 0; m < 8; ++m) {
              sum += tmp[(k + m + 4) * 8 + (l + 4)] * coeffs[m];
            }
            sum = clip_pixel_highbd(
                ROUND_POWER_OF_TWO_SIGNED(sum, 2 * WARPEDPIXEL_FILTER_BITS),
                bd);
            if (ref_frm)
              *p = ROUND_POWER_OF_TWO_SIGNED(*p + sum, 1);
            else
              *p = sum;
          }
        }
      }
    }
  } else {
    highbd_warp_plane_old(wm, ref8, width, height, stride, pred8, p_col, p_row,
                          p_width, p_height, p_stride, subsampling_x,
                          subsampling_y, x_scale, y_scale, bd, ref_frm);
  }
}

static double highbd_warp_erroradv(WarpedMotionParams *wm, uint8_t *ref8,
                                   int width, int height, int stride,
                                   uint8_t *dst8, int p_col, int p_row,
                                   int p_width, int p_height, int p_stride,
                                   int subsampling_x, int subsampling_y,
                                   int x_scale, int y_scale, int bd) {
  int gm_err = 0, no_gm_err = 0;
  int64_t gm_sumerr = 0, no_gm_sumerr = 0;
  int i, j;
  uint16_t *tmp = aom_malloc(p_width * p_height * sizeof(*tmp));
  uint16_t *dst = CONVERT_TO_SHORTPTR(dst8);
  uint16_t *ref = CONVERT_TO_SHORTPTR(ref8);
  highbd_warp_plane(wm, ref8, width, height, stride, CONVERT_TO_BYTEPTR(tmp),
                    p_col, p_row, p_width, p_height, p_width, subsampling_x,
                    subsampling_y, x_scale, y_scale, bd, 0);
  for (i = 0; i < p_height; ++i) {
    for (j = 0; j < p_width; ++j) {
      gm_err = dst[j + i * p_stride] - tmp[j + i * p_width];
      no_gm_err =
          dst[j + i * p_stride] - ref[(j + p_col) + (i + p_row) * stride];
      gm_sumerr += highbd_error_measure(gm_err, bd);
      no_gm_sumerr += highbd_error_measure(no_gm_err, bd);
    }
  }
  aom_free(tmp);
  return (double)gm_sumerr / no_gm_sumerr;
}
#endif  // CONFIG_AOM_HIGHBITDEPTH

static INLINE int error_measure(int err) {
  return error_measure_lut[255 + err];
}

static void warp_plane_old(WarpedMotionParams *wm, uint8_t *ref, int width,
                           int height, int stride, uint8_t *pred, int p_col,
                           int p_row, int p_width, int p_height, int p_stride,
                           int subsampling_x, int subsampling_y, int x_scale,
                           int y_scale, int ref_frm) {
  int i, j;
  ProjectPointsFunc projectpoints = get_project_points_type(wm->wmtype);
  if (projectpoints == NULL) return;
  for (i = p_row; i < p_row + p_height; ++i) {
    for (j = p_col; j < p_col + p_width; ++j) {
      int in[2], out[2];
      in[0] = j;
      in[1] = i;
      projectpoints(wm->wmmat, in, out, 1, 2, 2, subsampling_x, subsampling_y);
      out[0] = ROUND_POWER_OF_TWO_SIGNED(out[0] * x_scale, 4);
      out[1] = ROUND_POWER_OF_TWO_SIGNED(out[1] * y_scale, 4);
      if (ref_frm)
        pred[(j - p_col) + (i - p_row) * p_stride] = ROUND_POWER_OF_TWO(
            pred[(j - p_col) + (i - p_row) * p_stride] +
                warp_interpolate(ref, out[0], out[1], width, height, stride),
            1);
      else
        pred[(j - p_col) + (i - p_row) * p_stride] =
            warp_interpolate(ref, out[0], out[1], width, height, stride);
    }
  }
}

/* The warp filter for ROTZOOM and AFFINE models works as follows:
   * Split the input into 8x8 blocks
   * For each block, project the point (4, 4) within the block, to get the
     overall block position. Split into integer and fractional coordinates,
     maintaining full WARPEDMODEL precision
   * Filter horizontally: Generate 15 rows of 8 pixels each. Each pixel gets a
     variable horizontal offset. This means that, while the rows of the
     intermediate buffer align with the rows of the *reference* image, the
     columns align with the columns of the *destination* image.
   * Filter vertically: Generate the output block (up to 8x8 pixels, but if the
     destination is too small we crop the output at this stage). Each pixel has
     a variable vertical offset, so that the resulting rows are aligned with
     the rows of the destination image.

   To accomplish these alignments, we factor the warp matrix as a
   product of two shear / asymmetric zoom matrices:
   / a b \  = /   1       0    \ * / 1+alpha  beta \
   \ c d /    \ gamma  1+delta /   \    0      1   /
   where a, b, c, d are wmmat[2], wmmat[3], wmmat[4], wmmat[5] respectively.
   The second shear (with alpha and beta) is applied by the horizontal filter,
   then the first shear (with gamma and delta) is applied by the vertical
   filter.

   The only limitation is that, to fit this in a fixed 8-tap filter size,
   the fractional pixel offsets must be at most +-1. Since the horizontal filter
   generates 15 rows of 8 columns, and the initial point we project is at (4, 4)
   within the block, the parameters must satisfy
   4 * |alpha| + 7 * |beta| <= 1   and   4 * |gamma| + 7 * |delta| <= 1
   for this filter to be applicable.

   Note: warp_affine() assumes that the caller has done all of the relevant
   checks, ie. that we have a ROTZOOM or AFFINE model, that wm[4] and wm[5]
   are set appropriately (if using a ROTZOOM model), and that alpha, beta,
   gamma, delta are all in range.

   TODO(david.barker): Maybe support scaled references?
*/
static inline int16_t saturate_int16(int32_t v) {
  if (v > 32767)
    return 32767;
  else if (v < -32768)
    return -32768;
  return v;
}

void av1_warp_affine_c(int32_t *mat, uint8_t *ref, int width, int height,
                       int stride, uint8_t *pred, int p_col, int p_row,
                       int p_width, int p_height, int p_stride,
                       int subsampling_x, int subsampling_y, int ref_frm,
                       int32_t alpha, int32_t beta, int32_t gamma,
                       int32_t delta) {
  int16_t tmp[15 * 8];
  int i, j, k, l, m;

  /* Note: For this code to work, the left/right frame borders need to be
     extended by at least 13 pixels each. By the time we get here, other
     code will have set up this border, but we allow an explicit check
     for debugging purposes.
  */
  /*for (i = 0; i < height; ++i) {
    for (j = 0; j < 13; ++j) {
      assert(ref[i * stride - 13 + j] == ref[i * stride]);
      assert(ref[i * stride + width + j] == ref[i * stride + (width - 1)]);
    }
  }*/

  for (i = p_row; i < p_row + p_height; i += 8) {
    for (j = p_col; j < p_col + p_width; j += 8) {
      int32_t x4, y4, ix4, sx4, iy4, sy4;
      if (subsampling_x)
        x4 = ROUND_POWER_OF_TWO_SIGNED(
            mat[2] * 2 * (j + 4) + mat[3] * 2 * (i + 4) + mat[0] +
                (mat[2] + mat[3] - (1 << WARPEDMODEL_PREC_BITS)) / 2,
            1);
      else
        x4 = mat[2] * (j + 4) + mat[3] * (i + 4) + mat[0];

      if (subsampling_y)
        y4 = ROUND_POWER_OF_TWO_SIGNED(
            mat[4] * 2 * (j + 4) + mat[5] * 2 * (i + 4) + mat[1] +
                (mat[4] + mat[5] - (1 << WARPEDMODEL_PREC_BITS)) / 2,
            1);
      else
        y4 = mat[4] * (j + 4) + mat[5] * (i + 4) + mat[1];

      ix4 = x4 >> WARPEDMODEL_PREC_BITS;
      sx4 = x4 & ((1 << WARPEDMODEL_PREC_BITS) - 1);
      iy4 = y4 >> WARPEDMODEL_PREC_BITS;
      sy4 = y4 & ((1 << WARPEDMODEL_PREC_BITS) - 1);

      // Horizontal filter
      for (k = -7; k < 8; ++k) {
        int iy = iy4 + k;
        if (iy < 0)
          iy = 0;
        else if (iy > height - 1)
          iy = height - 1;

        if (ix4 <= -7) {
          // In this case, the rightmost pixel sampled is in column
          // ix4 + 3 + 7 - 3 = ix4 + 7 <= 0, ie. the entire block
          // will sample only from the leftmost column
          // (once border extension is taken into account)
          for (l = 0; l < 8; ++l) {
            tmp[(k + 7) * 8 + l] =
                ref[iy * stride] * (1 << WARPEDPIXEL_FILTER_BITS);
          }
        } else if (ix4 >= width + 6) {
          // In this case, the leftmost pixel sampled is in column
          // ix4 - 4 + 0 - 3 = ix4 - 7 >= width - 1, ie. the entire block
          // will sample only from the rightmost column
          // (once border extension is taken into account)
          for (l = 0; l < 8; ++l) {
            tmp[(k + 7) * 8 + l] =
                ref[iy * stride + (width - 1)] * (1 << WARPEDPIXEL_FILTER_BITS);
          }
        } else {
          // If we get here, then
          // the leftmost pixel sampled is
          // ix4 - 4 + 0 - 3 = ix4 - 7 >= -13
          // and the rightmost pixel sampled is at most
          // ix4 + 3 + 7 - 3 = ix4 + 7 <= width + 12
          // So, assuming that border extension has been done, we
          // don't need to explicitly clamp values.
          int sx = sx4 + alpha * (-4) + beta * k;

          for (l = -4; l < 4; ++l) {
            int ix = ix4 + l - 3;
            // At this point, sx = sx4 + alpha * l + beta * k
            const int16_t *coeffs =
                warped_filter[ROUND_POWER_OF_TWO(sx, WARPEDDIFF_PREC_BITS) +
                              WARPEDPIXEL_PREC_SHIFTS];
            int32_t sum = 0;
            for (m = 0; m < 8; ++m) {
              sum += ref[iy * stride + ix + m] * coeffs[m];
            }
            tmp[(k + 7) * 8 + (l + 4)] = saturate_int16(sum);
            sx += alpha;
          }
        }
      }

      // Vertical filter
      for (k = -4; k < AOMMIN(4, p_row + p_height - i - 4); ++k) {
        int sy = sy4 + gamma * (-4) + delta * k;
        for (l = -4; l < 4; ++l) {
          uint8_t *p =
              &pred[(i - p_row + k + 4) * p_stride + (j - p_col + l + 4)];
          // At this point, sy = sy4 + gamma * l + delta * k
          const int16_t *coeffs =
              warped_filter[ROUND_POWER_OF_TWO(sy, WARPEDDIFF_PREC_BITS) +
                            WARPEDPIXEL_PREC_SHIFTS];
          int32_t sum = 0;
          for (m = 0; m < 8; ++m) {
            sum += tmp[(k + m + 4) * 8 + (l + 4)] * coeffs[m];
          }
          sum =
              clip_pixel(ROUND_POWER_OF_TWO(sum, 2 * WARPEDPIXEL_FILTER_BITS));
          if (ref_frm)
            *p = ROUND_POWER_OF_TWO(*p + sum, 1);
          else
            *p = sum;
          sy += gamma;
        }
      }
    }
  }
}

static void warp_plane(WarpedMotionParams *wm, uint8_t *ref, int width,
                       int height, int stride, uint8_t *pred, int p_col,
                       int p_row, int p_width, int p_height, int p_stride,
                       int subsampling_x, int subsampling_y, int x_scale,
                       int y_scale, int ref_frm) {
  if (wm->wmtype == ROTZOOM) {
    wm->wmmat[5] = wm->wmmat[2];
    wm->wmmat[4] = -wm->wmmat[3];
  }
  if ((wm->wmtype == ROTZOOM || wm->wmtype == AFFINE) && x_scale == 16 &&
      y_scale == 16) {
    int32_t *mat = wm->wmmat;
    int32_t alpha, beta, gamma, delta;

    if (mat[2] == 0) {
      // assert(0 &&
      //   "Warped motion model is incompatible with new warp filter");
      warp_plane_old(wm, ref, width, height, stride, pred, p_col, p_row,
                     p_width, p_height, p_stride, subsampling_x, subsampling_y,
                     x_scale, y_scale, ref_frm);
      return;
    }

    alpha = mat[2] - (1 << WARPEDMODEL_PREC_BITS);
    beta = mat[3];
    gamma = ((int64_t)mat[4] << WARPEDMODEL_PREC_BITS) / mat[2];
    delta = mat[5] - (((int64_t)mat[3] * mat[4] + (mat[2] / 2)) / mat[2]) -
            (1 << WARPEDMODEL_PREC_BITS);

    if ((4 * abs(alpha) + 7 * abs(beta) > (1 << WARPEDMODEL_PREC_BITS)) ||
        (4 * abs(gamma) + 4 * abs(delta) > (1 << WARPEDMODEL_PREC_BITS))) {
      // assert(0 &&
      //   "Warped motion model is incompatible with new warp filter");
      warp_plane_old(wm, ref, width, height, stride, pred, p_col, p_row,
                     p_width, p_height, p_stride, subsampling_x, subsampling_y,
                     x_scale, y_scale, ref_frm);
      return;
    }

    av1_warp_affine(mat, ref, width, height, stride, pred, p_col, p_row,
                    p_width, p_height, p_stride, subsampling_x, subsampling_y,
                    ref_frm, alpha, beta, gamma, delta);
  } else {
    warp_plane_old(wm, ref, width, height, stride, pred, p_col, p_row, p_width,
                   p_height, p_stride, subsampling_x, subsampling_y, x_scale,
                   y_scale, ref_frm);
  }
}

static double warp_erroradv(WarpedMotionParams *wm, uint8_t *ref, int width,
                            int height, int stride, uint8_t *dst, int p_col,
                            int p_row, int p_width, int p_height, int p_stride,
                            int subsampling_x, int subsampling_y, int x_scale,
                            int y_scale) {
  int gm_err = 0, no_gm_err = 0;
  int gm_sumerr = 0, no_gm_sumerr = 0;
  int i, j;
  uint8_t *tmp = aom_malloc(p_width * p_height);
  warp_plane(wm, ref, width, height, stride, tmp, p_col, p_row, p_width,
             p_height, p_width, subsampling_x, subsampling_y, x_scale, y_scale,
             0);

  for (i = 0; i < p_height; ++i) {
    for (j = 0; j < p_width; ++j) {
      gm_err = dst[j + i * p_stride] - tmp[j + i * p_width];
      no_gm_err =
          dst[j + i * p_stride] - ref[(j + p_col) + (i + p_row) * stride];
      gm_sumerr += error_measure(gm_err);
      no_gm_sumerr += error_measure(no_gm_err);
    }
  }

  aom_free(tmp);
  return (double)gm_sumerr / no_gm_sumerr;
}

double av1_warp_erroradv(WarpedMotionParams *wm,
#if CONFIG_AOM_HIGHBITDEPTH
                         int use_hbd, int bd,
#endif  // CONFIG_AOM_HIGHBITDEPTH
                         uint8_t *ref, int width, int height, int stride,
                         uint8_t *dst, int p_col, int p_row, int p_width,
                         int p_height, int p_stride, int subsampling_x,
                         int subsampling_y, int x_scale, int y_scale) {
#if CONFIG_AOM_HIGHBITDEPTH
  if (use_hbd)
    return highbd_warp_erroradv(
        wm, ref, width, height, stride, dst, p_col, p_row, p_width, p_height,
        p_stride, subsampling_x, subsampling_y, x_scale, y_scale, bd);
#endif  // CONFIG_AOM_HIGHBITDEPTH
  return warp_erroradv(wm, ref, width, height, stride, dst, p_col, p_row,
                       p_width, p_height, p_stride, subsampling_x,
                       subsampling_y, x_scale, y_scale);
}

void av1_warp_plane(WarpedMotionParams *wm,
#if CONFIG_AOM_HIGHBITDEPTH
                    int use_hbd, int bd,
#endif  // CONFIG_AOM_HIGHBITDEPTH
                    uint8_t *ref, int width, int height, int stride,
                    uint8_t *pred, int p_col, int p_row, int p_width,
                    int p_height, int p_stride, int subsampling_x,
                    int subsampling_y, int x_scale, int y_scale, int ref_frm) {
#if CONFIG_AOM_HIGHBITDEPTH
  if (use_hbd)
    highbd_warp_plane(wm, ref, width, height, stride, pred, p_col, p_row,
                      p_width, p_height, p_stride, subsampling_x, subsampling_y,
                      x_scale, y_scale, bd, ref_frm);
  else
#endif  // CONFIG_AOM_HIGHBITDEPTH
    warp_plane(wm, ref, width, height, stride, pred, p_col, p_row, p_width,
               p_height, p_stride, subsampling_x, subsampling_y, x_scale,
               y_scale, ref_frm);
}

void av1_integerize_model(const double *model, TransformationType wmtype,
                          WarpedMotionParams *wm) {
  wm->wmtype = wmtype;
  switch (wmtype) {
    case HOMOGRAPHY:
      assert(fabs(model[8] - 1.0) < 1e-12);
      wm->wmmat[6] =
          (int32_t)lrint(model[6] * (1 << WARPEDMODEL_ROW3HOMO_PREC_BITS));
      wm->wmmat[7] =
          (int32_t)lrint(model[7] * (1 << WARPEDMODEL_ROW3HOMO_PREC_BITS));
    /* fallthrough intended */
    case AFFINE:
      wm->wmmat[4] = (int32_t)lrint(model[4] * (1 << WARPEDMODEL_PREC_BITS));
      wm->wmmat[5] = (int32_t)lrint(model[5] * (1 << WARPEDMODEL_PREC_BITS));
    /* fallthrough intended */
    case ROTZOOM:
      wm->wmmat[2] = (int32_t)lrint(model[2] * (1 << WARPEDMODEL_PREC_BITS));
      wm->wmmat[3] = (int32_t)lrint(model[3] * (1 << WARPEDMODEL_PREC_BITS));
    /* fallthrough intended */
    case TRANSLATION:
      wm->wmmat[0] = (int32_t)lrint(model[0] * (1 << WARPEDMODEL_PREC_BITS));
      wm->wmmat[1] = (int32_t)lrint(model[1] * (1 << WARPEDMODEL_PREC_BITS));
      break;
    default: assert(0 && "Invalid TransformationType");
  }
}

///////////////////////////////////////////////////////////////////////////////
// svdcmp
// Adopted from Numerical Recipes in C

static const double TINY_NEAR_ZERO = 1.0E-12;

static INLINE double sign(double a, double b) {
  return ((b) >= 0 ? fabs(a) : -fabs(a));
}

static INLINE double pythag(double a, double b) {
  double ct;
  const double absa = fabs(a);
  const double absb = fabs(b);

  if (absa > absb) {
    ct = absb / absa;
    return absa * sqrt(1.0 + ct * ct);
  } else {
    ct = absa / absb;
    return (absb == 0) ? 0 : absb * sqrt(1.0 + ct * ct);
  }
}

static void multiply_mat(const double *m1, const double *m2, double *res,
                         const int m1_rows, const int inner_dim,
                         const int m2_cols) {
  double sum;

  int row, col, inner;
  for (row = 0; row < m1_rows; ++row) {
    for (col = 0; col < m2_cols; ++col) {
      sum = 0;
      for (inner = 0; inner < inner_dim; ++inner)
        sum += m1[row * inner_dim + inner] * m2[inner * m2_cols + col];
      *(res++) = sum;
    }
  }
}

static int svdcmp(double **u, int m, int n, double w[], double **v) {
  const int max_its = 30;
  int flag, i, its, j, jj, k, l, nm;
  double anorm, c, f, g, h, s, scale, x, y, z;
  double *rv1 = (double *)aom_malloc(sizeof(*rv1) * (n + 1));
  g = scale = anorm = 0.0;
  for (i = 0; i < n; i++) {
    l = i + 1;
    rv1[i] = scale * g;
    g = s = scale = 0.0;
    if (i < m) {
      for (k = i; k < m; k++) scale += fabs(u[k][i]);
      if (scale != 0.) {
        for (k = i; k < m; k++) {
          u[k][i] /= scale;
          s += u[k][i] * u[k][i];
        }
        f = u[i][i];
        g = -sign(sqrt(s), f);
        h = f * g - s;
        u[i][i] = f - g;
        for (j = l; j < n; j++) {
          for (s = 0.0, k = i; k < m; k++) s += u[k][i] * u[k][j];
          f = s / h;
          for (k = i; k < m; k++) u[k][j] += f * u[k][i];
        }
        for (k = i; k < m; k++) u[k][i] *= scale;
      }
    }
    w[i] = scale * g;
    g = s = scale = 0.0;
    if (i < m && i != n - 1) {
      for (k = l; k < n; k++) scale += fabs(u[i][k]);
      if (scale != 0.) {
        for (k = l; k < n; k++) {
          u[i][k] /= scale;
          s += u[i][k] * u[i][k];
        }
        f = u[i][l];
        g = -sign(sqrt(s), f);
        h = f * g - s;
        u[i][l] = f - g;
        for (k = l; k < n; k++) rv1[k] = u[i][k] / h;
        for (j = l; j < m; j++) {
          for (s = 0.0, k = l; k < n; k++) s += u[j][k] * u[i][k];
          for (k = l; k < n; k++) u[j][k] += s * rv1[k];
        }
        for (k = l; k < n; k++) u[i][k] *= scale;
      }
    }
    anorm = fmax(anorm, (fabs(w[i]) + fabs(rv1[i])));
  }

  for (i = n - 1; i >= 0; i--) {
    if (i < n - 1) {
      if (g != 0.) {
        for (j = l; j < n; j++) v[j][i] = (u[i][j] / u[i][l]) / g;
        for (j = l; j < n; j++) {
          for (s = 0.0, k = l; k < n; k++) s += u[i][k] * v[k][j];
          for (k = l; k < n; k++) v[k][j] += s * v[k][i];
        }
      }
      for (j = l; j < n; j++) v[i][j] = v[j][i] = 0.0;
    }
    v[i][i] = 1.0;
    g = rv1[i];
    l = i;
  }
  for (i = AOMMIN(m, n) - 1; i >= 0; i--) {
    l = i + 1;
    g = w[i];
    for (j = l; j < n; j++) u[i][j] = 0.0;
    if (g != 0.) {
      g = 1.0 / g;
      for (j = l; j < n; j++) {
        for (s = 0.0, k = l; k < m; k++) s += u[k][i] * u[k][j];
        f = (s / u[i][i]) * g;
        for (k = i; k < m; k++) u[k][j] += f * u[k][i];
      }
      for (j = i; j < m; j++) u[j][i] *= g;
    } else {
      for (j = i; j < m; j++) u[j][i] = 0.0;
    }
    ++u[i][i];
  }
  for (k = n - 1; k >= 0; k--) {
    for (its = 0; its < max_its; its++) {
      flag = 1;
      for (l = k; l >= 0; l--) {
        nm = l - 1;
        if ((double)(fabs(rv1[l]) + anorm) == anorm || nm < 0) {
          flag = 0;
          break;
        }
        if ((double)(fabs(w[nm]) + anorm) == anorm) break;
      }
      if (flag) {
        c = 0.0;
        s = 1.0;
        for (i = l; i <= k; i++) {
          f = s * rv1[i];
          rv1[i] = c * rv1[i];
          if ((double)(fabs(f) + anorm) == anorm) break;
          g = w[i];
          h = pythag(f, g);
          w[i] = h;
          h = 1.0 / h;
          c = g * h;
          s = -f * h;
          for (j = 0; j < m; j++) {
            y = u[j][nm];
            z = u[j][i];
            u[j][nm] = y * c + z * s;
            u[j][i] = z * c - y * s;
          }
        }
      }
      z = w[k];
      if (l == k) {
        if (z < 0.0) {
          w[k] = -z;
          for (j = 0; j < n; j++) v[j][k] = -v[j][k];
        }
        break;
      }
      if (its == max_its - 1) {
        aom_free(rv1);
        return 1;
      }
      assert(k > 0);
      x = w[l];
      nm = k - 1;
      y = w[nm];
      g = rv1[nm];
      h = rv1[k];
      f = ((y - z) * (y + z) + (g - h) * (g + h)) / (2.0 * h * y);
      g = pythag(f, 1.0);
      f = ((x - z) * (x + z) + h * ((y / (f + sign(g, f))) - h)) / x;
      c = s = 1.0;
      for (j = l; j <= nm; j++) {
        i = j + 1;
        g = rv1[i];
        y = w[i];
        h = s * g;
        g = c * g;
        z = pythag(f, h);
        rv1[j] = z;
        c = f / z;
        s = h / z;
        f = x * c + g * s;
        g = g * c - x * s;
        h = y * s;
        y *= c;
        for (jj = 0; jj < n; jj++) {
          x = v[jj][j];
          z = v[jj][i];
          v[jj][j] = x * c + z * s;
          v[jj][i] = z * c - x * s;
        }
        z = pythag(f, h);
        w[j] = z;
        if (z != 0.) {
          z = 1.0 / z;
          c = f * z;
          s = h * z;
        }
        f = c * g + s * y;
        x = c * y - s * g;
        for (jj = 0; jj < m; jj++) {
          y = u[jj][j];
          z = u[jj][i];
          u[jj][j] = y * c + z * s;
          u[jj][i] = z * c - y * s;
        }
      }
      rv1[l] = 0.0;
      rv1[k] = f;
      w[k] = x;
    }
  }
  aom_free(rv1);
  return 0;
}

static int SVD(double *U, double *W, double *V, double *matx, int M, int N) {
  // Assumes allocation for U is MxN
  double **nrU = (double **)aom_malloc((M) * sizeof(*nrU));
  double **nrV = (double **)aom_malloc((N) * sizeof(*nrV));
  int problem, i;

  problem = !(nrU && nrV);
  if (!problem) {
    for (i = 0; i < M; i++) {
      nrU[i] = &U[i * N];
    }
    for (i = 0; i < N; i++) {
      nrV[i] = &V[i * N];
    }
  } else {
    if (nrU) aom_free(nrU);
    if (nrV) aom_free(nrV);
    return 1;
  }

  /* copy from given matx into nrU */
  for (i = 0; i < M; i++) {
    memcpy(&(nrU[i][0]), matx + N * i, N * sizeof(*matx));
  }

  /* HERE IT IS: do SVD */
  if (svdcmp(nrU, M, N, W, nrV)) {
    aom_free(nrU);
    aom_free(nrV);
    return 1;
  }

  /* aom_free Numerical Recipes arrays */
  aom_free(nrU);
  aom_free(nrV);

  return 0;
}

int pseudo_inverse(double *inv, double *matx, const int M, const int N) {
  double ans;
  int i, j, k;
  double *const U = (double *)aom_malloc(M * N * sizeof(*matx));
  double *const W = (double *)aom_malloc(N * sizeof(*matx));
  double *const V = (double *)aom_malloc(N * N * sizeof(*matx));

  if (!(U && W && V)) {
    return 1;
  }
  if (SVD(U, W, V, matx, M, N)) {
    aom_free(U);
    aom_free(W);
    aom_free(V);
    return 1;
  }
  for (i = 0; i < N; i++) {
    if (fabs(W[i]) < TINY_NEAR_ZERO) {
      aom_free(U);
      aom_free(W);
      aom_free(V);
      return 1;
    }
  }

  for (i = 0; i < N; i++) {
    for (j = 0; j < M; j++) {
      ans = 0;
      for (k = 0; k < N; k++) {
        ans += V[k + N * i] * U[k + N * j] / W[k];
      }
      inv[j + M * i] = ans;
    }
  }
  aom_free(U);
  aom_free(W);
  aom_free(V);
  return 0;
}

static void normalize_homography(double *pts, int n, double *T) {
  double *p = pts;
  double mean[2] = { 0, 0 };
  double msqe = 0;
  double scale;
  int i;
  for (i = 0; i < n; ++i, p += 2) {
    mean[0] += p[0];
    mean[1] += p[1];
  }
  mean[0] /= n;
  mean[1] /= n;
  for (p = pts, i = 0; i < n; ++i, p += 2) {
    p[0] -= mean[0];
    p[1] -= mean[1];
    msqe += sqrt(p[0] * p[0] + p[1] * p[1]);
  }
  msqe /= n;
  scale = sqrt(2) / msqe;
  T[0] = scale;
  T[1] = 0;
  T[2] = -scale * mean[0];
  T[3] = 0;
  T[4] = scale;
  T[5] = -scale * mean[1];
  T[6] = 0;
  T[7] = 0;
  T[8] = 1;
  for (p = pts, i = 0; i < n; ++i, p += 2) {
    p[0] *= scale;
    p[1] *= scale;
  }
}

static void invnormalize_mat(double *T, double *iT) {
  double is = 1.0 / T[0];
  double m0 = -T[2] * is;
  double m1 = -T[5] * is;
  iT[0] = is;
  iT[1] = 0;
  iT[2] = m0;
  iT[3] = 0;
  iT[4] = is;
  iT[5] = m1;
  iT[6] = 0;
  iT[7] = 0;
  iT[8] = 1;
}

static void denormalize_homography(double *params, double *T1, double *T2) {
  double iT2[9];
  double params2[9];
  invnormalize_mat(T2, iT2);
  multiply_mat(params, T1, params2, 3, 3, 3);
  multiply_mat(iT2, params2, params, 3, 3, 3);
}

static void denormalize_homography_reorder(double *params, double *T1,
                                           double *T2) {
  double params_denorm[MAX_PARAMDIM];
  memcpy(params_denorm, params, sizeof(*params) * 8);
  params_denorm[8] = 1.0;
  denormalize_homography(params_denorm, T1, T2);
  params[0] = params_denorm[2];
  params[1] = params_denorm[5];
  params[2] = params_denorm[0];
  params[3] = params_denorm[1];
  params[4] = params_denorm[3];
  params[5] = params_denorm[4];
  params[6] = params_denorm[6];
  params[7] = params_denorm[7];
}

static void denormalize_affine_reorder(double *params, double *T1, double *T2) {
  double params_denorm[MAX_PARAMDIM];
  params_denorm[0] = params[0];
  params_denorm[1] = params[1];
  params_denorm[2] = params[4];
  params_denorm[3] = params[2];
  params_denorm[4] = params[3];
  params_denorm[5] = params[5];
  params_denorm[6] = params_denorm[7] = 0;
  params_denorm[8] = 1;
  denormalize_homography(params_denorm, T1, T2);
  params[0] = params_denorm[2];
  params[1] = params_denorm[5];
  params[2] = params_denorm[0];
  params[3] = params_denorm[1];
  params[4] = params_denorm[3];
  params[5] = params_denorm[4];
  params[6] = params[7] = 0;
}

static void denormalize_rotzoom_reorder(double *params, double *T1,
                                        double *T2) {
  double params_denorm[MAX_PARAMDIM];
  params_denorm[0] = params[0];
  params_denorm[1] = params[1];
  params_denorm[2] = params[2];
  params_denorm[3] = -params[1];
  params_denorm[4] = params[0];
  params_denorm[5] = params[3];
  params_denorm[6] = params_denorm[7] = 0;
  params_denorm[8] = 1;
  denormalize_homography(params_denorm, T1, T2);
  params[0] = params_denorm[2];
  params[1] = params_denorm[5];
  params[2] = params_denorm[0];
  params[3] = params_denorm[1];
  params[4] = -params[3];
  params[5] = params[2];
  params[6] = params[7] = 0;
}

static void denormalize_translation_reorder(double *params, double *T1,
                                            double *T2) {
  double params_denorm[MAX_PARAMDIM];
  params_denorm[0] = 1;
  params_denorm[1] = 0;
  params_denorm[2] = params[0];
  params_denorm[3] = 0;
  params_denorm[4] = 1;
  params_denorm[5] = params[1];
  params_denorm[6] = params_denorm[7] = 0;
  params_denorm[8] = 1;
  denormalize_homography(params_denorm, T1, T2);
  params[0] = params_denorm[2];
  params[1] = params_denorm[5];
  params[2] = params[5] = 1;
  params[3] = params[4] = 0;
  params[6] = params[7] = 0;
}

int find_translation(const int np, double *pts1, double *pts2, double *mat) {
  int i;
  double sx, sy, dx, dy;
  double sumx, sumy;

  double T1[9], T2[9];
  normalize_homography(pts1, np, T1);
  normalize_homography(pts2, np, T2);

  sumx = 0;
  sumy = 0;
  for (i = 0; i < np; ++i) {
    dx = *(pts2++);
    dy = *(pts2++);
    sx = *(pts1++);
    sy = *(pts1++);

    sumx += dx - sx;
    sumy += dy - sy;
  }
  mat[0] = sumx / np;
  mat[1] = sumy / np;
  denormalize_translation_reorder(mat, T1, T2);
  return 0;
}

int find_rotzoom(const int np, double *pts1, double *pts2, double *mat) {
  const int np2 = np * 2;
  double *a = (double *)aom_malloc(sizeof(*a) * np2 * 9);
  double *b = a + np2 * 4;
  double *temp = b + np2;
  int i;
  double sx, sy, dx, dy;

  double T1[9], T2[9];
  normalize_homography(pts1, np, T1);
  normalize_homography(pts2, np, T2);

  for (i = 0; i < np; ++i) {
    dx = *(pts2++);
    dy = *(pts2++);
    sx = *(pts1++);
    sy = *(pts1++);

    a[i * 2 * 4 + 0] = sx;
    a[i * 2 * 4 + 1] = sy;
    a[i * 2 * 4 + 2] = 1;
    a[i * 2 * 4 + 3] = 0;
    a[(i * 2 + 1) * 4 + 0] = sy;
    a[(i * 2 + 1) * 4 + 1] = -sx;
    a[(i * 2 + 1) * 4 + 2] = 0;
    a[(i * 2 + 1) * 4 + 3] = 1;

    b[2 * i] = dx;
    b[2 * i + 1] = dy;
  }
  if (pseudo_inverse(temp, a, np2, 4)) {
    aom_free(a);
    return 1;
  }
  multiply_mat(temp, b, mat, 4, np2, 1);
  denormalize_rotzoom_reorder(mat, T1, T2);
  aom_free(a);
  return 0;
}

int find_affine(const int np, double *pts1, double *pts2, double *mat) {
  const int np2 = np * 2;
  double *a = (double *)aom_malloc(sizeof(*a) * np2 * 13);
  double *b = a + np2 * 6;
  double *temp = b + np2;
  int i;
  double sx, sy, dx, dy;

  double T1[9], T2[9];
  normalize_homography(pts1, np, T1);
  normalize_homography(pts2, np, T2);

  for (i = 0; i < np; ++i) {
    dx = *(pts2++);
    dy = *(pts2++);
    sx = *(pts1++);
    sy = *(pts1++);

    a[i * 2 * 6 + 0] = sx;
    a[i * 2 * 6 + 1] = sy;
    a[i * 2 * 6 + 2] = 0;
    a[i * 2 * 6 + 3] = 0;
    a[i * 2 * 6 + 4] = 1;
    a[i * 2 * 6 + 5] = 0;
    a[(i * 2 + 1) * 6 + 0] = 0;
    a[(i * 2 + 1) * 6 + 1] = 0;
    a[(i * 2 + 1) * 6 + 2] = sx;
    a[(i * 2 + 1) * 6 + 3] = sy;
    a[(i * 2 + 1) * 6 + 4] = 0;
    a[(i * 2 + 1) * 6 + 5] = 1;

    b[2 * i] = dx;
    b[2 * i + 1] = dy;
  }
  if (pseudo_inverse(temp, a, np2, 6)) {
    aom_free(a);
    return 1;
  }
  multiply_mat(temp, b, mat, 6, np2, 1);
  denormalize_affine_reorder(mat, T1, T2);
  aom_free(a);
  return 0;
}

int find_homography(const int np, double *pts1, double *pts2, double *mat) {
  // Implemented from Peter Kovesi's normalized implementation
  const int np3 = np * 3;
  double *a = (double *)aom_malloc(sizeof(*a) * np3 * 18);
  double *U = a + np3 * 9;
  double S[9], V[9 * 9], H[9];
  int i, mini;
  double sx, sy, dx, dy;
  double T1[9], T2[9];

  normalize_homography(pts1, np, T1);
  normalize_homography(pts2, np, T2);

  for (i = 0; i < np; ++i) {
    dx = *(pts2++);
    dy = *(pts2++);
    sx = *(pts1++);
    sy = *(pts1++);

    a[i * 3 * 9 + 0] = a[i * 3 * 9 + 1] = a[i * 3 * 9 + 2] = 0;
    a[i * 3 * 9 + 3] = -sx;
    a[i * 3 * 9 + 4] = -sy;
    a[i * 3 * 9 + 5] = -1;
    a[i * 3 * 9 + 6] = dy * sx;
    a[i * 3 * 9 + 7] = dy * sy;
    a[i * 3 * 9 + 8] = dy;

    a[(i * 3 + 1) * 9 + 0] = sx;
    a[(i * 3 + 1) * 9 + 1] = sy;
    a[(i * 3 + 1) * 9 + 2] = 1;
    a[(i * 3 + 1) * 9 + 3] = a[(i * 3 + 1) * 9 + 4] = a[(i * 3 + 1) * 9 + 5] =
        0;
    a[(i * 3 + 1) * 9 + 6] = -dx * sx;
    a[(i * 3 + 1) * 9 + 7] = -dx * sy;
    a[(i * 3 + 1) * 9 + 8] = -dx;

    a[(i * 3 + 2) * 9 + 0] = -dy * sx;
    a[(i * 3 + 2) * 9 + 1] = -dy * sy;
    a[(i * 3 + 2) * 9 + 2] = -dy;
    a[(i * 3 + 2) * 9 + 3] = dx * sx;
    a[(i * 3 + 2) * 9 + 4] = dx * sy;
    a[(i * 3 + 2) * 9 + 5] = dx;
    a[(i * 3 + 2) * 9 + 6] = a[(i * 3 + 2) * 9 + 7] = a[(i * 3 + 2) * 9 + 8] =
        0;
  }

  if (SVD(U, S, V, a, np3, 9)) {
    aom_free(a);
    return 1;
  } else {
    double minS = 1e12;
    mini = -1;
    for (i = 0; i < 9; ++i) {
      if (S[i] < minS) {
        minS = S[i];
        mini = i;
      }
    }
  }

  for (i = 0; i < 9; i++) H[i] = V[i * 9 + mini];
  denormalize_homography_reorder(H, T1, T2);
  aom_free(a);
  if (H[8] == 0.0) {
    return 1;
  } else {
    // normalize
    double f = 1.0 / H[8];
    for (i = 0; i < 8; i++) mat[i] = f * H[i];
  }
  return 0;
}

#if CONFIG_WARPED_MOTION
int find_projection(const int np, double *pts1, double *pts2,
                    WarpedMotionParams *wm_params) {
  double H[9];
  int result = 1;

  switch (wm_params->wmtype) {
    case AFFINE: result = find_affine(np, pts1, pts2, H); break;
    case ROTZOOM: result = find_rotzoom(np, pts1, pts2, H); break;
    case HOMOGRAPHY: result = find_homography(np, pts1, pts2, H); break;
    default: assert(0 && "Invalid warped motion type!"); return 1;
  }
  if (result == 0) {
    av1_integerize_model(H, wm_params->wmtype, wm_params);

    if (wm_params->wmtype == ROTZOOM) {
      wm_params->wmmat[5] = wm_params->wmmat[2];
      wm_params->wmmat[4] = -wm_params->wmmat[3];
    }
  }
  if (wm_params->wmtype == AFFINE || wm_params->wmtype == ROTZOOM) {
    // check compatibility with the fast warp filter
    int32_t *mat = wm_params->wmmat;
    int32_t alpha, beta, gamma, delta;

    if (mat[2] == 0) return 1;

    alpha = mat[2] - (1 << WARPEDMODEL_PREC_BITS);
    beta = mat[3];
    gamma = ((int64_t)mat[4] << WARPEDMODEL_PREC_BITS) / mat[2];
    delta = mat[5] - (((int64_t)mat[3] * mat[4] + (mat[2] / 2)) / mat[2]) -
            (1 << WARPEDMODEL_PREC_BITS);

    if ((4 * abs(alpha) + 7 * abs(beta) > (1 << WARPEDMODEL_PREC_BITS)) ||
        (4 * abs(gamma) + 4 * abs(delta) > (1 << WARPEDMODEL_PREC_BITS))) {
      return 1;
    }
  }

  return result;
}
#endif  // CONFIG_WARPED_MOTION
