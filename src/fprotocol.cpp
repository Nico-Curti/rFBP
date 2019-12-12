#include <fprotocol.h>

FocusingProtocol :: FocusingProtocol () : Nrep (0L), gamma (nullptr), n_rep (nullptr), beta (nullptr)
{
}

FocusingProtocol :: FocusingProtocol (const std :: string & prot, const long int & size)
{

#ifdef DEBUG

  assert (size > 1);

#endif

  const long int p = protocol.at(prot);
  double step = 1. / (size - 1.);

  this->Nrep  = size;
  this->gamma = std :: make_unique < double[] >(this->Nrep);
  this->beta  = std :: make_unique < double[] >(this->Nrep);
  this->n_rep = std :: make_unique < double[] >(this->Nrep);

  switch ( p )
  {
    case scoping_:
    {
      step = 7. / (size - 1L); // 7 is a magic number ??
      long int i = 0;
      std :: generate_n(this->gamma.get(), size - 1L, [&](){ return (i++) * step;});
      std :: fill_n(this->n_rep.get(), size, 21.); // 21 is magic number ??
      std :: fill_n(this->beta.get(), size, INF);

      this->gamma[size - 1L] = INF;

    } break;

    case pseudo_:
    {

      for (long int i = 0L; i < size; ++i)
      {
        this->gamma[i] = std :: atanh(std :: sqrt(i * step));  // std :: atanh(std :: pow(i * drho, .5));
        this->n_rep[i] = 1. + i * step / (1. - i * step);      // 1. + std :: pow(drho * i, 1. - 2. * x) / (1. - drho * i);
        this->beta[i]  = INF;
      }

      this->n_rep[size - 1L] = INF;

    } break;

    case free_scoping_:
    {

      for (long int i = 0L; i < size; ++i)
      {
        this->gamma[i] = 1. / (1. - i * step);
        this->n_rep[i] = (2. - i * step) / (1. - i * step);
        this->beta[i]  = INF;
      }

      this->n_rep[size - 1L] = INF;
      this->gamma[size - 1L] = INF;

    } break;

    case standard_:
    {

      std :: fill_n(this->gamma.get(), this->Nrep, INF);
      for (long int i = 0L; i < this->Nrep; ++i)
        this->n_rep[i] = 1. / (1. - i * step);
      std :: fill_n(this->beta.get(), this->Nrep, INF);

      this->n_rep[size - 1L] = INF;

    } break;

    default: error_protocol(prot);
    break;
  }
}

void FocusingProtocol :: StandardReinforcement (const double * rho, const long int & Nrho)
{
  std :: fill_n(this->gamma.get(), Nrho, INF);
  std :: transform(rho, rho + Nrho, this->n_rep.get(), [](const double & i){ return 1. / (1L - i);});
  std :: fill_n(this->beta.get(), Nrho, INF);
  return;
}

void FocusingProtocol :: StandardReinforcement (const double & drho)
{
  std :: fill_n(this->gamma.get(), this->Nrep, INF);
  for (long int i = 0L; i < this->Nrep; ++i) this->n_rep[i] = 1. / (1L - (drho * i));
  std :: fill_n(this->beta.get(), this->Nrep, INF);
  return;
}

void FocusingProtocol :: Scoping (const double * gr, const double & x, const long int & ngr)
{
  std :: copy_n(gr,                ngr, this->gamma.get());
  std :: fill_n(this->n_rep.get(), ngr, x);
  std :: fill_n(this->beta.get(),  ngr, INF);
  return;
}


void FocusingProtocol :: PseudoReinforcement (const double * rho, const long int & nrho, double x)
{
  std :: transform(rho, rho + nrho, this->gamma.get(), [&](const double & i){return std :: atanh(std :: pow(i, x));});
  std :: transform(rho, rho + nrho, this->n_rep.get(), [&](const double & i){return 1. + std :: pow(i, 1. - 2. * x) / (1. - i);});
  std :: fill_n(this->beta.get(), this->Nrep, INF);
  return;
}

void FocusingProtocol :: PseudoReinforcement(const double & drho, double x)
{
  for (long int i = 0L; i < this->Nrep; ++i)
  {
    this->gamma[i] = std :: atanh(std :: pow(drho * i, x));
    this->n_rep[i] = 1. + std :: pow(drho * i, 1. - 2. * x) / (1. - drho * i);
  }
  std :: fill_n(this->beta.get(), this->Nrep, INF);
  return;
}


void FocusingProtocol :: FreeScoping(double ** list, const long int & nlist)
{
  for (long int i = 0L; i < nlist; ++i)
  {
    this->gamma[i] = list[i][0];
    this->n_rep[i] = list[i][1];
    this->beta[i]  = list[i][2];
  }
  return;
}
