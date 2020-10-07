#ifndef __rfbp_h__
#define __rfbp_h__

#include <algorithm>
#include <numeric>

#ifdef DEBUG

  #include <cassert>

#endif

#ifdef STATS

  #include <iomanip>

#endif

/// @cond DEF
#ifdef _MSC_VER

  #ifndef __unused
    #define __unused
  #endif

#else

  #ifndef __unused
    #define __unused __attribute__((__unused__))
  #endif

#endif
/// @endcond

#include <cavity_message.hpp>
#include <fprotocol.h>
#include <pattern.h>

/**
* @brief Messages update for a perceptron-like factor graph (approximated version computationally efficient in the limit of large number of weights)
*
* @tparam Mag magnetization type (MagP or MagT)
* @param m Total magnetization of variables nodes belonging to lower layer
* @param M Total magnetization of variable node belonging to upper layer
* @param xi Single input pattern
* @param u Downward messages (cavity magnetizations) from factor node to lower variables nodes
* @param U Upward message (cavity magnetizations) from factor node to upper variable node
* @param params Parameters selected for the algorithm
* @param nxi Size of input pattern
* @param nm Number of variables node onto the lower layer
*
* @return Largest difference between new and old messages
*/
template < class Mag > double theta_node_update_approx (MagVec < Mag > m, Mag & M, const double * xi, MagVec < Mag > u, Mag & U, const Params < Mag > & params, const long int & nxi, const long int & nm);

/**
* @brief Messages update for a perceptron-like factor graph (fast approximated version)
*
* @tparam Mag magnetization type (MagP or MagT)
* @param m Total magnetization of variables nodes belonging to lower layer
* @param M Total magnetization of variable node belonging to upper layer
* @param xi Single input pattern
* @param u Downward messages (cavity magnetizations) from factor node to lower variables nodes
* @param U Upward message (cavity magnetizations) from factor node to upper variable node
* @param params Parameters selected for the algorithm
* @param nxi Size of input pattern
* @param nm Number of variables node onto the lower layer
*
* @return Largest difference between new and old messages
*/
template < class Mag > double theta_node_update_accurate (MagVec < Mag > m, Mag & M, const double * xi, MagVec < Mag > u, Mag & U, const Params < Mag > & params, const long int & nxi, const long int & nm);

/**
* @brief Messages update for a perceptron-like factor graph (exact version)
*
* @tparam Mag magnetization type (MagP or MagT)
* @param m Total magnetization of variables nodes belonging to lower layer
* @param M Total magnetization of variable node belonging to upper layer
* @param xi Single input pattern
* @param u Downward messages (cavity magnetizations) from factor node to lower variables nodes
* @param U Upward message (cavity magnetizations) from factor node to upper variable node
* @param params Parameters selected for the algorithm
* @param nxi Size of input pattern
* @param nm Number of variables node onto the lower layer
*
* @return Largest difference between new and old messages
*/
template < class Mag > double theta_node_update_exact (MagVec < Mag > m, Mag & M, const double * xi, MagVec < Mag > u, Mag & U, const Params < Mag > & params, const long int & nxi, const long int & nm);

/**
* @brief Computation of the free energy for a perceptron-like factor graph (fast approximated version)
*
* @tparam Mag magnetization type (MagP or MagT)
* @param m Total magnetization of variables nodes belonging to lower layer
* @param M Total magnetization of variable node belonging to upper layer
* @param xi Single input pattern
* @param u Downward messages (cavity magnetizations) from factor node to lower variables nodes
* @param U Upward message (cavity magnetizations) from factor node to upper variable node
* @param nxi Size of input pattern
* @param nm Number of variables node onto the lower layer
*
* @return Free energy for the system represented by a perceptron-like factor graph
*/
template < class Mag > double free_energy_theta (const MagVec < Mag > m, const Mag & M, const double * xi, const MagVec < Mag > u, const Mag & U, const long int & nxi, const long int & nm);

