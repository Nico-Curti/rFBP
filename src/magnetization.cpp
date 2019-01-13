#include <magnetization.h>

#include <magP.h>
#include <magT.h>

namespace mag
{
                      double   clamp         (const double &x, const double &low, const double &high)  {return (low <= x && x <= high) ? x : (x < low) ? low : high; }
                      double   lr            (const double &x)                                         { return std::log1p(std::exp(-2. * std::abs(x))); }
                      long int sign0         (const double &x)                                         { return 1L - 2L * static_cast<long int>(std::signbit(x));}

  template<class Mag> bool     signbit       (const Mag &m)
  {
    static_assert( (std::is_same_v<Mag, MagP64> ||
                   (std::is_same_v<Mag, MagT64>)),
                   "signbit function! Incompatible types found.");
    return std::signbit(m.mag);
  }

  template<class Mag> Mag      f2m           (const double &x)
  {
    static_assert( (std::is_same_v<Mag, MagP64> ||
                   (std::is_same_v<Mag, MagT64>)),
                   "f2m function! Incompatible types found.");
    return Mag(x);
  }

  template<class Mag> void     zeros         (Mag *x, const long int &n)
  {
    static_assert( (std::is_same_v<Mag, MagP64> ||
                   (std::is_same_v<Mag, MagT64>)),
                   "zeros function! Incompatible types found.");
    std::fill_n(x, n, 0.f);
  }

  template<class Mag> void     zero          (Mag &x)
  {
    static_assert( (std::is_same_v<Mag, MagP64> ||
                   (std::is_same_v<Mag, MagT64>)),
                   "zero function! Incompatible types found.");
    x.mag = 0.;
  }

  template<class Mag> double   abs           (const Mag &a)
  {
    static_assert( (std::is_same_v<Mag, MagP64> ||
                   (std::is_same_v<Mag, MagT64>)),
                   "abs function! Incompatible types found.");
    return std::abs(a.mag);
  }

  template<class Mag> void     copysign      (Mag &x, const double &y)
  {
    static_assert( (std::is_same_v<Mag, MagP64> ||
                   (std::is_same_v<Mag, MagT64>)),
                   "copysign function! Incompatible types found.");
    signbit(x) != signbit(y) ? -x : x;
  }

  template<class Mag> Mag      arrow         (const Mag &m, const double &x)
  {
    static_assert( (std::is_same_v<Mag, MagP64> ||
                   (std::is_same_v<Mag, MagT64>)),
                   "arrow function! Incompatible types found.");
    return mtanh(x * std::atanh(m.mag));
  }

  template<class Mag> long int sign0         (const Mag &x)
  {
    static_assert( (std::is_same_v<Mag, MagP64> ||
                   (std::is_same_v<Mag, MagT64>)),
                   "sign0 function! Incompatible types found.");
    return 1L - 2L * static_cast<long int>(signbit(x));
  }

  template<class Mag> double   logmag2p      (const Mag &x)
  {
    static_assert( (std::is_same_v<Mag, MagP64> ||
                   (std::is_same_v<Mag, MagT64>)),
                   "logmag2p function! Incompatible types found.");
    return std::log( (1. + x.mag) * .5 );
  }

  template<class Mag> Mag      convert       (const double &x)
  {
    static_assert( (std::is_same_v<Mag, MagP64> ||
                   (std::is_same_v<Mag, MagT64>)),
                   "convert function! Incompatible types found.");
    return Mag(x);
  }

  template<class Mag> Mag      convert       (const Mag &x)
  {
    static_assert( (std::is_same_v<Mag, MagP64> ||
                   (std::is_same_v<Mag, MagT64>)),
                   "convert function! Incompatible types found.");
    return Mag(clamp(std::atanh(x), -30., 30.));
  }


  template<class Mag> Mag      couple        (const double &x1, const double &x2)
  {
    static_assert( (std::is_same_v<Mag, MagP64> ||
                   (std::is_same_v<Mag, MagT64>)),
                   "couple function! Incompatible types found.");

    if constexpr      ( std::is_same_v<Mag, MagP64> )
      MagP64( (x1 - x2) / (x1 + x2) );
    else
      MagT64( (std::log(x1) - std::log(x2)) * .5 );
  }


