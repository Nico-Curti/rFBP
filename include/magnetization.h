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
  * @brief clamp value between boundaries
  *
  * @param x double value as argument of clamp
  * @param low lower boundary
  * @param high higher boundary
  *
  * @return double value clamped between boudaries
  */
                         double   clamp         (const double & x, const double & low, const double & high);
  /**
  * @brief log1p for magnetizations
  *
  * @param x double value
  *
  * @return double value computed as log1p(exp(-2*abs(x)))
  */
                         double   lr            (const double & x);
  /**
  * @brief sign for magnetizations
  *
  * @param x double value
  *
  * @return long int sign evaluated as 1 - 2*signbit(x)
  */
                         long int sign0         (const double & x);
  /**
  * @brief check if is infinite
  *
  * @tparam Mag magnetization type (MagP or MagT)
  * @param x double value
  *
  * @return MagP64: boolean true if is inf or -inf else false; MagT64: boolean true if is nan or -nan else false;
  */
  template < class Mag > bool     isinf         (const double & x);

  /**
  * @brief sign of magnetization
  *
  * @tparam Mag magnetization type (MagP or MagT)
  * @param m Mag
  *
  * @return boolean sign of magnetization
  */
  template < class Mag > bool     signbit       (const Mag & m);

  // template < class Mag > Mag      f2m           (const double & x);

  /**
  * @brief fill magnetization with zeros
  *
  * @tparam Mag magnetization type (MagP or MagT)
  * @param x mag array
  * @param n array size
  *
  * @return void
  */
  template < class Mag > void     zeros         (Mag * x, const long int & n);
  /**
  * @brief set magnetization to zero
  *
  * @tparam Mag magnetization type (MagP or MagT)
  * @param x mag object
  *
  * @return void
  */
  template < class Mag > void     zero          (Mag & x);
  /**
  * @brief magnetization abs
  *
  * @tparam Mag magnetization type (MagP or MagT)
  * @param a mag object
  *
  * @return double abs of magnetization
  */
  template < class Mag > double   abs           (const Mag & a);
  /**
  * @brief flip magnetization sign if necessary
  *
  * @tparam Mag magnetization type (MagP or MagT)
  * @param x mag object
  * @param y value with desired sign
  *
  * @return mag with the same sign of y
  */
  template < class Mag > Mag      copysign      (Mag & x, const double & y);
  /**
  * @brief arrow operator of original code
  *
  * @tparam Mag magnetization type (MagP or MagT)
  * @param m mag object
  * @param x value to multiply
  *
  * @return mag obtained as product of m * x if magT else x * atanh(m) if magP
  */
  template < class Mag > Mag      arrow         (const Mag & m, const double & x);
  /**
  * @brief magnetization sign
  *
  * @tparam Mag magnetization type (MagP or MagT)
  * @param x mag object
  *
  * @return sign computed as 1 - 2 * sign(x)
  */
  template < class Mag > long int sign0         (const Mag & x);
  /**
  * @brief magnetization log
  *
  * @tparam Mag magnetization type (MagP or MagT)
  * @param x mag object
  *
  * @return double as log( (1 + mag) / 2 )
  */
  template < class Mag > double   logmag2p      (const Mag & x);
  /**
  * @brief alias of constructor
  *
  * @tparam Mag magnetization type (MagP or MagT)
  * @param x double value
  *
  * @return magP(x) or magT(atanh(x), -30, 30)
  */
  template < class Mag > Mag      convert       (const double & x);
  /**
  * @brief convert mag to float
  *
  * @tparam Mag magnetization type (MagP or MagT)
  * @param x mag object
  *
  * @return extract magnetization (mag for MagP, value for MagT)
  */
  template < class Mag > double   convert       (const Mag & x);

  /**
  * @brief combine value to magnetization
  *
  * @tparam Mag magnetization type (MagP or MagT)
  * @param x1 double
  * @param x2 double
  *
  * @return (x1 - x2) / (x1 + x2) if MagP else log(x1) - log(x2) * .5
  */
  template < class Mag > Mag      couple        (const double & x1, const double & x2);
  /**
  * @brief update magnetization
  *
  * @tparam Mag magnetization type (MagP or MagT)
  * @param newx mag object
  * @param oldx mag object
  * @param l double value
  *
  * @return newx * (1 - l) + oldx * l
  */
  template < class Mag > Mag      damp          (const Mag & newx, const Mag & oldx, const double & l);
  /**
  * @brief tanh of magnetization
  *
  * @tparam Mag magnetization type (MagP or MagT)
  * @param x double value
  *
  * @return Mag with tanh(x) if MagP else x
  */
  template < class Mag > Mag      mtanh         (const double & x);

  // template < class Mag > Mag      matanh        (const Mag & m);

  /**
  * @brief atanherf of magnetization
  *
  * @tparam Mag magnetization type (MagP or MagT)
  * @param x double value
  *
  * @return Mag with erf(x) if MagP else atanherf(x)
  */
  template < class Mag > Mag      merf          (const double & x);
  /**
  * @brief diff of magnetizations
  *
  * @tparam Mag magnetization type (MagP or MagT)
  * @param m1 mag object
  * @param m2 mag object
  *
  * @return Mag with (m1 - m2)/(1 - m1 * m2) clamped to [-1, 1] if MagP else m1 - m2
  */
  template < class Mag > Mag      bar           (const Mag & m1, const Mag & m2);
  template < class Mag > double   log1pxy       (const Mag & x,  const Mag & y);
  template < class Mag > double   mcrossentropy (const Mag & x,  const Mag & y);
  template < class Mag > double   logZ          (const Mag & u0, const Mag * u,     const long int & nu);
  template < class Mag > Mag      auxmix        (const Mag & H,  const double & ap, const double & am);
  template < class Mag > Mag      erfmix        (const Mag & H,  const double & mp, const double & mm);
  template < class Mag > Mag      exactmix      (const Mag & H,  const Mag & pp,    const Mag & pm);
}

#endif // __cplusplus

#include <magP.h>
#include <magT.h>

#endif // __mag_h__