/**
* @brief Computation of the free energy for a perceptron-like factor graph (exact version)
*
* @tparam Mag magnetization type (MagP or MagT)
* @param m Total magnetization of variables nodes belonging to lower layer
* @param M Total magnetization of variable node belonging to upper layer
* @param xi Single input pattern
* @param u Downward messages (cavity magnetizations) from factor node to lower variables nodes
* @param U Upward message (cavity magnetizations) from factor node to upper variable node
* @param nm Number of variables node onto the lower layer
*
* @return Free energy for the system represented by a perceptron-like factor graph
*/
template < class Mag > double free_energy_theta_exact (MagVec < Mag > m, const Mag & M, const double * xi, MagVec < Mag > u, const Mag & U, const long int & nm);

/**
* @brief Extra message update rule due to replicas
*
* @tparam Mag magnetization type (MagP or MagT)
* @param m_j_star Total magnetization of a weight node
* @param m_star_j Extra message (cavity magnetizations) from replica node to its weight node
* @param params Parameters selected for the algorithm
*
* @return Largest value between older maximum difference and the difference between new and old extra message
*/
template < class Mag > double m_star_update (Mag & m_j_star, Mag & m_star_j, Params < Mag > & params);

/**
* @brief Management of the single iteration
*
* @tparam Mag magnetization type (MagP or MagT)
* @param messages All magnetizations, both total and cavity, container
* @param patterns All patterns, both input and output values, container
* @param params Parameters selected for the algorithm
*
* @return Largest difference between new and old messages across all updates
*/
template < class Mag > double iterate (Cavity_Message < Mag > & messages, const Patterns & patterns, Params < Mag > & params);

/**
* @brief Management of all iterations within protocol step (i.e. constant focusing and replica parameteres)
*
* @tparam Mag magnetization type (MagP or MagT)
* @param messages All magnetizations, both total and cavity, container
* @param patterns All patterns, both input and output values, container
* @param params Parameters selected for the algorithm
*
* @return True when convergence is reached, False otherwise
*/
template < class Mag > bool converge ( Cavity_Message < Mag > & messages, const Patterns & patterns, Params < Mag > & params);

/**
* @brief Prediction of labels given weights and input patterns
*
* @param sign_m_j_star Total magnetization of weights nodes
* @param patterns All patterns, both input and output values, container
* @param K Number of nodes onto the hidden layer
*
* @return Predicted labels
*/
long int * nonbayes_test (long int ** const sign_m_j_star, const Patterns & patterns, const long int & K);

/**
* @brief Computation of number of mistaken predicted labels
*
* @tparam Mag magnetization type (MagP or MagT)
* @param messages All magnetizations, both total and cavity, container
* @param patterns All patterns, both input and output values, container
*
* @return Number of mistaken predicted labels
*/
template < class Mag > long int error_test (const Cavity_Message < Mag > & messages, const Patterns & patterns);

/**
* @brief Computation of the free energy for the whole system
*
* @tparam Mag magnetization type (MagP or MagT)
* @param messages All magnetizations, both total and cavity, container
* @param patterns All patterns, both input and output values, container
* @param params Parameters selected for the algorithm
*
* @return Total free energy of the system
*/
template < class Mag > double free_energy (const Cavity_Message < Mag > &messages, const Patterns & patterns, const Params < Mag > & params);

#ifdef STATS

  /**
  * @brief Computation of the entropy for the whole system
  *
  * @tparam Mag magnetization type (MagP or MagT)
  * @param messages All magnetizations, both total and cavity, container
  * @param params Parameters selected for the algorithm
  *
  * @return Total entropy of the system
  */
  template < class Mag > double compute_S (const Cavity_Message < Mag > & messages, const Params < Mag > & params);

  /**
  * @brief Computation of the q order-term of the system without accounting for replicas
  *
  * @tparam Mag magnetization type (MagP or MagT)
  * @param messages All magnetizations, both total and cavity, container
  * @param params Parameters selected for the algorithm
  *
  * @return q order-term of the system without accounting for replicas
  */
  template < class Mag > double compute_q_bar (const Cavity_Message<Mag> & messages, const Params < Mag > & params);

  /**
  * @brief Computation of the q order-term of the whole system
  *
  * @tparam Mag magnetization type (MagP or MagT)
  * @param messages All magnetizations, both total and cavity, container
  * @param nm_j_star Number of nodes onto the hidden layer
  * @param nm_j_star_col Number of weights
  *
  * @return q order-term of the whole system
  */
  template < class Mag > double compute_q (const Cavity_Message < Mag > & messages, const long int & nm_j_star, const long int & nm_j_star_col);

  /**
  * @brief Visualization of weights total magnetization for each hidden layer across the whole neural network
  *
  * @tparam Mag magnetization type (MagP or MagT)
  * @param messages All magnetizations, both total and cavity, container
  * @param overlaps Empty double vector to fill up with visualization values
  *
  */
  template < class Mag > void mags_symmetry (const Cavity_Message < Mag > & messages, double * overlaps);

