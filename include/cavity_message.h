#ifndef MESSAGE_H
#define MESSAGE_H
#include <algorithm>
#include <numeric>
#include <random>
#include <fstream>
#include <sstream>
#include <iterator>

#include <magnetization.h>
#include <errors.h>
#include <params.h>

#ifdef DEBUG
#include <cassert>
#endif

#include <utils.h>

template < class Mag >
class Cavity_Message
{

public:

  // public member variables

  long int        M,          // input sample size
                  N,          // input layers size
                  K,          // hidden layers size
                  seed;       // random seed
  MagVec3 <Mag>   weights;    // uw
  MagVec2 <Mag>   m_star_j,   // ux
                  m_j_star,   // mw
                  m_in,       // mt1
                  m_no,       // Ut1
                  m_ni;       // ut1
  MagVec<Mag>     m_on;       // mt2

  // Constructors

  Cavity_Message();
  Cavity_Message (const std :: string & filename, const bool & bin);
  Cavity_Message (const long int & m, const long int & n, const long int & k, const double & x, const int & start);

  // Copy Operator and Copy Constructor

  Cavity_Message (const Cavity_Message < Mag > & m);
  Cavity_Message < Mag > & operator = (const Cavity_Message < Mag > & m);

  // Destructor

  ~Cavity_Message ();

  // Methods

  long int ** get_weights();

  void save_weights(const std :: string & filename, Params < Mag > & parameters);
  void save_weights(const std :: string & filename);

  void read_weights(const std :: string & filename, const bool & bin);

  void save_messages(const std :: string & filename, Params < Mag > & parameters);
  void save_messages(const std :: string & filename);

private:

  std :: vector <std :: string> split(const std :: string & txt, const std :: string & del);
};

#endif // MESSAGE_H
