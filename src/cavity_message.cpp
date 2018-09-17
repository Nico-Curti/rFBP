#include <cavity_message.h>

template<class Mag> Cavity_Message<Mag>::Cavity_Message(const long int &m, const long int &n, const long int &k, const double &x, const int &start)
{
  std::mt19937 engine(start);
  std::uniform_real_distribution<double> dist(0., 1.);

  this->M = m;
  this->N = n;
  this->K = k;

  this->m_star_j = new Mag* [this->K];
  this->m_j_star = new Mag* [this->K];
  this->m_in     = new Mag* [this->M];
  this->weights  = new Mag**[this->M];
  this->m_no     = new Mag* [this->M];
  this->m_on     = new Mag  [this->M];
  this->m_ni     = new Mag* [this->M];

#ifdef _OPENMP
#pragma omp for
  for (long int i = 0L; i < this->K; ++i)
  {
    this->m_star_j[i] = new Mag[this->N];
    this->m_j_star[i] = new Mag[this->N];

    for (long int j = 0L; j < this->N; ++j)
    {
      this->m_star_j[i][j] = Mag(0.);
      this->m_j_star[i][j] = Mag(0.);
    }
  }
#pragma omp for
  for (long int i = 0L; i < this->M; ++i)
  {
    this->m_in[i]    = new Mag [this->K];
    this->m_no[i]    = new Mag [this->K];
    this->m_ni[i]    = new Mag [this->K];
    this->weights[i] = new Mag*[this->K];

    this->m_on[i]    = Mag(0.);

    for (long int j = 0L; j < this->K; ++j)
    {
      this->m_in[i][j] = Mag(0.);
      this->m_no[i][j] = Mag(0.);
      this->m_ni[i][j] = Mag(0.);

      this->weights[i][j] = new Mag[this->N];
      this->m_in[i][j] = this->m_in[i][j] % this->m_no[i][j] % this->m_ni[i][j];
      std::generate_n(this->weights[i][j], this->N,
                      [&]()
                      {
                        return mag::f2m<Mag>(x * (2. * dist(engine) - 1.)); // troubles with rng
                      });
    }
  }

#else
  std::generate_n(this->m_star_j, this->K, [&n]{return new Mag [n];});
  std::generate_n(this->m_j_star, this->K, [&n]{return new Mag [n];});
  std::generate_n(this->m_in,     this->M, [&k]{return new Mag [k];});
  std::generate_n(this->m_no,     this->M, [&k]{return new Mag [k];});
  std::generate_n(this->m_ni,     this->M, [&k]{return new Mag [k];});
  std::generate_n(this->weights,  this->M, [&k]{return new Mag*[k];});

  std::generate_n(this->m_on,     this->M, []  {return Mag(0.);});

  for (long int i = 0L; i < this->K; ++i)
  {
    mag::zeros(this->m_star_j[i], this->N);
    mag::zeros(this->m_j_star[i], this->N);
  }
  for (long int i = 0L; i < this->M; ++i)
  {
    mag::zeros(this->m_in[i], this->K);
    mag::zeros(this->m_no[i], this->K);
    mag::zeros(this->m_ni[i], this->K);
    for (long int j = 0L; j < this->K; ++j)
    {
      this->weights[i][j] = new Mag[this->N];
      this->m_in[i][j] = this->m_in[i][j] % this->m_no[i][j] % this->m_ni[i][j];
      std::generate_n(this->weights[i][j], this->N,
                      [&]()
                      {
                        return mag::f2m<Mag>(x * (2. * dist(engine) - 1.));
                      });
    }
  }
#endif

#ifdef _OPENMP
#pragma omp for collapse(3)
#endif
  for (long int i = 0L; i < this->K; ++i)
   for (long int j = 0L; j < this->N; ++j)
      for (long int k = 0L; k < this->M; ++k)
        this->m_j_star[i][j] = this->m_j_star[i][j] % this->weights[k][i][j];
}


