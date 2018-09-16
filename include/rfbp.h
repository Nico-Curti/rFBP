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

namespace RFBP
{
  template<class Mag> double theta_node_update_approx(MagVec<Mag> m, Mag &M, double *xi, MagVec<Mag> u, Mag &U, const Params<Mag> &params, const int &nxi, const int &nm);
  template<class Mag> double theta_node_update_accurate(MagVec<Mag> m, Mag &M, const double *xi, MagVec<Mag> u, Mag &U, const Params<Mag> &params, const int &nxi, const int &nm);


}

#endif // RFBP_H
