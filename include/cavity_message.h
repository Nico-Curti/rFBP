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

template<class Mag> struct Cavity_Message
{
  long int        M,                    // input sample size
                  N,                    // input layers size
                  K,                    // hidden layers size
                  seed;                 // random seed
  MagVec3<Mag>    weights = nullptr;    // uw
  MagVec2<Mag>    m_star_j = nullptr,   // ux
                  m_j_star = nullptr,   // mw
                  m_in = nullptr,       // mt1
                  m_no = nullptr,       // Ut1
                  m_ni = nullptr;       // ut1
  MagVec<Mag>     m_on = nullptr;       // mt2

  Cavity_Message() : M(0L), N(0L), K(0L), seed(0L),
                     weights(nullptr),
                     m_star_j(nullptr), m_j_star(nullptr), m_in(nullptr), m_no(nullptr), m_ni(nullptr),
                     m_on(nullptr)
                     {};
  Cavity_Message(const long int &m, const long int &n, const long int &k, const double &x, const int &start);
  Cavity_Message(const Cavity_Message<Mag> &m);
  Cavity_Message<Mag>& operator=(const Cavity_Message<Mag> &m);
  ~Cavity_Message();

  void save_weights(const std::string &filename, const Params<Mag> &parameters);
  void save_weights(const std::string &filename);
  void read_weights(const std::string &filename, const bool &bin);

};

#endif // MESSAGE_H
