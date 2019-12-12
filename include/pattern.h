#ifndef __patterns_h__
#define __patterns_h__

#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <algorithm>
#include <numeric>

#include <errors.h>
#include <utils.hpp>

#ifdef DEBUG

  #include <cassert>

#endif

/**
* @class Pattern
* @brief Abstract type used to store the input Pattern as (data, labels).
* To be provided as an argument to `focusingBP`.
*
* @details the input pattern can be loaded from binary/ascii file, random generated
* or from a given matrix
*
*/
class Patterns
{

public:

  // Members

  long int Nrow;     ///< number of input vectors (rows of input or samples)
  long int Ncol;     ///< number of cols in input (probes)
  long int Nout;     ///< lenght of output labels
  long int * output; ///< output vector
  double ** input;   ///< input matrix

  // Constructors

  /**
  * @brief default constructor
  *
  */
  Patterns ();

  /**
  * @brief Load pattern from file
  *
  * @param filename Input filename
  * @param bin switch between binary/ascii files (default = false)
  * @param del delimiter string if the file is in ascii fmt (default = \'t')
  *
  */
  Patterns (const std :: string & filename, bool bin=false, const std :: string & del="\t");

  /**
  * @brief Generate random patter
  *
  * @details data are generated according to a Bernoulli distribution but rescaled in [-1, +1]
  *
  * @param N number of input vectors (samples)
  * @param M number of columns (probes)
  *
  */
  Patterns (const long int & N, const long int & M);

  /**
  * @brief Copy pattern from arrays
  *
  * @details data and labels are copied so be careful with this constructor
  *
  * @param data matrix of data in double format
  * @param label array of labels
  * @param Nrow number of rows in data matrix
  * @param Ncol number of columns in data matrix
  *
  */
  Patterns (double ** data, long int * label, const int & Nrow, const int & Ncol);

  // Copy Constructors and Copy operators

  /**
  * @brief copy operator
  *
  * @param p Pattern object
  *
  */
  Patterns & operator = (const Patterns & p);

  /**
  * @brief copy constructor
  *
  * @param p Pattern object
  *
  */
  Patterns (const Patterns & p);

  // Destructors

  /**
  * @brief destructor
  *
  */
  ~Patterns ();


private:

  // Private members

#ifdef DEBUG

  /**
  * @brief check if input and labels are made by binary values
  *
  */
  void check_binary ();

#endif

};

#endif // __patterns_h__
