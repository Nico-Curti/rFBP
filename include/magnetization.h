#ifndef __mag_h__
#define __mag_h__

#include <iostream>
#include <type_traits>
#include <cmath>
#include <string>
#include <algorithm>

#if (__cplusplus < 201700) && !(__clang_major__ > 4)

  #include <magnetization_oldgcc.h>

#else

namespace mag
{
  /**
  * @brief Clamp value between boundaries.
  *
  * @param x double value as argument of clamp
  * @param low lower boundary
  * @param high higher boundary
  *
  * @return value clamped between boudaries
  */
                         double   clamp         (const double & x, const double & low, const double & high);
  /**
  * @brief log1p for magnetizations.
  *
  * @param x double value
  *
  * @return value computed as log1p(exp(-2*abs(x)))
  */
                         double   lr            (const double & x);
  /**
  * @brief Sign operation valid also for magnetizations.
  *
  * @param x double value
  *
  * @return sign evaluated as 1 - 2*signbit(x)
  */
                         long int sign0         (const double & x);
  /**
  * @brief Check if is infinite.
  *
  * @tparam Mag magnetization type (MagP or MagT)
  * @param x double value
  *
  * @return result of the check
  *
  * @note The function behavior is different between MagP64 and MagT64.
  * - MagP64: boolean true if is inf or -inf else false
  * - MagT64: boolean true if is nan or -nan else false
  */
  template < class Mag > bool     isinf         (const double & x);

  /**
  * @brief Get the sign of magnetization.
  *
  * @tparam Mag magnetization type (MagP or MagT)
  * @param m Mag
  *
  * @return boolean sign of magnetization
  */
  template < class Mag > bool     signbit       (const Mag & m);

  // template < class Mag > Mag      f2m           (const double & x);

  /**
  * @brief Fill a magnetization array with zeros.
  *
  * @tparam Mag magnetization type (MagP or MagT)
  * @param x mag array
  * @param n array size
  *
  * @return void
  */
  template < class Mag > void     zeros         (Mag * x, const long int & n);
  /**
  * @brief Set magnetization to zero.
  *
  * @tparam Mag magnetization type (MagP or MagT)
  * @param x mag object
  *
  * @return void
  */
  template < class Mag > void     zero          (Mag & x);
  /**
  * @brief Abs for magnetization objects
  *
  * @tparam Mag magnetization type (MagP or MagT)
  * @param a mag object
  *
  * @return The absolute value of the input
  */
  template < class Mag > double   abs           (const Mag & a);
  /**
  * @brief Flip magnetization sign if necessary.
  *
  * @tparam Mag magnetization type (MagP or MagT)
  * @param x mag object
  * @param y value with desired sign
  *
  * @return The corrected mag object
  */
  template < class Mag > Mag      copysign      (Mag & x, const double & y);
  /**
  * @brief Arrow operator of original code.
  *
  * @tparam Mag magnetization type (MagP or MagT)
  * @param m mag object
  * @param x value to multiply
  *
  * @return The result of the operator.
  *
  * @note The function behavior is different between MagP64 and MagT64.
  * - MagP64: x * atanh(m)
  * - MagT64: m * x
  */
  template < class Mag > Mag      arrow         (const Mag & m, const double & x);
  /**
  * @brief Get magnetization sign.
  *
  * @tparam Mag magnetization type (MagP or MagT)
  * @param x mag object
  *
  * @return sign computed as 1 - 2 * sign(x)
  */
  template < class Mag > long int sign0         (const Mag & x);
  /**
  * @brief Log operation for magnetization objects.
  *
  * @tparam Mag magnetization type (MagP or MagT)
  * @param x mag object
  *
  * @return The result of the operation
  */
  template < class Mag > double   logmag2p      (const Mag & x);
  /**
  * @brief Convert a double to a mag value (as a constructor).
  *
  * @tparam Mag magnetization type (MagP or MagT)
  * @param x double value
  *
  * @return magnetization
  *
  * @note The function behavior is different between MagP64 and MagT64.
  * - MagP64: MagP64(x)
  * - MagT64: MagT64(atanh(x), -30, 30)
  */
  template < class Mag > Mag      convert       (const double & x);
  /**
  * @brief Convert a mag to double.
  *
  * @tparam Mag magnetization type (MagP or MagT)
  * @param x mag object
  *
  * @return extract magnetization.
  *
  * @note The function behavior is different between MagP64 and MagT64.
  * - MagP64: return mag
  * - MagT64: return value
  */
  template < class Mag > double   convert       (const Mag & x);

