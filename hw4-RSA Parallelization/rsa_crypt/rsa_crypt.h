/*
 ============================================================================
 Name        : rsa_crypt.h
 Author      : Hongyang Wang
 Version     :
 Copyright   : © Fall 2016 CSE 6010 Hongyang Wang
 Description : Basic RSA operations
 ============================================================================
 */

#ifndef RSA_CRYPT_H_
#define RSA_CRYPT_H_

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

/* Encrypt a number m with public key pair (n, e), return value to be stored in c */
void encrpyt_num(mpz_t c, mpz_t m, mpz_t n, mpz_t e);

/* Encrypt a string: the string will be cut into chunks and convert to numbers
*  Then will be encrypted as an array of mpz_t numbers and finally convert to array of string
*  Parameters:
*    siz: will be filled with the length of the returned array
*    str: string to be encrypted
*    n, e: public key pair
*  Return:
*  	 string array
*/
char **encrypt_str(int *siz, char *str, mpz_t n, mpz_t e);

/* Decrypt a number c with key n, d, return value will be stored in m */
void decrypt_num(mpz_t m, mpz_t c, mpz_t n, mpz_t d);

/* Decrypt an array of big integers (in char) and return the plain text
*  Parameters:
*    arr: array of big integers (in char)
*    siz: length of the arr
*    n, e: keys for the decryption
*  Return:
*    plain text, should match the original message
*/
char *decrypt_arr(char **arr, int siz, mpz_t n, mpz_t d);

/* Encrypt a msg and return the ciphertext as a string */
char *encrypt(char *msg, mpz_t n, mpz_t e);

/* Decrypt a ciphertext and return the msg as a string */
char *decrypt(char *ciphertext, mpz_t n, mpz_t d);

/* Encrypt a msg and return the ciphertext as a string with parallelization */
char *encrypt_parallel(char *msg, mpz_t n, mpz_t e, int nthread);

/* Decrypt a ciphertext and return the msg as a string with parallelization */
char *decrypt_parallel(char *ciphertext, mpz_t n, mpz_t d, int nthread);


#endif /* RSA_CRYPT_H_ */
