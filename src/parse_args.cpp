#include <parse_args.hpp>


int ArgumentParser :: PRINT_HELP            = 101;
int ArgumentParser :: ERROR_PARSER          = 102;
int ArgumentParser :: ERROR_PARSER_INPUTS   = 103;
int ArgumentParser :: ERROR_PARSER_REQUIRED = 104;
int ArgumentParser :: ERROR_PARSER_UNKNOWN  = 105;
int ArgumentParser :: ERROR_PARSER_INVARG   = 106;
int ArgumentParser :: ERROR_PARSER_OUTRANGE = 107;
int ArgumentParser :: ERROR_PARSER_BOOL     = 108;
int ArgumentParser :: ERROR_PARSER_CHAR     = 109;


// Argument Class

argument :: argument (std :: string && name, std :: string && short_flag, std :: string && long_flag, std :: string && help, const bool & required, std :: string && defualt_value, std :: string && data_type)
                      : name (std :: move(name)), short_flag (std :: move(short_flag)), long_flag (std :: move(long_flag)), help (std :: move(help)), defualt_value (std :: move(defualt_value)), data_type (std :: move(data_type)), required (required)
{
}


// ArgumentParse Class

ArgumentParser :: ArgumentParser ( std :: string && description ) : description (std :: move(description))
{
}

void ArgumentParser :: parse_args (const int & argc, char ** argv)
{
  this->program = std :: string(argv[0]);

  if ( static_cast < int >(this->args.size()) < ((argc - 1) >> 1) )
    this->error_parsing_inputs_arg();

  for (auto & arg : this->args)
  {
    bool found = false;

    for (int i = 0; i < argc; ++i)
    {
      if ( std :: string (argv[i]) == "-" + arg.short_flag ||
           std :: string (argv[i]) == "--" + arg.long_flag
         )
      {
        arg.values.emplace_back(argv[i + 1]);

        for (int j = i + 2; j < argc; ++j)
        {
          if ( argv[j][0] != '-' ) arg.values.emplace_back(argv[j]);
          else break;
        }

        found = true;
        break;
      }
    }

    if ( !found )
    {
      if (arg.required) this->error_parsing_required_arg(arg.name);
      else              arg.values.emplace_back(std :: move(arg.defualt_value));
    }
  }
}


void ArgumentParser :: print_help (const int & error_index)
{
  std :: cerr << "Usage: " << this->program << " ";

  for (auto & arg : this->args)
    std :: cerr << ((arg.required) ? "-" + arg.short_flag + " <" + arg.data_type + ">"
                                   : "[-" + arg.long_flag + " <" + arg.data_type + "> ]") << " ";

  std :: cerr << std :: endl << std :: endl;

  std :: cerr << this->description << std :: endl << std :: endl;

  std :: cerr << "optional arguments:" << std :: endl;

  for(auto & arg : this->args)
    std :: cerr << "\t" << std :: left << std :: setw(6)  << "-" + arg.short_flag + ","
                        << std :: left << std :: setw(12) <<"--" + arg.long_flag
                        << "\t\t" + arg.help
                        << std :: endl;

  std :: exit(error_index);
}



void ArgumentParser :: error_parsing_type (const std :: string & data_type)
{
  std :: cerr << "Error parsing! Wrong type variable: given, '"
              << data_type
              << "'; possible type are 'int', 'float', 'double', 'string', 'char', 'bool'"
              << std :: endl;

  this->print_help(ArgumentParser :: ERROR_PARSER);
}

void ArgumentParser :: error_parsing_inputs_arg ()
{
  std :: cerr << "Error parsing! Not enough inputs in command line."
              << std :: endl;

  this->print_help(ArgumentParser :: ERROR_PARSER_INPUTS);
}

void ArgumentParser :: error_parsing_required_arg (const std :: string & name)
{
  std :: cerr << "Error parsing! Input not found. Variable "
              << name << " is required but it's not found in command line."
              << std :: endl;

  this->print_help(ArgumentParser :: ERROR_PARSER_REQUIRED);
}

void ArgumentParser :: error_parsing_unknown_arg (const std :: string & name)
{
  std :: cerr << "Error parsing! Variable "
              << name
              << " unknown by parser."
              << std :: endl;

  this->print_help(ArgumentParser :: ERROR_PARSER_UNKNOWN);
}

