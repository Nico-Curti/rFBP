#include <cmd.h>
#include <rfbp.hpp>

#if (defined __clang__ || (!defined __clang__ && __GNUC__ > 4)) && defined WITH_SCORER
#define USE_SCORER

  #include <scorer.h>

#endif


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

#ifdef USE_SCORER

  scorer score;

#endif

#ifdef _OPENMP
#ifdef USE_SCORER
#pragma omp parallel shared (score) num_threads (nth)
#else
#pragma omp parallel num_threads (nth)
#endif
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

#ifdef USE_SCORER

    score.compute_score( temp_labels.get(), temp_predict.get(), patterns.Nrow, patterns.Nrow);

#endif

#ifdef _OPENMP
  }
#endif

#ifdef USE_SCORER

  if ( !output_file.empty() ) score.dump(output_file);
  else score.print();

#else

  std :: cout << "true_label\tpredict_label" << std :: endl;
  for (long int i = 0L; i < patterns.Nrow; ++i)
    std :: cout << patterns.output[i] << "\t" << temp_predict[i] << std :: endl;

#endif

  for (long int i = 0L; i < messages.K; ++i) delete[] bin_weights[i];
  delete[] bin_weights;
  delete[] predicted_labels;

  return 0;
}
