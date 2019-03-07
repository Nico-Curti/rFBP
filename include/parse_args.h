#ifndef PARSE_ARGS_H
#define PARSE_ARGS_H
#include <vector>
#include <type_traits>
#include <memory>
#include <string>
#include <iostream>
#include <iomanip>
#ifndef _MSC_VER
#include <cxxabi.h>
#endif

static constexpr int PRINT_HELP            = 101;
static constexpr int ERROR_PARSER          = 102;
static constexpr int ERROR_PARSER_INPUTS   = 103;
static constexpr int ERROR_PARSER_REQUIRED = 104;
static constexpr int ERROR_PARSER_UNKNOWN  = 105;
static constexpr int ERROR_PARSER_INVARG   = 106;
static constexpr int ERROR_PARSER_OUTRANGE = 107;
static constexpr int ERROR_PARSER_BOOL     = 108;
static constexpr int ERROR_PARSER_CHAR     = 109;


class argument
{
public:
    argument(const std::string &, const std::string &, const std::string &, const std::string &, const bool &, const std::string &, const std::string &);
    std::string name, shr_flag, lng_flag, help, def, type;
    std::vector<std::string> value;
    bool required;
    ~argument() = default;
};

class ArgumentParser
{
    std::vector<argument> args;
    std::string description, program;
    inline void print_help(const int & = PRINT_HELP);
    inline void error_parsing_type(const std::string &);
    inline void error_parsing_inputs();
    inline void error_parsing_required(const std::string &);
    inline void error_parsing_unknown(const std::string &);
    inline void error_parsing_invarg(const std::string &, const std::string &);
    inline void error_parsing_outrng(const std::string &, const std::string &);
    inline void error_parsing_bool(const std::string &, const std::string &);
    inline void error_parsing_char(const std::string &, const std::string &);
public:
    ArgumentParser(std::string description_) : description(description_){};

    template<typename T> void add_argument(const std::string &, const std::string &, const std::string &, const std::string &, const bool &, T, std::string = "");
    template<typename T> void add_argument(const std::string &, const std::string &, const std::string &, const std::string &, const bool &);
    void parse_args(const int &, char *[]);
    template<typename T> void get(const std::string&, T&);
    template<typename T> void get(const std::string&, std::vector<T>&);
    ~ArgumentParser() = default;
};

template<typename T> inline std::string type_name()
{
    typedef typename std::remove_reference<T>::type TR;
    std::unique_ptr<char, void(*)(void*)> own
           (
#ifndef _MSC_VER
                abi::__cxa_demangle(typeid(TR).name(), nullptr,
                                           nullptr, nullptr),
#else
                nullptr,
#endif
                std::free
           );
    std::string r = own != nullptr ? own.get() : typeid(TR).name();
    if (std::is_const<TR>::value)
        r += " const";
    if (std::is_volatile<TR>::value)
        r += " volatile";
    if (std::is_lvalue_reference<T>::value)
        r += "&";
    else if (std::is_rvalue_reference<T>::value)
        r += "&&";
    return r;
}