void ArgumentParser :: error_parsing_invalid_arg (const std :: string & name, const std :: string & value)
{
  std :: cerr << "Error parsing! Invalid argument for variable "
              << name
              << ". Given: "
              << value
              << " ."
              << std :: endl;

  this->print_help(ArgumentParser :: ERROR_PARSER_INVARG);
}

void ArgumentParser :: error_parsing_out_of_range_arg (const std :: string & name, const std :: string & value)
{
  std :: cerr << "Error parsing! Out of range argument for variable "
              << name
              << ". Given: "
              << value
              << " ."
              << std :: endl;

  this->print_help(ArgumentParser :: ERROR_PARSER_OUTRANGE);
}

void ArgumentParser :: error_parsing_bool (const std :: string & name, const std :: string & value)
{
  std :: cerr << "Error parsing! Invalid argument for bool variable "
              << name
              << ". Given: "
              << value
              << " ."
              << std :: endl;

  this->print_help(ArgumentParser :: ERROR_PARSER_BOOL);
}

void ArgumentParser :: error_parsing_char (const std :: string & name, const std :: string & value)
{
  std :: cerr << "Error parsing! Invalid argument for char variable "
              << name
              << ". Given: "
              << value
              << " ."
              << std :: endl;

  this->print_help(ArgumentParser :: ERROR_PARSER_CHAR);
}


#if __cplusplus < 201700 // no std=c++17 support

template < >
void ArgumentParser :: add_argument < int > (std :: string && name, std :: string && short_flag, std :: string && long_flag, std :: string && help, const bool & req, int default_value)
{
  std :: string string_data_type = "int";
  this->args.emplace_back(argument(static_cast < std :: string && >(name), static_cast < std :: string && >(short_flag), static_cast < std :: string && >(long_flag), static_cast < std :: string && >(help), req, static_cast < std :: string && >(std :: to_string(default_value)), static_cast < std :: string && >(string_data_type)));
}

template < >
void ArgumentParser :: add_argument < float > (std :: string && name, std :: string && short_flag, std :: string && long_flag, std :: string && help, const bool & req, float default_value)
{
  std :: string string_data_type = "float";
  this->args.emplace_back(argument(static_cast < std :: string && >(name), static_cast < std :: string && >(short_flag), static_cast < std :: string && >(long_flag), static_cast < std :: string && >(help), req, static_cast < std :: string && >(std :: to_string(default_value)), static_cast < std :: string && >(string_data_type)));
}

template < >
void ArgumentParser :: add_argument < double > (std :: string && name, std :: string && short_flag, std :: string && long_flag, std :: string && help, const bool & req, double default_value)
{
  std :: string string_data_type = "double";
  this->args.emplace_back(argument(static_cast < std :: string && >(name), static_cast < std :: string && >(short_flag), static_cast < std :: string && >(long_flag), static_cast < std :: string && >(help), req, static_cast < std :: string && >(std :: to_string(default_value)), static_cast < std :: string && >(string_data_type)));
}

template < >
void ArgumentParser :: add_argument < char > (std :: string && name, std :: string && short_flag, std :: string && long_flag, std :: string && help, const bool & req, char default_value)
{
  std :: string string_data_type = "char";
  this->args.emplace_back(argument(static_cast < std :: string && >(name), static_cast < std :: string && >(short_flag), static_cast < std :: string && >(long_flag), static_cast < std :: string && >(help), req, static_cast < std :: string && >(std :: to_string(default_value)), static_cast < std :: string && >(string_data_type)));
}

template < >
void ArgumentParser :: add_argument < bool > (std :: string && name, std :: string && short_flag, std :: string && long_flag, std :: string && help, const bool & req, bool default_value)
{
  std :: string string_data_type = "bool";
  this->args.emplace_back(argument(static_cast < std :: string && >(name), static_cast < std :: string && >(short_flag), static_cast < std :: string && >(long_flag), static_cast < std :: string && >(help), req, static_cast < std :: string && >(std :: to_string(default_value)), static_cast < std :: string && >(string_data_type)));
}

template < >
void ArgumentParser :: add_argument < std :: string > (std :: string && name, std :: string && short_flag, std :: string && long_flag, std :: string && help, const bool & req, std :: string default_value)
{
  std :: string string_data_type = "std :: string";
  this->args.emplace_back(argument(static_cast < std :: string && >(name), static_cast < std :: string && >(short_flag), static_cast < std :: string && >(long_flag), static_cast < std :: string && >(help), req, static_cast < std :: string && >(default_value), static_cast < std :: string && >(string_data_type)));
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

#endif

