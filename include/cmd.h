#ifndef CMD_H
#define CMD_H
#include <utils.h>
#include <parse_args.h>
#include <errors.h>

void parse_training_fbp (int argc, char *argv[], int & nth, std :: string & patternsfile, std :: string & output, bool & bin, std :: string & del, long int & K, long int & max_iters, long int & seed, double & randfact, double & damping, std :: string & accuracy1, std :: string & accuracy2, std :: string & fprotocol, double & epsil, long int & max_steps, char & mag, std :: string & inmess, std :: string & outmess, std :: string & delmess, bool & binmess);
void parse_test_args (   int argc, char *argv[], int & nth, std :: string & patternsfile, std :: string & del, bool & bin, std :: string & weight_file, std :: string & output_file);

#endif // CMD_H
