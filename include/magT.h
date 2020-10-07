#ifndef __magt_h__
#define __magt_h__

#include <cmath>
#include <string>
#include <atanherf.h>

/**
* @class MagT64
* @brief Abstract type representing magnetization type chosen for cavity messages.
*
* @note MagT64 means a double type wit `tanh` application.
*
*/
struct MagT64
{
  // member
  double mag;   ///< Magnetization
  double mInf;  ///< Boundary dimension

  // constructor and destructor

  /**
  * @brief Default constructor.
  *
  */
  MagT64 ()                                                                           {};

  /**
  * @brief Constructor with value.
  *
  * @param x magnetization
  * @param m boundary value
  *
  * @note In MagT64 the magnetization is converted to a `value` given by `tanh(x)`.
  */
  MagT64 (const double & x, double m=30.0) : mag (x), mInf (m)                        {};

  /**
  * @brief Default destructor.
  *
  */
  ~MagT64 ()                                                             = default;

  /**
  * @brief Copy constructor.
  *
  * @param m MagT64 object
  *
  */
  MagT64 (const MagT64 & m)                                              { this->mag = m.mag; this->mInf = m.mInf; }

  /**
  * @brief Assigment operator constructor.
  *
  * @param m MagT64 object
  *
  */
  MagT64 & operator = (const MagT64 & m)                                 { this->mag = m.mag; this->mInf = m.mInf; return *this; }

  // utilities

  /**
  * @brief Return the mag description (tanh for MagT64).
  *
  */
  std :: string magformat () const                                       { return "tanh"; }

  /**
  * @brief Get the magnetization value.
  *
  * @note In MagT64 the value is given by `tanh(mag)`.
  *
  */
  double value () const                                                  { return std :: tanh(this->mag); }

  // math operators

  /**
  * @brief Overload operator.
  *
  * @details Add magnetization (m1 + m2)
  *
  * @note The summation exclude the `tanh` evaluation.
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
  * @brief Overload operator.
  *
  * @details Just a simple addition of the mag values.
  *
  * @param m MagT64 object
  *
  */
  MagT64 operator + (const MagT64 & m)                                   { return MagT64(this->mag + m.mag); }

  /**
  * @brief Overload operator.
  *
  * @details Just a simple division as (mag / x)
  *
  * @param x double value
  *
  */
  MagT64 operator / (const double & x)                                   { return MagT64(this->mag / x); }

  /**
  * @brief Overload operator.
  *
  * @details Just a simple product as (x * mag)
  *
  * @param x double value
  * @param m MagT64 object
  *
  */
  friend double operator * (const double & x, const MagT64 & m)          { return m.mag * x; }

  /**
  * @brief Overload operator
  *
  * @details Combine two mags.
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
                                                                           const double t2 = (mag :: isinf < MagT64 >(ax) || mag :: isinf < MagT64 >(ay)) ?
                                                                                             0.                                                           :
                                                                                             std :: log((std :: exp(2 * std :: abs(ax + ay)) + 1) / (std :: exp(2 * std :: abs(ax - ay)) + 1)) + 2 * std :: abs(ax - ay) - 2 * std :: abs(ax + ay);
                                                                           return MagT64((t1 + t2) * .5);
                                                                         }

  /**
  * @brief Overload operator.
  *
  * @details Subtract values (val1 - val2)
  *
  * @param m MagT64 object
  *
  */
  double operator - (const MagT64 & m)                                  { return this->value() - m.value(); }

  /**
  * @brief Get a magnetization with a flipped sign.
  *
  * @details flip magnetization sign
  *
  */
  MagT64 operator - () const                                             { return MagT64(-this->mag); }

  // logical operators

  /**
  * @brief Check magnetization equality.
  *
  * @details Compare magnetizations
  *
  * @param m MagT64 object
  *
  */
  bool operator == (const MagT64 & m)                                    { return this->mag == m.mag; }

  /**
  * @brief Check magnetization not equality.
  *
  * @details compare magnetizations
  *
  * @param m MagT64 object
  *
  */
  bool operator != (const MagT64 & m)                                    { return this->mag != m.mag; }

  /**
  * @brief Print operator with stdout/stderr.
  *
  * @details print mag
  *
  * @param os ostream operator
  * @param m MagT64 object
  *
  */
  friend std :: ostream & operator << (std :: ostream & os, const MagT64 & m);
};

#endif // __magt_h__
