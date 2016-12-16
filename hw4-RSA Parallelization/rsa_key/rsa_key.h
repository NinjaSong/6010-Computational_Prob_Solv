/*
 ============================================================================
 Name        : rsa_key.h
 Author      : Hongyang Wang
 Version     :
 Copyright   : © Fall 2016 CSE 6010 Hongyang Wang
 Description : Basic RSA operations
 ============================================================================
 */

#ifndef RSA_KEY_H_
#define RSA_KEY_H_

#include <stdlib.h>
#include <gmp.h>

typedef struct {
	int siz;
	mpz_t p, q, f;  // aux variables
	mpz_t n, e, d;  // keys

} RSAKey;

/* Generate RSA keypair with the given size */
RSAKey *generate_keypair(int siz);

/* Given p, q and e, compute d */
// void computeD(mpz_t d, mpz_t p, mpz_t q, mpz_t e);

/* Print key */
void printRSAKey(RSAKey *rsaKey);

#endif /* RSA_KEY_H_ */
