/*
 ============================================================================
 Name        : file_ops.c
 Author      : Hongyang Wang
 Version     :
 Copyright   : © Fall 2016 CSE 6010 Hongyang Wang
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* Read file and return as a string */
char *read_file(char *filename) {
	// http://stackoverflow.com/questions/3747086/reading-the-whole-text-file-into-a-char-array-in-c

	FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
    	printf("Can't open %s\n", filename);
		return NULL;
    }

	fseek(fp, 0, SEEK_END);  // move file pointer to the end
	long lSize = ftell(fp);
	rewind(fp);  // reset the file pointer to the begining

	// allocate memory for entire content
	char *buffer = (char *)malloc(sizeof(char) * (lSize + 1));

	// copy the file into the buffer
	fread(buffer, lSize, 1, fp);
	buffer[lSize] = '\0';

	fclose(fp);
	return buffer;

}


/* Deep copy a string */
char* strdup(const char* s)
{
    char* p = malloc(strlen(s) + 1);
    if (p) 
    	strcpy(p, s);
    return p;
}


/* Parse a string, split into tokens 
*  Parameters:
* 	 siz: will be filled with the length of the returned array
* 	 str: a string (char array) to be split
* 	 delim: delimiter character
*  Return:
* 	 string array each of whose element is a string token
*/
char **parse_str(int* siz, char *str, char delim) {

	int i = 0;
	*siz = 1;
	while (str[i] != '\0') {
		if (str[i] == delim)
			(*siz)++;
		i++;
	}  // get the total number of elements

	char **tokens = (char **)malloc(sizeof(char *) * (*siz));
	i = 0;
	char *token = strtok(str, &delim);
    for ( ; token != NULL; token = strtok(NULL, &delim)) {  
        // tokens[i++] = token;
        tokens[i++] = strdup(token);  // need deepcopy because strtok will modify the original string
    }  // separate each token split by delim

    return tokens;

}


/* Dump string array to file 
*  Parameters:
*    filename: path & filename of the output file
*    tokens: string array
*    siz: length of the string array
*    delim: delimiter
*/
void write_file(char *filename, char **tokens, int siz, char delim) {

	FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
    	printf("Can't open %s\n", filename);
    	return;
    }

    int i = 0;
    for ( ; i < siz; i++) {
    	fprintf(fp, "%s", tokens[i]);
    	if (i != siz - 1)
    		fprintf(fp, " ");
    }

    fclose(fp);

}


/*
int main(int argc, char** argv) {

	char *filename = "testdata.txt";
	char *content = read_file(filename);

	char delim = ' ';
	int siz = 0;
	char **tokens = parse_str(&siz, content, delim);
	free(content);

	for (int i = 0; i < siz; i++)
		printf("%s\n", tokens[i]);

	write_file("out.txt", tokens, siz, delim);

	free(tokens);

}
*/


