#ifndef PATTERNS_H
#define PATTERNS_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <random>
#include <algorithm>
#include <numeric>

#include <errors.h>
#include <utils.h>
#ifdef DEBUG
#include <cassert>
#endif

struct Patterns
{
  long int Nrow,  // number of input vectors (rows of input)
           Ncol,  // number of cols in input
           Nout;  // lenght of output labels
  std::unique_ptr<long int[]> output; // output vector
  double **input;

  Patterns(const std::string &filename, bool bin = false, const std::string &del = "\t");
  Patterns(const long int &N, const long int &M);
  //Patterns& operator=(const Patterns &p);
  //Patterns(const Patterns &p);
  Patterns() : Nrow(0L), Ncol(0L), Nout(0L), output(nullptr), input(nullptr){};
  ~Patterns();

#ifdef DEBUG
private:
  void check_binary();
#endif
};

#endif // PATTERNS_H