#endif // STATS

#if (__cplusplus < 201700) && !(__clang_major__ > 4)

  #include <type_traits>

  /**
  * @brief Set the outcome variables nodes to training labels
  *
  * @tparam Mag magnetization type (MagP or MagT)
  * @param message All magnetizations, both total and cavity, container
  * @param output Output patterns (training labels)
  * @param beta Inverse of temperature (always infinite up to now)
  *
  */
  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagP64 > :: value > :: type * = nullptr >
  void set_outfields (const Cavity_Message < Mag > & message, const long int * output, const double & beta);

  /**
  * @brief Set the outcome variables nodes to training labels
  *
  * @tparam Mag magnetization type (MagP or MagT)
  * @param message All magnetizations, both total and cavity, container
  * @param output Output patterns (training labels)
  * @param beta Inverse of temperature (always infinite up to now)
  *
  */
  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * = nullptr >
  void set_outfields (const Cavity_Message < Mag > & message, const long int * output, const double & beta);

#else

  /**
  * @brief Set the outcome variables nodes to training labels
  *
  * @tparam Mag magnetization type (MagP or MagT)
  * @param message All magnetizations, both total and cavity, container
  * @param output Output patterns (training labels)
  * @param beta Inverse of temperature (always infinite up to now)
  *
  */
  template < class Mag > void set_outfields (const Cavity_Message < Mag > & message, const long int * output, const double & beta);

#endif // __clang__

/**
* @brief Management of all protocol step of the learning rule
*
* @tparam Mag magnetization type (MagP or MagT)
* @param K Number of nodes onto the hidden layer
* @param patterns All patterns, both input and output values, container
* @param max_iters Highest number of iterations to run within same protocol step
* @param max_steps Number of protocol steps
* @param seed Seed for random generator inside Cavity_Message initial messages creator
* @param damping Damping parameter for regularization of messages updates
* @param accuracy1 Accuracy level for first layer
* @param accuracy2 Accuracy level for second layer
* @param randfact Random value used inside Cavity_Message initial messages creator
* @param fprotocol Protocol type
* @param epsil error tollerance
* @param nth Number of cores to exploit
* @param outfile Filename which evolution measurements can be stored in
* @param outmessfiletmpl Filename which final messages can be written on
* @param initmess Filename which initial messages can be taken from
* @param bin_mess True if messages filename but me read/written as binary files, text files otherwise
*
*/
template < class Mag > long int ** focusingBP (const long int & K, const Patterns & patterns, const long int & max_iters, const long int & max_steps, const long int & seed, const double & damping, const std :: string & accuracy1, const std :: string & accuracy2, const double & randfact, const FocusingProtocol & fprotocol, const double & epsil, int nth = 1, std :: string outfile = "", std :: string outmessfiletmpl = "", std :: string initmess = "", const bool & bin_mess = false);

/// @cond DEF
template < class Mag > using theta_function = double (*) (MagVec < Mag >, Mag &, const double *, MagVec < Mag >, Mag &, const Params < Mag > &, const long int &, const long int &);
/// @endcond

#if (__cplusplus < 201700) && !(__clang_major__ > 4)

  /**
  * @brief Switch case for the right accuracy function
  *
  * @tparam Mag magnetization type (MagP or MagT)
  * @param acc accuracy name (possible values are "accurate", "exact", and "none")
  *
  */
  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagP64 > :: value > :: type * = nullptr >
  theta_function < Mag > get_accuracy ( const std :: string & acc );

  template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * = nullptr >
  theta_function < Mag > get_accuracy ( const std :: string & acc );

#else

  template < class Mag > static std :: unordered_map < std :: string, theta_function < Mag > > accuracy
  {
    {"accurate", & theta_node_update_accurate},
    {"exact",    & theta_node_update_exact   },
    {"none" ,    & theta_node_update_approx  }
  };

#endif

#endif // __rfbp_h__
