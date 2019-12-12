#include <atanherf.h>

namespace AtanhErf
{
  spline getinp ()
  {
    std :: string filename = std :: string(PWD) + "/data/atanherf_interp.max_16.step_0.0001.first_1.dat";
    std :: ifstream is(filename, std :: ios :: binary);

    spline inp;
    inp.load_points(filename);
    return inp;
  }

  double atanherf_interp (const double & x)
  {
    static spline inp = getinp();
    double res = inp(x);
    return res;
  }

  double evalpoly (const double & x)
  {
    const double x2 = x * x;
    const double x3 = x2 * x;
    const double x6 = x3 * x3;
    return 1. - 1.25 * x + 3.0833333333333335 * x2 -
           11.03125 * x3 + 51.0125 * x2 * x2 -
           287.5260416666667 * x3 * x2 + 1906.689732142857 * x6 -
           14527.3759765625 * x6 * x + 125008.12543402778 * x6 * x2 -
           1.1990066259765625e6 * x6 * x3;
  }

  double atanherf_largex (const double & x)
  {
    const double x2 = x * x;
    const double ix = 1. / x2;
    return sign(x) * (std :: log(x2 * 4 * M_PI) + 2. * x2 + ix * evalpoly(ix)) * .25;
  }

  double atanherf (const double & x)
  {
    const double ax = std :: abs(x);
    return (ax <= 2.)                    ?
           std :: atanh(std :: erf(x))   :
           (ax <= 15.)                   ?
           sign(x) * atanherf_interp(ax) :
           atanherf_largex(x)            ;
  }
}
