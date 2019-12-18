#ifndef __mag_hpp__
#define __mag_hpp__

#include <magnetization.h>

namespace mag
{

  double clamp (const double & x, const double & low, const double & high)
  {
    return std :: clamp(x, low, high);
  }

  double lr (const double & x)
  {
    return std :: log1p(std :: exp(-2. * std :: abs(x)));
  }

  long int sign0 (const double & x)
  {
    return std :: signbit(x) ? 1L : -1L;
  }

  bool isinf (const double & x)
  {
    return ( ( (x == INF) || (x == -INF) ) || (std :: isinf(x)) );
  }

  template < class Mag >
  bool signbit (const Mag & m)
  {
    static_assert( std :: is_same < Mag, MagP64 > :: value ||
                   std :: is_same < Mag, MagT64 > :: value,
                   "signbit function! Incompatible types found.");
    return std :: signbit(m.mag);
  }

  //template < class Mag >
  //Mag f2m (const double & x)
  //{
  //  static_assert( std :: is_same < Mag, MagP64 > :: value ||
  //                 std :: is_same < Mag, MagT64 > :: value,
  //                 "f2m function! Incompatible types found.");
  //  return Mag(x);
  //}

  template < class Mag >
  void zeros (Mag * x, const long int & n)
  {
    static_assert( std :: is_same < Mag, MagP64 > :: value ||
                   std :: is_same < Mag, MagT64 > :: value,
                   "zeros function! Incompatible types found.");
    std :: fill_n(x, n, 0.);
  }

  template < class Mag >
  void zero (Mag & x)
  {
    static_assert( std :: is_same < Mag, MagP64 > :: value ||
                   std :: is_same < Mag, MagT64 > :: value,
                   "zero function! Incompatible types found.");
    x.mag = 0.;
  }

  template < class Mag >
  double abs (const Mag & a)
  {
    static_assert( std :: is_same < Mag, MagP64 > :: value ||
                   std :: is_same < Mag, MagT64 > :: value,
                   "abs function! Incompatible types found.");
    return std :: abs(a.mag);
  }

  template < class Mag >
  Mag copysign (Mag & x, const double & y)
  {
    static_assert( std :: is_same < Mag, MagP64 > :: value ||
                   std :: is_same < Mag, MagT64 > :: value,
                   "copysign function! Incompatible types found.");
    return signbit(x) != std :: signbit(y) ? -x : x;
  }

  template < class Mag >
  Mag arrow (const Mag & m, const double & x)
  {
    static_assert( std :: is_same < Mag, MagP64 > :: value ||
                   std :: is_same < Mag, MagT64 > :: value,
                   "arrow function! Incompatible types found.");
    if constexpr ( std :: is_same < Mag, MagP64 > :: value )
      return mtanh < Mag >(x * std :: atanh(m.mag));
    else
      return mtanh < Mag >(x * m.mag);
  }

  template < class Mag >
  long int sign0 (const Mag & x)
  {
    static_assert( std :: is_same < Mag, MagP64 > :: value ||
                   std :: is_same < Mag, MagT64 > :: value,
                   "sign0 function! Incompatible types found.");
    return signbit(x) ? 1L : -1L;
  }

  template < class Mag >
  double logmag2p (const Mag & x)
  {
    static_assert( std :: is_same < Mag, MagP64 > :: value ||
                   std :: is_same < Mag, MagT64 > :: value,
                   "logmag2p function! Incompatible types found.");
    return std :: log( (1. + x.mag) * .5 );
  }

  template < class Mag >
  Mag convert (const double & x)
  {
    static_assert( std :: is_same < Mag, MagP64 > :: value ||
                   std :: is_same < Mag, MagT64 > :: value,
                   "convert function! Incompatible types found.");
    if constexpr ( std :: is_same < Mag, MagP64 > :: value )
      return MagP64(x);
    else
      return MagT64(clamp(std :: atanh(x), -30., 30.));
  }

  template < class Mag >
  double convert (const Mag & x)
  {
    static_assert( std :: is_same < Mag, MagP64 > :: value ||
                   std :: is_same < Mag, MagT64 > :: value,
                   "convert function! Incompatible types found.");
    return x.value();
  }

  template < class Mag >
  Mag couple (const double & x1, const double & x2)
  {
    static_assert( std :: is_same < Mag, MagP64 > :: value ||
                   std :: is_same < Mag, MagT64 > :: value,
                   "couple function! Incompatible types found.");

    if constexpr ( std :: is_same < Mag, MagP64 > :: value )
      return MagP64( (x1 - x2) / (x1 + x2) );
    else
      return convert < MagT64 >((std :: log(x1 / x2)) * .5);
  }