  /**
  * @brief Combine values to magnetizations.
  *
  * @tparam Mag magnetization type (MagP or MagT)
  * @param x1 double
  * @param x2 double
  *
  * @return The result of the combination.
  *
  * @note The function behavior is different between MagP64 and MagT64.
  * - MagP64: (x1 - x2) / (x1 + x2)
  * - MagT64: log(x1) - log(x2) * .5
  */
  template < class Mag > Mag      couple        (const double & x1, const double & x2);
  /**
  * @brief Update magnetization.
  *
  * @tparam Mag magnetization type (MagP or MagT)
  * @param newx mag object
  * @param oldx mag object
  * @param l double value
  *
  * @return The result of the update computed as newx * (1 - l) + oldx * l
  */
  template < class Mag > Mag      damp          (const Mag & newx, const Mag & oldx, const double & l);
  /**
  * @brief Perform `tanh` on magnetization value.
  *
  * @tparam Mag magnetization type (MagP or MagT)
  * @param x double value
  *
  * @return The result of `tanh` as Mag.
  *
  * @note The function behavior is different between MagP64 and MagT64.
  * - MagP64: tanh(x)
  * - MagT64: x
  */
  template < class Mag > Mag      mtanh         (const double & x);

  // template < class Mag > Mag      matanh        (const Mag & m);

  /**
  * @brief Perform `erf` on magnetization value.
  *
  * @tparam Mag magnetization type (MagP or MagT)
  * @param x double value
  *
  * @return The result of `atanherf(x)`.
  *
  * @note The function behavior is different between MagP64 and MagT64.
  * - MagP64: erf(x)
  * - MagT64: atanherf(x)
  */
  template < class Mag > Mag      merf          (const double & x);
  /**
  * @brief Diff of magnetizations.
  *
  * @tparam Mag magnetization type (MagP or MagT)
  * @param m1 mag object
  * @param m2 mag object
  *
  * @return The result of the diff.
  *
  * @note The function behavior is different between MagP64 and MagT64.
  * - MagP64: (m1 - m2)/(1 - m1 * m2) clamped to [-1, 1]
  * - MagT64: m1 - m2
  */
  template < class Mag > Mag      bar           (const Mag & m1, const Mag & m2);
  /**
  * @brief Compute the log1p for the combination of the magnetizations.
  *
  * @tparam Mag magnetization type (MagP or MagT)
  * @param x mag object
  * @param y mag object
  *
  * @return The result of the operation
  *
  * @note The function behavior is different between MagP64 and MagT64.
  * - MagP64: log((1. + (x.mag * y.mag)) * 0.5)
  * - MagT64: computation takes care of possible number overflows
  */
  template < class Mag > double   log1pxy       (const Mag & x,  const Mag & y);
  /**
  * @brief Compute the crossentropy score for magnetization objects
  *
  * @tparam Mag magnetization type (MagP or MagT)
  * @param x mag object
  * @param y mag object
  *
  * @return The resulting crossentropy score
  *
  * @note The function behavior is different between MagP64 and MagT64.
  * - MagP64: -x.mag * np.arctanh(y.mag) - np.log1p(- y.mag**2) * .5 + np.log(2)
  * - MagT64: -abs(y.mag) * (sign0(y.mag) * x.value - 1.) + lr(y.mag)
  */
  template < class Mag > double   mcrossentropy (const Mag & x,  const Mag & y);
  template < class Mag > double   logZ          (const Mag & u0, const Mag * u,     const long int & nu);
  /**
  * @brief Combine three MagT64 variables.
  *
  * @tparam Mag magnetization type (MagP or MagT)
  * @param H mag object
  * @param ap double
  * @param am double
  *
  * @return combination of the input
  *
  * @note This operation is valid only for MagT64 variables up to now
  */
  template < class Mag > Mag      auxmix        (const Mag & H,  const double & ap, const double & am);
  /**
  * @brief Combine exactly three magnetizations.
  *
  * @tparam Mag magnetization type (MagP or MagT)
  * @param H mag object
  * @param mp double
  * @param mm double
  *
  * @return The result of the mix
  *
  * @note The function behavior is different between MagP64 and MagT64.
  * - MagP64: H.mag * (erf(mp) - erf(mm)) / (2. + H.mag * (erf(mp) + erf(mm)))
  * - MagT64: auxmix(H, atanherf(mp), atanherf(mm))
  */
  template < class Mag > Mag      erfmix        (const Mag & H,  const double & mp, const double & mm);
  /**
  * @brief Combine exactly three magnetizations.
  *
  * @tparam Mag magnetization type (MagP or MagT)
  * @param H mag object
  * @param pp mag object
  * @param pm mag object
  *
  * @return The result of the mix
  *
  * @note The function behavior is different between MagP64 and MagT64.
  * - MagP64: (pp.mag - pm.mag) * H.mag / (2. + (pp.mag + pm.mag) * H.mag)
  * - MagT64: auxmix(H, pp.mag, pm.mag)
  */
  template < class Mag > Mag      exactmix      (const Mag & H,  const Mag & pp,    const Mag & pm);
}

#endif // __cplusplus

#include <magP.h>
#include <magT.h>

#endif // __mag_h__
