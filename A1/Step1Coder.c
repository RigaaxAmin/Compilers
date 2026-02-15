/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2025
* Author: Suleiman Amin - 041 130 274 / Blessy Joy - 041 134 4029
* Professors: Paulo Sousa
************************************************************
#
# ECHO "=---------------------------------------="
# ECHO "|  COMPILERS - ALGONQUIN COLLEGE (F25)  |"
# ECHO "=---------------------------------------="
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    ”
# ECHO "    @@                             @@    ”
# ECHO "    @@           %&@@@@@@@@@@@     @@    ”
# ECHO "    @@       @%% (@@@@@@@@@  @     @@    ”
# ECHO "    @@      @& @   @ @       @     @@    ”
# ECHO "    @@     @ @ %  / /   @@@@@@     @@    ”
# ECHO "    @@      & @ @  @@              @@    ”
# ECHO "    @@       @/ @*@ @ @   @        @@    ”
# ECHO "    @@           @@@@  @@ @ @      @@    ”
# ECHO "    @@            /@@    @@@ @     @@    ”
# ECHO "    @@     @      / /     @@ @     @@    ”
# ECHO "    @@     @ @@   /@/   @@@ @      @@    ”
# ECHO "    @@     @@@@@@@@@@@@@@@         @@    ”
# ECHO "    @@                             @@    ”
# ECHO "    @@         R I G A A X         @@    ”
# ECHO "    @@                             @@    ”
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    ”
# ECHO "                                         "
# ECHO "[CODER SCRIPT ..........................]"
# ECHO "                                         "
*/

/*
***********************************************************
* File name: Reader.c
* Compiler: MS Visual Studio 2026
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A12.
* Date: Sep 01 2025
* Professor: Paulo Sousa
* Purpose: This file is the main code for Buffer/Reader (A12)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * - Please check the "TODO" labels to develop your activity.
 * - Review the functions to use "Defensive Programming".
 *.............................................................................
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef CODER_H_
#include "Step1Coder.h"
#endif

 // Function to perform the Vigenère cipher (encoding or decoding)
rigaax_void vigenereFile(const rigaax_strg inputFileName, const rigaax_strg outputFileName, const rigaax_strg key, rigaax_intg encode) {
	// TO_DO: Define the input and output files (ex: FILE* inputFile, FILE* outputFile
	// TO_DO: Use defensive programming (checking files)
	FILE* inputFile = fopen(inputFileName, "rb");
	if (inputFile == NULL) {
		fprintf(stderr, "Error Opening input file: %s\n", inputFileName);
		return;
	}

	FILE* outputFile = fopen(outputFileName, "wb");
	if (outputFile == NULL) {
		fprintf(stderr, "Error opening output file: %s\n", outputFileName);
		fclose(inputFile);
		return;
	}

	// TO_DO: Define local variables
	rigaax_intg ch;
	rigaax_intg keylen = (rigaax_intg)strlen(key); // calculates the length of the key
	rigaax_intg keyIndex = 0; // tracks the index of the key

	// TO_DO: Logic: check if it is encode / decode to change the char (using Vigenere algorithm) - next function
	while ((ch = fgetc(inputFile)) != EOF) // reads one character at a time from the file until EOF
	{
		if (isalpha(ch)) {
			rigaax_char base = isupper(ch) ? 'A' : 'a'; // set the base to A or a converting to index= 0
			rigaax_char keyPass = (rigaax_char)tolower((unsigned char)key[keyIndex % keylen]); // converts the key to lower case and gets it as number
			rigaax_intg keyShift = keyPass - 'a';
			rigaax_intg charpost = ch - base; // gets the position of the character

			if (encode) {
				ch = (charpost + keyShift) % 26 + base;
			}
			else {
				ch = (charpost - keyShift + 26) % 26 + base;
			}
			keyIndex++;
		}
		fputc(ch, outputFile); // writing to the outputfile
	}
	// TO_DO: Close the files
	fclose(inputFile);
	fclose(outputFile);
}

// Function to perform the Vigenère cipher (encoding or decoding)
rigaax_strg vigenereMem(const rigaax_strg inputFileName, const rigaax_strg key, rigaax_intg encode) {
	// TO_DO define the return type and local variables
	rigaax_intg ch;
	rigaax_intg keylen = (rigaax_intg)strlen(key); // calculates the length of the key
	rigaax_intg keyIndex = 0; // tracks the index of the key
	rigaax_intg i = 0;
	rigaax_intg size;
	rigaax_strg output;

	// TO_DO: Check defensive programming
	FILE* inputFile = fopen(inputFileName, "r");
	if (inputFile == NULL) {
		fprintf(stderr, "Error Opening input file: %s\n", inputFileName);
		return NULL;
	}

	size = getSizeOfFile(inputFileName);
	if (size <= 0) {
		fclose(inputFile);
		return NULL;
	}

	output = (rigaax_strg)malloc((size_t)size + 1);
	if (output == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		fclose(inputFile);
		return NULL;
	}

	// TO_DO: Use the logic to code/decode - consider the logic about visible chars only
	// encoding it and returning the output
	while ((ch = fgetc(inputFile)) != EOF) // reads one character at a time from the file until EOF
	{
		if (isalpha(ch)) {
			rigaax_char base = isupper(ch) ? 'A' : 'a'; // set the base to A or a converting to index= 0
			rigaax_char keyPass = (rigaax_char)tolower((unsigned char)key[keyIndex % keylen]); // converts the key to lower case
			rigaax_intg keyShift = keyPass - 'a';
			rigaax_intg charpost = ch - base; // gets the position of the character

			if (encode) {
				ch = (charpost + keyShift) % 26 + base;
			}
			else {
				ch = (charpost - keyShift + 26) % 26 + base;
			}
			keyIndex++;
		}
		output[i++] = (rigaax_char)ch;
	}
	output[i] = '\0';
	fclose(inputFile);
	return output;
}

// Function to encode (cypher)
rigaax_void cypher(const rigaax_strg inputFileName, const rigaax_strg outputFileName, const rigaax_strg key) {
	vigenereFile(inputFileName, outputFileName, key, CYPHER);
}

// Function to decode (decypher)
rigaax_void decypher(const rigaax_strg inputFileName, const rigaax_strg outputFileName, const rigaax_strg key) {
	vigenereFile(inputFileName, outputFileName, key, DECYPHER);
}

// TO_DO: Get file size (util method)
rigaax_intg getSizeOfFile(const rigaax_strg filename) {
	FILE* file = fopen(filename, "rb");
	if (!file) {
		fprintf(stderr, "Error opening file: %s\n", filename);
		return 0;
	}

	fseek(file, 0L, SEEK_END);
	rigaax_intg size = (rigaax_intg)ftell(file);
	fclose(file);

	return size;
}

