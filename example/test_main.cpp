#include <cmd.h>
#include <rfbp.h>

int main(int argc, char *argv[])
{
  bool bin;
  std::string patternsfile,
              del,
              weight_file,
              output_file;

  parse_test_args(argc,
                  argv,
                  patternsfile,
                  del,
                  bin,
                  weight_file,
                  output_file);

  Patterns patterns(patternsfile, bin, del);

  Cavity_Message<MagP64> messages(weight_file, bin);
  long int **bin_weights = messages.get_weights();

  auto predicted_labels = nonbayes_test(bin_weights, patterns, messages.K);
//  auto score_fbp = score::perfs(patterns.output.get(), predicted_labels.get(), patterns.Nrow);
//#ifdef VERBOSE
//  std::cout << "fBP prediction on test set:" << std::endl;
//  for(const auto &i : score_fbp)
//    std::cout << i.first << " : " << i.second << std::endl;
//  auto accuracy_fbp = score::accuracy(patterns.output.get(), predicted_labels.get(), patterns.Nrow);
//  std::cout << "fBP accuracy on test set: "<< static_cast<double>(accuracy_fbp)/static_cast<double>(patterns.Nrow)*100L << "%" << std::endl;
//  auto cm_fbp = score::confusion_matrix(patterns.output.get(), predicted_labels.get(), patterns.Nrow);
//  std::cout << "fBP confusion matrix on test set: "<< std::endl;
//  std::cout << std::right << std::setw(8) << "1" << std::right << std::setw(5) << "-1" << std::endl;
//  for(int i=0; i< 2; ++i)
//    std::cout << std::right << std::setw(3) << 1-2*i << std::right << std::setw(5) << cm_fbp[i][0] << std::right << std::setw(5) << cm_fbp[i][1] << std::endl;
//#endif
  scorer score;
//  int *true_labels = new int[patterns.Nrow],
//      *pred_labels = new int[patterns.Nrow];
//  std::transform(patterns.output,  patterns.output  + patterns.Nrow, true_labels, [](long int &i){ return static_cast<int>(i); } );
//  std::transform(predicted_labels, predicted_labels + patterns.Nrow, pred_labels, [](long int &i){ return static_cast<int>(i); } );
#ifdef _OPENMP
#pragma omp parallel shared(score)
  {
#endif
    score.compute_score(reinterpret_cast<int*>(patterns.output), reinterpret_cast<int*>(predicted_labels), patterns.Nrow, patterns.Nrow);
#ifdef _OPENMP
  }
#endif

//#if VERBOSE
//  score.print();
//#endif

  if (output_file!="") score.dump(output_file);


  for (int i = 0; i < messages.K; ++i) delete[] bin_weights[i];
  delete[] bin_weights;
  delete[] predicted_labels;

  return 0;
}
