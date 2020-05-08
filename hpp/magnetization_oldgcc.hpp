#ifndef __mag_old_hpp__
#define __mag_old_hpp__

#if (__cplusplus < 201700) && !(__clang_major__ > 4)

#include <magnetization_oldgcc.h>
#include <atanherf.h>
#include <climits>

namespace std
{
  double clamp (const double & x, const double & low, const double & high)
  {
    return (low <= x && x <= high ) ? x :
           ( x < low )              ? low :
                                      high;
  }
}


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
    return std :: signbit(x) ? -1L : 1L;
  }

  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagP64 > :: value > :: type * >
  bool isinf (const double & x)
  {
    return ( ( (x == INF) || (x == -INF) ) || (std :: isinf(x)) );
  }

  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * >
  bool isinf (const double & x)
  {
    return ( ( (x == INF) || (x == -INF) ) || (std :: isinf(x)) || (std :: isnan(x)) );
  }

  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagP64 > :: value > :: type * >
  bool signbit (const Mag & m)
  {
    return std :: signbit(m.mag);
  }
  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * >
  bool signbit (const Mag & m)
  {
    return std :: signbit(m.mag);
  }

  //template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagP64 > :: value > :: type * >
  //Mag f2m (const double & x)
  //{
  //  return Mag(x);
  //}
  //template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * >
  //Mag f2m (const double & x)
  //{
  //  return Mag(x);
  //}

  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagP64 > :: value > :: type * >
  void zeros (Mag * x, const long int & n)
  {
    std :: fill_n(x, n, 0.);
  }
  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * >
  void zeros (Mag * x, const long int & n)
  {
    std :: fill_n(x, n, 0.);
  }

  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagP64 > :: value > :: type * >
  void zero (Mag & x)
  {
    x.mag = 0.;
  }
  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * >
  void zero (Mag & x)
  {
    x.mag = 0.;
  }


  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagP64 > :: value > :: type * >
  double abs (const Mag & a)
  {
    return std :: abs(a.mag);
  }
  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * >
  double abs (const Mag & a)
  {
    return std :: abs(a.mag);
  }


  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagP64 > :: value > :: type * >
  Mag copysign (Mag & x, const double & y)
  {
    return signbit(x) != std :: signbit(y) ? -x : x;
  }
  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * >
  Mag copysign (Mag & x, const double & y)
  {
    return signbit(x) != std :: signbit(y) ? -x : x;
  }

  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagP64 > :: value > :: type * >
  Mag arrow (const Mag & m, const double & x)
  {
    return mtanh < Mag >(x * std :: atanh(m.mag));
  }
  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * >
  Mag arrow (const Mag & m, const double & x)
  {
    return mtanh < Mag >(x * m.mag);
  }


  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagP64 > :: value > :: type * >
  long int sign0 (const Mag & x)
  {
    return signbit(x) ? -1L : 1L;
  }
  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * >
  long int sign0 (const Mag & x)
  {
    return signbit(x) ? -1L : 1L;
  }

  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagP64 > :: value > :: type * >
  double logmag2p (const Mag & x)
  {
    return std :: log( (1. + x.mag) * .5 );
  }
  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * >
  double logmag2p (const Mag & x)
  {
    return std :: log( (1. + x.mag) * .5 );
  }

  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagP64 > :: value > :: type * >
  Mag convert (const double & x)
  {
    return MagP64(x);
  }
  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * >
  Mag convert (const double & x)
  {
    return MagT64(clamp(std :: atanh(x), -30., 30.));
  }


  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagP64 > :: value > :: type * >
  double convert (const Mag & x)
  {
    return x.value();
  }
  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * >
  double convert (const Mag & x)
  {
    return x.value();
  }

  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagP64 > :: value > :: type * >
  Mag couple (const double & x1, const double & x2)
  {
    return MagP64( (x1 - x2) / (x1 + x2) );
  }
  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * >
  Mag couple (const double & x1, const double & x2)
  {
    return MagT64(clamp((std :: log(x1 / x2)) * .5, -30., 30.));
  }


  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagP64 > :: value > :: type * >
  Mag damp (const Mag & newx, const Mag & oldx, const double & l)
  {
    return MagP64(newx.mag * (1. - l) + oldx.mag * l);
  }
  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * >
  Mag damp (const Mag & newx, const Mag & oldx, const double & l)
  {
    return convert < MagT64 >( newx.value() * (1. - l) + oldx.value() * l );
  }

  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagP64 > :: value > :: type * >
  Mag mtanh (const double & x)
  {
    return MagP64(std :: tanh(x));
  }
  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * >
  Mag mtanh (const double & x)
  {
    return MagT64(x);
  }

  //template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * >
  //Mag matanh (const Mag & m)
  //{
  //  return MagT64(m.mag);
  //}

  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagP64 > :: value > :: type * >
  Mag merf (const double & x)
  {
    return MagP64(std :: erf(x));
  }
  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * >
  Mag merf (const double & x)
  {
    return MagT64(AtanhErf :: atanherf(x));
  }

  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagP64 > :: value > :: type * >
  Mag bar (const Mag & m1, const Mag & m2)
  {
    return ((m1.mag == m2.mag) ?
            MagP64(0.)         :
            MagP64(clamp( (m1.mag - m2.mag) / (1. - m1.mag * m2.mag), -1., 1.)));
  }
  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * >
  Mag bar (const Mag & m1, const Mag & m2)
  {
    return ((m1.mag == m2.mag) ?
            MagT64(0.)         :
            MagT64(m1.mag - m2.mag));
  }

  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagP64 > :: value > :: type * >
  double log1pxy (const Mag & x, const Mag & y)
  {
    return std :: log( ( 1. + (x.mag * y.mag)) * .5);
  }
  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * >
  double log1pxy (const Mag & x, const Mag & y)
  {
    const double ax = x.mag,
                  ay = y.mag;

    return !mag :: isinf < MagT64 >(ax) && !mag :: isinf < MagT64 >(ay)                                                 ?
            std :: abs(ax + ay) - std :: abs(ax) - std :: abs(ay) + lr(ax + ay) - lr(ax) - lr(ay) :
            mag :: isinf < MagT64 >(ax) && !mag :: isinf < MagT64 >(ay) ? sign(ax) * ay - std :: abs(ay) - lr(ay)       :
           !mag :: isinf < MagT64 >(ax) &&  mag :: isinf < MagT64 >(ay) ? sign(ay) * ax - std :: abs(ax) - lr(ax)       :
            sign(ax) == sign(ay) ? 0. : -INF;
  }

  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagP64 > :: value > :: type * >
  double mcrossentropy (const Mag & x, const Mag & y)
  {
    return (-x.mag) * std :: atanh(y.mag) - std :: log1p(- y.mag * y.mag) * .5 + log_2;
  }
  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * >
  double mcrossentropy (const Mag & x, const Mag & y)
  {
    const double tx = x.value(),
                 ay = y.mag;
    return !mag :: isinf < MagT64 >(ay)                     ?
           -std :: abs(ay) * (sign0(ay) * tx - 1.) + lr(ay) :
           (sign(tx) != sign(ay))                           ?
           INF                                              :
           0.;
  }

  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagP64 > :: value > :: type * >
  double logZ (const Mag & u0, const Mag * u, const long int & nu)
  {
    double zkip = std :: log( (1. + u0.mag) * .5);
    double zkim = std :: log( (1. - u0.mag) * .5);

    for (long int i = 0L; i < nu; ++i)
    {
      zkip += std :: log( (1. + u[i].mag) * .5);
      zkim += std :: log( (1. - u[i].mag) * .5);
    }
    return std :: log( std :: exp(zkip) + std :: exp(zkim));
  }
  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * >
  double logZ (const Mag & u0, const Mag * u, const long int & nu)
  {
    const double a0   = u0.mag;
    const bool is_inf = mag :: isinf < MagT64 >(a0);
    double s1     = is_inf ? 0.       : a0;
    double s2     = is_inf ? 0.       : std :: abs(a0);
    double s3     = is_inf ? 0.       : lr(a0);
    double hasinf = is_inf ? sign(a0) : 0.;
    for (long int i = 0L; i < nu; ++i)
    {
      const double ai = u[i].mag;
      if (!mag :: isinf < MagT64 >(ai))
      {
        s1 += ai;
        s2 += std :: abs(ai);
        s3 += lr(ai);
      }
      else if (hasinf == 0)        hasinf = sign(ai);
      else if (hasinf != sign(ai)) return -INF;
    }
    return std :: abs(s1) - s2 + lr(s1) - s3;
  }

  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * >
  Mag auxmix (const Mag & H, const double & ap, const double & am)
  {
    const double aH = H.mag;
    double t1, t2;
    if (aH == 0.) return MagT64(0.);
    else
    {
      const double xH   = aH + ap,
                   xh   = aH + am,
                   a_ap = std :: abs(ap),
                   a_am = std :: abs(am);
      const bool inf_ap = mag :: isinf < MagT64 >(ap),
                 inf_am = mag :: isinf < MagT64 >(am);
      if (mag :: isinf < MagT64 >(aH))
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
        t2  = lr(xH) - lr(ap) - lr(xh) + lr(am);
      }
    }
    return MagT64((t1 + t2) * .5);
  }


  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagP64 > :: value > :: type * >
  Mag erfmix (const Mag & H, const double & mp, const double & mm)
  {
    return MagP64( H.mag * (std :: erf(mp) - std :: erf(mm)) / ( 2. + H.mag * (std :: erf(mp) + std :: erf(mm))));
  }
  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * >
  Mag erfmix (const Mag & H, const double & mp, const double & mm)
  {
    return auxmix(H, AtanhErf :: atanherf(mp), AtanhErf :: atanherf(mm));
  }

  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagP64 > :: value > :: type * >
  Mag exactmix (const Mag & H, const Mag & pp, const Mag & pm)
  {
    return MagP64( (pp.mag - pm.mag) * H.mag / (2. + (pp.mag + pm.mag) * H.mag) );
  }
  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * >
  Mag exactmix (const Mag & H, const Mag & pp, const Mag & pm)
  {
    return auxmix(H, pp.mag, pm.mag);
  }

}


#endif // __cplusplus

#endif // __mag_old_hpp__
