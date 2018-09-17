#include <atanherf.h>

namespace AtanhErf
{
  void getinp(const double &mm, const double &st, std::unique_ptr<double[]> &inp)
  {
    std::string filename = "atanherf_interp.max_" + std::to_string(mm) + ".step_" + std::to_string(st) + ".txt";
    std::ifstream is(filename, std::ios::binary);
    int i = 0;
    if (is)
    {
      int N = static_cast<int>((mm - 1.) / st);
      inp = std::make_unique<double[]>( N );

      for(int i = 0; i < N; ++i) is.read((char*)&inp[i], sizeof(double));
      is.close();
      return;
    }
    else
    {
      // temporary solution without splines
#ifdef VERBOSE
      std::cout << "Computing atanh(erf(x)) table, this may take a while..." << std::endl;
#endif
      std::ofstream os(filename, std::ios::out | std::ios::binary);

      int N = static_cast<int>((mm - 1.) / st);
      inp = std::make_unique<double[]>( N );

      for (int i = 0; i < N; ++i)
      {
        inp[i] = std::atanh(std::erf(i * st + 1.));
        os.write( (const char *) &inp[i], sizeof( double ));
      }

      os.close();
      return;
    }
  }

  double atanherf_interp(const double &x)
  {
    int N = static_cast<int>( 15. / 1e-4 ); // TODO
    std::unique_ptr<double[]> r(new double[N]),
                              inp = nullptr;
    double res = 0.;

    std::generate_n(r.get(), N, [&res](){return (res++) * 1e-4 + 1.;} ); // TODO
    getinp(16., 1e-4, inp);
    res = inp[static_cast<int>((x - r[0]) / 1e-4 + 1.)]; // TODO

    return res;
  }

  double evalpoly(const double &t)
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

  double atanherf_largex(const double &x)
  {
    double t = 1. / (x * x);
    return sign(x) * (2. * std::log(std::abs(x)) + std::log(4. * M_PI) + 2. * x * x + t * evalpoly(t)) * .25;
  }

  double atanherf(const double &x)
  {
    double ax = std::abs(x);
    return (ax <= 2.)                    ?
           std::atanh(std::erf(x))       :
           (ax <= 15.)                   ?
           sign(x) * atanherf_interp(ax) :
           atanherf_largex(x)            ;
  }
}