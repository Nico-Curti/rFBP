#include <fprotocol.h>

void FocusingProtocol::StandardReinforcement(const double *rho, const long int &Nrho)
{
  this->Nrep  = Nrho;
  this->gamma = std::make_unique<double[]>(Nrho);
  this->n_rep = std::make_unique<double[]>(Nrho);
  this->beta  = std::make_unique<double[]>(Nrho);
  std::fill_n(this->gamma.get(), Nrho, inf);
  std::transform(rho, rho + Nrho, this->n_rep.get(), [](const double &i){ return 1. / (1. - i);});
  std::fill_n(this->beta.get(), Nrho, inf);
  return;
}

void FocusingProtocol::StandardReinforcement(const double &drho)
{
  this->Nrep = static_cast<int>((1. - drho) / drho);
  this->gamma = std::make_unique<double[]>(this->Nrep);
  this->beta  = std::make_unique<double[]>(this->Nrep);
  this->n_rep = std::make_unique<double[]>(this->Nrep);
  std::fill_n(this->gamma.get(),     this->Nrep, inf);
  for (long int i = 0L; i < this->Nrep; ++i) this->n_rep[i] = 1. / (1. - (drho * i));
  std::fill_n(this->beta.get(),      this->Nrep, inf);
  return;
}

void FocusingProtocol::Scoping(const double *gr, const double &x, const long int &ngr)
{
  this->Nrep  = ngr;
  this->gamma = std::make_unique<double[]>(this->Nrep);
  this->beta  = std::make_unique<double[]>(this->Nrep);
  this->n_rep = std::make_unique<double[]>(this->Nrep);
  std::copy_n(gr,                ngr, this->gamma.get());
  std::fill_n(this->n_rep.get(), ngr, x);
  std::fill_n(this->beta.get(),  ngr, inf);
  return;
}


void FocusingProtocol::PseudoReinforcement(const double *rho, const long int &nrho, double x)
{
  this->Nrep  = nrho;
  this->gamma = std::make_unique<double[]>(this->Nrep);
  this->beta  = std::make_unique<double[]>(this->Nrep);
  this->n_rep = std::make_unique<double[]>(this->Nrep);
  std::transform(rho, rho + nrho, this->gamma.get(), [&x](const double &i){return std::atanh(std::pow(i, x));});
  std::transform(rho, rho + nrho, this->n_rep.get(), [&x](const double &i){return 1. + std::pow(i, 1. - 2. * x) / (1. - i);});
  std::fill_n(this->beta.get(), this->Nrep, inf);
  return;
}

void FocusingProtocol::PseudoReinforcement(const double &drho, double x)
{
  this->Nrep  = static_cast<long int>((1. - drho) / drho);
  this->gamma = std::make_unique<double[]>(this->Nrep);
  this->n_rep = std::make_unique<double[]>(this->Nrep);
  this->beta  = std::make_unique<double[]>(this->Nrep);

  for (long int i = 0L; i < this->Nrep; ++i)
  {
    this->gamma[i] = std::atanh(std::pow(drho * i, x));
    this->n_rep[i] = 1. + std::pow(drho * i, 1. - 2. * x) / (1. - drho * i);
  }
  std::fill_n(this->beta.get(), this->Nrep, inf);
  return;
}


void FocusingProtocol::FreeScoping(const double **list, const long int &nlist)
{
  this->Nrep  = nlist;
  this->gamma = std::make_unique<double[]>(this->Nrep);
  this->n_rep = std::make_unique<double[]>(this->Nrep);
  this->beta  = std::make_unique<double[]>(this->Nrep);
  for (long int i = 0L; i < nlist; ++i)
  {
    this->gamma[i] = list[i][0];
    this->n_rep[i] = list[i][1];
    this->beta[i]  = list[i][2];
  }
  return;
}



