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
* File name: Step5Writer.c
* Compiler: MS Visual Studio
* Course: CST 8152 Compilers
* Assignment: A4.
* Purpose: This file is the Writer.c
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * - Please check the "TODO" labels to develop your activity.
 * - Review the functions to use "Defensive Programming".
 *.............................................................................
 */

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef WRITER_H_
#include "Step5Writer.h"
#endif

/* Global variables */
Variable variables[MAX_VARS];
jms_intg var_count = 0;
jms_intg initial_phase = 1;
jms_char output_buffer[MAX_EXPR_LEN * 10] = { 0 };

/* ---------- symbol table ---------- */

jms_intg find_variable(const jms_strg name) {
    jms_intg i;
    if (name == NULL) return -1;
    for (i = 0; i < var_count; ++i) {
        if (strcmp(variables[i].name, name) == 0)
            return i;
    }
    return -1;
}

jms_void assign_numeric_variable(const jms_strg name, jms_doub value) {
    jms_intg idx;
    if (name == NULL) return;
    idx = find_variable(name);
    if (idx == -1) {
        if (var_count >= MAX_VARS) return;
        idx = var_count++;
        strncpy(variables[idx].name, name, 31);
        variables[idx].name[31] = EOS;
    }
    variables[idx].type = NUMERIC;
    variables[idx].value.num_value = value;
}

jms_void assign_string_variable(const jms_strg name, const jms_strg value) {
    jms_intg idx;
    if (name == NULL || value == NULL) return;
    idx = find_variable(name);
    if (idx == -1) {
        if (var_count >= MAX_VARS) return;
        idx = var_count++;
        strncpy(variables[idx].name, name, 31);
        variables[idx].name[31] = EOS;
    }
    variables[idx].type = STRING;
    strncpy(variables[idx].value.str_value, value, 255);
    variables[idx].value.str_value[255] = EOS;
}

jms_void assign_boolean_variable(const jms_strg name, jms_intg value) {
    jms_intg idx;
    if (name == NULL) return;
    idx = find_variable(name);
    if (idx == -1) {
        if (var_count >= MAX_VARS) return;
        idx = var_count++;
        strncpy(variables[idx].name, name, 31);
        variables[idx].name[31] = EOS;
    }
    variables[idx].type = BOOLEAN;
    variables[idx].value.bool_value = value ? 1 : 0;
}

jms_void assign_char_variable(const jms_strg name, jms_char value) {
    jms_intg idx;
    if (name == NULL) return;
    idx = find_variable(name);
    if (idx == -1) {
        if (var_count >= MAX_VARS) return;
        idx = var_count++;
        strncpy(variables[idx].name, name, 31);
        variables[idx].name[31] = EOS;
    }
    variables[idx].type = CHAR_TYPE;
    variables[idx].value.char_value = value;
}

jms_doub get_numeric_value(const jms_strg name) {
    jms_intg idx = find_variable(name);
    if (idx != -1 && variables[idx].type == NUMERIC)
        return variables[idx].value.num_value;
    return ZERO;
}

const jms_strg get_string_value(const jms_strg name) {
    jms_intg idx = find_variable(name);
    if (idx != -1 && variables[idx].type == STRING)
        return variables[idx].value.str_value;
    return STREMPTY;
}

jms_intg get_boolean_value(const jms_strg name) {
    jms_intg idx = find_variable(name);
    if (idx != -1 && variables[idx].type == BOOLEAN)
        return variables[idx].value.bool_value;
    return 0;
}

jms_char get_char_value(const jms_strg name) {
    jms_intg idx = find_variable(name);
    if (idx != -1 && variables[idx].type == CHAR_TYPE)
        return variables[idx].value.char_value;
    return EOS;
}

/* ---------- expression evaluator ---------- */

