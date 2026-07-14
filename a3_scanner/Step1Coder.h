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
* File name: Step1Coder.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers
* Assignment: A1
* Author: James
* Professor: Paulo Sousa
* Purpose: Header for Coder (Vigenère Cipher)
*************************************************************/

/*
 *.............................................................................
 * MAIN ADVICE:
 * Please check the "TODO" labels to develop your activity.
 *.............................................................................
 */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef CODER_H_
#define CODER_H_

 // ASCII range for visible characters
#define ASCII_START 32
#define ASCII_END 126
#define ASCII_RANGE (ASCII_END - ASCII_START + 1)

#define CYPHER 1
#define DECYPHER 0

/* FUNCTIONS DECLARATION:  .................................. */

/* General Operations */
void vigenereFile(const jms_strg inputFileName, const jms_strg outputFileName, const jms_strg key, jms_intg encode);

jms_strg vigenereMem(const jms_strg inputFileName, const jms_strg key, jms_intg encode);

void cypher(const jms_strg inputFileName, const jms_strg outputFileName, const jms_strg key);

void decypher(const jms_strg inputFileName, const jms_strg outputFileName, const jms_strg key);

jms_intg getSizeOfFile(const jms_strg filename);

#endif
