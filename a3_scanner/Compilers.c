/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Spring, 2026
* Author: James
* Professors: Paulo Sousa
************************************************************
#
# ECHO "=---------------------------------------="
# ECHO "|  COMPILERS - ALGONQUIN COLLEGE (F25)  |"
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
# ECHO "    @@         J A M E S           @@   
# ECHO "    @@                             @@   
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@   
# ECHO "                                         "
# ECHO "[COMPILER SCRIPT .......................]"
# ECHO "                                         "
*/
/*
************************************************************
* File name: Compilers.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 Compilers
* Assignment: A1
* Date: May 30 2026
* Professor: Paulo Sousa
* Purpose: Main entry point of the Compilers Project
* Function list: main(), printLogo(), errorPrint()
************************************************************
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

#ifndef READER_H_
#include "Step2Reader.h"
#endif

/*
 * ............................................................................
 * ADVICE 1:
 * Please check the "TODO" labels to develop your activity.
 *
 * ADVICE 2: This file must be updated according to each assignment
 * (see the progression: reader > scanner > parser).
 * ............................................................................
 */

/*
***********************************************************
* Function name: main
* Purpose: Main function
* Author: James
* History/Versions: Ver 1
* Called functions: mainReader(), mainScanner(), mainParser()
* Parameters: Command line arguments - argc, argv
* Return value: Status
* Algorithm: -
*************************************************************
*/

jms_intg main(jms_intg argc, jms_strg* argv) {

    printLogo();

    if (DEBUG) {
        for (jms_intg i = 0; i < argc; ++i)
            printf("argv[%d] = %s\n", i, argv[i]);
    }

    if (argc < 2) {
        printf("%s%c%s%c%s%c%s%c%s%c%s", "OPTIONS:\n* [",
            PGM_CDR, "] - Coder\n* [",
            PGM_RDR, "] - Reader\n* [",
            PGM_SCN, "] - Scanner\n* [",
            PGM_PSR, "] - Parser\n* [",
            PGM_WRT, "] - Writer\n");
        return EXIT_FAILURE;
    }

    jms_char option = argv[1][0];

    switch (option) {
    case PGM_CDR:  /* '1' - Coder */
        printf("\n[Option '1': Starting CODER .....]\n\n");
        main1Coder(argc, argv);
        break;

    case PGM_RDR:
        printf("\n[Option '2': Starting READER .....]\n\n");
        main2Reader(argc, argv);
        break;

    case PGM_SCN:
        printf("\n[Option '3': Starting SCANNER .....]\n\n");
        main3Scanner(argc, argv);
        break;

    default:
        printf("%s%c%s%c%s%c%s%c%s%c%s", "OPTIONS:\n* [",
            PGM_CDR, "] - Coder\n* [",
            PGM_RDR, "] - Reader\n* [",
            PGM_SCN, "] - Scanner\n* [",
            PGM_PSR, "] - Parser\n* [",
            PGM_WRT, "] - Writer\n");
        break;
    }

    return EXIT_SUCCESS;
}

/*
***********************************************************
* Function name: printLogo
* Purpose: Print Logo
* Author: Paulo Sousa
* History/Versions: Ver F25
* Called functions: -
* Parameters: -
* Return value: (Null)
* Algorithm: -
*************************************************************
*/

jms_void printLogo() {
    printf("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
        "\t=---------------------------------------=\n",
        "\t|  COMPILERS - ALGONQUIN COLLEGE (S25)  |\n",
        "\t=---------------------------------------=\n",
        "\t    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    \n",
        "\t    @@                             @@    \n",
        "\t    @@           %&@@@@@@@@@@@     @@    \n",
        "\t    @@       @%% (@@@@@@@@@  @     @@    \n",
        "\t    @@      @& @   @ @       @     @@    \n",
        "\t    @@     @ @ %  / /   @@@@@@     @@    \n",
        "\t    @@      & @ @  @@              @@    \n",
        "\t    @@       @/ @*@ @ @   @        @@    \n",
        "\t    @@           @@@@  @@ @ @      @@    \n",
        "\t    @@            /@@    @@@ @     @@    \n",
        "\t    @@     @      / /     @@ @     @@    \n",
        "\t    @@     @ @@   /@/   @@@ @      @@    \n",
        "\t    @@     @@@@@@@@@@@@@@@         @@    \n",
        "\t    @@                             @@    \n",
        "\t    @@         J A M E S           @@    \n",
        "\t    @@                             @@    \n",
        "\t    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    \n",
        "\t                                         \n",
        "\t[COMPILER SCRIPT .......................]\n",
        "\t                                         \n"
    );
}

/*
************************************************************
* Error printing function with variable number of arguments
* Params: Variable arguments, using formats from C language.
*	- Internal vars use list of arguments and types from stdarg.h
*   - NOTE: The format is using signature from C Language
************************************************************
*/

jms_void errorPrint(jms_strg fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);

    if (strchr(fmt, '\n') == NULL)
        fprintf(stderr, "\n");
}