jms_doub parse_term(jms_strg* expr) {
    jms_doub value = 0.0;

    if (expr == NULL || *expr == NULL) return ZERO;

    while (isspace((unsigned char)**expr)) (*expr)++;

    if (**expr == LPAR) {
        (*expr)++;
        value = parse_expression(expr);
        if (**expr == RPAR) (*expr)++;
    }
    else if (isalpha((unsigned char)**expr) || **expr == '_') {
        jms_char var_name[32] = { 0 };
        jms_intg i = 0;
        while ((isalnum((unsigned char)**expr) || **expr == '_') && i < 31) {
            var_name[i++] = *(*expr)++;
        }
        var_name[i] = EOS;
        value = get_numeric_value(var_name);
    }
    else {
        value = strtod(*expr, expr);
    }

    while (isspace((unsigned char)**expr)) (*expr)++;

    while (**expr == MULT || **expr == DIV) {
        jms_char op = *(*expr)++;
        jms_doub next_value = parse_term(expr);
        if (op == MULT)
            value *= next_value;
        else {
            if (next_value != 0.0)
                value /= next_value;
            else {
                printf("Error: division by zero!\n");
                value = 0.0;
            }
        }
    }
    return value;
}

jms_doub parse_expression(jms_strg* expr) {
    jms_doub value;

    if (expr == NULL || *expr == NULL) return ZERO;

    value = parse_term(expr);
    while (isspace((unsigned char)**expr)) (*expr)++;

    while (**expr == PLUS || **expr == MINUS) {
        jms_char op = *(*expr)++;
        jms_doub next_value = parse_term(expr);
        if (op == PLUS)
            value += next_value;
        else
            value -= next_value;
    }
    return value;
}

/* ---------- write / log ---------- */

jms_void handle_write(jms_strg expression) {
    jms_char buffer[MAX_EXPR_LEN] = { 0 };
    jms_strg start;
    jms_strg end;

    if (expression == NULL) return;

    start = strchr(expression, LPAR);
    end = strrchr(expression, RPAR);
    if (start == NULL || end == NULL || start >= end) return;

    start++;
    *end = EOS;

    while (*start != EOS) {
        if (*start == QUOTES) {
            start++;
            while (*start != QUOTES && *start != EOS) {
                jms_char tmp[2] = { *start, EOS };
                strcat(buffer, tmp);
                start++;
            }
            if (*start == QUOTES) start++;
        }
        else if (isalpha((unsigned char)*start) || *start == '_') {
            jms_char var_name[32] = { 0 };
            jms_intg i = 0;
            jms_intg var_idx;

            while ((isalnum((unsigned char)*start) || *start == '_') && i < 31) {
                var_name[i++] = *start++;
            }
            var_name[i] = EOS;

            var_idx = find_variable(var_name);
            if (var_idx != -1) {
                if (variables[var_idx].type == STRING)
                    strcat(buffer, get_string_value(var_name));
                else if (variables[var_idx].type == NUMERIC) {
                    jms_char num_str[64];
                    sprintf(num_str, "%.2f", get_numeric_value(var_name));
                    strcat(buffer, num_str);
                }
                else if (variables[var_idx].type == BOOLEAN)
                    strcat(buffer, get_boolean_value(var_name) ? TRUE_STR : FALSE_STR);
                else if (variables[var_idx].type == CHAR_TYPE) {
                    jms_char char_str[2] = { get_char_value(var_name), EOS };
                    strcat(buffer, char_str);
                }
            }
        }
        else if (isspace((unsigned char)*start)) {
            jms_char tmp[2] = { *start, EOS };
            strcat(buffer, tmp);
            start++;
        }
        else {
            start++; /* skip commas etc. */
        }
    }

    if (initial_phase) {
        if (strlen(buffer) == 0)
            strcat(output_buffer, "[Undefined]");
        else
            strcat(output_buffer, buffer);
        strcat(output_buffer, "\n");
    }
    else {
        printf("%s\n", buffer);
    }
}

/* ---------- line dispatcher ---------- */

