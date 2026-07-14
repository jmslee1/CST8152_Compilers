/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Summer, 2026
* Author: TO_DO
* Professors: Paulo Sousa
************************************************************

/*
************************************************************
* File name: MainScanner.c
* Compiler: MS Visual Studio 2026
* Course: CST 8152 � Compilers, Lab Section: [301]
* Assignment: A1, A2.
* Date: Jul 01 2026
* Purpose: This file is the main code for Scanner (A22)
* Function list: (...).
************************************************************
*/

/*
 *.............................................................................
 * ADVICE 1:
 * Please check the "TODO" labels to develop your activity.
 *
 * ADVICE 2: Preprocessor directives
 * The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
 * to suppress the warnings about using "unsafe" functions like fopen()
 * and standard sting library functions defined in string.h.
 * The define directive does not have any effect on other compiler projects
 * (Gcc, VSCode, Codeblocks, etc.).
 *.............................................................................
 */

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef BUFFER_H_
#include "Step2Reader.h"
#endif

#ifndef SCANNER_H_
#include "Step3Scanner.h"
#endif

 /*check for ANSI C compliancy */
#define ANSI_C 0
#if defined(__STDC__)
#undef ANSI_C
#define ANSI_C 1
#endif

/*
 * -------------------------------------------------------------
 *  Global vars and External vars
 * -------------------------------------------------------------
 */

 /* Global objects - variables (used in other codes as external) */
BufferPointer stringLiteralTable;	/* This buffer implements String Literal Table */
jms_intg errorNumber;				/* Run-time error number = 0 by default (ANSI) */

/* External objects */
extern jms_intg line; /* Source code line numbers - defined in scanner.c */
extern Token tokenizer(jms_void);

/*
 * -------------------------------------------------------------
 *  Function declarations
 * -------------------------------------------------------------
 */
jms_void printScannerError(jms_strg fmt, ...);
jms_void displayScanner(BufferPointer ptrBuffer);
jms_long getScannerFilesize(jms_strg fname);
jms_void printToken(Token t);

/*
************************************************************
 *  Scanner Main function
 * Parameters:
 *   argc / argv = Parameters from command prompt
 * Return value:
 *	Success operation.
 ***********************************************************
 */


jms_intg main3Scanner(jms_intg argc, jms_strg* argv) {

    BufferPointer sourceBuffer;          /* Pointer to input (source) buffer */
    Token currentToken;                  /* Token produced by the scanner */
    jms_intg loadSize = 0;               /* The size of the file loaded in the buffer */

    /* Check for correct arguments - source file name */
    if (argc <= 2) {
        printScannerError("Date: %s  Time: %s", __DATE__, __TIME__);
        printScannerError("Runtime error at line %d in file %s", __LINE__, __FILE__);
        printScannerError("%s%s", argv[0], ": Missing source file name.");
        printScannerError("%s", "Usage: <Option=3> <SourceFile>");
        exit(EXIT_FAILURE);
    }

    /* Shows debug mode */
    printf("%s%d%s", "[Debug mode: ", DEBUG, "]\n");

    /* Create a source code input buffer */
    sourceBuffer = readerCreate(READER_DEFAULT_SIZE, READER_DEFAULT_FACTOR);
    if (sourceBuffer == NULL) {
        printScannerError("%s%s", argv[0], ": Could not create source buffer");
        exit(EXIT_FAILURE);
    }

    jms_strg sourceFile = argv[2];

    /* === Load file directly (NO decryption) === */
    printf("Reading file %s ....Please wait\n", sourceFile);

    FILE* fileHandler = fopen(sourceFile, "r");
    if (fileHandler == NULL) {
        printScannerError("%s%s%s", argv[0], ": Cannot open file: ", sourceFile);
        exit(EXIT_FAILURE);
    }

    jms_char ch;
    while ((ch = fgetc(fileHandler)) != EOF) {
        if (!readerAddChar(sourceBuffer, ch)) {
            printScannerError("Error: Failed to load file into buffer");
            fclose(fileHandler);
            exit(EXIT_FAILURE);
        }
    }
    fclose(fileHandler);

    loadSize = readerGetPosWrte(sourceBuffer);

    /* Add end of file marker */
    if (!readerAddChar(sourceBuffer, READER_TERMINATOR)) {
        printScannerError("Error: Failed to add terminator to buffer");
        exit(EXIT_FAILURE);
    }

    /* Display buffer contents */
    displayScanner(sourceBuffer);

    /* Create string literal table */
    stringLiteralTable = readerCreate(READER_DEFAULT_SIZE, READER_DEFAULT_FACTOR);
    if (stringLiteralTable == NULL) {
        printScannerError("%s%s", argv[0], ": Could not create string literals buffer");
        exit(EXIT_FAILURE);
    }

    /* Initialize scanner */
    if (startScanner(sourceBuffer)) {
        printScannerError("%s%s", argv[0], ": Empty program buffer - scanning canceled");
        exit(EXIT_FAILURE);
    }

    printf("\nScanning source file...\n\n");
    printf("Token\t\tAttribute\n");
    printf("----------------------------------\n");

    do {
        currentToken = tokenizer();
        printToken(currentToken);
    } while (currentToken.code != SEOF_T);

    /* Print string literal table */
    printf("\nPrinting string table...\n");
    printf("----------------------------------\n");
    if (readerGetPosWrte(stringLiteralTable)) {
        readerPrint(stringLiteralTable);
    }
    printf("\n----------------------------------\n");

    readerRestore(sourceBuffer);
    readerRestore(stringLiteralTable);
    sourceBuffer = stringLiteralTable = NULL;

    printScannerData(scData);

    /* Optional: print number of lines if third argument is 'l' */
    if (argc > 3 && argv[3] != NULL && *argv[3] == 'l')
        printf("The number of lines is: %d\n", line);

    return (EXIT_SUCCESS);
}



/*
************************************************************
 *  Error printing function with variable number of arguments
 *  Params: Variable arguments, using formats from C language.
 *	 - Internal vars use list of arguments and types from stdarg.h
 *   - NOTE: The format is using signature from C Language
***********************************************************
*/

jms_void printScannerError(jms_strg fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	(void)vfprintf(stderr, fmt, ap);
	va_end(ap);
	/* Move to new line */
	if (strchr(fmt, '\n') == NULL)
		fprintf(stderr, "\n");
}

/*
************************************************************
 * The function displays buffer contents
 * Param:
 *		- Scanner to be displayed.
 ***********************************************************
 */

jms_void displayScanner(BufferPointer ptrBuffer) {
	printf("\nPrinting buffer parameters:\n\n");
	printf("The capacity of the buffer is:  %d\n", readerGetSize(ptrBuffer));
	printf("The current size of the buffer is:  %d\n", readerGetPosWrte(ptrBuffer));
	printf("\nPrinting buffer contents:\n\n");
	readerRecover(ptrBuffer);
	readerPrint(ptrBuffer);
}

/*
 ************************************************************
 * The function gets size of scanner file
 * Param:
 *	- Filename
 * Return:
 *	- Size of the file
 ***********************************************************
 */

jms_long getScannerFilesize(jms_strg fname) {
	FILE* fileInput;
	jms_long fileLength;
	fileInput = fopen(fname, "r");
	if (fileInput == NULL) {
		printScannerError("%s%s", "Cannot open file: ", fname);
		return 0L;
	}
	fseek(fileInput, 0L, SEEK_END);
	fileLength = ftell(fileInput);
	fclose(fileInput);
	return fileLength;
}
