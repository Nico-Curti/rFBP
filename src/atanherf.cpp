#include <atanherf.h>
#include <spline.h>

namespace AtanhErf
{
  auto getinp()
  {
    // std::unique_ptr<double[]> inp = nullptr;
    std::string filename = "atanherf_interp.max_16.step_0.0001.first_1.txt";
    std::ifstream is(filename, std::ios::binary);
    if (!is)
    {
      std::cerr << "AtanhErf coefficients file not found! Given: " << filename << std::endl;
      std::exit(201);
    }
    spline inp;
    inp.load_points(filename);
    return inp;
//     std::string filename = "atanherf_interp.max_" + std::to_string(mm) + ".step_" + std::to_string(st) + ".first_" + std::to_string(first) + ".txt";
//     std::ifstream is(filename, std::ios::binary);
//     if (is)
//     {
//       // int N = static_cast<int>((mm - 1.) / st);
//       // inp = std::make_unique<double[]>( N );
//       //
//       // is.read(reinterpret_cast<char*>(inp.get()), sizeof(double)*N);
//       // is.close();
//       spline inp;
//       inp.load_points(filename);
//       return inp;
//     }
//     else
//     {
// // #ifdef VERBOSE
//       std::cout << "Computing atanh(erf(x)) table, this may take a while..." << std::endl;
// // #endif
//       std::ofstream os(filename, std::ios::out | std::ios::binary);
//       spline inp;
//       int N = static_cast<int>((mm - first) / st);
//       double * X = new double[N],
//              * Y = new double[N];
//       std::cout << "N = " << std::endl;
//       for (int i = 0; i < N; ++i) X[i] = first + i * st;
//       for (int i = 0; i < N; ++i) Y[i] = std::atanh(std::erf(X[i]));
//       inp.set_boundary(spline::second_deriv, 0., spline::second_deriv, 0., false);
//       inp.set_points(X, Y, N);
//       std::cout << inp << std::endl;
//       inp.dump_points(filename);
//       // temporary solution without splines
//       // inp = std::make_unique<double[]>( N );
//       //
//       // for (int i = 0; i < N; ++i) inp[i] = std::atanh(std::erf(i * st + 1.));
//       // os.write( reinterpret_cast<char*>(inp.get()), sizeof( double )*N);
//       //
//       // os.close();
//       return inp;
//     }
  }

  double atanherf_interp(const double &x)
  {
    // int N = static_cast<int>( 15. / 1e-4 ); // TODO
    // std::unique_ptr<double[]> r(new double[N]);
    // double res = 0.;
    // std::generate_n(r.get(), N, [&res](){return (res++) * 1e-4 + 1.;} ); // TODO
    // static auto inp = getinp(16., 1e-4);
    // res = inp[static_cast<int>((x - r[0]) / 1e-4 + 1.)]; // TODO
    static auto inp = getinp();
    double res = inp(x);
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
