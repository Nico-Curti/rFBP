#ifndef __message_h__
#define __message_h__

#include <algorithm>
#include <numeric>
#include <random>
#include <fstream>
#include <sstream>
#include <iterator>
#include <vector>
#include <string>

#include <magnetization.h>
#include <errors.h>
#include <params.hpp>

#ifdef DEBUG

  #include <cassert>

#endif

#include <utils.h>


/**
* @class Cavity_Message
* @brief Abstract type used to store weights and messages of FBP algorithm
*
* @details the initial messages can be loaded from file and the resulting one
* can be saved to file
*
* @tparam Mag magnetization chosen for training
*
*/
template < class Mag >
class Cavity_Message
{

public:

  // public member variables

  long int        M;          ///< input sample size
  long int        N;          ///< input layers size
  long int        K;          ///< hidden layers size
  long int        seed;       ///< random seed
  MagVec3 < Mag > weights;    ///< uw in paper nomeclature
  MagVec2 < Mag > m_star_j;   ///< ux in paper nomeclature
  MagVec2 < Mag > m_j_star;   ///< mw in paper nomeclature
  MagVec2 < Mag > m_in;       ///< mt1 in paper nomeclature
  MagVec2 < Mag > m_no;       ///< Ut1 in paper nomeclature
  MagVec2 < Mag > m_ni;       ///< ut1 in paper nomeclature
  MagVec < Mag >  m_on;       ///< mt2 in paper nomeclature

  // Constructors

  /**
  * @brief default constructor
  *
  */
  Cavity_Message ();

  /**
  * @brief Load messages from file
  *
  * @param filename Input filename
  * @param bin switch between binary/ascii files (default = false)
  *
  */
  Cavity_Message (const std :: string & filename, const bool & bin);

  /**
  * @brief Generate random messages
  *
  * @param m number of samples
  * @param n number of probes
  * @param k number of hidden layers
  * @param x initial value
  * @param start random seed
  *
  */
  Cavity_Message (const long int & m, const long int & n, const long int & k, const double & x, const int & start);

  // Copy Operator and Copy Constructor

  /**
  * @brief copy constructor
  *
  * @param m Cavity_Message object
  *
  */
  Cavity_Message (const Cavity_Message < Mag > & m);

  /**
  * @brief copy operator
  *
  * @param m Cavity_Message object
  *
  */
  Cavity_Message < Mag > & operator = (const Cavity_Message < Mag > & m);

  // Destructor

  /**
  * @brief destructor
  *
  */
  ~Cavity_Message ();

  // Methods

  /**
  * @brief Return weights matrix as long int **
  *
  * @details This function is useful as getter member for
  * weight matrix
  *
  */
  long int ** get_weights ();

  /**
  * @brief Save weight matrix to file
  *
  * @details this function save both weights and training parameters
  *
  * @param filename output filename
  * @param parameters Params object
  *
  */
  void save_weights (const std :: string & filename, Params < Mag > & parameters);

  /**
  * @brief Save weight matrix to file
  *
  * @details this function save ONLY weights
  *
  * @param filename output filename
  *
  */
  void save_weights (const std :: string & filename);

  /**
  * @brief Load weight matrix from file
  *
  * @details this function is the opposite of save_weights
  *
  * @param filename input filename
  * @param bin switch between binary/ascii fmt
  *
  */
  void read_weights (const std :: string & filename, const bool & bin);

  /**
  * @brief Save all the messages to file
  *
  * @details this function dump the complete object to file with also
  * the parameters used for the training section
  *
  * @param filename output filename
  * @param parameters Params object
  *
  */
  void save_messages (const std :: string & filename, Params < Mag > & parameters);

  /**
  * @brief Save all the messages to a BINARY file
  *
  * @details this function dump the complete object to BINARY file
  *
  * @param filename output filename
  *
  */
  void save_messages (const std :: string & filename);

};

#endif // __message_h__
