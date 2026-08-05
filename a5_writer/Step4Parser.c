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
* File name: Step4Parser.c
* Compiler: MS Visual Studio
* Course: CST 8152 Compilers
* Assignment: A4
* Purpose: This file is the main header
************************************************************
*/

/* TO_DO: Adjust the function header */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef SCANNER_H_
#include "Step3Scanner.h"
#endif

#ifndef PARSER_H_
#include "Step4Parser.h"
#endif

ParserData psData;
jms_intg numParserErrors = 0;

/* Parser data */
extern ParserData psData; /* BNF statistics */

/*
************************************************************
 * Process Parser
 ***********************************************************
 */
/* TO_DO: This is the function to start the parser - check your program definition */

jms_void startParser() {
	/* TO_DO: Initialize Parser data */
	jms_intg i = 0;
	for (i = 0; i < NUM_BNF_RULES; i++) {
		psData.parsHistogram[i] = 0;
	}
	/* Proceed parser */
	lookahead = tokenizer();
	if (lookahead.code != SEOF_T) {
		program();
	}
	matchToken(SEOF_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Source file parsed");
}


/*
 ************************************************************
 * Match Token
 ***********************************************************
 */
/* TO_DO: This is the main code for match - check your definition */
jms_void matchToken(jms_intg tokenCode, jms_intg tokenAttribute) {
	jms_intg matchFlag = JAMES_TRUE;

	if (tokenCode == KW_T) {
		if (lookahead.code != KW_T ||
			lookahead.attribute.codeType != tokenAttribute)
			matchFlag = JAMES_FALSE;
	}
	else {
		if (lookahead.code != tokenCode)
			matchFlag = JAMES_FALSE;
	}

	if (matchFlag && lookahead.code == SEOF_T)
		return;

	if (matchFlag) {
		lookahead = tokenizer();
		if (lookahead.code == ERR_T) {
			printError();
			lookahead = tokenizer();
			syntaxErrorNumber++;
		}
	}
	else {
		syncErrorHandler(tokenCode);
	}
}

/*
 ************************************************************
 * Syncronize Error Handler
 ***********************************************************
 */
/* TO_DO: This is the function to handler error - adjust basically datatypes */
jms_void syncErrorHandler(jms_intg syncTokenCode) {
	printError();
	syntaxErrorNumber++;
	while (lookahead.code != syncTokenCode) {
		if (lookahead.code == SEOF_T)
			exit(syntaxErrorNumber);
		lookahead = tokenizer();
	}
	if (lookahead.code != SEOF_T)
		lookahead = tokenizer();
}

/*
 ************************************************************
 * Print Error
 ***********************************************************
 */
/* TO_DO: This is the function to error printing - adjust basically datatypes */
jms_void printError() {
	Token t = lookahead;
	numParserErrors++;
	printf("%s: Syntax error:  Line:%3d\n", STR_LANGNAME, line);
	printf("*****  Token code:%3d Attribute: ", t.code);
	switch (t.code) {
	case ERR_T:
		printf("*ERROR*: %s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
	case MNID_T:
		printf("MNID_T:\t\t%s\t\n", t.attribute.idLexeme);
		break;
	case STR_T:
		printf("STR_T: %s\n", readerGetContent(stringLiteralTable, t.attribute.contentString));
		break;
	case KW_T:
		printf("KW_T: %s\n", keywordTable[t.attribute.codeType]);
		break;
	case LPR_T:
		printf("LPR_T\n");
		break;
	case RPR_T:
		printf("RPR_T\n");
		break;
	case LBR_T:
		printf("LBR_T\n");
		break;
	case RBR_T:
		printf("RBR_T\n");
		break;
	case EOS_T:
		printf("NA\n");
		break;
	default:
		printf("%s%s%d\n", STR_LANGNAME, ": Scanner error: invalid token code: ", t.code);
		numParserErrors++;
	}
}

/*
 ************************************************************
 * program
 * BNF: <program> → <opt_comments> <rules> SEOF
 * FIRST(<program>) = { CMT_T, if, run, log, enable, disable, set, SEOF }
 ***********************************************************
 */
jms_void program(jms_void) {
	psData.parsHistogram[BNF_program]++;
	optComments();
	rules();
	printf("%s: Program parsed\n", STR_LANGNAME);
}

/*
 ************************************************************
 * optComments
 * BNF: <opt_comments> → <comment> <opt_comments> | ε
 * FIRST = { CMT_T } ∪ { ε }
 ***********************************************************
 */
jms_void optComments(jms_void) {
	psData.parsHistogram[BNF_optComments]++;
	while (lookahead.code == CMT_T) {
		comment();
	}
	printf("%s: Optional comments parsed\n", STR_LANGNAME);
}

/*
 ************************************************************
 * comment
 * BNF: <comment> → CMT_T
 * FIRST = { CMT_T }
 ***********************************************************
 */
jms_void comment(jms_void) {
	psData.parsHistogram[BNF_comment]++;
	matchToken(CMT_T, NO_ATTR);
	printf("%s: Comment parsed\n", STR_LANGNAME);
}

/*
 ************************************************************
 * rules
 * BNF: <rules> → <rule> <rules> | ε
 * FIRST = { if, run, log, enable, disable, set } ∪ { ε }
 ***********************************************************
 */
jms_void rules(jms_void) {
	psData.parsHistogram[BNF_rules]++;
	while (lookahead.code == KW_T &&
		(lookahead.attribute.codeType == KW_if ||
			lookahead.attribute.codeType == KW_run ||
			lookahead.attribute.codeType == KW_log ||
			lookahead.attribute.codeType == KW_enable ||
			lookahead.attribute.codeType == KW_disable ||
			lookahead.attribute.codeType == KW_set)) {
		rule();
	}
	printf("%s: Rules parsed\n", STR_LANGNAME);
}

/*
 ************************************************************
 * rule
 * BNF:
 *   <rule> → if <condition> then <action> ;
 *          | run <identifier> ;
 *          | log <string> ;
 *          | enable <identifier> ;
 *          | disable <identifier> ;
 *          | set <identifier> <number> ;
 * FIRST = { if, run, log, enable, disable, set }
 ***********************************************************
 */
jms_void rule(jms_void) {
	psData.parsHistogram[BNF_rule]++;

	if (lookahead.code != KW_T) {
		printError();
		printf("%s: Rule parsed\n", STR_LANGNAME);
		return;
	}

	switch (lookahead.attribute.codeType) {

	case KW_if:
		matchToken(KW_T, KW_if);
		condition();
		matchToken(KW_T, KW_then);
		action();
		matchToken(EOS_T, NO_ATTR);
		break;

	case KW_run:
		matchToken(KW_T, KW_run);
		matchToken(MNID_T, NO_ATTR);
		matchToken(EOS_T, NO_ATTR);
		break;

	case KW_log:
		matchToken(KW_T, KW_log);
		matchToken(STR_T, NO_ATTR);
		matchToken(EOS_T, NO_ATTR);
		break;

	case KW_enable:
		matchToken(KW_T, KW_enable);
		matchToken(MNID_T, NO_ATTR);
		matchToken(EOS_T, NO_ATTR);
		break;

	case KW_disable:
		matchToken(KW_T, KW_disable);
		matchToken(MNID_T, NO_ATTR);
		matchToken(EOS_T, NO_ATTR);
		break;

	case KW_set:
		matchToken(KW_T, KW_set);
		matchToken(MNID_T, NO_ATTR);
		matchToken(INT_T, NO_ATTR);
		matchToken(EOS_T, NO_ATTR);
		break;

	default:
		printError();
		break;
	}

	printf("%s: Rule parsed\n", STR_LANGNAME);
}

/*
 ************************************************************
 * condition
 * BNF: <condition> → <identifier>
 * FIRST = { MNID_T }
 ***********************************************************
 */
jms_void condition(jms_void) {
	psData.parsHistogram[BNF_condition]++;
	matchToken(MNID_T, NO_ATTR);
	printf("%s: Condition parsed\n", STR_LANGNAME);
}

/*
 ************************************************************
 * action
 * BNF:
 *   <action> → enable <identifier>
 *            | disable <identifier>
 *            | set <identifier> <number>
 *            | run <identifier>
 *            | <identifier>
 * FIRST = { enable, disable, set, run, MNID_T }
 ***********************************************************
 */
jms_void action(jms_void) {
	psData.parsHistogram[BNF_action]++;

	if (lookahead.code == KW_T) {
		switch (lookahead.attribute.codeType) {
		case KW_enable:
			matchToken(KW_T, KW_enable);
			matchToken(MNID_T, NO_ATTR);
			break;
		case KW_disable:
			matchToken(KW_T, KW_disable);
			matchToken(MNID_T, NO_ATTR);
			break;
		case KW_set:
			matchToken(KW_T, KW_set);
			matchToken(MNID_T, NO_ATTR);
			matchToken(INT_T, NO_ATTR);
			break;
		case KW_run:
			matchToken(KW_T, KW_run);
			matchToken(MNID_T, NO_ATTR);
			break;
		default:
			printError();
			break;
		}
	}
	else if (lookahead.code == MNID_T) {
		matchToken(MNID_T, NO_ATTR);
	}
	else {
		printError();
	}

	printf("%s: Action parsed\n", STR_LANGNAME);
}

/*
 ************************************************************
 * The function prints statistics of BNF rules
 * Param:
 *	- Parser data
 * Return:
 *	- Void (procedure)
 ***********************************************************
 */

jms_void printBNFData(ParserData psData) { /* Print Parser statistics */
	printf("Statistics:\n");
	printf("----------------------------------\n");
	int cont = 0;
	for (cont = 0; cont < NUM_BNF_RULES; cont++) {
		if (psData.parsHistogram[cont] > 0)
			printf("%s%s%s%d%s", "Token[", BNFStrTable[cont], "]=", psData.parsHistogram[cont], "\n");
	}
	printf("----------------------------------\n");
}
