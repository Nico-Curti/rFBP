#ifndef MAGT_H
#define MAGT_H

#include <cmath>
#include <atanherf.h>

struct MagT64
{
  // member
  double mag,
         value,
         mInf;

  // constructor and destructor
  MagT64()                                                                           {};
  MagT64(const double &x, double m = 30.) : mag(x), value(std::tanh(x)), mInf(m)   {};
  ~MagT64()                                                              = default;
  MagT64(const MagT64 &m)                                                { this->mag = m.mag; this->value = m.value; this->mInf = m.mInf; }
  MagT64& operator=(const MagT64 &m)                                     { this->mag = m.mag; this->value = m.value; this->mInf = m.mInf; return *this; }

  // utilities
  std::string magformat()                                                { return "tanh"; }

  // math operators
  MagT64 operator%(const MagT64 &m)                                      { return MagT64(this->mag + m.mag);
                                                                           //  double tmp = this->mag + m.mag;
                                                                           // return std::isnan(tmp) ?
                                                                           //        MagT64(inf)     :
                                                                           //        MagT64(tmp)     ; // temporary solution of nan problem (see atanherf)
                                                                         }

  MagT64 operator+(const MagT64 &m)                                      { return MagT64(this->mag + m.mag); }
  // double operator+(const double &x)                                      { return x + this->mag; }
  // friend MagT64 operator+(const double &x, const MagT64 &m)              { return MagT64(x + m.mag); }
  MagT64& operator+=(const MagT64 &m)                                    { this->mag += m.mag; this->value = std::tanh( this->mag + m.value); return *this; }
  // friend MagT64 operator+=(const double &x, const MagT64 &m)             { return x + m.mag; }

  // MagT64 operator/(const MagT64 &m)                                      { return MagT64(this->mag / m.mag); }
  MagT64 operator/(const double &x)                                      { return MagT64(this->mag / x); }
  // friend MagT64 operator/(const double &x, const MagT64 &m)              { return MagT64(x / m.mag); }

  // double operator*(const MagT64 &m)                                      { return this->mag * m.mag;}
  // double operator*(const double &x)                                      { return this->mag * x; }
  friend double operator*(const double &x, const MagT64 &m)              { return m.mag * x; }
  MagT64 operator^(const MagT64 &m)                                      { double ax = this->mag,
                                                                                  ay = m.mag,
                                                                                  t1 = (ax >= ay && ax >= -ay) ?
                                                                                       2. * ay                 :
                                                                                       (ax >= ay && ax < -ay)  ?
                                                                                      -2. * ax                 :
                                                                                       (ax < ay && ax >= -ay)   ?
                                                                                       2. * ax                  :
                                                                                      -2. * ay,
                                                                                  t2 = (mag::isinf(ax) || mag::isinf(ay)) ?
                                                                                       0.                                 :
                                                                                       mag::lr(ax + ay) - mag::lr(ax - ay);
                                                                           return MagT64((t1 + t2) * .5);
                                                                         }

  double operator-(const MagT64 &m)                                      { return this->value - m.value; }
  // MagT64 operator-(const double &x)                                      { return MagT64(this->mag - x); }
  // friend MagT64 operator-(const double &x, const MagT64 &m)              { return MagT64(x - m.mag); }
  MagT64 operator-() const                                               { return MagT64(-this->mag); }

  // logical operators
  bool operator==(const MagT64 &m)                                       { return this->mag == m.mag; }
  bool operator!=(const MagT64 &m)                                       { return this->mag != m.mag; }

  friend std::ostream& operator<<(std::ostream& os, const MagT64 &m);
};

#endif // MAGT_H
