#ifndef __mag_old_h__
#define __mag_old_h__

#include <iostream>
#include <type_traits>
#include <cmath>
#include <string>
#include <algorithm>

namespace std
{
  double clamp (const double & x, const double & low, const double & high);
}

// forward declaration
struct MagP64;
struct MagT64;

namespace mag
{
                         double   clamp         (const double & x, const double & low, const double & high);
                         double   lr            (const double & x);
                         long int sign0         (const double & x);

  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagP64 > :: value > :: type * = nullptr >
  bool     isinf         (const double & x);
  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * = nullptr >
  bool     isinf         (const double & x);

  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagP64 > :: value > :: type * = nullptr >
  bool     signbit       (const Mag & m);
  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * = nullptr >
  bool     signbit       (const Mag & m);

  // template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagP64 > :: value > :: type * = nullptr >
  // Mag      f2m           (const double & x);
  // template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * = nullptr >
  // Mag      f2m           (const double & x);

  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagP64 > :: value > :: type * = nullptr >
  void     zeros         (Mag * x, const long int & n);
  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * = nullptr >
  void     zeros         (Mag * x, const long int & n);

  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagP64 > :: value > :: type * = nullptr >
  void     zero          (Mag & x);
  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * = nullptr >
  void     zero          (Mag & x);

  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagP64 > :: value > :: type * = nullptr >
  double   abs           (const Mag & a);
  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * = nullptr >
  double   abs           (const Mag & a);

  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagP64 > :: value > :: type * = nullptr >
  Mag      copysign      (Mag & x, const double & y);
  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * = nullptr >
  Mag      copysign      (Mag & x, const double & y);

  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagP64 > :: value > :: type * = nullptr >
  Mag      arrow         (const Mag & m, const double & x);
  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * = nullptr >
  Mag      arrow         (const Mag & m, const double & x);

  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagP64 > :: value > :: type * = nullptr >
  long int sign0         (const Mag & x);
  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * = nullptr >
  long int sign0         (const Mag & x);

  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagP64 > :: value > :: type * = nullptr >
  double   logmag2p      (const Mag & x);
  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * = nullptr >
  double   logmag2p      (const Mag & x);

  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagP64 > :: value > :: type * = nullptr >
  Mag      convert       (const double & x);
  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * = nullptr >
  Mag      convert       (const double & x);

  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagP64 > :: value > :: type * = nullptr >
  double   convert       (const Mag & x);
  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * = nullptr >
  double   convert       (const Mag & x);

  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagP64 > :: value > :: type * = nullptr >
  Mag      couple        (const double & x1, const double & x2);
  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * = nullptr >
  Mag      couple        (const double & x1, const double & x2);

  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagP64 > :: value > :: type * = nullptr >
  Mag      damp          (const Mag & newx, const Mag & oldx, const double & l);
  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * = nullptr >
  Mag      damp          (const Mag & newx, const Mag & oldx, const double & l);

  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagP64 > :: value > :: type * = nullptr >
  Mag      mtanh         (const double & x);
  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * = nullptr >
  Mag      mtanh         (const double & x);

  // template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * = nullptr >
  // Mag      matanh        (const Mag & m);

  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagP64 > :: value > :: type * = nullptr >
  Mag      merf          (const double & x);
  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * = nullptr >
  Mag      merf          (const double & x);

  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagP64 > :: value > :: type * = nullptr >
  Mag      bar           (const Mag & m1, const Mag & m2);
  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * = nullptr >
  Mag      bar           (const Mag & m1, const Mag & m2);

  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagP64 > :: value > :: type * = nullptr >
  double   log1pxy       (const Mag & x,  const Mag & y);
  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * = nullptr >
  double   log1pxy       (const Mag & x,  const Mag & y);

  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagP64 > :: value > :: type * = nullptr >
  double   mcrossentropy (const Mag & x,  const Mag & y);
  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * = nullptr >
  double   mcrossentropy (const Mag & x,  const Mag & y);

  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagP64 > :: value > :: type * = nullptr >
  double   logZ          (const Mag & u0, const Mag * u,     const long int & nu);
  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * = nullptr >
  double   logZ          (const Mag & u0, const Mag * u,     const long int & nu);

  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * = nullptr >
  Mag      auxmix        (const Mag & H,  const double & ap, const double & am);

  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagP64 > :: value > :: type * = nullptr >
  Mag      erfmix        (const Mag & H,  const double & mp, const double & mm);
  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * = nullptr >
  Mag      erfmix        (const Mag & H,  const double & mp, const double & mm);

  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagP64 > :: value > :: type * = nullptr >
  Mag      exactmix      (const Mag & H,  const Mag & pp,    const Mag & pm);
  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * = nullptr >
  Mag      exactmix      (const Mag & H,  const Mag & pp,    const Mag & pm);
}

#include <magP.h>
#include <magT.h>

#endif // __mag_old_h__
