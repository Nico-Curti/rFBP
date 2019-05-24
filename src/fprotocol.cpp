#include <fprotocol.h>

FocusingProtocol :: FocusingProtocol () : Nrep(0L), gamma(nullptr), n_rep(nullptr), beta(nullptr)
{
}

FocusingProtocol :: FocusingProtocol (const std :: string & prot, const long int & size)
{
  long int p = protocol.at(prot),
           i = 0;
  switch ( p )
  {
    case scoping_:
    {
      std :: unique_ptr < double[] > scop(new double[size]);
      std :: generate_n(scop.get(), size, [&](){ return (i++) * 1.;});
      Scoping(scop.get(), 21L, size);
    } break;
    case pseudo_:
    {
      std :: unique_ptr < double[] > pseudo(new double[size]);
      std :: generate_n(pseudo.get(), size, [&](){ return (i++) * 1e-2;});
      PseudoReinforcement(pseudo.get(), size);
    } break;
    case free_scoping_:
    {
      double **freeS = new double*[size];
      std :: generate_n(freeS, size, [](){return new double[3];});
      for (long int i = 0L; i < size; ++i)
      {
        // freeS[i][0] = std::atanh(std::sqrt(i * 1e-3));
        freeS[i][0] = 1. / (1. - i * 1e-2);
        freeS[i][1] = (2. - i * 1e-2) / (1. - i * 1e-2);
        freeS[i][2] = INF;
      }
      FreeScoping(freeS, size);
      for (long int i = 0L; i < size; ++i) delete[] freeS[i];
      delete[] freeS;
    } break;
    case standard_:
    {
      StandardReinforcement(1e-2);
    } break;
    default: error_protocol(prot);
    break;
  }
}

void FocusingProtocol :: StandardReinforcement (const double * rho, const long int & Nrho)
{
  this->Nrep  = Nrho;
  this->gamma = std :: make_unique < double[] >(Nrho);
  this->n_rep = std :: make_unique < double[] >(Nrho);
  this->beta  = std :: make_unique < double[] >(Nrho);
  std :: fill_n(this->gamma.get(), Nrho, INF);
  std :: transform(rho, rho + Nrho, this->n_rep.get(), [](const double & i){ return 1. / (1. - i);});
  std :: fill_n(this->beta.get(), Nrho, INF);
  return;
}

void FocusingProtocol :: StandardReinforcement (const double & drho)
{
  this->Nrep  = static_cast < int >((1. - drho) / drho);
  this->gamma = std :: make_unique < double[] >(this->Nrep);
  this->beta  = std :: make_unique < double[] >(this->Nrep);
  this->n_rep = std :: make_unique < double[] >(this->Nrep);
  std :: fill_n(this->gamma.get(), this->Nrep, INF);
  for (long int i = 0L; i < this->Nrep; ++i) this->n_rep[i] = 1. / (1. - (drho * i));
  std :: fill_n(this->beta.get(), this->Nrep, INF);
  return;
}

void FocusingProtocol :: Scoping (const double * gr, const double & x, const long int & ngr)
{
  this->Nrep  = ngr;
  this->gamma = std :: make_unique < double[] >(this->Nrep);
  this->beta  = std :: make_unique < double[] >(this->Nrep);
  this->n_rep = std :: make_unique < double[] >(this->Nrep);
  std :: copy_n(gr,                ngr, this->gamma.get());
  std :: fill_n(this->n_rep.get(), ngr, x);
  std :: fill_n(this->beta.get(),  ngr, INF);
  return;
}


void FocusingProtocol :: PseudoReinforcement (const double * rho, const long int & nrho, double x)
{
  this->Nrep  = nrho;
  this->gamma = std :: make_unique < double[] >(this->Nrep);
  this->beta  = std :: make_unique < double[] >(this->Nrep);
  this->n_rep = std :: make_unique < double[] >(this->Nrep);
  std :: transform(rho, rho + nrho, this->gamma.get(), [&x](const double & i){return std :: atanh(std :: pow(i, x));});
  std :: transform(rho, rho + nrho, this->n_rep.get(), [&x](const double & i){return 1. + std :: pow(i, 1. - 2. * x) / (1. - i);});
  std :: fill_n(this->beta.get(), this->Nrep, INF);
  return;
}

void FocusingProtocol :: PseudoReinforcement(const double & drho, double x)
{
  this->Nrep  = static_cast < long int >((1. - drho) / drho);
  this->gamma = std :: make_unique < double[] >(this->Nrep);
  this->n_rep = std :: make_unique < double[] >(this->Nrep);
  this->beta  = std :: make_unique < double[] >(this->Nrep);

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
  this->Nrep  = nlist;
  this->gamma = std :: make_unique < double[] >(this->Nrep);
  this->n_rep = std :: make_unique < double[] >(this->Nrep);
  this->beta  = std :: make_unique < double[] >(this->Nrep);
  for (long int i = 0L; i < nlist; ++i)
  {
    this->gamma[i] = list[i][0];
    this->n_rep[i] = list[i][1];
    this->beta[i]  = list[i][2];
  }
  return;
}
