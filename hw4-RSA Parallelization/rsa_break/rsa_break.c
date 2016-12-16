/*
 ============================================================================
 Name        : rsa_break.c
 Author      : Hongyang Wang
 Version     :
 Copyright   : © Fall 2016 CSE 6010 Hongyang Wang
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>
#include <omp.h>
#include <time.h>
#include "../file_ops/file_ops.h"

#define FILE_MODE 1
#define ARG_MODE 0


void breakRSA(mpz_t d, mpz_t n, mpz_t e, int nthread) {

	int isFound = 0;  // indicate if d is found

	if (nthread <= 1) {  // single thread

		mpz_t p, q, fai_n;
		mpz_init(p);
		mpz_init(q);
		mpz_init(fai_n);

		mpz_t r, p1, q1;
		mpz_init(r);
		mpz_init(p1);
		mpz_init(q1);

		// init p to be int(sqrt(n)) as well as a odd number
		mpz_sqrt(p, n);
		if (!mpz_odd_p(p))
			mpz_sub_ui(p, p, 1);

		// enumerate every odd numbers, sub by 2 each time util p == 3
		for ( ; mpz_cmp_ui(p, 3) >= 0; mpz_sub_ui(p, p, 2)) {
			mpz_mod(r, n, p);  // set r = n % p
			if (mpz_cmp_ui(r, 0) == 0) {  // if r == 0
				mpz_tdiv_q(q, n, p);  // q = n / p, tdiv: round to 0
				mpz_sub_ui(p1, p, 1);
				mpz_sub_ui(q1, q, 1);
				mpz_mul(fai_n, p1, q1);  // fai_n = (p - 1) * (q - 1)
  				mpz_invert(d, e, fai_n);
  				isFound = 1;
  				break;
			}
		}

		mpz_clear(p);
		mpz_clear(q);
		mpz_clear(fai_n);
		mpz_clear(r);
		mpz_clear(p1);
		mpz_clear(q1);		

	}
	else {
		
		int hasFound = 0;  // global variable indicating if some thread has found p, q

		#pragma omp parallel num_threads(nthread)
		{
			int id = omp_get_thread_num();
			// int numThread = omp_get_num_threads();
			// printf("nthread = %d\n", numThread);
			int numThread = nthread;
			int offset = 2 * numThread;

			mpz_t p, q, fai_n;
			mpz_init(p);
			mpz_init(q);
			mpz_init(fai_n);

			// init p according to thread number
			mpz_sqrt(p, n);
			if (!mpz_odd_p(p))  // make p odd
				mpz_sub_ui(p, p, 1);
			mpz_sub_ui(p, p, id * 2);  // different init values

			mpz_t r, p1, q1;
			mpz_init(r);
			mpz_init(p1);
			mpz_init(q1);

			for ( ; mpz_cmp_ui(p, 3) >= 0; mpz_sub_ui(p, p, offset)) {
				if (hasFound)
					break;
				mpz_mod(r, n, p);  // set r = n % p
				if (mpz_cmp_ui(r, 0) == 0) {  // if r == 0
					mpz_tdiv_q(q, n, p);  // q = n / p
					mpz_sub_ui(p1, p, 1);
					mpz_sub_ui(q1, q, 1);
					mpz_mul(fai_n, p1, q1);  // fai_n = (p - 1) * (q - 1)
					// critical section in case of two threads find p, q at the same time
					#pragma omp critical
					{
	  					mpz_invert(d, e, fai_n);
	  				}
	  				isFound = 1;
	  				hasFound = 1;
	  				break;
				}
			}

			mpz_clear(p);
			mpz_clear(q);
			mpz_clear(fai_n);
			mpz_clear(r);
			mpz_clear(p1);
			mpz_clear(q1);	
		}

	}

	if (!isFound)
		printf("WARNING: d not found\n");
	else
		gmp_printf("n = %Zd, e = %Zd, d = %Zd, nbits = %d\n", n, e, d, mpz_sizeinbase(n, 2));

}


void unitTest(char *N, char *E, int nthread) {

	mpz_t n, e;
	mpz_init_set_str(n, N, 10);
	mpz_init_set_str(e, E, 10);
	mpz_t d;
	mpz_init(d);

	struct timespec start, finish;
	double elapsed = 0;
	clock_gettime(CLOCK_MONOTONIC, &start);

	breakRSA(d, n, e, nthread);

	clock_gettime(CLOCK_MONOTONIC, &finish);
	elapsed = (finish.tv_sec - start.tv_sec);
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
	printf("Elapsed time: %fs\n", elapsed);

	mpz_clear(n);
	mpz_clear(e);
	mpz_clear(d);

}


int main(int argc, char** argv) {

	if (argc != 5) {
		printf("Parameters: (1) ./rsa_break -f input_file output_file nthread\n");
		printf("\t(2) ./rsa_break n e ciphertext nthread\n");
		return -1;
	}

	int mode = -1;
	if (strcmp(argv[1], "-f") == 0)
		mode = FILE_MODE;
	else
		mode = ARG_MODE;

	int nthread = atoi(argv[4]);

	if (mode == FILE_MODE) {

		char *content = read_file(argv[2]);

		// the first line will be n<space>e
		char DELIM = ' ';
		int i = 0;
		while (content[i] != DELIM) {
			i++;
		}
		content[i] = '\0';
		char *n = content;

		int j = i + 1;
		while (content[j] != '\n') {
			j++;
		}
		content[j] = '\0';
		char *e = content + (i + 1);

		// followed by the ciphertext
		char *ciphertext = content + (j + 1);

		printf("n = %s, e = %s, ciphertext = %s\n", n, e, ciphertext);

		unitTest(n, e, nthread);

		free(content);

	}
	else {

		char *n = argv[1], *e = argv[2], *ciphertext = argv[3];

		unitTest(n, e, nthread);

	}


}


