/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Spring, 2026
* Author: James
* Professors: Paulo Sousa
************************************************************
#
# ECHO "=---------------------------------------="
# ECHO "|  COMPILERS - ALGONQUIN COLLEGE (S26)  |"
# ECHO "=---------------------------------------="
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    �
# ECHO "    @@                             @@    �
# ECHO "    @@           %&@@@@@@@@@@@     @@    �
# ECHO "    @@       @%% (@@@@@@@@@  @     @@    �
# ECHO "    @@      @& @   @ @       @     @@    �
# ECHO "    @@     @ @ %  / /   @@@@@@     @@    �
# ECHO "    @@      & @ @  @@              @@    �
# ECHO "    @@       @/ @*@ @ @   @        @@    �
# ECHO "    @@           @@@@  @@ @ @      @@    �
# ECHO "    @@            /@@    @@@ @     @@    �
# ECHO "    @@     @      / /     @@ @     @@    �
# ECHO "    @@     @ @@   /@/   @@@ @      @@    �
# ECHO "    @@     @@@@@@@@@@@@@@@         @@    �
# ECHO "    @@                             @@    �
# ECHO "    @@         J A M E S           @@    �
# ECHO "    @@                             @@    �
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    �
# ECHO "                                         "
# ECHO "[CODER SCRIPT ..........................]"
# ECHO "                                         "
*/

/*
************************************************************
* File name: Step1Coder.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers
* Assignment: A1
* Author: James
* Professor: Paulo Sousa
* Purpose: Implementation of Vigenère Cipher (Coder)
*************************************************************/

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

#define _CRT_SECURE_NO_WARNINGS

// Function to get shift value for a character (based on ASCII range)
static jms_intg getShift(jms_char ch) {
    if (ch >= ASCII_START && ch <= ASCII_END) {
        return ch - ASCII_START;
    }
    return 0;
}

// Function to perform the Vigenere cipher (encoding or decoding)
void vigenereFile(const jms_strg inputFileName, const jms_strg outputFileName, const jms_strg key, jms_intg encode) {	// TO_DO: Define the input and output files (ex: FILE* inputFile, FILE* outputFile
	
	// TO_DO: Use defensive programming (checking files)
	if (inputFileName == NULL || outputFileName == NULL || key == NULL) {
        errorPrint("Error: Null parameter in vigenereFile\n");
        return;
    }
    if (encode != CYPHER && encode != DECYPHER) {
        errorPrint("Error: Invalid operation (must be 1=cypher or 0=decypher)\n");
        return;
    }
    if (strlen(key) == 0) {
        errorPrint("Error: Empty key\n");
        return;
    }

    FILE* inputFile = fopen(inputFileName, "rb");
    if (inputFile == NULL) {
        errorPrint("Error: Cannot open input file '%s'\n", inputFileName);
        return;
    }

    FILE* outputFile = fopen(outputFileName, "wb");
    if (outputFile == NULL) {
        errorPrint("Error: Cannot open output file '%s'\n", outputFileName);
        fclose(inputFile);
        return;
    }

	// TO_DO: Define local variables
	jms_intg keyLen = (jms_intg)strlen(key);
    jms_intg keyIndex = 0;
    jms_intg ch;

	// TO_DO: Logic: check if it is encode / decode to change the char (using Vigenere algorithm) - next function
	while ((ch = fgetc(inputFile)) != EOF) {
        if (ch >= ASCII_START && ch <= ASCII_END) {
            jms_intg shift = getShift(key[keyIndex]);
            jms_intg newCh;

            if (encode == CYPHER)
                newCh = ASCII_START + (ch - ASCII_START + shift) % ASCII_RANGE;
            else
                newCh = ASCII_START + (ch - ASCII_START - shift + ASCII_RANGE) % ASCII_RANGE;

            fputc(newCh, outputFile);
            keyIndex = (keyIndex + 1) % keyLen;
        } else {
            fputc(ch, outputFile);  /* Preserve non-visible characters */
        }
    }

	// TO_DO: Close the files
	fclose(inputFile);
    fclose(outputFile);
    printf("Operation completed: %s -> %s\n", inputFileName, outputFileName);
}

// Function to perform the Vigenere cipher (encoding or decoding)
jms_strg vigenereMem(const jms_strg inputFileName, const jms_strg key, jms_intg encode) {
	if (inputFileName == NULL || key == NULL || strlen(key) == 0) {
        return NULL;
    }

    jms_intg size = getSizeOfFile(inputFileName);
    if (size <= 0) return NULL;

    jms_strg output = (jms_strg)malloc(size + 1);
    if (output == NULL) return NULL;

    FILE* inputFile = fopen(inputFileName, "rb");
    if (inputFile == NULL) {
        free(output);
        return NULL;
    }

    jms_intg keyLen = (jms_intg)strlen(key);
    jms_intg keyIndex = 0;
    jms_intg i = 0;
    jms_intg ch;

    while ((ch = fgetc(inputFile)) != EOF && i < size) {
        if (ch >= ASCII_START && ch <= ASCII_END) {
            jms_intg shift = getShift(key[keyIndex]);
            if (encode == CYPHER)
                output[i] = ASCII_START + (ch - ASCII_START + shift) % ASCII_RANGE;
            else
                output[i] = ASCII_START + (ch - ASCII_START - shift + ASCII_RANGE) % ASCII_RANGE;
            keyIndex = (keyIndex + 1) % keyLen;
        } else {
            output[i] = (jms_char)ch;
        }
        i++;
    }

    output[i] = '\0';
    fclose(inputFile);
    return output;
}

// Function to encode (cypher)
void cypher(const jms_strg inputFileName, const jms_strg outputFileName, const jms_strg key) {
    vigenereFile(inputFileName, outputFileName, key, CYPHER);
}

// Function to decode (decypher)
void decypher(const jms_strg inputFileName, const jms_strg outputFileName, const jms_strg key) {
    vigenereFile(inputFileName, outputFileName, key, DECYPHER);
}

// TO_DO: Get file size (util method)
jms_intg getSizeOfFile(const jms_strg filename) {
    if (filename == NULL) return 0;

    FILE* file = fopen(filename, "rb");
    if (file == NULL) return 0;

    fseek(file, 0, SEEK_END);
    jms_intg size = (jms_intg)ftell(file);
    fclose(file);

    return size;
}
