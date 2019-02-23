#include <parse_args.h>
#include <rfbp.h>

int main(int argc, char *argv[]){

  // SET DESCRIPTION
  ArgumentParser argparser("Focusing Belief Propagation");

  // SET ARGUMENTS
  argparser.add_argument<std::string>("trainfile", "trf", "trainfile", "File with training set", true);
  argparser.add_argument<std::string>("testfile", "tstf", "testfile", "File with test set", true);
  argparser.add_argument<bool>("bin", "b", "bin", "Binary or text files", true);
  argparser.add_argument<int>("K", "k", "K", "Number of hidden nodes", true);
  argparser.add_argument<int>("max_iters", "itrs", "maxiters", "Maximum number of iterations", true);
  argparser.add_argument<int>("max_steps", "stps", "maxsteps", "Maximum number of steps", true);
  argparser.add_argument<int>("seed", "s", "seed", "Seed value", true);
  argparser.add_argument<double>("damping", "d", "damp", "Damping value", true);
  argparser.add_argument<double>("randfact", "rf", "randfact", "Random factor value", true);
  argparser.add_argument<double>("epsil", "e", "epsilon", "Epsilon value", true);
  argparser.add_argument<std::string>("accuracy1", "a1", "accuracy1", "Accuracy choice for first layer", true);
  argparser.add_argument<std::string>("accuracy2", "a2", "accuracy2", "Accuracy choice for second layer", true);
  argparser.add_argument<std::string>("prot", "pr", "protocol", "Protocol name", true);
  argparser.add_argument<std::string>("outfile", "o", "outfile", "Output file", true);
  argparser.add_argument<std::string>("outmessfiletmpl", "om", "outmess", "Output messages file", false);
  argparser.add_argument<std::string>("initmess", "im", "inmess", "Input messages file", false);
  argparser.add_argument<bool>("bin_mess", "bm", "binmess", "Binary or text input messages file", false);

  // PARSE FROM COMMAND LINE
  argparser.parse_args(argc, argv);

  // INPUT VARIABLES DEFINITIONS
  std::string trainfile,
              testfile;

  bool bin;

  long int K        ,
           max_iters,
           max_steps,
           seed     ;

  double damping ,
         randfact,
         epsil   ;

  std::string accuracy1      ,
              accuracy2      ,
              prot           ,
              outfile        ,
              outmessfiletmpl,
              initmess       ;

  bool bin_mess;


  argparser.get("trainfile", trainfile);
  argparser.get("testfile", testfile);
  argparser.get("bin", bin);
  argparser.get("K", K);
  argparser.get("max_iters", max_iters);
  argparser.get("max_steps", max_steps);
  argparser.get("seed", seed);
  argparser.get("damping", damping);
  argparser.get("randfact", randfact);
  argparser.get("epsil", epsil);
  argparser.get("accuracy1", accuracy1);
  argparser.get("accuracy2", accuracy2);
  argparser.get("prot", prot);
  argparser.get("outfile", outfile);
  argparser.get("outmessfiletmpl", outmessfiletmpl);
  argparser.get("initmess", initmess);
  argparser.get("bin_mess", bin_mess);

  FocusingProtocol fprotocol(prot, max_steps);
  Patterns patterns(trainfile, bin);
  focusingBP<MagT64>(K,
             patterns,
             max_iters,
             max_steps,
             seed,
             damping,
             accuracy1,
             accuracy2,
             randfact,
             fprotocol,
             epsil,
             outfile,
             outmessfiletmpl,
             initmess,
             bin_mess);
}
