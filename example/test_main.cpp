#include <cmd.h>
#include <rfbp.h>

int main (int argc, char *argv[])
{
  bool bin;
  int  nth;
  std :: string patternsfile,
                del,
                weight_file,
                output_file;

  parse_test_args(argc,
                  argv,
                  patternsfile,
                  del,
                  bin,
                  weight_file,
                  output_file,
                  nth);

  Patterns patterns(patternsfile, bin, del);

  Cavity_Message < MagP64 > messages(weight_file, bin);
  long int ** bin_weights = messages.get_weights();
  long int * predicted_labels = nullptr;

  scorer score;
#ifdef _OPENMP
#pragma omp parallel shared(score) num_threads(nth)
  {
#endif
    predicted_labels = nonbayes_test(bin_weights, patterns, messages.K);
    score.compute_score(reinterpret_cast<int*>(patterns.output), reinterpret_cast<int*>(predicted_labels), patterns.Nrow, patterns.Nrow);
#ifdef _OPENMP
  }
#endif

  if ( !output_file.empty() ) score.dump(output_file);

  for (long int i = 0L; i < messages.K; ++i) delete[] bin_weights[i];
  delete[] bin_weights;
  delete[] predicted_labels;

  return 0;
}
