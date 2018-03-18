/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef _OKAPI_MATHUTIL_HPP_
#define _OKAPI_MATHUTIL_HPP_

namespace okapi {
static constexpr double analogInToV = 286.0;
static constexpr double inchToMM = 25.4;
static constexpr double degreeToRadian = 0.01745;
static constexpr double radianToDegree = 57.2958;
static constexpr double imeTorqueTPR = 627.2;
static constexpr double imeSpeedTPR = 392.0;
static constexpr double imeTurboTPR = 261.333;
static constexpr double ime269TPR = 240.448;
static constexpr double imev5TPR = 1800.0;
static constexpr double quadEncoderTPR = 360.0;
static constexpr double pi = 3.14159265358979323846;

/**
 * Integer power function. Computes x^n.
 *
 * @param x base
 * @param n exponent
 * @return x^n
 */
constexpr double ipow(double x, int n) {
  return (n == 0)
           ? 1
           : n == 1 ? x
                    : n > 1 ? ((n & 1) ? x * ipow(x, n - 1) : ipow(x, n / 2) * ipow(x, n / 2))
                            : 1 / ipow(x, -n);
}
} // namespace okapi

#endif
