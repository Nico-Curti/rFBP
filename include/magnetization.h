#ifndef MAG_H
#define MAG_H
#include <type_traits>
#include <cmath>
#include <string>
#include <algorithm>

namespace mag
{
                      double   clamp         (const double &x, const double &low, const double &high);
                      double   lr            (const double &x);
                      long int sign0         (const double &x);
  template<class Mag> bool     signbit       (const Mag &m);
  template<class Mag> Mag      f2m           (const double &x);
  template<class Mag> void     zeros         (Mag *x, const long int &n);
  template<class Mag> void     zero          (Mag &x);
  template<class Mag> double   abs           (const Mag &a);
  template<class Mag> void     copysign      (Mag &x, const double &y);
  template<class Mag> Mag      arrow         (const Mag &m, const double &x);
  template<class Mag> long int sign0         (const Mag &x);
  template<class Mag> double   logmag2p      (const Mag &x);
  template<class Mag> Mag      convert       (const double &x);
  template<class Mag> double   convert       (const Mag &x);

  template<class Mag> Mag      couple        (const double &x1, const double &x2);
  template<class Mag> Mag      damp          (const Mag &newx, const Mag &oldx, const double &l);
  template<class Mag> Mag      mtanh         (const double &x);
  template<class Mag> Mag      matanh        (const Mag &m);
  template<class Mag> Mag      merf          (const double &x);
  template<class Mag> Mag      bar           (const Mag &m1, const Mag &m2);
  template<class Mag> double   log1pxy       (const Mag &x, const Mag &y);
  template<class Mag> double   mcrossentropy (const Mag &x, const Mag &y);
  template<class Mag> double   logZ          (const Mag &u0, const Mag *u, const long int &nu);
  template<class Mag> Mag      auxmix        (const Mag &H, const double &ap, const double &am);
  template<class Mag> Mag      erfmix        (const Mag &H, const double &mp, const double &mm);
  template<class Mag> Mag      exactmix      (const Mag &H, const Mag &pp, const Mag &pm);
}

#include <magP.h>
#include <magT.h>

#endif // MAG_H