/**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-***_**
-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**--**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**_**
*_**                                                                                                                                                                                            *_**
*_**    :::::::::::  ::::    ::::   :::::::::   :::         ::::::::::  ::::    ::::   ::::::::::  ::::    :::  :::::::::::      :::      :::::::::::  :::::::::::   ::::::::   ::::    :::     *_**
*_**        :+:      +:+:+: :+:+:+  :+:    :+:  :+:         :+:         +:+:+: :+:+:+  :+:         :+:+:   :+:      :+:        :+: :+:        :+:          :+:      :+:    :+:  :+:+:   :+:     *_**
*_**        +:+      +:+ +:+:+ +:+  +:+    +:+  +:+         +:+         +:+ +:+:+ +:+  +:+         :+:+:+  +:+      +:+       +:+   +:+       +:+          +:+      +:+    +:+  :+:+:+  +:+     *_**
*_**        +#+      +#+  +:+  +#+  +#++:++#+   +#+         +#++:++#    +#+  +:+  +#+  +#++:++#    +#+ +:+ +#+      +#+      +#++:++#++:      +#+          +#+      +#+    +:+  +#+ +:+ +#+     *_**
*_**        +#+      +#+       +#+  +#+         +#+         +#+         +#+       +#+  +#+         +#+  +#+#+#      +#+      +#+     +#+      +#+          +#+      +#+    +#+  +#+  +#+#+#     *_**
*_**        #+#      #+#       #+#  #+#         #+#         #+#         #+#       #+#  #+#         #+#   #+#+#      #+#      #+#     #+#      #+#          #+#      #+#    #+#  #+#   #+#+#     *_**
*_**    ###########  ###       ###  ###         ##########  ##########  ###       ###  ##########  ###    ####      ###      ###     ###      ###      ###########   ########   ###    ####     *_**
*_**                                                                                                                                                                                            *_**
-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**--**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**_**
-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**--**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**_**/





argument::argument(const std::string &name_, const std::string &shr_flag_, const std::string &lng_flag_, const std::string &help_, const bool &required_, const std::string &def_, const std::string &type_)
{
    name = name_;
    shr_flag = shr_flag_;
    lng_flag = lng_flag_;
    help = help_;
    type = type_;
    required = required_;
    def = def_;
}

inline void ArgumentParser::print_help(const int &err_idx)
{
    std::cerr << "Usage: " << program << " ";
    for(auto &arg : args) std::cerr << ((arg.required) ? "-" + arg.shr_flag + " <" + arg.type + ">"  : "[-" + arg.lng_flag + " <" + arg.type + "> ]") << " ";
    std::cerr << std::endl << std::endl;
    std::cerr << description << std::endl << std::endl;
    std::cerr << "optional arguments:" << std::endl;
    for(auto &arg : args) std::cerr << "\t" << std::left << std::setw(6) << "-" + arg.shr_flag + "," << std::left << std::setw(12) <<"--" + arg.lng_flag << "\t\t" + arg.help << std::endl;
    exit(err_idx);
    return;
}

inline void ArgumentParser::error_parsing_type(const std::string &type)
{
  std::cerr << "Error parsing! Wrong type variable: given, '" << type << "'; possible type are 'int', 'float', 'double', 'string', 'char', 'bool'" << std::endl;
  print_help(ERROR_PARSER);
}

inline void ArgumentParser::error_parsing_inputs()
{
  std::cerr << "Error parsing! Not enough inputs in command line." << std::endl;
  print_help(ERROR_PARSER_INPUTS);
}

inline void ArgumentParser::error_parsing_required(const std::string &name)
{
  std::cerr << "Error parsing! Input not found. Variable " << name << " is required but it's not found in command line." << std::endl;
  print_help(ERROR_PARSER_REQUIRED);
}

inline void ArgumentParser::error_parsing_unknown(const std::string &name)
{
  std::cerr << "Error parsing! Variable " << name << " unknown by parser." << std::endl;
  print_help(ERROR_PARSER_UNKNOWN);
}

inline void ArgumentParser::error_parsing_invarg(const std::string &name, const std::string &val)
{
  std::cerr << "Error parsing! Invalid argument for variable " << name << ". Given: " << val <<" ." << std::endl;
  print_help(ERROR_PARSER_INVARG);
}

inline void ArgumentParser::error_parsing_outrng(const std::string &name, const std::string &val)
{
  std::cerr << "Error parsing! Out of range argument for variable " << name << ". Given: " << val <<" ." << std::endl;
  print_help(ERROR_PARSER_OUTRANGE);
}

inline void ArgumentParser::error_parsing_bool(const std::string &name, const std::string &val)
{
  std::cerr << "Error parsing! Invalid argument for bool variable " << name << ". Given: " << val <<" ." << std::endl;
  print_help(ERROR_PARSER_BOOL);
}

