#include <atanherf.h>
#include <spline.h>

namespace AtanhErf
{
  auto getinp()
  {
    std :: string filename = "atanherf_interp.max_16.step_0.0001.first_1.dat";
    std :: ifstream is(filename, std :: ios :: binary);
    if ( !is )
    {
      std :: cerr << "AtanhErf coefficients file not found! Given: " << filename << std :: endl;
      std :: exit(201);
    }
    spline inp;
    inp.load_points(filename);
    return inp;
  }

  double atanherf_interp (const double & x)
  {
    static auto inp = getinp();
    double res = inp(x);
    return res;
  }

  double evalpoly(const double & t)
  {
    double t2 = t * t,
           t3 = t * t * t,
           t6 = t3 * t3;
    return 1. - 1.25 * t + 3.0833333333333335 * t2 -
           11.03125 * t3 + 51.0125 * t2 * t2 -
           287.5260416666667 * t3 * t2 + 1906.689732142857 * t6 -
           14527.3759765625 * t6 * t + 125008.12543402778 * t6 * t2 -
           1.1990066259765625e6 * t6 * t3;
  }

  double atanherf_largex(const double & x)
  {
    double t = 1. / (x * x);
    return sign(x) * (2. * std :: log(std :: abs(x)) + std :: log(4. * M_PI) + 2. * x * x + t * evalpoly(t)) * .25;
  }

  double atanherf(const double & x)
  {
    double ax = std :: abs(x);
    return (ax <= 2.)                    ?
           std :: atanh(std :: erf(x))   :
           (ax <= 15.)                   ?
           sign(x) * atanherf_interp(ax) :
           atanherf_largex(x)            ;
  }
}
