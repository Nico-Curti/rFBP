#ifndef __magp_h__
#define __magp_h__

#include <string>

/**
* @class MagP64
* @brief Abstract type representing magnetization type chosen for cavity messages
*
* @details MagP64 means a double type without tanh application
*
*/
struct MagP64
{
  // member
  double mag;   ///< magnetization

  // constructor and destructor
  /**
  * @brief default constructor
  *
  */
  MagP64 ()                                                   {};

  /**
  * @brief constructor with value
  *
  * @param x magnetization (equal to value in magP)
  *
  */
  MagP64 (const double & x) : mag (x)                         {};

  /**
  * @brief default destructor
  *
  */
  ~MagP64 ()                                                  = default;

  /**
  * @brief copy constructor
  *
  * @param m MagP64 object
  *
  */
  MagP64 (const MagP64 & m)                                   { this->mag = m.mag; }

  /**
  * @brief assigment operator constructor
  *
  * @param m MagP64 object
  *
  */
  MagP64 & operator = (const MagP64 & m)                      { this->mag = m.mag; return *this; }

  // utilities

  /**
  * @brief string format utility (=> "plain")
  *
  */
  std :: string magformat () const                            { return "plain"; }

  /**
  * @brief magnetization value (equal to mag in MagP64)
  *
  */
  double value () const                                       { return this->mag; }


  // math operators

  /**
  * @brief % operator
  *
  * @detail add magnetization ( (m1 + m2) / (1 + m1*m2) ) with clamp
  *
  * @param m MagP64 object
  *
  */
  MagP64 operator % (const MagP64 & m)                        { return MagP64(mag :: clamp( (this->mag + m.mag) / (1. + this->mag * m.mag), -1., 1.) ); }

  /**
  * @brief + operator
  *
  * @detail add magnetization
  *
  * @param m MagP64 object
  *
  */
  MagP64 operator + (const MagP64 & m)                        { return MagP64(this->mag + m.mag); }

  /**
  * @brief += operator
  *
  * @detail add magnetization AND value
  *
  * @param m MagP64 object
  *
  */
  MagP64& operator += (const MagP64 & m)                      { this->mag += m.mag; return *this; }

  /**
  * @brief / operator
  *
  * @detail divide magnetization (mag / x)
  *
  * @param x double value
  *
  */
  MagP64 operator / (const double & x)                        { return MagP64(this->mag / x); }

  /**
  * @brief * operator
  *
  * @detail prod magnetization (mag * x)
  *
  * @param x double value
  *
  */
  double operator * (const double & x)                        { return this->mag * x; }

  /**
  * @brief * operator
  *
  * @detail prod magnetization (x * mag)
  *
  * @param x double value
  * @param m MagP64 object
  *
  */
  friend double operator * (const double & x, const MagP64 & m) { return m.mag * x; }

  /**
  * @brief ^ operator
  *
  * @detail combination of mags (m1 * m2)
  *
  * @param m MagP64 object
  *
  */
  MagP64 operator ^ (const MagP64 & m)                        { return MagP64(this->mag * m.mag); }

  /**
  * @brief - operator
  *
  * @detail sub values (val1 - val2) <- equal to mag!
  *
  * @param m MagP64 object
  *
  */
  double operator - (const MagP64 & m)                        { return this->mag - m.mag; }

  /**
  * @brief - operator
  *
  * @detail flip magnetization sign
  *
  */
  MagP64 operator - () const                                  { return MagP64(-this->mag); }

  // logical operators

  /**
  * @brief == operator
  *
  * @detail compare magnetizations
  *
  * @param m MagP64 object
  *
  */
  bool operator == (const MagP64 & m)                         { return this->mag == m.mag; }

  /**
  * @brief != operator
  *
  * @detail compare magnetizations
  *
  * @param m MagP64 object
  *
  */
  bool operator != (const MagP64 & m)                         { return this->mag != m.mag; }

  /**
  * @brief << operator
  *
  * @detail print mag
  *
  * @param os ostream operator
  * @param m MagP64 object
  *
  */
  friend std :: ostream & operator << (std :: ostream & os, const MagP64 & m);
};

#endif // __magp_h__
