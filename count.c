/* Author: Petra Ajaka */
/* Compile: gcc count.
/* Run: ./count <input-filename> <search-string> <output-filename> */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
const int MAX_BUFFSIZE = 100;

int main(int argc, char *argv[]) {
	int i, j;
	FILE *input;
	FILE *output;
	char *search;
	char *buffer = malloc(MAX_BUFFSIZE);
	int read, filesize, matches, ismatch, index, err;
	
	if (argc != 4) {
		printf("The number of command line arguments is incorrect. Please enter command line arguments in the format 'count <input-filename> <search-string> <output-filename>'.\n");
		exit(1);
	}
	
	/* because search string will be 20 bytes or less */
	search = (char*)malloc(20 * sizeof(char));
	strcpy(search, argv[2]);
	if (strlen(search) > 20) {
		printf("The search string must be 20 bytes or less.\n");
		exit(1);
	}
	
	if ((input = fopen(argv[1], "rb")) == NULL) {
		printf("Input file name: %s is incorrect or not found. Please enter a correct name for the input file.\n", argv[1]);
		exit(1);
	}
	
	if ((output = fopen(argv[3], "wb+")) == NULL) {
		printf("Output file: %s could not be created. Please enter a correct name for the output file.\n", argv[3]);
		exit(1);
	}
	
	fseek(input, 0, SEEK_SET);
	read = fread(buffer, 1, 100, input);
	filesize = read;
	while (read >= strlen(search)) {
		for (i = 0; (i < read) && (read - i >= strlen(search)); i++) {
			if (buffer[i] == (search[0] & 0xff)) {
				ismatch = 1;
				for (j = 0; j < strlen(search); j++) {
					if (buffer[i+j] != (search[j] & 0xff) ) {
						ismatch = 0;
						break;
					}
				}
				if (ismatch == 1) {
		        		matches++;
		        	}
		        }
		}
		if (read < 100) {
			break;
		}
        	index = -(strlen(search)) + 1;
        	fseek(input, index, SEEK_CUR);
        	read = fread(buffer, 1, 100, input);
	}
	
	filesize = ftell(input);
	fseek(output, 0, SEEK_SET);
	
	fprintf(output,"Size of input file in bytes: %d\n", filesize);
	printf("Size of input file in bytes: %d\n", filesize);
	fprintf(output, "Number of matches for your search string: %d\n", matches);
	printf("Number of matches for your search string: %d\n", matches);
	
	free(buffer);
	free(search);
	err = fclose(output);
	if (err < 0) {
		perror("Close");
	}
	err = fclose(input);
	if (err < 0) {
		perror("Close");
	}
	return(0);
}


