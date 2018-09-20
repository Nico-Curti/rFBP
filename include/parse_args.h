#ifndef PARSE_ARGS_H
#define PARSE_ARGS_H
#include <vector>
#include <type_traits>
#include <memory>
#include <string>
#ifndef _MSC_VER
#include <cxxabi.h>
#endif

class argument
{
public:
    argument(const std::string &, const std::string &, const std::string &, const std::string &, const bool &, const std::string &, const std::string &);
    std::string name, shr_flag, lng_flag, help, type, def;
    std::vector<std::string> value;
    bool required;
    ~argument() = default;
};

class ArgumentParser
{
    std::vector<argument> args;
    std::string description, program;
    inline void print_help();
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

template<typename T> void ArgumentParser::add_argument(const std::string &name, const std::string &shr_flag, const std::string &lng_flag, const std::string &help, const bool &req)
{
    T def;
    std::string type = type_name<decltype(def)>();
    if(type != "int" && type != "float" && type != "double" && type != "string" && type != "char" && type != "bool"){ std::cerr << "Error parsing! Wrong type variable: given, '" << type << "'; possible type are 'int', 'float', 'double', 'string', 'char', 'bool'" << std::endl; print_help(); }
    args.push_back(argument(name, shr_flag, lng_flag, help, req, "", type));
    return;
}
template<> void ArgumentParser::add_argument<std::string>(const std::string &name, const std::string &shr_flag, const std::string &lng_flag, const std::string &help, const bool &req)
{
    std::string type = "string";
    //if(type != "int" && type != "float" && type != "double" && type != "string" && type != "char" && type != "bool"){ std::cerr << "Error parsing! Wrong type variable: given, '" << type << "'; possible type are 'int', 'float', 'double', 'string', 'char', 'bool'" << std::endl; print_help(); }
    args.push_back(argument(name, shr_flag, lng_flag, help, req, "", type));
    return;
}

template<typename T> void ArgumentParser::add_argument(const std::string &name, const std::string &shr_flag, const std::string &lng_flag, const std::string &help, const bool &req, T def, std::string type)
{
    if(type == "") type = type_name<decltype(def)>();
    if(type != "int" && type != "float" && type != "double" && type != "string" && type != "char" && type != "bool"){ std::cerr << "Error parsing! Wrong type variable: given, '" << type << "'; possible type are 'int', 'float', 'double', 'string', 'char', 'bool'" << std::endl; print_help(); }
    args.push_back(argument(name, shr_flag, lng_flag, help, req, std::to_string(def), type));
    return;
}
template<> void ArgumentParser::add_argument<std::string>(const std::string &name, const std::string &shr_flag, const std::string &lng_flag, const std::string &help, const bool &req, std::string def, std::string type)
{
    if(type == "") type = "string";
    if(type != "int" && type != "float" && type != "double" && type != "string" && type != "char" && type != "bool"){ std::cerr << "Error parsing! Wrong type variable: given, '" << type << "'; possible type are 'int', 'float', 'double', 'string', 'char', 'bool'" << std::endl; print_help(); }
    args.push_back(argument(name, shr_flag, lng_flag, help, req, def, type));
    return;
}

inline void ArgumentParser::print_help()
{
    std::cerr << "Usage: " << program << " ";
    for(auto &arg : args) std::cerr << ((arg.required) ? "-" + arg.shr_flag + " <" + arg.type + ">"  : "[-" + arg.lng_flag + " <" + arg.type + "> ]") << " ";
    std::cerr << std::endl << std::endl;
    std::cerr << description << std::endl << std::endl;
    std::cerr << "optional arguments:" << std::endl;
    for(auto &arg : args) std::cerr << "\t-" + arg.shr_flag + ", --" + arg.lng_flag + "\t\t" + arg.help << std::endl;
    exit(1);
    return;
}

void ArgumentParser::parse_args(const int &argc, char **argv)
{
    program = (std::string)argv[0];
    if((int)args.size() < (argc-1)/2) {std::cerr << "Error parsing! Not enough inputs in command line." << std::endl; print_help();}

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
            if(arg.required)
            {
                std::cerr << "Error parsing! Input not found. Variable " << arg.name << " is required but it's not found in command line." << std::endl;
                print_help();
            }
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
                vec.push_back((T)std::stod(v));
            found = false;
        }
    if(found){std::cerr << "Error parsing! Variable " << name << " unknown by parser." << std::endl; print_help();}
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
    if(found){std::cerr << "Error parsing! Variable " << name << " unknown by parser." << std::endl; print_help();}
    return;
}
template<typename T> void ArgumentParser::get(const std::string &name, T &val)
{
    for(int i = 0; i < (int)args.size(); ++i)
        if(args[i].name == name)
        {
            val = (T)std::stod(args[i].value[0]);
            return;
        }
    std::cerr << "Error parsing! Variable " << name << " unknown by parser." << std::endl;
    print_help();
}
template<> void ArgumentParser::get<std::string>(const std::string &name, std::string &val)
{
    for(int i = 0; i < (int)args.size(); ++i)
        if(args[i].name == name)
        {
            val = args[i].value[0];
            return;
        }
    std::cerr << "Error parsing! Variable " << name << " unknown by parser." << std::endl;
    print_help();
}


#endif // PARSE_ARGS_H