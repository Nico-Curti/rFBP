#ifndef __params_h__
#define __params_h__

#include <magnetization.h>

/**
* @class Params
* @brief Class to wrap training parameters.
*
* @details This class is used by the rFBP functions to facilitate the moving of a set
* of training parameters along the series of functions.
*
* @tparam Mag magnetization used
*
*/
template < class Mag >
class Params
{

public:

  // Members

  long int max_iters;      ///< Number of iterations
  double   damping,        ///< Damping factor
           epsil,          ///< Error tollerance
           beta,           ///< 1/kT
           r;              ///< Number of replicas -1
  Mag      tan_gamma;      ///< Hyperbolic tangent of distance weight between replicas (γ)
  std :: string accuracy1, ///< Updating accuracy of cavity probability (messages of hidden layers)
                accuracy2; ///< Updating accuracy of cavity probability (messages of output node)

  /**
  * @brief Parameter constructor.
  *
  * @param max_iters Number of iterations
  * @param damping Damping factor
  * @param epsil Error tollerance
  * @param beta 1 / KT
  * @param r Number of replicas - 1
  * @param gamma Hyperbolic tangent of distance weight between replicas (γ) as Mag object
  * @param accuracy1 Updating accuracy of cavity probability (messages of hidden layers)
  * @param accuracy2 Updating accuracy of cavity probability (messages of otuput node)
  *
  * @note In the constructor the value of gamma is converted to the appropriated Mag type.
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
  * @brief Default destructor.
  *
  */
  ~Params () = default;
};

#endif // __params_h__
