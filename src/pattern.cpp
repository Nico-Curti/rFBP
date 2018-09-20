#include <pattern.h>

#ifdef DEBUG
void Patterns::check_binary()
{
  assert(this->Nout == this->Nrow);
  int cnt = 0;
  for (long int i = 0L; i < this->Nrow; ++i)
    cnt += std::count_if(this->input[i], this->input[i] + this->Ncol,
                         [](const double &v)
                         {
                          return std::abs(v);
                         });
  assert(cnt == this->Nrow * this->Ncol);
  cnt = std::accumulate(this->output.get(), this->output.get() + this->Nout,
                        0, [](const int &res, const long int &v)
                        {
                          return res + static_cast<int>(std::abs(v));
                        });
  assert(cnt == this->Nout);
}
#endif

Patterns::Patterns(const std::string &filename, bool bin)
{
  if (bin)
  {
    std::ifstream is(filename, std::ios::binary);
    if (!is) error_pattern(filename);
    is.read((char*)&this->Nrow, sizeof(long int));
    is.read((char*)&this->Ncol, sizeof(long int));
    this->Nout   = this->Nrow;
    this->input  = new double*[this->Nrow];
    this->output = std::make_unique<long int[]>(this->Nout);
    for (long int i = 0L; i < this->Nout; ++i)
      is.read((char*)&this->output[i], sizeof(long int));

    std::generate_n(this->input, this->Nrow, [&](){return new double[this->Ncol];});
    for (long int i = 0L; i < this->Nrow; ++i)
      for (long int j = 0L; j < this->Ncol; ++j)
        is.read((char*)&this->input[i][j], sizeof(double));
    is.close();
  }
  else
  {
    std::ifstream is(filename);
    if (!is) error_pattern(filename);
    std::stringstream buff;
    buff << is.rdbuf();
    is.close();

    std::string row;
    std::getline(buff, row);
    this->Nout = std::count(row.begin(), row.end(), ' ') + 1;
    this->Nrow = this->Nout;

    std::getline(buff, row);
    this->Ncol = std::count(row.begin(), row.end(), ' ') + 1;

    buff.clear();
    buff.seekg(0, std::ios::beg);

    this->output = std::make_unique<long int[]>(this->Ncol);
    for (int i = 0; i < this->Ncol; ++i)
      buff >> this->output[i];

    this->input = new double*[this->Nrow];
    std::generate_n(this->input, this->Nrow, [&](){return new double[this->Ncol];});
    for (long int i = 0L; i < this->Nrow; ++i)
      for (long int j = 0L; j < this->Ncol; ++j)
        buff >> this->input[i][j];
  }

#ifdef DEBUG
  check_binary();
#endif
}


Patterns::Patterns(const long int &N, const long int &M)
{
  this->Nrow = N;
  this->input  = new double*[this->Nrow];
  this->output = std::make_unique<long int[]>(this->Nrow);
  this->Ncol = M;
  this->Nout = this->Nrow;

  std::default_random_engine engine;
  std::bernoulli_distribution dist(.5);
  std::fill_n(this->output.get(), this->Nout, 1L);
  std::generate_n(this->input, this->Nrow, [&](){return new double[this->Ncol];});
  for (long int i = 0L; i < this->Nrow; ++i)
    std::generate_n(this->input[i], this->Ncol,
                    [&]()
                    {
                      return (dist(engine)) ? 1. : -1.;
                    });
}

Patterns::~Patterns()
{
  if(this->Nrow)
  {
     for(long int i = 0L; i < this->Nrow; ++i) delete[] this->input[i];
      delete[] this->input;
  }
}

