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
# ECHO "     _           _          _      _   _    ____   "
# ECHO "    | |         | |        / \    | \ | |  / ___|  "
# ECHO " _  | |  _____  | |       / _ \   |  \| | | |  _   "
# ECHO "| |_| | |_____| | |___   / ___ \  | |\  | | |_| |  "
# ECHO " \___/          |_____| /_/   \_\ |_| \_|  \____|  "
# ECHO "                                                   "
# ECHO "                                                   "
# ECHO "[CODER SCRIPT ....................................]"
# ECHO "                                                   "
*/

/*
************************************************************
 * IMPORTANT NOTE:
 * The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
 * to suppress the warnings about using "unsafe" functions like fopen()
 * and standard sting library functions defined in string.h.
 * The define does not have any effect in other compilers  projects.
 *********************************************************
 */

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <stdarg.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#include "Step2Reader.h"
#endif

#ifndef SCANNER_H_
#include "Step3Scanner.h"
#endif

#ifndef PARSER_H_
#include "Step4Parser.h"
#endif

/* Check for ANSI C compliancy */
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

 /* Global objects - variables */
static BufferPointer sourceBuffer; /* pointer to input (source) buffer */
BufferPointer stringLiteralTable; /* This buffer is used as a repository for string literals */
jms_intg errorNumber;     /* Run-time error number = 0 by default (ANSI) */

/* External objects */
extern jms_intg syntaxErrorNumber /* number of syntax errors reported by the parser */;
extern jms_intg line; /* source code line number - defined in scanner.c */

extern ParserData psData;

/*
 * -------------------------------------------------------------
 *  Function declarations
 * -------------------------------------------------------------
 */

 /* Function declarations (prototypes) */
extern jms_void startParser(jms_void);
extern jms_intg startScanner(BufferPointer sc_buf);

static jms_void displayParser(BufferPointer ptrBuffer);
static jms_long getParserFileSize(jms_char* fname);
static jms_void callGarbageCollector(jms_void);

/*
************************************************************
 *  Parser Main function
 *  Parameters:
 *  - argc / argv = Parameters from command prompt
 *  Return value:
 *	- Success operation.
***********************************************************
*/

 jms_intg main4Parser(jms_intg argc, jms_strg* argv) {

	numParserErrors = 0;

	jms_intg loadsize = 0;
	jms_strg source;

	if (argc <= 2) {
		/* __DATE__, __TIME__, __LINE__, __FILE__ are predefined preprocessor macros*/
		errorPrint("Date: %s  Time: %s", __DATE__, __TIME__);
		errorPrint("Runtime error at line %d in file %s", __LINE__, __FILE__);
		errorPrint("%s%s%s", argv[0], ": ", "Missing source file name.");
		errorPrint("%s%s%s", "Usage: ", "parser", "  source_file_name");
		exit(EXIT_FAILURE);
	}

	/* create a source code input buffer - multiplicative mode */
	sourceBuffer = readerCreate(READER_DEFAULT_SIZE, READER_DEFAULT_FACTOR);
	if (sourceBuffer == NULL) {
		errorPrint("%s%s%s", argv[0], ": ", "Could not create source buffer");
		exit(EXIT_FAILURE);
	}

	source = argv[2];

	/* load source file into input buffer  */
	printf("Reading file %s ....Please wait\n", argv[2]);
	loadsize = readerLoad(sourceBuffer, source);

	if (loadsize == READER_ERROR) {
		printf("The input file %s %s\n", argv[2], "is not completely loaded.");
		printf("Input file size: %ld\n", getParserFileSize(argv[2]));
	}
	/* Add SEOF (EOF) to input buffer and display the source buffer */
	if ((loadsize != READER_ERROR) && (loadsize != 0)) {
		if (readerAddChar(sourceBuffer, READER_TERMINATOR)) {
			displayParser(sourceBuffer);
		}
	}
	/* create string Literal Table */
	stringLiteralTable = readerCreate(READER_DEFAULT_SIZE, READER_DEFAULT_FACTOR);
	if (stringLiteralTable == NULL) {
		errorPrint("%s%s%s", argv[0], ": ", "Could not create string literal buffer");
		exit(EXIT_FAILURE);
	}

	/* Registrer exit function */
	atexit(callGarbageCollector);

	/* Initialize scanner  */
	startScanner(sourceBuffer);

	/* Start parsing */
	printf("\nParsing the source file...\n\n");
	startParser();

	printf("\nNumber of Parser errors: %d\n", numParserErrors);

	/* Prints the statistics */
	printBNFData(psData);

	return (EXIT_SUCCESS);

}

/*
************************************************************
* The function return the size of an open file
* Param:
*	- Filename
* Return:
*	- Size of the file
************************************************************
*/

 jms_long getParserFileSize(jms_char* fname) {
	FILE* input;
	jms_long flength;
	input = fopen(fname, "r");
	if (input == NULL) {
		errorPrint("%s%s", "Cannot open file: ", fname);
		return 0;
	}
	fseek(input, 0L, SEEK_END);
	flength = ftell(input);
	fclose(input);
	return flength;
}

/*
************************************************************
* The function display buffer contents
* Param:
*	- Parser to be displayed.
************************************************************
*/

jms_void displayParser(BufferPointer ptrBuffer) {
	printf("\nPrinting input buffer parameters:\n\n");
	printf("The capacity of the buffer is:  %d\n", readerGetSize(ptrBuffer));
	printf("The current size of the buffer is:  %d\n", readerGetPosWrte(ptrBuffer));
	printf("\nPrinting input buffer contents:\n\n");
	readerRecover(ptrBuffer);
	readerPrint(ptrBuffer);
}

/*
************************************************************
* The function frees all dynamically allocated memory.
* This function is always called despite how the program terminates - normally or abnormally.
************************************************************
*/

jms_void callGarbageCollector(jms_void) {
	if (syntaxErrorNumber)
		printf("\nSyntax errors: %d\n", syntaxErrorNumber);
	printf("\nCollecting garbage...\n");
	readerFree(sourceBuffer);
	readerFree(stringLiteralTable);
}