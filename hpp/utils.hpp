#ifndef __utils_hpp__
#define __utils_hpp__

#include <utils.h>

template < class Mag >  using MagVec  = Mag *;             ///< alias to pointer to Mag
template < class Mag >  using MagVec2 = MagVec < Mag > *;  ///< alias to pointer to MagVec
template < class Mag >  using MagVec3 = MagVec2 < Mag > *; ///< alias to pointer to MagVec2


#if __cplusplus < 201400

namespace std
{

  template < typename T >
  std :: unique_ptr < T > make_unique ( std :: size_t size )
  {
    return std :: unique_ptr < T > ( new typename std :: remove_extent < T > :: type[size] () );
  }

}

#endif // __cplusplus

template < typename Time >
inline auto duration (const Time & start)
{
#ifdef _OPENMP

  return omp_get_wtime() - start;

#else

  return std :: chrono :: duration_cast < std :: chrono :: seconds >(std :: chrono :: high_resolution_clock :: now() - start).count();

#endif
}

#endif // __utils_hpp__