inline void ArgumentParser::error_parsing_char(const std::string &name, const std::string &val)
{
  std::cerr << "Error parsing! Invalid argument for char variable " << name << ". Given: " << val <<" ." << std::endl;
  print_help(ERROR_PARSER_CHAR);
}


template<typename T> void ArgumentParser::add_argument(const std::string &name, const std::string &shr_flag, const std::string &lng_flag, const std::string &help, const bool &req)
{
    T def;
    std::string type = type_name<decltype(def)>();
    if(type != "int" && type != "float" && type != "double" && type != "string" && type != "char" && type != "bool") error_parsing_type(type);
    args.push_back(argument(name, shr_flag, lng_flag, help, req, "0", type));
    return;
}

template<> void ArgumentParser::add_argument<std::string>(const std::string &name, const std::string &shr_flag, const std::string &lng_flag, const std::string &help, const bool &req)
{
    std::string type = "string";
    args.push_back(argument(name, shr_flag, lng_flag, help, req, "", type));
    return;
}

template<typename T> void ArgumentParser::add_argument(const std::string &name, const std::string &shr_flag, const std::string &lng_flag, const std::string &help, const bool &req, T def, std::string type)
{
    if(type == "") type = type_name<decltype(def)>();
    if(type != "int" && type != "float" && type != "double" && type != "string" && type != "char" && type != "bool") error_parsing_type(type);
    args.push_back(argument(name, shr_flag, lng_flag, help, req, std::to_string(def), type));
    return;
}

template<> void ArgumentParser::add_argument<std::string>(const std::string &name, const std::string &shr_flag, const std::string &lng_flag, const std::string &help, const bool &req, std::string def, std::string type)
{
    if(type == "") type = "string";
    if(type != "int" && type != "float" && type != "double" && type != "string" && type != "char" && type != "bool") error_parsing_type(type);
    args.push_back(argument(name, shr_flag, lng_flag, help, req, def, type));
    return;
}

template<> void ArgumentParser::add_argument<char>(const std::string &name, const std::string &shr_flag, const std::string &lng_flag, const std::string &help, const bool &req, char def, std::string type)
{
    if(type == "") type = "char";
    if(type != "int" && type != "float" && type != "double" && type != "string" && type != "char" && type != "bool") error_parsing_type(type);
    args.push_back(argument(name, shr_flag, lng_flag, help, req, std::string(1,def), type));
    return;
}

void ArgumentParser::parse_args(const int &argc, char **argv)
{
    program = (std::string)argv[0];
    if((int)args.size() < (argc-1)/2) error_parsing_inputs();

    bool found;
    for(auto &arg : args)
    {
        found = false;
        for(int i = 0; i < argc; ++i)
            if((std::string)argv[i] == "-" + arg.shr_flag || (std::string)argv[i] == "--" + arg.lng_flag)
            {
                arg.value.push_back(argv[i+1]);
                for(int j = i+2; j < argc; ++j)
                {
                    if(argv[j][0] != '-')
                        arg.value.push_back(argv[j]);
                    else
                        break;
                }
                found = true;
                break;
            }
        if(!found)
        {
            if(arg.required) error_parsing_required(arg.name);
            else arg.value.push_back(arg.def);
        }
    }
    return;
}

template<typename T> void ArgumentParser::get(const std::string &name, std::vector<T> &vec)
{
    bool found = true;
    for(int i = 0; i < (int)args.size(); ++i)
        if(args[i].name == name)
        {
            for(auto &v : args[i].value)
            {
              try                            { vec.push_back(static_cast<T>(std::stod(v))); }
              catch(std::invalid_argument &) { error_parsing_invarg(name, v);  }
              catch(std::out_of_range &)     { error_parsing_outrng(name, v);  }
            }
            found = false;
        }
    if(found) error_parsing_unknown(name);
    return;
}

