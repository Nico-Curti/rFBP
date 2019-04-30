#ifndef PARAMS_H
#define PARAMS_H

template < class Mag >
class Params
{

public:

  // Members

  long int    max_iters;      // number of iterations
  double      damping,        // damping
              epsil,          // error tollerance
              beta,           // 1/kT
              r;              // number of replicas -1
  Mag         tan_gamma;      // hyperbolic tangent of distance weight between replicas (gamma)
  std :: string accuracy1,    // updating accuracy of cavity probability (messages of hidden layers)
                accuracy2;    // updating accuracy of cavity probability (messages of output node)

  Params (const int & max_iters,
          const double & damping,
          const double & epsil,
          const double & beta,
          const double & r,
          const double & gamma,
          const std :: string & accuracy1,
          const std :: string & accuracy2)
                                           : max_iters(max_iters),
                                             damping(damping),
                                             epsil(epsil),
                                             beta(beta),
                                             r(r),
                                             tan_gamma(Mag(gamma)),
                                             accuracy1(accuracy1),
                                             accuracy2(accuracy2)
          {};
  ~Params () = default;
};

#endif // PARAMS_H
