#ifndef CMD_H
#define CMD_H
#include <parse_args.h>
#include <filesystem>
#include <errors.h>

void parse_training_fbp(int argc, char *argv[],
                        std::string &patternsfile,
                        std::string &output,
                        bool &bin,
                        std::string &del,
                        long int &K,
                        long int &max_iters,
                        long int &seed,
                        double &randfact,
                        double &damping,
                        std::string &accuracy1,
                        std::string &accuracy2,
                        std::string &fprotocol,
                        double &epsil,
                        long int &max_steps,
                        char &mag,
                        std::string &inmess,
                        std::string &outmess,
                        std::string &delmess,
                        bool &binmess)
{
  ArgumentParser argparse("Training BeliefPropagation 4.0");

  argparse.add_argument<std::string>("fArg",  "f",  "file",        "Pattern Filename (with extension)",                      true, "");
  argparse.add_argument<std::string>("oArg",  "o",  "output",      "Output Filename (with extension)",                       false, ""); // TODO: set true!
  argparse.add_argument<char>(       "bArg",  "b",  "bin",         "File format: "
                                                                   "(0) Textfile(default), "
                                                                   "(1) Binary",                                             false, '0');
  argparse.add_argument<std::string>("dlArg", "dl", "delimiter",   "Delimiter for text files(default: \"\\t\")",             false, "\t");
  argparse.add_argument<int>(        "kArg",  "k",  "hidden",      "Number of Hidden Layers(default:3)",                     false, 3);
  argparse.add_argument<int>(        "iArg",  "i",  "iteration",   "Max Number of Iterations(default: 1000)",                false, 1000);
  argparse.add_argument<int>(        "rArg",  "r",  "seed",        "Seed random generator(default: 135)",                    false, 135);
  argparse.add_argument<double>(     "gArg",  "g",  "randfact",    "Seed random generator of Cavity Messages(default: 0.1)", false, .1);
  argparse.add_argument<double>(     "dArg",  "d",  "damping",     "Damping parameter(default: 0.5)",                        false, .5);
  argparse.add_argument<std::string>("aArg",  "a",  "accuracy",    "Accuracy of the messages computation "
                                                                   "at the hidden units level (choose between "
                                                                   "'exact'(default), 'accurate', 'approx', 'none')",                 false, "exact"); // TODO: set true!
  argparse.add_argument<std::string>("pArg",  "p", "protocol",     "Specify protocol : "
                                                                   "(0) Scooping, "
                                                                   "(1) PseudoReinforcement(default), "
                                                                   "(2) FreeScoping, "
                                                                   "(3) StandardReinforcement",                              false, "pseudo_reinforcement"); // TODO: set true!
  argparse.add_argument<double>(     "eArg",  "e", "epsilon",      "Threshold for convergence(default: 0.1)",               false, 0.01);
  argparse.add_argument<int>(        "sArg",  "s", "steps",        "Max Number of Steps for chosen protocol(default: 101)",  false, 101);
  argparse.add_argument<char>(       "mArg",  "m", "mag",          "Specify Magnetization: "
                                                                   "(0) MagnetizationP (MagP64), "
                                                                   "(1) MagnetizationT (MagT64)",                            false, '1'); // TODO: set true!
  argparse.add_argument<std::string>("imArg", "im", "inmess",      "Input Messages file",                                    false, "");
  argparse.add_argument<std::string>("omArg", "om", "outmess",     "Output Messages file",                                   false, "");
  argparse.add_argument<std::string>("dmArg", "dm", "delmess",     "Delimiter for Messages files(default: \"\\t\")",         false, "\t");
  argparse.add_argument<char>(       "bmArg", "bm", "binmess",     "Messages files format: "
                                                                   "(0) Textfile(default), "
                                                                   "(1) Binary",                                             false, '0');

  argparse.parse_args(argc, argv);

  argparse.get<std::string>("fArg",  patternsfile);
  if(!std::filesystem::exists(std::filesystem::path(patternsfile))) error_pattern(patternsfile);
  argparse.get<std::string>("oArg",  output);
  char b;
  argparse.get<char>("bArg",  b);
  if(b != '0' && b != '1')
  {
    std::cerr << "Invalid format files option found. Given : " << b << std::endl;
    std::exit(11);
  };
  bin = (b!='0') ? true : false;
  argparse.get<std::string>("dlArg", del);
  argparse.get<long int>("kArg",  K);
  argparse.get<long int>("iArg",  max_iters);
  argparse.get<long int>("rArg",  seed);
  argparse.get<double>("gArg",  randfact);
  argparse.get<double>("dArg",  damping);

  std::vector<std::string> accuracy;
  argparse.get<std::string>("aArg", accuracy);
  if(static_cast<int>(accuracy.size()) > 2)
  {
    std::cerr << "Too many accuracy variables given. Needed two." << std::endl;
    std::exit(12);
  }

  for(auto &ac : accuracy)
    if(ac != "exact" && ac != "accurate" && ac != "approx" && ac != "none")
    {
      std::cerr << "Invalid accuracy variable given. Given : " << ac << std::endl;
      std::exit(13);
    }

  if(static_cast<int>(accuracy.size()) == 1)
  {
    accuracy1 = accuracy[0];
    accuracy2 = accuracy[0];
  }
  else
  {
    accuracy1 = accuracy[0];
    accuracy2 = accuracy[1];
  }
  accuracy.clear();

  argparse.get<std::string>("pArg", fprotocol);
  if (fprotocol != "scoping" && fprotocol != "pseudo_reinforcement" && fprotocol != "free_scoping" && fprotocol != "standard_reinforcement")
  {
    std::cerr << "Invalid focusing protocol found. Given : " << fprotocol << std::endl;
    std::exit(14);
  }

  argparse.get<double>("eArg", epsil);
  argparse.get<long int>("sArg", max_steps);

  argparse.get<char>("mArg", mag);
  if(mag != '0' && mag != '1')
  {
    std::cerr << "Invalid magnetization found. Given : " << mag << std::endl;
    std::exit(15);
  };

  argparse.get<std::string>("imArg", inmess);
  argparse.get<std::string>("omArg", outmess);
  argparse.get<std::string>("dmArg", delmess);
  argparse.get<char>("bmArg", b);
  if(b != '0' && b != '1')
  {
    std::cerr << "Invalid format messages option found. Given : " << b << std::endl;
    std::exit(16);
  };
  binmess = (b!='0') ? true : false;

  return;
}

void parse_test_args(int argc, char *argv[],
                     std::string &test_file,
                     std::string &weight_file,
                     std::string &output_file)
{
  ArgumentParser argparse("Test BeliefPropagation 4.0");
  argparse.add_argument<std::string>("fArg", "f", "test_file", "Test Filename (with extension)",           true, "");
  argparse.add_argument<std::string>("wArg", "w", "weights",   "Weights Matrix Filename (with extension)", true, "");
  argparse.add_argument<std::string>("oArg", "o", "output",    "Output Filename (with extension)",         false, "output.txt");

  argparse.parse_args(argc, argv);

  argparse.get<std::string>("fArg", test_file);
  if(!std::filesystem::exists(std::filesystem::path(test_file)))   error_pattern(test_file);
  argparse.get<std::string>("wArg", weight_file);
  if(!std::filesystem::exists(std::filesystem::path(weight_file))) error_message_weights(weight_file);
  argparse.get<std::string>("oArg", output_file);
  return;
}


#endif // CMD_H
