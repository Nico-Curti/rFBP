#ifndef SPLINE_H
#define SPLINE_H

#include <memory>
#include <array>
#include <algorithm>
#include <fstream>
#ifdef DEBUG
#include <cassert>
#endif

static constexpr double one_third = 1. / 3.;
static constexpr double two_third = 2. / 3.;


class band_matrix
{
private:
  int dim;
  std::array<std::unique_ptr<double[]>, 2> m_upper,
                                           m_lower;
public:
  band_matrix(const int &dim) : dim(dim)
  {
    this->m_upper[0] = std::make_unique<double[]>(dim);
    this->m_upper[1] = std::make_unique<double[]>(dim);
    this->m_lower[0] = std::make_unique<double[]>(dim);
    this->m_lower[1] = std::make_unique<double[]>(dim);
  }
  ~band_matrix() = default;

  double& operator()(const int &i, const int &j)
  {
    const int k = j - i;
#ifdef DEBUG
    assert( i >= 0 && i < this->dim && j >= 0 && j < this->dim);
    assert( -1 <= k && k <= 1);
#endif
    return k >= 0 ? this->m_upper[k][i] : this->m_lower[-k][i];
  }
  double operator()(const int &i, const int &j) const
  {
    const int k = j - i;
#ifdef DEBUG
    assert( i >= 0 && i < this->dim && j >= 0 && j < this->dim);
    assert( -1 <= k && k <= 1);
#endif
    return k >= 0 ? this->m_upper[k][i] : this->m_lower[-k][i];
  }

  void lu_decompose()
  {
    // preconditioning
    // normalize column i so that a_ii=1
    for (int i = 0; i < this->dim; ++i)
    {
      this->m_lower[0][i] = 1. / this->operator()(i, i);
      const int j_min = i - 1 > 0 ? i - 1 : 0;
      const int j_max = this->dim - 1 < i + 1 ? this->dim - 1 : i + 1;
      for (int j = j_min; j <= j_max; ++j)
        this->operator()(i, j) *= this->m_lower[0][i];
      this->operator()(i, i) = 1.; // prevents rounding errors
    }

    // Gauss LR-Decomposition
    for (int i = 0; i < this->dim; ++i)
    {
      const int j_max = this->dim - 1 < i + 1 ? this->dim - 1 : i + 1;
      for (int j = i + 1; j <= j_max; ++j)
      {
        const double x = -this->operator()(j, i) / this->operator()(j, j);
        this->operator()(j, i) = -x;
        for (int k = i + 1; k <= j_max; ++k)
          this->operator()(j, k) = this->operator()(j, k) + x * this->operator()(i, k);
      }
    }
  }

  auto l_solve(const double *b) const
  {
    std::unique_ptr<double[]> x(new double[this->dim]);
    for (int i = 0; i < this->dim; ++i)
    {
      double sum = 0.;
      const int j_start = i - 1 > 0 ? i - 1 : 0;
      for (int j = j_start; j < i; ++j)
        sum += this->operator()(i, j) * x[j];
      x[i] = (b[i] - this->m_lower[0][i]) - sum;
    }
    return x;
  }
  auto r_solve(const double *b) const
  {
    std::unique_ptr<double[]> x(new double[this->dim]);
    for (int i = this->dim - 1; i >= 0; --i)
    {
      double sum = 0.;
      const int j_stop = this->dim - 1 > i + 1 ? i + 1 : this->dim - 1;
      for (int j = i + 1; j <= j_stop; ++j)
        sum += this->operator()(i, j) * x[j];
      x[i] = (b[i] - sum) / this->operator()(i, i);
    }
    return x;
  }

  auto lu_solve(const double *b)
  {
    this->lu_decompose();
    auto y = this->l_solve(b);
    auto x = this->r_solve(y.get());
    return x;
  }

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
  std::unique_ptr<double[]> mx, my, ma, mb, mc;

#ifdef DEBUG
  inline void _assert_increasing(const float *t, const int &nt)
  {
    const auto check = std::inner_product(t, t + nt - 1,
                                          t + 1, 0,
                                          [](const float &i, const float &j)
                                          {
                                            return i < j ? 1 : 0;
                                          });
    assert(check == nt - 1);
  }
#endif

public:

