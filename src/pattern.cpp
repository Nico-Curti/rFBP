#include <pattern.h>

Patterns :: Patterns () : Nrow(0L), Ncol(0L), Nout(0L), output(nullptr), input(nullptr)
{
}

Patterns :: Patterns (const std :: string & filename, bool bin, const std :: string & del)
{
  if (bin)
  {
    std::ifstream is(filename, std::ios::binary);
    if (!is) error_pattern(filename);
    is.read(reinterpret_cast <char * >(&this->Nrow), sizeof(long int));
    is.read(reinterpret_cast <char * >(&this->Ncol), sizeof(long int));
    this->Nout   = this->Nrow;
    this->input  = new double*[this->Nrow];
    this->output = new long int[this->Nout];

    is.read(reinterpret_cast < char * >(this->output), sizeof(long int) * this->Nout);

    std::generate_n(this->input, this->Nrow, [&](){return new double[this->Ncol];});
    for (long int i = 0L; i < this->Nrow; ++i)
      is.read(reinterpret_cast < char * >(this->input[i]), sizeof(double) * this->Ncol);
    is.close();
  }
  else
  {
    std :: vector < std :: string > row_;
    std :: ifstream is(filename);
    if ( !is ) error_pattern(filename);
    std::stringstream buff;
    buff << is.rdbuf();
    is.close();

    // Get M
    std :: string row;
    std :: getline(buff, row);
    row_ = split(row, del);
    this->Nout = row_.size();
    this->Nrow = this->Nout;

    // Read outputs
    this->output = new long int [this->Nout];
    std :: transform( row_.begin(), row_.end(), this->output, [](std :: string & i){return std :: stod(i);} );

    // Get N
    this->input = new double*[this->Nrow];
    std :: getline(buff, row);
    row_ = split(row, del);
    this->Ncol = row_.size();
    std :: generate_n(this->input, this->Nrow, [&](){return new double[this->Ncol];});

    // Read first pattern
    std :: transform( row_.begin(), row_.end(), this->input[0L], [](std :: string & i){return std :: stod(i);} );

    // Read all others
    for (long int i = 1L; i < this->Nrow; ++i)
    {
      std :: getline(buff, row);
      row_ = split(row, del);
      std :: transform( row_.begin(), row_.end(), this->input[i], [](std :: string & rr){return std :: stod(rr);} );
    }

  }

#ifdef DEBUG
  check_binary();
#endif
}


Patterns :: Patterns(const long int & N, const long int & M) : Nrow (N), Ncol (M), Nout (N)
{
  this->input  = new double*[this->Nrow];
  this->output = new long int [this->Nrow];

  std :: default_random_engine engine;
  std :: bernoulli_distribution dist(.5);
  std :: generate_n(this->output, this->Nout, [&](){return (dist(engine)) ? 1L : -1L;});
  std :: generate_n(this->input, this->Nrow, [&](){return new double[this->Ncol];});
  for (long int i = 0L; i < this->Nrow; ++i)
    std :: generate_n(this->input[i], this->Ncol,
                      [&]()
                      {
                        return (dist(engine)) ? 1. : -1.;
                      });
}

Patterns :: Patterns (double ** data, long int * label, const int & Nrow, const int & Ncol)
{
  this->Nrow   = Nrow;
  this->Ncol   = Ncol;
  this->Nout   = Nrow;
  this->input  = new double*[this->Nrow];
  this->output = new long int[this->Nrow];

  std :: generate_n(this->input, this->Nrow, [&](){return new double[this->Ncol];});
  for (int i = 0; i < this->Nrow; ++i) std :: copy_n(data[i], this->Ncol, this->input[i]);
  std :: copy_n(label, this->Nrow, this->output);
}

Patterns & Patterns :: operator = (const Patterns & p)
{
  this->Nrow   = p.Nrow;
  this->Ncol   = p.Ncol;
  this->Nout   = p.Nrow;
  this->input  = new double*[this->Nrow];
  this->output = new long int[this->Nrow];

  std :: generate_n(this->input, this->Nrow, [&](){return new double[this->Ncol];});
  for (int i = 0; i < this->Nrow; ++i) std :: copy_n(p.input[i], this->Ncol, this->input[i]);
  std :: copy_n(p.output, this->Nrow, this->output);

  return *this;
}

Patterns :: Patterns (const Patterns & p)
{
  this->Nrow   = p.Nrow;
  this->Ncol   = p.Ncol;
  this->Nout   = p.Nrow;
  this->input  = new double*[this->Nrow];
  this->output = new long int[this->Nrow];

  std :: generate_n(this->input, this->Nrow, [&](){return new double[this->Ncol];});
  for (int i = 0; i < this->Nrow; ++i) std :: copy_n(p.input[i], this->Ncol, this->input[i]);
  std :: copy_n(p.output, this->Nrow, this->output);
}

Patterns :: ~Patterns ()
{
  if (this->Nrow)
  {
    for (long int i = 0L; i < this->Nrow; ++i) delete[] this->input[i];
    delete[] this->input;
  }
  if (this->Nout) delete[] this->output;
}


#ifdef DEBUG
void Patterns :: check_binary ()
{
  assert(this->Nout == this->Nrow);
  int cnt = 0;
  for (long int i = 0L; i < this->Nrow; ++i)
    cnt += std :: count_if(this->input[i], this->input[i] + this->Ncol,
                           [](const double & v)
                           {
                            return std :: abs(v);
                           });
  assert(cnt == this->Nrow * this->Ncol);
  cnt = std :: accumulate(this->output, this->output + this->Nout,
                          0, [](const int & res, const long int & v)
                          {
                            return res + static_cast < int >(std :: abs(v));
                          });
  assert(cnt == this->Nout);
}
#endif
