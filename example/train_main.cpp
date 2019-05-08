#include <cmd.h>
#include <rfbp.h>

int main (int argc, char *argv[])
{
  bool bin,
       binmess;
  int  nth,
       mag;
  long int K,
           max_iters,
           seed,
           max_steps;
  double randfact,
         damping,
         epsil;
  std :: string patternsfile,
                outfile,
                del,
                accuracy1,
                accuracy2,
                fprotocol,
                inmess,
                outmess,
                delmess;

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
                     nth,
                     max_steps,
                     mag,
                     inmess,
                     outmess,
                     delmess,
                     binmess);

  FocusingProtocol fp(fprotocol, max_steps);
  Patterns patterns(patternsfile, bin, del);

  long int ** bin_weights = nullptr;

  switch (mag)
  {
    case magP:
        bin_weights = focusingBP < MagP64 >(K,
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
                                            nth,
                                            outfile,
                                            outmess,
                                            inmess,
                                            binmess);
      break;
    case magT:
        bin_weights = focusingBP < MagT64 >(K,
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
                                            nth,
                                            outfile,
                                            outmess,
                                            inmess,
                                            binmess);
      break;
      default: error_magnetization(mag);
      break;
  }

  for (int i = 0; i < K; ++i) delete[] bin_weights[i];
  delete[] bin_weights;

  return 0;
}
