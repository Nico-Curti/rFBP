#ifndef __fprotocol_h__
#define __fprotocol_h__

#include <memory>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <climits>
#include <errors.h>
#include <utils.hpp>

#ifdef DEBUG

  #include <cassert>

#endif

/**
* @class FocusingProtocol
* @brief Abstract type representing a protocol for the focusing procedure, i.e. a way to produce
* successive values for the quantities `gamma`, `n_rep` and `beta`. Currently, however, only `beta=Inf`
* is supported. To be provided as an argument to `focusingBP`.
*
* @details Available protocols are: `StandardReinforcement`, `Scoping`, `PseudoReinforcement` and `FreeScoping`
*
*/
class FocusingProtocol
{

public:

  // Member variables

  long int Nrep;                         ///< number of repetitions, i.e. number of focusing iterations
  std :: unique_ptr < double[] > gamma;  ///< distance parameters
  std :: unique_ptr < double[] > n_rep;  ///< (y in the paper and original code) number of replicas
  std :: unique_ptr < double[] > beta;   ///< 1/kT

  // Constructors

  /**
  * @brief default constructor
  *
  */
  FocusingProtocol ();
  /**
  * @brief Constructor with protocol type and number of replicas
  *
  * @details Protocol types are set with default values. If you want
  * introduce other values you must use appropriated protocol functions
  *
  * @param prot protocol type. Available protocols are: `StandardReinforcement`, `Scoping`, `PseudoReinforcement` and `FreeScoping`.
  * This value is used to switch between the available protocols and the corresponding arrays are stored.
  * @param size number of step. Converted to Nrep into the class
  *
  */
  FocusingProtocol (const std :: string & prot, const long int & size);

  // Destructors

  /**
  * @brief destructor set as default
  *
  */
  ~FocusingProtocol () = default;

  /**
  * @brief Standard reinforcement protocol, returns `gamma=Inf` and `n_rep=1/(1-x)`, where `x` is taken from the given range `rho`.
  *
  * @param rho double pointer which store the range values of x
  * @param size number of step. Converted to Nrep into the class
  *
  */
  void StandardReinforcement (const double * rho, const long int & Nrho);

  /**
  * @brief Shorthand for Standard reinforcement protocol.
  *
  * @param drho double related to the range increment
  *
  */
  void StandardReinforcement (const double & drho);

  /**
  * @brief Focusing protocol with fixed `n_rep` and a varying `gamma` taken from the given `gamma * r` range.
  *
  * @param gr double pointer with gamma * r values
  * @param x fixed value of n_rep
  * @param ngr number of replicas
  *
  */
  void Scoping (const double * gr, const double & x, const long int & ngr);

  /**
  * @brief A focusing protocol in which both `gamma` and `n_rep` are progressively increased, according to
  * the formulas
  *
  * ```python
  * gamma = atanh(rho**x)
  * n_rep = 1 + rho**(1 - 2x) / (1 - rho)
  *
  * ```
  *
  * where `rho` is taken from the given range(ngr) `r`. With `x=0`, this is basically the same as `StandardReinforcement`
  *
  * @param rho double pointer with rho values
  * @param nrho lenght of rho array
  * @param x fixed value of n_rep
  */
  void PseudoReinforcement (const double * rho, const long int & nrho, double x=.5);

  /**
  * @brief Shorthand for Pseudo Reinforcement protocol.
  *
  * @param drho double related to the range increment
  *
  */
  void PseudoReinforcement (const double & drho, double x=.5);

  /**
  * @brief A focusing protocol which just returns the values of `(gamma, n_rep)` from the given `list`.
  *
  * @param list array of lists (nlist, 3) with values
  * @param nlist lenght of list
  *
  */
  void FreeScoping (double ** list, const long int & nlist);
};

#endif // __fprotocol_h__
