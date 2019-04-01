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
  long int **bin_weights = (mag=='0')?
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
                                               binmess) :
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
  std::unique_ptr<long int[]> predicted_labels = nonbayes_test(bin_weights, patterns, K);
  scorer score;
  int *true_labels = new int[patterns.Nrow];
  int *pred_labels = new int[patterns.Nrow];
  std::transform(patterns.output.get(),  patterns.output.get()  + patterns.Nrow, true_labels, [](long int &i){ return static_cast<int>(i); } );
  std::transform(predicted_labels.get(), predicted_labels.get() + patterns.Nrow, pred_labels, [](long int &i){ return static_cast<int>(i); } );
#ifdef _OPENMP
#pragma omp parallel shared(score)
  {
#endif
    score.compute_score(true_labels, pred_labels, patterns.Nrow, patterns.Nrow);
#ifdef _OPENMP
  }
#endif
  // score.print();

  return 0;
}