jms_void calculate(jms_strg expression) {
    jms_char var_name[32] = { 0 };
    jms_strg eq;
    jms_strg expr;

    if (expression == NULL) return;
    while (isspace((unsigned char)*expression)) expression++;
    if (*expression == EOS || *expression == '#') return; /* empty / comment */

    /* Output: log(...)  or  log ... */
    if (strstr(expression, WRITE) == expression || strstr(expression, WRITE) != NULL) {
        if (strchr(expression, LPAR) != NULL) {
            handle_write(expression);
            return;
        }
    }

    eq = strchr(expression, EQUALS);
    if (eq != NULL) {
        jms_intg i = 0;
        jms_strg p = expression;
        while (p < eq && (isalnum((unsigned char)*p) || *p == '_') && i < 31)
            var_name[i++] = *p++;
        var_name[i] = EOS;

        expr = eq + 1;
        while (isspace((unsigned char)*expr)) expr++;

        if (*expr == QUOTES) {
            jms_char str_value[256] = { 0 };
            jms_intg k = 0;
            expr++;
            while (*expr != QUOTES && *expr != EOS && k < 255)
                str_value[k++] = *expr++;
            str_value[k] = EOS;
            assign_string_variable(var_name, str_value);
            if (!initial_phase)
                printf("%s = \"%s\"\n", var_name, str_value);
        }
        else if (strncmp(expr, TRUE_STR, 4) == 0 || strncmp(expr, FALSE_STR, 5) == 0) {
            jms_intg bool_value = (strncmp(expr, TRUE_STR, 4) == 0) ? 1 : 0;
            assign_boolean_variable(var_name, bool_value);
            if (!initial_phase)
                printf("%s = %s\n", var_name, bool_value ? TRUE_STR : FALSE_STR);
        }
        else if (*expr == QUOTE) {
            jms_char char_value;
            expr++;
            char_value = *expr;
            assign_char_variable(var_name, char_value);
            if (!initial_phase)
                printf("%s = '%c'\n", var_name, char_value);
        }
        else {
            jms_doub value = parse_expression(&expr);
            assign_numeric_variable(var_name, value);
            if (!initial_phase)
                printf("%s = %.2f\n", var_name, value);
        }
        return;
    }

    /* bare expression */
    {
        jms_strg e = expression;
        jms_doub result = parse_expression(&e);
        if (!initial_phase)
            printf("Result: %.2f\n", result);
    }
}

/* ---------- file / content drivers ---------- */

static jms_void print_all_variables(jms_void) {
    jms_intg i;
    printf("\nVariable values:\n");
    for (i = 0; i < var_count; i++) {
        if (variables[i].type == STRING)
            printf("%s = \"%s\"\n", variables[i].name, variables[i].value.str_value);
        else if (variables[i].type == NUMERIC)
            printf("%s = %.2f\n", variables[i].name, variables[i].value.num_value);
        else if (variables[i].type == BOOLEAN)
            printf("%s = %s\n", variables[i].name,
                variables[i].value.bool_value ? TRUE_STR : FALSE_STR);
        else if (variables[i].type == CHAR_TYPE)
            printf("%s = '%c'\n", variables[i].name, variables[i].value.char_value);
    }
}

jms_void process_file(const jms_strg filename) {
    FILE* file;
    jms_char line[MAX_EXPR_LEN];

    if (filename == NULL) return;
    file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    var_count = 0;
    initial_phase = 1;
    output_buffer[0] = EOS;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = EOS;
        if (line[0] == EOS) continue;
        calculate(line);
    }
    fclose(file);

    initial_phase = 0;
    printf("Outputs from file:\n%s", output_buffer);
    print_all_variables();
}

jms_void process_content(jms_strg content) {
    jms_strg line_start;
    jms_strg line_end;
    jms_char line[MAX_EXPR_LEN];

    if (content == NULL) return;

    var_count = 0;
    initial_phase = 1;
    output_buffer[0] = EOS;

    line_start = content;
    while (*line_start != EOS) {
        jms_intg len;
        line_end = line_start;
        while (*line_end != EOS && *line_end != NEWLINE && *line_end != RETURN)
            line_end++;

        len = (jms_intg)(line_end - line_start);
        if (len >= MAX_EXPR_LEN) len = MAX_EXPR_LEN - 1;
        if (len > 0) {
            strncpy(line, line_start, len);
            line[len] = EOS;
            calculate(line);
        }
        if (*line_end == RETURN) line_end++;
        if (*line_end == NEWLINE) line_end++;
        line_start = line_end;
    }

    initial_phase = 0;
    printf("Outputs from file:\n%s", output_buffer);
    print_all_variables();
}