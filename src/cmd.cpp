#include <cmd.h>

void parse_training_fbp(int argc, char *argv[],
                        std :: string & patternsfile,
                        std :: string & output,
                        bool & bin,
                        std :: string & del,
                        long int & K,
                        long int & max_iters,
                        long int & seed,
                        double & randfact,
                        double & damping,
                        std :: string & accuracy1,
                        std :: string & accuracy2,
                        std :: string & fprotocol,
                        double & epsil,
                        int & nth,
                        long int & max_steps,
                        int & mag,
                        std :: string & inmess,
                        std :: string & outmess,
                        std :: string & delmess,
                        bool & binmess)
{
  ArgumentParser argparse("Training BeliefPropagation " + std :: to_string(MAJOR) + "." + std :: to_string(MINOR) + "." + std :: to_string(REVISION));

#ifdef _OPENMP

  nth  = omp_get_max_threads();
  nth -= nth % 2;
  argparse.add_argument < int >(          "tArg",  "t",  "threads",     "Max number of threads exploitable",                      false, nth);

#else

  nth = 1;

#endif

  argparse.add_argument < std :: string >("fArg",  "f",  "file",        "Pattern Filename (with extension)",                      true, "");
  argparse.add_argument < std :: string >("oArg",  "o",  "output",      "Output Filename (with extension)",                       false, ""); // TODO: set true!
  argparse.add_argument < bool >(         "bArg",  "b",  "bin",         "File format: "
                                                                        "(0) Textfile(default), "
                                                                        "(1) Binary",                                             false, false);
  argparse.add_argument < std :: string >("dlArg", "dl", "delimiter",   "Delimiter for text files(default: \"\\t\")",             false, "\t");
  argparse.add_argument < int >(          "kArg",  "k",  "hidden",      "Number of Hidden Layers(default:3)",                     false, 3);
  argparse.add_argument < int >(          "iArg",  "i",  "iteration",   "Max Number of Iterations(default: 1000)",                false, 1000);
  argparse.add_argument < int >(          "rArg",  "r",  "seed",        "Seed random generator(default: 135)",                    false, 135);
  argparse.add_argument < double >(       "gArg",  "g",  "randfact",    "Seed random generator of Cavity Messages(default: 0.1)", false, .1);
  argparse.add_argument < double >(       "dArg",  "d",  "damping",     "Damping parameter(default: 0.5)",                        false, .5);
  argparse.add_argument < std :: string >("aArg",  "a",  "accuracy",    "Accuracy of the messages computation "
                                                                        "at the hidden units level (choose between "
                                                                        "'exact'(default), 'accurate', 'approx', 'none')",        false, "exact"); // TODO: set true!
  argparse.add_argument < std :: string >("pArg",  "p", "protocol",     "Specify protocol : "
                                                                        "scoping, "
                                                                        "pseudo_reinforcement (default), "
                                                                        "free_scoping, "
                                                                        "standard_reinforcement",                                 false, "pseudo_reinforcement"); // TODO: set true!
  argparse.add_argument < double >(       "eArg",  "e", "epsilon",      "Threshold for convergence(default: 0.1)",                false, 0.1);
  argparse.add_argument < int >(          "sArg",  "s", "steps",        "Max Number of Steps for chosen protocol(default: 101)",  false, 101);
  argparse.add_argument < int >(          "mArg",  "m", "mag",          "Specify Magnetization: "
                                                                        "(0) MagnetizationP (MagP64), "
                                                                        "(1) MagnetizationT (MagT64)",                            false, 1); // TODO: set true!
  argparse.add_argument < std :: string >("imArg", "im", "inmess",      "Input Messages file",                                    false, "");
  argparse.add_argument < std :: string >("omArg", "om", "outmess",     "Output Messages file",                                   false, "");
  argparse.add_argument < std :: string >("dmArg", "dm", "delmess",     "Delimiter for Messages files(default: \"\\t\")",         false, "\t");
  argparse.add_argument < bool >(         "bmArg", "bm", "binmess",     "Messages files format: "
                                                                        "(0) Textfile(default), "
                                                                        "(1) Binary",                                             false, false);

  argparse.parse_args(argc, argv);

  std :: vector < std :: string > accuracy;

#ifdef _OPENMP

  argparse.get < int >(                            "tArg",  nth);

#endif
  argparse.get < std :: string >(                  "fArg",  patternsfile);
  argparse.get < std :: string >(                  "oArg",  output);
  argparse.get < bool >(                           "bArg",  bin);
  argparse.get < std :: string >(                  "dlArg", del);
  argparse.get < long int >(                       "kArg",  K);
  argparse.get < long int >(                       "iArg",  max_iters);
  argparse.get < long int >(                       "rArg",  seed);
  argparse.get < double >(                         "gArg",  randfact);
  argparse.get < double >(                         "dArg",  damping);
  argparse.get < std :: vector < std :: string > >("aArg",  accuracy);
  argparse.get < std :: string >(                  "pArg",  fprotocol);
  argparse.get < double >(                         "eArg",  epsil);
  argparse.get < long int >(                       "sArg",  max_steps);
  argparse.get < int >(                            "mArg",  mag);
  argparse.get < std :: string >(                  "imArg", inmess);
  argparse.get < std :: string >(                  "omArg", outmess);
  argparse.get < std :: string >(                  "dmArg", delmess);
  argparse.get < bool >(                           "bmArg", binmess);

  if( !file_exists(patternsfile) ) error_pattern(patternsfile);

  if ( static_cast < int >(accuracy.size()) > 2 ) error_num_accuracy();

  for (auto & ac : accuracy)
    if(ac != "exact" && ac != "accurate" && ac != "approx" && ac != "none") error_accuracy(ac);

  if (static_cast < int >(accuracy.size()) == 1 )
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

  if (fprotocol != "scoping" && fprotocol != "pseudo_reinforcement" && fprotocol != "free_scoping" && fprotocol != "standard_reinforcement") error_protocol(fprotocol);

  if (mag != 0 && mag != 1) error_magnetization(mag);

  return;
}

