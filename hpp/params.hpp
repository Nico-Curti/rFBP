#ifndef __params_hpp__
#define __params_hpp__

#include <params.h>

template < typename Mag >
Params < Mag > :: Params (const int & max_iters,
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
{
}


#endif // __params_hpp__
