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
* File name: Compilers.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 Compilers, Lab Section: [011, 012]
* Assignment: A1-A5.
* Date: May 30 2026
* Professor: Paulo Sousa
* Purpose: This file defines the functions called by main function.
* Function list: mainReader(), mainScanner(), mainParser().
*************************************************************/

#ifndef COMPILERS_H_
#define COMPILERS_H_

#define DEBUG 0

/*
 * ............................................................................
 * ADVICE 1:
 * Please check the "TODO" labels to develop your activity.
 *
 * ADVICE 2: This file must be updated according to each assignment
 * (see the progression: reader > scanner > parser).
 * ............................................................................
 */

/* TO_DO: Adjust your language (cannot be "Sofia") */
/* Language name */
#define STR_LANGNAME	"James"

/* Logical constants - adapt for your language */
#define JAMES_TRUE  1
#define JAMES_FALSE 0

#define INVALID NULL
#define EOF_CHAR '\0'

/*
------------------------------------------------------------
Data types definitions
NOTE: Some types may not be directly used by your language,
		but they can be necessary for conversions.
------------------------------------------------------------
*/

/* TO_DO: Define your typedefs */
typedef char			jms_char;
typedef char*			jms_strg;
typedef int				jms_intg;
typedef float			jms_real;
typedef void			jms_void;

typedef unsigned char	jms_boln;
typedef unsigned char	jms_byte;

typedef long			jms_long;
typedef double			jms_doub;


/* A2 Reader */
/*---------------------------------------------------------*/
#define NCHAR                   128
#define READER_DEFAULT_SIZE     100
#define READER_DEFAULT_FACTOR   0.5f
#define READER_MAX_SIZE         (INT_MAX - 1)
#define READER_ERROR            (-1)
#define READER_TERMINATOR       '\0'

/* Flag struct */
typedef struct flag {
	jms_boln isEmpty;
	jms_boln isFull;
	jms_boln isRead;
	jms_boln isMoved;
} Flag;

/* Position struct */
typedef struct position {
	jms_intg mark;
	jms_intg read;
	jms_intg wrte;
} Position;

/* Main Reader/Buffer struct */
typedef struct bufferReader {
	jms_strg  content;
	jms_intg  size;
	jms_real  factor;
	Flag      flags;
	Position  position;
	jms_intg  histogram[NCHAR];
	jms_intg  numReaderErrors;
	jms_intg  checkSum;
} Buffer, * BufferPointer;

/*
------------------------------------------------------------
Programs:
1: Coder - invokes Main1Coder code
2: Reader - invokes Main2Reader code
3: Scanner - invokes Main3Scanner code
4: Parser - invokes Main4Parser code
5: Writer - invokes Main5Writer code
------------------------------------------------------------
*/
enum PROGRAMS {
	PGM_CDR = '1', // Coder:	05pt
	PGM_RDR = '2', // Reader:	05pt
	PGM_SCN = '3', // Scanner:	15pt
	PGM_PSR = '4', // Parser:	15pt
	PGM_WRT = '5', // Writer:	10pt
};

/*
------------------------------------------------------------
Main functions signatures
(Code will be updated during next assignments)
------------------------------------------------------------
*/
jms_intg main1Coder	(jms_intg argc, jms_strg* argv);
jms_intg main2Reader(jms_intg argc, jms_strg* argv);
jms_intg main3Scanner(jms_intg argc, jms_strg* argv);

jms_void printLogo();
jms_void errorPrint(jms_strg fmt, ...);

#endif

/*
	  "args": [
		1,
		1,
		"README.txt",
		"CODED.txt"
	  ]
	  "args": [
		4,
		"CODED.txt"
	  ]
*/
