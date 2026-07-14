#define _CRT_SECURE_NO_WARNINGS 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef CODER_H_
#include "Step1Coder.h"
#endif

#ifndef READER_H_
#include "Step2Reader.h"
#endif

 /* Check for ANSI C compliancy */
#define ANSI_C 0
#if defined(__STDC__)
#undef ANSI_C
#define ANSI_C 1
#endif

/*
 * TODO .......................................................................
 * Basically, change all datatypes to your language definitions
 * (see "Compilers.h")
 */

/*
 * -------------------------------------------------------------
 *  Function declarations
 * -------------------------------------------------------------
 */
jms_void displayBuffer(BufferPointer ptr_Buffer);
jms_long getFileSize(jms_strg fname);
jms_intg isNumber(const jms_strg ns);
jms_void startReader(jms_strg, jms_strg, jms_intg, jms_real);

/*
************************************************************
* Main function from Buffer
* Parameters:
*   argc / argv = Parameters from command prompt
* Return value:
*	Success operation.
************************************************************
*/

jms_intg main2Reader(jms_intg argc, jms_strg* argv) {

	/* Create source input buffer */
	jms_strg program = argv[0];
	jms_strg input = argv[2];
	jms_intg size = 0;
	jms_real factor = 0.0f;
	/* Missing file name or/and mode parameter */
	if (argc <= 2) {
		errorPrint("\nDate: %s  Time: %s", __DATE__, __TIME__);
		errorPrint("\nRuntime error at line %d in file %s\n", __LINE__, __FILE__);
		errorPrint("%s\b\b\b\b%s%s", argv[0], ": ", "Missing parameters.");
		errorPrint("Usage: <Option=2> <SourceFile>");
		exit(EXIT_FAILURE);
	}
	/* TODO: Update all other options about parameters */
	startReader(program, input, size, factor);

	/* Return success */
	return (EXIT_SUCCESS);
}

/*
************************************************************
* Buffer starting method
* Params:
*	- Program: Name of the program
*	- Input: Filename
*	- Mode: Operational mode
*	- Size: Buffer capacity
*	- Increment: buffer increment.
************************************************************
*/
jms_void startReader(jms_strg program, jms_strg input, jms_intg size, jms_real factor) {

	BufferPointer bufferp;		/* pointer to Buffer structure */
	jms_intg loadSize = 0;	/* the size of the file loaded in the buffer */

	/* Create buffer */
	bufferp = readerCreate(READER_DEFAULT_SIZE, READER_DEFAULT_FACTOR);

	if (bufferp == NULL) {
		errorPrint("%s%s", program, ": Cannot allocate buffer - Use: buffer <input>.");
		errorPrint("Filename: %s \n", input);
		exit(1);
	}

	/* Load source file into input buffer  */
	printf("Reading file %s ....Please wait\n", input);
	loadSize = readerLoad(bufferp, input);

	/* Sets the checksum */
	readerChecksum(bufferp);

	/* If the input file has not been completely loaded, find the file size and print the last symbol loaded */
	if (loadSize == READER_ERROR) {
		printf("The input file %s %s\n", input, "has not been completely loaded.");
		printf("Current size of buffer: %d.\n", readerGetSize(bufferp));
		printf("Input file size: %ld\n", getFileSize(input));
	}

	/* Finishes the buffer: add end of file character (EOF) to the buffer display again */
	if ((loadSize != READER_ERROR) && (loadSize != 0)) {
		if (!readerAddChar(bufferp, READER_TERMINATOR)) {
			errorPrint("%s%s%s", program, ": ", "Error in compacting buffer.");
		}
	}

	/* Prints the buffer property and content */
	displayBuffer(bufferp);

	/* Free the dynamic memory used by the buffer */
	readerFree(bufferp);
	bufferp = NULL;
}

/*
************************************************************
* Get buffer size
* Params:
*	- Filename: Name of the file
************************************************************
*/

jms_long getFileSize(jms_strg fname) {
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
 * Tests for decimal-digit character string
 * Params:
 *		- String to be evaluated as numeric
 * Return:
 *		- Number value: Returns nonzero (true) if ns is a number; 0 (False) otherwise
************************************************************
*/

jms_intg isNumber(const jms_strg ns) {
	jms_char c; jms_intg i = 0;
	if (ns == NULL) return 0;
	while ((c = ns[i++]) != '\0') {
		if (!isdigit(c)) return 0;
	}
	return 1;
}


/*
************************************************************
* Print function
*	- Params: buffer to print all properties.
************************************************************
*/
jms_void displayBuffer(BufferPointer ptr_Buffer) {
	printf("\nPrinting buffer parameters:\n\n");
	printf("The size of the buffer is:  %d\n",
		readerGetSize(ptr_Buffer));
	printf("The current size of the buffer is:  %d\n",
		readerGetPosWrte(ptr_Buffer));
	printf("The first symbol in the buffer is:  %c\n",
		readerGetPosWrte(ptr_Buffer) ? *readerGetContent(ptr_Buffer, 0) : ' ');
	printf("The value of the flags:\n");
	readerPrintFlags(ptr_Buffer);
	printf("Checksum: %d\n", readerChecksum(ptr_Buffer));
	printf("%s", "Reader statistics : \n");
	readerPrintStat(ptr_Buffer);
	printf("Number of errors: %d\n",
		readerNumErrors(ptr_Buffer));
	printf("\nPrinting buffer contents:\n\n");
	readerRecover(ptr_Buffer);
	if (!readerPrint(ptr_Buffer))
		printf("Empty buffer\n");
}
