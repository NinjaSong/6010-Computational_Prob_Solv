/*
 ============================================================================
 Name        : rsa_crypt.c
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

#define CHAR_SIZE 8
#define MASK 0xff
#define BASE 2
#define DELIM ','


void encrpyt_num(mpz_t c, mpz_t m, mpz_t n, mpz_t e) {
	mpz_powm(c, m, e, n);
}

void decrypt_num(mpz_t m, mpz_t c, mpz_t n, mpz_t d) {
	mpz_powm(m, c, d, n);
}


int min(int a, int b) {
	return a <= b ? a : b;
}


// given n, compute the chunk size according to the number of bits in n
// chunk size means how many chars form a chunk
int getChunkSize(mpz_t n) {

	size_t keySize = mpz_sizeinbase(n, 2);
	if (keySize <= 8) {
		printf("WARNING: key is too small\n");
		return -1;
	}

	// get the largest number chunkSize = 8k such that n < keySize
	int chunkSize = keySize / CHAR_SIZE;
	if (keySize % CHAR_SIZE == 0)	// has to make sure msg < n
		chunkSize--;
	// printf("keySize = %d, chunkSize = %d\n", keySize, chunkSize);

	return chunkSize;

}


char **encrypt_str(int *siz, char *str, mpz_t n, mpz_t e) {

	int chunkSize = getChunkSize(n);

	// the chunk size will be chunkSize characters
	int len = strlen(str);
	int resArrSize = len / chunkSize;
	if (len % chunkSize != 0)
		resArrSize++;
	char **cipherArr = (char **)malloc(sizeof(char *) * resArrSize);

	for (int i = 0, k = 0; i < len; i += chunkSize, k++) {
		mpz_t m, c;
		mpz_init_set_ui(m, 0);
		mpz_init(c);

		for (int j = i; j < min(i + chunkSize, len); j++) {
			mpz_mul_2exp(m, m, CHAR_SIZE);  // left shift CHAR_SIZE bits
			mpz_add_ui(m, m, (long)str[j]);
			// printf("str[j] = %ld\n", (long)str[j]);
		}
		// printf("msgSize = %d, msg = %s\n", mpz_sizeinbase(m, 2), mpz_get_str(NULL, BASE, m));

		encrpyt_num(c, m, n, e);
		// printf("cipherSize = %d\n", mpz_sizeinbase(c, 2));
		cipherArr[k] = mpz_get_str(NULL, BASE, c);  // NULL means allocate new mem for the str
		// printf("cipher = %s\n", cipherArr[k]);

		mpz_clear(m);  // has to free the memory!!
		mpz_clear(c);
	}

	(*siz) = resArrSize;
	return cipherArr;

}


char *decrypt_arr(char **arr, int siz, mpz_t n, mpz_t d) {

	int chunkSize = getChunkSize(n);

	char buffer[siz * chunkSize];
	int bufSize = 0;  // how many chars in the original msg
	for (int i = 0; i < siz; i++) {
		mpz_t c;
		mpz_init(c);
		mpz_set_str(c, arr[i], BASE);
		// gmp_printf("c = %Zd\n", c);

		mpz_t m;
		mpz_init(m);
		decrypt_num(m, c, n, d);
		// gmp_printf("m = %Zd\n", m);

		char tempBuffer[chunkSize];
		int tempBufSize = 0;
		for (int j = 0; j < chunkSize && mpz_cmp_ui(m, 0) != 0; j++) {
			tempBuffer[j] = (char)(mpz_get_ui(m) & MASK);  // get the lower 8 bits of m
			// printf("tempBuffer[j] = %d\n", tempBuffer[j]);
			tempBufSize++;
			mpz_tdiv_q_2exp(m, m, CHAR_SIZE);  // right shift CHAR_SIZE bits
		}
		for (int j = tempBufSize - 1; j >= 0; j--)  // reversely copy to buffer
			buffer[bufSize++] = tempBuffer[j];

		mpz_clear(m);
		mpz_clear(c);
	}

	char *msg = (char *)malloc(sizeof(char) * (bufSize + 1));  // +1 for '\0'
	for (int i = 0; i < bufSize; i++) {
		msg[i] = buffer[i];
	}
	msg[bufSize] = '\0';
	// printf("%s\n", msg);

	return msg;

}


char *arr2string(char **arr, int siz, mpz_t n) {

	int len = 0;
	for (int i = 0; i < siz; i++)
		len += strlen(arr[i]);
	len += siz;  // plus delim and '\0'

	char *ciphertext = (char *)malloc(sizeof(char) * len);
	int k = 0;
	for (int i = 0; i < siz; i++) {
		for (int j = 0; j < strlen(arr[i]); j++) {
			ciphertext[k++] = arr[i][j];
		}
		ciphertext[k++] = DELIM;
	}
	ciphertext[k - 1] = '\0';

	return ciphertext;

}


char **string2arr(int *siz, char *str, mpz_t n) {

	return parse_str(siz, str, DELIM);

}


char *encrypt(char *msg, mpz_t n, mpz_t e) {

	int siz = 0;
	char **cipherArr = encrypt_str(&siz, msg, n, e);
	char *ciphertext = arr2string(cipherArr, siz, n);
	
	// http://stackoverflow.com/questions/15691477/c-mpir-mpz-t-to-stdstring
	void (*freefunc)(void *, size_t);
	mp_get_memory_functions (NULL, NULL, &freefunc);
	for (int i = 0; i < siz; i++) {
		freefunc(cipherArr[i], strlen(cipherArr[i]) + 1);
	}
	free(cipherArr);

	return ciphertext;

}


char *encrypt_parallel(char *msg, mpz_t n, mpz_t e, int nthread) {

	if (nthread >= 2) {

		char **ciphers = (char **)malloc(sizeof(char *) * nthread);
		int partSize = strlen(msg) / nthread;

		#pragma omp parallel num_threads(nthread)
		{
			int id = omp_get_thread_num();
			// the length of last part may differ
			int siz = id == nthread - 1 ? strlen(msg) - partSize * (nthread - 1) : partSize;

			char *sub_msg = (char *)malloc(sizeof(char) * (siz + 1));
			for (int i = id * partSize, j = 0; j < siz; i++, j++) {
				sub_msg[j] = msg[i];
			}
			sub_msg[siz] = '\0';
			// #pragma omp critical
			// {
			// 	printf("sub_msg = %s\n", sub_msg);
			// }

			char *cipher = encrypt(sub_msg, n, e);
			free(sub_msg);
			ciphers[id] = cipher;

			#pragma omp barrier
			// wait until all threads finish

		}

		int siz = 0;
		for (int i = 0; i < nthread; i++)
			siz += strlen(ciphers[i]);
		siz += nthread - 1;

		char *ciphertext = (char *)malloc(sizeof(char) * (siz + 1));
		int k = 0;
		for (int i = 0; i < nthread; i++) {
			for (int j = 0; j < strlen(ciphers[i]); j++)
				ciphertext[k++] = ciphers[i][j];
			ciphertext[k++] = DELIM;
		}
		ciphertext[k - 1] = '\0';

		for (int i = 0; i < nthread; i++)
			free(ciphers[i]);
		free(ciphers);
		return ciphertext;
		
	}
	else {

		return encrypt(msg, n, e);

	}

}


char *decrypt(char *ciphertext, mpz_t n, mpz_t d) {

	int siz = 0;
	char **cipherArr = string2arr(&siz, ciphertext, n);
	// printf("siz = %d\n", siz);
	char *msg = decrypt_arr(cipherArr, siz, n, d);
	
	void (*freefunc)(void *, size_t);
	mp_get_memory_functions (NULL, NULL, &freefunc);
	for (int i = 0; i < siz; i++) {
		freefunc(cipherArr[i], strlen(cipherArr[i]) + 1);
	}
	free(cipherArr);

	return msg;

}


char *decrypt_parallel(char *ciphertext, mpz_t n, mpz_t d, int nthread) {

	if (nthread >= 2) {

		// printf("ciphertext = %s\n", ciphertext);
		char **msgs = (char **)malloc(sizeof(char *) * nthread);
		
		// now ciphertext is split by ','
		int len = strlen(ciphertext);
		int num_tokens = 0;
		for (int i = 0; i < len; i++) {
			if (ciphertext[i] == DELIM)
				num_tokens++;
		}
		num_tokens++;

		int partSize = num_tokens / nthread;
		int begin[nthread];  // begining index for each thread
		begin[0] = 0;
		int k = 1;
		for (int i = 0, count = 0; k < nthread; i++) {
			if (ciphertext[i] == DELIM) {
				count++;
				if (count == partSize) {
					begin[k++] = i + 1;
					count = 0;
				}
			}
		}

		#pragma omp parallel num_threads(nthread)
		{
			int id = omp_get_thread_num();
			// the length of last part may differ
			int siz = id == nthread - 1 ? strlen(ciphertext) - begin[id] : begin[id + 1] - begin[id] - 1;

			char *sub_ciphertext = (char *)malloc(sizeof(char) * (siz + 1));
			for (int i = begin[id], j = 0; j < siz; i++, j++) {
				sub_ciphertext[j] = ciphertext[i];
			}
			sub_ciphertext[siz] = '\0';
			// #pragma omp critical
			// {
			// 	printf("sub_ciphertext = %s\n", sub_ciphertext);
			// }

			char *sub_msg = decrypt(sub_ciphertext, n, d);
			free(sub_ciphertext);
			msgs[id] = sub_msg;
			// #pragma omp critical
			// {
			// 	printf("sub_msg = %s\n", sub_msg);
			// }

			#pragma omp barrier
			// wait until all threads finish

		}

		int siz = 0;
		for (int i = 0; i < nthread; i++)
			siz += strlen(msgs[i]);

		char *plaintext = (char *)malloc(sizeof(char) * (siz + 1));
		k = 0;
		for (int i = 0; i < nthread; i++) {
			for (int j = 0; j < strlen(msgs[i]); j++)
				plaintext[k++] = msgs[i][j];
		}
		plaintext[k] = '\0';

		for (int i = 0; i < nthread; i++)
			free(msgs[i]);
		free(msgs);
		return plaintext;

	}
	else {

		return decrypt(ciphertext, n, d);

	}

}


int main(int argc, char** argv) {

	mpz_t n, e;
	mpz_init_set_str(n, "75690049882905677427243398237269124724376010308150229635851700653224662213483142459840\
		3361203736357710359848287240272915170332139856884875993469065343977456834126463192056117715074785653051\
		7546453615949472789921607397542057132419012367239973716160097301096010383838987355657723288016571446054\
		0377441156613334489203992747470127962202013533138237726858911537953375674218920940882796304200093706175\
		6951107177432135351369045062209934885093489555750139467760740620331802676612747210982259787978162275162\
		9964501259566251173486503430923669251123897675586559596382326173380403124935671349492671588410363206783\
		756998947674967", 10);
	
	mpz_init_set_str(e, "65537", 10);
	
	// mpz_init_set_ui(n, 2966772883822367927);
	// mpz_init_set_ui(e, 2642027824495698257);

	// mpz_t m;
	// mpz_init_set_ui(m, 1231209471);
	mpz_t c;
	mpz_init(c);
	mpz_t d;
	mpz_init_set_str(d, "1865081733309495071139285652583516326920591712263786957580251024381532676328698700862049\
		816146472746794116482291017771229547537074713342670921545376846650281583580001234190494679454768071643924\
		160084287776921196332420814298497511857892771386171815454038411881405749665178548248674461443341659316958\
		812942534305750904778197130501479437822334835468290402846883039566371210749660307636070954906440494073338\
		935596731718302580993884719831416428825202523968204131973563997666271702310587885661109391646118609866771\
		132238722659283504552685929112717694419235578579328814343871059352962611232226967421882761824639046048358\
		665", 10);

	// mpz_init_set_ui(d, 4105243553);

	// gmp_printf("%Zd\n", m);
	// encrpyt_num(c, m, n, e);
	// decrypt_num(m, c, n, d);
	// gmp_printf("%Zd\n", m);
	
	int nthread = atoi(argv[1]);

	// char *msg = "hello world! HELLO WORLD!";
	char *msg = read_file("subdata.txt");
	// printf("msg = %s\n", msg);

	struct timespec start, finish;
	double elapsed = 0;
	clock_gettime(CLOCK_MONOTONIC, &start);
	//----
	char *ciphertext = encrypt_parallel(msg, n, e, nthread);
	// printf("ciphertext = %s\n", ciphertext);
	char *msg_recover = decrypt_parallel(ciphertext, n, d, nthread);
	//----
	clock_gettime(CLOCK_MONOTONIC, &finish);
	elapsed = (finish.tv_sec - start.tv_sec);
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
	printf("Elapsed time: %fs\n", elapsed);

	// printf("msg_recover = %s\n", msg_recover);


	// mpz_clear(m);
	mpz_clear(n);
	mpz_clear(c);
	mpz_clear(e);
	mpz_clear(d);
	free(ciphertext);
	free(msg_recover);

}


