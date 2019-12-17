#ifndef __parse_args_hpp__
#define __parse_args_hpp__

#if __cplusplus < 201700 // no std=c++17 support

  #include <parse_args_old.hpp>

#else // modern version

#include <parse_args.h>

template < typename data_t >
void ArgumentParser :: add_argument (std :: string && name, std :: string && short_flag, std :: string && long_flag, std :: string && help, const bool & req, data_t default_value)
{
  static_assert (!std :: is_same_v < data_t, int >    ||
                 !std :: is_same_v < data_t, float >  ||
                 !std :: is_same_v < data_t, double > ||
                 !std :: is_same_v < data_t, char >   ||
                 !std :: is_same_v < data_t, bool >   ||
                 !std :: is_same_v < data_t, std :: string >,
                 "Error parsing! Argument type not understood in command line.");


  if constexpr ( std :: is_same_v < data_t, std :: string > )
  {
    std :: string string_data_type = "std :: string";
    this->args.emplace_back(argument(static_cast < std :: string && >(name), static_cast < std :: string && >(short_flag), static_cast < std :: string && >(long_flag), static_cast < std :: string && >(help), req, static_cast < std :: string && >(default_value), static_cast < std :: string && >(string_data_type)));
  }

  else
  {
    std :: string string_data_type = this->type_name < data_t >();
    this->args.emplace_back(argument(static_cast < std :: string && >(name), static_cast < std :: string && >(short_flag), static_cast < std :: string && >(long_flag), static_cast < std :: string && >(help), req, static_cast < std :: string && >(std :: to_string(default_value)), static_cast < std :: string && >(string_data_type)));
  }

}



template < typename data_t >
void ArgumentParser :: get (const std :: string & name, data_t & values)
{
  static_assert (!std :: is_same_v < data_t, int >                         ||
                 !std :: is_same_v < data_t, float >                       ||
                 !std :: is_same_v < data_t, double >                      ||
                 !std :: is_same_v < data_t, char >                        ||
                 !std :: is_same_v < data_t, bool >                        ||
                 !std :: is_same_v < data_t, std :: string >               ||
                 !std :: is_same_v < data_t, std :: vector < int > >       ||
                 !std :: is_same_v < data_t, std :: vector < float > >     ||
                 !std :: is_same_v < data_t, std :: vector < double > >    ||
                 !std :: is_same_v < data_t, std :: vector < char > >      ||
                 !std :: is_same_v < data_t, std :: vector < bool > >      ||
                 !std :: is_same_v < data_t, std :: vector < std :: string > >,
                 "Error parsing! Variable type unknown by parser."
                );


  if constexpr ( std :: is_same_v < data_t, std :: vector < std :: string > > )
  {
    bool found = true;

    for (std :: size_t i = 0; i < this->args.size(); ++i)
      if (args[i].name == name)
      {
        values = std :: move(args[i].values);
        found = false;
      }

    if (found)
      this->error_parsing_unknown_arg(name);
  }

  else if constexpr ( std :: is_same_v < data_t, std :: string > )
  {
    for (std :: size_t i = 0; i < this->args.size(); ++i)
      if (args[i].name == name)
      {
        values = std :: move(args[i].values[0]);
        return;
      }

    this->error_parsing_unknown_arg(name);
  }

  else if constexpr ( std :: is_fundamental_v < data_t > )
  {
    for (std :: size_t i = 0; i < args.size(); ++i)
      if (args[i].name == name)
      {
        try
        {
          values = static_cast < data_t >(std :: stod(args[i].values[0]));
          return;
        }
        catch ( std :: invalid_argument & )
        {
          this->error_parsing_invalid_arg(name, args[i].values[0]);
        }

        catch ( std :: out_of_range & )
        {
          this->error_parsing_out_of_range_arg(name, args[i].values[0]);
        }
      }

    this->error_parsing_unknown_arg(name);
  }

  // string case already skipped
  else
  {
    bool found = true;

    for (std :: size_t i = 0; i < args.size(); ++i)
      if (args[i].name == name)
      {
        for (const auto & val : args[i].values)
        {
          try
          {
            values = static_cast < data_t >(std :: stod(val));
          }
          catch ( std :: invalid_argument & )
          {
            this->error_parsing_invalid_arg(name, val);
          }

          catch ( std :: out_of_range & )
          {
            this->error_parsing_out_of_range_arg(name, val);
          }
        }
        found = false;
      }

    if (found)
      this->error_parsing_unknown_arg(name);
  }

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


#endif // old gcc

#endif // __parse_args_hpp__
