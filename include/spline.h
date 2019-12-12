#ifndef __spline_h__
#define __spline_h__

#include <memory>
#include <array>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>

#ifdef DEBUG

  #include <numeric>
  #include <cassert>

#endif

#include <utils.hpp>

static constexpr double one_third = 1. / 3.; ///< shortcut for 1/3
static constexpr double two_third = 2. / 3.; ///< shortcut for 2/3

/**
* @class band_matrix
* @brief Utility class for spline evaluation.
*
* @details It is used for LU decomposition
*
*/
class band_matrix
{

private:

  // Member variables

  std :: array < std :: unique_ptr < double[] >, 2 > m_upper; ///< array with upper "part" of matrix
  std :: array < std :: unique_ptr < double[] >, 2 > m_lower; ///< array with lower "part" of matrix

  int dim; ///< matrix dimension (square)

public:

  // Constructors

  /**
  * @brief default constructor
  *
  */
  band_matrix (const int & dim);

  // Destructors

  /**
  * @brief destructor set as default
  *
  */
  ~band_matrix () = default;

  // Operators

  /**
  * @brief Matrix element extraction
  *
  * @param i row index
  * @param j col index
  *
  * @return double reference to the element
  *
  */
  double & operator () (const int & i, const int & j);
  /**
  * @brief Matrix element extraction
  *
  * @param i row index
  * @param j col index
  *
  * @return double element
  *
  */
  double operator () (const int & i, const int & j) const;

  // Public Members

  std :: unique_ptr < double[] > lu_solve (const double * b);

private:

  // Private members

  void lu_decompose ();
  std :: unique_ptr < double[] > l_solve (const double * b) const;
  std :: unique_ptr < double[] > r_solve (const double * b) const;

};


class spline
{

public:

  enum bd_type { first_deriv = 1, second_deriv = 2};
  enum spline_type { linear_spline = 1, cubic_spline = 2};

private:

  std :: unique_ptr < double[] > mx;
  std :: unique_ptr < double[] > my;
  std :: unique_ptr < double[] > ma;
  std :: unique_ptr < double[] > mb;
  std :: unique_ptr < double[] > mc;

  double mb0;
  double mc0;
  double m_left_value;
  double m_right_value;

  int n;

  bd_type m_left;
  bd_type m_right;

  bool m_force_linear_extrapolation;


#ifdef DEBUG

  void _assert_increasing (const double * t, const int & nt);

#endif

public:

  // Constructors

  /**
  * @brief default constructor
  *
  */
  spline ();

  // Copy constructors

  /**
  * @brief copy constructor
  *
  * @param s spline object
  *
  */
  spline (const spline & s);
  /**
  * @brief assignment operator constructor
  *
  * @param s spline object
  *
  */
  spline & operator = (const spline & s);

  // Destructors

  /**
  * @brief destructor set as default
  *
  */
  ~spline () = default;

  // Operators

  /**
  * @brief get spline interpolation value
  *
  * @param x coordinate
  *
  * @return double interpolation value
  *
  */
  double operator() (const double & x) const;

  // Public members

  void set_boundary (const bd_type & left, const double & left_value, const bd_type & right, const double & right_value, bool force_linear_extrapolation = false);
  void set_points (double * & x, double * & y, const int & npts, spline_type type = cubic_spline);
  double deriv (const int & order, const double & x) const;

  void load_points (const std :: string & filename);
  void dump_points (const std :: string & filename);

  friend std :: ostream & operator << (std :: ostream & os, const spline & s);
};

std :: ostream & operator << (std :: ostream & os, const spline & s);

#endif // __spline_h__
