#if !defined __clang__ && __GNUC__ <= 6

#include <magnetization_oldgcc.hpp>

#else

#include <magnetization.hpp>

#endif


namespace mag
{

  template bool signbit < MagP64 >(const MagP64 & m);
  template bool signbit < MagT64 >(const MagT64 & m);

  // template MagP64 f2m < MagP64 >(const double & x);
  // template MagT64 f2m < MagT64 >(const double & x);

  template void zeros < MagP64 >(MagP64 * x, const long int & n);
  template void zeros < MagT64 >(MagT64 * x, const long int & n);

  template double abs < MagP64 >(const MagP64 & a);
  template double abs < MagT64 >(const MagT64 & a);

  template MagP64 copysign < MagP64 >(MagP64 & x, const double & y);
  template MagT64 copysign < MagT64 >(MagT64 & x, const double & y);

  template MagP64 arrow < MagP64 >(const MagP64 & m, const double & x);
  template MagT64 arrow < MagT64 >(const MagT64 & m, const double & x);

  template MagP64 convert < MagP64 >(const double & x);
  template MagT64 convert < MagT64 >(const double & x);

  template MagP64 couple < MagP64 >(const double & x1, const double & x2);
  template MagT64 couple < MagT64 >(const double & x1, const double & x2);

  template MagP64 damp < MagP64 >(const MagP64 & newx, const MagP64 & oldx, const double & l);
  template MagT64 damp < MagT64 >(const MagT64 & newx, const MagT64 & oldx, const double & l);

  template MagP64 mtanh < MagP64 >(const double & x);
  template MagT64 mtanh < MagT64 >(const double & x);

  template MagP64 merf < MagP64 >(const double & x);
  template MagT64 merf < MagT64 >(const double & x);

  template MagP64 bar < MagP64 >(const MagP64 & m1, const MagP64 & m2);
  template MagT64 bar < MagT64 >(const MagT64 & m1, const MagT64 & m2);

  template double log1pxy < MagP64 >(const MagP64 & x, const MagP64 & y);
  template double log1pxy < MagT64 >(const MagT64 & x, const MagT64 & y);

  template double mcrossentropy < MagP64 >(const MagP64 & x, const MagP64 & y);
  template double mcrossentropy < MagT64 >(const MagT64 & x, const MagT64 & y);

  template double logZ < MagP64 >(const MagP64 & u0, const MagP64 * u, const long int & nu);
  template double logZ < MagT64 >(const MagT64 & u0, const MagT64 * u, const long int & nu);

  template MagP64 erfmix < MagP64 >(const MagP64 & H, const double & mp, const double & mm);
  template MagT64 erfmix < MagT64 >(const MagT64 & H, const double & mp, const double & mm);

  template MagP64 exactmix < MagP64 >(const MagP64 & H, const MagP64 & pp, const MagP64 & pm);
  template MagT64 exactmix < MagT64 >(const MagT64 & H, const MagT64 & pp, const MagT64 & pm);

}


std :: ostream & operator <<(std :: ostream & os, const MagP64 & m)
{
  os << m.mag;
  return os;
}

std :: ostream & operator <<(std :: ostream & os, const MagT64 & m)
{
  os << m.value;
  return os;
}