  spline() : m_left(second_deriv), m_right(second_deriv),
             n(0),
             mb0(0.), mc0(0.), m_left_value(0.), m_right_value(0.),
             mx(nullptr), my(nullptr), ma(nullptr), mb(nullptr), mc(nullptr)
  {};
  ~spline() = default;

  void set_boundary(const bd_type &left, const double &left_value, const bd_type &right, const double &right_value, bool force_linear_extrapolation=false)
  {
#ifdef DEBUG
    assert(this->n == 0);
#endif
    this->m_left_value  = left_value;
    this->m_right_value = right_value;
    this->m_left        = left;
    this->m_right       = right;
    this->m_force_linear_extrapolation = force_linear_extrapolation;
  }

  void load_points(const std::string &filename)
  {
    std::ifstream is(filename, std::ios::binary);
    is.read((char*)&this->m_force_linear_extrapolation, sizeof(bool));
    is.read((char*)&this->m_left,                       sizeof(int));
    is.read((char*)&this->m_right,                      sizeof(int));
    is.read((char*)&this->n,                            sizeof(int));
    is.read((char*)&this->mb0,                          sizeof(double));
    is.read((char*)&this->mc0,                          sizeof(double));
    is.read((char*)&this->m_left_value,                 sizeof(double));
    is.read((char*)&this->m_right_value,                sizeof(double));

    this->mx.reset(new double[this->n]);
    this->my.reset(new double[this->n]);
    this->ma.reset(new double[this->n]);
    this->mb.reset(new double[this->n]);
    this->mc.reset(new double[this->n]);

    is.read(reinterpret_cast<char*>(this->mx.get()), sizeof(double)*this->n);
    is.read(reinterpret_cast<char*>(this->my.get()), sizeof(double)*this->n);
    is.read(reinterpret_cast<char*>(this->ma.get()), sizeof(double)*this->n);
    is.read(reinterpret_cast<char*>(this->mb.get()), sizeof(double)*this->n);
    is.read(reinterpret_cast<char*>(this->mc.get()), sizeof(double)*this->n);

    is.close();
  }

  void dump_points(const std::string &filename)
  {
    std::ofstream os(filename, std::ios::out | std::ios::binary);
    os.write((char*)&this->m_force_linear_extrapolation, sizeof(bool));
    os.write((char*)&this->m_left,                       sizeof(int));
    os.write((char*)&this->m_right,                      sizeof(int));
    os.write((char*)&this->n,                            sizeof(int));
    os.write((char*)&this->mb0,                          sizeof(double));
    os.write((char*)&this->mc0,                          sizeof(double));
    os.write((char*)&this->m_left_value,                 sizeof(double));
    os.write((char*)&this->m_right_value,                sizeof(double));

    os.write(reinterpret_cast<char*>(this->mx.get()), sizeof(double)*this->n);
    os.write(reinterpret_cast<char*>(this->my.get()), sizeof(double)*this->n);
    os.write(reinterpret_cast<char*>(this->ma.get()), sizeof(double)*this->n);
    os.write(reinterpret_cast<char*>(this->mb.get()), sizeof(double)*this->n);
    os.write(reinterpret_cast<char*>(this->mc.get()), sizeof(double)*this->n);

    os.close();
  }

