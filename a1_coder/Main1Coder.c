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
* File name: Main1Coder.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers
* Assignment: A1
* Author: James
* Professor: Paulo Sousa
* Purpose: Entry point for Coder
*************************************************************/

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
#include <ctype.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef CODER_H_
#include "Step1Coder.h"
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

/*
************************************************************
* Coder starting method
* Params:
*	- Mode: Cypher/decypher
*	- Input: Filename
*   - Key: Encrypt word
*	- Output: Filename
************************************************************
*/

// Main function to handle command-line arguments
jms_intg main1Coder(jms_intg argc, jms_strg* argv) {

    if (argc < 5) {
        printf("Error: Not enough arguments.\n");
        return EXIT_FAILURE;
    }

    jms_strg modeStr       = argv[2];
    jms_strg inputFileName  = argv[3];
    jms_strg outputFileName = argv[4];

    jms_intg mode = atoi(modeStr);

    if (mode == CYPHER) {
        cypher(inputFileName, outputFileName, STR_LANGNAME);
    } else if (mode == DECYPHER) {
        decypher(inputFileName, outputFileName, STR_LANGNAME);
    } else {
        errorPrint("Error: Invalid mode. Use 1 or 0\n");
        return EXIT_FAILURE;
    }

    printf("Operation completed successfully using key: %s\n", STR_LANGNAME);

    /* Verification */
    jms_strg result = vigenereMem(outputFileName, STR_LANGNAME, DECYPHER);
    if (result) {
        printf("\n=== Decoded back in memory ===\n%s\n", result);
        free(result);
    }

    return EXIT_SUCCESS;
}