template<> void ArgumentParser::get<std::string>(const std::string &name, std::vector<std::string> &vec)
{
    bool found = true;
    for(int i = 0; i < (int)args.size(); ++i)
        if(args[i].name == name)
        {
            vec = args[i].value;
            found = false;
        }
    if(found) error_parsing_unknown(name);
    return;
}

template<> void ArgumentParser::get<bool>(const std::string &name, std::vector<bool> &vec)
{
    bool found = true;
    for(int i = 0; i < (int)args.size(); ++i)
        if(args[i].name == name)
        {
            for(auto &v : args[i].value)
            {
              try                            {
                                               if (v.length() > 1) error_parsing_bool(name, v);
                                               else{
                                                 int val_  = std::stoi(v);
                                                 if ( val_!= 0 && val_ != 1) error_parsing_bool(name, v);
                                                 else vec.push_back( static_cast<bool>(val_) );
                                               }
                                             }
              catch(std::invalid_argument &) { error_parsing_invarg(name, v);  }
              catch(std::out_of_range &)     { error_parsing_outrng(name, v);  }
            }
            found = false;
        }
    if(found) error_parsing_unknown(name);
    return;
}

template<> void ArgumentParser::get<char>(const std::string &name, std::vector<char> &vec)
{
    bool found = true;
    for(int i = 0; i < (int)args.size(); ++i)
        if(args[i].name == name)
        {
            for(auto &v : args[i].value)
            {
              try                            {
                                                if (v.length() > 1) error_parsing_char(name, v);
                                                else vec.push_back( v.c_str()[0] );
                                             }
              catch(std::invalid_argument &) { error_parsing_invarg(name, v);  }
              catch(std::out_of_range &)     { error_parsing_outrng(name, v);  }
            }
            found = false;
        }
    if(found) error_parsing_unknown(name);
    return;
}

template<typename T> void ArgumentParser::get(const std::string &name, T &val)
{
    for(int i = 0; i < (int)args.size(); ++i)
        if(args[i].name == name)
        {
            try                            { val = static_cast<T>(std::stod(args[i].value[0])); return; }
            catch(std::invalid_argument &) { error_parsing_invarg(name, args[i].value[0]); }
            catch(std::out_of_range &)     { error_parsing_outrng(name, args[i].value[0]); }
        }
    error_parsing_unknown(name);
}

template<> void ArgumentParser::get<std::string>(const std::string &name, std::string &val)
{
    for(int i = 0; i < (int)args.size(); ++i)
        if(args[i].name == name)
        {
            val = args[i].value[0];
            return;
        }
    error_parsing_unknown(name);
}

template<> void ArgumentParser::get<bool>(const std::string &name, bool &val)
{
    for(int i = 0; i < (int)args.size(); ++i)
        if(args[i].name == name)
        {
            try                            {
                                              if (args[i].value[0].length() > 1) error_parsing_bool(name, args[i].value[0]);
                                              else{
                                                int val_  = std::stoi(args[i].value[0]);
                                                if ( val_!= 0 && val_ != 1) error_parsing_bool(name, args[i].value[0]);
                                                else {val = static_cast<bool>(val_); return;}
                                              }
                                            }
            catch(std::invalid_argument &) { error_parsing_invarg(name, args[i].value[0]); }
            catch(std::out_of_range &)     { error_parsing_outrng(name, args[i].value[0]); }
        }
    error_parsing_unknown(name);
}

template<> void ArgumentParser::get<char>(const std::string &name, char &val)
{
    for(int i = 0; i < (int)args.size(); ++i)
        if(args[i].name == name)
        {
            try                            {
                                              std::string val_ = args[i].value[0];
                                              if (val_.length() > 1) error_parsing_char(name, val_);
                                              else{
                                                val = val_.c_str()[0];
                                                return;
                                              }
                                            }
            catch(std::invalid_argument &) { error_parsing_invarg(name, args[i].value[0]); }
            catch(std::out_of_range &)     { error_parsing_outrng(name, args[i].value[0]); }
        }
    error_parsing_unknown(name);
}

#endif // PARSE_ARGS_H
