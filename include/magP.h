#ifndef __magp_h__
#define __magp_h__

#include <string>

/**
* @class MagP64
* @brief Abstract type representing magnetization type chosen for cavity messages.
*
* @note The `MagP64` type allows fast executions with inexact outcomes by neglecting all `tanh` operations.
*
*/
struct MagP64
{
  // member
  double mag;   ///< Magnetization

  // constructor and destructor
  /**
  * @brief Default constructor.
  *
  */
  MagP64 ()                                                   {};

  /**
  * @brief Constructor with value.
  *
  * @param x magnetization
  *
  * @note In MagP64 the value is equal to the mag.
  *
  */
  MagP64 (const double & x) : mag (x)                         {};

  /**
  * @brief Default destructor.
  *
  */
  ~MagP64 ()                                                  = default;

  /**
  * @brief Copy constructor.
  *
  * @param m MagP64 object
  *
  */
  MagP64 (const MagP64 & m)                                   { this->mag = m.mag; }

  /**
  * @brief Assigment operator constructor.
  *
  * @param m MagP64 object
  *
  */
  MagP64 & operator = (const MagP64 & m)                      { this->mag = m.mag; return *this; }

  // utilities

  /**
  * @brief Return the mag description (plain for MagP64).
  *
  */
  std :: string magformat () const                            { return "plain"; }

  /**
  * @brief Get the magnetization value.
  *
  * @note In MagP64 the value is equal to the mag.
  *
  */
  double value () const                                       { return this->mag; }


  // math operators

  /**
  * @brief Overload operator.
  *
  * @details Add magnetization ( (m1 + m2) / (1 + m1*m2) ) with clamp.
  *
  * @see mag :: clamp
  *
  * @param m MagP64 object
  *
  */
  MagP64 operator % (const MagP64 & m)                        { return MagP64(mag :: clamp( (this->mag + m.mag) / (1. + this->mag * m.mag), -1., 1.) ); }

  /**
  * @brief Overload operator.
  *
  * @details Just a simple addition of the mag values.
  *
  * @param m MagP64 object
  *
  */
  MagP64 operator + (const MagP64 & m)                        { return MagP64(this->mag + m.mag); }

  /**
  * @brief Overload operator.
  *
  * @details Just a simple division as (mag / x)
  *
  * @param x double value
  *
  */
  MagP64 operator / (const double & x)                        { return MagP64(this->mag / x); }

  /**
  * @brief Overload operator.
  *
  * @details Just a simple product as (mag * x)
  *
  * @param x double value
  *
  */
  double operator * (const double & x)                        { return this->mag * x; }

  /**
  * @brief Overload operator.
  *
  * @details Just a simple product as (x * mag)
  *
  * @param x double value
  * @param m MagP64 object
  *
  */
  friend double operator * (const double & x, const MagP64 & m) { return m.mag * x; }

  /**
  * @brief Overload operator
  *
  * @details Combine two mags as (mag * mag)
  *
  * @param m MagP64 object
  *
  */
  MagP64 operator ^ (const MagP64 & m)                        { return MagP64(this->mag * m.mag); }

  /**
  * @brief Overload operator.
  *
  * @details Subtract values (val1 - val2)
  *
  * @note In MagP64 the values are equal to the mags.
  *
  * @param m MagP64 object
  *
  */
  double operator - (const MagP64 & m)                        { return this->mag - m.mag; }

  /**
  * @brief Get a magnetization with a flipped sign.
  *
  * @details flip magnetization sign
  *
  */
  MagP64 operator - () const                                  { return MagP64(-this->mag); }

  // logical operators

  /**
  * @brief Check magnetization equality.
  *
  * @details Compare magnetizations
  *
  * @param m MagP64 object
  *
  */
  bool operator == (const MagP64 & m)                         { return this->mag == m.mag; }

  /**
  * @brief Check magnetization not equality.
  *
  * @details compare magnetizations
  *
  * @param m MagP64 object
  *
  */
  bool operator != (const MagP64 & m)                         { return this->mag != m.mag; }

  /**
  * @brief Print operator with stdout/stderr.
  *
  * @details print mag
  *
  * @param os ostream operator
  * @param m MagP64 object
  *
  */
  friend std :: ostream & operator << (std :: ostream & os, const MagP64 & m);
};

#endif // __magp_h__
