#include <utils.h>

double sign (const double & x)
{
  return (x > 0.) ? 1. : (x < 0. ? -1. : 0);
}

bool file_exists (const std :: string & filename)
{
//#if have_filesystem == 1
//
//  return std :: filesystem :: exists(std :: filesystem :: path(filename));
//
//#else

  if (FILE *file = fopen(filename.c_str(), "r"))
  {
    fclose(file);
    return true;
  }
  return false;

//#endif
}

#ifdef _OPENMP

  double what_time_is_it_now ()
  {
    return omp_get_wtime();
  }

#else

  std :: chrono :: time_point < std :: chrono :: high_resolution_clock > what_time_is_it_now ()
  {
    return std :: chrono :: high_resolution_clock :: now();
  }

#endif

std :: vector < std :: string > split (const std :: string & txt, const std :: string & del)
{
  std :: vector < std :: string > token;

  std :: size_t pos = txt.find_first_of(del);
  std :: size_t start = 0;
  std :: size_t end = txt.size();

  while (pos != std :: string :: npos)
  {
    if (pos) token.push_back(txt.substr(start, pos));
    start += pos + 1;
    pos = txt.substr(start, end).find_first_of(del);
  }

  if (start != end) token.push_back(txt.substr(start, pos));

  return token;
}
