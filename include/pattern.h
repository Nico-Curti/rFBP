#ifndef PATTERNS_H
#define PATTERNS_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <algorithm>
#include <numeric>

#include <errors.h>
#include <utils.h>
#ifdef DEBUG
#include <cassert>
#endif

class Patterns
{
public:

  // Members

  long int Nrow,     // number of input vectors (rows of input)
           Ncol,     // number of cols in input
           Nout;     // lenght of output labels
  long int * output; // output vector
  double ** input;   // input matrix

  // Constructors

  Patterns ();
  Patterns (const std :: string & filename, bool bin = false, const std :: string & del = "\t");
  Patterns (const long int & N, const long int &M);
  Patterns (double ** data, long int * label, const int & Nrow, const int & Ncol);

  // Copy Constructors and Copy operators

  Patterns & operator = (const Patterns & p);
  Patterns (const Patterns & p);

  // Destructors

  ~Patterns();


private:

  // Private members

#ifdef DEBUG
  void check_binary();
#endif

};

#endif // PATTERNS_H
