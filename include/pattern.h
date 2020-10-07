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
* @class Patterns
* @brief Abstract type used to store the input Pattern as (data, labels).
* To be provided as an argument to `focusingBP`.
*
* @details Inputs and outputs must have the same length.
* Outputs entries must be ∈ {-1,1}.
* Intputs entries must be arrays in which each element is ∈ {-1,1}, and
* all the vectors must have the same length.
* The input pattern can be loaded from binary/ascii file, random generated
* or from a given matrix.
*
*/
class Patterns
{

public:

  // Members

  long int Nrow;     ///< Number of input vectors (rows of input or samples)
  long int Ncol;     ///< Number of cols in input (probes)
  long int Nout;     ///< Lenght of output labels
  long int * output; ///< Output vector
  double ** input;   ///< Input matrix

  // Constructors

  /**
  * @brief Default constructor.
  *
  */
  Patterns ();

  /**
  * @brief Load pattern from file.
  *
  * @details The input file can store the values in binary or ascii format.
  * For the **binary** format the function requires a file formatted following these instructions:
  *
  * - Number of rows (long int)
  * - Number of columns (long int)
  * - labels array (long int) with a length equal to the number of rows
  * - data matrix (double) with a shape (number of rows x number of columns).
  *
  * For the **ascii** format the function requires a file formatted following these instructions:
  *
  * - labels array (as a series of separated numbers)
  * - matrix of data (as a series of rows separated by `\n`)
  *
  * @param filename Input filename
  * @param bin switch between binary/ascii files (default = false)
  * @param del delimiter string if the file is in ascii fmt (default = "\t")
  *
  * @note Outputs entries must be ∈ {-1,1}. Intputs entries must be arrays in which each element is ∈ {-1,1}.
  * @note In the ascii format the delimiter of the file can be set using the `del` variable.
  *
  */
  Patterns (const std :: string & filename, bool bin=false, const std :: string & del="\t");

  /**
  * @brief Generate random patter.
  *
  * @details The pattern is generated using a Bernoulli distribution and thus it creates a data (matrix) + labels (vector)
  * of binary values. The values are converted into the range (-1, 1) for the compatibility with the rFBP algorithm.
  *
  *
  * @param N number of input vectors (samples)
  * @param M number of columns (probes)
  *
  */
  Patterns (const long int & N, const long int & M);

  /**
  * @brief Copy pattern from arrays.
  *
  * @details Data and labels are copied, so be careful with this constructor.
  *
  * @param data matrix of data in double format
  * @param label array of labels
  * @param Nrow number of rows in data matrix
  * @param Ncol number of columns in data matrix
  *
  * @note The copy of the arrays is performed for compatibility with the Python API of the library.
  *
  */
  Patterns (double ** data, long int * label, const int & Nrow, const int & Ncol);

  // Copy Constructors and Copy operators

  /**
  * @brief Copy operator.
  *
  * @details The operator performs a deep copy of the object and if there are buffers
  * already allocated, the operatore deletes them and then re-allocates an appropriated
  * portion of memory.
  *
  * @param p Pattern object
  *
  */
  Patterns & operator = (const Patterns & p);

  /**
  * @brief Copy constructor.
  *
  * @details The copy constructor provides a deep copy of the object, i.e. all the
  * arrays are copied and not moved.
  *
  * @param p Pattern object
  *
  */
  Patterns (const Patterns & p);

  // Destructors

  /**
  * @brief Destructor
  *
  * @details Completely destroy the object releasing the data/labels memory.
  *
  */
  ~Patterns ();


private:

  // Private members

#ifdef DEBUG

  /**
  * @brief Check if input and labels are made by binary values.
  *
  * @note This function (check) is disabled in Release mode, thus be careful.
  *
  */
  void check_binary ();

#endif

};

#endif // __patterns_h__
