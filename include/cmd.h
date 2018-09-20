#ifndef CMD_H
#define CMD_H
#include <parse_args.h>
#include <filesystem>

void parse_training_fbp(int argc, char *argv[],
                        std::string &patternsfile,
                        std::string &output,
                        int &K,
                        int &seed,
                        int &max_iters,
                        double &randfact,
                        double &damping,
                        std::string &accuracy1,
                        std::string &accuracy2,
                        std::string &fprotocol,
                        char &mag)
{
  ArgumentParser argparse("Training BeliefPropagation 4.0");
  argparse.add_argument<std::string>("fArg", "f", "file",      "Pattern Filename (with extension)",        true, "");
  argparse.add_argument<std::string>("oArg", "o", "output",    "Output Filename (with extension)",         true, "");
  argparse.add_argument<int>(        "kArg", "k", "hidden",    "Number of Hidden Layers",                  false, 3);
  argparse.add_argument<int>(        "rArg", "r", "seed",      "Seed random generator",                    false, 135);
  argparse.add_argument<int>(        "iArg", "i", "iteration", "Max Number of Iterations",                 false, 1000);
  argparse.add_argument<double>(     "gArg", "g", "randfact",  "Seed random generator of Cavity Messages", false, .1);
  argparse.add_argument<double>(     "dArg", "d", "damping",   "Damping parameter",                        false, .5);
  argparse.add_argument<std::string>("aArg", "a", "accuracy",  "Accuracy of the messages computation "
                                                               "at the hidden units level (choose between "
                                                               "'exact', 'accurate', 'approx', 'none')",   true, "");
  argparse.add_argument<std::string>("pArg", "p", "protocol",  "Specify protocol : "
                                                               "(0) Scooping, "
                                                               "(1) PseudoReinforcement, "
                                                               "(2) FreeScoping, "
                                                               "(3) StandardReinforcement",                true);
  argparse.add_argument<char>(       "mArg", "m", "mag",       "Specify Magnetization: "
                                                               "(0) MagnetizationP (MagP64), "
                                                               "(1) MagnetizationT (MagT64)",              true);

  argparse.parse_args(argc, argv);

  argparse.get<std::string>("fArg", patternsfile);
  if(!std::filesystem::exists(std::filesystem::path(patternsfile))) error_pattern(patternsfile);
  argparse.get<std::string>("oArg", output);
  argparse.get<int>(        "kArg", K);
  argparse.get<int>(        "rArg", seed);
  argparse.get<int>(        "iArg", max_iters);
  argparse.get<double>(     "gArg", randfact);
  argparse.get<double>(     "dArg", damping);

  std::vector<std::string> accuracy;
  argparse.get<std::string>("aArg", accuracy);
  if(static_cast<int>(accuracy.size()) > 2)
  {
    std::cerr << "Too many accuracy variables given. Needed two." << std::endl;
    std::exit(11);
  }

  for(auto &ac : accuracy)
    if(ac != "exact" && ac != "accurate" && ac != "approx" && ac != "none")
    {
      std::cerr << "Invalid accuracy variable given. Given : " << ac << std::endl;
      std::exit(12);
    }

  if(static_cast<int>(accuracy.size()) == 1)
  {
    accuracy1 = accuracy[0];
    accuracy2 = "accurate";
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
    std::exit(13);
  }

  argparse.get<char>("mArg", mag);
  if(mag != '0' && mag != '1')
  {
    std::cerr << "Invalid magnetization found. Given : " << mag << std::endl;
    std::exit(14);
  };

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
