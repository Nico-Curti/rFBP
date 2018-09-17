#ifndef RFBP_H
#define RFBP_H
#include <algorithm>
#include <numeric>
#include <climits>

#ifdef DEBUG
#include <cassert>
#endif

#include <magnetization>

static constexpr double epsilon     = std::numeric_limits<double>::epsilon();
static constexpr double log2_over_2 = 0.34657359027997264311;

template<class Mag> double theta_node_update_approx(MagVec<Mag> m, Mag &M, const double *xi, MagVec<Mag> u, Mag &U, const Params<Mag> &params, const long int &nxi, const long int &nm);
template<class Mag> double theta_node_update_accurate(MagVec<Mag> m, Mag &M, const double *xi, MagVec<Mag> u, Mag &U, const Params<Mag> &params, const long int &nxi, const long int &nm);
template<class Mag> double free_energy_theta(const MagVec<Mag> m, const Mag &M, const double *xi, const MagVec<Mag> u, const Mag &U, const long int &nxi, const long int &nm);
template<class Mag> double free_energy_theta_exact(MagVec<Mag> m, const Mag &M, const double *xi, MagVec<Mag> u, const Mag &U, const long int &nm);
template<class Mag> double m_star_update(Mag &m_j_star, Mag &m_star_j, Params<Mag> &params);
template<class Mag> double iterate(Cavity_Message<Mag> &messages, const Patterns &patterns, const Params<Mag> &params);
template<class Mag> bool converge( Cavity_Message<Mag> &messages, const Patterns &patterns, const Params<Mag> &params);
template<class Mag> long int nonbayes_test(const Cavity_Message<Mag> &messages, const Patterns &patterns);
template<class Mag> double free_energy(const Cavity_Message<Mag> &messages, const Patterns &patterns, const Params<Mag> &params);
#ifdef STATS
template<class Mag> double compute_S(const Cavity_Message<Mag> &messages, const Params<Mag> &params);
template<class Mag> Mag compute_q_bar(const Cavity_Message<Mag> &messages, const Params<Mag> &params);
template<class Mag> double compute_q(const Cavity_Message<Mag> &messages, const long int &nm_j_star, const long int &nm_j_star_col);
#endif // STATS
template<class Mag> inline void set_outfields(const Cavity_Message<Mag> &message, const long int *output, const double &beta);
template<class Mag> auto focusingBP(const long int &K, const Patterns &patterns, const long int &max_iters, const long int &max_steps, const long int &seed, const double &damping, const std::string &accuracy1, const std::string &accuracy2, const double &randfact, const FocusingProtocol &fprotocol, const double &epsil, const std::string &initmess, const bool &outatzero, const std::string &writeoutfile, const std::string &outfile, const std::string &outmessfiletmpl, const std::string &output);
#endif // RFBP_H
