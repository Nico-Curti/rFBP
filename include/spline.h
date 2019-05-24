#ifndef SPLINE_H
#define SPLINE_H

#include <memory>
#include <array>
#include <algorithm>
#include <fstream>
#ifdef DEBUG
#include <numeric>
#include <cassert>
#endif
#include <iostream>
#include <iterator>
#include <utils.h>

static constexpr double one_third = 1. / 3.;
static constexpr double two_third = 2. / 3.;


class band_matrix
{

private:

  // Member variables

  int dim;
  std :: array < std :: unique_ptr < double[] >, 2> m_upper,
                                                    m_lower;
public:

  // Constructors

  band_matrix(const int & dim);

  // Destructors

  ~band_matrix() = default;

  // Operators

  double & operator()(const int & i, const int & j);
  double operator()(const int & i, const int & j) const;

  // Public Members

  std :: unique_ptr < double[] > lu_solve(const double *b);

private:

  // Private members

  void lu_decompose();
  std :: unique_ptr < double[] > l_solve(const double *b) const;
  std :: unique_ptr < double[] > r_solve(const double *b) const;

};


class spline
{

public:
  enum bd_type { first_deriv = 1, second_deriv = 2};
  enum spline_type { linear_spline = 1, cubic_spline = 2};

private:
  bool m_force_linear_extrapolation;
  bd_type m_left, m_right;
  int n;
  double mb0, mc0, m_left_value, m_right_value;
  std :: unique_ptr < double[] > mx, my, ma, mb, mc;

#ifdef DEBUG
  void _assert_increasing(const double * t, const int & nt);
#endif

public:

  spline ();

  spline (const spline &s);
  ~spline() = default;

  void set_boundary(const bd_type & left, const double & left_value, const bd_type & right, const double & right_value, bool force_linear_extrapolation = false);
  void set_points(double * & x, double * & y, const int & npts, spline_type type = cubic_spline);
  double operator() (const double & x) const;
  double deriv(const int & order, const double & x) const;
  spline & operator = (const spline & s);

  void load_points (const std :: string & filename);
  void dump_points (const std :: string & filename);

  friend std :: ostream & operator << (std :: ostream & os, const spline & s);
};

std :: ostream & operator << (std :: ostream & os, const spline & s);

#endif
