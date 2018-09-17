#ifndef MAGP_H
#define MAGP_H

#include <string>
#include <magnetization.h>

struct MagP64
{
  // member
  double mag;

  // constructor and destructor
  MagP64()                                                    {};
  MagP64(const double &x) : mag(x)                            {};
  ~MagP64()                                                   = default;
  MagP64(const MagP64 &m)                                     { this->mag = m.mag; }
  MagP64& operator=(const MagP64 &m)                          { this->mag = m.mag; return *this; }

  // utilities
  std::string magformat()                                     { return "plain"; }

  // math operators
  MagP64 operator%(const MagP64 &m)                           { return MagP64( mag::clamp( (this->mag + m.mag) / (1. + this->mag * m.mag), -1., 1.)); }

  MagP64 operator+(const MagP64 &m)                           { return MagP64(this->mag + m.mag); }
  double operator+(const double &x)                           { return x + this->mag; }
  friend MagP64 operator+(const double &x, const MagP64 &m)   { return MagP64(x + m.mag); }
  MagP64& operator+=(const MagP64 &m)                         { this->mag += m.mag; return *this; }

  MagP64 operator/(const MagP64 &m)                           { return MagP64(this->mag / m.mag); }
  MagP64 operator/(const double &x)                           { return MagP64(this->mag / x); }
  friend MagP64 operator/(const double &x, const MagP64 &m)   { return MagP64(x / m.mag); }

  MagP64 operator*(const MagP64 &m)                           { return MagP64(this->mag * m.mag); }
  double operator*(const double &x)                           { return this->mag * x; }
  friend double operator*(const double &x, const MagP64 &m)   { return m.mag * x; }

  MagP64 operator-(const MagP64 &m)                           { return MagP64(this->mag - m.mag ); }
  MagP64 operator-(const double &x)                           { return MagP64(this->mag - x); }
  friend MagP64 operator-(const double &x, const MagP64 &m)   { return MagP64(x - m.mag); }

  // logical operators
  bool operator==(const MagP64 &m)                            { return this->mag == m.mag; }
  bool operator!=(const MagP64 &m)                            { return this->mag != m.mag; }
};

std::ostream& operator<<(std::ostream& os, const MagP64 &m)
{
  os << m.mag;
  return os;
}

#endif // MAGP_H
