
/*
 * Copyright (c) 2012 Anthony Hu (anth_hu@hotmail.com).
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* 
 * sswf
 * String Substitution With File 
 * =============================
 * commentary: this probably would have been simpler in just about 
 *             any another language. 
 * function: replace a line containing a match of the substitution string
 *           with the contents of the substitution file in the input file 
 *           and write out results to the output file.  
 * usage: sswf inputfile substitutionfile outputfile substitutionstring
 * build: gcc -o sswf sswf.c
 */

#include <stdio.h>
#include <string.h>

/* 
 * An arbitrary length of strings. 
 * Please adjust as needed.
 */
const int LENGTH = 4096;

int main (int argc, char ** argv) {
	FILE * infptr = NULL;
	FILE * outfptr = NULL;
	FILE * subfptr = NULL;

	char line[LENGTH];
	char repl[LENGTH];

	if (argc != 5) {
		fprintf(stderr, "invalid number of params\n");
		return 0;
	}

	infptr = fopen(argv[1], "r");
	if (infptr == NULL) {
		fprintf(stderr, "bad input file name\n");
		return 0;
	}

	//we open and close this file to test that we 
	//we can properly do it. 
	subfptr = fopen(argv[2], "r");
	if (subfptr == NULL) {
		fprintf(stderr, "bad substitution file name\n");
		fclose(infptr);
		return 0;
	}
	fclose(subfptr);
	subfptr = NULL;
	
	outfptr = fopen(argv[3], "w");
	if (infptr == NULL) {
		fprintf(stderr, "bad output file name\n");
		fclose(infptr);
		return 0;
	}

	strncpy(repl, argv[4], LENGTH);

	while (fgets(line, LENGTH, infptr) != NULL) {
		if (strstr(line, repl) == NULL) {
			fputs(line, outfptr);
		} else {
			//we found a match!!  do the substitution. 
			subfptr = fopen(argv[2], "r");
			while(fgets(line, LENGTH, subfptr) != NULL) {
				fputs(line, outfptr);
			}
			fclose(subfptr);
			subfptr = NULL;
		}
	} 
	fclose(infptr);
	fclose(outfptr);
	return 0;
}
