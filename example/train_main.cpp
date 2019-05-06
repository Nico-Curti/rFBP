#include <cmd.h>
#include <rfbp.h>

int main (int argc, char *argv[])
{
  bool bin,
       binmess;
  int  nth;
  char mag;
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
                     nth,
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

  long int ** bin_weights = nullptr;

  switch (mag)
  {
    case magP:
        bin_weights = focusingBP < MagP64 >(nth,
                                            K,
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
    case magT:
        bin_weights = focusingBP < MagT64 >(nth,
                                            K,
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

  auto predicted_labels = nonbayes_test(bin_weights, patterns, K);
  scorer score;

#ifdef _OPENMP
#pragma omp parallel shared(score) num_threads(nth)
  {
#endif
    score.compute_score(reinterpret_cast<int*>(patterns.output), reinterpret_cast<int*>(predicted_labels), patterns.Nrow, patterns.Nrow);
#ifdef _OPENMP
  }
#endif

#ifdef VERBOSE
  score.print();
#endif

  for (int i = 0; i < K; ++i) delete[] bin_weights[i];
  delete[] bin_weights;
  delete[] predicted_labels;

  return 0;
}
