#ifndef __magt_h__
#define __magt_h__

#include <cmath>
#include <string>
#include <atanherf.h>

/**
* @class MagT64
* @brief Abstract type representing magnetization type chosen for cavity messages
*
* @details MagT64 means a double type wit tanh application
*
*/
struct MagT64
{
  // member
  double mag;   ///< magnetization
  double mInf;  ///< boundary

  // constructor and destructor

  /**
  * @brief default constructor
  *
  */
  MagT64 ()                                                                           {};

  /**
  * @brief constructor with value
  *
  * @param x magnetization (thus value is tanh(x))
  * @param m boundary value
  */
  MagT64 (const double & x, double m=30.) : mag (x), mInf (m)                         {};

  /**
  * @brief default destructor
  *
  */
  ~MagT64 ()                                                             = default;

  /**
  * @brief copy constructor
  *
  * @param m MagT64 object
  *
  */
  MagT64 (const MagT64 & m)                                              { this->mag = m.mag; this->mInf = m.mInf; }

  /**
  * @brief assigment operator constructor
  *
  * @param m MagT64 object
  *
  */
  MagT64 & operator = (const MagT64 & m)                                 { this->mag = m.mag; this->mInf = m.mInf; return *this; }

  // utilities

  /**
  * @brief string format utility (=> "tanh")
  *
  */
  std :: string magformat () const                                       { return "tanh"; }

  /**
  * @brief magnetization value (equal to mag in MagP64)
  *
  */
  double value () const                                                  { return std :: tanh(this->mag); }

  // math operators

  /**
  * @brief % operator
  *
  * @detail add magnetization (m1 + m2) <- tanh is already applied!
  *
  * @param m MagT64 object
  *
  */
  MagT64 operator % (const MagT64 & m)                                   { return MagT64(this->mag + m.mag);
                                                                           //  double tmp = this->mag + m.mag;
                                                                           // return std :: isnan(tmp) ?
                                                                           //        MagT64(inf)     :
                                                                           //        MagT64(tmp)     ; // temporary solution of nan problem (see atanherf)
                                                                         }

  /**
  * @brief + operator
  *
  * @detail add magnetization
  *
  * @param m MagT64 object
  *
  */
  MagT64 operator + (const MagT64 & m)                                   { return MagT64(this->mag + m.mag); }

  /**
  * @brief += operator
  *
  * @detail add magnetization AND value (as tanh(m1 + m2))
  *
  * @param m MagT64 object
  *
  */
  MagT64& operator += (const MagT64 & m)                                 { this->mag += m.mag; return *this; }

  /**
  * @brief / operator
  *
  * @detail divide magnetization (mag / x)
  *
  * @param x double value
  *
  */
  MagT64 operator / (const double & x)                                   { return MagT64(this->mag / x); }

  /**
  * @brief * operator
  *
  * @detail prod magnetization (x * mag)
  *
  * @param x double value
  * @param m MagT64 object
  *
  */
  friend double operator * (const double & x, const MagT64 & m)          { return m.mag * x; }

  /**
  * @brief ^ operator
  *
  * @detail combination of mags
  *
  * @param m MagT64 object
  *
  */
  MagT64 operator ^ (const MagT64 & m)                                   { const double ax = this->mag;
                                                                           const double ay = m.mag;
                                                                           const double t1 = (ax >= ay && ax >= -ay) ?
                                                                                             2. * ay                 :
                                                                                             (ax >= ay && ax < -ay)  ?
                                                                                            -2. * ax                 :
                                                                                             (ax < ay && ax >= -ay)  ?
                                                                                             2. * ax                 :
                                                                                            -2. * ay;
                                                                           const double t2 = (mag :: isinf(ax) || mag :: isinf(ay)) ?
                                                                                             0.                                     :
                                                                                             mag :: lr(ax + ay) - mag :: lr(ax - ay);
                                                                           return MagT64((t1 + t2) * .5);
                                                                         }

  /**
  * @brief - operator
  *
  * @detail sub magnetization (value - value)
  *
  * @param m MagT64 object
  *
  */
  double operator - (const MagT64 & m)                                  { return this->value() - m.value(); }

  /**
  * @brief - operator
  *
  * @detail flip magnetization sign
  *
  */
  MagT64 operator - () const                                             { return MagT64(-this->mag); }

  // logical operators

  /**
  * @brief == operator
  *
  * @detail compare magnetizations
  *
  * @param m MagT64 object
  *
  */
  bool operator == (const MagT64 & m)                                    { return this->mag == m.mag; }

  /**
  * @brief != operator
  *
  * @detail compare magnetizations
  *
  * @param m MagT64 object
  *
  */
  bool operator != (const MagT64 & m)                                    { return this->mag != m.mag; }

  /**
  * @brief << operator
  *
  * @detail print mag
  *
  * @param os ostream operator
  * @param m MagT64 object
  *
  */
  friend std :: ostream & operator << (std :: ostream & os, const MagT64 & m);
};

#endif // __magt_h__