  template < class Mag >
  Mag damp (const Mag & newx, const Mag & oldx, const double & l)
  {
    static_assert( std :: is_same < Mag, MagP64 > :: value ||
                   std :: is_same < Mag, MagT64 > :: value,
                   "damp function! Incompatible types found.");

    if constexpr ( std :: is_same < Mag, MagP64 > :: value )
      return MagP64(newx.value() * (1. - l) + oldx.value() * l);
    else
      return convert < MagT64 >( newx.value() * (1. - l) + oldx.value() * l );
  }

  template < class Mag >
  Mag mtanh (const double & x)
  {
    static_assert( std :: is_same < Mag, MagP64 > :: value ||
                   std :: is_same < Mag, MagT64 > :: value,
                   "mtanh function! Incompatible types found.");

    if constexpr ( std :: is_same < Mag, MagP64 > :: value )
      return MagP64(std :: tanh(x));
    else
      return MagT64(x);
  }

  //template < class Mag >
  //Mag matanh (const Mag & m)
  //{
  //  static_assert( std :: is_same < Mag, MagT64 > :: value,
  //                 "matanh function! Incompatible type found. You must use MagT64 variable for this function." );
  //  return MagT64(m.mag);
  //}

  template < class Mag >
  Mag merf (const double & x)
  {
    static_assert( std :: is_same < Mag, MagP64 > :: value ||
                   std :: is_same < Mag, MagT64 > :: value,
                   "merf function! Incompatible types found.");

    if constexpr ( std :: is_same < Mag, MagP64 > :: value )
      return MagP64(std :: erf(x));
    else
      return MagT64(AtanhErf :: atanherf(x));
  }

  template < class Mag >
  Mag bar (const Mag & m1, const Mag & m2)
  {
    static_assert( std :: is_same < Mag, MagP64 > :: value ||
                   std :: is_same < Mag, MagT64 > :: value,
                   "bar function! Incompatible types found.");

    if constexpr ( std :: is_same < Mag, MagP64 > :: value )
      return ((m1.mag == m2.mag) ?
              MagP64(0.)         :
              MagP64(clamp( (m1.mag - m2.mag) / (1. - m1.mag * m2.mag), -1., 1.)));
    else
      return ((m1.mag == m2.mag) ?
              MagT64(0.)         :
              MagT64(m1.mag - m2.mag));
  }


  template < class Mag >
  double log1pxy (const Mag & x, const Mag & y)
  {
    static_assert( std :: is_same < Mag, MagP64 > :: value ||
                   std :: is_same < Mag, MagT64 > :: value,
                   "log1pxy function! Incompatible types found.");

    if constexpr ( std :: is_same < Mag, MagP64 > :: value )
      return std :: log( ( 1. + (x.mag * y.mag)) * .5);
    else
    {
      const double ax = x.mag;
      const double ay = y.mag;

      return !mag :: isinf(ax) && !mag :: isinf(ay)                                                 ?
              std :: abs(ax + ay) - std :: abs(ax) - std :: abs(ay) + lr(ax + ay) - lr(ax) - lr(ay) :
              mag :: isinf(ax) && !mag :: isinf(ay) ? sign(ax) * ay - std :: abs(ay) - lr(ay)       :
             !mag :: isinf(ax) &&  mag :: isinf(ay) ? sign(ay) * ax - std :: abs(ax) - lr(ax)       :
              sign(ax) == sign(ay) ? 0. : -INF;
    }
  }

  template < class Mag >
  double mcrossentropy (const Mag & x, const Mag & y)
  {
    static_assert( std :: is_same < Mag, MagP64 > :: value ||
                   std :: is_same < Mag, MagT64 > :: value,
                   "mcrossentropy function! Incompatible types found.");

    if constexpr ( std :: is_same < Mag, MagP64 > :: value )
      return (-x.mag) * std :: atanh(y.mag) - std :: log(1. - y.mag * y.mag) * .5 + log_2;
    else
    {
      const double tx = x.value();
      const double ay = y.mag;
      return !mag :: isinf(ay)                                ?
             -std :: abs(ay) * (sign0(ay) * tx - 1.) + lr(ay) :
             (sign(tx) != sign(ay))                           ?
             INF                                              :
             0.;
    }
  }

