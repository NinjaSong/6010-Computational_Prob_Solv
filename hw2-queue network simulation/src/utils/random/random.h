/*
 * random.h
 *
 *  Created on: Sep 22, 2016
 *      Author: YS
 */

#ifndef UTILS_RANDOM_RANDOM_H_
#define UTILS_RANDOM_RANDOM_H_

void rand_init(int randseed);  // initialize and set the random seed; if < 0 then use time(NULL) as the seed
double urand();  // get a random number from a uniform distribution
double randexp(double mean);  // get a random number from an exponential distribution

#endif /* UTILS_RANDOM_RANDOM_H_ */
