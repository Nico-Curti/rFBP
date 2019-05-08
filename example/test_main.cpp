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

  std :: unique_ptr < int[] > temp_labels(  new int [patterns.Nrow] );
  std :: unique_ptr < int[] > temp_predict( new int [patterns.Nrow] );

  scorer score;
#ifdef _OPENMP
#pragma omp parallel shared(score) num_threads(nth)
  {
#endif

    predicted_labels = nonbayes_test(bin_weights, patterns, messages.K);

#ifdef _OPENMP
#pragma omp sections
    {
#endif

#ifdef _OPENMP
#pragma omp section
#endif
      std :: transform( patterns.output, patterns.output + patterns.Nrow,
                        temp_labels.get(), [](const long int & p){return static_cast < int >(p);});

#ifdef _OPENMP
#pragma omp section
#endif
      std :: transform( predicted_labels, predicted_labels + patterns.Nrow,
                        temp_predict.get(), [](const long int & p){return static_cast < int >(p);});

#ifdef _OPENMP
    }
#endif

    score.compute_score( temp_labels.get(), temp_predict.get(), patterns.Nrow, patterns.Nrow);

#ifdef _OPENMP
  }
#endif

  if ( !output_file.empty() ) score.dump(output_file);
  else score.print();

  for (long int i = 0L; i < messages.K; ++i) delete[] bin_weights[i];
  delete[] bin_weights;
  delete[] predicted_labels;

  return 0;
}
