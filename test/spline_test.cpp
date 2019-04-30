#include <iostream>
#include <cmath>
#include <spline.h>
#include <iomanip>


int main ()
{
  const int N = 10;
  double * X = new double[N],
         * Y = new double[N];

  for (int i = 0; i < N; ++i) X[i] = 1. + i * .1;
  for (int i = 0; i < N; ++i) Y[i] = std :: atanh(std :: erf(X[i]));

  spline s;
  s.set_boundary(spline :: second_deriv, -20., spline :: second_deriv, -20., false);

  s.set_points(X, Y, N);

  for (int i = 0; i < N; ++i)
    std :: cout << std :: setw(4) << X[i] << " "
                << std :: setw(4) << Y[i] << " "
                << std :: setw(4) << s(X[i])
                << std :: endl;

  std :: cout << "=======================" << std :: endl;

  std :: cout << "Spline at 1.234 is " << s(1.234) << std :: endl;


  delete[] X;
  delete[] Y;

  return 0;
}

