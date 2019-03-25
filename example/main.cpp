#include <cmd.h>
#include <rfbp.h>

int main(int argc, char *argv[])
{

  std::string patternsfile,
              outfile,
              del,
              accuracy1,
              accuracy2,
              fprotocol,
              inmess,
              outmess,
              delmess;
  bool bin,
       binmess;
  long int K,
           max_iters,
           seed,
           max_steps;
  double randfact,
         damping,
         epsil;
  char mag;

  parse_training_fbp(argc, argv,
                     patternsfile,
                     outfile,
                     bin,
                     del,
                     K,
                     max_iters,
                     seed,
                     randfact,
                     damping,
                     accuracy1,
                     accuracy2,
                     fprotocol,
                     epsil,
                     max_steps,
                     mag,
                     inmess,
                     outmess,
                     delmess,
                     binmess);

  FocusingProtocol fp(fprotocol, max_steps);
  Patterns patterns(patternsfile, bin, del);

  switch (mag) {
    case '0':
      {
        // std::cout << "MagP64" << std::endl;
        focusingBP<MagP64>(K,
               patterns,
               max_iters,
               max_steps,
               seed,
               damping,
               accuracy1,
               accuracy2,
               randfact,
               fp,
               epsil,
               outfile,
               outmess,
               inmess,
               binmess);
        break;
      }
    case '1':
      {
        // std::cout << "MagT64" << std::endl;
        focusingBP<MagT64>(K,
               patterns,
               max_iters,
               max_steps,
               seed,
               damping,
               accuracy1,
               accuracy2,
               randfact,
               fp,
               epsil,
               outfile,
               outmess,
               inmess,
               binmess);
        break;
      }
  }

  return 0;
}
