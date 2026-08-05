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
* File name: Step5Writer.h
* Compiler: MS Visual Studio
* Course: CST 8152 Compilers
* Assignment: A4.
* Purpose: This file is the Writer.h
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * Please check the "TODO" labels to develop your activity.
 *.............................................................................
 */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef WRITER_H_
#define WRITER_H_

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#define MAX_VARS      100
#define MAX_EXPR_LEN  256
#define MAX_LINES     100

/* J-LANG output keyword */
#define WRITE         "log"

#define EOS           '\0'
#define ZERO          0.0
#define STREMPTY      ""
#define LPAR          '('
#define RPAR          ')'
#define PLUS          '+'
#define MINUS         '-'
#define MULT          '*'
#define DIV           '/'
#define EQUALS        '='
#define SPACE         ' '
#define TAB           '\t'
#define NEWLINE       '\n'
#define RETURN        '\r'
#define QUOTES        '"'
#define QUOTE         '\''

#define TRUE_STR      "true"
#define FALSE_STR     "false"

typedef enum { NUMERIC, STRING, BOOLEAN, CHAR_TYPE } VarType;

typedef struct {
    jms_char name[32];
    VarType  type;
    union {
        jms_doub num_value;
        jms_char str_value[256];
        jms_intg bool_value;
        jms_char char_value;
    } value;
} Variable;

jms_intg find_variable(const jms_strg name);

jms_void assign_boolean_variable(const jms_strg name, jms_intg value);
jms_void assign_char_variable(const jms_strg name, jms_char value);
jms_void assign_numeric_variable(const jms_strg name, jms_doub value);
jms_void assign_string_variable(const jms_strg name, const jms_strg value);

jms_intg get_boolean_value(const jms_strg name);
jms_char get_char_value(const jms_strg name);
jms_doub get_numeric_value(const jms_strg name);
const jms_strg get_string_value(const jms_strg name);

jms_doub parse_term(jms_strg* expr);
jms_doub parse_expression(jms_strg* expr);
jms_void calculate(jms_strg expression);

jms_void handle_write(jms_strg expression);
jms_void process_file(const jms_strg filename);
jms_void process_content(jms_strg content);

#endif