  void set_points(double *&x, double *&y, const int &npts, spline_type type = cubic_spline)
  {
    this->n  = npts;
    this->mx = std::make_unique<double[]>(npts);
    this->my = std::make_unique<double[]>(npts);
    std::move(x, x + npts, this->mx.get());
    std::move(y, y + npts, this->my.get());

#ifdef DEBUG
    this->_assert_increasing(mx);
#endif

    switch (type)
    {
      default: case cubic_spline:
      {
        band_matrix A(npts);
        std::unique_ptr<double[]> rhs(new double[npts]);
        for (int i = 1, j = 2, k = 0; i < npts - 1; ++i, ++j, ++k)
        {
          A(i, k) = one_third * (x[i] - x[k]);
          A(i, i) = two_third * (x[j] - x[k]);
          A(i, j) = one_third * (x[j] - x[i]);
          rhs[i] = (y[j] - y[i]) / (x[j] - x[i]) - (y[i] - y[k]) / (x[i] - x[k]);
        }

        A(0, 0) = this->m_left == second_deriv ? 2.                 : 2. * (x[1] - x[0]);
        A(0, 1) = this->m_left == second_deriv ? 0.                 :      (x[1] - x[0]);
        rhs[0]  = this->m_left == second_deriv ? this->m_left_value : 3. * ((y[1] - y[0]) / (x[1] - x[0]) - this->m_left_value);

        A(npts - 1, npts - 1) = this->m_right == second_deriv ? 2.                  : 2. * (x[npts - 1] - x[npts - 2]);
        A(npts - 1, npts - 2) = this->m_right == second_deriv ? 0.                  :      (x[npts - 1] - x[npts - 2]);
        rhs[npts - 1]         = this->m_right == second_deriv ? this->m_right_value : 3. * (this->m_right_value - (y[npts - 1] - y[npts - 2]) / (x[npts - 1] - x[npts - 2]));

        this->mb = A.lu_solve(rhs.get());

        this->ma = std::make_unique<double[]>(npts);
        this->mc = std::make_unique<double[]>(npts);
        for (int i = 0, j = 1; i < npts - 1; ++i, ++j)
        {
          this->ma[i] = one_third * (this->mb[j] - this->mb[i]) / (x[j] - x[i]);
          this->mc[i] = (y[j] - y[i]) / (x[j] - x[i]) - one_third * (2. * this->mb[i] + this->mb[j]) * (x[j] - x[i]);
        }

      } break;
      case linear_spline:
      {
        this->ma = std::make_unique<double[]>(npts);
        this->mb = std::make_unique<double[]>(npts);
        this->mc = std::make_unique<double[]>(npts);
        std::fill_n(this->ma.get(), npts - 1, 0.f);
        std::fill_n(this->mb.get(), npts - 1, 0.f);
        for (int i = 0, j = 1; i < npts - 1; ++i, ++j)
          this->mc[i] = (my[j] - my[i]) / (mx[j] - mx[i]);
      } break;
    }

    this->mb0 = m_force_linear_extrapolation ? 0. : mb[0];
    this->mc0 = mc[0];

    const double h = x[npts - 1] - x[npts - 2];

    ma[npts - 1] = 0.;
    mc[npts - 1] = 3. * ma[npts - 2] * h * h + 2. * mb[npts - 2] * h + mc[npts - 2];
    mb[npts - 1] = 0.;

  }

  double operator() (const double &x) const
  {
    const auto it  = std::lower_bound(this->mx.get(), this->mx.get() + this->n, x);
    const int pos  = it - this->mx.get() - 1;
    const int idx  = pos > 0 ? pos : 0;
    const double h = x - this->mx[idx];

    const double interp = x < this->mx[0]           ? (this->mb0 * h + this->mc0)*h + this->my[0] : // extrapolation to the left
                          x > this->mx[this->n - 1] ? (this->mb[this->n - 1] * h + this->mc[this->n - 1]) * h + this->my[this->n - 1] : // extrapolation to the right
                                                      ((this->ma[idx]*h + this->mb[idx]) * h + this->mc[idx]) * h + this->my[idx]; // interpolation
    return interp;
  }

  double deriv(const int &order, const double &x) const
  {
#ifdef DEBUG
    assert(order > 0);
#endif
    const auto it  = std::lower_bound(this->mx.get(), this->mx.get() + this->n, x);
    const int pos  = it - this->mx.get() - 1;
    const int idx  = pos > 0 ? pos : 0;
    const double h = x - this->mx[idx];

    switch (order)
    {
      case 1:  return x < this->mx[0]           ? 2. * this->mb0 * h + this->mc0 :
                      x > this->mx[this->n - 1] ? 2. * this->mb[this->n - 1]     :
                                                 (3. * this->ma[idx] * h + 2. * this->mb[idx]) * h + this->mc[idx];
      break;
      case 2:  return x < this->mx[0]           ? 2. * this->mb0 * h :
                      x > this->mx[this->n - 1] ? 2. * this->mb[this->n - 1] :
                                                  6. * this->ma[idx] * h + 2. * this->mb[idx];
      break;
      case 3:  return x < this->mx[0] || x > this->mx[this->n - 1] ? 0. : 6. * this->ma[idx];
      default: return 0.;
      break;
    }

  }

};

#endif
