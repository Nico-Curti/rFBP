#ifndef __parse_args_old_hpp__
#define __parse_args_old_hpp__

#include <parse_args.h>

template < >
void ArgumentParser :: add_argument < int > (std :: string && name, std :: string && short_flag, std :: string && long_flag, std :: string && help, const bool & req, int && default_value)
{
  std :: string string_data_type = "int";
  this->args.emplace_back(argument(static_cast < std :: string && >(name), static_cast < std :: string && >(short_flag), static_cast < std :: string && >(long_flag), static_cast < std :: string && >(help), req, static_cast < std :: string && >(std :: to_string(default_value)), static_cast < std :: string && >(string_data_type)));
}

template < >
void ArgumentParser :: add_argument < float > (std :: string && name, std :: string && short_flag, std :: string && long_flag, std :: string && help, const bool & req, float && default_value)
{
  std :: string string_data_type = "float";
  this->args.emplace_back(argument(static_cast < std :: string && >(name), static_cast < std :: string && >(short_flag), static_cast < std :: string && >(long_flag), static_cast < std :: string && >(help), req, static_cast < std :: string && >(std :: to_string(default_value)), static_cast < std :: string && >(string_data_type)));
}

template < >
void ArgumentParser :: add_argument < double > (std :: string && name, std :: string && short_flag, std :: string && long_flag, std :: string && help, const bool & req, double && default_value)
{
  std :: string string_data_type = "double";
  this->args.emplace_back(argument(static_cast < std :: string && >(name), static_cast < std :: string && >(short_flag), static_cast < std :: string && >(long_flag), static_cast < std :: string && >(help), req, static_cast < std :: string && >(std :: to_string(default_value)), static_cast < std :: string && >(string_data_type)));
}

template < >
void ArgumentParser :: add_argument < char > (std :: string && name, std :: string && short_flag, std :: string && long_flag, std :: string && help, const bool & req, char && default_value)
{
  std :: string string_data_type = "char";
  this->args.emplace_back(argument(static_cast < std :: string && >(name), static_cast < std :: string && >(short_flag), static_cast < std :: string && >(long_flag), static_cast < std :: string && >(help), req, static_cast < std :: string && >(std :: to_string(default_value)), static_cast < std :: string && >(string_data_type)));
}

template < >
void ArgumentParser :: add_argument < bool > (std :: string && name, std :: string && short_flag, std :: string && long_flag, std :: string && help, const bool & req, bool && default_value)
{
  std :: string string_data_type = "bool";
  this->args.emplace_back(argument(static_cast < std :: string && >(name), static_cast < std :: string && >(short_flag), static_cast < std :: string && >(long_flag), static_cast < std :: string && >(help), req, static_cast < std :: string && >(std :: to_string(default_value)), static_cast < std :: string && >(string_data_type)));
}

template < >
void ArgumentParser :: add_argument < std :: string > (std :: string && name, std :: string && short_flag, std :: string && long_flag, std :: string && help, const bool & req, std :: string && default_value)
{
  std :: string string_data_type = "std :: string";
  this->args.emplace_back(argument(static_cast < std :: string && >(name), static_cast < std :: string && >(short_flag), static_cast < std :: string && >(long_flag), static_cast < std :: string && >(help), req, static_cast < std :: string && >(default_value), static_cast < std :: string && >(string_data_type)));
}

template < typename data_t >
void ArgumentParser :: add_argument (std :: string && name, std :: string && short_flag, std :: string && long_flag, std :: string && help, const bool & req, data_t && default_value)
{
  std :: cerr << "Error parsing! Argument type not understood in command line." << std :: endl;
  std :: exit(ERROR_PARSER_INPUTS);
}



template < >
void ArgumentParser :: get < int > (const std :: string & name, int & values)
{
  for (std :: size_t i = 0; i < args.size(); ++i)
    if (args[i].name == name)
    {
      try
      {
        values = static_cast < int >(std :: stod(args[i].values[0]));
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

template < >
void ArgumentParser :: get < float > (const std :: string & name, float & values)
{
  for (std :: size_t i = 0; i < args.size(); ++i)
    if (args[i].name == name)
    {
      try
      {
        values = static_cast < float >(std :: stod(args[i].values[0]));
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

template < >
void ArgumentParser :: get < double > (const std :: string & name, double & values)
{
  for (std :: size_t i = 0; i < args.size(); ++i)
    if (args[i].name == name)
    {
      try
      {
        values = std :: stod(args[i].values[0]);
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

template < >
void ArgumentParser :: get < char > (const std :: string & name, char & values)
{
  for (std :: size_t i = 0; i < args.size(); ++i)
    if (args[i].name == name)
    {
      try
      {
        values = static_cast < char >(std :: stod(args[i].values[0]));
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

template < >
void ArgumentParser :: get < bool > (const std :: string & name, bool & values)
{
  for (std :: size_t i = 0; i < args.size(); ++i)
    if (args[i].name == name)
    {
      try
      {
        values = static_cast < bool >(std :: stod(args[i].values[0]));
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

template < >
void ArgumentParser :: get < std :: string > (const std :: string & name, std :: string & values)
{
  for (std :: size_t i = 0; i < this->args.size(); ++i)
    if (args[i].name == name)
    {
      values = std :: move(args[i].values[0]);
      return;
    }

  this->error_parsing_unknown_arg(name);
}

template < >
void ArgumentParser :: get < std :: vector < int > > (const std :: string & name, std :: vector < int > & values)
{
  bool found = true;

  for (std :: size_t i = 0; i < args.size(); ++i)
    if (args[i].name == name)
    {
      for (const auto & val : args[i].values)
      {
        try
        {
          values.push_back(static_cast < int >(std :: stod(val)));
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

template < >
void ArgumentParser :: get < std :: vector < float > > (const std :: string & name, std :: vector < float > & values)
{
  bool found = true;

  for (std :: size_t i = 0; i < args.size(); ++i)
    if (args[i].name == name)
    {
      for (const auto & val : args[i].values)
      {
        try
        {
          values.push_back(static_cast < float >(std :: stod(val)));
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

template < >
void ArgumentParser :: get < std :: vector < double > > (const std :: string & name, std :: vector < double > & values)
{
  bool found = true;

  for (std :: size_t i = 0; i < args.size(); ++i)
    if (args[i].name == name)
    {
      for (const auto & val : args[i].values)
      {
        try
        {
          values.push_back(std :: stod(val));
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

template < >
void ArgumentParser :: get < std :: vector < char > > (const std :: string & name, std :: vector < char > & values)
{
  bool found = true;

  for (std :: size_t i = 0; i < args.size(); ++i)
    if (args[i].name == name)
    {
      for (const auto & val : args[i].values)
      {
        try
        {
          values.push_back(static_cast < char >(std :: stod(val)));
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

template < >
void ArgumentParser :: get < std :: vector < bool > > (const std :: string & name, std :: vector < bool > & values)
{
  bool found = true;

  for (std :: size_t i = 0; i < args.size(); ++i)
    if (args[i].name == name)
    {
      for (const auto & val : args[i].values)
      {
        try
        {
          values.push_back(static_cast < bool >(std :: stod(val)));
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

template < >
void ArgumentParser :: get < std :: vector < std :: string > > (const std :: string & name, std :: vector < std :: string > & values)
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

template < typename data_t >
void ArgumentParser :: get (const std :: string & name, data_t & values)
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