template<class Mag> Cavity_Message<Mag>::Cavity_Message(const Cavity_Message<Mag> &m)
{
  this->M = m.M;
  this->N = m.N;
  this->K = m.K;
  this->m_star_j  = new Mag* [this->K];
  this->m_j_star  = new Mag* [this->K];
  this->m_in      = new Mag* [this->M];
  this->weights   = new Mag**[this->M];
  this->m_no      = new Mag* [this->M];
  this->m_on      = new Mag  [this->M];
  this->m_ni      = new Mag* [this->M];

  std::generate_n(this->m_star_j, this->K, [this]{return new Mag [this->N];});
  std::generate_n(this->m_j_star, this->K, [this]{return new Mag [this->N];});
  std::generate_n(this->m_in,     this->M, [this]{return new Mag [this->K];});
  std::generate_n(this->m_no,     this->M, [this]{return new Mag [this->K];});
  std::generate_n(this->m_ni,     this->M, [this]{return new Mag [this->K];});
  std::generate_n(this->weights,  this->M, [this]{return new Mag*[this->K];});

  for (long int i = 0L; i < this->K; ++i)
  {
    std::copy_n(m.m_star_j[i], this->N, this->m_star_j[i]);
    std::copy_n(m.m_star_j[i], this->N, this->m_j_star[i]);
  }
  std::copy_n(m.m_on, this->M, this->m_on);
  for (long int i = 0L; i < this->M; ++i)
  {
    std::copy_n(m.m_in[i], this->K, this->m_in[i]);
    std::copy_n(m.m_no[i], this->K, this->m_no[i]);
    std::copy_n(m.m_ni[i], this->K, this->m_ni[i]);
    for(long int j = 0L; j < this->K; ++j)
    {
      this->weights[i][j] = new Mag[this->N];
      std::copy_n(m.weights[i][j], this->N, this->weights[i][j]);
    }
  }
}

template<class Mag> Cavity_Message<Mag>& Cavity_Message<Mag>::operator=(const Cavity_Message<Mag> &m)
{
  if(this->K != 0L)
  {
    for(long int i = 0L; i < this->K; ++i)
    {
      delete[] this->m_j_star[i];
      delete[] this->m_star_j[i];
    }
    delete[] this->m_j_star;
    delete[] this->m_star_j;
  }
  if(this->M != 0L)
  {
    for(long int i = 0L; i < this->M; ++i)
    {
      delete[] this->m_in[i];
      delete[] this->m_no[i];
      delete[] this->m_ni[i];
      for(long int j = 0L; j < this->K; ++j)
        delete[] this->weights[i][j];
      delete[] this->weights[i];
    }
    delete[] this->m_in;
    delete[] this->m_no;
    delete[] this->m_ni;
    delete[] this->weights;

    delete[] this->m_on;
  }
  this->M = m.M;
  this->N = m.N;
  this->K = m.K;
  this->m_star_j  = new Mag* [this->K];
  this->m_j_star  = new Mag* [this->K];
  this->m_in      = new Mag* [this->M];
  this->weights   = new Mag**[this->M];
  this->m_no      = new Mag* [this->M];
  this->m_on      = new Mag  [this->M];
  this->m_ni      = new Mag* [this->M];

  std::generate_n(this->m_star_j, this->K, [this]{return new Mag [this->N];});
  std::generate_n(this->m_j_star, this->K, [this]{return new Mag [this->N];});
  std::generate_n(this->m_in,     this->M, [this]{return new Mag [this->K];});
  std::generate_n(this->m_no,     this->M, [this]{return new Mag [this->K];});
  std::generate_n(this->m_ni,     this->M, [this]{return new Mag [this->K];});
  std::generate_n(this->weights,  this->M, [this]{return new Mag*[this->K];});

  for (long int i = 0L; i < this->K; ++i)
  {
    std::copy_n(m.m_star_j[i], this->N, this->m_star_j[i]);
    std::copy_n(m.m_star_j[i], this->N, this->m_j_star[i]);
  }
  std::copy_n(m.m_on, this->M, this->m_on);
  for (long int i = 0L; i < this->M; ++i)
  {
    std::copy_n(m.m_in[i], this->K, this->m_in[i]);
    std::copy_n(m.m_no[i], this->K, this->m_no[i]);
    std::copy_n(m.m_ni[i], this->K, this->m_ni[i]);
    for(long int j = 0L; j < this->K; ++j)
    {
      this->weights[i][j] = new Mag[this->N];
      std::copy_n(m.weights[i][j], this->N, this->weights[i][j]);
    }
  }
  return *this;
}

template<class Mag> Cavity_Message<Mag>::~Cavity_Message()
{
  if(this->K != 0L)
  {
    for(long int i = 0L; i < this->K; ++i)
    {
      delete[] this->m_j_star[i];
      delete[] this->m_star_j[i];
    }
    delete[] this->m_j_star;
    delete[] this->m_star_j;
  }
  if(this->M != 0L)
  {
    for(long int i = 0L; i < this->M; ++i)
    {
      delete[] this->m_in[i];
      delete[] this->m_no[i];
      delete[] this->m_ni[i];
      for(long int j = 0L; j < this->K; ++j)
        delete[] this->weights[i][j];
      delete[] this->weights[i];
    }
    delete[] this->m_in;
    delete[] this->m_no;
    delete[] this->m_ni;
    delete[] this->weights;

    delete[] this->m_on;
  }
}


