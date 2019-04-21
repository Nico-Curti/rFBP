#include <cmd.h>
#include <rfbp.h>

int main(int argc, char *argv[])
{

  bool bin,
       binmess;
  char mag;
  long int K,
           max_iters,
           seed,
           max_steps;
  double randfact,
         damping,
         epsil;
  std::string patternsfile,
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
                     max_steps,
                     mag,
                     inmess,
                     outmess,
                     delmess,
                     binmess);

  FocusingProtocol fp(fprotocol, max_steps);
  Patterns patterns(patternsfile, bin, del);

  long int **bin_weights = nullptr;

  switch (mag)
  {
    case magP:
        bin_weights = focusingBP<MagP64>(K,
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
        bin_weights = focusingBP<MagT64>(K,
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
  //int *true_labels = new int[patterns.Nrow];
  //int *pred_labels = new int[patterns.Nrow];
  //std::transform(patterns.output,  patterns.output  + patterns.Nrow, true_labels, [](long int &i){ return static_cast<int>(i); } );
  //std::transform(predicted_labels, predicted_labels + patterns.Nrow, pred_labels, [](long int &i){ return static_cast<int>(i); } );
#ifdef _OPENMP
#pragma omp parallel shared(score)
  {
#endif
    score.compute_score(reinterpret_cast<int*>(patterns.output), reinterpret_cast<int*>(predicted_labels), patterns.Nrow, patterns.Nrow);
#ifdef _OPENMP
  }
#endif
  // score.print();

  for (int i = 0; i < K; ++i) delete[] bin_weights[i];
  delete[] bin_weights;
  delete[] predicted_labels;

  return 0;
}