  template<class Mag> Mag      damp          (const Mag &newx, const Mag &oldx, const double &l)
  {
    static_assert( (std::is_same_v<Mag, MagP64> ||
                   (std::is_same_v<Mag, MagT64>)),
                   "damp function! Incompatible types found.");

    if constexpr      ( std::is_same_v<Mag, MagP64> )
      return MagP64(newx.mag * (1. - l) + oldx.mag * l);
    else
      return MagT64(std::atanh(newx.mag) * (1. - l) + std::atanh(oldx.mag) * l);
  }

  template<class Mag> Mag      mtanh         (const double &x)
  {
    static_assert( (std::is_same_v<Mag, MagP64> ||
                   (std::is_same_v<Mag, MagT64>)),
                   "mtanh function! Incompatible types found.");

    if constexpr      ( std::is_same_v<Mag, MagP64> )
      return MagP64(std::tanh(x));
    else
      return MagT64(x);
  }

  template<class Mag> Mag      matanh        (const Mag &m)
  {
    static_assert( std::is_same_v<Mag, MagT64>, "matanh function! Incompatible type found. You must use MagT64 variable for this function." );
    return MagT64(m.mag);
  }

  template<class Mag> Mag      merf          (const double &x)
  {
    static_assert( (std::is_same_v<Mag, MagP64> ||
                   (std::is_same_v<Mag, MagT64>)),
                   "mtanh function! Incompatible types found.");

    if constexpr      ( std::is_same_v<Mag, MagP64> )
      return MagP64(std::erf(x));
    else
      return MagT64(AtanhErf::atanherf(x));
  }

  template<class Mag> Mag      bar           (const Mag &m1, const Mag &m2)
  {
    static_assert( std::is_same_v<Mag, MagP64> ||
                   std::is_same_v<Mag, MagT64>,
                   "bar function! Incompatible types found.");

    if constexpr      ( std::is_same_v<Mag, MagP64> )
      return ((m1.mag == m2.mag) ?
              MagP64(0.)           :
              MagP64(clamp( (m1.mag - m2.mag) / (1. - m1.mag * m2.mag), -1., 1.)));
    else
      return ((m1.mag == m2.mag) ?
              MagT64(0.)           :
              MagT64(std::atanh(m1.mag) -  std::atanh(m2.mag)));
  }

  template<class Mag> double   log1pxy       (const Mag &x, const Mag &y)
  {
    static_assert( std::is_same_v<Mag, MagP64> ||
                   std::is_same_v<Mag, MagT64>,
                   "log1pxy function! Incompatible types found.");

    if constexpr      ( std::is_same_v<Mag, MagP64> )
      return std::log( ( 1. + (x.mag * y.mag)) * .5);
    else
    {
      double ax = std::atanh(x.mag),
             ay = std::atanh(y.mag);

      return !std::isinf(ax) && !std::isinf(ay) ?
              std::abs(ax + ay) - std::abs(ax) - std::abs(ay) + lr(ax + ay) - lr( ax) - lr( ay) :
              std::isinf(ax) && !std::isinf(ay) ? sign(ax) * ay - std::abs(ay) - lr(ay)         :
             !std::isinf(ax) &&  std::isinf(ay) ? sign(ay) * ax - std::abs(ax) - lr(ax)         :
              sign(ax) == sign(ay) ? 0. : -inf;
    }
  }

  template<class Mag> double   mcrossentropy (const Mag &x, const Mag &y)
  {
    static_assert( std::is_same_v<Mag, MagP64> ||
                   std::is_same_v<Mag, MagT64>,
                   "log1pxy function! Incompatible types found.");

    if constexpr      ( std::is_same_v<Mag, MagP64> )
      return x.mag * std::atanh(y.mag) - std::log(1. - y.mag * y.mag) * .5 + log_2;
    else
    {
      double tx = x.mag,
             ay = std::atanh(y.mag);
      return !std::isinf(ay)                                ?
             -std::abs(ay) * (sign0(ay) * tx - 1.) + lr(ay) :
             (sign(tx) != sign(ay))                         ?
             inf                                            :
             0.;
    }
  }


