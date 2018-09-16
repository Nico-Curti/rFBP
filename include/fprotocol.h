#ifndef FPROTOCOL_H
#define FPROTOCOL_H
#include <memory>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <climits>

static constexpr double inf = std::numeric_limits<double>::infinity();

struct FocusingProtocol
{
  long int Nrep;                    //
  std::unique_ptr<double[]> gamma,  // distance parameters
                            n_rep,  // (y) number of replicas
                            beta;   // 1/kT

  FocusingProtocol() : Nrep(0L), gamma(nullptr), n_rep(nullptr), beta(nullptr) {};
  ~FocusingProtocol() = default;
  void StandardReinforcement(const double *rho, const long int &Nrho);
  void StandardReinforcement(const double &drho);
  void Scoping(const double *gr, const double &x, const long int &ngr);
  void PseudoReinforcement(const double *rho, const long int &nrho, double x = .5);
  void PseudoReinforcement(const double &drho, double x = .5);
  void FreeScoping(const double **list, const long int &nlist);
};

#endif // FPROTOCOL_H
