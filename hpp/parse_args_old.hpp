#ifndef __parse_args_old_hpp__
#define __parse_args_old_hpp__

#include <parse_args.h>

template < typename data_t >
void ArgumentParser :: add_argument (__unused std :: string && name, __unused std :: string && short_flag, __unused std :: string && long_flag, __unused std :: string && help, __unused const bool & req, __unused data_t default_value)
{
  std :: cerr << "Error parsing! Argument type not understood in command line." << std :: endl;
  std :: exit(ERROR_PARSER_INPUTS);
}

template < typename data_t >
void ArgumentParser :: get (__unused const std :: string & name, __unused data_t & values)
{
  std :: cerr << "Error parsing! Variable type unknown by parser." << std :: endl;
  std :: exit (ERROR_PARSER_INVARG);
}



template < typename data_t >
std :: string ArgumentParser :: type_name ()
{
  typedef typename std :: remove_reference < data_t > tr;

  std :: unique_ptr < char, void(*)(void*) > own (


#ifndef _MSC_VER

                                                  abi :: __cxa_demangle (typeid(tr).name(), nullptr, nullptr, nullptr),

#else

                                                  nullptr,

#endif
                                                  std :: free
                                                 );

  std :: string r = own != nullptr ? own.get() : typeid(tr).name();

  if ( std :: is_const < tr > :: value ) r += " const";
  if ( std :: is_volatile < tr > :: value ) r += " volatile";

  if      ( std :: is_lvalue_reference < data_t > :: value ) r += "&";
  else if ( std :: is_rvalue_reference < data_t > :: value ) r += "&&";

  return r;
}

#endif // __parse_args_old_hpp__
