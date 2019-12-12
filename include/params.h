#ifndef __params_h__
#define __params_h__

#include <magnetization.h>

/**
* @class Params
* @brief Class to wrap training parameters
*
* @tparam Mag magnetization used
*
*/
template < class Mag >
class Params
{

public:

  // Members

  long int max_iters;      ///< number of iterations
  double   damping,        ///< damping
           epsil,          ///< error tollerance
           beta,           ///< 1/kT
           r;              ///< number of replicas -1
  Mag      tan_gamma;      ///< hyperbolic tangent of distance weight between replicas (gamma)
  std :: string accuracy1, ///< updating accuracy of cavity probability (messages of hidden layers)
                accuracy2; ///< updating accuracy of cavity probability (messages of output node)

  /**
  * @brief Parameter constructor
  *
  * @param max_iters number of iterations
  * @param damping damping factor
  * @param epsil error tollerance
  * @param beta 1 / KT
  * @param r number of replicas - 1
  * @param tan_gamma hyperbolic tangent of distance weight between replicas (gamma) as Mag object
  * @param accuracy1 updating accuracy of cavity probability (messages of hidden layers)
  * @param accuracy2 updating accuracy of cavity probability (messages of otuput node)
  *
  */
  Params (const int & max_iters,
          const double & damping,
          const double & epsil,
          const double & beta,
          const double & r,
          const double & gamma,
          const std :: string & accuracy1,
          const std :: string & accuracy2
          );

  /**
  * @brief default destructor
  *
  */
  ~Params () = default;
};

#endif // __params_h__
