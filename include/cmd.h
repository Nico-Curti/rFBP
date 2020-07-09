#ifndef __cmd_h__
#define __cmd_h__

#include <errors.h>
#include <utils.hpp>
#include <parse_args.hpp>

#include <string>

/**
* @brief Parse command line for training procedure using parse_args library
*
* @param[in] argc number of arguments in command line
* @param[in] argv list of arguments in command line
* @param[out] patternsfile pattern filename in which data are stored following Pattern requirements
* @param[out] output output filename where results are saved
* @param[out] bin boolean variable to enable/disable pattern IO in binary/ascii format
* @param[out] del delimiter char for ascii IO of pattern file (valid if bin is off and stored as STRING)
* @param[out] K number of Hidden Layers
* @param[out] max_iters max number of iterations for training
* @param[out] seed seed global random generator
* @param[out] randfact seed random generator of Cavity Messages
* @param[out] damping damping parameter for training protocol
* @param[out] accuracy1 accuracy of the messages computation at the hidden units level (choose between 'exact'(default), 'accurate', 'approx', 'none')
* @param[out] accuracy2 accuracy of the messages computation at the output units level (choose between 'exact'(default), 'accurate', 'approx', 'none')
* @param[out] fprotocol specify protocol : scooping, pseudo_reinforcement (default), free_scoping, standard_reinforcement
* @param[out] epsil threshold for convergence
* @param[out] nth max number of threads exploitable
* @param[out] max_steps max number of steps for chosen protocol
* @param[out] mag specify Magnetization to use in training: (0) MagnetizationP (MagP64), (1) MagnetizationT (MagT64)
* @param[out] inmess filename with initial message values
* @param[out] outmess filename where save trained message values
* @param[out] delmess delimiter for Messages files (as STRING)
* @param[out] binmess messages files format: (0) Textfile (default), (1) Binary
*
*/
void parse_training_fbp (int argc, char *argv[], std :: string & patternsfile, std :: string & output, bool & bin, std :: string & del, long int & K, long int & max_iters, long int & seed, double & randfact, double & damping, std :: string & accuracy1, std :: string & accuracy2, std :: string & fprotocol, double & epsil, int & nth, long int & max_steps, int & mag, std :: string & inmess, std :: string & outmess, std :: string & delmess, bool & binmess);

/**
* @brief Parse command line for testing procedure using parse_args library
*
* @param[in] argc number of arguments in command line
* @param[in] argv list of arguments in command line
* @param[out] patternsfile pattern filename in which data are stored following Pattern requirements
* @param[out] del delimiter char for ascii IO of pattern file (valid if bin is off and stored as STRING)
* @param[out] bin boolean variable to enable/disable pattern IO in binary/ascii format
* @param[out] weight_file weights matrix filename
* @param[out] output_file output filename where results are saved
* @param[out] nth max number of threads exploitable
*
*/
void parse_test_args (int argc, char *argv[], std :: string & patternsfile, std :: string & del, bool & bin, std :: string & weight_file, std :: string & output_file, int & nth);

#endif // __cmd_h__
