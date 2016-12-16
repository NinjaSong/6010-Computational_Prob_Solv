CSE 6010
Project - RSA Parallelization
Hongyang Wang (leader), Yunhe Song, Qiang Wang
Nov 14 - Dec 5, 2016


1. Folder Structure

This project was divided into 3 separate parts: 
	(1) RSA key generation (rsa_key/)
	(2) RSA encryption and decryption (rsa_crypt/)
	(3) RSA cracking (rsa_break/)

Here is the detailed folder structure.

file_ops/
	file_ops.c 				// Defines file i/o operations
	file_ops.h 				// Defines the file ops interface
	Makefile 				// Make and generates .o, which will be linked to other part of the software

rsa_key/
	rsa_key.c 				// Implements key generation
	rsa_key.h 				// Defines the data and function interface
	Makefile 				// Make and generates the executable, which takes one parameter <key_size_in_bit>

rsa_crypt/
	rsa_crypt.c 			// Implements the encryption and decryption of a single number as well as string, in both sequential and parallel
	rsa_crypt.h 			// Defines all the interfaces
	Makefile 				// Make and generates the executable, which takes one parameter <num_of_threads>
	subdata.txt 			// Sample test data

rsa_break/
	rsa_break.c 			// Implements the break of short RSA keys, in both sequential and parallel
	Makefile 				// Make and generates the executable, which takes following two sets of parameters: (1) ./rsa_break -f input_file output_file nthread; (2) ./rsa_break n e ciphertext nthread
	testdata.txt 			// Sample test data, in which first lines defines (n, e) separated by ' ', then the following lines gives the ciphertext


2. To Run the Software

The 3 parts is relatively independent to each other, so just run each part separately. Note that our software is depend on GMP and OpenMP library, so make sure the system has these 2 libs installed (we tested all the software on the deepthought cluster).


3. Work distribution

Generally, each students focuses on one of the 3 parts. Specifically,
	rsa_key, Yunhe Song
	rsa_crypt, Qiang Wang, Hongyang Wang
	rsa_break, Hongyang Wang

