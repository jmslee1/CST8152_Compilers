/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Spring, 2026
* Author: James
* Professor: Paulo Sousa
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
* File name: Main5Writer.c
* Compiler: MS Visual Studio
* Course: CST 8152 Compilers
* Assignment: A4.
* Purpose: This file is the Main5Writer.c
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

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef CODER_H_
#include "Step1Coder.h"
#endif

#ifndef WRITER_H_
#include "Step5Writer.h"
#endif

jms_intg main5Writer(jms_intg argc, jms_strg* argv) {
    jms_strg source;
    jms_strg content;
    jms_intg size;

    if (argc < 3) {
        printf("Usage: compilers 5 <encoded_or_source_file>\n");
        return EXIT_FAILURE;
    }

    source = argv[2];
    printf("Reading file %s ....Please wait\n", source);

    size = getSizeOfFile(source);
    content = vigenereMem(source, STR_LANGNAME, DECYPHER);

    if (content == NULL || size <= 0) {
        printf("The input file %s is not completely loaded.\n", source);
        return EXIT_FAILURE;
    }

    process_content(content);
    free(content);

    return EXIT_SUCCESS;
}