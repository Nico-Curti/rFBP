#ifndef FPROTOCOL_H
#define FPROTOCOL_H
#include <memory>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <climits>
#include <utils.h>
#include <errors.h>

struct FocusingProtocol
{
  long int Nrep;                    //
  std::unique_ptr<double[]> gamma,  // distance parameters
                            n_rep,  // (y) number of replicas
                            beta;   // 1/kT

  FocusingProtocol() : Nrep(0L), gamma(nullptr), n_rep(nullptr), beta(nullptr) {};
  FocusingProtocol(const std::string &prot, const long int &size);
  ~FocusingProtocol() = default;

private:
  void StandardReinforcement(const double *rho, const long int &Nrho);
  void StandardReinforcement(const double &drho);
  void Scoping(const double *gr, const double &x, const long int &ngr);
  void PseudoReinforcement(const double *rho, const long int &nrho, double x = .5);
  void PseudoReinforcement(const double &drho, double x = .5);
  void FreeScoping(double **list, const long int &nlist);
};

#endif // FPROTOCOL_H
