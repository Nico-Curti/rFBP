#ifndef __parse_args_h__
#define __parse_args_h__

#include <memory>   // std :: unique_ptr
#include <iostream> // std :: cerr
#include <utility>  // std :: move
#include <vector>   // std :: vector
#include <string>   // std :: stod
#include <iomanip>  // std :: setw
#include <type_traits> // std :: is_same_v
#include <stdexcept>

#ifndef _MSC_VER

#include <cxxabi.h>

#endif


class argument
{
  // Private members

public:

  // Public members

  std :: vector < std :: string > values;

  std :: string name;
  std :: string short_flag;
  std :: string long_flag;
  std :: string help;
  std :: string defualt_value;
  std :: string data_type;

  bool required;

  // Constructors

  argument (std :: string && name, std :: string && short_flag, std :: string && long_flag, std :: string && help, const bool & required, std :: string && defualt_value, std :: string && data_type);

  // Destructors

  ~argument () = default;

};


class ArgumentParser
{
  // Static Variables

  static int PRINT_HELP;
  static int ERROR_PARSER;
  static int ERROR_PARSER_INPUTS;
  static int ERROR_PARSER_REQUIRED;
  static int ERROR_PARSER_UNKNOWN;
  static int ERROR_PARSER_INVARG;
  static int ERROR_PARSER_OUTRANGE;
  static int ERROR_PARSER_BOOL;
  static int ERROR_PARSER_CHAR;

  // Private Members

  std :: vector < argument > args;

  std :: string description;
  std :: string program;

public:

  // Constructors

  ArgumentParser ( std :: string && description );

  // Destructors

  ~ArgumentParser () = default;

  // Public members

  void parse_args (const int & argc, char ** argv);

  // Template methods

  template < typename data_t >
  void add_argument (std :: string && name, std :: string && short_flag, std :: string && long_flag, std :: string && help, const bool & req, data_t && default_value);

  template < typename data_t >
  void get (const std :: string & name, data_t & values);

private:

  // Private methods

  void print_help (const int & error_index);
  void error_parsing_type (const std :: string & data_type);
  void error_parsing_inputs_arg ();
  void error_parsing_required_arg (const std :: string & name);
  void error_parsing_unknown_arg (const std :: string & name);
  void error_parsing_invalid_arg (const std :: string & name, const std :: string & value);
  void error_parsing_out_of_range_arg (const std :: string & name, const std :: string & value);
  void error_parsing_bool (const std :: string & name, const std :: string & value);
  void error_parsing_char (const std :: string & name, const std :: string & value);

  template < typename data_t > std :: string type_name ();

};

#endif // __parse_args_h__
