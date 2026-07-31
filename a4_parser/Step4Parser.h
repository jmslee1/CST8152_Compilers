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
* File name: Step4Parser.h
* Compiler: MS Visual Studio
* Course: CST 8152 Compilers
* Assignment: A4.
* Purpose: This file is the main header
************************************************************
*/

#ifndef PARSER_H_
#define PARSER_H_

/* Include section */
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

/* Global vars */
static Token			lookahead;
extern BufferPointer	stringLiteralTable;
extern jms_intg			line;
extern Token			tokenizer(jms_void);
extern jms_strg			keywordTable[KWT_SIZE];
static jms_intg			syntaxErrorNumber = 0;

/* TO_DO: Create ALL constants for keywords (sequence given in table.h) */

/* Constants */
enum KEYWORDS {
	NO_ATTR = -1,
	KW_if = 0,        /* "if" */
	KW_then,          /* 1 "then" */
	KW_else,          /* 2 "else" */
	KW_when,          /* 3 "when" */
	KW_rule,          /* 4 "rule" */
	KW_enable,        /* 5 "enable" */
	KW_disable,       /* 6 "disable" */
	KW_set,           /* 7 "set" */
	KW_run,           /* 8 "run" */
	KW_log,           /* 9 "log" */
	KW_activate       /* 10 "activate" */
};

/* TO_DO: Define the number of BNF rules */
#define NUM_BNF_RULES 10

/* Parser */
typedef struct parserData {
jms_intg parsHistogram[NUM_BNF_RULES];
} ParserData, * pParsData;

/* Number of errors */
extern jms_intg numParserErrors;

/* Scanner data */
extern ParserData psData;

/* Function definitions */
jms_void startParser(jms_void);
jms_void matchToken(jms_intg tokenCode, jms_intg tokenAttribute);
jms_void syncErrorHandler(jms_intg syncTokenCode);
jms_void printError(jms_void);
jms_void printBNFData(ParserData data);

/* List of BNF statements */
enum BNF_RULES {
	BNF_error = 0,       /* 0 */
	BNF_program,         /* 1 */
	BNF_optComments,     /* 2 */
	BNF_comment,         /* 3 */
	BNF_rules,           /* 4 */
	BNF_rule,            /* 5 */
	BNF_condition,       /* 6 */
	BNF_action,          /* 7 */
	BNF_identifier,      /* 8 */
	BNF_string           /* 9 */
};

/* TO_DO: Define the list of keywords */
static jms_strg BNFStrTable[NUM_BNF_RULES] = {
	"BNF_error",
	"BNF_program",
	"BNF_optComments",
	"BNF_comment",
	"BNF_rules",
	"BNF_rule",
	"BNF_condition",
	"BNF_action",
	"BNF_identifier",
	"BNF_string"
};

/* TO_DO: Place ALL non-terminal function declarations */
jms_void program(jms_void);
jms_void optComments(jms_void);
jms_void comment(jms_void);
jms_void rules(jms_void);
jms_void rule(jms_void);
jms_void condition(jms_void);
jms_void action(jms_void);

#endif