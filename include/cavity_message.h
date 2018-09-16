#ifndef MESSAGE_H
#define MESSAGE_H
#include <algorithm>
#include <numeric>
#include <random>

#include <magP.h>
#include <magT.h>
#include <errors.h>

template<class Mag>  using MagVec  = Mag*;
template<class Mag>  using MagVec2 = MagVec<Mag>*;
template<class Mag>  using MagVec3 = MagVec2<Mag>*;


template<class Mag> struct Cavity_Message
{
  long int        M,          // input sample size
                  N,          // input layers size
                  K,          // hidden layers size
                  seed;       // random seed
  MagVec3<Mag>    weights;    // uw
  MagVec2<Mag>    m_star_j,   // ux
                  m_j_star,   // mw
                  m_in,       // mt1
                  m_no,       // Ut1
                  m_ni;       // ut1
  MagVec<Mag>     m_on;       // mt2

  Cavity_Message() : M(0L), N(0L), K(0L), seed(0L), weights(nullptr), m_star_j(nullptr), m_j_star(nullptr), m_in(nullptr), m_no(nullptr), m_ni(nullptr), m_on(nullptr) {};
  Cavity_Message(const long int &m, const long int &n, const long int &k, const double &x, const int &start);
  Cavity_Message(const Cavity_Message<Mag> &m);
  Cavity_Message<Mag>& operator=(const Cavity_Message<Mag> &m);
  ~Cavity_Message();

  //Cavity_Message<T, Mag> read_messages(const std::string &io);
  //void save_weights(const std::string &filename, const bool &binary);

};

#endif // MESSAGE_H
