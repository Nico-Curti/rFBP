#include <algorithm>
#include <numeric>

template < class Ti, class To >
To ** double_pointers_for_cython ( Ti * input_pointer, const long int & n, const long int & m)
{
  To ** output_pointer = new To * [n];
  std :: generate_n( output_pointer, n, [&](){ return new To[m]; } );
  long int idx = 0;
  for (long int i = 0; i < n; ++i)
    for (long int j = 0; j < m; ++j, ++idx)
      output_pointer[i][j] = static_cast < To > (input_pointer[idx]);
  return output_pointer;
}

template < typename type >
type * unique_pointer_to_pointer (const std :: unique_ptr < type[] > & src, const long int & size)
{
  type * dest = new type[size];
  std :: copy_n(src.get(), size, dest);
  return dest;
}

template < typename type >
std :: unique_ptr < type[] > pointer_to_unique_pointer (type * src, const long int & size)
{
  std :: unique_ptr < type[] > dest (new type[size]);
  std :: move(src, src + size, dest.get());
  return dest;
}
