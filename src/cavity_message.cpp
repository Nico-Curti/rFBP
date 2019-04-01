#include <cavity_message.h>

template<class Mag> Cavity_Message<Mag>::Cavity_Message(const std::string &filename, const bool &bin)
{
  std::ifstream is;
  if (bin)
  {
    is.open(filename, std::ios::binary);
    if ( !is ) error_messages(filename);

    is.read((char*) &this->N, sizeof(long int));
    is.read((char*) &this->M, sizeof(long int));
    is.read((char*) &this->K, sizeof(long int));

    this->m_star_j = new Mag* [this->K];
    this->m_j_star = new Mag* [this->K];
    this->m_in     = new Mag* [this->M];
    this->weights  = new Mag**[this->M];
    this->m_no     = new Mag* [this->M];
    this->m_on     = new Mag  [this->M];
    this->m_ni     = new Mag* [this->M];

    for (long int i = 0L; i < this->K; ++i)
    {
      this->m_star_j[i] = new Mag[this->N];
      for (long int j = 0L; j < this->N; ++j)
        is.read( (char *) &this->m_star_j[i][j].mag, sizeof(double));
    }

    for (long int i = 0L; i < this->K; ++i)
    {
      this->m_j_star[i] = new Mag[this->N];
      for (long int j = 0L; j < this->N; ++j)
        is.read( (char *) &this->m_j_star[i][j].mag, sizeof(double));
    }

    for (long int i = 0L; i < this->M; ++i)
    {
      this->m_in[i]    = new Mag [this->K];
      for (long int j = 0L; j < this->K; ++j)
        is.read( (char *) &this->m_in[i][j].mag, sizeof(double));
    }

    for (long int i = 0L; i < this->M; ++i)
    {
      this->weights[i] = new Mag*[this->K];
      for (long int j = 0L; j < this->K; ++j)
      {
        this->weights[i][j] = new Mag[this->N];
        for (long int k = 0L; k < this->N; ++k)
          is.read( (char *) &this->weights[i][j][k].mag, sizeof(double));
      }
    }

    for (long int i = 0L; i < this->M; ++i)
    {
      this->m_no[i]    = new Mag [this->K];
      for (long int j = 0L; j < this->K; ++j)
        is.read( (char *) &this->m_no[i][j].mag, sizeof(double));
    }

    for (long int i = 0L; i < this->M; ++i)
      is.read( (char *) &this->m_on[i].mag, sizeof(double));

    for (long int i = 0L; i < this->M; ++i)
    {
      this->m_ni[i]    = new Mag [this->K];
      for (long int j = 0L; j < this->K; ++j)
        is.read( (char *) &this->m_ni[i][j].mag, sizeof(double));
    }
    is.close();
  }
  else
  {
    is.open(filename);
    std::stringstream buff;
    if ( !is ) error_messages(filename);
    std::string row;
    buff << is.rdbuf();
    is.close();

    buff >> row;
    if ( row != "fmt:") error_invalid_messages(filename);

    buff >> row;
    buff >> row;
    if ( row != "N,M,K:") error_invalid_messages(filename);

    buff >> this->N;
    buff >> this->M;
    buff >> this->K;

    this->m_star_j = new Mag* [this->K];
    this->m_j_star = new Mag* [this->K];
    this->m_in     = new Mag* [this->M];
    this->weights  = new Mag**[this->M];
    this->m_no     = new Mag* [this->M];
    this->m_on     = new Mag  [this->M];
    this->m_ni     = new Mag* [this->M];

    for (long int i = 0L; i < this->K; ++i)
    {
      this->m_star_j[i] = new Mag[this->N];
      for (long int j = 0L; j < this->N; ++j)
        buff >> this->m_star_j[i][j].mag;
    }

    for (long int i = 0L; i < this->K; ++i)
    {
      this->m_j_star[i] = new Mag[this->N];
      for (long int j = 0L; j < this->N; ++j)
        buff >> this->m_j_star[i][j].mag;
    }

    for (long int i = 0L; i < this->M; ++i)
    {
      this->m_in[i]    = new Mag [this->K];
      for (long int j = 0L; j < this->K; ++j)
        buff >> this->m_in[i][j].mag;
    }

    for (long int i = 0L; i < this->M; ++i)
    {
      this->weights[i] = new Mag*[this->K];
      for (long int j = 0L; j < this->K; ++j)
      {
        this->weights[i][j] = new Mag[this->N];
        for (long int k = 0L; k < this->N; ++k)
          buff >> this->weights[i][j][k].mag;
      }
    }

    for (long int i = 0L; i < this->M; ++i)
    {
      this->m_no[i]    = new Mag [this->K];
      for (long int j = 0L; j < this->K; ++j)
        buff >> this->m_no[i][j].mag;
    }

    for (long int i = 0L; i < this->M; ++i)
      buff >> this->m_on[i].mag;

    for (long int i = 0L; i < this->M; ++i)
    {
      this->m_ni[i]    = new Mag [this->K];
      for (long int j = 0L; j < this->K; ++j)
        buff >> this->m_ni[i][j].mag;
    }

  }
}