  template < class Mag >
  double logZ (const Mag & u0, const Mag * u, const long int & nu)
  {
    static_assert( std :: is_same < Mag, MagP64 > :: value ||
                   std :: is_same < Mag, MagT64 > :: value,
                   "log1pxy function! Incompatible types found.");

    if constexpr ( std :: is_same < Mag, MagP64 > :: value )
    {
      double zkip = (1. + u0.mag) * .5;
      double zkim = (1. - u0.mag) * .5;

      for (long int i = 0L; i < nu; ++i)
      {
        zkip *= (1. + u[i].mag) * .5;
        zkim *= (1. - u[i].mag) * .5;
      }
      return std :: log( zkip + zkim );
    }
    else
    {
      const double a0   = u0.mag;
      const bool is_inf = mag :: isinf(a0);
      double s1     = is_inf ? 0.       : a0;
      double s2     = is_inf ? 0.       : std :: abs(a0);
      double s3     = is_inf ? 1.       : a0;
      double hasinf = is_inf ? sign(a0) : 0.;
      for (long int i = 0L; i < nu; ++i)
      {
        const double ai = u[i].mag;
        if (!mag :: isinf(ai))
        {
          s1 += ai;
          s2 += std :: abs(ai);
          s3 *= ai;
        }
        else if (hasinf == 0)        hasinf = sign(ai);
        else if (hasinf != sign(ai)) return -INF;
      }
      return std :: abs(s1) - s2 + lr(s1 / s3);
    }
  }

  template < class Mag >
  Mag auxmix (const Mag & H, const double & ap, const double & am)
  {
    static_assert( std :: is_same < Mag, MagT64 > :: value,
                   "auxmix function! Incompatible type found. You must use MagT64 variable for this function." );

    const double aH = H.mag;
    double t1, t2;
    if (aH == 0.) return MagT64(0.);
    else
    {
      const double xH   = aH + ap;
      const double xh   = aH + am;
      const double a_ap = std :: abs(ap);
      const double a_am = std :: abs(am);
      const bool inf_ap = mag :: isinf(ap);
      const bool inf_am = mag :: isinf(am);
      if (mag :: isinf(aH))
      {
        if (!inf_ap && !inf_am)
        {
          t1 = sign(aH) * (ap - am) - a_ap + a_am;
          t2 = -lr(ap) + lr(am);
        }
        else if (inf_ap && !inf_am)
        {
          t1 = ( sign(ap) == sign(aH) ) ? -sign(aH) * am + a_am : -2. * H.mInf;
          t2 = ( sign(ap) == sign(aH) ) ? lr(am)                : 0.;
        }
        else if (!inf_ap && inf_am)
        {
          t1 = ( sign(am) == sign(aH) ) ? sign(aH) * ap - a_ap : 2. * H.mInf;
          t2 = ( sign(am) == sign(aH) ) ? -lr(ap)              : 0.;
        }
        else
        {
          t2 = 0.;
          // if ( (sign(ap) == sign(aH) && sign(ap) == sign(aH)) || ((sign(ap) != sign(aH) && sign(ap) != sign(aH))) )
          if ( (sign(ap) == sign(aH) && sign(am) == sign(aH)) || ((sign(ap) != sign(aH) && sign(am) != sign(aH))) ) t1 = 0.;
          else if (sign(ap) == sign(aH))  t1 = 2. * H.mInf;
          else  t1 = -2. * H.mInf;
        }
      }
      else
      {
        t1  = 0.;
        t1 += inf_ap ? 0. : std :: abs(xH) - a_ap;
        t1 -= inf_am ? 0. : std :: abs(xh) - a_am;
        t2 = std :: log( ( std :: exp(2. * std :: abs(xH)) + 1.) * ( std :: exp(2. * std :: abs(am)) + 1.) / (( std :: exp(2. * std :: abs(ap)) + 1.) * ( std :: exp(2. * std :: abs(xh)) + 1.))) -2. * std :: abs(xH) + 2. * std :: abs(ap) + 2. * std :: abs(xh) - 2. * std :: abs(am);
        // t2  = lr(xH) - lr(ap) - lr(xh) + lr(am);
      }
    }
    return MagT64((t1 + t2) * .5);
  }


  template < class Mag >
  Mag erfmix (const Mag & H, const double & mp, const double & mm)
  {
    static_assert( std :: is_same < Mag, MagP64 > :: value ||
                   std :: is_same < Mag, MagT64 > :: value,
                   "erfmix function! Incompatible types found.");

    if constexpr ( std :: is_same < Mag, MagP64 > :: value )
      return MagP64( H.mag * (std :: erf(mp) - std :: erf(mm)) / ( 2. + H.mag * (std :: erf(mp) + std :: erf(mm))));
    else
      return auxmix(H, AtanhErf :: atanherf(mp), AtanhErf :: atanherf(mm));
  }

  template < class Mag >
  Mag exactmix (const Mag & H, const Mag & pp, const Mag & pm)
  {
    static_assert( std :: is_same < Mag, MagP64 > :: value ||
                   std :: is_same < Mag, MagT64 > :: value,
                   "exactmix function! Incompatible types found.");

    if constexpr ( std :: is_same < Mag, MagP64 > :: value )
      return MagP64( (pp.mag - pm.mag) * H.mag / (2. + (pp.mag + pm.mag) * H.mag) );
    else
      return auxmix(H, pp.mag, pm.mag);
  }

}

#endif // __mag_hpp__