  template<class Mag> double   logZ          (const Mag &u0, const Mag *u, const long int &nu)
  {
    static_assert( std::is_same_v<Mag, MagP64> ||
                   std::is_same_v<Mag, MagT64>,
                   "log1pxy function! Incompatible types found.");

    if constexpr      ( std::is_same_v<Mag, MagP64> )
    {
      double zkip = std::log( (1. + u0.mag) * .5),
             zkim = std::log( (1. - u0.mag) * .5);
#ifdef _OPENMP
#pragma omp for reduction(+ : zkip, zkim)
#endif
      for (int i = 0; i < nu; ++i)
      {
        zkip += std::log( (1. - u[i].mag) * .5);
        zkim += std::log( (1. + u[i].mag) * .5);
      }
      return std::log( std::exp(zkip) + std::exp(zkim));
    }
    else
    {
      double a0 = std::atanh(u0.mag),
             s1, s2, s3,
             ai, hasinf;
      bool is_inf = std::isinf(a0);
      s1          = is_inf ? 0. : a0;
      s2          = is_inf ? 0. : std::abs(a0);
      s3          = is_inf ? 0. : lr(a0);
      hasinf      = is_inf ? 0. : 0.;

      for (int i = 0; i < nu; ++i)
      {
        ai = std::atanh(u[i].mag);
        if (!std::isinf(ai))
        {
          s1 += ai;
          s2 += std::abs(ai);
          s3 += lr(ai);
        }
        else if (hasinf == 0)        hasinf = sign(ai);
        else if (hasinf != sign(ai)) return -inf;
      }
      return std::abs(s1) - s2 + lr(s1) - s3;
    }
  }

  template<class Mag> Mag      auxmix        (const Mag &H, const double &ap, const double &am)
  {
    static_assert( std::is_same_v<Mag, MagT64>, "auxmix function! Incompatible type found. You must use MagT64 variable for this function." );

    double aH = std::atanh(H.mag),
           t1, t2;
    if (aH == 0.) return MagT64(0.);
    else
    {
      double xH   = aH + ap,
             xh   = aH + am,
             a_ap = std::abs(ap),
             a_am = std::abs(am);
      bool inf_ap = std::isinf(ap),
           inf_am = std::isinf(am);
      if (std::isinf(H.mag))
      {
        if (!inf_ap && !inf_am)
        {
          t1 = sign(aH) * (ap - am) - a_ap + a_am;
          t2 = -lr(ap) + lr(am);
        }
        else if (inf_ap && !inf_am)
        {
          t1 = ( sign(ap) == sign(am) ) ? -sign(aH) * am + a_am : -2. * H.mInf;
          t2 = ( sign(ap) == sign(am) ) ? lr(am)                : 0.;
        }
        else if (!inf_ap && inf_am)
        {
          t1 = ( sign(am) == sign(aH) ) ? -sign(aH) * ap - a_ap : 2. * H.mInf;
          t2 = ( sign(am) == sign(aH) ) ? -lr(ap)               : 0.;
        }
        else
        {
          t2 = 0.;
          if ( (sign(ap) == sign(aH) && sign(ap) == sign(aH)) || ((sign(ap) != sign(aH) && sign(ap) != sign(aH))) )
            t1 = 0.;
          else if (sign(ap) == sign(aH))
            t1 = 2. * H.mInf;
          else
            t1 = -2. * H.mInf;
        }
    }
    else
    {
      t1  = 0.;
      t1 += inf_ap ? 0. : std::abs(xH) - a_ap;
      t1 -= inf_am ? 0. : std::abs(xh) - a_am;
      t2  = lr(xH) - lr(ap) - lr(xh) + lr(am);
    }
  }
  return MagT64((t1 + t2) * .5);

  }

  template<class Mag> Mag      erfmix        (const Mag &H, const double &mp, const double &mm)
  {
    if constexpr      ( std::is_same_v<Mag, MagP64> )
      return MagP64( H.mag * (std::erf(mp) - std::erf(mm)) / ( 2. + H.mag * (std::erf(mp) + std::erf(mm))));
    else
      return auxmix(H, AtanhErf::atanherf(mp), AtanhErf::atanherf(mm));
  }

  template<class Mag> Mag      exactmix      (const Mag &H, const Mag &pp, const Mag &pm)
  {
    static_assert( std::is_same_v<Mag, MagP64> ||
                   std::is_same_v<Mag, MagT64>,
                   "exactmix function! Incompatible types found.");

    if constexpr      ( std::is_same_v<Mag, MagP64> )
      return MagP64( (pp.mag - pm.mag) * H.mag / (2. + (pp.mag + pm.mag) * H.mag) );
    else
      return auxmix(H, pp.mag, pm.mag);
  }
}
