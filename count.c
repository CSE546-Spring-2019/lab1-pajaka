/* Author: Petra Ajaka */
/* Compile: make
/* Run: ./count <input-filename> <search-string> <output-filename> */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
const int MAX_BUFFSIZE = 100;
const int CHUNK = 20;

int main(int argc, char *argv[]) {
	int i, j;
	FILE *input;
	FILE *output;
	char *search;
	char *buffer = malloc(MAX_BUFFSIZE);
	int read, filesize, ismatch, index, err;
	int matches = 0;
	int check[CHUNK];
	
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

	for (i = 0; i < CHUNK; i++) {
		check[i] = -1;
	} 
	do {
		read = fread(buffer, 1, sizeof(buffer), input);
		for (i = 0; i < read; i++) {
			filesize++;
			ismatch = 0;
			index = (buffer[i] == (search[0] & 0xff));
			for (j = 0; j < CHUNK; j++) {
				if (check[j] != -1) {
					if (buffer[i] == (search[check[j] + 1] & 0xff)) {
						if ((check[j] + 2) == strlen(search)) {
							matches++;
							check[j] = -1;
						}
						else {
							check[j]++;
						}
					}
					else {
						check[j] = -1;
					}
				}
				
				if (check[j] == -1) {
					if (!ismatch && index) {
						check[j] = 0;
						ismatch = 1;
					}
				}
			}	
		}
	} while (read > 0);
	
	fprintf(output,"Size of input file in bytes: %d\n", filesize);
	printf("Size of input file in bytes: %d\n", filesize);
	fprintf(output, "Number of matches for your search string: %d\n", matches);
	printf("Number of matches for your search string: %d\n", matches);
	
	free(search);
	free(buffer);
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