void parse_test_args(int argc, char *argv[],
                     std :: string & patternsfile,
                     std :: string & del,
                     bool & bin,
                     std :: string & weight_file,
                     std :: string & output_file,
                     int & nth)
{
  ArgumentParser argparse("Test BeliefPropagation " + std :: to_string(MAJOR) + "." + std :: to_string(MINOR) + "." + std :: to_string(REVISION));

#ifdef _OPENMP

  nth  = omp_get_max_threads();
  nth -= nth % 2;
  argparse.add_argument < int >(          "tArg",  "t",  "threads",     "Max number of threads exploitable",                      false, nth);

#else

  nth = 1;

#endif

  argparse.add_argument < std :: string >("fArg",  "f",  "file",        "Pattern Filename (with extension)",                      true,  "");
  argparse.add_argument < bool >(         "bArg",  "b",  "bin",         "File format: "
                                                                        "(0) Textfile(default), "
                                                                        "(1) Binary",                                             false, false);
  argparse.add_argument < std :: string >("wArg",  "w",  "weights",     "Weights Matrix Filename (with extension)",               true,  "");
  argparse.add_argument < std :: string >("dlArg", "dl", "delimiter",   "Delimiter for text files(default: \"\\t\")",             false, "\t");
  argparse.add_argument < std :: string >("oArg",  "o",  "output",      "Output Filename (no extension)",                         false, "");
  argparse.parse_args(argc, argv);

  argparse.get < std :: string >("fArg",  patternsfile);
  argparse.get < bool >(         "bArg",  bin);
  argparse.get < std :: string >("wArg",  weight_file);
  argparse.get < std :: string >("dlArg", del);
  argparse.get < std :: string >("oArg",  output_file);

  if ( !file_exists(patternsfile) ) error_pattern(patternsfile);
  if ( !file_exists(weight_file)  ) error_message_weights(weight_file);

#ifdef _OPENMP

  argparse.get < int >(          "tArg",  nth);

#endif

  return;
}
