#ifndef MAGT_H
#define MAGT_H

#include <cmath>
#include <numeric>
#include <string>
#include <magnetization.h>
#include <atanherf.h>

struct MagT64
{
  // member
  double mag,
         mInf;

  // constructor and destructor
  MagT64()                                                               {};
  MagT64(const double &x, double m = 0.) : mag(std::tanh(x)), mInf(m)    {};
  ~MagT64()                                                              = default;
  MagT64(const MagT64 &m)                                                { this->mag = m.mag; this->mInf = m.mInf;}
  MagT64& operator=(const MagT64 &m)                                     { this->mag = m.mag; this->mInf = m.mInf; return *this; }

  // utilities
  std::string magformat()                                                { return "tanh"; }

  // math operators
  MagT64 operator%(const MagT64 &m)                                      { double tmp = std::atanh(this->mag) + std::atanh(m.mag);
                                                                           return std::isnan(tmp) ?
                                                                                  MagT64(inf)     :
                                                                                  MagT64(tmp)     ; // temporary solution of nan problem (see atanherf)
                                                                         }

  MagT64 operator+(const MagT64 &m)                                      { return MagT64(this->mag + m.mag); }
  double operator+(const double &x)                                      { return x + this->mag; }
  friend MagT64 operator+(const double &x, const MagT64 &m)              { return MagT64(x + m.mag); }
  MagT64& operator+=(const MagT64 &m)                                    { this->mag += m.mag; return *this; }

  MagT64 operator/(const MagT64 &m)                                      { return MagT64(this->mag / m.mag); }
  MagT64 operator/(const double &x)                                      { return MagT64(this->mag / x); }
  friend MagT64 operator/(const double &x, const MagT64 &m)              { return MagT64(x / m.mag); }

  MagT64 operator*(const MagT64 &m)                                      { double ax = std::atanh(this->mag),
                                                                                  ay = std::atanh(m.mag),
                                                                                  t1 = (ax >= ay && ax >= -ay) ?
                                                                                       2. * ay                 :
                                                                                       (ax >= ay && ax < -ay)  ?
                                                                                      -2. * ax                 :
                                                                                       (ax < ay && ax >= -ay)   ?
                                                                                       2. * ax                  :
                                                                                      -2. * ay,
                                                                                  t2 = (std::isinf(ax) || std::isinf(ay)) ?
                                                                                       0.                                 :
                                                                                       mag::lr(ax + ay) - mag::lr(ax - ay);
                                                                           return MagT64((t1 + t2) * .5);
                                                                         }
  double operator*(const double &x)                                      { return this->mag * x; }
  friend double operator*(const double &x, const MagT64 &m)              { return m.mag * std::atanh(x); }

  MagT64 operator-(const MagT64 &m)                                      { return MagT64(this->mag - m.mag ); }
  MagT64 operator-(const double &x)                                      { return MagT64(this->mag - x); }
  friend MagT64 operator-(const double &x, const MagT64 &m)              { return MagT64(x - m.mag); }

  // logical operators
  bool operator==(const MagT64 &m)                                       { return this->mag == m.mag; }
  bool operator!=(const MagT64 &m)                                       { return this->mag != m.mag; }
};

std::ostream& operator<<(std::ostream& os, const MagT64 &m)
{
  os << m.mag;
  return os;
}

#endif // MAGT_H