template<class Mag> Cavity_Message<Mag>::Cavity_Message(const long int &m, const long int &n, const long int &k, const double &x, const int &start)
{
  std::mt19937 engine(start);
  std::uniform_real_distribution<double> dist(0., 1.);

  this->N = n;
  this->M = m;
  this->K = k;

  this->m_star_j = new Mag* [this->K];
  this->m_j_star = new Mag* [this->K];
  this->m_in     = new Mag* [this->M];
  this->weights  = new Mag**[this->M];
  this->m_no     = new Mag* [this->M];
  this->m_on     = new Mag  [this->M];
  this->m_ni     = new Mag* [this->M];

// #ifdef _OPENMP
// #pragma omp for
//   for (long int i = 0L; i < this->K; ++i)
//   {
//     this->m_star_j[i] = new Mag[this->N];
//     this->m_j_star[i] = new Mag[this->N];
//
//     for (long int j = 0L; j < this->N; ++j)
//     {
//       this->m_star_j[i][j] = Mag(0.);
//       this->m_j_star[i][j] = Mag(0.);
//     }
//   }
// #pragma omp for
//   for (long int i = 0L; i < this->M; ++i)
//   {
//     this->m_in[i]    = new Mag [this->K];
//     this->m_no[i]    = new Mag [this->K];
//     this->m_ni[i]    = new Mag [this->K];
//     this->weights[i] = new Mag*[this->K];
//
//     this->m_on[i]    = Mag(0.);
//
//     for (long int j = 0L; j < this->K; ++j)
//     {
//       this->m_in[i][j] = Mag(0.);
//       this->m_no[i][j] = Mag(0.);
//       this->m_ni[i][j] = Mag(0.);
//
//       this->weights[i][j] = new Mag[this->N];
//       this->m_in[i][j] = this->m_in[i][j] % this->m_no[i][j] % this->m_ni[i][j]; // always 0 ?
//       std::generate_n(this->weights[i][j], this->N,
//                       [&]()
//                       {
//                         return mag::f2m<Mag>(x * (2. * dist(engine) - 1.)); // troubles with rng
//                       });
//     }
//   }
//
// #else
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
// #endif

// #ifdef _OPENMP
// #pragma omp for collapse(3)
// #endif
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
    std::copy_n(m.m_j_star[i], this->N, this->m_j_star[i]); // CHECK IF THIS IS OK
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

template<class Mag> long int** Cavity_Message<Mag>::get_weights()
{
  static long int **sign_m_j_star;
  sign_m_j_star = new long int*[this->K];
  std::generate_n(sign_m_j_star, this->K, [&](){return new long int[this->N];});

#ifdef _OPENMP
#pragma omp barrier
#pragma omp for collapse(2)
#endif
  for (long int i = 0L; i < this->K; ++i)
    for (long int j = 0L; j < this->N; ++j)
      sign_m_j_star[i][j] = 1L - 2L * static_cast<long int>(mag::signbit(this->m_j_star[i][j]));

  return sign_m_j_star;
}

template<class Mag> void Cavity_Message<Mag>::save_weights(const std::string &filename, Params<Mag> &parameters)
{
  std::ofstream os(filename);
  os << "fmt: "         << parameters.tan_gamma.magformat()
     << ", max_iters: " << parameters.max_iters
     << ", damping: "   << parameters.damping
     << ", epsil: "     << parameters.epsil
     << ", replicas: "  << parameters.r
     << ", accuracy1: " << parameters.accuracy1
     << ", accuracy2: " << parameters.accuracy2
     << std::endl
     << "K,N: " << this->K << " " << this->N
     << std::endl;

  os.precision(6);
  os.setf( std::ios::fixed, std:: ios::floatfield );
  for (long int i = 0L; i < this->K; ++i)
  {
    std::copy_n(this->m_j_star[i], this->N, std::ostream_iterator<Mag>(os, " "));
    os << std::endl;
  }
  os.close();
}

template<class Mag> void Cavity_Message<Mag>::save_weights(const std::string &filename)
{
  std::ofstream os(filename, std::ios::out | std::ios::binary);
  os.write( (const char *) &this->K, sizeof( long int ));
  os.write( (const char *) &this->N, sizeof( long int ));
  for(long int i = 0L; i < this->K; ++i)
    for(long int j = 0L; j < this->N; ++j)
      os.write( (const char *) &this->m_j_star[i][j].mag, sizeof( double ));
  os.close();
}

template<class Mag> void Cavity_Message<Mag>::read_weights(const std::string &filename, const bool &bin)
{
  long int k, n;
  std::ifstream is;
  if(bin)
  {
    is.open(filename, std::ios::binary);
    if( !is ) error_message_weights(filename);
    is.read((char*)&k, sizeof(long int));
    is.read((char*)&n, sizeof(long int));

#ifdef DEBUG
    assert(this->K == k);
    assert(this->N == n);
#endif
    for (long int i = 0L; i < this->K; ++i)
      for (long int j = 0L; j < this->N; ++j)
        is.read( (char *) &this->m_j_star[i][j].mag, sizeof(double));
    is.close();
  }
  else
  {
    std::string row;
    is.open(filename);
    if(!is) error_message_weights(filename);
    std::stringstream buff;
    buff << is.rdbuf();
    is.close();

    std::getline(buff, row); // fmt:
    buff >> row;
    buff >> k;
    buff >> n;
#ifdef DEBUG
    assert(this->K == k);
    assert(this->N == n);
#endif
    for (long int i = 0L; i < this->K; ++i)
      for (long int j = 0L; j < this->N; ++j)
        buff >> this->m_j_star[i][j].mag;
  }

}

template<class Mag> void Cavity_Message<Mag>::save_messages(const std::string &filename, Params<Mag> &parameters)
{
   std::ofstream os(filename);
   os << "fmt: "   << parameters.tan_gamma.magformat()
      << std::endl
      << "N,M,K: " << this->N << " " << this->M << " " << this->K
      << std::endl;

  os.precision(5);
  os.setf( std::ios::fixed, std:: ios::floatfield );
  for (long int i = 0L; i < this->K; ++i)
  {
    std::copy_n(this->m_star_j[i], this->N, std::ostream_iterator<Mag>(os, " "));
    os << std::endl;
  }

  for (long int i = 0L; i < this->K; ++i)
  {
    std::copy_n(this->m_j_star[i], this->N, std::ostream_iterator<Mag>(os, " "));
    os << std::endl;
  }

  for (long int i = 0L; i < this->M; ++i)
  {
    std::copy_n(this->m_in[i], this->K, std::ostream_iterator<Mag>(os, " "));
    os << std::endl;
  }

  for (long int i = 0L; i < this->M; ++i)
  {
    for(long int j = 0L; j < this->K; ++j)
    {
      std::copy_n(this->weights[i][j], this->N, std::ostream_iterator<Mag>(os, " "));
      os << std::endl;
    }
  }

  for (long int i = 0L; i < this->M; ++i)
  {
    std::copy_n(this->m_no[i], this->K, std::ostream_iterator<Mag>(os, " "));
    os << std::endl;
  }

  std::copy_n(this->m_on, this->M, std::ostream_iterator<Mag>(os, " "));
  os << std::endl;

  for (long int i = 0L; i < this->M; ++i)
  {
    std::copy_n(this->m_ni[i], this->K, std::ostream_iterator<Mag>(os, " "));
    os << std::endl;
  }

  os.close();
}

template<class Mag> void Cavity_Message<Mag>::save_messages(const std::string &filename)
{ // WHAT ABOUT FMT???
  std::ofstream os(filename, std::ios::out | std::ios::binary);
  os.write( (const char *) &this->N, sizeof( long int ));
  os.write( (const char *) &this->M, sizeof( long int ));
  os.write( (const char *) &this->K, sizeof( long int ));

  for(long int i = 0L; i < this->K; ++i)
    for(long int j = 0L; j < this->N; ++j)
      os.write( (const char *) &this->m_j_star[i][j].mag, sizeof( double ));

  for (long int i = 0L; i < this->K; ++i)
    for(long int j = 0L; j < this->N; ++j)
      os.write( (const char *) &this->m_star_j[i][j].mag, sizeof( double ));

  for (long int i = 0L; i < this->K; ++i)
    for(long int j = 0L; j < this->N; ++j)
      os.write( (const char *) &this->m_j_star[i][j].mag, sizeof( double ));

  for (long int i = 0L; i < this->M; ++i)
    for(long int j = 0L; j < this->K; ++j)
      os.write( (const char *) &this->m_in[i][j].mag, sizeof( double ));

  for (long int i = 0L; i < this->M; ++i)
    for(long int j = 0L; j < this->K; ++j)
      for(long int k = 0L; k < this->N; ++k)
        os.write( (const char *) &this->weights[i][j][k].mag, sizeof( double ));

  for (long int i = 0L; i < this->M; ++i)
    for(long int j = 0L; j < this->K; ++j)
      os.write( (const char *) &this->m_no[i][j].mag, sizeof( double ));

  for (long int i = 0L; i < this->M; ++i)
    os.write( (const char *) &this->m_on[i].mag, sizeof( double ));

  for (long int i = 0L; i < this->M; ++i)
    for(long int j = 0L; j < this->K; ++j)
      os.write( (const char *) &this->m_ni[i][j].mag, sizeof( double ));

  os.close();
}

template struct Cavity_Message<MagP64>;
template struct Cavity_Message<MagT64>;
