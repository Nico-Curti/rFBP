#ifndef MAG_H
#define MAG_H
#include <type_traits>
#include <cmath>
#include <limits>

static constexpr double log_2 = 0.30102999566;
static constexpr double inf   = std::numeric_limits<double>::infinity();

namespace Magnetization
{
                      auto     sign          (const double &x);
  template<class Mag> bool     signbit       (const Mag &m);
  template<class Mag> auto     f2m           (const double &x);
  template<class Mag> void     zeros         (Mag *x, const long int &n);
  template<class Mag> void     zero          (Mag &x);
  template<class Mag> double   abs           (const Mag &a);
                      double   clamp         (const double &x, const double &low, const double &high);
  template<class Mag> void     copysign      (Mag &x, const double &y);
                      double   lr            (const double &x);
  template<class Mag> auto     arrow         (const Mag &m, const double &x);
                      long int sign0         (const double &x);
  template<class Mag> long int sign0         (const Mag &x);
  template<class Mag> double   logmag2p      (const Mag &x);
  template<class Mag> auto     convert       (const double &x);
  template<class Mag> auto     convert       (const Mag &x);

  template<class Mag> auto     couple        (const double &x1, const double &x2);
  template<class Mag> auto     damp          (const Mag &newx, const Mag &oldx, const double &l);
  template<class Mag> auto     mtanh         (const double &x);
  template<class Mag> auto     matanh        (const Mag &m);
  template<class Mag> auto     merf          (const double &x);
  template<class Mag> auto     bar           (const Mag &m1, const Mag &m2);
  template<class Mag> double   log1pxy       (const Mag &x, const Mag &y);
  template<class Mag> double   mcrossentropy (const Mag &x, const Mag &y);
  template<class Mag> double   logZ          (const Mag &u0, const Mag *u, const long int &nu);
  template<class Mag> auto     auxmix        (const Mag &H, const double &ap, const double &am);
  template<class Mag> auto     erfmix        (const Mag &H, const double &mp, const double &mm);
  template<class Mag> auto     exactmix      (const Mag &H, const Mag &pp, const Mag &pm);
}

#include <magP.h>
#include <magT.h>

#endif // MAG_H
