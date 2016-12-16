/*
 ============================================================================
 Name        : file_ops.h
 Author      : Hongyang Wang
 Version     :
 Copyright   : © Fall 2016 CSE 6010 Hongyang Wang
 Description : Basic RSA operations
 ============================================================================
 */

#ifndef FILE_OPS_H_
#define FILE_OPS_H_

#include <stdio.h>
#include <stdlib.h>

/* Read file and return as a string */
char *read_file(char *filename);

/* Parse a string, split into tokens 
*  Parameters:
* 	 siz: will be filled with the length of the returned array
* 	 str: a string (char array) to be split
* 	 delim: delimiter character
*  Return:
* 	 string array each of whose element is a string token
*/
char **parse_str(int* siz, char *str, char delim);

/* Dump string array to file 
*  Parameters:
*    filename: path & filename of the output file
*    tokens: string array
*    siz: length of the string array
*    delim: delimiter
*/
void write_file(char *filename, char **tokens, int siz, char delim);

#endif /* FILE_OPS_H_ */
