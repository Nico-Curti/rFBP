#include <algorithm>
#include <numeric>

template < class Ti, class To >
To ** double_pointers_for_cython ( Ti * input_pointer, const int & n, const int & m)
{
  To ** output_pointer = new To * [n];
  std :: generate_n( output_pointer, n, [&](){ return new To[m]; } );
  int idx = 0;
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < m; ++j, ++idx)
      output_pointer[i][j] = static_cast < To > (input_pointer[idx]);
  return output_pointer;
}
