#ifndef RFBP_H
#define RFBP_H
#include <algorithm>
#include <numeric>

#ifdef DEBUG
#include <cassert>
#endif

#ifdef STATS
#include <iomanip>
#endif

#ifdef _MSC_VER
  #ifndef __unused
  #define __unused
  #endif
#else
  #ifndef __unused
  #define __unused __attribute__((__unused__))
  #endif
#endif
#include <cavity_message.h>
#include <fprotocol.h>
#include <pattern.h>

template < class Mag > double theta_node_update_approx (MagVec < Mag > m, Mag & M, const double * xi, MagVec < Mag > u, Mag & U, const Params < Mag > & params, const long int & nxi, const long int & nm);
template < class Mag > double theta_node_update_accurate (MagVec < Mag > m, Mag & M, const double * xi, MagVec < Mag > u, Mag & U, const Params < Mag > & params, const long int & nxi, const long int & nm);
template < class Mag > double theta_node_update_exact (MagVec < Mag > m, Mag & M, const double * xi, MagVec < Mag > u, Mag & U, const Params < Mag > & params, const long int & nxi, const long int & nm);
template < class Mag > double free_energy_theta (const MagVec < Mag > m, const Mag & M, const double * xi, const MagVec < Mag > u, const Mag & U, const long int & nxi, const long int & nm);
template < class Mag > double free_energy_theta_exact (MagVec < Mag > m, const Mag & M, const double * xi, MagVec < Mag > u, const Mag & U, const long int & nm);
template < class Mag > double m_star_update (Mag & m_j_star, Mag & m_star_j, Params < Mag > & params);
template < class Mag > double iterate (Cavity_Message < Mag > & messages, const Patterns & patterns, Params < Mag > & params);
template < class Mag > bool converge ( Cavity_Message < Mag > & messages, const Patterns & patterns, Params < Mag > & params);
long int * nonbayes_test (long int ** const sign_m_j_star, const Patterns & patterns, const long int & K);
template < class Mag > long int error_test (const Cavity_Message < Mag > & messages, const Patterns & patterns);
template < class Mag > double free_energy (const Cavity_Message < Mag > &messages, const Patterns & patterns, const Params < Mag > & params);
#ifdef STATS
template < class Mag > double compute_S (const Cavity_Message < Mag > & messages, const Params < Mag > & params);
template < class Mag > double compute_q_bar (const Cavity_Message<Mag> & messages, const Params < Mag > & params);
template < class Mag > double compute_q (const Cavity_Message < Mag > & messages, const long int & nm_j_star, const long int & nm_j_star_col);
template < class Mag > void mags_symmetry (const Cavity_Message < Mag > & messages, double * overlaps);
#endif // STATS

#if !defined __clang__ &&  __GNUC__ <= 6

#include <type_traits>
template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagP64 > :: value > :: type * = nullptr >
void set_outfields (const Cavity_Message < Mag > & message, const long int * output, const double & beta);

template < class Mag, typename std :: enable_if < std :: is_same < Mag, MagT64 > :: value > :: type * = nullptr >
void set_outfields (const Cavity_Message < Mag > & message, const long int * output, const double & beta);

#else

template < class Mag > void set_outfields (const Cavity_Message < Mag > & message, const long int * output, const double & beta);

#endif // __clang__

template < class Mag > long int ** focusingBP (const long int & K, const Patterns & patterns, const long int & max_iters, const long int & max_steps, const long int & seed, const double & damping, const std :: string & accuracy1, const std :: string & accuracy2, const double & randfact, const FocusingProtocol & fprotocol, const double & epsil, int nth = 1, std :: string outfile = "", std :: string outmessfiletmpl = "", std :: string initmess = "", const bool & bin_mess = false);

template < class Mag > using theta_function = double (*) (MagVec < Mag >, Mag &, const double *, MagVec < Mag >, Mag &, const Params < Mag > &, const long int &, const long int &);

#if !defined __clang__ &&  __GNUC__ <= 6

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

#endif // RFBP_H
